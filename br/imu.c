#include "imu.h"

#include <stdint.h>
#include <pico/stdlib.h>

#include "log.h"
#include "mpu6050.h"

void imu_calibrate(uint32_t discard_count, uint32_t avg_count) {
    int16_t read_values[6] = { 0 };
    int32_t sum[6] = { 0 };
    int16_t avg[6] = { 0 }, bias[6] = { 0 };

    LOG_DEBUG("Calibrating sensor");

    // Prepare calibration
    mpu6050_pre_calibration();

    // Discard initial readings
    for(uint32_t i = 0; i < discard_count; ++i) {
        mpu6050_read_raw(read_values);
        busy_wait_ms(5);
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
        busy_wait_ms(5);
    }

    for (uint8_t i = 0; i < 6; ++i) {
        avg[i] = sum[i] / avg_count;
        bias[i] = -avg[i];
    }

    // Compensate for gravity on accel Z axis
    bias[2] = bias[2] - MPU6050_STATIC_Z_ACCEL_READ;

    // Correcting sensor
    int16_t *accel_offsets = bias;
    int16_t *gyro_offsets = bias + 3;
    mpu6050_set_accel_offset(accel_offsets);
    mpu6050_set_gyro_offset(gyro_offsets);
}

int imu_init(bool calibrate) {
    LOG_DEBUG("Init IMU");

    // Initialize sensor
    mpu6050_config mpu_config = {
        .i2c_inst = PICO_DEFAULT_I2C_INSTANCE,
        .i2c_sda_pin = PICO_DEFAULT_I2C_SDA_PIN,
        .i2c_scl_pin = PICO_DEFAULT_I2C_SCL_PIN
    };
    mpu6050_init(mpu_config);

    if (calibrate) {
        imu_calibrate(1000, 5000);
    }
}
