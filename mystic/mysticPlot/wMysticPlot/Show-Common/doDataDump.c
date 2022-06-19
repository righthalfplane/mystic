#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "SaveAllImages.h"
#include "Linedata.h"
#include "Message.h"
#include "Universal.h"

static int doDumpGetPointData(struct FileInfo2 *Files,struct linedata *li);
static int doDumpGetLineData(struct FileInfo2 *Files,struct linedata *li);
static int doPlot2(struct uDialogStruct *uList);
static int doDumpGetCellData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd);
static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int doScale2SetButtons(struct uDialogStruct *uList);
static int doScale2Keys(struct uDialogStruct *uList);
static int GetRange2(struct uDialogStruct *uList);
static int DoClickInList2(struct uDialogStruct *uList);
static int DoClickInList8(struct uDialogStruct *uList);
int doDataDump(IconPtr myIcon,struct FileInfo2 *Files);

IconPtr CDrawNewLine(struct linedata *li,int count);

struct FileInfo2 *FilesReOpen(struct FileInfo2 *File);

static int Menu(IconPtr myIcon,int menu,int item);

struct uDumpFile{
    struct FileInfo2 *Files;
    IconPtr myIcon;
    long CurrentFrame;
    long dataLength;
    double *fdata;
    int flag;
	uRect windowRect;
	int defaultsSet;
};

typedef struct uDumpFile *dumpPtr;

static int fClose(IconPtr myIcon);

static int DumpGetMessage(IconPtr myIcon,long MessageType,void *MessageData);

#define FileCDraw 		    19903	
#define FileNew   				1
#define FileClose 				2		
#define savePreferencesCommand 	4
#define PreferencesCommand 		5
#define loadTekCommand 			6
#define FileQuit  				7

#define EditCDraw			19904
#define Information			1
#define CUT_ITEM			2
#define COPY_ITEM			3
#define PASTE_ITEM			4
#define CLEAR_ITEM			5

int FilesOpenPIODUMP(struct FileInfo2 *Files);

static int doLoadImages(IconPtr myIcon);

static int FilesOpenPIODUMP2(IconPtr myIconin,struct FileInfo2 *Files,int flag);

static struct RayList Rays;

struct RayList *GetRayList(struct FileInfo2 *Files)
{
	if(!Files)return NULL;
	
	zerol((char *)&Rays,sizeof(Rays));
	
	if(FilesOpenPIODUMP2(NULL,Files,1))return NULL;
	
	return &Rays;
	
}

int doDataDump(IconPtr myIcon,struct FileInfo2 *Files)
{
	if(!Files)return 1;
	
	Files=FilesReOpen(Files);
	
	if(!Files)return 1;
	
	return 	FilesOpenPIODUMP2(myIcon,Files,0);
}


int FilesOpenPIODUMP(struct FileInfo2 *Files)
{
	return 	FilesOpenPIODUMP2(NULL,Files,0);
}


static int FilesOpenPIODUMP2(IconPtr myIconin,struct FileInfo2 *Files,int flag)
{
	static struct uMenuItems File[]={
	    {"File",  					FileCDraw,uWindowStandardMenu,0,},
	    {"New                 /N",	FileNew,0,0,},
	    {"Close               /W", 	FileClose,0,0,},
	    {"----------------------",	3,0,0,},
	    {"Save Preferences",		savePreferencesCommand,0,0,},
	    {"Preferences...  ",		PreferencesCommand,0,0,},
	    {"----------------------",	3,0,0,},
	    {"Load Images...  ",		loadTekCommand,0,0,},
	    {"----------------", 		3,0,0,},
	    {"Quit                /Q",	FileQuit,0,0,},
	    {NULL,0,0,0,},
	};


	static struct uMenuItems Edit[]={
	    {"Edit",  			EditCDraw,uWindowStandardMenu,0,},
	    {"Information...  /I",Information,0,0,},
	    {NULL,0,0,0,},
	};

	static struct uMenuList menuList[]={
	    {File},
	    {Edit},
	    /*
	    {Fonts},
	    {Styles},
	   	    */
	    {NULL}
	};
	
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Plot", {10,50+MenuOffSet,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Close", {80,50+MenuOffSet,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,40+MenuOffSet,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"Range", {315,29+MenuOffSet,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"dmin", {5,2+MenuOffSet,65,20},uDialogString,uDialogNone},
      /*  5 */      {"dmax", {180,2+MenuOffSet,65,20},uDialogString,uDialogNone},
      /*  6 */      {"", {50,2+MenuOffSet,110,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"", {240,2+MenuOffSet,110,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"File List",{20,40+MenuOffSet,65,65},uDialogScrollList,uDialogNone},
      /*  9 */      {"File List",{20,40+MenuOffSet,65,65},uDialogScrollList,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,320+MenuOffSet};

	IconPtr myIcon; 	
	dumpPtr d;
	
 	uRect *rectOpen;
 	
 	uDialogList *uList;
 	char buff[256];
	static int Count;
 	uPort Port;
    int ret;
    
	if(!Files)return 1;
	
	uGetPort(&Port);

	ret=1;
	
	uList=NULL;
	
	myIcon=NULL;

	if((d=(struct uDumpFile *)cMalloc(sizeof(struct uDumpFile),8183)) == NULL){
		WarningBatch("doDataDump Out of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)d,sizeof(struct uDumpFile));
	
	d->Files=Files;
	
	d->myIcon=myIconin;
	
	d->flag=flag;
	
	myIcon=(IconPtr)d;
		
	if(uPref.dumpSelectRect.xsize > 100 && uPref.dumpSelectRect.ysize > 100){
		rectOpen=&uPref.dataSelectRect;
	}else{
		rectOpen=&Rect1;
	}
	

	sprintf(buff,"DataDump_%03d",Count++);
	
	uList=uDialogOpen("buff",rectOpen,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,0L,
	              uDialogSetMyIcon,myIcon,
	              uDialogSetMove,doScale2Move,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetButtons,doScale2SetButtons,
	              uDialogSetKeys,doScale2Keys,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,

	              uDialogSetModeLess,!flag,	   
	              
	              uDialogSetItemClose,fClose,

	              uDialogWindowType,DumpWindowType,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
	

	if(uDialog(uList))goto ErrorOut;
	
	myIcon=uList->myIcon;
	
	if(uSetWindow(myIcon,	       
			  uWindowSetMenuList,menuList,
              uWindowSetDone,uWindowSetDone)
	)goto ErrorOut;

	ret=0;
	
ErrorOut:
  
	if(ret){
    	if(uList)uDialogClose(uList);
    	myIcon=NULL;
	}else if(!flag){
		if(myIcon)myIcon->isOpen=TRUE;
    	if(uList)doScale2Move(uList);
	}else{
    	if(uList)uDialogClose(uList);
    	myIcon=NULL;
	}

	if(!myIcon)uSetPort(&Port);

	return ret;
}
static int CheckMenu(IconPtr myIcon,int flag)
{

	if(!myIcon)return 1;
		
	if(flag){
	    doWindowMenu();
	}
	return 0;
}
static int doLoadImages(IconPtr myIcon)
{
	struct FileInfo2 *Files2;
	struct FileList *Files;
	int TypeList[]={OPEN_ALL_FILE_TYPES,0};

	if(!myIcon)return 1;

	Files=uOpenFileDialog(myIcon,TypeList);		
	if(!Files)return 1;
		
		Files2=FilesOpenList(Files);
		if(!Files2)return 1;
	    return FilesOpenDefault(Files2);
}
static int Menu(IconPtr myIcon,int menu,int item)
{
	struct uDialogStruct *uList;

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
		} else if(item == loadTekCommand){
		    doLoadImages(myIcon);
		}else if(item == FileQuit){
			doQuit();
		    myIcon=NULL;
		    break;
		}
		break;
	case EditCDraw:
		if(item == Information){
 		    doWindowInformation(myIcon);
		}
		break;
	case WindowMenu:
		pWindowMenu(myIcon,item);
		break;
	}

	uSetCursor(myIcon,-uARROW_CURSOR);
	HiliteMenu(0);
	return 0;
}
int doScale2SetButtons(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	struct ScrollList *list8;
	controlPtr Ok;
	controlPtr Cancel;
	controlPtr Range;
	IconPtr myIcon;
	int ret;
	
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&list8,
              	              	              
              uDialogSetItem,0L,
              uDialogGetControl,&Ok,

              uDialogSetItem,1L,
              uDialogGetControl,&Cancel,
              
              uDialogSetItem,3L,
              uDialogGetControl,&Range,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	if(ScrollListIsSelectAny(list2) && ScrollListIsSelectAny(list8)){
	    uHiliteControl(Ok,0);
	    uHiliteControl(Range,0);
	}else{
	    uHiliteControl(Ok,255);
	    uHiliteControl(Range,255);
	}

	return 0;
}

int doScale2Action(struct uDialogStruct *uList)
{
	
	struct FileInfo2 *Files;
	dumpPtr d;
	int reti;
	
	if(!uList)return 1;
	d=(dumpPtr)uList->myIconItem;
	if(!d)return 1;
	Files=d->Files;
	if(!Files)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 3){
		   GetRange2(uList);
		   uDialogUpdate(uList);
	}else if(uList->actionItem == 0){
	    if(doPlot2(uList))goto OutOfHere;
	    if(d->flag){
			reti=uDialogSet(uList,0L,
		      uDialogSetExitOk,
		      uDialogSetDone,uDialogSetDone
			);
			if(reti)goto OutOfHere;
		}
	}else if(uList->actionItem == 1){
		reti=uDialogSet(uList,0L,
	      uDialogSetExitOk,
	      uDialogSetDone,uDialogSetDone
		);
		if(reti)goto OutOfHere;
	}else if(uList->actionItem == 2){
	   if(DoClickInList2(uList))goto OutOfHere;
	}else if(uList->actionItem == 8){
	   if(DoClickInList8(uList))goto OutOfHere;
	}
OutOfHere:
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}


static int doPlot2(struct uDialogStruct *uList)
{
	struct ScrollList *List8,*List2,*List9;
	struct Message1000 m;
	char nameItem[256],name[256];
	struct FileInfo2 *Files;
	long counti;
	dumpPtr d;
	int reti;
	int ret;
    long first2,first3;
    long count,nlist,count3,nlist3;
    char **nameList;
    int isSimplePlot;
    
    
	if(!uList)return 1;
	d=(dumpPtr)uList->myIconItem;
	if(!d)return 1;
	Files=d->Files;
	if(!Files)return 1;
	
	ret = 1;

	reti=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List8,
              	              	              
              uDialogSetItem,9L,
              uDialogGetScrollList,&List9,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(reti)return 1;
	
	nameList=NULL;	
	
	isSimplePlot=TRUE;

	first2 = ScrollListFirstSelect(List8);
	if(first2 < 0){
	   Warning("No Item Selected To Plot");
	   return 0;
	}
	
	first3 = ScrollListFirstSelect(List9);
	count3=ScrollListCount(List9);
	
	counti=0;

	count=ScrollListCount(List8);
	for(nlist=0;nlist<count;++nlist){
		if(!ScrollListIsSelected(nlist,List8))continue;			
		if(ScrollListGet(nameItem,nlist,List8)){
		   	WarningBatch("doPlot2Out: Error Getting Item Name\n");
		   	goto OutOfHere;
		}
		
		if(!strncmp(nameItem,"tracer_data",11)){
		    if(first3 < 0){
	   		    Warning("Trace_data: Item Must Be Selected to Plot");
			    goto OutOfHere;
			}
			isSimplePlot=FALSE;
		    counti += count3;		
		}else if(!strncmp(nameItem,"laser_plot_e",12) || !strncmp(nameItem,"laser_plot_x",12) ||
		         !strncmp(nameItem,"laser_plot_y",12) || !strncmp(nameItem,"laser_plot_z",12)
		       ){
			isSimplePlot=FALSE;		
		    counti++;		
		}else{
		    if(first3 < 0){
	   		    Warning(" Item Index To Plot Must Be Selected ");
			    goto OutOfHere;
			}
		    counti++;		
		}
	}
	
	
	nameList=(char **)cMalloc(counti*sizeof(char *),2834);
	if(!nameList)goto OutOfHere;
	
	
	counti=0;
	for(nlist=0;nlist<count;++nlist){
		if(!ScrollListIsSelected(nlist,List8))continue;	
				
		if(ScrollListGet(nameItem,nlist,List8)){
		   	WarningBatch("doPlot2Out: Error Getting Item Name\n");
		   	goto OutOfHere;
		}
		
		if(isSimplePlot){
			sprintf(name,"%s[%ld]",nameItem,first3);
			nameList[counti]=strsave(name,9235);
			if(!nameList[counti]){
				sprintf(WarningBuff,"doPlot2Out: Error Of Memory %s\n",nameItem);
				WarningBatch(WarningBuff);
				goto OutOfHere;
			}				
			counti++;								
			break;
		}
		if(!strncmp(nameItem,"tracer_data",11)){
			for(nlist3=0;nlist3<count3;++nlist3){
				if(!ScrollListIsSelected(nlist3,List9))continue;	
				sprintf(nameItem,"tracer_data(%ld)",nlist3);
				nameList[counti]=strsave(nameItem,9237);
				if(!nameList[counti]){
					sprintf(WarningBuff,"doPlot2Out: Error Of Memory %ld %s\n",nlist3,nameItem);
					WarningBatch(WarningBuff);
					goto OutOfHere;
				}				
				counti++;								
			}
			
			continue;
		}
		nameList[counti]=strsave(nameItem,9237);
		if(!nameList[counti]){
			sprintf(WarningBuff,"doPlot2Out: Error Of Memory %ld %s\n",nlist,nameItem);
			WarningBatch(WarningBuff);
			goto OutOfHere;
		}
		
		counti++;
		
	}
	
	if(d->flag){	
	    Rays.nameList=nameList;
	    Rays.nameCount=counti;	
		nameList=NULL;
	}else if(isSimplePlot){
		struct linedata li;
		zerol((char *)&li,sizeof(li));
		
		GetWindowName(uList->myIcon,li.sourceName,0);
		
		mstrncpy((char *)&li.pioName,nameList[0],255);
		
		li.type=LINEDATA_POINT_DATA;

	    li.LastFrame=Files->ImageCount-1;

	    li.LastTime=(double)(Files->ImageCount-1);
	    
	    li.FrameCount=Files->ImageCount;
	
		CDrawNewLine(&li,1);
		
	}else{
	
		zerol((char *)&m,sizeof(m));
		
		m.nameList=nameList;
		m.nameCount=counti;
		
		SendMessageByIcon(d->myIcon,MessageType_PLOT_LINE_DATA,&m);
		nameList=NULL;
	}
	

	ret = 0;
OutOfHere:

	if(nameList){
	    for(nlist=0;nlist<counti;++nlist){
	        if(nameList[nlist])cFree((char *)nameList[nlist]);
	        nameList[nlist]=NULL;
	    }
	    cFree((char *)nameList);
	    nameList=NULL;
	}

	return ret;
}

static int GetRange2(struct uDialogStruct *uList)
{
	struct ScrollList *List1,*List2;
	double *fdata,*daughter;
	char nameItem[256];
	struct FileInfo2 *Files;
	dumpPtr d;
	long n;
	int reti;
	int ret;
    
	if(!uList)return 1;
	d=(dumpPtr)uList->myIconItem;
	if(!d)return 1;
	Files=d->Files;
	if(!Files)return 1;

	reti=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List1,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(reti)return 1;
	
	{
	    long first,first2;
	    double dmin,dmax;
	    long dataLength2;
	    long dataLength;
	    int plotItem;
	    long count,count1,nlist,nlist1;

	   first = ScrollListFirstSelect(List2);
	   if(first < 0){
	       Warning("No File Selected To Range");
	       return 0;
	   }
	   
	   first2 = ScrollListFirstSelect(List1);
	   if(first2 < 0){
	       Warning("No Item Selected To Range");
	       return 0;
	   }

		if(ScrollListGet(nameItem,first2,List1)){
		   Warning("Error Getting Item Name To Range");
		   return 0;
		}

        plotItem=0;
        count1=ScrollListCount(List1);
        	        
	    dmin =  1e60;
	    dmax = -1e60;

	    count=ScrollListCount(List2);
	    fdata=NULL;
	    daughter=NULL;

	    for(nlist=0;nlist<count;++nlist){
			if(!ScrollListIsSelected(nlist,List2))continue;
			
			ret=SageGetFileItem(Files,"cell_daughter",0,&daughter,&dataLength2,nlist);

	        for(nlist1=0;nlist1<count1;++nlist1){
			    if(!ScrollListIsSelected(nlist1,List1))continue;
		        ScrollListGet(nameItem,nlist1,List1);
			
				if(SageGetFileItem(Files,nameItem,plotItem,&fdata,&dataLength,nlist)){
				    ret=17;goto OutOfHere;
				}
								
				if(!ret && (dataLength == dataLength2)){
					for(n=0;n<dataLength;++n){
						if(daughter[n])continue;
						if(fdata[n] > dmax)dmax=fdata[n];
						if(fdata[n] < dmin)dmin=fdata[n];
					}
				}else{
					for(n=0;n<dataLength;++n){
						if(fdata[n] > dmax)dmax=fdata[n];
						if(fdata[n] < dmin)dmin=fdata[n];
					}
				}

				if(fdata)cFree((char *)fdata);
				fdata=NULL;
			
        	}
						
			if(daughter)cFree((char *)daughter);
			daughter=NULL;
	   }

		reti=uDialogSet(uList,6L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,dmax,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(reti)goto OutOfHere;
	   
	
	}

	ret = 0;
OutOfHere:
	if(fdata)cFree((char *)fdata);
	if(daughter)cFree((char *)daughter);
	ret = 1;
	return ret;
}

static int DoClickInList8(struct uDialogStruct *uList)
{
	struct ScrollList *List1,*List2,*List3;
	double *fdata;
	char namebuff[256],nameItem[256];
	struct FileInfo2 *Files;
    long count,ItemCount;
	dumpPtr d;
	int reti;
	int ret;
    
	if(!uList)return 1;
	d=(dumpPtr)uList->myIconItem;
	if(!d)return 1;
	Files=d->Files;
	if(!Files)return 1;

	reti=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List1,
              	              	              
              uDialogSetItem,9L,
              uDialogGetScrollList,&List3,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(reti)return 1;
	
	{
	    long first,first2;
	    long dataLength;
	    int plotItem;
	    long nlist;

	   first = ScrollListFirstSelect(List2);
	   if(first < 0){
	       return 0;
	   }
	   first2 = ScrollListFirstSelect(List1);
	   if(first2 < 0){
	       return 0;
	   }

		if(ScrollListGet(nameItem,first2,List1)){
		   Warning("Error Getting Item Name To Range");
		   return 0;
		}

        plotItem=0;
        count=ScrollListCount(List1);
        for(nlist=0;nlist<count;++nlist){
	        ScrollListGet(namebuff,nlist,List1);
            if(!strcmp(namebuff,nameItem)){
                if(nlist == first2)break;
	            ++plotItem;
            }
        }
	        
	    count=ScrollListCount(List2);
	    fdata=NULL;

	    for(nlist=0;nlist<count;++nlist){
			if(!ScrollListIsSelected(nlist,List2))continue;
			if(SageGetFileItem(Files,nameItem,plotItem,&fdata,&dataLength,nlist)){
			    ret=17;goto OutOfHere;
			}
			if(!fdata)break;
			if(d->fdata)cFree((char *)d->fdata);
			d->fdata=fdata;
			d->dataLength=dataLength;

			fdata=NULL;
			break;
	   }
	   
		ItemCount = -1;
		
		count=ScrollListCount(List3);
		if(count){
		   ItemCount=ScrollListFirstSelect(List3);
		}
		
		ScrollListDelete(0L,count,List3);	   

		ScrollListSetDouble(d->fdata,d->dataLength,List3);
		
		d->fdata=NULL;
	   
		count=ScrollListCount(List3);
		if(count && (ItemCount >= 0)){
		    ScrollListSetItemSelected(ItemCount,List3);
		}	   

    	ScrollListUpdate(List1);
    	ScrollListUpdate(List2);
    	ScrollListUpdate(List3);
	}

	ret = 0;
OutOfHere:
	if(fdata)cFree((char *)fdata);

	return ret;
}
static int DoClickInList2(struct uDialogStruct *uList)
{
	struct ScrollList *List1,*List2;
	char ListOneItem[256];
	struct FileInfo2 *Files;
    long n,count,ItemCount;
    dumpPtr d;
    int ret;

	if(!uList)return 1;
	d=(dumpPtr)uList->myIconItem;
	if(!d)return 1;
	Files=d->Files;
	if(!Files)return 1;


	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List1,
              	              	              
              uDialogSetDone,uDialogSetDone
    );		
	if(ret) return 1;
	
	ListOneItem[0]=0;
	ItemCount = -1;
	
	count=ScrollListCount(List1);
	if(count){
	   n=ScrollListFirstSelect(List1);
	   if(n >= 0){
	       ScrollListGet(ListOneItem,n,List1);
	       ItemCount=ScrollListGetItemCount(ListOneItem,n,List1);
	   }
	}
	ScrollListDelete(0L,count,List1);
	
	n=ScrollListFirstSelect(List2);

	if(filePIOList(Files,n,List1,0))goto ErrorOut;
		
	doScale2Move(uList);
		
	count=ScrollListCount(List1);
	if(count && ListOneItem[0]){
	    n=ScrollListFindItem(ListOneItem,ItemCount,List1);
	    if(n >= 0){
	       ScrollListSetItemSelected(n,List1);
	    }
	}
	
	DoClickInList8(uList);
	    
ErrorOut:

	return 0;
}
int doScale2Start(struct uDialogStruct *uList)
{
	struct ScrollList *List8;
	struct ScrollList *list2;
	struct FileInfo2 *Files;
	dumpPtr d;
	char *name;
	long iTem;
	int ret;
	long n;
	
	
	if(!uList)return 1;
	d=(dumpPtr)uList->myIconItem;
	if(!d)return 1;
	Files=d->Files;
	if(!Files)return 1;


	if(!IsShift()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List8,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	for(n=0;n<Files->ImageCount;++n){
	
 		char *names[1];
		int type=0;

		 names[0]= FilesImageName(Files,n);
		ScrollListInsert(names,&type,n,1L,list2);
	}
	ScrollListSetTop(0,list2);
	
	DoClickInList2(uList);
	
	name=Files->pioData.pioName;
	if(name[0] == 0)name="density";
	iTem=Files->pioData.pioIndex;
		
	n=ScrollListFindItem(name,iTem,List8);
	if(n >= 0){
		ScrollListSetItemSelected(n,List8);
	}

	ScrollListSetItemSelected(0L,list2);

	if(uList->myIcon){
	    InvalRectMyWindow(uList->myIcon);	
		uList->myIcon->pCheckMenu=CheckMenu;
		uList->myIcon->pMenu=Menu;
		uList->myIcon->Files=Files;
		uList->myIcon->pMessage=DumpGetMessage;
    	SetWindowToFileName(uList->myIcon,Files);
	    doUniversalDump(uList->myIcon,universal_GetDefaults,NULL);
	}	
	
	return 0;
}

static int doScale2Keys(struct uDialogStruct *uList)
{

	struct ScrollList *List1;
	int ret;
	
	
	if(!uList)return 1;
	
	ret=uDialogSet(uList,8L,
              uDialogGetScrollList,&List1,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	ScrollListSetString(uList->mess,List1);
	
	DoClickInList8(uList);
	
	return 0;
}
int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	struct ScrollList *list8;
	struct ScrollList *list9;
	uRect rList1,rList2,rList3;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;
	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,10,r.ysize-35,
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize-85,r.ysize-35,
              	              	              
              uDialogSetItem,3L,
              uDialogSetLocation,(r.xsize)/2-60,r.ysize-35,

              uDialogSetItem,2L,
              uDialogGetRect,&rList1,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,8L,
              uDialogGetRect,&rList2,
              uDialogGetScrollList,&list8,
              	              	              
              uDialogSetItem,9L,
              uDialogGetRect,&rList3,
              uDialogGetScrollList,&list9,
              	              	              
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)return 1;
	
	
			
	rList1.x = 15;
	rList1.y = 25+MenuOffSet;
	rList1.xsize = (r.xsize-rList1.x)/3;
	rList1.ysize = r.ysize-40-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	
	
	rList2=rList1;
	rList2.x = rList1.x+rList1.xsize;
	
	ScrollListFitRect(&rList2,list8);

	rList3=rList1;
	rList3.x = rList2.x+rList1.xsize;
	
	ScrollListFitRect(&rList3,list9);

	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList1,
              
              uDialogSetItem,8L,
              uDialogSetRect,&rList2,
              	              	              
              uDialogSetItem,9L,
              uDialogSetRect,&rList3,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	uDialogUpdate(uList);

	return 0;
}
static int fClose(IconPtr myIcon)
{
	dumpPtr d;
		
	if(myIcon){
	    d=(dumpPtr)myIcon;
   	    if(d){
		    if(d->Files && !d->flag){
			    freeFileList(d->Files);
		    	d->Files=NULL;
		    }
			if(d->fdata)cFree((char *)d->fdata);
		    cFree((char *)d);
	    }
	       	    
	}
	return 0;

}
static int doDumpGetCellData(IconPtr myIcon,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FileInfo2 *Files;
	struct SetFrameData *s;
	char name[256];
	long length;
	double *fdata;
    int ret;

	if(!myIcon)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	if(!sd)return 1;
	s = &Files->SetData;

	ret=1;
	

	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doDumpGetCellData - Only Returns LINEDATA_CELL_DATA\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	name[0]=0;
	mstrncpy(name,sd->pioName,255);
	if(name[0] == 0){
		mstrncpy(name,"controller_r8",255);
	}

	fdata=NULL;
	if(SageGetFileItem(Files,name,sd->pioIndex,&fdata,&length,CurrentFrame)){
	    ret=17;goto ErrorOut;
	}
	        			
    sd->xsize=length;
    sd->ysize=1;
    sd->zsize=1;
    sd->length=length;
    sd->data=fdata;
    sd->pioTime=s->pioTime;
    if(!sd->data){
    	sprintf(WarningBuff,"doDumpGetCellData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }

    
    ret=0;
    
ErrorOut:
	
	return ret;

}
static int doDumpGetPointData(struct FileInfo2 *Files,struct linedata *li)
{
	struct SetFrameData *s;
	double dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    char name[256],*fp;
	long length;
	double *fdata;
    int item;

	if(!Files || !li)return 1;
	s = &Files->SetData;
	
	if(li->type != LINEDATA_POINT_DATA)return 1;
	
		
	xD=NULL;
	yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	item=0;
	fdata=NULL;
	
	if(li->type != LINEDATA_POINT_DATA)return 1;
	
	if(li->pioName[0]){
	    mstrncpy(name,li->pioName,255);
		fp=strrchr((char *)name,'[');
		if(fp){
			*fp=0;
			++fp;
			item=atoi(fp);
			if(item < 0)return 1;
		}
	}else{
	    return 1;
	}

	nFirst=li->FirstFrame;
	nLast=li->LastFrame;
	if(nFirst >= nLast){
	    if(Files){
	    	nFirst=0;
	    	nLast=Files->ImageCount-1;
	    }else{
	    	goto OutOfHere;
	    }
	}
	
	if(nLast >= Files->ImageCount){
		nLast=Files->ImageCount-1;
		if(nLast < 0)nLast=0;
	}
	
	if(nFirst >= nLast)nFirst=0;

	ns=nLast-nFirst+1;

	if(ns < 2)goto OutOfHere;

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81872))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81882))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}

	tFirst=li->FirstTime;
	tLast=li->LastTime;
	if(tFirst >= tLast){
	    if(Files){
	    	tFirst=0;
	    	tLast=(double)(Files->ImageCount-1);
	    }else{
	    	goto OutOfHere;
	    }
	}

	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	
	nd=0;
	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;

		fdata=NULL;
		if(SageGetFileItem(Files,name,li->pioIndex,&fdata,&length,CurrentFrame)){
		    goto OutOfHere;
		}
		
		if(item >= length)continue;   
		
		value=fdata[item];
	    
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(s){
	        dp=s->pioTime;
	    }
	    xD[nd]=dp;
	    yD[nd]=value;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;
	    nd++;
    	if(fdata)cFree((char *)fdata);
    	fdata=NULL;
	}

	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	return 0;
OutOfHere:
    if(xD)cFree((char *)xD);
    if(yD)cFree((char *)yD);
    if(fdata)cFree((char *)fdata);
	return 1;
}	
static int doDumpGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	if(!Files || !li)return 1;
	
	if(li->type == LINEDATA_POINT_DATA){
	    return doDumpGetPointData(Files,li);
	}
	
	return 0;
}	

static int DumpGetMessage(IconPtr myIcon,long MessageType,void *MessageData)
{
	struct Message1000 *m;
	struct SetFrameData  *sd;
	struct linedata *li;
	char windowName[256];
	uPort save;
	dumpPtr s;
	long n;
	int ret;

	if(!myIcon)return 1;
	s=(dumpPtr)myIcon->DWindow;
	if(!s)return 1;	
	m=(struct Message1000 *)MessageData;
	if(!m)return 1;	
		
	if(!myIcon->Files)return 1;

	ret=1;
	uGetPort(&save);
	SetPortMyWindow(myIcon);
	
	switch(MessageType){
	case MessageType_LIMITS:
	    if(!m)goto ErrorOut;
		break;
	case MessageType_SET_SYNC_REGION:
		break;
	case MessageType_SET_FRAME_NUMBER:
	    if(!myIcon->Synchronize)goto OK;
	    s->CurrentFrame=m->CurrentFrame;
	    goto OK;
	case MessageType_SET_PALETTE:
	    break;
	case MessageType_GET_FRAME_COUNT:
	    if(myIcon->Files){
		    if(myIcon->Files->ImageCount > m->FrameCount)m->FrameCount=myIcon->Files->ImageCount;
		}
		goto OK;
	case MessageType_GET_LINE_DATA:
	    li=MessageData;
	    if(!li)goto ErrorOut;
	    ret=doDumpGetLineData(myIcon->Files,li);
	    goto ErrorOut;
	case MessageType_GET_CELL_DATA:
	    sd=MessageData;
	    ret = doDumpGetCellData(myIcon,sd->CurrentFrame,sd);
	    goto ErrorOut;	    	    
	case MessageType_GET_AREA_DATA:
	    goto ErrorOut;	    
	case MessageType_GET_REGION_DATA:
	    goto ErrorOut;
	case MessageType_SET_AREA_RANGE:
	    goto ErrorOut;
	case MessageType_SET_REGION_DATA:
	    goto ErrorOut;
	case MessageType_GET_LIMITS_DATA:
	break; 
	case MessageType_PRINT_FILE_INFORMATION:
		GetWindowName(myIcon,windowName,0);
	    sprintf(WarningBuff,"Window \'%s\'\n%c",windowName,0);
	    WarningBatch(WarningBuff);
	    printFileInformation(myIcon->Files);
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
	}
	InvalRectMyWindow(myIcon);
OK:
	ret = 0;
ErrorOut:
	uSetPort(&save);
	return ret;
}
int doUniversalDump(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct uDumpFile ss;
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
		{"TABL",4200,universal_TypeTable,(void *)&Global2},	
	};
	

	static struct universalTableStruct Global= {
		"DumpFile parameters",universal_WindowDump,
		(void *)&ss,sizeof(ss),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};


	dumpPtr s;
	int ret;
	
	s=NULL;
	if(myIcon && (myIcon->WindowType == DumpWindowType)){
		s=(dumpPtr)myIcon->DWindow;
	}else{
	    myIcon=NULL;
	}

	Global.tableDynamic=s;
	Global.tableDynamicSpace=sizeof(struct uDumpFile);

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
			
		}
		ss.defaultsSet=TRUE;
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		/*			
		if(myIcon && uPref.Files){
			doLoadImages(myIcon,uPref.Files);
		    uPref.Files=NULL;
		}
		*/			
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
