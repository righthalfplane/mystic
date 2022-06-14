#ifndef __SPREAD__
#define __SPREAD__

typedef struct Rect{
	int top;
	int left;
	int right;
	int bottom;
} Rect;

typedef struct Point{
	int v;
	int h;
} Point;

struct SpreadStruct{
	struct Icon *myIcon;
    Rect uViewRect;
	controlPtr HControl;
	controlPtr VControl;
	Rect TextRect;
	Rect Select;
	Point EntryLocation;
	Point Anchor;
	Rect TextView;
	Rect viewRect;
	Rect cellRect;
	Rect syncRect;
	long Length;
	int XCount;
	int YCount;
	int XLength;
	int YLength;
	int XScreen;
	int YScreen;
	int XOff;	
	int YOff;	
	double  *DataFloat;
	char *Flags;	
	char *Format1;
	char *Format2;
	char *Format3;
	int BreakFlag;
	void (*pListColumns)(char *out,int i,struct SpreadStruct *wp);
	void (*pListRows)(char *out,int i,struct SpreadStruct *wp);
	void (*pListGet)(char *out,int x,int y,struct SpreadStruct *wp,int clearFlag);
	void (*pListSet)(char *buff,int x,int y,struct SpreadStruct *wp);
	int (*pSyncTrack)(void *data,uRect *r);
	void *pSyncTrackData;
	int ListSubType;
	int RowOffSet;
	int isHidden;
	int eraseRectSize;
	int DoubleClick;
	char out[256];
};

struct SpreadStruct *SpreadNew(uRect *r,int xcount,int ycount,int LineHeight,int LineWidth,
		                       struct Icon *myIcon);
int SpreadUpdate(struct SpreadStruct *Spread);

int SpreadShow(struct SpreadStruct *Spread);

int SpreadHide(struct SpreadStruct *Spread);

int SpreadSetRegion(struct SpreadStruct *Spread,uRect *r);

int SpreadSetSyncTrack(struct SpreadStruct *Spread,int (*pSyncTrack)(void *data,uRect *r),void *data);

int SpreadFitRect(struct SpreadStruct *Spread,uRect *r);

int SpreadConfigure(struct SpreadStruct *Spread,int xcount,int ycount,
                    int LineHeight,int LineWidth,int Type);
                    
int SpreadInContent(struct SpreadStruct *Spread);
                                                
int SpreadCopy(struct SpreadStruct *Spread);
int SpreadPaste(struct SpreadStruct *Spread);
int SpreadCut(struct SpreadStruct *Spread);
int SpreadClear(struct SpreadStruct *Spread);
int SpreadSetCell(struct SpreadStruct *Spread,char *message,int c);
char *SpreadGetCell(struct SpreadStruct *Spread);
                                                
int SpreadDispose(struct SpreadStruct *Spread);
 #define SpreadTypeFloat	0
 

#endif
