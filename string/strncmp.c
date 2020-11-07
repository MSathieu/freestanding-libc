#include <stddef.h>

int strncmp(const char* a, const char* b, size_t len) {
  size_t i = 0;
  while (i < len && a[i] && a[i] == b[i]) {
    i++;
  }
  if (i == len) {
    return 0;
  }
  return (unsigned char) a[i] - (unsigned char) b[i];
}
