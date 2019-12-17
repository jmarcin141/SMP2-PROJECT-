#ifndef pwm_h
#define pwm_h

#include "MKL05Z4.h"


void PWMInitialize();

void PWMsetCh1(uint16_t channel);
void PWMsetCh2(uint16_t channel);
void PWMset(uint16_t channel1, uint16_t channel2, uint16_t channel3);


void TPM0_IRQHandler();

#endif
