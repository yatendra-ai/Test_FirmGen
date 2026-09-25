#ifndef PWM_RAMP_H
#define PWM_RAMP_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t cycle_start_ms;
} pwm_ramp_state_t;

void pwm_ramp_reset(pwm_ramp_state_t *p_state, uint32_t now_ms);
uint32_t pwm_ramp_duty_percent(const pwm_ramp_state_t *p_state, uint32_t now_ms);

#endif /* PWM_RAMP_H */
