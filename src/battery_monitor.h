#pragma once

#include "Arduino.h"

#define BATTERY_MONITOR_PIN       A11
#define BATTERY_ALERT_BUZZER_PIN  25

#define BATTERY_R1_VAL 100000.0f
#define BATTERY_R2_VAL 10000.0f

#define BATTERY_MONITOR_READ_RESOLUTION 12
#define BATTERY_MONITOR_PIN_DIVISOR ((float) pow(2, BATTERY_MONITOR_READ_RESOLUTION))
#define BATTERY_MONITOR_LOSS_FACTOR 1.198f

void battery_monitor_init();
void battery_monitor_update();

extern float battery_monitor_pin_voltage;
extern float battery_monitor_battery_voltage;
