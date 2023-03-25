#include"matrice.h"

char clean[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

char warning[8] = {
    0b00011000,
    0b00100100,
    0b01011010,
    0b01011010,
    0b01000010,
    0b10011001,
    0b10000001,
    0b11111111
};

char nuclear[8] = {
    0b01111110,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00111100,
    0b11111111,
    0b11100111,
    0b11000011
};

char explo[7][8] = {
{0b00000000,
 0b00000000,
 0b00000000,
 0b01111110,
 0b01111110,
 0b00000000,
 0b00000000,
 0b00000000},
{0b00000000,
 0b00000000,
 0b00000000,
 0b00111100,
 0b00111100,
 0b00000000,
 0b00000000,
 0b00000000},
{0b00000000,
 0b00000000,
 0b00000000,
 0b00011000,
 0b00011000,
 0b00000000,
 0b00000000,
 0b00000000},
{0b00000000,
 0b00000000,
 0b00011000,
 0b00111100,
 0b00111100,
 0b00011000,
 0b00000000,
 0b00000000},
{0b00000000,
 0b00111100,
 0b01100110,
 0b01000010,
 0b01000010,
 0b01100110,
 0b00111100,
 0b00000000},
{0b00010100,
 0b01000000,
 0b00000001,
 0b10000000,
 0b00000001,
 0b10000000,
 0b00000010,
 0b00101000},
{0b10000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000000,
 0b00000001}
};

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
	char i;
	for(i = 1; i < 9; i++) {
		WriteMatrice(i, warning[i-1]);
	}
}

void PrintNuclear() {
	char i;
	for(i = 1 ;i < 9; i++) {
		WriteMatrice(i, nuclear[i-1]);
	}
}

void ExploSequence() {
	char i, y;
    for(i = 0; i < 3; i++){
        for(y = 1; y < 9; y++) {
            WriteMatrice(y, explo[i][y-1]);
        }
        DELAY(1000);
    }

	CleanMatrix();
    
	DELAY(1000);

	for(i = 1; i < 9; i++) {
        WriteMatrice(i, explo[2][i-1]);
    }
    
	DELAY(1000);

	CleanMatrix();
    
	DELAY(1000);

	for(i = 1; i < 9; i++) {
        WriteMatrice(i, explo[2][i-1]);
    }
    
	DELAY(1000);

	CleanMatrix();
    
	DELAY(1000);

    for(i = 3; i < 7; i++){
        for(y = 1; y < 9; y++) {
            WriteMatrice(y, explo[i][y-1]);
        }
        DELAY(1000);
    }
}

void CleanMatrix() {
	char i;
    for(i = 1;i < 9;i++) {
		WriteMatrice(i, clean[i-1]);
	}
}

void sequence(){
	PrintWarning();
 	DELAY(1000);
 	PrintNuclear();
 	DELAY(1000);
 	ExploSequence();
 	CleanMatrix();
}
/*
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
 */