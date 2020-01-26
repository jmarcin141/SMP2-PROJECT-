#include "uart.h"


uint8_t data_out = 0x00;
uint8_t data_in = 0x00;



void uartInitialize(){
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[1] |= PORT_PCR_MUX(2UL); //ALT2 - UART0_TX PIN1
	PORTB->PCR[2] |= PORT_PCR_MUX(2UL); //ALT2 - UART0_RX PIN2

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
	
	// F=41943040  HZ
	// baud rate
	/*
				Asynch Module Clock			41943040 Hz			41943040 Hz
	BR =  -------------------- = -------------- = ----------- = 1200,29
				(OSR + 1) * SBR					(31 +1 )* SBR		32*SBR
	*/
 
	UART0->BDH |= UART0_BDH_SBR(0x04);
	UART0->BDL |= UART0_BDL_SBR(0x44);
	
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
	uartWriteData('E');
	uartWriteData('n');
	uartWriteData('g');
	uartWriteData('i');
	uartWriteData('n');
	uartWriteData('e');
	uartWriteData('-');
	uartWriteData('C');
	uartWriteData('o');
	uartWriteData('n');
	uartWriteData('t');
	uartWriteData('r');
	uartWriteData('o');
	uartWriteData('l');
	uartWriteData('-');
	uartWriteData('L');
	uartWriteData('a');
	uartWriteData('b');	
	uartWriteData('V');
	uartWriteData('i');
	uartWriteData('e');
	uartWriteData('w');
	
}




void uartEnable(){
	UART0->C2 |= UART0_C2_TE_MASK; // transmitter disabled
	UART0->C2 |= UART0_C2_RE_MASK; // receiver disabled
}

void uartDisable(){
	UART0->C2 &= ~UART0_C2_TE_MASK; // transmitter disabled
	UART0->C2 &= ~UART0_C2_RE_MASK; // receiver disabled
}


