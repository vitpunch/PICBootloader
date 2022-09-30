/* 
 * File:   parser.h
 * Author: spido
 *
 * Created on 30 сентября 2022 г., 17:48
 */

#ifndef PARSER_H
#define	PARSER_H

char TxtHalfByteToHalfByte(char halfByte);
char TxtByteToChar(uint8_t index);
char IsCorrectHEXLine(void);

void SendByteToUART(uint8_t);

void ParseCommand(void);
void ReadSN(void);
void ParseHEXLine(void);

char ParsingErroor;
char ParsedBuffer[22];
char LineBuffer[51];
char FlashBuffer[82];

#endif	/* PARSER_H */

