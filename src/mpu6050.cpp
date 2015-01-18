#include "mpu6050.h"

void mpu6050_write_reg(int addr, uint8_t data) {
  i2c_update_register(MPU6050_I2C_ADDRESS, addr, data);
  delay(1);
}

uint8_t mpu6050_read_byte(int addr) {
  i2c_send_byte(MPU6050_I2C_ADDRESS, addr);
  return i2c_read_byte(MPU6050_I2C_ADDRESS);
}

uint16_t mpu6050_read_word(int addr) {
  i2c_send_byte(MPU6050_I2C_ADDRESS, addr);
  return i2c_read_word(MPU6050_I2C_ADDRESS);
}

bool mpu6050_test_connection() {
  return mpu6050_read_byte(MPUREG_WHOAMI) == 0x68;
}

void mpu6050_read_gyro(axis_float_t *gyro_rates) {
  int16_t gyro_x = mpu6050_read_word(MPUREG_GYRO_XOUT_H);
  int16_t gyro_y = mpu6050_read_word(MPUREG_GYRO_YOUT_H);
  int16_t gyro_z = mpu6050_read_word(MPUREG_GYRO_ZOUT_H);

  gyro_rates->x = gyro_x / MPU6050_GYRO_500D_SENS;
  gyro_rates->y = gyro_y / MPU6050_GYRO_500D_SENS;
  gyro_rates->z = gyro_z / MPU6050_GYRO_500D_SENS;
}

void mpu6050_read_accel(axis_float_t *accel_raws) {
  int16_t accel_x = mpu6050_read_word(MPUREG_ACCEL_XOUT_H);
  int16_t accel_y = mpu6050_read_word(MPUREG_ACCEL_YOUT_H);
  int16_t accel_z = mpu6050_read_word(MPUREG_ACCEL_ZOUT_H);

  accel_raws->x = accel_x / MPU6050_ACCEL_4G_SENS;
  accel_raws->y = accel_y / MPU6050_ACCEL_4G_SENS;
  accel_raws->z = accel_z / MPU6050_ACCEL_4G_SENS;
}

void mpu6050_init() {
  mpu6050_write_reg(MPUREG_PWR_MGMT_1, BIT_H_RESET);
  delay(100);  // Startup time delay

  // Wake Up device and select GyroZ clock (better performance)
  mpu6050_write_reg(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
  mpu6050_write_reg(MPUREG_PWR_MGMT_2, 0);

  // SAMPLE RATE
  mpu6050_write_reg(MPUREG_SMPLRT_DIV, 0x00);     // Sample rate = 1kHz

  // FS & DLPF   FS=500º/s, DLPF = 256Hz (low pass filter)
  mpu6050_write_reg(MPUREG_CONFIG, BITS_DLPF_CFG_256HZ_NOLPF2);
  mpu6050_write_reg(MPUREG_GYRO_CONFIG, BITS_FS_500DPS);  // Gyro scale 1000º/s
  mpu6050_write_reg(MPUREG_ACCEL_CONFIG, 0x08);   // Accel scale +-4g

  if (mpu6050_test_connection()) {
    Serial.println("Connected to MPU6050!");
  } else {
    Serial.println("Unable to connect to MPU6050.");
    for(;;);
  }
}
