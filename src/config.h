#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include "pids.h"

#define CONFIG_VERSION 3

typedef struct __attribute__((packed)) {
  float kp;
  float ki;
  float i_max;
} pid_config;

typedef struct __attribute__((packed)) {
  uint16_t version;

  pid_config pids[NUM_PIDS];

} CONFIG_struct;

typedef union {
  CONFIG_struct data;
  uint8_t raw[sizeof(CONFIG_struct)];
} CONFIG_union;

extern CONFIG_union CONFIG;

void config_init();
void config_set();

#endif
