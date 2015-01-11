#include <Arduino.h>
#include "imu.h"

int32_t debug_timer = millis();

void debugger_print() {
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
