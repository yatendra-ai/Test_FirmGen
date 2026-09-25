/**
  ******************************************************************************
  * @file    app_config.h
  * @brief   Board identity / calibration blob stored in the reserved flash page.
  *
  *          The blob lives at a fixed address (0x080FF800) in its own flash
  *          page so that factory tooling can rewrite it without touching the
  *          application image. See the .app_config section in
  *          user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld.
  ******************************************************************************
  */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 'C','F','G','1' - little endian. */
#define APP_CONFIG_MAGIC        0x31474643UL

/** Bumped whenever the layout below changes. Readers must reject newer. */
#define APP_CONFIG_VERSION      2U

/** Default PWM carrier frequency for Arduino D7, in hertz. */
#define APP_PWM_DEFAULT_CARRIER_HZ 1000U

/** Lowest carrier frequency accepted by the runtime override interface. */
#define APP_PWM_MIN_CARRIER_HZ 1U

/** Highest carrier frequency accepted by the runtime override interface. */
#define APP_PWM_MAX_CARRIER_HZ 1000000U

/**
  * @brief Layout of the reserved config page.
  *
  * Fields are only ever appended, never reordered or resized: boards already
  * in the field carry blobs written by older tooling.
  */
typedef struct
{
  uint32_t magic;             /*!< APP_CONFIG_MAGIC                          */
  uint16_t version;           /*!< APP_CONFIG_VERSION                        */
  uint16_t board_rev;         /*!< Hardware revision, BCD                    */
  char     serial[16];        /*!< NUL-terminated board serial               */

  uint32_t pwm_carrier_hz;    /*!< Carrier for the D7 PWM output             */
  uint32_t pwm_ramp_ms;       /*!< Duration of one 0->100% ramp leg          */
  uint32_t pwm_step_ms;       /*!< Interval between duty updates             */

  uint32_t reserved[5];       /*!< Zero-filled, available for new fields     */
} app_config_t;

/**
  * @brief  Returns the config blob from the reserved flash page.
  * @note   Always non-NULL; call app_config_is_valid() before trusting it.
  */
const app_config_t *app_config_get(void);

/**
  * @brief  True when the flash page holds a blob this firmware understands.
  */
bool app_config_is_valid(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
