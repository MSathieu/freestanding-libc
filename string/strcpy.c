#include <stddef.h>

char* strcpy(char* restrict dst, const char* restrict src) {
  size_t i;
  for (i = 0; src[i]; i++) {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return dst;
}
