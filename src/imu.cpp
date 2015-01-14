#include "imu.h"
#include "mpu6050.h"

static axis_float_t gyro_rates;
static axis_float_t gyro_angles;
static axis_float_t accel_raws;
static axis_float_t accel_angles;
static axis_float_t rates;
static axis_float_t angles;
static uint32_t gyro_update_timer = micros();
static uint32_t accel_update_timer = millis();
static uint32_t combination_update_timer = micros();

void imu_init() {
  Wire.begin();
  TWBR = 10;  // setup i2c to run at 444 kHz
  mpu6050_init();
}

void update_gyro() {
  mpu6050_read_gyro(&gyro_rates);

  rates.x = gyro_rates.x + GYRO_X_OFFSET;
  rates.y = gyro_rates.y + GYRO_Y_OFFSET;
  rates.z = gyro_rates.z + GYRO_Z_OFFSET;

  //Integration of gyro rates to get the angles
  // gyro_angles.x += rates.x * (float)(micros() - gyro_update_timer) / 1000000;
  // gyro_angles.y += rates.y * (float)(micros() - gyro_update_timer) / 1000000;
}

void update_accel() {
  mpu6050_read_accel(&accel_raws);

  // This is the old method
  // accel_angles.x = (atan2(accel_raws.x, accel_raws.z)) * RAD_TO_DEG;
  // accel_angles.y = (atan2(accel_raws.y, accel_raws.z)) * RAD_TO_DEG;

  accel_angles.x = atan2(accel_raws.y, sqrt(
    accel_raws.x * accel_raws.x + accel_raws.z * accel_raws.z
  )) * RAD_TO_DEG;

  accel_angles.y = atan2(accel_raws.x, sqrt(
    accel_raws.y * accel_raws.y + accel_raws.z * accel_raws.z
  )) * RAD_TO_DEG;
}

void combine() {
  //Angle calculation through Complementary filter

  float dt = (float)(micros() - combination_update_timer) / 1000000.0;

  angles.x = GYRO_PART * (angles.x + (rates.x * dt)) + ACC_PART * accel_angles.x;
  angles.y = GYRO_PART * (angles.y + (rates.y * dt)) + ACC_PART * accel_angles.y;

  combination_update_timer = micros();
}

bool imu_read() {
  bool updated = false;

  // if ((millis() - accel_update_timer) > 20) {    // ~50 hz
  //   update_accel();
  //   accel_update_timer = millis();
  //   updated = true;
  // }

  if ((micros() - gyro_update_timer) > 2000) {   // ~500 Hz
    update_gyro();
    gyro_update_timer = micros();
    updated = true;
  }

  // if (updated) {
  //   combine();
  // }

  return updated;
}

axis_float_t imu_rates() { return rates; }
axis_float_t imu_angles() { return angles; }
axis_float_t imu_gyro_angles() { return gyro_angles; }
axis_float_t imu_gyro_rates() { return gyro_rates; }
axis_float_t imu_accel_raws() { return accel_raws; }
axis_float_t imu_accel_angles() { return accel_angles; }
