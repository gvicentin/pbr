#include "mpu6050.h"

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// MPU6050 IC2 uses 40kH
#define MPU6050_I2C_FREQ 400000

// Default address is 0x68 (AD0 -> GND).
#define MPU6050_I2C_ADDR 0x68

// Used registers
#define MPU6050_REG_GYRO_CONFIG     0x1B
#define MPU6050_REG_ACCEL_CONFIG    0x1C
#define MPU6050_REG_ACCEL_XOUT_H    0x3B
#define MPU6050_REG_TEMP_OUT_H      0x41
#define MPU6050_REG_GYRO_XOUT_H     0x43
#define MPU6050_REG_PWG_MGMT_1      0x6B

// Raspberry Pi Pico IC2 instance
static i2c_inst_t *m_i2c_inst = NULL;

// Sensitivity Scale Factor
static float m_gyro_ssf, m_accel_ssf;
static float m_gyro_ssf_values[] = {131.0f, 65.5f, 32.8f, 16.4f};
static float m_accel_ssf_values[] = {16384.0f, 8192.0f, 4096.0f, 2048.0f};

int mpu6050_init(mpu6050_config config) {
    // Setup globals
    m_i2c_inst = config.i2c_inst;

    // Select I2C instance from the Pico and setup desired pins
    i2c_init(config.i2c_inst, MPU6050_I2C_FREQ);
    gpio_set_function(config.i2c_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(config.i2c_scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(config.i2c_sda_pin);
    gpio_pull_up(config.i2c_scl_pin);

    // Reset board
    mpu6050_reset();

    return 0;
}

void mpu6050_reset(void) {
    // Bits:
    // 7     -> Reset device and set all registers to default value
    // 6     -> Enable sleep mode
    // [2:0] -> Clock source. Will be using one of the gyro oscillator
    uint8_t reset_buf[] = {
        MPU6050_REG_PWG_MGMT_1,
        0b00000001
    };
    i2c_write_blocking(m_i2c_inst, MPU6050_I2C_ADDR, reset_buf, 2, false);
}

void mpu6050_set_gyro_fs(mpu6050_gyro_fs gyro_fs) {
    // [4:3] bits to select the Full Scale
    uint8_t gyro_buf[] = {
        MPU6050_REG_GYRO_CONFIG,
        ((uint8_t) gyro_fs << 3)
    };
    i2c_write_blocking(m_i2c_inst, MPU6050_I2C_ADDR, gyro_buf, 2, false);

    // Select correct SSF
    m_gyro_ssf = m_gyro_ssf_values[gyro_fs];
}

void mpu6050_set_accel_fs(mpu6050_accel_fs accel_fs) {
    // [4:3] bits to select the Full Scale
    uint8_t accel_buf[] = {
        MPU6050_REG_ACCEL_CONFIG,
        ((uint8_t) accel_fs << 3)
    };
    i2c_write_blocking(m_i2c_inst, MPU6050_I2C_ADDR, accel_buf, 2, false);

    // Select correct SSF
    m_accel_ssf = m_accel_ssf_values[accel_fs];
}

void mpu6050_read_gyro(float *gyro) {
    // Read data from the sensot
    uint8_t read_buf[6];
    uint8_t reg_data[] = { MPU6050_REG_GYRO_XOUT_H };
    i2c_write_blocking(m_i2c_inst, MPU6050_I2C_ADDR, reg_data, 1, true);
    i2c_read_blocking(m_i2c_inst, MPU6050_I2C_ADDR, read_buf, 6, false);

    // Convert buffer to gyro raw reading
    int16_t gyro_raw[3];
    gyro_raw[0] = read_buf[0] << 8 | read_buf[1];
    gyro_raw[1] = read_buf[2] << 8 | read_buf[3];
    gyro_raw[2] = read_buf[4] << 8 | read_buf[5];

    // Correct values with sensitivity
    gyro[0] = gyro_raw[0] / m_gyro_ssf;
    gyro[1] = gyro_raw[1] / m_gyro_ssf;
    gyro[2] = gyro_raw[2] / m_gyro_ssf;
}

void mpu6050_read_accel(float *accel) {
    // Read data from the sensot
    uint8_t read_buf[6];
    uint8_t reg_data[] = { MPU6050_REG_ACCEL_XOUT_H };
    i2c_write_blocking(m_i2c_inst, MPU6050_I2C_ADDR, reg_data, 1, true);
    i2c_read_blocking(m_i2c_inst, MPU6050_I2C_ADDR, read_buf, 6, false);

    // Convert buffer to gyro raw reading
    int16_t accel_raw[3];
    accel_raw[0] = read_buf[0] << 8 | read_buf[1];
    accel_raw[1] = read_buf[2] << 8 | read_buf[3];
    accel_raw[2] = read_buf[4] << 8 | read_buf[5];

    // Correct values with sensitivity
    accel[0] = accel_raw[0] / m_accel_ssf;
    accel[1] = accel_raw[1] / m_accel_ssf;
    accel[2] = accel_raw[2] / m_accel_ssf;
}
