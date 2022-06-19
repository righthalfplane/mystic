#define EXTERN22 extern
#include "Xappl.h"
#include "fileSend.h"
#include "uDialog.h"
#include "Scanfile.h"
#include "uFiles.h"
#include "Universal.h"

char *DefaultPathString(void);
int CreateNewDirectory(void);
int goCD(char *name);
int GetTarget(char *SendTarget,int *SendPort,char *SendPassword,int *ErrorCheck);
int IsOption(void);


int pDoFontMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);
int pDoStyleMenu(IconPtr myIcon,int FontMenuNumber,int StyleMenuNumber,int Item);

extern int CatalogHiddenShow;

static int doCut(struct uDialogStruct *uList);
static int doCopy(struct uDialogStruct *uList);
static int doPaste(struct uDialogStruct *uList);
static int doClear(struct uDialogStruct *uList);

static int doScale2Double(struct uDialogStruct *uList);
static int SelectDirectory(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int FillList(struct uDialogStruct *uList);
static int NewDirectory(struct uDialogStruct *uList);
static int doScale2Keys(struct uDialogStruct *uList);
static int doScale2Move(struct uDialogStruct *uList);
static int SetButtons(struct uDialogStruct *uList);


static int CheckMenu(IconPtr myIcon,int flag);
static int Menu(IconPtr myIcon,int menu,int item);
static int fClose(IconPtr myIcon);
static int SetWindow(struct uDialogStruct *uList,int type);
static int SendSelected(struct uDialogStruct *uList);

extern int IsShift(void);

static int WriteSendWindow(IconPtr myIcon,char *mess);

IconPtr NewFileSendWindow(IconPtr myIconi)
{
 	uPort Port;
 	int ret;
 	
	static struct uMenuItems File[]={
	    {"File",  					FileCDraw,uWindowStandardMenu,0,},
	    {"New                 /N",	FileNew,0,0,},
	    {"Close               /W", 	FileClose,0,0,},
	    {"----------------------",	3,0,0,},
	    {"Save Preferences",		savePreferencesCommand,0,0,},
	    {"Preferences...  ",		PreferencesCommand,0,0,},
	    {"----------------------",	3,0,0,},
	    {"Quit                /Q",	FileQuit,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuItems Edit[]={
	    {"Edit",  				EditCDraw,uWindowStandardMenu,0,},
	    {"Undo              /Z",1,0,0,},
	    {"---------------------",2,0,0,},
	    {"Cut               /X",EditCut,0,0,},
	    {"Copy              /C",EditCopy,0,0,},
	    {"Paste             /V",EditPaste,0,0,},
	    {"Clear",				 EditClear,0,0,},
	    {"---------------------",7,0,0,},
	    {"Information        /I",EditInformation,0,0,},
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
	
			
	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    {Fonts},
	    {Styles},
	   	    
	    {NULL}
	};

 	 	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"New", {404,174,115,30},uDialogButton,uDialogNone},
      /*  1 */      {"Close", {404,74,115,30},uDialogButton,uDialogNone},
      /*  2 */      {"Quit", {404,268,115,30},uDialogButton,uDialogNone},
      /*  3 */      {"Up", {404,20,50,30},uDialogButton,uDialogNone},
      /*  4 */      {"Scroll", {25,35,349,233},uDialogScrollList,uDialogNone},
      /*  5 */      {"File Menu", {143,6,180,25},uDialogPopUpMenuControl,uDialogNone},
      /*  6 */      {"Scroll", {25,35,349,233},uDialogScrollList,uDialogNone},
      /*  7 */      {"Hidden", {5,170,70,30},uDialogCheck,uDialogGetLong},
      /*  8 */      {"Status", {10,35,70,20},uDialogButtonRadio,uDialogGetLong},
      /*  9 */      {"Local", {10,55,70,20},uDialogButtonRadio,uDialogGetLong},
      /* 10 */      {"Text", {10,85,70,20},uDialogButtonRadio,uDialogGetLong},
      /* 11 */      {"Binary", {10,105,70,20},uDialogButtonRadio,uDialogGetLong},
      /* 12 */      {"MacBinary", {10,125,90,20},uDialogButtonRadio,uDialogGetLong},
      /* 13 */      {"Raster", {10,145,70,20},uDialogButtonRadio,uDialogGetLong},
      /* 14 */      {"Options", {404,174,115,30},uDialogButton,uDialogNone},
      /* 15 */      {"Clear", {404,174,115,30},uDialogButton,uDialogNone},
      /* 16 */      {"Refresh", {404,174,115,30},uDialogButton,uDialogNone},
      /* 17 */      {"Send", {404,174,115,30},uDialogButton,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {117,45,575,434};
 	 	
	extern int doNetWork;
 	uDialogList *uList;

	uDialogReturn *uR;
 	char buff[256];
	static int Count;
	IconPtr myIcon; 	
	netPtr n;
 	 		
	uGetPort(&Port);
	
	ret=1;
	uR=NULL;
	uList=NULL;
	myIcon=NULL;	
	myIconi=myIconi;
 	
	if((n=(struct uNetFile *)cMalloc(sizeof(struct uNetFile),8183)) == NULL){
		WarningBatch("NewFileSendWindow Out of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)n,sizeof(struct uNetFile));
	
	myIcon=(IconPtr)n;
	
    n->SendPort=3600;
    mstrncpy(n->SendTarget,"gamma.lanl.gov",255);
    mstrncpy(n->SendPassword,"",255);
    n->netErrorCheck=0;
    
	
	n->Net=NetStart(myIcon);
	if(n->Net == NULL){
	    WarningBatch("NetWorking Has Been Disabled\n");
		doNetWork=FALSE;
	    goto ErrorOut;
	}

	sprintf(buff,"FileSend_%03d",Count++);
		
	uList=uDialogOpen(buff,&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
    
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,

	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetKeys,doScale2Keys,
	              uDialogSetMove,doScale2Move,
	              uDialogSetDoubleClick,doScale2Double,
	              
	              uDialogSetMyIcon,myIcon,
	              	              
	              uDialogSetModeLess,TRUE,	   
	              
	              uDialogSetItemClose,fClose,
	              
	              uDialogWindowType,SendWindowType,
	                         	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
    
 				
	if(uDialog(uList))goto ErrorOut;    
	
	myIcon=uList->myIcon;
	
	if(uSetWindow(myIcon,	       
              uWindowSetWindowName,buff,
			  uWindowSetMenuList,menuList,
              uWindowSetDone,uWindowSetDone)
	)goto ErrorOut;
	
  	SetPortMyWindow(myIcon);
  	
	SetWindow(uList,n->idialog);
 	
	ret=0;
ErrorOut:

	if(ret){
    	if(uList)uDialogClose(uList);
    	myIcon=NULL;
    	uList=NULL;
	}else{
		if(myIcon)myIcon->isOpen=TRUE;
    	if(uList)doScale2Move(uList);
	}
	

    if(uR)uDialogFreeReturn(uR);
    
	if(!myIcon)uSetPort(&Port);

	return myIcon;

}
static int doScale2Double(struct uDialogStruct *uList)
{
	int actionItem;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 4){
	   if(SelectDirectory(uList))goto OutOfHere;
	}
OutOfHere:
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int SetWindow(struct uDialogStruct *uList,int type)
{
	netPtr n;

	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;

	n->idialog=type;


	if(uDialogSet(uList,0L,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,3L,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,4L,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,5L,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,6L,
	              uDialogSetHidden,(n->idialog != B_STATUS),
	              
	              uDialogSetItem,7L,
	              uDialogSetLogical,CatalogHiddenShow,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,8L,
	              uDialogSetLogical,(n->idialog == B_STATUS),
	          	  uDialogSetRadioGroup,1,
	              
	              uDialogSetItem,9L,
	              uDialogSetLogical,(n->idialog == B_LOCAL),
	          	  uDialogSetRadioGroup,1,
	          	  
	              uDialogSetItem,10L,
	              uDialogSetLogical,(n->SendMode  == TextFile),
	          	  uDialogSetRadioGroup,2,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	          	  
	              uDialogSetItem,11L,
	              uDialogSetLogical,(n->SendMode  == BinaryFile),
	          	  uDialogSetRadioGroup,2,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	          	  
	              uDialogSetItem,12L,
	              uDialogSetLogical,(n->SendMode  == MacBinaryFile),
	          	  uDialogSetRadioGroup,2,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,13L,
	              uDialogSetLogical,(n->SendMode  == RasterFile),
	          	  uDialogSetRadioGroup,2,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	          	  
	              uDialogSetItem,15L,
	              uDialogSetHidden,(n->idialog != B_STATUS),
	              
	              uDialogSetItem,16L,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetItem,17L,
	              uDialogSetHidden,(n->idialog != B_LOCAL),
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
    
ErrorOut:
	SetButtons(uList);
	return 0;
}
static int SetButtons(struct uDialogStruct *uList)
{
	struct ScrollList *list4;
	controlPtr control;
	netPtr n;
	int ret;
	
	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;

	list4=NULL;
	control=NULL;
	
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,4L,
              uDialogGetScrollList,&list4,
              uDialogSetItem,17L,
              uDialogGetControl,&control,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto OutOfHere;
		
	if(list4 && ScrollListIsSelectAny(list4)){
	    if(control)uHiliteControl(control,0);
	}else{
	    if(control)uHiliteControl(control,255);
	}
OutOfHere:
	return 0;
}
static int SendSelected(struct uDialogStruct *uList)
{
	struct ScrollList *list4;
   	struct FileList *Files;
   	long lcount,count,nn;
   	char pathname[256];
	uDialogReturn *uR;
	netPtr n;
	int ret;
	
	if(!uList)return 1;
	if(!uList->myIcon)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;

	
	list4=NULL;
	Files=NULL;
	
	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    goto ErrorOut;
    }
    
    
    
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,4L,
              uDialogGetScrollList,&list4,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto ErrorOut;
	
	ret=1;
	
	if(!list4 || !ScrollListIsSelectAny(list4)){
	    WarningBatch("Nothing Selected to Send\n");
	    goto ErrorOut;
	}

	lcount=ScrollListCount(list4);
			
	count = 0;
	
	for(nn=0;nn<lcount;++nn){
 	    if(ScrollListIsSelected(nn,list4)){
 	        ++count;
	    }
	}
	
	if(count <= 0)goto ErrorOut;
		
	Files=(struct FileList *)cMalloc(sizeof(struct FileList),8105);
	if(!Files)goto ErrorOut;
	zerol((char *)Files,sizeof(struct FileList));
		
	if(n->directory){
 		Files->directory=strsave(n->directory,1024);
 	}else{
 		Files->directory=DefaultPathString();
 	}
 	if(!Files->directory)goto ErrorOut;

	Files->name=(char **)cMalloc(count*sizeof(char *),8106);
	if(!Files->name){
	    goto ErrorOut;
	}
	zerol((char *)Files->name,sizeof(count*sizeof(char *)));
	Files->count=count;
	
	count = 0;
	for(nn=0;nn<lcount;++nn){
 	    if(ScrollListIsSelected(nn,list4)){
 	        if(ScrollListGet(pathname,nn,list4))goto ErrorOut;
 	        Files->name[count]=strsave(pathname,8227);
 	        if(!Files->name[count++])goto ErrorOut;
	    }
	}
	
    n->SendMode=BinaryFile;    
    if(uR[10].lreturn){
    	n->SendMode=TextFile;
    }else if(uR[11].lreturn){
    	n->SendMode=BinaryFile;
    }else if(uR[12].lreturn){
    	n->SendMode=MacBinaryFile;
    }else if(uR[13].lreturn){
    	n->SendMode=RasterFile;
    }
    
    n->Files=Files;
    
    n->myIcon=uList->myIcon;
    
    n->myIcon->pdoWriteStatus=WriteSendWindow;

	SendFileList(n);

	ret=0;
	
ErrorOut:
    if(uR)uDialogFreeReturn(uR);
	if(Files){
	    goCD(Files->directory);
		if(n->directory){
	 		cFree(n->directory);
	 	}
	 	n->directory=DefaultPathString();	   
	    FileListFree(Files);
	}
    n->Files=NULL;
	return 0;
}
static int WriteSendWindow(IconPtr myIcon,char *mess)
{
	struct uDialogStruct *uList;
	struct ScrollList *list6;
	long count;
	int type=3;
	char **mp;
	netPtr n;
	int ret;
	
	if(!myIcon || !mess)return 1;
	
	uList=uDialogGetList(myIcon);
	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
		
	list6=NULL;
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,6L,
              uDialogGetScrollList,&list6,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret || !list6)goto OutOfHere;

	count=ScrollListCount(list6);
	mp = &mess;
	ScrollListInsert(mp,&type,count,1L,list6);
	ScrollListUpdate(list6);
		
OutOfHere:
	return 0;
	
}
static int doScale2Action(struct uDialogStruct *uList)
{
	struct ScrollList *list6;
	netPtr n;

	int actionItem;
	long count;
	int reti;
	int ret;
	
	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 0){
		CreateNewDirectory();
		FillList(uList);
	}else if(actionItem == 1){
		reti=uDialogSet(uList,0L,
	      uDialogSetExitOk,
	      uDialogSetDone,uDialogSetDone
		);
		if(reti)goto OutOfHere;
	}else if(actionItem == 2){
		doQuit();
		return 1;
	}else if(actionItem == 5){
	   NewDirectory(uList);
	}else if(actionItem == 7){
	    CatalogHiddenShow = !CatalogHiddenShow;
		FillList(uList);
	}else if(actionItem == 8){
	    SetWindow(uList,B_STATUS);
	}else if(actionItem == 9){
	    SetWindow(uList,B_LOCAL);
	}else if(actionItem == 3){
	   goCD("..");
		if(n->directory){
	 		cFree(n->directory);
	 	}
	 	n->directory=DefaultPathString();	   
	   if(FillList(uList))goto OutOfHere;
	}else if(actionItem == 14){
		GetTarget(n->SendTarget,&n->SendPort,
	          n->SendPassword,&n->netErrorCheck);
	}else if(actionItem == 15){
		ret=uDialogSet(uList,0L,                            	              	              
	              uDialogSetItem,6L,
	              uDialogGetScrollList,&list6,
	              uDialogSetDone,uDialogSetDone
	    );
		
		if(ret)goto OutOfHere;
		count=ScrollListCount(list6);
		if(count > 0){
			ScrollListDelete(0l,count,list6);
		}
	}else if(actionItem == 16){
		FillList(uList);
	}else if(actionItem == 17){
	    SendSelected(uList);
	}
	

OutOfHere:
	if(uList){
		SetButtons(uList);
		if(uList)uDialogUpdate(uList);
		uSetCursor(uList->myIcon,-uARROW_CURSOR);
	}
	return 0;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	netPtr n;

	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
		
	if(!IsShift()){
		doUniversalFileSend(uList->myIcon,universal_GetDefaults,NULL);
	
		uMoveWindow(uList->myIcon,n->windowRect.x,n->windowRect.y);
		uSizeWindowGlobal(uList->myIcon,n->windowRect.xsize,n->windowRect.ysize);
		
		if(n->directory){
		    goCD(n->directory);
		}
	}
	
	if(FillList(uList))return 1;
		
	uList->myIcon->pCheckMenu=CheckMenu;
	uList->myIcon->pMenu=Menu;
	
	return 0;
}
static int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	struct ScrollList *list6;
	uRect rList1;
	uRect rList6;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetItem,4L,
              uDialogGetRect,&rList1,
              uDialogGetScrollList,&list2,
                            	              	              
              uDialogSetItem,6L,
              uDialogGetRect,&rList6,
              uDialogGetScrollList,&list6,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto ErrorOut;
	
	rList1.x = 105;
	rList1.y = 35+MenuOffSet;
	rList1.xsize = (r.xsize-rList1.x-80);
	rList1.ysize = r.ysize-MenuOffSet-90-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	
	ScrollListFitRect(&rList1,list6);
	
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,r.xsize/2-60,r.ysize-85,	
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize-135,r.ysize-85,	
                            
              uDialogSetItem,2L,
              uDialogSetLocation,r.xsize-135,r.ysize-40,	
                            
              uDialogSetItem,3L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,5+MenuOffSet,
              	
              uDialogSetItem,4L,
              uDialogSetRect,&rList1,
                                                                                                                                                                        
              uDialogSetItem,5L,
              uDialogSetLocation,rList1.x+(rList1.xsize)/2-80,5+MenuOffSet,	
              
              uDialogSetItem,6L,
              uDialogSetRect,&rList1,

              uDialogSetItem,14L,
              uDialogSetLocation,r.xsize/2-60,r.ysize-40,	
              
              uDialogSetItem,15L,
              uDialogSetLocation,r.xsize/2-60,r.ysize-85,	
              
              
              uDialogSetItem,16L,
              uDialogSetLocation,10,r.ysize-40,	
              
              uDialogSetItem,17L,
              uDialogSetLocation,10,r.ysize-85,	
              
              uDialogSetDone,uDialogSetDone
    );
	
	uDialogUpdate(uList);
ErrorOut:	
	return 0;
}
static int doScale2Keys(struct uDialogStruct *uList)
{

	struct ScrollList *List1;
	int ret;
	
	
	if(!uList)return 1;
	
	ret=uDialogSet(uList,4L,
              uDialogGetScrollList,&List1,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	if(!List1)return 1;
	
	ScrollListSetString(uList->mess,List1);
/*
	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);	
*/
	return 0;
}
static int SelectDirectory(struct uDialogStruct *uList)
{
	struct ScrollList *List;
	char buff[256];
	long location;
	netPtr np;
	int ret;
	
	if(!uList)return 1;
	np=(netPtr)uList->myIconItem;
	if(!np)return 1;
		
	
	ret=uDialogSet(uList,4L,
              uDialogGetScrollList,&List,                            	              	                            
              uDialogSetDone,uDialogSetDone
    );
	if(ret)return 1;
	
	location=ScrollListFirstSelect(List);
	if(location < 0)return 1;
	
	if(ScrollListGet(buff,location,List))return 1;
	
	if(goCD(buff)){
	    sprintf(WarningBuff,"%s Is Not A Directory",buff);
	    Warning(WarningBuff);
	    return 1;
	}
	if(np->directory){
 		cFree(np->directory);
 	}
 	np->directory=DefaultPathString();	   
	
	FillList(uList);
	
	return 0;
}
static int NewDirectory(struct uDialogStruct *uList)
{
	controlPtr controlFile;
	menuPtr menuFile;
	int location;
	netPtr np;
	int ret;
	int n;
	
	if(!uList)return 1;
	np=(netPtr)uList->myIconItem;
	if(!np)return 1;
	
	ret=uDialogSet(uList,5L,
              uDialogGetControlMenu,&menuFile,                            	              	              
              uDialogGetControl,&controlFile,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	location=(int)uGetControlValue(controlFile);
	
	for(n=1;n<location;++n){
	   goCD("..");
	}
	if(np->directory){
 		cFree(np->directory);
 	}
 	np->directory=DefaultPathString();	   
	FillList(uList);
	
	return 0;
}
    
static int FillList(struct uDialogStruct *uList)
{
	struct ScrollList *List;
	struct scanList *filelist;
	controlPtr controlFile;
	menuPtr menuFile;
	long count;
	netPtr np;
	int ret;
	int n;
	
	if(!uList)return 1;
	np=(netPtr)uList->myIconItem;
	if(!np)return 1;

	ret=uDialogSet(uList,4L,
              uDialogGetScrollList,&List,
                            	              	              
	          uDialogSetItem,5L,
              uDialogGetControlMenu,&menuFile,
              uDialogGetControl,&controlFile,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	
    filelist=NULL;
	
	count=ScrollListCount(List);
	if(count){
		ScrollListDelete(0L,count,List);	
	}	
	
	if(np->directory)goCD(np->directory);
	
    filelist=scan_dir(".",".*");
    if(filelist == NULL){
        sprintf(WarningBuff,"scan_dir Error in FillList\n");
        WarningBatch(WarningBuff);
		goto OutOfHere;
    }
    
    count=0;
    
    if(filelist->countVol > 0){
		if(ScrollListInsertOneType(filelist->namesVolumes,4L,count,(long)(filelist->countVol),List)){
		   sprintf(WarningBuff,"ScrollListInsertError in FillList\n");
		   WarningBatch(WarningBuff);
		   goto OutOfHere;
		}
    	count += filelist->countVol;
    }
    
    if(filelist->countDirs > 0){
		if(ScrollListInsertOneType(filelist->namesDirs,1L,count,(long)(filelist->countDirs),List)){
		   sprintf(WarningBuff,"ScrollListInsertError in FillList\n");
		   WarningBatch(WarningBuff);
		   goto OutOfHere;
		}
    	count += filelist->countDirs;
    }
    
    if(filelist->countFiles > 0){
		if(ScrollListInsertOneType(filelist->namesFiles,0L,count,(long)(filelist->countFiles),List)){
		   sprintf(WarningBuff,"ScrollListInsertError in FillList\n");
		   WarningBatch(WarningBuff);
		   goto OutOfHere;
		}
    	count += filelist->countFiles;
    }
    
    ScrollListSort(LIST_SORT_ASCII,List);
    
	count=uCountMenuItems(menuFile);
	for(n=0;n<count;++n){
	    uDeleteMenuItem(menuFile,1);
	}
	if(filelist->countPwd > 0){
	    for(n=0;n<filelist->countPwd;++n){
 			uInsertMenuItem(menuFile,filelist->namesPwd[n],500);
		}
		uSetControlMaximum(controlFile,filelist->countPwd);
		uSetControlValue(controlFile,1);
    }

    ScrollListResetDoubleClick(List);
    
OutOfHere:    

	uDialogUpdate(uList);
	if(filelist)free_dir(filelist);
	return 0;
}
static int CheckMenu(IconPtr myIcon,int flag)
{

	if(!myIcon)return 1;
		
	if(flag){
	    doWindowMenu();
	}
	return 0;
}
static int Menu(IconPtr myIcon,int menu,int item)
{
	struct uDialogStruct *uList;
	int ret;

	if(!myIcon)return 1;
	uList=uDialogGetList(myIcon);
	if(!uList)return 1;
	
	uSetCursor(myIcon,-uWATCH_CURSOR);
	
	switch(menu){
	case AppleMenu:
		if(item == 1){
		    doAbout(myIcon);
		    break;
		}
		break;
	case FileCDraw:
		if(item == FileNew){
		    NewNewWindow(myIcon);
		    break;
		}else if(item == FileClose){
		    if(myIcon && myIcon->pClose){
		        (*myIcon->pClose)(myIcon);
		    }
		    myIcon=NULL;
		    break;
		} else if(item == savePreferencesCommand){		
	    	writeUniversal(myIcon,0);
		} else if(item == PreferencesCommand){		
			saveUniversal(myIcon);
		}else if(item == FileQuit){
			doQuit();
		    myIcon=NULL;
		    break;
		}
		break;
	case WindowMenu:
		pWindowMenu(myIcon,item);
		break;
		
	case FontCDraw:
		pDoFontMenu(myIcon,FontCDraw,StyleCDraw,item);
		
		ret=uDialogSet(uList,4L,
              uDialogSetFont,item,
                            	              	              
	          uDialogSetItem,6L,
              uDialogSetFont,item,
              
	          uDialogSetDone,uDialogSetDone
        );
    
		if(ret)goto ErrorOut;
		
		doScale2Move(uList);
		
		break;
	case StyleCDraw:
		
		pDoStyleMenu(myIcon,FontCDraw,StyleCDraw,item);
		
		ret=uDialogSet(uList,4L,
              uDialogSetFontStyle,item,
                            	              	              
	          uDialogSetItem,6L,
              uDialogSetFontStyle,item,
              
	          uDialogSetDone,uDialogSetDone
        );
    
		if(ret)goto ErrorOut;
		
		doScale2Move(uList);
		
		break;
				
		
	case EditCDraw:
		switch(item){
 		case EditCut:	 doCut(uList);break;
 		case EditCopy:	 doCopy(uList);break;
 		case EditPaste:	 doPaste(uList); break;
 		case EditClear:	 doClear(uList);break;
	    case EditInformation:  doWindowInformation(myIcon);break;
		}
		break;
	}
ErrorOut:
	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
static int doCut(struct uDialogStruct *uList)
{
	struct ScrollList *list6;
	netPtr n;
	int ret;

	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
	
	list6=NULL;
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,6L,
              uDialogGetScrollList,&list6,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret || !list6)goto OutOfHere;
		
OutOfHere:
	return 0;
}



static int doCopy(struct uDialogStruct *uList)
{
	struct ScrollList *list6;
	netPtr n;
	int ret;

	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
	
	list6=NULL;
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,6L,
              uDialogGetScrollList,&list6,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret || !list6)goto OutOfHere;
		
OutOfHere:
	return 0;
}
static int doPaste(struct uDialogStruct *uList)
{
	struct ScrollList *list6;
	netPtr n;
	int ret;

	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
	
	list6=NULL;
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,6L,
              uDialogGetScrollList,&list6,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret || !list6)goto OutOfHere;
		
OutOfHere:
	return 0;
}
static int doClear(struct uDialogStruct *uList)
{
	struct ScrollList *list6;
	netPtr n;
	int ret;

	if(!uList)return 1;
	n=(netPtr)uList->myIconItem;
	if(!n)return 1;
	
	list6=NULL;
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,6L,
              uDialogGetScrollList,&list6,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret || !list6)goto OutOfHere;
		
OutOfHere:
	return 0;
}

static int fClose(IconPtr myIcon)
{
	netPtr n;
		
	if(myIcon){
	    n=(netPtr)myIcon;
   	    if(n){
		    if(n->Net){
		        NetStop(n->Net);
		        n->Net=NULL;
		    }
		    if(n->directory)cFree(n->directory);
		    cFree((char *)n);
	    }
	       	    
	}
	return 0;

}
int GetTarget(char *SendTarget,int *SendPort,char *SendPassword,int *ErrorCheck)
{
	static uDialogGroup Items[] =
	{
      /*  0 */      {"Ok", {155,123,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {230,123,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"target", {135,10,276,21},uDialogText,uDialogNone},
      /*  3 */      {"Port", {135,42,275,18},uDialogText,uDialogGetLong},
      /*  4 */      {"Password", {135,71,273,20},uDialogText,uDialogNone},
      /*  5 */      {"Error Checking", {16,122,130,20},uDialogCheck,uDialogGetLong},
      /*  6 */      {"Send Target", {16,16,92,16},uDialogString,uDialogNone},
      /*  7 */      {"Send Port", {15,42,80,15},uDialogString,uDialogNone},
      /*  8 */      {"Send Password", {13,76,109,15},uDialogString,uDialogNone},
    };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {50,50,435,178};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    
	if(!SendTarget || !SendPort || !SendPassword || !ErrorCheck)return 1;
		
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Send Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetText,SendTarget,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)*SendPort,
	              
	              uDialogSetItem,4L,
	              uDialogSetText,SendPassword,
              
	              uDialogSetItem,5L,
	              uDialogSetLogical,*ErrorCheck,
	              	              
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	mstrncpy(SendTarget,uR[2].sreturn,255);

	mstrncpy(SendPassword,uR[4].sreturn,255);

	*SendPort = (int)uR[3].lreturn;
	*ErrorCheck = (int)uR[5].lreturn;
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
}
int doUniversalFileSend(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct uNetFile ss;
	static struct Icon mm;

	static struct universalTableListStruct Local2[] = {
		{"STRI",8202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",8203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
		{"STRI",8222,universal_TypeString,(void *)&mm.WindowName[0],sizeof(mm.WindowName)},
		{"REAL",8223,universal_TypeInt,(void *)&mm.WindowNameSet},									
	};
				
	static struct universalTableStruct Global2 = {
		"NewEdit parameters",universal_WindowNewEdit,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};

	static struct universalTableListStruct Local[] = {
		{"RECT",8001,universal_TypeRect,(void *)&ss.windowRect},
		{"STRI",8002,universal_TypeString,(void *)&ss.SendTarget[0],sizeof(ss.SendTarget)},
		{"STRI",8003,universal_TypeString,(void *)&ss.SendPassword[0],sizeof(ss.SendPassword)},
		{"REAL",8004,universal_TypeInt,(void *)&ss.SendPort},
		{"STRI",8005,universal_TypeStringPtr,(void *)&ss.directory},
		{"TABL",8200,universal_TypeTable,(void *)&Global2},	
		
	};
	
	static struct universalTableStruct Global= {
		"fileSend parameters",universal_WindowSendFile,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	struct uDialogStruct *uList;
	struct uNetFile *s;
	int ret;
	
	s=NULL;
	uList=NULL;
	if(myIcon && (myIcon->WindowType == SendWindowType)){
		uList=uDialogGetList(myIcon);
		if(uList){
			s=(netPtr)uList->myIconItem;
		}
	}
	
	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct uNetFile);

	Global2.tableDynamic=myIcon;
	Global2.tableDynamicSpace=sizeof(struct Icon);
			
	ret=1;
	if(flag == universal_SetDefaults){
	    if(s){
			if(uList && uList->myIcon){
				uGetPortBoundsGlobal(uList->myIcon,&s->windowRect);
			}else{
				uSetRect(&s->windowRect,117,45,575,434);
			}
 			if(moveUniversalTable(&Global,universal_MoveToStatic))goto ErrorOut;	
		}else{
			if(mm.infoSave.directory)cFree((char *)mm.infoSave.directory);
			mm.infoSave.directory=NULL;
			if(mm.infoLoad.directory)cFree((char *)mm.infoLoad.directory);
			mm.infoLoad.directory=NULL;
			if(ss.directory)cFree((char *)ss.directory);
			
			ss.directory=DefaultPathString();
        	ss.SendPort=3600;
        	mstrncpy(ss.SendTarget,"gamma.lanl.gov",255);
        	mstrncpy(ss.SendPassword,"",255);
        	ss.netErrorCheck=0;        
			uSetRect(&ss.windowRect,117,45,575,434);
		}
		ss.defaultsSet=TRUE;
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;				
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
		sprintf(WarningBuff,"doUniversalFileSend unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
