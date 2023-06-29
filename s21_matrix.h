#ifndef C6_S21_MATRIX_1_S21_MATRIX_H
#define C6_S21_MATRIX_1_S21_MATRIX_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATE_ERR 2
#define SUCCESS 1
#define FAILURE 0
typedef struct matrix_struct {
    double **matrix;
    int rows;
    int columns;
} matrix_t;
void matrix_minores(int row, int column, matrix_t *M, matrix_t *result);
int valid_matrix(matrix_t *matrix);
double det(matrix_t *M);
int s21_create_matrix(int rows, int columns, matrix_t *result);

void s21_remove_matrix(matrix_t *A);

int s21_eq_matrix(matrix_t *A, matrix_t *B);

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_mult_number(matrix_t *A, double number, matrix_t *result);

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_transpose(matrix_t *A, matrix_t *result);

int s21_calc_complements(matrix_t *A, matrix_t *result);

int s21_determinant(matrix_t *A, double *result);

int s21_inverse_matrix(matrix_t *A, matrix_t *result);
void matrix_minores(int row, int column, matrix_t *M, matrix_t *result);
#endif //C6_S21_MATRIX_1_S21_MATRIX_H
