#include "app.h"

#include <stdbool.h>
#include <stdint.h>

#include "app_config.h"
#include "board_io_if.h"
#include "button_logic.h"
#include "button_logic_config.h"
#include "board.h"
#include "diagnostics.h"
#include "logger.h"
#include "network_status_if.h"
#include "mqtt_service_if.h"
#include "http_service_if.h"
#include "adc_service_if.h"
#include "pwm_service_if.h"
#include "osal.h"
#include "firmgen_telemetry.h"
#include "mx_usart2.h"   /* firmgen_telemetry */

static const char *g_tag = "app";
static bool g_application_state = false;
static bool g_adc_ready = false;
static bool g_pwm_ready = false;

static void app_process_task(void *arg);

void app_start(void)
{
    firmgen_telemetry_start(mx_usart2_uart_gethandle());
    osal_task_t h_task;

    if (!board_io_init())
    {
        LOGE(g_tag, "board GPIO initialization failed");
        return;
    }

    board_io_set_led(BOARD_IO_LED_STATUS, false);
    board_io_set_led(BOARD_IO_LED_NETWORK, false);
    board_io_set_led(BOARD_IO_LED_MQTT, false);
    diagnostics_log_boot(board_name());
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_NETWORK, "waiting-for-lease");
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_HTTP, "not-started");
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_MQTT, "disconnected");
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_ADC, "not-started");
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_PWM, "not-started");
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_BUTTON, "ready");

    mqtt_service_start();
    http_service_start();
    g_adc_ready = adc_service_start();
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_ADC,
                           g_adc_ready ? "sampling active" : "initialization failed");

    g_pwm_ready = pwm_service_start();
    diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_PWM,
                           g_pwm_ready ? "1 kHz 50 percent PWM active" : "initialization failed");


    h_task = osal_task_create(app_process_task, "board_demo_task",
                              APP_APP_TASK_STACK_BYTES, NULL,
                              APP_APP_TASK_PRIORITY);
    if (h_task == NULL)
    {
        LOGE(g_tag, "board demo task creation failed");
    }
    else
    {
        LOGI(g_tag, "board demo task started");
    }
}

static void app_process_task(void *arg)
{
    button_logic_t button_logic;
    bool b_status_led_on = false;
    uint8_t blink_edges = 0U;
    uint32_t health_sequence = 0U;
    uint32_t last_health_ms = osal_uptime_ms();
    uint32_t last_blink_ms = osal_uptime_ms();

    (void)arg;
    button_logic_init(&button_logic, board_io_button_is_pressed(), last_blink_ms);

    while (true)
    {
        uint32_t now_ms = osal_uptime_ms();
        bool b_pressed = board_io_button_is_pressed();

        if (button_logic_update(&button_logic, b_pressed, now_ms, APP_BUTTON_DEBOUNCE_MS))
        {
            bool b_new_state = button_logic_toggle_state(&g_application_state);
            mqtt_service_set_application_state(b_new_state);
            LOGI(g_tag, "button press: application_state=%u", b_new_state ? 1U : 0U);
        }

        if ((uint32_t)(now_ms - last_health_ms) >= APP_HEALTH_REPORT_MS)
        {
            health_sequence++;
            diagnostics_log_health(health_sequence, network_status_is_ready(),
                                   http_service_weather_ready() && http_service_time_ready(),
                                   mqtt_service_is_connected(), g_adc_ready, g_pwm_ready, true);
            last_health_ms = now_ms;
        }
        else
        {
            /* The next consolidated report is not due yet. */
        }

        if ((blink_edges < (uint8_t)(APP_STATUS_BLINK_COUNT * 2U)) &&
            ((uint32_t)(now_ms - last_blink_ms) >= APP_STATUS_BLINK_MS))
        {
            b_status_led_on = !b_status_led_on;
            board_io_set_led(BOARD_IO_LED_STATUS, b_status_led_on);
            blink_edges++;
            last_blink_ms = now_ms;
        }
        else
        {
            /* Boot indication is complete or its interval has not elapsed. */
        }

        osal_delay_ms(APP_BUTTON_POLL_MS);
    }
}
