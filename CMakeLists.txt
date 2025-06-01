cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(my_project)

pico_sdk_init()

add_executable(main
    main.cpp
    lcd_display/lcd_display.cpp
)

target_include_directories(main PRIVATE
    lcd_display
)

target_link_libraries(main pico_stdlib hardware_spi hardware_pwm)

pico_add_extra_outputs(main)

