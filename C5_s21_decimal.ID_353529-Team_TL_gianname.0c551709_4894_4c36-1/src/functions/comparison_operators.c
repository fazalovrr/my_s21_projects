#include <stdio.h>

#include "../s21_decimal.h"
#include "sup_func.h"

int s21_is_less(s21_decimal value1, s21_decimal value2) {
  int res = TRUE;
  decimal_sign sign1 = get_sign(value1);
  decimal_sign sign2 = get_sign(value2);
  if (sign1 == POSITIVE && sign2 == NEGATIVE) {
    res = FALSE;
  } else if (sign1 == NEGATIVE && sign2 == POSITIVE) {
    res = TRUE;
  } else if (sign1 == ZERO && sign2 == NEGATIVE) {
    res = FALSE;
  } else if (sign1 == ZERO && sign2 == POSITIVE) {
    res = TRUE;
  } else if (sign1 == POSITIVE && sign2 == ZERO) {
    res = FALSE;
  } else if (sign1 == NEGATIVE && sign2 == ZERO) {
    res = TRUE;
  } else if (sign1 == POSITIVE && sign2 == POSITIVE) {
    res = is_less_by_mod(value1, value2);
  } else if (sign1 == NEGATIVE && sign2 == NEGATIVE) {
    res = !is_less_by_mod(value1, value2);
  } else if (sign1 == ZERO && sign2 == ZERO) {
    res = FALSE;
  }
  return res;
}

int s21_is_equal(s21_decimal value1, s21_decimal value2) {
  int res = TRUE;
  decimal_sign sign1 = get_sign(value1);
  decimal_sign sign2 = get_sign(value2);
  if (sign1 == POSITIVE && sign2 == NEGATIVE) {
    res = FALSE;
  } else if (sign1 == NEGATIVE && sign2 == POSITIVE) {
    res = FALSE;
  } else if (sign1 == ZERO && sign2 == NEGATIVE) {
    res = FALSE;
  } else if (sign1 == ZERO && sign2 == POSITIVE) {
    res = FALSE;
  } else if (sign1 == POSITIVE && sign2 == ZERO) {
    res = FALSE;
  } else if (sign1 == NEGATIVE && sign2 == ZERO) {
    res = FALSE;
  } else if (sign1 == POSITIVE && sign2 == POSITIVE) {
    res = is_equal_by_mod(value1, value2);
  } else if (sign1 == NEGATIVE && sign2 == NEGATIVE) {
    res = is_equal_by_mod(value1, value2);
  } else if (sign1 == ZERO && sign2 == ZERO) {
    res = TRUE;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2) {
  return (s21_is_equal(value1, value2) == TRUE ||
          s21_is_less(value1, value2) == TRUE);
}
int s21_is_greater(s21_decimal value1, s21_decimal value2) {
  return (s21_is_equal(value1, value2) == FALSE &&
          s21_is_less(value1, value2) == FALSE);
}
int s21_is_greater_or_equal(s21_decimal value1, s21_decimal value2) {
  return (s21_is_equal(value1, value2) == TRUE ||
          s21_is_less(value1, value2) == FALSE);
}

int s21_is_not_equal(s21_decimal value1, s21_decimal value2) {
  return (!s21_is_equal(value1, value2));
}
