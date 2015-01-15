#include <Arduino.h>
#include "flight_controller.h"
#include "imu.h"

uint16_t gyro_freeze_counter = 0;
float last_gyro_value = 0.0;
bool emergency_stopped = false;
uint8_t safety_mode = UNARMED;
uint8_t flight_mode = RATE;


void safety_check();

void fc_init() {
}

void compute_pids() {
}

void adjust_for_bounds() {
}

void arm() {
  safety_mode = ARMED;
}

void fc_process() {
  safety_check();

  // adjust_pid_tuning();
  compute_pids();

  if (safety_mode == ARMED) {
    //compute_motor_outputs();
    adjust_for_bounds();
    //motors_command_all();
  } else {
    //motors_command_all_off();
  }
}

void emergency_stop() {
  emergency_stopped = true;
  // motors.command_all_off();
  while(1);
}

void safety_check() {
  // watchdog to prevent stale imu values
  if (imu_rates().x == last_gyro_value) {
    gyro_freeze_counter++;
    if (gyro_freeze_counter == 500) {
      Serial.println("gyro freeze");
      emergency_stop();
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
