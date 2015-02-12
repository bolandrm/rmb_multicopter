#define SERIAL_PORT_SPEED 115200

#define TASK_500HZ 2
#define TASK_250HZ 4
#define TASK_50HZ  20

#include "imu.h"
#include "debugger.h"
// #include "flight_controller.h"
// #include "serial_commands.h"
// #include "remote_control.h"

int32_t schedule_timer = micros();
int16_t schedule_counter = 0;

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
  imu_init();
//  fc_init();
//  rc_init();
}

void loop() {
  if (micros() - schedule_timer > 1000) {
    schedule_timer = micros();
    schedule_counter++;

    imu_read_raw_values();

    if (schedule_counter % TASK_50HZ == 0) {

    }

    if (schedule_counter % TASK_250HZ == 0) {
    }

    if (schedule_counter % TASK_500HZ == 0) {
      imu_process_values();
    }

    if (schedule_counter == 1000) schedule_counter = 0;
  }

//  while(!imu_read());
  // imu_read();
//  serial_commands_process();
//  rc_read_values();
//  fc_process();
  debugger_print();
  //delay(1000);
}
