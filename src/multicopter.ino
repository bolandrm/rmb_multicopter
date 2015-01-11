#define SERIAL_PORT_SPEED 115200

#include "imu.h"

int32_t debug_timer = millis();

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
  imu_init();
}

void loop() {
  while(!imu_read());

  if (millis() - debug_timer > 500) {
    Serial.print("gyro_x: "); Serial.print(imu_rates().x);
    Serial.print("\t gyro_y: "); Serial.print(imu_rates().y);
    Serial.print("\t gyro_z: "); Serial.print(imu_rates().z);
    Serial.println();

    Serial.print("gyro_x_ang: "); Serial.print(imu_gyro_angles().x);
    Serial.print("\t gyro_y_ang: "); Serial.print(imu_gyro_angles().y);
    Serial.println();

    Serial.print("accel_x_raw: "); Serial.print(imu_accel_raws().x);
    Serial.print("\t accel_y_raw: "); Serial.print(imu_accel_raws().y);
    Serial.print("\t accel_z_raw: "); Serial.print(imu_accel_raws().z);
    Serial.println();

    Serial.print("accel_x_angle: "); Serial.print(imu_accel_angles().x);
    Serial.print("\t accel_y_angle: "); Serial.print(imu_accel_angles().y);
    Serial.println();

    Serial.println();

    debug_timer = millis();
  }
}
