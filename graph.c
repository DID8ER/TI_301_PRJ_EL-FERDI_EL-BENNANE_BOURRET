#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

Liste creerListeVide(void)
{
    Liste L;
    L.head = NULL;
    return L;
}
void afficherListe(const Liste *L)
{
    const Cellule *p = L->head;
    if (p == NULL) {
        printf("NULL");
        return;
    }
    while (p != NULL) {
        printf("(%d, %.2f)", p->sommet_arrivee, p->proba);
        p = p->suivant;
        if (p) printf(" -> ");
    }
}
ListeAdjacence creerListeAdjacence(int nbSommets)
{
    ListeAdjacence G;
    G.nbSommets = nbSommets;
    G.tableau = (Liste*)malloc(sizeof(Liste) * nbSommets);
    if (!G.tableau) {
        printf("Allocation impossible\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nbSommets; ++i) {
        G.tableau[i] = creerListeVide();
    }
    return G;
}
void afficherListeAdjacence(const ListeAdjacence *G)
{
    for (int i = 0; i < G->nbSommets; ++i) {
        printf("Liste pour le sommet %d: ", i + 1);
        afficherListe(&G->tableau[i]);
        printf("\n");
    }
}
void ajouterCellule(Liste *L, int dest, float proba)
{
    Cellule *c = creerCellule(dest, proba);
    c->suivant = L->head;   // insertion en tête
    L->head = c;
}
ListeAdjacence readGraph(const char *filename)
{
    FILE *file = fopen(filename, "rt");
    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    }
    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        printf("Lecture du nombre de sommets echouee\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    ListeAdjacence G = creerListeAdjacence(nbvert);
    int depart, arrivee;
    float proba;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart >= 1 && depart <= nbvert) {
            ajouterCellule(&G.tableau[depart - 1], arrivee, proba);
        }
    }
    fclose(file);
    return G;
}
double sommeSortante(const Liste *L)
{
    double s = 0.0;
    const Cellule *p = L->head;
    while (p != NULL) {
        s += p->proba;
        p = p->suivant;
    }
    return s;
}
void exportToMermaid(const ListeAdjacence *G, const char *filename)
{
    FILE *f = fopen(filename, "wt");
    if (!f) return;
    fprintf(f, "---\n");
    fprintf(f, "config:\n  layout: elk\n  theme: neo\n  look: neo\n---\n");
    fprintf(f, "flowchart LR\n\n");
    for (int i = 0; i < G->nbSommets; i++) {
        fprintf(f, "%s((%d))\n", getId(i + 1), i + 1);
    }
    fprintf(f, "\n");
    for (int i = 0; i < G->nbSommets; i++) {
        Cellule *p = G->tableau[i].head;
        while (p != NULL) {
            char src[8], dst[8];
            strcpy(src, getId(i + 1));
            strcpy(dst, getId(p->sommet_arrivee));
            fprintf(f, "%s -->|%.2f| %s\n", src, p->proba, dst);
            p = p->suivant;
        }
    }
    fclose(f);
}