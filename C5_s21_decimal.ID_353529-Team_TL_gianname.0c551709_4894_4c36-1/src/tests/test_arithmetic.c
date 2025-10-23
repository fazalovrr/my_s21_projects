#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_add_error) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{7, 0, 0, 0}};
  ck_assert_int_eq(s21_add(a, b, NULL), ERROR);
}
END_TEST

START_TEST(test_s21_add_simple) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{20, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{30, 0, 0, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_negative) {
  s21_decimal a = {{10, 0, 0, 0x80000000}};
  s21_decimal b = {{20, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{30, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_mixed_sign) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{30, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{20, 0, 0, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_mixed_sign_2) {
  s21_decimal a = {{50, 0, 0, 0x80000000}};
  s21_decimal b = {{30, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{20, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_diff_scale) {
  s21_decimal a = {{15, 0, 0, 0x00010000}};
  s21_decimal b = {{275, 0, 0, 0x00020000}};
  s21_decimal result = {0};
  s21_decimal expected = {{425, 0, 0, 0x00020000}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  ck_assert_int_eq(s21_add(a, b, &result), INF);
}
END_TEST

START_TEST(test_s21_add_antioverflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_carry) {
  s21_decimal a = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{0, 1, 0, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_carry_chain) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{0, 0, 1, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_sign_carry) {
  s21_decimal a = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_add_edge) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_add(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_error) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{7, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, NULL), ERROR);
}
END_TEST

START_TEST(test_s21_sub_simple) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{30, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{20, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_negative_result) {
  s21_decimal a = {{30, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{20, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_negative) {
  s21_decimal a = {{30, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{80, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_negative_2) {
  s21_decimal a = {{30, 0, 0, 0x80000000}};
  s21_decimal b = {{50, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{20, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_negative_3) {
  s21_decimal a = {{30, 0, 0, 0x80000000}};
  s21_decimal b = {{50, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{80, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_diff_scale) {
  s21_decimal a = {{275, 0, 0, 0x00020000}};
  s21_decimal b = {{15, 0, 0, 0x00010000}};
  s21_decimal result = {0};
  s21_decimal expected = {{125, 0, 0, 0x00020000}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_underflow) {
  s21_decimal a = {{0, 0, 0, 0x80000000}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{1, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_max_underflow) {
  s21_decimal a = {{1, 0, 0, 0x80000000}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result = {0};
  ck_assert_int_eq(s21_sub(a, b, &result), N_INF);
}
END_TEST

START_TEST(test_s21_sub_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_sign_switch) {
  s21_decimal a = {{0, 0, 0, 0x80000000}};
  s21_decimal b = {{1, 0, 0, 0x80000000}};
  s21_decimal result = {0};
  s21_decimal expected = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

START_TEST(test_s21_sub_edge) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result = {0};
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_sub(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected), TRUE);
}
END_TEST

Suite *s21_add_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_add");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_add_error);
  tcase_add_test(tc_core, test_s21_add_simple);
  tcase_add_test(tc_core, test_s21_add_negative);
  tcase_add_test(tc_core, test_s21_add_mixed_sign);
  tcase_add_test(tc_core, test_s21_add_mixed_sign_2);
  tcase_add_test(tc_core, test_s21_add_diff_scale);
  tcase_add_test(tc_core, test_s21_add_overflow);
  tcase_add_test(tc_core, test_s21_add_antioverflow);
  tcase_add_test(tc_core, test_s21_add_carry);
  tcase_add_test(tc_core, test_s21_add_carry_chain);
  tcase_add_test(tc_core, test_s21_add_sign_carry);
  tcase_add_test(tc_core, test_s21_add_edge);

  suite_add_tcase(s, tc_core);
  return s;
}

Suite *s21_sub_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_sub");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_sub_error);
  tcase_add_test(tc_core, test_s21_sub_simple);
  tcase_add_test(tc_core, test_s21_sub_negative_result);
  tcase_add_test(tc_core, test_s21_sub_negative);
  tcase_add_test(tc_core, test_s21_sub_negative_2);
  tcase_add_test(tc_core, test_s21_sub_negative_3);
  tcase_add_test(tc_core, test_s21_sub_diff_scale);
  tcase_add_test(tc_core, test_s21_sub_underflow);
  tcase_add_test(tc_core, test_s21_sub_max_underflow);
  tcase_add_test(tc_core, test_s21_sub_zero);
  tcase_add_test(tc_core, test_s21_sub_sign_switch);
  tcase_add_test(tc_core, test_s21_sub_edge);

  suite_add_tcase(s, tc_core);
  return s;
}