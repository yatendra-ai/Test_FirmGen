#include "app.h"
#include "osal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "app_config.h"
#include "logger.h"
#include "motor_board_if.h"
#include "motor_waveform.h"
#include "motor_lifecycle.h"
#include "command_transport_if.h"
#include "command_parser.h"
#include "uart_if.h"

static const char *TAG = "app";
static motor_lifecycle_t motor_lifecycle;
static uint32_t waveform_tick_ms;
static command_transport_line_t command_line;
static char command_buffer[APP_UART_COMMAND_BUFFER_SIZE];
static float command_frequency_hz;
static float command_duty;
static bool command_clockwise;

static void send_response(const char *text)
{
    uart_if_write(text, strlen(text));
}

static void report_status(void)
{
    char response[128];
    bool fault_active = false;
    bool fault_read_ok;
    const char *state = "Stopped";
    const char *direction = command_clockwise ? "CW" : "CCW";
    fault_read_ok = (motor_board_read_fault(&fault_active) == MOTOR_BOARD_OK);
    if (motor_lifecycle.state == MOTOR_LIFECYCLE_STARTING) {
        state = "Ramping";
    } else if (motor_lifecycle.state == MOTOR_LIFECYCLE_RUNNING) {
        state = "Running";
    } else if (motor_lifecycle.state == MOTOR_LIFECYCLE_FAULT) {
        state = "Fault";
    }
    (void)snprintf(response, sizeof(response), "STATUS state=%s dir=%s freq=%.3fHz duty=%.2f%% fault=%s\r\n", state, direction, command_frequency_hz, (command_duty * 100.0f), fault_read_ok ? (fault_active ? "ACTIVE" : "INACTIVE") : "UNKNOWN");
    send_response(response);
}

static void dispatch_command(const command_t *command)
{
    bool fault_active = false;

    if (command->kind == COMMAND_KIND_STATUS) {
        report_status();
    } else if (command->kind == COMMAND_KIND_DIRECTION) {
        command_clockwise = command->clockwise;
        motor_waveform_set_direction(&motor_lifecycle.waveform, command_clockwise);
        send_response("OK DIR\r\n");
    } else if (command->kind == COMMAND_KIND_FREQUENCY) {
        command_frequency_hz = command->value;
        motor_lifecycle.waveform.electrical_frequency_hz = command_frequency_hz;
        send_response("OK FREQ\r\n");
    } else if (command->kind == COMMAND_KIND_DUTY) {
        command_duty = (command->value > APP_MOTOR_MAX_AMPLITUDE) ? APP_MOTOR_MAX_AMPLITUDE : (command->value < 0.0f ? 0.0f : command->value);
        motor_lifecycle.waveform.amplitude = command_duty;
        send_response("OK DUTY\r\n");
    } else if (command->kind == COMMAND_KIND_START) {
        if ((motor_board_read_fault(&fault_active) == MOTOR_BOARD_OK) && !fault_active && motor_lifecycle_start(&motor_lifecycle, false) && (motor_board_set_enabled(true) == MOTOR_BOARD_OK)) {
            send_response("OK START\r\n");
        } else {
            motor_lifecycle_fault(&motor_lifecycle);
            (void)motor_board_shutdown();
            send_response("ERR START\r\n");
        }
    } else if (command->kind == COMMAND_KIND_STOP) {
        motor_lifecycle_stop(&motor_lifecycle);
        (void)motor_board_shutdown();
        send_response("OK STOP\r\n");
    } else {
        send_response("ERR UNKNOWN\r\n");
    }
}

static void process_uart(void)
{
    uint8_t byte;
    command_t command;

    while (uart_if_read_byte(&byte)) {
        if (command_transport_line_push(&command_line, (char)byte)) {
            if (command_line.overflow) {
                send_response("ERR INVALID\r\n");
            } else if (command_parser_parse(command_line.buffer, &command)) {
                dispatch_command(&command);
            } else {
                send_response("ERR INVALID\r\n");
            }
            command_line.length = 0U;
            command_line.overflow = false;
            command_line.buffer[0] = '\0';
        }
    }
}

void app_start(void)
{
    motor_board_status_t status = motor_board_prepare();
    uart_if_initialize();
    command_transport_line_initialize(&command_line, command_buffer, sizeof(command_buffer));
    command_frequency_hz = APP_MOTOR_INITIAL_ELECTRICAL_FREQUENCY_HZ;
    command_duty = APP_MOTOR_INITIAL_AMPLITUDE;
    command_clockwise = true;
    motor_lifecycle_initialize(&motor_lifecycle, osal_uptime_ms());
    waveform_tick_ms = osal_uptime_ms();
    (void)motor_board_shutdown();
    uart_if_write("app start\r\n", sizeof("app start\r\n") - 1U);
    LOGI(TAG, "firmware started");
    if (status != MOTOR_BOARD_OK) {
        LOGE(TAG, "STSPIN830 board preparation failed: %d", (int)status);
    }
}

void app_process(void)
{
    motor_waveform_output_t output;
    bool fault_active = false;
    uint32_t now_ms;
    uint32_t elapsed_ms;

    process_uart();
    now_ms = osal_uptime_ms();
    elapsed_ms = now_ms - waveform_tick_ms;
    if (elapsed_ms >= APP_MOTOR_CONTROL_PERIOD_MS) {
        waveform_tick_ms = now_ms;
        if ((motor_board_read_fault(&fault_active) != MOTOR_BOARD_OK) || fault_active) {
            motor_lifecycle_fault(&motor_lifecycle);
            (void)motor_board_shutdown();
        } else if (motor_lifecycle_process(&motor_lifecycle, now_ms, &output)) {
            (void)motor_board_apply_pwm(output.duty_u, output.duty_v, output.duty_w);
        } else {
            (void)motor_board_shutdown();
        }
    }
}
