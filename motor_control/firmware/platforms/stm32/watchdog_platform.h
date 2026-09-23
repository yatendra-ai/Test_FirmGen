#ifndef WATCHDOG_PLATFORM_H
#define WATCHDOG_PLATFORM_H

#include <stdbool.h>

bool watchdog_platform_start(void);
void watchdog_platform_kick(void);

#endif /* WATCHDOG_PLATFORM_H */
