#ifndef pit_h
#define pit_h

#include "MKL05Z4.h"

#define ldv_val 42527 //48750//42527 //45580 //42950 // 55926

void PitInitialize(void);
void PIT_IRQHandler();

void PITResetTimer();
void PITSilentResetTimer();  

void PitStop();
void PitStart();

#endif
