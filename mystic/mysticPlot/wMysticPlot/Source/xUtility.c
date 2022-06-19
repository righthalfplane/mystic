#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define EXTERN extern

#include "Xappl.h"
#include "Paux.h"
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
/*int FillRect(uRect *b,Window mywindow,GC gc);*/

int SetOptionKey(int flag);

/*int getXcolors(char *pal,XColor *colors);*/

int getPalette(int n,char *name,char *pal);

int mstrcmp(char *out,char *in);
int GetTime(long *Seconds,long *milliseconds);


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
int IsShift()
{
	BYTE cBuf[256];

	GetKeyboardState(cBuf);

	return cBuf[VK_SHIFT] & 0x80;
}
int IsControl(void)
{
	BYTE cBuf[256];

	GetKeyboardState(cBuf);

	return cBuf[VK_CONTROL] & 0x80;
}
int IsOption(void)
{
	BYTE cBuf[256];

	GetKeyboardState(cBuf);

	return cBuf[VK_CONTROL] & 0x80;
}
double rtime(void)
{
	long Seconds, milliseconds;
	GetTime(&Seconds,&milliseconds);
	return (double)Seconds+((double)milliseconds)/1000;
}
unsigned long int TickCount(void)
{
	static unsigned long sSeconds;
	unsigned long Seconds, milliseconds;

	GetTime(&Seconds,&milliseconds);

	if(!sSeconds){
		sSeconds=Seconds;
	}
	
	return (Seconds-sSeconds)*60+milliseconds*60/1000;
}
int MoveTo(int x,int y)
{
	xold=x;
	yold=y;
	return 0;
}
