#include "imu.h"
#include "mpu6050.h"
#include "MedianFilter.h"

static axis_float_t gyro_angles;
static axis_float_t accel_angles;
static axis_float_t accel_filtered;
static axis_float_t rates;
static axis_float_t angles;
static axis_int16_t gyro_raws;
static axis_float_t gyro_sums;
static int16_t gyro_sum_count = 0;
static uint32_t gyro_update_timer = micros();
static uint32_t combination_update_timer = micros();
static axis_int16_t accel_raws;
static MedianFilter accel_x_filter(11, 0);
static MedianFilter accel_y_filter(11, 0);
static MedianFilter accel_z_filter(11, 0);

static uint32_t value_process_timer = micros();
static uint32_t value_process_dt;

void imu_init() {
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
  mpu6050_init();
}

static void reset_gyro_sums() {
  gyro_sums.x = 0.0;
  gyro_sums.y = 0.0;
  gyro_sums.z = 0.0;
  gyro_sum_count = 0;
}

static void read_gyro_raws() {
  mpu6050_read_gyro(&gyro_raws);

  gyro_sums.x += gyro_raws.x;
  gyro_sums.y += gyro_raws.y;
  gyro_sums.z += gyro_raws.z;

  gyro_sum_count += 1;
}

static void read_accel_raws() {
  mpu6050_read_accel(&accel_raws);

  accel_x_filter.in(accel_raws.x);
  accel_y_filter.in(accel_raws.y);
  accel_z_filter.in(accel_raws.z);
}

static void process_gyro() {
  float rate_avg_x = (gyro_sums.x / gyro_sum_count) + GYRO_X_OFFSET;
  float rate_avg_y = (gyro_sums.y / gyro_sum_count) + GYRO_Y_OFFSET;
  float rate_avg_z = (gyro_sums.z / gyro_sum_count) + GYRO_Z_OFFSET;

  rates.x = rate_avg_x / MPU6050_GYRO_1000D_SENS;
  rates.y = rate_avg_y / MPU6050_GYRO_1000D_SENS;
  rates.z = rate_avg_z / MPU6050_GYRO_1000D_SENS;

  reset_gyro_sums();

  // Integration of gyro rates to get the angles for debugging only
  gyro_angles.x += rates.x * (float)(micros() - gyro_update_timer) / 1000000;
  gyro_angles.y += rates.y * (float)(micros() - gyro_update_timer) / 1000000;

  gyro_update_timer = micros();
}

static void process_accel() {
  accel_filtered.x = (float) accel_x_filter.out() / MPU6050_ACCEL_4G_SENS + ACCEL_X_OFFSET;
  accel_filtered.y = (float) accel_y_filter.out() / MPU6050_ACCEL_4G_SENS + ACCEL_Y_OFFSET;
  accel_filtered.z = (float) accel_z_filter.out() / MPU6050_ACCEL_4G_SENS + ACCEL_Z_OFFSET;
}

static void combine() {
  float dt = (float) (micros() - combination_update_timer) / 1000000.0;
  combination_update_timer = micros();

  accel_angles.x = atan2(accel_filtered.y, accel_filtered.z) * RAD_TO_DEG;
  accel_angles.y = atan2(-1 * accel_filtered.x,
    sqrt(accel_filtered.y * accel_filtered.y + accel_filtered.z * accel_filtered.z)
  ) * RAD_TO_DEG;

  angles.x = GYRO_PART * (angles.x + (rates.x * dt)) + ACC_PART * accel_angles.x;
  angles.y = GYRO_PART * (angles.y + (rates.y * dt)) + ACC_PART * accel_angles.y;
}

void imu_read_raw_values() {
  read_gyro_raws();
  read_accel_raws();
}

void imu_process_values() {
  value_process_dt = micros() - value_process_timer;  // for benchmarking
  value_process_timer = micros();

  process_gyro();
  process_accel();
  combine();
}

axis_float_t imu_rates() { return rates; }
axis_float_t imu_angles() { return angles; }
axis_float_t imu_gyro_angles() { return gyro_angles; }
axis_int16_t imu_gyro_raws() { return gyro_raws; }
axis_int16_t imu_accel_raws() { return accel_raws; }
axis_float_t imu_accel_angles() { return accel_angles; }
axis_float_t imu_accel_filtered() { return accel_filtered; }
uint32_t imu_value_process_dt() { return value_process_dt; }
