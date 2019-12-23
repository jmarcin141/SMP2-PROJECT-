#include "irreceiver.h"

void irReceiverInitialize(){
		// IR receiver to PTA0
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	//PTB->PDDR &= ~(1UL<<1); 
	//PORTB->PCR[1] |= PORT_PCR_PE_MASK;//pull enable
	//PORTB->PCR[1] |= PORT_PCR_PS_MASK; // pull up
}

/*
//funkcja zerujaca timer oraz flage jego przepelnienia  
zeruj_timer_i_flage_przepelnienia()  
{  
  resetuj_preskaler_timera  
  zeruj_timer  
  zgas_flage_przepelnienia_timera  
}  
  
  
//---------------------------------------------------------------------------  
  
  
FUNKCJA_OBSLUGI_PRZERWANIA()  
{  
  
  //Czy poprzednio odebrane dane zostaly wykorzystane?  
  if(poprzednie dane niewykorzystane) wyjdz_z_tej_funkcji  
  
  //Na samym poczatku zmieniamy wykrywanie zbocza przeciwnego do  
  //poprzedniego. Robimy to po to, by nie umknelo nam zadne przerwanie,  
  //kt�re moze wystapic w trakcie wykonywania niniejszej funkcji przerwania.  
  zmien_zbocze_na_przeciwne();  
  
  //W zaleznosci, kt�ry bit jest aktualnie dekodowany  
  switch(numer_bitu) {  
  
    //--- bit startowy nr 1 ------------------------------------  
  
  case 1:  
  
    //Wykryto pierwsze zbocze opadajace.  
    //Sprawdzamy, czy przerwa w sygnale byla wystarczajaco dluga  
    if(przerwa wystaczajaco dluga) {  
  
      //Przerwa miedzy kolejnymi ramkami byla wystarczajaca,  
      //by miec pewnosc, ze rozpoczynamy odbi�r nowej ramki.  
      zeruj_timer_i_flage_przepelnienia();  
      numer_bitu=2;  
      dane_temp = 0;  
      polbit_licznik = 0;  
      //konczymy czekajac na zbocze narastajace drugiego bitu startu  
  
    } else {  
      //Zbyt kr�tka przerwa w sygnale pomiedzy ramkami  
      //dlatego te ramke danych musimy przeczekac.  
  
      //Tutaj nie trzeba nic wykonywac, poniewaz aktualnie numer_bitu  
      //jest r�wny 1 co oznacza, ze warunek na konczu funkcji  
      //przerwania ustawi stan poczatkowy  
    }  
    break;  
  
  
    //--- bit startowy nr 2 ------------------------------------  
  
  case 2:  
  
    //sprawdzamy, czy czas polowy bitu jest zgodny z parametrami  
    if(  
      wystapilo przepelnienie timera  
      lub p�lbit za kr�tki  
      lub p�lbit za dlugi  
      )  
    {  
      //Wykryto blad w sygnale.  
      //Przerywamy dekodowanie tej ramki rozpoczynamy od nowa  
      numer_bitu = 1;  
      break;  //break, by nie sprawdzal koncowego if(numer_bitu > 14)  
  
    } else {  
  
      //Czas jest z zakresu p�lbit�w  
  
      //Kt�re zbocze wywolalo przerwanie?  
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
      lub czas kr�tszy niz minimalny dla p�lbitu  
      lub czas dluzszy niz maksymalny dla calego bitu  
      )  
    {  
      //Wykryto blad w sygnale.  
      //Przerywamy dekodowanie tej ramki rozpoczynamy od nowa  
      numer_bitu = 1;  
      break;  //break, by nie sprawdzal koncowego if(numer_bitu > 14)  
  
      //Czy minal czas p�lbitu?  
    } else if(  
             czas r�wny lub kr�tszy niz maksymalny dla p�lbitu  
             )  
    {  
      //Uplynal czas r�wny polowie bitu  
  
      //sprawdzamy, czy to druga polowa bitu  
      if(polbit_licznik == 1) {  
  
        //Tak to druga pol�wka aktualnie dekodowanego bitu  
        //i jestesmy aktualnie w polowie czasu odbieranego  
        //bitu. Jest to moment, w kt�rym ustalamy wartosc  
        //odebranego bitu na podstawie kierunku zbocza.  
  
        //Przesun dane o jeden bit w lewo, by zrobic miejsce  
        //na odebrany bit  
        dane_temp << 1  
  
        //Jezeli przerwanie wywolalo zbocze opadajace  
        //oznacza to, ze odebralismy jedynke wiec ja dodajemy  
        if(bylo zbocze opadajace) {  
          dane_temp |= 1;  
        }  
  
        //zeruj licznik p�lbit�w  
        polbit_licznik = 0;  
  
        //zwieksz licznik odebranych bit�w  
        numer_bitu++;  
  
      } else {  
  
        //To pierwsza polowa dekodowanego bitu, czyli jestesmy  
        //aktualnie na poczatku czasu przesylanego bitu.  
  
        //Ustawiamy licznik p�lbit�w  
        polbit_licznik = 1;  
  
      }  
  
      //w obu przypadkach p�lbit�w  
      zeruj_timer_i_flage_przepelnienia();  
  
    } else {  
  
      //Uplynal czas calego bitu i jestesmy aktualnie w polowie  
      //odbieranego bitu. Jest to moment, w kt�rym ustalamy wartosc  
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
  
      //zwieksz licznik bit�w  
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
