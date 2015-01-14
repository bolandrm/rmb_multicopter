#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#define GYRO_X_OFFSET 1.86
#define GYRO_Y_OFFSET 1.49
#define GYRO_Z_OFFSET (-1.16)

#define GYRO_ALPHA 0.8

#define GYRO_PART 0.994
#define ACC_PART (1.0 - GYRO_PART)

typedef struct {
  int16_t x, y, z;
} axis_int16_t;

typedef struct {
  float x, y, z;
} axis_float_t;

void imu_init();
bool imu_read();
axis_float_t imu_rates();
axis_float_t imu_angles();
axis_float_t imu_gyro_angles();
axis_float_t imu_gyro_rates();
axis_float_t imu_accel_raws();
axis_float_t imu_accel_angles();

#endif
