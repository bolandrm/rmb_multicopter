#include "debugger.h"
#include <Arduino.h>
#include "imu.h"
#include "pids.h"
#include "flight_controller.h"
#include "motors.h"
#include "serial_commands.h"
#include "remote_control.h"

int32_t debug_timer = millis();
int32_t loop_timer = micros();
int32_t loop_duration;

void text_debug() {
  Serial.print("pid_x_kp: "); Serial.print(pid(PID_RATE_X)->kp);
  Serial.print("\t pid_x_ki: "); Serial.print(pid(PID_RATE_X)->ki);
  Serial.println();

  Serial.print("gyro_x: "); Serial.print(imu_rates().x);
  Serial.print("\t pid_x_out: "); Serial.print(pid(PID_RATE_X)->output);
  Serial.print("\t pid_x_p: "); Serial.print(pid(PID_RATE_X)->p_term);
  Serial.print("\t pid_x_i: "); Serial.print(pid(PID_RATE_X)->i_term);
  Serial.print("\t rc_x: "); Serial.print(rc_get(RC_ROLL));
  Serial.println();

  Serial.print("gyro_y: "); Serial.print(imu_rates().y);
  Serial.print("\t pid_y_out: "); Serial.print(pid(PID_RATE_Y)->output);
  Serial.print("\t pid_y_p: "); Serial.print(pid(PID_RATE_Y)->p_term);
  Serial.print("\t pid_y_i: "); Serial.print(pid(PID_RATE_Y)->i_term);
  Serial.print("\t rc_y: "); Serial.print(rc_get(RC_PITCH));
  Serial.println();

  Serial.print("gyro_z: "); Serial.print(imu_rates().z);
  Serial.print("\t pid_z_out: "); Serial.print(pid(PID_RATE_Z)->output);
  Serial.print("\t pid_z_p: "); Serial.print(pid(PID_RATE_Z)->p_term);
  Serial.print("\t pid_z_i: "); Serial.print(pid(PID_RATE_Z)->i_term);
  Serial.print("\t rc_z: "); Serial.print(rc_get(RC_YAW));
  Serial.println();

  Serial.print("rc_throttle: "); Serial.print(rc_get(RC_THROTTLE));
  Serial.print("\t m1: "); Serial.print(motor_level(M1));
  Serial.print("\tm2: "); Serial.print(motor_level(M2));
  Serial.print("\tm3: "); Serial.print(motor_level(M3));
  Serial.print("\tm4: "); Serial.print(motor_level(M4));
  if (fc_armed()) {
    Serial.print("\t ARMED");
  } else {
    Serial.print("\t UNARMED");
  }

  Serial.println();

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

void logger_debug() {
//  Serial.print(imu_rates().x);
//  Serial.print("\t"); Serial.print(imu_rates().y);
//  Serial.print("\t"); Serial.print(imu_rates().z);
//
  String output = "";
  output += imu_rates().x;
  output += "\t";
  output += imu_rates().y;
  output += "\t";
  output += imu_rates().z;
  Serial.println(output);
}

void print_debug() {
  if (CHART_DEBUG) {
    chart_debug();
  } else if (LOGGER_DEBUG) {
    logger_debug();
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
