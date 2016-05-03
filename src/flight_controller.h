#pragma once

#define RATE 0
#define STABILIZE 1

#define ARMED 1
#define UNARMED 0

#define SAFE_ANGLE 60.0

void fc_init();
void fc_arm();
void fc_disarm();
void fc_process();
void fc_emergency_stop(char *reason);
uint8_t fc_armed();
int8_t fc_mode();
