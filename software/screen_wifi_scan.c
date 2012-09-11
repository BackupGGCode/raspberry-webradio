///////////////////////////////////////////////////////////////
//
//  WiFi Scanning
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_wifi_scan.h"

long last_scan;

void doScan() {
  char buffer[128];
  
  // scan for wifi networks
  ignore_result(system(Settings_Get("programs", "wifi_scan")));
  
  FILE* f = fopen(Settings_Get("files", "wifi_networks"), "r");
  while(fgets(buffer, 128, f) != NULL) {
    char* name = trim(buffer);
    Menu_AddItem(menu_wifi_scan, name); 
  }
  fclose(f);
  
  Menu_SetAutoIO(menu_wifi_scan, 1);
  Menu_SetBorder(menu_wifi_scan, BORDER_NONE);
  Menu_EnableImages(menu_wifi_scan, 0);
  Menu_SetScroll(menu_wifi_scan, SCROLL_AUTO);
  
  last_scan = time(NULL);  
}

// ---------------------------------------------------------------------------
void init_WifiScan() {
  menu_wifi_scan = Menu_Create(fnt_dejavu_9, 126, 55);
  
  doScan();
}


// ---------------------------------------------------------------------------
void draw_WifiScan() {
    Screen_DrawBorder(_lng(WIFI_SCAN_TITLE));
    
    // draw menu 
    Menu_Draw(menu_wifi_scan, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_WifiScan() {
  Menu_Destroy(menu_wifi_scan);
}
