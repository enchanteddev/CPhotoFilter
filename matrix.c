#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct matrix {
    uint32_t m;
    uint32_t n;
    uint8_t **M;
} Matrix;


Matrix new_matrix(uint32_t m, uint32_t n) {
    Matrix matrix;
    matrix.m = m;
    matrix.n = n;

    matrix.M = (uint8_t**)malloc(sizeof(uint8_t*) * m);
    if (matrix.M == NULL) {
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < m; i++) {
        matrix.M[i] = (uint8_t*)calloc(n, sizeof(uint8_t));
        if (matrix.M[i] == NULL) {
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}


uint8_t get_value(Matrix m, uint32_t i, uint32_t j){
    // printf("Reading %d %d\n", i, j);
    return *(*(m.M + i) + j);
}


void set_value(Matrix m, uint32_t i, uint32_t j, uint8_t value){
    *(*(m.M + i) + j) = value;
}


int multiply(Matrix A, Matrix B, Matrix P){
    if (A.n != B.m){
        return 1; // incompatible matrices
    }

    for (uint32_t i = 0; i < A.m; i++){
        uint8_t *rowA = *(A.M + i);
        for (uint32_t j = 0; j < B.n; j++){
            uint8_t mij = 0;
            for (int k = 0; k < B.m; k++){
                uint8_t a = *(rowA + k);
                uint8_t b = *(*(B.M + k) + j);
                mij += a * b;
            }
            set_value(P, i, j, mij);
        }
        
    }
    return 0;
}


void print(Matrix m){
    for (int i = 0; i < m.m; i++){
        for (int j = 0; j < m.n; j++){
            printf("%d ", *(*(m.M + i) + j));
        }
        printf("\n");
    }
}


// int main(){
//     Matrix m;
//     m = new_matrix(2, 2);
//     set_value(m, 0, 1, 6);
//     set_value(m, 1, 1, 4);
//     set_value(m, 1, 0, 5);
//     print(m);
//     Matrix m2;
//     m2 = new_matrix(2, 2);
//     set_value(m2, 0, 1, 4);
//     set_value(m2, 1, 1, 1);
//     set_value(m2, 1, 0, 9);
//     print(m2);

//     Matrix p;
//     p = new_matrix(2, 2);
//     multiply(m, m2, p);
//     print(p);
// }