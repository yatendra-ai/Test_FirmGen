#ifndef SAFETY_PLATFORM_H
#define SAFETY_PLATFORM_H

#include <stdbool.h>
#include "x_nucleo_ihm16m1.h"

x_nucleo_ihm16m1_status_t safety_platform_init(void);
x_nucleo_ihm16m1_status_t safety_platform_check_fault(bool *fault);
x_nucleo_ihm16m1_status_t safety_platform_disable(void);
void safety_platform_watchdog_refresh(void);

#endif /* SAFETY_PLATFORM_H */
