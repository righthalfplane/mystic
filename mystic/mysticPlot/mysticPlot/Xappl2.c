#define EXTERN22 extern
#include "Xappl.h"
#include <stdlib.h>
#include "FileManager.h"
int getScreenColors(unsigned long *c,IconPtr myIcon);

IconPtr FindWindowType(int Type);

#define PICT_8     8

static int StandardInfoData(struct Icon *myIcon,struct SetFrameData  *sd);
static int StandardGetData(struct Icon *myIcon,long CurrentFrame,struct SetFrameData  *sd);
static int StandardPutData(struct Icon *myIcon,long CurrentFrame,struct SetFrameData  *sd);
static int StandardSyncRegion(struct Icon *myIcon,uRect *r);


int  CloseNotify(IconPtr myIcon);
int AddControlToWindow(Window controlwindow,controlPtr thisControl);
int NotMyWindow(IconPtr myIcon,XEvent *event);
int PutChildWindow(IconPtr myIcon,Window newChild);

int SetMyIconWindowFront(IconPtr myIcon);

int SetMyIconWindowFront(IconPtr myIcon)
{
	if(!myIcon)return 1;
	myIcon=FindWindowInList(myIcon);
	if(!myIcon)return 1;
	XRaiseWindow(xg.mydisplay,myIcon->myWindow);
	return 0;
}
static int StandardSyncRegion(struct Icon *myIcon,uRect *r)
{
	if(!myIcon || !r)return 0;
	return 0;
}
static int StandardInfoData(struct Icon *myIcon,struct SetFrameData  *sd)
{
	if(!myIcon || !sd)return 0;
	return 0;
}
static int StandardGetData(struct Icon *myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	if(!myIcon || !sd)return 1;
	//CurrentFrame=CurrentFrame;
	return 1;
}
static int StandardPutData(struct Icon *myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	if(!myIcon || !sd)return 1;
	//CurrentFrame=CurrentFrame;
	return 1;
}


int  CloseNotify(IconPtr myIcon)
{
	IconPtr p;

	if(!myIcon)return 1;

	p=IconRoot;
	
	while(p){
	    if(p->pCloseNotify){
	       (*p->pCloseNotify)(p,myIcon);
	    }
	    p=p->IconNext;
	}
	return 0;

}

IconPtr CreateIcon(int Type,uRect *wRect)
{
	extern struct SYSFONTLIST pSysFontList;
	char buff[256];
	IconPtr p,o;

	p=IconRoot;
	o=NULL;
	
	while(p){		/*	Search for end of List	*/
		o=p;
		p=p->IconNext;
	}
	if(!(p=(IconPtr)cMalloc((long)sizeof(Icon),8730))){ /*	Make new link	*/
	    WarningBatch("CreateIcon Out of Memory");
	    return (IconPtr)NULL;
	}
	zerol((char *)p,sizeof(Icon));
	if(o){
	    o->IconNext=p;
	} else {
	    IconRoot=p;
	}
	if(wRect){

	   XSetWindowAttributes attr;
	   int attr_flags;

	   attr.colormap = xg.xColormapDefault;
	   attr.event_mask = ExposureMask | StructureNotifyMask;
	   attr.border_pixel = BlackPixel(xg.mydisplay,DefaultScreen(xg.mydisplay));
	   attr.background_pixel = BlackPixel(xg.mydisplay,DefaultScreen(xg.mydisplay));
	   attr_flags = CWColormap | CWEventMask | CWBorderPixel | CWBackPixel;
	   p->myWindow = XCreateWindow(xg.mydisplay,DefaultRootWindow (xg.mydisplay),
	                    wRect->x,wRect->y,
			    wRect->xsize, wRect->ysize, 0,
			    xg.vBits, InputOutput,
			    xg.xVisual,
			    attr_flags, &attr);
	    if(p->myWindow == (Window)NULL){
	        sprintf(WarningBuff,"XCreateWindow Has Returned NULL pointer");
	        WarningBatch(WarningBuff);
		    return (IconPtr)NULL;
	    }
	} else {
	    sprintf(buff,"wRect Has NULL pointer\n");
	    WarningBatch(buff);
	    return (IconPtr)NULL;
	}
	p->IconNext=NULL;
	p->FileSave=1;
	p->viewRect= *wRect;
	p->WindowType=Type;
	p->pGetData=StandardGetData;
	p->pPutData=StandardPutData;
	p->pInfoData=StandardInfoData;
	p->pSyncRegion=StandardSyncRegion;
	p->pSysFontList=pSysFontList;
	
#ifdef CleanExit	
     p->wm_delete_window = XInternAtom(xg.mydisplay, "WM_DELETE_WINDOW", False);
    (void) XSetWMProtocols(xg.mydisplay, p->myWindow, &p->wm_delete_window, 1);
#endif	

	myIcon=p;

	{
	    unsigned long c[10];

	    if(!getScreenColors(c,myIcon)){
	        ;
	    }
	}
	
	myIcon->gcRubber = XCreateGC(xg.mydisplay,myIcon->myWindow,0,0); 
	{
	    XGCValues v;

	    v.foreground=myIcon->c[4];
	    v.background=myIcon->c[2];

	    v.foreground = v.foreground ^ v.background ;

	    v.function = GXxor ;  

	     XChangeGC(xg.mydisplay,myIcon->gcRubber,  
	               GCForeground | GCBackground | GCFunction, &v);  
	}
	myIcon->ScreenBits=PICT_8;

	return p;
}
int getScreenColors(unsigned long *c,IconPtr myIcon)
{
	int n;

	if(!c || !myIcon)return 1;

	if(getColors(c))return 1;

	for(n=0;n<10;++n)myIcon->c[n]=c[n];

	return 0;
}
IconPtr RemoveIcon(IconPtr i)
{
	IconPtr p,o;
	
	p=IconRoot;
	o=NULL;
	
	while(p){		/*	Search for Icon	*/
	    if(p == i)break;
	    o=p;
	    p=p->IconNext;
	}
	
	if(!p)return (IconPtr)NULL;
	
	FlushAllEvents();
	
	if(o)o->IconNext=p->IconNext; /*	Connect link	*/

	if(p->MenuBarList)MenuBarDispose(p->MenuBarList);
	p->MenuBarList=NULL;
	if(p->VControl)ControlDispose(p->VControl);
	p->VControl=NULL;
	if(p->HControl)ControlDispose(p->HControl);
	p->HControl=NULL;
	
	if(p->mychild)cFree((char *)p->mychild);
	p->mychild=NULL;
	
	if(p->info)XFreeFontInfo(NULL, p->info, 1);
	p->info=NULL;

	if(p->Screen.buffer)cFree((char *)p->Screen.buffer);
	p->Screen.buffer=NULL;
	
	if(p->Screen.xImage){
		p->Screen.xImage->data=NULL;
		XDestroyImage(p->Screen.xImage);
		p->Screen.xImage=NULL;
	}
	
	if(p->ScreenRect.buffer)cFree((char *)p->ScreenRect.buffer);
	p->ScreenRect.buffer=NULL;
	
	if(p->ScreenRect.xImage){
		p->ScreenRect.xImage->data=NULL;
		XDestroyImage(p->ScreenRect.xImage);
		p->ScreenRect.xImage=NULL;
	}
	
	FlushAllEvents();
	
	if(p->myWindow)XDestroyWindow(xg.mydisplay,p->myWindow);

	if(p == IconRoot){
	    if((o=p->IconNext) != NULL){
			IconRoot=o;
	    } else {
			o=NULL;
	    	IconRoot=NULL;
	    	doQuit();
	    }
	}
	
	if(p->gc)XFreeGC(xg.mydisplay,p->gc);
	if(p->gcFont)XFreeGC(xg.mydisplay,p->gcFont);
	if(p->gcRubber)XFreeGC(xg.mydisplay,p->gcRubber);
	
	FileDialogRemoveInfo(&p->infoLoad);
	FileDialogRemoveInfo(&p->infoSave);
	FileDialogRemoveInfo(&p->uNet.infoSave);
	
	cFree((char *)p);
	myIcon=o;
	
	return o;
}

int PutChildWindow(IconPtr myIcon,Window newChild)
{

	Window  *mychild;
	int n;

	if(!myIcon || !newChild)return 1;

    mychild=(Window  *)cMalloc((myIcon->childCount+1)*sizeof(Window),9827);
    if(!mychild)return 1;
    	
	if((myIcon->childCount > 0) && myIcon->mychild){
	    for(n=0;n<myIcon->childCount;++n){
	        mychild[n]=myIcon->mychild[n];
	    }
	    
	    cFree((char *)myIcon->mychild);
	    myIcon->mychild=NULL;
	}
    mychild[myIcon->childCount]=newChild;
    myIcon->childCount++;
    myIcon->mychild=mychild;

	return 0;
}
IconPtr FindIWindow(Window w)
{
	IconPtr p;
	int n;
	
	p=IconRoot;
	
	while(p){		/*	Search for Window	*/
	    if(p->myWindow == w)return p;
	    if(p->mychild){
	        for(n=0;n<p->childCount;++n){
		    	if(p->mychild[n] == w)return p;
	        }
	    }
	    if(p->controlList){
	        controlPtr c;

	        c=p->controlList;
	        if(c->controlwindow == w)return p;
	        while(c->controlNext){
		    	c=c->controlNext;
		    	if(c->controlwindow == w)return p;
	        }
	    }
	    p=p->IconNext;
	}
	return (IconPtr)NULL;
}
int RemoveControlFromWindow(Window controlwindow,controlPtr thisControl)
{
	IconPtr win;
	controlPtr c,last;

	win=FindIWindow(controlwindow);
	if(win == NULL)return 1;

	last=NULL;
	c=win->controlList;
	while(c){
	    if(c == thisControl){
	        if(last){
	            last->controlNext=thisControl->controlNext;
	        }else{
	            win->controlList=thisControl->controlNext;
	        }
	        break;
	    }
	    c=c->controlNext;
		last=c;
	}

	return 0;
}
int AddControlToWindow(Window controlwindow,controlPtr thisControl)
{
	IconPtr win;
	controlPtr c;

	win=FindIWindow(controlwindow);
	if(win == NULL)return 1;

	if(win->controlList){
	    c=win->controlList;
	    while(c->controlNext){
	        c=c->controlNext;
	    }
	    c->controlNext=thisControl;
	}else{
	    win->controlList=thisControl;
	}

	return 0;
}
int FindControl(XEvent *myevent,Window win,controlPtr *control)
{
	IconPtr myIcon;

	myIcon=FindIWindow(win);
	if(myIcon == NULL){
	    printf("FindControl  Window Not found\n");
	    return 0;
	}

	if(myIcon->controlList){
        controlPtr c;

        c=myIcon->controlList;
        while(c){
		    if(c->controlwindow == myevent->xbutton.window){
	            *control = c;
		       (*control)->controlList=myIcon->controlList;
		       if(c->controlType == controlTypeVScroll || c->controlType == controlTypeHScroll){
		           return ControlScrollWhere(c,myevent);
		       }else{
		            return inButton;
		       }
		    }
		    c=c->controlNext;
        }
	}
	return 0;
}
int zerol(char *s,unsigned long n)
{
	unsigned long count;

	if(s == NULL)return 1;
	count=0;
	while(count++ < n)*s++ = 0;
	return 0;
}

int NotMyWindow(IconPtr myIcon,XEvent *event)
{
	IconPtr myIcon2;

	if(!myIcon || !event)return 0;
	myIcon2=FindIWindow(event->xany.window);
	if(myIcon2 && (myIcon2->myWindow != myIcon->myWindow)){
	   if(event->type == Expose){
		if(myIcon2->pdoExpose)
		   (*myIcon2->pdoExpose)(myIcon2,event);
	    }else if(event->type == ButtonPress){
	        Bell(62); 
	    }else if(event->type == ConfigureNotify){
		if(myIcon2->pdoConfigureNotify)  
		   (*myIcon2->pdoConfigureNotify)(myIcon2,event);
	    }
	    return 1;
	}
	return 0;
}
int  pDrawGo(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
	
	if(!myIcon || !DRect)return 1;
	
	if(myIcon->uDraw){
	    return (*myIcon->uDraw)(myIcon,CurrentFrame,DRect);
	}
	return 1;
}
