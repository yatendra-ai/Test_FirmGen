#include "x_nucleo_ihm16m1.h"
#include <string.h>

struct x_nucleo_ihm16m1 {
    x_nucleo_ihm16m1_io_t io;
    x_nucleo_ihm16m1_pwm_mode_t mode;
    bool initialized;
};

static bool valid_mode(x_nucleo_ihm16m1_pwm_mode_t mode)
{
    return (mode == X_NUCLEO_IHM16M1_THREE_PWM_INPUTS) ||
           (mode == X_NUCLEO_IHM16M1_SIX_PWM_INPUTS);
}

static bool valid_channel(x_nucleo_ihm16m1_adc_channel_t channel)
{
    return channel <= X_NUCLEO_IHM16M1_ADC_BEMF_W;
}

static x_nucleo_ihm16m1_status_t ready(x_nucleo_ihm16m1_t *handle)
{
    if (handle == NULL) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    return handle->initialized ? X_NUCLEO_IHM16M1_OK : X_NUCLEO_IHM16M1_NOT_INITIALIZED;
}

size_t x_nucleo_ihm16m1_handle_size(void)
{
    return sizeof(x_nucleo_ihm16m1_t);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_init(x_nucleo_ihm16m1_t *handle, size_t handle_size, const x_nucleo_ihm16m1_io_t *io, const x_nucleo_ihm16m1_config_t *config)
{
    x_nucleo_ihm16m1_status_t status;

    if ((handle == NULL) || (io == NULL) || (config == NULL) || (handle_size < sizeof(*handle)) || (io->gpio_write == NULL) || (io->pwm_set == NULL) || (io->pwm_enable == NULL) || (io->pwm_mode_set == NULL) || !valid_mode(config->pwm_mode)) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    if (handle->initialized) {
        return X_NUCLEO_IHM16M1_ALREADY_INITIALIZED;
    }

    (void)memset(handle, 0, sizeof(*handle));
    handle->io = *io;
    handle->mode = config->pwm_mode;
    status = handle->io.pwm_enable(handle->io.ctx, false);
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = handle->io.gpio_write(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, true);
    }
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = handle->io.pwm_mode_set(handle->io.ctx, config->pwm_mode);
    }
    if (status != X_NUCLEO_IHM16M1_OK) {
        (void)handle->io.pwm_enable(handle->io.ctx, false);
        (void)handle->io.gpio_write(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, true);
        (void)memset(handle, 0, sizeof(*handle));
        return status;
    }
    handle->initialized = true;
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_deinit(x_nucleo_ihm16m1_t *handle)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    x_nucleo_ihm16m1_status_t standby_status;

    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    status = handle->io.pwm_enable(handle->io.ctx, false);
    standby_status = handle->io.gpio_write(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, true);
    (void)memset(handle, 0, sizeof(*handle));
    return (status != X_NUCLEO_IHM16M1_OK) ? status : standby_status;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_standby(x_nucleo_ihm16m1_t *handle, bool standby)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    return handle->io.gpio_write(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, standby);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_pwm_mode(x_nucleo_ihm16m1_t *handle, x_nucleo_ihm16m1_pwm_mode_t mode)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    if (!valid_mode(mode)) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    status = handle->io.pwm_mode_set(handle->io.ctx, mode);
    if (status == X_NUCLEO_IHM16M1_OK) {
        handle->mode = mode;
    }
    return status;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_current_limit_reference(x_nucleo_ihm16m1_t *handle, float reference_v)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    if ((reference_v != reference_v) || (reference_v < 0.0f) || (handle->io.analog_write == NULL)) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    return handle->io.analog_write(handle->io.ctx, X_NUCLEO_IHM16M1_ANALOG_CURRENT_REFERENCE, reference_v);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_drive_motor(x_nucleo_ihm16m1_t *handle, float pwm_u, float pwm_v, float pwm_w)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    if ((pwm_u != pwm_u) || (pwm_v != pwm_v) || (pwm_w != pwm_w) || (pwm_u < 0.0f) || (pwm_u > 1.0f) || (pwm_v < 0.0f) || (pwm_v > 1.0f) || (pwm_w < 0.0f) || (pwm_w > 1.0f)) {
        return X_NUCLEO_IHM16M1_OUT_OF_RANGE;
    }
    status = handle->io.pwm_set(handle->io.ctx, X_NUCLEO_IHM16M1_PWM_U, pwm_u);
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = handle->io.pwm_set(handle->io.ctx, X_NUCLEO_IHM16M1_PWM_V, pwm_v);
    }
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = handle->io.pwm_set(handle->io.ctx, X_NUCLEO_IHM16M1_PWM_W, pwm_w);
    }
    return status;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_adc(x_nucleo_ihm16m1_t *handle, x_nucleo_ihm16m1_adc_channel_t channel, uint32_t *raw)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    if ((raw == NULL) || !valid_channel(channel) || (handle->io.adc_read == NULL)) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    return handle->io.adc_read(handle->io.ctx, channel, raw);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_sample(x_nucleo_ihm16m1_t *handle, x_nucleo_ihm16m1_adc_channel_t channel, x_nucleo_ihm16m1_sample_t *sample)
{
    x_nucleo_ihm16m1_status_t status;
    if (sample == NULL) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    status = x_nucleo_ihm16m1_read_adc(handle, channel, &sample->raw);
    sample->converted = false;
    sample->value = 0.0f;
    return status;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_fault(x_nucleo_ihm16m1_t *handle, bool *fault)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    if ((fault == NULL) || (handle->io.gpio_read == NULL)) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    return handle->io.gpio_read(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_EN_FAULT, fault);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_hall(x_nucleo_ihm16m1_t *handle, bool *h1, bool *h2, bool *h3)
{
    x_nucleo_ihm16m1_status_t status = ready(handle);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return status;
    }
    if ((h1 == NULL) || (h2 == NULL) || (h3 == NULL) || (handle->io.gpio_read == NULL)) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    status = handle->io.gpio_read(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_H1, h1);
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = handle->io.gpio_read(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_H2, h2);
    }
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = handle->io.gpio_read(handle->io.ctx, X_NUCLEO_IHM16M1_GPIO_H3, h3);
    }
    return status;
}
