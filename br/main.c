#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "log.h"
#include "mpu6050.h"

int main(void) {
    stdio_init_all();

    float gyro[3], accel[3];

    // init mpu
    mpu6050_config mpu_config = {
        .i2c_inst = i2c_default,
        .i2c_sda_pin = PICO_DEFAULT_I2C_SDA_PIN,
        .i2c_scl_pin = PICO_DEFAULT_I2C_SCL_PIN
    };
    mpu6050_init(mpu_config);
    mpu6050_set_gyro_fs(MPU6050_GFS_250);
    mpu6050_set_accel_fs(MPU6050_AFS_2G);

    while (true) {
        LOG_ERROR("This is a error log");
        LOG_DEBUG("This is a debug log");

        mpu6050_read_gyro(gyro);
        mpu6050_read_accel(accel);
        SP_RAW_SENSORS(gyro, accel);
        
        sleep_ms(20);
    }

    return 0;
}
