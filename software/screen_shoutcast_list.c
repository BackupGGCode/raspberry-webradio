///////////////////////////////////////////////////////////////
//
//  Shoutcast Station Browser
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////   

#include "screen_shoutcast_list.h"

char ShoutcastUrl[256];
char ShoutcastGenreParent[16];
char ShoutcastCurGenre[64];
ArrayList* ShoutcastList;

// ---------------------------------------------------------------------------
void setStationsParentGenre(char* genre) {
  strcpy(ShoutcastGenreParent, genre); 
}

// ---------------------------------------------------------------------------
void setCurrentGenre(char* genre) {
  strcpy(ShoutcastCurGenre, genre);
}

// ---------------------------------------------------------------------------
char* getCurrentGenre() {
  return ShoutcastCurGenre;
}


// ---------------------------------------------------------------------------
char* getStationsParentGenre() {
  return ShoutcastGenreParent; 
}

// ---------------------------------------------------------------------------
ShoutcastStation* getChosenStation(int id) {
  return AList_Get(ShoutcastList, id);
}

// ---------------------------------------------------------------------------
StationInfo* parseShoutcastList(ShoutcastStation* station) {
  char buffer[256];
  sprintf(buffer, "http://yp.shoutcast.com/sbin/tunein-station.pls?id=%s", station->id);
  Download_Get(buffer, "/tmp/station.pls");
  
  // shuffle playlist to choose a random mirror
  sprintf(buffer, "%s /tmp/station.pls > /tmp/station1.pls", Settings_Get("programs", "shuffle"));
  ignore_result(system(buffer));
  
  FILE *f = fopen("/tmp/station1.pls", "r");
  if(f == NULL) return NULL;
  
  char *url = NULL;
  while(fgets(buffer, 256, f) != NULL) {
    if(strncmp(buffer, "File", 4) != 0) continue;
    // found a mirror
    url = &buffer[4];
    while(*url != '\0' && *url != '=') url++;
    if(*url != '\0') {
      url++;
      break;
    }
  }
  url = trim(url);
  fclose(f);
   
  if(url != NULL) {
      char* s_url = (char*)malloc(sizeof(char) * (1 + strlen(url)));
      strcpy(s_url, url);
    
      printf("Play station '%s' @ '%s'\r\n", station->name, url);
      StationInfo* info = (StationInfo*)malloc(sizeof(StationInfo));
      info->name = station->name;
      info->genre = NULL;
      info->tag = '\0';
      info->url = s_url;  
      return info;
  } else {
   return NULL; 
  }  
}

// ---------------------------------------------------------------------------
void parseStation(xmlDocPtr doc, xmlNodePtr cur) {

	ShoutcastStation* info = (ShoutcastStation*)malloc(sizeof(ShoutcastStation));
  
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
	
	AList_Add(ShoutcastList, info);
}

// ---------------------------------------------------------------------------
void parseStationList(char *docname) {
	xmlDocPtr doc;
	xmlNodePtr cur;

	ShoutcastList = AList_Create();
	
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		xmlFreeDoc(doc);
		return;
	}
	
	// if we are not in legacy mode, travel to stationlist node
	int found = 0;
	if(!xmlStrcmp(cur->name, (const xmlChar*)"response")) {
	  cur = cur->xmlChildrenNode;
	  while (cur != NULL) {
	    if((!xmlStrcmp(cur->name, (const xmlChar*)"data"))) {
		  cur = cur->xmlChildrenNode;
		  
		  while(cur != NULL) {
		    if((!xmlStrcmp(cur->name, (const xmlChar*)"stationlist"))) {
		      found = 1;
		      break;
		    }
		    cur = cur->next;
		  }
		  if(found) break;
	    }
	    cur = cur->next;
	  }
	} 
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "stationlist")) {
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"station"))){
			parseStation (doc, cur);
		}
		 
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

// ---------------------------------------------------------------------------
void setShoutcastListUrl(char* url) {
  strcpy(ShoutcastUrl, url);
}


// ---------------------------------------------------------------------------
void init_ShoutcastList() {
  menu_station_list = Menu_Create(fnt_silkscreen_8, 126, 55);
  Menu_SetAutoIO(menu_station_list, 1);
  Menu_SetBorder(menu_station_list, BORDER_NONE);
  
  // download station xml
  Download_Get(ShoutcastUrl, "station.xml");
  // parse the stations
  parseStationList("station.xml");
  // add to menu
  int i;
  Menu_AddItem(menu_station_list, _lng(GENRE_BACK));
  for(i = 0; i < AList_Length(ShoutcastList); i++) {
    int id = Menu_AddItem(menu_station_list, ((ShoutcastStation*)AList_Get(ShoutcastList, i))->name);
    Menu_AddItemTag(menu_station_list, id, (void*)i);
  }
  
  // play or favorite menu
  menu_play_fav = Menu_Create(fnt_silkscreen_8, 64, 32);
  Menu_SetAutoIO(menu_play_fav, 1);
  Menu_SetBorder(menu_play_fav, BORDER_SOLID);
  Menu_AddItem(menu_play_fav, _lng(PLAY));
  Menu_AddItem(menu_play_fav, _lng(AS_FAVORITE));
  Menu_AddItem(menu_play_fav, _lng(CANCEL));
}


// ---------------------------------------------------------------------------
void draw_ShoutcastList() {
  Screen_DrawBorder(_lng(SHOUTCAST_TITLE));
  
  Menu_Draw(menu_station_list, 1, 8);
  if(!Menu_GetAutoIO(menu_station_list)) Menu_Draw(menu_play_fav, 32, 16);
}

// ---------------------------------------------------------------------------
void exit_ShoutcastList() {
  Menu_Destroy(menu_station_list);
  
  int i;
  for(i = 0; i < AList_Length(ShoutcastList); i++) {
    ShoutcastStation *info = AList_Get(ShoutcastList, i);
    free(info->name);
    free(info->id);
    free(info);
  }
  AList_Destroy(ShoutcastList);
}

