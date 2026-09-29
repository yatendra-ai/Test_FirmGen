# ML component handoff

Register components.json through FirmGen's firmgen_user_components.json. Its existing STM32 build compiles source components into the firmware target with its CPU/FPU options.

Supply a board-specific fg_ml_backend_t that calls the generated compiler runtime. Initialize clocks, external memory, NPU/cache, and model weights before fg_ml_init. Use the installed ST example matching this compiler version; source generation does not include a board BSP. Do not guess activation addresses or a linker script. Add the required STAI/LL_ATON middleware from that release through the existing mcu_component tool.

The bundle alone is not a running integration. Build, flash, initialization, actual inference and reference comparison must all succeed before TARGET_VALIDATED. Pre/postprocessing are recorded in pipeline.json and must be implemented by the application.
