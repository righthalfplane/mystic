#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "fileDialog.h"
#include "uDialog.h"
#include "Scanfile.h"
#include "SaveAllImages.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Universal.h"

int doSavePlotsProcess(struct ImageSaveList *Images,CommandPtr cp,BatchPtr Batch);
int doSavePlotsBatch(BatchPtr Batch,char *name);

int CreateNewDirectory(void);
int goCD(char *name);

static int doScale2Double(struct uDialogStruct *uList);
static int SelectDirectory(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int FillList(struct uDialogStruct *uList);
static int NewDirectory(struct uDialogStruct *uList);
static int doScale2Keys(struct uDialogStruct *uList);
static int doScale2Move(struct uDialogStruct *uList);
static int BailButton(struct uDialogStruct *uList);

char *DefaultPathString(void);

int IsOption(void);

#define IMAGE_HDF_OUT 		0
#define IMAGE_PICT_OUT 		1
#define IMAGE_PICT32_OUT 	2
#define IMAGE_JPEG_OUT 		3
#define IMAGE_TRUE_OUT 		4
#define IMAGE_SDS2D_OUT 	5
#define IMAGE_QUICK08_OUT 	6
#define IMAGE_QUICK32_OUT 	7
#define IMAGE_256_OUT 		8
#define IMAGE_BMP_OUT 		9
#define IMAGE_AVI_OUT 		10
#define IMAGE_TEK_OUT 		11
#define IMAGE_RGB_OUT 		12


int GetSequenceInfo(struct SaveInformation *SaveInfo)
{
	
	
	static char out1[256]={'I','m','a','g','e',0,};
	static char out2[256]={0,};
			      
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {280,110,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {280,73,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"Seq-Data", {165,130,90,20},uDialogText,uDialogGetLong},
      /*  3 */      {"Output-Data", {165,80,90,20},uDialogText,uDialogSetText},
      /*  4 */      {"Use File names", {49,18,267,21},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"Use Sequence Name", {49,38,267,21},uDialogButtonRadio,uDialogGetLong},
      /*  6 */      {"Sequence Name :", {33,80,120,20},uDialogString,uDialogNone},
      /*  7 */      {"Seq #    :", {90,130,65,21},uDialogString,uDialogNone},
      /*  8 */      {"Output-Data", {165,105,90,20},uDialogText,uDialogSetText},
      /*  9 */      {"Output Suffix     :", {33,105,120,20},uDialogString,uDialogNone},
        };
	
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,361,187};
 	
 	uDialogList *uList;
 	
	uDialogReturn *uR;

    int ret;
 	
	if(!SaveInfo)return 1;
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Save Image Sequence Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
    
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetItem,3L,
	              uDialogSetText,out1,
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,SaveInfo->UseImageNames,
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,!SaveInfo->UseImageNames,
	              
	              uDialogSetItem,2L,
	              uDialogSetDouble,(double)SaveInfo->SequenceNumber,

	              uDialogSetItem,8L,
	              uDialogSetText,out2,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
    
	if(uDialog(uList))goto ErrorOut;    
	
	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    goto ErrorOut;
    }
  
	out1[0]=0;
	mstrncpy(out1,uR[3].sreturn,255);
	
	out2[0]=0;
	mstrncpy(out2,uR[8].sreturn,255);
	
	SaveInfo->prefix=out1;	
	SaveInfo->suffix=out2;	
	SaveInfo->UseImageNames=(int)uR[4].lreturn;   
	SaveInfo->SequenceNumber=uR[2].lreturn;
	    	
	ret=0;
 ErrorOut:
 
  	if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
     
 	return ret;
}

static int doWritePlots(struct SaveInformation *SaveInfo)
{
	struct ImageSaveList *Images;
	struct FileInfo2 *Files;
	int NeedSequenceInfo;
	int type;
	long n;
	int ret;
	
	if(!SaveInfo)return 1;
	Files=SaveInfo->Files;
	if(!Files)return 1;
	
	ret = 1;
	Images=NULL;
	NeedSequenceInfo = TRUE;
	switch(SaveInfo->PlotTypeOut){
	    case IMAGE_HDF_OUT:
			type=IMAGE_HDF;
	        break;
	    case IMAGE_PICT_OUT:
			type=IMAGE_PICT;
	        break;
	    case IMAGE_PICT32_OUT:
			type=IMAGE_PICT32;
	        break;
	    case IMAGE_JPEG_OUT:
			type=IMAGE_JPG;
	        break;
	    case IMAGE_TRUE_OUT:
			type=IMAGE_TRUE;
	        break;
	    case IMAGE_SDS2D_OUT:
			type=IMAGE_SDS2D;
	        break;
	    case IMAGE_QUICK08_OUT:
			type=IMAGE_QUICKTIME8;
			NeedSequenceInfo = FALSE;
	        break;
	    case IMAGE_QUICK32_OUT:
			type=IMAGE_QUICKTIME32;
			NeedSequenceInfo = FALSE;
	        break;
	    case IMAGE_256_OUT:
			type=IMAGE_HDF_TO_256;
	        break;
	    case IMAGE_BMP_OUT:
			type=IMAGE_BMP;
	        break;
	    case IMAGE_AVI_OUT:
			type=IMAGE_AVI;
			NeedSequenceInfo = FALSE;
	        break;
	    case IMAGE_TEK_OUT:
			type=IMAGE_TEK;
	        break;
	    case IMAGE_RGB_OUT:
			type=IMAGE_RGB;
	        break;
	    default:
	    	goto ErrorOut;
	}

	SaveInfo->prefix=NULL;	
	SaveInfo->suffix=NULL;	
	SaveInfo->SequenceNumber=0;

							
	if(SaveInfo->PlotModeOut == 0){
	
		Images=CreateImageList(1L);
		if(!Images)goto ErrorOut;		
	 	Images->imageList[0]=SaveInfo->CurrentFrame;
		SaveInfo->UseImageNames = FALSE;   
	 					
	}else{
	
		if(NeedSequenceInfo){
		    if(GetSequenceInfo(SaveInfo))goto ErrorOut;	
		}	
		
		Images=CreateImageList(Files->ImageCount);
		if(!Images)goto ErrorOut;
		
		for(n=0;n<Images->count;++n)Images->imageList[n]=n;
					
		if(SaveInfo->PlotModeOut == 1){
		    ;
		}else if(SaveInfo->PlotModeOut == 2){
			switch(SaveInfo->PlotTypeOut){
			    case IMAGE_HDF_OUT:
					type=IMAGE_HDF_MERGE;
			        break;
			    case IMAGE_TRUE_OUT:
					type=IMAGE_TRUE_MERGE;
			        break;
			    case IMAGE_SDS2D_OUT:
					type=IMAGE_SDS2D_MERGE;
			        break;
			    case IMAGE_256_OUT:
					type=IMAGE_HDF_TO_256_MERGE;
			        break;
			}
		}else {
		   goto ErrorOut;
		}
	}
	

	Images->savename=(char *)SaveInfo->PlotName;	
	Images->type=type;
	Images->Files=Files;
	Images->DRect=SaveInfo->DRect;
	Images->directory=DefaultPathString();	
	Images->prefix=SaveInfo->prefix;
	Images->suffix=SaveInfo->suffix;
	Images->UseImageNames=SaveInfo->UseImageNames;
	Images->SequenceNumber=SaveInfo->SequenceNumber;
	
	if(type == IMAGE_QUICKTIME8 || type == IMAGE_QUICKTIME32){
		if(doSaveQuickTimeOptions(SaveInfo->myIcon,Images))goto ErrorOut;		
	}
	
	doSaveImages(SaveInfo->myIcon,Images);
	ret = 0;
ErrorOut:
	if(Images)ImageFreeList(Images);
	
	return ret;
}
int doSavePlotsProcess(struct ImageSaveList *Images,CommandPtr cp,BatchPtr Batch)
{
	static char savename[256]={'I','m','a','g','e','.','s','2','d',0,};
	static char out1[256]={'I','m','a','g','e',0,};
	char *command;
	double value;
	
	if(!Images || !cp || !Batch)return 1;


	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	if(!strcmp("cd",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    goCD(command);
	}else if(!strcmp("prefix",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(out1,command,256);
	}else if(!strcmp("savename",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(savename,command,256);
	}else if(!strcmp("UseImageNames",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Images->UseImageNames=(int)value;
	}else if(!strcmp("SequenceNumber",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Images->SequenceNumber=(int)value;
	}else if(!strcmp("end",command)){
		Images->directory=DefaultPathString();	
		Images->prefix=out1;
		Images->savename=savename;	
		doSaveImages(Batch->myIcon,Images);	
	    return  1;
	}
	return 0;
ErrorOut:
	return 1;
}
int doSavePlotsBatch(BatchPtr Batch,char *name)
{
	struct ImageSaveList *Images;
	struct CommandInfo cp;
	FilePtr Files;
	char line[4096];
	int ret;
	long n;

	if(!Batch || !name)return 1;
    Files=FileManagerGet(name);    
    if(!Files)return 1;
	
	ret=1;
	
	Images=CreateImageList(Files->ImageCount);
	if(!Images)goto ErrorOut;
	
	for(n=0;n<Images->count;++n)Images->imageList[n]=n;
	
	Images->type=IMAGE_SDS2D_MERGE;
	Images->Files=Files;
	Images->UseImageNames=TRUE;
	
	zerol((char *)&cp,sizeof(struct CommandInfo));

	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSavePlotsProcess(Images,&cp,Batch))goto Ok;
		}

	}
Ok:		
	ret=0;
ErrorOut:
 	getCommand(NULL,&cp);
	if(Images)ImageFreeList(Images);
	return ret;
	
}
int doSavePlots(IconPtr myIcon,struct FileInfo2 *Files,long CurrentFrame,uRect *DRect)
{
	static char hdf[256]=	{'I','m','a','g','e','.','h','d','f',0};
	static char pict[256]=	{'I','m','a','g','e','.','p','c','t',0};
	static char jpeg[256]=	{'I','m','a','g','e','.','j','p','g',0};
	static char true2[256]=	{'I','m','a','g','e','.','t','r','u',0};
	static char quick[256]=	{'I','m','a','g','e','.','m','o','v',0};
	static char sds2d[256]=	{'I','m','a','g','e','.','s','2','d',0};
	static char hdf256[256]={'I','m','a','g','e','.','h','d','f',0};
	static char bmp[256]=   {'I','m','a','g','e','.','b','m','p',0};
	static char avi[256]=   {'I','m','a','g','e','.','a','v','i',0};
	static char tek[256]=   {'I','m','a','g','e','.','t','e','k',0};
	static char rgb[256]=   {'I','m','a','g','e','.','r','g','b',0};
	static char *PlotTypes[]={hdf,pict,pict,jpeg,true2,sds2d,quick,quick,hdf256,bmp,avi,tek,rgb};
	static int NumberOfPlotTypes=sizeof(PlotTypes)/sizeof(char *);
	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"New", {404,74,115,30},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {404,268,115,30},uDialogButton,uDialogNone},
      /*  2 */      {"Scroll", {25,35,349,233},uDialogScrollList,uDialogNone},
      /*  3 */      {"File Menu", {143,6,180,25},uDialogPopUpMenuControl,uDialogNone},
      /*  4 */      {"Save All", {404,174,115,30},uDialogButton,uDialogNone},
      /*  5 */      {"Save As :", {127,281,169,19},uDialogString,uDialogNone},
      /*  6 */      {"Data Replacement String", {21,309,358,26},uDialogText,uDialogText},
      /*  7 */      {"Save One", {404,123,115,30},uDialogButton,uDialogNone},
      /*  8 */      {"Merge All", {404,221,115,30},uDialogButton,uDialogNone},
      /*  9 */      {"Type Menu", {146,355,180,25},uDialogPopUpMenuControl,uDialogGetLong},
      /*  10 */      {"Up", {404,20,50,30},uDialogButton,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {117,45,575,434};
 	
 	uRect *RectPtr;
 	
	struct SaveInformation *SaveInfo;
	struct FileListInfo *info;		
 	uDialogList *uList;
	uDialogReturn *uR;
    int ret;
 	
	if(!myIcon || !Files || !DRect)return 1;
	
	
	SaveInfo=&myIcon->SaveInfo;
	SaveInfo->Files=Files;
	SaveInfo->CurrentFrame=CurrentFrame;
	SaveInfo->DRect=DRect;
	SaveInfo->myIcon=myIcon;
	info=&myIcon->infoSave;
	
	if(SaveInfo->PlotTypeOut < 0 || SaveInfo->PlotTypeOut >= NumberOfPlotTypes)SaveInfo->PlotTypeOut=0;
		
	ret=1;
	
	uR=NULL;
	
	RectPtr=&Rect1;
	RectPtr=&uPref.SaveImageRect;

	uList=uDialogOpen("Save Images",RectPtr,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;

	if(uDialogSet(uList,0L,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetKeys,doScale2Keys,
	              uDialogSetMove,doScale2Move,
	              uDialogSetDoubleClick,doScale2Double,
	              uDialogSetMyIcon,myIcon,
	              
             	  uDialogSetItem,6L,
	              uDialogSetText,PlotTypes[SaveInfo->PlotTypeOut],
/*
	              uDialogSetButtons,doScale2SetButtons,
	              
*/
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
	

	if(uDialog(uList))goto ErrorOut;

	if(info->directory){
		cFree(info->directory);
	}
	
	info->directory=DefaultPathString();	

	if(uList && uList->myIcon){
        uRect portRect;
		uGetPortBoundsGlobal(uList->myIcon,&portRect);
		if(info){
			info->initialized=TRUE;
			info->WindowRect=portRect;
			FileDialogRecordInfo(info);
		}
		uPref.SaveImageRect=portRect;
	}

	uR=uDialogGetReturn(uList);

    if(!uR){
	    WarningBatch("doSavePlots Error Getting Search String Specified\n");
	    goto ErrorOut;
    }
    
    SaveInfo->PlotTypeOut=(int)(uR[9].lreturn-1);
    
	if(SaveInfo->PlotTypeOut < 0 || SaveInfo->PlotTypeOut >= NumberOfPlotTypes)SaveInfo->PlotTypeOut=0;
	
	mstrncpy(PlotTypes[SaveInfo->PlotTypeOut],uR[6].sreturn,256);
    	
	mstrncpy(SaveInfo->PlotName,uR[6].sreturn,256);
	    	
	ret=0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    
    if(uR)uDialogFreeReturn(uR);
    
    if(!ret){
        doWritePlots(SaveInfo);
    }

	return ret;
	
} 
static int doScale2Double(struct uDialogStruct *uList)
{
	int actionItem;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 2){
	   if(SelectDirectory(uList))goto OutOfHere;
	}
OutOfHere:
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int BailButton(struct uDialogStruct *uList)
{
	struct SaveInformation *SaveInfo;
    IconPtr myIcon;
	int actionItem;
	int ret;
	
	if(!uList)return 1;
     myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	
	SaveInfo=&myIcon->SaveInfo;
	actionItem=(int)uList->actionItem;
	if(actionItem == 7){
	    SaveInfo->PlotModeOut=0;
	}else if(actionItem == 4){
	    SaveInfo->PlotModeOut=1;
	}else if(actionItem == 8){
	    SaveInfo->PlotModeOut=2;
	}else {
	    return 1;
	}
	
	ret=uDialogSet(uList,0L,
              uDialogSetExitOk,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	
		
	return 0;
}
static int doScale2Action(struct uDialogStruct *uList)
{
	int actionItem;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 0){
		CreateNewDirectory();
		FillList(uList);
	}else if(actionItem == 4 || actionItem == 7 || actionItem == 8){
	    BailButton(uList);
	}else if(actionItem == 3){
	   NewDirectory(uList);
	}else if(actionItem == 10){
	   goCD("..");
	   if(FillList(uList))goto OutOfHere;
	}
OutOfHere:
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	struct SaveInformation *SaveInfo;
	struct FileListInfo *info;
	controlPtr controlType;
	menuPtr menuFile;
	menuPtr menuType;	
    IconPtr myIcon;
	int entry;
	int ret;
	int n;

	if(!uList)return 1;
     myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	SaveInfo=&myIcon->SaveInfo;
	info=&myIcon->infoSave;
	

	if(!IsOption()){
		if(info->directory){
			goCD(info->directory);
		}
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	if(FillList(uList))return 1;

	ret=uDialogSet(uList,3L,
              uDialogGetControlMenu,&menuFile,
                            	              	              
	          uDialogSetItem,9L,
              uDialogGetControlMenu,&menuType,
              uDialogGetControl,&controlType,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;

	entry=(int)uCountMenuItems(menuType);
	for(n=0;n<entry;++n){
	    uDeleteMenuItem(menuType,1);
	}
	
 	uInsertMenuItem(menuType,"HDF",500);
 	uInsertMenuItem(menuType,"PICT",500);
 	uInsertMenuItem(menuType,"PICT32",500);
 	uInsertMenuItem(menuType,"JPEG",500);
 	uInsertMenuItem(menuType,"TRUE",500);
 	uInsertMenuItem(menuType,"SDS2D",500);
 	uInsertMenuItem(menuType,"QuickTime 8 Bit",500);
 	uInsertMenuItem(menuType,"QuickTime 32 Bit",500);
 	uInsertMenuItem(menuType,"To 256 Colors",500);
 	uInsertMenuItem(menuType,"BMP",500);
 	uInsertMenuItem(menuType,"AVI",500);
 	uInsertMenuItem(menuType,"TEK",500);
 	uInsertMenuItem(menuType,"SGI RGB",500);
	uSetControlMaximum(controlType,13);
	uSetControlValue(controlType,SaveInfo->PlotTypeOut+1);
	
	return 0;
}
static int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	uRect rList1,rList2;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetItem,2L,
              uDialogGetRect,&rList1,
              uDialogGetScrollList,&list2,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	
	rList1.x = 15;
	rList1.y = 35;
	rList1.xsize = (r.xsize-rList1.x-140);
	rList1.ysize = r.ysize-85-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	
	rList2=rList1;
	
	rList2.y = rList1.y+rList1.ysize+20;
	rList2.ysize = 25;
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,50,	
              
              uDialogSetItem,7L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,100,	
                            
              uDialogSetItem,4L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,150,	
                            
              uDialogSetItem,8L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,200,	
                                                        
              uDialogSetItem,1L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,250,	
              
              uDialogSetItem,10L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,5,	
              
              uDialogSetItem,3L,
              uDialogSetLocation,rList1.x+(rList1.xsize)/2-80,6,	
              
              uDialogSetItem,5L,
              uDialogSetLocation,rList1.x,rList1.y+rList1.ysize,	
              
              uDialogSetItem,6L,
              uDialogSetLocation,rList1.x,rList1.y+rList1.ysize+20,	
              uDialogSetRect,&rList2,
              
              uDialogSetItem,9L,
              uDialogSetLocation,rList1.x+(rList1.xsize)/2-80,rList1.y+rList1.ysize+50,	
              
              uDialogSetItem,2L,
              uDialogSetRect,&rList1,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );
    
    if(ret)goto ErrorOut;
	
	uDialogUpdate(uList);
ErrorOut:	
	return 0;
}
static int doScale2Keys(struct uDialogStruct *uList)
{

	struct ScrollList *List1;
	int ret;
	
	
	if(!uList)return 1;
	
	ret=uDialogSet(uList,2L,
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
	int ret;
	
	if(!uList)return 1;
		
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List,                            	              	                            
              uDialogSetDone,uDialogSetDone
    );
	if(ret)return 1;
	
	location=ScrollListFirstSelect(List);
	if(location < 0)return 1;
	
	if(ScrollListGet(buff,location,List))return 1;
	
	goCD(buff);
	
	FillList(uList);
	
	return 0;
}
static int NewDirectory(struct uDialogStruct *uList)
{
	controlPtr controlFile;
	menuPtr menuFile;
	int location;
	int ret;
	int n;
	
	if(!uList)return 1;
	
	ret=uDialogSet(uList,3L,
              uDialogGetControlMenu,&menuFile,                            	              	              
              uDialogGetControl,&controlFile,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	location=(int)uGetControlValue(controlFile);
	
	for(n=1;n<location;++n){
	   goCD("..");
	}
	
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
	int ret;
	int n;
	
	if(!uList)return 1;

	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List,
                            	              	              
	          uDialogSetItem,3L,
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
	
    filelist=scan_dir(".",".*");
    if(filelist == NULL){
        sprintf(WarningBuff,"scan_dir Error in FillList");
        Warning(WarningBuff);
		goto OutOfHere;
    }
    
    count=0;
    
    if(filelist->countVol > 0){
		if(ScrollListInsertOneType(filelist->namesVolumes,4L,count,(long)(filelist->countVol),List)){
		   sprintf(WarningBuff,"ScrollListInsertError in FillList");
		   Warning(WarningBuff);
		   goto OutOfHere;
		}
    	count += filelist->countVol;
    }
    
    if(filelist->countDirs > 0){
		if(ScrollListInsertOneType(filelist->namesDirs,1L,count,(long)(filelist->countDirs),List)){
		   sprintf(WarningBuff,"ScrollListInsertError in FillList");
		   Warning(WarningBuff);
		   goto OutOfHere;
		}
    	count += filelist->countDirs;
    }
    
    if(filelist->countFiles > 0){
		if(ScrollListInsertOneType(filelist->namesFiles,0L,count,(long)(filelist->countFiles),List)){
		   sprintf(WarningBuff,"ScrollListInsertError in FillList");
		   Warning(WarningBuff);
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
    
OutOfHere:    

	uDialogUpdate(uList);
	if(filelist)free_dir(filelist);
	return 0;
}
