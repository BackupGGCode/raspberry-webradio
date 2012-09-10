///////////////////////////////////////////////////////////
//
// Disk functions
//
// 24.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _DISK_H_
#define _DISK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mntent.h>
#include <sys/vfs.h>
#include <getopt.h>
#include "macros.h"

int Disk_GetStat(char* path, long* size, int* used_perc);

#endif
