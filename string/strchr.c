#include <stddef.h>

char* strchr(const char* str, int c) {
  size_t i = 0;
  do {
    if (str[i] == (char) c) {
      return (char*) str + i;
    }
  } while (str[i++]);
  return 0;
}
