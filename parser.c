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
    else if (!strcmp(LineBuffer,"writeHEX"))
        WriteHEXFile();
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
            && ParsingError==0)
    
    {
        ParsingError = 0xF0;
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


void ParseHEXLine(void)
//    строку из буфера строки парсит из формата HEX в байты в буфер HEX строки
//    устанавливает код ошибки <ParsingErroor> по результату парсинга
{

    ReceiveLineFromUARTToBuffer();
    ParsingError = 0;
    
    uint8_t index = 0;
    uint8_t checksum = 0;
//    очищаем буфер
    while(index<21) 
    {
        ParsedBuffer[index]=0;
        index++;
    }
//    проверяем первый символ длину послания
    if (LineBuffer[0]!=':' && ParsingError==0)
        ParsingError=0xf1;
    ParsedBuffer[0] = TxtByteToChar(1);//длина послания
    if(ParsedBuffer[0]>0x10)
    {
        ParsingError=0xF5;
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
    if(checksum!=0x00 && ParsingError==0)
        ParsingError=0xf2;

    SendByteToUART(ParsingError);
}

void WriteHEXFile(void)
{
    // сначала стираем всю флэш память
    for(uint32_t i = 0x1000; i < END_FLASH; i += ERASE_FLASH_BLOCKSIZE)
    {
        FLASH_EraseBlock(i);
    }
    uint32_t CurrenBlock = 0xFFFF;
    uint32_t ParsedAdress;
    char WriteBuffer [80];
    
    do
    {

        //считали строку
        ParseHEXLine();

        //проверить ошибки
        if (ParsingError !=0)
        {
            SendLineToUART("\r\nError\r\n");
            SendByteToUART(ParsingError);
            break;
        }
        if (ParsedBuffer[0]==0)
                ParsingError = 0xF0; //обнаружен конец файла
        
        ParsedAdress = (((uint32_t)ParsedBuffer[1]) << 8) | (uint32_t)ParsedBuffer[2];
        
        if (CurrenBlock == 0xFFFF)
            CurrenBlock = ParsedAdress & 0xFFC0; //только при первой итерации
        
        if ((ParsedAdress & 0xFFC0) != CurrenBlock)
        {
            //записать текуущий блок
            FLASH_WriteBlock(CurrenBlock, (uint8_t *)WriteBuffer);   
            
            //сдвинуть хвост
            for (uint8_t i = 0; i<0x10; i++)
            {
                WriteBuffer[i] = WriteBuffer[i+WRITE_FLASH_BLOCKSIZE];
            }
            for (uint8_t i = 0x10; i< 80; i++)
                WriteBuffer[i] = 0x0;
            CurrenBlock += WRITE_FLASH_BLOCKSIZE;
        }
        // проверили в текущем ли блоке она
        
        //TODO записали блок. В буфере остался хвост. если новая строка не из следующего блока - записать хвост и поменять 
        
        for (uint8_t j = 0; j < ParsedBuffer[0]; j++)
        {
            WriteBuffer[(ParsedAdress & 0x3F) + j] = ParsedBuffer[j + 4];
        }
    }
    while(ParsingError != 0xF0); //до конца файла
    FLASH_WriteBlock(CurrenBlock, (uint8_t *)WriteBuffer);
    SendLineToUART("\r\nSuccess!\r\n");
}

//добавить проверку начального адреса в прошивке
