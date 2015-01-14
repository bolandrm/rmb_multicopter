#define DEBUG true
#define CHART_DEBUG false

#if DEBUG
  #if CHART_DEBUG
  #define DEBUG_RATE_MILLIS 50
  #else
  #define DEBUG_RATE_MILLIS 500
  #endif
#endif

void debugger_print();
