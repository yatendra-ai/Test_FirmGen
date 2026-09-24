#include "motor_waveform.h"
#include "app_config.h"
#include <stddef.h>
#include <math.h>

#define MOTOR_WAVEFORM_TWO_PI 6.28318530717958647692f
#define MOTOR_WAVEFORM_PHASE_SHIFT 2.09439510239319549231f

static float clamp_amplitude(float amplitude)
{
    if (amplitude < 0.0f) {
        return 0.0f;
    }
    if (amplitude > APP_MOTOR_MAX_AMPLITUDE) {
        return APP_MOTOR_MAX_AMPLITUDE;
    }
    return amplitude;
}

static float wrap_angle(float angle)
{
    while (angle >= MOTOR_WAVEFORM_TWO_PI) {
        angle -= MOTOR_WAVEFORM_TWO_PI;
    }
    while (angle < 0.0f) {
        angle += MOTOR_WAVEFORM_TWO_PI;
    }
    return angle;
}

void motor_waveform_initialize(motor_waveform_state_t *state)
{
    if (state != NULL) {
        state->electrical_angle_radians = 0.0f;
        state->electrical_frequency_hz = APP_MOTOR_INITIAL_ELECTRICAL_FREQUENCY_HZ;
        state->amplitude = APP_MOTOR_INITIAL_AMPLITUDE;
        state->clockwise = true;
    }
}

void motor_waveform_set_direction(motor_waveform_state_t *state, bool clockwise)
{
    if (state != NULL) {
        state->clockwise = clockwise;
    }
}

void motor_waveform_step(motor_waveform_state_t *state, float elapsed_seconds, motor_waveform_output_t *output)
{
    float direction;
    float angle;

    if ((state == NULL) || (output == NULL)) {
        return;
    }
    state->amplitude = clamp_amplitude(state->amplitude);
    direction = state->clockwise ? 1.0f : -1.0f;
    state->electrical_angle_radians = wrap_angle(state->electrical_angle_radians + (direction * MOTOR_WAVEFORM_TWO_PI * state->electrical_frequency_hz * elapsed_seconds));
    angle = state->electrical_angle_radians;
    output->duty_u = 0.5f + (0.5f * state->amplitude * sinf(angle));
    output->duty_v = 0.5f + (0.5f * state->amplitude * sinf(angle - MOTOR_WAVEFORM_PHASE_SHIFT));
    output->duty_w = 0.5f + (0.5f * state->amplitude * sinf(angle + MOTOR_WAVEFORM_PHASE_SHIFT));
}
