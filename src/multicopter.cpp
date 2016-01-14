#define SERIAL_PORT_SPEED 115200

#include "schedule.h"
#include "imu.h"
#include "debugger.h"
#include "flight_controller.h"
#include "serial_commands.h"
#include "remote_control.h"

extern "C" {
  void setup() {
    //Serial.begin(SERIAL_PORT_SPEED);
    //Serial2.begin(SERIAL_PORT_SPEED);
    //imu_init();
    //rc_init();
    //fc_init();
    //debugger_leds_init();

    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
  }
};

void loop() {
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);

//digitalWrite(14, HIGH);
  // run_loop();

  //if (schedule(TASK_1000HZ)) {
  //  imu_read_raw_values();

  //  if (schedule(TASK_50HZ)) {
  //    rc_read_values();
  //    serial_commands_process();
  //  }

  //  if (schedule(TASK_500HZ)) {
  //    imu_process_values();
  //    fc_process();
  //  }

  //  if (schedule(TASK_2HZ)) {
  //    debugger_leds();
  //  }

  //  schedule_end();
  //}

  //debugger_print();
}
