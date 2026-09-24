#include "app.h"
#include "app_config.h"
#include "logger.h"
#include "dht22_service.h"
#include "mpu6050_service.h"
#include "osal.h"
#include "network_service.h"
#include "diagnostics_service.h"

static const char *TAG = "app";

static void dht22_task(void *argument)
{
    (void)argument;
    dht22_service_start();
    return;
}

static void mpu6050_task(void *argument)
{
    (void)argument;
    mpu6050_service_start();
    return;
}

void app_start(void)
{
    ESP_LOGI(TAG, "firmware started");
    (void)osal_task_create(dht22_task, "dht22_task", 4096U, NULL, 4U);
    (void)osal_task_create(mpu6050_task, "mpu6050_task", 4096U, NULL, 4U);
    (void)osal_task_create(network_service_task, "network_task", 6144U, NULL, 5U);
    diagnostics_service_start();
}
