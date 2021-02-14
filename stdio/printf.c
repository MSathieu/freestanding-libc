#include <stdio.h>

int printf(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);
  int return_value = vprintf(format, args);
  va_end(args);
  return return_value;
}
