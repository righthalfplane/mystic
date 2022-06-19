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
#include "uGraph.h"
#include "mThread.h"

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2


#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))

struct dRange{
      double xmin,xmax,ymin,ymax,dxlim,dylim;
};

static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);

int doSage1DStart(struct FileInfo2 *Files);

int doSage1D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int doAuto(struct FileInfo2 *Files);

static int SetPIOData(struct FileInfo2 *Files,long CurrentFrame);

static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int setPioScales(struct FilePIOInfo *pio);

static int doSage1DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int doSage1DGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int pioGetValue(struct FilePIOInfo *pio,double x,double *v,long *n);

static int pioGetValue2(struct FilePIOInfo *pio,double x,double y,double *v,long *n);

static int intcmp99(const void *xx, const void *yy);

static double pioFindValue(double x,double *xy,long nd);

static int doSage1DDoDialog(struct FileInfo2 *Files,int *type);

static int doSage1DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSage1DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int setFloat(double *d,long length);

static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int SageGetGradients1D(struct FileInfo2 *Files,int flag);

static int doSage1DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int GatherBhangMeterMerge(mThread *Threads, int count);

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
	
	double dr;
	long nny,nnx;
	int bhangMeterSteps;
	struct LineInfo *info;
	double nx,ny;
	
};


int doSage1DBatch(BatchPtr Batch,char *name)
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
    
    if(Files->fileClass != PIO1D_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"doSage1DBatch Found (%s) instead of PIO1D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSage1DBatch Can Only Process PIO1D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
    
    if(doSage1DStart(Files))goto ErrorOut;
		
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSage1D(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}
int doSage1D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
	                        
    struct FilePIOInfo *pio;
    double xmin,xmax,ymin,ymax;
    /* long CurrentFrame; */
	char *command;
 	double value;
 	int rangeNew;
    int flag;
 	int ret;
    
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
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
/*
	}else if(!mstrcmp("plot",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    CurrentFrame=(long)value;
	    if(SetPIOData(Files,CurrentFrame))goto ErrorOut;
	    if(PlotPIOData(Files,CurrentFrame,1))goto ErrorOut;	 
*/   
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
    struct FilePIOInfo *pio;
	struct screenData ss;
	
	if(!Files || !buff)return 1;
	
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

	pioDraw(Files,buff,CurrentFrame,&ss);

	pioSetLimits(Files,CurrentFrame);
		
	return 0;
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
	
	
	return 0;
}
static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout)
{
    struct FilePIOInfo *pio;
	
    double xmin,xmax,ymin,ymax;
    double didx,djdy;
    double dxdi,dydj;
    long ixmin,ixmax,iymin,iymax;
    long n;		
		
	if(!Files || !sout)return 1;
	pio = &Files->pioData;
	
    setPioScales(pio);

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

    for(n=0;n<pio->numcell;++n){
        double xc,yc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double xs,ys;
        long i,j,ihi,ilo,jhi,jlo;
        int k;

        if(pio->daughter[n] || pio->active[n])continue;
        k=(int)(pio->level[n]);
        
        yc=0.5*pio->dxset;
        dy2=yc*1.000001;

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
    	    
    return 0;	        
}
static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r)
{
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2;
	double dn,rr,theta;
	unsigned char nc;
	int nstep,nn;
	int ix,iy;
	long n;
	int k;
	
	if(!pio || !ss || !r || !pio->value)return 1;
	
	if(pio->pd.Rotate1d)goto DoRotate;	
	
	for(n=0;n<pio->numcell;++n){
	    k=(int)(pio->level[n]);
	    if( k > ss->ZoneLevel)continue;
	    if((k < ss->ZoneLevel) && pio->daughter[n])continue;
	    nc=255;
	    if(pio->pa.DrawZonesColor){
	        FloatToColor(&pio->value[n],&pio->pd,&nc);
	    }
	    xc=pio->xcenter[n];
	    dx2=pio->dx2[k];
	    
        yc=0.5*pio->dxset;
        dy2=yc*1.000001;
	        
	    x1=xc-dx2;
	    x2=xc-dx2;
	    y1=yc-dy2;
	    y2=yc+dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix,&iy);
	        MoveB(ix,iy);
	        RealLoc(pio,&x2,&y2,&ix,&iy);
	        LineB(ix,iy,nc,ss);
	    }

	    x1=xc+dx2;
	    x2=xc+dx2;
	    y1=yc-dy2;
	    y2=yc+dy2;
	    if(!isOut(&x1,&y1,&x2,&y2,r)){
	        RealLoc(pio,&x1,&y1,&ix,&iy);
	        MoveB(ix,iy);
	        RealLoc(pio,&x2,&y2,&ix,&iy);
	        LineB(ix,iy,nc,ss);
	    }

	}
	return 0;
	
DoRotate:	

	nstep=(int)(0.1*(ss->viewRect.xsize+ss->viewRect.ysize));
	
	dn=2.0*atan(1.0)/(double)(nstep-1);

	for(n=0;n<pio->numcell;++n){
	    
	    k=(int)(pio->level[n]);
	    if( k > ss->ZoneLevel)continue;
	    if((k < ss->ZoneLevel) && pio->daughter[n])continue;	    
	    nc=255;
	    if(pio->pa.DrawZonesColor){
	        FloatToColor(&pio->value[n],&pio->pd,&nc);
	    }	    	    
	    xc=pio->xcenter[n];
	    dx2=pio->dx2[k];
	    
	    rr=xc+dx2;
	    
		for(nn=0;nn<nstep;++nn){
			theta=(double)nn*dn;
	    	x1=rr*cos(theta);
	   	 	y1=rr*sin(theta);
	    	x2=rr*cos(theta+dn);
	   	 	y2=rr*sin(theta+dn);
		    if(!isOut(&x1,&y1,&x2,&y2,r)){
		        RealLoc(pio,&x1,&y1,&ix,&iy);
		        MoveB(ix,iy);
		        RealLoc(pio,&x2,&y2,&ix,&iy);
		        LineB(ix,iy,nc,ss);
		    }
    	}
    	
	}

	return 0;

}
static int pioDrawVectorsRotate(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	double *xy,rmin,rmax,rr,dr;
	double vmax,vmaxLen,vlength,vx,vy,dx,dy;
	double xc,dx2;
	double x1,y1,x2,y2,x3,y3;
	double pi,turn,o1,oa,arrow;
	double vmax2,degen,vspace;
	int ix,iy;
	long length;
	long n,nd;
	int ret;
	int k;
    double x,y;
    long i,j;	        			
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	
	xy=NULL;
	
	ret = 1;
	
	if(mstrcmp(pio->vector.vectorxName,pio->vectorxNameOld)){
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	}	
	
	if(!pio->vectorx  || pio->CurrentFrameVectors != pio->CurrentFrame){
	
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	    	    
	    pio->CurrentFrameVectors = -1;  
	      
		if(SageGetFileItem(Files,pio->vector.vectorxName,0,
		                  &pio->vectorx,&length,pio->CurrentFrame)){
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
	        vx=pio->vectorx[n];
	        vlength = sqrt(vx*vx);
	        if(vlength > vmax)vmax=vlength;
        }

	    mstrncpy(pio->vectorxNameOld,pio->vector.vectorxName,32);
	    	    	    
	    pio->vmaxVectors=vmax;
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
	
	if(vmaxLen <= 0)goto ErrorOut;
	
	vmaxLen /= vmax;
	
	pi=4.*atan(1.);
	turn=pio->vector.arrowAngle*pi/180.;
	
	arrow=pio->vector.arrowLength*vmax*vmaxLen;
	
   	vmax2=vmax/3.;
		
	if(pio->vector.vspace == 0){
	    double dn,rr,theta;
	    int nstep,nn;
	
		nstep=20;
	
		dn=2.0*atan(1.0)/(double)(nstep-1);
	
	    for(n=0;n<pio->numcell;++n){
		    k=(int)(pio->level[n]);
		    if( k > ss->ZoneLevel)continue;
		    if((k < ss->ZoneLevel) && pio->daughter[n])continue;
	        vx=pio->vectorx[n];
	        vlength = sqrt(vx*vx);
	        if(vlength > vmax){
	            vx = vx*vmax/vlength;
	        }
	        if(vlength <= 0)continue;
	        
	        for(nn=0;nn<nstep;++nn){
	            rr=pio->xcenter[n];

				theta=(double)nn*dn;
		    	x1=rr*cos(theta);
		   	 	y1=rr*sin(theta);
		    	x3=x2=(rr+vx*vmaxLen)*cos(theta);
		   	 	y3=y2=(rr+vx*vmaxLen)*sin(theta);
	            
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


		if(!(xy=(double *)cMalloc((long)sizeof(double)*(2*pio->numcell),9186))){
		    WarningBatch("pioDrawVectorsRotate Out of Memory\n");
		    goto ErrorOut;
		}

		rmin  =1e60;
		rmax= -1e60;

		nd=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
		    k=(int)(pio->level[n]);
		    dr=pio->dx2[k];
			rr=pio->xcenter[n];
		    xy[2*nd]=rr-dr;
		    if(rr+dr > rmax)rmax=rr+dr;
		    if(rr-dr < rmin)rmin=rr-dr;
		    xy[2*nd+1]=(double)n;
		    nd++;
		}
		

		if(!nd)goto ErrorOut;
					
		qsort((char *)xy,(size_t)nd, (size_t)(2*sizeof(double)),intcmp99);   
		
	    for(i=ixmin;i<ixmax;++i){
	        x=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
	        if(x < 0.0)continue;
		    if(x < xmin || x > xmax)continue;
			for(j=iymin;j<iymax;++j){
			    y=ymin+(double)((double)(j-iymin)+.5)*dydj;
	        	if(y < 0.0)continue;
			    if(y < ymin || y > ymax)continue;
			    rr=sqrt(x*x+y*y);
			    if(rr < rmin || rr > rmax)continue;
			    n=(long)pioFindValue(rr,xy,nd);
        		xc=pio->xcenter[n];
        		

		        vx=pio->vectorx[n]*x/rr;
		        vy=pio->vectorx[n]*y/rr;
		        vlength = sqrt(vx*vx+vy*vy);
		        if(vlength > vmax){
		            vx = vx*vmax/vlength;
		            vy = vy*vmax/vlength;
		        }
		        if(vlength <= 0)continue;		
		        
				x1=x;
			    x3=x2=x1+vx*vmaxLen;
			    y1=y;
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




			
	ret = 1;
ErrorOut:
	if(xy)cFree((char *)xy);
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
	
	if(pio->pd.Rotate1d){
	    return pioDrawVectorsRotate(Files,ss,r);
	}

	
	ret = 1;
	
	if(mstrcmp(pio->vector.vectorxName,pio->vectorxNameOld)){
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	}

	if(!pio->vectorx || pio->CurrentFrameVectors != pio->CurrentFrame){
	
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	    
	    pio->CurrentFrameVectors = -1;  
	      
		if(SageGetFileItem(Files,pio->vector.vectorxName,0,
		                  &pio->vectorx,&length,pio->CurrentFrame)){
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
        	yc=0.5*pio->dxset;
        	dy2=yc*1.000001;
		    if(yc+dy2 < pio->range.yminData)continue;
		    if(yc-dy2 > pio->range.ymaxData)continue;
	        vx=pio->vectorx[n];
	        vy=0;
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > vmax)vmax=vlength;
        }

	    mstrncpy(pio->vectorxNameOld,pio->vector.vectorxName,32);
	    	    	    
	    pio->vmaxVectors=vmax;
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
	        vy=0;
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > vmax){
	            vx = vx*vmax/vlength;
	            vy = vy*vmax/vlength;
	        }
	        if(vlength <= 0)continue;
	        
		    x1=pio->xcenter[n];
		    x3=x2=x1+vx*vmaxLen;
		    y1=0.5*pio->dxset;
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
	        
        	yc=0.5*pio->dxset;
        	dy2=yc*1.000001;

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
	        vy=0;
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
static int getPlotRotated(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	double *xy;
	
    double xmin,xmax,ymin,ymax,dmin,dmax;
    double didx,djdy;
    double dxdi,dydj;
    double rmin,rmax,r,dr;
    long ixmin,ixmax,iymin,iymax;
    long n,nd,k;		
		
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	if(!pio->pd.Rotate1d)return 1;	
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx){
		pio->doGradients = TRUE;
	}


    setPioScales(pio);

    xmin=pio->range.xminData;
    xmax=pio->range.xmaxData;
    ymin=pio->range.yminData;
    ymax=pio->range.ymaxData;
    ixmin=pio->range.ixminData;
    ixmax=pio->range.ixmaxData;
    iymin=pio->range.iyminData;
    iymax=pio->range.iymaxData;
	    
	    
	if(!(xy=(double *)cMalloc((long)sizeof(double)*(2*pio->numcell),5186))){
	    WarningBatch("getPlotRotated Out of Memory\n");
	    goto OutOfHere;
	}

	rmin  =1e60;
	rmax= -1e60;

	nd=0;
	for(n=0;n<pio->numcell;++n){
		if(pio->daughter[n])continue;
	    k=(long)(pio->level[n]);
	    dr=pio->dx2[k];
		r=pio->xcenter[n];
	    xy[2*nd]=r-dr;
	    if(r+dr > rmax)rmax=r+dr;
	    if(r-dr < rmin)rmin=r-dr;
	    xy[2*nd+1]=(double)n;
	    nd++;
	}
	

	if(!nd)goto OutOfHere;

		
	qsort((char *)xy,(size_t)nd, (size_t)(2*sizeof(double)),intcmp99);
	    
	   	
    didx=((double)(ixmax-ixmin))/(xmax-xmin);
    dxdi=1.0/didx;
    djdy=((double)(iymax-iymin))/(ymax-ymin);
    dydj=1.0/djdy;

	dmin  =1e60;
	dmax= -1e60;
	

    {
        double svalue,x,y;
        double xc,value;
        long i,j;	        			
		
	    for(i=ixmin;i<ixmax;++i){
	        x=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
	        if(x < 0.0)continue;
		    if(x < xmin || x > xmax)continue;
			for(j=iymin;j<iymax;++j){
			    y=ymin+(double)((double)(j-iymin)+.5)*dydj;
	        	if(y < 0.0)continue;
			    if(y < ymin || y > ymax)continue;
			    r=sqrt(x*x+y*y);
			    if(r < rmin || r > rmax)continue;
			    n=(long)pioFindValue(r,xy,nd);
        		xc=pio->xcenter[n];
			    value=pio->value[n];
		        if(pio->doGradients){
		           svalue=value+(r-xc)*pio->gradx[n];
		        }else{
		           svalue=value;
		        }
		        
		        sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=svalue;
		        if(dmin > svalue)dmin=svalue;
		        if(dmax < svalue)dmax=svalue;
		        
		        
			} 		        
	    }
	}
    
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	pioSetLimits(Files,CurrentFrame);
		
OutOfHere:
    if(xy)cFree((char *)xy);
	    
	    return 0;
	        
}
static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	
    double xmin,xmax,ymin,ymax,dmin,dmax;
    double didx,djdy;
    double dxdi,dydj;
    long ixmin,ixmax,iymin,iymax;
    long n;		
		
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	if(pio->pd.Rotate1d){
	    return getPlotRotated(Files,sout,CurrentFrame);
	}
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx){
		pio->doGradients = TRUE;
	}


    setPioScales(pio);

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

    for(n=0;n<pio->numcell;++n){
        double xc,yc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double svalue,value,xs,ys;
        long i,j,ihi,ilo,jhi,jlo;
        int k;

        if(pio->daughter[n])continue;
        k=(int)(pio->level[n]);
        
        yc=0.5*pio->dxset;
        dy2=yc*1.000001;

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
		           svalue=value+(xs-xc)*pio->gradx[n];
		        }else{
		           svalue=value;
		        }
		        sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=svalue;
		        if(dmin > svalue)dmin=svalue;
		        if(dmax < svalue)dmax=svalue;
		    }
		}

    }
    
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	pioSetLimits(Files,CurrentFrame);
	
    
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
static int SetPIOData(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct PIO *sage;
	int active;
	int ret;
	long n;
	int k;
	double *fdata1,*fdata2,*fdata3,*fdata4,*fdata5,*fdata6;
	long dataLength1,dataLength3,dataLength4,dataLength5;
	double lmin,lmax,size,cmin,cmax;


	if(!Files)return 1;
	pio = &Files->pioData;
	if(Files->fileClass != PIO1D_FILES)return 1;
	if(!Files->PIO2DList)return 1;

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

	sage = &Files->PIO2DList[CurrentFrame];

	Files->limits.time=sage->time;
	Files->pioTime=sage->time;
	
	if((pio->CurrentFrame == CurrentFrame) && pio->xcenter && pio->level && pio->daughter){
	    
	    if(pio->value){

		    if(!strcmp(pio->pioName,pio->pioNameOld) && (pio->pioIndex == pio->pioIndexOld)){
		        return 0;
		    }
	        cFree((char *)pio->value);
	        pio->value=NULL;
	    }


        if(!pio->pioName[0]){
            mstrncpy(pio->pioName,"density",255);
            pio->pioIndex=0;
        }
	        
        if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
                  &fdata4,&dataLength4,CurrentFrame)){
	       ret=22;goto ErrorOut;
        }

        for(n=0;n<pio->numcell;++n){
            if(pio->daughter[n])continue;
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
 	    
		if(pio->pa.flagGradients)SageGetGradients1D(Files,pio->pa.flagGradients-1);

		pio->doGradients = FALSE;
		if(pio->pa.flagGradients && pio->gradx){
			pio->doGradients = TRUE;
		}
 	    
 	    return 0;

	}

	pio->CurrentFrame=CurrentFrame;


	if(pio->xcenter)cFree((char *)pio->xcenter);
	pio->xcenter=NULL;
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


	if(SageGetFileItem(Files,"cell_center",1,
	                  &fdata1,&dataLength1,CurrentFrame)){
		    ret=23;goto ErrorOut;
	}


	if(SageGetFileItem(Files,"cell_level",0,
	                  &fdata3,&dataLength3,CurrentFrame)){
		    ret=24;goto ErrorOut;
	}

	if(SageGetFileItem(Files,"cell_active",0,
	                  &fdata6,&dataLength3,CurrentFrame)){
		    ret=25;goto ErrorOut;
	}

	if(SageGetFileItem(Files,"cell_daughter",0,
	                  &fdata5,&dataLength5,CurrentFrame)){
		    ret=26;goto ErrorOut;
	}


    if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
                  &fdata4,&dataLength4,CurrentFrame)){
	    ret=27;goto ErrorOut;
    }

    for(n=0;n<pio->numcell;++n){
        if(fdata5[n])continue;
        size=fdata4[n];
        if(size < cmin)cmin=size;
        if(size > cmax)cmax=size;
    }
    
    mstrncpy(pio->pioNameOld,pio->pioName,255);
    pio->pioIndexOld=pio->pioIndex;
    mstrncpy(Files->pioName,pio->pioName,255);
    Files->pioIndex=pio->pioIndex;

	active=FALSE;
	
	for(n=0;n<pio->numcell;++n){
	    size=fdata3[n];
	    if(size < lmin){
	        lmin=size;
	    }
	    if(size > lmax){
	        lmax=size;
	    }	    
	    if(fdata6[n] <= 0.0)active=TRUE;
	}
	

	if(lmin < 1 || lmax >= 100){
	    sprintf(WarningBuff,"SetPIOData Error level min %f lenvel max %f\n",lmin,lmax);
	    WarningBatch(WarningBuff);
	     ret=666;
	    goto ErrorOut;
	}

	for(k=1;k<=(int)lmax;++k){
	    pio->dx2[k]=.5*pio->dxset/pow(2,(double)(k-1));
	}

	pio->xmin=sage->xmin;
	pio->xmax=sage->xmax;

	pio->lmin=lmin;
	pio->lmax=lmax;

	pio->cmin=cmin;
	pio->cmax=cmax;

	pio->xcenter=fdata1;
	pio->level=fdata3;
	pio->value=fdata4;
	pio->daughter=fdata5;

	if(pio->pa.flagGradients)SageGetGradients1D(Files,pio->pa.flagGradients-1);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx){
		pio->doGradients = TRUE;
	}

	if(active){
		pio->active=fdata6;
	}else{
	    pio->active=NULL;
		if(fdata6)cFree((char *)fdata6);
	}

	return 0;
ErrorOut:
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
static int SageGetGradients1D(struct FileInfo2 *Files,int flag)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	double *gradl,*next,*gradh,g;
	long dataLength;
	long n,n2;
	int k,k2,ret;
	long ierror;

	if(flag < 0)return 0;
	if(!Files)return 1;
	pio = &Files->pioData;
	if(!pio)return 1;
	if(!pio->value)return 1;
	if(Files->fileClass != PIO1D_FILES)return 1;
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

	pio->gradx=(double *)cMalloc(sizeof(double)*pio->numcell,19727);
	if(!pio->gradx)goto ErrorOut;

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
	        if(k2 <= k){
	            if(pio->daughter[n2]){
	                n2=(long)(pio->daughter[n2]-1);
					k2=(int)(pio->level[n2]);
					g =  (pio->value[n]-pio->value[n2+1])/(pio->dx2[k]+pio->dx2[k2]);
					g += (pio->value[n]-pio->value[n2+3])/(pio->dx2[k]+pio->dx2[k2]);
					gradl[n] = 0.5*g;
	            }else{
	                gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dx2[k]+pio->dx2[k2]);
	            }
	        }else{
	            ++ierror;
	        }
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
	        if(k2 <= k){
	            if(pio->daughter[n2]){
	                n2=(long)(pio->daughter[n2]-1);
					k2=(int)(pio->level[n2]);
					g =  (pio->value[n2+0]-pio->value[n])/(pio->dx2[k]+pio->dx2[k2]);
					g += (pio->value[n2+2]-pio->value[n])/(pio->dx2[k]+pio->dx2[k2]);
					gradh[n] = 0.5*g;
	            }else{
	                gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dx2[k]+pio->dx2[k2]);
	            }
	        }else{
	            ++ierror;
	        }
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

	if(flag > 0){
		for(n=0;n<pio->numcell;++n){
		    if(flag == 1){
		       pio->value[n]=pio->gradx[n];
		    }
		}
	}
	if(gradh)cFree((char *)gradh);
	if(gradl)cFree((char *)gradl);

	return 0;
ErrorOut:
	sprintf(WarningBuff,"SageGetGradients1D Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	if(gradl)cFree((char *)gradl);
	if(gradh)cFree((char *)gradh);
	return 1;
}
int doSage1DStart(struct FileInfo2 *Files)
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
	pio->range.MaintainAspectRatio=0;
	pio->range.receiveRange=1;
	

	pio->CurrentFrameGradients =  -1;

	pio->CurrentFrameVectors = -1;
	pio->vector.DrawVectors=0;
	pio->vector.arrowLength=0.1;
	pio->vector.arrowAngle=145.0;
	pio->vector.arrowHeads=1;
	pio->vector.vspace= -10;
	pio->vector.vmax=0;
	pio->vector.vfac=0.2;
	
	
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
	
	Files->pGetAreaData=doSage1DGetAreaData;
		
	Files->pGetData2=doSage1DGetData;
		
	Files->pDoDialog=doSage1DDoDialog;
	
	Files->pGetLineData=doSage1DGetLineData;
	
	Files->pInformation=doSage1DInformation;
	
	Files->pGetCellData=doSage1DGetCellData;
	
		
    mstrncpy(pio->pioName,"density",255);
    pio->pioIndex=0;
    mstrncpy(pio->vector.vectorxName,"xdt",32);
        
	SetRange(Files,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY);
	
	getPaletteByName("ps",pio->pd.palette);

		
	return 0;
}
static int doSage1DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	double *data;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;

	if(ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"doSage1DGetAreaData - Only Returns AREADATA_AREA_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	CurrentFrame=ai->CurrentFrame;
	
	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}

	pio->pd.AreaFill=TRUE;
	
	if(SetPIOData(Files,CurrentFrame))return 1;
		
	ai->pioTime=Files->pioTime;
	
	if(ai->xsize <= 0 || ai->ysize <= 0)return 1;

	if(ai->zsize > 1){
	    sprintf(WarningBuff,"doSage1DGetAreaData - Error cannot return Volume Data Sets\n");
	    WarningBatch(WarningBuff);
	    return 1;	
	}
	
	data=(double *)cMalloc(ai->xsize*ai->ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,ai->xsize*ai->ysize*sizeof(double));
	
	ai->data=data;	
	
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
	    sprintf(WarningBuff,"doSage1DGetAreaData - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        return 1;
    }	    
	

	    
	return 0;
}
static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	int AreaSave;
	long length;
    int ret;

	if(!Files)return 1;
	pio=&Files->pioData;
	if(!sd)return 1;
	limits=&Files->limits;

	ret=1;
	
	AreaSave=pio->pd.AreaFill;
	
	if(sd->type != FRAME_DATA_FLOAT_RASTER){
	    sprintf(WarningBuff,"FloatAndRaster - Only Returns FRAME_DATA_FLOAT_RASTER\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
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

	AreaSave=pio->pd.AreaFill;
	
	pio->pd.AreaFill=TRUE;
	
	if(SetPIOData(Files,CurrentFrame))goto ErrorOut;
	
	pio->pd.AreaFill=AreaSave;
        			
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9203);
    if(!sd->data){
    	sprintf(WarningBuff,"doSage1DGetData - Out of Memory\n");
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
       
	setFloat((double *)sd->data,length);
    if(getPlotData(Files,(double *)sd->data,CurrentFrame)){
	    sprintf(WarningBuff,"doSage1DGetData - getPlotData Error\n");
	    WarningBatch(WarningBuff);
		goto ErrorOut;
    }	

    zerol((char *)sd->data2,length);
  
    if(getPlotImage(Files,(unsigned char *)sd->data2,CurrentFrame)){
		goto ErrorOut;
    }	
	    
        
	sd->pd.dmin=pio->cmin;
	sd->pd.dmax=pio->cmax;	
    sd->limits = *limits;	
    
    ret=0;
    
ErrorOut:

	pio->pd.AreaFill=AreaSave;
	
	return ret;

}
static int doSage1DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	int AreaSave;
	long length;
    double *sout;
    int ret;

	if(!Files)return 1;
	pio=&Files->pioData;
	if(!sd)return 1;
	limits=&Files->limits;

	ret=1;
	
	AreaSave=pio->pd.AreaFill;
	
 	if(sd->type == FRAME_DATA_FLOAT_RASTER){
 	    return FloatAndRaster(Files,CurrentFrame,sd);
 	}

	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           ;
	    }else if(sd->type != FRAME_DATA_RASTER){
		    sprintf(WarningBuff,"doSage1DGetData - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
	    }else if(!pio->pa.DrawZones && !pio->vector.DrawVectors && !pio->active){
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

	AreaSave=pio->pd.AreaFill;
	
	pio->pd.AreaFill=TRUE;
	
	if(SetPIOData(Files,CurrentFrame))goto ErrorOut;
	
	pio->pd.AreaFill=AreaSave;
        			
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9203);
    if(!sd->data){
    	sprintf(WarningBuff,"doSage1DGetData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }

    sout=(double *)sd->data;
    zerol((char *)sout,length*sizeof(double));
    
    if(sd->type == FRAME_DATA_FLOAT){
		setFloat(sout,length);
	    if(getPlotData(Files,sout,CurrentFrame)){
		    sprintf(WarningBuff,"doSage1DGetData - getPlotData Error\n");
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

	pio->pd.AreaFill=AreaSave;
	
	return ret;

}


static int doSage1DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
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
	s = &Files->SetData;
	limits=&Files->limits;
	pio=&Files->pioData;

	ret=1;
	
	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage1DGetCellData - Only Returns LINEDATA_CELL_DATA\n");
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
	    ret=17;goto ErrorOut;
	}
	        			
    sd->xsize=length;
    sd->ysize=1;
    sd->zsize=1;
    sd->length=length;
    sd->data=fdata;
    sd->pioTime=s->pioTime;
    if(!sd->data){
    	sprintf(WarningBuff,"doSage1DGetCellData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }
    
	limits->xmin=sage->xzero;
	limits->ymin=0;
	limits->xmax=sage->dxset*sage->ixmax+sage->xzero;
	limits->ymax=0;
	limits->time=s->pioTime;
	limits->zmin=0;
	limits->zmax=0;
    ret=0;
    
ErrorOut:
	
	return ret;

}

int doDataScale(struct FileInfo2 *Files);
int doDataRange(struct FileInfo2 *Files);
int doDataSelection(struct FileInfo2 *Files);
int doVectorSelection(struct FileInfo2 *Files);
int doAreaSelection(struct FileInfo2 *Files);

static int doSage1DDoDialog(struct FileInfo2 *Files,int *type)
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
	}else if(*type == MessageType_DO_AREA_DIALOG){
		return doAreaSelection(Files);	
	}
	return 0;
}
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li)
{
	double dp,dr,xs,nr,nf;
	long ns,n,nd,NumberOfRings;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	double valuet;
	double x;
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

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81877))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81886))){
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

	
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	pio->pioIndex=(int)li->pioIndex;
	pio->pd.AreaFill=TRUE;
	
	x=li->x1;
	if(pio->pd.Rotate1d){
		x=sqrt(li->x1*li->x1+li->y1*li->y1);
	}
	
	if(li->SampleSteps < 1)li->SampleSteps=1;
	
	NumberOfRings=li->SampleSteps;
	
	if(NumberOfRings > 1){
	    dr=(2*li->SampleRadius)/(double)(NumberOfRings-1);
	}else{
	   dr=li->SampleRadius;
	}

	xs=x-li->SampleRadius;
	nd=0;
	for(n=nFirst;n<=nLast;++n){
	    double value;
	    long nrr;
	    
	    
	    CurrentFrame=n;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
	    
    	setPioScales(pio);
	    
    	nf=0;
    	valuet=0;
		x=xs;
	    for(nr=0;nr<NumberOfRings;++nr){
	    	if(!pioGetValue(pio,x,&value,&nrr))continue;
	    	/*
		    	sprintf(WarningBuff,"x %g ",x);
		    	WarningBatchFast(WarningBuff);
		    */

		    valuet += value;
		    ++nf;
	    	x += dr;
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
static int pioGetValue(struct FilePIOInfo *pio,double x,double *v,long *nr)
{
	double xc,dx2;
	int level;
	long n,k,ncell;

	if(!pio || !v || !pio->value)return 0;

	if(x < pio->xmin || x > pio->xmax)return 0;

	ncell = -1;
	level = -1;
	for(n=0;n<pio->numcell;++n){
	    k=(long)(pio->level[n]);
	    xc=pio->xcenter[n];
	    dx2=pio->dx2[k]*1.000001;
	    if(x < xc-dx2)continue;
	    if(x > xc+dx2)continue;
	    if(k < level)continue;
	    level = (int)k;
	    ncell=n;
	}
	if(ncell == -1)return 0;
	*v=pio->value[ncell];
	if(nr)*nr=ncell;
	return 1;
}
static int pioGetValue2(struct FilePIOInfo *pio,double x,double y,double *v,long *nr)
{
	double xc,dx2;
	int level;
	long n,k,ncell;

	if(!pio || !v || !pio->value)return 0;

	if(x < pio->xmin || x > pio->xmax)return 0;
	
	if(pio->pd.Rotate1d){
	    if(y < pio->xmin || y > pio->xmax)return 0;
	    x=sqrt(x*x+y*y);
	}
	
	ncell = -1;
	level = -1;
	for(n=0;n<pio->numcell;++n){
	    k=(long)(pio->level[n]);
	    xc=pio->xcenter[n];
	    dx2=pio->dx2[k]*1.000001;
	    if(x < xc-dx2)continue;
	    if(x > xc+dx2)continue;
	    if(k < level)continue;
	    level = (int)k;
	    ncell=n;
	}
	if(ncell == -1)return 0;
	*v=pio->value[ncell];
	if(nr)*nr=ncell;
	return 1;
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
	    		    
	    if(!pioGetValue2(pio,li->x[n],li->y[n],&li->v[n],&index)){
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
	double *xD,*yD,s;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax;
	double rx,ry;
	int ibreak;
	
	if(!li)return 1;
	if(!Files)return 1;
	
	pio=&Files->pioData;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA || ! li->integrate)return 1;
	
	if(pio->pd.Rotate1d){
        sprintf(WarningBuff,"doIntegrateLine1D : Error Rotated Line Integral not implemented\n");
		WarningBatch(WarningBuff);
		goto OutOfHere;
	}

	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;

	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	y1=0;
	y2=0;
	ry=0;


	if(SetPIOData(Files,li->CurrentFrame))goto OutOfHere;
	
	pio->doGradients = FALSE;

    setPioScales(pio);
	
	sage=pio->sage;
	
	np=10000;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81875))){
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
	    double value,dx2,xc,dl,s2;
	    long k;
	    if(!pioGetValue2(pio,x+dx,y+dy,&value,&ne)){
	        break;
	    }
	    k=(long)(pio->level[ne]);
        xc=pio->xcenter[ne];
	    dx2=pio->dx2[k];
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
	  	    
	    if(rx){
	        s=((xc-dx2)-x1)/rx;
	        s2=((xc+dx2)-x1)/rx;
	        if(s2 > s){	  
	            s=s2;
	        }      
	    }else{
	        sprintf(WarningBuff,"doIntegrateLine : Error Cell %ld No intersection\n",ne);
    		WarningBatch(WarningBuff);
    		goto OutOfHere;
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
	
	return 0;
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}

struct Sort1
{
	long ne;
	double x;
	double dx2;
};

static int intcmp3(const void *xx,const  void *yy)
{
	double sum;
	register struct Sort1  *x=(struct Sort1  *)xx;
	register struct Sort1  *y=(struct Sort1  *)yy;
	sum = x->x - y->x;
	if(sum == 0.0)return 0;
	return (int)((sum > 0.0) ? 1 : -1);
}
#define SORTMAX 50000L

struct LineInfo{
	long xsize;
	long ysize;
	long *values;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
};

int doIntegrateLineRadius(struct LineInfo *info,struct linedata *li);

int doIntegrateLineRadius(struct LineInfo *info,struct linedata *li)
{
	double *xD,*yD,s,s1,s2,s3,s4;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	long *values;
	double rx,ry;
	int ibreak;
	double odxset,odyset;
	double dx2,dy2;
	
	if(!li)return 1;
	if(!info)return 1;
	
	values=info->values;
	if(!values)return 1;
	
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
		if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81811))){
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
	
	odxset=(double)(info->xsize)/(info->xmax-info->xmin);
	dx2=0.5/odxset;


	odyset=(double)(info->ysize)/(info->ymax-info->ymin);
	dy2=0.5/odyset;

	while(1){
	    double value,xc,yc,dl,yy,xx;
	    long i,j;
	    
	    xx=x+dx;
	    if(xx < info->xmin || xx > info->xmax)break;
	    
	    yy=y+dy;
	    if(yy < info->ymin || yy > info->ymax)break;
	    
		i=(long)((xx-info->xmin)*odxset);	
		if(i < 0){
		    i=0;
		}else if(i >= info->xsize){
		    i=info->xsize-1;
		}
	    
		j=(long)((yy-info->ymin)*odyset);
		if(j < 0){
		    j=0;
		}else if(j >= info->ysize){
		    j=info->ysize-1;
		}
		
		
	    ne=i+j*info->xsize;
	    if(ne < 0 || ne >= info->xsize*info->ysize){
			break;
	    }
		
		value=values[ne];
        xc=((double)i+0.5)*dx2*2.0+info->xmin;
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
        yc=((double)j+0.5)*dy2*2.0+info->ymin;
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
		
    xD=NULL;
    yD=NULL;

	return 0;
	
OutOfHere:

	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}

static double valueSum;

static int CalculateBhangMeterMergeGroup(mThread *Threads)
{
	struct FilePIOInfo *pio;
	struct linedata li;
	struct DrawData *d;
	double **l351;
	double **l951;
	int numGroup;
	int ng;
	double sumValue;
	double pi;
	double nx,ny;
	double dx2,dy2,dr;
	long nny,nnx;
	long xn,yn;
	int bhangMeterSteps;
	struct LineInfo *info;
	
	double value[2000];


	sumValue=0;
	
	if(!Threads)goto ErrorOut;
	d=(struct DrawData *)Threads->data;
	if(!d)goto ErrorOut;
	
	pi=4.0*atan(1.0);
	numGroup=d->numGroup;
	pio=d->pio;
	l951=d->l951;
	l351=d->l351;
	li=*d->li;
	dr=d->dr;
	nny=d->nny;
	nnx=d->nnx;
	ny=d->ny;
	nx=d->nx;
	bhangMeterSteps=d->bhangMeterSteps;
	info=d->info;
		
	dx2=0.5*dr;
	dy2=0.5*dr;
	
	for(yn=Threads->smin;yn<Threads->smax;++yn){
		double xx,yy,r2,nxx,nyy,costh,dl;
		double surfaceArea;
		double xc,yc,r;
		long ne,m;
		yc=yn*dr-bhangMeterSteps*dr+0.5*dr;
		for(xn=0;xn<nnx;++xn){
			xc=xn*dr-bhangMeterSteps*dr+0.5*dr;
			r=sqrt(xc*xc+yc*yc);
			li.x1=xc;
			li.y1=yc;
			
			if(doIntegrateLineRadius(info,&li))continue;
			
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
			
			surfaceArea=0.5*2.0*pi*costh*((fabs(xc)+dx2)*(fabs(xc)+dx2)-(fabs(xc)-dx2)*(fabs(xc)-dx2)+2*dy2*((fabs(xc)+dx2)+(fabs(xc)-dx2)));
			
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
			}
			
			for(ng=0;ng<numGroup;++ng){
				sumValue += value[ng];
			}
		}
	}
	
	if(li.xData)cFree((char *)li.xData);
	if(li.yData)cFree((char *)li.yData);
	
ErrorOut:
	Threads->value1=sumValue;
	Threads->done=TRUE;
	return 0;
	
	
}	

static int BhangMeterGetGroupRadius(struct FileInfo2 *Files,struct linedata *lii)
{
	char name[256];
	struct LineInfo info;
	double nx,ny,mag;
	double dp;
	long ns,n,nd,nn,mm;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	struct linedata li;
	double pi;
	double *l351[2000];
	double *l951[2000];
	int numGroup;
	int ng;
	struct Sort1 *sort;
	long nume;
	int ret;
	int k;
	long lengthErrors;
	int bhangMeterSteps;
	double dr;
	long nnx,nny;
	long xn,yn;
	long *numr;
	double start,end;
	struct DrawData d;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!lii)return 1;
	
	start=rtime();

	ret=1;
	
	
	xD=NULL;
	yD=NULL;
	
	numr=NULL;
	sort=NULL;
	
	numGroup=0;
	
	lii->dCount=0;
   	lii->xData=NULL;
	lii->yData=NULL;
	
	lengthErrors=0;
	
	if(lii->type != LINEDATA_BHANGMETER_DATA)return 1;
	
	if(lii->bhangMeterRadius <= 0.0){
		return 1;
	}
	
	nFirst=lii->FirstFrame;
	nLast=lii->LastFrame;
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
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),91876))){
	    WarningBatch("BhangMeterGetGroupRadius Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81886))){
	    WarningBatch("BhangMeterGetGroupRadius Out of List Memory\n");
	    goto OutOfHere;
	}
	
	if(!(sort=(struct Sort1 *)cMalloc((long)sizeof(struct Sort1)*(SORTMAX),81896))){
	    WarningBatch("BhangMeterGetGroupRadius Out of Sort Memory\n");
	    goto OutOfHere;
	}
	
	if(lii->bhangMeterSteps <= 0)lii->bhangMeterSteps=5;
	
	bhangMeterSteps=lii->bhangMeterSteps;
	
	nnx=2*lii->bhangMeterSteps;
	
	nny=2*lii->bhangMeterSteps;
	
	if(!(numr=(long *)cMalloc((long)sizeof(long)*(nnx*nny),81886))){
	    WarningBatch("BhangMeterGetGroupRadius Out of List Memory\n");
	    goto OutOfHere;
	}
	
	
	dr=lii->bhangMeterRadius/lii->bhangMeterSteps;
	
	
	info.xsize=nnx;
	info.ysize=nny;
	info.values=numr;
	info.xmax=lii->bhangMeterRadius;
	info.xmin = -info.xmax;
	
	info.ymax=lii->bhangMeterRadius;
	info.ymin = -info.ymax;
	
	tFirst=lii->FirstTime;
	tLast=lii->LastTime;
	
	if(tFirst >= tLast){
	    if(Files){
	    	tFirst=0;
	    	tLast=(double)(Files->ImageCount-1);
	    }else{
	    	goto OutOfHere;
	    }
	}
	
	
	nx=lii->nx;
	ny=lii->ny;
	
	mag=sqrt(nx*nx+ny*ny);
	if(mag > 0.0){
		nx /= mag;
		ny /= mag;
	}
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;
	
	
	pi=4.0*atan(1.0);
	
	
	
	li=*lii;
	li.xData=NULL;
	li.yData=NULL;
	
	nd=0;
	for(nn=nFirst;nn<=nLast;++nn){
		long dataLength;
		double dx2;
	    
		
	    CurrentFrame=nn;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
		
		if(pio->numcell >= SORTMAX)
		{
			sprintf(WarningBuff,"BhangMeterGetGroupRadius : numcell %ld > SORTMAX %ld\n",
					pio->numcell,SORTMAX);
			WarningBatch(WarningBuff);
			goto OutOfHere;
		}
		
		nume=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
			sort[nume].ne=n;
			k=(int)(pio->level[n]);
			dx2=pio->dx2[k];
			sort[nume].dx2=dx2;
			sort[nume++].x=pio->xcenter[n];
			
		}
		
		qsort((char *)sort,nume, sizeof(struct Sort1),intcmp3);
		
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
			sprintf(WarningBuff,"BhangMeterGetGroupRadius Error - Frame %ld No Multigroup data Found\n",nn);
			WarningBatch(WarningBuff);
			goto OutOfHere;
			
		}
		
		for(yn=0;yn<nny;++yn){
			double xc,yc,r;
			int ifound;
			yc=yn*dr-bhangMeterSteps*dr+0.5*dr;
			for(xn=0;xn<nnx;++xn){
			    xc=xn*dr-bhangMeterSteps*dr+0.5*dr;
				r=sqrt(xc*xc+yc*yc);
				ifound=0;
				for(mm=0;mm<nume;++mm){
				    if(r >= sort[mm].x-sort[mm].dx2 && r <= sort[mm].x+sort[mm].dx2){
						numr[xn+yn*nnx]=sort[mm].ne;
						ifound=1;
						break;
					}
				}
				
				if(!ifound){
					sprintf(WarningBuff,"BhangMeterGetGroupRadius Error no cell found at Radius %g \n",r);
					WarningBatch(WarningBuff);
					ret=93;goto OutOfHere;
				}
			}
		}
		
		d.dr=dr;
		d.nny=nny;
		d.nnx=nnx;
		d.bhangMeterSteps=bhangMeterSteps;
		d.info=&info;
		d.li=&li;
		d.numGroup=numGroup;
		d.l951=l951;
		d.l351=l351;
		d.nx=nx;
		d.ny=ny;
		
		valueSum=0.0;
		
		runThreads2(15,&d,nny,CalculateBhangMeterMergeGroup,GatherBhangMeterMerge);
		
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
		
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
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
	}
	
	
	if(!nd)goto OutOfHere;
	
	if(lengthErrors > 0)
	{
		sprintf(WarningBuff,"BhangMeterGetGroupRadius Line lengthErrors %ld \n",lengthErrors);
		WarningBatch(WarningBuff);
	}
	
	ret=0;
	
	lii->dCount=nd;
   	lii->xData=xD;
	lii->yData=yD;
	lii->rymin=rymin;
	lii->rymax=rymax;
	xD=NULL;
	yD=NULL;
	
	pioSetLimits(Files,0L);
	
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(numr)cFree((char *)numr);
	if(sort)cFree((char *)sort);
	
	if(li.xData)cFree((char *)li.xData);
	if(li.yData)cFree((char *)li.yData);
	
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
	
	end=rtime();
	/*
	 sprintf(WarningBuff,"BhangMeterGetGroupRadius : %.2f Seconds Threads %d\n",end-start,pio->threads);
	 WarningBatch(WarningBuff);
	 */
	
	return ret;
}

static int GatherBhangMeterMerge(mThread *Threads, int count)
{
	int n;
	
	valueSum  =   0.0;
	
	if(!Threads)return 1;
	
	for(n=0;n<count;++n)
	{
		valueSum += Threads[n].value1;
		
	}
	
	return 0;
}	


static int doBhangMeterGetDataRadius(struct FileInfo2 *Files,struct linedata *lii)
{
	struct LineInfo info;
	double nx,ny,mag;
	double dp;
	long ns,n,nd,nn,mm;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	struct linedata li;
	double pi;
	double *lambda;
	double *t;
	struct Sort1 *sort;
	long nume;
	int ret;
	int k;
	double a=137.20172;
	double c=2.99792458e10;
	long lengthErrors;
	int bhangMeterSteps;
	double dr;
	long nnx,nny;
	long xn,yn;
	long *numr;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!lii)return 1;
	
	if(lii->multigroup){
		return BhangMeterGetGroupRadius(Files,lii);
	}
	
	
	ret=1;
	
	t=NULL;
	lambda=NULL;
	
	xD=NULL;
	yD=NULL;
	
	numr=NULL;
	sort=NULL;
	
	lii->dCount=0;
   	lii->xData=NULL;
	lii->yData=NULL;
	
	lengthErrors=0;
	
	if(lii->type != LINEDATA_BHANGMETER_DATA)return 1;
	
	if(lii->bhangMeterRadius <= 0.0){
		return 1;
	}
	
	nFirst=lii->FirstFrame;
	nLast=lii->LastFrame;
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
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),91876))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81886))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	
	if(!(sort=(struct Sort1 *)cMalloc((long)sizeof(struct Sort1)*(SORTMAX),81896))){
	    WarningBatch("doPointGetData Out of Sort Memory\n");
	    goto OutOfHere;
	}
	
	if(lii->bhangMeterSteps <= 0)lii->bhangMeterSteps=5;
	
	bhangMeterSteps=lii->bhangMeterSteps;
	
	nnx=2*lii->bhangMeterSteps;
	
	nny=2*lii->bhangMeterSteps;
	
	if(!(numr=(long *)cMalloc((long)sizeof(long)*(nnx*nny),81886))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	
	
	dr=lii->bhangMeterRadius/lii->bhangMeterSteps;
	
	
	info.xsize=nnx;
	info.ysize=nny;
	info.values=numr;
	info.xmax=lii->bhangMeterRadius;
	info.xmin = -info.xmax;
	
	info.ymax=lii->bhangMeterRadius;
	info.ymin = -info.ymax;

	tFirst=lii->FirstTime;
	tLast=lii->LastTime;
	
	if(tFirst >= tLast){
	    if(Files){
	    	tFirst=0;
	    	tLast=(double)(Files->ImageCount-1);
	    }else{
	    	goto OutOfHere;
	    }
	}
	
	
	nx=lii->nx;
	ny=lii->ny;
	
	mag=sqrt(nx*nx+ny*ny);
	if(mag > 0.0){
		nx /= mag;
		ny /= mag;
	}

	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;
	
	
	pi=4.0*atan(1.0);
	
	
	lambda=NULL;
	t=NULL;
	
	li=*lii;
	li.xData=NULL;
	li.yData=NULL;
	
	nd=0;
	for(nn=nFirst;nn<=nLast;++nn){
		long dataLength;
		double valueSum;
		double dx2,dy2;
	    
		
	    CurrentFrame=nn;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
		
		if(pio->numcell >= SORTMAX)
		{
			sprintf(WarningBuff,"doBhangMeterGetData : numcell %ld > SORTMAX %ld\n",
					pio->numcell,SORTMAX);
			WarningBatch(WarningBuff);
			goto OutOfHere;
		}
		
		nume=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
			sort[nume].ne=n;
			k=(int)(pio->level[n]);
			dx2=pio->dx2[k];
			sort[nume].dx2=dx2;
			sort[nume++].x=pio->xcenter[n];
			
		}
		
		
		qsort((char *)sort,nume, sizeof(struct Sort1),intcmp3);
		
		if(SageGetFileItem(Files,"lgt(-1)",0,
						   &lambda,&dataLength,CurrentFrame)){
		    ret=23;goto OutOfHere;
		}
		
		if(SageGetFileItem(Files,"tev",0,
						   &t,&dataLength,CurrentFrame)){
		    ret=24;goto OutOfHere;
		}
		
		for(yn=0;yn<nny;++yn){
			double xc,yc,r;
			int ifound;
			yc=yn*dr-bhangMeterSteps*dr+0.5*dr;
			for(xn=0;xn<nnx;++xn){
			    xc=xn*dr-bhangMeterSteps*dr+0.5*dr;
				r=sqrt(xc*xc+yc*yc);
				ifound=0;
				for(mm=0;mm<nume;++mm){
				    if(r >= sort[mm].x-sort[mm].dx2 && r <= sort[mm].x+sort[mm].dx2){
						numr[xn+yn*nnx]=sort[mm].ne;
						ifound=1;
						break;
					}
				}
				
				if(!ifound){
					sprintf(WarningBuff,"doBhangMeterGetDataRadius Error no cell found at Radius %g \n",r);
					WarningBatch(WarningBuff);
					ret=93;goto OutOfHere;
				}
			}
		}
		
		valueSum=0.0;
				
	    dx2=0.5*dr;
	    dy2=0.5*dr;
		
		for(yn=0;yn<nny;++yn){
			double xx,yy,r2,nxx,nyy,costh,dl;
			double surfaceArea,value;
			double xc,yc,r;
			long ne,m;
			yc=yn*dr-bhangMeterSteps*dr+0.5*dr;
			for(xn=0;xn<nnx;++xn){
			    xc=xn*dr-bhangMeterSteps*dr+0.5*dr;
				r=sqrt(xc*xc+yc*yc);
				li.x1=xc;
				li.y1=yc;
				
				if(doIntegrateLineRadius(&info,&li))continue;
				
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
										
				surfaceArea=0.5*2.0*pi*costh*((fabs(xc)+dx2)*(fabs(xc)+dx2)-(fabs(xc)-dx2)*(fabs(xc)-dx2)+2*dy2*((fabs(xc)+dx2)+(fabs(xc)-dx2)));
				
				value=surfaceArea*(c*a*pow(t[ne],4.0)/4.0)/(4.0*pi*r2);
			

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
				}
				
				valueSum += value;
			}
		}
				
		if(t)cFree((char *)t);
		t=NULL;
		if(lambda)cFree((char *)lambda);
		lambda=NULL;
		
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
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
	}
	
	
	if(!nd)goto OutOfHere;
	
	if(lengthErrors > 0)
	{
		sprintf(WarningBuff,"doBhangMeterGetDataRadius Line lengthErrors %ld \n",lengthErrors);
		WarningBatch(WarningBuff);
	}
	
	ret=0;
	
	lii->dCount=nd;
   	lii->xData=xD;
	lii->yData=yD;
	lii->rymin=rymin;
	lii->rymax=rymax;
	xD=NULL;
	yD=NULL;
	
	pioSetLimits(Files,0L);
	
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(t)cFree((char *)t);
	if(lambda)cFree((char *)lambda);
	if(numr)cFree((char *)numr);
	if(sort)cFree((char *)sort);

	if(li.xData)cFree((char *)li.xData);
	if(li.yData)cFree((char *)li.yData);

	return ret;
}

static int doBhangMeterGetGroup(struct FileInfo2 *Files,struct linedata *li)
{
	char name[256];
	double dp;
	long ns,n,nd,nn,mm;
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
		double *lambda;
	double *t;
	struct Sort1 *sort;
	long nume;
	int ret;
	int k;
	long lengthErrors;
/*	double *mgrp_bnd; */
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	if(li->bhangMeterRadius > 0.0){
		return doBhangMeterGetDataRadius(Files,li);
	}
	ret=1;
	
	t=NULL;
	lambda=NULL;
	sort=NULL;
	
	numGroup=0;
	
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
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),91876))){
	    WarningBatch("doBhangMeterGetGroup Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),91886))){
	    WarningBatch("doBhangMeterGetGroup Out of List Memory\n");
	    goto OutOfHere;
	}
	
	if(!(sort=(struct Sort1 *)cMalloc((long)sizeof(struct Sort1)*(SORTMAX),81891))){
	    WarningBatch("doBhangMeterGetGroup Out of Sort Memory\n");
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
	
	
	lambda=NULL;
	t=NULL;
	
	
	
	
	nd=0;
	for(nn=nFirst;nn<=nLast;++nn){
		long dataLength;
		double valueSum,valueSumG[2000];
		double xmin,xmax,dx2,x1;
		double length;
	    
		
	    CurrentFrame=nn;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
		
		if(pio->numcell >= SORTMAX)
		{
			sprintf(WarningBuff,"doBhangMeterGetGroup : numcell %ld > SORTMAX %ld\n",
					pio->numcell,SORTMAX);
			WarningBatch(WarningBuff);
			goto OutOfHere;
		}
		
		nume=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
			sort[nume].ne=n;
			sort[nume++].x=pio->xcenter[n];
			
		}
		
		
		qsort((char *)sort,nume, sizeof(struct Sort1),intcmp3);
		
		if(SageGetFileItem(Files,"lgt(-1)",0,
						   &lambda,&dataLength,CurrentFrame)){
		    ret=23;goto OutOfHere;
		}
		
		if(SageGetFileItem(Files,"tev",0,
						   &t,&dataLength,CurrentFrame)){
		    ret=24;goto OutOfHere;
		}
		
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
			sprintf(WarningBuff,"doBhangMeterGetGroup Error - Frame %ld No Multigroup data Found\n",nn);
			WarningBatch(WarningBuff);
			goto OutOfHere;
			
		}
/*
		if(SageGetFileItem(Files,"mgrp_bnd",0,
						   &mgrp_bnd,&dataLength,CurrentFrame)){
			ret=23;goto OutOfHere;
		}
*/
		for(ng=0;ng<numGroup;++ng){
			valueSumG[ng]=0.0;
		}
		
		
		length=0;
		
		for(mm=0;mm<nume;++mm){
			double value[2000];
			n=sort[mm].ne;
			k=(int)(pio->level[n]);
			dx2=pio->dx2[k];
			x1=pio->xcenter[n];
			xmin=x1-dx2;
			xmax=x1+dx2;
			
			
			if(li->x2 < xmin){
				xmin=xmax;
				break;
			}
			
			/* value = (0.25*c*a*pow(t[n],4.0)*xmax*xmax/pi)*(1-exp(-2*dx2/lambda[n])); */
			
				
			for(ng=0;ng<numGroup;++ng){
/*
				double h=2*3.14159*6.582e-16;
				double c=3e10;
				double hnu;
				double dnu;
				double bn;
				
				if(ng == 0){
					hnu=mgrp_bnd[ng]/2.0;
					dnu = mgrp_bnd[ng];
				}else{
					hnu=(mgrp_bnd[ng]-mgrp_bnd[ng-1])/2.0;
					dnu = mgrp_bnd[ng]-mgrp_bnd[ng-1];
				}
				
				bn=2*hnu*hnu*hnu/(h*c*h*c);
				
				bn=bn/(exp(hnu/t[n])-1.0);
				
				if(n == 1){
					printf("%d %g %g\n",ng,bn,l951[ng][n]/dnu);
				}
				
				 value[ng]=(bn)*xmax*xmax*(1.0-exp(-2.0*dx2*l351[ng][n]));
 */
				
				value[ng]=(l951[ng][n])*xmax*xmax*(1.0-exp(-2.0*dx2*l351[ng][n]));
			}
			
			
			if(li->x2 < xmax){
				break;  /* last zone adds nothing */
			}
			
			/* valueSum = (valueSum*exp(-2*dx2/lambda[n])*(xmin*xmin)+value)/(xmax*xmax); */
			
			for(ng=0;ng<numGroup;++ng){
				valueSumG[ng]=(valueSumG[ng]*exp(-2*dx2*l351[ng][n])*(xmin*xmin)+value[ng])/(xmax*xmax);
			}
		}
		
		valueSum=0.0;
		
		for(ng=0;ng<numGroup;++ng){
			valueSum += valueSumG[ng];
		}
	
		/* valueSum *= 2*pi; */
	
		/*
		 if(length > 0){
		 if(fabs(length-rx)/length > 1e-5){
		 lengthErrors++;
		 }
		 }
		 */
		if(t)cFree((char *)t);
		t=NULL;
		if(lambda)cFree((char *)lambda);
		lambda=NULL;
		
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
		
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(nn >= 0 && nn < Files->ImageCount){
			struct PIO *sage;
			sage = &Files->PIO2DList[nn];
			dp=sage->time;
	    }
	    xD[nd]=dp;
	    yD[nd]=valueSum;
		/*
		 if(nn == 92)
		 {
		 sprintf(WarningBuff,"doBhangMeterGetData valueSum %g \n",valueSum);
		 WarningBatch(WarningBuff);
		 }
		 */
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(valueSum < rymin)rymin=valueSum;
	    if(valueSum > rymax)rymax=valueSum;
	    nd++;
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
	if(sort)cFree((char *)sort);
	
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
	
	return ret;
}


static int doBhangMeterGetData(struct FileInfo2 *Files,struct linedata *li)
{
	double dp;
	long ns,n,nd,nn,mm;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	double pi;
	double *lambda;
	double *t;
	struct Sort1 *sort;
	long nume;
	int ret;
	int k;
	double a=137.20172;
	double c=2.99792458e10;
	long lengthErrors;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	if(li->bhangMeterRadius > 0.0){
		return doBhangMeterGetDataRadius(Files,li);
	}
	
	if(li->multigroup){
		return doBhangMeterGetGroup(Files,li);
	}
	ret=1;
	
	t=NULL;
	lambda=NULL;
	sort=NULL;
	
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
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),91876))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),91886))){
	    WarningBatch("doPointGetData Out of List Memory\n");
	    goto OutOfHere;
	}
	
	if(!(sort=(struct Sort1 *)cMalloc((long)sizeof(struct Sort1)*(SORTMAX),81891))){
	    WarningBatch("doPointGetData Out of Sort Memory\n");
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
	
	
	lambda=NULL;
	t=NULL;
	
	
	nd=0;
	for(nn=nFirst;nn<=nLast;++nn){
		long dataLength;
		double valueSum;
		double xmin,xmax,dx2,x1;
		double length;
	    
		
	    CurrentFrame=nn;
	    if(SetPIOData(Files,CurrentFrame))goto OutOfHere;
		
		if(pio->numcell >= SORTMAX)
		{
			sprintf(WarningBuff,"doBhangMeterGetData : numcell %ld > SORTMAX %ld\n",
					pio->numcell,SORTMAX);
			WarningBatch(WarningBuff);
			goto OutOfHere;
		}
		
		nume=0;
		for(n=0;n<pio->numcell;++n){
			if(pio->daughter[n])continue;
			sort[nume].ne=n;
			sort[nume++].x=pio->xcenter[n];
			
		}
		
		
		qsort((char *)sort,nume, sizeof(struct Sort1),intcmp3);
		
		if(SageGetFileItem(Files,"lgt(-1)",0,
						   &lambda,&dataLength,CurrentFrame)){
		    ret=23;goto OutOfHere;
		}
		
		if(SageGetFileItem(Files,"tev",0,
						   &t,&dataLength,CurrentFrame)){
		    ret=24;goto OutOfHere;
		}
		
		valueSum=0.0;
		
		length=0;
		
		for(mm=0;mm<nume;++mm){
			double value;
			n=sort[mm].ne;
			k=(int)(pio->level[n]);
			dx2=pio->dx2[k];
			x1=pio->xcenter[n];
			xmin=x1-dx2;
			xmax=x1+dx2;
						
			
			if(li->x2 < xmin){
				xmin=xmax;
				break;
			}
			
			
			/* value = (0.25*c*a*pow(t[n],4.0)*x1*x1)*(1-exp(-2*dx2/lambda[n]));*/
			
			value = (0.25*c*a*pow(t[n],4.0)*xmax*xmax)*(1-exp(-2*dx2/lambda[n]));
									
			if(li->x2 < xmax){
				break;  /* last zone adds nothing */
			}
			
			valueSum = (valueSum*exp(-2*dx2/lambda[n])*(xmin*xmin)+value)/(xmax*xmax);
			
		}
/*
		if(length > 0){
			if(fabs(length-rx)/length > 1e-5){
				lengthErrors++;
			}
		}
*/
		if(t)cFree((char *)t);
		t=NULL;
		if(lambda)cFree((char *)lambda);
		lambda=NULL;
		
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(nn >= 0 && nn < Files->ImageCount){
			struct PIO *sage;
			sage = &Files->PIO2DList[nn];
			dp=sage->time;
	    }
	    xD[nd]=dp;
	    yD[nd]=valueSum;
		/*
		if(nn == 92)
		{
			sprintf(WarningBuff,"doBhangMeterGetData valueSum %g \n",valueSum);
			WarningBatch(WarningBuff);
		}
		*/
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(valueSum < rymin)rymin=valueSum;
	    if(valueSum > rymax)rymax=valueSum;
	    nd++;
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
	if(sort)cFree((char *)sort);

	return ret;
}
static int doSage1DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	
	long ns,n,nn,nd,k,ne,PointLineCount;
	double *xD,*yD,*xy,ds;
	double rmin,rmax,r,dr;
	double x1,y1,x2,y2;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	
	
	if(!li)return 1;
	if(li->type == LINEDATA_POINT_DATA){
	    return doPointGetData(Files,li);
	}else if(li->type == LINEDATA_LIST_DATA){
	     return doPointGetListData(Files,li);
	}else if(li->type == LINEDATA_BHANGMETER_DATA){
		return doBhangMeterGetData(Files,li);
	}
if(!Files)return 1;
	pio=&Files->pioData;
	
	xy=NULL;
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
	        

	CurrentFrame=li->CurrentFrame;

	if(SetPIOData(Files,CurrentFrame))goto OutOfHere;

	ns=pio->numcell;
	
	PointLineCount=li->PointLineCount;
	
	if(PointLineCount < 2 || PointLineCount > 20000)PointLineCount=ns;
	

	if(!(xy=(double *)cMalloc((long)sizeof(double)*(2*ns),8186))){
	    WarningBatch("doLineGetData Out of Memory\n");
	    goto OutOfHere;
	}
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(PointLineCount),81877))){
	    WarningBatch("doLineGetData Out of Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(PointLineCount),81887))){
	    WarningBatch("doLineGetData Out of Memory\n");
	    goto OutOfHere;
	}
		
	if(pio->pd.Rotate1d){
		x1=li->x1;
		y1=li->y1;
		x2=li->x2;
		y2=li->y2;
	}else{
		x1=li->x1;
		y1=0;
		x2=li->x2;
		y2=0;	
	}
			
	rmin  =1e60;
	rmax= -1e60;

	nd=0;
	for(n=0;n<pio->numcell;++n){
		if(pio->daughter[n])continue;
	    k=(long)(pio->level[n]);
	    dr=pio->dx2[k];
		r=pio->xcenter[n];
	    xy[2*nd]=r-dr;
	    if(r+dr > rmax)rmax=r+dr;
	    if(r-dr < rmin)rmin=r-dr;
	    xy[2*nd+1]=(double)n;
	    nd++;
	}
	
	if(!nd)goto OutOfHere;

	li->rymin=rmin;
	li->rymax=rmax;
		
	qsort((char *)xy,(size_t)nd, (size_t)(2*sizeof(double)),intcmp99);
	
	ns=PointLineCount;
	ds=1.0/((double)(ns-1));
	ne=0;
	for(nn=0;nn<ns;++nn){
		double svalue,value,xc;
	    double x,y;

	    x=x1+(double)nn*ds*(x2-x1);
	    y=y1+(double)nn*ds*(y2-y1);
	    
	    if(pio->pd.Rotate1d){
	    	if( y < 0.0 && x < 0.0)continue;
	    	r=sqrt(x*x+y*y);
			if(r < rmin || r > rmax)continue;
		}else{
			r=x;
		}
	    n=(long)pioFindValue(r,xy,nd);
		xc=pio->xcenter[n];
	    value=pio->value[n];
        if(pio->doGradients){
           svalue=value+(r-xc)*pio->gradx[n];
        }else{
           svalue=value;
        }

	    xD[ne]=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
	    yD[ne]=svalue;
	    ++ne;
	}
	
	
    if(xy)cFree((char *)xy);
        	
	li->dCount=ne;
   	li->xData=xD;
	li->yData=yD;
	
	return 0;
OutOfHere:
    if(xy)cFree((char *)xy);
    if(xD)cFree((char *)xD);
    if(yD)cFree((char *)yD);
	return 1;

}
static double pioFindValue(double x,double *xy,long nd)
{
/*
	double value,dx,dy,x1,x2,y1,y2;
*/
	long it,ib,ns,n;
	
	if(!xy || nd <= 0)return 0.;
	
	if(nd == 1 || x <= xy[0])return xy[1];
	if(x >= xy[2*(nd-1)])return xy[2*(nd-1)+1];
	
	it=nd-1;
	ib=0;
	while(it > ib+1){
	    ns=(it+ib)/2;
	    if(x > xy[2*ns]){
	        ib=ns;
	    }else{
	        it=ns;
	    }
	}
	n=ib;
	
/*
    x1=xy[2*n];
    x2=xy[2*(n+1)];
    y1=xy[2*n+1];
    y2=xy[2*(n+1)+1];
    dx=x2-x1;
    dy=y2-y1;
    if(dx){
        value=y1+dy*(x-x1)/dx;
    }else{
        value=y1;
    }
*/
	return xy[2*n+1];
	
}
static int intcmp99(const void *xx, const void *yy)
{
	double *x;
	double *y;
	double result;
	
	x=(double *)xx;
	y=(double *)yy;
	result = *x - *y;
	if(result == 0.0){
	    return 0;
	}else if(result > 0.0){
	    return 1;
	}
	return -1;
}
static int doSage1DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
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

	if(pio->pioName[0]){
	    mstrncpy(sd->pioName,pio->pioName,255);
	    sd->pioIndex=(int)pio->pioIndex;
	}else{
	   sd->pioName[0]=0;
	   pio->pioIndex=0;
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
	pio->range.xmaxSet=1;
	pio->range.yminSet=1;
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
