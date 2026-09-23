#ifndef BOARD_H
#define BOARD_H

/* Declarative board facts for the NUCLEO-G431RB. */
#include <stdbool.h>
#include <stdint.h>

#include "app_config.h"

#define BOARD_NAME "NUCLEO-G431RB"
#define BOARD_LED_ACTIVE_LEVEL 1
#define BOARD_MOTOR_DRIVER "X-NUCLEO-IHM16M1 / STSPIN830"
#define BOARD_MOTOR_MODEL "GBM2804H-100T"
#define BOARD_MOTOR_POLE_PAIRS APP_MOTOR_POLE_PAIRS
#define BOARD_SUPPLY_VOLTAGE_MV APP_SUPPLY_VOLTAGE_MV

const char *board_name(void);

static inline uint32_t board_led_level(bool on)
{
    return on ? (uint32_t)(BOARD_LED_ACTIVE_LEVEL)
              : (uint32_t)(!(BOARD_LED_ACTIVE_LEVEL));
}

#endif /* BOARD_H */
