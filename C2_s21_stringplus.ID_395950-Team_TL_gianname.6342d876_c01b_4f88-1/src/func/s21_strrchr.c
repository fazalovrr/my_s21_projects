#include "../s21_string.h"

char *s21_strrchr(const char *str, int c) {
  const char *last_match = S21_NULL;
  unsigned char target = (unsigned char)c;

  while (*str != '\0') {
    if ((unsigned char)*str == target) {
      last_match = str;
    }
    str++;
  }
  if (target == '\0') {
    return (char *)str;
  }
  return (char *)last_match;
}