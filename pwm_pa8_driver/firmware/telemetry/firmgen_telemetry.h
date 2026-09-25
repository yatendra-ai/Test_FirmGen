/**
 * firmgen_telemetry.h — Firmgen live telemetry for STM32 HAL2 (STM32C5 family).
 *
 * HAL2 is ST's new driver generation (stm32c5xx_drivers) and is NOT source-compatible
 * with the classic HAL used by F4/F7/G4/H7/L4/U5/…, so this is a separate variant of the
 * component rather than an #ifdef inside the HAL1 one. Verified differences that force
 * the split:
 *
 *   | concern        | HAL1                              | HAL2 (this file)                  |
 *   |----------------|-----------------------------------|-----------------------------------|
 *   | UART handle    | UART_HandleTypeDef                | hal_uart_handle_t                 |
 *   | transmit       | (h, uint8_t*, uint16_t, u32)      | (h, const void*, uint32_t, u32)   |
 *   | reset flags    | RCC_FLAG_*RST + __HAL_RCC_GET_FLAG| HAL_RCC_RESET_FLAG_* +            |
 *   |                |                                   | HAL_RCC_GetResetSource()          |
 *   | clear flags    | __HAL_RCC_CLEAR_RESET_FLAGS()     | HAL_RCC_ClearResetFlags()         |
 *   | module headers | pulled in by stm32<f>xx_hal.h     | must be included per module       |
 *   | HAL_OK         | 0                                 | 0xEAEAEAEA                        |
 *
 * ## Periodic sampling: you must call firmgen_telemetry_poll()
 *
 * HAL1 drives sampling by overriding the weak `HAL_SYSTICK_Callback`. **HAL2 has no such
 * callback** — it does not exist anywhere in the driver set. `HAL_IncTick` IS `__WEAK`,
 * but its body increments the HAL's *private static* `uwTick`, which an override cannot
 * reach; overriding it would silently break `HAL_GetTick()` and `HAL_Delay()` for the
 * whole application. So there is no safe tick hook on HAL2, and telemetry is instead
 * driven by an explicit poll from the application's main loop:
 *
 *     #include "firmgen_telemetry.h"
 *     firmgen_telemetry_start(&huart2);   // once, after the UART is initialised
 *     for (;;) {
 *         firmgen_telemetry_poll();       // cheap; samples only when the interval elapses
 *     }
 *
 * The injector patches both call sites automatically. Consequence worth knowing: if the
 * application blocks for a long time inside its loop, telemetry stalls for exactly that
 * long — unlike the HAL1 variant, which samples from the SysTick interrupt and keeps
 * running through blocking application code.
 *
 * ## Debug vs production
 *
 * Same contract as HAL1 and as ESP: the generated `fg_telemetry_profile.h` either defines
 * FG_TELEMETRY_PROFILE_PRODUCTION or does not. Production compiles the sampler, emitters
 * and boot event out entirely — `firmgen_telemetry_start()` and
 * `firmgen_telemetry_poll()` become no-ops and nothing reaches the wire.
 *
 * ## Reset reason: one honest limitation vs HAL1
 *
 * HAL2 exposes a single `HAL_RCC_RESET_FLAG_PWR` covering "BOR or POR/PDR" — brownout and
 * ordinary power-on share one bit, where HAL1 has separate PORRST/BORRST. They therefore
 * cannot be distinguished on C5, and this reports `power_reset` classified as NOT a crash.
 * Treating it as a crash would raise a false alarm on every normal power-up, which is the
 * worse error.
 *
 * ## Linker requirement
 *
 * Free-RAM sampling needs the heap top and does NOT require a specific linker symbol:
 * it tries `_sbrk(0)` first (ST's own allocator view), then the weak symbols
 * `__end__`, `_end`, `end`, `_heap_start`, and reports free RAM as unavailable if none
 * resolve. Hard-requiring `_end` broke the link on a real C5 project — ST's
 * new-generation linker script defines `__end__`, not `_end`.
 */

#pragma once

/* HAL2 requires explicit per-module includes — stm32c5xx_hal.h pulls in only hal_def and
 * the LL system/utils headers, NOT the peripheral modules (unlike classic HAL). */
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_rcc.h"
#include "stm32c5xx_hal_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Start telemetry. Idempotent. No-op in the production profile.
 *
 * @param huart  HAL2 UART handle, already HAL_UART_Init()'d and configured. Telemetry
 *               only transmits on it; it never configures the peripheral.
 */
void firmgen_telemetry_start(hal_uart_handle_t *huart);

/**
 * Drive periodic sampling. Call from the application main loop — see the note above on
 * why HAL2 cannot use a tick callback. Returns immediately until the sampling interval
 * has elapsed, so calling it every loop iteration is intended and cheap. No-op in the
 * production profile.
 */
void firmgen_telemetry_poll(void);

#ifdef __cplusplus
}
#endif
