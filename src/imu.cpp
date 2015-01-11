#include "imu.h"
#include "mpu6050.h"

static axis_int16_t gyro_raws;
static axis_float_t gyro_rates;
static axis_float_t gyro_angles;
static axis_float_t rates;
static uint32_t gyro_update_timer = micros();

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
  gyro_raws.x = mpu6050_read_word(MPUREG_GYRO_XOUT_H);
  gyro_raws.y = mpu6050_read_word(MPUREG_GYRO_YOUT_H);
  gyro_raws.z = mpu6050_read_word(MPUREG_GYRO_ZOUT_H);

  gyro_rates.x = gyro_raws.x / MPU6050_GYRO_1000D_SENS + GYRO_X_OFFSET;
  gyro_rates.y = gyro_raws.y / MPU6050_GYRO_1000D_SENS + GYRO_Y_OFFSET;
  gyro_rates.z = gyro_raws.z / MPU6050_GYRO_1000D_SENS + GYRO_Z_OFFSET;

  rates.x = gyro_rates.x;
  rates.y = gyro_rates.y;
  rates.z = gyro_rates.z;

  //Integration of gyro rates to get the angles
  //gyro_angle.x += x_rate * (float)(micros() - gyro_update_timer) / 1000000;
  //gyro_angle.y += y_rate * (float)(micros() - gyro_update_timer) / 1000000;
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

axis_float_t imu_gyro_rates() {
  return gyro_rates;
}
