#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define CONFIG_VERSION 1

typedef struct __attribute__((packed)) {
  float kp;
  float p_max;
  float ki;
  float i_max;
} pid_config;

typedef struct __attribute__((packed)) {
  uint16_t version;

  pid_config pid_rate_xy;
  pid_config pid_rate_z;
  pid_config pid_angle_xy;

} CONFIG_struct;

typedef union {
  CONFIG_struct data;
  uint8_t raw[sizeof(CONFIG_struct)];
} CONFIG_union;

extern CONFIG_union CONFIG;

void config_init();
void config_set();

#endif
