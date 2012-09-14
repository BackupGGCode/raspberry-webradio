///////////////////////////////////////////////////////////////
//
//  Shoutcast Search Station
//
//  14.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////   

#include "screen_shoutcast_search.h"

// ---------------------------------------------------------------------------
void init_ShoutcastSearch() {
  Keyboard_Init();
}


// ---------------------------------------------------------------------------
void draw_ShoutcastSearch() {
  Screen_DrawBorder(_lng(SEARCH));
  
  Keyboard_Draw();
}


