#include "../s21_decimal.h"
#include "sup_func.h"

big_decimal mull_big_decimal(big_decimal big_1, big_decimal big_2);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }

  int return_code = OK;
  big_decimal big_1 = decimal_to_bigdecimal(value_1);
  big_decimal big_2 = decimal_to_bigdecimal(value_2);
  big_decimal big_res = mull_big_decimal(big_1, big_2);

  if (normalize_bigdecimal_for_decimal(&big_res) == 1) {
    if (big_res.sign == POSITIVE) {
      return_code = INF;
    } else if (big_res.sign == NEGATIVE) {
      return_code = N_INF;
    }
  }

  if (return_code == OK) {
    bigdecimal_to_decimal(big_res, result);
  }

  return return_code;
}

big_decimal mull_big_decimal(big_decimal big_1, big_decimal big_2) {
  big_decimal result = {0};
  if (big_1.sign == ZERO || big_2.sign == ZERO) {
    result.sign = ZERO;
  } else if (big_1.sign == big_2.sign) {
    result.sign = POSITIVE;
  } else {
    result.sign = NEGATIVE;
  }

  result.bits[0] = big_1.bits[0] * big_2.bits[0];
  result.bits[1] += (big_1.bits[1] * big_2.bits[0]);
  get_overflow(&result);
  result.bits[1] += (big_1.bits[0] * big_2.bits[1]);
  get_overflow(&result);
  result.bits[2] += (big_1.bits[2] * big_2.bits[0]);
  get_overflow(&result);
  result.bits[2] += (big_1.bits[1] * big_2.bits[1]);
  get_overflow(&result);
  result.bits[2] += (big_1.bits[0] * big_2.bits[2]);
  get_overflow(&result);
  result.bits[3] += (big_1.bits[2] * big_2.bits[1]);
  get_overflow(&result);
  result.bits[3] += (big_1.bits[1] * big_2.bits[2]);
  get_overflow(&result);
  result.bits[4] += (big_1.bits[2] * big_2.bits[2]);
  result.scale = big_1.scale + big_2.scale;
  return result;
}
