/*
 *  uFilesBatch2dBlkMPI01.c
 *  xShow
 *
 *  Created by Dale Ranta on 11/17/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */

#define EXTERN22 extern

#include <mpi.h>
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
#include "Blocks.h"

int BlksReadMPI(MPI_File mpifile,struct FileInfo2 *Files,unsigned long flags,long nb,long BlockSize,long CurrentFrame);

int GetCurrentFilePath(struct FileInfo2 *Files,long CurrentFrame,char *filePath,long size);

int SendToOthers(void *data,int length,int source);

int SendToAll(void *data,int length);

int Sage2DGetDataBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData *sd);

int doSage2DGetLineDataBlk2(struct FileInfo2 *Files,struct linedata *li);

static int doSage2DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files);

static int intcmp1(const void *xx,const  void *yy);

int setFloat(double *d,long length);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

int doSage2D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

int doSage2DStart(struct FileInfo2 *Files);

int getPIOHalfCellSize2D(struct FilePIOInfo *pio,int level,  double size[2]);

static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int setPioScales(struct FilePIOInfo *pio);

static int doSage2DCheckBlks(struct FileInfo2 *Files,long CurrentFrame);

static int getReflectedData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

int doSage2DGetAreaDataBlk2(struct FileInfo2 *Files,struct areadata *ai);

static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int CheckVectorRange(struct FileInfo2 *Files);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int getPlotImage2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame);
static int pioDraw2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);


static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double *v,long *index);

static int doRangeBlock(struct FileInfo2 *Files);

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li);

static int doIntegrateLine(struct FileInfo2 *Files,struct linedata *li);

struct sortdata{
	double x;
	double y;
	double x2;
	double y2;
	double sl;
	double sh;
	long ne;
};
static int doBlockGradients(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame);

int findit(long count,double *level,double value,long *ic);

static int doCell(long ne,struct DrawInfo *info);

static int pioDrawStream2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

struct streamStruct{
   int doneFore;
   int doneBack;
   double xFore;
   double yFore;
   double xBack;
   double yBack;
   long stepsFore;
   long stepsBack;
   int foundFore;
   int foundBack;
};


static int pioDrawStream2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	MPI_File mpifile;
	struct streamStruct *stream;
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
    double xmin,xmax,ymin,ymax;
    double didx,djdy;
    double dxdi,dydj,dv;
    long ixmin,ixmax,iymin,iymax;
	double x,y,x1,y1,x2,y2,vx,vy,vlen,sx,sy;
	double *lo_x,*hi_x,*lo_y,*hi_y;
	double gradxV1,gradyV1;
	double gradxV2,gradyV2;
	double gradl,gradh;
	unsigned long flags;
	int ix,iy;
	long n,nn,ne,k,n2,k2,ke;
	int streamGradients;
    long streamSteps;
    long streamPasses;
	long count;
	long nstep;
	int idone;
	int ipass;
	int nGrad;
	int ret;
	long BlockSize,BlockSizeSave,numcell,numcellSave;
	long CurrentFrame;
	int nnn,kk;

	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	count=pio->streamline2d.zPlane.count;
	if(count < 1)return 0;
	
	ret = 1;
	
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;	
	flags=0;
	
	stream=(struct streamStruct *)cMalloc(count*sizeof(struct streamStruct),3871);
	if(!stream){
	    WarningBatch("pioDrawStream2d Out of List Memory\n");
	    goto ErrorOut;
	}
	
	
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
    streamPasses=pio->streamline2d.streamPasses;
    
    
    for(n=0;n<count;++n){
   		stream[n].doneFore=0;
   		stream[n].doneBack=0;
   		stream[n].xFore=pio->streamline2d.zPlane.x[n];
   		stream[n].yFore=pio->streamline2d.zPlane.y[n];
   		stream[n].xBack=pio->streamline2d.zPlane.x[n];
   		stream[n].yBack=pio->streamline2d.zPlane.y[n];
   		stream[n].stepsFore=0;
   		stream[n].stepsBack=0;
   		stream[n].foundBack=FALSE;
   		stream[n].foundFore=FALSE;
   		
    }
	
	/*
	streamGradients=FALSE;
	*/
	
	flags=B_2D_STANDARD | B_XVEL | B_YVEL;
	pio->doGradients = FALSE;
    if(streamGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
    }
	    
	BlockSize=pio->BlockSize;
	CurrentFrame=pio->CurrentFrame;
			    
	if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
		    	  
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
    setPioScales(pio);
	
 	if(BlksStart(Files,flags))goto ErrorOut;
	
   	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

    /*
    sprintf(WarningBuff,"Range xmin %g ymin %g xmax %g ymax %g\n",pio->xmin,pio->ymin,pio->xmax,pio->ymax);
    WarningBatch(WarningBuff);
    */
    
    ipass=0;
StartAgain:    

	for(n=0;n<count;++n){
	    stream[n].foundFore = FALSE;
	    stream[n].foundBack = FALSE;
	}


	numcell=numcellSave;
	BlockSize=BlockSizeSave;
	
	for(nnn=0;nnn<block->nBlocks;nnn += xg.size){
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nnn + kk) % (int)(block->nBlocks);
			int np=(nnn + kk) % xg.size;
			
			if(np == xg.rank){
				BlockSize=block->nBlockSize[nb];
					
				pio->numcell=BlockSize;
						
				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
							
				doRangeBlock(Files);
		/*
				if(pio->doGradients){		
					if(doBlockGradientsVel(Files,nff,BlockSize,CurrentFrame))goto ErrorOut;
				}
		*/
			
				lo_x=pio->next[0];
				hi_x=pio->next[1];
				lo_y=pio->next[2];
				hi_y=pio->next[3];
					
				nGrad=-1;
				gradxV1=0;
				gradyV1=0;
				gradxV2=0;
				gradyV2=0;
			
				for(n=0;n<count;++n){
					double v;
					for(k=0;k<2;++k){
						if(k == 0){
						   if(stream[n].doneFore)continue;
							x1=x=stream[n].xFore;
							y1=y=stream[n].yFore;
							nstep=stream[n].stepsFore;
						}else{
						   if(stream[n].doneBack)continue;
							x1=x=stream[n].xBack;
							y1=y=stream[n].yBack;
							nstep=stream[n].stepsBack;
						}
						ne=-1;
						nn=n;
		doAgain3:
						
						if(!pioGetValue(pio,x,y,&v,&ne)){
							continue;
						}
						if(k == 0){
							stream[n].foundFore=TRUE;
						}else{
							stream[n].foundBack=TRUE;
						}
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
								gradl = (pio->vectory[ne]-pio->vectory[n2])/(pio->dx2[ke]+pio->dx2[k2]);
								
								n2=(long)(hi_x[ne]-1);
								k2=(int)(pio->level[n2]);
								ke=(int)(pio->level[ne]);
								gradh = (pio->vectory[n2]-pio->vectory[ne])/(pio->dx2[ke]+pio->dx2[k2]);
								
								gradxV2 = 0.5*(gradl+gradh);

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
							if(k == 0){
							   stream[n].doneFore = TRUE;
							}else{
							   stream[n].doneBack = TRUE;
							}
							/*
							sprintf(WarningBuff,"Point  %g %g zero Velocity\n",x2,y2);
							WarningBatch(WarningBuff);
							*/
							continue;
						}    	
							
						x=x1=x2;
						y=y1=y2;
						if(++nstep > streamSteps){
							if(k == 0){
							/*
								sprintf(WarningBuff,"Point %ld  Fore %g %g exceed Steps\n",(long)n,x2,y2);
								WarningBatch(WarningBuff);
							*/
							   stream[n].doneFore = TRUE;
							}else{
							/*
								sprintf(WarningBuff,"Point %ld  Back %g %g exceed Steps\n",(long)n,x2,y2);
								WarningBatch(WarningBuff);
							*/
							   stream[n].doneBack = TRUE;
							}
							continue;
						}
						if(k == 0){
							stream[n].xFore=x;
							stream[n].yFore=y;
							stream[n].stepsFore=nstep;
						}else{
							stream[n].xBack=x;
							stream[n].yBack=y;
							stream[n].stepsBack=nstep;
						}
						goto doAgain3;
					}
				}
	        }
	    }
	}

	for(n=0;n<count;++n){
	    int foundFore,foundBack,iAm,iAmR;
		iAmR=0;
		if(stream[n].foundFore || stream[n].foundBack)iAmR=xg.rank;
		MPI_Reduce ( &stream[n].foundFore, &foundFore, 1, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		MPI_Reduce ( &stream[n].foundBack, &foundBack, 1, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		MPI_Reduce ( &iAmR, &iAm, 1, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		SendToAll(&iAm,sizeof(iAm));
		SendToAll(&foundFore,sizeof(foundFore));
		SendToAll(&foundBack,sizeof(foundBack));
		if((foundFore || foundBack)){
			SendToOthers(&stream[n],sizeof(struct streamStruct),iAm);
		}
	}
	
	
	idone=0;
	for(n=0;n<count;++n){
		if(!stream[n].foundFore){
		    stream[n].doneFore=TRUE;
		}
		if(!stream[n].foundBack){
		    stream[n].doneBack=TRUE;
		}
		if(stream[n].doneFore)idone += 1;
		if(stream[n].doneBack)idone += 1;
	}
	
	++ipass;
	
	if(idone < 2*count && ipass < streamPasses)goto StartAgain;
	
	if(idone < 2*count){
		sprintf(WarningBuff," %ld pass(es) %ld Streamsline pieces finished out of %ld for Frame %ld\n",(long)ipass,
		                     (long)idone,(long)count*2,CurrentFrame);
		WarningBatch(WarningBuff);
	}
	

	ret=0;
ErrorOut:
	if(ret != 0){
	    fprintf(stderr,"pioDrawStream2d Error Exit\n");
	}
	MPI_File_close(&mpifile);
	
	BlksEnd(Files,flags);
    if(stream)cFree((char *)stream);
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;

	return ret;
}
static int doCell(long ne,struct DrawInfo *info)
{
    struct FilePIOInfo *pio;
	double r1,r2,s1,s2,dx,dy,dr,ds;
	double xc,yc,dx2,dy2;
	double x,y,r,s,rx,ry,sx,sy;
	long i1,i2,j1,j2;
	long k,nx,ny;
	long i,j;
/*
	long n,kk;
*/
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
    
/*    
    if(pio->daughter[ne]){   
		for(kk=0;kk<4;++kk){		
	        n=(kk+(long)(pio->daughter[ne]-1));	        
			doCell(n,info);
		}
		return 0;
	}
*/
	
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
	MPI_File mpifile;
    struct FilePIOInfo *pio;
	struct DrawInfo info;
	unsigned long flags;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	long CurrentFrame;
	long BlockSize;
    long length,nn,n;		
	int ret;
	
	
	if(!ai || !ai->data || !Files)return 1;
	pio=&Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	
	ret=1;
	
	CurrentFrame=ai->CurrentFrame;
	
	length=ai->xsize*ai->ysize;
	
	for(n=0;n<length;++n)ai->data[n]=-1e66;
		
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
			
	flags = B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}
	
	
	if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

	if(BlksStart(Files,flags))goto ErrorOut;

    setPioScales(pio);
    
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
	
	for(nn=0;nn<block->nBlocks;nn += xg.size){
        int kk;
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nn + kk) % (int)(block->nBlocks);
			int np=(nn + kk) % xg.size;
		
			if(np == xg.rank){
		
				BlockSize=block->nBlockSize[nb];
				pio->numcell=BlockSize;
						
				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
				
				doRangeBlock(Files);
						
				if(pio->doGradients){		
					if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
					if(!pio->gradx || !pio->grady){
						pio->doGradients = FALSE;
					}
				}

				for(n=0;n<BlockSize;++n){		
					if(pio->daughter[n])continue;	        
					if(doCell(n,&info))break;	        
				}
			}
		}
	}
	
	MPI_File_close(&mpifile);
    
	{
		double *soutg;
		
		soutg=NULL;
		if(xg.rank == xg.root){
			soutg=(double *)cMalloc(length*sizeof(double),9786);
			if(!soutg)goto ErrorOut;
		}
	
		MPI_Reduce ( ai->data, soutg, length, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		if(xg.rank == xg.root){
			if(ai->data)cFree((char *)ai->data);
			ai->data=soutg;
		}
			
	}
    ret = 0;
	
ErrorOut:
	BlksEnd(Files,flags);
	return  ret;
}
static int doBlockGradients(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	double *next;
	long n,n2;
	int ret;
	int k,k2;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	
	ret = 1;
	
	zerol((char *)pio->grady,sizeof(double)*BlockSize);
	
	zerol((char *)pio->gradx,sizeof(double)*BlockSize);

	for(n=0;n<BlockSize;++n){
	    pio->gradl[n]=0;
	    pio->gradh[n]=0;
	}

	
	next=pio->next[0];
	
	for(n=0;n<BlockSize;++n){         /* lo_x */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        pio->gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dx2[k]+pio->dx2[k2]);
	    }else{
	        pio->gradl[n] = 0;
	    }
	}
	
	next=pio->next[1];
	
	for(n=0;n<BlockSize;++n){        /* hi_x */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        pio->gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dx2[k]+pio->dx2[k2]);
	    }else{
	        pio->gradh[n] = 0;
	    }
	}
	
	
	for(n=0;n<BlockSize;++n){
		if(!pio->daughter[n] && (pio->gradh[n]*pio->gradl[n] >= 0.0)){
			if(fabs(pio->gradh[n]) < fabs(pio->gradl[n])){
				pio->gradx[n] = pio->gradh[n];
			}else{
				pio->gradx[n] = pio->gradl[n];
			}
		}else{
			pio->gradx[n] = 0;
		}
	    pio->gradh[n]  = 0;
	    pio->gradl[n] = 0;
	}
	
	
	next=pio->next[2];
	
	for(n=0;n<BlockSize;++n){        /* lo_y */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        pio->gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dy2[k]+pio->dy2[k2]);
	    }else{
	        pio->gradl[n] = 0;
	    }
	}

	next=pio->next[3];
	
	for(n=0;n<BlockSize;++n){        /* hi_y */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        pio->gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dy2[k]+pio->dy2[k2]);
	    }else{
	        pio->gradh[n] = 0;
	    }
	}


	for(n=0;n<BlockSize;++n){
		if(!pio->daughter[n] && (pio->gradh[n]*pio->gradl[n] >= 0.0)){
			if(fabs(pio->gradh[n]) < fabs(pio->gradl[n])){
				pio->grady[n] = pio->gradh[n];
			}else{
				pio->grady[n] = pio->gradl[n];
			}
		}else{
			pio->grady[n] = 0;
		}
	}

	
	ret = 0;
	
/*	
	if(level)cFree((char *)level);
*/	
	if(ret){
		if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=NULL;

		if(pio->grady)cFree((char *)pio->grady);
		pio->grady=NULL;
	}
		
	return ret;
}

static int doIntegrateLine(struct FileInfo2 *Files,struct linedata *li)
{
	MPI_File mpifile;
	long nff,BlockSize,BlockSizeSave,numcell,numcellSave;
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	double *xD,*yD,s,s1,s2,s3,s4,dx2,dy2;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral;
	double length;
	long nd,np,ne,n,ns,nn;
	long np1,np2;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	double llength;
	long CurrentFrame;
	struct sortdata *sort;
	double rx,ry,mx,yy,xx;
	double sl,sh,dl;
	int iDone;
	int i1,i2;
	int ihit;
	int ret;
	
	if(!li)return 1;
	if(!Files)return 1;	
	pio=&Files->pioData;
	
	ret=1;
    xD=NULL;
    yD=NULL;
    sort=NULL;
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;
	
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

	
	llength=sqrt(rx*rx+ry*ry);	
	
	mx = 1e-6*llength;

/*
    if(fabs(rx) < mx){
    	rx=0;
    }
	
    if(fabs(ry) < mx){
    	ry=0;
    }
*/
	
	pio->doGradients = FALSE;

    setPioScales(pio);
		
	np=10000;
	
	if(!(sort=(struct sortdata *)cMalloc((long)sizeof(struct sortdata)*(np),81876))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	
	CurrentFrame=li->CurrentFrame;
	
	BlockSize=pio->BlockSize;
		    		    
	if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

	if(BlksStart(Files,B_2D_STANDARD))goto ErrorOut;
	
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
	
	np1=-1;
	np2=-1;
	
	numcell=pio->numcell;
	BlockSize=pio->BlockSize;
	
	
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

 	nff=0;
	for(nn=0;nn<block->nBlocks;nn += xg.size){
        int kk;
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nn + kk) % (int)(block->nBlocks);
			int np=(nn + kk) % xg.size;
		
			if(np == xg.rank){
	
				BlockSize=block->nBlockSize[nb];
				
				pio->numcell=BlockSize;
				
				if(BlksReadMPI(mpifile,Files,B_3D_STANDARD,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
				
			doRangeBlock(Files);

			dx2=pio->xmax-pio->xmin;
			dy2=pio->ymax-pio->ymin;
			
			xmin=pio->xmin-dx2*1.0000001;
			xmax=pio->xmax+dx2*1.0000001;
			ymin=pio->ymin-dy2*1.0000001;
			ymax=pio->ymax+dy2*1.0000001;
			
			i1=0;
			i2=0;
			if(x1 < xmin || x1 > xmax)i1=1;
			if(x2 < xmin || x2 > xmax)i2=1;
			if(y1 < ymin || y1 > ymax)i1=1;
			if(y2 < ymin || y2 > ymax)i2=1;
			if(i1 == 0 || i2 == 0){
				goto Inside1;
			}
			
			ihit=0;
			if(fabs(rx) > mx){
				s1=((pio->xmin)-x1)/rx;
				yy=y1+s1*ry;
				if(yy < ymin || yy > ymax){
					;
				}else{
					if(s1 >= 0 && s1 <= 1.0)ihit |= 1;   /* hit low x */
				}	        
				s2=((pio->xmax)-x1)/rx;
				yy=y1+s2*ry;
				if(yy < ymin || yy > ymax){
					;
				}else{
					if(s2 >= 0 && s2 <= 1.0)ihit |= 2;   /* hit hi x */
				}	
			}        
				
			if(fabs(ry) > mx){
				s3=((pio->ymin)-y1)/ry;
				xx=x1+s3*rx;	                
				if(xx < xmin || xx > xmax){
					;
				}else{
					if(s3 >= 0 && s3 <= 1.0)ihit |= 4;   /* hit low y */
				}	        
				s4=((pio->ymax)-y1)/ry;
				xx=x1+s4*rx;	                
				if(xx < xmin || xx > xmax){
					;
				}else{
				   if(s4 >= 0 && s4 <= 1.0)ihit |= 8;    /* hit hi y */
				}	        
			}
			
			if(ihit == 0){
			/*
				sprintf(WarningBuff,"Block Skipped %ld\n",nff);
				WarningBatch(WarningBuff);
			*/
				goto NextBlock;
			}
	/*   
		No Gradients for integrated lines
		
	*/
			
	Inside1:
			for(ne=0;ne<pio->numcell;++ne){
				double value,xc,yc,dl1;
				double s;
				long k;
				if(pio->daughter[ne] || !pio->active[ne])continue;
				
				k=(long)(pio->level[ne]);
				xc=pio->xcenter[ne];
				dx2=pio->dx2[k];
				xmin=xc-dx2*1.0000001;
				xmax=xc+dx2*1.0000001;
				yc=pio->ycenter[ne];
				dy2=pio->dx2[k];
				ymin=yc-dy2*1.0000001;
				ymax=yc+dy2*1.0000001;
				
				s1=s2=s3=s4=-1;
				
				i1=0;
				i2=0;
				if(x1 < xmin || x1 > xmax)i1=1;
				if(x2 < xmin || x2 > xmax)i2=1;
				if(y1 < ymin || y1 > ymax)i1=1;
				if(y2 < ymin || y2 > ymax)i2=1;
				if(i1 == 0 && i2 == 0){
				  sl=0.0;
				  sh=1.0;
				  goto BothInside;
				}
				
				ihit=0;
				if(fabs(rx) > mx){
					s1=((xc-dx2)-x1)/rx;
					yy=y1+s1*ry;
					if(yy < ymin || yy > ymax){
						;
					}else{
						if(s1 >= 0 && s1 <= 1.0)ihit |= 1;   /* hit low x */
					}	        
					s2=((xc+dx2)-x1)/rx;
					yy=y1+s2*ry;
					if(yy < ymin || yy > ymax){
						;
					}else{
						if(s2 >= 0 && s2 <= 1.0)ihit |= 2;   /* hit hi x */
					}	
				}        
					
				if(fabs(ry) > mx){
					s3=((yc-dy2)-y1)/ry;
					xx=x1+s3*rx;	                
					if(xx < xmin || xx > xmax){
						;
					}else{
						if(s3 >= 0 && s3 <= 1.0)ihit |= 4;   /* hit low y */
					}	        
					s4=((yc+dy2)-y1)/ry;
					xx=x1+s4*rx;	                
					if(xx < xmin || xx > xmax){
						;
					}else{
					   if(s4 >= 0 && s4 <= 1.0)ihit |= 8;    /* hit hi y */
					}	        
				}
				
						
				
				
				if(ihit == 0){		    
					continue;		        
				}
	/*		    
				sprintf(WarningBuff,"ne %ld ",ne+nff);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"ihit %d ",ihit);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"i1 %d ",i1);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"i2 %d ",i2);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"s1 %g ",s1);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"s2 %g ",s2);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"s3 %g ",s3);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"s4 %g \n",s4);
				WarningBatch(WarningBuff);
	*/

				sl=sh=-1;
				if(i1 == 1 && i2 == 1){  /* both points outside */
					switch(ihit){
						case 15: /* lo x & hi x & lo y  & hi y*/
						case 11: /* lo x & hi x & hi y */
						case 7: /* lo x & hi x & lo y */
						case 3: /* lo x & hi x */
						sl=s1;
						sh=s2;
						break;
						case 5: /* lo x & lo y */
						sl=s1;
						sh=s3;
						break;
						case 9: /* lo x & hi y */
						sl=s1;
						sh=s4;
						break;
						
						case 6: /* hi x & lo y */
						sl=s2;
						sh=s3;
						break;
						
						case 10: /* hi x & hi y */
						sl=s2;
						sh=s4;
						break;

						case 14: /* lo y & hi y  & hi x*/
						case 13: /* lo y & hi y  & lo x*/
						case 12: /* lo y & hi y */
						sl=s3;
						sh=s4;
						break;


						case 1:
						case 2:
						case 4:
						case 8:
						case 0:
						default:
							goto NextCell; /* line did not hit this cell */
					}
				}else if(i1 == 1){  /* point 1 outside */
					switch(ihit){
						case 15: /* lo x & hi x & lo y  & hi y*/
						case 11: /* lo x & hi x & hi y */
						case 7: /* lo x & hi x & lo y */
						case 3: /* lo x & hi x */
						sl=s1;
						if(s2 < sl)sl=s2;
						sh=1;
						break;
						case 5: /* lo x & lo y */
						sl=s1;
						if(s3 < sl)sl=s3;
						sh=1;
						break;
						case 9: /* lo x & hi y */
						sl=s1;
						if(s4 < sl)sl=s4;
						sh=1;
						break;
						
						case 6: /* hi x & lo y */
						sl=s2;
						if(s3 < sl)sl=s3;
						sh=1;
						break;
						case 10: /* hi x & hi y */
						sl=s2;
						if(s4 < sl)sl=s4;
						sh=1;
						break;

						case 14: /* lo y & hi y  & hi x*/
						case 13: /* lo y & hi y  & lo x*/
						case 12: /* lo y & hi y */
						sl=s3;
						if(s4 < sl)sl=s4;
						sh=1;
						break;


						case 1:
						sl=s1;
						sh=1;
						break;
						case 2:
						sl=s2;
						sh=1;
						break;
						case 4:
						sl=s3;
						sh=1;
						break;
						case 8:
						sl=s4;
						sh=1;
						break;
						case 0:
						default:
							goto NextCell; /* line did not hit this cell */
					}
				}else{              /* point 2 outside */
					switch(ihit){
						case 15: /* lo x & hi x & lo y  & hi y*/
						case 11: /* lo x & hi x & hi y */
						case 7: /* lo x & hi x & lo y */
						case 3: /* lo x & hi x */
						sh=s1;
						if(s2 > sh)sh=s2;
						sl=0;
						break;
						case 5: /* lo x & lo y */
						sh=s1;
						if(s3 > sh)sh=s3;
						sl=0;
						break;
						case 9: /* lo x & hi y */
						sh=s1;
						if(s4 > sh)sh=s4;
						sl=0;
						break;
						
						case 6: /* hi x & lo y */
						sh=s2;
						if(s3 > sh)sh=s3;
						sl=0;
						break;
						case 10: /* hi x & hi y */
						sh=s2;
						if(s4 > sh)sh=s4;
						sl=0;
						break;

						case 14: /* lo y & hi y  & hi x*/
						case 13: /* lo y & hi y  & lo x*/
						case 12: /* lo y & hi y */
						sh=s3;
						if(s4 > sh)sh=s4;
						sl=0;
						break;


						case 1:
						sh=s1;
						sl=0;
						break;
						case 2:
						sh=s2;
						sl=0;
						break;
						case 4:
						sh=s3;
						sl=0;
						break;
						case 8:
						sh=s4;
						sl=0;
						break;
						case 0:
						default:
							goto NextCell; /* line did not hit this cell */
					}
				}
				
				if(i1 == 0){
					np1=ne+nff;
				}
				
				if(i2 == 0){
					np2=ne+nff;
				}
				
	BothInside:		    
				if(sl > sh){
					s=sl;
					sl=sh;
					sh=s;
				}
				
	/*			
				sprintf(WarningBuff,"sl %g ",sl);
				WarningBatch(WarningBuff);
				
				sprintf(WarningBuff,"sh %g \n",sh);
				WarningBatch(WarningBuff);

	*/
			
				if(sl == sh)continue;
			
				value=pio->value[ne];
				
				if(nd+4 > np){
					struct sortdata *st;
										
					st=(struct sortdata *)cRealloc((char *)sort,2*np*sizeof(struct sortdata),3761);
					if(!st || (nd > 1000000)){
						sprintf(WarningBuff,"doIntegrateLine : Error Too many Points %ld\n",nd);
						WarningBatch(WarningBuff);
						if(st){
							cFree((char *)st);
							sort=NULL;
						}
						goto OutOfHere;
					}
					sort=st;
					np=2*np;
				}


				xo=x1+sl*rx;
				yo=y1+sl*ry;	
				
				dl1=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y));

				sort[nd].x=dl1;
				sort[nd].y=value;
				sort[nd].ne=ne+nff;
				sort[nd].sl=sl;
				sort[nd].sh=sh;

				if(dl1 < rxmin)rxmin=dl1;
				if(dl1 > rxmax)rxmax=dl1;
				if(value < rymin)rymin=value;
				if(value > rymax)rymax=value;

				xo=x1+sh*rx;
				yo=y1+sh*ry;	
				
				dl=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y));

				sort[nd].x2=dl;
				sort[nd].y2=value;
				sort[nd].ne=ne+nff;
				sort[nd].sl=sl;
				sort[nd].sh=sh;
				nd++;

				if(dl < rxmin)rxmin=dl;
				if(dl > rxmax)rxmax=dl;
				if(value < rymin)rymin=value;
				if(value > rymax)rymax=value;
				
						
	NextCell:
				;
			}
		}
NextBlock:	
			nff += BlockSize;
		}
	}
		
	
	MPI_File_close(&mpifile);
	
	{
		int *nf,*nff,len,nps;
		MPI_Status status;
		
		MPI_Reduce ( &np1, &nps, 1, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		np1=nps;

		MPI_Reduce ( &np2, &nps, 1, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		np2=nps;

		nf=(int *)cMalloc(xg.size*sizeof(int),8276);
		if(!nf)goto ErrorOut;
		nff=(int *)cMalloc(xg.size*sizeof(int),8276);
		if(!nff)goto ErrorOut;
		for(n=0;n<xg.size;++n){
		   nf[n]=0;
		   if(n == xg.rank)nf[n]=nd;
		}
		
		MPI_Reduce ( nf, nff, xg.size, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		SendToAll(nff,xg.size*sizeof(int));
/*
		if(xg.rank == xg.root){
			fprintf(stderr,"nff ");
		   for(n=0;n<xg.size;++n){
		      fprintf(stderr," %ld ",(long)nff[n]);
		   }
		   fprintf(stderr,"\n");
		}
*/

		MPI_Barrier(MPI_COMM_WORLD);
		
		len=nff[0];
		for(n=1;n<xg.size;++n){
		   if(nff[n] > 0){
		        if(n == xg.rank){
					MPI_Send(sort, nff[n]*sizeof(struct sortdata), MPI_BYTE, xg.root, 1, MPI_COMM_WORLD);
					/* fprintf(stderr,"send send send done done -----5 rank %ld nd %ld\n",(long)xg.rank,(long)nff[n]); */
				}
		   }
		   len += nff[n];
		}
				
		len=nff[0];
		for(n=1;n<xg.size;++n){
		   if(nff[n] > 0){
		        if(xg.root == xg.rank){
					MPI_Recv(sort+len, nff[n]*sizeof(struct sortdata), MPI_BYTE, n, 1, MPI_COMM_WORLD,&status);
					/* fprintf(stderr,"receive receive receive done done ---- 6 rank %ld nd %ld\n",(long)xg.rank,(long)nff[n]); */
				}
				
				
		   }
		   len += nff[n];
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
		
		if(xg.root == xg.rank){
			nd=len;
		}else{
			nd=0;
		}
		cFree((char *)nf);
		cFree((char *)nff);
	}
	
	if(!nd)goto OutOfHere;
	
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(nd*2),81876))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(nd*2),81883))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}	
	
	qsort((char *)sort,nd, sizeof(struct sortdata),intcmp1);
	
	n=0;
	sl=0;
	np=0;
	iDone=FALSE;
	length=0;
	integral=0;
	do{
		int ifound;
	


		dl = sort[np].x2-sort[np].x;
		
/*		
		
			sprintf(WarningBuff,"test %ld %ld x %f x2 %f  dl %f sl %f sh %f\n",np,sort[np].ne,sort[np].x,sort[np].x2,dl,sort[np].sl,sort[np].sh);
			WarningBatch(WarningBuff);
*/		
			
		if(dl <= llength*1e-8)continue;

		if(fabs(sort[np].sh - 1.0) < 1e-6){
			xD[n]=sort[np].x;
			yD[n++]=sort[np].y;

			xD[n]=sort[np].x2;
			yD[n++]=sort[np].y2;

			integral += sort[np].y*(sort[np].x2-sort[np].x);
			length += (dl);
			iDone=TRUE;
			goto Done;
		}

		
		if(fabs(sort[np].sl - sl) < 1e-6){    		/* found first point */
		    sh=sort[np].sh;
		    ns=np+1;
		    ifound=FALSE;
		    while(1){
		    	if(fabs(sort[ns].sl - sh) < 1e-6){   /* found other end */
		    		sl=sh;
		    		ifound=TRUE;
					xD[n]=sort[np].x;
					yD[n++]=sort[np].y;

					xD[n]=sort[np].x2;
					yD[n++]=sort[np].y2;

					integral += sort[np].y*(sort[np].x2-sort[np].x);
					length += (dl);
					
/*					
			sprintf(WarningBuff,"save %ld %ld x %f x2 %f  dl %f sl %f sh %f\n",np,sort[np].ne,sort[np].x,sort[np].x2,dl,sort[np].sl,sort[np].sh);
			WarningBatch(WarningBuff);
*/					
					
					
		    		break;
		    	}
				if(++ns > nd)break;		    
		    }
		    if(ifound){
		    	np=ns-1;
		    }
		}
	}while(++np < nd);
	
Done: 
		if(!iDone){
		     sprintf(WarningBuff,"doIntegrateLine Line Not Complete\n");
		     WarningBatch(WarningBuff);
		}
		
	

	li->dCount=n;    
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
	xD=NULL;
	yD=NULL;
	ret=0;
ErrorOut:
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(sort)cFree((char *)sort);
	
	BlksEnd(Files,B_2D_STANDARD);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	
	return ret;
}
int intcmp1(const void *xx,const  void *yy)
{
	double sum;
	register struct sortdata *x=(struct sortdata *)xx;
	register struct sortdata *y=(struct sortdata *)yy;
	sum = x->x - y->x;
	if(sum == 0.0){
		sum = x->x2 - y->x2;
	    if(sum == 0.0)return 0;
	}
	return (int)((sum > 0.0) ? 1 : -1);
}
static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
	MPI_File mpifile;
	long BlockSize,BlockSizeSave,numcell,numcellSave;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
    struct FilePIOInfo *pio;
	long CurrentFrame;
	unsigned long flags;
	long index;
	long n,nn;
	int ret;
	
	if(!Files || !li)return 1;
	pio=&Files->pioData;
	
	if(li->type != LINEDATA_LIST_DATA){
	     return 1;
	}
	
	ret = 1;
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;
		
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;

	CurrentFrame=li->CurrentFrame;
		
	flags=B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}
	
	BlockSize=pio->BlockSize;
		
		
	if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

    setPioScales(pio);
	
	if(BlksStart(Files,flags))goto ErrorOut;
	
	
	for(n=0;n<li->PointLineCount;++n){	    
		li->v[n]= -2e66;
	}
	
/*	
	pio->pa.flagGradients=TRUE;
	
 */   
 
		MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

    	numcell=pio->numcell;
		BlockSize=pio->BlockSize;

		for(nn=0;nn<block->nBlocks;nn += xg.size){
			int kk;
			for(kk=0;kk<xg.size;++kk){
		
				int nb=(nn + kk) % (int)(block->nBlocks);
				int np=(nn + kk) % xg.size;
			
				if(np == xg.rank){
					BlockSize=block->nBlockSize[nb];
					pio->numcell=BlockSize;
					
					if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
				
					doRangeBlock(Files);

					if(pio->doGradients){		
						if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
						if(!pio->gradx || !pio->grady){
							pio->doGradients = FALSE;
						}
					}
					
					for(n=0;n<li->PointLineCount;++n){
									
						if(!pioGetValue(pio,li->x[n],li->y[n],&li->v[n],&index)){
							continue;
						}	    
					}
				}
			}
		}
		
		
		MPI_File_close(&mpifile);
		
		{
			double *data=NULL;
			long ifound=0;
			
			if(xg.root == xg.rank){
			   data=cMalloc(sizeof(double)*li->PointLineCount,9891);
			   if(!data)goto ErrorOut;
			}
			
			MPI_Reduce ( li->v, data, li->PointLineCount, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD);
			
			if(xg.root == xg.rank){
				for(n=0;n<li->PointLineCount;++n)li->v[n]=data[n];
			}
			
			ifound=0;
			for(n=0;n<li->PointLineCount;++n){	    
				if(li->v[n] < -1e66){
					li->v[n]=0;
				}else{
					++ifound;
				}
			}
			
			if(data)cFree((char *)data);

		}
		
	ret = 0;
ErrorOut:

	BlksEnd(Files,flags);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	
	return ret;
}


static int doRangeBlock(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax;
	double xx,dx,yy,dy;
	long n;
	int k;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!pio->daughter || !pio->xcenter || !pio->ycenter)return 1;
	if(!pio->active || !pio->level)return 1;
	
	xmin=1e60;
	ymin=1e60;
	ymax=-1e60;
	xmax=-1e60;
	
	for(n=0;n<pio->numcell;++n){
	    if(pio->daughter[n] || !pio->active[n])continue;
        k=(int)(pio->level[n]);
        xx=pio->xcenter[n];
        dx=pio->dx2[k];
        if(xx-dx < xmin)xmin=xx-dx;
        if(xx+dx > xmax)xmax=xx+dx;
        yy=pio->ycenter[n];
        dy=pio->dy2[k];
        if(yy-dy < ymin)ymin=yy-dy;
        if(yy+dy > ymax)ymax=yy+dy;
	}
	
	pio->xmin=xmin;
	pio->ymin=ymin;
	pio->xmax=xmax;
	pio->ymax=ymax;
	
	return 0;
}


int doSage2DGetLineDataBlk2(struct FileInfo2 *Files,struct linedata *li)
{
	MPI_File mpifile;
	int ix1,iy1,ix2,iy2;
	double x1,y1,x2,y2,dl,ds,dx,dy,dp;
	long ns,n,nd;
	double *xD,*yD,*zD,*fD,*fDD;
	double rxmin,rxmax,rymin,rymax,amax;
	long BlockSize,BlockSizeSave,numcell,numcellSave;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	unsigned long flags;
	long index,nn;
	int ret;
	
	if(!Files || !li)return 1;
	pio=&Files->pioData;
	
	
	if(li->type == LINEDATA_POINT_DATA){
	     return doPointGetData(Files,li);
	}else if(li->type == LINEDATA_LIST_DATA){
	     return doPointGetListData(Files,li);
	}
	
	ret = 1;
    xD=NULL;
    yD=NULL;
    zD=NULL;
    fD=NULL;
	fDD=NULL;
	
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA)return 1;
	
	
	if(li->integrate){	
	    return doIntegrateLine(Files,li);
	}
	

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
		
	
	flags=B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}
	
	BlockSize=pio->BlockSize;
			    
	if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

    setPioScales(pio);
	
	if(BlksStart(Files,flags))goto ErrorOut;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81877))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}


	if(!(zD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
	    goto OutOfHere;
	}

	if(!(fD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81883))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}

	if(!(fDD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81883))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}

	for(n=0;n<ns;++n){
	    double x,y;

	    x=x1+(double)n*ds*(x2-x1);
	    y=y1+(double)n*ds*(y2-y1);
	    
	    xD[n]=x;
	    yD[n]=y;
	    zD[n]=0;
	    fD[n]=0;

	}

	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	numcell=pio->numcell;
	BlockSize=pio->BlockSize;
		
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
	
	for(nn=0;nn<block->nBlocks;nn += xg.size){
        int kk;
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nn + kk) % (int)(block->nBlocks);
			int np=(nn + kk) % xg.size;
		
			if(np == xg.rank){
				BlockSize=block->nBlockSize[nb];
				pio->numcell=BlockSize;
					
				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
							
				doRangeBlock(Files);

				if(pio->doGradients){		
					if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
					if(!pio->gradx || !pio->grady){
						pio->doGradients = FALSE;
					}
				}

				for(n=0;n<ns;++n){
					double value,x,y;
					if(fD[n] > 0)continue;
					x=xD[n];
					y=yD[n];

					
					if(!pioGetValue(pio,x,y,&value,&index)){
						continue;
					}

					dx=x-x1;
					dy=y-y1;
					dp=sqrt(dx*dx+dy*dy);

					xD[n]=dp;
					yD[n]=value;
					if(dp < rxmin)rxmin=dp;
					if(dp > rxmax)rxmax=dp;
					if(value < rymin)rymin=value;
					if(value > rymax)rymax=value;
					fD[n]=1.0;
				}	
			}
		}
	}

	MPI_File_close(&mpifile);

	MPI_Barrier(MPI_COMM_WORLD);
	
	
	for(n=0;n<ns;++n){
	    if(fD[n] < 1.0){
	       xD[n]=-1e66;
	       yD[n]=-1e66;
	    }
	}
	
	MPI_Reduce ( fD, fDD, ns, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
	
	for(n=0;n<ns;++n)fD[n]=fDD[n];
	
	MPI_Reduce ( xD, fDD, ns, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
	
	for(n=0;n<ns;++n)xD[n]=fDD[n];

	MPI_Reduce ( yD, fDD, ns, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
	
	for(n=0;n<ns;++n)yD[n]=fDD[n];
	
	nd=0;
	for(n=0;n<ns;++n){
	    if(fD[n] > 0){
	       xD[nd]=xD[n];
	       yD[nd]=yD[n];
	       ++nd;
	    }
	}
	
	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	xD=NULL;
	yD=NULL;
	
	ret = 0;
ErrorOut:
OutOfHere:
	if(xD)cFree((char *)xD);
	
	if(yD)cFree((char *)yD);
	
	if(zD)cFree((char *)zD);
	
	if(fDD)cFree((char *)fDD);
		
	if(fD)cFree((char *)fD);

	BlksEnd(Files,flags);
        
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;

	return ret;
}

static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double *v,long *index)
{
	double xc,yc;
	struct PIO_BLK *sage;
	double xx,yy;
	long ne;

	double xmin,ymin;

	if(!pio || !v || !pio->value)return 0;
	if(!pio->daughter)return 0;
	sage=&pio->sageBlk;
	if(!sage)return 0;
		
	if(pio->pa.xReflect){
	   x=fabs(x);
	}
	
	xmin=pio->xmin;
	if(x < xmin || x > pio->xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin || y > pio->ymax)return 0;

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
			*v=pio->value[ne]+pio->gradx[ne]*(x-xc)+pio->grady[ne]*(y-yc);
		}else{
			*v=pio->value[ne];
		}
		if(index){
		    *index=ne;
		}
		return 1;
    
    }
    return 0;
}
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li)
{
	MPI_File mpifile;
	double dp;
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt;
	double r,theta,dr,dtheta;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long BlockSize,numcell,nn;
	double valuet;
	double x,y;
	double xs,ys;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
    long index;
    unsigned long flags;
    int ret;
  

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	ret = 1;
	xD=NULL;
	yD=NULL;
	flags=0;
	
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

	flags=B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
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
	
	BlockSize=pio->BlockSize;
		
		    	  	
	if(doSage2DCheckBlks(Files,nFirst))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
    setPioScales(pio);	

	if(BlksStart(Files,flags))goto ErrorOut;
	
	xs=x;
	ys=y;
	nd=0;

	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    
		if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
		
 		sage=&pio->sageBlk;
		if(!sage)goto ErrorOut;
		block=&sage->block;

		MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
		
    	numcell=pio->numcell;
		BlockSize=pio->BlockSize;
    	
    	nf=0;
    	valuet=0;	    	    
		for(nn=0;nn<block->nBlocks;nn += xg.size){
			int kk;
			for(kk=0;kk<xg.size;++kk){
				int nb=(nn + kk) % (int)(block->nBlocks);
				int np=(nn + kk) % xg.size;
				if(np == xg.rank){

					BlockSize=block->nBlockSize[nb];
					
					pio->numcell=BlockSize;
					
					if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
			
					doRangeBlock(Files);
					
					if(pio->doGradients){		
						if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
						if(!pio->gradx || !pio->grady){
							pio->doGradients = FALSE;
						}
					}
					
					x=xs;
					y=ys;
					r=dr;
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
					if(nf >= NumberOfRings*NumberOfPoints)goto breakOut;
				}
			}
		}
breakOut:		
		
		pio->numcell=numcell;
    	    
		MPI_File_close(&mpifile);

		{
			double valuetRoot=0;
			int nfRoot=0;
			
			MPI_Reduce(&nf, &nfRoot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			MPI_Reduce(&valuet, &valuetRoot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
			nf=nfRoot;
			valuet=valuetRoot;
		}
		
		if(xg.rank != xg.root)continue;
		
	    if(nf <= 0){
		   nf=0;
		   continue;
		}
	    
	    value=valuet/(double)nf;
	    	    
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(n >= 0 && n < Files->ImageCount){
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
	
	xD=NULL;
	yD=NULL;
	
	/*
			WarningBatchAtEnd(WarningBuff);        
    */
	ret = 0;
ErrorOut:
OutOfHere:
    if(xD)cFree((char *)xD);
    if(yD)cFree((char *)yD);
    
	BlksEnd(Files,flags);
            
	return ret;
}

static void addem2 ( unsigned char *, unsigned char *, int *, MPI_Datatype * );

static void addem2(unsigned char *in, unsigned char *out, int *len, MPI_Datatype *dtype)
{
	long count;
	int i;
		
	count=0;
	for ( i=0; i<*len; i++ ){
	    
	    if(in[i] != 0){
		   out[i] = in[i];
			++count;
		}
		
	}
	
	/* fprintf(stderr,"addem2 rank %ld len %ld count %ld in %p out %p\n",(long)xg.rank,(long)*len,count,in,out); */
}

static int pioDraw2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
	MPI_File mpifile;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
    static double  small2=1e-8;
	struct dRange rr;
	long BlockSize,numcell;
	unsigned long flags;
	int ret;
	double *d;
	long nn,n;
	long length;
	
	unsigned char *sout2;
	unsigned char *sout3;
	MPI_Op op;

	if(!Files || !ss || !buff || !data)return 1;

	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	
	pio->CurrentFrame=CurrentFrame;
	
	BlockSize=pio->BlockSize;
	
	sout2=NULL;
	sout3=NULL;
	
	ret=1;
	
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

	}
	
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	
    
	if(!pio->streamline2d.streamOn && !pio->pa.DrawZones && !pio->vector.DrawVectors){
		return 0;
	}
	
    flags = B_2D_STANDARD;
	if(pio->vector.DrawVectors)flags |= (B_XVEL | B_YVEL);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}

	
	if(BlksStart(Files,flags))goto ErrorOut;
		    	  			
	if(pio->vector.DrawVectors){
		CheckVectorRange(Files);
	}
	
    numcell=pio->numcell;
    
 	if(xg.rank == xg.root){
		sout2=(unsigned char *)cMalloc(sizeof(double)*length,1122);
		sout3=(unsigned char *)cMalloc(sizeof(double)*length,1122);
		for(n=0;n<length;++n){
			sout3[n]=buff[n];
			sout2[n]=0;
		}
	}
	
	for(n=0;n<length;++n){
		buff[n]=0;
	}
		
	MPI_Op_create( (MPI_User_function *)addem2, 1, &op );
	
	
	if(pio->pa.DrawZones || pio->vector.DrawVectors){

		MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

		for(nn=0;nn<block->nBlocks;nn += xg.size){
			int kk;
			for(kk=0;kk<xg.size;++kk){
				int nb=(nn + kk) % (int)(block->nBlocks);
				int np=(nn + kk) % xg.size;
				
				if(np == xg.rank){

					BlockSize=block->nBlockSize[nb];
					pio->numcell=BlockSize;
						
					if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
											
					if(pio->doGradients){		
						if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
						if(!pio->gradx || !pio->grady){
							pio->doGradients = FALSE;
						}
					}
					
					if(pio->pd.AreaFill){
						pioDrawVoid(Files,(unsigned char *)buff);
					}
					
					if(pio->pa.DrawZones){
						pioDrawZones(pio,ss,&rr);
					}
				
					if(pio->vector.DrawVectors){
						pioDrawVectors(Files,ss,&rr);
					}
				}
			}
		}
		MPI_File_close(&mpifile);
	}
	pio->numcell=numcell;
   
		
	if(pio->streamline2d.streamOn){
		pioDrawStream2d(Files,ss,&rr);
	}
	

	MPI_Reduce (buff, sout2, length, MPI_BYTE, op, xg.root, MPI_COMM_WORLD );	
		
	MPI_Barrier(MPI_COMM_WORLD);
	if(xg.rank == xg.root){
		MPI_Datatype dtype=MPI_BYTE;
		int len=length;
		
		addem2(sout2, sout3, &len, &dtype);
		
		for(n=0;n<length;++n)buff[n]=sout3[n];
	}
	MPI_Op_free( &op );
	
	
	ret=0;
	
ErrorOut:

	BlksEnd(Files,flags);
	
 	if(sout2)cFree((char *)sout2);
	if(sout3)cFree((char *)sout3);
       
	return ret;
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
			buff2=cMalloc(pio->range.ixmax*pio->range.iymax*sizeof(double),1847);
			if(!buff2)goto ErrorOut;
			data2=cMalloc(pio->range.ixmax*pio->range.iymax*sizeof(double),1847);
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
 
 /*
 	if(SetPIOData(Files,CurrentFrame))goto ErrorOut;
*/
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
    
    sd->data2=cMalloc(length*sizeof(double),9205);
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
static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	double vmax,vmaxLen,vlength,vx,vy,dx,dy;
	double x1,y1,x2,y2,x3,y3;
	double pi,turn,o1,oa,arrow;
	double vmax2,degen,vspace;
	int ix,iy;
	long n;
	int ret;
	int nc;
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	
	ret = 1;
	
	   
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
		
	nc=255;
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
		        LineB(ix,iy,nc,ss);
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
			        LineB(ix,iy,nc,ss);
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
			        LineB(ix,iy,nc,ss);
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
				        LineB(ix,iy,nc,ss);
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
					        LineB(ix,iy,nc,ss);
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
					        LineB(ix,iy,nc,ss);
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
static int CheckVectorRange(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	MPI_File mpifile;
	double vmax,vmaxg,vlength,vx,vy;
	double xc,yc,dx2,dy2;
	long BlockSize,numcell;
	unsigned long flags;
	long n,nn;
	int ret;
	int k,kk;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	BlockSize=pio->BlockSize;
	
	ret = 1;
	
    numcell=pio->numcell;
    
    flags = (B_X | B_Y | B_DAUGHTER  | B_LEVEL | B_XVEL | B_YVEL);
    
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
	
	vmax=0;	    	
	vmaxg=0;	    
	for(nn=0;nn<block->nBlocks;nn += xg.size){
	
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nn + kk) % (int)(block->nBlocks);
			int np=(nn + kk) % xg.size;
			
			if(np == xg.rank){
				BlockSize=block->nBlockSize[nb];
				pio->numcell=BlockSize;

				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],pio->CurrentFrame))goto ErrorOut;
				for(n=0;n<BlockSize;++n){
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
			}
		}
	    	    	    
	}
	
	MPI_Reduce ( &vmax, &vmaxg, 1, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	SendToAll(&vmaxg,sizeof(vmaxg));
	
	pio->vmaxVectors=vmaxg;

	MPI_File_close(&mpifile);
	    
	ret=0;
ErrorOut:
	return ret;
}
	
static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
	MPI_File mpifile;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
    static double  small2=1e-8;
	struct dRange rr;
	long BlockSize,numcell,nn;
	unsigned long flags;
	double *d;
	long length;
	int ret,n;
	
	unsigned char *sout2;
	unsigned char *sout3;
	MPI_Op op;
	
	if(!Files || !ss || !buff)return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	
	pio->CurrentFrame=CurrentFrame;
	
	BlockSize=pio->BlockSize;
	
	sout2=NULL;
	sout3=NULL;
	
	ret=1;
	
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
	d=(double *)buff;
	setFloat(d,length);
    if(getPlotData(Files,d,CurrentFrame)){
	    sprintf(WarningBuff,"pioDraw - getPlotData Error\n");
	    WarningBatch(WarningBuff);
		return 1;
    }	

	if(pio->pd.AreaFill){	    	    
	    FloatToImage(d,length,&pio->pd,NULL);
	}else{
		zerol((char *)d,length*sizeof(double));
	}
	
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
    
 	if(!pio->streamline2d.streamOn && !pio->pa.DrawZones && !pio->vector.DrawVectors){
		return 0;
	}
   
    flags = B_2D_STANDARD;
	if(pio->vector.DrawVectors)flags |= (B_XVEL | B_YVEL);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}

	if(BlksStart(Files,flags))goto ErrorOut;
		    	  			
	if(pio->vector.DrawVectors){
		CheckVectorRange(Files);
	}
	
    numcell=pio->numcell;
    
    numcell=pio->numcell;
	
		
	if(xg.rank == xg.root){
		sout2=(unsigned char *)cMalloc(sizeof(double)*length,1122);
		sout3=(unsigned char *)cMalloc(sizeof(double)*length,1122);
		for(n=0;n<length;++n){
			sout3[n]=buff[n];
			sout2[n]=0;
		}
	}
	
	for(n=0;n<length;++n){
		buff[n]=0;
	}
	
	MPI_Op_create( (MPI_User_function *)addem2, 1, &op );
	
	if(pio->pa.DrawZones || pio->vector.DrawVectors){
	
		MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

		for(nn=0;nn<block->nBlocks;nn += xg.size){
			int kk;
			for(kk=0;kk<xg.size;++kk){
				int nb=(nn + kk) % (int)(block->nBlocks);
				int np=(nn + kk) % xg.size;
				
				if(np == xg.rank){
									
					BlockSize=block->nBlockSize[nb];
					pio->numcell=BlockSize;
					
					if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
											
					if(pio->doGradients){		
						if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
						if(!pio->gradx || !pio->grady){
							pio->doGradients = FALSE;
						}
					}
					
					if(pio->pd.AreaFill){
						pioDrawVoid(Files,(unsigned char *)buff);
					}
					
					if(pio->pa.DrawZones){
						pioDrawZones(pio,ss,&rr);
					}
				
					if(pio->vector.DrawVectors){
						pioDrawVectors(Files,ss,&rr);
					}
				}
			}
		}
	
	}
	
	pio->numcell=numcell;
   
	
	if(pio->streamline2d.streamOn){
		pioDrawStream2d(Files,ss,&rr);
	}
	
	MPI_Reduce (buff, sout2, length, MPI_BYTE, op, xg.root, MPI_COMM_WORLD );	
		
	MPI_Barrier(MPI_COMM_WORLD);
	if(xg.rank == xg.root){
		MPI_Datatype dtype=MPI_BYTE;
		int len=length;
		
		addem2(sout2, sout3, &len, &dtype);
		
		for(n=0;n<length;++n)buff[n]=sout3[n];
	}
	
	MPI_Op_free( &op );
	
	
ErrorOut:

	BlksEnd(Files,flags);
    
	return 0;
}
static int pioDrawZones(struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r)
{
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2;
	unsigned char nc;
	int ix,iy;
	long n;
	int k;
	
	if(!pio || !ss || !r || !pio->value)return 1;
	
	ix=0;
	iy=0;
	for(n=0;n<pio->numcell;++n){
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
	
		pio->doGradients = FALSE;
		if(pio->pa.flagGradients && pio->gradx && pio->grady){
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


	    for(n=0;n<pio->numcell;++n){
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
	    
	    return 0;
	        
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

	if(!Files->Reflect_x || pio->range.xminData >= 0.0){
		pioDraw(Files,buff,CurrentFrame,&ss);
	}else if(pio->range.xmaxData <= 0){
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
	}else{
			
		buff2=cMalloc(pio->range.ixmax*pio->range.iymax*sizeof(double),1847);
		if(!buff2)goto ErrorOut;
		
		dx=pio->range.xmaxData-pio->range.xminData;
		if(dx <= 0)return 1;
		dxi=pio->range.ixmaxData-pio->range.ixminData;
		s=(0.0-pio->range.xminData)/dx;
		isplit=(long)(s*(double)dxi+0.5)+pio->range.ixminData;
		
		if(fabs(pio->range.xminData) <=  pio->range.xmaxData){
		
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
			

		}else{
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
int doSage2DGetAreaDataBlk2(struct FileInfo2 *Files,struct areadata *ai)
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
	
	/* FilesSetFrame(CurrentFrame,Files); */
	
	if(ai->type != GENERAL_PLANE_DATA && ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"doSage2DGetAreaDataBlk2 - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
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

	ai->pioTime=Files->pioTime;
	
	if(ai->xsize <= 0 || ai->ysize <= 0)return 1;
	
	if(ai->zsize > 1){
	    sprintf(WarningBuff,"doSage2DGetAreaDataBlk2 - Error cannot return Volume Data Sets\n");
	    WarningBatch(WarningBuff);
	    return 1;	
	}
	
	data=(double *)cMalloc(ai->xsize*ai->ysize*sizeof(double),72633);
	if(!data)return 1;
	zerol((char *)data,ai->xsize*ai->ysize*sizeof(double));
	
	ai->data=data;
	 	
	if(ai->type == GENERAL_PLANE_DATA){
		int ret2=doSage2DGetGeneratData(ai,Files);
	   /* fprintf(stderr,"doSage2DGetAreaDataBlk2 2b rank %d\n",xg.rank); */
	    return ret2;
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
	
	setFloat(data,ai->xsize*ai->ysize);
     if(getPlotData(Files,data,CurrentFrame)){
	    sprintf(WarningBuff,"doSage2DGetAreaDataBlk2 - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        goto ErrorOut;
    }	    
	
	
	ret=0;
	
ErrorOut:

	pio->range=saveRange;
	    
	return 0;
}

static void addem ( double *, double *, int *, MPI_Datatype * );

static void addem(double *invec, double *inoutvec, int *len, MPI_Datatype *dtype)
{
	int i;
	
	for ( i=0; i<*len; i++ ){
	    if(invec[i] != FLOAT_NOT_SET)inoutvec[i] = invec[i];
	}
}


static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
	MPI_File mpifile;
	MPI_Op op;
	struct HeaderBlock *block;
    struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
	
    double xmin,xmax,ymin,ymax,dmin,dmax;
    long ixmin,ixmax,iymin,iymax;
    double dming,dmaxg;
    double didx,djdy;
    double dxdi,dydj;
	long BlockSize;
	unsigned long flags;
	int ret,length;
	double *sout2;
    long nn,n;		
		
	if(!Files || !sout || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	
	MPI_Op_create( (MPI_User_function *)addem, 1, &op );
	
	ret=1;
	
	flags = B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}
	
	if(doSage2DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
	/*	printf("getPlotData 2 rank %d\n",xg.rank); */
	/* MPI_Barrier(MPI_COMM_WORLD); */
	
	if(BlksStart(Files,flags))goto ErrorOut;

    setPioScales(pio);
	
	length=pio->range.ixmax*pio->range.iymax;
		
	sout2=NULL;
	
	if(xg.rank == xg.root){
	    sout2=(double *)cMalloc(sizeof(double)*length,1122);
		setFloat(sout2,length);
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

	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
	
	dming  =1e60;
	dmaxg= -1e60;
	
	dmin  =1e60;
	dmax= -1e60;
					
	for(nn=0;nn<block->nBlocks;nn += xg.size){
        double xc,yc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double svalue,value,xs,ys;
        long i,j,ihi,ilo,jhi,jlo;
        int k,kk;
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nn + kk) % (int)(block->nBlocks);
			int np=(nn + kk) % xg.size;
		
			if(np == xg.rank){

				BlockSize=block->nBlockSize[nb];
				pio->numcell=BlockSize;
				
				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
						
				if(pio->doGradients){		
					if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
					if(!pio->gradx || !pio->grady){
						pio->doGradients = FALSE;
					}
				}

				for(n=0;n<BlockSize;++n){
				
					value=pio->value[n];
					if(!pio->daughter[n] && (pio->active[n] > 0.0)){
						if(dmin > value)dmin=value;
						if(dmax < value)dmax=value;
					}
					
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
			}
		}
	}
	
	MPI_Reduce ( &dmin, &dming, 1, MPI_DOUBLE, MPI_MIN, xg.root, MPI_COMM_WORLD );
	MPI_Reduce ( &dmax, &dmaxg, 1, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
	
	SendToAll(&dming,sizeof(dming));
	SendToAll(&dmaxg,sizeof(dmaxg));
	
	MPI_Reduce ( sout, sout2, length, MPI_DOUBLE, op, xg.root, MPI_COMM_WORLD );
	MPI_Barrier(MPI_COMM_WORLD);
	
    /*
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
    */
	
    pio->cmin=pio->pd.dmin=dming;
    pio->cmax=pio->pd.dmax=dmaxg;
	
	if(xg.rank == xg.root){
	    if(sout2){
			for(n=0;n<length;++n)sout[n]=sout2[n];
			cFree((char *)sout2);
		}
	}
	
	pioSetLimits(Files,CurrentFrame);  
	  
		/* printf("getPlotData 4 rank %d\n",xg.rank); */
	
	
	ret=0;

ErrorOut:
	MPI_Op_free( &op );
	MPI_File_close(&mpifile);
	BlksEnd(Files,flags);
    return ret;
	        
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
	
	if(!Files->Reflect_x || pio->range.xminData >= 0.0){
	    return getPlotData(Files,sout,CurrentFrame);
	}else if(pio->range.xmaxData <= 0){
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
	}else{
		dx=pio->range.xmaxData-pio->range.xminData;
		if(dx <= 0)return 1;
		dxi=pio->range.ixmaxData-pio->range.ixminData;
		s=(0.0-pio->range.xminData)/dx;
		isplit=(long)(s*(double)dxi+0.5)+pio->range.ixminData;
		
		
		pio->range.xminData=0;
		pio->range.ixminData=isplit;
		ret=getPlotData(Files,sout,CurrentFrame);
		if(ret)goto ErrorOut;
		
		pio->range.ixminData=range.ixminData;
		pio->range.ixmaxData=isplit;
		pio->range.xmaxData= -range.xminData;
		ret=getPlotData(Files,sout,CurrentFrame);
		if(ret)goto ErrorOut;
		
		for(j=range.iyminData;j< range.iymaxData;++j){
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
static int doSage2DCheckBlks(struct FileInfo2 *Files,long CurrentFrame)
{
	struct HeaderBlock *block;
	struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
    int ret;
    int k;
	
	if(!Files || !Files->PIOBlkList)return 1;
	pio=&Files->pioData;
	
	ret=1;
	
	if(xg.rank == xg.root){
		if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
		if(CurrentFrame < 0)CurrentFrame=0;
		
		sage = &Files->PIOBlkList[CurrentFrame];
	
		pio->sageBlk=*sage;
		
		GetCurrentFilePath(Files,CurrentFrame,Files->mpiPath,sizeof(Files->mpiPath));

	}else{
		if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
		if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
		if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
		if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
	}
	
	SendToAll(&pio->sageBlk,sizeof(pio->sageBlk));
	
	sage=&pio->sageBlk;
	
	SendToAll(Files->mpiPath, sizeof(Files->mpiPath));
	
	pio->dxset=sage->dxset;
	pio->dyset=sage->dyset;
	pio->dzset=sage->dzset;
	pio->numcell=sage->numcell;
	pio->numcellGlobal=sage->numcell;
	
	pio->nummat=sage->nummat;
	pio->numdim=sage->numdim;

	pio->tracer_num_pnts=sage->tracer_num_pnts;
	pio->tracer_num_vars=sage->tracer_num_vars;
	pio->tracer_words_per_record=sage->tracer_words_per_record;
	pio->tracer_words_per_point=sage->tracer_words_per_point;
	
    mstrncpy(Files->pioName,pio->pioName,255);
    Files->pioIndex=pio->pioIndex;


	pio->xmin=sage->xmin;
	pio->xmax=sage->xmax;
	
	pio->ymin=sage->ymin;
	pio->ymax=sage->ymax;
	
	pio->CurrentFrame=CurrentFrame;

	for(k=1;k<=100;++k){
	    pio->dx2[k]=.5*pio->dxset/pow(2,(double)(k-1));
	    pio->dy2[k]=.5*pio->dyset/pow(2,(double)(k-1));
	}
		
	Files->limits.time=sage->time;
	Files->pioTime=sage->time;
	
    mstrncpy(Files->pioName,pio->pioName,255);
    Files->pioIndex=pio->pioIndex;
	
	
	block=&sage->block;
	if(xg.rank != xg.root){
		sage->names=cMalloc(block->itemsNum*sizeof(struct ItemNames),94823);
		block->nBlockSize=(double *)cMalloc(block->nBlocks*sizeof(double),9482);
		block->xBlockSize=(double *)cMalloc(block->nBlocks*sizeof(double),9482);
		block->xBlockAdress=(double *)cMalloc(block->nBlocks*sizeof(double),9582);
		if(!block->xBlockSize || !block->xBlockAdress || !block->nBlockSize || !sage->names)goto ErrorOut;
		
		if(pio->streamline2d.zPlane.count){
			if(pio->streamline2d.zPlane.x)cFree((char *)pio->streamline2d.zPlane.x);
			if(pio->streamline2d.zPlane.y)cFree((char *)pio->streamline2d.zPlane.y);
			
			pio->streamline2d.zPlane.x=(double *)cMalloc(pio->streamline2d.zPlane.count*sizeof(double),9482);
			pio->streamline2d.zPlane.y=(double *)cMalloc(pio->streamline2d.zPlane.count*sizeof(double),9482);
		}
	}

	SendToAll(sage->names, block->itemsNum*sizeof(struct ItemNames));
	SendToAll(block->nBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockAdress, block->nBlocks*sizeof(double));
	
	if(pio->streamline2d.zPlane.count){
	    SendToAll(pio->streamline2d.zPlane.x, pio->streamline2d.zPlane.count*sizeof(double));
	    SendToAll(pio->streamline2d.zPlane.y, pio->streamline2d.zPlane.count*sizeof(double));
	}
	
    ret=0;
    
ErrorOut:
	return ret;
}

int Sage2DGetDataBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData *sd)
{
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	struct LIMITS *limits;
	long length;
    double *sout;
    int ret;

	if(!Files)return 1;
	pio=&Files->pioData;
	if(!sd)return 1;
	limits=&Files->limits;

	ret=1;
		
	pio->doGradients=FALSE;
	
	if(doSage2DCheckBlks(Files, CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
 	if(sd->type == FRAME_DATA_FLOAT_RASTER){
 	    return FloatAndRaster(Files,CurrentFrame,sd);
 	}

 	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           ;
	    }else if(sd->type != FRAME_DATA_RASTER){
		    sprintf(WarningBuff,"Sage2DGetDataBlk2 - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
		    WarningBatch(WarningBuff);
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
 
 	/* if(SetPIOData(Files,CurrentFrame))goto ErrorOut; */

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
static int pioSetLimits(struct FileInfo2 *Files,long nf)
{
    struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax,xdelta,ydelta;
	long xzones,yzones;
	struct LIMITS *limits;
	
	if(xg.rank != xg.root)return 0;

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

