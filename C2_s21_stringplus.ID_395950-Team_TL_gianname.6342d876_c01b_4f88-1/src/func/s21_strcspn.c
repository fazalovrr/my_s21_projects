#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  const char *first_match = s21_strpbrk(str1, str2);
  if (first_match == S21_NULL) {
    count = s21_strlen(str1);
  } else {
    count = first_match - str1;
  }
  return count;
}
