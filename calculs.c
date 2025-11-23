#include <stdio.h>
#include <stdlib.h>
#include "calculs.h"
/* "calculs.h" inclut déjà "matrix.h", donc on a accès à createMatrix, etc. */

/* ==========================================
   2. CONVERSION & PROBABILITES
   ========================================== */

t_matrix grapheToMatrix(const ListeAdjacence *g) {
    int n = g->nbSommets;
    t_matrix mat = createMatrix(n, n); /* Appel à la fonction de matrix.c */

    for (int i = 0; i < n; i++) {
        /* On parcourt la liste d'adjacence du sommet i+1 */
        Cellule *curr = g->tableau[i].head;
        while (curr != NULL) {
            /* Sommet destination (1..N) -> Indice colonne (0..N-1) */
            int j = curr->sommet_arrivee - 1;
            mat.data[i][j] = curr->proba;
            curr = curr->suivant;
        }
    }
    return mat;
}

float* multiplication_vecteur_matrice(float *v, t_matrix M) {
    /* Allocation du vecteur résultat */
    float *res = (float*)calloc(M.cols, sizeof(float));

    /* Multiplication : res = v * M */
    for (int j = 0; j < M.cols; j++) {
        res[j] = 0.0f;
        for (int i = 0; i < M.rows; i++) {
            res[j] += v[i] * M.data[i][j];
        }
    }
    return res;
}

/* ==========================================
   3. CALCUL DE LA PERIODE (LE DÉFI)
   ========================================== */

/* PGCD (Plus Grand Commun Diviseur) - Algorithme d'Euclide */
/* (Fonction statique = locale à ce fichier, pas de conflit) */
static int pgcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Extrait une petite matrice carrée pour une seule classe */
static t_matrix extract_sub_matrix(const ListeAdjacence *g, t_classe *c) {
    int n = c->taille_logique;
    t_matrix sub = createMatrix(n, n);

    /* Tableaux pour convertir ID Global <-> Indice Local */
    int *map = (int*)malloc(sizeof(int) * (g->nbSommets + 1));
    int *local_to_global = (int*)malloc(sizeof(int) * n);

    for(int k=0; k<=g->nbSommets; k++) map[k] = -1;

    for (int i = 0; i < n; i++) {
        int id_global = c->identifiants_sommets[i];
        local_to_global[i] = id_global;
        map[id_global] = i;
    }

    /* Remplissage de la sous-matrice */
    for (int i = 0; i < n; i++) {
        int u = local_to_global[i]; // Sommet réel

        Cellule *curr = g->tableau[u - 1].head;
        while (curr != NULL) {
            int v = curr->sommet_arrivee;

            if (map[v] != -1) {
                int j = map[v]; // Son indice local (colonne)
                sub.data[i][j] = 1.0f; // On met 1 pour marquer la connexion
            }
            curr = curr->suivant;
        }
    }

    free(map);
    free(local_to_global);
    return sub;
}

int calculer_periode_classe(const ListeAdjacence *g, t_classe *c) {
    /* Cas particulier : classe de taille 1 */
    if (c->taille_logique == 1) {
        int u = c->identifiants_sommets[0];
        Cellule *p = g->tableau[u-1].head;
        while(p) {
            if (p->sommet_arrivee == u) return 1;
            p = p->suivant;
        }
        return 0;
    }

    int n = c->taille_logique;
    t_matrix M = extract_sub_matrix(g, c);
    t_matrix Power = createMatrix(n, n);
    t_matrix Temp = createMatrix(n, n);

    copyMatrix(Power, M);

    int d = 0; // Le PGCD courant

    /* On teste les puissances k */
    for (int k = 1; k <= n + 5; k++) {
        int trace_non_nulle = 0;
        for (int i = 0; i < n; i++) {
            if (Power.data[i][i] > 0.0001f) {
                trace_non_nulle = 1;
                break;
            }
        }

        if (trace_non_nulle) {
            if (d == 0) d = k;
            else d = pgcd(d, k);

            if (d == 1) break;
        }

        multiplyMatrices(Power, M, Temp);
        copyMatrix(Power, Temp);
    }

    freeMatrix(&M);
    freeMatrix(&Power);
    freeMatrix(&Temp);

    return d;
}