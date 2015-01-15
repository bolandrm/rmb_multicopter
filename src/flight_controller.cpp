#include <Arduino.h>
#include "flight_controller.h"
#include "imu.h"
#include "pids.h"
#include "motors.h"

uint16_t gyro_freeze_counter = 0;
float last_gyro_value = 0.0;
bool emergency_stopped = false;
uint8_t safety_mode = UNARMED;
uint8_t flight_mode = RATE;

void safety_check();
void compute_pids();

void fc_init() {
  pids_init();
  motors_init();
}

void fc_arm() {
  safety_mode = ARMED;
}

void fc_process() {
  safety_check();

  compute_pids();

  if (safety_mode == ARMED) {
    //compute_motor_outputs();
    motors_command();
  } else {
    motors_command_all_off();
  }
}

void fc_emergency_stop() {
  emergency_stopped = true;
  motors_command_all_off();
  while(1);
}

void compute_pids() {
  pid_compute(PID_RATE_X);
  pid_compute(PID_RATE_Y);
}

void safety_check() {
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

  // MOVE TO MOTOR FILE
  //for(int i = 0; i < NUM_MOTORS; i++) {
  //  if (motors.outputs[i] > INDOOR_SAFE_MOTOR_SPEED) {
  //    Serial.println("motors too high");
  //    emergency_stop();
  //  }
  //}
}
