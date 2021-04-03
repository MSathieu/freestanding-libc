#include <stddef.h>

int memcmp(const void* aptr, const void* bptr, size_t size) {
  const unsigned char* a = aptr;
  const unsigned char* b = bptr;
  for (size_t i = 0; i < size; i++) {
    if (a[i] != b[i]) {
      return a[i] - b[i];
    }
  }
  return 0;
}
