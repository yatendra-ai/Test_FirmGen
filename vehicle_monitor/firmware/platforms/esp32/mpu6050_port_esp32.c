#include "mpu6050_port.h"

#include <stddef.h>

#include "app_config.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static i2c_master_bus_handle_t g_i2c_bus;
static i2c_master_dev_handle_t g_mpu6050_device;
static bool g_i2c_started;

bool mpu6050_port_start(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = APP_MPU6050_SDA_GPIO,
        .scl_io_num = APP_MPU6050_SCL_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7U,
        .flags.enable_internal_pullup = true,
    };
    i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = APP_MPU6050_I2C_ADDRESS,
        .scl_speed_hz = APP_MPU6050_I2C_FREQUENCY_HZ,
    };

    if (g_i2c_started) {
        return true;
    }
    if (i2c_new_master_bus(&bus_config, &g_i2c_bus) != ESP_OK) {
        return false;
    }
    if (i2c_master_bus_add_device(g_i2c_bus, &device_config, &g_mpu6050_device) != ESP_OK) {
        (void)i2c_del_master_bus(g_i2c_bus);
        return false;
    }
    g_i2c_started = true;
    return true;
}

bool mpu6050_port_read(uint8_t reg, uint8_t *data, uint32_t length)
{
    return (g_i2c_started && (i2c_master_transmit_receive(g_mpu6050_device, &reg, 1U,
        data, length, APP_MPU6050_I2C_TIMEOUT_MS) == ESP_OK));
}

bool mpu6050_port_write(uint8_t reg, const uint8_t *data, uint32_t length)
{
    uint8_t buffer[16];
    uint32_t index;

    if ((!g_i2c_started) || (length > (sizeof(buffer) - 1U))) {
        return false;
    }
    buffer[0] = reg;
    for (index = 0U; index < length; index++) {
        buffer[index + 1U] = data[index];
    }
    return (i2c_master_transmit(g_mpu6050_device, buffer, length + 1U,
        APP_MPU6050_I2C_TIMEOUT_MS) == ESP_OK);
}

void mpu6050_port_delay_ms(uint32_t milliseconds)
{
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

bool mpu6050_port_probe(uint8_t address)
{
    return (g_i2c_started &&
        (i2c_master_probe(g_i2c_bus, address, APP_MPU6050_I2C_TIMEOUT_MS) == ESP_OK));
}
