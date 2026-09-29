/*
 * @file app.h
 * @brief Application entry contract called after STM32 board initialization.
 */
#ifndef APP_H
#define APP_H

/**
 * @brief Initializes reset state and performs model/runtime bring-up inference.
 *
 * @pre STM32 system clock, FPU, USART2, and basic stdio console initialization
 * have completed successfully.
 * @post Test state is reset; the embedded model/runtime is initialized, exercised
 * once for bring-up, and deinitialized. A final FAIL remains until the five-vector
 * harness in the next category is implemented.
 */
void app_start(void);

#endif /* APP_H */
