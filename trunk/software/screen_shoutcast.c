///////////////////////////////////////////////////////////////
//
//  Shoutcast Lists
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_shoutcast.h"


// ---------------------------------------------------------------------------
void init_Shoutcast() {
  menu_shoutcast = Menu_Create(fnt_dejavu_9, 126, 55);

  Menu_AddItem(menu_shoutcast, _lng(SHOUTCAST_TOP));
  Menu_AddItem(menu_shoutcast, _lng(SHOUTCAST_RANDOM));
  Menu_AddItem(menu_shoutcast, _lng(GENRE));
  
  Menu_SetAutoIO(menu_shoutcast, 1);
  Menu_SetBorder(menu_shoutcast, BORDER_NONE);
  Menu_EnableImages(menu_shoutcast, 0);
  Menu_SetScroll(menu_shoutcast, SCROLL_AUTO);
  
}


// ---------------------------------------------------------------------------
void draw_Shoutcast() {
  Screen_DrawBorder(_lng(SHOUTCAST_TITLE));
  
  Menu_Draw(menu_shoutcast, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_Shoutcast() {
  Menu_Destroy(menu_shoutcast);
}


