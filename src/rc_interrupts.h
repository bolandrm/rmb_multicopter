#ifndef RC_INTERRUPTS_H
#define RC_INTERRUPTS_H

#include <PinChangeInt.h>
#include "remote_control.h"

#define  RC_CH1_INPUT  A0
#define  RC_CH2_INPUT  A1
#define  RC_CH3_INPUT  A2
#define  RC_CH4_INPUT  A3
#define  RC_CH5_INPUT  7
#define  RC_CH6_INPUT  8

void calc_ch_1() { RemoteControl::calc_input(RC_CH1, RC_CH1_INPUT); }
void calc_ch_2() { RemoteControl::calc_input(RC_CH2, RC_CH2_INPUT); }
void calc_ch_3() { RemoteControl::calc_input(RC_CH3, RC_CH3_INPUT); }
void calc_ch_4() { RemoteControl::calc_input(RC_CH4, RC_CH4_INPUT); }
void calc_ch_5() { RemoteControl::calc_input(RC_CH5, RC_CH5_INPUT); }
void calc_ch_6() { RemoteControl::calc_input(RC_CH6, RC_CH6_INPUT); }

void bind_rc_interrupts() {
  PCintPort::attachInterrupt(RC_CH1_INPUT, calc_ch_1, CHANGE);
  PCintPort::attachInterrupt(RC_CH2_INPUT, calc_ch_2, CHANGE);
  PCintPort::attachInterrupt(RC_CH3_INPUT, calc_ch_3, CHANGE);
  PCintPort::attachInterrupt(RC_CH4_INPUT, calc_ch_4, CHANGE);
  // PCintPort::attachInterrupt(RC_CH5_INPUT, calc_ch_5, CHANGE);
  // PCintPort::attachInterrupt(RC_CH6_INPUT, calc_ch_6, CHANGE);
}

#endif
