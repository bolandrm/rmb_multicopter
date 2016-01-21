#ifndef DEBUGGER_H
#define DEBUGGER_H

#define DEBUG true
#define DEBUG_RATE_MILLIS 500

void debugger_print();
void debugger_leds();
void debugger_leds_init();
void debugger_indicate_emergency();

#endif
