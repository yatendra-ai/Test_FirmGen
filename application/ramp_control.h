#ifndef RAMP_CONTROL_H
#define RAMP_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the non-blocking PWM ramp timeline.
 * @param now_ms Current monotonic millisecond tick.
 */
void ramp_control_init(uint32_t now_ms);

/**
 * @brief Records one elapsed system-tick event for the main-context updater.
 */
void ramp_control_on_tick(void);

/**
 * @brief Calculates the next duty command when the configured update interval elapses.
 * @param now_ms Current monotonic millisecond tick.
 * @param duty_permille Destination for the calculated duty command.
 * @return true when the caller should apply the duty command; otherwise false.
 */
bool ramp_control_update(uint32_t now_ms, uint16_t *duty_permille);

#endif
