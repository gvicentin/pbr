#ifndef IMU_H
#define IMU_H

#include <stdbool.h>

// MPU6050 full-scale range.
//
// ┌────────┬────────────────────┬────────────────┐
// │ Scale  │ Gyro Range (deg/s) │ Accl Range (g) │
// ├────────┼────────────────────┼────────────────┤
// │ 0      │ +/- 250            │ +/- 2          │
// │ 1      │ +/- 500            │ +/- 4          │
// │ 2      │ +/- 1000           │ +/- 8          │
// │ 3      │ +/- 2000           │ +/- 16         │
// └────────┴────────────────────┴────────────────┘
//
typedef enum {
    IMU_FS_0 = 0,
    IMU_FS_1,
    IMU_FS_2,
    IMU_FS_3
} imu_scale;

typedef struct imu_config_t {
    // full-scale range for the gyro and accel
    imu_scale gyro_scale;
    imu_scale accl_scale;

    // should perform calibration routine when initializing
    bool init_calibration;
} imu_config;

int imu_init(imu_config config);

void imu_read(float accl[3], float *temp, float gyro[3]);

#endif // IMU_H
