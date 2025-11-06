#include <stdio.h>
#include <stdlib.h>
#include "cellule.h"

Cellule* creerCellule(int dest, float proba) {
    Cellule *c = (Cellule*)malloc(sizeof(Cellule));
    if (c == NULL) {
        fprintf(stderr, "Erreur: allocation memoire.\n");
        return NULL;
    }
    c->sommet_arrivee = dest;
    c->proba = proba;
    c->suivant = NULL;
    return c;
}

void afficherCellule(const Cellule *c) {
    if (c == NULL) {
        printf("Cellule vide\n");
        return;
    }
    printf("Sommet d'arrivee: %d | Proba: %.2f\n", c->sommet_arrivee, c->proba);
}