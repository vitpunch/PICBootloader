/* 
 * File:   Led.h
 * Author: spido
 *
 * Created on September 17, 2022, 2:56 PM
 */

#include <xc.h>

#ifndef LED_H
#define	LED_H

void SetCornerLed(void);
void ResetCornerLed(void);
void SetMiddleLed(void);
void ResetMiddleLed(void);
void ToggleCornerLed(void);
void ToggleMiddleLed(void);
void LedInitialize(void);

#endif	/* LED_H */

