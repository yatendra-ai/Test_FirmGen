# FirmGen ML Validation Report

Final state: **MODEL_COMPILED**
Deployment validated: **False**

| Stage | Result |
|---|---|
| MODEL_RECEIVED | recorded |
| MODEL_PARSED | recorded |
| TARGET_CHECKED | recorded |
| MODEL_ANALYZED | recorded |
| MODEL_COMPATIBLE | recorded |
| MODEL_COMPILED | recorded |

## Model

```json
{
  "model": "c5_dense_relu.onnx",
  "format": "onnx",
  "framework": "ONNX",
  "sha256": "15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090",
  "size_bytes": 245,
  "inputs": [
    {
      "name": "input_output_array",
      "shape": [
        2
      ],
      "dtype": "float32",
      "size_bytes": 8,
      "quantization": {}
    }
  ],
  "outputs": [
    {
      "name": "output_output_array",
      "shape": [
        2
      ],
      "dtype": "float32",
      "size_bytes": 8,
      "quantization": {}
    }
  ],
  "operators": [
    "Add",
    "MatMul",
    "Relu"
  ],
  "quantization": "float",
  "inspection": "onnx",
  "warnings": [],
  "inspector_version": "1.23.0",
  "original_model": {
    "inputs": [
      {
        "name": "input",
        "shape": [
          1,
          2
        ],
        "dtype": "float32"
      }
    ],
    "outputs": [
      {
        "name": "output",
        "shape": [
          1,
          2
        ],
        "dtype": "float32"
      }
    ],
    "operators": [
      "Add",
      "MatMul",
      "Relu"
    ],
    "quantization": "unquantized_or_unknown"
  },
  "compiler_inspected": true
}
```

## Target

```json
{
  "board": "NUCLEO-C5A3ZG",
  "mcu": "STM32C5A3ZGT6",
  "family": "STM32C5",
  "vendor": "st",
  "compiler_target": "stm32c5",
  "cpu": "Cortex-M33",
  "cpu_frequency_hz": 144000000,
  "cpu_frequency_note": "Device maximum, not a requested/generated clock setting.",
  "fpu_present": true,
  "dsp_present": true,
  "ai_capable": true,
  "npu_present": false,
  "npu_type": null,
  "ml_backend": "st_edge_ai",
  "sram_bytes": 262144,
  "flash_bytes": 1048576,
  "external_ram_bytes": 0,
  "external_flash_bytes": 0,
  "memory_note": "DS15137 Rev3 table1 specifies256KiB SRAM and1MiB flash. These are chip capacity upper bounds; the generated project's linker map remains authoritative. No external memory is needed by this example.",
  "sources": [
    "https://www.st.com/resource/en/user_manual/dm01239539.pdf",
    "https://www.st.com/resource/en/datasheet/stm32c5a3zg.pdf",
    "https://www.st.com/en/evaluation-tools/nucleo-c5a3zg.html"
  ],
  "kb_source": "C:\\Users\\YatendraBhargava\\AppData\\Local\\Temp\\3JzyvBeU2zQKHyM0HnS92fPHtmf\\resources\\ml-agent\\runtime\\Lib\\site-packages\\firmgen_ml\\data\\nucleo-c5a3zg.json",
  "kb_mode": "standalone_profile",
  "ml_profile": "C:\\Users\\YatendraBhargava\\AppData\\Local\\Temp\\3JzyvBeU2zQKHyM0HnS92fPHtmf\\resources\\ml-agent\\runtime\\Lib\\site-packages\\firmgen_ml\\data\\nucleo-c5a3zg.json",
  "raw_kb_record": null
}
```

## Toolchain

```json
{
  "vendor": "st",
  "engine": "stedgeai",
  "executable": "C:\\ST\\STEdgeAI\\4.0\\Utilities\\windows\\stedgeai.exe",
  "version": "4.0.1-20581",
  "formats": [
    "onnx",
    "tflite",
    "h5",
    "hdf5"
  ],
  "frameworks": [
    "onnx",
    "tflite",
    "keras"
  ],
  "targets": [
    "stm32c0",
    "stm32c5",
    "stm32f0",
    "stm32f3",
    "stm32f4",
    "stm32f7",
    "stm32g0",
    "stm32g4",
    "stm32h5",
    "stm32h7",
    "stm32l0",
    "stm32l4",
    "stm32l5",
    "stm32n6",
    "stm32u3",
    "stm32u5",
    "stm32v8"
  ],
  "neural_art": true,
  "capability_source": "installed_cli_help",
  "neural_art_diagnostic": "Neural-ART backend help verified",
  "neural_art_executable": "C:\\ST\\STEdgeAI\\4.0\\Utilities\\windows\\atonn.exe",
  "optimization": true,
  "c_api_selection": true,
  "supported_report_schemas": [
    "2.0"
  ]
}
```

## Memory Check

```json
{
  "capacity_bytes": {
    "ram": 262144,
    "flash": 1048576
  },
  "required_bytes": {
    "ram": 16,
    "flash": 1578
  },
  "status": "capacity_check_only",
  "placement_validated": false
}
```

Board runtime binding, version-matched middleware, external weights and memory placement are required before target execution.
