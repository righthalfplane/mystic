#include <stdio.h>
#include <math.h>
#include <string.h>

#define EXTERN22 extern

#include "Xappl.h"
#include "xMenu.h"
#include "Tools.h"

int IsControl(void);
int IsShift(void);

char *strsave(char *,int tag);   

int xapplWindow(XEvent *myevent);

struct popMenu{
    Window menuWindow;
    struct MenuBar *Data;
    struct Menu *menu;
    XFontStruct *info;
    uRect menuRect;
    long menuCount;
    long LineCount;
    long topline;
    int MenuBarSide;
    int isExposed;
    long whichItem;
    int whichMenu;
    int screenBits;
    int xmax;
    int ymax;
    unsigned long c[10];
    GC gc;
};

int MenuBarUpdate(struct MenuBar *Data);
int DisposePop(struct popMenu *pop);
int DrawPopPt(struct popMenu *pop);
int DrawPop(struct popMenu *pop);
struct popMenu *BuildPop(struct Menu *menu,struct MenuBar *Data);
int Pointer(uPoint *pt,Window window);
int MenuClick(struct Menu *menuList);
int MenuUpdate(struct Menu *menuList);
struct Menu *MenuNew(char *name,uRect *menuRect,Window theWindow);
int MenuBarNext(char *name,uRect *menuRect,struct MenuBar *Data);
int MenuBarAdd(struct Menu *menuListin,struct MenuBar *Data);
int MenuSize(struct Menu *menuList);
int DrawTriangle3D(uRect *controlRect,Window controlwindow,GC gc,int controlState,
                   int controlSide,unsigned long *c);

static int DrawPopItem(struct popMenu *pop,long int n,int flag);

/*
static int DrawPalette(uRect *controlRect,Window controlwindow,GC gc,XColor *colors,struct popMenu *pop);
*/

int SearchVisualList(int image_depth,int  visual_class, Visual **visual);

int getPalette(int n,char *name,char *pal);

int getXcolors(char *pal,XColor *colors);

unsigned long ColorsLong(XColor *colors);

extern char WarningBuff[1024];

int WarningBatch(char *message);

int  MenuInsert(char *name,int flag,struct Menu *menuList,int location)
{
    struct MenuItems *Items;
	long menuCount,n,nn;

	if(!menuList || !name)return 1;
	menuCount=menuList->menuCount+1;

	Items=(struct MenuItems *)cMalloc(menuCount*sizeof(struct MenuItems),8739);
	if(!Items)return 1;
	zerol((char *)Items,menuCount*sizeof(struct MenuItems));

	nn=0;
	for(n=0;n<menuCount;++n){
	    if(n == location || 
	      (n == menuCount-1 && location >=  menuCount)){
			Items[n].menuNames = strsave(name,8832);
			Items[n].menuFlags = flag;
			Items[n].colors = NULL;
	    }else{
		    if(menuList->Items){
		        Items[n]=menuList->Items[nn];
		    }
		    ++nn;
	    }
	}
	
	
	if(menuList->Items)cFree((char *)menuList->Items);
	menuList->Items=Items;
	menuList->menuCount=menuCount;

	return 0;
}
int MenuBarGetName(int n,char *name,struct MenuBar *MenuBarList)
{
	if(!name || !MenuBarList || !MenuBarList->menuList)return 1;
	if(n < 0 || n >= MenuBarList->menuCount)return 1;
	if(!MenuBarList->menuList[n] || !MenuBarList->menuList[n]->menuName)return 1;
	mstrncpy(name,MenuBarList->menuList[n]->menuName->stringName,256);
	return 0;
}

int  MenuSetPopUpTitle(int item,struct MenuBar *MenuBarList)
{
	char buff[256];
	
	if(!MenuBarList)return 1;
	
	if(!MenuGetName(item,buff,MenuBarList->menuList[0])){
		MenuSetTitle(buff,MenuBarList->menuList[0]);
		MenuBarUpdate(MenuBarList);
	}			   
	
	return 0;
}
struct MenuBar *MenuBarNew(uRect *menuRect,Window theWindow){
	struct MenuBar *Data;

	Data=(struct MenuBar *)cMalloc(sizeof(struct MenuBar),8734);

	if(Data == NULL)return (struct MenuBar *)NULL;
	
	zerol((char *)Data,(long)sizeof(struct MenuBar));

	Data->menuRect = *menuRect;
	Data->myWindow=theWindow;
	Data->MenuBarSide=2;

	Data->menuWindow = XCreateSimpleWindow (xg.mydisplay,theWindow,
	        menuRect->x,menuRect->y,
	        menuRect->xsize,menuRect->ysize,
	        0,xg.myforeground,xg.mybackground);

	XSelectInput (xg.mydisplay, Data->menuWindow,
	                ButtonPressMask | KeyPressMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask |
	                StructureNotifyMask | FocusChangeMask);

	Data->gc = XCreateGC(xg.mydisplay,Data->menuWindow,0,0);

	XSetFont(xg.mydisplay,Data->gc,xg.font13); 

	Data->info=xg.font13info;
	if(!Data->info){
	    WarningBatch("MenuBarNew info NULL\n");
	    goto ErrorOut;
	}

	if(getColors(Data->c)){
	    WarningBatch("ListNew Error Getting Colors\n");
	}

	XMapRaised (xg.mydisplay, Data->menuWindow);

	return Data;
ErrorOut:
	return NULL;
}
struct Menu *MenuBarAddMenus(char *MenuName,
                             struct MenuItemsInsert *Items,
                             struct MenuBar *MenuBarList)
{
	struct Menu *Menur;
	uRect menuRect;
	long n;

	if(!MenuName || !MenuBarList || !Items)goto ErrorOut;

	if(MenuBarNext(MenuName,&menuRect,MenuBarList))goto ErrorOut;
    Menur=MenuNew(MenuName,&menuRect,MenuBarList->menuWindow);
    if(!Menur)goto ErrorOut;

	n=0;
	while(Items[n].ItemName){
	    if(MenuAdd(Items[n].ItemName,Items[n].ItemType,Menur))goto ErrorOut;
	    ++n;
	}

	if(MenuBarAdd(Menur,MenuBarList))goto ErrorOut;

	return Menur;
ErrorOut:
	return (struct Menu *)NULL;
}
int  MenuBarColors(unsigned long *c,struct MenuBar *Data)
{
	int n;
	if(!c || !Data)return 1;

	for(n=0;n<10;++n)Data->c[n]=c[n];

	for(n=0;n<Data->menuCount;++n){
	    StringColors(c,Data->menuList[n]->menuName);
	}
	return 0;
}
int MenuBarDispose(struct MenuBar *Data)
{
	int n;

	if(!Data)return 1;
	if(Data->gc)XFreeGC(xg.mydisplay,Data->gc);
	Data->gc=NULL;
	if(Data->menuList){
	    for(n=0;n<Data->menuCount;++n){
	        if(Data->menuList[n])MenuDispose(Data->menuList[n]);
	    }
	    cFree((char *)Data->menuList);
	}

	cFree((char *)Data);
	return 0;
}
int SizeMenuBar(struct MenuBar *Data,int xsize,int ysize)
{
	if(!Data || !xg.mydisplay)return 1;
	if(xsize <= 0)xsize=20;
	if(ysize <= 0)ysize=20;
	Data->menuRect.xsize=xsize;
	Data->menuRect.ysize=ysize;
	XResizeWindow(xg.mydisplay,Data->menuWindow,xsize,ysize);
	return 0;
}
int  MenuBarMove(struct MenuBar *Data,int x,int y)
{
	long n;
	
	if(!Data)return 1;
	
	Data->menuRect.y=y;
	Data->menuRect.x=x;
	XMoveWindow(xg.mydisplay,Data->menuWindow,x,y);
	    
	for(n=0;n<Data->menuCount;++n){
		if(Data->menuList && Data->menuList[n]){
	    	Data->menuList[n]->menuRect=Data->menuRect;
		}
	}
	return 0;
}
int  MenuBarUpdate(struct MenuBar *Data)
{
    uRect menuRect;
    int n;

	if(!Data)return 1;

	menuRect = Data->menuRect;
	menuRect.y=0;
	menuRect.x=0;
	DrawRect3D(&menuRect,Data->menuWindow,
	            Data->gc,0,
	            Data->MenuBarSide,Data->c);
	for(n=0;n<Data->menuCount;++n){
	    MenuUpdate(Data->menuList[n]);
	}

	return 0;
}
int  MenuUpdate(struct Menu *menuList)
{
	if(!menuList)return 1;
	if(menuList->menuName)StringUpdate(menuList->menuName);
	return 0;
}
int DisposePop(struct popMenu *pop)
 {
	if(!pop)return 1;
	/* if(pop->info)XFreeFontInfo(NULL, pop->info, 1); */
	if(pop->gc)XFreeGC(xg.mydisplay,pop->gc);
	pop->gc=NULL;
	if(pop->menuWindow)XDestroyWindow (xg.mydisplay,pop->menuWindow);
	pop->menuWindow=0;
	cFree((char *)pop);
 	return 0;
}
int DrawPopPt(struct popMenu *pop)
{
	struct MenuBar *Data;
	uPoint pt;
	struct Menu *menu;
	XFontStruct *info;
	uRect menuRect;
	int disableFlag;
	long int n;
	int y;
	
	if(!pop)return 1;

	Data=pop->Data;
	menu=pop->menu;
	info=pop->info;

	if(!Data || !menu || !info)return 1;

	menuRect.y=0;
	menuRect.x=0;
	menuRect.xsize=pop->menuRect.xsize;
	menuRect.ysize=pop->ymax;
Start:
	Pointer(&pt,pop->menuWindow);
/*
	printf("x %d y %d\n",pt.x,pt.y);
*/
	for(n=0;n<pop->LineCount;++n){
	    y=(int)(n*pop->ymax+pop->MenuBarSide);
	    menuRect.y=y;
	    if(PtInRect(&pt,&menuRect)){
			goto found;
	    }
	}
	if(pop->whichItem != -1)DrawPop(pop);
	pop->whichItem = -1;
    return 0;
found:	
	n=n+pop->topline;
	if(pop->whichItem == n){
	    if(n == 0 && pop->topline == 0)return 0;
	    if(n != 0 && n != (pop->LineCount-1))return 0;
	}
	pop->whichItem=n;
/*
    printf("whichItem %ld\n",pop->whichItem);
*/
	if(DrawPop(pop))return 1;
	if(menu->Items[n].menuNames[0] != '-' && !menu->Items[n].disableFlag){
	    DrawRect3D(&menuRect,pop->menuWindow,
	            pop->gc,0,
	            pop->MenuBarSide,pop->c);
		disableFlag=FALSE;
	}else{
		disableFlag=TRUE;
	}
	DrawPopItem(pop,n,1);
	if(n == pop->topline){
	    if(pop->topline > 0){
	        --pop->topline;
			Sleep2(250);
	        goto Start;
	    }
	}else if((n == (pop->topline+(pop->LineCount-1))) && (menu->menuCount-pop->topline) > pop->LineCount){
	    ++pop->topline;
	    Sleep2(250);
	    goto Start;
	}
	if(disableFlag)pop->whichItem = -1;
	return 0;
}
static int DrawPopItem(struct popMenu *pop,long int n,int flag)
{
	struct Menu *menu;
	XFontStruct *info;
	int offset;
	uRect triRect;
	long nn;
	int x,y;

	if(!pop)return 1;

	menu=pop->menu;
	info=pop->info;

	if(!menu || !info)return 1;

	offset=(info->ascent+info->descent)/2-info->descent;
	x=pop->MenuBarSide+10;

	if(menu->Items[n].disableFlag){
		XSetForeground (xg.mydisplay,pop->gc,  pop->c[8]);
		XSetBackground (xg.mydisplay,pop->gc,  pop->c[2]);
	}else{
		XSetForeground (xg.mydisplay,pop->gc,  pop->c[4]);
		XSetBackground (xg.mydisplay,pop->gc,  pop->c[2]);
	}
	nn=n;
	if(pop->LineCount != menu->menuCount){
	    nn=n-pop->topline;
	    y=(int)((nn+1)*pop->ymax-pop->ymax/2+offset+pop->MenuBarSide);
	    MoveTo(x,y);
	    if((n ==  pop->topline) && n != 0){
	        DrawString((unsigned char *)"Scroll Up",
	                pop->menuWindow,pop->gc);
	        return 0;
	    }else if(n >= (pop->topline+pop->LineCount)){
	        return 0;
	    }else if((n == (pop->topline+pop->LineCount-1)) && (n != (menu->menuCount-1))){
	        DrawString((unsigned char *)"Scroll Down",
	                pop->menuWindow,pop->gc);
	        return 0;
	    }
	}
	y=(int)((nn+1)*pop->ymax-pop->ymax/2+offset+pop->MenuBarSide);
	if(menu->Items[n].menuNames[0] == '-'){
	    y=(int)((nn+1)*pop->ymax-pop->ymax/2+pop->MenuBarSide);
	    MoveTo(0,y);
	    LineTo(pop->menuRect.xsize,y,pop->menuWindow,pop->gc);
	    return 0;
	}else{
	    MoveTo(x,y);
	    DrawString((unsigned char *)menu->Items[n].menuNames,
	                pop->menuWindow,pop->gc);
	}
	    

	if((menu->Items[n].menuFlags & 0xff00) == MENU_RADIO_MARK){
	    triRect.y=(int)(n*pop->ymax+pop->MenuBarSide);
	    triRect.x=pop->MenuBarSide;
	    triRect.ysize=10;
	    triRect.xsize=10;
	    DrawTriangle3D(&triRect,pop->menuWindow,pop->gc,1,2,pop->c);
	}else if((menu->Items[n].menuFlags & 0xff00) == MENU_CHECK_MARK){
	    triRect.y=(int)((nn+1)*pop->ymax-pop->ymax/2+pop->MenuBarSide-4);
	    triRect.x=pop->MenuBarSide;
	    triRect.ysize=8;
	    triRect.xsize=8;
	    DrawRect3D(&triRect,pop->menuWindow,pop->gc,1,2,pop->c);
	}

	if((menu->Items[n].menuFlags & 0xff)== MENU_PALETTE_NAMED){
	    triRect.y=(int)(nn*pop->ymax+pop->MenuBarSide);
	    triRect.x=pop->MenuBarSide+10;
	    triRect.ysize=pop->ymax;
	    triRect.xsize=256;
		/*
	    if(flag)DrawPalette(&triRect,pop->menuWindow,pop->gc,menu->Items[n].colors,pop);
		-- broken fix by calling WriteHDFToScreen();
		*/
	}
	
		    
	return 0;
}
/*
static int DrawPalette(uRect *controlRect,Window controlwindow,GC gc,XColor *colors,struct popMenu *pop)
{
        XImage *xImage;
	char *dataPtr;
	long ix,iy,n;

	if(!controlRect || !controlwindow || !gc || !colors)return 1;

	pop=pop;

	dataPtr=(char *)cMalloc(controlRect->xsize*controlRect->ysize*4,8736);
	if(!dataPtr)return 1;

	xImage=XCreateImage(xg.mydisplay,xg.xVisual,xg.vBits,ZPixmap,0,
		   (char *)dataPtr,controlRect->xsize,controlRect->ysize,8,0);
	if(!xImage){
		if(dataPtr)cFree((char *)dataPtr);
		return 1;
	}
	if(xg.vBits == 24){
		unsigned char *ps;
		unsigned long c;
		n=0;
		for(iy=0;iy<controlRect->ysize;++iy){
			ps= (unsigned char *)(dataPtr+iy*controlRect->xsize*4);
			for(ix=0;ix<controlRect->xsize;++ix){
				c=ColorsLong(&colors[ix]);
				*ps++ = (unsigned char)((c >> 24 ) & 0xff);
				*ps++ = (unsigned char)((c >> 16 ) & 0xff);
				*ps++ = (unsigned char)((c >> 8 ) & 0xff);
				*ps++ = (unsigned char)((c) & 0xff);
			}
		}
	}else if(xg.vBits == 15 || xg.vBits == 16){
		unsigned char *ps;
		unsigned long c;
		n=0;
		for(iy=0;iy<controlRect->ysize;++iy){
			ps= (unsigned char *)(dataPtr+iy*controlRect->xsize*2);
			for(ix=0;ix<controlRect->xsize;++ix){
				c=ColorsLong(&colors[ix]);
				*ps++ = (unsigned char)((c >> 8) & 0xff);
				*ps++ = (unsigned char)((c) & 0xff);
			}
		}
	}else if(xg.vBits == 8){
		n=0;
		for(iy=0;iy<controlRect->ysize;++iy){
			for(ix=0;ix<controlRect->xsize;++ix){
				dataPtr[n++] = (char)ix;
			}
		}
	}
	
	if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,colors,256);

	XPutImage(xg.mydisplay,controlwindow,gc,xImage,
		  0, 0, controlRect->x,controlRect->y ,xImage->width,xImage->height);

	if(xImage){
	   xImage->data=NULL;
		XDestroyImage(xImage);
		xImage=NULL;
	}

	if(dataPtr)cFree((char *)dataPtr);
	dataPtr=NULL;
	
	return 0;
}
*/
unsigned long ColorsLong(XColor *colors)
{
	unsigned long cr,c0,c1,c2;
	unsigned short sr,s0,s1,s2;

	if(!colors || !xg.xVisual)return 1;

	if(xg.vBits == 24){
	    c0=(colors->red >> 8) & 0xff;
	    c1=(colors->green >> 8) & 0xff;
	    c2=(colors->blue >> 8) & 0xff;
	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
				cr = (c0) | (c1 << 8) | (c2 << 16);
	        }else{
				cr = (c0<<24) | (c1 << 16) | (c2 << 8);
	        }
	    }else{
	        if(xg.ScreenIsHigh){
				cr = (c0<<16) | (c1 << 8) | (c2);
	        }else{
				cr = (c0<<8) | (c1 << 16) | (c2 << 24);
				cr = (c2) | (c1 << 8) | (c0 << 16);
	        }
	    }
	}else if (xg.vBits == 16){
	    s0=(colors->red >> 11) & 0x1f;
	    s1=(colors->green >> 10) & 0x3f;
	    s2=(colors->blue >> 11) & 0x1f;
	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
		    sr=(s0) | (s1 << 5) | (s2 << 11);
	        }else{	
		    sr=(s0) | (s1 << 5) | (s2 << 11);
		    sr = ((sr >> 8) & 0xff) | ((sr & 0xff) << 8);
	        }
	    }else{
	        if(xg.ScreenIsHigh){
		    sr = (s0 << 11) | (s1 << 5) | (s2);
	        }else{	
		    sr = (s0 << 11) | (s1 << 5) | (s2);
		    sr = ((sr >> 8) & 0xff) | ((sr & 0xff) << 8);
	        }
	    }
	    cr=sr;
	}else if (xg.vBits == 15){
	    s0=(colors->red >> 11)   & 0x1f;
	    s1=(colors->green >> 11) & 0x1f;
	    s2=(colors->blue >> 11)  & 0x1f;
	    if(xg.RedIsLow){
	        if(xg.ScreenIsHigh){
		    sr=(s0) | (s1 << 5) | (s2 << 10);
	        }else{	
		    sr=(s0) | (s1 << 5) | (s2 << 10);
		    sr = ((sr >> 8) & 0xff) | ((sr & 0xff) << 8);
	        }
	    }else{
	        if(xg.ScreenIsHigh){
		    sr = (s0 << 10) | (s1 << 5) | (s2);
	        }else{	
		    sr = (s0 << 10) | (s1 << 5) | (s2);
		    sr = ((sr >> 8) & 0xff) | ((sr & 0xff) << 8);
	        }
	    }
	    cr=sr;
	}else{
	    cr=0;
	}
	return cr;
}


int DrawPop(struct popMenu *pop)
{
	uRect menuRect;
	struct Menu *menu;
	int n;

	if(!pop)return 1;
	menu=pop->menu;
	if(!menu)return 1;
	
    if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,xg.colors,4);
	
	menuRect=pop->menuRect;

	menuRect.y=0;
	menuRect.x=0;
	DrawRect3D(&menuRect,pop->menuWindow,
	            pop->gc,0,
	            pop->MenuBarSide,pop->c);

	for(n=0;n<menu->menuCount;++n){
	   DrawPopItem(pop,n,0);
	}
	return 0;
}
int DrawTriangle3D(uRect *controlRect,Window controlwindow,GC gc,int controlState,
                  int controlSide,unsigned long *c)
{
	XPoint top[6],bot[6],mid[4];
	int size,size2,xc,yc,n;

	if(!controlRect || !controlwindow || !gc)return 1;


	size=controlRect->ysize;
	if(size > controlRect->xsize)size = controlRect->xsize;

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

	for(n=0;n<6;++n){
	    if(n < 4){
	       mid[n].x += controlRect->x;
	       mid[n].y += controlRect->y;
	    }
	    top[n].x += controlRect->x;
	    top[n].y += controlRect->y;
	    bot[n].x += controlRect->x;
	    bot[n].y += controlRect->y;
	}

	XSetForeground (xg.mydisplay, gc, c[2]);
	XSetBackground (xg.mydisplay, gc, c[2]);
	XFillRectangle(xg.mydisplay,controlwindow,
	               gc,
	               controlRect->x,controlRect->y,
	               controlRect->xsize,
	               controlRect->ysize);

	if(controlState){ /* it is pushed */
	    XSetForeground (xg.mydisplay, gc, c[1]);
	    XSetBackground (xg.mydisplay, gc, c[1]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    mid, 4, Convex, CoordModeOrigin);


	    XSetForeground (xg.mydisplay, gc, c[0]);
	    XSetBackground (xg.mydisplay, gc, c[0]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    top, 6, Complex, CoordModeOrigin);
 
	    XSetForeground (xg.mydisplay, gc, c[3]);
	    XSetBackground (xg.mydisplay,gc, c[3]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    bot, 6, Complex, CoordModeOrigin);

	}else{  /* it is not pushed */
	    XSetForeground (xg.mydisplay,gc, c[2]);
	    XSetBackground (xg.mydisplay,gc, c[2]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    mid, 4, Convex, CoordModeOrigin);

	    XSetForeground (xg.mydisplay,gc, c[3]);
	    XSetBackground (xg.mydisplay,gc, c[3]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    top, 6, Complex, CoordModeOrigin);

	    XSetForeground (xg.mydisplay, gc, c[0]);
	    XSetBackground (xg.mydisplay, gc, c[0]);
             XFillPolygon(xg.mydisplay,controlwindow,
                    gc,
                    bot, 6, Complex, CoordModeOrigin);
  
	}
	return 0;

}
struct popMenu *BuildPop(struct Menu *menu,struct MenuBar *Data)
{
	struct popMenu *pop;
	XFontStruct *info;
	uRect menuRect;
	int width,xmax,ymax,n;
	uPoint p1,p2;
	int xc,yc;

	if(!menu || !Data)return (struct popMenu *)NULL;

	info=Data->info;
	if(!info){
	    WarningBatch("BuildPop info NULL\n");
	    return (struct popMenu *)NULL;
	}

	pop=(struct popMenu *)cMalloc(sizeof(struct popMenu),8737);
	if(pop == NULL)return (struct popMenu *)NULL;
	zerol((char *)pop,sizeof(struct popMenu));

	pop->MenuBarSide=2;
	pop->whichItem = -1;

	xmax=0;
	ymax=info->ascent+info->descent+2*pop->MenuBarSide;
	for(n=0;n<menu->menuCount;++n){
	    width=XTextWidth(info,menu->Items[n].menuNames,(int)strlen(menu->Items[n].menuNames));
	    if(width > xmax)xmax=width;
	    if((menu->Items[n].menuFlags & 0xff) == MENU_PALETTE_NAMED){
	       width=256;
	    }
	    if(width > xmax)xmax=width;
	}

	{
		p1.x=p1.y=p2.x=p2.y=0;
	    Pointer(&p1,Data->myWindow);
	    Pointer(&p2,RootWindow(xg.mydisplay,xg.myscreen));
	    xc=p2.x-p1.x;
	    yc=p2.y-p1.y;
	}

	menuRect.y=menu->menuRect.y+menu->menuRect.ysize+yc;
	menuRect.x=menu->menuRect.x+xc;
	menuRect.xsize=xmax+20+2*pop->MenuBarSide;
	menuRect.ysize=(int)(ymax*(menu->menuCount)+2*pop->MenuBarSide);
	{
	    XWindowAttributes win;
	    long LineCount;

	    XGetWindowAttributes(xg.mydisplay,RootWindow(xg.mydisplay,xg.myscreen),&win);
	    if(menuRect.y+menuRect.ysize > win.height){
	        LineCount=(win.height-menuRect.y-2*pop->MenuBarSide)/ymax;
	        if(LineCount > menu->menuCount)LineCount=menu->menuCount;
	        if(LineCount < 3)LineCount=3;
	        pop->LineCount=LineCount;
		    menuRect.ysize=(int)(ymax*(LineCount)+2*pop->MenuBarSide);
	    }else{
	        pop->LineCount=menu->menuCount;
	    }
	    pop->topline=0;

	}

	{

	   XSetWindowAttributes attr;
	   int attr_flags;

	   attr.colormap = xg.xColormapDefault;
	   attr.event_mask = ExposureMask | StructureNotifyMask;
	   attr.border_pixel = BlackPixel(xg.mydisplay,DefaultScreen(xg.mydisplay));
	   attr.background_pixel = BlackPixel(xg.mydisplay,DefaultScreen(xg.mydisplay));
	   attr_flags = CWColormap | CWEventMask | CWBorderPixel | CWBackPixel;
	   pop->menuWindow = XCreateWindow(xg.mydisplay,DefaultRootWindow (xg.mydisplay),
	                    menuRect.x,menuRect.y,
			    		menuRect.xsize, menuRect.ysize, 0,
			    		xg.vBits, InputOutput,
			    		xg.xVisual,
			    		attr_flags, &attr);
	    if(pop->menuWindow == (Window)NULL){
			return (struct popMenu *)NULL;
	    }
	}



	XSelectInput (xg.mydisplay, pop->menuWindow,
	                ButtonPressMask | KeyPressMask | ExposureMask
	    	        | ButtonReleaseMask | Button1MotionMask |  PointerMotionMask |
	                EnterWindowMask | LeaveWindowMask |
	                StructureNotifyMask | FocusChangeMask);

	pop->gc = XCreateGC(xg.mydisplay,pop->menuWindow,0,0);

	pop->Data = Data;
	pop->menu = menu;
	pop->info = info;
	pop->menuRect = menuRect;

	pop->xmax = xmax;
	pop->ymax = ymax;


	XSetFont(xg.mydisplay,pop->gc,xg.font13); 

	{
	     unsigned long valueMask;
	     XSetWindowAttributes attsPtr;

	    attsPtr.save_under=TRUE;
	    attsPtr.override_redirect=TRUE;
	    valueMask= CWOverrideRedirect | CWSaveUnder;

	    XChangeWindowAttributes(xg.mydisplay, pop->menuWindow,
                valueMask,&attsPtr);

	}



	XSetWindowColormap(xg.mydisplay,pop->menuWindow,xg.xColormapDefault);
	XMapRaised (xg.mydisplay, pop->menuWindow);
	
    if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,xg.colors,4);

    for(n=0;n<10;++n)pop->c[n]=Data->c[n];

	XSetBackground (xg.mydisplay, pop->gc, pop->c[2]);
	XSetForeground (xg.mydisplay, pop->gc, pop->c[2]);

	return pop;
}
int MenuClickList(uPoint *pt,struct MenuBar *Data,int *Menu,int *item)
{
	struct Menu *menuold,*menu;
	struct popMenu *pop,*BuildPop(struct Menu *menuold,struct MenuBar *Data);
	XEvent event;
	int n,xold,yold;
	
	if(!pt || !Data || !item)return 1;
	

	menuold=NULL;
	for(n=0;n<Data->menuCount;++n){
 	    if(PtInRect(pt,&Data->menuList[n]->menuRect)){
 	        menuold=Data->menuList[n];
 	        break;
 	    }
	}
	pop=NULL;
	if(menuold){
	    pop=BuildPop(menuold,Data);
	    if(!pop){
	        Warning("MenuClickList Error Building Menu");
	        return  1;
	    }
	    pop->whichMenu = n;
	}
	xold= -1;
	yold= -1;
	while(1){
		XMaskEvent(xg.mydisplay,ButtonReleaseMask | PointerMotionMask  /*| Button1MotionMask*/  | ExposureMask | ButtonPressMask,&event);
		switch(event.type){
		case Expose:
		    if(xapplWindow(&event))continue;
		    if(pop && (pop->menuWindow == event.xexpose.window)){
		        DrawPop(pop);
				pop->isExposed=TRUE;
				break;
		    }
		break;
		case EnterNotify:
		case MapNotify:
		break;
		case LeaveNotify:
		break;
		case MotionNotify:		   
		/* fprintf(stderr,"MotionNotify %d %d %d %d\n",xold,event.xmotion.x,yold,event.xmotion.y); */
		    if(xold != event.xmotion.x || yold != event.xmotion.y){

		        while(XCheckMaskEvent(xg.mydisplay,PointerMotionMask /*Button1MotionMask */,&event)){
		            ;
		        }
				pt->x=event.xmotion.x;
				pt->y=event.xmotion.y;
				menu=NULL;
                if(!pop || (pop->menuWindow != event.xmotion.window)){
                    for(n=0;n<Data->menuCount;++n){
                        if(PtInRect(pt,&Data->menuList[n]->menuRect)){
                            menu=Data->menuList[n];
                            break;
                        }
                    }
                }
				if(menu && (menu != menuold)){
				    menuold=menu;
				    if(pop){
				        DisposePop(pop);
				    }
				    pop=BuildPop(menuold,Data);
				    if(!pop){
				        Warning("MenuClickList Error Building Menu");
				        return  1;
				    }
				    pop->whichMenu = n;
				}
                /* printf("menu %p pop %p %d\n",menu,pop,pop->isExposed); */
				if(pop && pop->isExposed){
			        DrawPopPt(pop);
				}
		    }		    
		    xold = event.xmotion.x;
		    yold = event.xmotion.y;
		break;
		case ButtonRelease:
			/* break; */
            continue;
		   goto OutOfHere;
		case ButtonPress:
		    goto OutOfHere;
		default:
		    sprintf(WarningBuff,"MenuClickList  event.type %d MotionNotify %d\n",event.type,MotionNotify);
		    WarningBatch(WarningBuff);
		}
	}
OutOfHere:
	if(pop){
	    *Menu = pop->whichMenu;
	    *item = (int)pop->whichItem;
	    DisposePop(pop);
	    if(*item == -1){
			return 1;
	    }else{
			return 0;
	    }
	}
	return 1;
}
int Pointer(uPoint *pt,Window window)
{
		Window child,root;
		unsigned int mask;
		int press_x,press_y;
		int win_x,win_y;

		if(!pt || !window)return 1;
		XQueryPointer(xg.mydisplay,window,
		&root,&child,&press_x,&press_y,
		&win_x,&win_y,&mask);
		    pt->x=win_x;
		    pt->y=win_y;
		    return 0;
}
int MenuClick(struct Menu *menuList)
{
	if(!menuList)return 1;
	sprintf(WarningBuff,"MenuClick in %s\n",menuList->menuName->stringName);
	WarningBatch(WarningBuff);
	return 0;
}
struct Menu *MenuNew(char *name,uRect *menuRect,Window theWindow)
{
	struct stringData *menuName;
	struct Menu *MenuList2;

	if(!name || !menuRect || !theWindow )return (struct Menu *)NULL;
	menuName=StringNew(name,menuRect,StringLeft,theWindow);
	if(!menuName)return (struct Menu *)NULL;

	MenuList2=(struct Menu *)cMalloc(sizeof(struct Menu),8738);

	if(MenuList2 == NULL){
	    StringDispose(menuName);
	    return (struct Menu *)NULL;
	}
	
	zerol((char *)MenuList2,(long)sizeof(struct Menu));

	MenuList2->menuName=menuName;

	MenuList2->menuRect = *menuRect;

	return MenuList2;
	
}
int  MenuBarNext(char *name,uRect *menuRect,struct MenuBar *Data)
{
	int width,n;

	if(!name || !menuRect || !Data )return 1;

	width=XTextWidth(Data->info,name,(int)strlen(name));
	menuRect->y=0;
	menuRect->xsize=width+2*(Data->MenuBarSide+5);
	menuRect->ysize=Data->menuRect.ysize;
	menuRect->x=0;
	for(n=0;n<Data->menuCount;++n){
	    menuRect->x += MenuSize(Data->menuList[n]);
	}
	if(!Data->menuCount)menuRect->x += 5;
	return 0;
}
int  MenuAdd(char *name,int flag,struct Menu *menuList)
{
    struct MenuItems *Items;
	long menuCount,n;

	if(!menuList || !name)return 1;
	menuCount=menuList->menuCount+1;

	Items=(struct MenuItems*)cMalloc(menuCount*sizeof(struct MenuItems),8739);
	if(!Items)return 1;
	zerol((char *)Items,menuCount*sizeof(struct MenuItems));

	for(n=0;n<menuCount-1;++n){
	    if(menuList->Items){
	        Items[n]=menuList->Items[n];
	    }
	}
	Items[menuCount-1].menuNames = strsave(name,8832);
	Items[menuCount-1].menuFlags = flag;
	if(menuList->Items)cFree((char *)menuList->Items);
	menuList->Items=Items;
	menuList->menuCount=menuCount;

	return 0;
}
int MenuDispose(struct Menu *menuList)
{
	int n;

	if(!menuList)return 1;
	if(menuList->menuName)StringDispose(menuList->menuName);
	
	if(menuList->Items){
	    for(n=0;n<menuList->menuCount;++n){
	        if(menuList->Items[n].menuNames)cFree((char *)menuList->Items[n].menuNames);
	        if(menuList->Items[n].colors)cFree((char *)menuList->Items[n].colors);
	    }
	    cFree((char *)menuList->Items);
	}
	cFree((char *)menuList);
	return 0;
}
int  MenuBarAdd(struct Menu *menuListin,struct MenuBar *Data)
{
	struct Menu **menuList;
	long menuCount,n;

	if(!menuListin || !Data)return 1;
	menuCount=Data->menuCount+1;
	menuList=(struct Menu **)cMalloc(menuCount*sizeof(struct Menu *),8742);
	if(!menuList)return 1;
	for(n=0;n<menuCount-1;++n){
	    if(Data->menuList)menuList[n]=Data->menuList[n];
	}
	menuList[menuCount-1] = menuListin;
	if(Data->menuList)cFree((char *)Data->menuList);
	Data->menuList=menuList;
	Data->menuCount=menuCount;

	return 0;
}
int  MenuSize(struct Menu *menuList)
{
	if(!menuList)return 0;
	return menuList->menuRect.xsize;
}

int  MenuDisable(struct Menu *menu,int item)
{
	if(!menu)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->Items)return 1;
	menu->Items[item].disableFlag= TRUE;
	return 0;
}

int  MenuEnable(struct Menu *menu,int item)
{
	if(!menu)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->Items)return 1;
	menu->Items[item].disableFlag= FALSE;
	return 0;
}

int  MenuSetMark(int flag,int item,struct Menu *menu)
{
	if(!menu)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->Items)return 1;
	menu->Items[item].menuFlags= (flag & 0xff00) | (menu->Items[item].menuFlags & 0xff);
	return 0;
}
int  MenuSetPalette(char *pal,int item,struct Menu *menu)
{
	XColor *colors;
	int i;

	if(!menu || !pal)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->Items)return 1;
	colors=(XColor *)cMalloc(256L*sizeof(XColor),8743);
	if(!colors)return 1;

	for(i=0;i<256;++i){
	    colors[i].pixel=i;
	    colors[i].red=pal[i*3] << 8;
	    colors[i].green=pal[i*3+1] << 8;
	    colors[i].blue=pal[i*3+2] << 8;
	    colors[i].flags=DoRed | DoGreen | DoBlue;
	}
	if(menu->Items[item].colors)cFree((char *)menu->Items[item].colors);
	menu->Items[item].colors=colors;
	menu->Items[item].menuFlags= (menu->Items[item].menuFlags & 0xff00) | (MENU_PALETTE_NAMED & 0xff);
	return 0;
}
int  MenuSetTitle(char *name,struct Menu *menu)
{

	if(!menu || !name || !menu->menuName)return 1;
	mstrncpy(menu->menuName->stringName,name,255);
	menu->menuName->stringRect.xsize=menu->menuRect.xsize;
	return 0;
}
int  MenuSetName(char *name,int item,struct Menu *menu)
{

	if(!menu || !name)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(menu->Items){
	    if(menu->Items[item].menuNames)cFree((char *)menu->Items[item].menuNames);
	    menu->Items[item].menuNames=strsave(name,8833);
	}else{
	    return 1;
	}
	return 0;
}
long MenuCount(struct Menu *menu)
{
	if(!menu)return 0;
	return menu->menuCount;
}
int MenuGetName(int item,char *name,struct Menu *menu)
{
	if(!menu || !name)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->Items || !menu->Items[item].menuNames)return 1;
	mstrncpy(name,menu->Items[item].menuNames,255);
	return 0;
}

int MenuGetPalette(int item,XColor *colors,struct Menu *menu)
{
	if(!menu || !colors)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	if(!menu->Items || !menu->Items[item].colors)return 1;
	memcpy(colors,menu->Items[item].colors,256L*sizeof(XColor));
	return 0;
}
int MenuDelete(int item,struct Menu *menu)
{
	int n;

	if(!menu)return 1;
	if((item < 0) || (item >= menu->menuCount))return 1;
	
	if(menu->Items && menu->Items[item].menuNames){
	    cFree((char *)menu->Items[item].menuNames);
	    menu->Items[item].menuNames=NULL;
	}
	if(menu->Items && menu->Items[item].colors){
	    cFree((char *)menu->Items[item].colors);
	    menu->Items[item].colors=NULL;
	}
	for(n=item;n<menu->menuCount-1;++n){
	    if(menu->Items){
	        menu->Items[n]=menu->Items[n+1];
	    }
	}
	--menu->menuCount;
	return 0;

}

