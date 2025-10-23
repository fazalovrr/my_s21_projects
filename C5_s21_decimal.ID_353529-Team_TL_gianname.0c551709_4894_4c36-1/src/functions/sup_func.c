#include "sup_func.h"

#include <stdio.h>
#define MAX 0x00000000ffffffff
#define MINUS 0x80000000

void print_big_dec(big_decimal val);

int is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

void clear_decimal(s21_decimal *dec) {
  for (int i = 0; i < 4; i++) dec->bits[i] = 0;
}

decimal_sign get_sign(s21_decimal value) {
  decimal_sign flag = (value.bits[3] & MINUS) ? NEGATIVE : POSITIVE;
  if (is_zero(value) == 1) {
    flag = ZERO;
  }
  return flag;
}

void set_sign(s21_decimal *dec, int sign) {
  if (sign)
    dec->bits[3] |= (1U << 31);
  else
    dec->bits[3] &= ~(1U << 31);
}

int get_scale(s21_decimal value) {
  int sc = (int)((value.bits[3] & (~MINUS)) >> 16);
  return sc;
}

void set_scale(s21_decimal *dst, int scale) {
  int sign = get_sign(*dst);
  dst->bits[3] = 0;
  dst->bits[3] = ((uint32_t)scale << 16) | ((uint32_t)sign << 31);
}

big_decimal decimal_to_bigdecimal(s21_decimal src) {
  big_decimal dst = {0};
  dst.bits[0] = (uint64_t)src.bits[0];
  dst.bits[1] = (uint64_t)src.bits[1];
  dst.bits[2] = (uint64_t)src.bits[2];
  dst.scale = get_scale(src);
  dst.sign = get_sign(src);
  return dst;
}

int get_overflow(big_decimal *src) {
  unsigned int overflow = 0;
  big_decimal tmp = *src;
  for (int i = 0; i < 7; ++i) {
    tmp.bits[i] += (uint64_t)overflow;
    overflow = (unsigned int)(tmp.bits[i] >> 32);
    tmp.bits[i] &= MAX;
  }
  int error_flag = 0;
  if (overflow == 0) {
    *src = tmp;
  } else {
    error_flag = 1;
  }
  return error_flag;
}

int pointleft(big_decimal *src) {
  big_decimal tmp = *src;
  int error_flag = 0;
  for (int i = 0; i < 7; ++i) {
    tmp.bits[i] *= 10;
  }
  if (get_overflow(&tmp) != 0) {
    error_flag = 1;
  } else {
    *src = tmp;
    src->scale += 1;
  }
  return error_flag;
}

int pointright(big_decimal *src) {
  unsigned int remainder = 0;
  for (int i = 6; i >= 0; i--) {
    src->bits[i] = ((uint64_t)remainder << 32) + src->bits[i];
    remainder = src->bits[i] % 10;
    src->bits[i] /= 10;
  }
  --src->scale;
  return remainder;
}

void round_bigdecimal(big_decimal *src, int remainder, int flag_remainder) {
  if (remainder > 5) {
    src->bits[0] += 1;
  } else if (remainder == 5) {
    if ((flag_remainder > 1) || (src->bits[0] % 2 == 1)) {
      src->bits[0] += 1;
    }
  }
}

int is_zero_for_normalize(big_decimal value) {
  return (value.bits[6] == 0 && value.bits[5] == 0 && value.bits[4] == 0 &&
          value.bits[3] == 0);
}

int normalize_bigdecimal_for_decimal(big_decimal *src) {
  int remainder = 0, flag_remainder = 0;
  int error_flag = 0;
  while ((src->scale > 0 &&
          (get_overflow(src) == 1 || is_zero_for_normalize(*src) != 1)) ||
         src->scale > 28) {
    remainder = pointright(src);
    if (remainder != 0) {
      ++flag_remainder;
    }
  }

  if (src->scale == 0 &&
      (get_overflow(src) == 1 || is_zero_for_normalize(*src) != 1)) {
    error_flag = 1;
  } else if (remainder != 0 || flag_remainder != 0) {
    round_bigdecimal(src, remainder, flag_remainder);
    error_flag = (get_overflow(src) == 0) ? error_flag : 1;
  } else {
    while (remainder == 0 && src->scale > 0) {
      big_decimal tmp = *src;
      remainder = pointright(&tmp);
      if (remainder == 0) {
        *src = tmp;
      }
    }
  }
  // проверить чтобы степень не была больше 28 (как будто тут и не надо)
  return error_flag;
}

int normalize_bigdecimal_scale(big_decimal *value_1, big_decimal *value_2) {
  uint16_t scale = 0;
  int error_flag = 0;
  if (value_1->scale < value_2->scale) {
    scale = value_2->scale;
  } else {
    scale = value_1->scale;
  }
  while (value_2->scale != scale) {
    error_flag = pointleft(value_2);
  }
  while (value_1->scale != scale) {
    error_flag = pointleft(value_1);
  }
  return error_flag;
}

void bigdecimal_to_decimal(big_decimal src, s21_decimal *dst) {
  dst->bits[0] = (uint32_t)src.bits[0];
  dst->bits[1] = (uint32_t)src.bits[1];
  dst->bits[2] = (uint32_t)src.bits[2];
  dst->bits[3] = 0;
  dst->bits[3] = ((uint32_t)src.scale << 16) | ((uint32_t)src.sign << 31);
}

int is_less_or_equal_by_mod(s21_decimal value_1, s21_decimal value_2) {
  big_decimal big_value1 = decimal_to_bigdecimal(value_1);
  big_decimal big_value2 = decimal_to_bigdecimal(value_2);
  normalize_bigdecimal_scale(&big_value1, &big_value2);
  int res = 9999;
  for (int i = 6; i >= 0 && res == 9999; i--) {
    if (big_value1.bits[i] < big_value2.bits[i]) {
      res = TRUE;
    } else if (big_value1.bits[i] > big_value2.bits[i]) {
      res = FALSE;
    }
  }
  return res;
}

int is_less_by_mod(s21_decimal value_1, s21_decimal value_2) {
  int res = FALSE;
  if (is_less_or_equal_by_mod(value_1, value_2) == TRUE) {
    res = TRUE;
  }
  return res;
}

int is_equal_by_mod(s21_decimal value_1, s21_decimal value_2) {
  int res = FALSE;
  if (is_less_or_equal_by_mod(value_1, value_2) == 9999) {
    res = TRUE;
  }
  return res;
}
