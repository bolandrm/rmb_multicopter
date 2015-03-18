#include "remote_control.h"
#include "rc_receiver.h"
#include "flight_controller.h"

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

static uint16_t rc_values[NUM_CHANNELS] = {1500};
static void process_channel_value(int channel);
static int16_t rc_out_values[NUM_CHANNELS] = {0};

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_receiver_shared(), rc_receiver_shared_size());
  interrupts();

  if (false) { //(millis() - last_update_time > RC_TIMEOUT) {
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
      value = constrain(value, rc_in_min[channel], rc_in_max[channel]);
      value = map(value, rc_in_min[channel], rc_in_max[channel], rc_out_min[channel], rc_out_max[channel]);
    }
  } else {
    value = constrain(value, rc_in_min[channel], rc_in_max[channel]);
    value = map(value, rc_in_min[channel], rc_in_max[channel], rc_out_min[channel], rc_out_max[channel]);

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

void rc_init() {
  rc_receiver_init();
}
