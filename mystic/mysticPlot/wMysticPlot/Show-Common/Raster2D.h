#include "Message.h"

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2

#define USE_TOP_LEFT				4
#define USE_TOP_RIGHT				8
#define USE_BOTTOM_LEFT 			16
#define USE_BOTTOM_RIGHT 			32


#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))



struct stackData{
   long x;
   long y;
   struct PlotRasterData raster;
};


struct ras2dInfo{
    struct SetFrameData sd;    
    struct PlotRangeData range;
    struct PlotVectorData vector;
    struct PlotRasterData raster;
	struct PlotAttributes pa;
    struct paletteDraw pd;
    struct SyncStruct sync;
    struct SyncStruct syncOld;
    char pioName[64];
    int pioIndex;
    
    int noReSizeWindow;

    struct stackData stack;
    pushpopPtr popStack;
    char *CurrentFileName;
    int  LoopBackAndForth;
    long ImageCount;
    long CurrentFrame;
    unsigned char *buffer;
    long length;
    long teklength;
    
    double scale;
    
    long milliSecondsPerFrame;
    unsigned long TimeToFlip;
    short int AnimationOn;
    uPoint pold;
    char BatchName[256];
    struct linedata pdata;
    struct linedata ldata;
    int BoundarySpace;
    int tiltsteps;
    int rotationsteps;
    int drawType;
    int Tool;			/* Tool Type  */
    int LoopBackForth;
    int LoopFlag;
	uRect windowRect;
	int defaultsSet;
	int WalkMotionContinuous;
	int WalkMotion;
	
   	char formatPalette[16];
   	char formatXaxis[16];
   	char formatYaxis[16];
   	
   	int countPalette;
   	int countXaxis;
   	int countYaxis;
   	
   	int showXaxis;
   	int showYaxis;
   	int showPalette;
   	
   	int showXspace;
   	int showYspace;
   	int showYxspace;
	
};

typedef struct ras2dInfo *ras2dPtr;


#define FileMenu4				1803
#define openTekCommand 			1
#define closeTekCommand 		2
#define savePreferencesCommand 	3
#define PreferencesCommand 		4
#define loadTekCommand 			5
#define previewTekCommand 		13
#define setupTekCommand 		14
#define printTekCommand 		15
#define quitTekCommand 			17
#define TekSavePlots			18

#define EditMenu4				1804
#define copyTekCommand 			1
#define clearTekCommand 		2
#define clearLoadTekCommand 	3
#define FrameTekCommand 		4
#define popItem					5
#define popAllItems				6
#define zoomIn					7
#define zoomOut					8
#define Information				10
#define optionsCursor	 		11
#define Magnification	 		12
#define ToolBar	 				14

#define NetMeu4				    1805
#define listenCommand 			1
#define listenSetCommand 		2
#define listenStopCommand 		3
#define netOptionsCommand		4
#define hdfToMemoryCommand		6
#define hdfToDiskCommand		7
#define hdfToAppendCommand		8
#define trueToMemoryCommand		9
#define trueToDiskCommand		10
#define optionsCommand	 		11


#define AnimationMenu4				1806
#define animateTekForward 			1
#define animateTekBackward 			2
#define animateTekStepForward 		3
#define animateTekStepBackward 		4
#define animateTekStop	 			5
#define loopTek	 					7
#define loopBackTek	 				8
#define optionsTek	 				9
#define synchronizeTek	 			10

#define ToolsMenu4					1807
#define drawFramesTek	 			1
#define zoomTek	 					2
#define plotPointsTek	 			3
#define plotLinesTek	 			4
#define RotateObjects	 			5
#define SweepImages	 				6
#define FiddleColors	 			7
#define SyncRegions	 				8
#define zoomTekALL	 				9

#define DataMenu4					1808
#define dataScalesTek	 			1
#define dataRangeTek	 			2
#define dataSelectionPointsTek	 	3
#define dataAxisTek	 				4

#define PalMenu4					1809
#define loadPaletteTek	 			1

#define ToolsNewMenu	114

int RasterNetMenu(IconPtr myIcon,int NetMeu,int item);

