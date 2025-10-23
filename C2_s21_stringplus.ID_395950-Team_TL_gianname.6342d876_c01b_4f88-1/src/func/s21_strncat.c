#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *ptr_dest = dest;
  while (*dest != '\0') {
    dest++;
  }

  while (n > 0 && *src != '\0') {
    *dest = *src;
    dest++;
    src++;
    n--;
  }

  *dest = '\0';

  return ptr_dest;
}
