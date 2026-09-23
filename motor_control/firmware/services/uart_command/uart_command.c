#include "uart_command.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "motor_control.h"
#include "uart_platform.h"

#define UART_COMMAND_LINE_CAPACITY 64U

static char g_line[UART_COMMAND_LINE_CAPACITY];

static const char *uart_command_state_name(motor_control_state_t state)
{
    static const char *const names[] = {
        "STOPPED", "ALIGN", "OPEN_LOOP", "RUN", "DECELERATING", "FAULT"
    };

    if ((uint32_t)state >= (sizeof(names) / sizeof(names[0]))) {
        return "UNKNOWN";
    }
    return names[state];
}

static const char *uart_command_direction_name(motor_control_direction_t direction)
{
    if (direction == MOTOR_CONTROL_DIRECTION_CCW) {
        return "CCW";
    }
    return "CW";
}

static void uart_command_status(void)
{
    char response[128U];
    motor_control_status_t status = motor_control_get_status();
    int length;

    length = snprintf(response, sizeof(response),
                      "STATE=%s TARGET_RPM=%ld EST_RPM=%ld DIR=%s FAULT=%s CODE=%lu\r\n",
                      uart_command_state_name(status.state),
                      (long)status.target_rpm,
                      (long)status.estimated_rpm,
                      uart_command_direction_name(status.direction),
                      status.fault_code == 0U ? "NONE" : "ACTIVE",
                      (unsigned long)status.fault_code);
    if (length > 0) {
        (void)uart_platform_write(response, (uint32_t)length);
    }
}

static bool uart_command_parse_speed(const char *p_argument)
{
    char *p_end;
    long value;

    if ((p_argument == NULL) || (*p_argument == '\0')) {
        return false;
    }
    value = strtol(p_argument, &p_end, 10);
    if ((*p_end != '\0') || (value < INT32_MIN) || (value > INT32_MAX)) {
        return false;
    }
    return motor_control_set_speed((int32_t)value);
}

bool uart_command_init(void)
{
    return uart_platform_start();
}

void uart_command_process(void)
{
    if (!uart_platform_read_line(g_line, sizeof(g_line))) {
        return;
    }

    if (strcmp(g_line, "MOTOR START") == 0) {
        (void)motor_control_start();
    } else if (strcmp(g_line, "MOTOR STOP") == 0) {
        motor_control_stop();
    } else if (strncmp(g_line, "SPEED ", 6U) == 0) {
        (void)uart_command_parse_speed(&g_line[6]);
    } else if (strcmp(g_line, "DIR CW") == 0) {
        (void)motor_control_set_direction(MOTOR_CONTROL_DIRECTION_CW);
    } else if (strcmp(g_line, "DIR CCW") == 0) {
        (void)motor_control_set_direction(MOTOR_CONTROL_DIRECTION_CCW);
    } else if (strcmp(g_line, "STATUS") == 0) {
        uart_command_status();
    } else {
        (void)uart_platform_write("ERROR\r\n", 7U);
    }
}
