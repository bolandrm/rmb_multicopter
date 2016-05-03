#include "Arduino.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void serial_printf(const char *format, ...) {
  char buffer[256];

  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);

  Serial.write(buffer, strlen(buffer));

  va_end(args);
}

void serial_printlnf(const char *format, ...) {
  char buffer[256];

  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);

  char newline_buffer[256];
  sprintf(newline_buffer, "%s\r\n", buffer);

  Serial.write(newline_buffer, strlen(newline_buffer));

  va_end(args);
}
