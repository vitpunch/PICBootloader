#include "main.h"

#define DEBUG //RELEASE or DEBUG
#define SET_LED 0
#define RESET_LED 1

#ifdef RELEASE
    #define CORNER_LED PORTBbits.RB6
    #define MIDDLE_LED PORTBbits.RB4
    #define TRISMask 0b10101111
#endif
#ifdef DEBUG
    #define CORNER_LED PORTBbits.RB7
    #define MIDDLE_LED PORTBbits.RB6
    #define TRISMask 0b00111111
#endif
    void LedInitialize(void)
    {
        TRISB &= TRISMask;
    }

void SetCornerLed(void)
{
    CORNER_LED = SET_LED;
}
void ResetCornerLed(void)
{
    CORNER_LED = RESET_LED;
}
void SetMiddleLed(void)
{
    MIDDLE_LED = SET_LED;
}
void ResetMiddleLed(void)
{
    MIDDLE_LED = RESET_LED;
}
void ToggleCornerLed(void)
{
    CORNER_LED ^= 0xFF;
}
void ToggleMiddleLed(void)
{
    MIDDLE_LED ^= 0xFF;
}