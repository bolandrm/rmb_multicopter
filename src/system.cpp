#include <Arduino.h>
#include "imu.h"
#include "i2c_helpers.h"
#include "mpu6050.h"

extern "C" {
  void _yield() {
    yield();
  }

  int _getpid(void) {
    return 1;
  }

  int _kill(int pid, int sig) {
    return -1;
  }

  // #include "schedule.h"
  // #include "debugger.h"
  // #include "flight_controller.h"
  // #include "serial_commands.h"
  // #include "remote_control.h"

  void setup() {
    //Serial.begin(SERIAL_PORT_SPEED);
    //Serial2.begin(SERIAL_PORT_SPEED);
    // imu_init();
    //rc_init();
    //fc_init();
    //debugger_leds_init();

    //Serial.begin(SERIAL_PORT_SPEED);
  }

  void _loop() {
    //digitalWrite(14, LOW);
    //digitalWrite(15, LOW);

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
}
