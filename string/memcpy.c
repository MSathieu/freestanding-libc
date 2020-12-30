#include <stddef.h>

void* memcpy(void* restrict destptr, const void* restrict srcptr, size_t len) {
  unsigned char* dest = destptr;
  const unsigned char* src = srcptr;
  for (size_t i = 0; i < len; i++) {
    dest[i] = src[i];
  }
  return destptr;
}
