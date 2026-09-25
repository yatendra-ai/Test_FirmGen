# Customer PWM bring-up firmware

Target: NUCLEO-C5A3ZG / STM32C5A3ZGT6, GCC and STM32CubeC5 HAL2.
This is an engineering handover snapshot, not a completed or hardware-qualified product.
The product requirements are supplied separately.

## Build

Requires CMake 3.20+, Ninja, arm-none-eabi GCC with newlib, and STM32CubeC5.
STM32CubeCLT 1.22.0 supplies the tested compiler, CMake and Ninja. Put their `bin`
directories on PATH (or use its developer shell).
The Cube package defaults to `%USERPROFILE%/STM32Cube/Repository/STM32CubeC5`.
Override `STM32CUBEC5_DIR` via environment or the CMake command line on another machine.

```powershell
cmake --preset debug_GCC_NUCLEO-C5A3ZG
cmake --build --preset debug_GCC_NUCLEO-C5A3ZG
```

Outputs: `build/debug/pwm_d7_customer.elf`, `.map` and `libcustomer_drivers.a`.
Use the release preset for an optimized build. Do not reuse somebody else's CMake cache.

## Integration contracts

- `drivers/inc` exposes the customer PWM and boot-record APIs; `customer_drivers`
  is a real source-built static library. Keep the application hardware-independent
  by using these APIs. Reuse existing components; explain any necessary changes.
- PWM owns PA8 / TIM1_CH1 (Arduino D7, CN9 pin 8). The driver sets duty in permille
  (0..1000). Set the carrier before starting; duty updates do not change its period.
- `generated/` holds the board setup and `user_modifiable/Device/STM32C5A3ZGT6/`
  holds the project startup, system code and custom linker script. The DFP selection
  uses `CMSIS_RTE_FOLDER` to select these local files rather than the pack templates.
- The link contract includes `.app_config` at `0x080FF800`, `.noinit` at `0x2003FC00`,
  and a copied `.ramfunc` section. Preserve their locations, symbols and copy/zero tables.
  The reserved 2 KiB config region is a fixture layout, not a claim about flash erase
  granularity. No flash-writing driver is supplied. The ELF includes engineering defaults;
  programming it is not a promise to preserve factory data on another device.
- `.noinit` is only a best-effort warm-reset breadcrumb. Power loss, a debugger or
  startup memory policies can invalidate it; the driver validates it before use.

UART telemetry is not part of this handover. The firmware does not transmit diagnostic
bytes on the ST-LINK VCP, so an empty COM12 capture is expected and is not a PWM
verification result. A steady LD1 indicates the normal application path; a repeating
error blink indicates that initialization or PWM startup stopped with a recorded reason.

For validation, connect a high-impedance oscilloscope or logic-analyzer probe from
Arduino D7 (CN9 pin 8 / PA8) to a board ground. Measure the 3.3 V logic waveform,
carrier frequency, duty-cycle endpoints, and the 10-second ramp cycle. Do not connect
a motor or other high-power load directly to PA8; use an external driver stage.

No oscilloscope or logic-analyzer result is included in this source handover; the
waveform and timing requirements remain hardware-verification work until measured.

## Electrical precautions

D7 is 3.3 V logic, not a variable 5 V rail. Use only a high-impedance scope/logic-analyzer
input for initial validation. Never connect a motor or other high-power load directly.
This development snapshot can leave the output continuously high. Review it before
flashing; no automatic flashing is performed by the build.
