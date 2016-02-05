#include "config.h"
#include "pids.h"

CONFIG_union CONFIG;

void config_init() {
  CONFIG.data.version = CONFIG_VERSION;

  CONFIG.data.pid_rate_xy.kp = 2.25;
  CONFIG.data.pid_rate_xy.p_max = 0;
  CONFIG.data.pid_rate_xy.ki = 2.25;
  CONFIG.data.pid_rate_xy.i_max = 20.0;

  CONFIG.data.pid_rate_z.kp = 2.0;
  CONFIG.data.pid_rate_z.p_max = 0;
  CONFIG.data.pid_rate_z.ki = 4.0;
  CONFIG.data.pid_rate_z.i_max = 20.0;

  CONFIG.data.pid_angle_xy.kp = 2.0;
  CONFIG.data.pid_angle_xy.p_max = 0;
  CONFIG.data.pid_angle_xy.ki = 1.90;
  CONFIG.data.pid_angle_xy.i_max = 20.0;
}

void config_set(uint8_t data_buffer[]) {
  for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
      CONFIG.raw[i] = data_buffer[i];
  }

  // Refresh config values in some places
  pids_init();
}
