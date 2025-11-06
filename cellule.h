#ifndef CELLULE_H
#define CELLULE_H

typedef struct cellule {
    int sommet_arrivee;
    float proba;
    struct cellule *suivant;
} Cellule;

Cellule* creerCellule(int dest, float proba);
void afficherCellule(const Cellule *c);

#endif