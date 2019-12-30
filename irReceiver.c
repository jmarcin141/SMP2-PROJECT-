#include "irreceiver.h"


#define RC5ToleranceTime 250  //150 - 350 [us]  
#define RC5HalfBitTime 889 // [us]
#define RC5BitTime 1778 // [us]

#define ledRedOn (PTB->PCOR |= 1UL<<8)
#define ledRedOff (PTB->PSOR |= 1UL<<8)


uint8_t bit_position = 0; 
/* 	Start 11, toggle 0/1, Address MSB|LSB, Command MSB|LSB
		[S1|S2|Tgl|A4|A3|A2|A1|A0|C5|C4|C3|C2|C1|C0]
		
		Bit [0|1|2|3|4|5|6|7|8|9|10|11|12|13]    */

void irReceiverInitialize(){
		// IR receiver to PTB1
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	
	
	//PTB->PDDR &= ~(1UL<<1); 
	//PORTB->PCR[1] |= PORT_PCR_PE_MASK;//pull enable
	//PORTB->PCR[1] |= PORT_PCR_PS_MASK; // pull up
}

void TPMInitialize(){
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[8] |= PORT_PCR_MUX(1UL); 
	PTB->PDDR |= 1UL<<8;
	PTB->PSOR |= 1UL<<8;

	
	// IR receiver to PTB1
	//SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	PORTB->PCR[1] |= PORT_PCR_PE_MASK;
	PORTB->PCR[1] |= PORT_PCR_PS_MASK;
	PORTB->PCR[1] |= PORT_PCR_IRQC(0x0B); // interrupt on falling and rising edge
	
	
	
	///////
	
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
	
	
	
	
	NVIC_ClearPendingIRQ(PORTB_IRQn);
	NVIC_EnableIRQ(PORTB_IRQn);
	NVIC_SetPriority(PORTB_IRQn,1);
	
}

void PORTB_IRQHandler(){
	//NVIC_ClearPendingIRQ(PORTB_IRQn);
	//if (PORTB->PCR[1] & PORT_PCR_ISF_MASK){ ... }
	

	
	
	resetTimer();
	
	
		while(!(TPM1->STATUS & TPM_STATUS_TOF_MASK )) { 
			PTB->PCOR |= 1UL<<8;
		}
		
			PTB->PSOR |= 1UL<<8;
		
		
		
		
		PORTB->PCR[1] |= PORT_PCR_ISF_MASK;		
}



	
	



void TPM1_IRQHandler(){
	//NVIC_ClearPendingIRQ(TPM1_IRQn);
	//TPM1->SC |= TPM_SC_TOF_MASK;
	
	//ledRedOn;
}



//funkcja zerujaca timer oraz flage jego przepelnienia  
void resetTimer()  
{  
	//while( !(TPM1->STATUS & TPM_STATUS_TOF_MASK )) { }
	
	TPM1->SC |= TPM_SC_CMOD(0); // wylaczenie , zaladowanie wypelnienia, wlaczenie TPM
	TPM1->CNT = TPM_CNT_COUNT(0);
	TPM1->SC |= TPM_SC_CMOD(1);
	
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK;
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
