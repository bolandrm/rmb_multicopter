#pragma once

#include "pids.h"
#include "remote_control.h"

#define CONFIG_VERSION 6

typedef struct __attribute__((packed)) {
  uint8_t function;
  uint16_t min;
  uint16_t max;
  bool invert;
} config_rc_channel;

typedef struct __attribute__((packed)) {
  float kp;
  float ki;
  float i_max;
  float kd;
} config_pid;

typedef struct __attribute__((packed)) {
  uint16_t version;

  config_rc_channel rc_channels[RC_NUM_CHANNELS];
  config_pid pids[NUM_PIDS];

} CONFIG_struct;

typedef union {
  CONFIG_struct data;
  uint8_t raw[sizeof(CONFIG_struct)];
} CONFIG_union;

extern CONFIG_union CONFIG;

void config_init();
void config_init_from_default();
void config_set(uint8_t data_buffer[]);
