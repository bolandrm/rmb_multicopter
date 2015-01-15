#include "debugger.h"
#include <Arduino.h>
#include "imu.h"
#include "pids.h"
#include "flight_controller.h"

int32_t debug_timer = millis();
int32_t loop_timer = micros();
int32_t loop_duration;

void text_debug() {
  Serial.print("gyro_x: "); Serial.print(imu_rates().x);
  Serial.print("\t gyro_y: "); Serial.print(imu_rates().y);
  Serial.print("\t gyro_z: "); Serial.print(imu_rates().z);
  Serial.println();

  Serial.print("pid_x_kp: "); Serial.print(pid(PID_RATE_X)->kp);
  Serial.print("\t pid_x_ki: "); Serial.print(pid(PID_RATE_X)->ki);
  Serial.println();

  Serial.print("throttle: "); Serial.print(fc_throttle());
  Serial.println();

  //Serial.print("gyro_x_ang: "); Serial.print(imu_gyro_angles().x);
  //Serial.print("\t gyro_y_ang: "); Serial.print(imu_gyro_angles().y);
  //Serial.println();

  //Serial.print("accel_x_raw: "); Serial.print(imu_accel_raws().x);
  //Serial.print("\t accel_y_raw: "); Serial.print(imu_accel_raws().y);
  //Serial.print("\t accel_z_raw: "); Serial.print(imu_accel_raws().z);
  //Serial.println();

  //Serial.print("accel_x_angle: "); Serial.print(imu_accel_angles().x);
  //Serial.print("\t accel_y_angle: "); Serial.print(imu_accel_angles().y);
  //Serial.println();

  //Serial.print("x_angle: "); Serial.print(imu_angles().x);
  //Serial.print("\t y_angle: "); Serial.print(imu_angles().y);
  //Serial.println();

  Serial.print("loop time: "); Serial.print(loop_duration);
  Serial.println();

  Serial.println();
}

void chart_debug() {
  //Serial.print(imu_gyro_angles().x);
  Serial.print(imu_rates().x);
  Serial.print(" ");
  //Serial.print(imu_accel_angles().x);
  Serial.print(imu_gyro_rates().x);
  Serial.print(" ");
  Serial.print(imu_angles().x);
  Serial.print(" ");
  Serial.print(1);
  Serial.print(" ");
  Serial.print(1);
  Serial.print(" ");
  Serial.print(1);
  Serial.print("\r");
}

void print_debug() {
  if (CHART_DEBUG) {
    chart_debug();
  } else {
    text_debug();
  }
}

void debugger_print() {
  loop_duration = micros() - loop_timer;

  if (DEBUG && millis() - debug_timer > DEBUG_RATE_MILLIS) {
    print_debug();
    debug_timer = millis();
  }

  loop_timer = micros();
}
