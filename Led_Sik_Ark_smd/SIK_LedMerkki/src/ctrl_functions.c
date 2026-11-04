#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "functions.h"
#include "confs.h"


//Duty cycles for each led by index
uint8_t dutyCycles[49] = {0, 0, 0, 0, 0, 0, 0,
					 0, 0, 0, 0, 0, 0, 0, 
					 0, 0, 0, 0, 0, 0, 0, 
					 0, 0, 0, 0, 0, 0, 0, 
					 0, 0, 0, 0, 0, 0, 0, 
					 0, 0, 0, 0, 0, 0, 0, 
					 0, 0, 0, 0, 0, 0, 0};

//Bit mast for each led by index
bool maskLEDs[49] = {1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 
				 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1};


//Inits all anodes low and cathodes low (=> no leds should be on)
void initPorts(void){
	//all are set as output
	DDRA = 0xFF; 
	DDRB = 0xBF;  

	//Set all anodes low
	PORTA = 0x80;
	
	//Set all cathodes high
	PORTB = 0xFF;

	//Power Reduction Register
	PRR = (1 << PRADC) | (1 << PRUSI);// | (1 << PRTIM0) | (1 << PRTIM1);
	//MCU Control Register
	MCUCR = (1<<BODS) | (1<<BODSE) | (1 << SM1);

	//Enable interrupt mask for INT0
	GIMSK |= (1 << INT0);
	//MCUCR == 0x02 (falling edge interrupt)
	MCUCR |= (1 << ISC01);

	//Enables all interrupts
	sei();

}


#define SET_LED_ANODE(x) 	{*ledPorts[0][x] |= (1<<pinIndexPointer[0][x]);}
#define SET_LED_CATHODE(y) 	{*ledPorts[1][y] &= ~(1<<pinIndexPointer[1][y]);}

#define SHUT_LED_ANODE(x)	{*ledPorts[0][x] &= ~(1<<pinIndexPointer[0][x]);}
#define SHUT_LED_CATHODE(y)	{*ledPorts[1][y] |= (1<<pinIndexPointer[1][y]);}


//Simple set pin high or low by changing state from 0 <-> 1 and pinNumber
//
//WARNING!! This may cause current overload if too many LEDs are ON
//
// Value 2 does nothing, value 1 sets pin ON, value 0 sets pin OFF
void setPin(uint8_t cathodeState, uint8_t anodeState, uint8_t LEDNumber){
	uint8_t firstDec = 0, secDec = 0;
	uint8_t newNumber = LEDNumber;

	if(newNumber >= 7) newNumber = newNumber+3;
	if(newNumber >= 17) newNumber = newNumber+3;
	if(newNumber >= 27) newNumber = newNumber+3;
	if(newNumber >= 37) newNumber = newNumber+3;
	if(newNumber >= 47) newNumber = newNumber+3;
	if(newNumber >= 57) newNumber = newNumber+3;
 
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
	uint16_t time0 = 0, time1 = 0, time2 = 0, time3 = 0, time4 = 0, time5 = 0, time6 = 0;

	for(uint8_t loop = 0; loop < loopTimes; loop++){
		for(uint8_t led = 0; led < LEDS; led = led+7){
			//Calculate off time for all three LEDs
			time0 = dutyCycles[led]*period_us/100;
			time1 = dutyCycles[led+1]*period_us/100;
			time2 = dutyCycles[led+2]*period_us/100;
			time3 = dutyCycles[led+3]*period_us/100;
			time4 = dutyCycles[led+4]*period_us/100;
			time5 = dutyCycles[led+5]*period_us/100;
			time6 = dutyCycles[led+6]*period_us/100;
			
			//Set all anodes of one column ON
			setPin(2, 1, led);
		
			//Set LEDs with duty cycle
			if((dutyCycles[led] > 0) && (maskLEDs[led] == true)) setPin(1, 2, led);
			if((dutyCycles[led+1] > 0) && (maskLEDs[led+1] == true)) setPin(1, 2, led+1);
			if((dutyCycles[led+2] > 0) && (maskLEDs[led+2] == true)) setPin(1, 2, led+2);
			if((dutyCycles[led+3] > 0) && (maskLEDs[led+3] == true)) setPin(1, 2, led+3);
			if((dutyCycles[led+4] > 0) && (maskLEDs[led+4] == true)) setPin(1, 2, led+4);
			if((dutyCycles[led+5] > 0) && (maskLEDs[led+5] == true)) setPin(1, 2, led+5);
			if((dutyCycles[led+6] > 0) && (maskLEDs[led+6] == true)) setPin(1, 2, led+6);

			//Actual delay loop and shutting LEDs down
			for(uint8_t loopNum = 0; loopNum <= period_us; loopNum++){ 	
				if(loopNum == time0){ if((dutyCycles[led] > 0) && (maskLEDs[led] == true)) setPin(0, 2, led);}
				if(loopNum == time1){ if((dutyCycles[led+1] > 0) && (maskLEDs[led+1] == true)) setPin(0, 2, led+1);}
				if(loopNum == time2){ if((dutyCycles[led+2] > 0) && (maskLEDs[led+2] == true)) setPin(0, 2, led+2);}
				if(loopNum == time3){ if((dutyCycles[led+3] > 0) && (maskLEDs[led+3] == true)) setPin(0, 2, led+3);}
				if(loopNum == time4){ if((dutyCycles[led+4] > 0) && (maskLEDs[led+4] == true)) setPin(0, 2, led+4);}
				if(loopNum == time5){ if((dutyCycles[led+5] > 0) && (maskLEDs[led+5] == true)) setPin(0, 2, led+5);}
				if(loopNum == time6){ if((dutyCycles[led+6] > 0) && (maskLEDs[led+6] == true)) setPin(0, 2, led+6);}
				_delay_us(1);
			}

			//Set all anodes of one column OFF
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
		if(offset == 0) {
			dutyCycles[47] = maxBrightness;
			checkAndBlinkLEDs(slowing+8, period_us);
		}
		else if(offset == 1) {
			dutyCycles[48] = maxBrightness;
			dutyCycles[46] = maxBrightness;
			dutyCycles[45] = maxBrightness;
			checkAndBlinkLEDs(slowing+5, period_us);
			 offset+=2;
		}
		else if(offset == 4) {
			dutyCycles[44] = maxBrightness;
			dutyCycles[43] = maxBrightness;
			dutyCycles[42] = maxBrightness;
			dutyCycles[41] = maxBrightness;
			checkAndBlinkLEDs(slowing+3, period_us);
			offset+=3;
		}
		else if(offset > 7 && offset < 48) dutyCycles[offset-7] = maxBrightness;
		else dutyCycles[0] = maxBrightness;

		checkAndBlinkLEDs(slowing, period_us);
	}
}

void shutLine(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	for(uint8_t led = 0; led < LEDS; led++) {dutyCycles[led] = maxBrightness;}
	//Offset increase for moving image
	for(uint8_t offset = 0; offset < LEDS; offset++){
		if(offset == 48) {
			dutyCycles[47] = 0;
			checkAndBlinkLEDs(slowing+8, period_us);
		}
		else if(offset == 45) {
			dutyCycles[48] = 0;
			dutyCycles[46] = 0;
			dutyCycles[45] = 0;
			offset+=2;
			checkAndBlinkLEDs(slowing+5, period_us);
		}
		else if(offset == 41) {
			dutyCycles[44] = 0;
			dutyCycles[43] = 0;
			dutyCycles[42] = 0;
			dutyCycles[41] = 0;
			checkAndBlinkLEDs(slowing+3, period_us);
			offset+=3;
		}
		else if(offset == 39) dutyCycles[0] = 0;
		else if(offset == 40) dutyCycles[1] = 0;
		else if(offset == 26) dutyCycles[8] = 0;
		else if(offset > 26) dutyCycles[offset+2] = 0;
		else if(offset >= 6) dutyCycles[offset+3] = 0;
		else dutyCycles[offset+2] = 0;

		checkAndBlinkLEDs(slowing, period_us);
	}
}


void LightWaves(uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness){
	//Set values for each LED
	for(uint8_t led = 0; led < LEDS-8; led = led+7){
		dutyCycles[led] = minBrightness+1;
		dutyCycles[led+1] = maxBrightness/4;
		dutyCycles[led+2] = maxBrightness/2;
		dutyCycles[led+3] = maxBrightness;
		dutyCycles[led+4] = maxBrightness/2;
		dutyCycles[led+5] = maxBrightness/4;
		dutyCycles[led+6] = minBrightness+1;
	}

	for(uint8_t led = 0; led < LEDS; led++) {maskLEDs[led] = false;}

	//Light ground
	for(uint8_t led = 41; led < LEDS; led++) {maskLEDs[led] = true;}

	for(uint8_t loop = 0; loop < 2; loop++){
		for(uint8_t percent = minBrightness; percent <= maxBrightness; percent++){
			for(uint8_t led = 41; led < LEDS; led++){
				dutyCycles[led] = percent;
			}checkAndBlinkLEDs(1, period_us);
		}
		for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
			for(uint8_t led = 41; led < LEDS; led++){
				dutyCycles[led] = percent-1;
			}checkAndBlinkLEDs(1, period_us);
		}
	}
	for(uint8_t percent = minBrightness; percent <= maxBrightness; percent++){
		for(uint8_t led = 41; led < LEDS; led++){
			dutyCycles[led] = percent;
		}checkAndBlinkLEDs(1, period_us);
	}


	//Ligth waves
	for(uint8_t led = 0; led < LEDS+60; led++){
		//1st wave
		if(led == 2) maskLEDs[1] = false;
		if(led == 3) maskLEDs[2] = false;
		if(led == 4) maskLEDs[3] = false;
		if(led > 4 && led < 42) maskLEDs[led-1] = false;
		if(led > 0 && led <= 36){
			maskLEDs[led] = true;
			if(led > 1) maskLEDs[led+1] = true;
			if(led > 2) maskLEDs[led+2] = true;
			if(led > 3) maskLEDs[led+3] = true;
			if(led > 4) maskLEDs[led+4] = true;
		}else if(led > 36 && led < 39){
			maskLEDs[led] = true;
			maskLEDs[led+1] = true;
			maskLEDs[led+2] = true;
			if(led == 37) maskLEDs[0] = true;
			if(led == 38) maskLEDs[1] = true;
		}else if(led == 42) maskLEDs[0] = false;
		else if(led == 43) maskLEDs[1] = false;

		//2st wave
		if(led == 17) maskLEDs[1] = false;
		if(led == 18) maskLEDs[2] = false;
		if(led == 19) maskLEDs[3] = false;
		if(led > 19 && led < 42+15) maskLEDs[led-16] = false;
		if(led > 15 && led <= 51){
			maskLEDs[led-15] = true;
			if(led > 16) maskLEDs[led-14] = true;
			if(led > 17) maskLEDs[led-13] = true;
			if(led > 18) maskLEDs[led-12] = true;
			if(led > 19) maskLEDs[led-11] = true;
		}else if(led > 51 && led < 54){
			maskLEDs[led-15] = true;
			maskLEDs[led-14] = true;
			maskLEDs[led-13] = true;
			if(led == 52) maskLEDs[0] = true;
			if(led == 53) maskLEDs[1] = true;
		}else if(led == 57) maskLEDs[0] = false;
		else if(led == 58) maskLEDs[1] = false;

		//3st wave
		if(led == 32) maskLEDs[1] = false;
		if(led == 33) maskLEDs[2] = false;
		if(led == 34) maskLEDs[3] = false;
		if(led > 34 && led < 42+30) maskLEDs[led-31] = false;
		if(led > 30 && led <= 66){
			maskLEDs[led-30] = true;
			if(led > 31) maskLEDs[led-29] = true;
			if(led > 32) maskLEDs[led-28] = true;
			if(led > 33) maskLEDs[led-27] = true;
			if(led > 34) maskLEDs[led-26] = true;
		}else if(led > 66 && led < 69){
			maskLEDs[led-30] = true;
			maskLEDs[led-29] = true;
			maskLEDs[led-28] = true;
			if(led == 67) maskLEDs[0] = true;
			if(led == 68) maskLEDs[1] = true;
		}else if(led == 72) maskLEDs[0] = false;
		else if(led == 73) maskLEDs[1] = false;

		//4st wave
		if(led == 47) maskLEDs[1] = false;
		if(led == 48) maskLEDs[2] = false;
		if(led == 49) maskLEDs[3] = false;
		if(led > 49 && led < 57+30) maskLEDs[led-46] = false;
		if(led > 45 && led <= 81){
			maskLEDs[led-45] = true;
			if(led > 46) maskLEDs[led-44] = true;
			if(led > 47) maskLEDs[led-43] = true;
			if(led > 48) maskLEDs[led-42] = true;
			if(led > 49) maskLEDs[led-41] = true;
		}else if(led > 81 && led < 84){
			maskLEDs[led-45] = true;
			maskLEDs[led-44] = true;
			maskLEDs[led-43] = true;
			if(led == 82) maskLEDs[0] = true;
			if(led == 83) maskLEDs[1] = true;
		}else if(led == 87) maskLEDs[0] = false;
		else if(led == 88) maskLEDs[1] = false;

		//5st wave
		if(led == 62) maskLEDs[1] = false;
		if(led == 63) maskLEDs[2] = false;
		if(led == 64) maskLEDs[3] = false;
		if(led > 64 && led < 72+30) maskLEDs[led-61] = false;
		if(led > 60 && led <= 96){
			maskLEDs[led-60] = true;
			if(led > 61) maskLEDs[led-59] = true;
			if(led > 62) maskLEDs[led-58] = true;
			if(led > 63) maskLEDs[led-57] = true;
			if(led > 64) maskLEDs[led-56] = true;
		}else if(led > 96 && led < 99){
			maskLEDs[led-60] = true;
			maskLEDs[led-59] = true;
			maskLEDs[led-58] = true;
			if(led == 97) maskLEDs[0] = true;
			if(led == 98) maskLEDs[1] = true;
		}else if(led == 102) maskLEDs[0] = false;
		else if(led == 103) maskLEDs[1] = false;

		checkAndBlinkLEDs(slowing, period_us);
	}

	//Shut ground
	for(uint8_t percent = maxBrightness; percent > minBrightness; percent--){
		for(uint8_t led = 41; led < LEDS; led++){
			dutyCycles[led] = percent-1;
		}checkAndBlinkLEDs(1, period_us);
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




















