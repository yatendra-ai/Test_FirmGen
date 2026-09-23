#include "ihm16m1_platform.h"

#include <stddef.h>

#include "app_config.h"
#include "main.h"
#include "uart_platform.h"

static ADC_HandleTypeDef g_hadc1;
static TIM_HandleTypeDef g_htim1;
static bool g_started;

static void ihm16m1_platform_gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_ADC12_CLK_ENABLE();
    GPIO_InitTypeDef adc_gpio = {0};
    adc_gpio.Pin = APP_PHASE_U_ADC_PIN | APP_PHASE_V_ADC_PIN | APP_PHASE_W_ADC_PIN;
    adc_gpio.Mode = GPIO_MODE_ANALOG;
    adc_gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &adc_gpio);

    RCC_PeriphCLKInitTypeDef adc_clock = {0};
    adc_clock.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
    adc_clock.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
    (void)HAL_RCCEx_PeriphCLKConfig(&adc_clock);

    gpio.Pin = (APP_PWM_U_GPIO_PIN | APP_PWM_V_GPIO_PIN | APP_PWM_W_GPIO_PIN);
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = APP_PWM_GPIO_AF;
    HAL_GPIO_Init(APP_PWM_U_GPIO_PORT, &gpio);

    gpio.Pin = APP_EN_FAULT_GPIO_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_EN_FAULT_GPIO_PORT, &gpio);

    gpio.Pin = APP_STBY_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(APP_STBY_GPIO_PORT, &gpio);
    HAL_GPIO_WritePin(APP_STBY_GPIO_PORT, APP_STBY_GPIO_PIN, APP_STBY_ACTIVE_LEVEL);
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_start(void)
{
    TIM_OC_InitTypeDef output = {0};

    if (g_started) {
        return X_NUCLEO_IHM16M1_ALREADY_INITIALIZED;
    }

    ihm16m1_platform_gpio_init();

    g_htim1.Instance = TIM1;
    g_htim1.Init.Prescaler = 0U;
    g_htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
    g_htim1.Init.Period = APP_PWM_PERIOD_TICKS - 1U;
    g_htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    g_htim1.Init.RepetitionCounter = 1U;
    g_htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&g_htim1) != HAL_OK) {
        (void)uart_platform_write("IHM_TIM_INIT_FAIL\r\n", 18U);
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    output.OCMode = TIM_OCMODE_PWM1;
    output.Pulse = 0U;
    output.OCPolarity = TIM_OCPOLARITY_HIGH;
    output.OCFastMode = TIM_OCFAST_DISABLE;
    output.OCIdleState = TIM_OCIDLESTATE_RESET;
    if ((HAL_TIM_PWM_ConfigChannel(&g_htim1, &output, TIM_CHANNEL_1) != HAL_OK) ||
        (HAL_TIM_PWM_ConfigChannel(&g_htim1, &output, TIM_CHANNEL_2) != HAL_OK) ||
        (HAL_TIM_PWM_ConfigChannel(&g_htim1, &output, TIM_CHANNEL_3) != HAL_OK)) {
        (void)uart_platform_write("IHM_TIM_CFG_FAIL\r\n", 18U);
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    if ((HAL_TIM_PWM_Start(&g_htim1, TIM_CHANNEL_1) != HAL_OK) ||
        (HAL_TIM_PWM_Start(&g_htim1, TIM_CHANNEL_2) != HAL_OK) ||
        (HAL_TIM_PWM_Start(&g_htim1, TIM_CHANNEL_3) != HAL_OK)) {
        (void)uart_platform_write("IHM_TIM_START_FAIL\r\n", 19U);
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }
    __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_1, 0U);
    __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_2, 0U);
    __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_3, 0U);

    g_hadc1.Instance = ADC1;
    g_hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    g_hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    g_hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    g_hadc1.Init.LowPowerAutoWait = DISABLE;
    g_hadc1.Init.ContinuousConvMode = DISABLE;
    g_hadc1.Init.NbrOfConversion = 1U;
    g_hadc1.Init.DiscontinuousConvMode = DISABLE;
    g_hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    g_hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    g_hadc1.Init.DMAContinuousRequests = DISABLE;
    g_hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    g_hadc1.Init.OversamplingMode = DISABLE;
    if (HAL_ADC_Init(&g_hadc1) != HAL_OK) {
        (void)uart_platform_write("IHM_ADC_INIT_FAIL\r\n", 18U);
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    if (HAL_ADCEx_Calibration_Start(&g_hadc1, ADC_SINGLE_ENDED) != HAL_OK) {
        (void)uart_platform_write("IHM_ADC_CAL_FAIL\r\n", 18U);
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }

    g_started = true;
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_pwm_set(
    void *ctx, x_nucleo_ihm16m1_pwm_channel_t channel, float duty)
{
    uint32_t pulse;
    (void)ctx;

    if ((duty < 0.0f) || (duty > 1.0f)) {
        return X_NUCLEO_IHM16M1_OUT_OF_RANGE;
    }
    pulse = (uint32_t)(duty * (float)(APP_PWM_PERIOD_TICKS - 1U));
    if (channel == X_NUCLEO_IHM16M1_PWM_U) {
        __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_1, pulse);
    } else if (channel == X_NUCLEO_IHM16M1_PWM_V) {
        __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_2, pulse);
    } else if (channel == X_NUCLEO_IHM16M1_PWM_W) {
        __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_3, pulse);
    } else {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_pwm_enable(void *ctx, bool enable)
{
    (void)ctx;
    if (!enable) {
        __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_1, 0U);
        __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_2, 0U);
        __HAL_TIM_SET_COMPARE(&g_htim1, TIM_CHANNEL_3, 0U);
        (void)HAL_GPIO_WritePin(APP_STBY_GPIO_PORT, APP_STBY_GPIO_PIN, GPIO_PIN_SET);
    }
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_pwm_mode_set(
    void *ctx, x_nucleo_ihm16m1_pwm_mode_t mode)
{
    (void)ctx;
    return mode == X_NUCLEO_IHM16M1_THREE_PWM_INPUTS
        ? X_NUCLEO_IHM16M1_OK : X_NUCLEO_IHM16M1_UNSUPPORTED;
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_gpio_write(
    void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool level)
{
    (void)ctx;
    if (pin != X_NUCLEO_IHM16M1_GPIO_STBY) {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    HAL_GPIO_WritePin(APP_STBY_GPIO_PORT, APP_STBY_GPIO_PIN,
                      level ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_gpio_read(
    void *ctx, x_nucleo_ihm16m1_gpio_pin_t pin, bool *level)
{
    (void)ctx;
    if ((pin != X_NUCLEO_IHM16M1_GPIO_EN_FAULT) || (level == NULL)) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    *level = (HAL_GPIO_ReadPin(APP_EN_FAULT_GPIO_PORT, APP_EN_FAULT_GPIO_PIN) == GPIO_PIN_RESET);
    return X_NUCLEO_IHM16M1_OK;
}

x_nucleo_ihm16m1_status_t ihm16m1_platform_adc_read(
    void *ctx, x_nucleo_ihm16m1_adc_channel_t channel, uint32_t *raw)
{
    uint32_t adc_channel;
    ADC_ChannelConfTypeDef adc_config = {0};
    (void)ctx;
    if (raw == NULL) {
        return X_NUCLEO_IHM16M1_INVALID_ARGUMENT;
    }
    if (channel == X_NUCLEO_IHM16M1_ADC_PHASE_U_CURRENT) {
        adc_channel = APP_PHASE_U_ADC_CHANNEL;
    } else if (channel == X_NUCLEO_IHM16M1_ADC_PHASE_V_CURRENT) {
        adc_channel = APP_PHASE_V_ADC_CHANNEL;
    } else if (channel == X_NUCLEO_IHM16M1_ADC_PHASE_W_CURRENT) {
        adc_channel = APP_PHASE_W_ADC_CHANNEL;
    } else {
        return X_NUCLEO_IHM16M1_UNSUPPORTED;
    }
    adc_config.Channel = adc_channel;
    adc_config.Rank = ADC_REGULAR_RANK_1;
    adc_config.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
    adc_config.SingleDiff = ADC_SINGLE_ENDED;
    adc_config.OffsetNumber = ADC_OFFSET_NONE;
    adc_config.Offset = 0U;
    if (HAL_ADC_ConfigChannel(&g_hadc1, &adc_config) != HAL_OK) {
        return X_NUCLEO_IHM16M1_COMMUNICATION_ERROR;
    }
    if ((HAL_ADC_Start(&g_hadc1) != HAL_OK) ||
        (HAL_ADC_PollForConversion(&g_hadc1, 2U) != HAL_OK)) {
        return X_NUCLEO_IHM16M1_TIMEOUT;
    }
    *raw = HAL_ADC_GetValue(&g_hadc1);
    (void)HAL_ADC_Stop(&g_hadc1);
    return X_NUCLEO_IHM16M1_OK;
}

void ihm16m1_platform_delay(void *ctx, uint32_t milliseconds)
{
    (void)ctx;
    HAL_Delay(milliseconds);
}
