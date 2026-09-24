# Requirement traceability

Generated 2026-09-24T06:10:56.367969700+00:00
Specification frozen 2026-09-24T06:06:33.981Z

53 of 53 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 53 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project. Host tests run on a separate machine; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## Target Hardware & Demonstration Build

gate: deferred · tasks: 4/4 done · recorded in: dae6d731-2131-4826-a713-9f4207a449a4

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `NFR-001` | The delivered firmware shall run on the NUCLEO-G431RB with the X-NUCLEO-IHM16M1 and automatically demonstrate basic rotation of the GBM2804H-100T three-phase gimbal motor after power-up. | hardware / instrument | After power-cycling the assembled target, the GBM2804H-100T shaft begins rotating without a button press, command, or other manual intervention. (expected: The motor begins the rotation demonstration automatically after power-up.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/motor_platform.c | app_motor_step, app_start, motor_platform_drive | — | implemented, untested |
| `NFR-003` | The firmware shall build successfully and be flashable to the target hardware. | inspection, hardware / instrument | The project build completes and produces the firmware image without build errors. (expected: The build exits successfully with no errors and produces a flashable firmware image.)<br>The generated firmware image programs the NUCLEO-G431RB successfully and executes after a target reset. (expected: The programming tool reports successful programming, and the target executes the firmware after reset.) | CMakeLists.txt<br>firmware/app/app.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_init | — | implemented, untested |
| `HW-001` | The target MCU board for the firmware shall be the NUCLEO-G431RB. | hardware / instrument | The demonstrated firmware execution occurs on a NUCLEO-G431RB board. (expected: The board is identified as NUCLEO-G431RB and the firmware runs on it after reset.) | CMakeLists.txt<br>Core/Startup/startup_stm32g431rbtx.s<br>firmware/configs/app_config.h | main | — | implemented, untested |
| `HW-002` | The motor driver board used shall be the X-NUCLEO-IHM16M1 (STSPIN830). | inspection | The hardware assembly and firmware configuration identify the X-NUCLEO-IHM16M1/STSPIN830 as the motor driver and use its PWM, GPIO, and EN/FAULT interfaces with settings matching the driver's documented default configuration. (expected: The board is an X-NUCLEO-IHM16M1, the firmware uses the STSPIN830 PWM/GPIO/EN/FAULT interfaces, and all inspected settings match the documented defaults.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1/x_nucleo_ihm16m1.c<br>firmware/platforms/stm32/motor_platform.c | motor_platform_enable, motor_platform_init, x_nucleo_ihm16m1_init | — | implemented, untested |
| `HW-003` | The motor used for the demonstration shall be the GBM2804H-100T with 7 pole pairs. | inspection | The motor connected to the U/V/W outputs is identified as GBM2804H-100T and its motor specification states seven pole pairs. (expected: The motor label is GBM2804H-100T and the specified pole-pair count is 7 pairs.) | firmware/README.md<br>firmware/configs/app_config.h | — | — | implemented, untested |
| `HW-004` | The test setup shall provide a 12 V DC supply. | inspection, not verifiable | The DC voltage measured at the motor-driver input during the test is 12 V. (expected: The measured input is 12 V DC.)<br>The firmware does not attempt to start the motor sequence when the supply is outside the permitted specification. | firmware/README.md<br>firmware/configs/app_config.h | — | — | implemented, untested |

Notes recorded with these traces:
- `NFR-001` — Automatic startup path is implemented: 3-second standby delay, fault check, driver enable, and repeating U/V/W commutation pattern. Shaft rotation and a reliable runtime marker have not been observed on hardware.
- `NFR-003` — Clean STM32 build completed and produced motor_demo.elf, .hex, and .bin. Initial deployment programmed successfully; first run hit HardFault in the generated 170 MHz clock setup. Clock was changed to a conservative 80 MHz setup; subsequent debug attach did not reproduce the HardFault, but monitor still received no UART bytes.
- `HW-001` — Detected ST-LINK reports Board Name NUCLEO-G431RB and Device STM32G43x/G44x, but the automatic run has not yet been physically demonstrated.
- `HW-002` — UM2415 Rev. 4 documented STSPIN830 default MODE-low, active-low STBY and EN/FAULT, and three-input interface are represented in configuration and adapter code.
- `HW-003` — GBM2804H-100T and seven pole pairs are recorded as application configuration and setup documentation.
- `HW-004` — The intended 12 V DC test supply is recorded; actual voltage measurement is not available in source inspection. The derived out-of-range guard criterion remains not verifiable because no permitted range or voltage-validity interface was specified.

## PWM Timer & Motor-Driver Interface

gate: deferred · tasks: 3/3 done · recorded in: dae6d731-2131-4826-a713-9f4207a449a4

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-001` | The firmware shall configure TIM1 to generate three-phase PWM. | hardware / instrument, host test | When TIM1 is enabled, three PWM waveforms are simultaneously observable on the U, V, and W output pins with 120° phase separation between adjacent phases. (expected: Three active PWM waveforms are present, with each phase separated from the next by 120° electrical phase.)<br>A failed TIM1 initialization leaves the motor-sequence controller in its non-running state and prevents any motor-start command from being issued. (expected: The sequence remains stopped or aborted, and the motor-start command log is empty.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/motor_platform.c | motor_platform_init | — | implemented, untested |
| `IF-002` | The firmware shall use the factory-default X-NUCLEO-IHM16M1 three-input configuration. | inspection | The driver interface configuration selects the X-NUCLEO-IHM16M1 factory-default three-PWM-input mode and contains no firmware or hardware configuration that selects an alternate input mode. (expected: The board remains in factory-default three-PWM-input mode, and no alternate-mode selection or reconfiguration is present.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1/x_nucleo_ihm16m1.c<br>firmware/platforms/stm32/motor_platform.c | motor_platform_pwm_mode, x_nucleo_ihm16m1_init | — | implemented, untested |
| `IF-003` | The firmware shall drive the U, V, and W three-phase motor outputs. | hardware / instrument | During the motor run window, three distinct GPIO signals are present at the STSPIN830 U, V, and W input pins. (expected: All three U, V, and W inputs show observable PWM activity, and the signals originate from three distinct GPIO outputs.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/motor_platform.c | motor_platform_drive, motor_platform_pwm_set | — | implemented, untested |
| `PERF-001` | The PWM switching frequency shall be 20 kHz. | hardware / instrument, not verifiable | The PWM carrier measured on each of the U, V, and W outputs has a nominal frequency of 20 kHz. (expected: Each phase measures 20 kHz nominally.)<br>The PWM-frequency pass/fail band around 20 kHz cannot be asserted until the required measurement tolerance is specified. | firmware/configs/app_config.h<br>firmware/platforms/stm32/motor_platform.c | motor_platform_init | — | implemented, untested |
| `FR-017` | The firmware shall complete TIM1 and PWM GPIO configuration with PWM outputs disabled before asserting the motor driver enable at the start of the sequence. | hardware / instrument, host test, not verifiable | Before the motor-driver EN signal rises, the U, V, and W outputs remain configured but show no PWM switching activity. (expected: Before EN rises, U, V, and W remain at their idle levels with no switching transitions.)<br>Immediately after EN rises and PWM is enabled, the first active PWM duty cycle on U, V, and W is 10%. (expected: The first active duty cycle is 10% on each of U, V, and W.)<br>The first active PWM duty cycle is followed by the startup duty-cycle ramp in the defined motor sequence without an EN assertion occurring before PWM/GPIO initialization completes. (expected: EN rises only after PWM/GPIO initialization completion, and subsequent duty-cycle changes match every specified startup-sequence timing and value.)<br>If the test injects an EN assertion before PWM/GPIO configuration completes, the startup controller deasserts EN, keeps PWM disabled, and aborts the sequence. (expected: EN is low, PWM remains disabled, and the sequence is in the aborted or stopped state.)<br>The complete startup ramp cannot be checked for exact duty-cycle values or timing until the referenced sequence ramp points are specified. | firmware/app/app.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_enable, motor_platform_init | — | implemented, untested |

Notes recorded with these traces:
- `IF-001`, `PERF-001` — TIM1 channels 1/2/3 are configured for three PWM outputs. With the 80 MHz system clock and APP_MOTOR_PWM_PERIOD_TICKS=4000, the configured carrier is 20 kHz by inspection; oscilloscope confirmation is still outstanding.
- `IF-002` — MODE is driven low and the portable driver is initialized with X_NUCLEO_IHM16M1_THREE_PWM_INPUTS, matching UM2415's documented factory-default MODE-low interface.
- `IF-003` — TIM1 CH1/CH2/CH3 are mapped to PA8/PA9/PA10 and drive U/V/W respectively; physical signal observation remains outstanding.
- `FR-017` — GPIO/timer setup and zero compare values occur before PWM start; driver initialization disables PWM and asserts STBY before the application delay and enable path.

## Open-Loop Sinusoidal Motor Control

gate: deferred · tasks: 5/5 done · recorded in: dae6d731-2131-4826-a713-9f4207a449a4

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The firmware shall generate a rotating three-phase electrical field using open-loop sinusoidal PWM. | hardware / instrument | The oscilloscope capture shall show three sinusoidal-modulated PWM phase waveforms on U, V, and W with 120° phase separation during the run window. (expected: Three sinusoidal-modulated PWM waveforms with 120° separation between adjacent phases.)<br>The measured three-phase electrical angle shall progress continuously in one rotating direction during the run window. (expected: The phase angle advances from cycle to cycle in a consistent rotating direction rather than remaining stationary or reversing.)<br>If rotating-field generation fails during the run, all motor PWM outputs shall be disabled and the sequence shall be aborted. (expected: PWM outputs are disabled and the sequence is in an aborted state.) | firmware/app/app.c<br>firmware/modules/sine_control/sine_control.c<br>firmware/modules/sine_control/sine_control.h | app_drive_sine, sine_control_output, sine_control_update | — | implemented, untested |
| `FR-003` | The firmware shall ramp PWM amplitude gradually to avoid an abrupt startup. | hardware / instrument | PWM duty shall increase monotonically from the initial amplitude to the maximum amplitude during the startup ramp, with no consecutive control-update change greater than 5 percentage points. (expected: Each duty value is greater than or equal to the preceding value, and every consecutive increase is ≤5 percentage points.)<br>PWM amplitude shall reach 20% by the end of the startup ramp at t=8 s. (expected: PWM duty is 20% at or before t=8 s.)<br>If the PWM amplitude ramp cannot be achieved, PWM shall be disabled and the driver shall enter standby. (expected: PWM is disabled and the driver is in standby.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `FR-004` | Between 3 s and 8 s from boot, the firmware shall ramp electrical frequency from 1 Hz to 10 Hz. | hardware / instrument | Measured electrical frequency shall increase monotonically from 1 Hz at t=3 s to 10 Hz by t=8 s. (expected: Frequency is 1 Hz at t=3 s, is nondecreasing at all sampled times, and reaches 10 Hz no later than t=8 s.)<br>If the electrical-frequency ramp cannot be achieved, PWM shall be disabled and the driver shall enter standby. (expected: PWM is disabled and the driver is in standby.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `FR-005` | Between 3 s and 8 s from boot, the firmware shall ramp PWM amplitude from 10% to 20%. | hardware / instrument | Measured PWM duty shall increase monotonically from 10% at t=3 s to 20% by t=8 s. (expected: Duty is 10% at t=3 s, never decreases during the interval, and reaches 20% no later than t=8 s.)<br>If the PWM amplitude ramp cannot be achieved, PWM shall be disabled and the driver shall enter standby. (expected: PWM is disabled and the driver is in standby.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `FR-007` | Between 8 s and 18 s from boot, the firmware shall hold the electrical frequency at 10 Hz. | hardware / instrument | Measured electrical frequency shall remain nominally 10 Hz for the entire hold window from t=8 s through t=18 s. (expected: 10 Hz throughout the hold window; the allowable measurement tolerance must be supplied by the requirements or test configuration.)<br>If the held electrical frequency deviates from its required value, the sequence shall enter the shutdown ramp at t=18 s. (expected: The shutdown-ramp state is entered at t=18 s.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `FR-008` | Between 8 s and 18 s from boot, the firmware shall limit PWM amplitude to a maximum of 20%. | hardware / instrument | Measured PWM duty shall not exceed 20% at any sampled time from t=8 s through t=18 s. (expected: Maximum PWM duty is ≤20%.)<br>If PWM duty exceeds 20% during the hold window, the firmware shall immediately start the shutdown ramp. (expected: The shutdown ramp starts immediately after the over-amplitude condition is detected.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `FR-018` | During the 3–8 s startup ramp, the firmware shall ramp electrical frequency smoothly from 1 Hz to 10 Hz with monotonic increase and with steps no greater than 1 Hz between consecutive control updates. | hardware / instrument | Electrical frequency shall be 1 Hz at t=3 s and shall reach 10 Hz by t=8 s while increasing monotonically during the startup ramp. (expected: Frequency is 1 Hz at t=3 s, every subsequent value is ≥ the preceding value, and frequency reaches 10 Hz no later than t=8 s.)<br>The electrical-frequency increase between consecutive control updates shall never exceed 1 Hz during t=3–8 s. (expected: Every positive step is ≤1 Hz; no negative step occurs.)<br>If the frequency ramp cannot be achieved, PWM shall be disabled and the driver shall return to standby. (expected: PWM is disabled and the driver is in standby.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `PERF-004` | The initial PWM amplitude at the start of the startup ramp shall be 10%. | hardware / instrument | PWM duty measured at the beginning of the startup ramp shall be 10% at t=3 s. (expected: PWM duty is 10%.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `PERF-005` | The maximum PWM amplitude during the demo shall be 20%. | hardware / instrument | The maximum measured PWM duty shall be no greater than 20% at every observation time during the automatic sequence. (expected: Maximum PWM duty is ≤20%.)<br>If PWM duty exceeds 20%, the firmware shall start the shutdown response and disable outputs at t=20 s. (expected: Shutdown begins after the violation and PWM outputs are disabled at t=20 s.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_set_profile | — | implemented, untested |
| `PERF-008` | Visible motor movement or rotation shall occur during the automatic run window. | hardware / instrument | The GBM2804H-100T shall exhibit visible movement or rotation continuously or intermittently during the automatic run window from t=3 s through t=20 s. (expected: Visible motor movement or rotation is observed at least once during t=3–20 s.)<br>Motor motion shall cease at or after t=20 s. (expected: No visible motor movement remains after t=20 s, and motor-driving PWM is disabled.) | firmware/app/app.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_drive | — | implemented, untested |

Notes recorded with these traces:
- `FR-001` — Portable controller maintains a continuous phase accumulator and computes U/V/W sinusoidal duties with ±120-degree offsets. Oscilloscope confirmation of the physical waveforms and rotating direction is outstanding.
- `FR-003`, `FR-004`, `FR-005`, `FR-018`, `PERF-004`, `PERF-005` — Source profile ramps frequency 1 to 10 Hz and amplitude 10% to 20% over 5 seconds, with monotonic linear commands and a 10 Hz/20% hold profile. Physical timing and duty measurements are outstanding.
- `FR-007`, `FR-008` — Source holds 10 Hz and 20% amplitude for the configured 10-second hold window; physical tolerance and waveform verification are outstanding.
- `PERF-008` — Automatic sinusoidal output is connected to the motor platform for the 3–20 second run. Visible movement and cessation after 20 seconds have not been observed.

## Automatic Demonstration Sequencer

gate: deferred · tasks: 5/5 done · recorded in: dae6d731-2131-4826-a713-9f4207a449a4

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-002` | The motor sequence shall start automatically after the configured boot delay without any UART input. | hardware / instrument | With no UART activity, the first sequence-start output occurs at 3.0 s after boot, within one scheduler tick. (expected: The first sequence-start event occurs at 3.0 s ± one scheduler tick; no UART input is required.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |
| `FR-006` | The firmware shall run the motor for the configured demonstration duration. | hardware / instrument | Motor motion is sustained throughout the 10 s hold interval after startup completes, and shutdown begins when that interval ends. (expected: Motion remains present throughout t=8 s through t=18 s, and the shutdown ramp begins at approximately t=18 s.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |
| `FR-009` | At 3 s from boot, the firmware shall enable the motor driver to begin the automatic sequence. | hardware / instrument | Driver EN transitions to its enabled level at approximately 3 s after boot and remains enabled until shutdown. (expected: EN becomes enabled at 3.0 s ± one scheduler tick and remains enabled until the shutdown sequence completes.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |
| `FR-010` | Between 18 s and 20 s from boot, the firmware shall ramp electrical frequency down to zero. | hardware / instrument | Electrical frequency decreases monotonically during the shutdown ramp. (expected: Frequency never increases during t=18 s to t=20 s.)<br>Electrical frequency reaches 0 Hz no later than 20 s after boot. (expected: Measured electrical frequency is 0 Hz by t=20 s and remains 0 Hz after the ramp.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_enable, motor_sequence_is_stopped | — | implemented, untested |
| `FR-011` | Between 18 s and 20 s from boot, the firmware shall ramp PWM amplitude down to zero. | hardware / instrument | PWM duty decreases monotonically during the shutdown ramp. (expected: PWM duty never increases during t=18 s to t=20 s.)<br>PWM duty reaches 0% no later than 20 s after boot. (expected: Measured PWM duty is 0% by t=20 s and remains 0% afterward.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_enable, motor_sequence_is_stopped | — | implemented, untested |
| `FR-012` | After 20 s from boot, the firmware shall disable PWM automatically. | hardware / instrument | No PWM switching activity is present at or after 20 s from boot. (expected: The PWM output has no rising or falling switching edges at any time t≥20 s.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_enable, motor_sequence_is_stopped | — | implemented, untested |
| `FR-013` | After 20 s from boot, the firmware shall place the motor driver in standby automatically. | hardware / instrument | Driver EN is deasserted at or after 20 s from boot. (expected: EN is deasserted no later than t=20 s and remains deasserted.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_enable, motor_sequence_is_stopped | — | implemented, untested |
| `FR-014` | After the automatic test completes, the motor shall remain stopped until the board is reset or power-cycled. | hardware / instrument | After the automatic sequence completes, no PWM switching or motor motion occurs before a subsequent reset or power cycle. (expected: No PWM edges and no motor motion occur for the entire observation interval after t=20 s and before the subsequent reset or power cycle.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c | app_start, motor_platform_enable, motor_sequence_is_stopped | — | implemented, untested |
| `PERF-002` | The boot delay before starting the sequence shall be 3 seconds. | hardware / instrument | The elapsed time from reset release to sequence start is 3.0 s within one scheduler tick. (expected: Sequence start occurs at 3.0 s ± one scheduler tick after reset release.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |
| `PERF-003` | The startup ramp duration shall be 5 seconds. | hardware / instrument | The startup frequency and amplitude ramps both complete within the interval from t=3 s through t=8 s. (expected: Both startup ramps are complete no later than t=8.0 s, giving a duration of no more than 5.0 s from t=3 s.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |
| `PERF-006` | The hold duration at 10 Hz and up to 20% PWM shall be 10 seconds. | hardware / instrument | The sequence remains in the hold state from t=8 s through t=18 s. (expected: The hold state lasts 10.0 s from t=8 s to t=18 s, with electrical frequency at 10 Hz and PWM duty no greater than 20%; shutdown begins at t=18 s.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |
| `PERF-007` | The shutdown ramp duration shall be 2 seconds. | hardware / instrument | The shutdown ramp lasts 2.0 s from t=18 s to t=20 s and reaches zero frequency and zero PWM amplitude by its end. (expected: The shutdown ramp starts at t=18.0 s, ends at t=20.0 s, and both electrical frequency and PWM duty are zero at or before t=20.0 s.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/motor_sequence/motor_sequence.h | app_start, motor_sequence_init, motor_sequence_step | — | implemented, untested |

Notes recorded with these traces:
- `FR-002`, `FR-006`, `FR-009`, `PERF-002`, `PERF-003`, `PERF-006`, `PERF-007` — Portable sequencer encodes the automatic 3–8 s startup ramp, 8–18 s hold, 18–20 s shutdown ramp, and no-restart stopped state. Hardware timing and driver EN measurements remain outstanding.
- `FR-010`, `FR-011`, `FR-012`, `FR-013`, `FR-014` — At terminal state the application disables the motor platform and does not restart. Physical PWM cessation, EN deassertion, and post-run motor stop remain unobserved.

## Fault Handling, Watchdog & Reset Safety

gate: deferred · tasks: 4/4 done · recorded in: dae6d731-2131-4826-a713-9f4207a449a4

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `SF-001` | An active STSPIN830 EN/FAULT condition shall immediately disable PWM at any time. | hardware / instrument | When EN/FAULT is asserted during any sequence phase, all PWM outputs stop within one control cycle and no later than 10 ms. (expected: Each PWM output has no transitions after the fault response point, and the response occurs within one control cycle and less than 10 ms after EN/FAULT assertion.)<br>After EN/FAULT is asserted, clearing the fault without applying the required reset or power-cycle does not produce any PWM output. (expected: No PWM transitions occur after EN/FAULT is cleared until the specified reset or power-cycle sequence is performed.) | firmware/app/app.c<br>firmware/platforms/stm32/safety_platform.c<br>firmware/platforms/stm32/safety_platform.h | app_start, safety_platform_check_fault, safety_platform_disable | — | implemented, untested |
| `SF-002` | The firmware shall enable a hardware watchdog such that, if the control task becomes unresponsive during the automatic sequence, the MCU resets and returns the system to the boot safe state. | on-target test, hardware / instrument | A deliberate control-task hang during the 8–18 s hold window causes a hardware-watchdog reset. (expected: The MCU restarts and the recorded reset cause identifies a hardware-watchdog reset.)<br>After a watchdog-induced reset, PWM remains inactive and the motor driver remains in standby throughout the first 3 seconds of boot. (expected: No PWM switching is observed and driver EN remains deasserted/standby asserted continuously from reset through t=3.000 s.) | firmware/app/app.c<br>firmware/platforms/stm32/safety_platform.c | safety_platform_init, safety_platform_watchdog_refresh | — | implemented, untested |
| `SF-003` | Following detection of an active STSPIN830 EN/FAULT condition, the firmware shall not attempt to re-enable the motor driver or PWM automatically; recovery shall require a reset or power-cycle. | hardware / instrument | After an active EN/FAULT condition stops the run, clearing EN/FAULT without reset or power-cycle does not re-enable the driver or generate PWM for at least the demo duration. (expected: For the entire observation interval, driver EN remains deasserted/standby remains asserted and no PWM transitions occur.) | firmware/app/app.c<br>firmware/platforms/stm32/safety_platform.c<br>firmware/platforms/stm32/safety_platform.h | app_start, safety_platform_check_fault, safety_platform_disable | — | implemented, untested |
| `SF-004` | On any MCU reset event (including initial boot), the firmware shall keep PWM disabled and the motor driver in standby for the first 3 seconds before any enable action may occur. | hardware / instrument | Following each supported reset source, no PWM switching occurs and the driver EN signal remains deasserted for the first 3 seconds after reset. (expected: For every reset source, no PWM transition is observed before t=3.000 s and driver EN remains deasserted/standby asserted throughout that interval.)<br>After the 3-second boot delay, the sequence does not perform an enable action while EN/FAULT is active. (expected: Driver EN remains deasserted/standby remains asserted and no PWM switching starts while EN/FAULT is active.) | firmware/app/app.c<br>firmware/platforms/stm32/motor_platform.c<br>firmware/platforms/stm32/safety_platform.c | app_start, motor_platform_enable, motor_platform_init | — | implemented, untested |

Notes recorded with these traces:
- `SF-001`, `SF-003` — Active-low EN/FAULT is polled in the control path, latched in runtime, and causes PWM disable plus driver standby. Immediate timing and fault-clear waveform behavior require hardware verification.
- `SF-002` — STM32 IWDG is initialized and refreshed from the responsive boot/control path. Deliberate hang and watchdog reset-cause observation remain on-target work.
- `SF-004` — PWM is initialized at zero duty, STBY is asserted during driver initialization, and watchdog refresh occurs during the 3-second boot delay before enable is permitted. Reset-source waveform verification remains outstanding.

## Control Scope & Architecture Boundaries

gate: deferred · tasks: 2/2 done · recorded in: dae6d731-2131-4826-a713-9f4207a449a4

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `NFR-002` | The project scope shall be limited to verifying the MCU, motor-driver board, PWM outputs, and motor connections. | inspection | The design and test artifacts contain only MCU I/O, STSPIN830 U/V/W drive, PWM/GPIO, and motor-connectivity verification for the basic rotation demo. (expected: All documented and implemented scope is limited to MCU I/O, STSPIN830 U/V/W driving, PWM/GPIO, motor connections, and the basic rotation demo.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-004` | The firmware shall not implement sensorless field-oriented control (FOC). | inspection | The firmware source contains no sensorless FOC algorithm or transform and its PWM generation is open-loop sinusoidal. (expected: No FOC algorithm or transform is present, and PWM is generated as open-loop sinusoidal PWM.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-005` | The firmware shall not implement a rotor observer. | inspection | The firmware source contains no rotor-observer or rotor-estimator module, call, or configuration entry. (expected: No rotor-observer or rotor-estimator module, call, or enabled configuration entry is present.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-006` | The firmware shall not implement Clarke or Park transforms. | inspection | The firmware source contains no Clarke or Park transform implementation or call. (expected: No code performing Clarke or Park transforms, including αβ or dq transformations, is present.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-007` | The firmware shall not implement current PI or speed PI control. | inspection | The firmware source and configuration contain no phase-current PI controller or speed PI controller. (expected: No PI controller for phase current or speed exists in source or configuration.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-008` | The firmware shall not perform phase-current ADC control. | inspection | No ADC channel, conversion result, or ADC-processing path is used to control phase-current PWM or motor operation. (expected: ADC is not used to sense phase currents for control purposes.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-009` | The firmware shall not perform VBUS ADC control. | inspection | No ADC channel, conversion result, or ADC-processing path is used to control the motor from measured DC-bus voltage. (expected: ADC is not used to sense DC-bus voltage for control purposes.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-010` | The firmware shall not use ADC-triggered DMA. | inspection | The project configuration and firmware contain no ADC-triggered DMA enablement or request path. (expected: No ADC-triggered DMA configuration or code path is present.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-011` | The firmware shall not perform current-offset calibration. | inspection | The firmware contains no routine that measures, computes, stores, or applies phase-current offsets. (expected: No phase-current offset measurement, storage, or application routine exists.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-012` | The firmware shall not include oscilloscope integration. | inspection | The firmware source and project configuration contain no oscilloscope capture, streaming, or scope-integration code. (expected: No oscilloscope integration, capture path, or streaming implementation is present.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-013` | The test shall not measure actual motor RPM. | inspection | The firmware contains no speed sensor or estimator input and reports or logs no RPM value. (expected: No speed sensor or estimator is used, and no RPM value is reported or logged.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-014` | The firmware shall not implement UART-based motor start/stop, and the automatic motor sequence shall neither require nor respond to any UART input to start or stop. | inspection, hardware / instrument | The firmware contains no UART command handler that starts, stops, or otherwise controls the automatic motor sequence. (expected: No UART input path can start, stop, or otherwise control the automatic sequence.)<br>With UART disconnected or receiving arbitrary commands, the automatic sequence starts after the boot delay and completes according to its timeline without UART-dependent state changes. (expected: Both runs start after the defined boot delay and complete the same timeline; UART input causes no start, stop, or other sequence-state transition.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-015` | The firmware and test shall not implement or perform speed/RPM regulation; no closed-loop speed control shall be present. | inspection | The firmware and test artifacts contain no speed feedback loop, RPM target, or electrical-frequency adjustment based on measured speed. (expected: No closed-loop speed-regulation path, RPM target, or speed-dependent electrical-frequency adjustment is present or enabled.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `NFR-016` | The firmware shall not perform motor-parameter identification or characterization (e.g., resistance, inductance, flux), whether automatic or manual. | inspection | The firmware contains no automatic or manual code path that estimates, identifies, characterizes, or stores motor resistance, inductance, or flux parameters. (expected: No code path or routine estimates, identifies, characterizes, or stores motor electrical parameters R, L, or flux.) | firmware/README.md<br>firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/docs/overview.md<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/modules/sine_control/sine_control.c | app_start, motor_sequence_step, sine_control_output | — | implemented, untested |
| `FR-015` | The automatic sequence shall depend only on successful PWM/GPIO initialization, completion of the boot delay, and absence of an active STSPIN830 hardware fault. | on-target test, hardware / instrument | After reset, the automatic sequence enters its running state only when PWM/GPIO initialization has succeeded, the 3 s boot delay has elapsed, and EN/FAULT indicates no active STSPIN830 fault. (expected: The sequence remains stopped before 3 s and starts after 3 s only in the all-true case; it does not start in any case where one dependency is false.)<br>When any automatic-sequence dependency is false, PWM remains disabled and the STSPIN830 remains in standby. (expected: PWM is disabled and the driver is in standby for every dependency-failure condition.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c<br>firmware/platforms/stm32/safety_platform.c | app_start, motor_platform_init, motor_sequence_step, safety_platform_check_fault | — | implemented, untested |
| `FR-016` | The automatic motor sequence shall not be blocked because ADC measurements, motor electrical parameters, characterization, FOC, observer, current sensing, UART commands, or closed-loop control are unavailable. | on-target test | With ADC measurements, motor-parameter characterization, FOC, observer, current sensing, UART commands, and closed-loop control disabled or absent, the automatic sequence starts and completes according to the defined timeline. (expected: The sequence starts after the boot delay and reaches each defined timeline state through completion without requiring any listed feature.) | firmware/app/app.c<br>firmware/modules/motor_sequence/motor_sequence.c<br>firmware/platforms/stm32/motor_platform.c<br>firmware/platforms/stm32/safety_platform.c | app_start, motor_platform_init, motor_sequence_step, safety_platform_check_fault | — | implemented, untested |

Notes recorded with these traces:
- `NFR-002`, `NFR-004`, `NFR-005`, `NFR-006`, `NFR-007`, `NFR-008`, `NFR-009`, `NFR-010`, `NFR-011`, `NFR-012`, `NFR-013`, `NFR-014`, `NFR-015`, `NFR-016` — Inspection audit and documentation establish the intentionally minimal open-loop scope. No UART command path, ADC control path, FOC/transform, observer, PI, RPM, characterization, or oscilloscope integration is present in the vendor-neutral firmware tree. Unused RTOS scaffolding was removed.
- `FR-015`, `FR-016` — The automatic path is gated by successful initialization, the 3-second boot delay, and an inactive EN/FAULT input; it does not consult ADC, characterization, FOC, observer, current sensing, UART, or closed-loop control. On-target dependency-gate behavior remains unverified.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### Host tests you could write (2)

These have a `host` acceptance criterion and cite no test, so they are the ones a unit test can actually cover -- and the only ones; see the next section. They do NOT run on the machine that generated this firmware, which carries the vendor cross-compilation toolchain and no host compiler. Run `test_handover export` and `sources/test_handover/` will hold this list machine-readably, with the criteria to assert, where each requirement's code lives, and which firmware layers compile standalone versus needing a mocked header.

- `IF-001` (PWM Timer & Motor-Driver Interface)
- `FR-017` (PWM Timer & Motor-Driver Interface)

### NOT unit-testable (51) — do not write host tests for these

No `host` criterion, so no unit test can cover them however it is written. Listed explicitly because the untested count above otherwise reads as a backlog of unit tests owed -- and closing one of these with a host test means inventing an assertion that passes without checking the thing. Each line says how it IS checked instead.

- `NFR-001` — checked by hardware / instrument · _Target Hardware & Demonstration Build_
- `NFR-003` — checked by inspection, hardware / instrument · _Target Hardware & Demonstration Build_
- `HW-001` — checked by hardware / instrument · _Target Hardware & Demonstration Build_
- `HW-002` — checked by inspection · _Target Hardware & Demonstration Build_
- `HW-003` — checked by inspection · _Target Hardware & Demonstration Build_
- `HW-004` — checked by inspection, not verifiable · _Target Hardware & Demonstration Build_
- `IF-002` — checked by inspection · _PWM Timer & Motor-Driver Interface_
- `IF-003` — checked by hardware / instrument · _PWM Timer & Motor-Driver Interface_
- `PERF-001` — checked by hardware / instrument, not verifiable · _PWM Timer & Motor-Driver Interface_
- `FR-001` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-003` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-004` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-005` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-007` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-008` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-018` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `PERF-004` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `PERF-005` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `PERF-008` — checked by hardware / instrument · _Open-Loop Sinusoidal Motor Control_
- `FR-002` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-006` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-009` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-010` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-011` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-012` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-013` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `FR-014` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `PERF-002` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `PERF-003` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `PERF-006` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `PERF-007` — checked by hardware / instrument · _Automatic Demonstration Sequencer_
- `SF-001` — checked by hardware / instrument · _Fault Handling, Watchdog & Reset Safety_
- `SF-002` — checked by on-target test, hardware / instrument · _Fault Handling, Watchdog & Reset Safety_
- `SF-003` — checked by hardware / instrument · _Fault Handling, Watchdog & Reset Safety_
- `SF-004` — checked by hardware / instrument · _Fault Handling, Watchdog & Reset Safety_
- `NFR-002` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-004` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-005` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-006` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-007` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-008` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-009` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-010` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-011` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-012` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-013` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-014` — checked by inspection, hardware / instrument · _Control Scope & Architecture Boundaries_
- `NFR-015` — checked by inspection · _Control Scope & Architecture Boundaries_
- `NFR-016` — checked by inspection · _Control Scope & Architecture Boundaries_
- `FR-015` — checked by on-target test, hardware / instrument · _Control Scope & Architecture Boundaries_
- `FR-016` — checked by on-target test · _Control Scope & Architecture Boundaries_

### Board checks owed (36)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `NFR-001` (Target Hardware & Demonstration Build)
- `NFR-003` (Target Hardware & Demonstration Build)
- `HW-001` (Target Hardware & Demonstration Build)
- `IF-001` (PWM Timer & Motor-Driver Interface)
- `IF-003` (PWM Timer & Motor-Driver Interface)
- `PERF-001` (PWM Timer & Motor-Driver Interface)
- `FR-017` (PWM Timer & Motor-Driver Interface)
- `FR-001` (Open-Loop Sinusoidal Motor Control)
- `FR-003` (Open-Loop Sinusoidal Motor Control)
- `FR-004` (Open-Loop Sinusoidal Motor Control)
- `FR-005` (Open-Loop Sinusoidal Motor Control)
- `FR-007` (Open-Loop Sinusoidal Motor Control)
- `FR-008` (Open-Loop Sinusoidal Motor Control)
- `FR-018` (Open-Loop Sinusoidal Motor Control)
- `PERF-004` (Open-Loop Sinusoidal Motor Control)
- `PERF-005` (Open-Loop Sinusoidal Motor Control)
- `PERF-008` (Open-Loop Sinusoidal Motor Control)
- `FR-002` (Automatic Demonstration Sequencer)
- `FR-006` (Automatic Demonstration Sequencer)
- `FR-009` (Automatic Demonstration Sequencer)
- `FR-010` (Automatic Demonstration Sequencer)
- `FR-011` (Automatic Demonstration Sequencer)
- `FR-012` (Automatic Demonstration Sequencer)
- `FR-013` (Automatic Demonstration Sequencer)
- `FR-014` (Automatic Demonstration Sequencer)
- `PERF-002` (Automatic Demonstration Sequencer)
- `PERF-003` (Automatic Demonstration Sequencer)
- `PERF-006` (Automatic Demonstration Sequencer)
- `PERF-007` (Automatic Demonstration Sequencer)
- `SF-001` (Fault Handling, Watchdog & Reset Safety)
- `SF-002` (Fault Handling, Watchdog & Reset Safety)
- `SF-003` (Fault Handling, Watchdog & Reset Safety)
- `SF-004` (Fault Handling, Watchdog & Reset Safety)
- `NFR-014` (Control Scope & Architecture Boundaries)
- `FR-015` (Control Scope & Architecture Boundaries)
- `FR-016` (Control Scope & Architecture Boundaries)

### Implemented, never tested (53)

Code exists and no test is recorded against it.

- `NFR-001` (Target Hardware & Demonstration Build)
- `NFR-003` (Target Hardware & Demonstration Build)
- `HW-001` (Target Hardware & Demonstration Build)
- `HW-002` (Target Hardware & Demonstration Build)
- `HW-003` (Target Hardware & Demonstration Build)
- `HW-004` (Target Hardware & Demonstration Build)
- `IF-001` (PWM Timer & Motor-Driver Interface)
- `IF-002` (PWM Timer & Motor-Driver Interface)
- `IF-003` (PWM Timer & Motor-Driver Interface)
- `PERF-001` (PWM Timer & Motor-Driver Interface)
- `FR-017` (PWM Timer & Motor-Driver Interface)
- `FR-001` (Open-Loop Sinusoidal Motor Control)
- `FR-003` (Open-Loop Sinusoidal Motor Control)
- `FR-004` (Open-Loop Sinusoidal Motor Control)
- `FR-005` (Open-Loop Sinusoidal Motor Control)
- `FR-007` (Open-Loop Sinusoidal Motor Control)
- `FR-008` (Open-Loop Sinusoidal Motor Control)
- `FR-018` (Open-Loop Sinusoidal Motor Control)
- `PERF-004` (Open-Loop Sinusoidal Motor Control)
- `PERF-005` (Open-Loop Sinusoidal Motor Control)
- `PERF-008` (Open-Loop Sinusoidal Motor Control)
- `FR-002` (Automatic Demonstration Sequencer)
- `FR-006` (Automatic Demonstration Sequencer)
- `FR-009` (Automatic Demonstration Sequencer)
- `FR-010` (Automatic Demonstration Sequencer)
- `FR-011` (Automatic Demonstration Sequencer)
- `FR-012` (Automatic Demonstration Sequencer)
- `FR-013` (Automatic Demonstration Sequencer)
- `FR-014` (Automatic Demonstration Sequencer)
- `PERF-002` (Automatic Demonstration Sequencer)
- `PERF-003` (Automatic Demonstration Sequencer)
- `PERF-006` (Automatic Demonstration Sequencer)
- `PERF-007` (Automatic Demonstration Sequencer)
- `SF-001` (Fault Handling, Watchdog & Reset Safety)
- `SF-002` (Fault Handling, Watchdog & Reset Safety)
- `SF-003` (Fault Handling, Watchdog & Reset Safety)
- `SF-004` (Fault Handling, Watchdog & Reset Safety)
- `NFR-002` (Control Scope & Architecture Boundaries)
- `NFR-004` (Control Scope & Architecture Boundaries)
- `NFR-005` (Control Scope & Architecture Boundaries)
- `NFR-006` (Control Scope & Architecture Boundaries)
- `NFR-007` (Control Scope & Architecture Boundaries)
- `NFR-008` (Control Scope & Architecture Boundaries)
- `NFR-009` (Control Scope & Architecture Boundaries)
- `NFR-010` (Control Scope & Architecture Boundaries)
- `NFR-011` (Control Scope & Architecture Boundaries)
- `NFR-012` (Control Scope & Architecture Boundaries)
- `NFR-013` (Control Scope & Architecture Boundaries)
- `NFR-014` (Control Scope & Architecture Boundaries)
- `NFR-015` (Control Scope & Architecture Boundaries)
- `NFR-016` (Control Scope & Architecture Boundaries)
- `FR-015` (Control Scope & Architecture Boundaries)
- `FR-016` (Control Scope & Architecture Boundaries)

### No observable exists (3)

These need a human decision — either the requirement is restated as something checkable, or it is accepted as unverifiable on the record. Inventing a test for one of these produces a test that passes and proves nothing.

- `HW-004` — The firmware does not attempt to start the motor sequence when the supply is outside the permitted specification. · _No allowable supply-voltage range and no observable voltage-validity or motor-start interface are specified, so an out-of-range failure check cannot be made concrete._
- `PERF-001` — The PWM-frequency pass/fail band around 20 kHz cannot be asserted until the required measurement tolerance is specified. · _No numeric PWM frequency tolerance is specified, so a measured value cannot be classified as passing or failing against the requirement._
- `FR-017` — The complete startup ramp cannot be checked for exact duty-cycle values or timing until the referenced sequence ramp points are specified. · _The startup ramp lacks explicit duty-cycle and timing values, so exact conformance cannot be asserted beyond the initial 10% check._
