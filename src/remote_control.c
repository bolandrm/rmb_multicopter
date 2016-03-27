#include "remote_control.h"
#include "flight_controller.h"
#include "utils.h"
#include "config.h"

static uint8_t functions_to_channels[RC_NUM_CHANNELS];
static uint32_t last_update_time = 0;
static uint16_t rc_values[RC_NUM_CHANNELS] = { 1500 };
static uint32_t rc_start[RC_NUM_CHANNELS];
static float rc_out_values[RC_NUM_CHANNELS] = { 0 };
static float out_min_values[RC_NUM_CHANNELS] = { 0 };
static float out_max_values[RC_NUM_CHANNELS] = { 0 };
static volatile uint16_t rc_shared[RC_NUM_CHANNELS] = { 1500 };
static void process_channel_value(uint8_t channel);

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();

  if (millis() - last_update_time > RC_TIMEOUT) {
    // If we don't get an input for RC_TIMEOUT, set all vals to 0
    fc_disarm();
    for (int i = 0; i < RC_NUM_CHANNELS; i++) {
      rc_out_values[i] = 0;
    }
  } else {
    for (int i = 0; i < RC_NUM_CHANNELS; i++) {
      process_channel_value(i);
    }
  }
}

static void process_channel_value(uint8_t channel) {
  config_rc_channel channel_config = CONFIG.data.rc_channels[channel];
  uint8_t function = channel_config.function;
  float value = rc_values[channel];

  if ((function == RC_YAW || function == RC_PITCH || function == RC_ROLL) &&
      ((value <= 1510 && value >= 1500) || (value >= 1490 && value <= 1500))) {
    value = 0.0;
  } else if (function == RC_THROTTLE && value < 1125) {
    value = 0.0;
  } else {
    value = constrain_c(value, channel_config.min, channel_config.max);

    value = map_c(
      value,
      channel_config.min, channel_config.max,
      out_min_values[function], out_max_values[function]
    );
  }

  if (channel_config.invert) value = -value;

  rc_out_values[function] = value;
}

float rc_get(uint8_t function) {
  return rc_out_values[function];
}

float rc_out_max(uint8_t function) {
  return out_max_values[function];
}

float rc_out_min(uint8_t function) {
  return out_min_values[function];
}

int16_t rc_get_raw(uint8_t function) {
  return rc_values[functions_to_channels[function]];
}

static void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint32_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }

  last_update_time = millis();
}

void rc_config_updated() {
  for (int i = 0; i < RC_NUM_CHANNELS; i++) {
    config_rc_channel channel_config = CONFIG.data.rc_channels[i];
    uint8_t function = channel_config.function;

    float out_max = 100.0;
    float out_min = 0.0;

    if (function == RC_THROTTLE) {
      out_max = RC_THROTTLE_MAX;
      out_min = RC_THROTTLE_MIN;
    } else if (function == RC_PITCH || function == RC_ROLL) {
      out_max = RC_PITCH_ROLL_BASE * RC_PITCH_ROLL_SENS / 100.0;
      out_min = -1 * out_max;
    } else if (function == RC_YAW) {
      out_max = RC_YAW_BASE * RC_YAW_SENS / 100.0;
      out_min = -1 * out_max;
    }

    // store variables based on config data
    functions_to_channels[function] = i;
    out_max_values[function] = out_max;
    out_min_values[function] = out_min;
  }
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
