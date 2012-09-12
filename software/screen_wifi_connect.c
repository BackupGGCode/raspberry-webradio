///////////////////////////////////////////////////////////////
//
//  WiFi Connect
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////    

#include "screen_wifi_connect.h"

int WifiConnect_First;

// ---------------------------------------------------------------------------
void init_WifiConnect() {
#ifndef SIMULATE
  FILE* network = fopen("/etc/network/interfaces", "w");
  if(network == NULL) {
    printf("Error opening /etc/network/interfaces! Can't connect to WiFi\r\n");
    Screen_Goto(SCREEN_INFO);
    return;
  }
  fprintf(network, "auto lo\r\n\r\niface lo inet loopback\r\niface eth0 inet dhcp\r\n\r\n");
  fprintf(network, "allow-hotplug wlan0\r\n\r\n");
  fprintf(network, "auto wlan0\r\n\r\n");
  fprintf(network, "iface wlan0 inet dhcp\r\n");
  fprintf(network, "wpa-ssid \"%s\"\r\n", Settings_Get("wifi", "ssid"));
  fprintf(network, "wpa-psk \"%s\"\r\n", Settings_Get("wifi", "pwd"));    
  fclose(network);
#endif
  
  WifiConnect_First = 1;
}

// ---------------------------------------------------------------------------
void draw_WifiConnect() {
      Screen_DrawBorder(_lng(WIFI_CONNECTING));
      GLCDD_Rect r;      
      
      r.x = SCREEN_W / 2 - GLCDD_StringWidth(fnt_dejavu_9b, _lng(PLEASE_WAIT)) / 2;
      r.y = SCREEN_H / 2 - GLCDD_FontHeight(fnt_dejavu_9b) / 2;
      r.w = SCREEN_W - 4;
      r.h = -1;
      GLCDD_Print(fnt_dejavu_9b, &r, _lng(PLEASE_WAIT));
      
      // connect to wifi
      if(WifiConnect_First == 1) {
	// first draw the 'please wait' screen, on next redraw connect
	WifiConnect_First = 2;
      }
      
      else if(WifiConnect_First == 2) {
	WifiConnect_First = 0;
#ifndef SIMULATE
	ignore_result(system(Settings_Get("programs", "wifi_connect")));
#endif
	Screen_Goto(SCREEN_INFO);
      }
}
