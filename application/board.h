/**
  ******************************************************************************
  * @file    board.h
  * @brief   NUCLEO-C5A3ZG board definitions for the PWM lab firmware.
  *
  *          Single source of truth for the pin map. Nothing in application/ or
  *          drivers/ should hard-code a port or pin number; add it here and
  *          refer to the BOARD_* name instead.
  ******************************************************************************
  */
#ifndef BOARD_H
#define BOARD_H

#include "mx_hal_def.h"
#include "mx_led.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Board identity
 * ---------------------------------------------------------------------- */
#define BOARD_NAME            "NUCLEO-C5A3ZG"
#define BOARD_MCU             "STM32C5A3ZGT6"

/** Core / timer clock after mx_rcc_init(): PSI 48 MHz ref -> 144 MHz SYSCLK,
  * AHB and all APB prescalers = 1, so TIM1 is clocked at 144 MHz. */
#define BOARD_SYSCLK_HZ       144000000UL
#define BOARD_TIMCLK_HZ       144000000UL

/* -------------------------------------------------------------------------
 * Arduino UNO connector mapping (MB1988 / CN7..CN10)
 *
 * Only the pins this firmware actually owns are listed. The board connector
 * table shows D7 as a plain GPIO, but the pin behind it is PA8, which has
 * TIM1_CH1 on AF1 - that is how the PWM output is driven.
 * ---------------------------------------------------------------------- */

/** Arduino D7 -> CN9 pin 8 -> PA8 -> TIM1_CH1 (AF1). 3.3 V logic output. */
#define BOARD_ARD_D7_PORT     HAL_GPIOA
#define BOARD_ARD_D7_PIN      PA8_PIN
#define BOARD_ARD_D7_AF       HAL_GPIO_AF_1
#define BOARD_ARD_D7_TIM      HAL_TIM1
#define BOARD_ARD_D7_CHANNEL  HAL_TIM_CHANNEL_1
#define BOARD_ARD_D7_CMP_UNIT HAL_TIM_OC_COMPARE_UNIT_1
#define BOARD_ARD_D7_LABEL    "D7 (CN9-8 / PA8 / TIM1_CH1)"

/* -------------------------------------------------------------------------
 * On-board peripherals
 * ---------------------------------------------------------------------- */

/** Green user LED LD1 on PA5. Driven through the Cube LED part driver, so use
  * led_on() / led_off() / led_toggle() with BOARD_STATUS_LED. */
#define BOARD_STATUS_LED      MX_STATUS_LED

/** ST-LINK Virtual COM Port: USART2 on PA2 (TX) / PA3 (RX), 115200 8N1. */
#define BOARD_CONSOLE_BAUD    115200U

/* -------------------------------------------------------------------------
 * Placement attributes tied to the custom linker script
 *
 * These only work because user_modifiable/Device/STM32C5A3ZGT6/
 * stm32c5a3xg_flash.ld defines the matching output sections. If that script
 * is ever replaced with the stock DFP template, every use of these macros
 * becomes a link error - which is the intended failure mode.
 * ---------------------------------------------------------------------- */

/** Place a function in SRAM (.ramfunc, copied from flash by the startup
  * copy table). Use for code that must not fetch from flash. */
#define BOARD_RAMFUNC         __attribute__((section(".ramfunc"), noinline))

/** Place a variable in the retained SRAM page (.noinit). Never zeroed by
  * startup, so the contents survive a warm reset. */
#define BOARD_NOINIT          __attribute__((section(".noinit")))

/** Place a constant in the reserved flash config page (.app_config @
  * 0x080FF800). Needs `used` so -ffunction-sections/--gc-sections keeps it. */
#define BOARD_APP_CONFIG      __attribute__((section(".app_config"), used))

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
