#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "bouton.h"
#include "matrice.h"

typedef struct ville
{
    char nom[20];
    int coorX;
    int coorY;
    int cible;
} ville;

void afficherMap(WINDOW * win, ville *choices);
void color(int i, int highlight, ville *villes, WINDOW *win);
int validation(WINDOW *win, ville *choices);