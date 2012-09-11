///////////////////////////////////////////////////////////
//
// On-Screen Keyboard
//
// 11.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "io.h"
#include "firmware.h"

void Keyboard_Init();
void Keyboard_Draw();
char* Keyboard_GetText();
int Keyboard_IsConfirmed();

#endif

