///////////////////////////////////////////////////////////////
//
//  USB file browser
//
//  02.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#ifndef _SCREEN_USB_H_
#define _SCREEN_USB_H_

#include "firmware.h"

extern Menu_Info* menu_usb;
char usb_root[512];

int isSupportedExtension(char* path, ArrayList* file_ext);
int isFileExtension(char* name, char* ext);
int nameCompare(void* str1, void* str2);
void createUSBMenu();
void playUSB(char* filename);
void init_USB();
void draw_USB();

#endif

