add_executable(br
        main.c
        imu.c
        )

pico_enable_stdio_usb(br 1)
pico_enable_stdio_uart(br 0)

pico_add_extra_outputs(br)

target_link_libraries(br pico_stdlib hardware_i2c)