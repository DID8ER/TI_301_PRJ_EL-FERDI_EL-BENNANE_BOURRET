#ifndef GRAPH_H
#define GRAPH_H
#include "cellule.h"

typedef struct {
    Cellule *head;
} Liste;
typedef struct {
    int nbSommets;
    Liste *tableau;
} ListeAdjacence;
Liste    creerListeVide(void);
void     afficherListe(const Liste *L);
ListeAdjacence creerListeAdjacence(int nbSommets);
void           afficherListeAdjacence(const ListeAdjacence *G);
void ajouterCellule(Liste *L, int dest, float proba);
ListeAdjacence readGraph(const char *filename);
double sommeSortante(const Liste *L);
void exportToMermaid(const ListeAdjacence *G, const char *filename);

#endif