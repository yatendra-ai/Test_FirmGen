#ifndef APP_CONFIG_H
#define APP_CONFIG_H
// Board: NUCLEO-C5A3ZG — LED macros are new-gen HAL tokens grounded from the .ioc2.
#define APP_LED_GPIO_PORT   HAL_GPIOA
#define APP_LED_GPIO_PIN    HAL_GPIO_PIN_5
#define APP_LD1_GPIO_PORT  HAL_GPIOA
#define APP_LD1_GPIO_PIN   HAL_GPIO_PIN_5
#define APP_LD2_GPIO_PORT  HAL_GPIOG
#define APP_LD2_GPIO_PIN   HAL_GPIO_PIN_1
#define APP_LD3_GPIO_PORT  HAL_GPIOG
#define APP_LD3_GPIO_PIN   HAL_GPIO_PIN_2

/* Fixed number of reference vectors whose result storage is reset at startup. */
#define APP_TEST_VECTOR_COUNT 5U

/* Blocking USART console transmission timeout in milliseconds. */
#define APP_CONSOLE_TX_TIMEOUT_MS 100U
#endif
