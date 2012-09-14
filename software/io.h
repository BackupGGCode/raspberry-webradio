///////////////////////////////////////////////////////////
//
// IO Library (using IO Bridge)
//
// 20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _IOLIB_H_
#define _IOLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>  
#include "settings.h"
#include "macros.h"

// define SIMULATE to simulate I/O (read from io.sim)
//#define SIMULATE

#ifndef SIMULATE
 #include <wiringPi.h>
#endif

// speed (the higher the value, the slower the speed)
#define IO_REPEAT 160
// GPIO pins
#define IO_CLOCK  23
#define IO_DATA   24

void IO_Init();
void IO_Get();
uint8_t IO_GetButton(uint8_t btn);
uint8_t IO_GetButtonLong(uint8_t btn);
int8_t IO_GetRotaryValue();
uint8_t IO_HasChanged();
void IO_SetSimulate(uint8_t sim);


#endif


