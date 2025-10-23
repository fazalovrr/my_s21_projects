#include <check.h>
#include <math.h>
#include <stdlib.h>

#include "../s21_matrix.h"

START_TEST(test_create_and_remove_matrix) {
  matrix_t A;
  int res = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(res, OK);
  ck_assert_ptr_nonnull(A.matrix);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 3);
  s21_remove_matrix(&A);
  ck_assert_ptr_null(A.matrix);

  res = s21_create_matrix(0, -1, &A);
  ck_assert_int_eq(res, WRONG_MATRIX);

  res = s21_create_matrix(0, 3, S21_NULL);
  ck_assert_int_eq(res, WRONG_MATRIX);

  res = s21_create_matrix(-2, 3, &A);
  ck_assert_int_eq(res, WRONG_MATRIX);

  res = s21_create_matrix(INT32_MAX, 0, &A);
  ck_assert_int_eq(res, WRONG_MATRIX);

  s21_remove_matrix(S21_NULL);
}
END_TEST

START_TEST(test_eq_matrix_success) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = B.matrix[0][0] = 1.0;
  A.matrix[0][1] = B.matrix[0][1] = 2.0;
  A.matrix[1][0] = B.matrix[1][0] = 3.0;
  A.matrix[1][1] = B.matrix[1][1] = 4.0;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  B.matrix[1][1] -= 1;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  ck_assert_int_eq(s21_eq_matrix(S21_NULL, S21_NULL), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix) {
  matrix_t A = {0}, B = {0}, result = {0};
  s21_create_matrix(2, 2, &A);
  int res = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_ge(res, WRONG_MATRIX);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = B.matrix[0][0] = 1.0;
  A.matrix[0][1] = B.matrix[0][1] = 2.0;
  A.matrix[1][0] = B.matrix[1][0] = 3.0;
  A.matrix[1][1] = B.matrix[1][1] = 4.0;

  double C[2][2] = {{2, 4}, {6, 8}};
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), OK);
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) {
      ck_assert_int_eq(result.matrix[i][j], C[i][j]);
    }
  s21_remove_matrix(&A);
  s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), CALC_ERROR);
  ck_assert_int_eq(s21_sum_matrix(S21_NULL, S21_NULL, &result), WRONG_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  int res = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_ge(res, WRONG_MATRIX);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = B.matrix[0][0] = 1.0;
  A.matrix[0][1] = B.matrix[0][1] = 2.0;
  A.matrix[1][0] = B.matrix[1][0] = 3.0;
  A.matrix[1][1] = B.matrix[1][1] = 4.0;

  double C[2][2] = {{0, 0}, {0, 0}};
  res = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(res, OK);
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], C[i][j], EPS);
    }
  s21_remove_matrix(&A);
  s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), CALC_ERROR);

  res = s21_sub_matrix(S21_NULL, S21_NULL, &result);
  ck_assert_int_eq(res, WRONG_MATRIX);
  res = s21_sub_matrix(&A, S21_NULL, &result);
  ck_assert_int_eq(res, WRONG_MATRIX);
  res = s21_sub_matrix(S21_NULL, &A, &result);
  ck_assert_int_eq(res, WRONG_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_ge(res, WRONG_MATRIX);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = B.matrix[0][0] = 1.0;
  A.matrix[0][1] = B.matrix[0][1] = 2.0;
  A.matrix[1][0] = B.matrix[1][0] = 3.0;
  A.matrix[1][1] = B.matrix[1][1] = 4.0;

  double C[2][2] = {{7, 10}, {15, 22}};
  res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, OK);
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], C[i][j], EPS);
    }

  res = s21_mult_matrix(S21_NULL, S21_NULL, &result);
  ck_assert_int_eq(res, WRONG_MATRIX);

  s21_create_matrix(3, 2, &B);
  res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  matrix_t A, result;
  s21_create_matrix(1, 1, &A);
  int res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(res, OK);
  ck_assert_double_eq_tol(result.matrix[0][0], 1, EPS);

  s21_create_matrix(1, 2, &A);
  res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(res, CALC_ERROR);

  res = s21_calc_complements(S21_NULL, &result);
  ck_assert_int_eq(res, WRONG_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_determinant_3x3) {
  matrix_t A;
  double result = 0.0;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  int res = s21_determinant(&A, &result);
  ck_assert_int_eq(res, OK);
  ck_assert_double_eq_tol(result, -40.0, EPS);
  s21_remove_matrix(&A);

  s21_create_matrix(3, 4, &A);
  res = s21_determinant(&A, &result);
  ck_assert_int_eq(res, CALC_ERROR);
  s21_remove_matrix(&A);

  s21_create_matrix(1, 1, &A);
  res = s21_determinant(&A, &result);
  ck_assert_int_eq(res, OK);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix) {
  matrix_t A, inv;
  int res = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(res, OK);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  res = s21_inverse_matrix(&A, &inv);
  ck_assert_double_eq_tol(inv.matrix[0][0], 1.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[0][1], -1.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[0][2], 1.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[1][0], -38.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[1][1], 41.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[1][2], -34.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[2][0], 27.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[2][1], -29.0, EPS);
  ck_assert_double_eq_tol(inv.matrix[2][2], 24.0, EPS);

  ck_assert_int_eq(res, OK);
  ck_assert_ptr_nonnull(inv.matrix);
  ck_assert_int_eq(inv.rows, 3);
  ck_assert_int_eq(inv.columns, 3);

  s21_remove_matrix(&A);
  s21_remove_matrix(&inv);

  s21_create_matrix(3, 4, &A);
  res = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(res, CALC_ERROR);
  s21_remove_matrix(&A);
  s21_remove_matrix(&inv);

  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 2;
  res = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(res, OK);
  ck_assert_double_eq_tol(inv.matrix[0][0], 0.5, EPS);

  A.matrix[0][0] = 0;
  res = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(res, CALC_ERROR);

  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 0;
  res = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&inv);
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s = suite_create("Matrix");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_and_remove_matrix);
  tcase_add_test(tc_core, test_eq_matrix_success);
  tcase_add_test(tc_core, test_sum_matrix);
  tcase_add_test(tc_core, test_sub_matrix);
  tcase_add_test(tc_core, test_mult_matrix);
  tcase_add_test(tc_core, test_calc_complements_1x1);
  tcase_add_test(tc_core, test_determinant_3x3);
  tcase_add_test(tc_core, test_inverse_matrix);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int failed = 0;
  Suite *s = matrix_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}