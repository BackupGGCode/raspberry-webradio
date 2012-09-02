///////////////////////////////////////////////////////////
//
// Get IP address
//
// 24.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#ifndef _IP_H_
#define _IP_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int IP_Get(char* buffer);

#endif
