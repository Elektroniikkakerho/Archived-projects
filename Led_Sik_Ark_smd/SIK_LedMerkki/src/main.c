#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "functions.h"


volatile uint8_t test = 0;

//Interrupt vector
ISR(INT0_vect){
	//Clear interrupt flag for INT0
	GIFR &= ~(1 << INTF0);

	//Set all anodes low
	PORTA = 0x80;
	
	//Set all cathodes high
	PORTB = 0xFF;

	//Usr should stop pushing buttom within 1sec
	//or double interrupt will be the case
	_delay_ms(1000); 

	if(test == 0) test++;
	else test = 0;
}


void main (void) {
	//Init all ports
 	initPorts();
	
	uint8_t maxPercent = 100;
	uint8_t minPercent = 0;
	uint8_t period = 150;
	uint8_t slowing = 5;

	MCUCR = (1 << SE); 
	sleep_cpu(); 
	MCUCR &= ~(1 << SE);

     while (1) {
		
		for(uint8_t loop = 0; loop < 5; loop++){
			if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
			lightShutWaveAll(1, period, 1, maxPercent);
		}
		if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
		lightLine(slowing, period, 1, maxPercent);
		
		if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
		shutWaveAll(1, period, 1, maxPercent);

		if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
		LightWaves(slowing, period, 1, maxPercent);

		if(period > 50) period = period-50;
		else period = 150;


		for(uint8_t loop = 0; loop < 5; loop++){
			if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
			lightShutWaveAll(1, period, 1, maxPercent);
		}

		if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
		lightWaveAll(1, period, 1, maxPercent);

		if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
		shutLine(slowing, period, 1, maxPercent);

		if(test != 0) {MCUCR = (1 << SE); sleep_cpu(); MCUCR &= ~(1 << SE);}
		LightWaves(slowing, period, 1, maxPercent);

		if(period > 50) period = period-50;
		else period = 150;


     }
} 
