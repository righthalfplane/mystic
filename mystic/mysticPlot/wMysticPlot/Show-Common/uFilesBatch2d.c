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
#include "Streamline.h"
#include "uGraph.h"
#include "uFilesBatch2d.h"
#include "Universal.h"
#include "mThread.h"

struct DrawData{
	struct FilePIOInfo *pio;
	struct screenData *ss;
	unsigned char *sout;
	double *soutd;
	struct dRange *r;
	double *lambda;
	double *t;
	double **l351;
	double **l951;
	int numGroup;
	int *processed;
	struct linedata *li;
	struct linedata *lii;

};

static int doIntegrateLineBhangMeter2(struct FilePIOInfo *pio,struct linedata *li);

static int doBhangMeterGetData(struct FileInfo2 *Files,struct linedata *li);

static int doIntegrateLineBhangMeter(struct FilePIOInfo *pio,struct linedata *li);

int checkBhangMeterData(struct dataStruct *plane);

int freeBhangMeter2d(struct BhangMeterData *streamline2d);

static int RangeCellVelocity(long n,struct FilePIOInfo *pio,double *vmax);

static int loadVectors(struct FileInfo2 *Files);

static int pioDrawStream2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

int checkStreamData(struct dataStruct *plane);

static int doSage2DCommandGet(struct FileInfo2 *Files,CommandPtr cp);

int freeStreamLine2d(struct PlotStreamData *streamline2d);

static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp);

static int pioDraw2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int getPlotImage2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame);

static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int doSage2DGetAreaDataBlock(struct FileInfo2 *Files,struct areadata *ai);

int doSage2DGetLineDataBlocks(struct FileInfo2 *Files,struct linedata *li);

int setFloat(double *d,long length);

static int doCell(long ne,struct DrawInfo *info);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

int pioDrawTracers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage2DSaveTracerLineData(struct FileInfo2 *Files,int *list,int nc);

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li);

static int doSage2DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int pioDrawStreamlines(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

int pioDrawLasers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage2DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

static int doSage2DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int doSage2DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files);

static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage2DDoDialog(struct FileInfo2 *Files,int *type);

int doSage2D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);


int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);


static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double *v,long *index);

static int doSage2DGetLineData(struct FileInfo2 *Files,struct linedata *li);

int doSage2DStart(struct FileInfo2 *Files);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int SetPIOData(struct FileInfo2 *Files,long CurrentFrame);

static int SageGetGradients2D(struct FileInfo2 *Files,int flag);

static int doAuto(struct FileInfo2 *Files);

static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int setPioScales(struct FilePIOInfo *pio);

static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int doSage2DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int getReflectedData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int doSage2DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int doSage2DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSage2DSetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int getPIOHalfCellSize2D(struct FilePIOInfo *pio,int level,  double size[2]);

static int pioDrawZonesT(mThread *Threads);

static int pioDrawVoidT(mThread *Threads);

static int getPlotDataT(mThread *Threads);

static int pioDrawBhangMeter2dT(mThread *Threads);

static int CalculateBhangMeterMerge(struct DrawData *d);

static int pioGetValueXX(struct FilePIOInfo *pio,double x,double y,double *v,long *index);

int doSage2DBatch(BatchPtr Batch,char *name)
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
    
    if(Files->fileClass != PIO2D_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"doSage2DBatch Found (%s) instead of PIO2D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSage2DBatch Can Only Process PIO2D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
    
    if(doSage2DStart(Files))goto ErrorOut;
		
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSage2D(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}


int doSage2D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
	                        
	struct CommandInfo cpl;
	char line[4096];
	int lines[2000];
    struct FilePIOInfo *pio;
    double xmin,xmax,ymin,ymax;
    long CurrentFrame;
	char *command;
 	double value;
 	int rangeNew;
    int flag;
 	int ret;
 	int n;
    
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));	
	
	xmin=pio->range.xminData;
	ymin=pio->range.yminData;
	xmax=pio->range.xmaxData;
	ymax=pio->range.ymaxData;
	
	rangeNew = FALSE;
	


	ret = 1;
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	if(!mstrcmp("xsize",command)){
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
	}else if(!mstrcmp("plot",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    CurrentFrame=(long)value;
	    if(SetPIOData(Files,CurrentFrame))goto ErrorOut;
	    if(PlotPIOData(Files,CurrentFrame,1))goto ErrorOut;	    
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
	}else if(!mstrcmp("laserRays",command)){
	
		n=0;
	    while(1){
	    	if(BatchNextLine(Batch,line,sizeof(line)))goto ErrorOut;
			if(getCommand(line,&cpl))goto ErrorOut;
			command=stringCommand(&cpl);
			if(command && !mstrcmp("end",command))break;
			for(cpl.n=0;cpl.n<cpl.nword;++cpl.n){
	    	    if(doubleCommand(&value,&cpl))goto ErrorOut;
	    	    if(n < 2000)lines[n++]=(int)value;
			}		
			
	    }
	
		if(n > 0){
		    doSage2DSavePlotLineData(Files,lines,n);
		}
	}else{
		goto ErrorOut;
	}
	
		
	if(rangeNew){
		flag=0;
		if(xmin != pio->range.xminData)flag  = 1;
		if(xmax != pio->range.xmaxData)flag += 2;
		if(ymin != pio->range.yminData)flag += 4;
		if(ymax != pio->range.ymaxData)flag += 8;		
		SetRange(Files,pio->range.xminData,pio->range.xmaxData,pio->range.yminData,pio->range.ymaxData,flagout[flag]);
	}
	ret = 0;
ErrorOut:
 	getCommand(NULL,&cpl);
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
	    if(SetPIOData(Files,n))goto ErrorOut;
	    if(n >= (Files->ImageCount-1))flag=1;
	    if(PlotPIOData(Files,n,flag))goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}
static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;
	unsigned char *buff;
	char filepath[2048];
	char name[256],*namef;
	long length;
	int ret;
	
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	ret=1;
	
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
	
	if(Files->outputType == 0 || Files->outputType == HDF_FILES){ 
	
		getPlotImage(Files,buff,CurrentFrame);
		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    	    
		if(hdfOut(filepath,name,(char *)pio->pd.palette,pio->range.ixmax,pio->range.iymax,(char *)buff,CurrentFrame,flag,limits))goto ErrorOut;

	}else if(Files->outputType == SDS2D_FILES){
	        
	    if(getPlotData(Files,(double *)buff,CurrentFrame))goto ErrorOut;
	    
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    
	    if(sds2DOut(filepath,name,pio->range.ixmax,pio->range.iymax,(double *)buff,CurrentFrame,flag,limits))goto ErrorOut;
	    
	}
	ret = 0;
ErrorOut:
	if(buff)cFree((char *)buff);
	return ret;
}
static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame)
{
	struct PlotRangeData range;
    struct FilePIOInfo *pio;
	struct screenData ss;
	unsigned char *buff2,save;
    double dx,s;
    long dxi,isplit,ihigh,ilow;
    int ret;
    long i,j;
	
	if(!Files || !buff)return 1;
	
	buff2=NULL;
	pio = &Files->pioData;
	
	
	zerol((char *)&ss,sizeof(struct screenData));
	ss.viewRect.x=0;
	ss.viewRect.y=0;
	ss.viewRect.xsize=(int)(pio->range.ixmax);
	ss.viewRect.ysize=(int)(pio->range.iymax);
	ss.buffer=buff;
	ss.ScreenBits=8;
	ss.palette=&pio->pd.palette[0];
	ss.ZoneLevel=pio->pa.ZoneLevel;
	if(pio->pa.DrawZones < 2)ss.ZoneLevel=99999;

    setPioScales(pio);

	range=pio->range;

	if(!Files->Reflect_x || pio->range.xminData >= 0.0){ /* no Reflection or no reflected data showing */
		pioDraw(Files,buff,CurrentFrame,&ss);
	}else if(pio->range.xmaxData <= 0){  /* doing reflected data with no unreflected data showing */
		pio->range.xminData= -range.xmaxData;
		pio->range.xmaxData= -range.xminData;
		ret=pioDraw(Files,buff,CurrentFrame,&ss);
		if(ret)goto ErrorOut;
		for(j=range.iyminData;j<range.iymaxData;++j){
		     ihigh=range.ixmaxData-1;
		     ilow=range.ixminData;
		     for(i=0;i<(range.ixmaxData-range.ixminData)/2;++i){
		          save=buff[ilow+(range.iymax-1-j)*pio->range.ixmax];
		          buff[ilow+(range.iymax-1-j)*pio->range.ixmax]=buff[ihigh+(range.iymax-1-j)*range.ixmax];
		          buff[ihigh+(range.iymax-1-j)*range.ixmax]=save;
		          --ihigh;
		          ++ilow;
		     }
		}
	}else{   /* have some unreflected data showing */
			
		
		dx=pio->range.xmaxData-pio->range.xminData;
		if(dx <= 0)return 1;
		dxi=pio->range.ixmaxData-pio->range.ixminData;
		s=(0.0-pio->range.xminData)/dx;
		isplit=(long)(s*(double)dxi+0.5)+pio->range.ixminData;
		
		if(fabs(pio->range.xminData) <=  pio->range.xmaxData){  /* do unreflected data and flip to negative side */
		
			ss.buffer=buff;			
			pio->range.xminData=0;
			pio->range.ixminData=isplit;
			ret=pioDraw(Files,buff,CurrentFrame,&ss);
			if(ret)goto ErrorOut;			
			
			for(j=0;j< range.iymax;++j){
			     ihigh=isplit;
			     ilow=isplit-1;
			     for(i=0;i<(isplit-range.ixminData);++i){
			          buff[ilow+(range.iymax-1-j)*pio->range.ixmax]=buff[ihigh+(range.iymax-1-j)*range.ixmax];
			          ++ihigh;
			          --ilow;
			     }
			}				
			

		}else{   /* have more reflected data than unreflected data */
			buff2=(unsigned char *)cMalloc(pio->range.ixmax*pio->range.iymax*sizeof(double),1847);
			if(!buff2)goto ErrorOut;
			ss.buffer=buff2;			
			pio->range.xminData=0;
			pio->range.ixminData=range.ixminData;
			pio->range.ixmaxData=isplit;
			pio->range.xmaxData= -range.xminData;
			ret=pioDraw(Files,buff2,CurrentFrame,&ss);
			if(ret)goto ErrorOut;
			
			
			for(j=0;j< range.iymax;++j){
			     ihigh=isplit;
			     ilow=range.ixminData;
			     for(i=0;i<(range.ixmaxData-isplit);++i){
			          buff[ihigh+(range.iymax-1-j)*pio->range.ixmax]=buff2[ilow+(range.iymax-1-j)*range.ixmax];
			          ++ihigh;
			          ++ilow;
			     }
			}			
				
			for(j=0;j< range.iymax;++j){
			     ihigh=isplit-1;
			     ilow=range.ixminData;
			     for(i=0;i<(isplit-range.ixminData);++i){
			          buff[ilow+(range.iymax-1-j)*pio->range.ixmax]=buff2[ihigh+(range.iymax-1-j)*range.ixmax];
			          --ihigh;
			          ++ilow;
			     }
			}			
			
		}
	
	}

ErrorOut:
	pio->range=range;
	pioSetLimits(Files,CurrentFrame);
	if(buff2)cFree((char *)buff2);
	return 0;
}

static int getReflectedData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
	struct PlotRangeData range;
    struct FilePIOInfo *pio;
    double dx,s,save;
    long dxi,isplit,ihigh,ilow;
    int ret;
    long i,j;
    
	if(!Files || !sout)return 1;
	pio = &Files->pioData;

    setPioScales(pio);
	
	range=pio->range;
	
	if(!Files->Reflect_x || pio->range.xminData >= 0.0){  /* No reflection or no reflected data showing */
	    return getPlotData(Files,sout,CurrentFrame);
	}else if(pio->range.xmaxData <= 0){ /* no unreflected data showing */
		pio->range.xminData= -range.xmaxData;
		pio->range.xmaxData= -range.xminData;
		ret=getPlotData(Files,sout,CurrentFrame);
		if(ret)goto ErrorOut;
		for(j=range.iyminData;j<range.iymaxData;++j){
		     ihigh=range.ixmaxData-1;
		     ilow=range.ixminData;
		     for(i=0;i<(range.ixmaxData-range.ixminData)/2;++i){
		          save=sout[ilow+(range.iymax-1-j)*range.ixmax];
		          sout[ilow+(range.iymax-1-j)*pio->range.ixmax]=sout[ihigh+(range.iymax-1-j)*range.ixmax];
		          sout[ihigh+(range.iymax-1-j)*range.ixmax]=save;
		          --ihigh;
		          ++ilow;
		     }
		}
	}else{   /* reflected and unreflected data showing */

		
		dx=pio->range.xmaxData-pio->range.xminData;
		if(dx <= 0)return 1;
		dxi=pio->range.ixmaxData-pio->range.ixminData;
		s=(0.0-pio->range.xminData)/dx;
		isplit=(long)(s*(double)dxi+0.5)+pio->range.ixminData;
		
		
		pio->range.xminData=0;
		pio->range.ixminData=isplit;
		ret=getPlotData(Files,sout,CurrentFrame);  /* get unreflected data */
		if(ret)goto ErrorOut;
		
		pio->range.ixminData=range.ixminData;
		pio->range.ixmaxData=isplit;
		pio->range.xmaxData= -range.xminData;
		ret=getPlotData(Files,sout,CurrentFrame);  /* get reflected data */
		if(ret)goto ErrorOut;
		
		for(j=range.iyminData;j< range.iymaxData;++j){  /* flip reflected data */
		     ihigh=isplit-1;
		     ilow=range.ixminData;
		     for(i=0;i<(isplit-range.ixminData)/2;++i){
		          save=sout[ilow+(range.iymax-1-j)*range.ixmax];
		          sout[ilow+(range.iymax-1-j)*pio->range.ixmax]=sout[ihigh+(range.iymax-1-j)*range.ixmax];
		          sout[ihigh+(range.iymax-1-j)*range.ixmax]=save;
		          --ihigh;
		          ++ilow;
		     }
		}
	 }
	
ErrorOut:
	pio->range=range;
	pioSetLimits(Files,CurrentFrame);    
	return ret;
}
static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct DrawData d;
		
	if(!Files || !sout || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

    setPioScales(pio);
	
	d.pio=pio;
	d.soutd=sout;
	
	pio->threads=8;
	
	runThreads(pio->threads,&d,pio->numcell,getPlotDataT);
	
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	pioSetLimits(Files,CurrentFrame);    
	return 0;
	
}
static int getPlotDataT(mThread *Threads)
{
	struct FilePIOInfo *pio;
	struct DrawData *d;
	double xmin,xmax,ymin,ymax;
	double didx,djdy;
	double dxdi,dydj;
	long ixmin,ixmax,iymin,iymax;
	double *sout;
	long n;		
	
	if(!Threads)goto ErrorOut;
	d=(struct DrawData *)Threads->data;
	if(!d)goto ErrorOut;
	
	pio=d->pio;
	sout=d->soutd;
		
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

	for(n=Threads->smin;n<Threads->smax;++n){
        double xc,yc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double svalue,value,xs,ys;
        long i,j,ihi,ilo,jhi,jlo;
        int k;

        if(pio->daughter[n])continue;
        k=(int)(pio->level[n]);
        
        yc=pio->ycenter[n];
        dy2=pio->dy2[k]*1.000001;

		ymn=max(ymin,yc-dy2);
		ymx=min(ymax,yc+dy2);
		if(ymx < ymn)continue;

        xc=pio->xcenter[n];
        dx2=pio->dx2[k]*1.000001;
        
		xmn=max(xmin,xc-dx2);
		xmx=min(xmax,xc+dx2);
		if(xmx < xmn)continue;
		
		ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
		ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);

		jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
		jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
		
		value=pio->value[n];
		
		for(j=jlo;j<=jhi;++j){
		    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
		    if(ys < ymn || ys > ymx)continue;
		    for(i=ilo;i<=ihi;++i){
		        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
		        if(xs < xmn || xs > xmx)continue;
		        if(pio->doGradients){
		           svalue=value+(xs-xc)*pio->gradx[n]+(ys-yc)*pio->grady[n];
		        }else{
		           svalue=value;
		        }
		        sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=svalue;
		    }
		}

    }
    
ErrorOut:
	Threads->done=TRUE;
	
    return 0;
	        
}
static int pioSetLimits(struct FileInfo2 *Files,long nf)
{
    struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax,xdelta,ydelta;
	long xzones,yzones;
	struct LIMITS *limits;

	if(!Files)return 1;
	pio = &Files->pioData;

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

	limits=NULL;
	FileGetLimits(&limits,nf,Files);

	if(!limits || !limits->iGot)return 1;
	 
	return 0;
}

/*
struct DrawData{
	struct FilePIOInfo *pio;
	struct screenData *ss;
	struct dRange *r;
}
*/

static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r)
{
	struct DrawData d;
	
	if(!pio || !ss || !r || !pio->value)return 1;
	
	d.pio=pio;
	d.ss=ss;
	d.r=r;
	
	runThreads(pio->threads,&d,pio->numcell,pioDrawZonesT);

	return 0;
}
static int pioDrawZonesT(mThread *Threads)
{
	struct FilePIOInfo *pio;
	struct screenData *ss;
	struct dRange *r;
	struct DrawData *d;
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2;
	unsigned char nc;
	int ix1,iy1,ix2,iy2;
	long n;
	int k;
	
	if(!Threads)goto ErrorOut;
	d=(struct DrawData *)Threads->data;
	if(!d)goto ErrorOut;
	
	pio=d->pio;
	ss=d->ss;
	r=d->r;
	
	ix1=0;
	iy1=0;
	ix2=0;
	iy2=0;
	for(n=Threads->smin;n<Threads->smax;++n){
	    k=(int)(pio->level[n]);
	    if( k > ss->ZoneLevel)continue;
	    if((k < ss->ZoneLevel) && pio->daughter[n])continue;
	    nc=255;
	    if(pio->pa.DrawZonesColor){
	        FloatToColor(&pio->value[n],&pio->pd,&nc);
	    }
	    xc=pio->xcenter[n];
	    yc=pio->ycenter[n];
	    dx2=pio->dx2[k];
	    dy2=pio->dy2[k];
	    x1=xc-dx2;
	    y1=yc-dy2;
	    x2=xc+dx2;
	    y2=yc-dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }

	    x1=xc+dx2;
	    y1=yc-dy2;
	    x2=xc+dx2;
	    y2=yc+dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }

	    x1=xc+dx2;
	    y1=yc+dy2;
	    x2=xc-dx2;
	    y2=yc+dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }

	    x1=xc-dx2;
	    y1=yc+dy2;
	    x2=xc-dx2;
	    y2=yc-dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }

	}
	
ErrorOut:
	
	Threads->done=TRUE;

	return 0;
}
static int pioDrawStreamlines(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	char name[256];
	long length,length1,nbr,index;
	double *u=NULL,*v=NULL;
	double x[2],y[2],z[2];
	double *nbrxmin=NULL,*nbrxmax=NULL,*nbrymin=NULL,*nbrymax=NULL;
	int isl,is,id,sgn,iseg;
	int ret,child1,child2,level,nbrlevel;
	double value,coord,speedCutoff,gu[4],gv[4],halfsize[2],nbrhalfsize[2],invdx;
	double x1,y1,x2,y2;
	int i,j,k,ix,iy,nsl,slStartSide0,slEndSide0,icnt,doGrad,maxSteps,maxCells,icells,dir,idir,ndir;
	double slStartPt0[3],slEndPt0[3],*slxMidPts0,*slyMidPts0,*slzMidPts0;
	double cellCenter0[3],cellSize0[3],cellVel0[3];
	double cellDvDx0[3][3],xLine0,yLine0,xLine1,yLine1,eps,epsMin,epsMax,hFrac,hFracMin,hFracMax;
	
	if(!Files || !ss || !r) return 1;

	pio = &Files->pioData;

	if (!pio) return 1;
	if (pio->streamline.num<=0) return 1;
	if (pio->streamline.error<0.0) pio->streamline.error=0.0;
	if (pio->streamline.error>1.0) pio->streamline.error=1.0;
	
	
	ret=1;
		
	slxMidPts0=NULL;
	slyMidPts0=NULL;
	slzMidPts0=NULL;
	nbrymax=NULL;
	nbrymin=NULL;
	nbrxmax=NULL;
	nbrxmin=NULL;
	v=NULL;
	u=NULL;
	
	x[1]=y[1]=0;
	
	nsl = pio->streamline.num;
	speedCutoff = pio->streamline.speedCutoff;
	doGrad = pio->streamline.doGrad;
	maxSteps = pio->streamline.maxSteps;
	maxCells = pio->streamline.maxCells;
	dir = pio->streamline.dir; /* -1=backward only, 0=both, 1=forward only */
	xLine0 = pio->streamline.xStart;
	yLine0 = pio->streamline.yStart;
	xLine1 = pio->streamline.xEnd;
	yLine1 = pio->streamline.yEnd;
	epsMin = 0.01; /* error bounds for integration routine */
	epsMax = 0.1;  /* Percent of minimum cell dimension */
	eps = epsMin+(epsMax-epsMin)*pio->streamline.error;
	hFracMin = 0.05; /* Initial step size factor integration routine */
	hFracMax = 0.2;  /* Percent of minimum cell dimension */
	hFrac = hFracMin+(hFracMax-hFracMin)*pio->streamline.error;

	slxMidPts0 = (double*)cMalloc((maxSteps+2)*sizeof(double),65431);
	if(!slxMidPts0)goto ErrorOut;
	slyMidPts0 = (double*)cMalloc((maxSteps+2)*sizeof(double),65432);
	if(!slyMidPts0)goto ErrorOut;
	slzMidPts0 = (double*)cMalloc((maxSteps+2)*sizeof(double),65433);
	if(!slzMidPts0)goto ErrorOut;

	sprintf(name,"xdt");
	if(SageGetFileItem(Files,name,0,&u,&length,pio->CurrentFrame)) goto ErrorOut;
	if(length != pio->numcell)goto ErrorOut;

	sprintf(name,"ydt");
	if(SageGetFileItem(Files,name,0,&v,&length1,pio->CurrentFrame)) goto ErrorOut;
	if(length != length1)goto ErrorOut;

	sprintf(name,"cell_index");
	if(SageGetFileItem(Files,name,1,&nbrxmin,&length1,pio->CurrentFrame)) goto ErrorOut;
	if(length != length1)goto ErrorOut;

	sprintf(name,"cell_index");
	if(SageGetFileItem(Files,name,2,&nbrxmax,&length1,pio->CurrentFrame)) goto ErrorOut;
	if(length != length1)goto ErrorOut;

	sprintf(name,"cell_index");
	if(SageGetFileItem(Files,name,3,&nbrymin,&length1,pio->CurrentFrame)) goto ErrorOut;
	if(length != length1)goto ErrorOut;

	sprintf(name,"cell_index");
	if(SageGetFileItem(Files,name,4,&nbrymax,&length1,pio->CurrentFrame)) goto ErrorOut;
	if(length != length1)goto ErrorOut;
	
	if (dir==0) {
		ndir = 2;
	} else {
		ndir = 1;
	}

	for(idir=0;idir<ndir;idir++) {
	for(isl=0;isl<nsl;++isl){

		if (nsl==1) {
			slStartPt0[0] = xLine0;
			slStartPt0[1] = yLine0;
		} else {
			slStartPt0[0] = xLine0 + (double)isl/(nsl-1)*(xLine1-xLine0);
			slStartPt0[1] = yLine0 + (double)isl/(nsl-1)*(yLine1-yLine0);
		}
		slStartPt0[2] = 0.0;
		slStartSide0 = -1;

		if(slStartPt0[0] <= pio->xmin) continue;
		if(slStartPt0[1] <= pio->ymin) continue;
		if(slStartPt0[0] >= pio->xmax) continue;
		if(slStartPt0[1] >= pio->ymax) continue;

		if (!pioGetValue(pio,slStartPt0[0],slStartPt0[1],&value,&index)) goto ErrorOut;
		if (index<0 || index>pio->numcell-1) goto ErrorOut;
	
		icells=0;
begin:
		cellCenter0[0] = pio->xcenter[index];
		cellCenter0[1] = pio->ycenter[index];
		cellCenter0[2] = 0.0;
		cellSize0[0] = pio->dxset/pow(2.0,(double)(pio->level[index]-1));
		cellSize0[1] = pio->dyset/pow(2.0,(double)(pio->level[index]-1));
		cellSize0[2] = 0.0;
		cellVel0[0] = u[index];
		cellVel0[1] = v[index];
		cellVel0[2] = 0.0;

		if (cellVel0[0]*cellVel0[0]+cellVel0[1]*cellVel0[1]<speedCutoff*speedCutoff) continue;

		for(j=0;j<3;j++) {
			for(i=0;i<3;i++) {
				cellDvDx0[j][i] = 0.0;
			}
		}

		/* Calculate gradients */
		if (doGrad) {
			for(is=0;is<4;is++) {
				switch (is) {
				case 0:
					nbr = (long)floor(nbrxmin[index]-1+.1);
					sgn = 1;
					child1 = 1;
					child2 = 3;
					break;
				case 1:
					nbr = (long)floor(nbrxmax[index]-1+.1);
					sgn = -1;
					child1 = 0;
					child2 = 2;
					break;
				case 2:
					nbr = (long)floor(nbrymin[index]-1+.1);
					sgn = 1;
					child1 = 2;
					child2 = 3;
					break;
				case 3:
					nbr = (long)floor(nbrymax[index]-1+.1);
					sgn = -1;
					child1 = 0;
					child2 = 1;
					break;
				default:
					nbr = 0;
					sgn = 1;
					child1 = 1;
					child2 = 3;
					goto ErrorOut;
				}

				level=(int)(pio->level[index]);
				if(nbr != index){
					nbrlevel=(int)(pio->level[nbr]);
					if(nbrlevel <= level){
						if(pio->daughter[nbr]){
							nbr=(long)(pio->daughter[nbr]-1);
							nbrlevel=(int)(pio->level[nbr]);
							getPIOHalfCellSize2D(pio,level,halfsize);
							getPIOHalfCellSize2D(pio,nbrlevel,nbrhalfsize);
							id=is/2;
							invdx=sgn/(halfsize[id]+nbrhalfsize[id]);
							gu[is] =  (u[index]-u[nbr+child1])*invdx;
							gu[is] += (u[index]-u[nbr+child2])*invdx;
							gu[is] = 0.5*gu[is];
							gv[is] =  (v[index]-v[nbr+child1])*invdx;
							gv[is] += (v[index]-v[nbr+child2])*invdx;
							gv[is] = 0.5*gv[is];
						}else{
							getPIOHalfCellSize2D(pio,level,halfsize);
							getPIOHalfCellSize2D(pio,nbrlevel,nbrhalfsize);
							id=is/2;
							invdx=sgn/(halfsize[id]+nbrhalfsize[id]);
							gu[is] = (u[index]-u[nbr])*invdx;
							gv[is] = (v[index]-v[nbr])*invdx;
						}
					}else{
						goto ErrorOut;
					}
				}else{
					gu[is] = 0;
					gv[is] = 0;
				}
			}

			cellDvDx0[0][0] = 0.5*(gu[0]+gu[1]);  /* du/dx */
			cellDvDx0[0][1] = 0.5*(gu[2]+gu[3]);  /* du/dy */
			cellDvDx0[1][0] = 0.5*(gv[0]+gv[1]);  /* dv/dx */
			cellDvDx0[1][1] = 0.5*(gv[2]+gv[3]);  /* dv/dy */
		}


		if (dir==-1) {
			sgn = -1;
		} else if (dir==0) {
			sgn = 1-2*idir;
		} else {
			sgn = 1;
		}
		for(j=0;j<3;j++) {
			cellVel0[j] = sgn*cellVel0[j];
			for(i=0;i<3;i++) {
				cellDvDx0[j][i] = sgn*cellDvDx0[j][i];
			}
		}


		icnt = 0;

		ret = slCellPath(cellCenter0,cellSize0,cellVel0,cellDvDx0,slStartPt0,
			slStartSide0,speedCutoff,  slEndPt0,&slEndSide0,&icnt,maxSteps,eps,hFrac,slxMidPts0,slyMidPts0,slzMidPts0);

		if (ret==0) goto ErrorOut;

		if (icnt>maxSteps+2) {
			goto ErrorOut;
		}

		/* Draw the line segment */
		for(iseg=0;iseg<icnt-1;iseg++) {
			x[0] = slxMidPts0[iseg];
			y[0] = slyMidPts0[iseg];
			z[0] = slzMidPts0[iseg];
			x[1] = slxMidPts0[iseg+1];
			y[1] = slyMidPts0[iseg+1];
			z[1] = slzMidPts0[iseg+1];

			k=1;
			x1=sqrt(x[k-1]*x[k-1]+z[k-1]*z[k-1]);
			x2=sqrt(x[k]*x[k]+z[k]*z[k]);
			y1=y[k-1];
			y2=y[k];
			if(!isOut(&x1,&y1,&x2,&y2,r)){
				RealLoc(pio,&x1,&y1,&ix,&iy);
				MoveB(ix,iy);
				RealLoc(pio,&x2,&y2,&ix,&iy);
				LineB(ix,iy,255,ss);
			}
		}

		/* If the streamline traversed and exited the cell properly */
		if (ret==1) {

			for(j=0;j<3;j++) {
				slStartPt0[j] = slEndPt0[j];
			}

			/* Find opposite side-index */
			slStartSide0 = slEndSide0 + 2*(int)floor(1.0-fmod(slEndSide0,2.0)+.1)-1;

			/* Get neighbor-cell index */
			if (slEndSide0==0) {
				nbr = (long)floor(nbrxmin[index]-1+.1);
				child1 = 1;
				child2 = 3;
				coord = y[1]-cellCenter0[1];
			} else if (slEndSide0==1) {
				nbr = (long)floor(nbrxmax[index]-1+.1);
				child1 = 0;
				child2 = 2;
				coord = y[1]-cellCenter0[1];
			} else if (slEndSide0==2) {
				nbr = (long)floor(nbrymin[index]-1+.1);
				child1 = 2;
				child2 = 3;
				coord = x[1]-cellCenter0[0];
			} else if (slEndSide0==3) {
				nbr = (long)floor(nbrymax[index]-1+.1);
				child1 = 0;
				child2 = 1;
				coord = x[1]-cellCenter0[0];
			} else {
				nbr = 0;
				child1 = 1;
				child2 = 3;
				coord = 0.0;
				goto ErrorOut;
			}

			level=(int)(pio->level[index]);
			if(nbr != index){
				nbrlevel=(int)(pio->level[nbr]);
				if(nbrlevel <= level){
					if(pio->daughter[nbr]){
						nbr=(long)(pio->daughter[nbr]-1);
						if (coord<0.0) {
							index = nbr+child1;
						} else {
							index = nbr+child2;
						}
					}else{
						index = nbr;
					}
				}else{
					goto ErrorOut;
				}
			}else{
				continue;
			}

			icells++;
			if (icells>maxCells) {
				continue;
			}
			goto begin;
		}
	}
	}

	ret=0;

ErrorOut:	
	if (slxMidPts0) cFree((char *)slxMidPts0);
	if (slyMidPts0) cFree((char *)slyMidPts0);
	if (slzMidPts0) cFree((char *)slzMidPts0);
	if(u)cFree((char *)u);
	if(v)cFree((char *)v);
	if(nbrxmin)cFree((char *)nbrxmin);
	if(nbrxmax)cFree((char *)nbrxmax);
	if(nbrymin)cFree((char *)nbrymin);
	if(nbrymax)cFree((char *)nbrymax);
	return ret;
}
int getPIOHalfCellSize2D(struct FilePIOInfo *pio,int level,  double size[2])
{
	if (!size || !pio) return 0;
	size[0] = pio->dx2[level];
	size[1] = pio->dy2[level];
	return 1;
}
int pioDrawLasers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	char name[256];
	long length,length1;
	double *x,*y,*z;
	long n,k;
	int ret;
	double x1,y1,x2,y2;
	int ix,iy;
	
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	
	x=NULL;
	y=NULL;
	z=NULL;
	
	ret=1;

	if((pio->LaserPlotCount < 1) || !pio->LaserPlotList)goto ErrorOut;
	
	
	for(n=0;n<pio->LaserPlotCount;++n){
		sprintf(name,"laser_plot_x(%d)%c",pio->LaserPlotList[n],0);
		x=NULL;
		if(SageGetFileItem(Files,name,0,&x,&length,pio->CurrentFrame)){
		/*
		    sprintf(WarningBuff,"pioDrawLasers Error Reading %s Frame %ld\n",name,pio->CurrentFrame);
		    WarningBatch(WarningBuff);
		*/
		    goto doContinue;
		}
				
		sprintf(name,"laser_plot_y(%d)%c",pio->LaserPlotList[n],0);
		y=NULL;
		if(SageGetFileItem(Files,name,0,&y,&length1,pio->CurrentFrame)){
		    goto doContinue;
		}
						
		sprintf(name,"laser_plot_z(%d)%c",pio->LaserPlotList[n],0);
		z=NULL;
		if(SageGetFileItem(Files,name,0,&z,&length1,pio->CurrentFrame)){
		    goto doContinue;
		}
		if(length != length1)goto doContinue;
		for(k=1;k<length;++k){
		    x1=sqrt(x[k-1]*x[k-1]+z[k-1]*z[k-1]);
		    x2=sqrt(x[k]*x[k]+z[k]*z[k]);
		    y1=y[k-1];
		    y2=y[k];
		    if(!isOut(&x1,&y1,&x2,&y2,r)){
		        RealLoc(pio,&x1,&y1,&ix,&iy);
		        MoveB(ix,iy);
		        RealLoc(pio,&x2,&y2,&ix,&iy);
		        LineB(ix,iy,255,ss);
		    }
		}		
doContinue:		
		if(x)cFree((char *)x);
		x=NULL;		
		if(y)cFree((char *)y);
		y=NULL;		
		if(z)cFree((char *)z);
		z=NULL;		
	}
	ret=0;
ErrorOut:	
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	return ret;
}
int pioDrawTracers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	long length,length1;
	double *x,*y,*v;
	long n,k;
	int ret;
	double x1,y1,x2,y2;
	int ix,iy,nv;
	
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	
	x=NULL;
	y=NULL;
	v=NULL;
	
	ret=1;

	if((pio->TracerPlotCount < 1) || !pio->TracerPlotList)goto ErrorOut;

	if(SageGetFileItem(Files,"tracer_data",0,&v,&length,pio->CurrentFrame))goto ErrorOut;
	if(length < 2)goto ErrorOut;
	
	x=(double *)cMalloc(length*sizeof(double),4261);
	if(!x)goto ErrorOut;
	
	y=(double *)cMalloc(length*sizeof(double),5262);
	if(!y)goto ErrorOut;
		
	for(n=0;n<pio->TracerPlotCount;++n){
	    nv = pio->TracerPlotList[n] % pio->tracer_words_per_record;
	    nv /= pio->tracer_words_per_point;
	    nv *= pio->tracer_words_per_point;
	    
	    length1=0;
	    for(k=0;k<length;k+=pio->tracer_words_per_record){
	    	x[length1  ]=v[1+nv+k];
	    	y[length1++]=v[2+nv+k];
	    }
		if(length1 < 2)continue;
		for(k=1;k<length1;++k){
		    x1=x[k-1];
		    x2=x[k];
		    y1=y[k-1];
		    y2=y[k];
		    if(!isOut(&x1,&y1,&x2,&y2,r)){
		        RealLoc(pio,&x1,&y1,&ix,&iy);
		        MoveB(ix,iy);
		        RealLoc(pio,&x2,&y2,&ix,&iy);
		        LineB(ix,iy,255,ss);
		    }
		}		
	}
	ret=0;
ErrorOut:	
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(v)cFree((char *)v);
	return ret;
}
static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	double vmax,vmaxLen,vlength,vx,vy,dx,dy;
	double xc,yc,dx2,dy2;
	double x1,y1,x2,y2,x3,y3;
	double pi,turn,o1,oa,arrow;
	double vmax2,degen,vspace;
	int ix,iy;
	long length;
	long n;
	int ret;
	int k;
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	
	ret = 1;
	
	if(mstrcmp(pio->vector.vectorxName,pio->vectorxNameOld)){
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	}
	
	if(mstrcmp(pio->vector.vectoryName,pio->vectoryNameOld)){
	    if(pio->vectory)cFree((char *)pio->vectory);
	    pio->vectory=NULL;
	}
	
	if(!pio->vectorx || !pio->vectory || pio->CurrentFrameVectors != pio->CurrentFrame){
	
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	    
	    if(pio->vectory)cFree((char *)pio->vectory);
	    pio->vectory=NULL;
	    
	    pio->CurrentFrameVectors = -1;  
	      
		if(SageGetFileItem(Files,pio->vector.vectorxName,0,
		                  &pio->vectorx,&length,pio->CurrentFrame)){
			goto ErrorOut;
		}
		
		if(length != pio->numcell)goto ErrorOut;

		if(SageGetFileItem(Files,pio->vector.vectoryName,0,
		                  &pio->vectory,&length,pio->CurrentFrame)){
			goto ErrorOut;
		}	  
		  
		if(length != pio->numcell)goto ErrorOut;
		
	    pio->CurrentFrameVectors = pio->CurrentFrame;  
	    	    
	    vmax=0;	    
        for(n=0;n<pio->numcell;++n){
            if(pio->daughter[n])continue;
		    k=(int)(pio->level[n]);
		    xc=pio->xcenter[n];
		    dx2=pio->dx2[k];
		    if(xc+dx2 < pio->range.xminData)continue;
		    if(xc-dx2 > pio->range.xmaxData)continue;
		    yc=pio->ycenter[n];
		    dy2=pio->dy2[k];
		    if(yc+dy2 < pio->range.yminData)continue;
		    if(yc-dy2 > pio->range.ymaxData)continue;
	        vx=pio->vectorx[n];
	        vy=pio->vectory[n];
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > vmax)vmax=vlength;
        }
	    	    	    
	    pio->vmaxVectors=vmax;
	    
	    mstrncpy(pio->vectorxNameOld,pio->vector.vectorxName,32);
	    mstrncpy(pio->vectoryNameOld,pio->vector.vectoryName,32);

	}
	
	if(pio->vector.vmax > 0){
		vmax=pio->vector.vmax;
	}else{
	    vmax=pio->vmaxVectors;
	}

	if(vmax <= 0)goto ErrorOut;
	
	dx=pio->range.xmaxData-pio->range.xminData;
	dy=pio->range.ymaxData-pio->range.yminData;
	if(dx < dy){
	    vmaxLen=dx*pio->vector.vfac;
	}else{
	    vmaxLen=dy*pio->vector.vfac;
	}
	
	vmaxLen=dy*pio->vector.vfac;
	
	if(vmaxLen <= 0)goto ErrorOut;
	
	vmaxLen /= vmax;
	
	pi=4.*atan(1.);
	turn=pio->vector.arrowAngle*pi/180.;
	
	arrow=pio->vector.arrowLength*vmax*vmaxLen;
	
   	vmax2=vmax/3.;
		
	if(pio->vector.vspace == 0){
	    for(n=0;n<pio->numcell;++n){
	        if(pio->daughter[n])continue;
	        vx=pio->vectorx[n];
	        vy=pio->vectory[n];
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > vmax){
	            vx = vx*vmax/vlength;
	            vy = vy*vmax/vlength;
	        }
	        if(vlength <= 0)continue;
	        
		    x1=pio->xcenter[n];
		    x3=x2=x1+vx*vmaxLen;
		    y1=pio->ycenter[n];
		    y3=y2=y1+vy*vmaxLen;
		    if(!isOut(&x1,&y1,&x2,&y2,r)){
		        RealLoc(pio,&x1,&y1,&ix,&iy);
		        MoveB(ix,iy);
		        RealLoc(pio,&x2,&y2,&ix,&iy);
		        LineB(ix,iy,255,ss);
		    }else{
		        continue;
		    }
		    
			if(x1 == x2 && y2 == y1)continue;

	      	if(vlength < vmax2){
	      	    degen=vlength/vmax2;
		    }else{
	      		degen=1.;
		    }
		    
	        x2=x3;
	        y2=y3;
	    	o1=atan2(y2-y1,x2-x1);
		    	
		    if(pio->vector.arrowHeads >= 1){
     	 		oa=o1+turn;
			    x1=x2+arrow*degen*cos(oa);
			    y1=y2+arrow*degen*sin(oa);
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,ss);
			    }
		    }
	        
		    if(pio->vector.arrowHeads >= 2){		    
		        x2=x3;
		        y2=y3;
     	 		oa=o1-turn;
			    x1=x2+arrow*degen*cos(oa);
			    y1=y2+arrow*degen*sin(oa);
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,ss);
			    }
		    }
	        
	    }
	}else{
	    long ixmin,ixmax,iymin,iymax;
	    double xmin,xmax,ymin,ymax;
	    double didx,dxdi,djdy,dydj;
	    vspace=pio->vector.vspace;
	    if(pio->vector.vspace < 0){
	    	vspace = -vspace;	    
			if(dx < dy){
				vspace=dx/vspace;
			}else{
				vspace=dy/vspace;
			}
			vspace=dy/(-pio->vector.vspace);
	    }
	    
	   	    
	    xmin=pio->range.xminData;
	    xmax=pio->range.xmaxData;
	    ymin=pio->range.yminData;
	    ymax=pio->range.ymaxData;
	    ixmin=0;
	    ixmax=(long)(dx/vspace);
	    iymin=0;
	    iymax=(long)(dy/vspace);

        didx=((double)(ixmax-ixmin))/(xmax-xmin);
        dxdi=1.0/didx;
        djdy=((double)(iymax-iymin))/(ymax-ymin);
        dydj=1.0/djdy;


	    for(n=0;n<pio->numcell;++n){
	        double xc,yc,dx2,dy2;
	        double ymn,ymx,xmn,xmx;
	        double xs,ys;
	        long i,j,ihi,ilo,jhi,jlo;
	        int k;

	        if(pio->daughter[n])continue;
	        k=(int)(pio->level[n]);
	        
	        yc=pio->ycenter[n];
	        dy2=pio->dy2[k]*1.000001;

			ymn=max(ymin,yc-dy2);
			ymx=min(ymax,yc+dy2);
			if(ymx < ymn)continue;

	        xc=pio->xcenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        
			xmn=max(xmin,xc-dx2);
			xmx=min(xmax,xc+dx2);
			if(xmx < xmn)continue;
			
			ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
			ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);

			jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
			jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
			
	        vx=pio->vectorx[n];
	        vy=pio->vectory[n];
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > vmax){
	            vx = vx*vmax/vlength;
	            vy = vy*vmax/vlength;
	        }
	        if(vlength <= 0)continue;
			
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        
					x1=xs;
				    x3=x2=x1+vx*vmaxLen;
				    y1=ys;
				    y3=y2=y1+vy*vmaxLen;
				    if(!isOut(&x1,&y1,&x2,&y2,r)){
				        RealLoc(pio,&x1,&y1,&ix,&iy);
				        MoveB(ix,iy);
				        RealLoc(pio,&x2,&y2,&ix,&iy);
				        LineB(ix,iy,255,ss);
				    }else{
				        continue;
				    }
				    
					if(x1 == x2 && y2 == y1)continue;

			      	if(vlength < vmax2){
			      	    degen=vlength/vmax2;
				    }else{
			      		degen=1.;
				    }
				    
			        x2=x3;
			        y2=y3;
			    	o1=atan2(y2-y1,x2-x1);
				    	
				    if(pio->vector.arrowHeads >= 1){
		     	 		oa=o1+turn;
					    x1=x2+arrow*degen*cos(oa);
					    y1=y2+arrow*degen*sin(oa);
					    if(!isOut(&x1,&y1,&x2,&y2,r)){
					        RealLoc(pio,&x1,&y1,&ix,&iy);
					        MoveB(ix,iy);
					        RealLoc(pio,&x2,&y2,&ix,&iy);
					        LineB(ix,iy,255,ss);
					    }
				    }
			        
				    if(pio->vector.arrowHeads >= 2){		    
				        x2=x3;
				        y2=y3;
		     	 		oa=o1-turn;
					    x1=x2+arrow*degen*cos(oa);
					    y1=y2+arrow*degen*sin(oa);
					    if(!isOut(&x1,&y1,&x2,&y2,r)){
					        RealLoc(pio,&x1,&y1,&ix,&iy);
					        MoveB(ix,iy);
					        RealLoc(pio,&x2,&y2,&ix,&iy);
					        LineB(ix,iy,255,ss);
					    }
				    }			          
			    }
			}
	    }    
	}
	
	ret = 1;
ErrorOut:
	return ret;
}
static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
    struct FilePIOInfo *pio;
    static double  small2=1e-8;
	struct dRange rr;
	
	
	if(!Files || !ss || !buff)return 1;
	pio = &Files->pioData;

	setPioScales(pio);

	rr.xmin=pio->range.xminData;
	rr.ymin=pio->range.yminData;
	rr.xmax=pio->range.xmaxData;
	rr.ymax=pio->range.ymaxData;

	rr.dxlim=small2*(rr.xmax-rr.xmin);
	if(rr.dxlim <= 0.0)rr.dxlim=small2;
	rr.dylim=small2*(rr.ymax-rr.ymin);
	if(rr.dylim <= 0.0)rr.dylim=small2;


	if(pio->pd.AreaFill){
		double *d;
		long length;

		length=pio->range.ixmax*pio->range.iymax;
		d=(double *)buff;
		setFloat(d,length);
	    if(getPlotData(Files,d,CurrentFrame)){
		    sprintf(WarningBuff,"pioDraw - getPlotData Error\n");
		    WarningBatch(WarningBuff);
			return 1;
	    }	
	    	    
	    FloatToImage(d,length,&pio->pd,NULL);

		if(pio->active){
		    pioDrawVoid(Files,(unsigned char *)d);
		}

	}
	
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	
	if(pio->pa.DrawZones){
		pioDrawZones(pio,ss,&rr);
	}
	
	if(pio->vector.DrawVectors){
		pioDrawVectors(Files,ss,&rr);
	}
	
	if(pio->streamline.DrawStreamlines){
	   pioDrawStreamlines(Files,ss,&rr);
	}
	
	
	if(pio->LaserPlotCount){
	   pioDrawLasers2d(Files,ss,&rr);
	}
	
	if(pio->TracerPlotCount){
	   pioDrawTracers2d(Files,ss,&rr);
	}
	
	return 0;
}
static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout)
{
    struct FilePIOInfo *pio;
	struct DrawData d;
		
	if(!Files || !sout)return 1;
	pio = &Files->pioData;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

	setPioScales(pio);
	
	d.pio=pio;
	d.sout=sout;
	
	runThreads(pio->threads,&d,pio->numcell,pioDrawVoidT);
	
	return 0;
}
	
static int pioDrawVoidT(mThread *Threads)
{
		struct FilePIOInfo *pio;
		unsigned char *sout;
		struct DrawData *d;


		double xmin,xmax,ymin,ymax;
		double didx,djdy;
		double dxdi,dydj;
		long ixmin,ixmax,iymin,iymax;
		long n;		

		if(!Threads)goto ErrorOut;
		d=(struct DrawData *)Threads->data;
		if(!d)goto ErrorOut;

		pio=d->pio;
		sout=d->sout;
		
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


	    for(n=Threads->smin;n<Threads->smax;++n){
	        double xc,yc,dx2,dy2;
	        double ymn,ymx,xmn,xmx;
	        double xs,ys;
	        long i,j,ihi,ilo,jhi,jlo;
	        int k;

	        if(pio->daughter[n] || pio->active[n])continue;
	        k=(int)(pio->level[n]);
	        
	        yc=pio->ycenter[n];
	        dy2=pio->dy2[k]*1.000001;

			ymn=max(ymin,yc-dy2);
			ymx=min(ymax,yc+dy2);
			if(ymx < ymn)continue;

	        xc=pio->xcenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        
			xmn=max(xmin,xc-dx2);
			xmx=min(xmax,xc+dx2);
			if(xmx < xmn)continue;
			
			ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
			ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);

			jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
			jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
						
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=255;
			    }
			}

	    }
ErrorOut:
		Threads->done=TRUE;
	    return 0;
	        
}
static int setPioScales(struct FilePIOInfo *pio)
{
	if(!pio)return 1;
	
	if(pio->range.ixminData < 0)pio->range.ixminData=0;
	if(pio->range.iyminData < 0)pio->range.iyminData=0;
	if(pio->range.ixmaxData > pio->range.ixmax)pio->range.ixmaxData=pio->range.ixmax;
	if(pio->range.iymaxData > pio->range.iymax)pio->range.iymaxData=pio->range.iymax;
	
	
	if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	if(!pio->range.yminSet)pio->range.yminData=pio->ymin;
	if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	if(!pio->range.ymaxSet)pio->range.ymaxData=pio->ymax;

    pio->range.xScale=((double)(pio->range.ixmaxData-pio->range.ixminData))/(pio->range.xmaxData-pio->range.xminData);
    pio->range.yScale=((double)(pio->range.iymaxData-pio->range.iyminData))/(pio->range.ymaxData-pio->range.yminData);
	return 0;
}
static int sageLocation(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	struct PIO *sage;
	long *location;
	long count;
	long n;
	long loc,length;
	long ixmax,ierror;
	long i,j;
	int kk;

	double xmin,ymin;
	double dxset,dyset;
	double odxset,odyset;
	double xc,yc;


	if(!Files)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage)return 1;

	length=sage->ixmax*sage->iymax;

	if(pio->location)cFree((char *)pio->location);
	location=(long *)cMalloc(sizeof(long)*length,19723);
	pio->location=location;
	if(!location)return 1;

	for(n=0;n<length;++n)location[n] = -1;

	ixmax=sage->ixmax;


	xmin=pio->xmin;
	ymin=pio->ymin;

	dxset=sage->dxset;
	odxset=1.0/dxset;
	dyset=sage->dyset;
	odyset=1.0/dyset;

	ierror=0;
	count=0;
	for(n=0;n<pio->numcell;++n){
	    kk=(int)(pio->level[n]);
	    if(kk != 1)continue;
	    ++count;
	    xc=pio->xcenter[n];
	    i=(long)((xc-xmin)*odxset);

	    yc=pio->ycenter[n];
	    j=(long)((yc-ymin)*odyset);

	    loc=i+j*ixmax;
	    if(loc < 0 || loc >= length){
	          if(++ierror < 10){
	              ;
	          }
	          continue;
	    }
	    location[loc]=n;
	}

	for(n=0;n<length;++n){
	    if((location[n] < 0) ||  (location[n] >= pio->numcell))++ierror;
	}

	if(ierror || (count != length)){
	    sprintf(WarningBuff,"Location Errors ierror %ld count %ld\n",ierror,count);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	return 0;
ErrorOut:
	return 1;
}
static int SetPIOData(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct PIO *sage;
	int active;
	int ret;
	long n;
	int k;
	double *fdata1,*fdata2,*fdata3,*fdata4,*fdata5,*fdata6;
	long dataLength1,dataLength2,dataLength3,dataLength4,dataLength5;
	double lmin,lmax,size,cmin,cmax;


	if(!Files)return 1;
	pio = &Files->pioData;
	if(Files->fileClass != PIO2D_FILES)return 1;
	if(!Files->PIO2DList)return 1;
	
	ret = 1;

	fdata1=NULL;
	fdata2=NULL;
	fdata3=NULL;
	fdata4=NULL;
	fdata5=NULL;
	fdata6=NULL;

	lmin=cmin=     1e60;
	lmax=cmax=    -1e60;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	pio->sage = sage = &Files->PIO2DList[CurrentFrame];
			    			
	Files->limits.time=sage->time;
	Files->pioTime=sage->time;
	
	if((pio->CurrentFrame == CurrentFrame) && pio->xcenter && pio->ycenter && pio->level && pio->daughter){
	    
	    if(pio->value){

		    if(!mstrcmp(pio->pioName,pio->pioNameOld) && (pio->pioIndex == pio->pioIndexOld)){
		        return 0;
		    }
	        cFree((char *)pio->value);
	        pio->value=NULL;
	    }

        if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
                  &fdata4,&dataLength4,CurrentFrame)){
	    	ret=22;goto ErrorOut;
        }
        
        if(pio->active){
	        for(n=0;n<pio->numcell;++n){
	        	if(pio->daughter[n] || (pio->active[n] <= 0.0))continue;
	            size=fdata4[n];
	            if(size < cmin)cmin=size;
	            if(size > cmax)cmax=size;
	        }
        }else{
	        for(n=0;n<pio->numcell;++n){
	        	if(pio->daughter[n])continue;
	            size=fdata4[n];
	            if(size < cmin)cmin=size;
	            if(size > cmax)cmax=size;
	        }
        }
        mstrncpy(pio->pioNameOld,pio->pioName,255);
        pio->pioIndexOld=pio->pioIndex;
        mstrncpy(Files->pioName,pio->pioName,255);
        Files->pioIndex=pio->pioIndex;
        
	    pio->cmin=cmin;
	    pio->cmax=cmax;
 	    pio->value=fdata4;
     	    
		if(pio->pa.flagGradients)SageGetGradients2D(Files,pio->pa.flagGradients-1);

		pio->doGradients = FALSE;
		if(pio->pa.flagGradients && pio->gradx && pio->grady){
			pio->doGradients = TRUE;
		}else{
			if(pio->gradx)cFree((char *)pio->gradx);
			pio->gradx=NULL;
			if(pio->grady)cFree((char *)pio->grady);
			pio->grady=NULL;
		}
		
		sageLocation(Files);
		
 	    return 0;

	}

	pio->CurrentFrame=CurrentFrame;

	if(pio->xcenter)cFree((char *)pio->xcenter);
	pio->xcenter=NULL;
	if(pio->ycenter)cFree((char *)pio->ycenter);
	pio->ycenter=NULL;
	if(pio->level)cFree((char *)pio->level);
	pio->level=NULL;
	if(pio->daughter)cFree((char *)pio->daughter);
	pio->daughter=NULL;
	if(pio->value)cFree((char *)pio->value);
	pio->value=NULL;
	if(pio->active)cFree((char *)pio->active);
	pio->active=NULL;

	pio->dxset=sage->dxset;
	pio->dyset=sage->dyset;
	pio->dzset=sage->dzset;
	pio->numcell=sage->numcell;
	pio->nummat=sage->nummat;
	pio->numdim=sage->numdim;

	pio->tracer_num_pnts=sage->tracer_num_pnts;
	pio->tracer_num_vars=sage->tracer_num_vars;
	pio->tracer_words_per_record=sage->tracer_words_per_record;
	pio->tracer_words_per_point=sage->tracer_words_per_point;

	if(SageGetFileItem(Files,"cell_center",1,
	                  &fdata1,&dataLength1,CurrentFrame)){
		    ret=23;goto ErrorOut;
	}

	if(SageGetFileItem(Files,"cell_center",2,
	                  &fdata2,&dataLength2,CurrentFrame)){
		    ret=24;goto ErrorOut;
	}

	if(SageGetFileItem(Files,"cell_level",0,
	                  &fdata3,&dataLength3,CurrentFrame)){
		    ret=25;goto ErrorOut;
	}

	if(SageGetFileItem(Files,"cell_active",0,
	                  &fdata6,&dataLength3,CurrentFrame)){
		    ret=26;goto ErrorOut;
	}
	
	if(SageGetFileItem(Files,"cell_daughter",0,
	                  &fdata5,&dataLength5,CurrentFrame)){
		    ret=27;goto ErrorOut;
	}


    if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
                  &fdata4,&dataLength4,CurrentFrame)){
	    ret=28;goto ErrorOut;
    }

    for(n=0;n<pio->numcell;++n){
		if(fdata5[n] || (fdata6[n] <= 0.0))continue;
        size=fdata4[n];
        if(size < cmin)cmin=size;
        if(size > cmax)cmax=size;
    }
    mstrncpy(pio->pioNameOld,pio->pioName,255);
    pio->pioIndexOld=pio->pioIndex;
    mstrncpy(Files->pioName,pio->pioName,255);
    Files->pioIndex=pio->pioIndex;
	pio->cmin=cmin;
	pio->cmax=cmax;
	pio->value=fdata4;


	active=FALSE;
	for(n=0;n<pio->numcell;++n){
	    size=fdata3[n];
	    if(size < lmin)lmin=size;
	    if(size > lmax)lmax=size;
	    if(fdata6[n] <= 0.0)active=TRUE;
	}
	
	
	if(lmin < 1){
	    long count;
	    
	    count=0;
		for(n=0;n<pio->numcell;++n){
		    if(fdata3[n] < 1){
		        fdata3[n]=lmax;
		        count++;
		    }
		}
	    sprintf(WarningBuff,"Error: %ld level zero cells set to level %.0f\n",count,lmax);
	    WarningBatch(WarningBuff);
	    
	    lmin=1;
	}

	if(lmin < 1 || lmax >= 100){
	    sprintf(WarningBuff,"Error level min %f lenvel max %f\n",lmin,lmax);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	for(k=1;k<=(int)lmax;++k){
	    pio->dx2[k]=.5*pio->dxset/pow(2,(double)(k-1));
	    pio->dy2[k]=.5*pio->dyset/pow(2,(double)(k-1));
	}

	pio->xmin=sage->xmin;
	pio->xmax=sage->xmax;

	pio->ymin=sage->ymin;
	pio->ymax=sage->ymax;

	pio->lmin=lmin;
	pio->lmax=lmax;

	pio->xcenter=fdata1;
	pio->ycenter=fdata2;
	pio->level=fdata3;
	pio->daughter=fdata5;

	if(pio->pa.flagGradients)SageGetGradients2D(Files,pio->pa.flagGradients-1);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}else{
		if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=NULL;
		if(pio->grady)cFree((char *)pio->grady);
		pio->grady=NULL;
	}

	if(active){
		pio->active=fdata6;
	}else{
	    pio->active=NULL;
		if(fdata6)cFree((char *)fdata6);
	}

	sageLocation(Files);
		
	return 0;
ErrorOut:
	pio->doGradients = FALSE;
	if(fdata1)cFree((char *)fdata1);
	if(fdata2)cFree((char *)fdata2);
	if(fdata3)cFree((char *)fdata3);
	if(fdata4)cFree((char *)fdata4);
	if(fdata5)cFree((char *)fdata5);
	if(fdata6)cFree((char *)fdata6);
    sprintf(WarningBuff,"SetPIOData Error Loading Item Name %s index %ld\n",pio->pioName,pio->pioIndex);
    WarningBatch(WarningBuff);
	return ret;
}

static int SageGetGradients2D(struct FileInfo2 *Files,int flag)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	double *gradl,*next,*gradh;
	long dataLength;
	long n,n2;
	int k,k2,ret;
	long ierror;

	if(flag < 0)return 0;
	if(!Files)return 1;
	pio = &Files->pioData;
	if(!pio)return 1;
	if(!pio->value)return 1;
	if(Files->fileClass != PIO2D_FILES)return 1;
	if(!Files->PIO2DList)return 1;
	
	ret = 1;

	CurrentFrame=pio->CurrentFrame;
	
	gradl=NULL;
	gradh=NULL;
	next=NULL;

	gradl=(double *)cMalloc(sizeof(double)*pio->numcell,19725);
	if(!gradl)goto ErrorOut;

	gradh=(double *)cMalloc(sizeof(double)*pio->numcell,19726);
	if(!gradh)goto ErrorOut;

	if(pio->gradx)cFree((char *)pio->gradx);
	pio->gradx=NULL;

	if(pio->grady)cFree((char *)pio->grady);
	pio->grady=NULL;

	if(pio->gradz)cFree((char *)pio->gradz);
	pio->gradz=NULL;

	pio->gradx=(double *)cMalloc(sizeof(double)*pio->numcell,19727);
	if(!pio->gradx)goto ErrorOut;

	pio->grady=(double *)cMalloc(sizeof(double)*pio->numcell,19728);
	if(!pio->grady)goto ErrorOut;

	for(n=0;n<pio->numcell;++n){
	    gradl[n]=0;
	    gradh[n]=0;
	}

	if(SageGetFileItem(Files,"cell_index",1,
	                  &next,&dataLength,CurrentFrame)){
		ret=20;goto ErrorOut;
	}

	ierror=0;

	for(n=0;n<pio->numcell;++n){        /* lo_x */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dx2[k]+pio->dx2[k2]);
	    }else{
	        gradl[n] = 0;
	    }
	}


	if(next)cFree((char *)next);
	next=NULL;

	if(SageGetFileItem(Files,"cell_index",2,
	                  &next,&dataLength,CurrentFrame)){
		ret=21;goto ErrorOut;
	}


	for(n=0;n<pio->numcell;++n){        /* hi_x */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dx2[k]+pio->dx2[k2]);
	    }else{
	        gradh[n] = 0;
	    }
	}


	if(next)cFree((char *)next);
	next=NULL;

	for(n=0;n<pio->numcell;++n){
		if(!pio->daughter[n] && (gradh[n]*gradl[n] >= 0.0)){
			if(fabs(gradh[n]) < fabs(gradl[n])){
				pio->gradx[n] = gradh[n];
			}else{
				pio->gradx[n] = gradl[n];
			}
		}else{
			pio->gradx[n] = 0;
		}
	    gradh[n]  = 0;
	    gradl[n] = 0;
	}


	if(SageGetFileItem(Files,"cell_index",3,
	                  &next,&dataLength,CurrentFrame)){
		ret=22;goto ErrorOut;
	}

	for(n=0;n<pio->numcell;++n){        /* lo_y */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dy2[k]+pio->dy2[k2]);
	    }else{
	        gradl[n] = 0;
	    }
	}

	if(next)cFree((char *)next);
	next=NULL;

	if(SageGetFileItem(Files,"cell_index",4,
	                  &next,&dataLength,CurrentFrame)){
		ret=23;goto ErrorOut;
	}

	for(n=0;n<pio->numcell;++n){        /* hi_y */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dy2[k]+pio->dy2[k2]);
	    }else{
	        gradh[n] = 0;
	    }
	}


	for(n=0;n<pio->numcell;++n){
		if(!pio->daughter[n] && (gradh[n]*gradl[n] >= 0.0)){
			if(fabs(gradh[n]) < fabs(gradl[n])){
				pio->grady[n] = gradh[n];
			}else{
				pio->grady[n] = gradl[n];
			}
		}else{
			pio->grady[n] = 0;
		}
	}

	if(next)cFree((char *)next);
	next=NULL;

	if(flag > 0){
		for(n=0;n<pio->numcell;++n){
		    if(flag == 1){
		       pio->value[n]=pio->gradx[n];
		    }else if(flag == 2){
		       pio->value[n]=pio->grady[n];
		    }else if(flag == 3){
		       pio->value[n]=pio->gradz[n];
		    }
		}
	}
	if(gradh)cFree((char *)gradh);
	if(gradl)cFree((char *)gradl);

	return 0;
ErrorOut:
	sprintf(WarningBuff,"sageGradients2D Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	if(gradl)cFree((char *)gradl);
	if(gradh)cFree((char *)gradh);
	return 1;
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
	
	if(!mstrcmp("stream2d",command)){
	    freeStreamLine2d(&pio->streamline2d);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline2d.streamOn=(long)value;
	    ++(cp->n);
	    if(pio->streamline2d.streamOn <= 0)goto Okout; 

	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline2d.streamGradients=(long)value;
	    ++(cp->n);

	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline2d.streamSteps=(long)value;
	    ++(cp->n);
	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->streamline2d.streamPasses=(long)value;
	    ++(cp->n);
	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    nn=(long)value;
	    ++(cp->n);
	    for(n=0;n<nn;++n){
	    	if(checkStreamData(&pio->streamline2d.zPlane))goto ErrorOut;
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline2d.zPlane.x[n]=value;
	    	++(cp->n);
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->streamline2d.zPlane.y[n]=value;
	    	++(cp->n);
	    	pio->streamline2d.zPlane.count++;
	    	
	    }
	    
	    
	}else if(!mstrcmp("bhangmeter",command)){
	    freeBhangMeter2d(&pio->bhangmeter2d);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->bhangmeter2d.bhangOn=(long)value;
	    ++(cp->n);
	    if(pio->bhangmeter2d.bhangOn <= 0)goto Okout; 
		
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->bhangmeter2d.targetValue=value;
	    ++(cp->n);
		
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->bhangmeter2d.pioIndex=(long)value;
	    ++(cp->n);
	    
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    strncpy(pio->bhangmeter2d.pioName,command,sizeof(pio->bhangmeter2d.pioName));
	    ++(cp->n);
	    
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    nn=(long)value;
	    ++(cp->n);
	    for(n=0;n<nn;++n){
	    	if(checkBhangMeterData(&pio->bhangmeter2d.xyz))goto ErrorOut;
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->bhangmeter2d.xyz.x[n]=value;
	    	++(cp->n);
			
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->bhangmeter2d.xyz.y[n]=value;
	    	++(cp->n);
			
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->bhangmeter2d.xyz.z[n]=value;
	    	++(cp->n);
			
	    	if(doubleCommand(&value,cp))goto ErrorOut;
	    	pio->bhangmeter2d.xyz.v[n]=value;
	    	++(cp->n);
			
	    	pio->bhangmeter2d.xyz.count++;
	    	
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
static int doSage2DCommandGet(struct FileInfo2 *Files,CommandPtr cp)
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
int doSage2DStart(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	
	pio->range.ixmax=532;
	pio->range.iymax=432;
	
	pio->range.ixminData=10;
	pio->range.ixmaxData=522;
	pio->range.iyminData=10;
	pio->range.iymaxData=422;

	pio->range.AspectRatio=1.0;
	pio->range.MaintainAspectRatio=1;
	pio->range.receiveRange=1;
	

	pio->CurrentFrameGradients =  -1;

	pio->CurrentFrameVectors = -1;
	pio->vector.DrawVectors=0;
	pio->vector.arrowLength=0.1;
	pio->vector.arrowAngle=145.0;
	pio->vector.arrowHeads=1;
	pio->vector.vspace= -21;
	pio->vector.vmax=0;
	pio->vector.vfac=0.2;
	
	pio->streamline.dir = 1;
	pio->streamline.doGrad = 0;
	pio->streamline.DrawStreamlines = 0;
	pio->streamline.maxCells = 10000;
	pio->streamline.maxSteps = 1000;
	pio->streamline.num = 4;
	pio->streamline.speedCutoff = 1.0;
	pio->streamline.error = 1.0;
	pio->streamline.xStart = 0.0;
	pio->streamline.xEnd = 1.0;
	pio->streamline.yStart = 0.0;
	pio->streamline.yEnd = 1.0;	
	
	pio->pd.sType=0;
	pio->pd.sPmin=0;
	pio->pd.sPmax=1;	
	pio->pd.LabeledPalette=FALSE;	
	pio->pd.paletteFont=2;
	pio->pa.DrawZones=0;
	pio->pa.ZoneLevel=1;
	pio->pd.AreaFill=1;

	pio->pd.red=1.0;
	pio->pd.green=1.0;
	pio->pd.blue=1.0;
	
	pio->threads=0;
	
	Files->pGetData2=doSage2DGetData;
	
	Files->pGetAreaData=doSage2DGetAreaData;
	
	Files->pGetLineData=doSage2DGetLineData;

	Files->pDoDialog=doSage2DDoDialog;
	
	Files->pInformation=doSage2DInformation;
	
	Files->pGetCellData=doSage2DGetCellData;

	Files->pSetCellData=doSage2DSetCellData;

	Files->pPlotLineData=doSage2DPlotLineData;
	
	Files->pCommandSend=doSage2DCommandGet;
	
    mstrncpy(pio->pioName,"density",255);
    pio->pioIndex=0;
    mstrncpy(pio->vector.vectorxName,"xdt",32);
    mstrncpy(pio->vector.vectoryName,"ydt",32);
    
	SetRange(Files,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY);
	
	getPaletteByName("ps",pio->pd.palette);
		
	return 0;
}
static int doSage2DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m)
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
		       !strcmp(name,"laser_plot_y") || !strcmp(name,"laser_plot_z")
		       ){
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
		sprintf(WarningBuff,"doSage2DPlotLineData  %s - cannot plot\n",m->nameList[n]);
		WarningBatch(WarningBuff);
		cFree((char *)m->nameList[n]);
		m->nameList[n]=NULL;
		
	}
	
	cFree((char *)m->nameList);
	m->nameList=NULL;
	
	if(nc1)doSage2DSavePlotLineData(Files,list1,nc1);
	
	if(nc2)doSage2DSaveTracerLineData(Files,list2,nc2);
	
	return 0;
}
static int doSage2DSaveTracerLineData(struct FileInfo2 *Files,int *list,int nc)
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
			WarningBatch("doSage2DPlotLineData out of memory\n");
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
	    pio->TracerPlotList=(int *)cMalloc(nc*sizeof(int),3261);
	    if(!pio->TracerPlotList){
			WarningBatch("doSage2DPlotLineData out of memory\n");
			return 1;
	    }
	
		for(n=0;n<nc;++n){
		    pio->TracerPlotList[n]=list[n];
		}
		
		pio->TracerPlotCount=nc;
	}

	return 0;
}

static int doSage2DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc)
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
			WarningBatch("doSage2DPlotLineData out of memory\n");
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
	    pio->LaserPlotList=(int *)cMalloc(nc*sizeof(int),1261);
	    if(!pio->LaserPlotList){
			WarningBatch("doSage2DPlotLineData out of memory\n");
			return 1;
	    }
	
		for(n=0;n<nc;++n){
		    pio->LaserPlotList[n]=list[n];
		}
		
		pio->LaserPlotCount=nc;
	}

	return 0;
}

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
    struct FilePIOInfo *pio;
    long index;
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

	if(SetPIOData(Files,li->CurrentFrame))goto OutOfHere;

    setPioScales(pio);
	
	for(n=0;n<li->PointLineCount;++n){
	    		    
	    if(!pioGetValue(pio,li->x[n],li->y[n],&li->v[n],&index)){
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
	struct PIO *sage;
	double *xD,*yD,s,s1,s2,s3,s4;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	int flagGradients;
	double rx,ry;
	int ibreak;
	int ret;
	
	if(!li)return 1;
	if(!Files)return 1;	
	pio=&Files->pioData;
	
	ret = 1;
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	flagGradients=pio->pa.flagGradients;
	
	pio->pa.flagGradients = FALSE;

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


	if(SetPIOData(Files,li->CurrentFrame))goto OutOfHere;
	
    setPioScales(pio);
	
	sage=pio->sage;
	
	np=10000;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81876))){
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
	
	while(1){
	    double value,dx2,dy2,xc,yc,dl,yy,xx;
	    long k;
	    if(!pioGetValue(pio,x+dx,y+dy,&value,&ne)){
	        break;
	    }
	    k=(long)(pio->level[ne]);
        xc=pio->xcenter[ne];
	    dx2=pio->dx2[k];
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
        yc=pio->ycenter[ne];
	    dy2=pio->dx2[k];
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
/*
		    sprintf(WarningBuff,"ne %ld length %g\n",ne,length);
		    WarningBatch(WarningBuff);
 */   
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
	
	if(length > 0){
	    double ll;
	    ll=sqrt(rx*rx+ry*ry);
		if(fabs(length-ll)/length > 1e-5){
		     sprintf(WarningBuff,"doIntegrateLine Line Length %g Not Equal To Integral Length %g\n",ll,length);
		     WarningBatch(WarningBuff);
		}
	}
    xD=NULL;
    yD=NULL;
	
	ret = 0;
OutOfHere:

	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	pio->pa.flagGradients=flagGradients;
	return ret;
}

static int doSage2DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	int ix1,iy1,ix2,iy2;
	double x1,y1,x2,y2,dl,ds,dx,dy,dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long CurrentFrame;
	struct FilePIOInfo *pio;
	long index;
	
	if(!li)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;
	
	if(pio->BlockSize <= 0 && uPref.BlockSizeGlobal > 0){
	    pio->BlockSize=uPref.BlockSizeGlobal;
	}
	
	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return doSage2DGetLineDataBlocks(Files,li);	
	}
	
	if(li->type == LINEDATA_POINT_DATA){
	     return doPointGetData(Files,li);
	}else if(li->type == LINEDATA_LIST_DATA){
	     return doPointGetListData(Files,li);
	}else if(li->type == LINEDATA_BHANGMETER_DATA){
		return doBhangMeterGetData(Files,li);
	}
	
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
		
	if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

    setPioScales(pio);
	

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81877))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
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

	    if(!pioGetValue(pio,x,y,&value,&index)){
	        continue;
	    }

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

static double valueSum;

static int CalculateBhangMeterMergeGroup(struct DrawData *d)
{
	struct FilePIOInfo *pio;
	struct linedata li;
	double nxx,nyy,nx,ny,mag,costh;
	double dx2,dy2;
	double xc,yc;
	int ix1,iy1,ix2,iy2;
	long n;
	int k,m;
	double **l351;
	double **l951;
	int numGroup;
	int ng;
	double sumValue;
	double pi;
	double surfaceArea;
	int *processed;
	
	
	sumValue=0;
	
	if(!d)goto ErrorOut;
	
	numGroup=d->numGroup;
	pio=d->pio;
	l951=d->l951;
	l351=d->l351;
	li=*d->li;
	processed=d->processed;

	pi=4.0*atan(1.0);
	
	nx=li.nx;
	ny=li.ny;
	
	mag=sqrt(nx*nx+ny*ny);
	if(mag > 0.0){
		nx /= mag;
		ny /= mag;
	}
	
	li.errors=0;
		
	ix1=0;
	iy1=0;
	ix2=0;
	iy2=0;
	for(n=0;n<pio->numcell;++n){
		double value[2000],xx,yy;
		double dl,r2;
		int found;
		long ne;
		
		
		if(processed[n])continue;
		
	    if(pio->daughter[n])continue;
		
		processed[n]=1;
		
		found=0;
		
		for(ng=0;ng<numGroup;++n){
		    if(l951[ng][n]){
			  found=1;
			  break;
			}
		}
		
	    if(found == 0.0)continue;
		
		
	    k=(int)(pio->level[n]);
		
	    xc=pio->xcenter[n];
	    yc=pio->ycenter[n];
		
		li.x1=xc;
		li.y1=yc;
		
	    dx2=pio->dx2[k];
		
	    dy2=pio->dy2[k];
		
		if(doIntegrateLineBhangMeter2(pio,&li))continue;
		
		ne=(long)(li.yData[0]);
		
		xx=(li.x2-xc);
		yy=(li.y2-yc);
		
		r2=(xx*xx+yy*yy);
		
		if(r2 <= 0.0)continue;
		
		nxx=xx/sqrt(r2);
		nyy=yy/sqrt(r2);
		
		costh= -(nx*nxx+ny*nyy);
		
		if(costh < 0.0){
		   continue;
		}
		
		dl=li.xData[1]-li.xData[0];
								
		surfaceArea=2*pi*costh*((xc+dx2)*(xc+dx2)-(xc-dx2)*(xc-dx2)+2*dy2*((xc+dx2)+(xc-dx2)));
		
		surfaceArea= surfaceArea/(4.0*pi*r2);
		
		for(ng=0;ng<numGroup;++ng){
			value[ng]=(l951[ng][ne])*surfaceArea;
		
			if(dl*l351[ng][ne] < 1e-7)
			{
				value[ng] *= 2.0*dl*l351[ng][ne];
			}
			else
			{
				value[ng] *= (1.0-exp(-2.0*dl*l351[ng][ne]));
			}
		}

		for(m=3;m<li.dCount;m += 2)
		{
			ne=(long)(li.yData[m]);
			
			dl=li.xData[m]-li.xData[m-1];
			
			
			for(ng=0;ng<numGroup;++ng){
				if(l351[ng][ne] <= 0.0){
					value[ng]=0.0;
				}else{
					value[ng]=value[ng]*exp(-dl*l351[ng][ne]);
				}
			}
			
/*
			if(!processed[ne])
			{
			    double value2;
				
				processed[ne]=1;
				
				k=(int)(pio->level[ne]);
				
				dx2=pio->dx2[k];
		
				dy2=pio->dy2[k];
	
				xc=pio->xcenter[ne];
				yc=pio->ycenter[ne];
			
				xx=(li.x2-xc);
				yy=(li.y2-yc);
			
				r2=(xx*xx+yy*yy);
		
				if(r2 <= 0.0)continue;
							
				surfaceArea=2*pi*((xc+dx2)*(xc+dx2)-(xc-dx2)*(xc-dx2)+2*dy2*((xc+dx2)+(xc-dx2)));
				
				value2=surfaceArea*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*r2);
				
				value2=xc*dy2*4.0*pi*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*(xx*xx+yy*yy));
						
				if(dl/lambda[ne] < 1e-7)
				{
					value2 *= 2.0*dl/lambda[ne];
				}
				else
				{
					value2 *= (1.0-exp(-2.0*dl/lambda[ne]));
				}
				value += value2;
			}
*/
		}
		
		for(ng=0;ng<numGroup;++ng){
			sumValue += value[ng];
		}
		
	}
	
	if(li.xData)cFree((char *)li.xData);
	if(li.yData)cFree((char *)li.yData);
	
ErrorOut:
	valueSum=sumValue;
	return 0;
}
static int BhangMeterGetGroupData(struct FileInfo2 *Files,struct linedata *li)
{
	char name[256];
	double dp;
	long ns,nd,nn;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	double pi;
	double *l351[2000];
	double *l951[2000];
	int numGroup;
	int ng;
	int ret;
	long lengthErrors;
	double start,end;
	int *processed;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	
	start=rtime();
	
	ret=1;
	
	processed=NULL;
	numGroup=0;
	
	xD=NULL;
	yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;
	
	lengthErrors=0;
	
	if(!li->multigroup){
	   return 1;
	}
	
	if(li->type != LINEDATA_BHANGMETER_DATA)return 1;
	
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
	
	if(ns < 1)goto OutOfHere;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns+1),81876))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns+1),81886))){
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

	pi=4.0*atan(1.0);
	
	
	
	nd=0;
	for(nn=nFirst;nn<=nLast;++nn){
		long dataLength;
		struct DrawData d;
				
	
	    CurrentFrame=nn;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
		numGroup=0;
		for(ng=0;ng<2000;++ng){
			sprintf(name,"%d(-1)",ng+351);
			l351[ng]=NULL;
			if(SageGetFileItem(Files,name,0,
							   &l351[ng],&dataLength,CurrentFrame)){
				ret=23;break;
			}
		
			sprintf(name,"%d(-1)",ng+951);
			l951[ng]=NULL;
			if(SageGetFileItem(Files,name,0,
							   &l951[ng],&dataLength,CurrentFrame)){
				ret=24;break;
			}
			numGroup=ng+1;
		}
		
		if(numGroup == 0)
		{
			sprintf(WarningBuff,"Error - Frame %ld No Multigroup data Found\n",nn);
			WarningBatch(WarningBuff);
			goto OutOfHere;

		}
		
		processed=(int *)cMalloc(pio->numcell*sizeof(int),92820);
		if(!processed){
		    ret=26;goto OutOfHere;
		}
		
		zerol((char *)processed,pio->numcell*sizeof(int));
		
		
		d.numGroup=numGroup;
		d.pio=pio;
		d.l351=l351;
		d.l951=l951;
		d.li=li;
		d.processed=processed;
		
		valueSum=0.0;
	
		CalculateBhangMeterMergeGroup(&d);

		for(ng=0;ng<numGroup;++ng){
			if(l351[ng]){
			   cFree((char *)l351[ng]);
			}
			l351[ng]=NULL;
			if(l951[ng]){
			   cFree((char *)l951[ng]);
			}
			l951[ng]=NULL;
		}
		
		numGroup=0;
		
		if(processed)cFree((char *)processed);
		processed=NULL;
		
	    dp=tFirst+(tLast-tFirst)*((double)(nn-nFirst))/(double)(nLast-nFirst);
	    if(nn >= 0 && nn < Files->ImageCount){
			struct PIO *sage;
			sage = &Files->PIO2DList[nn];
			dp=sage->time;
	    }
	    xD[nd]=dp;
	    yD[nd]=valueSum;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(valueSum < rymin)rymin=valueSum;
	    if(valueSum > rymax)rymax=valueSum;
	    nd++;
		/*
		sprintf(WarningBuff,"Frame %ld GatherBhangMeterValue %g\n",nn,valueSum);
		WarningBatch(WarningBuff);
		*/
	}
	
	
	if(!nd)goto OutOfHere;
	
	if(lengthErrors > 0)
	{
		sprintf(WarningBuff,"doBhangMeterGetData Line lengthErrors %ld \n",lengthErrors);
		WarningBatch(WarningBuff);
	}
	
	
	ret=0;
	
	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	xD=NULL;
	yD=NULL;
	
	pioSetLimits(Files,0L);
	
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	
	for(ng=0;ng<numGroup;++ng){
		if(l351[ng]){
		   cFree((char *)l351[ng]);
		}
		l351[ng]=NULL;
		if(l951[ng]){
		   cFree((char *)l951[ng]);
		}
		l951[ng]=NULL;
	}
	
	if(processed)cFree((char *)processed);
	
	end=rtime();
	/*
	sprintf(WarningBuff,"doBhangMeterGetData : %.2f Seconds Threads %d\n",end-start,pio->threads);
	WarningBatch(WarningBuff);
	*/
	return ret;
}

static int doBhangMeterGetData(struct FileInfo2 *Files,struct linedata *li)
{
	double dp;
	long ns,nd,nn;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	double pi;
	double *lambda;
	double *t;
	int ret;
	long lengthErrors;
	double start,end;
	int *processed;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	if(li->multigroup){
	   return BhangMeterGetGroupData(Files,li);
	}
	
	start=rtime();
	
	ret=1;
	
	t=NULL;
	lambda=NULL;
	processed=NULL;
	
	xD=NULL;
	yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;
	
	lengthErrors=0;
	
	if(li->type != LINEDATA_BHANGMETER_DATA)return 1;
	
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
	
	if(ns < 1)goto OutOfHere;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns+1),81876))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns+1),81886))){
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

	pi=4.0*atan(1.0);
	
	
	
	nd=0;
	for(nn=nFirst;nn<=nLast;++nn){
		long dataLength;
		struct DrawData d;
				
	
	    CurrentFrame=nn;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
				
		if(SageGetFileItem(Files,"lgt(-1)",0,
						   &lambda,&dataLength,CurrentFrame)){
		    ret=23;goto OutOfHere;
		}
		
		if(SageGetFileItem(Files,"tev",0,
						   &t,&dataLength,CurrentFrame)){
		    ret=24;goto OutOfHere;
		}
		
		
		processed=(int *)cMalloc(pio->numcell*sizeof(int),92820);
		if(!processed){
		    ret=26;goto OutOfHere;
		}
		
		zerol((char *)processed,pio->numcell*sizeof(int));
		
		
		d.pio=pio;
		d.lambda=lambda;
		d.li=li;
		d.t=t;
		d.processed=processed;
		
		valueSum=0.0;
	
		CalculateBhangMeterMerge(&d);

		if(t)cFree((char *)t);
		t=NULL;
		if(lambda)cFree((char *)lambda);
		lambda=NULL;
		
		if(processed)cFree((char *)processed);
		processed=NULL;
		
	    dp=tFirst+(tLast-tFirst)*((double)(nn-nFirst))/(double)(nLast-nFirst);
	    if(nn >= 0 && nn < Files->ImageCount){
			struct PIO *sage;
			sage = &Files->PIO2DList[nn];
			dp=sage->time;
	    }
	    xD[nd]=dp;
	    yD[nd]=valueSum;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(valueSum < rymin)rymin=valueSum;
	    if(valueSum > rymax)rymax=valueSum;
	    nd++;
		/*
		sprintf(WarningBuff,"Frame %ld GatherBhangMeterValue %g\n",nn,valueSum);
		WarningBatch(WarningBuff);
		*/
	}
	
	
	if(!nd)goto OutOfHere;
	
	if(lengthErrors > 0)
	{
		sprintf(WarningBuff,"doBhangMeterGetData Line lengthErrors %ld \n",lengthErrors);
		WarningBatch(WarningBuff);
	}
	
	
	ret=0;
	
	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	xD=NULL;
	yD=NULL;
	
	pioSetLimits(Files,0L);
	
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(t)cFree((char *)t);
	if(lambda)cFree((char *)lambda);
	if(processed)cFree((char *)processed);
	
	end=rtime();
	
	/*
	sprintf(WarningBuff,"doBhangMeterGetData : %.2f Seconds Threads %d\n",end-start,pio->threads);
	WarningBatch(WarningBuff);
	*/
	
	return ret;
}
static int CalculateBhangMeterMerge(struct DrawData *d)
{
	struct FilePIOInfo *pio;
	struct linedata li;
	double nx,ny,mag;
	double nxx,nyy;
	double dx2,dy2;
	double xc,yc;
	int ix1,iy1,ix2,iy2;
	long n;
	int k,m;
	double *lambda;
	double *t;
	double sumValue;
	double pi;
	double a=137.20172;
	double c=2.99792458e10;
	double surfaceArea;
	int *processed;
	double costh;
	
	
	sumValue=0;
	
	if(!d)goto ErrorOut;
	
	pio=d->pio;
	lambda=d->lambda;
	t=d->t;
	li=*d->li;
	processed=d->processed;

	pi=4.0*atan(1.0);
	
	nx=li.nx;
	ny=li.ny;
	
	mag=sqrt(nx*nx+ny*ny);
	if(mag > 0.0){
		nx /= mag;
		ny /= mag;
	}
	
	li.errors=0;
		
	ix1=0;
	iy1=0;
	ix2=0;
	iy2=0;
	for(n=0;n<pio->numcell;++n){
		double value,xx,yy;
		double dl,r2;
		long ne;
		
		
		if(processed[n])continue;
		
	    if(pio->daughter[n])continue;
		
		processed[n]=1;
		
	    if(t[n] <= 0.0)continue;
		
		
	    k=(int)(pio->level[n]);
		
	    xc=pio->xcenter[n];
	    yc=pio->ycenter[n];
		
		li.x1=xc;
		li.y1=yc;
		
	    dx2=pio->dx2[k];
		
	    dy2=pio->dy2[k];
		
		if(doIntegrateLineBhangMeter2(pio,&li))continue;
		
		ne=(long)(li.yData[0]);
		
		xx=(li.x2-xc);
		yy=(li.y2-yc);
		
		r2=(xx*xx+yy*yy);
		
		if(r2 <= 0.0)continue;
		
		nxx=xx/sqrt(r2);
		nyy=yy/sqrt(r2);
		
		costh= -(nx*nxx+ny*nyy);
		
		if(costh < 0.0){
		   continue;
		}
		
		dl=li.xData[1]-li.xData[0];
								
		surfaceArea=2*pi*costh*((xc+dx2)*(xc+dx2)-(xc-dx2)*(xc-dx2)+2*dy2*((xc+dx2)+(xc-dx2)));
		
		value=surfaceArea*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*r2);
		
		/* value=xc*dy2*4.0*pi*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*r2);*/
				

		if(dl/lambda[ne] < 1e-7)
		{
			value *= 2.0*dl/lambda[ne];
		}
		else
		{
			value *= (1.0-exp(-2.0*dl/lambda[ne]));
		}

		for(m=3;m<li.dCount;m += 2)
		{
			ne=(long)(li.yData[m]);
			
			if(lambda[ne] <= 0.0){
			    value=0.0;
				break;
			}
			
			dl=li.xData[m]-li.xData[m-1];
			
			value=value*exp(-dl/lambda[ne]);
/*
			if(!processed[ne])
			{
			    double value2;
				
				processed[ne]=1;
				
				k=(int)(pio->level[ne]);
				
				dx2=pio->dx2[k];
		
				dy2=pio->dy2[k];
	
				xc=pio->xcenter[ne];
				yc=pio->ycenter[ne];
			
				xx=(li.x2-xc);
				yy=(li.y2-yc);
			
				r2=(xx*xx+yy*yy);
		
				if(r2 <= 0.0)continue;
							
				surfaceArea=2*pi*((xc+dx2)*(xc+dx2)-(xc-dx2)*(xc-dx2)+2*dy2*((xc+dx2)+(xc-dx2)));
				
				value2=surfaceArea*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*r2);
				
				value2=xc*dy2*4.0*pi*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*(xx*xx+yy*yy));
						
				if(dl/lambda[ne] < 1e-7)
				{
					value2 *= 2.0*dl/lambda[ne];
				}
				else
				{
					value2 *= (1.0-exp(-2.0*dl/lambda[ne]));
				}
				value += value2;
			}
*/
		}
		
		sumValue += value;
		
	}
	
	if(li.xData)cFree((char *)li.xData);
	if(li.yData)cFree((char *)li.yData);
	
ErrorOut:
	valueSum=sumValue;
	return 0;
}
static int doIntegrateLineBhangMeter2(struct FilePIOInfo *pio,struct linedata *li)
{
	double *xD,*yD,s,s1,s2,s3,s4;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	double rx,ry;
	int ibreak;
	int iGet;
	int ret;
	
	if(!li)return 1;
	if(!pio)return 1;	
	
	ret = 1;
	iGet = 1;
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
	
	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	y1=li->y1;
	y2=li->y2;
	ry=y2-y1;
	
	
	np=10000;
	
	xD=li->xData;
	yD=li->yData;
	
   	li->xData=NULL;
	li->yData=NULL;
	
	if(!xD)
	{
		if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81876))){
			WarningBatch("doIntegrateLineBhangMeter Out of List Memory\n");
			goto OutOfHere;
		}
	}
	if(!yD)
	{
		if(!(yD=(double *)cMalloc((long)sizeof(double)*(np),81883))){
			WarningBatch("doIntegrateLineBhangMeter Out of List Memory\n");
			goto OutOfHere;
		}	
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
	
	while(1){
	    double value,dx2,dy2,xc,yc,dl,yy,xx;
	    long k;
		iGet=pioGetValueXX(pio,x+dx,y+dy,&value,&ne);
	    if(iGet){
		    if(iGet == 2){
			   ret=2;
			   goto OutOfHere;
			}
	        break;
	    }
	    k=(long)(pio->level[ne]);
        xc=pio->xcenter[ne];
	    dx2=pio->dx2[k];
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
        yc=pio->ycenter[ne];
	    dy2=pio->dx2[k];
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
		        sprintf(WarningBuff,"doIntegrateLineBhangMeter : Error Cell %ld No intersection\n",ne);
	    		WarningBatch(WarningBuff);
	    		goto OutOfHere;
		    }
	    }
	    
	    if(s < 0){
	        sprintf(WarningBuff,"doIntegrateLineBhangMeter : Error Cell %ld No length\n",ne);
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
	        	sprintf(WarningBuff,"doIntegrateLineBhangMeter : Error Too many Points %ld\n",nd);
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
	    yD[nd++]=ne;
		/*
		 sprintf(WarningBuff,"ne %ld length %g\n",ne,length);
		 WarningBatch(WarningBuff);
		 */   
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
	    yD[nd++]=ne;
	    
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
	
	if(length > 0){
	    double ll;
	    ll=sqrt(rx*rx+ry*ry);
		if(fabs(length-ll)/length > 1e-5){
			if(li->errors++ < 20){
				sprintf(WarningBuff,"doIntegrateLineBhangMeter Line Length %g Not Equal To Integral Length %g zones %ld\n",ll,length,nd/2);
				WarningBatch(WarningBuff);
			}
		}
	}
	
    xD=NULL;
    yD=NULL;
	
	ret = 0;
OutOfHere:
	
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	
	return ret;
}



static int doPointGetData(struct FileInfo2 *Files,struct linedata *li)
{
	double dp;
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt;
	double r,theta,dr,dtheta;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	double valuet;
	double x,y;
	double xs,ys;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
    long index;
  

	
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

	if(ns < 1)goto OutOfHere;

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns+1),81872))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns+1),81882))){
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

	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
	    
    	setPioScales(pio);
    	
    	
    	x=xs;
    	y=ys;
    	nf=0;
    	r=dr;
    	theta=0;
    	valuet=0;
    	for(nr=0;nr<NumberOfRings;++nr){
    	    theta=0;
    	    for(nt=0;nt<NumberOfPoints;++nt){
		    	if(pioGetValue(pio,x,y,&value,&index)){
		    		valuet += value;
		    		++nf;
		    	}
		    	
		    	x=xs+r*cos(theta);
		    	y=ys+r*sin(theta);
		    	theta += dtheta;
	    	}
		   	r += dr;	    	
	    }
    	
    	    
	    if(nf <= 0)continue;
	    
	    value=valuet/(double)nf;
	    	    
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(n >= 0 && n < Files->ImageCount){
	       struct PIO *sage;
	       sage = &Files->PIO2DList[n];
	       dp=sage->time;
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
static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double *v,long *index)
{
	double xc,yc;
	struct PIO *sage;
	long kk;
	double xx,yy;
	long i,j;
	long loc;
	long ne;

	double xmin,ymin;
	double dxset,dyset;
	double odxset,odyset;
	long ixmax;

	if(!pio || !v || !pio->value)return 0;
	if(!pio->location || !pio->daughter)return 0;
	sage=pio->sage;
	if(!sage)return 0;
		
	if(pio->pa.xReflect){
	   x=fabs(x);
	}
	
	xmin=pio->xmin;
	if(x < xmin || x > pio->xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin || y > pio->ymax)return 0;

	dxset=sage->dxset;
	odxset=1.0/dxset;
	dyset=sage->dyset;
	odyset=1.0/dyset;
	
	xx=x;
	yy=y;
	
	i=(long)((xx-xmin)*odxset);
	if(i < 0 || i >= sage->ixmax)return 0;
	j=(long)((yy-ymin)*odyset);
	if(j < 0 || j >= sage->iymax)return 0;

	ixmax=sage->ixmax;
	
    loc=i+j*ixmax;
    if(loc < 0 || loc >= ixmax*sage->iymax){
		return 0;
    }
    
/*    
    for(ne=0;ne< pio->numcell;++ne){
    	double dx,dy;
    	int k;
    	if(pio->daughter[ne])continue;
        k=(int)(pio->level[ne]);
        
        xx=pio->xcenter[ne];
        dx=pio->dx2[k]*1.000001;
        
        if(x < xx-dx || x > xx+dx)continue;
        
        yy=pio->ycenter[ne];
        dy=pio->dy2[k]*1.000001;
        
        if(y < yy-dy || y > yy+dy)continue;
    
    	if(pio->pa.flagGradients && pio->gradx && pio->grady){
        	xc=pio->xcenter[ne];
        	yc=pio->ycenter[ne];
			*v=pio->value[ne]+pio->gradx[ne]*(xx-xc)+pio->grady[ne]*(yy-yc);
		}else{
			*v=pio->value[ne];
		}
		if(index){
		    *index=ne;
		}
		return 1;
    
    }
    return 0;
    
*/    
    
    ne=pio->location[loc];
    
Start:
    if(ne < 0 || ne >= pio->numcell)return 0;
    
    if(!pio->daughter[ne]){    
    	if(pio->pa.flagGradients && pio->gradx && pio->grady){
        	xc=pio->xcenter[ne];
        	yc=pio->ycenter[ne];
			*v=pio->value[ne]+pio->gradx[ne]*(xx-xc)+pio->grady[ne]*(yy-yc);
		}else{
			*v=pio->value[ne];
		}
		if(index){
		    *index=ne;
		}
		return 1;
    }
    
	kk=0;
	if(x > pio->xcenter[ne])kk += 1; 
	if(y > pio->ycenter[ne])kk += 2; 
	ne=kk+(long)(pio->daughter[ne]-1);

	goto Start;
}


static int doCell(long ne,struct DrawInfo *info)
{
    struct FilePIOInfo *pio;
	double r1,r2,s1,s2,dx,dy,dr,ds;
	double xc,yc,dx2,dy2;
	double x,y,r,s,rx,ry,sx,sy;
	long k,n,kk,nx,ny;
	long i1,i2,j1,j2;
	long i,j;
	double *sout;
	int flagGradients;
	
	if(!info)return 1;
	pio=info->pio;
	if(!pio)return 1;
    if(ne < 0 || ne >= pio->numcell)return 1;
	
	rx=info->rx;
	ry=info->ry;
	sx=info->sx;
	sy=info->sy;
	
    k=(long)(pio->level[ne]);
	
	xc=pio->xcenter[ne];
    dx2=pio->dx2[k];
    dx=dx2*2.0;
    x=xc-dx2-info->p1x;
        	
	yc=pio->ycenter[ne];
    dy2=pio->dy2[k];
    dy=dy2*2.0;
    y=yc-dy2-info->p1y;
    
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
    
    if(r1 > 1.0 || r2 < 0.0)return 0;
    
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
    
    if(s1 > 1.0 || s2 < 0.0)return 0;
    
    
    if(pio->daughter[ne]){   
		for(kk=0;kk<4;++kk){		
	        n=(kk+(long)(pio->daughter[ne]-1));	        
			doCell(n,info);
		}
		return 0;
	}
	
	nx=info->nx;
	ny=info->ny;
	
	dr=info->dr;
	ds=info->ds;

	sout=info->sout;
	
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
	
	flagGradients=(pio->pa.flagGradients && pio->gradx && pio->grady);
	
	for(j=j1;j<=j2;++j){
	    double v;
	    s=((double)j+0.5)*ds;
	    for(i=i1;i<=i2;++i){
	        r=((double)i+0.5)*dr;
	        x=r*info->v1x+s*info->v2x+info->p1x;
	        if(x < xc-dx2 || x > xc+dx2)continue;
	        y=r*info->v1y+s*info->v2y+info->p1y;
	        if(y < yc-dy2 || y > yc+dy2)continue;
	    	if(flagGradients){
				v=pio->value[ne]+pio->gradx[ne]*(x-xc)+pio->grady[ne]*(y-yc);
			}else{
				v=pio->value[ne];
			}
			sout[i+(ny-1-j)*nx]=v;
	    }
	}


	return 0;
}
static int doSage2DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct DrawInfo info;
	struct PIO *sage;
	
	long nmax,n,ne;
	
	
	if(!ai || !ai->data || !Files)return 1;
	pio=&Files->pioData;
	sage=pio->sage;
	if(!sage || !pio->value || !pio->location || !pio->daughter)return 1;
	
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
	    sprintf(WarningBuff,"doSage2DGetGeneratData - Error l1 %g l2 %g d %g\n",info.l1,info.l2,info.d);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	info.od = 1.0/info.d;
	
	info.rx =  info.v2y*info.od;
	info.ry = -info.v2x*info.od;
	
	info.sx = -info.v1y*info.od;
	info.sy =  info.v1x*info.od;
	

	info.dxset=sage->dxset;
	info.odxset=1.0/info.dxset;
	info.dyset=sage->dyset;
	info.odyset=1.0/info.dyset;
		
	nmax=sage->ixmax*sage->iymax;
	
	
	for(n=0;n<nmax;++n){	
    	ne=pio->location[n];    	
    	if(doCell(ne,&info))break;
    }	
    
	return  0;
}
static int doSage2DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
	struct PlotRangeData saveRange;
    struct FilePIOInfo *pio;
	long CurrentFrame;
	double *data;
	int ret;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;

	ret=1;
	
	saveRange=pio->range;

	CurrentFrame=ai->CurrentFrame;
	
	FilesSetFrame(CurrentFrame,Files);
	
	if(pio->pa.BlockSize <= 0 && uPref.BlockSizeGlobal > 0){
	    pio->pa.BlockSize=uPref.BlockSizeGlobal;
	}
	
	pio->BlockSize= pio->pa.BlockSize;
	
	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return doSage2DGetAreaDataBlock(Files,ai);
	}

	if(ai->type != GENERAL_PLANE_DATA && ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"doSage2DGetAreaData - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}

	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}else{
	    mstrncpy(ai->pioName,pio->pioName,255);
	    ai->pioIndex=pio->pioIndex;
	}

	if(SetPIOData(Files,CurrentFrame))return 1;
		
	ai->pioTime=Files->pioTime;
	
	if(ai->xsize <= 0 || ai->ysize <= 0)return 1;
	
	if(ai->zsize > 1){
	    sprintf(WarningBuff,"doSage2DGetAreaData - Error cannot return Volume Data Sets\n");
	    WarningBatch(WarningBuff);
	    return 1;	
	}
	
	data=(double *)cMalloc(ai->xsize*ai->ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,ai->xsize*ai->ysize*sizeof(double));
	
	ai->data=data;
	 	
	if(ai->type == GENERAL_PLANE_DATA){
	    return doSage2DGetGeneratData(ai,Files);
	}
	
	
	
	pio->range.ixmax=ai->xsize;
	pio->range.iymax=ai->ysize;
	
	pio->range.ixminData=0;
	pio->range.iyminData=0;
	
	pio->range.ixmaxData=ai->xsize;
	pio->range.iymaxData=ai->ysize;
	
	pio->range.xminData=ai->xminArea;
	pio->range.xmaxData=ai->xmaxArea;
	pio->range.yminData=ai->yminArea;
	pio->range.ymaxData=ai->ymaxArea;
	
    if(getPlotData(Files,data,CurrentFrame)){
	    sprintf(WarningBuff,"doSage2DGetAreaData - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        goto ErrorOut;
    }	    
	

	ret=0;
	
ErrorOut:

	pio->range=saveRange;
	    
	return ret;
}
static int doSage2DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
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
	sd->streamline2d=pio->streamline2d;

	if(pio->pioName[0]){
	    mstrncpy(sd->pioName,pio->pioName,255);
	    sd->pioIndex=(int)pio->pioIndex;
	}else{
	   sd->pioName[0]=0;
	   pio->pioIndex=0;
	}

	
	return 0;
}
int convertTO24(struct SetFrameData  *sd,struct paletteDraw *pd)
{
	long n;
	unsigned char ic,*s,*d;
	
	if(!sd || !pd || !sd->data)return 1;
	
	s=d=(unsigned char *)sd->data;
	
	for(n=sd->length-1;n >= 0;--n){
	    ic=s[n];
	    if(ic == 0){
		    d[3*n]=(unsigned char)(255*pd->red);
		    d[3*n+1]=(unsigned char)(255*pd->green);
		    d[3*n+2]=(unsigned char)(255*pd->blue);
	
	    }else{
		    d[3*n]=pd->palette[3*ic];
		    d[3*n+1]=pd->palette[3*ic+1];
		    d[3*n+2]=pd->palette[3*ic+2];
	    }
	}
	
	return 0;
}
static int RangeCellVelocity(long n,struct FilePIOInfo *pio,double *vmax)
{
	double dx2,dy2;
	double xc,yc;
	double *xv,*yv;
	double v;
	int k,kk;
	long ne;
	    
	if(!pio || !vmax)return 1;
	
    k=(int)(pio->level[n]);

    if(!pio->daughter[n])goto DrawIt;
    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<4;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        RangeCellVelocity(ne,pio,vmax);
		}      
    }
    
    return 0;
    
DrawIt:
    
    xc=pio->xcenter[n];
    yc=pio->ycenter[n];
    dx2=pio->dx2[k];
    dy2=pio->dy2[k];
    xv=pio->vectorx;
    yv=pio->vectory;
    
    if(xc+dx2 < pio->range.xminData)return 0;
    if(xc-dx2 > pio->range.xmaxData)return 0;
    if(yc+dy2 < pio->range.yminData)return 0;
    if(yc-dy2 > pio->range.ymaxData)return 0;
    v=sqrt(xv[n]*xv[n]+yv[n]*yv[n]);
    if(*vmax < v)*vmax = v;
   	    
	return 0;
}
static int loadVectors(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	struct PIO *sage;
	double vmax;
	long length;
	long n,ne;
	
	if(!Files)return 1;
	
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage)return 1;

	if(mstrcmp(pio->vector.vectorxName,pio->vectorxNameOld)){
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	}
	
	if(mstrcmp(pio->vector.vectoryName,pio->vectoryNameOld)){
	    if(pio->vectory)cFree((char *)pio->vectory);
	    pio->vectory=NULL;
	}
		
	
	if(!pio->vectorx || !pio->vectory || (pio->CurrentFrameVectors != pio->CurrentFrame)){
	
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	    
	    if(pio->vectory)cFree((char *)pio->vectory);
	    pio->vectory=NULL;
	    
	    
	    pio->CurrentFrameVectors = -1;  
	      
		if(SageGetFileItem(Files,pio->vector.vectorxName,0,
		                  &pio->vectorx,&length,pio->CurrentFrame)){
			goto ErrorOut;
		}
		
		if(length != pio->numcell)goto ErrorOut;

		if(SageGetFileItem(Files,pio->vector.vectoryName,0,
		                  &pio->vectory,&length,pio->CurrentFrame)){
			goto ErrorOut;
		}	  
		  
		if(length != pio->numcell)goto ErrorOut;
		
		
	    pio->CurrentFrameVectors = pio->CurrentFrame;  	   
	    
	    
		length=sage->ixmax*sage->iymax*sage->izmax;
	    vmax=0;	    
        for(n=0;n<length;++n){
	    	ne=pio->location[n];	    
        	RangeCellVelocity(ne,pio,&vmax);
        }
	    	    	    
	    pio->vmaxVectors=vmax;
	     
	    mstrncpy(pio->vectorxNameOld,pio->vector.vectorxName,32);
	    mstrncpy(pio->vectoryNameOld,pio->vector.vectoryName,32);
	    mstrncpy(pio->vectorzNameOld,pio->vector.vectorzName,32);
	    
	}
	
	
	return 0;
ErrorOut:
	return 1;
}
static int pioDrawBhangMeter2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	double time,start,end;
	struct DrawData d;
	struct PIO *sage;
	
	
	pio = &Files->pioData;
	sage=pio->sage;
	
	if(!sage || !pio->location)return 1;
	
	
	if(!pio || !ss || !r || !pio->value)return 1;
	
	if(!pio->gradx || !pio->grady)
	{
		WarningBatch("pioDrawBhangMeter2d Needs Gradients Turned On \n");
		return 1;
	}
	
	d.pio=pio;
	d.ss=ss;
	d.r=r;
	
	start=rtime();

	runThreads(pio->threads,&d,pio->numcell,pioDrawBhangMeter2dT);
	
	end=rtime();
	
	time=(end-start);
	if(time < 0)time = 1;

	sprintf(WarningBuff,"BhangMeter %.2f Seconds \n",time);
	WarningBatch(WarningBuff);
	
	return 0;
}
	
static int pioDrawBhangMeter2dT(mThread *Threads)
{
	struct BhangMeterData *b;
	struct FilePIOInfo *pio;
	struct screenData *ss;
	struct linedata li;
	struct dRange *r;
	struct DrawData *d;
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2;
	double targetValue;
	unsigned char nc;
	int ix1,iy1,ix2,iy2;
	long n;
	int k,m;
	
	
	if(!Threads)goto ErrorOut;
	d=(struct DrawData *)Threads->data;
	if(!d)goto ErrorOut;
	
	pio=d->pio;
	ss=d->ss;
	r=d->r;
	
	zerol((char *)&li,sizeof(li));
	
	b=&pio->bhangmeter2d;
	
	targetValue=b->targetValue;
	
	li.x2=b->xyz.x[0];
	li.y2=b->xyz.y[0];
	
	ix1=0;
	iy1=0;
	ix2=0;
	iy2=0;
	for(n=Threads->smin;n<Threads->smax;++n){
		double trans;
	    k=(int)(pio->level[n]);
	    if(pio->daughter[n])continue;
		
		if(pio->value[n] <= 0.0)continue;
		
	    nc=255;
		/*
	    if(pio->pa.DrawZonesColor){
	        FloatToColor(&pio->value[n],&pio->pd,&nc);
	    }
		 */
	    xc=pio->xcenter[n];
	    yc=pio->ycenter[n];
		
		li.x1=xc;
		li.y1=yc;
						
		doIntegrateLineBhangMeter(pio,&li);
		
		trans=1.0;
		
		for(m=1;m<li.dCount;m += 2)
		{
			double value,dx;
			
			dx=li.xData[m]-li.xData[m-1];
			
			value=li.yData[m];
			
			if(value > 0){
				trans=trans*exp(-dx/value);
				if(trans < 1.0e-6)break;
			}
		}
		
	    dx2=pio->dx2[k];
	    dy2=pio->dy2[k];
		
		if(trans > 1.0e-6)continue;
		
	    x1=xc-dx2;
	    y1=yc-dy2;
	    x2=xc+dx2;
	    y2=yc-dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }
		
	    x1=xc+dx2;
	    y1=yc-dy2;
	    x2=xc+dx2;
	    y2=yc+dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }
		
	    x1=xc+dx2;
	    y1=yc+dy2;
	    x2=xc-dx2;
	    y2=yc+dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }
		
	    x1=xc-dx2;
	    y1=yc+dy2;
	    x2=xc-dx2;
	    y2=yc-dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix1,&iy1);
	        RealLoc(pio,&x2,&y2,&ix2,&iy2);
			LineC(ix1,iy1,ix2,iy2,nc,ss);
	    }
		
	}
	
	if(li.xData)cFree((char *)li.xData);
	if(li.yData)cFree((char *)li.yData);

ErrorOut:

	Threads->done=TRUE;	
	return 0;
}

static int doIntegrateLineBhangMeter(struct FilePIOInfo *pio,struct linedata *li)
{
	double *xD,*yD,s,s1,s2,s3,s4;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	double rx,ry;
	int ibreak;
	int ret;
	
	if(!li)return 1;
	if(!pio)return 1;	
	
	ret = 1;
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
	
	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	y1=li->y1;
	y2=li->y2;
	ry=y2-y1;
	
	
	np=10000;
	
	xD=li->xData;
	yD=li->yData;
	
   	li->xData=NULL;
	li->yData=NULL;
	
	if(!xD)
	{
		if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81876))){
			WarningBatch("doIntegrateLineBhangMeter Out of List Memory\n");
			goto OutOfHere;
		}
	}
	if(!yD)
	{
		if(!(yD=(double *)cMalloc((long)sizeof(double)*(np),81883))){
			WarningBatch("doIntegrateLineBhangMeter Out of List Memory\n");
			goto OutOfHere;
		}	
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
	
	while(1){
	    double value,dx2,dy2,xc,yc,dl,yy,xx;
	    long k;
	    if(!pioGetValue(pio,x+dx,y+dy,&value,&ne)){
	        break;
	    }
	    k=(long)(pio->level[ne]);
        xc=pio->xcenter[ne];
	    dx2=pio->dx2[k];
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
        yc=pio->ycenter[ne];
	    dy2=pio->dx2[k];
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
		        sprintf(WarningBuff,"doIntegrateLineBhangMeter : Error Cell %ld No intersection\n",ne);
	    		WarningBatch(WarningBuff);
	    		goto OutOfHere;
		    }
	    }
	    
	    if(s < 0){
	        sprintf(WarningBuff,"doIntegrateLineBhangMeter : Error Cell %ld No length\n",ne);
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
	        	sprintf(WarningBuff,"doIntegrateLineBhangMeter : Error Too many Points %ld\n",nd);
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
		/*
		 sprintf(WarningBuff,"ne %ld length %g\n",ne,length);
		 WarningBatch(WarningBuff);
		 */   
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
	
	if(length > 0){
	    double ll;
	    ll=sqrt(rx*rx+ry*ry);
		if(fabs(length-ll)/length > 1e-5){
			sprintf(WarningBuff,"doIntegrateLineBhangMeter Line Length %g Not Equal To Integral Length %g\n",ll,length);
			WarningBatch(WarningBuff);
		}
	}
	
    xD=NULL;
    yD=NULL;
	
	ret = 0;
OutOfHere:
	
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	
	return ret;
}
static int pioGetValueXX(struct FilePIOInfo *pio,double x,double y,double *v,long *index)
{
	struct PIO *sage;
	long kk;
	double xx,yy;
	long i,j;
	long loc;
	long ne;

	double xmin,ymin;
	double dxset,dyset;
	double odxset,odyset;
	long ixmax;

	if(!pio || !v || !pio->value)return 1;
	if(!pio->location || !pio->daughter)return 1;
	sage=pio->sage;
	if(!sage)return 1;
		
	if(pio->pa.xReflect){
	   x=fabs(x);
	}
	
	xmin=pio->xmin;
	if(x < xmin || x > pio->xmax)return 1;
	
	ymin=pio->ymin;
	if(y < ymin || y > pio->ymax)return 1;

	dxset=sage->dxset;
	odxset=1.0/dxset;
	dyset=sage->dyset;
	odyset=1.0/dyset;
	
	xx=x;
	yy=y;
	
	i=(long)((xx-xmin)*odxset);
	if(i < 0 || i >= sage->ixmax)return 1;
	j=(long)((yy-ymin)*odyset);
	if(j < 0 || j >= sage->iymax)return 1;

	ixmax=sage->ixmax;
	
    loc=i+j*ixmax;
    if(loc < 0 || loc >= ixmax*sage->iymax){
		return 1;
    }
    
    
    ne=pio->location[loc];
    
Start:
    if(ne < 0 || ne >= pio->numcell)return 1;
    
    if(!pio->daughter[ne]){    
		*v=pio->value[ne];
		if(index){
		    *index=ne;
		}
        if(pio->active){
		    if(pio->active[ne]){
			    return 0;
			}else{
			    return 2;
			}
        }else{
			return 0;
		}
    }
    
	kk=0;
	if(x > pio->xcenter[ne])kk += 1; 
	if(y > pio->ycenter[ne])kk += 2; 
	ne=kk+(long)(pio->daughter[ne]-1);

	goto Start;
}

static int pioDrawStream2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	struct PIO *sage;
    double xmin,xmax,ymin,ymax;
    double didx,djdy;
    double dxdi,dydj,dv;
    long ixmin,ixmax,iymin,iymax;
	double x,y,x1,y1,x2,y2,vx,vy,vlen,sx,sy;
	double *lo_x,*hi_x,*lo_y,*hi_y;
	double gradxV1,gradyV1;
	double gradxV2,gradyV2;
	double gradl,gradh;
	int ix,iy;
	long n,nn,ne,k,n2,k2,ke;
	int streamGradients;
	long dataLength;
    long streamSteps;
	long nstep;
	int nGrad;
	int ret;

	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage || !pio->location)return 1;
	
	ret = 1;
	
	loadVectors(Files);

	lo_x=hi_x=lo_y=hi_y=NULL;
	
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
    
    dv=min(dxdi,dydj);
    
    streamSteps=pio->streamline2d.streamSteps;
    if(streamSteps <= 0)streamSteps=2000;
    streamGradients=pio->streamline2d.streamGradients;
	
	
	    if(streamGradients){
			if(SageGetFileItem(Files,"cell_index",1,
			                  &lo_x,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",2,
			                  &hi_x,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",3,
			                  &lo_y,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",4,
			                  &hi_y,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
	    }
	    
	    for(n=0;n<pio->streamline2d.zPlane.count;++n){
	    	double v;
	        for(k=0;k<2;++k){
		        x1=x=pio->streamline2d.zPlane.x[n];
		        y1=y=pio->streamline2d.zPlane.y[n];
		        ne=-1;
		        nn=n;
		        nstep=0;
		        nGrad=-1;
	    		gradxV1=0;
	    		gradyV1=0;
	    		gradxV2=0;
	    		gradyV2=0;
doAgain3:
				
		    	if(!pioGetValue(pio,x,y,&v,&ne))continue;
	    		if(streamGradients){
	    		    if(nGrad != ne){
			    		gradxV1=0;
			    		gradyV1=0;
			    		gradxV2=0;
			    		gradyV2=0;
			    		
			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorx[ne]-pio->vectorx[n2])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorx[n2]-pio->vectorx[ne])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
						gradxV1 = 0.5*(gradl+gradh);
						/*
						if((gradh*gradl >= 0.0)){
							if(fabs(gradh) < fabs(gradl)){
								gradxV1 = gradh;
							}else{
								gradxV1 = gradl;
							}
						}else{
							gradxV1 = 0;
						}	                		                		                	
	                	*/		    		
			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
						gradxV2 = 0.5*(gradl+gradh);
						/*
						if((gradh*gradl >= 0.0)){
							if(fabs(gradh) < fabs(gradl)){
								gradxV2 = gradh;
							}else{
								gradxV2 = gradl;
							}
						}else{
							gradxV2 = 0;
						}	                		                		                	
	                	*/	    			                				    		
			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorx[ne]-pio->vectorx[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorx[n2]-pio->vectorx[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradyV1 = 0.5*(gradl+gradh);
	                	/*             				    		
						if((gradh*gradl >= 0.0)){
							if(fabs(gradh) < fabs(gradl)){
								gradyV1 = gradh;
							}else{
								gradyV1 = gradl;
							}
						}else{
							gradyV1 = 0;
						}	
						*/                		                		                	
			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradyV2 = 0.5*(gradl+gradh);

	                	/*
						if((gradh*gradl >= 0.0)){
							if(fabs(gradh) < fabs(gradl)){
								gradyV2 = gradh;
							}else{
								gradyV2 = gradl;
							}
						}else{
							gradyV2 = 0;
						}	 
						*/               		                		                	
                	
	    			}
			    		

	    		    nGrad=ne;
		    	    vx= pio->vectorx[ne]+gradxV1*(x-pio->xcenter[ne])+gradyV1*(y-pio->ycenter[ne]);
		    	    vy= pio->vectory[ne]+gradxV2*(x-pio->xcenter[ne])+gradyV2*(y-pio->ycenter[ne]);
/*		    	    
		    	    sprintf(WarningBuff,"ne %ld n2 %ld %g %g %g %g",ne,n2,gradxV1,gradxV2,gradyV1,gradyV2);
		    	    WarningBatchFast(WarningBuff);
*/
	    		}else{
		    	    vx= pio->vectorx[ne];
		    	    vy= pio->vectory[ne];
	    		}
		   
		    	vlen=sqrt(vx*vx+vy*vy);
		    	
		    	if(vlen > 0){
		    		if(k == 0){
		    	    	sx=dv*vx/vlen;
		    			sy=dv*vy/vlen;
		    		}else{
		    	    	sx = -dv*vx/vlen;
		    			sy = -dv*vy/vlen;
		    		}
		    		x2=x1+sx;  /* move one screen pixel distance */
		    		y2=y1+sy;	    		

				    if(!isOut(&x1,&y1,&x2,&y2,r)){
				        RealLoc(pio,&x1,&y1,&ix,&iy);
				        MoveB(ix,iy);
				        RealLoc(pio,&x2,&y2,&ix,&iy);
				        LineB(ix,iy,255,ss);
				    }
		    		
		    		
		    	}else{
		    		x2=x;
		    		y2=y;	    		
		    		
				    if(!isOut(&x1,&y1,&x2,&y2,r)){
				        RealLoc(pio,&x1,&y1,&ix,&iy);
				        MoveB(ix,iy);
				        RealLoc(pio,&x2,&y2,&ix,&iy);
				        LineB(ix,iy,255,ss);
				    }
		    		continue;
		    	}    	
		    	  	
		    	x=x1=x2;
		    	y=y1=y2;
		    	if(++nstep > streamSteps)continue;
		    	goto doAgain3;
	    	}
	    	
	    	
	    }
	
	
	/*
	WarningBatchAtEnd(WarningBuff);
	*/

	ret=0;
ErrorOut:
	if(lo_x)cFree((char *)lo_x);
	if(lo_y)cFree((char *)lo_y);
	if(hi_x)cFree((char *)hi_x);
	if(hi_y)cFree((char *)hi_y);
	return ret;
}

static int pioDraw2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
    struct FilePIOInfo *pio;
    static double  small2=1e-8;
	struct dRange rr;
	double *d;
	long length;
	
	
	if(!Files || !ss || !buff || !data)return 1;
	pio = &Files->pioData;

	setPioScales(pio);

	rr.xmin=pio->range.xminData;
	rr.ymin=pio->range.yminData;
	rr.xmax=pio->range.xmaxData;
	rr.ymax=pio->range.ymaxData;

	rr.dxlim=small2*(rr.xmax-rr.xmin);
	if(rr.dxlim <= 0.0)rr.dxlim=small2;
	rr.dylim=small2*(rr.ymax-rr.ymin);
	if(rr.dylim <= 0.0)rr.dylim=small2;

	length=pio->range.ixmax*pio->range.iymax;
	d=data;
	setFloat(d,length);
    if(getPlotData(Files,d,CurrentFrame)){
	    sprintf(WarningBuff,"pioDraw2 - getPlotData Error\n");
	    WarningBatch(WarningBuff);
		return 1;
    }	

	if(pio->pd.AreaFill){

		memcpy(buff,d,length*sizeof(double));
		
		d=(double *)buff;
	    	    
	    FloatToImage(d,length,&pio->pd,NULL);

		if(pio->active){
		    pioDrawVoid(Files,(unsigned char *)d);
		}

	}
	
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	
	if(pio->pa.DrawZones){
		pioDrawZones(pio,ss,&rr);
	}
	
	if(pio->vector.DrawVectors){
		pioDrawVectors(Files,ss,&rr);
	}
	
	if(pio->streamline.DrawStreamlines){
	   pioDrawStreamlines(Files,ss,&rr);
	}
	
	if(pio->streamline2d.streamOn){
		pioDrawStream2d(Files,ss,&rr);
	}
	
	if(pio->bhangmeter2d.bhangOn){
		pioDrawBhangMeter2d(Files,ss,&rr);
	}
	
	if(pio->LaserPlotCount){
	   pioDrawLasers2d(Files,ss,&rr);
	}
	
	if(pio->TracerPlotCount){
	   pioDrawTracers2d(Files,ss,&rr);
	}
	
	return 0;
}
static int getPlotImage2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame)
{
	struct PlotRangeData range;
    struct FilePIOInfo *pio;
	struct screenData ss;
	unsigned char *buff2,save;
	double *data2,save2;
    double dx,s;
    long dxi,isplit,ihigh,ilow;
    int ret;
    long i,j;
	
	if(!Files || !buff || !data)return 1;
	
	buff2=NULL;
	data2=NULL;
	pio = &Files->pioData;
	
	
	zerol((char *)&ss,sizeof(struct screenData));
	ss.viewRect.x=0;
	ss.viewRect.y=0;
	ss.viewRect.xsize=(int)(pio->range.ixmax);
	ss.viewRect.ysize=(int)(pio->range.iymax);
	ss.buffer=buff;
	ss.ScreenBits=8;
	ss.palette=&pio->pd.palette[0];
	ss.ZoneLevel=pio->pa.ZoneLevel;
	if(pio->pa.DrawZones < 2)ss.ZoneLevel=99999;

    setPioScales(pio);

	range=pio->range;

	if(!Files->Reflect_x || pio->range.xminData >= 0.0){ /* no Reflection or no reflected data showing */
		pioDraw2(Files,data,buff,CurrentFrame,&ss);
	}else if(pio->range.xmaxData <= 0){  /* doing reflected data with no unreflected data showing */
		pio->range.xminData= -range.xmaxData;
		pio->range.xmaxData= -range.xminData;
		ret=pioDraw2(Files,data,buff,CurrentFrame,&ss);
		if(ret)goto ErrorOut;
		for(j=range.iyminData;j<range.iymaxData;++j){
		     ihigh=range.ixmaxData-1;
		     ilow=range.ixminData;
		     for(i=0;i<(range.ixmaxData-range.ixminData)/2;++i){
		          save=buff[ilow+(range.iymax-1-j)*pio->range.ixmax];
		          buff[ilow+(range.iymax-1-j)*pio->range.ixmax]=buff[ihigh+(range.iymax-1-j)*range.ixmax];
		          buff[ihigh+(range.iymax-1-j)*range.ixmax]=save;
		          save2=data[ilow+(range.iymax-1-j)*pio->range.ixmax];
		          data[ilow+(range.iymax-1-j)*pio->range.ixmax]=data[ihigh+(range.iymax-1-j)*range.ixmax];
		          data[ihigh+(range.iymax-1-j)*range.ixmax]=save2;
		          --ihigh;
		          ++ilow;
		     }
		}
	}else{   /* have some unreflected data showing */
			
		
		dx=pio->range.xmaxData-pio->range.xminData;
		if(dx <= 0)return 1;
		dxi=pio->range.ixmaxData-pio->range.ixminData;
		s=(0.0-pio->range.xminData)/dx;
		isplit=(long)(s*(double)dxi+0.5)+pio->range.ixminData;
		
		if(fabs(pio->range.xminData) <=  pio->range.xmaxData){  /* do unreflected data and flip to negative side */
		
			ss.buffer=buff;			
			pio->range.xminData=0;
			pio->range.ixminData=isplit;
			ret=pioDraw2(Files,data,buff,CurrentFrame,&ss);
			if(ret)goto ErrorOut;			
			
			for(j=0;j< range.iymax;++j){
			     ihigh=isplit;
			     ilow=isplit-1;
			     for(i=0;i<(isplit-range.ixminData);++i){
			          buff[ilow+(range.iymax-1-j)*pio->range.ixmax]=buff[ihigh+(range.iymax-1-j)*range.ixmax];
			          data[ilow+(range.iymax-1-j)*pio->range.ixmax]=data[ihigh+(range.iymax-1-j)*range.ixmax];
			          ++ihigh;
			          --ilow;
			     }
			}				
			

		}else{   /* have more reflected data than unreflected data */
			buff2=(unsigned char *)cMalloc(pio->range.ixmax*pio->range.iymax*sizeof(double),1847);
			if(!buff2)goto ErrorOut;
			data2=(double *)cMalloc(pio->range.ixmax*pio->range.iymax*sizeof(double),1847);
			if(!data2)goto ErrorOut;
			ss.buffer=buff2;			
			pio->range.xminData=0;
			pio->range.ixminData=range.ixminData;
			pio->range.ixmaxData=isplit;
			pio->range.xmaxData= -range.xminData;
			ret=pioDraw2(Files,data2,buff2,CurrentFrame,&ss);
			if(ret)goto ErrorOut;
			
			
			for(j=0;j< range.iymax;++j){
			     ihigh=isplit;
			     ilow=range.ixminData;
			     for(i=0;i<(range.ixmaxData-isplit);++i){
			          buff[ihigh+(range.iymax-1-j)*pio->range.ixmax]=buff2[ilow+(range.iymax-1-j)*range.ixmax];
			          data[ihigh+(range.iymax-1-j)*pio->range.ixmax]=data2[ilow+(range.iymax-1-j)*range.ixmax];
			          ++ihigh;
			          ++ilow;
			     }
			}			
				
			for(j=0;j< range.iymax;++j){
			     ihigh=isplit-1;
			     ilow=range.ixminData;
			     for(i=0;i<(isplit-range.ixminData);++i){
			          buff[ilow+(range.iymax-1-j)*pio->range.ixmax]=buff2[ihigh+(range.iymax-1-j)*range.ixmax];
			          data[ilow+(range.iymax-1-j)*pio->range.ixmax]=data2[ihigh+(range.iymax-1-j)*range.ixmax];
			          --ihigh;
			          ++ilow;
			     }
			}			
			
		}
	
	}

ErrorOut:
	pio->range=range;
	pioSetLimits(Files,CurrentFrame);
	if(buff2)cFree((char *)buff2);
	if(data2)cFree((char *)data2);
	return 0;
}
static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    int ret;


	if(!Files)return 1;
	pio=&Files->pioData;
	if(!sd)return 1;
	limits=&Files->limits;
	
	ret=1;
	
	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
		pio->pa     = sd->pa;		
	}
 
 	if(SetPIOData(Files,CurrentFrame))goto ErrorOut;

    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9203);
    if(!sd->data){
    	sprintf(WarningBuff,"FloatAndRaster - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }

    zerol((char *)sd->data,length*sizeof(double));
    
    sd->data2=cMalloc(length*sizeof(double),9204);
    if(!sd->data2){
    	sprintf(WarningBuff,"FloatAndRaster - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }
    
    zerol((char *)sd->data2,length*sizeof(double));
    
    if(getPlotImage2(Files,(double *)sd->data,(unsigned char *)sd->data2,CurrentFrame)){
		goto ErrorOut;
    }	
    
    
    
	sd->pd.dmin=pio->cmin;
	sd->pd.dmax=pio->cmax;	
    sd->limits = *limits;	
    
	ret=0;
	
 ErrorOut:
    
    return ret;
    
}
static int doSage2DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    double *sout;
    int ret;


	if(!Files)return 1;
	pio=&Files->pioData;
	if(!sd)return 1;
	limits=&Files->limits;
	
	FilesSetFrame(CurrentFrame,Files);
	
	if(sd->pa.BlockSize <= 0 && uPref.BlockSizeGlobal > 0){
	    sd->pa.BlockSize=uPref.BlockSizeGlobal;
	}
	
	pio->BlockSize=sd->pa.BlockSize;
	
	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return doSage2DGetDataBlocks(Files,CurrentFrame,sd);	
	}

	

 	if(sd->type == FRAME_DATA_FLOAT_RASTER){
 	    return FloatAndRaster(Files,CurrentFrame,sd);
 	}

	ret=1;

 	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           ;
	    }else if(sd->type != FRAME_DATA_RASTER){
		    sprintf(WarningBuff,"doSage2DGetData - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
	    }else if(!pio->pa.DrawZones && !pio->vector.DrawVectors && !pio->active &&
	            (pio->LaserPlotCount < 1) && (pio->TracerPlotCount < 1) &&
	            !pio->streamline.DrawStreamlines){
		    goto ErrorOut;
	    }
	   
	}

	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
		pio->pa     = sd->pa;		
	}
 
 	if(SetPIOData(Files,CurrentFrame))goto ErrorOut;

    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9203);
    if(!sd->data){
    	sprintf(WarningBuff,"doSage2DGetData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }

    sout=(double *)sd->data;
    zerol((char *)sout,length*sizeof(double));
    
    if(sd->type == FRAME_DATA_FLOAT){
		setFloat(sout,length);
	    if(getReflectedData(Files,sout,CurrentFrame)){
		    sprintf(WarningBuff,"doSage2DGetData - getReflectedData Error\n");
		    WarningBatch(WarningBuff);
			goto ErrorOut;
	    }	
    }else{
        zerol((char *)sout,length);
	    if(getPlotImage(Files,(unsigned char *)sout,CurrentFrame)){
			goto ErrorOut;
	    }	
	    if(sd->type == FRAME_DATA_RGB){
	         convertTO24(sd,&pio->pd);
	    }
    }
        
	sd->pd.dmin=pio->cmin;
	sd->pd.dmax=pio->cmax;	
    sd->limits = *limits;	
    
    ret=0;
    
ErrorOut:
	return ret;
}
static int doSage2DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;
	struct LIMITS *limits;
	struct PIO *sage;
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
	

	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage2DGetCellData - Only Returns LINEDATA_CELL_DATA\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	FilesSetFrame(CurrentFrame,Files);

	sage=pio->sage;

	name[0]=0;
	mstrncpy(name,sd->pioName,255);
	if(name[0] == 0){
		mstrncpy(name,"density",255);
	}

	fdata=NULL;
	if(SageGetFileItem(Files,name,sd->pioIndex,&fdata,&length,CurrentFrame)){
	    sprintf(WarningBuff,"doSage2DGetCellData - Error Trying To Reading name %s index %ld\n",name,(long)sd->pioIndex);
	    WarningBatch(WarningBuff);
	    ret=17;goto ErrorOut;
	}
	        			
    sd->xsize=length;
    sd->ysize=1;
    sd->zsize=1;
    sd->length=length;
    sd->data=fdata;
    sd->pioTime=s->pioTime;
    if(!sd->data){
    	sprintf(WarningBuff,"doSage2DGetCellData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }
    
    
	limits->xmin=sage->xzero;
	limits->ymin=sage->yzero;
	limits->xmax=sage->dxset*sage->ixmax+sage->xzero;
	limits->ymax=sage->yzero+sage->dyset*sage->iymax;
	limits->time=s->pioTime;
	limits->zmin=0;
	limits->zmax=0;
    

    ret=0;
    
ErrorOut:
	
	return ret;


}
static int doSage2DSetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct SetFrameData *s;
    int ret;

	if(!Files)return 1;
	if(!sd)return 1;
	s = &Files->SetData;

	ret=1;
	

	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage2DSetCellData - Only Handles LINEDATA_CELL_DATA\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

    if(!sd->pioName[0]){
    	sprintf(WarningBuff,"doSage2DSetCellData - Error No Data Name\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }
    
    
    if(!sd->data){
    	sprintf(WarningBuff,"doSage2DSetCellData - Error No Data To Write\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }


	if(SageSetFileItem(Files,sd->pioName,sd->pioIndex,(void *)sd->data,sd->xsize,CurrentFrame)){
	    sprintf(WarningBuff,"doSage2DSetCellData - Error Trying To Set name %s index %ld\n",sd->pioName,(long)sd->pioIndex);
	    WarningBatch(WarningBuff);
	    ret=17;goto ErrorOut;
	}
	        			
    
    ret=0;
    
ErrorOut:
	
	return ret;


}
int doDataScale(struct FileInfo2 *Files);
int doDataRange(struct FileInfo2 *Files);
int doDataSelection(struct FileInfo2 *Files);
int doVectorSelection(struct FileInfo2 *Files);
int doStreamlineSelection(struct PlotStreamData *streamline);

int doAreaSelection(struct FileInfo2 *Files);


static int doRange2Action(struct uDialogStruct *uList);

static int doRange2Action(struct uDialogStruct *uList)
{
 	uDialogReturn *uR;
 	double dx,dy;
 	long ix,iy;
 	long ixs,iys;
 	int ret;
	
	if(!uList)return 1;
	
    uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 12){
	
		uR=uDialogGetReturn(uList);
		
	    if(!uR)goto OutOfHere;

		dx=uR[10].dreturn-uR[8].dreturn;
		dy=uR[11].dreturn-uR[9].dreturn;
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
 	
	}
OutOfHere:
    if(uR)uDialogFreeReturn(uR);
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
int doAreaSelection(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {250,270,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {340,270,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"ixmaxScr", {80,29,110,20},uDialogText,uDialogGetLong},
       /*  3 */      {"iymaxSc", {280,29,110,20},uDialogText,uDialogGetLong},
       /*  4 */      {"xmin data", {280,78,110,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"ymin data", {280,103,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"xmax data", {280,128,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"ymax data", {280,153,110,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"ixmin data", {80,78,110,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"iymindata", {80,103,110,20},uDialogText,uDialogGetDouble},
       /* 10 */      {"ixmaxdata", {80,128,110,20},uDialogText,uDialogGetDouble},
       /* 11 */      {"iymaxdata", {80,153,110,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"Adjust", {220,220,110,20},uDialogButton,uDialogNone},
       /* 13 */      {"xmin", {220,78,50,20},uDialogString,uDialogNone},
       /* 14 */      {"ymin", {220,103,50,20},uDialogString,uDialogNone},
       /* 15 */      {"xmax", {220,128,50,20},uDialogString,uDialogNone},
       /* 16 */      {"ymax", {220,153,50,20},uDialogString,uDialogNone},
       /* 17 */      {"xmin", {20,78,50,20},uDialogString,uDialogNone},
       /* 18 */      {"ymin", {20,104,50,20},uDialogString,uDialogNone},
       /* 19 */      {"xmax", {20,132,50,20},uDialogString,uDialogNone},
       /* 20 */      {"ymax", {20,153,50,20},uDialogString,uDialogNone},
       /* 21 */      {"xsize", {20,29,50,20},uDialogString,uDialogNone},
       /* 22 */      {"ysize", {220,29,50,20},uDialogString,uDialogNone},
       /* 23 */      {"Plot Domain", {80,55,100,20},uDialogString,uDialogNone},
       /* 24 */      {"Data Domain", {280,56,100,20},uDialogString,uDialogNone},
       /* 25 */      {"Rotate Grid",{20,220,120,20},uDialogCheck,uDialogGetLong},
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
	
	uList=uDialogOpen("Area Selection",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(Files->fileClass == PIO1D_FILES){
	    if(pio->pd.Rotate1d){
	        Files->yminr=Files->xminr;
	        Files->ymaxr=Files->xmaxr;
	    }else{
	        Files->yminr=0;
	        Files->ymaxr=1.0;
	    }	
	}

	
	
	if(pio->range.xminData < Files->xminr)pio->range.xminData = Files->xminr;
	if(pio->range.yminData < Files->yminr)pio->range.yminData = Files->yminr;
	
	if(pio->range.xmaxData > Files->xmaxr)pio->range.xmaxData = Files->xmaxr;
	if(pio->range.ymaxData > Files->ymaxr)pio->range.ymaxData = Files->ymaxr;
	

	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)pio->range.ixmax,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)pio->range.iymax,
	              
	              uDialogSetItem,4L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->xminr,
	              
	              uDialogSetItem,5L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->yminr,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->xmaxr,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)Files->ymaxr,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)pio->range.xminData,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)pio->range.yminData,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)pio->range.xmaxData,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)pio->range.ymaxData,
	             	              	              
	              uDialogSetItem,25L,
	          	  uDialogSetHidden,(Files->fileClass != PIO1D_FILES), 
	              uDialogSetLogical,(int)pio->pd.Rotate1d,
	              
	              
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
	
	
	if(uR[2].lreturn <= 0 || uR[3].lreturn <= 0){
	    Warning("Sizes must be greater then zero");
	    goto Start;
	}
	
	if(uR[10].dreturn <= uR[8].dreturn){
	    Warning("xmax must be greater then xmin");
	    goto Start;
	}
	
	if(uR[11].dreturn <= uR[9].dreturn){
	    Warning("ymax must be greater then ymin");
	    goto Start;
	}
	
	pio->range.ixmax=(int)uR[2].lreturn;
	pio->range.iymax=(int)uR[3].lreturn;
			
	pio->range.xminData=uR[8].dreturn;
	pio->range.yminData=uR[9].dreturn;
	pio->range.xmaxData=uR[10].dreturn;
	pio->range.ymaxData=uR[11].dreturn;
	
	pio->pd.Rotate1d=(int)uR[25].lreturn;
	
	
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	return ret;
}
static int doSage2DDoDialog(struct FileInfo2 *Files,int *type)
{
	if(!Files || !type)return 1;
	
	if(*type == MessageType_DO_SCALES_DIALOG){
		return doDataScale(Files);
	}else if(*type == MessageType_DO_RANGE_DIALOG){
		return doDataRange(Files);	
	}else if(*type == MessageType_DO_SELECTION_DIALOG){
		return doDataSelection(Files);	
	}else if(*type == MessageType_DO_VECTOR_DIALOG){
		return doVectorSelection(Files);	
	}else if(*type == MessageType_DO_STREAMLINE_DIALOG){
		return doStreamlineSelection(&Files->pioData.streamline2d);
	}else if(*type == MessageType_DO_AREA_DIALOG){
		return doAreaSelection(Files);	
	}
	return 0;
}
static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy)
{

	if(!pio || !x || !y || !ix || !iy)return 1;

	*ix=(int)((*x-pio->range.xminData)*pio->range.xScale+(double)pio->range.ixminData+.5);
	*iy=(int)((*y-pio->range.yminData)*pio->range.yScale+(double)pio->range.iyminData+.5);
	
	/* needed because clipping happens exactly to the boundary */
	
	if(*iy >= pio->range.iymaxData){
		*iy=pio->range.iymaxData-1;
	}

	if(*ix >= pio->range.ixmaxData){
		*ix=pio->range.ixmaxData-1;
	}

	return 0;
}

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *bp)
{
	long length;
	double *d;
	
	if(!sd || !bp)return 1;
	d=(double *)sd->data;
	if(!d)return 1;
	
	if(sd->zsize > 1){
	    length = sd->xsize*sd->ysize*sd->zsize;
	}else{
	    length = sd->xsize*sd->ysize;
	}
	
	if(length < 1)return 1;
			
	FloatToImage(d,length,&sd->pd,bp);    
	
	return 0;
}
int FloatToImage(double *d,long length,struct paletteDraw *pd,unsigned char *bp)
{
	double dmin;
	double dmax;
	double mmax;
	double dx;
	double a,b;
	double r;
	long n;
	int dolog;
	int v;
	
	if(!d || !pd)return 1;
	
	
	dmin=pd->sPmin;
	dmax=pd->sPmax;
	

	dolog = 0;
	    
	if(pd->sType == 0){
	    		    
   		dmin=pd->dmin;
    	dmax=pd->dmax;
    	
    	dx=dmax-dmin;
    	
    	mmax=max(fabs(dmin),fabs(dmax));
	    	    
	    if(dx <= 0.0 || dx < 1e-5*mmax){
	        dmax=dmin+1.;
	        --dmin;
	    }
	}else if(pd->sType == 1){
	    dolog = 1;
	}
		
	if(!bp){
	    bp=(unsigned char *)d;
	}
	
	if(dolog){
	    if(dmin <= 0.0)return 1;
	    
	    a=(254.0-2.0)/(log10(dmax)-log10(dmin));
	    b=2.0-a*log10(dmin);
	
		for(n=0;n<length;++n){
		    r=d[n];
		    if(r == FLOAT_NOT_SET){
		        *bp++ = 0;
		        continue;
		    }else if(r < dmin){
		       r = dmin;
		    }else if(r > dmax){
		       r = dmax;
		    }
		    
			r=log10(r);
		    *bp++ = (unsigned char)(a*r+b);
		}
	
	}else{
	    a=(254.0-2.0)/(dmax-dmin);
	    b=2.0-a*dmin;
		for(n=0;n<length;++n){
		    r=d[n];
		    if(r == FLOAT_NOT_SET){
		        *bp++ = 0;
		        continue;
		    }
		    		    
		    v=(int)(a*r+b);
		    		    
		    if(v < 2){
		        v=2;
		    }else if(v > 254){
		        v=254;
		    }
		      
		    *bp++ = (unsigned char)(v);
		}
	}
	
	pd->dmin=dmin;
	pd->dmax=dmax;
	
	return 0;
}
int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp)
{
	double dmin;
	double dmax;
	double mmax;
	double dx;
	double a,b;
	double r;
	int dolog;
	int v;
	
	if(!d || !pd || !bp)return 1;
	
	
	dmin=pd->sPmin;
	dmax=pd->sPmax;
	

	dolog = 0;
	    
	if(pd->sType == 0){
	    		    
   		dmin=pd->dmin;
    	dmax=pd->dmax;
    	
    	dx=dmax-dmin;
    	
    	mmax=max(fabs(dmin),fabs(dmax));
	    	    
	    if(dx <= 0.0 || dx < 1e-5*mmax){
	        dmax=dmin+1.;
	        --dmin;
	    }
	}else if(pd->sType == 1){
	    dolog = 1;
	}
			
	if(dolog){
	    if(dmin <= 0.0)return 1;
	    
		a=(254.0-2.0)/(log10(dmax)-log10(dmin));
		b=2.0-a*log10(dmin);

		r = *d;
		if(r == FLOAT_NOT_SET){
		    *bp = 0;
		}else if(r < dmin){
		   r = dmin;
		}else if(r > dmax){
		   r = dmax;
		}

		r=log10(r);
		*bp = (unsigned char)(a*r+b);
	
	}else{
	    a=(254.0-2.0)/(dmax-dmin);
	    b=2.0-a*dmin;
		    r = *d;
		    if(r == FLOAT_NOT_SET){
		        *bp = 0;
		    }
		    		    
		    v=(int)(a*r+b);
		    		    
		    if(v < 2){
		        v=2;
		    }else if(v > 254){
		        v=254;
		    }
		      
		    *bp = (unsigned char)(v);
	}
	
	
	return 0;
}

int setFloat(double *d,long length)
{
	if(!d)return 1;
	if(length <= 0)return 1;
	while(length-- > 0)*d++ = FLOAT_NOT_SET;
	return 0;
}
int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r)
{
      double sxn,syn,sxx,syx,dnom,t,xt,yt,sn,sx;
      double dxlim,dylim,xmind,ymind,xmaxd,ymaxd;
      double s[3];
      int i1,i2,iout,iflip;
      
      dxlim=r->dxlim;
      dylim=r->dylim;
      
      xmind=r->xmin-dxlim;
      xmaxd=r->xmax+dxlim;

      ymind=r->ymin-dylim;
      ymaxd=r->ymax+dylim;

      i1=0;
      i2=0;
      iout=0;
      if(*x1 < xmind || *x1 > xmaxd)i1=1;
      if(*y1 < ymind || *y1 > ymaxd)i1=1;
      if(*x2 < xmind || *x2 > xmaxd)i2=1;
      if(*y2 < ymind || *y2 > ymaxd)i2=1;
      if(i1 == 0 && i2 == 0)return iout;

      sxn=-1.;
      syn=-1.;
      sxx=-1.;
      syx=-1.;
      iout=1;
      iflip=0;

      dnom=*x2-*x1;
      if(fabs(dnom) > r->dxlim){
          sxn=(r->xmin-*x1)/dnom;
          sxx=(r->xmax-*x1)/dnom;
      }

      dnom=*y2-*y1;
      if(fabs(dnom) > r->dylim){
          syn=(r->ymin-*y1)/dnom;
          syx=(r->ymax-*y1)/dnom;
      }

      if(sxn >= 0. && sxn <= 1.){
          t=sxn* *y2+(1.-sxn)* *y1;
          if(t >= ymind && t <= ymaxd){
              iflip=iflip+1;
              s[iflip]=sxn;
          }
      }

      if(sxx >= 0. && sxx <= 1.){
          t=sxx* *y2+(1.-sxx)* *y1;
          if(t >= ymind && t <= ymaxd){
              iflip=iflip+1;
              s[iflip]=sxx;
              if(iflip > 1) goto gotThem;
          }
      }

      if(syn >= 0. && syn <= 1.){
          t=syn* *x2+(1.-syn)* *x1;
          if(t >= xmind && t <= xmaxd){
              iflip=iflip+1;
              s[iflip]=syn;
              if(iflip > 1) goto gotThem;
          }
      }

      if(syx >= 0. && syx <= 1.){
          t=syx* *x2+(1.-syx)* *x1;
          if(t >= xmind && t <= xmaxd){
              iflip=iflip+1;
              s[iflip]=syx;
          }
      }
gotThem:
      if(iflip == 0) return iout;
      iout=0;
      if(iflip == 1){
          if(i1 == 1){
              *x1=s[1]* *x2+(1.-s[1])* *x1;
              *y1=s[1]* *y2+(1.-s[1])* *y1;
          }else{
              *x2=s[1]* *x2+(1.-s[1])* *x1;
              *y2=s[1]* *y2+(1.-s[1])* *y1;
          }
          return iout;
      }

      if(s[1] < s[2]){
          sn=s[1];
          sx=s[2];
      }else{
          sn=s[2];
          sx=s[1];
      }

      xt=*x1;
      yt=*y1;
      *x1=sn* *x2+(1.-sn)*xt;
      *y1=sn* *y2+(1.-sn)*yt;
      *x2=sx* *x2+(1.-sx)*xt;
      *y2=sx* *y2+(1.-sx)*yt;
      return iout;
}

