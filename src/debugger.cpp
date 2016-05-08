#include "debugger.h"
#include <Arduino.h>
#include "imu.h"
#include "utils.h"
#include "pids.h"
#include "remote_control.h"
#include "motors.h"
#include "flight_controller.h"
#include "serial_commands.h"

void text_debug();
int32_t debug_timer = 0;

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

void debugger_indicate_emergency(const char *reason) {
  digitalWrite(15, LOW);

  if (digitalRead(14)) {
    digitalWrite(14, LOW);
  } else {
    digitalWrite(14, HIGH);
  }

  serial_printlnf("EMERGENCY STOP: %s", reason);
  text_debug();

  delay(500);
}

void text_debug() {
  serial_printf("\033[2J\033[1;1H");

  // serial_printf("pid_rate_kp: %8.3f", CONFIG.data.pids[PID_RATE_X].kp);
  // serial_printf("\t pid_rate_ki: %8.3f", pid(PID_RATE_X)->ki);
  // serial_printf("\t pid_rate_yaw_kp: %8.3f", pid(PID_RATE_Z)->kp);
  // serial_printf("\t pid_angle_kp: %8.3f", pid(PID_ANGLE_X)->kp);
  // serial_printf("\t pid_angle_ki: %8.3f", pid(PID_ANGLE_X)->ki);
  // serial_printlnf("");

  serial_printf("gyro_x: %8.3f", imu_rates().x);
  serial_printf("\t pid_x_out: %8.3f", pid(PID_RATE_X)->output);
  serial_printf("\t pid_x_p: %8.3f", pid(PID_RATE_X)->p_term);
  serial_printf("\t pid_x_i: %8.3f", pid(PID_RATE_X)->i_term);
  serial_printf("\t pid_x_d: %8.3f", pid(PID_RATE_X)->d_term);
  serial_printf("\t rc_x: %8.3f", rc_get(RC_ROLL));
  serial_printf("\t angle_x: %8.3f", imu_angles().x);
  serial_printlnf("");

  serial_printf("gyro_y: %8.3f", imu_rates().y);
  serial_printf("\t pid_y_out: %8.3f", pid(PID_RATE_Y)->output);
  serial_printf("\t pid_y_p: %8.3f", pid(PID_RATE_Y)->p_term);
  serial_printf("\t pid_y_i: %8.3f", pid(PID_RATE_Y)->i_term);
  serial_printf("\t pid_y_d: %8.3f", pid(PID_RATE_Y)->d_term);
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
  serial_printf("\t m1: %d", motor_level(M1));
  serial_printf("\t m2: %d", motor_level(M2));
  serial_printf("\t m3: %d", motor_level(M3));
  serial_printf("\t m4: %d", motor_level(M4));
  serial_printlnf("");

  serial_printlnf("value process dt: %d", imu_value_process_dt());

  serial_printlnf("max accel: %8.3f", imu_accel_max_value());
  serial_printlnf("max gyro: %8.3f", imu_gyro_max_value());
}

void print_debug() {
  text_debug();
}

void debugger_print() {
  if (DEBUG && millis() - debug_timer > DEBUG_RATE_MILLIS) {
    print_debug();
    debug_timer = millis();
  }
}
