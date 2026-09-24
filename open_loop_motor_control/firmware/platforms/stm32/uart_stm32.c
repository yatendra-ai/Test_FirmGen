#include "uart_if.h"
#include "app_config.h"
#include "stm32g4xx_hal.h"
#include <stddef.h>
#include <string.h>

extern void HAL_UART_MspInit(UART_HandleTypeDef *huart);

static UART_HandleTypeDef huart2;
static volatile uint8_t receive_byte;
static volatile bool receive_ready;
static volatile uint8_t receive_queue[32U];
static volatile uint8_t receive_write_index;
static volatile uint8_t receive_read_index;
static volatile uint32_t uart_init_status;

void uart_if_initialize(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_USART2_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = APP_UART_BAUD_RATE;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    HAL_StatusTypeDef init_status = HAL_UART_Init(&huart2);
    uart_init_status = (uint32_t)init_status;
    if (init_status != HAL_OK) {
        HAL_GPIO_WritePin(APP_LED_GPIO_PORT, APP_LED_GPIO_PIN, GPIO_PIN_RESET);
    } else {
        const uint8_t startup_text[] = "uart init ok\r\n";
        (void)HAL_UART_Transmit(&huart2, (uint8_t *)(void *)startup_text, (uint16_t)(sizeof(startup_text) - 1U), 100U);
    }
    HAL_NVIC_SetPriority(USART2_IRQn, 5U, 0U);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    receive_ready = false;
    receive_write_index = 0U;
    receive_read_index = 0U;
    (void)HAL_UART_Receive_IT(&huart2, (uint8_t *)(void *)&receive_byte, 1U);
}

bool uart_if_read_byte(uint8_t *byte)
{
    bool ready = false;
    uint8_t next_read_index;

    if (byte != NULL) {
        __disable_irq();
        next_read_index = (uint8_t)((receive_read_index + 1U) % 32U);
        if (receive_read_index != receive_write_index) {
            *byte = receive_queue[receive_read_index];
            receive_read_index = next_read_index;
            ready = true;
        }
        __enable_irq();
    }
    return ready;
}

void uart_if_write(const char *text, size_t length)
{
    if ((text != NULL) && (length > 0U)) {
        (void)HAL_UART_Transmit(&huart2, (uint8_t *)(void *)text, (uint16_t)length, 100U);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle == &huart2) {
        uint8_t next_write_index = (uint8_t)((receive_write_index + 1U) % 32U);
        if (next_write_index != receive_read_index) {
            receive_queue[receive_write_index] = receive_byte;
            receive_write_index = next_write_index;
        }
        receive_ready = true;
        (void)HAL_UART_Receive_IT(&huart2, (uint8_t *)(void *)&receive_byte, 1U);
    }
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}
