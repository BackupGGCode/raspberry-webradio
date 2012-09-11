///////////////////////////////////////////////////////////
//
// Webradio Firmware
//
// 20.08.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#ifndef _FIRMWARE_H_
#define _FIRMWARE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>

char* FW_VERSION;
char* SONG_FILE;
char* CURRENT_STATION_FILE;
char* STATIONS_FILE;
char* USB_PATH;

#include "macros.h"
#include "lcd.h"
#include "io.h"
#include "settings.h"
#include "language.h"
#include "arraylist.h"
#include "menu.h"
#include "ip.h"
#include "disk.h"
#include "screens.h"

// images
#include "images.h"

// fonts
#include "fonts.h"

GLCDD_Font* fnt_dejavu_9;
GLCDD_Font* fnt_dejavu_9b;
GLCDD_Font* fnt_silkscreen_8;

// screens
#include "screen_main.h"
#include "screen_now_playing.h"
#include "screen_info.h"
#include "screen_stations.h"
#include "screen_usb.h"
#include "screen_shutdown.h"

#endif
