#ifndef MOTOR_BOARD_IF_H
#define MOTOR_BOARD_IF_H

#include <stdbool.h>

typedef enum {
    MOTOR_BOARD_OK = 0,
    MOTOR_BOARD_ERROR = 1
} motor_board_status_t;

motor_board_status_t motor_board_prepare(void);
motor_board_status_t motor_board_apply_pwm(float duty_u, float duty_v, float duty_w);
motor_board_status_t motor_board_set_enabled(bool enabled);
motor_board_status_t motor_board_read_fault(bool *fault_active);
motor_board_status_t motor_board_shutdown(void);


#endif
