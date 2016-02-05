#include "pids.h"
#include "utils.h"
#include "config.h"

m_pid_t pids[NUM_PIDS];

m_pid_t *pid(int8_t pid_number) {
  return &pids[pid_number];
}

void pids_init() {
  pids[PID_RATE_X].kp = CONFIG.data.pid_rate_xy.kp;
  pids[PID_RATE_X].ki = CONFIG.data.pid_rate_xy.ki;
  pids[PID_RATE_X].i_max = CONFIG.data.pid_rate_xy.i_max;

  pids[PID_RATE_Y].kp = CONFIG.data.pid_rate_xy.kp;
  pids[PID_RATE_Y].ki = CONFIG.data.pid_rate_xy.ki;
  pids[PID_RATE_Y].i_max = CONFIG.data.pid_rate_xy.i_max;

  pids[PID_RATE_Z].kp = CONFIG.data.pid_rate_z.kp;
  pids[PID_RATE_Z].ki = CONFIG.data.pid_rate_z.ki;
  pids[PID_RATE_Z].i_max = CONFIG.data.pid_rate_z.i_max;

  pids[PID_ANGLE_X].kp = CONFIG.data.pid_angle_xy.kp;
  pids[PID_ANGLE_X].ki = CONFIG.data.pid_angle_xy.ki;
  pids[PID_ANGLE_X].i_max = CONFIG.data.pid_angle_xy.i_max;

  pids[PID_ANGLE_Y].kp = CONFIG.data.pid_angle_xy.kp;
  pids[PID_ANGLE_Y].ki = CONFIG.data.pid_angle_xy.ki;
  pids[PID_ANGLE_Y].i_max = CONFIG.data.pid_angle_xy.i_max;

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

  pid->p_term = pid->kp * error;
  pid->i_term = pid->integrator;

  pid->last_input = pid->input;
  pid->output = pid->p_term + pid->i_term;
}

void pids_reset_i() {
  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0;
  }
}
