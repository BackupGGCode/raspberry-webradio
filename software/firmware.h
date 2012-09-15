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
#include <sys/time.h>


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
#include "keyboard.h"
#include "download.h"

// images
#include "images.h"

// fonts
#include "fonts.h"

GLCDD_Font* fnt_dejavu_9;
GLCDD_Font* fnt_dejavu_9b;
GLCDD_Font* fnt_silkscreen_8;


typedef struct {
  char* name;
  char* genre;
  char* url;
  char tag;
} StationInfo;

// screens
#include "screen_main.h"
#include "screen_now_playing.h"
#include "screen_info.h"
#include "screen_stations.h"
#include "screen_usb.h"
#include "screen_shutdown.h"
#include "screen_settings.h"
#include "screen_wifi_scan.h"
#include "screen_wifi_auth.h"
#include "screen_wifi_connect.h"
#include "screen_language.h"
#include "screen_volume.h"
#include "screen_shoutcast.h"
#include "screen_shoutcast_list.h"
#include "screen_shoutcast_genre.h"
#include "screen_shoutcast_search.h"
#include "screen_manage_station.h"
#include "screen_snooze.h"
#include "screen_timeout.h"


#endif
