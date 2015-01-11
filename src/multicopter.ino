#define SERIAL_PORT_SPEED 115200

#include "imu.h"

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
  init_sensors();
}

void loop() {
  read_sensors();
  delay(1000);
}
