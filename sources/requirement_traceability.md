# Requirement traceability

Generated 2026-09-24T09:14:14.553345400+00:00
Specification frozen 2026-09-24T07:58:16.464Z

43 of 43 requirements have code recorded against them — 0 with tests passing, 0 with tests written but not passing, 43 implemented with no test at all. 0 have nothing recorded.

**Host unit tests.** No host unit-test results have been imported for this project. Host tests run on a separate machine; nothing here rests on host test evidence.

Everything below is derived from what the agent recorded with `requirement_trace` and from the frozen specification. A blank cell means nothing was recorded, which is not the same as nothing being needed.

## ESP32-C3 Platform & Power

gate: deferred · tasks: 2/2 done · recorded in: 29ce0a85-c3a6-412f-a563-5c13a90ef91d

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `HW-001` | The microcontroller unit shall be an ESP32-C3 development board. | inspection | The BOM and physical board markings identify the installed MCU board as an ESP32-C3 development board. (expected: The BOM and physical board identify an ESP32-C3 development board, specifically the configured esp32-c3-devkitm-1 target where the board model is recorded.) | firmware/README.md<br>firmware/configs/app_config.h<br>sdkconfig.defaults | app_start | — | implemented, untested |
| `HW-004` | The system power input shall be 5 V supplied via micro‑USB from a 12 V automotive adapter. | hardware / instrument | When powered through its micro-USB input from a 12 V automotive adapter, the device input supply measures 5 V and the device reaches its normal operating state. (expected: The measured device input is 5 V, and the device operates in its defined normal operating state.) | firmware/README.md<br>firmware/bsp/board.h<br>firmware/configs/app_config.h | app_start | — | implemented, untested |

Notes recorded with these traces:
- `HW-001` — Inspection evidence identifies target esp32c3 and board model ESP32-C3-DevKitM-1 in project configuration and documentation.
- `HW-004` — Documentation specifies regulated 5 V micro-USB input from an external 12 V-to-5 V automotive converter and prohibits direct 12 V connection; voltage remains unobserved.

## DHT22 Climate Sensing

gate: deferred · tasks: 3/3 done · recorded in: 29ce0a85-c3a6-412f-a563-5c13a90ef91d

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-002` | The firmware shall read temperature from the DHT22 every 2000 ms. | on-target test | The timestamp difference between consecutive successfully acquired temperature samples during normal operation is nominally 2000 ms, allowing only the scheduler jitter defined by the firmware test environment. (expected: Each consecutive temperature-sample interval is 2000 ms plus or minus the permitted scheduler jitter.) | firmware/configs/app_config.h<br>firmware/services/dht22_service.c<br>firmware/services/dht22_service.h | dht22_checksum_valid, dht22_service_start, dht22_try_read | — | implemented, untested |
| `FR-003` | The firmware shall read humidity from the DHT22 every 2000 ms. | on-target test | The timestamp difference between consecutive successfully acquired humidity samples during normal operation is nominally 2000 ms, allowing only the scheduler jitter defined by the firmware test environment. (expected: Each consecutive humidity-sample interval is 2000 ms plus or minus the permitted scheduler jitter.) | firmware/configs/app_config.h<br>firmware/services/dht22_service.c<br>firmware/services/dht22_service.h | dht22_checksum_valid, dht22_service_start, dht22_try_read | — | implemented, untested |
| `FR-016` | For each DHT22 sample, the firmware shall verify the sensor checksum; on checksum failure it shall retry once within the same 2000 ms cycle and, if still invalid, shall not update the published temperature/humidity for that cycle. | host test | When a DHT22 frame has an invalid checksum, the sampling logic performs exactly one retry before completing the sample cycle. (expected: Two checksum validation attempts occur: the initial attempt followed by exactly one retry within the same sampling cycle.)<br>When both the initial DHT22 frame and its single retry have invalid checksums, the published temperature and humidity remain equal to the last valid values for that cycle. (expected: The published temperature and humidity after the failed cycle are unchanged from the previously published values.)<br>When both DHT22 checksum validation attempts fail, the sampling logic records an error event for the failed sample. (expected: At least one error event identifying the failed DHT22 sample or checksum failure is recorded during that cycle.)<br>When both DHT22 checksum validation attempts fail, the DHT22 failure counter increases by one and the sampling cycle returns to its periodic loop without publishing new sensor values. (expected: The failure counter is initial value plus one, the published values are unchanged, and the sampling cycle completes so the periodic loop can proceed.) | firmware/configs/app_config.h<br>firmware/services/dht22_service.c<br>firmware/services/dht22_service.h | dht22_checksum_valid, dht22_service_start, dht22_try_read | — | implemented, untested |
| `IF-006` | The DHT22 data output shall be connected to ESP32-C3 GPIO2. | inspection | The inspected schematic or board wiring identifies the DHT22 data-output net as ESP32-C3 GPIO2. (expected: DHT22 OUT is electrically connected to ESP32-C3 GPIO2.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/platforms/esp32/dht22_port_esp32.c | dht22_port_open, dht22_read_frame | — | implemented, untested |
| `IF-007` | The DHT22 VCC pin shall be connected to 3.3 V. | inspection | The inspected schematic or board wiring identifies the DHT22 VCC pin as connected to the 3.3 V rail. (expected: DHT22 VCC is connected to the 3.3 V rail.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/platforms/esp32/dht22_port_esp32.c | dht22_port_open, dht22_read_frame | — | implemented, untested |
| `IF-008` | The DHT22 GND pin shall be connected to ground. | inspection | The inspected schematic or board wiring identifies the DHT22 GND pin as connected to system ground. (expected: DHT22 GND is connected to system ground.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/platforms/esp32/dht22_port_esp32.c | dht22_port_open, dht22_read_frame | — | implemented, untested |
| `HW-002` | The climate sensor shall be a DHT22 providing temperature and humidity. | inspection | The BOM and hardware inspection identify the installed climate sensor as a DHT22 with temperature and humidity outputs. (expected: The BOM and installed hardware identify a DHT22 sensor that provides both temperature and humidity measurements.) | firmware/README.md<br>firmware/configs/app_config.h<br>firmware/platforms/esp32/dht22_port_esp32.c | dht22_port_open, dht22_read_frame | — | implemented, untested |

Notes recorded with these traces:
- `FR-002`, `FR-003`, `FR-016` — Service performs periodic 2000 ms cycles, validates checksums, retries once, retains the previous valid sample after double failure, and increments a failure counter.
- `IF-006`, `IF-007`, `IF-008`, `HW-002` — GPIO2 is configured for the DHT22 data line with internal pull-up; the firmware documentation records DHT22 3.3 V and ground wiring assumptions. Physical sensor identity and wiring are not hardware-observed in this run.

## MPU6050 Motion Sensing & I2C Driver

gate: deferred · tasks: 4/4 done · recorded in: 29ce0a85-c3a6-412f-a563-5c13a90ef91d

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-004` | The firmware shall read X, Y, and Z acceleration from the MPU6050. | hardware / instrument | The published acceleration state contains finite numeric X, Y, and Z channel values when the MPU6050 is connected. (expected: All three acceleration fields contain finite numeric values.)<br>At least one published acceleration channel changes when the connected MPU6050 is moved through a detectable motion. (expected: At least one of the X, Y, or Z channel values differs between the stationary and motion intervals.) | firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.c<br>firmware/services/mpu6050_service.c | mpu6050_read_sample, mpu6050_service_start | — | implemented, untested |
| `FR-005` | The firmware shall convert the MPU6050 acceleration readings to G-force units for output. | not verifiable | The stationary upright output cannot be judged because the acceptance statement does not define a numeric tolerance for “near” 1 g or 0 g. | firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.c<br>firmware/services/mpu6050_service.c | mpu6050_read_sample, mpu6050_service_start | — | implemented, untested |
| `FR-006` | The firmware shall map IMU axes such that X corresponds to lateral (cornering), Y to longitudinal (acceleration/braking), and Z to vertical (bumps). | not verifiable | The axis-isolation mapping cannot be judged because the acceptance statement does not define how much the corresponding component must change or how small the non-corresponding components must remain. | firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.c<br>firmware/services/mpu6050_service.c | mpu6050_read_sample, mpu6050_service_start | — | implemented, untested |
| `FR-017` | At startup, the firmware shall verify MPU6050 presence via its identity register and initialize it to a defined measurement range and filtering configuration so acceleration‑to‑g conversion is deterministic. | hardware / instrument, not verifiable | Startup accepts the MPU6050 only when its WHO_AM_I register reads the MPU6050 identity value 0x68. (expected: WHO_AM_I reads 0x68 and the IMU enters the configured-ready state.)<br>Startup reads back each MPU6050 full-scale-range and filtering configuration register and obtains the same value that it wrote. (expected: Every required range and filtering register readback exactly matches the value written during initialization.)<br>When the identity check fails, the firmware does not publish an IMU sample for that initialization cycle and emits an IMU initialization error. (expected: No IMU data is published for the failed cycle, and the log contains an IMU initialization error.)<br>After an identity-check failure, the firmware makes another initialization attempt on a later retry period. (expected: At least one additional identity-register transaction occurs after the initial failed attempt.)<br>The stationary-upright conversion check cannot be judged because the acceptance statement does not define a numeric tolerance for “near” 1 g or 0 g. | firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.c<br>firmware/platforms/esp32/mpu6050_port_esp32.c<br>firmware/services/mpu6050_service.c | mpu6050_initialize, mpu6050_validate_configuration | — | implemented, untested |
| `IF-009` | The MPU6050 SCL pin shall be connected to ESP32-C3 GPIO8. | inspection | The hardware connectivity artifact shows the MPU6050 SCL net terminating at ESP32-C3 GPIO8. (expected: The SCL net is connected to ESP32-C3 GPIO8.) | firmware/configs/app_config.h<br>firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.h<br>firmware/platforms/esp32/mpu6050_port_esp32.c<br>firmware/services/mpu6050_service.c | mpu6050_initialize, mpu6050_port_start | — | implemented, untested |
| `IF-010` | The MPU6050 SDA pin shall be connected to ESP32-C3 GPIO9. | inspection | The hardware connectivity artifact shows the MPU6050 SDA net terminating at ESP32-C3 GPIO9. (expected: The SDA net is connected to ESP32-C3 GPIO9.) | firmware/configs/app_config.h<br>firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.h<br>firmware/platforms/esp32/mpu6050_port_esp32.c<br>firmware/services/mpu6050_service.c | mpu6050_initialize, mpu6050_port_start | — | implemented, untested |
| `IF-011` | The MPU6050 VCC pin shall be connected to 3.3 V. | inspection | The hardware connectivity artifact shows the MPU6050 VCC pin connected to the 3.3 V rail. (expected: VCC is connected to the 3.3 V rail.) | firmware/configs/app_config.h<br>firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.h<br>firmware/platforms/esp32/mpu6050_port_esp32.c<br>firmware/services/mpu6050_service.c | mpu6050_initialize, mpu6050_port_start | — | implemented, untested |
| `IF-012` | The MPU6050 GND pin shall be connected to ground. | inspection | The hardware connectivity artifact shows the MPU6050 GND pin connected to system ground. (expected: GND is connected to system ground.) | firmware/configs/app_config.h<br>firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.h<br>firmware/platforms/esp32/mpu6050_port_esp32.c<br>firmware/services/mpu6050_service.c | mpu6050_initialize, mpu6050_port_start | — | implemented, untested |
| `HW-003` | The inertial measurement unit shall be an MPU6050 3-axis accelerometer. | inspection | The BOM and hardware identification records list the inertial sensor as an MPU6050 3-axis accelerometer. (expected: The selected and fitted inertial sensor is identified as MPU6050, a 3-axis accelerometer.) | firmware/configs/app_config.h<br>firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.h<br>firmware/platforms/esp32/mpu6050_port_esp32.c<br>firmware/services/mpu6050_service.c | mpu6050_initialize, mpu6050_port_start | — | implemented, untested |

Observed on the board:
- `FR-004` — After WHO_AM_I=0x68 and configuration success, serial output repeatedly showed finite lateral, longitudinal, and vertical acceleration values. Motion-change criterion was not exercised because no controlled movement interval was captured.
- `FR-017` — Serial output showed IMU ready with WHO_AM_I=0x68, accel_range=2G, and dlpf=44Hz; no initialization errors appeared in the successful run. Retry-after-failure was not exercised.
- `IF-009`, `IF-010`, `IF-011`, `IF-012`, `HW-003` — The MPU6050 at address 0x68 acknowledged I2C, WHO_AM_I returned 0x68, and acceleration samples were read successfully. Code/config inspection records SCL GPIO8, SDA GPIO9, 3.3 V and ground assumptions, and the supplied MPU6050 driver identifies the sensor; physical rail and BOM inspection were not separately performed.

Notes recorded with these traces:
- `FR-004` — Hardware observation confirms finite published channels; movement sensitivity remains unverified.
- `FR-005`, `FR-006` — Conversion and X-to-lateral, Y-to-longitudinal, Z-to-vertical mapping are implemented. Frozen acceptance criteria provide no numeric tolerance for stationary or axis-isolation judgments, so those checks remain not verifiable.
- `FR-017` — The successful hardware run demonstrates identity acceptance and operational configuration readback path; the failed-run identity behavior was not reproduced.

## WiFi & MQTT Communication

gate: deferred · tasks: 5/5 done · recorded in: 29ce0a85-c3a6-412f-a563-5c13a90ef91d

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-001` | The system shall monitor interior cabin climate and vehicle ride dynamics using the ESP32-C3 and shall publish the data over MQTT. | hardware / instrument | A subscriber receives MQTT telemetry containing temperature, humidity, and three-axis acceleration values acquired from the installed sensors. (expected: Received MQTT messages contain numeric temperature, humidity, lateral acceleration, longitudinal acceleration, and vertical acceleration data.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_flush_queue, network_publish, network_queue_latest, network_task_loop | — | implemented, untested |
| `FR-007` | The firmware shall connect to WiFi automatically on startup. | hardware / instrument | After a power cycle, the device associates with the configured WiFi network without receiving provisioning input. (expected: The device is associated with the configured SSID without user interaction.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_event_handler, network_start_wifi | — | implemented, untested |
| `FR-008` | The firmware shall reconnect to WiFi if the connection drops. | hardware / instrument | After an induced WiFi disconnection and subsequent access-point recovery, the device re-associates without manual intervention. (expected: The device returns to an active WiFi association without a reboot or user action.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_event_handler, network_start_wifi | — | implemented, untested |
| `FR-009` | The firmware shall publish climate and motion data as a single JSON payload to MQTT every 2000 ms. | hardware / instrument | Each periodic MQTT telemetry payload contains both climate and motion fields and consecutive payloads are emitted at 2000 ms intervals within the allowed scheduling jitter. (expected: Every payload contains climate and motion data, and each inter-message interval is 2000 ms plus or minus the allowed scheduling jitter.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_flush_queue, network_publish, network_queue_latest, network_task_loop | — | implemented, untested |
| `FR-014` | The firmware shall persist the WiFi SSID and password in non‑volatile storage so the device can connect automatically on startup without user interaction. | hardware / instrument | Credentials configured once remain available after a power cycle and allow association to the same WiFi SSID without re-entry. (expected: After reboot, the device associates with the previously configured SSID without credential re-entry.)<br>When stored WiFi credentials are missing or corrupted, startup logs an error and enters a finite retry or configuration path rather than looping indefinitely. (expected: An error is logged and the device enters the configuration/retry flow; it does not remain in an unobservable infinite loop.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_event_handler, network_start_wifi | — | implemented, untested |
| `FR-015` | The firmware shall detect MQTT broker disconnection and automatically reconnect and resume periodic and alert publications without user intervention. | hardware / instrument | After the MQTT broker is stopped and then restored, the device reconnects without rebooting and resumes periodic telemetry and triggered alert publication. (expected: The device reconnects after broker recovery, telemetry resumes at the 2000 ms schedule, and a subsequently triggered alert is received.)<br>While repeated MQTT reconnect attempts fail, sensor acquisition continues and each connection failure is recorded according to the implemented backoff policy. (expected: Sensor acquisition continues, connection failures are logged, and reconnect attempts follow the configured backoff policy without stopping acquisition.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | mqtt_event_handler, network_load_endpoint, network_start_mqtt | — | implemented, untested |
| `IF-001` | The device shall use WiFi SSID "GopalaSutharsan" for network association. | hardware / instrument | The device configuration and runtime association both identify the WiFi network as SSID "GopalaSutharsan". (expected: The configured and associated SSID are exactly "GopalaSutharsan".) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_event_handler, network_start_wifi | — | implemented, untested |
| `IF-002` | The device shall use the WiFi password "123987456" to authenticate to the specified SSID. | hardware / instrument | The device successfully authenticates to SSID "GopalaSutharsan" using password "123987456". (expected: Authentication succeeds and the device reaches an associated WiFi state on "GopalaSutharsan".) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_event_handler, network_start_wifi | — | implemented, untested |
| `IF-003` | Each periodic telemetry JSON message shall include the keys: temp_c, humidity_percent, lateral_g, longitudinal_g, and vertical_g. | hardware / instrument | Every received periodic telemetry JSON object contains all five specified keys and each associated value is numeric. (expected: Each payload contains all five keys, and every listed value is numeric.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_flush_queue, network_publish, network_queue_latest, network_task_loop | — | implemented, untested |
| `IF-004` | Alert messages shall be published to the MQTT topic "vehicle/cabin/alert" on a threshold event. | hardware / instrument | A threshold-exceeding sensor event produces an MQTT message on topic "vehicle/cabin/alert". (expected: At least one alert message is received on exactly vehicle/cabin/alert after the threshold event.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_publish, network_task_loop | — | implemented, untested |
| `IF-005` | Each alert JSON message shall include temp_c, humidity_percent, lateral_g, longitudinal_g, vertical_g, and a field "trigger" indicating which metric triggered the alert. | hardware / instrument | For each induced metric threshold event, the alert JSON contains all five measurement fields and its trigger field identifies the exceeded metric. (expected: Every alert contains temp_c, humidity_percent, lateral_g, longitudinal_g, vertical_g, and trigger; trigger equals the metric whose threshold was exceeded.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_publish, network_task_loop | — | implemented, untested |
| `IF-013` | The firmware shall provide a configurable MQTT broker hostname/IP and TCP port used for publishing telemetry and alerts. | hardware / instrument | Changing the configured MQTT broker hostname/IP and TCP port takes effect without reflashing and directs both telemetry and alert messages to the new endpoint. (expected: The device connects to the configured test endpoint and publishes periodic telemetry and triggered alerts there, without a firmware reflash.)<br>When the configured MQTT endpoint is unreachable, retry activity and connection errors are logged while sensor and alert processing continues without a communication-induced block. (expected: Connection errors are logged, retries follow the configured backoff policy, and sensor acquisition and alert evaluation continue while MQTT is unavailable.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | mqtt_event_handler, network_load_endpoint, network_start_mqtt | — | implemented, untested |
| `IF-014` | Each periodic telemetry message shall be published on MQTT topic "vehicle/cabin/telemetry" at 2000 ms intervals. | hardware / instrument | Periodic telemetry messages are received on topic "vehicle/cabin/telemetry" with 2000 ms inter-arrival times within the allowed scheduling jitter. (expected: Every captured message is on vehicle/cabin/telemetry and each interval is 2000 ms plus or minus the allowed scheduling jitter.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_flush_queue, network_publish, network_queue_latest, network_task_loop | — | implemented, untested |
| `NFR-001` | MQTT publish operations shall use a finite timeout and shall not block the 2000 ms sensor sampling schedule; if a publish cannot complete within the timeout, the attempt shall be aborted and normal sampling shall continue. | hardware / instrument | When MQTT publishing is stalled, each publish attempt terminates at its finite timeout, sampling continues at 2000 ms cadence, and a timeout or abort event is logged. (expected: Each stalled publish is aborted at the finite timeout, a timeout or abort is logged, sensor samples remain 2000 ms apart within the permitted scheduling tolerance, and the device does not reset.)<br>After network recovery, the device publishes current periodic telemetry without first processing a backlog that delays normal sampling. (expected: Telemetry resumes on the normal schedule using current samples, with no accumulated backlog causing a prolonged publication or sampling delay.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_flush_queue, network_publish, network_queue_latest, network_task_loop | — | implemented, untested |
| `NFR-003` | WiFi connection and reconnection attempts shall use finite timeouts and a backoff strategy and shall not block sensor acquisition or alert evaluation. | hardware / instrument | With invalid WiFi credentials, association attempts have finite completion times and increasing retry intervals while sensor acquisition and alert evaluation continue without a watchdog reset. (expected: Each association attempt completes or times out, each successive retry interval is longer than the preceding interval, sensor acquisition and alert evaluation continue, and no watchdog reset occurs.)<br>When WiFi remains unavailable, the device continues offline operation and logs WiFi connection failures. (expected: Offline sensor acquisition and alert evaluation continue, and WiFi failures are logged.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_event_handler, network_start_wifi | — | implemented, untested |
| `NFR-004` | Any internal queueing of MQTT messages shall be bounded to prevent RAM exhaustion; when the bound is reached, the oldest unsent telemetry shall be discarded in favor of newer data. | hardware / instrument | During a prolonged broker outage, the MQTT queue remains bounded, the device remains responsive without an out-of-memory reset, and reconnection publishes current telemetry without flushing an unbounded backlog. (expected: Queue occupancy never exceeds its configured bound, the device remains responsive with no out-of-memory reset, and post-reconnection traffic contains current telemetry without an unbounded backlog flush.)<br>When the bounded MQTT queue reaches capacity, the oldest unsent telemetry is discarded in favor of newer telemetry and an overflow event is logged. (expected: An overflow event is logged; the retained set contains newer telemetry rather than the oldest entries, and the queue never contains more than its configured capacity.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/network_service_esp32.c | network_flush_queue, network_publish, network_queue_latest, network_task_loop | — | implemented, untested |

Observed on the board:
- `FR-001`, `FR-009`, `IF-003`, `IF-014`, `NFR-001`, `NFR-004` — The device remained running and MPU6050 acquisition continued while Wi-Fi associated. No MQTT telemetry payload was captured in the available hardware monitor evidence; telemetry receipt and 2000 ms broker timing remain unverified.
- `FR-007`, `FR-008`, `FR-014`, `IF-001`, `IF-002`, `NFR-003` — The device associated with GopalaSutharsan and obtained IP address 172.30.6.8 without user interaction. Wi-Fi drop/recovery, power-cycle persistence, and invalid-credential retry behavior were not separately exercised.
- `FR-015`, `IF-013` — The firmware loaded broker.hivemq.com:1883 and requested MQTT startup, but no MQTT connected event or publication was observed in the available monitor capture. Broker recovery was not verified.

Notes recorded with these traces:
- `FR-001`, `FR-009`, `IF-003`, `IF-014`, `NFR-001`, `NFR-004` — Current source includes threshold integration but preserves the five-key telemetry format, periodic scheduling, finite timeout, and bounded queue path.
- `FR-007`, `FR-008`, `FR-014`, `IF-001`, `IF-002`, `NFR-003` — The configured password is not emitted in logs.
- `FR-015`, `IF-013` — Earlier transport failures were observed; successful broker delivery remains unverified.
- `IF-004`, `IF-005` — Alert topic and JSON publication paths are present, but no threshold event or alert message was observed on hardware.

## Alert Threshold Configuration & Evaluation

gate: deferred · tasks: 4/4 done · recorded in: 29ce0a85-c3a6-412f-a563-5c13a90ef91d

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `FR-010` | The firmware shall maintain a configurable alert threshold for temperature. | host test | After the runtime configuration method sets the temperature threshold to a value different from its previous value, the stored threshold equals the new value and temperature evaluation uses that value for alert decisions. (expected: The configured threshold equals V_new, and the evaluation result matches comparison with V_new rather than V_old.) | firmware/configs/app_config.h<br>firmware/modules/threshold_evaluator.c<br>firmware/platforms/esp32/threshold_service_esp32.c<br>firmware/services/threshold_service.h | threshold_service_active, threshold_service_trigger, threshold_service_update | — | implemented, untested |
| `FR-011` | The firmware shall maintain a configurable alert threshold for humidity. | host test | After the runtime configuration method sets the humidity threshold to a value different from its previous value, the stored threshold equals the new value and humidity evaluation uses that value for alert decisions. (expected: The configured threshold equals V_new, and the evaluation result matches comparison with V_new rather than V_old.) | firmware/configs/app_config.h<br>firmware/modules/threshold_evaluator.c<br>firmware/platforms/esp32/threshold_service_esp32.c<br>firmware/services/threshold_service.h | threshold_service_active, threshold_service_trigger, threshold_service_update | — | implemented, untested |
| `FR-012` | The firmware shall maintain a configurable alert threshold for G-force. | host test | After the runtime configuration method sets the G-force threshold to a different value in g, the stored threshold equals the new value and G-force evaluation uses that value for alert decisions. (expected: The configured threshold equals V_new g, and the evaluation result matches comparison with V_new rather than V_old.) | firmware/configs/app_config.h<br>firmware/modules/threshold_evaluator.c<br>firmware/platforms/esp32/threshold_service_esp32.c<br>firmware/services/threshold_service.h | threshold_service_active, threshold_service_trigger, threshold_service_update | — | implemented, untested |
| `FR-013` | The firmware shall publish immediately to the alert topic when any configured threshold is exceeded, independent of the regular 2000 ms cycle. | hardware / instrument | When a live sensor value first exceeds a configured threshold, an alert message is published on the alert MQTT topic before the next scheduled 2000 ms periodic publication. (expected: An alert message is emitted asynchronously on the alert topic, and its publication timestamp is earlier than the next periodic publication deadline, which is 2000 ms after the preceding regular publication.) | firmware/platforms/esp32/network_service_esp32.c<br>firmware/platforms/esp32/threshold_service_esp32.c | network_publish, network_task_loop, threshold_service_trigger | — | implemented, untested |
| `NFR-002` | Updates to configurable alert thresholds shall be stored atomically in non‑volatile memory and shall persist across power cycles; after reset the most recently committed values shall be in effect. | hardware / instrument, on-target test, host test | If power is interrupted during a threshold-record write, the thresholds recovered after reboot equal either the complete previously committed record or the complete newly committed record, never a partially written or invalid combination. (expected: For every interruption point, the recovered record is exactly the old complete record or exactly the new complete record; no field is corrupted, missing, or mixed between records.)<br>After a threshold update is successfully committed, the updated threshold values are restored after reset and remain unchanged through a subsequent power cycle. (expected: The values after reset and after the power cycle exactly equal the most recently committed values for every configured threshold.)<br>When the non-volatile storage write reports a persistence failure, the configuration logic retains the last known-good threshold values and emits an error log. (expected: All active thresholds remain equal to the last known-good values, and the log contains an error entry identifying the persistence failure.) | firmware/platforms/esp32/threshold_service_esp32.c | threshold_service_load, threshold_service_update | — | implemented, untested |

Notes recorded with these traces:
- `FR-010`, `FR-011`, `FR-012` — Validated temperature, humidity, and G-force thresholds are represented, loaded, updated, and used by the evaluator. Host tests have not been run.
- `FR-013` — Threshold crossing path publishes the alert topic independently of the periodic telemetry publication path; alert publication was not observed on hardware.
- `NFR-002` — Threshold records are written to NVS and active state is changed only after successful commit; power-interruption and reset persistence have not been hardware-verified.

## Diagnostics, Security & Fault Recovery

gate: deferred · tasks: 4/4 done · recorded in: 29ce0a85-c3a6-412f-a563-5c13a90ef91d

| Req | Statement | Verified by | Acceptance criteria | Files | Symbols | Tests | Status |
|---|---|---|---|---|---|---|---|
| `SF-001` | The firmware shall enable a watchdog mechanism and, upon watchdog expiration or brown‑out reset, shall automatically reboot and resume WiFi association and MQTT telemetry without user action. | hardware / instrument, not verifiable | After a deliberate firmware hang causes watchdog expiration, the device reboots without user input and resumes periodic MQTT telemetry and alert publishing. (expected: The device reboots automatically, re-associates with WiFi, and resumes both periodic telemetry and alert publishing without user input.)<br>After a deliberate brown-out reset, the device reboots without user input and resumes periodic MQTT telemetry and alert publishing. (expected: The device reboots automatically, re-associates with WiFi, and resumes both periodic telemetry and alert publishing without user input.)<br>When resets repeat, the firmware limits the reboot rate and records a persistent fault indication. | firmware/app/app.c<br>firmware/platforms/esp32/diagnostics_service_esp32.c | app_start, diagnostics_task | — | implemented, untested |
| `DIAG-001` | On boot, the firmware shall output a single‑line status over the serial console including: firmware version, configured SSID, MQTT broker endpoint, and sensor initialization results; credentials shall be redacted. | inspection, on-target test | The boot status is emitted as exactly one serial-console line containing the firmware version, configured SSID without its password, MQTT broker host and port, and pass/fail results for DHT22 and MPU6050 initialization. (expected: Exactly one status line contains the firmware version, SSID with no password, broker host:port, and separate DHT22 and MPU6050 initialization pass/fail results.)<br>Loss of the serial console does not prevent the firmware from continuing its normal operation. (expected: The device continues normal operation without waiting indefinitely for or failing because of the unavailable console.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/diagnostics_service_esp32.c | diagnostics_task | — | implemented, untested |
| `DIAG-002` | The firmware shall maintain counters of sensor read failures per sensor and MQTT publish failures and shall make them available via the serial console on request. | hardware / instrument | The DHT22 failure counter increases for each induced DHT22 checksum failure reported through the serial-console query. (expected: The final DHT22 failure counter equals the initial value plus N.)<br>The IMU failure counter increases for each induced IMU read failure after the IMU is disconnected. (expected: The final IMU failure counter equals the initial value plus the number of induced failed read attempts.)<br>The MQTT publish-failure counter increases for each induced failed publish and is returned by the serial-console query. (expected: The final MQTT publish-failure counter equals the initial value plus N, and the query response includes that counter.)<br>A serial-console counter query returns the current sensor and MQTT failure counters while the device continues operating. (expected: The query returns the counters and the device remains operational without requiring a stop or restart.) | firmware/platforms/esp32/diagnostics_service_esp32.c<br>firmware/platforms/esp32/network_service_esp32.c<br>firmware/services/dht22_service.c<br>firmware/services/mpu6050_service.c | dht22_service_sample, diagnostics_task, mpu6050_service_state, network_service_publish_failures | — | implemented, untested |
| `SEC-001` | The firmware shall not include WiFi or MQTT credentials in telemetry payloads or console logs and shall redact them in any status output. | inspection | Captured MQTT telemetry and error payloads contain no configured WiFi or MQTT credentials. (expected: No WiFi password, MQTT username, MQTT password, or equivalent configured credential value appears in any captured MQTT payload.)<br>Serial boot, status, and error output contains no configured WiFi or MQTT credentials and replaces any sensitive status field with a redaction marker. (expected: No configured WiFi or MQTT credential value appears in console output; sensitive fields are represented by redaction markers rather than secret values.)<br>An attempted credential print is replaced by redacted output and produces a security warning. (expected: The credential value is absent, the sensitive field contains a redaction marker, and a security warning is logged.) | firmware/configs/app_config.h<br>firmware/platforms/esp32/diagnostics_service_esp32.c<br>firmware/platforms/esp32/network_service_esp32.c | diagnostics_task, mqtt_event_handler, network_task_loop | — | implemented, untested |

Observed on the board:
- `SF-001` — The latest flashed firmware booted without a watchdog or brown-out reset, started diagnostics, associated Wi-Fi, initialized the MPU6050, and continued sensor acquisition. Deliberate watchdog hang and brown-out injection were not performed; automatic post-reset MQTT/alert resumption remains unverified.
- `DIAG-001` — Exactly one STATUS line was captured: version=1.0, SSID GopalaSutharsan, broker.hivemq.com:1883, DHT22 and MPU6050 runtime status fields, and reset=normal. The Wi-Fi password was absent.
- `DIAG-002` — The firmware remained operational while sensor acquisition and network startup ran. A serial health query and induced sensor/MQTT failures were not performed during the capture, so counter increments and query response remain unverified.
- `SEC-001` — Captured status and runtime logs contained the configured SSID and broker endpoint but not the Wi-Fi password or MQTT credentials. No credential appeared in the observed telemetry path.

Notes recorded with these traces:
- `SF-001` — Watchdog task registration and reset-reason reporting are implemented. The frozen repeated-reset-rate criterion is not verifiable because it defines no limit or persistent-fault observable.
- `DIAG-001` — Sensor statuses are currently reported as runtime rather than final pass/fail values.
- `DIAG-002` — The health output path now includes DHT22, IMU, and MQTT publish-failure counters.
- `SEC-001` — The inspection criterion requiring a redaction marker for sensitive fields is only partially addressed because the status intentionally reports the non-secret SSID and endpoint and does not print a password.

## Gaps

Listed rather than omitted. A matrix showing only what was built cannot be told apart from one showing full coverage.

### Host tests you could write (5)

These have a `host` acceptance criterion and cite no test, so they are the ones a unit test can actually cover -- and the only ones; see the next section. They do NOT run on the machine that generated this firmware, which carries the vendor cross-compilation toolchain and no host compiler. Run `test_handover export` and `sources/test_handover/` will hold this list machine-readably, with the criteria to assert, where each requirement's code lives, and which firmware layers compile standalone versus needing a mocked header.

- `FR-016` (DHT22 Climate Sensing)
- `FR-010` (Alert Threshold Configuration & Evaluation)
- `FR-011` (Alert Threshold Configuration & Evaluation)
- `FR-012` (Alert Threshold Configuration & Evaluation)
- `NFR-002` (Alert Threshold Configuration & Evaluation)

### NOT unit-testable (38) — do not write host tests for these

No `host` criterion, so no unit test can cover them however it is written. Listed explicitly because the untested count above otherwise reads as a backlog of unit tests owed -- and closing one of these with a host test means inventing an assertion that passes without checking the thing. Each line says how it IS checked instead.

- `HW-001` — checked by inspection · _ESP32-C3 Platform & Power_
- `HW-004` — checked by hardware / instrument · _ESP32-C3 Platform & Power_
- `FR-002` — checked by on-target test · _DHT22 Climate Sensing_
- `FR-003` — checked by on-target test · _DHT22 Climate Sensing_
- `IF-006` — checked by inspection · _DHT22 Climate Sensing_
- `IF-007` — checked by inspection · _DHT22 Climate Sensing_
- `IF-008` — checked by inspection · _DHT22 Climate Sensing_
- `HW-002` — checked by inspection · _DHT22 Climate Sensing_
- `FR-004` — checked by hardware / instrument · _MPU6050 Motion Sensing & I2C Driver_
- `FR-005` — checked by not verifiable · _MPU6050 Motion Sensing & I2C Driver_
- `FR-006` — checked by not verifiable · _MPU6050 Motion Sensing & I2C Driver_
- `FR-017` — checked by hardware / instrument, not verifiable · _MPU6050 Motion Sensing & I2C Driver_
- `IF-009` — checked by inspection · _MPU6050 Motion Sensing & I2C Driver_
- `IF-010` — checked by inspection · _MPU6050 Motion Sensing & I2C Driver_
- `IF-011` — checked by inspection · _MPU6050 Motion Sensing & I2C Driver_
- `IF-012` — checked by inspection · _MPU6050 Motion Sensing & I2C Driver_
- `HW-003` — checked by inspection · _MPU6050 Motion Sensing & I2C Driver_
- `FR-001` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `FR-007` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `FR-008` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `FR-009` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `FR-014` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `FR-015` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-001` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-002` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-003` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-004` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-005` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-013` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `IF-014` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `NFR-001` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `NFR-003` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `NFR-004` — checked by hardware / instrument · _WiFi & MQTT Communication_
- `FR-013` — checked by hardware / instrument · _Alert Threshold Configuration & Evaluation_
- `SF-001` — checked by hardware / instrument, not verifiable · _Diagnostics, Security & Fault Recovery_
- `DIAG-001` — checked by inspection, on-target test · _Diagnostics, Security & Fault Recovery_
- `DIAG-002` — checked by hardware / instrument · _Diagnostics, Security & Fault Recovery_
- `SEC-001` — checked by inspection · _Diagnostics, Security & Fault Recovery_

### Board checks owed (7)

The counterpart of the host-test worklist, for whoever has the hardware. Each of these has a `hardware` or `target` criterion and nothing recorded as observed, so no host test and no reading of the code will ever close it. Per requirement rather than per category on purpose: a category usually mixes these with criteria that are settled by reading, and "this category needs hardware" sends someone to re-check things that do not.

- `HW-004` (ESP32-C3 Platform & Power)
- `FR-002` (DHT22 Climate Sensing)
- `FR-003` (DHT22 Climate Sensing)
- `IF-004` (WiFi & MQTT Communication)
- `IF-005` (WiFi & MQTT Communication)
- `FR-013` (Alert Threshold Configuration & Evaluation)
- `NFR-002` (Alert Threshold Configuration & Evaluation)

### Implemented, never tested (43)

Code exists and no test is recorded against it.

- `HW-001` (ESP32-C3 Platform & Power)
- `HW-004` (ESP32-C3 Platform & Power)
- `FR-002` (DHT22 Climate Sensing)
- `FR-003` (DHT22 Climate Sensing)
- `FR-016` (DHT22 Climate Sensing)
- `IF-006` (DHT22 Climate Sensing)
- `IF-007` (DHT22 Climate Sensing)
- `IF-008` (DHT22 Climate Sensing)
- `HW-002` (DHT22 Climate Sensing)
- `FR-004` (MPU6050 Motion Sensing & I2C Driver)
- `FR-005` (MPU6050 Motion Sensing & I2C Driver)
- `FR-006` (MPU6050 Motion Sensing & I2C Driver)
- `FR-017` (MPU6050 Motion Sensing & I2C Driver)
- `IF-009` (MPU6050 Motion Sensing & I2C Driver)
- `IF-010` (MPU6050 Motion Sensing & I2C Driver)
- `IF-011` (MPU6050 Motion Sensing & I2C Driver)
- `IF-012` (MPU6050 Motion Sensing & I2C Driver)
- `HW-003` (MPU6050 Motion Sensing & I2C Driver)
- `FR-001` (WiFi & MQTT Communication)
- `FR-007` (WiFi & MQTT Communication)
- `FR-008` (WiFi & MQTT Communication)
- `FR-009` (WiFi & MQTT Communication)
- `FR-014` (WiFi & MQTT Communication)
- `FR-015` (WiFi & MQTT Communication)
- `IF-001` (WiFi & MQTT Communication)
- `IF-002` (WiFi & MQTT Communication)
- `IF-003` (WiFi & MQTT Communication)
- `IF-004` (WiFi & MQTT Communication)
- `IF-005` (WiFi & MQTT Communication)
- `IF-013` (WiFi & MQTT Communication)
- `IF-014` (WiFi & MQTT Communication)
- `NFR-001` (WiFi & MQTT Communication)
- `NFR-003` (WiFi & MQTT Communication)
- `NFR-004` (WiFi & MQTT Communication)
- `FR-010` (Alert Threshold Configuration & Evaluation)
- `FR-011` (Alert Threshold Configuration & Evaluation)
- `FR-012` (Alert Threshold Configuration & Evaluation)
- `FR-013` (Alert Threshold Configuration & Evaluation)
- `NFR-002` (Alert Threshold Configuration & Evaluation)
- `SF-001` (Diagnostics, Security & Fault Recovery)
- `DIAG-001` (Diagnostics, Security & Fault Recovery)
- `DIAG-002` (Diagnostics, Security & Fault Recovery)
- `SEC-001` (Diagnostics, Security & Fault Recovery)

### No observable exists (4)

These need a human decision — either the requirement is restated as something checkable, or it is accepted as unverifiable on the record. Inventing a test for one of these produces a test that passes and proves nothing.

- `FR-005` — The stationary upright output cannot be judged because the acceptance statement does not define a numeric tolerance for “near” 1 g or 0 g. · _A numeric pass/fail tolerance is missing for vertical_g near 1 g and lateral_g/longitudinal_g near 0 g._
- `FR-006` — The axis-isolation mapping cannot be judged because the acceptance statement does not define how much the corresponding component must change or how small the non-corresponding components must remain. · _Numeric or otherwise observable isolation thresholds are missing for the three mapped output components._
- `FR-017` — The stationary-upright conversion check cannot be judged because the acceptance statement does not define a numeric tolerance for “near” 1 g or 0 g. · _A numeric pass/fail tolerance is missing for vertical_g near 1 g and the other axes near 0 g._
- `SF-001` — When resets repeat, the firmware limits the reboot rate and records a persistent fault indication. · _No numeric reboot-rate limit, time window, or defined persistent fault indication is specified._
