///////////////////////////////////////////////////////////////
//
//  Volume Control
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#ifndef _SCREEN_VOLUME_H_
#define _SCREEN_VOLUME_H_

#include "firmware.h"
#include <unistd.h>
#include <fcntl.h>
#include <alsa/asoundlib.h>



void init_Volume();
void draw_Volume();

#endif

