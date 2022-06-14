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
#include "Universal.h"

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2

#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))

struct DrawInfo{
    struct FilePIOInfo *pio;
	struct areadata *ai;
	double dxset,dyset;
	double odxset,odyset;
    double rx,ry,sx,sy;
	double p1x,p1y,v1x,v1y,v2x,v2y,d,od;
	double l1,l2;
	double drHalf,dsHalf;
	double dr,ds;
	long nx,ny;
	double *sout;
};

int doCells(struct DrawInfo *info);

int convertTO24(struct SetFrameData  *sd,struct paletteDraw *pd);

int IsOption(void);

int WriteRectToRect(unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize);
                    
static int getPlotDataRaster(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame,struct SetFrameData *sd);
static int getPlotRaster(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame,struct SetFrameData *sd);
static int getPlotReal(struct FileInfo2 *Files,double *sout,long CurrentFrame,struct SetFrameData *sd);

int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);

static int GetFrameInfo(struct FileInfo2 *Files,long CurrentFrame);

static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct SetFrameData *sd);

static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int doScale2SetButtons(struct uDialogStruct *uList);
static int doDataSelection(struct FileInfo2 *Files);
static int GetRange3(struct uDialogStruct *uList);

int setFloat(double *d,long length);

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

static int doSDS2DDoDialog(struct FileInfo2 *Files,int *type);

int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

static int doSDS2DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int doSDS2D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);


static int doAuto(struct FileInfo2 *Files);

static int GetSDS2DData(struct FileInfo2 *Files,long CurrentFrame);

static int PlotSDS2DData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int sds2dGradients(struct FileInfo2 *Files,int flag);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int setSds2dScales(struct FilePIOInfo *pio);

static int sds2dSetLimits(struct FileInfo2 *Files,long nf);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int doSDS2DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSDS2DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int pioGetValue(struct FileInfo2 *Files,double x,double y,double *v,long *ne);
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);
static int doSDS2DGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int hdfToSDS2D(struct FileInfo2 *Files,struct LIMITS *limits,struct paletteDraw *pd);

static int doSDS2DPutData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int doSDS2DBatch(BatchPtr Batch,char *name)
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

    if(Files->fileClass != SDS2D_FILES && Files->fileClass != HDF_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"doSDS2DBatch Found (%s) instead of SDS2D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSDS2DBatch Can Only Process SDS2D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
	
    if(doSDS2DStart(Files))goto ErrorOut;
	           
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSDS2D(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}
static int doSDS2D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
    struct FilePIOInfo *pio;
    double xmin,xmax,ymin,ymax;
    long CurrentFrame;
	char *command;
 	double value;
 	int rangeNew;
    int flag;
 	
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
	
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	rangeNew = FALSE;
		
	xmin=pio->range.xminData;
	ymin=pio->range.yminData;
	xmax=pio->range.xmaxData;
	ymax=pio->range.ymaxData;
	
	if(!mstrcmp("sagedata",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(pio->pioName,command,255);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pioIndex=(int)value;
	}else if(!mstrcmp("xsize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixmax=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ysize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iymax=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("palette",command)){	
		 ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		getPaletteByName(command,pio->pd.palette);
	}else if(!mstrcmp("ixmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixminData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ixmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixmaxData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("iymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iyminData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("iymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iymaxData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.xminData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.xmaxData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.yminData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ymaxData=value;
		rangeNew = TRUE;
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
	   if(GetSDS2DData(Files,CurrentFrame))goto ErrorOut;
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
	
	if(rangeNew){
		flag=0;
		if(xmin != pio->range.xminData)flag  = 1;
		if(xmax != pio->range.xmaxData)flag += 2;
		if(ymin != pio->range.yminData)flag += 4;
		if(ymax != pio->range.ymaxData)flag += 8;		
		SetRange(Files,pio->range.xminData,pio->range.xmaxData,pio->range.yminData,pio->range.ymaxData,flagout[flag]);
	}
	
	return 0;
ErrorOut:
	return 1;
}
int doSDS2DStart(struct FileInfo2 *Files)
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
		
	Files->CurrentFrame = -1;
	
	Files->pGetData2=doSDS2DGetData;
	
	Files->pPutData2=doSDS2DPutData;
	
	Files->pGetAreaData=doSDS2DGetAreaData;
	
	Files->pGetLineData=doSDS2DGetLineData;
	
	Files->pInformation=doSDS2DInformation;
	
	Files->pDoDialog=doSDS2DDoDialog;

	pio->raster.xsize=Files->xmax;
	pio->raster.ysize=Files->ymax;
	
	pio->raster.xScale=1.0;
	pio->raster.yScale=1.0;
	
	pio->raster.box.x=0;
	pio->raster.box.y=0;
	
	pio->raster.box.xsize=(int)pio->raster.xsize;
	pio->raster.box.ysize=(int)pio->raster.ysize;
	
	
	
	pio->range.ixmax=Files->xmax;	
	pio->range.iymax=Files->ymax;	
	
	pio->range.ixminData=0;	
	pio->range.iyminData=0;	
	
	pio->range.ixmaxData=Files->xmax;	
	pio->range.iymaxData=Files->ymax;
			
	pio->range.xmaxData=(double)Files->xmax;	
	
	if(Files->iGotData & 1){
		pio->range.xminData=Files->xminr;
	}else{
		pio->range.xminData=0;
	}
	
		
	if(Files->iGotData & 2){
		pio->range.xmaxData=Files->xmaxr;
	}else{
		pio->range.xmaxData=(double)Files->xmax;
	}
	

	if(Files->iGotData & 4){
		pio->range.yminData=Files->yminr;
	}else{
		pio->range.yminData=0;
	}
	
	if(Files->iGotData & 8){
		pio->range.ymaxData=Files->ymaxr;
	}else{
		pio->range.ymaxData=(double)Files->ymax;
	}
			
	pio->range.xminSet=1;		
	pio->range.xmaxSet=1;	
		
	pio->range.yminSet=1;		
	pio->range.ymaxSet=1;
	
	pio->raster.sampleType = FRAME_SAMPLE_SIMPLE;
	pio->raster.plotType   = FRAME_DATA_FLOAT;
	pio->raster.boxType    = FRAME_BOX_SAMPLE;
	
	if(Files->fileClass == HDF_FILES)pio->raster.plotType=FRAME_DATA_RASTER;
		
	getPaletteByName("ps",pio->pd.palette);
		
	return 0;
}
int doDataScale2(struct FileInfo2 *Files);
static int doScale1Action(struct uDialogStruct *uList);
int doDataRange(struct FileInfo2 *Files);
int doVectorSelection(struct FileInfo2 *Files);
int doAreaSelection(struct FileInfo2 *Files);

static int doSDS2DDoDialog(struct FileInfo2 *Files,int *type)
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
	}else if(*type == MessageType_DO_AREA_DIALOG){
		return doAreaSelection(Files);	
	}
	return 0;
}
int doDataScale2(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {220,290,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {310,290,70,20},uDialogButton,uDialogNone},
      
       /*  2 */      {"Auto Scale - Linear",   {20,20,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  3 */      {"Manual Scale - Log",    {20,50,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  4 */      {"Manual Scale - Linear", {20,75,160,20},uDialogButtonRadio,uDialogGetLong},
       
       /*  5 */      {"",{275,50,105,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"",{275,75,105,20},uDialogText,uDialogGetDouble},
       
       /*  7 */      {"Range Min", {190,50,80,20},uDialogString,uDialogNone},
       /*  8 */      {"Range Max", {190,75,80,20},uDialogString,uDialogNone},
       
       /*  9 */      {"Labeled Palette", {20,105,120,20},uDialogCheck,uDialogGetLong},
       /* 10 */      {"Use Plot Range",  {170,105,120,20},uDialogCheck,uDialogGetLong},
       
       /* 11 */      {"Show Range", {275,20,100,20},uDialogButton,uDialogNone},
       
       /* 12 */      {"xmin-data", {20,245,85,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"xmax-data", {110,245,85,20},uDialogText,uDialogGetDouble},
       /* 14 */      {"ymin-data", {200,245,85,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"ymax-data", {290,245,85,20},uDialogText,uDialogGetDouble},
       
       /* 16 */      {"xmin", {35,220,80,20},uDialogString,uDialogNone},
       /* 17 */      {"xmax", {125,220,80,20},uDialogString,uDialogNone},
       /* 18 */      {"ymin", {215,220,80,20},uDialogString,uDialogNone},
       /* 19 */      {"ymax", {305,220,80,20},uDialogString,uDialogNone},
       /* 20 */      {"Use Plot Time", {170,125,120,20},uDialogCheck,uDialogGetLong},
       /* 21 */      {"Use Plot Domain",{170,145,125,20},uDialogCheck,uDialogGetLong},
       
       /* 22 */      {"Show Pixel Domain", {20,195,140,20},uDialogButton,uDialogNone},
       /* 23 */      {"Show Plot Domain", {170,195,140,20},uDialogButton,uDialogNone},
       /* 24 */      {"Use Gradients",{20,125,120,20},uDialogCheck,uDialogGetLong},
       /* 25 */      {"Reflect-x",{20,145,120,20},uDialogCheck,uDialogGetLong},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
 	
 	static uRect  Rect1   = {20,40,400,340};
	uPort save;
 	
	struct FilePIOInfo *pio;
	struct FilesSetList list;
	int sType;

 	uDialogList *uList;
	uDialogReturn *uR;
	
	int reti;
    int ret;
    
	if(!Files)return 1;
	pio=&Files->pioData;
	
	uGetPort(&save);
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Range",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	reti=uDialogSet(uList,0L,
	              
	              uDialogSetMyIcon,Files,
	              uDialogSetAction,doScale1Action,
	              
	              uDialogSetItem,2L,
		          uDialogSetLogical,(pio->pd.sType == 0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(pio->pd.sType == 1),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(pio->pd.sType == 2),
	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,pio->pd.sPmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,pio->pd.sPmax,
	              
	              uDialogSetItem,9L,
	              uDialogSetLogical,(pio->pd.LabeledPalette == 1),
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(pio->pd.UsePlotScales  == 1),
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,pio->xmin,

	              uDialogSetItem,13L,
	              uDialogSetDouble,pio->xmax,

	              uDialogSetItem,14L,
	              uDialogSetDouble,pio->ymin,

	              uDialogSetItem,15L,
	              uDialogSetDouble,pio->ymax,

	              uDialogSetItem,20L,
	              uDialogSetLogical,(pio->pd.UsePlotTime  == 1),

	              uDialogSetItem,21L,
	              uDialogSetLogical,(pio->pd.UsePlotRange  == 1),
	              
	              uDialogSetItem,24L,
	              uDialogSetLogical,(pio->pa.flagGradients  == 1),

	              uDialogSetItem,25L,
	              uDialogSetLogical,(Files->Reflect_x  == 1),

	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	

Start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	if(uR[2].lreturn){
	    sType=0;
	}else if(uR[3].lreturn){
	    sType=1;
	    if(uR[5].dreturn <= 0){
	        Warning("Min Must Be Greater Than Zero For Log Plots");
	        goto Start;
	    }
	}else{
	    sType=2;
	}
	if(uR[6].dreturn <= uR[5].dreturn){
	    Warning("Max Must Be Greater Than Min");
	    goto Start;
	}
	
	if(uR[12].dreturn >= uR[13].dreturn){
	    Warning("xmax Must Be Greater Than xmin");
	    goto Start;
	}
	
	if(uR[14].dreturn >= uR[15].dreturn){
	    Warning("ymax Must Be Greater Than ymin");
	    goto Start;
	}
	
    pio->pd.sType=sType;
	pio->pd.sPmin=  uR[5].dreturn;
	pio->pd.sPmax=  uR[6].dreturn;
	
	pio->xmin    =  uR[12].dreturn;
	pio->xmax    =  uR[13].dreturn;
	pio->ymin    =  uR[14].dreturn;
	pio->ymax    =  uR[15].dreturn;
	
	pio->pd.LabeledPalette=(int)uR[9].lreturn;
	pio->pd.UsePlotScales=(int)uR[10].lreturn;
	pio->pd.UsePlotTime=(int)uR[20].lreturn;
	pio->pd.UsePlotRange=(int)uR[21].lreturn;
	pio->pa.flagGradients= (int)uR[24].lreturn;
	list.Reflect_x=(int)uR[25].lreturn;
	FilesSetParameters(Files,&list,FILES_SET_REFLECT_X);
    
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	uSetPort(&save);
	
	return ret;
}
static int doScale1Action(struct uDialogStruct *uList)
{
	struct SetFrameData  sd;
	struct FileInfo2 *Files;
 	double dmin,dmax;
	long n,nf,dataLength;
	double *fdata;
	uDialogReturn *uR;
	struct FilesSetList list;
	int ret;
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	
	
	uSetCursor(NULL,uWATCH_CURSOR);
	
	uR=uDialogGetReturn(uList);
    sd.data=NULL;  
	if(uList->actionItem == 11){
	    if(!Files)goto OutOfHere;
	
	    dmin =  1e60;
	    dmax = -1e60;
	    
		for(nf=0;nf<Files->ImageCount;++nf){
	    
		
		    zerol((char *)&sd,sizeof(struct SetFrameData));
				
		    sd.type=FRAME_DATA_FLOAT;
		    
		    if((*Files->pGetData2)(Files,nf,&sd)){
				goto OutOfHere;
		    }
		    
		    fdata=(double *)sd.data;
		    dataLength=sd.ysize*sd.xsize;
		

			for(n=0;n<dataLength;++n){
		    	if(fdata[n] > dmax)dmax=fdata[n];
		    	if(fdata[n] < dmin)dmin=fdata[n];
			}

		    if(sd.data)cFree((char *)sd.data); 
		    sd.data=NULL; 
		     
			if(Stop("Stop Data Ranging ?"))break;
	   }
	   
		ret=uDialogSet(uList,5L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,dmax,
	              	              	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,2L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(1),
	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
	
	}else if(uList->actionItem == 22){
	    double xmin,xmax,ymin,ymax;
	    
	    xmin=0;
	    ymin=0;
	    xmax=(double)Files->xmax;
	    ymax=(double)Files->ymax;
	    
	    if(xmax <= 0 || ymax <= 0)goto OutOfHere;
	
		ret=uDialogSet(uList,12L,
	              uDialogSetDouble,xmin,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,xmax,
	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,ymin,
	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,ymax,
	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
	}else if(uList->actionItem == 23){
	    double xmin,xmax,ymin,ymax;
		struct LIMITS *limits;
		long CurrentFrame;
		
    	CurrentFrame=Files->CurrentFrame;
    	if(CurrentFrame < 0)CurrentFrame=0;
    	
    	if(uR){
			list.Reflect_x=(int)uR[25].lreturn;
			FilesSetParameters(Files,&list,FILES_SET_REFLECT_X);
    	}

    	
		if(FilesSetFrame(CurrentFrame,Files))goto OutOfHere;
		
		limits=NULL;
		FileGetLimits(&limits,CurrentFrame,Files);
		if(!limits || !limits->iGot)goto OutOfHere;

	    xmin=limits->xmin;
	    ymin=limits->ymin;
	    xmax=limits->xmax;
	    ymax=limits->ymax;
	    if(Files->Reflect_x)xmin = -xmax;
	    	
		ret=uDialogSet(uList,12L,
	              uDialogSetDouble,xmin,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,xmax,
	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,ymin,
	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,ymax,
	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
	}
OutOfHere:	
	if(uList)uDialogUpdate(uList);
	if(sd.data)cFree((char *)sd.data); 
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(NULL,uARROW_CURSOR);
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

 	uRect *rectOpen;
 	
 	uDialogList *uList;
    int ret;
    
	if(!Files)return 1;

	if(uPref.dataSelectRect.xsize > 100 && uPref.dataSelectRect.ysize > 100){
		rectOpen=&uPref.dataSelectRect;
	}else{
		rectOpen=&Rect1;
	}
	
	ret=1;
	
	uList=uDialogOpen("Data Selection",rectOpen,ItemCount);
	
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
	
	if(uList && uList->myIcon){
        uRect portRect;
		uGetPortBoundsGlobal(uList->myIcon,&portRect);
		uPref.dataSelectRect=portRect;
	}
	
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

			if(Stop("Stop Data Ranging ?"))break;
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
	
	if(!IsOption()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
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

static int doSDS2DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	
	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	
	sd->rangeData=pio->rangeData;
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
static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
    struct FilePIOInfo *pio;
	long ne;
	long n;

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
		
	if(li->type != LINEDATA_LIST_DATA)return 1;
	
	if(!li->x || !li->y || !li->v || (li->PointLineCount < 1))return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	pio->pioIndex=(int)li->pioIndex;
		
	if(GetSDS2DData(Files,li->CurrentFrame))goto OutOfHere;
		
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

    setSds2dScales(pio);
    
	for(n=0;n<li->PointLineCount;++n){
	    		    
	    if(!pioGetValue(Files,li->x[n],li->y[n],&li->v[n],&ne)){
	        li->v[n]=0;
	    }	    
	}

	
	return 0;
OutOfHere:

	return 1;
}
static int doIntegrateLine(struct FileInfo2 *Files,struct linedata *li)
{
	struct FilePIOInfo *pio;
	struct SetFrameData  *ss;
	struct PIO *sage;
	double *xD,*yD,s,s1,s2,s3,s4;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	double *values;
	int flagGradients;
	double rx,ry;
	int ibreak;
	double odxset,odyset;
	double dx2,dy2;
	
	if(!li)return 1;
	if(!Files)return 1;
	
	pio=&Files->pioData;
	ss = &Files->SetData;
	values=(double *)ss->data;
	if(!values)return 1;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA || ! li->integrate)return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;

	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	y1=li->y1;
	y2=li->y2;
	ry=y2-y1;

	flagGradients=pio->pa.flagGradients;

	if(GetSDS2DData(Files,li->CurrentFrame))goto OutOfHere;
	
	pio->pa.flagGradients = FALSE;

    setSds2dScales(pio);
	
	sage=pio->sage;
	
	np=10000;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81879))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(np),81883))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}	
	
	x=x1;
	y=y1;
	
	length=0;
	integral=0;

	dx=0;
	dy=0;
	
	s=0;
	
	nd=0;
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;	
	
	odxset=(double)(ss->xsize)/(pio->xmax-pio->xmin);
	dx2=1.0/odxset;

	odyset=(double)(ss->ysize)/(pio->ymax-pio->ymin);
	dy2=1.0/odyset;

	while(1){
	    double value,xc,yc,dl,yy,xx;
	    long i,j;
	    
	    xx=x+dx;
	    if(xx < pio->xmin || xx > pio->xmax)break;
	    
	    yy=y+dy;
	    if(yy < pio->ymin || yy > pio->ymax)break;
	    
		i=(long)((xx-pio->xmin)*odxset);	
		if(i < 0){
		    i=0;
		}else if(i >= ss->xsize){
		    i=ss->xsize-1;
		}
	    
		j=(long)((yy-pio->ymin)*odyset);
		if(j < 0){
		    j=0;
		}else if(j >= ss->ysize){
		    j=ss->ysize-1;
		}
		
		
	    ne=i+(ss->ysize-1-j)*ss->xsize;
	    if(ne < 0 || ne >= ss->xsize*ss->ysize){
			break;
	    }
		
		value=values[ne];
        xc=((double)i+0.5)*dx2+pio->xmin;
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
        yc=((double)j+0.5)*dy2+pio->ymin;
	    ymin=yc-dy2*1.0000001;
	    ymax=yc+dy2*1.0000001;
 	    
	  	s=-1;
	    if(rx){
	        s1=((xc-dx2)-x1)/rx;
	        yy=y1+s1*ry;
	        if(yy < ymin || yy > ymax){
	            ;
	        }else{
	            s=s1;
	        }	        
	        s2=((xc+dx2)-x1)/rx;
	        if(s2 > s){	        
	        	yy=y1+s2*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            s=s2;
		        }	        
	        }
	        
		    if(ry){
		        s3=((yc-dy2)-y1)/ry;
	            if(s3 > s){
	                xx=x1+s3*rx;	                
			        if(xx < xmin || xx > xmax){
			            ;
			        }else{
			            s=s3;
			        }	        
	            }
		        s4=((yc+dy2)-y1)/ry;
	            if(s4 > s){
	                xx=x1+s4*rx;	                
			        if(xx < xmin || xx > xmax){
			            ;
			        }else{
			            s=s4;
			        }	        
	            }
		    }
	    }else{
		    if(ry){
		        s1=((yc-dy2)-y1)/ry;
                xx=x1+s1*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            s=s1;
		        }
		        s4=((yc+dy2)-y1)/ry;
	            if(s4 > s){
	                xx=x1+s4*rx;	                
			        if(xx < xmin || xx > xmax){
			            ;
			        }else{
			            s=s4;
			        }	        
	            }
		    }else{
		        sprintf(WarningBuff,"doIntegrateLine : Error Cell %ld No intersection\n",ne);
	    		WarningBatch(WarningBuff);
	    		goto OutOfHere;
		    }
	    }
	    
	    if(s < 0){
	        sprintf(WarningBuff,"doIntegrateLine : Error Cell %ld No length\n",ne);
    		WarningBatch(WarningBuff);
    		goto OutOfHere;
	    }
	    
		if(s >= 1.0){
		   s=1.0;
		   ibreak=TRUE;
		}else{
		   ibreak=FALSE;
		}
		
		if(nd+4 > np){
		    double *xdn,*ydn;
		    		    	    
		   	xdn=(double *)cRealloc((char *)xD,2*np*sizeof(double),3761);
		   	ydn=(double *)cRealloc((char *)yD,2*np*sizeof(double),3762);
		   	if(!xdn || !ydn || (nd > 1000000)){
	        	sprintf(WarningBuff,"doIntegrateLine : Error Too many Points %ld\n",nd);
    			WarningBatch(WarningBuff);
    			if(xdn){
    			    cFree((char *)xdn);
    				xD=NULL;
    			}
    			if(ydn){
    				cFree((char *)ydn);
    				yD=NULL;
    			}
    			goto OutOfHere;
    		}
    		xD=xdn;
    		yD=ydn;
    		np=2*np;
		}

	    xD[nd]=length;
	    yD[nd++]=value;

	    if(length < rxmin)rxmin=length;
	    if(length > rxmax)rxmax=length;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;

	    xo=x1+s*rx;
	    yo=y1+s*ry;	
		
		dl=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y));
		
		integral += value*dl;
		
		length += dl;
    
	    xD[nd]=length;
	    yD[nd++]=value;
	    
	    if(ibreak)break;
	    
	    x=xo;
	    y=yo;
	    dx=1e-6*rx;
	    dy=1e-6*ry;
	}
	
	
	
	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	li->integral=integral;

	pio->pa.flagGradients=flagGradients;
	
	if(length > 0){
	    double ll;
	    ll=sqrt(rx*rx+ry*ry);
		if(fabs(length-ll)/length > 1e-5){
		     sprintf(WarningBuff,"doIntegrateLine Line Length %g Not Equal To Integral Length %g\n",ll,length);
		     WarningBatch(WarningBuff);
		}
	}
	

	return 0;
OutOfHere:
	pio->pa.flagGradients=flagGradients;
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}
static int doSDS2DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	int ix1,iy1,ix2,iy2;
	double x1,y1,x2,y2,dl,ds,dx,dy,dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	long ne;
	
	if(!li)return 1;
	if(li->type == LINEDATA_POINT_DATA){
	    return doPointGetData(Files,li);
	}else if(li->type == LINEDATA_LIST_DATA){
	     return doPointGetListData(Files,li);
	}
	if(!Files)return 1;
	pio=&Files->pioData;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA)return 1;
	

	if(li->integrate)return doIntegrateLine(Files,li);

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
		
	if(GetSDS2DData(Files,CurrentFrame))goto OutOfHere;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

    setSds2dScales(pio);
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81873))){
	    WarningBatch("doSDS2DGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("doSDS2DGetLineData Out of List Memory\n");
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

	    if(!pioGetValue(Files,x,y,&value,&ne))continue;

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
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt;
	double valuet;
	double r,theta,dr,dtheta;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	double xs,ys;
	double x,y;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
    double rMax,thetaMax;
	long ne;

	
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

	
	if(li->SampleSteps < 1)li->SampleSteps=1;
	
	NumberOfRings=li->SampleSteps;
	
	dr=li->SampleRadius/(double)NumberOfRings;
	
	NumberOfPoints=li->SampleSteps;
	
	dtheta=8*atan(1.0)/((double)NumberOfPoints);

	xs=x;
	ys=y;
	nd=0;
	rMax=0;
	thetaMax=0;

	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    if(GetSDS2DData(Files,CurrentFrame))goto OutOfHere;
	    
    	setSds2dScales(pio);
		    
    	x=xs;
    	y=ys;
    	nf=0;
    	r=dr;
    	theta=0;
    	valuet=0;
    	for(nr=0;nr<NumberOfRings;++nr){
    	    theta=0;
    	    for(nt=0;nt<NumberOfPoints;++nt){
		    	if(!pioGetValue(Files,x,y,&value,&ne))continue;
		    
		    	valuet += value;
		    	++nf;
		    	
		    	x=xs+r*cos(theta);
		    	y=ys+r*sin(theta);
		    	/*
		    	sprintf(WarningBuff,"x %g y %g r %g theta %g",x,y,r,theta*180/3.1415925);
		    	WarningBatchFast(WarningBuff);
		    	*/
		    	theta += dtheta;
		    	if(r > rMax)rMax=r;
		    	if(theta > thetaMax)thetaMax=theta;
		    	
	    	}
		   	r += dr;	    	
	    }
	    
	    if(nf <= 0)continue;
	    
	    value=valuet/(double)nf;
	    
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
	/*
			WarningBatchAtEnd(WarningBuff);        
    */
	return 0;
OutOfHere:
    if(xD)cFree((char *)xD);
    if(yD)cFree((char *)yD);
	return 1;
}
static int pioGetValue(struct FileInfo2 *Files,double x,double y,double *v,long *nee)
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
	if(!nee)return 1;
		
	
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
    
    *nee=ne;
        
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

static int getPlotDataGeneral(struct DrawInfo *info,struct FileInfo2 *Files)
{
	struct SetFrameData  *ss;
	struct FilePIOInfo *pio;
	double value;
	long i,j;
	double dx,dy;
	double dxhalf,dyhalf;
	double *data;
	
	double dmin,dmax;

	double ymn,ymx,xmn,xmx;
	double xc,yc;
	long n;
	double *sout;
	double r1,r2,s1,s2,dr,ds;
	double x,y,r,s,rx,ry,sx,sy;
	long nx,ny;
	long i1,i2,j1,j2,ii,jj;
	
	        
	if(!info)return 1;
	pio=info->pio;
	if(!pio)return 1;
	sout=info->sout;
	if(!sout)return 1;
	
	ss = &Files->SetData;
	data=(double *)ss->data;
	if(!data)return 1;
			
	setSds2dScales(pio);
	
	
	rx=info->rx;
	ry=info->ry;
	sx=info->sx;
	sy=info->sy;
		
	nx=info->nx;
	ny=info->ny;
	
	dr=info->dr;
	ds=info->ds;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}


	dmin  =1e60;
	dmax= -1e60;
	
    dx=(pio->xmax-pio->xmin)/(double)ss->xsize;
    dxhalf=0.5*dx;
    dy=(pio->ymax-pio->ymin)/(double)ss->ysize;
    dyhalf=0.5*dy;
		    		
	yc=pio->ymin-dyhalf;
	for(j=0;j<ss->ysize;++j){
	    yc += dy;
		ymn=yc-dyhalf;
		ymx=yc+dyhalf;
				
		
		xc=pio->xmin-dxhalf;			
	    for(i=0;i<ss->xsize;++i){
	    	xc += dx;
			xmn=xc-dxhalf;
			xmx=xc+dxhalf;
						
    		x=xc-dxhalf-info->p1x;
    		y=yc-dyhalf-info->p1y;
			
			
		    r1 = 10;
		    r2 = -1;
		    
		    r=rx*x+ry*y;                
		    if(r < r1)r1 = r;
		    if(r > r2)r2 = r;
		    
		    x += dx;
		    r=rx*x+ry*y;                
		    if(r < r1)r1 = r;
		    if(r > r2)r2 = r;
		    
		    y += dy;
		    r=rx*x+ry*y;                
		    if(r < r1)r1 = r;
		    if(r > r2)r2 = r;
		    
		    x -= dx;
		    r=rx*x+ry*y;                
		    if(r < r1)r1 = r;
		    if(r > r2)r2 = r;
		    
		    if(r1 > 1.0 || r2 < 0.0)continue;
			
		    s1 = 10;
		    s2 = -1;
		    
		    y -= dy;
		    
		    s=sx*x+sy*y;                
		    if(s < s1)s1 = s;
		    if(s > s2)s2 = s;
		    
		    x += dx;
		    s=sx*x+sy*y;                
		    if(s < s1)s1 = s;
		    if(s > s2)s2 = s;
		    
		    y += dy;
		    s=sx*x+sy*y;                
		    if(s < s1)s1 = s;
		    if(s > s2)s2 = s;
		    
		    x -= dx;
		    s=sx*x+sy*y;                
		    if(s < s1)s1 = s;
		    if(s > s2)s2 = s;
		    
		    if(s1 > 1.0 || s2 < 0.0)continue;
		    
			i1=(int)(r1*(double)nx);
			if(i1 < 0)i1 = 0;
			if(i1 >= nx)i1 = nx - 1;

			i2=(int)(r2*(double)nx);
			if(i2 < 0)i2 = 0;
			if(i2 >= nx)i2 = nx - 1;


			j1=(int)(s1*(double)ny);
			if(j1 < 0)j1 = 0;
			if(j1 >= ny)j1 = ny - 1;

			j2=(int)(s2*(double)ny);
			if(j2 < 0)j2 = 0;
			if(j2 >= ny)j2 = ny - 1;
				 		    
		    
		    n=i+(ss->ysize-1-j)*ss->xsize;
			value=data[n];
			
	        if(dmin > value)dmin=value;
	        if(dmax < value)dmax=value;			
			
			for(jj=j1;jj<=j2;++jj){
			    double v;
			    s=((double)jj+0.5)*ds;
			    for(ii=i1;ii<=i2;++ii){
			        r=((double)ii+0.5)*dr;
			        x=r*info->v1x+s*info->v2x+info->p1x;
			        if(x < xmn || x > xmx)continue;
			        y=r*info->v1y+s*info->v2y+info->p1y;
			        if(y < ymn || y > ymx)continue;
			        
			        
			    	if(pio->doGradients){
						v=value+(x-xc)*pio->gradx[n]+
						  		(y-yc)*pio->grady[n];
					}else{
						v=value;
					}
					sout[ii+(ny-1-jj)*nx]=v;
			    }
			}			
						
	    }
	}

    	
	return 0;
}
static int doSDS2DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct DrawInfo info;
		
	
	if(!ai || !ai->data || !Files)return 1;
	pio=&Files->pioData;
	
	zerol((char *)&info,sizeof(struct DrawInfo));

	info.pio=pio;
	info.ai=ai;
	
	info.sout=ai->data;
		
	info.nx=ai->xsize;
	info.ny=ai->ysize;
	
	info.dr = 1.0/(double)ai->xsize;
	info.drHalf = 0.5*info.dr;
	info.ds = 1.0/(double)ai->ysize;
	info.dsHalf = 0.5*info.ds;
	
	info.p1x=ai->x1;
	info.p1y=ai->y1;
	
	info.v1x=ai->x2-ai->x1;
	info.v1y=ai->y2-ai->y1;
	
	info.l1=sqrt(info.v1x*info.v1x+info.v1y*info.v1y);
	
	info.v2x=ai->x3-ai->x1;
	info.v2y=ai->y3-ai->y1;
	
	info.l2=sqrt(info.v2x*info.v2x+info.v2y*info.v2y);
	
	info.d=info.v1x*info.v2y-info.v1y*info.v2x;
	
	if(info.l1 <= 0 || info.l2 <= 0 || info.d == 0.0){
	    sprintf(WarningBuff,"doSDS2DGetGeneratData - Error l1 %g l2 %g d %g\n",info.l1,info.l2,info.d);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	info.od = 1.0/info.d;
	
	info.rx =  info.v2y*info.od;
	info.ry = -info.v2x*info.od;
	
	info.sx = -info.v1y*info.od;
	info.sy =  info.v1x*info.od;
	
	return getPlotDataGeneral(&info,Files);
}
static int doSDS2DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	long xsize,ysize;
	double *data;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;

	if(ai->type != GENERAL_PLANE_DATA && ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"doSDS2DGetAreaData - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	CurrentFrame=ai->CurrentFrame;
	
	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}

	pio->pd.AreaFill=TRUE;
	
	if(GetSDS2DData(Files,CurrentFrame))return 1;
		
	ai->pioTime=Files->pioTime;
	
	xsize=ai->xsize;
	ysize=ai->ysize;
			
	if(ai->xsize <= 1){	
		sprintf(WarningBuff,"doSDS2DGetAreaData - xsize less that 2\n");
	    WarningBatch(WarningBuff);
	    return 1;		
	}else if(ai->ysize <= 1){	
		sprintf(WarningBuff,"doSDS2DGetAreaData - ysize less that 2\n");
	    WarningBatch(WarningBuff);
	    return 1;		
	}
	
	data=(double *)cMalloc(xsize*ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,xsize*ysize*sizeof(double));
	
	ai->data=data;
	 	
	if(ai->type == GENERAL_PLANE_DATA){
	    return doSDS2DGetGeneratData(ai,Files);
	}
		
	pio->range.xminData=ai->xminArea;
	pio->range.xmaxData=ai->xmaxArea;
	pio->range.yminData=ai->yminArea;
	pio->range.ymaxData=ai->ymaxArea;
	
	pio->range.ixmax=xsize;
	pio->range.iymax=ysize;
	
	pio->range.ixminData=0;
	pio->range.iyminData=0;
	
	pio->range.ixmaxData=xsize;
	pio->range.iymaxData=ysize;
		
	
    if(getPlotData(Files,data,CurrentFrame)){
	    sprintf(WarningBuff,"doSDS2DGetAreaData - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        return 1;
    }	    
	

	    
	return 0;
}
static int doSDS2DPutData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	int ret;

	if(!Files || !sd)return 1;
	
	ret=1;

	if(sd->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"doSDS2DPutData - Only Puts FRAME_DATA_FLOAT");
	    Warning(WarningBuff);
	    goto ErrorOut;
	}

	ret=FileAppendSDS2D(sd->pioName,(int)sd->xsize,(int)sd->ysize,(double *)sd->data,CurrentFrame,&Files);
	if(ret)goto ErrorOut;
	
	ret=0;
ErrorOut:
	return ret;
}
static int doSDS2DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
    struct PlotRasterData *raster;
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    double *sout;
    int type;
    int not1;

	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	limits=&Files->limits;

	type=sd->type;
	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           if(Files->fileClass == SDS2D_FILES)sd->type=FRAME_DATA_FLOAT;
	           if(Files->fileClass == HDF_FILES)sd->type=FRAME_DATA_FLOAT;
	    }else if(sd->type != FRAME_DATA_RASTER){
		    sprintf(WarningBuff,"doSDS2DGetData - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
		    WarningBatch(WarningBuff);
	    	return 1;
	    }else{
	        if(Files->fileClass == SDS2D_FILES)return 1;
	    }
	}

	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;		
		pio->pd     = sd->pd;		
	}
		
	pio->pd.AreaFill=TRUE;
	
	
    if(sd->typePlotRange == FRAME_RANGE_RASTER){
		raster  = &sd->raster;		
		pio->pd = sd->pd;	
		pio->raster=sd->raster;			
	}else{
		raster  = &pio->raster;		
	}
	
	 if(GetFrameInfo(Files,CurrentFrame)){
	     goto TryAgain;
	 }
	
    {
        double dxdr,dydr;
        
		
		pio->range.ixmax=raster->box.xsize;	
		pio->range.iymax=raster->box.ysize;	
		
		pio->range.ixminData=0;	
		pio->range.iyminData=0;	
		
		pio->range.ixmaxData=raster->box.xsize;	
		pio->range.iymaxData=raster->box.ysize;
		
		raster->xsize=pio->rangeData.xsize;
		
		dxdr=(pio->xmax-pio->xmin)/((double)raster->xsize*raster->xScale);
		dydr=(pio->ymax-pio->ymin)/((double)raster->ysize*raster->yScale);
		
		pio->range.xminData=raster->box.x*dxdr+pio->xmin;
		pio->range.xmaxData=pio->range.xminData+raster->box.xsize*dxdr;	
		
			
		pio->range.ymaxData=pio->ymax-raster->box.y*dydr;	
		pio->range.yminData=pio->range.ymaxData-raster->box.ysize*dydr;		
		
		pio->range.xminSet=1;		
		pio->range.xmaxSet=1;	
			
		pio->range.yminSet=1;		
		pio->range.ymaxSet=1;
TryAgain:		
		not1 = FALSE;
		if(pio->range.ixmax <= 0 || pio->range.iymax <= 0){
			if(Files->xSize <= 0 || Files->ySize <= 0)return 1;
			pio->range.ixmax=Files->xSize;
			pio->range.iymax=Files->ySize;
			not1 = TRUE;
		}
		
	}
        		
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9216);
    if(!sd->data){
    	sprintf(WarningBuff,"doSDS2DGetData - Out of Memory\n");
    	WarningBatch(WarningBuff);
    	return 1;
    }
    sout=(double *)sd->data;
    
    if(not1){
        zerol((char *)sout,length);
        return 0;
    }   
    
    if(sd->type == FRAME_DATA_FLOAT || (type == FRAME_DATA_RGB && Files->fileClass == SDS2D_FILES)){
		if(GetSDS2DData(Files,CurrentFrame)){
			return 0;
		}
		setFloat(sout,length);		
		if(!pio->pa.flagGradients && sd->raster.xScale && sd->raster.yScale){
	    	if(getPlotReal(Files,sout,CurrentFrame,sd)){
			    sprintf(WarningBuff,"doSDS2DGetData - getPlotReal Error\n");
			    WarningBatch(WarningBuff);
				goto ErrorOut;
		    }	
		}else{
		    if(getPlotData(Files,sout,CurrentFrame)){
			    sprintf(WarningBuff,"doSDS2DGetData - getPlotData Error\n");
			    WarningBatch(WarningBuff);
				goto ErrorOut;
		    }	
	    }	    
 	    if(type == FRAME_DATA_RGB){
			sd->pd.dmin=pio->cmin;
			sd->pd.dmax=pio->cmax;
			FrameDataToRaster(sd,(unsigned char *)sout);
			convertTO24(sd,&pio->pd);
	    }
    }else {
        zerol((char *)sout,length);
	    if(getPlotImage(Files,(unsigned char *)sout,CurrentFrame,sd)){
		    sprintf(WarningBuff,"doSDS2DGetData - getPlotImage Error\n");
		    WarningBatch(WarningBuff);
			goto ErrorOut;
	    }	
 	    if(sd->type == FRAME_DATA_RGB){
	         convertTO24(sd,&pio->pd);
	    }
   }
        
	sd->pd=pio->pd;
	sd->pd.dmin=pio->cmin;
	sd->pd.dmax=pio->cmax;
    sd->limits = *limits;	
    
ErrorOut:

	return 0;

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
	    if(GetSDS2DData(Files,n))goto ErrorOut;
	    if(n >= (Files->ImageCount-1))flag=1;
	    if(PlotSDS2DData(Files,n,flag))goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}
static int PlotSDS2DData(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;
	/*
	struct screenData ss;
	*/
	unsigned char *buff;
	    
	char filepath[2048];
	char name[256],*namef;
	long length;
	double *data;
	int ret;
	
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	ret=1;
	data=NULL;
	
	length=(long)(pio->range.ixmax*pio->range.iymax*sizeof(double));
	
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
	strncatToPath(filepath,Files->outputName,2048);

	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}
	
	if(Files->outputType == 0 || Files->outputType == HDF_FILES){ 
		unsigned char *bp;
		long length;
		double dat,pmin,pmax;
	    double *data,a,b;
	    long nn;
        
		data=(double *)buff;
		bp=(unsigned char *)buff;
		
		getPlotData(Files,data,CurrentFrame);
		
		length=pio->range.ixmax*pio->range.iymax;
		
	    pmin=pio->pd.sPmin;
	    pmax=pio->pd.sPmax;
		    
		if(pio->pd.sType == 0){		    
		    pmin =  data[0];
		    pmax = pmin;
		    for(nn=0;nn<length;++nn){
		        if(data[nn] < pmin){
		            pmin=data[nn];
		        }else if(data[nn] > pmax){
		            pmax=data[nn];
		        }
		    }
		    if(pmin >= pmax){
		       pmin = pmax - 1.0f;
		       ++pmax;
		    }
		    pio->pd.dmin=pmin;
		    pio->pd.dmax=pmax;
		    a=((254.0-2.0)/(pmax-pmin));
		    b=(2.0-a*pmin);
		    for(nn=0;nn<length;++nn){
				bp[nn]=(unsigned char)(a*data[nn]+b);
		    }
		        
		}else if(pio->pd.sType == 1){
		    a=((254.0-2.0)/(log10(pmax)-log10(pmin)));
		    b=(2.0-a*log10(pmin));
		    for(nn=0;nn<length;++nn){
			    dat=data[nn];
			    if(dat < pmin)dat=pmin;
			    if(dat > pmax)dat=pmax;
			    dat=log10(dat);
	            bp[nn]=(unsigned char)(a*dat+b);
		    }
		}else if(pio->pd.sType == 2){
		    a=((254.0-2.0)/(pmax-pmin));
		    b=(2.0-a*pmin);
		    
		    for(nn=0;nn<length;++nn){
				dat=data[nn];
				if(dat < pmin){
				    dat=pmin;
				}else if(dat > pmax){
				    dat=pmax;
				}
			    *bp++ = (unsigned char)(a*dat+b);
		    }
		}		
		
	    limits = &Files->limits;
	    if(!limits->iGot){
	        limits=NULL;
	    }else{
			limits->vmin=pmin;
			limits->vmax=pmax;
			limits->dolog = (pio->pd.sType == 1);
	    }
	    
		if(hdfOut(filepath,name,(char *)pio->pd.palette,pio->range.ixmax,pio->range.iymax,(char *)buff,CurrentFrame,flag,limits))goto ErrorOut;

	}else if(Files->outputType == SDS2D_FILES){
        double *sout;
						
		sout=(double *)buff;

		getPlotData(Files,sout,CurrentFrame);
    		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	        
	    if(sds2DOut(filepath,name,pio->range.ixmax,pio->range.iymax,sout,CurrentFrame,flag,limits))goto ErrorOut;
	    	   	    
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
static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct SetFrameData *sd)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	unsigned char *buffFloat;
	unsigned char *in;
	
	if(!Files || !buff || !sd)return 1;	
	pio = &Files->pioData;
	s = &Files->SetData;
	
	if(GetFrameInfo(Files,CurrentFrame))return 1;
	
	if(!s->data)return 1;
	
	buffFloat=NULL;
			
	if(Files->fileClass == SDS2D_FILES){
	    buffFloat=(unsigned char *)cMalloc(s->xsize*s->ysize,18236);
	    if(!buffFloat)return 1;	    
		sd->pd.dmin=pio->cmin;
		sd->pd.dmax=pio->cmax;	
	    s->pd=sd->pd;
		FrameDataToRaster(s,buffFloat);
		in=buffFloat;
	}else{
		in=(unsigned char *)s->data;
	}	
			
    s->data=in;

	getPlotDataRaster(Files,buff,CurrentFrame,sd);

	if(buffFloat)cFree((char *)buffFloat);
			
	return 0;
}
static int getPlotRaster(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame,struct SetFrameData *sd)
{
	struct SetFrameData  *s;
	unsigned char *data;
	long i,j;
	long x,y,xsize,ysize,xinc,iyinc,iystep;
	double xscale,yscale,oxscale,oyscale;
	int onetoOne;
	register unsigned char *bp,*dp;
	register double dx;
	
	if(!Files)return 1;
	s = &Files->SetData;
	data=(unsigned char *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	if(!sd)return 1;
	//CurrentFrame=CurrentFrame;
	
	onetoOne=TRUE;
	if((sd->raster.xScale != 1.0) || (sd->raster.yScale != 1.0))onetoOne=FALSE;	
		
	xscale=sd->raster.xScale*(double)s->xsize;
	oxscale=1.0/sd->raster.xScale;
	yscale=sd->raster.yScale*(double)s->ysize;
	oyscale=1.0/sd->raster.yScale;
	
	x=sd->raster.box.x;
	xsize=sd->raster.box.xsize;
	xinc=sd->xsize;
	if((double)(x+xsize) > xscale){
	    xsize=(long)xscale-x;
	}
	
	y=sd->raster.box.y;
	ysize=sd->raster.box.ysize;
	if((double)(y+ysize) > yscale){
	    ysize=(long)yscale-y;
	}
	
	if(onetoOne){	
		for(j=0;j<ysize;++j){
		    bp=sout+j*xinc;
		    dp=data+x+s->xsize*(y+j);		    
		    for(i=0;i<xsize;++i){
		    	*bp++ = *dp++;
		    }
		}	
	}else{
	    for(j=0;j<ysize;++j){
	        iystep=s->xsize*((long)((double)(y+j)*oyscale));
	        iyinc=j*xinc;
	        bp=sout+iyinc;
	        dx = (double)(x)*oxscale+(double)iystep;
	        for(i=0;i<xsize;++i){
	            *bp++ = *(data+((long)dx));
	            dx += oxscale;
	        }
	    }
	}
	
	return 0;
}
static int getPlotDataRaster(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame,struct SetFrameData *sd)
{
	struct FilePIOInfo *pio;
	        
	if(!Files)return 1;
	pio = &Files->pioData;
	if(!sout)return 1;
	if(!sd)return 1;
	
	
	setSds2dScales(pio);
	
	return getPlotRaster(Files,sout,CurrentFrame,sd);
	
}


static int getPlotReal(struct FileInfo2 *Files,double *sout,long CurrentFrame,struct SetFrameData *sd)
{
	long x,y,xsize,ysize,xinc,iyinc,iystep;
	double xscale,yscale,oxscale,oyscale;
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	register double *bp,*dp;
	register double dx;
	double dmin,dmax;
	int onetoOne;
	double *data,value;
	long i,j,n;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(double *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	if(!sd)return 1;
	
	setSds2dScales(pio);
	
	//CurrentFrame=CurrentFrame;
	
	onetoOne=TRUE;
	if((sd->raster.xScale != 1.0) || (sd->raster.yScale != 1.0))onetoOne=FALSE;	
	
	
	xscale=sd->raster.xScale*(double)s->xsize;
	oxscale=1.0/sd->raster.xScale;
	yscale=sd->raster.yScale*(double)s->ysize;
	oyscale=1.0/sd->raster.yScale;
	
	x=sd->raster.box.x;
	xsize=sd->raster.box.xsize;
	xinc=sd->xsize;
	if((double)(x+xsize) > xscale){
	    xsize=(long)xscale-x;
	}
	
	y=sd->raster.box.y;
	ysize=sd->raster.box.ysize;
	if((double)(y+ysize) > yscale){
	    ysize=(long)yscale-y;
	}
    
    sd->raster.sxsize=xsize;
    sd->raster.sysize=ysize;

/*
    fprintf(stderr,"getPlotReal raster.box x %d y %d xsize %d ysize %d\n",sd->raster.box.x,sd->raster.box.y,sd->raster.box.xsize,sd->raster.box.ysize);
    fprintf(stderr,"getPlotReal x %ld y %ld xsize %ld ysize %ld\n",x,y,xsize,ysize);
*/
	dmin  =1e60;
	dmax= -1e60;
	
	if(onetoOne){		
		for(j=0;j<ysize;++j){
		    bp=sout+j*xinc;
		    dp=data+x+s->xsize*(y+j);
		    for(i=0;i<xsize;++i){
	            value = *dp++;;
	        	if(dmin > value)dmin=value;
	        	if(dmax < value)dmax=value;
		    	*bp++ = value;
		    }
		}	
	}else{
	    for(j=0;j<ysize;++j){
	        iystep=s->xsize*((long)((double)(y+j)*oyscale));
	        iyinc=j*xinc;
	        bp=sout+iyinc;
	        dx = (double)(x)*oxscale+(double)iystep;
	        for(i=0;i<xsize;++i){
	            value = *(data+((long)(dx)));
	        	if(dmin > value)dmin=value;
	        	if(dmax < value)dmax=value;
	            *bp++ = value;
	            dx += oxscale;
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


	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}
	
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

	dmin  = data[0];
	dmax  = data[0];
	
	for(j=0;j<s->ysize;++j){
	    for(i=0;i<s->xsize;++i){
		    n=i+(s->ysize-1-j)*s->xsize;
			value=data[n];
	        if(dmin > value){
	            dmin=value;
	        }else if(dmax < value){
	            dmax=value;
	        }
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
	
	if(pio->pd.sType){	
		limits->vmin=pio->pd.sPmin;
		limits->vmax=pio->pd.sPmax;
		limits->dolog = (pio->pd.sType == 1);
	}
	
	FilePutCommand(limits);
	
	return 0;
}
static int sds2dGradients(struct FileInfo2 *Files,int flag)
{
    struct FilePIOInfo *pio;
	long Length;
	long n;
	struct SetFrameData  *s;
	double *value;
	double dx,dy,gradl,gradh;
	double odx,ody;
	long xsize,ysize;
	long i,j;
	
	if(flag < 0)return 0;
	if(!Files)return 1;
	pio = &Files->pioData;
	if(!pio)return 1;
	if(Files->fileClass != SDS2D_FILES && Files->fileClass != HDF_FILES)return 1;
	s = &Files->SetData;
	value=(double *)s->data;
	if(!value)return 1;
		
	xsize=s->xsize;
	ysize=s->ysize;
	Length=xsize*ysize;

	if(pio->gradx)cFree((char *)pio->gradx);
	pio->gradx=NULL;

	if(pio->grady)cFree((char *)pio->grady);
	pio->grady=NULL;

	pio->gradx=(double *)cMalloc(sizeof(double)*Length,19727);
	if(!pio->gradx)goto ErrorOut;
	zerol((char *)pio->gradx,sizeof(double)*Length);

	pio->grady=(double *)cMalloc(sizeof(double)*Length,19728);
	if(!pio->grady)goto ErrorOut;
	zerol((char *)pio->grady,sizeof(double)*Length);
	
    dx=(pio->xmax-pio->xmin)/(double)s->xsize;
    odx=1.0/dx;
    dy=(pio->ymax-pio->ymin)/(double)s->ysize;
    ody=1.0/dy;
    
    for(j=1;j<ysize-1;++j){
        for(i=1;i<xsize-1;++i){
            n=i+j*xsize;
            gradl=(value[n]-value[n-1])*odx;
            gradh=(value[n+1]-value[n])*odx;
			if((gradh*gradl >= 0.0)){
				if(fabs(gradh) < fabs(gradl)){
					pio->gradx[n] = gradh;
				}else{
					pio->gradx[n] = gradl;
				}
			}
            gradl=(value[n]-value[n+xsize])*ody;
            gradh=(value[n-xsize]-value[n])*ody;
			if((gradh*gradl >= 0.0)){
				if(fabs(gradh) < fabs(gradl)){
					pio->grady[n] = gradh;
				}else{
					pio->grady[n] = gradl;
				}
			}
        }
    }

	return 0;
ErrorOut:
	sprintf(WarningBuff,"sds2dGradients Error\n");
	WarningBatch(WarningBuff);
	return 1;
}
static int GetFrameInfo(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	struct LIMITS *limits;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	
	if(Files->fileClass != SDS2D_FILES && Files->fileClass != HDF_FILES)return 1;
	
	if(FilesSetFrame(CurrentFrame,Files))return 1;
	
	s = &Files->SetData;
		
	limits=NULL;
	FileGetLimits(&limits,CurrentFrame,Files);
	if(limits && limits->iGot){
		
		Files->limits.time=limits->time;
		Files->limits.iGotTime=limits->iGotTime;
				
		if(pio->pd.UsePlotScales){
			if(limits->dolog){
			   pio->pd.sType = 1;
			}else{
			   pio->pd.sType = 2;
			}

			if(limits->vmin == limits->vmax){
			   pio->pd.sType = 0;
			}

			if(pio->pd.sType != 0){
				pio->pd.sPmin=limits->vmin;
				pio->pd.sPmax=limits->vmax;
			}
		}
		
		if(pio->pd.UsePlotRange){
			pio->xmin=limits->xmin;
			pio->xmax=limits->xmax;

			pio->ymin=limits->ymin;
			pio->ymax=limits->ymax;

			pio->zmin=limits->zmin;
			pio->zmax=limits->zmax;
		}
		
	}else{	
		Files->limits.time=(double)CurrentFrame;
		Files->limits.iGotTime=FALSE;
				
		if(pio->pd.UsePlotScales){			
			pio->pd.sType = 0;			
			if(Files->fileClass == HDF_FILES){
				pio->pd.sType = 2;
				pio->pd.sPmin=0;
				pio->pd.sPmax=255;
			}						
		}
				
		if(pio->pd.UsePlotRange){
			pio->xmin=0;
			pio->xmax=(double)s->xsize;

			pio->ymin=0;
			pio->ymax=(double)s->ysize;

			pio->zmin=0;
			pio->zmax=(double)s->zsize;
		}
				
	}
	
	pio->rangeData.xmin=pio->xmin;
	pio->rangeData.ymin=pio->ymin;
	pio->rangeData.xmax=pio->xmax;
	pio->rangeData.ymax=pio->ymax;
	pio->rangeData.xsize=s->xsize;
	pio->rangeData.ysize=s->ysize;
	
	return 0;
}
static int GetSDS2DData(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;
	
	if(!Files)return 1;
	pio = &Files->pioData;
		
	if(GetFrameInfo(Files,CurrentFrame))return 1;
		
	if(Files->fileClass == HDF_FILES){
		limits=NULL;
		FileGetLimits(&limits,CurrentFrame,Files);
	    if(hdfToSDS2D(Files,limits,&pio->pd))return 1;
	}
	
	if(pio->pa.flagGradients)sds2dGradients(Files,pio->pa.flagGradients-1);
		
	return 0;
}
static int hdfToSDS2D(struct FileInfo2 *Files,struct LIMITS *limits,struct paletteDraw *pd)
{
	struct SetFrameData  *s;
	unsigned char *p;
	unsigned char n;
	long length;
	double *value;
	double r;
	long nn;
	
	if(!Files || !pd)return 1;
	
	s = &Files->SetData;

	value=(double *)s->data;
	if(!value)return 1;
	
	p=(unsigned char *)s->data;
	if(!p)return 1;
	
	length=s->xsize*s->ysize;
	
	if(length <= 0)return 1;
	
	if(limits && pd->UsePlotScales){
	    for(nn=length-1;nn >= 0;--nn){
	        
			n=p[nn];
			if(n <= 1){
			    r=limits->vmin;
			}else if(n >= 254){
			    r=limits->vmax;
			}else if(limits->dolog ==  1){	    
			    r=limits->vmin*pow(limits->vmax/limits->vmin,
			                                      (double)(n-2)/252.);
			}else{
			    r=limits->vmin+(limits->vmax-limits->vmin)*(n-2)/252.;
			}						
	        value[nn]=r;	        
	    }
	}else if(pd->UsePlotScales){
	    for(nn=length-1;nn >= 0;--nn){
	        value[nn]=p[nn];	        
	    }
	}else{
	    for(nn=length-1;nn >= 0;--nn){
	        
			n=p[nn];
			if(n <= 1){
			    r=pd->sPmin;
			}else if(n >= 254){
			    r=pd->sPmax;
			}else if(pd->sType ==  1){	    
			    r=pd->sPmin*pow(pd->sPmax/pd->sPmin,
			                                      (double)(n-2)/252.);
			}else{
			    r=pd->sPmin+(pd->sPmax-pd->sPmin)*(n-2)/252.;
			}						
	        value[nn]=r;	        
	    }
	}
	
	
	return 0;
}
static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag)
{
    struct FilePIOInfo *pio;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	



	if(pio->range.MaintainAspectRatio){
	    double dxo,dyo,dxi,dyi;

	    dxi=(double)(pio->range.ixmaxData-pio->range.ixminData);
	    dyi=(double)(pio->range.iymaxData-pio->range.iyminData);
	    dxo=(xmax-xmin)/dxi;
	    dyo=(ymax-ymin)/dyi;
	    
	   
	    if(flag == USE_XY){
		    if(dxo > pio->range.AspectRatio*dyo){  /* use x - range */
		        ymin = ymax - dyi*dxo/pio->range.AspectRatio;
		    }else{			     /* use y - range */
				xmax = xmin + dxi*dyo*pio->range.AspectRatio;
		    }
	    }else if(flag == USE_X){
		        ymax  = ymin + dyi*dxo/pio->range.AspectRatio;
	    }else if(flag == USE_Y){
				xmax =  xmin + dxi*dyo*pio->range.AspectRatio;
	    }
	}
	
	pio->range.xmaxData=xmax;
	pio->range.ymaxData=ymax;
	pio->range.xminData=xmin;
	pio->range.yminData=ymin;

    pio->range.xminSet=1;
    pio->range.yminSet=1;
    pio->range.xmaxSet=1;
    pio->range.ymaxSet=1;
    
	return 0;
}
static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy)
{

	if(!pio || !x || !y || !ix || !iy)return 1;

	*ix=(int)((*x-pio->range.xminData)*pio->range.xScale+(double)pio->range.ixminData+.5);
	*iy=(int)((*y-pio->range.yminData)*pio->range.yScale+(double)pio->range.iyminData+.5);

	return 0;
}

/*
static int getPlotDataRaster(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame,struct SetFrameData *sd)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	unsigned char value;
	long i,j;
	double dx,dy;
	double dxhalf,dyhalf;
	unsigned char *data;
	
	double xmin,xmax,ymin,ymax,dmin,dmax;
	double didx,djdy;
	double dxdi,dydj;
	long ixmin,ixmax,iymin,iymax;

	double ymn,ymx,xmn,xmx;
	double xs,ys;
	double xc,yc;
	long ii,jj,ihi,ilo,jhi,jlo;
	long n;
	        
	if(!Files)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(unsigned char *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	if(!sd)return 1;
	
	//CurrentFrame=CurrentFrame;
	
	setSds2dScales(pio);
	
	if(1){
	    return getPlotRaster(Files,sout,CurrentFrame,sd);
	}
	
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
							
			for(jj=jlo;jj<=jhi;++jj){
			    ys=ymin+(double)((double)(jj-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(ii=ilo;ii<=ihi;++ii){
			        xs=xmin+(double)((double)(ii-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
					sout[ii+(pio->range.iymax-1-jj)*pio->range.ixmax]=value;
			    }
			}				
	    }
	}
    	
	return 0;
}
*/
