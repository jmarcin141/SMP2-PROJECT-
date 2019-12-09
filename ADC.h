#ifndef adc_h
#define adc_h


#include "MKL46Z4.h"
#include "slcd.h"

/*
	Kanaly pomiarowe
	3 - light sensor
	26 - temp. sens
	29 - VREFH
	30 - VREFL
	*/
	
#define AD_LIGHT_SENS 3
#define AD_TEMP_SENS 26
#define AD_VREFH 29
#define AD_VREFL 30



void ADCInitialize();

void ADCCalibration();

uint16_t ADCRead(uint8_t channel);

void ADC0_IRQn_Handler(void);


#endif
