/*	Author: Vy Vo
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 5  Exercise #2
 *	Exercise Description: atmega1284 chip
 *	A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light. Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less.  (The example below shows the display for a fuel level of 3).    
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/vrQWb8pcT0k
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
  	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // configure port C's 8 pins as outputs 		

	unsigned char tmpA = 0x00;
	unsigned char tmpC = 0x00;

    	/* Insert your solution below */
    	while (1) {
		//read inputs
		tmpA = ~PINA & 0x0F;

		switch(tmpA) {
			case 0:
				tmpC = 0x40;	//0100 0000
				break;
		
			case 1:
			case 2:
				tmpC = 0x60;	//0110 0000
				break;

			case 3:
			case 4:
				tmpC = 0x70;	//0111 0000
				break;

			case 5:
			case 6:
				tmpC = 0x38;	//0011 1000
				break;

			case 7:
			case 8:
			case 9:
				tmpC = 0x3C;	//0011 1100
				break;

			case 10:
			case 11:
			case 12:
				tmpC = 0x3E;	//0011 1110
				break;

			case 13:
			case 14:
			case 15:
				tmpC = 0x3F;	//0011 1111
				break;

			default:
				break;
	
		}

		PORTC = tmpC;
		
    	}
	
    	return 1;
}
