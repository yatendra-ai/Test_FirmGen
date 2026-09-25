<!-- FirmGen source overview v1 -->
# pwm pa8 driver

Source-based overview for the generated stm32 firmware. This page records the current source structure and statically identified interfaces. Building the firmware or generating this report does not establish runtime correctness.

Application entry source: `application/main.c`.

This firmware starts a 1 kHz PWM carrier on Arduino D7 / PA8 using TIM1 channel 1. The initial compare value is 0%, keeping PA8 inactive until the timer and alternate-function routing are configured and enabled. Override the carrier with `APP_PWM_FREQUENCY_HZ` in `firmware/configs/app_config.h`. PA8 is a 3.3 V MCU logic output, not a variable 5 V supply; use an external driver stage for loads beyond GPIO electrical limits. Never connect a high-power load directly to PA8. USB ST-LINK provides development power, programming, and debugging through the board USB connection during verification.

## Modules and runtime entry points

- `app` — app (application, 2 source/header files).
- `application` — application (application, 2 source/header files).
- `bsp` — bsp (board, 2 source/header files).
- `configs` — configs (configuration, 4 source/header files).
- `osal` — osal (abstraction, 1 source/header files).
- `platforms/stm32` — platforms/stm32 (platform, 3 source/header files).
- `utils/helpers` — utils/helpers (utility, 2 source/header files).
- `utils/logger` — utils/logger (utility, 2 source/header files).

## Pins and interfaces found in source

- uart_device — `application/main.c`.

This is a static inventory, not verified wiring. Check the Firmware Topology panel and board wiring for unresolved connections.

## Validation evidence

- Build: clean STM32C5A3ZG ELF produced with the debug GCC preset.
- Runtime: flashed over ST-LINK USB; VCP output showed `TIM1 CH1 PWM started on PA8 at 1000 Hz`.
- Physical waveform: not electrically measured; the PA8/D7 carrier and ramp remain due for oscilloscope verification.
- Safety: no high-power external load was connected; any such load requires an external driver stage.
- Host tests: none owed; the frozen requirements have no host acceptance criteria.


## Build, flash, and documentation

Open this project in FirmGen and select its configured board/toolchain. Use **Build** to compile, **Flash** to program the connected board, and **Monitor** to inspect UART output. Debug/Production telemetry switching rebuilds and reflashes the firmware.

The API report is in `api/html/index.html` beside this overview. Review application behavior on hardware separately. Expand this overview with intended behavior, operating instructions, and validation evidence as these become available.

## Analyzed source inventory

- `NUCLEO-C5A3ZG/template_start_from_nucleo.ioc2`
- `application/main.c`
- `application/main.h`
- `firmware/app/app.c`
- `firmware/app/app.h`
- `firmware/bsp/board.c`
- `firmware/bsp/board.h`
- `firmware/configs/app_config.h`
- `firmware/configs/build_config.h`
- `firmware/configs/feature_config.h`
- `firmware/configs/platform_config.h`
- `firmware/osal/osal.h`
- `firmware/platforms/stm32/pwm_pa8.c`
- `firmware/platforms/stm32/pwm_pa8.h`
- `firmware/platforms/stm32/stm32_ll.h`
- `firmware/utils/helpers/helpers.c`
- `firmware/utils/helpers/helpers.h`
- `firmware/utils/logger/logger.c`
- `firmware/utils/logger/logger.h`
- `NUCLEO-C5A3ZG/generated/hal/mx_usart2.c`
- `NUCLEO-C5A3ZG/generated/hal/mx_usart2.h`
- `NUCLEO-C5A3ZG/generated/utilities/basic_stdio/mx_basic_stdio_app.c`
- `NUCLEO-C5A3ZG/generated/utilities/basic_stdio/mx_basic_stdio_app.h`
