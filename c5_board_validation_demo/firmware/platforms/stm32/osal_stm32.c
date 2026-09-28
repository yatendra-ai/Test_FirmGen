#include "osal.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void osal_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

uint32_t osal_uptime_ms(void)
{
    TickType_t ticks = xTaskGetTickCount();

    return (uint32_t)((((uint64_t)ticks) * 1000U) / configTICK_RATE_HZ);
}

osal_task_t osal_task_create(osal_task_fn_t fn,
                             const char *name,
                             size_t stack_bytes,
                             void *arg,
                             uint8_t priority)
{
    TaskHandle_t h_task = NULL;
    uint32_t stack_depth = (uint32_t)(stack_bytes / sizeof(StackType_t));

    if ((fn != NULL) && (stack_depth > 0U))
    {
        if (xTaskCreate(fn, name, (configSTACK_DEPTH_TYPE)stack_depth, arg,
                        (UBaseType_t)priority, &h_task) != pdPASS)
        {
            h_task = NULL;
        }
    }

    return (osal_task_t)h_task;
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
