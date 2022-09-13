#include "imu.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// MPU6050 IC2 uses 40kHz and address 0x68 by default
#define MPU6050_FREQ 400000
#define MPU6050_ADDR 0x68

// MPU6050 sensitivity scale factor
static float m_gyro_ssf, m_accl_ssf;
static float m_gyro_ssf_values[] = {131.0f, 65.5f, 32.8f, 16.4f};
static float m_accl_ssf_values[] = {16384.0f, 8192.0f, 4096.0f, 2048.0f};

int imu_init(imu_config config) {
    // selecting sensitivity to be used
    m_gyro_ssf = m_gyro_ssf_values[config.gyro_scale];
    m_accl_ssf = m_accl_ssf_values[config.accl_scale];

    // It will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, MPU6050_FREQ);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // Two byte reset. First byte register, second byte data
    uint8_t reset_buf[] = {0x6B, 0x00};
    i2c_write_blocking(i2c_default, MPU6050_ADDR, reset_buf, 2, false);

    // Config scale, uses bits 3 and 4 for the selection
    uint8_t gyro_byte = ((uint8_t) config.gyro_scale << 3);
    uint8_t accl_byte = ((uint8_t) config.accl_scale << 3);
    uint8_t gyro_buf[] = {0x1B, gyro_byte};
    uint8_t accl_buf[] = {0x1C, accl_byte};
    i2c_write_blocking(i2c_default, MPU6050_ADDR, gyro_buf, 2, false);
    i2c_write_blocking(i2c_default, MPU6050_ADDR, accl_buf, 2, false);

    return 0;
}

void imu_read(float accl[3], float *temp, float gyro[3]) {
    uint8_t buffer[14];
    uint8_t reg = 0x3B;

    // Start reading data from the sensor
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 14, false);

    int16_t accl_raw[3], temp_raw, gyro_raw[3];
    accl_raw[0] = buffer[0]  << 8 | buffer[1];
    accl_raw[1] = buffer[2]  << 8 | buffer[3];
    accl_raw[2] = buffer[4]  << 8 | buffer[5];
    temp_raw    = buffer[6]  << 8 | buffer[7];
    gyro_raw[0] = buffer[8]  << 8 | buffer[9];
    gyro_raw[1] = buffer[10] << 8 | buffer[11];
    gyro_raw[2] = buffer[12] << 8 | buffer[13];

    // Passing data back with the correct scale
    accl[0] = (float) accl_raw[0] / m_accl_ssf;
    accl[1] = (float) accl_raw[1] / m_accl_ssf;
    accl[2] = (float) accl_raw[2] / m_accl_ssf;
    *temp   = (float) temp_raw;
    gyro[0] = (float) gyro_raw[0] / m_gyro_ssf;
    gyro[1] = (float) gyro_raw[1] / m_gyro_ssf;
    gyro[2] = (float) gyro_raw[2] / m_gyro_ssf;
}
