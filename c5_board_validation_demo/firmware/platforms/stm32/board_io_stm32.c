#include "board_io_if.h"

#include <stdint.h>

#include "app_config.h"
#include "mx_hal_def.h"

static bool board_io_configure_output(hal_gpio_t gpio_port, uint32_t pin, uint32_t initial_level)
{
    hal_gpio_config_t gpio_config;

    gpio_config.mode = HAL_GPIO_MODE_OUTPUT;
    gpio_config.speed = HAL_GPIO_SPEED_FREQ_LOW;
    gpio_config.pull = HAL_GPIO_PULL_NO;
    gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
    gpio_config.init_state = initial_level;

    return (HAL_GPIO_Init(gpio_port, pin, &gpio_config) == HAL_OK);
}

bool board_io_init(void)
{
    hal_gpio_config_t gpio_config;
    bool b_ok = true;

    HAL_RCC_GPIOA_EnableClock();
    HAL_RCC_GPIOG_EnableClock();
    HAL_RCC_GPIOC_EnableClock();

    if (!board_io_configure_output(APP_LD1_GPIO_PORT, APP_LD1_GPIO_PIN, HAL_GPIO_PIN_RESET))
    {
        b_ok = false;
    }

    if (!board_io_configure_output(APP_LD2_GPIO_PORT, APP_LD2_GPIO_PIN, HAL_GPIO_PIN_SET))
    {
        b_ok = false;
    }

    if (!board_io_configure_output(APP_LD3_GPIO_PORT, APP_LD3_GPIO_PIN, HAL_GPIO_PIN_SET))
    {
        b_ok = false;
    }

    gpio_config.mode = HAL_GPIO_MODE_INPUT;
    gpio_config.speed = HAL_GPIO_SPEED_FREQ_LOW;
    gpio_config.pull = HAL_GPIO_PULL_NO;
    gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
    gpio_config.init_state = HAL_GPIO_PIN_RESET;

    if (HAL_GPIO_Init(APP_USER_BUTTON_GPIO_PORT, APP_USER_BUTTON_GPIO_PIN, &gpio_config) != HAL_OK)
    {
        b_ok = false;
    }

    return b_ok;
}

void board_io_set_led(board_io_led_t led, bool on)
{
    uint32_t level = HAL_GPIO_PIN_RESET;

    switch (led)
    {
        case BOARD_IO_LED_STATUS:
            level = on ? HAL_GPIO_PIN_SET : HAL_GPIO_PIN_RESET;
            HAL_GPIO_WritePin(APP_LD1_GPIO_PORT, APP_LD1_GPIO_PIN, level);
            break;
        case BOARD_IO_LED_NETWORK:
            /* NOTE: Assumes the board's PG indicator LEDs are active-low. */
            level = on ? HAL_GPIO_PIN_RESET : HAL_GPIO_PIN_SET;
            HAL_GPIO_WritePin(APP_LD2_GPIO_PORT, APP_LD2_GPIO_PIN, level);
            break;
        case BOARD_IO_LED_MQTT:
            /* NOTE: Assumes the board's PG indicator LEDs are active-low. */
            level = on ? HAL_GPIO_PIN_RESET : HAL_GPIO_PIN_SET;
            HAL_GPIO_WritePin(APP_LD3_GPIO_PORT, APP_LD3_GPIO_PIN, level);
            break;
        default:
            break;
    }
}

bool board_io_button_is_pressed(void)
{
    return (HAL_GPIO_ReadPin(APP_USER_BUTTON_GPIO_PORT, APP_USER_BUTTON_GPIO_PIN) == APP_USER_BUTTON_ACTIVE_LEVEL);
}
