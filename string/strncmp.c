#include <stddef.h>

int strncmp(const char* a, const char* b, size_t size) {
  size_t i = 0;
  while (i < size && a[i] && a[i] == b[i]) {
    i++;
  }
  if (i == size) {
    return 0;
  }
  return (unsigned char) a[i] - (unsigned char) b[i];
}
