#ifndef IHM16M1_PLATFORM_H
#define IHM16M1_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#include "x_nucleo_ihm16m1.h"

x_nucleo_ihm16m1_status_t ihm16m1_platform_start(void);
x_nucleo_ihm16m1_status_t ihm16m1_platform_pwm_set(
    void *ctx, x_nucleo_ihm16m1_pwm_channel_t channel, float duty);
x_nucleo_ihm16m1_status_t ihm16m1_platform_pwm_enable(void *ctx, bool enable);
x_nucleo_ihm16m1_status_t ihm16m1_platform_pwm_mode_set(
    void *ctx, x_nucleo_ihm16m1_pwm_mode_t mode);
x_nucleo_ihm16m1_status_t ihm16m1_platform_gpio_write(
    void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool level);
x_nucleo_ihm16m1_status_t ihm16m1_platform_gpio_read(
    void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool *level);
x_nucleo_ihm16m1_status_t ihm16m1_platform_adc_read(
    void *ctx, x_nucleo_ihm16m1_adc_channel_t channel, uint32_t *raw);
void ihm16m1_platform_delay(void *ctx, uint32_t milliseconds);

#endif /* IHM16M1_PLATFORM_H */
