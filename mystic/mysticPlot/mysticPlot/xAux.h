#ifndef _XAUX_
#define _XAUX_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <unistd.h>

#include "ulibTypes.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



int Warning(char *Message);

EXTERN22 char WarningBuff[1024];

char *strsave(char *,int tag);  

int InRect(XEvent *event,uRect *controlRect)  ;

int OffsetRect(uRect *LineRect,int x,int y);

unsigned long int TickCount(void);

int EraseRect(uRect *Box,Window mywindow,GC gc);
int FrameRect(uRect *Box,Window mywindow,GC gc);
int MoveTo(int x,int y);
int LineTo(int x,int y,Window mywindow,GC gc);
int DrawString(unsigned char *name,Window mywindow,GC gc);

int PtInRect(uPoint *event,uRect *controlRect);
int CheckControlAndShiftKeys(void);
int DrawRect3D(uRect *controlRect,Window controlwindow,GC gc,
                         int controlState,int controlSide,unsigned long *c);
int SetBuffers(long Length) ;
extern int Pointer(uPoint *pt,Window window);
int Sleep2(int ms);      
int IsShift(void);
void Bell(int tag);
int getColors(unsigned long *c);
int GetTime(long *Seconds,long *milliseconds); 

extern int mstrncpy(char *out,char *in,long n);

extern int mstrncat(char *out,char *in,long n);

#endif

