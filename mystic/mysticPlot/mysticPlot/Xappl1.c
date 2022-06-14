#define EXTERN22

#include "Xappl.h"
#include "BatchFile.h"
#include "Universal.h"
#include "uLib.h"
#include "VoiceCommand.h"
#ifdef MEMEORY_INFO
#include <malloc.h>
#endif

#include <sys/time.h>


struct aboutStuff{
    Window aboutWindow;
    uRect wRect;
    GC gc;

};
static int doExposeAbout(struct aboutStuff *about,XEvent *myevent,long used);
int SearchVisualList(int image_depth,int  visual_class, Visual **visual);
int getPalette(int n,char *name,char *pal);
int getXcolors(char *pal,XColor *colors);
int NotMyWindow(IconPtr myIcon,XEvent *event);

int StartX(int argc,char **argv);
int doButtonPress(IconPtr myIcon,XEvent *myevent);
int doExpose(IconPtr myIcon,XEvent *myevent);
int doEnterNotify(IconPtr myIcon,XEvent *myevent);
int doLeaveNotify(IconPtr myIcon,XEvent *myevent);
int doFocusIn(IconPtr myIcon,XEvent *myevent);
int doFocusOut(IconPtr myIcon,XEvent *myevent);
int doConfigureNotify(IconPtr myIcon,XEvent *myevent);
int doButtonRelease(IconPtr myIcon,XEvent *myevent);
int doKeyPress(IconPtr myIcon,XEvent *myevent);
int doMotionNotify(IconPtr myIcon,XEvent *myevent);
int doNULL(void);
int doEvents(void);
int doLoop(void);
int xapplWindow(XEvent *myevent);
int StartTestWindow(int argc,char **argv);

int isKeyBoardCommand(struct Icon *myIcon,int commandKey);

int SetTime(struct timeval *curTime,long milliseconds);

char *ProgramName="mysticPlot-161";

int idone;

extern struct timeval BellTime;

int FontEnd(void);

int treeClean(void);

extern int DFerror; 

IconPtr NewNewWindow(IconPtr myIcon);

int doNetWork=1;

int DefaultFont,DefaultFontSize;

struct SYSFONTLIST pSysFontList;

typedef int (*xErrorFunc)(Display *, XErrorEvent *);

static xErrorFunc xErrorHandler = 0;

static int xShowErrorHandler(Display *dpy, XErrorEvent *xevent);

int main(int argc,char **argv)
{

#ifdef MPI_ON
	int doCommands(int command);
	{
		int startMPI(int argc, char *argv[]);
		startMPI(argc,argv);
	}
	if(xg.rank==0) /* Only the Manager calls OpenGL commands. */
	{
#endif

	StartX(argc,argv);

	SetTime(&BellTime,10);
	
	 CheckControlAndShiftKeys();
	 
	if(!IsShift() && uPref.UseStartConfiguration && uPref.startConfiguration){
	    char *save=uPref.startConfiguration;
	    uPref.startConfiguration=NULL;
		readUniversal(save,1);   
		if(uPref.startConfiguration)cFree((char *)uPref.startConfiguration);
		uPref.startConfiguration=save;
	}else{
    	NewNewWindow(NULL);
    }


	doLoop();

	XCloseDisplay (xg.mydisplay);
	
#ifdef MPI_ON
	}else{
		while(1)
		{
			doCommands(0);
			Sleep2(100);
		}
	}
	{
		doCommands(1);
	}
#endif
	
	

	exit(0);

	return 0;
}
int xapplWindow(XEvent *myevent)
{
	myIcon=FindIWindow(myevent->xany.window);
	if(myIcon == NULL){
	    return 0;
	}
	doExpose(myIcon,myevent);
	return 1;
}
int doLoop(void)
{
	extern int FileManagerExit(int flag);
	extern int FilesOpenRemoveNotOwned(void);
	
	idone = 0;
	while (idone == 0) {
	    doEvents();
	}

	FontEnd();
	
	XFreeCursor(xg.mydisplay,xg.Arrow);
	XFreeCursor(xg.mydisplay,xg.Beam);
	XFreeCursor(xg.mydisplay,xg.Watch);
	XFreeCursor(xg.mydisplay,xg.PlusCursor);

	 
	if(xg.buffin)cFree((char *)xg.buffin);

	if(xg.buffout)cFree((char *)xg.buffout);

	treeClean();
	
	ZeroScrap();
	
	uZeroScrap();
    
	FileDialogRemoveInfoAll();
	
	FilesOpenRemoveNotOwned();
	
    FileManagerExit(1);
    	
	stopUniversal();
	
	DestroySysFontList(&pSysFontList);

	
	{
	   void checkall(void);

	   checkall();
	}

	return 0;
}
int doEvents(void)
{
	XEvent myevent;

	    if(XEventsQueued(xg.mydisplay,QueuedAfterFlush) > 0){
		    XNextEvent (xg.mydisplay, &myevent);
	    }else{
	        doNULL();
	        Sleep2(5);
	        return 0;
	    }

		if(myevent.type == KeymapNotify){
		    return 1;
		}
/*
		printf("Window  %lx type %ld\n",(long)myevent.xany.window,(long)myevent.type); 
*/

		myIcon=FindIWindow(myevent.xany.window);
		if(myIcon == NULL){
		     /* printf("Window Not found %x\n",myevent.xany.window); */
		     return 1;
		}

		switch(myevent.type){
		case ButtonPress:
		    doButtonPress(myIcon,&myevent);
		break;
		case Expose:
		    doExpose(myIcon,&myevent);
		break;
		case EnterNotify:
		    doEnterNotify(myIcon,&myevent);
		break;
		case LeaveNotify:
		    doLeaveNotify(myIcon,&myevent);
		break;
		case FocusIn:
		    doFocusIn(myIcon,&myevent);
		break;
		case FocusOut:
		    doFocusOut(myIcon,&myevent);
		break;
		case ConfigureNotify:
		    doConfigureNotify(myIcon,&myevent);
		break;
		case ButtonRelease:
		    doButtonRelease(myIcon,&myevent);
		break;
		case KeyPress:
		    doKeyPress(myIcon,&myevent);
		break;
		case KeymapNotify:
		    printf("KeymapNotify\n");
		break;
		case MotionNotify:
		    doMotionNotify(myIcon,&myevent);
		break;
		case MapNotify:
		/*
		    printf("MapNotify\n");
		*/
		break;
#ifdef CleanExit
		case ClientMessage:
		{
		Atom wm_protocols = XInternAtom(xg.mydisplay, "WM_PROTOCOLS", True);			
			if (myevent.xclient.message_type == wm_protocols)
			{
				if ((Atom) (myevent.xclient.data.l[0]) == myIcon->wm_delete_window)
				{
					if(myIcon->pClose){
					     (*myIcon->pClose)(myIcon);
					}
				}
			}

		}
		break;
#endif
		default:
		    ;
		   /*
		    printf("UnKnown myevent type %d\n",myevent.type);
		   */
		}

	return 1;
}
int FlushAllEvents(void)
{
	XEvent myevent;
	
    while(XEventsQueued(xg.mydisplay,QueuedAfterFlush) > 0){
	    XNextEvent (xg.mydisplay, &myevent);
	}
	
	return 0;
}
int doNULL(void)
{
	IconPtr p;
	
	p=IconRoot;
	
	while(p){		/*	Search for Window	*/
	    if(p->pNullEvent)(*p->pNullEvent)(p);
	    p=p->IconNext;
	}
	return 0;
}
int doMotionNotify(IconPtr myIcon,XEvent *myevent)
{
	uPoint pt;
	
	
	while(XCheckMaskEvent(xg.mydisplay,PointerMotionMask,myevent)){
	    ;
	}
	
	if(myIcon->pMouseMoved){
		pt.x=myevent->xmotion.x;
		pt.y=myevent->xmotion.y;
	    return (*myIcon->pMouseMoved)(myIcon,pt);
	}
	if(myIcon->pdoMotionNotify){
	    return (*myIcon->pdoMotionNotify)(myIcon,myevent);
	}
	return 0;
}
int doKeyPress(IconPtr myIcon,XEvent *myevent)
{
	unsigned long keysym;
	static int Characterlist[]={
								XK_A,XK_B,XK_C,XK_D,XK_E,
								XK_F,XK_G,XK_H,XK_I,XK_J,
								XK_K,XK_L,XK_M,XK_N,XK_O,
								XK_P,XK_Q,XK_R,XK_S,XK_T,
								XK_U,XK_V,XK_W,XK_X,XK_Y,
								XK_Z,
								XK_a,XK_b,XK_c,XK_d,XK_e,
								XK_f,XK_g,XK_h,XK_i,XK_j,
								XK_k,XK_l,XK_m,XK_n,XK_o,
								XK_p,XK_q,XK_r,XK_s,XK_t,
								XK_u,XK_v,XK_w,XK_x,XK_y,
								XK_z,
								};
	
	KeySym mykey;
	char text[10];
	int done,i;
	int n,c;
	
	text[0]=0;
	XLookupString (&myevent->xkey,text, 10, &mykey, 0);
	keysym=XKeycodeToKeysym(xg.mydisplay,myevent->xkey.keycode,0);
	if(keysym == XK_Left){
		c=leftArrow;
	}else if(keysym == XK_Right){
		c=rightArrow;
	}else if(keysym == XK_Up){
		c=upArrow;
	}else if(keysym == XK_Down){
		c=downArrow;
	}else{
	   c=text[0];
	}
		
	if(myevent->xkey.state & ControlMask){
	    c = -1;
	    for(n=0;n<sizeof(Characterlist)/sizeof(int);++n){
	        if(Characterlist[n] == keysym){
	            if(n < 26){
	                c='A'+n;
	            }else{
	                c='A'+n-26;
	            }
	            break;
	        }
	    }
	    if(c > 0){
	        if(isKeyBoardCommand(myIcon,c))return 0;
	    }
	}
	

	if(myIcon->pKeyBoard){	
	    return (*myIcon->pKeyBoard)(myIcon,c);	   
	}
	
	if(myIcon->pdoKeyPress){
	    return (*myIcon->pdoKeyPress)(myIcon,myevent);
	}
	i = XLookupString (&myevent->xkey,text, 10, &mykey, 0);
	done = 0;
	if (i == 1 && text[0] == 'q')done = 1;
	if(done){
	   myIcon=RemoveIcon(myIcon);
	   if(myIcon)return 0;
	}
	return done;
}
int doButtonRelease(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon->pdoButtonRelease){
	    return (*myIcon->pdoButtonRelease)(myIcon,myevent);
	}
	return 0;
	
}
int doConfigureNotify(IconPtr myIcon,XEvent *myevent)
{
	Window  mywindow;
	XEvent event;
	int ret;
	
	mywindow=myIcon->myWindow;
	
	if(myIcon->pMoveControls){
	    if(myevent->xconfigure.window == myIcon->myWindow){
	    

			while(1){
				if(XCheckTypedWindowEvent(xg.mydisplay,mywindow,ConfigureNotify,&event)){
					myevent=&event;
					/*
			fprintf(stderr,"doConfigureNotify eat ConfigureNotify %d %d %s\n",myevent->xconfigure.width,myevent->xconfigure.height,myIcon->WindowName);
			*/
					continue;
				}else{
					break;
				}
			}
	    
			myIcon->windowRect.y=myevent->xconfigure.y;
			myIcon->windowRect.x=myevent->xconfigure.x;
			
			
			
			ret=(myIcon->windowRect.xsize == myevent->xconfigure.width) &&
			    (myIcon->windowRect.ysize == myevent->xconfigure.height);
			if(ret)return 0;
			/*
				fprintf(stderr,"doConfigureNotify %d %d %s\n",myevent->xconfigure.width,myevent->xconfigure.height,myIcon->WindowName);
			*/
		
		/*
	XEvent event1;
			while(1){
				if(XCheckWindowEvent(xg.mydisplay,mywindow,ExposureMask,&event1)){
				
			fprintf(stderr,"doConfigureNotify eat ExposureMask\n");
			
					continue;
				}else{
					break;
				}
			}
		*/
			
			myIcon->windowRect.xsize=myevent->xconfigure.width;
			myIcon->windowRect.ysize=myevent->xconfigure.height;
			if(myIcon->MenuBarList)SizeMenuBar(myIcon->MenuBarList,myevent->xconfigure.width,25);
			(*myIcon->pMoveControls)(myIcon);	   
		}  
		return 0;
	}
	if(myIcon->pdoConfigureNotify){
	
/*	
			while(XCheckMaskEvent(xg.mydisplay,PointerMotionMask,&event)){
			    ;
			}
*/
			ret=(myIcon->windowRect.xsize == myevent->xconfigure.width) &&
			    (myIcon->windowRect.ysize == myevent->xconfigure.height);
			if(ret)return 0;
	
	    return (*myIcon->pdoConfigureNotify)(myIcon,myevent);
	}
	return 0;
	
}
/*
int doConfigureNotify(IconPtr myIcon,XEvent *myevent)
{
	int ret;
	
	if(myIcon->pMoveControls){
	    if(myevent->xconfigure.window == myIcon->myWindow){
			myIcon->windowRect.y=myevent->xconfigure.y;
			myIcon->windowRect.x=myevent->xconfigure.x;
			
			ret=(myIcon->windowRect.xsize == myevent->xconfigure.width) &&
			    (myIcon->windowRect.ysize == myevent->xconfigure.height);
			if(ret)return 0;
			
			myIcon->windowRect.xsize=myevent->xconfigure.width;
			myIcon->windowRect.ysize=myevent->xconfigure.height;
			if(myIcon->MenuBarList)SizeMenuBar(myIcon->MenuBarList,myevent->xconfigure.width,25);
			(*myIcon->pMoveControls)(myIcon);	   
		}  
		return 0;
	}
	if(myIcon->pdoConfigureNotify){
	
			ret=(myIcon->windowRect.xsize == myevent->xconfigure.width) &&
			    (myIcon->windowRect.ysize == myevent->xconfigure.height);
			if(ret)return 0;
	
	    return (*myIcon->pdoConfigureNotify)(myIcon,myevent);
	}
	return 0;
	
}
*/
int doFocusOut(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon->pdoFocusOut){
	    return (*myIcon->pdoFocusOut)(myIcon,myevent);
	}
	return 0;
	
}
int doFocusIn(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon->pdoFocusIn){
	    return (*myIcon->pdoFocusIn)(myIcon,myevent);
	}
	return 0;
	
}
int doLeaveNotify(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon->pdoLeaveNotify){
	    return (*myIcon->pdoLeaveNotify)(myIcon,myevent);
	}
	return 0;
	
}
int doEnterNotify(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon->pdoEnterNotify){
	    return (*myIcon->pdoEnterNotify)(myIcon,myevent);
	}
	return 0;
	
}
int doExpose(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon->pdoExpose){
	    return (*myIcon->pdoExpose)(myIcon,myevent);
	}
	return 0;
	
}
int doButtonPress(IconPtr myIcon,XEvent *myevent)
{
	if(myIcon && myevent){
	    myIcon->myevent=myevent;
		if(myIcon->pdoButtonPress){
		    return (*myIcon->pdoButtonPress)(myIcon,myevent);
		}
		myIcon=FindWindowInList(myIcon);
	    if(myIcon)myIcon->myevent=NULL;
	}
	return 0;
	
}

static int xShowErrorHandler(Display *dpy, XErrorEvent *xevent)
{
    
    if (xErrorHandler){
        return xErrorHandler(dpy, xevent);
    } else{
        return 0;
    }
}
	
int StartX(int argc,char **argv)
{

#define selector_width 16
#define selector_height 16
static unsigned char selector_bits[] = {
   0xff, 0xff, 0x03, 0xc0, 0x05, 0xa0, 0x09, 0x90, 0x11, 0x88, 0x21, 0x84,
   0xc1, 0x83, 0x41, 0x82, 0x41, 0x82, 0xc1, 0x83, 0x21, 0x84, 0x11, 0x88,
   0x09, 0x90, 0x05, 0xa0, 0x03, 0xc0, 0xff, 0xff};
#define mask_width 16
#define mask_height 16
static unsigned char mask_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0x0f, 0xf0, 0x1f, 0xf8, 0x3b, 0xdc, 0xf3, 0xcf,
   0xe3, 0xc7, 0x63, 0xc6, 0x63, 0xc6, 0xe3, 0xc7, 0xf3, 0xcf, 0x3b, 0xdc,
   0x1f, 0xf8, 0x0f, 0xf0, 0xff, 0xff, 0xff, 0xff};

static unsigned char fiddle_bits[] = {
   0x00, 0x60, 0x00, 0xb0, 0x00, 0xd0, 0x00, 0x68, 0xc0, 0x15, 0x20, 0x0a,
   0x20, 0x06, 0x30, 0x09, 0x9e, 0x08, 0x43, 0x08, 0x29, 0x07, 0x91, 0x01,
   0xa9, 0x00, 0x86, 0x00, 0x8c, 0x00, 0x78, 0x00};

static unsigned char fiddle_mask[] = {
   0x00, 0x60, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0x78, 0xc0, 0x1d, 0xe0, 0x0f,
   0xe0, 0x07, 0xf0, 0x0f, 0xfe, 0x0f, 0xff, 0x0f, 0xff, 0x07, 0xff, 0x01,
   0xff, 0x00, 0xfe, 0x00, 0xfc, 0x00, 0x78, 0x00};

	double factor[4]={.45,.75,1.0,1.0};
	unsigned short int red,green,blue;
	    
	Colormap color_map;
	XColor exact,screen;
	char *color,*installCMap;
	char *display;
	int n,xdepth;

	xg.argc=argc;
	xg.argv=argv;
	
	DefaultFont=0;
	
	DefaultFontSize=9;
	
    if((argc > 1) && !strcmp(argv[1],"-batch")){
	    char working[2048];
		
	    getcwd(working,2048);

		startUniversal();
		
		goCD(working);
		
		for(n=2;n<argc;++n){
		    if(processFile(argv[n]))break;
    	}
    	exit(1);
	}
	
	
	
	xErrorHandler = XSetErrorHandler( xShowErrorHandler );

	display="";

	xg.mydisplay = XOpenDisplay(display);
 
	if(xg.mydisplay == NULL){
	    fprintf(stderr,"Could Not Connect to \"%s\"\n",XDisplayName(display));
	    exit(1);
	}



	xg.myscreen = DefaultScreen (xg.mydisplay);

	xg.mybackground = WhitePixel (xg.mydisplay, xg.myscreen);
	xg.myforeground = BlackPixel (xg.mydisplay, xg.myscreen);

	xg.mybackground = 0;
	xg.myforeground = 255;
	

	CreateSysFontList(&pSysFontList);

	xg.font12 = 0;
	xg.font13 = 0;
	xg.font14 = 0;
	xg.font16 = 0;
	xg.font24 = 0;


	xg.font12info = XLoadQueryFont(xg.mydisplay,"6x12");
	if(!xg.font12info){
	    fprintf(stderr,"Could Load Font 6x12\n");
	}else{
		xg.font12 = xg.font12info->fid;
	}
	
	xg.font13info = XLoadQueryFont(xg.mydisplay,"6x13");
	if(!xg.font13info){
	    fprintf(stderr,"Could Load Font 6x13\n");
	}else{
		xg.font13 = xg.font13info->fid;
	}
	
	xg.font14info = XLoadQueryFont(xg.mydisplay,"7x14");
	if(!xg.font14info){
	    fprintf(stderr,"Could Load Font 7x14\n");
	}else{
		xg.font14 = xg.font14info->fid;
	}
	
	xg.font16info = XLoadQueryFont(xg.mydisplay,"8x16");
	if(!xg.font16info){
	    fprintf(stderr,"Could Load Font 8x16\n");
	}else{
		xg.font16 = xg.font16info->fid;
	}
	
	xg.font24info = XLoadQueryFont(xg.mydisplay,"12x24");
	if(!xg.font24info){
	    fprintf(stderr,"Could Load Font 12x24\n");
	}else{
		xg.font24 = xg.font24info->fid;
	}
	
	if(!xg.font12 || !xg.font13 || !xg.font14 || !xg.font16 || !xg.font24){
	
	    fprintf(stderr,"Error Could Load Font - Stoping\n");
		
		{
			char **name;
			int count,n;
			
			fprintf(stderr,"Fonts Found - \n");

			name=XListFonts(xg.mydisplay,"*",9000,&count);
			if(name){
				for(n=0;n<count;++n)fprintf(stderr,"%d %s\n",n,name[n]);
			}
		}
		
		exit(1);
	}


	xg.Arrow = XCreateFontCursor(xg.mydisplay,XC_left_ptr);
	xg.Beam = XCreateFontCursor(xg.mydisplay,XC_xterm);
	xg.Watch = XCreateFontCursor(xg.mydisplay,XC_watch);
	xg.PlusCursor = XCreateFontCursor(xg.mydisplay,XC_plus);


	color=NULL;
	installCMap=NULL;
	xdepth=0;
	for(n=1;n<argc;++n){
	    if(!strcmp(argv[n],"-bg")){
	        color=argv[n+1];
	        ++n;
	    }else if(!strcmp(argv[n],"-bpp")){
	        xdepth=atoi(argv[n+1]);
	        ++n;
	        if((xdepth != 24) && (xdepth != 16) && (xdepth != 15) && (xdepth != 8)){
	            fprintf(stderr,"******************************************************\n");
	            fprintf(stderr,"**       Error Screen Depth %d Not Supported        **\n",xdepth);
	            fprintf(stderr,"** xShow Only Supports Screen Depths 24,16,15,and 8 **\n");
	            fprintf(stderr,"******************************************************\n");
		    xdepth = 0;
	        }
	    }else if(!strcmp(argv[n],"-installCMap")){
	        installCMap=argv[n+1];
	        ++n;
	    }
	}




/*
	XSetStandardProperties (xg.mydisplay, mywindow, hello, hello,
	    None, argv, argc, &myhint);
*/

	{
	    Visual *xVisual;
 	    int nScreen;

	    xg.xVisual=NULL;

	    if(xdepth == 0){
	        if (!SearchVisualList((int) 24, TrueColor, &xVisual)){
		    	xg.vBits=24;
	        }else if(!SearchVisualList((int) 16, TrueColor, &xVisual)){
		    	xg.vBits=16;
	        }else if(!SearchVisualList((int) 15, TrueColor, &xVisual)){
		    	xg.vBits=15;
	        }else if(!SearchVisualList((int) 8, PseudoColor, &xVisual)){
		    	xg.vBits=8;
	        }else{
			    printf("SearchVisualList Failed !");
			    exit(1);
	        }
	    }else if(xdepth == 8){
	        if(!SearchVisualList((int) 8, PseudoColor, &xVisual)){
	            xg.vBits=8;
	        }else{
			    printf("SearchVisualList Failed for xdepth %d\n",xdepth);
			    exit(1);
	        }
	    }else{
	        if(!SearchVisualList((int) xdepth, TrueColor, &xVisual)){
	            xg.vBits=xdepth;
	        }else{
			    printf("SearchVisualList Failed for xdepth %d\n",xdepth);
			    exit(1);
	        }
	    }

	    xg.xVisual=xVisual;

	    xg.ScreenIsHigh=(BitmapBitOrder(xg.mydisplay) == MSBFirst);

	    xg.RedIsLow= (int)(xg.xVisual->red_mask & 1);

	    printf("xg.vBits %d ScreenIsHigh %d RedIsLow %d\n",xg.vBits,xg.ScreenIsHigh,
			xg.RedIsLow);

	   nScreen =DefaultScreen(xg.mydisplay);

	   if(xg.vBits == 8){
			color_map = XCreateColormap(xg.mydisplay, RootWindow(xg.mydisplay, nScreen),
                xVisual, AllocAll);
		}else{
			color_map = XCreateColormap(xg.mydisplay, RootWindow(xg.mydisplay, nScreen),
                xVisual, AllocNone);
		}

	}


	if(!installCMap || !strcmp(installCMap,"Yes")){
	    xg.installCMap=TRUE;
	}else{
	    xg.installCMap=FALSE;
	}

	if(!color)color=XGetDefault(xg.mydisplay,argv[0],"color");

	if(color){
	    if(color_map && XParseColor(xg.mydisplay,color_map,color,&screen)){
	       // color=color;
	    }else{
	        fprintf(stderr,"XParseColor Error for screen color\n");
	        color=NULL;
	    }
	}

	if(color == NULL){
	
	/* Blue */
	
		screen.red=145*256;
		screen.green=188*256;
		screen.blue=240*256;
		
	/* Gray */
	
			/*
		screen.red=238*256;
		screen.green=234*256;
		screen.blue=238*256;
		*/
	}

	for(n=0;n<4;++n){
		red=(short unsigned int)(screen.red*factor[n]);
		green=(short unsigned int)(screen.green*factor[n]);
		blue=(short unsigned int)(screen.blue*factor[n]);
		if(n == 3){
			if(screen.red >= screen.green){
				if(screen.red >= screen.blue){
					green=green*(float)(255*256)/(float)(screen.red);
					blue=blue*(float)(255*256)/(float)(screen.red);
					screen.red=255*256;
				}else{
					green=green*(float)(255*256)/(float)(screen.blue);
					red=red*(float)(255*256)/(float)(screen.blue);
					screen.blue=255*256;
				}
			}else if(screen.green >= screen.blue){
				blue=blue*(float)(255*256)/(float)(screen.green);
				red=red*(float)(255*256)/(float)(screen.green);
				screen.green=255*256;
			}else{
				green=green*(float)(255*256)/(float)(screen.blue);
				red=red*(float)(255*256)/(float)(screen.blue);
				screen.blue=255*256;
			}
		}
	    zerol((char *)&exact,sizeof(exact));
		exact.red=red;
		exact.green=green;
		exact.blue=blue;
		xg.c[n]=n+1;
		xg.colors[n]=exact;
		xg.colors[n].pixel=n+1;
		xg.colors[n].flags=DoRed | DoGreen | DoBlue;
	}

    {
		char name[256],pal[256*3];
		XColor colors[256];
		int n;

	    getPalette(12,name,pal);
		getXcolors(pal,colors);

		for(n=0;n<4;++n){
		    colors[xg.colors[n].pixel].red=xg.colors[n].red;
		    colors[xg.colors[n].pixel].green=xg.colors[n].green;
		    colors[xg.colors[n].pixel].blue=xg.colors[n].blue;
		}
        colors[xg.mybackground].red=255 << 8;
        colors[xg.mybackground].green=255 << 8;
        colors[xg.mybackground].blue=255 << 8;
        colors[xg.myforeground].red=0;
        colors[xg.myforeground].green=0;
        colors[xg.myforeground].blue=0;
		if(color_map && (xg.vBits == 8))XStoreColors(xg.mydisplay,color_map,colors,256);
	}
	
	xg.xColormapDefault=color_map;
	{
		Pixmap shape,mask;
		XColor white,black,red;
		if(color_map){
		    XParseColor(xg.mydisplay,color_map,"white",&white);
		    XParseColor(xg.mydisplay,color_map,"black",&black);
		    XParseColor(xg.mydisplay,color_map,"red",&red);
		}

		shape=XCreatePixmapFromBitmapData(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
		      (char *)selector_bits,selector_width,selector_height,1,0,1);
		mask=XCreatePixmapFromBitmapData(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
		      (char *)mask_bits,mask_width,mask_height,1,0,1);
		xg.Selector=XCreatePixmapCursor(xg.mydisplay,shape,mask,
				   &white,&black,7,7);
		shape=XCreatePixmapFromBitmapData(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
		      (char *)fiddle_bits,selector_width,selector_height,1,0,1);
		mask=XCreatePixmapFromBitmapData(xg.mydisplay,RootWindow(xg.mydisplay, xg.myscreen),
		      (char *)fiddle_mask,mask_width,mask_height,1,0,1);
		xg.Fiddle=XCreatePixmapCursor(xg.mydisplay,shape,mask,
				   &white,&black,7,7);

	}
		
	startUniversal();
	
	return 0;
}
int doQuit(void)
{
	int (*pClose)(struct Icon *);


	    while((myIcon=IconRoot) != NULL){
			if(myIcon){
			    pClose=myIcon->pClose;
			    if(pClose){
			        if((*pClose)(myIcon)){
			              pClose=NULL;
			        }
			    }
			}
			while(1){
			   if(!doEvents())break;
			}
	    }
	    idone = 1;
	    return 0;
}

int doAbout(IconPtr myIcon)
{
	struct aboutStuff about;
	Window aboutWindow;
	static uRect wRect={40,40,320,200};
#ifdef MEMEORY_INFO
	struct mallinfo info;
#endif
	long used;
	XEvent event;
	int ex;
	GC gc;

	//myIcon=myIcon;

	while(doEvents()){
	    ;
    }

#ifdef MEMEORY_INFO
	info=mallinfo();
	used=info.usmblks+info.uordblks;
#else
	used=0;
#endif


	{

	   XSetWindowAttributes attr;
	   int attr_flags;

	   attr.colormap = xg.xColormapDefault;
	   attr.event_mask = ExposureMask | StructureNotifyMask;
	   attr.border_pixel = BlackPixel(xg.mydisplay,DefaultScreen(xg.mydisplay));
	   attr.background_pixel = BlackPixel(xg.mydisplay,DefaultScreen(xg.mydisplay));
	   attr_flags = CWColormap | CWEventMask | CWBorderPixel | CWBackPixel;
	   aboutWindow = XCreateWindow(xg.mydisplay,DefaultRootWindow (xg.mydisplay),
	                    wRect.x,wRect.y,
			    wRect.xsize, wRect.ysize, 0,
			    xg.vBits, InputOutput,
			    xg.xVisual,
			    attr_flags, &attr);
	    if(aboutWindow == (Window)NULL){
			return 1;
	    }
	}



	XSelectInput (xg.mydisplay,aboutWindow,
	                ButtonPressMask | KeyPressMask | ExposureMask);

	{
	    XSizeHints hint;
	    static char *name="About";

	    hint.x = wRect.x;
	    hint.y = wRect.y;
	    hint.width  =  wRect.xsize;
	    hint.height =  wRect.ysize;
	    hint.flags = PPosition | PSize;

	    XSetStandardProperties (xg.mydisplay, aboutWindow, name, name,
	        None, xg.argv, xg.argc, &hint);
	}

	XMapRaised (xg.mydisplay, aboutWindow);

	gc = XCreateGC (xg.mydisplay, aboutWindow, 0, 0);

	about.aboutWindow=aboutWindow;
	about.wRect=wRect;
	about.gc=gc;

	ex=TRUE;


	while(1){
	     struct Icon my;
	     my.myWindow=aboutWindow;
		XMaskEvent(xg.mydisplay,ButtonPressMask | KeyPressMask | ExposureMask ,&event);
		if(NotMyWindow(&my,&event))continue;
		switch(event.type){
		case Expose:
		    doExposeAbout(&about,&event,used);
		    if(ex){

		        XGrabPointer(xg.mydisplay,aboutWindow,(Bool)False,
		        ButtonPressMask | ButtonReleaseMask,
		        GrabModeAsync,GrabModeAsync,aboutWindow,xg.Arrow,
		        CurrentTime);
		        ex=FALSE;
		    }
		    break;
		case ButtonPress:
		case KeyPress:
		   goto OutOfHere;
		default:
		    printf("doAbout  event.type %d \n",event.type);
		}
	}
OutOfHere:
	XUngrabPointer(xg.mydisplay,CurrentTime);
	if(gc)XFreeGC(xg.mydisplay,gc);
	gc=NULL;
	if(aboutWindow)XDestroyWindow (xg.mydisplay,aboutWindow);
	return 0;
}
static int doExposeAbout(struct aboutStuff *about,XEvent *myevent,long used)
{
	XFontStruct *info,*info1;
	char buff[256];
	unsigned long c[10];
	int width,y;

	static unsigned char mess[]={'C','o','p','y','r','i','g','h','t',' ',169,
	' ',' ','2','0','2','0','-','2','0','2','2','.',0};
	
	//used=used;
	if(myevent->xexpose.window == about->aboutWindow){
	    if(myevent->xexpose.count != 0)return 0;
/*
	    printf("doExposeAbout xexpose.window %ld\n",myevent->xexpose.window);
*/


		{

		    if(!getColors(c)){
			XSetBackground (xg.mydisplay, about->gc, c[2]);
			XSetForeground (xg.mydisplay, about->gc, c[2]);
		    }
		}


	    XFillRectangle(xg.mydisplay,about->aboutWindow,
	               about->gc,
	               0,0,
	               about->wRect.xsize,
	               about->wRect.ysize);

	}

	XSetFont(xg.mydisplay,about->gc,xg.font24); 
	info1=xg.font24info;
	if(!info1){
	    WarningBatch("doExposeAbout NULL font pointer\n");
	}

	width=XTextWidth(info1,ProgramName,(int)strlen(ProgramName));
	XSetForeground (xg.mydisplay,about->gc, c[4]);
	XSetBackground (xg.mydisplay,about->gc, c[2]);
	MoveTo(about->wRect.xsize/2-width/2,info1->ascent+info1->descent+5);
	DrawString((unsigned char *)ProgramName,about->aboutWindow,about->gc);

	XSetLineAttributes(xg.mydisplay,about->gc,2,LineSolid,CapRound,JoinRound);
	y=info1->ascent+info1->descent+10;
	XDrawLine(xg.mydisplay,about->aboutWindow,about->gc,0,y,about->wRect.xsize,y);

	XSetFont(xg.mydisplay,about->gc,xg.font14); 


	info=xg.font14info;
	if(!info){
	    WarningBatch("doExposeAbout NULL font pointer\n");
	    return 1;
	}
	width=XTextWidth(info,(char *)mess,(int)strlen((char *)mess));
	y = about->wRect.ysize-info->ascent-info->descent-5;
	MoveTo(about->wRect.xsize/2-width/2,y);
	DrawString((unsigned char *)mess,about->aboutWindow,about->gc);


	sprintf(buff,"by D.E. Ranta");

	width=XTextWidth(info,buff,(int)strlen(buff));
	y=info1->ascent+info1->descent+info->ascent+info->descent+15;
	MoveTo(about->wRect.xsize/2-width/2,y);
	DrawString((unsigned char *)buff,about->aboutWindow,about->gc);	
	

	sprintf(buff,"KdTree routines adapted from RayTrace 3.0");

	width=XTextWidth(info,buff,(int)strlen(buff));
	y += info->ascent+info->descent+15;
	MoveTo(about->wRect.xsize/2-width/2,y);
	DrawString((unsigned char *)buff,about->aboutWindow,about->gc);

	sprintf(buff,"Author: Samuel R. Buss, sbuss@ucsd.edu");

	width=XTextWidth(info,buff,(int)strlen(buff));
	y += info->ascent+info->descent+5;
	MoveTo(about->wRect.xsize/2-width/2,y);
	DrawString((unsigned char *)buff,about->aboutWindow,about->gc);


#ifdef MEMEORY_INFO
	sprintf(buff,"Memory Allocated : %ld",used);

	width=XTextWidth(info,buff,(int)strlen(buff));
	y += info->ascent+info->descent+5;
	MoveTo(about->wRect.xsize/2-width/2,y);
	DrawString((unsigned char *)buff,about->aboutWindow,about->gc);
#endif

	return 0;
}

int stopLanguage()
{
	return 1;
}
int startLanguage(struct VoiceCommands *Language,long Count,IconPtr myIconSpeech)
{
	if(!Language || (Count <= 0))return 1;
	
	//myIconSpeech=myIconSpeech;
	
	return 0;
}
