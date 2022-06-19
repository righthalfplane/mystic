#include <stdio.h>
#include <math.h>
#include <string.h>

#define EXTERN    extern
#include "xStrings.h"
#include "Tools.h"

struct stringData *StringNew(char *stringName,uRect *stringRect,int justification,HWND theWindow)
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
//	String->gc=XCreateGC(xg.mydisplay,theWindow,0,0);
//	XSetFont(xg.mydisplay,String->gc,xg.font13); 

//	if(getColors(String->c)){
//	    fprintf(stderr,"StringNew Error Getting Colors\n");
//	}

//	String->info=xg.font13info;
//	if(!String->info)goto ErrorOut;

	return String;	
//ErrorOut:
//	StringDispose(String);
//	return NULL;
}
struct stringData *StringNewMenu(char *stringName,uRect *stringRect,int justification,HWND theWindow)
{
	struct stringData *String;

 	if(!stringName || !stringRect || !theWindow)return (struct stringData *)NULL;
 		
	String=(struct stringData *)cMalloc(sizeof(struct stringData),8744);

	if(String == NULL)return (struct stringData *)NULL;
	
	zerol((char *)String,(long)sizeof(struct stringData));

	mstrncpy(String->stringName,stringName,255);
//	String->myMenu=theWindow;
	String->stringRect = *stringRect;
	String->justification=justification;
//	String->gc=XCreateGC(xg.mydisplay,theWindow,0,0);
//	XSetFont(xg.mydisplay,String->gc,xg.font13); 

//	if(getColors(String->c)){
//	    fprintf(stderr,"StringNew Error Getting Colors\n");
//	}

//	String->info=xg.font13info;
//	if(!String->info)goto ErrorOut;

	return String;	
//ErrorOut:
//	StringDispose(String);
//	return NULL;
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
	return 0;
}
int StringColors(unsigned long *c,struct stringData *String)
{
	int n;
	if(!c || !String)return 1;

	for(n=0;n<8;++n)String->c[n]=c[n];

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
	return 0;
}
int StringDispose(struct stringData *String)
{
	return 0;
}
