///////////////////////////////////////////////////////////////
//
//  WiFi Authentification
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////   

#include "screen_wifi_auth.h"

void init_WifiAuth() {
    Keyboard_Init();
}


void draw_WifiAuth() {
    Screen_DrawBorder(_lng(WIFI_ENTER_PWD));

    Keyboard_Draw();
}
