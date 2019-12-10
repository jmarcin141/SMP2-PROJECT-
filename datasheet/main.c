#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



int main (void)
{

	//################# Konfiguracja PWM ############################
	//Pierwszy kanal
	TCCR0 |= (1<<WGM01);// Tryb : Fast PWM 8bit
	TCCR0 |= (1<<WGM00);

	TCCR0 |= (1<<COM01)|(0<<COM00);//Clear OC0 on Compare Match
	TCCR0 |= (0<<CS02)|(1<<CS01)|(1<<CS00);// preskaler = 64
	/*
	000 No clock source (Timer/Counter stopped).
	001 clkI/O/(No prescaling )
	010 clkI/O/8 (From prescaler)
	011 clkI/O/64 (From prescaler)
	100 clkI/O/256 (From prescale)
	101 clkI/O/1024 (From pres)
	*/

	// datasheet, page 107!!!!!
	//Drugi kanal
	TCCR1A |= (1<<WGM10);// Tryb : Fast PWM 8bit
	TCCR1A |= (0<<WGM11);
	TCCR1B |= (1<<WGM12); // TCCR1B !!

	TCCR1A |= (1<<COM1A1)|(0<<COM1A0);//Clear OC1A on Compare Match
	TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);// preksaler = 64
	
	
	/*
	wyjscie na OCR0 (PB3) lewo
	wyjscie na OC1A (PD5) prawo
	ENABLE aktywe H 
	*/
	

	//################# Przetwornik ADC ############################
	ADMUX |= (0<<REFS0) | (0<<REFS1); // VREF OFF from VCC // VREF JP12
	/*
	REFS1 REFS0 Voltage Reference Selection
	0 0 AREF, Internal Vref turned off
	0 1 AVCC with external capacitor at AREF pin
	1 0 Reserved
	1 1 Internal 2.56V Voltage Reference with external capacitor at AREF pin
    */
	
	ADMUX |= (1<<MUX2) | (1<<MUX1) | (1<<MUX0); // PA7 / ADC7 input
	
	ADCSRA |= (1<<ADEN); //enable ADC
	ADCSRA |= (1<<ADSC); // pojedyncza konwersja / 1-free running mode
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // dzielnik f, 16MHz -> 128kHz
	
	ADCSRA |= (1<<ADLAR); // starsza do lewej


	//################# Konfiguracja portow ############################
	DDRB = 0xFF; // portB jako wyjscie dla PWM OC0(PB3), oraz (PB0) wyjœcie dla ENABLE (uklad L293D)
	DDRD = 0b11111110; // portD jako wyjscie dla PWM OC1A(PD5)
    PORTD= (1<<PIND0); //pullup 

	DDRA = 0x00; //wejscie pod Przetwornik ADC (PA7)
	PORTA = 0XFF; //pull-up


//######### < L_pos, H_pos > = martwy zakres, silnik nie pracuje w tym przedziale ####################
int h_pos = 600;//560;
int l_pos = 500;//540;



	while(1)
	{
		if(~(PIND >> 0) & 0x01) // gdy nacisniety przycisk na PD0, zmien PB0 (on/off engine)
		{
			_delay_ms(10);
			if(~(PIND >> 0) & 0x01)
			{
				PORTB^=(1<<PINB0);
				_delay_ms(50);
			}
		}
		
			
		if ((ADC >= l_pos) && (ADC <= h_pos))
		{
			//stop
			OCR0 = 0;
			OCR1A = 0;
		}
		
		else if (ADC >h_pos)
		{
			//prawo
			OCR0 = 0; //wylaczyc drugi kanal!
			OCR1A = ADC >> 2; // ADC w trybie 10 bit, usuwamy LSB do postaci 8 bit 
		}
		
		else if (ADC <l_pos)
		{	
			//lewo
			OCR1A = 0; //wylaczyc drugi kanal!
			OCR0 =  0xFF - (ADC >> 2);
		}

	}
}
