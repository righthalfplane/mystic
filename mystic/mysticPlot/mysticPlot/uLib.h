#ifndef __ULIB__
#define __ULIB__

#include "ulibTypes.h"

#include "ScrollList.h"

extern int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,struct Icon *myIcon);

extern	int uDrawPalette(struct paletteDraw *pd,struct Icon *myIcon);

extern int uGetPort(uPort *Port);

extern int uSetPort(uPort *Port);

extern int uSetRect(uRect *r,int x,int y,int xsize,int ysize);

extern struct Icon *uCreateIcon(int Type,uRect *r);

extern int uGetPortBounds(struct Icon *myIcon,uRect *r);

extern int uGetMaximumBounds(struct Icon *myIcon,uRect *r);

extern int uGetPortBoundsGlobal(struct Icon *myIcon,uRect *r);

extern int uSetWindow(struct Icon *myIcon,...);

extern int ScrollListFitRect(uRect *rect,struct ScrollList *scroll);

extern int uSetCursor(struct Icon *myIcon,int type);

extern int SetWindowName(struct Icon *myIcon,char *name);

extern int GetWindowName(struct Icon *myIcon,char *name,int flag);

extern int uStartCursors(void);

extern int uDrawControls(struct Icon *myIcon);

extern int uDrawGrowIcon(struct Icon *myIcon);

extern int uStillDown(struct Icon *myIcon);

extern struct Icon *NewNewWindow(struct Icon *myIcon);

extern int uDrawGrowIcon(struct Icon *myIcon);

extern int uDrawString(char *buff,struct Icon *myIcon);

extern int uFillRect(uRect *r,struct Icon *myIcon);

extern int uFrameRect(uRect *r,struct Icon *myIcon);

extern int uEraseRect(uRect *r,struct Icon *myIcon);

extern int uClipRect(uRect *r,struct Icon *myIcon);

extern int uGetClipRect(uRect *r,struct Icon *myIcon);

extern int uSetClipRect(uRect *r,struct Icon *myIcon);

extern int uInvertRect(uRect *r,struct Icon *myIcon);

extern int uMemcpy(void *out,void *in,long length);

extern int uPointer(uPoint *p,struct Icon *myIcon);

extern int uPtInRect(uPoint p,uRect *r);

extern int uDoControls(struct Icon *myIcon);

extern int uEnableMenuItem(struct Icon *myIcon,int menu,int item);

extern int uSetMenuItemText(struct Icon *myIcon,int menu,int item,unsigned char *name);

extern int uZeroScrap(void);

extern long uPutScrap(long Count,char *type,char *out);

extern long uGetScrap(char *type,char **data);

extern int uSizeWindow(struct Icon *myIcon,int xsize,int ysize);

extern int uSizeWindowGlobal(struct Icon *myIcon,int xsize,int ysize);

extern int uWriteBox(uRect *r,char *message,struct Icon *myIcon);

extern int uMoveWindow(struct Icon *myIcon,int xsize,int ysize);

extern int uFrontWindow(struct Icon *myIcon);

extern int uDisableMenuItem(struct Icon *myIcon,int menu,int item);

extern int uActivateMenuItem(struct Icon *myIcon,int menu,int item,int value);

extern int uCheckMenuItem(struct Icon *myIcon,int menu,int item,int value);

extern int uInsertMenuItem(menuPtr menu,char *name,int location);

extern long uCountMenuItems(menuPtr menu);

extern int uDeleteMenuItem(menuPtr menu,int location);

extern long putStringToScrap(char *string);

extern char *getStringFromScrap(void);

extern int uHiliteControl(controlPtr control,int value);

extern int uDisposeControl(struct Icon *myIcon,controlPtr control);

extern int uSetControlValue(controlPtr control,long value);

extern int uSetControlMaximum(controlPtr control,long value);

extern long uGetControlValue(controlPtr control);

extern long uGetControlMaximum(controlPtr control);

extern int strncatToPath(char *path,char *name,int length);

struct FileList *uOpenFileDialog(struct Icon *myIcon,int *TypeList);

extern int InvalRectMyWindow(struct Icon *myIcon);

extern int FlushCurrentPort(void);

extern int ZeroScrap(void);

extern double rtime(void);


extern int GetPaletteColors(struct Icon *myIcon,unsigned char *r,unsigned char *g,unsigned char *b);

extern int SetWindowPath(struct Icon *myIconIn,struct Icon *myIcon);

extern int uScrollListFitRect(uRect *r,struct ScrollList *scroll);

extern int uInvalWindowRect(IconPtr myIcon,uRect *r);

extern int uFlushVol(void);

extern int copyInfo(struct FileListInfo *in,struct FileListInfo *out);

extern int setInfo(char *in,struct FileListInfo *out);

void DisableDrawing(void);
void EnableDrawing(void);
void BeginThumbTracking(void);
void EndThumbTracking(void);

#define uWindowSetDone						0
#define uWindowSetMenuList					1
#define uWindowSetWindowName				2
#define uWindowGetMenuAndItem				3
#define uWindowStandardMenu		    		4
#define uWindowSubMenu		    			5
#define uWindowPaletteMenu		    		6
#define uWindowGetMenu		    			7
#define uWindowGetRealMenuAndItem		    8
#define uWindowFontMenu		    			9
#define uWindowStyleMenu		   			10



#define uARROW_CURSOR   		1
#define uWATCH_CURSOR   		2
#define uPLUS_CURSOR    		3
#define uBEAM_CURSOR    		4
#define uSELECTION_CURSOR    	5
#define uFIDDLE_CURSOR    		6
#define uCROSS_CURSOR    		7
#define uSPY_CURSOR    			8
#define uSPYPLUS_CURSOR    		9
#define uSPYMINUS_CURSOR    	10



#endif
