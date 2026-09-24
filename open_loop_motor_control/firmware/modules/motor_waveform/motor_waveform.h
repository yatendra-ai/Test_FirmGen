#ifndef MOTOR_WAVEFORM_H
#define MOTOR_WAVEFORM_H

#include <stdbool.h>

typedef struct {
    float electrical_angle_radians;
    float electrical_frequency_hz;
    float amplitude;
    bool clockwise;
} motor_waveform_state_t;

typedef struct {
    float duty_u;
    float duty_v;
    float duty_w;
} motor_waveform_output_t;

void motor_waveform_initialize(motor_waveform_state_t *state);
void motor_waveform_set_direction(motor_waveform_state_t *state, bool clockwise);
void motor_waveform_step(motor_waveform_state_t *state, float elapsed_seconds, motor_waveform_output_t *output);

#endif
