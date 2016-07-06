#include "debugger.h"
#include <Arduino.h>
#include "imu.h"
#include "utils.h"
#include "pids.h"
#include "remote_control.h"
#include "motors.h"
#include "flight_controller.h"
#include "serial_commands.h"
#include "battery_monitor.h"

void text_debug();
uint32_t debug_timer = 0;

void debugger_leds_init() {
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, HIGH);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void debugger_leds() {
  if (fc_armed()) {          // red steady, green off
    digitalWriteFast(RED_LED_PIN, HIGH);
    digitalWriteFast(GREEN_LED_PIN, LOW);
  } else {                   // green steady, red off
    digitalWriteFast(RED_LED_PIN, LOW);
    digitalWriteFast(GREEN_LED_PIN, HIGH);
  }
}

void debugger_indicate_emergency(const char *reason) {
  digitalWrite(GREEN_LED_PIN, LOW);

  if (digitalRead(RED_LED_PIN)) {
    digitalWrite(RED_LED_PIN, LOW);
  } else {
    digitalWrite(RED_LED_PIN, HIGH);
  }

  text_debug();
  serial_printlnf("EMERGENCY STOP: %s", reason);

  delay(500);
}

void text_debug() {
  // serial_printf("\033[2J\033[1;1H");

  // serial_printf("pid_rate_kp: %8.3f", CONFIG.data.pids[PID_RATE_X].kp);
  // serial_printf("\t pid_rate_ki: %8.3f", pid(PID_RATE_X)->ki);
  // serial_printf("\t pid_rate_yaw_kp: %8.3f", pid(PID_RATE_Z)->kp);
  // serial_printf("\t pid_angle_kp: %8.3f", pid(PID_ANGLE_X)->kp);
  // serial_printf("\t pid_angle_ki: %8.3f", pid(PID_ANGLE_X)->ki);
  // serial_printlnf("");

  serial_printf("gyro_x_raw: %i", imu_gyro_raws().x);
  serial_printf("\tgyro_y_raw: %i", imu_gyro_raws().y);
  serial_printf("\tgyro_z_raw: %i", imu_gyro_raws().z);
  serial_printlnf("");
  serial_printf("accel_x_raw: %i", imu_accel_raws().x);
  serial_printf("\taccel_y_raw: %i", imu_accel_raws().y);
  serial_printlnf("");

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

  serial_printf("value process dt: %d", imu_value_process_dt);
  serial_printlnf("\t max value process dt: %d", imu_max_value_process_dt);

  serial_printf("max accel: %8.3f", imu_accel_max_value());
  serial_printlnf("\t max gyro: %8.3f", imu_gyro_max_value());

  serial_printf("battery pin voltage: %8.3f", battery_monitor_pin_voltage);
  serial_printlnf("\t battery voltage: %8.3f", battery_monitor_battery_voltage);
}

void debugger_print() {
  text_debug();
}
