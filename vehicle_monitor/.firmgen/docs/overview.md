<!-- FirmGen source overview v1 -->
# vehicle monitor

Source-based overview for the generated esp32 firmware. This page records the current source structure and statically identified interfaces. Building the firmware or generating this report does not establish runtime correctness.

Target: `esp32c3`.

Application entry source: `main/entry.c`.

## Modules and runtime entry points

- `app` — app (application, 2 source/header files).
- `bsp` — bsp (board, 2 source/header files).
- `components/firmgen_telemetry` — components/firmgen_telemetry (module, 2 source/header files).
- `components/firmgen_telemetry/include` — components/firmgen_telemetry/include (module, 2 source/header files).
- `configs` — configs (configuration, 4 source/header files).
- `drivers/mpu6050/mpu_6000_mpu_6050` — drivers/mpu6050/mpu_6000_mpu_6050 (driver, 2 source/header files).
- `interfaces` — interfaces (module, 3 source/header files).
- `main` — main (entrypoint, 1 source/header files).
- `modules` — modules (module, 1 source/header files).
- `osal` — osal (abstraction, 1 source/header files).
- `platforms/esp32` — platforms/esp32 (platform, 6 source/header files).
- `services` — services (module, 6 source/header files).
- `utils/helpers` — utils/helpers (utility, 2 source/header files).
- `utils/logger` — utils/logger (utility, 2 source/header files).
- Runtime entry `monitor_task` — fg_proto in `components/firmgen_telemetry/firmgen_proto_monitor.c:588`.
- Runtime entry `telemetry_task` — fg_telemetry in `components/firmgen_telemetry/firmgen_telemetry.c:411`.
- Runtime entry `dht22_task` — dht22_task in `firmware/app/app.c:29`.
- Runtime entry `mpu6050_task` — mpu6050_task in `firmware/app/app.c:30`.
- Runtime entry `network_service_task` — network_task in `firmware/app/app.c:31`.
- Runtime entry `diagnostics_task` — diagnostics_task in `firmware/platforms/esp32/diagnostics_service_esp32.c:73`.

## Pins and interfaces found in source

- `GPIO2` — DHT22 DATA GPIO (GPIO 2, APP_DHT22_DATA_GPIO) (gpio_input) at `firmware/configs/app_config.h:11`.
- `GPIO8` — MPU6050 SCL GPIO (GPIO 8, APP_MPU6050_SCL_GPIO) (gpio_input) at `firmware/configs/app_config.h:15`.
- `GPIO9` — MPU6050 SDA GPIO (GPIO 9, APP_MPU6050_SDA_GPIO) (gpio_input) at `firmware/configs/app_config.h:16`.
- wifi_manager — `firmware/configs/app_config.h`.
- mqtt_client — `components/firmgen_telemetry/firmgen_proto_monitor.c`.
- i2c_device — `components/firmgen_telemetry/firmgen_proto_monitor.c`.
- uart_device — `components/firmgen_telemetry/firmgen_proto_monitor.c`.
- i2c_device — `firmware/configs/app_config.h`.
- storage_manager — `firmware/platforms/esp32/network_service_esp32.c`.
- event_handler — `firmware/platforms/esp32/network_service_esp32.c`.

This is a static inventory, not verified wiring. Check the Firmware Topology panel and board wiring for unresolved connections.

## Build, flash, and documentation

Open this project in FirmGen and select its configured board/toolchain. Use **Build** to compile, **Flash** to program the connected board, and **Monitor** to inspect UART output. Debug/Production telemetry switching rebuilds and reflashes the firmware.

The API report is in `api/html/index.html` beside this overview. Review application behavior on hardware separately. Expand this overview with intended behavior, operating instructions, and validation evidence as these become available.

## Analyzed source inventory

- `CMakeLists.txt`
- `components/firmgen_telemetry/firmgen_proto_monitor.c`
- `components/firmgen_telemetry/firmgen_telemetry.c`
- `components/firmgen_telemetry/include/firmgen_proto_monitor.h`
- `components/firmgen_telemetry/include/firmgen_telemetry.h`
- `firmware/app/app.c`
- `firmware/app/app.h`
- `firmware/bsp/board.c`
- `firmware/bsp/board.h`
- `firmware/configs/app_config.h`
- `firmware/configs/build_config.h`
- `firmware/configs/feature_config.h`
- `firmware/configs/platform_config.h`
- `firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.c`
- `firmware/drivers/mpu6050/mpu_6000_mpu_6050/mpu6050.h`
- `firmware/interfaces/dht22_port.h`
- `firmware/interfaces/mpu6050_port.h`
- `firmware/interfaces/network_service.h`
- `firmware/modules/threshold_evaluator.c`
- `firmware/osal/osal.h`
- `firmware/platforms/esp32/dht22_port_esp32.c`
- `firmware/platforms/esp32/diagnostics_service_esp32.c`
- `firmware/platforms/esp32/mpu6050_port_esp32.c`
- `firmware/platforms/esp32/network_service_esp32.c`
- `firmware/platforms/esp32/osal_freertos.c`
- `firmware/platforms/esp32/threshold_service_esp32.c`
- `firmware/services/dht22_service.c`
- `firmware/services/dht22_service.h`
- `firmware/services/diagnostics_service.h`
- `firmware/services/mpu6050_service.c`
- `firmware/services/mpu6050_service.h`
- `firmware/services/threshold_service.h`
- `firmware/utils/helpers/helpers.c`
- `firmware/utils/helpers/helpers.h`
- `firmware/utils/logger/logger.c`
- `firmware/utils/logger/logger.h`
- `main/entry.c`
- `sdkconfig`
- `sdkconfig.defaults`
