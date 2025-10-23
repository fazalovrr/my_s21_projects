#include <check.h>

#include "../functions/sup_func.h"
#include "../s21_decimal.h"

START_TEST(test_decimal_to_int_null) {
  s21_decimal a = {{1, 0, 0, 0x00000000}};
  ck_assert_int_eq(s21_from_decimal_to_int(a, NULL), (ERROR));
}
END_TEST

START_TEST(test_decimal_to_int_basic) {
  s21_decimal dec = {{123456789, 0, 0, 0x10000}};
  int result;
  int code = s21_from_decimal_to_int(dec, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result, 12345678);
}
END_TEST

START_TEST(test_decimal_to_int_negative) {
  s21_decimal dec = {{10009, 0, 0, 0x80010000}};
  int result;
  int code = s21_from_decimal_to_int(dec, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result, -1000);
}
END_TEST

START_TEST(test_decimal_to_int_error) {
  s21_decimal dec = {{10009, 10000, 10000, 0}};
  int result;
  int code = s21_from_decimal_to_int(dec, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_int_to_decimal_null) {
  int src = 1;
  ck_assert_int_eq(s21_from_int_to_decimal(src, NULL), (ERROR));
}
END_TEST

START_TEST(test_int_to_decimal_basic) {
  int src = 12345678;
  s21_decimal result;
  int code = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12345678);
}
END_TEST

START_TEST(test_int_to_decimal_negative) {
  int src = -12345678;
  s21_decimal result;
  int code = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12345678);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_decimal_to_float_basic) {
  s21_decimal dec = {{123456789, 0, 0, 0}};
  float result;
  int code = s21_from_decimal_to_float(dec, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(result, 123456789.0f);
}
END_TEST

START_TEST(test_decimal_to_float_with_scale) {
  s21_decimal dec = {{123456789, 0, 0, 0}};
  dec.bits[3] = (5 << 16);
  float result;
  int code = s21_from_decimal_to_float(dec, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq_tol(result, 1234.56789f, 1e-5);
}
END_TEST

START_TEST(test_decimal_to_float_negative) {
  s21_decimal dec = {{1000, 0, 0, 0}};
  dec.bits[3] = (uint32_t)(1 << 31);
  float result;
  int code = s21_from_decimal_to_float(dec, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(result, -1000.0f);
}
END_TEST

START_TEST(test_float_to_decimal_basic) {
  float src = 123456.0f;
  s21_decimal dec;
  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(dec.bits[0], 123456);
}
END_TEST

START_TEST(test_float_to_decimal_fractional) {
  float src = 12.345f;
  s21_decimal dec;
  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_scale(dec), 3);
  ck_assert_int_eq(dec.bits[0], 12345);
}
END_TEST

START_TEST(test_float_to_decimal_negative) {
  float src = -0.01f;
  s21_decimal dec;
  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_sign(dec), 1);
  ck_assert_int_eq(get_scale(dec), 2);
  ck_assert_int_eq(dec.bits[0], 1);
}
END_TEST

START_TEST(test_float_to_decimal_nan_inf) {
  float src = NAN;
  s21_decimal dec;
  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 1);

  src = INFINITY;
  code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_decimal_to_float_null_dst) {
  s21_decimal dec = {{1, 0, 0, 0}};
  int code = s21_from_decimal_to_float(dec, NULL);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_float_to_decimal_max_scale) {
  float src = 1e-30f;
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 1);
  ck_assert_int_le(get_scale(dec), 28);
}
END_TEST

START_TEST(test_float_to_decimal_big_val) {
  float src = pow(2.0, 70) + 1.0;
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
  ck_assert_int_le(get_scale(dec), 0);
}
END_TEST

START_TEST(test_float_to_decimal_max_val) {
  float src = pow(2.0, 97);
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_float_to_decimal_min_val) {
  float src = -pow(2.0, 97);
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_float_to_decimal1) {
  float src = -1.234568e-24;
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_float_to_decimal2) {
  float src = 0.000000e+00;
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_float_to_decimal3) {
  float src = 3.961408e+28;
  s21_decimal dec;

  int code = s21_from_float_to_decimal(src, &dec);
  ck_assert_int_eq(code, 0);
}
END_TEST

Suite *s21_from_int_to_decimal_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_from_int_to_decimal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_int_to_decimal_basic);
  tcase_add_test(tc_core, test_int_to_decimal_negative);
  tcase_add_test(tc_core, test_int_to_decimal_null);

  suite_add_tcase(s, tc_core);
  return s;
}
Suite *s21_from_float_to_decimal_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_from_float_to_decimal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_float_to_decimal_basic);
  tcase_add_test(tc_core, test_float_to_decimal_fractional);
  tcase_add_test(tc_core, test_float_to_decimal_negative);
  tcase_add_test(tc_core, test_float_to_decimal_nan_inf);
  tcase_add_test(tc_core, test_float_to_decimal_max_scale);
  tcase_add_test(tc_core, test_float_to_decimal_big_val);
  tcase_add_test(tc_core, test_float_to_decimal_max_val);
  tcase_add_test(tc_core, test_float_to_decimal_min_val);

  suite_add_tcase(s, tc_core);
  return s;
}

Suite *s21_from_decimal_to_int_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_from_decimal_to_int");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_decimal_to_int_basic);
  tcase_add_test(tc_core, test_decimal_to_int_negative);
  tcase_add_test(tc_core, test_decimal_to_int_error);
  tcase_add_test(tc_core, test_decimal_to_int_null);

  suite_add_tcase(s, tc_core);
  return s;
}

Suite *s21_from_decimal_to_float_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_from_decimal_to_float");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_decimal_to_float_basic);
  tcase_add_test(tc_core, test_decimal_to_float_with_scale);
  tcase_add_test(tc_core, test_decimal_to_float_negative);
  tcase_add_test(tc_core, test_decimal_to_float_null_dst);
  tcase_add_test(tc_core, test_float_to_decimal1);
  tcase_add_test(tc_core, test_float_to_decimal2);
  tcase_add_test(tc_core, test_float_to_decimal3);
  suite_add_tcase(s, tc_core);
  return s;
}
