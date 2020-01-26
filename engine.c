#include "engine.h"
#include "PWM.h"

char ENGINE0_STATUS = 0; // ENABLE/DISABLE ENGINE 
char ENGINE1_STATUS = 0;
//char ENGINEX_STATUS = 0;

void engineInitialize(){
	PWMInitialize();
}

char engineStatus(uint8_t id_engine){
	
			switch (id_engine){
				case ENGINE0:
					return ENGINE0_STATUS;
				break;
				
				case ENGINE1:
					/*
						PWMsetCh3.. channel 4. in the future...
					*/
				break;
				
				default:
					// zabezpieczenie, np wylacz wszystkie silniki?
				break;
			}
}

void engineLeft(uint8_t id_engine, uint16_t channel_source){
	
			switch (id_engine){
				case ENGINE0:
					if (ENGINE0_STATUS == 1){
						PWMsetCh1(channel_source);
						PWMsetCh2(0);
					}
				break;
				
				case ENGINE1:
					if (ENGINE1_STATUS == 1){
					/*
						PWMsetCh3.. channel 4. in the future...
					*/
					}
				break;
				
				default:
					// zabezpieczenie, np wylacz wszystkie silniki?
				break;
			}
}

void engineRight(uint8_t id_engine, uint16_t channel_source){

				switch (id_engine){
				case ENGINE0:
					if (ENGINE0_STATUS == 1){
						PWMsetCh2(channel_source);
						PWMsetCh1(0);
					}
				break;
				
				case ENGINE1:
					if (ENGINE0_STATUS == 1){
					/*
						PWMsetCh3.. channel 4. in the future...
					*/
					}
				break;
				
				default:
					// zabezpieczenie, np wylacz wszystkie silniki?
				break;
			}
}


void engineStart(uint8_t id_engine){

				switch (id_engine){
				case ENGINE0:
					ENGINE0_STATUS = 1;
				break;
				
				case ENGINE1:
					ENGINE1_STATUS = 1;
					/*
						PWMsetCh3.. channel 4. in the future...
					*/
				break;
				
				default:
					// zabezpieczenie, np wylacz wszystkie silniki?
				break;
			}
}

void engineStop(uint8_t id_engine){
	
			switch (id_engine){
				case ENGINE0:
					PWMsetCh2(0);
					PWMsetCh1(0);
					ENGINE0_STATUS = 0;
				break;
				
				case ENGINE1:
					ENGINE1_STATUS = 0;
					/*
						PWMsetCh3.. channel 4. in the future...
					*/
				break;
				
				default:
					// zabezpieczenie, np wylacz wszystkie silniki?
				break;
			}
}


void engine_IRQHandler(){
	
}

