#include <stdbool.h>
#include <stdio.h>
#include "pico/time.h"
#include "pico/stdlib.h"

#include "imu.h"

int main(void) {
    stdio_init_all();

    imu_config imu_conf = {
        .accl_scale = IMU_FS_0,
        .gyro_scale = IMU_FS_0,
        .init_calibration = true
    };
    imu_init(imu_conf);

    float accl[3], temp, gyro[3];

    while (true) {
        imu_read(accl, &temp, gyro);
        printf("AcclX: %f, GyroX: %f\n", accl[0], gyro[0]);
        sleep_ms(200);
    }

    return 0;
}
