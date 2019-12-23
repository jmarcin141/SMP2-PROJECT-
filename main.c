#include "MKL05Z4.h"

/*
		MKL05Z32VFM4
*/


#include "ADC.h"
#include "engine.h"
#include "irReceiver.h"

int main(){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[8] |= PORT_PCR_MUX(1UL); 
	PTB->PDDR |= 1UL<<8;
	PTB->PSOR |= 1UL<<8;
	
	
	ADCInitialize();
	irReceiverInitialize();
	engineInitialize();
	
unsigned char direction=0x00;

	while(1){

		if (!(PTB->PDIR & 1UL<<1)){
			//PTB->PTOR |= 1UL<<8;
			direction ^= 1UL<<0; 
			for (int i=0;i<1000000;i++){ } // opoznienie po odebraniu sygnalu IR
		}
		
		if (direction == 0x00){
			engineLeft(ENGINE0,2047);
		}
		else {
			engineRight(ENGINE0,3500);
		}
		
	}
	return 0;
}