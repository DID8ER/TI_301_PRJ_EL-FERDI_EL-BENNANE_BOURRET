#include <stdio.h>
#include <stdlib.h>
#include "tarjan.h"

/* --- Gestion interne de la Pile d'entiers (nécessaire pour l'algo) --- */
typedef struct {
    t_maillon *top;
} t_stack;

static void push_stack(t_stack *s, int val) {
    t_maillon *nouv = (t_maillon*)malloc(sizeof(t_maillon));
    nouv->sommet = val;
    nouv->suivant = s->top;
    s->top = nouv;
}

static int pop_stack(t_stack *s) {
    if (!s->top) return -1;
    t_maillon *tmp = s->top;
    int val = tmp->sommet;
    s->top = tmp->suivant;
    free(tmp);
    return val;
}

static int min(int a, int b) { return (a < b) ? a : b; }

/* Variable globale pour l'ordre de visite */
static int date_decouverte = 0;

/* --- Fonction récursive (Cœur de l'algo) --- */
void parcours_tarjan(int u, const ListeAdjacence *g, t_tarjan_info *infos, t_stack *pile, t_partition *partition) {
    infos[u].num = date_decouverte;
    infos[u].num_accessible = date_decouverte;
    date_decouverte++;

    push_stack(pile, u);
    infos[u].in_stack = 1;

    /* Parcours des voisins (ADAPTÉ à ton graph.h) */
    /* u est le sommet (1..N), le tableau est indexé à 0, donc u-1 */
    Cellule *p = g->tableau[u - 1].head;

    while (p != NULL) {
        int v = p->sommet_arrivee; /* Champ de ta structure Cellule */

        if (infos[v].num == -1) {
            /* Voisin non visité : on descend récursivement */
            parcours_tarjan(v, g, infos, pile, partition);
            infos[u].num_accessible = min(infos[u].num_accessible, infos[v].num_accessible);
        } else if (infos[v].in_stack) {
            /* Voisin déjà dans la pile : c'est un cycle */
            infos[u].num_accessible = min(infos[u].num_accessible, infos[v].num);
        }
        p = p->suivant;
    }

    /* Si u est la racine d'une Composante Fortement Connexe (CFC) */
    if (infos[u].num_accessible == infos[u].num) {
        /* On crée une nouvelle classe */
        t_classe *new_c = (t_classe*)malloc(sizeof(t_classe));
        new_c->id_classe = partition->taille_logique + 1; // ID: 1, 2, ...
        new_c->nom_classe[0] = '\0';
        new_c->taille_logique = 0;
        new_c->identifiants_sommets = NULL; /* On allouera le tableau après */

        /* On utilise une liste temporaire pour stocker les sommets dépilés */
        t_maillon *liste_temp = NULL;
        int v;
        do {
            v = pop_stack(pile);
            infos[v].in_stack = 0;

            /* Ajout en tête de liste temporaire */
            t_maillon *m = (t_maillon*)malloc(sizeof(t_maillon));
            m->sommet = v;
            m->suivant = liste_temp;
            liste_temp = m;

            new_c->taille_logique++;
        } while (u != v);

        /* Conversion de la liste temporaire en tableau pour la structure finale */
        new_c->identifiants_sommets = (int*)malloc(new_c->taille_logique * sizeof(int));
        t_maillon *curr = liste_temp;
        int idx = 0;
        while(curr) {
            new_c->identifiants_sommets[idx++] = curr->sommet;
            t_maillon *tmp = curr;
            curr = curr->suivant;
            free(tmp); // On libère le maillon temporaire
        }

        /* Ajout de la classe à la partition */
        new_c->next = partition->head;
        partition->head = new_c;
        partition->taille_logique++;
    }
}

/* --- Fonction principale Tarjan --- */
t_partition* tarjan(const ListeAdjacence *g) {
    t_partition *part = (t_partition*)malloc(sizeof(t_partition));
    part->head = NULL;
    part->classes = NULL;
    part->taille_logique = 0;

    t_stack pile = {NULL};
    date_decouverte = 0;

    /* Tableau d'infos : taille nbSommets + 1 car les sommets commencent à 1 */
    t_tarjan_info *infos = (t_tarjan_info*)malloc((g->nbSommets + 1) * sizeof(t_tarjan_info));
    for (int i = 1; i <= g->nbSommets; i++) {
        infos[i].num = -1;       /* -1 = non visité */
        infos[i].in_stack = 0;
    }

    /* Lancement du parcours sur tous les sommets non visités */
    for (int i = 1; i <= g->nbSommets; i++) {
        if (infos[i].num == -1) {
            parcours_tarjan(i, g, infos, &pile, part);
        }
    }

    /* Finalisation : Remplir le tableau 'classes' de la partition pour faciliter l'accès dans Hasse */
    /* (On copie les pointeurs de la liste chaînée vers un tableau) */
    if (part->taille_logique > 0) {
        part->classes = (t_classe*)malloc(part->taille_logique * sizeof(t_classe));
        t_classe *curr = part->head;
        /* L'ordre de la liste est inversé par l'ajout en tête, on remplit le tableau pour avoir l'ordre 0..N */
        int index = part->taille_logique - 1;

        while (curr != NULL) {
            // Copie de la structure classe dans le tableau
            part->classes[index] = *curr;
            // Attention: les pointeurs internes (identifiants_sommets) sont partagés, ce qui est voulu.

            // On met à jour l'ID pour qu'il corresponde à l'indice du tableau (0, 1, 2...)
            // C'est crucial pour que l'algo de Hasse fonctionne bien avec les indices de tableau
            part->classes[index].id_classe = index;

            curr = curr->next;
            index--;
        }
    }

    free(infos);
    return part;
}

void afficher_partition(t_partition *p) {
    if (!p || !p->classes) {
        printf("Partition vide ou non initialisee.\n");
        return;
    }

    for (int i = 0; i < p->taille_logique; i++) {
        printf("Composante C%d : { ", i+1); // Affichage 1-based pour l'utilisateur
        for (int j = 0; j < p->classes[i].taille_logique; j++) {
            printf("%d ", p->classes[i].identifiants_sommets[j]);
        }
        printf("}\n");
    }
}

void liberer_partition(t_partition *p) {
    /* Fonction de nettoyage (optionnelle mais recommandée) */
    if (!p) return;
    /* Note : comme on a copié les classes dans un tableau, il faut faire attention au double free.
       Ici, on libère le contenu via le tableau classes. */
    if (p->classes) {
        for (int i = 0; i < p->taille_logique; i++) {
            free(p->classes[i].identifiants_sommets);
        }
        free(p->classes);
    }
    /* On libère la structure de liste chaînée (juste les conteneurs t_classe, pas le contenu interne déjà libéré) */
    t_classe *curr = p->head;
    while(curr) {
        t_classe *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    free(p);
}