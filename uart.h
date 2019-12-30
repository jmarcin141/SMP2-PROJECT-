/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    uart.h
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef uart_h
#define uart_h
#include "MKL05Z4.h"   /* Device header */	

#define PIN_UART_RX 1
#define PIN_UART_TX 2


uint8_t getDataOut();
uint8_t getDataIn();
void setDataOut(uint8_t data);

void uartInitialize();
void uartEnable();
void uartDisable();
void uartWriteData(uint8_t data);
uint8_t uartReadData();

void uartDemoHello();

#endif
