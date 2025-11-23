#ifndef INC_2526_TI301_PJT_CALCULS_H
#define INC_2526_TI301_PJT_CALCULS_H

#include "graph.h"    /* Pour ListeAdjacence */
#include "tarjan.h"   /* Pour t_classe */
#include "matrix.h"   /* Pour t_matrix, createMatrix, etc. */

/* --- Conversion Graphe -> Matrice --- */
/* Crée la matrice de transition complète du graphe */
t_matrix grapheToMatrix(const ListeAdjacence *g);

/* --- Calculs de Probabilités (Évolution) --- */
/* Multiplie un vecteur (distribution à l'instant t) par la matrice */
/* Renvoie le nouveau vecteur (distribution à l'instant t+1) */
float* multiplication_vecteur_matrice(float *vecteur, t_matrix M);

/* --- Calcul de la Période (Le défi du sujet) --- */
/* Calcule la période d'une classe donnée */
int calculer_periode_classe(const ListeAdjacence *g, t_classe *c);

#endif //INC_2526_TI301_PJT_CALCULS_H