#include <check.h>
#include <stdio.h>

#include "../functions/sup_func.h"
#include "../s21_decimal.h"

START_TEST(test_s21_div_zero) {
  s21_decimal a = {{1, 2, 3, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  s21_decimal result = {{0}};
  ck_assert_int_eq(s21_div(a, b, &result), DIVISION_BY_ZERO);
}
END_TEST

START_TEST(test_s21_div_zero1) {
  s21_decimal b = {{1, 2, 3, 0x00000000}};
  s21_decimal a = {{0, 0, 0, 0x80000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_div_positive) {
  s21_decimal a = {{10, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{5, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_div_negative_result) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{5, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_div_negative_by_negative) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{2, 0, 0, 0x80000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{5, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_div_with_scale) {
  s21_decimal a = {{1000, 0, 0, 0x000A0000}};
  s21_decimal b = {{1, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{1000, 0, 0, 0x000A0000}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_div_rounding) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_not_equal(result, a), TRUE);
}
END_TEST

START_TEST(test_s21_div_overflow) {
  s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x00000000}};
  s21_decimal b = {{1, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  ck_assert_int_eq(s21_div(a, b, &result), OK);
  ck_assert_int_eq(s21_is_not_equal(result, b), TRUE);
}
END_TEST

START_TEST(test_s21_div_null_pointer) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  s21_decimal b = {{1, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_div(a, b, NULL), ERROR);
}
END_TEST

START_TEST(test_div_negative_scale_requires_rounding) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1000, 0, 0, 0}};
  set_scale(&a, 0);
  set_scale(&b, 3);

  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_lt(get_scale(result), MAX_POW);
}
END_TEST

START_TEST(test_div_scale_overflow) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  set_scale(&a, 0);
  set_scale(&b, MAX_POW + 1);

  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_div_bankers_round_tie_true) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_zero_result_sign_reset) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  set_sign(&a, 1);

  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_divmod_error_handling) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 3);
}
END_TEST

START_TEST(test_bankers_round_gt5) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_negative_result) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  a.bits[3] = 1u << 31;
  b.bits[3] = 0;
  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
  ck_assert((result.bits[3] >> 31) & 1);
}
END_TEST

START_TEST(test_s21_div_triggers_u96_add_small_carry_chain) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};

  a.bits[3] = (28 << 16);
  b.bits[3] = (0 << 16);

  s21_decimal result;
  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_1) {
  s21_decimal value_1 = {
      {UINT32_MAX, UINT32_MAX, UINT32_MAX, (28 << 16) | (1u << 31)}};
  s21_decimal value_2 = {{4, 0, 0, (14 << 16)}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_2) {
  s21_decimal value_1 = {{879, 0, 0, (5 << 16) | (1u << 31)}};
  s21_decimal value_2 = {{123456, 0, 0, (6 << 16)}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_3) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, (28 << 16)}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_4) {
  s21_decimal value_1 = {{UINT32_MAX - 1, UINT32_MAX, UINT32_MAX, (28 << 16)}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_5) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_6) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{15, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_8) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, (28 << 16)}};
  s21_decimal value_2 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_div_case_9) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal value_2 = {{1, 0, 0, (14 << 16)}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_div_case_10) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, (1u << 31)}};
  s21_decimal value_2 = {{1, 0, 0, (14 << 16)}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 2);
}
END_TEST

START_TEST(test_div_case_11) {
  s21_decimal value_1 = {{9, 0, 0, (28 << 16)}};
  s21_decimal value_2 = {{99, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_mul10_overflow_positive) {
  s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  a.bits[3] = (28 << 16);

  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_mul10_overflow_negative) {
  s21_decimal a = {
      {UINT32_MAX, UINT32_MAX, UINT32_MAX, (28 << 16) | (1u << 31)}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  int status = s21_div(a, b, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_rounding_on_mul10_overflow_triggers_tie_block) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, (28 << 16)}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result;
  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_rounding_on_mul10_overflow_negative_sign) {
  s21_decimal value_1 = {
      {UINT32_MAX, UINT32_MAX, UINT32_MAX, (28 << 16) | (1u << 31)}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result;
  int status = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(status, 0);
}
END_TEST

Suite *s21_div_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_div");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_div_zero);
  tcase_add_test(tc_core, test_s21_div_zero1);
  tcase_add_test(tc_core, test_s21_div_positive);
  tcase_add_test(tc_core, test_s21_div_negative_result);
  tcase_add_test(tc_core, test_s21_div_negative_by_negative);
  tcase_add_test(tc_core, test_s21_div_with_scale);
  tcase_add_test(tc_core, test_s21_div_rounding);
  tcase_add_test(tc_core, test_s21_div_overflow);
  tcase_add_test(tc_core, test_s21_div_null_pointer);
  tcase_add_test(tc_core, test_div_negative_scale_requires_rounding);
  tcase_add_test(tc_core, test_div_scale_overflow);
  tcase_add_test(tc_core, test_div_bankers_round_tie_true);
  tcase_add_test(tc_core, test_div_zero_result_sign_reset);
  tcase_add_test(tc_core, test_divmod_error_handling);
  tcase_add_test(tc_core, test_divmod_error_handling);
  tcase_add_test(tc_core, test_bankers_round_gt5);
  tcase_add_test(tc_core, test_div_negative_result);
  tcase_add_test(tc_core, test_s21_div_triggers_u96_add_small_carry_chain);
  tcase_add_test(tc_core, test_div_case_1);
  tcase_add_test(tc_core, test_div_case_2);
  tcase_add_test(tc_core, test_div_case_3);
  tcase_add_test(tc_core, test_div_case_4);
  tcase_add_test(tc_core, test_div_case_5);
  tcase_add_test(tc_core, test_div_case_6);
  tcase_add_test(tc_core, test_div_case_8);
  tcase_add_test(tc_core, test_div_case_9);
  tcase_add_test(tc_core, test_div_case_10);
  tcase_add_test(tc_core, test_div_case_11);
  tcase_add_test(tc_core, test_mul10_overflow_positive);
  tcase_add_test(tc_core, test_mul10_overflow_negative);
  tcase_add_test(tc_core, test_rounding_on_mul10_overflow_triggers_tie_block);
  tcase_add_test(tc_core, test_rounding_on_mul10_overflow_negative_sign);

  suite_add_tcase(s, tc_core);

  return s;
}
