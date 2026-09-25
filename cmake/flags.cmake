# Compiler / linker flags and include paths.

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})

if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-C5A3ZG")
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
    "SHELL:-Os -g -fdata-sections -ffunction-sections -std=gnu11 -Wall -Wextra \
-fstack-usage --specs=nano.specs --specs=nosys.specs \
-Werror=implicit-function-declaration ${CC_SECURE}")
else()
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
    "SHELL:-g3 -O0 -fdata-sections -ffunction-sections -std=gnu11 -Wall -Wextra \
-fstack-usage --specs=nano.specs --specs=nosys.specs \
-Werror=implicit-function-declaration ${CC_SECURE}")
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Project-owned headers
  ${CMAKE_SOURCE_DIR}/application
  ${CMAKE_SOURCE_DIR}/drivers/inc

  # Cube-generated code shipped with this project
  ${CMAKE_SOURCE_DIR}/generated/hal
  ${CMAKE_SOURCE_DIR}/generated/parts/led

  # STM32CubeC5 pack
  ${STM32CUBEC5_DIR}/arch/cmsis/CMSIS/Core/Include
  ${STM32CUBEC5_DIR}/part_drivers/led/gpio
  ${STM32CUBEC5_DIR}/stm32c5xx_dfp/Include
  ${STM32CUBEC5_DIR}/stm32c5xx_drivers/hal
  ${STM32CUBEC5_DIR}/stm32c5xx_drivers/ll
  ${STM32CUBEC5_DIR}/stm32c5xx_drivers/timebases
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
  STM32C5A3xx
  _RTE_
)

target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
  -Wl,-Map=${CMAKE_PROJECT_NAME}.map
  -Wl,--gc-sections
  -Wl,--print-memory-usage
  -Wl,--start-group -lc -lm -Wl,--end-group
  -static
  --specs=nano.specs
  --specs=nosys.specs
  ${CC_SECURE}
)
target_link_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
  "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")
