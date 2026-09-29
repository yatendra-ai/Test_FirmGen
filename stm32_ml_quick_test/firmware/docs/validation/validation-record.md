# Validation Record — NUCLEO-C5A3ZG ML Quick Test

- **Board:** NUCLEO-C5A3ZG / STM32C5A3ZGT6.
- **Build:** Clean rebuild using `debug_GCC_NUCLEO-C5A3ZG` completed and linked successfully with the ST Edge AI CPU model integration.
- **Flash:** ST-LINK deployment succeeded.
- **Physical inference:** After reset, COM5 at 115200 reported model hash, runtime ready, all five reference-vector comparisons PASS in fixed order, and `ML_SUMMARY passed=5 total=5 status=PASS`.
- **Expected milli outputs observed:** positive `[500,4000]`; first_clamped `[0,4000]`; second_clamped `[7500,0]`; zero_input `[500,0]`; both_clamped `[0,0]`.
- **Repeatability:** Two resets/deploys reported the same five outcomes and summary. Serial monitor had buffered bytes before the current model banner; only the fresh banner-delimited segment was attributed to each current build.
- **Model identity:** SHA-256 `15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090`.
- **Memory:** `mcu_budget` final image: ROM 29,600/1,048,576 bytes; RAM 2,336/262,144 bytes; linker reserves 1,024 bytes stack and 512 bytes heap. Model activation storage is 16 bytes and embedded weights are 24 bytes. Runtime stack high-water instrumentation was not available.
- **Current-run capture:** `category5-serial-capture.log`.
