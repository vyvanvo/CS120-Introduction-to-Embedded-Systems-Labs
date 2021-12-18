/*	Author: Vy Vo
 *	Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 5  Prelab
 *	Exercise Description: PA0 will be programmed in pull-up mode, meaning that when the pin has no input the program will read it as 1, and when the pin has a 0 input (ground) the program will read it as 0. Note how the above button provides PA0 with either no input (when not pressed, so read as 1) or with 0 (when pressed, so read as 0). Therefore, pressing the button causes PA0 to be read as 0, and releasing as 1.
 *    
 *	Reads an input button and sets two LEDs to 01 (if not pressed) or 10 (if pressed). 
 *   
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef SIMULATE
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    unsigned char led = 0x00;
    unsigned char button = 0x00;
    /* Insert your solution below */
    while (1) {
    	button = ~PINA & 0x01;
    	if(button) {
        	led = (led & 0xFC) | 0x02;
    	} else {
        	led = (led & 0xFC) | 0x01;
    	}

    	PORTB = led; // 0000 1111
    }
    
    return 1;
}
