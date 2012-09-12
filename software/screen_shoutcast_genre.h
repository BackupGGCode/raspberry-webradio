///////////////////////////////////////////////////////////////
//
//  Shoutcast Genre Lists
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  
 
#ifndef _SCREEN_SHOUTCAST_GENRE_H_
#define _SCREEN_SHOUTCAST_GENRE_H_

#include "firmware.h"


typedef struct {
  char* name;
  char* id;
  int has_children;
} ShoutcastGenre;


Menu_Info* menu_genres;

void init_ShoutcastGenre();
void draw_ShoutcastGenre();
void exit_ShoutcastGenre();

ShoutcastGenre* getChosenGenre(int id);
void setShoutcastGenreParent(char* parent);
char* getShoutcastGenreParent();

#endif
