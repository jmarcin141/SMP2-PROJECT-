#ifndef leds_h
#define leds_h


#include "MKL05Z4.h"


#define red_mask  1UL<<8
#define green_mask  1UL<<9
#define blue_mask  1UL<<10

//colors of led:
#define ledRed 1,0,0
#define ledGreen 0,1,0
#define ledBlue 0,0,1
#define ledOrange 1,1,0
#define ledPink 1,0,1
#define ledSky 0,1,1
#define ledWhite 1,1,1


void ledsInit(void);

void ledsOn(unsigned char red,unsigned char green,unsigned char blue);

void ledsOff(unsigned char red,unsigned char green,unsigned char blue);

void ledsToggle(unsigned char red,unsigned char green,unsigned char blue);

#endif