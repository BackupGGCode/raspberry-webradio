///////////////////////////////////////////////////////////////
//
//  Station list
//
//  02.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#include "screen_stations.h"

Menu_Info* menu_stations = NULL;

// ---------------------------------------------------------------------------
void playStation(StationInfo* station) {
  char buffer[512];
  sprintf(buffer, "%s \"%s\" &", Settings_Get("programs", "stream"), station->url);
  ignore_result(system(buffer));
  FILE* f = fopen(Settings_Get("files", "current_station"), "w");
  fprintf(f, "%s\r\n", station->name);
  fclose(f);
}

// ---------------------------------------------------------------------------
ArrayList* readStations() {
  char buffer[256];
  char* ptr;
  
  FILE* f = fopen(STATIONS_FILE, "r");
  if(f == NULL) return NULL;

  ArrayList* stations = AList_Create();
  
  while(fgets(buffer, 256, f) != NULL) {
      if(strlen(buffer) < 4) continue;
      
      StationInfo* info = (StationInfo*)malloc(sizeof(StationInfo));
      ptr = strtok(buffer, "|");
      if(ptr == NULL) continue;
      info->name = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
      strcpy(info->name, ptr);
      ptr = strtok(NULL, "|");
      if(ptr == NULL) continue;      
      info->url = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
      strcpy(info->url, ptr);
      ptr = strtok(NULL, "|");
      if(ptr != NULL) {
        info->genre = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
        strcpy(info->genre, ptr);
      } else {
	info->genre = (char*)malloc(sizeof(char));
	info->genre[0] = '\0';
      }
      ptr = strtok(NULL, "|");
      if(ptr == NULL) continue;      
      info->tag = '\0';
      if(ptr != NULL) info->tag = ptr[0];
      
      AList_Add(stations, info);
  }
  fclose(f);
  
  return stations;
}

// ---------------------------------------------------------------------------
void writeStations(ArrayList* stations) {
 FILE* f = fopen(STATIONS_FILE, "w");
 if(f == NULL) return;
 
 int i;
 for(i = 0; i < AList_Length(stations); i++) {
   StationInfo* info = AList_Get(stations, i);
   fprintf(f, "%s|%s|%s|%c\r\n", info->name, info->url, info->genre, (info->tag == '\0') ? '0' : info->tag);
 }
 
 fclose(f);
}

// ---------------------------------------------------------------------------
void freeStations(ArrayList* stations) {
 int i;
 for(i = 0; i < AList_Length(stations); i++) {
  StationInfo* info = AList_Get(stations, i);
  free(info->name);
  free(info->url);
  free(info->genre);
  free(info);
 }
}


// ---------------------------------------------------------------------------
void deleteStation(int index) {
  int i;
  ArrayList* stations = readStations();
  
  // delete station
  StationInfo* del = AList_Get(stations, index);
  free(del->name);
  free(del->url);
  free(del->genre);
  free(del);
  // shift other stations
  for(i = index; i < AList_Length(stations) - 1; i++) {
    AList_Set(stations, i, AList_Get(stations, i + 1)); 
  }
  AList_Delete(stations, AList_Length(stations) - 1);
  
  writeStations(stations);
  freeStations(stations);
  AList_Destroy(stations);
}


// ---------------------------------------------------------------------------
void asFavorite(int fav_id) {
  int sel_id = Menu_GetSelectedItem(menu_stations);
  ArrayList* stations = readStations();
  int i;

  char tag = fav_id + '0';
  Menu_SetTitleTag(menu_stations, sel_id, tag);
  
  for(i = 0; i < AList_Length(stations); i++) {
   StationInfo* info = AList_Get(stations, i);
   if(info->tag == tag) {
     info->tag = '\0';
     Menu_SetTitleTag(menu_stations, i, '\0');
   }
   if(i == sel_id) info->tag = tag;
   
  }
  
  writeStations(stations);
  
  freeStations(stations);
  AList_Destroy(stations);
}


// ---------------------------------------------------------------------------
void playFavorite(int id) {
 ArrayList* stations = readStations();
 
 int i;
 for(i = 0; i < AList_Length(stations); i++) {
   StationInfo* info = (StationInfo*)AList_Get(stations, i);
   if(info->tag == id + '0') {
    playStation(info);
    break;
   }
 }
 
 freeStations(stations);
 
 AList_Destroy(stations);
}

// ---------------------------------------------------------------------------
void init_Stations() {
	// get station list
	menu_stations = Menu_Create(fnt_silkscreen_8, 126, 55);
	Menu_SetAutoIO(menu_stations, 1);
	Menu_SetBorder(menu_stations, BORDER_NONE);
	
	ArrayList* stations = readStations();
	int i;
	for(i = 0; i < AList_Length(stations); i++) {
	  StationInfo* info = AList_Get(stations, i);
	  int s_id = Menu_AddItem(menu_stations, info->name);
	  Menu_AddItemTag(menu_stations, s_id, info);
	  char tag = info->tag;
	  if(tag != '\0' && tag != '0') Menu_SetTitleTag(menu_stations, s_id, tag);
	}
	AList_Destroy(stations);
}

// ---------------------------------------------------------------------------
void draw_Stations() {
  Screen_DrawBorder(_lng(STATIONS_TITLE));
  
  Menu_Draw(menu_stations, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_Stations() {
	if(menu_stations != NULL) {
		int i;
		// delete all tags
		for(i = 0; i < Menu_GetItems(menu_stations); i++) {
			StationInfo* info = (StationInfo*)Menu_GetItemTag(menu_stations, i);
			free(info->name);
			free(info->genre);
			free(info->url);
			info->name = NULL;
			info->genre = NULL;
			info->url = NULL;
		}
		Menu_Destroy(menu_stations);
		menu_stations = NULL;
	}
}
