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
