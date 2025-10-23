#include <stdlib.h>

#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  const char lowercase_letters[] = "abcdefghijklmnopqrstuvwxyz";
  char *up_str = S21_NULL;
  if (str != S21_NULL) {
    s21_size_t len = s21_strlen(str);
    up_str = malloc((len + 1) * sizeof(*str));
    if (up_str != S21_NULL) {
      for (s21_size_t i = 0; i < len; ++i) {
        if (s21_strchr(lowercase_letters, str[i]) != S21_NULL) {
          up_str[i] = str[i] - 32;
        } else {
          up_str[i] = str[i];
        }
      }
      up_str[len] = '\0';
    }
  }
  return up_str;
}
