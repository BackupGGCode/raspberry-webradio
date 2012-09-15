///////////////////////////////////////////////////////////////
//
//  Backlight timeout
//
//  15.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_timeout.h"

int Timeout_Seconds;

// ---------------------------------------------------------------------------
int Timeout_Get() {
  return Timeout_Seconds;
}

// ---------------------------------------------------------------------------
void init_Timeout() {
  Timeout_Seconds = atoi(Settings_Get("hardware", "timeout"));
}


// ---------------------------------------------------------------------------
void draw_Timeout() {
  GLCDD_Rect r;
  char buffer[4];

  Screen_DrawBorder(_lng(BACKLIGHT_TIMEOUT));

  // increase/decrease minutes  
  int8_t rotary = IO_GetRotaryValue();
  if(rotary != 0) {
   Timeout_Seconds += rotary;
   if(Timeout_Seconds < 0) Timeout_Seconds = 0;
   if(Timeout_Seconds > 300) Timeout_Seconds = 300;
  }
  
  // draw seconds
  sprintf(buffer, "%d", Timeout_Seconds);

  int width = GLCDD_StringWidth(fnt_dejavu_9, buffer);
  r.x = SCREEN_W / 2 - 5 - 3 - GLCDD_StringWidth(fnt_dejavu_9, _lng(SECONDS)) / 2 ;

  GLCDD_XBMDraw((uint8_t*)img_setter, r.x, SCREEN_H / 2 + 4 - 12, 9, 24);
  r.x += 5 - width / 2;
  r.y = SCREEN_H / 2 + 4 - GLCDD_FontHeight(fnt_dejavu_9) / 2;
  r.w = 32;
  r.h = -1;
  GLCDD_Print(fnt_dejavu_9, &r, buffer);
  r.x += width / 2 + 4 + 6;
  r.w = 64;
  GLCDD_Print(fnt_dejavu_9, &r, _lng(SECONDS));
  
  
}