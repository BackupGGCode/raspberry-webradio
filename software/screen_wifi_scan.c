///////////////////////////////////////////////////////////////
//
//  WiFi Scanning
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_wifi_scan.h"

int WifiScan_First;

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
  
}

// ---------------------------------------------------------------------------
void init_WifiScan() {
  menu_wifi_scan = Menu_Create(fnt_dejavu_9, 126, 55);

  Menu_SetAutoIO(menu_wifi_scan, 1);
  Menu_SetBorder(menu_wifi_scan, BORDER_NONE);
  Menu_EnableImages(menu_wifi_scan, 0);
  Menu_SetScroll(menu_wifi_scan, SCROLL_AUTO);

  WifiScan_First = 1;
 
}


// ---------------------------------------------------------------------------
void draw_WifiScan() {
    Screen_DrawBorder(_lng(WIFI_SCAN_TITLE));
    
    if(WifiScan_First == 1) {
      // first draw screen, on second draw scan for wifi networks
      WifiScan_First = 2; 
    }
    
    else if(WifiScan_First == 2) {
       WifiScan_First = 0;
       doScan(); 
       return;
    }
    
    if(WifiScan_First == 0) {
      // draw menu 
      Menu_Draw(menu_wifi_scan, 1, 8);
    } else {
      // draw 'scanning...'
      GLCDD_Rect r;      
      
      r.x = SCREEN_W / 2 - GLCDD_StringWidth(fnt_dejavu_9b, _lng(SCANNING)) / 2;
      r.y = SCREEN_H / 2 - GLCDD_FontHeight(fnt_dejavu_9b) / 2;
      r.w = SCREEN_W - 4;
      r.h = -1;
      GLCDD_Print(fnt_dejavu_9b, &r, _lng(SCANNING));
    }
}


// ---------------------------------------------------------------------------
void exit_WifiScan() {
  Menu_Destroy(menu_wifi_scan);
}
