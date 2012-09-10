///////////////////////////////////////////////////////////////
//
//  Now playing
//
//  02.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#include "screen_now_playing.h"


// ---------------------------------------------------------------------------
void draw_NowPlaying() {
  char buffer[512];

  Screen_DrawBorder(_lng(NOW_PLAYING_TITLE));
  
  GLCDD_Rect r;
  
  FILE* f = fopen(SONG_FILE, "r");
  if(f == NULL) return;
  ignore_result(fgets(buffer, 512, f));
  fclose(f);
 
  // split songname - artist
  int i, found = 0;
  for(i = 0; i < strlen(buffer); i++) {
   if(buffer[i] == ' ' && buffer[i+1] == '-' && buffer[i+2] == ' ') {
     buffer[i+1] = 0;
     found = 1;
     break;
   }
  }
  if(!found) i = -3;
 
  r.x = 6;
  r.y = 14;
  r.w = SCREEN_W - 12;
  r.h = -1;
  if(strlen(&buffer[i+3]) > 1) GLCDD_Print(fnt_dejavu_9b, &r, &buffer[i+3]); else GLCDD_Print(fnt_dejavu_9b, &r, _lng(NO_TITLE));
  r.y = 29;
  if(strlen(buffer) > 1) GLCDD_Print(fnt_dejavu_9, &r, buffer); else GLCDD_Print(fnt_dejavu_9, &r, _lng(NO_ARTIST));
  
 
  // get station
  f = fopen(CURRENT_STATION_FILE, "r");
  if(f == NULL) return;
  ignore_result(fgets(buffer, 512, f));
  fclose(f);
 
  r.y = 44;
  GLCDD_Print(fnt_dejavu_9, &r, buffer);
  
  // we play from usb and can therefore skip the song
  if(strncmp(buffer, _lng(USB), strlen(_lng(USB))) == 0) {
   GLCDD_RectRounded(SCREEN_W - 36, SCREEN_H - 14, 32, 10, 1);
   fnt_dejavu_9->color = 1;
   r.x = SCREEN_W - 29;
   r.y = SCREEN_H - 13;
   r.w = 28;
   r.h = -1;
   GLCDD_Print(fnt_dejavu_9, &r, _lng(NEXT));
   fnt_dejavu_9->color = 0;
   
   if(IO_GetButton(0)) {
      char cmd[64];
      sprintf(cmd, "%s &", Settings_Get("programs", "next_song"));
      ignore_result(system(cmd));
   }
  }
}

