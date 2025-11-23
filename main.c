#include <stdio.h>
#include <math.h>
#include <stdlib.h> // Nécessaire pour malloc, exit, etc.
#include "graph.h"  // Partie 1
#include "tarjan.h" // Partie 2
#include "hasse.h"  // Partie 2
#include "calculs.h" // AJOUT PARTIE 3 (Matrices et Périodes)
#include "matrix.h" // AJOUT PARTIE 3 (Matrices et Périodes)

int main(void)
{
    int choix;
    char inFile[256];
    char outFile[256];

    /* Déclarations pour la Partie 2 et 3 */
    t_partition *partition = NULL;
    t_link_array liens;

    printf("=== Menu ===\n");
    printf("1) Charger fichier (afficher le graphe)\n");
    printf("2) Verification Markov\n");
    printf("3) Creer fichier Mermaid\n");
    printf("4) Analyse complete (Partie 2 : Tarjan + Hasse + Proprietes)\n");
    printf("5) Probabilites et Periode (Partie 3 : Matrices)\n"); // NOUVEAU
    printf("Votre choix : ");
    scanf("%d", &choix);

    /* --- CHOIX 1 : Affichage (Partie 1) --- */
    if (choix == 1) {
        printf("Fichier a charger (ex: data/exemple1.txt) : ");
        scanf("%255s", inFile);
        ListeAdjacence G = readGraph(inFile);
        afficherListeAdjacence(&G);

        free(G.tableau);
    }

        /* --- CHOIX 2 : Markov (Partie 1) --- */
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

        free(G.tableau);
    }

        /* --- CHOIX 3 : Mermaid Graphe (Partie 1) --- */
    else if (choix == 3) {
        printf("Fichier a charger (ex: data/exemple1.txt) : ");
        scanf("%255s", inFile);
        ListeAdjacence G = readGraph(inFile);

        printf("Nom du fichier Mermaid (ex: out/graphe.mmd) : ");
        scanf("%255s", outFile);
        exportToMermaid(&G, outFile);
        printf("Fichier genere : %s\n", outFile);

        free(G.tableau);
    }

        /* --- CHOIX 4 : Analyse Complète (Partie 2) --- */
    else if (choix == 4) {
        printf("Fichier a charger pour analyse (ex: ../data/exemple_hasse1.txt) : ");
        scanf("%255s", inFile);

        /* 1. Chargement du graphe */
        ListeAdjacence G = readGraph(inFile);

        /* 2. Algorithme de Tarjan (Trouver les classes) */
        printf("\n--- [1] Partition en classes (Tarjan) ---\n");
        partition = tarjan(&G); // On passe l'adresse de G

        if (partition == NULL) {
            printf("Erreur lors de la partition.\n");
            return 1;
        }
        afficher_partition(partition);

        /* 3. Construction des liens (Hasse) */
        printf("\n--- [2] Construction du Diagramme de Hasse ---\n");
        liens = construire_liens_classes(&G, partition);

        /* 4. Suppression des liens transitifs (Optionnel mais propre) */
        removeTransitiveLinks(&liens);

        /* 5. Affichage code Mermaid pour les classes */
        printf("\n--- Code Mermaid ---\n");
        afficher_mermaid(partition, &liens);
        printf("\n-----------------------------------------\n");

        /* 6. Analyse des propriétés (Transitoire, Persistante, Irréductible) */
        analyser_graphe(partition, &liens);

        /* 7. Nettoyage Mémoire */
        liberer_link_array(&liens);
        liberer_partition(partition);
        free(G.tableau);
    }

        /* --- CHOIX 5 : Matrices et Périodes (Partie 3 - AJOUTÉ) --- */
    else if (choix == 5) {
        printf("Fichier a charger pour probabilites (ex: ../data/exemple_meteo.txt) : ");
        scanf("%255s", inFile);

        ListeAdjacence G = readGraph(inFile);

        // 1. Matrice de Transition
        printf("\n--- 1. Matrice de Transition ---\n");
        t_matrix M = grapheToMatrix(&G);
        displayMatrix(M);

        // 2. Simulation d'évolution
        printf("\n--- 2. Simulation d'evolution (10 etapes) ---\n");
        /* Vecteur initial : on part du sommet 1 (100% de chance) */
        float *distrib = (float*)calloc(G.nbSommets, sizeof(float));
        if(G.nbSommets > 0) distrib[0] = 1.0f; // Sommet 1 = index 0

        printf("Etat initial (Sommet 1) : [ ");
        for(int i=0; i<G.nbSommets; i++) printf("%.2f ", distrib[i]);
        printf("]\n");

        for(int t=1; t<=10; t++) {
            float *new_dist = multiplication_vecteur_matrice(distrib, M);
            free(distrib);
            distrib = new_dist;

            printf("Etape %d : [ ", t);
            for(int i=0; i<G.nbSommets; i++) printf("%.2f ", distrib[i]);
            printf("]\n");
        }
        free(distrib);

        // 3. Calcul des Périodes
        printf("\n--- 3. Calcul des Periodes ---\n");
        /* On réutilise Tarjan pour avoir les classes */
        partition = tarjan(&G);

        if (partition != NULL) {
            for(int i=0; i<partition->taille_logique; i++) {
                t_classe *c = &partition->classes[i];
                int periode = calculer_periode_classe(&G, c);

                printf("Classe C%d : { ", c->id_classe + 1);
                for(int k=0; k<c->taille_logique; k++) printf("%d ", c->identifiants_sommets[k]);
                printf("} -> Periode = %d\n", periode);
            }
            liberer_partition(partition);
        }

        // Nettoyage
        freeMatrix(&M);
        free(G.tableau);
    }
    else {
        printf("Choix invalide.\n");
    }

    return 0;
}