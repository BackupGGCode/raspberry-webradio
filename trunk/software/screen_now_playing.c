///////////////////////////////////////////////////////////////
//
//  Now playing
//
//  02.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#include "screen_now_playing.h"

int NowPlaying_ShowMenu;

StationInfo NowPlaying_CurStation;
int NowPlaying_IsFavorite = 0;
char NowPlaying_CSInfo[512];

// ---------------------------------------------------------------------------
void parseCurrentStation() {
  // check if station is in station list
  strcpy(NowPlaying_CSInfo, "\n");
  FILE* f = fopen(Settings_Get("files", "station_details"), "r");
  if(f != NULL) {
    ignore_result(fgets(NowPlaying_CSInfo, 512, f));
    fclose(f);
  }
  
  NowPlaying_IsFavorite = 0;
  
  // if info is available, check if station is in station list
  if(strlen(NowPlaying_CSInfo) > 3) {
    int i;
    // remove newline
    for(i = 0; i < strlen(NowPlaying_CSInfo); i++) {
      if(NowPlaying_CSInfo[i] == '\r' || NowPlaying_CSInfo[i] == '\n') {
	NowPlaying_CSInfo[i] = '\0';
	break;
      }
    }
    
    // save current station details
    ArrayList* cur_info = AList_Split(NowPlaying_CSInfo, "|");
    NowPlaying_CurStation.name = (char*)AList_Get(cur_info, 0);
    NowPlaying_CurStation.url = (char*)AList_Get(cur_info, 1);
    NowPlaying_CurStation.genre = (char*)AList_Get(cur_info, 2);
    
    // check if in favorite list
    ArrayList* stations = readStations();
    for(i = 0; i < AList_Length(stations); i++) {
      StationInfo* info = AList_Get(stations, i);
      if(strcmp(info->url, (char*)AList_Get(cur_info, 1)) == 0) {
	NowPlaying_IsFavorite = 1;
	break;
      }
    }  
    freeStations(stations);
    AList_Destroy(stations);
    AList_Destroy(cur_info);
    
  }
}

void createContextMenu() {
  // destroy old menu
  if(menu_now_playing != NULL)  Menu_Destroy(menu_now_playing);
  
  // create new menu
  menu_now_playing = Menu_Create(fnt_silkscreen_8, 64, 32); 
  Menu_SetAutoIO(menu_now_playing, 1);

  parseCurrentStation();
  
  Menu_AddItem(menu_now_playing, _lng(CANCEL));
  Menu_AddItem(menu_now_playing, _lng(SNOOZE));
  if(!NowPlaying_IsFavorite) Menu_AddItem(menu_now_playing, _lng(AS_FAVORITE));
}

// ---------------------------------------------------------------------------
void init_NowPlaying() {
  NowPlaying_ShowMenu = 0;
  menu_now_playing = NULL;
  
  createContextMenu();
}

// ---------------------------------------------------------------------------
void draw_NowPlaying() {
  char buffer[512];

  Screen_DrawBorder(_lng(NOW_PLAYING_TITLE));
  
  GLCDD_Rect r;
  
  FILE* f = fopen(SONG_FILE, "r");
  if(f == NULL) return;
  ignore_result(fgets(buffer, 512, f));
  fclose(f);
 
  // split songname - artist
  int i, found = 0;
  for(i = 0; i < strlen(buffer); i++) {
   if(buffer[i] == ' ' && buffer[i+1] == '-' && buffer[i+2] == ' ') {
     buffer[i+1] = 0;
     found = 1;
     break;
   }
  }
  if(!found) i = -3;
 
  r.x = 6;
  r.y = 14;
  r.w = SCREEN_W - 12;
  r.h = -1;
  if(strlen(&buffer[i+3]) > 1) GLCDD_Print(fnt_dejavu_9b, &r, &buffer[i+3]); else GLCDD_Print(fnt_dejavu_9b, &r, _lng(NO_TITLE));
  r.y = 29;
  if(strlen(buffer) > 1) GLCDD_Print(fnt_dejavu_9, &r, buffer); else GLCDD_Print(fnt_dejavu_9, &r, _lng(NO_ARTIST));
  
 
  // get station
  f = fopen(CURRENT_STATION_FILE, "r");
  if(f == NULL) return;
  ignore_result(fgets(buffer, 512, f));
  fclose(f);
 
  r.y = 44;
  if(strlen(buffer) <= 2) GLCDD_Print(fnt_dejavu_9, &r, _lng(NO_STATION)); else GLCDD_Print(fnt_dejavu_9, &r, buffer);
  
  // we play from usb and can therefore skip the song
  if(strncmp(buffer, _lng(USB), strlen(_lng(USB))) == 0) {
   GLCDD_RectRounded(SCREEN_W - 36, SCREEN_H - 14, 32, 10, 1);
   fnt_dejavu_9->color = 1;
   r.x = SCREEN_W - 29;
   r.y = SCREEN_H - 13;
   r.w = 28;
   r.h = -1;
   GLCDD_Print(fnt_dejavu_9, &r, _lng(NEXT));
   fnt_dejavu_9->color = 0;
   
   if(IO_GetButton(0)) {
      char cmd[64];
      sprintf(cmd, "%s &", Settings_Get("programs", "next_song"));
      ignore_result(system(cmd));
   }
  } else {
    // if no station is played, don't show menu
    if(strlen(buffer) <= 2) return; 
    
    // we play a station, enable context menu
    if(!NowPlaying_ShowMenu && IO_GetButton(0)) {
     createContextMenu();
     NowPlaying_ShowMenu = 1; 
    }
    
    if(NowPlaying_ShowMenu) Menu_Draw(menu_now_playing, 32, 16);
    
    int selection = Menu_IsChosen(menu_now_playing);
    if(selection != -1) {
      if(selection == 1) { 
	// snooze
	Screen_Goto(SCREEN_SNOOZE);
      }      
      if(selection == 2) { // add as favorite
	printf("add station '%s' as favorite\r\n", NowPlaying_CurStation.name);
	// add to favorite list
	ArrayList* stations = readStations();
	AList_Add(stations, &NowPlaying_CurStation);
	writeStations(stations);
	AList_Delete(stations, AList_Length(stations) - 1);
	freeStations(stations);
	AList_Destroy(stations);
      }
      
      NowPlaying_ShowMenu = 0; 
      Screen_ForceRedraw();
    }
  }
}


// ---------------------------------------------------------------------------
void exit_NowPlaying() {
  Menu_Destroy(menu_now_playing);
}

