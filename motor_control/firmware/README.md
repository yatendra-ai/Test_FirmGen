# motor_control

Firmware target: **NUCLEO-G431RB / STM32G431RB**.

Hardware setup:

- Motor driver: **X-NUCLEO-IHM16M1**, populated with **STSPIN830**.
- Motor: **GBM2804H-100T** gimbal motor, configured for **7 pole pairs**.
- Laboratory supply: **12 V DC**.
- Motor outputs: U, V, and W through the IHM16M1 three-phase connection.
- Current sensing: IHM16M1 three-shunt network with phase-U, phase-V, and phase-W ADC inputs.
- PWM: three-PWM mode using STM32G431RB hardware timer outputs.

The signal routing is based on UM2415 Rev. 4, Figure 6 and Table 1. The supplied
manual identifies the signal names and Nucleo connector positions; the exact
STM32G431RB alternate-function routing must be confirmed against the assembled
board wiring before applying motor power.

The application starts in a safe stopped state. PWM is disabled and the driver
is held in standby until initialization and current-sense validation are complete.

## Project layout

- `app/` — application orchestration and `app_start()`.
- `drivers/x_nucleo_ihm16m1/` — portable X-NUCLEO-IHM16M1 driver.
- `platforms/stm32/` — STM32 HAL implementation of the driver I/O callbacks.
- `bsp/` — declarative board facts.
- `configs/` — compile-time hardware and application configuration.

## Build

Use the STM32Cube/CMake workflow for the `motor_control` project. The generated
project targets STM32G431RB with the STM32G4 HAL, CMSIS startup file, and the
`STM32G431RBTX_FLASH.ld` linker script.

## Target setup record

The inspection target record is NUCLEO-G431RB with X-NUCLEO-IHM16M1/STSPIN830,
GBM2804H-100T (7 pole pairs), and a 12 V DC supply. Hardware verification still
requires the assembled boards, motor, power supply, and oscilloscope/current
observation; source configuration alone does not prove physical waveforms.
