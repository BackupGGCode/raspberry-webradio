///////////////////////////////////////////////////////////////
//
//  Select Language
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_language.h"

// ---------------------------------------------------------------------------
void init_Language() {
  int i;
  char file[32];
  char display[32];

  menu_language = Menu_Create(fnt_dejavu_9, 126, 55);
  Menu_SetAutoIO(menu_language, 1);
  
  DIR *dp;
  struct dirent *dirp;
  if((dp = opendir("language")) == NULL) return;
 
  // add files
  ArrayList* filelist = AList_Create();
 
  while((dirp = readdir(dp)) != NULL) {
   if(dirp->d_type == DT_DIR) continue;
   if(!isFileExtension(dirp->d_name, ".lng")) continue;
   
   strcpy(file, dirp->d_name);
   file[strlen(file) - 4] = 0;
   sprintf(display, "%s (%s)", Settings_Get("language", file), file);
   char* cfile = (char*)malloc(sizeof(char) * (1 + strlen(display)));
   strcpy(cfile, display);
   
   AList_Add(filelist, cfile);
  }
  AList_Sort(filelist, nameCompare);
 
  // add files to menu
  for(i = 0; i < AList_Length(filelist); i++) {
   Menu_AddItem(menu_language, AList_Get(filelist, i));
   free(AList_Get(filelist, i));
  }
  AList_Destroy(filelist);
 
  closedir(dp);
 
}


// ---------------------------------------------------------------------------
void draw_Language() {
  Screen_DrawBorder(_lng(LANGUAGE_TITLE));
  
  // draw menu 
  Menu_Draw(menu_language, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_Language() {
  Menu_Destroy(menu_language);
}


