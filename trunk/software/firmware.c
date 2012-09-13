///////////////////////////////////////////////////////////
//
// Webradio Firmware
//
// 20.08.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#include "firmware.h"


// ---------------------------------------------------------------------------
GLCDD_Font* createFont(const uint8_t* name) {
	GLCDD_Font* font = (GLCDD_Font*)malloc(sizeof(GLCDD_Font));
	font->name = (uint8_t*)name;
	font->color = 0;
	return font;
}

// ---------------------------------------------------------------------------
int cleanup(void) {
 printf("Goodbye!\r\n");
 exit(0);
}

// ---------------------------------------------------------------------------
void resetMetaInfo() {
 // reset current song
 FILE* f = fopen(Settings_Get("files", "song"), "w");
 fprintf(f, " - ");
 fclose(f);
 f = fopen(Settings_Get("files", "current_station"), "w");
 fprintf(f, "\n");
 fclose(f);
 f = fopen(Settings_Get("files", "station_details"), "w");
 fprintf(f, "\n");
 fclose(f);
}

// ---------------------------------------------------------------------------
void stopMusic() {
  // stop music
  char cmd[128];
  sprintf(cmd, "%s &", Settings_Get("programs", "stop"));
  ignore_result(system(cmd));
  resetMetaInfo(); 
}

// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
 signal(SIGINT, (sig_t)cleanup);

 // load settings file
 char* settings_file = NULL;
 int settings = 0;
 if(argc == 2) {
   settings_file = argv[1];
   settings = Settings_Load(argv[1]);
 } 
 if(!settings) {
   settings_file = "default.conf";
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
 
 // check for running instance
 char* single_instance_cmd = Settings_Get("programs", "check_running");
 ignore_result(system(single_instance_cmd));
 FILE *running = fopen("fw.running", "r");
 if(running == NULL) {
   printf("Cannot check if firmware is running, aborting.\r\n");
   exit(0);
 }
 if(fgetc(running) >= '2') {
   fclose(running);
   printf("Firmware is already running!\r\nExiting...\r\n");
   exit(0);
 } else {
   fclose(running);
 }
 

 if(strcmp(Settings_Get("hardware", "io"), "sim") == 0) IO_SetSimulate(1);
 if(strcmp(Settings_Get("hardware", "lcd"), "sim") == 0) GLCDD_SetSimulate(1);
 else if(strcmp(Settings_Get("hardware", "lcd"), "bmp") == 0) GLCDD_SetSimulate(2);
  
 GLCDD_Init();
 IO_Init();

 Language_Init(Settings_Get("gui", "language"));
 Screen_Init(silkscreen_8);
 
 Screen screen = SCREEN_MAIN;
 
 // create fonts
 fnt_dejavu_9 = createFont(dejavu_9);
 fnt_dejavu_9b = createFont(dejavu_9_b);
 fnt_silkscreen_8 = createFont(silkscreen_8);
 
 // register screens
 Screen_Add(SCREEN_MAIN, init_Main, draw_Main, exit_Main);
 Screen_Add(SCREEN_NOW_PLAYING, init_NowPlaying, draw_NowPlaying, exit_NowPlaying);
 Screen_Add(SCREEN_STATIONS, init_Stations, draw_Stations, exit_Stations);
 Screen_Add(SCREEN_INFO, NULL, draw_Info, NULL);
 Screen_Add(SCREEN_USB, init_USB, draw_USB, NULL);
 Screen_Add(SCREEN_SHUTDOWN, init_Shutdown, draw_Shutdown, NULL);
 Screen_Add(SCREEN_SETTINGS, init_Settings, draw_Settings, exit_Settings);
 Screen_Add(SCREEN_WIFI_SCAN, init_WifiScan, draw_WifiScan, exit_WifiScan);
 Screen_Add(SCREEN_WIFI_AUTH, init_WifiAuth, draw_WifiAuth, NULL);
 Screen_Add(SCREEN_WIFI_CONNECT, init_WifiConnect, draw_WifiConnect, NULL);
 Screen_Add(SCREEN_LANGUAGE, init_Language, draw_Language, exit_Language);
 Screen_Add(SCREEN_VOLUME, init_Volume, draw_Volume, NULL);
 Screen_Add(SCREEN_SHOUTCAST, init_Shoutcast, draw_Shoutcast, exit_Shoutcast);
 Screen_Add(SCREEN_SHOUTCAST_LIST, init_ShoutcastList, draw_ShoutcastList, exit_ShoutcastList);
 Screen_Add(SCREEN_SHOUTCAST_GENRE, init_ShoutcastGenre, draw_ShoutcastGenre, exit_ShoutcastGenre);
 Screen_Add(SCREEN_MANAGE_STATION, init_ManageStation, draw_ManageStation, exit_ManageStation);
 Screen_Add(SCREEN_SNOOZE, init_Snooze, draw_Snooze, exit_Snooze);
 Screen_SetRefreshTimeout(SCREEN_INFO, 2);
 Screen_SetRefreshTimeout(SCREEN_MAIN, 10);
 Screen_SetRefreshTimeout(SCREEN_NOW_PLAYING, 1);
 Screen_SetRefreshTimeout(SCREEN_STATIONS, 10);
 Screen_SetRefreshTimeout(SCREEN_USB, 1);
 Screen_SetRefreshTimeout(SCREEN_SHUTDOWN, 10);
 Screen_SetRefreshTimeout(SCREEN_WIFI_SCAN, 1); 
 Screen_SetRefreshTimeout(SCREEN_WIFI_AUTH, 10);
 Screen_SetRefreshTimeout(SCREEN_WIFI_CONNECT, 1);
 Screen_SetRefreshTimeout(SCREEN_LANGUAGE, 10);
 Screen_SetRefreshTimeout(SCREEN_VOLUME, 10);
 Screen_SetRefreshTimeout(SCREEN_SHOUTCAST, 10);
 Screen_SetRefreshTimeout(SCREEN_SHOUTCAST_LIST, 10);
 Screen_SetRefreshTimeout(SCREEN_SHOUTCAST_GENRE, 10);
 Screen_SetRefreshTimeout(SCREEN_MANAGE_STATION, 10);
 Screen_SetRefreshTimeout(SCREEN_SNOOZE, 10);
 Screen_ShowLoadingScreen(SCREEN_USB, 1);
 Screen_ShowLoadingScreen(SCREEN_VOLUME, 1);
 Screen_ShowLoadingScreen(SCREEN_SHOUTCAST_LIST, 1);
 Screen_ShowLoadingScreen(SCREEN_SHOUTCAST_GENRE, 1);
 
  
 // reset song info
 resetMetaInfo();
 
 // start ui
 Screen_Goto(SCREEN_MAIN);
 
 // background light
 GLCDD_BacklightTimeout(atoi(Settings_Get("hardware", "timeout")));
 int keep_light_when_playing = 0;
 if(strcmp(Settings_Get("gui", "keep_light_when_playing"), "true") == 0) keep_light_when_playing = 1;
 GLCDD_BacklightReset();
 
 while(1) {
 
  IO_Get();
  if(IO_HasChanged()) {
    Screen_ForceRedraw();
    GLCDD_BacklightReset();
  }
  
  screen = Screen_GetActive();
  
  // ---------------------------------------------------------------------------
  if(screen == SCREEN_MAIN) {
    int selection = Menu_IsChosen(menu_main);
    if(selection == 0) {
      // goto now playing screen
      Screen_Goto(SCREEN_NOW_PLAYING);
    } else if(selection == 1) {
      // goto stations screen
      Screen_Goto(SCREEN_STATIONS);	
    } else if(selection == 2) {
      // goto shoutcast browser
      Screen_Goto(SCREEN_SHOUTCAST);
    } else if(selection == 3) {
      // goto usb screen
      Screen_Goto(SCREEN_USB);
    } else if(selection == 4) {
      // goto info screen
      Screen_Goto(SCREEN_SETTINGS);
    }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_INFO) {
    if(IO_GetButton(0)) {
      Screen_Goto(SCREEN_SETTINGS);
    }
  }
  // ---------------------------------------------------------------------------
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
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_NOW_PLAYING) {
    // if at now playing screen, backlight is always on
    if(keep_light_when_playing) GLCDD_BacklightReset(); 
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_SETTINGS) {
      int selection = Menu_IsChosen(menu_settings);
      if(selection == 0) {
	// goto info screen
	Screen_Goto(SCREEN_INFO);
      } else if(selection == 1) {
	// goto wifi scanning screen
	Screen_Goto(SCREEN_WIFI_SCAN);
      } else if(selection == 2) {
	// goto language selection
	Screen_Goto(SCREEN_LANGUAGE);
      } else if(selection == 3) {
	// goto volume adjustment
	Screen_Goto(SCREEN_VOLUME);
      } else if(selection == 4) {
	// goto station manager
	Screen_Goto(SCREEN_MANAGE_STATION);
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_WIFI_SCAN) {
      int selection = Menu_IsChosen(menu_wifi_scan);
      if(selection != -1) {
	// save ssid
	Settings_Add("wifi", "ssid", Menu_GetItemText(menu_wifi_scan, selection));
	// go to authentification
	Screen_Goto(SCREEN_WIFI_AUTH);
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_WIFI_AUTH) {
      if(Keyboard_IsConfirmed()) {
	// connect to wifi network
	Settings_Add("wifi", "pwd", Keyboard_GetText());
	printf("Connecting to '%s' with key '%s'\r\n", Settings_Get("wifi", "ssid"), Settings_Get("wifi", "pwd"));
	Screen_Goto(SCREEN_WIFI_CONNECT);
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_LANGUAGE) {
      // change language
      int selection = Menu_IsChosen(menu_language);
      if(selection != -1) {
	char disp[64];
	int i;
	strcpy(disp, Menu_GetItemText(menu_language, selection));
	for(i = strlen(disp) - 1; i >= 0; i--) {
	  if(disp[i] == ')') disp[i] = 0;
	  if(disp[i] == '(') {
	   i++;
	   break;
	  }
	}
	Settings_Add("gui", "language", &disp[i]);
	Language_Cleanup();
	Language_Init(&disp[i]);
	Settings_Save(settings_file);
	Screen_Goto(SCREEN_SETTINGS);
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_VOLUME) {
      if(IO_GetButton(0)) {
	// go back to settings
	Screen_Goto(SCREEN_SETTINGS);
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_SHOUTCAST) {
      int selection = Menu_IsChosen(menu_shoutcast);
      if(selection == 0) {
	// go to shoutcast top stations
	setShoutcastListUrl("http://api.shoutcast.com/legacy/Top500?k=so1N15vhCB78Z6k4&limit=25&mt=audio/mpeg");
	setStationsParentGenre("X");
	setCurrentGenre(" ");
	Screen_Goto(SCREEN_SHOUTCAST_LIST);
      } 
      else if(selection == 1) { 
	// random station
	setShoutcastListUrl("http://api.shoutcast.com/station/randomstations?k=so1N15vhCB78Z6k4&f=xml&mt=audio/mpeg");
	setStationsParentGenre("X");
	setCurrentGenre(" ");
	Screen_Goto(SCREEN_SHOUTCAST_LIST);
      }
      else if(selection == 2) {
	// go to genre list
	setShoutcastGenreParent("0");
	Screen_Goto(SCREEN_SHOUTCAST_GENRE);
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_SHOUTCAST_GENRE) {
      int selection = Menu_IsChosen(menu_genres);
      if(selection != -1) {
	if(selection == 0) { // back
	  if(strcmp(getShoutcastGenreParent(), "0") == 0) { // main genres
	    Screen_Goto(SCREEN_SHOUTCAST);
	  } else { // sub genre -> go back to main genres
	    setShoutcastGenreParent("0");
	    Screen_Goto(SCREEN_SHOUTCAST_GENRE);
	  }
	} else { // list stations or sub genre
	  ShoutcastGenre* info = getChosenGenre((int)Menu_GetItemTag(menu_genres, selection));
	  // show sub genres
	  if(info->has_children) {
	    setStationsParentGenre(info->id);
	    setShoutcastGenreParent(info->id);
	    Screen_Goto(SCREEN_SHOUTCAST_GENRE);
	  } else { // show stations
	    char buffer[128];
	    sprintf(buffer, "http://api.shoutcast.com/legacy/genresearch?k=so1N15vhCB78Z6k4&f=xml&genre=%s", info->name);
	    printf("Stations: %s\r\n", buffer);
	    setShoutcastListUrl(buffer);
	    setCurrentGenre(info->name);
	    Screen_Goto(SCREEN_SHOUTCAST_LIST);
	  }
	}
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_SHOUTCAST_LIST) {
      if(Menu_GetAutoIO(menu_station_list)) {
	int selection = Menu_IsChosen(menu_station_list);
	if(selection != -1) {
	  if(selection == 0) { // back
	      if(strcmp(getStationsParentGenre(), "X") == 0) { // go to shoutcast menu
		Screen_Goto(SCREEN_SHOUTCAST); 
	      } else { // go to genre list
		setShoutcastGenreParent(getStationsParentGenre());
		Screen_Goto(SCREEN_SHOUTCAST_GENRE);
	      }
	  } else { // show play/as_favorite/cancel menu
	      Menu_SetAutoIO(menu_station_list, 0);
	      Screen_ForceRedraw();
	  }
	}
      } else {
	int selection = Menu_IsChosen(menu_play_fav);
	if(selection != -1) {
	  Menu_SetAutoIO(menu_station_list, 1);
	  Screen_ForceRedraw();
	  
	  if(selection == 0) { // play
	      ShoutcastStation* info = getChosenStation((int)Menu_GetItemTag(menu_station_list, Menu_GetSelectedItem(menu_station_list)));
	      StationInfo* station = parseShoutcastList(info);
	      station->genre = getCurrentGenre();
	      playStation(station);
	      printf("play %s @ 'http://yp.shoutcast.com/sbin/tunein-station.pls?id=%s'\r\n", info->name, info->id);
	      free(station->url);
	      free(station);
	  }
	  else if(selection == 1) { // add to station list
	      ShoutcastStation* sc_stat = getChosenStation((int)Menu_GetItemTag(menu_station_list, Menu_GetSelectedItem(menu_station_list)));
	      StationInfo* stat = parseShoutcastList(sc_stat);
	      
	      StationInfo* info = (StationInfo*)malloc(sizeof(StationInfo));
	      info->tag = '\0';
	      info->name = stat->name;
	      info->url = stat->url;
	      info->genre = getCurrentGenre();
	      
	      ArrayList* stations = readStations();
	      AList_Add(stations, info);
	      writeStations(stations);
	      AList_Delete(stations, AList_Length(stations) - 1);
	      freeStations(stations);
	      AList_Destroy(stations);
	      free(stat->url);
	      free(stat);
	      free(info);
	  }
	  else if(selection == 2) { // cancel
	    // do nothing
	  } 
	}
      }
  }
  // ---------------------------------------------------------------------------
  else if(screen == SCREEN_MANAGE_STATION) {
      if(Menu_GetAutoIO(menu_m_station)) { // handle station list menu
	int selection = Menu_IsChosen(menu_m_station);
	if(selection != -1) { // show menu
	  Menu_SetAutoIO(menu_m_station, 0);
	  Menu_ScrollTo(menu_m_menu, 0);
	  Screen_ForceRedraw();
	}
      } else {
	if(Menu_GetAutoIO(menu_m_menu)) { // handle station menu
	    int selection = Menu_IsChosen(menu_m_menu);
	    
	    if(selection == 0) {
	      // do nothing (cancel)
	    }
	    else if(selection == 1) {
	      // move -> handled below
	    }
	    else if(selection == 2) {
	      printf("Delete station\r\n");  
	      deleteStation(Menu_GetSelectedItem(menu_m_station));
	      Screen_Goto(SCREEN_MANAGE_STATION);
	    }
	    
	    // close menu
	    if(selection != -1) {
	      if(selection == 1) { // move -> disable all menus
		Menu_SetAutoIO(menu_m_menu, 0);
		Menu_SetTitleTag(menu_m_station, Menu_GetSelectedItem(menu_m_station), '*');
		Screen_ForceRedraw();
	      } else {
		Menu_SetAutoIO(menu_m_station, 1);
		Screen_ForceRedraw();
	      }
	    }
	}
	
	else { // moving stations
	  if(IO_GetButton(0)) { // we are done
	    Menu_SetTitleTag(menu_m_station, Menu_GetSelectedItem(menu_m_station), 0);
	    ArrayList* new_stations = AList_Create();
	    int i;
	    for(i = 0; i < Menu_GetItems(menu_m_station); i++) {
	      StationInfo* info = Menu_GetItemTag(menu_m_station, i);
	      AList_Add(new_stations, info);
	    }
	    writeStations(new_stations);
	    AList_Destroy(new_stations);
	    
	    Menu_SetAutoIO(menu_m_menu, 1);
	    Menu_SetAutoIO(menu_m_station, 1);
	    Screen_ForceRedraw();
	  }
	  else {
	   int8_t rotary = IO_GetRotaryValue();
	   if(rotary != 0) {
	      int index = Menu_GetSelectedItem(menu_m_station);
	      int new_index = index + ((rotary > 0) ? 1 : -1);
	      Menu_SwapItems(menu_m_station, index, new_index); 
	      Menu_Scroll(menu_m_station, (rotary > 0) ? 1 : -1);
	   }
	  }
	  
	}
	
      }
  }
  // ---------------------------------------------------------------------------

  // home button
  if(IO_GetButton(1)) Screen_Goto(SCREEN_MAIN);
  
  // play/stop button
  if(IO_GetButton(2)) {
    if(screen != SCREEN_USB) {
      // stop music
      stopMusic();
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
  
  // end of snooze time
  if(checkSnoozeStop()) {
   // stop the music
   stopMusic();
   // go to main screen
   Screen_Goto(SCREEN_MAIN);
  }
  
  // (1) button
  if(IO_GetButton(3)) playFavorite(1);
  // (2) button
  if(IO_GetButton(4)) playFavorite(2);
  // (3) button
  if(IO_GetButton(5)) playFavorite(3);
  // (4) button
  if(IO_GetButton(6)) playFavorite(4);
  
  // shutdown (stop button long)
  if(IO_GetButtonLong(2)) {
    Screen_Goto(SCREEN_SHUTDOWN); 
  }
  
  Screen_Draw();

  GLCDD_BacklightUpdate();
  
  usleep(100);
//sleep(1);
 
 }
 
 // really no need to cleanup, as firmware runs as long as device is powered
 // plus, the operating system frees all resources anyway
 
}
