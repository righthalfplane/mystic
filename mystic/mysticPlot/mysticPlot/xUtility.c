#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define EXTERN22 extern
#include "Xappl.h"

#include "xAux.h"
#include "Tools.h"

static int isShiftKey;
static int isControlKey;
static int isOptionKey;
static int xold;
static int yold;

int SetShiftKey(int flag);
int SetControlKey(int flag);

int WindowToFilename(char *Name,char *WindowName);

double rtime(void);

int IsOption(void);

int IsControl(void);
int FillRect(uRect *b,Window mywindow,GC gc);

int SetOptionKey(int flag);

int getXcolors(char *pal,XColor *colors);

int getPalette(int n,char *name,char *pal);

int mstrcmp(char *out,char *in);

int WindowToFilename(char *Name,char *WindowName)
{
	char Directory[1024];
	char *bp,*ds;

	if(!Name || !WindowName)return 1;

	mstrncpy(Directory,WindowName,1023);
	ds=strrchr(Directory,':');
	if(ds){
	   ++ds;
	}else{
	   ds=Directory;
	}

	bp=strrchr(ds,'/');
	if(bp){
	    mstrncpy(Name,bp+1,255);
	}else if(ds[0]){
	    mstrncpy(Name,ds,255);
	}else{
	    Name[0]=0;
	}

	bp=strrchr(Name,'<');
	if(bp)*bp = 0;

	return 0;
}
int CheckControlAndShiftKeys(void)
{
	{
	    unsigned char bits[] = {1,2,4,8,16,32,64,128};
	    unsigned char d[32];
	    int n,nc,k;

	    SetShiftKey(FALSE);
	    SetControlKey(FALSE);
	    SetOptionKey(FALSE);
	    XQueryKeymap(xg.mydisplay,(char *)d);
	    nc=0;
	    for(n=0;n<32;++n){
	        for(k=0;k<8;++k){
	            unsigned long keysym;
	            if(bits[k] & d[n]){
	                keysym=XKeycodeToKeysym(xg.mydisplay,nc,0);
					if(keysym == XK_Shift_L){
						SetShiftKey(TRUE);
					}else if(keysym == XK_Meta_L || keysym == XK_Meta_R){
						/* printf("keysym = %lx XK_Meta_L or XK_Meta_R\n",(long)keysym); */
					}else if(keysym == XK_Control_L){
						SetControlKey(TRUE);
					}else if(keysym == XK_Alt_L){
						SetOptionKey(TRUE);
					}
	            }
	            nc++;
	        }
	    }
	}
	return 0;
}
int SetShiftKey(int flag)
{
	isShiftKey=flag;
	return 0;
}
int SetControlKey(int flag)
{
	isControlKey=flag;
	return 0;
}
int SetOptionKey(int flag)
{
	isOptionKey=flag;
	return 0;
}
int IsShift(void)
{
	return isShiftKey;
}
int IsControl(void)
{
	return isControlKey;
}
int IsOption(void)
{
	return isOptionKey;
}
int OffsetRect(uRect *LineRect,int x,int y)
{
	if(!LineRect)return 1;
	LineRect->x += x;
	LineRect->y += y;
	return 0;
}
double rtime(void)
{
	long milliseconds;
	long Seconds;
	double ret;


	GetTime(&Seconds,&milliseconds);

	ret=(double)Seconds+(double)milliseconds/1000.;

	return ret;

}
unsigned long int TickCount(void)
{
	long Seconds,milliseconds;
	unsigned long int t,Sec;

	GetTime(&Seconds,&milliseconds);
	Sec=Seconds;
	Sec &= 0xFFFFFF;
	t=(unsigned long int)((double)Sec*60+(60.0*(double)milliseconds)/1000.);
	return t;
}
int EraseRect(uRect *Box,Window mywindow,GC gc)
{
/*
	XClearArea(xg.mydisplay,mywindow,Box->x,Box->y,
	Box->xsize,Box->ysize,FALSE);
*/
	XSetForeground (xg.mydisplay, gc, xg.c[2]);
	XSetBackground (xg.mydisplay, gc, xg.c[2]);
	XFillRectangle(xg.mydisplay,mywindow,
	               gc,
	               Box->x,Box->y,
	               Box->xsize,
	               Box->ysize);
	return 0;
}
int FillRect(uRect *b,Window mywindow,GC gc)
{
	XFillRectangle(xg.mydisplay,mywindow,gc,b->x,b->y,
				   b->xsize,b->ysize);
	return 0;
}
int FrameRect(uRect *Box,Window mywindow,GC gc)
{
	MoveTo(Box->x,Box->y);
	LineTo(Box->x,Box->y+Box->ysize-1,mywindow,gc);
	LineTo(Box->x+Box->xsize-1,Box->y+Box->ysize-1,mywindow,gc);
	LineTo(Box->x+Box->xsize-1,Box->y,mywindow,gc);
	LineTo(Box->x,Box->y,mywindow,gc);
	return 0;
}
int MoveTo(int x,int y)
{
	xold=x;
	yold=y;
	return 0;
}
int LineTo(int x,int y,Window mywindow,GC gc)
{
	XDrawLine(xg.mydisplay,mywindow,gc,xold,yold,
			  x,y);
        xold=x;
        yold=y;
	return 0;
}
int DrawString(unsigned char *name,Window mywindow,GC gc)
{
	XDrawImageString(xg.mydisplay,mywindow,gc,
	        xold,yold,(char *)name,(int)strlen((char *)name));
	return 0;
}
int PtInRect(uPoint *event,uRect *controlRect)
{
	if(event->x < controlRect->x || event->y < controlRect->y)return 0;
	if(event->x > controlRect->x+controlRect->xsize ||
	   event->y > controlRect->y+controlRect->ysize)return 0;
	return 1;
}
int getXcolors(char *pal,XColor *colors)
{
	int i;

	if(!pal || !colors)return 1;
	
	for(i=0;i<256;++i){
	    colors[i].pixel=i;
	    colors[i].red=pal[i*3] << 8;
	    colors[i].green=pal[i*3+1] << 8;
	    colors[i].blue=pal[i*3+2] << 8;
	    colors[i].flags=DoRed | DoGreen | DoBlue;
	}
	
	colors[0].red=255L << 8;
	colors[0].green=255L << 8;
	colors[0].blue=255L << 8;

	colors[255].red=0;
	colors[255].green=0;
	colors[255].blue=0;
	return 0;
}
