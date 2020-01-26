#include "irreceiver.h"
#include "PIT.h"
#include "uart.h"
#include "engine.h"

#include <cmath>

#define RC5TimerValue ldv_val //55926 //ldv_val


#define RC5ToleranceTime  250  //150 - 350 [us]  
#define RC5HalfBitTime 889 // [us]
#define RC5BitTime 1778 // [us]


#define RC5HalfBitTimeMinLDV (16020) //(uint32_t)(round( (RC5BitTime - RC5ToleranceTime)*20.97/2UL - 1))
#define RC5HalfBitTimeMaxLDV (21263) //(uint32_t)(round( (RC5BitTime + RC5ToleranceTime)*20.97/2UL - 1))
#define RC5BitTimeMinLDV (32041) //(uint32_t)(round( (RC5BitTime - RC5ToleranceTime)*20.97 - 1))
#define RC5BitTimeMaxLDV (42526) //(uint32_t)(round( (RC5BitTime + RC5ToleranceTime)*20.97 - 1))




/* 	Start 11, toggle 0/1, Address MSB|LSB, Command MSB|LSB
		[S1|S2|Tgl|A4|A3|A2|A1|A0|C5|C4|C3|C2|C1|C0]
		
		Bit [0|1|2|3|4|5|6|7|8|9|10|11|12|13]    */








	static uint8_t bitNumber = 0;
	static uint16_t dataTemp = 0;
	static uint8_t halfBitCounter = 0;
	uint8_t dataReceived = 0;
	static uint8_t edge = 0; // 0 - fall, 1- rising
	static uint8_t status = 0; 
	static uint8_t deviceAddress = 0;


uint8_t speedPWM = 0;


uint8_t irGetCommand(){
	//if (status == 1){
	return dataReceived;
	//}
	//else return 0;
}

uint8_t irGetAddress(){
	//if (status == 1){
		return deviceAddress;
	//}
	//else return 0;
}

uint8_t irGetStatus(){
	return status;
}


void irReceiverInitialize(){
	// IR receiver to PTA11
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[11] |= PORT_PCR_MUX(1);
	PORTA->PCR[11] |= PORT_PCR_PE_MASK;
	PORTA->PCR[11] |= PORT_PCR_PS_MASK;
	PORTA->PCR[11] |= PORT_PCR_IRQC(0x0B); // falling 0x09 - rising edge
	//PORTA->PCR[11] |= PORT_PCR_IRQC(0x0B); // interrupt on falling and rising edge
	
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_SetPriority(PORTA_IRQn,2);
	
	
	PitInitialize();
	//PitStop();
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[8] |= PORT_PCR_MUX(1UL); 
	PTB->PDDR |= 1UL<<8;
	PTB->PSOR |= 1UL<<8;

	
}



void resetTimer()  
{  
	PITResetTimer();

}  

void ToggleInterruptEdge(){
	if(PORTA->PCR[11] & PORT_PCR_IRQC(0x09)){
	PORTA->PCR[11] |= PORT_PCR_IRQC(0x0A); // falling edge
	}
	else if( PORTA->PCR[11] & PORT_PCR_IRQC(0x0A)){
	PORTA->PCR[11] |= PORT_PCR_IRQC(0x09); // rising edge
	}
}



void PORTA_IRQHandler(){


		
	
	NVIC_ClearPendingIRQ(PORTA_IRQn);

	status = 0;

	//ToggleInterruptEdge();
	if (PORTA->PCR[11] & PORT_PCR_ISF_MASK){
		if (PTA->PDIR & 1UL<<11){
			edge = 1; // 1 is rising edge, 0 - falling
		}
		else { edge = 0;}
}
	

 PTB->PCOR |= 1<<8;

	if(edge == 0){

      } 
	

 
			if(edge == 1){

      } 
		
			
			

	  
  switch(bitNumber) {  
//  
//    //--- bit startowy nr 0 ------------------------------------  
//  
		case 0:  
  

				
				bitNumber=1;
				dataTemp = 0;
				halfBitCounter = 0;
			resetTimer();
 
    break;  
//  
//  
//    //--- bit startowy nr 1 ------------------------------------  
//  
  case 1:  
		 if( (PIT->CHANNEL[0].CVAL < (RC5TimerValue - RC5HalfBitTimeMinLDV) ) || (PIT->CHANNEL[0].CVAL > (RC5TimerValue - RC5HalfBitTimeMaxLDV) ) ) {
			 // wykryto polowe drugiego bitu startu po 889us
		 
		 if (edge == 1){
			resetTimer();
		 bitNumber=1;
	 } else if (edge == 0){
		 
		 resetTimer();
		 bitNumber=2;
	 }
 } 
		 
	 
    break;  






  default:  
  
    if(   (PIT->CHANNEL[0].CVAL > (RC5TimerValue - RC5HalfBitTimeMinLDV) ) || (PIT->CHANNEL[0].CVAL < (RC5TimerValue - RC5BitTimeMaxLDV)  ) ) {// || (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ){ //|| (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ){// (PIT->CHANNEL[0].CVAL > (18642 + 5241) ) ){ (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ||

				bitNumber = 0;
				break;

    } else if(  PIT->CHANNEL[0].CVAL < (RC5TimerValue - RC5HalfBitTimeMaxLDV) )  { //PIT->CHANNEL[0].CVAL < (18642 - 5241) 

					if(halfBitCounter == 1) {  

						dataTemp <<=1;
 
	        if( edge == 0){ // PORTA->PCR[11] & PORT_PCR_IRQC(0x0A)){ // bylo zbocze opadajace) {  

						dataTemp |=1;

	        }  
  
  
					halfBitCounter = 0;
 
					bitNumber++;
 
			} else {  

				halfBitCounter =1;

  
      }  

			resetTimer();
  
    } else {  

			dataTemp <<=1;

      if( edge == 0 ) { // PORTA->PCR[11] & PORT_PCR_IRQC(0x0A) ){ //bylo zbocze opadajace) {  

				dataTemp |=1;

      }  

			resetTimer();
			bitNumber++;
 
    }  
  
//    //--- czy to juz ostatni bit? --------------------------------  
  
    if(bitNumber > 13) {  

			dataReceived = (dataTemp);// & 0x3F); //0b00111111;
			  uartWriteData(0xCC);
				uartWriteData(dataReceived);


				uartWriteData(0xAA);
				deviceAddress = ( (dataTemp>>8));
				uartWriteData(deviceAddress);
			
			status = 1;



			bitNumber=0;
			
			PTB->PSOR |= 1<<8;


    }  
    break;  
  }


  if(bitNumber == 0) {  

resetTimer();

halfBitCounter=0;		

	
		
  }  
//

	
	if (irGetCommand() == 0xAA){
		engineStart(ENGINE0);
		if (speedPWM <255){
			engineLeft(ENGINE0,speedPWM++);
			}
		}
	else if (irGetCommand() == 0xAB){
		engineStart(ENGINE0);
		if (speedPWM >0){
		engineLeft(ENGINE0,speedPWM--);
	}
}
	

	
	PORTA->PCR[11] |= PORT_PCR_ISF_MASK;		
}



void TPMInitialize(){

	
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); 	// 01 MCGFLLCLK clock = 47972352u = 47,98MHz

	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; // clock enabled
	
 
	TPM1->SC &= ~TPM_SC_CPWMS_MASK;
	TPM1->SC &= ~TPM_SC_CMOD_MASK;
	TPM1->SC |= TPM_SC_PS(4); // 48MHz/16 = 3MHz 
	
//	period = MOD + 1
//	period of overflow = 889 us
	
	TPM1->MOD = TPM_MOD_MOD(2666);
		
	// input compare
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK;



	TPM1->SC |= TPM_SC_CMOD(1); // enable timer

	
	
	
}
	



void TPM1_IRQHandler(){
	//NVIC_ClearPendingIRQ(TPM1_IRQn);
	//TPM1->SC |= TPM_SC_TOF_MASK;

}


  
