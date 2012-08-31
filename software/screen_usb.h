Menu_Info* menu_usb = NULL;
char usb_root[512];

void createUSBMenu() {
 if(menu_usb != NULL) Menu_Destroy(menu_usb);
  
 menu_usb = Menu_Create(fnt_silkscreen_8, 126, 55);
 Menu_SetAutoIO(menu_usb, 1);
 Menu_EnableImages(menu_usb, 1);
 
 DIR *dp;
 struct dirent *dirp;
 if((dp = opendir(usb_root)) == NULL) return;
 
 int id;
 // add directories
 while((dirp = readdir(dp)) != NULL) {
   if(strcmp(dirp->d_name, ".") == 0) continue;
   if(strcmp(usb_root, Settings_Get("files", "usb")) == 0 && strcmp(dirp->d_name, "..") == 0) continue;
   if(dirp->d_type != DT_DIR) continue; 
   
   id = Menu_AddItem(menu_usb, dirp->d_name);
   if(strcmp(dirp->d_name, "..") == 0) Menu_AddItemImage(menu_usb, id, img_up, 5, 5);
   Menu_AddItemTag(menu_usb, id, (void*)1);
 }
 rewinddir(dp);
 // add files
 while((dirp = readdir(dp)) != NULL) {
  if(dirp->d_type == DT_DIR) continue;
  
  id = Menu_AddItem(menu_usb, dirp->d_name);
  Menu_AddItemImage(menu_usb, id, img_song, 5, 5);
  Menu_AddItemTag(menu_usb, id, (void*)0);
 }
 
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
     sprintf(usb_root, "%s/%s", usb_root, Menu_GetItemText(menu_usb, selection));
     printf("cd %s\n", usb_root);
     createUSBMenu();
    } else {
     char cmd[128];
     sprintf(cmd, "%s \"%s/%s\" &", Settings_Get("programs", "local"), usb_root, Menu_GetItemText(menu_usb, selection));
     system(cmd);
    }
  }
}
