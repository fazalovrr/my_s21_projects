#include "../s21_string.h"

static char *search_delim(char *ptr_for_delim, char **ptr_token,
                          const char *delim) {
  if (*ptr_for_delim == '\0') {
    *ptr_token = S21_NULL;
    ptr_for_delim = S21_NULL;
  } else {
    *ptr_token = ptr_for_delim;
    ptr_for_delim = s21_strpbrk(ptr_for_delim, delim);

    if (ptr_for_delim != S21_NULL) {
      *ptr_for_delim = '\0';
      ptr_for_delim++;
    }
  }
  return ptr_for_delim;
}

char *s21_strtok(char *str, const char *delim) {
  char *token = S21_NULL;
  static char *ptr_for_delim = S21_NULL;
  if (str != S21_NULL) {
    ptr_for_delim = str;
    ptr_for_delim = search_delim(ptr_for_delim, &token, delim);
  } else {
    if (ptr_for_delim != S21_NULL) {
      ptr_for_delim = search_delim(ptr_for_delim, &token, delim);
    }
  }
  return token;
}
