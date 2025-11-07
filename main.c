#include <stdio.h>
#include <math.h>
#include "graph.h"

int main(void)
{
    int choix;
    char inFile[256];
    char outFile[256];

    printf("=== Menu ===\n");
    printf("1) Charger fichier (afficher le graphe)\n");
    printf("2) Verification Markov\n");
    printf("3) Creer fichier Mermaid\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    if (choix == 1) {
        printf("Fichier a charger (ex: data/exemple1.txt) : ");
        scanf("%255s", inFile);
        ListeAdjacence G = readGraph(inFile);
        afficherListeAdjacence(&G);
    }
    else if (choix == 2) {
        printf("Fichier a charger (ex: data/exemple1.txt) : ");
        scanf("%255s", inFile);
        ListeAdjacence G = readGraph(inFile);

        const double eps = 0.01;
        int ok = 1, sommetFaux = -1;
        double sommeFausse = 0.0;

        for (int i = 0; i < G.nbSommets; i++) {
            double s = sommeSortante(&G.tableau[i]);
            printf("Somme sommet %d = %.3f\n", i + 1, s);
            if (fabs(s - 1.0) > eps) {
                ok = 0;
                sommetFaux = i + 1;
                sommeFausse = s;
            }
        }

        if (ok)
            printf("C'est bien un graphe de Markov.\n");
        else
            printf("Ce n'est pas un graphe de Markov car la somme des probabilites du sommet %d est %.2f\n",
                   sommetFaux, sommeFausse);
    }
    else if (choix == 3) {
        printf("Fichier a charger (ex: data/exemple1.txt) : ");
        scanf("%255s", inFile);
        ListeAdjacence G = readGraph(inFile);

        printf("Nom du fichier Mermaid (ex: out/graphe.mmd) : ");
        scanf("%255s", outFile);
        exportToMermaid(&G, outFile);
        printf("Fichier genere : %s\n", outFile);
    }
    else {
        printf("Choix invalide.\n");
    }

    return 0;
}