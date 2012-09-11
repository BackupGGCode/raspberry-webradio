///////////////////////////////////////////////////////////////
//
//  WiFi Scanning
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#ifndef _SCREEN_WIFI_SCAN_H_
#define _SCREEN_WIFI_SCAN_H_

#include "firmware.h"

Menu_Info* menu_wifi_scan;

void init_WifiScan();
void draw_WifiScan();
void exit_WifiScan();

#endif

