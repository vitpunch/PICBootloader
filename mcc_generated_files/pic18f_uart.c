/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#define  STX   0x55

#define  READ_VERSION   'v'
#define  READ_FLASH     'r'
#define  WRITE_FLASH    'w'
#define  ERASE_FLASH    'e'
#define  READ_EE_DATA   'R'
#define  WRITE_EE_DATA  'W'
#define  READ_CONFIG    'c'
#define  WRITE_CONFIG   'C'
#define  CALC_CHECKSUM  's'
#define  RESET_DEVICE   'l'

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "bootload.h"
#include "mcc.h"

uint8_t RdData ();
void    WrData (uint8_t  data);
frame_t  frame;
// *****************************************************************************
//Autobaud:
//
// ___     ___     ___     ___     ___     __________
//    \_S_/ 1 \_0_/ 1 \_0_/ 1 \_0_/ 1 \_0_/ Stop
//       |                                |
//       |-------------- p ---------------|
//
// EUSART autobaud works by timing 4 rising edges (0x55).  It then uses the
// timed value as the baudrate generator value.
// *****************************************************************************
void Run_Bootloader()
{
    uint16_t    index;
    char        ch;

    while (1)
    {

        index = 0;       // Point to the buffer
        ch = 0;

        while (1)//ch!=0x0D )
        {
            ch = EUSART1_Read();// Get the data
            EUSART1_Write(ch);
            //frame.buffer [index++] = ch;
        }


// *****************************************************************************
// Send the data buffer back.
// *****************************************************************************
        EUSART1_Write(' ');//STX);
        index = 0;
        while (frame.buffer[index]!=0x0D)
        {
            EUSART1_Write (frame.buffer [index++]);
        }
    }
}
// *****************************************************************************


