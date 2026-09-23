#include "watchdog_platform.h"

#include "main.h"

static IWDG_HandleTypeDef g_hiwdg;
static bool g_started;

bool watchdog_platform_start(void)
{
    if (g_started) {
        return true;
    }

    g_hiwdg.Instance = IWDG;
    g_hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
    g_hiwdg.Init.Window = IWDG_WINDOW_DISABLE;
    g_hiwdg.Init.Reload = 500U;
    g_started = HAL_IWDG_Init(&g_hiwdg) == HAL_OK;
    return g_started;
}

void watchdog_platform_kick(void)
{
    if (g_started) {
        (void)HAL_IWDG_Refresh(&g_hiwdg);
    }
}
