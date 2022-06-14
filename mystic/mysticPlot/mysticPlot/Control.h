#ifndef _CONTROL_
#define _CONTROL_

#include <stdio.h>
#include <string.h>

#include "xAux.h"

enum {
	controlTypeButton,
	controlTypeRadio,
	controlTypeCheck,
	controlTypeVScroll,
	controlTypeHScroll,
	controlTypePopUpMenu
};

enum {
    inButton=1,
    inPageUp,
    inPageDown,
    inUpButton,
    inDownButton,
    inThumb

};

typedef struct controlInfoStruct{
   struct MenuBar *menubar;
   struct Menu *menu;
} controlInfo;

struct controls{
    char			controlName[256];
    Window			controlwindow;
    GC				gc;
    uRect			controlRect;
    int				controlState;
    int				controlSide;
    int				controlType;
    int				controlView;
    int				controlWhere;
    long			controlGroup;
    long			controlMin;
    long			controlMax;
    long			controlValue;
    long			controlRef;
    controlInfo		controlData;
    struct controls	*controlNext;
    struct controls	*controlList;
    unsigned long c[10];
    int hide;
    XFontStruct *info;
};

typedef  struct controls *controlPtr;

controlPtr ControlCreate(struct Icon *myIcon,uRect *controlRect,char *controlName,
		  int controlType,int controlView,long controlMin,
		  long controlMax,long controlValue,long controlRef,long controlGroup);

int DrawControl(controlPtr thisControl);

int TrackControl(controlPtr control,XEvent *myevent,long (*controlRoutine)(controlPtr control,short part,void *controlRoutineData),void *controlRoutineData);

long uGetControlValue(controlPtr control);
int uSetControlValue(controlPtr control,long controlValue);
int HideControl(controlPtr control);
int ShowControl(controlPtr control);
int uSetControlMaximum(controlPtr control,long controlValue);
int ControlDispose(controlPtr thisControl);
int ControlDisposeWindow(controlPtr thisControl);
int MoveControl(controlPtr thisControl,int x,int y);
int SizeControl(controlPtr thisControl,int xsize,int ysize);
int ControlScrollWhere(controlPtr control,XEvent *myevent);
int FindControl(XEvent *myevent,Window win,controlPtr *control);
int ControlGetRect(uRect *rect,controlPtr control);
int  ControlColors(unsigned long *c,controlPtr thisControl);
long GetControlReference(controlPtr control);
#endif

