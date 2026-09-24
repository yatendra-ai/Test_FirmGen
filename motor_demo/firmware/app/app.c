#include "app.h"

#include <stdbool.h>

#include "app_config.h"
#include "logger.h"
#include "motor_platform.h"
#include "motor_sequence.h"
#include "safety_platform.h"
#include "sine_control.h"

static const char *g_tag = "app";

static bool app_drive_sine(sine_control_state_t *state)
{
    static bool b_first_drive_logged;
    sine_control_output_t output = sine_control_output(state);
    x_nucleo_ihm16m1_status_t status = motor_platform_drive(output.duty_u, output.duty_v, output.duty_w);

    if (!b_first_drive_logged) {
        LOGI(g_tag, "first drive status=%u duty_u=%.3f duty_v=%.3f duty_w=%.3f",
             (unsigned int)status, (double)output.duty_u, (double)output.duty_v,
             (double)output.duty_w);
        b_first_drive_logged = true;
    }

    return status == X_NUCLEO_IHM16M1_OK;
}

static void app_set_profile(sine_control_state_t *state, uint32_t elapsed_ms)
{
    float ramp_ratio;
    float frequency_hz;
    float amplitude;

    if (elapsed_ms < APP_MOTOR_START_RAMP_MS) {
        ramp_ratio = (float)elapsed_ms / (float)APP_MOTOR_START_RAMP_MS;
        frequency_hz = APP_MOTOR_START_FREQUENCY_HZ +
                       ((APP_MOTOR_RUN_FREQUENCY_HZ - APP_MOTOR_START_FREQUENCY_HZ) * ramp_ratio);
        amplitude = APP_MOTOR_START_AMPLITUDE +
                    ((APP_MOTOR_RUN_AMPLITUDE - APP_MOTOR_START_AMPLITUDE) * ramp_ratio);
    } else if (elapsed_ms < (APP_MOTOR_START_RAMP_MS + APP_MOTOR_HOLD_MS)) {
        frequency_hz = APP_MOTOR_RUN_FREQUENCY_HZ;
        amplitude = APP_MOTOR_RUN_AMPLITUDE;
    } else {
        ramp_ratio = (float)(elapsed_ms - APP_MOTOR_START_RAMP_MS - APP_MOTOR_HOLD_MS) /
                     (float)APP_MOTOR_SHUTDOWN_RAMP_MS;
        if (ramp_ratio > 1.0F) {
            ramp_ratio = 1.0F;
        }
        frequency_hz = APP_MOTOR_RUN_FREQUENCY_HZ * (1.0F - ramp_ratio);
        amplitude = APP_MOTOR_RUN_AMPLITUDE * (1.0F - ramp_ratio);
    }

    sine_control_set_profile(state, frequency_hz, amplitude);
}

void app_start(void)
{
    bool fault = false;
    x_nucleo_ihm16m1_status_t status;
    sine_control_state_t control;
    motor_sequence_t sequence;

    motor_platform_led_on();
    sine_control_init(&control);
    motor_sequence_init(&sequence);
    status = safety_platform_init();
    if (status != X_NUCLEO_IHM16M1_OK) {
        return;
    }
    LOGI(g_tag, "target=%s driver=%s motor=%s pole_pairs=%u supply_mv=%u",
         "NUCLEO-G431RB", APP_MOTOR_DRIVER_NAME, APP_MOTOR_NAME,
         APP_MOTOR_POLE_PAIRS, APP_MOTOR_SUPPLY_MV);

    status = motor_platform_init();
    if (status != X_NUCLEO_IHM16M1_OK) {
        LOGE(g_tag, "motor initialization failed: %u", (unsigned int)status);
        return;
    }

    while (sequence.state == MOTOR_SEQUENCE_BOOT_DELAY) {
        safety_platform_watchdog_refresh();
        motor_platform_delay_ms(APP_MOTOR_CONTROL_PERIOD_MS);
        motor_sequence_step(&sequence, APP_MOTOR_CONTROL_PERIOD_MS);
    }

    status = safety_platform_check_fault(&fault);
    LOGI(g_tag, "pre-enable status=%u fault=%u", (unsigned int)status, fault ? 1U : 0U);
    if (status != X_NUCLEO_IHM16M1_OK || fault) {
        LOGE(g_tag, "motor driver fault before enable");
        (void)safety_platform_disable();
        return;
    }

    status = motor_platform_enable(true);
    LOGI(g_tag, "enable status=%u", (unsigned int)status);
    if (status != X_NUCLEO_IHM16M1_OK) {
        LOGE(g_tag, "motor enable failed: %u", (unsigned int)status);
        return;
    }

    LOGI(g_tag, "automatic sinusoidal rotation started");
    while (!motor_sequence_is_stopped(&sequence)) {
        status = safety_platform_check_fault(&fault);
        app_set_profile(&control, sequence.elapsed_ms - APP_MOTOR_START_DELAY_MS);
        if (status != X_NUCLEO_IHM16M1_OK || fault || !app_drive_sine(&control)) {
            (void)safety_platform_disable();
            LOGE(g_tag, "sinusoidal control stopped by fault");
            return;
        }

        sine_control_update(&control, APP_MOTOR_CONTROL_PERIOD_MS);
        safety_platform_watchdog_refresh();
        motor_platform_delay_ms(APP_MOTOR_CONTROL_PERIOD_MS);
        motor_sequence_step(&sequence, APP_MOTOR_CONTROL_PERIOD_MS);
    }

    (void)safety_platform_disable();
    LOGI(g_tag, "automatic sinusoidal rotation complete");
}
