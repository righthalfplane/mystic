#define EXTERN  extern
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

#include <shlobj.h>
#include <objbase.h>

#include <stdarg.h>
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "fileDialog.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "Linedata.h"
#include "BatchFile.h"
#include "Sagevol.h"
#include "Spread.h"
#include "ScrollList.h"
#include "Universal.h"
#include "uGraph.h"
#include "uAttributes.h"
#include "VoiceCommand.h"

#include "mThread.h"


/* CHANGE X-WINDOW TYPES TO SOMETHING COMPATIBLE WITH WINDOWS FOR NOW */
#define Visual void
#define XColor void
#define GC int
#define XImage void
#define XEvent void

#define uWindowGetMenuAndItem2 9999
int uCStringWidth(char *name,IconPtr myIcon);

int doGetPalette(IconPtr myIcon);
int doSavePalette(IconPtr myIcon);

IconPtr  macUtilities(IconPtr myIcon);
IconPtr  NewTableWindow(IconPtr myIcon);

#define ySHIFT	25

int Sleep2(int ms);

static int CDrawdoTextOther(IconPtr myIcon);

int uSetTextFontAndSize(struct Icon *myIcon,int font,int size,int style);

int uLoadCursor(IconPtr myIcon);

int LoadToMenu(IconPtr myIcon);
int SetDefaultPaletteName(IconPtr myIcon,char *name1);
int PointerM(xPoint *pt,IconPtr myIcon);
int FrameRectM(uRect *oldR,IconPtr myIcon);
int doPaletteToTable(IconPtr myIcon);
int WriteRectToRect2(unsigned char *pal, unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize);
int BatchFile(void);
int Write24ToScreen(unsigned char *bin,unsigned char *bout,int xsize,int ysize,XColor *colors);

int SetExpose2(IconPtr myIcon);

int getPalColors(XColor *colors,unsigned char *pal);

int WriteHDFToScreen(struct DisplayF *d,unsigned char *data);

struct Menu *uMenuBarAddMenus(char *MenuName,
                             struct uMenuItems *Items,
                             struct MenuBar *MenuBarList);
                             
static int doExpose(IconPtr myIcon,XEvent *myevent);

static int SetDefaultFont(IconPtr myIcon);

int getPalette(int n,char *name,char *pal);

controlPtr testForControlHandle(IconPtr myIcon,long controlID,struct ScrollList **ListD);

IconPtr FindMyFrontWindow();

int flip2(unsigned char *b,int xsize,int ysize);
unsigned char *BlockSave(unsigned char *buffin,long length);

static long doInPageUp(controlPtr control,short part,void *controlRoutineData);
static long doInPageDown(controlPtr control,short part,void *controlRoutineData);
static long doInUpButton(controlPtr control,short part,void *controlRoutineData);
static long doInDownButton(controlPtr control,short part,void *controlRoutineData);
static long doInThumb(controlPtr control,short part,void *controlRoutineData);
int FontStart(IconPtr IconPtr,HFONT hfont);
int atoh(char *s,unsigned int *ret);
int IsControl(void);
int testForControl(IconPtr myIcon,long controlID,int command);
int DoControlDialog(IconPtr myIcon,long controlID);

static char *scrap;

IconPtr NewEdit(struct FileList *ret);

IconPtr NewDraWindow(IconPtr myIcon);

int NewRastor(long count);

IconPtr openRender(struct Scene *scene);

IconPtr NewRenderWindow(IconPtr myIcon);

int WriteToWindowFast(char *name,char *buff);

int WriteToWindowAtEnd(char *name,char *buff);

int CreateNewDirectory(void);

int domkdir(char *name);

IconPtr  NewNetWindow(IconPtr myIcon);

static int xold,yold;

int WriteRectToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
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
IconPtr FindMyIcon(IconPtr w);

IconPtr NewFileSendWindow(IconPtr myIconi);

int WriteToWindow(char *name,char *buff);

struct uMenuList *CopyMenuList(struct uMenuList *menuList);

int pDoStyleMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);
int pDoFontMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);

int StringWidth(unsigned char *,struct Icon *myIcon);

struct InstalledList{
     int item;
     int isActive;
};

int doAddPalette(IconPtr myIcon,char *name);

int GetSpecialDirectory(char *dir);
char *strpathname(char *path,char *name);
IconPtr NewCDraw(IconPtr myIcon);

int uOpenFile(void);

struct FileList *FileDialog3(char *name);

int OpenViewLib(struct FileList *Files);

struct FileList *uOpenFileDialogLocal(struct Icon *myIcon,int *TypeList);

static int sScreenSetSize(long xsizei,long ysizei,struct ScreenInfo *s,struct Icon *myIcon);

static int sScreenWrite(long xsize,long ysize,unsigned char *data,int DataType,
                uRect *SRect,uRect *DRect,struct ScreenInfo *s,struct Icon *myIcon);


	static int points[]={9,10,12,14,24,36,42,48,54,72};

int startLanguage(struct VoiceCommands *Language,long Count,IconPtr myIconSpeech)
{
	Language=Language;
	Count=Count;
	myIconSpeech=myIconSpeech;
	return 0;
}
int stopLanguage(void)
{
	return 0;
}
int uCStringWidth(char *name,IconPtr myIcon)
{
	SIZE size;

	if(!name || !myIcon)return 1;

	GetTextExtentPoint32(myIcon->hdc,name,strlen(name),&size);

	return size.cx;
}


int uGetFontInfo(struct Icon *myIcon,uFontInfo *t)
{
	char buff[2];
	TEXTMETRIC tm;
	SIZE size;

	if(!t || !myIcon)return 1;

	GetTextMetrics(myIcon->hdc,&tm);
	buff[0]='0';
	buff[1]=0;
	
	GetTextExtentPoint32(myIcon->hdc,buff,strlen(buff),&size);
	
	t->CWidth = size.cx;
	t->LineHeight = size.cy+tm.tmDescent;
	t->Descent = 0;

	return 0;
}

int uSetFontSize(struct Icon *myIcon,int size)
{
	if(!myIcon)return 1;


	uSetTextFontAndSize(myIcon,myIcon->pSysFontList.lastFont,size,myIcon->pSysFontList.lastFace);

	return 0;
}
int uSetTextFont(struct Icon *myIcon,int font)
{
	if(!myIcon)return 1;

	uSetTextFontAndSize(myIcon,font,myIcon->pSysFontList.lastSize,myIcon->pSysFontList.lastFace);

	return 0;
}
int uSetTextFontAndSize(struct Icon *myIcon,int font,int size,int face)
{
	HFONT hfont;

	if(!myIcon)return 1;

	if(size <= 0)size=8;

	if(font == myIcon->pSysFontList.lastFont && 
	   size == myIcon->pSysFontList.lastSize &&
	   face == myIcon->pSysFontList.lastFace)return 0;

	if(!myIcon->pSysFontList.name)return 0;

	if(--font < 0 || font >= myIcon->pSysFontList.count)font=0;

	myIcon->hfont=CreateFont(size,0,0,0,
							 (face & FONTSTYLE_BOLD) ? FW_BOLD : FW_NORMAL,
							 (face & FONTSTYLE_ITALIC),
							 (face & FONTSTYPE_UNDERLINE),
							 (face & FONTSTYPE_STRIKEOUT),
							 ANSI_CHARSET,
							 OUT_DEFAULT_PRECIS,
							 CLIP_DEFAULT_PRECIS,
							 PROOF_QUALITY,
							 DEFAULT_PITCH | FF_MODERN,
							 myIcon->pSysFontList.name[font]);
	hfont=SelectObject(myIcon->hdc,myIcon->hfont);
	if (hfont) {
		DeleteObject(hfont);
	}
	myIcon->pSysFontList.lastFont=font+1;
	myIcon->pSysFontList.lastSize=size;
	myIcon->pSysFontList.lastFace=face;
/*
	sprintf(WarningBuff,"font %s %d size %d\n",myIcon->pSysFontList.name[font],font+1,size);
	if(myIcon->WindowType != ExpressionType)WarningBatch(WarningBuff);
*/
	return 0;
}
int pDoFontMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item)
{
	if(!myIcon || !myIcon->pSysFontList.name)return 1;

	FontMenuNumber=FontMenuNumber;
	StyleMenuNumber=StyleMenuNumber;

	uSetTextFont(myIcon,Item);

	return 0;
}


int pDoStyleMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item)
{
	int font,size,face;
	if(!myIcon)return 1;
	FontMenuNumber=FontMenuNumber;
	StyleMenuNumber=StyleMenuNumber;

	font=myIcon->pSysFontList.lastFont;
	face=myIcon->pSysFontList.lastFace;
	size=myIcon->pSysFontList.lastSize;
	switch(Item){
		case 1:	
		uSetTextFontAndSize(myIcon,font,size,0);
		return 0;
		case 2:	
				uSetTextFontAndSize(myIcon,font,size,
			           face ^ FONTSTYLE_BOLD);
		return 0;
		case 3:	
		uSetTextFontAndSize(myIcon,font,size,
			          face ^ FONTSTYLE_ITALIC);
		return 0;
		case 4:	
		uSetTextFontAndSize(myIcon,font,size,
			          face ^ FONTSTYPE_UNDERLINE);
		return 0;
		case 5:	
		uSetTextFontAndSize(myIcon,font,size,
			          face ^ FONTSTYPE_STRIKEOUT);
		return 0;
	}

	Item -= 8;

	if(Item >= 0 && Item <= 9){
		uSetFontSize(myIcon,points[Item]);
	}else if(Item == 10){
		CDrawdoTextOther(myIcon);
	}


	return 0;
}
static int CDrawdoTextOther(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {160,90,80,30},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {55,90,80,30},uDialogButton,uDialogNone},
      /*  2 */      {"15", {155,50,50,20},uDialogText,uDialogGetLong},
      /*  4 */      {"Points", {55,50,50,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {40,40,280,200};
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	
	static long int list=15;
	
	if(!myIcon)return 1;

	if(myIcon->pSysFontList.lastSize > 0)list=myIcon->pSysFontList.lastSize;
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Special Font Size",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,	
	              uDialogSetDouble,(double)list,
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	list=uR[2].lreturn;
		
	
	uSetFontSize(myIcon,list);

	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
	
}
int uSetTextFace(struct Icon *myIcon,int face)
{
	if(!myIcon)return 1;

	uSetTextFontAndSize(myIcon,myIcon->pSysFontList.lastFont,myIcon->pSysFontList.lastSize,face);
	return 0;
}

int uGetFontStuff(struct uFontStuff *f,IconPtr myIcon)
{
	
	if(!f || !myIcon)return 1;

	f->txFont = myIcon->pSysFontList.lastFont;
	f->txSize = myIcon->pSysFontList.lastSize;
	f->txFace = myIcon->pSysFontList.lastFace;

	return 0;
}
int uSetFontStuff(struct uFontStuff *f,IconPtr myIcon)
{

	if(!f || !myIcon)return 1;

	uSetTextFontAndSize(myIcon,f->txFont,f->txSize,f->txFace);

	return 0;
}
int uSetFontMenus(int FontMenuNumber,int StyleMenuNumber,IconPtr myIcon)
{
	int font,size,face;
	int check;
	int n;

	if(!myIcon)return 1;
	if(!myIcon->pSysFontList.name)return 1;
	FontMenuNumber=FontMenuNumber;
	StyleMenuNumber=StyleMenuNumber;
	font = myIcon->pSysFontList.lastFont;
	size = myIcon->pSysFontList.lastSize;
	face = myIcon->pSysFontList.lastFace;

	for(n=0;n<myIcon->pSysFontList.count;++n){
		uCheckMenuItem(myIcon,FontMenuNumber,n+1,(font == n+1));
	}


	uCheckMenuItem(myIcon,StyleMenuNumber,1,(face == FONTSTYLE_PLAIN));
	uCheckMenuItem(myIcon,StyleMenuNumber,2,(face & FONTSTYLE_BOLD));
	uCheckMenuItem(myIcon,StyleMenuNumber,3,(face & FONTSTYLE_ITALIC));
	uCheckMenuItem(myIcon,StyleMenuNumber,4,(face & FONTSTYPE_UNDERLINE));
	uCheckMenuItem(myIcon,StyleMenuNumber,5,(face & FONTSTYPE_STRIKEOUT));

	uActivateMenuItem(myIcon,StyleMenuNumber,6,FALSE);

	check=FALSE;
	for(n=0;n<10;++n){
		uCheckMenuItem(myIcon,StyleMenuNumber,n+8,(size == points[n]));
		if(size == points[n])check=TRUE;
	}

	if(check){
		uCheckMenuItem(myIcon,StyleMenuNumber,10+8,FALSE);
	}else{
		uCheckMenuItem(myIcon,StyleMenuNumber,10+8,TRUE);
	}


	return 0;
}
int uDrawImageRect(iRect *r,IconPtr myIcon)
{
	struct ScreenInfo *s;
	uRect SRect,DRect;
	if(!r || !myIcon)return 1;


	s=&myIcon->ScreenRect;

	sScreenSetSize(r->box.xsize,r->box.ysize,s,myIcon);

	DRect=r->box;
	SRect=DRect;
	SRect.x=0;
	SRect.y=0;

	return sScreenWrite(DRect.xsize,DRect.ysize,r->data,r->type,
                        &SRect,&DRect,s,myIcon);
}
static int sScreenWrite(long xsize,long ysize,unsigned char *data,int DataType,
                uRect *SRect,uRect *DRect,struct ScreenInfo *s,struct Icon *myIcon)
{
	/* int n; */
	
	if(!SRect || !DRect || !myIcon || !data)return 1;
		
	s->DataType=DataType;

	s->xsize=0;
	s->ysize=0;
	
	if(DataType == ScreenDataType8){
		WriteRectToRect2(myIcon->pal,data,SRect,xsize,ysize,s->buffer,SRect,
			             s->ibitmap.bmiHeader.biWidth,s->ysizei);

/*
		
			for(n=0;n<s->length/3;++n){
			s->buff[3*n]=0;
			s->buff[n*3+1]=255;
			s->buff[n*3+2]=0;
		}
*/
		
		if(!BitBlt(myIcon->hdc,DRect->x,DRect->y,DRect->xsize,
			DRect->ysize,(s->bitdc),0,0,SRCCOPY)) {
		}
	}else if(DataType == ScreenDataType24){
		

		WriteRectToRect24(data,SRect,xsize,ysize,s->buffer,SRect,
			             s->ibitmap.bmiHeader.biWidth,s->ysizei);

		/*
			for(n=0;n<s->length/3;++n){
			s->buff[3*n]=0;
			s->buff[n*3+1]=255;
			s->buff[n*3+2]=0;
		}
		*/
		
		if(!BitBlt(myIcon->hdc,DRect->x,DRect->y,DRect->xsize,
			DRect->ysize,(s->bitdc),0,0,SRCCOPY)) {
		}
	}	
	return 0;
}

struct FileList *uOpenFileDialogLocal(struct Icon *myIcon,int *TypeList)
{
    static struct FileListInfo infoLoad;
	struct FileList *Filesl;
    struct FileInfo2 *Files;
	int n;

	if(!myIcon){
		Filesl=FileDialog3("File Selection Dialog");
	}else{		
		Filesl=FileDialog3("File Selection Dialog");
	}		
	if(!Filesl)return NULL;

	if(FilesSetType(Filesl)){
		FileListFree(Filesl);
	    return NULL;
	}
	
	if(TypeList){
	    n=0;
	    while(TypeList[n]){
	        if((TypeList[n] == OPEN_ALL_FILE_TYPES) || (Filesl->type == TypeList[n++])){
	            return Filesl;
	        }
	    }
	}
	
	
	if(Filesl->type == VIEWLIB_MEMORY){
	    OpenViewLib(Filesl);
	}else{
		Files=FilesOpenList(Filesl);
	    if(Files){
	        if(FilesOpenDefault(Files)){
	            return NULL;
	        }
	    }
    }
	    
	return NULL;
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
IconPtr NewCDraw(IconPtr myIcon)
{
	return NewDrawWindow(myIcon);
}

char *getUniversalPath(void)
{
	char dir[MAX_PATH]; /* Windows path-size requirement */
	char path[2048];
	char *home;
	FILE *in;
	
	GetSpecialDirectory(dir);
	
    home=strsave(dir,7865);
	if(home){
	    home=strpathname(home,".showPreferenceName");
	    if(!home)return home;
	    in=fopen(home,"rb");
	    if(!in){
	        sprintf(WarningBuff,"getUniversalPath Could Not Open '%s' To Read\n",home);
	        WarningBatch(WarningBuff);
	        cFree(home);
	        home=NULL;
	    }else{	    
	    	fgets(path,2048,in);
	        cFree(home);
			home=strsave(path,8723);	    	
	    }
	    
	}
	return home;
}
int saveUniversalPath(char *path)
{
	char dir[MAX_PATH]; /* Windows path-size requirement */
	char *home;
	FILE *out;
	int ret;
	
	if(!path)return 1;
	
	ret=1;
	
	out=NULL;
	
	GetSpecialDirectory(dir);
	
    home=strpathname(dir,".showPreferenceName");
    if(!home)goto ErrorOut;
    
    out=fopen(home,"wb");
    if(!out){
        sprintf(WarningBuff,"saveUniversalPath Could Not Open '%s' To Write\n",home);
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }else{	    
    	if(fputs(path,out)){
        	sprintf(WarningBuff,"saveUniversalPath Error Writting '%s'\n",home);
        	WarningBatch(WarningBuff);
        	goto ErrorOut;
    	}
    }
	    

	ret=0;
	
ErrorOut:	

	if(home)cFree(home);
	if(out)fclose(out);

	return ret;
}

int GetSpecialDirectory(char *dir)
{
  LPITEMIDLIST ppidl;

  if (!dir) return 0;

/* Return path to C:\Documents and Settings\user\My Documents */
/*  if(SHGetSpecialFolderLocation(NULL,CSIDL_PERSONAL,&ppidl)!=NOERROR)
return 0; */

/* Return path to C:\Documents and Settings\user\Application Data */
  if(SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&ppidl)!=NOERROR)
		return 0;

  if (!ppidl) return 0;

  SHGetPathFromIDList(ppidl,dir);

  CoTaskMemFree(ppidl);

  return 1;
}
/*
void CWindowSetPalMenu(IconPtr myIcon)
{
	if(!myIcon)return;
}
*/
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

	
	mCount=uCountMenuItems(Pal);
	for(k=0;k<mCount;++k){
	    uDeleteMenuItem(Pal,1);
	}
	
	sortUniversalPalette();
	
 	uInsertMenuItem(Pal,"Load Palette...     ",500);
	uInsertMenuItem(Pal,"Save Palette...     ",500);
 	uInsertMenuItem(Pal,"Add Palette...      ",500);
	uInsertMenuItem(Pal,"Restore Palette   /R",500);
 	uInsertMenuItem(Pal,"Load Menu Palette...",500);
 	uInsertMenuItem(Pal,"Sychronize Palettes ",500);
 	uInsertMenuItem(Pal,"Palette To Table    ",500);
	uInsertMenuItem(Pal,"--------------------",500);


	n=0;
    while(!getUniversalPaletteItem(n++,&name,&pal)){
        uInsertMenuItem(Pal,name,500);
    }

	SetDefaultPaletteName(myIcon,myIcon->DefaultPaletteName);

OutOfHere:
	
	return;
}

int isInstalled(int item)
{
	struct InstalledList list[]= {
			{Installed_PrintPreview,FALSE},
			{Installed_PrintSetup,TRUE},
			{Installed_Print,TRUE},
			{Installed_RasterPreferences,TRUE},
			{Installed_CopyImages,TRUE},
			{Installed_PIO2DPreferences,TRUE},
			{Installed_PIO3DPreferences,TRUE},
			{Installed_TekPreferences,TRUE},			
			{Installed_LinePlots,TRUE},			
			{Installed_PointPlots,TRUE},			
			{Installed_DrawProgram,TRUE},			
			{Installed_LocalOpenDialog,TRUE},			
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

int uSetPenColor(int r,int g,int b,struct Icon *myIcon)
{
	
	if(!myIcon)return 1;

	myIcon->lineRed=r;
	myIcon->lineGreen=g;
	myIcon->lineBlue=b;
		    
	return 0;
}

int uSetPenXOR(int flag,struct Icon *myIcon)
{
	if(!myIcon)return 1;
	
	if(flag){
		SetROP2(myIcon->hdc,R2_NOTXORPEN);
	}else{
		SetROP2(myIcon->hdc,R2_COPYPEN);
	}

	return 0;
}

int StringWidth(unsigned char *buff,struct Icon *myIcon)
{
	uFontInfo Info;
	    
	if(!buff || !myIcon)return 1;

	uGetFontInfo(myIcon,&Info);
	
	
	return (int)strlen((char *)buff)*Info.CWidth;
}

int PointerM(xPoint *pt,IconPtr myIcon)
{
	POINT p,p1;

	if(!pt || !myIcon)return 1;

	GetCursorPos(&p);
	GetDCOrgEx(myIcon->hdc,&p1);
	pt->x=p.x-p1.x;
	pt->y=p.y-p1.y;
	return 0;
}

int FrameRectM(uRect *oldR,IconPtr myIcon)
{

	if(!oldR || !myIcon)return 1;

	MoveToM(oldR->x,oldR->y);
	LineToM(oldR->x,oldR->y+oldR->ysize-1,myIcon->myWindow,myIcon->hdc);
	LineToM(oldR->x+oldR->xsize-1,oldR->y+oldR->ysize-1,myIcon->myWindow,myIcon->hdc);
	LineToM(oldR->x+oldR->xsize-1,oldR->y,myIcon->myWindow,myIcon->hdc);
	LineToM(oldR->x,oldR->y,myIcon->myWindow,myIcon->hdc);

	return 0;
}

int WarningBatch(char *WarningBuff)
{
    return  WriteToWindow("BatchPrint",WarningBuff);    
}
int WarningBatchFast(char *WarningBuff)
{
    return  WriteToWindowFast("BatchPrint",WarningBuff);    
}
int WarningBatchAtEnd(char *WarningBuff)
{
    return  WriteToWindowAtEnd("BatchPrint",WarningBuff);    
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
	MSG msg;
	int ret;

	SetCapture(myIcon->myWindow);

	ret=!PeekMessage(&msg,myIcon->myWindow,WM_LBUTTONUP,WM_LBUTTONUP,PM_NOREMOVE);

	ReleaseCapture();

	return ret;
}
/*
int runThreads(long ThreadCount,void *data,long numsteps,int (*sageThread)(mThread *Thread))
{
    mThread Thread;
        
	if(!data || !sageThread)return 1;
	ThreadCount=ThreadCount;
	zerol((char *)&Thread,sizeof(Thread));
    Thread.smin=0;
    Thread.smax=numsteps;
    Thread.data=data;
    return (*sageThread)(&Thread);
}
*/
int (*sageThread)(void *data);

unsigned __stdcall  ThreadFunc( LPVOID lpParam ) 
{ 

	if(sageThread)(*sageThread)(lpParam);

	_endthreadex( 0 );

	return 0; 
} 
int runThreads(long ThreadCount,void *data,long numsteps,int (*sageThreadi)(mThread *Threads))
{
	mThread Threads;
	mThread *myThreads;
	HANDLE hThread; 
	/* extern HANDLE hPrintMutex; */

	int nt,threads;
	long tstep,tstart,tend;
	int done;

	if(!sageThreadi)return 1;

	/* hPrintMutex = NULL; */

	sageThread=sageThreadi;

	threads=ThreadCount;

	myThreads=NULL;
	if(threads <= 0){
	    zerol((char *)&Threads,sizeof(Threads));
	    Threads.smin=0;
	    Threads.smax=numsteps;
		Threads.data=data;
	    return (*sageThread)(&Threads);
	}else{

		/* hPrintMutex = CreateMutex( NULL, FALSE, NULL ); did not help */ 

	    myThreads=(mThread *)cMalloc(threads*sizeof(Threads),9020);
	    if(!myThreads){
			sprintf(WarningBuff,"runThreads Out of memory Requested (%ld)\n",(long)(threads*sizeof(Threads)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)myThreads,threads*sizeof(Threads));
	    tstep=numsteps/(threads+1);
	    if(tstep <= 0){
	        tstep = 1;
			threads = (int)numsteps-1;
	    }

	    tstart=0;
	    for(nt=0;nt<threads;++nt){
			tend=tstart+tstep;
	        myThreads[nt].data= data;
			myThreads[nt].done = FALSE;
			myThreads[nt].smin=tstart;
			myThreads[nt].nthread=nt;
			myThreads[nt].smax=tend;

			{
					DWORD dwThreadId;
	
					sageThread=sageThreadi;

					hThread = (HANDLE)_beginthreadex(  
						NULL,                       
						64000000,                          
						&ThreadFunc,  
						(void *)&myThreads[nt],					
						CREATE_SUSPENDED,                         
						&dwThreadId);               

					myThreads[nt].save=hThread;
					if (hThread == NULL){
						WarningBatch("CreateThread Failed\n");
					}else{
						ResumeThread(hThread);
					}



			}

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
				if(myThreads[nt].done){
					if(myThreads[nt].save){
						hThread=myThreads[nt].save;
						myThreads[nt].save=NULL;
						CloseHandle( hThread );
					}
				}else{
					done=FALSE;
				}
	        }
	        if(done)break;
	        Sleep2(50);
	    }

	}
End:
	if(myThreads)cFree((char *)myThreads);
	/* if(hPrintMutex)CloseHandle( hPrintMutex ); */
	return 0;
}


int launchThread(void *data,int (*sageThreadi)(void *data))
{
    HANDLE hThread; 
	DWORD dwThreadId;

	if(!sageThreadi)return 1;
	
	sageThread=sageThreadi;

    hThread = (HANDLE)_beginthreadex( 
        NULL,                       
        0,                          
        &ThreadFunc,  
        data,					
        0,                         
        &dwThreadId);               

	if (hThread == NULL){
		WarningBatch("CreateThread Failed\n");
	}else{
		CloseHandle( hThread );
	}

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
	
	if(!myIcon || !name)return 1;
	

	GetWindowText(myIcon->myWindow,name2,256);

	/* mstrncpy(name2,myIcon->WindowName,256); */

	if(flag){
		cp=strrchr(name2,'[');
		if(cp){
			*cp=0;
		}
	}

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
IconPtr  ListWindow(int XCount,int YCount,int XLength,int YLength)
{
	Warning("ListWindow Not Yet Active");
	XCount=XCount;
	YCount=YCount;
	XLength=XLength;
	YLength=YLength;
	return NULL;
}
int uClipRect(uRect *r,struct Icon *myIcon)
{
	SetRectRgn(myIcon->region, r->x, r->y, r->x + r->xsize, r->y + r->ysize);
	SelectClipRgn(myIcon->hdc,myIcon->region);
	return 0;
}
int uEraseRect(uRect *r,struct Icon *myIcon)
{	
	RECT xr;
	HBRUSH b;
	
	if(!myIcon || !r)return 1;
	
	xr.left=r->x;
	xr.top=r->y;	
	xr.right=r->x + r->xsize;
	xr.bottom=r->y + r->ysize;

	b=CreateSolidBrush(RGB(255,255,255));
	FillRect(myIcon->hdc,&xr,b);
	DeleteObject(b);

	return 0;
}
int uFillRect(uRect *r,struct Icon *myIcon)
{	
	RECT xr;
	
	if(!myIcon || !r)return 1;
	
	xr.left=r->x;
	xr.top=r->y;	
	xr.right=r->x + r->xsize;
	xr.bottom=r->y + r->ysize;

	if(!myIcon->b)myIcon->b=CreateSolidBrush(RGB(255,255,255));
	FillRect(myIcon->hdc,&xr,myIcon->b);

	return 0;
}
int uFrameRect(uRect *r,struct Icon *myIcon)
{
	
	if(!myIcon || !r)return 1;
	
	FrameRectM(r,myIcon);

	return 0;
}
int uInvertRect(uRect *r,struct Icon *myIcon)
{
	RECT xr;
	
	if(!myIcon || !r)return 1;
	
	xr.left=r->x;
	xr.top=r->y;	
	xr.right=r->x + r->xsize;
	xr.bottom=r->y + r->ysize;
	InvertRect(myIcon->hdc,&xr);
	return 0;
}
int uInsertMenuItem(menuPtr menu,char *name,int location)
{
	controlPtr c;
	int count;

	c=FindAnyWindowControl((controlPtr)menu);
	if(c){
	    return uControlInsertMenuItem(c,name,location);
	}

	count=GetMenuItemCount(menu);

	InsertMenu(menu,location,MF_BYPOSITION | MFT_STRING,count+1,name);

	return 0;
}

long uCountMenuItems(menuPtr menu)
{
	controlPtr c;
	long count;

	if(!menu)return -1l;


	c=FindAnyWindowControl((controlPtr)menu);
	if(c){
	    return uControlCountMenuItems(c);
	}

	
	count=GetMenuItemCount(menu);


	return count;
}

int uDeleteMenuItem(menuPtr menu,int item)
{
	controlPtr c;

	if(!menu)return 1;

	c=FindAnyWindowControl((controlPtr)menu);
	if(c){
	    return uControlDeleteMenuItem(c,item-1);
	}

	DeleteMenu(menu,item-1,MF_BYPOSITION);

	return 0;
}

int uDisableMenuItem(struct Icon *myIcon,int menu,int item)
{
	menuPtr Menu;
	int menudata;
	
	if(!myIcon)return 1;


	menudata = ((unsigned long)menu << 16) | item;
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
			              uWindowSetDone,uWindowSetDone)){
		menu=HIWORD(menudata);
		item=LOWORD(menudata);
	}
				
	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;

   	EnableMenuItem(Menu,item-1,MF_GRAYED | MF_BYPOSITION);

	return 0;
}
int uEnableMenuItem(struct Icon *myIcon,int menu,int item)
{
	menuPtr Menu;
	int menudata;
	
	if(!myIcon)return 1;


	menudata = ((unsigned long)menu << 16) | item;
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
			              uWindowSetDone,uWindowSetDone)){
		menu=HIWORD(menudata);
		item=LOWORD(menudata);
	}
				
	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;

   	EnableMenuItem(Menu,item-1,MF_ENABLED | MF_BYPOSITION);

	return 0;
}
int uSetMenuItemText(struct Icon *myIcon,int menu,int item,unsigned char *name)
{
	menuPtr Menu;
	int menudata;
	
	if(!myIcon || !name)return 1;


	menudata = ((unsigned long)menu << 16) | item;
	if(uSetWindow(myIcon,uWindowGetRealMenuAndItem,menudata,&menu,&item,
			              uWindowSetDone,uWindowSetDone)){
		menu=HIWORD(menudata);
		item=LOWORD(menudata);
	}
				
	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;

   	ModifyMenu(Menu,item-1,MFT_STRING,item-1,name);

	return 0;
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
	int retc;

	if(!name)return 1;

	retc=GetAsyncKeyState(VK_ESCAPE);
	if(retc & 0x8000){
		int ret;
		ret=MessageBox(IconRoot->myWindow,name,"Stop",MB_YESNO);
		if(ret != IDYES)return 0;
		return 1;
	}

	return 0;
}
int SetFileType(char *name,char *type)
{
	if(!name)return 1;
	type=type;
	return 0;
}

int uGetClipRect(uRect *r,struct Icon *myIcon)
{
	RECT rect;

	if(!myIcon || !r)return 1;

	GetClipBox(myIcon->hdc,&rect);
	r->x = rect.left;
	r->y = rect.top;
	r->xsize = rect.right-rect.left;
	r->ysize = rect.bottom-rect.top;
	return 0;
}

int uSetClipRect(uRect *r,struct Icon *myIcon)
{
	if(!myIcon || !r)return 1;


	SetRectRgn(myIcon->region, r->x, r->y, r->x+r->xsize,r->y+r->ysize);
	SelectClipRgn(myIcon->hdc,myIcon->region);

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

int MakeNewDirectory(char *name)
{
	if(!name)return 1;
	
	if(domkdir(name))return 1;
	
	return chdir(name);
}

char *DefaultPathString(void)
{
    char working[2048];
    
    if(!getcwd(working,2048))return NULL;

	return strsave(working,1982);
}
int uDrawString(char *name,struct Icon *myIcon)
{
	SIZE size;
	if(!myIcon || !name)return 1;
	GetTextExtentPoint32(myIcon->hdc,name,strlen(name),&size);
	TextOut(myIcon->hdc,xold,yold-size.cy,name,strlen(name));
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
		POINT p[2];

		if(!myIcon)return 1;

		p[0].x=xold;
		p[0].y=yold;
		p[1].x=ix;
		p[1].y=iy;
	    Polyline(myIcon->hdc,p,2);
        xold=ix;
        yold=iy;
	return 0;
}

static int sScreenSetSize(long xsizei,long ysizei,struct ScreenInfo *s,struct Icon *myIcon)
{
	char string[256];
	if(!s)return 1;

	if(myIcon->ThreadId != GetCurrentThreadId()){
		return 1;
	}

	if(!s->hbitmap || (s->xsizei != xsizei ||
		s->ysizei != ysizei)) {
		BITMAPINFOHEADER *ib;
		int xImageDepth;

		if(xsizei <= 0 || ysizei <= 0)return 0;

		xImageDepth=24;
		
		if(s->hbitold && s->bitdc)SelectObject(s->bitdc,s->hbitold);
		s->hbitold=NULL;

		if(s->hbitmap){
			DeleteObject(s->hbitmap);
			s->hbitmap=NULL;
			s->buffer=NULL;
		}


		{
			long xsize,ysize,diff;
			
			s->xsizei = 0;
			s->ysizei = 0;
			ib=&s->ibitmap.bmiHeader;
			
			ib->biSize  = sizeof(BITMAPINFOHEADER);
			ib->biPlanes  = 1;
			ib->biBitCount  = xImageDepth;
			xsize=xsizei/4;
			xsize = 4*xsize;
			diff=xsizei-xsize;
			if(diff > 0){
				xsize=xsizei+4-diff;
			}
			ib->biWidth  = xsize;
			
			ysize=ysizei;
			ib->biHeight = -ysize;
			ib->biSizeImage = 3*ysize*xsize;
//			ib->biSizeImage = 3*ysize*xsizei;
			ib->biCompression = BI_RGB;
		}
		
		s->length=ib->biSizeImage;
		
		
		s->hbitmap=CreateDIBSection(myIcon->hdc,&s->ibitmap,DIB_RGB_COLORS,
			&s->buffer,NULL,0);
/*
		{
			static FILE *out;
			static long count;

			if(!out)out=fopen("junk2.jnk","w");
			if(out && !s->hbitmap){
				fprintf(out,"CreateDIB %ld Depth %d xsize %ld ysize %ld Error %ld ThreadID %ld myIcon %lx\n",
				++count,xImageDepth,(long)xsizei,(long)ysizei,(long)GetLastError(),(long)GetCurrentThreadId(),(unsigned long)myIcon);
				fflush(out);
			}else{
				fprintf(out,"CreateDIB OK %ld\n",++count);
				fflush(out);
			}
		}
*/
		if(!s->hbitmap || !s->buffer){
			sprintf(string,"CreateDIBSection Error xImageDepth %d xsize %ld ysize %ld Error %ld ThreadID %ld myIcon %ld s %lx\n",
				xImageDepth,(long)xsizei,(long)ysizei,(long)GetLastError(),(long)GetCurrentThreadId(),(long)myIcon,(long)s);
			// WarningBatch(string);
			return 1;
		}
		
		if(!s->bitdc)s->bitdc=CreateCompatibleDC(myIcon->hdc);
		
		s->hbitold=SelectObject(s->bitdc,s->hbitmap);
		if(!s->hbitold){
			return 1;
		}

		s->xsizei = xsizei;
		s->ysizei = ysizei;

		return 0;
	}
		return 0;
}
int ScreenSetSize(long xsizei,long ysizei,struct Icon *myIcon)
{
	struct ScreenInfo *s;
	
	if(!myIcon)return 1;
	s=&myIcon->Screen;
	
	return sScreenSetSize(xsizei,ysizei,s,myIcon);
}

int uScreenWrite(long xsize,long ysize,unsigned char *data,unsigned char *buff,int DataType,
                uRect *SRect,uRect *DRect,struct Icon *myIcon)
{
	struct ScreenInfo *s;
	uRect DDRect;
	int x,y;
	/* int n; */
	
	if(!SRect || !DRect || !myIcon || !data)return 1;
	
	DDRect= *DRect;
	x=DDRect.x;
	y=DDRect.y;
	DDRect.x = 0;
	DDRect.y = 0;

	s = &myIcon->Screen;
	
	s->DataType=DataType;

	s->xsize=0;
	s->ysize=0;
	
	if(DataType == ScreenDataType8){
		WriteRectToRect2(myIcon->pal,data,SRect,xsize,ysize,s->buffer,&DDRect,
			             s->ibitmap.bmiHeader.biWidth,s->ysizei);

		s->xsize=xsize;
		s->ysize=ysize;
		s->buff=data;
		s->data=buff;
/*
		
			for(n=0;n<s->length/3;++n){
			s->buff[3*n]=0;
			s->buff[n*3+1]=255;
			s->buff[n*3+2]=0;
		}
*/

	/*
		if(!junk)junk=fopen("junk.junk","w");
		if(junk)fprintf(junk,"%s %ld %ld %ld\n",myIcon->WindowName,(long)myIcon,(long)myIcon->hdc,(long)myIcon->myWindow);
*/
		
		if(!BitBlt(myIcon->hdc,x,y,s->ibitmap.bmiHeader.biWidth,
			abs(s->ibitmap.bmiHeader.biHeight),(s->bitdc),0,0,SRCCOPY)) {
			/* WarningBatch("BitBlt EEEeeeek!\n"); */
		}
			
	}else if(DataType == ScreenDataType24){
		
		s->xsize=xsize;
		s->ysize=ysize;
		s->buff=data;
		s->data=buff;


		WriteRectToRect24(data,SRect,xsize,ysize,s->buffer,&DDRect,
			             s->ibitmap.bmiHeader.biWidth,s->ysizei);

		/*
			for(n=0;n<s->length/3;++n){
			s->buff[3*n]=0;
			s->buff[n*3+1]=255;
			s->buff[n*3+2]=0;
		}
		*/
/*				
		if(!junk)junk=fopen("junk.junk","w");
		if(junk)fprintf(junk,"%s %ld %ld %ld\n",myIcon->WindowName,(long)myIcon,(long)myIcon->hdc,(long)myIcon->myWindow);
*/
		
		BitBlt(myIcon->hdc,x,y,s->ibitmap.bmiHeader.biWidth,
			abs(s->ibitmap.bmiHeader.biHeight),(s->bitdc),0,0,SRCCOPY);
	}	
	return 0;
}
int WriteRectToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                      unsigned char *out,uRect *DRect,long oxsize,long oysize)
{
	double xScale,yScale;
	register unsigned char *outp,*inp;
	register int ic;
	long oxsize3;
	long xsize3;
	int i,j,jj,ii;
	int jd,id;
	int is,js;
	int ie,je;
	
	
	if(!in || !out || !SRect || !DRect )return 1;
	
	oxsize3=3*oxsize;
	xsize3=3*xsize;
	
	if((SRect->xsize == DRect->xsize) && (SRect->ysize == DRect->ysize)){
	    js=SRect->y;
	    if(js < 0)js = 0;
	    
	    je=SRect->y+SRect->ysize;
	    if(je >= ysize)je=ysize;
	    
	    is=SRect->x;
	    if(is < 0)is = 0;
	    
	    ie=SRect->x+SRect->xsize;
	    if(ie >= xsize)ie=xsize;
	    	    
	    jj=DRect->y;
	    if(jj < 0){
	        js -= jj;
	        jj=0;
	    }
	    
	    if(je-js > oysize){
	        je=js+oysize;
	    }


	    ii=DRect->x;
	    if(ii < 0){
	        is -= ii;
	        ii=0;
	    }

	    if(ie-is > oxsize){
	        ie=is+oxsize;
	    }
	    
	    ie -= is;
	    
	    if(ie < 0)ie = 0;
	    
	    for(j=js;j<je;++j){
	    	outp=out+jj++ *oxsize3;
	    	inp=in+is*3+j*xsize3;
	    	
	    	ic=ie+1;
	    	while(--ic){
	    		*(outp++ +2) = *inp++;
	    		*outp++      = *inp++;
	    		*(outp++ -2) = *inp++;
	    	}
	    }
	}else{
	    xScale=(double)SRect->xsize/(double)DRect->xsize;
	    yScale=(double)SRect->ysize/(double)DRect->ysize;
	    for(j=0;j<DRect->ysize;++j){
	        jd=j+DRect->y;
	    	if(jd < 0 || jd >= oysize)continue;
	    	js=SRect->y+(int)(j*yScale);
	    	if(js < 0 || js >= ysize)continue;	 
	    	for(i=0;i<DRect->xsize;++i){
	        	id=i+DRect->x;
	    		if(id < 0 || id >= oxsize)continue;
	    		is=SRect->x+(int)(i*xScale);
	    		if(is < 0 || is >= xsize)continue;	 
	    		out[id*3+jd*oxsize3]=  in[is*3+2+js*xsize3];
	    		out[id*3+1+jd*oxsize3]=in[is*3+1+js*xsize3];
	    		out[id*3+2+jd*oxsize3]=in[is*3+0+js*xsize3];
	    	}
	    }
	}
	
	return 0;
}

int WriteRectToRect2(unsigned char *pal, unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize)
{
	double xScale,yScale;
	register unsigned char *outp,*inp;
	register int ic;
	int i,j,jj,ii;
	int jd,id;
	int is,js;
	int ie,je;
	int nc;
	
	
	if(!in || !out || !SRect || !DRect || !pal)return 1;

	if((SRect->xsize == DRect->xsize) && (SRect->ysize == DRect->ysize)){
	    js=SRect->y;
	    if(js < 0)js = 0;
	    
	    je=SRect->y+SRect->ysize;
	    if(je >= ysize)je=ysize;
	    
	    is=SRect->x;
	    if(is < 0)is = 0;
	    
	    ie=SRect->x+SRect->xsize;
	    if(ie >= xsize)ie=xsize;
	    	    
	    jj=DRect->y;
	    if(jj < 0){
	        js -= jj;
	        jj=0;
	    }
	    
	    if(je-js > oysize){
	        je=js+oysize;
	    }


	    ii=DRect->x;
	    if(ii < 0){
	        is -= ii;
	        ii=0;
	    }

	    if(ie-is > oxsize){
	        ie=is+oxsize;
	    }
	    
	    ie -= is;
	    
	    if(ie < 0)ie = 0;
	    
	    for(j=js;j<je;++j){
	    	outp=out+jj++ *oxsize*3;
	    	inp=in+is+j*xsize;	    	
	    	ic=ie+1;
	    	while(--ic){	 
				nc= *inp++;
	    		*outp++ = pal[3*nc+2];
	    		*outp++ = pal[3*nc+1];
	    		*outp++ = pal[3*nc];
	    	}
	    }
	}else{
	    xScale=(double)SRect->xsize/(double)DRect->xsize;
	    yScale=(double)SRect->ysize/(double)DRect->ysize;
	    for(j=0;j<DRect->ysize;++j){
	        jd=j+DRect->y;
	    	if(jd < 0 || jd >= oysize)continue;
	    	js=SRect->y+(int)(j*yScale);
	    	if(js < 0 || js >= ysize)continue;	 
	    	for(i=0;i<DRect->xsize;++i){
	        	id=i+DRect->x;
	    		if(id < 0 || id >= oxsize)continue;
	    		is=SRect->x+(int)(i*xScale);
	    		if(is < 0 || is >= xsize)continue;	 
				nc=in[is+js*xsize];
	    		out[3*id+jd*oxsize*3]=pal[3*nc+2];
	    		out[3*id+1+jd*oxsize*3]=pal[3*nc+1];
	    		out[3*id+2+jd*oxsize*3]=pal[3*nc];
	    	}
	    }
	}
	
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
int goCD(char *name)
{
	if(!name)return 1;
	return chdir(name);
}

void pNewMenu(IconPtr myIcon,int item)
{
	if(!myIcon)return;
	item=item;
	return;
}

int uCheckMenuItem(struct Icon *myIcon,int menu,int item,int value)
{
	menuPtr Menu;
	int menudata;
	
	if(!myIcon)return 1;


	menudata = ((unsigned long)menu << 16) | item;
	if(uSetWindow(myIcon,uWindowGetMenuAndItem,menudata,&menu,&item,
			              uWindowSetDone,uWindowSetDone)){
		menu=HIWORD(menudata);
		item=LOWORD(menudata);
	}
				
	Menu=NULL;
	
	if(uSetWindow(myIcon,	       
		uWindowGetMenu,menu,&Menu,
		uWindowSetDone,uWindowSetDone)
	)return 1;
         
	if(value){
		CheckMenuItem(Menu,item-1,MF_CHECKED | MF_BYPOSITION);
//		CheckMenuItem(Menu,menudata,MF_CHECKED);
	}else{
		CheckMenuItem(Menu,item-1,MF_UNCHECKED | MF_BYPOSITION);
//		CheckMenuItem(Menu,menudata,MF_UNCHECKED);
	}
	return 0;
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

int uInvalWindowRect(IconPtr myIcon,uRect *r)
{
	RECT rr;
	if(!myIcon || !r)return 1;

	rr.top=r->y;
	rr.bottom=r->y+r->ysize;
	rr.left=r->x;
	rr.right=r->x+r->xsize;
	InvalidateRect(myIcon->myWindow,&rr,FALSE);
	
	return 0;
}

int InvalRectMyWindow(IconPtr myIcon)
{
	RECT r;

    if(!myIcon)return 1;
	GetClientRect(myIcon->myWindow,&r);
    InvalidateRect(myIcon->myWindow,&r,FALSE);
    return 0;
}

int uGetMaximumBounds(struct Icon *myIcon,uRect *r)
{
   BOOL fResult;
	RECT rw;

	if(!myIcon || !r)return 1;

	r->xsize=2000;	
	r->ysize=2000;	


    fResult = SystemParametersInfo(
      SPI_GETWORKAREA,   // get mouse information 
      0,           // not used 
      &rw,            // holds information
      0 );       // not used 
 
   // Double it. 
 
   if( fResult )
   {
		r->xsize=rw.right - rw.left;;	
		r->ysize=rw.bottom - rw.top;	
   }

	return 0;
}
int uGetPortBounds(IconPtr myIcon,uRect *r)
{
	RECT rw;

	if(!myIcon || !r)return 1;

	GetClientRect(myIcon->myWindow,&rw);
	r->x = rw.left;
	r->y = rw.top;
	r->xsize = rw.right - rw.left;
	r->ysize = rw.bottom - rw.top;
	return 0;
}
int uGetPortBoundsGlobal(IconPtr myIcon,uRect *r)
{
	RECT rw;

	if(!myIcon || !r)return 1;
 

	if(GetWindowRect(myIcon->myWindow,&rw)){
		r->x = rw.left;
		r->y = rw.top;
		r->xsize = rw.right - rw.left;
		r->ysize = rw.bottom - rw.top;
	}else{
		return 1;
	}
	return 0;
}
int uSizeWindow(struct Icon *myIcon,int xsize,int ysize)
{
	RECT r,ro;
	int dx,dy;

	if(!myIcon)return 1;

	GetWindowRect(myIcon->myWindow,&r);
	GetClientRect(myIcon->myWindow,&ro);

	dx=xsize-(ro.right-ro.left);
	dy=ysize-(ro.bottom-ro.top);

	MoveWindow(myIcon->myWindow,r.left,r.top,(int)(r.right-r.left+dx),
		(int)(r.bottom-r.top+dy),1);
	return 0;
}
int uSizeWindowGlobal(struct Icon *myIcon,int xsize,int ysize)
{
	RECT r;

	if(!myIcon)return 1;

	GetWindowRect(myIcon->myWindow,&r);

	MoveWindow(myIcon->myWindow,r.left,r.top,xsize,ysize,1);

	return 0;
}

int uMoveWindow(struct Icon *myIcon,int x,int y)
{
	RECT r;

	if(!myIcon)return 1;

	GetWindowRect(myIcon->myWindow,&r);
	MoveWindow(myIcon->myWindow,x,y,(int)(r.right-r.left),
									(int)(r.bottom-r.top),1);
	return 0;
}


int SynchronizePalettes(IconPtr myIcon,unsigned char *red,unsigned char *green,unsigned char *blue);

void CWindowdoPalMenu(IconPtr myIcon,int menu,int Item)
{
	unsigned char *pal;
	char *name;
	int n;

	if(Item == 1){		
		doGetPalette(myIcon);
	}else if(Item == 2){
		doSavePalette(myIcon);
	}else if(Item == 3){
		doAddPalette(myIcon,NULL);
	}else if(Item == 4){
		GetOldColorPalette(myIcon);
		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	}else if(Item == 5){
		LoadToMenu(myIcon);
	}else if(Item == 6){				
		SynchronizePalettes(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	}else if(Item == 7){	
		doPaletteToTable(myIcon);
	}else if(Item >= 9){

		getUniversalPaletteItem(Item-9,&name,&pal);

		if(IsShift()){
			deleteUniversalPaletteByName(name);
			return;
		}


		mstrncpy(myIcon->paletteName,name,255);

		for(n=0;n<256;++n){
			myIcon->red[n]=myIcon->pal[3*n]=pal[3*n];
			myIcon->green[n]=myIcon->pal[3*n+1]=pal[3*n+1];
			myIcon->blue[n]=myIcon->pal[3*n+2]=pal[3*n+2];
		}
		SetOldColorPalette(myIcon);
		SetDefaultPaletteName(myIcon,myIcon->paletteName);
	}

	InvalRectMyWindow(myIcon);

	return;
}
int getTrueFromScrap(long *xsizei,long *ysizei,unsigned char **imagei)
{
	PBITMAPINFO info;
	unsigned char *image1,*image2;
	long xsize;
	long ysize;
	long length,n;
	int ret;

	if(!xsizei || !ysizei || !imagei)return 1;

	ret = 1;
	if (!IsClipboardFormatAvailable(CF_DIB))return 1; 
	if(!IconRoot || !IconRoot->myWindow)return 1;
    if (!OpenClipboard(IconRoot->myWindow))return 1; 

        info = (PBITMAPINFO)GetClipboardData(CF_DIB); 
        if (info != NULL) 
        { 
			*xsizei = xsize = info->bmiHeader.biWidth;
			*ysizei = ysize = info->bmiHeader.biHeight;
			*imagei = image2= cMalloc(info->bmiHeader.biSizeImage,8641);
			if(image2){
				length=xsize*ysize;
				image1=(unsigned char *)info->bmiColors;
				if(info->bmiHeader.biBitCount == 32 &&
				  (length*4 == info->bmiHeader.biSizeImage)){
					for(n=0;n<length;++n){
						image2[3*n+2]=image1[4*n];
						image2[3*n+1]=image1[4*n+1];
						image2[3*n]=image1[4*n+2];
					}
					flip2(image2,xsize,ysize);
				}else if(info->bmiHeader.biBitCount == 24 &&
				  (length*3 == info->bmiHeader.biSizeImage)){
					for(n=0;n<length;++n){
						image2[3*n+2]=image1[3*n];
						image2[3*n+1]=image1[3*n+1];
						image2[3*n]=image1[3*n+2];
					}
					flip2(image2,xsize,ysize);
				}else{
					memcpy(image2,info->bmiColors,info->bmiHeader.biSizeImage);
				}
			}else{
				goto ErrorOut;
			}

		}else{
			goto ErrorOut;
		}

	ret = 0;
ErrorOut:
		CloseClipboard(); 

	return ret;
}
int drawToScrap(IconPtr myIcon,uRect *Boxi,long CurrentFrame,int flag)
{
	HBITMAP hbit;
	HDC memdc;
	BITMAPINFOHEADER *ib;
	struct ScreenInfo *s;
	int ret;

	if(!myIcon)return 1;

	if(flag){
		ZeroScrap();
		uZeroScrap();
	}

	s=&myIcon->Screen;
	if(!s)return 1;

	ret = 1;

	ib=&s->ibitmap.bmiHeader;
	memdc=CreateCompatibleDC(myIcon->hdc);
	hbit=CreateCompatibleBitmap(myIcon->hdc,(int)ib->biWidth,(int)abs(ib->biHeight));
	if(hbit){
		SelectObject(memdc,hbit);
        BitBlt(memdc,0,0,
			s->ibitmap.bmiHeader.biWidth,
			abs(s->ibitmap.bmiHeader.biHeight),
            (s->bitdc),0,0,SRCCOPY);
		if (!OpenClipboard(myIcon->myWindow)) goto ErrorOut;
		if (!EmptyClipboard()) goto ErrorOut;
		if (!SetClipboardData(CF_BITMAP,hbit)) goto ErrorOut;
		if (!CloseClipboard()) goto ErrorOut;
		InvalidateRect(myIcon->myWindow,NULL,TRUE);
	}

	ret = 0;

ErrorOut:

	if(memdc)DeleteDC(memdc);
	CloseClipboard();
	return ret;
}

int Print(IconPtr myIcon)
{
	PRINTDLG pd;
	struct ScreenInfo *s;
	
	if(!myIcon)return 1;
	s=&myIcon->Screen;
	if(!s)return 1;
	
	// Initialize the PRINTDLG structure.
	//...................................
	memset( &pd, 0, sizeof( PRINTDLG ) );
	pd.lStructSize    = sizeof( PRINTDLG );
	pd.hwndOwner      = myIcon->myWindow;
	pd.Flags          = PD_RETURNDC;
	pd.hDevMode       = myIcon->psd.hDevMode;
	pd.hDevNames      = myIcon->psd.hDevNames;
	
	// Create the print dialog.
	//.........................
	if ( PrintDlg( &pd ) )
	{
		WORD      wCopies;
		RECT      rect;
		DOCINFO   di;
		LPDEVMODE lpDevMode = (LPDEVMODE)GlobalLock( pd.hDevMode );
		
		memset( &di, 0, sizeof( DOCINFO ) );
		
		/* For debugging purposes */
		/*
		{
		unsigned int bitspixel,planes;
		unsigned int numcolors;
		int physoffsetx,physoffsety;
		int logpixelsx,logpixelsy;
		int pageTotalHorzDots,pageTotalVertDots;
		bitspixel=GetDeviceCaps( pd.hDC, BITSPIXEL );
		planes=GetDeviceCaps( pd.hDC, PLANES );
		numcolors=GetDeviceCaps( pd.hDC, NUMCOLORS);
		physoffsetx=GetDeviceCaps( pd.hDC, PHYSICALOFFSETX );
		physoffsety=GetDeviceCaps( pd.hDC, PHYSICALOFFSETY );
		logpixelsx=GetDeviceCaps( pd.hDC, LOGPIXELSX );
		logpixelsy=GetDeviceCaps( pd.hDC, LOGPIXELSY );
		pageTotalHorzDots=GetDeviceCaps( pd.hDC, HORZRES );
		pageTotalVertDots=GetDeviceCaps( pd.hDC, VERTRES );
		}
		*/
		
		// Initialize rect to size of page.
		//.................................
		rect.top = 
			( GetDeviceCaps( pd.hDC, LOGPIXELSY )*
			(myIcon->psd.rtMargin.top/1000 ))-
			GetDeviceCaps( pd.hDC, PHYSICALOFFSETY );
		
		rect.left = 
			( GetDeviceCaps( pd.hDC, LOGPIXELSX )*
			(myIcon->psd.rtMargin.left/1000 ))-
			GetDeviceCaps( pd.hDC, PHYSICALOFFSETX );
		
		rect.right = 
			GetDeviceCaps( pd.hDC, HORZRES ) - 
			( GetDeviceCaps( pd.hDC, LOGPIXELSX )*
			(myIcon->psd.rtMargin.right/1000 )) +
			GetDeviceCaps( pd.hDC, PHYSICALOFFSETX );
		
		rect.bottom = 
			GetDeviceCaps( pd.hDC, VERTRES ) - 
			( GetDeviceCaps( pd.hDC, LOGPIXELSY )*
			(myIcon->psd.rtMargin.bottom/1000 )) +
			GetDeviceCaps( pd.hDC, PHYSICALOFFSETY );
		
		// Initialize di with the document title.
		//.......................................
		di.cbSize      = sizeof( DOCINFO );
		di.lpszDocName = "TrueColor Print";
		
		if ( StartDoc( pd.hDC, &di ) > 0 )
		{
			HDC hdc;
			double scalex,scaley;
			BITMAPINFOHEADER *ib;
			long xsize,ysize,xsizeWin;
									
			ib=&s->ibitmap.bmiHeader;
			ib->biHeight=abs(ib->biHeight);
			
			/* Use WINDOW width rather than BITMAP width since bitmap has 
			been 4-byte aligned - avoids printing black line on right side of image */
			xsizeWin=myIcon->uviewRect.xsize;

			xsize=ib->biWidth;
			ysize=ib->biHeight;
						
			scalex=(double)(rect.right-rect.left)/xsizeWin;
			scaley=(double)(rect.bottom-rect.top)/ysize;
			if (scalex<scaley) scaley=scalex;
			else scalex=scaley;
			
			hdc=myIcon->hdc;
			myIcon->hdc=pd.hDC;
			
			for( wCopies = 0; wCopies < pd.nCopies; wCopies++ )
			{
				StartPage( pd.hDC );
				
				SetRectRgn(myIcon->region,rect.left,rect.top,rect.right,rect.bottom);
				SelectClipRgn(myIcon->hdc,myIcon->region);
				
				flip2(s->buffer,(int)xsize,(int) ysize);
				
				StretchDIBits(myIcon->hdc,
					rect.left,rect.top,
					(int)floor(scalex*xsizeWin),
					(int)floor(scaley*ysize),
					0,0,
					xsizeWin,ysize,
					s->buffer,&s->ibitmap,DIB_RGB_COLORS,
					SRCCOPY);
				
				EndPage( pd.hDC );
			}
			
			myIcon->hdc=hdc;
			
			EndDoc( pd.hDC );
		}
		
		// Unlock and free resources.
		//...........................
		GlobalUnlock( pd.hDevMode );
		
		myIcon->psd.hDevMode = pd.hDevMode;
		myIcon->psd.hDevNames = pd.hDevNames;
		
		DeleteDC( pd.hDC ); 
	}
	else if ( CommDlgExtendedError() == PDERR_NODEFAULTPRN ){
		Warning("No default printer selected.");
	}
	
	return 0;
}

int PageSetup(IconPtr myIcon)
{
	int ret;
	
	if(!myIcon)return 1;
	
	ret=PageSetupDlg(&myIcon->psd);
	if (ret==0) {
		return 1; /* Cancel, close, or error */
	} else {
		return 0; /* OK */
	}
}

int PagePreview(IconPtr myIcon)
{
	if(!myIcon)return 1;
	return 0;
}

int StandardDoActive(IconPtr myIcon,int flag)
{
	if(!myIcon)return 1;
	flag=flag;
	return 0;
}


IconPtr CDrawNewLine(struct linedata *li);

int HiliteMenu(int value)
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
	item=item;
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
	value=value;
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

int ZeroScrap(void)
{
	
	if (!OpenClipboard(NULL)) goto ErrorOut;
	if (!EmptyClipboard()) goto ErrorOut;
	if (!CloseClipboard()) goto ErrorOut;
	
	return 0;
ErrorOut:
	CloseClipboard();
	return 1;
}
long putStringToScrap(char *mess)
{
	char buff[256];
	long length1,length2;
	HANDLE hMem;
	char *lpData;
	int ret;

	if(!mess)return 1;

	ret=1;

	hMem=NULL;
	if (!OpenClipboard(NULL)) goto ErrorOut;
	if (!EmptyClipboard()) goto ErrorOut;
	length1=(long)(strlen(mess)+1);
	length2=2*length1+1;
	hMem=GlobalAlloc(GHND,length2);
	if(hMem == NULL) {
	    sprintf(buff,"GlobalAlloc Out of Memory Requested (%ld)\n",length2);
	    WarningBatch(buff);
	    goto ErrorOut;
	}
	lpData=(char*)GlobalLock(hMem);
	zerol(lpData,length2);

	while(*mess) {
		*lpData++ = *mess;
		if (*mess++ == '\r') {
			*lpData++ = 10;
		}
	}

	*lpData=0;

	GlobalUnlock(hMem);
	if(!SetClipboardData(CF_TEXT,hMem))goto ErrorOut;
	hMem=NULL;
	if (!CloseClipboard()) goto ErrorOut;

	ret = 0;

ErrorOut:
	if(hMem)GlobalFree(hMem);
	if(ret)CloseClipboard();
	return ret;
}
char *getStringFromScrap(void)
{
	long ic1,ic2,length;
	char *ret;
	char *cbData;
	
	ret=NULL;

	if (!OpenClipboard(NULL)) goto ErrorOut;
	if ((cbData = (char*)GetClipboardData(CF_TEXT))==NULL) goto ErrorOut;

	length=(long)(strlen(cbData)+1);
	ic1=0;
	ic2=0;
	while(ic1<length) {
		if (cbData[ic1]==10) ic1++;
		cbData[ic2++]=cbData[ic1++];
	}

	ret=strsave(cbData,92876);
	if (!CloseClipboard()) goto ErrorOut;

	return ret;

ErrorOut:

	if(ret)cFree((char *)ret);
	CloseClipboard();
	return NULL;
}
/*
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
*/
int uPointer(uPoint *pt,IconPtr myIcon)
{
	POINT p;

	if(!pt || !myIcon)return 1;

	GetCursorPos(&p);
	ScreenToClient(myIcon->myWindow,&p);

	pt->x=p.x;
	pt->y=p.y;
	
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
	
//	if(myIcon->MenuBarList)MenuBarUpdate(myIcon->MenuBarList);

	return 0;
}

static int doButtonPress(IconPtr myIcon,XEvent *myevent)
{
	return 0;
}

static int doExpose(IconPtr myIcon,XEvent *myevent)
{
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
        Menur=MenuNew(MenuName,&menuRect,MenuBarList->myWindow);
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

int isKeyBoardCommand(struct Icon *myIcon,int commandKey)
{
    struct uMenuList *menuList;
    struct uMenuItems *Items;
    char *cp;
    long n;
    int Item,Menu;
    
	if(!myIcon)return  0;
	menuList=myIcon->MenuList;
	if(!menuList)return 0;

	n=0;
	while((Items=menuList[n++].MenuItems) != NULL){
		if(!Items->ItemName)return 0;
		
		Menu=Items->ItemNumber;
		while(++Items && Items->ItemName){
			cp=strrchr(Items->ItemName,'/');
			if(cp){
				++cp;
				if(commandKey == *cp){
					Item=Items->ItemNumber;
					if(myIcon->pMenu)
						(*myIcon->pMenu)(myIcon,Menu,Item);
					return 1;
				}
			}
		}
    }
	
	return 0;
}
int uSetWindow(struct Icon *myIcon,...)
{
	struct uMenuList *menuList;
	struct uMenuItems *Items;
	HMENU *pMenu;
	unsigned int menudata;
	char buff[256],pal[256*3];
	HMENU menu,menuPopup;
	int getMenu;
	int fmenu,fitem;
	int command;
	char *name;
	va_list ap;
	int *ret;
	int n,nn,k,isPaletteMenu;
	int MenuNumber;
	
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
			pMenu=va_arg(ap,HMENU *);
			menuList=myIcon->MenuList;
			if(!menuList)return 1;
			n=0;
			while((Items=menuList[n++].MenuItems) != NULL){
				if(!Items->ItemName)return 1;
				
				if(Items->ItemID != getMenu){
					continue;
				}				
				*pMenu=Items->nMenu;				
				return 0;				
			}
			return 1;     	
			break;
			
		case uWindowSetMenuList:
			menuList=va_arg(ap, struct uMenuList *);
			if(!menuList)return 1;
			menu = CreateMenu();
			if(!menu) {    
				sprintf(buff,"wRect Has NULL pointer");
				Warning(buff);
				return 0;
			}
			n=0;
			while((Items=menuList[n++].MenuItems) != NULL){
			  isPaletteMenu = 0;
				if(Items->ItemName){
					mstrncpy(buff,Items->ItemName,255);
					MenuNumber=Items->ItemID=Items->ItemNumber;
					menuPopup = CreatePopupMenu();
					if(!menuPopup)continue;    
					Items->nMenu=menuPopup;
					if(Items->ItemType == uWindowStandardMenu){                   
						AppendMenu(menu,MF_POPUP | MF_STRING,(unsigned int)menuPopup,buff);
					}else if(Items->ItemType == uWindowFontMenu){
						isPaletteMenu = 2;
						AppendMenu(menu,MF_POPUP | MF_STRING,(unsigned int)menuPopup,buff);
					}else if(Items->ItemType == uWindowStyleMenu){
						isPaletteMenu = 3;
						AppendMenu(menu,MF_POPUP | MF_STRING,(unsigned int)menuPopup,buff);
					}else if(Items->ItemType == uWindowPaletteMenu){
						isPaletteMenu = 1;
						AppendMenu(menu,MF_POPUP | MF_STRING,(unsigned int)menuPopup,buff);						
					}                       
				}
				k=1;
				while(++Items && Items->ItemName){
					mstrncpy(buff,Items->ItemName,255);
					if(isPaletteMenu == 3){
						if(!strcmp(buff,"<BBold")){
							mstrncpy(buff,"Bold",255);
						}else if(!strcmp(buff,"<IItalic")){
							mstrncpy(buff,"Italic",255);
						}else if(!strcmp(buff,"<UUnderline")){
							mstrncpy(buff,"Underline",255);
						}else if(!strcmp(buff,"<OOutline")){
							mstrncpy(buff,"StrikeOut",255);
						}else if(!strcmp(buff,"<SShadow")){
							mstrncpy(buff,"Shadow",255);
						}
					}
					menudata = ((MenuNumber & 0xffff) << 16 | (Items->ItemNumber & 0xffff));
					AppendMenu(menuPopup,MF_STRING,menudata,buff);
					Items->ItemID=k++;
				}
				if(isPaletteMenu == 1){
					nn=0;
					while(!getPalette(nn,buff,pal)){
						menudata = ((MenuNumber & 0xffff) << 16 | (nn+k & 0xffff));
						AppendMenu(menuPopup,MF_STRING,menudata,buff);
						++nn;
					}
				}else if(isPaletteMenu == 2){
					
					DestroySysFontList(&myIcon->pSysFontList);

					if(CreateSysFontList(myIcon,&myIcon->pSysFontList)){
						for(nn=0;nn <myIcon->pSysFontList.count;++nn){
						    menudata = ((MenuNumber & 0xffff) << 16 | (nn+k & 0xffff));
						    AppendMenu(menuPopup,MF_STRING,menudata,myIcon->pSysFontList.name[nn]);
						}
					}
				}
			}
			myIcon->MenuList = CopyMenuList(menuList);	
			SetMenu(myIcon->myWindow,menu);
			break;
		case uWindowGetMenuAndItem2:
			menu=va_arg(ap, HMENU);
			menudata=va_arg(ap, long);
			fmenu=HIWORD(menudata);
			fitem=LOWORD(menudata);
			menuList=myIcon->MenuList;
			if(!menuList)goto ReturnValues;
			n=0;
			while((Items=menuList[n++].MenuItems) != NULL){
				if(!Items->ItemName)return 1;
				
				if(Items->nMenu != menu){
					continue;
				}
				
				fmenu=Items->ItemNumber;

				if(Items->ItemType == uWindowPaletteMenu || Items->ItemType == uWindowSubMenu){
					goto ReturnValues;
				}
				
				goto ReturnValues;
			}
			WarningBatch("uWindowGetMenuAndItem2 not found\n");
			break;
			
		case uWindowGetMenuAndItem:
			menudata=va_arg(ap, long);
			fmenu=HIWORD(menudata);
			fitem=LOWORD(menudata);
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
					if(Items->ItemID == fitem){
						fitem=Items->ItemNumber;
						goto ReturnValues;
					}
					
				}
				goto ReturnValues;
			}
ReturnValues:               
			ret=va_arg(ap,int *);
			*ret=fmenu;
			ret=va_arg(ap,int *);
			*ret=fitem;
			return 0;
			break;
			
		case uWindowGetRealMenuAndItem:
			menudata=va_arg(ap, long);
			fmenu=HIWORD(menudata);
			fitem=LOWORD(menudata);
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
			break;
			
		}
	}
	return 0;
}
int FreeMenuList(struct uMenuList *menuList)
{
	struct uMenuItems *Items;
	int n;

	if(!menuList)return 1;

	n=0;
	while((Items=menuList[n++].MenuItems) != NULL){
		cFree((char *)Items);
	}
	cFree((char *)menuList);

	return 0;
}
struct uMenuList *CopyMenuList(struct uMenuList *menuList)
{
	struct uMenuItems *Items;
	struct uMenuList *List;
	int n,ns,nsmax;

	if(!menuList)return NULL;

	n=0;
	nsmax=1;
	while((Items=menuList[n].MenuItems) != NULL){
		++n;
		ns=1;
		while(Items && Items->ItemName){
			++ns;
			++Items;
		}
		if(ns > nsmax)nsmax=ns;

	}

	++n;

	List=cMalloc(n*sizeof(struct uMenuList *),9374);
	if(!List){
		WarningBatch("CopyMenuList Out Of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)List,n*sizeof(struct uMenuList *));

	n=0;
	while((Items=menuList[n].MenuItems) != NULL){
		ns=0;
		List[n].MenuItems=(struct uMenuItems *)cMalloc(nsmax*sizeof(struct uMenuItems),9375);
	    if(!List[n].MenuItems){
		    WarningBatch("CopyMenuList Out Of Memory\n");
		    goto ErrorOut;
		}
		zerol((char *)List[n].MenuItems,nsmax*sizeof(struct uMenuItems));
		List[n].MenuItems[ns++] = *Items;
		while(++Items && Items->ItemName){
			List[n].MenuItems[ns++] = *Items;
		}
		++n;

	}
	
ErrorOut:
	return List;

}
int SetExpose2(IconPtr myIcon)
{
	return 1;
}

int StandardUpdateWrite(IconPtr myIcon)
{
    return 0;
}
int StandardUpdate(IconPtr myIcon)
{
	if(myIcon->pUpdateWrite != NULL){
		(*myIcon->pUpdateWrite)(myIcon);
	}else{
		StandardUpdateWrite(myIcon);
	}
    return 0;
}
static long doInThumb(controlPtr control,short part,void *controlRoutineData)
{
	IconPtr myIcon=(IconPtr)controlRoutineData;
	
	if(!myIcon || !myIcon->pThumb)return 1;
	
	part=part;

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
IconPtr uCreateIcon(int Type,uRect *wRect)
{
	char buff[256];
	IconPtr myIcon,myIconOld;
 	controlPtr control;
	uRect controlRect;
	static int first;

	myIcon=IconRoot;
	myIconOld=NULL;
	
	while(myIcon){		/*	Search for end of List	*/
		myIconOld=myIcon;
		myIcon=myIcon->IconNext;
	}
	if(!(myIcon=(IconPtr)cMalloc((long)sizeof(struct Icon),9194))){ /*	Make new link	*/
		Warning("uCreateIcon Out of Memory");
		return (IconPtr)NULL;
	}
	zerol((char *)myIcon,sizeof(struct Icon));
	if(myIconOld){
		myIconOld->IconNext=myIcon;
	} else {
		IconRoot=myIcon;
	}
	if(wRect){
		HWND hWnd;
		
		if (Type == xChangeType || Type == xFindType || Type == DumpWindowType || Type==DialogType || Type == SendWindowType || Type == NewWindowType) {
			hWnd = CreateWindowEx(
				WS_EX_OVERLAPPEDWINDOW,
				"AllWindowClassDialog",
				"Dialog",
				WS_OVERLAPPEDWINDOW,
				wRect->x,
				wRect->y,
				wRect->xsize,
				wRect->ysize,
				HWND_DESKTOP,
				NULL,
				xg.hInst,
				NULL);
		} else {
			hWnd = CreateWindowEx(
				WS_EX_OVERLAPPEDWINDOW,
				"AllWindowClass",
				"Window",
				WS_OVERLAPPEDWINDOW,
				wRect->x,
				wRect->y,
				wRect->xsize,
				wRect->ysize,
				HWND_DESKTOP,
				NULL,
				xg.hInst,
				NULL);
		}

		myIcon->myWindow=hWnd;
		
		controlRect.y=wRect->y+25;
		controlRect.x=wRect->x+wRect->xsize-ScrollOffSet;
		controlRect.ysize=wRect->ysize-ScrollOffSet-25;
		controlRect.xsize=ScrollOffSet;
		control=ControlCreate(myIcon,&controlRect,"controlVScroll",
		  controlTypeVScroll,1,0L,
		  0L,0L,10,0L);
		if(!control)goto ErrorOut;
		myIcon->VControl=control;

		controlRect.y=wRect->y+wRect->ysize-ScrollOffSet;
		controlRect.x=wRect->x;
		controlRect.ysize=ScrollOffSet;
		controlRect.xsize=wRect->xsize-ScrollOffSet;
		control=ControlCreate(myIcon,&controlRect,"controlHScroll",
		  controlTypeHScroll,1,0L,
		  0L,0L,20,0L);
		if(!control)goto ErrorOut;
		myIcon->HControl=control;

		myIcon->psd.lStructSize	   = sizeof( PAGESETUPDLG );
		myIcon->psd.hwndOwner	   = hWnd;
		myIcon->psd.rtMargin.top    = 1000;
		myIcon->psd.rtMargin.left   = 1000;
		myIcon->psd.rtMargin.bottom = 1000;
		myIcon->psd.rtMargin.right  = 1000;	
		myIcon->psd.hDevMode 	   = GlobalAlloc( GHND, sizeof( DEVMODE ) );
		myIcon->psd.hDevNames	   = GlobalAlloc( GHND, sizeof( DEVNAMES ) );
		myIcon->psd.Flags		   = PSD_MARGINS | PSD_INTHOUSANDTHSOFINCHES; 
		
		uLoadCursor(myIcon);

		ShowWindow(hWnd, xg.nCmdShow);
		//UpdateWindow(hWnd);
		myIcon->hdc=GetDC(myIcon->myWindow);
		myIcon->region=CreateRectRgn(0,0,100,100);
		if (myIcon->hfont) {
			DeleteObject(myIcon->hfont);
			myIcon->hfont = NULL;
		}

		if(first++ == 0){
			myIcon->hfont=CreateFont(16,0,0,0,FW_NORMAL,
							 0,0,0,ANSI_CHARSET,
							 OUT_DEFAULT_PRECIS,
							 CLIP_DEFAULT_PRECIS,
							 PROOF_QUALITY,
							 DEFAULT_PITCH | FF_MODERN,
							 "Courier New");
			FontStart(myIcon,myIcon->hfont);
			if (myIcon->hfont) {
				DeleteObject(myIcon->hfont);
				myIcon->hfont = NULL;
			}

		}

		if (Type == xChangeType || Type == xFindType || Type == DumpWindowType ||Type==DialogType || Type == SendWindowType || Type == NewWindowType) {
			myIcon->hfont=CreateFont(12,0,0,0,FW_NORMAL,
			0,0,0,ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			DEFAULT_PITCH | FF_MODERN,
			"MS Sans Serif");
		}else{
			myIcon->hfont=CreateFont(16,0,0,0,FW_NORMAL,
							 0,0,0,ANSI_CHARSET,
							 OUT_DEFAULT_PRECIS,
							 CLIP_DEFAULT_PRECIS,
							 PROOF_QUALITY,
							 DEFAULT_PITCH | FF_MODERN,
							 "Courier New");
	}

		SetMapperFlags(myIcon->hdc,1);
		SelectObject(myIcon->hdc,myIcon->hfont);
} else {
		sprintf(buff,"wRect Has NULL pointer");
		Warning(buff);
		return (IconPtr)NULL;
	}
	myIcon->IconNext=NULL;
	myIcon->FileSave=1;
	myIcon->viewRect= *wRect;
	myIcon->WindowType=Type;
	myIcon->pUpdate=StandardUpdate;
	myIcon->pUpdateWrite=StandardUpdateWrite;
	CreateSysFontList(myIcon,&myIcon->pSysFontList);
	SetDefaultFont(myIcon);

	myIcon->ThreadId=GetCurrentThreadId();

	GetPaletteByName("Ps",myIcon->red,myIcon->green,myIcon->blue);
	SetDefaultPaletteName(myIcon,"Ps");
	SetOldColorPalette(myIcon);

	return myIcon;
ErrorOut:
    return NULL;
}

static int SetDefaultFont(IconPtr myIcon)
{
	int nn;

	if(!myIcon)return 1;
	for(nn=0;nn <myIcon->pSysFontList.count;++nn){
		if(!mstrcmp(myIcon->pSysFontList.name[nn],"courier new")){
			DefaultFont=nn+1;
		}

	}

	return 0;

}
int StandardMenu(IconPtr myIcon,int menu,int item) 
{
	return 0;
}

int StandardInContent(IconPtr myIcon) 
{
	return 0;
}

int NetProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam)
{
	IconPtr win;
    int iRc; 
	
	win=FindIWindow(hWnd);
	if(win){
	    ;
	}else{
		return 0;
	}
 
   switch (message) {
    case WM_WSAASYNC:
        switch (WSAGETSELECTEVENT(lParam)) { 
        
        case FD_CONNECT:      
            iRc = WSAGETSELECTERROR(lParam);
            if (iRc) {
				Warning("FD_CONNECT error");
				if(win)win->iconnect = -1;
                return 0;
            }
			if(win)win->iconnect = 1;
            return 0;
        
        case FD_READ:       
 			if(win)win->iread = 1;
           break;
        
        case FD_WRITE:
 			if(win)win->iwrite = 1;
            break;
        
        case FD_CLOSE:
 			/* WriteStatusWindow(win,"Connection Closed"); */
            break;

         case FD_ACCEPT:       
 			if(win)win->iaccept = 0;
            win->rd = accept(wParam, NULL,NULL); 
            if (win->rd == INVALID_SOCKET) {
				Warning("accept error");
            }  
 			if(win)win->iaccept = 1;
           break;

             
        } 

        break;

    case WM_WSAGETHOST: 
        if ( (iRc = WSAGETASYNCERROR(lParam)) != 0) {
		   Warning("StatusWndProc Wrong hGetHostID");
		   if(win)win->gotname = -1;
            return 0;
        }    
		if(win)win->gotname=1;
		break;

	default:
		 return 0;

   }
   return 1;
}

LRESULT CALLBACK AllWndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam)
{
	IconPtr myIcon;
	HDC hdc;
	PAINTSTRUCT paint;
	int keysym,ifound;
	struct ScrollList *scroll;
	controlPtr control;
	SCROLLINFO si; 

	
	myIcon=FindIWindow(hWnd);
/*
	{
		static FILE *out;
		if(!out)out=fopen("Junkout.out","w");
		if(out && !myIcon)fprintf(out,"message %lx %lx\n",(long)message,(long)hWnd);

	}
*/

	if(!myIcon || !myIcon->isOpen) return DefWindowProc(hWnd, message, wParam, lParam);

	if(NetProc(hWnd,message,wParam,lParam))return 0;

/*	
	if(myIcon && message != 0x0d && message != 0x121 && strcmp(myIcon->WindowName,"BatchPrint")){
		sprintf(WarningBuff,"message %lx %lx\n",(long)message,(long)myIcon);
		WarningBatch(WarningBuff);
	}
*/

	switch (message) {	
		case WM_MOUSEMOVE:
		if(myIcon && myIcon->pMouseMoved){
			uPoint where;
			if(myIcon == FindMyFrontWindow()){
			    uPointer(&where,myIcon);
			    if((where.x != xg.pt.x) || (where.y != xg.pt.y)){
				    xg.pt.x=where.x;
				    xg.pt.y=where.y;
				    (*myIcon->pMouseMoved)(myIcon,where);
				}
			}
		}

			break;
		case WM_INITMENU:
			{
					
				if(myIcon->pCheckMenu){
					(*myIcon->pCheckMenu)(myIcon,TRUE);
				}

			}
			break;
		case WM_MENUSELECT:
			{

				int hflag;

				hflag=HIWORD(wParam);
/*
				sprintf(WarningBuff,"hflag %lx good %d\n",(long)hflag,(int)myIcon->itWasGood);
				WarningBatch(WarningBuff);
*/
				if(hflag == 0xFFFF){
					if(myIcon->itWasGood){

						int menu,item;
						int  menudata;

						myIcon->itWasGood=FALSE;
						menudata=myIcon->hitem;

/*						sprintf(WarningBuff,"menudata %ld\n",(long)menudata);
						WarningBatch(WarningBuff);
*/
						if(uSetWindow(myIcon,uWindowGetMenuAndItem2,myIcon->hmenu,menudata,&menu,&item,
						              uWindowSetDone,uWindowSetDone)){
							menu=HIWORD(menudata);
							item=LOWORD(menudata);
/*
						sprintf(WarningBuff,"failed menu %ld %ld\n",(long)menu,(long)item);
						WarningBatch(WarningBuff);
*/
						}
/*
						sprintf(WarningBuff,"menu %ld %ld\n",(long)menu,(long)item);
						WarningBatch(WarningBuff);
*/
						if(myIcon->pMenu){
							(*myIcon->pMenu)(myIcon,menu,item);
						} else {
							StandardMenu(myIcon,menu,item);
						}

					}else{
					    myIcon->hmenu=(HMENU)(0);
					}
				}else{
				    myIcon->hitem=LOWORD(wParam);
					myIcon->hmenu=(HMENU)(lParam);

					if( /*(hflag & MF_MOUSESELECT) && */
						!(hflag & MF_POPUP) &&
						!(hflag & MF_GRAYED) &&
						!(hflag & MF_DISABLED)){

						myIcon->itWasGood=TRUE;
					}else{
						myIcon->itWasGood=FALSE;
					}

				}

		
			}
			break;
		case WM_COMMAND:
			{
				long controlID;
		
				controlID = LOWORD(wParam);
				if(testForControl(myIcon,controlID,HIWORD(wParam))) {
					goto default1;
				}

			}
		break;		
		case WM_DESTROY:
		doDestroyWindow(myIcon);
		return 0;
	case WM_LBUTTONDOWN:

		myIcon->hit=0;
		
		if(myIcon->pInContent){
			(*myIcon->pInContent)(myIcon);
		}else{
			StandardInContent(myIcon);
		}


		break;
		
	case WM_SETCURSOR :
		{
			uPoint where;

			uPointer(&where,myIcon);
			if(myIcon && myIcon->pMouseMoved){
				if(myIcon == FindMyFrontWindow()){
					(*myIcon->pMouseMoved)(myIcon,where);
				}
			}
			if(!uPtInRect(where,&myIcon->uviewRect))goto default1;
		}
		break;
		
	case WM_SIZE :
		if(myIcon->pMoveControls){
			(*myIcon->pMoveControls)(myIcon);	   
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&paint);

		if(myIcon->pUpdate){
			HDC s;
			s=myIcon->hdc;
			SaveDC(s);
			RestoreDC(hdc,-1);
			myIcon->hdc=hdc;			
			SelectObject(myIcon->hdc,myIcon->hfont);
			(*myIcon->pUpdate)(myIcon);
			myIcon->hdc=s;
		}else{
			StandardUpdate(myIcon);
		}


		EndPaint(hWnd,&paint);
/*
		if(myIcon->pUpdate){

			(*myIcon->pUpdate)(myIcon);
		}else{
			StandardUpdate(myIcon);
		}
*/
		break;
		
	case WM_KEYUP:
		break;
	case WM_KEYDOWN:
		ifound=FALSE;
		keysym=(int)wParam;
		if(keysym == VK_LEFT){
			keysym = leftArrow;
			ifound=TRUE;
		}else if(keysym == VK_RIGHT){
			keysym = rightArrow;
			ifound=TRUE;
		}else if(keysym == VK_UP){
			keysym = upArrow;
			ifound=TRUE;
		}else if(keysym == VK_DOWN){
			keysym = downArrow;
			ifound=TRUE;
		}else if(keysym == VK_DELETE){
			keysym = 127;
			ifound=TRUE;
		}
		if (myIcon->pKeyBoard) {
		    if (ifound) {
				(*myIcon->pKeyBoard)(myIcon,keysym);
			}
		}
		break;
	case WM_CHAR:
		{
			unsigned int c;
			c = wParam;
			
			if ((c>=1 && c<=26) && IsControl()) {
				if(isKeyBoardCommand(myIcon,c+'A'-1))return 0;
			}
			if (myIcon->pKeyBoard) {
				(*myIcon->pKeyBoard)(myIcon,c);
			}
		}
		break;
	case WM_HSCROLL:
		control=testForControlHandle(myIcon,lParam,&scroll);
DialogScrollList:
		if(control) {			
			if(control != myIcon->HControl){
				switch(LOWORD(wParam)){
				case SB_THUMBTRACK:
				case SB_THUMBPOSITION:
					ZeroMemory(&si, sizeof(si));
					si.cbSize = sizeof(si);
					si.fMask = SIF_TRACKPOS;       
					GetScrollInfo (control->controlWindow, SB_CTL, &si);
					uSetControlValue(control,si.nTrackPos);
					/* uSetControlValue(control,HIWORD(wParam)); */
					ScrollListdothumb(control,0,scroll);
					break;
				case SB_LINEDOWN:
					ScrollListgoDown(control,0,scroll);
					break;
				case SB_LINEUP:
					ScrollListgoUp(control,0,scroll);
					break;
				case SB_PAGEDOWN:
					ScrollListdoPageDown(control,0,scroll);
					break;
				case SB_PAGEUP:
					ScrollListdoPageUp(control,0,scroll);
					break;
				default:
					return 0;
				}
				break;

			}
		}
		switch(LOWORD(wParam)){
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
            ZeroMemory(&si, sizeof(si));
            si.cbSize = sizeof(si);
            si.fMask = SIF_TRACKPOS;       
			GetScrollInfo (myIcon->HControl->controlWindow, SB_CTL, &si);
			uSetControlValue(myIcon->HControl,si.nTrackPos);
			doInThumb(myIcon->HControl,SB_THUMBPOSITION,(void*)myIcon);
			break;
		case SB_LINEDOWN:
			doInDownButton(myIcon->HControl,SB_LINEDOWN,(void*)myIcon);
			break;
		case SB_LINEUP:
			doInUpButton(myIcon->HControl,SB_LINEUP,(void*)myIcon);
			break;
		case SB_PAGEDOWN:
			doInPageDown(myIcon->HControl,SB_PAGEDOWN,(void*)myIcon);
			break;
		case SB_PAGEUP:
			doInPageUp(myIcon->HControl,SB_PAGEUP,(void*)myIcon);
			break;
		default:
			return 0;
		}
		break;
		
	case WM_VSCROLL:
		control=testForControlHandle(myIcon,lParam,&scroll);
		if(control) {			
			if(control != myIcon->VControl){
				goto DialogScrollList;
			}
		}
		switch(LOWORD(wParam)){
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
            ZeroMemory(&si, sizeof(si));
            si.cbSize = sizeof(si);
            si.fMask = SIF_TRACKPOS;       
			GetScrollInfo (myIcon->VControl->controlWindow, SB_CTL, &si);
			uSetControlValue(myIcon->VControl,si.nTrackPos);
			doInThumb(myIcon->VControl,SB_THUMBPOSITION,(void*)myIcon);
			break;
		case SB_LINEDOWN:
			doInDownButton(myIcon->VControl,SB_LINEDOWN,(void*)myIcon);
			break;
		case SB_LINEUP:
			doInUpButton(myIcon->VControl,SB_LINEUP,(void*)myIcon);
			break;
		case SB_PAGEDOWN:
			doInPageDown(myIcon->VControl,SB_PAGEDOWN,(void*)myIcon);
			break;
		case SB_PAGEUP:
			doInPageUp(myIcon->VControl,SB_PAGEUP,(void*)myIcon);
			break;
		default:
			return 0;
		}
		break;
		
	default:
    default1:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
controlPtr testForControlHandle(IconPtr myIcon,long controlID,struct ScrollList **ListD)
{
	struct ScrollList *List;
	controlPtr c;
	if (!myIcon || !ListD) return NULL;
	if (myIcon->WindowType != DialogType &&
		myIcon->WindowType != SendWindowType &&
		myIcon->WindowType != DumpWindowType &&
		myIcon->WindowType != xChangeType &&
		myIcon->WindowType != xFindType &&
		myIcon->WindowType != NewWindowType) return NULL;
	
	*ListD=NULL;

	c=myIcon->controlList;
	while(c){
		if((long)(c->controlWindow) == controlID) {
			goto FoundIt;
		}
		c=c->controlNext;
	}
	
	
	return NULL;

FoundIt:
	List=uDialogGetScroll(myIcon,c);
	if(!List)return NULL;
	*ListD=List;
	return c;

}

int testForControl(IconPtr myIcon,long controlID,int command)
{
	controlPtr c;
	if (!myIcon) return 0;
	if (myIcon->WindowType != DialogType &&
		myIcon->WindowType != SendWindowType &&
		myIcon->WindowType != DumpWindowType &&
		myIcon->WindowType != xFindType &&
		myIcon->WindowType != xChangeType &&
		myIcon->WindowType != NewWindowType) return 0;
	
	if(myIcon->controlList){
		c=myIcon->controlList;
		if(c->controlID == controlID) {
			goto FoundIt;
		}
		while(c->controlNext){
			c=c->controlNext;
			if(c->controlID == controlID) {
				goto FoundIt;
			}
		}
	}
	
	return 0;
FoundIt:
	if((c->controlType ==  controlTypePopUpMenu) &&
	   (command != LBN_SELCHANGE))return 1;
	DoControlDialog(myIcon,controlID);
//	if(myIcon->pInContent) {
//		(*myIcon->pInContent)(myIcon);
//	}
	return 1;
}

int uLoadCursor(IconPtr myIcon)
{
static unsigned char fiddle_bits[] = {
   0x00, 0x60, 0x00, 0xb0, 0x00, 0xd0, 0x00, 0x68, 0xc0, 0x15, 0x20, 0x0a,
   0x20, 0x06, 0x30, 0x09, 0x9e, 0x08, 0x43, 0x08, 0x29, 0x07, 0x91, 0x01,
   0xa9, 0x00, 0x86, 0x00, 0x8c, 0x00, 0x78, 0x00};

static unsigned char fiddle_mask[] = {
   0x00, 0x60, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0x78, 0xc0, 0x1d, 0xe0, 0x0f,
   0xe0, 0x07, 0xf0, 0x0f, 0xfe, 0x0f, 0xff, 0x0f, 0xff, 0x07, 0xff, 0x01,
   0xff, 0x00, 0xfe, 0x00, 0xfc, 0x00, 0x78, 0x00};

 
	if(!myIcon)return 1;
	myIcon->hCursorArrow =		LoadCursor(NULL,IDC_ARROW);
	myIcon->hCursorWatch =		LoadCursor(NULL,IDC_APPSTARTING);

	myIcon->hCursorFiddle =		LoadCursor(NULL,IDC_SIZEALL);

/*
	myIcon->hCursorFiddle =		CreateCursor(xg.hInst,0,0,32,32,
											fiddle_bits,fiddle_mask);
*/

	myIcon->hCursorSelection =	LoadCursor(NULL,IDC_ARROW);
	myIcon->hCursorBeam =		LoadCursor(NULL,IDC_IBEAM);
	myIcon->hCursorPlus =		LoadCursor(NULL,IDC_CROSS);
	return 0;
}
int uSetCursor(IconPtr myIcon,int Type)
{	
	if(!myIcon)return 1;

	if(!FindWindowInList(myIcon))return 1;

	switch (Type) {
	case uARROW_CURSOR:
		if (myIcon->hCursorArrow) {
			SetCursor(myIcon->hCursorArrow);
		}
		break;
	case uWATCH_CURSOR:
		if (myIcon->hCursorWatch) {
			SetCursor(myIcon->hCursorWatch);
		}
		break;
	case uFIDDLE_CURSOR:
		if (myIcon->hCursorFiddle) { 
			SetCursor(myIcon->hCursorFiddle);
		}
		break;
	case uSELECTION_CURSOR:
		if (myIcon->hCursorSelection) { 
			SetCursor(myIcon->hCursorSelection);
		}
		break;
	case uBEAM_CURSOR:
		if (myIcon->hCursorBeam) {
			SetCursor(myIcon->hCursorBeam);
		}
		break;
	case uPLUS_CURSOR:
		if (myIcon->hCursorPlus)  {
			SetCursor(myIcon->hCursorPlus);
		}
		break;
	}
    return 0;
}
int uDoControls(IconPtr myIcon)
{
	return 0;
}
int  uWriteBox(uRect *r,char *buff,IconPtr myIcon)
{
	extern int DefaultFontSize;
	uRect rSave;
	RECT rr;
	static RECT rold;
	HBRUSH b;

	if(!r || !myIcon)return 1;

	uGetClipRect(&rSave,myIcon);

	uClipRect(r,myIcon);

	b=CreateSolidBrush(RGB(255,255,255));
	rr.top=r->y;
	rr.left=r->x;
	rr.right=rr.left+r->xsize;
	rr.bottom=rr.top+r->ysize;
	rold = rr;
	FillRect(myIcon->hdc,&rr,b);
	DeleteObject(b);

	uSetTextFace(myIcon,0);

	uSetTextFont(myIcon,DefaultFont);		

	uSetFontSize(myIcon,DefaultFontSize);

	if(buff){
		SIZE size;


		GetTextExtentPoint32(myIcon->hdc,buff,strlen(buff),&size);

	    uMoveTo(r->x,r->y+size.cy);
	    uDrawString((unsigned char *)buff,myIcon);
	}

	uSetClipRect(&rSave,myIcon);

	return 0;
}

int  uPaintBox(uRect *r,IconPtr myIcon)
{
	RECT rr;
	HBRUSH b;

	if(!r || !myIcon)return 1;

	b=CreateSolidBrush(RGB(0,0,0));
	rr.top=r->y;
	rr.left=r->x;
	rr.right=rr.left+r->xsize;
	rr.bottom=rr.top+r->ysize;
	FillRect(myIcon->hdc,&rr,b);
	DeleteObject(b);

	return 0;
}
static unsigned char *FLST;
static long FLSTLenght;
static unsigned char *DRW3;
static long DRW3Lenght;
static unsigned char *TEK3;
static long TEK3Lenght;

int uZeroScrap(void)
{
   if(FLST)cFree((char *)FLST);
   FLST=NULL;
   if(DRW3)cFree((char *)DRW3);
   DRW3=NULL;
   if(TEK3)cFree((char *)TEK3);
   TEK3=NULL;
   return 0;
}

long uPutScrap(long Count,char *type,char *out)
{
	long Length;
	
	if(!type || !out)return 0L;
	
	Count=Count;
	
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
int  uPaintRect(uRect *r,IconPtr myIcon)
{
	RECT rr;
	HBRUSH b;

	if(!r || !myIcon)return 1;

	b=myIcon->b;
	rr.top=r->y;
	rr.left=r->x;
	rr.right=rr.left+r->xsize;
	rr.bottom=rr.top+r->ysize;
	FillRect(myIcon->hdc,&rr,b);
	/* Rectangle(myIcon->hdc,rr.left,rr.top,rr.right,rr.bottom); */

	return 0;
}

int doSystem()
{
	return 1;
}
int FilesGetPICT(char *name,unsigned char *buff,long length,IconPtr myIcon,struct FileInfo2 **Files)
{
	int ret;
	if(!name || !buff || !Files || !myIcon)return 1;
	ret=1;
	/*
	Buffer=(char *)buff;
	LengthC=length;
	if(PostGet(myIcon))goto ErrorOut;
	
	return FileAppendPICT(name,pH,Files);
		
	ret=0;
ErrorOut:
*/
	return ret;
}
int uDisposeControl(struct Icon *myIcon,controlPtr control)
{
	if(!myIcon || !control )return 1;

	ControlDispose(control);

	return 0;
}


struct SendData{
    int (*sageThread)(void *data);
	void *data;
};

int runThreads2(long ThreadCount,void *data,long numsteps,int (*sageThreadi)(mThread *Threads),
				int (*sageThread2)(mThread *Threads,int count))
{
	mThread Threads;
	mThread *myThreads;
	HANDLE hThread; 
	struct SendData *sdata;
	/* extern HANDLE hPrintMutex; */

	int nt,threads;
	long tstep,tstart,tend;
	int done;

	if(!sageThreadi)return 1;

	sdata=NULL;

	/* hPrintMutex = NULL; */

	// sageThread=(int(*)(void *))sageThreadi;

	threads=ThreadCount;

	myThreads=NULL;
	if(threads <= 0){
	    zerol((char *)&Threads,sizeof(Threads));
	    Threads.smin=0;
	    Threads.smax=numsteps;
		Threads.data=data;
	    (*sageThreadi)(&Threads);
	    return (*sageThread2)(&Threads,1);		

	}else{

		/* hPrintMutex = CreateMutex( NULL, FALSE, NULL ); did not help */ 

		sdata=(struct SendData *)cMalloc(threads*sizeof(struct SendData),9020);
	    if(!sdata){
			sprintf(WarningBuff,"runThreads Out of memory Requested (%ld)\n",(long)(threads*sizeof(struct SendData)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)sdata,threads*sizeof(struct SendData));

	    myThreads=(mThread *)cMalloc((threads+1)*sizeof(Threads),9020);
	    if(!myThreads){
			sprintf(WarningBuff,"runThreads Out of memory Requested (%ld)\n",(long)(threads*sizeof(Threads)));
			WarningBatch(WarningBuff);
	        goto End;
	    }
	    zerol((char *)myThreads,(threads+1)*sizeof(Threads));
	    tstep=numsteps/(threads+1);
	    if(tstep <= 0){
	        tstep = 1;
			threads = (int)numsteps-1;
	    }

	    tstart=0;
	    for(nt=0;nt<threads;++nt){
			tend=tstart+tstep;
	        myThreads[nt].data= data;
			myThreads[nt].done = FALSE;
			myThreads[nt].smin=tstart;
			myThreads[nt].nthread=nt;
			myThreads[nt].smax=tend;
			sdata[nt].data=&myThreads[nt];
			sdata[nt].sageThread=(int(*)(void *))sageThreadi;

			{
					//DWORD dwThreadId;
					unsigned int dwThreadId;
	
					hThread = (HANDLE)_beginthreadex(  
						NULL,                       
						64000000,                          
						&ThreadFunc,  
						(void *)&sdata[nt],					
						CREATE_SUSPENDED,                         
						&dwThreadId);               

					myThreads[nt].save=hThread;
					if (hThread == NULL){
						WarningBatch("CreateThread Failed\n");
						myThreads[nt].done=1;
					}else{
						ResumeThread(hThread);
					}



			}

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
			(*sageThreadi)(&Threads);
	    }

		while(1){
	        done=TRUE;
	        for(nt=0;nt<threads;++nt){
				if(myThreads[nt].done){
					;
				}else{
					done=FALSE;
				}
	        }
	        if(done)break;
	        Sleep2(50);
	    }

	    if(tstart < numsteps){
			myThreads[threads++]=Threads;
	    }
		
		(*sageThread2)(myThreads,threads);

		for(nt=0;nt<threads;++nt){
			if(myThreads[nt].done){
				if(myThreads[nt].save){
					hThread=myThreads[nt].save;
					myThreads[nt].save=NULL;
					CloseHandle( hThread );
				}
			}
		}
	}
End:
	if(myThreads)cFree((char *)myThreads);
	if(sdata)cFree((char *)sdata);
	/* if(hPrintMutex)CloseHandle( hPrintMutex ); */
	return 0;
}
