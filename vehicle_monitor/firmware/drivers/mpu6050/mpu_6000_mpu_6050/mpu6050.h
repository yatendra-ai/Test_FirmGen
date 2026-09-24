#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Portable I/O contract
 * ---------------------
 * The integrator supplies register-oriented callbacks for the selected
 * primary bus. For I2C, reg is the 8-bit register address and the callback
 * should address the MPU-6050 at the configured 7-bit address. For SPI, the
 * callback should assert chip select, transmit reg | 0x80 for reads (the
 * device read mask is 0x80), and use MSB-first transfers. The driver core
 * does not know or depend on the platform, bus controller, RTOS, or SDK.
 *
 * Every callback receives ctx. delay_ms is required because reset timing is
 * specified by the component specification. Callback failures must be
 * returned as an mpu6050_status_t value; the driver propagates them.
 */
typedef enum {
    MPU6050_OK = 0,
    MPU6050_E_INVALID_ARG,
    MPU6050_E_NOT_INITIALIZED,
    MPU6050_E_IO,
    MPU6050_E_TIMEOUT,
    MPU6050_E_UNSUPPORTED,
    MPU6050_E_ID_MISMATCH,
    MPU6050_E_VERIFY
} mpu6050_status_t;

typedef mpu6050_status_t (*mpu6050_reg_read_fn)(void *ctx, uint8_t reg,
                                                 uint8_t *data, size_t len);
typedef mpu6050_status_t (*mpu6050_reg_write_fn)(void *ctx, uint8_t reg,
                                                  const uint8_t *data, size_t len);
typedef void (*mpu6050_delay_ms_fn)(void *ctx, uint32_t milliseconds);

typedef struct {
    void *ctx;
    mpu6050_reg_read_fn read;
    mpu6050_reg_write_fn write;
    mpu6050_delay_ms_fn delay_ms;
} mpu6050_io_t;

typedef struct mpu6050_handle mpu6050_handle_t;

/* Register addresses defined by the supplied register map. */
#define MPU6050_REG_SELF_TEST_X       0x0Du
#define MPU6050_REG_SELF_TEST_Y       0x0Eu
#define MPU6050_REG_SELF_TEST_Z       0x0Fu
#define MPU6050_REG_SELF_TEST_A       0x10u
#define MPU6050_REG_SMPLRT_DIV        0x19u
#define MPU6050_REG_CONFIG            0x1Au
#define MPU6050_REG_GYRO_CONFIG       0x1Bu
#define MPU6050_REG_ACCEL_CONFIG      0x1Cu
#define MPU6050_REG_FIFO_EN           0x23u
#define MPU6050_REG_I2C_MST_CTRL      0x24u
#define MPU6050_REG_INT_PIN_CFG       0x37u
#define MPU6050_REG_INT_ENABLE        0x38u
#define MPU6050_REG_INT_STATUS        0x3Au
#define MPU6050_REG_ACCEL_XOUT_H      0x3Bu
#define MPU6050_REG_TEMP_OUT_H        0x41u
#define MPU6050_REG_GYRO_XOUT_H       0x43u
#define MPU6050_REG_SIGNAL_PATH_RESET 0x68u
#define MPU6050_REG_USER_CTRL         0x6Au
#define MPU6050_REG_PWR_MGMT_1        0x6Bu
#define MPU6050_REG_PWR_MGMT_2        0x6Cu
#define MPU6050_REG_FIFO_COUNTH       0x72u
#define MPU6050_REG_FIFO_COUNTL       0x73u
#define MPU6050_REG_FIFO_R_W          0x74u
#define MPU6050_REG_WHO_AM_I          0x75u

#define MPU6050_WHO_AM_I_VALUE        0x68u
#define MPU6050_SPI_READ_MASK         0x80u
#define MPU6050_I2C_ADDRESS_AD0_LOW   0x68u
#define MPU6050_I2C_ADDRESS_AD0_HIGH  0x69u

/* Field masks and shifts. */
#define MPU6050_CONFIG_EXT_SYNC_MASK  0x38u
#define MPU6050_CONFIG_EXT_SYNC_SHIFT 3u
#define MPU6050_CONFIG_DLPF_MASK      0x07u
#define MPU6050_GYRO_ST_MASK          0xE0u
#define MPU6050_GYRO_FS_MASK          0x18u
#define MPU6050_GYRO_FS_SHIFT         3u
#define MPU6050_ACCEL_ST_MASK         0xE0u
#define MPU6050_ACCEL_FS_MASK         0x18u
#define MPU6050_ACCEL_FS_SHIFT        3u
#define MPU6050_PWR_DEVICE_RESET      0x80u
#define MPU6050_PWR_SLEEP             0x40u
#define MPU6050_PWR_CYCLE             0x20u
#define MPU6050_PWR_TEMP_DIS          0x08u
#define MPU6050_PWR_CLKSEL_MASK       0x07u
#define MPU6050_SIGNAL_RESET_ALL      0x07u
#define MPU6050_USER_FIFO_EN         0x40u
#define MPU6050_USER_I2C_MST_EN      0x20u
#define MPU6050_USER_I2C_IF_DIS      0x10u
#define MPU6050_USER_FIFO_RESET      0x04u
#define MPU6050_USER_I2C_MST_RESET   0x02u
#define MPU6050_USER_SIG_COND_RESET  0x01u
#define MPU6050_INT_DATA_RDY_EN      0x01u
#define MPU6050_INT_FIFO_OFLOW_EN    0x10u
#define MPU6050_INT_I2C_MST_EN       0x08u
#define MPU6050_INT_DATA_RDY         0x01u
#define MPU6050_INT_FIFO_OFLOW       0x10u
#define MPU6050_INT_I2C_MST          0x08u
#define MPU6050_WHO_AM_I_MASK        0x7Eu

/* Strongly typed configuration values. */
typedef enum {
    MPU6050_CLOCK_INTERNAL_8MHZ = 0,
    MPU6050_CLOCK_PLL_X_GYRO = 1,
    MPU6050_CLOCK_PLL_Y_GYRO = 2,
    MPU6050_CLOCK_PLL_Z_GYRO = 3
} mpu6050_clock_t;

typedef enum { MPU6050_GYRO_250_DPS = 0, MPU6050_GYRO_500_DPS,
               MPU6050_GYRO_1000_DPS, MPU6050_GYRO_2000_DPS } mpu6050_gyro_range_t;
typedef enum { MPU6050_ACCEL_2G = 0, MPU6050_ACCEL_4G,
               MPU6050_ACCEL_8G, MPU6050_ACCEL_16G } mpu6050_accel_range_t;
typedef enum { MPU6050_DLPF_260_256 = 0, MPU6050_DLPF_184_188,
               MPU6050_DLPF_94_98, MPU6050_DLPF_44_42,
               MPU6050_DLPF_21_20, MPU6050_DLPF_10_10,
               MPU6050_DLPF_5_5 } mpu6050_dlpf_t;

typedef struct {
    int16_t accel_x_raw, accel_y_raw, accel_z_raw;
    int16_t temperature_raw;
    int16_t gyro_x_raw, gyro_y_raw, gyro_z_raw;
    float accel_x_g, accel_y_g, accel_z_g;
    float temperature_deg_c;
    float gyro_x_dps, gyro_y_dps, gyro_z_dps;
} mpu6050_sample_t;

typedef struct {
    uint8_t int_level;
    uint8_t int_open;
    uint8_t latch_int;
    uint8_t int_read_clear;
    uint8_t fsync_int_level;
    uint8_t fsync_int_enable;
    uint8_t i2c_bypass_enable;
} mpu6050_interrupt_pin_config_t;

typedef struct {
    bool fifo_overflow;
    bool i2c_master;
    bool data_ready;
} mpu6050_interrupt_status_t;

/* Caller-provided storage makes the driver allocation-free. */
size_t mpu6050_handle_size(void);
mpu6050_status_t mpu6050_init(mpu6050_handle_t *handle, size_t handle_size,
                              const mpu6050_io_t *io, bool primary_spi);
mpu6050_status_t mpu6050_deinit(mpu6050_handle_t *handle);
mpu6050_status_t mpu6050_reset(mpu6050_handle_t *handle);
mpu6050_status_t mpu6050_read_register(mpu6050_handle_t *handle, uint8_t reg,
                                       uint8_t *data, size_t len);
mpu6050_status_t mpu6050_write_register(mpu6050_handle_t *handle, uint8_t reg,
                                        const uint8_t *data, size_t len);
mpu6050_status_t mpu6050_read_modify_write(mpu6050_handle_t *handle, uint8_t reg,
                                            uint8_t mask, uint8_t value);
mpu6050_status_t mpu6050_wake(mpu6050_handle_t *handle, mpu6050_clock_t clock);
mpu6050_status_t mpu6050_set_gyro_range(mpu6050_handle_t *handle, mpu6050_gyro_range_t range);
mpu6050_status_t mpu6050_set_accel_range(mpu6050_handle_t *handle, mpu6050_accel_range_t range);
mpu6050_status_t mpu6050_set_sample_rate_divider(mpu6050_handle_t *handle, uint8_t divider);
mpu6050_status_t mpu6050_set_dlpf(mpu6050_handle_t *handle, mpu6050_dlpf_t setting);
mpu6050_status_t mpu6050_reset_signal_paths(mpu6050_handle_t *handle);
mpu6050_status_t mpu6050_read_sample(mpu6050_handle_t *handle, mpu6050_sample_t *sample);
mpu6050_status_t mpu6050_read_fifo_count(mpu6050_handle_t *handle, uint16_t *count);
mpu6050_status_t mpu6050_read_fifo(mpu6050_handle_t *handle, uint8_t *data, size_t len);
mpu6050_status_t mpu6050_write_fifo(mpu6050_handle_t *handle, const uint8_t *data, size_t len);
mpu6050_status_t mpu6050_set_fifo_enable(mpu6050_handle_t *handle, uint8_t enable_mask);
mpu6050_status_t mpu6050_enable_data_ready_interrupt(mpu6050_handle_t *handle, bool enable);
mpu6050_status_t mpu6050_configure_interrupt_pin(mpu6050_handle_t *handle,
                                                  const mpu6050_interrupt_pin_config_t *config);
mpu6050_status_t mpu6050_read_interrupt_status(mpu6050_handle_t *handle,
                                                mpu6050_interrupt_status_t *status);
mpu6050_status_t mpu6050_get_who_am_i(mpu6050_handle_t *handle, uint8_t *value);
mpu6050_status_t mpu6050_set_power(mpu6050_handle_t *handle, bool sleep, bool cycle,
                                   bool temperature_disabled, mpu6050_clock_t clock);

#ifdef __cplusplus
}
#endif
#endif
