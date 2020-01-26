#include "PIT.h"

//#define ldv_val 42526

#define ldv_value_silent 1048499


void PitInitialize(void)
{
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //clock to PIT
  PIT->MCR = 0x00;   //enable PIT module
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR &= ~PIT_MCR_FRZ_MASK;	
	
	//BusClock = 20.97MHz
	// T = 1778us
	//LDV=round(T*Busclock-1)=37284
	
	
	//PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(62910000); // 3 sekundY
	//PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(0x13FFA0F); // sekunda
	PIT->CHANNEL[0].LDVAL = ldv_val; //PIT_LDVAL_TSV(0xDA76); //0x91A4 //PIT_LDVAL_TSV(0xDA76); //0x91A4 
	PIT->CHANNEL[1].LDVAL = ldv_value_silent;
	

	//PIT->CHANNEL[0].TCTRL |=PIT_TCTRL_TIE_MASK;	
	//PIT->CHANNEL[1].TCTRL |=PIT_TCTRL_TIE_MASK;	
	
		
	//NVIC_SetPriority(PIT_IRQn,3);
	//NVIC_ClearPendingIRQ(PIT_IRQn);
	//NVIC_EnableIRQ(PIT_IRQn);
	

	
		PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
		PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
}


void PITResetTimer(){  

	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	
	PIT->CHANNEL[0].LDVAL = ldv_val; //0xDA76; //PIT_LDVAL_TSV(0xDA76);
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}  


void PITSilentResetTimer(){  

	PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	
	PIT->CHANNEL[1].LDVAL = ldv_value_silent;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
}  



void PIT_IRQHandler(){
	NVIC_ClearPendingIRQ(PIT_IRQn);
	
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
	
	
	if( (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK ) ) {

	}
	
}


void PitStop(){
	//PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TIE_MASK;	
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
}

void PitStart(){
	//PIT->CHANNEL[0].TCTRL |=PIT_TCTRL_TIE_MASK;	
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
}

