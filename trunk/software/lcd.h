///////////////////////////////////////////////////////////
//
// GLCD Drawing Library (using GLCD Bridge)
//
// 20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _GLCD_H_
#define _GLCD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include "lcd2bmp.h"
#include "macros.h"


// define SIMULATE to disable hardware output
//#define SIMULATE

#ifndef SIMULATE
  #include <wiringPi.h>
#endif

// speed (the higher the value, the slower the speed)
// -> 30 = ~12 FPS, 35 = ~10 FPS
#define REPEAT 50
// GPIO pins
#define CLOCK  7
#define DATA   8

typedef struct {
	uint8_t* name;
	uint8_t color;
} GLCDD_Font;

typedef struct {
	int x, y, w, h;
} GLCDD_Rect;

void GLCDD_SetSimulate(uint8_t sim);
void GLCDD_Init();
void GLCDD_SetPixel(uint8_t x, uint8_t y);
uint8_t GLCDD_GetPixel(uint8_t x, uint8_t y);
void GLCDD_ClearPixel(uint8_t x, uint8_t y);
void GLCDD_TogglePixel(uint8_t x, uint8_t y);
void GLCDD_Clear();
void GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void GLCDD_Circle(uint8_t x, uint8_t y, uint8_t r, uint8_t filled);
void GLCDD_Rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t filled);
void GLCDD_RectRounded(uint8_t x1, uint8_t y1, uint8_t w, uint8_t h, uint8_t filled);
int GLCDD_PutCharCol(const uint8_t *font, int x, int y, char c, uint8_t color);
int GLCDD_PutChar(const uint8_t *font, int x, int y, char c);
int GLCDD_Print(GLCDD_Font* font, GLCDD_Rect* area, char* str);
int GLCDD_Printf(GLCDD_Font* font, GLCDD_Rect* area, char* format, ...);
int GLCDD_CharWidth(const uint8_t* font, char c);
int GLCDD_StringWidth(GLCDD_Font* font, char* str);
int GLCDD_FontHeight(GLCDD_Font* font);
void GLCDD_Invert(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void GLCDD_ClearEx(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void GLCDD_XBMDraw(uint8_t* img, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void GLCDD_Draw();

#endif
