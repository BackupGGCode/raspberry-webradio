all: stream.o usb_play.o settings.o
	gcc stream.o settings.o -lcurl -o stream
	gcc usb_play.o settings.o -o usb_play


stream.o: stream.c 
	gcc stream.c -c -o stream.o
	
usb_play.o: usb_play.c 
	gcc usb_play.c -c -o usb_play.o

settings.o: ../../software/settings.c
	gcc ../../software/settings.c -c -o settings.o

clean:
	rm stream.o stream

