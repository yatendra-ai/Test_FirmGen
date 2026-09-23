#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

#include "x_nucleo_ihm16m1.h"
#include "drive_safety.h"

typedef enum {
    MOTOR_CONTROL_STOPPED = 0,
    MOTOR_CONTROL_ALIGN,
    MOTOR_CONTROL_OPEN_LOOP,
    MOTOR_CONTROL_RUNNING,
    MOTOR_CONTROL_DECELERATING,
    MOTOR_CONTROL_FAULT
} motor_control_state_t;

typedef enum {
    MOTOR_CONTROL_DIRECTION_CW = 1,
    MOTOR_CONTROL_DIRECTION_CCW = -1
} motor_control_direction_t;

typedef struct {
    x_nucleo_ihm16m1_t *p_driver;
    x_nucleo_ihm16m1_status_t (*pwm_enable)(void *p_context, bool enable);
    void *p_context;
    bool (*p_read_safety)(void *p_context,
                          drive_safety_measurements_t *p_measurements);
} motor_control_io_t;

typedef struct {
    motor_control_state_t state;
    motor_control_direction_t direction;
    int32_t target_rpm;
    int32_t estimated_rpm;
    uint32_t elapsed_ms;
    uint32_t startup_elapsed_ms;
    uint32_t fault_code;
} motor_control_status_t;

bool motor_control_init(const motor_control_io_t *p_io);
void motor_control_process(uint32_t elapsed_ms);
bool motor_control_start(void);
void motor_control_stop(void);
bool motor_control_set_speed(int32_t target_rpm);
bool motor_control_set_direction(motor_control_direction_t direction);
motor_control_status_t motor_control_get_status(void);

#endif /* MOTOR_CONTROL_H */
