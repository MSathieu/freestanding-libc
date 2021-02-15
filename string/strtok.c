#include <string.h>

char* strtok(char* restrict new_str, const char* restrict sep) {
  static char* str;
  if (new_str) {
    str = new_str;
  } else if (!str) {
    return 0;
  }
  str += strspn(str, sep);
  if (!str[0]) {
    return 0;
  }
  char* return_value = str;
  str += strcspn(str, sep);
  if (str[0]) {
    str[0] = 0;
    str++;
  }
  return return_value;
}
