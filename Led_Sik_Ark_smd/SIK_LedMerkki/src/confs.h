#include <avr/io.h>

#ifndef CONFS
#define CONFS

//Define clock frequency for self functions (MHz)
#define CLOCK 8
//#define F_CPU 8000000UL


#define LEDS 49


//Versio 1.0 atmega328pb pin confs
#define Va1	1
#define Va2	6
#define Va3	2
#define Va4	3
#define Va5	4
#define Va6	5
#define Va7	0

#define Vk1	7
#define Vk2	0
#define Vk3	1
#define Vk4	2
#define Vk5	3
#define Vk6	4
#define Vk7	5


#define port_Va1	&PORTA
#define port_Va2	&PORTA
#define port_Va3	&PORTA
#define port_Va4	&PORTA
#define port_Va5 	&PORTA
#define port_Va6 	&PORTA
#define port_Va7	&PORTA

#define port_Vk1	&PORTA
#define port_Vk2	&PORTB
#define port_Vk3 	&PORTB
#define port_Vk4	&PORTB
#define port_Vk5	&PORTB
#define port_Vk6	&PORTB
#define port_Vk7 	&PORTB


//Pointers from prot adresses; PORTC, PORTB, PORTE etc
volatile uint8_t *ledPorts[2][10] = {
	{port_Va1, port_Va2, port_Va3, port_Va4, port_Va5, port_Va6, port_Va7},
	{port_Vk1, port_Vk2, port_Vk3, port_Vk4, port_Vk5, port_Vk6, port_Vk7}
}; 

//indexes for selecting correct port from PORT*
uint8_t pinIndexPointer[2][10] = {
	{Va1, Va2, Va3, Va4, Va5, Va6, Va7}, 
	{Vk1, Vk2, Vk3, Vk4, Vk5, Vk6, Vk7}
};



#endif //CONFS
