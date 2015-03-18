#ifndef RC_RECEIVER_H
#define RC_RECEIVER_H

#include <Arduino.h>
#include "remote_control.h"

void rc_receiver_init();
volatile uint16_t *rc_receiver_shared();
uint16_t rc_receiver_shared_size();

#endif
