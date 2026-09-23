#include "uart_platform.h"

#include <stddef.h>

#include "main.h"

#define UART_PLATFORM_LINE_CAPACITY 64U

static UART_HandleTypeDef g_huart;
static uint8_t g_rx_byte;
static char g_line[UART_PLATFORM_LINE_CAPACITY];
static volatile uint32_t g_line_length;
static volatile bool g_line_ready;
static bool g_started;

bool uart_platform_start(void)
{
    if (g_started) {
        return true;
    }

    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_USART2_CONFIG(RCC_USART2CLKSOURCE_PCLK1);
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio);

    g_huart.Instance = USART2;
    g_huart.Init.BaudRate = 115200U;
    g_huart.Init.WordLength = UART_WORDLENGTH_8B;
    g_huart.Init.StopBits = UART_STOPBITS_1;
    g_huart.Init.Parity = UART_PARITY_NONE;
    g_huart.Init.Mode = UART_MODE_TX_RX;
    g_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    g_huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&g_huart) != HAL_OK) {
        return false;
    }

    HAL_UART_Transmit(&g_huart, (uint8_t *)"BOOT\r\n", 6U, 100U);

    HAL_NVIC_SetPriority(USART2_IRQn, 5U, 0U);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    g_line_length = 0U;
    g_line_ready = false;
    g_started = HAL_UART_Receive_IT(&g_huart, &g_rx_byte, 1U) == HAL_OK;
    return g_started;
}

bool uart_platform_read_line(char *p_line, uint32_t capacity)
{
    uint32_t index;

    if ((p_line == NULL) || (capacity == 0U) || !g_line_ready) {
        return false;
    }

    __disable_irq();
    if (g_line_length >= capacity) {
        g_line_ready = false;
        g_line_length = 0U;
        __enable_irq();
        return false;
    }

    for (index = 0U; index < g_line_length; index++) {
        p_line[index] = g_line[index];
    }
    p_line[g_line_length] = '\0';
    g_line_ready = false;
    g_line_length = 0U;
    __enable_irq();
    return true;
}

bool uart_platform_write(const char *p_text, uint32_t length)
{
    if ((p_text == NULL) || (length == 0U)) {
        return false;
    }

    return HAL_UART_Transmit(&g_huart, (uint8_t *)(uintptr_t)p_text,
                             (uint16_t)length, 100U) == HAL_OK;
}

void uart_platform_irq_handler(void)
{
    HAL_UART_IRQHandler(&g_huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *p_huart)
{
    if (p_huart == &g_huart) {
        if (!g_line_ready) {
            if ((g_rx_byte == '\r') || (g_rx_byte == '\n')) {
                if (g_line_length > 0U) {
                    g_line_ready = true;
                }
            } else if (g_line_length < (UART_PLATFORM_LINE_CAPACITY - 1U)) {
                g_line[g_line_length] = (char)g_rx_byte;
                g_line_length++;
            } else {
                g_line_length = 0U;
            }
        }
        (void)HAL_UART_Receive_IT(&g_huart, &g_rx_byte, 1U);
    }
}
