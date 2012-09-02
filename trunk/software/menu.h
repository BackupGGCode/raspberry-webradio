///////////////////////////////////////////////////////////////
//
//  GLCD Scrollable Menu
//
//  20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////


#ifndef _GLCD_MENU_H_
#define _GLCD_MENU_H_

//#define NDEBUG // to disable asserts

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "lcd.h"
#include "io.h"

// ---------------------------------------------------------------------------
typedef enum {
	BORDER_NONE,
	BORDER_SOLID,
	BORDER_ROUNDED
} Menu_Border;

// ---------------------------------------------------------------------------
typedef enum {
	SCROLL_AUTO,
	SCROLL_NEVER,
	SCROLL_ALWAYS
} Menu_Scrolling;


typedef struct {
	char* title;
	void* tag;
	uint8_t* image;
	uint8_t img_w;
	uint8_t img_h;
	char title_tag;
} Menu_Item;

// ---------------------------------------------------------------------------
typedef struct {
  int width, height;
  int items_per_page, items_count;
  int top_item;
  int selected_item;
  Menu_Item* items;
  GLCDD_Font* font;  
  uint8_t draw_lines;
  Menu_Border border;
  uint8_t circular;
  uint8_t auto_io, is_chosen;
  Menu_Scrolling scroll;
  uint8_t scroll_draw;
  int  scroll_size;
  double scroll_step;
  uint8_t has_images;
  uint8_t max_tag_width;
} Menu_Info;

// ---------------------------------------------------------------------------

Menu_Info* Menu_Create(GLCDD_Font* font, int width, int height);
void Menu_Destroy(Menu_Info* menu);
int Menu_AddItem(Menu_Info* menu, char* item);
void Menu_Draw(Menu_Info* menu, int x, int y);
void Menu_Scroll(Menu_Info* menu, int offset);
int Menu_GetSelectedItem(Menu_Info* menu);
void Menu_ScrollTo(Menu_Info* menu, int pos);
void Menu_SetCircular(Menu_Info* menu, uint8_t circular);
void Menu_SetDrawLines(Menu_Info* menu, uint8_t lines);
void Menu_SetBorder(Menu_Info* menu, Menu_Border border);
void Menu_SetAutoIO(Menu_Info* menu, uint8_t auto_io);
int Menu_IsChosen(Menu_Info* menu);
void Menu_SetScroll(Menu_Info* menu, Menu_Scrolling scroll);
void Menu_AddItemTag(Menu_Info* menu, int item, void* tag);
void* Menu_GetItemTag(Menu_Info* menu, int item);
char* Menu_GetItemText(Menu_Info* menu, int item);
void Menu_AddItemImage(Menu_Info* menu, int item, const uint8_t* image, uint8_t width, uint8_t height);
void Menu_EnableImages(Menu_Info* menu, uint8_t enable);
int Menu_GetItems(Menu_Info* menu);
void Menu_SetTitleTag(Menu_Info* menu, int item, char tag);

#endif
