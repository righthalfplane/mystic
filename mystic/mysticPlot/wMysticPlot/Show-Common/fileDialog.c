#define EXTERN22 extern
#include "Xappl.h"
#include "fileDialog.h"
#include "uDialog.h"
#include "Scanfile.h"
#include "uFiles.h"
#include "Universal.h"

char *DefaultPathString(void);
int CreateNewDirectory(void);
int goCD(char *name);


extern int CatalogHiddenShow;

static int TypeControlValue=1;
static long FileIncrement=1;
static long ImageIncrement=1;
static int Memory=1;
static int OkButton;
int IsOption(void);
int SetFileDialogType(struct FileList *Files,int TypeControlValue);

int doSingles(struct FileList *Files);

static struct FileList *FilesGet(struct uDialogStruct *uList,int Operation);
static int doScale2Double(struct uDialogStruct *uList);
static int SelectDirectory(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int FillList(struct uDialogStruct *uList);
static int NewDirectory(struct uDialogStruct *uList);
static int doScale2Keys(struct uDialogStruct *uList);
static int doScale2Move(struct uDialogStruct *uList);
static int BailButton(struct uDialogStruct *uList);
static int SetButtons(struct uDialogStruct *uList);

int GetTheSaveFileName(char *name,char *message,int length,struct FileListInfo *info)
{
	char *pathnameSave,*fp;
	int n,ret;
	
	if(!name || !message || (length <= 0))return 1;
	
	ret=1;
	
    fp=strrchr((char *)name,FILE_NAME_SEPERATOR);
    if(fp){
        fp += 1;
    }else{
        fp=name;
    }
	
	pathnameSave=FileSaveFilePath(fp,message,info);
	if(!pathnameSave)return 1;
	
	n=(int)strlen(pathnameSave)+1;
	
	if(n < length){
	    mstrncpy(name,pathnameSave,(long)length);
	    ret=0;
	}
	
	if(pathnameSave)cFree(pathnameSave);
	
	return ret;
	
}
int GetTheFileName(char *name,char *message,int length,struct FileListInfo *info)
{
	char *pathnameSave;
	int n,ret;
	
	if(!name || !message || (length <= 0))return 1;
	
	ret=1;
	
	pathnameSave=FileGetFilePath(message,info);
	if(!pathnameSave)return 1;
	
	n=(int)strlen(pathnameSave)+1;
	
	if(n < length){
	    mstrncpy(name,pathnameSave,(long)length);
	    ret=0;
	}
	
	if(pathnameSave)cFree(pathnameSave);
	
	return ret;

}
char *FileSaveFilePath(char *name,char *message,struct FileListInfo *info)
{
	struct FileList *Files;
	char path[4000];
	char *pathnameSave;

    if(!message)return NULL;
    
    pathnameSave=NULL;
    
    Files=FileDialog(name,message,FileDialog_SaveOne,info);		
	if(!Files)return NULL;
	
	mstrncpy(path,Files->directory,(long)4000);

	if(Files->count){
	    strncatToPath(path,Files->name[0],(long)4000);
	    pathnameSave=strsave(path,197463);
	}
	
	FileListFree(Files);

    return pathnameSave;
}


char *FileGetFilePath(char *message,struct FileListInfo *info)
{
	struct FileList *Files;
	char path[4000];
	char *pathnameSave;

    if(!message)return NULL;
    
    pathnameSave=NULL;
    
    Files=FileDialog(NULL,message,FileDialog_GetFiles,info);		
	if(!Files)return NULL;
	
	mstrncpy(path,Files->directory,(long)4000);

	if(Files->count){
	    strncatToPath(path,Files->name[0],(long)4000);
	    pathnameSave=strsave(path,197464);
	}
	
	FileListFree(Files);

    return pathnameSave;
}
char *FileGetPath(char *message,struct FileListInfo *info)
{
	struct FileList *Files;
	char *pathnameSave;

    if(!message)return NULL;
    Files=FileDialog(NULL,message,FileDialog_SetDirectory,info);		
	if(!Files)return NULL;
	pathnameSave=strsave(Files->directory,197465);
	
	FileListFree(Files);

    return pathnameSave;
}
struct FileList *FileDialog(char *name,char *message,int Operation,struct FileListInfo *info)
{
	struct FileList *Files;
 	uPort Port;
 	int ret;
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {404,174,115,30},uDialogButton,uDialogNone},
      /*  1 */      {"All", {404,74,115,30},uDialogButton,uDialogNone},
      /*  2 */      {"Cancel", {404,268,115,30},uDialogButton,uDialogNone},
      /*  3 */      {"Up", {404,20,50,30},uDialogButton,uDialogNone},
      /*  4 */      {"Scroll", {25,35,349,233},uDialogScrollList,uDialogNone},
      /*  5 */      {"File Menu", {143,6,180,25},uDialogPopUpMenuControl,uDialogNone},
      /*  6 */      {"Type Menu", {146,355,180,25},uDialogPopUpMenuControl,uDialogGetLong},
      /*  7 */      {"Save As :", {127,281,169,19},uDialogString,uDialogNone},
      /*  8 */      {"Data Replacement String", {21,309,358,26},uDialogText,uDialogText},
      /*  9 */      {"File Inc", {10,35,70,20},uDialogString,uDialogNone},
      /* 10 */      {"Data Replacement String", {10,55,70,20},uDialogText,uDialogGetLong},
      /* 11 */      {"Image Inc", {10,85,70,20},uDialogString,uDialogNone},
      /* 12 */      {"Data Replacement String", {10,105,70,20},uDialogText,uDialogGetLong},
      /* 13 */      {"New", {404,74,115,30},uDialogButton,uDialogNone},
      /* 14 */      {"Exit", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
      /* 15 */      {"Hidden", {5,160,70,30},uDialogCheck,uDialogGetLong},
      /* 16 */      {"Singles", {5,190,70,30},uDialogCheck,uDialogGetLong},
      /* 17 */      {"Memory", {5,220,70,30},uDialogCheck,uDialogGetLong},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {117,45,575,434};
 	
 	uRect  *RectDefault;
 	
 	int Singles;
  	
 	uDialogList *uList;

	uDialogReturn *uR;
 	
	if(!message)return NULL;
	
	uR=NULL;

	Files=NULL;
	
	info=info;
	
	Singles=0;
	
	ret=1;
	uGetPort(&Port);
	
	RectDefault = &Rect1;
	if(Operation == FileDialog_GetFiles){
	    RectDefault=&uPref.LoadFileRect;
	}else{
	    RectDefault=&uPref.SaveFileRect;
	}
	
	uList=uDialogOpen(message,RectDefault,ItemCount);
	
	if(!uList)goto ErrorOut;
    
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetItem,1L,
	              uDialogSetHidden,(Operation != FileDialog_GetFiles),
	              uDialogSetItem,6L,
	              uDialogSetHidden,(Operation != FileDialog_GetFiles),
	              uDialogSetItem,7L,
	              uDialogSetHidden,(Operation != FileDialog_SaveOne),
	              uDialogSetItem,8L,
	              uDialogSetHidden,(Operation != FileDialog_SaveOne),
	              uDialogSetActive,(Operation == FileDialog_SaveOne),
	              uDialogSetItem,9L,
	              uDialogSetHidden,(Operation != FileDialog_GetFiles),
	              uDialogSetItem,10L,
	              uDialogSetHidden,(Operation != FileDialog_GetFiles),
	              uDialogSetActive,(Operation == FileDialog_GetFiles),
	              uDialogSetItem,11L,
	              uDialogSetHidden,(Operation != FileDialog_GetFiles),
	              uDialogSetItem,12L,
	              uDialogSetHidden,(Operation != FileDialog_GetFiles),
	              uDialogSetActive,(Operation == FileDialog_GetFiles),
	              uDialogSetItem,13L,
	              uDialogSetHidden,(Operation == FileDialog_GetFiles),
	              uDialogSetItem,15L,
	              uDialogSetLogical,CatalogHiddenShow,
	              uDialogSetItem,16L,
	              uDialogSetLogical,Singles,
	              uDialogSetItem,17L,
	              uDialogSetLogical,Memory,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetKeys,doScale2Keys,
	              uDialogSetMove,doScale2Move,
	              uDialogSetDoubleClick,doScale2Double,
	              
	              uDialogSetMyIcon,info,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
    
  
	
	OkButton=FALSE;   
	
	if(Operation == FileDialog_GetFiles){
		if(uDialogSet(uList,0L,
		              uDialogSetItem,0L,
		              uDialogSetName,"Ok",
		              uDialogSetItem,10L,
		              uDialogSetDouble,(double)FileIncrement,
		              uDialogSetItem,12L,
		              uDialogSetDouble,(double)ImageIncrement,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	    OkButton=TRUE;   
	}else if(Operation == FileDialog_SaveOne){		
	    if(!name || !name[0]){
	        name="file.out";
	    }
	    
		if(uDialogSet(uList,0L,
		              uDialogSetItem,0L,
		              uDialogSetName,"Save",
		              uDialogSetItem,8L,
		              uDialogSetText,name,
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
		
	}else{
		if(uDialogSet(uList,0L,
		              uDialogSetItem,0L,
		              uDialogSetName,"Save Here",
		              uDialogSetDone,uDialogSetDone
		))goto ErrorOut;
	}
	
			
	if(uDialog(uList))goto ErrorOut;    
	
	uR=uDialogGetReturn(uList);
    if(!uR){
	    WarningBatch("Error Getting Search String Specified\n");
	    goto ErrorOut;
    }

	Singles=(int)uR[16].lreturn;
	
	Memory=(int)uR[17].lreturn;
	
	Files=FilesGet(uList,Operation);

	ret=0;
ErrorOut:

	if(ret){
	    if(Files){
			FileListFree(Files);
	        Files=NULL;
	    }
	}else{
        uRect portRect;
	    if(Files){
	    	if(info){
	        	if(info->directory)cFree((char *)info->directory);
	        	info->directory=strsave(Files->directory,88092);
	        }
	    }

		if(uList && uList->myIcon){
			uGetPortBoundsGlobal(uList->myIcon,&portRect);
			if(info){
				info->initialized=TRUE;
				info->WindowRect=portRect;
				FileDialogRecordInfo(info);
			}
			if(Operation == FileDialog_GetFiles){
			    uPref.LoadFileRect=portRect;
			}else{
			    uPref.SaveFileRect=portRect;
			}
		}
	    
	}
	
    if(uList)uDialogClose(uList);
    
    if(uR)uDialogFreeReturn(uR);
    
	uSetPort(&Port);
	
	if(Singles){
	    doSingles(Files);
	    Files=NULL;
	}

	return Files;

}
int doSingles(struct FileList *FilesIn)
{
    struct FileInfo2 *Files;
	struct FileList File;
	long n;
	
	if(!FilesIn)return 1;
	
	zerol((char *)&File,sizeof(File));
	
	for(n=0;n<FilesIn->count;++n){
	    zerol((char *)&File,sizeof(File));
	    File.donotfree=TRUE;
	    File.count=1;
	    File.directory=strsave(FilesIn->directory,986);
	    if(!File.directory)break;
	    File.name=cMalloc(sizeof(char **),983);
	    if(!File.name)break;
	    File.name[0]=strsave(FilesIn->name[n],986);
	    if(!File.name[0])break;	    
	    File.type=FilesIn->type;	    
		Files=FilesOpenList(&File);
	    if(Files){
	        if(FilesOpenDefault(Files)){
	            break;
	        }
	    }
	    	    
	}	
	
	FileListFree(FilesIn);

	return 0;
}
struct FileList *FilesGet(struct uDialogStruct *uList,int Operation)
{
   	struct FileList *Files;
	struct ScrollList *List1;
   	char pathname[256];
	long count,n,cf,lcount;
	int done;
	uDialogReturn *uR;
	int reti;
	int ret;
	
	
	if(!uList)return NULL;
	
	ret=1;
	
	Files=NULL;
	
	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    goto ErrorOut;
    }
    
    
	reti=uDialogSet(uList,0L,
              uDialogSetItem,4L,
              uDialogGetScrollList,&List1,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );
    
    if(reti)goto ErrorOut;

	done=(int)uR[14].dreturn;
	
	
	lcount=ScrollListCount(List1);
			
	count = 0;
	if(done == 0){
		for(n=0;n<lcount;++n){
	 	    if(ScrollListIsSelected(n,List1)){
	 	        ++count;
		    }
		}
	}else if(done == 1){
	   count=lcount;
		for(n=0;n<lcount;++n){
		    ScrollListSelect(n,List1);
		}
	}else{
	   count = 0;
	}
	
	if(Operation == FileDialog_GetFiles){
	    if(count <= 0 )goto ErrorOut;
	}
	
	Files=(struct FileList *)cMalloc(sizeof(struct FileList),8105);
	if(!Files)goto ErrorOut;
	zerol((char *)Files,sizeof(struct FileList));
		
 	Files->directory=DefaultPathString();
 	if(!Files->directory)goto ErrorOut;
		
	if(Operation == FileDialog_SetDirectory){
	    goto OK;
	}else if(Operation == FileDialog_SaveOne){
	    Files->name=(char **)cMalloc(sizeof(char *),8106);
	    if(!Files->name)goto ErrorOut;
	    Files->name[0]=strsave(uR[8].sreturn,903934);
	    if(!Files->name[0])goto ErrorOut;
	    Files->count=1;
	    goto OK;
	}
	
	Files->name=(char **)cMalloc(count*sizeof(char *),8106);
	if(!Files->name){
	    goto ErrorOut;
	}
	zerol((char *)Files->name,sizeof(count*sizeof(char *)));
	Files->count=count;
	
	
	FileIncrement=uR[10].lreturn;
	if(FileIncrement <= 0)FileIncrement = 1;
		
	ImageIncrement=uR[12].lreturn;
	if(ImageIncrement <= 0)ImageIncrement = 1;

	Files->ImageIncrement=ImageIncrement;

	cf=0;
	for(n=0;n<lcount;n += FileIncrement){
 	    if(ScrollListIsSelected(n,List1)){
 	        if(ScrollListGet(pathname,n,List1))goto ErrorOut;
 	        Files->name[cf]=strsave(pathname,8227);
 	        if(!Files->name[cf++])goto ErrorOut;
	    }
	}
	
	Files->count=cf;
	
	Files->loadToMemory=Memory;
	 	
 	TypeControlValue=(int)uR[6].lreturn;
 	
 	
	SetFileDialogType(Files,TypeControlValue);
	
OK:	
	ret=0;
	
ErrorOut:

    if(uR)uDialogFreeReturn(uR);
    
    if(ret){
	    FileListFree(Files);
	    Files=NULL;
	}
	
	return Files;
}
int SetFileDialogType(struct FileList *Files,int TypeControlValue)
{
	if(!Files)return 1;
	
 	if(TypeControlValue < 1 || TypeControlValue > 15){
 		Files->type=HDF_MEMORY;
 	}else if(TypeControlValue < 12){
 	    static int type1[]={HDF_DISK,PICT_DISK,TRUE_DISK,TEK_DISK,RGB_DISK,
 	    					SDS2D_DISK,SDS3D_DISK,BMP_DISK,GIF_DISK,JPG_DISK,QUICKTIME_DISK,
 	    					};
 		Files->type=type1[TypeControlValue-1]+Memory;
 	}else{
 	    static int type2[]={TEXT_MEMORY,SAGE_DISK,SAGE_DUMP,STL_DISK,};
 		Files->type=type2[TypeControlValue-12];
 	}
 	
	
	return 0;
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
static int BailButton(struct uDialogStruct *uList)
{
	int actionItem;
	int ret;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	if(actionItem == 0){
	    ;
	}else if(actionItem == 1){
	    ;
	}else if(actionItem == 2){
	    ;
	}else {
	    return 1;
	}
	
	ret=uDialogSet(uList,0L,
             uDialogSetExitOk,
	         uDialogSetItem,14L,
 	         uDialogSetFloatAux,(double)(actionItem),
             uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	
		
	return 0;
}
static int SetButtons(struct uDialogStruct *uList)
{
	struct ScrollList *list4;
	controlPtr control;
	int ret;
	
	if(!uList)return 1;

	list4=NULL;
	control=NULL;
	
	ret=uDialogSet(uList,0L,                            	              	              
              uDialogSetItem,4L,
              uDialogGetScrollList,&list4,
              uDialogSetItem,0L,
              uDialogGetControl,&control,
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto OutOfHere;
		
	if(!OkButton || (list4 && ScrollListIsSelectAny(list4))){
	    if(control)uHiliteControl(control,0);
	}else{
	    if(control)uHiliteControl(control,255);
	}
OutOfHere:
	return 0;
}
static int doScale2Action(struct uDialogStruct *uList)
{
	struct ScrollList *List1;
	char name[256];
	int actionItem;
	long lcount;
	int reti;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 13){
		CreateNewDirectory();
		FillList(uList);
	}else if(actionItem == 0 || actionItem == 1 || actionItem == 2){
	    BailButton(uList);
	}else if(actionItem == 4){
		reti=uDialogSet(uList,0L,
	              uDialogSetItem,4L,
	              uDialogGetScrollList,&List1,
	                            	              	              
	              uDialogSetDone,uDialogSetDone
	    );
	    
	    if(reti)goto OutOfHere;
	    
		lcount = ScrollListFirstSelect(List1);
		if(lcount < 0){
		   goto OutOfHere;
		}
	
		if(ScrollListGet(name,lcount,List1)){
		   	goto OutOfHere;
		}
		if(uDialogSet(uList,0L,
		              uDialogSetItem,8L,
		              uDialogSetText,name,
		              uDialogSetDone,uDialogSetDone
		))goto OutOfHere;
		
		uDialogUpdate(uList);
		
	}else if(actionItem == 5){
	   NewDirectory(uList);
	}else if(actionItem == 15){
	    CatalogHiddenShow = !CatalogHiddenShow;
		FillList(uList);
	}else if(actionItem == 3){
	   goCD("..");
	   if(FillList(uList))goto OutOfHere;
	}
OutOfHere:
	if(uList){
		SetButtons(uList);
	   /*  uDialogUpdate(uList); */
	    uSetCursor(uList->myIcon,-uARROW_CURSOR);
	}
	return 0;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	struct FileListInfo *info;
	controlPtr controlType;
	menuPtr menuFile;
	menuPtr menuType;	
	int entry;
	int ret;
	int n;

	if(!uList)return 1;

	if(!IsOption()){
		info=(struct FileListInfo *)uList->myIconItem;
		if(info){
			if(info->directory){
			    goCD(info->directory);
			}
	    }
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	if(uList)SetButtons(uList);
	
	if(FillList(uList))return 1;

	ret=uDialogSet(uList,5L,
              uDialogGetControlMenu,&menuFile,
                            	              	              
	          uDialogSetItem,6L,
              uDialogGetControlMenu,&menuType,
              uDialogGetControl,&controlType,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;

	entry=(int)uCountMenuItems(menuType);
	for(n=0;n<entry;++n){
	    uDeleteMenuItem(menuType,1);
	}
	
	{
		char *names[] = {
							"HDF",
							"PICT",
							"TRUE",
							"TEK",
							"RGB",
							"SDS2D",
							"SDS3D",
							"BMP",
							"GIF",
							"JPG",
							"QuickTime",
							"TEXT",
							"Display Pio",
							"Dump Pio",
							"STL",
						};
		int count = sizeof(names)/sizeof(char *);
		
		for(n=0;n<count;++n){
 			uInsertMenuItem(menuType,names[n],500);
		}
		uSetControlMaximum(controlType,count);
		uSetControlValue(controlType,TypeControlValue);		
		
	}
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
              uDialogSetItem,4L,
              uDialogGetRect,&rList1,
              uDialogGetScrollList,&list2,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto ErrorOut;
	
	rList1.x = 85;
	rList1.y = 35;
	rList1.xsize = (r.xsize-rList1.x-80);
	rList1.ysize = r.ysize-85-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	
	rList2=rList1;
	
	rList2.y = rList1.y+rList1.ysize+20;
	rList2.ysize = 25;
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,10,r.ysize-35,	
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize/2-60,r.ysize-35,	
              uDialogSetItem,13L,
              uDialogSetLocation,r.xsize/2-60,r.ysize-35,	
                            
              uDialogSetItem,2L,
              uDialogSetLocation,r.xsize-135,r.ysize-35,	
                            
              uDialogSetItem,3L,
              uDialogSetLocation,rList1.x+rList1.xsize+10,5,
              	
              uDialogSetItem,4L,
              uDialogSetRect,&rList1,
                                                                                                                                                                        
              uDialogSetItem,5L,
              uDialogSetLocation,rList1.x+(rList1.xsize)/2-80,6,	
              
              uDialogSetItem,6L,
              uDialogSetLocation,rList1.x+(rList1.xsize)/2-80,rList1.y+rList1.ysize+10,	
                            
              uDialogSetItem,7L,
              uDialogSetLocation,rList1.x,rList1.y+rList1.ysize,	
              
              uDialogSetItem,8L,
              uDialogSetRect,&rList2,
                            	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	/* uDialogUpdate(uList); */
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
	
	if(uList)SetButtons(uList);
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
		
	
	ret=uDialogSet(uList,4L,
              uDialogGetScrollList,&List,                            	              	                            
              uDialogSetDone,uDialogSetDone
    );
	if(ret)return 1;
	
	location=ScrollListFirstSelect(List);
	if(location < 0)return 1;
	
	if(ScrollListGet(buff,location,List))return 1;
	
	if(goCD(buff)){
		ret=uDialogSet(uList,0L,
	             uDialogSetExitOk,
		         uDialogSetItem,14L,
	 	         uDialogSetFloatAux,(double)(0),
	             uDialogSetDone,uDialogSetDone
	    );
		return ret;
	}
	
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

	ScrollListUpdate(List);
	if(filelist)free_dir(filelist);
	return 0;
}
