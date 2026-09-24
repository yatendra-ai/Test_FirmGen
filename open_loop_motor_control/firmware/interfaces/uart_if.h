#ifndef UART_IF_H
#define UART_IF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void uart_if_initialize(void);
bool uart_if_read_byte(uint8_t *byte);
void uart_if_write(const char *text, size_t length);

#endif
