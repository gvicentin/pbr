#include "imu.h"

#include <stdint.h>
#include <pico/stdlib.h>
#include "mpu6050.h"

void imu_calibrate(uint32_t discard_count, uint32_t avg_count) {
    int16_t read_values[6] = { 0 };
    int32_t sum[6] = { 0 };
    int16_t avg[6] = { 0 };

    // Prepare calibration
    mpu6050_pre_calibration();

    // Discard initial readings
    for(uint32_t i = 0; i < discard_count; ++i) {
        mpu6050_read_raw(read_values);
        busy_wait_ms(2);
    }

    // Calculate averages
    for (uint32_t i = 0; i < avg_count; ++i) {
        mpu6050_read_raw(read_values);
        // Accel X, Y and Z
        sum[0] += read_values[0];
        sum[1] += read_values[1];
        sum[2] += read_values[2];
        // Gyro X, Y and Z
        sum[3] += read_values[3];
        sum[4] += read_values[4];
        sum[5] += read_values[5];
    }
    
    for (uint8_t i = 0; i < 6; ++i) {
        avg[i] = sum[i] / avg_count;
    }

    // Compensate for gravity on accel Z axis
    avg[2] = avg[2] - MPU6050_STATIC_Z_ACCEL_READ;

    // Correcting sensor
    int16_t *accel_offsets = avg;
    int16_t *gyro_offsets = avg + 3;
    mpu6050_set_accel_offset(accel_offsets);
    mpu6050_set_gyro_offset(gyro_offsets);
}

int imu_init(void) {
    imu_calibrate(1000, 5000);
}
