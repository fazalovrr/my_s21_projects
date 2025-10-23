#include "../s21_string.h"

void *s21_memchr(const void *str, int c, const s21_size_t n) {
  const unsigned char *ptr = str;
  void *rez = S21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (ptr[i] == (unsigned char)c) {
      rez = (void *)(ptr + i);
    }
  }
  return rez;
}