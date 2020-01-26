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


//funkcja zerujaca timer oraz flage jego przepelnienia  
void resetTimer()  
{  
	PITResetTimer();
	//PitStop();
	
//	PIT->CHANNEL[0].LDVAL = 0; //PIT_LDVAL_TSV(0x0000);
	
	//PitStart();
//	PIT->CHANNEL[0].LDVAL = 55926; //PIT_LDVAL_TSV(0xDA76);
	
	
	//while( !(TPM1->STATUS & TPM_STATUS_TOF_MASK )) { }
	
//	TPM1->SC |= TPM_SC_CMOD(0); // wylaczenie , zaladowanie wypelnienia, wlaczenie TPM
//	TPM1->CNT = TPM_CNT_COUNT(0);
//	TPM1->SC |= TPM_SC_CMOD(1);
//	
//	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK;
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

	//PitStart();
		
	
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	//if (PORTA->PCR[11] & PORT_PCR_ISF_MASK){


//	resetTimer();
	status = 0;

	//ToggleInterruptEdge();
	if (PORTA->PCR[11] & PORT_PCR_ISF_MASK){
		if (PTA->PDIR & 1UL<<11){
			edge = 1; // 1 is rising edge, 0 - falling
		}
		else { edge = 0;}
}
	
	//PORTA->PCR[11] |= PORT_PCR_IRQC(0x0A);
 PTB->PCOR |= 1<<8;
	//uartWriteData(0xF0);	
	//if(!(PORTA->PCR[11] & PORT_PCR_ISF_MASK)){

	//if((PORTA->PCR[11] & PORT_PCR_IRQC_MASK) == (PORT_PCR_IRQC(0x09)) ){ //bylo zbocze opadajace) {  
	if(edge == 0){
				//PTB->PCOR |= 1<<8;
				////PTB->PCOR |= 1<<9;
      } 
	

		//if((PORTA->PCR[11] & PORT_PCR_IRQC_MASK) == (PORT_PCR_IRQC(0x0A))){ //bylo zbocze narastajace) {  
			if(edge == 1){
				////PTB->PSOR |= 1<<8;
				//PTB->PCOR |= 1<<9;
      } 
		//}
			
			

			
			
//  //W zaleznosci, który bit jest aktualnie dekodowany  
  switch(bitNumber) {  
//  
//    //--- bit startowy nr 0 ------------------------------------  
//  
		case 0:  
  
//    //Wykryto pierwsze zbocze opadajace.  
//    //Sprawdzamy, czy przerwa w sygnale byla wystarczajaco dluga  
  //  if( (PIT->CHANNEL[0].CVAL < (RC5TimerValue - RC5BitTimeMinLDV) ) ){ //(RC5TimerValue - RC5BitTimeMaxLDV) ) ) { // || (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ){//PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK 35400
   //PTB->PCOR |= 1<<8;
//      //Przerwa miedzy kolejnymi ramkami byla wystarczajaca,  
//      //by miec pewnosc, ze rozpoczynamy odbiór nowej ramki.  
				
				bitNumber=1;
				dataTemp = 0;
				halfBitCounter = 0;
			resetTimer();
			//PTB->PCOR |= 1<<8;
			//uartWriteData(0x1F);
//      //konczymy czekajac na zbocze narastajace drugiego bitu startu  
//  
   // } else {PTB->PCOR |= 1<<10; } //uartWriteData(0x1E);}
//      //Zbyt krótka przerwa w sygnale pomiedzy ramkami  
//      //dlatego te ramke danych musimy przeczekac.  
//  
//      //Tutaj nie trzeba nic wykonywac, poniewaz aktualnie numer_bitu  
//      //jest równy 1 co oznacza, ze warunek na konczu funkcji  
//      //przerwania ustawi stan poczatkowy  
//    }  
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
 } // else blad, co robic?...
		 
	 
    break;  






  default:  
   //uartWriteData(0xD0);
//    //Tutaj odbieramy pozostale bity ramki  
//  
//    //W zaleznosci jaki czas uplynal od ostatniego zbocza (przerwania)  
//    //Czy czas wykracza poza brzegowe parametry (min i max)  
    if(   (PIT->CHANNEL[0].CVAL > (RC5TimerValue - RC5HalfBitTimeMinLDV) ) || (PIT->CHANNEL[0].CVAL < (RC5TimerValue - RC5BitTimeMaxLDV)  ) ) {// || (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ){ //|| (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ){// (PIT->CHANNEL[0].CVAL > (18642 + 5241) ) ){ (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) ||
//      wystapilo przepelnienie timera  
//      lub czas krótszy niz minimalny dla pólbitu  
//      lub czas dluzszy niz maksymalny dla calego bitu  
//      )   
//      //Wykryto blad w sygnale.  
//      //Przerywamy dekodowanie tej ramki rozpoczynamy od nowa  
//      numer_bitu = 1;  
//      break;  //break, by nie sprawdzal koncowego if(numer_bitu > 14)  
			//uartWriteData(0xDE);
				bitNumber = 0;
				break;
//  
//      //Czy minal czas pólbitu?  
    } else if(  PIT->CHANNEL[0].CVAL < (RC5TimerValue - RC5HalfBitTimeMaxLDV) )  { //PIT->CHANNEL[0].CVAL < (18642 - 5241) 
//             czas równy lub krótszy niz maksymalny dla pólbitu  
//             )  
//    {  
//      //Uplynal czas równy polowie bitu  
//  
	//      //sprawdzamy, czy to druga polowa bitu  
					if(halfBitCounter == 1) {  
	//  
	//        //Tak to druga polówka aktualnie dekodowanego bitu  
	//        //i jestesmy aktualnie w polowie czasu odbieranego  
	//        //bitu. Jest to moment, w którym ustalamy wartosc  
	//        //odebranego bitu na podstawie kierunku zbocza.  
	//  
	//        //Przesun dane o jeden bit w lewo, by zrobic miejsce  
	//        //na odebrany bit  
	//        dane_temp << 1 
						dataTemp <<=1;
	  			//uartWriteData(0xDF);
	//        //Jezeli przerwanie wywolalo zbocze opadajace  
	//        //oznacza to, ze odebralismy jedynke wiec ja dodajemy  
	        if( edge == 0){ // PORTA->PCR[11] & PORT_PCR_IRQC(0x0A)){ // bylo zbocze opadajace) {  
	          //dane_temp |= 1;  
						dataTemp |=1;
						//uartWriteData(0xDD);
	        }  
//  
//        //zeruj licznik pólbitów  
//        polbit_licznik = 0;  
					halfBitCounter = 0;
					//uartWriteData(0xDC);
//        //zwieksz licznik odebranych bitów  
//        numer_bitu++;  
					bitNumber++;
//  
			} else {  
//  
//        //To pierwsza polowa dekodowanego bitu, czyli jestesmy  
//        //aktualnie na poczatku czasu przesylanego bitu.  
//  
//        //Ustawiamy licznik pólbitów  
//        polbit_licznik = 1;  
				halfBitCounter =1;
				//uartWriteData(0xDB);
//  
      }  
//  
//      //w obu przypadkach pólbitów  
//      zeruj_timer_i_flage_przepelnienia();  
			resetTimer();
//  
    } else {  
//  
//      //Uplynal czas calego bitu i jestesmy aktualnie w polowie  
//      //odbieranego bitu. Jest to moment, w którym ustalamy wartosc  
//      //odebranego bitu na podstawie kierunku zbocza.  
//  
//      //Przesun rejestr odbiorczy o jeden bit w lewo by zrobic  
//      //miejsce na odebrany bit  
//      dane_temp << 1  
			dataTemp <<=1;
//  
//      //Jezeli przerwanie wywolalo zbocze opadajace  
//      //oznacza to, ze odebralismy jedynke wiec ja dodajemy  
      if( edge == 0 ) { // PORTA->PCR[11] & PORT_PCR_IRQC(0x0A) ){ //bylo zbocze opadajace) {  
//        dane_temp |= 1;  
				dataTemp |=1;
			//	uartWriteData(0xD1);
      }  
//  
//      zeruj_timer_i_flage_przepelnienia();  
			resetTimer();
			bitNumber++;
  	//uartWriteData(0xD2);
//      //zwieksz licznik bitów  
//      numer_bitu++;  
    }  
//  
//    //--- czy to juz ostatni bit? --------------------------------  
//  
    if(bitNumber > 13) {  
			//uartWriteData(0xFF);
//      //Tak, odebrano ostatni bit zapisz dane do zmiennych globalnych  
//  
//      //1-Dane sa na szesciu najmlodszych bitach  
//      dane_odebrane = dane_temp[komenda]; 
			dataReceived = (dataTemp);// & 0x3F); //0b00111111;
			  uartWriteData(0xCC);
				uartWriteData(dataReceived);
			//dataTemp>>=6;
//      //2-Nr urzadzenia (adres) i bit toggle  
//      status = dane_temp[toggle bit oraz nr urzadzenia];
				uartWriteData(0xAA);
				deviceAddress = ( (dataTemp>>8));
				uartWriteData(deviceAddress);
			
			status = 1;



			bitNumber=0;
			
			PTB->PSOR |= 1<<8;


    }  
    break;  
  }

 
//  
//  //--- Blad lub koniec transmisji -------------------------------  
//  
//  //Jezeli na koncu funkcji przerwania numer bitu ma wartosc 1  
//  //to oznacza, ze zarzadano przerwania dekodowania z powodu bledu  
//  //lub poprawnego zakonczenia dekodowania  
  if(bitNumber == 0) {  
//    //ustawiamy stan poczatkowy  
//    zeruj_timer_i_flage_przepelnienia(); 
resetTimer();
//PORTA->PCR[11] |= PORT_PCR_IRQC(0x0A);
halfBitCounter=0;		
//    ustaw_zbocze_opadajace();  
//    polbit_licznik = 0; 
	
		
  }  
//

	
	if (irGetCommand() == 0xAA){
		engineStart(ENGINE0);
		engineLeft(ENGINE0,200);
	}
	else if (irGetCommand() == 0xAB){
		engineStart(ENGINE0);
		engineRight(ENGINE0,200);
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


	//TPM1->CONTROLS[1].CnV = TPM_CnV_VAL(0);

	// interrupts
//	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_CHIE_MASK;
//	TPM0->CONTROLS[2].CnSC |= TPM_CnSC_CHIE_MASK;
//	TPM0->CONTROLS[3].CnSC |= TPM_CnSC_CHIE_MASK;
	
	TPM1->SC |= TPM_SC_CMOD(1); // enable timer
//	NVIC_ClearPendingIRQ(TPM0_IRQn);
//	NVIC_EnableIRQ (TPM0_IRQn);
//	NVIC_SetPriority(TPM0_IRQn,3);

	/////
	
	
	
	
}
	



void TPM1_IRQHandler(){
	//NVIC_ClearPendingIRQ(TPM1_IRQn);
	//TPM1->SC |= TPM_SC_TOF_MASK;

}




  
  
//---------------------------------------------------------------------------  
  
/*
FUNKCJA_OBSLUGI_PRZERWANIA()  
{  
  
  //Czy poprzednio odebrane dane zostaly wykorzystane?  
  if(poprzednie dane niewykorzystane) wyjdz_z_tej_funkcji  
  
  //Na samym poczatku zmieniamy wykrywanie zbocza przeciwnego do  
  //poprzedniego. Robimy to po to, by nie umknelo nam zadne przerwanie,  
  //które moze wystapic w trakcie wykonywania niniejszej funkcji przerwania.  
  zmien_zbocze_na_przeciwne();  
  
  //W zaleznosci, który bit jest aktualnie dekodowany  
  switch(numer_bitu) {  
  
    //--- bit startowy nr 1 ------------------------------------  
  
  case 1:  
  
    //Wykryto pierwsze zbocze opadajace.  
    //Sprawdzamy, czy przerwa w sygnale byla wystarczajaco dluga  
    if(przerwa wystaczajaco dluga) {  
  
      //Przerwa miedzy kolejnymi ramkami byla wystarczajaca,  
      //by miec pewnosc, ze rozpoczynamy odbiór nowej ramki.  
      zeruj_timer_i_flage_przepelnienia();  
      numer_bitu=2;  
      dane_temp = 0;  
      polbit_licznik = 0;  
      //konczymy czekajac na zbocze narastajace drugiego bitu startu  
  
    } else {  
      //Zbyt krótka przerwa w sygnale pomiedzy ramkami  
      //dlatego te ramke danych musimy przeczekac.  
  
      //Tutaj nie trzeba nic wykonywac, poniewaz aktualnie numer_bitu  
      //jest równy 1 co oznacza, ze warunek na konczu funkcji  
      //przerwania ustawi stan poczatkowy  
    }  
    break;  
  
  
    //--- bit startowy nr 2 ------------------------------------  
  
  case 2:  
  
    //sprawdzamy, czy czas polowy bitu jest zgodny z parametrami  
    if(  
      wystapilo przepelnienie timera  
      lub pólbit za krótki  
      lub pólbit za dlugi  
      )  
    {  
      //Wykryto blad w sygnale.  
      //Przerywamy dekodowanie tej ramki rozpoczynamy od nowa  
      numer_bitu = 1;  
      break;  //break, by nie sprawdzal koncowego if(numer_bitu > 14)  
  
    } else {  
  
      //Czas jest z zakresu pólbitów  
  
      //Które zbocze wywolalo przerwanie?  
      if(zbocze opadajace) {  
        //Przerwanie wywolalo zbocze opadajace bitu startowego nr 2  
        //co oznacza, ze drugi bit startu odebrany prawidlowo  
        zeruj_timer_i_flage_przepelnienia();  
        numer_bitu++;  
      } else {  
        //wykryto pierwsze zbocze narastajace bitu startowego nr 2  
        zeruj_timer_i_flage_przepelnienia();  
      }  
    }  
    break;  
  
  
    //--- pozostale bity --------------------------------------  
  
  default:  
  
    //Tutaj odbieramy pozostale bity ramki  
  
    //W zaleznosci jaki czas uplynal od ostatniego zbocza (przerwania)  
    //Czy czas wykracza poza brzegowe parametry (min i max)  
    if(  
      wystapilo przepelnienie timera  
      lub czas krótszy niz minimalny dla pólbitu  
      lub czas dluzszy niz maksymalny dla calego bitu  
      )  
    {  
      //Wykryto blad w sygnale.  
      //Przerywamy dekodowanie tej ramki rozpoczynamy od nowa  
      numer_bitu = 1;  
      break;  //break, by nie sprawdzal koncowego if(numer_bitu > 14)  
  
      //Czy minal czas pólbitu?  
    } else if(  
             czas równy lub krótszy niz maksymalny dla pólbitu  
             )  
    {  
      //Uplynal czas równy polowie bitu  
  
      //sprawdzamy, czy to druga polowa bitu  
      if(polbit_licznik == 1) {  
  
        //Tak to druga polówka aktualnie dekodowanego bitu  
        //i jestesmy aktualnie w polowie czasu odbieranego  
        //bitu. Jest to moment, w którym ustalamy wartosc  
        //odebranego bitu na podstawie kierunku zbocza.  
  
        //Przesun dane o jeden bit w lewo, by zrobic miejsce  
        //na odebrany bit  
        dane_temp << 1  
  
        //Jezeli przerwanie wywolalo zbocze opadajace  
        //oznacza to, ze odebralismy jedynke wiec ja dodajemy  
        if(bylo zbocze opadajace) {  
          dane_temp |= 1;  
        }  
  
        //zeruj licznik pólbitów  
        polbit_licznik = 0;  
  
        //zwieksz licznik odebranych bitów  
        numer_bitu++;  
  
      } else {  
  
        //To pierwsza polowa dekodowanego bitu, czyli jestesmy  
        //aktualnie na poczatku czasu przesylanego bitu.  
  
        //Ustawiamy licznik pólbitów  
        polbit_licznik = 1;  
  
      }  
  
      //w obu przypadkach pólbitów  
      zeruj_timer_i_flage_przepelnienia();  
  
    } else {  
  
      //Uplynal czas calego bitu i jestesmy aktualnie w polowie  
      //odbieranego bitu. Jest to moment, w którym ustalamy wartosc  
      //odebranego bitu na podstawie kierunku zbocza.  
  
      //Przesun rejestr odbiorczy o jeden bit w lewo by zrobic  
      //miejsce na odebrany bit  
      dane_temp << 1  
  
      //Jezeli przerwanie wywolalo zbocze opadajace  
      //oznacza to, ze odebralismy jedynke wiec ja dodajemy  
      if(bylo zbocze opadajace) {  
        dane_temp |= 1;  
      }  
  
      zeruj_timer_i_flage_przepelnienia();  
  
      //zwieksz licznik bitów  
      numer_bitu++;  
    }  
  
    //--- czy to juz ostatni bit? --------------------------------  
  
    if(numer_bitu > 14) {  
  
      //Tak, odebrano ostatni bit zapisz dane do zmiennych globalnych  
  
      //1-Dane sa na szesciu najmlodszych bitach  
      dane_odebrane = dane_temp[komenda];  
  
      //2-Nr urzadzenia (adres) i bit toggle  
      status = dane_temp[toggle bit oraz nr urzadzenia];  
  
      //przygotuj sie do odbioru nowej ramki  
      numer_bitu = 1;  
  
      //koniec odbioru ramki ... uff nareszcie :-)  
    }  
    break;  
  }  
  
  //--- Blad lub koniec transmisji -------------------------------  
  
  //Jezeli na koncu funkcji przerwania numer bitu ma wartosc 1  
  //to oznacza, ze zarzadano przerwania dekodowania z powodu bledu  
  //lub poprawnego zakonczenia dekodowania  
  if(numer_bitu == 1) {  
    //ustawiamy stan poczatkowy  
    zeruj_timer_i_flage_przepelnienia();  
    ustaw_zbocze_opadajace();  
    polbit_licznik = 0;  
  }  
  
}  


*/
