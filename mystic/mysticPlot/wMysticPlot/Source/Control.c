#define EXTERN extern
#include <windows.h>
#include <windowsx.h>

#include "xMenu.h"
#include "Control.h"
#include "Tools.h"
enum {
	arrowLeft,
	arrowRight,
	arrowUp,
	arrowDown,
	arrowIn,
	arrowOut
};


int DrawControlScroll(controlPtr thisControl);

int AddControlToWindow(IconPtr myIcon,controlPtr thisControl);

int DrawControlButton(controlPtr thisControl);
int DrawControlVScroll(controlPtr thisControl);
int DrawArrow3D();
int DrawControlRadio(controlPtr thisControl);
int SetThumbRect();
int controlSetThumb();
int AdjustButtons(controlPtr control);
int Sleep2(int ms);
int DrawControlCheck(controlPtr thisControl);
int RemoveControlFromWindow(controlPtr thisControl);

static long controlID;

int GetControlText(controlPtr control,char *name)
{
	if(!control || !name)return 1;
	if(control->controlType == controlTypeEdit){
	    GetDlgItemText(control->myWindow,control->controlID,name,256);
	}
	return 0;
}
int uControlInsertMenuItem(controlPtr tc,char *name,int location)
{
	int count;

	if(!tc)return 1;
	if(tc->controlType != controlTypePopUpMenu)return 1;

	ComboBox_AddString(tc->controlWindow,name) ;

	count=ComboBox_GetCount(tc->controlWindow);

	ComboBox_SetCurSel(tc->controlWindow,0);
	ComboBox_SetText(tc->controlWindow,name);

	return 0;
}
long uControlCountMenuItems(controlPtr tc)
{
	long count;
	if(!tc)return -1L;
	if(tc->controlType != controlTypePopUpMenu)return -1l;

	count=ComboBox_GetCount(tc->controlWindow);

	return count;
}
int uControlDeleteMenuItem(controlPtr tc,int item)
{
	if(!tc)return 1;
	if(tc->controlType != controlTypePopUpMenu)return 1;
	ComboBox_DeleteString(tc->controlWindow,item);
	return 0;
}


controlPtr ControlCreate(IconPtr myIcon,uRect *controlRect,char *controlName,
		  int controlType,int controlView,long controlMin,
		  long controlMax,long controlValue,long controlRef,long controlGroup)
{
	controlPtr thisControl;

	thisControl=(controlPtr)cMalloc(sizeof(struct controls),8370);
	if(thisControl == NULL)return (controlPtr)NULL;
	zerol((char *)thisControl,sizeof(struct controls));
	
	switch (controlType) {
	case controlTypeVScroll:
		thisControl->controlWindow = 
			CreateWindow(
			"SCROLLBAR",
			"",
			SBS_VERT | WS_CHILD | WS_VISIBLE,
			10,
			10,
			20,
			120,
			myIcon->myWindow,
			NULL,
			xg.hInst,
			NULL);
		myIcon->UpAndDown = thisControl->controlWindow;
		break;
	case controlTypeHScroll:
		thisControl->controlWindow = 
			CreateWindow(
			"SCROLLBAR",
			"",
			SBS_HORZ | WS_CHILD | WS_VISIBLE,
			10,
			10,
			120,
			20,
			myIcon->myWindow,
			NULL,
			xg.hInst,
			NULL);
		myIcon->LeftAndRight = thisControl->controlWindow;
		break;
		
	case controlTypeCheck:
		thisControl->controlWindow =
			CreateWindow(
			"BUTTON",
			controlName,
			BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,
			controlRect->x,
			controlRect->y,
			controlRect->xsize,
			controlRect->ysize,
			myIcon->myWindow,
			(HMENU)controlID,
			xg.hInst,
			NULL);
		break;
		
	case controlTypeButton:
		thisControl->controlWindow =
			CreateWindow(
			"BUTTON",
			controlName,
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			controlRect->x,
			controlRect->y,
			controlRect->xsize,
			controlRect->ysize,
			myIcon->myWindow,
			(HMENU)controlID,
			xg.hInst,
			NULL);
		break;
	case controlTypeRadio:
		thisControl->controlWindow =
			CreateWindow(
			"BUTTON",
			controlName,
			BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE,
			controlRect->x,
			controlRect->y,
			controlRect->xsize,
			controlRect->ysize,
			myIcon->myWindow,
			(HMENU)controlID,
			xg.hInst,
			NULL);
		break;

	case controlTypePopUpMenu:
		thisControl->controlWindow =
			CreateWindow(
			"COMBOBOX",
			controlName,
			CBS_DROPDOWNLIST  | CBS_DISABLENOSCROLL | WS_VSCROLL | WS_CHILD | WS_VISIBLE,
			controlRect->x,
			controlRect->y,
			controlRect->xsize,
			controlRect->ysize,
			myIcon->myWindow,
			(HMENU)controlID,
			xg.hInst,
			NULL);
		break;
		
	case controlTypeStatic:		
		thisControl->controlWindow =
			CreateWindow(
			"STATIC",
			controlName,
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			controlRect->x,
			controlRect->y,
			controlRect->xsize,
			controlRect->ysize,
			myIcon->myWindow,
			(HMENU)controlID,
			xg.hInst,
			NULL);
		break;
		
	case controlTypeEdit:
		thisControl->controlWindow =
			CreateWindowEx(
			WS_EX_CLIENTEDGE,
			"EDIT",
			controlName,
			ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE,
			controlRect->x,
			controlRect->y,
			controlRect->xsize,
			controlRect->ysize,
			myIcon->myWindow,
			(HMENU)controlID,
			xg.hInst,
			NULL);
		break;
		
	default:
		break;
	}
	
	if (myIcon->hfont) {
		SendMessage(thisControl->controlWindow,WM_SETFONT,(WPARAM)myIcon->hfont,MAKELPARAM(TRUE,0));
	}
	
	thisControl->controlRect = *controlRect;
	
	if(controlName)mstrncpy(thisControl->controlName,controlName,255);
	thisControl->myWindow=myIcon->myWindow;
	thisControl->controlType=controlType;
	thisControl->controlView=controlView;
	thisControl->controlMin=controlMin;
	thisControl->controlMax=controlMax;
	thisControl->controlValue=controlValue;
	thisControl->controlRef=controlRef;
	thisControl->controlGroup=controlGroup;
	thisControl->controlID = controlID++;
	if(controlType == controlTypeButton){
		thisControl->controlSide=4;
	}else if(controlType == controlTypeCheck || 
		controlType == controlTypeVScroll || 
		controlType == controlTypeHScroll){
		thisControl->controlSide=2;
	}else if(controlType == controlTypeRadio){
		thisControl->controlSide=3;
	}

	if(AddControlToWindow(myIcon,thisControl)){
	    return (controlPtr)NULL;
	}

	return thisControl;
}
int  ControlSetIDStart(long control)
{
	controlID = control;
	return 0;
}
int  HideControl(controlPtr thisControl)
{
		return ControlHide(thisControl);
}
int  ControlHide(controlPtr thisControl)
{
	if (!thisControl) return 1;
	thisControl->hide = TRUE;
	ShowWindow(thisControl->controlWindow,SW_HIDE);
	return 0;
}
int  ShowControl(controlPtr thisControl)
{
	return ControlShow(thisControl);
}
int  ControlShow(controlPtr thisControl)
{
	if (!thisControl) return 1;
	thisControl->hide = FALSE;
	ShowWindow(thisControl->controlWindow,SW_SHOW);
	return 0;
}
int ControlColors(unsigned long *c,controlPtr thisControl)
{
	return 0;
}
int ControlDispose(controlPtr thisControl)
{
	if(!thisControl)return 1;

	if(!thisControl->doNotRemoveFromList)RemoveControlFromWindow(thisControl);
	if(thisControl->controlWindow) {
		DestroyWindow(thisControl->controlWindow);
	}
	cFree((char *)thisControl);
	return 0;
}
int DrawControl(controlPtr thisControl)
{
	if(!thisControl)return 1;

	if(thisControl->hide)return 0;

	if(thisControl->controlType == controlTypeButton){
	    DrawControlButton(thisControl);
	} else if(thisControl->controlType == controlTypeCheck){
	    DrawControlCheck(thisControl);
	} else if(thisControl->controlType == controlTypeVScroll){
	    DrawControlScroll(thisControl);
	} else if(thisControl->controlType == controlTypeHScroll){
	    DrawControlScroll(thisControl);
	} else if(thisControl->controlType == controlTypeRadio){
	    DrawControlRadio(thisControl);
	}
	return 0;
}

int DrawControlScroll(controlPtr thisControl)
{
	RECT r;
    if(!thisControl)return 1;
	GetClientRect(thisControl->controlWindow,&r);
    InvalidateRect(thisControl->controlWindow,&r,FALSE);
	return 0;
}
int DrawArrow3D()
{
	return 0;

}
int DrawControlRadio(controlPtr thisControl)
{
	return 0;
}
int DrawControlCheck(controlPtr thisControl)
{
	return 0;
}
int DrawControlButton(controlPtr thisControl)
{
//	ShowWindow(thisControl->myWindow,SW_SHOW);
//	UpdateWindow(thisControl->myWindow);
//	RedrawWindow(thisControl->myWindow,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
//	ShowWindow(thisControl->controlWindow,SW_SHOW);
//	UpdateWindow(thisControl->controlWindow);
	return 0;
}
int DrawRect3D()
{
	return 0;
}

int TrackControl()
{
	return 0;
}
int FindControl()
{
	return 0;
}

int AdjustButtons(controlPtr control)
{
	return 0;
	
}
int InRect()
{
	return 1;
}
int SetThumbRect()
{
	return 0;
}
int controlSetThumb()
{
	return 0;
	
}
int ControlScrollWhere()
{
	return 0;
}
long GetControlReference(controlPtr control)
{
  return 0;
}
int ControlGetRect()
{
	return 0;
}
int MoveControl(controlPtr c,int x,int y)
{	
	if(!c)return 1;
	if(!c->controlWindow)return 1;

	if(c->controlType != controlTypePopUpMenu){
		MoveWindow(c->controlWindow,x,y,c->controlRect.xsize,c->controlRect.ysize,1);
	}else{
		MoveWindow(c->controlWindow,x,y,c->controlRect.xsize,8*c->controlRect.ysize,1);
	}
	c->controlRect.x = x;
	c->controlRect.y = y;
	return 0;
}
int SizeControl(controlPtr c,long xsize,long ysize)
{
	if(!c)return 1;
	if(!c->controlWindow)return 1;

	if(c->controlType != controlTypePopUpMenu){
		MoveWindow(c->controlWindow,c->controlRect.x,c->controlRect.y,(int)xsize,(int)ysize,1);
	}else{
		MoveWindow(c->controlWindow,c->controlRect.x,c->controlRect.y,(int)xsize,(int)8*ysize,1);
	}
	c->controlRect.xsize = (int)xsize;
	c->controlRect.ysize = (int)ysize;
	return 0;
}
int SetControlMaximum(controlPtr control,long controlValue)
{
	return 0;
}
int SetControlValue(controlPtr control,long controlValue)
{
	return 0;
}
long GetControlValue(controlPtr control)
{
	return 0;
}
long uGetControlMaximum(controlPtr control)
{
	if(!control)return 0;

	return control->controlMax;
}
int uSetControlMaximum(controlPtr control,long controlValue)
{
	SCROLLINFO si;

	if(!control)return 1;

	control->controlMax=controlValue;
	if(control->controlValue > control->controlMax){
	    control->controlValue=control->controlMax;
	}

	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_RANGE | SIF_DISABLENOSCROLL;
	si.nMin=0;
	si.nMax=controlValue;
	SetScrollInfo(control->controlWindow,SB_CTL,&si,1);

	return 0;
}
int uSetControlValue(controlPtr control,long controlValue)
{
	SCROLLINFO si;

	if(!control)return 1;
	if(controlValue < control->controlMin){
	    controlValue=control->controlMin;
	}else if(controlValue > control->controlMax){
		controlValue=control->controlMax;
	}
	control->controlValue=controlValue;
	
	switch(control->controlType) {
	case controlTypeVScroll:
	case controlTypeHScroll:
		si.cbSize=sizeof(SCROLLINFO);
		si.fMask=SIF_POS;
		si.nPos=controlValue;
		SetScrollInfo(control->controlWindow,SB_CTL,&si,1);
		break;
	case controlTypePopUpMenu:	
		ComboBox_SetCurSel(control->controlWindow,controlValue-1);						
		break;
	case controlTypeRadio:
	case controlTypeCheck:
		SendDlgItemMessage(control->myWindow,control->controlID,BM_SETCHECK,controlValue,0);
		break;
	}
	return 0;
}
long uGetControlValue(controlPtr control)
{
	SCROLLINFO si;
	int vpos;

	if(!control)return -1;

	vpos = 0;
	switch(control->controlType) {
	case controlTypeVScroll:
	case controlTypeHScroll:
		si.cbSize=sizeof(SCROLLINFO);
		si.fMask=SIF_POS;				
		GetScrollInfo(control->controlWindow,SB_CTL,&si);
		vpos=si.nPos;
		break;
	case controlTypePopUpMenu:	
		vpos=ComboBox_GetCurSel(control->controlWindow)+1;						
		control->controlValue=vpos;
		break;
	case controlTypeRadio:
	case controlTypeCheck:
		vpos=SendDlgItemMessage(control->myWindow,control->controlID,BM_GETCHECK,0,0);
		control->controlValue=vpos;
		break;
	}

	return vpos;
}
