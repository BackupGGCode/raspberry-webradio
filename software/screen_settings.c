///////////////////////////////////////////////////////////////
//
//  Settings
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_settings.h"

// ---------------------------------------------------------------------------
void init_Settings() {
  menu_settings = Menu_Create(fnt_dejavu_9, 126, 55);
  int id;
  
  id = Menu_AddItem(menu_settings, _lng(INFO));
  Menu_AddItemImage(menu_settings, id, img_info, 10, 9);

  id = Menu_AddItem(menu_settings, _lng(WIFI));
  Menu_AddItemImage(menu_settings, id, img_wifi, 9, 10);
  
  id = Menu_AddItem(menu_settings, _lng(LANGUAGE));
  Menu_AddItemImage(menu_settings, id, img_language, 10, 10);
  
  id = Menu_AddItem(menu_settings, _lng(VOLUME));
  Menu_AddItemImage(menu_settings, id, img_volume, 10, 10);
  
  id = Menu_AddItem(menu_settings, _lng(MANAGE_STATIONS));
  Menu_AddItemImage(menu_settings, id, img_edit, 10, 10);
  
  id = Menu_AddItem(menu_settings, _lng(RESTART));
  Menu_AddItemImage(menu_settings, id, img_reset, 10, 10);
  
  id = Menu_AddItem(menu_settings, _lng(SHUTDOWN));
  Menu_AddItemImage(menu_settings, id, img_shutdown, 9, 10);
  
  Menu_SetAutoIO(menu_settings, 1);
  Menu_SetBorder(menu_settings, BORDER_NONE);
  Menu_EnableImages(menu_settings, 1);
  //Menu_SetScroll(menu_settings, SCROLL_ALWAYS);
}

// ---------------------------------------------------------------------------
void draw_Settings() {
  Screen_DrawBorder(_lng(SETTINGS_TITLE));
  
  // draw menu 
  Menu_Draw(menu_settings, 1, 8);
}

// ---------------------------------------------------------------------------
void exit_Settings() {
  Menu_Destroy(menu_settings);
}
