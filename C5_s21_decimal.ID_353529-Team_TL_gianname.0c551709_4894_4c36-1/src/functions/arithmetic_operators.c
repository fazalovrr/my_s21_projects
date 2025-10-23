#include <stdio.h>

#include "../s21_decimal.h"
#include "sup_func.h"

big_decimal sum_of_positive(big_decimal value_1, big_decimal value_2);
big_decimal sub_of_positive(big_decimal value_1, big_decimal value_2);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }

  int return_code = OK;
  big_decimal big_res;
  clear_decimal(result);

  decimal_sign sign1 = get_sign(value_1);
  decimal_sign sign2 = get_sign(value_2);
  big_decimal big_1 = decimal_to_bigdecimal(value_1);
  big_decimal big_2 = decimal_to_bigdecimal(value_2);

  if (normalize_bigdecimal_scale(&big_1, &big_2)) {
    return_code = INF;
  } else {
    if (sign1 == sign2) {
      big_res = sum_of_positive(big_1, big_2);
      big_res.sign = sign1;
    } else {
      int is_first_smaller = is_less_by_mod(value_1, value_2);

      if (is_first_smaller) {
        big_res = sub_of_positive(big_2, big_1);
        big_res.sign = sign2;
      } else {
        big_res = sub_of_positive(big_1, big_2);
        big_res.sign = sign1;
      }
    }

    if (normalize_bigdecimal_for_decimal(&big_res)) {
      return_code = (big_res.sign == POSITIVE) ? INF : N_INF;
    } else {
      bigdecimal_to_decimal(big_res, result);
    }
  }

  return return_code;
}

big_decimal sum_of_positive(big_decimal value_1, big_decimal value_2) {
  big_decimal big_result = {0};
  for (int i = 0; i < 7; ++i) {
    big_result.bits[i] += value_1.bits[i];
    big_result.bits[i] += value_2.bits[i];
    get_overflow(&big_result);
  }
  big_result.scale = value_1.scale;
  return big_result;
}

big_decimal sub_of_positive(big_decimal value_1, big_decimal value_2) {
  big_decimal big_result = {0};
  int borrow = 0;
  for (int i = 0; i < 7; ++i) {
    big_result.bits[i] = value_1.bits[i] - borrow;
    borrow = 0;
    if (big_result.bits[i] < value_2.bits[i]) {
      borrow = 1;
      big_result.bits[i] |= ((uint64_t)borrow << 32);
    }
    big_result.bits[i] -= value_2.bits[i];
  }
  big_result.scale = value_1.scale;
  return big_result;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return ERROR;
  }

  s21_decimal negated_value_2 = value_2;

  if (get_sign(value_2) == POSITIVE) {
    set_sign(&negated_value_2, NEGATIVE);
  } else {
    set_sign(&negated_value_2, POSITIVE);
  }

  return s21_add(value_1, negated_value_2, result);
}