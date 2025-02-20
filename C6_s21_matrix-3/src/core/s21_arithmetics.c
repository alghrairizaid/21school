#include "../s21_matrix.h"

double add_operation(double a, double b) { return a + b; }
double sub_operation(double a, double b) { return a - b; }
double mult_operation(double a, double b) { return a * b; }

int matrix_operation(matrix_t *A, matrix_t *B, matrix_t *result,
                     double (*operation)(double, double)) {
  int status = s21_is_bad_eq_matrix(A, B);
  if (status == OK) {
    status = s21_create_matrix(A->rows, A->columns, result);
    if (status == OK) {
      for (int i = 0; i < A->rows && status == OK; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = operation(A->matrix[i][j], B->matrix[i][j]);
        }
      }
    }
  }
  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return matrix_operation(A, B, result, add_operation);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return matrix_operation(A, B, result, sub_operation);
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = CALCULATION_ERROR;

  if (!s21_is_bad_matrix(A) && !s21_is_bad_matrix(B) && A->columns == B->rows) {
    status = s21_create_matrix(A->rows, B->columns, result);
    if (status == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          result->matrix[i][j] = 0;
          for (int k = 0; k < A->columns; k++) {
            result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
          }
        }
      }
    }
  }
  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = s21_is_bad_matrix(A);

  if (status == OK) {
    if ((status = s21_create_matrix(A->rows, A->columns, result)) == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = mult_operation(A->matrix[i][j], number);
        }
      }
    }
  }
  return status;
}
