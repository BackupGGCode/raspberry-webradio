///////////////////////////////////////////////////////////////
//
//  Shutdown
//
//  11.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_shutdown.h"

// ---------------------------------------------------------------------------
void init_Shutdown() {
    ignore_result(system("sync"));
    GLCDD_BacklightReset();
    ignore_result(system("shutdown -P now"));
    GLCDD_BacklightReset();
}

// ---------------------------------------------------------------------------
void draw_Shutdown() {
    GLCDD_Rect r;
    r.x = SCREEN_W / 2 - GLCDD_StringWidth(fnt_dejavu_9b, _lng(GOODBYE)) / 2;
    r.y = SCREEN_H / 2 - GLCDD_FontHeight(fnt_dejavu_9b) / 2;
    r.w = SCREEN_W;
    r.h = -1;
    GLCDD_Print(fnt_dejavu_9b, &r, _lng(GOODBYE));
}

