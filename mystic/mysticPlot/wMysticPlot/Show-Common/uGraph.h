#ifndef _UGRAPH_
#define _UGRAPH_
#include "paletteDraw.h"
typedef struct iRectStruct{
    int type;
    uRect box;
    unsigned char *data;
} iRect;

typedef struct uPatternStruct{
    unsigned char pat[8];
} uPattern;

int uDrawImageRect(iRect *r,IconPtr myIcon);

int uImageRectStart(iRect *ir,uRect *r);

int uImageRectEnd(iRect *ir);

int uImageRectMerge(iRect *ir,iRect *r);

int uImageRectPattern(iRect *ir,uPattern *pat,IconPtr myIcon);

int uFillRectPattern(uRect *r,uPattern *pat,IconPtr myIcon);

uPattern *PatternPattern(int n);
int PatternCount(void);

int uSetForeByIndex(int icolor,IconPtr myIcon);
int uSetBackByIndex(int icolor,IconPtr myIcon);

int uSetForeColor(int r,int g,int b,IconPtr myIcon);
int uSetBackColor(int r,int g,int b,IconPtr myIcon);
int uPaintBox(uRect *r,struct Icon *myIcon);

int uGetFontInfo(struct Icon *myIcon,uFontInfo *info);

int uSetFontSize(struct Icon *myIcon,int size);

int uSetTextFont(struct Icon *myIcon,int size);

int uSetTextFace(struct Icon *myIcon,int size);

int uSetPenColor(int r,int g,int b,struct Icon *myIcon);

int uSetPenXOR(int flag,struct Icon *myIcon);

int uSetFontSize(struct Icon *myIcon,int size);

int uSetPenWidth(double xsize,double ysize,IconPtr myIcon);

int uSetPenPattern(unsigned char *pattern,IconPtr myIcon);

int uMoveTo(int x,int y);
int uLineTo(int x,int y,struct Icon *myIcon);
int uPaintRect(uRect *r,IconPtr myIcon);


int uLineU(int x,int y,int nc,struct screenData *s);
int uMoveU(int x,int y,struct screenData *s);
int uDrawStringU(char *out,int nc,int fn,struct screenData *s);
int uDrawStringAngleU(char *out,double angle,int nc,int fn,struct screenData *s);
int uPaintRectU(uRect *r,struct screenData *s);
int uPaintOvalU(uRect *r,struct screenData *s);
int uFrameOvalU(uRect *r,struct screenData *s);

int uPaintRoundRectU(uRect *r,int ovalWidth,int ovalHeight,struct screenData *s);
int uFrameRoundRectU(uRect *r,int ovalWidth,int ovalHeight,struct screenData *s);
		 
extern int uLineTo(int ix,int iy,struct Icon *myIcon);
extern int uMoveTo(int ix,int iy);

int LineC(int x1,int y1,int x2,int y2,int nc,struct screenData *s);
int LineB(int x,int y,int nc,struct screenData *s);
int kpoint(int x,int y,int nc,struct screenData *s);
int MoveB(int x,int y);
int FindB(int *x,int *y);

int uPointerGlobal(uPoint *uwhere,IconPtr myIcon);

int getTrueFromScrap(long *xsize,long *ysize,unsigned char **image);

int convertTO24(struct SetFrameData  *sd,struct paletteDraw *pd);

int GetPaletteByName(char *name,unsigned char *r,unsigned char *g,unsigned char *b);

int SetDefaultPaletteName(IconPtr myIcon,char *name1);
int GetDefaultPaletteName(IconPtr myIcon,char *name1);

extern int DefaultFont;
extern int DefaultFontSize;

#endif
