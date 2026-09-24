#ifndef MOTOR_SEQUENCE_H
#define MOTOR_SEQUENCE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    MOTOR_SEQUENCE_BOOT_DELAY = 0,
    MOTOR_SEQUENCE_STARTUP_RAMP,
    MOTOR_SEQUENCE_HOLD,
    MOTOR_SEQUENCE_SHUTDOWN_RAMP,
    MOTOR_SEQUENCE_STOPPED
} motor_sequence_state_t;

typedef struct {
    motor_sequence_state_t state;
    uint32_t elapsed_ms;
} motor_sequence_t;

void motor_sequence_init(motor_sequence_t *sequence);
void motor_sequence_step(motor_sequence_t *sequence, uint32_t period_ms);
bool motor_sequence_is_active(const motor_sequence_t *sequence);
bool motor_sequence_is_stopped(const motor_sequence_t *sequence);

#endif /* MOTOR_SEQUENCE_H */
