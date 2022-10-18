#include "imu.h"

#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "log.h"
#include "mpu6050.h"

static float m_gyro[3];
static float m_accel[3];
static float m_gyro_accum[3] = { 0 };

static void imu_calibrate(uint32_t discard_count, uint32_t avg_count) {
    float accel_readings[3] = { 0 };
    float gyro_readings[3] = { 0 };
    float sum[6] = { 0 }, avg[6] = { 0 }, bias[6] = { 0 };

    LOG_DEBUG("Calibrating sensor");

    // Prepare calibration
    mpu6050_pre_calibration();

    // Discard initial readings
    for(uint32_t i = 0; i < discard_count; ++i) {
        mpu6050_read_accel(accel_readings);
        mpu6050_read_gyro(gyro_readings);
        busy_wait_ms(5);
    }

    // Calculate averages
    for (uint32_t i = 0; i < avg_count; ++i) {
        mpu6050_read_accel(accel_readings);
        mpu6050_read_gyro(gyro_readings);

        // Accel X, Y and Z
        sum[0] += accel_readings[0];
        sum[1] += accel_readings[1];
        sum[2] += accel_readings[2];
        // Gyro X, Y and Z
        sum[3] += gyro_readings[0];
        sum[4] += gyro_readings[1];
        sum[5] += gyro_readings[2];
        busy_wait_ms(5);
    }

    for (uint8_t i = 0; i < 6; ++i) {
        avg[i] = sum[i] / avg_count;
        bias[i] = -avg[i];
    }

    // Compensate for gravity on accel Z axis
    bias[2] = bias[2] + 1.0f;

    LOG_DEBUG("Accel bias: %f, %f, %f", bias[0], bias[1], bias[2]);
    LOG_DEBUG("Gyro bias: %f, %f, %f", bias[3], bias[4], bias[5]);

    // Correcting sensor
    mpu6050_set_accel_offset(bias);
    mpu6050_set_gyro_offset(bias + 3);
}

int imu_init(bool calibrate) {
    LOG_DEBUG("Init IMU");

    // Initialize sensor
    mpu6050_config mpu_config = {
        .i2c_inst = i2c_default,
        .i2c_sda_pin = PICO_DEFAULT_I2C_SDA_PIN,
        .i2c_scl_pin = PICO_DEFAULT_I2C_SCL_PIN
    };
    mpu6050_init(mpu_config);

    if (calibrate) {
        imu_calibrate(100, 1000);
    }

    // Setup full-scale
    mpu6050_set_gyro_fs(MPU6050_GFS_500);
    mpu6050_set_accel_fs(MPU6050_AFS_4G);
}

void imu_calculate(void) {
    mpu6050_read_accel(m_accel);
    mpu6050_read_gyro(m_gyro);

    m_gyro_accum[0] += m_gyro[0] * 0.05f;
    m_gyro_accum[1] += m_gyro[1] * 0.05f;
    m_gyro_accum[2] += m_gyro[2] * 0.05f;

    SP_RAW_SENSORS(m_gyro, m_accel);
    SP_IMU(m_gyro_accum, m_accel);
}
