#include "network_service.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "app_config.h"
#include "dht22_service.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "mpu6050_service.h"
#include "threshold_service.h"
#include "nvs_flash.h"

#define WIFI_CONNECTED_BIT BIT0
#define MQTT_CONNECTED_BIT BIT1
#define WIFI_RETRY_BIT BIT2
#define NETWORK_STACK_SIZE 6144U
#define NETWORK_PRIORITY 5U
#define JSON_SIZE 256U
#define NVS_NAMESPACE "vehicle"
#define NVS_HOST_KEY "mqtt_host"
#define NVS_PORT_KEY "mqtt_port"
#define MESSAGE_SIZE JSON_SIZE

typedef struct {
    char topic[64];
    char payload[JSON_SIZE];
    bool alert;
} network_message_t;

static QueueHandle_t g_message_queue;

static const char *TAG = "network";
static EventGroupHandle_t g_network_events;
static esp_mqtt_client_handle_t g_mqtt_client;
static bool g_initialized;
static uint32_t g_retry_delay_ms;
static uint32_t g_publish_failures;

uint32_t network_service_publish_failures(void)
{
    return g_publish_failures;
}
static char g_broker_host[64];
static uint32_t g_broker_port;
static threshold_config_t g_thresholds;
static bool g_alert_active;

static void network_save_defaults(void)
{
    nvs_handle_t handle;
    esp_err_t result;
    uint32_t port;
    size_t size;

    result = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "NVS open failed: %s", esp_err_to_name(result));
        return;
    }
    size = 0U;
    result = nvs_get_str(handle, NVS_HOST_KEY, NULL, &size);
    if (result == ESP_ERR_NVS_NOT_FOUND) {
        (void)nvs_set_str(handle, NVS_HOST_KEY, APP_MQTT_BROKER_HOST);
    }
    result = nvs_get_u32(handle, NVS_PORT_KEY, &port);
    if ((result == ESP_ERR_NVS_NOT_FOUND) || (port == 0U) || (port > 65535U)) {
        (void)nvs_set_u32(handle, NVS_PORT_KEY, APP_MQTT_BROKER_PORT);
    }
    (void)nvs_commit(handle);
    nvs_close(handle);
}

static void network_load_endpoint(char *host, size_t host_size, uint32_t *port)
{
    nvs_handle_t handle;
    size_t size;
    esp_err_t result;

    (void)snprintf(host, host_size, "%s", APP_MQTT_BROKER_HOST);
    *port = APP_MQTT_BROKER_PORT;
    result = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (result != ESP_OK) {
        return;
    }
    size = host_size;
    result = nvs_get_str(handle, NVS_HOST_KEY, host, &size);
    if ((result != ESP_OK) || (host[0] == '\0')) {
        ESP_LOGW(TAG, "MQTT host invalid; using default");
        (void)snprintf(host, host_size, "%s", APP_MQTT_BROKER_HOST);
    }
    if ((nvs_get_u32(handle, NVS_PORT_KEY, port) != ESP_OK) || (*port == 0U) || (*port > 65535U)) {
        ESP_LOGW(TAG, "MQTT port invalid; using default");
        *port = APP_MQTT_BROKER_PORT;
    }
    nvs_close(handle);
}

static void network_event_handler(void *argument, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data)
{
    (void)argument;
    (void)event_data;
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_START)) {
        (void)esp_wifi_connect();
    } else if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_DISCONNECTED)) {
        xEventGroupClearBits(g_network_events, WIFI_CONNECTED_BIT);
        xEventGroupSetBits(g_network_events, WIFI_RETRY_BIT);
        ESP_LOGW(TAG, "WiFi disconnected; retry scheduled");
    } else if ((event_base == IP_EVENT) && (event_id == IP_EVENT_STA_GOT_IP)) {
        g_retry_delay_ms = APP_NETWORK_RETRY_INITIAL_MS;
        xEventGroupSetBits(g_network_events, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG, "WiFi associated with %s", APP_WIFI_SSID);
    } else {
        /* Other events do not change network state. */
    }
}

static void mqtt_event_handler(void *argument, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t mqtt_event;

    (void)argument;
    (void)event_base;
    mqtt_event = (esp_mqtt_event_handle_t)event_data;
    if (event_id == MQTT_EVENT_CONNECTED) {
        xEventGroupSetBits(g_network_events, MQTT_CONNECTED_BIT);
        ESP_LOGI(TAG, "MQTT connected");
    } else if (event_id == MQTT_EVENT_DISCONNECTED) {
        xEventGroupClearBits(g_network_events, MQTT_CONNECTED_BIT);
        ESP_LOGW(TAG, "MQTT disconnected");
    } else if (event_id == MQTT_EVENT_ERROR) {
        g_publish_failures++;
        if (mqtt_event != NULL) {
            ESP_LOGW(TAG, "MQTT error count=%lu type=%d esp=%d sock=%d errno=%d",
                     (unsigned long)g_publish_failures,
                     (int)mqtt_event->error_handle->error_type,
                     mqtt_event->error_handle->esp_tls_last_esp_err,
                     mqtt_event->error_handle->esp_transport_sock_errno,
                     mqtt_event->error_handle->esp_transport_sock_errno);
        } else {
            ESP_LOGW(TAG, "MQTT error count=%lu without event data",
                     (unsigned long)g_publish_failures);
        }
    } else {
        /* Other MQTT events do not change connection state. */
    }
}

static void network_start_wifi(void)
{
    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t wifi_config = {0};
    esp_err_t result;

    result = esp_netif_init();
    if ((result != ESP_OK) && (result != ESP_ERR_INVALID_STATE)) {
        ESP_LOGE(TAG, "Network interface init failed: %s", esp_err_to_name(result));
        return;
    }
    result = esp_event_loop_create_default();
    if ((result != ESP_OK) && (result != ESP_ERR_INVALID_STATE)) {
        ESP_LOGE(TAG, "Event loop init failed: %s", esp_err_to_name(result));
        return;
    }
    if (esp_netif_create_default_wifi_sta() == NULL) {
        ESP_LOGE(TAG, "Default WiFi station interface failed");
        return;
    }
    if (esp_wifi_init(&wifi_init) != ESP_OK) {
        ESP_LOGE(TAG, "WiFi init failed");
        return;
    }
    (void)esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, network_event_handler, NULL);
    (void)esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, network_event_handler, NULL);
    (void)snprintf((char *)wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), "%s", APP_WIFI_SSID);
    (void)snprintf((char *)wifi_config.sta.password, sizeof(wifi_config.sta.password), "%s", APP_WIFI_PASSWORD);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    (void)esp_wifi_set_mode(WIFI_MODE_STA);
    (void)esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    (void)esp_wifi_start();
}

static void network_start_mqtt(void)
{
    esp_mqtt_client_config_t config = {0};

    network_load_endpoint(g_broker_host, sizeof(g_broker_host), &g_broker_port);
    ESP_LOGI(TAG, "MQTT endpoint %s:%lu", g_broker_host, (unsigned long)g_broker_port);
    config.broker.address.hostname = g_broker_host;
    config.broker.address.port = g_broker_port;
    config.broker.address.transport = MQTT_TRANSPORT_OVER_TCP;
    config.network.timeout_ms = APP_MQTT_PUBLISH_TIMEOUT_MS;
    g_mqtt_client = esp_mqtt_client_init(&config);
    ESP_LOGI(TAG, "MQTT init result=%s", (g_mqtt_client != NULL) ? "ok" : "failed");
    if (g_mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT init failed");
        return;
    }
    (void)esp_mqtt_client_register_event(g_mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    ESP_LOGI(TAG, "MQTT event handler registered");
    (void)esp_mqtt_client_start(g_mqtt_client);
    ESP_LOGI(TAG, "MQTT start requested");
}

static bool network_publish(const char *topic, const char *payload)
{
    int message_id;

    if ((xEventGroupGetBits(g_network_events) & MQTT_CONNECTED_BIT) == 0U) {
        return false;
    }
    message_id = esp_mqtt_client_publish(g_mqtt_client, topic, payload, 0, 0, 0);
    if (message_id < 0) {
        g_publish_failures++;
        ESP_LOGW(TAG, "MQTT publish failed count=%lu", (unsigned long)g_publish_failures);
        return false;
    }
    return true;
}

static void network_queue_latest(const char *topic, const char *payload, bool alert)
{
    network_message_t message;

    (void)snprintf(message.topic, sizeof(message.topic), "%s", topic);
    (void)snprintf(message.payload, sizeof(message.payload), "%s", payload);
    message.alert = alert;
    if (xQueueSend(g_message_queue, &message, 0U) != pdTRUE) {
        network_message_t discarded;
        if ((!alert) && (xQueueReceive(g_message_queue, &discarded, 0U) == pdTRUE) && (!discarded.alert)) {
            ESP_LOGW(TAG, "MQTT queue full; oldest telemetry discarded");
            (void)xQueueSend(g_message_queue, &message, 0U);
        } else {
            ESP_LOGW(TAG, "MQTT queue full; message discarded");
        }
    }
}

static void network_flush_queue(void)
{
    network_message_t message;

    while ((xEventGroupGetBits(g_network_events) & MQTT_CONNECTED_BIT) != 0U) {
        if (xQueueReceive(g_message_queue, &message, 0U) != pdTRUE) {
            break;
        }
        if (!network_publish(message.topic, message.payload)) {
            break;
        }
    }
}

static void network_task_loop(void)
{
    char payload[JSON_SIZE];
    float temperature_c;
    float humidity_percent;
    float maximum_g;
    const dht22_sample_t *climate;
    const mpu6050_state_t *motion;
    TickType_t next_wake;

    if (nvs_flash_init() != ESP_OK) {
        ESP_LOGE(TAG, "NVS initialization failed");
        return;
    }
    (void)threshold_service_load(&g_thresholds);
    network_save_defaults();
    network_start_wifi();
    (void)xEventGroupWaitBits(g_network_events, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE,
                              pdMS_TO_TICKS(15000U));
    network_start_mqtt();
    next_wake = xTaskGetTickCount();
    for (;;) {
        if ((xEventGroupGetBits(g_network_events) & WIFI_RETRY_BIT) != 0U) {
            vTaskDelay(pdMS_TO_TICKS(g_retry_delay_ms));
            (void)esp_wifi_connect();
            xEventGroupClearBits(g_network_events, WIFI_RETRY_BIT);
            if (g_retry_delay_ms < (APP_NETWORK_RETRY_MAX_MS / 2U)) {
                g_retry_delay_ms *= 2U;
            } else {
                g_retry_delay_ms = APP_NETWORK_RETRY_MAX_MS;
            }
        }
        climate = dht22_service_sample();
        motion = mpu6050_service_state();
        if (climate->valid && motion->valid) {
            temperature_c = (float)climate->temperature_tenths_c / 10.0F;
            humidity_percent = (float)climate->humidity_tenths_percent / 10.0F;
            maximum_g = fmaxf(fabsf(motion->lateral_g), fmaxf(fabsf(motion->longitudinal_g), fabsf(motion->vertical_g)));
                (void)snprintf(payload, sizeof(payload), "{\"temp_c\":%.1f,\"humidity_percent\":%.1f,\"lateral_g\":%.3f,\"longitudinal_g\":%.3f,\"vertical_g\":%.3f}", (float)climate->temperature_tenths_c / 10.0F, (float)climate->humidity_tenths_percent / 10.0F, motion->lateral_g, motion->longitudinal_g, motion->vertical_g);
            if (!network_publish(APP_MQTT_TELEMETRY_TOPIC, payload)) {
                network_queue_latest(APP_MQTT_TELEMETRY_TOPIC, payload, false);
            }
            network_flush_queue();
            if ((!g_alert_active) && threshold_service_trigger(&g_thresholds, temperature_c, humidity_percent, motion->lateral_g, motion->longitudinal_g, motion->vertical_g)) {
                g_alert_active = true;
                (void)snprintf(payload, sizeof(payload), "{\"temp_c\":%.1f,\"humidity_percent\":%.1f,\"lateral_g\":%.3f,\"longitudinal_g\":%.3f,\"vertical_g\":%.3f,\"trigger\":\"threshold\"}", temperature_c, humidity_percent, motion->lateral_g, motion->longitudinal_g, motion->vertical_g);
                if (!network_publish(APP_MQTT_ALERT_TOPIC, payload)) {
                    network_queue_latest(APP_MQTT_ALERT_TOPIC, payload, true);
                }
            }
            if (g_alert_active && (temperature_c < (g_thresholds.temperature_c - 5.0F)) && (humidity_percent < (g_thresholds.humidity_percent - 5.0F)) && (maximum_g < (g_thresholds.g_force - 0.1F))) {
                g_alert_active = false;
            }
            if (temperature_c >= APP_TEMPERATURE_ALERT_C) {
                (void)snprintf(payload, sizeof(payload), "{\"temp_c\":%.1f,\"humidity_percent\":%.1f,\"lateral_g\":%.3f,\"longitudinal_g\":%.3f,\"vertical_g\":%.3f,\"trigger\":\"temp_c\"}", (float)climate->temperature_tenths_c / 10.0F, (float)climate->humidity_tenths_percent / 10.0F, motion->lateral_g, motion->longitudinal_g, motion->vertical_g);
                if (!network_publish(APP_MQTT_ALERT_TOPIC, payload)) {
                    network_queue_latest(APP_MQTT_ALERT_TOPIC, payload, true);
                }
            }
            if ((float)climate->humidity_tenths_percent / 10.0F >= APP_HUMIDITY_ALERT_PERCENT) {
                (void)snprintf(payload, sizeof(payload), "{\"temp_c\":%.1f,\"humidity_percent\":%.1f,\"lateral_g\":%.3f,\"longitudinal_g\":%.3f,\"vertical_g\":%.3f,\"trigger\":\"humidity_percent\"}", (float)climate->temperature_tenths_c / 10.0F, (float)climate->humidity_tenths_percent / 10.0F, motion->lateral_g, motion->longitudinal_g, motion->vertical_g);
                if (!network_publish(APP_MQTT_ALERT_TOPIC, payload)) {
                    network_queue_latest(APP_MQTT_ALERT_TOPIC, payload, true);
                }
            }
        }
        vTaskDelayUntil(&next_wake, pdMS_TO_TICKS(APP_TELEMETRY_PERIOD_MS));
    }
}

void network_service_task(void *argument)
{
    (void)argument;
    if (!g_initialized) {
        g_network_events = xEventGroupCreate();
        g_message_queue = xQueueCreate(APP_MQTT_QUEUE_DEPTH, sizeof(network_message_t));
        if ((g_network_events != NULL) && (g_message_queue != NULL)) {
            g_retry_delay_ms = APP_NETWORK_RETRY_INITIAL_MS;
            g_initialized = true;
            network_task_loop();
        }
    }
    vTaskDelete(NULL);
}
