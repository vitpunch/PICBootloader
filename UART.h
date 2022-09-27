/* 
 * File:   ReceiveLineFromUART.h
 * Author: spido
 *
 * Created on September 17, 2022, 4:33 PM
 */

#ifndef UART_H
#define	UART_H

uint8_t EUSART1_Read(void);
void    EUSART1_Write(uint8_t);

void ReceiveLineFromUARTToBuffer(void);
void SendLineFromBuffToUART(void);
void SendLineToUART(char *start);
void SendByteToUART(uint8_t);
void SendHalfByteToUART(uint8_t halfbyte);

#endif	/* RECEIVELINEFROMUART_H */

