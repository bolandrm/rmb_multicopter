#ifndef schedule_h
#define schedule_h

#include <Arduino.h>

#define TASK_1000HZ 0
#define TASK_500HZ  2
#define TASK_250HZ  4
#define TASK_50HZ   20

bool schedule(uint16_t interval);
void schedule_end();

#endif
