#ifndef IMU_H
#define IMU_H

#include <stdbool.h>

int imu_init(bool calibrate);

void imu_calculate(void);

#endif // IMU_H
