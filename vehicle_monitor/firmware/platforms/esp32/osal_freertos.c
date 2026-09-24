#include "osal.h"

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

void osal_delay_ms(uint32_t milliseconds)
{
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

uint32_t osal_uptime_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000LL);
}

osal_task_t osal_task_create(osal_task_fn_t function,
                             const char *name,
                             size_t stack_bytes,
                             void *argument,
                             uint8_t priority)
{
    TaskHandle_t handle;
    BaseType_t result;

    handle = NULL;
    result = xTaskCreate(function, name, stack_bytes / sizeof(StackType_t), argument,
                         (UBaseType_t)priority, &handle);
    if (result != pdPASS) {
        return NULL;
    }
    return (osal_task_t)handle;
}

osal_mutex_t osal_mutex_create(void)
{
    return (osal_mutex_t)xSemaphoreCreateMutex();
}

bool osal_mutex_lock(osal_mutex_t mutex, uint32_t timeout_ms)
{
    return (xSemaphoreTake((SemaphoreHandle_t)mutex, pdMS_TO_TICKS(timeout_ms)) == pdTRUE);
}

void osal_mutex_unlock(osal_mutex_t mutex)
{
    (void)xSemaphoreGive((SemaphoreHandle_t)mutex);
}
