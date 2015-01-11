#define SERIAL_PORT_SPEED 115200

#include "imu.h"
#include "debugger.h"

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
  imu_init();
}

void loop() {
  while(!imu_read());
  debugger_print();
}
