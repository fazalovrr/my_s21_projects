#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  while (*str1 != '\0') {
    const char *a = str2;
    while (*a != '\0') {
      if (*str1 == *a) {
        return (char *)str1;
      }
      a++;
    }
    str1++;
  }
  return S21_NULL;
}
