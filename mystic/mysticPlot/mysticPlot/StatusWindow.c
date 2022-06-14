#define EXTERN22 extern
#include "Xappl.h"
#include "Control.h"
#include "fileDialog.h"
#include "xText.h"
#include "xStrings.h"

#include <string.h>

#define STRING_COUNT	1

struct tdata{
    controlPtr Control2;
    int Ok;
    int argc;
    char **argv;
    char *message;
    IconPtr myIconin;
    struct textData *Text;
    struct stringData *Strings[STRING_COUNT];
    uRect stringRect[STRING_COUNT];
};


static int doKeyPress(IconPtr myIcon,XEvent *myevent);
static int doButtonRelease(IconPtr myIcon,XEvent *myevent);
static int doConfigureNotify(IconPtr myIcon,XEvent *myevent);
static int doExpose(IconPtr myIcon,XEvent *myevent);
static int doButtonPress(IconPtr myIcon,XEvent *myevent);
static int doKeyPress(IconPtr myIcon,XEvent *myevent);
static int doControls(IconPtr myIcon,XEvent *myevent);
static int doClose(IconPtr myIcon);
static int doNULL(IconPtr myIcon);
static int paintThis(IconPtr myIcon);

char *strsave(char *,int tag);
int SetTime(struct timeval *curTime,long milliseconds);
int TimeUp(struct timeval *LastTime);   
int NotMyWindow(IconPtr myIcon,XEvent *event);

static int MemoryWarn(IconPtr myIcon);
static struct timeval LastTime;


IconPtr uStartStatusWindow(IconPtr myIcon,char *header);

int uStopStatusWindow(IconPtr StatusWindow);

int uWriteStatusWindow(IconPtr myIcon,char *message);
int uCheckStatusWindow(IconPtr StatusWindow,char *Message);

IconPtr uStartStatusWindow(IconPtr myIcon2,char *header)
{

	static uRect wRect={0,0,340,140};
	struct tdata *d;
	Window  mywindow;
	IconPtr myIcon;
	
	if(!xg.mydisplay)return NULL;

	//myIcon2=myIcon2;
	myIcon=CreateIcon(xViewType,&wRect);

	if(myIcon == NULL){
	    fprintf(stderr,"Could Not Open Window\n");
	    return NULL;
	}

	myIcon->pdoKeyPress=doKeyPress;
	myIcon->pdoButtonRelease=doButtonRelease;
	myIcon->pdoConfigureNotify=doConfigureNotify;
	myIcon->pdoExpose=doExpose;
	myIcon->pdoButtonPress=doButtonPress;
	myIcon->pdoKeyPress=doKeyPress;
	myIcon->pClose=doClose;
	myIcon->pNullEvent=doNULL;


	d=(struct tdata *)cMalloc(sizeof(struct tdata),8660);

	if(d == NULL){
	    fprintf(stderr,"Could Not allocate struct tdata\n");
	    return NULL;
	}

	zerol((char *)d,sizeof(struct tdata));

	d->argc = xg.argc;
	d->argv = xg.argv;

	myIcon->DWindow=(struct DumWindow*)d;

	mywindow=myIcon->myWindow;

	myIcon->gc = XCreateGC (xg.mydisplay, mywindow, 0, 0);

	XSetBackground (xg.mydisplay, myIcon->gc, myIcon->c[2]);
	XSetForeground (xg.mydisplay, myIcon->gc, myIcon->c[2]);

	XSetFont(xg.mydisplay,myIcon->gc,xg.font16);


	XSelectInput (xg.mydisplay, mywindow,
	                ButtonPressMask | KeyPressMask | ExposureMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask |
	                StructureNotifyMask | FocusChangeMask | KeymapStateMask);

	{
	    XSizeHints hint;

	    hint.x = 0;
	    hint.y = 0;
	    hint.width  =  wRect.xsize;
	    hint.height =  wRect.ysize;
	    hint.flags = PPosition | PSize;

	    XSetStandardProperties (xg.mydisplay, mywindow, header, header,
	        None, xg.argv, xg.argc, &hint);
	}




	{
	    controlPtr thisControl;
	    static uRect controlRect2={130,110,100,25};
	    char *name2;

	   name2="Cancel";

	    thisControl=ControlCreate(myIcon,&controlRect2,name2,
		  controlTypeButton,1,0L,
		  1L,0L,2000L,0L);
	    if(thisControl == NULL){
	        fprintf(stderr,"Warning thisControl == NULL\n");
	        goto OutOfHere;
	    }
	    d->Control2=thisControl;

	}

        d->stringRect[0].x=20;
        d->stringRect[0].xsize=150;
        d->stringRect[0].ysize=20;
        d->stringRect[0].y=20;

	{
	    struct textData *thisText;
	    static uRect cellRect={100,20,50,25};

	    cellRect.y=d->stringRect[0].y+20;
	    cellRect.xsize=100;
	    cellRect.ysize=d->stringRect[0].ysize;
	    cellRect.x=d->stringRect[0].x;

	    thisText=TextNew(&cellRect,mywindow);
	    if(thisText == NULL){
	       goto OutOfHere;
	    }
	    if(TextPut("Waiting For Data",thisText))goto OutOfHere;
	    d->Text=thisText;
	}
	/* map window to the screen */
	XMapRaised (xg.mydisplay, mywindow);
	MemoryWarn(myIcon);
	return myIcon;
OutOfHere:
	doClose(myIcon);
	return (IconPtr)NULL;	
}
int uStopStatusWindow(IconPtr myIcon)
{
	if(!myIcon)return 1;
	doClose(myIcon);
	return 0;
}
int uWriteStatusWindow(IconPtr myIcon,char *message)
{
	struct tdata *d;

	if(!myIcon || !message || !xg.mydisplay)return 1;
 	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;

	if(!TimeUp(&LastTime))return 0;

	if(d->Text){
	    if(TextPut(message,d->Text))return 1;
	    if(d->Text)TextUpdate(d->Text);
	}
	SetTime(&LastTime,1000);
	return 0;
}
int uCheckStatusWindow(IconPtr myIcon,char *Message)
{
	XEvent event;

	if(!myIcon || !xg.mydisplay)return 0;
	//Message=Message;
	if(XCheckMaskEvent(xg.mydisplay,StructureNotifyMask | EnterWindowMask |
	                                ButtonPressMask | ButtonReleaseMask | 
	                                LeaveWindowMask | ExposureMask ,&event)){
		myIcon->myevent = &event;
		if(NotMyWindow(myIcon,&event))return 0;
		switch(event.type){
		case ButtonPress:
		    if(doButtonPress(myIcon,&event)){
		        return 1;
		    }else{
		        return 0;
		    }
		case Expose:
		    doExpose(myIcon,&event);
		    break;
		case ConfigureNotify:
		    doConfigureNotify(myIcon,&event);
		break;
		case EnterNotify:
		    if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,xg.colors,4);
		break;
		}
	}
	myIcon->myevent = NULL;
	return 0;
}
static int MemoryWarn(IconPtr myIcon)
{
	XEvent event;
	int xold,yold;
	int igrabb;

	if(!myIcon || !xg.mydisplay)return 1;
	
	xold= -1;
	yold= -1;

	XGrabPointer(xg.mydisplay,myIcon->myWindow,(Bool)False,
		ButtonPressMask | ButtonReleaseMask,
		GrabModeAsync,GrabModeAsync,myIcon->myWindow,xg.Arrow,
		CurrentTime);
	igrabb=TRUE;
	SetTime(&LastTime,1000);
	while(1){
	/*
		XMaskEvent(xg.mydisplay,StructureNotifyMask | ButtonPressMask| KeyPressMask 
		     | ButtonReleaseMask | Button1MotionMask | ExposureMask ,&event);
	*/
		if(XCheckMaskEvent(xg.mydisplay,StructureNotifyMask | ButtonPressMask| KeyPressMask | EnterWindowMask
		     | ButtonReleaseMask | Button1MotionMask | ExposureMask ,&event)){
		}else{
		    doNULL(myIcon);
		    if(TimeUp(&LastTime)){
		        return 0;
		    }
		    continue;
		}
		myIcon->myevent = &event;
		if(NotMyWindow(myIcon,&event))continue;
		switch(event.type){
		case ButtonPress:
		    if(doButtonPress(myIcon,&event))goto OutOfHere;
		    break;
		case KeyPress:
		    doKeyPress(myIcon,&event);
		    break;
		case Expose:
		    doExpose(myIcon,&event);
		    if(igrabb)XUngrabPointer(xg.mydisplay,CurrentTime);
		    igrabb=FALSE;
		break;
		case ConfigureNotify:
		    doConfigureNotify(myIcon,&event);
		break;
		case EnterNotify:
		    if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,xg.colors,4);
		break;
		case MapNotify:
		break;
		case LeaveNotify:
		break;
		case MotionNotify:		   
		    if(xold != event.xmotion.x || yold != event.xmotion.y){
		    /*
			printf("%d %d %d %d\n",event.xmotion.x,event.xmotion.y,
			event.xmotion.x_root,event.xmotion.y_root);
		    */
		    }		    
		    xold = event.xmotion.x;
		    yold = event.xmotion.y;
		break;
		case ButtonRelease:
		    ;
		break;
		default:
		   ;
		   /*
		    printf("MemoryWarn  event.type %d MotionNotify %d\n",event.type,MotionNotify);
		   */
		}
	}
OutOfHere:
	myIcon->myevent = NULL;
	return 1;
}
static int doNULL(IconPtr myIcon)
{
	struct tdata *d;

	if(!myIcon || !xg.mydisplay)return 1;
	if(!myIcon->topWindow)return 0;
 	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;
	TextIdle(d->Text);
	/* Sleep2(50); */
	return 0;
}

static int doClose(IconPtr myIcon)
{
	struct tdata *d;
	int n;

	if(!myIcon || !xg.mydisplay)return 1;
	d=(struct tdata *)myIcon->DWindow;
	if(d){
	    if(myIcon->gc)XFreeGC(xg.mydisplay, myIcon->gc);
		myIcon->gc=NULL;
	    if(d->Control2)ControlDispose(d->Control2);
	    if(d->Text)TextDispose(d->Text);
	    for(n=0;n<STRING_COUNT;++n){
	        if(d->Strings[n])StringDispose(d->Strings[n]);
	    }
	    cFree((char *)d);
	}
	myIcon=RemoveIcon(myIcon);
	return 0;
}
static int doKeyPress(IconPtr myIcon,XEvent *myevent)
{
	struct tdata *d;
	char text[10];
	KeySym mykey;

	if(!myIcon || !xg.mydisplay)return 1;
	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;
	XLookupString (&myevent->xkey,text, 10, &mykey, 0);
	TextKey(text[0],d->Text);
	if(text[0] == 10 || text[0] == 13){
	    ;
	}
	return 0;
}
static int doButtonRelease(IconPtr myIcon,XEvent *myevent)
{
	//myIcon=myIcon;
	//myevent=myevent;
	return 0;
}
static int doConfigureNotify(IconPtr myIcon,XEvent *myevent)
{
	struct tdata *d;
	int ret;

	if(!myIcon || !xg.mydisplay)return 1;
	d=(struct tdata *)myIcon->DWindow;

	if(myevent->xconfigure.window != myIcon->myWindow)return 0;
	myIcon->windowRect.y=myevent->xconfigure.y;
	myIcon->windowRect.x=myevent->xconfigure.x;
	
	ret=(myIcon->windowRect.xsize == myevent->xconfigure.width) &&
	    (myIcon->windowRect.ysize == myevent->xconfigure.height);
	if(ret)return 0;
	
	myIcon->windowRect.xsize=myevent->xconfigure.width;
	myIcon->windowRect.ysize=myevent->xconfigure.height;
	if(!d)return 1;
	if(d->Control2){
	    MoveControl(d->Control2,myIcon->windowRect.xsize-100,
				    myIcon->windowRect.ysize-25);
	}

	{
	    int xsize;
	    if(d->Text){
	        xsize=myIcon->windowRect.xsize-d->Text->textRect.x-10;
		d->Text->textRect.xsize=xsize;
	    }
	}
	return 0;
}
static int doExpose(IconPtr myIcon,XEvent *myevent)
{

	if(!myIcon || !xg.mydisplay)return 1;

	if(myevent->xexpose.window == myIcon->myWindow){
	    if(myevent->xexpose.count != 0)return 0;
/*
	    printf("Expose xexpose.window %ld\n",myevent->xexpose.window);
*/
	    paintThis(myIcon);
	    XSetInputFocus(xg.mydisplay,myIcon->myWindow,RevertToPointerRoot,CurrentTime);
	}

	return 0;
}
static int paintThis(IconPtr myIcon)
{
	struct tdata *d;
	int n;

	if(!myIcon || !xg.mydisplay)return 1;
	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;

	XSetForeground (xg.mydisplay,myIcon->gc,  myIcon->c[2]);
	XSetBackground (xg.mydisplay,myIcon->gc,  myIcon->c[2]);
	    XFillRectangle(xg.mydisplay,myIcon->myWindow,
	               myIcon->gc,
	               0,0,
	               myIcon->windowRect.xsize,
	               myIcon->windowRect.ysize);

	    uDrawControls(myIcon);
	    if(d->Text)TextUpdate(d->Text);
	    for(n=0;n<STRING_COUNT;++n){
		if(d->Strings[n])StringUpdate(d->Strings[n]);
	    }

	return 0;
}
static int doButtonPress(IconPtr myIcon,XEvent *myevent)
{
	uPoint pt;
	struct tdata *d;

	if(!myIcon || !xg.mydisplay)return 1;
	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;

	uPointer(&pt,myIcon);

	CheckControlAndShiftKeys();

	XSetInputFocus(xg.mydisplay,myIcon->myWindow,RevertToPointerRoot,CurrentTime);
	myIcon->topWindow=TRUE;

 	if(PtInRect(&pt,&d->Text->textRect)) {
 	    TextClick(myevent,&pt,d->Text);
 	    return 0;
 	}

	return doControls(myIcon,myevent);
}
static int doControls(IconPtr myIcon,XEvent *myevent)
{
	controlPtr control;
	struct tdata *d;
	int part;

	if(!myIcon || !xg.mydisplay)return 1;
	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;

	if(!(part=FindControl(myevent,myIcon->myWindow,&control)))return 0;
	switch(part){

		case inButton:
	    	TrackControl(control,myevent,0l,0l);
	    	if(control == d->Control2){
	    	    if(control->controlValue){
			d->Ok=FALSE;
			return 1;
	    	    }else{
			d->Ok=TRUE;
	    	    }
		}else{
		    printf("TrackControl unknown %ld\n",control->controlValue);
		}
		break;


		default: ;
	}
	return 0;
}

