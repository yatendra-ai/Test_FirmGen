#include "mpu6050.h"
#include <string.h>

struct mpu6050_handle {
    mpu6050_io_t io;
    bool initialized;
    bool spi;
    uint8_t gyro_fs;
    uint8_t accel_fs;
};

static mpu6050_status_t valid(const mpu6050_handle_t *h) {
    if (h == NULL) return MPU6050_E_INVALID_ARG;
    return h->initialized ? MPU6050_OK : MPU6050_E_NOT_INITIALIZED;
}

static mpu6050_status_t wr1(mpu6050_handle_t *h, uint8_t r, uint8_t v) {
    return h->io.write(h->io.ctx, r, &v, 1u);
}

static mpu6050_status_t rd1(mpu6050_handle_t *h, uint8_t r, uint8_t *v) {
    return h->io.read(h->io.ctx, r, v, 1u);
}

static int16_t be16(const uint8_t *p) {
    uint16_t u = ((uint16_t)p[0] << 8) | p[1];
    return (int16_t)u;
}

size_t mpu6050_handle_size(void) { return sizeof(mpu6050_handle_t); }

mpu6050_status_t mpu6050_read_register(mpu6050_handle_t *h, uint8_t r,
                                       uint8_t *data, size_t len) {
    mpu6050_status_t s = valid(h);
    if (s != MPU6050_OK || data == NULL || len == 0u || h->io.read == NULL)
        return (s != MPU6050_OK) ? s : MPU6050_E_INVALID_ARG;
    return h->io.read(h->io.ctx, r, data, len);
}

mpu6050_status_t mpu6050_write_register(mpu6050_handle_t *h, uint8_t r,
                                        const uint8_t *data, size_t len) {
    mpu6050_status_t s = valid(h);
    if (s != MPU6050_OK || data == NULL || len == 0u || h->io.write == NULL)
        return (s != MPU6050_OK) ? s : MPU6050_E_INVALID_ARG;
    return h->io.write(h->io.ctx, r, data, len);
}

mpu6050_status_t mpu6050_read_modify_write(mpu6050_handle_t *h, uint8_t r,
                                            uint8_t mask, uint8_t value) {
    uint8_t old;
    mpu6050_status_t s = rd1(h, r, &old);
    if (s != MPU6050_OK) return s;
    old = (uint8_t)((old & (uint8_t)~mask) | (value & mask));
    return wr1(h, r, old);
}

mpu6050_status_t mpu6050_get_who_am_i(mpu6050_handle_t *h, uint8_t *value) {
    if (value == NULL) return MPU6050_E_INVALID_ARG;
    return rd1(h, MPU6050_REG_WHO_AM_I, value);
}

mpu6050_status_t mpu6050_reset(mpu6050_handle_t *h) {
    mpu6050_status_t s = valid(h);
    if (s != MPU6050_OK) return s;
    s = wr1(h, MPU6050_REG_PWR_MGMT_1, MPU6050_PWR_DEVICE_RESET);
    if (s != MPU6050_OK) return s;
    h->gyro_fs = 0u; h->accel_fs = 0u;
    if (h->io.delay_ms == NULL) return MPU6050_E_INVALID_ARG;
    h->io.delay_ms(h->io.ctx, 100u);
    s = wr1(h, MPU6050_REG_SIGNAL_PATH_RESET, MPU6050_SIGNAL_RESET_ALL);
    if (s != MPU6050_OK) return s;
    h->io.delay_ms(h->io.ctx, 100u);
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_init(mpu6050_handle_t *h, size_t size,
                              const mpu6050_io_t *io, bool primary_spi) {
    uint8_t id;
    mpu6050_status_t s;
    if (h == NULL || io == NULL || size < sizeof(*h) || io->read == NULL ||
        io->write == NULL || io->delay_ms == NULL) return MPU6050_E_INVALID_ARG;
    memset(h, 0, sizeof(*h));
    h->io = *io; h->spi = primary_spi;
    h->initialized = true;
    s = mpu6050_get_who_am_i(h, &id);
    if (s != MPU6050_OK) { h->initialized = false; return s; }
    if ((id & MPU6050_WHO_AM_I_MASK) != (MPU6050_WHO_AM_I_VALUE & MPU6050_WHO_AM_I_MASK)) {
        h->initialized = false; return MPU6050_E_ID_MISMATCH;
    }
    s = mpu6050_reset(h); if (s != MPU6050_OK) { h->initialized = false; return s; }
    s = wr1(h, MPU6050_REG_PWR_MGMT_1, (uint8_t)MPU6050_CLOCK_PLL_X_GYRO); if (s != MPU6050_OK) return s;
    s = wr1(h, MPU6050_REG_CONFIG, 0u); if (s != MPU6050_OK) return s;
    s = wr1(h, MPU6050_REG_SMPLRT_DIV, 0u); if (s != MPU6050_OK) return s;
    s = wr1(h, MPU6050_REG_GYRO_CONFIG, 0u); if (s != MPU6050_OK) return s;
    s = wr1(h, MPU6050_REG_ACCEL_CONFIG, 0u); if (s != MPU6050_OK) return s;
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_deinit(mpu6050_handle_t *h) {
    if (h == NULL) return MPU6050_E_INVALID_ARG;
    memset(h, 0, sizeof(*h));
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_wake(mpu6050_handle_t *h, mpu6050_clock_t clock) {
    if (clock > MPU6050_CLOCK_PLL_Z_GYRO) return MPU6050_E_INVALID_ARG;
    return mpu6050_set_power(h, false, false, false, clock);
}

mpu6050_status_t mpu6050_set_power(mpu6050_handle_t *h, bool sleep, bool cycle,
                                   bool temp_dis, mpu6050_clock_t clock) {
    uint8_t v;
    if (clock > 7u) return MPU6050_E_INVALID_ARG;
    v = (uint8_t)((sleep ? MPU6050_PWR_SLEEP : 0u) |
                  (cycle ? MPU6050_PWR_CYCLE : 0u) |
                  (temp_dis ? MPU6050_PWR_TEMP_DIS : 0u) | (uint8_t)clock);
    return mpu6050_read_modify_write(h, MPU6050_REG_PWR_MGMT_1,
                                      (uint8_t)(MPU6050_PWR_SLEEP | MPU6050_PWR_CYCLE |
                                      MPU6050_PWR_TEMP_DIS | MPU6050_PWR_CLKSEL_MASK), v);
}

mpu6050_status_t mpu6050_set_gyro_range(mpu6050_handle_t *h, mpu6050_gyro_range_t r) {
    if (r > MPU6050_GYRO_2000_DPS) return MPU6050_E_INVALID_ARG;
    mpu6050_status_t s = mpu6050_read_modify_write(h, MPU6050_REG_GYRO_CONFIG,
                                                     MPU6050_GYRO_FS_MASK, (uint8_t)r << 3);
    if (s == MPU6050_OK) h->gyro_fs = (uint8_t)r;
    return s;
}

mpu6050_status_t mpu6050_set_accel_range(mpu6050_handle_t *h, mpu6050_accel_range_t r) {
    if (r > MPU6050_ACCEL_16G) return MPU6050_E_INVALID_ARG;
    mpu6050_status_t s = mpu6050_read_modify_write(h, MPU6050_REG_ACCEL_CONFIG,
                                                     MPU6050_ACCEL_FS_MASK, (uint8_t)r << 3);
    if (s == MPU6050_OK) h->accel_fs = (uint8_t)r;
    return s;
}

mpu6050_status_t mpu6050_set_sample_rate_divider(mpu6050_handle_t *h, uint8_t d) {
    return wr1(h, MPU6050_REG_SMPLRT_DIV, d);
}

mpu6050_status_t mpu6050_set_dlpf(mpu6050_handle_t *h, mpu6050_dlpf_t s) {
    if (s > MPU6050_DLPF_5_5) return MPU6050_E_INVALID_ARG;
    return mpu6050_read_modify_write(h, MPU6050_REG_CONFIG, MPU6050_CONFIG_DLPF_MASK, (uint8_t)s);
}

mpu6050_status_t mpu6050_reset_signal_paths(mpu6050_handle_t *h) {
    return wr1(h, MPU6050_REG_SIGNAL_PATH_RESET, MPU6050_SIGNAL_RESET_ALL);
}

mpu6050_status_t mpu6050_read_sample(mpu6050_handle_t *h, mpu6050_sample_t *x) {
    uint8_t b[14];
    static const float accel_lsb[4] = {16384.0f, 8192.0f, 4096.0f, 2048.0f};
    static const float gyro_lsb[4] = {131.0f, 65.5f, 32.8f, 16.4f};
    mpu6050_status_t s;
    if (x == NULL) return MPU6050_E_INVALID_ARG;
    s = mpu6050_read_register(h, MPU6050_REG_ACCEL_XOUT_H, b, sizeof(b));
    if (s != MPU6050_OK) return s;
    x->accel_x_raw = be16(&b[0]); x->accel_y_raw = be16(&b[2]); x->accel_z_raw = be16(&b[4]);
    x->temperature_raw = be16(&b[6]); x->gyro_x_raw = be16(&b[8]);
    x->gyro_y_raw = be16(&b[10]); x->gyro_z_raw = be16(&b[12]);
    x->accel_x_g = (float)x->accel_x_raw / accel_lsb[h->accel_fs & 3u];
    x->accel_y_g = (float)x->accel_y_raw / accel_lsb[h->accel_fs & 3u];
    x->accel_z_g = (float)x->accel_z_raw / accel_lsb[h->accel_fs & 3u];
    x->temperature_deg_c = (float)x->temperature_raw / 340.0f + 36.53f;
    x->gyro_x_dps = (float)x->gyro_x_raw / gyro_lsb[h->gyro_fs & 3u];
    x->gyro_y_dps = (float)x->gyro_y_raw / gyro_lsb[h->gyro_fs & 3u];
    x->gyro_z_dps = (float)x->gyro_z_raw / gyro_lsb[h->gyro_fs & 3u];
    return MPU6050_OK;
}

mpu6050_status_t mpu6050_read_fifo_count(mpu6050_handle_t *h, uint16_t *count) {
    uint8_t b[2]; mpu6050_status_t s;
    if (count == NULL) return MPU6050_E_INVALID_ARG;
    s = mpu6050_read_register(h, MPU6050_REG_FIFO_COUNTH, b, 2u);
    if (s == MPU6050_OK) *count = (uint16_t)(((uint16_t)b[0] << 8) | b[1]);
    return s;
}

mpu6050_status_t mpu6050_read_fifo(mpu6050_handle_t *h, uint8_t *d, size_t n) {
    return mpu6050_read_register(h, MPU6050_REG_FIFO_R_W, d, n);
}
mpu6050_status_t mpu6050_write_fifo(mpu6050_handle_t *h, const uint8_t *d, size_t n) {
    return mpu6050_write_register(h, MPU6050_REG_FIFO_R_W, d, n);
}
mpu6050_status_t mpu6050_set_fifo_enable(mpu6050_handle_t *h, uint8_t mask) {
    return wr1(h, MPU6050_REG_FIFO_EN, mask);
}

mpu6050_status_t mpu6050_enable_data_ready_interrupt(mpu6050_handle_t *h, bool enable) {
    return mpu6050_read_modify_write(h, MPU6050_REG_INT_ENABLE, MPU6050_INT_DATA_RDY_EN,
                                      enable ? MPU6050_INT_DATA_RDY_EN : 0u);
}

mpu6050_status_t mpu6050_configure_interrupt_pin(mpu6050_handle_t *h,
                                                  const mpu6050_interrupt_pin_config_t *c) {
    uint8_t v;
    if (c == NULL || c->int_level > 1u || c->int_open > 1u || c->latch_int > 1u ||
        c->int_read_clear > 1u || c->fsync_int_level > 1u || c->fsync_int_enable > 1u ||
        c->i2c_bypass_enable > 1u) return MPU6050_E_INVALID_ARG;
    v = (uint8_t)((c->int_level << 7) | (c->int_open << 6) | (c->latch_int << 5) |
                  (c->int_read_clear << 4) | (c->fsync_int_level << 3) |
                  (c->fsync_int_enable << 2) | (c->i2c_bypass_enable << 1));
    return wr1(h, MPU6050_REG_INT_PIN_CFG, v);
}

mpu6050_status_t mpu6050_read_interrupt_status(mpu6050_handle_t *h,
                                                mpu6050_interrupt_status_t *out) {
    uint8_t v; mpu6050_status_t s;
    if (out == NULL) return MPU6050_E_INVALID_ARG;
    s = rd1(h, MPU6050_REG_INT_STATUS, &v);
    if (s != MPU6050_OK) return s;
    out->fifo_overflow = (v & MPU6050_INT_FIFO_OFLOW) != 0u;
    out->i2c_master = (v & MPU6050_INT_I2C_MST) != 0u;
    out->data_ready = (v & MPU6050_INT_DATA_RDY) != 0u;
    return MPU6050_OK;
}
