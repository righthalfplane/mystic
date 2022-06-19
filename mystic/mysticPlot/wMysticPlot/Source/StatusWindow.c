#define EXTERN22 extern

#include "Xappl.h"
#include "StatusWindow.h"
#include "Net.h"
IconPtr StartStatusWindow(char *header)
{
	IconPtr myIcon;
	static uRect wRect={40,40,400,100};
	statPtr s;

	 myIcon=CreateMyWindow(xStatusType,&wRect);

	/* myIcon=uCreateIcon(xStatusType,&wRect); */

	if(myIcon == NULL){
	   Warning("Could Not Open StartStatusWindow Window\n");
	   return (IconPtr)NULL;
	}

	s=(statPtr)cMalloc(sizeof(struct statStruct),9019);
	if(!s){
	    fprintf(stderr,"StartStatusWindow out of Memory\n");
	    goto ErrorOut;
	}
	zerol((char *)s,sizeof(struct statStruct));

	myIcon->DWindow=(struct DumWindow*)s;

	myIcon->pdoWriteStatus=WriteStatusWindow;

    myIcon->LeftAndRight = 
		CreateWindow(
        "BUTTON",
        "Stop",
        BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
        60,
        60,
        40,
        20,
        myIcon->myWindow,
        (HMENU)BUTTON_NUMBER,
        xg.hInst,
        NULL);

	SizeIt(myIcon);
	myIcon->isOpen=TRUE;
	InvalRectMyWindow(myIcon);
	return myIcon;
ErrorOut:
	RemoveIcon(myIcon);
	return NULL;
}
int WriteStatusWindow(IconPtr sw,char *mess)
{
	statPtr s;
	char str[256];

	if(!sw || !sw->hdc)return 1;

	s=(statPtr)sw->DWindow;

	SetBkColor(sw->hdc,RGB(240, 230, 230));

    sprintf(str,"%s%c",mess,0);

    TextOut(sw->hdc,16,10,str,strlen(str)); 
	
	if(s){
		if(s->lastMessage[0]){
			sprintf(str,"Last:%s%c",s->lastMessage,0);
			TextOut(sw->hdc,5,30,str,strlen(str)); 
		}
		strcpy(s->lastMessage,mess);
	}
	return 0;
}
int CheckStatusWindow(IconPtr myIcon)
{
	MSG msg;

	if(!myIcon)return 0;

	myIcon->hit=0;

	/*if(PeekMessage(&msg,myIcon->myWindow,0,0,PM_REMOVE)){ */
	if(PeekMessage(&msg,0,0,0,PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return myIcon->hit;
}
int StopStatusWindow(IconPtr myIcon)
{
	statPtr s;

	if(!myIcon)return 0;

	s=(statPtr)myIcon->DWindow;
	if(s)cFree((char *)s);
	myIcon->DWindow=NULL;
	RemoveIcon(myIcon);
	return 0;
}
int SizeIt(IconPtr win)
{
	RECT r;

	if(win){
		GetClientRect(win->myWindow,&r);
		win->windowRect.y=r.top;
		win->windowRect.x=r.left;
		win->windowRect.xsize=r.right-r.left;
		win->windowRect.ysize=r.bottom-r.top;
		MoveWindow(win->LeftAndRight,
			win->windowRect.xsize-80,
			win->windowRect.ysize-20,
			80,20,1);
	}
	return 0;
}
