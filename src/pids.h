#ifndef PIDS_H
#define PIDS_H

#include <Arduino.h>

#define NUM_PIDS 2

#define PID_RATE_X 0
#define PID_RATE_Y 1

typedef struct {
  float kp;
  float ki;
  uint32_t last_compute_time;
  float setpoint;
  float input;
  float integrator;
  float output;
} pid_t;

void pids_init();
pid_t pid(int8_t);
void pid_compute(int8_t);

#endif
