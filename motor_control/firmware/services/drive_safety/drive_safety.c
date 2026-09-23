#include "drive_safety.h"

#include <stddef.h>

#include "app_config.h"

static drive_safety_fault_t g_fault;

static bool drive_safety_phase_valid(uint32_t raw)
{
    return (raw > APP_PHASE_CURRENT_MIN_RAW) &&
           (raw < APP_PHASE_CURRENT_MAX_RAW);
}

bool drive_safety_validate(const drive_safety_measurements_t *p_measurements,
                           drive_safety_fault_t *p_fault)
{
    if ((p_measurements == NULL) || (p_fault == NULL)) {
        return false;
    }

    if (p_measurements->driver_fault) {
        g_fault = DRIVE_SAFETY_FAULT_DRIVER;
    } else if (!drive_safety_phase_valid(p_measurements->phase_u_raw) ||
               !drive_safety_phase_valid(p_measurements->phase_v_raw) ||
               !drive_safety_phase_valid(p_measurements->phase_w_raw)) {
        g_fault = DRIVE_SAFETY_FAULT_PHASE_CURRENT;
    } else if ((p_measurements->bus_voltage_raw < APP_BUS_VOLTAGE_MIN_RAW) ||
               (p_measurements->bus_voltage_raw > APP_BUS_VOLTAGE_MAX_RAW)) {
        g_fault = DRIVE_SAFETY_FAULT_BUS_VOLTAGE;
    } else {
        g_fault = DRIVE_SAFETY_OK;
    }

    *p_fault = g_fault;
    return g_fault == DRIVE_SAFETY_OK;
}

bool drive_safety_fault_active(void)
{
    return g_fault != DRIVE_SAFETY_OK;
}

drive_safety_fault_t drive_safety_fault(void)
{
    return g_fault;
}

void drive_safety_clear(void)
{
    g_fault = DRIVE_SAFETY_OK;
}
