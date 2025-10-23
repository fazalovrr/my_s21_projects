#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdint.h>
#define FALSE 0
#define TRUE 1
#define OK 0
#define INF 1
#define N_INF 2
#define DIVISION_BY_ZERO 3
#define ERROR 1

#define MAX_DECIMAL powl(2.0, 96)
#define MIN_DECIMAL -1 * MAX_DECIMAL
#define MAX_POW 28
#define FLOAT_ACCURACY 7

#define START_LOW 0
#define START_MID 32
#define START_HIGHT 64
#define START_INFO 96

typedef struct {
  uint32_t bits[4];
} s21_decimal;

typedef enum { ZERO = 2, NEGATIVE = 1, POSITIVE = 0 } decimal_sign;

typedef struct {
  uint64_t bits[7];
  uint16_t scale;
  decimal_sign sign;
} big_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  // S21_DECIMAL_H
