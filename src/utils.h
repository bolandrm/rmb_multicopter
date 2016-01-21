#ifndef UTILS_H
#define UTILS_H

#define constrain_c(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

void serial_printf(const char *format, ...);
void serial_printlnf(const char *format, ...);
void support_printing_floats();
long map_c(long x, long in_min, long in_max, long out_min, long out_max);

#endif
