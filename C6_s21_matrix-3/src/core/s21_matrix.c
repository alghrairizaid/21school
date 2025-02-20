#include "../s21_matrix.h"

int validate_matrix_dimensions(int rows, int columns) {
  return (rows > 0 && columns > 0) ? OK : INCORRECT_MATRIX;
}

int allocate_matrix_memory(int rows, int columns, matrix_t *result) {
  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix == NULL) return ALLOC_FAIL;

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = (double *)calloc(columns, sizeof(double));
    if (result->matrix[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
      return ALLOC_FAIL;
    }
  }
  return OK;
}

void initialize_matrix(matrix_t *matrix) {
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      matrix->matrix[i][j] = 0.0;
    }
  }
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = validate_matrix_dimensions(rows, columns);

  if (status == OK) {
    status = allocate_matrix_memory(rows, columns, result);
    if (status == OK) {
      result->rows = rows;
      result->columns = columns;
      initialize_matrix(result);
    }
  }

  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    for (int x = 0; x < A->rows; x += 1) {
      free(A->matrix[x]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}
