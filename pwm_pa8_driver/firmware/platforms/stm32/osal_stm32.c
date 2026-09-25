#include "osal.h"

#include "stm32_hal.h"

void osal_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

uint32_t osal_uptime_ms(void)
{
    return HAL_GetTick();
}

osal_task_t osal_task_create(osal_task_fn_t fn, const char *name, size_t stack_bytes, void *arg, uint8_t priority)
{
    (void)fn;
    (void)name;
    (void)stack_bytes;
    (void)arg;
    (void)priority;
    return NULL;
}

osal_mutex_t osal_mutex_create(void)
{
    return NULL;
}

bool osal_mutex_lock(osal_mutex_t mutex, uint32_t timeout_ms)
{
    (void)mutex;
    (void)timeout_ms;
    return false;
}

void osal_mutex_unlock(osal_mutex_t mutex)
{
    (void)mutex;
}
