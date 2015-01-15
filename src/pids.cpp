#include "pids.h"

pid_t pids[NUM_PIDS];

pid_t pid(int8_t pid_number) {
  return pids[pid_number];
}

void pids_init() {
  pids[PID_RATE_X].kp = 1.0;
  pids[PID_RATE_X].ki = 0.5;

  pids[PID_RATE_Y].kp = 1.0;
  pids[PID_RATE_Y].ki = 0.5;

  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0;
    pids[i].last_compute_time = micros();
  }
}

void pid_compute(int8_t pid_number) {
  pid_t pid = pids[pid_number];

  float dt = (float) (micros() - pid.last_compute_time) / 1000000.0f;
  pid.last_compute_time = micros();

  float error = pid.input - pid.setpoint;
  pid.integrator += pid.ki * error * dt;

  pid.output = pid.kp * error + pid.ki;
}

void pids_reset_i() {
  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0;
  }
}
