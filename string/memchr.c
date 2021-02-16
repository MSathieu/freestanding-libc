#include <stddef.h>

void* memchr(const void* ptr, int c, size_t len) {
  const unsigned char* str = ptr;
  for (size_t i = 0; i < len; i++) {
    if (str[i] == (unsigned char) c) {
      return (void*) str + i;
    }
  }
  return 0;
}
