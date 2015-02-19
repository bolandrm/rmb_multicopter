#include "serial_commands.h"
#include "pids.h"
#include "flight_controller.h"

void serial_update_pids(byte);

void serial_commands_process() {
  if (Serial.available() <= 0) return;
  byte incomingByte = Serial.read();

  serial_update_pids(incomingByte);
}

void serial_update_pids(byte incomingByte) {
  double kp, ki, kd;

  if (fc_mode() == STABILIZE) {
    kp = pid(PID_ANGLE_X)->kp;
    ki = pid(PID_ANGLE_X)->ki;
    kd = pid(PID_ANGLE_X)->kd;
  } else {
    kp = pid(PID_RATE_X)->kp;
    ki = pid(PID_RATE_X)->ki;
    kd = pid(PID_RATE_X)->kd;
  }

  if (incomingByte == 'a') {
    if (kp <= 0.05) kp = 0;
    else kp -= 0.05;
  } else if (incomingByte == 's') {
    if (ki <= 0.05) ki = 0;
    else ki -= 0.05;
  } else if (incomingByte == 'd') {
     if (kd <= 0.05) kd = 0;
     else kd -= 0.05;
  } else if (incomingByte == 'q') {
    if (kp == 0) kp = 0.01;
    else kp += 0.05;
  } else if (incomingByte == 'w') {
    if (ki == 0) ki = 0.01;
    else ki += 0.05;
  } else if (incomingByte == 'e') {
     if (kd == 0) kd = 0.01;
     else kd += 0.05;
  }

  if (fc_mode() == STABILIZE) {
    pid(PID_ANGLE_X)->kp = kp;
    pid(PID_ANGLE_X)->ki = ki;
    pid(PID_ANGLE_X)->kd = kd;

    pid(PID_ANGLE_Y)->kp = kp;
    pid(PID_ANGLE_Y)->ki = ki;
    pid(PID_ANGLE_Y)->kd = kd;
  } else {
    pid(PID_RATE_X)->kp = kp;
    pid(PID_RATE_X)->ki = ki;
    pid(PID_RATE_X)->kd = kd;

    pid(PID_RATE_Y)->kp = kp;
    pid(PID_RATE_Y)->ki = ki;
    pid(PID_RATE_Y)->kd = kd;
  }
}
