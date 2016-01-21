#define SERIAL_PORT_SPEED 115200

#include "WProgram.h"
#include "debugger.h"
#include "flight_controller.h"
#include "serial_commands.h"

#include "i2c_helpers.h"
extern "C" {
#include "utils.h"
#include "schedule.h"
#include "imu.h"
#include "remote_control.h"
}

void setup() {
  support_printing_floats();
  serial2_begin(SERIAL_PORT_SPEED);
  i2c_begin();
  imu_init();
  rc_init();
  fc_init();
  debugger_leds_init();
}

void loop() {
  if (schedule(TASK_1000HZ)) {
    imu_read_raw_values();

    if (schedule(TASK_50HZ)) {
      rc_read_values();
      serial_commands_process();
    }

    if (schedule(TASK_500HZ)) {
      imu_process_values();
      fc_process();
    }

    if (schedule(TASK_2HZ)) {
      debugger_leds();
    }

    schedule_end();
  }

  debugger_print();
}

int main(void) {
  setup();

  for(;;) {
    loop();
    yield();
  }
}
