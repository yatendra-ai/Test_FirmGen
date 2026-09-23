#ifndef DRIVE_SAFETY_H
#define DRIVE_SAFETY_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DRIVE_SAFETY_OK = 0,
    DRIVE_SAFETY_FAULT_DRIVER = 1,
    DRIVE_SAFETY_FAULT_PHASE_CURRENT = 2,
    DRIVE_SAFETY_FAULT_BUS_VOLTAGE = 3,
    DRIVE_SAFETY_FAULT_CURRENT_LIMIT = 4
} drive_safety_fault_t;

typedef struct {
    uint32_t phase_u_raw;
    uint32_t phase_v_raw;
    uint32_t phase_w_raw;
    uint32_t bus_voltage_raw;
    bool driver_fault;
} drive_safety_measurements_t;

bool drive_safety_validate(const drive_safety_measurements_t *p_measurements,
                           drive_safety_fault_t *p_fault);
bool drive_safety_fault_active(void);
drive_safety_fault_t drive_safety_fault(void);
void drive_safety_clear(void);

#endif /* DRIVE_SAFETY_H */
