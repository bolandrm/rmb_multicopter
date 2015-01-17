#include "remote_control.h"

int16_t RemoteControl::rc_in_min[] = { RC_CH1_IN_MIN, RC_CH2_IN_MIN, RC_CH3_IN_MIN,
                                       RC_CH4_IN_MIN, RC_CH5_IN_MIN, RC_CH6_IN_MIN };
int16_t RemoteControl::rc_in_max[] = { RC_CH1_IN_MAX, RC_CH2_IN_MAX, RC_CH3_IN_MAX,
                                       RC_CH4_IN_MAX, RC_CH5_IN_MAX, RC_CH6_IN_MAX };
int16_t RemoteControl::rc_out_min[] = { RC_CH1_OUT_MIN, RC_CH2_OUT_MIN, RC_CH3_OUT_MIN,
                                        RC_CH4_OUT_MIN, RC_CH5_OUT_MIN, RC_CH6_OUT_MIN };
int16_t RemoteControl::rc_out_max[] = { RC_CH1_OUT_MAX, RC_CH2_OUT_MAX, RC_CH3_OUT_MAX,
                                        RC_CH4_OUT_MAX, RC_CH5_OUT_MAX, RC_CH6_OUT_MAX };

uint32_t RemoteControl::last_update_time = 0;
uint32_t RemoteControl::last_read_time = 0;
uint16_t RemoteControl::rc_values[] = {0};
uint32_t RemoteControl::rc_start[] = {0};
volatile uint16_t RemoteControl::rc_shared[] = {0};

void RemoteControl::read_values() {
  //if (millis() - last_read_time > 20) {
    // read values at 50hz

    if (millis() - last_update_time > RC_TIMEOUT) {
      // If we don't get an input for RC_TIMEOUT, set all vals to 0
      for (int i = 0; i < NUM_CHANNELS; i++) {
        rc_values[i] = 0;
      }
    } else {
      noInterrupts();
      memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
      interrupts();
    }

    for (int i = 0; i < NUM_CHANNELS; i++) {
      process_channel_value(i);
    }

    //last_read_time = millis();
  //}
}

void RemoteControl::process_channel_value(int channel) {
  int16_t value = rc_values[channel];
  value = constrain(value, rc_in_min[channel], rc_in_max[channel]);
  value = map(value, rc_in_min[channel], rc_in_max[channel], rc_out_min[channel], rc_out_max[channel]);

  if ((channel == RC_CH1 || channel == RC_CH2 || channel == RC_CH4)
       && (value > 0 && value < 5 || value < 0 && value > -5) ) {
    value = 0;
  }

  if (channel == RC_CH1 || channel == RC_CH2) {
    value = -value; // invert roll & pitch
  }

  if (channel == RC_CH3 && value < 1070 && value > 750) {
    value = 1070;
  }

  rc_out_values[channel] = value;
}

int16_t RemoteControl::get(int channel) {
  return rc_out_values[channel];
}

void RemoteControl::calc_input(int channel, int input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint32_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }

  last_update_time = millis();
}

RemoteControl::RemoteControl() {}
