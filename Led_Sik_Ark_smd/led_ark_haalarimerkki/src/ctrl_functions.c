#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "functions.h"
#include "confs.h"


//Duty cycles for each led by index
uint8_t dutyCycles[90] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//Bit mast for each led by index
bool maskLEDs[90] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1};


//Inits all anodes low and cathodes low (=> no leds should be on)
void initPorts(void){
	//all are set as output
	DDRD = 0xEF; 
	DDRC = 0xFF;  
	DDRB = 0xFF;
	DDRE = 0x0F;

	//Set all anodes low
	PORTD = 0x00;
	PORTB = 0x00;
	PORTE = 0x00;
	
	//Set all cathodes high
	PORTC = 0x3f;
	PORTE = 0x0C;
	PORTB |= (1<<2);


	//enable sleep
	SMCR = (1<<SM1) | (1<<SE); //Enables power down mode 
	PRR0 = (1<<PRTWI0) |(1<<PRTIM2) |(1<<PRTIM0) |(1<<PRUSART1) |(1<<PRTIM1) | (1<<PRSPI0) | (1<<PRUSART0) | (1<<PRADC); //parts to be shut down
	MCUCR = (1<<BODS) | (1<<BODSE);
	

	//Enable interrupts for button
	PCICR |= (1<<PCIE2);	//Enable pin change interrupt for PCINT[23:16]
	PCIFR |= (1<<PCIF2);	//Setting interrupt flag for PCINT20
	PCMSK2 |= (1<<PCINT20);	//Mask for enabling external interrupt PCINT20
	sei();				//Enables all interrupts

}


//Macros for lighting and shutting led in a matrix
//#define SET_LED_MATRIX(x, y) 		{*ledPorts[0][x] |= (1<<pinIndexPointer[0][x]), *ledPorts[1][y]  &= ~(1<<pinIndexPointer[1][y]);}
//#define SHUT_LED_MATRIX(x, y)		{*ledPorts[0][x] &= ~(1<<pinIndexPointer[0][x]), *ledPorts[1][y] |= (1<<pinIndexPointer[1][y]);}


#define SET_LED_ANODE(x) 	{*ledPorts[0][x] |= (1<<pinIndexPointer[0][x]);}
#define SET_LED_CATHODE(y) 	{*ledPorts[1][y] &= ~(1<<pinIndexPointer[1][y]);}

#define SHUT_LED_ANODE(x)	{*ledPorts[0][x] &= ~(1<<pinIndexPointer[0][x]);}
#define SHUT_LED_CATHODE(y)	{*ledPorts[1][y] |= (1<<pinIndexPointer[1][y]);}


//Delay macro for functions 
#define DELAY_US(us)	{while(us--){_delay_us(1);}}



//Simple set pin high or low by changing state from 0 <-> 1 and pinNumber
//
//WARNING!! This may cause current overload if too many LEDs are ON
//
void setPin(uint8_t cathodeState, uint8_t anodeState, uint8_t LEDNumber){
	uint8_t firstDec = 0, secDec = 0;
	uint8_t newNumber = LEDNumber;

	#ifdef VER1_0//Required only for v1.0 (fixes wrong order of leds)
		if(LEDNumber == 45) newNumber = 50;
		if(LEDNumber == 46) newNumber = 53;
		if(LEDNumber == 47) newNumber = 45;
		if(LEDNumber == 48) newNumber = 46;
		if(LEDNumber == 49) newNumber = 47;
		if(LEDNumber == 50) newNumber = 48;
		if(LEDNumber == 51) newNumber = 49;
		if(LEDNumber == 52) newNumber = 51;
		if(LEDNumber == 53) newNumber = 52;

		if(LEDNumber == 54) newNumber = 59;
		if(LEDNumber == 55) newNumber = 62;
		if(LEDNumber == 56) newNumber = 54;
		if(LEDNumber == 57) newNumber = 55;
		if(LEDNumber == 58) newNumber = 56;
		if(LEDNumber == 59) newNumber = 57;
		if(LEDNumber == 60) newNumber = 58;
		if(LEDNumber == 61) newNumber = 60;
		if(LEDNumber == 62) newNumber = 61;

		if(LEDNumber == 63) newNumber = 68;
		if(LEDNumber == 64) newNumber = 71;
		if(LEDNumber == 65) newNumber = 63;
		if(LEDNumber == 66) newNumber = 64;
		if(LEDNumber == 67) newNumber = 65;
		if(LEDNumber == 68) newNumber = 66;
		if(LEDNumber == 69) newNumber = 67;
		if(LEDNumber == 70) newNumber = 69;
		if(LEDNumber == 71) newNumber = 70;

		if(LEDNumber == 72) newNumber = 77;
		if(LEDNumber == 73) newNumber = 80;
		if(LEDNumber == 74) newNumber = 72;
		if(LEDNumber == 75) newNumber = 73;
		if(LEDNumber == 76) newNumber = 74;
		if(LEDNumber == 77) newNumber = 75;
		if(LEDNumber == 78) newNumber = 76;
		if(LEDNumber == 79) newNumber = 78;
		if(LEDNumber == 80) newNumber = 79;
	
		if(LEDNumber == 81) newNumber = 82;
		if(LEDNumber == 82) newNumber = 83;
		if(LEDNumber == 83) newNumber = 84;
		if(LEDNumber == 84) newNumber = 85;
		if(LEDNumber == 85) newNumber = 86;
		if(LEDNumber == 86) newNumber = 87;
		if(LEDNumber == 87) newNumber = 88;
		if(LEDNumber == 88) newNumber = 89;
		if(LEDNumber == 89) newNumber = 90;
	#endif

	
	/////////
	if(newNumber >= 9) newNumber = newNumber+1;
	if(newNumber >= 19) newNumber = newNumber+1;
	if(newNumber >= 29) newNumber = newNumber+1;
	if(newNumber >= 39) newNumber = newNumber+1;
	if(newNumber >= 49) newNumber = newNumber+1;
	if(newNumber >= 59) newNumber = newNumber+1;
	if(newNumber >= 69) newNumber = newNumber+1;
	if(newNumber >= 79) newNumber = newNumber+1;
	#ifndef VER1_0 //for fixed version
		if(newNumber >= 89) newNumber = newNumber+1;
	#endif  
	
	//Checks tenth
	firstDec = newNumber%10;
	//Checks hundreds
	secDec = (newNumber/10)%10;	

	if(anodeState == 1){ SET_LED_ANODE(secDec);
	}else if(anodeState == 0){ SHUT_LED_ANODE(secDec);}
	
	//Light LED if true, else LED is shut down 
	if(cathodeState == 1){ SET_LED_CATHODE(firstDec);
	}else if(cathodeState == 0){ SHUT_LED_CATHODE(firstDec);}
}




//Checks all LEDs that if they have been set some value to be run and blinks those
void checkAndBlinkLEDs(uint8_t loopTimes, uint8_t period_us){
	uint16_t time0 = 0, time1 = 0, time2 = 0, time3 = 0, time4 = 0, time5 = 0, time6 = 0, time7 = 0, time8 = 0;

	for(uint8_t loop = 0; loop < loopTimes; loop++){
		for(uint8_t led = 0; led < LEDS; led = led+9){
			//Calculate off time for all three LEDs
			time0 = dutyCycles[led]*period_us/100;
			time1 = dutyCycles[led+1]*period_us/100;
			time2 = dutyCycles[led+2]*period_us/100;
			time3 = dutyCycles[led+3]*period_us/100;
			time4 = dutyCycles[led+4]*period_us/100;
			time5 = dutyCycles[led+5]*period_us/100;
			time6 = dutyCycles[led+6]*period_us/100;
			time7 = dutyCycles[led+7]*period_us/100;
			time8 = dutyCycles[led+8]*period_us/100;

			setPin(2, 1, led);
		
			//Set LEDs with duty cycle
			if((dutyCycles[led] > 0) && (maskLEDs[led] == true)) setPin(1, 2, led);
			if((dutyCycles[led+1] > 0) && (maskLEDs[led+1] == true)) setPin(1, 2, led+1);
			if((dutyCycles[led+2] > 0) && (maskLEDs[led+2] == true)) setPin(1, 2, led+2);
			if((dutyCycles[led+3] > 0) && (maskLEDs[led+3] == true)) setPin(1, 2, led+3);
			if((dutyCycles[led+4] > 0) && (maskLEDs[led+4] == true)) setPin(1, 2, led+4);
			if((dutyCycles[led+5] > 0) && (maskLEDs[led+5] == true)) setPin(1, 2, led+5);
			if((dutyCycles[led+6] > 0) && (maskLEDs[led+6] == true)) setPin(1, 2, led+6);
			if((dutyCycles[led+7] > 0) && (maskLEDs[led+7] == true)) setPin(1, 2, led+7);
			if((dutyCycles[led+8] > 0) && (maskLEDs[led+8] == true)) setPin(1, 2, led+8);
		

			//Actual delay loop and shutting LEDs down
			for(uint8_t loopNum = 0; loopNum <= period_us; loopNum++){ 	
				if(loopNum == time0){ if((dutyCycles[led] > 0) && (maskLEDs[led] == true)) setPin(0, 2, led);}
				if(loopNum == time1){ if((dutyCycles[led+1] > 0) && (maskLEDs[led+1] == true)) setPin(0, 2, led+1);}
				if(loopNum == time2){ if((dutyCycles[led+2] > 0) && (maskLEDs[led+2] == true)) setPin(0, 2, led+2);}
				if(loopNum == time3){ if((dutyCycles[led+3] > 0) && (maskLEDs[led+3] == true)) setPin(0, 2, led+3);}
				if(loopNum == time4){ if((dutyCycles[led+4] > 0) && (maskLEDs[led+4] == true)) setPin(0, 2, led+4);}
				if(loopNum == time5){ if((dutyCycles[led+5] > 0) && (maskLEDs[led+5] == true)) setPin(0, 2, led+5);}
				if(loopNum == time6){ if((dutyCycles[led+6] > 0) && (maskLEDs[led+6] == true)) setPin(0, 2, led+6);}
				if(loopNum == time7){ if((dutyCycles[led+7] > 0) && (maskLEDs[led+7] == true)) setPin(0, 2, led+7);}
				if(loopNum == time8){ if((dutyCycles[led+8] > 0) && (maskLEDs[led+8] == true)) setPin(0, 2, led+8);}
				_delay_us(1);
			}

			setPin(2, 0, led);
		}
	}
}




/////////////////////////////////////////////////////
//
//
//	Actual functions for images
//
//
/////////////////////////////////////////////////////

void lightWaveAll(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Loops from minimum brightness to maximum
	for(uint8_t percent = minBrightness; percent <= maxBrightness; percent++){
		for(uint8_t led = 0; led < LEDS; led++){
			dutyCycles[led] = percent;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutWaveAll(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Loops from maximun brightness to minimum
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 0; led < LEDS; led++){
			dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}


void lightLine(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t led = 0; led < LEDS; led++) {dutyCycles[led] = 0;}
	//Offset increase for moving image
	for(uint8_t offset = 0; offset < LEDS; offset++){
		for(uint8_t led = 0; led <= offset; led++){
			dutyCycles[led] = maxBrightness;
		}checkAndBlinkLEDs(slowing, period_us);
	}
	
}

void shutLine(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t led = 0; led < LEDS; led++) {dutyCycles[led] = maxBrightness;}
	//Offset increase for moving image
	for(uint8_t offset = 0; offset < LEDS; offset++){
		for(uint8_t led = 1; led <= offset; led++){
			dutyCycles[led] = 0;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void LightWaves(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LED
	for(uint8_t led = 0; led < LEDS; led = led+10){
		dutyCycles[led] = minBrightness+1;
		dutyCycles[led+1] = maxBrightness/4;
		dutyCycles[led+2] = maxBrightness*2/4;
		dutyCycles[led+3] = maxBrightness*3/4;
		dutyCycles[led+4] = maxBrightness;
		dutyCycles[led+5] = maxBrightness;
		dutyCycles[led+6] = maxBrightness*3/4;
		dutyCycles[led+7] = maxBrightness*2/4;
		dutyCycles[led+8] = maxBrightness/4;
		dutyCycles[led+9] = minBrightness+1;
	}

	for(uint8_t led = 0; led < LEDS; led++) {maskLEDs[led] = false;}

	for(uint8_t led = 0; led < LEDS+90; led++){
		//1st wave
		if(led > 0 && led < 90) maskLEDs[led-1] = false;
		if(led <= 85){
			maskLEDs[led] = true;
			maskLEDs[led+1] = true;
			maskLEDs[led+2] = true;
			maskLEDs[led+3] = true;
			maskLEDs[led+4] = true;
		}else if(led > 85 && led < 90) maskLEDs[led] = false;

		if(led == 86) maskLEDs[0] = true; 
		else if(led == 90) maskLEDs[0] = false;

		//2nd wave
		if(led > 15 && led <= 105) maskLEDs[led-16] = false;
		if(led >= 15 && led <= 100){
			maskLEDs[led-15] = true;
			maskLEDs[led-14] = true;
			maskLEDs[led-13] = true;
			maskLEDs[led-12] = true;
			maskLEDs[led-11] = true;
		}else if(led > 100 && led < 105) maskLEDs[led-15] = false;
		
		if(led == 101) maskLEDs[0] = true; 
		else if(led == 105) maskLEDs[0] = false;

		//3rd wave
		if(led > 30 && led <= 120) maskLEDs[led-31] = false;
		if(led >= 30 && led <= 115){
			maskLEDs[led-30] = true;
			maskLEDs[led-29] = true;
			maskLEDs[led-28] = true;
			maskLEDs[led-27] = true;
			maskLEDs[led-26] = true;
		}else if(led > 115 && led < 120) maskLEDs[led-30] = false;
		
		if(led == 116) maskLEDs[0] = true; 
		else if(led == 120) maskLEDs[0] = false;

		//4th wave
		if(led > 45 && led <= 135) maskLEDs[led-46] = false;
		if(led >= 45 && led <= 130){
			maskLEDs[led-45] = true;
			maskLEDs[led-44] = true;
			maskLEDs[led-43] = true;
			maskLEDs[led-42] = true;
			maskLEDs[led-41] = true;
		}else if(led > 130 && led < 135) maskLEDs[led-45] = false;
		
		if(led == 131) maskLEDs[0] = true; 
		else if(led == 135) maskLEDs[0] = false; 

		//5th wave
		if(led > 60 && led <= 150) maskLEDs[led-61] = false;
		if(led >= 60 && led <= 145){
			maskLEDs[led-60] = true;
			maskLEDs[led-59] = true;
			maskLEDs[led-58] = true;
			maskLEDs[led-57] = true;
			maskLEDs[led-56] = true;
		}else if(led > 145 && led < 150) maskLEDs[led-60] = false;
		
		if(led == 146) maskLEDs[0] = true; 
		else if(led == 150) maskLEDs[0] = false; 


		checkAndBlinkLEDs(slowing, period_us);
	}
	for(uint8_t led = 0; led < LEDS; led++) {maskLEDs[led] = true;}
}


void lightVertical(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Light blocks vertical
	for(uint8_t percent = minBrightness; percent < maxBrightness; percent++){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led >= 24 && led <= 28) dutyCycles[led] = percent;
			else if(led >= 31 && led <= 36) dutyCycles[led] = percent;
			else if(led >= 38 && led <= 40) dutyCycles[led] = percent;
			else if(led >= 42 && led <= 44) dutyCycles[led] = percent;
			else if(led >= 46 && led <= 48) dutyCycles[led] = percent;
			else if(led >= 50 && led <= 52) dutyCycles[led] = percent;
			else if(led >= 54 && led <= 59) dutyCycles[led] = percent;
			else if(led >= 62 && led <= 66) dutyCycles[led] = percent;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutVertical(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Shut blocks Vertical
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led >= 24 && led <= 28) dutyCycles[led] = percent-1;
			else if(led >= 31 && led <= 36) dutyCycles[led] = percent-1;
			else if(led >= 38 && led <= 40) dutyCycles[led] = percent-1;
			else if(led >= 42 && led <= 44) dutyCycles[led] = percent-1;
			else if(led >= 46 && led <= 48) dutyCycles[led] = percent-1;
			else if(led >= 50 && led <= 52) dutyCycles[led] = percent-1;
			else if(led >= 54 && led <= 59) dutyCycles[led] = percent-1;
			else if(led >= 62 && led <= 66) dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(slowing, period_us);
	}

}

void lightHorizonta(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Light blocks horizontal
	for(uint8_t percent = minBrightness; percent < maxBrightness; percent++){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led >= 19 && led <= 24) dutyCycles[led] = percent;
			else if(led >= 28 && led <= 31) dutyCycles[led] = percent;
			else if(led >= 36 && led <= 38) dutyCycles[led] = percent;
			else if(led >= 40 && led <= 42) dutyCycles[led] = percent;
			else if(led >= 44 && led <= 46) dutyCycles[led] = percent;
			else if(led >= 48 && led <= 50) dutyCycles[led] = percent;
			else if(led >= 52 && led <= 54) dutyCycles[led] = percent;
			else if(led >= 59 && led <= 62) dutyCycles[led] = percent;
			else if(led >= 66 && led <= 71) dutyCycles[led] = percent;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutHorizonta(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Shut blocks horizontal
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led >= 19 && led <= 24) dutyCycles[led] = percent-1;
			else if(led >= 28 && led <= 31) dutyCycles[led] = percent-1;
			else if(led >= 36 && led <= 38) dutyCycles[led] = percent-1;
			else if(led >= 40 && led <= 42) dutyCycles[led] = percent-1;
			else if(led >= 44 && led <= 46) dutyCycles[led] = percent-1;
			else if(led >= 48 && led <= 50) dutyCycles[led] = percent-1;
			else if(led >= 52 && led <= 54) dutyCycles[led] = percent-1;
			else if(led >= 59 && led <= 62) dutyCycles[led] = percent-1;
			else if(led >= 66 && led <= 71) dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(slowing, period_us);
	}

}


void lightCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//lighting center
	for(uint8_t percent = minBrightness; percent < maxBrightness; percent++){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led >= 24 && led <= 66) dutyCycles[led] = percent;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//shutting center
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led >= 24 && led <= 66) dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}


void lightBorder(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Lighting border
	for(uint8_t percent = minBrightness; percent < maxBrightness; percent++){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led <= 23) 		dutyCycles[led] = percent;
			else if(led >= 67) 	dutyCycles[led] = percent;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutBorder(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Shutting border
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led <= 23) 		dutyCycles[led] = percent-1;
			else if(led >= 67) 	dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(slowing, period_us);
	}	
}


void lightBorderHorizontal(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Lighting border
	for(uint8_t percent = minBrightness; percent < maxBrightness; percent++){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led <= 24) 		dutyCycles[led] = percent;
			else if(led >= 28 && led <= 31) dutyCycles[led] = percent;
			else if(led >= 36 && led <= 38) dutyCycles[led] = percent;
			else if(led >= 40 && led <= 42) dutyCycles[led] = percent;
			else if(led >= 44 && led <= 46) dutyCycles[led] = percent;
			else if(led >= 48 && led <= 50) dutyCycles[led] = percent;
			else if(led >= 52 && led <= 54) dutyCycles[led] = percent;
			else if(led >= 59 && led <= 62) dutyCycles[led] = percent;
			else if(led >= 66) 	dutyCycles[led] = percent;
		}checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutBorderHorizontal(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Shutting border
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 0; led < LEDS; led++){
			if(led <= 24) 		dutyCycles[led] = percent-1;
			else if(led >= 28 && led <= 31) dutyCycles[led] = percent-1;
			else if(led >= 36 && led <= 38) dutyCycles[led] = percent-1;
			else if(led >= 40 && led <= 42) dutyCycles[led] = percent-1;
			else if(led >= 44 && led <= 46) dutyCycles[led] = percent-1;
			else if(led >= 48 && led <= 50) dutyCycles[led] = percent-1;
			else if(led >= 52 && led <= 54) dutyCycles[led] = percent-1;
			else if(led >= 59 && led <= 62) dutyCycles[led] = percent-1;
			else if(led >= 66) 	dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(slowing, period_us);
	}	
}





void lightDownAndUp(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LEDS and mask
	for(uint8_t led = 0; led < LEDS; led++) dutyCycles[led] = maxBrightness;
	for(uint8_t led = 0; led < LEDS; led++) maskLEDs[led] = false;

	//Light top LED
	maskLEDs[0] = true;
	checkAndBlinkLEDs(slowing, period_us);

	//light rest of LEDs
	for(uint8_t led = 1; led < LEDS/2; led++){
		maskLEDs[led] = true;
		maskLEDs[LEDS-led] = true;
		checkAndBlinkLEDs(slowing, period_us);
	}
	
	//light middle LED
	maskLEDs[45] = true;
	checkAndBlinkLEDs(slowing, period_us);
}
	
void shutDownAndUp(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Shut top led
	maskLEDs[0] = false;
	checkAndBlinkLEDs(slowing, period_us);

	//Shut rest of LEDs
	for(uint8_t led = 1; led < LEDS/2; led++){
		maskLEDs[led] = false;
		maskLEDs[LEDS-led] = false;
		checkAndBlinkLEDs(slowing, period_us);
	}
	
	//light middle LED
	maskLEDs[45] = false;
	checkAndBlinkLEDs(slowing, period_us);
	
	for(uint8_t led = 0; led < LEDS; led++) {maskLEDs[led] = true;}
}


void lightMiddleToTop(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LEDS and mask
	for(uint8_t led = 0; led < LEDS; led++) dutyCycles[led] = maxBrightness;
	for(uint8_t led = 0; led < LEDS; led++) maskLEDs[led] = false;

	//Light top LED
	maskLEDs[45] = true;
	checkAndBlinkLEDs(slowing, period_us);

	//light rest of LEDs
	for(uint8_t led = 46; led < LEDS; led++){
		maskLEDs[led] = true;
		maskLEDs[LEDS-led] = true;
		checkAndBlinkLEDs(slowing, period_us);
	}
	
	//light middle LED
	maskLEDs[0] = true;
	checkAndBlinkLEDs(slowing, period_us);
}

void shutMiddleToTop(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LEDS and mask
	for(uint8_t led = 0; led < LEDS; led++) dutyCycles[led] = maxBrightness;
	for(uint8_t led = 0; led < LEDS; led++) maskLEDs[led] = true;

	//Shut top LED
	maskLEDs[45] = false;
	checkAndBlinkLEDs(slowing, period_us);

	//Shut rest of LEDs
	for(uint8_t led = 46; led < LEDS; led++){
		maskLEDs[led] = false;
		maskLEDs[LEDS-led] = false;
		checkAndBlinkLEDs(slowing, period_us);
	}
	
	//Shut middle LED
	maskLEDs[0] = false;
	checkAndBlinkLEDs(slowing, period_us);

	for(uint8_t led = 0; led < LEDS; led++) {maskLEDs[led] = true;}
}


void lightMiddleToCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LEDS and mask
	for(uint8_t led = 0; led < LEDS; led++) dutyCycles[led] = maxBrightness;
	for(uint8_t led = 0; led < LEDS; led++) maskLEDs[led] = false;

	//Light top LED
	maskLEDs[45] = true;
	checkAndBlinkLEDs(slowing, period_us);

	//light rest of LEDs
	for(uint8_t led = 46; led < 67; led++){
		maskLEDs[led] = true;
		maskLEDs[LEDS-led] = true;
		checkAndBlinkLEDs(slowing, period_us);
	}
}


void shutMiddleToCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LEDS and mask
	for(uint8_t led = 0; led < LEDS; led++) dutyCycles[led] = maxBrightness;
	for(uint8_t led = 24; led < 67; led++) maskLEDs[led] = true;

	//shut top LED
	maskLEDs[45] = false;
	checkAndBlinkLEDs(slowing, period_us);

	//Shut rest of LEDs
	for(uint8_t led = 46; led < 67; led++){
		maskLEDs[led] = false;
		maskLEDs[LEDS-led] = false;
		checkAndBlinkLEDs(slowing, period_us);
	}

	for(uint8_t led = 0; led < LEDS; led++) {maskLEDs[led] = true;}
}



//////////////////////////////////////////////////////////
//
//
//		Actual images
//
//
//////////////////////////////////////////////////////////



void lightShutWaveAll(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	lightWaveAll(slowing, period_us, minBrightness, maxBrightness);
	shutWaveAll(slowing, period_us, minBrightness, maxBrightness);
}


void lightShutLineWave(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	lightLine(slowing, period_us, minBrightness, maxBrightness);
	shutLine(slowing, period_us, minBrightness, maxBrightness);
	LightWaves(slowing, period_us, minBrightness, maxBrightness);
}


void lightShutHorizontalVertical(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t led = 0; led < LEDS; led++) {dutyCycles[led] = 0;}

	lightVertical(slowing, period_us, minBrightness, maxBrightness);	
	shutVertical(slowing, period_us, minBrightness, maxBrightness);

	lightCenter(slowing, period_us, minBrightness, maxBrightness);
	shutCenter(slowing, period_us, minBrightness, maxBrightness);		

	lightHorizonta(slowing, period_us, minBrightness, maxBrightness);
	shutHorizonta(slowing, period_us, minBrightness, maxBrightness);

	lightBorder(slowing, period_us, minBrightness, maxBrightness);
	shutBorder(slowing, period_us, minBrightness, maxBrightness);		


	lightBorderHorizontal(slowing, period_us, minBrightness, maxBrightness);
	shutBorderHorizontal(slowing, period_us, minBrightness, maxBrightness);
	
	lightWaveAll(slowing, period_us, minBrightness, maxBrightness);
	shutWaveAll(slowing, period_us, minBrightness, maxBrightness);

}


//Lighting center and border at same times
void lightShutBorderAndCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t led = 0; led < LEDS; led++) {dutyCycles[led] = 0;}
	lightCenter(slowing, period_us, minBrightness, maxBrightness);
	lightBorder(slowing, period_us, minBrightness, maxBrightness);
	shutCenter(slowing, period_us, minBrightness, maxBrightness);
	shutBorder(slowing, period_us, minBrightness, maxBrightness);		
}


//Lighting center and border at different times
void lightShutBorderOrCenter(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t led = 0; led < LEDS; led++) {dutyCycles[led] = 0;}
	lightCenter(slowing, period_us, minBrightness, maxBrightness);
	shutCenter(slowing, period_us, minBrightness, maxBrightness);

	lightBorder(slowing, period_us, minBrightness, maxBrightness);
	shutBorder(slowing, period_us, minBrightness, maxBrightness);		
}


void lightShutLineWavesMix(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	lightDownAndUp(slowing, period_us, minBrightness, maxBrightness);
	shutMiddleToTop(slowing, period_us, minBrightness, maxBrightness);

	lightMiddleToTop(slowing, period_us, minBrightness, maxBrightness);
	shutDownAndUp(slowing, period_us, minBrightness, maxBrightness);

	lightMiddleToCenter(slowing, period_us, minBrightness, maxBrightness);
	shutMiddleToCenter(slowing, period_us, minBrightness, maxBrightness);
}






















