#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "CExpress.h"
#include "NewEdit.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "Linedata.h"
#include "Universal.h"
#include "BatchFile.h"
#include "uGraph.h"

#ifdef __cplusplus
extern "C" {
#endif
	/* int chdir(char *pathname); */
#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) || defined(c_plusplus)
#define class c_class
#endif

char *strpathname(char *path,char *name);

static int showCursor(IconPtr myIcon);
static int nMouseMoved(IconPtr myIcon,uPoint pt);

int CExpressMenuInsert(IconPtr myIcon,int item);
int SetWindowName(IconPtr myIcon,char *name);
IconPtr CDrawNewLine(struct linedata *li,int count);

static int  EditdoAuto(IconPtr myIcon);
static int  EditSetFrame(IconPtr myIcon);
static int CEditdoOpen(IconPtr myIcon,struct FileList *FilesIn);
static int  EditdoExport(IconPtr myIcon);

long putStringToScrap(char *string);

static int UpdateWrite(IconPtr myIcon);
static int  EditSetFolder(IconPtr myIcon);
int CEditdoSave(IconPtr myIcon);

int EditRunAsBatch(IconPtr myIcon);


static int EditGetMessage(IconPtr myIcon,long MessageType,void *MessageData);

int CExpressMenu(menuPtr Function);
static int CVideodoOptions(IconPtr myIcon);
static int CVideodoGoToLine(IconPtr myIcon);



static int CVideodoOpen(IconPtr myIcon,struct FileList *files);
static void CVideoDumpIt(IconPtr myIcon);
static void CVideodoSelection(IconPtr myIcon);
static void CVideoDrawLine(IconPtr myIcon,int x,int y,char *Line,int n);
static void CVideoDrawChar(IconPtr myIcon,int c);
static void CVideoMoveDy(IconPtr myIcon);
static void CVideodoDownInView(IconPtr myIcon);
static void CVideoadjustHigh(IconPtr myIcon,long Row,long Column);
static void CVideoRowColumn(IconPtr myIcon,uPoint *pt,long *CursorRow,long *CursorColumn);
static int CVideoxColumn(IconPtr myIcon,short x,long row);
static void CVideoSetInsert(IconPtr myIcon,long Row,long Column);
static void CVideodoInsertDraw(IconPtr myIcon);
static void CVideoDeleteAtCursor(IconPtr myIcon);
static int CVideodoCopy(IconPtr myIcon);
static void CVideodoClear(IconPtr myIcon);
static void CVideodoSelectAll(IconPtr myIcon);
void CVideoDrawRow(IconPtr myIcon,long CursorRow);
static char *CVideogetSelection(IconPtr myIcon,long *nn);
static void CVideokillSelection(IconPtr myIcon);
static int CVideodoPaste(IconPtr myIcon);
static int CVideodoFind(IconPtr myIcon,int flag);
int Find(IconPtr myIconSearch);
int Change(IconPtr myIconSearch);
static int CEditpInContent(IconPtr myIcon);

int putString(unsigned char *data,long length,FILE *out);

static int CEditpKeyBoard(IconPtr myIcon,int c);

static int EditdoLineGetData(IconPtr myIcon,struct linedata *li);

static int  EditEvaluate(IconPtr myIcon);

#define FILE_MENU				2100
#define ABOUT_ITEM				0
#define NEW_ITEM				2
#define OPEN_ITEM				3
#define CLOSE_ITEM				4
#define savePreferencesCommand 	5
#define PreferencesCommand 		6
#define SAVE_ITEM				7
#define QUIT_ITEM				8


#define EDIT_MENU			2101
#define clearLoadTekCommand	0
#define WINDOW_OPTIONS_ITEM	1
#define CUT_ITEM			2
#define COPY_ITEM			3
#define PASTE_ITEM			4
#define CLEAR_ITEM			5
#define SELECTALL_ITEM		6
#define OPTIONS_ITEM		8




#define SEARCH_MENU			2102
#define FIND_ITEM			0
#define CHANGE_ITEM			1
#define GOTOLINE_ITEM		2


#define OPERATION_MENU		2103
#define EVALUATE_ITEM		0
#define SET_FRAME_ITEM		1
#define AUTO_ITEM			2
#define SET_OUTPUT_ITEM		3
#define EXPORT_ITEM			4
#define RUN_BATCH_ITEM		5


#define FUNCTION_MENU		2104




int InvalRectMyWindow(IconPtr myIcon);

static int cdoPageUp(controlPtr control,short part,IconPtr myIcon);
static int cdoPageDown(controlPtr control,short part,IconPtr myIcon);
static int cgoUp(controlPtr control,short part,IconPtr myIcon);
static int cgoDown(controlPtr control,short part,IconPtr myIcon);
static int cdothumb(controlPtr control,IconPtr myIcon);

int CEditCheckMenu(IconPtr myIcon,int flag);


int NewRastor(long count);
IconPtr NewTekType(struct FileInfo2 *files);
static void SetWindow(IconPtr myIcon);
int doEraseDown(IconPtr myIcon);
static int doMenus(IconPtr myIcon,int Menu,int Item);

static int doClose(IconPtr myIcon);
static int doNULL(IconPtr myIcon);

IconPtr uStartStatusWindow(IconPtr myIcon,char *header);

int uWriteStatusWindow(IconPtr StatusWindow,char *buff);
int uCheckStatusWindow(IconPtr StatusWindow,char *Message);

int uStopStatusWindow(IconPtr StatusWindow);

IconPtr NewCEdit(IconPtr myIconIn,struct FileList *files);

int FilesOpenEdit(struct FileInfo2 *Files);


IconPtr NewEditWindow(IconPtr myIcon);

#ifdef Mac_Test

static int test(IconPtr myIcon);


static int MyGetDeviceSettings (long myRefNum,
                            int *numChannels,
                            Fixed *sampleRate,
                            int  *sampleSize,
                            unsigned long *compressionType);

int MyRecordSnd (SndListHandle mySndH);

static int MyGetDeviceSettings (long myRefNum,
                            int *numChannels,
                            Fixed *sampleRate,
                            int  *sampleSize,
                            unsigned long *compressionType)
{

   OSErr myErr;
   
   if(!numChannels || !sampleRate || !sampleSize || !compressionType)return 1;

    /* Get number of active channels */
    myErr = SPBGetDeviceInfo (myRefNum, siNumberChannels, numChannels);
    /* Get sample rate*/
    myErr = SPBGetDeviceInfo(myRefNum, siSampleRate, sampleRate);
    /* Get sample size*/
    myErr = SPBGetDeviceInfo(myRefNum, siSampleSize, sampleSize);
    /* Get compression type*/
    myErr = SPBGetDeviceInfo(myRefNum, siCompressionType,compressionType);
    
    return 0;

}
int MyRecordSnd (SndListHandle mySndH)
{

    int kAsync = TRUE;
    unsigned char kMiddleC = 60;

    SPB mySPB;            /*a sound input parameter block*/
    long myInRefNum;      /*device reference number*/
    long myBuffSize;      /*size of buffer to record into*/
    short int myHeadrLen;       /*length of sound header*/
    int myNumChans;       /*number of channels*/
    int mySampSize;       /*size of a sample*/
    Fixed mySampRate;     /*sample rate*/
    OSType myCompType;    /*compression type*/
    OSErr myErr;
    
    if(!mySndH)return 1;

    /*Open the default input device for reading and writing.*/
    myErr = SPBOpenDevice(NULL, siWritePermission, &myInRefNum);

    if(myErr == noErr){
   
        /*Get current settings of sound input device*/
        MyGetDeviceSettings(myInRefNum, &myNumChans, &mySampRate,
                                    &mySampSize, &myCompType);

        /*Set up handle to contain the 'snd ' resource header*/
        
        myNumChans=1;
        mySampSize=16;
        myCompType=kSoundNotCompressed;
        
        myErr = SetupSndHeader(mySndH, myNumChans, mySampRate,mySampSize,
                                            myCompType, kMiddleC, 0, &myHeadrLen);

        /*Leave room in buffer for the sound resource header*/
        myBuffSize = GetHandleSize((Handle)mySndH) - myHeadrLen;

        /*Lock down the sound handle until the recording is over*/
        HLockHi((Handle)mySndH);

        /*Set up the sound input parameter block*/
            mySPB.inRefNum = myInRefNum;                     /*input device reference number*/
            mySPB.count = myBuffSize;                        /*number of bytes to record*/
            mySPB.milliseconds = 0;                          /*no milliseconds*/
            mySPB.bufferLength = myBuffSize;                 /*length of buffer*/
            mySPB.bufferPtr = (char *)(*mySndH) + myHeadrLen;
                                                             /*put data after 'snd ' header*/
            mySPB.completionRoutine = NULL;                   /*no completion routine*/
            mySPB.interruptRoutine = NULL;                    /*no interrupt routine*/
            mySPB.userLong = 0;                              /*no user data*/
            mySPB.error = noErr;                             /*clear error field*/
            mySPB.unused1 = 0;                               /*clear reserved field*/

        /*Record synchronously through the open sound input device.*/
        myErr = SPBRecord(&mySPB, !kAsync);

        HUnlock((Handle)mySndH);                                /*unlock the handle*/

        /* Indicate the number of bytes actually recorded*/
        myErr = SetupSndHeader(mySndH, myNumChans, mySampRate, mySampSize,
                                            myCompType, kMiddleC, mySPB.count,
                                            &myHeadrLen);
        {
        	short int *f=(short int *)mySPB.bufferPtr;
        	double dt,time,value,ff;
        	long count=mySPB.count/2;
        	int n;
        	
        	ff=261.626;
        	dt=(double)((unsigned long)mySampRate);
        	dt /= 65536;
        	dt = 1.0/dt;
        	time=-dt;
        	for(n=0;n<count;++n){
        	    time += dt;
        	    value= 5000*sin(2*3.1415926*ff*time);
        	    /*
        	    if(n > count/4 )value += 5000*sin(2*3.1415926*2*ff*time);
        	    if(n > count/2)value += 5000*sin(2*3.1415926*3*ff*time);
        	    if(n > count*3/4)value += 5000*sin(2*3.1415926*4*ff*time);
        	    */
        	    *f++ = value;
        	}
        }

        /*Close the input device*/
        myErr = SPBCloseDevice(myInRefNum);
    }
    return 0;
}


static int test(IconPtr myIcon)
{
	OSErr ret;
	SndListHandle mySnd;
	
	if(!myIcon)return 1;

	uSetCursor(myIcon,-uARROW_CURSOR);
	
	mySnd=(SndListHandle)NewHandle(200000);
	
	 ret=MyRecordSnd (mySnd);
	
	if(!ret){
		ret=SndPlay(NULL,mySnd,TRUE);
	}
	
	return 0;
}
/*
static int test(IconPtr myIcon)
{
	Point myCorner;
	OSErr ret;
	SndListHandle mySnd;
	
	if(!myIcon)return 1;

	uSetCursor(myIcon,-uARROW_CURSOR);
	
	SetPt(&myCorner,50,50);
	
	mySnd=NULL;
	
	ret=SndRecord(NULL,myCorner,siBestQuality,&mySnd);
	
	if(!ret){
		ret=SndPlay(NULL,mySnd,TRUE);
	}
	
	return 0;
}
*/

#endif
IconPtr NewEditWindow(IconPtr myIcon)
{
	return NewCEdit(myIcon,NULL);
}

int FilesOpenEdit(struct FileInfo2 *Files)
{
	struct FileList File;
	long n;

	if(!Files)return 1;

	for(n=0;n<Files->FileCount;++n){
	    zerol((char *)&File,sizeof(File));
	    File.donotfree=TRUE;
	    File.count=1;
	    File.directory=strsave(Files->directory,1986);
	    if(!File.directory)break;
	    File.name=cMalloc(sizeof(char **),1983);
	    if(!File.name)break;
	    File.name[0]=strsave(Files->FilesNames[n],1986);
	    if(!File.name[0])break;	    
	    File.type=TEXT_MEMORY;	    
	    NewEdit(&File);
	}

	if(Files)freeFileList(Files);

	return 0;
}

IconPtr NewEdit(struct FileList *files)
{
	return NewCEdit(NULL,files);
}

IconPtr NewCEdit(IconPtr myIconIn,struct FileList *files)
{
	static struct uMenuItems File[]={
	    {"File",  					FILE_MENU,uWindowStandardMenu,0,},
	    {"About...",				ABOUT_ITEM,0,0,},
	    {"----------------------",	9,0,0,},
	    {"New...              /N", 	NEW_ITEM ,0,0,},
	    {"Load Text...          ",	OPEN_ITEM,0,0,},
	    {"Close               /W",	CLOSE_ITEM,0,0,},
	    {"----------------------", 	9,0,0,},
	    {"Save Preferences",		savePreferencesCommand,0,0,},
	    {"Preferences...  ",		PreferencesCommand,0,0,},
	    {"----------------------", 	9,0,0,},	    
	    {"Save...              /S", SAVE_ITEM,0,0,},
	    {"----------------------", 	9,0,0,},
	    {"Quit                /Q",	QUIT_ITEM,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Edit[]={
	    {"Edit",  					EDIT_MENU,uWindowStandardMenu,0,},
	    {"Clear and Load...     ",  clearLoadTekCommand,0,0,},
	    {"Information...      /I",  WINDOW_OPTIONS_ITEM,0,0,},
	    {"----------------------", 	9,0,0,},
	    {"Cut                 /X",	CUT_ITEM,0,0,},
	    {"Copy                /C",	COPY_ITEM,0,0,},
	    {"Paste               /V",	PASTE_ITEM,0,0,},
	    {"Clear               /K",	CLEAR_ITEM,0,0,},
	    {"Select All          /A",	SELECTALL_ITEM,0,0,},	    	    
	    {"----------------------", 	9,0,0,},
	    {"Options...            ",	OPTIONS_ITEM,0,0,},
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Search[]={
	    {"Search",  					SEARCH_MENU,uWindowStandardMenu,0,},
	    {"Find           ...  /F",  FIND_ITEM,0,0,},
	    {"Change         ...  /G", 	CHANGE_ITEM,0,0,},
	    {"Go to Line     ...  /J", 	GOTOLINE_ITEM,0,0,},
	    
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Operations[]={
	    {"Operations",  					OPERATION_MENU,uWindowStandardMenu,0,},
	    {"Evaluate                /E",  EVALUATE_ITEM,0,0,},
	    {"Set Frame...                  ", 	SET_FRAME_ITEM,0,0,},
	    {"Auto...                       ", 	AUTO_ITEM,0,0,},
	    {"Set Output Folder...          ", 	SET_OUTPUT_ITEM,0,0,},
	    {"Export                        ", 	EXPORT_ITEM,0,0,},
	    {"Run as Batch                  ", 	RUN_BATCH_ITEM,0,0,},
	    
	    
	    {NULL,0,0,0,},
	};
	
	static struct uMenuItems Functions[]={
	    {"Functions",  	FUNCTION_MENU,uWindowStandardMenu,0,},
	    {"f1",  		1,0,0,},
	    {NULL,0,0,0,},
	};
	             
	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Search},
	    {Operations},
	    {Functions},
	    {NULL}
	};
	
	uRect Rect={40,40,500,300};
	struct FileInfo2 *Files;
	
	IconPtr myIcon;
	char buff[256];
	CEditPtr t;
 	static long nTek;
 	uPort Port;
	int ret;

	ret = 1;
	Files=NULL;
	uGetPort(&Port);

	myIcon=uCreateIcon(ExpressionType,&Rect);

	if(myIcon == NULL){
		WarningBatch("ExpressionType uCreateIcon Failed\n");
    	return myIcon;
	}

	myIcon->pClose=doClose;
	myIcon->pUpdateWrite=UpdateWrite;
	myIcon->pMenu=doMenus;
	myIcon->pNullEvent=doNULL;
	myIcon->pKeyBoard=CEditpKeyBoard;
	myIcon->pInContent=CEditpInContent;
	myIcon->pMoveControls=SetWindow;
	
	myIcon->pPageUp=cdoPageUp;
	myIcon->pPageDown=cdoPageDown;
	myIcon->pLineUp=cgoUp;
	myIcon->pLineDown=cgoDown;
	myIcon->pThumb=cdothumb;
	myIcon->pCheckMenu=CEditCheckMenu;
	myIcon->pMessage=EditGetMessage;
	myIcon->pMouseMoved=nMouseMoved;
	

	uSetCursor(myIcon,uWATCH_CURSOR);

	sprintf(buff,"ExpressionWindow %03ld%c",nTek++,0);

	if(uSetWindow(myIcon,	       
	              uWindowSetWindowName,buff,
				  uWindowSetMenuList,menuList,
	              uWindowSetDone,uWindowSetDone)
	)goto ErrorOut;

	t=(CEditPtr)cMalloc(sizeof(struct CEdit),8311);
	if(!t){
	    WarningBatch("CEditNew Out of Memory\n");
	    goto ErrorOut;
	}
	zerol((char *)t,sizeof(struct CEdit));

	myIcon->DWindow=(struct DumWindow*)t;

	myIcon->eraseRectSize=120;	

	myIcon->framesPerSecond=30;
	t->milliSecondsPerFrame=(long int)(1000./myIcon->framesPerSecond);

	t->CurrentFrame=0;

	t->TabSize=4;

	t->FrameIncrement=1;	
	t->lRowInsert=0;
	t->lColumnInsert=0;
	t->fRowHigh=0;
	t->fColumnHigh=0;
	t->lRowHigh=0;
	t->lColumnHigh=0;
	t->CursorFlag=0;
	t->CursorRow=0;
	t->CursorColumn=0;
	t->CursorTime=TickCount();
	t->xPageSize=0;
	t->yPageSize=0;
	t->LineTop=0;
	t->LineLeft=0;
	t->DeleteBp=NULL;
	{
	    uFontInfo Info;
	    int yl;
	    
		uSetTextFace(myIcon,0);
		
		uSetTextFont(myIcon,DefaultFont);
		
		uSetFontSize(myIcon,DefaultFontSize);
		
		uGetFontInfo(myIcon,&Info);
		
		t->LineHeight = Info.LineHeight;
	    t->Descent	  = Info.Descent;
	    t->CWidth 	  = Info.CWidth;
	    
	    yl=(Rect.ysize-ScrollOffSet)/t->LineHeight;
		t->yPageSize=yl*t->LineHeight;

	}
	if(files){
		
	    CVideodoOpen(myIcon,files);
		Files=initFILES(files);
	    if(!Files){
	        WarningBatch("NewCEdit out of Memory\n");
	        goto ErrorOut;
	    }
		Files->fileClass=TEXT_MEMORY;
		Files->pdefault=FilesOpenEdit;
		myIcon->Files=Files;

		SetWindowToFileName(myIcon,myIcon->Files);
	    
	}else{
	    t->e=EditNewFile(NULL);
	    if(!t->e)goto ErrorOut;
	}

	t->Express=CExpressStart(CExpressInteractive);
	
	if(!t->Express)goto ErrorOut;

	SetWindowPath(myIconIn,myIcon);
	
    if(!IsShift()){
    	doUniversalNewEdit(myIcon,universal_GetDefaults,NULL);
    	FlushCurrentPort();
    }
	
	
	SetWindow(myIcon);
	
	CEditCheckMenu(myIcon,TRUE);
	
	if(Files){
	    setInfo(Files->directory,&myIcon->infoLoad);
	    setInfo(Files->directory,&myIcon->infoSave);
	}
	
	ret = 0;
ErrorOut:
	if(ret){
	    doClose(myIcon);
	    myIcon=NULL;
	}else{	
		InvalRectMyWindow(myIcon);	
		if(myIcon)myIcon->isOpen=TRUE;
	}
	uSetCursor(myIcon,uARROW_CURSOR);
	uSetPort(&Port);
	return myIcon;
}
static int nMouseMoved(IconPtr myIcon,uPoint pt)
{
	CEditPtr t;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	
	if(uPtInRect(pt,&myIcon->uviewRect)) {		
	    uSetCursor(myIcon,uBEAM_CURSOR);
	    myIcon->inThisWindow=TRUE;
	}else{
	    uSetCursor(myIcon,uARROW_CURSOR);
	    myIcon->inThisWindow=FALSE;
	}		
	return 0;	
}
static int EditGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct Message1000 *m;
	struct linedata *li;
	char windowName[256];
	uPort save;
	CEditPtr w;
	long n;
	int ret;

	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	m=(struct Message1000 *)MessageData;

	ret=1;

	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	switch(MessageType){
	case MessageType_LIMITS:
	    if(!m)goto ErrorOut;
	    break;
	case MessageType_SET_FRAME_NUMBER:
	    w->CurrentFrame=m->CurrentFrame;
	    break;
	case MessageType_GET_FRAME_COUNT:
	    if(w->FrameCount > 0){
	        m->FrameCount=w->FrameCount;
	    }else{
	        m->FrameCount=w->FrameLast+1;
	    }
	    goto OK;
	case MessageType_GET_LINE_DATA:
	    li=MessageData;
	    ret = EditdoLineGetData(myIcon,li);
	    goto ErrorOut;
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
	    if(myIcon->Files){
			GetWindowName(myIcon,windowName,0);
		    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
		    WarningBatch(WarningBuff);
		    printFileInformation(myIcon->Files);
	    }
		goto OK;
	}
	InvalRectMyWindow(myIcon);
	UpdateWrite(myIcon);
OK:
	ret=0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
static int EditdoLineGetData(IconPtr myIcon,struct linedata *li)
{
	struct stack R,*R1,*R2;
	struct stackHolder *s;
	struct stack *name,*name2;
	long CurrentFrame;
	long stackCount;
	long n,k,ns;
	CEditPtr w;
	CExpressPtr e;
	double *xD,*yD;
	int ret;
	
	if(!li)return 1;
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w || !w->e || (w->e->LineCount <= 0))return 1;
	e=w->Express;
	if(!e)return 1;
	
	
	xD=NULL;
	yD=NULL;
	R1=NULL;
	R2=NULL;
		
	e->KeepStack=TRUE;
	
	CurrentFrame=w->CurrentFrame;
	w->CurrentFrame=li->CurrentFrame;
	
	ret=EditEvaluate(myIcon);
		
	
	if(ret)goto ErrorOut;
		
	s = &e->outData;
	if(!s)goto ErrorOut;
	
	stackCount=s->stackCount;

	w->FrameCount=CExpressGetFrameCount(e);
	
	for(n=0;n<stackCount;++n){
		
		if(CExpressPopFrom(e,s,T_STACK_R1,0))continue;
		R = e->R1;
		
		if(strcmp(li->pioName,R.pioName))continue;
		
		mstrncpy(e->ch,R.name,64);
		if(CExpressPush(e,T_STACK_NAME))continue;
		mstrncpy(e->ch,R.pioName,64);
		if(CExpressPush(e,T_STACK_NAME))continue;

		if(CExpressPop(e,T_STACK_R1,1))continue;
		if(CExpressPop(e,T_STACK_R2,1))continue;
		R1 = &e->R1;
		R2 = &e->R2;
		
		if(!R1->data || !R2->data)continue;

	    name=CExpressGetNameStack(e,R1->name);
		if(!name)continue;
	    name2=CExpressGetNameStack(e,R2->name);
		if(!name2)continue;
		if(name->PointLineType == LINEDATA_POINT_DATA){
			 ;		
		}else if(name->PointLineType == LINEDATA_LINE_DATA){
			 ;		
		}else if(name->PointLineType == LINEDATA_BHANGMETER_DATA){
			 ;		
		}else{
		    continue;
		}
		
		ns=R1->xsize;
		
		if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),8873))){
		    WarningBatch("EditdoLineGetData Out of List Memory\n");
		    goto ErrorOut;
		}
		if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),8883))){
		    WarningBatch("EditdoLineGetData Out of List Memory\n");
		    goto ErrorOut;
		}
		
		if(name2->type == T_STACK_VARIABLE){	
		    if(R1->xsize != R2->xsize){
		    	sprintf(WarningBuff,"EditdoLineGetData Size Stack Error %ld %ld\n",R1->xsize,R2->xsize);
		    	WarningBatch(WarningBuff);
		    	goto ErrorOut;
		    }
	   		for(k=0;k<R2->xsize;++k){
	   		    yD[k]=R1->data[k];   		    
	   		    xD[k]=R2->data[k];
	   		}   		
   		}else{
   		    if(R1->auxData){
	   			if(R1->auxLength != R1->xsize || !R1->auxData){
			    	sprintf(WarningBuff,"EditdoLineGetData Size Error %ld %ld\n",R1->xsize,R1->auxLength);
			    	WarningBatch(WarningBuff);
			    	goto ErrorOut;
	   			}
		   		for(k=0;k<R1->xsize;++k){
		   		    yD[k]=R1->data[k];   		    
		   		    xD[k]=R1->auxData[k];
		   		}   	
	   		}else{
		    	WarningBatch("EditdoLineGetData Data Pointer Null \n");
		    	goto ErrorOut;
	   		}
   		
   		}
   		
   		
   		
   		
		li->dCount=ns;
   		li->xData=xD;
		li->yData=yD;
		li->integral=name->integral;
		
		xD=NULL;
		yD=NULL;
		
		cFree((char *)R1->data);
		R1->data=NULL;
		cFree((char *)R2->data);
		R2->data=NULL;
		
		break;
			
	}

	
	ret=0;
	
ErrorOut:
	
	e->KeepStack=FALSE;
	CExpressFreeStacks(e);
	
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	
	w->CurrentFrame=CurrentFrame;
	
	return ret;
	
}
static int  EditdoExport(IconPtr myIcon)
{
	struct stackHolder *s;
	struct linedata *li;
	struct linedata *ldata;
	struct stack *name;
	long n,nsave;
	CEditPtr w;
	CExpressPtr e;
	int ret;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w || !w->e || (w->e->LineCount <= 0))return 1;
	e=w->Express;
	if(!e)return 1;
	
	SetPortMyWindow(myIcon);
	
	ldata=NULL;
		
	e->KeepStack=TRUE;
	
	e->lineplotgather=FALSE;
	
	ret=EditEvaluate(myIcon);
		
	if(ret)goto ErrorOut;
		
	s = &e->outData;
	if(!s)goto ErrorOut;
	
	if(s->stackCount <= 0)goto OKOut;
	
	nsave=s->stackCount;
	
	ldata=(struct linedata *)cMalloc(nsave*sizeof(struct linedata),99007);
	if(!ldata)goto ErrorOut;
	zerol((char *)ldata,nsave*sizeof(struct linedata));
	
	w->FrameCount=CExpressGetFrameCount(e);

	for(n=0;n<nsave;++n){
		struct stack R,*R1,*R2;
		
		li= &ldata[n];
		
		li->type= -1;
		
		if(CExpressPopFrom(e,s,T_STACK_R1,0))continue;
		R = e->R1;
		
		mstrncpy(e->ch,R.name,64);
		if(CExpressPush(e,T_STACK_NAME))continue;
		mstrncpy(e->ch,R.pioName,64);
		if(CExpressPush(e,T_STACK_NAME))continue;

		if(CExpressPop(e,T_STACK_R1,1))continue;
		if(CExpressPop(e,T_STACK_R2,1))continue;
		R1 = &e->R1;
		if(R1->data)cFree((char *)R1->data);
		R1->data=NULL;
		
		R2 = &e->R2;
		if(R2->data)cFree((char *)R2->data);
		R2->data=NULL;

	    name=CExpressGetNameStack(e,R1->name);
		if(!name)continue;
		if(name->PointLineType == LINEDATA_POINT_DATA){
			li->type=LINEDATA_POINT_DATA;		
		}else if(name->PointLineType == LINEDATA_LINE_DATA){
			li->type=LINEDATA_LINE_DATA;		
		}else if(name->PointLineType == LINEDATA_BHANGMETER_DATA){
			li->type=LINEDATA_BHANGMETER_DATA;		
		}else{
		    continue;
		}
		
		li->x1=name->x1;
		li->y1=name->y1;
		li->z1=name->z1;
		li->x2=name->x2;
		li->y2=name->y2;
		li->z2=name->z2;
		li->integral=name->integral;
		li->integrate=name->integrate;
	    li->FirstFrame=0;
	    li->FirstTime=0;
		if(w->FrameCount > 0){
		    li->LastFrame=w->FrameCount-1;
		    li->LastTime=(double)(w->FrameCount-1);
		    li->FrameCount=w->FrameCount;
		}else{
		    li->LastFrame=1;
		    li->LastTime=1;
		    li->FrameCount=w->FrameLast+1;
		}
			    
		GetWindowName(myIcon,li->sourceName,0);
		
		li->Plot=NULL;
		li->CurrentFrame=w->CurrentFrame;
		li->replaceData = -1;
		
		li->pioIndex=0;
		mstrncpy(li->pioName,R1->name,64);
		
			
	}

	e->KeepStack=FALSE;
	CExpressFreeStacks(w->Express);
	
	if(e->lineplotgather){
		CDrawNewLine(ldata,(int)nsave);
	}else{	
	
		for(n=0;n<nsave;++n){
			
			li= &ldata[n];
			
			if(li->type == -1)continue;
			
			CDrawNewLine(li,1);
		}
	
	}


OKOut:
	
	ret=0;
	
ErrorOut:
	
	e->KeepStack=FALSE;
	CExpressFreeStacks(w->Express);
	
	if(ldata)cFree((char *)ldata);
		
	return ret;
	
}
int CEditCheckMenu(IconPtr myIcon,int flag)
{
	menuPtr Function;
	long Length;
	char *bp;
	CEditPtr w;
	long n;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	
	if(flag){	/*    Become active	*/				
        if(w->e && (w->e->LineCount > 0)){
		    uEnableMenuItem(myIcon,FILE_MENU,SAVE_ITEM);
	        uEnableMenuItem(myIcon,EDIT_MENU,SELECTALL_ITEM);
        }else{
		    uDisableMenuItem(myIcon,FILE_MENU,SAVE_ITEM);
	        uDisableMenuItem(myIcon,EDIT_MENU,SELECTALL_ITEM);
        }
        bp=getStringFromScrap();
        if(bp){
            Length=(long)strlen(bp);
            cFree(bp);
            if(Length < 0){
                uDisableMenuItem(myIcon,EDIT_MENU,PASTE_ITEM);
            }else{
                uEnableMenuItem(myIcon,EDIT_MENU,PASTE_ITEM);
            }
        }	 
		if(w->clipboard)cFree((char *)w->clipboard);
		w->clipboard=CVideogetSelection(myIcon,&n);
        if(w->clipboard){
	        uEnableMenuItem(myIcon,EDIT_MENU,CUT_ITEM);
	        uEnableMenuItem(myIcon,EDIT_MENU,COPY_ITEM);
	        uEnableMenuItem(myIcon,EDIT_MENU,CLEAR_ITEM);
        }else{
            uDisableMenuItem(myIcon,EDIT_MENU,CUT_ITEM);
            uDisableMenuItem(myIcon,EDIT_MENU,COPY_ITEM);
            uDisableMenuItem(myIcon,EDIT_MENU,CLEAR_ITEM);
        }   
        Function=NULL;
	    if(!uSetWindow(myIcon,uWindowGetMenu,FUNCTION_MENU,&Function,
	    	                         uWindowSetDone,uWindowSetDone)){
			CExpressMenu(Function);
		}
		doWindowMenu();
	}
	return 0;
}
static int doNULL(IconPtr myIcon)
{

	CVideodoInsertDraw(myIcon);
	return 0;
}
static void CVideodoInsertDraw(IconPtr myIcon)
{
	char buff[256];
	short int x,y;
	uRect Box;
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	if(!myIcon->inThisWindow)return;
	
	if((t->CursorRow < t->LineTop) 		|| 
	   (t->CursorRow > (t->LineTop + t->yPageSize))  ||
	   (t->CursorColumn < t->LineLeft) 		|| 
	   (t->CursorColumn > (t->LineLeft + t->xPageSize)))return;
	if(TickCount() > (t->CursorTime+50)){
	    if(t->CursorFlag){
	        Box.y=(int)(t->dataRect.y+(t->CursorRow-t->LineTop)*t->LineHeight);
	        Box.ysize=t->LineHeight+1;
	        --Box.y;
	        Box.x=(int)(t->dataRect.x+(t->CursorColumn-t->LineLeft)*t->CWidth);
	        Box.xsize=3;
	        --Box.x;
	        uEraseRect(&Box,myIcon);
	    }else{
	        x=(short)(t->dataRect.x+(t->CursorColumn-t->LineLeft)*t->CWidth);
	        y=(short)(t->dataRect.y+(t->CursorRow-t->LineTop)*t->LineHeight);
	        if(t->fRowHigh != t->lRowHigh || t->fColumnHigh != t->lColumnHigh)return;
	        uMoveTo(x,y);
	        uLineTo(x,y+t->LineHeight-1,myIcon);
	    }
	    t->CursorFlag = !t->CursorFlag;
	    t->CursorTime=TickCount();
		sprintf(buff,"  %ld %ld%c",(long)t->CursorColumn+1,t->lColumnHigh-t->fColumnHigh,0);
		nWriteBox(myIcon,buff);
	}
}
static int showCursor(IconPtr myIcon)
{
	CEditPtr t;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	
	t->CursorTime=TickCount()-50;
	
	return 0;
}

static int CEditpKeyBoard(IconPtr myIcon,int c)
{
	CEditPtr t;
	char text[1024];
	long rFirst,cFirst;
	uPort save;
	uRect r;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	if(c <= 0)return 0;

	uGetPort(&save);
	SetPortMyWindow(myIcon);
	uGetPortBounds(myIcon,&r);
	uClipRect(&myIcon->uviewRect,myIcon);
	
	if(c == leftArrow){
	    rFirst=t->CursorRow;
	    cFirst=CVideocColumn(myIcon,t->CursorColumn,rFirst)-1;
	    t->CursorColumn=CVideocToColumn(myIcon,(short)(cFirst),rFirst);	
	    if(t->CursorColumn < 0)t->CursorColumn = 0;
	    CVideoCursorInView(myIcon);
	    CVideoSetInsert(myIcon,t->CursorRow,t->CursorColumn);
	    CVideoDrawRow(myIcon,t->CursorRow);
	}else if(c == rightArrow){
	    rFirst=t->CursorRow;
	    cFirst=CVideocColumn(myIcon,t->CursorColumn,rFirst)+1;
	    t->CursorColumn=CVideocToColumn(myIcon,(short)(cFirst),rFirst);	
	    if(t->CursorColumn >= 256)t->CursorColumn = 255;
	    CVideoCursorInView(myIcon);
	    CVideoSetInsert(myIcon,t->CursorRow,t->CursorColumn);
	    CVideoDrawRow(myIcon,t->CursorRow);
	}else if(c == upArrow){
	    rFirst=t->CursorRow;
	    cFirst=CVideocColumn(myIcon,t->CursorColumn,rFirst);
	    if(--t->CursorRow < 0)t->CursorRow = 0;
	    t->CursorColumn=CVideocToColumn(myIcon,(short)(cFirst),t->CursorRow);	
	    if(t->CursorColumn < 0)t->CursorColumn = 0;
	    if(t->CursorColumn >= 256)t->CursorColumn = 255;
	    CVideoCursorInView(myIcon);
	    CVideoSetInsert(myIcon,t->CursorRow,t->CursorColumn);
	    CVideoDrawRow(myIcon,t->CursorRow);
	    if(rFirst != t->CursorRow)CVideoDrawRow(myIcon,rFirst);
	}else if(c == downArrow){
	    rFirst=t->CursorRow;
	    cFirst=CVideocColumn(myIcon,t->CursorColumn,rFirst);
	    ++t->CursorRow;
	    if(t->e && (t->CursorRow >= t->e->LineCount))t->CursorRow = t->e->LineCount-1;
	    t->CursorColumn=CVideocToColumn(myIcon,(short)(cFirst),t->CursorRow);	
	    if(t->CursorColumn < 0)t->CursorColumn = 0;
	    if(t->CursorColumn >= 256)t->CursorColumn = 255;
	    CVideoCursorInView(myIcon);
	    CVideoSetInsert(myIcon,t->CursorRow,t->CursorColumn);
	    CVideoDrawRow(myIcon,t->CursorRow);
	    if(rFirst != t->CursorRow)CVideoDrawRow(myIcon,rFirst);
	} else if(c <= 127  ||
	          c == '\t' ||
	          c == '\n' ||
	          c == '\r' ||
	          c == 8    ||
	          c == 127   
	          ){
	    long saveRow;
		int atEnd;

		atEnd = FALSE;
		
	    text[0]=c;

	    t->CursorFlag = TRUE;
	    t->CursorTime = 0;
	    CVideodoInsertDraw(myIcon);
	    saveRow=t->CursorRow;
		if(c == 8 || c == 127){
			if((t->fRowHigh != t->lRowHigh) ||
                   (t->fColumnHigh != t->lColumnHigh)){
				CVideodoCut(myIcon);
				return 0;
            }
	       CVideoDeleteAtCursor(myIcon);
	       if(saveRow != t->CursorRow)CVideoCursorInView(myIcon);
	    }else{
			if((t->fRowHigh != t->lRowHigh) ||
                   (t->fColumnHigh != t->lColumnHigh)){
				CVideodoCut(myIcon);
			}
			if(c == '\r'){
				char *Data;
				int nc,nn;
				int n;
				
			    Data=EditListLine(t->e,saveRow);
			    if(Data){
			        nn=1;
			    	nc=(int)strlen(Data);
			    	for(n=0;n<nc;++n){
			    	    if(Data[n] == ' ' || Data[n] == '\t'){
			    	        text[nn++]=Data[n];
			    	    }else{
			    	        break;
			    	    }
			    	}
					rFirst=t->lRowInsert;
					cFirst=CVideocColumn(myIcon,t->lColumnInsert,rFirst);
					
					if(cFirst == nc){   /* CR at end of line */
					   atEnd = TRUE;
					}
					
				    text[nn]=0;
			    }else{
				    text[1]=0;
			    }			
			}else{
				text[1]=0;
			}
	        CVideoInsertAtCursorLong(myIcon,text);
	        if(atEnd){
	            long rsave,csave;
	            rsave=t->CursorRow;
	            csave=t->CursorColumn;
				text[0]='\r';
				text[1]=0;
	        	CVideoInsertAtCursorLong(myIcon,text);
	            t->CursorRow=rsave;
	            t->CursorColumn=csave;
	        }
	    }
	    CVideoSetInsert(myIcon,t->CursorRow,t->CursorColumn);
	    if(saveRow != t->CursorRow){
	        EditDrawIt(myIcon);
	        CVideoCursorInView(myIcon);
	    }else{
	        CVideoDrawRow(myIcon,t->CursorRow);
	    }
	}
	showCursor(myIcon);
	uClipRect(&r,myIcon);
	uSetPort(&save);
	return 0;
}
int CVideocToColumn(IconPtr myIcon,short x,long row)
{
	int col;
	int k,n;
	char *Line;
	CEditPtr t;

	if(!myIcon)return 0;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 0;
	
	Line=EditListLine(t->e,row);
	if(!Line)return 0;

	n=(int)strlen(Line);
	if(n <= 0)return 0;
	if(x < n)n=x;
	col=0;
	for(k=0;k<n;++k){
	    if(Line[k] == '\t'){
	        ++col;
	        while((col % t->TabSize) != 0){
	            ++col;
	        }
	    }else{
	        ++col;
	    }
	}
	return col;
}
void CVideodoCut(IconPtr myIcon)
{
 	CVideodoCopy(myIcon);CVideodoClear(myIcon);
}

static void CVideodoSelectAll(IconPtr myIcon)
{
	CEditPtr t;
	char *Line;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t || !t->e)return;
	
	if(t->e->LineCount <= 0)return;
	
	t->fColumnHigh = 0;
	t->fRowHigh = 0;
	
	Line=EditListLine(t->e,t->e->LineCount-1);
	if(Line){
		t->lColumnHigh = strlen(Line);
	}else{
		t->lColumnHigh = 0;
	}
	
	t->lRowHigh = t->e->LineCount-1;
	
	EditDrawIt(myIcon);
}
static void CVideodoClear(IconPtr myIcon)
{
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	
	if(t->DeleteBp)cFree((char *)t->DeleteBp);
	if((t->DeleteBp=CVideogetSelection(myIcon,&t->DeleteN)) == NULL)return;
	CVideokillSelection(myIcon);
	EditDrawIt(myIcon);
}
static int CVideodoCopy(IconPtr myIcon)
{
	CEditPtr t;
	long n;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	
	if(t->clipboard)cFree((char *)t->clipboard);
	t->clipboard=CVideogetSelection(myIcon,&n);
	
	if(t->clipboard){
	    ZeroScrap();
	    putStringToScrap(t->clipboard);
	}
	return 0;
}
static int CVideodoPaste(IconPtr myIcon)
{
	CEditPtr t;
	char *bp;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	
	bp=getStringFromScrap();
		
    if(t->DeleteBp)cFree((char *)t->DeleteBp);
    t->DeleteBp= CVideogetSelection(myIcon,&t->DeleteN);
    CVideokillSelection(myIcon);
    CVideoInsertAtCursorLong(myIcon,bp);
    
    cFree((char *)bp);
		
	EditDrawIt(myIcon);
	return 0;
}
static void CVideoDeleteAtCursor(IconPtr myIcon)
{
	CEditPtr t;
	long ncol,cFirst,rFirst;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	if(t->CursorRow < 0){
	    t->CursorRow=0;
	    return;
	}

	cFirst=CVideocColumn(myIcon,t->CursorColumn,t->CursorRow);
	if(t->CursorColumn <= 0){
	    if(t->CursorRow <= 0)return;
	    ncol=CVideocColumn(myIcon,2560,t->CursorRow-1);
	    EditListDeleteOne(t->e,t->CursorRow,-1);
        cFirst=ncol+1;
	    --t->CursorRow;
	}else{
	    EditListDeleteOne(t->e,t->CursorRow,cFirst-1);
	}

	rFirst=t->CursorRow;

	EditGetLast(t->e,&rFirst,&cFirst);

	t->CursorRow=rFirst;

	t->CursorColumn=CVideocToColumn(myIcon,(short)(cFirst),t->CursorRow);	

}

void CVideoCursorAtEnd(IconPtr myIcon)
{
	long rNew,cNew;
	CEditPtr t;
	uPort save;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	uGetPort(&save);
	
	SetPortMyWindow(myIcon);
	EditGetLast(t->e,&rNew,&cNew);
	t->CursorRow=rNew;
	t->CursorColumn=0;
	SetWindow(myIcon);
	CVideoCursorInView(myIcon);
	uSetPort(&save);
	return;
}
void CVideoInsertAtCursorLong(IconPtr myIcon,char *data)
{
	long rFirst,cFirst,rNew,cNew;
	uPort save;
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	uGetPort(&save);	
	SetPortMyWindow(myIcon);
	rFirst=t->lRowInsert;
	cFirst=CVideocColumn(myIcon,t->lColumnInsert,rFirst);

	EditListInsert(t->e,data,rFirst,cFirst);

	EditGetLast(t->e,&rNew,&cNew);

	t->lRowInsert=rNew;

	t->lColumnInsert=CVideocToColumn(myIcon,(short)cNew,rNew);

	t->CursorRow=t->lRowInsert;

	t->CursorColumn=t->lColumnInsert;

	if(rNew != rFirst){
	    SetWindow(myIcon);
	    CVideoCursorInView(myIcon);
	}
	uSetPort(&save);
}
void CVideoCursorInView(IconPtr myIcon)
{
	CEditPtr t;
	long oldv;
	int imove;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	imove=FALSE;
	if(t->CursorRow < t->LineTop){
	    oldv=uGetControlValue(myIcon->VControl);
	    uSetControlValue(myIcon->VControl,oldv-(t->LineTop-t->CursorRow+t->yPageSize/2));
	    CVideoMoveDy(myIcon);
	    imove=TRUE;
	}else if(t->CursorRow >= (t->LineTop + t->yPageSize)){
	    oldv=uGetControlValue(myIcon->VControl);
	    uSetControlValue(myIcon->VControl,oldv+(t->CursorRow-t->LineTop-t->yPageSize/2));
	    CVideoMoveDy(myIcon);
	    imove=TRUE;
	}
	if(t->CursorColumn < t->LineLeft){
	    oldv=uGetControlValue(myIcon->HControl);
	    uSetControlValue(myIcon->HControl,oldv-(t->LineLeft-t->CursorColumn+1));
	    CVideoMoveDy(myIcon);
	    imove=TRUE;
	}else if(t->CursorColumn > (t->LineLeft + t->xPageSize)){
	    oldv=uGetControlValue(myIcon->HControl);
	    uSetControlValue(myIcon->HControl,oldv+(t->CursorColumn-(t->LineLeft+t->xPageSize)+1));
	    CVideoMoveDy(myIcon);
	    imove=TRUE;
	}
	t->CursorFlag = TRUE;
	t->CursorTime = 0;
	if(imove)InvalRectMyWindow(myIcon);
	CVideodoInsertDraw(myIcon);

}
static void CVideokillSelection(IconPtr myIcon)
{
 	long rFirst,rLast,cFirst,cLast;
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	
	
	if((t->fRowHigh == t->lRowHigh) &&
           (t->fColumnHigh == t->lColumnHigh))return;
           
	t->CursorRow=rFirst=t->fRowHigh;
	t->CursorColumn=cFirst=t->fColumnHigh;
	cFirst=CVideocColumn(myIcon,t->fColumnHigh,t->fRowHigh);
	rLast=t->lRowHigh;
	cLast=CVideocColumn(myIcon,t->lColumnHigh,t->lRowHigh);
	t->lRowInsert=t->lRowHigh=t->fRowHigh;
	t->lColumnInsert=t->lColumnHigh=t->fColumnHigh;

	EditkillSelection(t->e,rFirst,cFirst,rLast,cLast);

	SetWindow(myIcon);
	return;
}
static char *CVideogetSelection(IconPtr myIcon,long *nn)
{
	long rFirst,rLast,cFirst,cLast;
	char *bp;
	CEditPtr t;
	
	if(!myIcon)return NULL;
	t=(CEditPtr)myIcon->DWindow;
	if(!t || !nn)return NULL;
	
	*nn = 0;

	if(t->lRowHigh < 0)t->lRowHigh = 0;

	if(t->fRowHigh < 0)t->fRowHigh = 0;
	if(t->fRowHigh > t->lRowHigh)t->fRowHigh = t->lRowHigh;
	
	if((t->fRowHigh == t->lRowHigh) &&
           (t->fColumnHigh == t->lColumnHigh))return NULL;
	
	t->DeleteRow=rFirst=t->fRowHigh;
	t->DeleteColumn=cFirst=CVideocColumn(myIcon,t->fColumnHigh,t->fRowHigh);
	rLast=t->lRowHigh;
	cLast=CVideocColumn(myIcon,t->lColumnHigh,t->lRowHigh);
	bp=EditgetSelection(t->e,rFirst,cFirst,rLast,cLast);
	if(bp){
	    *nn = (long)strlen(bp);
	}
	return bp;
}
void CVideoDrawRow(IconPtr myIcon,long CursorRow)
{
	long y,top,bottom,left;
	char buff[256];
	char *Data;
	uRect Box;
	long n,m;
	int nc;
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	if(!t->e)return;
	if(!t->e->LineCount)return;

	Box=myIcon->uviewRect;
	m=0;
	top=t->dataRect.y;
	left=t->dataRect.x;
	bottom=top+t->dataRect.ysize;
	for(n=t->LineTop;n<t->e->LineCount;++n,++m){
	    if((y=m*t->LineHeight+top) > bottom)break;
	    if(n != CursorRow)continue;
	    Box.y=(int)y;
	    y += t->LineHeight;
	    Box.ysize=t->LineHeight;
	    uEraseRect(&Box,myIcon);
	    if(t->ShowLineNumbers){
	    	sprintf(buff,"%ld%c",n+1,0);
	    	uMoveTo((int)(t->CWidth+t->CWidth*(6-strlen(buff))),(int)(y-t->Descent));
			uDrawString(buff,myIcon); 
	    }	    	    
	    Data=EditListLine(t->e,n);
	    if(!Data)continue;
	    nc=(int)strlen(Data);
	    CVideoDrawLine(myIcon,(int)left,(int)(y-t->Descent),Data,nc);
	}
}
int CVideocColumn(IconPtr myIcon,long x,long row)
{
	int col,cNoTab;
	int k,n;
	char *Line;
	CEditPtr t;

	if(!myIcon)return 0;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 0;
	
	Line=EditListLine(t->e,row);
	if(!Line)return 0;

	cNoTab=(int)x;
	n=(int)strlen(Line);
	if(n <= 0)return 0;
	k=0;
	col=0;
	while((col++ < cNoTab) && (k < n)){
	    if(Line[k++] == '\t'){
	        while((col % t->TabSize) != 0){
	            col++;
	        }
	    }
	}
	return k;
}
static int CVideoCharacter(IconPtr myIcon,long Row,long Column)
{
    /* return character to right of Column */
	CEditPtr t;
	char *Line;
	int n,k,col;
	
	if(!myIcon)return -1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return -1;
	
	Line=EditListLine(t->e,Row);
	if(!Line)return -1;
	n=(int)strlen(Line);
	if(n <= 0)return -1;

	k=0;
	col=0;
	while(k < n){
	    if(col > Column){
	        return -1;
	    }else if(col++ == Column){
	        return Line[k];
	    }else if(Line[k++] == '\t'){
	        while((col % t->TabSize) != 0){
	            col++;
	        }
	    }
	}
	
	return -1;
}
static int CVideoSelectWord(IconPtr myIcon,uPoint *pt)
{
	long Row,Column,cMax,right,left;
	CEditPtr t;
	long n;
	int c;
	
	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	if(!pt)return 1;
	
	CVideoRowColumn(myIcon,pt,&Row,&Column);
	c=CVideoCharacter(myIcon,Row,Column);
	if(c < 0 || !(isalpha(c) || isdigit(c) || c == '_'))return 0;
	
	cMax=CVideocToColumn(myIcon,3200,Row);
	
	right=Column;
	for(n=Column;n<cMax;++n){
	   c=CVideoCharacter(myIcon,Row,n);
	   if(c < 0 || !(isalpha(c) || isdigit(c) || c == '_'))break;
		right=n;
	}
	
	left=Column;
	
	for(n=Column;n>=0;--n){
	   c=CVideoCharacter(myIcon,Row,n);
	   if(c < 0 || !(isalpha(c) || isdigit(c) || c == '_'))break;
		left=n;
	}
	
	++right;
	
	t->fRowHigh=t->lRowHigh=t->CursorRow=Row;
	t->CursorColumn=Column;
	t->fColumnHigh=left;
	t->lColumnHigh=right;
	CVideoSetInsert(myIcon,Row,Column);
	EditDrawIt(myIcon);
	    
	return 0;
}
static int CEditpInContent(IconPtr myIcon)
{
	static uPoint LastPoint;
	uPoint pt;
	CEditPtr w;
	uRect cBox;
  	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;

	cBox=myIcon->eraseRect;
	cBox.x = cBox.x+cBox.xsize-4;
	cBox.xsize = 4;
	cBox.ysize = 4;

	uPointer(&pt,myIcon);
	
	if(uPtInRect(pt,&myIcon->uviewRect)){	

	    if((TickCount() < w->LastClick+120) &&
	        (pt.x == LastPoint.x)     && 
	        (pt.y == LastPoint.y)){
	        return CVideoSelectWord(myIcon,&pt);
	    }
	    
		LastPoint=pt;
		
		CVideodoDownInView(myIcon);
	} else if(uPtInRect(pt,&cBox)){
	    doEraseDown(myIcon);
	} else {
	    uDoControls(myIcon);
	}
	w->LastClick=TickCount();
	return 0;
}
static void CVideodoDownInView(IconPtr myIcon)
{
	long Row,Column,SelectRow,SelectColumn;
	CEditPtr t;
	uPoint pt;
	long oldv;
	
	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	Row=0;
	Column=0;
	uPointer(&pt,myIcon);
	t->CursorFlag = TRUE;
	t->CursorTime = 0;
	CVideodoInsertDraw(myIcon);
	if(!IsShift()){
	    CVideoRowColumn(myIcon,&pt,&t->CursorRow,&t->CursorColumn);
	    t->fRowHigh=t->lRowHigh=t->CursorRow;
	    t->fColumnHigh=t->lColumnHigh=t->CursorColumn;
	    CVideoSetInsert(myIcon,t->CursorRow,t->CursorColumn);
	    EditDrawIt(myIcon);
	}
	SelectRow=t->CursorRow;
	SelectColumn=t->CursorColumn;

	while(uStillDown(myIcon)){

	    uPointer(&pt,myIcon);

	    if(uPtInRect(pt,&myIcon->uviewRect)){
		    CVideoRowColumn(myIcon,&pt,&Row,&Column);

		    if((Row != SelectRow) || (Column != SelectColumn)){
		        SelectRow=Row;
		        SelectColumn=Column;
		        CVideoadjustHigh(myIcon,Row,Column);
				EditDrawIt(myIcon);
		    }
	    } else {
	        if(pt.x > t->dataRect.xsize){
		    	oldv=uGetControlValue(myIcon->HControl);
		    	uSetControlValue(myIcon->HControl,oldv+1);
	            CVideoMoveDy(myIcon);
	        }else if(pt.x < t->dataRect.x){
		    	oldv=uGetControlValue(myIcon->HControl);
		    	uSetControlValue(myIcon->HControl,oldv-1);
	        }
	        if(pt.y > t->dataRect.ysize){
		    	oldv=uGetControlValue(myIcon->VControl);
		    	uSetControlValue(myIcon->VControl,oldv+1);
	            CVideoMoveDy(myIcon);
	        }else if(pt.y < t->dataRect.y){
		    	oldv=uGetControlValue(myIcon->VControl);
		    	uSetControlValue(myIcon->VControl,oldv-1);
	            CVideoMoveDy(myIcon);
	        }
	        
		    CVideoRowColumn(myIcon,&pt,&Row,&Column);

		    if(Row != SelectRow){
		        SelectRow=Row;
		        Column=SelectColumn;
		        CVideoadjustHigh(myIcon,Row,Column);
				EditDrawIt(myIcon);
		    }	        	        
	    }
	}

}
static void CVideoSetInsert(IconPtr myIcon,long Row,long Column)
{
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	t->lRowInsert=Row;
	t->lColumnInsert=Column;
}
static void CVideoRowColumn(IconPtr myIcon,uPoint *pt,long *CursorRow,long *CursorColumn)
{
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	*CursorRow=t->LineTop+(pt->y-t->dataRect.y)/t->LineHeight;
	*CursorColumn=CVideoxColumn(myIcon,(short)pt->x,*CursorRow);
}
static int CVideoxColumn(IconPtr myIcon,short x,long row)
{
	long col,cNoTab;
	int CWidth2;
	long k,n;
	char *Line;
	CEditPtr t;

	if(!myIcon)return 0;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 0;

	Line=EditListLine(t->e,row);
	if(!Line)return 0;

	CWidth2=t->CWidth/2;
	cNoTab=t->LineLeft+(x+CWidth2-t->dataRect.x)/t->CWidth;
	n=(long)strlen(Line);
	if(n <= 0)return 0;
	col=0;
	for(k=0;k<n;++k){
	    if(Line[k] == '\t'){
	        if(col >= cNoTab)break;
	        col++;
	        while((col % t->TabSize) != 0){
	            col++;
	        }
	        if(col >= cNoTab)break;
	    }else{
	        if(col >= cNoTab)break;
	        ++col;
	    }
	}
	return (int)col;
}
static void CVideoadjustHigh(IconPtr myIcon,long Row,long Column)
{
	CEditPtr t;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	if(Row > t->CursorRow){
	    t->fRowHigh=t->CursorRow;
	    t->fColumnHigh=t->CursorColumn;
	    t->lRowHigh=Row;
	    t->lColumnHigh=Column;
	}else if(Row == t->CursorRow){
	    t->fRowHigh=t->lRowHigh=Row;
	    if(Column > t->CursorColumn){
	        t->fColumnHigh=t->CursorColumn;
	        t->lColumnHigh=Column;
	    }else{
	        t->fColumnHigh=Column;
	        t->lColumnHigh=t->CursorColumn;
	    }
	}else{
	    t->fRowHigh=Row;
	    t->fColumnHigh=Column;
	    t->lRowHigh=t->CursorRow;
	    t->lColumnHigh=t->CursorColumn;
	}
}
static int CVideodoOpen(IconPtr myIcon,struct FileList *files)
{
	CEditPtr t;
	char file[2048],buff[256];
	int ret;	
	static long nTek;

	if(!myIcon || !files)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	
	ret = 1;
	
	mstrncpy((char *)file,(char *)(files->directory),255);
	strncatToPath((char *)file,(files->name[0]),255);

	t->e=EditNewFile(file);

	if(!t->e)goto Out;

	sprintf(buff,"[%ld]%c",nTek++,0);

	mstrncat(file,buff,2048);

	SetWindowName(myIcon,file);


	ret=0;
Out:

	return ret;
}
static int doMenus(IconPtr myIcon,int Menu,int Item)
{
	CEditPtr t;
	char buff[256];

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;

	uSetCursor(myIcon,-uWATCH_CURSOR);
	
	if(Menu == AppleMenu){
		if(Item == 1){
		    doAbout(myIcon);
		}
	}else if(Menu == FILE_MENU){
	    switch(Item){
		case ABOUT_ITEM:
		  doAbout(myIcon);
		  break;
		case NEW_ITEM:
		  NewNewWindow(myIcon);
		  break;
		case OPEN_ITEM:
		  CEditdoOpen(myIcon,NULL);
		  break;
		case CLOSE_ITEM:
		  doClose(myIcon);
		  myIcon=NULL;
		  break;	
		case savePreferencesCommand:
	      writeUniversal(myIcon,0);
		  break;	
		case PreferencesCommand:
		  saveUniversal(myIcon);
		  break;	
		case SAVE_ITEM:
		  CEditdoSave(myIcon);
		  break;	
		case QUIT_ITEM:
		  doQuit();
		  myIcon=NULL;
		  break;
		default:
	          sprintf(buff,"Unknown FILE_MENU Item = %d",Item);
	          Warning(buff);
	          return 1;
	    }
	}else if(Menu == EDIT_MENU){
	    switch(Item){	    
		case clearLoadTekCommand:
		  EditListClose(t->e);t->e=NULL;CEditdoOpen(myIcon,NULL);
		  break;   
		case WINDOW_OPTIONS_ITEM:
		  doWindowInformation(myIcon);SetWindow(myIcon);
		  break;
		case CLEAR_ITEM:
		  CVideodoClear(myIcon);
		  break;
		case SELECTALL_ITEM:
		  CVideodoSelectAll(myIcon);
		  break;
		case CUT_ITEM:
		  CVideodoCut(myIcon);
		  break;
		case COPY_ITEM:
		  CVideodoCopy(myIcon);
		  break;
		case PASTE_ITEM:
		  CVideodoPaste(myIcon);
		  break;
		case OPTIONS_ITEM:
		  CVideodoOptions(myIcon);
		  SetWindow(myIcon);
		  break;
		  
	default:
	          sprintf(buff,"Unknown EDIT_MENU Item = %d",Item);
	          Warning(buff);
	          return 1;
	    }
	}else if(Menu == SEARCH_MENU){
	    switch(Item){
		case FIND_ITEM:
		  CVideodoFind(myIcon,0);
		  break;
		case CHANGE_ITEM:
		  CVideodoFind(myIcon,1);
		  break;
		  
		case GOTOLINE_ITEM:
		  CVideodoGoToLine(myIcon);
		  break;
		  
		default:
	          sprintf(buff,"Unknown SEARCH_MENU Item = %d",Item);
	          Warning(buff);
	          return 1;
	    }
	}else if(Menu == OPERATION_MENU){
	    switch(Item){
		case EVALUATE_ITEM:
		  EditEvaluate(myIcon);
		  break;
		case SET_FRAME_ITEM:
		  EditSetFrame(myIcon);
		  break;

		case AUTO_ITEM:
		  EditdoAuto(myIcon);
		  break;

		case SET_OUTPUT_ITEM:
			EditSetFolder(myIcon);
		    break;

		case EXPORT_ITEM:
			EditdoExport(myIcon);
		    break;
		    
		case RUN_BATCH_ITEM:
			EditRunAsBatch(myIcon);
		    break;
		    
		default:
	          sprintf(buff,"Unknown OPERATION_MENU Item = %d",Item);
	          Warning(buff);
	          return 1;
	    }
	}else if(Menu == FUNCTION_MENU){
	    CExpressMenuInsert(myIcon,Item);
	 	InvalRectMyWindow(myIcon);
	}else if(Menu == WindowMenu){
		pWindowMenu(myIcon,Item);
	}
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int  EditSetFolder(IconPtr myIcon)
{
 	char *pathname;
	CEditPtr w;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	
	
   
   	pathname=FileGetPath("Put Output Data Here",&myIcon->infoSave);
	if(!pathname)return 1;


	mstrncpy(w->path,pathname,2047);
	
	if(pathname)cFree((char *)pathname);

	return 0;
}
static int  EditSetFrame(IconPtr myIcon)
{
	CEditPtr t;
	int ret;
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {280,85,75,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel",{280,120,75,25},uDialogButton,uDialogNone},      
      /*  2 */      {"Current Frame",{10,50,110,25},uDialogString,uDialogNone},
      /*  3 */      {"Frame Count",{120,50,75,25},uDialogText,uDialogGetDouble},
      
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect  Rect1   = {20,40,380,175};
	
 	uDialogList *uList;
	uDialogReturn *uR;
	int reti;
	
	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Set Frame",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	reti=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)(t->CurrentFrame),
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
Start:	
    if(uR)uDialogFreeReturn(uR);
    uR = NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;

	
	if(uR[3].dreturn < 0){
	    Warning("Frame Number Must Be Zero Or Greater");
	    goto Start;
	}
	
	t->CurrentFrame  = (long)uR[3].dreturn;

	ret=0;
ErrorOut:
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

    return ret;
}
static int  EditdoAuto(IconPtr myIcon)
{
	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",{280,85,75,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel",{280,120,75,25},uDialogButton,uDialogNone}, 
           
      /*  2 */      {"First Frame", {10,20,110,25},uDialogString,uDialogNone},
      /*  3 */      {"Frame Count",{120,20,75,25},uDialogText,uDialogGetDouble},
      
      /*  4 */      {"Last Frame",  {10,55,110,25},uDialogString,uDialogNone},
      /*  5 */      {"Frame Count",{120,55,75,25},uDialogText,uDialogGetDouble},
      
      /*  6 */      {"Increment",   {10,90,110,25},uDialogString,uDialogNone},
      /*  7 */      {"Frame Count",{120,90,75,25},uDialogText,uDialogGetDouble},
      
      /*  8 */      {"Controller", {10,125,110,25},uDialogString,uDialogNone},
      /*  9 */       {"Password",{120,125,75,25},uDialogText,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect  Rect1   = {20,40,380,180};
	
 	uDialogList *uList;
	uDialogReturn *uR;
	IconPtr StatusWindow;
	char buff[256];
	CEditPtr t;
	long n;
	int reti;
	int ret;
	
	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Auto",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	reti=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,myIcon,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)(t->FrameFirst),
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)(t->FrameLast),
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)(t->FrameIncrement),

	              uDialogSetItem,9L,
	              uDialogSetText,t->controller,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;	
		
Start:	
    if(uR)uDialogFreeReturn(uR);
    uR = NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	
	if(uR[3].dreturn < 0){
	    Warning("Frame Number Must Be Zero Or Greater");
	    goto Start;
	}
	if(uR[5].dreturn < uR[3].dreturn){
	    Warning("Final Frame Number Must Be Greater That First Frame Number");
	    goto Start;
	}
	if(uR[7].dreturn <= 0){
	    Warning("Frame Increment Must Be Greater Than Zero");
	    goto Start;
	}
	
	t->FrameFirst  = (long)uR[3].dreturn;

	t->FrameLast  = (long)uR[5].dreturn;

	t->FrameIncrement  = (long)uR[7].dreturn;

	mstrncpy(t->controller,uR[9].sreturn,255);
	
	ret=0;
ErrorOut:
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(!ret){
		StatusWindow=uStartStatusWindow(myIcon,"Start Auto Evaluate");
		if(StatusWindow == NULL){
		    sprintf(WarningBuff,"StatusWindow NULL\n");
		    WarningBatch(WarningBuff);
		}
		if(t->controller[0]){
			struct Message1000 mo;
    		mo.FrameCount=0;
    		if(!SendMessageByName(t->controller,MessageType_GET_FRAME_COUNT,&mo)){
    		    if(mo.FrameCount > 0){
	    		    t->FrameFirst=0;
	    		    t->FrameIncrement=1;
	    		    t->FrameLast=mo.FrameCount-1;
    		    }
    		}
		}
		for(n=t->FrameFirst;n<=t->FrameLast;n+=t->FrameIncrement){
		    t->CurrentFrame=n;
		    sprintf(buff,"Frame %ld",n);
		    uWriteStatusWindow(StatusWindow,buff);
		    if(EditEvaluate(myIcon))break;
		    if(uCheckStatusWindow(StatusWindow,"Stop Auto ?"))break;
		}

		uStopStatusWindow(StatusWindow);    
    }

    return ret;
}
unsigned char *EditGetData(IconPtr myIcon)
{
	unsigned char *dat;
	long length,n;
	CEditPtr t;
	int c;
	
	if(!myIcon)return NULL;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return NULL;

	if(!t->e || t->e->LineCount <= 0){
	    WarningBatch("No Text");
	    return NULL;
	}

	t->fColumnHigh = 0;
	t->lColumnHigh = 0;
	t->fRowHigh = 0;
	t->lRowHigh = t->e->LineCount+1;

	dat=(unsigned char *)CVideogetSelection(myIcon,&length);

	for(n=0;n<length;++n){
		c=dat[n];
		if(c == '\r')c='\n';
		dat[n]=c;
	}
	
	dat[length]=0;

	
	
	return dat;
	
}

static int  EditEvaluate(IconPtr myIcon)
{
	CEditPtr t;
	unsigned char *dat;
	int ret;
	
	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;

	dat=EditGetData(myIcon);
	
	if(!dat)return 1;
	
	t->Express->CurrentFrame=t->CurrentFrame;
	t->Express->LastFrame=t->FrameLast;
	
	if(t->path[0]){
	    mstrncpy(t->Express->path,t->path,2048);
	}
	
	ret=CExpressEvaluate(t->Express,dat);
	
	if(dat)cFree((char *)dat);

	t->lRowHigh = 0;
	
	return ret;
}
static int CVideodoFind(IconPtr myIcon,int flag)
{
	CEditPtr t;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	if(!t->e || t->e->LineCount <= 0){
	    Warning("No Text To Search");
	    return 1;
	}

	if(flag == 0){
	   Find(myIcon);
	}else{
	   Change(myIcon);
	}

	return 0;
}
int CEditdoSave(IconPtr myIcon)
{
	static char fName[256]={'t','e','s','t','.','e','x','p',0};
	unsigned char *Data;
	struct FileInfo2 *Files;
	FILE *inout;
	CEditPtr w;
	int n;
	char *fp;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	if(!w->e)return 1;
		
	if(myIcon->FileSave){		
		GetWindowName(myIcon,fName,0);
	}else{
	    mstrncpy(fName,"test.exp",255);
	}
		
	if(GetTheSaveFileName(fName,"Save Expression File",256,&myIcon->infoSave))return 1;
	
	if((inout=fopen(fName,"wb")) == NULL){
	    sprintf(WarningBuff,"Cannot open file : %s to write\n",fName);
	    Warning(WarningBuff);
	    return 1;
	}
	
	for(n=0;n<w->e->LineCount;++n){
	    Data=(unsigned char *)EditListLine(w->e,n);
	    if(Data){
	        if(putString(Data,(long)strlen((char *)Data),inout))goto ErrorOut;
	    }
	    if(putENDOFLINE(inout))goto ErrorOut;
	}
	
	fp=strrchr((char *)fName,FILE_NAME_SEPERATOR);
    if(fp){
        fp += 1;
    }else{
        fp=fName;
    }

	myIcon->WindowNameSet=FALSE;
	
	SetWindowName(myIcon,fp);	
	if(SetFileType(fName,"TEXT"))goto ErrorOut;
	
	copyInfo(&myIcon->infoSave,&myIcon->infoLoad);
	
	Files=myIcon->Files;
	if(Files && (Files->FileCount > 0) && Files->FilesNames && Files->FilesNames[0]){
		mstrncpy((char *)Files->directory,(char *)(myIcon->infoSave.directory),255);
		if(Files->FilesNames[0])cFree((char *)Files->FilesNames[0]);
		Files->FilesNames[0]=strsave(fp,8726);
	}

	myIcon->WindowNameSet=TRUE;
	
ErrorOut:
	if(inout)fclose(inout);
	myIcon->FileSave=1;
	return 0;
}

int EditRunAsBatch(IconPtr myIconi)
{
	struct BatchInfo Batch;
	struct Icon myIcon;
	char line[4096];	
	double start,end;
	CEditPtr w;
	
	if(!myIconi)return 1;
	w=(CEditPtr)myIconi->DWindow;
	if(!w)return 1;
	if(!w->e)return 1;


	zerol((char *)&myIcon,sizeof(struct Icon));
	
	zerol((char *)&Batch,sizeof(struct BatchInfo));

	Batch.myIcon=&myIcon;

	Batch.myIcon2=myIconi;

	Batch.getLine=NewEditLine;
	
	start=rtime();
	
	w->LineCount=0;
		
	while(1){
	    if(BatchNextLine(&Batch,line,sizeof(line)))break;
	    if(ProcessLine(line,&Batch))break;
	}
	
	end=rtime();
	
	sprintf(WarningBuff,"Total Time in processFile %.2f Seconds\n",end-start);
	WarningBatch(WarningBuff);	

	return 0;
	
}
int NewEditLine(IconPtr myIcon,char *line,long len)
{
	CEditPtr w;
	char *cp;
	int c;
	
	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	if(!w->e)return 1;
	if(!line)return 1;
	
doWhite:
    cp=(char *)EditListLine(w->e,w->LineCount++);
    if(!cp)return 1;
    
    while((c=*cp) != 0){
    	if(c != ' ' || c != '\t'){
    	    break;
    	}
    	++cp;
    }
    
    if(!c || (c == '!'))goto doWhite;
    
        
    mstrncpy(line,cp,len);
	    
	return 0;
}
static void SetWindow(IconPtr myIcon)
{
	uRect eraseRect;
	CEditPtr t;
	long diff;
	uRect r;

	if(!myIcon)return;

	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	uGetPortBounds(myIcon,&r);
	
	t->dataRect=r;
	t->dataRect.y = MenuOffSet;
	t->dataRect.ysize -= (MenuOffSet+ScrollOffSet);
	t->dataRect.x = t->CWidth*(1+t->ShowLineNumbers*7);
	t->dataRect.xsize -= (t->CWidth*(1+t->ShowLineNumbers*7)+ScrollOffSet);
	t->yPageSize = (t->dataRect.ysize)/t->LineHeight;
	t->xPageSize = (t->dataRect.xsize)/t->CWidth;


	eraseRect.y=r.ysize-ScrollOffSet;
	eraseRect.x=0;
	eraseRect.ysize=ScrollOffSet;
	eraseRect.xsize=myIcon->eraseRectSize;	
	myIcon->eraseRect=eraseRect;

	if(myIcon->VControl){
	    MoveControl(myIcon->VControl,r.xsize-ScrollOffSet,MenuOffSet);
	    SizeControl(myIcon->VControl,ScrollOffSet,r.ysize-ScrollOffSet-MenuOffSet);
	    diff=0;
	    if(t->e && (t->e->LineCount > 0))diff=t->e->LineCount;
	    if(diff < 0)diff = 0;
	    uSetControlMaximum(myIcon->VControl,diff);
	}
	if(myIcon->HControl){
	
	    MoveControl(myIcon->HControl,myIcon->eraseRectSize,r.ysize-ScrollOffSet);
	    SizeControl(myIcon->HControl,r.xsize-ScrollOffSet-myIcon->eraseRectSize,ScrollOffSet);
	    diff=0;
	    if(t->e && (t->e->LineCount > 0))diff=256;
	    if(diff < 0)diff = 0;
	    uSetControlMaximum(myIcon->HControl,diff);
	}
	
	myIcon->uviewRect=r;
	myIcon->uviewRect.y = MenuOffSet;
	myIcon->uviewRect.x = 0;
	myIcon->uviewRect.ysize -= (MenuOffSet+ScrollOffSet);
	myIcon->uviewRect.xsize -= ScrollOffSet;
	InvalRectMyWindow(myIcon);
	return;
}
static int UpdateWrite(IconPtr myIcon)
{
	CEditPtr t;

	if(!myIcon || !myIcon->isOpen)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
		
    EditDrawIt(myIcon);

    uDrawControls(myIcon);

    uDrawGrowIcon(myIcon);

	return 0;
}
int EditDrawIt(IconPtr myIcon)
{
	char buff[256];
	CEditPtr t;
	uPort save;
	uRect r;

	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
		
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	uClipRect(&myIcon->uviewRect,myIcon);
	uEraseRect(&myIcon->uviewRect,myIcon);

	CVideoDumpIt(myIcon);
	uGetPortBounds(myIcon,&r);
	uClipRect(&r,myIcon);
	
	sprintf(buff,"  %ld %ld%c",(long)t->CursorColumn+1,t->lColumnHigh-t->fColumnHigh,0);
	
	nWriteBox(myIcon,buff);

	uSetPort(&save);

	return 0;
}
static void CVideoDumpIt(IconPtr myIcon)
{
	CEditPtr t;
	char buff[256];
	char *Data;
	long y,bottom,left;
	long n,m;
	int nc;
	
	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	if(!t->e)return;
	if(t->e->LineCount <= 0)return;

	m=0;
	left=t->dataRect.x;
	bottom=t->dataRect.ysize;
	for(n=t->LineTop;n<t->e->LineCount;++n,++m){
	    if((y=m*t->LineHeight) > bottom)break;
	    y += t->LineHeight;
	    if(t->ShowLineNumbers){
	    	sprintf(buff,"%ld%c",n+1,0);
	    	uMoveTo((int)(t->CWidth+t->CWidth*(6-strlen(buff))),(int)(y-t->Descent+t->dataRect.y));
			uDrawString(buff,myIcon); 
	    }	    	    
	    Data=EditListLine(t->e,n);
	    if(!Data)continue;
	    nc=(int)strlen(Data);
	    CVideoDrawLine(myIcon,(int)left,(int)(y-t->Descent+t->dataRect.y),Data,nc);
	}
	CVideodoSelection(myIcon);
}
static void CVideoDrawLine(IconPtr myIcon,int x,int y,char *Line,int n)
{
	CEditPtr t;
	int k,m,c;
	
	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	if(!Line)return;

	m=0;
	for(k=0;k<n;++k){
	    uMoveTo(x,y);
	    if((c=Line[k]) == '\t'){
	        if(m++ >= t->LineLeft){CVideoDrawChar(myIcon,' ');x+=t->CWidth;};
	        while((m % t->TabSize) != 0){
	            if(m++ >= t->LineLeft){CVideoDrawChar(myIcon,' ');x+=t->CWidth;};
	        }
	    }else{
	        if(m++ >= t->LineLeft){CVideoDrawChar(myIcon,c);x+=t->CWidth;};
	    }
	}
}
static void CVideoDrawChar(IconPtr myIcon,int c)
{
	CEditPtr t;
	char buff[2];

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	buff[0]=c;
	buff[1]=0;
	uDrawString(buff,myIcon); 
}
static void CVideodoSelection(IconPtr myIcon)
{
	CEditPtr t;
	long nr;
	uRect Box;
	
	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;

	if(t->fRowHigh > (t->LineTop + t->yPageSize)){
	    return;
	}
	if(t->lRowHigh < t->LineTop){
	    return;
	}

	
	if(t->fRowHigh == t->lRowHigh){
	    if(t->fColumnHigh != t->lColumnHigh){
	        nr=t->fRowHigh;
	        Box.y=(int)(t->dataRect.y+(nr-t->LineTop)*t->LineHeight);
	        Box.ysize=t->LineHeight;
	        Box.x=(int)(t->dataRect.x+(t->fColumnHigh-t->LineLeft)*t->CWidth);
	        Box.xsize=(int)((t->lColumnHigh-t->fColumnHigh)*t->CWidth);
	        uInvertRect(&Box,myIcon);
	    }
	}else{
	    nr=t->fRowHigh;
	    Box.y=(int)(t->dataRect.y+(nr-t->LineTop)*t->LineHeight);
	    Box.ysize=t->LineHeight;
	    Box.x=(int)(t->dataRect.x+(t->fColumnHigh-t->LineLeft)*t->CWidth);
	    Box.xsize=t->dataRect.xsize;
	    uInvertRect(&Box,myIcon);
	    nr=t->lRowHigh;
	    Box.y=(int)(t->dataRect.y+(nr-t->LineTop)*t->LineHeight);
	    Box.ysize=t->LineHeight;
	    Box.x=(int)(t->dataRect.x);
	    Box.xsize=(int)((t->lColumnHigh-t->LineLeft)*t->CWidth);
	    uInvertRect(&Box,myIcon);
	    for(nr=t->fRowHigh+1;nr<=t->lRowHigh-1;++nr){
	        Box.y=(int)(t->dataRect.y+(nr-t->LineTop)*t->LineHeight);
	        Box.ysize=t->LineHeight;
	        Box.x=(int)(t->dataRect.x);
	        Box.xsize=t->dataRect.xsize;
	        uInvertRect(&Box,myIcon);
	    }
	}
	

}
static int doClose(IconPtr myIcon)
{
	CEditPtr t;

	if(!myIcon)return 1;

	t=(CEditPtr)myIcon->DWindow;
	if(t){
	    if(t->buffer)cFree((char *)t->buffer);
	    if(t->DeleteBp)cFree((char *)t->DeleteBp);
	    if(t->clipboard)cFree((char *)t->clipboard);
	    t->buffer=NULL;
	    if(t->e) EditListClose(t->e);
	    t->e=NULL;
	    if(t->Express)CExpressClose(t->Express);
	    t->Express = NULL;
	    
	    if(t->pathname)cFree((char *)t->pathname);
	    t->pathname = NULL;
	    
	    if(myIcon->Files)freeFileList(myIcon->Files);
	    myIcon->Files=NULL;
	    
	    cFree((char *)t);
	}
	myIcon=RemoveIcon(myIcon);
	return 0;
}

static int cdothumb(controlPtr control,IconPtr myIcon)
{
    control=control;
	if(myIcon){
	    CVideoMoveDy(myIcon);
	}
	return 0;
}
static int cgoDown(controlPtr control,short part,IconPtr myIcon)
{
	long oldv;

	part=part;
	oldv=uGetControlValue(control);
	uSetControlValue(control,oldv+1);
	if(oldv != uGetControlValue(control) && myIcon){
	    CVideoMoveDy(myIcon);
	}
	return 0;
}
static int cgoUp(controlPtr control,short part,IconPtr myIcon)
{
	long oldv;

	part=part;
	oldv=uGetControlValue(control);
	uSetControlValue(control,oldv-1);
	if(oldv != uGetControlValue(control) && myIcon){
	    CVideoMoveDy(myIcon);
	}
	return 0;
}
static int cdoPageDown(controlPtr control,short part,IconPtr myIcon)
{
	long oldv,dy;
	CEditPtr t;

	if(myIcon && (t=(CEditPtr)myIcon->DWindow)){
	    if(control == myIcon->VControl){
	        dy=(myIcon->uviewRect.ysize-MenuOffSet-ScrollOffSet)/t->LineHeight;
	    }else{
	        dy=(myIcon->uviewRect.xsize)/t->CWidth;
	    }
	}else{
	    dy=8;
	}

	part=part;
	oldv=uGetControlValue(control);
	uSetControlValue(control,oldv+dy);
	if(oldv != uGetControlValue(control) && myIcon){
	    CVideoMoveDy(myIcon);
	}
	return 0;
}
static int cdoPageUp(controlPtr control,short part,IconPtr myIcon)
{
	long oldv,dy;
	CEditPtr t;

	if(myIcon && (t=(CEditPtr)myIcon->DWindow)){
	    if(control == myIcon->VControl){
	        dy=(myIcon->uviewRect.ysize-MenuOffSet-ScrollOffSet)/t->LineHeight;
	    }else{
	        dy=(myIcon->uviewRect.xsize)/t->CWidth;
	    }
	}else{
	    dy=8;
	}
	part=part;
	oldv=uGetControlValue(control);
	uSetControlValue(control,oldv-dy);
	if(oldv != uGetControlValue(control) && myIcon){
	    CVideoMoveDy(myIcon);
	}
	return 0;
}
static void CVideoMoveDy(IconPtr myIcon)
{
	CEditPtr t;
	long loc;
	int dy;

	if(!myIcon)return;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return;
	if(!t->e)return;
	
	dy=0;
	if(myIcon->VControl){
	    loc=uGetControlValue(myIcon->VControl);
	    if(loc != t->LineTop){
	        dy=1;
	        t->LineTop=loc;
	    }
        if(t->LineTop > (t->e->LineCount-1)){
			t->LineTop=t->e->LineCount-1;
	    }
        if(t->LineTop < 0){
			t->LineTop=0;
	    }
	    uSetControlValue(myIcon->VControl,t->LineTop);
	    
	}

	if(myIcon->HControl){
	    loc=uGetControlValue(myIcon->HControl);
	    if(loc != t->LineLeft){
	        dy=1;
	        t->LineLeft=loc;
	    }

	    if(t->LineLeft < 0){
        	t->LineLeft=0;
	    }
	    uSetControlValue(myIcon->HControl,t->LineLeft);
	}

	if(dy){
	    EditDrawIt(myIcon);
	}
}
int CEditdoOpen(IconPtr myIcon,struct FileList *Files)
{
	char file[2048],buff[256];
	struct FileInfo2 *Files2;
	CEditPtr w;
	char *pathname;
	static long nTek;
	int ret;

	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	
	ret=1;
	
	Files2=NULL;
	
	if(!Files){
	   Files=FileDialog(NULL,"Expression File To Open",FileDialog_GetFiles,&myIcon->infoLoad);
	   if(!Files)return 1;	
	   	copyInfo(&myIcon->infoLoad,&myIcon->infoSave);
	}
	
	pathname=strpathname(Files->directory,Files->name[0]);
	if(!pathname)return 1;			
	
	if(Files){		
	    if(myIcon->Files)freeFileList(myIcon->Files);
	    myIcon->Files=NULL;
		
		Files->type=TEXT_MEMORY;	    
		Files2=initFILES(Files);
	    if(!Files2){
	        WarningBatch("CEditdoOpen out of Memory\n");
	        goto ErrorOut;
	    }
		Files2->fileClass=TEXT_MEMORY;
		Files2->pdefault=FilesOpenEdit;
		myIcon->Files=Files2;
		
	}

	
	if(w->e && (w->e->LineCount > 0)){
	    myIcon=NewCEdit(myIcon,NULL);
	    if(!myIcon)return 1;
	    w=(CEditPtr)myIcon->DWindow;
	    if(!w)return 1;  
	}
	
    if(w->pathname)cFree((char *)w->pathname);
    w->pathname=pathname;
    if(w->e) EditListClose(w->e);
    w->e=NULL;

	w->e=EditNewFile(pathname);
			
	if(!w->e)goto ErrorOut;
	
	mstrncpy(file,pathname,2047);

	sprintf(buff,"[%ld]%c",nTek++,0);

	mstrncat(file,buff,2047);

	SetWindowToFileName(myIcon,Files2);
		
	SetWindow(myIcon);
	
	InvalRectMyWindow(myIcon);

	ret=0;
ErrorOut:

	return ret;
}
static int CVideodoGoToLine(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok",      {40,95,65,25},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {240,95,65,25},uDialogButton,uDialogNone},
      /*  2 */      {"Frames/sec data", {160,45,120,25},uDialogText,uDialogGetLong},
      /*  3 */      {"Line Number :",   {35,45,105,25},uDialogString,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,339,151};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	static long lineNumber=1;
    int reti;
    int ret;
    
	CEditPtr t;
	if(!myIcon)return 1;
	t=(CEditPtr)myIcon->DWindow;
	if(!t)return 1;
	
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Go To Line",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	reti=uDialogSet(uList,2L,
	              	              
	              uDialogSetDouble,(double)lineNumber,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
		
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
		
			
	lineNumber=uR[2].lreturn-1;
	if(lineNumber < 0)lineNumber=0;
	
	if(t->e && t->e->LineCount > 0){
		if(lineNumber >= t->e->LineCount)lineNumber=t->e->LineCount-1;
	}else{
		lineNumber=0;
	}
	
	
	
	t->CursorRow=lineNumber;
	t->CursorColumn=0;
	t->fRowHigh=t->lRowHigh=t->CursorRow;
	t->fColumnHigh=0;
	t->lColumnHigh=0;
	
	CVideoCursorInView(myIcon);
	InvalRectMyWindow(myIcon);
	
	lineNumber++;
	
	ret = 0;
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int CVideodoOptions(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	{
      /*  0 */      {"Ok", {155,123,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {230,123,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"show line numbers", {20,20,180,20},uDialogCheck,uDialogGetLong},
    };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {50,50,435,178};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
	CEditPtr w;

	if(!myIcon)return 1;
	w=(CEditPtr)myIcon->DWindow;
	if(!w)return 1;
	
			
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Options",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetLogical,(int)w->ShowLineNumbers,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	w->ShowLineNumbers = (int)uR[2].lreturn;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);	
	
	return ret;
}
int doUniversalNewEdit(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct CEdit ss;
	static struct Icon mm;

	static struct universalTableListStruct Local2[] = {
		{"REAL",2201,universal_TypeInt,(void *)&mm.eraseRectSize},
		{"STRI",2202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",2203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"FILE",2218,universal_TypeFilesPtr,(void *)&mm.Files},	
		{"STRI",2222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",2223,universal_TypeInt,(void *)&mm.WindowNameSet},									
	};
				
	static struct universalTableStruct Global2 = {
		"NewEdit parameters",universal_WindowNewEdit,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};


	static struct universalTableListStruct Local[] = {
		{"RECT",2004,universal_TypeRect,(void *)&ss.windowRect},	
		{"REAL",2005,universal_TypeLong,(void *)&ss.FrameFirst},									
		{"REAL",2006,universal_TypeLong,(void *)&ss.FrameLast},									
		{"REAL",2007,universal_TypeLong,(void *)&ss.FrameIncrement},									
		{"STRI",2008,universal_TypeString,(void *)&ss.controller[0],sizeof(ss.controller)},
		{"REAL",2009,universal_TypeInt,(void *)&ss.ShowLineNumbers},
		{"TABL",2200,universal_TypeTable,(void *)&Global2},	
	};
	
	static struct universalTableStruct Global= {
		"NewEdit parameters",universal_WindowNewEdit,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CEditPtr s;
	int ret;
	
	s=NULL;
	if(myIcon && (myIcon->WindowType == ExpressionType)){
		s=(CEditPtr)myIcon->DWindow;
	}else{
	    myIcon=NULL;
	}

	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct CEdit);

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
			mm.eraseRectSize = 120;
			
			ss.FrameFirst=0;
			ss.FrameLast=0;
			ss.FrameIncrement=1;
			uSetRect(&ss.windowRect,117,45,575,434);
        	mstrncpy(ss.controller,"",255);
			
		}
		ss.defaultsSet=TRUE;
		
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;
		if(myIcon && uPref.Files){
			CEditdoOpen(myIcon,uPref.Files);
		    uPref.Files=NULL;
		    goto LoadIt;
		}			
	}else if(flag == universal_GetDefaults){		
LoadIt:
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
		sprintf(WarningBuff,"doUniversalNewEdit unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
