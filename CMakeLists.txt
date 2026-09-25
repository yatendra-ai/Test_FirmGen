cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
if(DEFINED ENV{STM32CUBEC5_DIR})
  set(_cube_default "$ENV{STM32CUBEC5_DIR}")
elseif(DEFINED ENV{USERPROFILE})
  set(_cube_default "$ENV{USERPROFILE}/STM32Cube/Repository/STM32CubeC5")
else()
  set(_cube_default "$ENV{HOME}/STM32Cube/Repository/STM32CubeC5")
endif()
set(STM32CUBEC5_DIR "${_cube_default}" CACHE PATH "STM32CubeC5 package root")
if(NOT EXISTS "${STM32CUBEC5_DIR}/stm32c5xx_drivers/hal/stm32_hal.h")
  message(FATAL_ERROR "Missing STM32CubeC5 dependency: ${STM32CUBEC5_DIR}. Set STM32CUBEC5_DIR to your installed pack; do not regenerate this project.")
endif()
include(cmake/target.cmake)
include(cmake/GCC.14.3.1.cmake)
project(pwm_d7_customer C ASM)
add_executable(pwm_d7_customer)
include(cmake/files.cmake)
include(cmake/flags.cmake)
include(cmake/components.cmake)

# Customer driver library: the application uses its API, not duplicate register code.
add_library(customer_drivers STATIC drivers/src/drv_pwm.c drivers/src/drv_boot_record.c)
target_include_directories(customer_drivers PUBLIC drivers/inc application generated/hal generated/parts/led)
target_compile_options(customer_drivers PRIVATE ${CPU_FLAGS} -g3 -Og -ffunction-sections -fdata-sections -Wall -Wextra)
target_compile_definitions(customer_drivers PRIVATE STM32C5A3xx _RTE_)
target_link_libraries(customer_drivers PRIVATE
  generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1
  generated_STMicroelectronics_led_part_drivers_0_0_1
  STMicroelectronics_CMSIS_6_3_0
  STMicroelectronics_stm32c5xx_dfp_2_1_0
  STMicroelectronics_stm32c5xx_hal_drivers_2_1_0
  STMicroelectronics_led_part_drivers_2_0_0)
target_link_libraries(pwm_d7_customer customer_drivers)
set_property(TARGET pwm_d7_customer APPEND PROPERTY LINK_DEPENDS
  "${CMAKE_SOURCE_DIR}/user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld")
add_custom_command(TARGET pwm_d7_customer POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:pwm_d7_customer>)

# >>> FIRMGEN_USER_COMPONENTS
# User-provided components (custom linker script, existing libraries/drivers).
# Generated from firmgen_user_components.json; see .craftif/user_components.cmake.
include("${CMAKE_CURRENT_LIST_DIR}/.craftif/user_components.cmake" OPTIONAL)
if(COMMAND firmgen_attach_user_components)
  firmgen_attach_user_components(${PROJECT_NAME})
endif()
# <<< FIRMGEN_USER_COMPONENTS
