#ifndef SINE_CONTROL_H
#define SINE_CONTROL_H

#include <stdint.h>

typedef struct {
    float phase_cycles;
    float frequency_hz;
    float amplitude;
} sine_control_state_t;

typedef struct {
    float duty_u;
    float duty_v;
    float duty_w;
} sine_control_output_t;

void sine_control_init(sine_control_state_t *state);
void sine_control_set_profile(sine_control_state_t *state, float frequency_hz, float amplitude);
void sine_control_update(sine_control_state_t *state, uint32_t update_period_ms);
sine_control_output_t sine_control_output(const sine_control_state_t *state);

#endif /* SINE_CONTROL_H */
