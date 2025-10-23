#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_string.h"

#define BUFFER 2025
typedef struct {
  unsigned int f;
  unsigned int s;
  char *str;
  char *format;
  int starr;
  int width;
  char length;
  char specifier;
  int spec_proc;
  int error;
  int ret_count;
  int fraction;
  int count;
} data;

static void pa_format(data *structura, va_list *args);
static void scanf_str(data *structura, va_list *args);
static void reading_u(data *structura, va_list *args);
static void reading_i(data *structura, va_list *args);
static void reading_x(data *structura, va_list *args);
static void reading_o(data *structura, va_list *args);
static void reading_p(data *structura, va_list *args);
static void reading_f(data *structura, va_list *args);
static void reading_d(data *structura, va_list *args);
static void reading_s(data *structura, va_list *args);
static void reading_c(data *structura, va_list *args);
static void reading_e(data *structura, va_list *args);
static void reading_g(data *structura, va_list *args);
static int str_valid(const data *structura, int count);

int s21_sscanf(const char *str, const char *format, ...) {
  int result = 0;

  if (format == S21_NULL) {
    result = -1;
  } else {
    va_list args;
    data structura = {0};
    va_start(args, format);
    structura.count = 0;
    structura.error = 0;

    if (s21_strlen(str) == 0) {
      int has_non_space = 0;
      for (size_t i = 0; i < s21_strlen(format); i++) {
        if (!isspace(format[i])) {
          has_non_space = -1;
        }
      }
      result = has_non_space;
    } else {
      structura.str = malloc(s21_strlen(str) + 1);
      structura.format = malloc(s21_strlen(format) + 1);
      s21_strncpy(structura.str, str, s21_strlen(str) + 1);
      s21_strncpy(structura.format, format, s21_strlen(format) + 1);
      structura.f = 0;
      structura.s = 0;

      while (structura.f < s21_strlen(structura.format)) {
        int proceed = 1;

        if (isspace(structura.format[structura.f])) {
          while (isspace(structura.str[structura.s])) {
            structura.s++;
          }
          structura.f++;
          proceed = 0;
        }

        if (proceed && structura.format[structura.f] == '%') {
          pa_format(&structura, &args);

          if (structura.specifier != '\0') {
            if (!structura.error && structura.ret_count &&
                structura.specifier != 'n') {
              structura.count++;
            }
          }
          structura.f++;
          proceed = 0;
        }

        if (proceed) {
          if (structura.format[structura.f] != structura.str[structura.s]) {
            structura.error = 1;
            structura.f = s21_strlen(structura.format);
          } else {
            structura.s++;
            structura.f++;
          }
        }
      }

      free(structura.str);
      free(structura.format);
      result = structura.count;
    }

    va_end(args);
  }

  return result;
}

void pa_format(data *structura, va_list *args) {
  structura->starr = 0;
  structura->width = 0;
  structura->length = '\0';
  structura->specifier = '\0';
  structura->spec_proc = 0;
  structura->error = 0;
  structura->ret_count = 0;

  unsigned int i = structura->f + 1;

  if (structura->format[i] == '\0') {
    structura->error = 1;
  } else {
    if (structura->format[i] == '*') {
      structura->starr = 1;
      i++;
    }
    char tmp_w[9] = {0};
    int w_lench = 0;
    while (structura->format[i] >= '0' && structura->format[i] <= '9') {
      tmp_w[w_lench++] = structura->format[i++];
    }
    if (w_lench > 0) {
      structura->width = atoi(tmp_w);
    }

    if (structura->format[i] == 'L' || structura->format[i] == 'l' ||
        structura->format[i] == 'h') {
      structura->length = structura->format[i++];
    }

    if (s21_strchr("cdieEfgGosuxXpn%", structura->format[i])) {
      structura->specifier = structura->format[i++];
    } else {
      structura->specifier = '\0';
      structura->error = 1;
    }
    if (!structura->error) {
      structura->f = i - 1;

      if (structura->specifier == '%' && structura->format[i - 2] == '%') {
        structura->spec_proc = 1;
      }
      scanf_str(structura, args);
    }
  }
}

void scanf_str(data *structura, va_list *args) {
  while (isspace(structura->str[structura->s])) {
    structura->s++;
  }
  int should_parse = 1;

  if (structura->specifier == 'n') {
    if (!structura->starr) {
      *va_arg(*args, int *) = structura->s;
      structura->ret_count = 1;
    }
    should_parse = 0;
  }

  if (should_parse && structura->spec_proc) {
    if (structura->str[structura->s] == '%') {
      structura->s++;
    }
    should_parse = 0;
  }

  if (should_parse) {
    if (structura->specifier == 'f') {
      reading_f(structura, args);
    }
    if (structura->specifier == 'e' || structura->specifier == 'E') {
      reading_e(structura, args);
    }
    if (structura->specifier == 'g' || structura->specifier == 'G') {
      reading_g(structura, args);
    }
    if (structura->specifier == 's') {
      reading_s(structura, args);
    }
    if (structura->specifier == 'c') {
      reading_c(structura, args);
    }
    if (structura->specifier == 'p') {
      reading_p(structura, args);
    }
    if (structura->specifier == 'u') {
      reading_u(structura, args);
    }
    if (structura->specifier == 'o') {
      reading_o(structura, args);
    }
    if (structura->specifier == 'i') {
      reading_i(structura, args);
    }
    if (structura->specifier == 'x' || structura->specifier == 'X') {
      reading_x(structura, args);
    }
    if (structura->specifier == 'd') {
      reading_d(structura, args);
    }
  }
}

static int str_valid(const data *structura, int count) {
  int result = 0;
  if (structura->str[structura->s + count] != ' ' &&
      structura->str[structura->s + count] != '\t' &&
      structura->str[structura->s + count] != '\n' &&
      structura->str[structura->s + count] != '\0') {
    result = 1;
  }
  return result;
}

void reading_i(data *structura, va_list *args) {
  int int_count = 0;
  int parsing_ok = 1;

  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    if ((structura->str[structura->s] == '-' ||
         structura->str[structura->s] == '+')) {
      int_count++;
    }
    while (structura->str[structura->s + int_count] != ' ' &&
           structura->str[structura->s + int_count] != '\0') {
      int_count++;
    }
  }

  if (int_count == 0 ||
      (int_count == 1 && (structura->str[structura->s] == '-' ||
                          structura->str[structura->s] == '+'))) {
    structura->error = 1;
    parsing_ok = 0;
  }

  if (parsing_ok) {
    char tmp_str[BUFFER] = {0};
    s21_strncpy(tmp_str, structura->str + structura->s, int_count);
    long int number = strtol(tmp_str, 0, 0);

    if (!structura->starr) {
      structura->ret_count = 1;
      if (structura->length == 'h') {
        *va_arg(*args, short int *) = (short int)number;
      } else if (structura->length == '\0') {
        *va_arg(*args, int *) = (int)number;
      } else if (structura->length == 'l') {
        *va_arg(*args, long int *) = number;
      }
    }
    structura->s += int_count;
  }
}

void reading_p(data *structura, va_list *args) {
  int c_count = 0;
  if (structura->width > 0) {
    c_count = structura->width;
  } else {
    while (str_valid(structura, c_count)) {
      c_count++;
    }
  }
  structura->ret_count = 1;
  char str_ptr[BUFFER] = {0};
  s21_strncpy(str_ptr, structura->str + structura->s, c_count);
  uintptr_t number = strtoull(str_ptr, S21_NULL, 16);
  void **ptr_arg = va_arg(*args, void **);
  *ptr_arg = (void *)number;
  structura->s += c_count;
}

void reading_o(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while (structura->str[structura->s + int_count] != ' ' &&
           structura->str[structura->s + int_count] != '\0') {
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  unsigned long int number = 0;
  number = strtoll(tmp_str, 0, 8);

  structura->ret_count = 1;
  if (structura->length == 'h') {
    *va_arg(*args, unsigned short int *) = (unsigned short int)number;
  } else if (structura->length == '\0') {
    *va_arg(*args, unsigned int *) = (unsigned int)number;
  } else
    *va_arg(*args, unsigned long int *) = number;

  structura->s += int_count;
}

void reading_x(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while (structura->str[structura->s + int_count] != ' ' &&
           structura->str[structura->s + int_count] != '\0') {
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  unsigned long int number = 0;
  number = strtoll(tmp_str, 0, 16);
  if (!structura->starr) {
    structura->ret_count = 1;
    if (structura->length == 'h') {
      *va_arg(*args, unsigned short int *) = (unsigned short int)number;
    } else if (structura->length == '\0') {
      *va_arg(*args, unsigned int *) = (unsigned int)number;
    } else if (structura->length == 'l') {
      *va_arg(*args, unsigned long int *) = number;
    }
  }
  structura->s += int_count;
}

void reading_u(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while (structura->str[structura->s + int_count] >= '0' &&
           structura->str[structura->s + int_count] <= '9') {
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  unsigned long int number = strtol(tmp_str, S21_NULL, 10);
  if (!structura->starr && s21_strlen(tmp_str) > 0) {
    structura->ret_count = 1;
    if (structura->length == 'h') {
      *va_arg(*args, unsigned short int *) = (unsigned short int)number;
    } else if (structura->length == '\0') {
      *va_arg(*args, unsigned int *) = (unsigned int)number;
    } else if (structura->length == 'l') {
      *va_arg(*args, unsigned long int *) = number;
    }
  }
  structura->s += int_count;
}

void reading_f(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while ((structura->str[structura->s + int_count] >= '0' &&
            structura->str[structura->s + int_count] <= '9') ||
           structura->str[structura->s + int_count] == '.') {
      if (structura->str[structura->s + int_count] == '.') {
        structura->fraction = 1;
      }
      if (structura->fraction > 0) structura->fraction++;
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  if (!structura->starr && s21_strlen(tmp_str) > 0) {
    structura->ret_count = 1;
    if (structura->length == 'L') {
      *va_arg(*args, long double *) = strtold(tmp_str, S21_NULL);
    } else {
      *va_arg(*args, float *) = strtold(tmp_str, S21_NULL);
    }
  }
  structura->s += int_count;
}

void reading_d(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while (structura->str[structura->s + int_count] >= '0' &&
           structura->str[structura->s + int_count] <= '9') {
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  long int number = 0;
  number = strtol(tmp_str, S21_NULL, 10);
  if (!structura->starr && s21_strlen(tmp_str) > 0) {
    structura->ret_count = 1;
    if (structura->length == 'h') {
      *va_arg(*args, short int *) = (short int)number;
    } else if (structura->length == '\0') {
      *va_arg(*args, int *) = (int)number;
    } else if (structura->length == 'l') {
      *va_arg(*args, long int *) = number;
    }
  }
  structura->s += int_count;
}

void reading_s(data *structura, va_list *args) {
  int c_count = 0;
  while ((structura->width > 0 ? c_count < structura->width : 1) &&
         str_valid(structura, c_count)) {
    c_count++;
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, c_count);

  if (!structura->starr && tmp_str[0] != '\0') {
    structura->ret_count = 1;
    char *str_ptr = va_arg(*args, char *);
    s21_strncpy(str_ptr, structura->str + structura->s, c_count);
  }
  structura->s += c_count;
}

void reading_c(data *structura, va_list *args) {
  if (!structura->starr) {
    *va_arg(*args, char *) = structura->str[structura->s];
    structura->ret_count = 1;
  }
  structura->s += 1;
}

void reading_e(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while (str_valid(structura, int_count)) {
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  if (!structura->starr) {
    structura->ret_count = 1;
    if (structura->length == 'L') {
      *va_arg(*args, long double *) = strtold(tmp_str, S21_NULL);
    } else {
      *va_arg(*args, float *) = strtold(tmp_str, S21_NULL);
    }
  }
  structura->s += int_count;
}

void reading_g(data *structura, va_list *args) {
  int int_count = 0;
  if (structura->width > 0) {
    while (int_count < structura->width && str_valid(structura, int_count)) {
      int_count++;
    }
  } else {
    while (str_valid(structura, int_count)) {
      int_count++;
    }
  }
  char tmp_str[BUFFER] = {0};
  s21_strncpy(tmp_str, structura->str + structura->s, int_count);
  structura->ret_count = 1;
  if (structura->length == 'L') {
    *va_arg(*args, long double *) = strtold(tmp_str, S21_NULL);
  } else {
    *va_arg(*args, float *) = strtold(tmp_str, S21_NULL);
  }
  structura->s += int_count;
}
