#include "../s21_matrix.h"

int is_equal_with_precision(double a, double b) { return fabs(a - b) <= 1e-7; }

int compare_matrix_elements(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;

  for (int i = 0; i < A->rows && status; i++) {
    for (int j = 0; j < A->columns && status; j++) {
      if (!is_equal_with_precision(A->matrix[i][j], B->matrix[i][j])) {
        status = FAILURE;
      }
    }
  }
  return status;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (s21_is_bad_eq_matrix(A, B) != OK) {
    return FAILURE;
  }
  return compare_matrix_elements(A, B);
}
