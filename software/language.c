///////////////////////////////////////////////////////////
//
// Language File Manager
//
// 20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#include "language.h"


char language[] = {'e', 'n', 0};
char** language_strings = NULL;
int language_entries = 0;

int Language_Set(char* lang) {
  Language_Cleanup();
  language[0] = lang[0];
  language[1] = lang[1];
  return Language_Init(language);
}


char* Language_Get() {
  return &language[0];
}


int Language_Init(char* lang) {
  language[0] = lang[0];
  language[1] = lang[1];
  char file[32];
  char buffer[128];
  int entries = 64;
  sprintf(file, "language/%s.lng", language);

  FILE* f = fopen(file, "r");
  if(f == NULL) return 0;

  // reserve space for strings
  language_strings = (char**)malloc(sizeof(char*) * entries); 

  // read all entries
  int index = 0;
  while((fgets(buffer, 128, f)) != NULL) {
	language_strings[index] = (char*)malloc(sizeof(char) * (1 + strlen(buffer)));
	strcpy(language_strings[index], buffer);
	index++;
	if(index >= entries) {
		entries *= 2;
		language_strings = (char**)realloc(language_strings, entries * sizeof(char));
	}
  }   
  fclose(f);
  language_entries = index;
  return 1;
}


int Language_Cleanup() {
	int i;
	for(i = 0; i < language_entries; i++) {
		free(language_strings[i]);
	}
	free(language_strings);
	language_entries = 0;
	language_strings = NULL;
	return 1;
}


char* Language_Translate(Language_String str) {
	if(str >= language_entries) return NULL;
	return language_strings[str];
}

