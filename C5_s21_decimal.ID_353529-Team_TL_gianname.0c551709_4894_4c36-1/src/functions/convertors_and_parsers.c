#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_decimal.h"
#include "sup_func.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int code = OK;
  if (!dst) {
    code = ERROR;
  } else {
    clear_decimal(dst);
    int sign = (src < 0);
    if (sign) src = -src;
    dst->bits[0] = src;
    if (sign) dst->bits[3] |= (1u << 31);
  }
  return code;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int code = OK;
  if (!dst) {
    code = ERROR;
  } else {
    int sign = get_sign(src);
    s21_decimal value;
    clear_decimal(&value);
    s21_truncate(src, &value);

    int overflow = 0;
    overflow |= (value.bits[2] | value.bits[1]) != 0;
    overflow |= (sign == NEGATIVE && value.bits[0] > (uint32_t)INT_MAX + 1);
    overflow |= (sign == POSITIVE && value.bits[0] > (uint32_t)INT_MAX);

    if (overflow) {
      code = ERROR;
    } else {
      *dst = (int)value.bits[0];
      if (sign) *dst = -*dst;
    }
  }
  return code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int code = OK;
  if (!dst) {
    code = ERROR;
  } else {
    int sign = get_sign(src);
    int scale = get_scale(src);

    long double value = 0.0L;
    for (int i = 0; i < 3; i++) {
      value += (long double)(uint32_t)src.bits[i] * powl(2.0L, 32.0L * i);
    }

    if (scale > 0) value /= powl(10.0L, (long double)scale);
    if (sign) value = -value;

    *dst = (float)value;
  }
  return code;
}

static void dec_zero(s21_decimal *d) {
  d->bits[0] = 0;
  d->bits[1] = 0;
  d->bits[2] = 0;
  d->bits[3] = 0;
}

static void dec_set_sign_scale(s21_decimal *d, int sign, int scale) {
  uint32_t v = 0u;
  if (sign) v |= 0x80000000u;
  v |= ((uint32_t)scale & 0xFFu) << 16;
  d->bits[3] = (int)v;
}

static int dec_mul10(s21_decimal *d) {
  uint64_t c = 0;
  for (int i = 0; i < 3; ++i) {
    uint64_t cur = (uint64_t)(uint32_t)d->bits[i] * 10u + c;
    d->bits[i] = (uint32_t)cur;
    c = cur >> 32;
  }
  return c ? 1 : 0;
}

static int dec_add_small(s21_decimal *d, uint32_t add) {
  uint64_t cur = (uint64_t)(uint32_t)d->bits[0] + add;
  d->bits[0] = (uint32_t)cur;
  return (int)(cur >> 32);  // 0 или 1
}

static int dec_mul_pow10(s21_decimal *d, int k) {
  int overflow = 0;
  int i = 0;
  while (i < k && !overflow) {
    overflow = dec_mul10(d);
    i++;
  }
  return overflow ? 1 : 0;
}

static uint32_t dec_div10_get_remainder(s21_decimal *d) {
  uint64_t rem = 0;
  for (int i = 2; i >= 0; --i) {
    uint64_t cur = (rem << 32) | (uint32_t)d->bits[i];
    uint32_t q = (uint32_t)(cur / 10u);
    rem = cur % 10u;
    d->bits[i] = (int)q;
  }
  return (uint32_t)rem;
}

static void dec_div_pow10(s21_decimal *d, int k, uint32_t *rem10k,
                          uint32_t *pow10k) {
  static const uint32_t P10[10] = {1u,         10u,        100u,     1000u,
                                   10000u,     100000u,    1000000u, 10000000u,
                                   100000000u, 1000000000u};
  uint64_t R = 0;
  uint64_t P = 1;
  int i = 0;
  while (i < k) {
    uint32_t r = dec_div10_get_remainder(d);
    R += (uint64_t)r * P;
    P *= 10u;
    i++;
  }
  if (rem10k) *rem10k = (uint32_t)R;
  if (pow10k) *pow10k = (k >= 0 && k < 10) ? P10[k] : 0u;
}

static int dec_reduce_scale_with_round(s21_decimal *d, int *scale, int extra) {
  int err = 0;
  uint32_t R = 0, P = 0;
  s21_decimal q = *d;

  dec_div_pow10(&q, extra, &R, &P);

  uint64_t twiceR = (uint64_t)R * 2u;
  int round_up = (P == 0u) ? 0 : (twiceR >= (uint64_t)P);

  if (round_up) {
    if (dec_add_small(&q, 1u)) {
      err = 1;
    }
  }

  if (!err) {
    *d = q;
    *scale -= extra;
  }
  return err ? 1 : 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst || isnan(src) || isinf(src)) return ERROR;

  dec_zero(dst);
  if (src == 0.0f) return OK;

  float abs_val = fabsf(src);
  if (abs_val < 1e-28f || abs_val > MAX_DECIMAL) return ERROR;

  int sign = (src < 0.0f);
  char buf[64];
  snprintf(buf, sizeof(buf), "%.7g", (double)abs_val);

  char *e_ptr = strchr(buf, 'e');
  if (!e_ptr) e_ptr = strchr(buf, 'E');

  int exp10 = 0;
  if (e_ptr) {
    exp10 = atoi(e_ptr + 1);
    *e_ptr = '\0';
  }

  char mant[16] = {0};
  int dec_digits = 0;
  for (char *p = buf, *m = mant; *p; p++) {
    if (*p == '.') {
      for (const char *q = p + 1; *q; q++) dec_digits++;
    } else if (*p >= '0' && *p <= '9') {
      *m++ = *p;
    }
  }

  int mant_len = (int)strlen(mant);
  int first_nonzero = 0;
  while (mant[first_nonzero] == '0') first_nonzero++;
  if (first_nonzero > 0) {
    memmove(mant, mant + first_nonzero, mant_len - first_nonzero + 1);
    mant_len -= first_nonzero;
  }

  int final_exp = exp10 - dec_digits;
  s21_decimal value;
  dec_zero(&value);

  for (int i = 0; i < mant_len; i++) {
    if (dec_mul10(&value)) return ERROR;
    if (dec_add_small(&value, (uint32_t)(mant[i] - '0'))) return ERROR;
  }

  int scale = 0;
  if (final_exp >= 0) {
    if (dec_mul_pow10(&value, final_exp)) return ERROR;
  } else {
    scale = -final_exp;
    if (scale > 28) {
      int extra = scale - 28;
      if (dec_reduce_scale_with_round(&value, &scale, extra)) return ERROR;
    }
  }

  dst->bits[0] = value.bits[0];
  dst->bits[1] = value.bits[1];
  dst->bits[2] = value.bits[2];
  dec_set_sign_scale(dst, sign, scale);

  return OK;
}
