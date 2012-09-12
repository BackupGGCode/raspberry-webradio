///////////////////////////////////////////////////////////////
//
//  Shoutcast Station Browser
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////   

#ifndef _SCREEN_SHOUTCAST_LIST_H_
#define _SCREEN_SHOUTCAST_LIST_H_

#include "firmware.h"
#include <libxml/parser.h>

typedef struct {
  char* name;
  char* id;
} ShoutcastStation;

Menu_Info* menu_station_list;

void setShoutcastListUrl(char* url);
ShoutcastStation* getChosenStation(int id);
StationInfo* parseShoutcastList(ShoutcastStation* station);
void setStationsParentGenre(char* genre);
char* getStationsParentGenre();

void init_ShoutcastList();
void draw_ShoutcastList();
void exit_ShoutcastList();

#endif


