all: stream.o usb_play.o
	gcc stream.o -lcurl -o stream
	gcc usb_play.o -o usb_play


stream.o: stream.c
	gcc stream.c -c -o stream.o
	
usb_play.o: usb_play.c
	gcc usb_play.c -c -o usb_play.o

clean:
	rm stream.o stream

