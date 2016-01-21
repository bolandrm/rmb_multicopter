#include "pids.h"
#include "utils.h"

m_pid_t pids[NUM_PIDS];

m_pid_t *pid(int8_t pid_number) {
  return &pids[pid_number];
}

void pids_init() {
  pids[PID_RATE_X].kp = 2.25;
  pids[PID_RATE_X].ki = 2.25;
  pids[PID_RATE_X].kd = 0.0;
  pids[PID_RATE_X].i_max = 20.0;

  pids[PID_RATE_Y].kp = 2.25;
  pids[PID_RATE_Y].ki = 2.25;
  pids[PID_RATE_Y].kd = 0.0;
  pids[PID_RATE_Y].i_max = 20.0;

  pids[PID_RATE_Z].kp = 2.0;
  pids[PID_RATE_Z].ki = 4.0;
  pids[PID_RATE_Z].kd = 0.0;
  pids[PID_RATE_Z].i_max = 20.0;

  pids[PID_ANGLE_X].kp = 2.00;
  pids[PID_ANGLE_X].ki = 1.90;
  pids[PID_ANGLE_X].kd = 0.0;
  pids[PID_ANGLE_X].i_max = 20.0;

  pids[PID_ANGLE_Y].kp = 2.00;
  pids[PID_ANGLE_Y].ki = 1.90;
  pids[PID_ANGLE_Y].kd = 0.0;
  pids[PID_ANGLE_Y].i_max = 20.0;

  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0.0;
    pids[i].input = 0.0;
    pids[i].last_compute_time = micros();
  }
}

void pid_compute(int8_t pid_number) {
  m_pid_t *pid = &pids[pid_number];

  float dt = (float) (micros() - pid->last_compute_time) / 1000000.0f;
  pid->last_compute_time = micros();

  float error = pid->setpoint - pid->input;

  pid->integrator += pid->ki * error * dt;
  pid->integrator = constrain_c(pid->integrator, -pid->i_max, pid->i_max);

  float d_error = (pid->input - pid->last_input) / dt;

  pid->p_term = pid->kp * error;
  pid->i_term = pid->integrator;
  pid->d_term = pid->kd * d_error;

  pid->last_input = pid->input;
  pid->output = pid->p_term + pid->i_term - pid->d_term;
}

void pids_reset_i() {
  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0;
  }
}
