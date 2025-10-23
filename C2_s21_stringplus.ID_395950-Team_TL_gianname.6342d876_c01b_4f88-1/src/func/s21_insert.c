#include <stdio.h>
#include <stdlib.h>

#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL || s21_strlen(src) < start_index) {
    return S21_NULL;
  }
  s21_size_t len_str = s21_strlen(str);
  s21_size_t len_src = s21_strlen(src);
  char *insert_str = malloc((len_str + len_src + 100) * sizeof(char));

  if (insert_str != S21_NULL) {
    s21_size_t i = 0;
    for (; i < start_index; ++i) {
      insert_str[i] = src[i];
    }
    for (; i < (start_index + len_str); ++i) {
      insert_str[i] = str[i - start_index];
    }
    for (; i < (len_str + len_src); ++i) {
      insert_str[i] = src[i - len_str];
    }
    insert_str[len_str + len_src] = '\0';
  }
  return insert_str;
}
