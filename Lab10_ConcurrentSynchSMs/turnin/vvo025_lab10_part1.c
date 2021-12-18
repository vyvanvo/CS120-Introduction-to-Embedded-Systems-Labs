/*	Author: Vy Vo
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise #1
 *	Exercise Description: Concurrent SMs
 *	Connect LEDs to PB0, PB1, PB2, and PB3. 
In one state machine (ThreeLEDsSM), output to a shared variable (threeLEDs) the following behavior: set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second each. 
In a second state machine (BlinkingLEDSM), output to a shared variable (blinkingLED) the following behavior: set bit 3 to 1 for 1 second, then 0 for 1 second. 
In a third state machine (CombineLEDsSM), combine both shared variables and output to the PORTB. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/I6WkHbYd-Jk
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum TL_States {TL_Start, Init, Next_LED} TL_state;

unsigned char three_led = 0;
void TL_Tick() {	//Three LED SM
	
	switch (TL_state) {	//transitions
		case TL_Start:
			TL_state = Init;
			break;
		
		case Init:
			TL_state = Next_LED;
			break;
			
		case Next_LED:
			if (three_led == 0x04) {
				TL_state = Init;
			}
			else {
				TL_state = Next_LED;
			}
			
			break;
			
		default:
			break;
	}
	
	switch (TL_state) { //state actions
		case Init:
			three_led = 0x01;
			break;
			
		case Next_LED:
			three_led = three_led << 1;
			break;
			
		default:
			break;
	}
	
}

enum BL_States {BL_Start, Off, On} BL_state;

unsigned char blink_led = 0;
void BL_Tick() { //Blink LED SM
	
	switch (BL_state) {	//transitions
		case BL_Start:
			BL_state = Off;
			break;
		
		case Off:
			BL_state = On;
			break;
			
		case On:
			BL_state = Off;
			break;
			
		default:
			break;
			
	}
	
	switch (BL_state) { //state actions
		case Off:
			blink_led = 0x00;
			break;
			
		case On:
			blink_led = 0x08;
			break;
			
		default:
			break;
	}
	
}

enum C_States {C_Start, Combine} C_state;

unsigned char tmpB = 0; //variable to combine
void C_Tick() {

	switch(C_state) { // transitions
		case C_Start:
			C_state = Combine;
			tmpB = 0;
			break;
			
		case Combine:
			C_state = Combine;
			tmpB = 0;	
			break;
			
		default:
			break;
	}
	
	switch (C_state) { //state actions
		case Combine:
			tmpB = tmpB | three_led | blink_led;
			break;
			
		default:
			break;
	}
	
	PORTB = tmpB;
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00; //PORTB -> outputs
	
    /* Insert your solution below */
    TimerSet(1000); //set 1000 ms
    TimerOn();
    
    BL_state = BL_Start;
    TL_state = TL_Start;
    C_state = C_Start;
    
    while (1) {
    	
    	TL_Tick();
    	BL_Tick();
    	C_Tick();
		
		while(!TimerFlag) {}
		TimerFlag = 0;
	
    }
    
    return 1;
}
