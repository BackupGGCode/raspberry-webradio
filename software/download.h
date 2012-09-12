///////////////////////////////////////////////////////////
//
// Function to Download a File
//
// 12.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

int Download_Get(const char* url, const char* destination);

#endif

