#include <Arduino.h>
#include "flight_controller.h"
#include "imu.h"
#include "pids.h"
#include "motors.h"
#include "serial_commands.h"

void fc_safety_check();
void compute_pids();
void compute_motor_outputs();
bool min_throttle();
int16_t fc_throttle();

uint16_t gyro_freeze_counter = 0;
float last_gyro_value = 0.0;
bool emergency_stopped = false;
uint8_t safety_mode = UNARMED;
uint8_t flight_mode = RATE;

void fc_init() {
  pids_init();
  motors_init();
}

void fc_arm() {
  safety_mode = ARMED;
}

void fc_process() {
  fc_safety_check();

  compute_pids();
  compute_motor_outputs();

  if (safety_mode == ARMED && min_throttle()) {
    motors_command();
  } else {
    motors_command_all_off();
  }
}

void fc_emergency_stop() {
  emergency_stopped = true;
  motors_command_all_off();
  for(;;);
}

void compute_pids() {
  pid_compute(PID_RATE_X);
  pid_compute(PID_RATE_Y);
}

void fc_safety_check() {
  // watchdog to prevent stale imu values
  if (imu_rates().x == last_gyro_value) {
    gyro_freeze_counter++;
    if (gyro_freeze_counter == 500) {
      Serial.println("gyro freeze");
      fc_emergency_stop();
    }
  } else {
    gyro_freeze_counter = 0;
    last_gyro_value = imu_rates().x;
  }

  //if (imu->x_angle > 45.0 || imu->x_angle < -45.0
  //     || imu->y_angle > 45.0 || imu->y_angle < -45.0) {
  //  Serial.println("angles too high");
  //  emergency_stop();
  //}
}

void compute_motor_outputs() {
  double m1_r_out = fc_throttle() + pid(PID_RATE_X).output;
  double m2_l_out = fc_throttle() - pid(PID_RATE_X).output;
  double m3_f_out = fc_throttle() - pid(PID_RATE_Y).output;
  double m4_b_out = fc_throttle() + pid(PID_RATE_Y).output;

  motors_set_output(M1, (int16_t)(m1_r_out + 0.5));
  motors_set_output(M2, (int16_t)(m2_l_out + 0.5));
  motors_set_output(M3, (int16_t)(m3_f_out + 0.5));
  motors_set_output(M4, (int16_t)(m4_b_out + 0.5));
}

int16_t fc_throttle() {
  return serial_commands_throttle();
}

bool min_throttle() {
  return fc_throttle() >= 1100;
}
