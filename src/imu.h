#pragma once

#include "types.h"
#include "mpu9250.h"

#define GYRO_X_OFFSET (-103.87)
#define GYRO_Y_OFFSET (-34.90)
#define GYRO_Z_OFFSET (-47.83)

#define ACCEL_X_OFFSET (-80.05)
#define ACCEL_Y_OFFSET (40.97)
#define ACCEL_Z_OFFSET (-100.12)

#define GYRO_PART 0.994
#define ACC_PART (1.0 - GYRO_PART)

#define ACCEL_SENS  MPU_ACCEL_4G_SENS
#define GYRO_SENS   MPU_GYRO_1000D_SENS

void imu_init();
void imu_read_gyro_raws();
void imu_read_accel_raws();
void imu_process_values();

axis_float_t imu_rates();
axis_float_t imu_angles();
axis_float_t imu_gyro_angles();
axis_int32_t imu_gyro_raws();
axis_int32_t imu_accel_raws();
axis_float_t imu_accel_filtered();
axis_float_t imu_accel_angles();
extern uint32_t imu_value_process_dt;
extern uint32_t imu_max_value_process_dt;
float imu_gyro_max_value();
float imu_accel_max_value();
bool imu_new_data_available();
