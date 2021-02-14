#include <stdarg.h>

extern int vfprintf(void* restrict, const char* restrict, va_list);
extern void* stdout;

int vprintf(const char* restrict format, va_list args) {
  return vfprintf(stdout, format, args);
}
