#include "motor_sequence.h"

#include <stddef.h>

#include "app_config.h"

void motor_sequence_init(motor_sequence_t *sequence)
{
    if (sequence != NULL) {
        sequence->state = MOTOR_SEQUENCE_BOOT_DELAY;
        sequence->elapsed_ms = 0U;
    }
}

void motor_sequence_step(motor_sequence_t *sequence, uint32_t period_ms)
{
    if (sequence == NULL || sequence->state == MOTOR_SEQUENCE_STOPPED) {
        return;
    }

    sequence->elapsed_ms += period_ms;
    if (sequence->elapsed_ms < APP_MOTOR_START_DELAY_MS) {
        sequence->state = MOTOR_SEQUENCE_BOOT_DELAY;
    } else if (sequence->elapsed_ms < (APP_MOTOR_START_DELAY_MS + APP_MOTOR_START_RAMP_MS)) {
        sequence->state = MOTOR_SEQUENCE_STARTUP_RAMP;
    } else if (sequence->elapsed_ms < (APP_MOTOR_START_DELAY_MS + APP_MOTOR_START_RAMP_MS + APP_MOTOR_HOLD_MS)) {
        sequence->state = MOTOR_SEQUENCE_HOLD;
    } else if (sequence->elapsed_ms < (APP_MOTOR_START_DELAY_MS + APP_MOTOR_START_RAMP_MS + APP_MOTOR_HOLD_MS + APP_MOTOR_SHUTDOWN_RAMP_MS)) {
        sequence->state = MOTOR_SEQUENCE_SHUTDOWN_RAMP;
    } else {
        sequence->state = MOTOR_SEQUENCE_STOPPED;
    }
}

bool motor_sequence_is_active(const motor_sequence_t *sequence)
{
    return sequence != NULL && sequence->state != MOTOR_SEQUENCE_BOOT_DELAY &&
           sequence->state != MOTOR_SEQUENCE_STOPPED;
}

bool motor_sequence_is_stopped(const motor_sequence_t *sequence)
{
    return sequence == NULL || sequence->state == MOTOR_SEQUENCE_STOPPED;
}
