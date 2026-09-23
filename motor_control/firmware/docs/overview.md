<!-- FirmGen source overview v1 -->
# motor control

Source-based overview for the generated stm32 firmware. This page records the current source structure and statically identified interfaces. Building the firmware or generating this report does not establish runtime correctness.

Application entry source: `Core/Src/main.c`.

## Modules and runtime entry points

- `Core/Inc` — Core/Inc (module, 4 source/header files).
- `Core/Src` — Core/Src (module, 4 source/header files).
- `app` — app (application, 2 source/header files).
- `bsp` — bsp (board, 2 source/header files).
- `configs` — configs (configuration, 4 source/header files).
- `drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1` — drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1 (driver, 2 source/header files).
- `osal` — osal (abstraction, 1 source/header files).
- `platforms/stm32` — platforms/stm32 (platform, 6 source/header files).
- `services/drive_safety` — services/drive_safety (module, 2 source/header files).
- `services/motor_control` — services/motor_control (module, 2 source/header files).
- `services/uart_command` — services/uart_command (module, 2 source/header files).
- `utils/helpers` — utils/helpers (utility, 2 source/header files).
- `utils/logger` — utils/logger (utility, 2 source/header files).
- Runtime entry `app_start` — app_start in `firmware/app/app.c`.

## Pins and interfaces found in source

- `PA5` — PA5 (gpio_output) at `firmware/app/app.c`.
- `PC9` — PC9 (gpio_output) at `firmware/platforms/stm32/ihm16m1_platform.c`.
- `PA11` — PA11 (gpio_input) at `firmware/platforms/stm32/ihm16m1_platform.c`.
- uart_device — `firmware/platforms/stm32/uart_platform.c`.

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
- `firmware/drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1/x_nucleo_ihm16m1.c`
- `firmware/drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1/x_nucleo_ihm16m1.h`
- `firmware/osal/osal.h`
- `firmware/platforms/stm32/ihm16m1_platform.c`
- `firmware/platforms/stm32/ihm16m1_platform.h`
- `firmware/platforms/stm32/uart_platform.c`
- `firmware/platforms/stm32/uart_platform.h`
- `firmware/platforms/stm32/watchdog_platform.c`
- `firmware/platforms/stm32/watchdog_platform.h`
- `firmware/services/drive_safety/drive_safety.c`
- `firmware/services/drive_safety/drive_safety.h`
- `firmware/services/motor_control/motor_control.c`
- `firmware/services/motor_control/motor_control.h`
- `firmware/services/uart_command/uart_command.c`
- `firmware/services/uart_command/uart_command.h`
- `firmware/utils/helpers/helpers.c`
- `firmware/utils/helpers/helpers.h`
- `firmware/utils/logger/logger.c`
- `firmware/utils/logger/logger.h`
