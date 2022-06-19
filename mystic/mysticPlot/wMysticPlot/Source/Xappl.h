
#ifndef _XAPPL_
#define _XAPPL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef _int64 INT8_64;
typedef  int Boolean;

#include <windows.h>

typedef HWND WindowPtr;

#define PC 1
#define Windows 1

#include "Paux.h"
#include "SetFrameData.h"
#include "Limits2.h"
#include "paletteDraw.h"
#include "fileDialog.h"
#include "Control.h"
#include "Tools.h"
#include "xMenu.h"
#include "uNetWork.h"

#include "Installed.h"

#include "SysFontList.h"

#define MacintoshTextType 	0
#define UnixTextType 		1
#define WindowsTextType 	2

#define DefaultTextType WindowsTextType


#define upArrow 		30
#define downArrow		31
#define leftArrow		28
#define rightArrow		29

#define ScreenDataType8 	8
#define ScreenDataType24	24
#define ScreenDataTypePict	77


int FilesDirectory(char *Directory,struct FileInfo2 *Files);
int putString(unsigned char *data,long length,FILE *out);
int uMemcpy(void *out1,void *in1,long length);
int strncatToPath(char *pathname,char *name,int length);
int mstrncat(char *out,char *in,long n);
int mstrncpy(char *out,char *in,long n);
int FileListFree(struct FileList *Files);
int  WarningBatch(char *WarningBuff);
int  WarningBatchFast(char *WarningBuff);
int  WarningBatchAtEnd(char *WarningBuff);
int ScreenSetSize(long xsize,long ysize,struct Icon *myIcon);
int FreeFileList(struct FileList *Ret);

struct DumWindow{
	int dum;
};

struct ScreenInfo{
	unsigned char r[256];
	unsigned char g[256];
	unsigned char b[256]; 
	unsigned char pal[256*3]; 
	long xsize;
	long ysize;
	long xsizei;
	long ysizei;
	unsigned char *buff;
	unsigned char *data;
	int DataType;
	unsigned char *buffer;
	long length;
	struct Icon *myIcon;
	int hasPalette;
	BITMAPINFO ibitmap;
	HBITMAP hbitmap;
	HBITMAP hbitold;
	HDC bitdc;
};



struct SaveInformation{
  struct FileInfo2 *Files;
  struct Icon *myIcon;
  long CurrentFrame;
  uRect *DRect;
  int PlotModeOut;
  int PlotTypeOut;
  char PlotName[256];
  int UseImageNames;
  long ImageIncrement;
  long SequenceNumber;
  char *prefix;
  char *suffix;
};






#define DataSpace		14


struct CursorDisplayItems{
    char pioName[64];
    char format[8];
    int pioIndex;
    int valueFlag;
	double *data;
};

struct CursorStruct{
	struct CursorDisplayItems *Items;
    char coordinateFormat[8];
    char valueformat[8];
    char timeFormat[8];
    int valueFlag;
	int count;
};



typedef struct uFontStruct {
   int LineHeight;
   int Descent;
   int CWidth;
} uFontInfo;

typedef struct uPortInfo{
	int save;
} uPort;

struct movieStruct{
   int dummmy;
};
typedef struct movieStruct *Movie;

typedef  HMENU menuPtr;


typedef struct Icon {
 	char WindowName[256];
    struct CursorStruct Cursor;
	struct FileInfo2 *Files;
	struct uNetWork uNet;
	HWND myWindow;
    HMENU *myMenu;
	HWND UpAndDown;
	HWND LeftAndRight;
	struct Icon *myIconFrontWindow;
	HDC hdc;
	HFONT hfont;
	HRGN region;
	HACCEL  hAccel;
	PAGESETUPDLG psd;
	HCURSOR hCursorArrow,hCursorWatch,hCursorFiddle,hCursorSelection,hCursorBeam,hCursorPlus;
	char DefaultPaletteName[256];
	unsigned char pal[3*256];
	char paletteName[256];
	int ListenPort;
	int ListenStatus;
	char ListenDirectory[256];
	char ListenPassword[256];
	int SendPort;
	char SendPassword[256];
	char SendTarget[256];
	char ReceiveName[256];
	unsigned char red[256];
	unsigned char green[256];
	unsigned char blue[256];
	unsigned char redOld[256];
	unsigned char greenOld[256];
	unsigned char blueOld[256];
    controlPtr controlList;
	int netErrorCheck;
	int serverSocket;
	int clientSocket;
	int RastorToMemory;
	int RastorToDisk;
	int AppendToMemory;
	int childCount;
	int topWindow;
	struct DumWindow *DWindow;
	int WindowType;
	int FileSave;
	uRect viewRect;
	uRect windowRect;
	uRect eraseRect;
	uRect dataRect;
	int ScreenBits;
	unsigned long c[8];
	int Closing;
	int CursorSpace;
	int (*pdoNULL)(struct Icon *);
	int (*pdoWriteStatus)(struct Icon *,char *);
	int (*pClose)(struct Icon *);
	int (*pUpdateWrite)(struct Icon *);
	int (*pUpdate)(struct Icon *);
	int (*pInContent)(struct Icon *);
	void (*pMoveControls)(struct Icon *);
	int (*pCheckMenu)(struct Icon *,int item);
	int (*pMenu)(struct Icon *,int menu,int item);
	int (*pKeyBoard)(struct Icon *,int);
	int (*pNullEvent)(struct Icon *);
	void (*pPrint)(struct Icon *);
	void (*uDump)(struct Icon *,uRect *PageView,int scale);
	int  (*pGetData)(struct Icon *,long CurrentFrame,struct SetFrameData	*sd);
	int  (*pPutData)(struct Icon *,long CurrentFrame,struct SetFrameData	*sd);
	int  (*pMessage)(struct Icon *,long MessageType,void *MessageData);
	int  (*uDraw)(struct Icon *,long CurrentFrame,uRect *DRect);
	int  (*uDrawAnimate)(struct Icon *,long CurrentFrame);
	int paletteMenuNumber;
	int (*pMouseMoved)(struct Icon *,uPoint pt);
	int  (*pPageUp)(controlPtr control,short part,struct Icon *);
	int  (*pPageDown)(controlPtr control,short part,struct Icon *);
	int  (*pLineUp)(controlPtr control,short part,struct Icon *);
	int  (*pLineDown)(controlPtr control,short part,struct Icon *);
	int  (*pThumb)(controlPtr control,struct Icon *);
	int  (*uWriteCursor)(uPoint pt,struct Icon *);
	int  (*uAddFiles)(struct Icon *myIcon,struct FileInfo2 *Files);
	int  (*uSpeech)(struct Icon *,char *command);
	void (*pDoActivate)(struct Icon *,int flag);

	struct FileListInfo infoLoad;
	struct FileListInfo infoSave;
	struct ScreenInfo Screen;
	struct ScreenInfo ScreenRect;
	struct SaveInformation SaveInfo;
	struct Icon *IconNext;
	struct uMenuList *MenuList;
	struct MenuBar *MenuBarList;
	struct LIMITS limits;
	controlPtr VControl;
	controlPtr HControl;	  
	uRect uviewRect;
	int done;
	int doNotClose;
	int hit;
	int gotname;
	int iread;
	int iwrite;
	int iconnect;
	int iaccept;
	int rd;
	int sd;
	int lineRed;
	int lineGreen;
	int lineBlue;
	int  eraseRectSize;
	int Synchronize;
	double framesPerSecond;
	int WindowClass;
	int WindowNameSet;
	int isOpen;
	HMENU hmenu;	
	int hitem;
	int itWasGood;
	int inThisWindow;
	int iHaveMoved;
	int ControlSpace;
	HBRUSH b;
 	int ForeColorIndex;
 	int BackColorIndex;

	char *cUndo;
	int (*pUndo)(void *);
	int (*eUndo)(void *);

	struct SYSFONTLIST pSysFontList;
	int ThreadId;

	int mMenuOffSet;	    

	int sizeToolBar;	    

	int showToolBar;	    

}Icon,*IconPtr;
	  
IconPtr IconRoot;

#define xViewType		22
#define xRastorType 	23
#define xDialogType 	24
#define xSDSType		25
#define xSliceType		26
#define xRenderType 	27
#define xRenderControlType	28
#define xContourControlType 29
#define xTrueType		30
#define TekType			31
#define xSds2dType		32
#define xEditType		33
#define xFindType		34
#define xChangeType 	35
#define xPio2dType		36
#define xPio3dType		37
#define UtilityType		38
#define Pio2dType		39
#define Pio3dType		40
#define ExpressionType	41
#define Sds3dType		42
#define xStatusType 	43
#define xNetType		44
#define DialogType      45
#define SendWindowType  46
#define NewWindowType   47
#define DumpWindowType  48
#define VolumetricType  49
#define DrawNewType		50


IconPtr CreateMyWindow(int ,uRect *wRect);

IconPtr FindIWindow(HWND w);
IconPtr RemoveIcon(IconPtr i);
int DrawControls(IconPtr myIcon);
int doAbout(IconPtr myIcon);
int doQuit();
int doEvents(void);   
IconPtr FindWindowType(int Type);
int doDestroyWindow(IconPtr win);
int MakeNewDirectory(char *name);
int StandardDoActive(IconPtr myIcon,int flag);

struct DisplayF{
  IconPtr myIcon;
  unsigned char *buffer;
  unsigned short int xsize;
  unsigned short int ysize;
  char **CurrentFileName;
  char *value;
  long CurrentFrame;
  struct paletteDraw pd;
  long top;
  long left;
  double scale;
  int Interpolate;
  uRect imageRect;
  xPoint pt;
  /*
  XColor *colors;
  XImage *xImage;
  */
};

struct SyncDrawInfo{
	uRect DRect;
	uPoint start;
	uPoint current;
};

int SetWindowName(IconPtr myIcon,char *name);
extern void CWindowdoPalMenu(IconPtr myIcon,int menu,int item);
extern int SetPortMyWindow(IconPtr myIcon);
unsigned long int TickCount(void);
int Sleep2(int ms);
int Synchronize(IconPtr myIcon,long CurrentFrame);
extern int drawToScrap(IconPtr myIcon,uRect *Boxi,long CurrentFrame,int flag);
extern int PagePreview(IconPtr myIcon);
extern int Print(IconPtr myIcon);
extern int doSavePlots(IconPtr myIcon,struct FileInfo2 *Files,long CurrentFrame,uRect *DRect); 
int doWindowInformation(IconPtr myIcon);
int pWindowMenu(IconPtr myIcon,int item);
extern void pNewMenu(IconPtr myIcon,int item);
extern void CWindowSetPalMenu(IconPtr myIcon);
extern int doWindowMenu(void);
extern int drawLineOnScreen(IconPtr myIcon,uPoint *p1,uPoint *p2);
extern int drawBoxOnScreen(IconPtr myIcon,uRect *box,int (*uDraw)(IconPtr myIcon,struct SyncDrawInfo *DSync));
int uDrawControls(IconPtr myIcon);
int  nWriteBox(IconPtr myIcon,char *buff);
int ScreenSetPalette(IconPtr myIcon,unsigned char *Red,unsigned char *Green,unsigned char *Blue);
int uScreenWrite(long xsize,long ysize,unsigned char *data,unsigned char *buff,int DataType,
				uRect *SRect,uRect *DRect,IconPtr myIcon);
/*
int  WriteDataBox(IconPtr myIcon,char *buff);
*/
extern IconPtr FindWindowByName(char *name);
extern int mstrcmp(char *out,char *in);
extern int goCD(char *name);
int SetFileType(char *name,char *type);
IconPtr uStartStatusWindow(IconPtr myIcon,char *header);
int uWriteStatusWindow(IconPtr StatusWindow,char *buff);
int uCheckStatusWindow(IconPtr StatusWindow,char *Message);
int uStopStatusWindow(IconPtr StatusWindow);
int SetOldColorPalette(IconPtr myIcon);
int GetOldColorPalette(IconPtr myIcon);


#define xViewType		22
#define xRastorType 	23
#define xDialogType 	24
#define xSDSType		25
#define xSliceType		26
#define xRenderType 	27
#define xRenderControlType	28
#define xContourControlType 29
#define xTrueType		30
#define xTekType		31
#define xSds2dType		32
#define xEditType		33
#define xFindType		34
#define xChangeType 	35
#define xPio2dType		36
#define xPio3dType		37
#define Pio2dType		39
#define Pio3dType		40
#define ExpressionType	41
#define Sds3dType		42
#define RasterType		43
#define TableType		44



#define FILE_NAME_SEPERATOR 		'\\'
#define MenuOffSet		0
#define ScrollOffSet	16
#define Pio2dType		39
#define Pio3dType		40
#define LIST_SORT_ASCII 1

#define AppleMenu	500
#define WindowMenu	5003
#define ToolsNewMenu	114


#include "uLib.h"

double rtime(void);
int Stop(char *message);

int uDrawPalette(struct paletteDraw *pd,IconPtr myIcon);
IconPtr FindWindowInList(IconPtr myIcon);
IconPtr NewCEdit(IconPtr myIconIn,struct FileList *files);
void Bell(int ticks);

int GetSequenceInfo(struct SaveInformation *SaveInfo);

extern int  pDrawGo(struct Icon *,long CurrentFrame,uRect *DRect);

int NetStringToAddr(char *out5, long *ipNum);

EXTERN22 FILE *junk;

int SetWindowToFileName(IconPtr myIcon,struct FileInfo2 *Files);

struct pGlobal{
//    Display *mydisplay;
 	struct FileInfo2 *FilesList[1000];
	int FileOwner[1000];
	long FileCount;
	HINSTANCE hInst;
	int nCmdShow;
	long buffsize;
	char *buffin;
	char *buffout;
	struct timeval NetTime;
	uPoint pt;
}xg;

#endif

