/*
 * @file console_stm32.c
 * @brief Sends application console bytes through the board's USART2 VCP.
 *
 * This is the only firmware layer that uses the STM32 UART HAL. The configured
 * Cube USART2 instance routes TX to ST-LINK VCP PA2 at 115200 baud, 8-N-1.
 */
#include "console_if.h"

#include "app_config.h"
#include "mx_usart2.h"

bool console_if_write(const uint8_t *data, size_t length)
{
    hal_uart_handle_t *uart_handle;

    if ((data == NULL) || (length == 0U))
    {
        return false;
    }

    uart_handle = mx_usart2_uart_gethandle();
    if (uart_handle == NULL)
    {
        return false;
    }

    return HAL_UART_Transmit(uart_handle,
                             data,
                             (uint32_t)length,
                             APP_CONSOLE_TX_TIMEOUT_MS) == HAL_OK;
}
