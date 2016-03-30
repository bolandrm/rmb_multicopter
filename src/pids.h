#ifndef PIDS_H
#define PIDS_H

#include "stdint.h"
#include <Arduino.h>

#define NUM_PIDS 5

#define PID_RATE_X 0
#define PID_RATE_Y 1
#define PID_RATE_Z 2
#define PID_ANGLE_X 3
#define PID_ANGLE_Y 4

typedef struct {
  uint32_t last_compute_time;

  float setpoint;
  float input;
  float output;

  float integrator;
  float last_error;

  float delta;
  float delta1;
  float delta2;

  float p_term;
  float i_term;
  float d_term;
} m_pid_t;

void pids_init();
m_pid_t *pid(int8_t);
void pid_compute(int8_t);
void pids_reset_i();

#endif
