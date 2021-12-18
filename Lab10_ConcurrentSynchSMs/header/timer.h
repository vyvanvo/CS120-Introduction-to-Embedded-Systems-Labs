#include <avr/interrupt.h>

//timer
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmers should clear to 0

void TimerISR() {
	TimerFlag = 1;
}

//internal varaibles for mapping AVR's ISR to our cleaner Timer ISR model.
unsigned long avr_timer_M = 1; //start count from here, down to 0. default 1 ms
unsigned long avr_timer_cntcurr = 0; //current internal count of 1 ms ticks

void TimerOn() {
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;	//bit3 = 0: CTC mode (clear timer on compare)
					//bit2bit1bit0 = 011: prescaler /64
					//0000 1011: 0x0B
					//so, 8MHz clock or 8,000,000 /64 = 125,000 ticks/s
					//thus, TCNT1 register will count at 125,000 ticks/s
					
	//AVR output compare register OCR1A
	OCR1A = 125; 	//timer interrupt will be generated when TCNT1 == OCR1A
					//we want a 1 ms tick. 0.001s * 125,000 ticks/s = 125
					//so when TCNT1 register equals 125,
					//1 ms has passed. Thus, we compare to 125
					
	//AVR timer interrupt mask register
	TIMSK1 = 0x02; //bit1: OCIE1A -- enables compare match interrupt
	
	//Initial avr counter
	TCNT1 = 0;
	
	avr_timer_cntcurr = avr_timer_M;
	//TimerISR will be called every avr_timer_cntcurr milliseconds
	
	//enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; //bit3bit1bit0 = 000: timer off
}

//In our approach, the C programmer does not touch this IDR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	//CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	avr_timer_cntcurr--; //count down to 0 rather than up to TOP
	if (avr_timer_cntcurr == 0) { //results in a more efficient compare
		TimerISR(); //call the ISR that the user uses
		avr_timer_cntcurr = avr_timer_M;
	}
	
}

void TimerSet(unsigned long M) {
	avr_timer_M = M;
	avr_timer_cntcurr = avr_timer_M;
}

