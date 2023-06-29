#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = 1;
  if ((rows < 1) || (columns < 1) || result == NULL) {
    res = 0;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix == NULL)
      res = 0;
    else
      for (int i = 0; i < rows; i++)
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
  }
  return (res ? OK : INCORRECT_MATRIX);
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
  free(A->matrix);
  A->matrix = NULL;
  A->columns = 0;
  A->rows = 0;
}

int valid_matrix(matrix_t *matrix) {
  return matrix && (matrix->rows > 0) && (matrix->columns > 0) &&
         matrix->matrix;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = 1;
  if (A->rows != B->rows || A->columns != B->columns) {
    res = 0;
  }
  if (!valid_matrix(A) || !valid_matrix(B)) {
    res = 0;
  }
  for (int i = 0; i < res && A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) >= 1e-7) {
        res = 0;
        break;
      }
    }
  }

  return SUCCESS ? res : FAILURE;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A) || !valid_matrix(B))
    res = INCORRECT_MATRIX;
  else if (A->rows != B->rows || A->columns != B->columns)
    res = CALCULATE_ERR;
  else if (s21_create_matrix(A->rows, A->columns, result) != OK)
    res = INCORRECT_MATRIX;
  else
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A) || !valid_matrix(B))
    res = INCORRECT_MATRIX;
  else if (A->rows != B->rows || A->columns != B->columns)
    res = CALCULATE_ERR;
  else if (s21_create_matrix(A->rows, A->columns, result) != OK)
    res = INCORRECT_MATRIX;
  else
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A))
    res = INCORRECT_MATRIX;
  else if (s21_create_matrix(A->rows, A->columns, result) != OK)
    res = INCORRECT_MATRIX;
  else
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->rows; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A) || !valid_matrix(B))
    res = INCORRECT_MATRIX;
  else if (A->columns != B->rows)
    res = CALCULATE_ERR;
  else if (s21_create_matrix(A->rows, B->columns, result) != OK)
    res = INCORRECT_MATRIX;
  else
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < B->columns; j++)
        for (int k = 0; k < A->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A))
    res = INCORRECT_MATRIX;
  else if (s21_create_matrix(A->columns, A->rows, result) != OK)
    res = INCORRECT_MATRIX;
  else
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A))
    res = INCORRECT_MATRIX;
  else if (A->rows != A->columns)
    res = CALCULATE_ERR;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->rows == 1)
      result->matrix[0][0] = A->matrix[0][0];
    else {
      int sign;
      matrix_t temp;
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          matrix_minores(i, j, A, &temp);
          sign = ((i + j) % 2 == 0) ? 1 : -1;
          result->matrix[i][j] = det(&temp) * sign;
          s21_remove_matrix(&temp);
        }
      }
    }
  }
  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = OK;
  if (!valid_matrix(A))
    res = INCORRECT_MATRIX;
  else if (A->rows != A->columns)
    res = CALCULATE_ERR;
  else
    *result = det(A);
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (!valid_matrix(A))
    res = INCORRECT_MATRIX;
  else if (A->rows != A->columns)
    res = CALCULATE_ERR;
  else {
    double determinant = det(A);
    if (fabs(determinant) < 1e-7)
      res = CALCULATE_ERR;
    else {
      matrix_t comp, trans;

      s21_calc_complements(A, &comp);
      s21_transpose(&comp, &trans);

      s21_mult_number(&trans, 1 / determinant, result);

      s21_remove_matrix(&comp);
      s21_remove_matrix(&trans);
    }
  }
  return res;
}

double det(matrix_t *M) {
  double result = 0;
  if (M->rows == 1)
    result = M->matrix[0][0];
  else if (M->rows == 2)
    result = (M->matrix[0][0] * M->matrix[1][1]) -
             (M->matrix[0][1] * M->matrix[1][0]);
  else {
    int sign = 1;
    for (int i = 0; i < M->rows; i++) {
      matrix_t temp;
      matrix_minores(0, i, M, &temp);

      result += sign * M->matrix[0][i] * det(&temp);
      sign *= -1;

      s21_remove_matrix(&temp);
    }
  }
  return result;
}

void matrix_minores(int row, int column, matrix_t *M, matrix_t *result) {
  s21_create_matrix(M->rows - 1, M->columns - 1, result);

  int di = 0, dj = 0;
  for (int i = 0; i < result->rows; i++) {
    if (i == row) di = 1;
    dj = 0;
    for (int j = 0; j < result->columns; j++) {
      if (j == column) dj = 1;
      result->matrix[i][j] = M->matrix[i + di][j + dj];
    }
  }
}