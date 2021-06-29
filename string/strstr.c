#include <string.h>

char* strstr(const char* a, const char* b) {
  size_t len = strlen(b);
  for (size_t i = 0; a[i]; i++) {
    if (!memcmp(a + i, b, len)) {
      return (char*) a + i;
    }
  }
  return 0;
}
