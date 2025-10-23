#include <math.h>
#include <stdlib.h>

#include "../s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = OK;

  if (rows < 1 || columns < 1) {
    res = WRONG_MATRIX;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = calloc(rows, sizeof(double *));
    res = WRONG_MATRIX;
    if (result->matrix != S21_NULL) {
      for (int i = 0; i < result->rows; i++) {
        result->matrix[i] = calloc(columns, sizeof(double));
      }
      res = OK;
    }
  }
  return res;
}

static int matrix_no_valid(matrix_t *A) {
  int res = OK;
  if (A == S21_NULL)
    res = WRONG_MATRIX;
  else if (A->matrix == S21_NULL || A->rows <= 0 || A->columns <= 0)
    res = WRONG_MATRIX;
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (!A || A->matrix == S21_NULL) return;
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
  A->matrix = S21_NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (matrix_no_valid(A) || matrix_no_valid(B) || A->rows != B->rows ||
      A->columns != B->columns) {
    res = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
          res = FAILURE;
        }
      }
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (matrix_no_valid(A) || matrix_no_valid(B))
    res = WRONG_MATRIX;
  else if (A->rows != B->rows || A->columns != B->columns)
    res = CALC_ERROR;
  if (res) return res;
  res = s21_create_matrix(A->rows, A->columns, result);
  if (!res)
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];

  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (matrix_no_valid(A) || matrix_no_valid(B))
    res = WRONG_MATRIX;
  else if (A->rows != B->rows || A->columns != B->columns)
    res = CALC_ERROR;
  if (res) return res;
  matrix_t C = {0};
  res = s21_mult_number(B, -1, &C);
  if (!res) res = s21_sum_matrix(A, &C, result);
  s21_remove_matrix(&C);
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (matrix_no_valid(A)) return WRONG_MATRIX;

  int res = OK;
  res = s21_create_matrix(A->rows, A->columns, result);
  if (!res) {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (matrix_no_valid(A) || matrix_no_valid(B))
    res = WRONG_MATRIX;
  else if (res || A->columns != B->rows)
    res = CALC_ERROR;
  if (res) return res;

  res = s21_create_matrix(A->rows, B->columns, result);
  if (!res)
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < B->columns; j++)
        for (int k = 0; k < B->rows; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];

  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (matrix_no_valid(A)) return WRONG_MATRIX;

  res = s21_create_matrix(A->columns, A->rows, result);
  if (!res)
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];

  return res;
}

static int is_square(matrix_t *A) {
  return !matrix_no_valid(A) && A->rows == A->columns;
}

static int get_minor(matrix_t *A, int row, int col, matrix_t *minor) {
  int res = s21_create_matrix(A->rows - 1, A->columns - 1, minor);
  if (!res) {
    int mi = 0;
    for (int i = 0; i < A->rows; i++) {
      if (i != row) {
        int mj = 0;
        for (int j = 0; j < A->columns; j++) {
          if (j != col) {
            minor->matrix[mi][mj] = A->matrix[i][j];
            mj++;
          }
        }
        mi++;
      }
    }
  }
  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  if (matrix_no_valid(A) || result == S21_NULL) return WRONG_MATRIX;

  int res = OK;
  *result = 0.0;

  if (!is_square(A)) {
    res = CALC_ERROR;
  } else if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    for (int j = 0; j < A->columns && !res; j++) {
      matrix_t minor = {0};
      double det_minor = 0.0;
      res = get_minor(A, 0, j, &minor);
      if (!res) {
        res = s21_determinant(&minor, &det_minor);
        if (!res) {
          double sign = (j % 2 == 0) ? 1.0 : -1.0;
          *result += sign * A->matrix[0][j] * det_minor;
        }
      }
      s21_remove_matrix(&minor);
    }
  }
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (matrix_no_valid(A)) return WRONG_MATRIX;

  int res = OK;
  if (!is_square(A)) {
    res = CALC_ERROR;
  } else if (A->rows == 1 && A->columns == 1) {
    res = s21_create_matrix(1, 1, result);
    if (res == OK) {
      result->matrix[0][0] = 1.0;
    }
  } else {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == OK) {
      for (int i = 0; i < A->rows && res == OK; i++) {
        for (int j = 0; j < A->columns && res == OK; j++) {
          matrix_t minor = {0};
          double det_minor = 0.0;
          res = get_minor(A, i, j, &minor);
          if (res == OK) {
            res = s21_determinant(&minor, &det_minor);
            if (res == OK) {
              double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
              result->matrix[i][j] = sign * det_minor;
            }
          }
          s21_remove_matrix(&minor);
        }
      }
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (matrix_no_valid(A)) return WRONG_MATRIX;

  int res = OK;
  double det = 0.0;

  if (!is_square(A)) {
    res = CALC_ERROR;
  } else if (A->rows == 1 && A->columns == 1) {
    res = s21_create_matrix(1, 1, result);
    if (res == OK) {
      if (fabs(A->matrix[0][0]) < EPS) {
        s21_remove_matrix(result);
        res = CALC_ERROR;
      } else {
        result->matrix[0][0] = 1.0 / A->matrix[0][0];
      }
    }
  } else {
    res = s21_determinant(A, &det);
    if (!res && fabs(det) < EPS) {
      res = CALC_ERROR;
    } else {
      matrix_t complements = {0}, adjoint = {0};
      res = s21_calc_complements(A, &complements);
      if (!res) res = s21_transpose(&complements, &adjoint);
      if (!res) res = s21_mult_number(&adjoint, 1.0 / det, result);
      s21_remove_matrix(&complements);
      s21_remove_matrix(&adjoint);
    }
  }
  return res;
}
