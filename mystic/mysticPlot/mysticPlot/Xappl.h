
#ifndef _XAPPL_
#define _XAPPL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "uNetWork.h"
#include "Installed.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>


struct movieStruct{
   int dummmy;
};

typedef struct movieStruct *Movie;

#include "Tools.h"
#include "xAux.h"
#include "SetFrameData.h"
#include "xMenu.h"
#include "fileDialog.h"
#include "paletteDraw.h"
#include "SysFontList.h"

#define MacintoshTextType 	0
#define UnixTextType 		1
#define WindowsTextType 	2

#define DefaultTextType UnixTextType

#define CleanExit


#define upArrow			30
#define downArrow		31
#define leftArrow		28
#define rightArrow		29
#define controlRKey		18



typedef struct uFontStruct {
   int LineHeight;
   int Descent;
   int CWidth;
} uFontInfo;


struct ScreenInfo{
	unsigned char r[256];
	unsigned char g[256];
	unsigned char b[256];	
	unsigned char pal[256*3];	
	
	
	long xsize;
	long ysize;
	unsigned char *buff;
	unsigned char *data;
	int DataType;
	
    XColor colors[256];

    XImage *xImage;
    unsigned char *buffer;
    long length;
    struct Icon *myIcon;
	int hasPalette;
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

#include "Control.h"
struct DumWindow{
	int dum;
};

#define ScreenDataType8 	8
#define ScreenDataType24 	24
#define ScreenDataTypePict 	77



typedef struct Icon {
    struct CursorStruct Cursor;
    struct FileInfo2 *Files;
    struct uNetWork uNet;
    Window  myWindow;
    char WindowName[256];
    char DefaultPaletteName[256];
    Window  *mychild;
    unsigned char pal[3*256];
    char paletteName[256];
	unsigned char red[256];
	unsigned char green[256];
	unsigned char blue[256];
	unsigned char redOld[256];
	unsigned char greenOld[256];
	unsigned char blueOld[256];
    Colormap xColormap;  
    controlPtr controlList;
    int childCount;
    int topWindow;
    struct DumWindow *DWindow;
    int WindowType;
    int FileSave;
    uRect viewRect;
    uRect windowRect;
    uRect eraseRect;
    uRect dataRect;
    int CursorSpace;
    int (*pdoKeyPress)(struct Icon *,XEvent *myevent);
    int (*pdoButtonRelease)(struct Icon *,XEvent *myevent);
    int (*pdoConfigureNotify)(struct Icon *,XEvent *myevent);
    int (*pdoFocusOut)(struct Icon *,XEvent *myevent);
    int (*pdoFocusIn)(struct Icon *,XEvent *myevent);
    int (*pdoLeaveNotify)(struct Icon *,XEvent *myevent);
    int (*pdoEnterNotify)(struct Icon *,XEvent *myevent);
    int (*pdoExpose)(struct Icon *,XEvent *myevent);
    int (*pdoButtonPress)(struct Icon *,XEvent *myevent);
    int (*pdoButtonPress2)(struct Icon *,XEvent *myevent);
    int (*pdoMotionNotify)(struct Icon *,XEvent *myevent);
    int (*pClose)(struct Icon *);
    int (*pCheckMenu)(struct Icon *,int item);
    int (*pUpdateWrite)(struct Icon *);
    int (*pCloseNotify)(struct Icon *,struct Icon *);
    int (*pInfoData)(struct Icon *,struct SetFrameData  *sd);
    int  (*pGetData)(struct Icon *,long CurrentFrame,struct SetFrameData  *sd);
    int  (*pPutData)(struct Icon *,long CurrentFrame,struct SetFrameData  *sd);
    int  (*pSyncRegion)(struct Icon *,uRect *sd);
    int  (*pMessage)(struct Icon *,long MessageType,void *MessageData);
	int (*pMenu)(struct Icon *,int menu,int item);
	int (*pSetWindow)(struct Icon *);
    int  (*pPageUp)(controlPtr control,short part,struct Icon *);
    int  (*pPageDown)(controlPtr control,short part,struct Icon *);
    int  (*pLineUp)(controlPtr control,short part,struct Icon *);
    int  (*pLineDown)(controlPtr control,short part,struct Icon *);
    int  (*pThumb)(controlPtr control,struct Icon *);
    int  (*pdoWriteStatus)(struct Icon *,char *message);

	int  (*uSpeech)(struct Icon *,char *command);

    int (*pMouseMoved)(struct Icon *,uPoint pt);
/*    
    void (*pDump)(struct Icon *,Rect *PageView,int scale);
	int  (*pSyncRegion)(struct Icon *,Rect *sd);
	int  (*pDraw)(struct Icon *,long CurrentFrame,Rect *DRect);
    int  (*pWriteCursor)(Point pt,struct Icon *);
*/	

	void (*pDoActivate)(struct Icon *,int);
	void (*uDump)(struct Icon *,uRect *PageView,int scale);
	int  (*uSyncRegion)(struct Icon *,uRect *sd);
	int  (*uDraw)(struct Icon *,long CurrentFrame,uRect *DRect);
    int  (*uWriteCursor)(uPoint pt,struct Icon *);
	void (*pMoveControls)(struct Icon *);
	int (*pInContent)(struct Icon *);
	int (*pNullEvent)(struct Icon *);
	int (*pKeyBoard)(struct Icon *,int);
	void (*pPrint)(struct Icon *);
	int  (*uDrawAnimate)(struct Icon *,long CurrentFrame);

    int  (*uAddFiles)(struct Icon *,struct FileInfo2 *);
    
    int paletteMenuNumber;

    controlPtr VControl;
    controlPtr HControl;    
    uRect uviewRect;
    Visual *xVisual;
    int ScreenBits;
    unsigned long c[10];
    XEvent *myevent;
    int Closing;
    int done;
    GC gcFont;
    GC gcRubber;
    GC gc;
  
    struct uMenuList *MenuList;
    struct MenuBar *MenuBarList;
    
    struct LIMITS limits;
    
    struct ScreenInfo Screen;

    struct ScreenInfo ScreenRect;

 	int ForeColorIndex;
 	int BackColorIndex;

	struct SaveInformation SaveInfo;
	
	struct FileListInfo infoLoad;
	struct FileListInfo infoSave;
	
	int WindowNameSet;
	
	int isConfigured;
	
	int doNotClose;
	


	int (*pUndo)(void *);
	int (*eUndo)(void *);
	char *cUndo;
	
    int  eraseRectSize;
	double framesPerSecond;
    int Synchronize;
    int WindowClass;
    
    int isOpen;
    
    int inThisWindow;
    
    int iHaveMoved;

	int ControlSpace;
	
    XFontStruct *info;
    
    struct SYSFONTLIST pSysFontList;
	
	Atom	wm_delete_window;
	
	XFontStruct *fontLast;
	
	int mMenuOffSet;
	    
	int showToolBar;
	    
	int sizeToolBar;
	    
    struct Icon *IconNext;
}Icon,*IconPtr;
      
EXTERN22 IconPtr myIcon,IconRoot;


struct DisplayF{
	struct Icon *myIcon;
	XImage *xImage;
	uRect imageRect;
	unsigned char *buffer;
	unsigned short int xsize;
	unsigned short int ysize;
        char **CurrentFileName;
        char *value;
	long CurrentFrame;
	struct paletteDraw pd;
	XColor *colors;
	long top;
	long left;
	double scale;
	int Interpolate;
	uPoint pt;
	long length;
 };



#include "Message.h"
#include "uLib.h"


struct SyncDrawInfo{
    uRect DRect;
    uPoint start;
    uPoint current;
};

extern int drawBoxOnScreen(IconPtr myIcon,uRect *box,int (*uDraw)(struct Icon *,struct SyncDrawInfo *DSync));

int uScreenWrite(long xsize,long ysize,unsigned char *data,unsigned char *buff,int DataType,
                uRect *SRect,uRect *DRect,IconPtr myIcon);

int ScreenSetPalette(IconPtr myIcon,unsigned char *Red,unsigned char *Green,unsigned char *Blue);
    
int ScreenSetSize(long xsize,long ysize,IconPtr myIcon);

extern int doSavePlots(IconPtr myIcon,struct FileInfo2 *Files,long CurrentFrame,uRect *DRect); 


int SendMessageToAll(IconPtr myIcon,int SendToSelf,
                long MessageType,void *message);
     
int RemoveControlFromWindow(Window controlwindow,controlPtr thisControl);

#define xViewType		22
#define xRastorType		23
#define xDialogType		24
#define xSDSType		25
#define xSliceType		26
#define xRenderType		27
#define xRenderControlType	28
#define xContourControlType	29
#define xTrueType		30
#define TekType			31
#define xSds2dType		32
#define xEditType		33
#define xFindType		34
#define xChangeType		35
#define xPio2dType		36
#define xPio3dType		37
#define UtilityType		38
#define Pio2dType		39
#define Pio3dType		40
#define ExpressionType	41
#define Sds3dType		42
#define RasterType		43
#define SendWindowType	44
#define TableType		45
#define DialogType		46
#define NewWindowType	47
#define DumpWindowType	48
#define VolumetricType	50
#define DrawNewType		51
#define Pio3dBklType	52




#define DialogString	0
#define DialogFloat		1
#define DialogInt		2
#define Dialog2Button	3
#define Dialog3Button	4
#define DialogUser		5
#define DialogCheck		6


extern void pNewMenu(IconPtr myIcon,int item);
IconPtr CreateIcon(int Type,uRect *wRect);
IconPtr FindIWindow(Window w);
IconPtr RemoveIcon(IconPtr i);
int uDrawControls(IconPtr myIcon);
int doAbout(IconPtr myIcon);
int doQuit(void);
int doEvents(void);   

int  nWriteBox(IconPtr myIcon,char *buff);

IconPtr FindWindowInList(IconPtr myIcon);
extern int SetPortMyWindow(IconPtr myIcon);

extern int doWindowMenu(void);

int SetWindowName(IconPtr myIcon,char *name);

int doWindowInformation(IconPtr myIcon);

int pWindowMenu(IconPtr myIcon,int item);

int ScreenSetSize(long xsize,long ysize,IconPtr myIcon);

extern void CWindowdoPalMenu(IconPtr myIcon,int menu,short item);

extern void CWindowSetPalMenu(IconPtr myIcon);

extern int Print(IconPtr myIcon);

extern int PageSetup(IconPtr myIcon);

extern int PagePreview(IconPtr myIcon);

extern int drawToScrap(IconPtr myIcon,uRect *Boxi,long CurrentFrame,int flag);

extern int drawLineOnScreen(IconPtr myIcon,uPoint *p1,uPoint *p2);


int StandardDoActive(IconPtr myIcon,int flag);

int Synchronize(IconPtr myIcon,long CurrentFrame);

extern int goCD(char *name);

extern int GetSequenceInfo(struct SaveInformation *SaveInfo);

int putString(unsigned char *data,long length,FILE *out);

extern int  pDrawGo(struct Icon *,long CurrentFrame,uRect *DRect);

extern int  Stop(char *message);

IconPtr OpenTekFiles(struct FileInfo2 *Files);

IconPtr NewCEdit(IconPtr myIconIn,struct FileList *files);

IconPtr uStartStatusWindow(IconPtr myIcon,char *header);
int uWriteStatusWindow(IconPtr StatusWindow,char *buff);
int uCheckStatusWindow(IconPtr StatusWindow,char *Message);
int uStopStatusWindow(IconPtr StatusWindow);
int MakeNewDirectory(char *name);
char *DefaultPathString(void);
double rtime(void);

int WriteToWindow(char *name,char *buff);

extern int WarningBatch(char *message);

extern int WarningBatchFast(char *message);

extern int WarningBatchAtEnd(char *message);

extern int mstrncpy(char *out,char *in,long n);

extern int mstrncat(char *out,char *in,long n);

extern int mstrcmp(char *out,char *in);

extern IconPtr FindWindowByName(char *name);

int SetFileType(char *name,char *type);

int NetStringToAddr(char *out5,unsigned long *ipNum);

extern int WriteStatus(IconPtr myIcon,char *buff);


int SetWindowToFileName(IconPtr myIcon,struct FileInfo2 *Files);


int SetOldColorPalette(IconPtr myIcon);

int GetOldColorPalette(IconPtr myIcon);

int FlushAllEvents(void);

#define MenuOffSet 25
#define ScrollOffSet 16
#define DataSpace		14



#define AppleMenu 	500
#define WindowMenu	5003
#define ToolsNewMenu	114

#define systemFont   	1
#define kFontIDMonaco   2

EXTERN22 struct xGlobal {
 	struct FileInfo2 *FilesList[1000];
	int FileOwner[1000];
	long FileCount;
    Display *mydisplay;
    Colormap xColormapDefault; 
    int installCMap;
    int myscreen;
    unsigned long myforeground;
    unsigned long mybackground;
    Visual *xVisual;
    int vBits;
    int argc;
    char **argv;
    Font font12;
    Font font13;
    Font font14;
    Font font16;
    Font font24;
    XFontStruct *font12info;
    XFontStruct *font13info;
    XFontStruct *font14info;
    XFontStruct *font16info;
    XFontStruct *font24info;
    unsigned long c[10];
    XColor colors[256];
   Cursor Arrow;
   Cursor  Beam;
   Cursor  Watch;
   Cursor  Selector;
   Cursor  Fiddle;
   Cursor  PlusCursor;
   long buffsize;
   unsigned char *buffin;
   unsigned char *buffout;
   int ScreenIsHigh;
   int RedIsLow;
   uPoint pt;
   int rank;
   int size;
   int root;
} xg;


#endif

