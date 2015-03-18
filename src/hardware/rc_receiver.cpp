#include "rc_receiver.h"

static uint32_t rc_start[NUM_CHANNELS];
static volatile uint16_t rc_shared[NUM_CHANNELS] = {1500};
static uint32_t last_update_time = 0;

static void calc_input(int channel, int input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint32_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }

  last_update_time = millis();
}

static void calc_ch_1() { calc_input(RC_CH1, RC_CH1_INPUT); }
static void calc_ch_2() { calc_input(RC_CH2, RC_CH2_INPUT); }
static void calc_ch_3() { calc_input(RC_CH3, RC_CH3_INPUT); }
static void calc_ch_4() { calc_input(RC_CH4, RC_CH4_INPUT); }
static void calc_ch_5() { calc_input(RC_CH5, RC_CH5_INPUT); }
static void calc_ch_6() { calc_input(RC_CH6, RC_CH6_INPUT); }

uint16_t rc_receiver_shared_size() {
  return sizeof(rc_shared);
}

volatile uint16_t *rc_receiver_shared() {
  return rc_shared;
}

void rc_receiver_init() {
  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH2_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
  pinMode(RC_CH4_INPUT, INPUT);
  pinMode(RC_CH5_INPUT, INPUT);
  pinMode(RC_CH6_INPUT, INPUT);

  attachInterrupt(RC_CH1_INPUT, calc_ch_1, CHANGE);
  attachInterrupt(RC_CH2_INPUT, calc_ch_2, CHANGE);
  attachInterrupt(RC_CH3_INPUT, calc_ch_3, CHANGE);
  attachInterrupt(RC_CH4_INPUT, calc_ch_4, CHANGE);
  attachInterrupt(RC_CH5_INPUT, calc_ch_5, CHANGE);
  attachInterrupt(RC_CH6_INPUT, calc_ch_6, CHANGE);
}
