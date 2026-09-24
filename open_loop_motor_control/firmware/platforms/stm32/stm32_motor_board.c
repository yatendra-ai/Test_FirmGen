#include "motor_board_if.h"
#include "app_config.h"
#include "main.h"
#include "x_nucleo_ihm16m1.h"
#include "stm32g4xx_hal.h"
#include <stdbool.h>
#include <string.h>

static TIM_HandleTypeDef htim1;
static uint8_t driver_storage[256U];
static x_nucleo_ihm16m1_t *driver_handle;

static x_nucleo_ihm16m1_status_t pwm_set(void *ctx, x_nucleo_ihm16m1_pwm_channel_t channel, float duty)
{
    uint32_t pulse = (uint32_t)(duty * (float)__HAL_TIM_GET_AUTORELOAD(&htim1));
    uint32_t timer_channel = TIM_CHANNEL_1;
    (void)ctx;
    if (channel == X_NUCLEO_IHM16M1_PWM_V) {
        timer_channel = TIM_CHANNEL_2;
    } else if (channel == X_NUCLEO_IHM16M1_PWM_W) {
        timer_channel = TIM_CHANNEL_3;
    } else {
        timer_channel = TIM_CHANNEL_1;
    }
    __HAL_TIM_SET_COMPARE(&htim1, timer_channel, pulse);
    return X_NUCLEO_IHM16M1_OK;
}

static x_nucleo_ihm16m1_status_t pwm_enable(void *ctx, bool enable)
{
    HAL_StatusTypeDef hal_status;
    (void)ctx;
    if (enable) {
        hal_status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        if (hal_status == HAL_OK) {
            hal_status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        }
        if (hal_status == HAL_OK) {
            hal_status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
        }
    } else {
        hal_status = HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        (void)HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        (void)HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    }
    return (hal_status == HAL_OK) ? X_NUCLEO_IHM16M1_OK : X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
}

static x_nucleo_ihm16m1_status_t pwm_mode_set(void *ctx, x_nucleo_ihm16m1_pwm_mode_t mode)
{
    (void)ctx;
    return (mode == X_NUCLEO_IHM16M1_THREE_PWM_INPUTS) ? X_NUCLEO_IHM16M1_OK : X_NUCLEO_IHM16M1_UNSUPPORTED;
}

static x_nucleo_ihm16m1_status_t gpio_write(void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool level)
{
    (void)ctx;
    if (pin == X_NUCLEO_IHM16M1_GPIO_STBY) {
        HAL_GPIO_WritePin(APP_MOTOR_STBY_GPIO_PORT, APP_MOTOR_STBY_GPIO_PIN, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
        return X_NUCLEO_IHM16M1_OK;
    }
    return X_NUCLEO_IHM16M1_UNSUPPORTED;
}

static x_nucleo_ihm16m1_status_t gpio_read(void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool *level)
{
    (void)ctx;
    if ((pin != X_NUCLEO_IHM16M1_GPIO_EN_FAULT) || (level == NULL)) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    *level = (HAL_GPIO_ReadPin(APP_MOTOR_EN_FAULT_GPIO_PORT, APP_MOTOR_EN_FAULT_GPIO_PIN) == GPIO_PIN_SET);
    return X_NUCLEO_IHM16M1_OK;
}

static void configure_gpio(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio.Pin = APP_LED_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(APP_LED_GPIO_PORT, &gpio);
    HAL_GPIO_WritePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN, GPIO_PIN_SET);

    HAL_GPIO_WritePin(APP_MOTOR_STBY_GPIO_PORT, APP_MOTOR_STBY_GPIO_PIN, GPIO_PIN_SET);
    gpio.Pin = APP_MOTOR_STBY_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(APP_MOTOR_STBY_GPIO_PORT, &gpio);
    gpio.Pin = APP_MOTOR_EN_FAULT_GPIO_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(APP_MOTOR_EN_FAULT_GPIO_PORT, &gpio);
    gpio.Pin = APP_MOTOR_PHASE_U_GPIO_PIN | APP_MOTOR_PHASE_V_GPIO_PIN | APP_MOTOR_PHASE_W_GPIO_PIN;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOA, &gpio);

    HAL_GPIO_WritePin(APP_MOTOR_PHASE_U_GPIO_PORT, APP_MOTOR_PHASE_U_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(APP_MOTOR_PHASE_V_GPIO_PORT, APP_MOTOR_PHASE_V_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(APP_MOTOR_PHASE_W_GPIO_PORT, APP_MOTOR_PHASE_W_GPIO_PIN, GPIO_PIN_RESET);
}

static void configure_timer(void)
{
    TIM_OC_InitTypeDef channel = {0};
    __HAL_RCC_TIM1_CLK_ENABLE();
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 0U;
    htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
    htim1.Init.Period = 4249U;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0U;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    (void)HAL_TIM_PWM_Init(&htim1);
    channel.OCMode = TIM_OCMODE_PWM1;
    channel.Pulse = 0U;
    channel.OCPolarity = TIM_OCPOLARITY_HIGH;
    channel.OCFastMode = TIM_OCFAST_DISABLE;
    (void)HAL_TIM_PWM_ConfigChannel(&htim1, &channel, TIM_CHANNEL_1);
    (void)HAL_TIM_PWM_ConfigChannel(&htim1, &channel, TIM_CHANNEL_2);
    (void)HAL_TIM_PWM_ConfigChannel(&htim1, &channel, TIM_CHANNEL_3);
}

motor_board_status_t motor_board_apply_pwm(float duty_u, float duty_v, float duty_w)
{
    x_nucleo_ihm16m1_status_t status;

    if (driver_handle == NULL) {
        return MOTOR_BOARD_ERROR;
    }
    status = x_nucleo_ihm16m1_drive_motor(driver_handle, duty_u, duty_v, duty_w);
    return (status == X_NUCLEO_IHM16M1_OK) ? MOTOR_BOARD_OK : MOTOR_BOARD_ERROR;
}

motor_board_status_t motor_board_read_fault(bool *fault_active)
{
    x_nucleo_ihm16m1_status_t status;
    bool fault_level;

    if ((driver_handle == NULL) || (fault_active == NULL)) {
        return MOTOR_BOARD_ERROR;
    }
    status = x_nucleo_ihm16m1_read_fault(driver_handle, &fault_level);
    if (status != X_NUCLEO_IHM16M1_OK) {
        return MOTOR_BOARD_ERROR;
    }
    *fault_active = (fault_level == (APP_MOTOR_FAULT_ACTIVE_LEVEL != 0U));
    return MOTOR_BOARD_OK;
}

motor_board_status_t motor_board_shutdown(void)
{
    motor_board_status_t status = MOTOR_BOARD_OK;

    if (driver_handle == NULL) {
        return MOTOR_BOARD_ERROR;
    }
    if (motor_board_apply_pwm(0.0f, 0.0f, 0.0f) != MOTOR_BOARD_OK) {
        status = MOTOR_BOARD_ERROR;
    }
    if (pwm_enable(NULL, false) != X_NUCLEO_IHM16M1_OK) {
        status = MOTOR_BOARD_ERROR;
    }
    if (x_nucleo_ihm16m1_set_standby(driver_handle, true) != X_NUCLEO_IHM16M1_OK) {
        status = MOTOR_BOARD_ERROR;
    }
    return status;
}

motor_board_status_t motor_board_set_enabled(bool enabled)
{
    x_nucleo_ihm16m1_status_t status;

    if (driver_handle == NULL) {
        return MOTOR_BOARD_ERROR;
    }
    status = x_nucleo_ihm16m1_set_standby(driver_handle, !enabled);
    if (status == X_NUCLEO_IHM16M1_OK) {
        status = pwm_enable(NULL, enabled);
    }
    return (status == X_NUCLEO_IHM16M1_OK) ? MOTOR_BOARD_OK : MOTOR_BOARD_ERROR;
}

motor_board_status_t motor_board_prepare(void)
{
    x_nucleo_ihm16m1_io_t io = {0};
    x_nucleo_ihm16m1_config_t config = {0};
    configure_gpio();
    configure_timer();
    io.pwm_set = pwm_set;
    io.pwm_enable = pwm_enable;
    io.pwm_mode_set = pwm_mode_set;
    io.gpio_write = gpio_write;
    io.gpio_read = gpio_read;
    config.pwm_mode = X_NUCLEO_IHM16M1_THREE_PWM_INPUTS;
    config.start_in_standby = true;
    driver_handle = (x_nucleo_ihm16m1_t *)(void *)driver_storage;
    if (x_nucleo_ihm16m1_init(driver_handle, sizeof(driver_storage), &io, &config) != X_NUCLEO_IHM16M1_OK) {
        (void)memset(driver_storage, 0, sizeof(driver_storage));
        driver_handle = NULL;
        return MOTOR_BOARD_ERROR;
    }
    return MOTOR_BOARD_OK;
}
