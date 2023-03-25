#include"matrice.h"

void InitMatrice()
{
	if (!bcm2835_init()) {
		printf("Unable to init bcm2835.\n");
		return;
	}
	
    bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); // The default
	bcm2835_gpio_fsel(MATRICE_PIN_CS, BCM2835_GPIO_FSEL_OUTP); 
	bcm2835_gpio_write(explo[0][0],HIGH);
	DELAY(50);

	WriteMatrice(0x09,0x00);
	WriteMatrice(0x0a,0x03);
	WriteMatrice(0x0b,0x07);
	WriteMatrice(0x0c,0x01);
	WriteMatrice(0x0f,0x00);
}

void WriteMatrice_byte(char DATA)
{
	bcm2835_gpio_write(MATRICE_PIN_CS,LOW);
	bcm2835_spi_transfer(DATA);
}

void WriteMatrice(char address1, char dat1){
    bcm2835_gpio_write(MATRICE_PIN_CS,LOW);
	WriteMatrice_byte(address1);
	WriteMatrice_byte(dat1);
	bcm2835_gpio_write(MATRICE_PIN_CS,HIGH);
}

void PrintWarning() {
	for(char i = 1; i < 9; i++) {
		WriteMatrice(i, warning[i-1]);
	}
}

void PrintNuclear() {
	for(char i = 1 ;i < 9; i++) {
		WriteMatrice(i, nuclear[i-1]);
	}
}

void ExploSequence() {
    for(char i = 0; i < 3; i++){
        for(char y = 1; y < 9; y++) {
            WriteMatrice(y, explo[i][y-1]);
        }
        DELAY(1000);
    }

	CleanMatrix();
    
	DELAY(1000);

	for(char i = 1; i < 9; i++) {
        WriteMatrice(i, explo[2][i-1]);
    }
    
	DELAY(1000);

	CleanMatrix();
    
	DELAY(1000);

	for(char i = 1; i < 9; i++) {
        WriteMatrice(i, explo[2][i-1]);
    }
    
	DELAY(1000);

	CleanMatrix();
    
	DELAY(1000);

    for(char i = 3; i < 7; i++){
        for(char y = 1; y < 9; y++) {
            WriteMatrice(y, explo[i][y-1]);
        }
        DELAY(1000);
    }
}

void CleanMatrix() {
    for(char i = 1;i < 9;i++) {
		WriteMatrice(i, clean[i-1]);
	}
}

 void main()
 {
 	InitMatrice();
 	PrintWarning();
 	DELAY(1000);
 	PrintNuclear();
 	DELAY(1000);
 	ExploSequence();
 	CleanMatrix();
 }