#include <stddef.h>

void* memmove(void* destptr, const void* srcptr, size_t len) {
  unsigned char* dest = destptr;
  const unsigned char* src = srcptr;
  if (dest < src) {
    for (size_t i = 0; i < len; i++) {
      dest[i] = src[i];
    }
  } else {
    for (size_t i = len; i; i--) {
      dest[i - 1] = src[i - 1];
    }
  }
  return destptr;
}
