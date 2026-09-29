[page 1]
NUCLEO-C5A3ZG ML Quick Test | Functional PRD v1.1
Functional Requirements Document
NUCLEO-C5A3ZG ML Quick Test
Product Requirements Document | Version 1.1 | 29 September 2026
1. Objective
Create an STM32 application for the NUCLEO-C5A3ZG that executes the supplied FP32 ONNX model on the MCU 
CPU and reports five known-vector comparisons through the ST-LINK Virtual COM Port. After reset, the board 
shall produce a concise and deterministic PASS/FAIL result.
2. Target and operating conditions
 Board: NUCLEO-C5A3ZG, reference MB2310.
 MCU: STM32C5A3ZGT6, Cortex-M33 with single-precision FPU and DSP.
 Maximum device resources: 144 MHz CPU clock, 1 MiB Flash, 256 KiB SRAM.
 No external sensor, external memory, network connection, RTOS or NPU is required.
 Console: on-board STLINK-V3EC Virtual COM Port at 115200 baud, 8 data bits, no parity, 1 stop bit, no flow 
control.
 Use the UART instance and pins supported by the selected board configuration; do not assume unsupported pin 
mappings.
3. ML model contract
Use c5_dense_relu.onnx unchanged. The model is FP32, ONNX opset 17 / IR 8, with one input tensor named input 
and one output tensor named output, both with shape [1,2]. The graph consists of MatMul, Add and ReLU 
operations. Model weights and biases are embedded. No preprocessing or postprocessing other than direct FP32 
input/output handling is required.
Expected computation:
 y0 = max(0, 2*x0 - x1 + 0.5)
 y1 = max(0, -x0 + 3*x1 - 1.0)
The deployed firmware shall execute the compiled neural-network model using the ST AI runtime. A handwritten 
replacement of the equations shall not be considered valid model inference.
4. Required application behavior
 On reset, initialize the MCU clock, FPU and serial console.
 Initialize the generated ML network and all required activation, context and I/O buffers.
 Execute the five reference input vectors exactly once each.
 For every vector, compare both FP32 outputs with the expected values before formatting them for display.
 Deinitialize the ML runtime after the test sequence completes.
 Print one deterministic result line per vector and a final summary.
 Any initialization failure, inference failure, non-finite output, deinitialization failure or console transmission 
failure shall cause the test to fail.
 The application shall not require dynamic heap allocation for the test sequence.
5. Test vectors and console contract
ID Input Expected output Display in milli-units
positive [1,2] [0.5,4] [500,4000]
first_clamped [-2,1] [0,4] [0,4000]
second_clamped [3,-1] [7.5,0] [7500,0]
zero_input [0,0] [0.5,0] [500,0]

[page 2]
NUCLEO-C5A3ZG ML Quick Test | Functional PRD v1.1
Functional Requirements Document
both_clamped [-1,-1] [0,0] [0,0]
Comparison tolerance: absolute tolerance 0.000001 and relative tolerance 0. The comparison shall use the original 
FP32 values. Console display values shall be multiplied by 1000 and printed as integers.
 Print the model SHA-256 identifier at startup.
 Print ML_RUNTIME_READY only after successful model/runtime initialization.
 Print exactly one ML_VECTOR result line for each executed vector.
 Print ML_SUMMARY passed=5 total=5 status=PASS only when all five comparisons and cleanup succeed.
 If any vector or runtime operation fails, the final status shall be FAIL and the failing condition shall be 
reported when console output remains available.
6. Build and runtime requirements
 The model shall be compiled for STM32C5 CPU execution using an ST AI compatible C runtime.
 All generated model sources, runtime libraries and application objects shall use a mutually compatible Cortex-
M33 CPU/FPU ABI.
 The final firmware shall link without unresolved model/runtime symbols, ABI mismatch errors or overlapping 
memory regions.
 The model weights shall remain embedded in the firmware image.
 The generated linker memory placement shall fit within the target MCU Flash and SRAM limits.
 The application shall start correctly after flashing and a hardware reset.
7. Acceptance criteria
1. The supplied ONNX model is converted into deployable STM32C5 inference code without changing the model 
graph or weights.
2. The complete firmware builds and links successfully for NUCLEO-C5A3ZG.
3. The firmware can be flashed to the physical board and starts after reset.
4. The serial console reports all five test vectors.
5. Each vector matches the expected FP32 result within the defined tolerance.
6. The final console summary reports passed=5 total=5 status=PASS only when all five vectors succeed.
7. Wrong, missing or non-finite outputs, or any ML runtime failure, cannot be counted as a pass.
8. A fresh rebuild of the same source and model produces the same functional behavior on the target board.
8. Deliverables
 STM32 project source code.
 Compiled ML model sources and required runtime integration files.
 Final firmware binary/ELF artifact.
 Model SHA-256 identifier.
 Captured serial log showing startup, five vector results and the final PASS/FAIL summary.
 Short validation record stating board used, firmware build result, flash result and observed inference result.