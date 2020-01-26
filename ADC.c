#include "adc.h"

#include "PWM.h"


#define bit_length 15

void ADCInitialize(){
	// joystick ADC0_SE2 - AD2 -- PTA9
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[9] |= PORT_PCR_MUX(0);
	PTA->PDDR &= ~(0UL<<9); 
	PORTA->PCR[9] |= PORT_PCR_PE_MASK;//pull enable
	PORTA->PCR[9] |= PORT_PCR_PS_MASK; // pull up
	
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	ADC0->CFG1 |= ADC_CFG1_ADICLK(1); //podlaczenie bus clock /2
	ADC0->CFG1 &= ~ADC_CFG1_ADIV_MASK; // 0 - divide ratio >> clkRate/1
	ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK; // dlugi czas prkbkowania
	/*
	Bus clock = 24MHz
	fADCK = (24MHz / 2)/1= 12 MHz
	*/
	ADC0->CFG1 |= ADC_CFG1_MODE(1); 
	// 00 It is single-ended 8-bit conversion.
	// 01 It is single-ended 12-bit conversion .
	// 01 When DIFF=0:It is single-ended 12-bit conversion ; when DIFF=1, it is differential 13-bit conversion with 2's complement output.
	

	
	ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK; // // high speed
	ADC0->CFG2 |= ADC_CFG2_ADLSTS(3); // 11 - 2 extra ADCK cycles, 6 ADCK cycles total sample time
	
	ADC0->SC2 |= ADC_SC2_REFSEL(0); //00- ext.  01 - external VALTH VALTL
	ADC0->SC3 |= ADC_SC3_AVGE_MASK; // enable - usrednianie
	ADC0->SC3 |= ADC_SC3_AVGS(3); // 11 - 32 samples max, averaged
	
	ADC0->SC3 |= ADC_SC3_CAL_MASK; // begin calibration
	
	
	ADCCalibration();
	
}
	// kalibracja
void ADCCalibration(){

	uint16_t cnt1=0;
 while (ADC0->SC3 & (1UL << ADC_SC3_CAL_SHIFT )){ //ADC_SC3_CAL_SHIFT
	 cnt1++;
		if (ADC0->SC3 & (1UL << ADC_SC3_CALF_SHIFT )){ //ADC_SC3_CALF_SHIFT
			ADCCalibration(); // powtorzenie procesu kalibracji, gdy while(0) -> OK
			
		}
		else if (! (ADC0->SC3 & (1UL << ADC_SC3_CALF_SHIFT))){  //ADC_SC3_CALF_SHIFT  else -> powiodlo sie, zapisanie wartosci do rejestrow
				uint32_t sumCLPx = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPD + ADC0->CLPS;
				sumCLPx=sumCLPx/2;
				sumCLPx|=(1UL<<bit_length);
				ADC0->PG = ADC_PG_PG(sumCLPx);
			
			//adc minus-side gain register, nie wystepuje w kl05	
		}
	}
	 // prawidlowo skonfigurowano!
	//PTB->PCOR |= 1UL<<8; //enable PTB 8...10 as output
}

uint16_t ADCRead(uint8_t channel){

	uint16_t nCode = 0;
	float V_sensor = 0;
	
		ADC0->SC1[0] = ADC_SC1_ADCH(channel); //wybor kanalu
	//	ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
	
		while (ADC0->SC2 & ADC_SC2_ADACT_MASK){ }; // conversion in progress 
	
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)) {}; //czekaj poki trwa konwersja
	
	//ADC0->SC2 |= ADC_SC2_ADTRG(1); // wyzwalanie pomiaru
	
		nCode = ADC0->R[0] ;
		V_sensor = (float)(nCode*3000)/4095;
		switch (channel)
		{
			case AD_PTA9_SENS:
			return nCode;
			break;
			
			
			default:
				return ADC0->R[0];
			break;
		}
}


void ADC0_IRQn_Handler(void){
	NVIC_ClearPendingIRQ(ADC0_IRQn);

}

