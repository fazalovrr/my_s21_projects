#include "output.h"

#include <stdio.h>

void print_error(const char *message) {
  fprintf(stderr, "Error: %s\n", message);
}

void print_file_error(const char *filename) {
  fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
}

void print_special_char(unsigned char c) {
  switch (c) {
    case '\t':
    case '\n':
      putchar(c);
      break;
    case 127:
      printf("^?");
      break;
    default:
      if (c < 32) {
        printf("^%c", c + 64);
      } else if (c >= 128 && c < 160) {
        printf("M-%c", c - 64);
      } else if (c >= 160) {
        printf("M-%c", c - 128 + 64);
      } else {
        putchar(c);
      }
  }
}