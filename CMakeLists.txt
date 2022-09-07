cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picopter_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

include_directories("${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.c")
add_executable (picopter ${SOURCE_FILES})

pico_enable_stdio_usb(picopter 1)
pico_enable_stdio_uart(picopter 0)

pico_add_extra_outputs(picopter)

target_link_libraries(picopter pico_stdlib hardware_i2c)
