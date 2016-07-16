#pragma once

#define ANGLE_SAFETY_STOP true
#define SAFE_ANGLE 60.0

typedef enum {
  FC_UNARMED = 0,
  FC_ARMED
} fc_safety_mode_t;

typedef enum {
  FC_RATE_MODE = 0,
  FC_ANGLE_MODE
} fc_flight_mode_t;

void fc_init();
void fc_arm();
void fc_disarm();
void fc_process();
void fc_emergency_stop(const char *reason);
bool fc_armed();
fc_flight_mode_t fc_mode();
