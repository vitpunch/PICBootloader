//************
#include "mcc_generated_files/mcc.h"
#include "main.h"

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
    //uint8_t ch;

// здесь запущен бутлоадер    
    while (2)
    {
        //сначала получаем команду в буфер
        ReceiveLineFromUARTToBuffer();
        //теперь надо её обработать
        ParseCommand();
        
        //SendLineFromBuffToUART();        
    }
}

void ParseCommand(void)
/*
 read выдаёт в терминал hex файл прошивки и Епрома
 rSN выдаёт серийный номер моталки или 'FFFFFFFF' если не задан 4 байта, хранится в конце Епрома
 wSN xxxxxxxx записывает серийный номер в моталку, если он = 0 или E
 write следом ожидает зашифрованный hex файл с прошивкой
 checksum выдаёт чексумму прошивки
 
 */
{
//    //************************
//    strncpy(LineBuffer, ":107C1E001EEF3EF0030E206E080E1F6E770EE82E3E", 100);//", 100);*LineBuffer=":107C1E001EEF3EF0030E206E080E1F6E770EE82E3E";
//    ParseHEXLine();
//    //***********************
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
{
    uint16_t SNAddr = 0x3FC;
    while (SNAddr<0x400)
    {
        SendByteToUART(DATAEE_ReadByte(SNAddr++));
    }
}
char TxtHalfByteToHalfByte(char halfByte)
{
    //в допустимом ли диапазоне символ
    if ((halfByte<0x30 ||
        halfByte>0x46 ||
        (halfByte>0x39 && halfByte <0x41))
            && ParsingErroor==0)
    
    {
        ParsingErroor = 0xF0;
        return 0x00; // ошибка
    }
    char ret;
    if (halfByte<0x3A)
        return (halfByte-0x30);
    else
        return (halfByte-0x37);
}
char TxtByteToChar(uint8_t index)
{
    return (TxtHalfByteToHalfByte(LineBuffer[index])<<4)|
            TxtHalfByteToHalfByte(LineBuffer[index+1]);
}
void ParseTxtHexLineToByteBuffer(void)
{
    uint8_t index = 0;
    //uint8_t maxindex;
    uint8_t checksum = 0;
    while(index<21) //очищаем буфер
    {
        ParsedBuffer[index]=0;
        index++;
    }
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
    index = 0;
    while (index<ParsedBuffer[0]+5) //считаем кс
    {
        checksum+=ParsedBuffer[index];
        index++;
    }
    SendByteToUART(checksum);
    if(checksum!=0x00 && ParsingErroor==0)
        ParsingErroor=0xf2;
    //ParsedBuffer[21]=0x00;
}
void ParseHEXLine(void)
{
    ReceiveLineFromUARTToBuffer();
    ParsingErroor = 0;
    ParseTxtHexLineToByteBuffer();
    uint8_t index = 0;
    SendByteToUART(ParsingErroor);
    while (index<22)
    {
        EUSART1_Write(ParsedBuffer[index++]);
    }
}

void WriteFirmWare(void)
{
    //стираем всю память
    // начальный адрес буфера 0x1000
    //ждём строку
    //парсим их HEX
    //если не валидная - выходим с ошибкой
    //если валидная 
    // Проверяем адрес, вычисляем адрес блока,
    //  если текущий - добавляем в буфер
    //  усли нет - пишем блок
    // Если нет
    //  адрес идёт в следующем блоке?
    //   нет- записываем буфер, очищаем первые 64 байта в буфере, начальный адрес буфера берём из прилетевшей строки
    
}

