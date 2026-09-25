#include "app.h"

#include <stdbool.h>

#include "app_config.h"
#include "osal.h"
#include "logger.h"
#include "pwm_pa8.h"
#include "pwm_ramp.h"
#include "firmgen_telemetry.h"
#include "mx_usart2.h"   /* firmgen_telemetry */

static const char *g_app_tag = "app";
static pwm_ramp_state_t g_pwm_ramp_state;

void app_start(void)
{
    firmgen_telemetry_start(mx_usart2_uart_gethandle());
    bool b_pwm_started;

    b_pwm_started = pwm_pa8_start(APP_PWM_FREQUENCY_HZ);
    if (b_pwm_started)
    {
        LOGI(g_app_tag, "TIM1 CH1 PWM started on PA8 at %u Hz", APP_PWM_FREQUENCY_HZ);
    }
    else
    {
        LOGE(g_app_tag, "PWM initialization failed; PA8 remains inactive");
    }

    pwm_ramp_reset(&g_pwm_ramp_state, osal_uptime_ms());

    {
        uint32_t last_update_ms = osal_uptime_ms();

        while (true)
        {
            firmgen_telemetry_poll();
            uint32_t now_ms = osal_uptime_ms();

            if (now_ms != last_update_ms)
            {
                last_update_ms = now_ms;
                pwm_pa8_set_duty_percent(pwm_ramp_duty_percent(&g_pwm_ramp_state, now_ms));
            }
        }
    }
}
