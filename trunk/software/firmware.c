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

//#define SONG_FILE "song.txt"
//#define CURRENT_STATION_FILE "/var/www/m/current_station.txt"
//#define STATIONS_FILE "/var/www/m/stations.txt"
//#define STATIONS_FILE "stations.txt"
//#define USB_PATH "/media/3062-6ECE/"

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
#include "images/info.h"
#include "images/playing.h"
#include "images/station.h"
#include "images/usb.h"
#include "images/up.h"
#include "images/song.h"

// fonts
#include "fonts/fnt_dejavu_9.h"
#include "fonts/fnt_dejavu_9_b.h"
#include "fonts/fnt_silkscreen_8.h"

GLCDD_Font* fnt_dejavu_9;
GLCDD_Font* fnt_dejavu_9b;
GLCDD_Font* fnt_silkscreen_8;

// screens
#include "screen_main.h"
#include "screen_now_playing.h"
#include "screen_info.h"
#include "screen_stations.h"
#include "screen_usb.h"


GLCDD_Font* createFont(const uint8_t* name) {
	GLCDD_Font* font = (GLCDD_Font*)malloc(sizeof(GLCDD_Font));
	font->name = (uint8_t*)name;
	font->color = 0;
}

int cleanup(void) {
 printf("Goodbye!\r\n");
 exit(0);
}

int main(int argc, char* argv[]) {
 signal(SIGINT, (sig_t)cleanup);

 IO_SetSimulate(1);
 GLCDD_SetSimulate(1);
  
 GLCDD_Init();
 
 // load settings file
 int settings = 0;
 if(argc == 2) {
   settings = Settings_Load(argv[1]);
 } 
 if(!settings) {
   if(!Settings_Load("default.conf")) {
      printf("Error loading settings file!\r\n");
      return 0;
   }
 }
 
 // load settings
 FW_VERSION = Settings_Get("gui", "version");
 SONG_FILE = Settings_Get("files", "song");
 CURRENT_STATION_FILE = Settings_Get("files", "current_station");
 STATIONS_FILE = Settings_Get("files", "stations");
 USB_PATH = Settings_Get("files", "usb");
 
 Language_Init(Settings_Get("gui", "language"));
 Screen_Init(silkscreen_8);
 
 Screen screen = SCREEN_MAIN;
 
 // create fonts
 fnt_dejavu_9 = createFont(dejavu_9);
 fnt_dejavu_9b = createFont(dejavu_9_b);
 fnt_silkscreen_8 = createFont(silkscreen_8);
 
 // register screens
 Screen_Add(SCREEN_MAIN, init_Main, draw_Main, exit_Main);
 Screen_Add(SCREEN_NOW_PLAYING, NULL, draw_NowPlaying, NULL);
 Screen_Add(SCREEN_STATIONS, init_Stations, draw_Stations, exit_Stations);
 Screen_Add(SCREEN_INFO, NULL, draw_Info, NULL);
 Screen_Add(SCREEN_USB, init_USB, draw_USB, NULL);
 Screen_SetRefreshTimeout(SCREEN_INFO, 2);
 
 Screen_Goto(SCREEN_MAIN);
 
 while(1) {
 
  IO_Get();
  if(IO_HasChanged()) {
    Screen_ForceRedraw();
  }
  
  Screen_Draw();
 
  screen = Screen_GetActive();
  if(screen == SCREEN_MAIN) {
	int selection = Menu_IsChosen(menu_main);
	if(selection == 0) {
		// goto now playing screen
		Screen_Goto(SCREEN_NOW_PLAYING);
	} else if(selection == 1) {
	    // goto stations screen
		Screen_Goto(SCREEN_STATIONS);	
	} else if(selection == 2) {
	    // goto usb screen
		Screen_Goto(SCREEN_USB);
	} else if(selection == 3) {
	    // goto info screen
	    Screen_Goto(SCREEN_INFO);
	}
  }
  else if(screen == SCREEN_INFO) {
      if(IO_GetButton(0)) {
		Screen_Goto(SCREEN_MAIN);
      }
  }
  else if(screen == SCREEN_STATIONS) {
	int selection = Menu_IsChosen(menu_stations);
	if(selection != -1) {
		// start station
		playStation(Menu_GetItemTag(menu_stations, selection));
	}
	
	  
	if(IO_GetButtonLong(3)) {
	  asFavorite(1);
	}
	
	if(IO_GetButtonLong(4)) {
	  asFavorite(2);
	}
	
	if(IO_GetButtonLong(5)) {
	  asFavorite(3);
	}
	
	if(IO_GetButtonLong(6)) {
	  asFavorite(4);
	}
  }

  // home button
  if(IO_GetButton(1)) Screen_Goto(SCREEN_MAIN);
  
  // play/stop button
  if(IO_GetButton(2)) {
    if(screen != SCREEN_USB) {
      // stop music
      char cmd[128];
      sprintf(cmd, "%s &", Settings_Get("programs", "stop"));
      system(cmd);
    } else {
      // play folder
      int i;
      FILE* f = fopen("/tmp/playlist1.m3u", "w");
      for(i = 0; i < Menu_GetItems(menu_usb); i++) {
	// a file, add it to the list
	if((int)Menu_GetItemTag(menu_usb, i) == 0) {
	  fprintf(f, "%s/%s\r\n", usb_root, Menu_GetItemText(menu_usb, i));
	}
      }
      fclose(f);
      system("shuf /tmp/playlist1.m3u > /tmp/playlist.m3u");
      
      playUSB("/tmp/playlist.m3u");
    }
  }
  
  // (1) button
  if(IO_GetButton(3)) {
    playFavorite(1);
  }
  // (2) button
  if(IO_GetButton(4)) {
    playFavorite(2);
  }
  // (3) button
  if(IO_GetButton(5)) {
    playFavorite(3);
  }
  // (4) button
  if(IO_GetButton(6)) {
    playFavorite(4);
  }
  
  usleep(500);
 
 }
 
 // really no need to cleanup, as firmware runs as long as device is powered
 // plus, the operating system frees all resources anyway
 
}
