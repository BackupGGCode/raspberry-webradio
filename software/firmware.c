///////////////////////////////////////////////////////////
//
// Webradio Firmware
//
// 20.08.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#include "firmware.h"


GLCDD_Font* createFont(const uint8_t* name) {
	GLCDD_Font* font = (GLCDD_Font*)malloc(sizeof(GLCDD_Font));
	font->name = (uint8_t*)name;
	font->color = 0;
	return font;
}

int cleanup(void) {
 printf("Goodbye!\r\n");
 exit(0);
}

int main(int argc, char* argv[]) {
 signal(SIGINT, (sig_t)cleanup);

 // load settings file
 int settings = 0;
 if(argc == 2) {
   settings = Settings_Load(argv[1]);
 } 
 if(!settings) {
   if(!Settings_Load("default.conf")) {
      printf("%s\r\n", _lng(ERROR_LOADING_SETTINGS));
      return 0;
   }
 }

 // load settings
 FW_VERSION = Settings_Get("gui", "version");
 SONG_FILE = Settings_Get("files", "song");
 CURRENT_STATION_FILE = Settings_Get("files", "current_station");
 STATIONS_FILE = Settings_Get("files", "stations");
 USB_PATH = Settings_Get("files", "usb");
 

 if(strcmp(Settings_Get("hardware", "io"), "sim") == 0) IO_SetSimulate(1);
 if(strcmp(Settings_Get("hardware", "lcd"), "sim") == 0) GLCDD_SetSimulate(1);
 else if(strcmp(Settings_Get("hardware", "lcd"), "bmp") == 0) GLCDD_SetSimulate(2);
  
 GLCDD_Init();

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
 Screen_SetRefreshTimeout(SCREEN_MAIN, 10);
 Screen_SetRefreshTimeout(SCREEN_NOW_PLAYING, 1);
 Screen_SetRefreshTimeout(SCREEN_STATIONS, 10);
 Screen_SetRefreshTimeout(SCREEN_USB, 1);
 
 // reset current song
 FILE* f = fopen(Settings_Get("files", "song"), "w");
 fprintf(f, " - ");
 fclose(f);
 f = fopen(Settings_Get("files", "current_station"), "w");
 fprintf(f, "%s\r\n", _lng(NO_STATION));
 fclose(f);
 
 // start ui
 Screen_Goto(SCREEN_MAIN);
 
 while(1) {
 
  IO_Get();
  if(IO_HasChanged()) Screen_ForceRedraw();
  
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
    
    // add station as favorite
    if(IO_GetButtonLong(3)) asFavorite(1);
    if(IO_GetButtonLong(4)) asFavorite(2);
    if(IO_GetButtonLong(5)) asFavorite(3);
    if(IO_GetButtonLong(6)) asFavorite(4);
  }

  // home button
  if(IO_GetButton(1)) Screen_Goto(SCREEN_MAIN);
  
  // play/stop button
  if(IO_GetButton(2)) {
    if(screen != SCREEN_USB) {
      // stop music
      char cmd[128];
      sprintf(cmd, "%s &", Settings_Get("programs", "stop"));
      ignore_result(system(cmd));
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
      char cmd[128];
      sprintf(cmd, "%s /tmp/playlist1.m3u > /tmp/playlist.m3u", Settings_Get("programs", "shuffle"));
      ignore_result(system(cmd));
      
      playUSB("/tmp/playlist.m3u");
    }
  }
  
  // (1) button
  if(IO_GetButton(3)) playFavorite(1);
  // (2) button
  if(IO_GetButton(4)) playFavorite(2);
  // (3) button
  if(IO_GetButton(5)) playFavorite(3);
  // (4) button
  if(IO_GetButton(6)) playFavorite(4);
  
  Screen_Draw();
  usleep(200);
 
 }
 
 // really no need to cleanup, as firmware runs as long as device is powered
 // plus, the operating system frees all resources anyway
 
}
