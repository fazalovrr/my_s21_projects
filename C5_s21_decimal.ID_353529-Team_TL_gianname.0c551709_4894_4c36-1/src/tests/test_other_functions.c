#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_floor_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_floor(a, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_error) {
  s21_decimal a = {{5, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_floor(a, NULL), (ERROR));
}
END_TEST

START_TEST(test_s21_floor_1) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(10.5, &a_decimal);
  s21_from_float_to_decimal(10, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_2) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(100.9, &a_decimal);
  s21_from_float_to_decimal(100, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_3) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-10.5, &a_decimal);
  s21_from_float_to_decimal(-11, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_4) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-100.1, &a_decimal);
  s21_from_float_to_decimal(-101, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_5) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-100, &a_decimal);
  s21_from_float_to_decimal(-100, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_6) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(1000, &a_decimal);
  s21_from_float_to_decimal(1000, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_7) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(100.2, &a_decimal);
  s21_from_float_to_decimal(100, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_floor_8) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-1000.9, &a_decimal);
  s21_from_float_to_decimal(-1001, &expected_result);
  ck_assert_int_eq(s21_floor(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_round(a, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_error) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_round(a, NULL), (ERROR));
}
END_TEST

START_TEST(test_s21_round_1) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(10.5, &a_decimal);
  s21_from_float_to_decimal(11, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_2) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(100.9, &a_decimal);
  s21_from_float_to_decimal(101, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_3) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-11.5, &a_decimal);
  s21_from_float_to_decimal(-12, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_4) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-100.1, &a_decimal);
  s21_from_float_to_decimal(-100, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_5) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-100, &a_decimal);
  s21_from_float_to_decimal(-100, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_6) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(1000, &a_decimal);
  s21_from_float_to_decimal(1000, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_7) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(100.2, &a_decimal);
  s21_from_float_to_decimal(100, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_round_8) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-1000.9, &a_decimal);
  s21_from_float_to_decimal(-1001, &expected_result);
  ck_assert_int_eq(s21_round(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

//--
START_TEST(test_s21_truncate_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_truncate(a, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_error) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_truncate(a, NULL), (ERROR));
}
END_TEST

START_TEST(test_s21_truncate_1) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(10.5, &a_decimal);
  s21_from_float_to_decimal(10, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_2) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(100.9, &a_decimal);
  s21_from_float_to_decimal(100, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_3) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-11.500, &a_decimal);
  s21_from_float_to_decimal(-11, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_4) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-100.001, &a_decimal);
  s21_from_float_to_decimal(-100, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_5) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-100.00, &a_decimal);
  s21_from_float_to_decimal(-100, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_6) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(1000, &a_decimal);
  s21_from_float_to_decimal(1000, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_7) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(100.2, &a_decimal);
  s21_from_float_to_decimal(100, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_truncate_8) {
  s21_decimal a_decimal = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0}};
  s21_from_float_to_decimal(-1000.9, &a_decimal);
  s21_from_float_to_decimal(-1000, &expected_result);
  ck_assert_int_eq(s21_truncate(a_decimal, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_negate_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_negate(a, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_negate_error) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_negate(a, NULL), (ERROR));
}
END_TEST

START_TEST(test_s21_negate_1) {
  s21_decimal a = {{10, 20, 3, 0x80000000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{10, 20, 3, 0x00000000}};
  ck_assert_int_eq(s21_negate(a, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

START_TEST(test_s21_negate_2) {
  s21_decimal a = {{100, 0, 3, 0x00010000}};
  s21_decimal result = {{0}};
  s21_decimal expected_result = {{100, 0, 3, 0x80010000}};
  ck_assert_int_eq(s21_negate(a, &result), (OK));
  ck_assert_int_eq(s21_is_equal(result, expected_result), TRUE);
}
END_TEST

Suite *s21_floor_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_floor");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_floor_zero);
  tcase_add_test(tc_core, test_s21_floor_error);
  tcase_add_test(tc_core, test_s21_floor_1);
  tcase_add_test(tc_core, test_s21_floor_2);
  tcase_add_test(tc_core, test_s21_floor_3);
  tcase_add_test(tc_core, test_s21_floor_4);
  tcase_add_test(tc_core, test_s21_floor_5);
  tcase_add_test(tc_core, test_s21_floor_6);
  tcase_add_test(tc_core, test_s21_floor_7);
  tcase_add_test(tc_core, test_s21_floor_8);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_round_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_round");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_round_zero);
  tcase_add_test(tc_core, test_s21_round_error);
  tcase_add_test(tc_core, test_s21_round_1);
  tcase_add_test(tc_core, test_s21_round_2);
  tcase_add_test(tc_core, test_s21_round_3);
  tcase_add_test(tc_core, test_s21_round_4);
  tcase_add_test(tc_core, test_s21_round_5);
  tcase_add_test(tc_core, test_s21_round_6);
  tcase_add_test(tc_core, test_s21_round_7);
  tcase_add_test(tc_core, test_s21_round_8);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_truncate_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_truncate");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_truncate_zero);
  tcase_add_test(tc_core, test_s21_truncate_error);
  tcase_add_test(tc_core, test_s21_truncate_1);
  tcase_add_test(tc_core, test_s21_truncate_2);
  tcase_add_test(tc_core, test_s21_truncate_3);
  tcase_add_test(tc_core, test_s21_truncate_4);
  tcase_add_test(tc_core, test_s21_truncate_5);
  tcase_add_test(tc_core, test_s21_truncate_6);
  tcase_add_test(tc_core, test_s21_truncate_7);
  tcase_add_test(tc_core, test_s21_truncate_8);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_negate_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_negate");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_negate_zero);
  tcase_add_test(tc_core, test_s21_negate_error);
  tcase_add_test(tc_core, test_s21_negate_1);
  tcase_add_test(tc_core, test_s21_negate_2);

  suite_add_tcase(s, tc_core);

  return s;
}
