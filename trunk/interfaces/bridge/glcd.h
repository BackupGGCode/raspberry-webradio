//
// Graphic Display Functions for KS0108 Controller
//
// 29.07.2008
//

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define GLCD_ENABLE_PIN    PC6
#define GLCD_ENABLE_PORT   PORTC
#define GLCD_ENABLE_PORTD  DDRC
#define GLCD_DI            PC7
#define GLCD_DI_PORT       PORTC
#define GLCD_DI_PORTD      DDRC
#define GLCD_CS1           PC5
#define GLCD_CS1_PORT      PORTC
#define GLCD_CS1_PORTD     DDRC
#define GLCD_CS2           PC4
#define GLCD_CS2_PORT      PORTC
#define GLCD_CS2_PORTD     DDRC

#define GLCD_DATA_PORT     PORTA
#define GLCD_DATA_PORTD    DDRA

#define GLCD_DELAY  6

#define GLCD_CS_1   0
#define GLCD_CS_2   1

// makros
#define GLCD_MODE_DATA() GLCD_DI_PORT |= (1 << GLCD_DI)
#define GLCD_MODE_INST() GLCD_DI_PORT &= ~(1 << GLCD_DI)
#define GLCD_CS(c) \
 if(c == GLCD_CS_1) {\
  GLCD_CS1_PORT |= (1 << GLCD_CS1);\
  GLCD_CS2_PORT &= ~(1 << GLCD_CS2);\
 } else {\
  GLCD_CS1_PORT &= ~(1 << GLCD_CS1);\
  GLCD_CS2_PORT |= (1 << GLCD_CS2);\
 }
#define GLCD_ENABLE() GLCD_ENABLE_PORT |= (1 << GLCD_ENABLE_PIN)
#define GLCD_DISABLE() GLCD_ENABLE_PORT &= ~(1 << GLCD_ENABLE_PIN)

// commands
#define GLCD_COLUMN_SET 0x40
#define GLCD_PAGE_SET   0xB8
#define GLCD_ON         0x3F
#define GLCD_OFF        0x3E
#define GLCD_STARTLINE  0xC0


uint8_t GLCD_Data[128*8];
uint8_t GLCD_Dirty[128*1];

uint8_t GLCD_Column, GLCD_Page;


void GLCD_SendCommand(uint8_t cmd, uint8_t cs) {
 GLCD_DISABLE();
 GLCD_MODE_INST();
 GLCD_CS(cs);
 GLCD_DATA_PORT = cmd;
 GLCD_ENABLE();
 _delay_us(GLCD_DELAY);
 GLCD_DISABLE();
 GLCD_DATA_PORT = 0;
 //_delay_us(GLCD_DELAY);
}

void GLCD_SendData(uint8_t data) {
 uint8_t cs;
 cs = (GLCD_Column > 63) ? GLCD_CS_2 : GLCD_CS_1;
 
 GLCD_DISABLE();
 GLCD_MODE_DATA();
 GLCD_CS(cs);
 GLCD_DATA_PORT = data;
 GLCD_ENABLE();
 _delay_us(GLCD_DELAY);
 GLCD_DISABLE();
 GLCD_DATA_PORT = 0;
 
 GLCD_Column++;
 //_delay_us(GLCD_DELAY);
}

void GLCD_SetColumn(uint8_t column) {
 GLCD_Column = column;
 if (column < 64) {
  GLCD_SendCommand(GLCD_COLUMN_SET | (column & 63), GLCD_CS_1);
  GLCD_SendCommand(GLCD_COLUMN_SET | 0, GLCD_CS_2);
 } else {
  GLCD_SendCommand(GLCD_COLUMN_SET | 63, GLCD_CS_1);
  GLCD_SendCommand(GLCD_COLUMN_SET | ((column - 64) & 63), GLCD_CS_2);
 }
}

void GLCD_SetPage(uint8_t y) {
 GLCD_SendCommand(GLCD_PAGE_SET | y, GLCD_CS_1);
 GLCD_SendCommand(GLCD_PAGE_SET | y, GLCD_CS_2); 
 GLCD_Page = y;  
}

void GLCD_SetStartLine(uint8_t line) {
 GLCD_SendCommand(GLCD_STARTLINE | (line & 63), GLCD_CS_1);
 GLCD_SendCommand(GLCD_STARTLINE | (line & 63), GLCD_CS_2);
}



void GLCD_Init() {
 GLCD_ENABLE_PORTD |= (1 << GLCD_ENABLE_PIN);
 GLCD_DI_PORTD |= (1 << GLCD_DI);
 GLCD_CS1_PORTD |= (1 << GLCD_CS1);
 GLCD_CS2_PORTD |= (1 << GLCD_CS2);
 GLCD_DATA_PORTD = 0xff;
  
 GLCD_DISABLE();
 GLCD_CS(GLCD_CS_1);
 GLCD_MODE_INST();

 GLCD_SendCommand(GLCD_ON, GLCD_CS_1);
 GLCD_SendCommand(GLCD_ON, GLCD_CS_2);
 GLCD_SetStartLine(0);   
 
 GLCD_SetPage(0);
 GLCD_SetColumn(0);
 GLCD_Page = 0;
 GLCD_Column = 0;
}


void GLCD_Clear() {
 uint8_t x, y;
 for(y = 0; y < 8; y++) {
  GLCD_SetColumn(0);  
  GLCD_SetPage(y);
  for(x = 0; x < 128; x++) {
   GLCD_SendData(0);      
  }      
 } 
 GLCD_SetColumn(0);
 GLCD_SetPage(0);
 GLCD_Column = 0;
 GLCD_Page = 0;
}

void GLCD_SetDirty(uint8_t x, uint8_t line) {
 uint8_t b;
 //b = x + ((y >> 3) << 7);
 b = x + (line << 7);
 GLCD_Dirty[b >> 3] |= (1 << (b & 7));
}

uint8_t GLCD_GetDirty(uint8_t x, uint8_t line) {
 uint8_t b;
 //b = x + ((y >> 3) << 7);
 b = x + (line << 7);
 return GLCD_Dirty[b >> 3] & (1 << (b & 7));
}

void GLCD_ClearDirty(uint8_t x, uint8_t line) {
 uint8_t b;
 b = x + (line << 7);
 GLCD_Dirty[b >> 3] &= ~(1 << (b & 7));
}

// drawing
void GLCDD_SetPixel(uint8_t x, uint8_t y) {
 GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] |= 1 << (y & 7);
 // mark as dirty
 GLCD_SetDirty(x, y>>3);
}

uint8_t GLCDD_GetPixel(uint8_t x, uint8_t y) {
 return (GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)]) & (1 << (y & 7));
}

void GLCDD_ClearPixel(uint8_t x, uint8_t y) {
 GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] &= ~(1 << (y & 7));  
 // mark as dirty
 GLCD_SetDirty(x, y >> 3);
}

void GLCDD_TogglePixel(uint8_t x, uint8_t y) {
 GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] ^= (1 << (y & 7));  
 // mark as dirty
 GLCD_SetDirty(x, y >> 3);
}

void GLCDD_Clear() {
 uint8_t x;
 for(x = 0; x < 128; x++) {
  GLCD_Data[x + 128*0] = 0;
  GLCD_Data[x + 128*1] = 0;
  GLCD_Data[x + 128*2] = 0;
  GLCD_Data[x + 128*3] = 0;
  GLCD_Data[x + 128*4] = 0;
  GLCD_Data[x + 128*5] = 0;
  GLCD_Data[x + 128*6] = 0;
  GLCD_Data[x + 128*7] = 0;
 }
}

int16_t GLCDD_abs(int16_t a) {
 if(a < 0) return -a; else return a;
}

void GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
 int8_t deltax, deltay, numpixels, d, dinc1, dinc2, xinc1, xinc2, yinc1, yinc2, x, y, i;
 deltax = GLCDD_abs(x2-x1);
 deltay = GLCDD_abs(y2-y1);

 if(deltax >= deltay) {
  numpixels = deltax+1;
  d = (2*deltay) - deltax;
  dinc1 = deltay << 1;
  dinc2 = (deltay-deltax) << 1;
  xinc1 = 1;
  xinc2 = 1;
  yinc1 = 0;
  yinc2 = 1;
 } else {
  numpixels = deltay+1;
  d = (2*deltax) - deltay;
  dinc1 = deltax << 1;
  dinc2 = (deltax-deltay) << 1;
  xinc1 = 0;
  xinc2 = 1;
  yinc1 = 1;
  yinc2 = 1;
 }

 if(x1 > x2) {
  xinc1 = -xinc1;
  xinc2 = -xinc2;
 }
 if(y1 > y2) {
  yinc1 = -yinc1;
  yinc2 = -yinc2;
 }

 x=x1;
 y=y1;

 for(i = 1; i <= numpixels; i++) {
  GLCDD_SetPixel(x, y);
  if(d < 0) {
   d = d + dinc1;
   x = x + xinc1;
   y = y + yinc1;
  } else {
   d = d + dinc2;
   x = x + xinc2;
   y = y + yinc2;
  }
 }
}

void GLCDD_Circle(uint8_t x, uint8_t y, uint8_t r, uint8_t filled) {
 uint8_t xCenter, yCenter, radius;
 int8_t p, xx, yy;
 
 xCenter = x;
 yCenter = y;
 radius = r;

 xx = 0;
 yy = radius;
 p = 3 - 2*radius;
       
 while(xx <= yy) {
  GLCDD_SetPixel(xCenter + xx, yCenter + yy);
  GLCDD_SetPixel(xCenter - xx, yCenter + yy);
  GLCDD_SetPixel(xCenter + xx, yCenter - yy);
  GLCDD_SetPixel(xCenter - xx, yCenter - yy);
  
  GLCDD_SetPixel(xCenter + yy, yCenter + xx);
  GLCDD_SetPixel(xCenter - yy, yCenter + xx);
  GLCDD_SetPixel(xCenter + yy, yCenter - xx);
  GLCDD_SetPixel(xCenter - yy, yCenter - xx);

  if(filled) {
   GLCDD_Line(xCenter - xx, yCenter + yy, xCenter + xx, yCenter + yy);
   GLCDD_Line(xCenter + xx, yCenter - yy, xCenter - xx, yCenter - yy);
   GLCDD_Line(xCenter + yy, yCenter + xx, xCenter - yy, yCenter + xx);
   GLCDD_Line(xCenter + yy, yCenter - xx, xCenter - yy, yCenter - xx);
  }

  if(p < 0) {
   xx++;
   p += 4*xx + 6;
  } else {
   xx++;
   yy--;
   p += 4*(xx-yy) + 10;
  }
 }
}

void GLCDD_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t filled) {
 uint8_t xx, yy;
 for(xx = x; xx < x+w; xx++) {
  for(yy = y; yy < y+h; yy++) {
   if(filled) {
    GLCDD_SetPixel(xx, yy);           
   } else {
    if(xx == x || xx == x+w-1 || yy == y || yy == y+h-1) {
     GLCDD_SetPixel(xx, yy);      
    }       
   }
  }       
 }
}

void GLCDD_HFlip(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
 uint8_t xx, yy, pt;
 for(yy = 0; yy < h; yy++) {
  for(xx = 0; xx < w/2; xx++) {
   pt = GLCDD_GetPixel(xx + x, yy + y);
   if(GLCDD_GetPixel(x + (w-xx) - 1, yy + y)) GLCDD_SetPixel(xx + x, yy + y); else GLCDD_ClearPixel(xx + x, yy + y);
   if(pt) GLCDD_SetPixel(x + (w-xx) - 1, yy + y); else GLCDD_ClearPixel(x + (w-xx) - 1, yy + y);
  }       
 }   
}

void GLCDD_VFlip(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
 uint8_t xx, yy, pt;
 for(yy = 0; yy < h/2; yy++) {
  for(xx = 0; xx < w; xx++) {
   pt = GLCDD_GetPixel(xx + x, yy + y);
   if(GLCDD_GetPixel(xx + x, y + (h-yy) - 1)) GLCDD_SetPixel(xx + x, yy + y); else GLCDD_ClearPixel(xx + x, yy + y);
   if(pt) GLCDD_SetPixel(xx + x, y + (h-yy) - 1); else GLCDD_ClearPixel(xx + x, y + (h-yy) - 1);
  }       
 }   
}

register uint8_t GLCD_ftmp asm("r2");
uint8_t GLCD_ByteMirror(uint8_t b) {
  /*
  return ((((b & 1) << 3) | ((b & 2) << 2) | ((b & 4) << 1) | ((b & 8))) << 4) | ((b & 16) >> 1) | ((b & 32) >> 3) | ((b & 64) >> 5) | ((b & 128) >> 7);
*/
 asm volatile("push r2\n\t");
 GLCD_ftmp = b;
 asm volatile("push r3\n\t"
 "clr r3\n\t"
 // 8bits
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "rol r2\n\t"
 "ror r3\n\t"
 "mov r24, r3\n\t"
 "pop r3\n\t"
 "pop r2\n\t"
 "clr r25\n\t");
}

void GLCDD_Rot180() {
 uint8_t i, n, pt;
 for(n = 0; n < 8; n++) {
  for(i = 0; i < 64; i++) {
   pt = GLCD_Data[(127-i) + n*128];
   GLCD_Data[(127-i) + n*128] = GLCD_Data[i + n*128];
   GLCD_Data[i + n*128] = pt;
   GLCD_Dirty[i] = 255;
  }
 }
 for(n = 0; n < 4; n++) {
  for(i = 0; i < 128; i++) {
   pt = GLCD_Data[i + (7-n)*128];
   GLCD_Data[i + (7-n)*128] = GLCD_ByteMirror(GLCD_Data[i + n*128]);
   GLCD_Data[i + n*128] = GLCD_ByteMirror(pt);
   GLCD_Dirty[i] = 255;
  }
 }
}

void GLCD_SetBit(uint8_t* d, uint8_t b) {
 d[b >> 3] |= (1 << (b & 7));
}

void GLCD_ClearBit(uint8_t* d, uint8_t b) {
 d[b >> 3] &= ~(1 << (b & 7));
}

uint8_t GLCD_GetBit(uint8_t* d, uint8_t b) {
 return (d[b >> 3] & (1 << (b & 7))) > 0;
}

void GLCDD_Rot90(uint8_t x, uint8_t y, uint8_t l) {
#define P1 (xx + x), y
#define P4 (x), ((len-xx) + y - 1)
#define P3 ((len-xx) + x - 1), ((len) + y - 1)
#define P2 ((len) + x - 1), (y + xx)

 uint8_t xx, yy, pt;
 int8_t len = l;
 while(len >= 0) {
  for(xx = 0; xx < len-1; xx++) {
   pt = GLCDD_GetPixel(P1); // save 1
   if(GLCDD_GetPixel(P4)) GLCDD_SetPixel(P1); else GLCDD_ClearPixel(P1); // 4 -> 1
   if(GLCDD_GetPixel(P3)) GLCDD_SetPixel(P4); else GLCDD_ClearPixel(P4); // 3 -> 4
   if(GLCDD_GetPixel(P2)) GLCDD_SetPixel(P3); else GLCDD_ClearPixel(P3); // 2 -> 3
   if(pt) GLCDD_SetPixel(P2); else GLCDD_ClearPixel(P2); // 1 -> 2
  }
  len -= 2;
  x++;
  y++;
 }
}

void GLCDD_Invert(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
 uint8_t xx, yy;
 for(xx = x; xx < x+w; xx++) {
  for(yy = y; yy < y+h; yy++) {
   //if(!LCDD_GetPixel(xx, yy)) LCDD_SetPixel(xx, yy); else LCDD_ClearPixel(xx, yy);
   GLCDD_TogglePixel(xx, yy);
  }
 }   
}

void GLCDD_ClearEx(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
 uint8_t xx, yy;
 for(xx = x; xx < x+w; xx++) {
  for(yy = y; yy < y+h; yy++) {
   if(GLCDD_GetPixel(xx, yy)) GLCDD_ClearPixel(xx, yy);       
  }       
 }   
}

uint8_t GLCDD__TTFChar(uint8_t* PROGMEM font, uint8_t x, uint8_t y, uint8_t c, uint8_t simonly, uint8_t inv) {
 // true type font test
 uint8_t height, bpr, bcc, width, _x, _y, from, to, j, i;
 uint16_t f = (uint16_t)font;
 
 height = pgm_read_byte(f++);
 from = pgm_read_byte(f++);
 to = pgm_read_byte(f++);
 
 // skip characters
 for(j = from; j < c; j++) {
  bpr = pgm_read_byte(f++);
  f++; //fgetc(f); // real width
  for(i = 0; i < bpr*height; i++) f++; //fgetc(f);
 }
 
 bpr = pgm_read_byte(f++);
 width = pgm_read_byte(f++);
 for(_y = 0; _y < height; _y++) {
  for(_x = 0; _x < bpr; _x++) {        
   bcc = pgm_read_byte(f++);
   for(j = 0; j < 8; j++) {
    if((bcc & (1 << j)) && (_x*8 + j < width)) {
     if(!simonly) {
      if(!inv) GLCDD_SetPixel(x + _x*8 + j, y + _y); else GLCDD_ClearPixel(x + _x*8 + j, y + _y);
     }
    }      
   }       
  }
 }
 return width;   
}

uint8_t GLCDD_TTFCharWidth(uint8_t* PROGMEM font, uint8_t c) {
 return GLCDD__TTFChar(font, 0, 0, c, 1, 0);
}

uint8_t GLCDD_TTFCharGen(uint8_t* PROGMEM font, uint8_t x, uint8_t y, uint8_t c, uint8_t inv) {
 return GLCDD__TTFChar(font, x, y, c, 0, inv);
}


uint8_t GLCDD_TTFPrintGen(uint8_t* PROGMEM font, uint8_t x, uint8_t y, uint8_t* text, uint8_t inv) {
 uint8_t _x;
 _x = x;
 while(*text != 0) {
  _x += GLCDD_TTFCharGen(font, _x, y, *text, inv);
  text++;
 }
 return _x-x; 
}

uint8_t GLCDD_TTFPrintGen_P(uint8_t* PROGMEM font, uint8_t x, uint8_t y, uint8_t* PROGMEM text, uint8_t inv) {
 uint8_t _x;
 _x = x;
 while(pgm_read_byte(text) != 0) {
  _x += GLCDD_TTFCharGen(font, _x, y, pgm_read_byte(text), inv);
  text++;
 }
 return _x-x; 
}

uint8_t GLCDD_TTFIntGen(uint8_t* PROGMEM font, uint8_t x, uint8_t y, int16_t i, uint8_t simonly, uint8_t inv) {
 uint8_t buffer[5];
 uint8_t ptr = 4, _x = x;

 if(i < 0) {
  _x += GLCDD__TTFChar(font, _x, y, '-', simonly, inv);
  i = -i;
 }
 if(i == 0) {
  _x += GLCDD__TTFChar(font, _x, y, '0', simonly, inv); 
  return _x - x;
 }
 do {
  buffer[ptr] = i % 10;
  i = i / 10;
  ptr--;
 } while(i > 0);
 do {
  ptr++; 
  _x += GLCDD__TTFChar(font, _x, y, buffer[ptr] + '0', simonly, inv);
 } while(ptr < 4);
 return _x - x;
}

#define GLCDD_TTFChar(fnt, x, y, c) GLCDD_TTFCharGen(fnt, x, y, c, 0)
#define GLCDD_TTFPrint(fnt, x, y, txt) GLCDD_TTFPrintGen(fnt, x, y, txt, 0)
#define GLCDD_TTFCharCol(fnt, x, y, c, col) GLCDD_TTFCharGen(fnt, x, y, c, col)
#define GLCDD_TTFPrintCol(fnt, x, y, txt, col) GLCDD_TTFPrintGen(fnt, x, y, txt, col)
#define GLCDD_TTFInt(fnt, x, y, i) GLCDD_TTFIntGen(fnt, x, y, i, 0, 0)
#define GLCDD_TTFIntCol(fnt, x, y, i, col) GLCDD_TTFIntGen(fnt, x, y, i, 0, col)
#define GLCDD_TTFIntWidth(fnt, i) GLCDD_TTFIntGen(fnt, 0, 0, i, 1, 0)

#define GLCDD_TTFPrint_P(fnt, x, y, txt) GLCDD_TTFPrintGen_P(fnt, x, y, txt, 0)
#define GLCDD_TTFPrintCol_P(fnt, x, y, txt, col) GLCDD_TTFPrintGen_P(fnt, x, y, txt, col)


uint8_t GLCDD_TTFTextWidth(uint8_t* PROGMEM font, uint8_t* text) {
 uint8_t w = 0;
 while(*text != 0) {
  w += GLCDD_TTFCharWidth(font, *text);
  text++;
 }
 return w;
}

uint8_t GLCDD_TTFTextWidth_P(uint8_t* PROGMEM font, uint8_t* PROGMEM text) {
 uint8_t w = 0;
 while(pgm_read_byte(text) != 0) {
  w += GLCDD_TTFCharWidth(font, pgm_read_byte(text));
  text++;
 }
 return w;
}

void GLCDD_XBMDraw_P(uint8_t* PROGMEM img, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
 uint8_t __x, __y, __b, __rw;
 if((w & 7) == 0) __rw = w;
 else __rw = ((w >> 3) + 1) << 3;
 for(__y = 0; __y < h; __y++) {
  for(__x = 0; __x < w; __x++) {
   __b = pgm_read_byte(img + (__x + __y*__rw)/8);
   if(__b & (1 << ((__x + __y*__rw) & 7))) GLCDD_SetPixel(x + __x, y + __y);
  }
 }
}




void GLCD_Draw() {
 uint8_t x, y, rc;
 for(y = 0; y < 8; y++) {
  GLCD_SetColumn(0);
  GLCD_SetPage(y);
  rc = 0;
  for(x = 0; x < 128; x++) {
   if(GLCD_GetDirty(x, y)) {
    if(rc != x) {
     GLCD_SetColumn(x);
     rc = x;      
    }
    GLCD_SendData(GLCD_Data[x + (y << 7)]);
    //GLCD_DataPrev[x + (y << 7)] = LCD_Data[x + (y << 7)];      
    rc++;
   }
  }      
 }   
}

