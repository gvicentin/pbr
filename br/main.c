#include <stdbool.h>
#include <stdio.h>
#include "pico/time.h"
#include "pico/stdlib.h"

#include "imu.h"

int main(void) {
    stdio_init_all();

    imu_config imu_conf = {
        .accl_scale = IMU_FS_0,
        .gyro_scale = IMU_FS_1,
        .init_calibration = true
    };
    imu_init(imu_conf);

    float accl[3], temp, gyro[3];

    while (true) {
        imu_read(accl, &temp, gyro);
        // printf("%f %f %f %f %f %f\n", accl[0], accl[1], accl[2],
        //                               gyro[0], gyro[1], gyro[2]);
        printf("%f %f %f\n", gyro[0], gyro[1], gyro[2]);
        sleep_ms(200);
    }

    return 0;
}
