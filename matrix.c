#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

t_matrix createMatrix(int rows, int cols) {
    t_matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        mat.data[i] = (float*)calloc(cols, sizeof(float)); // Initialise à 0
    }
    return mat;
}

void freeMatrix(t_matrix *mat) {
    if (mat->data) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
        mat->data = NULL;
    }
    mat->rows = 0;
    mat->cols = 0;
}

void displayMatrix(t_matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        printf("| ");
        for (int j = 0; j < mat.cols; j++) {
            printf("%.2f ", mat.data[i][j]);
        }
        printf("|\n");
    }
}

void copyMatrix(t_matrix dest, t_matrix src) {
    for(int i=0; i<src.rows; i++) {
        for(int j=0; j<src.cols; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix res) {
    int n = A.rows;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            res.data[i][j] = 0;
            for(int k=0; k<n; k++) {
                res.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
}