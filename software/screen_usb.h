Menu_Info* menu_usb = NULL;
char usb_root[512];

char* isSupportedExtension(char* path, ArrayList* file_ext) {
 int i, j;
 for(i = strlen(path) - 1; i >= 0; i--) {
  if(path[i] == '.') {
   for(j = 0; j < AList_Length(file_ext); j++) {
     if(strcmp(&path[i + 1], AList_Get(file_ext, j)) == 0) return 1; 
   }
   return 0;
  }
 }
 return 0;
}

void createUSBMenu() {
 if(menu_usb != NULL) Menu_Destroy(menu_usb);
  
 menu_usb = Menu_Create(fnt_silkscreen_8, 126, 55);
 Menu_SetAutoIO(menu_usb, 1);
 Menu_EnableImages(menu_usb, 1);
 
 DIR *dp;
 struct dirent *dirp;
 if((dp = opendir(usb_root)) == NULL) return;
 
 // get supported files
 char* supported = Settings_Get("files", "supported_extensions");
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
 AList_Sort(dirlist, strcmp);
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
 AList_Sort(filelist, strcmp);
 
 // add files to menu
 for(i = 0; i < AList_Length(filelist); i++) {
  id = Menu_AddItem(menu_usb, AList_Get(filelist, i));
  Menu_AddItemImage(menu_usb, id, img_song, 5, 5);
  Menu_AddItemTag(menu_usb, id, (void*)0);
 }
 AList_Destroy(filelist);
 
 closedir(dp);
}

void init_USB() {
  strcpy(usb_root, Settings_Get("files", "usb"));
  createUSBMenu();
}

void draw_USB() {
  Screen_DrawBorder(_lng(USB_TITLE));  
  
  Menu_Draw(menu_usb, 1, 8);
  int selection = Menu_IsChosen(menu_usb);
  if(selection != -1) {
    int is_dir = (Menu_GetItemTag(menu_usb, selection) == 1);
    
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
     FILE* f = fopen(Settings_Get("files", "current_station"), "w");
     if(f != NULL) {
       fprintf(f, "USB\r\n");
       fclose(f);
     }

     char cmd[128];
     sprintf(cmd, "%s \"%s/%s\" &", Settings_Get("programs", "local"), usb_root, Menu_GetItemText(menu_usb, selection));
     system(cmd);
    }
  }
}
