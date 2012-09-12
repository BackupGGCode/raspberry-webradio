///////////////////////////////////////////////////////////////
//
//  Shoutcast Genre Lists
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  
 
#include "screen_shoutcast_genre.h"

char ShoutcastGenreParent[8];
ArrayList* ShoutcastGenres;


// ---------------------------------------------------------------------------
void setShoutcastGenreParent(char* parent) {
  strcpy(ShoutcastGenreParent, parent);
}

// ---------------------------------------------------------------------------
char* getShoutcastGenreParent() {
  return ShoutcastGenreParent;
}

// ---------------------------------------------------------------------------
ShoutcastGenre* getChosenGenre(int id) {
  return AList_Get(ShoutcastGenres, id);
}

// ---------------------------------------------------------------------------
void parseGenre(xmlDocPtr doc, xmlNodePtr cur) {
	ShoutcastGenre* info = (ShoutcastGenre*)malloc(sizeof(ShoutcastGenre));
  
	xmlChar *uri = xmlGetProp(cur, (const xmlChar*)"name");
	char* name = (char*)malloc(sizeof(char) * (1 + strlen((char*)uri)));
	strcpy(name, (char*)uri);
	info->name = name;	
	xmlFree(uri);
	
	uri = xmlGetProp(cur, (const xmlChar*)"id");
	char* id = (char*)malloc(sizeof(char) * (1 + strlen((char*)uri)));
	strcpy(id, (char*)uri);
	info->id = id;
	xmlFree(uri);
	
	uri = xmlGetProp(cur, (const xmlChar*)"haschildren");
	if(strcmp((char*)uri, "true") == 0) info->has_children = 1; else info->has_children = 0;
	xmlFree(uri);
	
	AList_Add(ShoutcastGenres, info);
}

// ---------------------------------------------------------------------------
void parseGenreList(char *docname) {
	xmlDocPtr doc;
	xmlNodePtr cur;

	ShoutcastGenres = AList_Create();
	
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "response")) {
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if((!xmlStrcmp(cur->name, (const xmlChar*)"data"))) {
		  cur = cur->xmlChildrenNode;
		  
		  while(cur != NULL) {
		    if((!xmlStrcmp(cur->name, (const xmlChar*)"genrelist"))) {
		      cur = cur->xmlChildrenNode;
		      
		      while(cur != NULL) {
			if((!xmlStrcmp(cur->name, (const xmlChar*)"genre"))) {
			  parseGenre(doc, cur); 
			}
			cur = cur->next;
		      }
		      
		      xmlFreeDoc(doc);
		      return;
		    }
		    
		    cur = cur->next;
		  }
		  
		  cur = cur->next;
		}
	  
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}


// ---------------------------------------------------------------------------
void init_ShoutcastGenre() {
  menu_genres = Menu_Create(fnt_silkscreen_8, 126, 55);
  Menu_SetAutoIO(menu_genres, 1);
  Menu_SetBorder(menu_genres, BORDER_NONE);
  
  // download station xml
  if(strcmp(ShoutcastGenreParent, "0") == 0) {
    Download_Get("http://api.shoutcast.com/genre/primary?k=so1N15vhCB78Z6k4&f=xml", "station.xml");
  } else {
    char buffer[128];
    sprintf(buffer, "http://api.shoutcast.com/genre/secondary?parentid=%s&k=so1N15vhCB78Z6k4&f=xml", ShoutcastGenreParent);
    printf("Get subgenre: %s\r\n", buffer);
    Download_Get(buffer, "station.xml");
  }
  // parse the stations
  parseGenreList("station.xml");
  // add to menu
  int i;
  Menu_AddItem(menu_genres, _lng(GENRE_BACK));
  for(i = 0; i < AList_Length(ShoutcastGenres); i++) {
    int id = Menu_AddItem(menu_genres, ((ShoutcastGenre*)AList_Get(ShoutcastGenres, i))->name);
    Menu_AddItemTag(menu_genres, id, (void*)i);
  }
}


// ---------------------------------------------------------------------------
void draw_ShoutcastGenre() {
  Screen_DrawBorder(_lng(SHOUTCAST_TITLE));
  
  Menu_Draw(menu_genres, 1, 8);
  
}


// ---------------------------------------------------------------------------
void exit_ShoutcastGenre() {
  Menu_Destroy(menu_genres);
  
  // destroy list
  int i;
  for(i = 0; i < AList_Length(ShoutcastGenres); i++) {
    ShoutcastGenre* info = AList_Get(ShoutcastGenres, i);
    free(info->name);
    free(info->id);
    free(info);
  }
  AList_Destroy(ShoutcastGenres);
}

