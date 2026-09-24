#include "motor_platform.h"

#include <stddef.h>

#include "app_config.h"
#include "stm32g4xx_hal.h"

static TIM_HandleTypeDef g_timer;
static UART_HandleTypeDef g_debug_uart;
static bool g_debug_uart_initialized;

static void motor_platform_debug_uart_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    gpio.Pin = (GPIO_PIN_2 | GPIO_PIN_3);
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio);

    g_debug_uart.Instance = USART2;
    g_debug_uart.Init.BaudRate = 115200U;
    g_debug_uart.Init.WordLength = UART_WORDLENGTH_8B;
    g_debug_uart.Init.StopBits = UART_STOPBITS_1;
    g_debug_uart.Init.Parity = UART_PARITY_NONE;
    g_debug_uart.Init.Mode = UART_MODE_TX_RX;
    g_debug_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_debug_uart.Init.OverSampling = UART_OVERSAMPLING_16;
    g_debug_uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    g_debug_uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&g_debug_uart) == HAL_OK) {
        g_debug_uart_initialized = true;
    }
}

int _write(int file, char *data, int length)
{
    (void)file;

    if (!g_debug_uart_initialized || data == NULL || length <= 0) {
        return 0;
    }

    if (HAL_UART_Transmit(&g_debug_uart, (uint8_t *)data, (uint16_t)length, 1000U) != HAL_OK) {
        return 0;
    }

    return length;
}
static bool g_initialized;
static x_nucleo_ihm16m1_t *g_driver;
static union {
    max_align_t alignment;
    uint8_t storage[512U];
} g_driver_storage;

void motor_platform_led_on(void)
{
    GPIO_InitTypeDef gpio = {0};

    motor_platform_debug_uart_init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpio.Pin = APP_LED_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_LED_GPIO_PORT, &gpio);
    HAL_GPIO_WritePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN, GPIO_PIN_SET);
}

static void motor_platform_configure_gpio(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    gpio.Pin = (APP_MOTOR_PWM_U_PIN | APP_MOTOR_PWM_V_PIN | APP_MOTOR_PWM_W_PIN);
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = APP_MOTOR_PWM_ALTERNATE_FUNCTION;
    HAL_GPIO_Init(APP_MOTOR_PWM_PORT, &gpio);

    gpio.Pin = (APP_MOTOR_STBY_PIN | APP_MOTOR_MODE_PIN);
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_MOTOR_STBY_PORT, &gpio);

    gpio.Pin = (APP_MOTOR_EN_U_PIN | APP_MOTOR_EN_V_PIN | APP_MOTOR_EN_W_PIN);
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_MOTOR_EN_U_PORT, &gpio);

    gpio.Pin = APP_MOTOR_EN_FAULT_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_MOTOR_EN_FAULT_PORT, &gpio);

    HAL_GPIO_WritePin(APP_MOTOR_STBY_PORT, APP_MOTOR_STBY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(APP_MOTOR_MODE_PORT, APP_MOTOR_MODE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(APP_MOTOR_EN_U_PORT, APP_MOTOR_EN_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(APP_MOTOR_EN_V_PORT, APP_MOTOR_EN_V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(APP_MOTOR_EN_W_PORT, APP_MOTOR_EN_W_PIN, GPIO_PIN_RESET);
}

static x_nucleo_ihm16m1_status_t motor_platform_status(HAL_StatusTypeDef status)
{
    if (status == HAL_OK) {
        return X_NUCLEO_IHM16M1_OK;
    }

    return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
}

static x_nucleo_ihm16m1_status_t motor_platform_pwm_set(
    void *context,
    x_nucleo_ihm16m1_pwm_channel_t channel,
    float duty)
{
    uint32_t compare;

    (void)context;
    if (!g_initialized || channel > X_NUCLEO_IHM16M1_PWM_W || duty < 0.0f || duty > 1.0f) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }

    compare = (uint32_t)(duty * (float)(APP_MOTOR_PWM_PERIOD_TICKS - 1U));
    if (channel == X_NUCLEO_IHM16M1_PWM_U) {
        __HAL_TIM_SET_COMPARE(&g_timer, TIM_CHANNEL_1, compare);
    } else if (channel == X_NUCLEO_IHM16M1_PWM_V) {
        __HAL_TIM_SET_COMPARE(&g_timer, TIM_CHANNEL_2, compare);
    } else {
        __HAL_TIM_SET_COMPARE(&g_timer, TIM_CHANNEL_3, compare);
    }

    return X_NUCLEO_IHM16M1_OK;
}

static x_nucleo_ihm16m1_status_t motor_platform_pwm_enable(void *context, bool enable)
{
    (void)context;
    return motor_platform_enable(enable);
}

static x_nucleo_ihm16m1_status_t motor_platform_pwm_mode(
    void *context,
    x_nucleo_ihm16m1_pwm_mode_t mode)
{
    (void)context;
    if (mode != X_NUCLEO_IHM16M1_THREE_PWM_INPUTS) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }

    HAL_GPIO_WritePin(APP_MOTOR_MODE_PORT, APP_MOTOR_MODE_PIN, GPIO_PIN_RESET);
    return X_NUCLEO_IHM16M1_OK;
}

static x_nucleo_ihm16m1_status_t motor_platform_gpio_write(
    void *context,
    x_nucleo_ihm16m1_gpio_pin_t pin,
    bool level)
{
    (void)context;
    if (pin != X_NUCLEO_IHM16M1_GPIO_STBY) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }

    HAL_GPIO_WritePin(APP_MOTOR_STBY_PORT, APP_MOTOR_STBY_PIN,
                      level ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return X_NUCLEO_IHM16M1_OK;
}

static x_nucleo_ihm16m1_status_t motor_platform_gpio_read(
    void *context,
    x_nucleo_ihm16m1_gpio_pin_t pin,
    bool *level)
{
    (void)context;
    if (level == NULL || pin != X_NUCLEO_IHM16M1_GPIO_EN_FAULT) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }

    *level = (HAL_GPIO_ReadPin(APP_MOTOR_EN_FAULT_PORT, APP_MOTOR_EN_FAULT_PIN) == GPIO_PIN_RESET);
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t motor_platform_init(void)
{
    TIM_OC_InitTypeDef channel = {0};
    x_nucleo_ihm16m1_io_t io = {0};
    x_nucleo_ihm16m1_config_t config = {0};

    if (g_initialized) {
        return X_NUCLEO_IHM16M1_ALREADY_INITIALIZED;
    }

    motor_platform_configure_gpio();
    g_timer.Instance = TIM1;
    g_timer.Init.Prescaler = 0U;
    g_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timer.Init.Period = (APP_MOTOR_PWM_PERIOD_TICKS - 1U);
    g_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    g_timer.Init.RepetitionCounter = 0U;
    g_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_PWM_Init(&g_timer) != HAL_OK) {
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    channel.OCMode = TIM_OCMODE_PWM1;
    channel.Pulse = 0U;
    channel.OCPolarity = TIM_OCPOLARITY_HIGH;
    channel.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&g_timer, &channel, TIM_CHANNEL_1) != HAL_OK) {
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }
    if (HAL_TIM_PWM_ConfigChannel(&g_timer, &channel, TIM_CHANNEL_2) != HAL_OK) {
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }
    if (HAL_TIM_PWM_ConfigChannel(&g_timer, &channel, TIM_CHANNEL_3) != HAL_OK) {
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    g_driver = (x_nucleo_ihm16m1_t *)(void *)g_driver_storage.storage;
    io.pwm_set = motor_platform_pwm_set;
    io.pwm_enable = motor_platform_pwm_enable;
    io.pwm_mode_set = motor_platform_pwm_mode;
    io.gpio_write = motor_platform_gpio_write;
    io.gpio_read = motor_platform_gpio_read;
    config.pwm_mode = X_NUCLEO_IHM16M1_THREE_PWM_INPUTS;
    config.start_in_standby = true;

    {
        x_nucleo_ihm16m1_status_t status;

        status = x_nucleo_ihm16m1_init(g_driver, x_nucleo_ihm16m1_handle_size(), &io, &config);
        if (status == X_NUCLEO_IHM16M1_OK) {
            g_initialized = true;
        } else {
            g_driver = NULL;
        }

        return status;
    }
}

x_nucleo_ihm16m1_status_t motor_platform_enable(bool enable)
{
    HAL_StatusTypeDef status = HAL_OK;

    /* The portable driver deliberately disables PWM during its own init, before
       g_initialized is published. That safety operation must remain valid. */
    if (!g_initialized && enable) {
        return X_NUCLEO_IHM16M1_NOT_INITIALIZED;
    }

    if (enable) {
        status = HAL_TIM_PWM_Start(&g_timer, TIM_CHANNEL_1);
        if (status == HAL_OK) {
            status = HAL_TIM_PWM_Start(&g_timer, TIM_CHANNEL_2);
        }
        if (status == HAL_OK) {
            status = HAL_TIM_PWM_Start(&g_timer, TIM_CHANNEL_3);
        }
        if (status == HAL_OK) {
            HAL_GPIO_WritePin(APP_MOTOR_EN_U_PORT, APP_MOTOR_EN_U_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(APP_MOTOR_EN_V_PORT, APP_MOTOR_EN_V_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(APP_MOTOR_EN_W_PORT, APP_MOTOR_EN_W_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(APP_MOTOR_STBY_PORT, APP_MOTOR_STBY_PIN, GPIO_PIN_RESET);
        }
    } else {
        HAL_GPIO_WritePin(APP_MOTOR_EN_U_PORT, APP_MOTOR_EN_U_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(APP_MOTOR_EN_V_PORT, APP_MOTOR_EN_V_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(APP_MOTOR_EN_W_PORT, APP_MOTOR_EN_W_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(APP_MOTOR_STBY_PORT, APP_MOTOR_STBY_PIN, GPIO_PIN_SET);
        (void)HAL_TIM_PWM_Stop(&g_timer, TIM_CHANNEL_1);
        (void)HAL_TIM_PWM_Stop(&g_timer, TIM_CHANNEL_2);
        (void)HAL_TIM_PWM_Stop(&g_timer, TIM_CHANNEL_3);
    }

    return motor_platform_status(status);
}

x_nucleo_ihm16m1_status_t motor_platform_read_fault(bool *fault)
{
    if (g_driver == NULL || fault == NULL) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }

    return x_nucleo_ihm16m1_read_fault(g_driver, fault);
}

x_nucleo_ihm16m1_status_t motor_platform_drive(float duty_u, float duty_v, float duty_w)
{
    if (g_driver == NULL) {
        return X_NUCLEO_IHM16M1_NOT_INITIALIZED;
    }

    return x_nucleo_ihm16m1_drive_motor(g_driver, duty_u, duty_v, duty_w);
}

void motor_platform_delay_ms(uint32_t milliseconds)
{
    HAL_Delay(milliseconds);
}
