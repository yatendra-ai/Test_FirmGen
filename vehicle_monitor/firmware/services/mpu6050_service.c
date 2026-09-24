#include "mpu6050_service.h"

#include <stddef.h>

#include "app_config.h"
#include "logger.h"
#include "mpu6050.h"
#include "mpu6050_port.h"

static const char *TAG = "mpu6050";
static mpu6050_state_t g_mpu6050_state;
static uint8_t g_mpu6050_storage[128U];

static mpu6050_status_t mpu6050_read(void *context, uint8_t reg, uint8_t *data, size_t length)
{
    (void)context;
    return mpu6050_port_read(reg, data, (uint32_t)length) ? MPU6050_OK : MPU6050_E_IO;
}

static mpu6050_status_t mpu6050_write(void *context, uint8_t reg, const uint8_t *data, size_t length)
{
    (void)context;
    return mpu6050_port_write(reg, data, (uint32_t)length) ? MPU6050_OK : MPU6050_E_IO;
}

static void mpu6050_delay(void *context, uint32_t milliseconds)
{
    (void)context;
    mpu6050_port_delay_ms(milliseconds);
}

static bool mpu6050_validate_configuration(mpu6050_handle_t *handle)
{
    uint8_t value;
    bool valid;

    valid = mpu6050_read_register(handle, MPU6050_REG_ACCEL_CONFIG, &value, 1U) == MPU6050_OK;
    valid = valid && (value == 0U);
    valid = valid && (mpu6050_read_register(handle, MPU6050_REG_CONFIG, &value, 1U) == MPU6050_OK);
    valid = valid && (value == (uint8_t)MPU6050_DLPF_44_42);
    return valid;
}

static bool mpu6050_initialize(void)
{
    mpu6050_io_t io = {
        .ctx = NULL,
        .read = mpu6050_read,
        .write = mpu6050_write,
        .delay_ms = mpu6050_delay,
    };
    mpu6050_handle_t *handle;
    uint8_t identity;

    handle = (mpu6050_handle_t *)g_mpu6050_storage;
    if (!mpu6050_port_start()) {
        return false;
    }
    identity = 0U;
    if ((!mpu6050_port_probe(APP_MPU6050_I2C_ADDRESS)) ||
        (!mpu6050_port_read(MPU6050_REG_WHO_AM_I, &identity, 1U)) ||
        ((identity & MPU6050_WHO_AM_I_MASK) != MPU6050_WHO_AM_I_VALUE)) {
        ESP_LOGE(TAG, "IMU initialization error: WHO_AM_I=0x%02x", identity);
        return false;
    }
    if (mpu6050_init(handle, sizeof(g_mpu6050_storage), &io, false) != MPU6050_OK) {
        ESP_LOGE(TAG, "IMU initialization error: driver init failed");
        return false;
    }
    if ((mpu6050_set_accel_range(handle, MPU6050_ACCEL_2G) != MPU6050_OK) ||
        (mpu6050_set_dlpf(handle, MPU6050_DLPF_44_42) != MPU6050_OK) ||
        (!mpu6050_validate_configuration(handle))) {
        ESP_LOGE(TAG, "IMU initialization error: configuration readback failed");
        return false;
    }
    g_mpu6050_state.initialized = true;
    ESP_LOGI(TAG, "IMU ready: WHO_AM_I=0x%02x accel_range=2G dlpf=44Hz", identity);
    return true;
}

void mpu6050_service_start(void)
{
    mpu6050_handle_t *handle;
    mpu6050_sample_t sample;

    g_mpu6050_state.lateral_g = 0.0F;
    g_mpu6050_state.longitudinal_g = 0.0F;
    g_mpu6050_state.vertical_g = 0.0F;
    g_mpu6050_state.failure_count = 0U;
    g_mpu6050_state.initialized = false;
    g_mpu6050_state.valid = false;
    handle = (mpu6050_handle_t *)g_mpu6050_storage;

    for (;;) {
        if ((!g_mpu6050_state.initialized) && (!mpu6050_initialize())) {
            g_mpu6050_state.failure_count++;
            mpu6050_port_delay_ms(APP_MPU6050_RETRY_PERIOD_MS);
        } else if (g_mpu6050_state.initialized) {
            if (mpu6050_read_sample(handle, &sample) == MPU6050_OK) {
                g_mpu6050_state.lateral_g = sample.accel_x_g;
                g_mpu6050_state.longitudinal_g = sample.accel_y_g;
                g_mpu6050_state.vertical_g = sample.accel_z_g;
                g_mpu6050_state.valid = true;
                ESP_LOGI(TAG, "accel_g lateral=%.3f longitudinal=%.3f vertical=%.3f", sample.accel_x_g, sample.accel_y_g, sample.accel_z_g);
            } else {
                g_mpu6050_state.failure_count++;
            }
            mpu6050_port_delay_ms(APP_MPU6050_SAMPLE_PERIOD_MS);
        } else {
            g_mpu6050_state.failure_count++;
        }
    }
}

const mpu6050_state_t *mpu6050_service_state(void)
{
    return &g_mpu6050_state;
}
