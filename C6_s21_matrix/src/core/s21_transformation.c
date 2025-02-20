#include "../s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = s21_is_bad_matrix(A);

  if (status == OK) {
    status = s21_create_matrix(A->columns, A->rows, result);
    if (status == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  }
  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = s21_is_bad_matrix(A);

  if (status == OK && A->rows == A->columns) {
    if (A->rows == 1) {
      status = handle_single_element_inverse(A, result);
    } else {
      matrix_t comp = {0};
      matrix_t trans = {0};
      status = handle_regular_inverse(A, result, &comp, &trans);
      s21_remove_matrix(&comp);
      s21_remove_matrix(&trans);
    }
  } else {
    status = CALCULATION_ERROR;
  }

  return status;
}

int handle_single_element_inverse(matrix_t *A, matrix_t *result) {
  int status = s21_create_matrix(1, 1, result);
  if (status == OK && fabs(A->matrix[0][0]) > 1e-6) {
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
  } else {
    status = CALCULATION_ERROR;
    s21_remove_matrix(result);
  }
  return status;
}

int handle_regular_inverse(matrix_t *A, matrix_t *result, matrix_t *comp,
                           matrix_t *trans) {
  int status = OK;
  double det = 0.0;

  if (s21_determinant(A, &det) == OK && fabs(det) > 1e-6) {
    if ((status = s21_calc_complements(A, comp)) == OK) {
      if ((status = s21_transpose(comp, trans)) == OK) {
        if ((status = s21_create_matrix(trans->rows, trans->columns, result)) == OK) {
          for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->columns; j++) {
              result->matrix[i][j] = trans->matrix[i][j] / det;
            }
          }
        }
      }
    }
  } else {
    status = CALCULATION_ERROR;
  }

  if (status != OK && result != NULL) {
    s21_remove_matrix(result);
  }

  return status;
}
