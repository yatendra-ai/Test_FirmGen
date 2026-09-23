#ifndef UART_PLATFORM_H
#define UART_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

bool uart_platform_start(void);
bool uart_platform_read_line(char *p_line, uint32_t capacity);
bool uart_platform_write(const char *p_text, uint32_t length);
void uart_platform_irq_handler(void);

#endif /* UART_PLATFORM_H */
