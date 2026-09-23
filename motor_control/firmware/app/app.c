#include "app.h"

#include <stddef.h>
#include <stdint.h>

#include "app_config.h"
#include "ihm16m1_platform.h"
#include "uart_platform.h"
#include "motor_control.h"
#include "uart_command.h"
#include "watchdog_platform.h"
#include "drive_safety.h"
#include "main.h"
#include "stm32g4xx_hal.h"

static void app_status_led_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpio.Pin = APP_LED_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_LED_GPIO_PORT, &gpio);
}

void app_start(void)
{
    x_nucleo_ihm16m1_io_t io = {0};
    x_nucleo_ihm16m1_config_t config = {0};
    static union {
        max_align_t alignment;
        uint8_t bytes[256U];
    } handle_storage;
    x_nucleo_ihm16m1_t *p_handle;
    motor_control_io_t control_io = {0};
    x_nucleo_ihm16m1_status_t status;

    app_status_led_init();
    HAL_GPIO_WritePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN, GPIO_PIN_RESET);

    if (!uart_platform_start()) {
        for (;;) {
            HAL_GPIO_TogglePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN);
            HAL_Delay(200U);
        }
    }
    HAL_GPIO_WritePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN, GPIO_PIN_SET);
    (void)uart_platform_write("BOOT\r\n", 6U);

    status = ihm16m1_platform_start();
    if (status != X_NUCLEO_IHM16M1_OK) {
        (void)uart_platform_write("IHM_FAIL\r\n", 10U);
        return;
    }
    (void)uart_platform_write("IHM_OK\r\n", 8U);

    io.pwm_set = ihm16m1_platform_pwm_set;
    io.pwm_enable = ihm16m1_platform_pwm_enable;
    io.pwm_mode_set = ihm16m1_platform_pwm_mode_set;
    io.gpio_write = ihm16m1_platform_gpio_write;
    io.gpio_read = ihm16m1_platform_gpio_read;
    io.adc_read = ihm16m1_platform_adc_read;
    io.delay_ms = ihm16m1_platform_delay;
    config.pwm_mode = X_NUCLEO_IHM16M1_THREE_PWM_INPUTS;
    config.start_in_standby = true;
    p_handle = (x_nucleo_ihm16m1_t *)(void *)handle_storage.bytes;

    if (x_nucleo_ihm16m1_handle_size() > sizeof(handle_storage)) {
        return;
    }
    status = x_nucleo_ihm16m1_init(p_handle, sizeof(handle_storage), &io, &config);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return;
    }

    control_io.p_driver = p_handle;
    control_io.pwm_enable = ihm16m1_platform_pwm_enable;
    control_io.p_context = NULL;
    control_io.p_read_safety = NULL;
    if (!motor_control_init(&control_io)) {
        return;
    }

    if (!uart_command_init()) {
        return;
    }
    (void)uart_platform_write("CONTROL_OK\r\n", 12U);

    if (!watchdog_platform_start()) {
        return;
    }

    {
        uint32_t heartbeat_ms = 0U;

        for (;;) {
            uart_command_process();
            motor_control_process(APP_MOTOR_CONTROL_PERIOD_MS);
            watchdog_platform_kick();
            heartbeat_ms += APP_MOTOR_CONTROL_PERIOD_MS;
            if (heartbeat_ms >= 500U) {
                HAL_GPIO_TogglePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN);
                heartbeat_ms = 0U;
            }
            ihm16m1_platform_delay(NULL, APP_MOTOR_CONTROL_PERIOD_MS);
        }
    }
}
