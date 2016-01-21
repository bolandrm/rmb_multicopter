#include "remote_control.h"
#include "flight_controller.h"
#include "utils.h"

static int16_t rc_in_min[] = { RC_CH1_IN_MIN, RC_CH2_IN_MIN, RC_CH3_IN_MIN,
                               RC_CH4_IN_MIN, RC_CH5_IN_MIN, RC_CH6_IN_MIN };
static int16_t rc_in_max[] = { RC_CH1_IN_MAX, RC_CH2_IN_MAX, RC_CH3_IN_MAX,
                               RC_CH4_IN_MAX, RC_CH5_IN_MAX, RC_CH6_IN_MAX };
static int16_t rc_out_min[] = { RC_CH1_OUT_MIN, RC_CH2_OUT_MIN, RC_CH3_OUT_MIN,
                                RC_CH4_OUT_MIN, RC_CH5_OUT_MIN, RC_CH6_OUT_MIN };
static int16_t rc_out_max[] = { RC_CH1_OUT_MAX, RC_CH2_OUT_MAX, RC_CH3_OUT_MAX,
                                RC_CH4_OUT_MAX, RC_CH5_OUT_MAX, RC_CH6_OUT_MAX };
static int16_t rc_offset[] = { RC_CH1_OFFSET, RC_CH2_OFFSET, RC_CH3_OFFSET,
                               RC_CH4_OFFSET, RC_CH5_OFFSET, RC_CH6_OFFSET };

static uint32_t last_update_time = 0;
static uint16_t rc_values[NUM_CHANNELS] = {1500};
static uint32_t rc_start[NUM_CHANNELS];
static int16_t rc_out_values[NUM_CHANNELS] = {0};
static volatile uint16_t rc_shared[NUM_CHANNELS] = {1500};
static void process_channel_value(int channel);

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();

  if (millis() - last_update_time > RC_TIMEOUT) {
    // If we don't get an input for RC_TIMEOUT, set all vals to 0
    fc_disarm();
    for (int i = 0; i < NUM_CHANNELS; i++) {
      rc_out_values[i] = 0;
    }
  } else {
    for (int i = 0; i < NUM_CHANNELS; i++) {
      process_channel_value(i);
    }
  }
}

static void process_channel_value(int channel) {
  int16_t value = rc_values[channel];

  if (channel == RC_CH3) {
    if (value < 1070) {
      value = 0;
    } else if (value > 1070) {
      value = constrain_c(value, rc_in_min[channel], rc_in_max[channel]);
      value = map_c(value, rc_in_min[channel], rc_in_max[channel], rc_out_min[channel], rc_out_max[channel]);
    }
  } else {
    value = constrain_c(value, rc_in_min[channel], rc_in_max[channel]);
    value = map_c(value, rc_in_min[channel], rc_in_max[channel], rc_out_min[channel], rc_out_max[channel]);

    // workaround ... bug here
    if (channel == RC_CH4 && value == -100) {
      value = 0;
    }

    if ((channel == RC_CH1 || channel == RC_CH2 || channel == RC_CH4)
         && ((value > 0 && value < 2) || (value < 0 && value > -2)) ) {
      value = 0;
    }

    if (channel == RC_CH4) {
      value = -value; // invert yaw
    }
  }

  value += rc_offset[channel];

  rc_out_values[channel] = value;
}

int16_t rc_get(int channel) {
  return rc_out_values[channel];
}

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

void rc_init() {
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
