#include "imu.h"
#include "i2c_helpers.h"
#include "mpu6050.h"
#include "utils.h"

static void mpu6050_write_reg(uint16_t addr, uint8_t data) {
  i2c_update_register(MPU6050_I2C_ADDRESS, addr, data);
  delay(1);
}

static uint8_t mpu6050_read_byte(uint16_t addr) {
  i2c_send_byte(MPU6050_I2C_ADDRESS, addr);
  return i2c_read_byte(MPU6050_I2C_ADDRESS);
}

static uint16_t mpu6050_read_word(uint16_t addr) {
  i2c_send_byte(MPU6050_I2C_ADDRESS, addr);
  return i2c_read_word(MPU6050_I2C_ADDRESS);
}

static int mpu6050_test_connection() {
  return mpu6050_read_byte(MPUREG_WHOAMI) == 0x68;
}

void mpu6050_read_gyro(axis_int32_t *gyro_rates) {
  int16_t gyro_x = mpu6050_read_word(MPUREG_GYRO_XOUT_H);
  int16_t gyro_y = mpu6050_read_word(MPUREG_GYRO_YOUT_H);
  int16_t gyro_z = mpu6050_read_word(MPUREG_GYRO_ZOUT_H);

  gyro_rates->x = gyro_x;
  gyro_rates->y = gyro_y;
  gyro_rates->z = -1 * gyro_z;
}

void mpu6050_read_accel(axis_int32_t *accel_raws) {
  int16_t accel_x = mpu6050_read_word(MPUREG_ACCEL_XOUT_H);
  int16_t accel_y = mpu6050_read_word(MPUREG_ACCEL_YOUT_H);
  int16_t accel_z = mpu6050_read_word(MPUREG_ACCEL_ZOUT_H);

  accel_raws->x = accel_x;
  accel_raws->y = accel_y;
  accel_raws->z = accel_z;
}

void calibrate_gyro() {
  axis_int32_t gyro_raws;
  axis_float_t gyro_sums = { 0.0, 0.0, 0.0 };

  delay(5000);
  serial_printlnf("calibrating gyro");

  for(int i = 0; i < 10000; i++) {
    mpu6050_read_gyro(&gyro_raws);
    gyro_sums.x += gyro_raws.x;
    gyro_sums.y += gyro_raws.y;
    gyro_sums.z += gyro_raws.z;
    delay(1);
  }

  for(;;) {
    serial_printlnf("Gyro calibrated:");
    serial_printlnf("x avg: %f", gyro_sums.x / 10000);
    serial_printlnf("y avg: %f", gyro_sums.y / 10000);
    serial_printlnf("z avg: %f", gyro_sums.z / 10000);
    serial_printlnf("--------------");
    delay(5000);
  }
}

void calibrate_accel() {
  axis_int32_t accel_raws;
  axis_float_t accel_sums = { 0.0, 0.0, 0.0 };

  delay(5000);
  serial_printlnf("calibrating accel");

  for(int i = 0; i < 10000; i++) {
    mpu6050_read_accel(&accel_raws);
    accel_sums.x += accel_raws.x;
    accel_sums.y += accel_raws.y;
    accel_sums.z += accel_raws.z;
    delay(1);
  }

  for(;;) {
    serial_printlnf("Accel calibrated:");
    serial_printlnf("x avg: %f", accel_sums.x / 10000);
    serial_printlnf("y avg: %f", accel_sums.y / 10000);
    serial_printlnf("z avg: %f", accel_sums.z / 10000);
    serial_printlnf("--------------");
    delay(5000);
  }
}

void mpu6050_init() {
  mpu6050_write_reg(MPUREG_PWR_MGMT_1, BIT_H_RESET);
  delay(100);  // Startup time delay

  // Wake Up device and select GyroZ clock (better performance)
  mpu6050_write_reg(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
  mpu6050_write_reg(MPUREG_PWR_MGMT_2, 0);

  // SAMPLE RATE
  mpu6050_write_reg(MPUREG_SMPLRT_DIV, 0x00);     // Sample rate = 1kHz

  // FS & DLPF   FS=1000º/s, DLPF = 42Hz (low pass filter)
  mpu6050_write_reg(MPUREG_CONFIG, BITS_DLPF_CFG_42HZ);
  mpu6050_write_reg(MPUREG_GYRO_CONFIG, BITS_FS_1000DPS);  // Gyro scale 1000º/s
  mpu6050_write_reg(MPUREG_ACCEL_CONFIG, 0x08);   // Accel scale +-4g

  if (mpu6050_test_connection()) {
    serial_printlnf("Connected to MPU6050!");
  } else {
    for(;;) {
      serial_printlnf("Unable to connect to MPU6050.");
      delay(1000);
    }
  }

  // calibrate_gyro();
  // calibrate_accel();
}
