#include "uart.h"


uint8_t data_out = 0x00;
uint8_t data_in = 0x00;



void uartInitialize(){
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	// PTB3, PTB4 
	PORTB->PCR[3] |= PORT_PCR_MUX(3UL); //ALT3 - UART0_TX PIN3
	PORTB->PCR[4] |= PORT_PCR_MUX(3UL); //ALT3 - UART0_RX PIN4

	// kurrr...   :(
	//PORTA->PCR[18] &= ~PORT_PCR_MUX(0); // extal 
	//PORTA->PCR[19] &= ~PORT_PCR_MUX(0); // xtal
	//OSC0->CR |= OSC_CR_ERCLKEN_MASK;
//	OSC0->CR |= OSC_CR_SC16P_MASK;
//	
//	MCG->S |= MCG_S_IRCST_MASK; // enable fast clock 4MHz IRC
//	MCG->SC |= MCG_SC_FCRDIV(0); // 4MHz/4=1MHz
	/* 	000 Divide Factor is 1
			001 Divide Factor is 2.
			010 Divide Factor is 4.
			011 Divide Factor is 8.
			100 Divide Factor is 16
			101 Divide Factor is 32
			110 Divide Factor is 64
			111 Divide Factor is 128. */
	
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); 
	/* 
		00 Clock disabled
		01 MCGFLLCLK clock or MCGPLLCLK/2 clock //<< 48MHz ? 47,98
		10 OSCERCLK clock = 48MHz ?
		11 MCGIRCLK clock = 32kHz(default) or 4MHz
	*/
	
	UART0->C2 &= ~UART0_C2_TE_MASK; // transmitter disabled
	UART0->C2 &= ~UART0_C2_RE_MASK; // receiver disabled
	
	UART0->C4 |= UART0_C4_OSR(31); // over sampling ratio (31)
	
	// baud rate
	/*
				Asynch Module Clock			47,98MHz				47,98MHz
	BR =  -------------------- = -------------- = -------- = 1199,5
				(OSR + 1) * SBR					(31 +1 )* SBR		32*SBR
	*/
 
	//SBR = 1250 = 0x04E2
	UART0->BDH |= UART0_BDH_SBR(0x04);
	UART0->BDL |= UART0_BDL_SBR(0xE2);
	
	UART0->BDH &= ~UART0_BDH_SBNS_MASK; // 0 - 1 bit stop; 1 - 2 bits stop
	
	UART0->C1 &= ~UART0_C1_M_MASK; // 8 bit data; start + 8 bit data(LSB first) + stop
	UART0->C1 &= ~UART0_C1_PE_MASK; // 0 - No hardware parity generation or checking. ; 1 - Parity enabled.
	
	// disabled interrupts
	UART0->C2 &= ~UART0_C2_TIE_MASK;
	UART0->C2 &= ~UART0_C2_RIE_MASK;
	
	
	UART0->C2 |= UART0_C2_TE_MASK; // transmitter enabled
	UART0->C2 |= UART0_C2_RE_MASK; // receiver enabled
	
	
}

void uartWriteData(uint8_t data){
	while(!(UART0->S1 & UART0_S1_TDRE_MASK)){}
	UART0->D = data;
}

uint8_t uartReadData(){
	//while(!(UART0->S1 & UART0_S1_RDRF_MASK)){}
	//data_in = UART0->D;
	while((UART0->S1 & UART0_S1_RDRF_MASK)){data_in = UART0->D;}
	return UART0->D;
}

uint8_t getDataOut(){
	return data_out;
}
uint8_t getDataIn(){
	return data_in;
}

void setDataOut(uint8_t data){
	data_out = data;
}


void uartDemoHello(){
	uartWriteData('H');
	uartWriteData('e');
	uartWriteData('l');
	uartWriteData('l');
	uartWriteData('o');
	uartWriteData('!');
	uartWriteData('-');
	uartWriteData('A');
	uartWriteData('G');
	uartWriteData('H');
	uartWriteData(0x20);
	uartWriteData(0x21);
	uartWriteData('<');
	uartWriteData('S');
	uartWriteData('M');
	uartWriteData('P');
	uartWriteData('2');
	uartWriteData('>');
}




void uartEnable(){
	UART0->C2 |= UART0_C2_TE_MASK; // transmitter disabled
	UART0->C2 |= UART0_C2_RE_MASK; // receiver disabled
}

void uartDisable(){
	UART0->C2 &= ~UART0_C2_TE_MASK; // transmitter disabled
	UART0->C2 &= ~UART0_C2_RE_MASK; // receiver disabled
}


