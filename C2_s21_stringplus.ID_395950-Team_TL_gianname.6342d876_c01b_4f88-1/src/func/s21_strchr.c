#include "../s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *result = S21_NULL;

  if (str != S21_NULL) {
    unsigned char target = (unsigned char)c;

    while (*str != '\0') {
      if ((unsigned char)*str == target) {
        result = (char *)str;
        break;
      }
      str++;
    }

    if (result == S21_NULL && target == '\0') {
      result = (char *)str;
    }
  }

  return result;
}