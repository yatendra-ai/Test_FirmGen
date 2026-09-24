# Open-loop motor control

Category 1 implementation target: NUCLEO-G431RB with the X-NUCLEO-IHM16M1 STSPIN830 expansion board and a GBM2804H-100T three-phase motor.

## Build and target

- MCU: STM32G431RBT6, STM32G431RB device definition.
- Board: NUCLEO-G431RB.
- Build: CMake + Ninja with the `arm-none-eabi` toolchain.
- Startup/linker: `Core/Startup/startup_stm32g431rbtx.s` and `STM32G431RBTX_FLASH.ld`.
- Programming/debug: embedded ST-LINK-V3E over SWD; the board VCP is COM4 when detected in this environment.
- Application entry: `firmware/app/app.c:app_start()` after HAL and system-clock initialization.

## Category 1 hardware configuration

The supplied X-NUCLEO-IHM16M1 driver is integrated under `firmware/drivers/x_nucleo_ihm16m1/`. The platform adapter is under `firmware/platforms/stm32/` and keeps HAL calls out of the portable driver.

The UM2415 shield schematic identifies the factory-default MODE-low three-input interface. This implementation records the following source-derived routes for the NUCLEO-G431RB stack:

- INU / phase U input: PA8 / TIM1 channel 1.
- INV / phase V input: PA9 / TIM1 channel 2.
- INW / phase W input: PA10 / TIM1 channel 3.
- STBY: PC9, active low for standby; the platform holds it high only after safe GPIO/timer setup.
- EN/FAULT: PB12, active low according to UM2415's EN/FAULT truth table.
- Motor phase terminals: U, V and W, per the category 1 requirement and UM2415 getting-started procedure.

The driver is initialized in the factory three-PWM mode and PWM is disabled at startup. No motor movement is commanded automatically at reset. The 12 V DC supply and physical U/V/W motor wiring are bench setup conditions, not values that firmware can prove.

## Motor configuration

- Motor: GBM2804H-100T.
- Pole pairs: `APP_MOTOR_POLE_PAIRS` is exactly 7.
- Test supply assumption: `APP_MOTOR_SUPPLY_VOLTAGE_V` is 12 V DC.

These values document the frozen category 1 assumptions. Mechanical rotation, supply voltage, shield attachment, jumper configuration, and terminal wiring require hardware observation.

## UART command and status interface

The ST-Link Virtual COM Port uses USART2 on PA2 (TX) and PA3 (RX), configured as 115200 baud, 8 data bits, no parity, one stop bit, no hardware flow control. Commands are terminated by CR or LF; CRLF is accepted as one command delimiter. The command buffer is bounded to 64 bytes. An overlong command is discarded and returns `ERR INVALID` at the next delimiter.

Supported commands are `MOTOR START`, `MOTOR STOP`, `DIR CW`, `DIR CCW`, `FREQ <Hz>`, `DUTY <%>`, and `STATUS`. Responses are one-line `OK ...`, `ERR ...`, or `STATUS state=... dir=... freq=...Hz duty=...% fault=...`. `DUTY` is clamped to the 20% safety maximum. STATUS is designed to complete within 100 ms of a complete command line being received.

## Category 2 PWM review status

TIM1 is configured for synchronized three-channel PWM on PA8, PA9, and PA10 using the factory three-PWM STSPIN830 interface. The 170 MHz timer clock, prescaler 0, and center-aligned period 4249 produce a nominal 20 kHz carrier. The portable waveform module generates U/V/W duties with 120-degree offsets, starts at 10% amplitude, clamps amplitude to 20%, and ramps electrical frequency from 1 Hz toward 10 Hz.

The waveform update path is reachable from `main()` through `app_process()`. The UART command ingress now dispatches `DIR CW`, `DIR CCW`, `MOTOR START`, `MOTOR STOP`, `FREQ`, `DUTY`, and `STATUS`. No automatic start is enabled; the lifecycle remains stopped after initialization.

The category 2 build and ST-LINK flash succeeded. Earlier VCP captures remained silent; the command-enabled UART path has not yet produced a usable runtime response. No oscilloscope or logic-analyzer capture was available. PWM carrier frequency, three-phase synchronization, 120-degree envelopes, STBY state, and physical direction reversal remain hardware-unverified.

## Safety state

The platform prepares GPIO and TIM1 with all PWM compare values at zero and leaves STSPIN830 in standby. Fault monitoring and the open-loop control lifecycle are subsequent category work; this category intentionally does not claim motor rotation or full command handling.

## Evidence status

The firmware build is clean and produces the NUCLEO-G431RB image. Source inspection supports the board, MCU, pole-pair, supply-assumption, and U/V/W configuration entries above. The drive-enable transition, PWM waveform, motor rotation, measured 12 V supply, and flashed-image execution still require a hardware review run; no such observation is claimed here.
