#include <stddef.h>

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t len) {
  unsigned char* dst = dstptr;
  const unsigned char* src = srcptr;
  for (size_t i = 0; i < len; i++) {
    dst[i] = src[i];
  }
  return dstptr;
}
