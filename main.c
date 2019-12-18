#include "MKL05Z4.h"

/*
		MKL05Z32VFM4
*/


#include "PWM.h"
#include "ADC.h"

int main(){
	//ledsInit();
	PWMInitialize();
	ADCInitialize();
	
PWMset(4905,4905,4905);

	

	
	//PWMsetCh1(ADCRead(AD_PTB11_SENS));

	while(1){
		PWMsetCh1((ADCRead(AD_PTA9_SENS)*1.2));

			PWMsetCh2( (ADCRead(AD_TEMP_SENS) << 7 ) - 123000 );
	}
	return 0;
}