///////////////////////////////////////////////////////////////
//
//  GLCD Scrollable Menu
//
//  20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////

#include "menu.h"


// ---------------------------------------------------------------------------
Menu_Info* Menu_Create(GLCDD_Font* font, int width, int height) {
	Menu_Info* menu = (Menu_Info*)malloc(sizeof(Menu_Info));
	if(menu == NULL) return NULL;
	assert(font != NULL);
	
	menu->width = width;
	menu->height = height;
	menu->font = font;
	menu->items = NULL;
	menu->items_count = 0;
	menu->top_item = 0;
	menu->selected_item = 0;
	menu->items_per_page = (int)(height / (GLCDD_FontHeight(font) + 2));
	menu->border = BORDER_SOLID;
	menu->draw_lines = 1;
	menu->circular = 0;
	menu->auto_io = 0;
	menu->is_chosen = 0;
	menu->scroll = SCROLL_AUTO;
	menu->scroll_draw = 0;
	menu->scroll_size = 1;
	menu->scroll_step = 0;
	menu->has_images = 0;
	menu->max_tag_width = 0;
	return menu;
}


// ---------------------------------------------------------------------------
void Menu_Destroy(Menu_Info* menu) {
	if(menu == NULL) return;
	int i;
	for(i = 0; i < menu->items_count; i++) {
		free(menu->items[i].title);
 		//free(menu->items[i].tag);
		//free(menu->items[i]);
		//menu->items[i] = NULL;
	}
	free(menu->items);
	menu->items = NULL;
	menu->items_count = 0;
	free(menu);
}


// ---------------------------------------------------------------------------
int Menu_AddItem(Menu_Info* menu, char* item) {
	assert(menu != NULL);
	menu->items_count++;
	menu->items = (Menu_Item*)realloc(menu->items, menu->items_count * sizeof(Menu_Item));
	menu->items[menu->items_count - 1].title = (char*)malloc(sizeof(char) * (1 + strlen(item)));
	strcpy(menu->items[menu->items_count - 1].title, item);
	menu->items[menu->items_count - 1].tag = NULL;
	menu->items[menu->items_count - 1].image = NULL;
	menu->items[menu->items_count - 1].title_tag = '\0';
	
	// check if scrollbar is needed and calculate step and size
	if(menu->items_count > menu->items_per_page) {
		menu->scroll_draw = 1;
		menu->scroll_size = (menu->height - 8) / (menu->items_count - menu->items_per_page);
		if(menu->scroll_size <= 1) menu->scroll_size = 2;
		menu->scroll_step = (double)(menu->height - 6 - menu->scroll_size) / (double)(menu->items_count - menu->items_per_page);
	}
	
	return menu->items_count - 1;
}

// ---------------------------------------------------------------------------
void Menu_AddItemTag(Menu_Info* menu, int item, void* tag) {
	assert(menu != NULL);
	if(item >= menu->items_count || item < 0) return;
	menu->items[item].tag = tag;
}

// ---------------------------------------------------------------------------
void* Menu_GetItemTag(Menu_Info* menu, int item) {
	assert(menu != NULL);
	if(item >= menu->items_count || item < 0) return NULL;
	return menu->items[item].tag;
}

// ---------------------------------------------------------------------------
void Menu_AddItemImage(Menu_Info* menu, int item, const uint8_t* image, uint8_t width, uint8_t height) {
	assert(menu != NULL);
	if(item >= menu->items_count || item < 0) return;
	menu->items[item].image = (uint8_t*)image;
	menu->items[item].img_w = width;
	menu->items[item].img_h = height;
}

// ---------------------------------------------------------------------------
void Menu_EnableImages(Menu_Info* menu, uint8_t enable) {
	assert(menu != NULL);
	menu->has_images = enable;
}

// ---------------------------------------------------------------------------
void Menu_SetTitleTag(Menu_Info* menu, int item, char tag) {
	assert(menu != NULL);
	if(item >= menu->items_count || item < 0) return;
	menu->items[item].title_tag = tag;
	menu->max_tag_width = GLCDD_CharWidth((const uint8_t*)menu->font->name, tag);
	int i;
	for(i = 0; i < menu->items_count; i++) {
	  if(menu->items[i].title_tag != '\0' && GLCDD_CharWidth((const uint8_t*)menu->font->name, menu->items[i].title_tag) > menu->max_tag_width) {
	   menu->max_tag_width = GLCDD_CharWidth((const uint8_t*)menu->font->name, menu->items[i].title_tag);
	  }
	}
}

// ---------------------------------------------------------------------------
char* Menu_GetItemText(Menu_Info* menu, int item) {
    assert(menu != NULL);
    if(item >= menu->items_count || item < 0) return NULL;
    return menu->items[item].title;
}


// ---------------------------------------------------------------------------
void Menu_Draw(Menu_Info* menu, int x, int y) {
	// if auto_io is enabled, acquire rotary encoder and evaluate it
	if(menu->auto_io) {
		int8_t rotary = IO_GetRotaryValue();
		Menu_Scroll(menu, rotary);
		if(IO_GetButton(0)) {
			menu->is_chosen = 1 + menu->selected_item;
		}
	}

	// clear area
	if(menu->border == BORDER_SOLID || menu->border == BORDER_ROUNDED) GLCDD_ClearEx(x - 1, y - 1, x + menu->width, y + menu->height);
	else GLCDD_ClearEx(x, y, x + menu->width - 1, y + menu->height - 1);
	
	// draw items
	int i;
	int line_height = menu->height / menu->items_per_page;
	GLCDD_Rect r;
	r.x = x + 2 + (menu->has_images * line_height);
	r.w = menu->width - 10 - (menu->has_images * line_height) - menu->max_tag_width - 1;
	r.h = -1;
	for(i = 0; i < menu->items_per_page; i++) {
		int item_index = i + menu->top_item;
		if(item_index >= menu->items_count || item_index < 0) continue;
		
		int line_y = y + 2 + i * line_height;
		// draw item text
		r.y = line_y;
		GLCDD_Print(menu->font, &r, menu->items[item_index].title);
		// draw item image (if exists)
		if(menu->has_images && menu->items[item_index].image != NULL) {
			GLCDD_XBMDraw(menu->items[item_index].image, x + 2, r.y, 
				menu->items[item_index].img_w, menu->items[item_index].img_h);
		}		
		// draw title tag (if exists)
		if(menu->items[item_index].title_tag != '\0') {
			int tag_x = x + menu->width - 8 - menu->max_tag_width;
			int tag_y = r.y;
			GLCDD_PutChar((const uint8_t*)menu->font->name, 
				tag_x, tag_y, 
				menu->items[item_index].title_tag);
			GLCDD_Invert(tag_x - 1, tag_y - 1, tag_x + menu->max_tag_width, tag_y + line_height - 3);
		}
		// draw line
		if(menu->draw_lines) {
			GLCDD_Line(x + 1, line_y + line_height - 2, x + menu->width - 2, line_y + line_height - 2);
		}
		// invert if selected
		if(item_index == menu->selected_item) {
			GLCDD_Invert(x + 1, line_y - 1, x + menu->width - 2, line_y + line_height - 2);
		}
	}
	
	// draw scrollbar
	if(menu->scroll == SCROLL_ALWAYS || (menu->scroll == SCROLL_AUTO && menu->scroll_draw == 1)) {
		int scrollbar_x = x + menu->width - 1 - 4;
		int scrollbar_y = y + 2;
		int scrollbar_h = menu->height - 5;
		GLCDD_ClearEx(scrollbar_x - 2, scrollbar_y - 1, scrollbar_x + 4, scrollbar_y + scrollbar_h + 2);
		GLCDD_Line(scrollbar_x, scrollbar_y + 1, scrollbar_x, scrollbar_y + scrollbar_h - 1);
		GLCDD_Line(scrollbar_x + 2, scrollbar_y + 1, scrollbar_x + 2, scrollbar_y + scrollbar_h - 1);
		GLCDD_SetPixel(scrollbar_x + 1, scrollbar_y);
		GLCDD_SetPixel(scrollbar_x + 1, scrollbar_y + scrollbar_h);
		// scroller positoin
		int scroll_y = (int)((menu->top_item * menu->scroll_step) + 0.5);
		GLCDD_Line(scrollbar_x + 1, scrollbar_y + scroll_y, scrollbar_x + 1, scrollbar_y + scroll_y + menu->scroll_size);
	}
	
	// draw border
	if(menu->border == BORDER_SOLID || menu->border == BORDER_ROUNDED) {
		GLCDD_Rectangle(x, y, menu->width, menu->height, 0);
	}
	if(menu->border == BORDER_ROUNDED) {
		GLCDD_ClearPixel(x, y);
		GLCDD_ClearPixel(x + menu->width, y);
		GLCDD_ClearPixel(x, y + menu->height);
		GLCDD_ClearPixel(x + menu->width, y + menu->height);
	}
}

// ---------------------------------------------------------------------------
void Menu_Scroll(Menu_Info* menu, int offset) {
	assert(menu != NULL);
	
	// calculate selected item
	menu->selected_item += offset;
	if(menu->selected_item < 0) {
		if(menu->circular) menu->selected_item += menu->items_count;
		else menu->selected_item = 0;
	} else if(menu->selected_item >= menu->items_count) {
		if(menu->circular) menu->selected_item -= menu->items_count;
		else menu->selected_item = menu->items_count - 1;
	}
	
	// check if item is visible
	// selected item is too far at the bottom
	if(menu->selected_item - menu->top_item >= menu->items_per_page) {
		menu->top_item = menu->selected_item - menu->items_per_page + 1;
	}
	// selected item is too far at the top
	if(menu->top_item - menu->selected_item > 0) {
		menu->top_item = menu->selected_item;
	}
}

// ---------------------------------------------------------------------------
int Menu_GetSelectedItem(Menu_Info* menu) {
	assert(menu != NULL);
	return menu->selected_item;
}

// ---------------------------------------------------------------------------
void Menu_ScrollTo(Menu_Info* menu, int pos) {
	assert(menu != NULL);
	if(pos >= menu->items_count) return;
	if(pos < 0) return;
	menu->selected_item = pos;
	menu->top_item = pos;
}

// ---------------------------------------------------------------------------
void Menu_SetCircular(Menu_Info* menu, uint8_t circular) {
	assert(menu != NULL);
	menu->circular = circular;
}

// ---------------------------------------------------------------------------
void Menu_SetDrawLines(Menu_Info* menu, uint8_t lines) {
	assert(menu != NULL);
	menu->draw_lines = lines;
}

// ---------------------------------------------------------------------------
void Menu_SetBorder(Menu_Info* menu, Menu_Border border) {
	assert(menu != NULL);
	menu->border = border;
}

// ---------------------------------------------------------------------------
void Menu_SetAutoIO(Menu_Info* menu, uint8_t auto_io) {
	assert(menu != NULL);
	menu->auto_io = auto_io;
}

// ---------------------------------------------------------------------------
uint8_t Menu_GetAutoIO(Menu_Info* menu) {
	assert(menu != NULL);
	return menu->auto_io;
}

// ---------------------------------------------------------------------------
int Menu_IsChosen(Menu_Info* menu) {
	assert(menu != NULL);
	assert(menu->auto_io != 0);
	// nothing was selected
	if(menu->is_chosen == 0) return -1;
	// return selected id and clear chosen id
	int ret = menu->is_chosen - 1;
	menu->is_chosen = 0;
	return ret;
}

// ---------------------------------------------------------------------------
void Menu_SetScroll(Menu_Info* menu, Menu_Scrolling scroll) {
    menu->scroll = scroll;
}

// ---------------------------------------------------------------------------
int Menu_GetItems(Menu_Info* menu) {
	assert(menu != NULL);
	return menu->items_count;
}


