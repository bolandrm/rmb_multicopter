#include "imu.h"
#include "MedianFilter.h"
#include "utils.h"

static axis_float_t gyro_angles;
static axis_float_t accel_angles;
static axis_float_t accel_filtered;
static axis_float_t rates;
static axis_float_t angles;
static axis_int32_t gyro_raws;
static uint32_t gyro_update_timer = 0;
static uint32_t combination_update_timer = 0;
static axis_int32_t accel_raws;
static median_filter_t accel_x_filter;
static median_filter_t accel_y_filter;
static median_filter_t accel_z_filter;

static int32_t accel_max_value = 0;
static int32_t gyro_max_value = 0;

static uint32_t value_process_timer = 0;
static uint32_t value_process_dt;

static void record_max_gyro_value();
static void record_max_accel_value();

void imu_init() {
  accel_x_filter = median_filter_new(11, 0);
  accel_y_filter = median_filter_new(11, 0);
  accel_z_filter = median_filter_new(11, 0);

  delay(300);

  mpu6050_init();

  delay(300);
}

void imu_read_gyro_raws() {
  mpu6050_read_gyro(&gyro_raws);
  record_max_gyro_value();
}

void imu_read_accel_raws() {
  mpu6050_read_accel(&accel_raws);
  record_max_accel_value();

  median_filter_in(accel_x_filter, accel_raws.x);
  median_filter_in(accel_y_filter, accel_raws.y);
  median_filter_in(accel_z_filter, accel_raws.z);
}

static void process_gyro() {
  rates.x = (gyro_raws.x - GYRO_X_OFFSET) / GYRO_SENS;
  rates.y = (gyro_raws.y - GYRO_Y_OFFSET) / GYRO_SENS;
  rates.z = (gyro_raws.z - GYRO_Z_OFFSET) / GYRO_SENS;

  // Integration of gyro rates to get the angles for debugging only
  gyro_angles.x += rates.x * (float)(micros() - gyro_update_timer) / 1000000;
  gyro_angles.y += rates.y * (float)(micros() - gyro_update_timer) / 1000000;

  gyro_update_timer = micros();
}

static void process_accel() {
  accel_filtered.x = (float) (median_filter_out(accel_x_filter) - ACCEL_X_OFFSET) / ACCEL_SENS;
  accel_filtered.y = (float) (median_filter_out(accel_y_filter) - ACCEL_Y_OFFSET) / ACCEL_SENS;
  accel_filtered.z = (float) (median_filter_out(accel_z_filter) - ACCEL_Z_OFFSET) / ACCEL_SENS;
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

void imu_process_values() {
  value_process_dt = micros() - value_process_timer;  // for benchmarking
  value_process_timer = micros();

  process_gyro();
  process_accel();
  combine();
}

static void record_max_gyro_value() {
  if (abs(gyro_raws.x) > gyro_max_value) { gyro_max_value = abs(gyro_raws.x); }
  if (abs(gyro_raws.y) > gyro_max_value) { gyro_max_value = abs(gyro_raws.y); }
  if (abs(gyro_raws.z) > gyro_max_value) { gyro_max_value = abs(gyro_raws.z); }
}

static void record_max_accel_value() {
  if (abs(accel_raws.x) > accel_max_value) { accel_max_value = abs(accel_raws.x); }
  if (abs(accel_raws.y) > accel_max_value) { accel_max_value = abs(accel_raws.y); }
  if (abs(accel_raws.z) > accel_max_value) { accel_max_value = abs(accel_raws.z); }
}

axis_float_t imu_rates() { return rates; }
axis_float_t imu_angles() { return angles; }
axis_float_t imu_gyro_angles() { return gyro_angles; }
axis_int32_t imu_gyro_raws() { return gyro_raws; }
axis_int32_t imu_accel_raws() { return accel_raws; }
axis_float_t imu_accel_angles() { return accel_angles; }
axis_float_t imu_accel_filtered() { return accel_filtered; }
uint32_t imu_value_process_dt() { return value_process_dt; }
float imu_gyro_max_value() { return gyro_max_value / GYRO_SENS; }
float imu_accel_max_value() { return accel_max_value / ACCEL_SENS; }
