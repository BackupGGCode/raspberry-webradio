# Name: Makefile
# Project: raspberry pi webradio
# Author: Michael Schwarz
# Creation Date: 2012-08-31
# Copyright: (c) 2012 by Michael Schwarz


WXFLAGS = `wx-config --cflags`
WXLIBS = `wx-config --libs`

all: simulator.o
	g++ simulator.o $(WXFLAGS) $(WXLIBS) -g -o simulator


simulator.o: simulator.cpp
	g++ simulator.cpp $(WXFLAGS) $(WXLIBS) -c -g -o simulator.o

