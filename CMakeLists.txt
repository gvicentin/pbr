cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pbr C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico's SDK
pico_sdk_init()

add_subdirectory(br) # balancing robot module
add_subdirectory(rc) # remote controller module