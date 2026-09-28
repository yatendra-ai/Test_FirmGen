/*
 * firmgen_telemetry.c — Firmgen live telemetry for STM32 HAL2 (STM32C5).
 *
 * Emits the SAME wire format as the ESP and HAL1 components, so the shared backend
 * parser, schema and Live Intelligence panel all apply unchanged. See
 * firmgen_telemetry.h for the HAL1-vs-HAL2 API differences that force this to be a
 * separate file, and for why sampling is polled rather than tick-driven here.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "firmgen_telemetry.h"
#include "fg_telemetry_profile.h"

/* ── Debug/production profile ─────────────────────────────────────────────────
 * Same mechanism as HAL1 and ESP: one generated header decides, rewritten by the
 * profile switch. */
#ifdef FG_TELEMETRY_PROFILE_PRODUCTION
#define FG_TELEMETRY_ENABLED 0
#else
#define FG_TELEMETRY_ENABLED 1
#endif

#if FG_TELEMETRY_ENABLED

#define FG_VERSION 1u
#ifndef FG_TELEMETRY_INTERVAL_MS
#define FG_TELEMETRY_INTERVAL_MS 500u
#endif

/* Match to the project's SRAM size / reserved stack. Defaults are conservative; the
 * numbers are only as good as these constants (documented in the header). */
#ifndef FG_TELEMETRY_TOTAL_RAM_BYTES
#define FG_TELEMETRY_TOTAL_RAM_BYTES (128u * 1024u)
#endif
#ifndef FG_TELEMETRY_STACK_RESERVE_BYTES
#define FG_TELEMETRY_STACK_RESERVE_BYTES (2u * 1024u)
#endif

/* FreeRTOS is optional. FG_FREERTOS_PRESENT is defined by the injector (in the generated
 * fg_telemetry_profile.h) only when the project actually contains FreeRTOS, so a
 * bare-metal build never sees these includes. */
#if FG_FREERTOS_PRESENT
#include "FreeRTOS.h"
#include "task.h"
#define FG_TOP_TASKS_MAX 3u
#endif

/* Heap top, resolved without requiring any ONE linker-script symbol.
 *
 * The first version hard-required `_end` and failed to LINK on a real STM32C5 project:
 * ST's new-generation linker script defines `__end__`, `end` and `_heap_start` but NOT
 * `_end`, and its own sysmem.c uses `__end__`. Classic CubeMX scripts PROVIDE `_end`,
 * which is why a vendor-template fixture never caught it. Rather than demand the user
 * edit their linker script, probe what is actually there:
 *
 *   1. `_sbrk(0)` — the allocator's own heap top. Most accurate, because it accounts
 *      for memory already handed to malloc rather than just the static end of .bss,
 *      and it is present in both ST generations. Returns -1 under nosys.specs.
 *   2. the weak linker symbols, in decreasing specificity.
 *   3. nothing found -> report free RAM as 0, which the GUI shows as unavailable. A
 *      missing number is better than a wrong one, and better than a build that fails.
 *
 * All declarations are weak, so an absent symbol links to 0 and is skipped at runtime
 * instead of breaking the link. */
extern uint8_t __end__      __attribute__((weak));  /* ST new-generation + CMSIS */
extern uint8_t _end         __attribute__((weak));  /* classic CubeMX */
extern uint8_t end          __attribute__((weak));  /* PROVIDE (end = .) */
extern uint8_t _heap_start  __attribute__((weak));  /* ST C5 .heap section */
extern void *_sbrk(int incr) __attribute__((weak));

static uint32_t heap_top(void)
{
    if (_sbrk != NULL) {
        void *p = _sbrk(0);
        if (p != NULL && p != (void *)-1) return (uint32_t)p;
    }
    if (&__end__ != NULL)     return (uint32_t)&__end__;
    if (&_end != NULL)        return (uint32_t)&_end;
    if (&end != NULL)         return (uint32_t)&end;
    if (&_heap_start != NULL) return (uint32_t)&_heap_start;
    return 0u;
}

static hal_uart_handle_t *s_huart = NULL;
static uint32_t           s_min_free_ram = 0xFFFFFFFFu;
static uint32_t           s_last_ms = 0u;
static bool               s_started = false;

/* -- Transmit -----------------------------------------------------------------
 *
 * Two ways out, because FirmGen-generated projects expose their UART in more than one
 * way. Normally telemetry is handed a HAL handle and uses HAL_UART_Transmit, which
 * respects that handle's busy state and so cannot interleave with the application's own
 * HAL traffic.
 *
 * FG_TELEMETRY_RAW_USART selects the other way: the application configured the peripheral
 * with direct register writes and keeps no HAL handle at all. A real project did exactly
 * that -- USART2->BRR and ->CR1 set by hand, with a printf retarget polling TXE and
 * poking TDR -- so there was no `UART_HandleTypeDef` anywhere to borrow, static or not,
 * and telemetry was refused with "No usable UART found in this project". In that mode
 * telemetry polls TXE and writes TDR itself. It still never configures the peripheral:
 * BRR, CR1 and the pin mux stay entirely the application's business.
 *
 * The trade-off is real and worth stating: without the HAL lock a telemetry line and an
 * application printf can interleave byte-wise. Each side writes one short line at a time
 * so it is uncommon, and the host parser discards a malformed line rather than choking,
 * but a HAL handle remains the better source whenever one exists -- which is why the
 * injector only falls back to this. */
static void fg_uart_write(const uint8_t *data, size_t len)
{
#ifdef FG_TELEMETRY_RAW_USART
    for (size_t i = 0u; i < len; i++) {
#if defined(USART_ISR_TXE_TXFNF)
        while ((FG_TELEMETRY_RAW_USART->ISR & USART_ISR_TXE_TXFNF) == 0u) { }
#else
        while ((FG_TELEMETRY_RAW_USART->ISR & USART_ISR_TXE) == 0u) { }
#endif
        FG_TELEMETRY_RAW_USART->TDR = (uint8_t)data[i];
    }
#else
    if (s_huart == NULL) {
        return;
    }
    (void)HAL_UART_Transmit(s_huart, data, (uint32_t)len, 100u);
#endif
}

/* ── Reset / crash reason ─────────────────────────────────────────────────── */

/* HAL2 returns a bitmask from HAL_RCC_GetResetSource() rather than offering
 * per-flag test macros. Order matters: several bits can be set together, so the more
 * specific/severe cause is reported first.
 *
 * Strings and the crash classification are kept identical to the HAL1 variant and to
 * BootEvent::classify() on the Rust side — the whole point is that the GUI cannot tell
 * which HAL generation produced a reading. The one exception is `power_reset`, which
 * exists only here: HAL2 merges BOR and POR into one bit (see the header). */
static const char *reset_reason_str(uint32_t src, bool *is_crash)
{
    *is_crash = false;
    if (src & HAL_RCC_RESET_FLAG_IWDG) { *is_crash = true; return "independent_watchdog"; }
    if (src & HAL_RCC_RESET_FLAG_WWDG) { *is_crash = true; return "window_watchdog"; }
    if (src & HAL_RCC_RESET_FLAG_LPWR) { *is_crash = true; return "low_power_reset"; }
    /* NOT a crash: this bit is BOR *or* POR/PDR and the two are indistinguishable on
     * HAL2, so flagging it would false-alarm on every normal power-up. */
    if (src & HAL_RCC_RESET_FLAG_PWR)  { return "power_reset"; }
    if (src & HAL_RCC_RESET_FLAG_SW)   { return "sw_reset"; }
    if (src & HAL_RCC_RESET_FLAG_PIN)  { return "ext_pin"; }
    return "unknown";
}

static void emit_boot_event(void)
{
    bool is_crash = false;
    uint32_t src = HAL_RCC_GetResetSource();
    const char *reason = reset_reason_str(src, &is_crash);

    /* static for the same reason as emit_json's buffer: keep telemetry's stack frame
     * negligible on targets with a small main stack. Distinct from that buffer, so a boot
     * line and a snapshot can never share storage. */
    static char line[128];
    int n = snprintf(line, sizeof(line),
        "{\"t\":\"boot\",\"v\":%u,\"reason\":\"%s\",\"code\":%lu,\"crash\":%s}\r\n",
        (unsigned)FG_VERSION, reason, (unsigned long)src, is_crash ? "true" : "false");
    if (n > 0) {
        size_t len = ((size_t)n < sizeof(line)) ? (size_t)n : sizeof(line) - 1u;
        /* HAL2 takes `const void *` plus a uint32_t length, where HAL1 took a
         * `uint8_t *` plus a uint16_t. */
        fg_uart_write((const uint8_t *)line, len);
    }
    /* Clear after reading, or the next boot inherits these flags and misreports forever. */
    HAL_RCC_ClearResetFlags();
}

/* ── Sampling ─────────────────────────────────────────────────────────────── */

static uint32_t free_ram_bytes(void)
{
    uint32_t sp = __get_MSP();
    uint32_t heap_end = heap_top();
    if (heap_end == 0u) return 0u;   /* unknown -> reported as unavailable */
    return (sp > heap_end) ? (sp - heap_end) : 0u;
}

static uint32_t stack_used_pct(void)
{
    uint32_t free_bytes = free_ram_bytes();
    if (free_bytes >= FG_TELEMETRY_STACK_RESERVE_BYTES) return 0u;
    uint32_t used = FG_TELEMETRY_STACK_RESERVE_BYTES - free_bytes;
    uint32_t pct = (used * 100u) / FG_TELEMETRY_STACK_RESERVE_BYTES;
    return (pct > 100u) ? 100u : pct;
}

#if FG_FREERTOS_PRESENT
typedef struct {
    char     name[configMAX_TASK_NAME_LEN];
    uint32_t cpu_pct;
    uint32_t stack_min;
} fg_task_t;

/* Same algorithm as the HAL1 variant and the ESP component: FreeRTOS's task API is
 * kernel-level and identical across ports, so only the transport differs here.
 * Reports zeros when the project's FreeRTOSConfig.h has the collection options off —
 * the injector turns them on, but a user may have turned them back off. */
static void sample_tasks(uint32_t *out_cpu_pct, uint32_t *out_task_count,
                         fg_task_t *top, uint32_t top_max, uint32_t *out_top_n)
{
    *out_cpu_pct = 0u;
    *out_task_count = 0u;
    *out_top_n = 0u;

#if (configGENERATE_RUN_TIME_STATS == 1) && (configUSE_TRACE_FACILITY == 1)
    uint32_t n = uxTaskGetNumberOfTasks();
    *out_task_count = n;

    TaskStatus_t *buf = pvPortMalloc(n * sizeof(TaskStatus_t));
    if (buf == NULL) return;

    uint32_t total_ticks = 0u;
    uint32_t found = uxTaskGetSystemState(buf, n, &total_ticks);

    uint32_t idle_ticks = 0u;
    for (uint32_t i = 0u; i < found; i++) {
        if (strncmp(buf[i].pcTaskName, "IDLE", 4) == 0) {
            idle_ticks += buf[i].ulRunTimeCounter;
        }
    }

    uint32_t top_n = 0u;
    if (top != NULL && top_max > 0u && total_ticks > 0u) {
        for (uint32_t i = 0u; i < found; i++) {
            if (strncmp(buf[i].pcTaskName, "IDLE", 4) == 0) continue;
            uint32_t pct = (uint32_t)(((uint64_t)buf[i].ulRunTimeCounter * 100u) / total_ticks);
            uint32_t insert_at = top_n;
            for (uint32_t j = 0u; j < top_n; j++) {
                if (pct > top[j].cpu_pct) { insert_at = j; break; }
            }
            if (insert_at >= top_max) continue;
            uint32_t shift_end = (top_n < top_max) ? top_n : top_max - 1u;
            for (uint32_t j = shift_end; j > insert_at; j--) top[j] = top[j - 1u];
            strncpy(top[insert_at].name, buf[i].pcTaskName, sizeof(top[insert_at].name) - 1u);
            top[insert_at].name[sizeof(top[insert_at].name) - 1u] = '\0';
            top[insert_at].cpu_pct   = pct;
            top[insert_at].stack_min = (uint32_t)buf[i].usStackHighWaterMark;
            if (top_n < top_max) top_n++;
        }
    }
    *out_top_n = top_n;
    vPortFree(buf);

    uint32_t idle_pct = (total_ticks > 0u) ? ((idle_ticks * 100u) / total_ticks) : 100u;
    *out_cpu_pct = (idle_pct > 100u) ? 0u : (100u - idle_pct);
#else
    (void)top; (void)top_max;
#endif
}
#endif /* FG_FREERTOS_PRESENT */

static void emit_json(uint32_t free_ram, uint32_t min_free_ram,
                      uint32_t stack_pct, uint32_t uptime_s,
                      uint32_t cpu_pct, uint32_t task_count,
                      const void *top_v, uint32_t top_n)
{
    /* `static`, NOT a stack array. This buffer is half a kilobyte, and ST's C5 template
     * ships a 1 KB main stack (STACK_SIZE = 0x400 in stm32c5a3xg_flash.ld). A 512-byte
     * frame plus newlib's snprintf frame plus the app's own call depth overflowed it, and
     * the Cortex-M33 stack-limit check turned that into a HardFault at the very first
     * 500 ms emit: the board booted, ran, started telemetry, then died silently. Telemetry
     * must never be the thing that overruns the stack it is measuring, so the buffer lives
     * in .bss and the emit frame stays tiny. Safe because emit_json has a single call site
     * and cannot re-enter itself. */
    static char line[512];
    size_t off = 0;
    /* cpu/tasks are real when FreeRTOS is present AND its collection options are on;
     * otherwise 0, which the GUI explains as "needs an RTOS" rather than a fault. */
    int n = snprintf(line, sizeof(line),
        "{\"t\":\"rtos\",\"v\":%u,\"cpu\":%lu,\"tasks\":%lu,"
        "\"heap\":{\"free\":%lu,\"min\":%lu,\"total\":%lu},"
        "\"stack_peak\":%lu,\"uptime\":%lu",
        (unsigned)FG_VERSION,
        (unsigned long)cpu_pct, (unsigned long)task_count,
        (unsigned long)free_ram, (unsigned long)min_free_ram,
        (unsigned long)FG_TELEMETRY_TOTAL_RAM_BYTES,
        (unsigned long)stack_pct, (unsigned long)uptime_s);
    if (n <= 0) return;
    off = ((size_t)n < sizeof(line)) ? (size_t)n : sizeof(line) - 1u;

#if FG_FREERTOS_PRESENT
    const fg_task_t *top = (const fg_task_t *)top_v;
    if (top != NULL && top_n > 0u) {
        int m = snprintf(line + off, sizeof(line) - off, ",\"top_tasks\":[");
        if (m > 0) off += ((size_t)m < sizeof(line) - off) ? (size_t)m : sizeof(line) - off - 1u;
        for (uint32_t i = 0u; i < top_n; i++) {
            m = snprintf(line + off, sizeof(line) - off,
                "%s{\"name\":\"%s\",\"cpu\":%lu,\"stack_min\":%lu}",
                (i == 0u) ? "" : ",", top[i].name,
                (unsigned long)top[i].cpu_pct, (unsigned long)top[i].stack_min);
            if (m > 0) off += ((size_t)m < sizeof(line) - off) ? (size_t)m : sizeof(line) - off - 1u;
        }
        if (off < sizeof(line) - 1u) line[off++] = ']';
    }
#else
    (void)top_v; (void)top_n;
#endif

    if (off < sizeof(line) - 3u) { line[off++] = '}'; line[off++] = '\r'; line[off++] = '\n'; }
    fg_uart_write((const uint8_t *)line, off);
}

#endif /* FG_TELEMETRY_ENABLED */

/* ── FreeRTOS run-time-stats timebase (DWT cycle counter) ─────────────────────
 *
 * `configGENERATE_RUN_TIME_STATS` is inert unless the port also supplies
 * portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() and portGET_RUN_TIME_COUNTER_VALUE().
 * ST's generated FreeRTOSConfig.h defines neither, so enabling the option alone still
 * yields 0% for every task. The injector points those two macros at the functions
 * below.
 *
 * DWT's cycle counter is used rather than a hardware timer because it consumes no
 * peripheral the application might want, and it exists on Cortex-M4/M7/M33 (all the
 * cores these families use).
 *
 * Two honest limits: DWT is part of the debug unit, so a target that powers it down
 * reports 0; and CYCCNT is 32-bit, so after the >>10 scaling below it wraps in a few
 * hours, after which FreeRTOS's cumulative percentages drift. Both are acceptable for
 * a development-time metric, which is the only profile this is compiled into.
 *
 * These are defined unconditionally on FG_FREERTOS_PRESENT (not on the stats option)
 * so that FreeRTOSConfig.h can reference them without ordering constraints.
 */
#if FG_FREERTOS_PRESENT
void fg_telemetry_init_runtime_stats(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0u;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t fg_telemetry_runtime_counter(void)
{
    /* FreeRTOS wants a counter ticking well above the scheduler tick but slow enough not
     * to wrap immediately; >>10 puts a 100-200 MHz core in the ~100-200 kHz range. */
    return (uint32_t)(DWT->CYCCNT >> 10);
}
#endif

/* ── Public API ───────────────────────────────────────────────────────────── */

void firmgen_telemetry_start(hal_uart_handle_t *huart)
{
#if FG_TELEMETRY_ENABLED
    if (s_started) return; /* idempotent */
    s_huart = huart;
    s_min_free_ram = free_ram_bytes();
    s_last_ms = HAL_GetTick();
    emit_boot_event();
    s_started = true;
#else
    (void)huart;
#endif
}

void firmgen_telemetry_poll(void)
{
#if FG_TELEMETRY_ENABLED
    if (!s_started) return;

    /* Unsigned subtraction, so this stays correct across the 32-bit tick wraparound
     * (~49.7 days) instead of stalling telemetry until the counter catches up. */
    uint32_t now = HAL_GetTick();
    if ((uint32_t)(now - s_last_ms) < FG_TELEMETRY_INTERVAL_MS) return;
    s_last_ms = now;

    uint32_t free_ram = free_ram_bytes();
    if (free_ram < s_min_free_ram) s_min_free_ram = free_ram;

    uint32_t cpu_pct = 0u, task_count = 0u, top_n = 0u;
#if FG_FREERTOS_PRESENT
    fg_task_t top[FG_TOP_TASKS_MAX];
    sample_tasks(&cpu_pct, &task_count, top, FG_TOP_TASKS_MAX, &top_n);
    emit_json(free_ram, s_min_free_ram, stack_used_pct(), now / 1000u,
              cpu_pct, task_count, top, top_n);
#else
    emit_json(free_ram, s_min_free_ram, stack_used_pct(), now / 1000u,
              cpu_pct, task_count, NULL, top_n);
#endif
#endif
}
