///////////////////////////////////////////////////////////
//
// Settings Manager
//
// 21.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Settings_Load(char* file);
int Settings_Unload();
char* Settings_Get(char* category, char* key);
int Settings_Add(char* category, char* key, char* value);
int Settings_Save(char* file);

// useful function
char* trim(char* str);

#endif
