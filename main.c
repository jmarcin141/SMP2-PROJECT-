#include "MKL05Z4.h"

/*
		MKL05Z32VFM4
*/


#include "ADC.h"
#include "engine.h"
#include "irReceiver.h"

#include "uart.h"

int main(){

	
	ADCInitialize();
	
	//irReceiverInitialize();
	TPMInitialize();
	
	engineInitialize();
	
	uartInitialize();
	
	
unsigned char direction=0x00;
	
	uint16_t speed = 255;

	//uartDemoHello();
	
	uartWriteData(0x01);
	

	
	
	while(1){
		//	if (uartReadData() == 0x01){
		//PTB->PTOR |= 1UL<<8;
	//}
			
	uartWriteData(0x01);
		

		if (!(PTB->PDIR & 1UL<<1)){
			//PTB->PTOR |= 1UL<<8;
			direction ^= 1UL<<0; 
		//speed=speed-5;
			for (int i=0;i<1000000;i++){ } // opoznienie po odebraniu sygnalu IR
		}
	
		if (direction == 0x00){
			engineLeft(ENGINE0,speed);
		}
		else {
			engineRight(ENGINE0,speed);
		}
		
	}
	return 0;
}