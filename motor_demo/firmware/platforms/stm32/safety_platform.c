#include "safety_platform.h"

#include "app_config.h"
#include "motor_platform.h"
#include "stm32g4xx_hal.h"

static IWDG_HandleTypeDef g_watchdog;
static bool g_watchdog_initialized;
static bool g_fault_latched;

x_nucleo_ihm16m1_status_t safety_platform_init(void)
{
    g_fault_latched = false;
    g_watchdog.Instance = IWDG;
    g_watchdog.Init.Prescaler = IWDG_PRESCALER_64;
    g_watchdog.Init.Window = IWDG_WINDOW_DISABLE;
    g_watchdog.Init.Reload = 1250U;

    if (HAL_IWDG_Init(&g_watchdog) != HAL_OK) {
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    g_watchdog_initialized = true;
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t safety_platform_check_fault(bool *fault)
{
    x_nucleo_ihm16m1_status_t status;

    if (fault == NULL) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }

    status = motor_platform_read_fault(fault);
    if (status != X_NUCLEO_IHM16M1_OK || *fault) {
        g_fault_latched = true;
    }

    *fault = g_fault_latched;
    return status;
}

x_nucleo_ihm16m1_status_t safety_platform_disable(void)
{
    g_fault_latched = true;
    return motor_platform_enable(false);
}

void safety_platform_watchdog_refresh(void)
{
    if (g_watchdog_initialized) {
        (void)HAL_IWDG_Refresh(&g_watchdog);
    }
}
