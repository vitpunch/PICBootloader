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



