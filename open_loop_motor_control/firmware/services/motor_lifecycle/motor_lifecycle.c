#include "motor_lifecycle.h"
#include "app_config.h"
#include <stddef.h>

static float ramp_frequency(float frequency, float elapsed_seconds)
{
    float next_frequency = frequency + (APP_MOTOR_FREQUENCY_RAMP_HZ_PER_SECOND * elapsed_seconds);
    if (next_frequency > APP_MOTOR_TARGET_ELECTRICAL_FREQUENCY_HZ) {
        next_frequency = APP_MOTOR_TARGET_ELECTRICAL_FREQUENCY_HZ;
    }
    return next_frequency;
}

void motor_lifecycle_initialize(motor_lifecycle_t *lifecycle, uint32_t now_ms)
{
    if (lifecycle != NULL) {
        lifecycle->state = MOTOR_LIFECYCLE_STOPPED;
        lifecycle->last_update_ms = now_ms;
        motor_waveform_initialize(&lifecycle->waveform);
    }
}

bool motor_lifecycle_start(motor_lifecycle_t *lifecycle, bool fault_active)
{
    if ((lifecycle == NULL) || fault_active || (lifecycle->state == MOTOR_LIFECYCLE_FAULT)) {
        return false;
    }
    lifecycle->state = MOTOR_LIFECYCLE_STARTING;
    motor_waveform_initialize(&lifecycle->waveform);
    return true;
}

void motor_lifecycle_stop(motor_lifecycle_t *lifecycle)
{
    if (lifecycle != NULL) {
        lifecycle->state = MOTOR_LIFECYCLE_STOPPED;
        lifecycle->waveform.amplitude = 0.0f;
    }
}

void motor_lifecycle_fault(motor_lifecycle_t *lifecycle)
{
    if (lifecycle != NULL) {
        lifecycle->state = MOTOR_LIFECYCLE_FAULT;
        lifecycle->waveform.amplitude = 0.0f;
    }
}

bool motor_lifecycle_process(motor_lifecycle_t *lifecycle, uint32_t now_ms, motor_waveform_output_t *output)
{
    uint32_t elapsed_ms;
    float elapsed_seconds;

    if ((lifecycle == NULL) || (output == NULL)) {
        return false;
    }
    if ((lifecycle->state != MOTOR_LIFECYCLE_STARTING) && (lifecycle->state != MOTOR_LIFECYCLE_RUNNING)) {
        return false;
    }
    elapsed_ms = now_ms - lifecycle->last_update_ms;
    lifecycle->last_update_ms = now_ms;
    elapsed_seconds = (float)elapsed_ms / 1000.0f;
    lifecycle->waveform.electrical_frequency_hz = ramp_frequency(lifecycle->waveform.electrical_frequency_hz, elapsed_seconds);
    motor_waveform_step(&lifecycle->waveform, elapsed_seconds, output);
    lifecycle->state = MOTOR_LIFECYCLE_RUNNING;
    return true;
}
