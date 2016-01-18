#define SERIAL_PORT_SPEED 115200

#include "WProgram.h"
#include "imu.h"
#include "debugger.h"
#include "flight_controller.h"
#include "serial_commands.h"
#include "remote_control.h"

//#include "schedule.h"
#define TASK_1000HZ 0
#define TASK_500HZ  2
#define TASK_250HZ  4
#define TASK_50HZ   20
#define TASK_2HZ   500
extern "C" {
  int8_t schedule(uint16_t interval);
  void schedule_end();
}
//#include "schedule.h"

void setup() {
  serial2_begin(SERIAL_PORT_SPEED);
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
