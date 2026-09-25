#include "pwm_ramp.h"

#include <stddef.h>

#define PWM_RAMP_HALF_CYCLE_MS 5000U
#define PWM_RAMP_FULL_CYCLE_MS 10000U
#define PWM_RAMP_MAX_DUTY_PERCENT 100U

static uint32_t pwm_ramp_elapsed_ms(uint32_t start_ms, uint32_t now_ms)
{
    return (uint32_t)(now_ms - start_ms);
}

void pwm_ramp_reset(pwm_ramp_state_t *p_state, uint32_t now_ms)
{
    if (p_state != NULL)
    {
        p_state->cycle_start_ms = now_ms;
    }
}

uint32_t pwm_ramp_duty_percent(const pwm_ramp_state_t *p_state, uint32_t now_ms)
{
    uint32_t elapsed_ms;
    uint32_t phase_ms;
    uint32_t duty_percent;

    if (p_state == NULL)
    {
        return 0U;
    }

    elapsed_ms = pwm_ramp_elapsed_ms(p_state->cycle_start_ms, now_ms);
    phase_ms = elapsed_ms % PWM_RAMP_FULL_CYCLE_MS;

    if (phase_ms <= PWM_RAMP_HALF_CYCLE_MS)
    {
        duty_percent = (phase_ms * PWM_RAMP_MAX_DUTY_PERCENT) / PWM_RAMP_HALF_CYCLE_MS;
    }
    else
    {
        duty_percent = ((PWM_RAMP_FULL_CYCLE_MS - phase_ms) * PWM_RAMP_MAX_DUTY_PERCENT) /
                       PWM_RAMP_HALF_CYCLE_MS;
    }

    if (duty_percent > PWM_RAMP_MAX_DUTY_PERCENT)
    {
        duty_percent = PWM_RAMP_MAX_DUTY_PERCENT;
    }

    return duty_percent;
}
