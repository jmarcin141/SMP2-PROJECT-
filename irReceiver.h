#ifndef irreceiver_h
#define irreceiver_h

#include "MKL05Z4.h"



void irReceiverInitialize();

void resetTimer();

void TPMInitialize();

void ToggleInterruptEdge();

uint8_t irGetCommand();
uint8_t irGetAddress();
uint8_t irGetStatus();

void PORTA_IRQHandler();

//void PitInitialize(void);
//void PIT_IRQHandler();



#endif
