#include <check.h>
#include <stdio.h>

#include "../s21_decimal.h"

Suite *s21_add_suite();
Suite *s21_sub_suite();
Suite *s21_mul_suite();
Suite *s21_div_suite();

Suite *s21_is_less_suite();
Suite *s21_is_less_or_equal_suite();
Suite *s21_is_greater_suite();
Suite *s21_is_greater_or_equal_suite();
Suite *s21_is_equal_suite();
Suite *s21_is_not_equal_suite();

Suite *s21_from_int_to_decimal_suite();
Suite *s21_from_float_to_decimal_suite();
Suite *s21_from_decimal_to_int_suite();
Suite *s21_from_decimal_to_float_suite();

Suite *s21_floor_suite();
Suite *s21_round_suite();
Suite *s21_truncate_suite();
Suite *s21_negate_suite();

int main(void) {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(s21_mul_suite());
  srunner_add_suite(sr, s21_add_suite());
  srunner_add_suite(sr, s21_sub_suite());
  srunner_add_suite(sr, s21_div_suite());

  srunner_add_suite(sr, s21_is_less_suite());
  srunner_add_suite(sr, s21_is_less_or_equal_suite());
  srunner_add_suite(sr, s21_is_greater_suite());
  srunner_add_suite(sr, s21_is_greater_or_equal_suite());
  srunner_add_suite(sr, s21_is_equal_suite());
  srunner_add_suite(sr, s21_is_not_equal_suite());

  srunner_add_suite(sr, s21_from_int_to_decimal_suite());
  srunner_add_suite(sr, s21_from_float_to_decimal_suite());
  srunner_add_suite(sr, s21_from_decimal_to_int_suite());
  srunner_add_suite(sr, s21_from_decimal_to_float_suite());

  srunner_add_suite(sr, s21_floor_suite());
  srunner_add_suite(sr, s21_round_suite());
  srunner_add_suite(sr, s21_truncate_suite());
  srunner_add_suite(sr, s21_negate_suite());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}