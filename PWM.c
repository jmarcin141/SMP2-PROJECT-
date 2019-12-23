#include "PWM.h"

void PWMInitialize(){

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	//PORTB->PCR[8] |= PORT_PCR_MUX(1UL); // TPM0_CH3
	PORTB->PCR[9] |= PORT_PCR_MUX(2UL); // TPM0_CH2
	PORTB->PCR[10] |= PORT_PCR_MUX(2UL); //TPM0_CH1
	
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); 	// 01 MCGFLLCLK clock = 47972352u = 47,98MHz
	// 11 MCGIRCLK clock	>> MCG->C1 |= MCG_C1_IRCLKEN_MASK; MCG->C1 |= MCG_C1_IREFS_MASK;


	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; // clock enabled
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;
	TPM0->SC &= ~TPM_SC_CMOD_MASK;
	TPM0->SC |= TPM_SC_PS(7); // 48MHz/128 = 37.5kHz 
	
//	period = MOD + 1
//	pulse width = CnV (duty Cycles)
	
	TPM0->MOD = TPM_MOD_MOD(4095);
	
	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM0->CONTROLS[2].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM0->CONTROLS[3].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;

	TPM0->CONTROLS[1].CnV = TPM_CnV_VAL(0); // wspolczynnik wypelnienia
	TPM0->CONTROLS[2].CnV = TPM_CnV_VAL(0);
	TPM0->CONTROLS[3].CnV = TPM_CnV_VAL(0);	
	
	// interrupts
//	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_CHIE_MASK;
//	TPM0->CONTROLS[2].CnSC |= TPM_CnSC_CHIE_MASK;
//	TPM0->CONTROLS[3].CnSC |= TPM_CnSC_CHIE_MASK;
	
	TPM0->SC |= TPM_SC_CMOD(1);
//	NVIC_ClearPendingIRQ(TPM0_IRQn);
//	NVIC_EnableIRQ (TPM0_IRQn);
//	NVIC_SetPriority(TPM0_IRQn,3);
}

void PWMsetCh1(uint16_t channel){

	while( !(TPM0->STATUS & TPM_STATUS_TOF_MASK )) { }
	
	TPM0->SC |= TPM_SC_CMOD(0); // wylaczenie , zaladowanie wypelnienia, wlaczenie PWM
	TPM0->CONTROLS[1].CnV = TPM_CnV_VAL(channel);
	TPM0->SC |= TPM_SC_CMOD(1);
}

void PWMsetCh2(uint16_t channel){

	while( !(TPM0->STATUS & TPM_STATUS_TOF_MASK )) { }
	
	TPM0->SC |= TPM_SC_CMOD(0);
	TPM0->CONTROLS[2].CnV = TPM_CnV_VAL(channel);
	TPM0->SC |= TPM_SC_CMOD(1);
}

void PWMset(uint16_t channel1, uint16_t channel2, uint16_t channel3){

	while( !(TPM0->STATUS & TPM_STATUS_TOF_MASK )) { }
	
	TPM0->SC |= TPM_SC_CMOD(0);
	TPM0->CONTROLS[1].CnV = TPM_CnV_VAL(channel1);
	TPM0->CONTROLS[2].CnV = TPM_CnV_VAL(channel2);
	TPM0->CONTROLS[3].CnV = TPM_CnV_VAL(channel3);
	TPM0->SC |= TPM_SC_CMOD(1);
}

void TPM0_IRQHandler(){
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	TPM0->SC |= TPM_SC_TOF_MASK;
	
	//TPM0->CONTROLS[1].CnV+=TPM_CnV_VAL(4);
	//TPM0->CONTROLS[2].CnV+=TPM_CnV_VAL(3);
	//TPM0->CONTROLS[3].CnV+=TPM_CnV_VAL(6);
	
	//if (TPM0->CONTROLS[2].CnV > 4900){
	//TPM0->CONTROLS[1].CnV = TPM_CnV_VAL(0);
	//TPM0->CONTROLS[2].CnV = TPM_CnV_VAL(0);
	//TPM0->CONTROLS[3].CnV = TPM_CnV_VAL(0);
	//}
}

