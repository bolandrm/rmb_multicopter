#include "pids.h"
#include "utils.h"
#include "config.h"

m_pid_t pids[NUM_PIDS];

m_pid_t *pid(int8_t pid_number) {
  return &pids[pid_number];
}

void pids_init() {
  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0.0;
    pids[i].input = 0.0;
    pids[i].last_compute_time = micros();
  }
}

void pid_compute(int8_t pid_number) {
  m_pid_t *pid = &pids[pid_number];

  float kp = CONFIG.data.pids[pid_number].kp;
  float ki = CONFIG.data.pids[pid_number].ki;
  float i_max = CONFIG.data.pids[pid_number].i_max;
  float kd = CONFIG.data.pids[pid_number].kd;

  float dt = (float) (micros() - pid->last_compute_time) / 1000000.0f;
  pid->last_compute_time = micros();

  float error = pid->setpoint - pid->input;

  pid->integrator += ki * error * dt;
  pid->integrator = constrain_c(pid->integrator, -i_max, i_max);

  // d term moving average filter
  // pid->delta = (error - pid->last_error) / dt;
  // float delta_sum = pid->delta + pid->delta1 + pid->delta2;
  // pid->delta2 = pid->delta1;
  // pid->delta1 = pid->delta;
  // pid->delta = delta_sum / 3.0f;

  // d term frequency filter
  float new_delta = (error - pid->last_error) / dt;
  uint8_t f_cut = 14; // Hz
  float rc = 1.0f / (2.0f * (float)M_PI * f_cut);
  pid->delta = pid->delta + dt / (rc + dt) * (new_delta - pid->delta);

  pid->p_term = kp * error;
  pid->i_term = pid->integrator;
  pid->d_term = constrain_c(kd * pid->delta, -200.0f, 200.0f);

  pid->last_error = error;
  pid->output = pid->p_term + pid->i_term + pid->d_term;
}

void pids_reset_i() {
  for (int i = 0; i < NUM_PIDS; i++) {
    pids[i].integrator = 0;
  }
}
