#ifndef DEBUGGER_H
#define DEBUGGER_H

#define DEBUG true
#define LOGGER_DEBUG false
#define CHART_DEBUG false

#if CHART_DEBUG
  #define DEBUG_RATE_MILLIS 50
#elif LOGGER_DEBUG
  #define DEBUG_RATE_MILLIS 10
#else
  #define DEBUG_RATE_MILLIS 500
#endif

void debugger_print();

#endif
