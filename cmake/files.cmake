# Project-owned sources.
#
# Cube-generated code under generated/ is pulled in by components.cmake, and
# the startup / system / linker files under user_modifiable/ are picked up by
# the DFP through CMSIS_RTE_FOLDER. Only hand-written code belongs here.

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  # Application
  ${CMAKE_SOURCE_DIR}/application/main.c
  ${CMAKE_SOURCE_DIR}/application/ramp_control.c

  # Reserved flash config page (.app_config)
  ${CMAKE_SOURCE_DIR}/config/app_config.c

  # Custom drivers are linked through customer_drivers in the root CMakeLists.
)
