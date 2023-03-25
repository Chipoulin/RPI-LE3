#include <bcm2835.h>
#include <stdio.h>

#define MATRICE_PIN_CS  RPI_GPIO_P1_26
#define DELAY(x) bcm2835_delay((x));

extern char clean[8];
extern char warning[8];
extern char nuclear[8];
extern char explo[7][8];

void InitMatrice();
void WriteMatrice(char address1, char dat1);
void sequence();
void PrintWarning();
void PrintNuclear();
void ExploSequence();
void CleanMatrix();