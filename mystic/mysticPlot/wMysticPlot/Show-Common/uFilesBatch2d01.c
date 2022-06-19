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
#include "Blocks.h"

static int doSage2DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files);

static int intcmp1(const void *xx,const  void *yy);

static int intcmp2(const void *xx,const  void *yy);

int pioDrawLasers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

int pioDrawTracers2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

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

static int doSage2DCheckBlocks(struct FileInfo2 *Files,long CurrentFrame);

static int getReflectedData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

int doSage2DGetAreaDataBlock(struct FileInfo2 *Files,struct areadata *ai);

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

int doSage2DGetLineDataBlocks(struct FileInfo2 *Files,struct linedata *li);

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

static int doBlockGradientsGet(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame,
							   double *lo,long nLow,double *hi,long nHigh,double *data,char *name,long item);

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

static int doBlockGradientsVel(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame);

static int doBlockGradientsVel(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	double *next;
	long nf,nl,ni;
	long n,ne;
	long nLow,nHigh;
	long nLows,nHighs;
	long minLow,maxLow;
	long minHigh,maxHigh;
	long xBlockSize;
	int GradientType=0;
	long ierror;
	int ret;
	int k;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	
	ret = 1;
	
	
	nf=start;
	nl=nf+BlockSize;
			
	nLow=0;
	nHigh=0;

	minLow=minHigh = 1000000000;
	maxLow=maxHigh =-1000000000;

	for(k=1;k<5;++k){

		next=pio->next[k-1];
		
		for(n=0;n<BlockSize;++n){
		    ne=(long)next[n]-1;
		    if(ne < nf){
		        ++nLow;
		        if(ne < minLow)minLow=ne;
		        if(ne > maxLow)maxLow=ne;
		    }
		    if(ne >= nl){
		        if(ne < minHigh)minHigh=ne;
		        if(ne > maxHigh)maxHigh=ne;
		        ++nHigh;
		    }
		}
	}
	
	
/*
	sprintf(WarningBuff,"start %ld nl %ld nLow %ld hHigh %ld BlockSize %ld \n",start,nl,nLow,nHigh,BlockSize);
	WarningBatch(WarningBuff);

	sprintf(WarningBuff,"minLow %ld maxLow %ld minHigh %ld maxHigh %ld\n",minLow,maxLow,minHigh,maxHigh);
	WarningBatch(WarningBuff);

	sprintf(WarningBuff,"xBlockSize %ld BlockSize %ld \n\n",xBlockSize,BlockSize);
	WarningBatch(WarningBuff);
*/

		
	nLows=nLow;
		
	if(nLows > 0){
	/*
	    lo=(double *)cMalloc(nLows*sizeof(double),9182);
	    if(!lo)goto ErrorOut;
	*/
	}
	
	nHighs=nHigh;
	
	if(nHighs > 0){
	/*
	    hi=(double *)cMalloc(nHighs*sizeof(double),9182);
	    if(!hi)goto ErrorOut;
	*/
	}
	
	nLow=0;
	nHigh=0;
	
	for(k=0;k<4;++k){

		next=pio->next[k];
		
		for(n=0;n<BlockSize;++n){
		    ne=(long)next[n]-1;
		    if(ne < nf){
		        pio->lo[nLow++]=ne;
		        if(ne < minLow)minLow=ne;
		        if(ne > maxLow)maxLow=ne;
		    }
		    if(ne >= nl){
		        pio->hi[nHigh++]=ne;
		        if(ne < minHigh)minHigh=ne;
		        if(ne > maxHigh)maxHigh=ne;
		    }
		}
	}
	
	
/*
	value=(double *)cMalloc(sizeof(double)*xBlockSize,19727);
	if(!value)goto ErrorOut;
	
	level=(double *)cMalloc(sizeof(double)*xBlockSize,19727);
	if(!level)goto ErrorOut;
*/	
	if(GradientType == 0){

		if(nLows){
			qsort((char *)pio->lo,nLows, sizeof(double),intcmp2);
			
			nLow=1;
			for(n=1;n<nLows;++n){   /* remove duplicates */
			    if(pio->lo[nLow-1] == pio->lo[n])continue;
			    pio->lo[nLow++]=pio->lo[n];
			}
			nLows=nLow;
		}
		
		if(nHighs){
			qsort((char *)pio->hi,nHighs, sizeof(double),intcmp2);
			
			nHigh=1;
			for(n=1;n<nHighs;++n){  /* remove duplicates */
			    if(pio->hi[nHigh-1] == pio->hi[n])continue;
			    pio->hi[nHigh++]=pio->hi[n];
			}
			nHighs=nHigh;
		}
		
		xBlockSize=BlockSize+nLow+nHigh;

	
		for(n=BlockSize;n<xBlockSize;++n)pio->level[n]=-1;
		
		
		if(doBlockGradientsGet(Files,start,BlockSize,CurrentFrame,
							   pio->lo,nLow,pio->hi,nHigh,pio->level,"cell_level",0))goto ErrorOut;
							   
		if(pio->vectorLength < xBlockSize){
			pio->vectorx=(double *)cRealloc((char *)pio->vectorx,xBlockSize*sizeof(double),9180);
			if(!pio->vectorx)goto ErrorOut;
			pio->vectory=(double *)cRealloc((char *)pio->vectory,xBlockSize*sizeof(double),9183);
			if(!pio->vectory)goto ErrorOut;
			pio->vectorLength=xBlockSize;
		}
		
		
		if(doBlockGradientsGet(Files,start,BlockSize,CurrentFrame,
							   pio->lo,nLow,pio->hi,nHigh,pio->vectorx,pio->vector.vectorxName,0L))goto ErrorOut;
		
		if(doBlockGradientsGet(Files,start,BlockSize,CurrentFrame,
							   pio->lo,nLow,pio->hi,nHigh,pio->vectory,pio->vector.vectoryName,0L))goto ErrorOut;
		
		ierror=0;
		for(n=BlockSize;n<xBlockSize;++n)if(pio->level[n]== -1)ierror++;
		
		if(ierror > 0){
			sprintf(WarningBuff,"%ld Errors in Level Data\n",ierror);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
		
		
		ni=0;
		ierror=0;
		for(k=0;k<4;++k){
		    next=pio->next[k];
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
		        if(ne < start){
					findit(nLows,pio->lo,(double)ne,&ni);
		            next[n]=(double)(BlockSize+ni+1);
		        }else if(ne >= start+BlockSize){
					findit(nHighs,pio->hi,(double)ne,&ni);
		            next[n]=(double)(BlockSize+nLow+ni+1);
		        }else{
		           next[n]=(double)(ne-start+1);
		        }
		    }
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
			    if(ne < 0 || ne >= xBlockSize){
		           ierror++;	           
			    }
			}
		}
		
		
	
	}else{
		ierror=0;
		for(k=0;k<4;++k){
		    next=pio->next[k];
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
		        if(ne < start){
		           next[n]=(double)(n+1);
		        }else if(ne >= start+BlockSize){
		           next[n]=(double)(n+1);
		        }else{
		           next[n]=(double)(ne-start+1);
		        }
		    }
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
			    if(ne < 0 || ne >= BlockSize){
		           ierror++;	           
			    }
			}
		}
	}
	    	
	
	if(ierror){
		sprintf(WarningBuff,"%ld Errors Sorting Gradient data\n",ierror);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	zerol((char *)pio->grady,sizeof(double)*BlockSize);
	
	zerol((char *)pio->gradx,sizeof(double)*BlockSize);


	
	ret = 0;
ErrorOut:	
	
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


static int pioDrawStream2d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	struct streamStruct *stream;
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
	long nff,nl,BlockSize,BlockSizeSave,numcell,numcellSave,nb;
	long CurrentFrame;

	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	sage=pio->sage;
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
			    
	if(BlocksStart(Files,flags))goto ErrorOut;
		    	  
	if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
	
    setPioScales(pio);
    
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

    nb=0;
 	nff=0;
	do{
		nl=nff+BlockSize;
		if(nl > numcell){
			nl=numcell;
			BlockSize=nl-nff;
		}
		++nb;
		pio->numcell=BlockSize;
		
		if(BlocksRead(Files,flags,nff,BlockSize,CurrentFrame))goto ErrorOut;
					
		doRangeBlock(Files);
		
		if(pio->doGradients){		
			if(doBlockGradientsVel(Files,nff,BlockSize,CurrentFrame))goto ErrorOut;
		}
    
    
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
	
		nff += BlockSize;
	}while(nl < numcell);
	
	idone=0;
	for(n=0;n<count;++n){
		if(!stream[n].foundFore){
		/*
    	    sprintf(WarningBuff,"Point Fore %g %g not Found\n",stream[n].xFore,stream[n].yFore);
    	    WarningBatch(WarningBuff);
    	*/
		    stream[n].doneFore=TRUE;
		}
		if(!stream[n].foundBack){
		/*
    	    sprintf(WarningBuff,"Point Back %g %g not Found\n",stream[n].xBack,stream[n].yBack);
    	    WarningBatch(WarningBuff);
    	*/
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
	
	BlocksEnd(Files,flags);
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
    struct FilePIOInfo *pio;
	struct DrawInfo info;
	unsigned long flags;
	struct PIO *sage;
	long CurrentFrame;
	long BlockSize;
    long nf,nl,n;		
	
	
	if(!ai || !ai->data || !Files)return 1;
	pio=&Files->pioData;
	sage=pio->sage;
	if(!sage)return 1;
	
	CurrentFrame=ai->CurrentFrame;
	
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
		
	BlockSize=pio->BlockSize;
	if(BlockSize <= 0)return 1;
	
	flags = B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}
	
	
	if(BlocksStart(Files,flags))goto ErrorOut;

	if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
		
    setPioScales(pio);
    
	nf=0;
	do{
		nl=nf+BlockSize;
		if(nl > pio->numcell){
			nl=pio->numcell;
			BlockSize=nl-nf;
		}
		
		if(BlocksRead(Files,flags,nf,BlockSize,CurrentFrame))goto ErrorOut;
				
		if(pio->doGradients){		
			if(doBlockGradients(Files,nf,BlockSize,CurrentFrame))goto ErrorOut;
			if(!pio->gradx || !pio->grady){
				pio->doGradients = FALSE;
			}
		}

		for(n=0;n<BlockSize;++n){		
	        if(pio->daughter[n])continue;	        
    		if(doCell(n,&info))break;	        
		}
		
		nf += BlockSize;
	}while(nl < pio->numcell);
    
ErrorOut:
	BlocksEnd(Files,flags);
	return  0;
}
static int doBlockGradientsGet(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame,
							   double *lo,long nLow,double *hi,long nHigh,double *data,char *name,long item)
{
	struct FilePIOInfo *pio;
	long first,last;
	long found,BlockRead,BlockSizeStart;
	long n,nf,nl;
	int ret;
	
	if(!Files || !name || !data)return 1;
	if(nLow && !lo)return 1;
	if(nHigh && !hi)return 1;
	pio=&Files->pioData;
	
	
	ret=1;
	
	start=start;
	
	BlockSizeStart=BlockSize;
	
	
	if(nLow){
		nf=0;
				
		found=0;
		
		while(1){
		
			BlockSize=BlockSizeStart;
			
			first=(long)lo[nf];
						
			last=first+BlockSize;
			
			nl=nf;
			for(n=nf;n<nLow;++n){
			    if((long)lo[n] >= first && (long)lo[n] < last){
			        nl=n;
			    }
			}
			
			if(nl == nf){
				BlockSize=1;
			}else{
				BlockSize=(long)(lo[nl]-lo[nf])+1;
			}
			
			BlockRead=BlockSize;
			
			if(first+BlockSize > pio->numcellGlobal){
			     BlockRead=pio->numcellGlobal-first;
			}
			
			if(SageGetFileItemBlock(Files,name,item,
		                  pio->gradx,first,BlockRead,CurrentFrame)){
				ret=2;goto ErrorOut;
			}
			
			nl=nf;
			for(n=nf;n<nLow;++n){
			    if((long)lo[n] >= first && (long)lo[n] < last){
			        data[BlockSizeStart+n]=pio->gradx[(long)lo[n]-first];
			        nl=n;
			        ++found;
			    }
			}
			
			if(found >= nLow)break;
			
			if(++nl >=  nLow){
				WarningBatch("doBlockGradientsGet Not All Low data not found\n");
				break;
			}
			
			nf=nl;   /* skip to next needed Item */
			
		}
		
	}
/*	
	if(SageGetFileItemBlock(Files,name,item,
                  data,start,BlockSizeStart,CurrentFrame)){
		ret=2;goto ErrorOut;
	}
*/		
	if(nHigh){
	
		nf=0;
				
		found=0;
		
		while(1){
		
			BlockSize=BlockSizeStart;
		
			first=(long)hi[nf];
			
			last=first+BlockSize;
			
			nl=nf;
			for(n=nf;n<nHigh;++n){
			    if((long)hi[n] >= first && (long)hi[n] < last){
			        nl=n;
			    }
			}
			
			if(nl == nf){
				BlockSize=1;
			}else{
				BlockSize=(long)(hi[nl]-hi[nf])+1;
			}
			
			
			BlockRead=BlockSize;
			
			if(first+BlockSize > pio->numcellGlobal){
			     BlockRead=pio->numcellGlobal-first;
			}
			
			if(SageGetFileItemBlock(Files,name,item,
		                  pio->gradx,first,BlockRead,CurrentFrame)){
				ret=2;goto ErrorOut;
			}
			
			nl=nf;
			for(n=nf;n<nHigh;++n){
			    if((long)hi[n] >= first && (long)hi[n] < last){
			        data[BlockSizeStart+nLow+n]=pio->gradx[(long)hi[n]-first];
			        nl=n;
			        ++found;
			    }
			}
			
			if(found >= nHigh)break;
			
			if(last >= pio->numcellGlobal)break;
			
			if(++nl >=  nHigh){
				WarningBatch("doBlockGradientsGet Not All High data not found\n");
				break;
			}
			nf=nl;   /* skip to next needed Item */
			
		}
		
	}
	
	
    ret=0;
 
ErrorOut:

	return ret;
	
}
int findit(long count,double *level,double value,long *ic)
{
	long ib,it,ns;
	
	if(!level || !ic)return 1;
	
	if(value <= *level){
	    *ic=0;
	    return 0;
	}
	it=count-1;
	if(value >= level[it]){
	    *ic=it;
	    return 0;
	}
	ib=0;
	while(it > ib+1){
	    ns=(it+ib)/2;
	    if(value >= level[ns]){
	        ib=ns;
	    }else{
	        it=ns;
	    }
	}
	*ic=ib;
	return 0;
	
}
static int doBlockGradients(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	double *next;
	long nf,nl,ni;
	long n,n2,ne;
	long nLow,nHigh;
	long nLows,nHighs;
	long minLow,maxLow;
	long minHigh,maxHigh;
	long xBlockSize;
	int GradientType=0;
	long ierror;
	int ret;
	int k,k2;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	
	ret = 1;
	
	
	nf=start;
	nl=nf+BlockSize;
			
	nLow=0;
	nHigh=0;

	minLow=minHigh = 1000000000;
	maxLow=maxHigh =-1000000000;

	for(k=1;k<5;++k){

		next=pio->next[k-1];
		
		for(n=0;n<BlockSize;++n){
		    ne=(long)next[n]-1;
		    if(ne < nf){
		        ++nLow;
		        if(ne < minLow)minLow=ne;
		        if(ne > maxLow)maxLow=ne;
		    }
		    if(ne >= nl){
		        if(ne < minHigh)minHigh=ne;
		        if(ne > maxHigh)maxHigh=ne;
		        ++nHigh;
		    }
		}
	}
	
	
/*
	sprintf(WarningBuff,"start %ld nl %ld nLow %ld hHigh %ld BlockSize %ld \n",start,nl,nLow,nHigh,BlockSize);
	WarningBatch(WarningBuff);

	sprintf(WarningBuff,"minLow %ld maxLow %ld minHigh %ld maxHigh %ld\n",minLow,maxLow,minHigh,maxHigh);
	WarningBatch(WarningBuff);

	sprintf(WarningBuff,"xBlockSize %ld BlockSize %ld \n\n",xBlockSize,BlockSize);
	WarningBatch(WarningBuff);
*/

		
	nLows=nLow;
		
	if(nLows > 0){
	/*
	    lo=(double *)cMalloc(nLows*sizeof(double),9182);
	    if(!lo)goto ErrorOut;
	*/
	}
	
	nHighs=nHigh;
	
	if(nHighs > 0){
	/*
	    hi=(double *)cMalloc(nHighs*sizeof(double),9182);
	    if(!hi)goto ErrorOut;
	*/
	}
	
	nLow=0;
	nHigh=0;
	
	for(k=0;k<4;++k){

		next=pio->next[k];
		
		for(n=0;n<BlockSize;++n){
		    ne=(long)next[n]-1;
		    if(ne < nf){
		        pio->lo[nLow++]=ne;
		        if(ne < minLow)minLow=ne;
		        if(ne > maxLow)maxLow=ne;
		    }
		    if(ne >= nl){
		        pio->hi[nHigh++]=ne;
		        if(ne < minHigh)minHigh=ne;
		        if(ne > maxHigh)maxHigh=ne;
		    }
		}
	}
	
	
/*
	value=(double *)cMalloc(sizeof(double)*xBlockSize,19727);
	if(!value)goto ErrorOut;
	
	level=(double *)cMalloc(sizeof(double)*xBlockSize,19727);
	if(!level)goto ErrorOut;
*/	
	if(GradientType == 0){

		if(nLows){
			qsort((char *)pio->lo,nLows, sizeof(double),intcmp2);
			
			nLow=1;
			for(n=1;n<nLows;++n){   /* remove duplicates */
			    if(pio->lo[nLow-1] == pio->lo[n])continue;
			    pio->lo[nLow++]=pio->lo[n];
			}
			nLows=nLow;
		}
		
		if(nHighs){
			qsort((char *)pio->hi,nHighs, sizeof(double),intcmp2);
			
			nHigh=1;
			for(n=1;n<nHighs;++n){  /* remove duplicates */
			    if(pio->hi[nHigh-1] == pio->hi[n])continue;
			    pio->hi[nHigh++]=pio->hi[n];
			}
			nHighs=nHigh;
		}
		
		xBlockSize=BlockSize+nLow+nHigh;

	
		for(n=BlockSize;n<xBlockSize;++n)pio->level[n]=-1;
		
		
		if(doBlockGradientsGet(Files,start,BlockSize,CurrentFrame,
							   pio->lo,nLow,pio->hi,nHigh,pio->level,"cell_level",0))goto ErrorOut;
		
		
		if(doBlockGradientsGet(Files,start,BlockSize,CurrentFrame,
							   pio->lo,nLow,pio->hi,nHigh,pio->value,pio->pioName,pio->pioIndex))goto ErrorOut;
		
		ierror=0;
		for(n=BlockSize;n<xBlockSize;++n)if(pio->level[n]== -1)ierror++;
		
		if(ierror > 0){
			sprintf(WarningBuff,"%ld Errors in Level Data\n",ierror);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
		
		
		ni=0;
		ierror=0;
		for(k=0;k<4;++k){
		    next=pio->next[k];
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
		        if(ne < start){
					findit(nLows,pio->lo,(double)ne,&ni);
		            next[n]=(double)(BlockSize+ni+1);
		        }else if(ne >= start+BlockSize){
					findit(nHighs,pio->hi,(double)ne,&ni);
		            next[n]=(double)(BlockSize+nLow+ni+1);
		        }else{
		           next[n]=(double)(ne-start+1);
		        }
		    }
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
			    if(ne < 0 || ne >= xBlockSize){
		           ierror++;	           
			    }
			}
		}
		
		
	
	}else{
		ierror=0;
		for(k=0;k<4;++k){
		    next=pio->next[k];
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
		        if(ne < start){
		           next[n]=(double)(n+1);
		        }else if(ne >= start+BlockSize){
		           next[n]=(double)(n+1);
		        }else{
		           next[n]=(double)(ne-start+1);
		        }
		    }
		    for(n=0;n<BlockSize;++n){
			    ne=(long)next[n]-1;
			    if(ne < 0 || ne >= BlockSize){
		           ierror++;	           
			    }
			}
		}
	}
	    	
	
	if(ierror){
		sprintf(WarningBuff,"%ld Errors Sorting Gradient data\n",ierror);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

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
ErrorOut:	
	
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
	long nff,nl,BlockSize,BlockSizeSave,numcell,numcellSave,nb;
	struct FilePIOInfo *pio;
	struct PIO *sage;
	double *xD,*yD,s,s1,s2,s3,s4,dx2,dy2;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral;
	double length;
	long nd,np,ne,n,ns;
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
	
	sage=pio->sage;
	
	np=10000;
	
	if(!(sort=(struct sortdata *)cMalloc((long)sizeof(struct sortdata)*(np),81876))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	
	CurrentFrame=li->CurrentFrame;
	
	BlockSize=pio->BlockSize;
		    
	if(BlocksStart(Files,B_2D_STANDARD))goto ErrorOut;
		    
	if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
	
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
	
	numcell=pio->numcell;
	BlockSize=pio->BlockSize;
    nb=0;
 	nff=0;
	do{
		nl=nff+BlockSize;
		if(nl > numcell){
			nl=numcell;
			BlockSize=nl-nff;
		}
		++nb;
		
		pio->numcell=BlockSize;
		
		if(BlocksRead(Files,B_2D_STANDARD,nff,BlockSize,CurrentFrame))goto ErrorOut;
				
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
NextBlock:	
			nff += BlockSize;
		}while(nl < numcell);
		
	
	
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
	
	BlocksEnd(Files,B_2D_STANDARD);
    
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
int intcmp2(const void *xx,const  void *yy)
{
	double sum;
	register double *x=(double *)xx;
	register double *y=(double *)yy;
	sum = *x - *y;
	if(sum == 0.0)return 0;
	return (int)((sum > 0.0) ? 1 : -1);
}

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
	long nff,nl,BlockSize,BlockSizeSave,numcell,numcellSave,nb;
	struct FilePIOInfo *pio;
	long CurrentFrame;
	unsigned long flags;
	long index;
	long n;
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
		
		
	if(BlocksStart(Files,flags))goto ErrorOut;
		    	    
	if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
	
    setPioScales(pio);
    
	for(n=0;n<li->PointLineCount;++n){	    
		li->v[n]=0;
	}
	
/*	
	pio->pa.flagGradients=TRUE;
	
 */   
    	numcell=pio->numcell;
		BlockSize=pio->BlockSize;
	    nb=0;
	 	nff=0;
		do{
			nl=nff+BlockSize;
			if(nl > numcell){
				nl=numcell;
				BlockSize=nl-nff;
			}
			++nb;
			
			pio->numcell=BlockSize;
			
			if(BlocksRead(Files,flags,nff,BlockSize,CurrentFrame))goto ErrorOut;
			
			doRangeBlock(Files);

			if(pio->doGradients){		
				if(doBlockGradients(Files,nff,BlockSize,CurrentFrame))goto ErrorOut;
				if(!pio->gradx || !pio->grady){
					pio->doGradients = FALSE;
				}
			}
			
			for(n=0;n<li->PointLineCount;++n){
			    		    
			    if(!pioGetValue(pio,li->x[n],li->y[n],&li->v[n],&index)){
			        continue;
			    }	    
			}
			nff += BlockSize;
		}while(nl < numcell);
		
	ret = 0;
ErrorOut:

	BlocksEnd(Files,flags);
    
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


int doSage2DGetLineDataBlocks(struct FileInfo2 *Files,struct linedata *li)
{
	int ix1,iy1,ix2,iy2;
	double x1,y1,x2,y2,dl,ds,dx,dy,dp;
	long ns,n,nd;
	double *xD,*yD,*zD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nff,nl,BlockSize,BlockSizeSave,numcell,numcellSave,nb;
	long CurrentFrame;
	struct FilePIOInfo *pio;
	unsigned long flags;
	long index;
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
			    
	if(BlocksStart(Files,flags))goto ErrorOut;
		    	  
	if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
	
    setPioScales(pio);
	
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


	for(n=0;n<ns;++n){
	    double x,y;

	    x=x1+(double)n*ds*(x2-x1);
	    y=y1+(double)n*ds*(y2-y1);
	    
	    xD[n]=x;
	    yD[n]=y;
	    zD[n]=0;

	}

	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

    	numcell=pio->numcell;
		BlockSize=pio->BlockSize;
	    nb=0;
	 	nff=0;
		do{
			nl=nff+BlockSize;
			if(nl > numcell){
				nl=numcell;
				BlockSize=nl-nff;
			}
			++nb;
			pio->numcell=BlockSize;
			
			if(BlocksRead(Files,flags,nff,BlockSize,CurrentFrame))goto ErrorOut;
						
			doRangeBlock(Files);
			
			if(pio->doGradients){		
				if(doBlockGradients(Files,nff,BlockSize,CurrentFrame))goto ErrorOut;
				if(!pio->gradx || !pio->grady){
					pio->doGradients = FALSE;
				}
			}

			for(n=0;n<ns;++n){
			    double value,x,y;
				if(zD[n] > 0)continue;
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
			    zD[n]=1.0;
			}	
			nff += BlockSize;
		}while(nl < numcell);
		
	nd=0;
	for(n=0;n<ns;++n){
	    if(zD[n] > 0){
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
	
	BlocksEnd(Files,flags);
        
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;

	return ret;
}

static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double *v,long *index)
{
	double xc,yc;
	struct PIO *sage;
	double xx,yy;
	long ne;

	double xmin,ymin;

	if(!pio || !v || !pio->value)return 0;
	if(!pio->daughter)return 0;
	sage=pio->sage;
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
	double dp;
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt;
	double r,theta,dr,dtheta;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long nff,nl,BlockSize,numcell,nb;
	double valuet;
	double x,y;
	double xs,ys;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
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
		
	if(BlocksStart(Files,flags))goto ErrorOut;
		    	  	
    setPioScales(pio);	

	xs=x;
	ys=y;
	nd=0;

	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    
		if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
		
    	nf=0;
    	valuet=0;	    	    
    	numcell=pio->numcell;
		BlockSize=pio->BlockSize;
    	
	    nb=0;
	 	nff=0;
		do{
			nl=nff+BlockSize;
			if(nl > numcell){
				nl=numcell;
				BlockSize=nl-nff;
			}
			++nb;
			pio->numcell=BlockSize;
			
			if(BlocksRead(Files,flags,nff,BlockSize,CurrentFrame))goto ErrorOut;
			
			doRangeBlock(Files);
			
			if(pio->doGradients){		
				if(doBlockGradients(Files,nff,BlockSize,CurrentFrame))goto ErrorOut;
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
    		if(nf >= NumberOfRings*NumberOfPoints)break;
			nff += BlockSize;
		}while(nl < numcell);
/*
		sprintf(WarningBuff,"nff = %ld ",nff);
		WarningBatch(WarningBuff);
		
		sprintf(WarningBuff,"nb = %ld\n",nb);
		WarningBatch(WarningBuff);
*/
		pio->numcell=numcell;
    	    
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
    
	BlocksEnd(Files,flags);
            
	return ret;
}
static int pioDraw2(struct FileInfo2 *Files,double *data,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
    struct FilePIOInfo *pio;
    static double  small2=1e-8;
	struct dRange rr;
	long nf,nl,BlockSize,numcell,nb;
	unsigned long flags;
	int ret;
	double *d;
	long length;
	
	
	if(!Files || !ss || !buff || !data)return 1;

	pio = &Files->pioData;
	
	pio->CurrentFrame=CurrentFrame;
	
	BlockSize=pio->BlockSize;
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
    
    flags = B_2D_STANDARD;
	if(pio->vector.DrawVectors)flags |= (B_XVEL | B_YVEL);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}

	if(BlocksStart(Files,flags))goto ErrorOut;
		    	  			
	if(pio->vector.DrawVectors){
		CheckVectorRange(Files);
	}
	
    numcell=pio->numcell;
    
    nb=0;
 	nf=0;
	do{
		nl=nf+BlockSize;
		if(nl > numcell){
			nl=numcell;
			BlockSize=nl-nf;
		}
		++nb;
		pio->numcell=BlockSize;
		
		if(BlocksRead(Files,flags,nf,BlockSize,CurrentFrame))goto ErrorOut;
								
		if(pio->doGradients){		
			if(doBlockGradients(Files,nf,BlockSize,CurrentFrame))goto ErrorOut;
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
		
		nf += BlockSize;
	}while(nl < numcell);
	
	pio->numcell=numcell;
   
		
	if(pio->streamline2d.streamOn){
		pioDrawStream2d(Files,ss,&rr);
	}
		
	if(pio->LaserPlotCount){
	   pioDrawLasers2d(Files,ss,&rr);
	}
	
	if(pio->TracerPlotCount){
	   pioDrawTracers2d(Files,ss,&rr);
	}
	
ErrorOut:

	BlocksEnd(Files,flags);
        
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
static int CheckVectorRange(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	double vmax,vlength,vx,vy;
	double xc,yc,dx2,dy2;
	long BlockSize,numcell;
	unsigned long flags;
	long n,nf,nl;
	int ret;
	int k;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	BlockSize=pio->BlockSize;
	
	ret = 1;
	
	      
    numcell=pio->numcell;
    
    flags = (B_X | B_Y | B_DAUGHTER  | B_LEVEL | B_XVEL | B_YVEL);
    
 	nf=0;
	vmax=0;	    
	do{
		nl=nf+BlockSize;
		if(nl > numcell){
			nl=numcell;
			BlockSize=nl-nf;
		}

		if(BlocksRead(Files,flags,nf,BlockSize,pio->CurrentFrame))goto ErrorOut;
									    	    
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
	    	    	    
		nf += BlockSize;
	}while(nl < numcell);
	    pio->vmaxVectors=vmax;
	    
	ret=0;
ErrorOut:
	return ret;
}
	
static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss)
{
    struct FilePIOInfo *pio;
    static double  small2=1e-8;
	struct dRange rr;
	long nf,nl,BlockSize,numcell,nb;
	unsigned long flags;
	double *d;
	long length;
	int ret;
	
	
	if(!Files || !ss || !buff)return 1;
	pio = &Files->pioData;
	
	pio->CurrentFrame=CurrentFrame;
	
	BlockSize=pio->BlockSize;
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
    
    
    flags = B_2D_STANDARD;
	if(pio->vector.DrawVectors)flags |= (B_XVEL | B_YVEL);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}

	if(BlocksStart(Files,flags))goto ErrorOut;
		    	  			
	if(pio->vector.DrawVectors){
		CheckVectorRange(Files);
	}
	
    numcell=pio->numcell;
    
    nb=0;
 	nf=0;
	do{
		nl=nf+BlockSize;
		if(nl > numcell){
			nl=numcell;
			BlockSize=nl-nf;
		}
		++nb;
		pio->numcell=BlockSize;
		
		if(BlocksRead(Files,flags,nf,BlockSize,CurrentFrame))goto ErrorOut;
										
		if(pio->doGradients){		
			if(doBlockGradients(Files,nf,BlockSize,CurrentFrame))goto ErrorOut;
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
		
		nf += BlockSize;
	}while(nl < numcell);
	
	pio->numcell=numcell;
   
	
	if(pio->streamline2d.streamOn){
		pioDrawStream2d(Files,ss,&rr);
	}
	
	
	if(pio->LaserPlotCount){
	   pioDrawLasers2d(Files,ss,&rr);
	}
	
	if(pio->TracerPlotCount){
	   pioDrawTracers2d(Files,ss,&rr);
	}
	
ErrorOut:

	BlocksEnd(Files,flags);
    
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
int doSage2DGetAreaDataBlock(struct FileInfo2 *Files,struct areadata *ai)
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
	
	if(ai->type != GENERAL_PLANE_DATA && ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"doSage2DGetAreaDataBlock - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
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
	    sprintf(WarningBuff,"doSage2DGetAreaDataBlock - Error cannot return Volume Data Sets\n");
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
	    sprintf(WarningBuff,"doSage2DGetAreaDataBlock - getPlotData Error\n");
	    WarningBatch(WarningBuff);
        goto ErrorOut;
    }	    
	
	ret=0;
	
ErrorOut:

	pio->range=saveRange;
	    
	return 0;
}


static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	
    double xmin,xmax,ymin,ymax,dmin,dmax;
    long ixmin,ixmax,iymin,iymax;
    double didx,djdy;
    double dxdi,dydj;
	long BlockSize;
	unsigned long flags;
	int ret;
    long n,nf,nl;		
		
	if(!Files || !sout || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	ret=1;

	BlockSize=pio->BlockSize;
	if(BlockSize <= 0)return 1;
	
	flags = B_2D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_2D;
	}
	
	
	if(BlocksStart(Files,flags))goto ErrorOut;

	if(doSage2DCheckBlocks(Files,CurrentFrame))goto ErrorOut;
		
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
		
	nf=0;
	do{
        double xc,yc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double svalue,value,xs,ys;
        long i,j,ihi,ilo,jhi,jlo;
        int k;
		nl=nf+BlockSize;
		if(nl > pio->numcell){
			nl=pio->numcell;
			BlockSize=nl-nf;
		}
		
		if(BlocksRead(Files,flags,nf,BlockSize,CurrentFrame))goto ErrorOut;
				
		if(pio->doGradients){		
			if(doBlockGradients(Files,nf,BlockSize,CurrentFrame))goto ErrorOut;
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
		nf += BlockSize;
	}while(nl < pio->numcell);
    /*
    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
    */
    pio->cmin=pio->pd.dmin=dmin;
    pio->cmax=pio->pd.dmax=dmax;
	pioSetLimits(Files,CurrentFrame);    
	ret=0;

ErrorOut:
	BlocksEnd(Files,flags);
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
static int doSage2DCheckBlocks(struct FileInfo2 *Files,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	struct PIO *sage;
	long BlockSize;
    int ret;
    int k;
	
	if(!Files || !Files->PIO2DList)return 1;
	pio=&Files->pioData;
	
	ret=1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIO2DList[CurrentFrame];
	
	Files->limits.time=sage->time;
	Files->pioTime=sage->time;
	
	pio->sage=sage;
	
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
		
	BlockSize=pio->BlockSize;
	if(BlockSize <= 0)return 1;
	
    ret=0;
    
	return ret;
}

int doSage2DGetDataBlocks(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData *sd)
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

	ret=1;
	
	pio->BlockSize=pio->pa.BlockSize=sd->pa.BlockSize;
	
	if(pio->BlockSize <= 0)return 1;
	
	pio->doGradients=FALSE;
	
	if(doSage2DCheckBlocks(Files, CurrentFrame))goto ErrorOut;
	
 	if(sd->type == FRAME_DATA_FLOAT_RASTER){
 	    return FloatAndRaster(Files,CurrentFrame,sd);
 	}

 	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           ;
	    }else if(sd->type != FRAME_DATA_RASTER){
		    sprintf(WarningBuff,"doSage2DGetDataBlocks - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
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
