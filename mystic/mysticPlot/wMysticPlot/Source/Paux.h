#ifndef _PAUX_
#define _PAUX_

#define EXTERN22 extern


#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>
#include "Resource.h"
#include "ulibTypes.h"


void *cMalloc(unsigned long r,int add);
void *cRealloc(char *p,unsigned long r,int add);
int cFree(char *ptr);
int SetBuffers(long Length);
int Warning(char *message);
char *strsave(char *s,int add);
int zerol(char *p,unsigned long length);

int MoveToM(int x,int y);
int LineToM(int x,int y,HWND mywindow,HDC hdc);
int DrawString(char *buff,HWND mywindow,HDC hdc,int len);

int gettimeofday(struct timeval *curTime,struct timezone *v);
int SetTime(struct timeval *curTime,long milliseconds);
int TimeUp(struct timeval *LastTime);
int IsShift(void);



char WarningBuff[256];


#endif