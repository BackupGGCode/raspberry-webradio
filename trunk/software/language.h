///////////////////////////////////////////////////////////
//
// Language File Manager
//
// 20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	NOW_PLAYING_TITLE,
	DATE_TIME_FORMAT,
	NOW_PLAYING,
	STATIONS,
	USB,
	INFO,
	INFORMATION_TITLE,
	IP,
	VERSION,
	FREE,
	OK,
	NOT_AVAILABLE,
	NOT_CONNECTED,
	STATIONS_TITLE,
	USB_TITLE,
	NEXT,
	NO_TITLE,
	NO_ARTIST,
	ERROR_LOADING_SETTINGS,
	NO_STATION,
	GOODBYE,
	SETTINGS_TITLE,
	SETTINGS,
	WIFI,
	WIFI_SCAN_TITLE,
	WIFI_ENTER_PWD,
	WIFI_CONNECTING,
	PLEASE_WAIT,
	SCANNING,
	LANGUAGE,
	LANGUAGE_TITLE,
	VOLUME,
	VOLUME_TITLE,
	SHOUTCAST_TITLE,
	SHOUTCAST,
	SHOUTCAST_TOP,
	SHOUTCAST_RANDOM,
	GENRE,
	GENRE_BACK,
	PLAY,
	AS_FAVORITE,
	CANCEL,
	MANAGE_STATIONS,
	STATIONS_MANAGE_TITLE,
	DELETE,
	MOVE,
	SNOOZE,
	LOADING,
	MINUTES,
	SEARCH,
	RESTART,
	SHUTDOWN,
	BACKLIGHT_TIMEOUT,
	SECONDS
} Language_String;

int Language_Init(char* lang);
int Language_Cleanup();
int Language_Set(char* lang);
char* Language_Get();
char* Language_Translate(Language_String str);

#define _lng(str) Language_Translate(str)

#endif

