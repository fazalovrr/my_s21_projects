#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  if (*needle == '\0') {
    return (char *)haystack;
  }
  int is_found = 0;
  while (!is_found && *haystack != '\0') {
    const char *h = haystack;
    const char *n = needle;

    while (*h != '\0' && *n != '\0' && *h == *n) {
      h++;
      n++;
    }

    if (*n == '\0') {
      is_found = 1;
    } else
      haystack++;
  }
  return is_found ? (char *)haystack : S21_NULL;
}
