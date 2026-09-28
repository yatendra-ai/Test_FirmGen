# NUCLEO-C5A3ZG Board Validation Demo

This firmware extends ST's `tcp_echo_freertos` base with a board demo, periodic VCP diagnostics, DHCP reporting, an optional UDP NTP request, MQTT and HTTP client attempts, internal ADC measurements, and a timer PWM output. Firmware build success is distinct from hardware verification; the current deployment logs a LAN8742 link-down state, which blocks all IP networking.

## Board wiring and behavior

- **LD1:** PA5, driven active-high according to the generated LED part. The app schedules three boot blink cycles.
- **LD2/LD3:** PG1/PG2, configured active-low from the board-specific assumption. Confirm this polarity visually before relying on the network/MQTT indicators.
- **USER button:** B1 on PC13, active-high, no-pull input. Firmware polls every 10 ms and uses a 50 ms stable-level debounce; one debounced press toggles the application state.
- **UART:** ST-LINK VCP on USART2, 115200 baud, 8 data bits, no parity, one stop bit. Boot, subsystem, button, address and periodic health output use this channel.
- **Ethernet:** generated LAN8742/RMII interface with LwIP DHCP. If an IPv4-valid event arrives, firmware reports IP, gateway and DNS and sends a separate NTP-format UDP request to port 123.
- **PWM:** TIM2_CH2 on PB3, Arduino D3 / CN11 pin 4, AF1. The target is 1 kHz and 50% duty. The current monitor log confirms that the firmware started this timer output, not the electrical waveform.
- **MQTT:** configured for plaintext `broker.hivemq.com:1883`, telemetry `/firmgen/c5/telemetry`, control `/firmgen/c5/control`, 5-second telemetry, button-state publishing and reconnect attempts. The MQTT adapter calls the registered lwIP MQTT client's documented public API directly; the registered component provides those API sources.

- **HTTP:** configured GETs for Open-Meteo current weather at Bengaluru coordinates (12.9716, 77.5946) and WorldTimeAPI for Asia/Kolkata. The STM32 adapter uses lwIP TCP sockets for plaintext HTTP on port 80, parses the response status/body, and accepts data only when expected service fields appear. A later Ethernet-link-up run was observed; HTTP requests were enabled in the final adapter, but that capture did not include HTTP response lines, so weather/time receipt remains unverified.
- **ADC1:** injected sampling of internal VREFINT and temperature-sensor channels, with the generated 289-cycle sample time. The driver reports raw readings, calibrated VDDA and temperature. Zero/invalid temperature samples are reported as invalid rather than as plausible measurements.

## Latest hardware evidence and limitations

The most recent fresh deployment built and flashed successfully and emitted boot/health logs, ADC readings and `TIM2_CH2 PWM started on PB3: target 1 kHz, 50 percent duty`. The connected PHY was directly observed: repeated LAN8742 BSR reads were `0x7809`; link-status mask `0x0004` is clear. The interface stayed `link_up=0`, waiting for IP, and the HTTP task waited for network. No DHCP lease/ACK, NTP packet exchange, HTTP response, or MQTT CONNACK/publication was observed. MQTT and HTTP remain implemented in their current source but are runtime-blocked by unavailable network. This measured link-down finding must not be taken as successful IP validation.

No oscilloscope/logic-analyzer measurement was recorded for PWM frequency/duty, no direct visual LED/button observation was recorded, and no host unit-test results have been imported. The PRD does not define a plausibility range for ADC readings or an observation interval/tolerance for PWM stability; those criteria cannot be judged reproducibly as written. The PG LED active-low configuration remains an assumption pending visual confirmation.

## Build and documentation

Build using the project's `debug_GCC_NUCLEO-C5A3ZG` CMake preset through FirmGen. Use the connected ST-LINK to flash and monitor the USART2 VCP. The generated API report is at `api/html/index.html`. Its current Doxygen warning is inherited from the ST example's `application/example.h` (`@user` is not a recognized Doxygen command); that ST-owned file was not modified.

## Layer inventory

- `firmware/app/`: application startup and coordination.
- `firmware/services/`: ADC, HTTP, MQTT and PWM service entry points.
- `firmware/modules/`: vendor-independent button debounce and network state.
- `firmware/interfaces/`: app/platform contracts.
- `firmware/platforms/stm32/`: HAL, FreeRTOS and LwIP-backed adapters.
- `firmware/bsp/`: board identity/facts.
- `firmware/configs/app_config.h`: application pins, periods, endpoint/topic strings and PWM defaults.
- `firmware/utils/`: diagnostics, logging and helper code.
- `application/` and `NUCLEO-C5A3ZG/generated/`: ST example and Cube-generated integration/glue.
