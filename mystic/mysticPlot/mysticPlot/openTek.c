#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uFiles.h"
#include "Message.h"
#include "uLib.h"
#include "uDialog.h"
#include "SaveAllImages.h"
#include "Universal.h"
#include "uGraph.h"
#include "VoiceCommand.h"


char *strpathname(char *path,char *name);
int IsTEK(char *name);



struct stackT{
   long y;
   long x;
   double scale;
};

struct TekWindow{
    struct stackT *stackList;
    long stackCount;
    long stackMax;
    char *CurrentFileName;
    long ImageCount;
    long CurrentFrame;
    unsigned char *buffer;
    long length;
    long teklength;
    int ixmax,iymax;
    double scale;
    unsigned long TimeToFlip;
    short int AnimationOn;
    
    int Tool;			/* Tool Type  */
    int  LoopBackAndForth;
    int LoopFlag;

	uRect windowRect;
	int defaultsSet;
       
	int WalkMotionContinuous;
	int WalkMotion;
    
};

typedef struct TekWindow *tekPtr;


struct FileInfo2 *FilesOpenList(struct FileList *FilesIn);

int FilesGetTEK(char **ImageName,long *teklength,char **buffer,
                long CurrentFrame,struct FileInfo2 *Files);

int getPaletteRGB(int n,unsigned char *r,unsigned char *g,unsigned char *b);

int doWriteTEK(IconPtr myIcon,struct ImageSaveList *Images,int flag);
static int drawTek2(IconPtr myIcon,long CurrentFrame,uRect *DRect);

int freeFileList(struct FileInfo2 *Files);

static void drawIt2(IconPtr myIcon);

int OpenTek(struct FileList *Files);

static void drawIt(IconPtr myIcon);

static int SetFrame(IconPtr myIcon);

static void tMoveControls(IconPtr myIcon);
static int tInContent(IconPtr myIcon);
static int doClose(IconPtr myIcon);
static int UpdateWrite(IconPtr myIcon);
int doEraseDown(IconPtr myIcon);
int doMouseDown(IconPtr myIcon);
static int doMouseZoom(IconPtr myIcon);
static int checkStack(tekPtr t);


static int CheckMenu(IconPtr myIcon,int flag);
static int Menu(IconPtr myIcon,int menu,int item);
static int doAnimateMenu(IconPtr myIcon,int item);
static int KeyBoard(IconPtr myIcon,int c);
static int n2NullEvent(IconPtr myIcon);
static void doFrameImage(IconPtr myIcon);



static int tdothumb(controlPtr control,IconPtr myIcon);
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int tdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int tgoUp(controlPtr control,short part,IconPtr myIcon);
static int tgoDown(controlPtr control,short part,IconPtr myIcon);
static int tMoveDy(IconPtr myIcon);


#define FileMenu4			 	8003
#define openTekCommand 			1
#define closeTekCommand 		2
#define savePreferencesCommand 	3
#define PreferencesCommand 		4
#define saveTekCommand 			5
#define loadTekCommand 			6
#define previewTekCommand 		7
#define setupTekCommand 		8
#define printTekCommand 		9
#define quitTekCommand 			11

#define EditMenu4				8004
#define copyTekCommand 			1
#define clearTekCommand 		2
#define clearLoadTekCommand 	3
#define FrameTekCommand 		4
#define popItem					5
#define popAllItems				6
#define zoomIn					7
#define zoomOut					8
#define Information				10


#define AnimationMenu4				8005
#define animateTekForward 			1
#define animateTekBackward 			2
#define animateTekStepForward 		3
#define animateTekStepBackward 		4
#define animateTekStop	 			5
#define loopTek	 					7
#define loopBackTek	 				8
#define optionsTek	 				9
#define synchronizeTek	 			10


#define ToolsMenu4					8006
#define drawFramesTek	 			1
#define zoomTek	 					2
#define SweepImages	 				3

#define ToolsNewMenu				114


IconPtr NewTekWindow(IconPtr myIcon);
int doMouseSweepDown(IconPtr myIcon);

static int TekGetMessage(IconPtr myIcon,long MessageType,void *MessageData);
static int push(uRect *oldR,IconPtr myIcon);
static void tPrint(IconPtr myIcon);
int PageSetup(IconPtr myIcon);


static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect);
static void tDumpItP(IconPtr myIcon,uRect *PageView,int scale);

int FilesOpenTek(struct FileInfo2 *files);

static int DrawAnimate(IconPtr myIcon,long CurrentFrame);

int doTestSpeed(IconPtr myIcon);

static int nMouseMoved(IconPtr myIcon,uPoint pt);

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

static void DoActivate(IconPtr myIcon,int flag)
{
	tekPtr t;
	
	if(!myIcon)return;
	t=(tekPtr)myIcon->DWindow;
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
	tekPtr r;
	
	if(!myIcon)return 1;
	r=(tekPtr)myIcon->DWindow;
	if(!r)return 1;	
	
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
int FilesOpenTek(struct FileInfo2 *Files)
{
	if(!OpenTekFiles(Files))return 1;
	
	return 0;
}

int OpenTek(struct FileList *FilesIn)
{
	    struct FileInfo2 *Files;
	    IconPtr myIcon;
		
		Files=FilesOpenList(FilesIn);
		if(!Files)return 1;
		
		myIcon=OpenTekFiles(Files);
		if(myIcon)return 0;
		return 1;
		
}

IconPtr NewTekWindow(IconPtr myIcon)
{
	//myIcon=myIcon;
	return OpenTekFiles(NULL);
}

IconPtr OpenTekFiles(struct FileInfo2 *Files)
{
	static struct uMenuItems File[]={
	    {"File",  			FileMenu4,uWindowStandardMenu,0,},
	    {"New           /N",openTekCommand,0,0,},
	    {"Close         /W",closeTekCommand,0,0,},
	    {"----------------", openTekCommand,0,0,},
	    {"Save Preferences",savePreferencesCommand,0,0,},
	    {"Preferences...  ",PreferencesCommand,0,0,},
	    {"----------------",openTekCommand,0,0,},
	    {"Save Images...  ",saveTekCommand,0,0,},
	    {"Load Images...  ",loadTekCommand,0,0,},
	    {"----------------",openTekCommand,0,0,},
	    {"Print Preview...",previewTekCommand,0,0,},
	    {"Page Setup...   ",setupTekCommand,0,0,},
	    {"Print...      /P",printTekCommand,0,0,},
	    {"----------------",openTekCommand,0,0,},
	    {"Quit          /Q",quitTekCommand,0,0,},
	    {NULL,0,0,0,},
	};
		
	static struct uMenuItems Edit[]={
	    {"Edit",EditMenu4,uWindowStandardMenu,0,},
	    {"Copy          /C",copyTekCommand,0,0,},
	    {"Clear         /K",clearTekCommand,0,0,},
	    {"Clear and Load...",clearLoadTekCommand,0,0,},
	    {"----------------",openTekCommand,0,0,},
	    {"Frame Image   /F",FrameTekCommand,0,0,},
	    {"Pop One         ",popItem,0,0,},
	    {"Pop All         ",popAllItems,0,0,},
	    {"Zoom In         ",zoomIn,0,0,},
	    {"Zoom Out        ",zoomOut,0,0,},
	    {"----------------",openTekCommand,0,0,},
	    {"Information.../I",Information,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Animation[]={
	    {"Animation",  			AnimationMenu4,uWindowStandardMenu,0,},
	    {"Forward", 			animateTekForward,0,0,},
	    {"Backward",			animateTekBackward,0,0,},
	    {"Step Forward",		animateTekStepForward,0,0,},
	    {"Step Backward",		animateTekStepBackward,0,0,},
	    {"Stop",				animateTekStop,0,0,},
	    {"----------------",	openTekCommand,0,0,},
	    {"Loop",				loopTek,0,0,},
	    {"Loop Back and Forth",	loopBackTek,0,0,},
	    {"Options...",			optionsTek,0,0,},
	    {"Synchronize",			synchronizeTek,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Tools[]={
	    {"Tools",  				ToolsMenu4,uWindowStandardMenu,0,},
	    {"Drag Frames", 		drawFramesTek,0,0,},
	    {"Zoom",				zoomTek,0,0,},
	    {"Sweep Images",		SweepImages,0,0,},
	    { NULL,0,0,0},
	};


	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Animation},
	    {Tools},
	    {NULL}
	};
	
	uRect Rectl={40,40,400,200};
	
	    struct TekWindow *t;
	    IconPtr myIcon;
		static int Count;
		char buff[256];
		
		t=NULL;
		
		if(!(myIcon=uCreateIcon(TekType,&Rectl))){
			WarningBatch("OpenTekFiles uCreateIcon Failed\n");
			goto ErrorOut;
		}
		
		sprintf(buff,"TekWindow %03d",Count++);
		
		myIcon->pClose=doClose;
		myIcon->pUpdateWrite=UpdateWrite;
		myIcon->pInContent=tInContent;
		myIcon->pMoveControls=tMoveControls;
		myIcon->pCheckMenu=CheckMenu;
		myIcon->pMenu=Menu;
		myIcon->pKeyBoard=KeyBoard;
		myIcon->pNullEvent=n2NullEvent;
		myIcon->pPrint=tPrint;
		myIcon->uDump=tDumpItP;
		myIcon->pMessage=TekGetMessage;
		myIcon->uDrawAnimate=DrawAnimate;
		myIcon->pMouseMoved=nMouseMoved;
		
		myIcon->uDraw=drawTek;
		
		myIcon->uSpeech=SpeechCommand;	
		myIcon->pDoActivate=DoActivate;
		
		myIcon->pPageUp=tdoPageUp;
		myIcon->pPageDown=tdoPageDown;
		myIcon->pLineUp=tgoUp;
		myIcon->pLineDown=tgoDown;
		myIcon->pThumb=tdothumb;
		
		myIcon->mMenuOffSet=MenuOffSet;
				
		if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,
	              uWindowSetDone,uWindowSetDone)
		)goto ErrorOut;
	    
	    if(!(t=(struct TekWindow *)cMalloc((long)sizeof(struct TekWindow),8183))){
	        WarningBatch("OpenTekFiles Out of List Memory\n");
			goto ErrorOut;
	    }
	    zerol((char *)t,sizeof(struct TekWindow));
	    
	    myIcon->DWindow=(struct DumWindow *)t;

	    
		t->ixmax=1024;
		t->iymax=780;
		t->scale=.5;
		
		myIcon->framesPerSecond=30;

	    if(Files){
	        myIcon->Files=Files;
		    t->ImageCount=Files->ImageCount;
		    t->length=Files->length;
		}
		t->CurrentFrame=0;
		t->Tool=zoomTek;
		
		myIcon->CursorSpace=0;
		myIcon->eraseRectSize=120;
		
	    Files=NULL;
	    
	    if(!IsShift()){
	    	doUniversalTek(myIcon,universal_GetDefaults,NULL);
	    	FlushCurrentPort();
	    }
	    
		SetFrame(myIcon);
		doFrameImage(myIcon);
	    InvalRectMyWindow(myIcon);
	    
    	SetWindowToFileName(myIcon,myIcon->Files);
    	
    	StandardDoActive(myIcon,TRUE);
    	
		ScreenSetSize(Rectl.xsize,Rectl.ysize,myIcon);

		getPaletteRGB(12,myIcon->red,myIcon->green,myIcon->blue);
		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	    
	    tMoveControls(myIcon);
	    
	    CheckMenu(myIcon,TRUE);

		if(myIcon)myIcon->isOpen=TRUE;

		if(myIcon->Files){
		    setInfo(myIcon->Files->directory,&myIcon->infoLoad);
		    setInfo(myIcon->Files->directory,&myIcon->infoSave);
		}

	    return myIcon;
ErrorOut:
		if(Files)freeFileList(Files);
		if(myIcon)doClose(myIcon);
		return NULL;
}
static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
    struct FileInfo2 *Files;
	tekPtr t;

	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
	Files=myIcon->Files;
  	if(!Files)return 1;
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {		
	    if(t->Tool == zoomTek){
	       uSetCursor(myIcon,uPLUS_CURSOR);
	    }else{
	        uSetCursor(myIcon,uARROW_CURSOR);
	    }
	}else{
	    uSetCursor(myIcon,uARROW_CURSOR);
	}		
	
	return 0;	
}
static int TekGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct Message1000 *m;
	char windowName[256];
	uPort save;
	tekPtr s;
	long n;
	int ret;

	if(!myIcon)return 1;
	s=(tekPtr)myIcon->DWindow;
	if(!s)return 1;
	m=(struct Message1000 *)MessageData;

	ret=1;
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	switch(MessageType){
	case MessageType_LIMITS:
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
	case MessageType_GET_LINE_DATA:
	    goto ErrorOut;
	case MessageType_GET_FILE_LIST:
		{
		    int isFound;
		    
			if(!myIcon->Files)return 0;
			
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
		GetWindowName(myIcon,windowName,0);
	    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
	    WarningBatch(WarningBuff);
	    printFileInformation(myIcon->Files);
		goto OK;
	}
	InvalRectMyWindow(myIcon);
	drawIt(myIcon);
OK:
	ret=0;
ErrorOut:
	uSetPort(&save);
	return ret;
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
    
 	static uRect  Rectl   = {100,50,339,151};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	tekPtr t;
	int reti;
    int ret;
		
	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
		
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Options",&Rectl,ItemCount);
	
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
static int n2NullEvent(IconPtr myIcon)
{
	tekPtr t;


	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;

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
       
	    if(myIcon->uDrawAnimate){
	        (*myIcon->uDrawAnimate)(myIcon,t->CurrentFrame);
	    }
	    
       if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
	       
		if(!t->LoopFlag){
		    if(t->AnimationOn > 0){	        
			    if(t->CurrentFrame+1 == t->ImageCount){
	            	t->AnimationOn=FALSE;
					InvalRectMyWindow(myIcon);
			    }
		    }else if(!t->CurrentFrame){ 
			    t->AnimationOn=FALSE;
				InvalRectMyWindow(myIcon);
		    }
		}
	}
	return 0;
}
static int KeyBoard(IconPtr myIcon,int c)
{

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
	return 0;
}
static int doAnimateMenu(IconPtr myIcon,int item)
{
	tekPtr t;


	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
	
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
	        InvalRectMyWindow(myIcon);
	        break;
	    case loopTek:
	        t->LoopFlag = !t->LoopFlag;
	        break;
	    case loopBackTek:
	        t->LoopBackAndForth = !t->LoopBackAndForth;
	        break;
 		case optionsTek:
 		 	doOptions(myIcon);
  		 	break;
 		case synchronizeTek:
 		 	myIcon->Synchronize = !myIcon->Synchronize;
  		 	break;
	}
	if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
	return 0;
}
static int doClear(IconPtr myIcon)
{
	uRect portRect;
	tekPtr t;

	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
	
	uGetPortBounds(myIcon,&portRect);
	uClipRect(&portRect,myIcon);
	uEraseRect(&portRect,myIcon);
		
	if(myIcon->Files){
		freeFileList(myIcon->Files);
		myIcon->Files=NULL;
	}
	t->buffer = NULL;
	t->CurrentFileName = NULL;
	t->ImageCount=0;
	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return 0;
}

static int n2SaveScrap(IconPtr myIcon)
{
	long Length;
	uRect Box;
	tekPtr t;

	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
		
		
  	if(!t->ImageCount)return 1;
  	
  	Box.x = 0;
  	Box.y = 0;
  	Box.xsize=(int)((double)t->ixmax*t->scale);
  	Box.ysize=(int)((double)t->iymax*t->scale);
  	  	
	drawToScrap(myIcon,&Box,t->CurrentFrame,1);


	if(t->teklength > 0 && t->buffer){
	    Length=uPutScrap(t->teklength,"TEK3",(char *)t->buffer);
	    if(Length < 0){
	        WarningBatch("n2SaveScrap Error Writing Scrap\n");
	    }
	}
	return 0;

}
static int doLoadImages(IconPtr myIcon,struct FileList *Files)
{
	struct FileInfo2 *Files2;
	
	int TypeList[]={OPEN_ALL_FILE_TYPES,0};
	char *pathname;
	int isTek;
	tekPtr t;

	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;

	if(!Files){
		Files=uOpenFileDialog(myIcon,TypeList);		
		if(!Files)return 1;
	}
		
	isTek=FALSE;
	if(Files->type != TEK_DISK && Files->type != TEK_MEMORY){
		pathname=strpathname(Files->directory,Files->name[0]);
		if(pathname){
		    if(IsTEK(pathname)){
		        Files->type=TEK_MEMORY;
				isTek=TRUE;
		    }
		    cFree(pathname);
		}

	}else{
		isTek=TRUE;
	}
	
	if(isTek){
		if(myIcon->Files)return OpenTek(Files);
		Files2=FilesOpenList(Files);
		if(!Files2)return 1;
		myIcon->Files=Files2;		
	    t->ImageCount=Files2->ImageCount;
	    t->length=Files2->length;				
	    tMoveControls(myIcon);
    	SetWindowToFileName(myIcon,Files2);
		doFrameImage(myIcon);
    	InvalRectMyWindow(myIcon);
	}else{
		Files2=FilesOpenList(Files);
		if(!Files2)return 1;
	    return FilesOpenDefault(Files2);
	}
	return 0;
}
static int Menu(IconPtr myIcon,int menu,int item)
{
	uRect Box;
	tekPtr t;

	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;


	Box.x = 0;
	Box.y = 0;
	Box.xsize=(int)((double)t->ixmax*t->scale);
	Box.ysize=(int)((double)t->iymax*t->scale);
	
		
	uSetCursor(myIcon,-uWATCH_CURSOR);
	
	switch(menu){
	case AppleMenu:
		if(item == 1){
		    doAbout(myIcon);
		    break;
		}
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
		} else if(item == saveTekCommand){
			doSavePlots(myIcon,myIcon->Files,t->CurrentFrame,&Box);
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
		}				    
		break;		
	case EditMenu4:
		
		switch(item){		
 		case Information:
 		    doWindowInformation(myIcon);tMoveControls(myIcon);
 		    break;
 		case 1:
 			n2SaveScrap(myIcon);
 		    break;
 		case 2:
 			doClear(myIcon);
 		    break;
 		case clearLoadTekCommand:
 			doClear(myIcon);
			doLoadImages(myIcon,NULL);
 		    break;
 		case 4:
 			doFrameImage(myIcon);
 		    break;
 		case popItem:
		  if(t->stackCount <= 0 || !t->stackList)goto OutOfHere;
		  --(t->stackCount);
POP:
		  t->scale=t->stackList[t->stackCount].scale;
		  
		  tMoveControls(myIcon);
		  uSetControlValue(myIcon->VControl,t->stackList[t->stackCount].y);
		  uSetControlValue(myIcon->HControl,t->stackList[t->stackCount].x);
 		  InvalRectMyWindow(myIcon);
 		break;
 		case popAllItems:
		  if(t->stackCount <= 0 || !t->stackList)goto OutOfHere;
		  t->stackCount=0;
		  goto POP;
 		case zoomIn:	
		    if(checkStack(t)){
			    goto OutOfHere;
		    }
		    t->stackList[t->stackCount].y=uGetControlValue(myIcon->VControl);
		    t->stackList[t->stackCount].x=uGetControlValue(myIcon->HControl);
		    t->stackList[t->stackCount].scale=t->scale;
		    ++(t->stackCount);
		    t->scale *= 2;
		    if(t->scale > 32.){
		        t->scale = 32.;
	   			WarningBatch("Scale Factor Has Been Set Back To 32\n");
		    }
		    tMoveControls(myIcon);
		    uSetControlValue(myIcon->VControl,(long)((double)(t->stackList[t->stackCount-1].y)*t->scale/t->stackList[t->stackCount-1].scale));
		    uSetControlValue(myIcon->HControl,(long)((double)(t->stackList[t->stackCount-1].x)*t->scale/t->stackList[t->stackCount-1].scale));
 		   	InvalRectMyWindow(myIcon);
 		 break;
 		case zoomOut:	
		    if(checkStack(t)){
			    goto OutOfHere;
		    }
		    t->stackList[t->stackCount].y=uGetControlValue(myIcon->VControl);
		    t->stackList[t->stackCount].x=uGetControlValue(myIcon->HControl);
		    t->stackList[t->stackCount].scale=t->scale;
		    ++(t->stackCount);
		    t->scale /= 2;
		    tMoveControls(myIcon);
		    uSetControlValue(myIcon->VControl,(long)((double)(t->stackList[t->stackCount-1].y)*0.5));
		    uSetControlValue(myIcon->HControl,(long)((double)(t->stackList[t->stackCount-1].x)*0.5));
 		   InvalRectMyWindow(myIcon);
 		 break;
		 
		}
		
		break;
	case AnimationMenu4:
		doAnimateMenu(myIcon,item);
		break;
	case ToolsMenu4:
		if(item == zoomTek){
	    	t->Tool=zoomTek;
		}else if(item == drawFramesTek){
	    	t->Tool=drawFramesTek;
		}else if(item == SweepImages){
	    	t->Tool=SweepImages;
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
int doWriteTEK(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	char *output;
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,seq,nameflag;
	char *namef,*path;
	IconPtr StatusWindow;
	long ni;
	int ret;
	uRect rSave;
	FILE *Out;
	tekPtr t;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
  	
  	if(myIcon->WindowType != TekType){
  		WarningBatch("Error Only Tektronix Windows Save Tektronix Plots\n");
  		return 1;
  	}
  	
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
  	if(!t->ImageCount)return 1;
  	
	Out=NULL;
	
	//flag=flag;
	
	ret=1;
		
	StatusWindow=NULL;
		
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;
		
	StatusWindow=uStartStatusWindow(myIcon,"WriteTEK");
		
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(myIcon->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);

	    t->CurrentFrame=ni;
	    SetFrame(myIcon);
	    				
	    mstrncpy(filepath,Images->directory,2048);
	    strncatToPath(filepath,name,2048);
	    	    
	    Out=fopen(filepath,"wb");
	    if(!Out)goto OutOfHere;
	    
	    if(putString(t->buffer,t->teklength,Out))goto OutOfHere;
		if(SetFileType(filepath,"TEKB"))goto OutOfHere;
		if(Out)fclose(Out);
		Out=NULL;
	    		    
	    if(uCheckStatusWindow(StatusWindow,"Stop Writing TEK ?"))goto OutOfHere;
		
	}
	ret=0;
OutOfHere:
	uStopStatusWindow(StatusWindow);
	
	uSetClipRect(&rSave,myIcon);

	if(Out)fclose(Out);
	
	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return ret;
}
static int CheckMenu(IconPtr myIcon,int flag)
{
    struct FileInfo2 *Files;
	tekPtr t;


	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	Files=myIcon->Files;
		
	if(flag){
		if(!t)return 1;	    
		
		
		if(Files){
			uActivateMenuItem(myIcon,FileMenu4,previewTekCommand,isInstalled(Installed_PrintPreview));
			uActivateMenuItem(myIcon,FileMenu4,printTekCommand,isInstalled(Installed_Print));
			uActivateMenuItem(myIcon,FileMenu4,saveTekCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,copyTekCommand,TRUE && isInstalled(Installed_CopyImages));
			uActivateMenuItem(myIcon,EditMenu4,clearTekCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,FrameTekCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,zoomIn,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,zoomOut,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,popItem,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,popAllItems,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekForward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekBackward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepForward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepBackward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStop,TRUE);
		}else{
			uActivateMenuItem(myIcon,FileMenu4,previewTekCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,printTekCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,saveTekCommand,FALSE);
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
		}
		
		uActivateMenuItem(myIcon,FileMenu4,setupTekCommand,isInstalled(Installed_PrintSetup));	    
		uActivateMenuItem(myIcon,FileMenu4,savePreferencesCommand,isInstalled(Installed_TekPreferences));
	    		
		uCheckMenuItem(myIcon,AnimationMenu4,loopTek,t->LoopFlag);
		uCheckMenuItem(myIcon,AnimationMenu4,loopBackTek,t->LoopBackAndForth);
		uCheckMenuItem(myIcon,AnimationMenu4,synchronizeTek,myIcon->Synchronize);
	    	    
		uCheckMenuItem(myIcon,ToolsMenu4,drawFramesTek, (t->Tool == drawFramesTek));
		uCheckMenuItem(myIcon,ToolsMenu4,zoomTek,       (t->Tool == zoomTek));
		uCheckMenuItem(myIcon,ToolsMenu4,SweepImages,       (t->Tool == SweepImages));
	    	    
	    doWindowMenu();
	}
	return 0;
}
static int tInContent(IconPtr myIcon)
{
	struct TekWindow *t;
	uPoint uwhere;
	uRect cBox;
	
	
	if(!myIcon || !(t=(struct TekWindow *)myIcon->DWindow))return 1;

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
	    }else if(t->Tool == SweepImages){
			doMouseSweepDown(myIcon);
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
static int doMouseZoom(IconPtr myIcon)
{
	struct TekWindow *t;
	uRect oldR;

	if(!myIcon)return 1;
	t=(struct TekWindow *)myIcon->DWindow;
	if(!t)return 1;

	if(drawBoxOnScreen(myIcon,&oldR,NULL))return 1;
	
	push(&oldR,myIcon);
	
	return 0;
}
static int push(uRect *oldR,IconPtr myIcon)
{
	tekPtr t;
	double scale,scalex,scaley;
	long top,left,xsize,ysize,vxsize,vysize;
	
	if(!oldR)return 1;
	if(!myIcon)return 1;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return 1;
	
	xsize=oldR->xsize;
	ysize=oldR->ysize;

	if(xsize <= 2 || ysize <= 2)return 0;

	vxsize=myIcon->uviewRect.xsize;
	vysize=myIcon->uviewRect.ysize;

	if(vxsize <= 0 || vysize <= 0){
	   Warning("Error Zero View Rectangle Found");
	   return 1;
	}

	if(checkStack(t)){
	    return 1;
	}
	top=t->stackList[t->stackCount].y=uGetControlValue(myIcon->VControl);
	left=t->stackList[t->stackCount].x=uGetControlValue(myIcon->HControl);
	scale=t->stackList[t->stackCount].scale=t->scale;
	++(t->stackCount);


	scalex=(double)((t->scale*((double)vxsize-ScrollOffSet))/(double)xsize);
	scaley=(double)((t->scale*((double)vysize-ScrollOffSet))/(double)ysize);
	if(scalex < scaley){
	    t->scale=scalex;
	}else{
	    t->scale=scaley;
	}
	if(t->scale > 32.){
	    t->scale = 32.;
	    WarningBatch("Scale Factor Has Been Set Back To 32\n");
	}

	tMoveControls(myIcon);

	uSetControlValue(myIcon->VControl,(long)((double)(oldR->y+top-myIcon->mMenuOffSet)*t->scale/scale));
	uSetControlValue(myIcon->HControl,(long)((double)(oldR->x+left)*t->scale/scale));
	
 	InvalRectMyWindow(myIcon);

 	    
	return 0;
}
#define STACK_INCREMENT 50
static int checkStack(tekPtr t)
{
	struct stackT *stackList;
        long stackMax;

	if(!t)return 1;

	if(t->stackCount+1 < t->stackMax)return 0;

	stackMax = t->stackMax+STACK_INCREMENT;

	stackList=NULL;
	if(t->stackList){
	    stackList=(struct stackT *)cRealloc((char *)t->stackList,stackMax*sizeof(struct stackT),8269);
	    if(!stackList){
	        sprintf(WarningBuff,"checkStack out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&stackList[t->stackMax],STACK_INCREMENT*sizeof(struct stackT));
	}else{
	    stackList=(struct stackT *)cMalloc(stackMax*sizeof(struct stackT),8185);
	    if(!stackList){
	        sprintf(WarningBuff,"checkStack out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)stackList,stackMax*sizeof(struct stackT));
	}

	t->stackMax=stackMax;
	t->stackList=stackList;
	return 0;
ErrorOut:
	return 1;
}
static int tdothumb(controlPtr control,IconPtr myIcon)
{
	//control=control;
	return tMoveDy(myIcon);
}
static int tdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
		int dy,newp;
		
		if(!myIcon || !control)return 0;
		
		//part=part;
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
		
		//part=part;
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
		//part=part;
		old=(int)uGetControlValue(control);
		uSetControlValue(control,old-8);
		if(uGetControlValue(control) == old)return 0;
		return tMoveDy(myIcon);

}
static int tgoDown(controlPtr control,short part,IconPtr myIcon)
{			
		int old;
		
		if(!myIcon || !control)return 0;
		//part=part;
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
static void doFrameImage(IconPtr myIcon)
{
	long xsize,ysize;
	tekPtr t;


	if(!myIcon)return;
	t=(tekPtr)myIcon->DWindow;
	if(!t)return;
	if(t->ImageCount <= 0)return;
	

	xsize=(long)((double)t->ixmax*t->scale);
	ysize=(long)((double)t->iymax*t->scale);
	
	uSizeWindow(myIcon,(int)(xsize+ScrollOffSet),(int)(ysize+ScrollOffSet+myIcon->mMenuOffSet));	
	
	InvalRectMyWindow(myIcon);

	tMoveControls(myIcon);
	
	uSetControlValue(myIcon->VControl,0L);
	uSetControlValue(myIcon->HControl,0L);
	SetFrame(myIcon);
}
static void tMoveControls(IconPtr myIcon)
{
	
	struct FileInfo2 *Files;
	controlPtr control;
	struct TekWindow *t;
	uRect eraseRect;
	int MaxValue;
	uRect r;
	int bottom;
	int right;	
	int top;
	int left;
	
	if(!myIcon)return;
	t=(struct TekWindow *)myIcon->DWindow;
	if(!t)return;
	Files=myIcon->Files;
	if(!Files){
		if(myIcon->VControl)HideControl(myIcon->VControl);
		if(myIcon->HControl)HideControl(myIcon->HControl);
	    return;
	}else{
		if(myIcon->VControl)ShowControl(myIcon->VControl);
		if(myIcon->HControl)ShowControl(myIcon->HControl);
	}
	
	uGetPortBounds(myIcon,&r);
	
	top=0;
	left=0;
	bottom=r.ysize;
	right=r.xsize;
	
	myIcon->uviewRect = r;
	myIcon->uviewRect.y = myIcon->mMenuOffSet;
	myIcon->uviewRect.x = 0;
	myIcon->uviewRect.ysize -= (myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace);
	myIcon->uviewRect.xsize -= ScrollOffSet;

	
	eraseRect.y=bottom-ScrollOffSet;
	eraseRect.x=left;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;
	
	control=myIcon->VControl;
	if(control){
		MoveControl(control,right-ScrollOffSet,top+myIcon->mMenuOffSet);
		SizeControl(control,ScrollOffSet,bottom-top-ScrollOffSet-myIcon->mMenuOffSet);
		if(t->ImageCount > 0){
			MaxValue=(int)(t->iymax*t->scale-(r.ysize-ScrollOffSet-myIcon->mMenuOffSet-myIcon->CursorSpace));
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
		if(t->ImageCount > 0){
			MaxValue=(int)(t->ixmax*t->scale-(right-ScrollOffSet));
			if(MaxValue < 0)MaxValue = 0;
		} else {
	    	MaxValue=0;
		}
		uSetControlMaximum(control,MaxValue);
	}
	
	InvalRectMyWindow(myIcon);
}
static int UpdateWrite(IconPtr myIcon)
{

		if(!myIcon || !myIcon->isOpen)return 1;
			    
		drawIt(myIcon);
				
	    uDrawControls(myIcon);
	    uDrawGrowIcon(myIcon);
	    
	    return 0;
}
static void drawIt(IconPtr myIcon)
{
	    struct TekWindow *t;
	    uRect portRect;
		uRect Box;
	    
		if(!myIcon)return;
		t=(struct TekWindow *)myIcon->DWindow;
		if(!t)return;
		
	
	    uClipRect(&myIcon->uviewRect,myIcon);
	    uEraseRect(&myIcon->uviewRect,myIcon);
	    
  		Box.x = 0;
  		Box.y = myIcon->mMenuOffSet;
  		Box.xsize=(int)((double)t->ixmax*t->scale);
  		Box.ysize=(int)((double)t->iymax*t->scale);
  		
		drawTek(myIcon,t->CurrentFrame,&Box);
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    
		nWriteBox(myIcon,t->CurrentFileName);
}
#define ANMODE 31
#define GRAPHMODE 29
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRecti)
{
		int mode,imove,seen96,xvalue,yvalue,width;
		int hix,hiy,lox,loy,extx,exty,extxsave,extysave;
		int xoff,yoff;
		unsigned char c,v;
		double scale;
		int ix,iy,tsize;
	    struct TekWindow *t;
		struct screenData s;
	    uFontInfo Info;
		long n,length;
		uRect SRect;
		uRect DRect;
	    
		if(!myIcon)return 1;
		t=(struct TekWindow *)myIcon->DWindow;
		if(!t)return 1;
		if(!DRecti)return 1;
		
		if(t->teklength <= 0)return 0;

		if(t->buffer == NULL)return 1;
		
       	t->CurrentFrame=CurrentFrame;

	   	SetFrame(myIcon);
	   	
	   	SRect.x=0;
	   	SRect.y=0;
	   	SRect.xsize=myIcon->uviewRect.xsize;
	   	SRect.ysize=myIcon->uviewRect.ysize;
		ScreenSetSize(SRect.xsize,SRect.ysize,myIcon);
		
		length=SRect.xsize*SRect.ysize*3+512L;

		if(SetBuffers(length))goto ErrorOut;		
		
		zerol((char *)xg.buffin,length);
		
		zerol((char *)&s,sizeof(struct screenData));
		s.buffer=xg.buffin;
		s.viewRect.xsize=SRect.xsize;
		s.viewRect.ysize=SRect.ysize;
		s.ScreenBits = 8;
		s.myIcon = myIcon;
		
		xoff=0;
		yoff=0;
		
		tsize=(int)(18*t->scale);
		
		uSetTextFace(myIcon,0);
		
		uSetTextFont(myIcon,DefaultFont);
		
		uSetFontSize(myIcon,tsize);
				
		uGetFontInfo(myIcon,&Info);
		
	    width=Info.CWidth;
	    
		scale=t->scale*(1024.)/(4096.);

		xvalue=0;
		yvalue=0;		
		if(myIcon->VControl)yvalue=(int)uGetControlValue(myIcon->VControl);
		if(myIcon->HControl)xvalue=(int)uGetControlValue(myIcon->HControl);
		
		mode=ANMODE;
		seen96=FALSE;
		imove = TRUE;
		hix=hiy=lox=loy=extx=exty=extxsave=extysave=0;

		ix=0;
		iy=0;
		for(n=0;n<t->teklength;++n){
	    	c=t->buffer[n];
	    	if(c == 7){
				continue;
	    	}else if(c == 31){
				mode=ANMODE;
				continue;
	    	}else if(c == 29){
	        /* reset all move draw information */
				mode=GRAPHMODE;
				imove = TRUE;
				seen96=FALSE;
				continue;
	    	}else if(c == 27){
	        	if(n >= t->teklength)break;
	        	c=t->buffer[++n];
				continue;
	    	} else if(c < 32){
	        	continue;
	    	}

	    	if(mode == ANMODE){
	        	if(c <= 127){
	            	char buff[2];
	            	buff[0]=c;
	            	buff[1]=0;
		    		uDrawStringU(buff,255,0,&s);  		
		    		ix += width;
		    		uMoveU(ix+xoff,iy+yoff,&s);
		    		
	        	}
				continue;
	    	}
	    	v=c & 31;
	    	if(c <= 63){
	        	if(seen96){
	           		hix=v;
	        	}else{
	           		hiy=v;
	        	}
	    	}else if(c <= 95){
	        	lox=v;
	        	ix=(hix << 7)+(lox<<2)+extx;
	        	ix=(int)((ix)*scale-xvalue+.5);
	        	iy=((hiy << 7)+(loy<<2)+exty);
	        	iy=(int)((3120-iy)*scale-yvalue+.5);
				if(imove){
		    		uMoveU(ix+xoff,iy+yoff,&s);
				}else{
		    		uLineU(ix+xoff,iy+yoff,255,&s);
				}
				imove=FALSE;
				seen96=FALSE;
	    	}else if(c <= 127){
	        	if(seen96){
	           		loy=v;
	           		extx=extxsave;
	           		exty=extysave;
	        	}else{
	           		extxsave=v & 3;
	           		extysave=(v >> 2) & 3;
	           		loy=v;
	        	}
				seen96=TRUE;
	    	}
		}
	
	DRect=SRect;
	DRect.x=DRecti->x;
	DRect.y=DRecti->y;
	
	ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	uScreenWrite(SRect.xsize,SRect.ysize,(unsigned char *)xg.buffin,(unsigned char *)xg.buffout,ScreenDataType8,&SRect,&DRect,myIcon);   
		
ErrorOut:
		return 0;
}
static int DrawAnimate(IconPtr myIcon,long CurrentFrame)
{
	tekPtr s;
	uPort save;
	
	if(!myIcon)return 1;
	s=(tekPtr)myIcon->DWindow;
	if(!s)return 1;

	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	if(s->CurrentFrame != CurrentFrame){
	    s->CurrentFrame=CurrentFrame;
	    SetFrame(myIcon);
	}
		
	drawIt2(myIcon);	
		
	uSetPort(&save);
	return 0;
}
static void drawIt2(IconPtr myIcon)
{
	    struct TekWindow *t;
	    uRect portRect;
		uRect Box;
	    
		if(!myIcon)return;
		t=(struct TekWindow *)myIcon->DWindow;
		if(!t)return;
		
	
	    uClipRect(&myIcon->uviewRect,myIcon);
	    uEraseRect(&myIcon->uviewRect,myIcon);
	    
  		Box.x = 0;
  		Box.y = myIcon->mMenuOffSet;
  		Box.xsize=(int)((double)t->ixmax*t->scale);
  		Box.ysize=(int)((double)t->iymax*t->scale);
  		
		drawTek2(myIcon,t->CurrentFrame,&Box);
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    
		nWriteBox(myIcon,t->CurrentFileName);
}
static int drawTek2(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
		int mode,imove,seen96,xvalue,yvalue,width;
		int hix,hiy,lox,loy,extx,exty,extxsave,extysave;
		int xoff,yoff;
		unsigned char c,v;
		double scale;
		int ix,iy,tsize;
	    struct TekWindow *t;
	    uFontInfo Info;
		long n;
	    
		if(!myIcon)return 1;
		t=(struct TekWindow *)myIcon->DWindow;
		if(!t)return 1;
		
		if(t->teklength <= 0)return 0;

		if(t->buffer == NULL)return 1;
		
       t->CurrentFrame=CurrentFrame;
       
	   SetFrame(myIcon);
				
		xoff=DRect->x;
		yoff=DRect->y;
		
		tsize=(int)(18*t->scale);
		
		uSetTextFace(myIcon,0);
		
		uSetTextFont(myIcon,DefaultFont);
		
		uSetFontSize(myIcon,tsize);
				
		uGetFontInfo(myIcon,&Info);
		
	    width=Info.CWidth;
	    
		scale=t->scale*(1024.)/(4096.);

		xvalue=0;
		yvalue=0;		
		if(myIcon->VControl)yvalue=(int)uGetControlValue(myIcon->VControl);
		if(myIcon->HControl)xvalue=(int)uGetControlValue(myIcon->HControl);
		
		mode=ANMODE;
		seen96=FALSE;
		imove = TRUE;
		hix=hiy=lox=loy=extx=exty=extxsave=extysave=0;

		ix=0;
		iy=0;
		for(n=0;n<t->teklength;++n){
	    	c=t->buffer[n];
	    	if(c == 7){
				continue;
	    	}else if(c == 31){
				mode=ANMODE;
				continue;
	    	}else if(c == 29){
	        /* reset all move draw information */
				mode=GRAPHMODE;
				imove = TRUE;
				seen96=FALSE;
				continue;
	    	}else if(c == 27){
	        	if(n >= t->teklength)break;
	        	c=t->buffer[++n];
				continue;
	    	} else if(c < 32){
	        	continue;
	    	}

	    	if(mode == ANMODE){
	        	if(c <= 127){
	            	char buff[2];
	            	buff[0]=c;
	            	buff[1]=0;
		    		uDrawString(buff,myIcon); 		    		
		    		ix += width;
		    		uMoveTo(ix+xoff,iy+yoff);
		    		
	        	}
				continue;
	    	}
	    	v=c & 31;
	    	if(c <= 63){
	        	if(seen96){
	           		hix=v;
	        	}else{
	           		hiy=v;
	        	}
	    	}else if(c <= 95){
	        	lox=v;
	        	ix=(hix << 7)+(lox<<2)+extx;
	        	ix=(int)((ix)*scale-xvalue+.5);
	        	iy=((hiy << 7)+(loy<<2)+exty);
	        	iy=(int)((3120-iy)*scale-yvalue+.5);
				if(imove){
		    		uMoveTo(ix+xoff,iy+yoff);
				}else{
		    		uLineTo(ix+xoff,iy+yoff,myIcon);
				}
				imove=FALSE;
				seen96=FALSE;
	    	}else if(c <= 127){
	        	if(seen96){
	           		loy=v;
	           		extx=extxsave;
	           		exty=extysave;
	        	}else{
	           		extxsave=v & 3;
	           		extysave=(v >> 2) & 3;
	           		loy=v;
	        	}
				seen96=TRUE;
	    	}
		}


		return 0;
}
static int SetFrame(IconPtr myIcon)
{
	struct TekWindow *t;
	char *buffer;

	if(!myIcon)return 1;
	t=(struct TekWindow *)myIcon->DWindow;
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

	if(FilesGetTEK(&(t->CurrentFileName),&(t->teklength),&buffer,
	                 t->CurrentFrame,myIcon->Files)){
	    WarningBatch("SetFrame Error - File List Cleared\n");
	    doClear(myIcon);
		return 1;
	}
	t->buffer=(unsigned char *)buffer;
	return 0;
	
}
static int doClose(IconPtr myIcon)
{
	struct TekWindow *t;

	stopLanguage();
		
	if(myIcon){
	    SetPortMyWindow(myIcon);
	    t=(struct TekWindow *)myIcon->DWindow;
   	    if((t)){
			if(myIcon->Files){
			    freeFileList(myIcon->Files);
		    	myIcon->Files=NULL;
			}
	        if(t->stackList){
	           cFree((char *)t->stackList);
	           t->stackList=NULL;
	        }
	        
	    		        
	    	myIcon->DWindow=NULL;
	        cFree((char *)t);
	        
		}
	    
	    myIcon=RemoveIcon(myIcon);
	    
	}
	return 0;
}
int doUniversalTek(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct TekWindow ss;
	static struct Icon mm;

	static struct universalTableListStruct Local2[] = {
		{"REAL",4201,universal_TypeInt,(void *)&mm.eraseRectSize},
		{"STRI",4202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",4203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"DOUB",4204,universal_TypeDouble,(void *)&mm.framesPerSecond},	
		{"REAL",4205,universal_TypeInt,(void *)&mm.Synchronize},	
		{"FILE",4218,universal_TypeFilesPtr,(void *)&mm.Files},	
		{"STRI",4222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",4223,universal_TypeInt,(void *)&mm.WindowNameSet},									
				
	};
	
	static struct universalTableStruct Global2 = {
		"Tektronix parameters",universal_WindowTEK,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local[] = {
		{"RECT",4004,universal_TypeRect,(void *)&ss.windowRect},	
		{"REAL",4005,universal_TypeInt,(void *)&ss.Tool},	
		{"REAL",4007,universal_TypeInt,(void *)&ss.LoopBackAndForth},	
		{"REAL",4008,universal_TypeInt,(void *)&ss.LoopFlag},	
		{"TABL",4200,universal_TypeTable,(void *)&Global2},	
	};
	

	static struct universalTableStruct Global= {
		"Tektronix parameters",universal_WindowTEK,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};


	tekPtr s;
	int ret;
	
	s=NULL;
	if(myIcon && (myIcon->WindowType == TekType)){
		s=(tekPtr)myIcon->DWindow;
	}else{
	    myIcon=NULL;
	}

	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct TekWindow);

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
			mm.Synchronize=0;
			mm.eraseRectSize=140;
			mm.framesPerSecond=30;
			
			
			uSetRect(&ss.windowRect,117,45,575,434);
			ss.Tool=zoomTek;
			ss.LoopBackAndForth=0;
			ss.LoopFlag=0;
			
		}
		ss.defaultsSet=TRUE;
		
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;				
		if(myIcon && uPref.Files){
			doLoadImages(myIcon,uPref.Files);
		    uPref.Files=NULL;
		}			
	}else if(flag == universal_GetDefaults){		
	    if(s && myIcon){
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
	    }
	}else if(flag == universal_Stop){	
		if(freeUniversalTable(&Global))goto ErrorOut;				
	}else{
		sprintf(WarningBuff,"doUniversalTek unknown flag %d\n",flag );
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
	extern void cLineSet(int num,int dem);
	Rect PageView;
	Rect DRect;
	uRect uDRect;
	double saveScale;
	tekPtr t;
	
	if(myIcon && (t=(tekPtr)myIcon->DWindow)){
		PageView=RectConvert(PageViewi);
	    GetClip(myIcon->saveRegion);
	    ClipRect(&PageView);
		DRect.top = 0;
		DRect.left = 0;
  		DRect.right=(int)((double)t->ixmax*t->scale);
		DRect.bottom=(int)((double)t->iymax*t->scale);

		DRect.bottom = DRect.bottom/scale;
		DRect.right = DRect.right/scale;
		OffsetRect(&DRect,PageView.left,PageView.top);    
		
		saveScale=t->scale;
		t->scale /= scale;
		cLineSet(1,8);
		uDRect=uRectConvert(&DRect);
	    drawTek2(myIcon,t->CurrentFrame,&uDRect);
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
