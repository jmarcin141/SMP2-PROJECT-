#include "engine.h"
#include "PWM.h"



void engineInitialize(){
	PWMInitialize();
}

uint16_t engineStatus(uint8_t id_engine){
	
			switch (id_engine){
				case ENGINE0:
					
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
					PWMsetCh1(channel_source);
					PWMsetCh2(0);
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

void engineRight(uint8_t id_engine, uint16_t channel_source){

				switch (id_engine){
				case ENGINE0:
					PWMsetCh2(channel_source);
					PWMsetCh1(0);
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

void engineStop(uint8_t id_engine){
	
			switch (id_engine){
				case ENGINE0:
					PWMsetCh2(0);
					PWMsetCh1(0);
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


void engine_IRQHandler(){
	
}

