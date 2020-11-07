#include <stddef.h>

void* memmove(void* dstptr, const void* srcptr, size_t len) {
  unsigned char* dst = dstptr;
  const unsigned char* src = srcptr;
  if (dst < src) {
    for (size_t i = 0; i < len; i++) {
      dst[i] = src[i];
    }
  } else {
    for (size_t i = len; i; i--) {
      dst[i - 1] = src[i - 1];
    }
  }
  return dstptr;
}
