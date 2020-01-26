#include "lvDecoder.h"
#include "engine.h"


void lvDecoder(uint8_t command){

		if (command == 0x01){
			engineStop(ENGINE0);
			PTB->PCOR |= 1UL<<8;
		}
		else if (command == 0x02){
			engineStart(ENGINE0);
			PTB->PSOR |= 1UL<<8;
		}
		else if (command == 0x03){
			// engineLeft
		}
		else if (command == 0x04){
			// engineRight
		}
		
		else if ( ( command >= 0x64 ) && ( command <= 0x96 ) ){ // >=100 && < 150
			 engineLeft(ENGINE0,( (command - 100)*5.12 ) );

		}
		else if ( ( command >= 0xC8 ) && ( command <= 0xFA ) ){ // >= 200 && <250
			engineRight(ENGINE0,( (command - 200)*5.12 ) );
		}
		
		
	}
		
		