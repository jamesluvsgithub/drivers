
cmake_minimum_required(VERSION 3.13)

if(NOT PICO_BOARD)
    set(PICO_BOARD adafruit_feather_rp2040)
endif()

# Import the Raspberry Pi Pico SDK.
include(pico_sdk_import.cmake)

project(bubble_level C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

pico_sdk_init()

# Source directory.
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# LIS3DH static library.
add_library(lis3dh STATIC
    ${SRC_DIR}/lis3dh.cpp
)

target_include_directories(lis3dh PUBLIC
    ${SRC_DIR}
)

target_link_libraries(lis3dh PUBLIC
    pico_stdlib
    hardware_i2c
    hardware_gpio
)

# NeoMatrix static library.
add_library(neomatrix STATIC
    ${SRC_DIR}/neomatrix.cpp
)

target_include_directories(neomatrix PUBLIC
    ${SRC_DIR}
)

# Generate the C/C++ header from the PIO assembly program.
pico_generate_pio_header(neomatrix
    ${SRC_DIR}/ws2812.pio
)

target_link_libraries(neomatrix PUBLIC
    hardware_pio
    hardware_gpio
    pico_stdlib
)

# Bubble level application.
add_executable(level
    ${SRC_DIR}/level.cpp
)

target_link_libraries(level PRIVATE
    pico_stdlib
    lis3dh
    neomatrix
)

# Generate .uf2, .bin, .hex, and other outputs.
pico_add_extra_outputs(level)

pico_enable_stdio_usb(level 1)
pico_add_extra_outputs(level)