/**
  ******************************************************************************
  * @file    app_config.c
  * @brief   The compiled-in default config blob for the reserved flash page.
  *
  *          Engineering-only test identity, not data from a production board.
  *          This fixture does not implement flash programming or page erase.
  ******************************************************************************
  */

#include "app_config.h"
#include "board.h"

/* Placed at 0x080FF800 by the .app_config output section of the custom linker
 * script. `used` plus KEEP() in the script keep it out of --gc-sections. */
const app_config_t g_app_config BOARD_APP_CONFIG =
{
  .magic          = APP_CONFIG_MAGIC,
  .version        = APP_CONFIG_VERSION,
  .board_rev      = 0x0103U,           /* MB1988 rev C */
  .serial         = "TEST-0000000001",

  .pwm_carrier_hz = APP_PWM_DEFAULT_CARRIER_HZ, /* 1 kHz carrier on D7       */
  .pwm_ramp_ms    = 5000U,             /* 5 s per ramp leg               */
  .pwm_step_ms    = 10U,               /* fixed 10 ms update interval   */

  .reserved       = { 0U, 0U, 0U, 0U, 0U },
};

const app_config_t *app_config_get(void)
{
  return &g_app_config;
}

bool app_config_is_valid(void)
{
  return (g_app_config.magic == APP_CONFIG_MAGIC)
      && (g_app_config.version == APP_CONFIG_VERSION)
      && (g_app_config.pwm_carrier_hz >= APP_PWM_MIN_CARRIER_HZ)
      && (g_app_config.pwm_carrier_hz <= APP_PWM_MAX_CARRIER_HZ)
      && (g_app_config.pwm_ramp_ms > 0U)
      && (g_app_config.pwm_ramp_ms <= 60000U)
      && (g_app_config.pwm_step_ms > 0U)
      && (g_app_config.pwm_step_ms <= g_app_config.pwm_ramp_ms);
}
