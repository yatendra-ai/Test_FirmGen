#include "dht22_port.h"

#include <stdbool.h>
#include <stdint.h>

#include "app_config.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DHT22_TIMEOUT_US 120U

static bool dht22_wait_level(int level)
{
    uint32_t elapsed_us;

    elapsed_us = 0U;
    while (gpio_get_level((gpio_num_t)APP_DHT22_DATA_GPIO) != level) {
        esp_rom_delay_us(1U);
        elapsed_us++;
        if (elapsed_us >= DHT22_TIMEOUT_US) {
            return false;
        }
    }
    return true;
}

static bool dht22_read_frame(uint8_t frame[5])
{
    uint8_t byte_index;
    uint8_t bit_index;
    uint32_t high_time_us;

    gpio_set_direction((gpio_num_t)APP_DHT22_DATA_GPIO, GPIO_MODE_OUTPUT_OD);
    gpio_set_level((gpio_num_t)APP_DHT22_DATA_GPIO, 0);
    esp_rom_delay_us(1200U);
    gpio_set_level((gpio_num_t)APP_DHT22_DATA_GPIO, 1);
    esp_rom_delay_us(30U);
    gpio_set_direction((gpio_num_t)APP_DHT22_DATA_GPIO, GPIO_MODE_INPUT);

    if (!dht22_wait_level(0) || !dht22_wait_level(1) || !dht22_wait_level(0)) {
        return false;
    }

    for (byte_index = 0U; byte_index < 5U; byte_index++) {
        frame[byte_index] = 0U;
        for (bit_index = 0U; bit_index < 8U; bit_index++) {
            if (!dht22_wait_level(1)) {
                return false;
            }
            esp_rom_delay_us(40U);
            high_time_us = 0U;
            while (gpio_get_level((gpio_num_t)APP_DHT22_DATA_GPIO) != 0) {
                esp_rom_delay_us(1U);
                high_time_us++;
                if (high_time_us >= DHT22_TIMEOUT_US) {
                    return false;
                }
            }
            frame[byte_index] = (uint8_t)(frame[byte_index] << 1U);
            if (high_time_us > 30U) {
                frame[byte_index] = (uint8_t)(frame[byte_index] | 1U);
            }
        }
    }
    return true;
}

static void dht22_delay_ms(uint32_t milliseconds)
{
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

bool dht22_port_open(dht22_port_t *port)
{
    gpio_config_t config;

    if (port == NULL) {
        return false;
    }
    config.pin_bit_mask = (1ULL << APP_DHT22_DATA_GPIO);
    config.mode = GPIO_MODE_INPUT_OUTPUT_OD;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    if (gpio_config(&config) != ESP_OK) {
        return false;
    }
    port->read_frame = dht22_read_frame;
    port->delay_ms = dht22_delay_ms;
    return true;
}
