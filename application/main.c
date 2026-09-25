#include "board.h"
#include "app_config.h"
#include "drv_pwm.h"
#include "drv_boot_record.h"
#include "ramp_control.h"
#include "mx_system.h"

static volatile drv_pwm_status_t s_pwm_status;

static void stop_on_error(uint32_t reason)
{
    uint32_t blink_count = 0U;

    (void)drv_pwm_set_duty_permille(0U);
    (void)drv_pwm_stop();
    drv_boot_record_status(reason);

    if (reason > 6U)
    {
        blink_count = 1U;
    }
    else
    {
        blink_count = reason;
    }

    for (;;)
    {
        uint32_t index = 0U;

        led_off(MX_STATUS_LED);
        HAL_Delay(500U);
        while (index < blink_count)
        {
            led_on(MX_STATUS_LED);
            HAL_Delay(150U);
            led_off(MX_STATUS_LED);
            HAL_Delay(350U);
            index++;
        }
        HAL_Delay(1000U);
    }
}

int main(void)
{
    if (mx_system_init() != SYSTEM_OK)
    {
        for (;;)
        {
            __WFI();
        }
    }
    drv_boot_record_init();
    if (!app_config_is_valid()) { stop_on_error(1U); }
    s_pwm_status = drv_pwm_init();
    if (s_pwm_status != DRV_PWM_OK) { stop_on_error(2U); }
    s_pwm_status = drv_pwm_set_carrier_hz(app_config_get()->pwm_carrier_hz);
    if (s_pwm_status != DRV_PWM_OK)
    {
        s_pwm_status = drv_pwm_set_carrier_hz(APP_PWM_DEFAULT_CARRIER_HZ);
    }
    if (s_pwm_status != DRV_PWM_OK)
    {
        stop_on_error(3U);
    }
    s_pwm_status = drv_pwm_set_duty_permille(0U);
    if (s_pwm_status != DRV_PWM_OK) { stop_on_error(4U); }
    s_pwm_status = drv_pwm_start();
    if (s_pwm_status != DRV_PWM_OK) { stop_on_error(5U); }
    ramp_control_init(HAL_GetTick());
    drv_boot_record_status(0U);
    led_on(MX_STATUS_LED);
    for (;;)
    {
        uint16_t duty;
        if (ramp_control_update(HAL_GetTick(), &duty))
        {
            s_pwm_status = drv_pwm_set_duty_permille(duty);
            if (s_pwm_status != DRV_PWM_OK) { stop_on_error(6U); }
        }
        else
        {
            __WFI();
        }
    }
}
