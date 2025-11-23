#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclusion des headers pour les définitions de structures (Liste, Cellule, etc.)
#include "graph.h"
// Inclusion des headers pour les fonctions utilitaires (comme getId pour Mermaid)
#include "utils.h"

// Définition de la fonction creerCellule (manquante dans l'extrait, mais nécessaire)
// Cellule *creerCellule(int dest, float proba) { ... }

/**
 * @brief Crée une structure Liste représentant une liste chaînée vide.
 * * Initialise la tête de liste à NULL.
 * Chaque Liste stockera les arêtes sortant d'un sommet spécifique.
 * * @return Liste La nouvelle liste initialisée.
 */
Liste creerListeVide(void)
{
    Liste L;
    L.head = NULL;
    return L;
}

/**
 * @brief Affiche le contenu d'une liste chaînée d'arêtes.
 * * Le format d'affichage est (sommet_arrivée, probabilité)[cite: 589].
 * * @param L Pointeur vers la Liste à afficher.
 */
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
        if (p) printf(" -> "); // Affiche la flèche si ce n'est pas le dernier élément
    }
}

/**
 * @brief Crée et initialise la structure ListeAdjacence.
 * * Alloue dynamiquement un tableau de 'nbSommets' Listes, puis initialise
 * chacune d'elles comme une liste vide.
 * * @param nbSommets Le nombre total de sommets dans le graphe.
 * @return ListeAdjacence La structure contenant le tableau de listes.
 */
ListeAdjacence creerListeAdjacence(int nbSommets)
{
    ListeAdjacence G;
    G.nbSommets = nbSommets;
    // Allouer l'espace pour le tableau des 'nbSommets' listes
    G.tableau = (Liste*)malloc(sizeof(Liste) * nbSommets);
    if (!G.tableau) {
        printf("Allocation impossible\n");
        exit(EXIT_FAILURE);
    }
    // Initialiser chaque liste dans le tableau comme une liste vide
    for (int i = 0; i < nbSommets; ++i) {
        G.tableau[i] = creerListeVide();
    }
    return G;
}

/**
 * @brief Affiche l'intégralité de la liste d'adjacence du graphe.
 * * Parcourt le tableau et appelle 'afficherListe' pour chaque sommet.
 * Note : Les sommets sont affichés de 1 à N (i+1), conformément à la numérotation des états.
 * * @param G Pointeur vers la ListeAdjacence à afficher.
 */
void afficherListeAdjacence(const ListeAdjacence *G)
{
    for (int i = 0; i < G->nbSommets; ++i) {
        // Le sommet i (indice 0) correspond à l'état 1 (i + 1)
        printf("Liste pour le sommet %d: ", i + 1);
        afficherListe(&G->tableau[i]);
        printf("\n");
    }
}

/**
 * @brief Ajoute une nouvelle arête (cellule) à une liste d'adjacence.
 * * Implémente une **insertion en tête** pour une complexité O(1).
 * C'est le choix d'implémentation le plus simple et le plus rapide pour ce contexte,
 * même si cela inverse l'ordre des arêtes par rapport au fichier source.
 * * @param L Pointeur vers la Liste à modifier.
 * @param dest Sommet d'arrivée de l'arête (int).
 * @param proba Probabilité de transition (float).
 */
void ajouterCellule(Liste *L, int dest, float proba)
{
    // créerCellule est supposée exister et allouer une nouvelle Cellule.
    Cellule *c = creerCellule(dest, proba);
    c->suivant = L->head;   // La nouvelle cellule pointe vers l'ancienne tête
    L->head = c;            // La nouvelle cellule devient la nouvelle tête
}

/**
 * @brief Lit un graphe à partir d'un fichier texte et construit la liste d'adjacence.
 * * Le format du fichier est :
 * - Ligne 1: nombre de sommets (N) [cite: 572]
 * - Lignes suivantes: sommet_de_départ sommet_d_arrivée probabilité [cite: 573]
 * * @param filename Le nom du fichier source.
 * @return ListeAdjacence Le graphe construit.
 */
ListeAdjacence readGraph(const char *filename)
{
    FILE *file = fopen(filename, "rt");
    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    }
    int nbvert;

    // Lecture du nombre de sommets
    if (fscanf(file, "%d", &nbvert) != 1) {
        printf("Lecture du nombre de sommets echouee\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialisation de la structure principale
    ListeAdjacence G = creerListeAdjacence(nbvert);
    int depart, arrivee;
    float proba;

    // Boucle de lecture des arêtes (tant que 3 valeurs sont lues)
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        // Vérification de la validité du sommet de départ
        if (depart >= 1 && depart <= nbvert) {
            // Ajout de l'arête. On utilise depart - 1 car les sommets sont numérotés de 1 à N,
            // mais l'indice du tableau en C va de 0 à N-1.
            ajouterCellule(&G.tableau[depart - 1], arrivee, proba);
        }
    }
    fclose(file);
    return G;
}

/**
 * @brief Calcule la somme des probabilités des arêtes sortantes d'un sommet.
 * * Cette fonction est utilisée pour l'Étape 2 de la Partie 1: vérifier que la somme
 * des probabilités sortantes pour chaque état est égale à 1[cite: 659].
 * * @param L Pointeur vers la Liste (arêtes sortantes d'un sommet) à sommer.
 * @return double La somme des probabilités.
 */
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

/**
 * @brief Exporte le graphe au format texte pour l'outil de visualisation Mermaid.
 * * Crée un fichier de sortie avec la syntaxe `flowchart LR` pour dessiner le graphe orienté.
 * Utilise la fonction utilitaire 'getId' pour nommer les sommets (A pour 1, B pour 2, etc.)[cite: 695].
 * Le format d'une arête est `Source -->|Probabilité Destination`.
 * * @param G Pointeur vers la ListeAdjacence à exporter.
 * @param filename  Le nom du fichier de sortie (.mmd ou .txt).
 */
void exportToMermaid(const ListeAdjacence *G, const char *filename)
{
    FILE *f = fopen(filename, "wt");
    if (!f) return;

    // Configuration de base pour l'affichage (comme les exemples du sujet)
    fprintf(f, "config:\n  layout: elk\n  theme: neo\n  look: neo\n---\n");
    fprintf(f, "flowchart LR\n\n");

    // Définition des sommets (ex: A((1)), B((2)), ...)
    for (int i = 0; i < G->nbSommets; i++) {
        // getId(i + 1) donne le nom 'A', 'B', etc. pour le sommet 'i + 1'
        fprintf(f, "%s((%d))\n", getId(i + 1), i + 1);
    }
    fprintf(f, "\n");

    // Définition des arêtes
    for (int i = 0; i < G->nbSommets; i++) {
        Cellule *p = G->tableau[i].head;
        while (p != NULL) {
            char src[8], dst[8];
            // Source : nom du sommet de départ (i + 1)
            strcpy(src, getId(i + 1));
            // Destination : nom du sommet d'arrivée (p->sommet_arrivee)
            strcpy(dst, getId(p->sommet_arrivee));
            // Format de l'arête avec la probabilité
            fprintf(f, "%s -->|%.2f| %s\n", src, p->proba, dst);
            p = p->suivant;
        }
    }
    fclose(f);
}