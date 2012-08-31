#ifndef __LCD_HD44780
#define __LCD_HD44780

#include <avr/pgmspace.h>

#define LCD_REG_PORT     PORTD
#define LCD_REG          PD4
#define LCD_REG_PORTD    DDRD
#define LCD_ENABLE_PORT  PORTD
#define LCD_ENABLE       PD5
#define LCD_ENABLE_PORTD DDRD
#define LCD_DATA_PORT    PORTD
#define LCD_DATA_PORTD   DDRD
#define LCD_DATA1        PD0
#define LCD_DATA2        PD1
#define LCD_DATA3        PD2
#define LCD_DATA4        PD3

#define LB(x) (x & 0xf)
#define HB(x) ((x & 0xf0) >> 4)


#define LCDCURSOR_HIDE 0
#define LCDCURSOR_SHOW 2
#define LCDCURSOR_BLINK 1
#define LCDCURSOR_NOBLINK 0
#define LCDDISPLAY_ON 4
#define LCDDISPLAY_OFF 0


void LCDCmd8(uint8_t cmd);
void LCDCmd4(uint8_t b_l, uint8_t b_h);
void LCDData(uint8_t b_l, uint8_t b_h);
void LCDInit(void);
void LCDSetData(uint8_t byte);
void LCDClk(void);
void LCDClear(void);
void LCDChar(uint8_t c);
void LCDPrint(uint8_t *s);


void LCDInit() {
 LCD_REG_PORTD |= (1 << LCD_REG);
 LCD_ENABLE_PORTD |= (1 << LCD_ENABLE);
 LCD_DATA_PORTD |= (1 << LCD_DATA1) | (1 << LCD_DATA2) | (1 << LCD_DATA3) | (1 << LCD_DATA4);
 _delay_ms(20);
 
 LCDCmd8(48);
 _delay_ms(5);
 LCDCmd8(48);
 _delay_ms(5);
 LCDCmd8(2); // set 4bit mode

 LCDCmd4(2, 8);       // 4 bit data mode
 LCDCmd4(0, 12);      // turn on display and cursor
 LCDCmd4(0, 6);       // entry mode
 LCDCmd4(0, 1);       // cls
 return;
}

void LCDSetData(uint8_t byte) {
 if(byte & 1) LCD_DATA_PORT |= (1 << LCD_DATA1); else LCD_DATA_PORT &= ~(1 << LCD_DATA1);
 if(byte & 2) LCD_DATA_PORT |= (1 << LCD_DATA2); else LCD_DATA_PORT &= ~(1 << LCD_DATA2);
 if(byte & 4) LCD_DATA_PORT |= (1 << LCD_DATA3); else LCD_DATA_PORT &= ~(1 << LCD_DATA3);
 if(byte & 8) LCD_DATA_PORT |= (1 << LCD_DATA4); else LCD_DATA_PORT &= ~(1 << LCD_DATA4);
 return;
}

inline void LCDClk() {
 LCD_ENABLE_PORT |= (1 << LCD_ENABLE);
 _delay_ms(1);
 LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE);
 _delay_ms(1);
}

void LCDCmd8(uint8_t byte) {
 LCD_REG_PORT &= ~(1 << LCD_REG);
 LCDSetData(byte);
 LCDClk();
}

void LCDCmd4(uint8_t byte_l, uint8_t byte_h) {
 LCD_REG_PORT &= ~(1 << LCD_REG);
 LCDSetData(byte_l);
 LCDClk();
 LCDSetData(byte_h);
 LCDClk();
}

void LCDCmd(uint8_t cmd) {
 LCDCmd4(HB(cmd), LB(cmd));
}

void LCDData(uint8_t d_l, uint8_t d_h) {
 LCD_REG_PORT |= (1 << LCD_REG);
 LCDSetData(d_h);
 LCDClk();
 LCDSetData(d_l);
 LCDClk();
}

// functions used by user

void LCDClear() {
 LCDCmd4(0, 1);
}
 
void LCDChar(uint8_t c) {
 LCDData(LB(c), HB(c));
}
 
void LCDPrint(uint8_t *s) {
 while(*s > 0) {
  LCDData(LB(*s), HB(*s));
  s++;
 }
}

void LCDPrint_P(const char* PROGMEM s) {
 uint16_t p = (uint16_t)s;
 char b;
 while((b = pgm_read_byte(p)) > 0) {
  //b = pgm_read_byte(p);
  LCDData(LB(b), HB(b));
  p++;
 }
}

void LCDInt(int16_t x) {
 char buffer[5];
 uint8_t ptr = 4;

 if(x < 0) {
  LCDChar('-');
  x = -x;
 }
 if(x == 0) {LCDChar('0'); return;}
 do {
  buffer[ptr] = x % 10;
  x = x / 10;
  ptr--;
 } while(x > 0);
 do {
  ptr++; 
  LCDChar(buffer[ptr]+48);
 } while(ptr < 4);
}

void LCDUInt(uint16_t x) {
 char buffer[5];
 uint8_t ptr = 4;

 if(x == 0) {LCDChar('0'); return;}
 do {
  buffer[ptr] = x % 10;
  x = x / 10;
  ptr--;
 } while(x > 0);
 do {
  ptr++; 
  LCDChar(buffer[ptr]+48);
 } while(ptr < 4);
}




void LCDPos(uint8_t row, uint8_t col) {
 uint8_t c;
 c = 128 | (row << 6) | col;
 LCDCmd4(HB(c), LB(c));
}

void LCDLoadChar(uint8_t id, uint8_t *data, uint8_t height) {
 LCDCmd(64 | (id<<3));
 uint8_t i;
 for(i = 0; i < height; i++) {
  LCDChar(data[i]);
 }
 //LCDData(0, 0);
 /*
 for(; i < 8; i++) {
  LCDData(0, 0);
 }
 */
 LCDCmd(128);
}

#endif

