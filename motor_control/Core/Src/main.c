#include "main.h"
#include "app.h"

static void system_clock_config(void);

int main(void)
{
    HAL_Init();
    system_clock_config();
    app_start();

    while (1) {
    }
}

static void system_clock_config(void)
{
    RCC_ClkInitTypeDef clock_config = {0};
    RCC_OscInitTypeDef oscillator_config = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
    __HAL_RCC_PWR_CLK_DISABLE();

    oscillator_config.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    oscillator_config.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    oscillator_config.HSIState = RCC_HSI_ON;
    oscillator_config.PLL.PLLState = RCC_PLL_ON;
    oscillator_config.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    oscillator_config.PLL.PLLM = RCC_PLLM_DIV4;
    oscillator_config.PLL.PLLN = 85U;
    oscillator_config.PLL.PLLP = RCC_PLLP_DIV2;
    oscillator_config.PLL.PLLQ = RCC_PLLQ_DIV2;
    oscillator_config.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&oscillator_config) != HAL_OK) {
        while (1) {
        }
    }

    clock_config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                             RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clock_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clock_config.AHBCLKDivider = RCC_HCLK_DIV1;
    clock_config.APB1CLKDivider = RCC_HCLK_DIV1;
    clock_config.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clock_config, FLASH_LATENCY_8) != HAL_OK) {
        while (1) {
        }
    }
}
