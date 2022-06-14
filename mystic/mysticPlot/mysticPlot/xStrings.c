#include <stdio.h>
#include <math.h>
#include <string.h>

#define EXTERN22 extern
#include "Xappl.h"
#include "xStrings.h"
#include "Tools.h"

struct stringData *StringNew(char *stringName,uRect *stringRect,int justification,Window theWindow)
{
	struct stringData *String;

 	if(!stringName || !stringRect || !theWindow)return (struct stringData *)NULL;
 		
	String=(struct stringData *)cMalloc(sizeof(struct stringData),8744);

	if(String == NULL)return (struct stringData *)NULL;
	
	zerol((char *)String,(long)sizeof(struct stringData));

	mstrncpy(String->stringName,stringName,255);
	String->myWindow=theWindow;
	String->stringRect = *stringRect;
	String->justification=justification;
	String->gc=XCreateGC(xg.mydisplay,theWindow,0,0);
	XSetFont(xg.mydisplay,String->gc,xg.font13); 

	if(getColors(String->c)){
	    fprintf(stderr,"StringNew Error Getting Colors\n");
	}

	String->info=xg.font13info;
	if(!String->info)goto ErrorOut;

	return String;	
ErrorOut:
	StringDispose(String);
	return NULL;
}
int StringMove(struct stringData *String,int ix,int iy)
{
	if(!String)return 1;
	String->stringRect.x=ix;
	String->stringRect.y=iy;
	
	return 0;
}
int StringFont(struct stringData *String,int num)
{
	XFontStruct *info;
	Font font;

	if(!String)return 1;
	switch(num){
	case 12:
		font=xg.font12;
		info=xg.font12info;
		break;
	case 13:
		font=xg.font13;
		info=xg.font13info;
		break;
	case 14:
		font=xg.font14;
		info=xg.font14info;
		break;
	case 16:
		font=xg.font16;
		info=xg.font16info;
		break;
	case 24:
		font=xg.font24;
		info=xg.font24info;
		break;
	default:
		font=xg.font12;
		info=xg.font12info;
		break;
	}
	XSetFont(xg.mydisplay,String->gc,font); 
	String->info=info;
	return 0;
}
int StringColors(unsigned long *c,struct stringData *String)
{
	int n;
	if(!c || !String)return 1;

	for(n=0;n<10;++n)String->c[n]=c[n];

	return 0;
}
int StringGetRect(uRect *stringRect,struct stringData *String)
{
	if(!stringRect || !String)return 1;
	*stringRect=String->stringRect;
	return 0;
}
int StringUpdate(struct stringData *String)
{
	int linestart,width;
	uRect stringRect;
	XRectangle r;

	if(!String)return 1;

	width=XTextWidth(String->info,String->stringName,(int)strlen(String->stringName));

	if(String->justification == StringLeft){
	    linestart=String->stringRect.x+String->stringSide+2;
	}else if(String->justification == StringRight){
	    linestart=String->stringRect.x+String->stringRect.xsize
	             -String->stringSide-2-width;
	}else{
	    linestart=String->stringRect.x+String->stringRect.xsize/2-width/2;
	}

	stringRect=String->stringRect;
	r.x=stringRect.x+String->stringSide;
	r.y=stringRect.y+String->stringSide;
	r.width=stringRect.xsize-2*String->stringSide;
	r.height=stringRect.ysize-2*String->stringSide;
	XSetClipRectangles(xg.mydisplay,String->gc,0,0,&r,1,0);

	XSetForeground (xg.mydisplay,String->gc,String->c[4]);

	XSetBackground (xg.mydisplay,String->gc,String->c[5]);
	MoveTo(linestart,stringRect.y+stringRect.ysize/2+String->info->ascent/2);
	DrawString((unsigned char *)String->stringName,String->myWindow,String->gc);
	return 0;
}
int StringDispose(struct stringData *String)
{
	if(!String)return 1;
	if(String->gc)XFreeGC(xg.mydisplay,String->gc);
	String->gc=NULL;
	cFree((char *)String);
	return 0;
}
