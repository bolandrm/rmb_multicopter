#include "schedule.h"

static int16_t schedule_counter = 0;

int8_t schedule(uint16_t interval) {
  return schedule_counter % interval == 0;
}

void schedule_end() {
  if (schedule_counter == 1000) {
    schedule_counter = 0;
  } else {
    schedule_counter++;
  }
}
