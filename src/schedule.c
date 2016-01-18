#include "schedule.h"

static int32_t schedule_timer = 0;
static int16_t schedule_counter = 0;

static int8_t check_loop_start() {
  if (micros() - schedule_timer > 1000) {
    schedule_timer = micros();
    schedule_counter++;
    return 1;
  } else {
    return 0;
  }
}

int8_t schedule(uint16_t interval) {
  if (interval == TASK_1000HZ) {
    return check_loop_start();
  } else {
    return schedule_counter % interval == 0;
  }
}

void schedule_end() {
  if (schedule_counter == 1000) {
    schedule_counter = 0;
  }
}
