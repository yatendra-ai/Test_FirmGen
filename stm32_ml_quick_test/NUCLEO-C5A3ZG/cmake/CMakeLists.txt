# file-format: 1.0.0
cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

include(cmake/target.cmake)
include(cmake/${TOOLCHAIN_FILE})

project(template_start_from_nucleo)
add_executable(template_start_from_nucleo)
include(cmake/files.cmake)
include(cmake/flags.cmake)
include(cmake/components.cmake)

# Stubs to be able to add compiler flag overrides, e.g. -Os
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  # Add additional source files here
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional include directories here
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional defined symbols here
)

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional compiler flags here
)

target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker flags here
)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker search directories here
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker libraries here
)

# >>> FIRMGEN_FIRMWARE
include(${CMAKE_CURRENT_LIST_DIR}/firmgen_firmware.cmake)
# <<< FIRMGEN_FIRMWARE

# >>> FIRMGEN_USER_COMPONENTS
# User-provided components (custom linker script, existing libraries/drivers).
# Generated from firmgen_user_components.json; see .craftif/user_components.cmake.
include("${CMAKE_CURRENT_LIST_DIR}/../../.craftif/user_components.cmake" OPTIONAL)
if(COMMAND firmgen_attach_user_components)
  firmgen_attach_user_components(${CMAKE_PROJECT_NAME})
endif()
# <<< FIRMGEN_USER_COMPONENTS
