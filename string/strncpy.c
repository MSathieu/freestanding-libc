#include <stddef.h>

char* strncpy(char* restrict dst, const char* restrict src, size_t len) {
  size_t i;
  for (i = 0; i < len && src[i]; i++) {
    dst[i] = src[i];
  }
  for (; i < len; i++) {
    dst[i] = 0;
  }
  return dst;
}
