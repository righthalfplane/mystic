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
#include "mThread.h"

extern double *buildBlockSDS3D(struct FileInfo2 *Files,long CurrentFrame);

static int doBhangMeterGetData(struct FileInfo2 *Files,struct linedata *li);

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

static int CalculateBhangMeterMerge(mThread *Threads);

static int doIntegrateLineBhangMeter2(struct FilePIOInfo *pio,struct linedata *li);

static long pioGetValue_ne;

int WarningBatchHoldDump(void);
int WarningBatchHold(char *buff);

static int GetRange3(struct uDialogStruct *uList);

static int doSage3DGetStreamLine(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int pioGetValueX(struct FilePIOInfo *pio,double x,double y,double z,long *nn);

int checkStreamData(struct dataStruct *plane);

static int loadVectors(struct FileInfo2 *Files);

static int pioGetValueN(struct FilePIOInfo *pio,double x,double y,double z,long *nn);

int freeStreamLine3d(struct PlotStreamData *streamline3d);

static int pioDrawStream3d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage3DCommandGet(struct FileInfo2 *Files,CommandPtr cp);

static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp);

static int doSage3DSetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doFindMaximum(struct FileInfo2 *Files);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

int doStreamlineSelection(struct PlotStreamData *streamline);

static int doSage3DTracerPlotLineData(struct FileInfo2 *Files,int *list,int nc);

int pioDrawTracers3D(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage3DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files);

static int doSage3DGetGeneratDataOffSet(struct areadata *ai,struct FileInfo2 *Files);

static int doRange4Action(struct uDialogStruct *uList);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

int doVolumeSelection(struct FileInfo2 *Files);

static int doGeneralSelection(struct FileInfo2 *Files);

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li);

int setFloat(double *d,long length);

static int doSage3DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m);

static int doSage3DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int pioDrawLasers(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

/*
static int RangeCell(long n,struct FilePIOInfo *pio,double *dmin,double *dmax);
*/

static int RangeCellVelocity(long n,struct FilePIOInfo *pio,double *vmax);

static int doSage3DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int DrawCellVelocity(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc);
static int DrawCellVelocity2(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc);

static int doSage3DDoDialog(struct FileInfo2 *Files,int *type);
static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int sageLocation(struct FileInfo2 *Files);

static int doAuto(struct FileInfo2 *Files);

static int GetPIOData(struct FileInfo2 *Files,long CurrentFrame);

static int pioSetLimitsSDS3D(struct FileInfo2 *Files,long nf);

int doSage3DStart(struct FileInfo2 *Files);

static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double z,double *v);

static double *buildSDS3D(struct FileInfo2 *Files);

int doSage3D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

static int SageGetGradients(struct FileInfo2 *Files,int flag);

static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag);
static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int doSage3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);
static int doSage3DGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int DrawCellLines(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);
static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);
static int setPioScales(struct FilePIOInfo *pio);
static int doSage3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);
static int doSage3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int doRange3Action(struct uDialogStruct *uList);

static int GatherBhangMeterMerge(mThread *Threads, int count);

static int pioGetValueXX(struct FilePIOInfo *pio,double x,double y,double z,long *nn);

static int BhangMeterGetGroupData(struct FileInfo2 *Files,struct linedata *li);

int doSage3DBatch(BatchPtr Batch,char *name)
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
	
    if(doSage3DStart(Files))goto ErrorOut;
	           
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSage3D(Files,&cp,Batch))goto OK;
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
	    if(GetPIOData(Files,n))goto ErrorOut;
	    if(n >= (Files->ImageCount-1))flag=1;
	    if(PlotPIOData(Files,n,flag))goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
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
static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
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
	
	if(Files->outputType == 0 || Files->outputType == HDF_FILES){ 
	
		getPlotImage(Files,buff,CurrentFrame);
		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    	    
		if(hdfOut(filepath,name,(char *)pio->pd.palette,pio->range.ixmax,pio->range.iymax,(char *)buff,CurrentFrame,flag,limits))goto ErrorOut;

	}else if(Files->outputType == SDS2D_FILES){
	
		getPlotData(Files,(double *)buff,CurrentFrame);
		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	        
	    if(sds2DOut(filepath,name,pio->range.ixmax,pio->range.iymax,(double *)buff,CurrentFrame,flag,limits))goto ErrorOut;
	    
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
        double xc,yc,zc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double value,xs,ys,zs,vout;
        long i,j,ihi,ilo,jhi,jlo;
        int k;

        if(pio->daughter[n])continue;
        k=(int)(pio->level[n]);

	    if(pio->plane.Plane_type == X_PLANE){
	        zc=pio->xcenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        if(pio->plane.Plane_xloc < (zc-dx2) || pio->plane.Plane_xloc > (zc+dx2))continue;
	        zs=pio->plane.Plane_xloc;
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k]*1.000001;
	        dy2=pio->dz2[k]*1.000001;
	    }else if(pio->plane.Plane_type == Y_PLANE){
	        zc=pio->ycenter[n];
	        dx2=pio->dy2[k]*1.000001;
	        if(pio->plane.Plane_yloc < (zc-dx2) || pio->plane.Plane_yloc > (zc+dx2))continue;
	        zs=pio->plane.Plane_yloc;
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        dy2=pio->dz2[k]*1.000001;
	    }else if(pio->plane.Plane_type == Z_PLANE){
	        zc=pio->zcenter[n];
	        dx2=pio->dz2[k]*1.000001;
	        if(pio->plane.Plane_zloc < (zc-dx2) || pio->plane.Plane_zloc > (zc+dx2))continue;
	        zs=pio->plane.Plane_zloc;
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        dy2=pio->dy2[k]*1.000001;
	    }else{
	        sprintf(WarningBuff,"PlotPIOData Unknown Plane option %d\n",(int)pio->plane.Plane_type);
	        WarningBatch(WarningBuff);
	        break;
	    }

		
		ymn=max(ymin,yc-dy2);
		ymx=min(ymax,yc+dy2);
		if(ymx < ymn)continue;

		xmn=max(xmin,xc-dx2);
		xmx=min(xmax,xc+dx2);
		if(xmx < xmn)continue;
		
		ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
		ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);

		jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
		jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);

		value=pio->value[n];
		
	    if(pio->plane.Plane_type == X_PLANE){
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        if(pio->doGradients){
						vout=value+(xs-xc)*pio->grady[n]+
						  		   (ys-yc)*pio->gradz[n]+ 
						  	       (zs-zc)*pio->gradx[n];
						sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=vout;
			        }else{
						vout=value;
						sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=vout;
			        }			        				        
			        if(dmin > vout)dmin=vout;
			        if(dmax < vout)dmax=vout;
			    }
			}
	    }else if(pio->plane.Plane_type == Y_PLANE){
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        if(pio->doGradients){
						vout=value+(xs-xc)*pio->gradx[n]+
						  		   (ys-yc)*pio->gradz[n]+ 
						  	       (zs-zc)*pio->grady[n];
						sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=vout;
			        }else{
						vout=value;
						sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=vout;
			        }			        				        
			        if(dmin > vout)dmin=vout;
			        if(dmax < vout)dmax=vout;
			    }
			}
	    }else if(pio->plane.Plane_type == Z_PLANE){
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        if(pio->doGradients){
						vout=value+(xs-xc)*pio->gradx[n]+
						  		   (ys-yc)*pio->grady[n]+ 
						  	       (zs-zc)*pio->gradz[n];
						sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=vout;
			        }else{
						vout=value;
						sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=vout;
			        }			        				        
			        if(dmin > vout)dmin=vout;
			        if(dmax < vout)dmax=vout;
			    }
			}
	    }
    }
    
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	pioSetLimits(Files,CurrentFrame);
	
    return 0;
        
}
static double *buildSDS3D(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double didx,djdy,dkdz,dmin,dmax;
	double dxdi,dydj,dzdk;
	long ixmax,iymax,izmax;
	double *sout;
	long n,length,nn;
	long error;
	int ret;
 	
	if(!Files)return NULL;
	
	pio = &Files->pioData;

	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return buildBlockSDS3D(Files,pio->CurrentFrame);	
	}
	
	if(!pio->daughter || !pio->level   || !pio->xcenter ||
	   !pio->ycenter  || !pio->zcenter || !pio->value)return NULL;
	
	ret=1;
	sout=NULL;
	
	
	
	xmin=pio->plane.Plane_xmin;
	xmax=pio->plane.Plane_xmax;

	ymin=pio->plane.Plane_ymin;
	ymax=pio->plane.Plane_ymax;

	zmin=pio->plane.Plane_zmin;
	zmax=pio->plane.Plane_zmax;

    ixmax=pio->plane.Plane_xcount;
	
    iymax=pio->plane.Plane_ycount;
    
    izmax=pio->plane.Plane_zcount;
	
	length=ixmax*iymax*izmax;
	sout=(double *)cMalloc(length*sizeof(double),9657);
	if(!sout)goto OutOfHere;

	for(n=0;n<length;++n)sout[n] = 0.0;
	
	
    didx=((double)(ixmax))/(xmax-xmin);
    dxdi = 1.0/didx;
    djdy=((double)(iymax))/(ymax-ymin);
    dydj = 1.0/djdy;
    dkdz=((double)(izmax))/(zmax-zmin);
    dzdk = 1.0/dkdz;
	
	dmin =  1e60;
	dmax = -1e60;
    error=0;
    for(n=0;n<pio->numcell;++n){
        double xc,yc,zc,dx2,dy2,dz2;
        double ymn,ymx,xmn,xmx,zmn,zmx;
        double ovalue,value,xs,ys,zs;
        long i,j,k,ihi,ilo,jhi,jlo,khi,klo;
        int kk;

        if(pio->daughter[n])continue;
        
        kk=(int)(pio->level[n]);
        
        zc=pio->zcenter[n];
        dz2=pio->dz2[kk]*1.000001;

		zmn=max(zmin,zc-dz2);
		zmx=min(zmax,zc+dz2);
		if(zmx < zmn)continue;

        yc=pio->ycenter[n];
        dy2=pio->dy2[kk]*1.000001;
        
		ymn=max(ymin,yc-dy2);
		ymx=min(ymax,yc+dy2);
		if(ymx < ymn)continue;

        xc=pio->xcenter[n];
        dx2=pio->dx2[kk]*1.000001;
        
		xmn=max(xmin,xc-dx2);
		xmx=min(xmax,xc+dx2);
		if(xmx < xmn)continue;

		ilo=max(0,(long)((xmn-xmin)*didx));
		ihi=min(ixmax,(long)((xmx-xmin)*didx));

		jlo=max(0,(long)((ymn-ymin)*djdy));
		jhi=min(iymax,(long)((ymx-ymin)*djdy));

		klo=max(0,(long)((zmn-zmin)*dkdz));
		khi=min(izmax,(long)((zmx-zmin)*dkdz));
		
		value=pio->value[n];
		
	    for(k=klo;k<=khi;++k){
			zs=zmin+((double)k+0.5)*dzdk;
			if(zs < zmn || zs > zmx)continue;
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+((double)j+0.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+((double)i+0.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
					nn=i+j*ixmax+k*ixmax*iymax;
					if(nn < 0 || nn >= length){
					   if(++error < 10){
					       ;
					   }
					}else{
				        if(pio->doGradients){
				            ovalue=value+(xs-xc)*pio->gradx[n]+
				                           (ys-yc)*pio->grady[n]+
				                           (zs-zc)*pio->gradz[n];
				        }else{
				            ovalue=value;
				        }
				        sout[nn]=ovalue;
				        if(ovalue < dmin)dmin=ovalue;
				        if(ovalue > dmax)dmax=ovalue;
				    }
				}
			}
	    }

	}
	
	pio->plane.Plane_dmin=dmin;
	pio->plane.Plane_dmax=dmax;
	
	ret=0;
OutOfHere:
    if(ret){
        if(sout)cFree((char *)sout);
        sout=NULL;
    }
	
	return sout;
}
int doSage3D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
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
	    CurrentFrame=(long)value;
	    if(GetPIOData(Files,CurrentFrame))goto ErrorOut;
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
int doSage3DStart(struct FileInfo2 *Files)
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
	
	
	pio->CurrentFrameGradients =  -1;
	
	pio->CurrentFrameVectors = -1;
	pio->vector.DrawVectors=0;
	pio->vector.arrowLength=0.1;
	pio->vector.arrowAngle=145.0;
	pio->vector.arrowHeads=1;
	pio->vector.vspace= -21;
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

	
    mstrncpy(pio->pioName,"density",255);
    pio->pioIndex=0;

    mstrncpy(pio->vector.vectorxName,"xdt",32);
    mstrncpy(pio->vector.vectoryName,"ydt",32);
    mstrncpy(pio->vector.vectorzName,"zdt",32);
		    
	SetRange(Files,Files->xminr,Files->xmaxr,Files->yminr,Files->ymaxr,USE_XY);
	
	getPaletteByName("ps",pio->pd.palette);
		
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
static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
    struct FilePIOInfo *pio;
	long n;

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
		
	if(li->type != LINEDATA_LIST_DATA)return 1;
	
	if(!li->x || !li->y || !li->z || !li->v || (li->PointLineCount < 1))return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	pio->pioIndex=(int)li->pioIndex;

	if(GetPIOData(Files,li->CurrentFrame))goto OutOfHere;

    setPioScales(pio);
	
	for(n=0;n<li->PointLineCount;++n){
	    if(!pioGetValue(pio,li->x[n],li->y[n],li->z[n],&li->v[n])){
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
	double *xD,*yD,s,s1,s2;
	double x1,y1,z1,x2,y2,z2,x,y,z,xo,yo,zo,dx,dy,dz,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double rx,ry,rz;
	double llength;
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
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;


	if(GetPIOData(Files,li->CurrentFrame))goto OutOfHere;
	
	pio->doGradients = FALSE;

    setPioScales(pio);

	sage=pio->sage;
	
	np=10000;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81878))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(np),81883))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}	
		
	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	
	y1=li->y1;
	y2=li->y2;
	ry=y2-y1;
	
	z1=li->z1;
	z2=li->z2;
	rz=z2-z1;

	llength=sqrt(rx*rx+ry*ry+rz*rz);
	    
	x=x1;
	y=y1;
	z=z1;
	
	length=0;
	integral=0;

	dx=0;
	dy=0;
	dz=0;
	
	s=0;
	
	nd=0;
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;	
	
	while(1){
	    double value,dx2,dy2,dz2,xc,yc,zc,dl,yy,xx,zz;
	    long k;
	    if(!pioGetValueX(pio,x+dx,y+dy,z+dz,&ne)){
	        break;
	    }
	    
	    value=pio->value[ne];
	    k=(long)(pio->level[ne]);
	    
        xc=pio->xcenter[ne];
	    dx2=pio->dx2[k];
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
	    
        yc=pio->ycenter[ne];
	    dy2=pio->dy2[k];
	    ymin=yc-dy2*1.0000001;
	    ymax=yc+dy2*1.0000001;
 	    
        zc=pio->zcenter[ne];
	    dz2=pio->dz2[k];
	    zmin=zc-dz2*1.0000001;
	    zmax=zc+dz2*1.0000001;
	  	
	  	
	  	s=-1;
	    if(rx){
	        s1=((xc-dx2)-x1)/rx;
	        if(s1 > s){
		        yy=y1+s1*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            zz=z1+s1*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }	        
	        }
	        s2=((xc+dx2)-x1)/rx;
	        if(s2 > s){	        
	        	yy=y1+s2*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            zz=z1+s2*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
	        }
	    }
	        
	    if(ry){
	        s1=((yc-dy2)-y1)/ry;
            if(s1 > s){        
            	xx=x1+s1*rx;	        
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            zz=z1+s1*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }
	        }
	        s2=((yc+dy2)-y1)/ry;
            if(s2 > s){
                xx=x1+s2*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            zz=z1+s2*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
            }
	    }
	    
	    if(rz){
	        s1=((zc-dz2)-z1)/rz;
            if(s1 > s){        
            	xx=x1+s1*rx;	        
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            yy=y1+s1*ry;
			        if(yy < ymin || yy > ymax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }
	        }
	        s2=((zc+dz2)-z1)/rz;
            if(s2 > s){
                xx=x1+s2*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            yy=y1+s2*ry;
			        if(yy < ymin || yy > ymax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
            }
	    }
	    
	    
	    if(s < 0){
	    	if(dx == 0 && dy == 0 && dz == 0)goto NextCell;
	        sprintf(WarningBuff,"doIntegrateLine : Error Cell %ld No length Current Frame %ld\n",ne,li->CurrentFrame);
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
 	        	sprintf(WarningBuff,"x1 %g y1 %g z1 %g x2 %g y2 %g z2 %g\n",
 	        	                    li->x1,li->y1,li->z1,li->x2,li->y2,li->z2);
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
	    zo=z1+s*rz;	
		
		dl=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y)+(zo-z)*(zo-z));
		
		integral += value*dl;
		
		length += dl;
    
	    xD[nd]=length;
	    yD[nd++]=value;
	    
	    if(ibreak){
	    /*
		    sprintf(WarningBuff,"Last %g %g %g %g %g %g  %g %g\n",xo,yo,zo,x2,y2,z2,llength,length);
		    WarningBatch(WarningBuff);
		*/
	        break;
	    }
	    x=xo;
	    y=yo;
	    z=zo;
NextCell:
	    dx=1e-5*rx;
	    dy=1e-5*ry;
	    dz=1e-5*rz;
	}
	
	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	li->integral=integral;
	
	if(length > 0){
		if(fabs(length-llength)/length > 1e-5){
		     sprintf(WarningBuff,"doIntegrateLine Line Length %g Not Equal To Integral Length %g\n",llength,length);
		     WarningBatch(WarningBuff);
		}
	}
	
	return 0;
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}
static int doSage3DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	int ix1,iy1,ix2,iy2;
	double x1,y1,z1,x2,y2,z2,dl,ds,dx,dz,dy,dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	
	if(!li)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;
	

	CurrentFrame=li->CurrentFrame;

	FilesSetFrame(CurrentFrame,Files);
	
	if(pio->BlockSize <= 0 && uPref.BlockSizeGlobal > 0){
	    pio->BlockSize=uPref.BlockSizeGlobal;
	}
	
	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return doSage3DGetLineDataBlocks(Files,li);	
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
	z1=li->z1;
	x2=li->x2;
	y2=li->y2;
	z2=li->z2;

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


		
	if(GetPIOData(Files,CurrentFrame))goto OutOfHere;
	

    setPioScales(pio);
	

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81875))){
	    WarningBatch("Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("Out of List Memory\n");
	    goto OutOfHere;
	}


	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	nd=0;
	for(n=0;n<ns;++n){
	    double value,x,y,z;

	    x=x1+(double)n*ds*(x2-x1);
	    y=y1+(double)n*ds*(y2-y1);
	    z=z1+(double)n*ds*(z2-z1);

	    if(!pioGetValue(pio,x,y,z,&value))continue;

	    dx=x-x1;
	    dy=y-y1;
	    dz=z-z1;
	    dp=sqrt(dx*dx+dy*dy+dz*dz);

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

static int CalculateBhangMeterMergeGroup(mThread *Threads)
{
	struct FilePIOInfo *pio;
	struct linedata li;
	struct DrawData *d;
	long n;
	int m;
	double **l351;
	double **l951;
	int numGroup;
	int ng;
	double sumValue;
	double pi;
	double nx,ny,nz,mag,costh;
	int *processed;
	
	
	sumValue=0;
	
	if(!Threads)goto ErrorOut;
	d=(struct DrawData *)Threads->data;
	if(!d)goto ErrorOut;
	
	numGroup=d->numGroup;
	pio=d->pio;
	l951=d->l951;
	l351=d->l351;
	li=*d->li;
	processed=d->processed;

	zerol((char *)processed,pio->numcell*sizeof(int));
		
	pi=4.0*atan(1.0);
	
	nx=li.nx;
	ny=li.ny;
	nz=li.nz;
	
	mag=sqrt(nx*nx+ny*ny+nz*nz);
	if(mag > 0.0){
		nx /= mag;
		ny /= mag;
		nz /= mag;
	}
	
	li.errors=0;
	
	for(n=Threads->smin;n<Threads->smax;++n){
		double value[2000];
		double dl,dx2,dy2,dz2;
		double xx,yy,zz,aratio,r2;
		double nxx,nyy,nzz;
		double nlx,nly,nlz;
		double dx,dy,dz;
		double cosl;
		int found;
		long ne;
		long k;
		
		/* if(processed[n])continue; */
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
		
		li.x1=pio->xcenter[n];
		li.y1=pio->ycenter[n];
		li.z1=pio->zcenter[n];

		
		if(doIntegrateLineBhangMeter2(pio,&li))continue;
		
		k=(int)(pio->level[n]);
		
	    dx2=pio->dx2[k];   /* these are 1/2 the cell width, height, and depth */
		
	    dy2=pio->dy2[k];
		
	    dz2=pio->dz2[k];
		
		xx=(li.x2-li.x1);
		yy=(li.y2-li.y1);
		zz=(li.z2-li.z1);
		
		r2=(xx*xx+yy*yy+zz*zz);
		
		if(r2 <= 0.0)continue;
		
		nxx=xx/sqrt(r2);
		nyy=yy/sqrt(r2);
		nzz=zz/sqrt(r2);
		
		costh= -(nx*nxx+ny*nyy+nz*nzz);
		
		if(costh < 0.0){
		   continue;
		}
		
		dl=li.xData[1]-li.xData[0];
		
		nxx *= dl;
		
		if(nxx < 0){
		      dx=nxx + dx2;
			  nlx = -1.0;
		}else{
		      dx=nxx - dx2;
			  nlx = 1.0;
		}
		
		nyy *= dl;
		
		if(nyy < 0){
		      dy=nyy + dy2;
			  nly = -1.0;
		}else{
		      dy=nyy - dy2;
			  nly = 1.0;
		}
		
		nzz *= dl;
		
		if(nzz < 0){
		      dz=nzz + dz2;
			  nlz = -1.0;
		}else{
		      dz=nzz - dz2;
			  nlz = 1.0;
		}
		
		if(fabs(dx) < fabs(dy)){
		   if(fabs(dx) < fabs(dz)){
		       cosl=  nxx*nlx/dl;       /* dx smallest */
		   }else{
		       cosl=  nzz*nlz/dl;       /* dz smallest */
		   }
		}else if(fabs(dz) < fabs(dy)){
			   cosl=  nzz*nlz/dl;       /* dz smallest */
		}else{
		       cosl=  nyy*nly/dl;       /* dy smallest */
		}
		
		if(cosl < 0.0){
		   cosl = -cosl;
		}
				
		aratio=cosl*costh*4*(dx2*dy2+dx2*dz2+dy2*dz2)/(3*pi*r2);
				
		ne=(long)(li.yData[0]);
				
		for(ng=0;ng<numGroup;++ng){
			value[ng]=(l951[ng][ne])*aratio;
		
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
				
				dz2=pio->dz2[k];
				
				xx=(li.x2-pio->xcenter[ne]);
				yy=(li.y2-pio->ycenter[ne]);
				zz=(li.z2-pio->zcenter[ne]);
			
				r2=(xx*xx+yy*yy+zz*zz);
		
				if(r2 <= 0.0)continue;
								
				nxx=xx/sqrt(r2);
				nyy=yy/sqrt(r2);
				nzz=zz/sqrt(r2);
				
				costh= -(nx*nxx+ny*nyy+nz*nzz);
				
				if(costh < 0.0){
				   continue;
				}
				
				aratio=cosl*costh*4*(dx2*dy2+dx2*dz2+dy2*dz2)/(3*pi*r2);
													
				value2=(c*a*pow(t[ne],4.0)/(4))*aratio;
				
				if(dl/lambda[ne] < 1e-7)
				{
					value2 *= dl/lambda[ne];
				}
				else
				{
					value2 *= (1.0-exp(-dl/lambda[ne]));
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
	Threads->value1=sumValue;
	Threads->done=TRUE;
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
	
	numGroup=0;
	
	processed=NULL;
	
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
	    WarningBatch("BhangMeterGetGroupData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns+1),81886))){
	    WarningBatch("BhangMeterGetGroupData Out of List Memory\n");
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
		
	    if(GetPIOData(Files,CurrentFrame))goto OutOfHere;
				
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
		
		d.numGroup=numGroup;
		d.pio=pio;
		d.l351=l351;
		d.l951=l951;
		d.li=li;
		d.processed=processed;
		
		valueSum=0.0;
	
		runThreads2(15,&d,pio->numcell,CalculateBhangMeterMergeGroup,GatherBhangMeterMerge);

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
		sprintf(WarningBuff,"BhangMeterGetGroupData Line lengthErrors %ld \n",lengthErrors);
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
	if(processed)cFree((char *)processed);
	
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
		
	    if(GetPIOData(Files,CurrentFrame))goto OutOfHere;
				
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
		
		d.pio=pio;
		d.lambda=lambda;
		d.li=li;
		d.t=t;
		d.processed=processed;
		
		valueSum=0.0;
	
		runThreads2(15,&d,pio->numcell,CalculateBhangMeterMerge,GatherBhangMeterMerge);

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
static int CalculateBhangMeterMerge(mThread *Threads)
{
	struct FilePIOInfo *pio;
	struct linedata li;
	struct DrawData *d;
	long n;
	int m;
	double *lambda;
	double *t;
	double sumValue;
	double pi;
	double a=137.20172;
	double c=2.99792458e10;
	double nx,ny,nz,mag,costh;
	int *processed;
	
	
	sumValue=0;
		
	if(!Threads)goto ErrorOut;
	d=(struct DrawData *)Threads->data;
	if(!d)goto ErrorOut;
	
	pio=d->pio;
	lambda=d->lambda;
	t=d->t;
	li=*d->li;
	
	li.xData=NULL;
	li.yData=NULL;
	
	processed=d->processed;
	
	zerol((char *)processed,pio->numcell*sizeof(int));
		
	pi=4.0*atan(1.0);
	
	nx=li.nx;
	ny=li.ny;
	nz=li.nz;
	
	mag=sqrt(nx*nx+ny*ny+nz*nz);
	if(mag > 0.0){
		nx /= mag;
		ny /= mag;
		nz /= mag;
	}
	
	li.errors=0;
	
	for(n=Threads->smin;n<Threads->smax;++n){
		double value;
		double dl,dx2,dy2,dz2;
		double xx,yy,zz,aratio,r2;
		double nxx,nyy,nzz;
		double cosl;
		long ne;
		long k;
		
		/* if(processed[n])continue; */
	    if(pio->daughter[n])continue;
		
		processed[n]=1;
		
	    if(t[n] <= 0.0)continue;
		
		li.x1=pio->xcenter[n];
		li.y1=pio->ycenter[n];
		li.z1=pio->zcenter[n];

		
		if(doIntegrateLineBhangMeter2(pio,&li))continue;
		
		k=(int)(pio->level[n]);
		
	    dx2=pio->dx2[k];   /* these are 1/2 the cell width, height, and depth */
		
	    dy2=pio->dy2[k];
		
	    dz2=pio->dz2[k];
		
		xx=(li.x2-li.x1);
		yy=(li.y2-li.y1);
		zz=(li.z2-li.z1);
		
		r2=(xx*xx+yy*yy+zz*zz);
		
		if(r2 <= 0.0)continue;
		
		nxx=xx/sqrt(r2);
		nyy=yy/sqrt(r2);
		nzz=zz/sqrt(r2);
		
		costh= -(nx*nxx+ny*nyy+nz*nzz);
		
		if(costh < 0.0){
		   continue;
		}
		
		dl=li.xData[1]-li.xData[0];
		
		if(fabs(nxx)>fabs(nyy)){       /* find largest dot product with each of the six sides */
		   if(fabs(nxx) > fabs(nzz)){
		      cosl = fabs(nxx);
		   }else{
		      cosl = fabs(nzz);
		   }
		}else{
		   if(fabs(nyy) > fabs(nzz)){
		      cosl = fabs(nyy);
		   }else{
		      cosl = fabs(nzz);
		   }
		}
		
		aratio=cosl*costh*4*(dx2*dy2+dx2*dz2+dy2*dz2)/(3*pi*r2);
		
		/* aratio=costh*8*2*(dx2*dy2+dx2*dz2+dy2*dz2)/(4*3*pi*r2); */
				
		ne=(long)(li.yData[0]);
				
		value=(c*a*pow(t[ne],4.0)/(4))*aratio;
	
		/* dl=dx2; */
		
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
				
				dz2=pio->dz2[k];
				
				xx=(li.x2-pio->xcenter[ne]);
				yy=(li.y2-pio->ycenter[ne]);
				zz=(li.z2-pio->zcenter[ne]);
			
				r2=(xx*xx+yy*yy+zz*zz);
		
				if(r2 <= 0.0)continue;
								
				nxx=xx/sqrt(r2);
				nyy=yy/sqrt(r2);
				nzz=zz/sqrt(r2);
				
				costh= -(nx*nxx+ny*nyy+nz*nzz);
				
				if(costh < 0.0){
				   continue;
				}
				
				aratio=cosl*costh*4*(dx2*dy2+dx2*dz2+dy2*dz2)/(3*pi*r2);
													
				value2=(c*a*pow(t[ne],4.0)/(4))*aratio;
				
				if(dl/lambda[ne] < 1e-7)
				{
					value2 *= dl/lambda[ne];
				}
				else
				{
					value2 *= (1.0-exp(-dl/lambda[ne]));
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
	Threads->value1=sumValue;
	Threads->done=TRUE;
	return 0;
}
static int doIntegrateLineBhangMeter2(struct FilePIOInfo *pio,struct linedata *li)
{
	double *xD,*yD,s,s1,s2;
	double x1,y1,z1,x2,y2,z2,x,y,z,xo,yo,zo,dx,dy,dz,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double rx,ry,rz;
	double llength;
	int ibreak;
	int iGet;
	int ret;
	
	if(!li)return 1;
	if(!pio)return 1;
	
	
	ret = 1;
	
	iGet = 1;
	
	li->dCount=0;
	
    xD=li->xData;
    yD=li->yData;
	
   	li->xData=NULL;
	li->yData=NULL;
	
	np=10000;
	
	if(!xD)
	{
		if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81878))){
			WarningBatch("doIntegrateLine Out of List Memory\n");
			goto OutOfHere;
		}
	}
	if(!yD)
	{
		if(!(yD=(double *)cMalloc((long)sizeof(double)*(np),81883))){
			WarningBatch("doIntegrateLine Out of List Memory\n");
			goto OutOfHere;
		}	
	}
	
	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	
	y1=li->y1;
	y2=li->y2;
	ry=y2-y1;
	
	z1=li->z1;
	z2=li->z2;
	rz=z2-z1;

	llength=sqrt(rx*rx+ry*ry+rz*rz);
	
	if(llength <= 0)goto OutOfHere;
	    
	x=x1;
	y=y1;
	z=z1;
	
	length=0;
	integral=0;

	dx=0;
	dy=0;
	dz=0;
	
	s=0;
	
	nd=0;
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;	
	
	while(1){
	    double value,dx2,dy2,dz2,xc,yc,zc,dl,yy,xx,zz;
	    long k;
		iGet=pioGetValueXX(pio,x+dx,y+dy,z+dz,&ne);
	    if(iGet){
			if(iGet == 2){
			    ret = 2;
				goto OutOfHere;
			}
	        break;
	    }
	    
	    value=pio->value[ne];
	    k=(long)(pio->level[ne]);
	    
        xc=pio->xcenter[ne];
	    dx2=pio->dx2[k];
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
	    
        yc=pio->ycenter[ne];
	    dy2=pio->dy2[k];
	    ymin=yc-dy2*1.0000001;
	    ymax=yc+dy2*1.0000001;
 	    
        zc=pio->zcenter[ne];
	    dz2=pio->dz2[k];
	    zmin=zc-dz2*1.0000001;
	    zmax=zc+dz2*1.0000001;
	  	
	  	
	  	s=-1;
	    if(rx){
	        s1=((xc-dx2)-x1)/rx;
	        if(s1 > s){
		        yy=y1+s1*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            zz=z1+s1*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }	        
	        }
	        s2=((xc+dx2)-x1)/rx;
	        if(s2 > s){	        
	        	yy=y1+s2*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            zz=z1+s2*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
	        }
	    }
	        
	    if(ry){
	        s1=((yc-dy2)-y1)/ry;
            if(s1 > s){        
            	xx=x1+s1*rx;	        
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            zz=z1+s1*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }
	        }
	        s2=((yc+dy2)-y1)/ry;
            if(s2 > s){
                xx=x1+s2*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            zz=z1+s2*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
            }
	    }
	    
	    if(rz){
	        s1=((zc-dz2)-z1)/rz;
            if(s1 > s){        
            	xx=x1+s1*rx;	        
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            yy=y1+s1*ry;
			        if(yy < ymin || yy > ymax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }
	        }
	        s2=((zc+dz2)-z1)/rz;
            if(s2 > s){
                xx=x1+s2*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            yy=y1+s2*ry;
			        if(yy < ymin || yy > ymax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
            }
	    }
	    
	    
	    if(s < 0){
	    	if(dx == 0 && dy == 0 && dz == 0)goto NextCell;
	        sprintf(WarningBuff,"doIntegrateLine : Error Cell %ld No length Current Frame %ld\n",ne,li->CurrentFrame);
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
 	        	sprintf(WarningBuff,"x1 %g y1 %g z1 %g x2 %g y2 %g z2 %g\n",
 	        	                    li->x1,li->y1,li->z1,li->x2,li->y2,li->z2);
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

	    if(length < rxmin)rxmin=length;
	    if(length > rxmax)rxmax=length;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;

	    xo=x1+s*rx;
	    yo=y1+s*ry;	
	    zo=z1+s*rz;	
		
		dl=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y)+(zo-z)*(zo-z));
		
		integral += value*dl;
		
		length += dl;
    
	    xD[nd]=length;
	    yD[nd++]=ne;
	    
	    if(ibreak){
	    /*
		    sprintf(WarningBuff,"Last %g %g %g %g %g %g  %g %g\n",xo,yo,zo,x2,y2,z2,llength,length);
		    WarningBatch(WarningBuff);
		*/
	        break;
	    }
	    x=xo;
	    y=yo;
	    z=zo;
NextCell:
	    dx=1e-5*rx;
	    dy=1e-5*ry;
	    dz=1e-5*rz;
	}
	
	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	li->integral=integral;
	
	if(length > 0){
		if(fabs(length-llength)/length > 1e-5){
		     if(li->errors++ < 20){
		          sprintf(WarningBuff,"doIntegrateLine Line Length %g Not Equal To Integral Length %g zones %ld\n",llength,length,nd/2);
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
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt1,nt2;
	double valuet;
	double r,theta1,theta2,dr,dtheta;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	double x,y,z;
	double xs,ys,zs;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
    double rMax,thetaMax1,thetaMax2;

	
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

	if(ns < 2)goto OutOfHere;

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81872))){
	    WarningBatch("Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81882))){
	    WarningBatch("Out of List Memory\n");
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


	x=li->x1;
	y=li->y1;
	z=li->z1;
	
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
	zs=z;
	nd=0;
	rMax=0;
	thetaMax1=0;
	thetaMax2=0;
	
	
	nd=0;
	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    if(GetPIOData(Files,CurrentFrame))goto OutOfHere;
	    
    	setPioScales(pio);
		    
    	x=xs;
    	y=ys;
    	z=zs;
    	nf=0;
    	r=dr;
    	valuet=0;
    	for(nr=0;nr<NumberOfRings;++nr){
    	    theta1=0;
    	    for(nt1=0;nt1<NumberOfPoints;++nt1){
    	    	theta2=0;
    	    	for(nt2=0;nt2<NumberOfPoints;++nt2){
			        if(!pioGetValue(pio,x,y,z,&value))continue;
			        /*printf("FoundIt %ld Zone %ld\n",(long)n,(long)pioGetValue_ne);*/
			    	valuet += value;
			    	++nf;
			    	
			    	x=xs+r*cos(theta1)*sin(theta2);
			    	y=ys+r*sin(theta1)*sin(theta2);
			    	z=zs+r*cos(theta1);
			    	/*
			    	sprintf(WarningBuff,"x %g y %g z %g r %g theta1 %g theta2 %g",x,y,z,r,theta1*180/3.1415925,theta2*180/3.1415925);
			    	WarningBatchFast(WarningBuff);
			    	*/
			    	theta2 += 0.5*dtheta;
			    	if(theta2 > thetaMax2)thetaMax2=theta2;
			    	
	    	    }
			    if(theta1 > thetaMax1)thetaMax1=theta1;
			    theta1 += dtheta;
	    	}
			if(r > rMax)rMax=r;
	    	r += dr;
	    }
	    
	    if(nf <= 0){
			/* printf("Missed It %ld\n",(long)n); */
		    continue;
		}
	    
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
static int pioGetValueXX(struct FilePIOInfo *pio,double x,double y,double z,long *nn)
{
	struct PIO *sage;
	long kk;
	long i,j,k;
	long loc;
	long ne;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	long ixmax,ixymax;

	if(!pio || !nn || !pio->value)return 1;
	if(!pio->location || !pio->daughter)return 1;
	sage=pio->sage;
	if(!sage)return 1;
	
	
	xmin=pio->xmin;
	if(x < xmin || x >pio->xmax)return 1;
	
	ymin=pio->ymin;
	if(y < ymin || y >pio->ymax)return 1;
	
	zmin=pio->zmin;
	if(z < zmin || z >pio->zmax)return 1;
	
	
	dxset=sage->dxset;
	odxset=1.0/dxset;
	
	i=(long)((x-xmin)*odxset);
	if(i < 0){
	    i=0;
	}else if(i >= sage->ixmax){
	    i=sage->ixmax-1;
	}
	
	dyset=sage->dyset;
	odyset=1.0/dyset;
	
	j=(long)((y-ymin)*odyset);
	if(j < 0){
	    j=0;
	}else if(j >= sage->iymax){
	    j=sage->iymax-1;
	}
	
	dzset=sage->dzset;
	odzset=1.0/dzset;
	
	k=(long)((z-zmin)*odzset);
	if(k < 0){
	    k=0;
	}else if(k >= sage->izmax){
	    k=sage->izmax-1;
	}

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;
	
    loc=i+j*ixmax+k*ixymax;
    if(loc < 0 || loc >= ixymax*sage->izmax){
		return 1;
    }
    
    ne=pio->location[loc];
    
Start:
    if(ne < 0 || ne >= pio->numcell){
        return 1;
    }
    if(!pio->daughter[ne]){
        *nn=ne;
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
	if(z > pio->zcenter[ne])kk += 4; 
	ne=kk+(long)(pio->daughter[ne]-1);
	    
	goto Start;
}

static int pioGetValueX(struct FilePIOInfo *pio,double x,double y,double z,long *nn)
{
	struct PIO *sage;
	long kk;
	long i,j,k;
	long loc;
	long ne;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	long ixmax,ixymax;

	if(!pio || !nn || !pio->value)return 0;
	if(!pio->location || !pio->daughter)return 0;
	sage=pio->sage;
	if(!sage)return 0;
	
	
	xmin=pio->xmin;
	if(x < xmin || x >pio->xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin || y >pio->ymax)return 0;
	
	zmin=pio->zmin;
	if(z < zmin || z >pio->zmax)return 0;
	
	
	dxset=sage->dxset;
	odxset=1.0/dxset;
	
	i=(long)((x-xmin)*odxset);
	if(i < 0){
	    i=0;
	}else if(i >= sage->ixmax){
	    i=sage->ixmax-1;
	}
	
	dyset=sage->dyset;
	odyset=1.0/dyset;
	
	j=(long)((y-ymin)*odyset);
	if(j < 0){
	    j=0;
	}else if(j >= sage->iymax){
	    j=sage->iymax-1;
	}
	
	dzset=sage->dzset;
	odzset=1.0/dzset;
	
	k=(long)((z-zmin)*odzset);
	if(k < 0){
	    k=0;
	}else if(k >= sage->izmax){
	    k=sage->izmax-1;
	}

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;
	
    loc=i+j*ixmax+k*ixymax;
    if(loc < 0 || loc >= ixymax*sage->izmax){
		return 0;
    }
    
    ne=pio->location[loc];
    
Start:
    if(ne < 0 || ne >= pio->numcell){
        return 0;
    }
    if(!pio->daughter[ne]){
        *nn=ne;
        if(pio->active){
            return (int)pio->active[ne];
        }else{
			return 1;
		}
    }
    
	kk=0;
	if(x > pio->xcenter[ne])kk += 1; 
	if(y > pio->ycenter[ne])kk += 2; 
	if(z > pio->zcenter[ne])kk += 4; 
	ne=kk+(long)(pio->daughter[ne]-1);
	    
	goto Start;
}


static int pioGetValueN(struct FilePIOInfo *pio,double x,double y,double z,long *nn)
{
	double xc,yc,zc,dx2,dy2,dz2;
	struct PIO *sage;
	long kk,ncell;
	long i,j,k;
	long loc,n;
	long ne;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	long ixmax,ixymax;

	if(!pio || !nn || !pio->value)return 0;
	if(!pio->location || !pio->daughter)return 0;
	sage=pio->sage;
	if(!sage)return 0;
	
	
	xmin=pio->xmin;
	if(x < xmin || x >pio->xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin || y >pio->ymax)return 0;
	
	zmin=pio->zmin;
	if(z < zmin || z >pio->zmax)return 0;
	
	
	dxset=sage->dxset;
	odxset=1.0/dxset;
	
	i=(long)((x-xmin)*odxset);
	if(i < 0){
	    i=0;
	}else if(i >= sage->ixmax){
	    i=sage->ixmax-1;
	}
	
	dyset=sage->dyset;
	odyset=1.0/dyset;
	
	j=(long)((y-ymin)*odyset);
	if(j < 0){
	    j=0;
	}else if(j >= sage->iymax){
	    j=sage->iymax-1;
	}
	
	dzset=sage->dzset;
	odzset=1.0/dzset;
	
	k=(long)((z-zmin)*odzset);
	if(k < 0){
	    k=0;
	}else if(k >= sage->izmax){
	    k=sage->izmax-1;
	}

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;
	
    loc=i+j*ixmax+k*ixymax;
    if(loc < 0 || loc >= ixymax*sage->izmax){
		return 0;
    }
    
    ne=pio->location[loc];
    
Start:
    if(ne < 0 || ne >= pio->numcell){
        return 0;
    }
    if(!pio->daughter[ne]){
        *nn=ne;
        if(pio->active){
            return (int)pio->active[ne];
        }else{
			return 1;
		}
    }
    

	ncell = -1;
	for(kk=0;kk<8;++kk){
        n=kk+(long)(pio->daughter[ne]-1);
	
	    k=(long)(pio->level[n]);
    
        xc=pio->xcenter[n];
        dx2=pio->dx2[k]*1.0000001;
	    if(x < (xc-dx2) || x > (xc+dx2))continue;
	        
        yc=pio->ycenter[n];
        dy2=pio->dy2[k]*1.0000001;
	    if(y < (yc-dy2) || y > (yc+dy2))continue;
	        
        zc=pio->zcenter[n];
        dz2=pio->dz2[k]*1.0000001;
	    if(z < (zc-dz2) || z > (zc+dz2))continue;
	        
		ncell = n;
		
		break;
	        
	}
	
	if(ncell == -1){
	    return 0;
	}
	ne=ncell;
	
	goto Start;
}

static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double z,double *v)
{
	double xc,yc,zc,dx2,dy2,dz2;
	struct PIO *sage;
	long kk,ncell;
	long i,j,k;
	long loc,n;
	long ne;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	long ixmax,ixymax;

	if(!pio || !v || !pio->value)return 0;
	if(!pio->location || !pio->daughter)return 0;
	sage=pio->sage;
	if(!sage)return 0;
	
	
	xmin=pio->xmin;
	if(x < xmin || x >pio->xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin || y >pio->ymax)return 0;
	
	zmin=pio->zmin;
	if(z < zmin || z >pio->zmax)return 0;
	
	
	dxset=sage->dxset;
	odxset=1.0/dxset;
	
	i=(long)((x-xmin)*odxset);
	if(i < 0){
	    i=0;
	}else if(i >= sage->ixmax){
	    i=sage->ixmax-1;
	}
	
	dyset=sage->dyset;
	odyset=1.0/dyset;
	
	j=(long)((y-ymin)*odyset);
	if(j < 0){
	    j=0;
	}else if(j >= sage->iymax){
	    j=sage->iymax-1;
	}
	
	dzset=sage->dzset;
	odzset=1.0/dzset;
	
	k=(long)((z-zmin)*odzset);
	if(k < 0){
	    k=0;
	}else if(k >= sage->izmax){
	    k=sage->izmax-1;
	}

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;
	
    loc=i+j*ixmax+k*ixymax;
    if(loc < 0 || loc >= ixymax*sage->izmax){
		return 0;
    }
    
    ne=pio->location[loc];
    
Start:
    if(ne < 0 || ne >= pio->numcell){
        return 0;
    }
    if(!pio->daughter[ne]){
    	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
        	xc=pio->xcenter[ne];
        	yc=pio->ycenter[ne];
        	zc=pio->zcenter[ne];
			*v=pio->value[ne]+pio->gradx[ne]*(x-xc)
							 +pio->grady[ne]*(y-yc)
							 +pio->gradz[ne]*(z-zc);
		}else{
			*v=pio->value[ne];
			pioGetValue_ne=ne;
		}
		return 1;
    }
    

	ncell = -1;
	for(kk=0;kk<8;++kk){
        n=kk+(long)(pio->daughter[ne]-1);
	
	    k=(long)(pio->level[n]);
    
        xc=pio->xcenter[n];
        dx2=pio->dx2[k]*1.0000001;
	    if(x < (xc-dx2) || x > (xc+dx2))continue;
	        
        yc=pio->ycenter[n];
        dy2=pio->dy2[k]*1.0000001;
	    if(y < (yc-dy2) || y > (yc+dy2))continue;
	        
        zc=pio->zcenter[n];
        dz2=pio->dz2[k]*1.0000001;
	    if(z < (zc-dz2) || z > (zc+dz2))continue;
	        
		ncell = n;
		
		break;
	        
	}
	
	if(ncell == -1){
	    return 0;
	}
	ne=ncell;
	
	goto Start;
}


static int doCell(long ne,struct DrawInfo3 *info);

static int doCell(long ne,struct DrawInfo3 *info)
{
    struct FilePIOInfo *pio;
	double r1,r2,s1,s2,t1,t2,dx,dy,dz,dr,ds;
	double xc,yc,zc,dx2,dy2,dz2;
	double x,y,z,r,s,t,rx,ry,rz,sx,sy,sz,tx,ty,tz;
	long k,n,kk,nx,ny;
	long i1,i2,j1,j2;
	long i,j;
	double *sout;
	
	if(!info)return 1;
	pio=info->pio;
	if(!pio)return 1;
    if(ne < 0 || ne >= pio->numcell)return 1;
	
	rx=info->rx;
	ry=info->ry;
	rz=info->rz;
	sx=info->sx;
	sy=info->sy;
	sz=info->sz;
	tx=info->tx;
	ty=info->ty;
	tz=info->tz;
	
    k=(long)(pio->level[ne]);
	
	xc=pio->xcenter[ne];
    dx2=pio->dx2[k];
    dx=dx2*2.0;
    x=xc-dx2-info->p1x;
        	
	yc=pio->ycenter[ne];
    dy2=pio->dy2[k];
    dy=dy2*2.0;
    y=yc-dy2-info->p1y;
    
	zc=pio->zcenter[ne];
    dz2=pio->dz2[k];
    dz=dz2*2.0;
    z=zc-dz2-info->p1z;
    
    r1 = 10;
    r2 = -1;
    
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    x += dx;
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    y += dy;
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    x -= dx;
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;

    y -= dy;
    z += dz;
    
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    x += dx;
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    y += dy;
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    x -= dx;
    r=rx*x+ry*y+rz*z;                
    if(r < r1)r1 = r;
    if(r > r2)r2 = r;
    
    if(r1 > 1.0 || r2 < 0.0)return 0;
    
    s1 = 10;
    s2 = -1;
    
    y -= dy;
    z -= dz;
    
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    x += dx;
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    y += dy;
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    x -= dx;
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    y -= dy;
    z += dz;
    
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    x += dx;
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    y += dy;
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    x -= dx;
    s=sx*x+sy*y+sz*z;                
    if(s < s1)s1 = s;
    if(s > s2)s2 = s;
    
    if(s1 > 1.0 || s2 < 0.0)return 0;
    

    t1 = 10;
    t2 = -1;
    
    y -= dy;
    z -= dz;

    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    x += dx;
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    y += dy;
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    x -= dx;
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    y -= dy;
    z += dz;
    
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    x += dx;
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    y += dy;
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    x -= dx;
    t=tx*x+ty*y+tz*z;                
    if(t < t1)t1 = t;
    if(t > t2)t2 = t;
    
    if(t1 > 0.0 || t2 < 0.0)return 0;
    
    
    if(pio->daughter[ne]){   
		for(kk=0;kk<8;++kk){		
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
	
	i1=(long)(r1*(double)nx);
	if(i1 < 0)i1 = 0;
	if(i1 >= nx)i1 = nx - 1;

	i2=(long)(r2*(double)nx);
	if(i2 < 0)i2 = 0;
	if(i2 >= nx)i2 = nx - 1;


	j1=(long)(s1*(double)ny);
	if(j1 < 0)j1 = 0;
	if(j1 >= ny)j1 = ny - 1;

	j2=(long)(s2*(double)ny);
	if(j2 < 0)j2 = 0;
	if(j2 >= ny)j2 = ny - 1;
	
	for(j=j1;j<=j2;++j){
	    double v;
	    s=((double)j+0.5)*ds;
	    for(i=i1;i<=i2;++i){
	        r=((double)i+0.5)*dr;
	        x=r*info->v1x+s*info->v2x+info->p1x;
	        if(x < xc-dx2 || x > xc+dx2)continue;
	        y=r*info->v1y+s*info->v2y+info->p1y;
	        if(y < yc-dy2 || y > yc+dy2)continue;
	        z=r*info->v1z+s*info->v2z+info->p1z;
	        if(z < zc-dz2 || z > zc+dz2)continue;
	    	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
				v=pio->value[ne]+pio->gradx[ne]*(x-xc)+pio->grady[ne]*(y-yc)+pio->gradz[ne]*(z-zc);
			}else{
				v=pio->value[ne];
			}
			sout[i+(ny-1-j)*nx]=v;
	    }
	}


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
	

	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage3DSetCellData - Only Handles LINEDATA_CELL_DATA\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

    if(!sd->pioName[0]){
    	sprintf(WarningBuff,"doSage3DSetCellData - Error No Data Name\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }
    
    
    if(!sd->data){
    	sprintf(WarningBuff,"doSage3DSetCellData - Error No Data To Write\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }


	if(SageSetFileItem(Files,sd->pioName,sd->pioIndex,(void *)sd->data,sd->xsize,CurrentFrame)){
	    ret=17;goto ErrorOut;
	}
	        			
    
    ret=0;
    
ErrorOut:
	
	return ret;


}

static int doSage3DGetGeneratDataOffSet(struct areadata *ai,struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct DrawInfo3 info;	
	struct PIO *sage;
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
	sage=pio->sage;
	if(!sage)return 1;
		
	info.v1x=ai->x2-ai->x1;
	info.v1y=ai->y2-ai->y1;
	info.v1z=ai->z2-ai->z1;
	
	info.l1=sqrt(info.v1x*info.v1x+info.v1y*info.v1y+info.v1z*info.v1z);
	
	info.v2x=ai->x3-ai->x1;
	info.v2y=ai->y3-ai->y1;
	info.v2z=ai->z3-ai->z1;
	
	info.l2=sqrt(info.v2x*info.v2x+info.v2y*info.v2y+info.v2z*info.v2z);
	
	info.v3x =  (info.v1y*info.v2z-info.v2y*info.v1z);
	info.v3y = -(info.v1x*info.v2z-info.v2x*info.v1z);
	info.v3z =  (info.v1x*info.v2y-info.v2x*info.v1y);
	
	info.l3=sqrt(info.v3x*info.v3x+info.v3y*info.v3y+info.v3z*info.v3z);
	
	if(info.l1 <= 0 || info.l2 <= 0 || info.l3 <= 0.0){
	    sprintf(WarningBuff,"doSage3DGetGeneratDataOffSet - Error |v1| %g |v2| %g |v2| %g\n",info.l1,info.l2,info.l3);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	info.v3x /= info.l3;
	info.v3y /= info.l3;
	info.v3z /= info.l3;

	info.v3x *= ai->gloc;
	info.v3y *= ai->gloc;
	info.v3z *= ai->gloc;
	
	ai->x1 += info.v3x;
	ai->y1 += info.v3y;
	ai->z1 += info.v3z;
	
	ai->x2 += info.v3x;
	ai->y2 += info.v3y;
	ai->z2 += info.v3z;
	
	ai->x3 += info.v3x;
	ai->y3 += info.v3y;
	ai->z3 += info.v3z;
	
	return doSage3DGetGeneratData(ai,Files);
}
static int doSage3DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct DrawInfo3 info;
	struct PIO *sage;
	double *data;
	
	long nmax,n,ne;
	
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
	sage=pio->sage;
	if(!sage || !pio->value || !pio->location || !pio->daughter)return 1;
	
	
	
	data=(double *)cMalloc(ai->xsize*ai->ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,ai->xsize*ai->ysize*sizeof(double));
	
	ai->data=data;

	
	zerol((char *)&info,sizeof(struct DrawInfo3));

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
	info.p1z=ai->z1;
	
	info.v1x=ai->x2-ai->x1;
	info.v1y=ai->y2-ai->y1;
	info.v1z=ai->z2-ai->z1;
	
	info.l1=sqrt(info.v1x*info.v1x+info.v1y*info.v1y+info.v1z*info.v1z);
	
	info.v2x=ai->x3-ai->x1;
	info.v2y=ai->y3-ai->y1;
	info.v2z=ai->z3-ai->z1;
	
	info.l2=sqrt(info.v2x*info.v2x+info.v2y*info.v2y+info.v2z*info.v2z);
	
	info.v3x =  (info.v1y*info.v2z-info.v2y*info.v1z);
	info.v3y = -(info.v1x*info.v2z-info.v2x*info.v1z);
	info.v3z =  (info.v1x*info.v2y-info.v2x*info.v1y);
	
	info.d=info.v1x*(info.v2y*info.v3z-info.v2z*info.v3y)-
	       info.v2x*(info.v1y*info.v3z-info.v1z*info.v3y)+
	       info.v3x*(info.v1y*info.v2z-info.v1z*info.v2y);
	
	if(info.l1 <= 0 || info.l2 <= 0 || info.d == 0.0){
	    sprintf(WarningBuff,"doSage3DGetGeneratData - Error l1 %g l2 %g d %g\n",info.l1,info.l2,info.d);
	    WarningBatch(WarningBuff);
	    return 1;
	}

	info.od = 1.0/info.d;
	
	info.rx =  (info.v2y*info.v3z-info.v2z*info.v3y)*info.od;
	info.ry =  (info.v3x*info.v2z-info.v2x*info.v3z)*info.od;
	info.rz =  (info.v3y*info.v2x-info.v3x*info.v2y)*info.od;
	
	info.sx =  (info.v1z*info.v3y-info.v1y*info.v3z)*info.od;
	info.sy =  (info.v3z*info.v1x-info.v3x*info.v1z)*info.od;
	info.sz =  (info.v3x*info.v1y-info.v1x*info.v3y)*info.od;
	
	info.tx =  (info.v1y*info.v2z-info.v1z*info.v2y)*info.od;
	info.ty =  (info.v2x*info.v1z-info.v1x*info.v2z)*info.od;
	info.tz =  (info.v1x*info.v2y-info.v2x*info.v1y)*info.od;
	
	nmax=sage->ixmax*sage->iymax*sage->izmax;
		
	for(n=0;n<nmax;++n){	
    	ne=pio->location[n];    	
    	if(doCell(ne,&info))break;
    }	
    
	return  0;
}
static int doSage3DGetGridData(struct areadata *ai,struct FileInfo2 *Files)
{
	static double scale[]={
							-1,-1,-1, 1,-1,-1, 1,1,-1, -1,1,-1, /*bottom*/
							-1,-1,1,  1,-1,1,  1,1,1,  -1,1,1, /*top*/
							-1,-1,-1, 1,-1,-1, 1,-1,1, -1,-1,1, /*front*/
							-1,1,-1,  1,1,-1,  1,1,1,  -1,1,1, /*back*/
							-1,-1,-1, -1,1,-1, -1,1,1, -1,-1,1, /*left*/
							1,-1,-1,  1,1,-1,  1,1,1,  1,-1,1, /*right*/
					      };
					      
    double xx[4],yy[4],zz[4];
    struct FilePIOInfo *pio;
	double *x,*y,*z,*v;
	double dx,dy,dz;
	
	int k,ZoneLevel,m,i;
	long n,ne;
	
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
	if(!pio->value || !pio->daughter || !pio->level)return 1;

	ZoneLevel=ai->pa.ZoneLevel;
	if(ai->pa.DrawZones < 2)ZoneLevel=99999;

	ne=0;	
	for(n=0;n<pio->numcell;++n){
    	k=(int)(pio->level[n]);
		if(k > ZoneLevel)continue;	
        if(pio->daughter[n] && k != ZoneLevel)continue;
        dx=pio->dx2[k];
        dy=pio->dy2[k];
        dz=pio->dz2[k];
        for(m=0;m<6;++m){
            xx[0]=pio->xcenter[n]+dx*scale[12*m];
            xx[1]=pio->xcenter[n]+dx*scale[12*m+3];
            xx[2]=pio->xcenter[n]+dx*scale[12*m+6];
            xx[3]=pio->xcenter[n]+dx*scale[12*m+9];
            
            yy[0]=pio->ycenter[n]+dy*scale[12*m+1];
            yy[1]=pio->ycenter[n]+dy*scale[12*m+4];
            yy[2]=pio->ycenter[n]+dy*scale[12*m+7];
            yy[3]=pio->ycenter[n]+dy*scale[12*m+10];
            
            zz[0]=pio->zcenter[n]+dz*scale[12*m+2];
            zz[1]=pio->zcenter[n]+dz*scale[12*m+5];
            zz[2]=pio->zcenter[n]+dz*scale[12*m+8];
            zz[3]=pio->zcenter[n]+dz*scale[12*m+11];
            
            for(i=0;i<4;++i){
         		if(xx[i] < ai->xminArea || xx[i] > ai->xmaxArea)goto Done1;
         		if(yy[i] < ai->yminArea || yy[i] > ai->ymaxArea)goto Done1;
         		if(zz[i] < ai->zminArea || zz[i] > ai->zmaxArea)goto Done1;
			}
			++ne;	
Done1:		;
        }
	}
	
	if(ne <= 0)return 0;
	
	
	x=(double *)cMalloc(ne*4*sizeof(double),5921);
	y=(double *)cMalloc(ne*4*sizeof(double),5922);
	z=(double *)cMalloc(ne*4*sizeof(double),5923);
	v=(double *)cMalloc(ne*sizeof(double),5924);
	if(!x || !y || !z || !v)goto ErrorOut;
	
	
	ne=0;	
	for(n=0;n<pio->numcell;++n){
    	k=(int)(pio->level[n]);
		if(k > ZoneLevel)continue;	
        if(pio->daughter[n] && k != ZoneLevel)continue;
        dx=pio->dx2[k];
        dy=pio->dy2[k];
        dz=pio->dz2[k];
        for(m=0;m<6;++m){
            xx[0]=pio->xcenter[n]+dx*scale[12*m];
            xx[1]=pio->xcenter[n]+dx*scale[12*m+3];
            xx[2]=pio->xcenter[n]+dx*scale[12*m+6];
            xx[3]=pio->xcenter[n]+dx*scale[12*m+9];
            
            yy[0]=pio->ycenter[n]+dy*scale[12*m+1];
            yy[1]=pio->ycenter[n]+dy*scale[12*m+4];
            yy[2]=pio->ycenter[n]+dy*scale[12*m+7];
            yy[3]=pio->ycenter[n]+dy*scale[12*m+10];
            
            zz[0]=pio->zcenter[n]+dz*scale[12*m+2];
            zz[1]=pio->zcenter[n]+dz*scale[12*m+5];
            zz[2]=pio->zcenter[n]+dz*scale[12*m+8];
            zz[3]=pio->zcenter[n]+dz*scale[12*m+11];
            
            for(i=0;i<4;++i){
         		if(xx[i] < ai->xminArea || xx[i] > ai->xmaxArea)goto Done2;
         		if(yy[i] < ai->yminArea || yy[i] > ai->ymaxArea)goto Done2;
         		if(zz[i] < ai->zminArea || zz[i] > ai->zmaxArea)goto Done2;
			}
			
            for(i=0;i<4;++i){
                x[ne*4+i]=xx[i];
                y[ne*4+i]=yy[i];
                z[ne*4+i]=zz[i];
			}
            v[ne]=pio->value[n];
			++ne;	
Done2:		;
        }
	}
	
	if(ai->uDraw && ai->info){
	    if((*ai->uDraw)(ai->info,x,y,z,v,ne))goto ErrorOut;
		x=NULL;
		y=NULL;
		z=NULL;
		v=NULL;
	}
	
ErrorOut:
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	if(v)cFree((char *)v);
	return  0;
}
static int doSage3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	long xsize,ysize;
	double *data;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;
	
	CurrentFrame=ai->CurrentFrame;
	
	FilesSetFrame(CurrentFrame,Files);
	
	if(pio->pa.BlockSize <= 0 && uPref.BlockSizeGlobal > 0){
	    pio->pa.BlockSize=uPref.BlockSizeGlobal;
	}
	
	pio->BlockSize=pio->pa.BlockSize;
	    
	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return doSage3DGetAreaDataBlock(Files,ai);
	}
	

	if(ai->type != GENERAL_PLANE_DATA  && ai->type != AREADATA_AREA_DATA  &&
	   ai->type != GENERAL_OFFSET_DATA && ai->type != AREADATA_GRID_DATA){
	    sprintf(WarningBuff,"doSage3DGetAreaData - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	
	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}

	if(GetPIOData(Files,CurrentFrame))return 1;
		
	ai->pioTime=Files->pioTime;
	

	if(ai->type == GENERAL_PLANE_DATA){
	    return doSage3DGetGeneratData(ai,Files);
	}else if(ai->type == GENERAL_OFFSET_DATA){
	    return doSage3DGetGeneratDataOffSet(ai,Files);
	}else if(ai->type == AREADATA_GRID_DATA){
	    return doSage3DGetGridData(ai,Files);
	}
	
	
	if(ai->zsize <= 1){

		pio->plane.Plane_type=Z_PLANE;
		xsize=ai->xsize;
		ysize=ai->ysize;
		
		pio->range.xminData=ai->xminArea;
		pio->range.xmaxData=ai->xmaxArea;
		pio->range.yminData=ai->yminArea;
		pio->range.ymaxData=ai->ymaxArea;
		
		pio->plane.Plane_zloc=ai->zminArea;		
	
	}else if(ai->xsize <= 1){
	
		pio->plane.Plane_type=X_PLANE;
		
		ysize=ai->zsize;
		xsize=ai->ysize;
		
		pio->range.xminData=ai->yminArea;
		pio->range.xmaxData=ai->ymaxArea;
		pio->range.yminData=ai->zminArea;
		pio->range.ymaxData=ai->zmaxArea;
				
		pio->plane.Plane_xloc=ai->xminArea;
		
	}else if(ai->ysize <= 1){
	
		pio->plane.Plane_type=Y_PLANE;
		xsize=ai->xsize;
		ysize=ai->zsize;
		
		pio->range.xminData=ai->xminArea;
		pio->range.xmaxData=ai->xmaxArea;
		pio->range.yminData=ai->zminArea;
		pio->range.ymaxData=ai->zmaxArea;
				
		
		pio->plane.Plane_yloc=ai->yminArea;
		
	}else{
		pio->plane.Plane_xmin=ai->xminArea;
		pio->plane.Plane_xmax=ai->xmaxArea;

		pio->plane.Plane_ymin=ai->yminArea;
		pio->plane.Plane_ymax=ai->ymaxArea;

		pio->plane.Plane_zmin=ai->zminArea;
		pio->plane.Plane_zmax=ai->zmaxArea;

	    pio->plane.Plane_xcount=ai->xsize;
		
	    pio->plane.Plane_ycount=ai->ysize;
	    
	    pio->plane.Plane_zcount=ai->zsize;
	    
	    ai->data=buildSDS3D(Files);
	    
	    if(!ai->data){
	        WarningBatch("doSage3DGetAreaData Error in buildSDS3D\n");
	        return 1;
	    }else{
	        return 0;
	    }
	    
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
	    sprintf(WarningBuff,"doSage2DGetAreaData - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        return 1;
    }	    
		    
	return 0;
}

static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    int ret;

	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	limits=&Files->limits;
	ret=1;


	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;
		pio->plane  = sd->plane;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
		pio->pa     = sd->pa;		
	}
	
	
	if(GetPIOData(Files,CurrentFrame)){
		goto ErrorOut;
	}
	        		
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9206);
    if(!sd->data){
    	sprintf(WarningBuff,"FloatAndRaster - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }

    
    sd->data2=cMalloc(length*sizeof(double),9204);
    if(!sd->data2){
    	sprintf(WarningBuff,"FloatAndRaster - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }

	setFloat((double *)sd->data,length);
    if(getPlotData(Files,(double *)sd->data,CurrentFrame)){
	    sprintf(WarningBuff,"FloatAndRaster - getPlotData Error\n");
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

	return ret;

}
static int doSage3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    double *sout;
    int ret;

	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	limits=&Files->limits;
	ret=1;
	

	FilesSetFrame(CurrentFrame,Files);
	
	if(sd->pa.BlockSize <= 0 && uPref.BlockSizeGlobal > 0){
	    sd->pa.BlockSize=uPref.BlockSizeGlobal;
	}
	
	pio->BlockSize=sd->pa.BlockSize;
	
	if(pio->BlockSize > 0 && pio->BlockSize < pio->numcell){
		return doSage3DGetDataBlocks(Files,CurrentFrame,sd);	
	}
	

 	if(sd->type == FRAME_DATA_FLOAT_RASTER){
 	    return FloatAndRaster(Files,CurrentFrame,sd);
 	}


	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           ;
	    }else if(sd->type != FRAME_DATA_RASTER){
		    sprintf(WarningBuff,"doSage3DGetData - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
	    }else if(!pio->pa.DrawZones && !pio->vector.DrawVectors && !pio->active &&
	            (pio->LaserPlotCount < 1) && (pio->TracerPlotCount < 1) && !pio->streamline3d.streamOn){
		    goto ErrorOut;
	    }
	}

	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;
		pio->plane  = sd->plane;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
		pio->pa     = sd->pa;		
	}
	
	
	if(GetPIOData(Files,CurrentFrame)){
		goto ErrorOut;
	}
	        		
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9206);
    if(!sd->data){
    	sprintf(WarningBuff,"doSage3DGetData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }

    sout=(double *)sd->data;
    
    
    if(sd->type == FRAME_DATA_FLOAT){
		setFloat(sout,length);
	    if(getPlotData(Files,sout,CurrentFrame)){
		    sprintf(WarningBuff,"doSage3DGetData - getPlotData Error\n");
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
static int doSage3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
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
	

	if(sd->type == LINEDATA_STREAMLINE_DATA){
	   return  doSage3DGetStreamLine(Files,CurrentFrame,sd);
	}else if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSage3DGetCellData - Only Returns LINEDATA_CELL_DATA\n");
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
static int GetPIOData(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct PIO *sage;
	int active;
	int ret;
	long n;
	int k;
	double *fdata1,*fdata2,*fdata3,*fdata4,*fdata5,*fdata6,*fdata7;
	long dataLength1,dataLength2,dataLength3,dataLength4,dataLength5;
	double lmin,lmax,size,cmin,cmax;



	if(!Files)return 1;
	pio = &Files->pioData;
	if(Files->fileClass != PIO3D_FILES)return 1;
	if(!Files->PIO2DList)return 1;
	
	ret = 1;

	fdata1=NULL;
	fdata2=NULL;
	fdata3=NULL;
	fdata4=NULL;
	fdata5=NULL;
	fdata6=NULL;
	fdata7=NULL;

	lmin=cmin=     1e60;
	lmax=cmax=    -1e60;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	pio->sage = sage = &Files->PIO2DList[CurrentFrame];
	
	Files->limits.time=sage->time;
	Files->pioTime=sage->time;

	if((pio->CurrentFrame == CurrentFrame) && pio->xcenter && pio->ycenter && pio->zcenter &&
	      pio->level && pio->daughter){
	    
	    if(pio->value){
			if(!mstrcmp(pio->pioName,pio->pioNameOld) && (pio->pioIndex == pio->pioIndexOld)){						
				SageGetGradients(Files,pio->pa.flagGradients-1);
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
 	    
		pio->CurrentFrameGradients = -1;
	    SageGetGradients(Files,pio->pa.flagGradients-1);
	        
 	    return 0;
	}


	pio->CurrentFrame=CurrentFrame;

	if(pio->xcenter)cFree((char *)pio->xcenter);
	pio->xcenter=NULL;
	if(pio->ycenter)cFree((char *)pio->ycenter);
	pio->ycenter=NULL;
	if(pio->zcenter)cFree((char *)pio->zcenter);
	pio->zcenter=NULL;
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

	if(SageGetFileItem(Files,"cell_center",3,
	                  &fdata6,&dataLength2,CurrentFrame)){
		    ret=25;goto ErrorOut;
	}


	if(SageGetFileItem(Files,"cell_level",0,
	                  &fdata3,&dataLength3,CurrentFrame)){
		    ret=26;goto ErrorOut;
	}

	if(SageGetFileItem(Files,"cell_active",0,
	                  &fdata7,&dataLength3,CurrentFrame)){
		    ret=27;goto ErrorOut;
	}
	
	if(SageGetFileItem(Files,"cell_daughter",0,
	                  &fdata5,&dataLength5,CurrentFrame)){
		    ret=28;goto ErrorOut;
	}

    if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
                  &fdata4,&dataLength4,CurrentFrame)){
	    ret=29;goto ErrorOut;
    }


    for(n=0;n<pio->numcell;++n){
        if(fdata5[n] || (fdata7[n] <= 0.0))continue;
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
	    if(fdata7[n] <= 0.0)active=TRUE;
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
	    WarningBatchHold(WarningBuff);
	    
	    lmin=1;
	}


	if(lmin < 1 || lmax >= 100){
	    sprintf(WarningBuff,"Error level min %f lenvel max %f\n",lmin,lmax);
	    WarningBatchHold(WarningBuff);
	    goto ErrorOut;
	}

	for(k=1;k<=(int)lmax;++k){
	    pio->dx2[k]=.5*pio->dxset/pow(2,(double)(k-1));
	    pio->dy2[k]=.5*pio->dyset/pow(2,(double)(k-1));
	    pio->dz2[k]=.5*pio->dzset/pow(2,(double)(k-1));
	}

	pio->xmin=sage->xmin;
	pio->xmax=sage->xmax;

	pio->ymin=sage->ymin;
	pio->ymax=sage->ymax;

	pio->zmin=sage->zmin;
	pio->zmax=sage->zmax;

	pio->lmin=lmin;
	pio->lmax=lmax;


	pio->xcenter=fdata1;
	pio->ycenter=fdata2;
	pio->zcenter=fdata6;
	pio->level=fdata3;
	pio->daughter=fdata5;
	
	pio->CurrentFrameGradients = -1;
	
	SageGetGradients(Files,pio->pa.flagGradients-1);

	if(active){
		pio->active=fdata7;
	}else{
	    pio->active=NULL;
		if(fdata7)cFree((char *)fdata7);
	}
	
	ret = sageLocation(Files);
	
	WarningBatchHoldDump();
	
	return ret;
ErrorOut:
	if(fdata1)cFree((char *)fdata1);
	if(fdata2)cFree((char *)fdata2);
	if(fdata3)cFree((char *)fdata3);
	if(fdata4)cFree((char *)fdata4);
	if(fdata5)cFree((char *)fdata5);
	if(fdata6)cFree((char *)fdata6);
	if(fdata7)cFree((char *)fdata7);
    sprintf(WarningBuff,"SetPIOData Error Loading Item Name %s index %ld\n",pio->pioName,pio->pioIndex);
    WarningBatchHold(WarningBuff);
	WarningBatchHoldDump();
	return ret;
}
static int sageLocation(struct FileInfo2 *Files)
{
	struct PIO *sage;
    struct FilePIOInfo *pio;
	long *location;
	long count;
	long n;
	long loc,length;
	long ixmax,ixymax,ierror,nset;
	long i,j,k;
	int kk;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	double xc,yc,zc;


	if(!Files)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage)return 1;


	length=sage->ixmax*sage->iymax*sage->izmax;

	if(pio->location)cFree((char *)pio->location);
	location=(long *)cMalloc(sizeof(long)*length,19723);
	pio->location=location;
	if(!location)return 1;

	for(n=0;n<length;++n)location[n] = -1;

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;


	xmin=pio->xmin;
	ymin=pio->ymin;
	zmin=pio->zmin;

	dxset=sage->dxset;
	odxset=1.0/dxset;
	dyset=sage->dyset;
	odyset=1.0/dyset;
	dzset=sage->dzset;
	odzset=1.0/dzset;

	nset=0;
	ierror=0;
	count=0;

	for(n=0;n<pio->numcell;++n){
	    double ix,iy,iz;
	    double dx,dy,dz;
	    
	    kk=(int)(pio->level[n]);
	    
	    xc=pio->xcenter[n];
	    ix=((xc-xmin)*odxset);
	    i=(long)ix;
	    dx=ix-(double)i;
	    if(dx < 0.499999999 || dx > 0.500000001)continue;

	    yc=pio->ycenter[n];
	    iy=((yc-ymin)*odyset);
	    j=(long)iy;
	    dy=iy-(double)j;
	    if(dy < 0.499999999 || dy > 0.500000001)continue;

	    zc=pio->zcenter[n];
	    iz=((zc-zmin)*odzset);
	    k=(long)iz;
	    dz=iz-(double)k;
	    if(dz < 0.499999999 || dz > 0.500000001)continue;
	    ++count;
	    
	    loc=i+j*ixmax+k*ixymax;
	    if(loc < 0 || loc >= length){
	          if(++ierror < 10){
	              ;
	          }
	          continue;
	    }
	    if(kk != 1){
	    	pio->level[n]=1;	    
			++nset;
	    }
	    location[loc]=n;
	}



	for(n=0;n<length;++n){
	    if((location[n] < 0) ||  (location[n] >= pio->numcell))++ierror;
	}

	if(nset > 0){
	    sprintf(WarningBuff,"Location Errors %ld cell at Level 1 locations reset\n",nset);
	    WarningBatchHold(WarningBuff);
	}
	
	
	if(ierror || (count != length)){
	    sprintf(WarningBuff,"Location Errors ierror %ld count %ld length %ld",ierror,count,length);
	    WarningBatchHold(WarningBuff);
	    goto ErrorOut;
	}

	return 0;
ErrorOut:
	return 1;
}
/*
static int sageLocation(struct FileInfo2 *Files)
{
	struct PIO *sage;
    struct FilePIOInfo *pio;
	long *location;
	long count;
	long n;
	long loc,length;
	long ixmax,ixymax,ierror;
	long i,j,k;
	int kk;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	double xc,yc,zc;


	if(!Files)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage)return 1;


	length=sage->ixmax*sage->iymax*sage->izmax;

	if(pio->location)cFree((char *)pio->location);
	location=(long *)cMalloc(sizeof(long)*length,19723);
	pio->location=location;
	if(!location)return 1;

	for(n=0;n<length;++n)location[n] = -1;

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;


	xmin=pio->xmin;
	ymin=pio->ymin;
	zmin=pio->zmin;

	dxset=sage->dxset;
	odxset=1.0/dxset;
	dyset=sage->dyset;
	odyset=1.0/dyset;
	dzset=sage->dzset;
	odzset=1.0/dzset;

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

	    zc=pio->zcenter[n];
	    k=(long)((zc-zmin)*odzset);

	    loc=i+j*ixmax+k*ixymax;
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
	    sprintf(WarningBuff,"Location Errors ierror %ld count %ld length %ld\n",ierror,count,length);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	return 0;
ErrorOut:
	return 1;
}
*/

static int SageGetGradients(struct FileInfo2 *Files,int flag)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	double *gradl,*next,*gradh;
	long dataLength;
	long n,n2;
	int k,k2,ret;
	long ierror;

	if(!Files)return 1;
	pio = &Files->pioData;
	if(!pio)return 1;
	if(Files->fileClass != PIO3D_FILES)return 1;
	if(!Files->PIO2DList)return 1;
	
	pio->doGradients = FALSE;
		
	if(flag < 0){
		return 0;
	}
	
	ret = 1;

	CurrentFrame=pio->CurrentFrame;
	
	if(pio->gradx && pio->grady && pio->gradz &&
	   (CurrentFrame  == pio->CurrentFrameGradients)){
	    goto OK;
	}

	pio->CurrentFrameGradients = -1;
		
	gradh=NULL;
	gradl=NULL;
	next=NULL;

	gradh=(double *)cMalloc(sizeof(double)*pio->numcell,19725);
	if(!gradh)goto ErrorOut;

	gradl=(double *)cMalloc(sizeof(double)*pio->numcell,19726);
	if(!gradl)goto ErrorOut;

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

	pio->gradz=(double *)cMalloc(sizeof(double)*pio->numcell,19729);
	if(!pio->gradz)goto ErrorOut;

	for(n=0;n<pio->numcell;++n){
	    gradh[n]=0;
	    gradl[n]=0;
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
	        gradh[n] += (pio->value[n2]-pio->value[n])/(pio->dy2[k]+pio->dy2[k2]);
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

	if(SageGetFileItem(Files,"cell_index",5,
	                  &next,&dataLength,CurrentFrame)){
		ret=24;goto ErrorOut;
	}


	for(n=0;n<pio->numcell;++n){        /* lo_z */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dz2[k]+pio->dz2[k2]);
	    }else{
	        gradl[n] = 0;
	    }
	}



	if(next)cFree((char *)next);
	next=NULL;

	if(SageGetFileItem(Files,"cell_index",6,
	                  &next,&dataLength,CurrentFrame)){
		ret=25;goto ErrorOut;
	}


	for(n=0;n<pio->numcell;++n){        /* hi_z */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dz2[k]+pio->dz2[k2]);
	    }else{
	        gradh[n] = 0;
	    }
	}


	if(next)cFree((char *)next);
	next=NULL;

	for(n=0;n<pio->numcell;++n){
		if(!pio->daughter[n] && (gradh[n]*gradl[n] >= 0.0)){
			if(fabs(gradh[n]) < fabs(gradl[n])){
				pio->gradz[n] = gradh[n];
			}else{
				pio->gradz[n] = gradl[n];
			}
		}else{
			pio->gradz[n] = 0;
		}
	}

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
		
	pio->CurrentFrameGradients = CurrentFrame;
	
OK:

	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady &&  pio->gradz){
		pio->doGradients = TRUE;
	}    
	

	return 0;
ErrorOut:
	sprintf(WarningBuff,"sageGradients Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	if(gradh)cFree((char *)gradh);
	if(gradl)cFree((char *)gradl);
	
	if(pio->gradx)cFree((char *)pio->gradx);
	pio->gradx=NULL;

	if(pio->grady)cFree((char *)pio->grady);
	pio->grady=NULL;

	if(pio->gradz)cFree((char *)pio->gradz);
	pio->gradz=NULL;
	
	return ret;
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
		
	if(mstrcmp(pio->vector.vectorzName,pio->vectorzNameOld)){
	    if(pio->vectorz)cFree((char *)pio->vectorz);
	    pio->vectorz=NULL;
	}	
	
	if(!pio->vectorx || !pio->vectory || !pio->vectorz || (pio->CurrentFrameVectors != pio->CurrentFrame)){
	
	    if(pio->vectorx)cFree((char *)pio->vectorx);
	    pio->vectorx=NULL;
	    
	    if(pio->vectory)cFree((char *)pio->vectory);
	    pio->vectory=NULL;
	    
	    if(pio->vectorz)cFree((char *)pio->vectorz);
	    pio->vectorz=NULL;
	    
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
		
		if(SageGetFileItem(Files,pio->vector.vectorzName,0,
		                  &pio->vectorz,&length,pio->CurrentFrame)){
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
static int doSage3DGetStreamLine(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;
	double *lo_x,*hi_x,*lo_y,*hi_y,*lo_z,*hi_z;
	double *xd,*yd,*zd,*vd,*xs,*ys,*zs,*vs;
	double dxdi,dydj,dzdk,didx,djdy,dkdz,dv;
	double gradxV1,gradyV1,gradzV1;
	double gradxV2,gradyV2,gradzV2;
	double gradxV3,gradyV3,gradzV3;
	double gradl,gradh,vlen,sx,sy,sz;
	double x1,y1,z1,x,y,z,vx,vy,vz,vv,x2,y2,z2;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	long streamSteps,dataLength;
    int streamsReturned;
	int streamGradients;
	long n,k,ne,nn,nstep,n2,nGrad,nstepsave;
	int k2,ke;
    int ret;
    
	if(!Files)return 1;
	if(!sd || (sd->streamline3d.xyz.count <= 0))return 1;
	s = &Files->SetData;
	pio = &Files->pioData;

	ret=1;
	
	streamsReturned=0;
	xs=ys=zs=vs=NULL;
	xd=yd=zd=vd=NULL;
	hi_z=NULL;
	lo_z=NULL;
	hi_y=NULL;
	lo_y=NULL;
	hi_x=NULL;
	lo_x=NULL;
		
	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=(int)sd->pioIndex;
	}
		
	if(GetPIOData(Files,CurrentFrame))goto ErrorOut;
	
	
	if(loadVectors(Files))goto ErrorOut;
	
	if(pio->gradx && pio->grady && pio->gradz){
	   pio->doGradients=TRUE;
	}else{
	   pio->doGradients=FALSE;
	}
	
	xmin=sd->plane.Plane_xmin;
	xmax=sd->plane.Plane_xmax;
	
	ymin=sd->plane.Plane_ymin;
	ymax=sd->plane.Plane_ymax;
	
	zmin=sd->plane.Plane_zmin;
	zmax=sd->plane.Plane_zmax;
	
    didx=((double)(sd->plane.Plane_xcount))/(xmax-xmin);
    dxdi=1.0/didx;
    djdy=((double)(sd->plane.Plane_ycount))/(ymax-ymin);
    dydj=1.0/djdy;
    dkdz=((double)(sd->plane.Plane_zcount))/(zmax-zmin);
    dzdk=1.0/dkdz;
    
    dv=min(dxdi,dydj);
    dv=min(dv,dzdk);
    
    streamSteps=sd->streamline3d.streamSteps;
    if(streamSteps <= 0)streamSteps=2000;
    streamGradients=sd->streamline3d.streamGradients;
    xd=yd=zd=NULL;
	lo_x=hi_x=lo_y=hi_y=lo_z=hi_z=NULL;
    if(streamGradients){
		if(SageGetFileItem(Files,"cell_index",1,
		                  &lo_x,&dataLength,pio->CurrentFrame)){
			ret=21;goto ErrorOut;
		}
		if(SageGetFileItem(Files,"cell_index",2,
		                  &hi_x,&dataLength,pio->CurrentFrame)){
			ret=22;goto ErrorOut;
		}
		if(SageGetFileItem(Files,"cell_index",3,
		                  &lo_y,&dataLength,pio->CurrentFrame)){
			ret=23;goto ErrorOut;
		}
		if(SageGetFileItem(Files,"cell_index",4,
		                  &hi_y,&dataLength,pio->CurrentFrame)){
			ret=24;goto ErrorOut;
		}
		if(SageGetFileItem(Files,"cell_index",5,
		                  &lo_z,&dataLength,pio->CurrentFrame)){
			ret=25;goto ErrorOut;
		}
		if(SageGetFileItem(Files,"cell_index",6,
		                  &hi_z,&dataLength,pio->CurrentFrame)){
			ret=26;goto ErrorOut;
		}
    }
	
		sd->streamline3d.xyzReturned=(struct dataStruct *)cMalloc(sd->streamline3d.xyz.count*sizeof(struct dataStruct),8721);
        xd=(double *)cMalloc(2*streamSteps*sizeof(double),8721);
        yd=(double *)cMalloc(2*streamSteps*sizeof(double),8721);
        zd=(double *)cMalloc(2*streamSteps*sizeof(double),8721);
        vd=(double *)cMalloc(2*streamSteps*sizeof(double),8721);
        if(!xd || !yd || !zd || !vd || !sd->streamline3d.xyzReturned){
            WarningBatch("doSage3DGetStreamLine Out of Memory\n");
            goto ErrorOut;
        }
	
	
	    for(n=0;n<sd->streamline3d.xyz.count;++n){
		    nstepsave=-1;
	        for(k=0;k<2;++k){
		        x1=x=sd->streamline3d.xyz.x[n];
		        y1=y=sd->streamline3d.xyz.y[n];
		        z1=z=sd->streamline3d.xyz.z[n];
		        ne=-1;
		        nn=n;
		        nstep=0;
		        nGrad=-1;
	    		gradxV1=0;
	    		gradyV1=0;
	    		gradzV1=0;
	    		gradxV2=0;
	    		gradyV2=0;
	    		gradzV2=0;
	    		gradxV3=0;
	    		gradyV3=0;
	    		gradzV3=0;
doAgain1:
				if(x < xmin || x > xmax || y < ymin || y > ymax || z < zmin || z > zmax)continue;
				
		    	if(!pioGetValueN(pio,x,y,z,&ne))continue;
	    		if(streamGradients){
	    		    if(nGrad != ne){
			    		gradxV1=0;
			    		gradyV1=0;
			    		gradzV1=0;
			    		gradxV2=0;
			    		gradyV2=0;
			    		gradzV2=0;
			    		gradxV3=0;
			    		gradyV3=0;
			    		gradzV3=0;
			    		
			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorx[ne]-pio->vectorx[n2])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorx[n2]-pio->vectorx[ne])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
						gradxV1 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
						gradxV2 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
						gradxV3 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorx[ne]-pio->vectorx[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorx[n2]-pio->vectorx[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradyV1 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradyV2 = 0.5*(gradl+gradh);
                	
			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradyV3 = 0.5*(gradl+gradh);
                	

			    		n2=(long)(lo_z[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorx[ne]-pio->vectorx[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_z[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorx[n2]-pio->vectorx[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradzV1 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_z[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_z[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradzV2 = 0.5*(gradl+gradh);
                	
			    		n2=(long)(lo_z[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_z[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradzV3 = 0.5*(gradl+gradh);
                	
	    			}
			    		

	    		    nGrad=ne;
		    	    vx= pio->vectorx[ne]+gradxV1*(x-pio->xcenter[ne])+gradyV1*(y-pio->ycenter[ne])+gradzV1*(z-pio->zcenter[ne]);
		    	    vy= pio->vectory[ne]+gradxV2*(x-pio->xcenter[ne])+gradyV2*(y-pio->ycenter[ne])+gradzV2*(z-pio->zcenter[ne]);
		    	    vz= pio->vectorz[ne]+gradxV3*(x-pio->xcenter[ne])+gradyV3*(y-pio->ycenter[ne])+gradzV3*(z-pio->zcenter[ne]);
/*		    	    
		    	    sprintf(WarningBuff,"ne %ld n2 %ld %g %g %g %g",ne,n2,gradxV1,gradxV2,gradyV1,gradyV2);
		    	    WarningBatchFast(WarningBuff);
*/
	    		}else{
		    	    vx= pio->vectorx[ne];
		    	    vy= pio->vectory[ne];
		    	    vz= pio->vectorz[ne];
	    		}
	    		
		   		    	
		        if(pio->doGradients){
					vv=pio->value[ne]+(x-pio->xcenter[ne])*pio->gradx[n]+
					  		          (y-pio->ycenter[ne])*pio->grady[n]+ 
					  	              (z-pio->zcenter[ne])*pio->gradz[n];
		        }else{
		    	    vv= pio->value[ne];
		        }			        				        
				    	
		    	vlen=sqrt(vx*vx+vy*vy+vz*vz);
		    	
		    	if(vlen > 0){
		    		if(k == 0){
		    	    	sx=dv*vx/vlen;
		    			sy=dv*vy/vlen;
		    			sz=dv*vz/vlen;
		    		}else{
		    	    	sx = -dv*vx/vlen;
		    			sy = -dv*vy/vlen;
		    			sz = -dv*vz/vlen;
		    		}
		    		x2=x1+sx;
		    		y2=y1+sy;	    		
		    		z2=z1+sz;	    		
		    		if(k == 0){
		    		    xd[streamSteps+nstep]=x2;
		    		    yd[streamSteps+nstep]=y2;
		    		    zd[streamSteps+nstep]=z2;
		    		    vd[streamSteps+nstep]=vv;
		    		    nstepsave=nstep+1;
		    		}else{
		    		    xd[nstep]=x2;
		    		    yd[nstep]=y2;
		    		    zd[nstep]=z2;
		    		    vd[nstep]=vv;
		    		}
		    	x=x1=x2;
		    	y=y1=y2;
		    	z=z1=z2;
		    	if(++nstep >= streamSteps)continue;
		    	goto doAgain1;
			}
		}
		
		
		if(nstep > 0 || nstepsave > 0){
		    nn=0;
	        xs=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),8721);
	        ys=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),8721);
	        zs=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),8721);
	        vs=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),8721);
	        if(!xs || !ys ||!zs || !vs){
	            WarningBatch("doSage3DGetStreamLine Out of Memory\n");
	            goto ErrorOut;
	        }
	        if(nstep > 0){
	            for(k=0;k<nstep;++k){
	                xs[nn]=xd[nstep-1-k];
	                ys[nn]=yd[nstep-1-k];
	                zs[nn]=zd[nstep-1-k];
	                vs[nn]=vd[nstep-1-k];
	                ++nn;
	            }
	        }

	        
	        if(nstepsave > 0){
	            for(k=0;k<nstepsave;++k){
	                xs[nn]=xd[streamSteps+k];
	                ys[nn]=yd[streamSteps+k];
	                zs[nn]=zd[streamSteps+k];
	                vs[nn]=vd[streamSteps+k];
	                ++nn;
	            }
	        }
	        
	        
	       sd->streamline3d.xyzReturned[streamsReturned].count=nn;  
	       sd->streamline3d.xyzReturned[streamsReturned].x=xs;  
	       sd->streamline3d.xyzReturned[streamsReturned].y=ys;  
	       sd->streamline3d.xyzReturned[streamsReturned].z=zs;  
	       sd->streamline3d.xyzReturned[streamsReturned].v=vs;  
	       streamsReturned++;
	        
		}
	}
	
	
	ret=0;
ErrorOut:
	sd->streamline3d.streamsReturned=streamsReturned;
	if(xd)cFree((char *)xd);
	if(yd)cFree((char *)yd);
	if(zd)cFree((char *)zd);
	if(vd)cFree((char *)vd);
	if(lo_x)cFree((char *)lo_x);
	if(hi_x)cFree((char *)hi_x);
	if(lo_y)cFree((char *)lo_y);
	if(hi_y)cFree((char *)hi_y);
	if(lo_z)cFree((char *)lo_z);
	if(hi_z)cFree((char *)hi_z);
	
    return ret;
	
}
static int pioDrawStream3d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
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
/*
    	    sprintf(WarningBuff,"pioDrawStream3d In\n");
    	    WarningBatch(WarningBuff);
	*/
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
    
    streamSteps=pio->streamline3d.streamSteps;
    if(streamSteps <= 0)streamSteps=2000;
    streamGradients=pio->streamline3d.streamGradients;
	
	if(pio->plane.Plane_type == X_PLANE && pio->streamline3d.xPlane.count > 0){
	    if(streamGradients){
			if(SageGetFileItem(Files,"cell_index",3,
			                  &lo_x,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",4,
			                  &hi_x,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",5,
			                  &lo_y,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",6,
			                  &hi_y,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
	    }
	    for(n=0;n<pio->streamline3d.xPlane.count;++n){
	        for(k=0;k<2;++k){
		        x1=x=pio->streamline3d.xPlane.x[n];
		        y1=y=pio->streamline3d.xPlane.y[n];
		        ne=-1;
		        nn=n;
		        nstep=0;
		        nGrad=-1;
	    		gradxV1=0;
	    		gradyV1=0;
	    		gradxV2=0;
	    		gradyV2=0;
			    		
doAgain1:
				
		    	if(!pioGetValueN(pio,pio->plane.Plane_xloc,x,y,&ne))continue;
	    		if(streamGradients){
	    		    if(nGrad != ne){
			    		gradxV1=0;
			    		gradyV1=0;
			    		gradxV2=0;
			    		gradyV2=0;
			    		
			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradxV1 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dy2[ke]+pio->dy2[k2]);
	                	
						gradxV2 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradyV1 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradyV2 = 0.5*(gradl+gradh);
                	
	    			}
			    		

	    		    nGrad=ne;
		    	    vx= pio->vectory[ne]+gradxV1*(x-pio->ycenter[ne])+gradyV1*(y-pio->zcenter[ne]);
		    	    vy= pio->vectorz[ne]+gradxV2*(x-pio->ycenter[ne])+gradyV2*(y-pio->zcenter[ne]);
/*		    	    
		    	    sprintf(WarningBuff,"ne %ld n2 %ld %g %g %g %g",ne,n2,gradxV1,gradxV2,gradyV1,gradyV2);
		    	    WarningBatchFast(WarningBuff);
*/
	    		}else{
		    	    vx= pio->vectory[ne];
		    	    vy= pio->vectorz[ne];
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
		    		x2=x1+sx;
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
		    	goto doAgain1;
	    	}
	    	
	    	
	    }
	
	}else if((pio->plane.Plane_type == Y_PLANE) && (pio->streamline3d.yPlane.count > 0)){
	    if(streamGradients){
			if(SageGetFileItem(Files,"cell_index",1,
			                  &lo_x,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",2,
			                  &hi_x,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",5,
			                  &lo_y,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
			if(SageGetFileItem(Files,"cell_index",6,
			                  &hi_y,&dataLength,pio->CurrentFrame)){
				ret=20;goto ErrorOut;
			}
	    }
	    for(n=0;n<pio->streamline3d.yPlane.count;++n){
	        for(k=0;k<2;++k){
		        x1=x=pio->streamline3d.yPlane.x[n];
		        y1=y=pio->streamline3d.yPlane.y[n];
		        ne=-1;
		        nn=n;
		        nstep=0;
		        nGrad=-1;
	    		gradxV1=0;
	    		gradyV1=0;
	    		gradxV2=0;
	    		gradyV2=0;
doAgain2:
				
		    	if(!pioGetValueN(pio,x,pio->plane.Plane_yloc,y,&ne))continue;
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

			    		n2=(long)(lo_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
			    		n2=(long)(hi_x[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dx2[ke]+pio->dx2[k2]);
	                	
						gradxV2 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorx[ne]-pio->vectorx[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorx[n2]-pio->vectorx[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradyV1 = 0.5*(gradl+gradh);

			    		n2=(long)(lo_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradl = (pio->vectorz[ne]-pio->vectorz[n2])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
			    		n2=(long)(hi_y[ne]-1);
	        			k2=(int)(pio->level[n2]);
	        			ke=(int)(pio->level[ne]);
	                	gradh = (pio->vectorz[n2]-pio->vectorz[ne])/(pio->dz2[ke]+pio->dz2[k2]);
	                	
						gradyV2 = 0.5*(gradl+gradh);
                	
	    			}
			    		

	    		    nGrad=ne;
		    	    vx= pio->vectorx[ne]+gradxV1*(x-pio->xcenter[ne])+gradyV1*(y-pio->zcenter[ne]);
		    	    vy= pio->vectorz[ne]+gradxV2*(x-pio->xcenter[ne])+gradyV2*(y-pio->zcenter[ne]);
/*		    	    
		    	    sprintf(WarningBuff,"ne %ld n2 %ld %g %g %g %g",ne,n2,gradxV1,gradxV2,gradyV1,gradyV2);
		    	    WarningBatchFast(WarningBuff);
*/
	    		}else{
		    	    vx= pio->vectorx[ne];
		    	    vy= pio->vectorz[ne];
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
		    		x2=x1+sx;
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
		    	goto doAgain2;
	    	}
	    	
	    	
	    }
	
	}else if((pio->plane.Plane_type == Z_PLANE) && (pio->streamline3d.zPlane.count > 0)){
	
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
	    
	    for(n=0;n<pio->streamline3d.zPlane.count;++n){
	        for(k=0;k<2;++k){
		        x1=x=pio->streamline3d.zPlane.x[n];
		        y1=y=pio->streamline3d.zPlane.y[n];
		        ne=-1;
		        nn=n;
		        nstep=0;
		        nGrad=-1;
	    		gradxV1=0;
	    		gradyV1=0;
	    		gradxV2=0;
	    		gradyV2=0;
doAgain3:
				
		    	if(!pioGetValueN(pio,x,y,pio->plane.Plane_zloc,&ne))continue;
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
		    		x2=x1+sx;
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
		    	/*
	    	    			sprintf(WarningBuff,"Point %ld k %ld %g %g %g %g ne %ld\n",(long)n,(long)k,x,y,sx,sy,ne);
	    	    			WarningBatch(WarningBuff);
	    	    */
		    	if(++nstep > streamSteps)continue;
		    	goto doAgain3;
	    	}
	    	
	    	
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
	/*
    	    sprintf(WarningBuff,"pioDrawStream3d In\n");
    	    WarningBatch(WarningBuff);
	*/
	return ret;
}

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	struct PIO *sage;
	double pi;
	long length;
	long n,ne;
	int ret;

	struct DrawCellStuff dc;
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage || !pio->location)return 1;
	
	ret = 1;
	
	loadVectors(Files);
	
	if(pio->vector.vmax > 0){
		dc.vmax=pio->vector.vmax;
	}else{	
		dc.vmax=pio->vmaxVectors;
	}

	if(dc.vmax <= 0)goto ErrorOut;
	
	dc.dx=pio->range.xmaxData-pio->range.xminData;
	dc.dy=pio->range.ymaxData-pio->range.yminData;
	if(dc.dx < dc.dy){
	    dc.vmaxLen=dc.dx*pio->vector.vfac;
	}else{
	    dc.vmaxLen=dc.dy*pio->vector.vfac;
	}
	
	if(dc.vmaxLen <= 0)goto ErrorOut;
	
	dc.vmaxLen /= dc.vmax;
	
	pi=4.*atan(1.);
	dc.turn=pio->vector.arrowAngle*pi/180.;
	
	dc.arrow=pio->vector.arrowLength*dc.vmax*dc.vmaxLen;
	
   	dc.vmax2=dc.vmax/3.;

	dc.r=r;
	
	dc.ss=ss;

	length=sage->ixmax*sage->iymax*sage->izmax;
			
	if(pio->vector.vspace == 0){
	    for(n=0;n<length;++n){
	    	ne=pio->location[n];	    
        	DrawCellVelocity(ne,pio,&dc);
        }
	}else{
	    for(n=0;n<length;++n){
	    	ne=pio->location[n];	    
        	DrawCellVelocity2(ne,pio,&dc);
        }
	}
	
	ret = 1;
ErrorOut:
	return ret;
}
static int DrawCellVelocity(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc)
{
	double dx2,dy2;
	double xc,yc;
	double *xv,*yv;
	int k,kk;
	long ne;
	    
	if(!pio || !dc)return 1;
	
    k=(int)(pio->level[n]);
    if(pio->plane.Plane_type == X_PLANE){
        xc=pio->xcenter[n];
        dx2=pio->dx2[k];
        if(pio->plane.Plane_xloc < (xc-dx2) || pio->plane.Plane_xloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k];
	        dy2=pio->dz2[k];
	        xv=pio->vectory;
	        yv=pio->vectorz;
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Y_PLANE){
        xc=pio->ycenter[n];
        dx2=pio->dy2[k];
        if(pio->plane.Plane_yloc < (xc-dx2) || pio->plane.Plane_yloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dz2[k];
	        xv=pio->vectorx;
	        yv=pio->vectorz;
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Z_PLANE){
        xc=pio->zcenter[n];
        dx2=pio->dz2[k];
        if(pio->plane.Plane_zloc < (xc-dx2) || pio->plane.Plane_zloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dy2[k];
	        xv=pio->vectorx;
	        yv=pio->vectory;
	        goto DrawIt;
        }
    }else{
        sprintf(WarningBuff,"RangeCell Unknown Plane option %d",(int)pio->plane.Plane_type);
		return 1;
    }
    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        DrawCellVelocity(ne,pio,dc);
		}      
    }
    
    return 0;
    
DrawIt:
    
    
    if(xc+dx2 < pio->range.xminData)return 0;
    if(xc-dx2 > pio->range.xmaxData)return 0;
    if(yc+dy2 < pio->range.yminData)return 0;
    if(yc-dy2 > pio->range.ymaxData)return 0;
   	{
		double vlength,vx,vy;
		double x1,y1,x2,y2,x3,y3;
		double o1,oa;
		double degen;
		int ix,iy;
		    ix=0;
		    iy=0;
	        vx=xv[n];
	        vy=yv[n];
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > dc->vmax){
	            vx = vx*dc->vmax/vlength;
	            vy = vy*dc->vmax/vlength;
	        }
	        if(vlength <= 0)return 0;
	        
		    x1=xc;
		    x3=x2=x1+vx*dc->vmaxLen;
		    y1=yc;
		    y3=y2=y1+vy*dc->vmaxLen;
		    if(!isOut(&x1,&y1,&x2,&y2,dc->r)){
		        RealLoc(pio,&x1,&y1,&ix,&iy);
		        MoveB(ix,iy);
		        RealLoc(pio,&x2,&y2,&ix,&iy);
		        LineB(ix,iy,255,dc->ss);
		    }
		    
			if(x1 == x2 && y2 == y1)return 0;

	      	if(vlength < dc->vmax2){
	      	    degen=vlength/dc->vmax2;
		    }else{
	      		degen=1.;
		    }
		    
	        x2=x3;
	        y2=y3;
	    	o1=atan2(y2-y1,x2-x1);
		    	
		    if(pio->vector.arrowHeads >= 1){
     	 		oa=o1+dc->turn;
			    x1=x2+dc->arrow*degen*cos(oa);
			    y1=y2+dc->arrow*degen*sin(oa);
			    if(!isOut(&x1,&y1,&x2,&y2,dc->r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,dc->ss);
			    }
		    }
	        
		    if(pio->vector.arrowHeads >= 2){		    
		        x2=x3;
		        y2=y3;
     	 		oa=o1-dc->turn;
			    x1=x2+dc->arrow*degen*cos(oa);
			    y1=y2+dc->arrow*degen*sin(oa);
			    if(!isOut(&x1,&y1,&x2,&y2,dc->r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,dc->ss);
			    }
		    }
	        
   	    
   	 }   
   	    
	return 0;
}
	
static int DrawCellVelocity2(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc)
{
	double dx2,dy2;
	double xc,yc;
	double *xv,*yv;
	int k,kk;
	long ne;
	    
	if(!pio || !dc)return 1;
	
    k=(int)(pio->level[n]);
    if(pio->plane.Plane_type == X_PLANE){
        xc=pio->xcenter[n];
        dx2=pio->dx2[k];
        if(pio->plane.Plane_xloc < (xc-dx2) || pio->plane.Plane_xloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k];
	        dy2=pio->dz2[k];
	        xv=pio->vectory;
	        yv=pio->vectorz;
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Y_PLANE){
        xc=pio->ycenter[n];
        dx2=pio->dy2[k];
        if(pio->plane.Plane_yloc < (xc-dx2) || pio->plane.Plane_yloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dz2[k];
	        xv=pio->vectorx;
	        yv=pio->vectorz;
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Z_PLANE){
        xc=pio->zcenter[n];
        dx2=pio->dz2[k];
        if(pio->plane.Plane_zloc < (xc-dx2) || pio->plane.Plane_zloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dy2[k];
	        xv=pio->vectorx;
	        yv=pio->vectory;
	        goto DrawIt;
        }
    }else{
        sprintf(WarningBuff,"RangeCell Unknown Plane option %d",(int)pio->plane.Plane_type);
		return 1;
    }
    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        DrawCellVelocity2(ne,pio,dc);
		}      
    }
    
    return 0;
    
DrawIt:
    
    
    if(xc+dx2 < pio->range.xminData)return 0;
    if(xc-dx2 > pio->range.xmaxData)return 0;
    if(yc+dy2 < pio->range.yminData)return 0;
    if(yc-dy2 > pio->range.ymaxData)return 0;
   	{   	
	    long ixmin,ixmax,iymin,iymax;
	    double xmin,xmax,ymin,ymax;
	    double didx,dxdi,djdy,dydj;
	    double vx,vy,vlength;
	    double vspace;
		double x1,y1,x2,y2,x3,y3;
		double degen;
		int ix,iy;

		double o1,oa;
	   
		ix=0;
		iy=0;
	    vspace=pio->vector.vspace;
	    if(vspace < 0){
	    	vspace = -vspace;	    
			if(dc->dx < dc->dy){
				vspace=dc->dx/vspace;
			}else{
				vspace=dc->dy/vspace;
			}
	    }
	    
	   	    
	    xmin=pio->range.xminData;
	    xmax=pio->range.xmaxData;
	    ymin=pio->range.yminData;
	    ymax=pio->range.ymaxData;
	    ixmin=0;
	    ixmax=(long)(dc->dx/vspace);
	    iymin=0;
	    iymax=(long)(dc->dy/vspace);

        didx=((double)(ixmax-ixmin))/(xmax-xmin);
        dxdi=1.0/didx;
        djdy=((double)(iymax-iymin))/(ymax-ymin);
        dydj=1.0/djdy;


	    {
	        double ymn,ymx,xmn,xmx;
	        double xs,ys;
	        long i,j,ihi,ilo,jhi,jlo;
	        
			ymn=max(ymin,yc-dy2);
			ymx=min(ymax,yc+dy2);
			if(ymx < ymn)return 0;
	        
			xmn=max(xmin,xc-dx2);
			xmx=min(xmax,xc+dx2);
			if(xmx < xmn)return 0;
			
			ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
			ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);

			jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
			jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
			
	        vx=xv[n];
	        vy=yv[n];
	        vlength = sqrt(vx*vx+vy*vy);
	        if(vlength > dc->vmax){
	            vx = vx*dc->vmax/vlength;
	            vy = vy*dc->vmax/vlength;
	        }
	        if(vlength <= 0)return 0;
			
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
			        
					x1=xs;
				    x3=x2=x1+vx*dc->vmaxLen;
				    y1=ys;
				    y3=y2=y1+vy*dc->vmaxLen;
				    if(!isOut(&x1,&y1,&x2,&y2,dc->r)){
				        RealLoc(pio,&x1,&y1,&ix,&iy);
				        MoveB(ix,iy);
				        RealLoc(pio,&x2,&y2,&ix,&iy);
				        LineB(ix,iy,255,dc->ss);
				    }else{
				        continue;
				    }
				    
					if(x1 == x2 && y2 == y1)continue;

			      	if(vlength < dc->vmax2){
			      	    degen=vlength/dc->vmax2;
				    }else{
			      		degen=1.;
				    }
				    
			        x2=x3;
			        y2=y3;
			    	o1=atan2(y2-y1,x2-x1);
				    	
				    if(pio->vector.arrowHeads >= 1){
		     	 		oa=o1+dc->turn;
					    x1=x2+dc->arrow*degen*cos(oa);
					    y1=y2+dc->arrow*degen*sin(oa);
					    if(!isOut(&x1,&y1,&x2,&y2,dc->r)){
					        RealLoc(pio,&x1,&y1,&ix,&iy);
					        MoveB(ix,iy);
					        RealLoc(pio,&x2,&y2,&ix,&iy);
					        LineB(ix,iy,255,dc->ss);
					    }
				    }
			        
				    if(pio->vector.arrowHeads >= 2){		    
				        x2=x3;
				        y2=y3;
		     	 		oa=o1-dc->turn;
					    x1=x2+dc->arrow*degen*cos(oa);
					    y1=y2+dc->arrow*degen*sin(oa);
					    if(!isOut(&x1,&y1,&x2,&y2,dc->r)){
					        RealLoc(pio,&x1,&y1,&ix,&iy);
					        MoveB(ix,iy);
					        RealLoc(pio,&x2,&y2,&ix,&iy);
					        LineB(ix,iy,255,dc->ss);
					    }
				    }			          
			    }
			}
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
    if(pio->plane.Plane_type == X_PLANE){
        xc=pio->xcenter[n];
        dx2=pio->dx2[k];
        if(pio->plane.Plane_xloc < (xc-dx2) || pio->plane.Plane_xloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k];
	        dy2=pio->dz2[k];
	        xv=pio->vectory;
	        yv=pio->vectorz;
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Y_PLANE){
        xc=pio->ycenter[n];
        dx2=pio->dy2[k];
        if(pio->plane.Plane_yloc < (xc-dx2) || pio->plane.Plane_yloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dz2[k];
	        xv=pio->vectorx;
	        yv=pio->vectorz;
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Z_PLANE){
        xc=pio->zcenter[n];
        dx2=pio->dz2[k];
        if(pio->plane.Plane_zloc < (xc-dx2) || pio->plane.Plane_zloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dy2[k];
	        xv=pio->vectorx;
	        yv=pio->vectory;
	        goto DrawIt;
        }
    }else{
        sprintf(WarningBuff,"RangeCell Unknown Plane option %d",(int)pio->plane.Plane_type);
		return 1;
    }
    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        RangeCellVelocity(ne,pio,vmax);
		}      
    }
    
    return 0;
    
DrawIt:
    
    
    if(xc+dx2 < pio->range.xminData)return 0;
    if(xc-dx2 > pio->range.xmaxData)return 0;
    if(yc+dy2 < pio->range.yminData)return 0;
    if(yc-dy2 > pio->range.ymaxData)return 0;
    v=sqrt(xv[n]*xv[n]+yv[n]*yv[n]);
    if(*vmax < v)*vmax = v;
   	    
	return 0;
}
int pioDrawTracers3D(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	long length,length1;
	double *x,*y,*z,*v;
	long n,k;
	int ret;
	double x1,y1,x2,y2;
	int ix,iy,nv;
	
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	
	x=NULL;
	y=NULL;
	z=NULL;
	v=NULL;
	
	ret=1;

	if((pio->TracerPlotCount < 1) || !pio->TracerPlotList)goto ErrorOut;

	if(SageGetFileItem(Files,"tracer_data",0,&v,&length,pio->CurrentFrame))goto ErrorOut;
	if(length < 2)goto ErrorOut;
	
	x=(double *)cMalloc(length*sizeof(double),3261);
	if(!x)goto ErrorOut;
	
	y=(double *)cMalloc(length*sizeof(double),3262);
	if(!y)goto ErrorOut;
		
	z=(double *)cMalloc(length*sizeof(double),3263);
	if(!z)goto ErrorOut;
		
	for(n=0;n<pio->TracerPlotCount;++n){
	    nv = pio->TracerPlotList[n] % pio->tracer_words_per_record;
	    nv /= pio->tracer_words_per_point;
	    nv *= pio->tracer_words_per_point;
	    
	    length1=0;
	    for(k=0;k<length;k+=pio->tracer_words_per_record){
	    	x[length1  ]=v[1+nv+k];
	    	y[length1  ]=v[2+nv+k];
	    	z[length1++]=v[3+nv+k];
	    }
		if(length1 < 2)continue;
		
				
	    if(pio->plane.Plane_type == X_PLANE){
			for(k=1;k<length1;++k){
			    x1=y[k-1];
			    x2=y[k];
			    y1=z[k-1];
			    y2=z[k];
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,ss);
			    }
			}		
	    }else if(pio->plane.Plane_type == Y_PLANE){
			for(k=1;k<length1;++k){
			    x1=x[k-1];
			    x2=x[k];
			    y1=z[k-1];
			    y2=z[k];
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,ss);
			    }
			}		
	    }else if(pio->plane.Plane_type == Z_PLANE){	    
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
		    
	    }else{
	        sprintf(WarningBuff,"pioDrawTracers3D Unknown Plane option %d",(int)pio->plane.Plane_type);
			goto ErrorOut;
	    }
				
				
	}
	ret=0;
ErrorOut:	
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	if(v)cFree((char *)v);
	return ret;
}
static int pioDrawLasers(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
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
		
	    if(pio->plane.Plane_type == X_PLANE){
			for(k=1;k<length;++k){
			    x1=y[k-1];
			    x2=y[k];
			    y1=z[k-1];
			    y2=z[k];
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,ss);
			    }
			}		
	    }else if(pio->plane.Plane_type == Y_PLANE){
			for(k=1;k<length;++k){
			    x1=x[k-1];
			    x2=x[k];
			    y1=z[k-1];
			    y2=z[k];
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			        RealLoc(pio,&x1,&y1,&ix,&iy);
			        MoveB(ix,iy);
			        RealLoc(pio,&x2,&y2,&ix,&iy);
			        LineB(ix,iy,255,ss);
			    }
			}		
	    }else if(pio->plane.Plane_type == Z_PLANE){	    
			for(k=1;k<length;++k){
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
		    
	    }else{
	        sprintf(WarningBuff,"pioDrawLasers Unknown Plane option %d",(int)pio->plane.Plane_type);
			goto ErrorOut;
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
static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
	struct FilePIOInfo *pio;
   	static double  small2=1e-10;
	struct PIO *sage;
	struct dRange r;
	long length;	
	long n,ne;

	if(!Files || !ss || !buff)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
	if(!sage || !pio->location)return 1;

	setPioScales(pio);

	r.xmin=pio->range.xminData;
	r.ymin=pio->range.yminData;
	r.xmax=pio->range.xmaxData;
	r.ymax=pio->range.ymaxData;

	r.dxlim=small2*(r.xmax-r.xmin);
	if(r.dxlim <= 0.0)r.dxlim=small2;
	r.dylim=small2*(r.ymax-r.ymin);
	if(r.dylim <= 0.0)r.dylim=small2;


	if(pio->pd.AreaFill){
		double *d;
		long length;

		length=pio->range.ixmax*pio->range.iymax;
		d=(double *)buff;
		setFloat(d,length);
	    if(getPlotData(Files,d,CurrentFrame)){
		    sprintf(WarningBuff,"doSage2DGetData - getPlotData Error\n");
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
	
	length=sage->ixmax*sage->iymax*sage->izmax;
	
	if(pio->pa.DrawZones && pio->level){
	    for(n=0;n<length;++n){
			ne=pio->location[n];	    
	        DrawCellLines(ne,pio,ss,&r);
	    }
    }
	    
	if(pio->vector.DrawVectors){
		pioDrawVectors(Files,ss,&r);
	}

	if(pio->streamline3d.streamOn){
		pioDrawStream3d(Files,ss,&r);
	}

	if(pio->LaserPlotCount){
	   pioDrawLasers(Files,ss,&r);
	}

	if(pio->TracerPlotCount){
	   pioDrawTracers3D(Files,ss,&r);
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
        double xc,yc,zc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double xs,ys;
        long i,j,ihi,ilo,jhi,jlo;
        int k;

        if(pio->daughter[n] || pio->active[n])continue;
        k=(int)(pio->level[n]);

	    if(pio->plane.Plane_type == X_PLANE){
	        zc=pio->xcenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        if(pio->plane.Plane_xloc < (zc-dx2) || pio->plane.Plane_xloc > (zc+dx2))continue;
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k]*1.000001;
	        dy2=pio->dz2[k]*1.000001;
	    }else if(pio->plane.Plane_type == Y_PLANE){
	        zc=pio->ycenter[n];
	        dx2=pio->dy2[k]*1.000001;
	        if(pio->plane.Plane_yloc < (zc-dx2) || pio->plane.Plane_yloc > (zc+dx2))continue;
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        dy2=pio->dz2[k]*1.000001;
	    }else if(pio->plane.Plane_type == Z_PLANE){
	        zc=pio->zcenter[n];
	        dx2=pio->dz2[k]*1.000001;
	        if(pio->plane.Plane_zloc < (zc-dx2) || pio->plane.Plane_zloc > (zc+dx2))continue;
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k]*1.000001;
	        dy2=pio->dy2[k]*1.000001;
	    }else{
	        sprintf(WarningBuff,"PlotPIOData Unknown Plane option %d\n",(int)pio->plane.Plane_type);
	        WarningBatch(WarningBuff);
	        break;
	    }

		
		ymn=max(ymin,yc-dy2);
		ymx=min(ymax,yc+dy2);
		if(ymx < ymn)continue;

		xmn=max(xmin,xc-dx2);
		xmx=min(xmax,xc+dx2);
		if(xmx < xmn)continue;
		
		ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
		ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);

		jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
		jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);

		
	    if(pio->plane.Plane_type == X_PLANE){
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
					sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=255;
			    }
			}
	    }else if(pio->plane.Plane_type == Y_PLANE){
			for(j=jlo;j<=jhi;++j){
			    ys=ymin+(double)((double)(j-iymin)+.5)*dydj;
			    if(ys < ymn || ys > ymx)continue;
			    for(i=ilo;i<=ihi;++i){
			        xs=xmin+(double)((double)(i-ixmin)+.5)*dxdi;
			        if(xs < xmn || xs > xmx)continue;
					sout[i+(pio->range.iymax-1-j)*pio->range.ixmax]=255;
			    }
			}
	    }else if(pio->plane.Plane_type == Z_PLANE){
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
    }
    
    return 0;
	        
}
/*
static int RangeCell(long n,struct FilePIOInfo *pio,double *dmin,double *dmax)
{
	double dx2,dy2;
	double xc,yc;
	double v;
	int k,kk;
	long ne;
	    
	if(!pio || !dmin || !dmax)return 1;
	
    k=(int)(pio->level[n]);
    if(pio->plane.Plane_type == X_PLANE){
        xc=pio->xcenter[n];
        dx2=pio->dx2[k];
        if(pio->plane.Plane_xloc < (xc-dx2) || pio->plane.Plane_xloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k];
	        dy2=pio->dz2[k];
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Y_PLANE){
        xc=pio->ycenter[n];
        dx2=pio->dy2[k];
        if(pio->plane.Plane_yloc < (xc-dx2) || pio->plane.Plane_yloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dz2[k];
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Z_PLANE){
        xc=pio->zcenter[n];
        dx2=pio->dz2[k];
        if(pio->plane.Plane_zloc < (xc-dx2) || pio->plane.Plane_zloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dy2[k];
	        goto DrawIt;
        }
    }else{
        sprintf(WarningBuff,"RangeCell Unknown Plane option %d",(int)pio->plane.Plane_type);
		return 1;
    }
    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        RangeCell(ne,pio,dmin,dmax);
		}      
    }
    
    return 0;
    
DrawIt:
    
    
    if(xc+dx2 < pio->range.xminData)return 0;
    if(xc-dx2 > pio->range.xmaxData)return 0;
    if(yc+dy2 < pio->range.yminData)return 0;
    if(yc-dy2 > pio->range.ymaxData)return 0;
    v=pio->value[n];
    if(*dmin > v)*dmin = v;
    if(*dmax < v)*dmax = v;
   	    
	return 0;
}
*/
static int DrawCellLines(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r)
{
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2;
	unsigned char nc;
	int ix,iy;
	int k,kk;
	long ne;
	    
	if(!pio || !ss || !r || !pio->value)return 1;
	
    k=(int)(pio->level[n]);
	if(k > ss->ZoneLevel)return 0;
	
	ix=0;
	iy=0;
    if(pio->plane.Plane_type == X_PLANE){
        xc=pio->xcenter[n];
        dx2=pio->dx2[k];
        if(pio->plane.Plane_xloc < (xc-dx2) || pio->plane.Plane_xloc > (xc+dx2))return 1;
        if(!pio->daughter[n] || k == ss->ZoneLevel){
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k];
	        dy2=pio->dz2[k];
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Y_PLANE){
        xc=pio->ycenter[n];
        dx2=pio->dy2[k];
        if(pio->plane.Plane_yloc < (xc-dx2) || pio->plane.Plane_yloc > (xc+dx2))return 1;
        if(!pio->daughter[n] || k == ss->ZoneLevel){
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dz2[k];
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Z_PLANE){
        xc=pio->zcenter[n];
        dx2=pio->dz2[k];
        if(pio->plane.Plane_zloc < (xc-dx2) || pio->plane.Plane_zloc > (xc+dx2))return 1;
        if(!pio->daughter[n] || k == ss->ZoneLevel){
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dy2[k];
	        goto DrawIt;
        }
    }else{
        sprintf(WarningBuff,"DrawCellLines Unknown Plane option %d",(int)pio->plane.Plane_type);
		return 1;
    }
    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        DrawCellLines(ne,pio,ss,r);
		}      
    }
    
    return 0;
    
DrawIt:
    nc=255;
    if(pio->pa.DrawZonesColor){
        FloatToColor(&pio->value[n],&pio->pd,&nc);
    }
    x1=xc-dx2;
    y1=yc-dy2;
    x2=xc+dx2;
    y2=yc-dy2;
    if(!isOut(&x1,&y1,&x2,&y2,r)){
        RealLoc(pio,&x1,&y1,&ix,&iy);
        MoveB(ix,iy);
        RealLoc(pio,&x2,&y2,&ix,&iy);
        LineB(ix,iy,nc,ss);
    }

    x1=xc+dx2;
    y1=yc-dy2;
    x2=xc+dx2;
    y2=yc+dy2;
    if(!isOut(&x1,&y1,&x2,&y2,r)){
        RealLoc(pio,&x1,&y1,&ix,&iy);
        MoveB(ix,iy);
        RealLoc(pio,&x2,&y2,&ix,&iy);
        LineB(ix,iy,nc,ss);
    }

    x1=xc+dx2;
    y1=yc+dy2;
    x2=xc-dx2;
    y2=yc+dy2;
    if(!isOut(&x1,&y1,&x2,&y2,r)){
        RealLoc(pio,&x1,&y1,&ix,&iy);
        MoveB(ix,iy);
        RealLoc(pio,&x2,&y2,&ix,&iy);
        LineB(ix,iy,nc,ss);
    }

    x1=xc-dx2;
    y1=yc+dy2;
    x2=xc-dx2;
    y2=yc-dy2;
    if(!isOut(&x1,&y1,&x2,&y2,r)){
        RealLoc(pio,&x1,&y1,&ix,&iy);
        MoveB(ix,iy);
        RealLoc(pio,&x2,&y2,&ix,&iy);
        LineB(ix,iy,nc,ss);
    }
	    
	return 0;
}
static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy)
{

	if(!pio || !x || !y || !ix || !iy)return 1;

	*ix=(int)((*x-pio->range.xminData)*pio->range.xScale+(double)pio->range.ixminData+.5);
	*iy=(int)((*y-pio->range.yminData)*pio->range.yScale+(double)pio->range.iyminData+.5);

	return 0;
}
static int setPioScales(struct FilePIOInfo *pio)
{
	if(!pio)return 1;

	if(pio->range.ixminData < 0)pio->range.ixminData=0;
	if(pio->range.iyminData < 0)pio->range.iyminData=0;
	if(pio->range.ixmaxData > pio->range.ixmax)pio->range.ixmaxData=pio->range.ixmax;
	if(pio->range.iymaxData > pio->range.iymax)pio->range.iymaxData=pio->range.iymax;
	

	if(pio->plane.Plane_type == X_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->ymin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->zmin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->ymax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->zmax;
	}else if(pio->plane.Plane_type == Y_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->zmin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->zmax;
	}else if(pio->plane.Plane_type == Z_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->ymin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->ymax;
	}else if(pio->plane.Plane_type == G_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->ymin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->ymax;
	}

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

static int doScale1Action(struct uDialogStruct *uList);
int doDataScale(struct FileInfo2 *Files);

int doDataScale(struct FileInfo2 *Files)
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

int doDataRange(struct FileInfo2 *Files);

double doubleConvert(double x);

double doubleConvert(double x)
{
	char buff[2048];
	
	sprintf(buff,"%g",x);
	
	return atof(buff);
}
int doDataRange(struct FileInfo2 *Files)
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
int doDataSelection(struct FileInfo2 *Files);

int doDataSelection(struct FileInfo2 *Files)
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
			if(SageGetFileItem(Files,nameItem,plotItem,&fdata,&dataLength,nlist)){
			    ret=17;goto OutOfHere;
			}

			if(SageGetFileItem(Files,"cell_daughter",0,&daughter,&dataLength,nlist)){
			    ret=18;goto OutOfHere;
			}

			if(SageGetFileItem(Files,"cell_active",0,&active,&dataLength,nlist)){
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

	if(filePIOList(Files,n,List1,1))goto ErrorOut;
		
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
	double vmax;
	long n,nmax,k;
	
	
	if(!Files)return 1;
	pio = &Files->pioData;
		
	if(GetPIOData(Files,pio->CurrentFrame))goto OutOfHere;
	
	if(!pio->daughter || !pio->level   || !pio->xcenter ||
	   !pio->ycenter  || !pio->zcenter || !pio->value)goto OutOfHere;

	nmax = -1;
	vmax=0;
    for(n=0;n<pio->numcell;++n){
        if(pio->daughter[n])continue;
        if(fabs(pio->value[n]) > vmax){
            vmax=fabs(pio->value[n]);
            nmax=n;            
        }
	}
		
	if(nmax == -1)goto OutOfHere;
		
	pio->plane.Plane_xloc=pio->xcenter[nmax];
	pio->plane.Plane_yloc=pio->ycenter[nmax];
	pio->plane.Plane_zloc=pio->zcenter[nmax];
	
	k=(long)(pio->level[nmax]);    
	    
	pio->plane.Plane_xmin=pio->xcenter[nmax]-40*pio->dx2[k];
	pio->plane.Plane_xmax=pio->xcenter[nmax]+40*pio->dx2[k];
	
	pio->plane.Plane_ymin=pio->ycenter[nmax]-40*pio->dy2[k];
	pio->plane.Plane_ymax=pio->ycenter[nmax]+40*pio->dy2[k];
	
	pio->plane.Plane_zmin=pio->zcenter[nmax]-40*pio->dz2[k];
	pio->plane.Plane_zmax=pio->zcenter[nmax]+40*pio->dz2[k];
	
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
				
OutOfHere:
	return 0;
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
	    
	    
		if(GetPIOData(Files,ni))goto OutOfHere;
	    
	    rank=3;
		size[0]=(int)pio->plane.Plane_zcount;
		size[1]=(int)pio->plane.Plane_ycount;
		size[2]=(int)pio->plane.Plane_xcount;
		    					    
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
	        pio->sage->time);
	        
	      	    
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
			        pio->sage->time);
		        
		        
		        
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
			        pio->sage->time);
		        
		        
		        
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
			        pio->sage->time);
		      		        
		        
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
int doVectorSelection(struct FileInfo2 *Files);
int doVectorSelection(struct FileInfo2 *Files)
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
int doStreamlineSelection(struct PlotStreamData *streamline)
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
int doVolumeSelection(struct FileInfo2 *Files)
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

#ifdef JUNKIN

int doStreamlineSelectionOLD(struct PlotStreamlineData *streamline)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {200,350,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {290,350,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"Draw Streamlines", {20,20,140,20},uDialogCheck,uDialogGetLong},
       /*  3 */      {"xStart", {100,100,70,20},uDialogText,uDialogGetDouble},
       /*  4 */      {"yStart", {190,100,70,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"xEnd",   {100,130,70,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"yEnd",   {190,130,70,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"num",    {100,160,70,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"error",  {150,210,70,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"speedCutoff", {150,240,70,20},uDialogText,uDialogGetDouble},
       /* 10 */      {"maxSteps",    {150,270,70,20},uDialogText,uDialogGetDouble},
       /* 11 */      {"maxCells",    {150,300,70,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"Use Gradients", {20,210,110,20},uDialogCheck,uDialogGetLong},
       /* 13 */      {"Forward Only",  {20,240,110,20},uDialogButtonRadio,uDialogGetLong},
       /* 14 */      {"Backward Only", {20,270,115,20},uDialogButtonRadio,uDialogGetLong},
       /* 15 */      {"Fwd and Bkwd",    {20,300,110,20},uDialogButtonRadio,uDialogGetLong},
	   /* 16 */      {"Define a line to start multiple streamlines from:", {20,60,300,20},uDialogString,uDialogNone},
       /* 17 */      {"x", {120,75,20,20},uDialogString,uDialogNone},
       /* 18 */      {"y", {210,75,20,20},uDialogString,uDialogNone},
       /* 19 */      {"Start Point", {20,100,75,20},uDialogString,uDialogNone},
       /* 20 */      {"End Point", {20,130,75,20},uDialogString,uDialogNone},
       /* 21 */      {"Number", {20,160,75,20},uDialogString,uDialogNone},
       /* 22 */      {"Integ Error (0=min, 1=max)", {240,210,200,20},uDialogString,uDialogNone},
       /* 23 */      {"Speed Cutoff", {240,240,200,20},uDialogString,uDialogNone},
       /* 24 */      {"Max Steps Per Cell", {240,270,200,20},uDialogString,uDialogNone},
       /* 25 */      {"Max Cells Per Streamline", {240,300,200,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,420,410};
 	
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
	              uDialogSetLogical,(streamline->DrawStreamlines == 1),
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,streamline->xStart,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,streamline->yStart,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,streamline->xEnd,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,streamline->yEnd,
	              
	              uDialogSetItem,7L,
		          uDialogSetDouble,(double)streamline->num,
	              
	              uDialogSetItem,8L,
		          uDialogSetDouble,streamline->error,
	              
	              uDialogSetItem,9L,
		          uDialogSetDouble,streamline->speedCutoff,
	              
	              uDialogSetItem,10L,
		          uDialogSetDouble,(double)streamline->maxSteps,
	              
	              uDialogSetItem,11L,
		          uDialogSetDouble,(double)streamline->maxCells,
	              
	              uDialogSetItem,12L,
	              uDialogSetLogical,(streamline->doGrad == 1),

				  uDialogSetItem,13L,
	              uDialogSetLogical,(streamline->dir == 1),
	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(streamline->dir == -1),
	              
	              uDialogSetItem,15L,
	              uDialogSetLogical,(streamline->dir == 0),

	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;


    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	streamline->DrawStreamlines = (int)uR[2].lreturn;
	streamline->xStart = uR[3].dreturn;
	streamline->yStart = uR[4].dreturn;
	streamline->xEnd = uR[5].dreturn;
	streamline->yEnd = uR[6].dreturn;
	streamline->num = (int)uR[7].dreturn;
	streamline->error = uR[8].dreturn;
	streamline->speedCutoff = uR[9].dreturn;
	streamline->maxSteps = (int)uR[10].dreturn;
	streamline->maxCells = (int)uR[11].dreturn;
	streamline->doGrad = (int)uR[12].lreturn;
	if(uR[13].lreturn){
		streamline->dir=1;
	}else if(uR[14].lreturn){
		streamline->dir=-1;
	}else if(uR[15].lreturn){
		streamline->dir=0;
	}

	if (streamline->num<1) streamline->num=1;
	if (streamline->error<0.0) streamline->error=0.0;
	if (streamline->error>1.0) streamline->error=1.0;
	if (streamline->speedCutoff<0.0) streamline->speedCutoff=0.0;
	if (streamline->maxSteps<100) streamline->maxSteps=100;
	if (streamline->maxCells<1) streamline->maxCells=1;

	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int ConOne(double *x,double *y,double *p,struct FilePIOInfo *pio,struct screenData *ss);
static int DrawCellColor(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);

static int DrawCellColor(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r)
{
	double x[3],y[3],c[3],value;
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2,x3,y3,x4,y4;
	long ne;
	int k,kk,in;
	    
	if(!pio || !ss || !r)return 1;
	
	
    k=(int)(pio->level[n]);

    if(pio->plane.Plane_type == X_PLANE){
        xc=pio->xcenter[n];
        dx2=pio->dx2[k];
        if(pio->plane.Plane_xloc < (xc-dx2) || pio->plane.Plane_xloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->ycenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dy2[k];
	        dy2=pio->dz2[k];
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Y_PLANE){
        xc=pio->ycenter[n];
        dx2=pio->dy2[k];
        if(pio->plane.Plane_yloc < (xc-dx2) || pio->plane.Plane_yloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->zcenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dz2[k];
	        goto DrawIt;
        }
    }else if(pio->plane.Plane_type == Z_PLANE){
        xc=pio->zcenter[n];
        dx2=pio->dz2[k];
        if(pio->plane.Plane_zloc < (xc-dx2) || pio->plane.Plane_zloc > (xc+dx2))return 1;
        if(!pio->daughter[n]){
	        xc=pio->xcenter[n];
	        yc=pio->ycenter[n];
	        dx2=pio->dx2[k];
	        dy2=pio->dy2[k];
	        goto DrawIt;
        }
    }else{
        sprintf(WarningBuff,"DrawCellColor Unknown Plane option %d",(int)pio->plane.Plane_type);
		return 1;
    }

    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        DrawCellColor(ne,pio,ss,r);
		}      
    }
    return 0;
    
DrawIt:    
    
	value=pio->value[n];

    x1=xc-dx2;
    y1=yc-dy2;
    x2=xc+dx2;
    y2=yc-dy2;
    x3=xc+dx2;
    y3=yc+dy2;
    x4=xc-dx2;
    y4=yc+dy2;

	in=0;
    if(!isOut(&x1,&y1,&x2,&y2,r)){
    	in=1;
    }

    if(!isOut(&x2,&y2,&x3,&y3,r)){
    	in=1;
    }

    if(!isOut(&x3,&y3,&x4,&y4,r)){
    	in=1;
    }

    if(!isOut(&x4,&y4,&x1,&y1,r)){
    	in=1;
    }
    if(!in)return 1;
    
    
    
    x1=xc-dx2;
    y1=yc-dy2;
    x2=xc+dx2;
    y2=yc-dy2;
    x3=xc+dx2;
    y3=yc+dy2;
    x4=xc-dx2;
    y4=yc+dy2;
	x[0]=x1;
	x[1]=x2;
	x[2]=x4;
	y[0]=y1;
	y[1]=y2;
	y[2]=y4;
	if(pio->doGradients){	
	    if(pio->plane.Plane_type == X_PLANE){
		    c[0]=value+(x1-xc)*pio->grady[n]+(y1-yc)*pio->gradz[n]+(pio->plane.Plane_xloc-pio->xcenter[n])*pio->gradx[n];
		    c[1]=value+(x2-xc)*pio->grady[n]+(y2-yc)*pio->gradz[n]+(pio->plane.Plane_xloc-pio->xcenter[n])*pio->gradx[n];
		    c[2]=value+(x4-xc)*pio->grady[n]+(y4-yc)*pio->gradz[n]+(pio->plane.Plane_xloc-pio->xcenter[n])*pio->gradx[n];
	    }else if(pio->plane.Plane_type == Y_PLANE){
		    c[0]=value+(x1-xc)*pio->gradx[n]+(y1-yc)*pio->gradz[n]+(pio->plane.Plane_yloc-pio->ycenter[n])*pio->grady[n];
		    c[1]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->gradz[n]+(pio->plane.Plane_yloc-pio->ycenter[n])*pio->grady[n];
		    c[2]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->gradz[n]+(pio->plane.Plane_yloc-pio->ycenter[n])*pio->grady[n];
	    }else if(pio->plane.Plane_type == Z_PLANE){
		    c[0]=value+(x1-xc)*pio->gradx[n]+(y1-yc)*pio->grady[n]+(pio->plane.Plane_zloc-pio->zcenter[n])*pio->gradz[n];
		    c[1]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->grady[n]+(pio->plane.Plane_zloc-pio->zcenter[n])*pio->gradz[n];
		    c[2]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->grady[n]+(pio->plane.Plane_zloc-pio->zcenter[n])*pio->gradz[n];
	    }		
	}else{
	    c[0]=value;
	    c[1]=value;
	    c[2]=value;
    }
	ConOne(x,y,c,pio,ss);
	x[0]=x3;
	x[1]=x4;
	x[2]=x2;
	y[0]=y3;
	y[1]=y4;
	y[2]=y2;
	if(pio->doGradients){
	    if(pio->plane.Plane_type == X_PLANE){
		    c[0]=value+(x3-xc)*pio->grady[n]+(y3-yc)*pio->gradz[n]+(pio->plane.Plane_xloc-pio->xcenter[n])*pio->gradx[n];
		    c[1]=value+(x4-xc)*pio->grady[n]+(y4-yc)*pio->gradz[n]+(pio->plane.Plane_xloc-pio->xcenter[n])*pio->gradx[n];
		    c[2]=value+(x2-xc)*pio->grady[n]+(y2-yc)*pio->gradz[n]+(pio->plane.Plane_xloc-pio->xcenter[n])*pio->gradx[n];
	    }else if(pio->plane.Plane_type == Y_PLANE){
		    c[0]=value+(x3-xc)*pio->gradx[n]+(y3-yc)*pio->gradz[n]+(pio->plane.Plane_yloc-pio->ycenter[n])*pio->grady[n];
		    c[1]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->gradz[n]+(pio->plane.Plane_yloc-pio->ycenter[n])*pio->grady[n];
		    c[2]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->gradz[n]+(pio->plane.Plane_yloc-pio->ycenter[n])*pio->grady[n];
	    }else if(pio->plane.Plane_type == Z_PLANE){
		    c[0]=value+(x3-xc)*pio->gradx[n]+(y3-yc)*pio->grady[n]+(pio->plane.Plane_zloc-pio->zcenter[n])*pio->gradz[n];
		    c[1]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->grady[n]+(pio->plane.Plane_zloc-pio->zcenter[n])*pio->gradz[n];
		    c[2]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->grady[n]+(pio->plane.Plane_zloc-pio->zcenter[n])*pio->gradz[n];
	    }		
	}
	ConOne(x,y,c,pio,ss);
	
	return 0;
}
static int ConOne(double *x,double *y,double *p,struct FilePIOInfo *pio,struct screenData *ss)
{
	int ix,iy,ixmin,ixmax,iymin,iymax,large=32000,n,itIn;
	double td,te,tf,tg,th,ti,d,od,r,s,pp,xx,yy;
	double xstart,xend,ystart,yend,dx,dy,tj,tk,tl;
	unsigned int ic;
	double pmin,pmax;
	static double small2 = -1.0e-8;
	static double  onep = 1.000001;
	double a,b;
	int sType;

	if(!x || !y || !p || !pio || !ss)return 1;
	
	
	sType=pio->pd.sType;
	a=pio->a;
	b=pio->b;
	pmin=pio->pd.sPmin;
	pmax=pio->pd.sPmax;

	if(!(d=(x[1]-x[0])*(y[2]-y[0])-(y[1]-y[0])*(x[2]-x[0]))){
	    return 1;
	} 
	    
	    
	od = 1./d;

	iymin=ixmin=  large;
	iymax=ixmax= -large;
	for(n=0;n<3;++n){
	    RealLoc(pio,&x[n],&y[n],&ix,&iy);
	    if(ixmin > ix)ixmin = ix;
	    if(ixmax < ix)ixmax = ix;
	    if(iymin > iy)iymin = iy;
	    if(iymax < iy)iymax = iy;
	}

	td=od*(y[2]-y[0]);
	te=od*(-(x[2]-x[0]));
	tf= -td*x[0]-te*y[0];
	
	tg=od*(-(y[1]-y[0]));
	th=od*(x[1]-x[0]);
	ti= -tg*x[0]-th*y[0];
	
	tj=p[0];
	tk=p[1]-p[0];
	tl=p[2]-p[0];

	if(ixmin < pio->range.ixminData)ixmin=(int)(pio->range.ixminData);
	if(ixmax > pio->range.ixmaxData)ixmax=(int)(pio->range.ixmaxData);
	if(iymin < pio->range.iyminData)iymin=(int)(pio->range.iyminData);
	if(iymax > pio->range.iymaxData)iymax=(int)(pio->range.iymaxData);
	
	
	IntLoc(pio,&xstart,&ystart,ixmin,iymin);
	IntLoc(pio,&xend,&yend,ixmax,iymax);
	if((n=ixmax-ixmin)){
	    dx = (xend - xstart)/n;
	} else {
	    dx=0.;
	}
	if((n=iymax-iymin)){
	    dy = (yend - ystart)/n;
	} else {
	    dy=0.;
	}
	yy=ystart;
	for(iy=iymin;iy<=iymax;++iy,yy += dy){
	    xx=xstart;
	    itIn=FALSE;
	    for(ix=ixmin;ix<=ixmax;++ix,xx += dx){ 
	        if(((r=td*xx+te*yy+tf) >= small2) && ((s=tg*xx+th*yy+ti) >= small2) &&
	           ((r+s) <= onep)){
			    pp=tj+r*tk+s*tl;
			    if(sType == 1){
			        if(pp < pmin)pp=pmin;
			        if(pp > pmax)pp=pmax;
			        pp=log10(pp);
			    }else if(sType == 2){
			        if(pp < pmin)pp=pmin;
			        if(pp > pmax)pp=pmax;
			    }
			    ic=(unsigned char)(a*pp+b);
			    kpoint(ix,iy,ic,ss);
		        itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	}
	return 0;
	
}
static int IntLoc(struct FilePIOInfo *pio,double *x,double *y,int ix,int iy)
{

	if(!pio || !x || !y)return 1;
	if((pio->range.xScale == 0) || (pio->range.yScale == 0))return 1;

	*x=((double)(ix-pio->range.ixminData))/pio->range.xScale+pio->range.xminData;
	*y=((double)(iy-pio->range.iyminData))/pio->range.yScale+pio->range.yminData;

	return 0;
}

#endif

