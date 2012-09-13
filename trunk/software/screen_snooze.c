///////////////////////////////////////////////////////////////
//
//  Set Snooze Time
//
//  13.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////   

#include "screen_snooze.h"

int Snooze_Minutes;
int Snooze_IsSnooze;
unsigned long Snooze_Time;

// ---------------------------------------------------------------------------
int isSnooze() {
 return Snooze_IsSnooze; 
}

// ---------------------------------------------------------------------------
int checkSnoozeStop() {
 if(Snooze_IsSnooze) {
   if(time(NULL) >= Snooze_Time) {
    Snooze_IsSnooze = 0;
    return 1;
   } else return 0;
 } else return 0;
}

// ---------------------------------------------------------------------------
void init_Snooze() {
  Snooze_Minutes = 0;
  Snooze_IsSnooze = 0;
}


// ---------------------------------------------------------------------------
void draw_Snooze() {
  GLCDD_Rect r;
  char buffer[4];

  Screen_DrawBorder(_lng(SNOOZE));

  // increase/decrease minutes  
  int8_t rotary = IO_GetRotaryValue();
  if(rotary != 0) {
   Snooze_Minutes += rotary;
   if(Snooze_Minutes < 0) Snooze_Minutes = 0;
   if(Snooze_Minutes > 59) Snooze_Minutes = 59;
  }
  
  if(IO_GetButton(0)) {
    if(Snooze_Minutes == 0) {
      Snooze_IsSnooze = 0; 
    } else {
      Snooze_IsSnooze = 1;
      Snooze_Time = time(NULL) + (60 * Snooze_Minutes);
    }
    // go back to now playing
    Screen_Goto(SCREEN_NOW_PLAYING); 
  }

  // draw minutes
  sprintf(buffer, "%d", Snooze_Minutes);

  int width = GLCDD_StringWidth(fnt_dejavu_9, buffer);
  r.x = SCREEN_W / 2 - 5 - 3 - GLCDD_StringWidth(fnt_dejavu_9, _lng(MINUTES)) / 2 ;

  GLCDD_XBMDraw((uint8_t*)img_setter, r.x, SCREEN_H / 2 + 4 - 12, 9, 24);
  r.x += 5 - width / 2;
  r.y = SCREEN_H / 2 + 4 - GLCDD_FontHeight(fnt_dejavu_9) / 2;
  r.w = 32;
  r.h = -1;
  GLCDD_Print(fnt_dejavu_9, &r, buffer);
  r.x += width / 2 + 4 + 6;
  r.w = 64;
  GLCDD_Print(fnt_dejavu_9, &r, _lng(MINUTES));
  
  
}


// ---------------------------------------------------------------------------
void exit_Snooze() {
  
}

