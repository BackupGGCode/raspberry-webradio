# Name: Makefile
# Project: raspberry pi webradio
# Author: Michael Schwarz
# Creation Date: 2012-08-31
# Copyright: (c) 2012 by Michael Schwarz

all: firmware.o menu.o lcd.o language.o lcd2bmp.o io.o ip.o disk.o screens.o settings.o arraylist.o
	gcc firmware.o menu.o lcd.o lcd2bmp.o language.o io.o ip.o disk.o screens.o settings.o arraylist.o -g -o firmware

firmware.o: firmware.c
	gcc firmware.c -c -g -o firmware.o

menu.o: menu.c
	gcc menu.c -c -g -o menu.o

lcd.o: lcd.c
	gcc lcd.c -c -g -o lcd.o

language.o: language.c
	gcc language.c -c -g -o language.o

lcd2bmp.o: lcd2bmp.c
	gcc lcd2bmp.c -c -g -o lcd2bmp.o

io.o: io.c
	gcc io.c -c -g -o io.o

ip.o: ip.c
	gcc ip.c -c -g -o ip.o

disk.o: disk.c
	gcc disk.c -c -g -o disk.o

screens.o: screens.c
	gcc screens.c -c -g -o screens.o

settings.o: settings.c
	gcc settings.c -c -g -o settings.o

arraylist.o: arraylist.c
	gcc arraylist.c -c -g -o arraylist.o


clean:
	rm *.o firmware


iosim: iosim.c
	gcc iosim.c -lcurses -g -o iosim

