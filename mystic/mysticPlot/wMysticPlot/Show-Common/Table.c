#define EXTERN22 extern
#include "Xappl.h"
#include "uDialog.h"
#include "uFiles.h"
#include "FileManager.h"
#include "Message.h"
#include "Table.h"
#include "Universal.h"

#define breakCommand		4	
#define FileMenu2     		703
#define newListCommand 		1
#define openListCommand 	2
#define closeListCommand 	3
#define saveListCommand 	5
#define saveasListCommand 	6
#define quitListCommand 	8


#define EditMenu2     				704
#define undoListCommand 			1
#define cutListCommand 				3
#define copyListCommand 			4
#define pasteListCommand 			5
#define clearListCommand 			6
#define InfoListCommand 			8
#define SendPaletteCommand 			9


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

#define ToolsMenu     				706
#define pickListCommand 			1
#define SyncRegions 				2
#define NewMenu2					114

int SynchronizePalettes(IconPtr myIcon,unsigned char *red,unsigned char *green,unsigned char *blue);

static int TablePutData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd);

static int TableGetData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd);

static int SetFrame(IconPtr myIcon);

static int TableGetMessage(IconPtr myIcon,long MessageType,void *MessageData);

static int IntLocC(IconPtr myIcon,double *x,double *y,int ixx,int iyy);

static int IntLoc(IconPtr myIcon,double *x,double *y,int ix,int iy);

static int DrawIt(IconPtr myIcon);

static int nMouseMoved(IconPtr myIcon,uPoint where);

static int TableClose(IconPtr myIconIn);

static int doTableStart(IconPtr myIcon);

static int TableMenu(IconPtr myIcon,int menu,int item);

static int TableCheckMenu(IconPtr myIcon,int flag);

static int SetToolMenu(IconPtr myIcon);

static IconPtr doLoadImages(IconPtr myIconIn);

static void tMoveControls(IconPtr myIcon);

static int tInContent(IconPtr myIcon);

static int UpdateWrite(IconPtr myIcon);

static int TableNullEvent(IconPtr myIcon);

static int KeyBoard(IconPtr myIcon,int c);

IconPtr TableWindow2(int XCount,int YCount,int XLength,int YLength);

static long tCount;	

IconPtr TableWindow2(int XCount,int YCount,int XLength,int YLength)
{
	IconPtr myIcon;
	TablePtr t;
	
	myIcon=TableWindow(NULL);
	
	if(!myIcon)goto ErrorOut;
	t=(TablePtr)myIcon->DWindow;
	if(!t)goto ErrorOut;
	if(!t->Spread)goto ErrorOut;
	
	if(SpreadConfigure(t->Spread,XCount,YCount,
                       YLength,XLength,SpreadTypeFloat))goto ErrorOut;

	return myIcon;
ErrorOut:
	if(myIcon)TableClose(myIcon);
	return NULL;
	
}

IconPtr  NewTableWindow(IconPtr myIconIn)
{
	return doLoadImages(myIconIn);
}

static IconPtr TableDoOpen(struct FileList *Files);

static IconPtr TableDoOpen(struct FileList *Files)
{
	struct SetFrameData sd;
	char file[2048];
	unsigned char *bp;
	FILE *inout;
	int n,m,c;
	long length,blength;
	long xsize,ysize;
	double *data;
	IconPtr p;
	int itWas;
	
	if(!Files || !Files->directory ||
	   !Files->name || !Files->name[0])return NULL;
	
	p=NULL;

	mstrncpy(file,Files->directory,2048);
	strncatToPath(file,Files->name[0],2048);

	blength=100000;
	if(SetBuffers(blength))goto ErrorOut;
	
	if((inout=fopen(file,"rb")) == NULL){
		sprintf(WarningBuff,"Cannot open file : %s to read%c\n",file,0);
		WarningBatch(WarningBuff);
		goto ErrorOut;;
	}

	itWas = -7777;
	xsize=0;
	ysize=0;
	while((m=(int)fread(xg.buffin,1,blength,inout)) > 0){
		bp=(unsigned char *)xg.buffin;
		for(n=0;n<m;++n){
			c = *bp++;
			if((c == '\t') && (ysize == 0))++xsize;
			if(c == '\r')++ysize;
			if((c == '\n') && (itWas != '\r'))++ysize;
			itWas = c;			
		}
	}

	rewind(inout);
	++xsize;
	length=xsize*ysize;
	data=(double *)cMalloc(length*sizeof(double),8221);
	if(!data){
		WarningBatch("TableDoOpen Out Of Memory\n");
		goto ErrorOut;
	}
	
	for(n=0;n<length;++n){
		fscanf(inout,"%lf",&data[n]);
	}

	p=TableWindow2((int)xsize,(int)ysize,85,16);
	if(p == NULL){
		sprintf(WarningBuff,"TableDoOpen - Error Opening Table\n");
		WarningBatch(WarningBuff);
		goto ErrorOut;    	 
	}
	

	zerol((char *)&sd,sizeof(struct SetFrameData));

	sd.xsize=xsize;
	sd.ysize=ysize;
	sd.data=data;
	sd.type = FRAME_DATA_FLOAT;

	sd.CurrentFrame=0;	    			    	
	if(SendMessageByIcon(p,MessageType_SET_REGION_DATA,&sd)){	        	        
		sprintf(WarningBuff,"TableDoOpen - Error Puting Table Data\n");
		WarningBatch(WarningBuff);
		goto ErrorOut;
    }
	
	SetWindowName(p,Files->name[0]);
	
ErrorOut:	
	return p;
}
static IconPtr doLoadImages(IconPtr myIconIn)
{
	struct FileInfo2 *Files2;
	struct FileList *Files;
	int TypeList[]={HDF_DISK,HDF_MEMORY,SDS2D_DISK,SDS2D_MEMORY,
	                TEXT_MEMORY,
					0};
	
	IconPtr myIcon;

	Files=uOpenFileDialog(myIconIn,TypeList);		
	if(!Files)return NULL;
	
	if(Files->type == TEXT_MEMORY){
	    return TableDoOpen(Files) ;
	}
		
	    
	Files2=FilesOpenList(Files);
	if(!Files2)return NULL;
	
	myIcon=TableWindow(Files2);
	
	if(myIcon){
    	SetWindowToFileName(myIcon,Files2);
    }
    
    return myIcon;

}

IconPtr  TableWindow(struct FileInfo2 *Files)
{
	static struct uMenuItems File[]={
	    {"File",  			FileMenu2,uWindowStandardMenu,0,},
	    {"New       /N",	newListCommand,0,0,},
	    {"Open...   /O",	openListCommand,0,0,},
	    {"Close     /W",	closeListCommand,0,0,},
	    {"------------", 	newListCommand,0,0,},
	    {"Save      /S",	saveListCommand,0,0,},
	    {"Save as...  ",	saveasListCommand,0,0,},
	    {"------------", 	newListCommand,0,0,},
	    {"Quit      /Q",	quitListCommand,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Edit[]={
	    {"Edit",  			EditMenu2,uWindowStandardMenu,0,},
	    {"(Undo", 			undoListCommand,0,0,},
	    {"----------------",newListCommand,0,0,},
	    {"Cut           /X",cutListCommand,0,0,},
	    {"Copy          /C",copyListCommand,0,0,},
	    {"Paste         /V",pasteListCommand,0,0,},
	    {"Clear         /K",clearListCommand,0,0,},
	    {"----------------",newListCommand,0,0,},
	    {"Information.../I",InfoListCommand,0,0,},
	    {"Send As Palette ",SendPaletteCommand,0,0,},
	    
	    
	    {NULL,0,0,0,},
	};
	
	
	static struct uMenuItems Animation[]={
	    {"Animation",  			AnimationMenu4,uWindowStandardMenu,0,},
	    {"Forward", 			animateTekForward,0,0,},
	    {"Backward",			animateTekBackward,0,0,},
	    {"Step Forward",		animateTekStepForward,0,0,},
	    {"Step Backward",		animateTekStepBackward,0,0,},
	    {"Stop",				animateTekStop,0,0,},
	    {"-------------------",	newListCommand,0,0,},
	    {"Loop",				loopTek,0,0,},
	    {"Loop Back and Forth",	loopBackTek,0,0,},
	    {"Options...",			optionsTek,0,0,},	    
	    {"Synchronize",			synchronizeTek,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Tools[]={
	    {"Tools",  			ToolsMenu,uWindowStandardMenu,0,},
	    {"Pick", 			pickListCommand,0,0,},
	    {"Sync Regions",	SyncRegions,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Animation},
	    {Tools},
	    {NULL}
	};
	
    
 	static uRect  Rect1   = {117,45,575,434};
	uRect Rect2;
 	
    struct SetFrameData sd;    
 	
	IconPtr myIcon;
	char buff[256];	
	char buff2[256];
	TablePtr t;	
 	uPort Port;
 	int ret;
		
	ret=1;
	Rect2=Rect1;
	myIcon=NULL;
	
	uGetPort(&Port);


	sprintf(buff,"TableWindow.%03ld",tCount++);

	if(Files){
	    mstrncpy(buff2,buff,255);
	    mstrncat(buff2,".b",255);
	    if(FileManagerAddToList(Files,buff2))goto ErrorOut;
		if(SendMessageByName(buff2,MessageType_GET_INFORMATION,&sd)){
			goto ErrorOut;
		}
		Rect2.xsize=(int)(sd.raster.xsize);
		Rect2.ysize=(int)(sd.raster.ysize);	
	}	

	if(!(myIcon=uCreateIcon(TableType,&Rect2))){
		WarningBatch("TableWindow uCreateIcon Failed\n");
		goto ErrorOut;
	}

	uDisposeControl(myIcon,myIcon->VControl);
	myIcon->VControl=NULL;
	uDisposeControl(myIcon,myIcon->HControl);
	myIcon->HControl=NULL;

	if(uSetWindow(myIcon,	       
              uWindowSetWindowName,buff,
			  uWindowSetMenuList,menuList,
              uWindowSetDone,uWindowSetDone)
	)goto ErrorOut;
	
	if(!(t=(TablePtr)cMalloc((long)sizeof(*t),8163))){
	    WarningBatch("TableWindow Out of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)t,sizeof(*t));
	
	myIcon->DWindow=(struct DumWindow *)t;

	myIcon->pClose=TableClose;
	myIcon->pCheckMenu=TableCheckMenu;
	myIcon->pMenu=TableMenu;
	myIcon->pUpdateWrite=UpdateWrite;
	myIcon->pMoveControls=tMoveControls;
	myIcon->pInContent=tInContent;
	myIcon->pKeyBoard=KeyBoard;
	myIcon->pMessage=TableGetMessage;
	myIcon->pNullEvent=TableNullEvent;
	myIcon->pGetData=TableGetData;
	myIcon->pPutData=TablePutData;
	myIcon->pMouseMoved=nMouseMoved;
	
	myIcon->eraseRectSize=120;
	
	myIcon->framesPerSecond=30;
	t->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);
	t->CurrentFrame=0;
	
		
	t->XCount=Rect2.xsize;
	t->YCount=Rect2.ysize;
	t->XLength=80;
	t->YLength=16;
	t->Tool=SyncRegions;

	if(Files){
	    mstrncpy(t->BatchName,buff2,255);
	    t->ImageCount=Files->ImageCount;
	    t->Files=Files;
		t->raster  = sd.raster;
		t->range  = sd.range;
		t->vector = sd.vector;
		t->pd=sd.pd;
		if(t->sd.pioName[0]){
    		mstrncpy(t->pioName,t->sd.pioName,255);
    		t->pioIndex=t->sd.pioIndex;			    
		}					
	}
	
	if(doTableStart(myIcon))goto ErrorOut;

	ret=0;
ErrorOut:

	if(ret){
    	myIcon=NULL;
	}else{
		if(myIcon)myIcon->isOpen=TRUE;
	}
	
	uSetPort(&Port);

	return myIcon;
}
static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
    struct FileInfo2 *Files;
	TablePtr t;

	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	Files=t->Files;
  	if(!Files)return 1;
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {		
	    uSetCursor(myIcon,uARROW_CURSOR);
	}else{
	    uSetCursor(myIcon,uARROW_CURSOR);
	}		
	
	return 0;	
}
static int TablePutData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	TablePtr t;
	uPort save;
	int ret;

	if(!myIcon || !sd)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	
	ret=1;
	uGetPort(&save);
	
	SetPortMyWindow(myIcon);

	if(sd->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"RasterPutData - Only Puts FRAME_DATA_FLOAT");
	    Warning(WarningBuff);
	    goto ErrorOut;
	}
	
	sd->CurrentFrame=CurrentFrame;
	if(SendMessageByName(t->BatchName,MessageType_SET_REGION_DATA,sd)){	        	        
	    sprintf(WarningBuff,"RasterPutData - Window %s Error Puting Data\n",t->BatchName);
	    WarningBatch(WarningBuff);
	   goto ErrorOut;
	}

	t->CurrentFrame=CurrentFrame;

	ret=0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
static int TableGetData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	TablePtr t;
	int ret;

	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	
	sd->CurrentFrame=CurrentFrame;
	
	ret=SendMessageByName(t->BatchName,MessageType_GET_REGION_DATA,sd);

	myIcon->limits=sd->limits;
	
	return ret;
}
static int TableNullEvent(IconPtr myIcon)
{
    struct FileInfo2 *Files;
	TablePtr t;

	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
		
    if(!(t->AnimationOn) || !(t->ImageCount))return 1;
	    
	Files=t->Files;
  	if(!Files)return 1;
  	
    if(TickCount() > t->TimeToFlip){
       t->CurrentFrame += t->AnimationOn;
       if(t->CurrentFrame >= t->ImageCount){
           if(t->LoopBackForth){
               t->CurrentFrame=t->ImageCount-1;
               t->AnimationOn = -1;
           }else{
               t->CurrentFrame=0;
           }
       }else if(t->CurrentFrame < 0){
           if(t->LoopBackForth){
               t->CurrentFrame=0;
               t->AnimationOn = 1;
           }else{
               t->CurrentFrame=t->ImageCount-1;
           }
       }
       t->TimeToFlip = TickCount() + t->milliSecondsPerFrame*60/1000;
	   SetFrame(myIcon);
       InvalRectMyWindow(myIcon);
       
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
static int SetFrame(IconPtr myIcon)
{
	TablePtr t;

	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;

	if(t->ImageCount <= 0){
	    return 1;
	}

	if(!t->Files){
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
static int RealLoc(IconPtr myIcon,double *x,double *y,int *ix,int *iy)
{
    struct SetFrameData  *sd;
   	struct LIMITS *limits;
	TablePtr s;

	if(!myIcon || !x || !y || !ix || !iy)return 1;
	s=(TablePtr)myIcon->DWindow;
	if(!s)return 0;
	sd = &s->sd;
	limits=&sd->limits;


	*ix=(int)((*x-limits->xmin)*(double)sd->xsize/(limits->xmax-limits->xmin)+0.5);
	*iy=(int)((*y-limits->ymin)*(double)sd->ysize/(limits->ymax-limits->ymin)+0.5);
	
	
	return 0;
}
static int doSyncFrame(IconPtr myIcon)
{		
    struct SetFrameData  *sd;
    int x1,y1,x2,y2;
	TablePtr t;
	uRect r;

	
	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	sd = &t->sd;
	
	if(t->Tool != SyncRegions)return 0;
	
	x1=y1=x2=y2=0;
	
	if(t->sync.xminData >= t->sync.xmaxData)return 0;
	if(t->sync.yminData >= t->sync.ymaxData)return 0;
	
	RealLoc(myIcon,&(t->sync.xminData),&(t->sync.yminData),&x1,&y2);
	RealLoc(myIcon,&(t->sync.xmaxData),&(t->sync.ymaxData),&x2,&y1);

	y1=(int)(sd->ysize-y1);	
	y2=(int)(sd->ysize-y2);	
	
	r.x=x1;
	r.xsize=x2-x1;
	r.y=y1;
	r.ysize=y2-y1;
	
	SpreadSetRegion(t->Spread,&r);
	
	DrawIt(myIcon);
	
	return 0;

}
static int TableGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct PlotRangeData *pio;
	struct SetFrameData  *sd;
	struct SetFrameData   sd2;
	struct Message1000 *m;
	struct SyncStruct *sync;
	struct linedata *li;
	struct areadata *ai;
	struct LIMITS *limits;
	struct LIMITS *limitl;
	char windowName[256];
	char buff[256];
	uPort save;
	TablePtr s;
	long n;
	int ret;
	int setSDS2D;

	if(!myIcon)return 1;
	s=(TablePtr)myIcon->DWindow;
	if(!s)return 1;
	pio = &s->range;
	m=(struct Message1000 *)MessageData;
	
	
	setSDS2D=FALSE;
	if(MessageType == MessageType_SET_REGION_DATA){
	    if(!s->Files){
        	s->Files=FilesSDS2D();
        	if(!s->Files)return 1;
			sprintf(buff,"TableWindow.%03ld",tCount++);
			mstrncat(buff,".b",255);
			if(FileManagerAddToList(s->Files,buff))return 1;
		    mstrncpy(s->BatchName,buff,255);
			setSDS2D=TRUE;
	    }	
	}
	
	if(!s->Files)return 1;

	ret=1;
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	switch(MessageType){
	case MessageType_LIMITS:
	    if(!pio->receiveRange)goto OK;
	    if(!m)goto ErrorOut;
	    /*
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

	    pio->xminSet=1;
	    pio->xmaxSet=1;
	    pio->yminSet=1;
	    pio->ymaxSet=1;
	    
		if(pushStack(myIcon)){
		    goto ErrorOut;
		}
		
		
	    SetFrame(myIcon);
	    
    	uSizeWindow(myIcon,(int)(pio->ixmax+ScrollOffSet),(int)(pio->iymax+MenuOffSet+ScrollOffSet+myIcon->CursorSpace));	
    	
    	FlushCurrentPort();
    		    	    
	    s->BoundarySpace = -1;
	    
	    tMoveControls(myIcon);
	    
	    s->BoundarySpace = 0;
		*/
		break;
	case MessageType_SET_SYNC_REGION:
	    if(s->Tool != SyncRegions)goto OK;
		sync=(struct SyncStruct *)MessageData;
		if(!sync)goto ErrorOut;
		s->syncOld=s->sync;
	    s->sync = *sync;
	    doSyncFrame(myIcon);
	    break;
	case MessageType_SET_FRAME_NUMBER:
	    if(!myIcon->Synchronize)goto OK;
	    s->CurrentFrame=m->CurrentFrame;
	    SetFrame(myIcon);
	    break;
	case MessageType_SET_PALETTE:
		for(n=0;n<256;++n){
		    myIcon->red[n]=m->red[n];
		    myIcon->green[n]=m->green[n];
		    myIcon->blue[n]=m->blue[n];
		}
		break;
	case MessageType_GET_FRAME_COUNT:
	    if(s->Files){
		    if(s->Files->ImageCount > m->FrameCount)m->FrameCount=s->Files->ImageCount;
		}
		goto OK;
	case MessageType_GET_LINE_DATA:
	    li=MessageData;
	    ret = SendMessageByName(s->BatchName,MessageType_GET_LINE_DATA,li);
	    goto ErrorOut;
	case MessageType_GET_AREA_DATA:
	    ai=MessageData;
		ret=SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,ai);
		if(!ret)myIcon->limits=s->Files->limits;
	    goto ErrorOut;	    
	case MessageType_GET_REGION_DATA:
	    sd=MessageData;
	    if(myIcon->pGetData){
	    	ret = (*myIcon->pGetData)(myIcon,sd->CurrentFrame,sd);
	    }
	    goto ErrorOut;
	case MessageType_SET_AREA_RANGE:
	    ai=MessageData;
	    ret = SendMessageByName(s->BatchName,MessageType_SET_AREA_RANGE,ai);
	    goto ErrorOut;
	case MessageType_SET_REGION_DATA:
	    sd=MessageData;
	    if(myIcon->pPutData){
	    	ret = (*myIcon->pPutData)(myIcon,sd->CurrentFrame,sd);	
	    	if(ret){
	    	   ret = -1;
	    	}    	
			if(!ret && setSDS2D){
				doSDS2DStart(s->Files);
				if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&sd2)){
					goto ErrorOut;
				}
				
				s->range  = sd2.range;
				s->vector = sd2.vector;
				s->pd=sd2.pd;
				if(s->sd.pioName[0]){
		    		mstrncpy(s->pioName,s->sd.pioName,255);
		    		s->pioIndex=s->sd.pioIndex;			    
				}					
				s->raster  = sd2.raster;
				s->raster.xsize=s->range.ixmax;
				s->raster.ysize=s->range.iymax;

				s->ImageCount=s->Files->ImageCount;	    	
				/* doFrameImage(myIcon); */
			}
			
			s->ImageCount=s->Files->ImageCount;	    	
	    }
	    goto ErrorOut;
	case MessageType_GET_LIMITS_DATA:
	    limitl=MessageData;
	    if(!limitl)goto ErrorOut;
	    limits=NULL;
		FileGetLimits(&limits,limitl->CurrentFrame,s->Files);
		if(limits && limits->iGot){
		    limits->CurrentFrame=limitl->CurrentFrame;
		    *limitl = *limits;
		}
		break;
	case MessageType_GET_FILE_LIST:
		{
		    int isFound;
		    
			if(!s->Files)goto OK;
			
		    isFound=FALSE;
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
			}
			if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
			    m->FilesList[m->FileCount++]=s->Files;
			}
		}
		goto OK;
	case MessageType_GET_FILE_OWNER:
		for(n=0;n<m->FileCount;++n){
		    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
		}
		goto OK;
	case MessageType_PRINT_FILE_INFORMATION:
		GetWindowName(myIcon,windowName,0);
	    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
	    WarningBatch(WarningBuff);
	    SendMessageByName(s->BatchName,MessageType_PRINT_FILE_INFORMATION,m);
		goto OK;
	}
	UpdateWrite(myIcon);
OK:
	ret = 0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
static int KeyBoard(IconPtr myIcon,int c)
{
	TablePtr t;
	
	if(!myIcon || !(t=(TablePtr)myIcon->DWindow))return 1;

	switch(c){
	    case upArrow:
	        if(++(t->CurrentFrame) >= t->ImageCount)t->CurrentFrame = 0;
	        t->AnimationOn = 0;
	        break;
	    case downArrow:
	        if(--(t->CurrentFrame) < 0)t->CurrentFrame = t->ImageCount-1;
	        t->AnimationOn = 0;
	        break;
	    case leftArrow:
	        if(--(t->CurrentFrame) < 0)t->CurrentFrame = t->ImageCount-1;
	        t->AnimationOn = -1;
	        break;
	    case rightArrow:
	        if(++(t->CurrentFrame) >= t->ImageCount)t->CurrentFrame = 0;
	        t->AnimationOn = 1;
	        break;
	    default:
	    	return 0;
	}
	InvalRectMyWindow(myIcon);
	if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
	return 0;
}
static int tInContent(IconPtr myIcon)
{
	TablePtr t;
	
	if(!myIcon || !(t=(TablePtr)myIcon->DWindow))return 1;
	
	t->AnimationOn = 0;
	
	SpreadInContent(t->Spread);

	if(t->AnimationOn){	  
		t->AnimationOn=FALSE;
		InvalRectMyWindow(myIcon);
	}		      
	
	return 0;
}
static void tMoveControls(IconPtr myIcon)
{
	uRect eraseRect;
	TablePtr t;
	uRect r;
			
	if(!myIcon)return;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return;
	if(!t->Spread)return;
	
	uGetPortBounds(myIcon,&r);
	
	eraseRect.y=r.ysize-ScrollOffSet;
	eraseRect.x=0;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;
	
	t->Spread->eraseRectSize=myIcon->eraseRectSize;
	
	r.y      = MenuOffSet;
	r.ysize -= MenuOffSet;
	
	SpreadFitRect(t->Spread,&r);
	
	InvalRectMyWindow(myIcon);
}
static int UpdateWrite(IconPtr myIcon)
{
	uRect portRect;
	TablePtr t;	
	char *name;

	if(!myIcon || !myIcon->isOpen)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
		
    uGetPortBounds(myIcon,&portRect);
    uClipRect(&portRect,myIcon);
    /* uEraseRect(&portRect,myIcon); */

	DrawIt(myIcon);
	
    uClipRect(&portRect,myIcon);
    
    
	name=FilesImageName(t->Files,t->CurrentFrame);

	nWriteBox(myIcon,name);
   
    uDrawControls(myIcon);
    uDrawGrowIcon(myIcon);
    
    return 0;
}
static int DrawIt(IconPtr myIcon)
{
	struct SpreadStruct *Spread;
  	struct SetFrameData  *sd;
	struct FileInfo2 *Files;
	TablePtr s;	
	long n;

	if(!myIcon)return 1;
	s=(TablePtr)myIcon->DWindow;
	if(!s)return 1;
	Files=s->Files;
	if(!Files)return 1;
	sd=&s->sd;
	Spread=s->Spread;
	if(!Spread)return 1;

	if(sd->data)cFree((char *)sd->data);
	sd->data=NULL;

	zerol((char *)sd,sizeof(struct SetFrameData));
	
	sd->CurrentFrame=s->CurrentFrame;
	
	sd->raster  =  s->raster;
	sd->pd 	   =  s->pd;
	sd->pa 	   =  s->pa;
	
	if(s->pioName){
		mstrncpy(sd->pioName,s->pioName,255);
		sd->pioIndex=s->pioIndex;			    
	}						

	
	sd->type = FRAME_DATA_FLOAT;
	sd->typePlotRange = FRAME_RANGE_RASTER;
	
	if(SendMessageByName(s->BatchName,MessageType_GET_REGION_DATA,sd)){
		return 1;
	}
	
	s->range.ixmax=sd->xsize;
	s->range.iymax=sd->ysize;

	s->pd = sd->pd;

	myIcon->limits = sd->limits;
	
	if((sd->xsize ==  Spread->XCount) && (sd->ysize == Spread->YCount) && Spread->DataFloat && sd->data){	
		double *d;
		d=(double *)sd->data;
		for(n=0;n<sd->xsize*sd->ysize;++n){
		    Spread->DataFloat[n]=d[n];
		}
		SpreadUpdate(Spread);
	}
    
    return 0;
}
static int TablepSyncTrack(void *data,uRect *r)
{
	struct SyncStruct m;
	IconPtr myIcon;
	TablePtr t;	
	
	myIcon=(IconPtr)data;
	
	if(!myIcon || !r)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	
	if(t->Tool != SyncRegions)return 0;

	zerol((char *)&m,sizeof(struct SyncStruct));


	IntLocC(myIcon,&m.xminData,&m.ymaxData,r->x,r->y);

	IntLocC(myIcon,&m.xmaxData,&m.yminData,r->x+r->xsize,r->y+r->ysize);

	IntLocC(myIcon,&m.xCurrent,&m.yCurrent,r->x+r->xsize,r->y+r->ysize);
	
	IntLocC(myIcon,&m.xStart,&m.yStart,r->x,r->y);

	SendMessageToAll(myIcon,FALSE,MessageType_SET_SYNC_REGION,&m);
	
	return 0;
}
static int IntLocC(IconPtr myIcon,double *x,double *y,int ixx,int iyy)
{
    struct SetFrameData  *sd;
	TablePtr s;
	int iy,ix;

	if(!myIcon || !x || !y)return 1;
	s=(TablePtr)myIcon->DWindow;
	if(!s)return 0;
	sd = &s->sd;

	iy=(int)(sd->ysize-iyy);	
	ix=ixx;

	return IntLoc(myIcon,x,y,ix,iy);
}
static int IntLoc(IconPtr myIcon,double *x,double *y,int ix,int iy)
{
    struct SetFrameData  *sd;
   	struct LIMITS *limits;
	TablePtr s;

	if(!myIcon || !x || !y)return 1;
	s=(TablePtr)myIcon->DWindow;
	if(!s)return 0;
	sd = &s->sd;
	if((sd->xsize <= 0) || (sd->ysize <= 0))return 1;
	
	limits=&sd->limits;

	*x=(limits->xmax-limits->xmin)*((double)(ix)/(double)sd->xsize)+limits->xmin;
	*y=(limits->ymax-limits->ymin)*((double)(iy)/(double)sd->ysize)+limits->ymin;

	return 0;
}

static int doTableStart(IconPtr myIcon)
{
	struct SpreadStruct *Spread;
	int ret;
	TablePtr t;	
	uRect r;
		
	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	
	ret=1;
	
	uGetPortBounds(myIcon,&r);
		
	Spread=SpreadNew(&r,20,20,16,80,myIcon);
	
	if(SpreadConfigure(Spread,t->XCount,t->YCount,
                       t->YLength,t->XLength,SpreadTypeFloat))goto ErrorOut;

	t->Spread=Spread;
	
	
	SpreadSetSyncTrack(Spread,TablepSyncTrack,(void *)myIcon);
	
	tMoveControls(myIcon);
			
	ret=0;
ErrorOut:	

	return ret;
}
static int TableCheckMenu(IconPtr myIcon,int flag)
{
		
	if(flag){
	  	SetToolMenu(myIcon);
	    doWindowMenu();
	}
	
	return 0;
}
static int SetToolMenu(IconPtr myIcon)
{
    struct FileInfo2 *Files;
	TablePtr t;	
	
	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	Files=t->Files;
	
	if(Files){
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
	
	
	
	uCheckMenuItem(myIcon,ToolsMenu,pickListCommand, (t->Tool == pickListCommand));
	uCheckMenuItem(myIcon,ToolsMenu,SyncRegions, (t->Tool == SyncRegions));
	
	
	uCheckMenuItem(myIcon,AnimationMenu4,loopTek,t->LoopFlag);
	uCheckMenuItem(myIcon,AnimationMenu4,loopBackTek,t->LoopBackForth);	
	uCheckMenuItem(myIcon,AnimationMenu4,synchronizeTek,myIcon->Synchronize);
	
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
    
 	static uRect  Rectl   = {100,50,339,151};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	TablePtr t;
	int reti;
    int ret;
		
	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
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
	
	t->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);
     
    ret = 0;
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int ListdoSave(IconPtr myIcon,int nFlag)
{
	static char fName[256]={'t','e','s','t','.','e','x','p',0};
 	TablePtr w;
	double *data;
	FILE *inout;
	int x,y;
	
	if(!myIcon)return 1;
	w=(TablePtr)myIcon->DWindow;
	if(!w)return 1;
	if(!w->Spread)return 1;
	
	nFlag=nFlag;
	
	if(myIcon->FileSave){
		GetWindowName(myIcon,(char *)fName,0);
	}else{
	    mstrncpy(fName,"table.dat",255);
	}
			
	if(GetTheSaveFileName(fName,"Save List File",256,&myIcon->infoSave))return 1;
	
	if((inout=fopen(fName,"wb")) == NULL){
	    sprintf(WarningBuff,"Cannot open file : %s to write\n",fName);
	    Warning(WarningBuff);
	    return 1;
	}

	uSetCursor(myIcon,uWATCH_CURSOR);	
	if(w->Spread->DataFloat){
		data=w->Spread->DataFloat;
		for(y=0;y<w->Spread->YCount;++y){
		    for(x=0;x<w->Spread->XCount;++x){
				if(x < w->Spread->XCount-1){
					fprintf(inout,"%g\t",*data++);
				}else{
					fprintf(inout,"%g",*data++);
					putENDOFLINE(inout);
				}
			}
		}
	}
	uSetCursor(myIcon,uARROW_CURSOR);
	if(inout)fclose(inout);
	myIcon->FileSave=1;
	/* SetWindowTitle(myIcon->myWindow,fName);	*/
	if(SetFileTypeOwner(fName,"QED1","TEXT"))return 1;
	return 0;
}
static int doSendPalette(IconPtr myIcon)
{
    unsigned char red[256],green[256],blue[256];
	TablePtr w;
	double *data;
	int n;

	if(!myIcon)return 1;
	w=(TablePtr)myIcon->DWindow;
	if(!w || !w->Spread || !w->Spread->DataFloat || 
	   (w->Spread->YCount < 256) || (w->Spread->XCount < 3) )return 1;

	data=w->Spread->DataFloat;
	for(n=0;n<255;++n){
		red[n]   = (char)data[0];
		green[n] = (char)data[1];
		blue[n]  = (char)data[2];
		data += w->Spread->XCount;
	}
	
	SynchronizePalettes(myIcon,red,green,blue);

	return 0;
}
static int TableMenu(IconPtr myIcon,int menu,int item)
{
	TablePtr t;	

	if(!myIcon)return 1;
	t=(TablePtr)myIcon->DWindow;
	if(!t)return 1;
	
	uSetCursor(myIcon,uWATCH_CURSOR);
	
	switch(menu){
	case AppleMenu:
		if(item == 1){
		    doAbout(myIcon);
		    break;
		}
		break;
	case FileMenu2:
		if(item == newListCommand){
		    NewNewWindow(myIcon);
		} else if(item == openListCommand){
			if(doLoadImages(myIcon))break;
		} else if(item == closeListCommand){
		    if(myIcon && myIcon->pClose){
	    		(*myIcon->pClose)(myIcon);
		    }
		    myIcon=NULL;
		    goto OutOfHere;
		} else if(item == saveListCommand){
		   ListdoSave(myIcon,0);
		} else if(item == saveasListCommand){
		   ListdoSave(myIcon,1);
		} else if(item == quitListCommand){
		   doQuit();
		   myIcon=NULL;
		}
		break;
	case EditMenu2:
		switch(item){
 		case cutListCommand:	SpreadCut(t->Spread); break;
 		case copyListCommand:	SpreadCopy(t->Spread); break;
 		case pasteListCommand:	SpreadPaste(t->Spread); break;
 		case clearListCommand:	SpreadClear(t->Spread); break;
		case InfoListCommand:   doWindowInformation(myIcon);tMoveControls(myIcon);break;
		case SendPaletteCommand: doSendPalette(myIcon);break;
		}
		break;
	case AnimationMenu4:
		switch(item){
	    case animateTekForward:
	        t->AnimationOn = 1;
	        break;
	    case animateTekBackward:
	        t->AnimationOn = -1;
	        break;
 		case animateTekStepForward:
 			if(++(t->CurrentFrame) >= t->ImageCount)t->CurrentFrame = 0;
	        t->AnimationOn = 0;
 			break;
 		case animateTekStepBackward:
 			if(--(t->CurrentFrame) < 0)t->CurrentFrame = t->ImageCount-1;
	        t->AnimationOn = 0;
 			break;
 		case synchronizeTek:
 			myIcon->Synchronize = !myIcon->Synchronize;
 			 break;
	    case animateTekStop:
	        t->AnimationOn = 0;
	        break;
	    case loopTek:
	        t->LoopFlag = !t->LoopFlag;
	        break;
	    case loopBackTek:
	        t->LoopBackForth = !t->LoopBackForth;
	        break;	        
	    case optionsTek:
	        doOptions(myIcon);
	        break;	        
		}
		InvalRectMyWindow(myIcon);
		if(myIcon->Synchronize)Synchronize(myIcon,t->CurrentFrame);
		break;
	case ToolsMenu:
		switch(item){
 		case pickListCommand: t->Tool=pickListCommand; break;
 		case SyncRegions: t->Tool=SyncRegions; break;
 		case 3:	; break;

		}
		break;

	case WindowMenu:
		pWindowMenu(myIcon,item);
		break;

	case NewMenu2:
		pNewMenu(myIcon,item);
		break;
	}
OutOfHere:
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int TableClose(IconPtr myIcon)
{
	TablePtr t;
		
	if(myIcon){
	    SetPortMyWindow(myIcon);
	    t=(TablePtr)myIcon->DWindow;
   	    if(t){
   	    
 			if(t->sd.data)cFree((char *)t->sd.data);
			t->sd.data=NULL;
			
  	    	SpreadDispose(t->Spread);

			if(t->BatchName[0])FileManagerCloseName(t->BatchName);
			
			t->BatchName[0]=0;
			
		    t->Files=NULL;
		    	
	    	myIcon->DWindow=NULL;
	        cFree((char *)t);
	        
		}
	    myIcon=RemoveIcon(myIcon);
	    
	}
	return 0;

}
