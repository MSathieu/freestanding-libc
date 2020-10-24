#include <stddef.h>

void* memset(void* ptr, int c, size_t len) {
  unsigned char* buf = ptr;
  for (size_t i = 0; i < len; i++) {
    buf[i] = c;
  }
  return ptr;
}
