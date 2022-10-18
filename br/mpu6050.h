#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "hardware/i2c.h"

// Calibration register offset
#define MPU6050_STATIC_Z_ACCEL_READ 4096

// Gyroscope Full-Scale Range.
//
// MPU6050_GFS_X range from -X to +X (°/s)
// e.g. MPU_6050_GFS_250 range from -250 (°/s) to +250 (°/s)
typedef enum {
    MPU6050_GFS_250 = 0,
    MPU6050_GFS_500,
    MPU6050_GFS_1000,
    MPU6050_GFS_2000
} mpu6050_gyro_fs;

// Accelerometer Full-Scale Range.
//
// MPU6050_AFS_X range from -X to +X (G)
// e.g. MPU_6050_AFS_2G range from -2G to +2G.
typedef enum {
    MPU6050_AFS_2G = 0,
    MPU6050_AFS_4G,
    MPU6050_AFS_8G,
    MPU6060_AFS_16G
} mpu6050_accel_fs;

typedef struct mpu6050_config_t {
    // Raspberry Pi Pico hardware
    i2c_inst_t *i2c_inst;
    uint8_t i2c_sda_pin;
    uint8_t i2c_scl_pin;
} mpu6050_config;

int mpu6050_init(mpu6050_config config);

void mpu6050_reset(void);

void mpu6050_set_gyro_fs(mpu6050_gyro_fs gyro_fs);

void mpu6050_set_accel_fs(mpu6050_accel_fs accel_fs);

void mpu6050_read_gyro(float *gyro);

void mpu6050_read_accel(float *accel);

void mpu6050_pre_calibration(void);

void mpu6050_set_gyro_offset(float *offsets);

void mpu6050_set_accel_offset(float *offsets);

#endif // MPU6050_H
