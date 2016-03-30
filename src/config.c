#include "config.h"
#include "remote_control.h"
#include "utils.h"
#include "eeprom_helpers.h"

CONFIG_union CONFIG;

void config_init() {
  if (eeprom_read(0) == 255) {
    config_init_from_default();
  } else {
    for (int i = 0; i < sizeof(CONFIG); i++) {
      CONFIG.raw[i] = eeprom_read(i);
    }

    if (CONFIG.data.version != CONFIG_VERSION) {
      config_init_from_default();
    }
  }
}

void config_update_eeprom() {
  for (int i = 0; i < sizeof(CONFIG); i++) {
    eeprom_update(i, CONFIG.raw[i]);
  }
}

void config_set(uint8_t data_buffer[]) {
  for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
    CONFIG.raw[i] = data_buffer[i];
  }

  config_update_eeprom();

  rc_config_updated();
}

void config_init_from_default() {
  CONFIG.data.version = CONFIG_VERSION;

  CONFIG.data.pids[PID_RATE_X].kp = 2.25;
  CONFIG.data.pids[PID_RATE_X].ki = 2.25;
  CONFIG.data.pids[PID_RATE_X].i_max = 20.0;
  CONFIG.data.pids[PID_RATE_X].kd = 0.0;

  CONFIG.data.pids[PID_RATE_Y].kp = 2.25;
  CONFIG.data.pids[PID_RATE_Y].ki = 2.25;
  CONFIG.data.pids[PID_RATE_Y].i_max = 20.0;
  CONFIG.data.pids[PID_RATE_Y].kd = 0.0;

  CONFIG.data.pids[PID_RATE_Z].kp = 2.0;
  CONFIG.data.pids[PID_RATE_Z].ki = 4.0;
  CONFIG.data.pids[PID_RATE_Z].i_max = 20.0;
  CONFIG.data.pids[PID_RATE_Z].kd = 0.0;

  CONFIG.data.pids[PID_ANGLE_X].kp = 2.0;
  CONFIG.data.pids[PID_ANGLE_X].ki = 1.90;
  CONFIG.data.pids[PID_ANGLE_X].i_max = 20.0;
  CONFIG.data.pids[PID_ANGLE_X].kd = 0.0;

  CONFIG.data.pids[PID_ANGLE_Y].kp = 2.0;
  CONFIG.data.pids[PID_ANGLE_Y].ki = 1.90;
  CONFIG.data.pids[PID_ANGLE_Y].i_max = 20.0;
  CONFIG.data.pids[PID_ANGLE_Y].kd = 0.0;

  for (int i = 0; i < RC_NUM_CHANNELS; i++) {
    CONFIG.data.rc_channels[i].invert = false;
    CONFIG.data.rc_channels[i].min = 1000;
    CONFIG.data.rc_channels[i].max = 2000;
  }

  CONFIG.data.rc_channels[RC_CH1].function = RC_THROTTLE;
  CONFIG.data.rc_channels[RC_CH1].min = 1125;
  CONFIG.data.rc_channels[RC_CH1].max = 1795;

  CONFIG.data.rc_channels[RC_CH2].function = RC_YAW;
  CONFIG.data.rc_channels[RC_CH2].invert = true;
  CONFIG.data.rc_channels[RC_CH2].min = 1092;
  CONFIG.data.rc_channels[RC_CH2].max = 1956;

  CONFIG.data.rc_channels[RC_CH3].function = RC_PITCH;
  CONFIG.data.rc_channels[RC_CH3].invert = true;
  CONFIG.data.rc_channels[RC_CH3].min = 1124;
  CONFIG.data.rc_channels[RC_CH3].max = 1890;

  CONFIG.data.rc_channels[RC_CH4].function = RC_ROLL;
  CONFIG.data.rc_channels[RC_CH4].min = 1090;
  CONFIG.data.rc_channels[RC_CH4].max = 1910;

  CONFIG.data.rc_channels[RC_CH5].function = RC_POT_A;
  CONFIG.data.rc_channels[RC_CH6].function = RC_POT_B;

  rc_config_updated();
}
