#ifndef __TABLE__
#define __TABLE__
#include "Spread.h"

struct TableWindowStruct{
	struct SpreadStruct *Spread;
    struct SetFrameData sd;    
    struct PlotRangeData range;
    struct PlotVectorData vector;
    struct PlotRasterData raster;
    struct FileInfo2 *Files;
	struct PlotAttributes pa;
    struct paletteDraw pd;
    struct SyncStruct sync;
    struct SyncStruct syncOld;
    char BatchName[256];
    char pioName[64];
    int pioIndex;
    long CurrentFrame;
    long ImageCount;
	int XCount;
	int YCount;
	int XLength;
	int YLength;
	int Tool;
    short int AnimationOn;
    short int LoopBackForth;
    short int LoopFlag;
    int  LoopBackAndForth;
    long milliSecondsPerFrame;
    unsigned long TimeToFlip;
};

typedef struct TableWindowStruct *TablePtr;


IconPtr  NewTableWindow(IconPtr myIconIn);

IconPtr  TableWindow(struct FileInfo2 *Files2);
#endif
