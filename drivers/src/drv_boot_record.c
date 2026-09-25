#include "drv_boot_record.h"
#include "board.h"

#define BOOT_MAGIC 0x424F4F54UL
typedef struct { uint32_t magic, count, status, check; } boot_record_t;
static volatile boot_record_t s_record BOARD_NOINIT;

/* This customer routine exercises the RAM load/copy-table linker contract. */
static uint32_t BOARD_RAMFUNC record_check(uint32_t count, uint32_t status)
{
    return BOOT_MAGIC ^ count ^ status ^ 0xA56E192CUL;
}

void drv_boot_record_init(void)
{
    uint32_t count = 0U;
    if (s_record.magic == BOOT_MAGIC && s_record.check == record_check(s_record.count, s_record.status))
    { count = s_record.count; }
    s_record.magic = 0U;
    s_record.count = count + 1U;
    s_record.status = 0U;
    s_record.check = record_check(s_record.count, s_record.status);
    s_record.magic = BOOT_MAGIC;
}
void drv_boot_record_status(uint32_t status)
{
    s_record.magic = 0U;
    s_record.status = status;
    s_record.check = record_check(s_record.count, status);
    s_record.magic = BOOT_MAGIC;
}
uint32_t drv_boot_record_count(void) { return s_record.count; }
