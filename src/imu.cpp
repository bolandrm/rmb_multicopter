#include "imu.h"
#include "i2c_helpers.h"

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

void init_sensors() {
  Wire.begin();
  TWBR = 10;  // setup i2c to run at 444 kHz

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
  mpu6050_write_reg(MPUREG_ACCEL_CONFIG, 0x08);   // Accel scale +-4g (4096LSB/g)

  if (mpu6050_test_connection()) {
    Serial.println("Connected to MPU6050!");
  } else {
    Serial.println("Unable to connect to MPU6050.");
    for(;;) {}
  }
}

void read_sensors() {
  int16_t gyro_x_in = mpu6050_read_word(MPUREG_GYRO_XOUT_H);
  int16_t gyro_y_in = mpu6050_read_word(MPUREG_GYRO_YOUT_H);
  int16_t gyro_z_in = mpu6050_read_word(MPUREG_GYRO_ZOUT_H);
  Serial.print("gyro_x: "); Serial.print(gyro_x_in/MPU6050_GYRO_1000D_SENS);
  Serial.print("\t gyro_y: "); Serial.print(gyro_y_in/MPU6050_GYRO_1000D_SENS);
  Serial.print("\t gyro_z: "); Serial.print(gyro_z_in/MPU6050_GYRO_1000D_SENS);
  Serial.println();
  delay(100);
}
