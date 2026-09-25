# CMSIS component selection for this project.
#
# Identical for every build type, so unlike the Cube examples there is no
# per-configuration branching here. Adding a peripheral means adding its
# HAL/LL components to this list AND generating the matching mx_*.c.

set(CMSIS_COMPONENTS_LIST)
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LED#Csub:GPIO#Cversion:1.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LED#Cversion:1.0.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:CMSIS#Cgroup:CORE#Cversion:6.2.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:CORTEX#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:Core#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:DMA#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:FLASH_ITF#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:GPIO#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:ICACHE#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:PWR#Cversion:2.0.1")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:Q#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:RCC#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:TIM#Cversion:2.0.1")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 HAL#Csub:timebases#Cvariant:SYSTICK#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:BUS#Cversion:2.0.1")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:Common#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:DMA#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:EXTI#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:FLASH#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:GPIO#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:ICACHE#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:PWR#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:RCC#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:SBS#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:TIM#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32 LL#Csub:UTILS#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Codegen#Csub:Generated Code#Cversion:2.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:CORE#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:CORTEX MPU#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:CORTEX NVIC#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:GPIO#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:ICACHE#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:RCC#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:System#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:STM32CubeMX2 Config#Csub:TIM#Cversion:2.1.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Device#Cgroup:Startup#Cversion:2.1.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:STM32CubeMX2 Config#Csub:syscalls#Cversion:1.0.0#generated:true")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:syscalls#Cvariant:Standalone syscalls#Cversion:1.0.0")
list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:sysmem#Cvariant:Standalone sysmem#Cversion:1.0.0")

# Where the DFP looks for project-owned startup / system / linker files.
# This is what makes user_modifiable/Device/STM32C5A3ZGT6/stm32c5a3xg_flash.ld
# win over the stock template shipped in the pack.
set(CMSIS_RTE_FOLDER "${CMAKE_SOURCE_DIR}/user_modifiable")

# --- Pack components -------------------------------------------------------
add_subdirectory(${STM32CUBEC5_DIR}/arch/cmsis            ${CMAKE_BINARY_DIR}/STMicroelectronics.CMSIS.6.3.0)
add_subdirectory(${STM32CUBEC5_DIR}/part_drivers/led      ${CMAKE_BINARY_DIR}/STMicroelectronics.led_part_drivers.2.0.0)
add_subdirectory(${STM32CUBEC5_DIR}/stm32c5xx_dfp         ${CMAKE_BINARY_DIR}/STMicroelectronics.stm32c5xx_dfp.2.1.0)
add_subdirectory(${STM32CUBEC5_DIR}/stm32c5xx_drivers     ${CMAKE_BINARY_DIR}/STMicroelectronics.stm32c5xx_hal_drivers.2.1.0)
add_subdirectory(${STM32CUBEC5_DIR}/utilities/syscalls    ${CMAKE_BINARY_DIR}/STMicroelectronics.syscalls.2.0.0)

# --- Cube-generated code shipped with this project -------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/generated/hal                ${CMAKE_BINARY_DIR}/generated/hal)
add_subdirectory(${CMAKE_SOURCE_DIR}/generated/parts/led          ${CMAKE_BINARY_DIR}/generated/parts/led)
add_subdirectory(${CMAKE_SOURCE_DIR}/generated/utilities/syscalls ${CMAKE_BINARY_DIR}/generated/utilities/syscalls)

target_link_libraries(${CMAKE_PROJECT_NAME}
  generated_STMicroelectronics_led_part_drivers_0_0_1
  generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1
  generated_STMicroelectronics_syscalls_2_0_0
  STMicroelectronics_CMSIS_6_3_0
  STMicroelectronics_led_part_drivers_2_0_0
  STMicroelectronics_stm32c5xx_dfp_2_1_0
  STMicroelectronics_stm32c5xx_hal_drivers_2_1_0
  STMicroelectronics_syscalls_2_0_0
)
