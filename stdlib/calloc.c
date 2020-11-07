#include <string.h>

extern void* malloc(size_t);

void* calloc(size_t num, size_t size) {
  void* buf = malloc(num * size);
  memset(buf, 0, num * size);
  return buf;
}
