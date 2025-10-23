#include "../s21_decimal.h"
#include "sup_func.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }
  big_decimal big = decimal_to_bigdecimal(value);
  int sign = big.sign;
  int remainder_exists = 0;
  while (big.scale > 0) {
    int rem = pointright(&big);
    if (rem != 0) {
      remainder_exists = 1;
    }
  }
  if (sign == NEGATIVE && remainder_exists) {
    big_decimal one = {0};
    one.bits[0] = 1;
    big = sum_of_positive(big, one);
  }
  big.sign = sign;
  bigdecimal_to_decimal(big, result);
  return OK;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }
  *result = value;
  set_sign(result, !get_sign(value));
  return OK;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }
  big_decimal big = decimal_to_bigdecimal(value);
  int sign = big.sign;
  big.sign = POSITIVE;
  if (big.scale == 0) {
    big.sign = sign;
    bigdecimal_to_decimal(big, result);
    return OK;
  }
  int first_remainder = 0;

  while (big.scale > 1) {
    pointright(&big);
  }
  first_remainder = pointright(&big);
  int round_up = 0;
  if (first_remainder > 4) {
    round_up = 1;
  }
  if (round_up) {
    big_decimal one = {0};
    one.bits[0] = 1;
    big = sum_of_positive(big, one);
  }
  big.sign = sign;
  bigdecimal_to_decimal(big, result);
  return OK;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }
  big_decimal big = decimal_to_bigdecimal(value);
  while (big.scale > 0) {
    pointright(&big);
  }
  bigdecimal_to_decimal(big, result);
  return OK;
}