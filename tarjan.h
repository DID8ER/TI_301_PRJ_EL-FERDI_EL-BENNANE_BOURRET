#ifndef INC_2526_TI301_PJT_TARJAN_H
#define INC_2526_TI301_PJT_TARJAN_H

#include "graph.h" /* Nécessaire pour connaître le type ListeAdjacence */

/* --- Structure pour une liste simple d'entiers (utilisée pour la Pile interne) --- */
typedef struct s_maillon {
    int sommet;
    struct s_maillon *suivant;
} t_maillon;

/* --- Structure d'une Classe (Composante Fortement Connexe) --- */
/* C'est ici qu'on définit ce que 'hasse.c' va utiliser */
typedef struct s_classe {
    int id_classe;              /* Identifiant (ex: 1, 2, 3...) */
    char nom_classe[10];        /* Pour l'affichage (ex: "C1") */

    int *identifiants_sommets;  /* Tableau des sommets (ex: {1, 5, 7}) */
    int taille_logique;         /* Nombre de sommets dans la classe */

    struct s_classe *next;      /* Pour faire une liste chaînée de classes */
} t_classe;

/* --- Structure de la Partition (Résultat final de Tarjan) --- */
typedef struct {
    t_classe *classes;      /* Tableau de classes (pour accès rapide dans Hasse) */
    t_classe *head;         /* Liste chaînée des classes (pour parcours simple) */
    int taille_logique;     /* Nombre total de classes trouvées */
} t_partition;

/* --- Structure pour les infos internes de l'algo Tarjan --- */
typedef struct {
    int num;            /* Numéro de visite */
    int num_accessible; /* Low-link value */
    int in_stack;       /* Est-ce dans la pile ? */
} t_tarjan_info;

/* --- Prototypes des fonctions --- */
t_partition* tarjan(const ListeAdjacence *g);
void afficher_partition(t_partition *p);
void liberer_partition(t_partition *p);

#endif //INC_2526_TI301_PJT_TARJAN_H