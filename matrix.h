#include <stdint.h>

typedef struct matrix {
    uint32_t m;
    uint32_t n;
    uint8_t **M;
} Matrix;

Matrix new_matrix(uint32_t m, uint32_t n);
uint8_t get_value(Matrix m, uint32_t i, uint32_t j);
void set_value(Matrix m, uint32_t i, uint32_t j, uint8_t value);
int multiply(Matrix A, Matrix B, Matrix P);
void print(Matrix m);