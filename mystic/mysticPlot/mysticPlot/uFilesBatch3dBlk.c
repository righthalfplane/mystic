/*
 *  uFilesBatch3dBlk.c
 *  mShow
 *
 *  Created by Dale Ranta on 7/29/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */

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
#include "uFilesBatch3d.h"
#include "Universal.h"
#include "uGraph.h"
#include "doSage3DGet.h"

int doSage3DBlkBatch(BatchPtr Batch,char *name);

int checkStreamData(struct dataStruct *plane);

int freeStreamLine3d(struct PlotStreamData *streamline3d);

int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

static int GetRange3(struct uDialogStruct *uList);

static int doSage3DCommandGet(struct FileInfo2 *Files,CommandPtr cp);

static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp);

static int doSage3DSetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doFindMaximum(struct FileInfo2 *Files);

static int doStreamlineSelection(struct PlotStreamData *streamline);

static int doSage3DTracerPlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int doRange4Action(struct uDialogStruct *uList);

static int doVolumeSelection(struct FileInfo2 *Files);

static int doGeneralSelection(struct FileInfo2 *Files);

static int doSage3DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m);

static int doSage3DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int doSage3DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int doSage3DDoDialog(struct FileInfo2 *Files,int *type);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int doAuto(struct FileInfo2 *Files);

static int pioSetLimitsSDS3D(struct FileInfo2 *Files,long nf);

static double *buildSDS3D(struct FileInfo2 *Files);

static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int doSage3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int doSage3DGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int doSage3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int doSage3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int doRange3Action(struct uDialogStruct *uList);

static int doVectorSelection(struct FileInfo2 *Files);

static int doSage3DBlk(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);


int doSage3DBlkBatch(BatchPtr Batch,char *name)
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

    if(Files->fileClass != PIO3D_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"doSage3DBatch Found (%s) instead of PIO3D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSage3DBatch Can Only Process PIO3D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
	
    if(doSage3DBlkStart(Files))goto ErrorOut;
	           
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSage3DBlk(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
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
	    if(n >= (Files->ImageCount-1))flag=1;
	    if(PlotPIOData(Files,n,flag))goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}
static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
	struct SetFrameData  sd;
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	unsigned char *buff;
	char filepath[2048];
	char name[256],*namef;
	long length;
	double *data;
	int ret;
	
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	ret=1;
	
	buff=NULL;
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
	
	zerol((char *)&sd,sizeof(struct SetFrameData));
	
	sd.pioIndex=pio->pioIndex;
	mstrncpy(sd.pioName,pio->pioName,64);
	sd.CurrentFrame=CurrentFrame;

	if(Files->outputType == 0 || Files->outputType == HDF_FILES){ 
	
		/*getPlotImage(Files,buff,CurrentFrame);*/
		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    	    
		if(hdfOut(filepath,name,(char *)pio->pd.palette,pio->range.ixmax,pio->range.iymax,(char *)buff,CurrentFrame,flag,limits))goto ErrorOut;

	}else if(Files->outputType == SDS2D_FILES){
	
		/*getPlotData(Files,(double *)buff,CurrentFrame)*/
		
		sd.type=FRAME_DATA_FLOAT;
		
		doSage3DGetData(Files,pio->CurrentFrame,&sd);
		
		if(!sd.data)goto ErrorOut;
		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	        
	    if(sds2DOut(filepath,name,sd.xsize,sd.ysize,(double *)sd.data,CurrentFrame,flag,limits))goto ErrorOut;
		
		if(sd.data)cFree((char *)sd.data);
		sd.data=NULL;
	    
	}else if(Files->outputType == SDS3D_FILES){
	    
	    data=buildSDS3D(Files);
	    if(!data)goto ErrorOut;
	    
		pioSetLimitsSDS3D(Files,CurrentFrame);
			        
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    
	    if(sds3DOut(filepath,name,pio->plane.Plane_xcount,pio->plane.Plane_ycount,pio->plane.Plane_zcount,data,CurrentFrame,flag,limits))goto ErrorOut;
	   	    
	}
	ret = 0;
ErrorOut:
	if(buff)cFree((char *)buff);
	if(data)cFree((char *)data);
	return ret;
}
static int doSage3DBlk(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
    struct FilePIOInfo *pio;
    double xmin,xmax,ymin,ymax;
	char *command;
 	double value;
 	int rangeNew;
    int flag;
 	
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
	rangeNew = FALSE;
	
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	xmin=pio->range.xminData;
	ymin=pio->range.yminData;
	xmax=pio->range.xmaxData;
	ymax=pio->range.ymaxData;
		
	if(!mstrcmp("plane_type",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    if(!mstrcmp("x",command)){
			pio->plane.Plane_type=X_PLANE;
			SetRange(Files,Files->yminr,Files->ymaxr,Files->zminr,Files->zmaxr,USE_XY);
	    }else if(!mstrcmp("y",command)){
			pio->plane.Plane_type=Y_PLANE;
			SetRange(Files,Files->xminr,Files->xmaxr,Files->zminr,Files->zmaxr,USE_XY);
	    }else{
			pio->plane.Plane_type=Z_PLANE;
			SetRange(Files,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY);
	    }
	}else if(!mstrcmp("plane_xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xmin=value;
	}else if(!mstrcmp("plane_ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_ymin=value;
	}else if(!mstrcmp("plane_zmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zmin=value;
	}else if(!mstrcmp("plane_xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xmax=value;
	}else if(!mstrcmp("plane_ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_ymax=value;
	}else if(!mstrcmp("plane_zmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zmax=value;
	}else if(!mstrcmp("plane_xcount",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xcount=(long)value;
	}else if(!mstrcmp("plane_ycount",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_ycount=(long)value;
	}else if(!mstrcmp("plane_zcount",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zcount=(long)value;
	}else if(!mstrcmp("plane_xloc",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xloc=value;
	}else if(!mstrcmp("plane_yloc",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_yloc=value;
	}else if(!mstrcmp("plane_zloc",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zloc=value;
	}else if(!mstrcmp("sagedata",command)){
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
	    pio->CurrentFrame=(long)value;
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
	}else if(!mstrcmp("drawvectors",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.DrawVectors=(int)value;
	}else if(!mstrcmp("arrowHeads",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.arrowHeads=(int)value;
	}else if(!mstrcmp("arrowLength",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.arrowLength=value;
	}else if(!mstrcmp("arrowAngle",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.arrowAngle=value;
	}else if(!mstrcmp("vfac",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.vfac=value;
	}else if(!mstrcmp("vmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.vmax=value;
	}else if(!mstrcmp("vspace",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->vector.vspace=value;
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
static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp)
{
	struct CommandInfo cpl;
    struct FilePIOInfo *pio;
	/* char line[4096]; */
	char *command;
 	double value;
 	long n,nn;
	int ret;

	if(!Files || !cp)return 1;
	pio = &Files->pioData;
	ret = 1;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));

	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	if(!mstrcmp("stream3d",command)){
	    freeStreamLine3d(&pio->streamline3d);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline3d.streamOn=(long)value;
	    ++(cp->n);
	    if(pio->streamline3d.streamOn <= 0)goto Okout; 

	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline3d.streamGradients=(long)value;
	    ++(cp->n);

	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline3d.streamSteps=(long)value;
	    ++(cp->n);

	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline3d.streamPasses=(long)value;
	    ++(cp->n);

	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    nn=(long)value;
	    ++(cp->n);
	    for(n=0;n<nn;++n){
	    	if(checkStreamData(&pio->streamline3d.xPlane))goto ErrorOut;
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline3d.xPlane.x[n]=value;
	    	++(cp->n);
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline3d.xPlane.y[n]=value;
	    	++(cp->n);
	    	pio->streamline3d.xPlane.count++;
	    	
	    }
	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    nn=(long)value;
	    ++(cp->n);
	    for(n=0;n<nn;++n){
	    	if(checkStreamData(&pio->streamline3d.yPlane))goto ErrorOut;
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline3d.yPlane.x[n]=value;
	    	++(cp->n);
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline3d.yPlane.y[n]=value;
	    	++(cp->n);
	    	
	    	pio->streamline3d.yPlane.count++;
	    }
	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    nn=(long)value;
	    ++(cp->n);
	    for(n=0;n<nn;++n){
	    	if(checkStreamData(&pio->streamline3d.zPlane))goto ErrorOut;
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline3d.zPlane.x[n]=value;
	    	++(cp->n);
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline3d.zPlane.y[n]=value;
	    	++(cp->n);
	    	pio->streamline3d.zPlane.count++;
	    	
	    }
	    
	    
	}else if(!mstrcmp("stop",command)){
	    goto ErrorOut;
	}else if(!mstrcmp("end",command)){
	    goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
	;
Okout:
 	getCommand(NULL,&cpl);
	return ret;
}
static int doSage3DCommandGet(struct FileInfo2 *Files,CommandPtr cp)
{
	char *command;
	int ret;

	if(!Files || !cp)return 1;
	ret = 1;
	
	for(cp->n=0;cp->n<cp->nword;++cp->n){
		command=stringCommand(cp);
		if(!command)break;
		if(CommandGetProcess(Files,cp))break;
	}
	
	ret = 0;
	
	return ret;
}
int doSage3DBlkStart(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	
	pio->range.ixmax=532;
	pio->range.iymax=432;
	
	pio->range.ixminData=20;
	pio->range.ixmaxData=522;
	pio->range.iyminData=20;
	pio->range.iymaxData=422;

	pio->range.AspectRatio=1.0;
	pio->range.MaintainAspectRatio=1;
	pio->range.receiveRange=1;
	
	
	pio->vector.DrawVectors=0;
	pio->vector.arrowLength=0.1;
	pio->vector.arrowAngle=145.0;
	pio->vector.arrowHeads=1;
	pio->vector.vspace= -21;
	pio->vector.vmax=0;
	pio->vector.vfac=0.2;
	
    mstrncpy(pio->vector.vectorxName,"xdt",32);
    mstrncpy(pio->vector.vectoryName,"ydt",32);
    mstrncpy(pio->vector.vectorzName,"zdt",32);
	
	pio->pd.sType=0;
	pio->pd.sPmin=0;
	pio->pd.sPmax=1;	
	pio->pd.LabeledPalette=FALSE;	
	pio->pd.paletteFont=2;
	pio->pd.AreaFill=1;
	
	pio->pd.red=1.0;
	pio->pd.green=1.0;
	pio->pd.blue=1.0;
	
	getPaletteByName("ps",pio->pd.palette);
		
	pio->pa.DrawZones=0;
	pio->pa.ZoneLevel=1;
	
	pio->plane.Plane_type=Z_PLANE;
	pio->plane.Plane_xstep=0;
	pio->plane.Plane_xcount=50;
	pio->plane.Plane_ystep=0;
	pio->plane.Plane_ycount=50;
	pio->plane.Plane_zstep=0;
	pio->plane.Plane_zcount=50;
	pio->plane.Plane_gloc=0;
	pio->plane.Plane_gcount=50;
	
	pio->plane.Plane_xloc=pio->plane.Plane_xmin=Files->xminr;
	pio->plane.Plane_xmax=Files->xmaxr;
	pio->plane.Plane_yloc=pio->plane.Plane_ymin=Files->yminr;
	pio->plane.Plane_ymax=Files->ymaxr;
	pio->plane.Plane_zloc=pio->plane.Plane_zmin=Files->zminr;
	pio->plane.Plane_zmax=Files->zmaxr;
	
	pio->plane.Plane_gx1=Files->xminr;
	pio->plane.Plane_gy1=Files->yminr;
	pio->plane.Plane_gz1=Files->zminr;
	
	pio->plane.Plane_gx2=Files->xmaxr;
	pio->plane.Plane_gy2=Files->yminr;
	pio->plane.Plane_gz2=Files->zminr;
	
	pio->plane.Plane_gx3=Files->xminr;
	pio->plane.Plane_gy3=Files->ymaxr;
	pio->plane.Plane_gz3=Files->zminr;
	
	pio->plane.Plane_gmin=Files->zminr;
	pio->plane.Plane_gmax=Files->zmaxr;
	pio->plane.Plane_gloc=0;
	
	Files->pGetData2=doSage3DGetData;
	
	Files->pGetData3=doSage3DGetData;
	
	Files->pGetAreaData=doSage3DGetAreaData;
	
	Files->pGetLineData=doSage3DGetLineData;
	
	Files->pDoDialog=doSage3DDoDialog;
	
	Files->pInformation=doSage3DInformation;
	
	Files->pGetCellData=doSage3DGetCellData;
	
	Files->pSetCellData=doSage3DSetCellData;

	Files->pPlotLineData=doSage3DPlotLineData;
	
	Files->pCommandSend=doSage3DCommandGet;

	pio->CurrentFrameGradients =  -1;
	pio->CurrentFrameVectors = -1;
	pio->threads=16;
	
    mstrncpy(pio->pioName,"density",255);
    pio->pioIndex=0;

	SetRange(Files,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY);
	
	return 0;
}
static int doSage3DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m)
{
	char name[256];
	int list1[4000],list2[4000];
	char *fp;
	long n;
	int nc1,nc2;
	
	if(!Files || !m || !m->nameList || (m->nameCount < 1))return 1;

	nc1=0;
	nc2=0;
	for(n=0;n<m->nameCount;++n){
	    if(!m->nameList[n])continue;
		mstrncpy(name,m->nameList[n],255);
		fp=strrchr((char *)name,'(');
		if(fp){
		    *fp=0;
		    if(!strcmp(name,"laser_plot_e") || !strcmp(name,"laser_plot_x") ||
		       !strcmp(name,"laser_plot_y") || !strcmp(name,"laser_plot_z")){
				if(nc1 < 4000){
					++fp;
					list1[nc1++]=atoi(fp);
		        }
				cFree((char *)m->nameList[n]);
				m->nameList[n]=NULL;
				continue;
		    }else if(!strcmp(name,"tracer_data")){
				if(nc2 < 4000){
					++fp;
					list2[nc2++]=atoi(fp);
		        }
				cFree((char *)m->nameList[n]);
				m->nameList[n]=NULL;
				continue;
		    }
		}  
		sprintf(WarningBuff,"doSage3DPlotLineData  %s - cannot plot\n",m->nameList[n]);
		WarningBatch(WarningBuff);
		cFree((char *)m->nameList[n]);
		m->nameList[n]=NULL;
		
	}
	
	cFree((char *)m->nameList);
	m->nameList=NULL;
	
	if(nc1)doSage3DSavePlotLineData(Files,list1,nc1);
	
	if(nc2)doSage3DTracerPlotLineData(Files,list2,nc2);
	
	return 0;
}
static int doSage3DTracerPlotLineData(struct FileInfo2 *Files,int *list,int nc)
{
    struct FilePIOInfo *pio;
	long n,k;
	int *TracerPlotList;
	int nn,iskip;
	
	if(!Files || !list || (nc < 1))return 1;
	pio=&Files->pioData;
	
	if(pio->TracerPlotCount && pio->TracerPlotList){
	    nn=0;
		for(n=0;n<nc;++n){
			iskip = FALSE;
	    	for(k=0;k<pio->TracerPlotCount;++k){
			    if(pio->TracerPlotList[k] == list[n]){
			       iskip = TRUE;
			       break;
			    }
			}			
			if(!iskip){
			    list[nn++]=list[n];
			}
	    }
	    TracerPlotList=(int *)cMalloc((pio->TracerPlotCount+nn)*sizeof(int),8260);
	    if(!TracerPlotList){
			WarningBatch("doSage3DTracerPlotLineData out of memory\n");
			return 1;
	    }
	    for(k=0;k<pio->TracerPlotCount;++k){
	        TracerPlotList[k]=pio->TracerPlotList[k];
	    }
		for(n=0;n<nn;++n){
		   TracerPlotList[pio->TracerPlotCount+n]=list[n];
		}
	    cFree((char *)pio->TracerPlotList);
	    pio->TracerPlotList=TracerPlotList;
	    pio->TracerPlotCount=pio->TracerPlotCount+nn;
	    
	}else{
	    pio->TracerPlotList=(int *)cMalloc(nc*sizeof(int),0261);
	    if(!pio->TracerPlotList){
			WarningBatch("doSage3DTracerPlotLineData out of memory\n");
			return 1;
	    }
	
		for(n=0;n<nc;++n){
		    pio->TracerPlotList[n]=list[n];
		}
		
		pio->TracerPlotCount=nc;
	}

	return 0;
}

static int doSage3DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc)
{
    struct FilePIOInfo *pio;
	long n,k;
	int *LaserPlotList;
	int nn,iskip;
	
	if(!Files || !list || (nc < 1))return 1;
	pio=&Files->pioData;
	
	if(pio->LaserPlotCount && pio->LaserPlotList){
	    nn=0;
		for(n=0;n<nc;++n){
			iskip = FALSE;
	    	for(k=0;k<pio->LaserPlotCount;++k){
			    if(pio->LaserPlotList[k] == list[n]){
			       iskip = TRUE;
			       break;
			    }
			}			
			if(!iskip){
			    list[nn++]=list[n];
			}
	    }
	    LaserPlotList=(int *)cMalloc((pio->LaserPlotCount+nn)*sizeof(int),8260);
	    if(!LaserPlotList){
			WarningBatch("doSage3DSavePlotLineData out of memory\n");
			return 1;
	    }
	    for(k=0;k<pio->LaserPlotCount;++k){
	        LaserPlotList[k]=pio->LaserPlotList[k];
	    }
		for(n=0;n<nn;++n){
		   LaserPlotList[pio->LaserPlotCount+n]=list[n];
		}
	    cFree((char *)pio->LaserPlotList);
	    pio->LaserPlotList=LaserPlotList;
	    pio->LaserPlotCount=pio->LaserPlotCount+nn;
	    
	}else{
	    pio->LaserPlotList=(int *)cMalloc(nc*sizeof(int),8281);
	    if(!pio->LaserPlotList){
			WarningBatch("doSage3DSavePlotLineData out of memory\n");
			return 1;
	    }
	
		for(n=0;n<nc;++n){
		    pio->LaserPlotList[n]=list[n];
		}
		
		pio->LaserPlotCount=nc;
	}

	return 0;
}
static int doSage3DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	
	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	
	sd->range=pio->range;
	sd->pd=pio->pd;
	sd->pa=pio->pa;
	sd->plane=pio->plane;
	sd->vector=pio->vector;
	sd->streamline3d=pio->streamline3d;

	if(pio->pioName[0]){
	    mstrncpy(sd->pioName,pio->pioName,255);
	    sd->pioIndex=(int)pio->pioIndex;
	}else{
	   sd->pioName[0]=0;
	   pio->pioIndex=0;
	}

	return 0;
}
static double *buildSDS3D(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
 	
	if(!Files)return NULL;
	
	pio = &Files->pioData;

	return doSage3DGetSDS3DFnt(Files,pio->CurrentFrame);	
}
static int doSage3DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	long CurrentFrame;
    struct FilePIOInfo *pio;
	
	if(!li)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;
	

	CurrentFrame=li->CurrentFrame;

	FilesSetFrame(CurrentFrame,Files);
	
	return doSage3DGetLineDataFnt(Files,li);	
}


static int doSage3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
	long CurrentFrame;

	if(!ai)return 1;
	if(!Files)return 1;
	
	CurrentFrame=ai->CurrentFrame;
	
	FilesSetFrame(CurrentFrame,Files);
	
	return doSage3DGetAreaDataFnt(Files,ai);
}

static int doSage3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{

	if(!Files)return 1;
	if(!sd)return 1;
	
	FilesSetFrame(CurrentFrame,Files);
	
	return doSage3DGetDataFnt(Files,CurrentFrame,sd);	/* always Blocked Data */
}
static int doSage3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;
	struct LIMITS *limits;
	struct PIO_BLK *sage;
	char name[256];
	long length;
	double *fdata;
    int ret;

	if(!Files)return 1;
	if(!sd)return 1;
	s = &Files->SetData;
	limits=&Files->limits;
	pio=&Files->pioData;

	ret=1;
	
	if(sd->type == LINEDATA_STREAMLINE_DATA){
	   return  doSage3DGetStreamLineFnt(Files,CurrentFrame,sd);
	}else if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage3DGetCellData - Only Returns LINEDATA_CELL_DATA\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	FilesSetFrame(CurrentFrame,Files);

	sage=&pio->sageBlk;

	name[0]=0;
	mstrncpy(name,sd->pioName,255);
	if(name[0] == 0){
		mstrncpy(name,"density",255);
	}

	fdata=NULL;
	if(SageGetFileItemFnt(Files,name,sd->pioIndex,&fdata,&length,CurrentFrame)){
	    ret=17;goto ErrorOut;
	}
	        			
    sd->xsize=length;
    sd->ysize=1;
    sd->zsize=1;
    sd->length=length;
    sd->data=fdata;
    sd->pioTime=s->pioTime;
    if(!sd->data){
    	sprintf(WarningBuff,"doSage3DGetCellData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }

	limits->xmin=sage->xzero;
	limits->ymin=sage->yzero;
	limits->xmax=sage->dxset*sage->ixmax+sage->xzero;
	limits->ymax=sage->yzero+sage->dyset*sage->iymax;
	limits->zmin=sage->zzero;;
	limits->zmax=sage->zzero+sage->dzset*sage->izmax;
	limits->time=s->pioTime;
    
    ret=0;
    
ErrorOut:
	
	return ret;
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
	pio->range.xmaxSet=1;
	pio->range.yminSet=1;
	pio->range.ymaxSet=1;

	return 0;
}	
	
static int pioSetLimitsSDS3D(struct FileInfo2 *Files,long nf)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;


	if(!Files)return 1;
	pio = &Files->pioData;

	limits=&Files->limits;
	limits->iGot = FALSE;
	
	limits->pioIndex=Files->pioIndex;
	if(limits->pioName)cFree((char *)limits->pioName);
	limits->pioName=(unsigned char *)strsave(Files->pioName,5281);
	if(!limits->pioName)return 1;

	limits->iGot = TRUE;

	limits->xmin=pio->plane.Plane_xmin;
	limits->ymin=pio->plane.Plane_ymin;
	limits->zmin=pio->plane.Plane_zmin;
	limits->xmax=pio->plane.Plane_xmax;
	limits->ymax=pio->plane.Plane_ymax;
	limits->zmax=pio->plane.Plane_zmax;
	limits->vmin=pio->plane.Plane_dmin;
	limits->vmax=pio->plane.Plane_dmax;
	
	limits->dolog = (pio->pd.sType == 1);

	limits=NULL;
	FileGetLimits(&limits,nf,Files);

	if(!limits || !limits->iGot)return 1;
	 
	return 0;
}

static int doSage3DSetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct SetFrameData *s;
    int ret;

	if(!Files)return 1;
	if(!sd)return 1;
	s = &Files->SetData;
	
	ret=1;
	
	sprintf(WarningBuff,"doSage3DSetCellData - Does Not Work On Blocked Files\n");
	WarningBatch(WarningBuff);
	goto ErrorOut;
	        			
    ret=0;
    
ErrorOut:
	
	return ret;
}

static int doScale1Action(struct uDialogStruct *uList);
static int doDataScale(struct FileInfo2 *Files);

static int doDataScale(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {220,275,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {310,275,70,20},uDialogButton,uDialogNone},
       
       /*  2 */      {"Auto Scale - Linear",   {20,20,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  3 */      {"Manual Scale - Log",    {20,50,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  4 */      {"Manual Scale - Linear", {20,75,160,20},uDialogButtonRadio,uDialogGetLong},
       
       /*  5 */      {"",{275,50,105,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"",{275,75,105,20},uDialogText,uDialogGetDouble},
       
       /*  7 */      {"Range Min", {190,50,80,20},uDialogString,uDialogNone},
       /*  8 */      {"Range Max", {190,75,80,20},uDialogString,uDialogNone},
      
       /*  9 */      {"Labeled Palette", {20,105,120,20},uDialogCheck,uDialogGetLong},
       
       
       /* 10 */      {"Color Fill Cells",{295,105,120,20},uDialogCheck,uDialogGetLong},
      
       /* 11 */      {"Show Range", {275,20,100,20},uDialogButton,uDialogNone},
       /* 12 */      {"Use Gradients",{20,125,120,20},uDialogCheck,uDialogGetLong},
       
       /* 13 */      {"Hide Mesh", {170,105,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 14 */      {"Show Mesh", {170,125,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 15 */      {"Show To Level:", {170,145,120,20},uDialogButtonRadio,uDialogGetLong},
       /* 16 */      {"",{295,145,85,20},uDialogText,uDialogGetDouble},
       /* 17 */      {"Rotate Grid",{20,145,120,20},uDialogCheck,uDialogGetLong},
       /* 18 */      {"Show Mesh Color",{170,170,160,20},uDialogCheck,uDialogGetLong},

      /*  19 */      {"0", {80,190,80,20},uDialogText,uDialogGetDouble},
      /*  20 */      {"0", {80,212,80,20},uDialogText,uDialogGetDouble},
      /*  21 */      {"0", {80,235,80,20},uDialogText,uDialogGetDouble},

      /*  22 */      {"red", {20,190,50,20},uDialogString,uDialogNone},
      /*  23 */      {"green",{20,212,50,20},uDialogString,uDialogNone},
      /*  24 */      {"blue", {20,235,50,20},uDialogString,uDialogNone},

      /*  25 */      {"Mesh Background", {20,168,140,20},uDialogString,uDialogNone},
       /* 26 */      {"Reflect-x",{170,190,120,20},uDialogCheck,uDialogGetLong},
       
       /* 27 */      {"Zones Per Block", {170,215,115,20},uDialogString,uDialogNone},
       /* 28 */      {"Blocksizedata", {170,235,100,20},uDialogText,uDialogGetLong},
       

        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,420,330};
	struct FilesSetList list;
 	
 	uDialogList *uList;
	uDialogReturn *uR;
    
	int sType;
	int dType;
	struct FilePIOInfo *pio;
 	int oldGradients;
    int reti;
    int ret;
    
	if(!Files)return 1;
	pio=&Files->pioData;
	
	oldGradients=pio->pa.flagGradients;
		
	ret=1;
	
	uR=NULL;
	


	uList=uDialogOpen("Range",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,2L,
	          	  uDialogSetRadioGroup,1,
		          uDialogSetLogical,(pio->pd.sType == 0),
	              
	              uDialogSetItem,3L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(pio->pd.sType == 1),
	              
	              uDialogSetItem,4L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(pio->pd.sType == 2),
	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,pio->pd.sPmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,pio->pd.sPmax,

	              uDialogSetItem,9L,
	              uDialogSetLogical,(pio->pd.LabeledPalette == 1),	              
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(pio->pd.AreaFill  == 1),
	              
	              uDialogSetItem,12L,
	              uDialogSetLogical,(pio->pa.flagGradients  == 1),

	              uDialogSetItem,13L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(pio->pa.DrawZones  == 0),
	              
	              uDialogSetItem,14L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(pio->pa.DrawZones  == 1),
	              
	              uDialogSetItem,15L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(pio->pa.DrawZones  == 2),

	              uDialogSetItem,16L,
	              uDialogSetDouble,(double)pio->pa.ZoneLevel,
	              
	              uDialogSetItem,17L,
	          	  uDialogSetHidden,(Files->fileClass != PIO1D_FILES), 
	              uDialogSetLogical,(int)pio->pd.Rotate1d,
	              	              
	              uDialogSetItem,18L,
	              uDialogSetLogical,(pio->pa.DrawZonesColor  == 1),

	              uDialogSetItem,19L,
	              uDialogSetDouble,pio->pd.red,
	              
	              uDialogSetItem,20L,
	              uDialogSetDouble,pio->pd.green,
	              	              
	              uDialogSetItem,21L,
	              uDialogSetDouble,pio->pd.blue,
	              
	              uDialogSetItem,26L,
	          	  uDialogSetHidden,(Files->fileClass != PIO2D_FILES), 
	              uDialogSetLogical,(Files->Reflect_x  == 1),
	              
	              uDialogSetItem,28L,
	              uDialogSetLong,(long)pio->pa.BlockSize,
	              
	              
	              uDialogSetMyIcon,Files,	              
	              uDialogSetAction,doScale1Action,	 
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
		
	dType=0;
	if(uR[14].lreturn){
	    dType=1;
	}else if(uR[15].lreturn){
	    dType=2;
	}
	
    pio->pd.sType=sType;
	pio->pd.sPmin=  uR[5].dreturn;
	pio->pd.sPmax=  uR[6].dreturn;
	pio->pd.LabeledPalette=(int)uR[9].lreturn;
	pio->pd.AreaFill= (int)uR[10].lreturn;    
	pio->pa.flagGradients= (int)uR[12].lreturn;
	
	pio->pa.DrawZones=(int)dType;
	pio->pa.DrawZonesColor=(int)uR[18].lreturn;
	
	pio->pa.ZoneLevel=(int)uR[16].dreturn;
			
	pio->pd.Rotate1d=(int)uR[17].lreturn;

	pio->pd.red   =  uR[19].dreturn;
	pio->pd.green =  uR[20].dreturn;
	pio->pd.blue  =  uR[21].dreturn;
			
	list.Reflect_x=(int)uR[26].lreturn;
	FilesSetParameters(Files,&list,FILES_SET_REFLECT_X);
	
	pio->pa.BlockSize=uR[28].lreturn;
	if(pio->pa.BlockSize < 0)pio->pa.BlockSize=0;
	
	if(pio->pa.flagGradients && !oldGradients){
	    pio->pioNameOld[0]=0;
	    pio->pioIndexOld=-1;
	}
	
	if(Files->fileClass == PIO1D_FILES){
	    if(pio->pd.Rotate1d){
	        Files->yminr=Files->xminr;
	        Files->ymaxr=Files->xmaxr;
	        pio->range.MaintainAspectRatio=TRUE;
	    }else{
	        Files->yminr=0;
	        Files->ymaxr=pio->dxset;
	        pio->range.MaintainAspectRatio=FALSE;
	    }	
	}

    
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doScale1Action(struct uDialogStruct *uList)
{
	struct SetFrameData  sd;
	struct FileInfo2 *Files;
 	double data,dmin,dmax;
	long nf;
	int ret;
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	if(!Files->pGetData2){
	    Warning("doScale1Action pGetData2 NULL");
	    return 1;
	}
	
	
	uSetCursor(NULL,-uWATCH_CURSOR);
	
    sd.data=NULL;  
	if(uList->actionItem == 11){
	
	    dmin =  1e60;
	    dmax = -1e60;
	    
		for(nf=0;nf<Files->ImageCount;++nf){
	    
		
		    zerol((char *)&sd,sizeof(struct SetFrameData));
				
		    sd.type=FRAME_DATA_FLOAT;
		    
		    if((*Files->pGetData2)(Files,nf,&sd)){
				goto OutOfHere;
		    }
	    		    
		    data=sd.limits.vmin;
		
	    	if(data > dmax)dmax=data;
	    	if(data < dmin)dmin=data;

		    data=sd.limits.vmax;
		
	    	if(data > dmax)dmax=data;
	    	if(data < dmin)dmin=data;

		    if(sd.data)cFree((char *)sd.data); 
		    sd.data=NULL;  
		    
		    if(Stop("Stop Ranging Data ?"))break;
	   }
	   
		ret=uDialogSet(uList,5L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,dmax,
	              	              	              	              
	              uDialogSetItem,2L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(1),
	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
		
		if(uList)uDialogUpdate(uList);
	
	}
OutOfHere:	
	if(sd.data)cFree((char *)sd.data); 
	uSetCursor(NULL,-uARROW_CURSOR);
	return 0;
}
static int uTestRange(uDialogReturn *uR);
static int doRange2Action(struct uDialogStruct *uList);
static int SendRange(struct FileInfo2 *Files,uDialogReturn *uR);

static int doDataRange(struct FileInfo2 *Files);

static double doubleConvert(double x);

static double doubleConvert(double x)
{
	char buff[2048];
	
	sprintf(buff,"%g",x);
	
	return atof(buff);
}
static int doDataRange(struct FileInfo2 *Files)
{
/*
	static int lflag[]={
			    DialogInt,DialogInt,
			    DialogFloat,DialogFloat,DialogFloat,DialogFloat,
			    DialogInt,DialogInt,DialogInt,DialogInt,
			    DialogCheck,DialogCheck,DialogUser
			    };
*/			    
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
	                        
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {250,270,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {340,270,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"ixmaxScr", {80,29,85,20},uDialogText,uDialogGetLong},
       /*  3 */      {"iymaxSc", {280,29,85,20},uDialogText,uDialogGetLong},
       /*  4 */      {"xmin data", {280,78,110,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"ymindata", {280,103,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"xmaxdata", {280,128,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"ymaxdata", {280,153,110,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"ixmin data", {80,78,85,20},uDialogText,uDialogGetLong},
       /*  9 */      {"iymindata", {80,103,85,20},uDialogText,uDialogGetLong},
       /* 10 */      {"ixmaxdata", {80,128,85,20},uDialogText,uDialogGetLong},
       /* 11 */      {"iymaxdata", {80,153,85,20},uDialogText,uDialogGetLong},
       /* 12 */      {"Maintain Aspect Ratio", {20,190,177,22},uDialogCheck,uDialogGetLong},
       /* 13 */      {"Receive Domains", {220,190,170,20},uDialogCheck,uDialogGetLong},
       /* 14 */      {"Send Domains", {220,220,110,20},uDialogButton,uDialogNone},
       /* 15 */      {"xmin", {220,78,50,20},uDialogString,uDialogNone},
       /* 16 */      {"ymin", {220,103,50,20},uDialogString,uDialogNone},
       /* 17 */      {"xmax", {220,128,50,20},uDialogString,uDialogNone},
       /* 18 */      {"ymax", {220,153,50,20},uDialogString,uDialogNone},
       /* 19 */      {"ixmin", {20,78,50,20},uDialogString,uDialogNone},
       /* 20 */      {"iymin", {20,104,50,20},uDialogString,uDialogNone},
       /* 21 */      {"ixmax", {20,132,50,20},uDialogString,uDialogNone},
       /* 22 */      {"iymax", {20,153,50,20},uDialogString,uDialogNone},
       /* 23 */      {"xsize", {20,29,50,20},uDialogString,uDialogNone},
       /* 24 */      {"ysize", {220,29,50,20},uDialogString,uDialogNone},
       /* 25 */      {"Image Domain Size", {183,5,130,20},uDialogString,uDialogNone},
       /* 26 */      {"Pixel Domain", {80,55,100,20},uDialogString,uDialogNone},
       /* 27 */      {"Plot Domain", {280,56,100,20},uDialogString,uDialogNone},
       
       
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,430,310};

    struct FilePIOInfo *pio;
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    int flag;
	
	if(!Files)return 1;
	pio=&Files->pioData;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Domain",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)pio->range.ixmax,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)pio->range.iymax,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)pio->range.xminData,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)pio->range.yminData,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)pio->range.xmaxData,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)pio->range.ymaxData,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)pio->range.ixminData,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)pio->range.iyminData,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)pio->range.ixmaxData,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)pio->range.iymaxData,
	              
	              uDialogSetItem,12L,
	              uDialogSetLogical,pio->range.MaintainAspectRatio,
	              
	              uDialogSetItem,13L,
	              uDialogSetLogical,pio->range.receiveRange,
	              
	              
	              uDialogSetAction,doRange2Action,
	              
	              uDialogSetMyIcon,Files,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
		
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uTestRange(uR))goto Start;
	
	pio->range.ixmax=(int)uR[2].lreturn;
	pio->range.iymax=(int)uR[3].lreturn;
	
	flag=0;
	if(uR[4].dreturn != doubleConvert(pio->range.xminData))flag  = 1;
	if(uR[6].dreturn != doubleConvert(pio->range.xmaxData))flag += 2;
	if(uR[5].dreturn != doubleConvert(pio->range.yminData))flag += 4;
	if(uR[7].dreturn != doubleConvert(pio->range.ymaxData))flag += 8;
		
		
	pio->range.xminData=uR[4].dreturn;
	pio->range.yminData=uR[5].dreturn;
	pio->range.xmaxData=uR[6].dreturn;
	pio->range.ymaxData=uR[7].dreturn;

	pio->range.ixminData=uR[8].lreturn;
	pio->range.iyminData=uR[9].lreturn;
	pio->range.ixmaxData=uR[10].lreturn;
	pio->range.iymaxData=uR[11].lreturn;

	pio->range.MaintainAspectRatio=(int)uR[12].lreturn;
	pio->range.receiveRange=(int)uR[13].lreturn;
	
	SetRange(Files,pio->range.xminData,pio->range.xmaxData,pio->range.yminData,pio->range.ymaxData,flagout[flag]);

/*
	SendRange((struct FileInfo2 *)uList->myIconItem,uR);	
*/

	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	return ret;
}
static int doRange2Action(struct uDialogStruct *uList)
{
 	uDialogReturn *uR;
	
	if(!uList)return 1;
	
    uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem ==14){
	
		uR=uDialogGetReturn(uList);
		
	    if(!uR)goto OutOfHere;

 	   SendRange((struct FileInfo2 *)uList->myIconItem,uR);	
 	   
		if(uList)uDialogUpdate(uList);
	}
OutOfHere:
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int uTestRange(uDialogReturn *uR)
{
	if(!uR)return 1;
	
	if(uR[2].lreturn < 1){
	    Warning("xsize Must Be Greater Than 1");
	    goto Error;
	}

	if(uR[3].lreturn < 1){
	    Warning("ysize Must Be Greater Than 1");
	    goto Error;
	}


	if(uR[4].dreturn >= uR[6].dreturn){
	    Warning("xmax Must Be Greater Than xmin");
	    goto Error;
	}
	
	if(uR[8].lreturn >= uR[10].lreturn){
	    Warning("ixmax Must Be Greater Than ixmin");
	    goto Error;
	}

	if(uR[5].dreturn >= uR[7].dreturn){
	    Warning("ymax Must Be Greater Than ymin");
	    goto Error;
	}
	
	if(uR[9].lreturn >= uR[11].lreturn){
	    Warning("iymax Must Be Greater Than iymin");
	    goto Error;
	}
	
	if(uR[10].lreturn > uR[2].lreturn){
	    Warning("xsize Must Be Greater Than or Equal To ixmax");
	    goto Error;
	}

	if(uR[11].lreturn > uR[3].lreturn){
	    Warning("ysize Must Be Greater Than or Equal To iymax");
	    goto Error;
	}

	if(uR[8].lreturn < 0){
	    Warning("ixmin Must Be Greater Than or Equal To 0");
	    goto Error;
	}

	if(uR[9].lreturn < 0){
	    Warning("iymin Must Be Greater Than or Equal To 0");
	    goto Error;
	}
	return 0;
Error:
	return 1;
}

static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int doScale2SetButtons(struct uDialogStruct *uList);
static int doScale2Keys(struct uDialogStruct *uList);
static int GetRange2(struct uDialogStruct *uList);
static int DoClickInList2(struct uDialogStruct *uList);
static int doDataSelection(struct FileInfo2 *Files);

static int doDataSelection(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {10,50,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {80,50,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,40,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"Range", {315,29,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"dmin", {5,2,65,20},uDialogString,uDialogNone},
      /*  5 */      {"dmax", {180,2,65,20},uDialogString,uDialogNone},
      /*  6 */      {"", {50,2,110,20},uDialogText,uDialogGetDouble},
      /*  7 */      {"", {240,2,110,20},uDialogText,uDialogGetDouble},
      /*  8 */      {"File List",{20,40,65,65},uDialogScrollList,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,300};

 	uRect *rectOpen;
 	
 	uDialogList *uList;
	struct ScrollList *List1;
 	long count,n;
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
	              uDialogSetKeys,doScale2Keys,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
	

	if(uDialog(uList))goto ErrorOut;
	
	Files->pioName[0]=0;
	Files->pioIndex = -1;

	if(uDialogSet(uList,0L,
	              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List1,
              	              	              
              uDialogSetDone,uDialogSetDone
    ))goto ErrorOut;
	
	count=ScrollListCount(List1);
	if(count){
	   n=ScrollListFirstSelect(List1);
	   if(n >= 0){
	       ScrollListGet(Files->pioName,n,List1);
	       Files->pioIndex = ScrollListGetItemCount(Files->pioName,n,List1);
		   if(!Files->pioData.pa.DrawZones){
		       Files->pioData.pd.AreaFill=TRUE;
		   }
		   Files->pioData.pioIndex=Files->pioIndex;
		   mstrncpy(Files->pioData.pioName,Files->pioName,255);
	       
	   }
	}	


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
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 3){
		if(Files->fileClass == SDS2D_FILES || Files->fileClass == HDF_FILES){
		   GetRange3(uList);
		}else{
		   GetRange2(uList);
		}
		uDialogUpdate(uList);
	}else if(uList->actionItem == 2){
	   if(DoClickInList2(uList))goto Ok;
	}
Ok:
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}

static int GetRange3(struct uDialogStruct *uList)
{
	struct SetFrameData  sd;
	struct ScrollList *List1,*List2;
	double *data;
	struct FileInfo2 *Files;
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
              
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List1,
              	              	              
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
		

	ret = 0;
OutOfHere:
	if(sd.data)cFree((char *)sd.data); 
	ret = 1;
	return ret;
}

static int GetRange2(struct uDialogStruct *uList)
{
	struct ScrollList *List1,*List2;
	double *fdata,*daughter,*active;
	char namebuff[256],nameItem[256];
	struct FileInfo2 *Files;
	long n;
	int reti;
	int ret;
    
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
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
	    long dataLength;
	    int plotItem;
	    long count,nlist;

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
        count=ScrollListCount(List1);
        for(nlist=0;nlist<count;++nlist){
	        ScrollListGet(namebuff,nlist,List1);
            if(!strcmp(namebuff,nameItem)){
                if(nlist == first2)break;
	            ++plotItem;
            }
        }
	        
	    dmin =  1e60;
	    dmax = -1e60;

	    count=ScrollListCount(List2);
	    fdata=NULL;
	    daughter=NULL;
		active=NULL;
	    for(nlist=0;nlist<count;++nlist){
			if(!ScrollListIsSelected(nlist,List2))continue;
			if(SageGetFileItemFnt(Files,nameItem,plotItem,&fdata,&dataLength,nlist)){
			    ret=17;goto OutOfHere;
			}

			if(SageGetFileItemFnt(Files,"cell_daughter",0,&daughter,&dataLength,nlist)){
			    ret=18;goto OutOfHere;
			}

			if(SageGetFileItemFnt(Files,"cell_active",0,&active,&dataLength,nlist)){
				    ret=27;goto OutOfHere;
			}
			
			for(n=0;n<dataLength;++n){
				if(daughter[n] || (active[n] <= 0.0))continue;
				if(fdata[n] > dmax)dmax=fdata[n];
				if(fdata[n] < dmin)dmin=fdata[n];
			}

			if(fdata)cFree((char *)fdata);
			fdata=NULL;
			if(daughter)cFree((char *)daughter);
			daughter=NULL;
			if(active)cFree((char *)active);
			active=NULL;
			
			if(Stop("Stop Data Ranging ?"))break;
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
	if(active)cFree((char *)active);
	ret = 1;
	return ret;
}
static int DoClickInList2(struct uDialogStruct *uList)
{
	struct ScrollList *List1,*List2;
	char ListOneItem[256];
	struct FileInfo2 *Files;
    long n,count,ItemCount;
    int ret;

	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
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

	if(filePIOBlkList(Files,n,List1,1))goto ErrorOut;
		
	doScale2Move(uList);
		
	count=ScrollListCount(List1);
	if(count && ListOneItem[0]){
	    n=ScrollListFindItem(ListOneItem,ItemCount,List1);
	    if(n >= 0){
	       ScrollListSetItemSelected(n,List1);
	    }
	}
	    
ErrorOut:



	return 0;
}
int doScale2Start(struct uDialogStruct *uList)
{
	struct ScrollList *List1;
	struct ScrollList *list2;
	struct FileInfo2 *Files;
	char *name;
	long iTem;
	int ret;
	long n;
	
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;


	if(!IsOption()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,8L,
              uDialogGetScrollList,&List1,
              
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
		
	n=ScrollListFindItem(name,iTem,List1);
	if(n >= 0){
		ScrollListSetItemSelected(n,List1);
	}

	ScrollListSetItemSelected(0L,list2);

	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	
	return 0;
}

static int doScale2Keys(struct uDialogStruct *uList)
{

	struct ScrollList *List2;
	struct ScrollList *List8;
	int ret;
	
	
	if(!uList)return 1;
	
	
	ret=uDialogSet(uList,8L,
              uDialogGetScrollList,&List8,
	          uDialogSetItem,2L,
              uDialogGetScrollList,&List2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;

	if(uList->actionItem == 2){
		ScrollListSetString(uList->mess,List2);
	}else{	
		ScrollListSetString(uList->mess,List8);
	}

/*
	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);	
*/
	
	return 0;
}
int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	struct ScrollList *list8;
	uRect rList1,rList2;
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
              
              uDialogSetItem,8L,
              uDialogGetRect,&rList2,
              uDialogGetScrollList,&list8,
              	              	              
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)return 1;
	
	
			
	rList1.x = 15;
	rList1.y = 25;
	rList1.xsize = (r.xsize-rList1.x)/2;
	rList1.ysize = r.ysize-35-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	
	
	rList2=rList1;
	rList2.x = rList1.x+rList1.xsize;
	
	ScrollListFitRect(&rList2,list8);

	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList1,
              
              uDialogSetItem,8L,
              uDialogSetRect,&rList2,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	return 0;
}

static int doSDS3D(struct FileInfo2 *Files);


static int doWrite2Dplanes(struct FileInfo2 *Files);
static int doPlane1Action(struct uDialogStruct *uList);
static int doDataPlane(struct FileInfo2 *Files);

static int doDataPlane(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {410,200,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {490,200,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"Save As SDS3D", {150,170,120,20},uDialogButton,uDialogNone},
       /*  3 */      {"xmin-data", {220,20,90,20},uDialogText,uDialogGetDouble},
       /*  4 */      {"ymin-data", {220,50,90,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"zmin-data", {220,80,90,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"gmin-data", {220,110,90,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"xmax-data", {370,20,90,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"ymax-data", {370,50,90,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"zmax-data", {370,80,90,20},uDialogText,uDialogGetDouble},
       /* 10 */      {"gmax-data", {370,110,90,20},uDialogText,uDialogGetDouble},
       /* 11 */      {"xloc-data", {510,20,90,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"yloc-data", {510,50,90,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"zloc-data", {510,80,90,20},uDialogText,uDialogGetDouble},
       /* 14 */      {"gloc-data", {510,110,90,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"xmin", {170,20,35,20},uDialogString,uDialogNone},
       /* 16 */      {"ymin", {170,50,35,20},uDialogString,uDialogNone},
       /* 17 */      {"zmin", {170,80,35,20},uDialogString,uDialogNone},
       /* 18 */      {"gmin", {170,110,35,20},uDialogString,uDialogNone},
       /* 19 */      {"xmax", {320,20,39,20},uDialogString,uDialogNone},
       /* 20 */      {"ymax", {320,50,39,20},uDialogString,uDialogNone},
       /* 21 */      {"zmax", {320,80,39,20},uDialogString,uDialogNone},
       /* 22 */      {"gmax", {320,110,39,20},uDialogString,uDialogNone},
       /* 23 */      {"xloc", {470,20,30,20},uDialogString,uDialogNone},
       /* 24 */      {"yloc", {470,50,30,20},uDialogString,uDialogNone},
       /* 25 */      {"zloc", {470,80,30,20},uDialogString,uDialogNone},
       /* 26 */      {"gloc", {470,110,30,20},uDialogString,uDialogNone},
       /* 27 */      {"x-slices", {15,20,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 28 */      {"y-slices", {15,50,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 29 */      {"z-slices", {15,80,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 30 */      {"g-slices", {15,110,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 31 */      {"xmin-data", {100,20,50,20},uDialogText,uDialogGetLong},
       /* 32 */      {"ymin-data", {100,50,50,20},uDialogText,uDialogGetLong},
       /* 33 */      {"zmin-data", {100,80,50,20},uDialogText,uDialogGetLong},
       /* 34 */      {"gmin-data", {100,110,50,20},uDialogText,uDialogGetLong},
       /* 35 */      {"Save As SDS2D", {20,170,120,20},uDialogButton,uDialogNone},
       /* 36 */      {"Use Gradients", {20,140,120,20},uDialogCheck,uDialogGetLong},
       /* 37 */      {"Find Maximum", {280,170,120,20},uDialogButton,uDialogNone},
       /* 38 */      {"Reset Range", {410,170,120,20},uDialogButton,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {160,60,620,240};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
	struct FilePIOInfo *pio;
  	struct PlotRangeData range;
	int Plane_type;
    
    int reti;
    int ret;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	
	Plane_type=pio->plane.Plane_type;
	
	range=pio->range;
	
	ret = 1;
		
	uR=NULL;
	
	uList=uDialogOpen("Planes",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,27L,
		          uDialogSetLogical,(pio->plane.Plane_type == X_PLANE),
	              
	              uDialogSetItem,28L,
	              uDialogSetLogical,(pio->plane.Plane_type == Y_PLANE),
	              
	              uDialogSetItem,29L,
	              uDialogSetLogical,(pio->plane.Plane_type == Z_PLANE),
	              
	              uDialogSetItem,30L,
	              uDialogSetLogical,(pio->plane.Plane_type == G_PLANE),
	              
	              
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,pio->plane.Plane_xmin,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,pio->plane.Plane_ymin,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,pio->plane.Plane_zmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,pio->plane.Plane_gmin,
	              
	              	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,pio->plane.Plane_xmax,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,pio->plane.Plane_ymax,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,pio->plane.Plane_zmax,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,pio->plane.Plane_gmax,
	              
	              
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,pio->plane.Plane_xloc,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,pio->plane.Plane_yloc,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,pio->plane.Plane_zloc,
	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,pio->plane.Plane_gloc,
	              
	              	              
	              uDialogSetItem,31L,
	              uDialogSetDouble,(double)pio->plane.Plane_xcount,
	              
	              uDialogSetItem,32L,
	              uDialogSetDouble,(double)pio->plane.Plane_ycount,
	              
	              uDialogSetItem,33L,
	              uDialogSetDouble,(double)pio->plane.Plane_zcount,
	              
	              uDialogSetItem,34L,
	              uDialogSetDouble,(double)pio->plane.Plane_gcount,
	              
	              uDialogSetItem,36L,
	              uDialogSetLogical,(pio->pa.flagGradients == 1),
	              	              
	              uDialogSetMyIcon,Files,	      
	              uDialogSetAction,doPlane1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;

/*
Start:
*/
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
	if(Plane_type != pio->plane.Plane_type){   		
	    if(range.xminData == pio->range.xminData && range.xmaxData == pio->range.xmaxData &&
	       range.yminData == pio->range.yminData && range.ymaxData == pio->range.ymaxData){
	   		if(pio->plane.Plane_type == X_PLANE){
				SetRange(Files,Files->yminr,Files->ymaxr,Files->zminr,Files->zmaxr,USE_XY);
		    }else if(pio->plane.Plane_type == Y_PLANE){
				SetRange(Files,Files->xminr,Files->xmaxr,Files->zminr,Files->zmaxr,USE_XY);
		    }else if(pio->plane.Plane_type == Z_PLANE){
				SetRange(Files,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY);
		    }else{
	   			SetRange(Files,pio->range.xminData,pio->range.xmaxData,pio->range.yminData,pio->range.ymaxData,USE_XY);
		    }
	    }
	}
    
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doPlane1Action(struct uDialogStruct *uList)
{
	uDialogReturn *uR;
	struct FilePIOInfo *pio;
 	struct FileInfo2 *Files;
 	int Type;
 	int reti;
 	int ret;
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	pio = &Files->pioData;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem ==  0 || uList->actionItem == 2 || uList->actionItem == 35 || 
	   uList->actionItem == 37 || uList->actionItem == 38 ){
	    if(!Files)goto OutOfHere;	
	    
		uR=uDialogGetReturn(uList);

    	if(!uR)goto OutOfHere;
    	
	    if(uR[31].lreturn <= 0){
	        Warning("x-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[32].lreturn <= 0){
	        Warning("y-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[33].lreturn <= 0){
	        Warning("z-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[34].lreturn <= 0){
	        Warning("g-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[3].dreturn >= uR[7].dreturn){
	        Warning("xmin Must Be Less Than xmax");
	        goto OutOfHere;
	    }

	    if(uR[4].dreturn >= uR[8].dreturn){
	        Warning("ymin Must Be Less Than ymax");
	        goto OutOfHere;
	    }

	    if(uR[5].dreturn >= uR[9].dreturn){
	        Warning("zmin Must Be Less Than zmax");
	        goto OutOfHere;
	    }

	    if(uR[6].dreturn >= uR[10].dreturn){
	        Warning("gmin Must Be Less Than gmax");
	        goto OutOfHere;
	    }
	    if(uR[27].lreturn){
	       Type=0;
	    }else if(uR[28].lreturn){
	       Type=1;
	    }else if(uR[29].lreturn){
	       Type=2;
	    }else if(uR[30].lreturn){
	       Type=3;
	    }else{
	       Type=0;
	    }

	    if(Type == 3){
	        Warning("g-slices Not Ready");
	        goto OutOfHere;
	    }
	    
		pio->plane.Plane_xmin=uR[3].dreturn;
		pio->plane.Plane_ymin=uR[4].dreturn;
		pio->plane.Plane_zmin=uR[5].dreturn;
		pio->plane.Plane_gmin=uR[6].dreturn;
	              
		pio->plane.Plane_xmax=uR[7].dreturn;
		pio->plane.Plane_ymax=uR[8].dreturn;
		pio->plane.Plane_zmax=uR[9].dreturn;
		pio->plane.Plane_gmax=uR[10].dreturn;
	              
		pio->plane.Plane_xloc=uR[11].dreturn;
		pio->plane.Plane_yloc=uR[12].dreturn;
		pio->plane.Plane_zloc=uR[13].dreturn;
		pio->plane.Plane_gloc=uR[14].dreturn;
	              
	    pio->plane.Plane_xcount=uR[31].lreturn;
	    pio->plane.Plane_ycount=uR[32].lreturn;
	    pio->plane.Plane_zcount=uR[33].lreturn;
	    pio->plane.Plane_gcount=uR[34].lreturn;
	              	              	              
	    pio->pa.flagGradients=(int)uR[36].lreturn;
	    
	    if(Type != pio->plane.Plane_type){
	        pio->plane.Plane_type = Type;
	        pio->range.xminSet=0;
	        pio->range.xmaxSet=0;
	        pio->range.yminSet=0;
	        pio->range.ymaxSet=0;
	    }
    
	    if(uList->actionItem == 2){
	        doSDS3D(Files);
	    }else if(uList->actionItem == 35){
	        doWrite2Dplanes(Files);
	    }else if(uList->actionItem == 37 || uList->actionItem == 38){
	        if(uList->actionItem == 37){
	        	doFindMaximum(Files);	       
	        }else{
	           pio->plane.Plane_xmin=Files->xminr;
	           pio->plane.Plane_xmax=Files->xmaxr;
	           pio->plane.Plane_ymin=Files->yminr;
	           pio->plane.Plane_ymax=Files->ymaxr;
	           pio->plane.Plane_zmin=Files->zminr;
	           pio->plane.Plane_zmax=Files->zmaxr;
	        }       
	              
			reti=uDialogSet(uList,27L,
			
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,pio->plane.Plane_xmin,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,pio->plane.Plane_ymin,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,pio->plane.Plane_zmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,pio->plane.Plane_gmin,	              
	              	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,pio->plane.Plane_xmax,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,pio->plane.Plane_ymax,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,pio->plane.Plane_zmax,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,pio->plane.Plane_gmax,		
			
	              uDialogSetItem,11L,
	              uDialogSetDouble,pio->plane.Plane_xloc,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,pio->plane.Plane_yloc,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,pio->plane.Plane_zloc,
	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,pio->plane.Plane_gloc,	              
	              
	              uDialogSetDone,uDialogSetDone
	    	);
	    	
	    	if(uList)uDialogUpdate(uList);
	              	              
	    }  
    	
	}
	ret = 0;
OutOfHere:	
    if(uR)uDialogFreeReturn(uR);
	/* if(uList)uDialogUpdate(uList); */
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int doFindMaximum(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	double *value,*daughter,*level;
	double *xcenter,*ycenter,*zcenter;
	double vmax;
	long n,nmax,k,length;
	int ret;
	
	
	if(!Files)return 1;
	pio = &Files->pioData;
	
	ret=1;
		
	value=NULL;
	daughter=NULL;
	xcenter=NULL;
	ycenter=NULL;
	zcenter=NULL;
	level=NULL;
	
	if(SageGetFileItemFnt(Files,pio->pioName,pio->pioIndex,&value,&length,pio->CurrentFrame)){
	    ret=17;goto ErrorOut;
	}
	
	if(SageGetFileItemFnt(Files,"cell_daughter",0L,&daughter,&length,pio->CurrentFrame)){
	    ret=10;goto ErrorOut;
	}

	nmax = -1;
	vmax=0;
    for(n=0;n<length;++n){
        if(daughter[n])continue;
        if(fabs(value[n]) > vmax){
            vmax=fabs(value[n]);
            nmax=n;            
        }
	}
	
	if(daughter)cFree((char *)daughter);
	daughter=NULL;
		
	if(value)cFree((char *)value);
	value=NULL;
		
	if(nmax == -1)goto ErrorOut;
	
	if(SageGetFileItemFnt(Files,"cell_level",0L,&level,&length,pio->CurrentFrame)){
	    ret=13;goto ErrorOut;
	}
	
	k=(long)(level[nmax]);    
	    
	if(level)cFree((char *)level);
	level=NULL;

	if(SageGetFileItemFnt(Files,"cell_center(1)",0L,&xcenter,&length,pio->CurrentFrame)){
	    ret=11;goto ErrorOut;
	}

	pio->plane.Plane_xloc=xcenter[nmax];
	pio->plane.Plane_xmin=xcenter[nmax]-40*pio->dx2[k];
	pio->plane.Plane_xmax=xcenter[nmax]+40*pio->dx2[k];
	
	if(xcenter)cFree((char *)xcenter);
	xcenter=NULL;
	
	if(SageGetFileItemFnt(Files,"cell_center(2)",0L,&ycenter,&length,pio->CurrentFrame)){
	    ret=12;goto ErrorOut;
	}

	pio->plane.Plane_yloc=ycenter[nmax];
	pio->plane.Plane_ymin=ycenter[nmax]-40*pio->dy2[k];
	pio->plane.Plane_ymax=ycenter[nmax]+40*pio->dy2[k];
	
	
	if(ycenter)cFree((char *)ycenter);
	ycenter=NULL;
	
	if(SageGetFileItemFnt(Files,"cell_center(3)",0L,&zcenter,&length,pio->CurrentFrame)){
	    ret=13;goto ErrorOut;
	}

	pio->plane.Plane_zloc=zcenter[nmax];
	pio->plane.Plane_zmin=zcenter[nmax]-40*pio->dz2[k];
	pio->plane.Plane_zmax=zcenter[nmax]+40*pio->dz2[k];

	if(zcenter)cFree((char *)zcenter);
	zcenter=NULL;
	
    if(pio->plane.Plane_type == X_PLANE){
        pio->range.xminData=pio->plane.Plane_ymin;
        pio->range.xmaxData=pio->plane.Plane_ymax;
        pio->range.yminData=pio->plane.Plane_zmin;
        pio->range.ymaxData=pio->plane.Plane_zmax;
    }else if(pio->plane.Plane_type == Y_PLANE){
        pio->range.xminData=pio->plane.Plane_xmin;
        pio->range.xmaxData=pio->plane.Plane_xmax;
        pio->range.yminData=pio->plane.Plane_zmin;
        pio->range.ymaxData=pio->plane.Plane_zmax;
    }else if(pio->plane.Plane_type == Z_PLANE){
        pio->range.xminData=pio->plane.Plane_xmin;
        pio->range.xmaxData=pio->plane.Plane_xmax;
        pio->range.yminData=pio->plane.Plane_ymin;
        pio->range.ymaxData=pio->plane.Plane_ymax;
    }
	
	ret=0;
				
ErrorOut:

	if(xcenter)cFree((char *)xcenter);
	if(ycenter)cFree((char *)ycenter);
	if(zcenter)cFree((char *)zcenter);
	if(daughter)cFree((char *)daughter);
	if(value)cFree((char *)value);
	if(level)cFree((char *)level);

	return ret;
}
static int doSDS3D(struct FileInfo2 *Files)
{
	static  unsigned char file[256]={'I','m','a','g','e','.','s','3','d',0};
	char *namef,name[1024];
	char buff[256];
	struct ImageSaveList *Images;
	char limits[1024];
	static struct FileListInfo info;
	IconPtr StatusWindow;
	double *sout;
	struct FilePIOInfo *pio;
 	long n,ni,nf;
	int ret;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	
	nf=0;
	ret=1;
	sout=NULL;
	
	StatusWindow=NULL;
	
	Images=SaveAllImages("Save Selected SDS3D Images",Files);
	if(!Images)return 1;
	if(GetTheSaveFileName((char *)file,"Save SDS3D File",256,&info))goto OutOfHere;
	    	    
	StatusWindow=uStartStatusWindow(NULL,"SDS3D");
	    
	for(n=0;n<Images->count;++n){
		int rank,size[3];
			
	    ni=Images->imageList[n];
	    if((namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,1024);
	    }else{
			sprintf(name,"%s%c",file,0);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    
	    rank=3;
		size[0]=(int)pio->plane.Plane_zcount;
		size[1]=(int)pio->plane.Plane_ycount;
		size[2]=(int)pio->plane.Plane_xcount;
		
		pio->CurrentFrame=ni;
		    					    
		sout=buildSDS3D(Files);
		if(!sout)goto OutOfHere;
			        
    	FilePutCommandBuff(limits,
	        pio->plane.Plane_xmin,pio->plane.Plane_xmax,
	        pio->plane.Plane_ymin,pio->plane.Plane_ymax,
	        pio->plane.Plane_zmin,pio->plane.Plane_zmax,
	        pio->plane.Plane_dmin,pio->plane.Plane_dmax,
	        0,
	        (int)pio->pioIndex,
	        (unsigned char *)pio->pioName,
	        pio->sageBlk.time);
	        
	      	    
		if(sdsOut((char *)file,name,limits,rank,size,sout,nf++))goto OutOfHere;
		
		if(sout)cFree((char *)sout);
		sout=NULL;

	    if(uCheckStatusWindow(StatusWindow,"Stop Writing SDS3D ?"))goto OutOfHere;
	}
	
	SetFileType((char *)file,"SDS3");
	
	ret=0;
OutOfHere:

	uStopStatusWindow(StatusWindow);
	
	if(Images)ImageFreeList(Images);
	
    if(sout)cFree((char *)sout);
	
	if(ret){
	    WarningBatch("doSDS3D Error Writing Files\n");
	}
	
	return ret;	 
	
}
static int doWrite2Dplanes(struct FileInfo2 *Files)
{
	static  unsigned char file[256]={'I','m','a','g','e','.','s','2','d',0};
	static struct FileListInfo info;
	struct SetFrameData sd;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dmin,dmax;
	double *sout,*sout2;
	long n,length,nn;
	struct FilePIOInfo *pio;
	int ret;
    char name[256],limits[1024];
	int rank,size[2];
	
	if(!Files)return 1;
	pio=&Files->pioData;
	
	ret=1;
	sout=NULL;
	sout2=NULL;
	if(GetTheSaveFileName((char *)file,"SDS2D Name",256,&info))goto OutOfHere;
	
	sd.xsize = pio->plane.Plane_xcount;
	sd.ysize = pio->plane.Plane_ycount;
	sd.zsize = pio->plane.Plane_zcount;

	xmin=pio->plane.Plane_xmin;
	xmax=pio->plane.Plane_xmax;

	ymin=pio->plane.Plane_ymin;
	ymax=pio->plane.Plane_ymax;

	zmin=pio->plane.Plane_zmin;
	zmax=pio->plane.Plane_zmax;
	
	
	sout=buildSDS3D(Files);
	if(!sout)goto OutOfHere;
	
	if(pio->plane.Plane_type == X_PLANE){
	     double dx;
	     
	     dx=(xmax-xmin)/(double)sd.xsize;
	
		length=sd.ysize*sd.zsize;
		sout2=(double *)cMalloc(length*sizeof(double),8658);
		if(!sout2)goto OutOfHere;

	
	    for(n=0;n<sd.xsize;++n){
			long i,j;
	    
	    	sprintf(name,"x-plane(%9.2e)",((double)n+0.5)*dx+xmin);
	    	
	    	dmin =  1e60;
			dmax = -1e60;
			
			
			for(j=0;j<sd.zsize;++j){
			    for(i=0;i<sd.ysize;++i){
				    double value;
				    value=sout[n+j*sd.xsize*sd.ysize+i*sd.xsize];
				    sout2[i+(sd.zsize-1-j)*sd.ysize]=value;
				    if(value < dmin)dmin=value;
				    if(value > dmax)dmax=value;
			    }
			}
								        
	    FilePutCommandBuff(limits,
			        ymin,ymax,
			        zmin,zmax,
			        xmin,xmax,
			        dmin,dmax,
			        0,
			        (int)pio->pioIndex,
			        (unsigned char *)pio->pioName,
			        pio->sageBlk.time);
		        
		        
		        
	    	rank=2;
			size[0]=(int)sd.zsize;
			size[1]=(int)sd.ysize;
			
			if(sdsOut((char *)file,name,limits,rank,size,sout2,n))goto OutOfHere;
			
			
	    }
		SetFileType((char *)file,"SDS2");
	}else if(pio->plane.Plane_type == Y_PLANE){
	     double dy;
	     
	     dy=(ymax-ymin)/(double)sd.ysize;
	
		length=sd.xsize*sd.zsize;
		sout2=(double *)cMalloc(length*sizeof(double),8659);
		if(!sout2)goto OutOfHere;

	
	    for(n=0;n<sd.ysize;++n){
			long i,j;
	    
	    	sprintf(name,"y-plane(%9.2e)",((double)n+0.5)*dy+ymin);
	    	
	    	dmin =  1e60;
			dmax = -1e60;
			
			
			for(j=0;j<sd.zsize;++j){
			    for(i=0;i<sd.xsize;++i){
				    double value;
				    value=sout[i+j*sd.xsize*sd.ysize+n*sd.xsize];
				    sout2[i+(sd.zsize-1-j)*sd.xsize]=value;
				    if(value < dmin)dmin=value;
				    if(value > dmax)dmax=value;
			    }
			}
					        
		        
	    FilePutCommandBuff(limits,
			        ymin,ymax,
			        zmin,zmax,
			        xmin,xmax,
			        dmin,dmax,
			        0,
			        (int)pio->pioIndex,
			        (unsigned char *)pio->pioName,
			        pio->sageBlk.time);
		        
		        
		        
	    	rank=2;
			size[0]=(int)sd.zsize;
			size[1]=(int)sd.xsize;
			
			if(sdsOut((char *)file,name,limits,rank,size,sout2,n))goto OutOfHere;
			
	    }
		SetFileType((char *)file,"SDS2");
	}else if(pio->plane.Plane_type == Z_PLANE){
	
	     double dz;
	     
	     dz=(zmax-zmin)/(double)sd.zsize;
	
		length=sd.xsize*sd.ysize;
		sout2=(double *)cMalloc(length*sizeof(double),8660);
		if(!sout2)goto OutOfHere;

	
	    for(n=0;n<sd.zsize;++n){
			long i,j;
	    
	    	sprintf(name,"z-plane(%9.2e)",((double)n+0.5)*dz+zmin);
	    	
	    	dmin =  1e60;
			dmax = -1e60;
			
			nn=0;
			
			for(j=0;j<sd.ysize;++j){
			    for(i=0;i<sd.xsize;++i){
				    double value;
				    value=sout[n*sd.xsize*sd.ysize+nn++];
				    sout2[i+(sd.ysize-1-j)*sd.xsize]=value;
				    if(value < dmin)dmin=value;
				    if(value > dmax)dmax=value;
			    }
			}
						
		        
	    FilePutCommandBuff(limits,
			        ymin,ymax,
			        zmin,zmax,
			        xmin,xmax,
			        dmin,dmax,
			        0,
			        (int)pio->pioIndex,
			        (unsigned char *)pio->pioName,
			        pio->sageBlk.time);
		      		        
		        
	    	rank=2;
			size[0]=(int)sd.ysize;
			size[1]=(int)sd.xsize;
			
			if(sdsOut((char *)file,name,limits,rank,size,sout2,n))goto OutOfHere;
			
	    }
		SetFileType((char *)file,"SDS2");
	}else{
	    goto OutOfHere;
	}
	
	ret=0;
OutOfHere:
    if(sout)cFree((char *)sout);
    if(sout2)cFree((char *)sout2);
	return ret;	 
}
static int SendRange(struct FileInfo2 *Files,uDialogReturn *uR)
{
	struct Message1000 m;

	if(!Files)return 1;

    if(uTestRange(uR))return 1;
		
	m.MaintainAspectRatio=(int)uR[12].lreturn;
	m.ixmax=uR[2].lreturn;
	m.iymax=uR[3].lreturn;
	m.xminData=uR[4].dreturn;
	m.xmaxData=uR[6].dreturn;
	m.yminData=uR[5].dreturn;
	m.ymaxData=uR[7].dreturn;
	m.ixminData=uR[8].lreturn;
	m.ixmaxData=uR[10].lreturn;
	m.iyminData=uR[9].lreturn;
	m.iymaxData=uR[11].lreturn;
	
	SendMessageToAll(NULL,0,MessageType_LIMITS,(void *)&m);
	return 0;
}
static int doVectorSelection(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {160,240,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {250,240,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"vspace data", {255,20,70,20},uDialogText,uDialogGetDouble},
       /*  3 */      {"vfac-data", {255,45,70,20},uDialogText,uDialogGetDouble},
       /*  4 */      {"vmax-data", {255,70,70,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"arrow length - data", {255,95,70,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"arrow angle data", {255,120,70,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"Draw Vectors", {20,20,110,20},uDialogCheck,uDialogGetLong},
       /*  8 */      {"No Head", {20,70,100,20},uDialogButtonRadio,uDialogGetLong},
       /*  9 */      {"One side", {20,95,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 10 */      {"Both Sides", {20,120,100,20},uDialogButtonRadio,uDialogGetLong},
       /* 11 */      {"Arrow Angle ", {130,120,110,20},uDialogString,uDialogNone},
       /* 12 */      {"Vector Factor ", {130,45,110,20},uDialogString,uDialogNone},
       /* 13 */      {"Vector Maximum", {130,70,120,20},uDialogString,uDialogNone},
       /* 14 */      {"Arrow Length", {130,95,110,20},uDialogString,uDialogNone},
       /* 15 */      {"Vector Spacing ", {130,20,110,20},uDialogString,uDialogNone},
       /* 16 */      {"V1", {110,145,40,20},uDialogString,uDialogNone},
       /* 17 */      {"vectorx", {155,145,170,20},uDialogText,uDialogNone},
       /* 18 */      {"V2", {110,170,40,20},uDialogString,uDialogNone},
       /* 19 */      {"vectory", {155,170,170,20},uDialogText,uDialogNone},
       /* 20 */      {"V3", {110,195,40,20},uDialogString,uDialogNone},
       /* 21 */      {"vectorz", {155,195,170,20},uDialogText,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,340,320};
 	struct PlotVectorData *vector;
 	uDialogList *uList;
	uDialogReturn *uR;
    
    int reti;
    int ret;
    
	if(!Files)return 1;
	vector=&Files->pioData.vector;
	if(!vector)return 1;
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Vectors",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,2L,
		          uDialogSetDouble,(vector->vspace),
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(vector->vfac),
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(vector->vmax),
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,vector->arrowLength,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,vector->arrowAngle,
	              
	              uDialogSetItem,7L,
	              uDialogSetLogical,(vector->DrawVectors == 1),
	              
	              uDialogSetItem,8L,
	              uDialogSetLogical,(vector->arrowHeads == 0),
	              
	              uDialogSetItem,9L,
	              uDialogSetLogical,(vector->arrowHeads == 1),
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(vector->arrowHeads == 2),

	              uDialogSetItem,17L,
	              uDialogSetText,vector->vectorxName,
	              
	              uDialogSetItem,18L,
	          	  uDialogSetHidden,(Files->fileClass == PIO1D_FILES), 
	          	  
	              uDialogSetItem,19L,	              
	          	  uDialogSetHidden,(Files->fileClass == PIO1D_FILES), 
	              uDialogSetText,vector->vectoryName,
	              
	              uDialogSetItem,20L,
	          	  uDialogSetHidden,(Files->fileClass == PIO1D_FILES || Files->fileClass == PIO2D_FILES), 
	          	  
	              uDialogSetItem,21L,
	          	  uDialogSetHidden,(Files->fileClass == PIO1D_FILES || Files->fileClass == PIO2D_FILES), 
	              uDialogSetText,vector->vectorzName,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;


    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	vector->vspace=uR[2].dreturn;
	vector->vfac=uR[3].dreturn;
	vector->vmax= uR[4].dreturn;
	vector->arrowLength= uR[5].dreturn;
	vector->arrowAngle=  uR[6].dreturn;
	vector->DrawVectors=  (int)uR[7].lreturn;
	vector->arrowHeads=0;
	if(uR[9].lreturn){
		vector->arrowHeads=1;
	}else if(uR[10].lreturn){
		vector->arrowHeads=2;
	}
    
    mstrncpy(vector->vectorxName,uR[17].sreturn,32);
    mstrncpy(vector->vectoryName,uR[19].sreturn,32);
    mstrncpy(vector->vectorzName,uR[21].sreturn,32);

	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doStreamlineSelection(struct PlotStreamData *streamline)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {200,150,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {290,150,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"Draw Streamlines", {20,20,140,20},uDialogCheck,uDialogGetLong},
       /*  3 */      {"Use Gradients", {20,45,140,20},uDialogCheck,uDialogGetLong},
       /*  4 */      {"maxSteps",    {20,95,70,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"Max Steps", {110,95,200,20},uDialogString,uDialogNone},
       /*  6 */      {"maxPasses",    {20,120,70,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"Max Passes", {110,120,200,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,420,210};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
    
    int reti;
    int ret;
    
	if(!streamline)return 1;
	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Streamlines",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,2L,
	              uDialogSetLogical,(streamline->streamOn == 1),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(streamline->streamGradients == 1),
	              
	              uDialogSetItem,4L,
		          uDialogSetDouble,(double)streamline->streamSteps,
	              
	              uDialogSetItem,6L,
		          uDialogSetDouble,(double)streamline->streamPasses,
	              

	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;


    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	streamline->streamOn = (int)uR[2].lreturn;
	streamline->streamGradients = (int)uR[3].lreturn;
	streamline->streamSteps = (int)uR[4].dreturn;
	streamline->streamPasses = (int)uR[6].dreturn;

	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doRange3Action(struct uDialogStruct *uList)
{
 	uDialogReturn *uR;
 	double dx,dy,dz;
 	long ix,iy,iz;
 	int ret;
	
	if(!uList)return 1;
	
    uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 17){
	
		uR=uDialogGetReturn(uList);
		
	    if(!uR)goto OutOfHere;

		dx=uR[14].dreturn-uR[11].dreturn;
		dy=uR[15].dreturn-uR[12].dreturn;
		dz=uR[16].dreturn-uR[13].dreturn;
		ix=uR[2].lreturn;
		iy=uR[3].lreturn;
		iz=uR[4].lreturn;
		if(ix > iy){
		    if(ix > iz){
			    if(dx){
			        iy=(long)((double)ix*dy/dx+0.5);
			        iz=(long)((double)ix*dz/dx+0.5);
			    }
		    }else{
		    }
		}else if(iy > iz){
		    if(dy){
		        ix=(long)((double)iy*dx/dy+0.5);
		        iz=(long)((double)iy*dz/dy+0.5);
		    }
		}else{
		    if(dz){
		        ix=(long)((double)iz*dx/dz+0.5);
		        iy=(long)((double)iz*dy/dz+0.5);
		    }
		}
		
		
		
		ret=uDialogSet(uList,2L,
	              uDialogSetDouble,(double)ix,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)iy,
	              	              	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)iz,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
 	
		if(uList)uDialogUpdate(uList);
	}
OutOfHere:
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int doVolumeSelection(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {250,270,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {340,270,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"ixmaxScr", {80,05,110,20},uDialogText,uDialogGetLong},
       /*  3 */      {"iymaxSc", {280,05,110,20},uDialogText,uDialogGetLong},
       /*  4 */      {"izmaxSc", {80,29,110,20},uDialogText,uDialogGetLong},
       
       /*  5 */      {"xmin data", {280,78,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"ymin data", {280,103,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"xmax data", {280,128,110,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"ymax data", {280,153,110,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"xmax data", {280,178,110,20},uDialogText,uDialogGetDouble},
       /*  10 */     {"ymax data", {280,203,110,20},uDialogText,uDialogGetDouble},
       
       /* 11 */      {"ixmindata", {80,78,110,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"iymindata", {80,103,110,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"ixmaxdata", {80,128,110,20},uDialogText,uDialogGetDouble},
       /* 14 */      {"iymaxdata", {80,153,110,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"ixmaxdata", {80,178,110,20},uDialogText,uDialogGetDouble},
       /* 16 */      {"iymaxdata", {80,203,110,20},uDialogText,uDialogGetDouble},
       
       /* 17 */      {"Adjust", {220,240,110,20},uDialogButton,uDialogNone},
       
       /* 18 */      {"xmin", {220,78,50,20},uDialogString,uDialogNone},
       /* 19 */      {"ymin", {220,103,50,20},uDialogString,uDialogNone},
       /* 20 */      {"zmin", {220,128,50,20},uDialogString,uDialogNone},
       /* 21 */      {"xmax", {220,153,50,20},uDialogString,uDialogNone},
       /* 22 */      {"ymax", {220,178,50,20},uDialogString,uDialogNone},
       /* 23 */      {"zmax", {220,203,50,20},uDialogString,uDialogNone},
       
       
       /* 24 */      {"xmin", {20,78,50,20},uDialogString,uDialogNone},
       /* 25 */      {"ymin", {20,104,50,20},uDialogString,uDialogNone},
       /* 26 */      {"zmin", {20,132,50,20},uDialogString,uDialogNone},
       /* 27 */      {"xmax", {20,153,50,20},uDialogString,uDialogNone},
       /* 28 */      {"ymax", {20,178,50,20},uDialogString,uDialogNone},
       /* 29 */      {"zmax", {20,203,50,20},uDialogString,uDialogNone},
       
       
       /* 30 */      {"xsize", {20,05,50,20},uDialogString,uDialogNone},
       /* 31 */      {"ysize", {220,05,50,20},uDialogString,uDialogNone},
       /* 32 */      {"zsize", {20,29,50,20},uDialogString,uDialogNone},
       
       /* 33 */      {"Plot Domain", {80,55,100,20},uDialogString,uDialogNone},
       /* 34 */      {"Data Domain", {280,56,100,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,430,310};

    struct FilePIOInfo *pio;
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
	if(!Files)return 1;
	pio=&Files->pioData;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Volume Selection",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(pio->plane.Plane_xmin < Files->xminr)pio->plane.Plane_xmin = Files->xminr;
	if(pio->plane.Plane_ymin < Files->yminr)pio->plane.Plane_ymin = Files->yminr;
	if(pio->plane.Plane_zmin < Files->zminr)pio->plane.Plane_zmin = Files->zminr;
	
	if(pio->plane.Plane_xmax > Files->xmaxr)pio->plane.Plane_xmax = Files->xmaxr;
	if(pio->plane.Plane_ymax > Files->ymaxr)pio->plane.Plane_ymax = Files->ymaxr;
	if(pio->plane.Plane_zmax > Files->zmaxr)pio->plane.Plane_zmax = Files->zmaxr;
	

	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)pio->plane.Plane_xcount,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)pio->plane.Plane_ycount,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)pio->plane.Plane_zcount,
	              
	              uDialogSetItem,5L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->xminr,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->yminr,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->zminr,
	              
	              uDialogSetItem,8L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->xmaxr,
	              
	              uDialogSetItem,9L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->ymaxr,
	              
	              uDialogSetItem,10L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->zmaxr,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)pio->plane.Plane_xmin,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,(double)pio->plane.Plane_ymin,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,(double)pio->plane.Plane_zmin,
	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,(double)pio->plane.Plane_xmax,
	             	              	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,(double)pio->plane.Plane_ymax,
	              
	              uDialogSetItem,16L,
	              uDialogSetDouble,(double)pio->plane.Plane_zmax,
	             	              	              
	              uDialogSetAction,doRange3Action,
	                            
	              uDialogSetMyIcon,Files,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
	if(uR[2].lreturn <= 0 || uR[3].lreturn <= 0 || uR[4].lreturn <= 0){
	    Warning("Sizes must be greater then zero");
	    goto Start;
	}
	
	if(uR[14].dreturn <= uR[11].dreturn){
	    Warning("xmax must be greater then xmin");
	    goto Start;
	}
	
	if(uR[15].dreturn <= uR[12].dreturn){
	    Warning("ymax must be greater then ymin");
	    goto Start;
	}
	
	if(uR[16].dreturn <= uR[13].dreturn){
	    Warning("zmax must be greater then zmin");
	    goto Start;
	}
	
	pio->plane.Plane_xcount=(int)uR[2].lreturn;
	pio->plane.Plane_ycount=(int)uR[3].lreturn;
	pio->plane.Plane_zcount=(int)uR[4].lreturn;
			
	pio->plane.Plane_xmin=uR[11].dreturn;
	pio->plane.Plane_ymin=uR[12].dreturn;
	pio->plane.Plane_zmin=uR[13].dreturn;
	
	
	pio->plane.Plane_xmax=uR[14].dreturn;
	pio->plane.Plane_ymax=uR[15].dreturn;
	pio->plane.Plane_zmax=uR[16].dreturn;
	
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	return ret;
}
static int doRange4Action(struct uDialogStruct *uList)
{
 	uDialogReturn *uR;
 	double dxx,dyy,dzz;
 	double dx,dy;
 	long ix,iy;
 	long ixs,iys;
 	int ret;
	
	if(!uList)return 1;
	
    uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 22){
	
		uR=uDialogGetReturn(uList);
		
	    if(!uR)goto OutOfHere;

		dxx=uR[13].dreturn-uR[10].dreturn;
		dyy=uR[14].dreturn-uR[11].dreturn;
		dzz=uR[15].dreturn-uR[12].dreturn;
		dx=sqrt(dxx*dxx+dyy*dyy+dzz*dzz);
		dxx=uR[16].dreturn-uR[10].dreturn;
		dyy=uR[17].dreturn-uR[11].dreturn;
		dzz=uR[18].dreturn-uR[12].dreturn;
		dy=sqrt(dxx*dxx+dyy*dyy+dzz*dzz);
		ix=uR[2].lreturn;
		iy=uR[3].lreturn;
		ixs=0;
		iys=0;
		
	    if(dy){
	        ixs=(long)((double)iy*dx/dy+0.5);
	    }
	    
	    if(dx){
		    iys=(long)((double)ix*dy/dx+0.5);
	    }
	    
	    if(iys > iy){
	       iy=iys;
	    }else if(ixs > ix){
	       ix=ixs;
	    }
		
		ret=uDialogSet(uList,2L,
	              uDialogSetDouble,(double)ix,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)iy,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
		
		if(uList)uDialogUpdate(uList);
 	
	}
OutOfHere:
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int doGeneralSelection(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {250,270,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {340,270,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"ixmaxScr", {80,05,85,20},uDialogText,uDialogGetLong},
       /*  3 */      {"iymaxSc", {280,05,85,20},uDialogText,uDialogGetLong},
       
       /*  4 */      {"xmin data", {280,78,110,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"ymin data", {280,103,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"xmax data", {280,128,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"ymax data", {280,153,110,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"xmax data", {280,178,110,20},uDialogText,uDialogGetDouble},
       /*  9 */     {"ymax data", {280,203,110,20},uDialogText,uDialogGetDouble},
       
       /* 10 */      {"x1", {80,78,110,20},uDialogText,uDialogGetDouble},
       /* 11 */      {"y1", {80,103,110,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"z1", {80,128,110,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"x2", {80,153,110,20},uDialogText,uDialogGetDouble},
       /* 14 */      {"y2", {80,178,110,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"z2", {80,203,110,20},uDialogText,uDialogGetDouble},
       /* 16 */      {"x3", {80,228,110,20},uDialogText,uDialogGetDouble},
       /* 17 */      {"y3", {80,253,110,20},uDialogText,uDialogGetDouble},
       /* 18 */      {"zz", {80,278,110,20},uDialogText,uDialogGetDouble},
       
       /* 19 */      {"gmin", {80,303,110,20},uDialogText,uDialogGetDouble},
       /* 20 */      {"gmax", {80,328,110,20},uDialogText,uDialogGetDouble},
       /* 21 */      {"gloc", {80,353,110,20},uDialogText,uDialogGetDouble},
       
       /* 22 */      {"Adjust", {220,240,110,20},uDialogButton,uDialogNone},
       
       /* 23 */      {"xmin", {220,78,50,20},uDialogString,uDialogNone},
       /* 24 */      {"ymin", {220,103,50,20},uDialogString,uDialogNone},
       /* 25 */      {"zmin", {220,128,50,20},uDialogString,uDialogNone},
       /* 26 */      {"xmax", {220,153,50,20},uDialogString,uDialogNone},
       /* 27 */      {"ymax", {220,178,50,20},uDialogString,uDialogNone},
       /* 28 */      {"zmax", {220,203,50,20},uDialogString,uDialogNone},
       
       
       /* 29 */      {"x1", {20,78,50,20},uDialogString,uDialogNone},
       /* 30 */      {"y1", {20,103,50,20},uDialogString,uDialogNone},
       /* 31 */      {"z1", {20,128,50,20},uDialogString,uDialogNone},
       /* 32 */      {"x2", {20,153,50,20},uDialogString,uDialogNone},
       /* 33 */      {"y2", {20,178,50,20},uDialogString,uDialogNone},
       /* 34 */      {"z2", {20,203,50,20},uDialogString,uDialogNone},
       
       /* 35 */      {"x3", {20,228,50,20},uDialogString,uDialogNone},
       /* 36 */      {"y3", {20,253,50,20},uDialogString,uDialogNone},
       /* 37 */      {"z3", {20,278,50,20},uDialogString,uDialogNone},
       
       /* 38 */      {"gmin", {20,303,50,20},uDialogString,uDialogNone},
       /* 39 */      {"gmax", {20,328,50,20},uDialogString,uDialogNone},
       /* 40 */      {"gloc", {20,353,50,20},uDialogString,uDialogNone},
       
       /* 41 */      {"xsize", {20,05,50,20},uDialogString,uDialogNone},
       /* 42 */      {"ysize", {220,05,50,20},uDialogString,uDialogNone},
       
       /* 43 */      {"Plot Domain", {80,55,100,20},uDialogString,uDialogNone},
       /* 44 */      {"Data Domain", {280,56,100,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,430,400};

    struct FilePIOInfo *pio;
 	uDialogList *uList;
 	uDialogReturn *uR;
 	double dd,dxx,dyy,dzz;
    int ret;
	if(!Files)return 1;
	pio=&Files->pioData;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("General Area Selection",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
/*	
	if(pio->plane.Plane_gx1 < Files->xminr)pio->plane.Plane_gx1 = Files->xminr;
	if(pio->plane.Plane_gx2 < Files->xminr)pio->plane.Plane_gx2 = Files->xminr;
	if(pio->plane.Plane_gx3 < Files->xminr)pio->plane.Plane_gx3 = Files->xminr;

	if(pio->plane.Plane_gy1 < Files->yminr)pio->plane.Plane_gy1 = Files->yminr;
	if(pio->plane.Plane_gy2 < Files->yminr)pio->plane.Plane_gy2 = Files->yminr;
	if(pio->plane.Plane_gy3 < Files->yminr)pio->plane.Plane_gy3 = Files->yminr;

	if(pio->plane.Plane_gz1 < Files->zminr)pio->plane.Plane_gz1 = Files->zminr;
	if(pio->plane.Plane_gz2 < Files->zminr)pio->plane.Plane_gz2 = Files->zminr;
	if(pio->plane.Plane_gz3 < Files->zminr)pio->plane.Plane_gz3 = Files->zminr;
	
	if(pio->plane.Plane_gx1 > Files->xmaxr)pio->plane.Plane_gx1 = Files->xmaxr;
	if(pio->plane.Plane_gx2 > Files->xmaxr)pio->plane.Plane_gx3 = Files->xmaxr;
	if(pio->plane.Plane_gx3 > Files->xmaxr)pio->plane.Plane_gx3 = Files->xmaxr;
	
	if(pio->plane.Plane_gy1 > Files->ymaxr)pio->plane.Plane_gy1 = Files->ymaxr;
	if(pio->plane.Plane_gy2 > Files->ymaxr)pio->plane.Plane_gy2 = Files->ymaxr;
	if(pio->plane.Plane_gy3 > Files->ymaxr)pio->plane.Plane_gy3 = Files->ymaxr;

	if(pio->plane.Plane_gz1 > Files->zmaxr)pio->plane.Plane_gz1 = Files->zmaxr;
	if(pio->plane.Plane_gz2 > Files->zmaxr)pio->plane.Plane_gz2 = Files->zmaxr;
	if(pio->plane.Plane_gz3 > Files->zmaxr)pio->plane.Plane_gz3 = Files->zmaxr;
*/	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)pio->plane.Plane_xcount,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)pio->plane.Plane_ycount,
	              	              
	              uDialogSetItem,4L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->xminr,
	              
	              uDialogSetItem,5L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->yminr,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->zminr,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->xmaxr,
	              
	              uDialogSetItem,8L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->ymaxr,
	              
	              uDialogSetItem,9L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->zmaxr,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)pio->plane.Plane_gx1,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)pio->plane.Plane_gy1,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,(double)pio->plane.Plane_gz1,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,(double)pio->plane.Plane_gx2,
	             	              	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,(double)pio->plane.Plane_gy2,
	              
	              uDialogSetItem,15L,
	              uDialogSetDouble,(double)pio->plane.Plane_gz2,
	             	              	              
	              uDialogSetItem,16L,
	              uDialogSetDouble,(double)pio->plane.Plane_gx3,
	             	              	              
	              uDialogSetItem,17L,
	              uDialogSetDouble,(double)pio->plane.Plane_gy3,
	             	              	              
	              uDialogSetItem,18L,
	              uDialogSetDouble,(double)pio->plane.Plane_gz3,
	             	              	              
	              uDialogSetItem,19L,
	              uDialogSetDouble,(double)pio->plane.Plane_gmin,
	             	              	              
	              uDialogSetItem,20L,
	              uDialogSetDouble,(double)pio->plane.Plane_gmax,
	             	              	              
	              uDialogSetItem,21L,
	              uDialogSetDouble,(double)pio->plane.Plane_gloc,
	             	              	              
	              uDialogSetAction,doRange4Action,
	                            
	              uDialogSetMyIcon,Files,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	
	if(uR[2].lreturn <= 0 || uR[3].lreturn <= 0){
	    Warning("Sizes must be greater then zero");
	    goto Start;
	}
	
	
	
	dxx=uR[13].dreturn-uR[10].dreturn;
	dyy=uR[14].dreturn-uR[11].dreturn;
	dzz=uR[15].dreturn-uR[12].dreturn;
	dd=sqrt(dxx*dxx+dyy*dyy+dzz*dzz);
	
	
	if(dd <= 0){
	    Warning("v1 must be longer then zero");
	    goto Start;
	}
	dxx=uR[16].dreturn-uR[10].dreturn;
	dyy=uR[17].dreturn-uR[11].dreturn;
	dzz=uR[18].dreturn-uR[12].dreturn;
	dd=sqrt(dxx*dxx+dyy*dyy+dzz*dzz);
	
	if(dd <= 0){
	    Warning("v2 must be longer then zero");
	    goto Start;
	}
	
	
	pio->plane.Plane_xcount=(int)uR[2].lreturn;
	pio->plane.Plane_ycount=(int)uR[3].lreturn;
			
			
	pio->plane.Plane_gx1=uR[10].dreturn;
	pio->plane.Plane_gy1=uR[11].dreturn;
	pio->plane.Plane_gz1=uR[12].dreturn;
	
	pio->plane.Plane_gx2=uR[13].dreturn;
	pio->plane.Plane_gy2=uR[14].dreturn;
	pio->plane.Plane_gz2=uR[15].dreturn;
	
	pio->plane.Plane_gx3=uR[16].dreturn;
	pio->plane.Plane_gy3=uR[17].dreturn;
	pio->plane.Plane_gz3=uR[18].dreturn;
	
	pio->plane.Plane_gmin=uR[19].dreturn;
	pio->plane.Plane_gmax=uR[20].dreturn;
	pio->plane.Plane_gloc=uR[21].dreturn;
			
	pio->plane.Plane_type=G_PLANE;
				
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	return ret;
}
static int doSage3DDoDialog(struct FileInfo2 *Files,int *type)
{
	if(!Files || !type)return 1;
	
	if(*type == MessageType_DO_PLANE_DIALOG){
		return doDataPlane(Files);	
	}else if(*type == MessageType_DO_SCALES_DIALOG){
		return doDataScale(Files);	
	}else if(*type == MessageType_DO_RANGE_DIALOG){
		return doDataRange(Files);	
	}else if(*type == MessageType_DO_SELECTION_DIALOG){
		return doDataSelection(Files);	
	}else if(*type == MessageType_DO_VECTOR_DIALOG){
		return doVectorSelection(Files);	
	}else if(*type == MessageType_DO_AREA_DIALOG){
		return doVolumeSelection(Files);	
	}else if(*type == MessageType_DO_STREAMLINE_DIALOG){
		return doStreamlineSelection(&Files->pioData.streamline3d);
	}else if(*type == MessageType_DO_GENERAL_DIALOG){
		return doGeneralSelection(Files);	
	}
	return 0;
}

