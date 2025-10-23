#include <stdlib.h>

#include "../s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL || trim_chars == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t end_position = s21_strlen(src) - 1;
  s21_size_t start_position = 0;
  while (start_position <= end_position &&
         s21_strchr(trim_chars, src[start_position]) != S21_NULL) {
    start_position++;
  }
  while (start_position <= end_position &&
         s21_strchr(trim_chars, src[end_position]) != S21_NULL) {
    end_position--;
  }

  s21_size_t len = end_position - start_position + 1;
  if (end_position < start_position) len = 0;
  char *trim_str = malloc((len + 1) * sizeof(*src) + 1);

  if (trim_str != S21_NULL) {
    for (s21_size_t i = 0; i < len; ++i) {
      trim_str[i] = src[i + start_position];
    }
    trim_str[len] = '\0';
  }
  return trim_str;
}
