#ifndef DIAGNOSTICS_UART_STM32_H
#define DIAGNOSTICS_UART_STM32_H

#include <stdbool.h>

bool diagnostics_uart_write_line(const char *line);

#endif /* DIAGNOSTICS_UART_STM32_H */
