///////////////////////////////////////////////////////////
//
// GLCD Simulator (for GLCD Library)
//
// 20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _LCD2BMP_H_
#define _LCD2BMP_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void LCD2BMP_Save(char* file, uint8_t data[128*8]);


#endif
