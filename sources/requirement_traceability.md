# Requirement traceability

Generated 2026-09-28T11:05:43.769269900+00:00
Specification frozen 2026-09-28T08:43:10.017Z

33 of 33 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 33 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## Board Integration, LEDs & User Input

gate: deferred · tasks: 5/5 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The firmware shall be a single integrated application that validates major NUCLEO-C5A3ZG board functions. | inspection, not verifiable | The delivered demo is built and delivered as one firmware application image rather than as separate per-function images. (expected: One firmware application image contains the integrated demo entry point.)<br>The demo validates every major board function identified by the PRD. (expected: Not assessable from the supplied information.) | firmware/app/app.c<br>firmware/platforms/stm32/board_io_stm32.c | app_start, board_io_init, board_io_set_led | — | implemented, untested |
| `HW-001` | The demo shall use onboard resources wherever possible to validate the major NUCLEO-C5A3ZG board functions. | inspection | The demo's resource-to-function mapping uses onboard resources wherever possible and identifies each exception. (expected: Each board function uses an onboard resource where possible; every exception is identified.) | firmware/app/app.c<br>firmware/platforms/stm32/board_io_stm32.c | app_start, board_io_init, board_io_set_led | — | implemented, untested |
| `HW-002` | The firmware shall initialize the onboard LEDs. | hardware / instrument | Each onboard LED responds to commands to turn it on and off after LED initialization. (expected: Each commanded LED visibly turns on and then off.) | firmware/app/app.c<br>firmware/platforms/stm32/board_io_stm32.c | app_process_task, board_io_init, board_io_set_led | — | implemented, untested |
| `FR-002` | The firmware shall blink a status LED after boot. | hardware / instrument | After boot, the status LED alternates between on and off at least once. (expected: The status LED changes from on to off or from off to on and then changes back after boot.) | firmware/app/app.c<br>firmware/platforms/stm32/board_io_stm32.c | app_process_task, board_io_init, board_io_set_led | — | implemented, untested |
| `FR-003` | The firmware shall use LEDs to indicate network state. | hardware / instrument | A change between distinct network states produces visibly distinguishable LED indications. (expected: The LED patterns for the two distinct network states are visibly distinguishable.) | firmware/app/app.c<br>firmware/platforms/stm32/board_io_stm32.c | app_start, board_io_set_led | — | implemented, untested |
| `FR-004` | The firmware shall use LEDs to indicate MQTT state. | hardware / instrument | A change between distinct MQTT states produces visibly distinguishable LED indications. (expected: The LED patterns for the two distinct MQTT states are visibly distinguishable.) | firmware/app/app.c<br>firmware/platforms/stm32/board_io_stm32.c | app_start, board_io_set_led | — | implemented, untested |
| `FR-005` | The firmware shall detect the onboard USER button. | hardware / instrument | A physical press of the onboard USER button produces a detectable press event. (expected: A press event is recorded after the physical press.) | firmware/app/app.c<br>firmware/modules/button_logic.c<br>firmware/platforms/stm32/board_io_stm32.c | app_process_task, board_io_button_is_pressed, button_logic_update | — | implemented, untested |
| `FR-006` | Each press of the onboard USER button shall toggle an application state. | host test | Each detected button-press event changes the application state from its current value to its toggled value. (expected: The state is toggled after each call and returns to its initial value after two calls.) | firmware/app/app.c<br>firmware/modules/button_logic.c | app_process_task, button_logic_toggle_state | — | implemented, untested |
| `FR-012` | The firmware shall register each physical USER button actuation as exactly one press event despite switch contact transitions. | hardware / instrument | One physical USER-button actuation, including its contact transitions, produces exactly one press event and exactly one application-state toggle. (expected: The event count increases by exactly one, and the application state is toggled exactly once.) | firmware/app/app.c<br>firmware/modules/button_logic.c<br>firmware/platforms/stm32/board_io_stm32.c | app_process_task, board_io_button_is_pressed, button_logic_update | — | implemented, untested |

Observed on the board:
- `FR-001`, `HW-001` — Latest post-fix deploy boots the app and starts ADC/PWM services; Ethernet later linked and acquired a configured IPv4 through its timeout fallback, though no visual LED/button observation was made. Requirement FR-001 is not fully demonstrated because the PRD does not list all major board functions to validate.
- `HW-002`, `FR-002` — Boot blink logic runs on PA5; no visual LED observation recorded. PG LED polarity remains an assumption.
- `FR-003`, `FR-004` — Network eventually became ready on the fallback address and MQTT connected in the latest run before MQTT adapter changes. No physical indicator LED visual confirmation recorded.
- `FR-005`, `FR-012` — PC13 is configured active-high/no-pull with 10 ms polling and 50 ms debounce. No physical press occurred in captured logs.
- `FR-006` — No physical button press was captured. Button toggle logic remains reachable; host test results are not imported.

## UART Console & Diagnostics

gate: deferred · tasks: 3/3 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-001` | The firmware shall use the ST-LINK Virtual COM Port at 115200 baud, 8 data bits, no parity, and 1 stop bit. | hardware / instrument | A serial terminal connected to the NUCLEO-C5A3ZG ST-LINK Virtual COM Port at 115200 baud, 8 data bits, no parity, and 1 stop bit receives firmware UART output. (expected: Firmware UART output is received and legible with the terminal configured for 115200 baud, 8-N-1.) | NUCLEO-C5A3ZG/generated/hal/mx_usart2.c<br>application/app_logger.c<br>firmware/platforms/stm32/diagnostics_uart_stm32.c | AppLoggerHeader, diagnostics_uart_write_line, mx_usart2_uart_init | — | implemented, untested |
| `DIAG-001` | The firmware shall print boot, network, HTTP, MQTT, ADC, PWM, and button status over UART. | hardware / instrument | The captured UART log contains identifiable status output for boot, network, HTTP, MQTT, ADC, PWM, and buttons. (expected: The captured log contains identifiable status output for all seven listed areas.) | firmware/app/app.c<br>firmware/utils/diagnostics/diagnostics.c | app_start, diagnostics_log_boot, diagnostics_log_status | — | implemented, untested |
| `DIAG-007` | The firmware shall print a consolidated board-health status periodically over UART. | hardware / instrument | The UART log contains at least two consolidated board-health status reports emitted at separate times. (expected: At least two consolidated board-health reports appear in the UART capture at separate times; no specific reporting interval is asserted.) | firmware/app/app.c<br>firmware/utils/diagnostics/diagnostics.c | app_process_task, diagnostics_log_health | — | implemented, untested |

Observed on the board:
- `IF-001` — Latest post-fix deploy serial output was legible on COM12/ST-LINK VCP at 115200; setup remains generated USART2 8-N-1.
- `DIAG-001` — Latest capture emitted boot, network, HTTP, MQTT, ADC, PWM, and button statuses. ADC/PWM correctly show ready after initialization; no HTTP result because there was no lease.
- `DIAG-007` — Latest post-fix deployment showed periodic health reports with network/HTTP not-ready, MQTT disconnected, and ADC/PWM ready. Multiple reports were seen during the capture.

## Ethernet Configuration & Network Validation

gate: deferred · tasks: 3/3 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `IF-002` | The firmware shall acquire network configuration through DHCP. | hardware / instrument | The IPv4 address assigned to the Ethernet interface matches the address offered in a DHCP ACK. (expected: The interface IPv4 address matches the address in the DHCP ACK.) | NUCLEO-C5A3ZG/generated/middleware/lwip/mx_lwip.c<br>NUCLEO-C5A3ZG/generated/parts/lan8742/mx_lan8742.c<br>application/app_lwip.c | AppLwipNetifExtCallbackFn, AppLwipThread, mx_lan8742_get_link_mode, mx_lwip_init | — | implemented, untested |
| `DIAG-002` | The firmware shall print the acquired IP address over UART. | hardware / instrument | The UART output contains the acquired IP address. (expected: The acquired IP address appears in the UART output.) | application/app_lwip.c<br>firmware/utils/diagnostics/diagnostics.c | AppLwipNetifExtCallbackFn, diagnostics_log_network_address | — | implemented, untested |
| `DIAG-003` | The firmware shall print the acquired gateway address over UART. | hardware / instrument | The UART output contains the acquired gateway address. (expected: The acquired gateway address appears in the UART output.) | application/app_lwip.c<br>firmware/utils/diagnostics/diagnostics.c | AppLwipNetifExtCallbackFn, diagnostics_log_network_address | — | implemented, untested |
| `DIAG-004` | The firmware shall print the acquired DNS server address over UART. | hardware / instrument | The UART output contains the acquired DNS server address. (expected: The acquired DNS server address appears in the UART output.) | application/app_lwip.c<br>firmware/utils/diagnostics/diagnostics.c | AppLwipNetifExtCallbackFn, diagnostics_log_network_address | — | implemented, untested |
| `IF-006` | The demo may validate NTP separately over UDP port 123 as additional networking validation; NTP validation shall not be treated as HTTP validation. | hardware / instrument | If NTP validation is run, its captured NTP exchange uses UDP port 123 and its result is evaluated separately from HTTP validation. (expected: The NTP exchange uses UDP destination port 123, and the NTP and HTTP validations have separate results.) | application/app_lwip.c | AppLwipNetifExtCallbackFn, AppLwipNtpTask | — | implemented, untested |

Observed on the board:
- `IF-002` — Final deploy shows LAN8742 link recovered during run: BSR 0x782d has link bit set; PHYSCSR 0x1058 read succeeded; LwIP server started waiting on static fallback 192.168.1.21:7 after DHCP timeout. No DHCP ACK/lease was seen, so DHCP acquisition itself remains unverified.
- `DIAG-002`, `DIAG-003`, `DIAG-004` — The app logs addresses on IPv4-valid callback; no DHCP lease/address output appeared in the captured window.
- `IF-006` — NTP UDP/123 is a separate post-IP task, but no lease callback occurred and no NTP request/packet capture was observed.

## MQTT Communication

gate: deferred · tasks: 5/5 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-007` | After each USER button press, the firmware shall publish the updated application state over MQTT. | hardware / instrument | After a USER-button press that toggles the application state, an MQTT publication carries the post-press application-state value. (expected: The publication contains the application-state value after the press, which differs from the value recorded before the press.) | firmware/app/app.c<br>firmware/modules/button_logic.c<br>firmware/platforms/stm32/mqtt_platform_stm32.c | app_process_task, button_logic_toggle_state, mqtt_platform_publish_state | — | implemented, untested |
| `IF-003` | The firmware shall connect to broker.hivemq.com:1883 using non-TLS MQTT. | hardware / instrument | The device establishes an MQTT connection to broker.hivemq.com over plaintext TCP port 1883. (expected: The peer is broker.hivemq.com on TCP port 1883, a successful MQTT CONNACK is observed, and no TLS handshake occurs.) | firmware/interfaces/mqtt_platform_if.h<br>firmware/platforms/stm32/mqtt_platform_stm32.c<br>firmware/services/mqtt_service.c | mqtt_platform_connection_callback, mqtt_platform_task, mqtt_service_start | — | implemented, untested |
| `FR-008` | The firmware shall publish telemetry every 5 seconds. | hardware / instrument | Consecutive telemetry MQTT publications are emitted at 5-second intervals. (expected: Each adjacent pair of telemetry publications is separated by 5 seconds.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/mqtt_platform_stm32.c | mqtt_platform_publish_telemetry | — | implemented, untested |
| `IF-004` | The firmware shall publish MQTT messages to the topic /firmgen/c5/telemetry. | hardware / instrument | A telemetry MQTT publication is sent on the topic /firmgen/c5/telemetry. (expected: The topic field is exactly /firmgen/c5/telemetry.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/mqtt_platform_stm32.c | mqtt_platform_publish_telemetry | — | implemented, untested |
| `IF-005` | The firmware shall subscribe to the MQTT topic /firmgen/c5/control. | hardware / instrument | The device sends an MQTT SUBSCRIBE request for /firmgen/c5/control. (expected: The SUBSCRIBE packet contains the topic filter /firmgen/c5/control.)<br>A publication made to /firmgen/c5/control is delivered to the device over its MQTT connection on TCP port 1883. (expected: The device connection uses TCP port 1883 and the device receives an MQTT PUBLISH packet on /firmgen/c5/control.) | firmware/platforms/stm32/mqtt_platform_stm32.c | mqtt_platform_incoming_data_callback, mqtt_platform_on_connected | — | implemented, untested |
| `IF-007` | When an MQTT connection is lost and network connectivity is available again, the firmware shall re-establish the MQTT connection. | hardware / instrument | After network connectivity is restored following an MQTT disconnection, the device establishes a new MQTT connection to the broker. (expected: A new connection attempt from the device is followed by a successful MQTT CONNACK.) | firmware/interfaces/mqtt_platform_if.h<br>firmware/platforms/stm32/mqtt_platform_stm32.c<br>firmware/services/mqtt_service.c | mqtt_platform_connection_callback, mqtt_platform_task, mqtt_service_start | — | implemented, untested |

Observed on the board:
- `FR-007` — The button path passes the updated application state into MQTT publishing, but no physical press or state-specific MQTT publication was observed in the latest monitor capture.
- `IF-003`, `IF-007` — Latest build/flash/monitor logged LAN8742 link-up (BSR 0x782d, link bit set), MQTT CONNACK accepted from broker.hivemq.com:1883 over plaintext TCP, and MQTT marked ready in board health. An interruption/reconnect cycle was not tested.
- `FR-008`, `IF-004` — Latest monitor logged telemetry publish requests on /firmgen/c5/telemetry. No packet capture confirmed broker receipt or measured adjacent publish packet timestamps to verify the exact 5-second interval.
- `IF-005` — Latest monitor logged accepted CONNACK and subscription request result 0 for /firmgen/c5/control. No inbound control publication was sent or captured.

## HTTP Weather & Time Services

gate: deferred · tasks: 3/3 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-009` | The firmware shall use HTTP GET to fetch current Bengaluru weather information from a public endpoint such as Open-Meteo. | hardware / instrument | A captured HTTP GET to a public weather endpoint results in weather data being delivered to the firmware. (expected: The request method is GET, the endpoint is public, and a nonempty weather-data response is received by the firmware.)<br>The weather request selects Bengaluru, Karnataka as its location. (expected: The selected weather location is Bengaluru, Karnataka.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/http_platform_stm32.c<br>firmware/services/http_service.c | APP_HTTP_WEATHER_HOST, APP_HTTP_WEATHER_LOCATION, APP_HTTP_WEATHER_URI, http_platform_get, http_platform_parse_response, http_platform_task | — | implemented, untested |
| `FR-010` | The firmware shall fetch the current date and time using an HTTP-accessible time service. | hardware / instrument | The firmware receives the current date and time from an HTTP-accessible time service. (expected: The firmware's date and time fields are populated from a nonempty time-service response.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/http_platform_stm32.c<br>firmware/services/http_service.c | APP_HTTP_TIME_HOST, APP_HTTP_TIME_URI, http_platform_get, http_platform_parse_response, http_platform_task | — | implemented, untested |
| `FR-011` | For the assumed sample data, the weather location shall be Bengaluru, Karnataka. | inspection | The assumed weather sample data identifies its location as Bengaluru, Karnataka. (expected: The location is Bengaluru, Karnataka.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/http_platform_stm32.c | APP_HTTP_WEATHER_HOST, APP_HTTP_WEATHER_LOCATION, APP_HTTP_WEATHER_URI | — | implemented, untested |

Observed on the board:
- `FR-009`, `FR-010` — Latest run reached network-ready after Ethernet link-up and static fallback address, but no HTTP GET/response diagnostic appeared in the capture; weather and time data were not observed. HTTP GET path now issues plain TCP/80 requests when network is available; runtime result remains unverified.
- `FR-010` — WorldTimeAPI /api/timezone/Asia/Kolkata is configured; no response-derived date/time was observed in the latest capture.

Notes recorded with these traces:
- `FR-009`, `FR-011` — Inspection: app_config.h selects Open-Meteo current weather with Bengaluru coordinates 12.9716,77.5946 and label Bengaluru, Karnataka.

## ADC & Analog Measurement

gate: deferred · tasks: 3/3 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-003` | The firmware shall validate the ADC without external hardware by sampling internal VREFINT and the MCU internal temperature sensor. | hardware / instrument, not verifiable | With no external analog source connected, the firmware obtains ADC samples from both VREFINT and the MCU internal temperature sensor. (expected: A successful sample result is obtained for each of the two internal channels.)<br>The plausibility of the VREFINT and temperature-sensor readings cannot be assessed against a defined acceptance range. (expected: No pass/fail range or plausibility definition is specified.) | NUCLEO-C5A3ZG/generated/hal/mx_adc1.c<br>firmware/platforms/stm32/adc_platform_stm32.c | adc_platform_read_injected, adc_platform_start, adc_platform_task, mx_adc1_init | — | implemented, untested |
| `DIAG-005` | The firmware shall print raw ADC values. | hardware / instrument | After ADC sampling, the UART output contains the raw ADC sample value or values. (expected: The UART output contains numeric raw ADC sample value or values.) | firmware/platforms/stm32/adc_platform_stm32.c | adc_platform_task | — | implemented, untested |
| `DIAG-006` | The firmware shall print converted ADC values. | hardware / instrument | After ADC sampling, the UART output contains the converted ADC measurement or measurements. (expected: The UART output contains at least one numeric converted ADC measurement.) | firmware/platforms/stm32/adc_platform_stm32.c | adc_platform_task | — | implemented, untested |

Observed on the board:
- `HW-003` — Latest deploy logged successful VREFINT/TEMPSENSOR injected sampling. Raw sample 1517/816 yielded VDDA 3617 mV, temperature input 720 mV, and calibrated temperature 30 C. Another early zero raw temperature sample was flagged invalid. Acquisition of both channels is observable.
- `DIAG-005`, `DIAG-006` — Latest capture printed raw VREFINT/TEMPSENSOR values and converted VDDA, sensor voltage, and temperature over the VCP. A zero raw temperature sample is reported invalid rather than a plausible converted temperature.

Notes recorded with these traces:
- `HW-003` — The separate plausibility judgment criterion is not verifiable as written: the spec defines no acceptance range, tolerance, or calibration assumptions.

## PWM Timer & Output

gate: deferred · tasks: 2/2 done · recorded in: 03e8f425-9f58-430f-b520-d188751ac0fc

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-004` | The firmware shall generate PWM using a hardware timer on an available PWM-capable header pin. | inspection | The firmware configuration assigns a hardware-timer PWM channel to a header pin whose board mapping supports that timer channel's PWM alternate function. (expected: The configured output is a PWM-capable header pin mapped to the configured hardware-timer PWM channel.) | NUCLEO-C5A3ZG/generated/hal/mx_tim2.c<br>NUCLEO-C5A3ZG/generated/hal/mx_tim2.h<br>firmware/platforms/stm32/pwm_platform_stm32.c | pwm_platform_start | — | implemented, untested |
| `PERF-001` | The default PWM test shall use a frequency of 1 kHz. | hardware / instrument | With the default PWM test running, the PWM output frequency measured at the header pin is 1 kHz. (expected: 1 kHz.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_platform_stm32.c | pwm_platform_start | — | implemented, untested |
| `PERF-002` | The default PWM test shall use a 50% duty cycle. | hardware / instrument | With the default PWM test running, the PWM output duty cycle measured at the header pin is 50%. (expected: 50%.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_platform_stm32.c | pwm_platform_start | — | implemented, untested |
| `DIAG-008` | The PWM waveform shall be stable at 1 kHz and 50% duty cycle when verified with an oscilloscope or logic analyzer. | hardware / instrument, not verifiable | With PWM output enabled, the waveform frequency measured at the output pin is 1 kHz. (expected: 1 kHz.)<br>With PWM output enabled, the waveform duty cycle measured at the output pin is 50%. (expected: 50%.)<br>The waveform's stability cannot be accepted as a bounded check until an observation interval and allowable frequency and duty-cycle variation are specified. (expected: Not defined by the requirement; an observation interval and allowable variation are needed.) | firmware/configs/app_config.h<br>firmware/platforms/stm32/pwm_platform_stm32.c | pwm_platform_start | — | implemented, untested |

Observed on the board:
- `HW-004` — Latest final deployment logged successful TIM2_CH2 PWM startup on PB3. Board knowledge and generated timer header map this to PB3/D3 and AF1. No electrical waveform measurement was captured.
- `PERF-001`, `PERF-002`, `DIAG-008` — Configuration targets 1 kHz and 50% duty and the final monitor logged PWM active. No oscilloscope/logic-analyzer reading establishes actual frequency or duty. Stability is not verifiable as written: the specification omits measurement interval and tolerance.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### Host tests you could write (1)

These have a `host` acceptance criterion and cite no test. They are tested at the end, from Artifacts -> Unit Tests.

- `FR-006` (Board Integration, LEDs & User Input)

### Acceptance needs non-host evidence (32)

No `host` acceptance criterion is recorded. Host tests cannot settle these acceptance claims, but implementation calculations and failure recovery can still need regression tests. Each line identifies the required acceptance evidence; do not substitute a host result for it.

- `FR-001` — checked by inspection, not verifiable · _Board Integration, LEDs & User Input_
- `HW-001` — checked by inspection · _Board Integration, LEDs & User Input_
- `HW-002` — checked by hardware / instrument · _Board Integration, LEDs & User Input_
- `FR-002` — checked by hardware / instrument · _Board Integration, LEDs & User Input_
- `FR-003` — checked by hardware / instrument · _Board Integration, LEDs & User Input_
- `FR-004` — checked by hardware / instrument · _Board Integration, LEDs & User Input_
- `FR-005` — checked by hardware / instrument · _Board Integration, LEDs & User Input_
- `FR-012` — checked by hardware / instrument · _Board Integration, LEDs & User Input_
- `IF-001` — checked by hardware / instrument · _UART Console & Diagnostics_
- `DIAG-001` — checked by hardware / instrument · _UART Console & Diagnostics_
- `DIAG-007` — checked by hardware / instrument · _UART Console & Diagnostics_
- `IF-002` — checked by hardware / instrument · _Ethernet Configuration & Network Validation_
- `DIAG-002` — checked by hardware / instrument · _Ethernet Configuration & Network Validation_
- `DIAG-003` — checked by hardware / instrument · _Ethernet Configuration & Network Validation_
- `DIAG-004` — checked by hardware / instrument · _Ethernet Configuration & Network Validation_
- `IF-006` — checked by hardware / instrument · _Ethernet Configuration & Network Validation_
- `FR-007` — checked by hardware / instrument · _MQTT Communication_
- `IF-003` — checked by hardware / instrument · _MQTT Communication_
- `FR-008` — checked by hardware / instrument · _MQTT Communication_
- `IF-004` — checked by hardware / instrument · _MQTT Communication_
- `IF-005` — checked by hardware / instrument · _MQTT Communication_
- `IF-007` — checked by hardware / instrument · _MQTT Communication_
- `FR-009` — checked by hardware / instrument · _HTTP Weather & Time Services_
- `FR-010` — checked by hardware / instrument · _HTTP Weather & Time Services_
- `FR-011` — checked by inspection · _HTTP Weather & Time Services_
- `HW-003` — checked by hardware / instrument, not verifiable · _ADC & Analog Measurement_
- `DIAG-005` — checked by hardware / instrument · _ADC & Analog Measurement_
- `DIAG-006` — checked by hardware / instrument · _ADC & Analog Measurement_
- `HW-004` — checked by inspection · _PWM Timer & Output_
- `PERF-001` — checked by hardware / instrument · _PWM Timer & Output_
- `PERF-002` — checked by hardware / instrument · _PWM Timer & Output_
- `DIAG-008` — checked by hardware / instrument, not verifiable · _PWM Timer & Output_

### Implemented, never tested (33)

Code exists and no test is recorded against it.

- `FR-001` (Board Integration, LEDs & User Input)
- `HW-001` (Board Integration, LEDs & User Input)
- `HW-002` (Board Integration, LEDs & User Input)
- `FR-002` (Board Integration, LEDs & User Input)
- `FR-003` (Board Integration, LEDs & User Input)
- `FR-004` (Board Integration, LEDs & User Input)
- `FR-005` (Board Integration, LEDs & User Input)
- `FR-006` (Board Integration, LEDs & User Input)
- `FR-012` (Board Integration, LEDs & User Input)
- `IF-001` (UART Console & Diagnostics)
- `DIAG-001` (UART Console & Diagnostics)
- `DIAG-007` (UART Console & Diagnostics)
- `IF-002` (Ethernet Configuration & Network Validation)
- `DIAG-002` (Ethernet Configuration & Network Validation)
- `DIAG-003` (Ethernet Configuration & Network Validation)
- `DIAG-004` (Ethernet Configuration & Network Validation)
- `IF-006` (Ethernet Configuration & Network Validation)
- `FR-007` (MQTT Communication)
- `IF-003` (MQTT Communication)
- `FR-008` (MQTT Communication)
- `IF-004` (MQTT Communication)
- `IF-005` (MQTT Communication)
- `IF-007` (MQTT Communication)
- `FR-009` (HTTP Weather & Time Services)
- `FR-010` (HTTP Weather & Time Services)
- `FR-011` (HTTP Weather & Time Services)
- `HW-003` (ADC & Analog Measurement)
- `DIAG-005` (ADC & Analog Measurement)
- `DIAG-006` (ADC & Analog Measurement)
- `HW-004` (PWM Timer & Output)
- `PERF-001` (PWM Timer & Output)
- `PERF-002` (PWM Timer & Output)
- `DIAG-008` (PWM Timer & Output)

### No observable exists (3)

These need a human decision — either the requirement is restated as something checkable, or it is accepted as unverifiable on the record. Inventing a test for one of these produces a test that passes and proves nothing.

- `FR-001` — The demo validates every major board function identified by the PRD. · _The PRD's list of major board functions and a checkable validation outcome for each function are missing._
- `HW-003` — The plausibility of the VREFINT and temperature-sensor readings cannot be assessed against a defined acceptance range. · _The acceptance criteria call the readings “plausible” but provide no valid value ranges, tolerance, calibration assumptions, or other definition of plausibility._
- `DIAG-008` — The waveform's stability cannot be accepted as a bounded check until an observation interval and allowable frequency and duty-cycle variation are specified. · _The requirement provides no stability tolerance or observation duration, so a stable-versus-unstable waveform cannot be judged reproducibly._
