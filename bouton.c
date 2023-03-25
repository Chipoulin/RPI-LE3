#include "bouton.h"

// set IDs of the buttons in the matrix
int buttonIDs[ROW_COUNT][COLUMN_COUNT] = {
    {1, 2, 3, 0},
    {17, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}};

// declarate GPIO pins for the lines
int rowPins[ROW_COUNT] = {RPI_V2_GPIO_P1_13, RPI_GPIO_P1_15, RPI_V2_GPIO_P1_29, RPI_V2_GPIO_P1_31};

// declarate GPIO pins for the columns
int columnPins[COLUMN_COUNT] = {RPI_V2_GPIO_P1_33, RPI_V2_GPIO_P1_35, RPI_V2_GPIO_P1_37, RPI_V2_GPIO_P1_22};

//--------------------Fonction pour la croix--------------------//

void initCroixBtn()
{
    // Set PIN to be an input
    bcm2835_gpio_fsel(PIN_CROIX_HAUT, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(PIN_CROIX_BAS, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(PIN_CROIX_GAUCHE, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(PIN_CROIX_DROITE, BCM2835_GPIO_FSEL_INPT);
    //  add a pullup
    bcm2835_gpio_set_pud(PIN_CROIX_HAUT, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_set_pud(PIN_CROIX_BAS, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_set_pud(PIN_CROIX_GAUCHE, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_set_pud(PIN_CROIX_DROITE, BCM2835_GPIO_PUD_UP);
    // and enble low
    bcm2835_gpio_len(PIN_CROIX_HAUT);
    bcm2835_gpio_len(PIN_CROIX_BAS);
    bcm2835_gpio_len(PIN_CROIX_GAUCHE);
    bcm2835_gpio_len(PIN_CROIX_DROITE);
}

int croixPressed()
{
    if (bcm2835_gpio_eds(PIN_CROIX_HAUT))
    {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(PIN_CROIX_HAUT);
        // printf("croix haut\n");
        return 1;
    }

    if (bcm2835_gpio_eds(PIN_CROIX_BAS))
    {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(PIN_CROIX_BAS);
        // printf("croix bas\n");
        return 2;
    }

    if (bcm2835_gpio_eds(PIN_CROIX_GAUCHE))
    {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(PIN_CROIX_GAUCHE);
        // printf("croix gauche\n");
        return 3;
    }

    if (bcm2835_gpio_eds(PIN_CROIX_DROITE))
    {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(PIN_CROIX_DROITE);
        // printf("croix droite\n");
        return 4;
    }

    return 0;
}

//--------------Fonction pour la matrice--------------//

int activateButton(int rowPin, int colPin)
{
    // get button number
    int btnIndex = buttonIDs[rowPin][colPin] - 1;
    //printf("button %d pressed\n", btnIndex + 1);
    // prevent several presses on a button in a short time
    usleep(300000);
    return btnIndex;
}

int buttonHeldDown(int pin)
{
    if (bcm2835_gpio_lev(rowPins[pin]) == LOW)
        return 1;
    return 0;
}

void initMatrice()
{
    // define 4 inputs with pull up resistors
    for (int i = 0; i < ROW_COUNT; i++)
        bcm2835_gpio_set_pud(rowPins[i], BCM2835_GPIO_PUD_UP);

    // define 4 outputs ans set them on high
    for (int j = 0; j < COLUMN_COUNT; j++)
    {
        bcm2835_gpio_fsel(columnPins[j], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(columnPins[j], HIGH);
    }
}

int scanMatrix()
{
    int btnIndex = 0;
    for (int j = 0; j < COLUMN_COUNT; j++)
    {
        // every output pin is set on low
        bcm2835_gpio_write(columnPins[j], LOW);
        for (int i = 0; i < ROW_COUNT; i++)
        {
            if (bcm2835_gpio_lev(rowPins[i]) == LOW)
            {
                btnIndex = activateButton(i, j); // on peut mettre sa valeur dans un int
                while (buttonHeldDown(i)); // bouton enfoncé on ne fait rien
                //printf("button %d released\n", btnIndex + 1);
                return btnIndex + 1;
            }
        }
        bcm2835_gpio_write(columnPins[j], HIGH);
    }
}

//--------------Thread--------------//
void *threadCroix(void *arg)
{
    int res;
    initCroixBtn();
    while (1)
    {
        res = croixPressed();
        if (res != 0)
        {
            printf("res %d\n", res);
        }
        delay(500);
    }
}
void *threadMatrice(void *arg)
{
    long *res = (long *)malloc(sizeof(long));
    if (res == NULL)
    {
        printf("Error allocating memory for result pointer\n");
        exit(1);
    }

    initMatrice();
    while (1)
    {
        *res = scanMatrix();
        if (*res != 4)
            pthread_exit((void *)res);
    }
}
//--------------main test--------------//

/*
int main(int argc, char **argv)
{

    if (!bcm2835_init())
    {
        return 1;
    }
    pthread_t idThread;
    pthread_create(&idThread, NULL, (pfThread) threadMatrice, NULL);
    pthread_join(idThread, NULL);
    bcm2835_close();
    return 0;
}*/

/*/ Test croix sans thread
int main(int argc, char **argv)
{

    int res;
    if (!bcm2835_init())
        return 1;

    initCroixBtn();

    while (1)
    {
        res = croixPressed();
        if ( res != 0)
        {
            printf("croix %d \n", res);
        }
        delay(500);
    }
    return 0;
}*/

/*// test matrice
int main()
{
    int res= 0;
    if (!bcm2835_init())
        return 1;

    initMatrice();

    while (1)
    {
        res = scanMatrix();
        if (res != 4)
        {
            printf("res %d\n", res);
        }


    }

    bcm2835_close();
    return 0;
}
*/