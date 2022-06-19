#ifndef _XSTINGS_
#define _XSTINGS_

//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>

//#include <sys/time.h>

//#include "Xaux.h"

#include <windows.h>
#include "Xappl.h"

struct stringData{
    char stringName[256];
    HWND myWindow; 
//    HMENU myMenu; 
    uRect stringRect;
    int justification;
    int stringSide;
    unsigned long c[8];
//    GC gc;
   int hide;
//    XFontStruct *info;
};

#define StringLeft		0
#define StringRight		1
#define StringCenter	2

struct stringData *StringNewMenu(char *stringName,uRect *stringRect,int justification,HWND theWindow);

int StringUpdate(struct stringData *String);

int StringColors(unsigned long *c,struct stringData *String);

int StringDispose(struct stringData *String);

int StringGetRect(uRect *stringRect,struct stringData *String);

int StringFont(struct stringData *String,int num);

int StringMove(struct stringData *String,int ix,int iy);

#endif
