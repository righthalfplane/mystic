#define EXTERN22 extern
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#ifdef MAC_RESOURCES
#include <sys/attr.h>
#endif

#include <stdarg.h>
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "fileDialog.h"
#include "uFiles.h"
#include "SaveAllImages.h"
#include "BatchFile.h"
#include "Universal.h"

#include "Linedata.h"

#include "BatchFile.h"
#include "Sagevol.h"
#include "uGraph.h"
#include "uAttributes.h"
#include "GWorld.h"
#include "uLib.h"
#include "mThread.h"

int BatchFile(void);

int FontStart(GC gc);

#define ySHIFT	25

#define PaletteStart	8

int uFontIsScalable(IconPtr myIcon);

int uSetTextFontAndSize(struct Icon *myIcon,int font,int size);

XFontStruct *LoadQueryScalableFont(Display *dpy, int screen, char *name, int size);

int isKeyBoardCommand(struct Icon *myIcon,int commandKey);

int GetTrueFromGWorld(long *xsize,long *ysize,unsigned char **image,struct sWorld *World);

int Warning2(char *message,int flag);

int uCStringWidth(char *name,IconPtr myIcon);

int doAddPalette(IconPtr myIcon,char *name);

int WriteToWindowFast(char *name,char *buff);

int WriteToWindowAtEnd(char *name,char *buff);

int doPaletteToTable(IconPtr myIcon);

int PutChildWindow(IconPtr myIcon,Window newChild);

Window getTopWindow(Window mywindow,int *xoff,int *yoff);

int doSavePalette(IconPtr myIcon);

int doGetPalette(IconPtr myIcon);

IconPtr  NewTableWindow(IconPtr myIcon);

IconPtr  macUtilities(IconPtr myIconIn);

IconPtr  NewNetWindow(IconPtr myIcon);

IconPtr NewFileSendWindow(IconPtr myIconi);

int SynchronizePalettes(IconPtr myIcon,unsigned char *red,unsigned char *green,unsigned char *blue);

int Write24ToScreen(unsigned char *bin,unsigned char *bout,int xsize,int ysize,XColor *colors);

int SetExpose2(IconPtr myIcon);

int SetWindowName(IconPtr myIcon,char *name);

int getPalColors(XColor *colors,unsigned char *pal);

int WriteHDFToScreen(struct DisplayF *d,unsigned char *data);

struct Menu *uMenuBarAddMenus(char *MenuName,
                             struct uMenuItems *Items,
                             struct MenuBar *MenuBarList);
                             
static int doExpose(IconPtr myIcon,XEvent *myevent);

int getPalette(int n,char *name,char *pal);

static long doInPageUp(controlPtr control,short part,void *controlRoutineData);
static long doInPageDown(controlPtr control,short part,void *controlRoutineData);
static long doInUpButton(controlPtr control,short part,void *controlRoutineData);
static long doInDownButton(controlPtr control,short part,void *controlRoutineData);
static long doInThumb(controlPtr control,short part,void *controlRoutineData);

static char *scrap;

IconPtr NewEdit(struct FileList *ret);

IconPtr openRender(struct Scene *scene);

int CreateNewDirectory(void);

int domkdir(char *name);

int LoadToMenu(IconPtr myIcon);

static int xold,yold;


int WriteRect24ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                        unsigned char *out,uRect *DRect,long oxsize,long oysize);


int WriteRectToRect(unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize);

int doEraseDown(IconPtr myIcon);


IconPtr  ListWindow(int XCount,int YCount,int XLength,int YLength);

int uStillDown(IconPtr myIcon);

int NotMyWindow(IconPtr myIcon,XEvent *event);

int  WarningBatch(char *WarningBuff);

int getLong(long *n,FILE *in);
int getInt(int *i,FILE *in);

int UnCompress(unsigned int Esc1,unsigned int Esc2,long UnCompressed,long Compressed,unsigned char *fData,unsigned char *oData);

int uMemcpy(void *out1,void *in1,long length);

int domkdir(char *nb);

int IsOption(void);

int doFiddleColors(IconPtr myIcon);

int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);

int LineB(int x,int y,int nc,struct screenData *s);
int kpoint(int x,int y,int nc,struct screenData *s);
int MoveB(int x,int y);
int FindB(int *x,int *y);
int sear(double cmax,double cmin,double *zcon,int *ncon,int nmax);
int FontToScreen(uRect *cRect,unsigned char *image,int nc,struct screenData *s);
int FontToRect(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon);

int SetFileTypeOwner(char *newpath,char *BinaryOwner,char *BinaryType);

int doSystem(void);
int FilesGetPICT(char *name,unsigned char *buff,long length,IconPtr myIcon,struct FileInfo2 **Files);


char *strpathname(char *path,char *name);

int isDirectory(char *name);

int WriteStatus(IconPtr myIcon,char *buff);

int putString(unsigned char *data,long length,FILE *out);
int getString(unsigned char *data,long length,FILE *out);

int putInt(long n,FILE *out);
int putLong(long n,FILE *out);

int Synchronize(IconPtr myIconin,long CurrentFrame);
int getPalColors(XColor *colors,unsigned char *pal);

int  nWriteBox(IconPtr myIcon,char *buff);

int pDoStyleMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);
int pDoFontMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);

int getColorFromRGB(unsigned long *c,int r,int g,int b);

struct InstalledList{
     int item;
     int isActive;
};

int StringWidth(unsigned char *,struct Icon *myIcon);

int uOpenFile(void);
IconPtr NewCDraw(IconPtr myIcon);

struct FileList *uOpenFileDialogLocal(struct Icon *myIcon,int *TypeList);

IconPtr NewDrawWindow(IconPtr myIcon);

static int sScreenWrite(long xsize,long ysize,unsigned char *data,unsigned char *buff,int DataType,
                uRect *SRect,uRect *DRect,struct ScreenInfo *s);

static int sScreenSetSize(long xsize,long ysize,struct ScreenInfo *s);

int SetDefaultPaletteName(IconPtr myIcon,char *name1);
int launchThread(void *data,int (*sageThread)(void *data))
{
	pthread_t thread;
	int ret;

	if(!data || !sageThread)return 1;

	zerol((char *)&thread,sizeof(pthread_t));

	ret=pthread_create(&thread, NULL,(void *(*)(void *))sageThread,(void *)data);
	if(ret != 0){
	    sprintf(WarningBuff,"launchThread pthread_create ret %d\n",ret);
		WarningBatch(WarningBuff);
	    goto End;
	}
	pthread_detach(thread);
	return 0;
End:
	return 1;
}

int runThreads3(long ThreadCount,void *data,long numsteps,long ZonesPerBlock,
                int (*sageThread)(mThread *Threads),
			    int (*sageThread2)(mThread *Threads,int count))
{
	pthread_t *thread;
	mThread Threads;
	mThread *myIconTH;
	
	int nt,threads;
	long tstart,tend;
	int done,ret;
	
	if(!sageThread)return 1;
	
	
	if(ZonesPerBlock <= 0)
	{
		return runThreads2(ThreadCount,data,numsteps,sageThread,sageThread2);
	}
	
	threads=ThreadCount;
	
	myIconTH=NULL;
	thread=NULL;
	if(threads <= 0){
	    zerol((char *)&Threads,sizeof(Threads));
	    Threads.smin=0;
	    Threads.smax=numsteps;
		Threads.data=data;
	    (*sageThread)(&Threads);
	    return (*sageThread2)(&Threads,1);		
		
	}else{
		
	    myIconTH=(mThread *)cMalloc((threads+1)*sizeof(Threads),9020);
	    if(!myIconTH){
			sprintf(WarningBuff,"runThreads3 Out of memory Requested (%ld)\n",(long)(threads*sizeof(Threads)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)myIconTH,(threads+1)*sizeof(Threads));
		
		
	    thread=(pthread_t *)cMalloc(threads*sizeof(pthread_t),9021);
	    if(!thread){
	        sprintf(WarningBuff,"runThreads3 Out of memory Requested (%ld)\n",(long)(threads*sizeof(pthread_t)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)thread,threads*sizeof(pthread_t));
		
		for(nt=0;nt<threads;++nt){
			myIconTH[nt].data= data;
			myIconTH[nt].done = -1;
			myIconTH[nt].nthread=nt;
		}
		
				
	    tstart=0;
		do{
			tend=tstart+ZonesPerBlock;
			if(tend > numsteps){
				tend=numsteps;
			}
		
			while(1){
				done=FALSE;
				for(nt=0;nt<threads;++nt){
					if(!myIconTH[nt].done)continue;
					if(myIconTH[nt].done == 1)
					{
						if(sageThread2)(*sageThread2)(&myIconTH[nt],1);
					}
					myIconTH[nt].done = FALSE;
					myIconTH[nt].smin=tstart;
					myIconTH[nt].smax=tend;
					ret=pthread_create(&thread[nt], NULL,(void *(*)(void *))sageThread,(void *)&myIconTH[nt]);
					if(ret != 0){
						sprintf(WarningBuff,"runThreads3 pthread_create ret %d\n",ret);
						WarningBatch(WarningBuff);
						goto End;
					}
					pthread_detach(thread[nt]);
	                done=TRUE;
					break;
				}
	            if(done)break;
	            Sleep2(5);
			}
			tstart=tend;
		}while(tend < numsteps);
		
		
	    while(1){
	        done=TRUE;
	        for(nt=0;nt<threads;++nt){
	            if(!myIconTH[nt].done)done=FALSE;
	        }
	        if(done)break;
	        Sleep2(5);
	    }
		
		for(nt=0;nt<threads;++nt){
			if(myIconTH[nt].done == 1)
			{
				if(sageThread2)(*sageThread2)(&myIconTH[nt],1);
			}
		}
		
	}
End:
	if(myIconTH)cFree((char *)myIconTH);
	if(thread)cFree((char *)thread);
	return 0;
}



int runThreads2(long ThreadCount,void *data,long numsteps,int (*sageThread)(mThread *Threads),
			    int (*sageThread2)(mThread *Threads,int count))
{
	pthread_t *thread;
	mThread Threads;
	mThread *myIconTH;
	
	int nt,threads;
	long tstep,tstart,tend;
	int done,ret;
	
	if(!sageThread)return 1;
	
	threads=ThreadCount;
	
	myIconTH=NULL;
	thread=NULL;
	if(threads <= 0){
	    zerol((char *)&Threads,sizeof(Threads));
	    Threads.smin=0;
	    Threads.smax=numsteps;
		Threads.data=data;
	    (*sageThread)(&Threads);
	    return (*sageThread2)(&Threads,1);		
		
	}else{
		
	    myIconTH=(mThread *)cMalloc((threads+1)*sizeof(Threads),9020);
	    if(!myIconTH){
			sprintf(WarningBuff,"runThreads2 Out of memory Requested (%ld)\n",(long)(threads*sizeof(Threads)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)myIconTH,(threads+1)*sizeof(Threads));
		
		
	    thread=(pthread_t *)cMalloc(threads*sizeof(pthread_t),9021);
	    if(!thread){
	        sprintf(WarningBuff,"runThreads2 Out of memory Requested (%ld)\n",(long)(threads*sizeof(pthread_t)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)thread,threads*sizeof(pthread_t));
		
	    tstep=numsteps/(threads+1);
	    if(tstep <= 0){
	        tstep = 1;
			threads = (int)numsteps-1;
	    }
		
	    tstart=0;
	    for(nt=0;nt<threads;++nt){
	        myIconTH[nt].data= data;
			myIconTH[nt].done = FALSE;
			myIconTH[nt].smin=tstart;
			myIconTH[nt].nthread=nt;
			tend=tstart+tstep;
			myIconTH[nt].smax=tend;
			
			ret=pthread_create(&thread[nt], NULL,(void *(*)(void *))sageThread,(void *)&myIconTH[nt]);
			if(ret != 0){
			    sprintf(WarningBuff,"runThreads2 pthread_create ret %d\n",ret);
				WarningBatch(WarningBuff);
			    goto End;
			}
		    pthread_detach(thread[nt]);
			tstart += tstep;
			if(tstart >= numsteps){
			    tstart=numsteps;
			}
	    }
		
	    if(tstart < numsteps){
			zerol((char *)&Threads,sizeof(Threads));
			Threads.smin=tstart;
			Threads.smax=numsteps;
	        Threads.nthread=threads;
	        Threads.data=data;
			(*sageThread)(&Threads);
	    }
	    
		
	    while(1){
	        done=TRUE;
	        for(nt=0;nt<threads;++nt){
	            if(!myIconTH[nt].done)done=FALSE;
	        }
	        if(done)break;
	        Sleep2(5);
	    }
		
	    if(tstart < numsteps){
			myIconTH[threads++]=Threads;
	    }
		
		(*sageThread2)(myIconTH,threads);
	}
End:
	if(myIconTH)cFree((char *)myIconTH);
	if(thread)cFree((char *)thread);
	return 0;
}

int runThreads(long ThreadCount,void *data,long numsteps,int (*sageThread)(mThread *Threads))
{
	pthread_t *thread;
	mThread Threads;
	mThread *myIconTH;

	int nt,threads;
	long tstep,tstart,tend;
	int done,ret;

	if(!sageThread)return 1;

	threads=ThreadCount;

	myIconTH=NULL;
	thread=NULL;
	if(threads <= 0){
	    zerol((char *)&Threads,sizeof(Threads));
	    Threads.smin=0;
	    Threads.smax=numsteps;
		Threads.data=data;
	    return (*sageThread)(&Threads);
	}else{
	
	    myIconTH=(mThread *)cMalloc(threads*sizeof(Threads),9020);
	    if(!myIconTH){
			sprintf(WarningBuff,"runThreads Out of memory Requested (%ld)\n",(long)(threads*sizeof(Threads)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)myIconTH,threads*sizeof(Threads));


	    thread=(pthread_t *)cMalloc(threads*sizeof(pthread_t),9021);
	    if(!thread){
	        sprintf(WarningBuff,"runThreads Out of memory Requested (%ld)\n",(long)(threads*sizeof(pthread_t)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)thread,threads*sizeof(pthread_t));

	    tstep=numsteps/(threads+1);
	    if(tstep <= 0){
	        tstep = 1;
			threads = (int)numsteps-1;
	    }

	    tstart=0;
	    for(nt=0;nt<threads;++nt){
	        myIconTH[nt].data= data;
			myIconTH[nt].done = FALSE;
			myIconTH[nt].smin=tstart;
			myIconTH[nt].nthread=nt;
			tend=tstart+tstep;
			myIconTH[nt].smax=tend;
						
			ret=pthread_create(&thread[nt], NULL,(void *(*)(void *))sageThread,(void *)&myIconTH[nt]);
			if(ret != 0){
			    sprintf(WarningBuff,"runThreads pthread_create ret %d\n",ret);
				WarningBatch(WarningBuff);
			    goto End;
			}
		    pthread_detach(thread[nt]);
			tstart += tstep;
			if(tstart >= numsteps){
			    tstart=numsteps;
			}
	    }

	    if(tstart < numsteps){
			zerol((char *)&Threads,sizeof(Threads));
			Threads.smin=tstart;
			Threads.smax=numsteps;
	        Threads.nthread=threads;
	        Threads.data=data;
			(*sageThread)(&Threads);
	    }
	    

	    while(1){
	        done=TRUE;
	        for(nt=0;nt<threads;++nt){
	            if(!myIconTH[nt].done)done=FALSE;
	        }
	        if(done)break;
	        Sleep2(5);
	    }

	}
End:
	if(myIconTH)cFree((char *)myIconTH);
	if(thread)cFree((char *)thread);
	return 0;
}

int pDoFontMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int font)
{
	if(!myIcon)return 1;
	//FontMenuNumber=FontMenuNumber;
	//StyleMenuNumber=StyleMenuNumber;

	uSetTextFont(myIcon,font);
	return 0;
}


int pDoStyleMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item)
{
	int fontsize[]={9,10,12,14,24,36,42,48,54,72};
    int size;
	if(!myIcon)return 1;
	//FontMenuNumber=FontMenuNumber;
	//StyleMenuNumber=StyleMenuNumber;
	if(Item >= 8 && Item <= 17){
	    size=fontsize[Item-8];
		if(size == myIcon->pSysFontList.lastSize)return 0;
		myIcon->pSysFontList.lastSize=size;
	}
	return 0;
}
int uGetFontStuff(struct uFontStuff *f,IconPtr myIcon)
{
	
	if(!f || !myIcon)return 1;
	
	f->txFont=myIcon->pSysFontList.lastFont;
	f->txSize=myIcon->pSysFontList.lastSize;
	f->txFace=myIcon->pSysFontList.lastFace;
	
	
	return 0;
}
int uSetFontStuff(struct uFontStuff *f,IconPtr myIcon)
{
	if(!f || !myIcon)return 1;
	
	uSetTextFont(myIcon,f->txFont);
	
	return 0;
}
int uSetFontMenus(int FontMenuNumber,int StyleMenuNumber,IconPtr myIcon)
{
	int fontOn;
	
	if(!myIcon)return 1;
	//FontMenuNumber=FontMenuNumber;
	//StyleMenuNumber=StyleMenuNumber;
	uActivateMenuItem(myIcon,StyleMenuNumber,1,FALSE);
	uActivateMenuItem(myIcon,StyleMenuNumber,2,FALSE);
	uActivateMenuItem(myIcon,StyleMenuNumber,3,FALSE);
	uActivateMenuItem(myIcon,StyleMenuNumber,4,FALSE);
	uActivateMenuItem(myIcon,StyleMenuNumber,5,FALSE);
	uActivateMenuItem(myIcon,StyleMenuNumber,6,FALSE);
	
	fontOn=uFontIsScalable(myIcon);
	
	uActivateMenuItem(myIcon,StyleMenuNumber,8,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,9,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,10,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,11,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,12,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,13,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,14,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,15,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,16,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,17,fontOn);
	uActivateMenuItem(myIcon,StyleMenuNumber,18,fontOn);
	return 0;
}
int uGetFontInfo(struct Icon *myIcon,uFontInfo *t)
{
    XFontStruct *f;
    XCharStruct *c;
	    
	if(!myIcon || !t || !xg.mydisplay)return 1;
	
    f=XQueryFont(xg.mydisplay,XGContextFromGC(myIcon->gc));
    if(!f)return 1;
    c=&(f->max_bounds);
    t->CWidth = c->width;	
    t->LineHeight=f->ascent+f->descent;
    t->Descent=f->descent;	    
	XFreeFontInfo(NULL, f, 1);
	
	return 0;
}
int uFontIsScalable(IconPtr myIcon)
{
	char *name,*pc;
	/*
	int fontOn;
	*/
	int font;
	int n;
	
	if(!myIcon)return 0;
	/*
	fontOn=FALSE;
	*/
	font=myIcon->pSysFontList.lastFont;
	if(font > 0 && font <= myIcon->pSysFontList.count){
	    --font;
		if(myIcon->pSysFontList.name && myIcon->pSysFontList.name[font]){
		    name=myIcon->pSysFontList.name[font];
			for(n=0;n<7;++n){
				pc=strchr(name,'-');
				if(pc){
					name=pc+1;
				}else{
				    name=NULL;
				    break;
				}
			}
			
			if(name && *name == '0'){
				;
			}else{
			    return FALSE;
			}
			
		    name=myIcon->pSysFontList.name[font];
			for(n=0;n<12;++n){
				pc=strchr(name,'-');
				if(pc){
					name=pc+1;
				}else{
				    name=NULL;
				    break;
				}
			}
			
			if(name && *name == '0'){
			/*
				fontOn=TRUE;
			*/
			}
		}
	}
	/* return fontOn; */
	return FALSE;
}

int uSetFontSize(struct Icon *myIcon,int size)
{
	if(!myIcon || (myIcon->WindowType != DrawNewType))return 1;
	return uSetTextFontAndSize(myIcon,myIcon->pSysFontList.lastFont,size);
}

	
int uSetTextFontAndSize(struct Icon *myIcon,int font,int size)
{

	XFontStruct *fontSet;

	if(!myIcon || (myIcon->WindowType != DrawNewType))return 1;
	
	if((font == myIcon->pSysFontList.lastFont) && (size == myIcon->pSysFontList.lastSize))return 0;
	myIcon->pSysFontList.lastSize=size;
	
	if((font == myIcon->pSysFontList.lastFont) && !uFontIsScalable(myIcon))return 0;
	
	myIcon->pSysFontList.lastFont=font;
	
	fontSet=NULL;
	
	if(font > 0 && font <= myIcon->pSysFontList.count){
	    --font;
		if(myIcon->fontLast){
		    XFreeFont(xg.mydisplay,myIcon->fontLast);
		    myIcon->fontLast=NULL;
		}
		if(myIcon->pSysFontList.name && myIcon->pSysFontList.name[font]){
			if(uFontIsScalable(myIcon)){
		        fontSet=LoadQueryScalableFont(xg.mydisplay,xg.myscreen,myIcon->pSysFontList.name[font],25*myIcon->pSysFontList.lastSize);
			}else{
			    fontSet = XLoadQueryFont(xg.mydisplay,myIcon->pSysFontList.name[font]);
			}
		    myIcon->fontLast=fontSet;
		}
	}

	if(!fontSet)fontSet=xg.font13info;
	XSetFont(xg.mydisplay,myIcon->gc,fontSet->fid); 
	
	if(myIcon->info)XFreeFontInfo(NULL,myIcon->info,1);
    myIcon->info=XQueryFont(xg.mydisplay,XGContextFromGC(myIcon->gc));
	    
	return 0;
}
int uSetTextFont(struct Icon *myIcon,int font)
{

	if(!myIcon || (myIcon->WindowType != DrawNewType))return 1;	
	
	return uSetTextFontAndSize(myIcon,font,myIcon->pSysFontList.lastSize);
	
}
int uSetTextFace(struct Icon *myIcon,int size)
{
	if(!myIcon || (myIcon->WindowType != DrawNewType))return 1;
	//size=size;
    /* XSetFont(xg.mydisplay,myIcon->gc,xg.font16); */ 
	return 0;
}
int uCStringWidth(char *name,IconPtr myIcon)
{
    XFontStruct *f;
    int width;
    
	if(!myIcon || !name || !xg.mydisplay)return 1;
	
	if(!myIcon->info){
        myIcon->info=XQueryFont(xg.mydisplay,XGContextFromGC(myIcon->gc));
    }
    f=myIcon->info;
    if(!f)return 1;
    width=XTextWidth(f,name,(int)strlen(name));
	/* XFreeFontInfo(NULL, f, 1); */
	return width;
}
int StringWidth(unsigned char *buff,struct Icon *myIcon)
{
	uFontInfo Info;
	    
	if(!buff || !myIcon)return 1;
	
	Info.CWidth=12;

	uGetFontInfo(myIcon,&Info);
		
	return (int)strlen((char *)buff)*Info.CWidth;
}
XFontStruct *LoadQueryScalableFont(Display *dpy, int screen, char *name, int size)
{
    int i,j, field;
    char newname[500];    /* big enough for a long font name */
    int res_x, res_y;     /* resolution values for this screen */
    /* catch obvious errors */
    if ((name == NULL) || (name[0] != '-')) return NULL;
    /* calculate our screen resolution in dots per inch. 25.4mm = 1 inch */
    res_x = DisplayWidth(dpy, screen)/(DisplayWidthMM(dpy, screen)/25.4);
    res_y = DisplayHeight(dpy, screen)/(DisplayHeightMM(dpy, screen)/25.4);
    /* copy the font name, changing the scalable fields as we do so */
    for(i = j = field = 0; name[i] != '\0' && field <= 14; i++) {
        newname[j++] = name[i];
        if (name[i] == '-') {
            field++;
            switch(field) {
            case 7:  /* pixel size */
            case 12: /* average width */
                /* change from "-0-" to "-*-" */
                newname[j] = '*';
                j++;
                if (name[i+1] != '\0') i++;
                break;
            case 8:  /* point size */
                /* change from "-0-" to "-<size>-" */
                sprintf(&newname[j], "%d", size);
                while (newname[j] != '\0') j++;
                if (name[i+1] != '\0') i++;
                break;
            case 9:  /* x-resolution */
            case 10: /* y-resolution */
                /* change from an unspecified resolution to res_x or res_y */
                sprintf(&newname[j], "%d", (field == 9) ? res_x : res_y);
                while(newname[j] != '\0') j++;
                while((name[i+1] != '-') && (name[i+1] != '\0')) i++;
                break;
            }
        }
    }
    newname[j] = '\0';
    /* if there aren't 14 hyphens, it isn't a well formed name */
    if (field != 14) return NULL;
    return XLoadQueryFont(dpy, newname);
}
int CreateSysFontList(struct SYSFONTLIST *pSysFontList)
{
	int aFontCount[]={0,0,0};
    char **name;
    int count,n,nn;
	
	if (!pSysFontList) return 0;
	
	pSysFontList->count = 0;
	pSysFontList->nRaster = aFontCount[0];
	pSysFontList->nVector = aFontCount[1];
	pSysFontList->nTrueType = aFontCount[2];
	
    /* name=XListFonts(xg.mydisplay,"*medium-r*iso8859-1",9000,&count); */
    
    /*
        name=XListFonts(xg.mydisplay,"*--0-*iso8859-1",9000,&count);
        get scalable fonts
	*/
	
    name=XListFonts(xg.mydisplay,"*iso8859-1",9000,&count);

    if(!name)return 1;
	


	if (!(pSysFontList->name = (char**)cMalloc(count*sizeof(char*),4736))) return 0;
	if (!(pSysFontList->type = (unsigned long*)cMalloc(count*sizeof(unsigned long),4737))) return 0;
	
	nn=0;
	for (n=0;n<count;n++) {
		pSysFontList->name[n]=strsave(name[n],8765);
		if(!pSysFontList->name[n])break;
		pSysFontList->type[n]=0;
		++nn;
	}

	pSysFontList->count = nn;

	XFreeFontNames(name);

	if(nn <= 0){
	   DestroySysFontList(pSysFontList);
	   return 0;
	}
		
	return 1;
}

int DestroySysFontList(struct SYSFONTLIST *pSysFontList)
{
	int n;
	
	if (!pSysFontList) return 0;
		
	for (n=0;n<pSysFontList->count;n++) {
		if (pSysFontList->name[n]) {
			cFree(pSysFontList->name[n]);
			pSysFontList->name[n] = NULL;
		}
	}

	if (pSysFontList->name) cFree((char*)pSysFontList->name);
	pSysFontList->name = NULL;
	if (pSysFontList->type) cFree((char*)pSysFontList->type);
	pSysFontList->type = NULL;
	
	pSysFontList->nTrueType = 0;
	pSysFontList->nRaster = 0;
	pSysFontList->nVector = 0;

	pSysFontList->count = 0;

	return 1;
}
int uDrawImageRect(iRect *r,IconPtr myIcon)
{
    uRect SRect,DRect;
	struct ScreenInfo *s;
	if(!r || !myIcon)return 1;
	
	
	if(!myIcon)return 1;
	
	s=&myIcon->ScreenRect;
	
	DRect=r->box;
	SRect=DRect;
	SRect.x=0;
	SRect.y=0;

    if(SetBuffers(DRect.xsize*DRect.ysize*3+512L))return 1;
	
	sScreenSetSize(r->box.xsize,r->box.ysize,s);
	
	return sScreenWrite(r->box.xsize,r->box.ysize,r->data,(unsigned char *)xg.buffout,r->type,
                &SRect,&DRect,s);
}
static int sScreenWrite(long xsize,long ysize,unsigned char *data,unsigned char *buff,int DataType,
                uRect *SRect,uRect *DRect,struct ScreenInfo *s)
{
	struct DisplayF d;
	long n;
	
	if(!SRect || !DRect || !s || !data || !xg.mydisplay)return 1;
	
	if(DataType == ScreenDataType8){
	    d.xsize=s->xImage->width;
	    d.ysize=s->xImage->height;
	    d.buffer=s->buffer;
		for(n=0;n<3*256;++n){
		    d.pd.palette[n]=s->myIcon->pal[n];
		}
		d.imageRect.xsize=(int)xsize;
					
		WriteRectToRect(data,SRect,xsize,ysize,buff,SRect,d.xsize,d.ysize);

		WriteHDFToScreen(&d,buff);
		
        XPutImage(xg.mydisplay,s->myIcon->myWindow, s->myIcon->gc, s->xImage,
            (int)0, (int)0, DRect->x,DRect->y, 
            (int)(s->xImage->width), (int)(s->xImage->height));	
				       	            
		
	}else if(DataType == ScreenDataType24){
	    d.xsize=s->xImage->width;
	    d.ysize=s->xImage->height;
	    
		WriteRect24ToRect24(data,SRect,xsize,ysize,buff,SRect,d.xsize,d.ysize);
		
	    if(Write24ToScreen((unsigned char *)buff,(unsigned char *)s->buffer,
	            (int)(d.xsize),(int)(d.ysize),s->colors))return 1;
		

        XPutImage(xg.mydisplay,s->myIcon->myWindow, s->myIcon->gc, s->xImage,
            (int)0, (int)0, DRect->x,DRect->y , 
            (int)(s->xImage->width), (int)(s->xImage->height));		

	}
	return 0;
}

struct FileList *uOpenFileDialogLocal(struct Icon *myIcon,int *TypeList)
{
	return uOpenFileDialog(myIcon,TypeList);
}
IconPtr NewCDraw(IconPtr myIcon)
{
	return NewDrawWindow(myIcon);
}

int uOpenFile()
{
	struct FileList *ret;

	ret=uOpenFileDialog(NULL,NULL);
	if(ret){
	   struct FileInfo2 *files;
	    files=FilesOpenList(ret);
	    if(files){
	        if(FilesOpenDefault(files)){
	            freeFileList(files);
	        }
	    }
	}
	return 0;
}
int saveUniversalPath(char *path)
{
	char *home;
	FILE8 *out;
	int ret;
	
	if(!path)return 1;
	
	ret=1;
	
	out=NULL;
	
	home=getenv("HOMESHOW");
	if(!home){
		home=getenv("HOME");
		if(!home)goto ErrorOut;
	}
	
    home=strpathname(home,".showPreferenceName");
    if(!home)goto ErrorOut;
    
    out=fopen8(home,"wb");
    if(!out){
        sprintf(WarningBuff,"saveUniversalPath Could Not Open '%s' To Write\n",home);
        WarningBatch(WarningBuff);
        sprintf(WarningBuff,"use 'setenv HOMESHOW LOCATION' to set new save path\n");
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }else{	    
    	if(saveUniversalString(path,9999,out)){
        	sprintf(WarningBuff,"saveUniversalPath Error Writing '%s'\n    path '%s' \n",home,path);
        	WarningBatch(WarningBuff);
        	sprintf(WarningBuff,"use 'setenv HOMESHOW LOCATION' to set new save path\n");
        	WarningBatch(WarningBuff);
        	goto ErrorOut;
    	}
    }
	    

	ret=0;
	
ErrorOut:	

	if(home)cFree(home);
	if(out)fclose8(out);

	return ret;
}
char *getUniversalPath(void)
{
	char *home;
	FILE8 *in;
	char head[5];
	long length;
	long tag;
	int ret;
	
	ret = 1;
	in  = NULL;
	home  = NULL;
	
	home=getenv("HOMESHOW");
	if(!home){
		home=getenv("HOME");
	}
	
	if(home){
	    home=strpathname(home,".showPreferenceName");
	    if(!home)goto ErrorOut;
	    in=fopen8(home,"rb");
	    if(!in){
	        sprintf(WarningBuff,"getUniversalPath Could Not Open '%s' To Read\n",home);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }else{	    
			if(getUniversalHeader(head,&tag,&length,in))goto ErrorOut;;
			if(strcmp(head,"STRI")){
			     WarningBatch("getUniversalPath Looking for STRI\n");
			     goto ErrorOut;
			}
           if(home)cFree(home);
	        home=NULL;
	        if(getUniversalStringPtr(&home,length,in)){
			     WarningBatch("getUniversalPath Error on getUniversalStringPtr\n");
			     goto ErrorOut;
		    }

	    }
	    
	}

	ret = 0;
	
ErrorOut:	

	if(ret){
        sprintf(WarningBuff,"use 'setenv HOMESHOW LOCATION' to set new save path\n");
        WarningBatch(WarningBuff);
        if(home){
            cFree(home);
            home=NULL;
        }
	}
	
	if(in)fclose8(in);
	
	return home;
}


int uSetPenColor(int r,int g,int b,struct Icon *myIcon)
{
	unsigned long color;
		
	if(!myIcon)return 1;
		
	color=0;
	
	getColorFromRGB(&color,r,g,b);
	
	XSetForeground (xg.mydisplay, myIcon->gc, color);
	
	
	return 0;
}

int uSetPenXOR(int flag,struct Icon *myIcon)
{
	XGCValues v;
	    
	if(!myIcon)return 1;
	
	if(flag){
	    v.foreground=myIcon->c[4];
	    v.background=myIcon->c[2];

	    v.foreground = v.foreground ^ v.background ;

	    v.function = GXxor ;  

	     XChangeGC(xg.mydisplay,myIcon->gc,  
	               GCForeground | GCBackground | GCFunction, &v);  

	}else{
	    v.foreground=myIcon->c[4];
	    v.background=myIcon->c[2];

	    v.function = GXcopy ;  

	     XChangeGC(xg.mydisplay,myIcon->gc,  
	               GCForeground | GCBackground | GCFunction, &v);  
	}

	return 0;
}



int uDisposeControl(struct Icon *myIcon,controlPtr control)
{
	if(!myIcon || !control)return 1;
	
	
	RemoveControlFromWindow(myIcon->myWindow,control);
	ControlDisposeWindow(control);	
	
	return 0;
}

int isInstalled(int item)
{
	struct InstalledList list[]= {
			{Installed_PrintPreview,FALSE},
			{Installed_PrintSetup,FALSE},
			{Installed_Print,FALSE},
			{Installed_RasterPreferences,TRUE},
			{Installed_CopyImages,TRUE},
			{Installed_PIO2DPreferences,TRUE},
			{Installed_PIO3DPreferences,TRUE},
			{Installed_TekPreferences,TRUE},			
			{Installed_LinePlots,TRUE},			
			{Installed_PointPlots,TRUE},			
			{Installed_DrawProgram,TRUE},			
	};
	
	int count=sizeof(list)/sizeof(struct InstalledList);
	int n;

	if(item <=0)return 0;
	
	for(n=0;n<count;++n){
	    if(item == list[n].item){
	        return list[n].isActive;
	    }
	}

	return 0;
}
int  uWriteBox(uRect *r,char *buff,IconPtr myIcon)
{


	if(!r || !myIcon)return 1;

	uSetTextFace(myIcon,0);
	
	uSetTextFont(myIcon,DefaultFont);
	
	uSetFontSize(myIcon,9);

	XSetForeground (xg.mydisplay,myIcon->gc,myIcon->c[6]);
	XSetBackground (xg.mydisplay,myIcon->gc,myIcon->c[7]);
	XFillRectangle(xg.mydisplay,myIcon->myWindow,
               myIcon->gc,
               r->x,r->y,
               r->xsize,
               r->ysize);
	XSetForeground (xg.mydisplay,myIcon->gc,myIcon->c[7]);
	XSetBackground (xg.mydisplay,myIcon->gc,myIcon->c[6]);
	
	if(buff){
		uMoveTo(r->x+5,
	           r->y+r->ysize-2);
	           
		uDrawString((char *)buff,myIcon); 
	}
	return 0;
}
int  uPaintBox(uRect *r,IconPtr myIcon)
{

	if(!r || !myIcon)return 1;

	XSetForeground (xg.mydisplay,myIcon->gc,myIcon->c[7]);
	XSetBackground (xg.mydisplay,myIcon->gc,myIcon->c[6]);
	
	
	XFillRectangle(xg.mydisplay,myIcon->myWindow,
               myIcon->gc,
               r->x,r->y,
               r->xsize,
               r->ysize);
               
	return 0;
}
int getPalColors(XColor *colors,unsigned char *pal)
{
	int i;

	if(!pal || !colors)return 1;

	for(i=0;i<256;++i){
	    pal[i*3]=(colors[i].red >> 8) & 255;
	    pal[i*3+1]=(colors[i].green >> 8) & 255;
	    pal[i*3+2]=(colors[i].blue >> 8) & 255;
	}
	pal[0]=255L;
	pal[1]=255L;
	pal[2]=255L;

	pal[765]=0;
	pal[766]=0;
	pal[767]=0;

	return 0;
}
int isDirectory(char *name)
{
	struct stat buf;
	
	if(!name)return 0;

    if(stat(name,&buf)){
        return 0;
    }
    
    if(buf.st_mode & S_IFDIR){
        return 1;		   
    }
	    	
	return 0;
}

char *strpathname(char *path,char *name)
{
	char *newpath;
	long size;
	
	if(!path || !name)return NULL;
	
	if(strrchr(name,'/')){
	    sprintf(WarningBuff,"strpathname Bad pathname %s\n",name);
	    WarningBatch(WarningBuff);
	    return NULL;
	}
	
	size=(long)(strlen(path)+strlen(name)+16);
	newpath=cMalloc(size,7362);
	if(!newpath)return NULL;
	
	mstrncpy(newpath,path,size);
    strncatToPath(newpath,name,(int)size);
	
	return newpath;
}

int FilesGetPICT(char *name,unsigned char *buff,long length,IconPtr myIcon,struct FileInfo2 **Files)
{
	if(!name || !buff || !myIcon || !Files)return 1;
	//length=length;
	return 0;
}

int doSystem(void)
{
	return 0;
}

int SetFileTypeOwner(char *newpath,char *BinaryOwner,char *BinaryType)
{
	if(!newpath || !BinaryOwner || !BinaryType)return 1;
	return 0;
}
int uDrawPalette(struct paletteDraw *pd,IconPtr myIcon)
{
	long xsizep,ysizep;
	int y,xs,ys,yp;
	int yflip;
	char out[256];
	double Small=1.0e-6;
	struct screenData s;
	uFontInfo Info;
	uRect cRect;
	unsigned char *image;
	int xh,yh;

	if(!pd)return 1;
	if(!pd->LabeledPalette)return 0;
	if(uGetPaletteSize(&xsizep,&ysizep,pd,myIcon))return 1;
	
	Info.CWidth=12;
	Info.LineHeight=12;
	
	zerol((char *)&s,sizeof(struct screenData));
	s.buffer=pd->buffer;
	s.viewRect.xsize=(int)pd->xsize;
	s.viewRect.ysize=(int)pd->ysize;
	s.ScreenBits = pd->ScreenBits;
	s.palette = pd->palette;
	yflip=(int)pd->ysize;
	
	xs=(int)(pd->left+xsizep-20);
	ys=(int)(pd->top+10);

   for(y=0;y<256;++y){
       yp=ys+y;
       MoveB(xs,yflip-yp);
       LineB(xs+15,yflip-yp,255-y,&s);
   }

	uGetFontInfo(myIcon,&Info);		
	xh=Info.CWidth;
	yh=Info.LineHeight;

	if(pd->sType == 0 || pd->sType == 2){
	    static char *Format=(char *)"%9.2e";
	    double cmin,cmax,dz,dh;
	    double zcon[200];
	    int ncon,n,ysize,ih;

	    ysize=255;
	    if(pd->sType == 0){
	        cmin=pd->dmin;
	        cmax=pd->dmax;
	    }else{
	        cmin=pd->sPmin;
	        cmax=pd->sPmax;
	    }

	    if(cmin >= cmax)cmax=cmin+1.0;

	    sear(cmax,cmin,zcon,&ncon,10);
	    dz=cmax-cmin;
	    for(n=0;n<ncon;++n){
	        if(zcon[n] > (cmax+1.0e-4*dz)){
	            break;
	        }else if(zcon[n] < cmin){
	            continue;
	        }
	        dh=(zcon[n]-cmin)/dz;
	        ih=(int)(ys+ysize-ysize*(dh));
		    sprintf((char *)out,Format,zcon[n]);
       		if(FontToRect(out,&cRect,&image,myIcon))break;
       		MoveB((int)(xs-8-strlen(out)*xh),yflip-(ih+yh/2));
       		if(FontToScreen(&cRect,image,255,&s))break;       		
       		MoveB(xs-5,yflip-ih);
       		LineB(xs+15,yflip-ih,255,&s);
	        
	    }
	  }else if(pd->sType == 1){
	    double y,End,Start,Step,yViewMinLog,yViewMaxLog,scale,slide;
	    int iy,ysize,iEnd,iStart,iDiff;

	    ysize=255;

	    yViewMaxLog=log10(pd->sPmax);
	    if(yViewMaxLog < 0.0){
	       End=yViewMaxLog*(1-Small);
	    }else{
	       End=yViewMaxLog*(1+Small);
	    }


	    yViewMinLog=log10(pd->sPmin);
	    if(yViewMinLog < 0.0){
	       slide=yViewMinLog*(1+Small);
	    }else{
	       slide=yViewMinLog*(1-Small);
	    }
	    iEnd=(int)End;
	    iStart=(int)(yViewMinLog-.9999);
	    iDiff=iEnd-iStart;
	    if((iDiff) <= 9){
	        Step=1;
	    }else{
	        Step=4;
	    }
	    Start=iStart;
	    scale=ysize/(log10(pd->sPmax)-yViewMinLog);
  	    for(y=Start;y <= End;y += Step){
  	        double yy,yn;
	        if(y > End)break;	   
	        if(y < slide)continue;
		    iy=(int)(ys+ysize-scale*(y-yViewMinLog)+.5);
	        sprintf((char *)out,"%g%c",pow(10.,y),0);
       		if(FontToRect(out,&cRect,&image,myIcon))break;
   			MoveB((int)(xs-8-strlen(out)*xh),yflip-(iy+yh/2));
        	if(FontToScreen(&cRect,image,255,&s))break;       		
  			MoveB(xs-5,yflip-iy);
   			LineB(xs+15,yflip-iy,255,&s);
	        if(iDiff > 4)continue;
	        for(yn=2;yn < 10;++yn){
	            yy=y+log10(yn);
	            if(yy > End)break;
		        iy=(int)(ys+ysize-scale*(yy-yViewMinLog)+.5);
		        /*
		        MoveTo(xs-3,iy);
	            LineTo(xs,iy); 
	            */
       			MoveB(xs-3,yflip-iy);
       			LineB(xs,yflip-iy,255,&s);
		    }
	    }
	  }
	return 0;
}
int FontToScreen(uRect *cRect,unsigned char *image,int nc,struct screenData *s)
{
	int i,j;
	int x, y;
	
	if(!cRect || !image || !s)return 1;
	
	if(FindB(&x,&y))return 1;
	
	for(j=0;j<cRect->ysize;++j){
	    for(i=0;i<cRect->xsize;++i){
	         if(image[i+j*cRect->xsize]){
	             kpoint(i+x,j+y,nc,s);
	         }
	    }
	}
	
	if(image)cFree((char *)image);

	return 0;
}
int WarningBatch(char *WarningBuff)
{
/*
	static volatile pthread_t who;
	pthread_t me=pthread_self();
*/
	int ret;
	int n;
	
	if(!WarningBuff)return 1;
	
	ret=0;
/*	
	while(1){
	    if(!who){
	        who=me;
	        if(who == me)break;
	    }else if(who == me){
	        break;
	    }
	}
*/	
	if(xg.mydisplay == NULL){
		while(*WarningBuff){
		    n = *WarningBuff++;
		    if(n == '\r' || n == '\n'){
		        printf("\n");
		        fflush(stdout);
		    }else{
		        putc(n,stdout);
		    }
		}
	}else{
	   	ret=WriteToWindow("BatchPrint",WarningBuff);
	}
	
 	
 	/*
	who=(pthread_t)NULL;
	*/
	return ret;
}
int WarningBatchFast(char *WarningBuff)
{
	int n;
	
	if(!WarningBuff)return 1;
	
	if(xg.mydisplay == NULL){
		while(*WarningBuff){
		    n = *WarningBuff++;
		    if(n == '\r' || n == '\n'){
		        printf("\n");
		        fflush(stdout);
		    }else{
		        putc(n,stdout);
		    }
		}
		putc('\n',stdout);
	}else{
	   return WriteToWindowFast("BatchPrint",WarningBuff);
	}
	return 0;
}
int WarningBatchAtEnd(char *WarningBuff)
{	
	if(!WarningBuff)return 1;
	
	if(xg.mydisplay != NULL){
	   return WriteToWindowAtEnd("BatchPrint",WarningBuff);
	}
	return 0;
}
int FlushCurrentPort(void)
{
	while(1){
	   if(!doEvents())break;
	}
	return 0;
}
int uStillDown(IconPtr myIcon)
{
		Window child,root;
		unsigned int mask;
		int press_x,press_y;
		int win_x,win_y;
		XEvent event;
		int ret;


		if(!myIcon ||  !xg.mydisplay)return 0;
		
		XQueryPointer(xg.mydisplay,myIcon->myWindow,
		&root,&child,&press_x,&press_y,
		&win_x,&win_y,&mask);
		
		ret = mask & 0x100;
		
		if(!ret){		
			while(XCheckMaskEvent(xg.mydisplay,PointerMotionMask,&event)){
			    ;
			}		
		}
		
		return ret;
}
/*
int uStillDown(IconPtr myIcon)
{
	XEvent event;
	if(!myIcon)return 0;

	if(XCheckMaskEvent(xg.mydisplay,StructureNotifyMask | EnterWindowMask |
	                   ButtonPressMask | ButtonReleaseMask | ExposureMask ,&event)){
		if(NotMyWindow(myIcon,&event))return 0;
		switch(event.type){
		case Expose:
		    doExpose(myIcon,&event);
		    break;
		case ButtonRelease:
		    return 0;
		}
	}
	return 1;
}
*/
int uPointer(uPoint *pt,IconPtr myIcon)
{
		Window child,root;
		unsigned int mask;
		int press_x,press_y;
		int win_x,win_y;


		if(!pt || !myIcon ||  !xg.mydisplay)return 1;
		
		XQueryPointer(xg.mydisplay,myIcon->myWindow,
		&root,&child,&press_x,&press_y,
		&win_x,&win_y,&mask);
/*	
	        printf("root %x child %x\n",root,child);		
	        printf("press_x %d press_y %d win_x %d win_y %d\n",
	            press_x,press_y,win_x,win_y);	
	
*/
		    pt->x=win_x;
		    pt->y=win_y;
		    return 0;
}
int strncatToPath(char *pathname,char *name,int length)
{
	int nn;
	
	if(!pathname || !name)return 1;
	
	nn=(int)strlen(pathname);
	if(nn > 0){
	    if(pathname[nn-1] != FILE_NAME_SEPERATOR){
	        pathname[nn] = FILE_NAME_SEPERATOR;
	        if(nn+1 < length)pathname[nn+1] = 0;
	    }
	}
	mstrncat(pathname,name,length);
	return 0;
	    
}
int GetWindowName(struct Icon *myIcon,char *name,int flag)
{
	char name2[256];
	char *cp;
	char *nam;
	
	if(!myIcon || !name)return 1;
	
	//flag=flag;
	
	mstrncpy(name2,myIcon->WindowName,256);
	
	nam=NULL;
	XFetchName(xg.mydisplay,myIcon->myWindow,&nam);
	if(nam){
		mstrncpy(name2,nam,256);
		XFree(nam);
	}
/*
	cp=strrchr(name2,'[');
	if(cp){
	   *cp=0;
	}
*/
	cp=strrchr(name2,'/');
	if(cp){
		mstrncpy(name,cp+1,256);
	}else{
		cp=strrchr(name2,':');
		if(cp){
			mstrncpy(name,cp+1,256);
		}else{
			mstrncpy(name,name2,256);
		}
	}
			
	return 0;
}
int SetWindowName(IconPtr myIcon,char *name)
{
	XSizeHints hint;
	char nameWindow[256],nameHost[256],*p;

	if(!myIcon || !name)return 1;

	if(myIcon->WindowNameSet)return 0;
	
	FlushCurrentPort();
	
	hint.x = 0;
	hint.y = 0;
	hint.width  =  myIcon->viewRect.xsize;
	hint.height =  myIcon->viewRect.ysize;
	hint.flags = PSize;

	nameHost[0]=0;

	gethostname(nameHost,256);
	p=strchr(nameHost,'.');
	if(p){
	    *p=0;
	}
	mstrncpy(nameWindow,nameHost,255);
	mstrncat(nameWindow,":",255);
	mstrncat(nameWindow,name,255);

	mstrncpy(myIcon->WindowName,nameWindow,255);

	XSetStandardProperties (xg.mydisplay, myIcon->myWindow, nameWindow, nameWindow,
	        None, xg.argv, xg.argc, &hint);
	        
	GetWindowName(myIcon,myIcon->WindowName,0);
	
	return 0;
}
IconPtr  ListWindow(int XCount,int YCount,int XLength,int YLength)
{
	Warning("ListWindow Not Yet Active");
	//XCount=XCount;
	//YCount=YCount;
	//XLength=XLength;
	//YLength=YLength;
	return NULL;
}
int uClipRect(uRect *r,struct Icon *myIcon)
{
	XRectangle xr;
	
	if(!myIcon || !r || !xg.mydisplay)return 1;
	
	xr.x=r->x;
	xr.y=r->y;	
	xr.width=r->xsize;
	xr.height=r->ysize;
	
	XSetClipRectangles(xg.mydisplay,myIcon->gc,0,0,&xr,1,0);
	return 0;
}
int uEraseRect(uRect *r,struct Icon *myIcon)
{	
	uRect xr;
	
	if(!myIcon || !r || !xg.mydisplay)return 1;
	
	xr.x=r->x;
	xr.y=r->y;	
	xr.xsize=r->xsize;
	xr.ysize=r->ysize;
	
	XSetForeground (xg.mydisplay,myIcon->gc, myIcon->c[6]);
	XSetBackground (xg.mydisplay,myIcon->gc, myIcon->c[6]);
	XFillRectangle(xg.mydisplay,myIcon->myWindow,
	               myIcon->gc,
	               xr.x,xr.y,
	               xr.xsize,
	               xr.ysize);
	               
	               
    XSetForeground (xg.mydisplay,myIcon->gc,myIcon->c[7]);
    XSetBackground (xg.mydisplay,myIcon->gc,myIcon->c[6]);
	               
	if(myIcon->Screen.buffer){
	    zerol((char *)myIcon->Screen.buffer,myIcon->Screen.length);
	}
	return 0;
}
int uPaintRect(uRect *r,struct Icon *myIcon)
{
	uRect xr;
	
	if(!r || !myIcon)return 1;
	
	xr.x=r->x;
	xr.y=r->y;
	
	xr.xsize=r->xsize;
	xr.ysize=r->ysize;

	XFillRectangle(xg.mydisplay,myIcon->myWindow,
	   myIcon->gc,
	   xr.x,xr.y,
	   xr.xsize,
	   xr.ysize);
	   
	return 0;
}
int uFrameRect(uRect *r,struct Icon *myIcon)
{
	uRect xr;
	
	if(!r || !myIcon)return 1;
	
	xr.x=r->x;
	xr.y=r->y;
	
	xr.xsize=r->xsize;
	xr.ysize=r->ysize;
	
	FrameRect(&xr,myIcon->myWindow,myIcon->gc);
	return 0;
}
int uInvertRect(uRect *r,struct Icon *myIcon)
{
	uRect xr;
	
	if(!myIcon || !r || !xg.mydisplay)return 1;
	
	xr.x=r->x;
	xr.y=r->y;
	
	xr.xsize=r->xsize;
	xr.ysize=r->ysize;
	
	XFillRectangle(xg.mydisplay,myIcon->myWindow,
	               myIcon->gcRubber,
	               xr.x,xr.y,
	               xr.xsize,
	               xr.ysize);
	
	return 0;
}

int uCheckMenuItem(struct Icon *myIcon,int menu,int item,int value)
{
	menuPtr Menu;
	long menudata;
	
	if(!myIcon)return 1;
		
	menudata=(menu << 16) | item;
		
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
	              uWindowSetDone,uWindowSetDone))return 1;

	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;
         
	if(value){
		MenuSetMark(MENU_RADIO_MARK,item,Menu);
	}else{
		MenuSetMark(MENU_NO_MARK,item,Menu);
	}
	return 0;
}
int uDisableMenuItem(struct Icon *myIcon,int menu,int item)
{
	menuPtr Menu;
	long menudata;
	
	if(!myIcon)return 1;
		
	menudata=(menu << 16) | item;
		
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
	              uWindowSetDone,uWindowSetDone))return 1;

	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;
         
	MenuDisable(Menu,item);
		
	return 0;
}
int ZeroScrap(void)
{
	if(scrap)cFree(scrap);
	scrap=NULL;
	return 0;
}
long putStringToScrap(char *mess)
{
	if(!mess)return 1;
	ZeroScrap();
	scrap=strsave(mess,92876);
	if(!scrap)return 1;
	return 0;
}
char *getStringFromScrap(void)
{
	char *ret;
	
	if(!scrap)return NULL;
	
	ret=strsave(scrap,92876);
	
	return ret;
}

static unsigned char *FLST;
static long FLSTLenght;
static unsigned char *DRW3;
static long DRW3Lenght;
static unsigned char *TEK3;
static long TEK3Lenght;
static unsigned char *DRWdataPtr;
static long DRWxsize;
static long DRWysize;

int drawToScrap(IconPtr myIcon,uRect *Boxi,long CurrentFrame,int flag)
{
	struct sWorld World;
	int retd;
	int ret;
	
	if(!myIcon || !Boxi)return 1;
	
	if(flag){
	 	ZeroScrap();
	 	uZeroScrap();
	}
	
	ret=1;
	
	zerol((char *)&World,sizeof(struct sWorld));
	
	World.myIcon=myIcon;
	
	if(DRWdataPtr)cFree((char *)DRWdataPtr);
	DRWdataPtr=NULL;
	
 	if(uOpenGWorld(myIcon,&World,Boxi,GWorldDepth32)){
			goto OutOfHere;
	}	
	
    StartGWorld(&World);
    retd=pDrawGo(myIcon,CurrentFrame,Boxi);
    StopGWorld(&World);
    if(retd)goto OutOfHere;
		
    if(GetTrueFromGWorld(&DRWxsize,&DRWysize,(unsigned char **)&DRWdataPtr,&World))goto OutOfHere;
	
	
	//CurrentFrame=CurrentFrame;
	//flag=flag;

	ret=0;
OutOfHere:	
	return ret;
}

int getTrueFromScrap(long *xsize,long *ysize,unsigned char **image)
{
	long length;
	
	if(!xsize || !ysize || !image)return 1;
	
	*xsize=0;
	*ysize=0;
	*image=NULL;
	if(!DRWdataPtr)return 1;
	length=DRWxsize*DRWysize*3;
	*image=cMalloc(length,8619);
	if(!*image)return 1;
	
	memcpy(*image,DRWdataPtr,length);
	*xsize=DRWxsize;
	*ysize=DRWysize;
	
	return 0;
}


int uZeroScrap(void)
{
   if(FLST)cFree((char *)FLST);
   FLST=NULL;
   if(DRW3)cFree((char *)DRW3);
   DRW3=NULL;
   if(TEK3)cFree((char *)TEK3);
   TEK3=NULL;
   if(DRWdataPtr)cFree((char *)DRWdataPtr);
   DRWdataPtr=NULL;
   return 0;
}

long uPutScrap(long Count,char *type,char *out)
{
	long Length;
	
	if(!type || !out)return 0L;
	
	//Count=Count;
	
	Length=0;
	if(!mstrcmp("FLST",type)){
		if(FLST)cFree((char *)FLST);
		FLST=cMalloc(Count,8265);
		if(!FLST)return 0;
		memcpy(FLST,out,Count);
		FLSTLenght=Count;
		Length=Count;
	}else if(!mstrcmp("DRW3",type)){
		if(DRW3)cFree((char *)DRW3);
		DRW3=cMalloc(Count,8265);
		if(!DRW3)return 0;
		memcpy(DRW3,out,Count);
		DRW3Lenght=Count;
		Length=Count;
	}else if(!mstrcmp("TEK3",type)){
		if(TEK3)cFree((char *)TEK3);
		TEK3=cMalloc(Count,8265);
		if(!TEK3)return 0;
		memcpy(TEK3,out,Count);
		TEK3Lenght=Count;
		Length=Count;
	}

	return Length;
}
long uGetScrap(char *type,char **out)
{
	long Length;
	
	if(!type || !out)return 0L;

	Length=0;
	if(!mstrcmp("FLST",type)){
		if(FLST && (FLSTLenght > 0)){
			*out=cMalloc(FLSTLenght,8265);
			if(!*out)return 0;
			Length=FLSTLenght;
			memcpy(*out,FLST,Length);
		}
	}else if(!mstrcmp("DRW3",type)){
		if(DRW3 && (DRW3Lenght > 0)){
			*out=cMalloc(DRW3Lenght,8265);
			if(!*out)return 0;
			Length=DRW3Lenght;
			memcpy(*out,DRW3,Length);
		}
	}else if(!mstrcmp("TEK3",type)){
		if(TEK3 && (TEK3Lenght > 0)){
			*out=cMalloc(TEK3Lenght,8265);
			if(!*out)return 0;
			Length=TEK3Lenght;
			memcpy(*out,TEK3,Length);
		}
	}


	return Length;
}

int uSetMenuItemText(struct Icon *myIcon,int menu,int item,unsigned char *name)
{
	menuPtr Menu;
	long menudata;
	
	if(!myIcon || !name)return 1;
		
	menudata=(menu << 16) | item;
		
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
	              uWindowSetDone,uWindowSetDone))return 1;

	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;

	MenuSetName((char *)name,item,Menu);

	return 0;

}
int uEnableMenuItem(struct Icon *myIcon,int menu,int item)
{
	menuPtr Menu;
	long menudata;
	
	if(!myIcon)return 1;
		
	menudata=(menu << 16) | item;
		
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
	              uWindowSetDone,uWindowSetDone))return 1;

	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;

	MenuEnable(Menu,item);

	return 0;
}
struct Menu *uMenuBarAddMenus(char *MenuName,
                             struct uMenuItems *Items,
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
	    Items[n].ItemID=(int)n;
	    ++n;
	}

	if(MenuBarAdd(Menur,MenuBarList))goto ErrorOut;

	return Menur;
ErrorOut:
	return (struct Menu *)NULL;
}
int GetPaletteColors(struct Icon *myIcon,unsigned char *r,unsigned char *g,unsigned char *b)
{
	int n;
	if(!myIcon || !r || !g || !b)return 1;
	
	for(n=0;n<256;++n){
	    r[n]=myIcon->pal[3*n];
	    g[n]=myIcon->pal[3*n+1];
	    b[n]=myIcon->pal[3*n+2];
	}
	return 0;
}

int Stop(char *name)
{
	XEvent event;
	int ret;
	
	if(!name)return 1;	

	if(XCheckMaskEvent(xg.mydisplay,KeyPressMask,&event)){
		switch(event.type){
		case KeyPress:
		    ret=Warning2(name,1);
		    return !ret;
		}
	}	
	
	return 0;
}

#ifdef MAC_RESOURCES

int makeCommand(char *command, char *file, char *Application, int length);

struct fileinfobuf {
   u_int32_t info_length;
   union {
     u_int32_t padding[8];
     struct {
       u_int32_t type;
       u_int32_t creator;
       u_int16_t fdFlags;
       u_int16_t location;
       u_int32_t padding[4];
     } info;
   } data;
};

int SetFileType(char *path,char *type)
{
   	struct attrlist alist;
  	struct fileinfobuf finfo;
	u_int32_t creatorName;
	u_int32_t typeName;
	char *crea="NTL1";
	int ret;
   	int err;
	int n;
	
	if(!path || !type)return 1;
	
	ret=1;
	
	alist.bitmapcount = 5;
	alist.reserved = 0;
	alist.commonattr = ATTR_CMN_FNDRINFO;
	alist.volattr = 0;
	alist.dirattr = 0;
	alist.fileattr = 0;
	alist.forkattr = 0;

	err = getattrlist(path, &alist, &finfo, sizeof(finfo),
	            FSOPT_NOFOLLOW);

	if(err) {
		sprintf(WarningBuff, "SetFileType Can't get file information for %s\n", path);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
		
	typeName=0;
	creatorName=0;
	for(n=0;n<4;++n){
#ifdef PC
		typeName    |= ((unsigned long)(type[n] & 255) << 8*(n));
		creatorName |= ((unsigned long)(crea[n] & 255) << 8*(n));
#else
		typeName    |= ((unsigned long)(type[n] & 255) << 8*(3-n));
		creatorName |= ((unsigned long)(crea[n] & 255) << 8*(3-n));
#endif
	}

	finfo.data.info.type=typeName;
	finfo.data.info.creator=creatorName;


	err = setattrlist(path, &alist, &finfo.data, sizeof(finfo.data), 1);
	if(err) {
		sprintf(WarningBuff, "SetFileType Error while setting file information for %s\n",path);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	
	{
		char command[4096];
		if(uPref.OpenWithApplication){
	        makeCommand(command, path, uPref.OpenWithApplication, sizeof(command));
			ret=system(command);
		}
	}
	
	ret=0;
ErrorOut:
	
	return ret;
}
#else
int SetFileType(char *path,char *type)
{
	int ret;
	
	
	if(!path || !type)return 1;
	
	
	
	ret=0;
	
	return ret;
}
#endif
int uGetClipRect(uRect *r,struct Icon *myIcon)
{
	if(!myIcon || !r)return 1;
	return 0;
}

int uSetClipRect(uRect *r,struct Icon *myIcon)
{
	if(!myIcon || !r)return 1;
	return 0;
}


int uFlushVol(void)
{
	return 0;
}

int doSaveQuickTimeOptions(IconPtr myIcon,struct ImageSaveList *Images)
{
	if(!myIcon || !Images)return 1;
	return 0;
}

int uInsertMenuItem(menuPtr menu,char *name,int location)
{
	if(!menu || !name)return -1l;
	return MenuInsert(name,MENU_SIMPLE,menu,location);
}

long uCountMenuItems(menuPtr menu)
{
	if(!menu)return -1l;
	return menu->menuCount;
}

int uDeleteMenuItem(menuPtr menu,int item)
{
	if(!menu)return -1l;
	return MenuDelete(item-1,menu);
}
int uDrawString(char *name,struct Icon *myIcon)
{
	if(!name || !myIcon || !xg.mydisplay)return 1;
	XDrawImageString(xg.mydisplay,myIcon->myWindow,myIcon->gc,
	        xold,yold,(char *)name,(int)strlen((char *)name));
	return 0;
}

int uMoveTo(int ix,int iy)
{
    xold=ix;
    yold=iy;
	return 0;
}		 
int uLineTo(int ix,int iy,struct Icon *myIcon)
{
	if(!myIcon || !xg.mydisplay)return 1;
	
	XDrawLine(xg.mydisplay,myIcon->myWindow,myIcon->gc,xold,yold,
	ix,iy);
		
    xold=ix;
    yold=iy;
    
        
	return 0;
}

static int sScreenSetSize(long xsize,long ysize,struct ScreenInfo *s)
{
	if(!s || !xg.mydisplay)return 1;
		
	if(!s->xImage || (s->xImage->width  != xsize ||
			 s->xImage->height != ysize)){
	    int xImageDepth;

	    if(s->buffer)cFree((char *)s->buffer);
	    s->buffer=NULL;

	    if(s->xImage){
	        s->xImage->data=NULL;
	        XDestroyImage(s->xImage);
	        s->xImage=NULL;
	    }

	    s->length=xsize*ysize*4;

	    xImageDepth=xg.vBits;

	    s->buffer=(unsigned char *)cMalloc(s->length,8658);
	    if(!s->buffer){
	         fprintf(stderr,"ScreenSetSize out of memory (%ld)\n",s->length);
	         return 1;
	    }

	    s->xImage=XCreateImage(xg.mydisplay,xg.xVisual,xImageDepth,
	           ZPixmap,0,
	           (char *)s->buffer,(unsigned int)xsize,(unsigned int)ysize,8,0);
	     if(!s->xImage){
            fprintf(stderr,"ScreenSetSize XCreateImage Error xImageDepth %d xsize %ld ysize %ld\n",
	                        xImageDepth,(long)xsize,(long)ysize);
		    return 1;
	     }
	}

	return 0;
}
int ScreenSetSize(long xsize,long ysize,IconPtr myIcon)
{
	struct ScreenInfo *s;
/*	uRect r; */
	
	if(!myIcon)return 1;
	
	s=&myIcon->Screen;
	
	if(sScreenSetSize(xsize,ysize,s))return 1;
/*	
	r.x=0;
	r.y=0;
	r.xsize=xsize;
	r.ysize=ysize;
	uEraseRect(&r,myIcon);
*/	
	return 0;
}
int uScreenWrite(long xsize,long ysize,unsigned char *data,unsigned char *buff,int DataType,
                uRect *SRect,uRect *DRect,struct Icon *myIcon)
{
	struct ScreenInfo *s;
	struct DisplayF d;
	uRect DDrect;
	int x,y;
	long n;
	
	if(!SRect || !DRect || !myIcon || !data || !xg.mydisplay)return 1;
	
	s = &myIcon->Screen;
	
	s->DataType=DataType;
	
	DDrect = *DRect;
	x=DDrect.x;
	DDrect.x=0;
	y=DDrect.y;
	DDrect.y=0;
				
	if(DataType == ScreenDataType8){
	    d.xsize=s->xImage->width;
	    d.ysize=s->xImage->height;
	    d.buffer=s->buffer;
		for(n=0;n<3*256;++n){
		    d.pd.palette[n]=s->myIcon->pal[n];
		}
		d.imageRect.xsize=(int)xsize;
					
		WriteRectToRect(data,SRect,xsize,ysize,buff,&DDrect,d.xsize,d.ysize);

		WriteHDFToScreen(&d,buff);
		
        XPutImage(xg.mydisplay,s->myIcon->myWindow, s->myIcon->gc, s->xImage,
            (int)0, (int)0, x,y, 
            (int)(s->xImage->width), (int)(s->xImage->height));	
				       	            
		s->buff=buff;
		s->data=data;
		
		s->xsize=d.xsize;
		s->ysize=d.ysize;
		
	}else if(DataType == ScreenDataType24){
	    d.xsize=s->xImage->width;
	    d.ysize=s->xImage->height;
	    
		WriteRect24ToRect24(data,SRect,xsize,ysize,buff,&DDrect,d.xsize,d.ysize);
		
	    if(Write24ToScreen((unsigned char *)buff,(unsigned char *)s->buffer,
	            (int)(d.xsize),(int)(d.ysize),s->colors))return 1;
		

        XPutImage(xg.mydisplay,s->myIcon->myWindow, s->myIcon->gc, s->xImage,
            (int)0, (int)0, x,y , 
            (int)(s->xImage->width), (int)(s->xImage->height));		

		s->buff=buff;
		s->data=data;
		
		s->xsize=d.xsize;
		s->ysize=d.ysize;
	}
	
	DDrect.x=x;
	DDrect.y=y;

	return 0;
}

int ScreenSetPalette(IconPtr myIcon,unsigned char *Red,unsigned char *Green,unsigned char *Blue)
{
	struct ScreenInfo *s,*s2;
	int n;
	
	if(!myIcon)return 1;
	if(!Red || !Green || !Blue)return 1;
	s=&myIcon->Screen;
	s2=&myIcon->ScreenRect;
		
	for(n=0;n<256;++n){
	    s->r[n]=Red[n];
	    s->g[n]=Green[n];
	    s->b[n]=Blue[n];
	    s->pal[n*3]=Red[n];
	    s->pal[n*3+1]=Green[n];
	    s->pal[n*3+2]=Blue[n];
	    s2->r[n]=Red[n];
	    s2->g[n]=Green[n];
	    s2->b[n]=Blue[n];
	    s2->pal[n*3]=Red[n];
	    s2->pal[n*3+1]=Green[n];
	    s2->pal[n*3+2]=Blue[n];
	    myIcon->pal[n*3]=Red[n];
	    myIcon->pal[n*3+1]=Green[n];
	    myIcon->pal[n*3+2]=Blue[n];
	}
	
	return 0;
}

void pNewMenu(IconPtr myIcon,int item)
{
	if(!myIcon)return;
	//item=item;
	return;
}


int uDrawGrowIcon(struct Icon *myIcon)
{
	if(!myIcon)return 1;
	return 0;
}
int uFrontWindow(struct Icon *myIcon)
{
	if(!myIcon)return 1;
	return 0;
}

int uSizeWindow(struct Icon *myIcon,int xsize,int ysize)
{
	if(!myIcon || !xg.mydisplay)return 1;	
	if(xsize <= 0)xsize=20;
	if(ysize <= 0)ysize=20;
	XResizeWindow(xg.mydisplay,myIcon->myWindow,xsize,ysize);
	if(myIcon->pMoveControls){
		if(myIcon->MenuBarList)SizeMenuBar(myIcon->MenuBarList,xsize,25);
	}  
	return 0;
}
int uSizeWindowGlobal(struct Icon *myIcon,int xsize,int ysize)
{
	if(!myIcon || !xg.mydisplay)return 1;	
	if(xsize <= 0)xsize=20;
	if(ysize <= 0)ysize=20;
	XResizeWindow(xg.mydisplay,myIcon->myWindow,xsize,ysize);
	if(myIcon->pMoveControls){
		if(myIcon->MenuBarList)SizeMenuBar(myIcon->MenuBarList,xsize,25);
	}  
	return 0;
}

int uMoveWindow(struct Icon *myIcon,int xsize,int ysize)
{
	if(!myIcon || !xg.mydisplay)return 1;
	XMoveWindow(xg.mydisplay,myIcon->myWindow,xsize,ysize);
	return 0;
}


void CWindowdoPalMenu(IconPtr myIcon,int menu,short Item)
{
    XColor colors[256];
    char buff[256];
	menuPtr Palette;
	
	if(!myIcon)return;
	
	Palette=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Palette,
		uWindowSetDone,uWindowSetDone)
	)goto OutOfHere;
	
	
	    switch(Item){
		case 1:
		  	 doGetPalette(myIcon);
		  break;
		case 2:
			doSavePalette(myIcon);
		  break;
		case 3:
			doAddPalette(myIcon,NULL);
		  break;
		case 4:
		  GetOldColorPalette(myIcon);
		  break;
		case 5:
		  	LoadToMenu(myIcon);
		  break;
		case 6:
		  SynchronizePalettes(myIcon,myIcon->red,myIcon->green,myIcon->blue);
		  break;
		case 7:
	     doPaletteToTable(myIcon);
		  break;
		default:
			Item -= 1;
		    if(Item >= PaletteStart && Item < MenuCount(Palette))
		    {
		        char buff[256];
		        int n,count;

			CheckControlAndShiftKeys();

			if(IsShift()){
			    MenuGetName(Item,buff,Palette);
			    deleteUniversalPaletteByName(buff);
			    MenuDelete(Item,Palette);
			    goto OutOfHere;
			}

			if(MenuGetPalette(Item,colors,Palette)){
			    sprintf(buff,"Error no palette for menu item %d\n",Item);
			    WarningBatch(buff);
			    goto OutOfHere;
			}
			if(xg.vBits == 8)XStoreColors(xg.mydisplay,xg.xColormapDefault,colors,256);
				count=(int)MenuCount(Palette);
				for(n=PaletteStart;n<count;++n)MenuSetMark(MENU_NO_MARK,n,Palette);
				MenuSetMark(MENU_CHECK_MARK,Item,Palette);
				if(MenuGetName(Item,buff,Palette))goto OutOfHere;
				mstrncpy(myIcon->paletteName,buff,255);
				if(getPalColors(colors,myIcon->pal))goto OutOfHere;
				for(n=0;n<256;++n){
			    	myIcon->red[n]=myIcon->pal[3*n];
			     	myIcon->green[n]=myIcon->pal[3*n+1];
			     	myIcon->blue[n]=myIcon->pal[3*n+2];
				}
				SetOldColorPalette(myIcon);
				SetDefaultPaletteName(myIcon,myIcon->paletteName);
				goto OutOfHere;
		    }
	          sprintf(buff,"Unknown PALETTE_MENU Item = %d\n",Item);
	          WarningBatch(buff);
	          goto OutOfHere;
	    }
OutOfHere:	
	    InvalRectMyWindow(myIcon);
	return;
}

void CWindowSetPalMenu(IconPtr myIcon)
{
	unsigned char *pal;
	menuPtr Pal;
	int k,mCount,n;
	char *name;
	
	if(!myIcon)return;

	if(uSetWindow(myIcon,	       
		uWindowGetMenu,myIcon->paletteMenuNumber,&Pal,
		uWindowSetDone,uWindowSetDone)
	)goto OutOfHere;

	
	mCount=(int)uCountMenuItems(Pal);
	for(k=0;k<mCount;++k){
	    uDeleteMenuItem(Pal,1);
	}
	
	sortUniversalPalette();
	
    n=0;
    while(!getUniversalPaletteItem(n++,&name,&pal)){
        uInsertMenuItem(Pal,name,500);
        MenuSetPalette((char *)pal,n-1,Pal);
    }

	uInsertMenuItem(Pal,"--------------------",0);
	uInsertMenuItem(Pal,"Palette To Table    ",0);
	uInsertMenuItem(Pal,"Sychronize Palettes ",0);
	uInsertMenuItem(Pal,"Load Menu Palette...",0);
	uInsertMenuItem(Pal,"Restore Palette   /R",0);
	uInsertMenuItem(Pal,"Add Palette...      ",0);
	uInsertMenuItem(Pal,"Save Palette...     ",0);
	uInsertMenuItem(Pal,"Load Palette...     ",0);

OutOfHere:
	
	return;
}

int Print(IconPtr myIcon)
{
	if(!myIcon)return 1;
	return 0;
}

int PageSetup(IconPtr myIcon)
{
	if(!myIcon)return 1;
	return 0;
}

int PagePreview(IconPtr myIcon)
{
	if(!myIcon)return 1;
	return 0;
}

int StandardDoActive(IconPtr myIcon,int flag)
{
	if(!myIcon)return 1;
	//flag=flag;
	return 0;
}
int  HiliteMenu(int value)
{
	return value;
}
int doWindowMenu(void)
{
	return 0;
}
int pWindowMenu(IconPtr myIcon,int item)
{
	if(!myIcon)return 1;
	//item=item;
	return 0;
}

int SetPortMyWindow(IconPtr myIcon)
{
	if(!myIcon)return 1;
	return 0;
}
int  uHiliteControl(controlPtr control,int value)
{
	if(!control)return 1;
	//value=value;
	/* HiliteControl(control,value); */
	return 0;
}


int uGetPort(uPort *Port)
{
	if(!Port)return 1;
	Port->save=0;
	return 0;
}

int uSetPort(uPort *Port)
{
	if(!Port)return 1;
	Port->save=1;
	return 0;
}

int uDrawControls(IconPtr myIcon)
{
	controlPtr c;

	if(myIcon->controlList){
	    c=myIcon->controlList;
	    DrawControl(c);
	    while(c->controlNext){
	        DrawControl(c->controlNext);
	        c=c->controlNext;
	    }
	}
	
	if(myIcon->MenuBarList)MenuBarUpdate(myIcon->MenuBarList);

	return 0;
}

static int doButtonPress(IconPtr myIcon,XEvent *myevent)
{
	int found;
	uPoint pt;

	if(!myIcon || !xg.mydisplay)return 1;
	
	//myevent=myevent;

	uPointer(&pt,myIcon);

	CheckControlAndShiftKeys();

	XSetInputFocus(xg.mydisplay,myIcon->myWindow,RevertToPointerRoot,CurrentTime);
	myIcon->topWindow=TRUE;

 	if(PtInRect(&pt,&myIcon->MenuBarList->menuRect)) {
 	    int Menu,Item,n,k;
    	struct uMenuList *menuList;
    	struct uMenuItems *Items;
 
	   if(myIcon->pCheckMenu)
	     (*myIcon->pCheckMenu)(myIcon,TRUE);
				     
	    if(MenuClickList(&pt,myIcon->MenuBarList,&Menu,&Item))return 0;
	    
			menuList=myIcon->MenuList;
	        if(!menuList)return 1;
            n=0;
            while((Items=menuList[n++].MenuItems) != NULL){
				  if(!Items->ItemName)return 1;
               
                  if((n-1) != Menu){
                     continue;
                  }
                  Menu=Items->ItemNumber;
                   k=0;
                   found = FALSE;
                   while(++Items && Items->ItemName){
                       if(k++ == Item){
                           Item=Items->ItemNumber;
                   		  found = TRUE;
                          break;
                       }
                  
                   }
                   if(!found)++Item;
				   if(myIcon->pMenu)
				     (*myIcon->pMenu)(myIcon,Menu,Item);
                   return 0;
            }       
	    return 0;
	}else{
			if(myIcon->pInContent){		
		    return (*myIcon->pInContent)(myIcon);
		}
	    if(myIcon->pdoButtonPress2){
	        (*myIcon->pdoButtonPress2)(myIcon,myevent);
	    }
	}
	return 0;
	
}

static int doExpose(IconPtr myIcon,XEvent *myevent)
{
	Window  mywindow;
	XEvent event;

	if(!myIcon || !xg.mydisplay)return 1;

	mywindow=myIcon->myWindow;
	
	if(myevent->xexpose.window == mywindow){
	    if(myevent->xexpose.count != 0)return 0;

	    if(myIcon->pUpdateWrite)
	        (*myIcon->pUpdateWrite)(myIcon);
	        
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
int isKeyBoardCommand(struct Icon *myIcon,int commandKey)
{
 	struct MenuBar *MenuBarList;
    struct uMenuList *menuList;
    struct uMenuItems *Items;
    struct Menu *pMenu;
    char *cp,buff[256];
    long count,n;
    int Item,Menu;
    int nn,k;
    
	if(!myIcon)return  0;
	menuList=myIcon->MenuList;
	if(!menuList)return 0;
	MenuBarList=myIcon->MenuBarList;	
	if(!MenuBarList)return 0;
	
	for(nn=0;nn<MenuBarList->menuCount;++nn){
	    Menu=nn;
		pMenu=MenuBarList->menuList[nn];  
		count=MenuCount(pMenu);
		if(count > 0){
			for(n=0;n<count;++n){
				if(!MenuGetName((int)n,buff,pMenu)){
					cp=strrchr(buff,'/');
					if(cp){
					    ++cp;
					    if(commandKey == *cp){
					        Item=(int)n;
					        goto FoundIt;
					    }
				  	}
				}
			}
		}
	}		
	return 0;
FoundIt:


    n=0;
    while((Items=menuList[n++].MenuItems) != NULL){
		  if(!Items->ItemName)return 0;
       
          if((n-1) != Menu){
             continue;
          }
          Menu=Items->ItemNumber;
           k=0;
           while(++Items && Items->ItemName){
               if(k++ == Item){
                   Item=Items->ItemNumber;
                  break;
               }
          
           }
		   if(myIcon->pMenu)
		     (*myIcon->pMenu)(myIcon,Menu,Item);
           return 1;
    }       



	return 0;
}
int uSetWindow(struct Icon *myIcon,...)
{
    struct uMenuList *menuList;
    struct uMenuItems *Items;
 	struct MenuBar *MenuBarList;
    struct Menu **pMenu;
	uRect controlRect;
    /* long menudata; */
    char buff[256];
    unsigned char *pal2;
    char *name2;
    int command;
    char *name;
    va_list ap;
    int getMenu;
    int n,nn;
    long menudata;
    int fmenu,fitem;
    int *ret;
    
	if(!myIcon)return 1;
	
	va_start(ap,myIcon);
	while((command=va_arg(ap,int)) != uWindowSetDone){
	    switch(command){
           case uWindowSetWindowName:
				name=va_arg(ap,char *);
				if(!name)return 1;
				SetWindowName(myIcon,name);	
           break;
           case uWindowGetMenu:
               getMenu=va_arg(ap, int);
               pMenu=va_arg(ap, struct Menu **);
	           menuList=myIcon->MenuList;
	           if(!menuList)return 1;
			   MenuBarList=myIcon->MenuBarList;	
	           if(!MenuBarList)return 1;
               n=0;
               while((Items=menuList[n++].MenuItems) != NULL){
				  if(!Items->ItemName)return 1;
               
                  if(Items->ItemID != getMenu){
                     continue;
                  }
                  
                  for(nn=0;nn<MenuBarList->menuCount;++nn){
                      if(MenuBarGetName(nn,buff,MenuBarList))continue;
                      if(!strcmp(Items->ItemName,buff)){
                  		  *pMenu=MenuBarList->menuList[nn];    
                  		  return 0;                 
                      }
                  }
                  return 1;
                  
               }
               return 1;     	
           case uWindowSetMenuList:
               	menuList=va_arg(ap, struct uMenuList *);
               	if(!menuList)return 1;
			   	controlRect=myIcon->viewRect;
				controlRect.y=0;
				controlRect.x=0;
				controlRect.ysize=25;
				MenuBarList=MenuBarNew(&controlRect,myIcon->myWindow);
		   
			   
               	n=0;
               	while((Items=menuList[n++].MenuItems) != NULL){
                   struct Menu *nMenu;
                   if(Items->ItemName){
                       mstrncpy(&buff[1],Items->ItemName,255);
                       buff[0]=(char)strlen(Items->ItemName);
                       Items->ItemID=Items->ItemNumber;
                       nMenu=uMenuBarAddMenus(Items->ItemName,&Items[1],MenuBarList);
                       if(!nMenu)continue;  
                       if(Items->ItemType == uWindowStandardMenu){                   
                           	;
                       }else if(Items->ItemType == uWindowSubMenu){
                            ;
                       }else if(Items->ItemType == uWindowFontMenu){
                            for(nn=0;nn<myIcon->pSysFontList.count;++nn){
                            	mstrncpy(buff,myIcon->pSysFontList.name[nn],255);
								if(MenuAdd(buff+1,MENU_SIMPLE,nMenu))break;
	                          }
                       }else if(Items->ItemType == uWindowPaletteMenu){
                       
			 	         nn=0;
				         while(!getUniversalPaletteItem(nn,&name2,&pal2)){
							if(MenuAdd(name2,MENU_PALETTE_NAMED,nMenu))break;
							if(MenuSetPalette((char *)pal2,PaletteStart+nn,nMenu))break;
							++nn;
				         }
                      
                       }                       
                       
                   }
               	}
			   	myIcon->MenuList = menuList;	
			   	myIcon->MenuBarList=MenuBarList;	
			   	
			   	PutChildWindow(myIcon,MenuBarList->menuWindow);

				myIcon->pdoExpose=doExpose;
				myIcon->pdoButtonPress2=myIcon->pdoButtonPress;
				myIcon->pdoButtonPress=doButtonPress;
				MenuBarColors(myIcon->c,MenuBarList);
				myIcon->viewRect.y=ySHIFT;
				XMapRaised (xg.mydisplay, myIcon->myWindow);
           break;
           case uWindowGetMenuAndItem:
/*
               menudata=va_arg(ap, long);
               fmenu=HiWord(menudata);
	           fitem=LoWord(menudata);
	           menuList=myIcon->MenuList;
	           if(!menuList)return 1;
               n=0;
               while((Items=menuList[n].MenuItems) != NULL){
				  if(!Items->ItemName)return 1;
               
                  if(Items->ItemID != fmenu){
                     ++n;
                     continue;
                  }
                  fmenu=Items->ItemNumber;
                   while(++Items && Items->ItemName){
                       if(Items->ItemID == fitem){
                           ret=va_arg(ap,int *);
                           *ret=fmenu;
                           ret=va_arg(ap,int *);
                           *ret=Items->ItemNumber;
                           return 0;
                       }
                  
                   }
               }
               return 1;
*/
           break;
           case uWindowGetRealMenuAndItem:
               menudata=va_arg(ap, long);
               fmenu=(int)HiWord(menudata);
	           fitem=(int)LoWord(menudata);
	           if(fmenu == AppleMenu || fmenu == WindowMenu){
	               goto ReturnValues;
	           }
	           menuList=myIcon->MenuList;
	           if(!menuList)goto ReturnValues;
               n=0;
               while((Items=menuList[n++].MenuItems) != NULL){
				  if(!Items->ItemName)return 1;
               
                  if(Items->ItemID != fmenu){
                     continue;
                  }
                  
				  if(Items->ItemType == uWindowPaletteMenu || Items->ItemType == uWindowSubMenu){
	                  goto ReturnValues;
                  }
                  
                  fmenu=Items->ItemNumber;
                   while(++Items && Items->ItemName){
						if(Items->ItemNumber == fitem){
							fitem=Items->ItemID;
	           	   			goto ReturnValues;
						}
                   }
	           	   goto ReturnValues;
               }
	           goto ReturnValues;
ReturnValues:               
           	   ret=va_arg(ap,int *);
          	   *ret=fmenu;
           	   ret=va_arg(ap,int *);
          	   *ret=fitem;
               return 0;
	    }
	}
	return 0;
}

int SetExpose2(IconPtr myIcon)
{
	if(myIcon && myIcon->myWindow){
	    XEvent event;
	    event.xexpose.window=myIcon->myWindow;
	    event.xexpose.count=0;
	    event.type=Expose;
            XSendEvent(xg.mydisplay,myIcon->myWindow,
                       TRUE,ExposureMask,&event);
	    return 0;
	}
	return 1;
}

int uInvalWindowRect(IconPtr myIcon,uRect *r)
{
	if(!myIcon || !r)return 1;
	
	if(myIcon && myIcon->myWindow){
	    XEvent event;
	    event.xexpose.window=myIcon->myWindow;
	    event.xexpose.count=0;
	    event.xexpose.x=r->x;
	    event.xexpose.y=r->y;
	    event.type=Expose;
	    event.xexpose.width=r->xsize;
	    event.xexpose.height=r->ysize;
            XSendEvent(xg.mydisplay,myIcon->myWindow,
                       TRUE,ExposureMask,&event);
	    return 0;
	}
	
	return 0;
}
int InvalRectMyWindow(IconPtr myIcon)
{
    if(!myIcon)return 1;
    SetExpose2(myIcon);
    return 0;
}
int uScrollListFitRect(uRect *r,struct ScrollList *scroll)
{
	uRect rm;
	
    if(!r || !scroll)return 1;
    
	rm.y = r->y;
	rm.x= r->x;
	rm.ysize = r->ysize;
	rm.xsize = r->xsize;
	
    return ScrollListFitRect(&rm,scroll);
}
IconPtr uCreateIcon(int Type,uRect *r)
{
	IconPtr myIcon;
	uRect rm;
 	controlPtr control;
 	uRect controlRect;
	
	if(!r || !xg.mydisplay)return NULL;
	
	
	rm = *r;
	
	if(rm.xsize > 1920)rm.xsize=1920;
	
	myIcon=CreateIcon(Type,&rm);
	
	if(!myIcon)return myIcon;
	
	GetPaletteByName("Ps",myIcon->red,myIcon->green,myIcon->blue);
	SetDefaultPaletteName(myIcon,"Ps");
	SetOldColorPalette(myIcon);
	
	
	XSelectInput (xg.mydisplay, myIcon->myWindow,
	                ButtonPressMask | KeyPressMask | ExposureMask 
	    	        | ButtonReleaseMask | Button1MotionMask |
	                EnterWindowMask | LeaveWindowMask | PointerMotionMask |
	                StructureNotifyMask | FocusChangeMask | KeymapStateMask);
	
	myIcon->gc = XCreateGC (xg.mydisplay,myIcon->myWindow, 0, 0);
	myIcon->gcFont = XCreateGC (xg.mydisplay,myIcon->myWindow, 0, 0);

	XSetBackground (xg.mydisplay, myIcon->gc, myIcon->c[2]);
	XSetForeground (xg.mydisplay, myIcon->gc, myIcon->c[2]);
	
	XSetFont(xg.mydisplay,myIcon->gc,xg.font13); 
	
	myIcon->Screen.myIcon=myIcon;
	
	myIcon->ScreenRect.myIcon=myIcon;
	
	getPalette(12,myIcon->paletteName,(char *)myIcon->pal);
	
	
	controlRect.y=rm.y+25;
	controlRect.x=rm.x+rm.xsize-16;
	controlRect.ysize=rm.ysize-16-25;
	controlRect.xsize=16;

	control=ControlCreate(myIcon,&controlRect,"controlName",
		  controlTypeVScroll,1,0L,
		  0L,0L,10,0L);

	if(!control)goto ErrorOut;
	myIcon->VControl=control;

	controlRect.y=rm.y+rm.ysize-16;
	controlRect.x=rm.x;
	controlRect.ysize=16;
	controlRect.xsize=rm.xsize-16;

	control=ControlCreate(myIcon,&controlRect,"controlName",
		  controlTypeHScroll,1,0L,
		  0L,0L,20,0L);

	if(!control)goto ErrorOut;
	myIcon->HControl=control;

	XMapRaised (xg.mydisplay, myIcon->myWindow);
	
	FontStart(myIcon->gc);
	
	/*
	XMoveWindow(xg.mydisplay,myIcon->myWindow,rm.x,rm.y);	
	*/
	return myIcon;
ErrorOut:
    return NULL;
}

int uGetMaximumBounds(IconPtr myIcon,uRect *r)
{
	Window root_return;
   	int x_return, y_return;
   	unsigned int width_return, height_return;
   	unsigned int border_width_return;
   	unsigned int depth_return;
	XWindowAttributes win;
	
	if(!myIcon || !r || !xg.mydisplay)return 1;
	
	XGetGeometry(xg.mydisplay,myIcon->myWindow,&root_return,&x_return,
            	&y_return,&width_return,&height_return,&border_width_return,
           		&depth_return);
           		
	XGetWindowAttributes(xg.mydisplay,root_return,&win);
	
/*	
	r->y=win.y;
	r->x=win.x;
*/
	r->y=0;
	r->x=0;
	
	r->ysize=win.height;
	r->xsize=win.width;
	/*
	r->ysize=2000;
	r->xsize=2000;
	*/
	return 0;
}
int uGetPortBounds(IconPtr myIcon,uRect *r)
{
	XWindowAttributes win;
	
	if(!myIcon || !r || !xg.mydisplay)return 1;
	
	XGetWindowAttributes(xg.mydisplay,myIcon->myWindow,&win);
	
/*	
	r->y=win.y;
	r->x=win.x;
*/
	r->y=0;
	r->x=0;
	r->ysize=win.height;
	r->xsize=win.width;
	
	return 0;
}
Window getTopWindow(Window mywindow,int *xoff,int *yoff)
{
	Window root_return;
	Window parent_return;
	Window *children_return;
	unsigned int nchildren_return;
   	int x_return, y_return;
   	unsigned int width_return, height_return;
   	unsigned int border_width_return;
   	unsigned int depth_return;
	Window ret;
	
	
	if(!mywindow)return (Window)NULL;

	parent_return=(Window)NULL;
	children_return=NULL;
    XQueryTree(xg.mydisplay,mywindow, &root_return, &parent_return,&children_return, &nchildren_return);	
	if(children_return){
		XFree(children_return);
		children_return=NULL;
	}

	XGetGeometry(xg.mydisplay,mywindow,&root_return,&x_return,
            	&y_return,&width_return,&height_return,&border_width_return,
           		&depth_return);
	
	if(root_return == parent_return){
	    ret=mywindow;
	}else{	
		*xoff += (x_return+border_width_return);
		*yoff += (y_return+border_width_return);
	    ret=getTopWindow(parent_return,xoff,yoff);
	}
	
	return ret;
}
int uGetPortBoundsGlobal(IconPtr myIcon,uRect *r)
{
   Window root_return1;
   int x_return1, y_return1;
   unsigned int width_return1, height_return1;
   unsigned int border_width_return1;
   unsigned int depth_return1;

   Window root_return;
   int x_return, y_return;
   unsigned int width_return, height_return;
   unsigned int border_width_return;
   unsigned int depth_return;
   
   int xoff,yoff;
   
	Window top_parent;
	
	if(!myIcon || !r || !xg.mydisplay)return 1;
	
	XGetGeometry(xg.mydisplay,myIcon->myWindow,&root_return,&x_return,
            	&y_return,&width_return,&height_return,&border_width_return,
           		&depth_return);

	xoff=0;
	yoff=0;
	top_parent=getTopWindow(myIcon->myWindow,&xoff,&yoff);
	if(top_parent){

		XGetGeometry(xg.mydisplay,top_parent,&root_return1,&x_return1,
            		&y_return1,&width_return1,&height_return1,&border_width_return1,
           			&depth_return1);
      	x_return=x_return1+xoff+border_width_return1;
   		y_return=y_return1+border_width_return1;
	}
	r->x=x_return;
	r->y=y_return;
	r->xsize=width_return;
	r->ysize=height_return;
	
	return 0;
}
int uSetCursor(IconPtr myIcon,int Type)
{
    if(!myIcon || !xg.mydisplay)return 1;
    
    if(Type == uARROW_CURSOR){
        XDefineCursor(xg.mydisplay,myIcon->myWindow,xg.Arrow);
    }else if(Type == uWATCH_CURSOR){
        XDefineCursor(xg.mydisplay,myIcon->myWindow,xg.Watch);
    }else if(Type == uFIDDLE_CURSOR){
        XDefineCursor(xg.mydisplay,myIcon->myWindow,xg.Fiddle);
    }else if(Type == uSELECTION_CURSOR){
        XDefineCursor(xg.mydisplay,myIcon->myWindow,xg.Selector);
    }else if(Type == uBEAM_CURSOR){
        XDefineCursor(xg.mydisplay,myIcon->myWindow,xg.Beam);
    }else if(Type == uPLUS_CURSOR){
        XDefineCursor(xg.mydisplay,myIcon->myWindow,xg.PlusCursor);
    }
    
    
    
    return 0;
}
int uDoControls(IconPtr myIcon)
{
	XEvent *myevent;
	controlPtr control;
	int part;

	if(!myIcon)return 1;

	myevent=myIcon->myevent;
	
	if(!(part=FindControl(myevent,myIcon->myWindow,&control))){
	    /* printf("doControls no part found\n"); */
	    return 1;
	}
	
	switch(part){

		case inButton:
		printf("TrackControl %ld\n",control->controlValue);
		break;

		case inUpButton:
	    	TrackControl(control,myevent,doInUpButton,(void *)myIcon);
		break;

		case inDownButton:
	    	TrackControl(control,myevent,doInDownButton,(void *)myIcon);
		break;

		case inPageUp:
	    	TrackControl(control,myevent,doInPageUp,(void *)myIcon);
		break;

		case inPageDown:
	    	TrackControl(control,myevent,doInPageDown,(void *)myIcon);
		break;

		case inThumb:
	    	TrackControl(control,myevent,doInThumb,(void *)myIcon);
		break;

		default: ;
	}
	return 0;
}
static long doInThumb(controlPtr control,short part,void *controlRoutineData)
{
	IconPtr myIcon=(IconPtr)controlRoutineData;
	
	if(!myIcon || !myIcon->pThumb)return 1;
	
	//part=part;

    (*myIcon->pThumb)(control,myIcon);

	return 0;
}
static long doInDownButton(controlPtr control,short part,void *controlRoutineData)
{
	IconPtr myIcon=(IconPtr)controlRoutineData;

	if(!myIcon || !myIcon->pLineDown)return 1;
	
    (*myIcon->pLineDown)(control,part,myIcon);
    	
	return 0;
}
static long doInUpButton(controlPtr control,short part,void *controlRoutineData)
{
	IconPtr myIcon=(IconPtr)controlRoutineData;

	if(!myIcon || !myIcon->pLineUp)return 1;

    (*myIcon->pLineUp)(control,part,myIcon);
	
	return 0;
}
static long doInPageDown(controlPtr control,short part,void *controlRoutineData)
{
	IconPtr myIcon=(IconPtr)controlRoutineData;

	if(!myIcon || !myIcon->pPageDown)return 1;

    (*myIcon->pPageDown)(control,part,myIcon);
	
	return 0;
}
static long doInPageUp(controlPtr control,short part,void *controlRoutineData)
{
	IconPtr myIcon=(IconPtr)controlRoutineData;

	if(!myIcon || !myIcon->pPageUp)return 1;

    (*myIcon->pPageUp)(control,part,myIcon);
	
	return 0;
}
