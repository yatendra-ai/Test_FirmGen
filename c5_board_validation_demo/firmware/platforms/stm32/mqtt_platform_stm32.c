#include "mqtt_platform_if.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "network_status_if.h"
#include "example.h"
#include "app_config.h"
#include "app_logger.h"
#include "diagnostics.h"
#include "board_io_if.h"
#include "app_lwip.h"
#include "button_logic_config.h"

static mqtt_client_t *g_mqtt_client = NULL;
static struct mqtt_connect_client_info_t g_mqtt_client_info;
static volatile bool g_mqtt_connected = false;
static volatile bool g_mqtt_connect_pending = false;
static volatile bool g_application_state = false;
static volatile uint32_t g_last_telemetry_ms = 0U;
static TaskHandle_t g_mqtt_task_handle = NULL;
static char g_control_payload[64];
static u16_t g_control_payload_length = 0U;

static void mqtt_platform_task(void *arg);
static void mqtt_platform_connection_callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_platform_publish_callback(void *arg, err_t error);
static void mqtt_platform_incoming_publish_callback(void *arg, const char *topic, u32_t total_length);
static void mqtt_platform_incoming_data_callback(void *arg, const u8_t *data, u16_t length, u8_t flags);
static void mqtt_platform_on_connected(mqtt_client_t *client);
static void mqtt_platform_publish_telemetry(void);
static bool mqtt_platform_connect(void);

void mqtt_platform_start(void)
{
    BaseType_t result;

    if (g_mqtt_task_handle == NULL)
    {
        result = xTaskCreate(mqtt_platform_task, "mqtt_client_task",
                             (configSTACK_DEPTH_TYPE)(APP_MQTT_TASK_STACK_BYTES / sizeof(StackType_t)),
                             NULL, APP_MQTT_TASK_PRIORITY, &g_mqtt_task_handle);
        if (result != pdPASS)
        {
            g_mqtt_task_handle = NULL;
            APP_LOG_ERROR("MQTT", "MQTT task creation failed\n");
        }
    }
}

bool mqtt_platform_is_connected(void)
{
    return g_mqtt_connected;
}

void mqtt_platform_publish_state(bool state)
{
    g_application_state = state;

    if (g_mqtt_connected && (g_mqtt_client != NULL))
    {
        char payload[32];

        (void)snprintf(payload, sizeof(payload), "{\"application_state\":%u}", state ? 1U : 0U);
        if (mqtt_publish(g_mqtt_client, APP_MQTT_TELEMETRY_TOPIC,
                         payload, (u16_t)strlen(payload), 0U, 0U,
                         mqtt_platform_publish_callback, NULL) != ERR_OK)
        {
            APP_LOG_ERROR("MQTT", "button-state publish request failed\n");
        }
    }
    else
    {
        APP_LOG_INFO("MQTT", "state changed while disconnected; latest value retained for reconnect\n");
    }
}

static void mqtt_platform_task(void *arg)
{
    (void)arg;

    while (true)
    {
        uint32_t now_ms = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);

        if (g_mqtt_connected && (g_mqtt_client != NULL) &&
            (mqtt_client_is_connected(g_mqtt_client) == 0U))
        {
            g_mqtt_connected = false;
            g_mqtt_connect_pending = false;
            board_io_set_led(BOARD_IO_LED_MQTT, false);
            diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_MQTT, "connection lost; reconnecting");
            mqtt_client_free(g_mqtt_client);
            g_mqtt_client = NULL;
        }
        else
        {
            /* Connection state is unchanged. */
        }

        if (network_status_is_ready() && !g_mqtt_connected && !g_mqtt_connect_pending)
        {
            (void)mqtt_platform_connect();
        }

        if (g_mqtt_connected &&
            ((uint32_t)(now_ms - g_last_telemetry_ms) >= APP_MQTT_TELEMETRY_PERIOD_MS))
        {
            mqtt_platform_publish_telemetry();
            g_last_telemetry_ms = now_ms;
        }

        vTaskDelay(pdMS_TO_TICKS(APP_MQTT_RETRY_PERIOD_MS));
    }
}

static bool mqtt_platform_connect(void)
{
    ip_addr_t broker_address;
    err_t result;

    if (app_lwip_dns_resolve(APP_MQTT_BROKER_HOST, &broker_address,
                             APP_MQTT_RETRY_PERIOD_MS) != EXEC_STATUS_OK)
    {
        APP_LOG_ERROR("MQTT", "broker DNS resolution failed\n");
        return false;
    }

    if (g_mqtt_client == NULL)
    {
        g_mqtt_client = mqtt_client_new();
        if (g_mqtt_client == NULL)
        {
            APP_LOG_ERROR("MQTT", "client allocation failed\n");
            return false;
        }

        (void)memset(&g_mqtt_client_info, 0, sizeof(g_mqtt_client_info));
        g_mqtt_client_info.client_id = APP_MQTT_CLIENT_ID;
        g_mqtt_client_info.client_user = NULL;
        g_mqtt_client_info.client_pass = NULL;
        g_mqtt_client_info.keep_alive = 30U;
        mqtt_set_inpub_callback(g_mqtt_client, mqtt_platform_incoming_publish_callback,
                                mqtt_platform_incoming_data_callback, NULL);
    }

    g_mqtt_connect_pending = true;
    result = mqtt_client_connect(g_mqtt_client, &broker_address, APP_MQTT_BROKER_PORT,
                                 mqtt_platform_connection_callback, NULL, &g_mqtt_client_info);
    if (result != ERR_OK)
    {
        g_mqtt_connect_pending = false;
        APP_LOG_ERROR("MQTT", "TCP/MQTT connect request failed: %d\n", result);
        mqtt_client_free(g_mqtt_client);
        g_mqtt_client = NULL;
        return false;
    }

    APP_LOG_INFO("MQTT", "connecting to %s:%u over plaintext TCP\n",
                 APP_MQTT_BROKER_HOST, (unsigned int)APP_MQTT_BROKER_PORT);
    return true;
}

static void mqtt_platform_connection_callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    (void)arg;
    g_mqtt_connect_pending = false;

    if (status == MQTT_CONNECT_ACCEPTED)
    {
        g_mqtt_connected = true;
        g_last_telemetry_ms = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
        board_io_set_led(BOARD_IO_LED_MQTT, true);
        diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_MQTT, "connected to broker.hivemq.com:1883");
        APP_LOG_INFO("MQTT", "CONNACK accepted via plaintext TCP port 1883\n");
        mqtt_platform_on_connected(client);
    }
    else
    {
        g_mqtt_connected = false;
        board_io_set_led(BOARD_IO_LED_MQTT, false);
        diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_MQTT, "connection ended/refused; retry pending");
        APP_LOG_INFO("MQTT", "connection status=%u\n", (unsigned int)status);
    }
}

static void mqtt_platform_on_connected(mqtt_client_t *client)
{
    err_t result;

    result = mqtt_subscribe(client, APP_MQTT_CONTROL_TOPIC, 0U,
                            mqtt_platform_publish_callback, NULL);
    APP_LOG_INFO("MQTT", "subscribe /firmgen/c5/control result=%d\n", result);
    mqtt_platform_publish_state(g_application_state);
}

static void mqtt_platform_publish_telemetry(void)
{
    char payload[96];

    if ((g_mqtt_client != NULL) && g_mqtt_connected)
    {
        (void)snprintf(payload, sizeof(payload), "{\"application_state\":%u,\"uptime_ms\":%lu}",
                       g_application_state ? 1U : 0U,
                       (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        if (mqtt_publish(g_mqtt_client, APP_MQTT_TELEMETRY_TOPIC,
                         payload, (u16_t)strlen(payload), 0U, 0U,
                         mqtt_platform_publish_callback, NULL) != ERR_OK)
        {
            APP_LOG_ERROR("MQTT", "telemetry publish request failed\n");
        }
        else
        {
            APP_LOG_INFO("MQTT", "telemetry publish requested on %s\n", APP_MQTT_TELEMETRY_TOPIC);
        }
    }
}

static void mqtt_platform_publish_callback(void *arg, err_t error)
{
    (void)arg;
    if (error != ERR_OK)
    {
        APP_LOG_ERROR("MQTT", "publish/subscribe completion error=%d\n", error);
    }
}

static void mqtt_platform_incoming_publish_callback(void *arg, const char *topic, u32_t total_length)
{
    (void)arg;
    g_control_payload_length = 0U;
    g_control_payload[0] = '\0';
    APP_LOG_INFO("MQTT", "incoming publish topic=%s length=%lu\n",
                 (topic != NULL) ? topic : "unknown", (unsigned long)total_length);
}

static void mqtt_platform_incoming_data_callback(void *arg, const u8_t *data, u16_t length, u8_t flags)
{
    u16_t remaining = (u16_t)(sizeof(g_control_payload) - 1U - g_control_payload_length);

    (void)arg;
    if ((data != NULL) && (length <= remaining))
    {
        (void)memcpy(&g_control_payload[g_control_payload_length], data, length);
        g_control_payload_length = (u16_t)(g_control_payload_length + length);
        g_control_payload[g_control_payload_length] = '\0';
    }

    if ((flags & MQTT_DATA_FLAG_LAST) != 0U)
    {
        APP_LOG_INFO("MQTT", "control payload=%s\n", g_control_payload);
        if (strstr(g_control_payload, "1") != NULL)
        {
            g_application_state = BUTTON_LOGIC_STATE_SETTER(true);
        }
        else if (strstr(g_control_payload, "0") != NULL)
        {
            g_application_state = BUTTON_LOGIC_STATE_SETTER(false);
        }
        else
        {
            /* No boolean state command was found; preserve application state. */
        }
    }
}
