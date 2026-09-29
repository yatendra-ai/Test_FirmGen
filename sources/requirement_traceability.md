# Requirement traceability

Generated 2026-09-29T12:10:04.046820300+00:00
Specification frozen 2026-09-29T11:24:20.579Z

61 of 61 requirements have code recorded against them — 2 with tests passing, 0 with tests written but not passing, 59 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## STM32 Board Bring-up & Startup

gate: deferred · tasks: 5/5 done · recorded in: 3feb38cf-a2b1-426d-82f8-5258b1bbee42

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-001` | The firmware shall target the NUCLEO-C5A3ZG board, reference MB2310. | inspection | The selected firmware target configuration identifies the NUCLEO-C5A3ZG board and its MB2310 reference. (expected: The configuration identifies NUCLEO-C5A3ZG, reference MB2310.) | NUCLEO-C5A3ZG/cmake/cmake/target.cmake<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld | — | — | implemented, untested |
| `HW-002` | The firmware shall execute on an STM32C5A3ZGT6 MCU with a Cortex-M33 processor, single-precision FPU and DSP capability. | inspection | The selected MCU target configuration names STM32C5A3ZGT6 and specifies the Cortex-M33 core. (expected: The target is STM32C5A3ZGT6 and the core is Cortex-M33.)<br>The MCU part specification identifies single-precision FPU and DSP capability for STM32C5A3ZGT6. (expected: The specification lists a single-precision FPU and DSP capability.) | NUCLEO-C5A3ZG/cmake/cmake/target.cmake<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld | — | — | implemented, untested |
| `HW-003` | The firmware shall operate within a maximum device resource envelope of 144 MHz CPU clock, 1 MiB Flash and 256 KiB SRAM. | inspection | The configured maximum CPU clock does not exceed 144 MHz. (expected: CPU clock is at most 144 MHz.)<br>The linked firmware image occupies no more than 1 MiB of Flash. (expected: Flash image size is at most 1 MiB.)<br>The linked firmware's SRAM usage does not exceed 256 KiB. (expected: Reported SRAM usage is at most 256 KiB.) | NUCLEO-C5A3ZG/cmake/cmake/target.cmake<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld | — | — | implemented, untested |
| `HW-004` | The application shall not require an external sensor, external memory, network connection, RTOS or NPU. | inspection | The application's required dependency and startup configuration contains no mandatory external sensor, external memory, network service, RTOS, or NPU. (expected: None of the five listed components or services is a mandatory application dependency.) | NUCLEO-C5A3ZG/cmake/cmake/components.cmake<br>application/main.c<br>firmware/app/app.c | app_start, main | — | implemented, untested |
| `IF-002` | The application shall use the UART instance and pins supported by the selected board configuration and shall not assume unsupported pin mappings. | inspection | The selected board configuration names the UART instance and pins used by the application, and those pins match a UART mapping supported by the board. (expected: The UART instance and pins are identified and form a board-supported mapping.) | NUCLEO-C5A3ZG/generated/hal/mx_rcc.c<br>NUCLEO-C5A3ZG/generated/hal/mx_system.c<br>NUCLEO-C5A3ZG/generated/hal/mx_usart2.c<br>NUCLEO-C5A3ZG/generated/utilities/basic_stdio/mx_basic_stdio_app.c<br>application/main.c | main, mx_basic_stdio_init, mx_rcc_init, mx_system_init, mx_usart2_uart_init | — | implemented, untested |
| `FR-008` | On reset, the application shall initialize the MCU clock, FPU and serial console. | on-target test | After reset, clock and FPU initialization are complete before the application enters the test sequence. (expected: Clock and FPU initialization complete before test-sequence entry.)<br>After reset, serial-console initialization completes before the application enters the test sequence. (expected: Serial-console initialization completes before test-sequence entry.)<br>If clock, FPU, or serial-console initialization fails, the target run reports test failure and does not proceed into the test sequence. (expected: The run reports failure and does not enter the test sequence.) | NUCLEO-C5A3ZG/generated/hal/mx_rcc.c<br>NUCLEO-C5A3ZG/generated/hal/mx_system.c<br>NUCLEO-C5A3ZG/generated/hal/mx_usart2.c<br>NUCLEO-C5A3ZG/generated/utilities/basic_stdio/mx_basic_stdio_app.c<br>application/main.c | main, mx_basic_stdio_init, mx_rcc_init, mx_system_init, mx_usart2_uart_init | — | implemented, untested |
| `FR-023` | The application shall start correctly after flashing and a hardware reset. | hardware / instrument | After firmware flashing and a hardware reset, the ST-LINK Virtual COM Port receives startup output and the application reaches its execution-start state. (expected: Startup output is present after reset and the application reaches its execution-start state.) | NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/startup_stm32c5a3xx.c<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld<br>application/main.c | Reset_Handler, main | — | verified · observed on hardware |
| `FR-026` | The firmware shall be flashable to the physical board and shall start after reset. | hardware / instrument | Firmware flashes to a physical NUCLEO-C5A3ZG and, after hardware reset, produces startup output and begins application execution. (expected: Flashing completes, startup output is present after reset, and application execution begins.) | NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/startup_stm32c5a3xx.c<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld<br>application/main.c | Reset_Handler, main | — | verified · observed on hardware |
| `FR-037` | The application shall complete clock and FPU initialization before starting ML runtime initialization or executing FP32 inference. | on-target test | The target execution trace records clock and FPU initialization completion before ML runtime initialization and before the first inference call. (expected: Clock and FPU initialization complete before ML runtime initialization, and both complete before the first inference call.)<br>If required clock or FPU initialization fails, the target run fails without entering ML runtime initialization or executing inference. (expected: The run reports failure, with no ML runtime initialization or inference event after the failure.) | NUCLEO-C5A3ZG/generated/hal/mx_rcc.c<br>NUCLEO-C5A3ZG/generated/hal/mx_system.c<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/system_stm32c5xx.c<br>application/main.c | SystemInit, app_start, main, mx_rcc_init, mx_system_init | — | implemented, untested |
| `FR-039` | The application shall initialize the test status, vector counters and comparison results to a known state after every reset before beginning the test sequence. | on-target test | Repeated hardware resets produce identical initial test-status, vector-counter, and comparison-result state before the first vector executes. (expected: The initial snapshots are identical across the two runs.)<br>Under identical execution conditions, repeated hardware-reset runs produce the same vector count, comparison results, and final PASS/FAIL status. (expected: Vector count, comparison results, and final PASS/FAIL status match between runs.)<br>The test-state initialization event occurs before the first vector execution, and a run with stale or indeterminate initial state cannot report PASS. (expected: Initialization precedes the first vector; the stale-state run does not report PASS.) | firmware/app/app.c<br>firmware/app/app.h<br>firmware/configs/app_config.h | app_start, app_test_state_initialize | — | implemented, untested |

Observed on the board:
- `IF-002`, `FR-008` — Latest mcu_deploy reached application output over COM5 at 115200. Source order is CMSIS FPU enable, generated clock setup, USART2 VCP setup, basic stdio init, app entry.
- `FR-023`, `FR-026` — Successful mcu_deploy flash and board reset produced model banner, ready marker, five inference lines and final PASS on COM5.
- `FR-039` — The current test state initializes counters and five result slots before the vector loop; the successful target run produced the same five-result state after reset.

Notes recorded with these traces:
- `HW-001`, `HW-002`, `HW-003` — Inspection: selected target is NUCLEO-C5A3ZG/STM32C5A3ZGT6, Cortex-M33 with SP_FPU/DSP, 144 MHz, 1 MiB ROM and 256 KiB RAM linker regions.
- `HW-004` — Current application requires no external sensor, memory, network, RTOS, or NPU.
- `IF-002`, `FR-008` — USART2 TX is PA2, 115200 8-N-1, no flow control.
- `FR-037` — FPU is enabled in CMSIS SystemInit before main; RCC clock init precedes application/ML init.
- `FR-039` — Later app.c behavior changes expanded category scope without changing reset-state initialization semantics.

## UART Console & Result Reporting

gate: deferred · tasks: 4/4 done · recorded in: 3feb38cf-a2b1-426d-82f8-5258b1bbee42

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-002` | The application shall report five known-vector comparisons through the ST-LINK Virtual COM Port. | hardware / instrument | The captured ST-LINK VCP log contains a result for each of the five specified vector comparisons. (expected: 5 vector comparison results, one for each specified vector.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | C5_REFERENCE_VECTORS, app_start, result_report_vector | — | implemented, untested |
| `FR-003` | After reset, the application shall produce a concise and deterministic PASS/FAIL result. | hardware / instrument, not verifiable | Repeated reset runs with the same firmware and model produce byte-identical result output containing a PASS or FAIL status. (expected: The result output is byte-identical across runs and contains a PASS or FAIL status.)<br>The result output is concise. (expected: No pass condition can be stated because no maximum length or other definition of “concise” is specified.) | firmware/app/app.c<br>firmware/services/result_report.c<br>firmware/services/result_report.h | app_start, result_report_failure, result_report_send_line, result_report_summary | — | implemented, untested |
| `IF-001` | The application shall use the on-board STLINK-V3EC Virtual COM Port at 115200 baud, 8 data bits, no parity, 1 stop bit and no flow control. | hardware / instrument | The UART signal to the ST-LINK VCP uses 115200 baud, 8 data bits, no parity, 1 stop bit, and no flow control. (expected: 115200 baud, 8 data bits, no parity, 1 stop bit, and no flow control.)<br>A VCP receiver configured for 115200 baud and 8-N-1 without flow control captures the firmware's console output without framing errors. (expected: Console output is received and the receiver reports zero framing errors.) | NUCLEO-C5A3ZG/generated/hal/mx_usart2.c<br>NUCLEO-C5A3ZG/generated/utilities/basic_stdio/mx_basic_stdio_app.c<br>application/main.c<br>firmware/configs/app_config.h<br>firmware/interfaces/console_if.h<br>firmware/platforms/stm32/console_stm32.c | console_if_write, main, mx_basic_stdio_init, mx_system_init, mx_usart2_uart_init | — | implemented, untested |
| `FR-013` | The application shall print one deterministic result line per vector and a final summary. | hardware / instrument | A successful run produces exactly five vector result lines and one final summary line in the console log. (expected: Exactly 5 vector result lines and exactly 1 final summary line.)<br>A console transmission failure during the run causes the test result to fail rather than pass. (expected: The test status is FAIL; it is not reported as PASS.) | firmware/app/app.c<br>firmware/services/result_report.c<br>firmware/services/result_report.h | app_start, result_report_failure, result_report_send_line, result_report_summary | — | implemented, untested |
| `FR-022` | If any vector or runtime operation fails, the final status shall be FAIL and the failing condition shall be reported while console output remains available. | hardware / instrument | An injected vector or runtime failure produces a final FAIL status and a log entry identifying the failing condition when subsequent console output is available. (expected: Each run reports the injected failing condition and ends with status FAIL.) | firmware/app/app.c<br>firmware/services/result_report.c<br>firmware/services/result_report.h | app_start, result_report_failure, result_report_send_line, result_report_summary | — | implemented, untested |
| `IF-005` | The application shall multiply console display values by 1000 and print them as integers. | hardware / instrument | Each displayed output value is an integer equal to the corresponding comparison output multiplied by 1000. (expected: For every displayed output, printed integer = corresponding output × 1000.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | C5_REFERENCE_VECTORS, app_start, result_report_vector | — | implemented, untested |
| `IF-006` | The application shall print the model SHA-256 identifier at startup. | hardware / instrument | The startup console output contains the SHA-256 digest of the model deployed with the firmware. (expected: The printed model identifier matches the SHA-256 digest of the deployed model.) | firmware/app/app.c<br>firmware/configs/model_identity.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | app_start, result_report_runtime_ready, result_report_startup | — | implemented, untested |
| `FR-021` | The application shall print ML_RUNTIME_READY only after successful model and runtime initialization. | hardware / instrument | ML_RUNTIME_READY appears after successful model and runtime initialization and is absent when either initialization fails. (expected: The successful run contains ML_RUNTIME_READY after initialization; neither failure run contains ML_RUNTIME_READY.) | firmware/app/app.c<br>firmware/configs/model_identity.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | app_start, result_report_runtime_ready, result_report_startup | — | implemented, untested |
| `IF-007` | The application shall print exactly one ML_VECTOR result line for each executed vector. | hardware / instrument | The log contains exactly one ML_VECTOR result line for each executed vector and no ML_VECTOR result line for a vector that was not executed. (expected: Each executed vector has exactly 1 ML_VECTOR line; each unexecuted vector has 0 such lines.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | C5_REFERENCE_VECTORS, app_start, result_report_vector | — | implemented, untested |
| `IF-008` | The application shall detect an unsuccessful console transmission for each required startup, vector-result and final-status message. | host test | A simulated unsuccessful transmission of each required startup, vector-result, or final-status message sets the test result to FAIL and prevents a PASS summary. (expected: For each injected failure, the test result is FAIL and no PASS summary is emitted.) | NUCLEO-C5A3ZG/generated/hal/mx_usart2.c<br>NUCLEO-C5A3ZG/generated/utilities/basic_stdio/mx_basic_stdio_app.c<br>application/main.c<br>firmware/configs/app_config.h<br>firmware/interfaces/console_if.h<br>firmware/platforms/stm32/console_stm32.c | console_if_write, main, mx_basic_stdio_init, mx_system_init, mx_usart2_uart_init | — | implemented, untested |
| `IF-009` | Each ML_VECTOR result line shall identify the reference vector and indicate the outcome of its FP32 comparison. | hardware / instrument | Each named reference vector has exactly one ML_VECTOR line that identifies the vector and contains an explicit, parseable FP32 comparison outcome. (expected: Exactly 1 identifiable ML_VECTOR line per named vector, each with an explicit parseable comparison outcome.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | C5_REFERENCE_VECTORS, app_start, result_report_vector | — | implemented, untested |
| `FR-027` | The serial console shall report all five test vectors. | hardware / instrument | The captured serial log reports positive, first_clamped, second_clamped, zero_input, and both_clamped. (expected: A result is present for each of the 5 named vectors.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c<br>firmware/services/result_report.h | C5_REFERENCE_VECTORS, app_start, result_report_vector | — | implemented, untested |
| `IF-010` | The application shall print ML_SUMMARY passed=5 total=5 status=PASS only when all five reference-vector comparisons and cleanup succeed; otherwise it shall not report a PASS summary. | hardware / instrument | After all five vector comparisons and cleanup succeed, the final log contains exactly ML_SUMMARY passed=5 total=5 status=PASS. (expected: The final summary is exactly `ML_SUMMARY passed=5 total=5 status=PASS`.)<br>A failed vector, comparison, or cleanup operation prevents emission of a PASS summary. (expected: Zero PASS summaries are emitted in every failure case.) | firmware/app/app.c<br>firmware/services/result_report.c<br>firmware/services/result_report.h | app_start, result_report_failure, result_report_send_line, result_report_summary | — | implemented, untested |

Observed on the board:
- `FR-002`, `IF-005`, `IF-007`, `IF-009`, `FR-027` — Five current-run lines identified positive, first_clamped, second_clamped, zero_input, both_clamped; each explicit comparison=PASS and milli-unit outputs match the reference display table.
- `FR-003`, `FR-013`, `FR-022`, `IF-010` — Latest current-run serial segment contains five ML_VECTOR lines and exactly `ML_SUMMARY passed=5 total=5 status=PASS`; the run succeeded after all vectors and cleanup.
- `FR-003` — Two repeated target runs had identical five-vector result lines and PASS summary in the captured fresh-run portion.
- `IF-001`, `IF-008` — Two latest mcu_deploy runs received output via COM5 at 115200, including startup and five vectors. No simulated UART transmission fault/framing instrument used.
- `IF-006`, `FR-021` — Current-run segment emitted the model hash matching the model manifest followed by ML_RUNTIME_READY after successful initialization.

Notes recorded with these traces:
- `FR-003`, `FR-013`, `FR-022`, `IF-010` — Code only gates PASS after all five comparisons and cleanup. Failure injection was not exercised.
- `FR-003` — The 'concise' criterion is NOT VERIFIABLE as written; no objective output-length bound is defined.
- `IF-001`, `IF-008` — USART2 PA2 TX 115200 8-N-1, no flow control, checked 100 ms HAL transmit timeout.

## Build, Memory & Release Configuration

gate: deferred · tasks: 6/6 done · recorded in: 3feb38cf-a2b1-426d-82f8-5258b1bbee42

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `NFR-001` | The application shall not require dynamic heap allocation for the test sequence. | inspection | The startup and test-sequence call paths contain no dynamic heap allocation or reachable allocator calls. (expected: No dynamic heap allocation occurs on any startup or test-sequence path.) | application/main.c<br>firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/platforms/stm32/console_stm32.c<br>firmware/services/result_report.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/backend_stai/fg_ml_stai.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/runtime/fg_ml_runtime.c | app_start, c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init, console_if_write, fg_ml_deinit, fg_ml_init, fg_ml_run, main | — | implemented, untested |
| `NFR-003` | All generated model sources, runtime libraries and application objects shall use a mutually compatible Cortex-M33 CPU/FPU ABI. | inspection | The build configuration and component artifacts identify the same Cortex-M33 CPU/FPU ABI for generated model sources, runtime libraries, and application objects. (expected: All listed components use mutually compatible Cortex-M33 CPU/FPU ABI settings.) | NUCLEO-C5A3ZG/cmake/cmake/GCC.14.3.1.cmake<br>NUCLEO-C5A3ZG/cmake/cmake/target.cmake<br>firmgen_user_components.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json | — | — | implemented, untested |
| `NFR-004` | The final firmware shall link without unresolved model or runtime symbols, ABI mismatch errors or overlapping memory regions. | inspection | The final link produces a firmware artifact without unresolved symbols, ABI mismatch diagnostics, or overlapping memory-region diagnostics. (expected: A linked firmware artifact is present, with zero diagnostics for the three listed link conditions.) | NUCLEO-C5A3ZG/cmake/build/debug_GCC_NUCLEO-C5A3ZG/template_start_from_nucleo.map<br>firmgen_user_components.json | — | — | implemented, untested |
| `NFR-005` | The generated linker memory placement shall fit within the target MCU Flash and SRAM limits. | inspection | The linker map places all Flash sections within 1 MiB and all SRAM sections within 256 KiB of the target memory regions. (expected: Placed Flash sections remain within 1 MiB, and placed SRAM sections remain within 256 KiB.) | NUCLEO-C5A3ZG/cmake/build/debug_GCC_NUCLEO-C5A3ZG/template_start_from_nucleo.map<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld | — | — | implemented, untested |
| `FR-025` | The complete firmware shall build and link successfully for NUCLEO-C5A3ZG. | inspection | A complete build configured for NUCLEO-C5A3ZG produces a successfully linked firmware artifact. (expected: The build and link succeed for NUCLEO-C5A3ZG, and the target firmware artifact is present.) | NUCLEO-C5A3ZG/cmake/build/debug_GCC_NUCLEO-C5A3ZG/template_start_from_nucleo.map<br>firmgen_user_components.json | — | — | implemented, untested |
| `NFR-006` | A fresh rebuild of the same source and model shall produce the same functional behavior on the target board. | hardware / instrument | Two fresh builds from identical source and model inputs produce equivalent startup, five-vector, and final-summary behavior when run on the target board. (expected: The two captures show equivalent startup behavior, the same five vector results, and equivalent final-summary behavior.) | firmware/docs/validation/category5-serial-capture.log<br>firmware/docs/validation/validation-record.md | — | — | implemented, untested |
| `FR-030` | The project shall deliver STM32 project source code. | inspection | The delivered package contains the STM32 project source required to build the application. (expected: STM32 project source required to build the application is present in the delivered package.) | NUCLEO-C5A3ZG/cmake/build/debug_GCC_NUCLEO-C5A3ZG/template_start_from_nucleo.elf<br>application/main.c<br>firmware/app/app.c | app_start, main | — | implemented, untested |
| `FR-032` | The project shall deliver the final firmware binary/ELF artifact. | inspection | The delivered package contains a final firmware binary or ELF artifact for the target board. (expected: At least one final firmware binary or ELF artifact for the target board is present in the package.) | NUCLEO-C5A3ZG/cmake/build/debug_GCC_NUCLEO-C5A3ZG/template_start_from_nucleo.elf<br>application/main.c<br>firmware/app/app.c | app_start, main | — | implemented, untested |
| `FR-033` | The project shall deliver the model SHA-256 identifier. | inspection | The delivered package contains a SHA-256 identifier that matches the deployed model. (expected: The delivered identifier is a 64-character hexadecimal SHA-256 digest and matches the deployed model's computed digest.) | firmware/configs/model_identity.h<br>firmware/docs/validation/category5-serial-capture.log<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json | APP_MODEL_SHA256_HEX | — | implemented, untested |
| `FR-034` | The project shall deliver a captured serial log showing startup, five vector results and the final PASS/FAIL summary. | inspection | The delivered serial-log capture contains startup output, five vector-result lines, and a final PASS or FAIL summary. (expected: The capture contains startup output, exactly five vector-result lines, and a final PASS or FAIL summary.) | firmware/configs/model_identity.h<br>firmware/docs/validation/category5-serial-capture.log<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json | APP_MODEL_SHA256_HEX | — | implemented, untested |
| `FR-035` | The project shall deliver a short validation record stating the board used, firmware build result, flash result and observed inference result. | inspection | The delivered validation record identifies the board used, firmware build result, flash result, and observed inference result. (expected: All four required items are identified in the record.) | firmware/docs/validation/validation-record.md | — | — | implemented, untested |
| `NFR-007` | The complete startup and five-vector test sequence shall execute without runtime stack overflow and with all stack, static, activation, context and I/O storage within the target SRAM allocation. | inspection, on-target test | The memory map and runtime-storage evidence account for stack, static, activation, context, and I/O storage within the target's 256 KiB SRAM allocation. (expected: All listed runtime storage is accounted for within 256 KiB (262144 bytes) of target SRAM.)<br>During startup and execution of all five vectors, the stack high-water mark remains within its allocated stack range and the runtime stack-overflow indicator remains clear. (expected: The measured stack use does not exceed the allocated stack size, and the stack-overflow indicator is clear.) | NUCLEO-C5A3ZG/cmake/build/debug_GCC_NUCLEO-C5A3ZG/template_start_from_nucleo.map<br>NUCLEO-C5A3ZG/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/reports/ml_validation_report.json | c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init | — | implemented, untested |

Observed on the board:
- `NFR-006` — Two fresh deploys from the same source produced same vector sequence/values and final PASS in observed clean run segment. First capture contained initial buffered old output before the current SHA marker; comparison uses only current-build output beginning at marker.
- `FR-033`, `FR-034` — Clean serial segment includes the 64-hex model hash and five named vector lines plus PASS summary. Hash matches toolchain lock and model manifest metadata.
- `FR-035` — Validation record states board, successful build, successful flash and successful five-vector target inference result.

Notes recorded with these traces:
- `NFR-001` — Application/runtime buffer sources use static storage and contain no malloc/calloc/realloc/free calls. The current harness is static; generated Edge AI archive internals were not independently audited for allocator usage.
- `NFR-003` — Firmware uses Cortex-M33, fpv5-sp-d16, hard-float ABI. Model generator targets STM32C5 and registered matching ST Edge AI NetworkRuntime1201_CM33_GCC.a runtime archive; integrated link succeeds.
- `NFR-004`, `FR-025` — Latest full firmware build and link succeeded for the NUCLEO-C5A3ZG preset, with no unresolved symbol/ABI/overlap diagnostics.
- `NFR-005` — mcu_budget reports ROM 27,216/1,048,576 B and RAM 2,336/262,144 B after model integration; all fit current regions.
- `FR-030`, `FR-032` — STM32 source project and final linked ELF are present; ELF remains in the generated build output and is not separately packaged.
- `NFR-007` — Current map uses 1024-byte stack and 2336 bytes total RAM including data/bss/heap reservations; compiler estimates 16-byte activation and 24-byte weights, and code uses fixed buffers. No target stack high-water/overflow instrumentation was available, so runtime high-water criterion is not verified.

## ONNX Model & ST AI Runtime

gate: deferred · tasks: 5/5 done · recorded in: 3feb38cf-a2b1-426d-82f8-5258b1bbee42

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The STM32 application shall execute the supplied FP32 ONNX model on the MCU CPU. | hardware / instrument | The NUCLEO-C5A3ZG completes an inference by executing the supplied FP32 model on its MCU CPU. (expected: The trace records a completed model inference on the MCU CPU.) | firmware/app/app.c<br>firmware/drivers/c5_stai/include/c5_ml_quick_test.h<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/backend_stai/fg_ml_stai.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/runtime/fg_ml_runtime.c | app_start, c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init, fg_ml_deinit, fg_ml_init, fg_ml_run, stai_network_run | — | implemented, untested |
| `FR-004` | The application shall use c5_dense_relu.onnx unchanged. | inspection | A comparison of the deployed model with c5_dense_relu.onnx finds no graph or weight changes. (expected: The graph and weights match without changes.) | model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/reports/ml_validation_report.json | — | — | implemented, untested |
| `IF-003` | The deployed model shall be an FP32 ONNX model using opset 17 and IR 8, with one input tensor named input and one output tensor named output, both having shape [1,2]. | inspection | Inspection of the deployed ONNX artifact confirms FP32 tensors, opset 17, IR version 8, and exactly one input named input and one output named output, each with shape [1,2]. (expected: IR version is 8; opset is 17; there is one FP32 input named input with shape [1,2] and one FP32 output named output with shape [1,2].) | model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/reports/ml_validation_report.json | — | — | implemented, untested |
| `IF-004` | The model graph shall consist of MatMul, Add and ReLU operations. | inspection | Inspection of the deployed model graph finds MatMul, Add, and Relu as its operations and finds no other operation types. (expected: The graph operation types are MatMul, Add, and Relu only.) | model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/reports/ml_validation_report.json | — | — | implemented, untested |
| `FR-005` | The application shall perform no preprocessing or postprocessing other than direct FP32 input and output handling. | inspection | Inspection of the inference data path finds direct FP32 input and output handling and no additional preprocessing or postprocessing. (expected: Input and output values are handled directly as FP32, with no additional preprocessing or postprocessing.) | firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.h<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network_data.c | c5_ml_infer, g_network_weights_array, stai_network_run | — | implemented, untested |
| `FR-006` | The deployed firmware shall execute the compiled neural-network model using the ST AI runtime. | hardware / instrument | The inference execution trace shows inference dispatched through the compiled model using the ST AI runtime. (expected: The trace identifies an inference call through the ST AI runtime using the compiled model.) | firmware/app/app.c<br>firmware/drivers/c5_stai/include/c5_ml_quick_test.h<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/backend_stai/fg_ml_stai.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/runtime/fg_ml_runtime.c | app_start, c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init, fg_ml_deinit, fg_ml_init, fg_ml_run, stai_network_run | — | implemented, untested |
| `FR-007` | The application shall not use a handwritten replacement of the model equations as valid model inference. | inspection | The inference call path invokes the compiled model's inference entry point rather than a handwritten implementation of the model equations. (expected: The application inference path invokes the compiled model and does not substitute handwritten model equations.) | firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.h<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network_data.c | c5_ml_infer, g_network_weights_array, stai_network_run | — | implemented, untested |
| `FR-009` | The application shall initialize the generated ML network and all required activation, context and I/O buffers. | on-target test | After reset initialization, generated-network initialization succeeds and all required activation, context, and I/O buffers are available before the first inference. (expected: Network initialization succeeds; every required buffer is available to the network before the first inference.) | firmware/app/app.c<br>firmware/drivers/c5_stai/include/c5_ml_quick_test.h<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/backend_stai/fg_ml_stai.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/runtime/fg_ml_runtime.c | app_start, c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init, fg_ml_deinit, fg_ml_init, fg_ml_run, stai_network_run | — | implemented, untested |
| `FR-012` | The application shall deinitialize the ML runtime after the test sequence completes. | on-target test | The ML runtime deinitialization operation is invoked after completion of the fifth vector execution. (expected: All five vector executions complete before the ML runtime deinitialization operation is invoked.) | firmware/app/app.c<br>firmware/drivers/c5_stai/include/c5_ml_quick_test.h<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/backend_stai/fg_ml_stai.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/runtime/fg_ml_runtime.c | app_start, c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init, fg_ml_deinit, fg_ml_init, fg_ml_run, stai_network_run | — | implemented, untested |
| `NFR-002` | The model shall be compiled for STM32C5 CPU execution using an ST AI compatible C runtime. | inspection | The build configuration and generated model artifacts identify STM32C5 CPU execution as the model target. (expected: Both identify STM32C5 CPU execution as the target.)<br>The delivered runtime integration identifies an ST AI compatible C runtime for model execution. (expected: The model execution runtime is identified as an ST AI compatible C runtime.) | firmgen_user_components.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/components.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>user/lib/stedgeai_runtime_4_0_1_cm33/NetworkRuntime1201_CM33_GCC.a | — | — | implemented, untested |
| `FR-024` | The supplied ONNX model shall be converted into deployable STM32C5 inference code without changing the model graph or weights. | inspection | Comparison of the supplied ONNX model with the conversion inputs and outputs finds no changes to the model graph or weights. (expected: The graph and weights are unchanged from the supplied model.)<br>The delivered conversion output contains inference code generated for deployment on STM32C5. (expected: Inference code is present and the target metadata identifies STM32C5.) | model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/reports/ml_validation_report.json | — | — | implemented, untested |
| `FR-031` | The project shall deliver compiled ML model sources and required runtime integration files. | inspection | The delivered project package contains the compiled ML model sources and the runtime integration files required by its build configuration. (expected: All compiled ML model sources and runtime integration files required by the build configuration are present in the package.) | firmgen_user_components.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/components.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>user/lib/stedgeai_runtime_4_0_1_cm33/NetworkRuntime1201_CM33_GCC.a | — | — | implemented, untested |
| `HW-005` | The firmware shall embed the model weights and biases in the firmware image, and inference shall not depend on external model or weight storage. | inspection | Inspection of the firmware image and linked model data confirms that the model weights and biases required for inference are embedded in the image. (expected: The firmware image contains the model weights and biases required for inference.)<br>Inspection of the inference data path finds no dependency on external model or weight storage. (expected: Inference obtains the required model data from the firmware image and does not require external model or weight storage.) | firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network.h<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/bundle/generated/output/network_data.c | c5_ml_infer, g_network_weights_array, stai_network_run | — | implemented, untested |
| `FR-038` | The SHA-256 identifier printed at startup shall correspond to the model actually embedded and used by the deployed firmware. | inspection | The SHA-256 identifier in the startup log matches an independently calculated SHA-256 of the model artifact embedded in the firmware and used by the ST AI runtime. (expected: The startup identifier exactly matches the independently calculated SHA-256 of the embedded model artifact used by the runtime.) | firmware/configs/model_identity.h<br>firmware/services/result_report.c<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/ml_toolchain.lock.json<br>model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json | APP_MODEL_SHA256_HEX, app_start, result_report_startup | — | implemented, untested |

Observed on the board:
- `FR-001`, `FR-006`, `FR-009`, `FR-012` — mcu_deploy built and flashed successfully. COM5 @115200 after reset captured ML_MODEL_SHA256=15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090, ML_RUNTIME_READY, and ML_INFERENCE_PROBE status=PASS output_milli=[500,4000], followed by ML_SUMMARY passed=0 total=5 status=FAIL. The ST AI model completed one CPU probe inference with expected positive-vector output. The full five-vector sequence is not part of category 4 and has not run yet. Runtime deinit is called in app_start after the probe, but the serial trace does not independently expose deinit return details.
- `FR-038` — The startup VCP line reported `ML_MODEL_SHA256=15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090`, matching the toolchain lock and model manifest digest.

Notes recorded with these traces:
- `FR-001`, `FR-006`, `FR-009`, `FR-012` — Generated ST Edge AI v4.0.1 STAI API bundle; fixed 16-byte activation storage and generated embedded 24-byte weights; matching CM33 GCC runtime library linked.
- `FR-004`, `IF-003`, `IF-004`, `FR-024` — ST Edge AI Inspector 1.23.0 metadata records source model c5_dense_relu.onnx (245 bytes), FP32 input/output `[1,2]`, original tensor names `input` and `output`, and opset/graph operations as Add, MatMul, Relu; model SHA-256 is 15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090. Toolchain lock records ST Edge AI Core 4.0.1-20581, target stm32c5, CPU STAI API. The compiler emitted normalized tensor identifiers input_output_array/output_output_array, with original metadata retained.
- `FR-005`, `FR-007`, `HW-005` — Source inference path passes direct FP32 buffers to fg_ml_run/STAI, with no transformations; model computation is generated STAI network code, not handwritten equations. Model weight symbols are static const g_network_weights_array[3] in network_data.c (24 bytes) linked into ROM.
- `NFR-002`, `FR-031` — Bundle registered generated source and runtime/backend source directories; matching NetworkRuntime1201_CM33_GCC.a imported as prebuilt from ST Edge AI 4.0.1 path, Cortex-M33 GCC archive; CPU/FPU flags match firmware. Compile warnings note analysis capacity estimates exclude full app and placement.
- `FR-038` — Digest belongs to the original ONNX model used to generate the embedded model weights; no separate independent recomputation against firmware bytes was performed.

## Inference Test Harness & Verification

gate: deferred · tasks: 4/4 done · recorded in: 3feb38cf-a2b1-426d-82f8-5258b1bbee42

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-010` | The application shall execute each of the five reference input vectors exactly once. | host test | A run records exactly one inference call for each of the five reference-vector IDs and records no other inference calls. (expected: The trace contains exactly five calls, one for each reference-vector ID, with no duplicate or additional calls.) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h | C5_REFERENCE_VECTORS, app_start, c5_ml_infer | — | implemented, untested |
| `FR-011` | For every reference vector, the application shall compare both FP32 outputs with the expected values before formatting them for display. | host test | For each vector, the event trace records comparisons of both original FP32 outputs against their expected values before any display conversion or formatting event. (expected: Each vector has two FP32 comparison events before its first display-conversion or formatting event; the full run contains ten comparison events.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/modules/vector_compare.c<br>firmware/modules/vector_compare.h | app_start, vector_compare_fp32, vector_output_to_milli | — | implemented, untested |
| `FR-014` | Any initialization failure, inference failure, non-finite output, deinitialization failure or console transmission failure shall cause the test to fail. | host test | Injecting each listed initialization, inference, non-finite-output, deinitialization, and console-transmission failure leaves the final test status at FAIL and does not set or report PASS. (expected: For every injected failure, final status is FAIL and PASS is not reported.) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/vector_compare.c<br>firmware/services/result_report.c | app_start, c5_ml_infer, c5_ml_runtime_deinit, c5_ml_runtime_init, result_report_latch_failure, result_report_summary | — | implemented, untested |
| `FR-015` | The application shall execute the positive reference vector with input [1,2] and expected output [0.5,4], displaying [500,4000] in milli-units. | hardware / instrument | The positive-vector run supplies input [1,2], produces FP32 outputs within 0.000001 absolute error of [0.5,4], and displays [500,4000] milli-units. (expected: Input is [1,2]; each FP32 output differs from its expected value by at most 0.000001; the displayed values are [500,4000].) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c | C5_REFERENCE_VECTORS, app_start, c5_ml_infer, result_report_vector | — | implemented, untested |
| `FR-016` | The application shall execute the first_clamped reference vector with input [-2,1] and expected output [0,4], displaying [0,4000] in milli-units. | hardware / instrument | The first_clamped run supplies input [-2,1], produces FP32 outputs within 0.000001 absolute error of [0,4], and displays [0,4000] milli-units. (expected: Input is [-2,1]; each FP32 output differs from its expected value by at most 0.000001; the displayed values are [0,4000].) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c | C5_REFERENCE_VECTORS, app_start, c5_ml_infer, result_report_vector | — | implemented, untested |
| `FR-017` | The application shall execute the second_clamped reference vector with input [3,-1] and expected output [7.5,0], displaying [7500,0] in milli-units. | hardware / instrument | The second_clamped run supplies input [3,-1], produces FP32 outputs within 0.000001 absolute error of [7.5,0], and displays [7500,0] milli-units. (expected: Input is [3,-1]; each FP32 output differs from its expected value by at most 0.000001; the displayed values are [7500,0].) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c | C5_REFERENCE_VECTORS, app_start, c5_ml_infer, result_report_vector | — | implemented, untested |
| `FR-018` | The application shall execute the zero_input reference vector with input [0,0] and expected output [0.5,0], displaying [500,0] in milli-units. | hardware / instrument | The zero_input run supplies input [0,0], produces FP32 outputs within 0.000001 absolute error of [0.5,0], and displays [500,0] milli-units. (expected: Input is [0,0]; each FP32 output differs from its expected value by at most 0.000001; the displayed values are [500,0].) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c | C5_REFERENCE_VECTORS, app_start, c5_ml_infer, result_report_vector | — | implemented, untested |
| `FR-019` | The application shall execute the both_clamped reference vector with input [-1,-1] and expected output [0,0], displaying [0,0] in milli-units. | hardware / instrument | The both_clamped run supplies input [-1,-1], produces FP32 outputs within 0.000001 absolute error of [0,0], and displays [0,0] milli-units. (expected: Input is [-1,-1]; each FP32 output differs from its expected value by at most 0.000001; the displayed values are [0,0].) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h<br>firmware/services/result_report.c | C5_REFERENCE_VECTORS, app_start, c5_ml_infer, result_report_vector | — | implemented, untested |
| `FR-020` | The application shall compare each original FP32 output using an absolute tolerance of 0.000001 and a relative tolerance of 0. | host test | The FP32 comparator accepts an output whose absolute difference is within 0.000001 of the expected value and rejects one outside that bound, without adding relative-tolerance allowance. (expected: The first comparison passes and the second fails; the acceptance boundary is absolute error <= 0.000001 with relative tolerance 0.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/modules/vector_compare.c<br>firmware/modules/vector_compare.h | app_start, vector_compare_fp32, vector_output_to_milli | — | implemented, untested |
| `FR-028` | Each vector shall match the expected FP32 result within the defined tolerance. | host test | A complete five-vector test run counts as passing only when all ten original FP32 output comparisons are within absolute tolerance 0.000001 with relative tolerance 0. (expected: With all outputs in tolerance, all ten comparisons pass; when any output is outside tolerance, its comparison and vector do not pass.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/modules/vector_compare.c<br>firmware/modules/vector_compare.h | app_start, vector_compare_fp32, vector_output_to_milli | — | implemented, untested |
| `FR-029` | Wrong, missing or non-finite outputs, or any ML runtime failure, shall not be counted as a pass. | host test | Injecting a wrong output, a missing output, a non-finite output, or an ML runtime failure produces an outcome other than PASS for each case. (expected: None of the four runs has final outcome PASS.) | firmware/app/app.c<br>firmware/modules/c5_vectors.h<br>firmware/modules/vector_compare.c<br>firmware/modules/vector_compare.h | app_start, vector_compare_fp32, vector_output_to_milli | — | implemented, untested |
| `FR-036` | The application shall execute the five reference vectors in a fixed, documented order on every test run. | inspection, hardware / instrument | The test documentation identifies a fixed execution order containing each of the five reference vectors exactly once. (expected: One fixed sequence is stated, and it lists each of the five reference vectors exactly once.)<br>Repeated target resets with the same firmware produce the five vector executions and result lines in the documented order on every run. (expected: Every captured run contains the five vectors once, in the documented order, and the ordered sequences are identical across runs.) | firmware/app/app.c<br>firmware/drivers/c5_stai/src/c5_ml_quick_test.c<br>firmware/modules/c5_vectors.h | C5_REFERENCE_VECTORS, app_start, c5_ml_infer | — | implemented, untested |

Observed on the board:
- `FR-010`, `FR-036` — Target VCP segment after the current SHA banner recorded exactly one ML_VECTOR line for each vector in fixed documented order positive, first_clamped, second_clamped, zero_input, both_clamped. A second deploy/reset showed identical vector outputs/order and final PASS; its initial serial text was buffered prior output and was excluded from current-run comparison.
- `FR-011`, `FR-020`, `FR-028`, `FR-029` — On target, five outputs were emitted as [500,4000], [0,4000], [7500,0], [500,0], [0,0], each comparison=PASS. The raw FP32 values and comparison-before-format ordering are established by the source path; UART does not expose raw FP32 values.
- `FR-015`, `FR-016`, `FR-017`, `FR-018`, `FR-019` — COM5 @115200 capture: positive [500,4000], first_clamped [0,4000], second_clamped [7500,0], zero_input [500,0], both_clamped [0,0]; every line had comparison=PASS. A second run showed the same five outputs.

Notes recorded with these traces:
- `FR-010`, `FR-036` — No host test run has been imported; code is reachable from app_start and model inference executes once in the loop per vector.
- `FR-011`, `FR-020`, `FR-028`, `FR-029` — Comparator requires finite values and absolute error <= 0.000001F, with zero relative tolerance. Host failure injection and boundary testing are not run in this chat.
- `FR-014` — Code has fail-closed handling for initialization, inference, non-finite/unrepresentable output, comparison, deinit, and report-transmit errors. Host-injected failure cases were not executed here.
- `FR-015`, `FR-016`, `FR-017`, `FR-018`, `FR-019` — Inputs and expected FP32 outputs match the PRD table; milli display follows original outputs times 1000.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### Host tests you could write (7)

These have a `host` acceptance criterion and cite no test. They are tested at the end, from Artifacts -> Unit Tests.

- `IF-008` (UART Console & Result Reporting)
- `FR-010` (Inference Test Harness & Verification)
- `FR-011` (Inference Test Harness & Verification)
- `FR-014` (Inference Test Harness & Verification)
- `FR-020` (Inference Test Harness & Verification)
- `FR-028` (Inference Test Harness & Verification)
- `FR-029` (Inference Test Harness & Verification)

### Acceptance needs non-host evidence (54)

No `host` acceptance criterion is recorded. Host tests cannot settle these acceptance claims, but implementation calculations and failure recovery can still need regression tests. Each line identifies the required acceptance evidence; do not substitute a host result for it.

- `HW-001` — checked by inspection · _STM32 Board Bring-up & Startup_
- `HW-002` — checked by inspection · _STM32 Board Bring-up & Startup_
- `HW-003` — checked by inspection · _STM32 Board Bring-up & Startup_
- `HW-004` — checked by inspection · _STM32 Board Bring-up & Startup_
- `IF-002` — checked by inspection · _STM32 Board Bring-up & Startup_
- `FR-008` — checked by on-target test · _STM32 Board Bring-up & Startup_
- `FR-023` — checked by hardware / instrument · _STM32 Board Bring-up & Startup_
- `FR-026` — checked by hardware / instrument · _STM32 Board Bring-up & Startup_
- `FR-037` — checked by on-target test · _STM32 Board Bring-up & Startup_
- `FR-039` — checked by on-target test · _STM32 Board Bring-up & Startup_
- `FR-002` — checked by hardware / instrument · _UART Console & Result Reporting_
- `FR-003` — checked by hardware / instrument, not verifiable · _UART Console & Result Reporting_
- `IF-001` — checked by hardware / instrument · _UART Console & Result Reporting_
- `FR-013` — checked by hardware / instrument · _UART Console & Result Reporting_
- `FR-022` — checked by hardware / instrument · _UART Console & Result Reporting_
- `IF-005` — checked by hardware / instrument · _UART Console & Result Reporting_
- `IF-006` — checked by hardware / instrument · _UART Console & Result Reporting_
- `FR-021` — checked by hardware / instrument · _UART Console & Result Reporting_
- `IF-007` — checked by hardware / instrument · _UART Console & Result Reporting_
- `IF-009` — checked by hardware / instrument · _UART Console & Result Reporting_
- `FR-027` — checked by hardware / instrument · _UART Console & Result Reporting_
- `IF-010` — checked by hardware / instrument · _UART Console & Result Reporting_
- `NFR-001` — checked by inspection · _Build, Memory & Release Configuration_
- `NFR-003` — checked by inspection · _Build, Memory & Release Configuration_
- `NFR-004` — checked by inspection · _Build, Memory & Release Configuration_
- `NFR-005` — checked by inspection · _Build, Memory & Release Configuration_
- `FR-025` — checked by inspection · _Build, Memory & Release Configuration_
- `NFR-006` — checked by hardware / instrument · _Build, Memory & Release Configuration_
- `FR-030` — checked by inspection · _Build, Memory & Release Configuration_
- `FR-032` — checked by inspection · _Build, Memory & Release Configuration_
- `FR-033` — checked by inspection · _Build, Memory & Release Configuration_
- `FR-034` — checked by inspection · _Build, Memory & Release Configuration_
- `FR-035` — checked by inspection · _Build, Memory & Release Configuration_
- `NFR-007` — checked by inspection, on-target test · _Build, Memory & Release Configuration_
- `FR-001` — checked by hardware / instrument · _ONNX Model & ST AI Runtime_
- `FR-004` — checked by inspection · _ONNX Model & ST AI Runtime_
- `IF-003` — checked by inspection · _ONNX Model & ST AI Runtime_
- `IF-004` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-005` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-006` — checked by hardware / instrument · _ONNX Model & ST AI Runtime_
- `FR-007` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-009` — checked by on-target test · _ONNX Model & ST AI Runtime_
- `FR-012` — checked by on-target test · _ONNX Model & ST AI Runtime_
- `NFR-002` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-024` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-031` — checked by inspection · _ONNX Model & ST AI Runtime_
- `HW-005` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-038` — checked by inspection · _ONNX Model & ST AI Runtime_
- `FR-015` — checked by hardware / instrument · _Inference Test Harness & Verification_
- `FR-016` — checked by hardware / instrument · _Inference Test Harness & Verification_
- `FR-017` — checked by hardware / instrument · _Inference Test Harness & Verification_
- `FR-018` — checked by hardware / instrument · _Inference Test Harness & Verification_
- `FR-019` — checked by hardware / instrument · _Inference Test Harness & Verification_
- `FR-036` — checked by inspection, hardware / instrument · _Inference Test Harness & Verification_

### Board checks owed (2)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `FR-037` (STM32 Board Bring-up & Startup)
- `NFR-007` (Build, Memory & Release Configuration)

### Implemented, never tested (59)

Code exists and no test is recorded against it.

- `HW-001` (STM32 Board Bring-up & Startup)
- `HW-002` (STM32 Board Bring-up & Startup)
- `HW-003` (STM32 Board Bring-up & Startup)
- `HW-004` (STM32 Board Bring-up & Startup)
- `IF-002` (STM32 Board Bring-up & Startup)
- `FR-008` (STM32 Board Bring-up & Startup)
- `FR-037` (STM32 Board Bring-up & Startup)
- `FR-039` (STM32 Board Bring-up & Startup)
- `FR-002` (UART Console & Result Reporting)
- `FR-003` (UART Console & Result Reporting)
- `IF-001` (UART Console & Result Reporting)
- `FR-013` (UART Console & Result Reporting)
- `FR-022` (UART Console & Result Reporting)
- `IF-005` (UART Console & Result Reporting)
- `IF-006` (UART Console & Result Reporting)
- `FR-021` (UART Console & Result Reporting)
- `IF-007` (UART Console & Result Reporting)
- `IF-008` (UART Console & Result Reporting)
- `IF-009` (UART Console & Result Reporting)
- `FR-027` (UART Console & Result Reporting)
- `IF-010` (UART Console & Result Reporting)
- `NFR-001` (Build, Memory & Release Configuration)
- `NFR-003` (Build, Memory & Release Configuration)
- `NFR-004` (Build, Memory & Release Configuration)
- `NFR-005` (Build, Memory & Release Configuration)
- `FR-025` (Build, Memory & Release Configuration)
- `NFR-006` (Build, Memory & Release Configuration)
- `FR-030` (Build, Memory & Release Configuration)
- `FR-032` (Build, Memory & Release Configuration)
- `FR-033` (Build, Memory & Release Configuration)
- `FR-034` (Build, Memory & Release Configuration)
- `FR-035` (Build, Memory & Release Configuration)
- `NFR-007` (Build, Memory & Release Configuration)
- `FR-001` (ONNX Model & ST AI Runtime)
- `FR-004` (ONNX Model & ST AI Runtime)
- `IF-003` (ONNX Model & ST AI Runtime)
- `IF-004` (ONNX Model & ST AI Runtime)
- `FR-005` (ONNX Model & ST AI Runtime)
- `FR-006` (ONNX Model & ST AI Runtime)
- `FR-007` (ONNX Model & ST AI Runtime)
- `FR-009` (ONNX Model & ST AI Runtime)
- `FR-012` (ONNX Model & ST AI Runtime)
- `NFR-002` (ONNX Model & ST AI Runtime)
- `FR-024` (ONNX Model & ST AI Runtime)
- `FR-031` (ONNX Model & ST AI Runtime)
- `HW-005` (ONNX Model & ST AI Runtime)
- `FR-038` (ONNX Model & ST AI Runtime)
- `FR-010` (Inference Test Harness & Verification)
- `FR-011` (Inference Test Harness & Verification)
- `FR-014` (Inference Test Harness & Verification)
- `FR-015` (Inference Test Harness & Verification)
- `FR-016` (Inference Test Harness & Verification)
- `FR-017` (Inference Test Harness & Verification)
- `FR-018` (Inference Test Harness & Verification)
- `FR-019` (Inference Test Harness & Verification)
- `FR-020` (Inference Test Harness & Verification)
- `FR-028` (Inference Test Harness & Verification)
- `FR-029` (Inference Test Harness & Verification)
- `FR-036` (Inference Test Harness & Verification)

### No observable exists (1)

These need a human decision — either the requirement is restated as something checkable, or it is accepted as unverifiable on the record. Inventing a test for one of these produces a test that passes and proves nothing.

- `FR-003` — The result output is concise. · _A maximum output length or objective definition of “concise” is missing._
