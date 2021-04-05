#include <stddef.h>

void* memmove(void* destptr, const void* srcptr, size_t size) {
  unsigned char* dest = destptr;
  const unsigned char* src = srcptr;
  if (dest < src) {
    for (size_t i = 0; i < size; i++) {
      dest[i] = src[i];
    }
  } else {
    for (size_t i = size; i; i--) {
      dest[i - 1] = src[i - 1];
    }
  }
  return destptr;
}
