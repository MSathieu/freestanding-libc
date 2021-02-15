#include <ctype.h>
#include <string.h>

long long atoll(const char* str) {
  size_t i = 0;
  while (isspace(str[i])) {
    i++;
  }
  char sign = '+';
  if (str[i] == '+') {
    i++;
  } else if (str[i] == '-') {
    sign = '-';
    i++;
  }
  long long return_value = 0;
  const char* digits = "0123456789";
  for (char* digit; str[i] && (digit = strchr(digits, str[i])); i++) {
    return_value = return_value * 10 + (digit - digits);
  }
  if (sign == '-') {
    return_value = -return_value;
  }
  return return_value;
}
