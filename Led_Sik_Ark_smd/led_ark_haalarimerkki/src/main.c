#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "functions.h"


volatile uint8_t test = 0;

//Interrupt vector
ISR(PCINT2_vect){
	//Disable interrupt flag from PCINT0		
	PCIFR &= ~(1<< PCIF2);

	//Set all anodes low
	PORTD = 0x00;
	PORTB = 0x00;
	PORTE = 0x00;
	
	//Set all cathodes high
	PORTC = 0x3f;
	PORTE = 0x0C;
	PORTB |= (1<<2);
	
	//Usr should stop pushing buttom within 1sec
	//or double interrupt will be the case
	_delay_ms(2000); 

	if(test == 0) test++;
	else test = 0;
	
	//Enable interrupt flag from PCINT0
	PCIFR |= (1<< PCIF2);
}


void main (void) {
	//Init all ports
 	initPorts();
	
	uint8_t maxPercent = 80;
	uint8_t minPercent = 0;
	uint8_t period = 200;
	uint8_t slowing = 1;
	
	//sleep_cpu();

     while (1) {
		if(test != 0) sleep_cpu();



		//image2
		lightShutLineWave(slowing+2, period, minPercent, maxPercent);
		if(test != 0) sleep_cpu();

		//image3
		for(uint8_t loop = 0; loop < 3; loop++){
			lightShutBorderAndCenter(slowing, period, minPercent, maxPercent);
			if(test != 0) sleep_cpu();
		}

		//image4
		for(uint8_t loop = 0; loop < 2; loop++){
			lightShutHorizontalVertical(slowing, period, minPercent, maxPercent);
			if(test != 0) sleep_cpu();
		}

		//image1
		for(uint8_t loop = 0; loop < 3; loop++){
			lightShutWaveAll(slowing, period, 1, maxPercent);
			if(test != 0) sleep_cpu();
		}

		//image2
		lightShutLineWave(slowing+2, period, minPercent, maxPercent);
		if(test != 0) sleep_cpu();


		//image5
		for(uint8_t loop = 0; loop < 3; loop++){
			lightShutBorderOrCenter(slowing, period, minPercent, maxPercent);
			if(test != 0) sleep_cpu();
		}
		
		//image6
		lightShutLineWavesMix(slowing+4, period, minPercent, maxPercent);
		if(test != 0) sleep_cpu();

		
		//image1
		for(uint8_t loop = 0; loop < 3; loop++){
			lightShutWaveAll(slowing, period, 1, maxPercent);
			if(test != 0) sleep_cpu();
		}
		
		//image5
		for(uint8_t loop = 0; loop < 3; loop++){
			lightShutBorderOrCenter(slowing, period, minPercent, maxPercent);
			if(test != 0) sleep_cpu();
		}

     }
} 
