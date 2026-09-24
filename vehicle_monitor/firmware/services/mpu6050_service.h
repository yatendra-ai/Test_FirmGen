#ifndef MPU6050_SERVICE_H
#define MPU6050_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float lateral_g;
    float longitudinal_g;
    float vertical_g;
    uint32_t failure_count;
    bool initialized;
    bool valid;
} mpu6050_state_t;

void mpu6050_service_start(void);
const mpu6050_state_t *mpu6050_service_state(void);

#endif /* MPU6050_SERVICE_H */
