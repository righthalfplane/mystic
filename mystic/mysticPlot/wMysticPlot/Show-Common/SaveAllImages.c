#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "uDialog.h"
#include "SaveAllImages.h"

struct ImageSaveList *ImagesGet(struct uDialogStruct *uList,struct FileInfo2 *Files);
char *DefaultPathString(void);


static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int doScale2SetButtons(struct uDialogStruct *uList);

struct ImageSaveList *SaveAllImages(char *name,struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {10,50,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {80,50,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,40,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"All", {315,29,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"Option", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,300};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
 	
	struct ImageSaveList *Images;
 	uPort Port;
	int ret;
	
	
	if(!name || !Files)return NULL;
	
	ret=1;
	uGetPort(&Port);
	
	Images=(struct ImageSaveList *)NULL;
	
	uR=(uDialogReturn *)NULL;
	
	uList=uDialogOpen(name,&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetMyIcon,Files,
	              uDialogSetMove,doScale2Move,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetButtons,doScale2SetButtons,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
	Images=ImagesGet(uList,Files);
	if(!Images)goto ErrorOut;
	
	
	ret=0;
	
ErrorOut:

    if(ret){
	    if(Images)ImageFreeList(Images);
	    Images=(struct ImageSaveList *)NULL;
	}
	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
     
	uSetPort(&Port);
	
	return Images;
}
struct ImageSaveList *ImagesGet(struct uDialogStruct *uList,struct FileInfo2 *Files)
{

   	struct ImageSaveList *Images;
	struct ScrollList *list2;
	uDialogReturn *uR;
	long lcount,count,n,cf;
	int done,ret;
	
	if(!uList || !Files)return (struct ImageSaveList *)NULL;
	
	Images=(struct ImageSaveList *)NULL;

	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)goto ErrorOut;

	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
	done=(int)uR[4].dreturn+1;
	
    if(uR)uDialogFreeReturn(uR);	
	
	lcount=ScrollListCount(list2);
	count=0;
	if(done == 1){
		for(n=0;n<lcount;++n){
		    if(ScrollListIsSelected(n,list2)){
	 	        ++count;		       
		    }
		}
	}else{
		for(n=0;n<lcount;++n){
            ScrollListSelect(n,list2);
 	        ++count;		       
		}
	}
	
	if(count <= 0)return NULL;
	
	Images=CreateImageList(count);
	if(!Images)goto ErrorOut;
	
	Images->Files=Files;
	
	cf=0;
	for(n=0;n<lcount;++n){
		if(ScrollListIsSelected(n,list2)){
 	        Images->imageList[cf++]=n;
	    }
	}

	Images->count=cf;
		
 	Images->directory=DefaultPathString();	
 	if(!Images->directory)goto ErrorOut;
	
	return Images;
ErrorOut:

	if(Images)ImageFreeList(Images);
	
	return (struct ImageSaveList *)NULL;
}
static int doScale2SetButtons(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	controlPtr Ok;
	controlPtr Range;
	int ret;
	
	
	if(!uList)return 1;
		
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,0L,
              uDialogGetControl,&Ok,
              
              uDialogSetItem,3L,
              uDialogGetControl,&Range,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	uHiliteControl(Range,0);
	    
	if(ScrollListIsSelectAny(list2)){
	    uHiliteControl(Ok,0);
	}else{
	    uHiliteControl(Ok,255);
	}

	
	return 0;
}

static int doScale2Action(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	int ret;
	
	
	if(!uList)return 1;
	
	uSetCursor(uList->myIcon,uWATCH_CURSOR);
	if((uList->actionItem == 0) ||(uList->actionItem == 3)){
	
	
		ret=uDialogSet(uList,2L,
	              uDialogGetScrollList,&list2,
	              
	              uDialogSetItem,4L,
	              uDialogSetFloatAux,(double)(uList->actionItem == 3),
	              
	              uDialogSetExitOk,
	              	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
	    
		if(ret)goto OutOfHere;
				 
	}
OutOfHere:
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,uARROW_CURSOR);
	return 0;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	struct FileInfo2 *Files;
	struct ScrollList *list2;
	int ret;
	int n;
	
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	for(n=0;n<Files->ImageCount;++n){
	
 		char *names[1];
		int type=3;

		 names[0]= FilesImageName(Files,n);
		ScrollListInsert(names,&type,n,1L,list2);
	}
	ScrollListSetTop(0,list2);

	return 0;
}
static int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	uRect rList1;
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
              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto OutOfHere;
	
	
	
	
	rList1.x = 15;
	rList1.y = 25;
	rList1.xsize = (r.xsize-rList1.x);
	rList1.ysize = r.ysize-35-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	

	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList1,
              
              uDialogSetDone,uDialogSetDone
    );
OutOfHere:
	return 0;
}

struct ImageSaveList *CreateImageList(long count)
{
	struct ImageSaveList *Images;
	
	if(count <= 0)return (struct ImageSaveList *)NULL;
	
	Images=(struct ImageSaveList *)cMalloc(sizeof(struct ImageSaveList),8199);
	if(!Images)return (struct ImageSaveList *)NULL;
	zerol((char *)Images,sizeof(struct ImageSaveList));
	
	Images->imageList=(long *)cMalloc(count*sizeof(long),8200);
	if(!Images->imageList){
	    cFree((char *)Images);
	    return (struct ImageSaveList *)NULL;
	}
	zerol((char *)Images->imageList,sizeof(count*sizeof(long)));
	Images->count=count;
	return Images;
}


int ImageFreeList(struct ImageSaveList *Images)
{
	if(!Images)return 1;
	
	if(Images->directory){
		cFree((char *)Images->directory);
	    Images->directory=NULL;
	}
	
	if(Images->imageList){
		cFree((char *)Images->imageList);
	    Images->imageList=NULL;
	}
	
	cFree((char *)Images);
	
	return 0;
}
