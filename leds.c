#include "leds.h"

void ledsInit(void){
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable clock to port B
	PORTB->PCR[8] = PORT_PCR_MUX(1UL); // PIN PTB 8-10 is GPIO 
	PORTB->PCR[9] = PORT_PCR_MUX(1UL); 
	PORTB->PCR[10] = PORT_PCR_MUX(1UL); 
	
	PTB->PDDR |= red_mask; //enable PTB 8...10 as output
	PTB->PDDR |= green_mask;
	PTB->PDDR |= blue_mask;
	
	PTB->PSOR = red_mask; // switch off leds
	PTB->PSOR = blue_mask;
	PTB->PSOR = green_mask;
}


void ledsOn(unsigned char red,unsigned char green,unsigned char blue){
	
	if( red==1)		PTB->PCOR |= red_mask; 
	if( green==1)	PTB->PCOR |= green_mask;
	if( blue==1)	PTB->PCOR |= blue_mask;
}

void ledsOff(unsigned char red,unsigned char green,unsigned char blue){
	
	if( red==1)		PTB->PSOR |= red_mask;
	if( green==1)	PTB->PSOR |= green_mask;
	if( blue==1)	PTB->PSOR |= blue_mask;
}

void ledsToggle(unsigned char red,unsigned char green,unsigned char blue){
	
	if( red==1)		PTB->PTOR |= red_mask;
	if( green==1)	PTB->PTOR |= green_mask;
	if( blue==1)	PTB->PTOR |= blue_mask;
}