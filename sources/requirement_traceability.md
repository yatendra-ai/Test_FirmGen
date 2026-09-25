# Requirement traceability

Generated 2026-09-25T07:12:24.975545700+00:00
Specification frozen 2026-09-25T06:55:02.634Z

24 of 24 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 24 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project. Host tests run on a separate machine; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## PWM Timer and PA8 Output Driver

gate: deferred · tasks: 5/5 done · recorded in: 12d24b74-2437-45cf-bd3c-cf3b84961d11

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The firmware shall start PWM generation automatically after board initialization. | hardware / instrument | After power-up or reset, PWM generation is present on Arduino D7/PA8 without any user interaction. (expected: A periodic PWM waveform is present on D7/PA8 without user interaction.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `FR-002` | The firmware shall output a PWM signal on the Arduino D7 pin (CN9 pin 8 / MCU pin PA8). | hardware / instrument | A periodic PWM waveform is measurable at the electrically connected Arduino D7, CN9 pin 8, and MCU PA8 signal. (expected: A periodic PWM waveform is measurable at D7/CN9 pin 8, corresponding to PA8.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `IF-001` | PA8 shall be configured in a supported timer alternate-function PWM output mode. | inspection | The GPIO configuration assigns PA8 to the timer channel alternate function used by the PWM driver. (expected: PA8 is configured for the supported timer PWM alternate function, and the selected timer channel is the channel driven by the PWM implementation.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `FR-009` | The PWM carrier frequency shall remain constant while the duty cycle changes. | hardware / instrument | The measured PWM frequency is unchanged while the commanded duty cycle is ramped through its supported values. (expected: Every duty-cycle measurement reports the same carrier frequency as the initial measurement; no frequency change is observed during the ramp.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `FR-010` | The default PWM carrier frequency shall be 1 kHz. | hardware / instrument | The startup PWM carrier frequency with the default configuration measures 1000 Hz. (expected: The measured carrier frequency is approximately 1000 Hz.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `IF-002` | The PWM output shall be interpreted as duty-cycle modulation of a 3.3 V MCU logic output on D7, not as a variable 5 V supply rail. | inspection | The PA8/D7 output is configured and documented as a 3.3 V logic GPIO signal rather than a variable 5 V supply rail. (expected: The reviewed interface identifies D7/PA8 as a 3.3 V logic output and does not specify it as a variable 5 V supply rail.)<br>The external test connection requirements limit loads connected directly to D7/PA8 to the MCU GPIO electrical limits. (expected: All direct test loads are within the MCU GPIO electrical limits, and no high-power load is connected directly to D7/PA8.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `IF-003` | The D7/PA8 output shall remain in a defined inactive state with no pulses until PWM is fully configured and enabled, and the first active duty cycle shall be 0%. | hardware / instrument | During startup, D7/PA8 remains at one constant inactive logic level with no pulses until PWM is enabled. (expected: D7/PA8 remains at a single inactive logic level and contains no pulses before PWM enable.)<br>The first active PWM period after enable has a commanded duty cycle of 0%. (expected: The first active PWM period has 0% high-time, so the output remains low for the entire period.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `FR-014` | If PWM/timer or PA8 configuration fails during initialization, the firmware shall place PA8 in a defined inactive state and shall not toggle the output. | inspection | Every detected PWM/timer or PA8 configuration error routes initialization to an error path that leaves PA8 in the defined inactive state and does not enable PWM. (expected: On every configuration failure path, PA8 is assigned the defined inactive state, PWM is not enabled, and the failure is reported through the available build or test log mechanism.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `IF-004` | PWM duty-cycle percentage shall be defined as the proportion of time that D7/PA8 is at logic-high level within each PWM period. | hardware / instrument | At a commanded 50% duty cycle, the measured D7/PA8 high-time is approximately half of one PWM period. (expected: The high-time is approximately 50% of the measured PWM period.)<br>At a commanded 0% duty cycle, D7/PA8 remains continuously low, and at a commanded 100% duty cycle it remains continuously high. (expected: The output is continuously low at 0% duty and continuously high at 100% duty, with no transitions during either interval.) | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `IF-005` | The firmware shall provide a mechanism to override the default 1 kHz PWM carrier frequency. | not verifiable | The PWM carrier-frequency override cannot be tested because the requirement does not identify the override mechanism or its supported frequency range. | NUCLEO-C5A3ZG/generated/hal/mx_tim1.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim1.h<br>NUCLEO-C5A3ZG/generated/hal/stm32c5xx_hal_conf.h<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, b_pwm_pa8_configure_gpio, mx_tim1_deinit, mx_tim1_init, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |

Observed on the board:
- `FR-001`, `FR-002`, `IF-001`, `FR-009`, `FR-010`, `IF-002`, `IF-003`, `FR-014`, `IF-004`, `IF-005` — Fresh build flashed successfully before the ramp changes. After reset, the ST-Link VCP monitor printed: 'I app: TIM1 CH1 PWM started on PA8 at 1000 Hz'. This confirms the firmware reached the PWM startup path; the physical PA8 waveform and inactive-state level were not electrically measured.

Notes recorded with these traces:
- `FR-001`, `FR-002`, `IF-001`, `FR-009`, `FR-010`, `IF-002`, `IF-003`, `FR-014`, `IF-004`, `IF-005` — Re-recorded after category 3 changed only the app_start scheduling loop; the carrier setup and startup behavior remain represented by the listed symbols. Category gate remains awaiting the user's review/skip choice.

## PWM Ramp Control Logic

gate: deferred · tasks: 3/3 done · recorded in: 12d24b74-2437-45cf-bd3c-cf3b84961d11

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-003` | Each PWM ramp cycle shall begin at 0% duty cycle. | hardware / instrument | At the start of each 10-second PWM cycle, the measured duty cycle on D7/PA8 is approximately 0%. (expected: Approximately 0% at every cycle start.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-004` | The duty cycle shall ramp monotonically from 0% to 100% over 5.0 seconds. | hardware / instrument | The measured duty cycle rises monotonically from approximately 0% to approximately 100% and reaches approximately 100% at about 5.0 seconds. (expected: Duty-cycle samples never decrease during the rising ramp, and the duty cycle reaches approximately 100% at about 5.0 s.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-005` | The duty cycle shall ramp monotonically from 100% to 0% over the following 5.0 seconds. | hardware / instrument | The measured duty cycle falls monotonically from approximately 100% to approximately 0% over about 5.0 seconds and is approximately 0% at about 10.0 seconds from cycle start. (expected: Duty-cycle samples never increase during the falling ramp, the fall lasts about 5.0 s, and the endpoint is approximately 0% at about 10.0 s from cycle start.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-006` | A complete rise-and-fall PWM duty-cycle cycle shall be 10.0 seconds in duration. | hardware / instrument | The measured time from approximately 0% at one cycle start to approximately 0% at the next cycle end is approximately 10.0 seconds. (expected: Each complete rise-and-fall cycle measures approximately 10.0 s.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-007` | The 10-second PWM duty-cycle ramp cycle shall repeat continuously until reset or power-off. | hardware / instrument | The PWM ramp produces multiple consecutive 10-second cycles without terminating before a reset or power-off event. (expected: At least three consecutive cycles are present with no termination before the reset or power-off event.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-008` | After reset, the firmware shall restart the PWM sequence from the defined initial state (0% duty cycle). | hardware / instrument | After a system reset, the PWM output returns to approximately 0% at the reset-start of the sequence and resumes the 10-second ramp. (expected: The post-reset initial duty cycle is approximately 0%, followed by the defined 10-second ramp sequence.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-011` | The PWM duty-cycle ramp shall be linear. | hardware / instrument | During both ramp directions, sampled duty-cycle values are proportional to elapsed ramp time, including approximately 50% duty at 2.5 seconds into the rising ramp and 7.5 seconds from cycle start. (expected: The rising and falling samples follow proportional linear progress, with approximately 50% duty at both t = 2.5 s and t = 7.5 s.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-012` | The duty-cycle endpoints shall be 0% and 100%. | hardware / instrument | The measured PWM duty-cycle trace reaches an approximately 0% endpoint at the start and end of each cycle. (expected: The start and end endpoints are approximately 0%.)<br>The measured PWM duty-cycle trace reaches an approximately 100% endpoint at the end of each rising ramp. (expected: The rising-ramp maximum is approximately 100% in every captured cycle.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-013` | The firmware shall saturate the computed PWM duty cycle to the inclusive range 0% to 100% before applying it to the output. | hardware / instrument | Across all PWM updates, the measured output duty cycle remains within the inclusive range from 0% to 100%. (expected: Minimum duty cycle is at least 0% and maximum duty cycle is at most 100%.)<br>An injected duty-cycle command below 0% produces 0% output and an injected command above 100% produces 100% output. (expected: The below-range command produces 0% output and the above-range command produces 100% output.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |
| `FR-015` | At both duty-cycle endpoints (0% and 100%), the firmware shall reverse the ramp direction immediately with no intentional hold time. | hardware / instrument | After the PWM reaches approximately 100%, the next duty-cycle update is decreasing rather than remaining at a plateau. (expected: The immediately following update is lower than the endpoint value, with no intentional plateau beyond the update interval.)<br>After the PWM reaches approximately 0%, the next duty-cycle update is increasing rather than remaining at a plateau. (expected: The immediately following update is higher than the endpoint value, with no intentional plateau beyond the update interval.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_delay_ms, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent, pwm_ramp_reset | — | implemented, untested |

Notes recorded with these traces:
- `FR-003`, `FR-004`, `FR-005`, `FR-006`, `FR-007`, `FR-008`, `FR-011`, `FR-012`, `FR-013`, `FR-015` — Re-recorded after category 3 changed only the scheduling loop in app_start: ramp calculation and PWM update behavior remain represented by the listed symbols. Clean build completed; no category-2 hardware sweep or host test has been performed.

## Nonblocking Waveform Update Execution

gate: deferred · tasks: 2/2 done · recorded in: 12d24b74-2437-45cf-bd3c-cf3b84961d11

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `PERF-001` | The firmware shall avoid blocking delays that prevent normal firmware execution during duty-cycle updates. | inspection | The duty-cycle update path contains no blocking delay call or loop that waits for elapsed time before returning. (expected: No blocking delay API or time-waiting polling loop is present in the duty-cycle update path.)<br>Duty-cycle updates are scheduled by a timer, system tick, interrupt-driven callback, or equivalent mechanism that returns control to normal firmware execution between updates. (expected: The update path is driven by a timer, tick, interrupt callback, or equivalent non-blocking mechanism, and each update returns control before the next scheduled update.) | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent | — | implemented, untested |
| `PERF-002` | Duty-cycle increment steps shall be sufficiently small to produce a smooth ramp without perceptible jumps or long pauses. | hardware / instrument, not verifiable | The measured PWM duty cycle is approximately 50% at 2.5 s after the ramp starts. (expected: The measured duty cycle is approximately 50%, subject to the project's measurement tolerance.)<br>The measured PWM duty cycle is approximately 50% at 7.5 s after the ramp starts. (expected: The measured duty cycle is approximately 50%, subject to the project's measurement tolerance.)<br>The ramp waveform has no unexpected duty-cycle jumps or long pauses. | firmware/app/app.c<br>firmware/modules/pwm_ramp.c<br>firmware/modules/pwm_ramp.h<br>firmware/osal/osal.h<br>firmware/platforms/stm32/osal_stm32.c<br>firmware/platforms/stm32/pwm_pa8.c<br>firmware/platforms/stm32/pwm_pa8.h | app_start, osal_uptime_ms, pwm_pa8_set_duty_percent, pwm_ramp_duty_percent | — | implemented, untested |

Notes recorded with these traces:
- `PERF-001`, `PERF-002` — Clean build completed. The app uses elapsed millisecond timestamps and updates duty at most once per millisecond without HAL_Delay or a multi-second blocking wait; PWM compare updates are small and monotonic at the timer update cadence. No hardware sweep or host test has been performed.

## External Load and USB Development Interface

gate: deferred · tasks: 2/2 done · recorded in: 12d24b74-2437-45cf-bd3c-cf3b84961d11

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-001` | High-power loads shall not be connected directly to the MCU pin; an external driver stage shall be used when controlling loads that exceed GPIO electrical limits. | inspection | The design and test setup show an external driver stage between every load exceeding the applicable MCU GPIO electrical limit and the MCU pin, with no direct high-power-load connection to the pin. (expected: For every load exceeding a GPIO limit, an external driver stage is present in the signal path; zero such loads are connected directly to Arduino D7, PA8, or another MCU GPIO.) | NUCLEO-C5A3ZG/cmake/CMakePresets.json<br>application/main.c<br>firmware/app/app.c<br>firmware/docs/overview.md<br>firmware/platforms/stm32/pwm_pa8.c | main, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |
| `HW-002` | A USB connection shall be used for board power, programming, and debugging during development and verification. | hardware / instrument | During development and verification, the board receives power through USB and the programming/debug connection is established through USB. (expected: The board is powered by USB, and both programming and debugging are performed through the USB connection during the test run.) | NUCLEO-C5A3ZG/cmake/CMakePresets.json<br>application/main.c<br>firmware/app/app.c<br>firmware/docs/overview.md<br>firmware/platforms/stm32/pwm_pa8.c | main, pwm_pa8_set_duty_percent, pwm_pa8_start | — | implemented, untested |

Observed on the board:
- `HW-001`, `HW-002` — Fresh build flashed successfully over the board's ST-LINK USB connection. The VCP monitor on reset printed the expected startup markers, including 'I app: TIM1 CH1 PWM started on PA8 at 1000 Hz'. USB power/programming/debug path was exercised by the successful flash and monitor capture; no external high-power load was connected or measured.

Notes recorded with these traces:
- `HW-001`, `HW-002` — External driver-stage safety remains a documented design constraint and was not electrically tested with a load.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### NOT unit-testable (24) — do not write host tests for these

No `host` criterion, so no unit test can cover them however it is written. Listed explicitly because the untested count above otherwise reads as a backlog of unit tests owed -- and closing one of these with a host test means inventing an assertion that passes without checking the thing. Each line says how it IS checked instead.

- `FR-001` — checked by hardware / instrument · _PWM Timer and PA8 Output Driver_
- `FR-002` — checked by hardware / instrument · _PWM Timer and PA8 Output Driver_
- `IF-001` — checked by inspection · _PWM Timer and PA8 Output Driver_
- `FR-009` — checked by hardware / instrument · _PWM Timer and PA8 Output Driver_
- `FR-010` — checked by hardware / instrument · _PWM Timer and PA8 Output Driver_
- `IF-002` — checked by inspection · _PWM Timer and PA8 Output Driver_
- `IF-003` — checked by hardware / instrument · _PWM Timer and PA8 Output Driver_
- `FR-014` — checked by inspection · _PWM Timer and PA8 Output Driver_
- `IF-004` — checked by hardware / instrument · _PWM Timer and PA8 Output Driver_
- `IF-005` — checked by not verifiable · _PWM Timer and PA8 Output Driver_
- `FR-003` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-004` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-005` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-006` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-007` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-008` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-011` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-012` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-013` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `FR-015` — checked by hardware / instrument · _PWM Ramp Control Logic_
- `PERF-001` — checked by inspection · _Nonblocking Waveform Update Execution_
- `PERF-002` — checked by hardware / instrument, not verifiable · _Nonblocking Waveform Update Execution_
- `HW-001` — checked by inspection · _External Load and USB Development Interface_
- `HW-002` — checked by hardware / instrument · _External Load and USB Development Interface_

### Board checks owed (11)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `FR-003` (PWM Ramp Control Logic)
- `FR-004` (PWM Ramp Control Logic)
- `FR-005` (PWM Ramp Control Logic)
- `FR-006` (PWM Ramp Control Logic)
- `FR-007` (PWM Ramp Control Logic)
- `FR-008` (PWM Ramp Control Logic)
- `FR-011` (PWM Ramp Control Logic)
- `FR-012` (PWM Ramp Control Logic)
- `FR-013` (PWM Ramp Control Logic)
- `FR-015` (PWM Ramp Control Logic)
- `PERF-002` (Nonblocking Waveform Update Execution)

### Implemented, never tested (24)

Code exists and no test is recorded against it.

- `FR-001` (PWM Timer and PA8 Output Driver)
- `FR-002` (PWM Timer and PA8 Output Driver)
- `IF-001` (PWM Timer and PA8 Output Driver)
- `FR-009` (PWM Timer and PA8 Output Driver)
- `FR-010` (PWM Timer and PA8 Output Driver)
- `IF-002` (PWM Timer and PA8 Output Driver)
- `IF-003` (PWM Timer and PA8 Output Driver)
- `FR-014` (PWM Timer and PA8 Output Driver)
- `IF-004` (PWM Timer and PA8 Output Driver)
- `IF-005` (PWM Timer and PA8 Output Driver)
- `FR-003` (PWM Ramp Control Logic)
- `FR-004` (PWM Ramp Control Logic)
- `FR-005` (PWM Ramp Control Logic)
- `FR-006` (PWM Ramp Control Logic)
- `FR-007` (PWM Ramp Control Logic)
- `FR-008` (PWM Ramp Control Logic)
- `FR-011` (PWM Ramp Control Logic)
- `FR-012` (PWM Ramp Control Logic)
- `FR-013` (PWM Ramp Control Logic)
- `FR-015` (PWM Ramp Control Logic)
- `PERF-001` (Nonblocking Waveform Update Execution)
- `PERF-002` (Nonblocking Waveform Update Execution)
- `HW-001` (External Load and USB Development Interface)
- `HW-002` (External Load and USB Development Interface)

### No observable exists (2)

These need a human decision — either the requirement is restated as something checkable, or it is accepted as unverifiable on the record. Inventing a test for one of these produces a test that passes and proves nothing.

- `IF-005` — The PWM carrier-frequency override cannot be tested because the requirement does not identify the override mechanism or its supported frequency range. · _No named configuration/API/interface is provided for selecting the override frequency, and no supported override range or concrete non-default test value is specified._
- `PERF-002` — The ramp waveform has no unexpected duty-cycle jumps or long pauses. · _The acceptance statement does not define the maximum allowed duty-cycle step, pause duration, update interval, or what constitutes an unexpected jump, so no reproducible pass/fail threshold can be asserted._
