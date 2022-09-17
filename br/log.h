#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>

// Normal log levels
#define LEVEL_NONE  0x00
#define LEVEL_ERROR 0x01
#define LEVEL_DEBUG 0x02

// Serial Plot levels
#define LEVEL_SP             0x03
#define LEVEL_SP_RAW_SENSORS 0x04
#define LEVEL_SP_IMU         0x05
#define LEVEL_SP_PID         0x06

#define DEBUG_TAG "DEBUG"
#define INFO_TAG  "INFO"
#define ERROR_TAG "ERROR"

#ifndef LOG_LEVEL
#define LOG_LEVEL LEVEL_DEBUG
#endif

#define LOG_FMT             "%-5s | %s::%d -> "
#define LOG_ARGS(LOG_TAG)   LOG_TAG, __FUNCTION__, __LINE__

#define NEW_LINE "\n"

#if LOG_LEVEL >= LEVEL_ERROR && LOG_LEVEL < LEVEL_SP
#define LOG_ERROR(message, ...) \
    printf(LOG_FMT message NEW_LINE, LOG_ARGS(ERROR_TAG), ##__VA_ARGS__)
#else
#define LOG_ERROR(message, ...)
#endif

#if LOG_LEVEL >= LEVEL_DEBUG && LOG_LEVEL < LEVEL_SP
#define LOG_DEBUG(message, ...) \
    printf(LOG_FMT message NEW_LINE, LOG_ARGS(DEBUG_TAG), ##__VA_ARGS__)
#else
#define LOG_DEBUG(message, ...)
#endif

#if LOG_LEVEL == LEVEL_SP_RAW_SENSORS
#define SP_RAW_SENSORS(gyro, accel) \
    printf("%f,%f,%f,%f,%f,%f\n", gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2])
#else
#define SP_RAW_SENSORS(gyro, accel)
#endif

#endif // LOG_H
