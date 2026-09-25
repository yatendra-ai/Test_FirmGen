# Compatibility entry point used by Firmgen's generic STM32 build command.
# The project-specific GCC settings are loaded by CMakeLists.txt after the
# device description is selected.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
