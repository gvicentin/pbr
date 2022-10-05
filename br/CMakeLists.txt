add_executable(br
        main.c
        mpu6050.c
        uln2003.c
        imu.c
        )

pico_enable_stdio_usb(br 1)
pico_enable_stdio_uart(br 0)

pico_add_extra_outputs(br)

# Log Level
#
# 0 -> NONE
# 1 -> ERROR
# 2 -> DEBUG
#
# For Serial Plot debugging, use one of the following levels.
# Setting with one of these values will disable the default logs.
# 4 -> RAW SENSORS
# 5 -> IMU
# 6 -> PID
target_compile_definitions(br PUBLIC LOG_LEVEL=4)

target_link_libraries(br pico_stdlib hardware_i2c)