#ifndef X_NUCLEO_IHM16M1_H
#define X_NUCLEO_IHM16M1_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    X_NUCLEO_IHM16M1_OK = 0,
    X_NUCLEO_IHM16M1_INVALID_ARGUMENT = 1,
    X_NUCLEO_IHM16M1_NOT_INITIALIZED = 2,
    X_NUCLEO_IHM16M1_ALREADY_INITIALIZED = 3,
    X_NUCLEO_IHM16M1_COMMUNICATION_ERROR = 4,
    X_NUCLEO_IHM16M1_TIMEOUT = 5,
    X_NUCLEO_IHM16M1_UNSUPPORTED = 6,
    X_NUCLEO_IHM16M1_IDENTITY_MISMATCH = 7,
    X_NUCLEO_IHM16M1_OUT_OF_RANGE = 8
} x_nucleo_ihm16m1_status_t;

typedef enum {
    X_NUCLEO_IHM16M1_PWM_U = 0,
    X_NUCLEO_IHM16M1_PWM_V,
    X_NUCLEO_IHM16M1_PWM_W,
    X_NUCLEO_IHM16M1_PWM_ENU,
    X_NUCLEO_IHM16M1_PWM_ENV,
    X_NUCLEO_IHM16M1_PWM_ENW
} x_nucleo_ihm16m1_pwm_channel_t;

typedef enum {
    X_NUCLEO_IHM16M1_GPIO_STBY = 0,
    X_NUCLEO_IHM16M1_GPIO_EN_FAULT,
    X_NUCLEO_IHM16M1_GPIO_H1,
    X_NUCLEO_IHM16M1_GPIO_H2,
    X_NUCLEO_IHM16M1_GPIO_H3
} x_nucleo_ihm16m1_gpio_pin_t;

typedef enum {
    X_NUCLEO_IHM16M1_ADC_PHASE_U_CURRENT = 0,
    X_NUCLEO_IHM16M1_ADC_PHASE_V_CURRENT,
    X_NUCLEO_IHM16M1_ADC_PHASE_W_CURRENT,
    X_NUCLEO_IHM16M1_ADC_BUS_VOLTAGE,
    X_NUCLEO_IHM16M1_ADC_PCB_TEMPERATURE,
    X_NUCLEO_IHM16M1_ADC_BEMF_U,
    X_NUCLEO_IHM16M1_ADC_BEMF_V,
    X_NUCLEO_IHM16M1_ADC_BEMF_W
} x_nucleo_ihm16m1_adc_channel_t;

typedef enum {
    X_NUCLEO_IHM16M1_ANALOG_CURRENT_REFERENCE = 0
} x_nucleo_ihm16m1_analog_output_t;

typedef enum {
    X_NUCLEO_IHM16M1_THREE_PWM_INPUTS = 3,
    X_NUCLEO_IHM16M1_SIX_PWM_INPUTS = 6
} x_nucleo_ihm16m1_pwm_mode_t;

struct x_nucleo_ihm16m1;
typedef struct x_nucleo_ihm16m1 x_nucleo_ihm16m1_t;

typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_pwm_set_fn)(void *ctx, x_nucleo_ihm16m1_pwm_channel_t channel, float duty);
typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_pwm_enable_fn)(void *ctx, bool enable);
typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_pwm_mode_fn)(void *ctx, x_nucleo_ihm16m1_pwm_mode_t mode);
typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_gpio_write_fn)(void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool level);
typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_gpio_read_fn)(void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool *level);
typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_adc_read_fn)(void *ctx, x_nucleo_ihm16m1_adc_channel_t channel, uint32_t *raw);
typedef x_nucleo_ihm16m1_status_t (*x_nucleo_ihm16m1_analog_write_fn)(void *ctx, x_nucleo_ihm16m1_analog_output_t output, float volts);
typedef void (*x_nucleo_ihm16m1_delay_ms_fn)(void *ctx, uint32_t milliseconds);

typedef struct {
    void *ctx;
    x_nucleo_ihm16m1_pwm_set_fn pwm_set;
    x_nucleo_ihm16m1_pwm_enable_fn pwm_enable;
    x_nucleo_ihm16m1_pwm_mode_fn pwm_mode_set;
    x_nucleo_ihm16m1_gpio_write_fn gpio_write;
    x_nucleo_ihm16m1_gpio_read_fn gpio_read;
    x_nucleo_ihm16m1_adc_read_fn adc_read;
    x_nucleo_ihm16m1_analog_write_fn analog_write;
    x_nucleo_ihm16m1_delay_ms_fn delay_ms;
} x_nucleo_ihm16m1_io_t;

typedef struct {
    x_nucleo_ihm16m1_pwm_mode_t pwm_mode;
    bool start_in_standby;
} x_nucleo_ihm16m1_config_t;

typedef struct {
    uint32_t raw;
    bool converted;
    float value;
} x_nucleo_ihm16m1_sample_t;

size_t x_nucleo_ihm16m1_handle_size(void);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_init(x_nucleo_ihm16m1_t *handle, size_t handle_size, const x_nucleo_ihm16m1_io_t *io, const x_nucleo_ihm16m1_config_t *config);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_deinit(x_nucleo_ihm16m1_t *handle);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_standby(x_nucleo_ihm16m1_t *handle, bool standby);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_pwm_mode(x_nucleo_ihm16m1_t *handle, x_nucleo_ihm16m1_pwm_mode_t mode);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_current_limit_reference(x_nucleo_ihm16m1_t *handle, float reference_v);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_drive_motor(x_nucleo_ihm16m1_t *handle, float pwm_u, float pwm_v, float pwm_w);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_adc(x_nucleo_ihm16m1_t *handle, x_nucleo_ihm16m1_adc_channel_t channel, uint32_t *raw);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_sample(x_nucleo_ihm16m1_t *handle, x_nucleo_ihm16m1_adc_channel_t channel, x_nucleo_ihm16m1_sample_t *sample);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_fault(x_nucleo_ihm16m1_t *handle, bool *fault);
x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_hall(x_nucleo_ihm16m1_t *handle, bool *h1, bool *h2, bool *h3);

#endif
