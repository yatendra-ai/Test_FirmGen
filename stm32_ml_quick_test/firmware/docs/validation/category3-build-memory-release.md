# Build, Memory, and Release Evidence

This record applies to the currently integrated console/startup firmware, before ONNX model and ST AI runtime integration.

## Target and ABI configuration

- Board build preset: `debug_GCC_NUCLEO-C5A3ZG`.
- MCU: `STM32C5A3ZGT6`, Cortex-M33, single-precision FPU (`SP_FPU`), DSP enabled.
- Clock limit configured: 144,000,000 Hz.
- `GCC.14.3.1.cmake` derives Cortex-M33 SP-FPU + DSP compiler flags: `-mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard`.
- The linked generated board and application objects therefore use the project's selected target ABI. No generated model/runtime artifacts have been imported yet, so ABI compatibility with them is pending.

## Linker regions and measured current build

The linker script declares ROM at `0x08000000`, length `0x100000` (1 MiB), and RAM at `0x20000000`, length `0x40000` (256 KiB). It reserves 512 bytes of heap and 1024 bytes of stack.

`mcu_budget` for the successful current build reported:

| Region | Capacity | Used | Free |
| --- | ---: | ---: | ---: |
| ROM | 1,048,576 B | 18,960 B | 1,029,616 B |
| RAM | 262,144 B | 2,144 B | 260,000 B |

This fit applies only to the current firmware without generated model weights, ST AI runtime, or activation/context buffers. Those later components can significantly change the budget and must be measured again after integration.

## Heap and runtime storage

The current application/service/platform sources do not contain calls to `malloc`, `calloc`, `realloc`, or `free`. The linker does reserve a 512-byte heap region; its existence is not used as evidence that the test harness allocates. Model/runtime allocation behavior and their buffers are not yet available for inspection.

## Build and deliverables

- `mcu_build` completed and linked `template_start_from_nucleo.elf` for NUCLEO-C5A3ZG.
- The build produced a linker map in the target build output. Use `mcu_budget` to inspect memory; build output files are generated.
- STM32 project source is present in this project.
- The current ELF exists in the build output, not as a separately packaged release artifact.
- No authentic deployed-model SHA-256 identifier is available yet; the provided ONNX artifact was not found within the permitted workspace scope.
- Serial capture is stored in `category2-serial-capture.log`. It records intentional `model_runtime_not_integrated` failure, not five inference results.
- This record names the board, build and flash result and observed output, but does not claim an observed inference result.

## Remaining requirements not evidenced by this build

- **NFR-006:** Fresh-build behavioral reproducibility requires two complete model-enabled target runs; the current model-less firmware cannot demonstrate it.
- **FR-033:** The model's SHA-256 cannot be delivered until the model artifact is integrated and its exact identity is established.
- **FR-034:** The required five-vector result log is not available; current capture contains only the startup failure and FAIL summary.
- **FR-035:** A validation record exists, but inference outcome is explicitly unavailable because the model/runtime is absent.
- **NFR-007:** Current map includes static state, 512-byte linker heap reservation, and 1024-byte stack reservation. Activation/context/I/O model storage and runtime stack high-water measurements are not yet available, so the complete ML runtime SRAM criterion is not established.
