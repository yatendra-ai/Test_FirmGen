#ifndef UART_COMMAND_H
#define UART_COMMAND_H

#include <stdbool.h>
#include <stdint.h>

bool uart_command_init(void);
void uart_command_process(void);

#endif /* UART_COMMAND_H */
