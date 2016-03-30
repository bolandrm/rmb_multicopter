#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>

#define TASK_1000HZ 0
#define TASK_500HZ  2
#define TASK_250HZ  4
#define TASK_100HZ  10
#define TASK_50HZ   20
#define TASK_2HZ   500

int8_t schedule(uint16_t interval);
void schedule_end();

#endif
