//
// Graphic Display KS0108 Bridge for Raspberry PI
//
// 14.08.2012
// 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "glcd.h"
#include "silk8fnt.h"

#include "boot.h"

#define VERSION "1.0"

volatile uint8_t ringbuffer[256];
volatile uint8_t recv = 0;
volatile uint8_t bit = 0;
volatile uint8_t read = 0;

ISR(INT2_vect) {
 if(PINB & (1 << PB1)) ringbuffer[recv] |= 1 << bit; else ringbuffer[recv] &=~(1 << bit);
 bit++;
 if(bit == 8) {
  recv++;
  bit = 0;
 }
}

int main() {
 // init interrupts
 MCUCSR |= (1 << ISC2);
 GICR |= (1 << INT2);

 // init
 GLCD_Init();
 GLCD_Clear();
 
 // draw bootscreen
 GLCDD_Clear();
 GLCDD_Rect(0, 0, 128, 64, 0);
 GLCDD_Rect(0, 0, 128, 7, 1);
 GLCDD_ClearPixel(0, 0);
 GLCDD_ClearPixel(127, 0);
 GLCDD_ClearPixel(127, 63);
 GLCDD_ClearPixel(0, 63);
 
 GLCDD_TTFPrintCol(GLCD_silk8, 40, -1, "STARTING...", 1);
 
 GLCDD_XBMDraw_P(boot_img, 34, 7, 59, 56);

 GLCD_Draw();  
 
 
 // enable bridge functionality
 sei();
  
 uint8_t x = 0, y = 0;
 GLCD_SetColumn(0);
 GLCD_SetPage(0);
 GLCD_SetColumn(0);

 while(1) {
   if(read != recv) {
    GLCD_SendData(ringbuffer[read]);
    read++;
    x++;
    if(x == 128) {
     x = 0;
     y++;
     GLCD_SetColumn(0);
     GLCD_SetPage(y);
    }
    
    if(y == 8) {
     y = 0;
     GLCD_SetColumn(0);
     GLCD_SetPage(y);
    }
   }
 }
  
}
