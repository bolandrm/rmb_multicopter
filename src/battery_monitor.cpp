#include "battery_monitor.h"

static bool buzzer_on = false;

float battery_monitor_pin_voltage = 0.0;
float battery_monitor_battery_voltage = 0.0;

void battery_monitor_init() {
  pinMode(BATTERY_MONITOR_PIN, INPUT);

  analogReference(INTERNAL);
  analogReadResolution(12);
  analogReadAveraging(32);
}

void battery_monitor_update() {
  uint16_t pin_value = analogRead(BATTERY_MONITOR_PIN);

  battery_monitor_pin_voltage = pin_value / BATTERY_MONITOR_PIN_DIVISOR * BATTERY_MONITOR_LOSS_FACTOR;
  battery_monitor_battery_voltage = battery_monitor_pin_voltage / (BATTERY_R2_VAL / (BATTERY_R1_VAL + BATTERY_R2_VAL));

  if (battery_monitor_battery_voltage < 9.0) { // no battery connected
    noTone(BATTERY_ALERT_BUZZER_PIN);
    buzzer_on = false;

  } else if (battery_monitor_battery_voltage < 10.85) { // low voltage
    if (buzzer_on) {
      noTone(BATTERY_ALERT_BUZZER_PIN);
      buzzer_on = false;
    } else {
      tone(BATTERY_ALERT_BUZZER_PIN, 400);
      buzzer_on = true;
    }

  } else { // voltage good
    noTone(BATTERY_ALERT_BUZZER_PIN);
    buzzer_on = false;
  }
}
