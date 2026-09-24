<!-- FirmGen source overview v1 -->
# motor demo

Source-based overview for the generated STM32 firmware. This page records the current source structure, identified interfaces, and the latest hardware validation evidence. The NUCLEO-G431RB motor demonstration has been flashed and observed working.

Application entry source: `Core/Src/main.c`.

## Modules and runtime entry points

- `Core/Inc` — Core/Inc (module, 4 source/header files).
- `Core/Src` — Core/Src (module, 4 source/header files).
- `app` — app (application, 2 source/header files).
- `bsp` — bsp (board, 2 source/header files).
- `configs` — configs (configuration, 4 source/header files).
- `drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1` — drivers/x_nucleo_ihm16m1/x_nucleo_ihm16m1 (driver, 2 source/header files).
- `osal` — osal (abstraction, 1 source/header files).
- `platforms/stm32` — platforms/stm32 (platform, 2 source/header files).
- `utils/helpers` — utils/helpers (utility, 2 source/header files).
- `utils/logger` — utils/logger (utility, 2 source/header files).

## Pins and interfaces found in source

- `PC9` — PC9 (gpio_output) at `firmware/platforms/stm32/motor_platform.c`.
- `PC8` — PC8 (gpio_output) at `firmware/platforms/stm32/motor_platform.c`.
- `PB12` — PB12 (gpio_input) at `firmware/platforms/stm32/motor_platform.c`.
- `PB13`, `PB14`, `PB15` — STSPIN830 ENU, ENV, and ENW gate outputs, asserted only after PWM initialization.

The listed GPIO behavior has now been exercised on the connected NUCLEO-G431RB/X-NUCLEO-IHM16M1 assembly. The motor rotated during the automatic run; oscilloscope-level waveform and timing measurements remain separate evidence.

## Control scope and architecture boundaries

This is intentionally a minimal open-loop motor bring-up. The firmware contains only MCU GPIO/PWM initialization, STSPIN830 standby and EN/FAULT handling, a portable sinusoidal duty generator, a monotonic automatic sequencer, watchdog safety, and U/V/W motor demonstration output. It has no UART command path and does not depend on UART input.

The project deliberately contains no sensorless FOC, rotor observer, Clarke/Park transform, current or speed PI loop, phase-current or VBUS ADC control, ADC-triggered DMA, current-offset calibration, RPM measurement/regulation, motor characterization, or oscilloscope integration. Hall pins are documented as reserved board-interface signals and are not used as control feedback. The `firmware/osal/` header remains unused because this is a bare-metal application with no RTOS requirement.

## Fault handling and reset safety

The STM32 safety adapter latches an active-low STSPIN830 EN/FAULT condition, disables PWM, and returns the driver to standby. The independent hardware IWDG is initialized at startup and refreshed only from the responsive control path. PWM remains disabled and standby is asserted during initialization and the 3-second boot delay. Watchdog reset-cause and oscilloscope timing measurements require on-target validation.

## Automatic demonstration sequencer

`firmware/modules/motor_sequence/` owns the monotonic boot timeline: 0–3 s boot delay, 3–8 s startup ramp, 8–18 s hold, 18–20 s shutdown ramp, and a latched stopped state after 20 s. No UART input is used. PWM and driver disable are performed when the terminal state is reached or when a fault is detected.

## Open-loop sinusoidal control

The automatic run uses `firmware/modules/sine_control/` for vendor-neutral phase accumulation and three sinusoidal duties separated by 120 degrees. Starting at 3 seconds, frequency ramps from 1 Hz to 10 Hz and amplitude ramps from 10% to 20% over 5 seconds. The controller holds 10 Hz and 20% amplitude for 10 seconds, then ramps both to zero over 2 seconds and disables PWM. Hardware validation: after flashing the current image, the NUCLEO-G431RB reported a clear EN/FAULT state, enabled PWM successfully, started the automatic sinusoidal sequence, and the connected motor was observed rotating. The sequence completed and shut down normally.

## Demonstration configuration

The firmware targets the NUCLEO-G431RB with the X-NUCLEO-IHM16M1/STSPIN830, the GBM2804H-100T motor with seven pole pairs, and a 12 V DC driver supply. It holds STBY active for 3 seconds, checks the active-low EN/FAULT input, then starts an automatic bounded U/V/W commutation pattern. The motor outputs are the expansion-board CN1 motor terminals 3, 4, and 5. These facts come from `firmware/configs/app_config.h` and UM2415 Rev. 4. The current image was flashed successfully and the shaft was observed rotating; the exact 12 V meter reading remains separate bench evidence.

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
- `firmware/platforms/stm32/motor_platform.c`
- `firmware/platforms/stm32/motor_platform.h`
- `firmware/utils/helpers/helpers.c`
- `firmware/utils/helpers/helpers.h`
- `firmware/utils/logger/logger.c`
- `firmware/utils/logger/logger.h`
