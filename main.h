/* 
 * File:   main.h
 * Author: spido
 *
 * Created on September 17, 2022, 2:54 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define WORK_MODE 0x3FB

#include <xc.h>
#include <string.h>
#include "Led.h"
#include "UART.h"
#include "parser.h"
#include "mcc_generated_files/memory.h"


void StartBootloader(void);
bool BootloadRequired(void);
#endif	/* MAIN_H */

