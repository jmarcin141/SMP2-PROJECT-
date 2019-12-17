#include "MKL05Z4.h"

/*
		MKL05Z32VFM4
*/


#include "PWM.h"

int main(){
	//ledsInit();
	PWMInitialize();
	
PWMset(4700,4400,4900);
//PWMsetCh1(4500);
//PWMsetCh2(4500);

	
	while(1){
	}
	return 0;
}