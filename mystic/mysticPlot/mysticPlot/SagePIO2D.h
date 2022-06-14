/*
 *  SagePIO2D.h
 *  mShow
 *
 *  Created by Dale Ranta on 6/7/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */
#include <ctype.h>
#include "Xappl.h"
#include "uLib.h"
#include "uFiles.h"
#include "paletteDraw.h"
#include "Linedata.h"
#include "Message.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Tools.h"
#include "Universal.h"
#include "uGraph.h"
#include "VoiceCommand.h"

struct stackData2{
	long x;
	long y;
	double scale;
	double xminData;
	double xmaxData;
	double yminData;
	double ymaxData;
	int xminSet;
	int xmaxSet;
	int yminSet;
	int ymaxSet;
};



struct pio2dInfo{
	struct BhangMeterData bhangmeter2d;
	struct PlotStreamData streamline2d;
    struct PlotStreamlineData streamline;
    struct PlotVectorData vector;
    struct PlotRangeData range;
	struct PlotAttributes pa;
    struct SetFrameData sd;    
    struct paletteDraw pd;
    
    struct stackData2 stack;
    pushpopPtr popStack;
    char *CurrentFileName;
    long ImageCount;
    long CurrectFrameLoaded;
    long CurrentFrame;
    unsigned char *buffer;
    long length;
    long teklength;
    double scale;
    unsigned long TimeToFlip;
    short int AnimationOn;
    uPoint pold;
    char BatchName[256];
    struct linedata pdata;
    struct linedata ldata;
    
    int initFile;
    
    int Tool;					/* Tool Type  */    
    int BoundarySpace;
	uRect windowRect;
	int defaultsSet;
    int  LoopBackAndForth;
    int LoopFlag;
    char pioName[64];
    int pioIndex;
	int WalkMotionContinuous;
	int WalkMotion;
};

typedef struct pio2dInfo *pio2dPtr;

int doBhangMeter(struct BhangMeterData *streamline2d); 
