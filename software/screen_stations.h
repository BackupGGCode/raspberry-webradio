///////////////////////////////////////////////////////////////
//
//  Station list
//
//  02.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#ifndef _SCREEN_STATIONS_H_
#define _SCREEN_STATIONS_H_

#include "firmware.h"

typedef struct {
  char* name;
  char* genre;
  char* url;
  char tag;
} StationInfo;

extern Menu_Info* menu_stations;

void playStation(StationInfo* station);
ArrayList* readStations();
void writeStations(ArrayList* stations);
void asFavorite(int fav_id);
void playFavorite(int id);
void init_Stations();
void draw_Stations();
void exit_Stations();

#endif
