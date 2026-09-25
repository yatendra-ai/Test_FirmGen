#ifndef DRV_BOOT_RECORD_H
#define DRV_BOOT_RECORD_H
#include <stdint.h>
/* Best-effort warm-reset breadcrumb, not persistent flash storage. */
void drv_boot_record_init(void);
void drv_boot_record_status(uint32_t status);
uint32_t drv_boot_record_count(void);
#endif
