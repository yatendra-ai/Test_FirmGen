#ifndef PWM_PA8_H
#define PWM_PA8_H

#include <stdbool.h>
#include <stdint.h>

bool pwm_pa8_start(uint32_t frequency_hz);
void pwm_pa8_set_duty_percent(uint32_t duty_percent);

#endif /* PWM_PA8_H */
