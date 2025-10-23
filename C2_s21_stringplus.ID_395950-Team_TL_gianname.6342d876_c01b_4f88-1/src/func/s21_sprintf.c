#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_string.h"

typedef struct {
  int minus;
  int plus;
  int space;
  int hash;
  int zero;
  int width;
  int precision;
  char length;
  char specifier;
  int width_star;
  int prec_star;
  int *n_arg;
} s_specifier;

static void init_specifier(s_specifier *spec) {
  spec->minus = spec->plus = spec->space = spec->hash = spec->zero = 0;
  spec->width = spec->precision = 0;
  spec->width_star = spec->prec_star = 0;
  spec->length = spec->specifier = '\0';
  spec->precision = -1;
  spec->n_arg = S21_NULL;
}

static int is_digit(char c) { return (c >= '0' && c <= '9'); }

static long long get_signed_arg(const s_specifier *spec, va_list args) {
  if (spec->length == 'l') return va_arg(args, long);
  if (spec->length == 'h') return (short)va_arg(args, int);
  return va_arg(args, int);
}

static unsigned long long get_unsigned_arg(const s_specifier *spec,
                                           va_list args) {
  if (spec->length == 'l') return va_arg(args, unsigned long);
  if (spec->length == 'h') return (unsigned short)va_arg(args, unsigned int);
  return va_arg(args, unsigned int);
}

static long double get_float_arg(const s_specifier *spec, va_list args) {
  if (spec->length == 'L') {
    return va_arg(args, long double);
  } else {
    return (long double)va_arg(args, double);
  }
}

static void reverse_string(char *str, const int length) {
  for (int i = 0, j = length - 1; i < j; i++, j--) {
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  }
}

static int int_to_str(long long num, char *str, const int base) {
  const char *digits = "0123456789abcdef";
  int i = 0, negative = num < 0;

  if (negative) num = -num;
  do {
    str[i++] = digits[num % base];
    num /= base;
  } while (num != 0);

  if (negative) str[i++] = '-';
  reverse_string(str, i);
  str[i] = '\0';
  return i;
}

static int unsigned_to_str(unsigned long long num, char *str, const int base,
                           int uppercase) {
  const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  int i = 0;

  do {
    str[i++] = digits[num % base];
    num /= base;
  } while (num != 0);

  reverse_string(str, i);
  str[i] = '\0';
  return i;
}

static void add_prefix(char *buf, int *len, const char *prefix,
                       const int prefix_len) {
  for (int i = *len; i >= 0; i--) buf[i + prefix_len] = buf[i];
  for (int i = 0; i < prefix_len; i++) buf[i] = prefix[i];
  *len += prefix_len;
}

static void apply_precision(char *buf, const s_specifier *spec, int *len) {
  if (spec->precision > *len) {
    int is_negative = (buf[0] == '-');
    int num_start = is_negative ? 1 : 0;

    for (int i = *len; i >= num_start; i--)
      buf[i + spec->precision - (*len - num_start)] = buf[i];

    for (int i = num_start;
         i < num_start + spec->precision - (*len - num_start); i++)
      buf[i] = '0';

    *len += spec->precision - (*len - num_start);
  }
}

static void apply_width(char **str, const s_specifier *spec, const char *buf,
                        int len) {
  int padding = spec->width - len;
  char pad_char =
      (spec->zero && !spec->minus && spec->precision == -1) ? '0' : ' ';

  if (spec->zero && !spec->minus && spec->precision == -1) {
    if (buf[0] == '-' || buf[0] == '+' || buf[0] == ' ') {
      *(*str)++ = buf[0];
      padding--;
      len--;
      buf++;
    }
  }

  if (!spec->minus && padding > 0) {
    while (padding-- > 0) *(*str)++ = pad_char;
  }
  for (int i = 0; i < len; i++) *(*str)++ = buf[i];

  if (spec->minus && padding > 0) {
    while (padding-- > 0) *(*str)++ = ' ';
  }
}

static void process_char(char **str, const s_specifier *spec, va_list args) {
  const char buf[2] = {(char)va_arg(args, int), '\0'};
  apply_width(str, spec, buf, 1);
}

static void process_string(char **str, const s_specifier *spec, va_list args) {
  const char *s = va_arg(args, char *);
  if (!s) s = "(null)";

  int len = s21_strlen(s);
  if (spec->precision != -1 && spec->precision < len) len = spec->precision;

  apply_width(str, spec, s, len);
}

static void process_int(char **str, const s_specifier *spec, va_list args) {
  char buf[50];
  long long num = get_signed_arg(spec, args);
  int len = int_to_str(num, buf, 10);

  if (spec->precision == 0 && num == 0) {
    buf[0] = '\0';
    len = 0;
  } else {
    apply_precision(buf, spec, &len);
  }

  if (num >= 0) {
    if (spec->plus) {
      add_prefix(buf, &len, "+", 1);
    } else if (spec->space) {
      add_prefix(buf, &len, " ", 1);
    }
  }

  if (spec->zero && !spec->minus && spec->precision == -1) {
    int sign_len = (buf[0] == '+' || buf[0] == '-' || buf[0] == ' ') ? 1 : 0;
    int padding = spec->width - len;

    if (padding > 0) {
      for (int i = len; i >= sign_len; i--) buf[i + padding] = buf[i];

      for (int i = sign_len; i < sign_len + padding; i++) buf[i] = '0';

      len += padding;
    }
  }

  apply_width(str, spec, buf, len);
}

static void process_unsigned(char **str, const s_specifier *spec,
                             va_list args) {
  int base = 10;
  int uppercase = 0;

  switch (spec->specifier) {
    case 'o':
      base = 8;
      break;
    case 'x':
      base = 16;
      uppercase = 0;
      break;
    case 'X':
      base = 16;
      uppercase = 1;
      break;
    default:
      break;
  }

  char buf[50];
  unsigned long long num = get_unsigned_arg(spec, args);
  int len = unsigned_to_str(num, buf, base, uppercase);

  if (spec->precision == 0 && num == 0 && !spec->hash) {
    buf[0] = '\0';
    len = 0;
  } else {
    apply_precision(buf, spec, &len);
  }

  if (spec->hash && num != 0) {
    if (spec->specifier == 'x') {
      add_prefix(buf, &len, "0x", 2);
    } else if (spec->specifier == 'X') {
      add_prefix(buf, &len, "0X", 2);
    } else if (spec->specifier == 'o') {
      add_prefix(buf, &len, "0", 1);
    }
  }

  apply_width(str, spec, buf, len);
}

static void process_pointer(char **str, const s_specifier *spec, va_list args) {
  void *ptr = va_arg(args, void *);
  char buf[20] = "(nil)";
  int len = 5;

  if (ptr != S21_NULL) {
    len = unsigned_to_str((unsigned long long)ptr, buf, 16, 0);
    add_prefix(buf, &len, "0x", 2);
  }

  apply_width(str, spec, buf, len);
}

static void process_percent(char **str, const s_specifier *spec) {
  const char buf[2] = {'%', '\0'};
  apply_width(str, spec, buf, 1);
}

static int float_to_str(long double num, char *str, int precision) {
  int negative = num < 0;
  if (negative) num = -num;

  long long int_part = (long long)num;
  long double frac_part = num - int_part;
  int len = int_to_str(int_part, str, 10);

  if (precision > 0 || (precision == 0 && frac_part >= 0.5)) {
    str[len++] = '.';
    for (int i = 0; i < precision; i++) {
      frac_part *= 10;
      int digit = (int)frac_part;
      str[len++] = '0' + digit;
      frac_part -= digit;
    }

    if (frac_part >= 0.5) {
      int dot_pos = -1;
      for (int i = 0; i < len; i++) {
        if (str[i] == '.' && dot_pos == -1) {
          dot_pos = i;
        }
      }
      int i = len - 1;
      while (i > dot_pos && str[i] == '9') {
        str[i--] = '0';
      }

      if (i > dot_pos) {
        str[i]++;
      } else {
        int j = dot_pos - 1;
        while (j >= 0 && str[j] == '9') {
          str[j--] = '0';
        }
        if (j >= 0) {
          str[j]++;
        } else {
          add_prefix(str, &len, "1", 1);
        }
      }
    }
  }

  if (negative) {
    add_prefix(str, &len, "-", 1);
  }

  str[len] = '\0';
  return len;
}

static void remove_trailing_zeros(char *buf, int *len) {
  const char *dot = s21_strchr(buf, '.');

  int i = *len - 1;
  while (i > (dot - buf) && buf[i] == '0') i--;
  if (buf[i] == '.') i--;
  buf[i + 1] = '\0';
  *len = i + 1;
}

static int scientific_to_str(long double num, char *buf,
                             const s_specifier *spec, int precision,
                             int uppercase) {
  int negative = (num < 0);
  if (negative) num = -num;
  int idx = 0;
  if (num == 0.0) {
    buf[idx++] = '0';
    if (precision > 0) {
      buf[idx++] = '.';
      for (int i = 0; i < precision; i++) {
        buf[idx++] = '0';
      }
    }
    const char *exp_str = uppercase ? "E+00" : "e+00";
    for (int i = 0; exp_str[i]; i++) {
      buf[idx++] = exp_str[i];
    }
    buf[idx] = '\0';
  } else {
    int exponent = 0;
    long double num_normalized = num;
    {
      while (num_normalized >= 10.0) {
        num_normalized /= 10.0;
        exponent++;
      }
      while (num_normalized < 1.0) {
        num_normalized *= 10.0;
        exponent--;
      }
    }

    long double factor = powl(10, precision);
    num_normalized = roundl(num_normalized * factor) / factor;

    if (num_normalized >= 10.0) {
      num_normalized /= 10.0;
      exponent++;
    }

    long long int_part = (long long)num_normalized;
    long double fractional = num_normalized - int_part;
    long long frac_int = (long long)roundl(fractional * factor);

    buf[idx++] = '0' + int_part;

    if (precision > 0) {
      buf[idx++] = '.';
      if (frac_int == 0) {
        for (int i = 0; i < precision; i++) {
          buf[idx++] = '0';
        }
      } else {
        char temp_buf[30];
        int frac_len = 0;
        long long temp = frac_int;
        do {
          temp_buf[frac_len++] = '0' + (temp % 10);
          temp /= 10;
        } while (temp);

        for (int i = frac_len - 1; i >= 0; i--) {
          buf[idx++] = temp_buf[i];
        }
      }
    }
    if (spec->specifier == 'g' || spec->specifier == 'G') {
      remove_trailing_zeros(buf, &idx);
    }
    buf[idx++] = uppercase ? 'E' : 'e';
    buf[idx++] = exponent < 0 ? '-' : '+';

    int exp_abs = exponent < 0 ? -exponent : exponent;

    char exp_buf[10];
    int exp_len = 0;

    int temp = exp_abs;
    do {
      exp_buf[exp_len++] = '0' + (temp % 10);
      temp /= 10;
    } while (temp);

    if (exp_len == 1) {
      exp_buf[exp_len++] = '0';
    }

    for (int i = 0; i < exp_len / 2; i++) {
      char t = exp_buf[i];
      exp_buf[i] = exp_buf[exp_len - 1 - i];
      exp_buf[exp_len - 1 - i] = t;
    }

    for (int i = 0; i < exp_len; i++) {
      buf[idx++] = exp_buf[i];
    }

    buf[idx] = '\0';

    if (negative) {
      add_prefix(buf, &idx, "-", 1);
    }
  }
  return idx;
}

static int general_to_str(long double num, char *buf, const s_specifier *spec,
                          int precision, int uppercase) {
  if (precision < 1) precision = 1;
  int len = 0;
  if (num == 0.0) {
    if (signbit(num)) {
      s21_strncpy(buf, "-0", 3);
      len = 2;
    } else {
      s21_strncpy(buf, "0", 2);
      len = 1;
    }
  } else {
    int exponent;
    if (fabsl(num) < 1e-308) {
      exponent = (int)floorl(log10l(fabsl(num) * 1e308)) - 308;
    } else {
      exponent = (int)floorl(log10l(fabsl(num)));
    }
    int use_scientific = (exponent < -4 || exponent >= precision);

    if (use_scientific) {
      len = scientific_to_str(num, buf, spec, precision - 1, uppercase);
    } else {
      len = float_to_str(num, buf,
                         precision - 1 - ((exponent > 0) ? exponent : 0));
      remove_trailing_zeros(buf, &len);
    }
  }
  return len;
}

static void process_special_float(char *buf, int *len, const s_specifier *spec,
                                  int is_nan, int is_negative) {
  const char *str_value;
  int use_upper = (spec->specifier == 'F' || spec->specifier == 'E' ||
                   spec->specifier == 'G');

  if (is_nan) {
    str_value = use_upper ? "NAN" : "nan";
    if (is_negative) {
      s21_strncpy(buf, "-", 2);
      s21_strncpy(buf + 1, str_value, 4);
      *len = 4;
    } else {
      s21_strncpy(buf, str_value, 4);
      *len = 3;
    }
  } else {
    str_value = use_upper ? "INF" : "inf";
    if (is_negative) {
      s21_strncpy(buf, "-", 2);
      s21_strncpy(buf + 1, str_value, 4);
      *len = 4;
    } else {
      s21_strncpy(buf, str_value, 4);
      *len = 3;
      add_prefix(buf, len, " ", 1);
    }
  }
}

static void process_float(char **str, const s_specifier *spec, va_list args) {
  long double num = get_float_arg(spec, args);
  int max_len = 32 + spec->precision;
  char *buf = calloc(max_len + 1, sizeof(char));
  if (!buf) return;

  int len = 0;

  if (isnan(num)) {
    process_special_float(buf, &len, spec, 1, signbit(num));
  } else if (isinf(num)) {
    process_special_float(buf, &len, spec, 0, signbit(num));
  } else {
    switch (spec->specifier) {
      case 'f':
      case 'F':
        len =
            float_to_str(num, buf, spec->precision == -1 ? 6 : spec->precision);
        if (spec->hash && spec->precision == 0) {
          buf[len++] = '.';
          buf[len] = '\0';
        }
        if (num >= 0) {
          if (spec->plus) {
            add_prefix(buf, &len, "+", 1);
          } else if (spec->space) {
            add_prefix(buf, &len, " ", 1);
          }
        }
        break;
      case 'e':
      case 'E':
        len = scientific_to_str(num, buf, spec,
                                spec->precision == -1 ? 6 : spec->precision,
                                spec->specifier == 'E');
        if (num >= 0) {
          if (spec->plus) {
            add_prefix(buf, &len, "+", 1);
          } else if (spec->space) {
            add_prefix(buf, &len, " ", 1);
          }
        }
        break;
      case 'g':
      case 'G':
        len = general_to_str(num, buf, spec,
                             spec->precision == -1 ? 6 : spec->precision,
                             spec->specifier == 'G');
        if (num >= 0) {
          if (spec->plus) {
            add_prefix(buf, &len, "+", 1);
          } else if (spec->space) {
            add_prefix(buf, &len, " ", 1);
          }
        }
        break;
    }
  }

  apply_width(str, spec, buf, len);
  free(buf);
}

static const char *parse_format(const char *format, s_specifier *spec,
                                va_list args) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' ||
         *format == '0') {
    switch (*format++) {
      case '-':
        spec->minus = 1;
        break;
      case '+':
        spec->plus = 1;
        break;
      case ' ':
        spec->space = 1;
        break;
      case '#':
        spec->hash = 1;
        break;
      case '0':
        spec->zero = 1;
        break;
    }
  }

  if (*format == '*') {
    spec->width = va_arg(args, int);
    if (spec->width < 0) {
      spec->minus = 1;
      spec->width = -spec->width;
    }
    spec->width_star = 1;
    format++;
  } else {
    while (is_digit(*format))
      spec->width = spec->width * 10 + (*format++ - '0');
  }

  if (*format == '.') {
    format++;
    if (*format == '*') {
      spec->precision = va_arg(args, int);
      spec->prec_star = 1;
      format++;
    } else {
      spec->precision = 0;
      while (is_digit(*format))
        spec->precision = spec->precision * 10 + (*format++ - '0');
    }
  }

  if (*format == 'h' || *format == 'l' || *format == 'L')
    spec->length = *format++;

  if (*format == 'n') {
    spec->n_arg = va_arg(args, int *);
    spec->specifier = *format++;
    return format;
  }

  if (*format) spec->specifier = *format++;

  return format;
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *start = str;

  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
    } else {
      const char *old_format = format;
      s_specifier spec;
      init_specifier(&spec);
      format = parse_format(format + 1, &spec, args);

      if (spec.specifier == 'n') {
        if (spec.n_arg != S21_NULL) *spec.n_arg = str - start;
      } else {
        switch (spec.specifier) {
          case 'c':
            process_char(&str, &spec, args);
            break;
          case 's':
            process_string(&str, &spec, args);
            break;
          case 'd':
          case 'i':
            process_int(&str, &spec, args);
            break;
          case 'u':
          case 'o':
          case 'x':
          case 'X':
            process_unsigned(&str, &spec, args);
            break;
          case 'p':
            process_pointer(&str, &spec, args);
            break;
          case '%':
            process_percent(&str, &spec);
            break;
          case 'f':
          case 'F':
          case 'e':
          case 'E':
          case 'g':
          case 'G':
            process_float(&str, &spec, args);
            break;
          default: {
            if (*old_format == '%' && *format == '\000') *str-- = '\0';
            break;
          }
        }
      }
    }
  }

  *str = '\0';
  va_end(args);
  return (str - start);
}