#define SERIAL_PORT_SPEED 115200

#include "WProgram.h"
#include "debugger.h"
#include "flight_controller.h"
#include "serial_commands.h"
#include "remote_control.h"
#include <i2c_t3.h>

extern "C" {
  #include "schedule.h"
  #include "imu.h"
}

void setup() {
  serial2_begin(SERIAL_PORT_SPEED);
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
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
	while (1) {
		loop();
		yield();
  }
}
