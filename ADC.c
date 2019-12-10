#include "adc.h"

// smp2 project
#define bit_length 15

void ADCInitialize(){
	// tranzystor
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[22] |= PORT_PCR_MUX(1UL);
	PORTE->PCR[22] |= PORT_PCR_PE_MASK;//pull enable
	PORTE->PCR[22] |= PORT_PCR_PS_MASK; // pull up
	
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	ADC0->CFG1 |= ADC_CFG1_ADICLK(1); //podlaczenie bus clock /2
	ADC0->CFG1 &= ~ADC_CFG1_ADIV_MASK; // 0 - divide ratio >> clkRate/1
	ADC0->CFG1 |= ADC_CFG1_ADLSMP(1); // dlugi czas prkbkowania
	/*
	Bus clock = 24MHz
	fADCK = (24MHz / 2)/1= 12 MHz
	*/
	ADC0->CFG1 |= ADC_CFG1_MODE(1); 
	/*
	01 When DIFF=0:It is single-ended 12-bit conversion ; when DIFF=1, it is differential 13-bit conversion with 2's complement output.
	*/
	ADC0->SC1[0] |= ADC_SC1_AIEN(1); // aktywne przerwanie
	ADC0->SC1[0] &= ~ADC_SC1_DIFF_MASK; //- signle ended
	//ADC0->SC1[0] |= ADC_SC1_ADCH(28); //3 //pte 22? 3 czy 22
	
	ADC0->CFG2 |= ADC_CFG2_ADHSC(1); //
	ADC0->CFG2 |= ADC_CFG2_ADLSTS(3); // 11 - 2 extra ADCK cycles, 6 ADCK cycles total sample time
	
	ADC0->SC2 &= ~ADC_SC2_REFSEL_MASK; //00- ext.  01 - external VALTH VALTL
	ADC0->SC3 |= ADC_SC3_AVGE(1); // enable
	ADC0->SC3 |= ADC_SC3_AVGS(3); // 11 - 32 samples max, averaged
	
	ADC0->SC3 |= ADC_SC3_CAL(1);
	
	
	
	
	
		//TPM0->SC |= TPM_SC_CMOD(1);
		//TPM0->SC |= TPM_SC_TOIE_MASK;
//		NVIC_SetPriority(ADC0_IRQn,3);
//		NVIC_ClearPendingIRQ(ADC0_IRQn);
//		NVIC_EnableIRQ(ADC0_IRQn);
	
	
	//SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(4); // pit trigger 0/1 (4/5)
}
	// kalibracja
void ADCCalibration(){

	slcdDisplay(0xCA10,HEX);

	uint16_t cnt1=0;
 while (ADC0->SC3 & (1UL << ADC_SC3_CAL_SHIFT )){ //ADC_SC3_CAL_SHIFT
	 cnt1++;
		if (ADC0->SC3 & (1UL << ADC_SC3_CALF_SHIFT )){ //ADC_SC3_CALF_SHIFT
			ADCCalibration(); // powtorzenie procesu kalibracji, gdy while(0) -> OK
			slcdErr(5);
		}
		else if (! (ADC0->SC3 & (1UL << ADC_SC3_CALF_SHIFT))){  //ADC_SC3_CALF_SHIFT  else -> powiodlo sie, zapisanie wartosci do rejestrow
				uint32_t sumCLPx = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPD + ADC0->CLPS;
				sumCLPx=sumCLPx/2;
				sumCLPx|=(1UL<<bit_length);
				ADC0->PG = ADC_MG_MG(sumCLPx);
				
				uint32_t sumCLMx= ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 +ADC0->CLM4 + ADC0->CLMD + ADC0->CLMS; 
				sumCLMx=sumCLMx/2;
				sumCLMx|=(1UL<<bit_length);
				ADC0->MG = ADC_MG_MG(sumCLMx);
			
				//konfiguracja po kalibracji
//					ADC0->CFG1 |= ADC_CFG1_ADICLK(1); //podlaczenie bus clock /2
//					ADC0->CFG1 |= ADC_CFG1_ADIV(0); // 0 - divide ratio >> clkRate/1
//					ADC0->CFG1 |= ADC_CFG1_ADLSMP(1); // dlugi czas prkbkowania
//					
//				ADC0->SC1[0] |= ADC_SC1_AIEN(1);
			slcdDisplay(0xCA11,HEX);
		}
		//slcdDisplay(cnt1,DEC); // ile razy wykonala sie kalibracja
	}
	slcdDisplay(0xCA1C,HEX); // prawidlowo skonfigurowano!
}

uint16_t ADCRead(uint8_t channel){
/*
	Kanaly pomiarowe
	3 - light sensor
	26 - temp. sens
	29 - VREFH
	30 - VREFL
	*/
	uint16_t nCode = 0;
	float V_sensor = 0;
	
		ADC0->SC1[0] = ADC_SC1_ADCH(channel); //wybor kanalu
		ADC0->SC1[0] |= ADC_SC1_AIEN(1);
	
		while (ADC0->SC2 & ADC_SC2_ADACT_MASK){}; // conversion in progress 
	
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)) {}; //czekaj poki trwa konwersja
		
	//ADC0->SC2 |= ADC_SC2_ADTRG(1); // wyzwalanie pomiaru
	//slcdDisplay(ADC0->R[0], DEC); // dane od D0-D11
	
		nCode = ADC0->R[0];
		V_sensor = (float)(nCode*3000)/4095;
		switch (channel)
		{
			case AD_LIGHT_SENS:
				slcdDisplay(V_sensor,DEC);
			return V_sensor; //[mV]
			break;
			
			case AD_TEMP_SENS:
				/* datasheet: http://cache.freescale.com/files/microcontrollers/doc/app_note/AN3031.pdf */
			//1.715f
			slcdDisplay((uint16_t)(25-((V_sensor/1000 - 0.719)/0.001749)),DEC);
			return (25-((V_sensor/1000 - 0.719)/0.001749)); // [degreesC]
			break;
						
			case AD_VREFH:
				slcdDisplay(V_sensor,DEC);
			return V_sensor; // [mV]
			break;
			
			default:
				slcdDisplay(V_sensor,DEC);
				return ADC0->R[0];
			break;
		}
}


//void ADC0_IRQn_Handler(void){
//	NVIC_ClearPendingIRQ(ADC0_IRQn);
//	
//	//TPM0->SC |= TPM_SC_TOF_MASK; 
//	
//	PTE->PTOR = (1UL << 29);
//	
//	ADCRead(AD_LIGHT_SENS);
//}

