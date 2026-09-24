#include "diagnostics_service.h"

#include <stdio.h>

#include "app_config.h"
#include "dht22_service.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mpu6050_service.h"
#include "network_service.h"

#define DIAGNOSTICS_STACK_SIZE 3072U
#define DIAGNOSTICS_PRIORITY 3U
#define DIAGNOSTICS_QUEUE_DEPTH 4U
#define DIAGNOSTICS_WATCHDOG_TIMEOUT_MS 10U

static const char *TAG = "diagnostics";
static QueueHandle_t g_command_queue;

static const char *diagnostics_reset_name(esp_reset_reason_t reason)
{
    if (reason == ESP_RST_BROWNOUT) {
        return "brownout";
    }
    if (reason == ESP_RST_TASK_WDT || reason == ESP_RST_INT_WDT || reason == ESP_RST_WDT) {
        return "watchdog";
    }
    return "normal";
}

static void diagnostics_task(void *argument)
{
    char command;
    esp_task_wdt_config_t watchdog_config = {
        .timeout_ms = DIAGNOSTICS_WATCHDOG_TIMEOUT_MS,
        .idle_core_mask = 0U,
        .trigger_panic = true,
    };

    (void)argument;
    (void)watchdog_config;
    (void)esp_task_wdt_add(NULL);
    ESP_LOGI(TAG, "STATUS version=1.0 ssid=%s mqtt=%s:%u dht22=runtime mpu6050=runtime reset=%s",
             APP_WIFI_SSID, APP_MQTT_BROKER_HOST, APP_MQTT_BROKER_PORT,
             diagnostics_reset_name(esp_reset_reason()));
    for (;;) {
        command = 0;
        if (xQueueReceive(g_command_queue, &command, pdMS_TO_TICKS(250U)) == pdTRUE) {
            if (command == '?') {
                ESP_LOGI(TAG, "HEALTH dht22_failures=%lu imu_failures=%lu mqtt_publish_failures=%lu",
                         (unsigned long)dht22_service_sample()->failure_count,
                         (unsigned long)mpu6050_service_state()->failure_count,
                         (unsigned long)network_service_publish_failures());
            } else {
                ESP_LOGW(TAG, "unknown diagnostics command");
            }
        }
        (void)esp_task_wdt_reset();
    }
}

void diagnostics_service_start(void)
{
    g_command_queue = xQueueCreate(DIAGNOSTICS_QUEUE_DEPTH, sizeof(char));
    if (g_command_queue == NULL) {
        ESP_LOGE(TAG, "diagnostics queue initialization failed");
        return;
    }
    if (xTaskCreate(diagnostics_task, "diagnostics_task", DIAGNOSTICS_STACK_SIZE,
                    NULL, DIAGNOSTICS_PRIORITY, NULL) != pdPASS) {
        ESP_LOGE(TAG, "diagnostics task initialization failed");
    }
}
