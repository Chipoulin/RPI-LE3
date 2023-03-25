#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// Input Croix
#define PIN_CROIX_HAUT RPI_V2_GPIO_P1_37
#define PIN_CROIX_BAS RPI_V2_GPIO_P1_33
#define PIN_CROIX_DROITE RPI_V2_GPIO_P1_35
#define PIN_CROIX_GAUCHE RPI_V2_GPIO_P1_22

// Matrice
#define ROW_COUNT 4
#define COLUMN_COUNT 4

typedef void *(*pfThread)(void *);

void initCroixBtn();
int croixPressed();
void initMatrice();
int activateButton(int row, int column);
int buttonHeldDown(int pin);
int scanMatrix();
void *threadCroix(void *arg);
void *threadMatrice(void *arg);


