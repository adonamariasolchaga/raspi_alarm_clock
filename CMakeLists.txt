cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(alarm_clock C CXX ASM)

pico_sdk_init()

add_executable(alarm_clock
    src/main.cpp
    lib/lcd_display/lcd_display.cpp
    lib/lcd_ui/lcd_ui.cpp
)

target_include_directories(alarm_clock PRIVATE
    src
    lib/lcd_display
    lib/lcd_ui
)

target_link_libraries(alarm_clock
    pico_stdlib
    pico_time
    hardware_spi
    hardware_pwm
)

pico_add_extra_outputs(alarm_clock)

