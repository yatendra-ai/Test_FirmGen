#include "adc_platform_if.h"

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "mx_adc1.h"
#include "app_config.h"
#include "app_logger.h"
#include "diagnostics.h"

#define ADC_CONVERSION_TIMEOUT_MS 20U
#define ADC_REPORT_PERIOD_MS 5000U

static hal_adc_handle_t *g_adc_handle = NULL;
static TaskHandle_t g_adc_task_handle = NULL;

static bool adc_platform_read_injected(uint16_t *p_vref_raw, uint16_t *p_temperature_raw);
static void adc_platform_task(void *arg);

bool adc_platform_start(void)
{
    BaseType_t task_result;
    bool b_started = false;

    if (g_adc_handle == NULL)
    {
        g_adc_handle = mx_adc1_init();
        if (g_adc_handle == NULL)
        {
            APP_LOG_ERROR("ADC", "ADC1 initialization failed\n");
        }
        else if (HAL_ADC_Start(g_adc_handle) != HAL_OK)
        {
            APP_LOG_ERROR("ADC", "ADC activation failed\n");
        }
        else if (HAL_ADC_Calibrate(g_adc_handle) != HAL_OK)
        {
            APP_LOG_ERROR("ADC", "ADC calibration failed\n");
        }
        else
        {
            task_result = xTaskCreate(adc_platform_task, "adc_internal_task",
                                      (configSTACK_DEPTH_TYPE)(APP_ADC_TASK_STACK_BYTES / sizeof(StackType_t)),
                                      NULL, APP_ADC_TASK_PRIORITY, &g_adc_task_handle);
            if (task_result == pdPASS)
            {
                b_started = true;
                diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_ADC, "VREFINT and temperature sensor sampling active");
            }
            else
            {
                g_adc_task_handle = NULL;
                APP_LOG_ERROR("ADC", "ADC task creation failed\n");
            }
        }
    }
    else
    {
        b_started = true;
    }

    return b_started;
}

static bool adc_platform_read_injected(uint16_t *p_vref_raw, uint16_t *p_temperature_raw)
{
    bool b_success = false;

    if ((p_vref_raw != NULL) && (p_temperature_raw != NULL) && (g_adc_handle != NULL))
    {
        if (HAL_ADC_INJ_StartConv(g_adc_handle) == HAL_OK)
        {
            if (HAL_ADC_INJ_PollForConv(g_adc_handle, ADC_CONVERSION_TIMEOUT_MS) == HAL_OK)
            {
                *p_vref_raw = (uint16_t)HAL_ADC_INJ_ReadConversionDataRank(g_adc_handle, 1U);
                *p_temperature_raw = (uint16_t)HAL_ADC_INJ_ReadConversionDataRank(g_adc_handle, 2U);
                b_success = true;
            }
            else
            {
                APP_LOG_ERROR("ADC", "injected conversion polling timed out or failed\n");
            }
        }
        else
        {
            APP_LOG_ERROR("ADC", "injected conversion start failed\n");
        }
    }

    return b_success;
}

static void adc_platform_task(void *arg)
{
    uint16_t vref_raw = 0U;
    uint16_t temperature_raw = 0U;

    (void)arg;

    while (true)
    {
        if (adc_platform_read_injected(&vref_raw, &temperature_raw))
        {
            uint32_t vdda_mv = HAL_ADC_CALC_VREFANALOG_VOLTAGE(vref_raw, HAL_ADC_RESOLUTION_12_BIT);
            uint32_t temperature_vdda_mv = HAL_ADC_CALC_DATA_TO_VOLTAGE(vdda_mv, temperature_raw, HAL_ADC_RESOLUTION_12_BIT);
            int32_t temperature_c = HAL_ADC_CALC_TEMPERATURE(vdda_mv, temperature_raw, HAL_ADC_RESOLUTION_12_BIT);

            if ((temperature_c == (int32_t)HAL_ADC_TEMPERATURE_CALC_ERROR) || (temperature_raw == 0U))
            {
                APP_LOG_ERROR("ADC", "invalid internal sensor conversion raw VREFINT=%u raw TEMPSENSOR=%u vdda=%lu mV\n",
                              (unsigned int)vref_raw, (unsigned int)temperature_raw, (unsigned long)vdda_mv);
                diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_ADC, "temperature conversion invalid");
            }
            else
            {
                APP_LOG_INFO("ADC", "raw VREFINT=%u raw TEMPSENSOR=%u converted VDDA=%lu mV temp_input=%lu mV temperature=%ld C\n",
                             (unsigned int)vref_raw, (unsigned int)temperature_raw,
                             (unsigned long)vdda_mv, (unsigned long)temperature_vdda_mv, (long)temperature_c);
                diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_ADC, "internal readings sampled and converted");
            }
        }
        else
        {
            diagnostics_log_status(DIAGNOSTICS_SUBSYSTEM_ADC, "internal sample failed");
        }

        vTaskDelay(pdMS_TO_TICKS(ADC_REPORT_PERIOD_MS));
    }
}
