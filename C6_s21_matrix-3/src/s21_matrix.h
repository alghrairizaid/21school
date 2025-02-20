#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0

#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2
#define ALLOC_FAIL 3

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int allocate_matrix_memory(int rows, int columns, matrix_t *result);

void initialize_matrix(matrix_t *matrix);

int validate_matrix_dimensions(int rows, int columns);

int matrix_operation(matrix_t *A, matrix_t *B, matrix_t *result,
                     double (*operation)(double, double));

double add_operation(double a, double b);

double sub_operation(double a, double b);

double mult_operation(double a, double b);

int calculate_minor_matrix(matrix_t *A, matrix_t *minor, int skip_row,
                           int skip_col);

int handle_determinant_calculation(matrix_t *A, double *result);

int validate_square_matrix(matrix_t *A);

double calculate_algebraic_complement(matrix_t *A, int row, int col);

int validate_inverse_matrix(matrix_t *A, double *det);

void calculate_inverse_matrix(matrix_t *A, matrix_t *result, double det);

int transpose_matrix_elements(matrix_t *A, matrix_t *result);

int compare_matrix_elements(matrix_t *A, matrix_t *B);

int is_equal_with_precision(double a, double b);

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

int s21_eq_matrix(matrix_t *A, matrix_t *B);

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

int s21_determinant(matrix_t *A, double *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);

int s21_transpose(matrix_t *A, matrix_t *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

int s21_is_bad_matrix(matrix_t *A);
double calcDeterminant(matrix_t *A, int size);
int s21_is_bad_eq_matrix(matrix_t *A, matrix_t *B);
int simpleArithmetics(matrix_t *A, matrix_t *B, matrix_t *result, int znak);
void calcMinor(double **A, double **aux, int skipRow, int skipCol, int size);

void multiply_elements(matrix_t *A, matrix_t *B, matrix_t *result, int i,
                       int j);

int handle_single_element_inverse(matrix_t *A, matrix_t *result);
int handle_regular_inverse(matrix_t *A, matrix_t *result, matrix_t *comp,
                           matrix_t *trans);
int calculate_determinant_and_check(matrix_t *A, double *det);
int calculate_inverse_components(matrix_t *A, matrix_t *result, matrix_t *comp,
                                 matrix_t *trans, double det);
void fill_inverse_matrix(matrix_t *result, matrix_t *trans, double det);

#endif  // SRC_S21_MATRIX_H_
