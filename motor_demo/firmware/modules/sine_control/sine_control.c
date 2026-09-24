#include "sine_control.h"

#include <math.h>

static float sine_control_wrap(float cycles)
{
    while (cycles >= 1.0F) {
        cycles -= 1.0F;
    }
    while (cycles < 0.0F) {
        cycles += 1.0F;
    }
    return cycles;
}

void sine_control_init(sine_control_state_t *state)
{
    if (state != NULL) {
        state->phase_cycles = 0.0F;
        state->frequency_hz = 0.0F;
        state->amplitude = 0.0F;
    }
}

void sine_control_set_profile(sine_control_state_t *state, float frequency_hz, float amplitude)
{
    if (state != NULL) {
        state->frequency_hz = frequency_hz;
        state->amplitude = amplitude;
    }
}

void sine_control_update(sine_control_state_t *state, uint32_t update_period_ms)
{
    if (state != NULL) {
        state->phase_cycles = sine_control_wrap(
            state->phase_cycles + (state->frequency_hz * ((float)update_period_ms / 1000.0F)));
    }
}

sine_control_output_t sine_control_output(const sine_control_state_t *state)
{
    sine_control_output_t output = {0.5F, 0.5F, 0.5F};
    float angle;

    if (state != NULL) {
        angle = state->phase_cycles * 6.28318530718F;
        output.duty_u = 0.5F + (state->amplitude * sinf(angle));
        output.duty_v = 0.5F + (state->amplitude * sinf(angle - 2.09439510239F));
        output.duty_w = 0.5F + (state->amplitude * sinf(angle + 2.09439510239F));
    }

    return output;
}
