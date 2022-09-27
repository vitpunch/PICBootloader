#include "main.h"
#include "mcc_generated_files/eusart1.h"



void ReceiveLineFromUARTToBuffer(void)
{
    uint8_t index = 0;
    uint8_t ch;
    
    SetCornerLed();
    
    while (index<50)
    {
        ch = EUSART1_Read();
        LineBuffer[index] = ch;
        EUSART1_Write(ch);
        ToggleCornerLed();
        index++;
        
        if (ch == 0x0A)
            break;
    }
    LineBuffer[index-2]=0;
}        

void SendLineFromBuffToUART(void)
{
    uint8_t index = 0;
    uint8_t ch;
    
    SetCornerLed();
    
    while (index<50)
    {
        ch = LineBuffer[index];
        index++;        
        if (!ch)
            break;
        EUSART1_Write(ch);
        ToggleCornerLed();
    }
    EUSART1_Write(0x0D);
    EUSART1_Write(0x0A);
}
void SendLineToUART(char *start)
{
    uint16_t index = 0;
    while (start[index]!=0)
    {
        EUSART1_Write(start[index++]);
    }
}
void SendByteToUART(uint8_t byte)
{
    //EUSART1_Write(byte);
    SendHalfByteToUART((byte & 0xF0)>>4);
    SendHalfByteToUART(byte & 0xF);
    
}
void SendHalfByteToUART(uint8_t halfbyte)
{
    if (halfbyte<0xA)
        halfbyte +=0x30;
    else
        halfbyte += 0x37;
    EUSART1_Write(halfbyte);
}