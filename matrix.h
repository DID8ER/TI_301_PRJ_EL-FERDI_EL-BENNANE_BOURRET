#ifndef INC_2526_TI301_PJT_MATRIX_H
#define INC_2526_TI301_PJT_MATRIX_H

/* Structure de Matrice générique */
typedef struct {
    int rows;       /* Nombre de lignes */
    int cols;       /* Nombre de colonnes */
    float **data;   /* Tableau 2D */
} t_matrix;

/* Prototypes des fonctions de base (implémentées dans matrix.c) */
t_matrix createMatrix(int rows, int cols);
void freeMatrix(t_matrix *mat);
void displayMatrix(t_matrix mat);
void copyMatrix(t_matrix dest, t_matrix src);
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix res);

#endif //INC_2526_TI301_PJT_MATRIX_H