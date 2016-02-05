#include "config.h"

CONFIG_union CONFIG;

void config_init() {
  CONFIG.data.version = CONFIG_VERSION;

  CONFIG.data.pids[PID_RATE_X].kp = 2.25;
  CONFIG.data.pids[PID_RATE_X].p_max = 0;
  CONFIG.data.pids[PID_RATE_X].ki = 2.25;
  CONFIG.data.pids[PID_RATE_X].i_max = 20.0;

  CONFIG.data.pids[PID_RATE_Y].kp = 2.25;
  CONFIG.data.pids[PID_RATE_Y].p_max = 0;
  CONFIG.data.pids[PID_RATE_Y].ki = 2.25;
  CONFIG.data.pids[PID_RATE_Y].i_max = 20.0;

  CONFIG.data.pids[PID_RATE_Z].kp = 2.0;
  CONFIG.data.pids[PID_RATE_Z].p_max = 0;
  CONFIG.data.pids[PID_RATE_Z].ki = 4.0;
  CONFIG.data.pids[PID_RATE_Z].i_max = 20.0;

  CONFIG.data.pids[PID_ANGLE_X].kp = 2.0;
  CONFIG.data.pids[PID_ANGLE_X].p_max = 0;
  CONFIG.data.pids[PID_ANGLE_X].ki = 1.90;
  CONFIG.data.pids[PID_ANGLE_X].i_max = 20.0;

  CONFIG.data.pids[PID_ANGLE_Y].kp = 2.0;
  CONFIG.data.pids[PID_ANGLE_Y].p_max = 0;
  CONFIG.data.pids[PID_ANGLE_Y].ki = 1.90;
  CONFIG.data.pids[PID_ANGLE_Y].i_max = 20.0;
}

void config_set(uint8_t data_buffer[]) {
  for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
      CONFIG.raw[i] = data_buffer[i];
  }
}
