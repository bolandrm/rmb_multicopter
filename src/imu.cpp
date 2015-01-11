#include "imu.h"
#include "i2c_helpers.h"

static gyro_raw_t gyro_raw;
static gyro_rate_t gyro_rate;
static uint32_t gyro_update_timer = micros();

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

void imu_init() {
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

void update_gyro() {
  gyro_raw.x = mpu6050_read_word(MPUREG_GYRO_XOUT_H);
  gyro_raw.y = mpu6050_read_word(MPUREG_GYRO_YOUT_H);
  gyro_raw.z = mpu6050_read_word(MPUREG_GYRO_ZOUT_H);

  gyro_rate.x = gyro_raw.x / MPU6050_GYRO_1000D_SENS + GYRO_X_OFFSET;
  gyro_rate.y = gyro_raw.y / MPU6050_GYRO_1000D_SENS + GYRO_Y_OFFSET;
  gyro_rate.z = gyro_raw.z / MPU6050_GYRO_1000D_SENS + GYRO_Z_OFFSET;
}

bool imu_read() {
  bool updated = false;

  // if ((millis() - accel_update_timer) > 20) {    // ~50 hz
  //   update_accel();
  //   accel_update_timer = millis();
  //   updated = true;
  // }

  if ((micros() - gyro_update_timer) > 1300) {   // ~800 Hz
    update_gyro();
    gyro_update_timer = micros();
    updated = true;
  }

  if (updated) {
    //combine();

    //x_angle = comp_angle_x - 180;
    //y_angle = comp_angle_y - 180;
  }

  return updated;
}


gyro_rate_t imu_gyro_rate() {
  return gyro_rate;
}
