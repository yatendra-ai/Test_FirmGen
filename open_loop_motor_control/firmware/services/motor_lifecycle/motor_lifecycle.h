#ifndef MOTOR_LIFECYCLE_H
#define MOTOR_LIFECYCLE_H

#include <stdbool.h>
#include <stdint.h>
#include "motor_waveform.h"

typedef enum {
    MOTOR_LIFECYCLE_STOPPED = 0,
    MOTOR_LIFECYCLE_STARTING,
    MOTOR_LIFECYCLE_RUNNING,
    MOTOR_LIFECYCLE_FAULT
} motor_lifecycle_state_t;

typedef struct {
    motor_lifecycle_state_t state;
    motor_waveform_state_t waveform;
    uint32_t last_update_ms;
} motor_lifecycle_t;

void motor_lifecycle_initialize(motor_lifecycle_t *lifecycle, uint32_t now_ms);
bool motor_lifecycle_start(motor_lifecycle_t *lifecycle, bool fault_active);
void motor_lifecycle_stop(motor_lifecycle_t *lifecycle);
void motor_lifecycle_fault(motor_lifecycle_t *lifecycle);
bool motor_lifecycle_process(motor_lifecycle_t *lifecycle, uint32_t now_ms, motor_waveform_output_t *output);

#endif
