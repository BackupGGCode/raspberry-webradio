///////////////////////////////////////////////////////////////
//
//  Main menu
//
//  02.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#include "screen_main.h"

// ---------------------------------------------------------------------------
void init_Main() {
  // main menu
  int id;
  menu_main = Menu_Create(fnt_dejavu_9, 126, 55);
  id = Menu_AddItem(menu_main, _lng(NOW_PLAYING));
  Menu_AddItemImage(menu_main, id, img_playing, 8, 10);
  
  id = Menu_AddItem(menu_main, _lng(STATIONS));
  Menu_AddItemImage(menu_main, id, img_station, 10, 10);
  
  id = Menu_AddItem(menu_main, _lng(USB));
  Menu_AddItemImage(menu_main, id, img_usb, 10, 8);
  
  id = Menu_AddItem(menu_main, _lng(INFO));
  Menu_AddItemImage(menu_main, id, img_info, 10, 9);
  
  Menu_SetAutoIO(menu_main, 1);
  Menu_SetBorder(menu_main, BORDER_NONE);
  Menu_EnableImages(menu_main, 1);
  //Menu_SetScroll(menu_main, SCROLL_ALWAYS);
}


// ---------------------------------------------------------------------------
void draw_Main() {
  time_t rawtime;
  struct tm * timeinfo;
  char date[24];
  
  // format date
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  
  strftime(date, 24, _lng(DATE_TIME_FORMAT), timeinfo);
  
  Screen_DrawBorder(date);
  
  // draw menu 
  Menu_Draw(menu_main, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_Main() {
  Menu_Destroy(menu_main);
}

 
