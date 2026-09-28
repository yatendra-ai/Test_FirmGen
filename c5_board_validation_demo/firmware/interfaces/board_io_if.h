#ifndef BOARD_IO_IF_H
#define BOARD_IO_IF_H

#include <stdbool.h>
#include <stdint.h>

typedef enum board_io_led_t
{
    BOARD_IO_LED_STATUS = 0,
    BOARD_IO_LED_NETWORK,
    BOARD_IO_LED_MQTT
} board_io_led_t;

bool board_io_init(void);
void board_io_set_led(board_io_led_t led, bool on);
bool board_io_button_is_pressed(void);

#endif /* BOARD_IO_IF_H */
