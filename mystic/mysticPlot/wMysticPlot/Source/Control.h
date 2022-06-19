#ifndef _CONTROL_
#define _CONTROL_

#include <stdio.h>
#include <string.h>


#include <windows.h>
#include "Paux.h"


enum {
	controlTypeButton,
	controlTypeRadio,
	controlTypeCheck,
	controlTypeVScroll,
	controlTypeHScroll,
	controlTypePopUpMenu,
	controlTypeEdit,
	controlTypeStatic
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
	char	controlName[256];
	HWND	controlWindow;
	HWND	myWindow;
	HDC		hdc;
//	GC		gc;
	uRect	controlRect;
	int		controlState;
	int		controlSide;
	int		controlType;
	int		controlView;
	int		controlWhere;
	long	controlGroup;
	long	controlMin;
	long	controlMax;
	long	controlValue;
	long	controlRef;
	controlInfo		controlData;
	struct controls	*controlNext;
	struct controls	*controlList;
	unsigned long c[8];
	int hide;
	long controlID;
	int doNotRemoveFromList;
};
typedef struct controls *controlPtr;


int DrawControl(controlPtr thisControl);

int TrackControl();

controlPtr ControlCreate(struct Icon *myIcon,uRect *controlRect,char *controlName,
		  int controlType,int controlView,long controlMin,
		  long controlMax,long controlValue,long controlRef,long controlGroup);
long GetControlValue(controlPtr control);
int SetControlValue(controlPtr control,long controlValue);
int SetControlMaximum(controlPtr control,long controlValue);
int GetControlText(controlPtr control,char *name);
int ControlDispose(controlPtr thisControl);
int MoveControl(controlPtr thisControl,int x,int y);
int SizeControl(controlPtr thisControl,long x,long y);
int ControlScrollWhere();
int FindControl();
int ControlGetRect();
int  ControlHide(controlPtr thisControl);
int  ControlShow(controlPtr thisControl);
int  HideControl(controlPtr thisControl);
int  ShowControl(controlPtr thisControl);
int  ControlColors(unsigned long *c,controlPtr thisControl);
long GetControlReference(controlPtr control);
long uGetControlValue(controlPtr control);
int uSetControlMaximum(controlPtr control,long controlValue);
int uSetControlValue(controlPtr control,long controlValue);
int mstrncpy(char *out,char *in,long n);
int ControlSetIDStart(long control);
controlPtr FindAnyWindowControl(controlPtr thisControl);
int uControlInsertMenuItem(controlPtr thisControl,char *name,int location);
long uControlCountMenuItems(controlPtr thisControl);
int uControlDeleteMenuItem(controlPtr thisControl,int item);

#endif

