#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_is_less_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_zero0) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_zero1) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_less_zero2) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_zero3) {
  s21_decimal a = {{2, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_zero4) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_less_pos_neg) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_neg_pos) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), TRUE);
}
END_TEST

START_TEST(test_s21_is_less_pos1) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{13, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_less_pos2) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_neg1) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{13, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_neg2) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80010000}};
  ck_assert_int_eq(s21_is_less(a, b), TRUE);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_less_or_equal_pos) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_less_or_equal_neg1) {
  s21_decimal a = {{4, 0, 0, 0x80010000}};
  s21_decimal b = {{4, 0, 0, 0x80010000}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_less_or_equal_neg2) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80010000}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), TRUE);
}
END_TEST

START_TEST(test_s21_is_equal_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_equal_zero0) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_equal_zero1) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_zero2) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_zero3) {
  s21_decimal a = {{2, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_zero4) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_pos_neg) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{4, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_neg_pos) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_pos1) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{4, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_equal_pos2) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_neg1) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{13, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_equal_neg2) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(a, b), TRUE);
}
END_TEST

START_TEST(test_s21_is_not_equal_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_not_equal_pos_neg) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{4, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_not_equal_neg1) {
  s21_decimal a = {{14, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_not_equal_neg2) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(a, b), FALSE);
}
END_TEST

START_TEST(test_s21_is_greater_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_greater_zero0) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_greater_zero1) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_greater_zero2) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{2, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_zero3) {
  s21_decimal a = {{2, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_zero4) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{0, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_greater_pos_neg) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_neg_pos) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_greater_pos1) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{13, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST
START_TEST(test_s21_is_greater_pos2) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_neg1) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{13, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_neg2) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80010000}};
  ck_assert_int_eq(s21_is_greater(a, b), FALSE);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_zero) {
  s21_decimal a = {{0, 0, 0, 0x00000000}};
  s21_decimal b = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_or_equal_pos) {
  s21_decimal a = {{4, 0, 0, 0x00000000}};
  s21_decimal b = {{3, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_or_equal_neg1) {
  s21_decimal a = {{4, 0, 0, 0x80010000}};
  s21_decimal b = {{4, 0, 0, 0x80010000}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), TRUE);
}
END_TEST
START_TEST(test_s21_is_greater_or_equal_neg2) {
  s21_decimal a = {{4, 0, 0, 0x80000000}};
  s21_decimal b = {{4, 0, 0, 0x80010000}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), FALSE);
}
END_TEST

Suite *s21_is_less_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_is_less");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_is_less_zero);
  tcase_add_test(tc_core, test_s21_is_less_zero0);
  tcase_add_test(tc_core, test_s21_is_less_zero1);
  tcase_add_test(tc_core, test_s21_is_less_zero2);
  tcase_add_test(tc_core, test_s21_is_less_zero3);
  tcase_add_test(tc_core, test_s21_is_less_zero4);
  tcase_add_test(tc_core, test_s21_is_less_pos_neg);
  tcase_add_test(tc_core, test_s21_is_less_neg_pos);
  tcase_add_test(tc_core, test_s21_is_less_pos1);
  tcase_add_test(tc_core, test_s21_is_less_pos2);
  tcase_add_test(tc_core, test_s21_is_less_neg1);
  tcase_add_test(tc_core, test_s21_is_less_neg2);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_is_less_or_equal_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_is_less_or_equal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_is_less_or_equal_zero);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_pos);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_neg1);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_neg2);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_is_greater_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_is_greater");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_is_greater_zero);
  tcase_add_test(tc_core, test_s21_is_greater_zero0);
  tcase_add_test(tc_core, test_s21_is_greater_zero1);
  tcase_add_test(tc_core, test_s21_is_greater_zero2);
  tcase_add_test(tc_core, test_s21_is_greater_zero3);
  tcase_add_test(tc_core, test_s21_is_greater_zero4);
  tcase_add_test(tc_core, test_s21_is_greater_pos_neg);
  tcase_add_test(tc_core, test_s21_is_greater_neg_pos);
  tcase_add_test(tc_core, test_s21_is_greater_pos1);
  tcase_add_test(tc_core, test_s21_is_greater_pos2);
  tcase_add_test(tc_core, test_s21_is_greater_neg1);
  tcase_add_test(tc_core, test_s21_is_greater_neg2);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_is_greater_or_equal_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_is_greater_or_equal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_is_greater_or_equal_zero);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_pos);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_neg1);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_neg2);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_is_equal_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_is_equal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_is_equal_zero);
  tcase_add_test(tc_core, test_s21_is_equal_zero0);
  tcase_add_test(tc_core, test_s21_is_equal_zero1);
  tcase_add_test(tc_core, test_s21_is_equal_zero2);
  tcase_add_test(tc_core, test_s21_is_equal_zero3);
  tcase_add_test(tc_core, test_s21_is_equal_zero4);
  tcase_add_test(tc_core, test_s21_is_equal_pos_neg);
  tcase_add_test(tc_core, test_s21_is_equal_neg_pos);
  tcase_add_test(tc_core, test_s21_is_equal_pos1);
  tcase_add_test(tc_core, test_s21_is_equal_pos2);
  tcase_add_test(tc_core, test_s21_is_equal_neg1);
  tcase_add_test(tc_core, test_s21_is_equal_neg2);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *s21_is_not_equal_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_is_not_equal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_is_not_equal_zero);
  tcase_add_test(tc_core, test_s21_is_not_equal_pos_neg);
  tcase_add_test(tc_core, test_s21_is_not_equal_neg1);
  tcase_add_test(tc_core, test_s21_is_not_equal_neg2);

  suite_add_tcase(s, tc_core);

  return s;
}