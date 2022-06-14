#ifndef _XTEXT_
#define _XTEXT_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <sys/time.h>

#include "xAux.h"

struct textData{
    char textName[256];
    struct timeval LastTime;
    Window myWindow; 
    uRect textRect;
    int DoubleClick;
    unsigned long LastClick;
    int start;
    int end;
    int firstSelected;
    int lastSelected;
    int textCursor;
    int textDelete;
    int textBackspace;
    int textSide;
    int xOffSet;
    GC gc;
    GC gcX;
    XFontStruct *info;
};

struct textData *TextNew(uRect *cellRect,Window theWindow);

int TextPut(char *text,struct textData *Text);

int TextGet(char *text,struct textData *Text);

int TextSelect(struct textData *Text);

int TextDeSelect(struct textData *Text);

int TextUpdate(struct textData *Text);

int TextClick(XEvent *myevent,uPoint *pt,struct textData *Text);

int TextKey(int c,struct textData *Text);

int TextIdle(struct textData *Text);

int TextDispose(struct textData *Text);

int TextGetRect(uRect *cellRect,struct textData *Text);

int TextMove(struct textData *Text,int ix,int iy);

int TextSize(struct textData *Text,int xsize,int ysize);

#endif
