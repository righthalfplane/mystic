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
#include "doSage3DGet.h"

struct DrawData{
	struct FilePIOInfo *pio;
	struct screenData *ss;
	unsigned char *sout;
	double *soutd;
	struct dRange *r;
	double *lambda;
	double *t;
	int *processed;
	struct linedata *li;
	struct linedata *lii;

};

/* static int RangeCellVelocity(long n,struct FilePIOInfo *pio,double *vmax); */

int doSage2DGetDataFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int checkStreamData(struct dataStruct *plane);

static int doSage2DCommandGet(struct FileInfo2 *Files,CommandPtr cp);

int freeStreamLine2d(struct PlotStreamData *streamline2d);

static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp);

int doSage2DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai);

int doSage2DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li);

int setFloat(double *d,long length);

//static int doCell(long ne,struct DrawInfo *info);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

static int pioDrawTracers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage2DSaveTracerLineData(struct FileInfo2 *Files,int *list,int nc);

static int doSage2DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int pioDrawStreamlines(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int pioDrawLasers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage2DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

static int doSage2DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage2DDoDialog(struct FileInfo2 *Files,int *type);

int doSage2DBlK(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);

int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double *v,long *index);

static int doSage2DGetLineData(struct FileInfo2 *Files,struct linedata *li);

int doSage2DStartBlk(struct FileInfo2 *Files);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int SetPIOData(struct FileInfo2 *Files,long CurrentFrame);

static int SageGetGradients2D(struct FileInfo2 *Files,int flag);

static int doAuto(struct FileInfo2 *Files);

static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int setPioScales(struct FilePIOInfo *pio);

static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int doSage2DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int doSage2DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int doSage2DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSage2DSetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int getPIOHalfCellSize2D(struct FilePIOInfo *pio,int level,  double size[2]);

static int pioDrawZonesT(mThread *Threads);

static int pioDrawVoidT(mThread *Threads);

static int getPlotDataT(mThread *Threads);


int doSage2DBatchBlk(BatchPtr Batch,char *name);

int doSage2DBatchBlk(BatchPtr Batch,char *name)
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
            sprintf(WarningBuff,"doSage2DBatchBlk Found (%s) instead of PIO2D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSage2DBatchBlk Can Only Process PIO2D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
    
    if(doSage2DStartBlk(Files))goto ErrorOut;
		
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSage2DBlK(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}


int doSage2DBlK(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
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
static int getPIOHalfCellSize2D(struct FilePIOInfo *pio,int level,  double size[2])
{
	if (!size || !pio) return 0;
	size[0] = pio->dx2[level];
	size[1] = pio->dy2[level];
	return 1;
}
static int pioDrawLasers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
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
static int pioDrawTracers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
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
	    
/*
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
*/
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
int doSage2DStartBlk(struct FileInfo2 *Files)
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


static int doSage2DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	
	return doSage2DGetLineDataFnt(Files,li);	
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

/*
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
*/
static int doSage2DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	int ret;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;

	ret=1;
	
	CurrentFrame=ai->CurrentFrame;
	
	FilesSetFrame(CurrentFrame,Files);
		
	return doSage2DGetAreaDataFnt(Files,ai);
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
/*
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

*/
static int doSage2DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	
	FilesSetFrame(CurrentFrame,Files);
	
	return doSage2DGetDataFnt(Files,CurrentFrame,sd);	

}
static int doSage2DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
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
	

	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage2DGetCellData - Only Returns LINEDATA_CELL_DATA\n");
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

static int doAreaSelection(struct FileInfo2 *Files);

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
static int doAreaSelection(struct FileInfo2 *Files)
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


