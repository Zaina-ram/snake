//TAKEN FROM LAB
#include <stdint.h>
#include <pic32mx.h>
#include "header.h" 

int sw4;
int score;

void user_isr() {
	return;
}
//function to get switch4
int getsw4(void) {
    int temp;
    temp = (PORTD >> 11) & 0x1;
    return temp;
}

int main(void) {
	/*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;

	PORTE = 0x0;
	TRISE &= ~0xff; //set to output (LEDs)
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200; 
	
	/* Set up input pins */
	TRISDSET = (1 << 8); //for btns 2,3,4
	TRISFSET = (1 << 1); //for btn 1
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	display_init(); //Initialize OLED display 
	// display the start page until sw4 is filpped
	while(!sw4) {
	display_string(0, "WELCOME TO");
	display_string(1, "SNAKE!");
	display_string(2, "START: SW4");
	display_update(); 
	sw4 = getsw4();
    }

	// snake x coordintes
	snake.x = snakeX; ¨
	// snake y coordintes
	snake.y = snakeY; 
	//start length
	snake.length = 10;
	//start direction
	snake.dir = 'R'; 
	//start position of snake
	snake.x[0] = 15; 
	snake.x[1] = 14;
	snake.x[2] = 13;
	snake.x[3] = 12;
	snake.x[4] = 11;
	snake.x[5] = 10;
	snake.x[6] = 9;
	snake.x[7] = 8;
	snake.x[8] = 7;
	snake.x[9] = 6;

	snake.y[0] = 3; 
	snake.y[1] = 3;
	snake.y[2] = 3;
	snake.y[3] = 3;
	snake.y[4] = 3;
	snake.y[5] = 3;
	snake.y[6] = 3;
	snake.y[7] = 3;
	snake.y[8] = 3;
	snake.y[9] = 3;

	while(1) {
		start(); //Start display
	}
	return 0;
}