#include <stddef.h>

char* strncpy(char* restrict dest, const char* restrict src, size_t len) {
  size_t i;
  for (i = 0; i < len && src[i]; i++) {
    dest[i] = src[i];
  }
  for (; i < len; i++) {
    dest[i] = 0;
  }
  return dest;
}
