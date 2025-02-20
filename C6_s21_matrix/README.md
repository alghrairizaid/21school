# s21_matrix

Implementation of the matrix.h library for working with matrices in C.

## Build Instructions

1. To compile the library:
```bash
make
```

2. To run tests:
```bash
make test
```

3. To check for memory leaks:
```bash
make valgrind
```

## Library Functions

The library implements the following matrix operations:

- `s21_create_matrix` - Creates a matrix with specified rows and columns
- `s21_remove_matrix` - Removes the matrix
- `s21_eq_matrix` - Compares two matrices
- `s21_sum_matrix` - Adds two matrices
- `s21_sub_matrix` - Subtracts one matrix from another
- `s21_mult_number` - Multiplies a matrix by a number
- `s21_mult_matrix` - Multiplies two matrices
- `s21_transpose` - Creates a new transposed matrix
- `s21_calc_complements` - Calculates the algebraic complements matrix
- `s21_determinant` - Calculates the matrix determinant
- `s21_inverse_matrix` - Calculates the inverse matrix

## Usage Example

```c
#include "s21_matrix.h"

int main() {
    matrix_t A, B, result;
    
    s21_create_matrix(3, 3, &A);
    s21_create_matrix(3, 3, &B);
    
    // Fill matrices with values
    // ...
    
    // Perform matrix addition
    s21_sum_matrix(&A, &B, &result);
    
    // Clean up
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
    
    return 0;
}
```

## Testing

Tests are implemented using the Check framework. Run `make test` to execute all tests.
