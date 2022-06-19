#define EXTERN22 extern
#include "Xappl.h"
#include "uDialog.h"
#include "Scanfile.h"


int CreateNewDirectory(void);

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


int doSetImageInfo(char *Name,char *Directory,int flag);

int doSetImageInfo(char *Name,char *Directory,int flag)
{

	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"New", {404,74,115,30},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {404,268,115,30},uDialogButton,uDialogNone},
      /*  2 */      {"Scroll", {25,35,349,233},uDialogScrollList,uDialogNone},
      /*  3 */      {"File Menu", {143,6,180,25},uDialogPopUpMenuControl,uDialogNone},
      /*  4 */      {"Save All", {404,174,125,30},uDialogButton,uDialogNone},
      /*  5 */      {"Save As :", {127,281,169,19},uDialogString,uDialogNone},
      /*  6 */      {"Data Replacement String", {21,309,358,26},uDialogText,uDialogText},
      /*  7 */      {"Save One", {404,123,125,30},uDialogButton,uDialogNone},
      /*  8 */      {"Merge All", {404,221,125,30},uDialogButton,uDialogNone},
      /*  9 */      {"Type Menu", {146,355,180,25},uDialogPopUpMenuControl,uDialogGetLong},
      /*  10 */      {"Up", {404,20,50,30},uDialogButton,uDialogNone},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {117,45,575,434};
 	
	struct SaveInformation *SaveInfo;
	
	char *direct;
	
 	struct Icon myIcon;
 	uDialogList *uList;
	uDialogReturn *uR;
	int RetFlag;
    int ret;
    
    
    if(!Name || !Directory)return 1;
    
    RetFlag=1;
       
    zerol((char *)&myIcon,sizeof(struct Icon));
	    
	ret=1;
	
	uR=NULL;

	uList=uDialogOpen("Save Images",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;

	if(uDialogSet(uList,0L,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetKeys,doScale2Keys,
	              uDialogSetMove,doScale2Move,
	              uDialogSetDoubleClick,doScale2Double,
	              uDialogSetMyIcon,&myIcon,

             	  uDialogSetItem,6L,
	              uDialogSetText,Name,

	              uDialogSetItem,9L,
	              uDialogSetHidden,(1),
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
	
	
	
	if(flag == 1){
	
		if(uDialogSet(uList,0L,
             	  uDialogSetItem,4L,
	              uDialogSetText,"Save To Files",
             	  uDialogSetItem,7L,
	              uDialogSetText,"Save To Memory",
             	  uDialogSetItem,8L,
	              uDialogSetText,"Merge To File",
		          uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
		
	
	}	
	
	

	if(uDialog(uList))goto ErrorOut;


	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    goto ErrorOut;
    }
    
	    
	SaveInfo=&myIcon.SaveInfo;
	
	RetFlag=SaveInfo->PlotModeOut+2;
	
	mstrncpy(Name,uR[6].sreturn,255);
	    
	direct=DefaultPathString();
	
	if(!direct)goto ErrorOut;
	    
	mstrncpy(Directory,direct,255);
	
	cFree((char *)direct);
	    
	ret = 0;	    

ErrorOut:

    if(uList)uDialogClose(uList);
    
    if(uR)uDialogFreeReturn(uR);

    if(ret)return 1;
	
	return RetFlag;
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
	uSetControlMaximum(controlType,11);
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
