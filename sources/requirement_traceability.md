# Requirement traceability

Generated 2026-09-24T00:44:27.793587400+00:00
Specification frozen 2026-09-24T00:38:51.026Z

46 of 46 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 46 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project. Host tests run on a separate machine; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## Target Hardware & Build Configuration

gate: reviewed · tasks: 4/4 done · recorded in: 305bfa04-5f10-49ba-ac62-2eb3829f110a

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-001` | The firmware shall interface with the X-NUCLEO-IHM16M1 motor driver based on the STSPIN830. | hardware / instrument | With the X-NUCLEO-IHM16M1 attached, the firmware changes the STSPIN830 drive-enable signal between enabled and disabled states. (expected: The driver-enable signal is in the enabled state after the enable command and in the disabled state after the disable command.)<br>With the X-NUCLEO-IHM16M1 attached, the firmware produces PWM on the driver input associated with TIM1. (expected: A periodic PWM waveform is present at the configured driver input while drive is enabled.) | firmware/drivers/x_nucleo_ihm16m1/include/x_nucleo_ihm16m1.h<br>firmware/drivers/x_nucleo_ihm16m1/src/x_nucleo_ihm16m1.c<br>firmware/platforms/stm32/stm32_motor_board.c | motor_board_prepare, x_nucleo_ihm16m1_init | — | implemented, untested |
| `HW-002` | The firmware shall be capable of rotating the GBM2804H-100T three-phase gimbal motor. | hardware / instrument | When commanded to start with the GBM2804H-100T connected and powered, the motor exhibits observable rotation. (expected: The rotor exhibits observable movement or rotation after the start command.) | firmware/README.md<br>firmware/configs/app_config.h | motor_board_prepare | — | implemented, untested |
| `HW-003` | The firmware shall assume the motor has 7 pole pairs. | inspection | The firmware's motor pole-pair configuration used by motor calculations or reporting is set to 7. (expected: The configured motor pole-pair value is exactly 7 pairs.) | firmware/app/app.c<br>firmware/configs/app_config.h | app_start | — | implemented, untested |
| `HW-004` | The system shall be powered by a 12 V DC supply during this test. | hardware / instrument | The motor-driver supply measured at the test setup is 12 V DC. (expected: The measured supply is 12 V DC.) | firmware/README.md<br>firmware/configs/app_config.h | app_start | — | implemented, untested |
| `HW-005` | The motor drive outputs shall be the U, V, W three-phase outputs. | inspection | The three motor phase connections use the hardware output terminals labeled U, V, and W. (expected: The three motor phase connections terminate at U, V, and W, with no motor phase connected to another output.) | firmware/README.md<br>firmware/configs/app_config.h | motor_board_prepare | — | implemented, untested |
| `NFR-005` | The firmware shall build successfully, be flashable to the NUCLEO-G431RB target board, and execute on that board without modification. | inspection, hardware / instrument | A clean firmware build completes for the NUCLEO-G431RB without compiler, linker, or build errors. (expected: The clean build exits successfully with no build errors and produces a firmware image for the NUCLEO-G431RB.)<br>The generated firmware image flashes to the NUCLEO-G431RB and the board executes it without changing the image or source after the build. (expected: Programming completes successfully, the flashed image is unchanged from the clean-build artifact, and the NUCLEO-G431RB powers up and executes that image without source or binary modification.) | CMakeLists.txt<br>STM32G431RBTX_FLASH.ld<br>firmware/app/app.c<br>firmware/platforms/stm32/stm32_motor_board.c | app_start, main, motor_board_prepare | — | implemented, untested |

Observed on the board:
- `HW-001` — Fresh image flashed successfully. The ST-LINK monitor captured no UART bytes, and the user reported LD2 dark or erratic. Debug attachment found the target still running and did not hit HardFault; enable/PWM signal levels were not instrumented, so this criterion remains unverified.
- `HW-002` — No motor rotation observation was obtained; monitor was silent after reset, so mechanical rotation remains unverified.
- `HW-004` — No bench-supply measurement was captured; the 12 V DC condition remains unverified.
- `NFR-005` — Clean CMake/Ninja build and ST-LINK flash both succeeded without modifying the generated artifact. The board was detected as NUCLEO-G431RB / STM32G43x with 128 KB flash and 3.29 V. Runtime monitor was silent and no application marker was captured, so execute-path success is not verified.

Notes recorded with these traces:
- `HW-003` — APP_MOTOR_POLE_PAIRS is exactly 7U and is used in the startup status format.
- `HW-005` — Source inspection records U/V/W terminal labels and PA8/PA9/PA10 routes; physical phase-terminal wiring was not inspected.

## Motor Driver & PWM Generation

gate: reviewed · tasks: 4/4 done · recorded in: 305bfa04-5f10-49ba-ac62-2eb3829f110a

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-001` | The firmware shall configure TIM1 to generate three-phase PWM. | hardware / instrument | With the firmware initialized, TIM1 produces three simultaneous PWM signals on the U, V, and W driver-input connections with a common synchronized time base. (expected: Three active PWM waveforms are present simultaneously on U, V, and W, and their carrier timing is synchronized.) | firmware/platforms/stm32/stm32_motor_board.c | configure_timer, pwm_enable | — | implemented, untested |
| `IF-002` | The firmware shall use the factory-default X-NUCLEO-IHM16M1 three-input configuration. | inspection | The driver configuration selects the factory three-PWM-input mode and contains no configuration that selects an alternate commutation mode. (expected: The factory three-PWM-input mode is selected, and no alternate commutation mode is configured.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/stm32_motor_board.c | motor_board_prepare | — | implemented, untested |
| `FR-001` | The firmware shall generate a rotating three-phase electrical field using open-loop sinusoidal PWM. | hardware / instrument | When PWM is enabled without an active fault, the U, V, and W PWM duty cycles form three sinusoidal modulation signals separated by 120 electrical degrees. (expected: The three modulation envelopes are phase-shifted by 120 electrical degrees, within the measurement resolution.)<br>The phase sequence of the three PWM modulation envelopes advances at the commanded electrical frequency. (expected: The measured electrical frequency equals the commanded electrical frequency within the measurement resolution.) | firmware/app/app.c<br>firmware/modules/motor_waveform/motor_waveform.c<br>firmware/platforms/stm32/stm32_motor_board.c | app_process, motor_board_apply_pwm, motor_waveform_step | — | implemented, untested |
| `IF-003` | STBY shall be controlled to enable and disable the motor driver. | hardware / instrument | Driving STBY to its standby level places the STSPIN830 in standby, and driving it to its enable level places the driver in the enabled state. (expected: The standby-level command results in the datasheet-defined standby/disabled state, and the enable-level command results in the datasheet-defined enabled state.) | firmware/drivers/x_nucleo_ihm16m1/src/x_nucleo_ihm16m1.c<br>firmware/platforms/stm32/stm32_motor_board.c | motor_board_set_enabled, x_nucleo_ihm16m1_set_standby | — | implemented, untested |
| `PERF-001` | The PWM switching frequency shall be 20 kHz. | hardware / instrument | The measured TIM1 PWM carrier frequency is 20 kHz. (expected: The carrier frequency is 20 kHz within the measurement resolution.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/stm32_motor_board.c | configure_timer | — | implemented, untested |
| `PERF-002` | The initial PWM amplitude shall be 10%. | on-target test | At entry to the startup ramp, the commanded PWM duty-cycle amplitude is 10%. (expected: The initial commanded PWM amplitude is 10%.) | firmware/configs/app_config.h<br>firmware/modules/motor_waveform/motor_waveform.c | motor_waveform_initialize, motor_waveform_step | — | implemented, untested |
| `PERF-003` | The PWM amplitude shall not exceed 20% during this test. | hardware / instrument | The effective PWM duty cycle on every output remains at or below 20% when a duty command greater than 20% is applied. (expected: No measured output duty cycle exceeds 20%.) | firmware/configs/app_config.h<br>firmware/modules/motor_waveform/motor_waveform.c | motor_waveform_initialize, motor_waveform_step | — | implemented, untested |
| `PERF-004` | The initial electrical frequency shall be 1 Hz. | hardware / instrument | At entry to the startup ramp, the generated electrical frequency is 1 Hz. (expected: The initial electrical frequency is 1 Hz within the measurement resolution.) | firmware/configs/app_config.h<br>firmware/modules/motor_waveform/motor_waveform.c | motor_waveform_initialize, motor_waveform_step | — | implemented, untested |
| `PERF-005` | The electrical-frequency ramp target shall be 10 Hz. | hardware / instrument | During the startup ramp, the generated electrical frequency reaches 10 Hz and does not exceed 10 Hz without an explicit reconfiguration command. (expected: The measured frequency reaches 10 Hz, never exceeds 10 Hz while unreconfigured, and remains at the target until explicitly reconfigured.) | firmware/configs/app_config.h<br>firmware/modules/motor_waveform/motor_waveform.c | motor_waveform_initialize, motor_waveform_step | — | implemented, untested |
| `FR-012` | The firmware shall support clockwise and counter-clockwise rotation; issuing "DIR CW" or "DIR CCW" shall reverse the generated phase sequence and rotation direction accordingly. | hardware / instrument | After issuing DIR CW and then DIR CCW, the measured phase order changes to match the most recently commanded direction. (expected: The phase order after `DIR CW` matches the CW sequence, and the phase order after `DIR CCW` is the reversed CCW sequence and remains the sequence associated with the last command.)<br>The motor's observed rotation direction reverses when the command changes from DIR CW to DIR CCW. (expected: The motor rotates in the CW direction after `DIR CW` and in the opposite, CCW direction after `DIR CCW`.) | firmware/modules/motor_waveform/motor_waveform.c | motor_waveform_set_direction, motor_waveform_step | — | implemented, untested |

Observed on the board:
- `IF-001` — Category 2 review image built and flashed successfully. No oscilloscope or logic-analyzer capture was available; simultaneous U/V/W PWM remains unverified.
- `FR-001` — The waveform update path is wired from main through app_process, but no instrument capture was available to verify 120-degree separation or electrical frequency.
- `IF-003` — ST-LINK flash succeeded, but STBY level and STSPIN830 enabled state were not measured.

Notes recorded with these traces:
- `IF-002` — Source inspection confirms X_NUCLEO_IHM16M1_THREE_PWM_INPUTS and no alternate commutation mode.
- `PERF-001` — APP_MOTOR_PWM_FREQUENCY_HZ is 20000U; timer configuration is sourced from the 170 MHz clock and center-aligned period 4249. Measurement remains outstanding.
- `PERF-002`, `PERF-003` — Initial amplitude is 0.10 and clamp maximum is 0.20.
- `PERF-004`, `PERF-005` — Initial and target electrical-frequency values are 1 Hz and 10 Hz; no hardware frequency capture was obtained.
- `FR-012` — The waveform supports opposite phase progression signs, but no UART command ingress exists and motor_waveform_set_direction is not reachable. Runtime direction reversal is not claimed.

## Motor Startup, Lifecycle & Fault Safety

gate: reviewed · tasks: 4/4 done · recorded in: 305bfa04-5f10-49ba-ac62-2eb3829f110a

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-002` | The startup sequence shall start with a low PWM amplitude. | hardware / instrument, host test | At the first sample of the startup ramp, the commanded PWM duty is 10%. (expected: 10% duty at startup-ramp entry)<br>Every PWM duty command generated during the startup ramp is no greater than the configured maximum amplitude. (expected: Each duty value is less than or equal to the configured maximum amplitude) | firmware/configs/app_config.h<br>firmware/modules/motor_waveform/motor_waveform.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | motor_lifecycle_process, motor_lifecycle_start, motor_waveform_initialize | — | implemented, untested |
| `FR-003` | The startup sequence shall gradually ramp the electrical frequency from its initial value toward the target. | hardware / instrument | The measured electrical frequency samples collected during startup are monotonically non-decreasing from the configured initial frequency toward the 10 Hz ramp target. (expected: Frequency never decreases and reaches the configured 10 Hz target) | firmware/configs/app_config.h<br>firmware/modules/motor_waveform/motor_waveform.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | motor_lifecycle_process, motor_lifecycle_start, motor_waveform_initialize | — | implemented, untested |
| `FR-005` | MOTOR START shall not be blocked because ADC measurements, motor electrical parameters, characterization, FOC, observer, current sensing, or closed-loop control are unavailable. | hardware / instrument | With ADC measurement, motor-parameter, characterization, FOC, observer, current-sensing, and closed-loop-control features disabled or absent, a MOTOR START command produces the first startup-ramp PWM command. (expected: Startup-ramp state is entered and PWM startup output begins) | firmware/app/app.c<br>firmware/interfaces/motor_board_if.h<br>firmware/platforms/stm32/stm32_motor_board.c | app_process, app_start, motor_board_read_fault, motor_board_shutdown | — | implemented, untested |
| `FR-006` | MOTOR START shall depend only on successful PWM/GPIO initialization and the absence of an active STSPIN830 hardware fault. | hardware / instrument | When PWM/GPIO initialization succeeds and EN/FAULT is inactive, MOTOR START is accepted and the startup ramp begins. (expected: MOTOR START is accepted and the state changes to startup ramp)<br>When PWM/GPIO initialization fails or EN/FAULT is active, MOTOR START does not enable PWM or enter the startup ramp. (expected: PWM remains disabled and the startup-ramp state is not entered) | firmware/app/app.c<br>firmware/interfaces/motor_board_if.h<br>firmware/platforms/stm32/stm32_motor_board.c | app_process, app_start, motor_board_read_fault, motor_board_shutdown | — | implemented, untested |
| `FR-007` | On reset or power-up, the firmware shall hold the motor driver in standby (disabled) via STBY until TIM1/GPIO are fully configured and PWM outputs are in a defined inactive state; only then may the driver be enabled. | hardware / instrument | After reset or power-up, STBY remains at the configured disable level until TIM1/GPIO configuration is complete and PWM outputs are at their defined inactive levels. (expected: STBY remains at the configured disable level until configuration completion, and all three PWM inputs are at their defined inactive levels before STBY is enabled)<br>After STBY is enabled, the motor remains stopped until a valid MOTOR START command is received. (expected: No motor drive or motion occurs before MOTOR START; startup PWM begins only after the valid command) | firmware/app/app.c<br>firmware/interfaces/motor_board_if.h<br>firmware/platforms/stm32/stm32_motor_board.c | app_process, app_start, motor_board_read_fault, motor_board_shutdown | — | implemented, untested |
| `IF-018` | When PWM is disabled (by MOTOR STOP or driver fault), the three driver input pins shall be driven to the documented inactive level for the factory three-input configuration, ensuring no residual drive is applied to the motor phases. | hardware / instrument | After MOTOR STOP or while EN/FAULT is active, all three driver-input pins remain at the documented inactive levels and show no PWM switching. (expected: All three inputs are continuously at their documented inactive levels with no switching after either event)<br>Following MOTOR STOP, STATUS reports Stopped and following an EN/FAULT event, STATUS reports the applicable fault state. (expected: MOTOR STOP produces STATUS=Stopped, and an asserted EN/FAULT produces STATUS indicating the active driver fault)<br>When the inactive driver-input level cannot be driven, STBY is asserted to the configured disable level. (expected: STBY is at the configured disable level and the driver is forced into standby) | firmware/app/app.c<br>firmware/interfaces/motor_board_if.h<br>firmware/platforms/stm32/stm32_motor_board.c | app_process, app_start, motor_board_read_fault, motor_board_shutdown | — | implemented, untested |
| `FR-008` | After boot/reset and before any command is issued, PWM shall remain disabled and the motor shall remain stopped. | hardware / instrument | Immediately after initialization and before any command, the three driver-input pins show no PWM activity and the motor remains stopped. (expected: No PWM transitions are present and the motor does not move) | firmware/app/app.c<br>firmware/interfaces/motor_board_if.h<br>firmware/platforms/stm32/stm32_motor_board.c | app_process, app_start, motor_board_read_fault, motor_board_shutdown | — | implemented, untested |
| `FR-009` | Upon receiving the UART command "MOTOR START", the firmware shall initiate the defined open-loop startup ramp. | hardware / instrument | A UART MOTOR START command begins the open-loop startup ramp with the configured initial amplitude and configured initial frequency. (expected: The startup-ramp state begins; first amplitude equals the configured initial amplitude and first frequency equals the configured initial frequency) | firmware/app/app.c<br>firmware/platforms/stm32/stm32_motor_board.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | dispatch_command, motor_board_shutdown, motor_lifecycle_fault, motor_lifecycle_start, motor_lifecycle_stop | — | implemented, untested |
| `FR-010` | Upon receiving the UART command "MOTOR STOP", the firmware shall immediately disable PWM (within one control cycle) and stop the motor. | hardware / instrument | While PWM is enabled or the startup ramp is active, receiving MOTOR STOP disables all PWM outputs within one control cycle. (expected: All PWM outputs are disabled no later than one control cycle after MOTOR STOP reception)<br>After MOTOR STOP, the motor is no longer driven. (expected: No phase-drive waveform remains and the motor is not driven) | firmware/app/app.c<br>firmware/platforms/stm32/stm32_motor_board.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | dispatch_command, motor_board_shutdown, motor_lifecycle_fault, motor_lifecycle_start, motor_lifecycle_stop | — | implemented, untested |
| `FR-011` | An active STSPIN830 EN/FAULT condition shall immediately disable PWM (within one control cycle). | hardware / instrument | When EN/FAULT is asserted while PWM is enabled, all PWM outputs are disabled within one control cycle. (expected: All PWM outputs are disabled no later than one control cycle after EN/FAULT assertion)<br>PWM remains disabled after an EN/FAULT event until the fault clears and a new MOTOR START command is received. (expected: PWM remains disabled after fault clearing alone and resumes only after the subsequent MOTOR START command) | firmware/app/app.c<br>firmware/platforms/stm32/stm32_motor_board.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | dispatch_command, motor_board_shutdown, motor_lifecycle_fault, motor_lifecycle_start, motor_lifecycle_stop | — | implemented, untested |

Observed on the board:
- `FR-002`, `FR-003` — Current command-enabled image built and flashed successfully. Reachability analysis confirms motor_lifecycle_start is now wired through the command dispatcher. No usable UART response or oscilloscope capture was obtained, so startup duty and measured frequency ramp remain unverified on hardware.
- `FR-005`, `FR-006`, `FR-007`, `IF-018`, `FR-008` — Current source treats fault-read failure as a fault-safe shutdown, calls centralized shutdown at initialization, and does not automatically start. Build and flash succeeded; monitor was silent and no phase/STBY instrument capture was available.
- `FR-009`, `FR-010`, `FR-011` — Current command dispatcher and lifecycle start/stop/fault paths are wired in source; the fresh deployment produced no UART bytes, so MOTOR START, MOTOR STOP, and injected EN/FAULT timing were not exercised on hardware.

## UART Command & Status Interface

gate: reviewed · tasks: 5/5 done · recorded in: 305bfa04-5f10-49ba-ac62-2eb3829f110a

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-004` | Upon receiving the UART command "DIR CW", the firmware shall set the rotation direction to clockwise. | hardware / instrument | After transmitting `DIR CW`, the observed motor phase sequence is clockwise and the reported direction is CW. (expected: The phase sequence corresponds to clockwise rotation and STATUS reports `CW`.) | firmware/app/app.c<br>firmware/modules/command_parser/command_parser.c<br>firmware/modules/motor_waveform/motor_waveform.c | command_parser_parse, dispatch_command, motor_waveform_set_direction | — | implemented, untested |
| `IF-005` | Upon receiving the UART command "DIR CCW", the firmware shall set the rotation direction to counter-clockwise. | hardware / instrument | After transmitting `DIR CCW`, the observed motor phase sequence is counter-clockwise and the reported direction is CCW. (expected: The phase sequence corresponds to counter-clockwise rotation and STATUS reports `CCW`.) | firmware/app/app.c<br>firmware/modules/command_parser/command_parser.c<br>firmware/modules/motor_waveform/motor_waveform.c | command_parser_parse, dispatch_command, motor_waveform_set_direction | — | implemented, untested |
| `IF-006` | Upon receiving the UART command "FREQ <Hz>", the firmware shall set the electrical frequency to the specified value. | hardware / instrument | After transmitting `FREQ x`, the measured electrical frequency is x Hz by the next control update. (expected: Measured electrical frequency equals x Hz within the test instrument's stated measurement tolerance by the next control update.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/command_parser/command_parser.c | command_parser_parse, dispatch_command | — | implemented, untested |
| `IF-007` | Upon receiving the UART command "DUTY <%>", the firmware shall set the PWM amplitude to the specified value within the configured safe limit. | hardware / instrument | The effective PWM duty after a `DUTY x` command equals x percent for x at or below 20 and is no greater than 20 percent for x above 20. (expected: The first measurement is 10% within the measurement tolerance; the second measurement is less than or equal to 20%.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/command_parser/command_parser.c | command_parser_parse, dispatch_command | — | implemented, untested |
| `IF-008` | The STATUS response shall include the current motor state. | hardware / instrument | A `STATUS` response contains a field whose value unambiguously identifies the current motor state. (expected: The response contains a motor-state field with an unambiguous state value such as `Stopped`, `Running`, or `Ramping`.) | firmware/app/app.c<br>firmware/interfaces/uart_if.h<br>firmware/platforms/stm32/uart_stm32.c | report_status, uart_if_read_byte, uart_if_write | — | implemented, untested |
| `IF-009` | The STATUS response shall include the current rotation direction. | hardware / instrument | A `STATUS` response contains a direction field whose value is either CW or CCW and matches the configured direction. (expected: The response contains `CW` or `CCW` matching the configured direction.) | firmware/app/app.c<br>firmware/interfaces/uart_if.h<br>firmware/platforms/stm32/uart_stm32.c | report_status, uart_if_read_byte, uart_if_write | — | implemented, untested |
| `IF-010` | The STATUS response shall include the current electrical frequency. | hardware / instrument | A `STATUS` response contains a numeric electrical-frequency field in Hz matching the internal frequency setpoint or measurement. (expected: The response contains a numeric Hz value equal to the current internal setpoint or measurement within the defined measurement tolerance.) | firmware/app/app.c<br>firmware/interfaces/uart_if.h<br>firmware/platforms/stm32/uart_stm32.c | report_status, uart_if_read_byte, uart_if_write | — | implemented, untested |
| `IF-011` | The STATUS response shall include the current PWM duty. | hardware / instrument | A `STATUS` response contains a numeric PWM-duty field in percent matching the effective duty within measurement tolerance. (expected: The reported percentage matches the measured effective PWM duty within the test's stated measurement tolerance.) | firmware/app/app.c<br>firmware/interfaces/uart_if.h<br>firmware/platforms/stm32/uart_stm32.c | report_status, uart_if_read_byte, uart_if_write | — | implemented, untested |
| `IF-012` | The STATUS response shall include the driver fault status. | hardware / instrument | A `STATUS` response contains a driver-fault field that reflects whether the STSPIN830 EN/FAULT condition is active. (expected: The fault field indicates inactive in the first case and active in the second case.) | firmware/app/app.c<br>firmware/interfaces/uart_if.h<br>firmware/platforms/stm32/uart_stm32.c | report_status, uart_if_read_byte, uart_if_write | — | implemented, untested |
| `IF-013` | The firmware shall accept UART commands to set rotation direction using "DIR CW" and "DIR CCW". | hardware / instrument | Transmitting `DIR CW` and then `DIR CCW` updates the internal direction state to CW and then CCW, with each value appearing in the subsequent STATUS response. (expected: The first STATUS reports `CW` and the second STATUS reports `CCW`.) | firmware/app/app.c<br>firmware/modules/command_parser/command_parser.c<br>firmware/modules/motor_waveform/motor_waveform.c | command_parser_parse, dispatch_command, motor_waveform_set_direction | — | implemented, untested |
| `IF-014` | The firmware shall specify and use a fixed UART configuration (port, baud rate, data bits, parity, stop bits, and command delimiter) for the command interface so that the listed commands can be reliably exchanged. | inspection, hardware / instrument | The firmware configuration and interface documentation each specify the UART port, baud rate, data bits, parity, stop bits, and command delimiter, and the values agree. (expected: All six settings are explicitly specified in both places and are identical.)<br>Using the documented UART settings, a host transmitting `STATUS` receives a correctly formatted response containing the STATUS fields. (expected: A complete, correctly formatted STATUS response is received using the configured UART connection.)<br>A command sent with deliberately incorrect UART settings does not produce a valid command action or a partial motor/configuration change. (expected: No valid action is acknowledged and every motor/configuration field in STATUS remains equal to its baseline value.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/uart_stm32.c | uart_if_initialize | — | implemented, untested |
| `IF-015` | The firmware shall reject any unknown UART command with an explicit error response and shall not change motor state or configuration as a result of an unrecognized command. | hardware / instrument | An unrecognized command such as `FOO` produces an explicit error response and leaves all motor and configuration fields unchanged. (expected: The command response explicitly identifies an error, and every compared STATUS field is identical before and after `FOO`.) | firmware/app/app.c<br>firmware/modules/command_parser/command_parser.c | command_parser_parse, dispatch_command | — | implemented, untested |
| `IF-016` | The firmware shall validate the FREQ command argument and reject non-numeric, missing, or negative values with an error response; the electrical-frequency setpoint shall remain unchanged on rejection. | hardware / instrument | Each invalid command `FREQ abc` and `FREQ -1` produces an error response and leaves the reported electrical-frequency value unchanged. (expected: Both commands produce explicit error responses, and the frequency remains equal to the initial value after each command.)<br>A `FREQ` command with a missing argument produces an error response and does not alter the electrical-frequency setpoint. (expected: The response is an explicit error and the reported frequency equals the pre-command value.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/command_parser/command_parser.c | command_parser_parse, dispatch_command | — | implemented, untested |
| `IF-017` | The firmware shall validate the DUTY command argument and reject non-numeric, missing, or negative values with an error response; the PWM duty setting shall remain unchanged on rejection. | hardware / instrument | Each invalid command `DUTY abc` and `DUTY -5` produces an error response and leaves the reported PWM-duty value unchanged. (expected: Both commands produce explicit error responses, and the duty remains equal to the initial value after each command.)<br>A `DUTY` command with a missing argument produces an error response and does not alter the PWM-duty setting. (expected: The response is an explicit error and the reported duty equals the pre-command value.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/modules/command_parser/command_parser.c | command_parser_parse, dispatch_command | — | implemented, untested |
| `IF-019` | Upon receiving the UART command "STATUS", the firmware shall return a response within a reasonable time containing the current motor state, rotation direction, electrical frequency (Hz), PWM duty (%), and driver fault status. | hardware / instrument, not verifiable | A `STATUS` command returns one response containing the motor state, rotation direction, electrical frequency in Hz, PWM duty in percent, and driver-fault status. (expected: The response contains all five fields: motor state, direction, numeric electrical frequency in Hz, numeric PWM duty in percent, and driver-fault status.)<br>The STATUS response is returned within a reasonable time after the request. | firmware/app/app.c<br>firmware/interfaces/uart_if.h<br>firmware/platforms/stm32/uart_stm32.c | report_status, uart_if_read_byte, uart_if_write | — | implemented, untested |

Observed on the board:
- `IF-004`, `IF-005`, `IF-013` — Fresh USART2 deploy produced startup markers, confirming the VCP path. DIR commands were not injected during the available monitor session; phase order and physical reversal remain unverified.
- `IF-006`, `IF-007`, `IF-016`, `IF-017` — USART2 startup markers were captured, but FREQ/DUTY valid and invalid command responses were not injected or observed.
- `IF-008`, `IF-009`, `IF-010`, `IF-011`, `IF-012`, `IF-019` — Captured output: uart init ok, main uart probe, uart init ok, app start. No STATUS request was injected, so field contents and response timing remain unverified.
- `IF-015` — UART transmit path is active, but FOO and malformed-command responses were not injected during the review capture.

Notes recorded with these traces:
- `IF-014` — USART2 PA2/PA3, 115200 8N1, no flow control, and CR/LF are consistent in configuration, implementation, and documentation.

## Open-Loop Demonstration Scope & Validation

gate: not decided · tasks: 3/3 done · recorded in: 305bfa04-5f10-49ba-ac62-2eb3829f110a

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-004` | MOTOR START shall produce visible motor movement or rotation. | hardware / instrument | After issuing the MOTOR START command with the driver enabled and fault-free, the motor shaft exhibits visible movement or rotation within the test's defined reasonable-time limit. (expected: Visible shaft movement or rotation occurs before the reasonable-time limit expires.) | firmware/README.md<br>firmware/app/app.c<br>firmware/platforms/stm32/stm32_motor_board.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | dispatch_command, motor_board_apply_pwm, motor_board_set_enabled, motor_lifecycle_start | — | implemented, untested |
| `NFR-001` | The firmware shall demonstrate basic rotation to verify the MCU board, motor-driver board, PWM outputs, and motor connections. | hardware / instrument | A correctly assembled and powered target produces observable motor rotation during the demonstration, with the rotation driven by the configured PWM outputs. (expected: The motor rotates while PWM is being generated, and the demonstration completes with observable rotation.) | firmware/README.md<br>firmware/app/app.c<br>firmware/platforms/stm32/stm32_motor_board.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | dispatch_command, motor_board_apply_pwm, motor_board_set_enabled, motor_lifecycle_start | — | implemented, untested |
| `NFR-002` | Electrical frequency shall be used solely to create a rotating field and not as a closed-loop RPM setpoint. | inspection | The waveform-generation implementation applies the configured electrical frequency directly to the rotating-field waveform and contains no feedback path that adjusts that frequency from measured or commanded mechanical RPM. (expected: Electrical frequency is used as a direct waveform-generation input for the rotating field; no RPM feedback or closed-loop speed adjustment is present.) | firmware/README.md<br>firmware/app/app.c<br>firmware/modules/motor_waveform/motor_waveform.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | motor_lifecycle_process, motor_waveform_step, report_status | — | implemented, untested |
| `NFR-003` | The firmware shall not measure or guarantee actual mechanical RPM. | inspection | The firmware source and UART status interface contain no mechanical-RPM measurement, RPM estimate, or claim that the motor is operating at a guaranteed mechanical RPM. (expected: No RPM sensor or estimator is implemented, STATUS reports no mechanical RPM, and documentation makes no mechanical-RPM guarantee.) | firmware/README.md<br>firmware/app/app.c<br>firmware/modules/motor_waveform/motor_waveform.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | motor_lifecycle_process, motor_waveform_step, report_status | — | implemented, untested |
| `NFR-004` | The firmware shall not implement the following features: Sensorless FOC; Motor resistance, inductance or flux characterization; Rotor observer; Clarke/Park and inverse transforms; Current PI or speed PI controller; Closed-loop RPM control; Phase-current ADC control; VBUS ADC control; ADC-triggered DMA; Current-offset calibration; Oscilloscope integration; Automatic motor-parameter identification. | inspection | Code and configuration inspection finds no implementation or operational dependency on sensorless FOC, rotor observation, Clarke/Park or inverse transforms, current PI control, speed PI control, or closed-loop RPM control. (expected: None of the listed algorithms or control loops is implemented, invoked, or required by the demonstration path.)<br>Code and configuration inspection finds no motor-parameter characterization or automatic identification for resistance, inductance, flux, or other motor parameters. (expected: No listed characterization or automatic motor-parameter-identification routine is present or required for operation.)<br>Code and configuration inspection finds no phase-current ADC control, VBUS ADC control, ADC-triggered DMA, or current-offset calibration required by the demonstration. (expected: None of the listed ADC, DMA, or current-offset-calibration features is implemented or required for demonstration operation.)<br>Code and project inspection finds no oscilloscope integration and no external motor-parameter-identification dependency in the demonstration firmware. (expected: No oscilloscope integration is present, and the demonstration has no dependency on automatic motor-parameter identification or an external identification tool.) | CMakeLists.txt<br>firmware/modules/motor_waveform/motor_waveform.c<br>firmware/platforms/stm32/stm32_motor_board.c<br>firmware/services/motor_lifecycle/motor_lifecycle.c | motor_board_apply_pwm, motor_lifecycle_process, motor_waveform_step | — | implemented, untested |

Observed on the board:
- `FR-004`, `NFR-001` — Fresh USART2 deployment reached app start and produced startup markers. No MOTOR START command was injected and no motor shaft or PWM instrument observation was captured; visible rotation remains unverified.

Notes recorded with these traces:
- `NFR-002`, `NFR-003` — Inspection: electrical frequency directly advances waveform angle; no mechanical RPM measurement, estimate, or RPM STATUS field exists.
- `NFR-004` — Inspection found no excluded FOC, observer, transform, PI, characterization, ADC/DMA control, calibration, oscilloscope, or identification implementation or dependency.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### Host tests you could write (1)

These have a `host` acceptance criterion and cite no test, so they are the ones a unit test can actually cover -- and the only ones; see the next section. They do NOT run on the machine that generated this firmware, which carries the vendor cross-compilation toolchain and no host compiler. Run `test_handover export` and `sources/test_handover/` will hold this list machine-readably, with the criteria to assert, where each requirement's code lives, and which firmware layers compile standalone versus needing a mocked header.

- `FR-002` (Motor Startup, Lifecycle & Fault Safety)

### NOT unit-testable (45) — do not write host tests for these

No `host` criterion, so no unit test can cover them however it is written. Listed explicitly because the untested count above otherwise reads as a backlog of unit tests owed -- and closing one of these with a host test means inventing an assertion that passes without checking the thing. Each line says how it IS checked instead.

- `HW-001` — checked by hardware / instrument · _Target Hardware & Build Configuration_
- `HW-002` — checked by hardware / instrument · _Target Hardware & Build Configuration_
- `HW-003` — checked by inspection · _Target Hardware & Build Configuration_
- `HW-004` — checked by hardware / instrument · _Target Hardware & Build Configuration_
- `HW-005` — checked by inspection · _Target Hardware & Build Configuration_
- `NFR-005` — checked by inspection, hardware / instrument · _Target Hardware & Build Configuration_
- `IF-001` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `IF-002` — checked by inspection · _Motor Driver & PWM Generation_
- `FR-001` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `IF-003` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `PERF-001` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `PERF-002` — checked by on-target test · _Motor Driver & PWM Generation_
- `PERF-003` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `PERF-004` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `PERF-005` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `FR-012` — checked by hardware / instrument · _Motor Driver & PWM Generation_
- `FR-003` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-005` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-006` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-007` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `IF-018` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-008` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-009` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-010` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `FR-011` — checked by hardware / instrument · _Motor Startup, Lifecycle & Fault Safety_
- `IF-004` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-005` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-006` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-007` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-008` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-009` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-010` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-011` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-012` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-013` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-014` — checked by inspection, hardware / instrument · _UART Command & Status Interface_
- `IF-015` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-016` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-017` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-019` — checked by hardware / instrument, not verifiable · _UART Command & Status Interface_
- `FR-004` — checked by hardware / instrument · _Open-Loop Demonstration Scope & Validation_
- `NFR-001` — checked by hardware / instrument · _Open-Loop Demonstration Scope & Validation_
- `NFR-002` — checked by inspection · _Open-Loop Demonstration Scope & Validation_
- `NFR-003` — checked by inspection · _Open-Loop Demonstration Scope & Validation_
- `NFR-004` — checked by inspection · _Open-Loop Demonstration Scope & Validation_

### Board checks owed (7)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `PERF-001` (Motor Driver & PWM Generation)
- `PERF-002` (Motor Driver & PWM Generation)
- `PERF-003` (Motor Driver & PWM Generation)
- `PERF-004` (Motor Driver & PWM Generation)
- `PERF-005` (Motor Driver & PWM Generation)
- `FR-012` (Motor Driver & PWM Generation)
- `IF-014` (UART Command & Status Interface)

### Implemented, never tested (46)

Code exists and no test is recorded against it.

- `HW-001` (Target Hardware & Build Configuration)
- `HW-002` (Target Hardware & Build Configuration)
- `HW-003` (Target Hardware & Build Configuration)
- `HW-004` (Target Hardware & Build Configuration)
- `HW-005` (Target Hardware & Build Configuration)
- `NFR-005` (Target Hardware & Build Configuration)
- `IF-001` (Motor Driver & PWM Generation)
- `IF-002` (Motor Driver & PWM Generation)
- `FR-001` (Motor Driver & PWM Generation)
- `IF-003` (Motor Driver & PWM Generation)
- `PERF-001` (Motor Driver & PWM Generation)
- `PERF-002` (Motor Driver & PWM Generation)
- `PERF-003` (Motor Driver & PWM Generation)
- `PERF-004` (Motor Driver & PWM Generation)
- `PERF-005` (Motor Driver & PWM Generation)
- `FR-012` (Motor Driver & PWM Generation)
- `FR-002` (Motor Startup, Lifecycle & Fault Safety)
- `FR-003` (Motor Startup, Lifecycle & Fault Safety)
- `FR-005` (Motor Startup, Lifecycle & Fault Safety)
- `FR-006` (Motor Startup, Lifecycle & Fault Safety)
- `FR-007` (Motor Startup, Lifecycle & Fault Safety)
- `IF-018` (Motor Startup, Lifecycle & Fault Safety)
- `FR-008` (Motor Startup, Lifecycle & Fault Safety)
- `FR-009` (Motor Startup, Lifecycle & Fault Safety)
- `FR-010` (Motor Startup, Lifecycle & Fault Safety)
- `FR-011` (Motor Startup, Lifecycle & Fault Safety)
- `IF-004` (UART Command & Status Interface)
- `IF-005` (UART Command & Status Interface)
- `IF-006` (UART Command & Status Interface)
- `IF-007` (UART Command & Status Interface)
- `IF-008` (UART Command & Status Interface)
- `IF-009` (UART Command & Status Interface)
- `IF-010` (UART Command & Status Interface)
- `IF-011` (UART Command & Status Interface)
- `IF-012` (UART Command & Status Interface)
- `IF-013` (UART Command & Status Interface)
- `IF-014` (UART Command & Status Interface)
- `IF-015` (UART Command & Status Interface)
- `IF-016` (UART Command & Status Interface)
- `IF-017` (UART Command & Status Interface)
- `IF-019` (UART Command & Status Interface)
- `FR-004` (Open-Loop Demonstration Scope & Validation)
- `NFR-001` (Open-Loop Demonstration Scope & Validation)
- `NFR-002` (Open-Loop Demonstration Scope & Validation)
- `NFR-003` (Open-Loop Demonstration Scope & Validation)
- `NFR-004` (Open-Loop Demonstration Scope & Validation)

### No observable exists (1)

These need a human decision — either the requirement is restated as something checkable, or it is accepted as unverifiable on the record. Inventing a test for one of these produces a test that passes and proves nothing.

- `IF-019` — The STATUS response is returned within a reasonable time after the request. · _The requirement does not define a maximum response time or other measurable bound for 'reasonable time'._
