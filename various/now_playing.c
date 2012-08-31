#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"

#include "fnt_dejavu_9.h"
#include "fnt_dejavu_9_b.h"
#include "fnt_silkscreen_8.h"

int main() {
 GLCDD_Init();
 
 while(1) {
 
 GLCDD_Clear();
 GLCDD_Rect(0, 0, 128, 64, 0);
 GLCDD_Rect(0, 0, 128, 7, 1);
 GLCDD_ClearPixel(0, 0);
 GLCDD_ClearPixel(127, 0);
 GLCDD_ClearPixel(0, 63);
 GLCDD_ClearPixel(127, 63);
 
 GLCDD_PrintCol(silkscreen_8, 40, 0, "NOW PLAYING", 1);
 
 char buffer[512];
 FILE* f = fopen("song.txt", "r");
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
 
 GLCDD_Print(dejavu_9_b, 6, 14, &buffer[i+3]);
 GLCDD_Print(dejavu_9, 6, 29, buffer);
 
 // get station
 f = fopen("/var/www/m/current_station.txt", "r");
 fgets(buffer, 512, f);
 fclose(f);
 
 GLCDD_Print(dejavu_9, 6, 44, buffer);
 
 
 GLCDD_Draw();
 sleep(1);
 
 }
 
}
