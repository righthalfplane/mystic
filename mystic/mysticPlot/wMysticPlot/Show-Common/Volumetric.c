#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "Vrml.h"
#include "Universal.h"
#include "FileManager.h"
#include "TracerDef.h"
#include "uGraph.h"
#include "VoiceCommand.h"


struct rStackData{
    struct P Target;
    double Degrees;
};


struct volumetricInfo{
   struct PlotPlaneData plane;
    CKeyFrame CKeyFrames;
    struct FileInfo2 *Files;
    struct rStackData stack;
    pushpopPtr popStack;
    struct vScene *scene;
    IconPtr myIcon;
    IconPtr myIcon4;
    long CurrentFrame;
    unsigned char *buffer;
    long xsize;
    long ysize;
    int Tool;
    uPoint p1;
    uPoint p2;
    double theta1,theta2;
    uRect windowRect;
    int defaultsSet;
    int mat;
    int AnimationOn;
    long ImageCount;
    char BatchName[64];
    struct SetFrameData  sd;
    struct paletteDraw pd;
    char pioName[64];
    int pioIndex;
    unsigned long TimeToFlip;
    int  LoopBackAndForth;
    int LoopFlag;
    double scale;
    struct linedata pdata;
    struct linedata ldata;
	int WalkMotionContinuous;
	int WalkMotion;
    
    /*   save junk */
    int saveWhere;
    FILE *saveFILE;
    char *saveDirectory;
    char *saveName;
    char *saveFileName;
    struct FileInfo2 *saveFiles;
    /*  save junk end */
};

typedef struct volumetricInfo *volumetricPtr;


#define ControlModeDrag				1
#define ControlModeZoom				2


#define SUM(n)   (((n) & 1)  ? (((n)+2)*((n)/2)+1):(((n)+1)*((n)/2)))

int CenterScreenVol(IconPtr myIcon,int flag);

int PageSetup(IconPtr myIcon);

IconPtr CDrawNewLine(struct linedata *li,int count);

static int SetScreen(struct vScene *scene);

static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);

static int doOpacity(IconPtr myIcon);

static int n2SaveScrap(IconPtr myIcon);

static int pushStack(volumetricPtr r);

int doSetImageInfo(char *Name,char *Directory,int flag);

static int DrawAnimate(IconPtr myIcon,long CurrentFrame);

static int fixScene(volumetricPtr r);
static int GetPlotOptions(IconPtr myIcon);
static int nMouseMoved(IconPtr myIcon,uPoint where);

static int doRotate(IconPtr myIcon);
static int doSweep(IconPtr myIcon);
static int Rotate1Action(struct uDialogStruct *uList);

static int doView(IconPtr myIcon);
static int doView1Action(struct uDialogStruct *uList);

static int doMove(IconPtr myIcon);
static int doOptions(IconPtr myIcon);

int FilesOpenDefault(struct FileInfo2 *Files);

int TrueWriteMerge(char *Name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,FILE *out);
           
int TrueMergedDone(FILE *out);

int FilesSaveTRUEToMemory(char *name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,struct FileInfo2 *Files);
			  
static int doOrbit(IconPtr myIcon);
struct FileInfo2 *FilesTRUE(void);

static int CheckMenu(IconPtr myIcon,int flag);
static int doNULL(IconPtr myIcon);
int doEraseDown(IconPtr myIcon);

int Sleep2(int ticks);

int doTestSpeed(IconPtr myIcon);

static int doZoomDown(IconPtr myIcon);
static int doDragDown(IconPtr myIcon);
int findAngles(struct System *Local);
int Transform(struct P *p,struct Matrix *m);

int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);
static void tMoveControls(IconPtr myIcon);
static void tMoveControls2(IconPtr myIcon);
static void doFrameImage(IconPtr myIcon);
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect);

static int RenderVolume(IconPtr myIcon,int flag);

static int DefaultScene(struct vScene **scenei);
static int adjustPaletteV(struct vScene *scene,int mat);
static void drawIt(IconPtr myIcon);

int getPaletteReal(struct Color *ct,int n);
int fillct(struct Color *ct,unsigned char *palette);

static int doViewSend(IconPtr myIcon,char *name);
int doSageVolStart(struct FileInfo2 *Files);
IconPtr TableWindow3(int XCount,int YCount,int XLength,int YLength);

static void tPrint(IconPtr myIcon);
static void tDumpItP(IconPtr myIcon,uRect *PageViewi,int scale);
void cLineSet(int num,int dem);

static int Dump(struct vScene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData);


#define File_Menu  				7000
#define About_Item 				1
#define Open_Item 				2
#define Close_Item 				3
#define savePreferencesCommand	4
#define PreferencesCommand 		5
#define Quit_Item 				6
#define TekSavePlots			7
#define New_Item 				8
#define previewTekCommand 		9
#define setupTekCommand 		10
#define printTekCommand 		11



#define EDIT_MENU				7001
#define copyTekCommand			1
#define clearLoadTekCommand 	2
#define FRAME_IMAGE_ITEM		3
#define popItem					4
#define popAllItems				5
#define zoomIn					6
#define zoomOut					7
#define WINDOW_OPTIONS_ITEM		8
#define OPTIONS_ITEM			9
#define VIEW_ITEM				10
#define dataOpacity	 			11


#define OPERATIONS_MENU		7002
#define SET_DRAG_ITEM		1
#define ZOOM_ITEM			2

#define ANIMATIONS_MENU			7003
#define ORBIT_ITEM				1
#define MOVE_TARGET_ITEM		2
#define ROTATE_OBJECT_ITEM		3
#define DATA_SWEEP_OPACITY_ITEM	4


#define DataMenu4					7004
#define dataScalesTek	 			2
#define dataSelectionPointsTek	 	3
#define dataSelectionVolumeTek	 	4
#define dataHistogramVolume	 		5



#define NEW_Menu  					48

#define PalMenu4	1908



static int doClose(IconPtr myIcon);
static int doMenus(IconPtr myIcon,int menu,int item);
static int doLoadImages(IconPtr myIcon,struct FileList *Files);
static int UpdateWrite(IconPtr myIcon);

static int RenderGetMessage(IconPtr myIcon,long MessageType,void *MessageData);
static int UpdateWrite(IconPtr myIcon);


static int tMoveDy(IconPtr myIcon);
static int tgoDown(controlPtr control,short part,IconPtr myIcon);
static int tgoUp(controlPtr control,short part,IconPtr myIcon);
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int tdothumb(controlPtr control,IconPtr myIcon);

static int tInContent(IconPtr myIcon);

static int RenderKeyBoard(IconPtr myIcon,int c);

IconPtr  NewVolumetricWindow(IconPtr myIcon);

IconPtr  OpenVolumetricWindow(struct FileInfo2 *Files);

static int freeScene(struct vScene *scene);

static int SetFileStuff(IconPtr myIcon,struct FileInfo2 *Files2);

static int Count;
	
int FilesOpenVolumetric(struct FileInfo2 *files);

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
	
/*	
	{"Move Up",c_Move_Up},
	{"Move Down",c_Move_Down},
	{"Move Left",c_Move_Left},
	{"Move Right",c_Move_Right},
	{"Move Faster",c_Animate_Faster},
	{"Move Slower",c_Animate_Slower},
		
	
	
	{"Continue",c_Start},
*/
	{"Step Forward",c_Step_Forward},
	{"Step Backward",c_Step_Backward},
	
	{"Stop",c_Stop},
	{"Halt",c_Stop},
	{"Help",c_Help},
};
static long CommandLength=sizeof(Commands)/sizeof(struct VoiceCommands);


static int doCommands(IconPtr myIcon,int c);

static void DoActivate(IconPtr myIcon,int flag)
{
	volumetricPtr t;
	
	if(!myIcon)return;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return;

	StandardDoActive(myIcon,flag);
	
	if(flag){
		startLanguage(Commands,CommandLength,myIcon);
	}else{
		stopLanguage();
	}

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
	volumetricPtr r;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;	
	
	switch(c){	
	    case c_Animate_Forward:
	    	r->WalkMotion = 0;
			RenderKeyBoard(myIcon,rightArrow);
	    break;
	    case c_Animate_Backward:
	    	r->WalkMotion = 0;
			RenderKeyBoard(myIcon,leftArrow);
	    break;
	    case c_Animate_Faster:
	    	myIcon->framesPerSecond *= 2;
	    break;
	    case c_Animate_Slower:
	    	myIcon->framesPerSecond /= 2;
	    break;
	    case c_Step_Forward:
			RenderKeyBoard(myIcon,upArrow);
	    break;
	    case c_Step_Backward:
			RenderKeyBoard(myIcon,downArrow);
	    break;
	    case c_Zoom_In:
	        doMenus(myIcon,EDIT_MENU,zoomIn);
	    break;
	    case c_Zoom_Out:
	        doMenus(myIcon,EDIT_MENU,zoomOut);
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
	    case c_Start:
	    	r->WalkMotionContinuous = 1;	 
	   	break;   	
	    case c_Stop:
	    	r->WalkMotionContinuous = 0;	    	
	        r->AnimationOn = 0;
	    break;
	}
	return 0;
	
}
int FilesOpenVolumetric(struct FileInfo2 *files)
{
	if(!files)return 1;
	if(!OpenVolumetricWindow(files))return 1;
	return 0;
}
IconPtr  NewVolumetricWindow(IconPtr myIconIn)
{
	myIconIn=myIconIn;
	return  OpenVolumetricWindow(NULL);
}


IconPtr  OpenVolumetricWindow(struct FileInfo2 *Files)
{
	static struct uMenuItems File[]={
	    {"File",  		File_Menu,uWindowStandardMenu,0,},
	    {"New           /N",New_Item,0,0,},
	    {"Close         /W",Close_Item,0,0,},
	    {"----------------",About_Item,0,0,},
	    {"Save Preferences",savePreferencesCommand,0,0,},
	    {"Preferences...  ",PreferencesCommand,0,0,},
	    {"----------------",About_Item,0,0,},
	    {"Load Images.../L",Open_Item,0,0,},
	    {"Save Images...  ",TekSavePlots,0,0,},
	    {"----------------",About_Item,0,0,},
	    {"Print Preview...",previewTekCommand,0,0,},
	    {"Page Setup...",	setupTekCommand,0,0,},
	    {"Print...      /P",printTekCommand,0,0,},
	    {"----------------",About_Item,0,0,},
	    {"Quit          /Q",Quit_Item,0,0,},
	    {NULL,0,0,0,},
	};
	

	static struct uMenuItems Edit[]={
	    {"Edit",  					7001,uWindowStandardMenu,0,},
	    {"Copy               /C",   copyTekCommand,0,0,},
	    {"Clear and Load...    ",clearLoadTekCommand,0,0,},
	    {"---------------------",About_Item,0,0,},
	    {"Frame Image        /F",FRAME_IMAGE_ITEM,0,0,},
	    {"Pop One              ",popItem,0,0,},
	    {"Pop All              ",popAllItems,0,0,},
	    {"Zoom In              ",zoomIn,0,0,},
	    {"Zoom Out             ",zoomOut,0,0,},
	    {"---------------------",About_Item,0,0,},
	    {"Information...     /I",	WINDOW_OPTIONS_ITEM,0,0,},
	    {"Options...",				OPTIONS_ITEM,0,0,},
	    {"View...",					VIEW_ITEM,0,0,},
	    {"Data Opacity  ...",	dataOpacity,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Operations[]={
	    {"Tools",  			OPERATIONS_MENU,uWindowStandardMenu,0,},
	    
	    {"Drag Frames", 	SET_DRAG_ITEM,0,0,},
	    {"Zoom",			ZOOM_ITEM,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Animation[]={
	    {"Animation",  			    ANIMATIONS_MENU,uWindowStandardMenu,0,},
	    {"Orbit Target...", 		ORBIT_ITEM,0,0,},
	    {"Move Target...",			MOVE_TARGET_ITEM,0,0,},
	    {"Rotate Objects...",		ROTATE_OBJECT_ITEM,0,0,},
	    {"Sweep Opacity...",		DATA_SWEEP_OPACITY_ITEM,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Data[]={
	    {"Data",  				DataMenu4,uWindowStandardMenu,0,},
	    {"Data Range...    ", 	dataScalesTek,0,0,},
	    {"Data Selection...",	dataSelectionPointsTek,0,0,},
	    {"Select Volume... ",	dataSelectionVolumeTek,0,0,},
	    {"Histogram...     ",	dataHistogramVolume,0,0,},
	    
	    
	    
	    
	    
	    { NULL,0,0,0},
	};
	static struct uMenuItems Palette[]={
	    {"Palette",  				    PalMenu4,uWindowPaletteMenu,0,},
	    {"Load Palette...", 			1,0,0,},
	    {"Save Palette...", 			2,0,0,},
	    {"Add Palette...", 				3,0,0,},
	    {"Restore Palette   /R", 		4,0,0,},
	    {"Load Menu Palette", 			5,0,0,},
	    {"Synchronize Palettes", 		6,0,0,},
	    {"Palette To Table", 		    7,0,0,},
	    {"--------------------", 		8,0,0,},
	    { NULL,0,0,0},

	};
	
	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Operations},
	    {Animation},
	    {Data},
	    {Palette},
	    {NULL}
	};
	
	IconPtr myIcon;
	uRect Rect={40,40,400,200};
	char buff[256];
	struct vScene *scene;
	volumetricPtr r;
	int ret;
		
    ret=1;
	if(!(myIcon=uCreateIcon(VolumetricType,&Rect))){
		WarningBatch("NewVolumetricWindow uCreateIcon Failed\n");
		goto ErrorOut;
	}
	uSetCursor(myIcon,uWATCH_CURSOR);
	
	
	sprintf(buff,"VolumetricWindow.%03d",Count++);
	
	SetWindowName(myIcon,buff);			
	if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,
	              uWindowSetDone,uWindowSetDone)
				  )goto ErrorOut;
				  
				  
	if(!(r=(volumetricPtr)cMalloc((long)sizeof(struct volumetricInfo),8202))){
	    WarningBatch("NewVolumetricWindow Out of Memory\n");
	    goto ErrorOut;
	}
	zerol((char *)r,(long)sizeof(struct volumetricInfo));
	
	myIcon->DWindow=(struct DumWindow *)r;
	
	myIcon->pInContent=tInContent;
	myIcon->pCheckMenu=CheckMenu;
	myIcon->pClose=doClose;
	myIcon->pMenu=doMenus;
	myIcon->pMessage=RenderGetMessage;
	myIcon->pMoveControls=tMoveControls2;
	myIcon->pUpdateWrite=UpdateWrite;
	myIcon->pNullEvent=doNULL;
	myIcon->pPrint=tPrint;
	myIcon->uDump=tDumpItP;
	myIcon->pKeyBoard=RenderKeyBoard;
	myIcon->uDraw=drawTek;
	myIcon->uDrawAnimate=DrawAnimate;
	myIcon->pMouseMoved=nMouseMoved;
	myIcon->uSpeech=SpeechCommand;	
	myIcon->pDoActivate=DoActivate;
	
	myIcon->pPageUp=tdoPageUp;
	myIcon->pPageDown=tdoPageDown;
	myIcon->pLineUp=tgoUp;
	myIcon->pLineDown=tgoDown;
	myIcon->pThumb=tdothumb;
	
	myIcon->paletteMenuNumber=PalMenu4;
		
	myIcon->eraseRectSize=120;
	
	myIcon->mMenuOffSet=MenuOffSet;

	r->popStack=pushpopStart(&r->stack);
	if(!r->popStack)goto ErrorOut;		
				  
	scene=NULL;
	
	if(DefaultScene(&scene))goto ErrorOut;
	if(ScreenSetSize(scene->xResolution,scene->yResolution,myIcon))goto ErrorOut;
	r->scene=scene;
	
	r->myIcon=myIcon;
	r->Tool=ControlModeZoom;
	
	r->pd.sType=0;
	r->pd.sPmin=0;
	r->pd.sPmax=1;	
	r->pd.LabeledPalette=FALSE;	
	r->pd.paletteFont=2;
	
	r->pdata.CurrentFrame = -1;
	
	r->scale=1.0;
	
	myIcon->framesPerSecond=30;
	
	r->mat=1;
		
	SetFileStuff(myIcon,Files);	
	
	if(!IsShift()){
	    doUniversalVolumetric(myIcon,universal_GetDefaults,NULL);
		doFrameImage(myIcon);
	}
	    		
	tMoveControls(myIcon);
	
	InvalRectMyWindow(myIcon);
	doFrameImage(myIcon);

    StandardDoActive(myIcon,TRUE);

	CheckMenu(myIcon,TRUE);
	    
	GetPaletteByName("Ps",myIcon->red,myIcon->green,myIcon->blue);
	SetDefaultPaletteName(myIcon,"Ps");

	if(Files){
	    setInfo(Files->directory,&myIcon->infoLoad);
	    setInfo(Files->directory,&myIcon->infoSave);
	}

		
    ret=0;
ErrorOut:
    if(ret){
        if(myIcon)doClose(myIcon);
	}else{
		if(myIcon){
		    myIcon->isOpen=TRUE;
		}
	}

	uSetCursor(myIcon,uARROW_CURSOR);
	return myIcon;
}
static int RenderKeyBoard(IconPtr myIcon,int c)
{
	volumetricPtr r;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	
	switch(c){
	    case upArrow:
	        if(++(r->CurrentFrame) >= r->ImageCount)r->CurrentFrame = 0;
			RenderVolume(myIcon,1);
	        InvalRectMyWindow(myIcon);

	        r->AnimationOn = 0;
	        break;
	    case downArrow:
	        if(--(r->CurrentFrame) < 0)r->CurrentFrame = r->ImageCount-1;
			RenderVolume(myIcon,1);
	        InvalRectMyWindow(myIcon);

	        r->AnimationOn = 0;
	        break;
	    case leftArrow:
	        r->AnimationOn = -1;
	        break;
	    case rightArrow:
	        r->AnimationOn = 1;
	        break;
	    case 't': 
	         doTestSpeed(myIcon);	    
	         break;
	}
	
	

	return 0;
}
static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
    struct FileInfo2 *Files;
	volumetricPtr t;

	if(!myIcon)return 1;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
  	if(!Files){
	    uSetCursor(myIcon,uARROW_CURSOR);
  	    return 1;
	}
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {	
	   if(t->Tool == ControlModeZoom){
		   uSetCursor(myIcon,uPLUS_CURSOR);
	    }else{
		   uSetCursor(myIcon,uARROW_CURSOR);
	    }
	}else{	
		uSetCursor(myIcon,uARROW_CURSOR);	
	}		
	return 0;	
}
static int DrawAnimate(IconPtr myIcon,long CurrentFrame)
{
	volumetricPtr s;
	uPort save;
	
	if(!myIcon)return 1;
	s=(volumetricPtr)myIcon->DWindow;
	if(!s)return 1;

	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	if(s->CurrentFrame != CurrentFrame){
	    s->CurrentFrame=CurrentFrame;
	    RenderVolume(myIcon,1);
	}
		
	drawIt(myIcon);	
		
	uSetPort(&save);
	return 0;
}
static int doNULL(IconPtr myIcon)
{
	volumetricPtr t;
    struct FileInfo2 *Files;

	if(!myIcon)return 1;
	t=(volumetricPtr)myIcon->DWindow;
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
	   RenderVolume(myIcon,1);
       InvalRectMyWindow(myIcon);
/*       
       if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
*/	       
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
static int CheckMenu(IconPtr myIcon,int flag)
{
	volumetricPtr t;


	if(!myIcon)return 1;
	t=(volumetricPtr)myIcon->DWindow;
		
	if(flag){
	    
		if(!t)return 1;
	    					
		uCheckMenuItem(myIcon,OPERATIONS_MENU,SET_DRAG_ITEM,      (t->Tool == ControlModeDrag));
		uCheckMenuItem(myIcon,OPERATIONS_MENU,ZOOM_ITEM, 		  (t->Tool == ControlModeZoom));
	    CWindowSetPalMenu(myIcon);	    
/*	    
	    uDisableMenuItem(myIcon,DataMenu4,dataSelectionVolumeTek);
*/	    
	    doWindowMenu();
	}
	return 0;
}
static int pushStack(volumetricPtr r)
{
   	struct vScene *scene;
   	
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	r->stack.Target=scene->Target;
	r->stack.Degrees=scene->Degrees;
	
	return pushpopPush(&r->stack,r->popStack);
}
static int doClear(IconPtr myIcon)
{
	uRect portRect;
	volumetricPtr t;

	if(!myIcon)return 1;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return 1;
	
	uGetPortBounds(myIcon,&portRect);
	uClipRect(&portRect,myIcon);
	uEraseRect(&portRect,myIcon);
	
	if(t->BatchName[0])FileManagerCloseName(t->BatchName);
	
	t->BatchName[0]=0;
	
	myIcon->Files=NULL;
	t->ImageCount=0;
	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return 0;
}

static int doHistogramStart(IconPtr myIcon)
{
	struct linedata *li;
	volumetricPtr t;
	
	if(!myIcon || !(t=(volumetricPtr)myIcon->DWindow))return 1;
	li= &t->pdata;
	
	

	if(li->CurrentFrame < 0){
	    li->CurrentFrame=0;
	    li->FirstFrame=0;
	    li->LastFrame=t->ImageCount-1;
	    li->FirstTime=0;
	    li->LastTime=(double)(t->ImageCount-1);
	    li->FrameCount=t->ImageCount;	    
	    li->PointLineCount=200;
	}
	li->type=LINEDATA_HISTOGRAM_DATA;
	GetWindowName(myIcon,li->sourceName,0);
	li->Plot=NULL;
	li->CurrentFrame=t->CurrentFrame;
	li->replaceData = -1;
	
	li->pioIndex=t->pioIndex;
	mstrncpy(li->pioName,t->pioName,255);
	
	
	if(CDrawNewLine(li,1))return 1;
	
	
	return 0;
}
static int doMenus(IconPtr myIcon,int menu,int item)
{
   struct PlotPlaneData plane;
   	struct vScene *scene;
	volumetricPtr r;
	uRect Box;
	int type;
	
	if(!myIcon || !(r=(volumetricPtr)myIcon->DWindow))return 1;
	scene=r->scene;
	if(!scene)return 1;
     
	Box.x = 0;
	Box.y = 0;
	Box.xsize=(int)(myIcon->uviewRect.xsize);
	Box.ysize=(int)(myIcon->uviewRect.ysize);
  		     
	uSetCursor(myIcon,-uWATCH_CURSOR);
	
		switch(menu){
		case AppleMenu:
			if(item == 1){
			    doAbout(myIcon);
			    break;
			}
		break;
		case File_Menu:
		    if(item == Open_Item){
		        doLoadImages(myIcon,NULL);
		    }else if(item == Close_Item){
		        doClose(myIcon);
		  		myIcon=NULL;
			} else if(item == previewTekCommand){
				PagePreview(myIcon);
			} else if(item == setupTekCommand){
				PageSetup(myIcon);
			} else if(item == printTekCommand){
		    Print(myIcon);
			} else if(item == New_Item){
		    	NewNewWindow(myIcon);
			} else if(item == savePreferencesCommand){		
		    	writeUniversal(myIcon,0);
			} else if(item == PreferencesCommand){		
				saveUniversal(myIcon);
		    }else if(item == Quit_Item){
		        doQuit();
		  		myIcon=NULL;
			} else if(item == TekSavePlots){
				doSavePlots(myIcon,myIcon->Files,r->CurrentFrame,&Box);
			}
		    
		break;
		
		case EDIT_MENU:
			switch(item){
 			case clearLoadTekCommand:
 				doClear(myIcon);
				doLoadImages(myIcon,NULL);
 			break;
 			case FRAME_IMAGE_ITEM:
				doFrameImage(myIcon);
				break;
 			case WINDOW_OPTIONS_ITEM:
		  		doWindowInformation(myIcon);
				break;
 			case OPTIONS_ITEM:
		  		doOptions(myIcon);
				break;
 			case VIEW_ITEM:
		  		doView(myIcon);
				break;
 			case copyTekCommand:
 				n2SaveScrap(myIcon);
				break;
				
 			case popItem:
 		   		if(pushpopPop(&r->stack,r->popStack))goto OutOfHere;
POP:		  
		  		scene->Target=r->stack.Target;
		  		scene->Degrees=r->stack.Degrees;
				if(myIcon)RenderVolume(myIcon,1);
 			 	break;
								
 			case popAllItems:
 		   		if(pushpopPopAll(&r->stack,r->popStack))goto OutOfHere;
		   		goto POP;
		   				   		
 			case zoomOut:	
		    	if(pushStack(r)){
					break;
		    	}
				scene->Degrees *= 2;
				if(myIcon)RenderVolume(myIcon,1);
				break;		   		
		   		
 			case zoomIn:	
		    	if(pushStack(r)){
					break;
		    	}
				scene->Degrees /= 2;
				if(myIcon)RenderVolume(myIcon,1);
				break;		   		
  		case dataOpacity:	  
		
			doOpacity(myIcon);
			
  		    break;
		   		
		    }
		break;
		
		case OPERATIONS_MENU:
			if(item == SET_DRAG_ITEM){
		        r->Tool=ControlModeDrag;
		    }else if(item == ZOOM_ITEM){
		        r->Tool=ControlModeZoom;
		    }
		break;
		
		case ANIMATIONS_MENU:
		    if(item == ORBIT_ITEM){
		  		doOrbit(myIcon);
		    }else if(item == MOVE_TARGET_ITEM){
		  		doMove(myIcon);
		    }else if(item == ROTATE_OBJECT_ITEM){
		  		doRotate(myIcon);
		    }else if(item == DATA_SWEEP_OPACITY_ITEM){
		  		doSweep(myIcon);
		    }
		break;	
		
		case NEW_Menu:
		  pNewMenu(myIcon,item);
		break;
		
	case DataMenu4:
		switch(item){
 		case dataScalesTek:
  		    type = MessageType_DO_SCALES_DIALOG;
			if(SendMessageByName(r->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(r->BatchName,MessageType_GET_INFORMATION,&r->sd)){
				goto OutOfHere;
			}
			r->pd=r->sd.pd;
			RenderVolume(myIcon,1);
  		    break;
  		case dataSelectionPointsTek:	  
  		    type = MessageType_DO_SELECTION_DIALOG;
			if(SendMessageByName(r->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			
			if(SendMessageByName(r->BatchName,MessageType_GET_INFORMATION,&r->sd)){
				goto OutOfHere;
			}
			
			if(r->sd.pioName[0]){
	    		mstrncpy(r->pioName,r->sd.pioName,255);
	    		r->pioIndex=r->sd.pioIndex;			    
			}						
			RenderVolume(myIcon,1);			
  		    break;
 		case dataSelectionVolumeTek:	  
 			plane=r->plane;
		    type = MessageType_DO_AREA_DIALOG;
			if(SendMessageByName(r->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			if(SendMessageByName(r->BatchName,MessageType_GET_INFORMATION,&r->sd)){
				goto OutOfHere;
			}
			if(
			   plane.Plane_xmin != r->sd.plane.Plane_xmin ||
			   plane.Plane_ymin != r->sd.plane.Plane_ymin ||
			   plane.Plane_zmin != r->sd.plane.Plane_zmin ||
			   plane.Plane_xmax != r->sd.plane.Plane_xmax ||
			   plane.Plane_ymax != r->sd.plane.Plane_ymax ||
			   plane.Plane_zmax != r->sd.plane.Plane_zmax
			){
 				r->plane=r->sd.plane;
				RenderVolume(myIcon,1);
			}
   		    break;  		   
 		case dataHistogramVolume:	  
 		    doHistogramStart(myIcon);
   		    break;  		      		     
  		     
		}
		
		
		
		break;
		
		
		
		
	case PalMenu4:
		CWindowdoPalMenu(myIcon,menu,item);
	    {
	    	if(r->scene){
	        	
	        	mstrncpy(r->scene->palname,myIcon->DefaultPaletteName,255);

				if(myIcon)RenderVolume(myIcon,1);
	        }
	    }
		break;
		
		case WindowMenu:
			pWindowMenu(myIcon,item);
			break;
					
			}  
OutOfHere:		   
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int doOptions(IconPtr myIcon)
{
	return GetPlotOptions(myIcon);
}
static int tInContent(IconPtr myIcon)
{
	volumetricPtr r;
	uRect cBox;
	
	
	if(!myIcon || !(r=(volumetricPtr)myIcon->DWindow))return 1;

	cBox=myIcon->eraseRect;
	cBox.x = cBox.x+cBox.xsize-4;
	cBox.xsize = 4;
	cBox.ysize = 4;

	uPointer(&r->p1,myIcon);
		
	if(uPtInRect(r->p1,&myIcon->uviewRect)){	
		if(r->Tool == ControlModeZoom){
			doZoomDown(myIcon);
		} else if(r->Tool == ControlModeDrag){
			doDragDown(myIcon);
		} 	
	
		if(r->AnimationOn){	  
			r->AnimationOn=FALSE;
			InvalRectMyWindow(myIcon);
		}		      
	} else if(uPtInRect(r->p1,&cBox)){
	    doEraseDown(myIcon);
	} else {
	   uDoControls(myIcon);
	}
	return 0;
}
static int doDragDown(IconPtr myIcon)
{
    FilePtr Files;
	uPoint ptStart,pt;
	long CurrentFrame;
	int dx,dxold;
   	struct vScene *scene;
   	char *name;
	volumetricPtr r;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	
	scene=r->scene;
	if(!scene)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	
	CurrentFrame=r->CurrentFrame;
	
	dxold=0;
	uPointer(&ptStart,myIcon);
	while(uStillDown(myIcon)){
		uPointer(&pt,myIcon);
	    dx=pt.x-ptStart.x;	
	    if(dx == dxold)continue;  
	    dxold = dx;  
	    CurrentFrame = Files->ImageCount/2 + dx;
	    if(CurrentFrame >= Files->ImageCount){
	        CurrentFrame=Files->ImageCount-1;
	    }else if(CurrentFrame < 0){
	       CurrentFrame=0;
	    }
	    
		name=FilesImageName(myIcon->Files,CurrentFrame);
		nWriteBox(myIcon,name);

	}
	
	if(CurrentFrame != r->CurrentFrame){
	    r->CurrentFrame=CurrentFrame;
		RenderVolume(myIcon,1);	    
	}
	
	InvalRectMyWindow(myIcon);

	return 0;

}
static int doZoomDown(IconPtr myIcon)
{
   	struct vScene *scene;
   	struct P diff;
	struct P p1s,V1,V2;
	uPoint where;
	uRect r;
	volumetricPtr rr;
	int rastorY,xshift,yshift;
	long ix,iy;
	double Degrees,Length;
	
	if(!myIcon)return 1;
	rr=(volumetricPtr)myIcon->DWindow;
	if(!rr)return 1;
	
	scene=rr->scene;
	if(!scene)return 1;
	
	
	xshift = -myIcon->uviewRect.x;
	yshift = -myIcon->uviewRect.y;
	rastorY=(int)(scene->yResolution-1);
	
	uPointer(&where,myIcon);
	
	if(drawBoxOnScreen(myIcon,&r,NULL))return 1;
	
	if(r.xsize < 2 || r.ysize < 2)return 1;
	
	ix=r.x+r.xsize/2+xshift;
	if(ix < 0 || ix >= scene->xResolution)return 1;
	iy=r.y+r.ysize/2+yshift;
	if(iy < 0 || iy >= scene->yResolution)return 1;

	pushStack(rr);
	
	diff=Sub(&scene->Target,&scene->Eye);
	Length=Len(&diff);
	

	SetScreen(scene);
	
	p1s=scene->pScreen;
	
	V1=Mult(&scene->V1,scene->xmin+((double)ix+.5)*scene->dx);
	
	V2=Mult(&scene->V2,scene->ymin+((double)(rastorY-iy)+.5)*scene->dy);
	

	p1s=Add(&p1s,&V1);
	p1s=Add(&p1s,&V2);
	
	diff=Sub(&p1s,&scene->Eye);
	diff=Norm(&diff);
	diff=Mult(&diff,Length);
	
	scene->Target=Add(&scene->Eye,&diff);
	
	Degrees=scene->Degrees;
	
	if((double)scene->xResolution/(double)r.xsize <  (double)scene->yResolution/(double)r.ysize){
	    Degrees *= ((double)r.xsize/(double)scene->xResolution);
	}else{
	    Degrees *= ((double)r.ysize/(double)scene->yResolution);
	}
	
	scene->Degrees=Degrees;
	    
	
	RenderVolume(myIcon,1);

	return 0;

}
static int SetScreen(struct vScene *scene)
{
	double xmin,xmax,ymin,ymax,delx,dely,odx,ody;
	struct P EyeToTarget,V1,V2,V3,pScreen,V4;
	
	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);
	
	    /* Local x Direction */
	V1=CrossN(&EyeToTarget,&scene->Up);
	
	    /* Local y Direction */
	V2=CrossN(&V1,&EyeToTarget);
	
	    /* Local z Direction */
	V3=Mult(&EyeToTarget,-1.0);
	
	pScreen=Mult(&EyeToTarget,scene->Height);
	
	   /* Point At Center of Screen */
	pScreen=Add(&pScreen,&scene->Eye);
	
   	scene->V1=V1;
   	scene->V2=V2;
   	scene->V3=V3;
   	scene->pScreen=pScreen;
   	
   	V4=Mult(&V1,0.5*scene->EyeSeparation);
   	scene->EyeRight=Add(&V4,&scene->Eye);
   	
   	V4=Mult(&V1,-0.5*scene->EyeSeparation);
   	scene->EyeLeft=Add(&V4,&scene->Eye);
   	
    ymax = (double)(scene->Height*tan((scene->Degrees*4.0*atan(1.0))/360.));
	ymin = -ymax;

    xmax = (double)(ymax*scene->AspectRatio*(double)(scene->xResolution)/(double)(scene->yResolution));
	xmin = - xmax;

	delx=xmax-xmin;
	dely=ymax-ymin;

	odx=(double)(scene->xResolution)/delx;	
	ody=(double)(scene->yResolution)/dely;	
	
	scene->delx=delx;
	scene->dely=dely;
	
	scene->odx=odx;
	scene->ody=ody;
	
	scene->dx=1.0/odx;
	scene->dy=1.0/ody;
	
	scene->xmin=xmin;
	scene->ymin=ymin;

	scene->xmax=xmax;
	scene->ymax=ymax;   	
   	
	return 0;
}
static int UpdateWrite(IconPtr myIcon)
{
	    uRect portRect;
		uPort save;

		if(!myIcon || !myIcon->isOpen)return 1;
		
		uGetPort(&save);
		SetPortMyWindow(myIcon);
	
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    /* uEraseRect(&portRect,myIcon); */
	
		drawIt(myIcon);
		
	    uDrawControls(myIcon);
	    uDrawGrowIcon(myIcon);
	    
		uSetPort(&save);
	    return 0;
}
static int n2SaveScrap(IconPtr myIcon)
{
	struct vScene *scene;
	volumetricPtr t;
	uRect Box;


	if(!myIcon)return 1;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return 1;
	scene=t->scene;
	if(!scene)return 1;
		  	
  	Box.x = 0;
  	Box.y = 0;
  	Box.xsize=(int)(myIcon->uviewRect.xsize);
  	Box.ysize=(int)(myIcon->uviewRect.ysize);
  	  	
	return drawToScrap(myIcon,&Box,t->CurrentFrame,1);
}
static void drawIt(IconPtr myIcon)
{
		struct vScene *scene;
	    char buffscr[256];
        uRect portRect;
	    volumetricPtr t;
		uRect Box;
	    char *name;
	    
		if(!myIcon)return;
		t=(volumetricPtr)myIcon->DWindow;
		if(!t)return;
		scene=t->scene;
		if(!scene)return;
		
	    uClipRect(&myIcon->uviewRect,myIcon);
	    
  		Box.x = 0;
  		Box.y = myIcon->uviewRect.y;
  		Box.xsize=(int)(myIcon->uviewRect.xsize);
  		Box.ysize=(int)(myIcon->uviewRect.ysize);
  		
  		
		drawTek(myIcon,t->CurrentFrame,&Box);
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    
		name=FilesImageName(myIcon->Files,t->CurrentFrame);

		if(name){
			sprintf(buffscr,"%s",name);
			nWriteBox(myIcon,buffscr);
		}
	    
	    
}
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
		struct FileInfo2 *Files;
		struct vScene *scene;
	    uRect SRect,LRect;
	    int top,left,dx,dy;
	    volumetricPtr s;
	    long xsize,ysize;
	    
		if(!myIcon)return 1;
		s=(volumetricPtr)myIcon->DWindow;
		if(!s)return 1;
		Files=myIcon->Files;
		if(!Files)return 1;
		scene=s->scene;
		if(!scene)return 1;
		if(!DRect)return 1;
		
		if(s->CurrentFrame != CurrentFrame){
		    s->CurrentFrame=CurrentFrame;
		    RenderVolume(myIcon,0);
		}
				
		xsize=DRect->x+DRect->xsize;
		if(xsize < scene->xResolution)xsize = scene->xResolution;
		
		ysize=DRect->y+DRect->ysize;
		if(ysize < scene->yResolution)ysize = scene->yResolution;
							
	    if(SetBuffers(xsize*ysize*3+512L))return 1;
	    
	   /* ScreenSetSize(scene->xResolution,scene->xResolution,myIcon); */
	   
	    ScreenSetSize(xsize,ysize,myIcon);
	    
	    LRect= *DRect;
	    
	    dx=LRect.xsize;
	    dy=LRect.ysize;
	    
	    top=0;
	    left=0;

		if(myIcon->VControl)top=(int)uGetControlValue(myIcon->VControl);
		if(myIcon->HControl)left=(int)uGetControlValue(myIcon->HControl);

        SRect.y=top;
        if(SRect.y < 0)SRect.y=0;
        if(SRect.y > s->ysize)SRect.y=(int)s->ysize;
        
        SRect.ysize = dy;
        if(SRect.ysize < 0)SRect.ysize=0;
        if(SRect.y+SRect.ysize > s->ysize)SRect.ysize=(int)(s->ysize-SRect.y);
        
        
        SRect.x=left;
        if(SRect.x < 0)SRect.x=0;
        if(SRect.x > s->xsize)SRect.x=(int)s->xsize;
        
        SRect.xsize = dx;
        if(SRect.xsize < 0)SRect.xsize=0;
        if(SRect.x+SRect.xsize > s->xsize)SRect.xsize=(int)(s->xsize-SRect.x);
        
	     LRect.xsize=SRect.xsize;
	    
	    LRect.ysize=SRect.ysize;
	    
		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
		    	    
		uScreenWrite(s->xsize,s->ysize,(unsigned char *)s->buffer,(unsigned char *)xg.buffout,
		            ScreenDataType24,&SRect,&LRect,myIcon);
	    	    			    
		return 0;
}


static int RenderGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct Message1000 *m;
	uPort save;
	struct vScene *scene;
	volumetricPtr s;
	struct FileInfo2 *Files;
	struct linedata *li;
	int ret;
	long n;

	if(!myIcon)return 1;
	s=(volumetricPtr)myIcon->DWindow;
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	
	
	m=(struct Message1000 *)MessageData;

	ret = 1;
	
	uGetPort(&save);
	
	SetPortMyWindow(myIcon);

	switch(MessageType){
	case MessageType_LIMITS:
	    break;
	case MessageType_SET_FRAME_NUMBER:
	    if(!myIcon->Synchronize)goto OK;
	    if(!m)goto ErrorOut;
	    /*
	    s->CurrentFrame=m->CurrentFrame;
	    */
	    break;
	case MessageType_SET_PALETTE:
	/*
		SetDefaultPaletteName(myIcon,myIcon->DefaultPaletteName);
     */
     	break;
	case MessageType_GET_FRAME_COUNT:
		Files=myIcon->Files;
		if(Files){
			if(Files->ImageCount > m->FrameCount)m->FrameCount=Files->ImageCount;
		}
		goto OK;
	case MessageType_GET_AREA_DATA:
	    /* ai=MessageData;
	     ret = doAreaGetData(ai); */
	    goto ErrorOut;
	case MessageType_SET_AREA_RANGE:
	   /* ai=MessageData;
	    ret = doSetAreaRange(ai); */
	    goto ErrorOut;
	case MessageType_GET_LINE_DATA:
	    li=MessageData;
	    ret = SendMessageByName(s->BatchName,MessageType_GET_LINE_DATA,li);
	    goto OK;
	case MessageType_GET_FILE_LIST:
	    {
		    int isFound;
		    
			if(!myIcon->Files)goto OK;
			
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
	case MessageType_PRINT_FILE_INFORMATION:
	    printFileInformation(myIcon->Files);
		goto OK;
	}
	InvalRectMyWindow(myIcon);

OK:
	ret = 0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
static int DefaultScene(struct vScene **scenei)
{
	double xmin,xmax,ymin,ymax,zmin,zmax;
	struct vScene *scene;
	int n;
	
    xmin=ymin=zmin=-400;
    xmax=ymax=zmax=400;
	    
	if(*scenei){
	    scene = *scenei;	    
	}else{
	    scene=(struct vScene *)cMalloc(sizeof(struct vScene),8622);
	    if(scene == NULL)return 1;
	    zerol((char *)scene,sizeof(struct vScene));
	    *scenei=scene;
	    {
	        unsigned char pal[256*3];
	        getPaletteByName("ps",pal);
	        for(n=0;n<3*256;++n){
	            scene->pd.palette[n]=pal[n];
	        }
	    }
	    scene->pd.sType=0;
	    scene->pd.sPmin=-2;
	    scene->pd.sPmax=10;
	    scene->pd.LabeledPalette=FALSE;	
	    scene->pd.paletteFont=2;
	}

	scene->xResolution=300;
	scene->yResolution=300;

	scene->BackGround=c(1.0,1.,1.);

	scene->Degrees=20.;
	scene->AspectRatio=1.;
	scene->Height=1.;
	
	scene->Up=p(0.,0.,1.);
	
	scene->Eye=p(xmax+2.*(xmax-xmin),
	                 ymax+2.*(ymax-ymin),
	                 zmax+2.*(zmax-zmin));
	                 
	scene->Target=p(.5*(xmax+xmin),
	                 .5*(ymax+ymin),
	                 .5*(zmax+zmin));	

	scene->Lamp=scene->Eye;
	
	mstrncpy(scene->sequenceName,"true",255);
	
	mstrncpy(scene->palname,"ps",255);
	
	scene->sequenceSave=TRUE;

	scene->tcount=2;
	scene->tvalue[0]=1;
	scene->tvalue[1]=10;
	
	scene->topacity[0]=1;
	scene->topacity[1]=1;
	
	return 0;
}
static int adjustPaletteV(struct vScene *scene,int mat)
{

	if(!scene)return 1;
	mat=mat;
		
	return 0;
}
static int doLoadImages(IconPtr myIcon,struct FileList *Files)
{
	struct FileInfo2 *Files2;
	int TypeList[]={SDS3D_DISK,SDS3D_MEMORY,SAGE3D_DISK,0};
	struct vScene *scene;
	volumetricPtr t;
	
	if(!myIcon)return 1;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return 1;
	scene=t->scene;
	if(!scene)return 1;
	
	if(!Files){
		Files=uOpenFileDialog(myIcon,TypeList);		
		if(!Files)return 1;
	}
	
	Files2=FilesOpenList(Files);
	if(!Files2)return 1;
	
	if(myIcon->Files){
	    if(!OpenVolumetricWindow(Files2))return 1;
	    return 0;
	}
		
	if(SetFileStuff(myIcon,Files2))return 1;
		
	if(scene->AutoCenterScreen){	
		CenterScreenVol(myIcon,0);
	}
	
	return RenderVolume(myIcon,1);
}
static int SetFileStuff(IconPtr myIcon,struct FileInfo2 *Files2)
{
	struct CommandInfo Commands;
	volumetricPtr r;
	char buff[256];
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	
	
	if(Files2){
		myIcon->Files=Files2;
		r->ImageCount=Files2->ImageCount;

		sprintf(buff,"VolumetricWindow_%03d",Count++);			
		mstrncat(buff,".b",255);
		mstrncpy(r->BatchName,buff,255);
		if(FileManagerAddToList(Files2,r->BatchName)){
		    WarningBatch("SetFileStuff  Error FileManagerAddToList");
		    return 1;
		}
	    SetWindowToFileName(myIcon,myIcon->Files);
	    
	    doSageVolStart(Files2);
	    

		Commands.command[0]="dataType";
		Commands.type[0]=BATCH_STRING;

		Commands.command[1]=&r->pioName[0];
		Commands.type[1]=BATCH_STRING;


	 	Commands.value[2]=r->pioIndex;
		Commands.type[2]=BATCH_DOUBLE;


		Commands.nword=3;

		SendMessageByName(r->BatchName,MessageType_SEND_COMMAND_DATA,&Commands);
	    	    
		if(SendMessageByName(r->BatchName,MessageType_GET_INFORMATION,&r->sd)){
			goto ErrorOut;
		}
		
		if(r->CurrentFrame > r->sd.CurrentFrame){
		    r->CurrentFrame=0;
		}
	
		r->plane=r->sd.plane;
	    
	}
	
ErrorOut:
		
	return 0;
}
static int doClose(IconPtr myIcon)
{
	struct CommandInfo Commands;
	volumetricPtr r;
	long nn;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
		
	stopLanguage();

	if(r){
	    
		zerol((char *)&Commands,sizeof(Commands));
		nn=0;
		Commands.command[nn]="end";
		Commands.type[nn++]=BATCH_STRING;		
		Commands.nword=(int)nn;

		SendMessageByName(r->BatchName,MessageType_SEND_COMMAND_DATA,&Commands);
	
	    
	    
	    if(r->buffer)cFree((char *)r->buffer);
	    r->buffer=NULL;

	    myIcon->Closing=TRUE;

	    if(r->scene){
	        freeScene(r->scene);
	    }		
		
		if(r->BatchName[0])FileManagerCloseName(r->BatchName);
		
		r->BatchName[0]=0;
		
	    myIcon->Files=NULL;
		    
		pushpopEND(r->popStack);
				
	    cFree((char *)r);
	    myIcon->DWindow=NULL;
	}
	
	myIcon=RemoveIcon(myIcon);
	    
	return 0;
}
static int freeScene(struct vScene *scene)
{
	
	if(!scene)return 1;
	cFree((char *)scene);
	return 0;
}
static void doFrameImage(IconPtr myIcon)
{
	struct vScene *scene;
	int xsize,ysize;
	long xsizep,ysizep;
	volumetricPtr t;


	if(!myIcon)return;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return;
	scene=t->scene;
	if(!scene)return;
	

	xsize=(int)scene->xResolution;
	ysize=(int)(scene->yResolution);
	
	
	uGetPaletteSize(&xsizep,&ysizep,&scene->pd,myIcon);
	xsize += xsizep;
	if(ysizep > ysize)ysize=(int)ysizep;
	
	
	
	uSizeWindow(myIcon,(int)(xsize+ScrollOffSet),(int)(ysize+ScrollOffSet+myIcon->mMenuOffSet));	
	
	InvalRectMyWindow(myIcon);

	tMoveControls(myIcon);
	
	uSetControlValue(myIcon->VControl,0L);
	uSetControlValue(myIcon->HControl,0L);
}
static void tMoveControls2(IconPtr myIcon)
{
	struct vScene *scene;
	uRect uviewRect;
	volumetricPtr r;
	
	if(!myIcon)return;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return;
	scene=r->scene;
	if(!scene)return;
	
	uviewRect=myIcon->uviewRect;
	
	tMoveControls(myIcon);
	
	if(uviewRect.xsize != myIcon->uviewRect.xsize || uviewRect.ysize != myIcon->uviewRect.ysize){	
		scene->xResolution=myIcon->uviewRect.xsize;
		scene->yResolution=myIcon->uviewRect.ysize;
		tMoveControls(myIcon);
    	fixScene(r);
        RenderVolume(myIcon,1);
	}
	
}
static void tMoveControls(IconPtr myIcon)
{
	
	controlPtr control;
	struct vScene *scene;
	long xsizep,ysizep;
	volumetricPtr t;
	uRect eraseRect;
	int MaxValue;
	uRect r;
	int bottom;
	int right;	
	int top;
	int left;
	
	if(!myIcon)return;
	t=(volumetricPtr)myIcon->DWindow;
	if(!t)return;
	scene=t->scene;
	if(!scene)return;
	
	uGetPortBounds(myIcon,&r);
	
	top=0;
	left=0;
	bottom=r.ysize;
	right=r.xsize;
	

	if(uGetPaletteSize(&xsizep,&ysizep,&scene->pd,myIcon))return;

	myIcon->uviewRect = r;
	myIcon->uviewRect.y = myIcon->mMenuOffSet;
	myIcon->uviewRect.x = 0;
	myIcon->uviewRect.ysize -= (myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace);
	myIcon->uviewRect.xsize -= ScrollOffSet;


	scene->pd.left=myIcon->uviewRect.xsize-xsizep;
	scene->pd.top=myIcon->uviewRect.ysize/2-128-10;


	ScreenSetSize(myIcon->uviewRect.xsize,myIcon->uviewRect.ysize,myIcon);
	
	eraseRect.y=bottom-ScrollOffSet;
	eraseRect.x=left;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;
	
	control=myIcon->VControl;
	if(control){
		MoveControl(control,right-ScrollOffSet,top+myIcon->mMenuOffSet);
		SizeControl(control,ScrollOffSet,bottom-top-ScrollOffSet-myIcon->mMenuOffSet);
		MaxValue=(int)(scene->yResolution-(r.ysize-ScrollOffSet-myIcon->mMenuOffSet-myIcon->CursorSpace));
		if(MaxValue < 0)MaxValue = 0;
		uSetControlMaximum(control,MaxValue);
	}
		
	control=myIcon->HControl;	
	if(control){
		MoveControl(control,eraseRect.x+eraseRect.xsize,bottom-ScrollOffSet);
		SizeControl(control,right-(eraseRect.xsize+ScrollOffSet),ScrollOffSet);
		MaxValue=(int)(scene->xResolution-(right-ScrollOffSet));
		if(MaxValue < 0)MaxValue = 0;
		uSetControlMaximum(control,MaxValue);
	}
	
	InvalRectMyWindow(myIcon);
}
static int tdothumb(controlPtr control,IconPtr myIcon)
{
	control=control;
	return tMoveDy(myIcon);
}
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;
		
		if(!myIcon || !control)return 0;
		
		part=part;
		dy=0;
		if(control == myIcon->VControl){
		    dy= myIcon->uviewRect.ysize;
		} else if(control == myIcon->HControl){
		    dy=myIcon->uviewRect.xsize;
		}
		newp=(int)uGetControlValue(control);
		uSetControlValue(control,newp-dy);
		if(uGetControlValue(control) == newp)return 0;
		return tMoveDy(myIcon);

}
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;

		if(!myIcon || !control)return 0;
		
		part=part;
		dy=0;
		if(control == myIcon->VControl){
		    dy= myIcon->uviewRect.ysize;
		} else if(control == myIcon->HControl){
		    dy=myIcon->uviewRect.xsize;
		}
		newp=(int)uGetControlValue(control);
		uSetControlValue(control,newp+dy);
		if(uGetControlValue(control) == newp)return 0;
		return tMoveDy(myIcon);
}
static int tgoUp(controlPtr control,short part,IconPtr myIcon)
{	
		int old;
	
		if(!myIcon || !control)return 0;
		part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old-8);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tgoDown(controlPtr control,short part,IconPtr myIcon)
{			
		int old;
		
		if(!myIcon || !control)return 0;
		part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old+8);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tMoveDy(IconPtr myIcon)
{	
	drawIt(myIcon);
	return 0;
}

static int doOrbit(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
      /*  0 */      {"0", {163,25,80,20},uDialogText,uDialogGetLong},
      /*  1 */      {"0", {73,94,80,20},uDialogText,uDialogGetLong},
      /*  2 */      {"0", {73,119,80,20},uDialogText,uDialogGetLong},
      /*  3 */      {"0", {73,144,80,20},uDialogText,uDialogGetLong},
      /*  4 */      {"0", {195,95,110,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"0", {195,120,110,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"0", {195,145,110,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"0", {427,95,110,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"0", {427,120,110,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"0", {427,145,110,20},uDialogText,uDialogGetLong},
      /* 10 */      {"0", {397,8,80,20},uDialogText,uDialogNone},
      /* 11 */      {"0", {397,33,80,20},uDialogText,uDialogGetLong},
      /* 12 */      {"Save Images", {329,66,125,20},uDialogCheck,uDialogGetLong},
      /* 13 */      {"sequence", {317,34,65,20},uDialogString,uDialogNone},
      /* 14 */      {"in", {5,94,24,18},uDialogString,uDialogNone},
      /* 15 */      {"around", {5,119,52,19},uDialogString,uDialogNone},
      /* 16 */      {"root name", {317,9,77,20},uDialogString,uDialogNone},
      /* 17 */      {"Ok", {315,184,80,20},uDialogButton,uDialogNone},
      /* 18 */      {"Cancel", {401,184,80,20},uDialogButton,uDialogNone},
      /* 19 */      {"Orbits", {59,25,65,20},uDialogString,uDialogNone},
      /* 20 */      {"out", {5,144,41,18},uDialogString,uDialogNone},
      /* 21 */      {"x", {173,95,15,20},uDialogString,uDialogNone},
      /* 22 */      {"y", {173,120,13,17},uDialogString,uDialogNone},
      /* 23 */      {"z", {173,145,19,20},uDialogString,uDialogNone},
      /* 24 */      {"Target Radius", {317,95,100,17},uDialogString,uDialogNone},
      /* 25 */      {"Target Degrees", {317,120,106,20},uDialogString,uDialogNone},
      /* 26 */      {"Target Loops", {317,145,100,16},uDialogString,uDialogNone},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,555,220};
 		
	static char saveFileName[128]={'o','r','b','i','t','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	static long orbits=1;
	static double targetradius=0;
	static double targetdegrees=0;
	static long targetloops=1;
	static long in=20;
	static long around=10;
	static long out=20;
	static struct P offset={0.,0.,0.};
	struct P EyeSave,LampSave;
	struct P EyeToTarget,TargetLoop,V1,V2;
	struct P OutToEye,EyeToIn;
	double DegreesSave;
	long o,l;
	double pi;
	double r1,r2,a1,a2;
	double dsin,dsout,epsin,epsout;
	double stime,etime;
	long ntime;
	
	
	
	
	struct vScene *scene;
	volumetricPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	int saveWhere;
    int reti;
    int ret;
    long n;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	EyeSave=scene->Eye;

	LampSave=scene->Lamp;

	DegreesSave=scene->Degrees;

	if(targetradius <= 0.){
	    struct P diff;
	    diff=Sub(&scene->Target,&scene->Eye);
	    targetradius=.3*Len(&diff);
	}

	if(targetdegrees <= 0.){
	   targetdegrees=scene->Degrees*3.0;
	   if(targetdegrees > 170.)targetdegrees=170.;
	}

	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	uList=uDialogOpen("Orbit Target",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
			
	reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)orbits,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)in,
	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,(double)around,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)out,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)offset.x,
	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)offset.y,
	              	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)offset.z,
	              	              	              	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)targetradius,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)targetdegrees,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)targetloops,
	              	              
	              uDialogSetItem,10L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)scene->sequence,
	                            	          	              
	              uDialogSetItem,12L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
              
	              uDialogSetMyIcon,myIcon,	    
	              
	              /*          
	              uDialogSetAction,Time1Action,	              
	              */
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	

	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
	orbits=uR[0].lreturn;
	
	in=uR[1].lreturn;
	    	
	around=uR[2].lreturn;
	    	
	out=uR[3].lreturn;
	    	
	offset.x=uR[4].dreturn;
	
	offset.y=uR[5].dreturn;
	
	offset.z=uR[6].dreturn;
	
	targetradius=uR[7].dreturn;
	
	targetdegrees=uR[8].dreturn;
	
	targetloops=uR[9].lreturn;
	
	mstrncpy(scene->sequenceName,uR[10].sreturn,255);
	
	scene->sequence=uR[11].lreturn;
	
	scene->sequenceSave=uR[12].lreturn;
	
    if(uList){
        uDialogClose(uList);
        uList=NULL;
    }
    
    if(uR){
        uDialogFreeReturn(uR);
		uR=NULL;
	}
	
		
	stime=rtime();

	ntime=scene->sequence;

	pi=4.0*atan(1.0);

	if(targetradius < 0.)targetradius=0.;

	TargetLoop=Add(&offset,&scene->Target);

	EyeToTarget=Sub(&TargetLoop,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);

	V1=CrossN(&EyeToTarget,&scene->Up);

	V2=EyeToTarget;

	V2=CrossN(&scene->Up,&V1);

	{
	    struct P VinTarget;
	    VinTarget=Mult(&V1,targetradius);
	    VinTarget=Add(&VinTarget,&TargetLoop);
	    EyeToIn=Sub(&VinTarget,&scene->Eye);
	}
	{
	    struct P VoutTarget;
	    VoutTarget=Mult(&V1,-targetradius);
	    VoutTarget=Add(&VoutTarget,&TargetLoop);
	    OutToEye=Sub(&scene->Eye,&VoutTarget);
	}

	{
	    struct P Range;
	    Range=Sub(&scene->Target,&scene->Eye);
	    r1=Len(&Range);  
	    a1=r1*tan(.5*DegreesSave*pi/180.);
	    Range=Mult(&V1,targetradius);
	    Range=Add(&Range,&TargetLoop);
	    Range=Sub(&scene->Target,&Range);
	    r2=Len(&Range);
	    a2=r2*tan(.5*targetdegrees*pi/180.);
	}
	{
		double length;

		if(around > 0 && in > 3){
		    dsin=(targetradius*pi/((double)around))/(r1-r2);
		    length=dsin*(double)in;
		    if(length > 1.0){
			dsin=1.0/((double)in);
		        epsin=0.;
		    }else{
		        epsin=(1.0 - (double)length)/(double)SUM(in-1);
		    }
		}else{
		    dsin=0.0;
		    if(in > 0)dsin=1.0/((double)in);
		    epsin=0.;
		}
	}
	{
		double length;

		if(around > 0 && out > 3){
		    dsout=(targetradius*pi/((double)around))/(r1-r2);
		    length=dsout*(double)out;
		    if(length > 1.0){
			dsout=1.0/((double)out);
			epsout=0.;
		    }else {
		        epsout=(1.0 - (double)length)/(double)SUM(out-1);
		    }
		}else{
		    dsout=0.0;
		    if(out > 0)dsout=1.0/((double)out);
		    epsout=0.;
		}
	}

	if(orbits < 0)orbits = 1;

	if(scene->sequenceSave > 0){
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Contours");
	if(StatusWindow == NULL){
	    sprintf(WarningBuff,"StatusWindow NULL\n");
		WarningBatch(WarningBuff);
	}
	if(saveWhere == 3){
	    scene->FileOut=FileOut;
	}else if(saveWhere == 2 || saveWhere == 4){
	    scene->Dump=Dump;
	    scene->DumpData=(void *)myIcon;
	    r->saveWhere=saveWhere;
	    r->saveDirectory=Directory;
	    r->saveName=fileName;
	    r->saveFileName=saveFileName;
	}

	for(o=0;o<orbits;++o){
	    struct P Move,Move2;
	    double dm,s,rp,sp;

	    dm=0.0;
	    if(in)dm = 1.0/((double)(in));
	    for(n=0;n<in;++n){
	        s=((double)(in-n)*dsin);
	        if(epsin > 0.0){
	            if((in-n-1) >= 1)s += epsin*(double)SUM((in-n-1));
	        }
	        s = 1.0 - s;
	        Move=Mult(&EyeToIn,s);
	        scene->Eye=Add(&EyeSave,&Move);
			scene->Lamp=scene->Eye;
			rp=(1.-s)*r1+s*r2;
			sp=(1.-s)*a1+s*a2;
	        scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	        sprintf(buff,"Creating: %s",fileName);
	        mstrncpy(FileOut,Directory,1280);
	        strncatToPath(FileOut,fileName,1280);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop Oribits"))goto ErrorOut;
			RenderVolume(myIcon,1);
	        ++scene->sequence;	    
	    }
	    scene->Degrees=targetdegrees;
	    for(l=0;l<targetloops;++l){
	        dm=0.0;
	        if(around)dm = 1.0/((double)(around));
	        for(n=0;n<around;++n){
	            Move=Mult(&V1,targetradius*cos((((double)n*dm+(double)l)*pi)));
		    	Move2=Mult(&V2,targetradius*sin((((double)n*dm+(double)l)*pi)));
	            Move=Add(&Move,&Move2);
	            scene->Eye=Add(&TargetLoop,&Move);
		    	scene->Lamp=scene->Eye;
		    	sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	            sprintf(buff,"Creating: %s",fileName);
	            mstrncpy(FileOut,Directory,1280);
	            strncatToPath(FileOut,fileName,1280);
	            uWriteStatusWindow(StatusWindow,buff);
	            if(uCheckStatusWindow(StatusWindow,"Stop Orbits ?"))goto ErrorOut;
				RenderVolume(myIcon,1);
	            ++scene->sequence;	  
	        }
	    }
	    dm=0.0;
	    if(out)dm = 1.0/((double)(out));
	    for(n=0;n<out;++n){
	        s=(double)n*dsout;
	        if(epsout > 0.0){
	            if((n-1) >= 1)s += epsout*(double)SUM((n-1));
	        }
	        s = 1.0 - s;
	        Move=Mult(&OutToEye,-s);
	        scene->Eye=Add(&EyeSave,&Move);
		    scene->Lamp=scene->Eye;
			rp=(1.-s)*r1+s*r2;
			sp=(1.-s)*a1+s*a2;
	        scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	        sprintf(buff,"Creating: %s",fileName);
	        mstrncpy(FileOut,Directory,1280);
	        strncatToPath(FileOut,fileName,1280);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop Orbits ?"))goto ErrorOut;
			RenderVolume(myIcon,1);
	        ++scene->sequence;	    
	    }
	}

	etime=rtime()-stime;

	ntime=scene->sequence-ntime;

	if(scene->debug > 0){
		sprintf(WarningBuff,"%.2f Seconds  %ld Frames  %.2f Frames/sec\n",
	    	etime,ntime,((double)ntime)/etime);
		WarningBatch(WarningBuff);
	}


	ret = 0;	
	
ErrorOut:

	uStopStatusWindow(StatusWindow);
	
	scene->Eye=EyeSave;

	scene->Lamp=LampSave;

	scene->Degrees=DegreesSave;
	
	
	if(r->saveWhere == 2){
	    if(r->saveFiles){
	       FilesOpenDefault(r->saveFiles);
	    }
	    r->saveFiles=NULL;
	} else if(r->saveWhere == 4){
	    if(r->saveFILE){
	       TrueMergedDone(r->saveFILE);
	       r->saveFILE=NULL;
	    }
	}
	scene->FileOut=NULL;
	scene->Dump=NULL;
	scene->DumpData=NULL;
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	RenderVolume(myIcon,1);
	
	return ret;
}
static int doMove(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
      /*  0 */      {"0", {163,25,80,20},uDialogText,uDialogGetLong},
      /*  1 */      {"0", {73,94,80,20},uDialogText,uDialogGetDouble},
      /*  2 */      {"0", {73,119,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"0", {73,144,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"0", {196,95,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"0", {196,120,80,20},uDialogText,uDialogGetDouble},
      /*  6 */      {"0", {196,145,80,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"0", {397,8,80,20},uDialogText,uDialogNone},
      /*  8 */      {"0", {397,33,80,20},uDialogText,uDialogGetLong},
      /*  9 */      {"0", {397,124,80,20},uDialogText,uDialogGetDouble},
      /* 10 */      {"Save Images", {329,66,125,20},uDialogCheck,uDialogGetLong},
      /* 11 */      {"Reset Target", {328,93,106,18},uDialogCheck,uDialogGetLong},
      /* 12 */      {"Steps", {59,25,65,20},uDialogString,uDialogNone},
      /* 13 */      {"x", {173,95,15,20},uDialogString,uDialogNone},
      /* 14 */      {"y", {173,120,13,17},uDialogString,uDialogNone},
      /* 15 */      {"z", {173,145,19,20},uDialogString,uDialogNone},
      /* 16 */      {"Find Degrees", {297,124,92,17},uDialogString,uDialogNone},
      /* 17 */      {"x", {49,92,15,20},uDialogString,uDialogNone},
      /* 18 */      {"y", {49,117,13,17},uDialogString,uDialogNone},
      /* 19 */      {"z", {49,142,19,20},uDialogString,uDialogNone},
      /* 20 */      {"Target", {87,70,55,16},uDialogString,uDialogNone},
      /* 21 */      {"Find Point", {201,72,75,16},uDialogString,uDialogNone},
      /* 22 */      {"sequence", {317,34,65,20},uDialogString,uDialogNone},
      /* 23 */      {"root name", {317,9,77,20},uDialogString,uDialogNone},
      /* 24 */      {"Ok", {315,163,80,20},uDialogButton,uDialogNone},
      /* 25 */      {"Cancel", {401,163,80,20},uDialogButton,uDialogNone},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,498,202};
 	
	
	static char saveFileName[128]={'m','o','v','e','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	struct vScene *scene;
	volumetricPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	static long orbits=10;
	static double targetradius=0;
	static double targetdegrees=0;
	static long resettarget=1;
	struct P EyeToTarget,EyeToFind;
	struct P TargetSave,TargetToFind;
	double DegreesSave;
	long o;
	int saveWhere;
	double pi;
	double r1,r2,a1,a2;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	if(targetradius <= 0.){
	    struct P diff;
	    diff=Sub(&scene->Target,&scene->Eye);
	    targetradius=.3*Len(&diff);
	}

	if(targetdegrees <= 0.){
	   targetdegrees=scene->Degrees;
	   if(targetdegrees > 170.)targetdegrees=170.;
	}
	
	TargetSave=scene->Target;

	DegreesSave=scene->Degrees;	
	
	
	uList=uDialogOpen("Move Target",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
			
		reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)orbits,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)scene->Target.x,
	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,(double)scene->Target.y,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)scene->Target.z,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)scene->FindPoint.x,
	              	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)scene->FindPoint.y,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)scene->FindPoint.z,
	              
	              uDialogSetItem,7L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)targetdegrees,
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              uDialogSetItem,11L,
	              uDialogSetLogical,(resettarget == 1),
	              
	              uDialogSetMyIcon,myIcon,	              	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
		
	orbits=uR[0].lreturn;
	    	
	scene->Target.x=uR[1].dreturn;
	    		
	scene->Target.y=uR[2].dreturn;
	    		
	scene->Target.z=uR[3].dreturn;
	    		
	scene->FindPoint.x=uR[4].dreturn;
	
	scene->FindPoint.y=uR[5].dreturn;
	
	scene->FindPoint.z=uR[6].dreturn;
	
	mstrncpy(scene->sequenceName,uR[7].sreturn,255);
	
	scene->sequence=uR[8].lreturn;
	
	targetdegrees=uR[9].dreturn;
	
	scene->sequenceSave=uR[10].lreturn;
		
	resettarget=uR[11].lreturn;
	
    if(uList){
        uDialogClose(uList);
        uList=NULL;
    }
    
    if(uR){
        uDialogFreeReturn(uR);
		uR=NULL;
	}
	
	pi=4.0*atan(1.0);


	EyeToTarget=Sub(&scene->Target,&scene->Eye);

	EyeToFind=Sub(&scene->FindPoint,&scene->Eye);

	TargetToFind=Sub(&scene->FindPoint,&scene->Target);


	{
	    r1=Len(&EyeToTarget);  
	    a1=r1*tan(.5*DegreesSave*pi/180.);
	    r2=Len(&EyeToFind);
	    a2=r2*tan(.5*targetdegrees*pi/180.);
	}

	if(orbits < 0)orbits = 1;

	if(scene->sequenceSave > 0){
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Contours");
	if(StatusWindow == NULL){
	    sprintf(WarningBuff,"StatusWindow NULL\n");
		WarningBatch(WarningBuff);
	}

	if(saveWhere == 3){
	    scene->FileOut=FileOut;
	}else if(saveWhere == 2 || saveWhere == 4){
	    scene->Dump=Dump;
	    scene->DumpData=(void *)myIcon;
	    r->saveWhere=saveWhere;
	    r->saveDirectory=Directory;
	    r->saveName=fileName;
	    r->saveFileName=saveFileName;
	}

	for(o=0;o<orbits;++o){
	    struct P Move;
	    double dm,s,rp,sp;

	    dm=0.0;
	    if(orbits > 1)dm = 1.0/((double)(orbits-1));

	    s=((double)(o)*dm);
	    Move=Mult(&TargetToFind,s);
	    scene->Target=Add(&TargetSave,&Move);
	    rp=(1.-s)*r1+s*r2;
	    sp=(1.-s)*a1+s*a2;
	    scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	    sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	    sprintf(buff,"Creating: %s",fileName);
	    mstrncpy(FileOut,Directory,1280);
	    strncatToPath(FileOut,fileName,1280);
	    uWriteStatusWindow(StatusWindow,buff);
	    if(uCheckStatusWindow(StatusWindow,"Stop Move"))goto ErrorOut;
	    RenderVolume(myIcon,1);
	    ++scene->sequence;	    
	}

	ret = 0;	
	
ErrorOut:

	if(resettarget)scene->Target=TargetSave;
	scene->Degrees=DegreesSave;

	uStopStatusWindow(StatusWindow);
	if(r->saveWhere == 2){
	    if(r->saveFiles){
	       FilesOpenDefault(r->saveFiles);
	    }
	    r->saveFiles=NULL;
	} else if(r->saveWhere == 4){
	    if(r->saveFILE){
	       TrueMergedDone(r->saveFILE);
	       r->saveFILE=NULL;
	    }
	}
	scene->FileOut=NULL;
	scene->Dump=NULL;
	scene->DumpData=NULL;
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(myIcon)RenderVolume(myIcon,1);
	
	return ret;
}
static int doViewSend(IconPtr myIcon,char *name)
{
	struct CommandInfo Commands;
	struct vScene *scene;
	volumetricPtr r;
	long nn,n,ns;
	double dmin,dmax;
	
	if(!myIcon || !name)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	zerol((char *)&Commands,sizeof(Commands));
	

	Commands.command[0]="debug";
	Commands.type[0]=BATCH_STRING;
	
 	Commands.value[1]=scene->debug;
	Commands.type[1]=BATCH_DOUBLE;
	
	
	Commands.command[2]="eye";
	Commands.type[2]=BATCH_STRING;
	
	Commands.value[3]=scene->Eye.x;
	Commands.type[3]=BATCH_DOUBLE;
	
	Commands.value[4]=scene->Eye.y;
	Commands.type[4]=BATCH_DOUBLE;
	
	Commands.value[5]=scene->Eye.z;
	Commands.type[5]=BATCH_DOUBLE;    	

	
	Commands.command[6]="target";
	Commands.type[6]=BATCH_STRING;

 	Commands.value[7]=scene->Target.x;
	Commands.type[7]=BATCH_DOUBLE;
	
	Commands.value[8]=scene->Target.y;
	Commands.type[8]=BATCH_DOUBLE;
	
	Commands.value[9]=scene->Target.z;
	Commands.type[9]=BATCH_DOUBLE;

	
	Commands.command[10]="lamp";
	Commands.type[10]=BATCH_STRING;

 	Commands.value[11]=scene->Lamp.x;
	Commands.type[11]=BATCH_DOUBLE;
	
	Commands.value[12]=scene->Lamp.y;
	Commands.type[12]=BATCH_DOUBLE;
	
	Commands.value[13]=scene->Lamp.z;
	Commands.type[13]=BATCH_DOUBLE;

	
	Commands.command[14]="up";
	Commands.type[14]=BATCH_STRING;

 	Commands.value[15]=scene->Up.x;
	Commands.type[15]=BATCH_DOUBLE;
	
	Commands.value[16]=scene->Up.y;
	Commands.type[16]=BATCH_DOUBLE;
	
	Commands.value[17]=scene->Up.z;
	Commands.type[17]=BATCH_DOUBLE;
	
	Commands.command[18]="degrees";
	Commands.type[18]=BATCH_STRING;

 	Commands.value[19]=scene->Degrees;
	Commands.type[19]=BATCH_DOUBLE;
	
	Commands.command[20]="xsize";
	Commands.type[20]=BATCH_STRING;

 	Commands.value[21]=(double)scene->xResolution;
	Commands.type[21]=BATCH_DOUBLE;


	Commands.command[22]="ysize";
	Commands.type[22]=BATCH_STRING;

 	Commands.value[23]=(double)scene->yResolution;
	Commands.type[23]=BATCH_DOUBLE;


	Commands.command[24]="background";
	Commands.type[24]=BATCH_STRING;

 	Commands.value[25]=scene->BackGround.red;
	Commands.type[25]=BATCH_DOUBLE;
	
	Commands.value[26]=scene->BackGround.green;
	Commands.type[26]=BATCH_DOUBLE;
	
	Commands.value[27]=scene->BackGround.blue;
	Commands.type[27]=BATCH_DOUBLE;


	Commands.command[28]="limits";
	Commands.type[28]=BATCH_STRING;

 	Commands.value[29]=r->plane.Plane_xmin;
	Commands.type[29]=BATCH_DOUBLE;

 	Commands.value[30]=r->plane.Plane_xmax;
	Commands.type[30]=BATCH_DOUBLE;

 	Commands.value[31]=r->plane.Plane_ymin;
	Commands.type[31]=BATCH_DOUBLE;

 	Commands.value[32]=r->plane.Plane_ymax;
	Commands.type[32]=BATCH_DOUBLE;

 	Commands.value[33]=r->plane.Plane_zmin;
	Commands.type[33]=BATCH_DOUBLE;

 	Commands.value[34]=r->plane.Plane_zmax;
	Commands.type[34]=BATCH_DOUBLE;

	Commands.command[35]="Height";
	Commands.type[35]=BATCH_STRING;
	
 	Commands.value[36]=scene->Height;
	Commands.type[36]=BATCH_DOUBLE;
	
	
	Commands.command[37]="logscale";
	Commands.type[37]=BATCH_STRING;
	
 	Commands.value[38]=(r->pd.sType == 1);
	Commands.type[38]=BATCH_DOUBLE;
		
	
	Commands.command[39]="eye";
	Commands.type[39]=BATCH_STRING;
	
	Commands.command[40]="seperation";
	Commands.type[40]=BATCH_STRING;
	
 	Commands.value[41]=scene->EyeSeparation;
	Commands.type[41]=BATCH_DOUBLE;

	Commands.command[42]="eye";
	Commands.type[42]=BATCH_STRING;
	
	if(scene->EyeView == Scene_EyeLeft){
	   Commands.command[43]="left";
	}else if(scene->EyeView == Scene_EyeRight){
	   Commands.command[43]="right";
	}else if(scene->EyeView == Scene_Anaglyph){
	   Commands.command[43]="anaglyph";
	}else{
	   Commands.command[43]="normal";
	}
	Commands.type[43]=BATCH_STRING;
		
	Commands.command[44]="reflect";
	Commands.type[44]=BATCH_STRING;
	
 	Commands.value[45]=scene->xReflect;
	Commands.type[45]=BATCH_DOUBLE;
	
 	Commands.value[46]=scene->yReflect;
	Commands.type[46]=BATCH_DOUBLE;
	
 	Commands.value[47]=scene->zReflect;
	Commands.type[47]=BATCH_DOUBLE;
	
	Commands.command[48]="threads";
	Commands.type[48]=BATCH_STRING;
	
 	Commands.value[49]=(double)scene->threads;
	Commands.type[49]=BATCH_DOUBLE;
	
	nn=50;
	
	dmin=r->pd.sPmin;
	dmax=r->pd.sPmax;

	if(r->pd.sType == 0 || scene->AllOpaque){
		r->pd.sType = 0;
	    dmin=dmax=0;
	}
	
	Commands.command[nn]="dmin";
	Commands.type[nn++]=BATCH_STRING;
	
 	Commands.value[nn]=dmin;
	Commands.type[nn++]=BATCH_DOUBLE;
	
	Commands.command[nn]="dmax";
	Commands.type[nn++]=BATCH_STRING;
	
 	Commands.value[nn]=dmax;
	Commands.type[nn++]=BATCH_DOUBLE;		

	if(scene->AllOpaque){
			Commands.command[nn]="opacity";
			Commands.type[nn++]=BATCH_STRING;

			Commands.value[nn]=(double)2.0;
			Commands.type[nn++]=BATCH_DOUBLE;

			Commands.value[nn]=(double)0.0;
			Commands.type[nn++]=BATCH_DOUBLE;
			Commands.value[nn]=(double)1.0;
			Commands.type[nn++]=BATCH_DOUBLE;
			
			Commands.value[nn]=(double)1.0;
			Commands.type[nn++]=BATCH_DOUBLE;
			Commands.value[nn]=(double)1.0;
			Commands.type[nn++]=BATCH_DOUBLE;
						
	}else{
		ns=nn;
		if(scene->tcount > 0){
			Commands.command[nn]="opacity";
			Commands.type[nn++]=BATCH_STRING;

			Commands.value[nn]=(double)scene->tcount;
			Commands.type[nn++]=BATCH_DOUBLE;
			
			for(n=0;n<scene->tcount;++n){
		        if(nn > 252){
		           nn = ns;
		           break;
		        }
				Commands.value[nn]=scene->tvalue[n];
				Commands.type[nn++]=BATCH_DOUBLE;
				Commands.value[nn]=scene->topacity[n];
				Commands.type[nn++]=BATCH_DOUBLE;
			}

		}
	}

	Commands.command[nn]="palette";
	Commands.type[nn++]=BATCH_STRING;
	
	Commands.command[nn]=scene->palname;
	Commands.type[nn++]=BATCH_STRING;		
	
	
	Commands.nword=(int)nn;

	SendMessageByName(name,MessageType_SEND_COMMAND_DATA,&Commands);
    	
	return 0;
}
int CenterScreenVol(IconPtr myIcon,int flag)
{

	struct P EyeToTarget,pScreen;
	double rc;
	double at=0.26794919;

    struct PlotRangeData *range;
	struct vScene *scene;
	volumetricPtr s;
	
	if(!myIcon)return 1;
	s=(volumetricPtr)myIcon->DWindow;
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	doViewSend(myIcon,s->BatchName);

	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	range=&s->sd.range;
	
	flag=flag;
	
	if(range->xminData > 1e59){
	    goto ErrorOut;
	}
	
	
	scene->Target.x=.5*(range->xminData+range->xmaxData);
	scene->Target.y=.5*(range->yminData+range->ymaxData);
	scene->Target.z=.5*(range->zminData+range->zmaxData);
	rc=sqrt((scene->Target.x-range->xmaxData)*(scene->Target.x-range->xmaxData)+
	        (scene->Target.y-range->ymaxData)*(scene->Target.y-range->ymaxData)+
	        (scene->Target.z-range->zmaxData)*(scene->Target.z-range->zmaxData));
	        
	scene->Eye.x=scene->Target.x+1.001;
	scene->Eye.y=scene->Target.y+1.002;
	scene->Eye.z=scene->Target.z+1.003;

	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);

	pScreen=Mult(&EyeToTarget,-(rc/at));
	scene->Eye=Add(&pScreen,&scene->Target);

	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	
	scene->Height=Len(&EyeToTarget);
	
	scene->EyeSeparation=0.02*scene->Height;

	scene->Degrees=30;

	scene->Lamp=scene->Eye;
	
	scene->EyeView=Scene_Eye;

ErrorOut:
	return 0;
}
static int doView1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct vScene *scene;
	volumetricPtr r;
	uDialogReturn *uR;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 19 || uList->actionItem == 21 || uList->actionItem == 30){
		uR=uDialogGetReturn(uList);

    	if(!uR)goto ErrorOut;
    	
		scene->Eye.x=uR[5].dreturn;
		scene->Eye.y=uR[6].dreturn;
		scene->Eye.z=uR[7].dreturn;
		
    	scene->Target.x=uR[8].dreturn;
    	scene->Target.y=uR[9].dreturn;
    	scene->Target.z=uR[10].dreturn;
    	
	    scene->Lamp.x=uR[11].dreturn;          
	    scene->Lamp.y=uR[12].dreturn;          
	    scene->Lamp.z=uR[13].dreturn;          
	              
		scene->Up.x=uR[14].dreturn; 
		scene->Up.y=uR[15].dreturn; 
		scene->Up.z=uR[16].dreturn; 
		
		scene->Degrees=uR[17].dreturn;
		
		scene->AutoCenterScreen=uR[18].lreturn;
		
		scene->Height=uR[24].dreturn;
	
		scene->EyeSeparation=uR[25].dreturn;

		scene->EyeView=Scene_Eye;
		if(uR[27].lreturn)scene->EyeView=Scene_EyeLeft;
		if(uR[28].lreturn)scene->EyeView=Scene_EyeRight;	
		if(uR[29].lreturn)scene->EyeView=Scene_Anaglyph;	
		
		scene->AllOpaque=(int)uR[31].lreturn;
				
		if(uList->actionItem == 19){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
              uDialogSetDone,uDialogSetDone
    		);
    
			if(reti)goto ErrorOut;
	
		}else if(uList->actionItem == 30){               
	       
	       CenterScreenVol(myIcon,0);

			reti=uDialogSet(uList,5L,
				          uDialogSetDouble,scene->Eye.x,
			              
			              uDialogSetItem,6L,
			              uDialogSetDouble,scene->Eye.y,
			              
			              uDialogSetItem,7L,
			              uDialogSetDouble,scene->Eye.z,
			              
			              uDialogSetItem,8L,
			              uDialogSetDouble,scene->Target.x,
			              
			              uDialogSetItem,9L,
			              uDialogSetDouble,scene->Target.y,
			              
			              uDialogSetItem,10L,
			              uDialogSetDouble,scene->Target.z,
			              
			              uDialogSetItem,11L,
			              uDialogSetDouble,scene->Eye.x,
			              
			              uDialogSetItem,12L,
			              uDialogSetDouble,scene->Eye.y,
			              
			              uDialogSetItem,13L,
			              uDialogSetDouble,scene->Eye.z,

			              uDialogSetItem,17L,
			              uDialogSetDouble,scene->Degrees,
			              
						  uDialogSetItem,24L,
						  uDialogSetDouble,scene->Height,

						  uDialogSetItem,25L,
						  uDialogSetDouble,scene->EyeSeparation,
	              		              
				          uDialogSetItem,26L,
			              uDialogSetLogical,(scene->EyeView == Scene_Eye),
			              
			              uDialogSetItem,27L,
			              uDialogSetLogical,(scene->EyeView == Scene_EyeLeft),
			              
			              uDialogSetItem,28L,
			              uDialogSetLogical,(scene->EyeView == Scene_EyeRight),
			              
			              uDialogSetItem,29L,
			              uDialogSetLogical,(scene->EyeView == Scene_Anaglyph),
			              			              
			              uDialogSetDone,uDialogSetDone
			    );
			if(reti)goto ErrorOut;
						
	     
	    	if(myIcon)RenderVolume(myIcon,1);
		}else{
	                  	
	       if(myIcon)RenderVolume(myIcon,1);
	    
	    }
	}
	ret = 0;
ErrorOut:	
	/* if(uList)uDialogUpdate(uList); */
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doView(IconPtr myIcon)
{
		static uDialogGroup Items[] =
    {
       /*  0 */      {"Eye",     {20,20,65,20},uDialogString,uDialogNone},
       /*  1 */      {"Target",  {20,50,65,20},uDialogString,uDialogNone},
       /*  2 */      {"Lamp",    {20,80,65,20},uDialogString,uDialogNone},
       /*  3 */      {"Up",      {20,110,65,20},uDialogString,uDialogNone},
       /*  4 */      {"Degrees", {20,140,65,20},uDialogString,uDialogNone},
       
       /*  5 */      {"0", {110,20,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"0", {230,20,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"0", {350,20,110,20},uDialogText,uDialogGetDouble},
       
       /*  8 */      {"0", {110,50,110,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"0", {230,50,110,20},uDialogText,uDialogGetDouble},
       /* 10 */      {"0", {350,50,110,20},uDialogText,uDialogGetDouble},
       
       /* 11 */      {"0", {110,80,110,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"0", {230,80,110,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"0", {350,80,110,20},uDialogText,uDialogGetDouble},
       
       /* 14 */      {"0", {110,110,110,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"0", {230,110,110,20},uDialogText,uDialogGetDouble},
       /* 16 */      {"0", {350,110,110,20},uDialogText,uDialogGetDouble},
       
       /* 17 */      {"0", {110,140,110,20},uDialogText,uDialogGetDouble},
       
       /* 18 */      {"Center Screen", {20,230,120,20},uDialogCheck,uDialogGetLong},
       
       /* 19 */      {"Ok",     {190,280,70,20},uDialogButton,uDialogNone},
       /* 20 */      {"Cancel", {280,280,70,20},uDialogButton,uDialogNone},
       /* 21 */      {"Render", {20 ,280,70,20},uDialogButton,uDialogNone},
       /* 22 */      {"Height", {20,170,65,20},uDialogString,uDialogNone},
       /* 23 */      {"Separation", {20,200,85,20},uDialogString,uDialogNone},
       /* 24 */      {"0", {110,170,110,20},uDialogText,uDialogGetDouble},
       /* 25 */      {"0", {110,200,110,20},uDialogText,uDialogGetDouble},
       /* 26 */      {"Eye",        {240,140,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 27 */      {"Left Eye",   {240,170,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 28 */      {"Right Eye",  {240,200,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 29 */      {"Anaglyph",   {240,230,100,18},uDialogButtonRadio,uDialogGetLong},
       /* 30 */      {"Center",     {100 ,280,70,20},uDialogButton,uDialogNone},
       /* 31 */      {"All Opaque", {20,250,120,20},uDialogCheck,uDialogGetLong},
       
    };


    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,490,340};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	struct P Eye,Target,Lamp,Up;
	double Degrees;
	int reti;
    int ret;


	struct vScene *scene;
	volumetricPtr r;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	ret=1;
	
	
	Eye=scene->Eye;
	
	Target=scene->Target;
	
    Lamp=scene->Lamp;          
              
	Up=scene->Up; 
	
	Degrees=scene->Degrees;	
		
	uR=NULL;
	
	uList=uDialogOpen("View",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	reti=uDialogSet(uList,5L,
		          uDialogSetDouble,scene->Eye.x,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,scene->Eye.y,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,scene->Eye.z,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,scene->Target.x,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,scene->Target.y,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,scene->Target.z,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,scene->Lamp.x,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,scene->Lamp.y,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,scene->Lamp.z,

	              uDialogSetItem,14L,
	              uDialogSetDouble,scene->Up.x,
	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,scene->Up.y,
	              
	              uDialogSetItem,16L,
	              uDialogSetDouble,scene->Up.z,
	              
	              uDialogSetItem,17L,
	              uDialogSetDouble,scene->Degrees,
	              
	              uDialogSetItem,18L,
	              uDialogSetLogical,(scene->AutoCenterScreen  == 1),
	              
	              uDialogSetItem,24L,
	              uDialogSetDouble,scene->Height,
	              
	              uDialogSetItem,25L,
	              uDialogSetDouble,scene->EyeSeparation,
	              
	              uDialogSetItem,26L,
	              uDialogSetLogical,(scene->EyeView == Scene_Eye),
	              
	              uDialogSetItem,27L,
	              uDialogSetLogical,(scene->EyeView == Scene_EyeLeft),
	              
	              uDialogSetItem,28L,
	              uDialogSetLogical,(scene->EyeView == Scene_EyeRight),
	              
	              uDialogSetItem,29L,
	              uDialogSetLogical,(scene->EyeView == Scene_Anaglyph),
	              	              
	              uDialogSetItem,31L,
	              uDialogSetLogical,(scene->AllOpaque  == 1),
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,doView1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
/*	
Start:
*/
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
/*    
    if((uR[27].lreturn || uR[28].lreturn) &&
        (uR[25].dreturn <= 0.0)){
        Warning("Eye Separation Must be greater than zero");
        goto Start;
    }
*/	
	scene->Eye.x=uR[5].dreturn;
	scene->Eye.y=uR[6].dreturn;
	scene->Eye.z=uR[7].dreturn;
	
	scene->Target.x=uR[8].dreturn;
	scene->Target.y=uR[9].dreturn;
	scene->Target.z=uR[10].dreturn;
	
    scene->Lamp.x=uR[11].dreturn;          
    scene->Lamp.y=uR[12].dreturn;          
    scene->Lamp.z=uR[13].dreturn;          
              
	scene->Up.x=uR[14].dreturn; 
	scene->Up.y=uR[15].dreturn; 
	scene->Up.z=uR[16].dreturn; 
	
	scene->Degrees=uR[17].dreturn;
	
	scene->AutoCenterScreen=uR[18].lreturn;

	scene->Height=uR[24].dreturn;
	
	scene->EyeSeparation=uR[25].dreturn;

	scene->EyeView=Scene_Eye;
	if(uR[27].lreturn)scene->EyeView=Scene_EyeLeft;
	if(uR[28].lreturn)scene->EyeView=Scene_EyeRight;	
	if(uR[29].lreturn)scene->EyeView=Scene_Anaglyph;	

	scene->AllOpaque=(int)uR[31].lreturn;
	
	ret=0;
ErrorOut:	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	if(ret){
		scene->Eye=Eye;
		
		scene->Target=Target;
		
	    scene->Lamp=Lamp;          
	              
		scene->Up=Up; 
		
		scene->Degrees=Degrees;
	}
	if(myIcon)RenderVolume(myIcon,1);
	return ret;
}
static int Rotate1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct vScene *scene;
	volumetricPtr r;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 6){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
	          uDialogSetItem,12L,
 	          uDialogSetFloatAux,(double)(uList->actionItem),
              uDialogSetDone,uDialogSetDone
    		);
    
			if(reti)goto ErrorOut;
	}
	
	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}

static int doSweep(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
       /*  0 */      {"0", {100,30,80,20},uDialogText,uDialogGetLong},
       /*  1 */      {"Sweep Down", {20,80,120,20},uDialogButtonRadio,uDialogGetLong},
       /*  2 */      {"0", {300,50,80,20},uDialogText,uDialogNone},
       /*  3 */      {"0", {300,80,80,20},uDialogText,uDialogGetLong},
       /*  4 */      {"Save Images", {220,20,100,20},uDialogCheck,uDialogGetLong},
       /*  5 */      {"", {40,20,0,0},uDialogString,uDialogNone},
       /*  6 */      {"Ok", {220,120,70,20},uDialogButton,uDialogNone},
       /*  7 */      {"Cancel", {310,120,70,20},uDialogButton,uDialogNone},
       /*  8 */      {"Sequence", {220,80,70,20},uDialogString,uDialogNone},
       /*  9 */      {"Bins", {20,30,70,20},uDialogString,uDialogNone},
       /* 10 */      {"Sweep", {20,60,120,20},uDialogButtonRadio,uDialogNone},
       /* 11 */      {"Root Name", {220,50,70,20},uDialogString,uDialogNone},
       /* 12 */      {"Exit", {69,70,70,20},uDialogFloatAux,uDialogFloatAux},
       /* 13 */      {"Sweep Up", {20,100,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 14 */      {"Print Bins", {20,120,100,20},uDialogCheck,uDialogGetLong},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,400,160};
 	
	static char saveFileName[128]={'s','w','e','e','p','.','t','r','u',};
	static long tiltsteps=10;
	static int sweep;
	static int printBins;
	
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	struct vScene *scene;
	volumetricPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	int saveWhere;
    int reti;
    int ret;
	int n;
	
	double dmin,dmax;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	
	uList=uDialogOpen("Sweep Opacity",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	
		reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)tiltsteps,
	              
	              uDialogSetItem,1L,
	              uDialogSetLogical,(sweep == 1),
	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              

	              uDialogSetItem,10L,
	              uDialogSetLogical,(sweep == 0),
	              
	              uDialogSetItem,13L,
	              uDialogSetLogical,(sweep == 2),
	              	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(printBins),
	              	              
	              uDialogSetMyIcon,myIcon,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uR[0].lreturn < 2){
	    Warning("Steps Must Be Greater Than 1");
	    goto Start;
	}
	
	
	tiltsteps=uR[0].lreturn;
	
	sweep=0;	
	if(uR[1].lreturn)sweep=1;
	if(uR[13].lreturn)sweep=2;
	    		
	mstrncpy(scene->sequenceName,uR[2].sreturn,255);
	
	scene->sequence=uR[3].lreturn;

	printBins=(int)uR[14].lreturn;
	
	scene->sequenceSave=uR[4].lreturn;
		
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
		
	dmin=r->sd.limits.vmin;
	dmax=r->sd.limits.vmax;
	
	if(dmin >= dmax){
	    sprintf(WarningBuff,"No Sweep Data - dmin %g dmax %g\n",dmin,dmax);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
        	
	if(scene->sequenceSave > 0){	
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Ojbect");
	if(StatusWindow == NULL){
	    sprintf(WarningBuff,"StatusWindow NULL\n");
		WarningBatch(WarningBuff);
	}

	if(saveWhere == 3){
	    scene->FileOut=FileOut;
	}else if(saveWhere == 2 || saveWhere == 4){
	    scene->Dump=Dump;
	    scene->DumpData=(void *)myIcon;
	    r->saveWhere=saveWhere;
	    r->saveDirectory=Directory;
	    r->saveName=fileName;
	    r->saveFileName=saveFileName;
	}
	{
		double tvalue[4],topacity[4];
	   	double dminsave,dmaxsave;
	   	double dx,min,max;
	   	int AllOpaque;
	   	long tcount;	   	
	   	long nn;
	   	   
	   dx=(dmax-dmin)/(double)tiltsteps;
	   if(dx <= 0)goto ErrorOut;
	   
	   	
   		dminsave=r->pd.sPmin;
		dmaxsave=r->pd.sPmax;
		AllOpaque=scene->AllOpaque;
		scene->AllOpaque=FALSE;
		tcount=scene->tcount;
		scene->tcount=4;

		r->pd.sPmin=dmin;
		r->pd.sPmax=dmax;

		for(nn=0;nn<4;++nn){
			tvalue[nn]=scene->tvalue[nn];
			topacity[nn]=scene->topacity[nn];
		}

		min=dmin;
		max=dmax;
		
	    for(n=0;n< tiltsteps; ++n){
			if(sweep == 1){
			   ;
			}else if(sweep == 2){
			   ;
			}else{
			    max=min+dx;
			}
			
			
			scene->tvalue[0]=min-.01*dx;
			scene->tvalue[1]=min;
			scene->tvalue[2]=max;
			scene->tvalue[3]=max+.01*dx;

			scene->topacity[0]=0;
			scene->topacity[1]=1;
			scene->topacity[2]=1;
			scene->topacity[3]=0;
			
						
			sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
			
			if(printBins){
			   sprintf(WarningBuff,"%s min %g max %g dx %g\n",fileName,min,max,dx);
			   WarningBatch(WarningBuff);
			}
			
			
			
			sprintf(buff,"Creating: %s",fileName);
			mstrncpy(FileOut,Directory,1280);
			strncatToPath(FileOut,fileName,1280);
			uWriteStatusWindow(StatusWindow,buff);
			if(uCheckStatusWindow(StatusWindow,"Stop Contours"))break;
			if(myIcon)RenderVolume(myIcon,1);
			++scene->sequence;	    
			
			if(sweep == 1){
        		max -= dx;
			}else if(sweep == 2){
			    min += dx;
			}else{
			    min += dx;
			}
		}
   		r->pd.sPmin=dminsave;
		r->pd.sPmax=dmaxsave;
		scene->AllOpaque=AllOpaque;
		scene->tcount=tcount;
	
		for(nn=0;nn<4;++nn){
			scene->tvalue[nn]=tvalue[nn];
			scene->topacity[nn]=topacity[nn];
		}
	}
	
	ret = 0;	
	
ErrorOut:

	
	uStopStatusWindow(StatusWindow);
	if(r->saveWhere == 2){
	    if(r->saveFiles){
	       FilesOpenDefault(r->saveFiles);
	    }
	    r->saveFiles=NULL;
	} else if(r->saveWhere == 4){
	    if(r->saveFILE){
	       TrueMergedDone(r->saveFILE);
	       r->saveFILE=NULL;
	    }
	}
	scene->FileOut=NULL;
	scene->Dump=NULL;
	scene->DumpData=NULL;
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(myIcon)RenderVolume(myIcon,1);
	
	return ret;
}
static int doRotate(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
       /*  0 */      {"0", {100,50,80,20},uDialogText,uDialogGetLong},
       /*  1 */      {"0", {100,80,80,20},uDialogText,uDialogGetLong},
       /*  2 */      {"0", {300,50,80,20},uDialogText,uDialogNone},
       /*  3 */      {"0", {300,80,80,20},uDialogText,uDialogGetLong},
       /*  4 */      {"Save Images", {220,20,100,20},uDialogCheck,uDialogGetLong},
       /*  5 */      {"Object View Angles", {40,20,150,20},uDialogString,uDialogNone},
       /*  6 */      {"Ok", {220,120,70,20},uDialogButton,uDialogNone},
       /*  7 */      {"Cancel", {310,120,70,20},uDialogButton,uDialogNone},
       /*  8 */      {"Sequence", {220,80,70,20},uDialogString,uDialogNone},
       /*  9 */      {"Tilt Steps", {20,50,70,20},uDialogString,uDialogNone},
       /* 10 */      {"Rotate Steps", {20,80,70,20},uDialogString,uDialogNone},
       /* 11 */      {"Root Name", {220,50,70,20},uDialogString,uDialogNone},
       /* 12 */      {"Exit", {69,70,70,20},uDialogFloatAux,uDialogFloatAux},
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,400,160};
 	
	static char saveFileName[128]={'r','o','t','a','t','e','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	char buff[256];
	IconPtr StatusWindow;
	
	struct vScene *scene;
	volumetricPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	static long tiltsteps=9;
	static long rotatesteps=36;
	double DegreesToRadians;
	struct P EyeSave,originSave;
	double vstep,vinc,vert,vertDegree;
	double hstep,hinc,hort,hortDegree;
	struct P EyeToLamp,EyeToTarget,Up,Target;
	double stime,etime;
	long ntime;
	struct P x,y,z;
	struct P xEye,yEye,zEye;
	double radius,rOut;
	struct P lamp;
	int saveWhere;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret=1;
	
	uR=NULL;
	
	r->saveWhere=0;
	
	StatusWindow=NULL;
	
	lamp=scene->Lamp;
	originSave=lamp;
	EyeSave=scene->Eye;
	
	
	uList=uDialogOpen("Rotate Objects",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	
		reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)tiltsteps,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)rotatesteps,
	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,Rotate1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uR[0].lreturn <= 2){
	    Warning("Tilt Steps Must Be Greater Than 2");
	    goto Start;
	}
	
	if(uR[1].lreturn <= 2){
	    Warning("Rotate Steps Must Be Greater Than 2");
	    goto Start;
	}
	
	tiltsteps=uR[0].lreturn;
	    	
	rotatesteps=uR[1].lreturn;
	    		
	mstrncpy(scene->sequenceName,uR[2].sreturn,255);
	
	scene->sequence=uR[3].lreturn;
	
	scene->sequenceSave=uR[4].lreturn;
		
    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	stime=rtime();

	ntime=scene->sequence;
	
		
  	DegreesToRadians=atan(1.0)/45.0;
	
        
	vstep = (int)tiltsteps;
	vinc= 180./(vstep-1.0);
	hstep= (int)rotatesteps;
	hinc=360./hstep;
	radius=sqrt((scene->Eye.x-scene->Target.x)*(scene->Eye.x-scene->Target.x)+
                (scene->Eye.y-scene->Target.y)*(scene->Eye.y-scene->Target.y)+
                (scene->Eye.z-scene->Target.z)*(scene->Eye.z-scene->Target.z));

	EyeToLamp.x=lamp.x-scene->Eye.x;
	EyeToLamp.y=lamp.y-scene->Eye.y;
	EyeToLamp.z=lamp.z-scene->Eye.z;
    
	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);
	
	Up=Norm(&scene->Up);
	
	z=Up;
	x=CrossN(&EyeToTarget,&Up);
	y=CrossN(&z,&x);
	
	Target=scene->Target;
	
	if(scene->sequenceSave > 0){	
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)return 1;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Ojbect");
	if(StatusWindow == NULL){
	    sprintf(WarningBuff,"StatusWindow NULL\n");
		WarningBatch(WarningBuff);
	}

	if(saveWhere == 3){
	    scene->FileOut=FileOut;
	}else if(saveWhere == 2 || saveWhere == 4){
	    scene->Dump=Dump;
	    scene->DumpData=(void *)myIcon;
	    r->saveWhere=saveWhere;
	    r->saveDirectory=Directory;
	    r->saveName=fileName;
	    r->saveFileName=saveFileName;
	}

        for(vert=90.0;vert>= -90.0; vert -= vinc){
			vertDegree = vert;
			if(vertDegree == 90){
				vertDegree -= .0001;
			}else if(vertDegree == -90){
				vertDegree += .0001;
			}
			rOut=radius*cos(vertDegree*DegreesToRadians);
			zEye=Mult(&z,radius*sin(vertDegree*DegreesToRadians));
			for(hort=0;hort<hstep;++hort){
			hortDegree=hort*hinc;

			xEye=Mult(&x,rOut*cos(hortDegree*DegreesToRadians));
			yEye=Mult(&y,rOut*sin(hortDegree*DegreesToRadians));


			scene->Eye=p(xEye.x+yEye.x+zEye.x+Target.x,
				 xEye.y+yEye.y+zEye.y+Target.y,
				 xEye.z+yEye.z+zEye.z+Target.z);
/*
			lamp.x=scene->Eye.x+EyeToLamp.x;
			lamp.y=scene->Eye.y+EyeToLamp.y;
			lamp.z=scene->Eye.z+EyeToLamp.z;
*/
			
			scene->Lamp=scene->Eye;

			sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
			sprintf(buff,"Creating: %s",fileName);
			mstrncpy(FileOut,Directory,1280);
			strncatToPath(FileOut,fileName,1280);
			uWriteStatusWindow(StatusWindow,buff);
			if(uCheckStatusWindow(StatusWindow,"Stop Contours"))goto ErrorOut;
			if(myIcon)RenderVolume(myIcon,1);
			++scene->sequence;	    
    	}
	}
	
	etime=rtime()-stime;
	
	ntime=scene->sequence-ntime;

	sprintf(WarningBuff,"Time To Rotate Object %.2f Seconds  %ld Frames  %.2f Frames/sec\n",
	    etime,ntime,((double)ntime)/etime);
	WarningBatch(WarningBuff);
	
	ret = 0;	
	
ErrorOut:

	scene->Eye=EyeSave;
	scene->Lamp=originSave;
	
	uStopStatusWindow(StatusWindow);
	if(r->saveWhere == 2){
	    if(r->saveFiles){
	       FilesOpenDefault(r->saveFiles);
	    }
	    r->saveFiles=NULL;
	} else if(r->saveWhere == 4){
	    if(r->saveFILE){
	       TrueMergedDone(r->saveFILE);
	       r->saveFILE=NULL;
	    }
	}
	scene->FileOut=NULL;
	scene->Dump=NULL;
	scene->DumpData=NULL;
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(myIcon)RenderVolume(myIcon,1);
	
	return ret;
}
static int Options1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct vScene *scene;
	volumetricPtr r;
	uDialogReturn *uR;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 5){
			reti=uDialogSet(uList,0L,
              uDialogSetExitOk,
              uDialogSetDone,uDialogSetDone
    		);
    
			if(reti)goto ErrorOut;
	}else if(uList->actionItem == 7){
		uR=uDialogGetReturn(uList);

    	if(!uR)goto ErrorOut;
    	    	
		scene->xResolution=uR[0].lreturn;
		scene->yResolution=uR[1].lreturn;
		
		scene->BackGround.red=uR[2].dreturn;		
    	scene->BackGround.green=uR[3].dreturn;
    	scene->BackGround.blue=uR[4].dreturn;
    	
		scene->debug=(int)uR[11].lreturn;

		scene->threads=(int)uR[17].lreturn;

		scene->xReflect=(int)uR[13].lreturn;
		scene->yReflect=(int)uR[14].lreturn;
		scene->zReflect=(int)uR[15].lreturn;
	
    	    	
	    if(r){
	    	fixScene(r);
			doFrameImage(myIcon);
			
	        RenderVolume(myIcon,1);
	    }
	}

	ret = 0;
	
ErrorOut:	
	/* if(uList)uDialogUpdate(uList); */
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int GetPlotOptions(IconPtr myIcon)
{
	static uDialogGroup Items[] =
        {
      /*  0 */      {"0", {116,14,80,20},uDialogText,uDialogGetLong},
      /*  1 */      {"0", {116,39,80,20},uDialogText,uDialogGetLong},
      /*  2 */      {"0", {116,64,80,20},uDialogText,uDialogGetDouble},
      /*  3 */      {"0", {201,64,80,20},uDialogText,uDialogGetDouble},
      /*  4 */      {"0", {286,64,80,20},uDialogText,uDialogGetDouble},
      /*  5 */      {"Ok", {220,200,70,20},uDialogButton,uDialogNone},
      /*  6 */      {"Cancel", {310,200,70,20},uDialogButton,uDialogNone},
      /*  7 */      {"Render", {20,200,70,20},uDialogButton,uDialogNone},
      /*  8 */      {"Background", {20,64,93,20},uDialogString,uDialogNone},
      /*  9 */      {"xsize", {20,14,65,20},uDialogString,uDialogNone},
      /* 10 */      {"ysize", {20,39,65,20},uDialogString,uDialogNone},
      /* 11 */      {"0", {265,140,80,20},uDialogText,uDialogGetLong},
      /* 12 */      {"debug", {200,140,55,20},uDialogString,uDialogNone},
      /* 13 */      {"Reflect x", {20,95,125,20},uDialogCheck,uDialogGetLong},
      /* 14 */      {"Reflect y", {20,120,125,20},uDialogCheck,uDialogGetLong},
      /* 15 */      {"Reflect z", {20,145,125,20},uDialogCheck,uDialogGetLong},
      /* 16 */      {"threads", {200,165,55,20},uDialogString,uDialogNone},
      /* 17 */      {"0", {265,165,80,20},uDialogText,uDialogGetLong},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,400,240};
     struct Color BackGround;
	
	int xReflect,yReflect,zReflect;
	struct vScene *scene;
	volumetricPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	long xsize,ysize;
	int debug;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	xsize=scene->xResolution;
		
	ysize=scene->yResolution;
		
	BackGround=scene->BackGround;		

	debug=scene->debug;
	
	xReflect=scene->xReflect;
	yReflect=scene->yReflect;
	zReflect=scene->zReflect;
	
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	reti=uDialogSet(uList,0L,
		          uDialogSetDouble,(double)scene->xResolution,
	              
	              uDialogSetItem,1L,
	              uDialogSetDouble,(double)scene->yResolution,
	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,scene->BackGround.red,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,scene->BackGround.green,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,scene->BackGround.blue,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)scene->debug,
	              
	              uDialogSetItem,13L,
	              uDialogSetLogical,(scene->xReflect == 1),
	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(scene->yReflect == 1),
	              
	              uDialogSetItem,15L,
	              uDialogSetLogical,(scene->zReflect == 1),
	              
	              uDialogSetItem,17L,
	              uDialogSetDouble,(double)scene->threads,
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetAction,Options1Action,	              

	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	    	
	if(uR[0].lreturn < 2){
	    Warning("xResolution Must be at least 2");
	    goto Start;
	}
	
	if(uR[1].lreturn < 2){
	    Warning("yResolution Must be at least 2");
	    goto Start;
	}
	
	scene->xResolution=uR[0].lreturn;
	
	scene->yResolution=uR[1].lreturn;
	
	scene->BackGround.red=uR[2].dreturn;		
	scene->BackGround.green=uR[3].dreturn;
	scene->BackGround.blue=uR[4].dreturn;
	scene->debug=(int)uR[11].lreturn;
	
	scene->xReflect=(int)uR[13].lreturn;
	scene->yReflect=(int)uR[14].lreturn;
	scene->zReflect=(int)uR[15].lreturn;
	
	scene->threads=(int)uR[17].lreturn;
	
	fixScene(r);
	doFrameImage(myIcon);

	ret = 0;	
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(ret){
		scene->xResolution=xsize;
		
		scene->yResolution=ysize;
		
		scene->BackGround=BackGround;		
	
		scene->debug=debug;
	
		scene->xReflect=xReflect;
		scene->yReflect=yReflect;
		scene->zReflect=zReflect;
	}
	if(myIcon)RenderVolume(myIcon,1);
	
	return ret;
}
static int fixScene(volumetricPtr r)
{
    struct vScene *scene;

	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	if(!r->myIcon)return 1;

	if(ScreenSetSize(scene->xResolution,scene->yResolution,r->myIcon))goto OutOfHere;

	return 0;
OutOfHere:

	return 1;
}
int doUniversalVolumetric(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct volumetricInfo ss;
	static struct Icon mm;
	static struct vScene se;

	static struct universalTableListStruct Local3[] = {
		{"REAL",4301,universal_TypeLong,(void *)&se.xResolution},
		{"REAL",4302,universal_TypeLong,(void *)&se.yResolution},
		{"DOUB",4304,universal_TypeDouble,(void *)&se.Degrees},

		{"DOUB",4305,universal_TypeDouble,(void *)&se.Up.x},
		{"DOUB",4306,universal_TypeDouble,(void *)&se.Up.y},
		{"DOUB",4307,universal_TypeDouble,(void *)&se.Up.z},
		
		{"DOUB",4308,universal_TypeDouble,(void *)&se.Eye.x},
		{"DOUB",4309,universal_TypeDouble,(void *)&se.Eye.y},
		{"DOUB",4310,universal_TypeDouble,(void *)&se.Eye.z},
		
		{"DOUB",4311,universal_TypeDouble,(void *)&se.Target.x},
		{"DOUB",4312,universal_TypeDouble,(void *)&se.Target.y},
		{"DOUB",4313,universal_TypeDouble,(void *)&se.Target.z},
		
		{"DOUB",4314,universal_TypeDouble,(void *)&se.Lamp.x},
		{"DOUB",4315,universal_TypeDouble,(void *)&se.Lamp.y},
		{"DOUB",4316,universal_TypeDouble,(void *)&se.Lamp.z},
		{"DOUB",4317,universal_TypeDouble,(void *)&se.Height},		
		
		{"REAL",4318,universal_TypeLong,(void *)&se.tcount},
		{"DOUB",4319,universal_TypeDoubleArray,(void *)&se.tvalue[0],128L},				
		{"DOUB",4320,universal_TypeDoubleArray,(void *)&se.topacity[0],128L},		
		
		{"DOUB",4321,universal_TypeDouble,(void *)&se.EyeSeparation},		
		{"REAL",4322,universal_TypeLong,(void *)&se.EyeView},
		{"REAL",4324,universal_TypeInt,(void *)&se.debug},
		{"REAL",4325,universal_TypeLong,(void *)&se.AutoCenterScreen},
		{"REAL",4326,universal_TypeInt,(void *)&se.AllOpaque},		
		{"REAL",4327,universal_TypeInt,(void *)&se.xReflect},		
		{"REAL",4328,universal_TypeInt,(void *)&se.yReflect},		
		{"REAL",4329,universal_TypeInt,(void *)&se.zReflect},		
		{"REAL",4330,universal_TypeInt,(void *)&se.threads},
		
	};
	

	
	static struct universalTableStruct Global3 = {
		"Volumetric Window Global parameters",universal_WindowVolumetric,
		(void *)&se,sizeof(se),
		NULL,0L,							
		Local3,sizeof(Local3)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local2[] = {
		{"STRI",4202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",4203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"REAL",4204,universal_TypeInt,(void *)&mm.eraseRectSize},
		{"STRI",4222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",4223,universal_TypeInt,(void *)&mm.WindowNameSet},									
		{"FILE",4224,universal_TypeFilesPtr,(void *)&mm.Files},	
	};
	
	static struct universalTableStruct Global2 = {
		"Volumetric Window Global parameters",universal_WindowVolumetric,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local[] = {
		{"RECT",4004,universal_TypeRect,(void *)&ss.windowRect},	
		{"REAL",4005,universal_TypeInt,(void *)&ss.Tool},
		
		{"REAL",4006,universal_TypeInt,(void *)&ss.pd.sType},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&ss.pd.sPmin},		
		{"DOUB",4008,universal_TypeDouble,(void *)&ss.pd.sPmax},		
		
		{"STRI",4009,universal_TypeString,(void *)&ss.pioName[0],sizeof(ss.pioName)},
		{"REAL",4010,universal_TypeInt,(void *)&ss.pioIndex},

		{"REAL",4011,universal_TypeLong,(void *)&ss.CurrentFrame},						
			
		{"TABL",4200,universal_TypeTable,(void *)&Global2},	
		{"TABL",4201,universal_TypeTable,(void *)&Global3},	
				
	};
	

	static struct universalTableStruct Global= {
		"Volumetric Window parameters",universal_WindowVolumetric,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};


	struct vScene *scene;
	volumetricPtr s;
	int ret;
	
	s=NULL;
	scene=NULL;
	if(myIcon && (myIcon->WindowType == VolumetricType)){
		s=(volumetricPtr)myIcon->DWindow;
		if(!s)return 1;
		scene=s->scene;
		if(!scene)return 1;
	}else{
	    myIcon=NULL;
	}

	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct volumetricInfo);
	
	Global2.tableDynamic=myIcon;
	Global2.tableDynamicSpace=sizeof(struct Icon);
	
	Global3.tableDynamic=scene;
	Global3.tableDynamicSpace=sizeof(struct vScene);
	
	ret=1;
	if(flag == universal_SetDefaults){
	
	    if(s && myIcon && scene){
			uGetPortBoundsGlobal(myIcon,&s->windowRect);			
			if(moveUniversalTable(&Global,universal_MoveToStatic))goto ErrorOut;	
		}else{

			if(mm.infoSave.directory)cFree((char *)mm.infoSave.directory);
			mm.infoSave.directory=NULL;
			if(mm.infoLoad.directory)cFree((char *)mm.infoLoad.directory);
			mm.infoLoad.directory=NULL;
			mm.eraseRectSize=140;
						
			uSetRect(&ss.windowRect,117,45,300,300);
			ss.Tool=ControlModeZoom;
			se.xResolution=300;
			se.yResolution=300;
			
			se.Degrees=20;
			
			se.BackGround=c(1.0,1.,1.);
			se.Up=p(0.,0.,1.);
			se.Eye=p(400.,400.,400.);
			se.Target=p(0.,0.,0.);
			se.Lamp=se.Eye;
			se.Height=1.0;
			
			se.tcount=2;
			se.tvalue[0]=1;
			se.tvalue[1]=10;
			se.topacity[0]=1;
			se.topacity[1]=1;
			
			se.EyeSeparation=0.0;
			se.EyeView=0;
						
		    ss.pd.sType=0;
		    ss.pd.sPmin=0;
		    ss.pd.sPmax=1;
		    
		    se.debug=0;
		    se.threads=0;
		    se.AllOpaque=0;
		    se.xReflect=0;
		    se.yReflect=0;
		    se.zReflect=0;
		    
		    ss.CurrentFrame=0;
			
		}
				
		ss.defaultsSet=TRUE;
		
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;				
		doUniversalVolumetric(myIcon,universal_GetDefaults,NULL);
		if(myIcon && uPref.Files){
			doLoadImages(myIcon,uPref.Files);
		    uPref.Files=NULL;
		}		
	}else if(flag == universal_GetDefaults){		
	    if(s && myIcon && scene){
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
		    fixScene(s);
	        RenderVolume(myIcon,1);
	    }
	}else if(flag == universal_Stop){	
		if(freeUniversalTable(&Global))goto ErrorOut;				
	}else{
		sprintf(WarningBuff,"doUniversalVolumetric unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}

static int RenderVolume(IconPtr myIcon,int flag)
{
	struct SetFrameData  *sd;
	struct FileInfo2 *Files;
	struct vScene *scene;
	volumetricPtr s;
	long length;
	uPort save;

	if(!myIcon)return 1;
	s=(volumetricPtr)myIcon->DWindow;
	if(!s)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	scene=s->scene;
	if(!scene)return 1;
	sd=&s->sd;

	doViewSend(myIcon,s->BatchName);
		
	zerol((char *)sd,sizeof(struct SetFrameData));

	sd->CurrentFrame=s->CurrentFrame;
	sd->type = FRAME_DATA_RASTER;

	if(s->pioName){
		mstrncpy(sd->pioName,s->pioName,255);
		sd->pioIndex=s->pioIndex;			    
	}						
	sd->typePlotRange = FRAME_RANGE_LIMITS;

	sd->pd=s->pd;

	if(SendMessageByName(s->BatchName,MessageType_GET_REGION_DATA,sd)){
	/*
	    if(strcmp(s->pioName,"density")){
	        InvalRectMyWindow(myIcon);
	    	mstrncpy(s->pioName,"density",255);
	   	 	s->pioIndex=0;
	    }
	 */
		return 1;
	}
	
    s->buffer=sd->data;
    s->xsize=sd->xsize;
    s->ysize=sd->ysize;

    length=3*s->xsize*s->ysize;
    if(SetBuffers(length))goto ErrorOut;
    
    if(scene->Dump)
        (*scene->Dump)(scene,s->buffer,xg.buffin,sd->xsize,sd->ysize,scene->DumpData);
 	
ErrorOut:
    if(flag){
		uGetPort(&save);
		SetPortMyWindow(myIcon);
		drawIt(myIcon);
		uSetPort(&save);
	}
	
	return 0;
}
static int Dump(struct vScene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData)
{
	IconPtr myIcon;
	volumetricPtr r;

	myIcon=(IconPtr)DumpData;
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	if(!scene)return 1;

	if(r->saveWhere == 1){
	    return 0;
	}else if(r->saveWhere == 2){
	    if(r->saveFiles == NULL){
	        r->saveFiles=FilesTRUE();
	        if(!r->saveFiles)return 1;
	    }
	    return FilesSaveTRUEToMemory(r->saveName,bline,bout,
	                                 xsize,ysize,r->saveFiles);
	}else if(r->saveWhere == 4){
	    char path[1024];
	    if(r->saveFILE == NULL){
	        mstrncpy(path,r->saveDirectory,1024);
	        strncatToPath(path,r->saveFileName,1024);
	        r->saveFILE=fopen(path,"wb");
	        if(!r->saveFILE)return 1;
	    }
	    return TrueWriteMerge(r->saveName,bline,bout,
	                          xsize,ysize,r->saveFILE);
	}
	return 0;
}
static int doOpacity(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
      /*  0 */      {"Ok", {10,50,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {80,50,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,60,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"Render", {315,29,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"Value", {5,22,65,20},uDialogString,uDialogNone},
      /*  5 */      {"Opacity", {180,22,65,20},uDialogString,uDialogNone},
      /*  6 */      {"", {50,2,110,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"", {240,2,110,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"File List",{20,60,65,65},uDialogScrollList,uDialogNone},
      /*  9 */      {"enter", {10,50,65,20},uDialogButton,uDialogNone},
      /*  10 */      {"delete", {80,50,65,20},uDialogButton,uDialogNone},
      /*  11 */      {"enter", {10,50,65,20},uDialogButton,uDialogNone},
      /*  12 */      {"delete", {80,50,65,20},uDialogButton,uDialogNone},
      /*  13 */      {"All Opaque", {20,2,120,20},uDialogCheck,uDialogGetLong},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,400,260};
 	
	struct ScrollList *List8;
	struct ScrollList *List2;
	struct vScene *scene;
	double topacity[128];
	double tvalue[128];
	char number[256];
	volumetricPtr r;
 	uDialogList *uList;
	uDialogReturn *uR;
	long count,tcount,n;
    int ret;
	
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	tcount=scene->tcount;
	
    for(n=0;n<tcount;++n){
        tvalue[n]=scene->tvalue[n];
        topacity[n]=scene->topacity[n];
    }
	
		
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Opacity",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	if(uDialogSet(uList,0L,
	
		          uDialogSetItem,13L,
	              uDialogSetLogical,(scene->AllOpaque  == 1),
	              uDialogSetMyIcon,myIcon,
	              uDialogSetMove,doScale2Move,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
	
Start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List8,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)goto ErrorOut;
    
    count=ScrollListCount(List2);
	        
    
	if(count != ScrollListCount(List8)){
	    Warning("Lists Must Be The Same Length");
	    goto Start;
	}
	
	if(count < 2){
	    Warning("Lists Must Have At Least 2 Points");
	    goto Start;
	}
	
	scene->AllOpaque=(int)uR[13].lreturn;

	scene->tcount=count;
	
    for(n=0;n<count;++n){
        ScrollListGet(number,n,List2);
        sscanf(number,"%lf",&scene->tvalue[n]);
        ScrollListGet(number,n,List8);
        sscanf(number,"%lf",&scene->topacity[n]);
    }
       	
	adjustPaletteV(scene,r->mat);

	doFrameImage(myIcon);

	ret=0;

ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	if(ret){
		scene->tcount=tcount;
		
	    for(n=0;n<tcount;++n){
	        scene->tvalue[n]=tvalue[n];
	        scene->topacity[n]=topacity[n];
	    }

  	}
	if(myIcon)RenderVolume(myIcon,1);
	
	return ret;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	struct ScrollList *List8;
	struct ScrollList *List2;
	struct vScene *scene;
	char number[256],*np[1];
	volumetricPtr r;
	IconPtr myIcon;
	long count;
	int ret;
	long n;
	
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;


	if(!IsShift()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List8,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	

    count=ScrollListCount(List2);

    ScrollListDelete(0L,count,List2);

    count=ScrollListCount(List8);

    ScrollListDelete(0L,count,List8);

    for(n=0;n<scene->tcount;++n){
        double ll;

        ll=(double)((long)scene->tvalue[n]);
        if(ll == scene->tvalue[n]){
            sprintf(number,"%ld%c",(long)(scene->tvalue[n]),0);
        }else{
            sprintf(number,"%g%c",scene->tvalue[n],0);
        }
        ScrollListInsertSort(number,3,LIST_SORT_DOUBLE,List2);

        ll=(double)((long)scene->topacity[n]);
        if(ll == scene->topacity[n]){
            sprintf(number,"%ld%c",(long)(scene->topacity[n]),0);
        }else{
            sprintf(number,"%g%c",scene->topacity[n],0);
        }
        np[0]=number;
		ScrollListInsertOneType(np,3,-1L,1L,List8);


    }	    


	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	
	return 0;
}

static int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	struct ScrollList *list8;
	uRect rList1,rList2;
	uRect rList4,rList5;
	uRect rList6,rList7;
	uRect rList9,rList10;
	uRect rList11,rList12;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;
	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,10,r.ysize-25,
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize-85,r.ysize-25,
              	              	              
              uDialogSetItem,3L,
              uDialogSetLocation,(r.xsize)/2-60,r.ysize-25,

              uDialogSetItem,2L,
              uDialogGetRect,&rList1,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,8L,
              uDialogGetRect,&rList2,
              uDialogGetScrollList,&list8,
              	              	              

              uDialogSetItem,4L,
              uDialogGetRect,&rList4,
              
              uDialogSetItem,5L,
              uDialogGetRect,&rList5,
              
              uDialogSetItem,6L,
              uDialogGetRect,&rList6,
              
              uDialogSetItem,7L,
              uDialogGetRect,&rList7,
              
              	              	              
              uDialogSetItem,9L,
              uDialogGetRect,&rList9,
              
              uDialogSetItem,10L,
              uDialogGetRect,&rList10,
              
              uDialogSetItem,11L,
              uDialogGetRect,&rList11,
              
              uDialogSetItem,12L,
              uDialogGetRect,&rList12,
              
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)return 1;
	
	
			
	rList1.x = 15;
	rList1.y = 45;
	rList1.xsize = (r.xsize-rList1.x)/2;
	rList1.ysize = r.ysize-80-rList1.y;
	
	rList4.x=rList1.x;
	
	
	rList6.x=rList1.x;
	rList6.xsize=rList1.xsize-ScrollOffSet;
	rList6.y=rList1.y+rList1.ysize;
	
	rList9.x=rList6.x;
	rList9.y=rList6.y+rList6.ysize+5;
	
	
	rList10.x=rList6.x+rList6.xsize-rList10.xsize;
	rList10.y=rList9.y;
	
	
	ScrollListFitRect(&rList1,list2);
	
	
	rList2=rList1;
	rList2.x = rList1.x+rList1.xsize;

	rList5.x=rList2.x;

	rList7.x=rList2.x;
	rList7.xsize=rList2.xsize-ScrollOffSet;
	rList7.y=rList2.y+rList2.ysize;
	
	rList11.x=rList7.x;
	rList11.y=rList7.y+rList7.ysize+5;
	
	
	rList12.x=rList7.x+rList7.xsize-rList12.xsize;
	rList12.y=rList11.y;
		
	ScrollListFitRect(&rList2,list8);

	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList1,
              
              uDialogSetItem,4L,
              uDialogSetRect,&rList4,
              
              uDialogSetItem,5L,
              uDialogSetRect,&rList5,
              
              uDialogSetItem,6L,
              uDialogSetRect,&rList6,
              
              uDialogSetItem,7L,
              uDialogSetRect,&rList7,
              
              uDialogSetItem,8L,
              uDialogSetRect,&rList2,
              	              	              
              uDialogSetItem,9L,
              uDialogSetLocation,rList9.x,rList9.y,
              
              uDialogSetItem,10L,
              uDialogSetLocation,rList10.x,rList10.y,
              
              uDialogSetItem,11L,
              uDialogSetLocation,rList11.x,rList11.y,
              
              uDialogSetItem,12L,
              uDialogSetLocation,rList12.x,rList12.y,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	return 0;
}
static int doScale2Action(struct uDialogStruct *uList)
{
	
	struct vScene *scene;
	char number[256];
	volumetricPtr r;
	IconPtr myIcon;
	uDialogReturn *uR;
	struct ScrollList *list2;
	struct ScrollList *list8;
	long count,n;
	char *np[1];
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(volumetricPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	
	uR=NULL;
	ret = 1;
	
	
	reti=uDialogSet(uList,0L,

              uDialogSetItem,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&list8,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(reti)return 1;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
	
	
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	
	
	if(uList->actionItem == 3){	
	    count=ScrollListCount(list2);
		        
	    
		if(count != ScrollListCount(list8)){
		    Warning("Lists Must Be The Same Length");
		    goto ErrorOut;
		}
		
		if(count < 2){
		    Warning("Lists Must Have At Least 2 Points");
		    goto ErrorOut;
		}
				
		scene->AllOpaque=(int)uR[13].lreturn;
		
		scene->tcount=count;
		
	    for(n=0;n<count;++n){
	        ScrollListGet(number,n,list2);
	        sscanf(number,"%lf",&scene->tvalue[n]);
	        ScrollListGet(number,n,list8);
	        sscanf(number,"%lf",&scene->topacity[n]);
	    }
	
		if(myIcon)RenderVolume(myIcon,1);
		
	
	}else if(uList->actionItem == 9){	
	    ScrollListInsertSort(uR[6].sreturn,3,LIST_SORT_DOUBLE,list2);
	    ScrollListUpdate(list2);
	}else if(uList->actionItem == 10){	
	    ScrollListDeleteSelected(list2);
	    ScrollListUpdate(list2);
	}else if(uList->actionItem == 11){		
	    long nl;    
		nl=ScrollListFirstSelect(list8);
        np[0]=uR[7].sreturn;
		ScrollListInsertOneType(np,3,nl,1L,list8);		
	    if(nl >= 0)ScrollListSetTop(nl,list8);
	    ScrollListUpdate(list8);
	}else if(uList->actionItem == 12){	
	    ScrollListDeleteSelected(list8);
	    ScrollListUpdate(list8);
	}
	ret = 0;
ErrorOut:
/*
	if(uList)uDialogUpdate(uList);
*/
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
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
	volumetricPtr t;
	
	if(myIcon && (t=(volumetricPtr)myIcon->DWindow)){
		PageView=RectConvert(PageViewi);
	    GetClip(myIcon->saveRegion);
	    ClipRect(&PageView);
		DRect.top = 0;
		DRect.left = 0;
  		DRect.right= (int)((double)myIcon->uviewRect.xsize*t->scale);
		DRect.bottom=(int)((double)myIcon->uviewRect.ysize*t->scale);

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
	myIcon=myIcon;
}
static void tDumpItP(IconPtr myIcon,uRect *PageViewi,int scale)
{
	myIcon=myIcon;
	PageViewi=PageViewi;
	scale=scale;
}
#endif

