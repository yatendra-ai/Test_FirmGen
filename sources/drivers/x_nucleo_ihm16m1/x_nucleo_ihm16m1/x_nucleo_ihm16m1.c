#include "x_nucleo_ihm16m1.h"
#include <string.h>

struct x_nucleo_ihm16m1 {
    x_nucleo_ihm16m1_io_t io;
    x_nucleo_ihm16m1_pwm_mode_t mode;
    bool initialized;
};

static bool valid_mode(x_nucleo_ihm16m1_pwm_mode_t mode)
{
    return mode == X_NUCLEO_IHM16M1_THREE_PWM_INPUTS ||
           mode == X_NUCLEO_IHM16M1_SIX_PWM_INPUTS;
}

static bool valid_channel(x_nucleo_ihm16m1_adc_channel_t channel)
{
    return channel <= X_NUCLEO_IHM16M1_ADC_BEMF_W;
}

static x_nucleo_ihm16m1_status_t ready(x_nucleo_ihm16m1_t *h)
{
    if (h == NULL) return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    return h->initialized ? X_NUCLEO_IHM16M1_OK : X_NUCLEO_IHM16M1_NOT_INITIALIZED;
}

size_t x_nucleo_ihm16m1_handle_size(void)
{
    return sizeof(x_nucleo_ihm16m1_t);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_init(
    x_nucleo_ihm16m1_t *h, size_t size, const x_nucleo_ihm16m1_io_t *io,
    const x_nucleo_ihm16m1_config_t *cfg)
{
    x_nucleo_ihm16m1_status_t s;
    if (h == NULL || io == NULL || cfg == NULL || size < sizeof(*h) ||
        io->gpio_write == NULL || io->pwm_set == NULL ||
        io->pwm_enable == NULL || !valid_mode(cfg->pwm_mode))
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    if (h->initialized) return X_NUCLEO_IHM16M1_ALREADY_INITIALIZED;

    memset(h, 0, sizeof(*h));
    h->io = *io;
    h->mode = cfg->pwm_mode;

    /* Safety state is established before any PWM output is enabled. */
    s = h->io.pwm_enable(h->io.ctx, false);
    if (s != X_NUCLEO_IHM16M1_OK) goto fail;
    s = h->io.gpio_write(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, true);
    if (s != X_NUCLEO_IHM16M1_OK) goto fail;
    if (h->io.pwm_mode_set == NULL) { s = X_NUCLEO_IHM16M1_UNSUPPORTED; goto fail; }
    s = h->io.pwm_mode_set(h->io.ctx, cfg->pwm_mode);
    if (s != X_NUCLEO_IHM16M1_OK) goto fail;
    h->initialized = true;
    return X_NUCLEO_IHM16M1_OK;
fail:
    (void)h->io.pwm_enable(h->io.ctx, false);
    (void)h->io.gpio_write(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, true);
    memset(h, 0, sizeof(*h));
    return s;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_deinit(x_nucleo_ihm16m1_t *h)
{
    x_nucleo_ihm16m1_status_t s, t;
    s = ready(h); if (s != X_NUCLEO_IHM16M1_OK) return s;
    s = h->io.pwm_enable(h->io.ctx, false);
    t = h->io.gpio_write(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, true);
    memset(h, 0, sizeof(*h));
    return s != X_NUCLEO_IHM16M1_OK ? s : t;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_standby(x_nucleo_ihm16m1_t *h, bool standby)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    return h->io.gpio_write(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_STBY, standby);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_pwm_mode(
    x_nucleo_ihm16m1_t *h, x_nucleo_ihm16m1_pwm_mode_t mode)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    if (!valid_mode(mode)) return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    if (h->io.pwm_mode_set == NULL) return X_NUCLEO_IHM16M1_UNSUPPORTED;
    s = h->io.pwm_mode_set(h->io.ctx, mode);
    if (s == X_NUCLEO_IHM16M1_OK) h->mode = mode;
    return s;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_set_current_limit_reference(
    x_nucleo_ihm16m1_t *h, float volts)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    if (volts != volts || volts < 0.0f) return X_NUCLEO_IHM16M1_OUT_OF_RANGE;
    if (h->io.analog_write == NULL) return X_NUCLEO_IHM16M1_UNSUPPORTED;
    return h->io.analog_write(h->io.ctx, X_NUCLEO_IHM16M1_ANALOG_CURRENT_REFERENCE, volts);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_drive_motor(
    x_nucleo_ihm16m1_t *h, float u, float v, float w)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    if (u != u || v != v || w != w || u < 0.0f || u > 1.0f ||
        v < 0.0f || v > 1.0f || w < 0.0f || w > 1.0f)
        return X_NUCLEO_IHM16M1_OUT_OF_RANGE;
    s = h->io.pwm_set(h->io.ctx, X_NUCLEO_IHM16M1_PWM_U, u); if (s) return s;
    s = h->io.pwm_set(h->io.ctx, X_NUCLEO_IHM16M1_PWM_V, v); if (s) return s;
    return h->io.pwm_set(h->io.ctx, X_NUCLEO_IHM16M1_PWM_W, w);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_adc(
    x_nucleo_ihm16m1_t *h, x_nucleo_ihm16m1_adc_channel_t c, uint32_t *raw)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    if (raw == NULL || !valid_channel(c)) return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    if (h->io.adc_read == NULL) return X_NUCLEO_IHM16M1_UNSUPPORTED;
    return h->io.adc_read(h->io.ctx, c, raw);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_sample(
    x_nucleo_ihm16m1_t *h, x_nucleo_ihm16m1_adc_channel_t c,
    x_nucleo_ihm16m1_sample_t *sample)
{
    x_nucleo_ihm16m1_status_t s;
    if (sample == NULL) return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    s = x_nucleo_ihm16m1_read_adc(h, c, &sample->raw);
    sample->converted = false;
    sample->value = 0.0f;
    return s;
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_fault(x_nucleo_ihm16m1_t *h, bool *fault)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    if (fault == NULL) return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    if (h->io.gpio_read == NULL) return X_NUCLEO_IHM16M1_UNSUPPORTED;
    return h->io.gpio_read(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_EN_FAULT, fault);
}

x_nucleo_ihm16m1_status_t x_nucleo_ihm16m1_read_hall(
    x_nucleo_ihm16m1_t *h, bool *h1, bool *h2, bool *h3)
{
    x_nucleo_ihm16m1_status_t s = ready(h);
    if (s != X_NUCLEO_IHM16M1_OK) return s;
    if (h1 == NULL || h2 == NULL || h3 == NULL) return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    if (h->io.gpio_read == NULL) return X_NUCLEO_IHM16M1_UNSUPPORTED;
    s = h->io.gpio_read(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_H1, h1); if (s) return s;
    s = h->io.gpio_read(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_H2, h2); if (s) return s;
    return h->io.gpio_read(h->io.ctx, X_NUCLEO_IHM16M1_GPIO_H3, h3);
}
