#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define REPEAT 35

void sendByte(unsigned char b) {
 int i;
 for(i = 0; i < 8; i++) {
   int j;
   for(j = 0; j < REPEAT; j++) {
    digitalWrite(8, (b & (1 << i)) ? 1 : 0);
   }
   
   for(j = 0; j < REPEAT; j++) {
    digitalWrite(7, 1); 
   }
   for(j = 0; j < REPEAT; j++) {
    digitalWrite(7, 0);
   }
 }
}

int main() {
  if(wiringPiSetupGpio() == -1)  exit(1);

  pinMode (7, OUTPUT);
  pinMode (8, OUTPUT);
  
  int j;
  int i;
  for(i = 0; i < 128*8; i++) {
   sendByte(0); 
  }


  unsigned int start = millis();
  for(j = 0; j < 128; j++) {
   for(i = 0; i < 128*8; i++) {
    sendByte(1 << ((i+j) % 8)); 
   }
  }
  unsigned int end = millis();
  printf("%d frames in %dms => %d FPS\n", j, end - start, j * 1000 / (end - start));

}
