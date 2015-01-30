#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#define GYRO_X_OFFSET 0.0
#define GYRO_Y_OFFSET 0.0
#define GYRO_Z_OFFSET 0.0

#define ACCEL_X_OFFSET (0.01)
#define ACCEL_Y_OFFSET (0.03)
#define ACCEL_Z_OFFSET (0.05)

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
axis_float_t imu_accel_filtered();
axis_float_t imu_accel_angles();

#endif
