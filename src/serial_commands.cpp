#include "serial_commands.h"
#include "pids.h"

void serial_update_pids(byte);
void serial_update_throttle(byte);
int16_t throttle = 600;

void serial_commands_process() {
  if (Serial.available() <= 0) return;
  byte incomingByte = Serial.read();

  serial_update_pids(incomingByte);
  serial_update_throttle(incomingByte);
}

void serial_update_throttle(byte incomingByte) {
  if (incomingByte == 'i') {
    throttle += 50;
  } else if (incomingByte == 'k') {
    throttle -= 50;
  } else if (incomingByte == 'o') {
    throttle = 600;
  }
}

void serial_update_pids(byte incomingByte) {
  double kp, ki;

  kp = pid(PID_RATE_X)->kp;
  ki = pid(PID_RATE_X)->ki;

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

  pid(PID_RATE_X)->kp = kp;
  pid(PID_RATE_X)->ki = ki;
  pid(PID_RATE_Y)->kp = kp;
  pid(PID_RATE_Y)->ki = ki;
}

int16_t serial_commands_throttle() {
  return throttle;
}
