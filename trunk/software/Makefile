# Name: Makefile
# Project: raspberry pi webradio
# Author: Michael Schwarz
# Creation Date: 2012-08-31
# Copyright: (c) 2012 by Michael Schwarz

OBJ=firmware.o menu.o lcd.o language.o lcd2bmp.o io.o ip.o disk.o screens.o settings.o arraylist.o screen_info.o screen_main.o screen_now_playing.o screen_stations.o screen_usb.o fonts/fnt_dejavu.o fonts/fnt_dejavu_9.o fonts/fnt_dejavu_9_b.o fonts/fnt_silkscreen_8.o images/info.o images/playing.o images/playlist.o images/song.o images/station.o images/up.o images/usb.o
TARGET=firmware
CFLAGS=-g -Wall
CC=gcc

all: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o firmware


iosim: iosim.c
	gcc iosim.c -lcurses -g -o iosim

