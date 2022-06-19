#define EXTERN extern
#include <stdio.h>
#include <memory.h> 
#include <stdlib.h> 
#include <math.h> 
#include "Xappl.h"
#include "uFiles.h"
#include "Net.h"

/* CHANGE X-WINDOW TYPES TO SOMETHING COMPATIBLE WITH WINDOWS FOR NOW */
#define Visual void
#define XColor void
#define GC int
#define XImage void

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#if defined(__cplusplus) || defined(c_plusplus)
#define class c_class
#endif

#ifndef FIONREAD
#include <sys/filio.h>
#endif

int GetTime(long *Seconds,long *milliseconds);

int SetTime(struct timeval *curTime,long milliseconds);

int TimeUp(struct timeval *LastTime);

int SetWindowName(IconPtr myIcon,char *name);

int CheckSocket(struct Network *net,int *count);

int Sleep2(int ms);

struct timeval BellTime;

void Bell(int tag);

int SearchVisualList(int image_depth,int  visual_class, Visual **visual);

int WriteHDFToScreen(struct DisplayF *d,unsigned char *data);

int WriteToScreen(struct SetFrameData *s,struct DisplayF *d,unsigned char *data,int type);

int Write24ToScreen(unsigned char *bin,unsigned char *bout,int xsize,int ysize,XColor *colors);

int do24to8(unsigned char *pal,unsigned char *pin,unsigned char *pout,long xsize,long ysize);

/*int getXcolors(char *pal,XColor *colors);*/


int pclose(FILE *in);

FILE *getPIPE(void);

FILE *getPIPE(void)
{
	return NULL;
}

int pclose(FILE *in)
{
	if(!in)return 1;
	return 0;
}
void Bell(int tag)
{
	tag=tag;
	MessageBeep(0xFFFFFFFF);
}

int GetTime(long *Seconds,long *milliseconds)
{
	struct timeval curTime;

	gettimeofday(&curTime,(struct timezone *) NULL);
	*Seconds=curTime.tv_sec;
	*milliseconds=curTime.tv_usec/1000;
	return 0;
}
int SetTime(struct timeval *curTime,long milliseconds)
{
	gettimeofday(curTime,(struct timezone *) NULL);
	curTime->tv_sec += milliseconds/1000;
	curTime->tv_usec += (milliseconds%1000)*1000;
	if (curTime->tv_usec > 1000000) {
            curTime->tv_usec -= 1000000;
            curTime->tv_sec += 1;
	}
    return 0;
}
int TimeUp(struct timeval *LastTime)
{
  struct timeval curTime;
  
  (void) gettimeofday(&curTime, (struct timezone *) NULL);
  if ((LastTime->tv_sec < curTime.tv_sec)
    || ((LastTime->tv_sec == curTime.tv_sec)
    &&  (LastTime->tv_usec <= curTime.tv_usec))) {
    return 1;
  }
  return 0;
}

int Sleep2(int ms)
{
	Sleep(ms);

	return 0;
}

int SetWindowName(IconPtr myIcon,char *name)
{
	if (!myIcon) return 1;
	if(myIcon->WindowNameSet)return 1;
	mstrncpy(myIcon->WindowName,name,256);
	SetWindowText(myIcon->myWindow,name);
	return 0;
}
int SearchVisualList(int image_depth,int  visual_class, Visual **visual)
{
    return 1;
}
int WriteToScreen(struct SetFrameData *s,struct DisplayF *d,unsigned char *data,int type)
{
	return 1;
}
int Write24ToScreen(unsigned char *bin,unsigned char *bout,int xsize,int ysize,XColor *colors)
{
	return 0;
	
}
int WriteHDFToScreen(struct DisplayF *d,unsigned char *data)
{
	return 0;
}

struct fontData{
    int on;
    int xsize;
    int ysize;
    int first;
    int last;
    int xinc;
    XImage *xi;
};

struct fontData *fd;
static int FontCount;
static int xBuff,yBuff;
