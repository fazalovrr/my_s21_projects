#include <check.h>
#include <stdio.h>

#include "../s21_decimal.h"

START_TEST(test_s21_mul_zero) {
  s21_decimal a = {{1, 2, 3, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_mul(a, b, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_base) {
  s21_decimal a = {{0x000007D0, 0, 0, 0x00030000}};
  s21_decimal b = {{0x00000014, 0, 0, 0x00010000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0x00009C40, 0, 0, 0x00040000}};
  ck_assert_int_eq(s21_mul(a, b, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_1) {
  s21_decimal a = {{0x00000001, 0x00000001, 0, 0}};
  s21_decimal b = {{0x00000001, 0x00000001, 0, 0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0x00000001, 0x00000002, 0x00000001, 0}};
  ck_assert_int_eq(s21_mul(a, b, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_multi_bits_1) {
  s21_decimal a = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal b = {{0xFFFFFFFF, 0, 0, 0x80000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0x00000001, 0xFFFFFFFE, 0, 0x80000000}};
  ck_assert_int_eq(s21_mul(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_multi_bits_2) {
  s21_decimal a = {{0xFFFFFFFF, 0x00000002, 0x00000000, 0x00000000}};
  s21_decimal b = {{0x00000002, 0x00000001, 0x00000000, 0x00000000}};

  s21_decimal result = {{0}};
  s21_decimal expected_result = {
      {0xFFFFFFFE, 0x00000004, 0x00000003, 0x00000000}};

  ck_assert_int_eq(s21_mul(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_full_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};

  s21_decimal result = {{0}};
  ck_assert_int_eq(s21_mul(a, b, &result), N_INF);
}
END_TEST

START_TEST(test_s21_mul_no_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00090000}};
  s21_decimal b = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00080000}};
  ck_assert_int_eq(s21_mul(a, b, &result), OK);
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_single_bit_overflow) {
  s21_decimal a = {{0, 0, 0x80000000, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};

  s21_decimal result = {{0}};
  ck_assert_int_eq(s21_mul(a, b, &result), INF);
}
END_TEST

START_TEST(test_s21_mul_2) {
  s21_decimal a = {{555, 0, 0, 0x00020000}};
  s21_decimal b = {{1, 0, 0, 0x001C0000}};
  s21_decimal result = {{}};

  s21_decimal expected_result = {{6, 0, 0, 0x001C0000}};

  ck_assert_int_eq(s21_mul(a, b, &result), OK);
  // printf("%d\n", result.bits[0]);
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_mul_3) {
  s21_decimal a = {{666, 0, 0, 0x00020000}};
  s21_decimal b = {{1, 0, 0, 0x001C0000}};
  s21_decimal result = {{}};

  s21_decimal expected_result = {{7, 0, 0, 0x001C0000}};

  ck_assert_int_eq(s21_mul(a, b, &result), OK);
  // printf("%d\n", result.bits[0]);
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

Suite *s21_mul_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_mul");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_mul_zero);
  tcase_add_test(tc_core, test_s21_mul_base);
  tcase_add_test(tc_core, test_s21_mul_1);
  tcase_add_test(tc_core, test_s21_mul_multi_bits_1);
  tcase_add_test(tc_core, test_s21_mul_multi_bits_2);
  tcase_add_test(tc_core, test_s21_mul_full_overflow);
  tcase_add_test(tc_core, test_s21_mul_single_bit_overflow);
  tcase_add_test(tc_core, test_s21_mul_no_overflow);
  tcase_add_test(tc_core, test_s21_mul_2);
  tcase_add_test(tc_core, test_s21_mul_3);
  suite_add_tcase(s, tc_core);

  return s;
}
