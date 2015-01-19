#include "serial_commands.h"
#include "pids.h"
#include "flight_controller.h"

void serial_update_pids(byte);
void serial_update_throttle(byte);
void serial_update_control(byte);
int16_t throttle = 600;
float target_control = 0.0;
int32_t control_timer = 0;

void serial_commands_process() {
  if (millis() - control_timer > 1000) {
    target_control = 0.0;
  }

  if (Serial.available() <= 0) return;
  byte incomingByte = Serial.read();

  serial_update_pids(incomingByte);
  serial_update_throttle(incomingByte);
  serial_update_control(incomingByte);
}

void serial_update_throttle(byte incomingByte) {
  if (incomingByte == 'i') {
    throttle += 50;
  } else if (incomingByte == 'k') {
    throttle -= 50;
  } else if (incomingByte == 'o') {
    throttle = 600;
  } else if (incomingByte == 'u') {
    fc_arm();
  }
}

void serial_update_pids(byte incomingByte) {
  double kp, ki;

  //kp = pid(PID_RATE_X)->kp;
  //ki = pid(PID_RATE_X)->ki;
  kp = pid(PID_ANGLE_X)->kp;
  ki = pid(PID_ANGLE_X)->ki;

  if (incomingByte == 'a') {
    if (kp <= 0.05) kp = 0;
    else kp -= 0.05;
  } else if (incomingByte == 's') {
    if (ki <= 0.05) ki = 0;
    else ki -= 0.05;
  // } else if (incomingByte == 'd') {
    // if (kd <= 0.05) kd = 0;
    // else kd -= 0.05;
  } else if (incomingByte == 'q') {
    if (kp == 0) kp = 0.01;
    else kp += 0.05;
  } else if (incomingByte == 'w') {
    if (ki == 0) ki = 0.01;
    else ki += 0.05;
  //} else if (incomingByte == 'e') {
    // if (kd == 0) kd = 0.01;
    // else kd += 0.05;
  }

  // pid(PID_RATE_X)->kp = kp;
  // pid(PID_RATE_X)->ki = ki;
  // pid(PID_RATE_Y)->kp = kp;
  // pid(PID_RATE_Y)->ki = ki;
  pid(PID_ANGLE_X)->kp = kp;
  pid(PID_ANGLE_X)->ki = ki;
  pid(PID_ANGLE_Y)->kp = kp;
  pid(PID_ANGLE_Y)->ki = ki;
}

void serial_update_control(byte incomingByte) {
  if (incomingByte == ',') {
    target_control = 30.0;
    control_timer = millis();
  } else if (incomingByte == '.') {
    target_control = -30.0;
    control_timer = millis();
  }
}

int16_t serial_commands_throttle() {
  return throttle;
}

float serial_commands_target_control() {
  return target_control;
}
