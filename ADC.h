#ifndef adc_h
#define adc_h


#include "MKL05Z4.h"
	
#define AD_PTA9_SENS 2


void ADCInitialize();

void ADCCalibration();

uint16_t ADCRead(uint8_t channel);

void ADC0_IRQn_Handler(void);


#endif
