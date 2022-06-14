#ifndef _XSTINGS_
#define _XSTINGS_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <sys/time.h>

#include "xAux.h"

struct stringData{
    char stringName[256];
    Window myWindow; 
    uRect stringRect;
    int justification;
    int stringSide;
    unsigned long c[10];
    GC gc;
   int hide;
    XFontStruct *info;
};

#define StringLeft		0
#define StringRight		1
#define StringCenter	2

struct stringData *StringNew(char *stringName,uRect *stringRect,int justification,Window theWindow);

int StringUpdate(struct stringData *String);

int StringColors(unsigned long *c,struct stringData *String);

int StringDispose(struct stringData *String);

int StringGetRect(uRect *stringRect,struct stringData *String);

int StringFont(struct stringData *String,int num);

int StringMove(struct stringData *String,int ix,int iy);

#endif
