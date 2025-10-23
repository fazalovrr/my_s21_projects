#include "../s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;

  if (n > 0) {
    for (s21_size_t i = 0; i < n; i++) {
      if (str1[i] != str2[i] || str1[i] == '\0') {
        if (!result) result = (unsigned char)str1[i] - (unsigned char)str2[i];
      }
    }
  }

  return result;
}
