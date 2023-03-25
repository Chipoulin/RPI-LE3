#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "bouton.h"

typedef struct ville
{
    char nom[20];
    int coorX;
    int coorY;
    int cible;
} ville;