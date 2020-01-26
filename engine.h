#ifndef engine_h
#define engine_h

#include "MKL05Z4.h"

//engine Identify number
#define ENGINE0 ((uint8_t)(0))
#define ENGINE1 ((uint8_t)(1))
//#define ENGINEn ((uint8_t)(n))

void engineInitialize();

char engineStatus(uint8_t id_engine);

void engineLeft(uint8_t id_engine, uint16_t channel_source);

void engineRight(uint8_t id_engine, uint16_t channel_source);

void engineStop(uint8_t id_engine);

void engineStart(uint8_t id_engine);


void engine_IRQHandler();

#endif
