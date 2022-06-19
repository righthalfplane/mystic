#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uLib.h"
#include "uFiles.h"
#include "paletteDraw.h"
#include "Linedata.h"
#include "Message.h"
#include "uDialog.h"
#include "SaveAllImages.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Universal.h"
#include "uGraph.h"
#include "VoiceCommand.h"
#include "initPIOBlocked.h"

#define X_PLANE	0
#define Y_PLANE	1
#define Z_PLANE	2
#define G_PLANE	3

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2

/*
#define CONTROL_SCALE  	16
#define CONTROL_SCALEO  0.0625
*/

#define CONTROL_SCALE  	1
#define CONTROL_SCALEO 1


#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))

static void tMoveControls2(IconPtr myIcon);

static int adjustWindow(IconPtr myIcon);

int freeStreamLine3d(struct PlotStreamData *streamline3d);

int writeCursorString(uRect *dataRect,char *buffout,IconPtr myIcon);

static int getCursorSting(char *buffout,long sizebuff,uPoint *pt,IconPtr myIcon);

int	uGetCursorItemsString(char *buffout,long sizebuff,IconPtr myIcon,long nn);

static int getCursorStringLineCount(IconPtr myIcon);

int uCStringWidth(char *name,IconPtr myIcon);

int uWriteCursorItems(IconPtr myIcon,long nn,uRect *dataRect2);

int doDataDump(IconPtr myIcon,struct FileInfo2 *Files);

int doCursorItems(char *BatchName,struct CursorStruct *Cursor,IconPtr myIcon);

int doGetCursorData(IconPtr myIcon,char *BatchName,struct SetFrameData  *sd);

static int PIOPutData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd);

int doFiddleColors(IconPtr myIcon);

struct FileInfo2 *FilesSDS3D(void);

int doSage3DStart(struct FileInfo2 *Files);

int doSDS3DStart(struct FileInfo2 *Files);

int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

static int SendStreamlineInfo(IconPtr myIcon); 

int doTestSpeed(IconPtr myIcon);


char *DefaultPathString(void);

static int doPointPlotDown(IconPtr myIcon);

static int doLinePlot(uPoint p1,uPoint p2,IconPtr myIcon);

static int doLinePlotDown(IconPtr myIcon);

IconPtr CDrawNewLine(struct linedata *li,int count);

#define PIO_AREA_FILL 1
int SynchronizePalettes(IconPtr myIcon,unsigned char *red,unsigned char *green,unsigned char *blue);

static int SetRange(IconPtr myIcon,double xmin,double xmax,double ymin,double ymax,int flag,int flag2);

static int PIO3dGetMessage(IconPtr myIcon,long MessageType,void *MessageData);



struct stackData3{
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
   
   
   
   long ixmax;
   long iymax;

   long ixminData;
   long ixmaxData;
   long iyminData;
   long iymaxData;
   
   int CursorSpace;
   
};

struct pio3dInfo{
	struct PlotStreamData streamline3d;
    struct PlotVectorData vector;
    struct PlotRangeData range;
    struct PlotPlaneData plane;
	struct PlotAttributes pa;
    struct paletteDraw pd;
    
    pushpopPtr popStack;
    struct stackData3 stack;
    char *CurrentFileName;
    long ImageCount;
    long CurrentFrame;    
    double scale;
    unsigned long TimeToFlip;
    short int AnimationOn;    
    uPoint pold;
    struct linedata pdata;
    struct linedata ldata;
    char BatchName[256];
    struct SetFrameData  sd;
    
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
    
    
    long CurrectFrameLoaded;
    
};


typedef struct pio3dInfo *pio3dPtr;

struct FileInfo2 *FilesOpenList(struct FileList *FilesIn);

int freeFileList(struct FileInfo2 *Files);


static void drawIt(IconPtr myIcon);


static int SetFrame(IconPtr myIcon);

static void tMoveControls(IconPtr myIcon);
static int tInContent(IconPtr myIcon);
static int doClose(IconPtr myIcon);
static int UpdateWrite(IconPtr myIcon);


int doEraseDown(IconPtr myIcon);
int doMouseDown(IconPtr myIcon);
static int doSwitchPlanes(IconPtr myIcon);
static int doStreamLines(IconPtr myIcon);
static int doMouseZoom(IconPtr myIcon);
static int push(uRect *oldR,int iflag,IconPtr myIcon);

static int CheckMenu(IconPtr myIcon,int flag);
static int Menu(IconPtr myIcon,int menu,int item);
static int doAnimateMenu(IconPtr myIcon,int item);
static int KeyBoard(IconPtr myIcon,int c);
static int n2NullEvent(IconPtr myIcon);
static void doFrameImage(IconPtr myIcon);

/*
static int RealLoc(IconPtr myIcon,double *x,double *y,int *ix,int *iy);
*/

static int IntLoc(IconPtr myIcon,double *x,double *y,int ix,int iy);
static int IntLocC(IconPtr myIcon,double *x,double *y,int ixx,int iyy);


static int tdothumb(controlPtr control,IconPtr myIcon);
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int tgoUp(controlPtr control,short part,IconPtr myIcon);
static int tgoDown(controlPtr control,short part,IconPtr myIcon);
static int tMoveDy(IconPtr myIcon);

int doCursorDisplay(IconPtr myIcon);

#define FileMenu4				3003
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

#define EditMenu4				3004
#define copyTekCommand 			1
#define clearTekCommand 		2
#define clearLoadTekCommand 	3
#define clearLinesTekCommand 	4
#define FrameTekCommand 		5
#define popItem					6
#define popAllItems				7
#define zoomIn					8
#define zoomOut					9
#define Information				10

#define AnimationMenu4				3005
#define animateTekForward 			1
#define animateTekBackward 			2
#define animateTekStepForward 		3
#define animateTekStepBackward 		4
#define animateTekStop	 			5
#define loopTek	 					7
#define loopBackTek	 				8
#define optionsTek	 				9
#define synchronizeTek	 			10
#define optionsCursor	 			11
#define itemsCursor	 				12

#define ToolsMenu4					3006
#define drawFramesTek	 			1
#define zoomTek	 					2
#define plotPointsTek	 			3
#define plotLinesTek	 			4
#define SweepImages	 				5
#define FiddleColors	 			6
#define switchPlanesTek	 			7
#define streamLines	 				8

#define DataMenu4					3007
#define dataScalesTek	 			1
#define dataRangeTek	 			2
#define dataSelectionPointsTek	 	3
#define vectorDialog	 	        4
#define dataDumpDialog	 	        5
#define dataSelectionPlaneTek	 	6
#define streamlineDialog	 	    7
#define bhangmeterDialog	 	    8


#define PalMenu4					3008
#define loadPaletteTek	 			1


IconPtr NewPIO3DWindow(IconPtr myIcon);

int doMouseSweepDown(IconPtr myIcon);

IconPtr OpenPIO3DFiles(struct FileInfo2 *Files);

static int PIO3dGetData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd);

static int pioGetValue(IconPtr myIcon,double x,double y,double *v,long *nn);
static int pushStack(IconPtr myIcon);

static int writeCursor(uPoint pt,IconPtr myIcon);

static int drawTek(IconPtr myIcon,long CurrectFrame,uRect *DRect);
int PageSetup(IconPtr myIcon);
static void tDumpItP(IconPtr myIcon,uRect *PageView,int scale);
static void tPrint(IconPtr myIcon);
void cLineSet(int num,int dem);

static int nMouseMoved(IconPtr myIcon,uPoint where);

int FilesOpenSDS3D(struct FileInfo2 *files);

static int Count;

int FilesOpenPIO3D(struct FileInfo2 *files);

static int OpenPIO3D(struct FileList *FilesIn);

static int DrawAnimate(IconPtr myIcon,long CurrentFrame);

static void DoActivate(IconPtr myIcon,int flag);
static int SpeechCommand(IconPtr myIcon,char *command);

static struct VoiceCommands Commands[]=
{
	{"Animate Forward",c_Animate_Forward},
	{"Animate Backward",c_Animate_Backward},
	{"Animate Faster",c_Animate_Faster},
	{"Animate Slower",c_Animate_Slower},
	
	{"Zoom In",c_Zoom_In},
	{"Zoom Out",c_Zoom_Out},
	
	{"Move Up",c_Move_Up},
	{"Move Down",c_Move_Down},
	{"Move Left",c_Move_Left},
	{"Move Right",c_Move_Right},
	
	{"Move Forward",c_Move_Forward},
	{"Move Backward",c_Move_Backward},
	
	{"Move Faster",c_Animate_Faster},
	{"Move Slower",c_Animate_Slower},
		
	{"Step Forward",c_Step_Forward},
	{"Step Backward",c_Step_Backward},
	
	{"Stop",c_Stop},
	
	{"Continue",c_Start},
	
	{"Halt",c_Stop},
	{"Help",c_Help},
};
static long CommandLength=sizeof(Commands)/sizeof(struct VoiceCommands);


static int doCommands(IconPtr myIcon,int c);

int checkStreamData(struct dataStruct *plane);

static void DoActivate(IconPtr myIcon,int flag)
{
	pio3dPtr t;
	
	if(!myIcon)return;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return;

	StandardDoActive(myIcon,flag);
	
	if(flag){
		startLanguage(Commands,CommandLength,myIcon);
	}else{
		stopLanguage();
	}

}
static int doBhangMeterStart(IconPtr myIcon)
{
	static struct linedata li;

	pio3dPtr r;
	
	if(!myIcon)return 1;
	r=(pio3dPtr)myIcon->DWindow;
	if(!r)return 1;	
	
	
	if(li.CurrentFrame < 0){
	    li.CurrentFrame=0;
	    li.FirstFrame=0;
	    li.LastFrame=r->ImageCount-1;
	    li.FirstTime=0;
	    li.LastTime=(double)(r->ImageCount-1);
	    li.FrameCount=r->ImageCount;	    
	    li.PointLineCount=200;
	}
	li.type=LINEDATA_BHANGMETER_DATA;
	GetWindowName(myIcon,li.sourceName,0);
	li.Plot=NULL;
	li.CurrentFrame=r->CurrentFrame;
	li.replaceData = -1;
	
	li.pioIndex=r->pioIndex;
	mstrncpy(li.pioName,r->pioName,255);
	
	if(CDrawNewLine(&li,1))return 1;
	
	return 0;
}
static int SpeechCommand(IconPtr myIcon,char *command)
{
	long n;
	
	if(!myIcon || !command)return 1;
	
	sprintf(WarningBuff,"Voice Command = %s\n",command);
	WarningBatch(WarningBuff);
	
	if(!mstrcmp(command,"help")){
		for(n=0;n<CommandLength;++n){
			sprintf(WarningBuff,"Voice Commands[%ld] = %s\n",n,Commands[n].CommandText);
			WarningBatch(WarningBuff);
		}
		return 0;
	}
	
	for(n=0;n<CommandLength;++n){
		if(!strcmp(command,Commands[n].CommandText)){
			doCommands(myIcon,Commands[n].CommandIndex); 
		}
	}
	
	return 0;
}
static int doCommands(IconPtr myIcon,int c)
{
	struct PlotPlaneData *pio;
	pio3dPtr r;
	
	if(!myIcon)return 1;
	r=(pio3dPtr)myIcon->DWindow;
	if(!r)return 1;	
	pio = &r->plane;
	
	switch(c){	
	    case c_Animate_Forward:
	    	r->WalkMotion = 0;
	        doAnimateMenu(myIcon,animateTekForward);
	    break;
	    case c_Animate_Backward:
	    	r->WalkMotion = 0;
	        doAnimateMenu(myIcon,animateTekBackward);
	    break;
	    case c_Animate_Faster:
	    	myIcon->framesPerSecond *= 2;
	    break;
	    case c_Animate_Slower:
	    	myIcon->framesPerSecond /= 2;
	    break;
	    case c_Step_Forward:
	        doAnimateMenu(myIcon,animateTekStop);
	        doAnimateMenu(myIcon,animateTekStepForward);
	    break;
	    case c_Step_Backward:
	        doAnimateMenu(myIcon,animateTekStop);
	        doAnimateMenu(myIcon,animateTekStepBackward);
	    break;
	    case c_Zoom_In:
	        Menu(myIcon,EditMenu4,zoomIn);
	    break;
	    case c_Zoom_Out:
	        Menu(myIcon,EditMenu4,zoomOut);
	    break;
	    case c_Move_Left:
	    	r->WalkMotion = c_Move_Left;
	    	tgoUp(myIcon->HControl,0,myIcon);
	    break;
	    case c_Move_Right:
	    	r->WalkMotion = c_Move_Right;
	    	tgoDown(myIcon->HControl,0,myIcon);
	    break;
	    case c_Move_Up:
	    	r->WalkMotion = c_Move_Up;
	    	tgoUp(myIcon->VControl,0,myIcon);
	    break;
	    case c_Move_Down:
	    	r->WalkMotion = c_Move_Down;
	    	tgoDown(myIcon->VControl,0,myIcon);
	    break;
	    	    
	    case c_Move_Forward:	    
	    	r->WalkMotion = c_Move_Forward;
		    if(pio->Plane_type == X_PLANE){
		        pio->Plane_xloc=pio->Plane_xloc+(pio->Plane_xmax-pio->Plane_xmin)/pio->Plane_xcount;
		        if(pio->Plane_xloc > pio->Plane_xmax){
	    			r->WalkMotionContinuous = 0;	    	
		            pio->Plane_xloc=pio->Plane_xmax;
		        }
		    }else if(pio->Plane_type == Y_PLANE){
		        pio->Plane_yloc=pio->Plane_yloc+(pio->Plane_ymax-pio->Plane_ymin)/pio->Plane_ycount;
		        if(pio->Plane_yloc > pio->Plane_ymax){
	    			r->WalkMotionContinuous = 0;	    	
		            pio->Plane_yloc=pio->Plane_ymax;
		        }
		    }else if(pio->Plane_type == Z_PLANE){
		        pio->Plane_zloc=pio->Plane_zloc+(pio->Plane_zmax-pio->Plane_zmin)/pio->Plane_zcount;
		        if(pio->Plane_zloc > pio->Plane_zmax){
	    			r->WalkMotionContinuous = 0;	    	
		            pio->Plane_zloc=pio->Plane_zmax;
		        }
		    }	    	    
		    drawIt(myIcon);
	    break;
	    
	    case c_Move_Backward:
	    	r->WalkMotion = c_Move_Backward;
		    if(pio->Plane_type == X_PLANE){
		        pio->Plane_xloc=pio->Plane_xloc-(pio->Plane_xmax-pio->Plane_xmin)/pio->Plane_xcount;
		        if(pio->Plane_xloc < pio->Plane_xmin){
	    			r->WalkMotionContinuous = 0;	    	
		            pio->Plane_xloc=pio->Plane_xmin;
		        }
		    }else if(pio->Plane_type == Y_PLANE){
		        pio->Plane_yloc=pio->Plane_yloc-(pio->Plane_ymax-pio->Plane_ymin)/pio->Plane_ycount;
		        if(pio->Plane_yloc < pio->Plane_ymin){
	    			r->WalkMotionContinuous = 0;	    	
		            pio->Plane_yloc=pio->Plane_ymin;
		        }
		    }else if(pio->Plane_type == Z_PLANE){
		        pio->Plane_zloc=pio->Plane_zloc-(pio->Plane_zmax-pio->Plane_zmin)/pio->Plane_zcount;
		        if(pio->Plane_zloc < pio->Plane_zmin){
	    			r->WalkMotionContinuous = 0;	    	
		            pio->Plane_zloc=pio->Plane_zmin;
		        }
		    }	    	    
		    drawIt(myIcon);
	    break;
	    
	    case c_Start:
	    	r->WalkMotionContinuous = 1;	 
	   	break;   	
	    case c_Stop:
	    	r->WalkMotionContinuous = 0;	    	
	        doAnimateMenu(myIcon,animateTekStop);
	    break;
	}
	return 0;
	
}
int FilesOpenSDS3D(struct FileInfo2 *files)
{
	if(!files)return 1;
	if(!OpenPIO3DFiles(files))return 1;
	return 0;
}

int FilesOpenPIO3D(struct FileInfo2 *files)
{
	if(!files)return 1;
	if(!OpenPIO3DFiles(files))return 1;
	return 0;
}

static int OpenPIO3D(struct FileList *FilesIn)
{
	    struct FileInfo2 *Files;
	    IconPtr myIcon;
		
		Files=FilesOpenList(FilesIn);
		if(!Files)return 1;
		
		myIcon=OpenPIO3DFiles(Files);
		if(myIcon)return 0;
		return 1;
		
}

IconPtr NewPIO3DWindow(IconPtr myIcon)
{
	//myIcon=myIcon;
	return OpenPIO3DFiles(NULL);
}

static int SetPIO3DFiles(IconPtr myIcon,struct FileInfo2 *Files)
{
	pio3dPtr t;
	    
	if(!myIcon)return 1;
	
    if(!(t=(pio3dPtr)cMalloc((long)sizeof(struct pio3dInfo),8183))){
        Warning("Out of List Memory");
		return 1;
    }
    zerol((char *)t,sizeof(struct pio3dInfo));
    
    myIcon->DWindow=(struct DumWindow *)t;
    	    
	myIcon->pClose=doClose;
	myIcon->pUpdateWrite=UpdateWrite;
	myIcon->pInContent=tInContent;
	myIcon->pMoveControls=tMoveControls2;
	myIcon->pCheckMenu=CheckMenu;
	myIcon->pMenu=Menu;
	myIcon->pKeyBoard=KeyBoard;
	myIcon->pNullEvent=n2NullEvent;
	myIcon->pPrint=tPrint;
	myIcon->uDump=tDumpItP;
	myIcon->pGetData=PIO3dGetData;
	myIcon->pMessage=PIO3dGetMessage;
	myIcon->uDraw=drawTek;
	myIcon->paletteMenuNumber=PalMenu4;
	myIcon->pMouseMoved=nMouseMoved;
	myIcon->uDrawAnimate=DrawAnimate;
	
	myIcon->pPutData=PIOPutData;
	myIcon->pMouseMoved=nMouseMoved;
	myIcon->uDrawAnimate=DrawAnimate;
	
	myIcon->pPageUp=tdoPageUp;
	myIcon->pPageDown=tdoPageDown;
	myIcon->pLineUp=tgoUp;
	myIcon->pLineDown=tgoDown;
	myIcon->pThumb=tdothumb;
			
	myIcon->uWriteCursor=writeCursor;

	myIcon->uSpeech=SpeechCommand;	
	myIcon->pDoActivate=DoActivate;
    
	t->range.ixmax=532;
	t->range.iymax=432;
	
	t->range.AspectRatio=1.0;
	t->range.MaintainAspectRatio=1;
	t->range.receiveRange=1;
	
	t->BoundarySpace=0;
	
	t->pd.sType=0;
	t->pd.sPmin=0;
	t->pd.sPmax=1;	
	t->pd.LabeledPalette=FALSE;	
	t->pd.paletteFont=2;
	t->pa.ZoneLevel=1;
	
	t->pdata.CurrentFrame= -1;
	t->ldata.CurrentFrame= -1;
	t->CurrectFrameLoaded = -1;
	
	myIcon->Cursor.valueFlag=1;
	mstrncpy(myIcon->Cursor.valueformat,"%10.3e",8);
	mstrncpy(myIcon->Cursor.coordinateFormat,"%10.3e",8);
	mstrncpy(myIcon->Cursor.timeFormat,"%8.2e",8);
	
	t->scale=1.0;
	
	myIcon->framesPerSecond=30;
	t->CurrentFrame=0;
	t->Tool=zoomTek;
	
	myIcon->mMenuOffSet=MenuOffSet;
	
	myIcon->CursorSpace=DataSpace;
	myIcon->eraseRectSize=120;
	
	t->streamline3d.streamSteps=2000;
	t->streamline3d.streamGradients=1;

	
	if(Files){
	    if(Files->fileClass == SDS3D_FILES){ 
	    	doSDS3DStart(Files);
	    }else if(Files->fileClass == PIO3D_FILES) {
	    	doSage3DStart(Files);
	    }else if(Files->fileClass == PIO3D_BLK_FILES) {
	    	doSage3DBlkStart(Files);
	    }
	}
	return 0;
}
static int initPIO3d(IconPtr myIcon)
{
	pio3dPtr s;
	int ret;
	
	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;
	
	if(s->initFile)return 0;
	
	ret=1;
	
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->plane=s->sd.plane;
	s->range=s->sd.range;
	s->vector=s->sd.vector;
	s->pd=s->sd.pd;
	s->pa=s->sd.pa;
	if(s->sd.pioName[0]){
		mstrncpy(s->pioName,s->sd.pioName,255);
		s->pioIndex=s->sd.pioIndex;			    
	}		

	s->initFile=TRUE;
	
	ret=0;

ErrorOut:	
	return ret;
}
IconPtr OpenPIO3DFiles(struct FileInfo2 *Files)
{
	static struct uMenuItems File[]={
	    {"File",  					FileMenu4,uWindowStandardMenu,0,},
	    {"New           /N",		openTekCommand,0,0,},
	    {"Close         /W",		closeTekCommand,0,0,},
	    {"----------------", 		openTekCommand,0,0,},
	    {"Save Preferences",		savePreferencesCommand,0,0,},
	    {"Preferences...  ",		PreferencesCommand,0,0,},
	    {"----------------", 		openTekCommand,0,0,},
	    {"Load Images...  ",		loadTekCommand,0,0,},
	    {"Save Images...  ",		TekSavePlots,0,0,},
	    {"----------------", 		openTekCommand,0,0,},
	    {"Print Preview...",		previewTekCommand,0,0,},
	    {"Page Setup...",			setupTekCommand,0,0,},
	    {"Print...      /P",		printTekCommand,0,0,},
	    {"----------------", 		openTekCommand,0,0,},
	    {"Quit          /Q",		quitTekCommand,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Edit[]={
	    {"Edit",EditMenu4,uWindowStandardMenu,0,},
	    {"Copy            /C",copyTekCommand,0,0,},
	    {"Clear           /K",clearTekCommand,0,0,},
	    {"Clear and Load... ",clearLoadTekCommand,0,0,},
	    {"Clear Lines       ",clearLinesTekCommand,0,0,},
	    {"------------------",openTekCommand,0,0,},
	    {"Frame Image     /F",FrameTekCommand,0,0,},
	    {"Pop One           ",popItem,0,0,},
	    {"Pop All           ",popAllItems,0,0,},
	    {"Zoom In           ",zoomIn,0,0,},
	    {"Zoom Out          ",zoomOut,0,0,},
	    {"------------------",openTekCommand,0,0,},
	    {"Information...  /I",Information,0,0,},
	    {"Cursor Displays...",	optionsCursor,0,0,},	    
	    {"Cursor Items...   ",itemsCursor,0,0,},	  
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Animation[]={
	    {"Animation",  			AnimationMenu4,uWindowStandardMenu,0,},
	    {"Forward", 			animateTekForward,0,0,},
	    {"Backward",			animateTekBackward,0,0,},
	    {"Step Forward",		animateTekStepForward,0,0,},
	    {"Step Backward",		animateTekStepBackward,0,0,},
	    {"Stop",				animateTekStop,0,0,},
	    {"-------------------",	openTekCommand,0,0,},
	    {"Loop",				loopTek,0,0,},
	    {"Loop Back and Forth",	loopBackTek,0,0,},
	    {"Options...",			optionsTek,0,0,},
	    {"Synchronize",			synchronizeTek,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Tools[]={
	    {"Tools",  				ToolsMenu4,uWindowStandardMenu,0,},
	    {"Drag Frames", 			drawFramesTek,0,0,},
	    {"Zoom",					zoomTek,0,0,},
	    {"Point Plots",				plotPointsTek,0,0,},
	    {"Line Plots",				plotLinesTek,0,0,},
	    {"Sweep Images",			SweepImages,0,0,},	    
	    {"Fiddle Colors",			FiddleColors,0,0,},	    
	    {"Scroll Layers",			switchPlanesTek,0,0,},	    
	    {"Streamlines",				streamLines,0,0,},	    
	    { NULL,0,0,0},
	};
	
	static struct uMenuItems Data[]={
	    {"Data",  				DataMenu4,uWindowStandardMenu,0,},
	    {"Data Range...", 		dataScalesTek,0,0,},
	    {"Data Domain...",		dataRangeTek,0,0,},
	    {"Data Selection...",	dataSelectionPointsTek,0,0,},
	    {"Data Vectors...",		vectorDialog,0,0,},
	    {"Data Dump...",	    dataDumpDialog,0,0,},	    
	    {"Data Plane...",		dataSelectionPlaneTek,0,0,},
	    {"Data Streamlines...",	streamlineDialog,0,0,},
	    {"Data Bhangmeter...",	bhangmeterDialog,0,0,},	    
	    { NULL,0,0,0},
	};
	
	static struct uMenuItems Palette[]={
	    {"Palette",  				    PalMenu4,uWindowPaletteMenu,0,},
	    {"Load Palette...", 			1,0,0,},
	    {"Save Palette...", 			2,0,0,},
	    {"Add Palette...", 				3,0,0,},
	    {"Restore Palette   /R", 		4,0,0,},
	    {"Load Menu Palette...", 		5,0,0,},
	    {"Synchronize Palettes", 		6,0,0,},
	    {"Palette To Table", 		    7,0,0,},
	    {"--------------------", 		8,0,0,},
	    { NULL,0,0,0},
	};
	
		
	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Animation},
	    {Tools},
	    {Data},
	    {Palette},
	    {NULL}
	};
	
	uRect Rect1={40,40,548,462};
	    pio3dPtr t;
	    IconPtr myIcon;
		char buff[256];
		int ret;
		
		
	    ret=1;
		t=NULL;
		
		if(!(myIcon=uCreateIcon(Pio3dType,&Rect1))){
			WarningBatch("OpenPIO3DFiles uCreateIcon Failed\n");
			goto ErrorOut;
		}
		
		sprintf(buff,"PIO3D_%03d",Count++);
		SetWindowName(myIcon,buff);	
	    
		SetWindowName(myIcon,buff);	
		if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,
	              uWindowSetDone,uWindowSetDone)
				  )goto ErrorOut;
	    
	    
		if(SetPIO3DFiles(myIcon,Files))goto ErrorOut;		
		t=(pio3dPtr)myIcon->DWindow;
		if(!t)goto ErrorOut;
		
		t->popStack=pushpopStart(&t->stack);
		if(!t->popStack)goto ErrorOut;

		if(Files){
		    mstrncat(buff,".b",255);
		    mstrncpy(t->BatchName,buff,255);
		    if(FileManagerAddToList(Files,t->BatchName))goto ErrorOut;
		    t->ImageCount=Files->ImageCount;
		    myIcon->Files=Files;
	    	Files=NULL;
		    
		    initPIO3d(myIcon);
		    
			if(ScreenSetSize(t->range.ixmax,t->range.iymax,myIcon))goto ErrorOut;		    
		}else{
			if(ScreenSetSize(Rect1.xsize,Rect1.ysize,myIcon))goto ErrorOut;
		}
	    

	    if(!IsShift()){
	    	doUniversalSagePIO3D(myIcon,universal_GetDefaults,NULL);
	    	FlushCurrentPort();
	    }
	    
		SetFrame(myIcon);
		doFrameImage(myIcon);
	    InvalRectMyWindow(myIcon);
	    
    	SetWindowToFileName(myIcon,myIcon->Files);
    	
    	StandardDoActive(myIcon,TRUE);
    	
		GetPaletteByName("Ps",myIcon->red,myIcon->green,myIcon->blue);
		SetDefaultPaletteName(myIcon,"Ps");

		tMoveControls(myIcon);
		
	    InvalRectMyWindow(myIcon);
		
	    CheckMenu(myIcon,TRUE);

		if(myIcon->Files){
		    setInfo(myIcon->Files->directory,&myIcon->infoLoad);
		    setInfo(myIcon->Files->directory,&myIcon->infoSave);
		}
		    	    	    		
	    ret=0;
ErrorOut:
		if(ret){
			if(Files)freeFileList(Files);
			if(myIcon)doClose(myIcon);
			myIcon=NULL;
		}else{
			if(myIcon)myIcon->isOpen=TRUE;
		}
		return myIcon;
}
static int DrawAnimate(IconPtr myIcon,long CurrentFrame)
{
	pio3dPtr s;
	uPort save;
	
	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;

	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	if(s->CurrentFrame != CurrentFrame){
	    s->CurrentFrame=CurrentFrame;
	    SetFrame(myIcon);
	}
		
	drawIt(myIcon);	
		
	uSetPort(&save);
	return 0;
}
static int PIO3dGetData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	pio3dPtr t;
	int ret;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	
	sd->CurrentFrame=CurrentFrame;
	
	ret=SendMessageByName(t->BatchName,MessageType_GET_REGION_DATA,sd);

	myIcon->limits=sd->limits;
	
	return ret;
	

}
static int PIOPutData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	pio3dPtr t;
	uPort save;
	int ret;

	if(!myIcon || !sd)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	
	ret=1;
	uGetPort(&save);
	
	SetPortMyWindow(myIcon);

	if(sd->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"PIOPutData - Only Puts FRAME_DATA_FLOAT\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	sd->CurrentFrame=CurrentFrame;
	if(SendMessageByName(t->BatchName,MessageType_SET_REGION_DATA,sd)){	        	        
	    sprintf(WarningBuff,"PIOPutData - Window %s Error Puting Data\n",t->BatchName);
	    WarningBatch(WarningBuff);
	   goto ErrorOut;
	}

	t->CurrentFrame=CurrentFrame;

	ret=0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
static int PIO3dGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct PlotRangeData *pio;
	struct SetFrameData  *sd;
	struct Message1000 *m;
	struct linedata *li;
	struct areadata *ai;
	struct LIMITS *limits;
	struct LIMITS *limitl;
	char windowName[256];
	char buff[256];
	uPort save;
	pio3dPtr s;
	int ret;
	long n;
	int setSDS3D;

	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;
	pio = &s->range;
	m=(struct Message1000 *)MessageData;


	setSDS3D=FALSE;
	if(MessageType == MessageType_SET_REGION_DATA){
	    if(!myIcon->Files){
        	myIcon->Files=FilesSDS3D();
        	if(!myIcon->Files)return 1;        	
			sprintf(buff,"PIO3D_%03d",Count++);			
			mstrncat(buff,".b",255);        	
			if(FileManagerAddToList(myIcon->Files,buff))return 1;
		    mstrncpy(s->BatchName,buff,255);
			setSDS3D=TRUE;
	    }	
	}

	if(!myIcon->Files)return 1;

	ret=1;
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	switch(MessageType){
	case MessageType_LIMITS:
	    if(!pio->receiveRange)goto OK;
	    if(!m)goto ErrorOut;
	    
		if(pushStack(myIcon)){
		    goto ErrorOut;
		}	    
	    
	    pio->MaintainAspectRatio=m->MaintainAspectRatio;
	    pio->ixmax=m->ixmax;
	    pio->iymax=m->iymax;	    
	    if(ScreenSetSize(pio->ixmax,pio->iymax,myIcon))goto ErrorOut;
	    pio->xminData=m->xminData;
	    pio->xmaxData=m->xmaxData;
	    pio->yminData=m->yminData;
	    pio->ymaxData=m->ymaxData;
	    pio->ixminData=m->ixminData;
	    pio->ixmaxData=m->ixmaxData;
	    pio->iyminData=m->iyminData;
	    pio->iymaxData=m->iymaxData;

	    adjustWindow(myIcon);
	    
		break;
	case MessageType_SET_FRAME_NUMBER:
	    if(!myIcon->Synchronize)goto OK;
	    s->CurrentFrame=m->CurrentFrame;
	    SetFrame(myIcon);
	    break;
	case MessageType_GET_FRAME_COUNT:
	    if(myIcon->Files){
		    if(myIcon->Files->ImageCount > m->FrameCount)m->FrameCount=myIcon->Files->ImageCount;
		}
		goto OK;
	case MessageType_SET_PALETTE:
		for(n=0;n<256;++n){
		    myIcon->red[n]=m->red[n];
		    myIcon->green[n]=m->green[n];
		    myIcon->blue[n]=m->blue[n];
		}
		break;
	case MessageType_GET_LINE_DATA:
	    li=(struct linedata *)MessageData;
	    ret = SendMessageByName(s->BatchName,MessageType_GET_LINE_DATA,li);
	    goto ErrorOut;
	case MessageType_GET_AREA_DATA:
	    ai=(struct areadata *)MessageData;
		ret=SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,ai);
		myIcon->limits=myIcon->Files->limits;
	    goto ErrorOut;
	case MessageType_GET_CELL_DATA:
	    sd=(struct SetFrameData  *)MessageData;
		ret=SendMessageByName(s->BatchName,MessageType_GET_CELL_DATA,sd);
	    goto ErrorOut;
	case MessageType_GET_REGION_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(myIcon->pGetData){
	    	ret = (*myIcon->pGetData)(myIcon,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;
	case MessageType_SET_AREA_RANGE:
	    ai=(struct areadata *)MessageData;
	    ret = SendMessageByName(s->BatchName,MessageType_SET_AREA_RANGE,ai);
		myIcon->Files->xminr=s->plane.Plane_xmin=s->plane.Plane_xloc=ai->xminArea;
		myIcon->Files->xmaxr=s->plane.Plane_xmax=ai->xmaxArea;
		myIcon->Files->yminr=s->plane.Plane_ymin=s->plane.Plane_yloc=ai->yminArea;
		myIcon->Files->ymaxr=s->plane.Plane_ymax=ai->ymaxArea;
		myIcon->Files->zminr=s->plane.Plane_zmin=s->plane.Plane_zloc=ai->zminArea;
		myIcon->Files->zmaxr=s->plane.Plane_zmax=ai->zmaxArea;	
		s->plane.Plane_gloc=s->plane.Plane_gmin=ai->xminArea;
		s->plane.Plane_gmax=ai->xmaxArea;
		doFrameImage(myIcon);		
	    goto ErrorOut;
	case MessageType_SET_REGION_DATA:
	    sd=(struct SetFrameData  *)MessageData;
	    if(myIcon->pPutData){
	    	ret = (*myIcon->pPutData)(myIcon,sd->CurrentFrame,sd);
	    	if(ret){
	    	   ret = -1;
	    	}    	
			if(!ret && setSDS3D){
				doSDS3DStart(myIcon->Files);
				initPIO3d(myIcon);
				s->ImageCount=myIcon->Files->ImageCount;	    	
				doFrameImage(myIcon);
			}
			
			s->ImageCount=myIcon->Files->ImageCount;	    	
	    }
	    goto ErrorOut;
	case MessageType_GET_LIMITS_DATA:
	    limitl=(struct LIMITS *)MessageData;
	    if(!limitl)goto ErrorOut;
	    limits=(struct LIMITS *)NULL;
		FileGetLimits(&limits,limitl->CurrentFrame,myIcon->Files);
		if(limits && limits->iGot){
		    limits->CurrentFrame=limitl->CurrentFrame;		
		    limits->type=limitl->type;    		    
		    *limitl = *limits;
		}
	    if(limitl->type == LIMIT_TYPE3D){
			limitl->xmin=s->plane.Plane_xmin;
			limitl->ymin=s->plane.Plane_ymin;
			limitl->zmin=s->plane.Plane_zmin;
			limitl->xmax=s->plane.Plane_xmax;
			limitl->ymax=s->plane.Plane_ymax;
			limitl->zmax=s->plane.Plane_zmax;
	    }
	    break;
	case MessageType_GET_FILE_LIST:
	    {
		    int isFound;
		    
			if(!myIcon->Files)goto OK;;
			
		    isFound=FALSE;
			for(n=0;n<m->FileCount;++n){
			    if(myIcon->Files == m->FilesList[n]){isFound=TRUE;break;}
			}
			if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
			    m->FilesList[m->FileCount++]=myIcon->Files;
			}
		}
		goto OK;
	case MessageType_GET_FILE_OWNER:
		for(n=0;n<m->FileCount;++n){
		    if(myIcon->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
		}
		goto OK;
	case MessageType_PLOT_LINE_DATA:
	    SendMessageByName(s->BatchName,MessageType_PLOT_LINE_DATA,m);
	    break;
	case MessageType_PRINT_FILE_INFORMATION:
		GetWindowName(myIcon,windowName,0);
	    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
	    WarningBatch(WarningBuff);
	    SendMessageByName(s->BatchName,MessageType_PRINT_FILE_INFORMATION,m);
		goto OK;
	}
    s->CurrectFrameLoaded = -1;
	/* InvalRectMyWindow(myIcon); */
	drawIt(myIcon);
OK:
	ret = 0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
static int adjustWindow(IconPtr myIcon)
{
	struct PlotRangeData *pio;
	int BoundarySpace;
	int CursorSpace;
	pio3dPtr s;
	int itry;
	
	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;
	pio = &s->range;
	
    pio->xminSet=1;
    pio->xmaxSet=1;
    pio->yminSet=1;
    pio->ymaxSet=1;
    
    
    
	if(pushStack(myIcon)){
	    return 1;
	}
	
	
    SetFrame(myIcon);
    
    itry=0;
    
    do {
    
	    CursorSpace=getCursorStringLineCount(myIcon)*DataSpace;
		if(CursorSpace <= 0)CursorSpace=DataSpace;
	    
    	uSizeWindow(myIcon,(int)(pio->ixmax+ScrollOffSet),(int)(pio->iymax+myIcon->mMenuOffSet+ScrollOffSet+CursorSpace));	
    	
    	FlushCurrentPort();
    	
    	myIcon->CursorSpace=getCursorStringLineCount(myIcon)*DataSpace;
		if(myIcon->CursorSpace <= 0)myIcon->CursorSpace=DataSpace;
	
	}while((myIcon->CursorSpace != CursorSpace) && (itry++ < 2));


	BoundarySpace=s->BoundarySpace;
		    	    
    s->BoundarySpace = -1;
    
    tMoveControls(myIcon);
    
    s->BoundarySpace = BoundarySpace;

    s->CurrectFrameLoaded = -1;
	
	return 0;
}
static int doOptions(IconPtr myIcon)
{
 	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",      {40,95,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {240,95,65,25},uDialogButton,uDialogNone},
      /*  2 */      {"Frames/sec data", {160,45,120,25},uDialogText,uDialogGetDouble},
      /*  3 */      {"Frames/sec :",     {35,45,105,25},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,339,151};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	pio3dPtr t;
    int reti;
    int ret;
	
	
	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	reti=uDialogSet(uList,2L,
	              	              
	              uDialogSetDouble,(double)myIcon->framesPerSecond,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
		
    if(uR[2].dreturn <= 0.0){
		Warning("Frames/sec must be greater than Zero");
		goto Start;
	}
			
	myIcon->framesPerSecond=uR[2].dreturn;
	
	ret = 0;
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int writeCursor(uPoint pt,IconPtr myIcon)
{
	char buffout[512];
	
	if(!myIcon)return 1;
	
	buffout[0]=0;
	
	getCursorSting(buffout,sizeof(buffout),&pt,myIcon);
	
	writeCursorString(&myIcon->dataRect,buffout,myIcon);
	
  	return 0;

}
static int getCursorSting(char *buffout,long sizebuff,uPoint *pt,IconPtr myIcon)
{
   	struct LIMITS *limits;
    struct FileInfo2 *Files;
    static char *planelist[]={"","x-s","y-s","z-s","g-s",};
	struct PlotPlaneData *pio;
	double depth;
	int nplane;
	char buff[256];
    char namex[32],namey[32],namev[32],namet[32],named[32];
    char *name;
	pio3dPtr t;
	double x,y,v;
	long nn;
	long n;

	if(!myIcon || !buffout)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
  	if(!Files)return 1;
	pio=&t->plane;
  	
  	v=0;
	x=0;
	y=0;
	depth=0.0;
	nplane=0;
    if(pio->Plane_type == X_PLANE){
		nplane=1;
		depth=pio->Plane_xloc;
    }else if(pio->Plane_type == Y_PLANE){
		nplane=2;
		depth=pio->Plane_yloc;
    }else if(pio->Plane_type == Z_PLANE){
		nplane=3;
		depth=pio->Plane_zloc;
    }
  	
  	if(pt){
	    IntLocC(myIcon,&x,&y,pt->x,pt->y);
	}else{
	    limits=&t->sd.limits;
	    x=0.5*(limits->xmin+limits->xmax);
	    y=0.5*(limits->ymin+limits->ymax);
	}
	
	sprintf(namex,myIcon->Cursor.coordinateFormat,x);	    
	sprintf(namey,myIcon->Cursor.coordinateFormat,y);	    
	sprintf(named,myIcon->Cursor.coordinateFormat,depth);	    
	sprintf(namet,myIcon->Cursor.timeFormat,Files->pioTime);	
		
	nn = -1L;	    	 
	if(pioGetValue(myIcon,x,y,&v,&nn) || !pt){
	
	    if(myIcon->Cursor.valueFlag == 0){
			n=(long)v;
	        sprintf(namev,"%12ld",n);
	    }else{
	    	sprintf(namev,myIcon->Cursor.valueformat,v);	    
	    }
	
	    if(t->pioName[0]){
	        name=t->pioName;
	    }else{
	    	name="v";
	    }
	    
	    sprintf(buff,"%s=%s%c",name,namev,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    
	    sprintf(buff,"x=%s%c",namex,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    
	    sprintf(buff,"y=%s%c",namey,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    	    
	    sprintf(buff,"%s=%s%c",planelist[nplane],named,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);	    
	    
	    sprintf(buff,"t=%s%c",namet,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    	    	    
	}else{
	    sprintf(buff,"x=%s%c",namex,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    
	    sprintf(buff,"y=%s%c",namey,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    	    
	    sprintf(buff,"%s=%s%c",planelist[nplane],named,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);	    
	    
	    sprintf(buff,"t=%s%c",namet,0);
	    mstrncat(buffout,buff,sizebuff);
	    mstrncat(buffout,"#",sizebuff);
	    
	    return 0;
	}
		
	uGetCursorItemsString(buffout,sizebuff,myIcon,nn);
	
  	return 0;

}

static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
    struct FileInfo2 *Files;
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
  	if(!Files){
	    uSetCursor(myIcon,uARROW_CURSOR);
  	    return 1;
	}
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {	
	    if(t->Tool == drawFramesTek){
		   uSetCursor(myIcon,uARROW_CURSOR);
	    }else if(t->Tool == zoomTek){
		   uSetCursor(myIcon,uPLUS_CURSOR);
	    }else if(t->Tool == plotPointsTek){
	       uSetCursor(myIcon,uPLUS_CURSOR);
	    }else if(t->Tool == plotLinesTek){
	       uSetCursor(myIcon,uPLUS_CURSOR);
	    }else if(t->Tool == streamLines){
	       uSetCursor(myIcon,uPLUS_CURSOR);
	    }else if(t->Tool == FiddleColors){
	       uSetCursor(myIcon,uFIDDLE_CURSOR);
	    }else{
		   uSetCursor(myIcon,uARROW_CURSOR);
	    }
		writeCursor(pt,myIcon);
	}else{	
		writeCursor(pt,myIcon);
		uSetCursor(myIcon,uARROW_CURSOR);	
	}		
	return 0;	
}
static int n2NullEvent(IconPtr myIcon)
{
    struct FileInfo2 *Files;
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
  	if(!Files)return 1;
	

	if((t->WalkMotion > 0) && t->WalkMotionContinuous){
		if(TickCount() > t->TimeToFlip){
			doCommands(myIcon,t->WalkMotion);
       		t->TimeToFlip = TickCount() + (long int)(60./myIcon->framesPerSecond);
		}
		return 0;
	}
	
    if(!(t->AnimationOn) || !(t->ImageCount))return 1;
    
    if(TickCount() > t->TimeToFlip){
       t->CurrentFrame += t->AnimationOn;
       if(t->CurrentFrame >= t->ImageCount){
           if(t->LoopBackAndForth){
               t->CurrentFrame=t->ImageCount-1;
               t->AnimationOn = -1;
           }else{
               t->CurrentFrame=0;
           }
       }else if(t->CurrentFrame < 0){
           if(t->LoopBackAndForth){
               t->CurrentFrame=0;
               t->AnimationOn = 1;
           }else{
               t->CurrentFrame=t->ImageCount-1;
           }
       }
       t->TimeToFlip = TickCount() + (long int)(60./myIcon->framesPerSecond);
	   SetFrame(myIcon);
       drawIt(myIcon);
       
       if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
	       
		if(!t->LoopFlag){
		    if(t->AnimationOn > 0){	        
			    if(t->CurrentFrame+1 == t->ImageCount){
	                    t->AnimationOn=FALSE;
			    }
		    }else if(!t->CurrentFrame){ 
			    t->AnimationOn=FALSE;
		    }
		}
	}
	return 0;
}
static int KeyBoard(IconPtr myIcon,int c)
{
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;

	switch(c){
	    case upArrow:
	        doAnimateMenu(myIcon,animateTekStop);
	        doAnimateMenu(myIcon,animateTekStepForward);
	        break;
	    case downArrow:
	        doAnimateMenu(myIcon,animateTekStop);
	        doAnimateMenu(myIcon,animateTekStepBackward);
	        break;
	    case leftArrow:
	        doAnimateMenu(myIcon,animateTekBackward);
	        break;
	    case rightArrow:
	        doAnimateMenu(myIcon,animateTekForward);
	        break;
	    case 't': 
	         doTestSpeed(myIcon);	    
	         break;
	    case 'n': 
	         break;
	   default:
	        doAnimateMenu(myIcon,animateTekStop);
	        break;
	}
	if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
	return 0;
}
static int doAnimateMenu(IconPtr myIcon,int item)
{
	pio3dPtr t;


	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	
	t->WalkMotionContinuous = 0;	
	
	switch(item){
	    case animateTekForward:
	        t->AnimationOn = 1;
	        break;
	    case animateTekBackward:
	        t->AnimationOn = -1;
	        break;
	    case animateTekStepForward:
	        if(++(t->CurrentFrame) >= t->ImageCount)t->CurrentFrame = 0;
			SetFrame(myIcon);
	        InvalRectMyWindow(myIcon);

	        t->AnimationOn = 0;
	        break;
	    case animateTekStepBackward:
	        if(--(t->CurrentFrame) < 0)t->CurrentFrame = t->ImageCount-1;
			SetFrame(myIcon);
	        InvalRectMyWindow(myIcon);

	        t->AnimationOn = 0;
	        break;
	    case animateTekStop:
	        t->AnimationOn = 0;
	        break;
	    case 7:
	        t->LoopFlag = !t->LoopFlag;
	        break;
	    case 8:
	        t->LoopBackAndForth = !t->LoopBackAndForth;
	        break;
 		case 9:
 		 	doOptions(myIcon);
  		 	break;
 		case 10:
 		 	myIcon->Synchronize = !myIcon->Synchronize;
  		 	break;
	}
	return 0;
}
static int doClear(IconPtr myIcon)
{
	uRect portRect;
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	
	uGetPortBounds(myIcon,&portRect);
	uClipRect(&portRect,myIcon);
	uEraseRect(&portRect,myIcon);
	
	if(t->BatchName[0])FileManagerCloseName(t->BatchName);
	
	t->BatchName[0]=0;
	
	myIcon->Files=NULL;
	t->CurrentFileName = NULL;
	t->ImageCount=0;
	t->pdata.CurrentFrame = -1;
	t->ldata.CurrentFrame = -1;
	t->CurrectFrameLoaded = -1;
	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return 0;
}

static int n2SaveScrap(IconPtr myIcon)
{
	uRect Box;
	pio3dPtr t;


	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	
	
  	if(!t->ImageCount)return 1;
  	
  	Box.x = 0;
  	Box.y = 0;
  	Box.xsize=(int)((double)t->range.ixmax*t->scale);
  	Box.ysize=(int)((double)t->range.iymax*t->scale);
  	
  	
	return drawToScrap(myIcon,&Box,t->CurrentFrame,1);
}
static int doLoadImages(IconPtr myIcon,struct FileList *Files)
{
	struct FileInfo2 *Files2;
	int TypeList[]={SAGE3D_BLOCKED,SAGE3D_DISK,SDS3D_MEMORY,SDS3D_DISK,0};
	char buff[256];
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;

	if(!Files){
		Files=uOpenFileDialog(myIcon,TypeList);		
		if(!Files)return 1;		
	}
		
	if(myIcon->Files)return OpenPIO3D(Files);

	Files2=FilesOpenList(Files);
	if(!Files2)return 1;
	myIcon->Files=Files2;
	t->ImageCount=Files2->ImageCount;

	sprintf(buff,"PIO3D_%03d",Count++);			
	mstrncat(buff,".b",255);
	mstrncpy(t->BatchName,buff,255);
	if(FileManagerAddToList(Files2,t->BatchName)){
	    Warning("doLoadImages  Error FileManagerAddToList");
	    doClear(myIcon);
	    return 1;
	}

	if(!t->initFile){
	    doUniversalSagePIO3D(myIcon,universal_SetDefaults,NULL);
	    initPIO3d(myIcon);
	    doUniversalSagePIO3D(myIcon,universal_GetDefaults,NULL);
		doFrameImage(myIcon);
	}
	
    SetWindowToFileName(myIcon,myIcon->Files);
    	
	SetFrame(myIcon);
	
	tMoveControls(myIcon);
	
	InvalRectMyWindow(myIcon);
	    
	return 0;
}
static int Menu(IconPtr myIcon,int menu,int item)
{
	struct PlotRangeData *pio;
  	struct PlotRangeData range;
	uRect Box;
	int plane;
	int type;
	int ido;

	pio3dPtr t;


	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	pio = &t->range;
	
	Box.x = 0;
	Box.y = 0;
	Box.xsize=(int)((double)t->range.ixmax*t->scale);
	Box.ysize=(int)((double)t->range.iymax*t->scale);
	
	uSetCursor(myIcon,-uWATCH_CURSOR);
	
	switch(menu){
	case AppleMenu:
		if(item == 1){
		    doAbout(myIcon);
		    break;
		}
		break;
	case PalMenu4:
		CWindowdoPalMenu(myIcon,menu,item);
		break;
	case FileMenu4:
		if(item == closeTekCommand){
		    if(myIcon && myIcon->pClose){
		        (*myIcon->pClose)(myIcon);
		    }
		    myIcon=NULL;
		} else if(item == savePreferencesCommand){		
	    	writeUniversal(myIcon,0);
		} else if(item == PreferencesCommand){		
			saveUniversal(myIcon);
		} else if(item == loadTekCommand){
		    doLoadImages(myIcon,NULL);
		} else if(item == previewTekCommand){
			PagePreview(myIcon);
		} else if(item == setupTekCommand){
			PageSetup(myIcon);
		} else if(item == printTekCommand){
		    Print(myIcon);
		} else if(item == openTekCommand){
		    NewNewWindow(myIcon);
		} else if(item == quitTekCommand){
		   doQuit();
		    myIcon=NULL;
		} else if(item == TekSavePlots){
			doSavePlots(myIcon,myIcon->Files,t->CurrentFrame,&Box);
		}
				    
		break;
		
	case DataMenu4:
	    /* UpdateWrite(myIcon); */
		switch(item){
 		case dataScalesTek:
  		    type = MessageType_DO_SCALES_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&t->sd)){
				goto OutOfHere;
			}
			if(t->pd.LabeledPalette != t->sd.pd.LabeledPalette){
			    ido=TRUE;
			}else{
			    ido=FALSE;
			}
			t->pd=t->sd.pd;
			t->pa=t->sd.pa;
			if(ido)doFrameImage(myIcon);
			t->CurrectFrameLoaded = -1;
  		    break;
 		case dataRangeTek:
  		    type = MessageType_DO_RANGE_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&t->sd)){
				goto OutOfHere;
			}
			t->range=t->sd.range;
			
			adjustWindow(myIcon);

		 	if(ScreenSetSize(t->range.ixmax,t->range.iymax,myIcon))goto OutOfHere;
			t->CurrectFrameLoaded = -1;
		 	
  		    break;
  		case dataSelectionPointsTek:	  
  		    type = MessageType_DO_SELECTION_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&t->sd)){
				goto OutOfHere;
			}
			
			if(t->sd.pioName[0]){
	    		mstrncpy(t->pioName,t->sd.pioName,255);
	    		t->pioIndex=t->sd.pioIndex;			    
			    if(!t->pa.DrawZones)t->pd.AreaFill=TRUE;
			}						
			t->CurrectFrameLoaded = -1;
			
  		    break;
  		     
  		case vectorDialog:	  
  		    type = MessageType_DO_VECTOR_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&t->sd)){
				goto OutOfHere;
			}
			t->vector=t->sd.vector;
			t->CurrectFrameLoaded = -1;
  		    break;
  		    
   		case dataDumpDialog:	
   		    doDataDump(myIcon,myIcon->Files);  
  		    break;
			
			case bhangmeterDialog:	
				doBhangMeterStart(myIcon);
				t->CurrectFrameLoaded = -1;
				break;  
						     
  		case dataSelectionPlaneTek:	 
		    plane=t->plane.Plane_type;
  		    type = MessageType_DO_PLANE_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&t->sd)){
				goto OutOfHere;
			}
			t->plane=t->sd.plane;
			range=t->range;
			t->range=t->sd.range;
			adjustWindow(myIcon);
			t->pd=t->sd.pd;
			if(plane != t->plane.Plane_type){
			    if(range.xminData == t->range.xminData && range.xmaxData == t->range.xmaxData &&
			       range.yminData == t->range.yminData && range.ymaxData == t->range.ymaxData){
			    	doFrameImage(myIcon);
			    }
			}
			if(myIcon->Files){
			    if(t->plane.Plane_type == X_PLANE){
					if(t->plane.Plane_xloc < myIcon->Files->xminr)t->plane.Plane_xloc=myIcon->Files->xminr;
					if(t->plane.Plane_xloc > myIcon->Files->xmaxr)t->plane.Plane_xloc=myIcon->Files->xmaxr;
			    }else if(t->plane.Plane_type == Y_PLANE){
					if(t->plane.Plane_yloc < myIcon->Files->yminr)t->plane.Plane_yloc=myIcon->Files->yminr;
					if(t->plane.Plane_yloc > myIcon->Files->ymaxr)t->plane.Plane_yloc=myIcon->Files->ymaxr;
			    }else if(t->plane.Plane_type == Z_PLANE){
					if(t->plane.Plane_zloc < myIcon->Files->zminr)t->plane.Plane_zloc=myIcon->Files->zminr;
					if(t->plane.Plane_zloc > myIcon->Files->zmaxr)t->plane.Plane_zloc=myIcon->Files->zmaxr;
			    }
		    }
			t->CurrectFrameLoaded = -1;
  		    break;
  		case streamlineDialog:	  
  		    type = MessageType_DO_STREAMLINE_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&t->sd)){
				goto OutOfHere;
			}
			
			t->streamline3d.streamOn=t->sd.streamline3d.streamOn;
			t->streamline3d.streamGradients=t->sd.streamline3d.streamGradients;
			t->streamline3d.streamSteps=t->sd.streamline3d.streamSteps;
			t->streamline3d.streamPasses=t->sd.streamline3d.streamPasses;
		
			t->CurrectFrameLoaded = -1;

  		    break;
		}
		SetFrame(myIcon);
	    InvalRectMyWindow(myIcon);
		break;
		
	case EditMenu4:
		
		switch(item){
 		case copyTekCommand:
 			n2SaveScrap(myIcon);
 		    break;
 		case clearTekCommand:
 			doClear(myIcon);
 		    break;
 		case clearLoadTekCommand:
 		
 			{
			    int *LaserPlotList;
			    long LaserPlotCount;
			    
			    int *TracerPlotList;
			    long TracerPlotCount;
			    
			    int ret;
 				
 				LaserPlotList=NULL;
 				LaserPlotCount=0;
 				
 				TracerPlotList=NULL;
 				TracerPlotCount=0;
 				
	  		    if(myIcon->Files){
 		    		LaserPlotList=myIcon->Files->pioData.LaserPlotList;
 		    		LaserPlotCount=myIcon->Files->pioData.LaserPlotCount;
 		    		myIcon->Files->pioData.LaserPlotList=NULL;
 		        	myIcon->Files->pioData.LaserPlotCount=0;
 		    		TracerPlotList=myIcon->Files->pioData.TracerPlotList;
 		    		TracerPlotCount=myIcon->Files->pioData.TracerPlotCount;
 		    		myIcon->Files->pioData.TracerPlotList=NULL;
 		        	myIcon->Files->pioData.TracerPlotCount=0;
	 		    } 			
 				doClear(myIcon);
				ret=doLoadImages(myIcon,NULL);
	  		    if(myIcon->Files || !ret){
		    		myIcon->Files->pioData.LaserPlotList=LaserPlotList;
		        	myIcon->Files->pioData.LaserPlotCount=LaserPlotCount;
		    		myIcon->Files->pioData.TracerPlotList=TracerPlotList;
		        	myIcon->Files->pioData.TracerPlotCount=TracerPlotCount;
	 		    }else{
 		    		if(LaserPlotList)cFree((char *)LaserPlotList);	 		        
 		    		if(TracerPlotList)cFree((char *)TracerPlotList);
	 		    }			
			}
 		    break;
 		case clearLinesTekCommand:
 		    if(myIcon->Files){
 		        if(myIcon->Files->pioData.LaserPlotCount && myIcon->Files->pioData.LaserPlotList){
 		    		cFree((char *)myIcon->Files->pioData.LaserPlotList);
 		    		myIcon->Files->pioData.LaserPlotList=NULL;
 		        	myIcon->Files->pioData.LaserPlotCount=0;
 		        }
 		        if(myIcon->Files->pioData.TracerPlotCount && myIcon->Files->pioData.TracerPlotList){
 		    		cFree((char *)myIcon->Files->pioData.TracerPlotList);
 		    		myIcon->Files->pioData.TracerPlotList=NULL;
 		        	myIcon->Files->pioData.TracerPlotCount=0;
 		        }
	    		freeStreamLine3d(&t->streamline3d);
 				SendStreamlineInfo(myIcon);   	
		    }
 		    InvalRectMyWindow(myIcon);
			t->CurrectFrameLoaded = -1;
 		    break;
 		     		    
 		case FrameTekCommand:
 		doFrameImage(myIcon);
 		    break;
 		case popItem:
 		   if(pushpopPop(&t->stack,t->popStack))goto OutOfHere;
POP:
				t->scale=t->stack.scale;

				myIcon->CursorSpace=t->stack.CursorSpace;
				
				if(pio->ixmax != t->stack.ixmax || pio->iymax != t->stack.iymax){
				    pio->ixmax = t->stack.ixmax;
				    pio->iymax = t->stack.iymax;
				    
					uSizeWindow(myIcon,(int)(pio->ixmax+ScrollOffSet),(int)(pio->iymax+myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace));	

					FlushCurrentPort();
		  		}
				pio->xmaxData=t->stack.xmaxData;
				pio->ymaxData=t->stack.ymaxData;
				pio->xminData=t->stack.xminData;
				pio->yminData=t->stack.yminData;
				pio->xminSet=t->stack.xminSet;
				pio->xmaxSet=t->stack.xmaxSet;
				pio->yminSet=t->stack.yminSet;
				pio->ymaxSet=t->stack.ymaxSet;

	    		pio->ixminData=t->stack.ixminData;
	    		pio->ixmaxData=t->stack.ixmaxData;
	    		pio->iyminData=t->stack.iyminData;
	    		pio->iymaxData=t->stack.iymaxData;

				tMoveControls(myIcon);

				uSetControlValue(myIcon->VControl,t->stack.y);
				uSetControlValue(myIcon->HControl,t->stack.x);

				InvalRectMyWindow(myIcon);
				t->CurrectFrameLoaded = -1;

 			 break;
 		case popAllItems:
 		   if(pushpopPopAll(&t->stack,t->popStack))goto OutOfHere;
		   goto POP;
 		case zoomIn:	
		    if(pushStack(myIcon)){
				break;
		    }

		    pio->xmaxData = pio->xminData+.5*(pio->xmaxData-pio->xminData);
		    pio->ymaxData = pio->yminData+.5*(pio->ymaxData-pio->yminData);

		    pio->xminSet=1;
		    pio->xmaxSet=1;
		    pio->yminSet=1;
		    pio->ymaxSet=1;

			tMoveControls(myIcon);
		    uSetControlValue(myIcon->VControl,(long)((double)(t->stack.y)*t->scale/t->stack.scale));
		    uSetControlValue(myIcon->HControl,(long)((double)(t->stack.x)*t->scale/t->stack.scale));
 		    InvalRectMyWindow(myIcon);
			t->CurrectFrameLoaded = -1;

 		 break;
 		case zoomOut:	
		    if(pushStack(myIcon)){
				break;
		    }

			
	    	pio->xmaxData = pio->xminData+2.*(pio->xmaxData-pio->xminData);
	    	pio->ymaxData = pio->yminData+2.*(pio->ymaxData-pio->yminData);
		    
		    pio->xminSet=1;
		    pio->xmaxSet=1;
		    pio->yminSet=1;
		    pio->ymaxSet=1;

			tMoveControls(myIcon);
		    uSetControlValue(myIcon->VControl,(long)((double)(t->stack.y)*0.5));
		    uSetControlValue(myIcon->HControl,(long)((double)(t->stack.x)*0.5));
 		   	InvalRectMyWindow(myIcon);
			t->CurrectFrameLoaded = -1;

 		 break;
 		case Information:
 		   {
				uRect r1,r2;
	 			uGetPortBoundsGlobal(myIcon,&r1);
			    doWindowInformation(myIcon);
	 			uGetPortBoundsGlobal(myIcon,&r2);
	 		    if(r1.xsize != r2.xsize || r1.ysize != r2.ysize){
	 		        tMoveControls(myIcon);
	 		    }
				t->CurrectFrameLoaded = -1;
		   }
 		    break;
		 
 		case optionsCursor:
 		 	doCursorDisplay(myIcon);
			t->CurrectFrameLoaded = -1;
  		 	break;

 		case itemsCursor:
 		 	doCursorItems(t->BatchName,&myIcon->Cursor,myIcon);
			tMoveControls(myIcon);
			t->CurrectFrameLoaded = -1;
  		 	break;  		 	  		 	  		 	
		}
		
		break;
	case AnimationMenu4:
		doAnimateMenu(myIcon,item);
		if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
		break;
	case ToolsMenu4:
		if(item == drawFramesTek){
	    	t->Tool=drawFramesTek;
		}else if(item == zoomTek){
	    	t->Tool=zoomTek;
		}else if(item == plotPointsTek){
	    	t->Tool=plotPointsTek;
		}else if(item == plotLinesTek){
	    	t->Tool=plotLinesTek;
		}else if(item == switchPlanesTek){
	    	t->Tool=switchPlanesTek;
		}else if(item == SweepImages){
	    	t->Tool=SweepImages;
		}else if(item == FiddleColors){
	    	t->Tool=FiddleColors;
		}else if(item == streamLines){
	    	t->Tool=streamLines;
		}
		t->CurrectFrameLoaded = -1;
		break;

	case WindowMenu:
		pWindowMenu(myIcon,item);
		break;
		
	case ToolsNewMenu:
		pNewMenu(myIcon,item);
		break;
		
	}
	

OutOfHere:
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int CheckMenu(IconPtr myIcon,int flag)
{
    struct FileInfo2 *Files;
	pio3dPtr t;


	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	Files=myIcon->Files;
	
	
	if(flag){
	    
		if(!t)return 1;
		if(Files){
			uActivateMenuItem(myIcon,FileMenu4,previewTekCommand,isInstalled(Installed_PrintPreview));
			uActivateMenuItem(myIcon,FileMenu4,printTekCommand,isInstalled(Installed_Print));
			uActivateMenuItem(myIcon,FileMenu4,TekSavePlots,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,copyTekCommand,TRUE && isInstalled(Installed_CopyImages));
			uActivateMenuItem(myIcon,EditMenu4,clearTekCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,FrameTekCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,zoomIn,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,zoomOut,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,popItem,TRUE && pushpopDepth(t->popStack));
			uActivateMenuItem(myIcon,EditMenu4,popAllItems,TRUE && pushpopDepth(t->popStack));
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekForward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekBackward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepForward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepBackward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStop,TRUE);
			uActivateMenuItem(myIcon,DataMenu4,dataScalesTek,TRUE);
			uActivateMenuItem(myIcon,DataMenu4,dataRangeTek,TRUE);
			uActivateMenuItem(myIcon,DataMenu4,dataSelectionPointsTek,TRUE);
			uActivateMenuItem(myIcon,DataMenu4,vectorDialog,TRUE);						
			uActivateMenuItem(myIcon,DataMenu4,switchPlanesTek,TRUE);						
			uActivateMenuItem(myIcon,DataMenu4,streamLines,TRUE);						
		}else{
			uActivateMenuItem(myIcon,FileMenu4,previewTekCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,printTekCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,TekSavePlots,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,copyTekCommand,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,clearTekCommand,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,FrameTekCommand,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,zoomIn,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,zoomOut,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,popItem,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,popAllItems,FALSE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekForward,FALSE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekBackward,FALSE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepForward,FALSE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepBackward,FALSE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStop,FALSE);
			uActivateMenuItem(myIcon,DataMenu4,dataScalesTek,FALSE);
			uActivateMenuItem(myIcon,DataMenu4,dataRangeTek,FALSE);
			uActivateMenuItem(myIcon,DataMenu4,dataSelectionPointsTek,FALSE);
			uActivateMenuItem(myIcon,DataMenu4,vectorDialog,FALSE);						
			uActivateMenuItem(myIcon,DataMenu4,switchPlanesTek,FALSE);						
			uActivateMenuItem(myIcon,DataMenu4,streamLines,FALSE);						
		}
		
		uActivateMenuItem(myIcon,ToolsMenu4,plotPointsTek,isInstalled(Installed_LinePlots));	    
		uActivateMenuItem(myIcon,ToolsMenu4,plotLinesTek,isInstalled(Installed_PointPlots));	    
		uActivateMenuItem(myIcon,FileMenu4,setupTekCommand,isInstalled(Installed_PrintSetup));	    
		uActivateMenuItem(myIcon,FileMenu4,savePreferencesCommand,isInstalled(Installed_PIO3DPreferences));
				
		uCheckMenuItem(myIcon,AnimationMenu4,loopTek,t->LoopFlag);
		uCheckMenuItem(myIcon,AnimationMenu4,loopBackTek,t->LoopBackAndForth);
		uCheckMenuItem(myIcon,AnimationMenu4,synchronizeTek,myIcon->Synchronize);
		     	    
		uCheckMenuItem(myIcon,ToolsMenu4,drawFramesTek,   	(t->Tool == drawFramesTek));
		uCheckMenuItem(myIcon,ToolsMenu4,zoomTek,         	(t->Tool == zoomTek));
		uCheckMenuItem(myIcon,ToolsMenu4,plotPointsTek,   	(t->Tool == plotPointsTek));
		uCheckMenuItem(myIcon,ToolsMenu4,plotLinesTek,  	(t->Tool == plotLinesTek));
		uCheckMenuItem(myIcon,ToolsMenu4,switchPlanesTek,  	(t->Tool == switchPlanesTek));
		uCheckMenuItem(myIcon,ToolsMenu4,SweepImages,  		(t->Tool == SweepImages));
		uCheckMenuItem(myIcon,ToolsMenu4,FiddleColors,  	(t->Tool == FiddleColors));
		uCheckMenuItem(myIcon,ToolsMenu4,streamLines,  		(t->Tool == streamLines));
	    CWindowSetPalMenu(myIcon);
	    
	    doWindowMenu();
	}
	return 0;
}
static int tInContent(IconPtr myIcon)
{
	pio3dPtr t;
	uPoint uwhere;
	uRect cBox;
	
	
	if(!myIcon || !(t=(pio3dPtr)myIcon->DWindow))return 1;

	cBox=myIcon->eraseRect;
	cBox.x = cBox.x+cBox.xsize-4;
	cBox.xsize = 4;
	cBox.ysize = 4;

	uPointer(&uwhere,myIcon);
	if(uPtInRect(uwhere,&myIcon->uviewRect)){
	    if(t->Tool == drawFramesTek){
	        doMouseDown(myIcon);
	    }else if(t->Tool == zoomTek){
			doMouseZoom(myIcon);
	    }else if(t->Tool == plotPointsTek){
			doPointPlotDown(myIcon);
	    }else if(t->Tool == plotLinesTek){
			doLinePlotDown(myIcon);
	    }else if(t->Tool == SweepImages){
			doMouseSweepDown(myIcon);
	    }else if(t->Tool == FiddleColors){
			doFiddleColors(myIcon);
	    }else if(t->Tool == switchPlanesTek){
			doSwitchPlanes(myIcon);
	    }else if(t->Tool == streamLines){
			doStreamLines(myIcon);
	    }
		if(t->AnimationOn){	  
			t->AnimationOn=FALSE;
			InvalRectMyWindow(myIcon);
		}		      
	} else if(uPtInRect(uwhere,&cBox)){
	    doEraseDown(myIcon);
	} else {
	   uDoControls(myIcon);
	}
	return 0;
}
static int doPointPlotDown(IconPtr myIcon)
{
	struct PlotPlaneData *pio;
	struct linedata *li;
	pio3dPtr t;
	uPoint pt;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	if(!myIcon->Files)return 1;
	pio = &t->plane;
	
	
	li= &t->pdata;
	uPointer(&pt,myIcon);
	
	IntLocC(myIcon,&li->x1,&li->y1,pt.x,pt.y);

    if(pio->Plane_type == X_PLANE){
        li->z1=li->y1;   
        li->y1=li->x1;
        li->x1=pio->Plane_xloc;
    }else if(pio->Plane_type == Y_PLANE){
        li->z1=li->y1;     
        li->y1=pio->Plane_yloc;
    }else if(pio->Plane_type == Z_PLANE){
        li->z1=pio->Plane_zloc;   
    }
    
		
	if(li->CurrentFrame < 0){
	    li->CurrentFrame=0;
	    li->FirstFrame=0;
	    li->LastFrame=t->ImageCount-1;
	    li->FirstTime=0;
	    li->LastTime=(double)(t->ImageCount-1);
	    li->FrameCount=t->ImageCount;
	}
	
	
	li->type=LINEDATA_POINT_DATA;
	GetWindowName(myIcon,li->sourceName,0);
	li->Plot=NULL;
	li->CurrentFrame=t->CurrentFrame;
	li->replaceData = -1;
	
	li->pioIndex=t->pioIndex;
	mstrncpy(li->pioName,t->pioName,255);
	
	
	if(CDrawNewLine(li,1))return 1;
	
	
	return 0;

}
static int doLinePlotDown(IconPtr myIcon)
{
	uPoint p1,p2;

	if(!myIcon)return 1;
		
	if(drawLineOnScreen(myIcon,&p1,&p2))return 1;

	return doLinePlot(p1,p2,myIcon);
}
static int doLinePlot(uPoint p1,uPoint p2,IconPtr myIcon)
{
    struct PlotPlaneData *pio;
	struct linedata *li;
	pio3dPtr t;
	double dl;


	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	if(!myIcon->Files)return 1;
	pio=&t->plane;

	li= &t->ldata;

	if((p1.x == p2.x) && (p1.y == p2.y))return 0;
	
	IntLocC(myIcon,&li->x1,&li->y1,p1.x,p1.y);
	IntLocC(myIcon,&li->x2,&li->y2,p2.x,p2.y);

    if(pio->Plane_type == X_PLANE){
        li->z1=li->y1;
        li->z2=li->y2;
        li->y1=li->x1;
        li->y2=li->x2;
        li->x1=li->x2=pio->Plane_xloc;
    }else if(pio->Plane_type == Y_PLANE){
        li->z1=li->y1;
        li->z2=li->y2;
        li->y1=li->y2=pio->Plane_yloc;
    }else if(pio->Plane_type == Z_PLANE){
        li->z1=li->z2=pio->Plane_zloc;   
    }

	dl=(p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y);
	dl=sqrt(dl);
	if(dl <= 0.0)dl=2;
	
	li->PointLineCount=(int)(2*dl+2);
	
	if(li->CurrentFrame < 0){
	    li->CurrentFrame=0;
	    li->FirstFrame=0;
	    li->LastFrame=t->ImageCount-1;
	    li->FirstTime=0;
	    li->LastTime=(double)(t->ImageCount-1);
	    li->FrameCount=t->ImageCount;
	}
	li->type=LINEDATA_LINE_DATA;
	GetWindowName(myIcon,li->sourceName,0);
	li->Plot=NULL;
	li->CurrentFrame=t->CurrentFrame;
	li->replaceData = -1;
	
	li->pioIndex=t->pioIndex;
	mstrncpy(li->pioName,t->pioName,255);
	
	if(CDrawNewLine(li,1))return 1;
	
	return 0;
}
static int doMouseZoom(IconPtr myIcon)
{
	uPoint where;
	pio3dPtr t;
	uRect oldR;
	int dx,dy,dx1,dy1,len,lens;
	int x[4],y[4];
	int iflag,n;

	
	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	
	uPointer(&where,myIcon);
	
	if(drawBoxOnScreen(myIcon,&oldR,NULL))return 1;
	
	x[0]=where.x-oldR.x;
	x[1]=x[0];
	x[2]=x[1]-oldR.xsize;
	x[3]=x[2];
	
	y[0]=where.y-oldR.y;
	y[1]=y[0]-oldR.ysize;
	y[2]=y[1];
	y[3]=y[0];
	
	iflag=0;
	dx=x[iflag];
	dy=y[iflag];
	lens=dx*dx+dy*dy;
	
	for(n=1;n<4;++n){
	    dx1=x[n];
	    dy1=y[n];
		len=dx1*dx1+dy1*dy1;
		if(len < lens){
			iflag=n;		
			lens=len;
		}
	}
	
	
	push(&oldR,iflag,myIcon);
	
	return 0;
}
static int pushStack(IconPtr myIcon)
{
    struct PlotRangeData *pio;
	pio3dPtr s;

	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;
	if(!myIcon->Files)return 1;

	pio=&s->range;

	s->stack.y=uGetControlValue(myIcon->VControl);
	s->stack.x=uGetControlValue(myIcon->HControl);
	s->stack.scale=s->scale;
	s->stack.xmaxData=pio->xmaxData;
	s->stack.ymaxData=pio->ymaxData;
	s->stack.xminData=pio->xminData;
	s->stack.yminData=pio->yminData;
	s->stack.xminSet=pio->xminSet;
	s->stack.xmaxSet=pio->xmaxSet;
	s->stack.yminSet=pio->yminSet;
	s->stack.ymaxSet=pio->ymaxSet;
	s->stack.ixmax=pio->ixmax;
	s->stack.iymax=pio->iymax;

	s->stack.ixminData=pio->ixminData;
	s->stack.ixmaxData=pio->ixmaxData;
	s->stack.iyminData=pio->iyminData;
	s->stack.iymaxData=pio->iymaxData;

	s->stack.CursorSpace=myIcon->CursorSpace;

	
	return pushpopPush(&s->stack,s->popStack);
}
static int push(uRect *oldR,int iflag,IconPtr myIcon)
{
	long xsize,ysize;
	double xmin,xmax,ymin,ymax;
	struct FileInfo2 *Files;
	pio3dPtr t;
	
	if(!oldR)return 1;
	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	
	xsize=oldR->xsize;
	ysize=oldR->ysize;

	if(xsize <= 2 || ysize <= 2)return 0;

	xmin=xmax=ymin=ymax=0;
	IntLocC(myIcon,&xmin,&ymin,oldR->x,oldR->y+oldR->ysize);
	IntLocC(myIcon,&xmax,&ymax,oldR->x+oldR->xsize,oldR->y);


	if(pushStack(myIcon)){
	    return 1;
	}
			
	SetRange(myIcon,xmin,xmax,ymin,ymax,USE_XY,iflag);	
	
	tMoveControls(myIcon);
	
 	InvalRectMyWindow(myIcon);
 	
	t->CurrectFrameLoaded = -1;
 	    
	return 0;
}
static int SendStreamlineInfo(IconPtr myIcon)
{
	struct CommandInfo Commands;
	struct PlotPlaneData *pio;
	long n,nn;
	pio3dPtr t;
	
	if(!myIcon || !(t=(pio3dPtr)myIcon->DWindow))return 1;
	pio = &t->plane;
	
	zerol((char *)&Commands,sizeof(Commands));
	

	nn=0;
	Commands.command[nn]="stream3d";
	Commands.type[nn++]=BATCH_STRING;
	
 	Commands.value[nn]=t->streamline3d.streamOn;
	Commands.type[nn++]=BATCH_DOUBLE;
	
	if(t->streamline3d.streamOn <= 0)goto doSend;

 	Commands.value[nn]=t->streamline3d.streamGradients;
	Commands.type[nn++]=BATCH_DOUBLE;
	
 	Commands.value[nn]=t->streamline3d.streamSteps;
	Commands.type[nn++]=BATCH_DOUBLE;
	
 	Commands.value[nn]=t->streamline3d.streamPasses;
	Commands.type[nn++]=BATCH_DOUBLE;
	
	
 	Commands.value[nn]=t->streamline3d.xPlane.count;
	Commands.type[nn++]=BATCH_DOUBLE;
	
	for(n=0;n<t->streamline3d.xPlane.count;++n){
	 	Commands.value[nn]=t->streamline3d.xPlane.x[n];
		Commands.type[nn++]=BATCH_DOUBLE;
	 	Commands.value[nn]=t->streamline3d.xPlane.y[n];
		Commands.type[nn++]=BATCH_DOUBLE;	
	}
	
	
 	Commands.value[nn]=t->streamline3d.yPlane.count;
	Commands.type[nn++]=BATCH_DOUBLE;
	
	for(n=0;n<t->streamline3d.yPlane.count;++n){
	 	Commands.value[nn]=t->streamline3d.yPlane.x[n];
		Commands.type[nn++]=BATCH_DOUBLE;
	 	Commands.value[nn]=t->streamline3d.yPlane.y[n];
		Commands.type[nn++]=BATCH_DOUBLE;	
	}
	
	
 	Commands.value[nn]=t->streamline3d.zPlane.count;
	Commands.type[nn++]=BATCH_DOUBLE;
	
	for(n=0;n<t->streamline3d.zPlane.count;++n){
	 	Commands.value[nn]=t->streamline3d.zPlane.x[n];
		Commands.type[nn++]=BATCH_DOUBLE;
	 	Commands.value[nn]=t->streamline3d.zPlane.y[n];
		Commands.type[nn++]=BATCH_DOUBLE;	
	}
	
	
doSend:
	Commands.nword=(int)nn;

	SendMessageByName(t->BatchName,MessageType_SEND_COMMAND_DATA,&Commands);

	
	return 0;
}  	

static int doStreamLines(IconPtr myIcon)
{
	struct PlotPlaneData *pio;
	pio3dPtr t;
	uPoint pt;
	double x,y;	
	
	if(!myIcon || !(t=(pio3dPtr)myIcon->DWindow))return 1;
	if((t->ImageCount) <= 0)return 1;
	if(!myIcon->Files)return 1;
	pio = &t->plane;

	uPointer(&pt,myIcon);

	if(uPtInRect(pt,&myIcon->uviewRect)){		
	    x = -1e60;
	    y = -1e60;
	    IntLocC(myIcon,&x,&y,pt.x,pt.y);	
	    
	    if(pio->Plane_type == X_PLANE){
	    	if(x < pio->Plane_ymin ||  x > pio->Plane_ymax)goto NotFound;
	    	if(y < pio->Plane_zmin ||  y > pio->Plane_zmax)goto NotFound;
	    	if(checkStreamData(&t->streamline3d.xPlane))goto ErrorOut;
	    	t->streamline3d.xPlane.x[t->streamline3d.xPlane.count]=x;
	    	t->streamline3d.xPlane.y[t->streamline3d.xPlane.count]=y;
	    	t->streamline3d.xPlane.count++;
	    	t->streamline3d.streamOn=TRUE;
			sprintf(WarningBuff,"x %g y %g z %g\n",pio->Plane_xloc,x,y);
	    }else if(pio->Plane_type == Y_PLANE){
	    	if(x < pio->Plane_xmin ||  x > pio->Plane_xmax)goto NotFound;;
	    	if(y < pio->Plane_zmin ||  y > pio->Plane_zmax)goto NotFound;
	    	if(checkStreamData(&t->streamline3d.yPlane))goto ErrorOut;
	    	t->streamline3d.yPlane.x[t->streamline3d.yPlane.count]=x;
	    	t->streamline3d.yPlane.y[t->streamline3d.yPlane.count]=y;
	    	t->streamline3d.yPlane.count++;
	    	t->streamline3d.streamOn=TRUE;
			sprintf(WarningBuff,"x %g y %g z %g\n",x,pio->Plane_yloc,y);
	    }else if(pio->Plane_type == Z_PLANE){
	    	if(x < pio->Plane_xmin ||  x > pio->Plane_xmax)goto NotFound;
	    	if(y < pio->Plane_ymin ||  y > pio->Plane_ymax)goto NotFound;
	    	if(checkStreamData(&t->streamline3d.zPlane))goto ErrorOut;
	    	t->streamline3d.zPlane.x[t->streamline3d.zPlane.count]=x;
	    	t->streamline3d.zPlane.y[t->streamline3d.zPlane.count]=y;
	    	t->streamline3d.zPlane.count++;
	    	t->streamline3d.streamOn=TRUE;
			sprintf(WarningBuff,"x %g y %g z %g\n",x,y,pio->Plane_zloc);
	    }	  
		WarningBatch(WarningBuff);  	
		SendStreamlineInfo(myIcon);   	
	    InvalRectMyWindow(myIcon);
		t->CurrectFrameLoaded = -1;
		
	}	
	
	return 0;
NotFound:
	sprintf(WarningBuff,"Stream Point Out of Range x %g y %g \n",x,y);
	WarningBatch(WarningBuff);  	    	
	sprintf(WarningBuff,"xmin %g xmax %g \n",pio->Plane_xmin,pio->Plane_xmax);
	WarningBatch(WarningBuff);  	    	
	sprintf(WarningBuff,"ymin %g ymax %g \n",pio->Plane_ymin,pio->Plane_ymax);
	WarningBatch(WarningBuff);  	    	
	sprintf(WarningBuff,"zmin %g zmax %g \n",pio->Plane_zmin,pio->Plane_zmax);
	WarningBatch(WarningBuff);  	    	
	return 0;
ErrorOut:
	WarningBatch("doStreamLines Out of Memory\n");  	    	
	return 1;
}



static int doSwitchPlanes(IconPtr myIcon)
{
	struct PlotPlaneData *pio;
	pio3dPtr t;
	uPoint pt;
	long Show,ImageCount,LastFrame;
	int x,xold;
	long xsize,ysize,zsize;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double ddx,ddy,ddz;
	uRect r;
	
	
	if(!myIcon || !(t=(pio3dPtr)myIcon->DWindow))return 1;
	if((t->ImageCount) <= 0)return 1;
	if(!myIcon->Files)return 1;
	pio = &t->plane;

	uGetPortBounds(myIcon,&r);

	xsize = pio->Plane_xcount;
	ysize = pio->Plane_ycount;
	zsize = pio->Plane_zcount;

	r.xsize -= 16;
	
	xsize = r.xsize-30;
	ysize = r.xsize-30;
	zsize = r.xsize-30;
	
	ImageCount=r.xsize-30;

	xmin=pio->Plane_xmin;
	xmax=pio->Plane_xmax;

	ymin=pio->Plane_ymin;
	ymax=pio->Plane_ymax;

	zmin=pio->Plane_zmin;
	zmax=pio->Plane_zmax;
	
	ddx=(xmax-xmin)/(double)xsize;
	ddy=(ymax-ymin)/(double)ysize;
	ddz=(zmax-zmin)/(double)zsize;
	
    
	Show=0;
	xold = -1;
	LastFrame = -1;
	while(uStillDown(myIcon)){
		uPointer(&pt,myIcon);
	    x=pt.x-15;	
	    if(x == xold)continue;  
	    xold = x;  
	    
	    Show = x;
	    
	    if(Show >= ImageCount){
	        Show=ImageCount;
	    }else if(Show < 0){
	       Show=0;
	    }
	    
	    if(Show == LastFrame)continue;
	    LastFrame = Show;
	    
	    if(pio->Plane_type == X_PLANE){
	        pio->Plane_xloc=pio->Plane_xmin+(double)Show*ddx;
	    }else if(pio->Plane_type == Y_PLANE){
	        pio->Plane_yloc=pio->Plane_ymin+(double)Show*ddy;
	    }else if(pio->Plane_type == Z_PLANE){
	        pio->Plane_zloc=pio->Plane_zmin+(double)Show*ddz;
	    }
	    
	    if(IsShift()){
			writeCursor(pt,myIcon);
			xold = -1;
			LastFrame = -1;
	    }else{
			t->CurrectFrameLoaded = -1;
		    drawIt(myIcon);
		    if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
	    }
	}
	
	if(LastFrame != Show){
		t->CurrectFrameLoaded = -1;
	    drawIt(myIcon);
	    if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
    }
    
	t->AnimationOn = 0;
	return 0;
}
static void doFrameImage(IconPtr myIcon)
{
    struct PlotRangeData *pio;
    struct PlotPlaneData *pio2;
	struct FileInfo2 *Files;
	long xsizep,ysizep;
	pio3dPtr t;
	uRect r;


	if(!myIcon)return;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return;
	if(t->ImageCount <= 0)return;
	Files=myIcon->Files;
	if(!Files)return;
	pio = &t->range;
	pio2 = &t->plane;
	
	if(uGetPaletteSize(&xsizep,&ysizep,&t->pd,myIcon))return;

	if(IsShift()){
	   t->BoundarySpace = 0;
	}else{
	   t->BoundarySpace = 10;
	}

	myIcon->CursorSpace=getCursorStringLineCount(myIcon)*DataSpace;
	if(myIcon->CursorSpace <= 0)myIcon->CursorSpace=DataSpace;
	
	uGetPortBounds(myIcon,&r);

	pio->ixmax=r.xsize-ScrollOffSet;
	pio->iymax=r.ysize-(myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace);

	pio->ixminData=t->BoundarySpace;
	pio->iyminData=t->BoundarySpace;
	pio->ixmaxData=pio->ixmax-t->BoundarySpace-xsizep;
	pio->iymaxData=pio->iymax-t->BoundarySpace;
	
	ScreenSetSize(pio->ixmax,pio->iymax,myIcon);
		
    if(pio2->Plane_type == X_PLANE){
    	SetRange(myIcon,Files->yminr,Files->ymaxr,Files->zminr,Files->zmaxr,USE_XY,0);
    }else if(pio2->Plane_type == Y_PLANE){
    	SetRange(myIcon,Files->xminr,Files->xmaxr,Files->zminr,Files->zmaxr,USE_XY,0);
    }else if(pio2->Plane_type == Z_PLANE){
    	SetRange(myIcon,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY,0);
    }
    
		
	InvalRectMyWindow(myIcon);

	tMoveControls(myIcon);
	
	uSetControlValue(myIcon->VControl,0L);
	uSetControlMaximum(myIcon->VControl,0L);
	uSetControlValue(myIcon->HControl,0L);
	uSetControlMaximum(myIcon->HControl,0L);
	SetFrame(myIcon);
	t->CurrectFrameLoaded=-1;
}
static int getCursorStringLineCount(IconPtr myIcon)
{
	char buff[512],*bp,*bs;
	int linecount;
	int width,swidth;
	long length;
	uRect r;
	
	if(!myIcon)return 1;
	
	buff[0]=0;	

	getCursorSting(buff,sizeof(buff),NULL,myIcon);
		
	uGetPortBounds(myIcon,&r);
	
	width=r.xsize-ScrollOffSet;
	
	length=0;
	bs=bp=buff;
	linecount=0;
	while(*bp){
	    if(*bp == '#'){
	       *bp=0;
	       swidth=uCStringWidth(bs,myIcon);
	       if((length+swidth) >= width){
				++linecount;
				if(length > 0){
					length=0;
					*bp = '#';
					continue;
				}else{
					length=0;
				}
	       }else{
				length += (swidth+uCStringWidth(" ",myIcon));          
	       }
	       bs=bp+1;
	    }
	    ++bp;
	}
	
	if(length > 0)++linecount;
	
	return linecount;
}
static void tMoveControls2(IconPtr myIcon)
{
	pio3dPtr t;
	uRect uviewRect;
	
	if(!myIcon)return;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return;
	
	uviewRect=myIcon->uviewRect;
	
	tMoveControls(myIcon);
	
	if(uviewRect.xsize != myIcon->uviewRect.xsize || uviewRect.ysize != myIcon->uviewRect.ysize){	
		t->CurrectFrameLoaded = -1;
	}
	
}
static void tMoveControls(IconPtr myIcon)
{
    struct PlotRangeData *pio;
    struct PlotPlaneData *pio2;
	struct FileInfo2 *Files;
	long xsizep,ysizep;
	controlPtr control;
	uRect eraseRect;
	uRect dataRect;
	pio3dPtr t;
	double Value,didx,didy,dydi,dxdi;
	double xminr,xmaxr,yminr,ymaxr;
	int xsize,ysize;
	int MaxValue;
	int bottom;
	int right;
	int top;
	int left;
	uRect r;
		
	
	if(!myIcon)return;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return;
	pio = &t->range;
	pio2 = &t->plane;
	Files=myIcon->Files;
	if(!Files){
		if(myIcon->VControl)HideControl(myIcon->VControl);
		if(myIcon->HControl)HideControl(myIcon->HControl);
	    return;
	}else{
		if(myIcon->VControl)ShowControl(myIcon->VControl);
		if(myIcon->HControl)ShowControl(myIcon->HControl);
	}
	
	myIcon->CursorSpace=getCursorStringLineCount(myIcon)*DataSpace;
	if(myIcon->CursorSpace <= 0)myIcon->CursorSpace=DataSpace;

	uGetPortBounds(myIcon,&r);
		
	top=0;
	left=0;
	bottom=r.ysize;
	right=r.xsize;
		
	if(uGetPaletteSize(&xsizep,&ysizep,&t->pd,myIcon))return;
		
	myIcon->uviewRect = r;
	myIcon->uviewRect.y = myIcon->mMenuOffSet;
	myIcon->uviewRect.x = 0;
	myIcon->uviewRect.ysize -= (myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace);
	myIcon->uviewRect.xsize -= ScrollOffSet;
	
	
	t->pd.left=myIcon->uviewRect.xsize-xsizep;
	t->pd.top=myIcon->uviewRect.ysize/2-128-10;
		
	xsize=myIcon->uviewRect.xsize;
	ysize=myIcon->uviewRect.ysize;
	
	pio->ixmax=xsize;
	pio->iymax=ysize;
	
	dydi=(pio->ymaxData-pio->yminData)/((double)(pio->iymaxData-pio->iyminData));
	dxdi=(pio->xmaxData-pio->xminData)/((double)(pio->ixmaxData-pio->ixminData));

	if(t->BoundarySpace >= 0){
		pio->ixminData=t->BoundarySpace;
		pio->iyminData=t->BoundarySpace;
		pio->ixmaxData=pio->ixmax-t->BoundarySpace;
		pio->iymaxData=pio->iymax-t->BoundarySpace;
	}else{
		if(pio->ixminData < 0)pio->ixminData=0;
		if(pio->iyminData < 0)pio->iyminData=0;
		if(pio->ixmaxData > pio->ixmax)pio->ixmaxData=pio->ixmax;
		if(pio->iymaxData > pio->iymax)pio->iymaxData=pio->iymax;
	}
	
	pio->xmaxData=pio->xminData+dxdi*((double)(pio->ixmaxData-pio->ixminData));
    pio->yminData=pio->ymaxData-dydi*((double)(pio->iymaxData-pio->iyminData));
	
	ScreenSetSize(pio->ixmax,pio->iymax,myIcon);
	
	eraseRect.y=bottom-ScrollOffSet;
	eraseRect.x=left;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;
	
	dataRect=myIcon->uviewRect;
	dataRect.y=myIcon->uviewRect.y+myIcon->uviewRect.ysize;
	dataRect.ysize=DataSpace;
	
	myIcon->dataRect=dataRect;
	
    if(pio2->Plane_type == X_PLANE){
		xminr=Files->yminr;
		yminr=Files->zminr;
		xmaxr=Files->ymaxr;
		ymaxr=Files->zmaxr;
		if(pio2->Plane_xloc < Files->xminr)pio2->Plane_xloc=Files->xminr;
		if(pio2->Plane_xloc > Files->xmaxr)pio2->Plane_xloc=Files->xmaxr;
    }else if(pio2->Plane_type == Y_PLANE){
		xminr=Files->xminr;
		yminr=Files->zminr;
		xmaxr=Files->xmaxr;
		ymaxr=Files->zmaxr;
		if(pio2->Plane_yloc < Files->yminr)pio2->Plane_yloc=Files->yminr;
		if(pio2->Plane_yloc > Files->ymaxr)pio2->Plane_yloc=Files->ymaxr;
    }else if(pio2->Plane_type == Z_PLANE){
		xminr=Files->xminr;
		yminr=Files->yminr;
		xmaxr=Files->xmaxr;
		ymaxr=Files->ymaxr;
		if(pio2->Plane_zloc < Files->zminr)pio2->Plane_zloc=Files->zminr;
		if(pio2->Plane_zloc > Files->zmaxr)pio2->Plane_zloc=Files->zmaxr;
    }else{
		xminr=Files->xminr;
		yminr=Files->yminr;
		xmaxr=Files->xmaxr;
		ymaxr=Files->ymaxr;
		if(pio2->Plane_xloc < Files->xminr)pio2->Plane_xloc=Files->xminr;
		if(pio2->Plane_xloc > Files->xmaxr)pio2->Plane_xloc=Files->xmaxr;
    }
	
	control=myIcon->VControl;
	if(control){
		MoveControl(control,right-ScrollOffSet,top+myIcon->mMenuOffSet);
		SizeControl(control,ScrollOffSet,bottom-top-ScrollOffSet-myIcon->mMenuOffSet);
		if(t->ImageCount > 0){
		if(t->ImageCount > 0){
		    didy=((double)(pio->iymaxData-pio->iyminData))/(pio->ymaxData-pio->yminData);
		    Value=CONTROL_SCALEO*((ymaxr-yminr)-(pio->ymaxData-pio->yminData))*didy;
		    if(Value > 0.0)++Value;
			MaxValue=(int)(Value);
			if(MaxValue < 0)MaxValue = 0;
		} else {
	    	MaxValue=0;
		}
		uSetControlMaximum(control,MaxValue);
	}
	
	control=myIcon->HControl;	
	if(control){
		MoveControl(control,eraseRect.x+eraseRect.xsize,bottom-ScrollOffSet);
		SizeControl(control,right-(eraseRect.xsize+ScrollOffSet),ScrollOffSet);
		    didx=((double)(pio->ixmaxData-pio->ixminData))/(pio->xmaxData-pio->xminData);
		    Value=CONTROL_SCALEO*((xmaxr-xminr)-(pio->xmaxData-pio->xminData))*didx;
		    if(Value > 0.0)++Value;
			MaxValue=(int)(Value);
			if(MaxValue < 0)MaxValue = 0;
		} else {
	    	MaxValue=0;
		}
		uSetControlMaximum(control,MaxValue);
	}
	
	if(myIcon->VControl){
	    double dyy,dydi;
	    int top;
		dyy=pio->ymaxData-pio->yminData;
		dydi=dyy/((double)(pio->iymaxData-pio->iyminData));
		top=(int)(0.5+(ymaxr-pio->ymaxData)/(dydi*CONTROL_SCALE));
		if(top <= 0){
		   top = 0;	
		}
		
		uSetControlValue(myIcon->VControl,top);
	}
	if(myIcon->HControl){
	    double dxx,dxdi;
	    int left;
		dxx=pio->xmaxData-pio->xminData;
		dxdi=dxx/((double)(pio->ixmaxData-pio->ixminData));
		left=(int)(0.5+(pio->xminData-xminr)/(dxdi*CONTROL_SCALE));
		if(left <= 0){
		   left=0;	
		}
		uSetControlValue(myIcon->HControl,left);
	}
	InvalRectMyWindow(myIcon);	
}
static int UpdateWrite(IconPtr myIcon)
{
	    uRect portRect;

		if(!myIcon || !myIcon->isOpen)return 1;
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    /* uEraseRect(&portRect,myIcon); */
	
		drawIt(myIcon);
		
	    uClipRect(&portRect,myIcon);
	    uDrawControls(myIcon);
	    uDrawGrowIcon(myIcon);
	    
	    return 0;
}
static void drawIt(IconPtr myIcon)
{
	    pio3dPtr t;
	    char buffscr[256];
        uRect portRect;
	    char *name;
		uRect Box;
		uPoint pt;
	    
		if(!myIcon)return;
		t=(pio3dPtr)myIcon->DWindow;
		if(!t)return;
		if(!myIcon->Files)return;
		
	    uClipRect(&myIcon->uviewRect,myIcon);
	    /* uEraseRect(&myIcon->uviewRect,myIcon); */
	    
  		Box.x = 0;
  		Box.y = myIcon->uviewRect.y;
  		Box.xsize=(int)((double)t->range.ixmax*t->scale);
  		Box.ysize=(int)((double)t->range.iymax*t->scale);
		drawTek(myIcon,t->CurrentFrame,&Box);
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
				
		    
		name=FilesImageName(myIcon->Files,t->CurrentFrame);

		if(name){
			sprintf(buffscr,"%s",name);
			nWriteBox(myIcon,buffscr);
		}

		uPointer(&pt,myIcon);
		nMouseMoved(myIcon,pt);
}
static int loadFrame(IconPtr myIcon,long CurrentFrame)
{
    struct PlotRangeData *range;
    struct PlotPlaneData *plane;
	struct FileInfo2 *Files;
  	struct SetFrameData  *sd;
  	double dxx,dyy,dydi,dxdi,x,y;
  	double xminr,ymaxr;
    pio3dPtr s;
    int top,left;
    long length;
    int ret;
    
	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	range =  &s->range;
	plane = &s->plane;
	sd=&s->sd;
	
	ret=1;

	if(s->CurrectFrameLoaded == CurrentFrame && sd->data && sd->data2){
	/* WarningBatch("loadFrame skip\n"); */
		return 0;
	}
	
	/* WarningBatch("loadFrame New\n"); */
	
	
	s->CurrectFrameLoaded=CurrentFrame;

	
    if(plane->Plane_type == X_PLANE){
		xminr=Files->yminr;
		ymaxr=Files->zmaxr;
    }else if(plane->Plane_type == Y_PLANE){
		xminr=Files->xminr;
		ymaxr=Files->zmaxr;
    }else if(plane->Plane_type == Z_PLANE){
		xminr=Files->xminr;
		ymaxr=Files->ymaxr;
    }else{
		xminr=Files->xminr;
		ymaxr=Files->ymaxr;
    }
    
	dyy=range->ymaxData-range->yminData;
	dydi=dyy/((double)(range->iymaxData-range->iyminData));
	dxx=range->xmaxData-range->xminData;
	dxdi=dxx/((double)(range->ixmaxData-range->ixminData));
		
	top=0;
	left=0;
	if(myIcon->VControl)top=(int)uGetControlValue(myIcon->VControl);
	if(myIcon->HControl)left=(int)uGetControlValue(myIcon->HControl);
	if(left >= 0){	
	   x=xminr+left*dxdi*CONTROL_SCALE;
	   range->xminData=x;
	   range->xmaxData=x+dxx;
	}
	
	if(top >= 0){	
	   y=ymaxr-top*dydi*CONTROL_SCALE;
	   range->ymaxData=y;
	   range->yminData=y-dyy;
	}
	
	
	if(sd->data)cFree((char *)sd->data);
	sd->data=NULL;

	if(sd->data2)cFree((char *)sd->data2);
	sd->data2=NULL;
		
	s->CurrentFrame=CurrentFrame;	
		
	zerol((char *)sd,sizeof(struct SetFrameData));
	
	sd->CurrentFrame=CurrentFrame;
	sd->type = FRAME_DATA_FLOAT;
	
	sd->range  =  s->range;
	sd->plane  =  s->plane;
	sd->vector =  s->vector;
	sd->pd 	   =  s->pd;
	sd->pa 	   =  s->pa;
	
	sd->typePlotRange = FRAME_RANGE_LIMITS;
	
	if(myIcon->Cursor.count > 0){
	    doGetCursorData(myIcon,s->BatchName,sd);
	}
	
	sd->type = FRAME_DATA_FLOAT_RASTER;
	
	if(s->pioName[0]){
		mstrncpy(sd->pioName,s->pioName,255);
		sd->pioIndex=s->pioIndex;			    
	}						
	
	if(SendMessageByName(s->BatchName,MessageType_GET_SCREEN_DATA,sd)){
	    if(strcmp(s->pioName,"density")){
	        InvalRectMyWindow(myIcon);
	    	mstrncpy(s->pioName,"density",255);
	   	 	s->pioIndex=0;
	    }
		goto ErrorOut;
	}
	
	range->ixmax=sd->xsize;
	range->iymax=sd->ysize;
	
	s->pd = sd->pd;
	s->pa = sd->pa;
	
	ScreenSetSize(range->ixmax,range->iymax,myIcon);
	
	length=sd->xsize*sd->ysize;

	myIcon->limits = sd->limits;
	if(sd->limits.pioName){
		mstrncpy(s->pioName,(char *)sd->limits.pioName,255);
		s->pioIndex=sd->limits.pioIndex;			    
	
	}


	ret=0;
ErrorOut:	
	return ret;	
}
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
    struct PlotRangeData *range;
    struct PlotPlaneData *plane;
	struct FileInfo2 *Files;
  	struct SetFrameData  *sd;
    pio3dPtr s;
	unsigned char *buffRead;
    uRect SRect,LRect;
    int top,left,dx,dy,dx1,dy1;
    long length;
    
	if(!myIcon)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	range =  &s->range;
	plane = &s->plane;
	sd=&s->sd;
	
	if(loadFrame(myIcon,CurrentFrame))return 1;

	range->ixmax=sd->xsize;
	range->iymax=sd->ysize;
	
	s->pd = sd->pd;

	ScreenSetSize(range->ixmax,range->iymax,myIcon);
	
	length=sd->xsize*sd->ysize;

	myIcon->limits = sd->limits;

    if(SetBuffers(length*3+512L))goto ErrorOut;
    
    buffRead=(unsigned char *)sd->data2;
	
	LRect= *DRect;

	dx=LRect.xsize;
	dy=LRect.ysize;

	top=0;
	left=0;

	SRect.y=top;
	if(SRect.y < 0)SRect.y=0;
	if(SRect.y > range->iymax)SRect.y=(int)range->iymax;

	SRect.ysize = dy;
	if(SRect.ysize < 0)SRect.ysize=0;
	if(SRect.y+SRect.ysize > range->iymax)SRect.ysize=(int)(range->iymax-SRect.y);


	SRect.x=left;
	if(SRect.x < 0)SRect.x=0;
	if(SRect.x > range->ixmax)SRect.x=(int)range->ixmax;

	SRect.xsize = dx;
	if(SRect.xsize < 0)SRect.xsize=0;
	if(SRect.x+SRect.xsize > range->ixmax)SRect.xsize=(int)(range->ixmax-SRect.x);

	dx1=SRect.xsize;
	if(dx1 != dx){
	    LRect.xsize = dx1;
	}

	dy1=SRect.ysize;
	if(dy1 != dy){
	    LRect.ysize = dy1;
	}

	ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);

	s->pd.xsize=sd->xsize;
	s->pd.ysize=sd->ysize;
	s->pd.buffer=buffRead;
	s->pd.ScreenBits=RASTER_8;
	
	
	if(sd->pd.AreaFill){	
		uDrawPalette(&s->pd,myIcon);
		uScreenWrite(range->ixmax,range->iymax,(unsigned char *)buffRead,(unsigned char *)xg.buffout,ScreenDataType8,&SRect,&LRect,myIcon);  
	}else{
	    void *data;
		uMemcpy(&s->pd.palette,myIcon->pal,768);
		uDrawPalette(&s->pd,myIcon);
	    data=sd->data;
	    sd->length=sd->xsize*sd->ysize;
	    memcpy(xg.buffin,buffRead,sd->length);
	    sd->data=xg.buffin;
		convertTO24(sd,&s->pd);
	    sd->data=data;
	    sd->length=0L;
		uScreenWrite(range->ixmax,range->iymax,(unsigned char *)xg.buffin,(unsigned char *)xg.buffout,ScreenDataType24,&SRect,&LRect,myIcon);  
	}
/*	
	if(!myIcon->iHaveMoved){
	   myIcon->iHaveMoved=TRUE;
	   tMoveControls(myIcon);
	}
*/
ErrorOut: 

	    			    
		return 0;
}
static int SetFrame(IconPtr myIcon)
{
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;

	if(t->ImageCount <= 0){
	    t->CurrentFileName="No Images";
	    return 1;
	}

	if(!myIcon->Files){
	    return 0;
	}

 	if(t->CurrentFrame >= t->ImageCount){
	    if(t->LoopBackAndForth){
		    t->CurrentFrame=t->ImageCount-1;
	    }else{
	        t->CurrentFrame=0;
	    }
	}

	if(t->CurrentFrame < 0){
	    if(t->LoopBackAndForth){
	        t->CurrentFrame=0;
	    }else{
	        t->CurrentFrame=t->ImageCount-1;
	    }
	}
	return 0;
}
int doCursorFree(struct CursorStruct *Cursor);
static int doClose(IconPtr myIcon)
{
	pio3dPtr t;
	
	stopLanguage();
		
	if(myIcon){
	    SetPortMyWindow(myIcon);
	    t=(pio3dPtr)myIcon->DWindow;
   	    if(t){
			if(t->sd.data)cFree((char *)t->sd.data);
			t->sd.data=NULL;
			if(t->sd.data2)cFree((char *)t->sd.data2);
			t->sd.data2=NULL;
			doCursorFree(&myIcon->Cursor);
			if(t->BatchName[0])FileManagerCloseName(t->BatchName);
			
			t->BatchName[0]=0;
			
		    myIcon->Files=NULL;
		    	
			pushpopEND(t->popStack);	     
						   
	    	freeStreamLine3d(&t->streamline3d);
	        
	    	myIcon->DWindow=NULL;
	    	
	        cFree((char *)t);
	        
		}
	    
	    myIcon=RemoveIcon(myIcon);
	    
	}
	return 0;
}
static int tdothumb(controlPtr control,IconPtr myIcon)
{
	//control=control;
	return tMoveDy(myIcon);
}
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
		int old;
	
		if(!myIcon || !control)return 0;
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old-12);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);
}
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon)
{
		int old;
		
		if(!myIcon || !control)return 0;
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old+12);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);
}
static int tgoUp(controlPtr control,short part,IconPtr myIcon)
{	
		int old;
	
		if(!myIcon || !control)return 0;
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old-1);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tgoDown(controlPtr control,short part,IconPtr myIcon)
{			
		int old;
		
		if(!myIcon || !control)return 0;
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old+1);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tMoveDy(IconPtr myIcon)
{			
	pio3dPtr t;
	
	if(myIcon){
	    t=(pio3dPtr)myIcon->DWindow;
		if(t)t->CurrectFrameLoaded = -1;		
	}
	drawIt(myIcon);
	return 0;
}
static int IntLocC(IconPtr myIcon,double *x,double *y,int ixx,int iyy)
{
	int iy,ix,top,left;
	pio3dPtr s;

	if(!myIcon || !x || !y)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 1;

	top=0;
	left=0;

	if(myIcon->VControl){
	    top=(int)uGetControlValue(myIcon->VControl);
	    top=0;
	}
	
	if(myIcon->HControl){
	    left=(int)uGetControlValue(myIcon->HControl);
	    left=0;
	}

	iy=(iyy+top-myIcon->mMenuOffSet);
	iy=(int)(s->range.iymax-1-iy);	
	ix=ixx+left;

	return IntLoc(myIcon,x,y,ix,iy);
}
static int IntLoc(IconPtr myIcon,double *x,double *y,int ix,int iy)
{
    struct SetFrameData  *sd;
   	struct LIMITS *limits;
	pio3dPtr s;

	if(!myIcon || !x || !y)return 1;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 0;
	sd = &s->sd;
	if((sd->xsize <= 0) || (sd->ysize <= 0))return 1;
	limits=&sd->limits;

	*x=(limits->xmax-limits->xmin)*((double)(ix)/(double)sd->xsize)+limits->xmin;
	*y=(limits->ymax-limits->ymin)*((double)(iy)/(double)sd->ysize)+limits->ymin;

	return 0;
}
/*
static int RealLoc(IconPtr myIcon,double *x,double *y,int *ix,int *iy)
{

	if(!pio || !x || !y || !ix || !iy)return 1;

	*ix=(int)((*x-pio->xminData)*pio->xScale+(double)pio->ixminData+.5);
	*iy=(int)((*y-pio->yminData)*pio->yScale+(double)pio->iyminData+.5);

	return 0;
}
*/
static int pioGetValue(IconPtr myIcon,double x,double y,double *v,long *nn)
{
    struct SetFrameData  *sd;
   	struct LIMITS *limits;
    double *d,dd;
	pio3dPtr s;
	long i,j;

	if(!myIcon || !v || !nn)return 0;
	s=(pio3dPtr)myIcon->DWindow;
	if(!s)return 0;
	sd = &s->sd;
	d=(double *)sd->data;
	if(!d)return 0;
	limits=&sd->limits;
	
	if(x < limits->xmin || x > limits->xmax)return 0;
	if(y < limits->ymin || y > limits->ymax)return 0;
	
	i=(long)((x-limits->xmin)*((double)sd->xsize)/(limits->xmax-limits->xmin));
	if(i < 0){
	    i=0;
	}else if(i >= sd->xsize){
	    i=sd->xsize-1;
	}
	
	j=(long)((y-limits->ymin)*((double)sd->ysize)/(limits->ymax-limits->ymin));
	if(j < 0){
	    j=0;
	}else if(j >= sd->ysize){
	    j=sd->ysize-1;
	}
	
	j=sd->ysize-1-j;
	
	dd=d[i+j*sd->xsize];
	
	if(dd == FLOAT_NOT_SET)return 0;
	
	*v = dd;
	*nn = i+j*sd->xsize;
	return 1;
		
}
static int SetRange(IconPtr myIcon,double xmin,double xmax,double ymin,double ymax,int flag,int flag2)
{
	struct PlotRangeData *pio;
    struct FileInfo2 *Files;
	pio3dPtr t;

	if(!myIcon)return 1;
	t=(pio3dPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	pio=&t->range;


	if(pio->MaintainAspectRatio){
	    double dxo,dyo,dxi,dyi,dx,dy;

	    dxi=(double)(pio->ixmaxData-pio->ixminData);
	    dyi=(double)(pio->iymaxData-pio->iyminData);
	    dxo=(xmax-xmin)/dxi;
	    dyo=(ymax-ymin)/dyi;
	    	    	   
	    if(flag == USE_XY){
		    if(dxo > pio->AspectRatio*dyo){  /* use x - range */
		        dy=dyi*dxo/pio->AspectRatio;
		        if(flag2 == 1 || flag2 == 2){
		            ymax = ymin + dy;
		        }else{
		            ymin = ymax - dy;
		        }
		    }else{			     /* use y - range */
		        dx=dxi*dyo*pio->AspectRatio;
		        if(flag2 == 0 || flag2 == 3){
		            xmin = xmax - dx;
		        }else{
				    xmax = xmin + dx;
				}
		    }
	    }else if(flag == USE_X){
	            dy=dyi*dxo/pio->AspectRatio;
		        if(flag2 == 1 || flag2 == 2){
		            ymax = ymin + dy;
		        }else{
		            ymin = ymax - dy;
		        }
	    }else if(flag == USE_Y){
	            dx=dxi*dyo*pio->AspectRatio;
		        if(flag2 == 0 || flag2 == 3){
		            xmin = xmax - dx;
		        }else{
				    xmax = xmin + dx;
				}
	    }
	}
	
	pio->xmaxData=xmax;
	pio->ymaxData=ymax;
	pio->xminData=xmin;
	pio->yminData=ymin;

	pio->xminSet=1;
	pio->xmaxSet=1;
	pio->yminSet=1;
	pio->ymaxSet=1;


	if(myIcon->VControl){
		uSetControlValue(myIcon->VControl,0L);
	}
	if(myIcon->HControl){
		uSetControlValue(myIcon->HControl,0L);
	}
	InvalRectMyWindow(myIcon);
	return 0;
}
int doUniversalSagePIO3D(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct pio3dInfo ss;
	static struct Icon mm;

	static struct universalTableListStruct Local2[] = {
		{"REAL",5201,universal_TypeInt,(void *)&mm.eraseRectSize},
		{"STRI",5202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",5203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"DOUB",5204,universal_TypeDouble,(void *)&mm.framesPerSecond},	
		{"REAL",5205,universal_TypeInt,(void *)&mm.Synchronize},	
		{"FILE",5218,universal_TypeFilesPtr,(void *)&mm.Files},	
		{"STRI",5222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",5223,universal_TypeInt,(void *)&mm.WindowNameSet},		
		{"FILE",5224,universal_TypeCursorPtr,(void *)&mm.Cursor},	
		
		{"REAL",5224,universal_TypeInt,(void *)&mm.Cursor.valueFlag},									
		{"STRI",5225,universal_TypeString,(void *)&mm.Cursor.valueformat[0],sizeof(mm.Cursor.valueformat)},
		{"STRI",5226,universal_TypeString,(void *)&mm.Cursor.coordinateFormat[0],sizeof(mm.Cursor.coordinateFormat)},
		{"STRI",5227,universal_TypeString,(void *)&mm.Cursor.timeFormat[0],sizeof(mm.Cursor.timeFormat)},
		
									
	};
	
	static struct universalTableStruct Global2 = {
		"SagePIO3D parameters",universal_WindowSagePIO3D,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local[] = {
		{"REAL",5059,universal_TypeDoubleArrayPtr,(void *)&ss.streamline3d.xPlane.x},	
		{"REAL",5060,universal_TypeDoubleArrayPtr,(void *)&ss.streamline3d.xPlane.y},	
		
		{"REAL",5061,universal_TypeDoubleArrayPtr,(void *)&ss.streamline3d.yPlane.x},	
		{"REAL",5062,universal_TypeDoubleArrayPtr,(void *)&ss.streamline3d.yPlane.y},	
		
		{"REAL",5063,universal_TypeDoubleArrayPtr,(void *)&ss.streamline3d.zPlane.x},	
		{"REAL",5064,universal_TypeDoubleArrayPtr,(void *)&ss.streamline3d.zPlane.y},	
	
	
		{"RECT",5004,universal_TypeRect,(void *)&ss.windowRect},	
		{"REAL",5005,universal_TypeInt,(void *)&ss.Tool},	
		{"REAL",5007,universal_TypeInt,(void *)&ss.LoopBackAndForth},	
		{"REAL",5008,universal_TypeInt,(void *)&ss.LoopFlag},	
		{"REAL",5010,universal_TypeInt,(void *)&ss.pioIndex},	
		{"STRI",5011,universal_TypeString,(void *)&ss.pioName[0],sizeof(ss.pioName)},
		{"REAL",5012,universal_TypeInt,(void *)&ss.plane.Plane_type},	
		{"REAL",5013,universal_TypeInt,(void *)&ss.pa.DrawZones},	
		{"REAL",5014,universal_TypeInt,(void *)&ss.pa.flagGradients},	
		{"REAL",5015,universal_TypeInt,(void *)&ss.pd.LabeledPalette},	
		{"REAL",5016,universal_TypeInt,(void *)&ss.pa.ZoneLevel},	
		
		{"DOUB",5019,universal_TypeDouble,(void *)&ss.vector.arrowLength},	
		{"DOUB",5020,universal_TypeDouble,(void *)&ss.vector.arrowAngle},	
		{"DOUB",5021,universal_TypeDouble,(void *)&ss.vector.vspace},	
		{"DOUB",5022,universal_TypeDouble,(void *)&ss.vector.vfac},	
		{"DOUB",5023,universal_TypeDouble,(void *)&ss.vector.vmax},	
		{"REAL",5024,universal_TypeInt,(void *)&ss.vector.DrawVectors},	
		{"REAL",5025,universal_TypeInt,(void *)&ss.vector.arrowHeads},	
		
		{"REAL",5026,universal_TypeInt,(void *)&ss.ldata.pioIndex2},	
		{"STRI",5027,universal_TypeString,(void *)&ss.ldata.pioName2[0],sizeof(ss.ldata.pioName2)},
		{"REAL",5028,universal_TypeInt,(void *)&ss.ldata.LineSourceMode},	
		
		{"REAL",5029,universal_TypeInt,(void *)&ss.pdata.pioIndex2},	
		{"STRI",5030,universal_TypeString,(void *)&ss.pdata.pioName2[0],sizeof(ss.pdata.pioName2)},
		{"REAL",5031,universal_TypeInt,(void *)&ss.pdata.LineSourceMode},	
		
		{"REAL",5032,universal_TypeInt,(void *)&ss.pa.DrawZonesColor},
		{"REAL",5033,universal_TypeInt,(void *)&ss.pd.AreaFill},
		
		{"DOUB",5034,universal_TypeDouble,(void *)&ss.pd.red},	
		{"DOUB",5035,universal_TypeDouble,(void *)&ss.pd.green},	
		{"DOUB",5036,universal_TypeDouble,(void *)&ss.pd.blue},	
		
		{"REAL",5037,universal_TypeInt,(void *)&ss.pd.sType},
		{"DOUB",5038,universal_TypeDouble,(void *)&ss.pd.sPmin},	
		{"DOUB",5039,universal_TypeDouble,(void *)&ss.pd.sPmax},	

		{"REAL",5040,universal_TypeLong,(void *)&ss.CurrentFrame},	
		
							
		{"STRI",5042,universal_TypeString,(void *)&ss.vector.vectorxName[0],sizeof(ss.vector.vectorxName)},
		{"STRI",5043,universal_TypeString,(void *)&ss.vector.vectoryName[0],sizeof(ss.vector.vectoryName)},
		{"STRI",5044,universal_TypeString,(void *)&ss.vector.vectorzName[0],sizeof(ss.vector.vectorzName)},
		
		
		{"DOUB",5045,universal_TypeDouble,(void *)&ss.plane.Plane_xloc},	
		{"DOUB",5046,universal_TypeDouble,(void *)&ss.plane.Plane_yloc},	
		{"DOUB",5047,universal_TypeDouble,(void *)&ss.plane.Plane_zloc},	
		
		
		{"REAL",5050,universal_TypeInt,(void *)&ss.streamline3d.streamOn},
		{"REAL",5051,universal_TypeLong,(void *)&ss.streamline3d.streamSteps},	
		{"REAL",5052,universal_TypeInt,(void *)&ss.streamline3d.streamGradients},
		
		{"REAL",5053,universal_TypeLong,(void *)&ss.streamline3d.xPlane.count},	
		{"REAL",5054,universal_TypeLong,(void *)&ss.streamline3d.xPlane.countMax},	
		
		{"REAL",5055,universal_TypeLong,(void *)&ss.streamline3d.yPlane.count},	
		{"REAL",5056,universal_TypeLong,(void *)&ss.streamline3d.yPlane.countMax},	
		
		{"REAL",5057,universal_TypeLong,(void *)&ss.streamline3d.zPlane.count},	
		{"REAL",5058,universal_TypeLong,(void *)&ss.streamline3d.zPlane.countMax},	
		
        /* missing numbers are array types at top for simple numbering*/
        
		{"REAL",5065,universal_TypeLong,(void *)&ss.pa.BlockSize},					
		
		
		{"REAL",5066,universal_TypeLong,(void *)&ss.streamline3d.streamPasses},	
		
	
		{"TABL",5200,universal_TypeTable,(void *)&Global2},	
				
	};
	

	static struct universalTableStruct Global= {
		"SagePIO3D parameters",universal_WindowSagePIO3D,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	pio3dPtr s;
	int ret;
	
	s=NULL;
	if(myIcon && (myIcon->WindowType == Pio3dType)){
		s=(pio3dPtr)myIcon->DWindow;
	}else{
	    myIcon=NULL;
	}

	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct pio3dInfo);

	Global2.tableDynamic=myIcon;
	Global2.tableDynamicSpace=sizeof(struct Icon);
	
	ret=1;
	if(flag == universal_SetDefaults){
	
	    if(s && myIcon){
			uGetPortBoundsGlobal(myIcon,&s->windowRect);
			if(moveUniversalTable(&Global,universal_MoveToStatic))goto ErrorOut;	
		}else{
		
			if(mm.infoSave.directory)cFree((char *)mm.infoSave.directory);
			mm.infoSave.directory=NULL;
			if(mm.infoLoad.directory)cFree((char *)mm.infoLoad.directory);
			mm.infoLoad.directory=NULL;
			mm.Files=NULL;
			
			mm.eraseRectSize=140;
			mm.Synchronize=0;
			mm.framesPerSecond=30;			
	
			zerol((char *)&mm.Cursor,sizeof(mm.Cursor));
			mm.Cursor.valueFlag=1;
			mstrncpy(mm.Cursor.valueformat,"%10.3e",8);
			mstrncpy(mm.Cursor.coordinateFormat,"%10.3e",8);
			mstrncpy(mm.Cursor.timeFormat,"%8.2e",8);
						
			uSetRect(&ss.windowRect,117,45,575,434);
			ss.Tool=zoomTek;
			ss.LoopBackAndForth=0;
			ss.LoopFlag=0;
			ss.pioIndex=0;
			mstrncpy(ss.pioName,"density",64);
			
			ss.ldata.LineSourceMode=0;
			ss.ldata.pioIndex2=0;
			mstrncpy(ss.ldata.pioName2,"pres",64);			
			
			ss.plane.Plane_type=Z_PLANE;
			ss.plane.Plane_xloc=0;
			ss.plane.Plane_yloc=0;
			ss.plane.Plane_zloc=0;
			
			ss.pa.DrawZones=0;
			ss.pa.DrawZonesColor=0;
			ss.pd.AreaFill=1;
			ss.pa.flagGradients=0;
			ss.pa.ZoneLevel=1;
			
			ss.pd.red=0.5;
			ss.pd.green=0.5;
			ss.pd.blue=0.5;
			
			ss.vector.arrowLength=0.1;
			ss.vector.arrowAngle=145.0;	
			ss.vector.vspace= -21;
			ss.vector.vfac=0.2;	
			ss.vector.vmax=0;	
			ss.vector.DrawVectors=0;	
			ss.vector.arrowHeads=1;						
			
			
			ss.pd.sType=0;
			ss.pd.sPmin=0;
			ss.pd.sPmax=1;	
			
			ss.CurrentFrame=0;
		
			mstrncpy(ss.vector.vectorxName,"xdt",32);						
			mstrncpy(ss.vector.vectoryName,"ydt",32);						
			mstrncpy(ss.vector.vectorzName,"zdt",32);			
			
			ss.streamline3d.streamOn=0;			
			ss.streamline3d.streamSteps=2000;			
			ss.streamline3d.streamGradients=1;
			
			ss.streamline3d.xPlane.count=0;
			ss.streamline3d.xPlane.countMax=0;						
			ss.streamline3d.xPlane.x=NULL;
			ss.streamline3d.xPlane.y=NULL;
						
			ss.streamline3d.yPlane.count=0;
			ss.streamline3d.yPlane.countMax=0;						
			ss.streamline3d.yPlane.x=NULL;
			ss.streamline3d.yPlane.y=NULL;
						
			ss.streamline3d.zPlane.count=0;
			ss.streamline3d.zPlane.countMax=0;						
			ss.streamline3d.zPlane.x=NULL;
			ss.streamline3d.zPlane.y=NULL;
						
			ss.pa.BlockSize=0;
			
						
		}
		ss.defaultsSet=TRUE;
		
	}else if(flag == universal_SaveData){		
		
	    
	    if(s){
	    	if(s->streamline3d.xPlane.x || s->streamline3d.xPlane.y){
	    		Local[0].space=s->streamline3d.xPlane.count;
	    		Local[1].space=s->streamline3d.xPlane.count;
	    	
	    		ss.streamline3d.xPlane.x=s->streamline3d.xPlane.x;
	    		ss.streamline3d.xPlane.y=s->streamline3d.xPlane.y;
	    	}else{
	    		Local[0].space=0;
	    		Local[1].space=0;
	    	
	    		ss.streamline3d.xPlane.x=NULL;
	    		ss.streamline3d.xPlane.y=NULL;
	    	}
	

	    	if(s->streamline3d.yPlane.x && s->streamline3d.yPlane.y){
	    		ss.streamline3d.yPlane.x=s->streamline3d.yPlane.x;
	    		ss.streamline3d.yPlane.y=s->streamline3d.yPlane.y;
	    		Local[2].space=s->streamline3d.yPlane.count;
	   	 		Local[3].space=s->streamline3d.yPlane.count;
	    	}else{
	    		ss.streamline3d.yPlane.x=NULL;
	    		ss.streamline3d.yPlane.y=NULL;
	    		Local[2].space=0;
	   	 		Local[3].space=0;
	    	}
		
	    	
	    	if(s->streamline3d.zPlane.x && s->streamline3d.zPlane.y){
	    		ss.streamline3d.zPlane.x=s->streamline3d.zPlane.x;
	    		ss.streamline3d.zPlane.y=s->streamline3d.zPlane.y;
	    		Local[4].space=s->streamline3d.zPlane.count;
	    		Local[5].space=s->streamline3d.zPlane.count;
	    	}else{
	    		ss.streamline3d.zPlane.x=NULL;
	    		ss.streamline3d.zPlane.y=NULL;
	    		Local[4].space=0;
	    		Local[5].space=0;
	    	}
	
	    }else{
	    	Local[0].space=0;
	    	Local[1].space=0;
	
	    	Local[2].space=0;
	   	 	Local[3].space=0;
	
	    	Local[4].space=0;
	    	Local[5].space=0;
	    	
			ss.streamline3d.streamOn=0;			
			ss.streamline3d.xPlane.count=0;			
			ss.streamline3d.yPlane.count=0;			
			ss.streamline3d.zPlane.count=0;			
	    }
	
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
	
	    Local[0].space=0;
	    Local[1].space=0;
	
	    Local[2].space=0;
	    Local[3].space=0;
	
	    Local[4].space=0;
	    Local[5].space=0;
	
		if(getUniversalTable(&Global,inOut))goto ErrorOut;				
		
		if(s){
		    if(Local[0].space && Local[1].space){
		        s->streamline3d.xPlane.x=ss.streamline3d.xPlane.x;
		        ss.streamline3d.xPlane.x=NULL;
		        Local[0].space=0;
		        s->streamline3d.xPlane.y=ss.streamline3d.xPlane.y;
		        ss.streamline3d.xPlane.y=NULL;
		        Local[1].space=0;
		    }else{
		        ss.streamline3d.xPlane.x=NULL;
		        Local[0].space=0;
		        ss.streamline3d.xPlane.y=NULL;
		        Local[1].space=0;
				ss.streamline3d.xPlane.count=0;			
		    }
		    
		    if(Local[2].space && Local[3].space){
		        s->streamline3d.yPlane.x=ss.streamline3d.yPlane.x;
		        ss.streamline3d.yPlane.x=NULL;
		        Local[2].space=0;
		        s->streamline3d.yPlane.y=ss.streamline3d.yPlane.y;
		        ss.streamline3d.yPlane.y=NULL;
		        Local[3].space=0;
		    }else{		    
		        ss.streamline3d.yPlane.x=NULL;
		        Local[2].space=0;
		        ss.streamline3d.yPlane.y=NULL;
		        Local[3].space=0;
				ss.streamline3d.yPlane.count=0;			
		    }

		    if(Local[4].space && Local[5].space){
		        s->streamline3d.zPlane.x=ss.streamline3d.zPlane.x;
		        ss.streamline3d.zPlane.x=NULL;
		        Local[4].space=0;
		        s->streamline3d.zPlane.y=ss.streamline3d.zPlane.y;
		        ss.streamline3d.zPlane.y=NULL;
		        Local[5].space=0;
		    }else{
		        ss.streamline3d.zPlane.x=NULL;
		        Local[4].space=0;
		        ss.streamline3d.zPlane.y=NULL;
		        Local[5].space=0;
				ss.streamline3d.zPlane.count=0;			
		    }

		}else{
			ss.streamline3d.xPlane.count=0;
			ss.streamline3d.yPlane.count=0;
			ss.streamline3d.zPlane.count=0;
			
	        ss.streamline3d.xPlane.x=NULL;
	        Local[0].space=0;
	        ss.streamline3d.xPlane.y=NULL;
	        Local[1].space=0;
	        ss.streamline3d.yPlane.x=NULL;
	        Local[2].space=0;
	        ss.streamline3d.yPlane.y=NULL;
	        Local[3].space=0;
	        ss.streamline3d.zPlane.x=NULL;
	        Local[4].space=0;
	        ss.streamline3d.zPlane.y=NULL;
	        Local[5].space=0;
		}
		
		doUniversalSagePIO3D(myIcon,universal_GetDefaults,NULL);
		
		if(myIcon && uPref.Files){
			doLoadImages(myIcon,uPref.Files);
			if(s->streamline3d.streamOn){
				SendStreamlineInfo(myIcon);   	
				s->CurrectFrameLoaded = -1;
				InvalRectMyWindow(myIcon);
			}
		    uPref.Files=NULL;
		}		
	}else if(flag == universal_GetDefaults){		
	    if(s){
			if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
			uMoveWindow(myIcon,s->windowRect.x,s->windowRect.y);
			uSizeWindowGlobal(myIcon,s->windowRect.xsize,s->windowRect.ysize);
			if(myIcon->WindowNameSet && uPref.config){
			    myIcon->WindowNameSet=FALSE;
				SetWindowName(myIcon,myIcon->WindowName);	
				myIcon->WindowNameSet=TRUE;
			}else{
			    GetWindowName(myIcon,myIcon->WindowName,0);
				myIcon->WindowNameSet=FALSE;
			}
			if(myIcon->pMoveControls){
			    (myIcon->pMoveControls)(myIcon);
			}			
			if(uPref.Cursor.count > 0){
			    myIcon->Cursor.count=uPref.Cursor.count;
			    myIcon->Cursor.Items=uPref.Cursor.Items;
			    uPref.Cursor.count=0;
			    uPref.Cursor.Items=NULL;
			}
	    }
	}else if(flag == universal_Stop){	
		if(freeUniversalTable(&Global))goto ErrorOut;				
	}else{
		sprintf(WarningBuff,"doUniversalSagePIO3D unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
#ifdef Macintosh
static void tPrint(IconPtr myIcon)
{
	extern Rect PageView;
	uRect sPageView;

	
	sPageView=uRectConvert(&PageView);
	
	tDumpItP(myIcon,&sPageView,1);	
}
static void tDumpItP(IconPtr myIcon,uRect *PageViewi,int scale)
{
	Rect PageView;
	Rect DRect;
	uRect uDRect;
	double saveScale;
	pio3dPtr t;
	
	if(myIcon && (t=(pio3dPtr)myIcon->DWindow)){
		PageView=RectConvert(PageViewi);
	    GetClip(myIcon->saveRegion);
	    ClipRect(&PageView);
		DRect.top = 0;
		DRect.left = 0;
  		DRect.right=(int)((double)t->range.ixmax*t->scale);
		DRect.bottom=(int)((double)t->range.iymax*t->scale);

		DRect.bottom = DRect.bottom/scale;
		DRect.right = DRect.right/scale;
		OffsetRect(&DRect,PageView.left,PageView.top);    
		
		saveScale=t->scale;
		t->scale /= scale;
		cLineSet(1,8);
		uDRect=uRectConvert(&DRect);
	    drawTek(myIcon,t->CurrentFrame,&uDRect);
		t->scale=saveScale;
		
	    SetClip(myIcon->saveRegion);
	}
}
#else
static void tPrint(IconPtr myIcon)
{
	//myIcon=myIcon;
}
static void tDumpItP(IconPtr myIcon,uRect *PageViewi,int scale)
{
	//myIcon=myIcon;
	//PageViewi=PageViewi;
	//scale=scale;
}
#endif
