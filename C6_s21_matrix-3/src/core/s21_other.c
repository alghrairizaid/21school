#include "../s21_matrix.h"

int calculate_minor_matrix(matrix_t *A, matrix_t *minor, int skip_row,
                           int skip_col) {
  int mi = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i != skip_row) {
      int mj = 0;
      for (int j = 0; j < A->columns; j++) {
        if (j != skip_col) {
          minor->matrix[mi][mj++] = A->matrix[i][j];
        }
      }
      mi++;
    }
  }
  return OK;
}

double calculate_algebraic_complement(matrix_t *A, int row, int col) {
  double result = 0.0;
  matrix_t minor = {0};
  
  if (s21_create_matrix(A->rows - 1, A->columns - 1, &minor) == OK) {
    calculate_minor_matrix(A, &minor, row, col);
    double det = 0.0;
    if (s21_determinant(&minor, &det) == OK) {
      result = det * pow(-1, row + col);
    }
    s21_remove_matrix(&minor);
  }
  return result;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = s21_is_bad_matrix(A);

  if (status == OK) {
    if (A->rows != A->columns) {
      status = CALCULATION_ERROR;
    } else {
      status = s21_create_matrix(A->rows, A->columns, result);
      if (status == OK) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = calculate_algebraic_complement(A, i, j);
          }
        }
      }
    }
  }

  if (status != OK && result != NULL) {
    s21_remove_matrix(result);
  }

  return status;
}

double calcDeterminant(matrix_t *A, int size) {
  if (size == 1) return A->matrix[0][0];
  if (size == 2) {
    return A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  }

  double det = 0.0;
  matrix_t minor = {0};
  
  if (s21_create_matrix(size - 1, size - 1, &minor) == OK) {
    for (int j = 0; j < size; j++) {
      calculate_minor_matrix(A, &minor, 0, j);
      double cofactor = (j % 2 == 0 ? 1 : -1) * A->matrix[0][j];
      det += cofactor * calcDeterminant(&minor, size - 1);
    }
    s21_remove_matrix(&minor);
  }
  return det;
}

int s21_determinant(matrix_t *A, double *result) {
  int status = s21_is_bad_matrix(A);
  *result = 0.0;  // Initialize result

  if (status == OK) {
    if (A->rows != A->columns) {
      status = CALCULATION_ERROR;
    } else if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else {
      *result = calcDeterminant(A, A->rows);
    }
  }
  return status;
}

int s21_is_bad_eq_matrix(matrix_t *A, matrix_t *B) {
  return (s21_is_bad_matrix(A) || s21_is_bad_matrix(B) || A->rows != B->rows ||
          A->columns != B->columns)
             ? INCORRECT_MATRIX
             : OK;
}

int s21_is_bad_matrix(matrix_t *A) {
  return (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0);
}
