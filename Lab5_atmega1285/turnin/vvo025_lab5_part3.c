/*	Author: Vy Vo
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 5  Exercise #3
 *	Exercise Description: atmega1284 chip
 *	Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/6TwQH9a9fz4
 * 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Init, Press_btn, Wait_btn} state;

int LED_seq[17] = {0x15, 0x2A, 0x15, 0x2A, 0x07, 0x38, 0x06, 0x18, 0x04, 0x08, 0x0C, 0x12, 0x21, 0x12, 0x0C, 0x3F, 0x00};
unsigned char i = 0; //iterator i

unsigned char tmpB = 0x00;	//output

void Tick() {
	//read inputs
	unsigned char tmpA = ~PINA & 0x01;

	switch(state) { //transitions
		case Start:
			state = Init;
			break;
			
		case Init:
			state = tmpA? Press_btn : Init;
			break;
			
		case Press_btn:
			if (tmpA) {
				state = Press_btn;
			}
			else {
				state = Wait_btn;
			}
			
			break;
			
		case Wait_btn:
			if (tmpA && i < 16) {
				state = Press_btn;
				i++;	//increment i to go to the next light confirguration in the LED sequence
			}		 
			else if (tmpA && i >= 16) {
				state = Press_btn;
				i = 0;	//at the end of the sequence, set i = 0 to restart the sequence at the beginning
			}
			else if (!tmpA) {	//btn not pressed
				state = Wait_btn;
			}
			else {
				state = Wait_btn; 
			}
		
			break;
			
		default:
			break;
	
	}
			
	switch(state) { //state actions
		case Init:
			tmpB = 0;
			PORTB = tmpB;
			i = 0;
			break;
		
		case Press_btn:
			tmpB = LED_seq[i];
			PORTB = tmpB;
			break;
			
		case Wait_btn:
			break;
			
		default:
			break;
	
	}
		
		
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTC = 0x00; // configure port C's 8 pins as outputs
		
	// initialize starting state
	state = Start;
	/* Insert your solution below */
    while (1) {
		Tick();
    }

	return 1;
}
