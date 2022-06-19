/*
 *  uFilesBatch3dBlkMPI01.c
 *  xShow
 *
 *  Created by Dale Ranta on 9/8/11.
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
#include "SaveAllImages.h"
#include "Linedata.h"
#include "Message.h"
#include "uFilesBatch3d.h"
#include "Universal.h"
#include "uGraph.h"
#include "Blocks.h"

struct sortdata{
	double x;
	double y;
	double z;
	double x2;
	double y2;
	double z2;
	double sl;
	double sh;
	long ne;
};


struct streamStruct{
   int doneFore;
   int doneBack;
   double xFore;
   double yFore;
   double zFore;
   double xBack;
   double yBack;
   double zBack;
   long stepsFore;
   long stepsBack;
   int foundFore;
   int foundBack;
};

struct streamStruct2{
   double x;
   double y;
   double z;
   long steps;
   int found;
   int done;
};

struct sort1{
  double x;
  double y;
  double z;
};

/*
static int intcmp3(const void *xx,const  void *yy);
*/

int GetCurrentFilePath(struct FileInfo2 *Files,long CurrentFrame,char *filePath,long size);

int SendToOthers(void *data,int length,int source);

int clearData(void **data);

static int doRangeBlock(struct FileInfo2 *Files);

static int pioDrawStream3d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int pioGetValueN(struct FilePIOInfo *pio,double x,double y,double z,double *v,long *nn);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

static int DrawCellLines(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int DrawCellVelocity(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc);

static int DrawCellVelocity2(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

int SendToAll(void *data,int length);

int setFloat(double *d,long length);

static int doBlockGradients(struct FileInfo2 *Files,long start,long BlockSize,long CurrentFrame);

static int setPioScales(struct FilePIOInfo *pio);

static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);

static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);

int Sage3DGetDataBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int FloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSage3DCheckBlks(struct FileInfo2 *Files,long CurrentFrame);

int SageItemNumber(struct PIO_BLK *sage,char *dataName,long plotItem);


static int CheckVectorRange(struct FileInfo2 *Files);

int doSage3DGetLineDataBlk2(struct FileInfo2 *Files,struct linedata *li);

static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li);

static int doIntegrateLine(struct FileInfo2 *Files,struct linedata *li);

static int intcmp1(const void *xx,const  void *yy);
static int intcmp2(const void *xx,const  void *yy);
static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double z,double *v);
static long pioGetValue_ne;

int BlksReadMPI(MPI_File mpifile,struct FileInfo2 *Files,unsigned long flags,long nb,long BlockSize,long CurrentFrame);

static int doCell(long ne,struct DrawInfo3 *info);

static int doSage3DGetGeneratDataOffSet(struct areadata *ai,struct FileInfo2 *Files);

static int doSage3DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files);

static int doSage3DGetGridData(struct areadata *ai,struct FileInfo2 *Files);

int doSage3DGetAreaDataBlk2(struct FileInfo2 *Files,struct areadata *ai);

double *doSage3DGetSDS3DBlk2(struct FileInfo2 *Files,long CurrentFrame);

int doSage3DGetStreamLineBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int doSage3DGetStreamLineBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	MPI_File mpifile;
	struct streamStruct2 *streamFore;
	struct streamStruct2 *streamBack;
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	struct SetFrameData *s;
	double *lo_x,*hi_x,*lo_y,*hi_y,*lo_z,*hi_z;
	double *xd,*yd,*zd,*vd,*xs,*ys,*zs,*vs;
	double dxdi,dydj,dzdk,didx,djdy,dkdz,dv;
	double gradxV1,gradyV1,gradzV1;
	double gradxV2,gradyV2,gradzV2;
	double gradxV3,gradyV3,gradzV3;
	double gradl,gradh,vlen,sx,sy,sz;
	double x1,y1,z1,x,y,z,v,vx,vy,vz,vv,x2,y2,z2;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	long streamSteps;
    int streamsReturned;
	int streamGradients;
	long n,k,ne,nn,nstep,n2,nGrad,nstepsave;
	long BlockSize,nnn;
	struct dataStruct *d;
	int streamPasses;
	long count;
	unsigned long flags;
	int idone;
	int ipass;
	int k2,ke;
    int ret;
    
	if(!Files)return 1;
	if(!sd || (sd->streamline3d.xyz.count <= 0))return 1;
	s = &Files->SetData;
	pio = &Files->pioData;

	ret=1;
	
	streamFore=NULL;
	streamBack=NULL;
	count=sd->streamline3d.xyz.count;
	streamsReturned=0;
	xs=ys=zs=vs=NULL;
	xd=yd=zd=vd=NULL;
		
	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=(int)sd->pioIndex;
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
	streamPasses=sd->streamline3d.streamPasses;
    xd=yd=zd=NULL;
	lo_x=hi_x=lo_y=hi_y=lo_z=hi_z=NULL;
	
	
	flags=B_3D_STANDARD | B_XVEL | B_YVEL  | B_ZVEL;
	pio->doGradients = FALSE;
    if(streamGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
    }	
    
	sd->streamline3d.xyzReturned=(struct dataStruct *)cMalloc(count*sizeof(struct dataStruct),8727);
	d=(struct dataStruct *)cMalloc(count*sizeof(struct dataStruct),8722);
	if(!d || !sd->streamline3d.xyzReturned){
		WarningBatch("doSage3DGetStreamLineBlk2 Out of Memory\n");
		goto ErrorOut;
	}
	
	
	streamFore=(struct streamStruct2 *)cMalloc(count*sizeof(struct streamStruct2),3871);
	if(!streamFore){
	    WarningBatch("doSage3DGetStreamLineBlk2 Blocks Out of List Memory\n");
	    goto ErrorOut;
	}
	
	streamBack=(struct streamStruct2 *)cMalloc(count*sizeof(struct streamStruct2),3871);
	if(!streamBack){
	    WarningBatch("doSage3DGetStreamLineBlk2 Blocks Out of List Memory\n");
	    goto ErrorOut;
	}
	
	/*
		fprintf(stderr,"2 doSage3DGetStreamLineBlk2 rank %d count %ld %p %p %p\n",xg.rank,(long)sd->streamline3d.xyz.count,
		        sd->streamline3d.xyz.x,sd->streamline3d.xyz.y,sd->streamline3d.xyz.z);
	*/
	
	for(n=0;n<count;++n){
	
		streamFore[n].x=sd->streamline3d.xyz.x[n];
		streamBack[n].x=streamFore[n].x;
		streamFore[n].y=sd->streamline3d.xyz.y[n];
		streamBack[n].y=streamFore[n].y;
		streamFore[n].z=sd->streamline3d.xyz.z[n];
		streamBack[n].z=streamFore[n].z;

   		streamFore[n].done=0;
   		streamBack[n].done=0;
   		streamFore[n].steps=0;
   		streamBack[n].steps=0;
   		streamBack[n].found=FALSE;
   		streamBack[n].found=FALSE;
		
		d[n].x=(double *)cMalloc(2*streamSteps*sizeof(double),7721);
		d[n].y=(double *)cMalloc(2*streamSteps*sizeof(double),7721);
		d[n].z=(double *)cMalloc(2*streamSteps*sizeof(double),7721);
		d[n].v=(double *)cMalloc(2*streamSteps*sizeof(double),7721);

		if(!d[n].x ||!d[n].y ||!d[n].z ||!d[n].v){
			WarningBatch("doSage3DGetStreamLine3 Out of Memory\n");
			goto ErrorOut;
		}
    }

	
				    
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
    setPioScales(pio);
	
	if(BlksStart(Files,flags))goto ErrorOut;
		    	  
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

    ipass=0;
	
StartAgain:    

	for(n=0;n<sd->streamline3d.xyz.count;++n){
	    streamFore[n].found = FALSE;
	    streamBack[n].found = FALSE;
	}

	for(nnn=0;nnn<block->nBlocks;nnn += xg.size){
        int kk;
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nnn + kk) % (int)(block->nBlocks);
			int np=(nnn + kk) % xg.size;
		
			if(np == xg.rank){
		
				BlockSize=block->nBlockSize[nb];
				
				pio->numcell=BlockSize;
								
				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;

				doRangeBlock(Files);

				if(streamGradients){
					lo_x=pio->next[0];
					hi_x=pio->next[1];
					lo_y=pio->next[2];
					hi_y=pio->next[3];
					lo_z=pio->next[4];
					hi_z=pio->next[5];
				}
				
				for(n=0;n<count;++n){
					nstepsave=-1;
					for(k=0;k<2;++k){
						if(k == 0){
						   if(streamFore[n].done)continue;
							x1=x=streamFore[n].x;
							y1=y=streamFore[n].y;
							z1=z=streamFore[n].z;
							nstep=streamFore[n].steps;
						}else{
						   if(streamBack[n].done)continue;
							x1=x=streamBack[n].x;
							y1=y=streamBack[n].y;
							z1=z=streamBack[n].z;
							nstep=streamBack[n].steps;
						}
					
						ne=-1;
						nn=n;
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
						
						if(!pioGetValueN(pio,x,y,z,&v,&ne))continue;
							if(k == 0){
								streamFore[n].found=TRUE;
							}else{
								streamBack[n].found=TRUE;
							}
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
								d[n].x[streamSteps+nstep]=x2;
								d[n].y[streamSteps+nstep]=y2;
								d[n].z[streamSteps+nstep]=z2;
								d[n].v[streamSteps+nstep]=vv;
								nstepsave=nstep+1;
							}else{
								d[n].x[nstep]=x2;
								d[n].y[nstep]=y2;
								d[n].z[nstep]=z2;
								d[n].v[nstep]=vv;
							}
							x=x1=x2;
							y=y1=y2;
							z=z1=z2;
							if(++nstep >= streamSteps){
								if(k == 0){
								   streamFore[n].done = TRUE;
								}else{
								   streamBack[n].done = TRUE;
								}
								continue;
							}
							if(k == 0){
								streamFore[n].x=x;
								streamFore[n].y=y;
								streamFore[n].z=z;
								streamFore[n].steps=nstep;
								/*
								if(n == 1){
								   fprintf(stderr,"ipass %ld nb %d rank %d x %g y %g z %g nstep %ld\n",
										  (long)ipass,nb,xg.rank,x,y,z,(long)nstep);
								}
								*/
							}else{
								streamBack[n].x=x;
								streamBack[n].y=y;
								streamBack[n].z=z;
								streamBack[n].steps=nstep;
							}
							goto doAgain1;
						}
					}
				
				}
			}
		}
	}	
	
/*
		MPI_Barrier(MPI_COMM_WORLD);
		
	if(xg.rank == xg.root){
	    fprintf(stderr,"streamFore %ld rank %d\n",(long)streamFore[1].steps,xg.rank);
	    for(n=0;n<streamFore[1].steps;++n){
		    fprintf(stderr,"n %ld x %g y %g z %g\n",(long)n,d[1].x[n+streamSteps],d[1].y[n+streamSteps],d[1].z[n+streamSteps]);
		}
	}
					
			
						
		MPI_Barrier(MPI_COMM_WORLD);
*/	
	
	
	{
		int *nf,*nff,nn,smax,ifound;
		double *xx,*yy,*zz,*vv;
		
		xx=NULL;
		yy=NULL;
		zz=NULL;
		vv=NULL;
		
		nf=(int *)cMalloc(xg.size*sizeof(int),8276);
		if(!nf)goto ErrorOut;
		nff=(int *)cMalloc(xg.size*sizeof(int),8276);
		if(!nff)goto ErrorOut;
		
				
	
		for(n=0;n<count;++n){
			int iAm;
		
			for(nn=0;nn<xg.size;++nn){
				nf[nn]=0;
				if(nn == xg.rank)nf[nn]=streamFore[n].steps;
			}
		
			MPI_Reduce ( nf, nff, xg.size, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
			SendToAll(nff,xg.size*sizeof(int));
/*
			fprintf(stderr,"rank %ld n %ld nff ",(long)xg.rank,(long)n);
			for(nn=0;nn<xg.size;++nn){
				  fprintf(stderr," %ld ",(long)nf[nn]);
			   }
			fprintf(stderr,"\n");
*/
			
			smax=-1;
			for(nn=0;nn<xg.size;++nn){
				  if(nff[nn] > smax)smax=nff[nn];
			}

			ifound=0;
			for(nn=0;nn<xg.size;++nn){
				  if(nff[nn] == smax && !ifound){
				      ifound=1;
				  }else{
				      nff[nn]=0;
				  }
			}
			
/*
			if(xg.rank == xg.root){
				fprintf(stderr,"n %ld nff ",(long)n);
			   for(nn=0;nn<xg.size;++nn){
				  fprintf(stderr," %ld ",(long)nff[nn]);
			   }
			   fprintf(stderr,"\n");
			}

*/		
			
			
			/* fprintf(stderr,"1 ipass %ld n %ld rank %ld steps %ld\n ",(long)ipass,(long)n,(long)xg.rank,(long)streamFore[n].steps); */
		
			for(nn=0;nn<xg.size;++nn){

				if(nff[nn] > 0){
				    iAm=nn;
					
					SendToOthers(&streamFore[n],sizeof(struct streamStruct2),iAm);
					/*
					  if(n == 1){
						fprintf(stderr,"SendToOthers ipass %ld n %ld rank %d iAm %ld steps Fore %ld x %g y %g z %g\n",
						 (long)ipass,(long)n,xg.rank,(long)iAm,(long)streamFore[n].steps,
						 streamFore[n].x,streamFore[n].y,streamFore[n].y);
					}
				*/
					SendToOthers(d[n].x+streamSteps,streamFore[n].steps*sizeof(double),iAm);
					SendToOthers(d[n].y+streamSteps,streamFore[n].steps*sizeof(double),iAm);
					SendToOthers(d[n].z+streamSteps,streamFore[n].steps*sizeof(double),iAm);
					SendToOthers(d[n].v+streamSteps,streamFore[n].steps*sizeof(double),iAm);
				}
			}
			
			/* fprintf(stderr,"2 ipass %ld n %ld rank %ld steps %ld\n ",(long)ipass,(long)n,(long)xg.rank,(long)streamFore[n].steps); */
			
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
/*
	if(xg.rank == xg.root){
	    fprintf(stderr,"streamFore %ld\n",(long)streamFore[1].steps);
	    for(n=0;n<streamFore[1].steps;++n){
		    fprintf(stderr,"n %ld x %g y %g z %g\n",(long)n,d[1].x[n+streamSteps],d[1].y[n+streamSteps],d[1].z[n+streamSteps]);
		}
	}
					
*/			
						
		MPI_Barrier(MPI_COMM_WORLD);
		
		for(n=0;n<count;++n){
			int iAm;
		
			for(nn=0;nn<xg.size;++nn){
				nf[nn]=0;
				if(nn == xg.rank)nf[nn]=streamBack[n].steps;
			}
		
			MPI_Reduce ( nf, nff, xg.size, MPI_INT, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		
			SendToAll(nff,xg.size*sizeof(int));
			
			
			smax=-1;
			for(nn=0;nn<xg.size;++nn){
				  if(nff[nn] > smax)smax=nff[nn];
			}

			ifound=0;
			for(nn=0;nn<xg.size;++nn){
				  if(nff[nn] == smax && !ifound){
				      ifound=1;
				  }else{
				      nff[nn]=0;
				  }
			}
			
		
			for(nn=0;nn<xg.size;++nn){

				if(nff[nn] > 0){
				    iAm=nn;
										
					SendToOthers(&streamBack[n],sizeof(struct streamStruct2),iAm);
					/*
						fprintf(stderr,"SendToOthers ipass %ld n %ld rank %d iAm %ld steps Back %ld x %g y %g z %g\n",
						 (long)ipass,(long)n,xg.rank,(long)iAm,(long)streamBack[n].steps,
						 streamBack[n].x,streamBack[n].y,streamBack[n].z);
					*/
					SendToOthers(d[n].x,streamBack[n].steps*sizeof(double),iAm);
					SendToOthers(d[n].y,streamBack[n].steps*sizeof(double),iAm);
					SendToOthers(d[n].z,streamBack[n].steps*sizeof(double),iAm);
					SendToOthers(d[n].v,streamBack[n].steps*sizeof(double),iAm);
				}
			}
		}
					
		cFree((char *)nf);
		cFree((char *)nff);
	}
		
	MPI_Barrier(MPI_COMM_WORLD);
		
	idone=0;
		
	for(n=0;n<count;++n){
		if(streamFore[n].done)idone += 1;
		if(streamBack[n].done)idone += 1;
	}
	
	
	++ipass;
	
	if(idone < 2*count && ipass < streamPasses)goto StartAgain;
	
	if(idone < 2*count){
		sprintf(WarningBuff," %ld pass(es) %ld Streamsline pieces finished out of %ld for Frame %ld\n",(long)ipass,
		                     (long)idone,(long)count*2,CurrentFrame);
		WarningBatch(WarningBuff);
	}
	
	MPI_File_close(&mpifile);
	
	
/*	
	if(xg.rank == xg.root){
	    for(n=0;n<streamFore[1].steps;++n){
		    fprintf(stderr,"n %ld x %g y %g z %g\n",(long)n,d[1].x[n+streamSteps],d[1].y[n+streamSteps],d[1].z[n+streamSteps]);
		}
	    for(n=0;n<streamBack[1].steps;++n){
		    fprintf(stderr,"n %ld x %g y %g z %g\n",(long)n,d[1].x[n],d[1].y[n],d[1].z[n]);
		}
	}
*/
	
	for(n=0;n<count;++n){
		nstepsave=streamFore[n].steps;
		nstep=streamBack[n].steps;
		if(nstep > 0 || nstepsave > 0){
		    nn=0;
	        xs=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),6721);
	        ys=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),6721);
	        zs=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),6721);
	        vs=(double *)cMalloc((nstep+nstepsave+3)*sizeof(double),6721);
	        if(!xs || !ys ||!zs || !vs){
	            WarningBatch("doSage3DGetStreamLine Out of Memory\n");
	            goto ErrorOut;
	        }
			
	        if(nstep > 0){
	            for(k=0;k<nstep;++k){
	                xs[nn]=d[n].x[nstep-1-k];
	                ys[nn]=d[n].y[nstep-1-k];
	                zs[nn]=d[n].z[nstep-1-k];
	                vs[nn]=d[n].v[nstep-1-k];
	                ++nn;
	            }
	        }

	        
	        if(nstepsave > 0){
	            for(k=0;k<nstepsave;++k){
	                xs[nn]=d[n].x[streamSteps+k];
	                ys[nn]=d[n].y[streamSteps+k];
	                zs[nn]=d[n].z[streamSteps+k];
	                vs[nn]=d[n].v[streamSteps+k];
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
	
/*
	if(xg.rank == xg.root){
	    for(n=0;n<sd->streamline3d.xyzReturned[1].count;++n){
		    fprintf(stderr,"n %ld x %g y %g z %g\n",(long)n,sd->streamline3d.xyzReturned[1].x[n],
			sd->streamline3d.xyzReturned[1].y[n],sd->streamline3d.xyzReturned[1].z[n]);
		}
	}
*/

/*
	fprintf(stderr,"doSage3DGetStreamLineBlk2 rank %d streamsReturned %ld\n",xg.rank,(long)streamsReturned);
	
	for(n=0;n<streamsReturned;++n){
		fprintf(stderr,"rank %d streamsReturned %ld count %ld\n",xg.rank,(long)n,(long)sd->streamline3d.xyzReturned[n].count);
	}
*/
	ret=0;
ErrorOut:
	sd->streamline3d.streamsReturned=streamsReturned;
	
	BlksEnd(Files,flags);
	
    if(streamFore)cFree((char *)streamFore);
    if(streamBack)cFree((char *)streamBack);
	
	if(d){
		for(n=0;n<count;++n){
			if(d[n].x)cFree((char *)d[n].x);
			if(d[n].y)cFree((char *)d[n].y);
			if(d[n].z)cFree((char *)d[n].z);
			if(d[n].v)cFree((char *)d[n].v);
		}
		if(d)cFree((char *)d);
    }

    return ret;
	
}

int doSage3DGetAreaDataBlk2(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	long xsize,ysize;
	double *data;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;
	
		

	if(ai->type != GENERAL_PLANE_DATA  && ai->type != AREADATA_AREA_DATA  &&
	   ai->type != GENERAL_OFFSET_DATA && ai->type != AREADATA_GRID_DATA){
	    sprintf(WarningBuff,"doSage3DGetAreaDataBlk2 - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	
	CurrentFrame=ai->CurrentFrame;
	
	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}

		
	ai->pioTime=Files->pioTime;
	
/*
	fprintf(stderr,"1 doSage3DGetAreaDataBlk2 rank %d type %ld\n",xg.rank,(long)ai->type);
*/

	if(ai->type == GENERAL_PLANE_DATA){
	    return doSage3DGetGeneratData(ai,Files);
	}else if(ai->type == GENERAL_OFFSET_DATA){
	    return doSage3DGetGeneratDataOffSet(ai,Files);
	}else if(ai->type == AREADATA_GRID_DATA){
	    return doSage3DGetGridData(ai,Files);
	}
/*
	fprintf(stderr,"2 doSage3DGetAreaDataBlk2  rank %d  type %ld\n",xg.rank,(long)ai->type);
*/
	
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
	    
	    
	    ai->data=doSage3DGetSDS3DBlk2(Files,CurrentFrame);
	    
	
	    if(!ai->data){
	        WarningBatch("doSage3DGetAreaData Error in doSage3DGetSDS3DBlk\n");
	        return 1;
	    }else{
	        return 0;
	    }
	    
	}
	
	fprintf(stderr,"4 doSage3DGetAreaDataBlk2  rank %d  type %ld\n",xg.rank,(long)ai->type);
			
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
double *doSage3DGetSDS3DBlk2(struct FileInfo2 *Files,long CurrentFrame)
{
	MPI_File mpifile;
	long BlockSize,BlockSizeSave,numcell,numcellSave;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double didx,djdy,dkdz,dmin,dmax;
	double dxdi,dydj,dzdk;
	long ixmax,iymax,izmax;
	unsigned long flags;
	double *sout;
	long n,length,nn,nnn;
	long error;
	int ret;
 	
	if(!Files)return NULL;
	pio = &Files->pioData;
	

	ret=1;
	sout=NULL;

	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;

	pio->CurrentFrame=CurrentFrame;
		
	flags=B_3D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
	}
	
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
	
	if(BlksStart(Files,flags))goto ErrorOut;
		    	    
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
	if(!sout)goto ErrorOut;

	for(n=0;n<length;++n)sout[n] = -1e66;
	
    didx=((double)(ixmax))/(xmax-xmin);
    dxdi = 1.0/didx;
    djdy=((double)(iymax))/(ymax-ymin);
    dydj = 1.0/djdy;
    dkdz=((double)(izmax))/(zmax-zmin);
    dzdk = 1.0/dkdz;
    
	
	dmin =  1e60;
	dmax = -1e60;
    error=0;
    	
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

	numcell=pio->numcell;
	BlockSize=pio->BlockSize;
	
	for(nnn=0;nnn<block->nBlocks;nnn += xg.size){
        int kk;
		for(kk=0;kk<xg.size;++kk){
		    int nb=(nnn + kk) % (int)(block->nBlocks);
			int np=(nnn + kk) % xg.size;
			
			if(np == xg.rank){
			
				BlockSize=block->nBlockSize[nb];
				
				pio->numcell=BlockSize;
				
				if(BlksReadMPI(mpifile,Files,flags,nb,block->xBlockSize[nb],CurrentFrame))goto ErrorOut;
				
				doRangeBlock(Files);

				if(pio->doGradients){		
					if(doBlockGradients(Files,nb,BlockSize,CurrentFrame))goto ErrorOut;
					if(!pio->gradx || !pio->grady || !pio->gradz){
						pio->doGradients = FALSE;
					}
				}
			
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
			}
		}
	}
	
	MPI_File_close(&mpifile);
	
	
	{
		double *soutg,dd;
		
		soutg=NULL;
		if(xg.rank == xg.root){
			soutg=(double *)cMalloc(length*sizeof(double),9786);
			if(!soutg)goto ErrorOut;
		}
	
	
		MPI_Reduce ( sout, soutg, length, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		if(xg.rank == xg.root){
			if(sout)cFree((char *)sout);
			sout=soutg;
		}
		
		MPI_Reduce ( &dmax, &dd, 1, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
		
		dmax=dd;
	
		MPI_Reduce ( &dmin, &dd, 1, MPI_DOUBLE, MPI_MIN, xg.root, MPI_COMM_WORLD );
		
		dmin=dd;
	
	}

	pio->plane.Plane_dmin=dmin;
	pio->plane.Plane_dmax=dmax;
	
		/* fprintf(stderr,"doSage3DGetSDS3DBlk2  OK rank %d\n",xg.rank); */

	ret=0;
ErrorOut:
    if(ret){
        if(sout)cFree((char *)sout);
        sout=NULL;
    }
    
	BlksEnd(Files,flags);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	
	
	return sout;
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
					      
	MPI_File mpifile;
	long BlockSize,BlockSizeSave,numcell,numcellSave,nn;
	unsigned long flags;
	long CurrentFrame,BufferSize;
    double xx[4],yy[4],zz[4];
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	double *x,*y,*z,*v;
	double dx,dy,dz;
	
	int k,ZoneLevel,m,i;
	long n,ne;
	int ret;
	
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
	
	x=y=z=v=NULL;

	ZoneLevel=ai->pa.ZoneLevel;
	if(ai->pa.DrawZones < 2)ZoneLevel=99999;
	
	ret = 1;
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;
	CurrentFrame=ai->CurrentFrame;
		
	flags=B_3D_STANDARD;
	
	BlockSize=pio->BlockSize;
		
		
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

	if(BlksStart(Files,flags))goto ErrorOut;
		    	    
	
	BufferSize=10000;
	x=(double *)cMalloc(BufferSize*4*sizeof(double),5921);
	y=(double *)cMalloc(BufferSize*4*sizeof(double),5922);
	z=(double *)cMalloc(BufferSize*4*sizeof(double),5923);
	v=(double *)cMalloc(BufferSize*sizeof(double),5924);
	if(!x || !y || !z || !v)goto ErrorOut;
	
	numcell=pio->numcell;
	BlockSize=pio->BlockSize;
	
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
	
    ne=0;
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
						if(ne >= BufferSize){
							double *xx,*yy,*zz,*vv;
							BufferSize *= 2;
							xx=(double *)cRealloc((char *)x,BufferSize*4*sizeof(double),5921);
							if(!xx)goto ErrorOut;
							x=xx;
							yy=(double *)cRealloc((char *)y,BufferSize*4*sizeof(double),5922);
							if(!yy)goto ErrorOut;
							y=yy;
							zz=(double *)cRealloc((char *)z,BufferSize*4*sizeof(double),5923);
							if(!zz)goto ErrorOut;
							z=zz;
							vv=(double *)cRealloc((char *)v,BufferSize*sizeof(double),5924);
							if(!vv)goto ErrorOut;
							v=vv;
						}
			Done2:		;
					}
				}
			}
		}
	}
	
	
	MPI_File_close(&mpifile);
	
	
	
	{
		int *nf,*nff,len;
		MPI_Status status;
		double *xx,*yy,*zz,*vv;
		
		xx=NULL;
		yy=NULL;
		zz=NULL;
		vv=NULL;
		
		nf=(int *)cMalloc(xg.size*sizeof(int),8276);
		if(!nf)goto ErrorOut;
		nff=(int *)cMalloc(xg.size*sizeof(int),8276);
		if(!nff)goto ErrorOut;
		for(n=0;n<xg.size;++n){
		   nf[n]=0;
		   if(n == xg.rank)nf[n]=ne;
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
		len=0;
		for(n=0;n<xg.size;++n){
		   len += nff[n];
		}
		
		if(xg.rank == xg.root){
			xx=(double *)cMalloc(len*sizeof(double)*4,5921);
			yy=(double *)cMalloc(len*sizeof(double)*4,5922);
			zz=(double *)cMalloc(len*sizeof(double)*4,5923);
			vv=(double *)cMalloc(len*sizeof(double),5924);
			if(!xx || !yy || !zz || !vv)goto ErrorOut;
			if(nff[0] > 0){
				for(n=0;n<nff[0];++n){
					vv[n]=v[n];
				}
				for(n=0;n<nff[0]*4;++n){
					xx[n]=x[n];
					yy[n]=y[n];
					zz[n]=z[n];
				}
			}
		}


		MPI_Barrier(MPI_COMM_WORLD);
		
		for(n=1;n<xg.size;++n){
		   if(nff[n] > 0){
		        if(n == xg.rank){
					MPI_Send(x, nff[n]*4, MPI_DOUBLE, xg.root, 1, MPI_COMM_WORLD);
					MPI_Send(y, nff[n]*4, MPI_DOUBLE, xg.root, 2, MPI_COMM_WORLD);
					MPI_Send(z, nff[n]*4, MPI_DOUBLE, xg.root, 3, MPI_COMM_WORLD);
					MPI_Send(v, nff[n], MPI_DOUBLE, xg.root, 4, MPI_COMM_WORLD);
					 /* fprintf(stderr,"send send send done done -----5 rank %ld nd %ld\n",(long)xg.rank,(long)nff[n]); */
				}
		   }
		}
		
		len=nff[0];
		for(n=1;n<xg.size;++n){
		   if(nff[n] > 0){
		        if(xg.root == xg.rank){				
					MPI_Recv(xx+len*4, nff[n]*4, MPI_DOUBLE, n, 1, MPI_COMM_WORLD,&status);
					MPI_Recv(yy+len*4, nff[n]*4, MPI_DOUBLE, n, 2, MPI_COMM_WORLD,&status);
					MPI_Recv(zz+len*4, nff[n]*4, MPI_DOUBLE, n, 3, MPI_COMM_WORLD,&status);
					MPI_Recv(vv+len, nff[n], MPI_DOUBLE, n, 4, MPI_COMM_WORLD,&status);
					/* fprintf(stderr,"receive receive receive done done ---- 6 rank %ld nd %ld\n",(long)xg.rank,(long)nff[n]); */
				}
		   }
		   len += nff[n];
		}
		
		
		
		if(xg.rank == xg.root){
			if(ne > 0){
				if(ai->uDraw && ai->info){
					if((*ai->uDraw)(ai->info,xx,yy,zz,vv,len))goto ErrorOut;
				}
				xx=NULL;
				yy=NULL;
				zz=NULL;
				vv=NULL;
			}
			
		}
	
		MPI_Barrier(MPI_COMM_WORLD);
		
		cFree((char *)nf);
		cFree((char *)nff);
	}
	
	ret = 0;
	
ErrorOut:
	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	if(v)cFree((char *)v);
	
	BlksEnd(Files,flags);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	return  ret;
}

static int doCell(long ne,struct DrawInfo3 *info)
{
    struct FilePIOInfo *pio;
	double r1,r2,s1,s2,t1,t2,dx,dy,dz,dr,ds;
	double xc,yc,zc,dx2,dy2,dz2;
	double x,y,z,r,s,t,rx,ry,rz,sx,sy,sz,tx,ty,tz;
	long k,nx,ny;
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
static int doSage3DGetGeneratDataOffSet(struct areadata *ai,struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct DrawInfo3 info;	
	struct PIO_BLK *sage;
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
	sage=&pio->sageBlk;
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
	MPI_File mpifile;
	long BlockSize,BlockSizeSave,numcell,numcellSave;
    struct FilePIOInfo *pio;
	struct DrawInfo3 info;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	double *data;
	unsigned long flags;
	long CurrentFrame;
	long length;
	int ret;
	long n,nn;
	
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
	
	
	length=ai->xsize*ai->ysize;
	data=(double *)cMalloc(length*sizeof(double),72635);
	if(!data)return 1;
	
	for(n=0;n<length;++n)data[n]=-1e66;
	
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
	
	ret = 1;
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;
	CurrentFrame=ai->CurrentFrame;
		
	flags=B_3D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
	}
	
	BlockSize=pio->BlockSize;
		
		
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

	if(BlksStart(Files,flags))goto ErrorOut;
	
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
					if(!pio->gradx || !pio->grady || !pio->gradz){
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

	/* fprintf(stderr,"doSage3DGetGeneratData OK rank %d\n",xg.rank); */
	
    ret = 0;
ErrorOut:    
	BlksEnd(Files,flags);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	return  ret;
}


int doSage3DGetLineDataBlk2(struct FileInfo2 *Files,struct linedata *li)
{
	long BlockSize,BlockSizeSave,numcell,numcellSave;
	int ix1,iy1,ix2,iy2;
	double x1,y1,z1,x2,y2,z2,dl,ds,dx,dz,dy,dp;
	long ns,n,nn,nd;
	double *xD,*yD,*zD,*fD,*fDD;
	double rxmin,rxmax,rymin,rymax,amax;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	unsigned long flags;
	int ret;
	MPI_File mpifile;
	
	if(!li)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	
/*
	fprintf(stderr,"1 type %ld\n",(long)li->type);
*/
	
	if(li->type == LINEDATA_POINT_DATA){
	     return doPointGetData(Files,li);
	}else if(li->type == LINEDATA_LIST_DATA){
	     return doPointGetListData(Files,li);
	}
	
/*
	fprintf(stderr,"2 type %ld\n",(long)li->type);
*/

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

/*
	fprintf(stderr,"3 type %ld\n",(long)li->type);
*/
	
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


	CurrentFrame=li->CurrentFrame;
		
	flags=B_3D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
	}
	
	BlockSize=pio->BlockSize;
			    
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
/*
	fprintf(stderr,"3 type %ld ns %ld CurrentFrame %ld path %s\n",(long)li->type,ns,CurrentFrame,Files->mpiPath);
*/

	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

    setPioScales(pio);
	
	if(BlksStart(Files,flags))goto ErrorOut;
		    	  
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81875))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81883))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}

	if(!(zD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81883))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}

	if(!(fD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81883))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}

	if(!(fDD=(double *)cMalloc((long)sizeof(double)*(ns+2*xg.size),81883))){
	    WarningBatch("doSage3DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}

	nd=0;
	for(n=0;n<ns;++n){
	    double x,y,z;

	    x=x1+(double)n*ds*(x2-x1);
	    y=y1+(double)n*ds*(y2-y1);
	    z=z1+(double)n*ds*(z2-z1);

	    xD[n]=x;
	    yD[n]=y;
	    zD[n]=z;
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
					double value,x,y,z;
					
					/* int k; */

					if(fD[n] > 0)continue;
					x=xD[n];
					y=yD[n];
					z=zD[n];

					if(!pioGetValue(pio,x,y,z,&value))continue;
/*
		        k=(int)(pio->level[pioGetValue_ne]);
        
				
					fprintf(stderr,"n %ld rank %ld nb %ld ne %ld xc %g yc %g zc %g z %g dz %g\n",(long)n,(long)xg.rank,(long)nb,(long)pioGetValue_ne,
					        pio->xcenter[pioGetValue_ne],pio->ycenter[pioGetValue_ne],pio->zcenter[pioGetValue_ne],z,pio->dz2[k]);
							
*/
					dx=x-x1;
					dy=y-y1;
					dz=z-z1;
					dp=sqrt(dx*dx+dy*dy+dz*dz);

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
	if(xg.rank == xg.root){
		for(n=0;n<ns;++n){
			if(fD[n] > 0){
			   xD[nd]=xD[n];
			   yD[nd]=yD[n];
			   ++nd;
			}
		}
	}
	
	if(!nd)goto ErrorOut;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	xD=NULL;
	yD=NULL;
	
	ret=0;

ErrorOut:

	BlksEnd(Files,flags);

	if(fD)cFree((char *)fD);
	if(zD)cFree((char *)zD);
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(fDD)cFree((char *)fDD);
	return ret;
}
static int doIntegrateLine(struct FileInfo2 *Files,struct linedata *li)
{
	MPI_File mpifile;
	long nff,BlockSize,BlockSizeSave,numcell,numcellSave,nn;
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	double *xD,*yD,*zD,s,s1,s2,s3,s4,s5,s6;
	double x1,y1,x2,y2,x,y,xo,yo,dx,dy,integral;
	double z,zo,z1,z2,dz,rz,zz,zc,dz2,zmin,zmax,dx2,dy2;
	double length;
	long nd,np,ne,n,ns;
	long np1,np2;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,ymin,ymax;
	double llength;
	long CurrentFrame;
	struct sortdata *sort;
	double rx,ry,mx,yy,xx;
	double sl,sh;
	double dl;
	int i1,i2;
	int iDone;
	int ihit;
	int ret;
	

	if(!li)return 1;
	if(!Files)return 1;	
	pio=&Files->pioData;
	
	ret=1;
	nd=0;
    xD=NULL;
    yD=NULL;
    zD=NULL;
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

	
	z1=li->z1;
	z2=li->z2;
	rz=z2-z1;
		
	llength=sqrt(rx*rx+ry*ry+rz*rz);

	mx = 1.0e-6*llength;

	pio->doGradients = FALSE;

    setPioScales(pio);
		
	np=10000;
	
	if(!(sort=(struct sortdata *)cMalloc((long)sizeof(struct sortdata)*(np),81876))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	
	CurrentFrame=li->CurrentFrame;
	
	/* fprintf(stderr,"doIntegrateLine %ld CurrentFrame %ld\n",(long)xg.rank,(long)CurrentFrame); */
		
	BlockSize=pio->BlockSize;
		    
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

	if(BlksStart(Files,B_3D_STANDARD))goto ErrorOut;
		    
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
				xmin=pio->xmin-dx2*1.0000001;
				xmax=pio->xmax+dx2*1.0000001;
				
				dy2=pio->ymax-pio->ymin;
				ymin=pio->ymin-dy2*1.0000001;
				ymax=pio->ymax+dy2*1.0000001;
				
				dz2=pio->zmax-pio->zmin;
				zmin=pio->zmin-dz2*1.0000001;
				zmax=pio->zmax+dz2*1.0000001;
					
				i1=0;
				i2=0;
				if(x1 < xmin || x1 > xmax)i1=1;
				if(x2 < xmin || x2 > xmax)i2=1;
				if(y1 < ymin || y1 > ymax)i1=1;
				if(y2 < ymin || y2 > ymax)i2=1;
				if(z1 < zmin || z1 > zmax)i1=1;
				if(z2 < zmin || z2 > zmax)i2=1;
				if(i1 == 0 || i2 == 0){
					goto Inside1;
				}
				
				ihit=0;
				if(fabs(rx) > mx){
					s1=((pio->xmin)-x1)/rx;
					yy=y1+s1*ry;
					if(yy >= ymin && yy <= ymax){
						zz=z1+s1*rz;
						if(zz >= zmin && zz <= zmax){
							if(s1 >= -1e-6 && s1 <= 1.0000001)ihit |= 1;   /* hit low x */
						}
					}	        
					s2=((pio->xmax)-x1)/rx;
					yy=y1+s2*ry;
					if(yy >= ymin && yy <= ymax){
						zz=z1+s2*rz;
						if(zz >= zmin && zz <= zmax){
							if(s2 >= -1e-6 && s2 <= 1.0000001)ihit |= 2;   /* hit high x */
						}
					}	        
				}        
					
				if(fabs(ry) > mx){
					s3=((pio->ymin)-y1)/ry;
					xx=x1+s3*rx;	                
					if(xx >= xmin && xx <= xmax){
						zz=z1+s3*rz;
						if(zz >= zmin && zz <= zmax){
							if(s3 >= -1e-6 && s3 <= 1.0000001)ihit |= 4;   /* hit low x */
						}
					}	        
					s4=((pio->ymax)-y1)/ry;
					xx=x1+s4*rx;	                
					if(xx >= xmin && xx <= xmax){
						zz=z1+s4*rz;
						if(zz >= zmin && zz <= zmax){
							if(s4 >= -1e-6 && s4 <= 1.0000001)ihit |= 8;   /* hit low x */
						}
					}	        
				}
				
				if(fabs(rz) > mx){
					s5=((pio->zmin)-z1)/rz;
					xx=x1+s5*rx;	                
					if(xx >= xmin && xx <= xmax){
						yy=y1+s5*ry;
						if(yy >= ymin && yy <= ymax){
							if(s5 >= -1e-6 && s5 <= 1.0000001)ihit |= 16;   /* hit low z */
						}
					}	        
					s6=((pio->zmax)-z1)/rz;
					xx=x1+s6*rx;	                
					if(xx >= xmin && xx <= xmax){
						yy=y1+s6*ry;
						if(yy >= ymin && yy <= ymax){
							if(s6 >= -1e-6 && s6 <= 1.0000001)ihit |= 32;   /* hit low z */
						}
					}	        
				}
				
				if(ihit == 0){
					goto NextBlock;
				}
		/*   
			No Gradients for integrated lines
			
		*/
				
		Inside1:
				for(ne=0;ne<pio->numcell;++ne){
					double value,xc,yc,dl,dl1;
					long k;
					if(pio->daughter[ne] || !pio->active[ne])continue;		    
					
					
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

					s1=s2=s3=s4=s5=s6=-1;
					
					i1=0;
					i2=0;
					if(x1 < xmin || x1 > xmax)i1=1;
					if(x2 < xmin || x2 > xmax)i2=1;
					if(y1 < ymin || y1 > ymax)i1=1;
					if(y2 < ymin || y2 > ymax)i2=1;
					if(z1 < zmin || z1 > zmax)i1=1;
					if(z2 < zmin || z2 > zmax)i2=1;
					if(i1 == 0 && i2 == 0){
					  sl=0.0;
					  sh=1.0;
					  goto BothInside;
					}

					ihit=0;
					if(fabs(rx) > mx){
						s1=((xc-dx2)-x1)/rx;
						yy=y1+s1*ry;
						if(yy >= ymin && yy <= ymax){
							zz=z1+s1*rz;
							if(zz >= zmin && zz <= zmax){
								if(s1 >= 0 && s1 <= 1.0)ihit |= 1;   /* hit low x */
							}
						}	        
						s2=((xc+dx2)-x1)/rx;
						yy=y1+s2*ry;
						if(yy >= ymin && yy <= ymax){
							zz=z1+s2*rz;
							if(zz >= zmin && zz <= zmax){
								if(s2 >= 0 && s2 <= 1.0)ihit |= 2;   /* hit high x */
							}
						}	        
					}        
						
					if(fabs(ry) > mx){
						s3=((yc-dy2)-y1)/ry;
						xx=x1+s3*rx;	                
						if(xx >= xmin && xx <= xmax){
							zz=z1+s3*rz;
							if(zz >= zmin && zz <= zmax){
								if(s3 >= 0 && s3 <= 1.0)ihit |= 4;   /* hit low x */
							}
						}	        
						s4=((yc+dy2)-y1)/ry;
						xx=x1+s4*rx;
						
						if(xx >= xmin && xx <= xmax){
							zz=z1+s4*rz;
							
							if(zz >= zmin && zz <= zmax){
								if(s4 >= 0 && s4 <= 1.0)ihit |= 8;   /* hit low x */
							}
						}	
					}
					
					
					if(fabs(rz) > mx){
						s5=((zc-dz2)-z1)/rz;
						xx=x1+s5*rx;	                
						if(xx >= xmin && xx <= xmax){
							yy=y1+s5*ry;
							if(yy >= ymin && yy <= ymax){
								if(s5 >= 0 && s5 <= 1.0)ihit |= 16;   /* hit low z */
							}
						}	        
						s6=((zc+dz2)-z1)/rz;
						xx=x1+s6*rx;	                
						if(xx >= xmin && xx <= xmax){
							yy=y1+s6*ry;
							if(yy >= ymin && yy <= ymax){
								if(s6 >= 0 && s6 <= 1.0)ihit |= 32;   /* hit low z */
							}
						}	        
					}
					
					
					
					
					if(ihit == 0)continue;
					sl=sh=-1;
					
					{
						double s[6];
						int mm;
						s[0]=(ihit & 1)  ? s1 : -1.0;
						s[1]=(ihit & 2)  ? s2 : -1.0;
						s[2]=(ihit & 4)  ? s3 : -1.0;
						s[3]=(ihit & 8)  ? s4 : -1.0;
						s[4]=(ihit & 16) ? s5 : -1.0;
						s[5]=(ihit & 32) ? s6 : -1.0;
						
						qsort((char *)s,6, sizeof(double),intcmp2);

						for(mm=0;mm<6;++mm){
							if(s[mm] >= 0 && s[mm] <= 1.0){
								if(sl < 0){
									sl=s[mm];
								}else{
									sh=s[mm];
								}
							}
						}
						if(sl < -1e-6)continue;

						if(i1 == 0){
							if(sh < 0)sh=sl;
							sl=0;
							np1=ne+nff;
						}
						
						if(i2 == 0){
							sh=1.0;
							np2=ne+nff;
						}
					
						if(sh == sl)continue;
						
						if(sh < 0){
						/*
							sprintf(WarningBuff,"Error Zone %ld Block Offset %ld ihit %x\n",ne+nff,nff,ihit);
							WarningBatch(WarningBuff);
							sprintf(WarningBuff,"ne %ld nff %ld s1 %g s2 %g s3 %g s4 %g s5 %g s6 %g\n",ne+nff,nff,s1,s2,s3,s4,s5,s6);
							WarningBatch(WarningBuff);
						*/
							continue;  /* error */
						}
						

						
					}
					
					
					
		BothInside:		    
					if(sl > sh){
						s=sl;
						sl=sh;
						sh=s;
					}
					
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
					zo=z1+sl*rz;	
					
					dl1=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y)+(zo-z)*(zo-z));

					sort[nd].x=dl1;
					sort[nd].y=value;
					sort[nd].ne=ne+nff;
					sort[nd].sl=sl;

					if(dl1 < rxmin)rxmin=dl1;
					if(dl1 > rxmax)rxmax=dl1;
					if(value < rymin)rymin=value;
					if(value > rymax)rymax=value;

					xo=x1+sh*rx;
					yo=y1+sh*ry;	
					zo=z1+sh*rz;	
					
					dl=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y)+(zo-z)*(zo-z));

					sort[nd].x2=dl;
					sort[nd].y2=value;
					sort[nd].ne=ne+nff;
					sort[nd].sh=sh;
					nd++;

					if(dl < rxmin)rxmin=dl;
					if(dl > rxmax)rxmax=dl;
					if(value < rymin)rymin=value;
					if(value > rymax)rymax=value;
					
							
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
	
	qsort((char *)sort,nd, sizeof(struct sortdata),intcmp1);
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(nd*2),81876))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(nd*2),81883))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}	
	
	
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
			sprintf(WarningBuff,"rank %d test %ld %ld x %f x2 %f  dl %f sl %f sh %f\n",xg.rank,np,sort[np].ne,sort[np].x,sort[np].x2,dl,sort[np].sl,sort[np].sh);
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
		     sprintf(WarningBuff,"x1 %15.10g y1 %15.10g z1 %15.10g x2 %15.10g y2 %15.10g z2 %15.10g\n",x1,y1,z1,x2,y2,z2);
		     WarningBatch(WarningBuff);
		}
	}
	
	/*
		    sprintf(WarningBuff,"Blocked Last %g %g %g %g %g %g  %g %g\n",xo,yo,zo,x2,y2,z2,llength,length);
		    WarningBatch(WarningBuff);
	*/
	
	
	if(np1 < 0){
	     sprintf(WarningBuff,"Point One x1 %f y1 %f z1 %f not Found\n",x1,y1,z1);
	     WarningBatch(WarningBuff);
	}
	if(np2 < 0){
	     sprintf(WarningBuff,"Point Two x2 %f y2 %f z2 %f not Found\n",x2,y2,z2);
	     WarningBatch(WarningBuff);
	}
		
	xD=NULL;
	yD=NULL;
	
	ret=0;
	
ErrorOut:
OutOfHere:

	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	if(sort)cFree((char *)sort);
	
	BlksEnd(Files,B_3D_STANDARD);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	
	return ret;
}

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
	long BlockSize,BlockSizeSave,numcell,numcellSave;
	MPI_File mpifile;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
    struct FilePIOInfo *pio;
	long CurrentFrame;
	unsigned long flags;
	long n,nn;
	long ifound;
	int ret;

	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
		
	if(li->type != LINEDATA_LIST_DATA)return 1;
	
	if(!li->x || !li->y || !li->z || !li->v || (li->PointLineCount < 1))return 1;
	   
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
		
	flags=B_3D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
	}
	
	BlockSize=pio->BlockSize;
		
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;

    setPioScales(pio);
	  
	if(BlksStart(Files,flags))goto ErrorOut;
	
	for(n=0;n<li->PointLineCount;++n){	    
		li->v[n]= -2e66;
	}
 	
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
	
	numcell=pio->numcell;
	BlockSize=pio->BlockSize;
	
	ifound=0;
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
					if(!pioGetValue(pio,li->x[n],li->y[n],li->z[n],&li->v[n])){
						continue;
					}	    
					++ifound;
				}
			}
		}
	}
	
	MPI_File_close(&mpifile);
	
	{
		double *data=NULL;
		
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

	ret=0;
	
ErrorOut:


	BlksEnd(Files,flags);
    
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
    
	return ret;
}
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li)
{
	MPI_File mpifile;
	double dp;
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt1,nt2;
	double valuet;
	double r,theta1,theta2,dr,dtheta;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long BlockSize;
	double x,y,z;
	double xs,ys,zs;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	
    double rMax,thetaMax1,thetaMax2;
    unsigned long flags;
    int ret,nn;

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	ret=1;
	xD=NULL;
	yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;
	
	if(li->type != LINEDATA_POINT_DATA)return 1;
	
	flags=B_3D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
	}

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
	    	goto ErrorOut;
	    }
	}
	
	if(nLast >= Files->ImageCount){
		nLast=Files->ImageCount-1;
		if(nLast < 0)nLast=0;
	}
	
	if(nFirst >= nLast)nFirst=0;

	ns=nLast-nFirst+1;

	if(ns < 2)goto ErrorOut;

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81872))){
	    WarningBatch("Out of List Memory\n");
	    goto ErrorOut;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81882))){
	    WarningBatch("Out of List Memory\n");
	    goto ErrorOut;
	}

	tFirst=li->FirstTime;
	tLast=li->LastTime;
	if(tFirst >= tLast){
	    if(Files){
	    	tFirst=0;
	    	tLast=(double)(Files->ImageCount-1);
	    }else{
	    	goto ErrorOut;
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

	BlockSize=pio->BlockSize;
		
	if(doSage3DCheckBlks(Files,nFirst))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
			    	  	
    setPioScales(pio);	

	if(BlksStart(Files,flags))goto ErrorOut;
	
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
	    
		if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
		
		sage=&pio->sageBlk;
		if(!sage)goto ErrorOut;
		block=&sage->block;

		MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

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
						if(!pio->gradx || !pio->grady || !pio->gradz){
							pio->doGradients = FALSE;
						}
					}

					x=xs;
					y=ys;
					z=zs;
					
					r=dr;
					for(nr=0;nr<NumberOfRings;++nr){
						theta1=0;
						for(nt1=0;nt1<NumberOfPoints;++nt1){
							theta2=0;
							for(nt2=0;nt2<NumberOfPoints;++nt2){
								if(!pioGetValue(pio,x,y,z,&value))continue;
								
								valuet += value;
								++nf;
								
								x=xs+r*cos(theta1)*sin(theta2);
								y=ys+r*sin(theta1)*sin(theta2);
								z=zs+r*cos(theta1);
								
								theta2 += 0.5*dtheta;
								if(theta2 > thetaMax2)thetaMax2=theta2;
								
							}
							if(theta1 > thetaMax1)thetaMax1=theta1;
							theta1 += dtheta;
						}
						if(r > rMax)rMax=r;
						r += dr;
					}
				
					if(nf >= NumberOfRings*NumberOfPoints)goto breakOut;
				}
			}
		}
breakOut:	    
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
			/* printf("Missed It - %d \n",n); */
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

	if(!nd)goto ErrorOut;
/*
fprintf(stderr,"rank %ld nd = %ld\n",(long)xg.rank,(long)nd);
*/

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	xD=NULL;
	yD=NULL;
	
	ret = 0;
ErrorOut:


	BlksEnd(Files,flags);
	
    if(xD)cFree((char *)xD);
    if(yD)cFree((char *)yD);
	return ret;
}
int Sage3DGetDataBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	struct LIMITS *limits;
	long length;
    double *sout;
    int ret;

	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	limits=&Files->limits;
	ret=1;
	
	pio->BlockSize=pio->pa.BlockSize=sd->pa.BlockSize;
		
	pio->doGradients=FALSE;
	
	if(doSage3DCheckBlks(Files, CurrentFrame))goto ErrorOut;
	
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
		    sprintf(WarningBuff,"doSage3DGetDataBlk - Only Returns FRAME_DATA_FLOAT and FRAME_DATA_RASTER\n");
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
		pio->plane  = sd->plane;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
		pio->pa     = sd->pa;		
	}
		        		
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9206);
    if(!sd->data){
    	sprintf(WarningBuff,"doSage3DGetDataBlk - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }

    sout=(double *)sd->data;
    
    if(sd->type == FRAME_DATA_FLOAT){
    
		setFloat(sout,length);
	    if(getPlotData(Files,sout,CurrentFrame)){
		    sprintf(WarningBuff,"doSage3DGetDataBlk - getPlotData Error\n");
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

	setFloat(sd->data,length);
    if(getPlotData(Files,sd->data,CurrentFrame)){
	    sprintf(WarningBuff,"FloatAndRaster - getPlotData Error\n");
	    WarningBatch(WarningBuff);
		goto ErrorOut;
    }	

    zerol((char *)sd->data2,length);
    if(getPlotImage(Files,sd->data2,CurrentFrame)){
		goto ErrorOut;
    }	
  
    clearData(&sd->data2);
	
    clearData(&sd->data);
	
	sd->pd.dmin=pio->cmin;
	sd->pd.dmax=pio->cmax;	
    sd->limits = *limits;	
    
    ret=0;
    
ErrorOut:

	return ret;

}

int BlksReadMPI(MPI_File mpifile,struct FileInfo2 *Files,unsigned long flags,long nb,long BlockSize,long CurrentFrame)
{
	struct HeaderBlock *block;
	unsigned char header[9];
	double two;
	double twoLocal=2.0;
	struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
	MPI_Status status;
	MPI_Offset offset;
	int itemLocation;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	ret = 1;
	
	
	if(BlockSize > sage->xBlockSizeMax){
		ret=322;goto ErrorOut;
	}

	offset=block->xBlockAdress[nb];
				
	MPI_File_seek(mpifile, offset, MPI_SEEK_SET);

	zerol((char *)&status,sizeof(status));
	
	MPI_File_read(mpifile, header, 8L, MPI_CHARACTER, &status);
	
	header[8]=0;
	
	MPI_File_seek(mpifile, offset + 8, MPI_SEEK_SET);
	
	zerol((char *)&status,sizeof(status));
	
	MPI_File_read(mpifile, &two, sizeof(double), MPI_CHARACTER, &status);
	
	sage->SageFlip= !(two == twoLocal);


	if(flags & B_LEVEL){
	    itemLocation=SageItemNumber(sage,"cell_level",0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->level, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	
	if(flags & B_GRADIENTS_2D){
		char *name2d[]={"cell_index(1)","cell_index(2)","cell_index(3)","cell_index(4)"};
		for(n=0;n<4;++n){
			itemLocation=SageItemNumber(sage,name2d[n],0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
			MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
			MPI_File_read(mpifile,pio->next[n], BlockSize*sizeof(double), MPI_CHARACTER, &status);
		}
	}
	
	if(flags & B_GRADIENTS_3D){
		char *name3d[]={"cell_index(1)","cell_index(2)","cell_index(3)","cell_index(4)","cell_index(5)","cell_index(6)"};
		for(n=0;n<6;++n){
			itemLocation=SageItemNumber(sage,name3d[n],0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
			MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
			MPI_File_read(mpifile,pio->next[n], BlockSize*sizeof(double), MPI_CHARACTER, &status);
		}
	}
	
	
	

	if(flags & B_X){
	    itemLocation=SageItemNumber(sage,"cell_center(1)",0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->xcenter, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_Y){
	    itemLocation=SageItemNumber(sage,"cell_center(2)",0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->ycenter, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_Z){
	    itemLocation=SageItemNumber(sage,"cell_center(3)",0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->zcenter, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_DAUGHTER){
	    itemLocation=SageItemNumber(sage,"cell_daughter",0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->daughter, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_ACTIVE){
	    itemLocation=SageItemNumber(sage,"cell_active",0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->active, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_VALUE){
	    itemLocation=SageItemNumber(sage,pio->pioName,pio->pioIndex)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->value, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_XVEL){
	    itemLocation=SageItemNumber(sage,pio->vector.vectorxName,0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->vectorx, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_YVEL){
	    itemLocation=SageItemNumber(sage,pio->vector.vectoryName,0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->vectory, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
	if(flags & B_ZVEL){
	    itemLocation=SageItemNumber(sage,pio->vector.vectorzName,0L)*block->xBlockSize[nb]*sizeof(double)+7*sizeof(double); 
		MPI_File_seek(mpifile, offset + itemLocation, MPI_SEEK_SET);
	    MPI_File_read(mpifile,pio->vectorz, BlockSize*sizeof(double), MPI_CHARACTER, &status);
	}
	
		
	ret = 0;
ErrorOut:
	if(ret != 0){
		fprintf(stderr,"BlksReadMPI rank %ld Error Read\n",(long)xg.rank);	
	}
	return ret;
}
static void addem ( double *, double *, int *, MPI_Datatype * );

static void addem(double *invec, double *inoutvec, int *len, MPI_Datatype *dtype)
{
	int i;
	
	for ( i=0; i<*len; i++ ){
	    if(invec[i] != FLOAT_NOT_SET)inoutvec[i] = invec[i];
	}
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

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
	MPI_File mpifile;
	MPI_Op op;
	struct HeaderBlock *block;
    struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
	
    double xmin,xmax,ymin,ymax,dmin,dmax;
    double didx,djdy;
    double dming,dmaxg;
    double dxdi,dydj;
    long ixmin,ixmax,iymin,iymax;
	long BlockSize;
	unsigned long flags;
	int ret,length;
	double *sout2;
    long n,nn;
		
	if(!Files || !sout ||  (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	
	MPI_Op_create( (MPI_User_function *)addem, 1, &op );
	
	
	ret=1;
	
	flags = B_3D_STANDARD;
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
	}
	
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
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

	
	/* fprintf(stderr,"getPlotData rank %ld time %f mpiPath %s\n",(long)xg.rank,sage->time,Files->mpiPath); */
	
	
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);

	dming  =  1e60;
	dmaxg  = -1e60;
	
	dmin  =1e60;
	dmax= -1e60;
				
	for(nn=0;nn<block->nBlocks;nn += xg.size){
        double xc,yc,zc,dx2,dy2;
        double ymn,ymx,xmn,xmx;
        double value,xs,ys,zs,vout;
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
			}
		}
	}
	
	MPI_Reduce ( &dmin, &dming, 1, MPI_DOUBLE, MPI_MIN, xg.root, MPI_COMM_WORLD );
	MPI_Reduce ( &dmax, &dmaxg, 1, MPI_DOUBLE, MPI_MAX, xg.root, MPI_COMM_WORLD );
	SendToAll(&dming,sizeof(dming));
	SendToAll(&dmaxg,sizeof(dmaxg));
	
	MPI_Reduce ( sout, sout2, length, MPI_DOUBLE, op, xg.root, MPI_COMM_WORLD );
	
	/*
	fprintf(stderr,"getPlotData rank %ld dming %g dmaxg %g length %ld Plane_zloc %g\n",(long)xg.rank,dming,dmaxg,(long)length,
	                pio->plane.Plane_zloc);
    */
   
    pio->cmin=pio->pd.dmin=dming;
    pio->cmax=pio->pd.dmax=dmaxg;
	pioSetLimits(Files,CurrentFrame);    
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(xg.rank == xg.root){
	    if(sout2){
			for(n=0;n<length;++n)sout[n]=sout2[n];
			cFree((char *)sout2);
		}
	}

	
	ret = 0;
ErrorOut:	
	MPI_Op_free( &op );
	MPI_File_close(&mpifile);
	BlksEnd(Files,flags);
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
	MPI_File mpifile;
	long BlockSize,numcell;
	struct FilePIOInfo *pio;
   	static double  small2=1e-10;
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	unsigned long flags;
	struct dRange r;
	double *d;
	long length;
	long nn,n;
	int ret;
	
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

	r.xmin=pio->range.xminData;
	r.ymin=pio->range.yminData;
	r.xmax=pio->range.xmaxData;
	r.ymax=pio->range.ymaxData;

	r.dxlim=small2*(r.xmax-r.xmin);
	if(r.dxlim <= 0.0)r.dxlim=small2;
	r.dylim=small2*(r.ymax-r.ymin);
	if(r.dylim <= 0.0)r.dylim=small2;

	length=pio->range.ixmax*pio->range.iymax;
	d=(double *)buff;
	setFloat(d,length);

	if(pio->pd.AreaFill){	    	    
		if(getPlotData(Files,d,CurrentFrame)){
			sprintf(WarningBuff,"doSage2DGetData - getPlotData Error\n");
			WarningBatch(WarningBuff);
			return 1;
		}	
	    FloatToImage(d,length,&pio->pd,NULL);
	}else{
		zerol((char *)d,length*sizeof(double));
	}

    pio->pd.dmin=pio->cmin;
    pio->pd.dmax=pio->cmax;
	
	
	if(!pio->streamline3d.streamOn && !pio->pa.DrawZones && !pio->vector.DrawVectors){
		return 0;
	}

    flags = B_3D_STANDARD;
	if(pio->vector.DrawVectors)flags |= (B_XVEL | B_YVEL | B_ZVEL);
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients){		
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
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
						if(!pio->gradx || !pio->grady || !pio->gradz){
							pio->doGradients = FALSE;
						}
					}
			
					if(pio->active){
						pioDrawVoid(Files,(unsigned char *)buff);
					}
						
					if(pio->pa.DrawZones){
						for(n=0;n<BlockSize;++n){
						   DrawCellLines(n,pio,ss,&r);
						}
					}
		
					if(pio->vector.DrawVectors){
						pioDrawVectors(Files,ss,&r);
					}
				}
			}
		}
		
		MPI_File_close(&mpifile);
	}


	pio->numcell=numcell;

	
	if(pio->streamline3d.streamOn){
		pioDrawStream3d(Files,ss,&r);
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
	
	ret = 0;
ErrorOut:

	BlksEnd(Files,flags);
	
	if(sout2)cFree((char *)sout2);
	if(sout3)cFree((char *)sout3);
    
	return ret;
}

static int pioDrawStream3d(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
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
	double v;
	int ix,iy;
	long n,nn,kk,nnn,ne,k,n2,k2,ke;
	int streamGradients;
	long streamPasses;
    long streamSteps;
    long count;
	long nstep;
	int nGrad;
	int ipass;
	int idone;
	int ret;
	long BlockSize,BlockSizeSave,numcell,numcellSave;
	unsigned long flags;
	long CurrentFrame;

	if(!Files || !ss || !r)return 1;
	pio=&Files->pioData;
	
	
	
	if(pio->plane.Plane_type == X_PLANE){
	   count=pio->streamline3d.xPlane.count;
	}else if(pio->plane.Plane_type == Y_PLANE){
	   count=pio->streamline3d.yPlane.count;
	}else if(pio->plane.Plane_type == Z_PLANE){
	   count=pio->streamline3d.zPlane.count;
	}else{
		 count=0;
	}
	if(count < 1)return 0;
	
	ret = 1;
	
	flags=0;	
	BlockSizeSave=pio->BlockSize;
    numcellSave=pio->numcell;	
	
	stream=(struct streamStruct *)cMalloc(count*sizeof(struct streamStruct),3871);
	if(!stream){
	    WarningBatch("pioDrawStream3d Blocks Out of List Memory\n");
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
    
    streamSteps=pio->streamline3d.streamSteps;
    if(streamSteps <= 0)streamSteps=2000;
    streamGradients=pio->streamline3d.streamGradients;
    streamPasses=pio->streamline3d.streamPasses;
    
    for(n=0;n<count;++n){
   		if(pio->plane.Plane_type == X_PLANE){
	   		stream[n].xFore=pio->streamline3d.xPlane.x[n];
	   		stream[n].xBack=stream[n].xFore;
	   		stream[n].yFore=pio->streamline3d.xPlane.y[n];
	   		stream[n].yBack=stream[n].yFore;
   		}else if(pio->plane.Plane_type == Y_PLANE){
	   		stream[n].xFore=pio->streamline3d.yPlane.x[n];
	   		stream[n].xBack=stream[n].xFore;
	   		stream[n].yFore=pio->streamline3d.yPlane.y[n];
	   		stream[n].yBack=stream[n].yFore;
   		}else{
	   		stream[n].xFore=pio->streamline3d.zPlane.x[n];
	   		stream[n].xBack=stream[n].xFore;
	   		stream[n].yFore=pio->streamline3d.zPlane.y[n];
	   		stream[n].yBack=stream[n].yFore;
   		}
   		stream[n].doneFore=0;
   		stream[n].doneBack=0;
   		stream[n].stepsFore=0;
   		stream[n].stepsBack=0;
   		stream[n].foundBack=FALSE;
   		stream[n].foundFore=FALSE;
   		
    }
    
	flags=B_3D_STANDARD | B_XVEL | B_YVEL  | B_ZVEL;
	pio->doGradients = FALSE;
    if(streamGradients){
		pio->doGradients = TRUE;
		flags |= B_GRADIENTS_3D;
    }
	    
	BlockSize=pio->BlockSize;
	CurrentFrame=pio->CurrentFrame;
			
	if(doSage3DCheckBlks(Files,CurrentFrame))goto ErrorOut;
	
	sage=&pio->sageBlk;
	if(!sage)goto ErrorOut;
	block=&sage->block;
	
    setPioScales(pio);

	if(BlksStart(Files,flags))goto ErrorOut;
	
	MPI_File_open(MPI_COMM_WORLD,Files->mpiPath,MPI_MODE_RDONLY,MPI_INFO_NULL,&mpifile);
		    	  
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
				
				nGrad=-1;
				gradxV1=0;
				gradyV1=0;
				gradxV2=0;
				gradyV2=0;
		  
				if(pio->plane.Plane_type == X_PLANE && pio->streamline3d.xPlane.count > 0){
					if(streamGradients){
						lo_x=pio->next[2];
						hi_x=pio->next[3];
						lo_y=pio->next[4];
						hi_y=pio->next[5];
					}
					for(n=0;n<count;++n){
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
									
			doAgain1:
							
							if(!pioGetValueN(pio,pio->plane.Plane_xloc,x,y,&v,&ne))continue;
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
								if(k == 0){
								   stream[n].doneFore = TRUE;
								}else{
								   stream[n].doneBack = TRUE;
								}
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
							goto doAgain1;
						}
						
						
					}
				
				}else if((pio->plane.Plane_type == Y_PLANE) && (pio->streamline3d.yPlane.count > 0)){
					if(streamGradients){
						lo_x=pio->next[0];
						hi_x=pio->next[1];
						lo_y=pio->next[4];
						hi_y=pio->next[5];
					}
					for(n=0;n<count;++n){
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
			doAgain2:
							
							if(!pioGetValueN(pio,x,pio->plane.Plane_yloc,y,&v,&ne))continue;
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
								if(k == 0){
								   stream[n].doneFore = TRUE;
								}else{
								   stream[n].doneBack = TRUE;
								}
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
							goto doAgain2;
						}
						
						
					}
				
				}else if((pio->plane.Plane_type == Z_PLANE) && (pio->streamline3d.zPlane.count > 0)){
				
					if(streamGradients){
						lo_x=pio->next[0];
						hi_x=pio->next[1];
						lo_y=pio->next[2];
						hi_y=pio->next[3];
					}
					
					for(n=0;n<count;++n){
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
							
							if(!pioGetValueN(pio,x,y,pio->plane.Plane_zloc,&v,&ne)){
							/*
								sprintf(WarningBuff,"Point %ld   %g %g %g  %g %g %g %g %g %g Not Found\n",(long)n,x,y,pio->plane.Plane_zloc,
										pio->xmin,pio->ymin,pio->zmin,pio->xmax,pio->ymax,pio->zmax);
								WarningBatch(WarningBuff);
							*/

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
								if(k == 0){
								   stream[n].doneFore = TRUE;
								}else{
								   stream[n].doneBack = TRUE;
								}
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
								/*
									sprintf(WarningBuff,"Point %ld  Fore %g %g %g %g ne %ld\n",(long)n,x,y,sx,sy,ne+nff);
									WarningBatch(WarningBuff);
								*/
							}else{
								stream[n].xBack=x;
								stream[n].yBack=y;
								stream[n].stepsBack=nstep;
								/*
									sprintf(WarningBuff,"Point %ld  Back %g %g %g %g ne %ld\n",(long)n,x,y,sx,sy,ne+nff);
									WarningBatch(WarningBuff);
								*/
							}
							goto doAgain3;
						}
						
						
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
	    fprintf(stderr,"pioDrawStream3d Error Exit\n");
	}
	MPI_File_close(&mpifile);
	BlksEnd(Files,flags);
    if(stream)cFree((char *)stream);
    pio->BlockSize=BlockSizeSave;
    pio->numcell=numcellSave;
	return ret;
}
static int doRangeBlock(struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double xx,dx,yy,dy,zz,dz;
	long n;
	int k;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!pio->daughter || !pio->xcenter || !pio->ycenter || !pio->zcenter)return 1;
	if(!pio->active || !pio->level)return 1;
	
	xmin=1e60;
	ymin=1e60;
	zmin=1e60;
	ymax=-1e60;
	xmax=-1e60;
	zmax=-1e60;
	
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
        zz=pio->zcenter[n];
        dz=pio->dz2[k];
        if(zz-dz < zmin)zmin=zz-dz;
        if(zz+dz > zmax)zmax=zz+dz;
	}
	
	pio->xmin=xmin;
	pio->ymin=ymin;
	pio->zmin=zmin;
	pio->xmax=xmax;
	pio->ymax=ymax;
	pio->zmax=zmax;
	
	return 0;
}
static int pioGetValueN(struct FilePIOInfo *pio,double x,double y,double z,double *v,long *nn)
{
	double xc,yc,zc,dx2,dy2,dz2;
	struct PIO_BLK *sage;
	long ne;

	if(!pio || !nn || !pio->value || !v)return 0;
	if(!pio->daughter)return 0;
	sage=&pio->sageBlk;
	if(!sage)return 0;
	
	
	if(x < pio->xmin || x >pio->xmax)return 0;
	
	if(y < pio->ymin || y >pio->ymax)return 0;
	
	if(z < pio->zmin || z >pio->zmax)return 0;
	
	
    for(ne=0;ne< pio->numcell;++ne){
    	int k;
    	if(pio->daughter[ne])continue;
        k=(int)(pio->level[ne]);
        
        xc=pio->xcenter[ne];
        dx2=pio->dx2[k]*1.000001;
        
        if(x < xc-dx2 || x > xc+dx2)continue;
        
        yc=pio->ycenter[ne];
        dy2=pio->dy2[k]*1.000001;
        
        if(y < yc-dy2 || y > yc+dy2)continue;
    
        zc=pio->zcenter[ne];
        dz2=pio->dz2[k]*1.000001;
        
        if(z < zc-dz2 || z > zc+dz2)continue;
    
    	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
			*v=pio->value[ne]+pio->gradx[ne]*(x-xc)+pio->grady[ne]*(y-yc)+pio->gradz[ne]*(z-zc);
		}else{
			*v=pio->value[ne];
		}
		if(nn){
		    *nn=ne;
		}
		return 1;
    
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
static int DrawCellLines(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r)
{
	double dx2,dy2;
	double xc,yc;
	double x1,y1,x2,y2;
	unsigned char nc;
	int ix,iy;
	int k;
	    
	if(!pio || !ss || !r || !pio->value)return 1;
	
    k=(int)(pio->level[n]);
	if(k > ss->ZoneLevel)return 0;
	
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
/*    
    if(pio->daughter[n]){    
	  	for(kk=0;kk<8;++kk){
	        ne=kk+(long)(pio->daughter[n]-1);
	        DrawCellLines(ne,pio,ss,r);
		}      
    }
*/    
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
static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
	double pi;
	long n;
	int ret;

	struct DrawCellStuff dc;
	
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	
	if(!sage)return 1;

	/* fprintf(stderr,"rank %ld vmax %f\n",(long)xg.rank,pio->vector.vmax); */
	
	ret = 1;
	
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

			
	if(pio->vector.vspace == 0){
	    for(n=0;n<pio->numcell;++n){
	        if(pio->daughter[n])continue;
        	DrawCellVelocity(n,pio,&dc);
        }
	}else{
	    for(n=0;n<pio->numcell;++n){
	        if(pio->daughter[n])continue;
        	DrawCellVelocity2(n,pio,&dc);
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
	double vmax,vmaxg,vlength,vx,vy,vz;
	double xc,yc,dx2,dy2;
	long BlockSize;
	unsigned long flags;
	long n,nn;
	int ret;
	int k,kk;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	block=&sage->block;
	
	
	ret = 1;
	    
    flags = (B_X | B_Y | B_Z | B_DAUGHTER  | B_LEVEL | B_XVEL | B_YVEL | B_ZVEL);
    
	
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
					vz=pio->vectorz[n];
					vlength = sqrt(vx*vx+vy*vy+vz*vz);
					if(vlength > vmax)vmax=vlength;
				}
				
			}
		}
	}
	
	MPI_Reduce ( &vmax, &vmaxg, 1, MPI_DOUBLE, MPI_MIN, xg.root, MPI_COMM_WORLD );
	
	SendToAll(&vmaxg,sizeof(vmaxg));
	
	/* fprintf(stderr,"rank %ld vmaxg %f\n",(long)xg.rank,vmaxg); */
	
	pio->vmaxVectors=vmaxg;
	    
	MPI_File_close(&mpifile);
	
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

	zerol((char *)pio->gradz,sizeof(double)*BlockSize);

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

	
	next=pio->next[4];
	
	for(n=0;n<BlockSize;++n){        /* lo_z */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        pio->gradl[n] = (pio->value[n]-pio->value[n2])/(pio->dz2[k]+pio->dz2[k2]);
	    }else{
	        pio->gradl[n] = 0;
	    }
	}

	next=pio->next[5];
	
	for(n=0;n<BlockSize;++n){        /* hi_z */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        pio->gradh[n] = (pio->value[n2]-pio->value[n])/(pio->dz2[k]+pio->dz2[k2]);
	    }else{
	        pio->gradh[n] = 0;
	    }
	}


	for(n=0;n<BlockSize;++n){
		if(!pio->daughter[n] && (pio->gradh[n]*pio->gradl[n] >= 0.0)){
			if(fabs(pio->gradh[n]) < fabs(pio->gradl[n])){
				pio->gradz[n] = pio->gradh[n];
			}else{
				pio->gradz[n] = pio->gradl[n];
			}
		}else{
			pio->gradz[n] = 0;
		}
	}

	
	ret = 0;
	
	if(ret){
		if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=NULL;

		if(pio->grady)cFree((char *)pio->grady);
		pio->grady=NULL;
		
		if(pio->gradz)cFree((char *)pio->gradz);
		pio->gradz=NULL;
	}
		
	return ret;
}

static int doSage3DCheckBlks(struct FileInfo2 *Files,long CurrentFrame)
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
				
		pio->sageBlk = *sage;
		
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
	

	pio->xmin=sage->xmin;
	pio->xmax=sage->xmax;
	
	pio->ymin=sage->ymin;
	pio->ymax=sage->ymax;
	
	pio->CurrentFrame=CurrentFrame;

	for(k=1;k<=100;++k){
		pio->dx2[k]=.5*pio->dxset/pow(2,(double)(k-1));
		pio->dy2[k]=.5*pio->dyset/pow(2,(double)(k-1));
		pio->dz2[k]=.5*pio->dzset/pow(2,(double)(k-1));
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
		
		if(pio->streamline3d.xPlane.count){
			if(pio->streamline3d.xPlane.x)cFree((char *)pio->streamline3d.xPlane.x);
			if(pio->streamline3d.xPlane.y)cFree((char *)pio->streamline3d.xPlane.y);
			/*
			if(pio->streamline3d.xPlane.z)cFree((char *)pio->streamline3d.xPlane.z);
			if(pio->streamline3d.xPlane.v)cFree((char *)pio->streamline3d.xPlane.v);
			*/
			pio->streamline3d.xPlane.x=(double *)cMalloc(pio->streamline3d.xPlane.count*sizeof(double),9482);
			pio->streamline3d.xPlane.y=(double *)cMalloc(pio->streamline3d.xPlane.count*sizeof(double),9482);
			/*
			pio->streamline3d.xPlane.z=(double *)cMalloc(pio->streamline3d.xPlane.count*sizeof(double),9482);
			pio->streamline3d.xPlane.v=(double *)cMalloc(pio->streamline3d.xPlane.count*sizeof(double),9482);
			*/
		}
		if(pio->streamline3d.yPlane.count){
			if(pio->streamline3d.yPlane.x)cFree((char *)pio->streamline3d.yPlane.x);
			if(pio->streamline3d.yPlane.y)cFree((char *)pio->streamline3d.yPlane.y);
			/*
			if(pio->streamline3d.yPlane.z)cFree((char *)pio->streamline3d.yPlane.z);
			if(pio->streamline3d.yPlane.v)cFree((char *)pio->streamline3d.yPlane.v);
			*/
			pio->streamline3d.yPlane.x=(double *)cMalloc(pio->streamline3d.yPlane.count*sizeof(double),9482);
			pio->streamline3d.yPlane.y=(double *)cMalloc(pio->streamline3d.yPlane.count*sizeof(double),9482);
			/*
			pio->streamline3d.yPlane.z=(double *)cMalloc(pio->streamline3d.yPlane.count*sizeof(double),9482);;
			pio->streamline3d.yPlane.v=(double *)cMalloc(pio->streamline3d.yPlane.count*sizeof(double),9482);;
			*/
		}
		if(pio->streamline3d.zPlane.count){
			if(pio->streamline3d.zPlane.x)cFree((char *)pio->streamline3d.zPlane.x);
			if(pio->streamline3d.zPlane.y)cFree((char *)pio->streamline3d.zPlane.y);
			/*
			if(pio->streamline3d.zPlane.z)cFree((char *)pio->streamline3d.zPlane.z);
			if(pio->streamline3d.zPlane.v)cFree((char *)pio->streamline3d.zPlane.v);
			*/
			pio->streamline3d.zPlane.x=(double *)cMalloc(pio->streamline3d.zPlane.count*sizeof(double),9482);
			pio->streamline3d.zPlane.y=(double *)cMalloc(pio->streamline3d.zPlane.count*sizeof(double),9482);
			/*
			pio->streamline3d.zPlane.z=(double *)cMalloc(pio->streamline3d.zPlane.count*sizeof(double),9482);
			pio->streamline3d.zPlane.v=(double *)cMalloc(pio->streamline3d.zPlane.count*sizeof(double),9482);
			*/
		}
		
		
		
	}

	SendToAll(sage->names, block->itemsNum*sizeof(struct ItemNames));
	SendToAll(block->nBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockAdress, block->nBlocks*sizeof(double));
	
	/*
	fprintf(stderr,"1 rank %ld time %f xcount %ld ycount %ld zcount %ld %p %p %p %p\n",(long)xg.rank,sage->time,
	(long)pio->streamline3d.xPlane.count,(long)pio->streamline3d.yPlane.count,(long)pio->streamline3d.zPlane.count,
	pio->streamline3d.zPlane.y,pio->streamline3d.zPlane.y,pio->streamline3d.zPlane.z,pio->streamline3d.zPlane.v
	);
	*/
	
	if(pio->streamline3d.xPlane.count){
	    SendToAll(pio->streamline3d.xPlane.x, pio->streamline3d.xPlane.count*sizeof(double));
	    SendToAll(pio->streamline3d.xPlane.y, pio->streamline3d.xPlane.count*sizeof(double));
		/*
	    SendToAll(pio->streamline3d.xPlane.z, pio->streamline3d.xPlane.count*sizeof(double));
	    SendToAll(pio->streamline3d.xPlane.v, pio->streamline3d.xPlane.count*sizeof(double));
		*/
	}
	if(pio->streamline3d.yPlane.count){
	    SendToAll(pio->streamline3d.yPlane.x, pio->streamline3d.yPlane.count*sizeof(double));
	    SendToAll(pio->streamline3d.yPlane.y, pio->streamline3d.yPlane.count*sizeof(double));
		/*
	    SendToAll(pio->streamline3d.yPlane.z, pio->streamline3d.yPlane.count*sizeof(double));
	    SendToAll(pio->streamline3d.yPlane.v, pio->streamline3d.yPlane.count*sizeof(double));
		*/
	}
	if(pio->streamline3d.zPlane.count){
	    SendToAll(pio->streamline3d.zPlane.x, pio->streamline3d.zPlane.count*sizeof(double));
	    SendToAll(pio->streamline3d.zPlane.y, pio->streamline3d.zPlane.count*sizeof(double));
		/*
	    SendToAll(pio->streamline3d.zPlane.z, pio->streamline3d.zPlane.count*sizeof(double));
	    SendToAll(pio->streamline3d.zPlane.v, pio->streamline3d.zPlane.count*sizeof(double));
		*/
	}
	

	/* fprintf(stderr,"1 rank %ld time %f\n",(long)xg.rank,sage->time); */

    ret=0;
	
ErrorOut:
	return ret;
}
static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double z,double *v)
{
	double xc,yc,zc;
	struct PIO_BLK *sage;
	long ne;

	double xmin,ymin,zmin;

	if(!pio || !v || !pio->value)return 0;
	if(!pio->daughter)return 0;
	sage=&pio->sageBlk;
	if(!sage)return 0;
	
	
	xmin=pio->xmin;
	if(x < xmin || x >pio->xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin || y >pio->ymax)return 0;
	
	zmin=pio->zmin;
	if(z < zmin || z >pio->zmax)return 0;
	
	
    for(ne=0;ne<pio->numcell;++ne){
    	double dx,dy,dz;
    	int k;
    	if(pio->daughter[ne])continue;
        k=(int)(pio->level[ne]);
        
        xc=pio->xcenter[ne];
        dx=pio->dx2[k]*1.000001;
        
        if(x < xc-dx || x > xc+dx)continue;
        
        yc=pio->ycenter[ne];
        dy=pio->dy2[k]*1.000001;
        
        if(y < yc-dy || y > yc+dy)continue;
    
        zc=pio->zcenter[ne];
        dz=pio->dz2[k]*1.000001;
        
        if(z < zc-dz || z > zc+dz)continue;
    
		pioGetValue_ne=ne;
    	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
        	xc=pio->xcenter[ne];
        	yc=pio->ycenter[ne];
        	zc=pio->zcenter[ne];
			*v=pio->value[ne]+pio->gradx[ne]*(x-xc)
							 +pio->grady[ne]*(y-yc)
							 +pio->gradz[ne]*(z-zc);
		}else{
			*v=pio->value[ne];
		}
		return 1;
    
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
static int intcmp2(const void *xx,const  void *yy)
{
	double sum;
	register double *x=(double *)xx;
	register double *y=(double *)yy;
	sum = *x - *y;
	if(sum == 0.0)return 0;
	return (int)((sum > 0.0) ? 1 : -1);
}
static int intcmp1(const void *xx,const  void *yy)
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
/*
static int intcmp3(const void *xx,const  void *yy)
{
	double sum;
	register struct sort1 *x=(struct sort1 *)xx;
	register struct sort1 *y=(struct sort1 *)yy;
	sum = x->x - y->x;
	if(sum == 0.0){
		sum = x->y - y->y;
	    if(sum == 0.0)return 0;
	}
	return (int)((sum > 0.0) ? 1 : -1);
}
*/
