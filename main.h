/* 
 * File:   main.h
 * Author: spido
 *
 * Created on September 17, 2022, 2:54 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include <string.h>
#include "Led.h"
#include "UART.h"

void ParseTxtHexLineToByteBuffer(void);
char TxtHalfByteToHalfByte(char halfByte);
char TxtByteToChar(uint8_t index);
char IsCorrectHEXLine(void);

void ParseCommand(void);
void ReadSN(void);
void WriteFirmWare(void);
void ParseHEXLine(void);

char ParsingErroor;
char ParsedBuffer[22];
char LineBuffer[51];
char FlashBuffer[82];

#endif	/* MAIN_H */

