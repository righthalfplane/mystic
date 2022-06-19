#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uFiles.h"
#include "Message.h"
#include "uLib.h"
#include "uDialog.h"
#include "SaveAllImages.h"
#include "Universal.h"
#include "uDraw.h"

static int doAntiAlias(int ixmax,int iymax,unsigned char *data);
void uWriteLineBox(uRect *LineWidthRect,int xLineWidth,int yLineWidth,IconPtr myIcon);
static int DrawGetFrameCount(IconPtr myIcon);
static int drawLine(IconPtr myIcon,uPoint *p1,uPoint *p2);

char *strpathname(char *path,char *name);
int IsTEK(char *name);
int GetRotateDirection(int *direction);
int GetFlipDirection(int *direction);
int pDoFontMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);
int pDoStyleMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);
static int DoLineDown(IconPtr myIcon);
static int DrawBackGround(IconPtr myIcon);
static void uDrawPaste(IconPtr myIcon);
static void uDrawCopy(IconPtr myIcon);
static int DrawdoBackGround(IconPtr myIcon);
static void ToArrowMode(IconPtr myIcon);
static void ToNoMode(IconPtr myIcon);
static void DrawDoAlphaIn(IconPtr myIcon,int c);
/*
static void DrawAlphaAdjust(IconPtr myIcon);
*/
static int writeCursor(uPoint pt,IconPtr myIcon);
static int DoBoxDown(IconPtr myIcon);
static int DoOvalDown(IconPtr myIcon);
static int DoRoundRectDown(IconPtr myIcon);
static int drawBox(IconPtr myIcon,uRect *box);
static int uDrawDoLineWidth(IconPtr myIcon);

int writeCursorString(uRect *dataRect,char *buffout,IconPtr myIcon);

static int DoAlphaDown(IconPtr myIcon,uPoint *wherei,int extend);
static void StartRect(uRect *r,IconPtr myIcon);
static int DrawMyControls(IconPtr myIcon);

struct FileInfo2 *FilesOpenList(struct FileList *FilesIn);

int getPaletteRGB(int n,unsigned char *r,unsigned char *g,unsigned char *b);

static int drawTek2(IconPtr myIcon,long CurrentFrame,uRect *DRect);

int freeFileList(struct FileInfo2 *Files);

static void drawIt2(IconPtr myIcon);

static void drawIt(IconPtr myIcon);

static int SetFrame(IconPtr myIcon);


int uInsetRect(uRect *r,int x,int y);

static void tMoveControls(IconPtr myIcon);
static int tInContent(IconPtr myIcon);
static int doClose(IconPtr myIcon);
static int UpdateWrite(IconPtr myIcon);
int doEraseDown(IconPtr myIcon);
static int doSelection(IconPtr myIcon,uPoint *uwhere,int clicks);
/*
static int checkStack(drawPtr t);
*/

static int CheckMenu(IconPtr myIcon,int flag);
static int Menu(IconPtr myIcon,int menu,int item);
static int doAnimateMenu(IconPtr myIcon,int item);
static int KeyBoard(IconPtr myIcon,int c);
static int n2NullEvent(IconPtr myIcon);
static void doFrameImage(IconPtr myIcon);
static void uDrawCut(IconPtr myIcon);



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
#define editUndoCommand 		1
#define editCutCommand 			3
#define editCopyCommand 		4
#define editPasteCommand 		5
#define editClearCommand		6
#define editSelectAllCommand	7
#define editDupicateCommand		9
#define editPasteLinesCommand	10
#define Information				12
#define editBackGround			13
#define editFrameCommand		14



#define Objects4				8005
#define objectToBottom 			1
#define objectDownOne 			2
#define objectToTop 			3
#define objectUpOne 			4
#define objectGroup 			5
#define objectUnGroup 			6
#define objectAnimation 		8
#define objectNewAxis 			9
#define objectNewColorBar 		10
#define objectFlip 				11
#define objectRotate 			12
#define objectPenWidth 			13

#define AnimationMenu4				8006
#define animateTekForward 			1
#define animateTekBackward 			2
#define animateTekStepForward 		3
#define animateTekStepBackward 		4
#define animateTekStop	 			5
#define loopTek	 					7
#define loopBackTek	 				8
#define optionsTek	 				9
#define synchronizeTek	 			10

#define FontCDraw 					9906	

#define StyleCDraw 					9907	

#define ToolsNewMenu				114

#define PalMenu4					8008
#define loadPaletteTek	 			1


IconPtr NewDrawWindow(IconPtr myIcon);

static int TekGetMessage(IconPtr myIcon,long MessageType,void *MessageData);
/*
static int push(uRect *oldR,IconPtr myIcon);
*/
static void tPrint(IconPtr myIcon);
int PageSetup(IconPtr myIcon);


static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect);
static void tDumpItP(IconPtr myIcon,uRect *PageView,int scale);

static int DrawAnimate(IconPtr myIcon,long CurrentFrame);

int doTestSpeed(IconPtr myIcon);

static int nMouseMoved(IconPtr myIcon,uPoint pt);

IconPtr OpenDrawFiles(struct FileInfo2 *Files);

int doUniversalDraw(IconPtr myIcon,int flag,FILE8 *inOut);

IconPtr CDrawNewLine(struct linedata *li,int count);

int uDrawdoLineGetData(IconPtr myIcon,struct linedata *li,int count);

int doLineRange(struct linedata *li);

static int SetImageRects(IconPtr myIcon);


IconPtr NewDrawWindow(IconPtr myIcon)
{
	//myIcon=myIcon;
	return OpenDrawFiles(NULL);
}

IconPtr OpenDrawFiles(struct FileInfo2 *Files)
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
	    {"Edit",  				EditMenu4,uWindowStandardMenu,0,},
	    {"Undo            /Z",	editUndoCommand,0,0,},
	    {"------------------", 	2,0,0,},
	    {"Cut             /X",	editCutCommand,0,0,},
	    {"Copy            /C",	editCopyCommand,0,0,},
	    {"Paste           /V",	editPasteCommand,0,0,},
	    {"Clear           /K",	editClearCommand,0,0,},
	    {"Select All",			editSelectAllCommand,0,0,},
	    {"------------------",	8,0,0,},
	    {"Duplicate       /D",	editDupicateCommand,0,0,},
	    {"Paste Lines       ",	editPasteLinesCommand,0,0,},
	    {"------------------",	11,0,0,},
	    {"Information...  /I",	Information,0,0,},
	    {"Frame Image     /F",  editFrameCommand,0,0,},
	    {"Options...         ",	editBackGround,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Objects[]={
	    {"Objects",  					Objects4,uWindowStandardMenu,0,},
	    {"Send To Bottom     /B", 		objectToBottom,0,0,},
	    {"Down One Layer     /-",		objectDownOne,0,0,},
	    {"Bring To Top       /T",		objectToTop,0,0,},
	    {"Up One Layer       /+",		objectUpOne,0,0,},
	    {"Group              /G",		objectGroup,0,0,},
	    {"UnGroup            /U",		objectUnGroup,0,0,},
	    {"---------------------",		7,0,0,},
	    {"Animation Flags... /A",		objectAnimation,0,0,},
	    {"New Axis",					objectNewAxis,0,0,},
	    {"ColorBar...",					objectNewColorBar,0,0,},
	    {"Flip...",						objectFlip,0,0,},
	    {"Rotate...",					objectRotate,0,0,},
	    {"Pen Width...",				objectPenWidth,0,0,},
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
	    {"Animation Options...",optionsTek,0,0,},
	    {"Synchronize",			synchronizeTek,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Fonts[]={
	    {"Fonts",  				FontCDraw,uWindowFontMenu,0,},
	    { NULL,0,0,0},
	};
	
	static struct uMenuItems Styles[]={
	    {"Style",  			StyleCDraw,uWindowStyleMenu,0,},
	    {"Plain", 			1,0,0,},
	    {"<BBold", 			2,0,0,},
	    {"<IItalic", 		3,0,0,},
	    {"<UUnderline", 	4,0,0,},
	    {"<OOutline", 		5,0,0,},
	    {"<SShadow", 		6,0,0,},
	    {"-----------", 	7,0,0,},
	    {"9 Point", 		8,0,0,},
	    {"10 Point", 		9,0,0,},
	    {"12 Point", 		10,0,0,},
	    {"14 Point", 		11,0,0,},
	    {"24 Point", 		12,0,0,},
	    {"36 Point", 		13,0,0,},
	    {"42 Point", 		14,0,0,},
	    {"48 Point", 		15,0,0,},
	    {"54 Point", 		16,0,0,},
	    {"72 Point", 		17,0,0,},
	    {"Other...", 		18,0,0,},
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
	    {Objects},
	    {Animation},
	    {Fonts},
	    {Styles},
	    {Palette},
	    {NULL}
	};
	
	uRect Rectl={40,40,400,200};
	
	    struct DrawWindow *t;
		DOListPtr l;
	    IconPtr myIcon;
		static int Count;
		char buff[256];
		
		t=NULL;
		
		if(!(myIcon=uCreateIcon(DrawNewType,&Rectl))){
			WarningBatch("OpenDrawFiles uCreateIcon Failed\n");
			goto ErrorOut;
		}
		
		sprintf(buff,"DrawWindow %03d",Count++);
		
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
		
		myIcon->uDraw=drawTek2;
		
		myIcon->paletteMenuNumber=PalMenu4;
		
		myIcon->pPageUp=tdoPageUp;
		myIcon->pPageDown=tdoPageDown;
		myIcon->pLineUp=tgoUp;
		myIcon->pLineDown=tgoDown;
		myIcon->pThumb=tdothumb;
		
		myIcon->uWriteCursor=writeCursor;
		
		myIcon->mMenuOffSet=MenuOffSet;
		
		if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,
	              uWindowSetDone,uWindowSetDone)
		)goto ErrorOut;
	    
	    if(!(t=(struct DrawWindow *)cMalloc((long)sizeof(struct DrawWindow),8183))){
	        WarningBatch("OpenDrawFiles Out of List Memory\n");
			goto ErrorOut;
	    }
	    zerol((char *)t,sizeof(struct DrawWindow));
	    
	    myIcon->DWindow=(struct DumWindow *)t;
	    
	    l=&t->DList;
/*
		l->Patterns=Patterns;
*/
		l->xsize=720;
		l->ysize=480;
		l->dpi=72;
		l->raster=RASTER_24;

		zerol((char *)&l->Attributes,(long)sizeof(l->Attributes));

		l->Attributes.font.txFont=DefaultFont;
		l->Attributes.font.txFace=0;
		l->Attributes.font.txSize=DefaultFontSize;

	 	l->Attributes.LinePattern=0;
	 	l->Attributes.LineSymbol=0;
	 	l->Attributes.SolidPattern=19;
		l->Attributes.nLineFore=255;
		l->Attributes.nLineBack=0;
		l->Attributes.nSolidFore=0;
		l->Attributes.nSolidBack=0;
	 	l->Attributes.xLineWidth=1;
	 	l->Attributes.yLineWidth=1;
	 	l->Attributes.DashFlag=FALSE;
	 	mstrncpy(l->Attributes.LineDash,"9",16);	 	
	 	mstrncpy(l->Attributes.AnimationFormat,"%.0f\0",10);
	 	
	 	mstrncpy(l->LinePatterns[0],"9",16);
		mstrncpy(l->LinePatterns[1],"0",16);
	 	mstrncpy(l->LinePatterns[2],"34",16);
	 	mstrncpy(l->LinePatterns[3],"56",16);
	 	mstrncpy(l->LinePatterns[4],"78",16);
	 	mstrncpy(l->LinePatterns[5],"145541",16);
	 	mstrncpy(l->LinePatterns[6],"1222",16);
	  	mstrncpy(l->LinePatterns[7],"1112",16);
	 	l->NumberOfPatterns=8;
	 	
	 	

		l->nLineFore=255;
		
		mstrncpy(l->palname,"ps",3);
		getPaletteByName(l->palname,(unsigned char *)l->palette);
		
		l->red=255;
		l->green=255;
		l->blue=255;
		
		l->myIcon=myIcon;
		l->s.myIcon=myIcon;

		
		DOSetDrawCanvas(l);
	    
		t->ixmax=(int)l->xsize;
		t->iymax=(int)l->ysize;
		t->scale=1.0;
		
		t->BackGround=TRUE;
		t->GridLines=FALSE;
		t->GridDelta=72;
		t->AntiAlias=FALSE;
		
	 	t->SnapTo=FALSE;
	 	t->vSnapTo=0.125;
		
		myIcon->framesPerSecond=30;
		t->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);

	    if(Files){
	        myIcon->Files=Files;
		    t->ImageCount=Files->ImageCount;
		    t->length=Files->length;
		}
		
		t->CurrentFrame=0;
		t->ActiveControl=ControlArrow;
		
		myIcon->ControlSpace=26;
		myIcon->CursorSpace=16;
		myIcon->eraseRectSize=0;

		uSetForeByIndex(255,myIcon);

		uSetBackByIndex(0,myIcon);		
		
		if(SetImageRects(myIcon))goto ErrorOut;
				
		
	    Files=NULL;
	    
		if(ScreenSetSize(Rectl.xsize,Rectl.ysize,myIcon))goto ErrorOut;
	    
	    
	    if(!IsShift()){
	    	doUniversalDraw(myIcon,universal_GetDefaults,NULL);
	    	FlushCurrentPort();
	    }
	    
		SetFrame(myIcon);
		doFrameImage(myIcon);

		getPaletteRGB(12,myIcon->red,myIcon->green,myIcon->blue);
		
		
		ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
    	
    	
		uSetTextFace(myIcon,0);
		
		uSetTextFont(myIcon,DefaultFont);
		
		uSetFontSize(myIcon,DefaultFontSize);
		
    	SetWindowToFileName(myIcon,myIcon->Files);
    	
    	StandardDoActive(myIcon,TRUE);
    	
	    CheckMenu(myIcon,TRUE);
	    
		GetPaletteByName("Ps",myIcon->red,myIcon->green,myIcon->blue);
		SetDefaultPaletteName(myIcon,"Ps");
	    
	    tMoveControls(myIcon);
	    
	    InvalRectMyWindow(myIcon);

		if(myIcon)myIcon->isOpen=TRUE;

	    return myIcon;
ErrorOut:
		if(Files)freeFileList(Files);
		if(myIcon)doClose(myIcon);
		return NULL;
}

int uDrawdoLineGetData(IconPtr myIcon,struct linedata *li,int count)
{
	uGridPlotPtr Plot;
	uLineDataPtr Line;
	DOListPtr l;
	drawPtr t;
	int ret;
	int n;
	
	if(!myIcon || !li)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	l=&t->DList;
	
	ret = 1;
	

	Plot=DOGridPlotCreate(l);
	if(!Plot)goto ErrorOut;
	
	if(Plot->DeSelectOne){
	   (*Plot->DeSelectOne)((DObjPtr)Plot,NULL);
	   Plot->Selected=TRUE;
	   Plot->lSelected=count;
	}
	
	for(n=0;n<count;++n){
		Line=DOLineDataCreate(&li[n],&l->Attributes);
		if(!Line)goto ErrorOut;
		if(GridPlotAdd(l,Plot->name,Line))goto ErrorOut;	
		DOLineAttributes(l,Line,n);
		t->CurrentFrame=li[n].CurrentFrame;
	}
	t->ImageCount=DOFrameCount(l);
	
	if(DODraw(l,t->CurrentFrame))goto ErrorOut;

	ret = 0;
ErrorOut:
	return ret;
}

IconPtr CDrawNewLine(struct linedata *li,int count)
{
	IconPtr myIcon;
	int ret;

	if(!li)return NULL;
	
	ret=1;
	
		
	if(doLineRange(li))return NULL;
	
	myIcon=NewDrawWindow(NULL);
	if(!myIcon)goto ErrorOut;
	
	if(uDrawdoLineGetData(myIcon,li,count))goto ErrorOut;
	 
	ret=0;
		     
ErrorOut:
	if(ret && myIcon){
	    doClose(myIcon);
	    myIcon=NULL;	
	}	 

	return myIcon;
}
static int writeCursor(uPoint pt,IconPtr myIcon)
{
	char buffout[512];
	drawPtr t;
	rPoint p;
	
	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	
	buffout[0]=0;
	
	uDrawAdjustReal(&pt,myIcon);
	
	p=screenToLocal(&pt,&t->DList);
	
	sprintf(buffout," X %10.4g# Y %10.4g# Step %ld#",p.x,p.y,t->CurrentFrame);
	
	writeCursorString(&myIcon->dataRect,buffout,myIcon);
	
  	return 0;

}
static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
	drawPtr t;

	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {		
	    if(t->ActiveControl == ControlAlpha){
	       uSetCursor(myIcon,uBEAM_CURSOR);
	    }else if(t->ActiveControl == ControlLine || t->ActiveControl == ControlBox ||
	             t->ActiveControl == ControlOval || t->ActiveControl == ControlRoundRect){
	       uSetCursor(myIcon,uPLUS_CURSOR);
	    }else{
	        if(DOStretchRgn(pt,&t->DList)){
	            uSetCursor(myIcon,uPLUS_CURSOR);
	        }else {
	            uSetCursor(myIcon,uARROW_CURSOR);
	        }
	    }
	    writeCursor(pt,myIcon);
	}else{
         uSetCursor(myIcon,uARROW_CURSOR);
         writeCursorString(&myIcon->dataRect," ",myIcon);
	}		
	
	return 0;	
}
static int TekGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct Message1000 *m;
	char windowName[256];
	uPort save;
	drawPtr s;
	int ret;

	if(!myIcon)return 1;
	s=(drawPtr)myIcon->DWindow;
	if(!s)return 1;
	m=(struct Message1000 *)MessageData;

	ret=1;
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	switch(MessageType){
	case MessageType_SET_FRAME_NUMBER:
	    if(!myIcon->Synchronize)goto OK;

	    s->CurrentFrame=m->CurrentFrame;
	    SetFrame(myIcon);
		break;
	case MessageType_GET_FRAME_COUNT:
	    if(DOListGetMessage(&s->DList,MessageType,MessageData))goto ErrorOut;
	    goto OK;
	case MessageType_GET_LINE_DATA:
	    goto ErrorOut;
	case MessageType_GET_FILE_LIST:
	    if(DOListGetMessage(&s->DList,MessageType,MessageData))goto ErrorOut;
	    goto OK;
	case MessageType_GET_FILE_OWNER:
	    if(DOListGetMessage(&s->DList,MessageType,MessageData))goto ErrorOut;
	    goto OK;
	case MessageType_LIMITS:
	    goto OK;	    
	case MessageType_PRINT_FILE_INFORMATION:
		GetWindowName(myIcon,windowName,0);
	    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
	    WarningBatch(WarningBuff);
	    if(DOListGetMessage(&s->DList,MessageType,MessageData))goto ErrorOut;
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
      /*  0 */      {"Ok",      {40,95,80,23},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {240,95,80,23},uDialogButton,uDialogNone},
      /*  2 */      {"Frames/sec data", {160,45,120,25},uDialogText,uDialogGetDouble},
      /*  3 */      {"Frames/sec :",     {35,45,105,25},uDialogString,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rectl   = {100,50,339,151};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	drawPtr t;
	int reti;
    int ret;
		
	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
		
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Animation Options",&Rectl,ItemCount);
	
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
	
	t->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);
     
    ret = 0;
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int n2NullEvent(IconPtr myIcon)
{
	drawPtr t;


	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;

	if(t->ActiveControl == ControlAlpha){
	    uEditDrawCursor(t->DList.Edit);	
	    return 0;
	}	

	if(!(t->AnimationOn) || (t->ImageCount <= 1))return 1;
	    
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
       t->TimeToFlip = TickCount() + t->milliSecondsPerFrame*60/1000;
       
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
	drawPtr t;
	
	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	if(c <= 0)return 0;
	
	
	if(t->ActiveControl == ControlAlpha){
	    DrawDoAlphaIn(myIcon,c);	
	    return 0;
	}	
	
	if(c == 't'){
		DrawGetFrameCount(myIcon);
		myIcon->Files=&t->Files;
	 	doTestSpeed(myIcon);	
		myIcon->Files=NULL;
 		return 0;
	}
	
	if(t->DList.stretchCount){
	
		if((c == leftArrow) || (c == rightArrow) ||(c == upArrow) ||(c == downArrow)){
			 DOListdoKeyMove(&t->DList,c);
	 		InvalRectMyWindow(myIcon);
		}else if(c == DEL || c == 127){
			DOListKillSelected(&t->DList);
	 		InvalRectMyWindow(myIcon);
	 	}
		return 0;
	}

	t->ImageCount=DOFrameCount(&t->DList);

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
	   default:
	        doAnimateMenu(myIcon,animateTekStop);
	        break;
	}
	return 0;
}
static int doAnimateMenu(IconPtr myIcon,int item)
{
	drawPtr t;


	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
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
	        if(t->AnimationOn)InvalRectMyWindow(myIcon);
	        t->AnimationOn = 0;
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
int uDrawSaveScrap(IconPtr myIcon)
{
	uRect Box;
	drawPtr t;

	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
		
		
  	if(!t->ImageCount)return 1;
  	
  	Box.x = 0;
  	Box.y = 0;
  	Box.xsize=(int)((double)t->ixmax*t->scale);
  	Box.ysize=(int)((double)t->iymax*t->scale);
  	
  	
	return drawToScrap(myIcon,&Box,t->CurrentFrame,0);

}
static int DrawGetFrameCount(IconPtr myIcon)
{
	struct Message1000 m;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	
	m.FrameCount=0;
	
	w->Files.ImageCount=1;
	
    DOListGetMessage(&w->DList,MessageType_GET_FRAME_COUNT,&m);
    
	w->Files.ImageCount=m.FrameCount;
	
	return 0;
}
static int Menu(IconPtr myIcon,int menu,int item)
{
	static int FlipDirection;
	static int RotateDirection;
	uRect Box;
	drawPtr t;
	int k;

	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;

	DrawGetFrameCount(myIcon);

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
		switch(item){		
		case closeTekCommand:
		    if(myIcon && myIcon->pClose){
		        (*myIcon->pClose)(myIcon);
		    }
		    myIcon=NULL;
 		    break;
		case savePreferencesCommand:
			DOListUpdate(&t->DList,1);
	    	writeUniversal(myIcon,0);
 		    break;
		case PreferencesCommand:
			DOListUpdate(&t->DList,1);
			saveUniversal(myIcon);
 		    break;
		case saveTekCommand:
			doSavePlots(myIcon,&t->Files,t->CurrentFrame,&Box);
 		    break;
		case loadTekCommand:
		    DOListdoLoadImages(&t->DList);
 		    break;
		case previewTekCommand:
			PagePreview(myIcon);
 		    break;
		case setupTekCommand:
			PageSetup(myIcon);
 		    break;
		case printTekCommand:
		    Print(myIcon);
 		    break;
		case openTekCommand:
		    NewNewWindow(myIcon);
 		    break;
		case quitTekCommand:
		   doQuit();
		   myIcon=NULL;
 		   break;
		}				    
		break;	
		
	case Objects4:
		switch(item){		
		case objectToBottom:
		    DOListSendBack(&t->DList);
 		    break;
		case objectDownOne:
		    DOListSendDown(&t->DList);
 		    break;
		case objectToTop:
		    DOListSendFront(&t->DList);
 		    break;
		case objectUpOne:
		    DOListSendUp(&t->DList);
 		    break;
		case objectGroup:
			DOListGroup(&t->DList); 
 		    break;
		case objectUnGroup:
			DOListUnGroup(&t->DList); 
 		    break;
		case objectAnimation:
		   DOListSetAnimation(&t->DList);
 		    break;
		case objectNewAxis:
			DOListDoNewPlot(&t->DList);
 		    break;
		case objectNewColorBar:
		    DOListColorBar(&t->DList);
 		    break;
		case objectFlip:
 			if(GetFlipDirection(&FlipDirection))goto ErrorOut;		
 			DOListDoFlipDirection(&t->DList,FlipDirection);
 		    break;
		case objectRotate:
 			if(GetRotateDirection(&RotateDirection))goto ErrorOut;		
 			DOListDoRotateDirection(&t->DList,RotateDirection);
 		    break;
		case objectPenWidth:
			uDrawDoLineWidth(myIcon);
 		    break;
		}				    
		break;	
		
				
	case EditMenu4:
		
		switch(item){		
		case editUndoCommand:
		  if(myIcon->pUndo){
				(*myIcon->pUndo)(&t->DList);
		   }
			break;
 		case Information:
 			if(t->DList.stretchCount){
 			    DOListDoInformation(&t->DList);
 			}else{
 		    	doWindowInformation(myIcon);tMoveControls(myIcon);
 		   	}
 		    break;
 		    
 		case editClearCommand:
 		    DOListClear(&t->DList);
 		    break;
 		    
 		case editDupicateCommand:
 		    DOListDuplicate(&t->DList);
 		    break;
 		    
 		case editSelectAllCommand:
 		    DOListSelectAll(&t->DList);
 		    break;
 		    
 		case editCutCommand:
 		    uDrawCut(myIcon);
 		    break;

 		case editBackGround:
 		    DrawdoBackGround(myIcon);
 		    break;
 		    
 		case editFrameCommand:
 		    doFrameImage(myIcon);
 		    break;

 		case editCopyCommand:
 		    uDrawCopy(myIcon);
	    	InvalRectMyWindow(myIcon);
		    break; 		    

 		case editPasteCommand:
 		    uDrawPaste(myIcon);
		    break; 		    
		    
 		case editPasteLinesCommand:
 		    DOListPasteLines(&t->DList);
		    break; 		    	    
 		}		
		break;
	case AnimationMenu4:
		doAnimateMenu(myIcon,item);
		break;
	case WindowMenu:
		pWindowMenu(myIcon,item);
		break;
	case PalMenu4:
		CWindowdoPalMenu(myIcon,menu,item);
		for(k=0;k<256;++k){
	    	t->DList.palette[k*3]=myIcon->red[k];
	     	t->DList.palette[k*3+1]=myIcon->green[k];
	     	t->DList.palette[k*3+2]=myIcon->blue[k];
		}		
		DOSetPalette(&t->DList);
		break;
	case FontCDraw:
		pDoFontMenu(myIcon,FontCDraw,StyleCDraw,item);
		DOListChangeAlpha(&t->DList);
		break;
	case StyleCDraw:
		pDoStyleMenu(myIcon,FontCDraw,StyleCDraw,item);
		DOListChangeAlpha(&t->DList);
		break;
	}
ErrorOut:
	if(FindWindowInList(myIcon) && t){
	    t->ImageCount=DOFrameCount(&t->DList);
	}
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int uDrawDoLineWidth(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {50,100,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {160,100,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"Edit Text", {49,67,75,16},uDialogText,uDialogGetDouble},
      /*  3 */      {"Edit Text", {157,67,75,16},uDialogText,uDialogGetDouble},
      /*  4 */      {"x Width", {58,38,75,16},uDialogString,uDialogNone},
      /*  5 */      {"y Width", {155,38,75,16},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {79,54,269,146};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
	drawPtr w;
	struct uAttributes *ap;
	struct uAttributes vAttributes;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	ap=DOListGetAttributes(&w->DList);
	if(!ap)return 1;
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Pen Width",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)ap->xLineWidth,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)ap->yLineWidth,
	              	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
		
    vAttributes.xLineWidth=uR[2].dreturn;
    vAttributes.yLineWidth=uR[3].dreturn;
    DOListSaveAttributes(&w->DList,&vAttributes,lWidth);
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
}
static void uDrawPaste(IconPtr myIcon)
{
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
/*	
	if(w->textH){
	    TEFromScrap();TEPaste(w->textH); 
 	}else{
	    COListPaste(w->OList);
 	}
 */
		DOListPaste(&w->DList);
}
static void uDrawCopy(IconPtr myIcon)
{
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
/*	
	if(w->textH){
 	    TECopy(w->textH);ZeroScrap();TEToScrap();
 	}else{
	    COListCopy(w->OList);
 	}
 */
	if(w->DList.stretchCount){
		DOListCopy(&w->DList);
	}else{
		uZeroScrap();
		uDrawSaveScrap(myIcon);
	}
 
}
static void uDrawCut(IconPtr myIcon)
{
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
/*	
	if(w->textH){
 	    TECopy(w->textH);ZeroScrap();TEToScrap();TECut(w->textH);
 	}else{
	    COListCut(w->OList);
 	}
*/

	    DOListCut(&w->DList);
}
static int DrawdoBackGround(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {260,220,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {360,220,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"BackGround", {30,25,130,20},uDialogCheck,uDialogGetLong},
      /*  3 */      {"Grid Lines", {255,25,125,20},uDialogCheck,uDialogGetLong},
      /*  4 */      {"x value", {125,60,95,20},uDialogText,uDialogGetLong},
      /*  5 */      {"y value", {125,95,95,20},uDialogText,uDialogGetLong},
      /*  6 */      {"d value", {345,60,95,20},uDialogText,uDialogGetLong},
      /*  7 */      {"Grid Snap To", {255,88,125,20},uDialogCheck,uDialogGetLong},
      /*  8 */      {"d value", {345,117,95,20},uDialogText,uDialogGetDouble},
      /*  9 */      {"dx value", {125,130,95,20},uDialogText,uDialogGetLong},
      /* 10 */      {"dy value", {125,165,95,20},uDialogText,uDialogGetLong},
      /* 11 */      {"x Size", {30,60,60,20},uDialogString,uDialogNone},
      /* 12 */      {"y Size", {30,95,60,20},uDialogString,uDialogNone},
      /* 13 */      {"Delta", {255,60,65,20},uDialogString,uDialogNone},
      /* 14 */      {"Delta", {255,118,65,20},uDialogString,uDialogNone},
      /* 15 */      {"d Width", {30,130,60,20},uDialogString,uDialogNone},
      /* 16 */      {"d Height", {30,165,60,20},uDialogString,uDialogNone},
      /* 17 */      {"Scale", {255,165,60,20},uDialogString,uDialogNone},
      /* 18 */      {"d value", {345,165,95,20},uDialogText,uDialogGetDouble},
      /* 19 */      {"Anti Alias", {30,200,130,20},uDialogCheck,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,475,260};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	int xsize,ysize;
	int AntiAlias;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
		
    xsize=0;
    ysize=0;
	
	AntiAlias=w->AntiAlias;
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,	
	
	              uDialogSetLogical,w->BackGround,
	             
	              uDialogSetItem,3L,
	              uDialogSetLogical,w->GridLines,
	
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)w->ixmax,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)w->iymax,

	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)w->GridDelta,
	 	              
	              uDialogSetItem,7L,
	              uDialogSetLogical,w->SnapTo,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)w->vSnapTo,
	 
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)xsize,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)ysize,
	              
	              uDialogSetItem,18L,
	              uDialogSetDouble,(double)w->scale,
	              
	              
	              uDialogSetItem,19L,
	              uDialogSetLogical,w->AntiAlias,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;	
	
	

	if(uR[4].lreturn <= 0){
	     Warning("ixmin <= 0");
	     goto Start;
	}
	
	if(uR[5].lreturn <= 0){
	     Warning("iymin <= 0");
	     goto Start;
	}
	
	if(uR[18].dreturn <= 0){
	     Warning("Scale <= 0");
	     goto Start;
	}
	
	w->GridLines=(int)uR[3].lreturn;
	w->GridDelta=(int)uR[6].lreturn;
	w->BackGround=(int)uR[2].lreturn;
	w->SnapTo=(int)uR[7].lreturn;
	w->vSnapTo=uR[8].dreturn;
	w->scale=uR[18].dreturn;
	w->AntiAlias=(int)uR[19].lreturn;
	
	if((uR[4].lreturn != w->ixmax) || (uR[5].lreturn != w->iymax) || (w->AntiAlias != AntiAlias)){
		if(w->AntiAlias != AntiAlias){
			if(w->AntiAlias){
				w->scale=0.5*w->scale;
				uR[4].lreturn *= 2;
				uR[5].lreturn *= 2;
			}else{
				w->scale=w->scale*2.0;
				uR[4].lreturn /= 2;
				uR[5].lreturn /= 2;
			}
		}
	    w->ixmax=(int)uR[4].lreturn;
	    w->iymax=(int)uR[5].lreturn;
		w->DList.xsize=w->ixmax;
		w->DList.ysize=w->iymax;
	    DOSetDrawCanvas(&w->DList);
	}
	tMoveControls(myIcon);	
 	InvalRectMyWindow(myIcon);

	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);

	return ret;
}
static int CheckMenu(IconPtr myIcon,int flag)
{
	DOListPtr DList;
	drawPtr t;
	int oflag,any;
	long n;


	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;	    
	DList=&t->DList;

	if(flag){
		
		
		oflag=FALSE;

		any=FALSE;
		
	    if(DList->oCount){
	        for(n=0;n<DList->oCount;++n){
		    DObjPtr Current;
	            if((Current=DList->oCurrent[n]) != NULL){
					any=TRUE;
	                if(Current->Selected){
	    		    	oflag=TRUE;
	    		    	break;
	    			}
	            }
	        }
	    }
	    
	    if(any){
			uActivateMenuItem(myIcon,EditMenu4,editCopyCommand,TRUE);
			uActivateMenuItem(myIcon,FileMenu4,previewTekCommand,isInstalled(Installed_PrintPreview));
			uActivateMenuItem(myIcon,FileMenu4,printTekCommand,isInstalled(Installed_Print));
			uActivateMenuItem(myIcon,FileMenu4,saveTekCommand,TRUE);
		}else{
			uActivateMenuItem(myIcon,EditMenu4,editCopyCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,previewTekCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,printTekCommand,FALSE);
			uActivateMenuItem(myIcon,FileMenu4,saveTekCommand,FALSE);
		}	

	    if(myIcon->pUndo){
	        uSetMenuItemText(myIcon,EditMenu4,editUndoCommand,(unsigned char *)myIcon->cUndo);
	        uEnableMenuItem(myIcon,EditMenu4,editUndoCommand);
	    }else{
	        uDisableMenuItem(myIcon,EditMenu4,editUndoCommand);
	    }

		if(oflag){
			uActivateMenuItem(myIcon,EditMenu4,editCutCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,editClearCommand,TRUE);
			uActivateMenuItem(myIcon,EditMenu4,editDupicateCommand,TRUE);		
			
	        uEnableMenuItem(myIcon,Objects4,objectToBottom);
	        uEnableMenuItem(myIcon,Objects4,objectDownOne);
	        uEnableMenuItem(myIcon,Objects4,objectToTop);
	        uEnableMenuItem(myIcon,Objects4,objectUpOne);
	        uEnableMenuItem(myIcon,Objects4,objectGroup);
	        uEnableMenuItem(myIcon,Objects4,objectUnGroup);
	        uEnableMenuItem(myIcon,Objects4,objectAnimation);
	        uEnableMenuItem(myIcon,Objects4,objectFlip);
	        uEnableMenuItem(myIcon,Objects4,objectRotate);
	        uEnableMenuItem(myIcon,Objects4,objectPenWidth);										
		}else{
			uActivateMenuItem(myIcon,EditMenu4,editCutCommand,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,editClearCommand,FALSE);
			uActivateMenuItem(myIcon,EditMenu4,editDupicateCommand,FALSE);
			
	        uDisableMenuItem(myIcon,Objects4,objectToBottom);
	        uDisableMenuItem(myIcon,Objects4,objectDownOne);
	        uDisableMenuItem(myIcon,Objects4,objectToTop);
	        uDisableMenuItem(myIcon,Objects4,objectUpOne);
	        uDisableMenuItem(myIcon,Objects4,objectGroup);
	        uDisableMenuItem(myIcon,Objects4,objectUnGroup);
	        uDisableMenuItem(myIcon,Objects4,objectAnimation);
	        uDisableMenuItem(myIcon,Objects4,objectFlip);
	        uDisableMenuItem(myIcon,Objects4,objectRotate);
	        uDisableMenuItem(myIcon,Objects4,objectPenWidth);
		}
		
		
		if(t->ImageCount){
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekForward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekBackward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepForward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStepBackward,TRUE);
			uActivateMenuItem(myIcon,AnimationMenu4,animateTekStop,TRUE);
		}else{
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
	    	    		
		DOListSetFontSelected(&t->DList);
		
		uSetFontMenus(FontCDraw,StyleCDraw,myIcon);

	    CWindowSetPalMenu(myIcon);
	    	    
	    doWindowMenu();
	}
	return 0;
}
static void ToArrowMode(IconPtr myIcon)
{
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	
	if(!w->ActiveLocked){
	    ToNoMode(myIcon);
	    w->ActiveControl = ControlArrow;
	    DrawMyControls(myIcon);
	}
}
static void ToNoMode(IconPtr myIcon)
{
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	
	if(w->ActiveControl == ControlAlpha){
	    DOListSaveAlpha(&w->DList,(DObjPtr)w->DList.Alpha);
	} 
	
}
static void DrawDoAlphaIn(IconPtr myIcon,int c)
{
	drawPtr w;
	DOListPtr l;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	l=&w->DList;
	if(!l)return;
	if(!l->Edit)return;
	
	if(c == '\n' || c == '\r'){
	    l->Edit->box.ysize += l->Edit->Info.LineHeight;
	}
	uEditKey(c,l->Edit);
	
	/* DrawAlphaAdjust(myIcon); */
}
/*
static void DrawAlphaAdjust(IconPtr myIcon)
{
	uEditPtr Edit;
	char *text;
	int length,n;
	int c,Lines,Width,TWidth;
	int right,bottom;
	drawPtr w;
	DOListPtr l;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
	l=&w->DList;
	if(!l)return;
	Edit=l->Edit;
	if(!Edit)return;
	
	text=Edit->text;
	length=Edit->length;
	Lines=0;
	Width=0;
	TWidth=0;
	for(n=0;n<length;++n){
	    c=text[n];
	    if(c == '\r' || c == '\n'){
		if(TWidth > Width)Width = TWidth;
		TWidth=0;
		++Lines;
	    }else{
	        TWidth += Edit->Info.CWidth;
	    }
	}
	bottom =  Lines*Edit->Info.LineHeight;
	if(bottom > Edit->box.ysize){
	    Edit->box.ysize=bottom;
	}

	if(TWidth > Width)Width = TWidth;
	right =  Width+2*Edit->Info.CWidth;
	if(right > Edit->box.xsize){
	    Edit->box.xsize=right;
	}

}
*/
static int DoAlphaDown(IconPtr myIcon,uPoint *where,int extend)
{
	uAlphaPtr AlphaOld;
	static double dx,dy;
	DObjPtr Object;
	drawPtr w;
	DOListPtr l;
	rPoint pt;
	uRect ri;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	l=&w->DList;
	if(!l)return 1;
	
	//extend=extend;
	
	if(l->Edit){
	    if(uPtInRect(*where,&(l->Edit->box))){
			uEditClick(where,extend,l->Edit);
	        return 0;
	    } else if(w->ActiveLocked){
	        DOListSaveAlpha(l,(DObjPtr)l->Alpha);
	    } else {
	        ToArrowMode(myIcon);
	        return 0;
	    }
	}
	

	pt=screenToLocal(where,l);
		
	l->AlphaNumber++;

	DOListDeSelectAll(l);
		
	if(((Object = DOListFindObject(&pt,l)) != NULL) &&
	     (Object->type == AlphaType)){
	        AlphaOld=(uAlphaPtr)Object;
	        AlphaOld->Selected=TRUE;
	        AlphaOld->AlphaNumber=l->AlphaNumber;
			uSetFontStuff(&AlphaOld->Attributes.font,myIcon);
			ri=DOuRectFromLocal(&AlphaOld->box,l);
	        l->Edit=uEditNew(&ri,l->myIcon);
	        if(l->Edit){
		    	uEditInsert(AlphaOld->save,(int)AlphaOld->n,l->Edit);
		    	uEditClick(where,extend,l->Edit);
	        }
	    
	        l->Alpha = (uAlphaPtr)uAlphaMove((DObjPtr)AlphaOld,&dx,&dy);
	        if(l->Alpha == NULL){
	            return 0;
	        }
	}else{
	     DrawSetEditAlpha(l,where);
	}
	return 0;
}
int DrawSetEditAlpha(DOListPtr l,uPoint *where)
{
	uFontInfo Info;
	uRect ri;
		
	if(!l)return 1;
		
    l->Alpha = uAlphaAlpha(&l->Attributes,l);
    if(l->Alpha == NULL){
        return 0;
    }
    l->Alpha->AlphaNumber=l->AlphaNumber;


	uSetFontStuff(&l->Alpha->Attributes.font,l->myIcon);

    uGetFontInfo(l->myIcon,&Info);
    
    where->y -= Info.LineHeight/2;
	uDrawAdjustReal(where,l->myIcon);	    
    ri.x=where->x;
    ri.y=where->y;
    ri.ysize = Info.LineHeight;
    ri.xsize = Info.CWidth;
    
    l->Edit=uEditNew(&ri,l->myIcon);
    if(l->Edit){
    /*
		TEActivate(w->textH);
		(*w->textH)->crOnly = -1;
	*/
    }

	l->AlphaNumber++;
	    
	return 0;
}
static int tInContent(IconPtr myIcon)
{
	static uPoint LastPoint;
	drawPtr t;
	uPoint uwhere;
	int fActiveLocked;
	int extend;
	uRect cBox;
	
	
	if(!myIcon || !(t=(drawPtr)myIcon->DWindow))return 1;

	cBox=myIcon->eraseRect;
	cBox.x = cBox.x+cBox.xsize-4;
	cBox.xsize = 4;
	cBox.ysize = 4;

	uPointer(&uwhere,myIcon);

    if((TickCount() < t->LastClick+120) &&
        (uwhere.x == LastPoint.x)     && 
        (uwhere.y == LastPoint.y)){	/* Double Click  */
		fActiveLocked = TRUE;
    } else {				/* Single Click  */
		fActiveLocked = FALSE;
    }
    LastPoint=uwhere;


	if(uPtInRect(uwhere,&myIcon->uviewRect)){
	    if(t->ActiveControl == ControlAlpha){
			extend=(IsShift()) ? TRUE : FALSE;	    	    
			DoAlphaDown(myIcon,&uwhere,extend);	    
	    }else if(t->ActiveControl == ControlLine){	   
			DoLineDown(myIcon);	    
	    }else if(t->ActiveControl == ControlBox){	   
			DoBoxDown(myIcon);	    
	    }else if(t->ActiveControl == ControlOval){	   
			DoOvalDown(myIcon);	    
	    }else if(t->ActiveControl == ControlRoundRect){	   
			DoRoundRectDown(myIcon);	    
	    }else{	   
	        doSelection(myIcon,&uwhere,fActiveLocked);
	    }
		if(t->AnimationOn){	  
			t->AnimationOn=FALSE;
			InvalRectMyWindow(myIcon);
		}		      
	    t->LastClick=TickCount();
	} else if(uPtInRect(uwhere,&t->ArrowRect)){
		ToNoMode(myIcon);
		t->ActiveControl = ControlArrow;
		t->ActiveLocked = fActiveLocked;
		DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->TextRect)){
        ToNoMode(myIcon);
		t->ActiveControl = ControlAlpha;
		t->ActiveLocked = fActiveLocked;
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->LineRect)){
        ToNoMode(myIcon);
		t->ActiveControl = ControlLine;
		t->ActiveLocked = fActiveLocked;
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->BoxRect)){
        ToNoMode(myIcon);
		t->ActiveControl = ControlBox;
		t->ActiveLocked = fActiveLocked;
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->OvalRect)){
        ToNoMode(myIcon);
		t->ActiveControl = ControlOval;
		t->ActiveLocked = fActiveLocked;
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->RoundRectRect)){
        ToNoMode(myIcon);
		t->ActiveControl = ControlRoundRect;
		t->ActiveLocked = fActiveLocked;
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->LineForeRect)){
        uDrawcPopUp(myIcon,lForeColor);
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->SolidForeRect)){
        uDrawcPopUp(myIcon,sForeColor);
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->LineBackRect)){
        uDrawcPopUp(myIcon,lBackColor);
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->SolidBackRect)){
        uDrawcPopUp(myIcon,sBackColor);
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->LineWidthRect)){
        uDrawwPopUp(myIcon,lWidth);
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->LinePatternRect)){
        uDrawpPopUp(myIcon,lPattern);
        DrawMyControls(myIcon);
    } else if(uPtInRect(uwhere,&t->SolidPatternRect)){
        uDrawpPopUp(myIcon,sPattern);
        DrawMyControls(myIcon);
	} else if(uPtInRect(uwhere,&cBox)){
	    doEraseDown(myIcon);
	} else {
	   uDoControls(myIcon);
	   t->LastClick=0;
	   return 0;
	}
	t->LastClick=TickCount();
	return 0;
}
static int drawLine(IconPtr myIcon,uPoint *p1,uPoint *p2)
{
	uPoint po;
	int start;
	
	if(!myIcon || !p1 || !p2 )return 1;
		
	uPointer(p1,myIcon);
	uDrawAdjustReal(p1,myIcon);
	start=TRUE;
	po = *p1;
	while(uStillDown(myIcon)){
		uPointer(p2,myIcon);
		uDrawAdjustReal(p2,myIcon);
		uSetPenXOR(TRUE,myIcon);
	    if(po.x != p2->x || po.y != p2->y){
	        if(!start){
		        uMoveTo(p1->x,p1->y);
		        uLineTo(po.x,po.y,myIcon);
	        } 
		    po = *p2;
		    uMoveTo(p1->x,p1->y);
		    uLineTo(po.x,po.y,myIcon);
		    
			uSetPenXOR(FALSE,myIcon);
			if(myIcon->uWriteCursor){
				(*myIcon->uWriteCursor)(po,myIcon);
			}
		    
		    start=FALSE;
	    }
	}
	if(!start){
		uSetPenXOR(TRUE,myIcon);
	    uMoveTo(p1->x,p1->y);
	    uLineTo(po.x,po.y,myIcon);
	}

	uSetPenXOR(FALSE,myIcon);
	
	if(start)return 1;

	return 0;
}
static int drawBox(IconPtr myIcon,uRect *box)
{
	uPoint start,current,last;
	int isDrawn;
	uRect oldR;
	
	if(!myIcon || !box)return 1;
	
	uSetPenXOR(TRUE,myIcon);
	
	uPointer(&start,myIcon);
	uDrawAdjustReal(&start,myIcon);
	last=start;
	oldR.y=0;
	oldR.x=0;
	oldR.xsize=0;
	oldR.ysize=0;
	isDrawn=FALSE;
	while(uStillDown(myIcon)){
		    uPointer(&current,myIcon);
			uDrawAdjustReal(&current,myIcon);
		    if((current.x == last.x) && (current.y == last.y))continue;
		    if(isDrawn){
			    uFrameRect(&oldR,myIcon);
		    }
		    isDrawn=TRUE;
		    last=current;
		    if(start.x < last.x){
		        oldR.x=start.x;
		        oldR.xsize=last.x-start.x;
		    }else{
		        oldR.x=last.x;
		        oldR.xsize=start.x-last.x;
		    }
		    if(start.y < last.y){
		        oldR.y=start.y;
		        oldR.ysize=last.y-start.y;
		    }else{
		        oldR.y=last.y;
		        oldR.ysize=start.y-last.y;
		    }
		    uFrameRect(&oldR,myIcon);
	}

	if(isDrawn){
		uFrameRect(&oldR,myIcon);
	}
	
	box->x=oldR.x;
	box->y=oldR.y;
	
	box->xsize=oldR.xsize;
	box->ysize=oldR.ysize;
	
	uSetPenXOR(FALSE,myIcon);
	
	return 0;
}
static int DoRoundRectDown(IconPtr myIcon)
{
	uRect r;
	uRect oldRect;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	if(drawBox(myIcon,&r))goto OutOfHere;
	
	if(r.xsize || r.ysize){
	
	    if(!IsShift()){
	        DOListDeSelectAll(&w->DList);
	    }
	    oldRect=r;
	    DOListdoBoxes(&w->DList,RoundRectType,&oldRect);
	}
	ToArrowMode(myIcon);
OutOfHere:
	return 0;
}
static int DoOvalDown(IconPtr myIcon)
{
	uRect r;
	uRect oldRect;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	if(drawBox(myIcon,&r))goto OutOfHere;
	
	if(r.xsize || r.ysize){
	
	    if(!IsShift()){
	        DOListDeSelectAll(&w->DList);
	    }
	    oldRect=r;
	    DOListdoBoxes(&w->DList,OvalType,&oldRect);
	}
	ToArrowMode(myIcon);
OutOfHere:
	return 0;
}
static int DoBoxDown(IconPtr myIcon)
{
	uRect r;
	uRect oldRect;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	if(drawBox(myIcon,&r))goto OutOfHere;
	
	if(r.xsize || r.ysize){
	
	    if(!IsShift()){
	        DOListDeSelectAll(&w->DList);
	    }
	    oldRect=r;
	    DOListdoBoxes(&w->DList,BoxType,&oldRect);
	}
	ToArrowMode(myIcon);
OutOfHere:
	return 0;
}
static int DoLineDown(IconPtr myIcon)
{
	uPoint p1,p2;
	uRect oldRect;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	if(drawLine(myIcon,&p1,&p2))return 1;
	
	if(p1.x != p2.x || p1.y != p2.y){
	    if(!IsShift()){
	        DOListDeSelectAll(&w->DList);
	    }
	    oldRect.x=p1.x;
	    oldRect.y=p1.y;
	    oldRect.xsize=p2.x;
	    oldRect.ysize=p2.y;
	    DOListdoBoxes(&w->DList,LineType,&oldRect);
	}
	ToArrowMode(myIcon);
	return 0;
}
static int doSelection(IconPtr myIcon,uPoint *uwhere,int clicks)
{
	struct DrawWindow *t;
	DObjPtr alpha;
	rPoint pt;
	int ret;
	uRect r;

	if(!myIcon || !uwhere)return 1;
	t=(struct DrawWindow *)myIcon->DWindow;
	if(!t)return 1;
		
	ret=1;

	pt=screenToLocal(uwhere,&t->DList);
		        
    if(DOStretchRgn(*uwhere,&t->DList)){
	    DOListDoStretch(*uwhere,&t->DList);
	    goto OutOfHere;
	}
	
	if(clicks){
	    ret=DODoubleClick(&pt,&t->DList);
	    goto OutOfHere;
	}
	
	if((alpha = DOListFindObject(&pt,&t->DList)) != NULL){
	    if(alpha->SelectedPoint && (*alpha->SelectedPoint)(alpha,&pt)){   /* If true, this part of the object is Selected */
		    if(IsShift()){
		        DOListDeSelectClick(alpha,&pt);
		        t->LastClick=TickCount();
		        goto OutOfHere;
		    }
	    }else {   /* At this point, the part of the object was not selected */
		    if(!IsShift()){
		        DOListDeSelectAll(&t->DList);
		    }
		    DOListSelectClick(alpha,&pt);
		    t->DuplicateFlag=FALSE;
	    }
	    DOListDragIt(&t->DList,alpha);
	    t->LastClick=TickCount();
	    goto OutOfHere;	 
	}else{   /* The Click on Nothing Path */
	
	    if(!IsShift()){
		    DOListDeSelectAll(&t->DList);
	    }
		
		if(drawBox(myIcon,&r)){
		    goto OutOfHere;
		}
		
		if(r.xsize || r.ysize){
		    DOListSelectBox(&t->DList,&r);
		}else{
		    goto OutOfHere;
		}
	}
		
OutOfHere:	
    DOListUpdate(&t->DList,0);
	return ret;
	 
}
rPoint screenToLocal(uPoint *uwhere,DOListPtr l)
{
	double scale,oscale;
	static rPoint zero;
	IconPtr myIcon;
	rPoint ret;
	drawPtr t;	
	int x,y;
	
	ret=zero;
	
	if(!uwhere || !l || !l->myIcon)return ret;	
	myIcon=l->myIcon;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return ret;


	x=0;
	y=0;
	if(myIcon->VControl)y=(int)uGetControlValue(myIcon->VControl);
	if(myIcon->HControl)x=(int)uGetControlValue(myIcon->HControl);
	
	scale=t->scale;
	if(scale <= 0.0)scale=1.0;
	oscale=1.0/scale;
	
	ret.x= PixelToLocal((int)((double)(uwhere->x+x-myIcon->ControlSpace)*oscale),l);
	ret.y= PixelToLocal((int)((double)(uwhere->y+y-myIcon->mMenuOffSet)*oscale),l);

	return ret;
}
uPoint localToScreen(rPoint *r,DOListPtr l)
{
	double scale;
	static uPoint zero;
	IconPtr myIcon;
	int x,y;
	drawPtr t;	
	uPoint ret;
	
	ret=zero;
	
	if(!r || !l || !l->myIcon)return ret;	
	myIcon=l->myIcon;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return ret;

	scale=t->scale;
	if(scale <= 0.0)scale=1.0;

	x=0;
	y=0;
	if(myIcon->VControl)y=(int)uGetControlValue(myIcon->VControl);
	if(myIcon->HControl)x=(int)uGetControlValue(myIcon->HControl);
	
	ret.x= (int)((double)(LocalToPixel(r->x,l))*scale)+l->myIcon->ControlSpace-x;
	ret.y= (int)((double)(LocalToPixel(r->y,l))*scale)+myIcon->mMenuOffSet-y;

	return ret;
}
int uDrawAdjustReal(uPoint *start,IconPtr myIcon)
{
	long int truncate;
	rPoint r;
	drawPtr w;
	
	if(!myIcon)return 1;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return 1;
	
	if(!start)return 1;
	
	if(!w->SnapTo){
	    return 0;
	}
	
	r=screenToLocal(start,&w->DList);
	
	if(r.x >= 0){
	    truncate = (long)(r.x/w->vSnapTo + .5);
	}else{
	    truncate = (long)(r.x/w->vSnapTo - .5);
	}
	r.x = (double)truncate*w->vSnapTo;
	
	if(r.y >= 0){
	    truncate = (long)(r.y/w->vSnapTo + .5);
	}else{
	    truncate = (long)(r.y/w->vSnapTo - .5);
	}
	r.y = (double)truncate*w->vSnapTo;
	
	*start=localToScreen(&r,&w->DList);
		
	return 0;
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
	drawPtr t;


	if(!myIcon)return;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return;	

	xsize=(long)((double)t->ixmax*t->scale);
	ysize=(long)((double)t->iymax*t->scale);
	
	uSizeWindow(myIcon,(int)(xsize+ScrollOffSet+myIcon->ControlSpace),(int)(ysize+ScrollOffSet+myIcon->mMenuOffSet+myIcon->CursorSpace));	
	
	InvalRectMyWindow(myIcon);

	tMoveControls(myIcon);
	
	uSetControlValue(myIcon->VControl,0L);
	uSetControlValue(myIcon->HControl,0L);
	SetFrame(myIcon);
}
static void tMoveControls(IconPtr myIcon)
{
	
	controlPtr control;
	struct DrawWindow *t;
	uRect eraseRect;
	uRect dataRect;
	int MaxValue;
	uRect r;
	int bottom;
	int right;	
	int top;
	int left;
	
	if(!myIcon)return;
	t=(struct DrawWindow *)myIcon->DWindow;
	if(!t)return;
	
	if(myIcon->VControl)ShowControl(myIcon->VControl);
	if(myIcon->HControl)ShowControl(myIcon->HControl);
	
	uGetPortBounds(myIcon,&r);
	
	top=0;
	left=0;
	bottom=r.ysize;
	right=r.xsize;
	
	myIcon->uviewRect = r;
	myIcon->uviewRect.y = myIcon->mMenuOffSet;
	myIcon->uviewRect.x = myIcon->ControlSpace;
	myIcon->uviewRect.ysize -= (myIcon->mMenuOffSet+ScrollOffSet+myIcon->CursorSpace);
	myIcon->uviewRect.xsize -= (ScrollOffSet+myIcon->ControlSpace);

	ScreenSetSize(myIcon->uviewRect.xsize,myIcon->uviewRect.ysize,myIcon);

	dataRect=myIcon->uviewRect;
	dataRect.y=myIcon->uviewRect.y+myIcon->uviewRect.ysize+1;
	dataRect.ysize=DataSpace;
	
	myIcon->dataRect=dataRect;

	
	eraseRect.y=bottom-ScrollOffSet;
	eraseRect.x=left;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;
	
	control=myIcon->VControl;
	if(control){
		MoveControl(control,right-ScrollOffSet,top+myIcon->mMenuOffSet);
		SizeControl(control,ScrollOffSet,bottom-top-ScrollOffSet-myIcon->mMenuOffSet);
		MaxValue=(int)(t->iymax*t->scale-(r.ysize-ScrollOffSet-myIcon->mMenuOffSet-myIcon->CursorSpace));
		if(MaxValue < 0)MaxValue = 0;
		uSetControlMaximum(control,MaxValue);
	}
	
	control=myIcon->HControl;
	if(control){	
		MoveControl(control,eraseRect.x+eraseRect.xsize,bottom-ScrollOffSet);
		SizeControl(control,right-(eraseRect.xsize+ScrollOffSet),ScrollOffSet);
		MaxValue=(int)(t->ixmax*t->scale-(right-ScrollOffSet-myIcon->ControlSpace));
		if(MaxValue < 0)MaxValue = 0;
		uSetControlMaximum(control,MaxValue);
	}
	
	InvalRectMyWindow(myIcon);
}

static void EndRect(IconPtr myIcon,int ThisControl,uRect *r)
{
	drawPtr w;
	
	if(!myIcon)return;
	w=(drawPtr)myIcon->DWindow;
	if(!w)return;
  	
	uSetForeByIndex(255,myIcon);
	uSetBackByIndex(0,myIcon);
	
	if(w->ActiveControl == ThisControl){
	    if(w->ActiveLocked){
	        uInvertRect(r,myIcon);
	    } else {
	        ;
	    }
	}
	StartRect(r,myIcon);

}
static void StartRect(uRect *r,IconPtr myIcon)
{
	uRect o;
	
	if(!r)return;
	
	o = *r;
	
	o.x -= 1;
	o.y -= 1;
	o.xsize += 1;
	o.ysize += 1;

	uFrameRect(&o,myIcon);
}
static int DrawMyControls(IconPtr myIcon)
{
	struct DOList *l;
	
	uPattern *pat;

    drawPtr t;
	uRect r;
    
	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	
	l=&t->DList;
	
	uGetPortBounds(myIcon,&r);
	uClipRect(&r,myIcon);
	
	r.y=myIcon->mMenuOffSet;
	r.xsize = myIcon->ControlSpace;
	r.ysize -= myIcon->CursorSpace;
	
	uEraseRect(&r,myIcon);
	
	uSetForeByIndex(255,myIcon);

	uSetBackByIndex(0,myIcon);
	
	/*
	pat=PatternPattern(0);
	if(pat){
       uSetPenPattern(pat->pat,myIcon);
    }
    */
    
	r=myIcon->uviewRect;
	
	uMoveTo(r.x-1,r.y);
	uLineTo(r.x-1,r.y+r.ysize+myIcon->CursorSpace,myIcon);
	
	uMoveTo(r.x,r.y+r.ysize);
	uLineTo(r.x+r.xsize-1,r.y+r.ysize,myIcon);	
	
	StartRect(&t->TextRect,myIcon);
	
	if(t->ActiveControl == ControlAlpha && !t->ActiveLocked){
	    uDrawImageRect(&t->TextImageHigh,myIcon);	
	}else{
	    uDrawImageRect(&t->TextImage,myIcon);	
	}
	EndRect(myIcon,ControlAlpha,&t->TextRect);
	
		
	StartRect(&t->ArrowRect,myIcon);
	if(t->ActiveControl == ControlArrow && !t->ActiveLocked){
	    uDrawImageRect(&t->ArrowImageHigh,myIcon);	
	}else{
	    uDrawImageRect(&t->ArrowImage,myIcon);	
	}
	EndRect(myIcon,ControlArrow,&t->ArrowRect);
		
	StartRect(&t->LineRect,myIcon);
	if(t->ActiveControl == ControlLine && !t->ActiveLocked){
	    uDrawImageRect(&t->LineImageHigh,myIcon);	
	}else{
	    uDrawImageRect(&t->LineImage,myIcon);	
	}
	EndRect(myIcon,ControlLine,&t->LineRect);
		
	StartRect(&t->BoxRect,myIcon);
	if(t->ActiveControl == ControlBox && !t->ActiveLocked){
	    uDrawImageRect(&t->BoxImageHigh,myIcon);	
	}else{
	    uDrawImageRect(&t->BoxImage,myIcon);	
	}
	EndRect(myIcon,ControlBox,&t->BoxRect);
		
	StartRect(&t->OvalRect,myIcon);
	if(t->ActiveControl == ControlOval && !t->ActiveLocked){
	    uDrawImageRect(&t->OvalImageHigh,myIcon);	
	}else{
	    uDrawImageRect(&t->OvalImage,myIcon);	
	}
	EndRect(myIcon,ControlOval,&t->OvalRect);
		
	StartRect(&t->RoundRectRect,myIcon);
	if(t->ActiveControl == ControlRoundRect && !t->ActiveLocked){
	    uDrawImageRect(&t->RoundRectImageHigh,myIcon);	
	}else{
	    uDrawImageRect(&t->RoundRectImage,myIcon);	
	}
	EndRect(myIcon,ControlRoundRect,&t->RoundRectRect);
		
	r=t->LineForeRect;
	uInsetRect(&r,1,1);
	uSetForeByIndex(l->Attributes.nLineFore,myIcon);
	uSetBackByIndex(l->Attributes.nLineFore,myIcon);
	uPaintRect(&r,myIcon);
	
	
	r=t->LineBackRect;
	uInsetRect(&r,1,1);
	uSetForeByIndex(l->Attributes.nLineBack,myIcon);
	uSetBackByIndex(l->Attributes.nLineBack,myIcon);
	uPaintRect(&r,myIcon);
	
	r=t->SolidForeRect;
	uInsetRect(&r,1,1);
	uSetForeByIndex(l->Attributes.nSolidFore,myIcon);
	uSetBackByIndex(l->Attributes.nSolidFore,myIcon);
	uPaintRect(&r,myIcon);
	
	
	r=t->SolidBackRect;
	uInsetRect(&r,1,1);
	uSetForeByIndex(l->Attributes.nSolidBack,myIcon);
	uSetBackByIndex(l->Attributes.nSolidBack,myIcon);
	uPaintRect(&r,myIcon);
	
	
	r=t->LinePatternRect;
	uInsetRect(&r,1,1);
	uSetForeByIndex(l->Attributes.nLineFore,myIcon);
	uSetBackByIndex(l->Attributes.nLineBack,myIcon);
	
	pat=PatternPattern(l->Attributes.LinePattern);
	if(pat){
		uFillRectPattern(&r,pat,myIcon);
	}else{
		uPaintRect(&r,myIcon);
	}


	r=t->SolidPatternRect;
	uInsetRect(&r,1,1);
	uSetForeByIndex(l->Attributes.nSolidFore,myIcon);
	uSetBackByIndex(l->Attributes.nSolidBack,myIcon);
	
	pat=PatternPattern(l->Attributes.SolidPattern);
	if(pat){
		uFillRectPattern(&r,pat,myIcon);
	}else{
		uPaintRect(&r,myIcon);
	}
	
	
	
	uSetForeByIndex(255,myIcon);

	uSetBackByIndex(0,myIcon);
	
	uFrameRect(&t->LineForeRect,myIcon);
	
	uFrameRect(&t->LineBackRect,myIcon);
	
	
	uFrameRect(&t->SolidForeRect,myIcon);
	
	uFrameRect(&t->SolidBackRect,myIcon);
	
	uWriteLineBox(&t->LineWidthRect,(int)l->Attributes.xLineWidth,(int)l->Attributes.yLineWidth,myIcon);
	
	uFrameRect(&t->LineWidthRect,myIcon);
	
	uFrameRect(&t->LinePatternRect,myIcon);
	
	uFrameRect(&t->SolidPatternRect,myIcon);
	


	return 0;	
	
}
static int UpdateWrite(IconPtr myIcon)
{
        uRect portRect;
		DOListPtr l;
	    drawPtr t;

		if(!myIcon || !myIcon->isOpen)return 1;
		t=(drawPtr)myIcon->DWindow;
		if(!t)return 1;
		l=&t->DList;

			    
		drawIt(myIcon);
		
		DODrawSelected(l);

	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);		


	    if(l->Edit){
	        uEditDraw(l->Edit);
	    }
					
		DrawMyControls(myIcon);
		
	    uDrawControls(myIcon);
	    uDrawGrowIcon(myIcon);
	    
	    return 0;
}
		
static void drawIt(IconPtr myIcon)
{
	drawPtr t;
	uRect portRect;
	uRect Box;
	uPoint pt;

	if(!myIcon)return;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return;


	uClipRect(&myIcon->uviewRect,myIcon);
	
	/*
	uEraseRect(&myIcon->uviewRect,myIcon);
    */
    
	Box.x = myIcon->ControlSpace;
	Box.y = myIcon->uviewRect.y;
	Box.xsize=(int)((double)t->ixmax*t->scale);
	Box.ysize=(int)((double)t->iymax*t->scale);

	drawTek(myIcon,t->CurrentFrame,&Box);

	DrawBackGround(myIcon);

	uGetPortBounds(myIcon,&portRect);
	uClipRect(&portRect,myIcon);

	uPointer(&pt,myIcon);
	nMouseMoved(myIcon,pt);
	

}
static int drawTek(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
    struct DrawWindow *s;
	unsigned char *buffRead;
    uRect SRect,LRect;
    int top,left,dx,dy,dx1,dy1;
    double scale,oscale;
    long xsize,ysize;
    long length;
	DOListPtr l;
    
	if(!myIcon)return 1;
	s=(struct DrawWindow *)myIcon->DWindow;
	if(!s)return 1;
	l=&s->DList;
	

	if(DODraw(l,CurrentFrame))goto ErrorOut;
	
	top=0;
	left=0;
	
	
	length=l->xsize*l->ysize;

    if(SetBuffers(length*3+512L))goto ErrorOut;
	    					  
	buffRead=(unsigned char *)xg.buffin;

    memcpy(buffRead,l->data,length*3);
		
	LRect= *DRect;
	
	dx=LRect.xsize;
	dy=LRect.ysize;

	top=0;
	left=0;
	if(myIcon->VControl)top=(int)uGetControlValue(myIcon->VControl);
	if(myIcon->HControl)left=(int)uGetControlValue(myIcon->HControl);
	
	scale=s->scale;
	
	if(scale <= 0.0)scale=1.0;
	
	oscale=1.0/scale;

	SRect.y=(int)((double)top*oscale+0.5);
	if(SRect.y < 0)SRect.y=0;
	if(SRect.y > l->ysize)SRect.y=(int)l->ysize;

	SRect.ysize = (int)((double)dy*oscale+0.5);
	if(SRect.ysize < 0)SRect.ysize=0;
	if(SRect.y+SRect.ysize > l->ysize)SRect.ysize=(int)(l->ysize-SRect.y);


	SRect.x=(int)((double)left*oscale+0.5);
	if(SRect.x < 0)SRect.x=0;
	if(SRect.x > l->xsize)SRect.x=(int)l->xsize;

	SRect.xsize = (int)((double)dx*oscale+0.5);
	if(SRect.xsize < 0)SRect.xsize=0;
	if(SRect.x+SRect.xsize > l->xsize)SRect.xsize=(int)(l->xsize-SRect.x);

	dx1=SRect.xsize;
	if(dx1 != (int)((double)dx*oscale+0.5)){
	    LRect.xsize = (int)((double)dx1*scale+0.5);
	}

	dy1=SRect.ysize;
	if(dy1 != (int)((double)dy*oscale+0.5)){
	    LRect.ysize = (int)((double)dy1*scale+0.5);
	}

	ScreenSetPalette(myIcon,myIcon->red,myIcon->green,myIcon->blue);
	
	xsize=l->xsize;
	ysize=l->ysize;
	
	if(scale == 0.5 && SRect.x == 0 && SRect.y == 0){
		doAntiAlias(xsize,ysize,(unsigned char *)buffRead);
		xsize /= 2;
		ysize /= 2;
		SRect.xsize /= 2;
		SRect.ysize /= 2;
	}

	ScreenSetSize(xsize,ysize,myIcon);
	
	uScreenWrite(xsize,ysize,(unsigned char *)buffRead,(unsigned char *)xg.buffout,ScreenDataType24,&SRect,&LRect,myIcon);   
		
ErrorOut: 
	    			    
		return 0;
}
static int doAntiAlias(int ixmax,int iymax,unsigned char *data)
{
	long n1,n2,n3,n4,n5,n6,n7,n8,n9;
	unsigned char *bline,*bc,*bp;
	double d1,d2,d3;
	long LineSize,length,n;
	int ix,iy;
	
	if(!data)return 1;

	LineSize=ixmax*3;
	
	length=LineSize*iymax;
	
    
	bline=(unsigned char *)cMalloc(length*sizeof(unsigned char),8472);
    if(!bline){
        WarningBatch("doAntiAlias Out of Memory\n");
        return 1;
    }
	
	for(n=0;n<length;++n)bline[n]=data[n];
	
	bp=bline;
	bc=data;

	ixmax=ixmax/2;
	iymax=iymax/2;
	for(iy=0;iy<iymax;++iy){
	    for(ix=0;ix<ixmax;++ix){
	        n1=2L*ix*3+2L*iy*LineSize;
		if(ix == 0){
		    if(iy == 0){
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        d1= 0.1111f*(4*bp[n1]+2*(bp[n2]+bp[n4])+bp[n3]);
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        d2= 0.1111f*(4*bp[n1]+2*(bp[n2]+bp[n4])+bp[n3]);
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        d3= 0.1111f*(4*bp[n1]+2*(bp[n2]+bp[n4])+bp[n3]);
		    }else{
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-LineSize;
		        n6=n5+3;
		        d1= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5])+(bp[n3]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-LineSize;
		        n6=n5+3;
		        d2= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5])+(bp[n3]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-LineSize;
		        n6=n5+3;
		        d3= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5])+(bp[n3]+bp[n6]));
		    }
		}else{
		    if(iy == 0){
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        d1= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n3]+bp[n5])+(bp[n4]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        d2= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n3]+bp[n5])+(bp[n4]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        d3= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n3]+bp[n5])+(bp[n4]+bp[n6]));
		    }else{
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        n7=n5-LineSize;
		        n8=n1-LineSize;
		        n9=n4-LineSize;
		        d1= 0.0625f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5]+bp[n8])+(bp[n3]+bp[n6]+bp[n7]+bp[n9]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        n7=n5-LineSize;
		        n8=n1-LineSize;
		        n9=n4-LineSize;
		        d2= 0.0625f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5]+bp[n8])+(bp[n3]+bp[n6]+bp[n7]+bp[n9]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        n7=n5-LineSize;
		        n8=n1-LineSize;
		        n9=n4-LineSize;
		        d3= 0.0625f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5]+bp[n8])+(bp[n3]+bp[n6]+bp[n7]+bp[n9]));
		    }
		}

	        *bc++ = (unsigned char)d1;
	        *bc++ = (unsigned char)d2;
	        *bc++ = (unsigned char)d3;
	    }
	}
	
	if(bline)cFree((char *)bline);
	
	return 0;
}
static int DrawAnimate(IconPtr myIcon,long CurrentFrame)
{
	drawPtr s;
	uPort save;
	
	if(!myIcon)return 1;
	s=(drawPtr)myIcon->DWindow;
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
	    struct DrawWindow *t;
	    uRect portRect;
		uRect Box;
	    
		if(!myIcon)return;
		t=(struct DrawWindow *)myIcon->DWindow;
		if(!t)return;
		
	
	    uClipRect(&myIcon->uviewRect,myIcon);
	    /* uEraseRect(&myIcon->uviewRect,myIcon); */
	    
  		Box.x = myIcon->ControlSpace;
  		Box.y = myIcon->mMenuOffSet;
  		Box.xsize=(int)((double)t->ixmax*t->scale);
  		Box.ysize=(int)((double)t->iymax*t->scale);
  		
		drawTek2(myIcon,t->CurrentFrame,&Box);
		
	    uGetPortBounds(myIcon,&portRect);
	    uClipRect(&portRect,myIcon);
	    
}
static int drawTek2(IconPtr myIcon,long CurrentFrame,uRect *DRect)
{
	struct DrawWindow *t;

	if(!myIcon || !DRect)return 1;
	t=(struct DrawWindow *)myIcon->DWindow;
	if(!t)return 1;
	
	t->CurrentFrame=CurrentFrame;
			
	drawTek(myIcon,CurrentFrame,DRect);
	
	return 0;
}
static int DrawBackGround(IconPtr myIcon)
{
	DOListPtr l;
	drawPtr t;
	static rPoint r;
	uPoint p;
	int j,i,delta;
	double scale;
	/*
	uPattern *pat;
	*/
	
	if(!myIcon)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	l=&t->DList;
	
	
    
	if(!t->BackGround)return 1;
	
	scale=t->scale;
	if(scale <= 0.0)scale=1.0;
	
    p=localToScreen(&r,l);

	uSetForeByIndex(255,myIcon);
/*
	pat=PatternPattern(0);
	if(pat){
       uSetPenPattern(pat->pat,myIcon);
    }

*/	
	uMoveTo(p.x,p.y+(int)((double)(t->iymax)*scale+0.5));
	uLineTo(p.x+(int)((double)(t->ixmax)*scale+0.5),p.y+(int)((double)(t->iymax)*scale+0.5),myIcon);
	uLineTo(p.x+(int)((double)(t->ixmax)*scale+0.5),p.y,myIcon);
	
	uMoveTo(p.x,p.y+(int)((double)(t->iymax)*scale+0.5)+1);
	uLineTo(p.x+(int)((double)(t->ixmax)*scale+0.5)+1,p.y+(int)((double)(t->iymax)*scale+0.5)+1,myIcon);
	uLineTo(p.x+(int)((double)(t->ixmax)*scale+0.5)+1,p.y,myIcon);
	
	if(!t->GridLines)return 1;
	
    
    delta=t->GridDelta;
    
    if(delta <= 0)delta=72;
    
    delta=(int)((double)delta*scale + 0.5);
    
    for(j=0;j<(int)((double)(t->iymax)*scale+0.5);j += delta){
        for(i=0;i<(int)((double)(t->ixmax)*scale+0.5);i += 8){
            uMoveTo(p.x+i,p.y+j);
            uLineTo(p.x+i+1,p.y+j,myIcon);
        }
    }
	
	
    for(i=0;i<(int)((double)(t->ixmax)*scale+0.5);i += delta){
    	for(j=0;j<(int)((double)(t->iymax)*scale+0.5);j += 8){
        	uMoveTo(p.x+i,p.y+j);
        	uLineTo(p.x+i,p.y+j+1,myIcon);
        }
    }
	
	
	return 0;
}
static int SetFrame(IconPtr myIcon)
{
	struct DrawWindow *t;

	if(!myIcon)return 1;
	t=(struct DrawWindow *)myIcon->DWindow;
	if(!t)return 1;

	if(t->ImageCount <= 0){
	    t->CurrentFileName="No Images";
	    return 1;
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
static int doClose(IconPtr myIcon)
{
	struct DrawWindow *t;
	DOListPtr l;
	
	if(myIcon){
	    SetPortMyWindow(myIcon);
	    t=(struct DrawWindow *)myIcon->DWindow;
   	    if((t)){
			if(myIcon->Files){
			    freeFileList(myIcon->Files);
		    	myIcon->Files=NULL;
			}
	        if(t->stackList){
	           cFree((char *)t->stackList);
	           t->stackList=NULL;
	        }
	        
	        uImageRectEnd(&t->TextImageHigh);
	        uImageRectEnd(&t->ArrowImageHigh);

	        uImageRectEnd(&t->LineImageHigh);
	        uImageRectEnd(&t->BoxImageHigh);

	        uImageRectEnd(&t->OvalImageHigh);
	        uImageRectEnd(&t->RoundRectImageHigh);
	        
	        l=&t->DList;
	        
	        if(l->Edit)uEditDelete(l->Edit);
	        l->Edit=NULL;
	        
 			DOListDelete(l);
	    		        
	    	myIcon->DWindow=NULL;
	        cFree((char *)t);
	        
		}
	    
	    myIcon=RemoveIcon(myIcon);
	    
	}
	return 0;
}
static int DrawListReadWrite(IconPtr myIcon,FILE8 *out,int flag)
{
	drawPtr t;
	int ret;
	
	if(!myIcon || !FindWindowInList(myIcon) || !out)return 1;
	t=(drawPtr)myIcon->DWindow;
	if(!t)return 1;
	
	
	ret=DOListReadWrite(&t->DList,out,flag);

	t->ImageCount=DOFrameCount(&t->DList);
		
	return ret;
}
int doUniversalDraw(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct DrawWindow ss;
	static struct Icon mm;
	static struct lineAttributes{
	     struct uAttributes lp[10];
	     int Count;
	}ll;

	static struct universalTableListStruct Local3[] = {
		{"REAL",5000,universal_TypeInt,(void *)&ll.Count},	
		
		{"DOUB",5001,universal_TypeDouble,(void *)&ll.lp[0].xLineWidth},	
		{"DOUB",5002,universal_TypeDouble,(void *)&ll.lp[0].yLineWidth},			
		{"REAL",5003,universal_TypeInt,(void *)&ll.lp[0].LinePattern},	
		{"REAL",5004,universal_TypeInt,(void *)&ll.lp[0].LineSymbol},	
		{"REAL",5005,universal_TypeInt,(void *)&ll.lp[0].DashFlag},	
		{"REAL",5006,universal_TypeInt,(void *)&ll.lp[0].nLineFore},	
		{"REAL",5007,universal_TypeInt,(void *)&ll.lp[0].nLineBack},	
		{"REAL",5008,universal_TypeInt,(void *)&ll.lp[0].LineDashNumber},	
		{"REAL",5009,universal_TypeInt,(void *)&ll.lp[0].LineIncrement},
		{"REAL",5010,universal_TypeInt,(void *)&ll.lp[0].font.txFace},
		{"REAL",5011,universal_TypeInt,(void *)&ll.lp[0].font.txFont},
		{"REAL",5012,universal_TypeInt,(void *)&ll.lp[0].font.txSize},
		{"STRI",5014,universal_TypeString,(void *)&ll.lp[0].LineDash,sizeof(ll.lp[0].LineDash)},

		
		{"DOUB",5101,universal_TypeDouble,(void *)&ll.lp[1].xLineWidth},	
		{"DOUB",5102,universal_TypeDouble,(void *)&ll.lp[1].yLineWidth},			
		{"REAL",5103,universal_TypeInt,(void *)&ll.lp[1].LinePattern},	
		{"REAL",5104,universal_TypeInt,(void *)&ll.lp[1].LineSymbol},	
		{"REAL",5105,universal_TypeInt,(void *)&ll.lp[1].DashFlag},	
		{"REAL",5106,universal_TypeInt,(void *)&ll.lp[1].nLineFore},	
		{"REAL",5107,universal_TypeInt,(void *)&ll.lp[1].nLineBack},	
		{"REAL",5108,universal_TypeInt,(void *)&ll.lp[1].LineDashNumber},	
		{"REAL",5109,universal_TypeInt,(void *)&ll.lp[1].LineIncrement},
		{"REAL",5110,universal_TypeInt,(void *)&ll.lp[1].font.txFace},
		{"REAL",5111,universal_TypeInt,(void *)&ll.lp[1].font.txFont},
		{"REAL",5112,universal_TypeInt,(void *)&ll.lp[1].font.txSize},
		{"STRI",5114,universal_TypeString,(void *)&ll.lp[1].LineDash,sizeof(ll.lp[1].LineDash)},
		
		{"DOUB",5201,universal_TypeDouble,(void *)&ll.lp[2].xLineWidth},	
		{"DOUB",5202,universal_TypeDouble,(void *)&ll.lp[2].yLineWidth},			
		{"REAL",5203,universal_TypeInt,(void *)&ll.lp[2].LinePattern},	
		{"REAL",5204,universal_TypeInt,(void *)&ll.lp[2].LineSymbol},	
		{"REAL",5205,universal_TypeInt,(void *)&ll.lp[2].DashFlag},	
		{"REAL",5206,universal_TypeInt,(void *)&ll.lp[2].nLineFore},	
		{"REAL",5207,universal_TypeInt,(void *)&ll.lp[2].nLineBack},	
		{"REAL",5208,universal_TypeInt,(void *)&ll.lp[2].LineDashNumber},	
		{"REAL",5209,universal_TypeInt,(void *)&ll.lp[2].LineIncrement},
		{"REAL",5210,universal_TypeInt,(void *)&ll.lp[2].font.txFace},
		{"REAL",5211,universal_TypeInt,(void *)&ll.lp[2].font.txFont},
		{"REAL",5212,universal_TypeInt,(void *)&ll.lp[2].font.txSize},
		{"STRI",5214,universal_TypeString,(void *)&ll.lp[2].LineDash,sizeof(ll.lp[2].LineDash)},
		
		{"DOUB",5301,universal_TypeDouble,(void *)&ll.lp[3].xLineWidth},	
		{"DOUB",5302,universal_TypeDouble,(void *)&ll.lp[3].yLineWidth},			
		{"REAL",5303,universal_TypeInt,(void *)&ll.lp[3].LinePattern},	
		{"REAL",5304,universal_TypeInt,(void *)&ll.lp[3].LineSymbol},	
		{"REAL",5305,universal_TypeInt,(void *)&ll.lp[3].DashFlag},	
		{"REAL",5306,universal_TypeInt,(void *)&ll.lp[3].nLineFore},	
		{"REAL",5307,universal_TypeInt,(void *)&ll.lp[3].nLineBack},	
		{"REAL",5308,universal_TypeInt,(void *)&ll.lp[3].LineDashNumber},	
		{"REAL",5309,universal_TypeInt,(void *)&ll.lp[3].LineIncrement},
		{"REAL",5310,universal_TypeInt,(void *)&ll.lp[3].font.txFace},
		{"REAL",5311,universal_TypeInt,(void *)&ll.lp[3].font.txFont},
		{"REAL",5312,universal_TypeInt,(void *)&ll.lp[3].font.txSize},
		{"STRI",5314,universal_TypeString,(void *)&ll.lp[3].LineDash,sizeof(ll.lp[3].LineDash)},
		
		{"DOUB",5401,universal_TypeDouble,(void *)&ll.lp[4].xLineWidth},	
		{"DOUB",5402,universal_TypeDouble,(void *)&ll.lp[4].yLineWidth},			
		{"REAL",5403,universal_TypeInt,(void *)&ll.lp[4].LinePattern},	
		{"REAL",5404,universal_TypeInt,(void *)&ll.lp[4].LineSymbol},	
		{"REAL",5405,universal_TypeInt,(void *)&ll.lp[4].DashFlag},	
		{"REAL",5406,universal_TypeInt,(void *)&ll.lp[4].nLineFore},	
		{"REAL",5407,universal_TypeInt,(void *)&ll.lp[4].nLineBack},	
		{"REAL",5408,universal_TypeInt,(void *)&ll.lp[4].LineDashNumber},	
		{"REAL",5409,universal_TypeInt,(void *)&ll.lp[4].LineIncrement},
		{"REAL",5410,universal_TypeInt,(void *)&ll.lp[4].font.txFace},
		{"REAL",5411,universal_TypeInt,(void *)&ll.lp[4].font.txFont},
		{"REAL",5412,universal_TypeInt,(void *)&ll.lp[4].font.txSize},
		{"STRI",5414,universal_TypeString,(void *)&ll.lp[4].LineDash,sizeof(ll.lp[4].LineDash)},
		
		{"DOUB",5501,universal_TypeDouble,(void *)&ll.lp[5].xLineWidth},	
		{"DOUB",5502,universal_TypeDouble,(void *)&ll.lp[5].yLineWidth},			
		{"REAL",5503,universal_TypeInt,(void *)&ll.lp[5].LinePattern},	
		{"REAL",5504,universal_TypeInt,(void *)&ll.lp[5].LineSymbol},	
		{"REAL",5505,universal_TypeInt,(void *)&ll.lp[5].DashFlag},	
		{"REAL",5506,universal_TypeInt,(void *)&ll.lp[5].nLineFore},	
		{"REAL",5507,universal_TypeInt,(void *)&ll.lp[5].nLineBack},	
		{"REAL",5508,universal_TypeInt,(void *)&ll.lp[5].LineDashNumber},	
		{"REAL",5509,universal_TypeInt,(void *)&ll.lp[5].LineIncrement},
		{"REAL",5510,universal_TypeInt,(void *)&ll.lp[5].font.txFace},
		{"REAL",5511,universal_TypeInt,(void *)&ll.lp[5].font.txFont},
		{"REAL",5512,universal_TypeInt,(void *)&ll.lp[5].font.txSize},
		{"STRI",5514,universal_TypeString,(void *)&ll.lp[5].LineDash,sizeof(ll.lp[5].LineDash)},
		
		{"DOUB",5601,universal_TypeDouble,(void *)&ll.lp[6].xLineWidth},	
		{"DOUB",5602,universal_TypeDouble,(void *)&ll.lp[6].yLineWidth},			
		{"REAL",5603,universal_TypeInt,(void *)&ll.lp[6].LinePattern},	
		{"REAL",5604,universal_TypeInt,(void *)&ll.lp[6].LineSymbol},	
		{"REAL",5605,universal_TypeInt,(void *)&ll.lp[6].DashFlag},	
		{"REAL",5606,universal_TypeInt,(void *)&ll.lp[6].nLineFore},	
		{"REAL",5607,universal_TypeInt,(void *)&ll.lp[6].nLineBack},	
		{"REAL",5608,universal_TypeInt,(void *)&ll.lp[6].LineDashNumber},	
		{"REAL",5609,universal_TypeInt,(void *)&ll.lp[6].LineIncrement},
		{"REAL",5610,universal_TypeInt,(void *)&ll.lp[6].font.txFace},
		{"REAL",5611,universal_TypeInt,(void *)&ll.lp[6].font.txFont},
		{"REAL",5612,universal_TypeInt,(void *)&ll.lp[6].font.txSize},
		{"STRI",5614,universal_TypeString,(void *)&ll.lp[6].LineDash,sizeof(ll.lp[6].LineDash)},
		
		{"DOUB",5701,universal_TypeDouble,(void *)&ll.lp[7].xLineWidth},	
		{"DOUB",5702,universal_TypeDouble,(void *)&ll.lp[7].yLineWidth},			
		{"REAL",5703,universal_TypeInt,(void *)&ll.lp[7].LinePattern},	
		{"REAL",5704,universal_TypeInt,(void *)&ll.lp[7].LineSymbol},	
		{"REAL",5705,universal_TypeInt,(void *)&ll.lp[7].DashFlag},	
		{"REAL",5706,universal_TypeInt,(void *)&ll.lp[7].nLineFore},	
		{"REAL",5707,universal_TypeInt,(void *)&ll.lp[7].nLineBack},	
		{"REAL",5708,universal_TypeInt,(void *)&ll.lp[7].LineDashNumber},	
		{"REAL",5709,universal_TypeInt,(void *)&ll.lp[7].LineIncrement},
		{"REAL",5710,universal_TypeInt,(void *)&ll.lp[7].font.txFace},
		{"REAL",5711,universal_TypeInt,(void *)&ll.lp[7].font.txFont},
		{"REAL",5712,universal_TypeInt,(void *)&ll.lp[7].font.txSize},
		{"STRI",5714,universal_TypeString,(void *)&ll.lp[7].LineDash,sizeof(ll.lp[7].LineDash)},
		
		{"DOUB",5801,universal_TypeDouble,(void *)&ll.lp[8].xLineWidth},	
		{"DOUB",5802,universal_TypeDouble,(void *)&ll.lp[8].yLineWidth},			
		{"REAL",5803,universal_TypeInt,(void *)&ll.lp[8].LinePattern},	
		{"REAL",5804,universal_TypeInt,(void *)&ll.lp[8].LineSymbol},	
		{"REAL",5805,universal_TypeInt,(void *)&ll.lp[8].DashFlag},	
		{"REAL",5806,universal_TypeInt,(void *)&ll.lp[8].nLineFore},	
		{"REAL",5807,universal_TypeInt,(void *)&ll.lp[8].nLineBack},	
		{"REAL",5808,universal_TypeInt,(void *)&ll.lp[8].LineDashNumber},	
		{"REAL",5809,universal_TypeInt,(void *)&ll.lp[8].LineIncrement},
		{"REAL",5810,universal_TypeInt,(void *)&ll.lp[8].font.txFace},
		{"REAL",5811,universal_TypeInt,(void *)&ll.lp[8].font.txFont},
		{"REAL",5812,universal_TypeInt,(void *)&ll.lp[8].font.txSize},
		{"STRI",5814,universal_TypeString,(void *)&ll.lp[8].LineDash,sizeof(ll.lp[8].LineDash)},
		
		{"DOUB",5901,universal_TypeDouble,(void *)&ll.lp[9].xLineWidth},	
		{"DOUB",5902,universal_TypeDouble,(void *)&ll.lp[9].yLineWidth},			
		{"REAL",5903,universal_TypeInt,(void *)&ll.lp[9].LinePattern},	
		{"REAL",5904,universal_TypeInt,(void *)&ll.lp[9].LineSymbol},	
		{"REAL",5905,universal_TypeInt,(void *)&ll.lp[9].DashFlag},	
		{"REAL",5906,universal_TypeInt,(void *)&ll.lp[9].nLineFore},	
		{"REAL",5907,universal_TypeInt,(void *)&ll.lp[9].nLineBack},	
		{"REAL",5908,universal_TypeInt,(void *)&ll.lp[9].LineDashNumber},	
		{"REAL",5909,universal_TypeInt,(void *)&ll.lp[9].LineIncrement},
		{"REAL",5910,universal_TypeInt,(void *)&ll.lp[9].font.txFace},
		{"REAL",5911,universal_TypeInt,(void *)&ll.lp[9].font.txFont},
		{"REAL",5912,universal_TypeInt,(void *)&ll.lp[9].font.txSize},
		{"STRI",5914,universal_TypeString,(void *)&ll.lp[9].LineDash,sizeof(ll.lp[9].LineDash)},
		
	};
	
	static struct universalTableStruct Global3 = {
		"uDraw parameters",universal_WindowDRAW,
		(void *)&ll,sizeof(ll),
		NULL,0L,							
		Local3,sizeof(Local3)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local2[] = {
		{"REAL",4201,universal_TypeInt,(void *)&mm.eraseRectSize},
		{"STRI",4202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",4203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"DOUB",4204,universal_TypeDouble,(void *)&mm.framesPerSecond},	
		{"REAL",4205,universal_TypeInt,(void *)&mm.Synchronize},	
		{"FILE",4218,universal_TypeFilesPtr,(void *)&mm.Files},	
		{"STRI",4222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",4223,universal_TypeInt,(void *)&mm.WindowNameSet},									
				
		{"FUNT",4500,universal_TypeFunctionPtr,(void *)DrawListReadWrite},	
	};
	
	static struct universalTableStruct Global2 = {
		"uDraw parameters",universal_WindowDRAW,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local[] = {
		{"RECT",4004,universal_TypeRect,(void *)&ss.windowRect},	
		{"REAL",4005,universal_TypeInt,(void *)&ss.ActiveControl},	
		{"REAL",4007,universal_TypeInt,(void *)&ss.LoopBackAndForth},	
		{"REAL",4008,universal_TypeInt,(void *)&ss.LoopFlag},	
		{"REAL",4009,universal_TypeInt,(void *)&ss.BackGround},	
		{"REAL",4010,universal_TypeInt,(void *)&ss.GridLines},	
		{"REAL",4011,universal_TypeInt,(void *)&ss.GridDelta},	
		{"REAL",4012,universal_TypeInt,(void *)&ss.SnapTo},	
		{"DOUB",4014,universal_TypeDouble,(void *)&ss.vSnapTo},			
		{"DOUB",4015,universal_TypeDouble,(void *)&ss.scale},			

		{"REAL",4016,universal_TypeInt,(void *)&ss.ixmax},	
		{"REAL",4017,universal_TypeInt,(void *)&ss.iymax},	
		
		{"REAL",4018,universal_TypeInt,(void *)&ss.AntiAlias},	
		
		{"REAL",4031,universal_TypeInt,(void *)&ss.DList.Attributes.font.txFace},
		{"REAL",4032,universal_TypeInt,(void *)&ss.DList.Attributes.font.txFont},
		{"REAL",4033,universal_TypeInt,(void *)&ss.DList.Attributes.font.txSize},
		
		{"REAL",4034,universal_TypeLong,(void *)&ss.CurrentFrame},	
		
				
		{"TABL",4200,universal_TypeTable,(void *)&Global2},	
		
		{"TABL",4300,universal_TypeTable,(void *)&Global3},	
	};
	

	static struct universalTableStruct Global= {
		"uDraw parameters",universal_WindowDRAW,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};


	drawPtr s;
	int n,nc;
	int ret;
	
	s=NULL;
	if(myIcon && (myIcon->WindowType == DrawNewType)){
		s=(drawPtr)myIcon->DWindow;
	}else{
	    myIcon=NULL;
	}

	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct DrawWindow);

	Global2.tableDynamic=myIcon;
	Global2.tableDynamicSpace=sizeof(struct Icon);
		
	Global3.tableDynamic=NULL;
	Global3.tableDynamicSpace=0;
		
	ret=1;
	if(flag == universal_SetDefaults){
	
	    if(s && myIcon){
			uGetPortBoundsGlobal(myIcon,&s->windowRect);			
			if(moveUniversalTable(&Global,universal_MoveToStatic))goto ErrorOut;
			ll.Count=0;
			if(s->DList.lineAttributesCount > 0){
			    nc=s->DList.lineAttributesCount;
			    if(nc > 10)nc = 10;
				ll.Count=nc;
			    for(n=0;n<nc;++n)ll.lp[n]=s->DList.lAttributes[n];
			}	
		}else{
			if(mm.infoSave.directory)cFree((char *)mm.infoSave.directory);
			mm.infoSave.directory=NULL;
			if(mm.infoLoad.directory)cFree((char *)mm.infoLoad.directory);
			mm.infoLoad.directory=NULL;
			mm.Synchronize=0;
			mm.eraseRectSize=0;
			mm.framesPerSecond=30;
			
			
			uSetRect(&ss.windowRect,117,45,720,480);
			ss.ActiveControl=ControlArrow;
			ss.LoopBackAndForth=0;
			ss.LoopFlag=0;

			ss.BackGround=TRUE;
			ss.GridLines=FALSE;
			ss.AntiAlias=FALSE;
			ss.GridDelta=72;
			
			ss.SnapTo=FALSE;
			ss.vSnapTo=0.125;
			
			ss.scale=1.0;
			ss.ixmax=720;
			ss.iymax=480;
			
			ss.CurrentFrame=0;
						
			ss.DList.Attributes.font.txFont=DefaultFont;
			ss.DList.Attributes.font.txFace=0;
			ss.DList.Attributes.font.txSize=DefaultFontSize;
			
			ll.Count=0;
						
		}
		ss.defaultsSet=TRUE;
		
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;		
		doUniversalDraw(myIcon,universal_GetDefaults,NULL);		
	}else if(flag == universal_GetDefaults){		
	    if(s && myIcon){
			if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;				
			uMoveWindow(myIcon,s->windowRect.x,s->windowRect.y);
			uSizeWindowGlobal(myIcon,s->windowRect.xsize,s->windowRect.ysize);
			s->DList.xsize=s->ixmax;
			s->DList.ysize=s->iymax;
	    	DOSetDrawCanvas(&s->DList);
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
			if(ll.Count > 0){
				struct DOList *l;
				l=&s->DList;
			    if(ll.Count > l->lineAttributesCount){
			        l->lineAttributesCount=0;
					for(n=0;n<ll.Count;++n){				
						if(ResizeArray(&l->lAttributes,sizeof(struct uAttributes),l->lineAttributesCount,&l->lineAttributesMax,2114))return 0;	
						l->lAttributes[l->lineAttributesCount++]= ll.lp[n];	
					}
				}else{
					for(n=0;n<ll.Count;++n){				
						l->lAttributes[n]= ll.lp[n];
					}
				}
			}
	    }
	}else if(flag == universal_Stop){	
		if(freeUniversalTable(&Global))goto ErrorOut;				
	}else{
		sprintf(WarningBuff,"doUniversalDraw unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
int GetFlipDirection(int *direction)
{
	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",     {10,205,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {100,205,65,25},uDialogButton,uDialogNone},
      
      /*  2 */      {"None",   {10, 25,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"X",      {10, 65,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"Y",      {10,105,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"X+Y",    {10,145,225,25},uDialogButtonRadio,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,200,250};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
    
	if(!direction)return 1;
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Flip Direction",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,2L,
		          uDialogSetLogical,(*direction == 0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(*direction == 1),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(*direction == 2),
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,(*direction == 3),
	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;


	if(uR[2].lreturn){
	    *direction = 0;
	}else if(uR[3].lreturn){
	    *direction = 1;
	}else if(uR[4].lreturn){
	    *direction = 2;
	}else if(uR[5].lreturn){
	    *direction = 3;
	}else{
	    *direction = 0;
	}
		
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);	
	
	return ret;
}
int GetRotateDirection(int *direction)
{
	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",     {10,205,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {100,205,65,25},uDialogButton,uDialogNone},
      
      /*  2 */      {"None",   {10, 25,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"90",     {10, 65,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"180",    {10,105,225,25},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"270",    {10,145,225,25},uDialogButtonRadio,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,200,250};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
    
	if(!direction)return 1;
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Rotate Direction",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,2L,
		          uDialogSetLogical,(*direction == 0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(*direction == 1),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(*direction == 2),
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,(*direction == 3),
	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;


	if(uR[2].lreturn){
	    *direction = 0;
	}else if(uR[3].lreturn){
	    *direction = 1;
	}else if(uR[4].lreturn){
	    *direction = 2;
	}else if(uR[5].lreturn){
	    *direction = 3;
	}else{
	    *direction = 0;
	}
		
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);	
	
	return ret;
}
static int SetImageRects(IconPtr myIcon)
{
	static unsigned char pict10040[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0xff,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0xff,0x00,
			0xff,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0xff,
			0x00,0xff,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0xff,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
		};

	static unsigned char pict10039[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10038[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10037[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10036[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10035[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10034[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10033[]=
		{
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,

		};

	static unsigned char pict10032[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10031[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10030[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10029[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10028[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10027[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		};

	static unsigned char pict10026[]=
		{
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,

		};

	static unsigned char pict10025[]=
		{
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,

		};

	static unsigned char pict10024[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10023[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0x00,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10022[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10021[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10020[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10019[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0x00,0xff,0x00,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0xff,0xff,0xff,0x00,0xff,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		};

	static unsigned char pict10018[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		};

	static unsigned char pict10017[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		};

	static unsigned char pict10016[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10015[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10014[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10013[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10012[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


		};

	static unsigned char pict10011[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,
			0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10010[]=
		{

			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
		};

	static unsigned char pict10009[]=
		{
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10008[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10007[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10006[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10005[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10004[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};

	static unsigned char pict10003[]=
		{
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
			0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,
			0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,
			0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		};

	static unsigned char pict10002[]=
		{
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		};

	static unsigned char pict10001[]=
		{
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		};

	
	static unsigned char Text[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,
			0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
			0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,
			0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,
			0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,
			0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		};
	static unsigned char Arrow[]=
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0xff,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0xff,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		};
	static unsigned char Line[]=
		{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
		};
		
		
	static unsigned char Box[]=
	
		{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};
		
		
	static unsigned char Oval[]=
		{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
		0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
		0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};
		
		
	static unsigned char RoundRect[]=
		{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
		0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
		0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
		0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		};
		
	
		
  	static uPattern Pat={{0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44}};
  	
  	unsigned char *picts[]={
  	                        pict10001,pict10002,pict10003,pict10004,pict10005,
  	                        pict10006,pict10007,pict10008,pict10009,pict10010,
  	                        pict10011,pict10012,pict10013,pict10014,pict10015,
  	                        pict10016,pict10017,pict10018,pict10019,pict10020,
  	                        pict10021,pict10022,pict10023,pict10024,pict10025,
  	                        pict10026,pict10027,pict10028,pict10029,pict10030,
  	                        pict10031,pict10032,pict10033,pict10034,pict10035,
  	                        pict10036,pict10037,pict10038,pict10039,pict10040,
  	                        
  	                        };
	DOListPtr l;
	drawPtr s;
	int ret;
	int i,j,n;
		
	if(!myIcon)return 1;
	s=(drawPtr)myIcon->DWindow;
	if(!s)return 1;
	l=&s->DList;
	
	ret=1;
	
	n=0;
	for(j=0;j<5;++j){
	    for(i=0;i<8;++i){
	         l->Picts[n].type=ScreenDataType8;
	         l->Picts[n].box.x =i*12+5;
	         l->Picts[n].box.y =j*12+10;
	         l->Picts[n].data=picts[n];
	         l->Picts[n].box.xsize =11;
	         l->Picts[n].box.ysize =11;
	         ++n;
	    }
	}
	s->ArrowImage.type=ScreenDataType8;
	s->ArrowImage.box.x=5;
	s->ArrowImage.box.y=5;
	s->ArrowImage.box.xsize=16;
	s->ArrowImage.box.ysize=16;
	s->ArrowImage.data=Arrow;
	
	s->ArrowRect.x=0;
	s->ArrowRect.y=myIcon->mMenuOffSet;
	s->ArrowRect.xsize=26;
	s->ArrowRect.ysize=26;
	
	s->ArrowImageHigh.type=ScreenDataType8;
	if(uImageRectStart(&s->ArrowImageHigh,&s->ArrowRect))goto ErrorOut;	
	uImageRectMerge(&s->ArrowImageHigh,&s->ArrowImage);		
	uImageRectPattern(&s->ArrowImageHigh,&Pat,myIcon);
	
	s->ArrowImage.box.y += myIcon->mMenuOffSet;
	
	
	s->TextImage.type=ScreenDataType8;
	s->TextImage.box.x=5;
	s->TextImage.box.y=5;
	s->TextImage.box.xsize=16;
	s->TextImage.box.ysize=16;
	s->TextImage.data=Text;
	
	s->TextRect.x=0;
	s->TextRect.y=6*26+myIcon->mMenuOffSet;
	s->TextRect.xsize=26;
	s->TextRect.ysize=26;

	s->TextImageHigh.type=ScreenDataType8;
	if(uImageRectStart(&s->TextImageHigh,&s->TextRect))goto ErrorOut;	
	uImageRectMerge(&s->TextImageHigh,&s->TextImage);	
	uImageRectPattern(&s->TextImageHigh,&Pat,myIcon);
	
	s->TextImage.box.y += 6*26+myIcon->mMenuOffSet;
	
	
	s->LineImage.type=ScreenDataType8;
	s->LineImage.box.x=5;
	s->LineImage.box.y=5;
	s->LineImage.box.xsize=16;
	s->LineImage.box.ysize=16;
	s->LineImage.data=Line;
	
	s->LineRect.x=0;
	s->LineRect.y=4*26+myIcon->mMenuOffSet;
	s->LineRect.xsize=26;
	s->LineRect.ysize=26;

	s->LineImageHigh.type=ScreenDataType8;
	if(uImageRectStart(&s->LineImageHigh,&s->LineRect))goto ErrorOut;	
	uImageRectMerge(&s->LineImageHigh,&s->LineImage);	
	uImageRectPattern(&s->LineImageHigh,&Pat,myIcon);
	
	s->LineImage.box.y += 4*26+myIcon->mMenuOffSet;
	
	
	s->BoxImage.type=ScreenDataType8;
	s->BoxImage.box.x=5;
	s->BoxImage.box.y=5;
	s->BoxImage.box.xsize=16;
	s->BoxImage.box.ysize=16;
	s->BoxImage.data=Box;
	
	s->BoxRect.x=0;
	s->BoxRect.y=2*26+myIcon->mMenuOffSet;
	s->BoxRect.xsize=26;
	s->BoxRect.ysize=26;

	s->BoxImageHigh.type=ScreenDataType8;
	if(uImageRectStart(&s->BoxImageHigh,&s->BoxRect))goto ErrorOut;	
	uImageRectMerge(&s->BoxImageHigh,&s->BoxImage);	
	uImageRectPattern(&s->BoxImageHigh,&Pat,myIcon);
	
	s->BoxImage.box.y += 2*26+myIcon->mMenuOffSet;
	
	
	s->OvalImage.type=ScreenDataType8;
	s->OvalImage.box.x=5;
	s->OvalImage.box.y=5;
	s->OvalImage.box.xsize=16;
	s->OvalImage.box.ysize=16;
	s->OvalImage.data=Oval;
	
	s->OvalRect.x=0;
	s->OvalRect.y=3*26+myIcon->mMenuOffSet;
	s->OvalRect.xsize=26;
	s->OvalRect.ysize=26;

	s->OvalImageHigh.type=ScreenDataType8;
	if(uImageRectStart(&s->OvalImageHigh,&s->OvalRect))goto ErrorOut;	
	uImageRectMerge(&s->OvalImageHigh,&s->OvalImage);	
	uImageRectPattern(&s->OvalImageHigh,&Pat,myIcon);
	
	s->OvalImage.box.y += 3*26+myIcon->mMenuOffSet;
	
	
	s->RoundRectImage.type=ScreenDataType8;
	s->RoundRectImage.box.x=5;
	s->RoundRectImage.box.y=5;
	s->RoundRectImage.box.xsize=16;
	s->RoundRectImage.box.ysize=16;
	s->RoundRectImage.data=RoundRect;
	
	s->RoundRectRect.x=0;
	s->RoundRectRect.y=5*26+myIcon->mMenuOffSet;
	s->RoundRectRect.xsize=26;
	s->RoundRectRect.ysize=26;

	s->RoundRectImageHigh.type=ScreenDataType8;
	if(uImageRectStart(&s->RoundRectImageHigh,&s->RoundRectRect))goto ErrorOut;	
	uImageRectMerge(&s->RoundRectImageHigh,&s->RoundRectImage);	
	uImageRectPattern(&s->RoundRectImageHigh,&Pat,myIcon);
	
	s->RoundRectImage.box.y += 5*26+myIcon->mMenuOffSet;
	
	
	uSetRect(&s->LineWidthRect,5,245+myIcon->mMenuOffSet,16,16);

	uSetRect(&s->LineForeRect,2,215+myIcon->mMenuOffSet,10,10);
	uSetRect(&s->LineBackRect,14,215+myIcon->mMenuOffSet,10,10);
	uSetRect(&s->LinePatternRect,2,228+myIcon->mMenuOffSet,22,15);
	
	uSetRect(&s->SolidForeRect,2,185+myIcon->mMenuOffSet,10,10);
	uSetRect(&s->SolidBackRect,14,185+myIcon->mMenuOffSet,10,10);
	uSetRect(&s->SolidPatternRect,2,198+myIcon->mMenuOffSet,22,15);
	
	ret=0;
ErrorOut:
	return ret;
}


#ifdef Macintosh
static int iPrint;
static void tPrint(IconPtr myIcon)
{
	extern Rect PageView;
	uRect sPageView;
	DOListPtr l;
	drawPtr t;
	long n;
	
	iPrint=FALSE;
	if(myIcon && (t=(drawPtr)myIcon->DWindow)){
	    l=&t->DList;
		iPrint=TRUE;
		for(n=0;n<l->oCount;++n){
		    DObjPtr Current;	
		    if((Current = l->oCurrent[n]) == NULL)continue;
		    if(Current->type != GridPlotType && Current->type != AlphaType && Current->type != TekPlotType){
				iPrint=FALSE;
				break;
		    }
		}		
	}
		
	sPageView=uRectConvert(&PageView);
	
	tDumpItP(myIcon,&sPageView,1);	
	iPrint=FALSE;
}
static void tDumpItP(IconPtr myIcon,uRect *PageViewi,int scale)
{
	extern void cLineSet(int num,int dem);
	Rect PageView;
	Rect DRect;
	uRect uDRect;
	double saveScale;
	drawPtr t;
	
	if(myIcon && (t=(drawPtr)myIcon->DWindow)){
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
		if(iPrint){
		    t->DList.PrintRect=uDRect;
		    t->DList.PrintFlag=TRUE;
			DODraw(&t->DList,t->CurrentFrame);		    
		    t->DList.PrintFlag=FALSE;
		    
		}else{		
	    	drawTek2(myIcon,t->CurrentFrame,&uDRect);
	    }
	    
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
