///////////////////////////////////////////////////////////
//
// On-Screen Keyboard
//
// 11.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#include "keyboard.h"

#define KEYBOARD_MAX_LEN 64

char Keyboard_Text[KEYBOARD_MAX_LEN];
int Keyboard_Len;
char* Keyboard_Layout[] = {"abcdefghijklm", "nopqrstuvwxyz", "\2 \5 \4 \3     \1"};
char* Keyboard_Layout_Upper[] = {"ABCDEFGHIJKLM", "NOPQRSTUVWXYZ", "\2 \6 \4 \3     \1"};
char* Keyboard_Layout_Special[] = {"1234567890.,-", "_!@#$%&=()+*?", "^|<>[]{}    \4"};
char** Keyboard_CurrentLayout;
const uint8_t* Keyboard_Special[] = {img_enter, img_space, img_back, img_numbers, img_upper, img_lower};
int Keyboard_Sel;
int Keyboard_Chars = 13;
int Keyboard_Y = 26;
int Keyboard_IsDone;

// ---------------------------------------------------------------------------
void Keyboard_Init() {
  strcpy(Keyboard_Text, "");
  Keyboard_Sel = 0;
  Keyboard_CurrentLayout = Keyboard_Layout;
  Keyboard_Len = 0;
  Keyboard_IsDone = 0;
}

// ---------------------------------------------------------------------------
void Keyboard_Draw() {
  int i, j, width, height;
  GLCDD_Rect r;

  GLCDD_ClearEx(1, 8, SCREEN_W - 2, SCREEN_H - 2);
  GLCDD_Rectangle(2, Keyboard_Y, SCREEN_W - 4, 64 - 2 - Keyboard_Y, 0);
  
  width = GLCDD_CharWidth(fnt_dejavu_9->name, 'W');
  height = GLCDD_FontHeight(fnt_dejavu_9);
  char current;
  
  // switch letter
  int8_t rotary = IO_GetRotaryValue();
  Keyboard_Sel += rotary;
  do {
    if(Keyboard_Sel < 0) Keyboard_Sel = Keyboard_Chars * 3 - 1;
    if(Keyboard_Sel >= Keyboard_Chars * 3) Keyboard_Sel = 0;
    current = Keyboard_CurrentLayout[Keyboard_Sel / Keyboard_Chars][Keyboard_Sel % Keyboard_Chars];
    if(current == ' ' || current == '\0') {
      if(rotary < 0) Keyboard_Sel--;
      else if(rotary > 0) Keyboard_Sel++;    
    }
  } while(current == ' ' || current == '\0');
  
  if(IO_GetButton(0)) {
    if(current == '\4') { // special <-> normal keyboard
      if(Keyboard_CurrentLayout == Keyboard_Layout_Special) Keyboard_CurrentLayout = Keyboard_Layout;
      else Keyboard_CurrentLayout = Keyboard_Layout_Special;
      Keyboard_Sel = 0;
    }
    
    else if(current > ' ') { // normal char
      Keyboard_Text[Keyboard_Len++] = current;
      Keyboard_Text[Keyboard_Len] = 0;
    }
    
    else if(current == '\3') { // backspace
      if(Keyboard_Len > 0) {
	Keyboard_Len--;
	Keyboard_Text[Keyboard_Len] = 0;
      }
    }
    
    else if(current == '\2') { // space
      Keyboard_Text[Keyboard_Len++] = ' ';
      Keyboard_Text[Keyboard_Len] = 0;
    }
    
    else if(current == '\5') { // upper
      Keyboard_CurrentLayout = Keyboard_Layout_Upper;
    }
    
    else if(current == '\6') { // lower
      Keyboard_CurrentLayout = Keyboard_Layout;
    }
    
    else if(current == '\1') { // enter
      Keyboard_IsDone = 1;
    }
    
    // prevent buffer overflow
    if(Keyboard_Len == KEYBOARD_MAX_LEN - 1) Keyboard_Len--;
  }
  
  // draw input field
  GLCDD_Line(4, Keyboard_Y - 4, SCREEN_W - 4, Keyboard_Y - 4);
  GLCDD_SetPixel(4, Keyboard_Y - 5);
  GLCDD_SetPixel(SCREEN_W - 4, Keyboard_Y - 5);
  
  // draw entered text
  r.x = 7;
  r.y = Keyboard_Y - 4 - height;
  r.w = SCREEN_W - 16;
  r.h = -1;
  char* text_ptr = Keyboard_Text;
  while(GLCDD_StringWidth(fnt_dejavu_9, text_ptr) > SCREEN_W - 16) text_ptr++;
  GLCDD_Print(fnt_dejavu_9, &r, text_ptr);
  r.x += GLCDD_StringWidth(fnt_dejavu_9, text_ptr);
  r.w += 4;
  GLCDD_Print(fnt_dejavu_9, &r, "|");
  
  // draw keyboard
  for(i = 0; i < Keyboard_Chars; i++) {
    for(j = 0; j < 3; j++) {
      int off = width - GLCDD_CharWidth(fnt_dejavu_9->name, Keyboard_CurrentLayout[j][i]);
      int x = i * (width + 2) + 5 + off/2;
      int y = Keyboard_Y + 2 + j * (height + 1);
      // normal character
      if(Keyboard_CurrentLayout[j][i] >= ' ') GLCDD_PutChar(fnt_dejavu_9->name, x, y, Keyboard_CurrentLayout[j][i]);
      else { // special key
	GLCDD_XBMDraw((uint8_t*)Keyboard_Special[Keyboard_CurrentLayout[j][i] - 1], x - off/2, y, 7, 10);
      }
      
      if(Keyboard_Sel == j * Keyboard_Chars + i) {
	GLCDD_Invert(x - off/2 - 1, y - 1, x + width - off/2, y + height);
      }
    }
  }
}


// ---------------------------------------------------------------------------
char* Keyboard_GetText() {
    return Keyboard_Text;
}

// ---------------------------------------------------------------------------
int Keyboard_IsConfirmed() {
    return Keyboard_IsDone;
}

