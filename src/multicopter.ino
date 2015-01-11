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
    Serial.print("gyro_x: "); Serial.print(imu_gyro_rate().x);
    Serial.print("\t gyro_y: "); Serial.print(imu_gyro_rate().y);
    Serial.print("\t gyro_z: "); Serial.print(imu_gyro_rate().z);
    Serial.println();

    debug_timer = millis();
  }
}
