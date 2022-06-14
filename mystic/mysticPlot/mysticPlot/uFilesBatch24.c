#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Linedata.h"
#include "Message.h"

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2

#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))

int getPICTData(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame);

int trueOut(char *filepath,char *name,long xsize,long ysize,unsigned char *data,long n,int flag);

int WriteRectToRect(unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize);
                    
int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);

int doJPG(char *filename,unsigned char *image,int xsize,int ysize);

static int GetFrameInfo(struct FileInfo2 *Files,long CurrentFrame);

static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int doScale2SetButtons(struct uDialogStruct *uList);
static int doDataSelection(struct FileInfo2 *Files);
static int GetRange3(struct uDialogStruct *uList);

int setFloat(double *d,long length);

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

static int do24BitDoDialog(struct FileInfo2 *Files,int *type);

int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

static int do24BitInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int do24Bit(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);


static int doAuto(struct FileInfo2 *Files);

static int Get24BitData(struct FileInfo2 *Files,long CurrentFrame);

static int Plot24BitData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int getTrueData(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame);

static int setSds2dScales(struct FilePIOInfo *pio);

static int sds2dSetLimits(struct FileInfo2 *Files,long nf);

static int do24BitGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int do24BitGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int pioGetValue(struct FileInfo2 *Files,double x,double y,double *v);
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);
static int do24BitGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int do24BitPutData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);


int do24BitBatch(BatchPtr Batch,char *name)
{
	struct FileInfo2 *Files;
	struct CommandInfo cp;
	char line[4096];
	char *fclass;
	int ret;
	
    if(!Batch || !name)return 1;
    Files=FileManagerGet(name);    
    if(!Files)return 1;
    
    ret=1;
    
	zerol((char *)&cp,sizeof(struct CommandInfo));

    if(Files->fileClass != RGB_FILES && Files->fileClass != PICT_FILES &&
       Files->fileClass != TRUE_FILES &&
       Files->fileClass != BMP_FILES && Files->fileClass != GIF_FILES &&
       Files->fileClass != JPG_FILES && Files->fileClass != QUICKTIME_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"do24BitBatch Found (%s) instead of a 24 Bit Color Files\n",fclass);
        }else{
            sprintf(WarningBuff,"do24BitBatch Can Only Process 24 Bit Color Files\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
	
    if(do24BitStart(Files))goto ErrorOut;
	           
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(do24Bit(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}
int do24BitStart(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	

	pio->range.AspectRatio=1.0;
	pio->range.MaintainAspectRatio=1;
	pio->range.receiveRange=1;
		
	pio->pd.sType=0;
	pio->pd.sPmin=0;
	pio->pd.sPmax=1;	
	pio->pd.LabeledPalette=FALSE;	
	pio->pd.paletteFont=2;

	pio->pd.UsePlotScales=1;
	pio->pd.UsePlotTime=1;
	pio->pd.UsePlotRange=1;
	
	pio->pa.DrawZones=0;
	pio->pd.AreaFill=1;
	
	pio->xmax=1;
	pio->ymax=1;
	
	Files->pGetData2=do24BitGetData;
	
	Files->pPutData2=do24BitPutData;
	
	Files->pGetAreaData=do24BitGetAreaData;
	
	Files->pGetLineData=do24BitGetLineData;
	
	Files->pInformation=do24BitInformation;
	
	Files->pDoDialog=do24BitDoDialog;

	
	pio->raster.xsize=Files->xmax;
	pio->raster.ysize=Files->ymax;
	
	pio->raster.xScale=1.0;
	pio->raster.yScale=1.0;
	
	pio->raster.box.x=0;
	pio->raster.box.y=0;
	
	pio->raster.box.xsize=(int)pio->raster.xsize;
	pio->raster.box.ysize=(int)pio->raster.ysize;

	pio->raster.sampleType = FRAME_SAMPLE_SIMPLE;
	pio->raster.plotType   = FRAME_DATA_RGB;
	pio->raster.boxType    = FRAME_BOX_SAMPLE;
		
	return 0;
}
int doDataScale2(struct FileInfo2 *Files);
int doDataRange(struct FileInfo2 *Files);
int doVectorSelection(struct FileInfo2 *Files);

static int do24BitDoDialog(struct FileInfo2 *Files,int *type)
{
	if(!Files || !type)return 1;
	
	if(*type == MessageType_DO_SCALES_DIALOG){
		return doDataScale2(Files);
	}else if(*type == MessageType_DO_RANGE_DIALOG){
		return doDataRange(Files);	
	}else if(*type == MessageType_DO_SELECTION_DIALOG){
		return doDataSelection(Files);	
	}else if(*type == MessageType_DO_VECTOR_DIALOG){
		return doVectorSelection(Files);	
	}
	return 0;
}
static int doDataSelection(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {10,50,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {80,50,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,40,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"Range", {315,29,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"dmin", {5,5,65,20},uDialogString,uDialogNone},
      /*  5 */      {"dmax", {180,5,65,20},uDialogString,uDialogNone},
      /*  6 */      {"", {50,5,110,15},uDialogText,uDialogGetDouble},
      /*  7 */      {"", {240,5,110,15},uDialogText,uDialogGetDouble},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,300};
 	
 	uDialogList *uList;
    int ret;
    
	if(!Files)return 1;
	
	ret=1;
	
	uList=uDialogOpen("Range Data",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetMyIcon,Files,
	              uDialogSetMove,doScale2Move,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetButtons,doScale2SetButtons,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
	
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);

	return ret;
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
	
	if(ScrollListIsSelectAny(list2)){
	    uHiliteControl(Ok,0);
	    uHiliteControl(Range,0);
	}else{
	    uHiliteControl(Ok,255);
	    uHiliteControl(Range,255);
	}

	
	return 0;
}

static int doScale2Action(struct uDialogStruct *uList)
{
	
	
	if(!uList)return 1;
	
	uSetCursor(NULL,uWATCH_CURSOR);
	if(uList->actionItem == 3){
	   GetRange3(uList);	 
	}

	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,uARROW_CURSOR);
	return 0;
}
static int GetRange3(struct uDialogStruct *uList)
{
	struct FileInfo2 *Files;
	struct SetFrameData  sd;
	struct ScrollList *List2;
	double *data;
 	double dmin,dmax;
	long nlist,count;
	long n,length;
	int reti;
	int ret;
    
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	
	sd.data=NULL;  

	reti=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
              
              uDialogSetDone,uDialogSetDone
    );
    
	if(reti)return 1;
	

		
	    dmin =  1e60;
	    dmax = -1e60;

	    count=ScrollListCount(List2);

	    for(nlist=0;nlist<count;++nlist){
	    
		   if(!ScrollListIsSelected(nlist,List2))continue;
		
		    
		    if(FilesGetData(&data,&length,nlist,Files))break;		    		

			for(n=0;n<length;++n){
		    	if(data[n] > dmax)dmax=data[n];
		    	if(data[n] < dmin)dmin=data[n];
			}

		    if(Stop("Stop Ranging Data ?"))break;
	   }
	   
		ret=uDialogSet(uList,6L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,dmax,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
		
		
		
		reti=uDialogSet(uList,6L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,dmax,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(reti)goto OutOfHere;
		
	   
		if(uList->myIcon)InvalRectMyWindow(uList->myIcon);	

	ret = 0;
OutOfHere:
	if(sd.data)cFree((char *)sd.data); 
	ret = 1;
	return ret;
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

static int do24BitInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	
	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	
	sd->raster=pio->raster;
	sd->range=pio->range;
	sd->pd=pio->pd;
	sd->pa=pio->pa;
	sd->plane=pio->plane;
	sd->vector=pio->vector;

	if(pio->pioName[0]){
	    mstrncpy(sd->pioName,pio->pioName,255);
	    sd->pioIndex=(int)pio->pioIndex;
	}else{
	   sd->pioName[0]=0;
	   pio->pioIndex=0;
	}

	
	return 0;
}
static int do24BitGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	int ix1,iy1,ix2,iy2;
	double x1,y1,x2,y2,dl,ds,dx,dy,dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	
	if(!li)return 1;
	if(li->type == LINEDATA_POINT_DATA)return doPointGetData(Files,li);
	if(!Files)return 1;
	pio=&Files->pioData;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA)return 1;
	

	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;
	
	pio->pd.AreaFill=TRUE;

	x1=li->x1;
	y1=li->y1;
	x2=li->x2;
	y2=li->y2;

	if(RealLoc(pio,&x1,&y1,&ix1,&iy1))return 1;
	if(RealLoc(pio,&x2,&y2,&ix2,&iy2))return 1;
	
	dl=sqrt((ix2-ix1)*(ix2-ix1)+(iy2-iy1)*(iy2-iy1));

	if(dl <= 0.0)dl=2.0;

	ns=(long)(2*dl+2);

	if(x1 == x2){
	    dl=abs(iy1-iy2);
	    ns=(long)(dl+1);
	}else if(y1 == y2){
	    dl=abs(ix1-ix2);
	    ns=(long)(dl+1);
	}

	if(ns < 2)ns=2;
	
	if((li->PointLineCount > 2) && (li->PointLineCount < 20000))ns=li->PointLineCount;

	ds=1.0/((double)(ns-1));


	CurrentFrame=li->CurrentFrame;
		
	if(Get24BitData(Files,CurrentFrame))goto OutOfHere;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

    setSds2dScales(pio);
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81873))){
	    WarningBatch("do24BitGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("do24BitGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}


	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	nd=0;
	for(n=0;n<ns;++n){
	    double value,x,y;

	    x=x1+(double)n*ds*(x2-x1);
	    y=y1+(double)n*ds*(y2-y1);

	    if(!pioGetValue(Files,x,y,&value))continue;

	    dx=x-x1;
	    dy=y-y1;
	    dp=sqrt(dx*dx+dy*dy);

	    xD[nd]=dp;
	    yD[nd]=value;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;
	    nd++;
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
	return 1;
}
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li)
{
	double dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	double x,y;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	xD=NULL;
	yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;
	
	if(li->type != LINEDATA_POINT_DATA)return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	pio->pioIndex=(int)li->pioIndex;
	pio->pd.AreaFill=TRUE;

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


	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

	x=li->x1;
	y=li->y1;
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	
	nd=0;
	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    if(Get24BitData(Files,CurrentFrame))goto OutOfHere;
	    
    	setSds2dScales(pio);
		    
	    if(!pioGetValue(Files,x,y,&value))continue;
	    
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(Files->limits.iGotTime && pio->pd.UsePlotTime){
	       dp=Files->limits.time;
	    }
	    xD[nd]=dp;
	    yD[nd]=value;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;
	    nd++;
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
	return 1;
}
static int pioGetValue(struct FileInfo2 *Files,double x,double y,double *v)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	double xc,yc;
	double *value;
	long i,j;
	long loc;
	long ne;

	double xmin,ymin;
	double xmax,ymax;
	double odxset,odyset;
	long ixmax,ixymax;

	if(!Files || !v)return 0;
	pio=&Files->pioData;
	s = &Files->SetData;
	value=(double *)s->data;
	if(!value)return 1;
		
	
	xmin=pio->xmin;
	if(x < xmin)return 0;
	
	xmax=pio->xmax;
	if(x > xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin)return 0;
	
	ymax=pio->ymax;
	if(y > ymax)return 0;
		
	odxset=(double)(s->xsize)/(xmax-xmin);
	
	i=(long)((x-xmin)*odxset);	
	if(i < 0){
	    i=0;
	}else if(i >= s->xsize){
	    i=s->xsize-1;
	}
	
	odyset=(double)(s->ysize)/(ymax-ymin);
	
	j=(long)((y-ymin)*odyset);
	if(j < 0){
	    j=0;
	}else if(j >= s->ysize){
	    j=s->ysize-1;
	}
	
	ixmax=s->xsize;
	ixymax=ixmax*s->ysize;
	
    loc=i+(s->ysize-1-j)*ixmax;
    if(loc < 0 || loc >= ixymax){
		return 0;
    }
    
    ne=loc;
        
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
    	xc=((double)i+0.5)/odxset+xmin;
    	yc=((double)j+0.5)/odyset+ymin;
		*v=value[ne]+pio->gradx[ne]*(x-xc)+
					 pio->grady[ne]*(y-yc);
	}else{
		*v=value[ne];
	}
    
	return 1;
}

static int do24BitGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	long xsize,ysize;
	double *data;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;

	if(ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"do24BitGetAreaData - Only Returns AREADATA_AREA_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	CurrentFrame=ai->CurrentFrame;
	
	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}

	pio->pd.AreaFill=TRUE;
	
	if(Get24BitData(Files,CurrentFrame))return 1;
		
	ai->pioTime=Files->pioTime;
	
	xsize=ai->xsize;
	ysize=ai->ysize;
	
	pio->range.xminData=ai->xminArea;
	pio->range.xmaxData=ai->xmaxArea;
	pio->range.yminData=ai->yminArea;
	pio->range.ymaxData=ai->ymaxArea;
		
	if(ai->xsize <= 1){
	
		sprintf(WarningBuff,"do24BitGetAreaData - xsize less that 2\n");
	    WarningBatch(WarningBuff);
	    return 1;
		
	}else if(ai->ysize <= 1){
	
		sprintf(WarningBuff,"do24BitGetAreaData - ysize less that 2\n");
	    WarningBatch(WarningBuff);
	    return 1;
		
	}
	
		
	pio->range.ixmax=xsize;
	pio->range.iymax=ysize;
	
	pio->range.ixminData=0;
	pio->range.iyminData=0;
	
	pio->range.ixmaxData=xsize;
	pio->range.iymaxData=ysize;
		
	
	data=(double *)cMalloc(xsize*ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,xsize*ysize*sizeof(double));
	
	ai->data=data;
	 	
    if(getPlotData(Files,data,CurrentFrame)){
	    sprintf(WarningBuff,"do24BitGetAreaData - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        return 1;
    }	    
	

	    
	return 0;
}
static int do24BitPutData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	int ret;

	if(!Files || !sd)return 1;
	
	ret=1;

	if(sd->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"do24BitPutData - Only Puts FRAME_DATA_FLOAT");
	    Warning(WarningBuff);
	    goto ErrorOut;
	}

	ret=FileAppendSDS2D(NULL,(int)sd->xsize,(int)sd->ysize,(double *)sd->data,CurrentFrame,&Files);
	
	ret=0;
ErrorOut:
	return ret;
}
static int do24BitGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	long length;
    int not1;

	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	s = &Files->SetData;


	if(sd->type != FRAME_DATA_RASTER){
	    sprintf(WarningBuff,"do24BitGetData - Only Returns FRAME_DATA_RASTER\n");
	    WarningBatch(WarningBuff);
    	return 1;
	}

	not1 = 0;
	if(Get24BitData(Files,CurrentFrame)){
		not1 = 1;
		if(Files->xSize <= 0 || Files->ySize <= 0)return 1;
		pio->range.ixmax=Files->xSize;
		pio->range.iymax=Files->ySize;
	}

	pio->raster  = sd->raster;		
        		
    sd->xsize=pio->raster.box.xsize;
    sd->ysize=pio->raster.box.ysize;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*3,9207);
    if(!sd->data){
    	sprintf(WarningBuff,"do24BitGetData - Out of Memory\n");
    	WarningBatch(WarningBuff);
    	return 1;
    }

 	memset((char *)sd->data, 255, length*3);
    
    if(not1)return 0;

	if(s->type == PICT_FILES){
	    if(getPICTData(Files,(unsigned char *)sd->data,CurrentFrame))goto ErrorOut;
	}else{    
	    if(getTrueData(Files,(unsigned char *)sd->data,CurrentFrame))goto ErrorOut;
	}
         
ErrorOut:

	return 0;

}
static int getTrueData(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	unsigned char *data;
	long i,j;
	long x,y,xsize,ysize,xinc,iystep;
	double xscale,yscale,oxscale,oyscale;
	int onetoOne;
	register unsigned char *bp,*dp;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(unsigned char *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	
	//CurrentFrame=CurrentFrame;
	
	onetoOne=TRUE;
	if((pio->raster.xScale != 1.0) || (pio->raster.yScale != 1.0))onetoOne=FALSE;	
		
	xscale=pio->raster.xScale*(double)s->xsize;
	oxscale=1.0/pio->raster.xScale;
	yscale=pio->raster.yScale*(double)s->ysize;
	oyscale=1.0/pio->raster.yScale;
	
	x=pio->raster.box.x;
	xsize=pio->raster.box.xsize;
	xinc=xsize*3;
	if((double)(x+xsize) > xscale){
	    xsize=(long)xscale-x;
	}
	
	y=pio->raster.box.y;
	ysize=pio->raster.box.ysize;
	if((double)(y+ysize) > yscale){
	    ysize=(long)yscale-y;
	}
	
	if(onetoOne){		
		for(j=0;j<ysize;++j){
		    bp=sout+j*xinc;
		    dp=data+x*3+s->xsize*3*(y+j);
		    for(i=0;i<xsize;++i){
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
		    }
		}	
	}else{
	    for(j=0;j<ysize;++j){
	        iystep=s->xsize*3*((long)((double)(y+j)*oyscale));
	        bp=sout+j*xinc;
	        for(i=0;i<xsize;++i){
	            dp=data+(3*((long)((double)(x+i)*oxscale))+iystep);
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
	        }
	    }
	}
	
	return 0;
}
static int do24Bit(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
    struct FilePIOInfo *pio;
    long CurrentFrame;
	char *command;
 	double value;
 	
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
	
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
			
	
	if(!mstrcmp("sagedata",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(pio->pioName,command,255);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pioIndex=(int)value;
	}else if(!mstrcmp("xScale",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->raster.xScale=value;
	    pio->raster.box.xsize=(int)(pio->raster.xScale*(double)pio->raster.xsize);
	}else if(!mstrcmp("yScale",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->raster.yScale=value;
	    pio->raster.box.ysize=(int)(pio->raster.yScale*(double)pio->raster.ysize);
	}else if(!mstrcmp("xsize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixmax=(long)value;
	}else if(!mstrcmp("ysize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iymax=(long)value;
	}else if(!mstrcmp("ixmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixminData=(long)value;
	}else if(!mstrcmp("ixmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixmaxData=(long)value;
	}else if(!mstrcmp("iymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iyminData=(long)value;
	}else if(!mstrcmp("iymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iymaxData=(long)value;
	}else if(!mstrcmp("xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.xminData=value;
	}else if(!mstrcmp("xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.xmaxData=value;
	}else if(!mstrcmp("ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.yminData=value;
	}else if(!mstrcmp("ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ymaxData=value;
	}else if(!mstrcmp("drawzones",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pa.DrawZones=(int)value;
	}else if(!mstrcmp("areafill",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.AreaFill=(int)value;
	}else if(!mstrcmp("MaintainAspectRatio",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.MaintainAspectRatio=(int)value;
	}else if(!mstrcmp("useGradients",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pa.flagGradients=(int)value;	    
	}else if(!mstrcmp("pos",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    CurrentFrame=(long)value;
	   if(Get24BitData(Files,CurrentFrame))goto ErrorOut;
	}else if(!mstrcmp("datatype",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(pio->pioName,command,255);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pioIndex=(int)value;
	    pio->pd.AreaFill=1;
	}else if(!mstrcmp("filelist",command)){
	    if(FileManagerBatch(Batch,cp))goto ErrorOut;	
	}else if(!mstrcmp("datascale",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.sType=(int)value;
	    if(pio->pd.sType < 0 || pio->pd.sType > 2)pio->pd.sType=0;
	}else if(!mstrcmp("dmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.sPmin=value;
	}else if(!mstrcmp("dmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.sPmax=value;
	}else if(!mstrcmp("auto",command)){
	    if(doAuto(Files))goto ErrorOut;
	}else if(!mstrcmp("end",command)){
	    return 1;
	}
	
	
	return 0;
ErrorOut:
	return 1;
}
static int doAuto(struct FileInfo2 *Files)
{
	int flag;
	long n;
	int ret;
	
	if(!Files)return 1;
	
	ret=1;
	
	flag=0;
	for(n=0;n<Files->ImageCount;++n){	    
	    if(Get24BitData(Files,n))goto ErrorOut;
	    if(n >= (Files->ImageCount-1))flag=1;
	    if(Plot24BitData(Files,n,flag))goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}
static int Plot24BitData(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
    struct FilePIOInfo *pio;
	unsigned char *buff;
	    
	char filepath[2048];
	char name[256],*namef;
	long length;
	double *data;
	int ret;
	
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	
	//flag=flag;
	
	ret=1;
	data=NULL;
	
	length=(long)(pio->raster.box.xsize*pio->raster.box.ysize*3);
	
	buff=(unsigned char *)cMalloc(length,8765);
	if(!buff)goto ErrorOut;
	zerol((char *)buff,length);
	
		
	namef=FilesImageName(Files,CurrentFrame);
	if(namef){
		mstrncpy((char *)name,namef,255);	
	}else{
		sprintf(name,"image.%04ld%c",CurrentFrame,0);
	}
		    
	filepath[0]=0;
	mstrncpy(filepath,Files->outputDirectory,2048);
	
	if(Files->outputType == 0 || Files->outputType == TRUE_FILES){ 
	    unsigned char *data;
        
		data=(unsigned char *)buff;
		
		getTrueData(Files,data,CurrentFrame);
		
		strncatToPath(filepath,Files->outputName,2048);

		if(trueOut(filepath,name,pio->raster.box.xsize,pio->raster.box.ysize,data,CurrentFrame,flag))goto ErrorOut;
		
	}else if(Files->outputType == JPG_FILES){ 
	    unsigned char *data;
        
		data=(unsigned char *)buff;
		
		sprintf(name,"%s.%04ld.jpg%c",Files->outputName,CurrentFrame,0);
			
		strncatToPath(filepath,name,2048);
		
		getTrueData(Files,data,CurrentFrame);
		
		if(doJPG(filepath,data,(int)pio->raster.box.xsize,(int)pio->raster.box.ysize))goto ErrorOut;

	}
	
	ret = 0;
ErrorOut:
	if(buff)cFree((char *)buff);
	if(data)cFree((char *)data);
	return ret;
}
static int setSds2dScales(struct FilePIOInfo *pio)
{
	if(!pio)return 1;

    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
    if(!pio->range.yminSet)pio->range.yminData=pio->ymin;
    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->ymax;

    pio->range.xScale=((double)(pio->range.ixmaxData-pio->range.ixminData))/(pio->range.xmaxData-pio->range.xminData);
    pio->range.yScale=((double)(pio->range.iymaxData-pio->range.iyminData))/(pio->range.ymaxData-pio->range.yminData);

	return 0;
}
static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	double value;
	long i,j;
	double dx,dy;
	double dxhalf,dyhalf;
	double *data;
	
	double xmin,xmax,ymin,ymax,dmin,dmax;
	double didx,djdy;
	double dxdi,dydj;
	long ixmin,ixmax,iymin,iymax;

	double ymn,ymx,xmn,xmx;
	double xs,ys,vout;
	double xc,yc;
	long ii,jj,ihi,ilo,jhi,jlo;
	long n;
	        
	if(!Files)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(double *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	
	//CurrentFrame=CurrentFrame;
	
	setSds2dScales(pio);

	
	xmin=pio->range.xminData;
	xmax=pio->range.xmaxData;
	ymin=pio->range.yminData;
	ymax=pio->range.ymaxData;
	ixmin=pio->range.ixminData;
	ixmax=pio->range.ixmaxData;
	iymin=pio->range.iyminData;
	iymax=pio->range.iymaxData;

	didx=((double)(ixmax-ixmin))/(xmax-xmin);
	dxdi=1.0/didx;
	djdy=((double)(iymax-iymin))/(ymax-ymin);
	dydj=1.0/djdy;


	dmin  =1e60;
	dmax= -1e60;
	
    dx=(pio->xmax-pio->xmin)/(double)s->xsize;
    dxhalf=0.5*dx;
    dy=(pio->ymax-pio->ymin)/(double)s->ysize;
    dyhalf=0.5*dy;
		    		
	yc=pio->ymin-dyhalf;
	for(j=0;j<s->ysize;++j){
	    yc += dy;
		ymn=max(ymin,yc-dyhalf);
		ymx=min(ymax,yc+dyhalf);
		if(ymx < ymn)continue;
		jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
		jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
		
		xc=pio->xmin-dxhalf;			
	    for(i=0;i<s->xsize;++i){
	    	xc += dx;
			xmn=max(xmin,xc-dxhalf);
			xmx=min(xmax,xc+dxhalf);
			if(xmx < xmn)continue;
			
			ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
			ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);
		    
		    n=i+(s->ysize-1-j)*s->xsize;
			value=data[n];
			
	        if(dmin > value)dmin=value;
	        if(dmax < value)dmax=value;
							
			for(jj=jlo;jj<=jhi;++jj){
			    ys=ymin+(double)((double)(jj-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(ii=ilo;ii<=ihi;++ii){
			        xs=xmin+(double)((double)(ii-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        if(pio->doGradients){
						vout=value+(xs-xc)*pio->gradx[n]+
						  		   (ys-yc)*pio->grady[n];
						  		   
			        	if(dmin > vout)dmin=vout;
			        	if(dmax < vout)dmax=vout;
			        }else{
						vout=value;
			        }			        				        
					sout[ii+(pio->range.iymax-1-jj)*pio->range.ixmax]=vout;
			    }
			}				
	    }
	}


    pio->pd.dmin=dmin;
    pio->pd.dmax=dmax;

	dmin  =1e60;
	dmax= -1e60;
	
	for(j=0;j<s->ysize;++j){
	    for(i=0;i<s->xsize;++i){
		    n=i+(s->ysize-1-j)*s->xsize;
			value=data[n];
	        if(dmin > value)dmin=value;
	        if(dmax < value)dmax=value;
	    }
	}
	
    pio->cmin=dmin;
    pio->cmax=dmax;

	sds2dSetLimits(Files,CurrentFrame);
    	
	return 0;
}
static int sds2dSetLimits(struct FileInfo2 *Files,long nf)
{
    struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax,xdelta,ydelta;
	long xzones,yzones;
	struct LIMITS *limits;


	if(!Files)return 1;
	pio = &Files->pioData;

	//nf=nf;

	limits=&Files->limits;
	limits->iGot = FALSE;
	   
	xdelta=0.0;
	ydelta=0.0;
	xzones=pio->range.ixmaxData-pio->range.ixminData;
	if(xzones > 0)xdelta=(pio->range.xmaxData-pio->range.xminData)/(double)xzones;
	yzones=pio->range.iymaxData-pio->range.iyminData;
	if(yzones > 0)ydelta=(pio->range.ymaxData-pio->range.yminData)/(double)yzones;

	if(!xdelta || !ydelta)return 1;

	limits->pioIndex=Files->pioIndex;

	if(limits->pioName)cFree((char *)limits->pioName);
	limits->pioName=(unsigned char *)strsave(Files->pioName,5281);
	if(!limits->pioName)return 1;

	limits->iGot = TRUE;

	xmin=pio->range.xminData-xdelta*(double)(pio->range.ixminData);
	xmax=pio->range.xmaxData+xdelta*(double)(pio->range.ixmax-pio->range.ixmaxData);

	ymin=pio->range.yminData-ydelta*(double)(pio->range.iyminData);
	ymax=pio->range.ymaxData+ydelta*(double)(pio->range.iymax-pio->range.iymaxData);


	limits->xmin=xmin;
	limits->ymin=ymin;
	limits->xmax=xmax;
	limits->ymax=ymax;
	limits->zmin=0;
	limits->zmax=0;


	limits->vmin=pio->pd.dmin;

	limits->vmax=pio->pd.dmax;

	limits->dolog = (pio->pd.sType == 1);
	
	FilePutCommand(limits);
	
	return 0;
}
static int GetFrameInfo(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	
	if(!Files)return 1;
	pio = &Files->pioData;
		
	if(FilesSetFrame(CurrentFrame,Files))return 1;
	
	s = &Files->SetData;
	
	pio->raster.xsize=s->xsize;
	pio->raster.ysize=s->ysize;
	
	return 0;
}
static int Get24BitData(struct FileInfo2 *Files,long CurrentFrame)
{
	
	if(!Files)return 1;
		
	if(GetFrameInfo(Files,CurrentFrame))return 1;
				
	return 0;
}
static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy)
{

	if(!pio || !x || !y || !ix || !iy)return 1;

	*ix=(int)((*x-pio->range.xminData)*pio->range.xScale+(double)pio->range.ixminData+.5);
	*iy=(int)((*y-pio->range.yminData)*pio->range.yScale+(double)pio->range.iyminData+.5);

	return 0;
}
