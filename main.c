//************
#include "main.h"
#include "mcc_generated_files/mcc.h"


void main(void)
{
    LedInitialize();
    SYSTEM_Initialize();
    
    SetCornerLed();
    ResetMiddleLed();

    // Initialize the device

    
//    __delay_ms(5000);
    
    if (BootloadRequired() == true)
        StartBootloader();
    SendLineToUART("\r\nSMP\r\n");    
    STKPTR = 0x00;
	BSR = 0x00;
    __asm( "goto  " "0x1000" );//asm ("goto  "  str(NEW_RESET_VECTOR));

    while(1)
    {
    }
}

// здесь запущен бутлоадер    
void StartBootloader()
{
    SendLineToUART("\r\nBS\r\n");   
    while (2)
    {
        //сначала получаем команду в буфер
        ReceiveLineFromUARTToBuffer();
        //теперь надо её обработать
        ParseCommand();
    } 
}

bool BootloadRequired(void)
{

    
    char BLR = DATAEE_ReadByte(WORK_MODE);
    SendLineToUART("\r\nRequired?");
    SendByteToUART(BLR);
    switch (BLR)
    {
        case 0xFF:
            DATAEE_WriteByte(WORK_MODE,0x01); //ожидаем 2 секунды
            SetCornerLed();
            ResetMiddleLed();
            for(uint8_t i = 0;i < 6; i++)
            {
                __delay_ms(500);
                ToggleCornerLed();
                ToggleMiddleLed();
            }
            DATAEE_WriteByte(WORK_MODE,0xFF);
            return (false);
        case 0x01:
            DATAEE_WriteByte(WORK_MODE,0x02); //ожидаем 10 секунд
            SetCornerLed();
            ResetMiddleLed();
            for(uint8_t i = 0;i < 50; i++)
            {
                __delay_ms(200);
                ToggleCornerLed();
                ToggleMiddleLed();
            }
            DATAEE_WriteByte(WORK_MODE,0xFF);
            return (false);
        case 0x02:
            DATAEE_WriteByte(WORK_MODE,0xFF);
            return (true);
    }
    return false;
}
