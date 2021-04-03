#include <stddef.h>

char* strncpy(char* restrict dest, const char* restrict src, size_t size) {
  size_t i;
  for (i = 0; i < size && src[i]; i++) {
    dest[i] = src[i];
  }
  for (; i < size; i++) {
    dest[i] = 0;
  }
  return dest;
}
