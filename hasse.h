#ifndef __HASSE_H__
#define __HASSE_H__

#include <stdio.h>
#include "tarjan.h"   // Nécessaire pour t_partition
#include "cellule.h"  // Nécessaire pour t_adjlist (Attention au nom: cellule.h ou graph.h selon ton projet)
#include "graph.h"

/* ==========================================
   STRUCTURES (A NE PAS TOUCHER)
   ========================================== */

typedef struct {
    int from;   /* indice de classe de départ */
    int to;     /* indice de classe d'arrivée */
} t_link;

typedef struct {
    t_link *links;        /* tableau dynamique de liens */
    int taille_logique;   /* nombre de liens utilisés */
    int taille_physique;  /* capacité du tableau */
} t_link_array;

/* ==========================================
   FONCTION DONNÉ / DEMANDÉ
   ========================================== */

/* Fonction fournie ou demandée explicitement dans le sujet */
void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 * (C'est la fonction que nous avons appelée construire_liens_classes)
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */
t_link_array construire_liens_classes(ListeAdjacence *graphe, t_partition *partition);

/* ==========================================
   FONCTIONS UTILITAIRES (Pas touche non plus)
   ========================================== */

/* Création / destruction de tableau de liens */
t_link_array create_link_array(int capacite_initiale);
void         liberer_link_array(t_link_array *p_link_array);

/* Ajout d'un lien (sans doublons) */
void ajouter_lien_si_nouveau(t_link_array *p_link_array, int from, int to);

/* Affichage et Analyse */
void afficher_mermaid(t_partition *partition, t_link_array *p_link_array);
void analyser_graphe(t_partition *partition, t_link_array *p_link_array);

#endif