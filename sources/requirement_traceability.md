# Requirement traceability

Generated 2026-09-25T06:21:23.746818700+00:00
Specification frozen 2026-09-25T06:13:00.836Z

25 of 25 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 25 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project. Host tests run on a separate machine; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## PWM Peripheral, Pin & Electrical Interface

gate: deferred · tasks: 4/4 done · recorded in: 4edac7c9-bc3f-4444-b99c-5d52acd7e63e

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-001` | The firmware shall output the PWM signal on Arduino D7 (CN9 pin 8, MCU pin PA8). | hardware / instrument | A PWM waveform is observable on Arduino D7 (CN9 pin 8, MCU PA8) while the firmware is operating. (expected: A periodic PWM waveform is present on Arduino D7 during operation.) | application/board.h<br>drivers/src/drv_pwm.c<br>generated/hal/mx_tim1.c<br>generated/hal/mx_tim1.h | drv_pwm_init, drv_pwm_start, mx_tim1_init | — | implemented, untested |
| `IF-002` | The firmware shall configure PA8 in a timer alternate-function mode that supports PWM output (e.g., TIM1 Channel 1). | inspection | The GPIO and timer configuration assigns PA8 to a timer alternate-function capable of PWM output, such as TIM1_CH1. (expected: PA8 is configured for a PWM-capable timer alternate function, specifically the selected mapping such as TIM1_CH1.) | application/board.h<br>drivers/src/drv_pwm.c<br>generated/hal/mx_tim1.c<br>generated/hal/mx_tim1.h | drv_pwm_init, drv_pwm_start, mx_tim1_init | — | implemented, untested |
| `IF-003` | The PWM requirement shall refer to duty cycle on a 3.3 V MCU logic output and shall not imply a variable 5 V supply rail. | inspection | The PWM interface documentation and hardware description identify PA8/D7 as a 3.3 V logic output rather than a firmware-controlled variable 5 V supply. (expected: The PA8/D7 output voltage domain is 3.3 V logic, and no firmware requirement or configuration controls a variable 5 V supply rail.) | README.md<br>application/board.h<br>drivers/inc/drv_pwm.h | drv_pwm_init | — | implemented, untested |
| `HW-001` | The system shall not drive high‑power loads directly from the MCU pin; an external driver stage shall be used for loads exceeding GPIO electrical limits. | inspection | The load-interface design places an external driver stage between PA8 and every load whose requirements exceed the MCU GPIO electrical limits. (expected: No load exceeding the GPIO electrical limits is connected directly to PA8; each such load is driven through an external driver stage.) | README.md<br>application/board.h<br>drivers/inc/drv_pwm.h | drv_pwm_init | — | implemented, untested |
| `PERF-001` | The PWM carrier frequency shall remain constant while the duty cycle changes and throughout the ramp. | hardware / instrument | PWM frequency measurements taken at multiple points during both increasing and decreasing duty-cycle ramps are unchanged while the duty cycle varies. (expected: The measured carrier frequency is the same at every sampled point during both ramps; only the duty cycle changes.) | application/app_config.h<br>drivers/src/drv_pwm.c<br>generated/hal/mx_tim1.c | drv_pwm_set_carrier_hz, drv_pwm_set_duty_permille, mx_tim1_init | — | implemented, untested |
| `PERF-002` | By default, the PWM frequency shall be 1 kHz unless an application configuration overrides it. | hardware / instrument | With no application-level PWM-frequency override, the measured carrier frequency is 1 kHz. (expected: The measured PWM frequency is 1000 Hz.) | application/app_config.h<br>drivers/src/drv_pwm.c<br>generated/hal/mx_tim1.c | drv_pwm_set_carrier_hz, drv_pwm_set_duty_permille, mx_tim1_init | — | implemented, untested |
| `IF-004` | The firmware shall not reconfigure PA8 or its alternate-function selection while PWM generation is active; the pin shall remain bound to the selected timer channel throughout operation. | inspection | The implementation contains no runtime path that changes PA8's GPIO mode or alternate-function selection after PWM generation becomes active. (expected: PA8 is configured and bound to the selected timer channel before activation, and no active-operation path reconfigures the pin or changes its timer-channel binding.) | application/board.h<br>drivers/src/drv_pwm.c<br>generated/hal/mx_tim1.c<br>generated/hal/mx_tim1.h | drv_pwm_init, drv_pwm_start, mx_tim1_init | — | implemented, untested |
| `PERF-008` | Duty-cycle updates shall not introduce extra PWM edges; during active operation each carrier period shall contain no more than one rising and one falling transition, except at 0% or 100% where the output is static. | hardware / instrument | During duty-cycle updates, each carrier period contains no more than one rising transition and one falling transition, with no additional narrow pulses. (expected: Each non-endpoint carrier period contains at most one rising edge and one falling edge, with no extra transitions; at 0% or 100% duty cycle the output is static.) | application/app_config.h<br>drivers/src/drv_pwm.c<br>generated/hal/mx_tim1.c | drv_pwm_set_carrier_hz, drv_pwm_set_duty_permille, mx_tim1_init | — | implemented, untested |

Observed on the board:
- `IF-001`, `IF-002`, `IF-004` — After fresh flash and reset, LD1 was steady ON, indicating main() reached its normal running loop. No oscilloscope or logic-analyzer waveform was captured, so PA8/D7 electrical behavior remains unmeasured.
- `PERF-001`, `PERF-002`, `PERF-008` — Fresh deployment reached the normal running loop with LD1 steady ON; no instrument capture was available to verify carrier frequency, ramp invariance, or edge behavior.

Notes recorded with these traces:
- `IF-003`, `HW-001` — Documentation identifies PA8/D7 as a 3.3 V logic output and requires an external driver for loads beyond MCU GPIO limits.
- `PERF-001`, `PERF-002`, `PERF-008` — Generated TIM1 setup defaults to 1 kHz with compare preload enabled; duty updates write only the compare pulse and preserve the period.

## PWM Ramp State Machine & Runtime Execution

gate: deferred · tasks: 3/3 done · recorded in: 4edac7c9-bc3f-4444-b99c-5d52acd7e63e

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The firmware shall start PWM generation automatically after board initialization. | hardware / instrument | After board initialization, PA8/D7 produces a PWM waveform without any user-triggered action. (expected: A PWM waveform is present on D7/PA8 automatically after initialization.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-002` | The firmware shall begin each cycle at 0% duty cycle. | hardware / instrument | At each detected cycle-start transition, the measured PWM output is low for the complete first PWM period, representing 0% duty cycle. (expected: The measured duty cycle is 0% at every cycle start.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-003` | The firmware shall repeat the complete 10‑second up/down PWM duty‑cycle cycle continuously until reset or power‑off. | hardware / instrument | After one 10-second cycle completes, a new cycle starts automatically and the same up/down sequence repeats for each subsequently observed cycle until reset or power-off. (expected: At least three consecutive cycles are present; each cycle lasts 10 seconds, and another cycle begins automatically after the preceding cycle ends.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-004` | After any reset, the firmware shall restart the PWM sequence from the defined initial state. | hardware / instrument | Following a system reset, the first post-reset PWM cycle starts at 0% duty and proceeds into the repeating ramp sequence. (expected: The first post-reset cycle starts at 0% duty, then the sequence repeats with the defined up/down ramp.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-005` | During the up‑ramp, the duty cycle shall increase monotonically from 0% to 100%. | hardware / instrument | Every successive duty-cycle update in an up-ramp is greater than or equal to the preceding update, and the up-ramp terminates at 100%. (expected: For every adjacent pair, duty[n+1] >= duty[n], and the final up-ramp duty is 100%.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-006` | During the down‑ramp, the duty cycle shall decrease monotonically from 100% to 0%. | hardware / instrument | Every successive duty-cycle update in a down-ramp is less than or equal to the preceding update, and the down-ramp terminates at 0%. (expected: For every adjacent pair, duty[n+1] <= duty[n], and the final down-ramp duty is 0%.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-007` | The firmware shall hold PA8 low (0% duty) whenever PWM generation is inactive, including during initialization and after any reset, until the PWM timer is configured and the initial 0% state is established. | hardware / instrument | PA8 remains continuously low from reset or power-on until PWM is configured and the initial 0% state is established, with no pre-start pulses. (expected: PA8 is a stable low level with no pulses before PWM starts, and the first active PWM state represents 0% duty.)<br>If PWM timer configuration or start is forced to fail, PA8 remains low and no PWM waveform is emitted before a later successful initialization or reset. (expected: PA8 remains low and no PWM generation occurs during the failed initialization attempt.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `FR-008` | The firmware shall clamp computed duty-cycle values to the valid range inclusive of the endpoints so the timer compare value never underflows below 0 or exceeds the auto-reload period, ensuring realizable 0% and 100% duty cycles. | inspection | The duty-to-compare conversion constrains every computed timer compare value to the inclusive interval from 0 through ARR before writing TIM1_CH1 CCR. (expected: For every input duty value, the value written to CCR satisfies 0 <= CCR <= ARR.)<br>The endpoint conversion maps below-range and above-range duty inputs to saturated compare values rather than wrapped values. (expected: Inputs at or below 0% produce CCR=0, inputs at or above 100% produce CCR=ARR, and no boundary input produces a value outside [0, ARR].) | drivers/inc/drv_pwm.h<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, duty_to_pulse | — | implemented, untested |
| `FR-009` | The firmware shall reverse ramp direction immediately at the duty‑cycle endpoints with no intentional hold time: upon reaching 100% the next update shall decrease duty cycle, and upon reaching 0% the next update shall increase duty cycle. | hardware / instrument | When the up-ramp reaches 100%, the immediately following duty-cycle update is lower, and when the down-ramp reaches 0%, the immediately following update is higher. (expected: At 100%, the next update decreases duty; at 0%, the next update increases duty; neither endpoint is repeated for more than one update interval.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |
| `NFR-001` | The firmware shall avoid blocking delays that prevent normal firmware execution while updating the PWM duty cycle. | on-target test | A heartbeat or other enabled periodic firmware activity continues to execute while the PWM ramp is active. (expected: Heartbeat events continue to occur throughout the PWM ramp; the counter or event timestamps show no interval in which PWM updating prevents the enabled activity from executing.) | application/main.c<br>application/ramp_control.c<br>application/ramp_control.h<br>generated/hal/mx_system.c | SysTick_Handler, main, ramp_control_init, ramp_control_on_tick, ramp_control_update | — | implemented, untested |

Observed on the board:
- `FR-001`, `FR-002`, `FR-003`, `FR-004`, `FR-005`, `FR-006`, `FR-007`, `FR-009`, `NFR-001` — Fresh deployment completed and LD1 was steady ON after reset, indicating main() reached its normal running loop. No oscilloscope or logic-analyzer capture was available, so automatic D7 waveform, endpoints, repetition, reset startup, and monotonic ramp behavior were not measured.

Notes recorded with these traces:
- `FR-008` — The driver rejects inputs above 1000 permille and maps 0% to pulse 0 and 100% to the timer resolution; intermediate values are rounded and remain within the inclusive timer range.

## PWM Ramp Timing & Profile Configuration

gate: deferred · tasks: 3/3 done · recorded in: 4edac7c9-bc3f-4444-b99c-5d52acd7e63e

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `PERF-003` | The up‑ramp duration from 0% to 100% duty cycle shall be 5.0 seconds. | hardware / instrument | The elapsed time from the observed start of the 0% to 100% up-ramp until the duty cycle reaches 100% is 5.0 seconds. (expected: Measured elapsed time is 5.0 seconds.) | application/app_config.h<br>application/ramp_control.c<br>config/app_config.c<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, ramp_control_calculate_duty, ramp_control_update | — | implemented, untested |
| `PERF-004` | The down‑ramp duration from 100% to 0% duty cycle shall be 5.0 seconds. | hardware / instrument | The elapsed time from the observed start of the 100% to 0% down-ramp until the duty cycle reaches 0% is 5.0 seconds. (expected: Measured elapsed time is 5.0 seconds.) | application/app_config.h<br>application/ramp_control.c<br>config/app_config.c<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, ramp_control_calculate_duty, ramp_control_update | — | implemented, untested |
| `PERF-005` | The complete up‑and‑down PWM duty‑cycle cycle duration shall be 10.0 seconds. | hardware / instrument | The elapsed time from the beginning of a 0% to 100% to 0% sequence until the duty cycle returns to 0% is 10.0 seconds. (expected: Measured complete-cycle duration is 10.0 seconds, and the cycle ends at 0% duty cycle.) | application/app_config.h<br>application/ramp_control.c<br>config/app_config.c<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, ramp_control_calculate_duty, ramp_control_update | — | implemented, untested |
| `PERF-006` | The PWM duty‑cycle ramp shape shall be linear. | hardware / instrument | A duty-cycle-versus-time plot for each ramp shows a linear relationship over the 5.0-second ramp interval. (expected: Both 5.0-second ramp plots verify a linear duty-cycle-versus-time relationship within the measurement method used.) | application/app_config.h<br>application/ramp_control.c<br>config/app_config.c<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, ramp_control_calculate_duty, ramp_control_update | — | implemented, untested |
| `PERF-007` | The PWM duty‑cycle endpoints shall be 0% and 100%. | hardware / instrument | Each running cycle produces an observed 0% low endpoint and an observed 100% high endpoint on Arduino D7. (expected: The minimum endpoint is 0% and the maximum endpoint is 100% in each observed cycle.) | application/app_config.h<br>application/ramp_control.c<br>config/app_config.c<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, ramp_control_calculate_duty, ramp_control_update | — | implemented, untested |
| `NFR-002` | The firmware should use sufficiently small duty‑cycle increments to create a smooth ramp. | hardware / instrument | At the default 1 kHz carrier frequency, the observed duty-cycle ramp has no perceptible discontinuities when viewed on the oscilloscope or LED driver. (expected: The ramp presents as smooth, with no perceptible discontinuities at the selected carrier frequency.) | application/app_config.h<br>application/ramp_control.c<br>config/app_config.c<br>drivers/src/drv_pwm.c | drv_pwm_set_duty_permille, ramp_control_calculate_duty, ramp_control_update | — | implemented, untested |
| `NFR-003` | Duty-cycle updates during each ramp shall be paced by a single monotonic timebase with a fixed update interval for the duration of the ramp. | on-target test | Consecutive duty-cycle update timestamps collected during a ramp are strictly increasing and have a constant interval equal to the configured update period. (expected: Every timestamp is strictly greater than its predecessor, and every consecutive interval equals the configured update period for the entire ramp.)<br>The target-side update trace contains no missing duty-cycle updates or blocking-induced gaps during a ramp. (expected: The update sequence is continuous for the full ramp, with no missing updates and no blocking-induced interval exceeding the configured update interval.) | application/ramp_control.c<br>config/app_config.c<br>generated/hal/mx_system.c | SysTick_Handler, ramp_control_update | — | implemented, untested |

Notes recorded with these traces:
- `PERF-003`, `PERF-004`, `PERF-005`, `PERF-006`, `PERF-007`, `NFR-002` — Configuration is 5000 ms per ramp leg, yielding a 10 s triangle cycle; the duty calculation is linear in elapsed milliseconds and clamps the inclusive 0..1000 permille endpoints. No oscilloscope/logic-analyzer measurement was available.
- `NFR-003` — The single HAL millisecond tick drives a fixed 10 ms update interval and nonblocking main-loop updates. Target-side timestamp trace was not available, so timing remains source/config evidence only.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### NOT unit-testable (25) — do not write host tests for these

No `host` criterion, so no unit test can cover them however it is written. Listed explicitly because the untested count above otherwise reads as a backlog of unit tests owed -- and closing one of these with a host test means inventing an assertion that passes without checking the thing. Each line says how it IS checked instead.

- `IF-001` — checked by hardware / instrument · _PWM Peripheral, Pin & Electrical Interface_
- `IF-002` — checked by inspection · _PWM Peripheral, Pin & Electrical Interface_
- `IF-003` — checked by inspection · _PWM Peripheral, Pin & Electrical Interface_
- `HW-001` — checked by inspection · _PWM Peripheral, Pin & Electrical Interface_
- `PERF-001` — checked by hardware / instrument · _PWM Peripheral, Pin & Electrical Interface_
- `PERF-002` — checked by hardware / instrument · _PWM Peripheral, Pin & Electrical Interface_
- `IF-004` — checked by inspection · _PWM Peripheral, Pin & Electrical Interface_
- `PERF-008` — checked by hardware / instrument · _PWM Peripheral, Pin & Electrical Interface_
- `FR-001` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-002` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-003` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-004` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-005` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-006` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-007` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `FR-008` — checked by inspection · _PWM Ramp State Machine & Runtime Execution_
- `FR-009` — checked by hardware / instrument · _PWM Ramp State Machine & Runtime Execution_
- `NFR-001` — checked by on-target test · _PWM Ramp State Machine & Runtime Execution_
- `PERF-003` — checked by hardware / instrument · _PWM Ramp Timing & Profile Configuration_
- `PERF-004` — checked by hardware / instrument · _PWM Ramp Timing & Profile Configuration_
- `PERF-005` — checked by hardware / instrument · _PWM Ramp Timing & Profile Configuration_
- `PERF-006` — checked by hardware / instrument · _PWM Ramp Timing & Profile Configuration_
- `PERF-007` — checked by hardware / instrument · _PWM Ramp Timing & Profile Configuration_
- `NFR-002` — checked by hardware / instrument · _PWM Ramp Timing & Profile Configuration_
- `NFR-003` — checked by on-target test · _PWM Ramp Timing & Profile Configuration_

### Board checks owed (7)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `PERF-003` (PWM Ramp Timing & Profile Configuration)
- `PERF-004` (PWM Ramp Timing & Profile Configuration)
- `PERF-005` (PWM Ramp Timing & Profile Configuration)
- `PERF-006` (PWM Ramp Timing & Profile Configuration)
- `PERF-007` (PWM Ramp Timing & Profile Configuration)
- `NFR-002` (PWM Ramp Timing & Profile Configuration)
- `NFR-003` (PWM Ramp Timing & Profile Configuration)

### Implemented, never tested (25)

Code exists and no test is recorded against it.

- `IF-001` (PWM Peripheral, Pin & Electrical Interface)
- `IF-002` (PWM Peripheral, Pin & Electrical Interface)
- `IF-003` (PWM Peripheral, Pin & Electrical Interface)
- `HW-001` (PWM Peripheral, Pin & Electrical Interface)
- `PERF-001` (PWM Peripheral, Pin & Electrical Interface)
- `PERF-002` (PWM Peripheral, Pin & Electrical Interface)
- `IF-004` (PWM Peripheral, Pin & Electrical Interface)
- `PERF-008` (PWM Peripheral, Pin & Electrical Interface)
- `FR-001` (PWM Ramp State Machine & Runtime Execution)
- `FR-002` (PWM Ramp State Machine & Runtime Execution)
- `FR-003` (PWM Ramp State Machine & Runtime Execution)
- `FR-004` (PWM Ramp State Machine & Runtime Execution)
- `FR-005` (PWM Ramp State Machine & Runtime Execution)
- `FR-006` (PWM Ramp State Machine & Runtime Execution)
- `FR-007` (PWM Ramp State Machine & Runtime Execution)
- `FR-008` (PWM Ramp State Machine & Runtime Execution)
- `FR-009` (PWM Ramp State Machine & Runtime Execution)
- `NFR-001` (PWM Ramp State Machine & Runtime Execution)
- `PERF-003` (PWM Ramp Timing & Profile Configuration)
- `PERF-004` (PWM Ramp Timing & Profile Configuration)
- `PERF-005` (PWM Ramp Timing & Profile Configuration)
- `PERF-006` (PWM Ramp Timing & Profile Configuration)
- `PERF-007` (PWM Ramp Timing & Profile Configuration)
- `NFR-002` (PWM Ramp Timing & Profile Configuration)
- `NFR-003` (PWM Ramp Timing & Profile Configuration)
