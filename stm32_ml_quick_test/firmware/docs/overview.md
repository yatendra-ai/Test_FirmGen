# NUCLEO-C5A3ZG ML Quick Test Firmware

This STM32C5 firmware targets the NUCLEO-C5A3ZG / STM32C5A3ZGT6, using the generated board startup configuration for a 144 MHz Cortex-M33 with single-precision FPU and DSP. Its current application path initializes per-reset test state, uses the board's USART2 ST-LINK VCP route (PA2 TX, 115200 baud, 8-N-1, no flow control), and emits deterministic failure reporting until the supplied model and ST AI runtime integration is available. It deliberately does not print a fabricated model digest, readiness marker, or inference values.

## Current application modules

- `application/main.c` — calls the generated board initialization, initializes the generated basic stdio VCP route, and enters `app_start()`.
- `firmware/app/app.c` — initializes test status, counters, and comparison slots before reporting current integration failure.
- `firmware/services/result_report.c` — formats bounded startup, vector, failure, and final-summary lines and prevents PASS unless the run is complete and successful.
- `firmware/interfaces/console_if.h` — vendor-neutral synchronous console transmission contract.
- `firmware/platforms/stm32/console_stm32.c` — checks a bounded STM32 HAL UART transmit through the Cube-configured USART2 handle.
- `firmware/configs/app_config.h` — compile-time board/application values including five test slots and 100 ms console write timeout.

## Build and deploy

Use the project's STM32Cube CMake preset `debug_GCC_NUCLEO-C5A3ZG` through FirmGen's STM32 build/deploy workflow. The selected board configuration targets 1 MiB ROM and 256 KiB RAM. The linker currently reserves 512 bytes for heap and 1024 bytes for stack. Heap-free inference remains a requirement for the upcoming model integration; the presence of a reserved linker heap does not mean the test sequence may allocate dynamically.

## Current validation evidence

A clean integrated build and ST-LINK deployment succeeded for `debug_GCC_NUCLEO-C5A3ZG`. On the current NUCLEO-C5A3ZG reset, the fresh COM5 VCP segment began with SHA-256 `15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090`, then `ML_RUNTIME_READY`, five PASS comparisons in order positive, first_clamped, second_clamped, zero_input, both_clamped, and `ML_SUMMARY passed=5 total=5 status=PASS`. Milli outputs were `[500,4000]`, `[0,4000]`, `[7500,0]`, `[500,0]`, and `[0,0]`.

A second deploy/reset produced the same ordered results. Monitor capture included buffered previous serial bytes before the fresh model hash; those were excluded from the current-run segment. See `docs/validation/category5-serial-capture.log` and `docs/validation/validation-record.md`.

Final linker-map budget is ROM 29,600/1,048,576 bytes and RAM 2,336/262,144 bytes, including the 1,024-byte reserved stack. ST Edge AI reports 16 bytes activations and 24 bytes embedded weights. Runtime stack high-water instrumentation was not available.
