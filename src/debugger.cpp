#include "debugger.h"
#include <Arduino.h>

extern "C" {
#include "imu.h"
#include "utils.h"
#include "pids.h"
#include "remote_control.h"
}

#include "flight_controller.h"
#include "motors.h"
#include "serial_commands.h"

int32_t debug_timer = millis();

void debugger_leds_init() {
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
}

void debugger_leds() {
  if (fc_armed()) {          // red steady, green off
    digitalWrite(14, HIGH);
    digitalWrite(15, LOW);
  } else {                   // green steady, red off
    digitalWrite(14, LOW);
    digitalWrite(15, HIGH);
  }
}

void debugger_indicate_emergency() {
  digitalWrite(15, LOW);

  if (digitalRead(14)) {
    digitalWrite(14, LOW);
  } else {
    digitalWrite(14, HIGH);
  }

  delay(500);
}

void text_debug() {
  serial_printf("\033[2J\033[1;1H");

  // Serial.print("\t pid_x_out: "); Serial.print(pid(PID_ANGLE_X)->output);
  // Serial.print("\t pid_x_p: "); Serial.print(pid(PID_ANGLE_X)->p_term);
  // Serial.print("\t pid_x_i: "); Serial.print(pid(PID_ANGLE_X)->i_term);
  // Serial.println();

  // Serial.print("\t pid_y_out: "); Serial.print(pid(PID_ANGLE_Y)->output);
  // Serial.print("\t pid_y_p: "); Serial.print(pid(PID_ANGLE_Y)->p_term);
  // Serial.print("\t pid_y_i: "); Serial.print(pid(PID_ANGLE_Y)->i_term);
  // Serial.println();

  serial_printf("pid_rate_kp: %8.3f", pid(PID_RATE_X)->kp);
  serial_printf("\t pid_rate_ki: %8.3f", pid(PID_RATE_X)->ki);
  serial_printf("\t pid_rate_kd: %8.3f", pid(PID_RATE_X)->kd);
  serial_printf("\t pid_rate_yaw_kp: %8.3f", pid(PID_RATE_Z)->kp);
  serial_printf("\t pid_angle_kp: %8.3f", pid(PID_ANGLE_X)->kp);
  serial_printf("\t pid_angle_ki: %8.3f", pid(PID_ANGLE_X)->ki);
  serial_printf("\t pid_angle_kd: %8.3f", pid(PID_ANGLE_X)->kd);
  serial_printlnf("");

  serial_printf("gyro_x: %8.3f", imu_rates().x);
  serial_printf("\t pid_x_out: %8.3f", pid(PID_RATE_X)->output);
  serial_printf("\t pid_x_p: %8.3f", pid(PID_RATE_X)->p_term);
  serial_printf("\t pid_x_i: %8.3f", pid(PID_RATE_X)->i_term);
  serial_printf("\t rc_x: %8.3f", rc_get(RC_ROLL));
  serial_printf("\t angle_x: %8.3f", imu_angles().x);
  serial_printlnf("");

  serial_printf("gyro_y: %8.3f", imu_rates().y);
  serial_printf("\t pid_y_out: %8.3f", pid(PID_RATE_Y)->output);
  serial_printf("\t pid_y_p: %8.3f", pid(PID_RATE_Y)->p_term);
  serial_printf("\t pid_y_i: %8.3f", pid(PID_RATE_Y)->i_term);
  serial_printf("\t rc_y: %8.3f", rc_get(RC_PITCH));
  serial_printf("\t angle_y: %8.3f", imu_angles().y);
  serial_printlnf("");

  serial_printf("gyro_z: %8.3f", imu_rates().z);
  serial_printf("\t pid_z_out: %8.3f", pid(PID_RATE_Z)->output);
  serial_printf("\t pid_z_p: %8.3f", pid(PID_RATE_Z)->p_term);
  serial_printf("\t pid_z_i: %8.3f", pid(PID_RATE_Z)->i_term);
  serial_printf("\t rc_z: %8.3f", rc_get(RC_YAW));
  serial_printlnf("");

  serial_printf("rc_throttle: %8.3f", rc_get(RC_THROTTLE));
  serial_printf("\t m1: %8.3f", motor_level(M1));
  serial_printf("\t m2: %8.3f", motor_level(M2));
  serial_printf("\t m3: %8.3f", motor_level(M3));
  serial_printf("\t m4: %8.3f", motor_level(M4));
  serial_printlnf("");

  serial_printlnf("value process dt: "); Serial.print(imu_value_process_dt());
}

void chart_debug() {
  ////Serial.print(imu_gyro_angles().y);
  //Serial.print(imu_gyro_angles().x);
  //Serial.print(" ");
  ////Serial.print(imu_accel_angles().y);
  //Serial.print(imu_accel_angles().x);
  //Serial.print(" ");
  ////Serial.print(imu_angles().y);
  //Serial.print(imu_angles().x);
  //Serial.print(" ");
  //Serial.print(1);
  //Serial.print(" ");
  //Serial.print(1);
  //Serial.print(" ");
  //Serial.print(1);
  //Serial.println();

  String output = "";
  output += imu_gyro_angles().x;
  output += " ";
  output += imu_accel_angles().x;
  output += " ";
  output += imu_angles().x;
  output += " ";
  output += rc_get(RC_ROLL);
  output += " ";
  output += motor_level(M1);
  output += " ";
  output += motor_level(M2);
  output += " ";
  output += motor_level(M3);
  output += " ";
  output += motor_level(M4);
  Serial.println(output);
  Serial2.println(output);
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
  // output += imu_accel_raws().x;
  // output += "\t";
  // output += imu_accel_raws().y;
  // output += "\t";
  // output += imu_accel_raws().z;
  // output += "\t";
  // output += imu_accel_filtered().x;
  // output += "\t";
  // output += imu_accel_filtered().y;
  // output += "\t";
  // output += imu_accel_filtered().z;
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
  if (DEBUG && millis() - debug_timer > DEBUG_RATE_MILLIS) {
    print_debug();
    debug_timer = millis();
  }
}
