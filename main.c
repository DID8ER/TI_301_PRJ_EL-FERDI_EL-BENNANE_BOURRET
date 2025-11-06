#include <stdio.h>
#include <stdlib.h>
#include "cellule.h"

int main(void) {
    printf("=== Validation 1 ===\n");
    Cellule *c = creerCellule(3, 0.25f);
    afficherCellule(c);
    free(c);
    return 0;
}
