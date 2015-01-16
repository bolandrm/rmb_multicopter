#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

#include <Arduino.h>

void serial_commands_process();
int16_t serial_commands_throttle();
float serial_commands_target_control();

#endif
