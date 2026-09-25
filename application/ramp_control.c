#include "ramp_control.h"

#include "app_config.h"

#include <stddef.h>
#include <stdint.h>

static volatile uint32_t s_tick_events;
static uint32_t s_start_ms;
static uint32_t s_last_update_ms;

static uint16_t ramp_control_calculate_duty(uint32_t elapsed_ms,
                                             uint32_t ramp_ms)
{
    uint32_t duty_permille = 0U;
    uint32_t cycle_ms;
    uint32_t phase_ms;

    if (ramp_ms == 0U)
    {
        return 0U;
    }

    if (ramp_ms > (UINT32_MAX / 2U))
    {
        cycle_ms = UINT32_MAX;
    }
    else
    {
        cycle_ms = ramp_ms * 2U;
    }

    phase_ms = elapsed_ms % cycle_ms;

    if (phase_ms <= ramp_ms)
    {
        duty_permille = (phase_ms * 1000U) / ramp_ms;
    }
    else
    {
        duty_permille = ((cycle_ms - phase_ms) * 1000U) / ramp_ms;
    }

    if (duty_permille > 1000U)
    {
        duty_permille = 1000U;
    }
    else
    {
    }

    return (uint16_t)duty_permille;
}

void ramp_control_init(uint32_t now_ms)
{
    s_tick_events = 0U;
    s_start_ms = now_ms;
    s_last_update_ms = now_ms;
}

void ramp_control_on_tick(void)
{
    s_tick_events++;
}

bool ramp_control_update(uint32_t now_ms, uint16_t *duty_permille)
{
    const app_config_t *config = app_config_get();
    const uint32_t elapsed_ms = now_ms - s_start_ms;
    const uint32_t since_update_ms = now_ms - s_last_update_ms;
    bool update_ready = false;

    if ((duty_permille != NULL) && (s_tick_events > 0U)
        && (config->pwm_ramp_ms != 0U)
        && (config->pwm_step_ms != 0U)
        && (since_update_ms >= config->pwm_step_ms))
    {
        *duty_permille = ramp_control_calculate_duty(elapsed_ms,
                                                     config->pwm_ramp_ms);
        s_last_update_ms = now_ms;
        s_tick_events = 0U;
        update_ready = true;
    }
    else
    {
    }

    return update_ready;
}
