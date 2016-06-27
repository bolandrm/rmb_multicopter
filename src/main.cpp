#define SERIAL_PORT_SPEED 57600

#include "Arduino.h"
#include "i2c_helpers.h"
#include "flight_controller.h"
#include "debugger.h"
#include "schedule.h"
#include "imu.h"
#include "remote_control.h"
#include "serial_commands.h"
#include "config.h"
#include "watchdog.h"
#include "battery_monitor.h"

#include "utils.h"

void setup() {
  serial_begin(BAUD2DIV2(SERIAL_PORT_SPEED));
  serial2_begin(BAUD2DIV2(SERIAL_PORT_SPEED));
  i2c_begin();
  config_init();
  imu_init();
  rc_init();
  fc_init();
  debugger_leds_init();
  battery_monitor_init();
  watchdog_enable();
}

void loop() {
  if (imu_new_data_available()) {
    imu_read_gyro_raws();
    imu_read_accel_raws();
    imu_process_values();
    fc_process();

    if (schedule(TASK_100HZ)) {
      serial_commands_process();
    }

    if (schedule(TASK_50HZ)) {
      rc_read_values();
    }

    if (schedule(TASK_2HZ)) {
      debugger_leds();
      debugger_print();
      battery_monitor_update();
    }

    schedule_end();
    watchdog_reset();
  }
}

int main(void) {
  setup();

  for(;;) {
    loop();
    yield();
  }
}
