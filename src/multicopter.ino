#define SERIAL_PORT_SPEED 115200

// #include "imu.h"
// #include "debugger.h"
// #include "flight_controller.h"
// #include "serial_commands.h"
// #include "remote_control.h"

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
//  imu_init();
//  fc_init();
//  rc_init();
}

void loop() {
//  while(!imu_read());
//  serial_commands_process();
//  rc_read_values();
//  fc_process();
//  debugger_print();
  Serial.println("ok");
  Serial.println(F_CPU);
  delay(1000);
}
