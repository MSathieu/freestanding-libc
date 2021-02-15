#include <string.h>

size_t strcspn(const char* str, const char* chars) {
  size_t i;
  for (i = 0; str[i] && !strchr(chars, str[i]); i++)
    ;
  return i;
}
