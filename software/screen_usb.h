Menu_Info* menu_usb = NULL;
char usb_root[512];


// ---------------------------------------------------------------------------
int isSupportedExtension(char* path, ArrayList* file_ext) {
 int i, j;
 for(i = strlen(path) - 1; i >= 0; i--) {
  if(path[i] == '.') {
   for(j = 0; j < AList_Length(file_ext); j++) {
     if(strcmp(&path[i + 1], (char*)AList_Get(file_ext, j)) == 0) return 1; 
   }
   return 0;
  }
 }
 return 0;
}


// ---------------------------------------------------------------------------
int isFileExtension(char* name, char* ext) {
 if(strlen(name) < strlen(ext)) return 0;
 if(strcmp(&name[strlen(name) - strlen(ext)], ext) == 0) return 1;
 else return 0;
}

// ---------------------------------------------------------------------------
int nameCompare(void* str1, void* str2) {
  char* name1 = (char*)str1;
  char* name2 = (char*)str2;
  int i, len = strlen(name1);
  
  // playlist before songs
  if(isFileExtension(name1, ".m3u") && !isFileExtension(name2, ".m3u")) return -1;
  if(!isFileExtension(name1, ".m3u") && isFileExtension(name2, ".m3u")) return 1;
  
  if(strlen(name2) < len) len = strlen(name2);
  for(i = 0; i < len; i++) {
   if((name1[i] | 32) < (name2[i] | 32)) return -1;
   else if((name1[i] | 32) > (name2[i] | 32)) return 1;
  }
  if(strlen(name1) > strlen(name2)) return 1;
  else if(strlen(name1) < strlen(name2)) return -1;
  else return 0;
}

// ---------------------------------------------------------------------------
void createUSBMenu() {
 if(menu_usb != NULL) Menu_Destroy(menu_usb);
  
 menu_usb = Menu_Create(fnt_silkscreen_8, 126, 55);
 Menu_SetAutoIO(menu_usb, 1);
 Menu_EnableImages(menu_usb, 1);
 
 DIR *dp;
 struct dirent *dirp;
 if((dp = opendir(usb_root)) == NULL) return;
 
 // get supported files
 char* supported_ro = Settings_Get("files", "supported_extensions");
 char* supported = (char*)malloc(sizeof(char) * (1 + strlen(supported_ro)));
 strcpy(supported, supported_ro);
 ArrayList* file_ext = AList_Split(supported, ";");
 
 int id, i;
 
 // add directories
 ArrayList* dirlist = AList_Create();
 
 while((dirp = readdir(dp)) != NULL) {
   if(strcmp(dirp->d_name, ".") == 0) continue;
   if(strcmp(usb_root, Settings_Get("files", "usb")) == 0 && strcmp(dirp->d_name, "..") == 0) continue;
   if(dirp->d_type != DT_DIR) continue; 
   
   AList_Add(dirlist, dirp->d_name);
 }
 AList_Sort(dirlist, nameCompare);
 rewinddir(dp);
 
 // add directories to menu
 for(i = 0; i < AList_Length(dirlist); i++) {
  id = Menu_AddItem(menu_usb, AList_Get(dirlist, i));
  if(strcmp(AList_Get(dirlist, i), "..") == 0) Menu_AddItemImage(menu_usb, id, img_up, 5, 5);
  Menu_AddItemTag(menu_usb, id, (void*)1);
 }
 AList_Destroy(dirlist);
 
 // add files
 ArrayList* filelist = AList_Create();
 
 while((dirp = readdir(dp)) != NULL) {
  if(dirp->d_type == DT_DIR) continue;
  if(!isSupportedExtension(dirp->d_name, file_ext)) continue;
  AList_Add(filelist, dirp->d_name);
 }
 AList_Sort(filelist, nameCompare);
 
 // add files to menu
 for(i = 0; i < AList_Length(filelist); i++) {
  id = Menu_AddItem(menu_usb, AList_Get(filelist, i));
  if(isFileExtension(AList_Get(filelist, i), ".m3u")) Menu_AddItemImage(menu_usb, id, img_playlist, 5, 5);
  else Menu_AddItemImage(menu_usb, id, img_song, 5, 5);
  Menu_AddItemTag(menu_usb, id, (void*)0);
 }
 AList_Destroy(filelist);
 
 closedir(dp);
 
 AList_Destroy(file_ext);
 free(supported);
}

// ---------------------------------------------------------------------------
void playUSB(char* filename) {
  FILE* f = fopen(Settings_Get("files", "current_station"), "w");
  if(f != NULL) {
    fprintf(f, "%s\r\n", _lng(USB));
    fclose(f);
  }

  char cmd[512];
  sprintf(cmd, "%s \"%s\" &", Settings_Get("programs", "local"), filename);
  system(cmd);
}


// ---------------------------------------------------------------------------
void init_USB() {
  strcpy(usb_root, Settings_Get("files", "usb"));
  createUSBMenu();
}

// ---------------------------------------------------------------------------
void draw_USB() {
  Screen_DrawBorder(_lng(USB_TITLE));  
  
  Menu_Draw(menu_usb, 1, 8);
  int selection = Menu_IsChosen(menu_usb);
  if(selection != -1) {
    int is_dir = ((int)Menu_GetItemTag(menu_usb, selection) == 1);
    
    if(is_dir) {
     // change directory
     if(strcmp(Menu_GetItemText(menu_usb, selection), "..") == 0) {
       // directory up 
       int i;
       if(strlen(usb_root) > 0) {
         for(i = strlen(usb_root) - 1; i >= 0; i--) {
	   if(usb_root[i] == '/') {
	     usb_root[i] = '\0';
	     break;
	   }
         }
       }
     } else {
       sprintf(usb_root, "%s/%s", usb_root, Menu_GetItemText(menu_usb, selection));
     }
     printf("cd %s\n", usb_root);
     createUSBMenu();
    } else {
      char file[512];
      sprintf(file, "%s/%s", usb_root, Menu_GetItemText(menu_usb, selection));
      playUSB(file);
    }
  }
}
