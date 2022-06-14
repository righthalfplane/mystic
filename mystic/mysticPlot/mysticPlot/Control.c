#define EXTERN22 extern
#include <sys/types.h>
#include <sys/time.h>
#include "Xappl.h"
#include "Control.h"
#include "xMenu.h"
#include "Tools.h"

enum {
	arrowLeft,
	arrowRight,
	arrowUp,
	arrowDown,
	arrowIn,
	arrowOut
};


int AddControlToWindow(Window controlwindow,controlPtr thisControl);

int DrawControlButton(controlPtr thisControl);
int DrawControlScroll(controlPtr thisControl);
int DrawArrow3D(uRect *controlRect,Window controlwindow,GC gc,
                int controlState,int controlSide,int Direction,unsigned long *c);
int DrawControlRadio(controlPtr thisControl);
int SetThumbRect(uRect *controlRect,controlPtr thisControl);
int controlSetThumb(controlPtr thisControl,XEvent *event);
int AdjustButtons(controlPtr control);
int Sleep2(int ms);
int DrawControlCheck(controlPtr thisControl);

controlPtr ControlCreate(struct Icon *myIcon,uRect *controlRect,char *controlName,
		  int controlType,int controlView,long controlMin,
		  long controlMax,long controlValue,long controlRef,long controlGroup)
{
	controlPtr thisControl;


	thisControl=(controlPtr)cMalloc(sizeof(*thisControl),8370);
	if(thisControl == NULL)return (controlPtr)NULL;

	zerol((char *)thisControl,sizeof(*thisControl));

	thisControl->controlRect = *controlRect;
	if(controlName)mstrncpy(thisControl->controlName,controlName,255);
	thisControl->controlType=controlType;
	thisControl->controlView=controlView;
	thisControl->controlMin=controlMin;
	thisControl->controlMax=controlMax;
	thisControl->controlValue=controlValue;
	thisControl->controlRef=controlRef;
	thisControl->controlGroup=controlGroup;
	if(controlType == controlTypeButton){
	    thisControl->controlSide=4;
	}else if(controlType == controlTypeCheck || controlType == controlTypeVScroll){
	    thisControl->controlSide=2;
	}else if(controlType == controlTypeHScroll){
	    thisControl->controlSide=2;
	}else if(controlType == controlTypeRadio){
	    thisControl->controlSide=3;
	}

	thisControl->controlwindow = XCreateSimpleWindow (xg.mydisplay,myIcon->myWindow,
	        controlRect->x,controlRect->y,
	        controlRect->xsize,controlRect->ysize,
	        0,xg.myforeground,xg.mybackground);


	XSelectInput (xg.mydisplay, thisControl->controlwindow,
	                ButtonPressMask | KeyPressMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask);

	if(AddControlToWindow(myIcon->myWindow,thisControl)){
	    return (controlPtr)NULL;
	}

	thisControl->gc = XCreateGC(xg.mydisplay, thisControl->controlwindow, 0, 0);

	XSetFont(xg.mydisplay,thisControl->gc,xg.font16);

    if(getColors(thisControl->c)){
       fprintf(stderr,"ControlCreate Error Getting Colors\n");
    }

	thisControl->info=xg.font16info;
	if(!thisControl->info)goto ErrorOut;

	XMapRaised (xg.mydisplay, thisControl->controlwindow);

	return thisControl;
ErrorOut:

	return NULL;
}

int HideControl(controlPtr thisControl)
{

	if(!thisControl)return 1;

	thisControl->hide=1;
	
	return 0;

}
int ShowControl(controlPtr thisControl)
{

	if(!thisControl)return 1;

	thisControl->hide=0;
	
	return 0;

}

int  ControlColors(unsigned long *c,controlPtr thisControl)
{
	int n;
	if(!c || !thisControl)return 1;

	for(n=0;n<6;++n)thisControl->c[n]=c[n];

	return 0;
}
int ControlDisposeWindow(controlPtr thisControl)
{
	if(!thisControl)return 1;
	if(thisControl->gc)XFreeGC(xg.mydisplay,thisControl->gc);
	thisControl->gc=NULL;
	if(thisControl->controlwindow)XDestroyWindow (xg.mydisplay,thisControl->controlwindow);
	cFree((char *)thisControl);
	return 0;
}
int ControlDispose(controlPtr thisControl)
{
	if(!thisControl)return 1;
	if(thisControl->gc)XFreeGC(xg.mydisplay,thisControl->gc);
	thisControl->gc=NULL;
	cFree((char *)thisControl);
	return 0;
}
int DrawControl(controlPtr thisControl)
{

	if(!thisControl)return 1;

	if(thisControl->hide)return 0;

	XClearWindow(xg.mydisplay,thisControl->controlwindow);

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
	uRect controlRect,thumbRect;
	int showArrowUp,showArrowDown;

	if(!thisControl)return 1;

	controlRect = thisControl->controlRect;
	controlRect.y=0;
	controlRect.x=0;
	DrawRect3D(&controlRect,thisControl->controlwindow,
	            thisControl->gc,1,
	            thisControl->controlSide,thisControl->c);

	SetThumbRect(&thumbRect,thisControl);

	DrawRect3D(&thumbRect,thisControl->controlwindow,
	            thisControl->gc,0,
	            thisControl->controlSide,thisControl->c);

	showArrowUp=arrowOut;
	showArrowDown=arrowOut;
	
	if(thisControl->controlWhere == inUpButton && thisControl->controlState){
	    showArrowUp=arrowIn;
	}else if(thisControl->controlWhere == inDownButton && thisControl->controlState){
	    showArrowDown=arrowIn;
	}

	
	controlRect= thisControl->controlRect;
	if(controlRect.xsize > controlRect.ysize){ /* Hoz */
	    thumbRect.x=0;
	    thumbRect.xsize=thumbRect.ysize;
	    DrawArrow3D(&thumbRect,thisControl->controlwindow,
	            thisControl->gc,showArrowUp,
	            thisControl->controlSide,arrowLeft,thisControl->c);
	    thumbRect.x=controlRect.xsize-thumbRect.xsize;
	    DrawArrow3D(&thumbRect,thisControl->controlwindow,
	            thisControl->gc,showArrowDown,
	            thisControl->controlSide,arrowRight,thisControl->c);
	}else{
	    thumbRect.y=0;
	    thumbRect.ysize=thumbRect.xsize;
	    DrawArrow3D(&thumbRect,thisControl->controlwindow,
	            thisControl->gc,showArrowUp,
	            thisControl->controlSide,arrowUp,thisControl->c);
	    thumbRect.y=controlRect.ysize-thumbRect.xsize;
	    DrawArrow3D(&thumbRect,thisControl->controlwindow,
	            thisControl->gc,showArrowDown,
	            thisControl->controlSide,arrowDown,thisControl->c);
	}
	return 0;
}
int DrawArrow3D(uRect *controlRect,Window controlwindow,GC gc,
                int controlState,int controlSide,int Direction,unsigned long *c)
{
        XPoint top[6],bot[6],mid[3];
	int n,ntop,nbot,size2,size;

	if(c == NULL)c = &xg.c[0];

	size=controlRect->ysize;
	size2=size/2;
	
	if(Direction == arrowLeft){
	    mid[0].x = size;
	    mid[0].y = 0;

	    mid[1].x = 0;
	    mid[1].y = size2;

	    mid[2].x = size;
	    mid[2].y = size;

	    top[0].x = 0;
	    top[0].y = size2;

	    top[1].x = controlSide;
	    top[1].y = size2;

	    top[2].x = size-controlSide;
	    top[2].y = controlSide;

	    top[3].x = size;
	    top[3].y = 0;

	    ntop=4;

	    bot[0].x = 0;
	    bot[0].y = size2;

	    bot[1].x = size;
	    bot[1].y = size;

	    bot[2].x = size;
	    bot[2].y = 0;

	    bot[3].x = size-controlSide;
	    bot[3].y = controlSide;

	    bot[4].x = size-controlSide;
	    bot[4].y = size-controlSide;

	    bot[5].x = controlSide;
	    bot[5].y = size2;

	    nbot=6;

	}else if(Direction == arrowRight){

	    mid[0].x = 0;
	    mid[0].y = 0;

	    mid[1].x = 0;
	    mid[1].y = size;

	    mid[2].x = size;
	    mid[2].y = size2;

	    top[0].x = 0;
	    top[0].y = size;

	    top[1].x = controlSide;
	    top[1].y = size-controlSide;

	    top[2].x = controlSide;
	    top[2].y = controlSide;

	    top[3].x = size-controlSide;
	    top[3].y = size2;

	    top[4].x = size;
	    top[4].y = size2;

	    top[5].x = 0;
	    top[5].y = 0;

	    ntop=6;

	    bot[0].x = controlSide;
	    bot[0].y = size;

	    bot[1].x = size;
	    bot[1].y = size2;

	    bot[2].x = size-controlSide;
	    bot[2].y = size2;

	    bot[3].x = controlSide;
	    bot[3].y = size-controlSide;


	    nbot=4;


	}else if(Direction == arrowUp){
	    mid[0].x = 0;
	    mid[0].y = size;

	    mid[1].x = size;
	    mid[1].y = size;

	    mid[2].x = size2;
	    mid[2].y = 0;

	    top[0].x = size2;
	    top[0].y = 0;

	    top[1].x = 0;
	    top[1].y = size;

	    top[2].x = controlSide;
	    top[2].y = size-controlSide;

	    top[3].x = size2;
	    top[3].y = controlSide;

	    ntop=4;

	    bot[0].x = 0;
	    bot[0].y = size;

	    bot[1].x = size;
	    bot[1].y = size;

	    bot[2].x = size2;
	    bot[2].y = 0;

	    bot[3].x = size2;
	    bot[3].y = controlSide;

	    bot[4].x = size-controlSide;
	    bot[4].y = size-controlSide;

	    bot[5].x = controlSide;
	    bot[5].y = size-controlSide;

	    nbot=6;

	}else if(Direction == arrowDown){
	    mid[0].x = 0;
	    mid[0].y = 0;

	    mid[1].x = size2;
	    mid[1].y = size;

	    mid[2].x = size;
	    mid[2].y = 0;

	    top[0].x = size;
	    top[0].y = 0;

	    top[1].x = 0;
	    top[1].y = 0;

	    top[2].x = size2;
	    top[2].y = size;

	    top[3].x = size2;
	    top[3].y = size-controlSide;

	    top[4].x = controlSide;
	    top[4].y = controlSide;

	    top[5].x = size-controlSide;
	    top[5].y = controlSide;

	    ntop=6;

	    bot[0].x = size2;
	    bot[0].y = size;

	    bot[1].x = size;
	    bot[1].y = 0;

	    bot[2].x = size-controlSide;
	    bot[2].y = controlSide;

	    bot[3].x = size2;
	    bot[3].y = size-controlSide;


	    nbot=4;

	}else{
	    return 1;
	}
	

	for(n=0;n<6;++n){
	    top[n].x += controlRect->x;
	    top[n].y += controlRect->y;
	    bot[n].x += controlRect->x;
	    bot[n].y += controlRect->y;
	    if(n < 3){
	        mid[n].x += controlRect->x;
	        mid[n].y += controlRect->y;
	    }
	}

	if(controlState == arrowIn){ /* it is in */
	    XSetForeground (xg.mydisplay, gc, c[1]);
	    XSetBackground (xg.mydisplay, gc, c[1]);

             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    mid, 3, Convex, CoordModeOrigin);


	    XSetForeground (xg.mydisplay, gc, c[0]);
	    XSetBackground (xg.mydisplay, gc, c[0]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    top, ntop, Complex, CoordModeOrigin);
	    XSetForeground (xg.mydisplay, gc, c[3]);
	    XSetBackground (xg.mydisplay, gc, c[3]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    bot, nbot, Complex, CoordModeOrigin);

	}else{  /* it is out */
	    XSetForeground (xg.mydisplay, gc, c[2]);
	    XSetBackground (xg.mydisplay, gc, c[2]);
            XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    mid, 3, Convex, CoordModeOrigin);

	    XSetForeground (xg.mydisplay, gc, c[3]);
	    XSetBackground (xg.mydisplay, gc, c[3]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    top, ntop, Complex, CoordModeOrigin);
	    XSetForeground (xg.mydisplay, gc, c[0]);
	    XSetBackground (xg.mydisplay, gc, c[0]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    bot, nbot, Complex, CoordModeOrigin);
	}
	return 0;

}
int DrawControlRadio(controlPtr thisControl)
{
	XFontStruct *info;
	uRect controlRect;
	int controlSide;
	int controlState;
	long controlValue;
        XPoint top[6],bot[6],mid[4];
	int size,size2,xc,yc;
	unsigned long n;

	if(!thisControl)return 1;

	controlRect = thisControl->controlRect;
	controlSide = thisControl->controlSide;
	controlState = thisControl->controlState;
	controlValue = thisControl->controlValue;
	if(controlState) controlValue = !controlValue;

	size=controlRect.ysize;
	if(size > controlRect.xsize)size = controlRect.xsize;

	xc=size/2;
	yc=2+xc;

	size -= 4;
	size2 = size/2;

	mid[0].x = xc;
	mid[0].y = yc-size2;

	mid[1].x = xc-size2;
	mid[1].y = yc;

	mid[2].x = xc;
	mid[2].y = yc+size2;

	mid[3].x = xc+size2;
	mid[3].y = yc;

	top[0].x = xc-size2;
	top[0].y = yc;

	top[1].x = xc-size2+controlSide;
	top[1].y = yc;

	top[2].x = xc;
	top[2].y = yc-size2+controlSide;

	top[3].x = xc+size2-controlSide;
	top[3].y = yc;

	top[4].x = xc+size2;
	top[4].y = yc;

	top[5].x = xc;
	top[5].y = yc-size2;

	bot[0].x = xc-size2;
	bot[0].y = yc;

	bot[1].x = xc;
	bot[1].y = yc+size2;

	bot[2].x = xc+size2;
	bot[2].y = yc;

	bot[3].x = xc+size2-controlSide;
	bot[3].y = yc;

	bot[4].x = xc;
	bot[4].y = yc+size2-controlSide;

	bot[5].x = xc-size2+controlSide;
	bot[5].y = yc;

	XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	XFillRectangle(xg.mydisplay,thisControl->controlwindow,
	               thisControl->gc,
	               0,0,
	               thisControl->controlRect.xsize,
	               thisControl->controlRect.ysize);

	if(controlValue){ /* it is pushed */
	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[1]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[1]);
             XFillPolygon(xg.mydisplay,thisControl->controlwindow,
                    thisControl->gc,
                    mid, 4, Convex, CoordModeOrigin);


	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[0]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[0]);
             XFillPolygon(xg.mydisplay,thisControl->controlwindow,
                    thisControl->gc,
                    top, 6, Complex, CoordModeOrigin);
 
	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[3]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[3]);
             XFillPolygon(xg.mydisplay,thisControl->controlwindow,
                    thisControl->gc,
                    bot, 6, Complex, CoordModeOrigin);

	}else{  /* it is not pushed */
	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
             XFillPolygon(xg.mydisplay,thisControl->controlwindow,
                    thisControl->gc,
                    mid, 4, Convex, CoordModeOrigin);

	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[3]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[3]);
             XFillPolygon(xg.mydisplay,thisControl->controlwindow,
                    thisControl->gc,
                    top, 6, Complex, CoordModeOrigin);

	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[0]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[0]);
             XFillPolygon(xg.mydisplay,thisControl->controlwindow,
                    thisControl->gc,
                    bot, 6, Complex, CoordModeOrigin);
  
	}
	n=strlen(thisControl->controlName);
	if(n != 0){
	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[4]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	    info=thisControl->info;
	    if(info){
            XDrawImageString(xg.mydisplay,thisControl->controlwindow,
            thisControl->gc,
            size+5,
            thisControl->controlRect.ysize/2+info->ascent/2,
            thisControl->controlName,(int)n);
	    }else{
	        XDrawImageString(xg.mydisplay,thisControl->controlwindow,
	            thisControl->gc,25,20,thisControl->controlName,(int)n);
	    }
	}
	return 0;

}
int DrawControlCheck(controlPtr thisControl)
{
	XFontStruct *info;
	uRect controlRect;
	int controlState;
	long controlValue;
	int size,size2,xc,yc;
	unsigned long n;

	if(!thisControl)return 1;

	controlRect = thisControl->controlRect;
	controlState = thisControl->controlState;
	controlValue = thisControl->controlValue;
	if(controlState) controlValue = !controlValue;

	size=controlRect.ysize;
	if(size > controlRect.xsize)size = controlRect.xsize;
	size /= 2;
	xc=size;
	yc=size;
	size2 = size/2;

	controlRect.y=yc-size2;
	controlRect.x=xc-size2;
	controlRect.xsize=size;
	controlRect.ysize=size;

	XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	XFillRectangle(xg.mydisplay,thisControl->controlwindow,
	               thisControl->gc,
	               0,0,
	               thisControl->controlRect.xsize,
	               thisControl->controlRect.ysize);

	DrawRect3D(&controlRect,thisControl->controlwindow,
	            thisControl->gc,(int)controlValue,
	            thisControl->controlSide,thisControl->c);


	n=strlen(thisControl->controlName);
	if(n != 0){
	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[4]);
	    XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	    info=thisControl->info;
	    if(info){
/*
	        int width;

	        width=XTextWidth(info,thisControl->controlName,(int)n);
	        XDrawImageString(xg.mydisplay,thisControl->controlwindow,
	        thisControl->gc,thisControl->controlRect.xsize/2-width/2,thisControl->controlRect.ysize/2+info->ascent/2,thisControl->controlName,(int)n);
*/
	        XDrawImageString(xg.mydisplay,thisControl->controlwindow,
	        thisControl->gc,30,thisControl->controlRect.ysize/2+info->ascent/2,thisControl->controlName,(int)n);
	    }else{
	        XDrawImageString(xg.mydisplay,thisControl->controlwindow,
	            thisControl->gc,25,20,thisControl->controlName,(int)n);
	    }
	}
	return 0;

}
int DrawControlButton(controlPtr thisControl)
{
	XFontStruct *info;
	uRect controlRect;
	unsigned long n;

	if(!thisControl)return 1;

	controlRect = thisControl->controlRect;
	controlRect.y=0;
	controlRect.x=0;
	DrawRect3D(&controlRect,thisControl->controlwindow,
	            thisControl->gc,thisControl->controlState,
	            thisControl->controlSide,thisControl->c);

	n=strlen(thisControl->controlName);
	if(n != 0){
	    XSetForeground (xg.mydisplay, thisControl->gc, thisControl->c[4]);
	    if(thisControl->controlState){ /* it is pushed */
	        XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[1]);
	    }else{  /* it is not pushed */
	        XSetBackground (xg.mydisplay, thisControl->gc, thisControl->c[2]);
	    }
	    info=thisControl->info;
	    if(info){
	        int width;

	        width=XTextWidth(info,thisControl->controlName,(int)n);

	        XDrawImageString(xg.mydisplay,thisControl->controlwindow,
	        thisControl->gc,thisControl->controlRect.xsize/2-width/2,thisControl->controlRect.ysize/2+info->ascent/2,thisControl->controlName,(int)n);
	    }else{
	        XDrawImageString(xg.mydisplay,thisControl->controlwindow,
	            thisControl->gc,25,20,thisControl->controlName,(int)n);
	    }
	}
	return 0;

}
int DrawRect3D(uRect *controlRect,Window controlwindow,GC gc,
               int controlState,int controlSide, unsigned long *c)
{
        XPoint top[6],bot[6];
	int n;

	if(c == NULL)c = &xg.c[0];

	top[0].x = 0;
	top[0].y = controlRect->ysize;

	top[1].x = controlSide;
	top[1].y = controlRect->ysize-controlSide;

	top[2].x = controlSide;
	top[2].y = controlSide;

	top[3].x = controlRect->xsize-controlSide;
	top[3].y = controlSide;

	top[4].x = controlRect->xsize;
	top[4].y = 0;

	top[5].x = 0;
	top[5].y = 0;

	bot[0].x = 0;
	bot[0].y = controlRect->ysize;

	bot[1].x = controlRect->xsize;
	bot[1].y = controlRect->ysize;

	bot[2].x = controlRect->xsize;
	bot[2].y = 0;

	bot[3].x = controlRect->xsize-controlSide;
	bot[3].y = controlSide;

	bot[4].x = controlRect->xsize-controlSide;
	bot[4].y = controlRect->ysize-controlSide;

	bot[5].x = controlSide;
	bot[5].y = controlRect->ysize-controlSide;

	for(n=0;n<6;++n){
	    top[n].x += controlRect->x;
	    top[n].y += controlRect->y;
	    bot[n].x += controlRect->x;
	    bot[n].y += controlRect->y;
	}

	if(controlState){ /* it is in */
	    if(controlState == 1){
	        XSetForeground (xg.mydisplay, gc, c[1]);
	        XSetBackground (xg.mydisplay, gc, c[1]);
	    }else{
	        XSetForeground (xg.mydisplay, gc, c[2]);
	        XSetBackground (xg.mydisplay, gc, c[2]);
	    }
	    XFillRectangle(xg.mydisplay,controlwindow,
	               gc,
	               controlRect->x,controlRect->y,
	               controlRect->xsize,
	               controlRect->ysize);


	    XSetForeground (xg.mydisplay, gc, c[0]);
	    XSetBackground (xg.mydisplay, gc, c[0]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    top, 6, Convex, CoordModeOrigin);
	    XSetForeground (xg.mydisplay, gc, c[3]);
	    XSetBackground (xg.mydisplay, gc, c[3]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    bot, 6, Convex, CoordModeOrigin);

	}else{  /* it is out */
	    XSetForeground (xg.mydisplay, gc, c[2]);
	    XSetBackground (xg.mydisplay, gc, c[2]);
	    XFillRectangle(xg.mydisplay,controlwindow,
	               gc,
	               controlRect->x,controlRect->y,
	               controlRect->xsize,
	               controlRect->ysize);

	    XSetForeground (xg.mydisplay, gc, c[3]);
	    XSetBackground (xg.mydisplay, gc, c[3]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    top, 6, Convex, CoordModeOrigin);
	    XSetForeground (xg.mydisplay, gc, c[0]);
	    XSetBackground (xg.mydisplay, gc, c[0]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    bot, 6, Convex, CoordModeOrigin);
	}
   
	return 0;
}

int TrackControl(controlPtr control,XEvent *eventin,long (*controlRoutine)(controlPtr control,short part,void *controlRoutineData),void *controlRoutineData)
{
	XEvent event;
	int init,itWas,controlWhere;
	long valuet;

    if(!control)return 1;

/* Make sure that the calling window eats LeaveWindow events or they happen here */

	//eventin=eventin;
	init=TRUE;
	itWas = -1;
	valuet= -123;
	control->controlState=1;
	controlWhere=control->controlWhere;
	DrawControl(control);
	while(1){
	    if(XCheckMaskEvent(xg.mydisplay,ButtonPressMask | KeyPressMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask | PointerMotionMask
	                ,&event)){
	    }else{
	        if(controlRoutine){
	            long value;

				value=uGetControlValue(control);
				if(controlWhere == inThumb){
					if(value != valuet)(*controlRoutine)(control,controlWhere,controlRoutineData);
				}else{
					if(init)(*controlRoutine)(control,controlWhere,controlRoutineData);
				}		
				valuet=value;
				if(value != uGetControlValue(control)){
					DrawControl(control);
				}
				Sleep2(50);
			}
	        continue;
	    }
		if(event.xany.window != control->controlwindow){
		    continue;
		}
		switch(event.type){
		case EnterNotify:
		    init=TRUE;
		break;
		case LeaveNotify:
		    init=FALSE;
		break;
		case MotionNotify:
		    if(InRect(&event,&control->controlRect)){
				init=TRUE;
		    }else{
				init=FALSE;
		    }
		    control->controlState=init;

		    if(controlWhere == inThumb){
				while(XCheckMaskEvent(xg.mydisplay,PointerMotionMask,&event)){
					;
				}
				
				valuet=uGetControlValue(control);
				controlSetThumb(control,&event);
				if(valuet != uGetControlValue(control))(*controlRoutine)(control,controlWhere,controlRoutineData);
		        itWas = -1;
		    }

		    if(itWas != init){
		        DrawControl(control);
		    }
		    itWas=init;
		break;
		case ButtonRelease:
		    goto OutOfHere;
		default:
		    printf("TrackControl  event.type %d MotionNotify %d\n",event.type,MotionNotify);
		}
	}
OutOfHere:
	if(control->controlType == controlTypeButton){
	    control->controlValue=init;
	} else if(control->controlType == controlTypeCheck){
	    if(control->controlState)control->controlValue = !control->controlValue;
	} else if(control->controlType == controlTypeRadio){
	    if(control->controlState){
	        control->controlValue = 1;
	        AdjustButtons(control);
	    }
	}
	control->controlState=0;
	DrawControl(control);
	return 0;
}

int AdjustButtons(controlPtr control)
{
	controlPtr c;

	if(!control)return 1;
	if(!(c=control->controlList))return 1;
	while(c){
	    if(c->controlType == controlTypeRadio && 
	       c->controlGroup == control->controlGroup &&
	       c != control){

			c->controlValue = !control->controlValue;
			c->controlState=0;
			DrawControl(c);
	    }
	    c=c->controlNext;
	}
	return 0;
	
}
int InRect(XEvent *event,uRect *controlRect)
{
	if(!event || !controlRect)return 0;
	if(event->xmotion.x < 0 || event->xmotion.y < 0)return 0;
	if(event->xmotion.x > controlRect->xsize ||
	   event->xmotion.y > controlRect->ysize)return 0;
	return 1;
}
int SetThumbRect(uRect *controlRect,controlPtr thisControl)
{
	INT8_64 rangeD,range;
	int size,xsize,ysize;
	long left,top;
	
    if(!thisControl || !controlRect)return 1;

	*controlRect= thisControl->controlRect;


	if(thisControl->controlValue < thisControl->controlMin){
	    thisControl->controlValue = thisControl->controlMin;
	}else if(thisControl->controlValue > thisControl->controlMax){
	    thisControl->controlValue = thisControl->controlMax;
	}
	
	rangeD=thisControl->controlMax-thisControl->controlMin;
	if(rangeD <= 0 ){
	    rangeD=1;
	    thisControl->controlMax=thisControl->controlMin;
	    if(controlRect->xsize > controlRect->ysize){ /* Hoz */
	        ysize=controlRect->ysize-2*thisControl->controlSide;
	        xsize=controlRect->xsize-2*ysize;
	        top=thisControl->controlSide;
	        left=ysize;
	    }else{
	        xsize=controlRect->xsize-2*thisControl->controlSide;
	        ysize=controlRect->ysize-2*xsize;
	        top=xsize;
	        left=thisControl->controlSide;
	    }
	    controlRect->y=(int)top;
	    controlRect->x=(int)left;
	    controlRect->xsize=xsize;
	    controlRect->ysize=ysize;
	    return 0;
	}

	if(controlRect->xsize > controlRect->ysize){ /* Hoz */
	    size=controlRect->ysize-2*thisControl->controlSide;
	    range=controlRect->xsize-3*size;
	    top=thisControl->controlSide;
	    left=size+(thisControl->controlValue-thisControl->controlMin)*range/rangeD;
	}else{ /* Ver */
	    size=controlRect->xsize-2*thisControl->controlSide;
	    range=controlRect->ysize-3*size;
	    top=size+(thisControl->controlValue-thisControl->controlMin)*range/rangeD;
	    left=thisControl->controlSide;
	}

	controlRect->y=(int)top;
	controlRect->x=(int)left;
	controlRect->xsize=size;
	controlRect->ysize=size;
	return 0;
	
}
int controlSetThumb(controlPtr thisControl,XEvent *event)
{
	uRect controlRect;
	INT8_64 rangeD,range;
	long size;
	
    if(!thisControl)return 1;

	controlRect= thisControl->controlRect;

	rangeD=thisControl->controlMax-thisControl->controlMin;
	if(rangeD <= 0 ){
	    rangeD=1;
	    thisControl->controlMax=thisControl->controlMin;
	}
	if(controlRect.xsize > controlRect.ysize){ /* Hoz */
	    size=controlRect.ysize-2*thisControl->controlSide;
	    if(event->xmotion.x > controlRect.xsize-size){
	        thisControl->controlValue=thisControl->controlMax;
	    }else if(event->xmotion.x < size){
	        thisControl->controlValue=thisControl->controlMin;
	    }else{
	        range=controlRect.xsize-3*size;
	        thisControl->controlValue=(event->xmotion.x-size)*rangeD/range+thisControl->controlMin;
	    }
	}else{ /* Ver */
	    size=controlRect.xsize-2*thisControl->controlSide;
	    if(event->xmotion.y > controlRect.ysize-size){
	        thisControl->controlValue=thisControl->controlMax;
	    }else if(event->xmotion.y < size){
	        thisControl->controlValue=thisControl->controlMin;
	    }else{
	      range=controlRect.ysize-3*size;
	      thisControl->controlValue=(event->xmotion.y-size)*rangeD/range+thisControl->controlMin;
	    }
	}

	if(thisControl->controlValue < thisControl->controlMin){
	    thisControl->controlValue = thisControl->controlMin;
	}else if(thisControl->controlValue > thisControl->controlMax){
	    thisControl->controlValue = thisControl->controlMax;
	}
	return 0;
	
}
int ControlScrollWhere(controlPtr control,XEvent *myevent)
{
	uRect thumbRect,controlRect;
	int size,Where;

    if(!control)return 1;
    
	SetThumbRect(&thumbRect,control);

	controlRect=control->controlRect;

	size=thumbRect.xsize;

	Where=inButton;

	if(controlRect.xsize > controlRect.ysize){ /* Hoz */
	    if(myevent->xbutton.x < size){
	        Where=inUpButton;
	    } else if(myevent->xbutton.x > controlRect.xsize-size){
	        Where=inDownButton;
	    } else if(myevent->xbutton.x < thumbRect.x){
	        Where=inPageUp;
	    } else if(myevent->xbutton.x > thumbRect.x+size){
	        Where=inPageDown;
	    } else{
	        Where=inThumb;
	    }
	}else{ /* Ver */
	    if(myevent->xbutton.y < size){
	        Where=inUpButton;
	    } else if(myevent->xbutton.y > controlRect.ysize-size){
	        Where=inDownButton;
	    } else if(myevent->xbutton.y < thumbRect.y){
	        Where=inPageUp;
	    } else if(myevent->xbutton.y > thumbRect.y+size){
	        Where=inPageDown;
	    } else{
	        Where=inThumb;
	    }
	}
	control->controlWhere=Where;
	return Where;
}
long GetControlReference(controlPtr control)
{
	if(!control)return -1L;
	
	return control->controlRef;
}
int ControlGetRect(uRect *rect,controlPtr control)
{
	if(!rect || !control)return 1;

	*rect=control->controlRect;

	return 0;
}
int MoveControl(controlPtr control,int x,int y)
{
	if(!control || !xg.mydisplay)return 1;
	control->controlRect.x=x;
	control->controlRect.y=y;
	XMoveWindow(xg.mydisplay,control->controlwindow,x,y);
	return 0;
}
int SizeControl(controlPtr control,int xsize,int ysize)
{
	if(!control || !xg.mydisplay)return 1;
	if(xsize <= 0)xsize=20;
	if(ysize <= 0)ysize=20;	
	control->controlRect.xsize=xsize;
	control->controlRect.ysize=ysize;	
	XResizeWindow(xg.mydisplay,control->controlwindow,xsize,ysize);
	return 0;
}
int uSetControlMaximum(controlPtr control,long controlValue)
{
	if(!control)return 1;

	control->controlMax=controlValue;
	if(control->controlValue > control->controlMax){
	    control->controlValue=control->controlMax;
	}
	return 0;
}
long uGetControlMaximum(controlPtr control)
{
	if(!control)return 1;

	return control->controlMax;
}
int uSetControlValue(controlPtr control,long controlValue)
{
	if(!control)return 1;
	if(controlValue < control->controlMin){
	    controlValue=control->controlMin;
	}else if(controlValue > control->controlMax){
	    controlValue=control->controlMax;
	}
	control->controlValue=controlValue;
	
	if(control->controlType == controlTypePopUpMenu){
		MenuSetPopUpTitle((int)(controlValue-1),control->controlData.menubar);	
	}
	return 0;
}
long uGetControlValue(controlPtr control)
{

	if(!control)return -1;
	
	return control->controlValue;
}
