#define EXTERN22 extern
#include "Xappl.h"
#include "uDialog.h"
#include "uLib.h"

#include <stdarg.h>

struct dialogFile{
    unsigned long dialogStart;
    uDialogList *uList;
    long KeyTicks;
    IconPtr myIcon;
    Window  mywindow;
    char label[256];
    uRect wRect;
    int ret;
    GC gc;
    GC gcX;
    int (*firstAction)(void *firstData,struct dialogStruct *dialog);
    void *firstData;
    double KeyTime;
    unsigned long dialogEnd;
};

typedef struct dialogFile *dialogPtr;


#define DIALOG_START 0x55667788

#define DIALOG_END   0x88776655

double rtime(void);

int SetDefaultPaletteName(IconPtr myIcon,char *name1);

int uInsetRect(uRect *r,int x,int y);
int GetDefaultPaletteName(IconPtr myIcon,char *name1);

int SetMyIconWindowFront(IconPtr myIcon);

static int doConfigureNotify(IconPtr myIcon,XEvent *myevent);
static int doExpose(IconPtr myIcon,XEvent *myevent);
static int doNULL(IconPtr myIcon);
static int doButtonPress(IconPtr myIcon,XEvent *myevent);
static int doControls(IconPtr myIcon,XEvent *myevent);
static int doKeyPress(IconPtr myIcon,XEvent *myevent);
static int doEnterNotify(IconPtr myIcon,XEvent *myevent);
static int doMotionNotify(IconPtr myIcon,XEvent *myevent);
static int doLeaveNotify(IconPtr myIcon,XEvent *myevent);
static void tMoveControls(IconPtr myIcon);

IconPtr uStartStatusWindow(IconPtr myIcon,char *header);
int uWriteStatusWindow(IconPtr StatusWindow,char *buff);
int uCheckStatusWindow(IconPtr StatusWindow,char *Message);
int uStopStatusWindow(IconPtr StatusWindow);
int PutChildWindow(IconPtr myIcon,Window newChild);

static void fUpdateWrite(IconPtr myIcon);

static int fClose(IconPtr myIcon);

static int MemoryLoop(IconPtr myIcon);

int NotMyWindow(IconPtr myIcon,XEvent *event);

static int SetButtons(dialogPtr s);

int getUniversalPaletteByName(char *name,unsigned char **pal);

int SetFillAttributes(IconPtr myIcon,struct FillAttributes *FillData);

unsigned long ColorsLong(XColor *colors);

int uSetForeColor(int r,int g,int b,IconPtr myIcon);
int uSetBackColor(int r,int g,int b,IconPtr myIcon);

int uSetForeColor(int r,int g,int b,IconPtr myIcon)
{
	XColor Colorv;
	unsigned long c;
	
	if(!myIcon)return 1;
	
    Colorv.red=(r << 8);
    Colorv.green=(g << 8);
    Colorv.blue=(b << 8);
	    
    c=ColorsLong(&Colorv);

	XSetForeground (xg.mydisplay, myIcon->gc,c);
	    
	return 0;	    
}
int uSetBackColor(int r,int g,int b,IconPtr myIcon)
{
	XColor Colorv;
	unsigned long c;
	
	if(!myIcon)return 1;
	
    Colorv.red=(r << 8);
    Colorv.green=(g << 8);
    Colorv.blue=(b << 8);
	    
    c=ColorsLong(&Colorv);

	XSetBackground (xg.mydisplay, myIcon->gc,c);
	    
	return 0;	    
}

int SetFillAttributes(IconPtr myIcon,struct FillAttributes *FillData)
{
	unsigned char r,g,b;
	unsigned char *pal;
	char name[256];
	
	
	if(!myIcon)return 1;

	if(GetDefaultPaletteName(myIcon,name)){
	   mstrncpy(name,"ps",255);
	}

	
	if(!getUniversalPaletteByName(name,&pal)){
		r=pal[3*FillData->ForeGround];
		g=pal[3*FillData->ForeGround+1];
		b=pal[3*FillData->ForeGround+2];
		uSetForeColor(r,g,b,myIcon);

		
		r=pal[3*FillData->BackGround];
		g=pal[3*FillData->BackGround+1];
		b=pal[3*FillData->BackGround+2];

		uSetBackColor(r,g,b,myIcon);
			
	    return 0;
	}
	return 1;
}
uDialogList *uDialogGetList(IconPtr myIcon)
{
	uDialogList *uList;
	dialogPtr s;
	
	if(!myIcon|| (s=(dialogPtr)myIcon->DWindow) == NULL)return NULL;
	uList=s->uList;
	return uList;
}
int FindFound(IconPtr myIconSearch,char *string,int flag)
{
	uDialogReturn *uR;
 	uDialogList *uList;
	IconPtr p,iclose;
	dialogPtr s;
	int ret;

	uR=NULL;
	p=IconRoot;
	iclose=NULL;
	ret=0;
	while(p){
	    if(p->WindowType == xChangeType){
	    	s=(dialogPtr)p->DWindow;
	    	if(!s)continue;
	    	uList=s->uList;
	    	if(!uList)continue;	 
	    	if(flag == 0){  	
				uR=uDialogGetReturn(uList);
			    if(!uR){
				    Warning("Error Getting Search String Specified");
				    return 1;
			    }
				string[0]=0;
				mstrncpy(string,uR[5].sreturn,255);
		        iclose=p;
	        }else{
				uList->myIconItem=myIconSearch;
				SetMyIconWindowFront(uList->myIcon);
				ret=1;	        
	        }
			break;
	    }else if(p->WindowType == xFindType){
	    	s=(dialogPtr)p->DWindow;
	    	if(!s)continue;
	    	uList=s->uList;
	    	if(!uList)continue;
	    	if(flag == 0){
				uList->myIconItem=myIconSearch;
				SetMyIconWindowFront(uList->myIcon);
				ret=1;
			}else{
				uR=uDialogGetReturn(uList);
			    if(!uR){
				    Warning("Error Getting Search String Specified");
				    return 1;
			    }
				string[0]=0;
				mstrncpy(string,uR[3].sreturn,255);
		        iclose=p;
			}
			break;
	    }
	    p=p->IconNext;
	}

	if(iclose)(*iclose->pClose)(iclose);
	
    if(uR)uDialogFreeReturn(uR);

	return ret;
}

int uDialog(uDialogList *uList)
{
	char title[256];
	struct textData *text;
	controlPtr button;
    IconPtr myIcon;
	dialogPtr d;
	uRect r;
	int ret;
	long n;
	long nnMenu;
	
    if(!uList)return 1;
    myIcon=uList->myIcon;
    if(!myIcon)return 1;
    d=(dialogPtr)myIcon->DWindow;
    if(!d)return 1;
    
	ret=1;
	
	if(!uList->isOpen){
	    nnMenu=0;
	    if(uList->menuCount > 0){
	    	uList->mychild=(Window  *)cMalloc(uList->menuCount*sizeof(Window),9827);
	    	if(!uList->mychild)goto EndIt;
	    }	
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    
			uD = &uList->uDialogItemList[n];
			
			r.y=uD->Rect.y;
			r.x=uD->Rect.x;
			r.xsize=uD->Rect.xsize;
			r.ysize=uD->Rect.ysize;
			
			uD->IsMaped=TRUE;
					
		    switch(uD->type){
	           case uDialogNone:
	           break;
	           case uDialogButton:
	               mstrncpy(title,uD->name,256);
	               if(!strcmp(title,"Ok"))uD->type=uDialogButtonOK;
	               if(!strcmp(title,"Cancel"))uD->type=uDialogButtonCancel;
	               
				    button=ControlCreate(myIcon,&r,title,
					  controlTypeButton,1,0L,
					  1L,0L,(long)n,2000L);
				    if(button == NULL){
				        Warning("doRange Error Creating radio Button");
				        goto EndIt;
				    }
	               
	               uD->buttonList.button=button;
					XSetFont(xg.mydisplay,button->gc,xg.font12);
					button->info=xg.font12info;
	               break;
	           case uDialogScrollList:
	           
		        	uD->Scroll=ScrollListNew(uD->Rect.y,uD->Rect.x,20,315,5,myIcon);
	        		if(uD->Scroll == NULL){
	            		Warning("ScrollListNew Error in doScale");
	            		goto EndIt;
	       			 }
	       			 ScrollListFitRect(&r,uD->Scroll);
           			 uD->scrollList.scroll=uD->Scroll;
	           break;
	           case uDialogString:
	                uD->stringList=StringNew(uD->name,&r,StringLeft,d->mywindow);
	            	if(!uD->stringList)goto EndIt;
					StringFont(uD->stringList,12); 
	           break;
	           case uDialogText:
                 	text=TextNew(&r,d->mywindow);
	            	if(!text)goto EndIt;
					uD->textList.text=text;	
					title[0] = 0;
	            	mstrncpy(title,uD->name,255);
	         		if(TextPut(title,text))goto EndIt;
	           break;
	           case uDialogPopUpMenuControl:
	           {
			        static struct MenuItemsInsert Items[]={
						   {"About...",0,},
						   {"----",0,},
						   {"New",0,},
						   {"Open...",0,},
						   {"Close",0,},
						   { "----",0,},
						   { "Save Images...",0,},
						   { "----",0,},
						   { "Quit",0,},
						   { NULL,0,},
					};
		           mstrncpy(title,uD->name,256);      
		           uD->popList.menubar=MenuBarNew(&r,d->mywindow);   
		           if(!uD->popList.menubar)goto EndIt; 
		           uD->popList.menu=MenuBarAddMenus(title,Items,uD->popList.menubar);		           					
		           if(!uD->popList.menu)goto EndIt;
		           
		           uD->popList.menu->menuRect=uD->popList.menubar->menuRect;
		           uD->popList.control.controlType=controlTypePopUpMenu;
		           uD->popList.control.controlData.menubar=uD->popList.menubar;
		           uD->popList.control.controlData.menu=uD->popList.menu;
		           uList->mychild[nnMenu]=uD->popList.menubar->menuWindow;
		           PutChildWindow(uList->myIcon,uList->mychild[nnMenu++]);
		       } 
	           break;
	           case uDialogButtonRadio:
		            mstrncpy(title,uD->name,256);           
				    button=ControlCreate(myIcon,&r,title,
					  controlTypeRadio,1,0L,
					  1L,0L,(long)n,(long)uD->RadioGroup);
				    if(button == NULL){
				        Warning("doRange Error Creating radio Button");
				        goto EndIt;
				    }
	               
					uD->radioList.radio=button;
					uSetControlValue(button,(long) uD->isStart);
					XSetFont(xg.mydisplay,button->gc,xg.font12);
					button->info=xg.font12info;
	               break;
	           case uDialogCheck:
		            mstrncpy(title,uD->name,256);           
				    button=ControlCreate(myIcon,&r,title,
					  controlTypeCheck,1,0L,
					  1L,0L,(long)n,2000L);
				    if(button == NULL){
				        Warning("doRange Error Creating radio Button");
				        goto EndIt;
				    }
	               
	               	uD->checkList.check=button;
	           	   	uSetControlValue(button,(long) uD->isStart);
					XSetFont(xg.mydisplay,button->gc,xg.font12);
					button->info=xg.font12info;
	               	break;
		    }
		    
		}
			    
	    FlushCurrentPort();
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	    
	   	if(uList->uDialogStart)(*uList->uDialogStart)(uList);	
   
   }else{
   		myIcon=uList->myIcon;
   }
   
	myIcon->done = 0;
   		
   
	if(uList->uDialogMove)(*uList->uDialogMove)(uList);
		
	SetButtons(d);
   
	XMapRaised(xg.mydisplay,d->mywindow);
	
	uList->isOpen = TRUE;
	
	myIcon->doNotClose=1;
	
	if(!uList->ModeLess){
	    MemoryLoop(d->myIcon);
	}else{
	   myIcon->done = 1;
	}
	
	myIcon->doNotClose=0;	
   
	ret=0;
   
EndIt:

    if(ret){
        ;
	}else{
	    if(myIcon->done != 1)ret = 1;
	}
	    
	myIcon->done=0;
	
	return ret;
}
static int SetButtons(dialogPtr s)
{
	uDialogList *uList;
	if(!s)return 1;
	uList=s->uList;
	if(!uList)return 0;
	
	if(uList->uDialogButtons)(*uList->uDialogButtons)(uList);
	
	return 0;
}
static int MemoryLoop(IconPtr myIcon)
{
	XEvent event;
	struct dialogStruct *d;
	int first;

	if(!myIcon)return 1;
	d=(struct dialogStruct *)myIcon->DWindow;
	if(!d)return 1;

	first=1;	
	while(!myIcon->done){
	/*
	    if(!XCheckMaskEvent(xg.mydisplay,StructureNotifyMask | ButtonPressMask
		     | ButtonReleaseMask | Button1MotionMask | ExposureMask 
		     | EnterWindowMask | LeaveWindowMask
		     | PointerMotionMask | KeyPressMask,&event)){
	        doNULL(myIcon);
		    Sleep2(100);
	        continue;
	    }
	*/
	
	    if(XEventsQueued(xg.mydisplay,QueuedAfterFlush) > 0){
		    XNextEvent (xg.mydisplay, &event);
	    }else{
	        doNULL(myIcon);
	        Sleep2(100);
	        continue;
	    }
	
	
	
		myIcon->myevent = &event;
		if(NotMyWindow(myIcon,&event)){
		    if(event.type == ButtonPress){
		    	myIcon->done = -1;
		    
		    }
		    continue;
		}
		switch(event.type){
		case ButtonPress:
		    if(doButtonPress(myIcon,&event))goto OutOfHere;
		break;
		case Expose:
		    doExpose(myIcon,&event);
		    if(first){
			     first=0;
			     /* InvalRectMyWindow(myIcon); */
		    }
		break;
		case ConfigureNotify:
		    doConfigureNotify(myIcon,&event);
		break;
		case KeyPress:
		    doKeyPress(myIcon,&event);
		break;
		case EnterNotify:
		    doEnterNotify(myIcon,&event);
		break;
		case MapNotify:
		break;
		case LeaveNotify:
		break;
		case MotionNotify:			
		    doMotionNotify(myIcon,&event);
		break;
		case ButtonRelease:
		    ;
		break;
#ifdef CleanExit
		case ClientMessage:
		{
		Atom wm_protocols = XInternAtom(xg.mydisplay, "WM_PROTOCOLS", True);			
			if (event.xclient.message_type == wm_protocols)
			{
				if ((Atom) (event.xclient.data.l[0]) == myIcon->wm_delete_window)
				{
		    		myIcon->done = -1;
				}
			}

		}
		break;
#endif
		default:
		;
		/*
		    printf("MemoryLoop  event.type %d MotionNotify %d\n",event.type,MotionNotify);
		*/
		}
	}
OutOfHere:
	if(myIcon)myIcon->myevent = NULL;
	return 1;
}

static int doExpose(IconPtr myIcon,XEvent *myevent)
{
	Window  mywindow;
	dialogPtr d;
	XEvent event;

	mywindow=myIcon->myWindow;
	d=(dialogPtr)myIcon->DWindow;

	if(myevent->xexpose.window == mywindow){
	    if(myevent->xexpose.count != 0)return 0;
/*
	    printf("Expose xexpose.window %ld\n",myevent->xexpose.window);
*/
	    uDialogUpdate(d->uList);

	    while(1){
		if(XCheckWindowEvent(xg.mydisplay,mywindow,ExposureMask,&event)){
		    continue;
		}else{
		    break;
		}
	    }
	}

	return 0;
}
int uDialogUpdate(uDialogList *uList)
{
	struct ScrollList *scroll;
	struct MenuBar *mMenuBar;
	controlPtr button;
	controlPtr check;
	controlPtr radio;
	uDialogItem *uD;
	long n;

	    
	if(!uList)return 1;
	
	
	for(n=0;n<uList->uDialogItemCount;++n){
	    
	    uD = &uList->uDialogItemList[n];
	    
		
	    switch(uD->type){
        case uDialogButton:
        case uDialogButtonOK:
        case uDialogButtonCancel:
            button=uD->buttonList.button;
			if(button){			
		        if(uD->IsMaped && uD->isHidden){
			    	XUnmapWindow(xg.mydisplay, button->controlwindow);
		            uD->IsMaped=FALSE;
		        }else if(!(uD->IsMaped) && !(uD->isHidden)){
			    	XMapWindow(xg.mydisplay, button->controlwindow);
		            uD->IsMaped=TRUE;
		        }
				button->hide=uD->isHidden;
			}	           	    
		break;
		case uDialogButtonRadio:
		    radio=uD->radioList.radio;
			if(radio){			
		        if(uD->IsMaped && uD->isHidden){
			    	XUnmapWindow(xg.mydisplay, radio->controlwindow);
		            uD->IsMaped=FALSE;
		        }else if(!(uD->IsMaped) && !(uD->isHidden)){
			    	XMapWindow(xg.mydisplay, radio->controlwindow);
		            uD->IsMaped=TRUE;
		        }
				radio->hide=uD->isHidden;
			}	           	    
		   break;
		case uDialogCheck:
		    check=uD->checkList.check;
			if(check){			
		        if(uD->IsMaped && uD->isHidden){
			    	XUnmapWindow(xg.mydisplay, check->controlwindow);
		            uD->IsMaped=FALSE;
		        }else if(!(uD->IsMaped) && !(uD->isHidden)){
			    	XMapWindow(xg.mydisplay, check->controlwindow);
		            uD->IsMaped=TRUE;
		        }
				check->hide=uD->isHidden;
			}	           	    
		break;	            
        case uDialogPopUpMenuControl:
            mMenuBar=uD->popList.menubar;
			if(mMenuBar){			
		        if(uD->IsMaped && uD->isHidden){
			    	XUnmapWindow(xg.mydisplay, mMenuBar->menuWindow);
		            uD->IsMaped=FALSE;
		        }else if(!(uD->IsMaped) && !(uD->isHidden)){
			    	XMapWindow(xg.mydisplay, mMenuBar->menuWindow);
		            uD->IsMaped=TRUE;
		        }
				mMenuBar->hide=uD->isHidden;
			}	           	    
        break;
		case uDialogScrollList:
		    scroll=uD->Scroll;
			if(scroll){			
		        if(uD->IsMaped && uD->isHidden){
				    XUnmapWindow(xg.mydisplay, scroll->control1->controlwindow);
				    scroll->control1->hide = 1;
				    XUnmapWindow(xg.mydisplay, scroll->control2->controlwindow);
				    scroll->control2->hide = 1;
		            uD->IsMaped=FALSE;
		        }else if(!(uD->IsMaped) && !(uD->isHidden)){
				    XMapWindow(xg.mydisplay, scroll->control1->controlwindow);
				    scroll->control1->hide = 0;
				    XMapWindow(xg.mydisplay, scroll->control2->controlwindow);
				    scroll->control2->hide = 0;
		            uD->IsMaped=TRUE;
		        }
			}	           	    
		break;	  
		
		case uDialogText:
		    if(uD->isActive && !uD->isHidden){
				if(n == uList->activeText || (uList->activeText == -1)){
			    	if(uD->textList.text)TextSelect(uD->textList.text);
			    	uList->activeText = n;
				}else{
			    	if(uD->textList.text)TextDeSelect(uD->textList.text);
				}
		    }
		          
		break;	  
	    }
	}
	
	fUpdateWrite(uList->myIcon);
	return 0;
}
static void fUpdateWrite(IconPtr myIcon)
{
	dialogPtr s;
	uDialogList *uList;
	/*
	uRect r;
	*/
    long n;
	
	if(myIcon && (s=(dialogPtr)myIcon->DWindow) != NULL){
	
		XFillRectangle(xg.mydisplay,myIcon->myWindow,
					   s->gc,
					   0,0,
					   myIcon->windowRect.xsize,
					   myIcon->windowRect.ysize);
	
		
	    uList=s->uList;
	    if(uList){
			for(n=0;n<uList->uDialogItemCount;++n){
			    uDialogItem *uD;
			    
			    uD = &uList->uDialogItemList[n];
			    
				
			    switch(uD->type){
	            case uDialogButton:
	            case uDialogButtonOK:
	            case uDialogButtonCancel:
					if(uD->buttonList.button ){
					   uD->buttonList.button->hide=uD->isHidden;
					}	           	    
				break;
				case uDialogButtonRadio:
					if(uD->radioList.radio ){
					   uD->radioList.radio->hide=uD->isHidden;
					}
				   break;
				case uDialogCheck:
					if(uD->checkList.check ){
					   uD->checkList.check->hide=uD->isHidden;
					}
				break;	            
	            case uDialogScrollList:
	           	    if(!uD->isHidden && uD->Scroll){	           	    
	           	        ScrollListUpdate(uD->Scroll);
	           	    }
	            break;
	            case uDialogString:
	           	    if(!uD->isHidden){
	    			    if(uD->stringList)StringUpdate(uD->stringList);
	           	    }
	            break;
	            case uDialogText:
	           	    if(!uD->isHidden){
					 	if(uD->textList.text){
					 	   TextUpdate(uD->textList.text);
					 	}			
	           	    }
	            break;
	            case uDialogFillRect:
	            	SetFillAttributes(myIcon,&uD->FillData);
					XFillRectangle(xg.mydisplay,myIcon->myWindow,
					   myIcon->gc,
					   uD->Rect.x,uD->Rect.y,
					   uD->Rect.xsize,
					   uD->Rect.ysize);
					   uSetForeColor(255,255,255,myIcon);
					   uSetBackColor(0,0,0,myIcon);
	            break;	            
	            case uDialogFrameRect:
	            	SetFillAttributes(myIcon,&uD->FillData);
	    			uFrameRect(&uD->Rect,myIcon);
	            	uSetForeColor(0,0,0,myIcon);
					uSetBackColor(255,255,255,myIcon);
	            break;	            
	            case uDialogPopUpMenuControl:
	           	    if(!uD->isHidden){
					 	if(uD->popList.menubar){
					 	   MenuBarUpdate(uD->popList.menubar);
			    	    }
			
	           	    }
	            break;
			    }
			}
	 
			if(uList->uDialogUserDraw){
			    (*uList->uDialogUserDraw)(uList);
			}
	    }
	    
		uDrawControls(myIcon);
	}
}
int uDialogSet(uDialogList *uList,long Item,...)
{
	struct ScrollList **List;
	struct ScrollList *Scroll;
    controlPtr *Control;
	char buff[256];
    va_list ap;
    int command,type;
    int isActive,lflag;
    long *plflag;
    char *name;
    uRect  *Rect;
    IconPtr myIcon,myIconItem;
    int (*uDialogMove)(struct uDialogStruct *uList);
    int (*uDialogStart)(struct uDialogStruct *uList);
    int (*uDialogAction)(struct uDialogStruct *uList);
    int (*uDialogButtons)(struct uDialogStruct *uList);
    int (*uDialogKeys)(struct uDialogStruct *uList);
    typedef int (*fReturn)(struct uDialogStruct *uList);
    uDialogItem *uD;
    double value,*pvalue;
    long *lvalue,valuel;
    int ix,iy;
    int returnType;
    int fontType;

        
	if(!uList)return 1;
	if(Item < 0 || Item >= uList->uDialogItemCount)return 1;
	
	plflag=NULL;
	
	va_start(ap,Item);
	while((command=va_arg(ap,int)) != uDialogSetDone){
	    switch(command){
           case uDialogSetExitOk:
				myIcon=uList->myIcon;
	 			if(!myIcon)return 1;
	            myIcon->done = 1;
           break;
           case uDialogSetExitCancel:
				myIcon=uList->myIcon;
	 			if(!myIcon)return 1;
	            myIcon->done = -1;
           break;
           case uDialogSetType:
               type=va_arg(ap,int);
               uList->uDialogItemList[Item].type=type;
               uList->uDialogItemList[Item].isActive=TRUE;
               if(type == uDialogPopUpMenuControl){
                   ++uList->menuCount;
               }
           break;
           case uDialogSetName:
               name=va_arg(ap,char *);
               if(!name)return 1;
               mstrncpy(uList->uDialogItemList[Item].name,name,256);
           break;
           case uDialogSetRect:
               Rect=va_arg(ap,uRect  *);
               if(!Rect)return 1;
               uD = &uList->uDialogItemList[Item];
               uD->Rect = *Rect;
			   if(uD->radioList.radio){
                    /* MoveControl(uD->radioList.radio,ix,iy); */
			   }
			   if(uD->checkList.check){
                    /* MoveControl(uD->checkList.check,ix,iy); */
			   }
               if(uD->buttonList.button){
                    /* MoveControl(uD->buttonList.button,ix,iy); */
               }
			   if(uD->popList.menubar){
			      /* MenuBarMove(uD->popList.menubar,ix,iy); */
			   }
			   if(uD->stringList){
			      /* StringMove(uD->stringList,ix,iy); */
			   }
			   if(uD->textList.text){
			      TextMove(uD->textList.text,Rect->x,Rect->y);
			      TextSize(uD->textList.text,Rect->xsize,Rect->ysize);
			   }
           break;
           
           case uDialogSetMyIcon:
               myIconItem=va_arg(ap,IconPtr);
               /* if(!myIconItem)return 1; */
               uList->myIconItem = myIconItem;
           break;
           case uDialogSetItemClose:           
               uList->pClose = (int (*)(struct Icon *))va_arg(ap,void *);           
           break;
           case uDialogSetMove:
               uDialogMove=(int (*)(struct uDialogStruct *))va_arg(ap,fReturn);
               if(!uDialogMove)return 1;
               uList->uDialogMove = uDialogMove;
           break;
           case uDialogSetUserDraw:
               uDialogAction=(int (*)(struct uDialogStruct *))va_arg(ap,void *);
               if(!uDialogAction)return 1;
               uList->uDialogUserDraw = uDialogAction;
           break;
           case uDialogSetDoubleClick:
               uDialogMove=(int (*)(struct uDialogStruct *))va_arg(ap,void *);
               if(!uDialogMove)return 1;
               uList->uDialogDoubleClick = uDialogMove;
           break;
           case uDialogSetStart:
               uDialogStart=va_arg(ap,fReturn);
               if(!uDialogStart)return 1;
               uList->uDialogStart = uDialogStart;
           break;
           case uDialogSetHidden:
               isActive=va_arg(ap,int);
               uList->uDialogItemList[Item].isHidden=isActive;
           break;
           case uDialogSetRadioGroup:
               isActive=va_arg(ap,int);
               uList->uDialogItemList[Item].RadioGroup=isActive;
           break;
           case uDialogSetAction:
               uDialogAction=va_arg(ap,fReturn);
               if(!uDialogAction)return 1;
               uList->uDialogAction = uDialogAction;
           break;
           
           case uDialogSetKeys:
               uDialogKeys=va_arg(ap,fReturn);
               if(!uDialogKeys)return 1;
               uList->uDialogKeys = uDialogKeys;
           break;
           
           case uDialogSetButtons:
               uDialogButtons=va_arg(ap,fReturn);
               if(!uDialogButtons)return 1;
               uList->uDialogButtons = uDialogButtons;
           break;
           case uDialogSetItem:
               Item=va_arg(ap,long);
			   if(Item < 0 || Item >= uList->uDialogItemCount)return 1;
           break;
           case uDialogSetActive:
               isActive=va_arg(ap,int);
               uList->uDialogItemList[Item].isActive=isActive;
           break;           
           case uDialogSetValue:
           case uDialogSetLogical:
               lflag=va_arg(ap,int);
               uD = &uList->uDialogItemList[Item];
               uD->isStart=lflag;
               if(uD->radioList.radio){
				   uSetControlValue(uD->radioList.radio,uD->isStart);
               }
               if(uD->checkList.check){
				   uSetControlValue(uD->checkList.check,uD->isStart);
               }               
			   if(uD->popList.menubar){		
			       MenuSetPopUpTitle(lflag,uD->popList.menubar);	  
			   /* 
					if(!MenuGetName(lflag,buff,uD->popList.menu)){
						MenuSetTitle(buff,uD->popList.menu);
						MenuBarUpdate(uD->popList.menubar);
					}		
				*/	   
			   }                              
           break;
           case uDialogSetFont:
           case uDialogSetFontStyle:
               fontType=va_arg(ap,int);
               	Scroll=uList->uDialogItemList[Item].Scroll;
               	//fontType=fontType;
               if(Scroll){
               /*
 					struct FontStuff f;
					if(!GetFontStuff(&f)){
						if(Scroll->ListData){
							Scroll->ListData->listTextFont=f.txFont;
							Scroll->ListData->listTextSize=f.txSize;
							Scroll->ListData->listTextFace=f.txFace;
						}
					}
				*/
               }
               
           break;
          case uDialogSetDouble:
               value=va_arg(ap,double);
               uD = &uList->uDialogItemList[Item];
	           sprintf(uD->name,"%g",value);
	           if(uD->textList.text){
			        mstrncpy(uD->textList.text->textName,uD->name,255);
	           }
           break;
          case uDialogSetLong:
               valuel=va_arg(ap,long);
               uD = &uList->uDialogItemList[Item];
	           sprintf(uD->name,"%ld",valuel);
	           if(uD->textList.text){
			        mstrncpy(uD->textList.text->textName,uD->name,255);
	           }
           break;
           case uDialogSetModeLess:
               uList->ModeLess=va_arg(ap,int);
           break;
           case uDialogSetForeColor:
               uD = &uList->uDialogItemList[Item];
               uD->FillData.ForeGround = va_arg(ap,int);
           break;
           
           case uDialogSetBackColor:
               uD = &uList->uDialogItemList[Item];
               uD->FillData.BackGround = va_arg(ap,int);
           break;
           
           case uDialogSetPattern:
               uD = &uList->uDialogItemList[Item];
               uD->FillData.Pattern = va_arg(ap,int);
           break;
          case uDialogSetFloatAux:
               value=va_arg(ap,double);
               uD = &uList->uDialogItemList[Item];
               uD->DoubleAux=value;
           break;
          case uDialogGetFloatAux:
               pvalue=va_arg(ap,double *);
               uD = &uList->uDialogItemList[Item];
               if(pvalue){
                   	*pvalue = uD->DoubleAux;
               }else{
                   return 1;
               }
           break;
          case uDialogSetText:
               name=va_arg(ap,char *);
               uD = &uList->uDialogItemList[Item];
	           mstrncpy(uD->name,name,255);
	           if(uD->textList.text){
			        mstrncpy(uD->textList.text->textName,uD->name,255);
	           }
           break;           
          case uDialogSetPalette:
                name=va_arg(ap,char *);
				myIcon=uList->myIcon;
               SetDefaultPaletteName(myIcon,name);
           break;
           case uDialogWindowType:
               uList->WindowType=va_arg(ap,int);
               uList->myIcon->WindowType=uList->WindowType;
           break;
           case uDialogSetLocation:
               ix=va_arg(ap,int);
               iy=va_arg(ap,int);
               uD = &uList->uDialogItemList[Item];
			   if(uD->radioList.radio){
                    MoveControl(uD->radioList.radio,ix,iy);
			   }
			   if(uD->checkList.check){
                    MoveControl(uD->checkList.check,ix,iy);
			   }
               if(uD->buttonList.button){
                    MoveControl(uD->buttonList.button,ix,iy);
               }
			   if(uD->popList.menubar){
			      MenuBarMove(uD->popList.menubar,ix,iy);
			   }
			   if(uD->stringList){
			      StringMove(uD->stringList,ix,iy);
			   }
			   if(uD->textList.text){
			      TextMove(uD->textList.text,ix,iy);
			   }
           break;
           case uDialogSetReturnType:
               returnType=va_arg(ap,int);
               uList->uDialogItemList[Item].returnType=returnType;
           break;
           case uDialogGetRect:
               Rect=va_arg(ap,uRect  *);
               if(!Rect)return 1;
                *Rect=uList->uDialogItemList[Item].Rect;
           break;
           case uDialogGetLogical:
               plflag=va_arg(ap,long *);
               uD = &uList->uDialogItemList[Item];
               if(plflag){
				   if(uD->radioList.radio){
                   	   *plflag = uGetControlValue(uD->radioList.radio);
				   }
				   if(uD->checkList.check){
                   	   *plflag = uGetControlValue(uD->checkList.check);
				   }
               }else{
                   return 1;
               }
           break;
          case uDialogGetDouble:
               pvalue=va_arg(ap,double *);
               uD = &uList->uDialogItemList[Item];
               if(pvalue && uD->textList.text){
                    mstrncpy(buff,uD->textList.text->textName,256);
                   	*pvalue = atof(buff);
               }else{
                   return 1;
               }
           break;
          case uDialogGetLong:
               lvalue=va_arg(ap,long *);
               uD = &uList->uDialogItemList[Item];
               if(lvalue && uD->textList.text){
                     mstrncpy(buff,uD->textList.text->textName,256);
                   	*lvalue = atol(buff);
					value=atof(buff);
					if((long)value > *lvalue)*lvalue=(long)value;         
               }else{
                   return 1;
               }
           break;
           case uDialogGetScrollList:
               List=va_arg(ap,struct ScrollList **);
               if(!List || !uList->uDialogItemList[Item].Scroll)return 1;
                *List=uList->uDialogItemList[Item].Scroll;
           break;
           case uDialogGetControl:
               Control=va_arg(ap,controlPtr *);
               if(!Control)return 1;
               uD = &uList->uDialogItemList[Item];
				if(uD->radioList.radio){
				   *Control=uD->radioList.radio;
				}else if(uD->checkList.check){
				   *Control=uD->radioList.radio;
				}else if(uD->buttonList.button){
				   *Control=uD->buttonList.button;
				}else if(uD->popList.menubar){
				   *Control= &uD->popList.control;
				}else{
					return 1;
				}
           break;
           
           case uDialogGetControlMenu:
				Control=va_arg(ap,controlPtr *);
				if(!Control)return 1;
				uD = &uList->uDialogItemList[Item];
				if(uD->popList.menubar){
					*Control=(struct controls *)uD->popList.menu;
				}else{
					return 1;
				}
           break;
           
	    }
	}
	return 0;
}
uDialogReturn *uDialogGetReturn(uDialogList *uList)
{
    uDialogReturn *uReturn,*uR;
	double value;
    long n;
    
	if(!uList || (uList->uDialogItemCount <= 0))return NULL;
	
	uReturn=(uDialogReturn *)cMalloc(sizeof(uDialogReturn)*uList->uDialogItemCount,923872);
	zerol((char *)uReturn,sizeof(uDialogReturn)*uList->uDialogItemCount);
	
	
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    
			uD = &uList->uDialogItemList[n];
			uR = &uReturn[n];
			uR->returnType=uD->returnType;
			switch(uD->type){
	           case uDialogNone:
	           break;
	           case uDialogButton:
	               mstrncpy(uR->sreturn,uD->name,256);
	               break;
	           case uDialogScrollList:
	               mstrncpy(uR->sreturn,uD->name,256);
	           break;
	           case uDialogFloatAux:
	               uR->dreturn=uD->DoubleAux;
	           break;
	           case uDialogText:
		            mstrncpy(uR->sreturn,uD->name,256);
		            if(uD->textList.text){
		                mstrncpy(uR->sreturn,(char *)(uD->textList.text),255);
		            }
		            if(uD->returnType == uDialogGetLong){
		                value=atof(uR->sreturn);
	                   	uR->lreturn = atol(uR->sreturn);
	                   	if((long)value > uR->lreturn)uR->lreturn=(long)value;         
		            }else if(uD->returnType == uDialogGetDouble){
	                   	uR->dreturn = atof(uR->sreturn);	            
		            }
	           break;
	           case uDialogButtonRadio:
		            mstrncpy(uR->sreturn,uD->name,256);
		            if(uD->returnType == uDialogGetLong){
	                   	if(uD->radioList.radio){
	                   	    uR->lreturn = uGetControlValue(uD->radioList.radio);	 
	                   	}           
		            }else if(uD->returnType == uDialogGetDouble){
	                   	if(uD->radioList.radio){
	                   	    uR->dreturn = (double)uGetControlValue(uD->radioList.radio);	  
	                   	}          
		            }
	            break;
	           case uDialogPopUpMenuControl:
		            mstrncpy(uR->sreturn,uD->name,256);
		            if(uD->returnType == uDialogGetLong){
	                    uR->lreturn = uGetControlValue(&uD->popList.control);	 
		            }else if(uD->returnType == uDialogGetDouble){
	                    uR->dreturn = (double)uGetControlValue(&uD->popList.control);	  
		            }
	            break;
	           case uDialogCheck:
		            mstrncpy(uR->sreturn,uD->name,256);
		            if(uD->returnType == uDialogGetLong){
	                   	if(uD->checkList.check){
	                   	    uR->lreturn = uGetControlValue(uD->checkList.check);	
	                   	}            
		            }else if(uD->returnType == uDialogGetDouble){
	                   	if(uD->checkList.check){
	                   	    uR->dreturn = (double)uGetControlValue(uD->checkList.check);
	                   	}	            
		            }
	            break;
		    }
		    
		}
	
	return uReturn;
}
uDialogList *uDialogOpen(char *name,uRect *Rect,long Count)
{
    uDialogItem *uDialogItemList;
	uDialogList *uList;
	dialogPtr d;
	int ret;
	
	if(!name || !Rect || (Count < 1))return NULL;
	
	uList=NULL;
	uDialogItemList=NULL;
	
	ret = 1;
	
	uList=(uDialogList *)cMalloc(sizeof(uDialogList),649123);
	if(!uList)goto ErrorOut;
	zerol((char *)uList,sizeof(uDialogList));
	
	uDialogItemList=(uDialogItem *)cMalloc(sizeof(uDialogItem)*Count,649122);
	if(!uDialogItemList)goto ErrorOut;
	zerol((char *)uDialogItemList,sizeof(uDialogItem)*Count);
	
	uList->uDialogItemList=uDialogItemList;
	uList->uDialogItemCount=Count;
	uList->Rect = *Rect;
	uList->RectPtr = Rect;
	
	uList->activeText = -1;
	
	mstrncpy(uList->name,name,256);
	
	d=(struct dialogFile *)cMalloc(sizeof(struct dialogFile),8371);

	if(d == NULL)goto ErrorOut;
	
	zerol((char *)d,sizeof(struct dialogFile));
	
	mstrncpy(d->label,name,256);
	
	d->dialogStart = DIALOG_START;

	d->dialogEnd   = DIALOG_END;
	
	uList->WindowType=DialogType;

	FlushAllEvents();
	
    myIcon=uCreateIcon(DialogType,Rect);

	if(myIcon == NULL){
	    WarningBatch("uDialogOpen Could Not Open Window\n");
	    goto ErrorOut;
	}
	
	myIcon->WindowClass=DialogType;
	
	if(myIcon->VControl){
	    RemoveControlFromWindow(myIcon->myWindow,myIcon->VControl);
	    ControlDisposeWindow(myIcon->VControl);
	    myIcon->VControl=NULL;
	}

	if(myIcon->HControl){
	    RemoveControlFromWindow(myIcon->myWindow,myIcon->HControl);
	    ControlDisposeWindow(myIcon->HControl);
	    myIcon->HControl=NULL;
	}

	d->myIcon=myIcon;
	d->uList=uList;
	uList->myIcon=myIcon;
	d->mywindow=myIcon->myWindow;
	
	d->KeyTime=rtime();

	myIcon->pdoConfigureNotify=doConfigureNotify;
	myIcon->pdoExpose=doExpose;
	myIcon->pClose=fClose;
	myIcon->pNullEvent=doNULL;
	myIcon->pdoButtonPress=doButtonPress;
	myIcon->pdoKeyPress=doKeyPress;
	myIcon->pdoEnterNotify=doEnterNotify;
	myIcon->pdoMotionNotify=doMotionNotify;
	myIcon->pdoLeaveNotify=doLeaveNotify;
	myIcon->pMoveControls=tMoveControls;
/*
	myIcon->pdoFocusOut=doFocusOut;
	myIcon->pdoFocusIn=doFocusIn;
	myIcon->pdoMotionNotify=doMotionNotify;
*/

	myIcon->DWindow=(struct DumWindow*)d;

	d->gc = XCreateGC (xg.mydisplay, myIcon->myWindow, 0, 0);
	d->gcX = XCreateGC(xg.mydisplay,myIcon->myWindow,0,0);  


	{
		    unsigned long c[10];

	    if(!getColors(c)){
		    XSetBackground (xg.mydisplay, d->gc, c[2]);
		    XSetForeground (xg.mydisplay, d->gc, c[2]);
	    }
	    
		XSetForeground (xg.mydisplay,d->gcX,c[4]);

		XSetBackground (xg.mydisplay,d->gcX,c[5]);
	}


	XSetFont(xg.mydisplay,d->gc,xg.font16);

	XSelectInput (xg.mydisplay, myIcon->myWindow,
	                ButtonPressMask | KeyPressMask | ExposureMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask | PointerMotionMask |
	                StructureNotifyMask | FocusChangeMask | KeymapStateMask);

	{
	    XSizeHints hint;

	    hint.x = 0;
	    hint.y = 0;
	    hint.width  =  Rect->xsize;
	    hint.height =  Rect->ysize;
	    hint.flags = PPosition | PSize;

	    XSetStandardProperties (xg.mydisplay, myIcon->myWindow, name, name,
	        None, xg.argv, xg.argc, &hint);
	}


	
	
	
	
	ret = 0;
ErrorOut:
	if(ret){
	    uDialogClose(uList);
	    uList=NULL;
	}
	return uList;
}
int uDialogFreeReturn(uDialogReturn *uReturn)
{
	if(!uReturn)return 1;
	cFree((char *)uReturn);
	return 0;
}

int uDialogSetList(uDialogList *uList,uDialogGroup *Items,long ItemCount)
{
	long n;
	int ret;
	
	if(!uList || !Items || (ItemCount <= 0))return 1;
	
	for(n=0;n<ItemCount;++n){
	
		ret=uDialogSet(uList,(long)n,
		              uDialogSetType,Items[n].type,
		              uDialogSetName,Items[n].name,
		              uDialogSetRect,&Items[n].Rect,
		              uDialogSetReturnType,Items[n].returnType,

		              uDialogSetDone,uDialogSetDone
		    );
		if(ret)return 1;

    }
    
	
	
	return 0;
}
static int fClose(IconPtr myIcon)
{
	uDialogList *uList;
	dialogPtr s;
	
	if(myIcon){
	
		if(myIcon->doNotClose){
		     myIcon->done = -1;
		     return 1;
		}	
	
	    s=(dialogPtr)myIcon->DWindow;
  	    if(s){
			if(s->gc)XFreeGC(xg.mydisplay,s->gc);
			s->gc=NULL;
			if(s->gcX)XFreeGC(xg.mydisplay,s->gcX);
			s->gcX=NULL;
			uList=s->uList;
			if(uList){
			   uList->myIcon=NULL;
			   uDialogClose(uList);
			}
		    cFree((char *)s);
	    }
	       
	    
	    myIcon->DWindow=NULL;
	    
	    myIcon=RemoveIcon(myIcon);
	    
	}
	return 0;

}
int uDialogClose(uDialogList *uList)
{
    uDialogItem *uDialogItemList;
    IconPtr myIconItem;
    IconPtr myIcon;
    long n;
    
	if(!uList)return 1;
	
	myIcon=uList->myIcon;
    uList->myIcon=NULL;
	myIconItem=uList->myIconItem;
	
	uDialogItemList=uList->uDialogItemList;
	
	uGetPortBoundsGlobal(myIcon,uList->RectPtr);

	if(uDialogItemList){
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    
			uD = &uList->uDialogItemList[n];
			
		    switch(uD->type){
	           case uDialogNone:
	           break;
	           case uDialogButton:
	           case uDialogButtonOK:
	           case uDialogButtonCancel:
					if(uD->buttonList.button ){
					    ControlDispose(uD->buttonList.button);
					    uD->buttonList.button=NULL;
					}
	               break;
	           case uDialogScrollList:
				if(uD->Scroll ){
				    ScrollListDispose(uD->Scroll);
				    uD->Scroll=NULL;
				}
	           break;
	           case uDialogText:
				if(uD->textList.text){
				    TextDispose(uD->textList.text);
				    uD->textList.text=NULL;
				}
	           break;
	           case uDialogButtonRadio:
					if(uD->radioList.radio ){
					    ControlDispose(uD->radioList.radio);
					    uD->radioList.radio=NULL;
					}
	               break;
	           case uDialogCheck:
					if(uD->checkList.check ){
					    ControlDispose(uD->checkList.check);
					    uD->checkList.check=NULL;
					}
	           case uDialogPopUpMenuControl:
					if(uD->popList.menubar ){
					    MenuBarDispose(uD->popList.menubar);
					    uD->popList.menubar=NULL;
					}
	           case uDialogString:
	            	if(uD->stringList)StringDispose(uD->stringList);
	            	uD->stringList=NULL;
	           break;
		    }
		    
		}
	    if(uDialogItemList)cFree((char *)uDialogItemList);
	    uDialogItemList=NULL;
	}
	
	if(uList->pClose)(*uList->pClose)(myIconItem);
	
	if(uList){
	    if(uList->mychild)cFree((char *)uList->mychild);
	    uList->mychild=NULL;
	    cFree((char *)uList);
	    uList=NULL;
	}
	

	if(myIcon){
		dialogPtr s;
	    s=(dialogPtr)myIcon->DWindow;
	    if(s)s->uList=NULL;
	    myIcon->doNotClose = 0;
	    fClose(myIcon);
	}
	
	if(myIconItem){
	/*
	    SetIWindow(myIconItem->mywindow,1);
	*/
	}
	
	return 0;
}
static int doButtonPress(IconPtr myIcon,XEvent *myevent)
{
	struct textStruct *t;
	uDialogList *uList;
	uRect cellRect;
	uPoint pt;
	dialogPtr d;
	int found;
	int n;

	if(!myIcon)return 1;
	d=(dialogPtr)myIcon->DWindow;
	if(!d)return 1;
	uList=d->uList;
	if(!uList)return 1;	


	uPointer(&pt,myIcon);

	CheckControlAndShiftKeys();

	XSetInputFocus(xg.mydisplay,myIcon->myWindow,RevertToPointerRoot,CurrentTime);
	myIcon->topWindow=TRUE;

	found=FALSE;
	for(n=0;n<uList->uDialogItemCount;++n){
	    struct ScrollList *List;
	    /* long itWas,itIs; */
	    uDialogItem *uD;
	    uD = &uList->uDialogItemList[n];
		cellRect.y=uD->Rect.y;
		cellRect.x=uD->Rect.x;
		cellRect.xsize=uD->Rect.xsize;
		cellRect.ysize=uD->Rect.ysize;
		if(uD->isHidden)continue;
	    switch(uD->type){
	    case uDialogPopUpMenuControl:
	    {
  			struct MenuBar *menubar;
 	    	int Menu,Item;
  			menubar=uD->popList.menubar;
	    	if(menubar && PtInRect(&pt,&menubar->menuRect)){
 	    		if(MenuClickList(&pt,menubar,&Menu,&Item))return 0;
	        	uSetControlValue(&uD->popList.control,Item+1);
				if(uList->uDialogAction){
					uList->actionItem=n;
					if((*uList->uDialogAction)(uList))goto OutOfHere;
				}
	    	}
	    	
	    }
	    break;
	    case uDialogScrollList:
	    List=uD->Scroll;
	    if(List && PtInRect(&pt,&cellRect)) {
			if(ScrollListDoControl(myIcon,pt,List))goto OutOfHere;
			if(List->DoubleClick){
			    if(uList->uDialogDoubleClick){
					uList->actionItem=n;
					if((*uList->uDialogDoubleClick)(uList))goto OutOfHere;
				}
				List->DoubleClick=FALSE;
			}
			if(uList->uDialogAction){
				uList->actionItem=n;
				if((*uList->uDialogAction)(uList))goto OutOfHere;
			}
			ScrollListUpdate(List);
			goto OutOfHere;
	    }
	    break;
        case uDialogFillRect:
            uInsetRect(&cellRect,-2,-2);
		    if(uD->isActive  && !uD->isHidden && PtInRect(&pt,&cellRect)){
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))goto OutOfHere;
   				 }
			    goto OutOfHere;
       	     }
        break;
        case uDialogText:
	        t= &uD->textList;
		    if(PtInRect(&pt,&cellRect) && uD->isActive){
 	            TextClick(myevent,&pt,t->text);
				if(t->text && (n != uList->activeText)){
				    t->text->DoubleClick=FALSE;
				    /* TextDeSelect(t->text); */
				    if(!t->hide)TextUpdate(t->text);
				}
		 	    uList->activeText=n;
	    	    found = TRUE;
			    goto OutOfHere;
       	     }
             break;
        case uDialogUser:
		    if(uD->isActive && !uD->isHidden && PtInRect(&pt,&cellRect)){
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))return 1;
   				}
	    	    found = TRUE;
			    goto OutOfHere;
       	     }
        break;
	    }
	}


	if(doControls(myIcon,myevent))return 1;
	
OutOfHere:

	if(!FindWindowInList(myIcon))return 1;

	if(found){
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    uD = &uList->uDialogItemList[n];
			cellRect.y=uD->Rect.y;
			cellRect.x=uD->Rect.x;
			cellRect.xsize=uD->Rect.xsize;
			cellRect.ysize=uD->Rect.ysize;
		    switch(uD->type){
	        case uDialogText:
	            t= &uD->textList;
		        if(t->text){
		        	if(uList->activeText != n){
					    TextDeSelect(t->text);
		        	}
		            if(uD->isActive && !uD->isHidden)TextUpdate(t->text);
		        }
		        break;
		    }
		}
	    return 0;
	}

    SetButtons(d);
    
   if(myIcon->done && uList->ModeLess){
    	uDialogClose(uList);
    }
    
    
	return 0;

}
static int doControls(IconPtr myIcon,XEvent *myevent)
{
	uDialogList *uList;
	controlPtr control;
	dialogPtr d;
	long label;
	int part;
	long k,n;
	/* int newp; */

	if(!myIcon)return 1;
	d=(dialogPtr)myIcon->DWindow;
	if(!d)return 1;
	uList=d->uList;
	if(!uList)return 1;	
	
	if(!(part=FindControl(myevent,myIcon->myWindow,&control)))return 0;
	switch(part){

		case inButton:
	    	TrackControl(control,myevent,0l,0l);
	    label=GetControlReference(control);
		
		for(n=0;n<uList->uDialogItemCount;++n){
		    uDialogItem *uD;
		    uD = &uList->uDialogItemList[n];
	        if(label != n)continue;
		    if(uD->type == uDialogCheck){	      
		    /* Track control Flips it */
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))return 1;
   				 }
		    }else if(uD->type == uDialogButtonRadio){	 		               
				for(k=0;k<uList->uDialogItemCount;++k){
		    		uDialogItem *u;
		    		u = &uList->uDialogItemList[k];
	        		if((u->type != uDialogButtonRadio) || 
	        		   (u->RadioGroup != uD->RadioGroup ))continue;
	        	       uSetControlValue(u->radioList.radio,(k == n));
			    }
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))return 1;
   				}
		    }else if(uD->type == uDialogButtonOK){	 
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))return 1;
   				 }
		         myIcon->done = 1;
		    }else if(uD->type == uDialogButtonCancel){	 
			     myIcon->done = -1;
		    }else if(uD->type == uDialogButton){	 
   				if(uList->uDialogAction){
		            uList->actionItem=n;
   				    if((*uList->uDialogAction)(uList))return 1;
   				}
		    }
		    break;
		}
		break;
	}
	
/*
	if(FindWindowInList(myIcon)){
		if(uList)uDialogUpdate(uList);
	}
*/
	return 0;
}
static int doKeyPress(IconPtr myIcon,XEvent *myevent)
{
	uPoint pt;
	double ticks;
	/* static int count; */
	dialogPtr d;
	char text[10];
	char buff[256],*bp;
	KeySym mykey;
	unsigned long keysym;
	uDialogList *uList;
	uDialogItem *uD;
	uRect cellRect;
	int found;
	long n;
	int c,nn,nf;

	if(!myIcon)return 1;
	d=(dialogPtr)myIcon->DWindow;
	if(!d)return 1;
	uList=d->uList;
	if(!uList)return 1;	

	XLookupString (&myevent->xkey,text, 10, &mykey, 0);

	keysym=XKeycodeToKeysym(xg.mydisplay,myevent->xkey.keycode,0);
    
    //fprintf(stderr,"keysym %ld XK_LEFT %d XK_RIGHT %d\n",keysym,XK_Left,XK_Right);

	if(keysym == XK_Shift_L || keysym == XK_Shift_R)return 0; 
	

	ticks=rtime();

	uPointer(&pt,myIcon);

	found=FALSE;
	
	uD = NULL;
	
	for(n=0;n<uList->uDialogItemCount;++n){
	    uD = &uList->uDialogItemList[n];
		cellRect.y=uD->Rect.y;
		cellRect.x=uD->Rect.x;
		cellRect.xsize=uD->Rect.xsize;
		cellRect.ysize=uD->Rect.ysize;
	    switch(uD->type){
        case uDialogScrollList:
		    if(uD->isActive  && !uD->isHidden && PtInRect(&pt,&cellRect)){
		        uList->actionItem=n;
		        goto doKeys;
		    }
		    break;	    	    
        case uDialogText:
		    if(uD->isActive  && !uD->isHidden){
			    if(n == uList->activeText){
			       c=text[0];
			       if(c == '\t'){			
			       		found=FALSE;
			            nf=(int)n+1;  
TryAgain:  
						for(nn=nf;nn<uList->uDialogItemCount;++nn){
							uD = &uList->uDialogItemList[nn];
							if((uD->type == uDialogText) && uD->isActive  && !uD->isHidden){
							    uList->activeText=nn;
							    found=TRUE;
							    break;
							}
						}
						if(!found && nf != 0){
						    nf=0;
						    goto TryAgain;
						}	
						
						for(nn=0;nn<uList->uDialogItemCount;++nn){
							uD = &uList->uDialogItemList[nn];
							if((uD->type == uDialogText) && uD->isActive  && !uD->isHidden){
							    if(uList->activeText == nn){
							        TextSelect(uD->textList.text);
							    }else{
							        TextDeSelect(uD->textList.text);
							    }
								TextUpdate(uD->textList.text);
							}
						}
						
								       
                   }else{
 //                      fprintf(stderr,"keysym %ld XK_LEFT %d XK_RIGHT %d\n",keysym,XK_Left,XK_Right);
                       if(keysym == XK_Left)c=XK_Left;
                       if(keysym == XK_Right)c=XK_Right;
                       TextKey(c,uD->textList.text);
			       }
			       found=TRUE;
			       goto OutOfHere;
		    	}
       	     }
        break;
	    }
	}
OutOfHere:
	if(found){
		if(myevent->xkey.state & ControlMask){
	    	if(!uD)return 0;
			if(keysym == XK_c){
	        	TextGet(buff,uD->textList.text);
	    		putStringToScrap(buff);
			}else if(keysym == XK_x){
	        	TextGet(buff,uD->textList.text);
	    		putStringToScrap(buff);
				TextKey(uD->textList.text->textDelete,uD->textList.text);
			}else if(keysym == XK_v){
				bp=getStringFromScrap();
	        	TextPut(bp,uD->textList.text);
	        	if(bp)cFree(bp);
			}
			TextUpdate(uD->textList.text);
		}
	    return 0;
	}	
	
doKeys:
	
	if(ticks > 1.0+d->KeyTime){
 	    uList->count=0;
	}
	uList->mess[uList->count++]=text[0];
	uList->mess[uList->count]=0;
	
	
	if(uList->uDialogKeys){
	    (*uList->uDialogKeys)(uList);
	}
	
	if(FindWindowInList(myIcon)){		
		SetButtons(d);	
		d->KeyTime=rtime();	
	}

	return 0;
}
static int doNULL(IconPtr myIcon)
{
	uDialogList *uList;
	/* uRect cellRect; */
	uPoint pt;
	dialogPtr d;
	long n;

	if(!myIcon)return 1;
 	d=(dialogPtr)myIcon->DWindow;
	if(!d)return 1;
	uList=d->uList;
	if(!uList)return 1;	

	uPointer(&pt,myIcon);
	
	for(n=0;n<uList->uDialogItemCount;++n){
	    uDialogItem *uD;
	    uD = &uList->uDialogItemList[n];
	    /*
		cellRect.y=uD->Rect.y;
		cellRect.x=uD->Rect.x;
		cellRect.xsize=uD->Rect.xsize;
		cellRect.ysize=uD->Rect.ysize;
		*/
	    switch(uD->type){
        case uDialogText:
		    if(/*PtInRect(&pt,&cellRect) && */ uD->isActive && !uD->isHidden){
			    if(n == uList->activeText){
	    		   TextIdle(uD->textList.text);
			       goto OutOfHere;
		    	}
       	     }
        break;
	    }
	}
OutOfHere:

	return 0;
}
static int doMotionNotify(IconPtr myIcon,XEvent *myevent)
{
	Window  mywindow;
	uPoint where;
	dialogPtr d;
	uDialogList *uList;
	uRect cellRect;
	uPoint pt;
	int n;

	if(!myIcon)return 1;
	d=(dialogPtr)myIcon->DWindow;
	if(!d)return 1;
	uList=d->uList;
	if(!uList)return 1;	
	mywindow=myIcon->myWindow;
	if(!mywindow)return 1;

	pt.x=myevent->xmotion.x;
	pt.y=myevent->xmotion.y;


	uPointer(&where,myIcon);
	if((where.x != xg.pt.x) || (where.y != xg.pt.y)){
	    xg.pt.x=where.x;
	    xg.pt.y=where.y;
	    if(myIcon->pMouseMoved)(*myIcon->pMouseMoved)(myIcon,where);
	}

	for(n=0;n<uList->uDialogItemCount;++n){
	    uDialogItem *uD;
	    uD = &uList->uDialogItemList[n];
		cellRect.y=uD->Rect.y;
		cellRect.x=uD->Rect.x;
		cellRect.xsize=uD->Rect.xsize;
		cellRect.ysize=uD->Rect.ysize;
	    switch(uD->type){
        case uDialogText:
        

		    if(PtInRect(&pt,&cellRect) && uD->isActive && !uD->isHidden){
 	        	uSetCursor(myIcon,uBEAM_CURSOR);
	        	return 0;
       	     }
        break;
	    }
	}
 	
 	
	uSetCursor(myIcon,uARROW_CURSOR);


	return 0;
}
static int doLeaveNotify(IconPtr myIcon,XEvent *myevent)
{

	if(!myIcon)return 1;

/*
	printf("LeaveNotify %x mywindow %x\n",myevent->xcrossing.window,mywindow);
*/

	if(myevent->xcrossing.window == myIcon->myWindow){
	    uSetCursor(myIcon,uARROW_CURSOR);
            if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault, xg.colors, 4);
	}	

	return 0;
}
static int doEnterNotify(IconPtr myIcon,XEvent *myevent)
{
	Window  mywindow;

	mywindow=myIcon->myWindow;
/*
	printf("EnterNotify biff %x mywindow %x\n",myevent->xcrossing.window,mywindow);
*/
	if(myevent->xcrossing.window == mywindow){
            if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,xg.colors,4);
	}
	return 0;
}

static void tMoveControls(IconPtr myIcon)
{
	dialogPtr d;
	uDialogList *uList;
	
	if(!myIcon)return;
	d=(dialogPtr)myIcon->DWindow;
	if(!d)return;
	uList=d->uList;
	if(!uList)return;	
	
	if(uList->uDialogMove)(*uList->uDialogMove)(uList);

	return;
}
static int doConfigureNotify(IconPtr myIcon,XEvent *myevent)
{
	dialogPtr d;
	uDialogList *uList;
	int ret;
	
	if(!myIcon)return 1;
	d=(dialogPtr)myIcon->DWindow;
	if(!d)return 1;
	uList=d->uList;
	if(!uList)return 1;	

	myIcon->windowRect.y=myevent->xconfigure.y;
	myIcon->windowRect.x=myevent->xconfigure.x;
	
	ret=(myIcon->windowRect.xsize == myevent->xconfigure.width) &&
	    (myIcon->windowRect.ysize == myevent->xconfigure.height);
	if(ret)return 0;
	    
	myIcon->windowRect.xsize=myevent->xconfigure.width;
	myIcon->windowRect.ysize=myevent->xconfigure.height;
	
	if(myIcon->MenuBarList)SizeMenuBar(myIcon->MenuBarList,myevent->xconfigure.width,25);
	
	if(uList->uDialogMove)(*uList->uDialogMove)(uList);

	return 0;
}
