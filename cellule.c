#include <stdio.h>
#include <stdlib.h>
#include "cellule.h"

Cellule* creerCellule(int dest, float proba) {
    Cellule *c = (Cellule*)malloc(sizeof(Cellule));
    if (!c) {
        fprintf(stderr, "Erreur: allocation memoire.\n");
        exit(EXIT_FAILURE);
    }
    c->sommet_arrivee = dest;
    c->proba = proba;
    c->suivant = NULL;
    return c;
}