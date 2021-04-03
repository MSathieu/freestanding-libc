#include <stddef.h>

void* memchr(const void* ptr, int c, size_t size) {
  const unsigned char* str = ptr;
  for (size_t i = 0; i < size; i++) {
    if (str[i] == (unsigned char) c) {
      return (void*) str + i;
    }
  }
  return 0;
}
