#include "hasse.h"
#include <stdlib.h>
#include <stdio.h>

/* ==========================================
   1. FONCTION PROFESSEUR (Sans le log_size)
   ========================================== */
void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->taille_logique)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->taille_logique && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    int k = 0;
                    while (k < p_link_array->taille_logique && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            p_link_array->links[i] = p_link_array->links[p_link_array->taille_logique - 1];
            p_link_array->taille_logique--;
        }
        else
        {
            i++;
        }
    }
}

/* ==========================================
   2. FONCTIONS UTILITAIRES
   ========================================== */

t_link_array create_link_array(int capacite_initiale)
{
    t_link_array arr;
    arr.taille_logique = 0;
    arr.taille_physique = (capacite_initiale > 0) ? capacite_initiale : 4;
    arr.links = (t_link *)malloc(arr.taille_physique * sizeof(t_link));
    if (arr.links == NULL) {
        perror("Erreur malloc create_link_array");
        exit(EXIT_FAILURE);
    }
    return arr;
}

void liberer_link_array(t_link_array *p_link_array)
{
    if (p_link_array == NULL) return;
    if (p_link_array->links != NULL) {
        free(p_link_array->links);
        p_link_array->links = NULL;
    }
    p_link_array->taille_logique = 0;
    p_link_array->taille_physique = 0;
}

static void agrandir_link_array_si_besoin(t_link_array *p_link_array)
{
    if (p_link_array->taille_logique >= p_link_array->taille_physique) {
        int nouvelle_physique = (p_link_array->taille_physique == 0) ? 4 : p_link_array->taille_physique * 2;
        t_link *nouveau = (t_link *)realloc(p_link_array->links, nouvelle_physique * sizeof(t_link));
        if (nouveau == NULL) {
            perror("Erreur realloc agrandir_link_array");
            exit(EXIT_FAILURE);
        }
        p_link_array->links = nouveau;
        p_link_array->taille_physique = nouvelle_physique;
    }
}

static int lien_existe(t_link_array *p_link_array, int from, int to)
{
    for (int i = 0; i < p_link_array->taille_logique; ++i) {
        if (p_link_array->links[i].from == from && p_link_array->links[i].to == to) {
            return 1;
        }
    }
    return 0;
}

void ajouter_lien_si_nouveau(t_link_array *p_link_array, int from, int to)
{
    if (lien_existe(p_link_array, from, to)) return;
    agrandir_link_array_si_besoin(p_link_array);
    p_link_array->links[p_link_array->taille_logique].from = from;
    p_link_array->links[p_link_array->taille_logique].to   = to;
    p_link_array->taille_logique++;
}

/* ==========================================
   3. CONSTRUCTION ET RECHERCHE
   ========================================== */

int trouver_indice_classe_pour_sommet(int sommet, t_partition *partition)
{
    for (int indice_classe = 0; indice_classe < partition->taille_logique; ++indice_classe) {
        t_classe *classe = &partition->classes[indice_classe];
        for (int i = 0; i < classe->taille_logique; ++i) {
            if (classe->identifiants_sommets[i] == sommet) {
                return indice_classe;
            }
        }
    }
    return -1;
}

t_link_array construire_liens_classes(ListeAdjacence *graphe, t_partition *partition)
{
    t_link_array liens = create_link_array(8);
    /* Utilisation de nbSommets (avec S majuscule) */
    int nb_sommets = graphe->nbSommets;

    for (int u = 1; u <= nb_sommets; ++u) {
        int classe_depart = trouver_indice_classe_pour_sommet(u, partition);
        if (classe_depart == -1) continue;

        /* Utilisation de Cellule, tableau, head */
        Cellule *arc = graphe->tableau[u - 1].head;

        while (arc != NULL) {
            /* Utilisation de sommet_arrivee */
            int v = arc->sommet_arrivee;
            int classe_arrivee = trouver_indice_classe_pour_sommet(v, partition);

            if (classe_arrivee != -1 && classe_arrivee != classe_depart) {
                ajouter_lien_si_nouveau(&liens, classe_depart, classe_arrivee);
            }
            /* Utilisation de suivant */
            arc = arc->suivant;
        }
    }
    return liens;
}

/* ==========================================
   4. AFFICHAGE ET ANALYSE
   ========================================== */

void afficher_mermaid(t_partition *partition, t_link_array *p_link_array)
{
    printf("graph TD;\n");
    for (int i = 0; i < partition->taille_logique; ++i) {
        t_classe *c = &partition->classes[i];
        if (c->nom_classe[0] == '\0') sprintf(c->nom_classe, "C%d", i + 1);
        printf("  %s;\n", c->nom_classe);
    }
    for (int i = 0; i < p_link_array->taille_logique; ++i) {
        int from = p_link_array->links[i].from;
        int to = p_link_array->links[i].to;
        printf("  %s --> %s;\n", partition->classes[from].nom_classe, partition->classes[to].nom_classe);
    }
}

void analyser_graphe(t_partition *partition, t_link_array *p_link_array)
{
    int nb_classes = partition->taille_logique;
    int *a_des_sorties = (int *)calloc(nb_classes, sizeof(int));
    if (!a_des_sorties) exit(1);

    for (int i = 0; i < p_link_array->taille_logique; ++i) {
        int from = p_link_array->links[i].from;
        if (from >= 0 && from < nb_classes) a_des_sorties[from] = 1;
    }

    printf("\n--- Analyse des classes ---\n");
    for (int i = 0; i < nb_classes; ++i) {
        t_classe *c = &partition->classes[i];
        printf("%s {", c->nom_classe);
        for (int j = 0; j < c->taille_logique; ++j) {
            printf("%d", c->identifiants_sommets[j]);
            if (j < c->taille_logique - 1) printf(",");
        }
        printf("} : ");

        if (a_des_sorties[i]) {
            printf("TRANSITOIRE\n");
        } else {
            printf("PERSISTANTE (puits)\n");
            if (c->taille_logique == 1) {
                printf("  -> Etat ABSORBANT : %d\n", c->identifiants_sommets[0]);
            }
        }
    }

    if (nb_classes == 1) printf("\nLe graphe est IRREDUCTIBLE.\n");
    else printf("\nLe graphe n'est PAS irreductible (%d classes).\n", nb_classes);

    free(a_des_sorties);
}