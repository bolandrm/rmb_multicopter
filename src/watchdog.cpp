#include "IntervalTimer.h"
#include "watchdog.h"
#include "flight_controller.h"

IntervalTimer watchdog_timer;
volatile bool watchdog_was_reset = true;

void watchdog_verify() {
  if (!watchdog_was_reset) {
    fc_emergency_stop("failed to reset watchdog");
  }

  watchdog_was_reset = false;
}

void watchdog_reset() {
  watchdog_was_reset = true;
}

void watchdog_enable() {
  watchdog_timer.begin(watchdog_verify, 1000000);
}

void watchdog_disable() {
  watchdog_timer.end();
}
