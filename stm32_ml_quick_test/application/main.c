/*
 * @file main.c
 * @brief STM32 reset entry that initializes the board before application code.
 *
 * The generated Cube system initializer owns HAL, clock, FPU, and USART2 setup.
 * This file performs only the startup handoff and keeps the application logic
 * in firmware/app/.
 */
#include "main.h"

#include "app.h"

/**
 * @brief Runs board initialization and enters the firmware application.
 *
 * @return This function does not return during normal operation.
 */
int main(void)
{
    if (mx_system_init() != SYSTEM_OK)
    {
        for (;;)
        {
        }
    }

    if (mx_basic_stdio_init() != SYSTEM_OK)
    {
        for (;;)
        {
        }
    }

    app_start();

    for (;;)
    {
    }
}
