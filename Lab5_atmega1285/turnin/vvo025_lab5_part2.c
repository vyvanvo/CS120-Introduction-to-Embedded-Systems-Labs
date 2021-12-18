/*	Author: Vy Vo
 *  	Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 5  Exercise #2
 *	Exercise Description: State Machines
 *	Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/9Bjxa7DuwQo
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait, Increment, Decrement, Wait_A0, Wait_A1, Reset} state;

unsigned char tmpC = 0x07;

void Tick() {
	//read inputs
	unsigned char tmpA = ~PINA & 0x03;

	switch(state) { //transitions
		case Start:
			state = Wait;
			break;
		
		case Wait: 
			if (tmpA == 0x00) {
				state = Wait;
			}
			else if (tmpA == 0x01) {
                state = Increment;
            }
           	else if (tmpA == 0x02) {
                state = Decrement;
            }
            else if (tmpA == 0x03) {
                state = Reset;
			}
                
			break;

		case Increment: 
			if (tmpA == 0x00) {
				state = Wait;
			}
			else if (tmpA == 0x01) {
            	state = Wait_A0;
            }
			else if (tmpA == 0x02) {
				state = Decrement;
			}
			else if (tmpA == 0x03) {
				state = Reset;
			}

			break;

		case Decrement: 
			if (tmpA == 0x00) {
				state = Wait;
			}
			else if (tmpA == 0x01) {
				state = Increment;
			}
			else if (tmpA == 0x02) {
            	state = Wait_A1;
            }
			else if (tmpA == 0x03) {
				state = Reset;
			}

			break;

		case Wait_A0:
			if (tmpA == 0x00) {
				state = Wait;
			}
			else if (tmpA == 0x01) {
				state = Wait_A0;
			}
			else if (tmpA == 0x02) {
				state = Decrement;
			}
			else if (tmpA == 0x03) {
				state = Reset;
			}

			break;

		case Wait_A1:
			if (tmpA == 0x00) {
				state = Wait;
			}
			else if (tmpA == 0x01) {
				state = Increment;
			}
			else if (tmpA == 0x02) {
				state = Wait_A1;
			}
			else if (tmpA == 0x03) {
				state = Reset;
			}

			break;

		case Reset:
			if (tmpA == 0x00) {
				state = Wait;
			}
			else if (tmpA == 0x01) {
                state = Wait_A0;
            }
            else if (tmpA == 0x02) {
        		state = Wait_A1;
    		}
            else if (tmpA == 0x03) {
        		state = Reset;
        	}
                
			break;

		default:
			break;	

	}

	switch(state) { // state actions
		case Wait:
			break;

		case Increment:
			if (tmpC < 9) {
                tmpC++;
            }
			
			break;

		case Decrement:
			if (tmpC > 0) {
				tmpC--;
			}

			break;

		case Wait_A0:
			break;

		case Wait_A1:
			break;

		case Reset:
			tmpC = 0;

			break;

		default:
			break;

	}

	PORTC = tmpC;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // configure port C's 8 pins as outputs
		
	// initialize starting state
	state = Start;
	/* Insert your solution below */
    while (1) {
		Tick();
    }

	return 1;
}
