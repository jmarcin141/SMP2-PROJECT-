#ifndef adc_h
#define adc_h


#include "MKL05Z4.h"


///*
//	Kanaly pomiarowe
//	3 - 
//	26 - temp. sens
//	29 - VREFH
//	30 - VREFL
//	*/
//	
#define AD_PTA9_SENS 2
#define AD_TEMP_SENS 26
#define AD_VREFH 29
#define AD_VREFL 30



void ADCInitialize();

void ADCCalibration();

uint16_t ADCRead(uint8_t channel);

void ADC0_IRQn_Handler(void);


#endif
