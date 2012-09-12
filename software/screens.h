///////////////////////////////////////////////////////////
//
// Screen Manager
//
// 21.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _SCREEN_MAN_H_
#define _SCREEN_MAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lcd.h"
#include "menu.h"

#define SCREEN_W 128
#define SCREEN_H 64

typedef void (*ScreenFunc)(void);

typedef enum {
	SCREEN_MAIN,
	SCREEN_NOW_PLAYING,
	SCREEN_INFO,
	SCREEN_STATIONS,
	SCREEN_USB,
	SCREEN_SHUTDOWN,
	SCREEN_SETTINGS,
	SCREEN_WIFI_SCAN,
	SCREEN_WIFI_AUTH,
	SCREEN_WIFI_CONNECT,
	SCREEN_LANGUAGE,
	SCREEN_VOLUME,
	SCREEN_END
} Screen;

typedef struct {
	ScreenFunc init_fnc;
	ScreenFunc draw_fnc;
	ScreenFunc exit_fnc;
	long last_update;
	int refresh_timeout;
} ScreenInfo;


void Screen_Init(const uint8_t* font_border);
void Screen_DrawBorder(char* title);
void Screen_Add(Screen id, ScreenFunc init_fnc, ScreenFunc draw_fnc, ScreenFunc exit_fnc);
void Screen_Goto(Screen id);
Screen Screen_GetActive();
void Screen_Draw();
void Screen_ForceRedraw();
void Screen_SetRefreshTimeout(Screen id, int refresh_timeout);
void Screen_Cleanup();

#endif
