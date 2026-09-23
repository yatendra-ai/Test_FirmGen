# Requirement traceability

Generated 2026-09-23T13:50:24.455984600+00:00
Specification frozen 2026-09-23T12:24:48.066Z

39 of 39 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 39 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project. Host tests run on a separate machine; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## MCU, Inverter & Motor Hardware Configuration

gate: deferred · tasks: 4/4 done · recorded in: 4f4e8899-c422-4086-a97c-72b9121765de

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-001` | The firmware shall target the NUCLEO-G431RB MCU board. | inspection | The build configuration and target documentation identify NUCLEO-G431RB as the firmware target, and the recorded target test uses that board. (expected: The target is identified as NUCLEO-G431RB in both the build configuration and the target test record.) | CMakeLists.txt<br>firmware/README.md<br>firmware/configs/app_config.h | main | — | implemented, untested |
| `HW-002` | The system shall use the X-NUCLEO-IHM16M1 motor-driver board (STSPIN830). | inspection | The motor-driver board listed in the hardware configuration and test setup is X-NUCLEO-IHM16M1 populated with the STSPIN830. (expected: The installed and documented motor-driver board is X-NUCLEO-IHM16M1 with an STSPIN830.) | firmware/README.md<br>firmware/bsp/board.h<br>firmware/configs/app_config.h | board_name | — | implemented, untested |
| `HW-003` | The target motor shall be the GBM2804H-100T gimbal motor with 7 pole pairs. | inspection | The motor identified in the test setup and documentation is GBM2804H-100T and its configured pole-pair count is 7. (expected: The motor model is GBM2804H-100T and the pole-pair parameter equals 7.) | firmware/README.md<br>firmware/bsp/board.h<br>firmware/configs/app_config.h | board_name | — | implemented, untested |
| `HW-004` | The system supply shall be 12 V DC. | inspection | The documented laboratory supply setting for the system is 12 V DC. (expected: The supply is DC and its set voltage is 12 V.) | firmware/README.md<br>firmware/bsp/board.h<br>firmware/configs/app_config.h | board_name | — | implemented, untested |
| `IF-001` | The firmware shall drive the three-phase motor via U, V, and W outputs to the motor connection. | hardware / instrument | When a run command is applied, PWM activity is observable on each of the U, V, and W motor outputs. (expected: Each of U, V, and W has a continuous PWM waveform during the commanded run.)<br>When a motor-control fault is injected or detected, the U, V, and W drive outputs are disabled. (expected: PWM is disabled on U, V, and W after the fault is detected.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/ihm16m1_platform.c | ihm16m1_platform_pwm_enable, ihm16m1_platform_pwm_set, ihm16m1_platform_start | — | implemented, untested |
| `IF-002` | The firmware shall measure motor phase currents using the X-NUCLEO-IHM16M1 three-shunt current-sensing circuitry. | hardware / instrument | During motor operation, the firmware reports phase-current samples from the IHM16M1 three-shunt sensing network, and the reported samples vary when motor load or phase conduction changes. (expected: The samples are sourced from the three-shunt ADC channels and at least one phase-current sample changes when load or phase conduction changes.)<br>When phase-current sensing is made unavailable, the motor-drive outputs are disabled. (expected: Motor-drive PWM is disabled after current sensing becomes unavailable.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/ihm16m1_platform.c | ihm16m1_platform_adc_read | — | implemented, untested |
| `IF-004` | The firmware shall use MCU hardware PWM generation for the three-phase inverter drive signals. | inspection, hardware / instrument | The firmware's three-phase drive implementation selects MCU hardware PWM peripherals rather than software-toggled GPIO timing. (expected: All three inverter phases are assigned to MCU hardware PWM channels, with duty controlled through PWM peripheral registers.)<br>During steady motor operation, the three inverter PWM waveforms remain continuously hardware-timed without observable CPU-jitter artifacts. (expected: PWM pulses remain present for the full capture and their period and edge timing remain hardware-timed without missing pulses or observable CPU-scheduling jitter artifacts.)<br>When a detected inverter or motor-control fault occurs, the hardware PWM outputs are disabled. (expected: PWM output is disabled on all three inverter phases after fault detection.) | firmware/app/app.c<br>firmware/configs/app_config.h<br>firmware/platforms/stm32/ihm16m1_platform.c | ihm16m1_platform_pwm_enable, ihm16m1_platform_pwm_set, ihm16m1_platform_start | — | implemented, untested |

Notes recorded with these traces:
- `HW-001` — Build targets STM32G431xx and project documentation identifies NUCLEO-G431RB; target inspection is grounded by the detected board but not yet flashed.
- `HW-002`, `HW-003`, `HW-004` — Configuration and README record X-NUCLEO-IHM16M1/STSPIN830, GBM2804H-100T, 7 pole pairs, and 12 V DC.
- `IF-001`, `IF-004` — TIM1 CH1/CH2/CH3 hardware PWM is configured for U/V/W in three-PWM mode; startup leaves compare values at zero and the driver in standby.
- `IF-002` — Three ADC channel mappings and callback reads are implemented; raw samples are returned because the supplied manual does not establish a complete conversion formula.

## Sensorless FOC & Motor Operating Control

gate: deferred · tasks: 6/6 done · recorded in: 14056c19-f482-4c45-b04c-ad9ef894553b

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The firmware shall implement sensorless Field-Oriented Control (FOC) for three-phase motor control. | hardware / instrument | The motor operates under closed-loop sensorless FOC with valid commutation and torque control throughout the specified speed range. (expected: Closed-loop sensorless commutation remains synchronized and commanded torque is controlled at every tested speed in the specified range.)<br>A failed-to-converge sensorless observer causes the motor drive to be disabled. (expected: An estimator failure is indicated and all motor-drive PWM outputs are disabled.) | firmware/services/motor_control/motor_control.c<br>firmware/services/motor_control/motor_control.h | motor_control_apply_duty, motor_control_process | — | implemented, untested |
| `FR-002` | After system initialization, the motor shall remain stopped until a valid START command is received. | hardware / instrument | Immediately after power-on or reset, before any START command, the motor is stationary and all motor PWM outputs are disabled. (expected: The shaft does not rotate and all motor PWM outputs remain disabled until START is received.) | firmware/app/app.c<br>firmware/services/motor_control/motor_control.c | motor_control_init, motor_control_process, motor_control_start | — | implemented, untested |
| `FR-003` | The firmware shall support clockwise and counter-clockwise motor rotation as commanded. | hardware / instrument | The measured shaft rotation direction matches each commanded direction for both CW and CCW commands. (expected: The first run rotates clockwise and the second run rotates counter-clockwise, with no direction-command mismatch.) | firmware/services/motor_control/motor_control.c<br>firmware/services/motor_control/motor_control.h | motor_control_set_direction, motor_control_set_speed, motor_control_stop | — | implemented, untested |
| `FR-009` | The firmware shall allow changing the target speed while the motor is running without requiring a stop. | hardware / instrument | A valid SPEED command issued during running changes the motor to the new target speed without entering the stopped state. (expected: The commanded speed changes to the new value, the motor remains in the run state throughout the transition, and no stop state occurs between the two speeds.) | firmware/services/motor_control/motor_control.c<br>firmware/services/motor_control/motor_control.h | motor_control_set_direction, motor_control_set_speed, motor_control_stop | — | implemented, untested |
| `FR-010` | If a direction change is commanded while the motor is running, the firmware shall perform a controlled deceleration to zero speed before applying torque in the new direction. | hardware / instrument | A running CW motor commanded to reverse decelerates through zero before any reverse torque is applied and then accelerates CCW. (expected: Signed speed reaches 0 RPM before torque changes to the CCW direction; the motor subsequently accelerates CCW, and no reverse-torque spike occurs before the zero-speed crossing.)<br>If the controller cannot complete the commanded reversal sequence, it rejects the direction change and continues operating in the current direction. (expected: The opposite-direction command is refused and the motor remains in its original direction.) | firmware/services/motor_control/motor_control.c<br>firmware/services/motor_control/motor_control.h | motor_control_set_direction, motor_control_set_speed, motor_control_stop | — | implemented, untested |
| `FR-011` | If the motor fails to achieve sustained rotation within a bounded startup time or number of attempts from standstill, the firmware shall declare a startup fault and disable PWM. | hardware / instrument | When startup is prevented, the controller aborts within the configured maximum startup time or attempt count, disables PWM, and reports a startup fault in STATUS. (expected: The abort occurs no later than the configured maximum startup time or attempt count; STATUS indicates a startup fault and all PWM outputs are disabled.)<br>After a startup fault, the drive remains disabled until the fault is cleared and a new START command is issued. (expected: The fault remains latched and PWM remains disabled before clear plus new START; only after both actions may the drive leave the fault-disabled state.) | firmware/app/app.c<br>firmware/services/motor_control/motor_control.c | motor_control_init, motor_control_process, motor_control_start | — | implemented, untested |
| `PERF-001` | The system shall achieve reliable motor startup from standstill. | hardware / instrument | Every startup attempt in a representative series reaches the running state and sustained rotation without manual intervention or a fault. (expected: Each attempt reaches the running state with sustained rotation, no manual intervention, and no fault indication.) | firmware/configs/app_config.h<br>firmware/services/motor_control/motor_control.c | motor_control_process | — | implemented, untested |
| `PERF-002` | The system shall maintain stable rotation at the commanded speed. | hardware / instrument | With a fixed SPEED command and constant load, the motor remains synchronized and STATUS reports an approximately constant estimated speed throughout the observation interval. (expected: No loss of synchronism occurs and the STATUS estimated-speed samples remain approximately constant for the full observation interval.) | firmware/configs/app_config.h<br>firmware/services/motor_control/motor_control.c | motor_control_process | — | implemented, untested |
| `PERF-003` | The system shall perform a controlled motor stop when stopping is commanded. | hardware / instrument | A stop command decelerates the motor to standstill without oscillation or reverse kick and leaves the drive disabled. (expected: Speed decreases to 0 RPM, no uncontrolled oscillation or reverse kick is observed, the motor is stopped, and all drive PWM outputs are disabled.) | firmware/configs/app_config.h<br>firmware/services/motor_control/motor_control.c | motor_control_process | — | implemented, untested |
| `NFR-001` | Final speed, startup, and current limits shall be tuned and validated on the actual motor and supply before extended operation. | inspection | Project artifacts contain the final speed, startup, and current tuning parameters together with validation results for the actual motor and supply, dated or otherwise attributable before extended runtime testing. (expected: All three limit groups have recorded final values, the records identify the actual motor and supply, validation results are present, and the validation predates extended runtime testing.) | firmware/README.md<br>firmware/configs/app_config.h | APP_MOTOR_ALIGN_TIME_MS, APP_MOTOR_MAX_SPEED_RPM, APP_MOTOR_MIN_SPEED_RPM, APP_MOTOR_STARTUP_TIMEOUT_MS | — | implemented, untested |

Notes recorded with these traces:
- `FR-001` — A bounded control state machine and three-phase duty output path are implemented, but true closed-loop sensorless FOC/observer feedback is not implemented; the observer-unavailable path intentionally faults before RUN.
- `FR-002`, `FR-011` — Initialization disables PWM and enters STOPPED; START is not invoked by the current app; startup has a bounded timeout and fault state.
- `FR-003`, `FR-009`, `FR-010` — Signed direction, bounded speed setters, and a deceleration transition are present, but no UART command path or hardware validation exists.
- `PERF-001`, `PERF-002`, `PERF-003` — Startup/stop timing contracts and safe PWM shutdown path exist; performance cannot be claimed because the observer is unavailable and no motor run was observed.
- `NFR-001` — Configuration records commissioning defaults and identifies the actual motor/supply, but validation results and final tuned values are not present.

## UART Command & Status Interface

gate: deferred · tasks: 4/4 done · recorded in: 14056c19-f482-4c45-b04c-ad9ef894553b

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-003` | The firmware shall provide a UART interface configured for 115200 baud, 8 data bits, no parity, 1 stop bit (8-N-1). | inspection, hardware / instrument | The UART configuration in the firmware build is set to 115200 baud with 8 data bits, no parity, and 1 stop bit. (expected: 115200 baud, 8 data bits, no parity, and 1 stop bit (8-N-1).)<br>A terminal or UART loopback exchanges command and response data successfully using the configured interface. (expected: The transmitted command is received by the firmware and a valid response is received without framing or communication errors.) | firmware/platforms/stm32/uart_platform.c | uart_platform_start | — | implemented, untested |
| `FR-004` | Upon receiving "MOTOR START" over UART, the firmware shall start the motor and enter the run state. | hardware / instrument | A valid MOTOR START command causes the motor to rotate and the subsequent STATUS response to report the run state. (expected: The motor is rotating and STATUS reports RUN or the project-defined equivalent run state.)<br>A MOTOR START command issued while a stated start-safety condition is violated does not start the motor. (expected: The motor remains stopped and the system does not enter RUN; the response/state indicates the project-defined safety refusal or fault behavior.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `FR-005` | Upon receiving "MOTOR STOP" over UART, the firmware shall stop the motor. | hardware / instrument | A MOTOR STOP command issued while the motor is running leaves the motor stationary and disables the PWM outputs at the end of the stop sequence. (expected: The shaft is stationary and all motor-drive PWM outputs are disabled after the stop sequence completes.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `FR-006` | Upon receiving "SPEED <RPM>" over UART, the firmware shall set the target speed to the specified value within the allowed range of 300 to 1000 RPM. | hardware / instrument | SPEED commands for 300 RPM, 1000 RPM, and a valid intermediate RPM update the target-speed field to the commanded value. (expected: The reported target speeds are respectively 300 RPM, 1000 RPM, and exactly the commanded intermediate value.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `FR-007` | Upon receiving "DIR CW" over UART, the firmware shall set the commanded direction to clockwise. | hardware / instrument | A DIR CW command causes the commanded direction field to become CW and subsequent motor rotation to be clockwise. (expected: STATUS reports direction=CW and the motor rotates clockwise.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `FR-008` | Upon receiving "DIR CCW" over UART, the firmware shall set the commanded direction to counter-clockwise. | hardware / instrument | A DIR CCW command causes the commanded direction field to become CCW and subsequent motor rotation to be counter-clockwise. (expected: STATUS reports direction=CCW and the motor rotates counter-clockwise.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `IF-005` | The firmware shall implement the STATUS command over UART to report the system status. | hardware / instrument | Sending STATUS over an established UART link produces a response payload containing the system state, target speed, estimated actual speed, commanded direction, and fault status fields. (expected: One response payload is received and contains fields for state, target speed, estimated actual speed, direction, and fault status.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |
| `IF-006` | The firmware shall ignore unrecognized or malformed UART commands without changing the system state. | hardware / instrument | Each unknown, malformed, or truncated UART command leaves the system state, target speed, direction, and fault status identical to their values before the command. (expected: For every invalid input, state, target speed, direction, and fault status are unchanged.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `IF-007` | The firmware shall reject SPEED commands outside 300–1000 RPM and shall not change the target speed in response. | hardware / instrument | SPEED 200 and SPEED 1100 are rejected without changing the previously valid target speed or system state. (expected: After each out-of-range command, the target speed equals the prior valid value and the system state is unchanged.) | firmware/platforms/stm32/uart_platform.c<br>firmware/services/uart_command/uart_command.c | uart_command_parse_speed, uart_command_process, uart_platform_read_line | — | implemented, untested |
| `DIAG-001` | The STATUS command shall include the current system state. | hardware / instrument | The STATUS response contains a state field whose value unambiguously identifies the current system state. (expected: The response contains a non-ambiguous state value matching the actual state, such as INIT, STOPPED, RUN, or FAULT.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |
| `DIAG-002` | The STATUS command shall include the target speed. | hardware / instrument | The target-speed field in STATUS equals the most recently commanded valid target speed. (expected: The field is numeric and equals the commanded value in RPM, within the inclusive range 300–1000 RPM.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |
| `DIAG-003` | The STATUS command shall include the estimated actual speed. | hardware / instrument | The STATUS response contains a numeric estimated-actual-speed field expressed in RPM. (expected: The response contains a parseable numeric value representing estimated speed in RPM at the query time.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |
| `DIAG-004` | The STATUS command shall include the current commanded direction (CW or CCW). | hardware / instrument | The direction field in STATUS is either CW or CCW and matches the most recent DIR command. (expected: The direction field reports CW after DIR CW and CCW after DIR CCW.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |
| `DIAG-005` | The STATUS command shall include the current fault status. | hardware / instrument | The STATUS response contains a fault-status field that changes to the faulted indication when a known fault is induced. (expected: The response contains a fault-status field, and during the induced fault it indicates that a fault is present.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |
| `DIAG-006` | When a fault is present, the STATUS response shall include a fault code that identifies the fault category (e.g., undervoltage limit, overcurrent limit, sensor fault, startup failure). | hardware / instrument | When each representative fault is active, STATUS reports a non-null fault code matching that fault category. (expected: Each STATUS response contains a non-null code, and the codes identify respectively undervoltage-limit, overcurrent-limit, current-sense, and startup-failure categories.) | firmware/services/motor_control/motor_control.c<br>firmware/services/uart_command/uart_command.c | uart_command_direction_name, uart_command_state_name, uart_command_status | — | implemented, untested |

Notes recorded with these traces:
- `IF-003` — LPUART1 is configured in source for 115200 baud, 8 data bits, no parity, one stop bit; hardware exchange was not verified because the monitor received no bytes.
- `FR-004`, `FR-005`, `FR-006`, `FR-007`, `FR-008`, `IF-006`, `IF-007` — Strict exact command matching and bounded line reception are implemented and dispatch to the existing motor-control API; hardware command behavior was not observed.
- `IF-005`, `DIAG-001`, `DIAG-002`, `DIAG-003`, `DIAG-004`, `DIAG-005`, `DIAG-006` — STATUS formatting includes state, target RPM, estimated RPM, direction, fault indication, and code; no UART response was captured on hardware.

## Drive Safety, Fault Handling & Reset Protection

gate: deferred · tasks: 5/5 done · recorded in: 14056c19-f482-4c45-b04c-ad9ef894553b

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `SF-001` | The firmware shall limit motor current so that RMS current does not exceed 1.5 A RMS or a lower validated project limit. | hardware / instrument | Measured steady-state and transient phase RMS motor current remains at or below the configured maximum, with the configured maximum no greater than 1.5 A RMS unless a lower validated project limit is configured. (expected: For every recorded interval, measured phase RMS current is <= the configured limit; the configured limit is <=1.5 A RMS unless a lower validated limit is explicitly configured.) | firmware/configs/app_config.h<br>firmware/services/drive_safety/drive_safety.c<br>firmware/services/drive_safety/drive_safety.h<br>firmware/services/motor_control/motor_control.c | drive_safety_validate, motor_control_process | — | implemented, untested |
| `SF-002` | On detection of a motor-control fault, the firmware shall immediately disable PWM output to the inverter. | hardware / instrument, on-target test | When a detectable motor-driver fault is asserted while the motor is running, all inverter PWM outputs enter the disabled state and produce no subsequent drive pulses. (expected: PWM outputs transition to the disabled state and no drive pulse occurs after the fault assertion.)<br>After a motor-control fault disables PWM, the firmware remains in the disabled condition until the fault is cleared and a new START command is received. (expected: PWM remains disabled before the new START command and may be enabled only after the fault is cleared and the new START command is received.) | firmware/app/app.c<br>firmware/platforms/stm32/ihm16m1_platform.c<br>firmware/services/motor_control/motor_control.c | ihm16m1_platform_pwm_enable, motor_control_disable_pwm, motor_control_init | — | implemented, untested |
| `SF-003` | The system shall not operate the motor outside the configured voltage and current limits. | hardware / instrument | An attempted start or continued run with either a configured voltage-limit violation or a configured current-limit violation results in drive inhibition or automatic stop with PWM disabled. (expected: For every out-of-limit condition, START is refused or an already-running motor is stopped, PWM is disabled, and fault status is reported.) | firmware/configs/app_config.h<br>firmware/services/drive_safety/drive_safety.c<br>firmware/services/drive_safety/drive_safety.h<br>firmware/services/motor_control/motor_control.c | drive_safety_validate, motor_control_process | — | implemented, untested |
| `SF-004` | The firmware shall prevent motor start and shall stop the motor and disable PWM when the measured DC supply voltage is outside the configured allowable range. | hardware / instrument, on-target test | A START command issued while measured DC supply voltage is below the configured minimum or above the configured maximum is refused and leaves the drive disabled. (expected: For both below-minimum and above-maximum conditions, START is refused and PWM remains disabled.)<br>When the measured DC supply voltage crosses either configured threshold while the motor is running, the drive is disabled and STATUS indicates a fault. (expected: For each threshold crossing, PWM is disabled, the motor is no longer driven, and STATUS indicates a fault.)<br>After a voltage-limit shutdown, the drive remains disabled while the measured voltage is out of range and does not restart until the voltage is valid and a new START command is received. (expected: PWM remains disabled while voltage is out of range and after voltage recovery without START; it can resume only after valid voltage and a new START command.) | firmware/configs/app_config.h<br>firmware/services/drive_safety/drive_safety.c<br>firmware/services/drive_safety/drive_safety.h<br>firmware/services/motor_control/motor_control.c | drive_safety_validate, motor_control_process | — | implemented, untested |
| `SF-005` | The firmware shall detect loss or out-of-range behavior of phase current measurements (e.g., disconnection, saturation, rail) and immediately disable PWM, entering a fault state. | hardware / instrument, on-target test | While running closed-loop control, inducing a phase-current sensing disconnection, ADC rail or saturation value, or ADC-DMA disable causes PWM to be disabled with no subsequent drive pulses and STATUS to indicate a faulted state. (expected: For every induced sensor fault, PWM is disabled, no drive pulse occurs after fault detection/assertion, and STATUS indicates the FAULT state.)<br>After a phase-current sensing fault is cleared, the firmware keeps the drive disabled until START is issued again. (expected: The system remains in FAULT or an equivalent non-driving state with PWM disabled before START, and does not resume driving until START is issued.) | firmware/configs/app_config.h<br>firmware/services/drive_safety/drive_safety.c<br>firmware/services/drive_safety/drive_safety.h<br>firmware/services/motor_control/motor_control.c | drive_safety_validate, motor_control_process | — | implemented, untested |
| `SF-006` | On any MCU reset, watchdog reset, or brown-out event, the firmware shall ensure PWM outputs are disabled and the motor remains stopped until initialization completes and a valid START command is received. | hardware / instrument, on-target test | Following an induced watchdog reset or brown-out while the motor is running, PWM outputs remain inactive during reset and after reboot until a new valid START command is received. (expected: No PWM activity is present during either reset or reboot, no PWM appears before the new valid START command, and PWM may resume only after initialization and START.)<br>After either a watchdog reset or brown-out reboot, STATUS reports STOPPED before any new START command is received. (expected: STATUS is STOPPED for both watchdog-reset and brown-out recovery before any START command.) | firmware/app/app.c<br>firmware/platforms/stm32/ihm16m1_platform.c<br>firmware/services/motor_control/motor_control.c | ihm16m1_platform_pwm_enable, motor_control_disable_pwm, motor_control_init | — | implemented, untested |
| `NFR-002` | The firmware shall enable a watchdog that resets the MCU if the control software fails to service it within a configured interval, ensuring fail-safe shutdown of the drive on software hang. | hardware / instrument | A deliberate control-software hang while the motor is running causes the watchdog to reset the MCU, after which the motor remains stopped with PWM disabled until a new START command is received. (expected: The MCU resets after the control software stops servicing the watchdog; PWM is disabled after reset, the motor remains stopped before START, and a new START is required before operation resumes.) | firmware/app/app.c<br>firmware/platforms/stm32/watchdog_platform.c | watchdog_platform_kick, watchdog_platform_start | — | implemented, untested |

Notes recorded with these traces:
- `SF-001`, `SF-003`, `SF-004`, `SF-005` — Safety validation is now reachable through the optional motor-control safety callback seam, but app_start currently supplies NULL because the project lacks validated bus-voltage/current conversion and sampling integration. Raw plausibility thresholds are configured; hardware limits are not verified.
- `SF-002`, `SF-006` — PWM compare values are cleared and STBY asserted on shutdown and initialization; reset and fault behavior were not instrumented on hardware.
- `NFR-002` — Independent watchdog is initialized and serviced in the main loop; deliberate watchdog reset and post-reset PWM behavior were not hardware-verified.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### NOT unit-testable (39) — do not write host tests for these

No `host` criterion, so no unit test can cover them however it is written. Listed explicitly because the untested count above otherwise reads as a backlog of unit tests owed -- and closing one of these with a host test means inventing an assertion that passes without checking the thing. Each line says how it IS checked instead.

- `HW-001` — checked by inspection · _MCU, Inverter & Motor Hardware Configuration_
- `HW-002` — checked by inspection · _MCU, Inverter & Motor Hardware Configuration_
- `HW-003` — checked by inspection · _MCU, Inverter & Motor Hardware Configuration_
- `HW-004` — checked by inspection · _MCU, Inverter & Motor Hardware Configuration_
- `IF-001` — checked by hardware / instrument · _MCU, Inverter & Motor Hardware Configuration_
- `IF-002` — checked by hardware / instrument · _MCU, Inverter & Motor Hardware Configuration_
- `IF-004` — checked by inspection, hardware / instrument · _MCU, Inverter & Motor Hardware Configuration_
- `FR-001` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `FR-002` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `FR-003` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `FR-009` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `FR-010` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `FR-011` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `PERF-001` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `PERF-002` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `PERF-003` — checked by hardware / instrument · _Sensorless FOC & Motor Operating Control_
- `NFR-001` — checked by inspection · _Sensorless FOC & Motor Operating Control_
- `IF-003` — checked by inspection, hardware / instrument · _UART Command & Status Interface_
- `FR-004` — checked by hardware / instrument · _UART Command & Status Interface_
- `FR-005` — checked by hardware / instrument · _UART Command & Status Interface_
- `FR-006` — checked by hardware / instrument · _UART Command & Status Interface_
- `FR-007` — checked by hardware / instrument · _UART Command & Status Interface_
- `FR-008` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-005` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-006` — checked by hardware / instrument · _UART Command & Status Interface_
- `IF-007` — checked by hardware / instrument · _UART Command & Status Interface_
- `DIAG-001` — checked by hardware / instrument · _UART Command & Status Interface_
- `DIAG-002` — checked by hardware / instrument · _UART Command & Status Interface_
- `DIAG-003` — checked by hardware / instrument · _UART Command & Status Interface_
- `DIAG-004` — checked by hardware / instrument · _UART Command & Status Interface_
- `DIAG-005` — checked by hardware / instrument · _UART Command & Status Interface_
- `DIAG-006` — checked by hardware / instrument · _UART Command & Status Interface_
- `SF-001` — checked by hardware / instrument · _Drive Safety, Fault Handling & Reset Protection_
- `SF-002` — checked by hardware / instrument, on-target test · _Drive Safety, Fault Handling & Reset Protection_
- `SF-003` — checked by hardware / instrument · _Drive Safety, Fault Handling & Reset Protection_
- `SF-004` — checked by hardware / instrument, on-target test · _Drive Safety, Fault Handling & Reset Protection_
- `SF-005` — checked by hardware / instrument, on-target test · _Drive Safety, Fault Handling & Reset Protection_
- `SF-006` — checked by hardware / instrument, on-target test · _Drive Safety, Fault Handling & Reset Protection_
- `NFR-002` — checked by hardware / instrument · _Drive Safety, Fault Handling & Reset Protection_

### Board checks owed (34)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `IF-001` (MCU, Inverter & Motor Hardware Configuration)
- `IF-002` (MCU, Inverter & Motor Hardware Configuration)
- `IF-004` (MCU, Inverter & Motor Hardware Configuration)
- `FR-001` (Sensorless FOC & Motor Operating Control)
- `FR-002` (Sensorless FOC & Motor Operating Control)
- `FR-003` (Sensorless FOC & Motor Operating Control)
- `FR-009` (Sensorless FOC & Motor Operating Control)
- `FR-010` (Sensorless FOC & Motor Operating Control)
- `FR-011` (Sensorless FOC & Motor Operating Control)
- `PERF-001` (Sensorless FOC & Motor Operating Control)
- `PERF-002` (Sensorless FOC & Motor Operating Control)
- `PERF-003` (Sensorless FOC & Motor Operating Control)
- `IF-003` (UART Command & Status Interface)
- `FR-004` (UART Command & Status Interface)
- `FR-005` (UART Command & Status Interface)
- `FR-006` (UART Command & Status Interface)
- `FR-007` (UART Command & Status Interface)
- `FR-008` (UART Command & Status Interface)
- `IF-005` (UART Command & Status Interface)
- `IF-006` (UART Command & Status Interface)
- `IF-007` (UART Command & Status Interface)
- `DIAG-001` (UART Command & Status Interface)
- `DIAG-002` (UART Command & Status Interface)
- `DIAG-003` (UART Command & Status Interface)
- `DIAG-004` (UART Command & Status Interface)
- `DIAG-005` (UART Command & Status Interface)
- `DIAG-006` (UART Command & Status Interface)
- `SF-001` (Drive Safety, Fault Handling & Reset Protection)
- `SF-002` (Drive Safety, Fault Handling & Reset Protection)
- `SF-003` (Drive Safety, Fault Handling & Reset Protection)
- `SF-004` (Drive Safety, Fault Handling & Reset Protection)
- `SF-005` (Drive Safety, Fault Handling & Reset Protection)
- `SF-006` (Drive Safety, Fault Handling & Reset Protection)
- `NFR-002` (Drive Safety, Fault Handling & Reset Protection)

### Implemented, never tested (39)

Code exists and no test is recorded against it.

- `HW-001` (MCU, Inverter & Motor Hardware Configuration)
- `HW-002` (MCU, Inverter & Motor Hardware Configuration)
- `HW-003` (MCU, Inverter & Motor Hardware Configuration)
- `HW-004` (MCU, Inverter & Motor Hardware Configuration)
- `IF-001` (MCU, Inverter & Motor Hardware Configuration)
- `IF-002` (MCU, Inverter & Motor Hardware Configuration)
- `IF-004` (MCU, Inverter & Motor Hardware Configuration)
- `FR-001` (Sensorless FOC & Motor Operating Control)
- `FR-002` (Sensorless FOC & Motor Operating Control)
- `FR-003` (Sensorless FOC & Motor Operating Control)
- `FR-009` (Sensorless FOC & Motor Operating Control)
- `FR-010` (Sensorless FOC & Motor Operating Control)
- `FR-011` (Sensorless FOC & Motor Operating Control)
- `PERF-001` (Sensorless FOC & Motor Operating Control)
- `PERF-002` (Sensorless FOC & Motor Operating Control)
- `PERF-003` (Sensorless FOC & Motor Operating Control)
- `NFR-001` (Sensorless FOC & Motor Operating Control)
- `IF-003` (UART Command & Status Interface)
- `FR-004` (UART Command & Status Interface)
- `FR-005` (UART Command & Status Interface)
- `FR-006` (UART Command & Status Interface)
- `FR-007` (UART Command & Status Interface)
- `FR-008` (UART Command & Status Interface)
- `IF-005` (UART Command & Status Interface)
- `IF-006` (UART Command & Status Interface)
- `IF-007` (UART Command & Status Interface)
- `DIAG-001` (UART Command & Status Interface)
- `DIAG-002` (UART Command & Status Interface)
- `DIAG-003` (UART Command & Status Interface)
- `DIAG-004` (UART Command & Status Interface)
- `DIAG-005` (UART Command & Status Interface)
- `DIAG-006` (UART Command & Status Interface)
- `SF-001` (Drive Safety, Fault Handling & Reset Protection)
- `SF-002` (Drive Safety, Fault Handling & Reset Protection)
- `SF-003` (Drive Safety, Fault Handling & Reset Protection)
- `SF-004` (Drive Safety, Fault Handling & Reset Protection)
- `SF-005` (Drive Safety, Fault Handling & Reset Protection)
- `SF-006` (Drive Safety, Fault Handling & Reset Protection)
- `NFR-002` (Drive Safety, Fault Handling & Reset Protection)
