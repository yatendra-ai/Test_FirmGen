# motor_demo

## Purpose

This firmware targets the **NUCLEO-G431RB** and the **X-NUCLEO-IHM16M1 / STSPIN830** three-phase motor-driver expansion board. After reset it keeps the driver in standby for 3 seconds, checks EN/FAULT, enables the documented three-input interface, and automatically applies a bounded open-loop sinusoidal U/V/W demonstration pattern to the **GBM2804H-100T** motor.

The motor specification is seven pole pairs. The intended driver input is a measured 12 V DC supply. The setup must be current-limited and supervised because the driver and motor can heat during operation.

## Documented wiring and board configuration

UM2415 Rev. 4 documents the default MODE-low STSPIN830 interface, active-low STBY, active-low EN/FAULT shutdown, and the three-input control signals. The firmware configuration uses the following NUCLEO-G431RB signals for the stacked default mapping used by this project:

- TIM1 CH1 / PA8: INU (phase U)
- TIM1 CH2 / PA9: INV (phase V)
- TIM1 CH3 / PA10: INW (phase W)
- PC9: active-low STBY
- PB12: active-low EN/FAULT monitor
- PC8: MODE low for the factory-default three-input mode
- PA15, PB3, PB10: Hall inputs H1, H2, H3 reserved for the board interface
- Motor outputs: X-NUCLEO-IHM16M1 CN1 outputs 3, 4, and 5
- Motor supply: X-NUCLEO-IHM16M1 CN1 input 2 (VIN) and input 1 (GND)

The values are held in `firmware/configs/app_config.h`. The portable board driver is under `firmware/drivers/x_nucleo_ihm16m1/`; open-loop math and timing are under `firmware/modules/`; STM32 HAL access is confined to `firmware/platforms/stm32/`. No UART command, ADC control, closed-loop feedback, motor characterization, or RTOS path is part of this demonstration.

## Build and flash

```text
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build
```

Flash `build/motor_demo.elf` to the NUCLEO-G431RB with the ST-Link. The UART log is diagnostic only; motor start is automatic and does not require UART input.

## Evidence status

The current firmware builds and flashes successfully to the NUCLEO-G431RB. Runtime logs showed a clear EN/FAULT state, successful PWM enable, automatic sinusoidal rotation startup, and normal sequence completion; the connected GBM2804H-100T motor was observed rotating. The exact 12 V meter reading and oscilloscope timing measurements remain separate bench evidence. The specification's derived requirement for rejecting out-of-range supply voltage is not verifiable because no voltage-validity interface or permitted range was defined; this firmware does not invent one.
