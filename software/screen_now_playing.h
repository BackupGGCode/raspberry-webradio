void draw_NowPlaying() {
  char buffer[512];

  Screen_DrawBorder(_lng(NOW_PLAYING_TITLE));
  
  GLCDD_Rect r;
  
  FILE* f = fopen(SONG_FILE, "r");
  if(f == NULL) return;
  fgets(buffer, 512, f);
  fclose(f);
 
  // split songname - artist
  int i;
  for(i = 0; i < strlen(buffer); i++) {
   if(buffer[i] == ' ' && buffer[i+1] == '-' && buffer[i+2] == ' ') {
     buffer[i+1] = 0;
     break;
   }
  }
 
  r.x = 6;
  r.y = 14;
  r.w = SCREEN_W - 12;
  r.h = -1;
  if(strlen(&buffer[i+3]) > 1) GLCDD_Print(fnt_dejavu_9b, &r, &buffer[i+3]); else GLCDD_Print(fnt_dejavu_9b, &r, "(no title)");
  r.y = 29;
  if(strlen(buffer) > 1) GLCDD_Print(fnt_dejavu_9, &r, buffer); else GLCDD_Print(fnt_dejavu_9, &r, "(no artist)");
  
 
  // get station
  f = fopen(CURRENT_STATION_FILE, "r");
  if(f == NULL) return;
  fgets(buffer, 512, f);
  fclose(f);
 
  r.y = 44;
  GLCDD_Print(fnt_dejavu_9, &r, buffer);
}
