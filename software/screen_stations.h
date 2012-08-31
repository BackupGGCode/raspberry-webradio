Menu_Info* menu_stations = NULL;

typedef struct {
  char* name;
  char* genre;
  char* url;
} StationInfo;

// ---------------------------------------------------------------------------
void init_Stations() {
	// get station list
	menu_stations = Menu_Create(fnt_silkscreen_8, 126, 55);
	Menu_SetAutoIO(menu_stations, 1);
	Menu_SetBorder(menu_stations, BORDER_NONE);
	
	char buffer[256];
	char* ptr;
	FILE* f = fopen(STATIONS_FILE, "r");
	if(f == NULL) return;
	
	// read stations
	while(fgets(buffer, 256, f) != NULL) {
		if(strlen(buffer) < 3) continue;
		
		StationInfo* info = (StationInfo*)malloc(sizeof(StationInfo));
		ptr = strtok(buffer, "|");
		info->name = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
		strcpy(info->name, ptr);
		ptr = strtok(NULL, "|");
		info->url = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
		strcpy(info->url, ptr);
		ptr = strtok(NULL, "|");
		info->genre = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
		strcpy(info->genre, ptr);
		ptr = strtok(NULL, "|");
		char tag = '\0';
		if(ptr != NULL) tag = ptr[0];
		// add to station list
		int s_id = Menu_AddItem(menu_stations, info->name);
		Menu_AddItemTag(menu_stations, s_id, (void*)info);
		if(tag != '\0' && tag != '0') Menu_SetTitleTag(menu_stations, s_id, tag);
	}
	fclose(f);
}

// ---------------------------------------------------------------------------
void draw_Stations() {
  Screen_DrawBorder(_lng(STATIONS_TITLE));
  
  Menu_Draw(menu_stations, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_Stations() {
	if(menu_stations != NULL) {
		int i;
		// delete all tags
		for(i = 0; i < Menu_GetItems(menu_stations); i++) {
			StationInfo* info = (StationInfo*)Menu_GetItemTag(menu_stations, i);
			free(info->name);
			free(info->genre);
			free(info->url);
			info->name = NULL;
			info->genre = NULL;
			info->url = NULL;
		}
		Menu_Destroy(menu_stations);
		menu_stations = NULL;
	}
}
