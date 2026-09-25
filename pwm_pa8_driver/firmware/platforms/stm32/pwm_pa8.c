#include "pwm_pa8.h"

#include <stddef.h>

#include "stm32_ll.h"
#include "mx_tim1.h"

static bool b_pwm_pa8_configure_gpio(void)
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinSpeed(PA8_PORT, PA8_PIN, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinOutputType(PA8_PORT, PA8_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(PA8_PORT, PA8_PIN, LL_GPIO_PULL_NO);
    LL_GPIO_SetAFPin_8_15(PA8_PORT, PA8_PIN, LL_GPIO_AF_1);
    LL_GPIO_SetPinMode(PA8_PORT, PA8_PIN, LL_GPIO_MODE_ALTERNATE);

    return true;
}

static TIM_TypeDef *g_pwm_timer;
static uint32_t g_pwm_period_ticks;

bool pwm_pa8_start(uint32_t frequency_hz)
{
    TIM_TypeDef *p_timer;
    uint32_t period_ticks;

    if (frequency_hz == 0U)
    {
        return false;
    }

    period_ticks = 144000000U / (144U * frequency_hz);
    if (period_ticks == 0U)
    {
        return false;
    }

    p_timer = mx_tim1_init();
    if (p_timer == NULL)
    {
        return false;
    }

    if (!b_pwm_pa8_configure_gpio())
    {
        mx_tim1_deinit();
        return false;
    }

    LL_TIM_SetPrescaler(p_timer, 143U);
    LL_TIM_SetAutoReload(p_timer, period_ticks - 1U);
    LL_TIM_OC_SetMode(p_timer, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_SetCompareValue(p_timer, LL_TIM_OC_COMPARE_UNIT_1, 0U);
    LL_TIM_OC_EnablePreload(p_timer, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(p_timer, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableAllOutputs(p_timer);
    LL_TIM_EnableCounter(p_timer);
    g_pwm_timer = p_timer;
    g_pwm_period_ticks = period_ticks;

    return true;
}

void pwm_pa8_set_duty_percent(uint32_t duty_percent)
{
    uint32_t compare_ticks;

    if (g_pwm_timer == NULL)
    {
        return;
    }

    if (duty_percent > 100U)
    {
        duty_percent = 100U;
    }

    compare_ticks = (g_pwm_period_ticks * duty_percent) / 100U;
    LL_TIM_OC_SetCompareValue(g_pwm_timer, LL_TIM_OC_COMPARE_UNIT_1, compare_ticks);
}
