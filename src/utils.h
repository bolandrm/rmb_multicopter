#pragma once

#define constrain_c(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define abs_c(x) ((x)>0?(x):-(x))

#define map_c(x, in_min, in_max, out_min, out_max) ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

void serial_printf(const char *format, ...);
void serial_printlnf(const char *format, ...);
void support_printing_floats();
