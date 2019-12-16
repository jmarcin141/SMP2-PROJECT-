#include "MKL05Z4.h"

#include "leds.h"


int main(){
	ledsInit();
	
	while(1){
		
		for(int i=0;i<600000;i++){
			ledsOn(ledSky);
		}
		ledsOff(ledWhite);
		for(int i=0;i<600000;i++){
			ledsOn(ledPink);
		}
		ledsOff(ledWhite);
	}
	return 0;
}