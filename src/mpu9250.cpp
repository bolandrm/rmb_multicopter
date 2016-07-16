#include "imu.h"
#include "i2c_helpers.h"
#include "mpu9250.h"
#include "utils.h"

static bool data_ready = false;

static void mpu9250_write_reg(uint16_t addr, uint8_t data) {
  i2c_update_register(MPU_I2C_ADDRESS, addr, data);
  delay(1);
}

static uint8_t mpu9250_read_byte(uint16_t addr) {
  i2c_send_byte(MPU_I2C_ADDRESS, addr);
  return i2c_read_byte(MPU_I2C_ADDRESS);
}

static uint16_t mpu9250_read_word(uint16_t addr) {
  i2c_send_byte(MPU_I2C_ADDRESS, addr);
  return i2c_read_word(MPU_I2C_ADDRESS);
}

static int mpu9250_test_connection() {
  return mpu9250_read_byte(MPUREG_WHOAMI) == MPU_WHOAMI_IDENTIFIER;
}

void mpu9250_read_gyro(axis_int32_t *gyro_rates) {
  int16_t gyro_x = mpu9250_read_word(MPUREG_GYRO_XOUT_H);
  int16_t gyro_y = mpu9250_read_word(MPUREG_GYRO_YOUT_H);
  int16_t gyro_z = mpu9250_read_word(MPUREG_GYRO_ZOUT_H);

  gyro_rates->x = gyro_y;
  gyro_rates->y = -1 * gyro_x;
  gyro_rates->z = -1 * gyro_z;
}

void mpu9250_read_accel(axis_int32_t *accel_raws) {
  int16_t accel_x = mpu9250_read_word(MPUREG_ACCEL_XOUT_H);
  int16_t accel_y = mpu9250_read_word(MPUREG_ACCEL_YOUT_H);
  int16_t accel_z = mpu9250_read_word(MPUREG_ACCEL_ZOUT_H);

  accel_raws->x = accel_y;
  accel_raws->y = -1 * accel_x;
  accel_raws->z = accel_z;
}

void calibrate_gyro() {
  axis_int32_t gyro_raws;
  axis_float_t gyro_sums = { 0.0, 0.0, 0.0 };

  delay(5000);
  serial_printlnf("calibrating gyro");

  for(int i = 0; i < 10000; i++) {
    mpu9250_read_gyro(&gyro_raws);
    gyro_sums.x += gyro_raws.x;
    gyro_sums.y += gyro_raws.y;
    gyro_sums.z += gyro_raws.z;
    delay(1);
  }

  for(;;) {
    serial_printlnf("Gyro calibrated:");
    serial_printlnf("x offset: %f", gyro_sums.x / 10000);
    serial_printlnf("y offset: %f", gyro_sums.y / 10000);
    serial_printlnf("z offset: %f", gyro_sums.z / 10000);
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
    mpu9250_read_accel(&accel_raws);
    accel_sums.x += accel_raws.x;
    accel_sums.y += accel_raws.y;
    accel_sums.z += accel_raws.z;
    delay(1);
  }

  for(;;) {
    serial_printlnf("Accel calibrated:");
    serial_printlnf("x offset: %f", accel_sums.x / 10000);
    serial_printlnf("y offset: %f", accel_sums.y / 10000);
    serial_printlnf("z offset: %f", (accel_sums.z / 10000) - MPU_ACCEL_4G_SENS);
    serial_printlnf("--------------");
    delay(5000);
  }
}

void mpu9250_data_ready() {
  data_ready = true;
}

bool mpu9250_new_data_available() {
  bool new_data_available = data_ready;
  data_ready = false;
  return new_data_available;
}

void mpu9250_init() {
  pinMode(MPU_DATA_READY_INTERRUPT_PIN, INPUT);
  attachInterrupt(MPU_DATA_READY_INTERRUPT_PIN, mpu9250_data_ready, RISING);

  mpu9250_write_reg(MPUREG_PWR_MGMT_1, BIT_H_RESET);
  delay(100);  // Startup time delay

  // Wake Up device and select GyroZ clock (better performance)
  mpu9250_write_reg(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
  mpu9250_write_reg(MPUREG_PWR_MGMT_2, 0);

  mpu9250_write_reg(MPUREG_SMPLRT_DIV, 0x00);     // Sample rate = 1kHz
  mpu9250_write_reg(MPUREG_INT_ENABLE, 0x01);     // Enable data ready interrupt

  // FS & DLPF   FS=1000º/s, DLPF = 42Hz (low pass filter)
  mpu9250_write_reg(MPUREG_CONFIG, BITS_DLPF_CFG_188HZ);
  mpu9250_write_reg(MPUREG_GYRO_CONFIG, BITS_FS_1000DPS);  // Gyro scale 1000º/s
  mpu9250_write_reg(MPUREG_ACCEL_CONFIG, 0x08);   // Accel scale +-4g

  if (mpu9250_test_connection()) {
    serial_printlnf("Connected to MPU!");
  } else {
    for(;;) {
      serial_printlnf("Unable to connect to MPU.");
      delay(1000);
    }
  }

  // calibrate_gyro();
  // calibrate_accel();
}
