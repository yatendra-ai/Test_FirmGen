#include "motor_control.h"

#include <stddef.h>

#include "app_config.h"

#define MOTOR_CONTROL_FAULT_OBSERVER_UNAVAILABLE 3U

static motor_control_io_t g_io;
static motor_control_status_t g_status;
static bool g_initialized;
static bool g_start_requested;
static bool g_direction_change_requested;

static bool motor_control_valid_direction(motor_control_direction_t direction)
{
    return (direction == MOTOR_CONTROL_DIRECTION_CW) ||
           (direction == MOTOR_CONTROL_DIRECTION_CCW);
}

static float motor_control_clamp_duty(float duty)
{
    if (duty < 0.0f) {
        return 0.0f;
    } else if (duty > 1.0f) {
        return 1.0f;
    } else {
        return duty;
    }
}

static bool motor_control_disable_pwm(void)
{
    x_nucleo_ihm16m1_status_t status;

    status = g_io.pwm_enable(g_io.p_context, false);
    return status == X_NUCLEO_IHM16M1_OK;
}

static bool motor_control_apply_duty(float duty)
{
    x_nucleo_ihm16m1_status_t status;
    float phase_u;
    float phase_v;
    float phase_w;

    phase_u = motor_control_clamp_duty(duty);
    phase_v = motor_control_clamp_duty(0.5f - (duty * 0.5f));
    phase_w = motor_control_clamp_duty(0.5f + (duty * 0.5f));
    status = x_nucleo_ihm16m1_drive_motor(g_io.p_driver, phase_u, phase_v, phase_w);
    return status == X_NUCLEO_IHM16M1_OK;
}

bool motor_control_init(const motor_control_io_t *p_io)
{
    bool disabled;

    if ((p_io == NULL) || (p_io->p_driver == NULL) ||
        (p_io->pwm_enable == NULL)) {
        return false;
    }

    g_io = *p_io;
    g_status.state = MOTOR_CONTROL_STOPPED;
    g_status.direction = MOTOR_CONTROL_DIRECTION_CW;
    g_status.target_rpm = APP_MOTOR_DEFAULT_SPEED_RPM;
    g_status.estimated_rpm = 0;
    g_status.elapsed_ms = 0U;
    g_status.startup_elapsed_ms = 0U;
    g_status.fault_code = 0U;
    g_start_requested = false;
    g_direction_change_requested = false;
    disabled = motor_control_disable_pwm();
    g_initialized = disabled;
    return disabled;
}

bool motor_control_start(void)
{
    if (!g_initialized || (g_status.state == MOTOR_CONTROL_FAULT)) {
        return false;
    }

    g_start_requested = true;
    return true;
}

void motor_control_stop(void)
{
    if (!g_initialized) {
        return;
    }

    g_start_requested = false;
    g_direction_change_requested = false;
    g_status.state = MOTOR_CONTROL_DECELERATING;
}

bool motor_control_set_speed(int32_t target_rpm)
{
    if (!g_initialized || (target_rpm < APP_MOTOR_MIN_SPEED_RPM) ||
        (target_rpm > APP_MOTOR_MAX_SPEED_RPM)) {
        return false;
    }

    g_status.target_rpm = target_rpm;
    return true;
}

bool motor_control_set_direction(motor_control_direction_t direction)
{
    if (!g_initialized || !motor_control_valid_direction(direction)) {
        return false;
    }

    if ((g_status.state == MOTOR_CONTROL_RUNNING) &&
        (g_status.direction != direction)) {
        g_status.direction = direction;
        g_direction_change_requested = true;
        g_status.state = MOTOR_CONTROL_DECELERATING;
    } else {
        g_status.direction = direction;
    }
    return true;
}

static void motor_control_process_startup(void)
{
    if (g_status.state == MOTOR_CONTROL_ALIGN) {
        if (g_status.startup_elapsed_ms >= APP_MOTOR_ALIGN_TIME_MS) {
            g_status.state = MOTOR_CONTROL_OPEN_LOOP;
        }
    } else if (g_status.state == MOTOR_CONTROL_OPEN_LOOP) {
        if (g_status.startup_elapsed_ms >= APP_MOTOR_STARTUP_TIMEOUT_MS) {
            (void)motor_control_disable_pwm();
            g_status.state = MOTOR_CONTROL_FAULT;
            g_status.fault_code = APP_MOTOR_FAULT_STARTUP;
        } else if (APP_MOTOR_SENSORLESS_OBSERVER_ENABLED != 0U) {
            (void)motor_control_disable_pwm();
            g_status.state = MOTOR_CONTROL_FAULT;
            g_status.fault_code = MOTOR_CONTROL_FAULT_OBSERVER_UNAVAILABLE;
        } else if (g_status.startup_elapsed_ms >= APP_MOTOR_OPEN_LOOP_TIME_MS) {
            g_status.estimated_rpm = g_status.target_rpm;
            g_status.state = MOTOR_CONTROL_RUNNING;
        }
    } else {
        g_status.state = MOTOR_CONTROL_ALIGN;
        g_status.startup_elapsed_ms = 0U;
    }
}

void motor_control_process(uint32_t elapsed_ms)
{
    bool applied;

    drive_safety_measurements_t measurements = {0};
    drive_safety_fault_t safety_fault;

    if (!g_initialized) {
        return;
    }

    if ((g_io.p_read_safety != NULL) &&
        g_io.p_read_safety(g_io.p_context, &measurements) &&
        !drive_safety_validate(&measurements, &safety_fault)) {
        (void)motor_control_disable_pwm();
        g_status.state = MOTOR_CONTROL_FAULT;
        g_status.fault_code = (uint32_t)safety_fault;
        return;
    }

    g_status.elapsed_ms += elapsed_ms;
    if (g_status.state == MOTOR_CONTROL_STOPPED) {
        if (g_start_requested) {
            g_status.startup_elapsed_ms = 0U;
            g_status.state = MOTOR_CONTROL_ALIGN;
        }
    } else if ((g_status.state == MOTOR_CONTROL_ALIGN) ||
               (g_status.state == MOTOR_CONTROL_OPEN_LOOP)) {
        g_status.startup_elapsed_ms += elapsed_ms;
        applied = motor_control_apply_duty(0.5f);
        if (!applied) {
            (void)motor_control_disable_pwm();
            g_status.state = MOTOR_CONTROL_FAULT;
            g_status.fault_code = APP_MOTOR_FAULT_PWM;
        } else {
            motor_control_process_startup();
        }
    } else if (g_status.state == MOTOR_CONTROL_RUNNING) {
        if (g_direction_change_requested) {
            g_status.state = MOTOR_CONTROL_DECELERATING;
        } else {
            g_status.estimated_rpm = g_status.target_rpm;
            applied = motor_control_apply_duty(0.5f);
            if (!applied) {
                (void)motor_control_disable_pwm();
                g_status.state = MOTOR_CONTROL_FAULT;
                g_status.fault_code = APP_MOTOR_FAULT_PWM;
            }
        }
    } else if (g_status.state == MOTOR_CONTROL_DECELERATING) {
        g_status.estimated_rpm = 0;
        (void)motor_control_disable_pwm();
        g_start_requested = false;
        g_direction_change_requested = false;
        g_status.state = MOTOR_CONTROL_STOPPED;
    } else {
        (void)motor_control_disable_pwm();
    }
}

motor_control_status_t motor_control_get_status(void)
{
    return g_status;
}
