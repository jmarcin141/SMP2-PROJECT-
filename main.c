#include "MKL05Z4.h"
/*
		MKL05Z32VFM4
*/

//#include "ADC.h"
#include "engine.h"
#include "uart.h"
#include "lvDecoder.h"
#include "PIT.h"
#include "irReceiver.h"


uint8_t command = 0;


int main(){
//	ADCInitialize();
	
	engineInitialize();
	uartInitialize();
	uartDemoHello();
	irReceiverInitialize();
	
	engineStart(ENGINE0);
	while(1){
////		1 - stop
////		2 - start
////		100 - obroty lewo predkosc 0
////		111 - obroty lewo predkosc 1
////		112...149 - obroty lewo predkosc 2..49
////		200...249 - obroty prawo predkosc 2..49
		
	command = uartReadData();
		
	lvDecoder(command);

		
	}
	return 0;
}