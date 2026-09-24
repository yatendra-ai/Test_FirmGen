# Host unit tests for this firmware

You are being asked to run unit tests that cannot run on the machine that generated this firmware -- that box carries the ESP-IDF cross-compilation toolchain and no host C compiler. Nothing here needs ESP-IDF.

## What to copy

The whole `firmware/` tree. Not a subset: a portable module includes headers from `interfaces/` and `configs/`, and those are not compiled but must be on the include path.

## What compiles, and what needs a mock

```
WHAT A HOST TEST COULD REACH IN THIS TREE (a file scan, no compiler needed):
  modules/ -- 1 source file(s), host-compilable
  services/ -- NEEDS A MOCK: mpu6050_service.c includes logger.h, which pulls the vendor SDK. Compilable on the test host once that header is mocked (CMock generates one from it); not compilable as it stands.
  utils/ -- NEEDS A MOCK: logger.c includes logger.h, which pulls the vendor SDK. Compilable on the test host once that header is mocked (CMock generates one from it); not compilable as it stands.
  app/ -- NEEDS A MOCK: app.c includes logger.h, which pulls the vendor SDK. Compilable on the test host once that header is mocked (CMock generates one from it); not compilable as it stands.
  A logging header is the usual culprit, and it taints every layer that includes it -- one include cost three of four layers on a real project. Mocking it on the test host works; splitting it behind an interface (a `*_if.h` with the platform implementation under `platforms/`) is better, because then nobody has to maintain the mock.
```

`platforms/` and `bsp/` are the vendor SDK's home and are not host-testable at all. Anything in them is checked on the board, not here.

## Suggested harness

Ceedling, with CMock for the headers listed above and for the `*_io_t` callback structs the drivers take. The firmware is written for GCC; MSVC rejects extensions it uses, so prefer gcc or clang.

## The worklist (5 requirements)

`worklist.json` has the machine-readable version, including which files and symbols carry each requirement. Write ONE test per criterion.

### `FR-016` — DHT22 Climate Sensing

For each DHT22 sample, the firmware shall verify the sensor checksum; on checksum failure it shall retry once within the same 2000 ms cycle and, if still invalid, shall not update the published temperature/humidity for that cycle.

- When a DHT22 frame has an invalid checksum, the sampling logic performs exactly one retry before completing the sample cycle. (expected: Two checksum validation attempts occur: the initial attempt followed by exactly one retry within the same sampling cycle.)
- When both the initial DHT22 frame and its single retry have invalid checksums, the published temperature and humidity remain equal to the last valid values for that cycle. (expected: The published temperature and humidity after the failed cycle are unchanged from the previously published values.)
- When both DHT22 checksum validation attempts fail, the sampling logic records an error event for the failed sample. (expected: At least one error event identifying the failed DHT22 sample or checksum failure is recorded during that cycle.)
- When both DHT22 checksum validation attempts fail, the DHT22 failure counter increases by one and the sampling cycle returns to its periodic loop without publishing new sensor values. (expected: The failure counter is initial value plus one, the published values are unchanged, and the sampling cycle completes so the periodic loop can proceed.)

Code: `firmware/services/dht22_service.c`, `firmware/services/dht22_service.h`, `firmware/configs/app_config.h`
Symbols: `dht22_service_start`, `dht22_checksum_valid`, `dht22_try_read`

### `FR-010` — Alert Threshold Configuration & Evaluation

The firmware shall maintain a configurable alert threshold for temperature.

- After the runtime configuration method sets the temperature threshold to a value different from its previous value, the stored threshold equals the new value and temperature evaluation uses that value for alert decisions. (expected: The configured threshold equals V_new, and the evaluation result matches comparison with V_new rather than V_old.)

Code: `firmware/services/threshold_service.h`, `firmware/platforms/esp32/threshold_service_esp32.c`, `firmware/modules/threshold_evaluator.c`, `firmware/configs/app_config.h`
Symbols: `threshold_service_update`, `threshold_service_active`, `threshold_service_trigger`

### `FR-011` — Alert Threshold Configuration & Evaluation

The firmware shall maintain a configurable alert threshold for humidity.

- After the runtime configuration method sets the humidity threshold to a value different from its previous value, the stored threshold equals the new value and humidity evaluation uses that value for alert decisions. (expected: The configured threshold equals V_new, and the evaluation result matches comparison with V_new rather than V_old.)

Code: `firmware/services/threshold_service.h`, `firmware/platforms/esp32/threshold_service_esp32.c`, `firmware/modules/threshold_evaluator.c`, `firmware/configs/app_config.h`
Symbols: `threshold_service_update`, `threshold_service_active`, `threshold_service_trigger`

### `FR-012` — Alert Threshold Configuration & Evaluation

The firmware shall maintain a configurable alert threshold for G-force.

- After the runtime configuration method sets the G-force threshold to a different value in g, the stored threshold equals the new value and G-force evaluation uses that value for alert decisions. (expected: The configured threshold equals V_new g, and the evaluation result matches comparison with V_new rather than V_old.)

Code: `firmware/services/threshold_service.h`, `firmware/platforms/esp32/threshold_service_esp32.c`, `firmware/modules/threshold_evaluator.c`, `firmware/configs/app_config.h`
Symbols: `threshold_service_update`, `threshold_service_active`, `threshold_service_trigger`

### `NFR-002` — Alert Threshold Configuration & Evaluation

Updates to configurable alert thresholds shall be stored atomically in non‑volatile memory and shall persist across power cycles; after reset the most recently committed values shall be in effect.

- When the non-volatile storage write reports a persistence failure, the configuration logic retains the last known-good threshold values and emits an error log. (expected: All active thresholds remain equal to the last known-good values, and the log contains an error entry identifying the persistence failure.)

Code: `firmware/platforms/esp32/threshold_service_esp32.c`
Symbols: `threshold_service_load`, `threshold_service_update`

## What to send back

A JSON file shaped like `results.example.json`. Four things in it are load-bearing:

- `host` and `harness` — evidence with no provenance is refused on import. Results from a stale tree look identical to fresh ones without them.
- `at` — when the tests ran, so a wide gap from the import time is visible.
- `sources_compiled` — how many FIRMWARE source files the test binaries compiled. If this is 0, the tests were the harness plus your own test files and asserted their own literals; that has happened here and was reported as coverage for five categories running. Report it honestly and it is recorded as the verdict it is.
- `tests[].name` — the exact test name. A claim of verification has to name a test from this list, so a name that does not match is rejected rather than assumed.

A test that could not be written is more useful reported than omitted: add it with `"outcome": "blocked"` and a `message` saying why. An absent test and a failing one are different facts, and only one of them is a problem with the firmware.
