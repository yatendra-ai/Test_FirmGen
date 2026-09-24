#ifndef MOTOR_PLATFORM_H
#define MOTOR_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#include "x_nucleo_ihm16m1.h"

void motor_platform_led_on(void);
x_nucleo_ihm16m1_status_t motor_platform_init(void);
x_nucleo_ihm16m1_status_t motor_platform_enable(bool enable);
x_nucleo_ihm16m1_status_t motor_platform_read_fault(bool *fault);
x_nucleo_ihm16m1_status_t motor_platform_drive(float duty_u, float duty_v, float duty_w);
void motor_platform_delay_ms(uint32_t milliseconds);

#endif /* MOTOR_PLATFORM_H */
