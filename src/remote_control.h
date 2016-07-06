#pragma once

#include <Arduino.h>

#define  RC_NUM_CHANNELS   6
#define  RC_TIMEOUT        1000  // milliseconds

#define RC_PITCH_ROLL_BASE 35.0    // 35.0 for RATE, 15.0 for STAB
#define RC_PITCH_ROLL_SENS 100.0

#define RC_YAW_BASE 60.0
#define RC_YAW_SENS 100.0

#define  RC_THROTTLE_MIN    1070.0
#define  RC_THROTTLE_MAX    1675.0 // 1864 is max motor input

#define  RC_CH1  0
#define  RC_CH2  1
#define  RC_CH3  2
#define  RC_CH4  3
#define  RC_CH5  4
#define  RC_CH6  5

#define  RC_ROLL      0
#define  RC_PITCH     1
#define  RC_THROTTLE  2
#define  RC_YAW       3
#define  RC_POT_A     4
#define  RC_POT_B     5

#define  RC_CH1_INPUT  2
#define  RC_CH2_INPUT  3
#define  RC_CH3_INPUT  4
#define  RC_CH4_INPUT  27
#define  RC_CH5_INPUT  26
#define  RC_CH6_INPUT  24

void rc_init();
void rc_read_values();
float rc_get(uint8_t function);
int16_t rc_get_raw(uint8_t function);
float rc_out_max(uint8_t function);
float rc_out_min(uint8_t function);
void rc_config_updated();
