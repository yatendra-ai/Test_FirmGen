#ifndef MPU6050_PORT_H
#define MPU6050_PORT_H

#include <stdbool.h>
#include <stdint.h>

bool mpu6050_port_start(void);
bool mpu6050_port_read(uint8_t reg, uint8_t *data, uint32_t length);
bool mpu6050_port_write(uint8_t reg, const uint8_t *data, uint32_t length);
void mpu6050_port_delay_ms(uint32_t milliseconds);
bool mpu6050_port_probe(uint8_t address);

#endif /* MPU6050_PORT_H */
