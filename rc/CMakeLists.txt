add_executable(rc
        main.c
        )

pico_enable_stdio_usb(rc 1)
pico_enable_stdio_uart(rc 0)

pico_add_extra_outputs(rc)

target_link_libraries(rc pico_stdlib hardware_i2c)
