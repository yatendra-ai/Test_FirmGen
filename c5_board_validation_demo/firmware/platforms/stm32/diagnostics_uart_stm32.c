#include "diagnostics_uart_stm32.h"

#include <stddef.h>

#include "app_logger.h"

bool diagnostics_uart_write_line(const char *line)
{
    bool b_written = false;

    if (line != NULL)
    {
        /* app_logger_init() creates the mutex before the scheduler starts. */
        APP_LOG_INFO("board-health", "%s\n", line);
        b_written = true;
    }
    else
    {
        /* No line was supplied. */
    }

    return b_written;
}
