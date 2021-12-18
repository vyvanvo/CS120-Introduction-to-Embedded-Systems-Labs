/*	Author: Vy Vo
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise #4
 *	Exercise Description: Concurrent SMs
 *	Extend the previous exercise to allow a user to adjust the sound frequency up or down using buttons connected to PA0 (up) and PA1 (down). Using our 1 ms timer abstraction, the fastest you'll be able to pulse is 1 ms on and 1 ms off, meaning 500 Hz. Again: Don’t use the PWM for this task. Hint: You'll probably want to introduce another synchSM that polls the buttons and sets a global variable storing the current frequency that in turn is read by the frequency generator task. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/kLuALkQnDqo
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

enum S_States {S_Start, S_Off, S_On} S_state;

unsigned char speaker = 0;
void S_Tick() {
	
	unsigned char tmpA2 = ~PINA & 0x04;
	static unsigned char tmpB4 = 0;
	
	switch(S_state) {	//transitions
		case S_Start:
			S_state = S_Off;
			break;
			
		case S_Off:
			if(tmpA2) {
				S_state = S_On;
			}
			else {
				S_state = S_Off;
			}
			break;
			
		case S_On:
			if(tmpA2) {
				S_state = S_On;
				tmpB4 = !tmpB4;
			}
			else {
				S_state = S_Off;
			}
			break;
			
		default:
			break;
	}
	
	switch(S_state) {	//state actions
		case S_Off:
			tmpB4 = 0;
			speaker = tmpB4;
			break;
			
		case S_On:
			speaker = (tmpB4 << 4) & 0x10;
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
			tmpB = tmpB | speaker | three_led | blink_led;
			break;
			
		default:
			break;
	}
	
	PORTB = tmpB;
	
}

enum FG_States {FG_Start, FG_Wait, Increase, Decrease} FG_state;

unsigned char i = 4;
void FG_Tick() {
	unsigned char tmpA0 = ~PINA & 0x01;
	unsigned char tmpA1 = ~PINA & 0x02;
	
	switch(FG_state) { //transitions
		case FG_Start:
			FG_state = FG_Wait;
			break;
			
		case FG_Wait:
			if (tmpA0 && !tmpA1) {
				FG_state = Increase;
				if (i > 1) {
					i--;
				}
				
			}
			else if (!tmpA0 && tmpA1) {
				FG_state = Decrease;
				if (i < 8) {
					i++;
				}
				
			}
			else if(!tmpA0 && !tmpA1) {
				FG_state = FG_Wait;
			}
			else {
				FG_state = FG_Wait;
			}
		
			break;
			
		case Increase:
			if (tmpA0 && !tmpA1) {
				FG_state = Increase;
			}
			else if (!tmpA0 && tmpA1) {
				FG_state = Decrease;
				if (i < 8) {
					i++;
				}
				
			}
			else if(!tmpA0 && !tmpA1) {
				FG_state = FG_Wait;
			}
			else {
				FG_state = FG_Wait;
			}
			
			break;
			
		case Decrease:
			if (tmpA0 && !tmpA1) {
				FG_state = Increase;
				if (i > 1) {
					i--;
				}
				
			}
			else if (!tmpA0 && tmpA1) {
				FG_state = Decrease;
			}
			else if(!tmpA0 && !tmpA1) {
				FG_state = FG_Wait;
			}
			else {
				FG_state = FG_Wait;
			}
			
			break;
			
		default:
			break;
	
	}
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; //PORTA -> inputs
	DDRB = 0xFF; PORTB = 0x00; //PORTB -> outputs
	
    /* Insert your solution below */
    TimerSet(1); //set 100 ms
    TimerOn();
    
    unsigned char timer_period = 1;
    unsigned short TL_elapsedTime = 300;
    unsigned short BL_elapsedTime = 1000;
    unsigned short FG_elapsedTime = 50;
    unsigned short S_elapsedTime = i;
    unsigned short C_elapsedTime = i;
    
    BL_state = BL_Start;
    TL_state = TL_Start;
    FG_state = FG_Start;
    S_state = S_Start;
    C_state = C_Start;
   
    while (1) {
    	
    	if (TL_elapsedTime >= 300) {
    		TL_Tick();
    		TL_elapsedTime = 0;
    	}
    	
    	if (BL_elapsedTime >= 1000) {
			BL_Tick();
			BL_elapsedTime = 0;
		}
		
		if (FG_elapsedTime >= 50) {
			FG_Tick();
			FG_elapsedTime = 0;
		}
		
		if(S_elapsedTime >= i) {
			S_Tick();
			S_elapsedTime = 0;
		}
		
		if (C_elapsedTime >= i) {
			C_Tick();
			C_elapsedTime = 0;
		}
		
		while(!TimerFlag) {}
		TimerFlag = 0;
		
		BL_elapsedTime += timer_period;
		TL_elapsedTime += timer_period;
		FG_elapsedTime += timer_period;
		S_elapsedTime += timer_period;
		C_elapsedTime += timer_period;
		
    }
    
    return 1;
}
