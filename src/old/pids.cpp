#include "pids.h"

pid_t pids[NUM_PIDS];

pid_t *pid(int8_t pid_number) {
  return &pids[pid_number];
}

void pids_init() {
  pids[PID_RATE_X].kp = 2.0;
  pids[PID_RATE_X].ki = 1.0;
  pids[PID_RATE_X].kd = 0.0;

  pids[PID_RATE_X].i_max = 20.0;
  pids[PID_RATE_X].d_max = 50.0;

  pids[PID_RATE_Y].kp = 2.0;
  pids[PID_RATE_Y].ki = 1.0;
  pids[PID_RATE_Y].kd = 0.0;

  pids[PID_RATE_Y].i_max = 20.0;
  pids[PID_RATE_Y].d_max = 50.0;

  pids[PID_RATE_Z].kp = 1.0;
  pids[PID_RATE_Z].ki = 1.0;

  pids[PID_ANGLE_X].kp = 0.0;
  pids[PID_ANGLE_X].ki = 0.0;

  pids[PID_ANGLE_Y].kp = 4.5;
  pids[PID_ANGLE_Y].ki = 0.0;

  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0.0;
    pids[i].input = 0.0;
    pids[i].last_compute_time = micros();
  }
}

void pid_compute(int8_t pid_number) {
  pid_t *pid = &pids[pid_number];

  float dt = (float) (micros() - pid->last_compute_time) / 1000000.0f;
  pid->last_compute_time = micros();

  float error = pid->input - pid->setpoint;
  pid->integrator += pid->ki * error * dt;

  float d_error = ((error - pid->last_error) / dt) / 100;

  if (pid->i_max > 0.0 && pid->integrator > pid->i_max)
    pid->integrator = pid->i_max;

  pid->p_term = pid->kp * error;
  pid->i_term = pid->integrator;
  pid->d_term = pid->kd * d_error;

  if (pid->d_max > 0.0 && pid->d_term > pid->d_max)
    pid->d_term = pid->d_max;

  pid->last_error = error;
  pid->output = pid->p_term + pid->i_term + pid->d_term;
}

void pids_reset_i() {
  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0;
  }
}
