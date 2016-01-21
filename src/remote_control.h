#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include <Arduino.h>

#define  NUM_CHANNELS    6
#define  RC_TIMEOUT      1000  // milliseconds

#define  RC_CH1_IN_MIN   1154
#define  RC_CH1_IN_MAX   1898
#define  RC_CH1_OUT_MIN  -25
#define  RC_CH1_OUT_MAX  25
#define  RC_CH1_OFFSET   0

#define  RC_CH2_IN_MIN   1192
#define  RC_CH2_IN_MAX   1824
#define  RC_CH2_OUT_MIN  -25
#define  RC_CH2_OUT_MAX  25
#define  RC_CH2_OFFSET   0

#define  RC_CH3_IN_MIN   1172
#define  RC_CH3_IN_MAX   1812
#define  RC_CH3_OUT_MIN  1070
#define  RC_CH3_OUT_MAX  1500 // 1864 is max motor input
#define  RC_CH3_OFFSET   0

#define  RC_CH4_IN_MIN   1171
#define  RC_CH4_IN_MAX   1871
#define  RC_CH4_OUT_MIN  -100
#define  RC_CH4_OUT_MAX  100
#define  RC_CH4_OFFSET   (-13)

#define  RC_CH5_IN_MIN   996
#define  RC_CH5_IN_MAX   2000
#define  RC_CH5_OUT_MIN  0
#define  RC_CH5_OUT_MAX  100
#define  RC_CH5_OFFSET   0

#define  RC_CH6_IN_MIN   996
#define  RC_CH6_IN_MAX   2000
#define  RC_CH6_OUT_MIN  0
#define  RC_CH6_OUT_MAX  100
#define  RC_CH6_OFFSET   0

#define  RC_CH1  0
#define  RC_CH2  1
#define  RC_CH3  2
#define  RC_CH4  3
#define  RC_CH5  4
#define  RC_CH6  5

#define RC_ROLL     RC_CH1
#define RC_PITCH    RC_CH2
#define RC_THROTTLE RC_CH3
#define RC_YAW      RC_CH4
#define RC_POT_A    RC_CH5
#define RC_POT_B    RC_CH6

#define  RC_CH1_INPUT  7
#define  RC_CH2_INPUT  6
#define  RC_CH3_INPUT  5
#define  RC_CH4_INPUT  4
#define  RC_CH5_INPUT  3
#define  RC_CH6_INPUT  2

void rc_init();
void rc_read_values();
int16_t rc_get(int channel);

#endif
