#include <stddef.h>

int strcmp(const char* a, const char* b) {
  size_t i = 0;
  while (a[i] && a[i] == b[i]) {
    i++;
  }
  return (unsigned char) a[i] - (unsigned char) b[i];
}
