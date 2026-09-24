<!-- FirmGen source overview v1 -->
# open loop motor control

Source-based overview for the generated stm32 firmware. This page records the current source structure and statically identified interfaces. Building the firmware or generating this report does not establish runtime correctness.

Application entry source: `Core/Src/main.c`.

## Modules and runtime entry points

- `Core/Inc` — Core/Inc (module, 4 source/header files).
- `Core/Src` — Core/Src (module, 4 source/header files).
- `app` — app (application, 2 source/header files).
- `bsp` — bsp (board, 2 source/header files).
- `configs` — configs (configuration, 4 source/header files).
- `drivers/x_nucleo_ihm16m1/include` — drivers/x_nucleo_ihm16m1/include (driver, 1 source/header files).
- `drivers/x_nucleo_ihm16m1/src` — drivers/x_nucleo_ihm16m1/src (driver, 1 source/header files).
- `interfaces` — interfaces (module, 4 source/header files).
- `modules/command_parser` — modules/command_parser (module, 2 source/header files).
- `modules/motor_waveform` — modules/motor_waveform (module, 2 source/header files).
- `osal` — osal (abstraction, 1 source/header files).
- `platforms/stm32` — platforms/stm32 (platform, 4 source/header files).
- `services/motor_lifecycle` — services/motor_lifecycle (module, 2 source/header files).
- `utils/helpers` — utils/helpers (utility, 2 source/header files).
- `utils/logger` — utils/logger (utility, 2 source/header files).

## Pins and interfaces found in source

- `PA5` — PA5 (gpio_output) at `Core/Src/main.c`.
- `PC9` — PC9 (gpio_output) at `firmware/platforms/stm32/stm32_motor_board.c`.
- `PA8` — PA8 (gpio_output) at `firmware/platforms/stm32/stm32_motor_board.c`.
- `PA9` — PA9 (gpio_output) at `firmware/platforms/stm32/stm32_motor_board.c`.
- `PA10` — PA10 (gpio_output) at `firmware/platforms/stm32/stm32_motor_board.c`.
- `PB12` — PB12 (gpio_input) at `firmware/platforms/stm32/stm32_motor_board.c`.
- pwm_output — `firmware/platforms/stm32/stm32_motor_board.c`.
- uart_device — `firmware/platforms/stm32/uart_stm32.c`.

This is a static inventory, not verified wiring. Check the Firmware Topology panel and board wiring for unresolved connections.

## Build, flash, and documentation

Open this project in FirmGen and select its configured board/toolchain. Use **Build** to compile, **Flash** to program the connected board, and **Monitor** to inspect UART output. Debug/Production telemetry switching rebuilds and reflashes the firmware.

The API report is in `api/html/index.html` beside this overview. Review application behavior on hardware separately. Expand this overview with intended behavior, operating instructions, and validation evidence as these become available.

## Analyzed source inventory

- `CMakeLists.txt`
- `Core/Inc/main.h`
- `Core/Inc/stm32g4xx_hal_conf.h`
- `Core/Inc/stm32g4xx_it.h`
- `Core/Inc/stm32g4xx_nucleo_conf.h`
- `Core/Src/main.c`
- `Core/Src/stm32g4xx_hal_msp.c`
- `Core/Src/stm32g4xx_it.c`
- `Core/Src/system_stm32g4xx.c`
- `firmware/app/app.c`
- `firmware/app/app.h`
- `firmware/bsp/board.c`
- `firmware/bsp/board.h`
- `firmware/configs/app_config.h`
- `firmware/configs/build_config.h`
- `firmware/configs/feature_config.h`
- `firmware/configs/platform_config.h`
- `firmware/drivers/x_nucleo_ihm16m1/include/x_nucleo_ihm16m1.h`
- `firmware/drivers/x_nucleo_ihm16m1/src/x_nucleo_ihm16m1.c`
- `firmware/interfaces/command_transport_if.c`
- `firmware/interfaces/command_transport_if.h`
- `firmware/interfaces/motor_board_if.h`
- `firmware/interfaces/uart_if.h`
- `firmware/modules/command_parser/command_parser.c`
- `firmware/modules/command_parser/command_parser.h`
- `firmware/modules/motor_waveform/motor_waveform.c`
- `firmware/modules/motor_waveform/motor_waveform.h`
- `firmware/osal/osal.h`
- `firmware/platforms/stm32/osal_stm32.c`
- `firmware/platforms/stm32/stm32_motor_board.c`
- `firmware/platforms/stm32/stm32_motor_board.h`
- `firmware/platforms/stm32/uart_stm32.c`
- `firmware/services/motor_lifecycle/motor_lifecycle.c`
- `firmware/services/motor_lifecycle/motor_lifecycle.h`
- `firmware/utils/helpers/helpers.c`
- `firmware/utils/helpers/helpers.h`
- `firmware/utils/logger/logger.c`
- `firmware/utils/logger/logger.h`
