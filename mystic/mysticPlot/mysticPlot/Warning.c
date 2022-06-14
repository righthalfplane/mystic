#define EXTERN22 extern
#include "Xappl.h"
#include "Control.h"
#include "fileDialog.h"

#include <string.h>
#include "uGraph.h"

struct tdata{
    controlPtr Control1;
    controlPtr Control2;
    int Ok;
    int argc;
    char **argv;
    char *message;
    XFontStruct *info;
};

int NotMyWindow(IconPtr myIcon,XEvent *event);

static int doKeyPress(IconPtr myIcon,XEvent *myevent);
static int doButtonRelease(IconPtr myIcon,XEvent *myevent);
static int doConfigureNotify(IconPtr myIcon,XEvent *myevent);
static int doExpose(IconPtr myIcon,XEvent *myevent);
static int doButtonPress(IconPtr myIcon,XEvent *myevent);
static int doKeyPress(IconPtr myIcon,XEvent *myevent);
static int doControls(IconPtr myIcon,XEvent *myevent);
static int doClose(IconPtr myIcon);
char *strsave(char *,int tag);

static int MemoryWarn(IconPtr myIcon);

extern int IsControl(void);
extern int IsShift(void);
extern int IsOption(void);

int WriteMessage(IconPtr myIcon);

int Warning2(char *message,int flag);

int Warning(char *message)
{
	return Warning2(message,0);
}
int Warning2(char *message,int flag)
{

	static uRect wRect={0,0,240,140};
	struct tdata *d;
	Window  mywindow;
	int Ok;

	if(!message)return 1;
	
	if(!xg.mydisplay){
	    fprintf(stderr,"%s\n",message);
	    return 0;
	}
	
	CheckControlAndShiftKeys();
	
	if(IsShift() || IsControl() || IsOption())return 0;

	myIcon=CreateIcon(xViewType,&wRect);

	if(myIcon == NULL){
	    fprintf(stderr,"Could Not Open Window\n");
	    return 1;
	}

	myIcon->pdoKeyPress=doKeyPress;
	myIcon->pdoButtonRelease=doButtonRelease;
	myIcon->pdoConfigureNotify=doConfigureNotify;
	myIcon->pdoExpose=doExpose;
	myIcon->pdoButtonPress=doButtonPress;
	myIcon->pdoKeyPress=doKeyPress;
	myIcon->pClose=doClose;


	d=(struct tdata *)cMalloc(sizeof(struct tdata),8330);

	if(d == NULL){
	    fprintf(stderr,"Could Not allocate struct tdata\n");
	    return 1;
	}

	zerol((char *)d,sizeof(struct tdata));

	d->argc = xg.argc;
	d->argv = xg.argv;

	myIcon->DWindow=(struct DumWindow*)d;

	mywindow=myIcon->myWindow;

	myIcon->gc = XCreateGC (xg.mydisplay, mywindow, 0, 0);

	{
	    unsigned long c[10];

	    if(!getColors(c)){
			XSetBackground (xg.mydisplay, myIcon->gc, c[2]);
			XSetForeground (xg.mydisplay, myIcon->gc, c[2]);
	    }
	}



	XSetFont(xg.mydisplay,myIcon->gc,xg.font16);

	d->info=XQueryFont(xg.mydisplay,XGContextFromGC(myIcon->gc));
	if(!d->info)goto ErrorOut;

	XSelectInput (xg.mydisplay, mywindow,
	                ButtonPressMask | KeyPressMask | ExposureMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask | PointerMotionMask |
	                StructureNotifyMask | FocusChangeMask | KeymapStateMask);

	{
	    XSizeHints hint;
	    static char *name="Warning Alert";

	    hint.x = 0;
	    hint.y = 0;
	    hint.width  =  wRect.xsize;
	    hint.height =  wRect.ysize;
	    hint.flags = PPosition | PSize;

	    XSetStandardProperties (xg.mydisplay, mywindow, name, name,
	        None, xg.argv, xg.argc, &hint);
	}




	{
	    controlPtr thisControl;
	    static uRect controlRect2={130,110,100,25};
	    char *name1,*name2;

	   name1="Ok";
	   name2="Cancel";

	    thisControl=ControlCreate(myIcon,&controlRect2,name1,
		  controlTypeButton,1,0L,
		  1L,0L,2000L,0L);
	    if(thisControl == NULL){
	        fprintf(stderr,"Warning thisControl == NULL\n");
	        return 1;
	    }
	    d->Control1=thisControl;
	    if(flag == 1){
	        thisControl=ControlCreate(myIcon,&controlRect2,name2,
		      controlTypeButton,1,0L,
		      1L,0L,2000L,0L);
	        if(thisControl == NULL){
	            fprintf(stderr,"Warning thisControl == NULL\n");
	            return 1;
	        }
	        d->Control2=thisControl;
	    }

	}

	/* map window to the screen */
	XMapRaised (xg.mydisplay, mywindow);
        if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,xg.colors,4);

	 d->message=message;

	MemoryWarn(myIcon);
	Ok=d->Ok;
	doClose(myIcon);
	if(Ok){
	   return 0;   
	}
	return 1;
ErrorOut:
	return 1;
}
static int MemoryWarn(IconPtr myIcon)
{
	XEvent event;
	int xold,yold;
	int ex;

	Bell(61);
	xold= -1;
	yold= -1;
	ex=TRUE;
	ex=FALSE;
	while(1){
		XMaskEvent(xg.mydisplay,ButtonPressMask | KeyPressMask | ExposureMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask | PointerMotionMask |
	                StructureNotifyMask | FocusChangeMask | KeymapStateMask ,&event);
		myIcon->myevent = &event;
		if(NotMyWindow(myIcon,&event))continue;
		switch(event.type){
		case ButtonPress:
		    if(doButtonPress(myIcon,&event))goto OutOfHere;
		case Expose:
		    doExpose(myIcon,&event);
		    
		    if(ex){
		        XGrabPointer(xg.mydisplay,myIcon->myWindow,(Bool)True,
		        ButtonPressMask | ButtonReleaseMask,
		        GrabModeAsync,GrabModeAsync,myIcon->myWindow,xg.Arrow,
		        CurrentTime);
		        ex=FALSE;
		    }
		    
		break;
		case ConfigureNotify:
		    doConfigureNotify(myIcon,&event);
		break;
		case EnterNotify:
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

static int doClose(IconPtr myIcon)
{
	struct tdata *d;

	if(!myIcon)return 1;
	d=(struct tdata *)myIcon->DWindow;
	if(d){
	    if(myIcon->gc)XFreeGC(xg.mydisplay, myIcon->gc);
		myIcon->gc=NULL;
	    if(d->Control1)ControlDispose(d->Control1);
	    if(d->Control2)ControlDispose(d->Control2);
	    if(d->info)XFreeFontInfo(NULL, d->info, 1);
	    cFree((char *)d);
	}
	myIcon=RemoveIcon(myIcon);
	return 0;
}
static int doKeyPress(IconPtr myIcon,XEvent *myevent)
{
	int done,i;
	char text[10];
	KeySym mykey;

	//myIcon=myIcon;

	i = XLookupString (&myevent->xkey,text, 10, &mykey, 0);
	done = 0;
	if (i == 1 && text[0] == 'q')done = 1;
	if(done){
	    doQuit();
	}
	return done;
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

	if(!myIcon)return 1;
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
	    if(d->Control1){
	        MoveControl(d->Control1,0,
				    myIcon->windowRect.ysize-25);
	    }

	}else if(d->Control1){
	    MoveControl(d->Control1,myIcon->windowRect.xsize-100,
				    myIcon->windowRect.ysize-25);
	}

	return 0;
}
static int doExpose(IconPtr myIcon,XEvent *myevent)
{
	Window  mywindow;
	struct tdata *d;

	mywindow=myIcon->myWindow;
	d=(struct tdata *)myIcon->DWindow;

	if(myevent->xexpose.window == mywindow){
	    if(myevent->xexpose.count != 0)return 0;
/*
	    printf("Expose xexpose.window %ld\n",myevent->xexpose.window);
*/

	{
	    unsigned long c[10];

	    if(!getColors(c)){
			XSetBackground (xg.mydisplay, myIcon->gc, c[2]);
			XSetForeground (xg.mydisplay, myIcon->gc, c[2]);
	    }
	}



	    XFillRectangle(xg.mydisplay,mywindow,
	               myIcon->gc,
	               0,0,
	               myIcon->windowRect.xsize,
	               myIcon->windowRect.ysize);

	    uDrawControls(myIcon);
	    if(d->message){
			WriteMessage(myIcon);
	    }

	    XSetInputFocus(xg.mydisplay, mywindow,RevertToPointerRoot,CurrentTime);
	}

	return 0;
}
int WriteMessage(IconPtr myIcon)
{
	XFontStruct *info;
	struct tdata *d;
	char out[256],*ps,*pe,*po,*pp;
	int x,y;
	int nc,w;

	if(!myIcon)return  1;
	d=(struct tdata *)myIcon->DWindow;
	if(!d)return 1;
	info=d->info;
	if(!info){
	    fprintf(stderr,"StringUpdate info NULL failed\n");
	    return 1;
	}

	{
	    unsigned long c[10];

	    if(!getColors(c)){
			XSetBackground (xg.mydisplay, myIcon->gc, c[2]);
			XSetForeground (xg.mydisplay, myIcon->gc, c[4]);
	    }
	}




	/* MoveTo(25,stringRect.y+stringRect.ysize/2+info->ascent/2); */
	x=25;
	y=info->ascent+info->descent+10;
	nc=(int)strlen(d->message);
	ps=(char *)d->message;
	while(ps < d->message+nc){
	   MoveTo(x,y);
	   pe=strchr(ps,' ');
	   po=out;
	   if(!pe){
			pe=d->message+nc;
	   }
	   for(pp=ps;pp<=pe;++pp){
		  *po++ = *pp;
	   }
		*po++ = 0;
		w=XTextWidth(info,out,(int)strlen(out));
		if(x+w > myIcon->windowRect.xsize){
		   x=25;
		   y += (info->ascent+info->descent);
		}
		uMoveTo(x,y);
		uDrawString((char *)out,myIcon);
		x += w;
		ps=pe+1;
	}
	return 0;
}
static int doButtonPress(IconPtr myIcon,XEvent *myevent)
{

	CheckControlAndShiftKeys();

	return doControls(myIcon,myevent);
}
static int doControls(IconPtr myIcon,XEvent *myevent)
{
	controlPtr control;
	struct tdata *d;
	int part;

	d=(struct tdata *)myIcon->DWindow;


	if(!(part=FindControl(myevent,myIcon->myWindow,&control)))return 0;

	switch(part){

		case inButton:
	    	TrackControl(control,myevent,0l,0l);
	    	if(control == d->Control1){
	    	    if(control->controlValue){
			d->Ok=TRUE;
			return 1;
	    	    }else{
			d->Ok=FALSE;
	    	    }
	    	}else if(control == d->Control2){
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

