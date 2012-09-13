///////////////////////////////////////////////////////////////
//
//  Manage Stations
//
//  13.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////   

#include "screen_manage_station.h"

// ---------------------------------------------------------------------------
void init_ManageStation() {
  // get station list
  menu_m_station = Menu_Create(fnt_silkscreen_8, 126, 55);
  Menu_SetAutoIO(menu_m_station, 1);
  Menu_SetBorder(menu_m_station, BORDER_NONE);

  ArrayList* stations = readStations();
  int i;
  for(i = 0; i < AList_Length(stations); i++) {
    StationInfo* info = AList_Get(stations, i);
    int s_id = Menu_AddItem(menu_m_station, info->name);
    Menu_AddItemTag(menu_m_station, s_id, info);
  }
  AList_Destroy(stations);
  
  // create menu
  menu_m_menu = Menu_Create(fnt_silkscreen_8, 64, 32);
  Menu_SetAutoIO(menu_m_menu, 1);
  Menu_SetBorder(menu_m_menu, BORDER_SOLID);
  
  Menu_AddItem(menu_m_menu, _lng(CANCEL));
  Menu_AddItem(menu_m_menu, _lng(MOVE));
  Menu_AddItem(menu_m_menu, _lng(DELETE));
}


// ---------------------------------------------------------------------------
void draw_ManageStation() {
  Screen_DrawBorder(_lng(STATIONS_MANAGE_TITLE));

  // draw menu
  Menu_Draw(menu_m_station, 1, 8);
  if(!Menu_GetAutoIO(menu_m_station) && Menu_GetAutoIO(menu_m_menu)) Menu_Draw(menu_m_menu, 32, 16);
}


// ---------------------------------------------------------------------------
void exit_ManageStation() {
  if(menu_m_station != NULL) {
    int i;
    // delete all tags
    for(i = 0; i < Menu_GetItems(menu_m_station); i++) {
	    StationInfo* info = (StationInfo*)Menu_GetItemTag(menu_m_station, i);
	    free(info->name);
	    free(info->genre);
	    free(info->url);
	    info->name = NULL;
	    info->genre = NULL;
	    info->url = NULL;
    }
    Menu_Destroy(menu_m_station);
    menu_m_station = NULL;
  }
}
