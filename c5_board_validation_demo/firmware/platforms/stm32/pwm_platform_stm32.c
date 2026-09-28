#include "pwm_platform_if.h"

#include <stdbool.h>
#include <stdint.h>

#include "mx_tim2.h"
#include "app_logger.h"
#include "app_config.h"

/* TIM2 source clock and divisor follow the NUCLEO-C5A3ZG generated timer setup. */
#define PWM_PRESCALER_DIVISOR 144U
#define PWM_TIMER_INPUT_HZ 144000000U

static bool g_pwm_started = false;

bool pwm_platform_start(void)
{
    hal_tim_handle_t *p_timer;
    hal_tim_config_t timer_config;
    hal_tim_oc_channel_config_t channel_config;
    hal_tim_oc_compare_unit_config_t compare_config;
    bool b_started = false;

    if (!g_pwm_started)
    {
        p_timer = mx_tim2_init();
        if (p_timer == NULL)
        {
            APP_LOG_ERROR("PWM", "TIM2 initialization failed\n");
        }
        else
        {
            timer_config.prescaler = PWM_PRESCALER_DIVISOR - 1U;
            timer_config.counter_mode = HAL_TIM_COUNTER_UP;
            timer_config.period = (PWM_TIMER_INPUT_HZ / (PWM_PRESCALER_DIVISOR * APP_PWM_FREQUENCY_HZ)) - 1U;
            timer_config.repetition_counter = 0U;
            timer_config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;

            channel_config.polarity = HAL_TIM_OC_HIGH;
            channel_config.idle_state = HAL_TIM_OC_IDLE_STATE_RESET;
            channel_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
            channel_config.break_mode = HAL_TIM_OC_BREAKMODE_IMMEDIATE;

            compare_config.mode = HAL_TIM_OC_PWM1;
            compare_config.pulse = (((timer_config.period + 1U) * APP_PWM_DUTY_PERCENT) / 100U);

            if ((HAL_TIM_SetConfig(p_timer, &timer_config) == HAL_OK) &&
                (HAL_TIM_OC_SetConfigChannel(p_timer, HAL_TIM_CHANNEL_2, &channel_config) == HAL_OK) &&
                (HAL_TIM_OC_SetConfigCompareUnit(p_timer, HAL_TIM_OC_COMPARE_UNIT_2, &compare_config) == HAL_OK) &&
                (HAL_TIM_OC_StartChannel(p_timer, HAL_TIM_CHANNEL_2) == HAL_OK) &&
                (HAL_TIM_Start(p_timer) == HAL_OK))
            {
                g_pwm_started = true;
                b_started = true;
                APP_LOG_INFO("PWM", "TIM2_CH2 PWM started on PB3: target 1 kHz, 50 percent duty\n");
            }
            else
            {
                APP_LOG_ERROR("PWM", "TIM2_CH2 PWM configuration or start failed\n");
            }
        }
    }
    else
    {
        b_started = true;
    }

    return b_started;
}
