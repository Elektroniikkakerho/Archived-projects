#include <avr/io.h>

#ifndef CONFS
#define CONFS

//#define VER1_0 //Comment out for fixed version

//Define clock frequency for self functions (MHz)
#define CLOCK 8
//#define F_CPU 16000000UL


#define LEDS 90


//Versio 1.0 atmega328pb pin confs
#define Va1	1
#define Va2	0
#define Va3	7
#define Va4	6
#define Va5	5
#define Va6	3
#define Va7	2
#ifdef VER1_0  //for fault fersion
	#define Va8	4 
#endif
#ifndef VER1_0 //For fixed version
	#define Va8	0  
#endif
#define Va9	1
#define Va10	0

#define Vk1	2
#define Vk2	0
#define Vk3	2
#define Vk4	3
#define Vk5	1
#define Vk6	2
#define Vk7	3
#define Vk8	4
#define Vk9	5

#define port_Va1	&PORTB
#define port_Va2	&PORTB
#define port_Va3	&PORTD
#define port_Va4	&PORTD
#define port_Va5 	&PORTD
#define port_Va6 	&PORTD
#define port_Va7	&PORTD
#ifdef VER1_0 //For vault version
	#define port_Va8	&PORTD 
#endif

#ifndef VER1_0 //For fixed version
	#define port_Va8	&PORTE 
#endif
#define port_Va9	&PORTD
#define port_Va10	&PORTD

#define port_Vk1	&PORTB
#define port_Vk2	&PORTC
#define port_Vk3 	&PORTE
#define port_Vk4	&PORTE
#define port_Vk5	&PORTC
#define port_Vk6	&PORTC
#define port_Vk7 	&PORTC
#define port_Vk8 	&PORTC
#define port_Vk9 	&PORTC

//Pointers from prot adresses; PORTC, PORTB, PORTE etc
volatile uint8_t *ledPorts[2][10] = {
	{port_Va1, port_Va2, port_Va3, port_Va4, port_Va5, port_Va6, port_Va7, port_Va8, port_Va9, port_Va10},
	{port_Vk1, port_Vk2, port_Vk3, port_Vk4, port_Vk5, port_Vk6, port_Vk7, port_Vk8, port_Vk9}
}; 

//indexes for selecting correct port from PORT*
uint8_t pinIndexPointer[2][10] = {
	{Va1, Va2, Va3, Va4, Va5, Va6, Va7, Va8, Va9, Va10}, 
	{Vk1, Vk2, Vk3, Vk4, Vk5, Vk6, Vk7, Vk8, Vk9}
};




#endif //CONFS
