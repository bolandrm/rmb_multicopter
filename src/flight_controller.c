#include <Arduino.h>
#include "flight_controller.h"
#include "imu.h"
#include "pids.h"
#include "remote_control.h"
#include "motors.h"
#include "debugger.h"
#include "utils.h"
#include <stdbool.h>

#define ANGLE_SAFETY_STOP true

void fc_safety_check();
void compute_pids();
void compute_motor_outputs();
bool min_throttle();

uint16_t gyro_freeze_counter = 0;
float last_gyro_value = 0.0;
bool emergency_stopped = false;
uint8_t safety_mode = UNARMED;
uint8_t flight_mode = RATE;
// uint8_t flight_mode = STABILIZE;
bool on_ground = true;

void fc_init() {
  pids_init();
  motors_init();
}

int8_t fc_mode() {
  return flight_mode;
}

void fc_process() {
  fc_safety_check();

  if (on_ground) pids_reset_i();
  compute_pids();
  compute_motor_outputs();

  if (fc_armed() && min_throttle()) {
    on_ground = false;
    motors_command();
  } else {
    on_ground = true;
    motors_command_all_off();
  }
}

void fc_emergency_stop() {
  emergency_stopped = true;
  motors_command_all_off();
  for(;;) debugger_indicate_emergency();
}

void compute_pids() {
  pid(PID_RATE_X)->input = imu_rates().x;
  pid(PID_RATE_Y)->input = imu_rates().y;
  pid(PID_ANGLE_X)->input = imu_angles().x;
  pid(PID_ANGLE_Y)->input = imu_angles().y;
  pid(PID_RATE_Z)->input = imu_rates().z;

  if (flight_mode == STABILIZE) {
    pid(PID_ANGLE_X)->setpoint = rc_get(RC_ROLL);
    pid(PID_ANGLE_Y)->setpoint = rc_get(RC_PITCH);

    pid_compute(PID_ANGLE_X);
    pid_compute(PID_ANGLE_Y);

    pid(PID_RATE_X)->setpoint = pid(PID_ANGLE_X)->output;
    pid(PID_RATE_Y)->setpoint = pid(PID_ANGLE_Y)->output;
  } else {
    pid(PID_RATE_X)->setpoint = rc_get(RC_ROLL);
    pid(PID_RATE_Y)->setpoint = rc_get(RC_PITCH);
  }

  pid(PID_RATE_Z)->setpoint = rc_get(RC_YAW);

  pid_compute(PID_RATE_X);
  pid_compute(PID_RATE_Y);
  pid_compute(PID_RATE_Z);
}

void fc_safety_check() {
  float yaw = rc_get(RC_YAW);
  float yaw_max = rc_out_max(RC_YAW);

  if (rc_get(RC_THROTTLE) == 0 && yaw < yaw_max * -0.9) {
    fc_disarm();
  }

  if (rc_get(RC_THROTTLE) == 0 && yaw > yaw_max * 0.9) {
    fc_arm();
  }

  // watchdog to prevent stale imu values
  if (imu_rates().x == last_gyro_value) {
    gyro_freeze_counter++;
    if (gyro_freeze_counter == 500) {
      serial_printlnf("gyro freeze");
      fc_emergency_stop();
    }
  } else {
    gyro_freeze_counter = 0;
    last_gyro_value = imu_rates().x;
  }

  if (ANGLE_SAFETY_STOP && (imu_angles().x > SAFE_ANGLE || imu_angles().x < -SAFE_ANGLE
                             || imu_angles().y > SAFE_ANGLE || imu_angles().y < -SAFE_ANGLE)) {
    serial_printlnf("angles too high");
    fc_emergency_stop();
  }
}

void compute_motor_outputs() {
  float m1_fr_out = rc_get(RC_THROTTLE) - pid(PID_RATE_X)->output
                                        - pid(PID_RATE_Y)->output
                                        - pid(PID_RATE_Z)->output;
  float m2_bl_out = rc_get(RC_THROTTLE) + pid(PID_RATE_X)->output
                                        + pid(PID_RATE_Y)->output
                                        - pid(PID_RATE_Z)->output;
  float m3_fl_out = rc_get(RC_THROTTLE) - pid(PID_RATE_Y)->output
                                        + pid(PID_RATE_X)->output
                                        + pid(PID_RATE_Z)->output;
  float m4_br_out = rc_get(RC_THROTTLE) + pid(PID_RATE_Y)->output
                                        - pid(PID_RATE_X)->output
                                        + pid(PID_RATE_Z)->output;

  motors_set_output(M1, (int16_t)(m1_fr_out + 0.5));
  motors_set_output(M2, (int16_t)(m2_bl_out + 0.5));
  motors_set_output(M3, (int16_t)(m3_fl_out + 0.5));
  motors_set_output(M4, (int16_t)(m4_br_out + 0.5));
}

bool min_throttle() {
  return rc_get(RC_THROTTLE) >= RC_THROTTLE_MIN;
}

void fc_arm() {
  safety_mode = ARMED;
}

void fc_disarm() {
  safety_mode = UNARMED;
}

uint8_t fc_armed() {
  if (safety_mode == ARMED) {
    return 1;
  } else {
    return 0;
  }
}
