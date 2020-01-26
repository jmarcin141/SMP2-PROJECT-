#include "MKL05Z4.h"

/*
		MKL05Z32VFM4
*/



#define RC5ToleranceTime  250  //150 - 350 [us]  
#define RC5HalfBitTime 889 // [us]
#define RC5BitTime 1778 // [us]


//#define RC5ToleranceTimeLDV (RC5ToleranceTime*20)
//#define RC5HalfBitTimeLDV (RC5HalfBitTime*20) //(18641)
//#define RC5BitTimeLDV (RC5BitTime*20) //(37284)

#define RC5HalfBitTimeMinLDV (float)(( (RC5BitTime - RC5ToleranceTime)*20.97/2UL - 1))





//#include "ADC.h"
#include "engine.h"
#include "irReceiver.h"

#include "pit.h"

#include "uart.h"

#include "lvDecoder.h"



unsigned char direction=0x00;
uint8_t speed = 255;

	float i;


	uint8_t command = 0;

int main(){
 	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable clock to port B
	PORTB->PCR[8] = PORT_PCR_MUX(1UL); // PIN PTB 8-10 is GPIO 

	PTB->PDDR |= 1 <<8; //enable PTB 8...10 as output
	PTB->PSOR |= 1<<8;
	
//	ADCInitialize();
	
	//irReceiverInitialize();
	
	//TPMInitialize();
	
	//PitInitialize();
	engineInitialize();
	
	uartInitialize();

	uartDemoHello();

	
	while(1){
		
////		1 - stop
////		2 - start

////		100 - obroty lewo predkosc 0
////		111 - obroty lewo predkosc 1
////		112 - obroty lewo predkosc 2
////		...
////		149 - obroty lewo predkosc 49

////		200 - obroty prawo predkosc 0
////		...
////		249 - obroty prawo predkosc 49
		
	command = uartReadData();
		
	lvDecoder(command);
		

		

//i=(float)((RC5BitTime - RC5ToleranceTime)*20.97/2);
		
		//	if (i>16020 && i<16022){
		//		PTB->PCOR |= 1UL<<9;
		//	}
	
		
		//if (irGetStatus()){
	//	PTB->PTOR |= 1UL<<8;
		
	//	uartWriteData(0xAA);
	//		uartWriteData(irGetAddress() );
	//	uartWriteData('A');
	//	uartWriteData(irGetCommand());
	//}
		
		
	
	
		
		
//			if (uartReadData() == 0x01){
//		PTB->PTOR |= 1UL<<8;
//		engineLeft(ENGINE0,speed);
//	}
//			
//	if (uartReadData() == 'a'){
//		PTB->PTOR |= 1UL<<8;
//		engineRight(ENGINE0,speed);
//	}
			
	//uartWriteData(0x01);
		

//////	if (!(PTA->PDIR & 1UL<<11)){
//////			//PTB->PTOR |= 1UL<<8;
//////			direction ^= 1UL<<0; 
//////		//speed=speed-5;
//////		for ( i=0;i<10000000;i++){ } // opoznienie po odebraniu sygnalu IR
//////		//uartWriteData(0x48+8);
//////		}
//////	
//////		if (direction == 0x00){
//////		engineLeft(ENGINE0,speed);
//////			//uartWriteData('L');

//////		}
//////		else {
//////			engineRight(ENGINE0,speed);
//////			//uartWriteData('P');

//////		}
		
	}
	return 0;
}