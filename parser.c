#include "main.h"

void ParseCommand(void)
/*
 **** расшифровывает команду из буфера строки и вызывает соответствующую функцию
 * parse строку из буфера строки парсит из формата HEX в байты в буфер HEX строки
 * 
 read выдаёт в терминал hex файл прошивки и Епрома
 rSN выдаёт серийный номер моталки или 'FFFFFFFF' если не задан 4 байта, хранится в конце Епрома
 wSN xxxxxxxx записывает серийный номер в моталку, если он = 0 или E
 write следом ожидает зашифрованный hex файл с прошивкой
 checksum выдаёт чексумму прошивки
 
 */
{
    if (!strcmp(LineBuffer,"rSN"))
        ReadSN();
    else if (!strcmp(LineBuffer,"write"))
        WriteFirmWare();
    else if (!strcmp(LineBuffer,"parse"))
        ParseHEXLine();
    else
        SendLineToUART("\r\nDon't understand\r\n");

}

void ReadSN(void)
//считывает серийный номер и выдаёт в терминал
{
    uint16_t SNAddr = 0x3FC;
    while (SNAddr<0x400)
    {
        SendByteToUART(DATAEE_ReadByte(SNAddr++));
    }
    SendLineToUART("\r\n");
}

char TxtHalfByteToHalfByte(char halfByte)
//принимает символ 0-F возвращает байт от 0x00 до 0x0F с проверкой и установкой ошибки
{
    //в допустимом ли диапазоне символ
    if ((halfByte<0x30 ||
        halfByte>0x46 ||
        (halfByte>0x39 && halfByte <0x41))
            && ParsingErroor==0)
    
    {
        ParsingErroor = 0xF0;
        return 0xFF;
    }
    char ret;
    if (halfByte<0x3A)
        return (halfByte-0x30);
    else
        return (halfByte-0x37);
}

char TxtByteToChar(uint8_t index)
//принимает два байта символьное представление HEX, возвращает байт
{
    return (TxtHalfByteToHalfByte(LineBuffer[index])<<4)|
            TxtHalfByteToHalfByte(LineBuffer[index+1]);
}

void ParseTxtHexLineToByteBuffer(void)
{
    
}

void ParseHEXLine(void)
//    строку из буфера строки парсит из формата HEX в байты в буфер HEX строки
//    устанавливает код ошибки <ParsingErroor> по результату парсинга
{

    ReceiveLineFromUARTToBuffer();
    ParsingErroor = 0;
    
    uint8_t index = 0;
    uint8_t checksum = 0;
//    очищаем буфер
    while(index<21) 
    {
        ParsedBuffer[index]=0;
        index++;
    }
//    проверяем первый символ длину послания
    if (LineBuffer[0]!=':' && ParsingErroor==0)
        ParsingErroor=0xf1;
    ParsedBuffer[0] = TxtByteToChar(1);//длина послания
    if(ParsedBuffer[0]>0x10)
    {
        ParsingErroor=0xF5;
        return;
    }
    index =1;
    while(index<ParsedBuffer[0]+5) // парсим текст в байты
    {
        ParsedBuffer[index] = TxtByteToChar((index<<1)+1);
        index++;
    }
//    считаем КС
    index = 0;
    while (index<ParsedBuffer[0]+5)
    {
        checksum+=ParsedBuffer[index];
        index++;
    }
    if(checksum!=0x00 && ParsingErroor==0)
        ParsingErroor=0xf2;

    SendByteToUART(ParsingErroor);
}

