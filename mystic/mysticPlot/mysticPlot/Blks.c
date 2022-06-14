/*
 *  Blks.c
 *  mShow
 *
 *  Created by Dale Ranta on 8/2/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */

#define EXTERN22 extern

#include "Blocks.h"


int SageGetFileItemBlk(struct FileInfo2 *Files,char *dataName,long plotItem,
					   double *data,long nb,long dataLength,long CurrentFrame);

int BlksStart(struct FileInfo2 *Files,unsigned long flags)
{
	struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
	long BlockSize;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	ret = 1;

	
	BlockSize=sage->xBlockSizeMax;

	pio->vectorLength=BlockSize;
	
	if(flags & B_X){
    	if(pio->xcenter)cFree((char *)pio->xcenter);
		pio->xcenter=(double *)cMalloc(BlockSize*sizeof(double),9111);
		if(!pio->xcenter)goto ErrorOut;
	}
	
	if(flags & B_Y){
    	if(pio->ycenter)cFree((char *)pio->ycenter);
		pio->ycenter=(double *)cMalloc(BlockSize*sizeof(double),9121);
		if(!pio->ycenter)goto ErrorOut;
	}
	
	if(flags & B_Z){
    	if(pio->zcenter)cFree((char *)pio->zcenter);
		pio->zcenter=(double *)cMalloc(BlockSize*sizeof(double),9131);
		if(!pio->zcenter)goto ErrorOut;
	}
	
	if(flags & B_DAUGHTER){
    	if(pio->daughter)cFree((char *)pio->daughter);
		pio->daughter=(double *)cMalloc(BlockSize*sizeof(double),9141);
		if(!pio->daughter)goto ErrorOut;
	}
	
	if(flags & B_ACTIVE){
    	if(pio->active)cFree((char *)pio->active);
		pio->active=(double *)cMalloc(BlockSize*sizeof(double),9151);
		if(!pio->active)goto ErrorOut;
	}
	
	if(flags & B_LEVEL){
    	if(pio->level)cFree((char *)pio->level);
		pio->level=(double *)cMalloc(BlockSize*sizeof(double),9161);
		if(!pio->level)goto ErrorOut;
	}
	
	if(flags & B_VALUE){
    	if(pio->value)cFree((char *)pio->value);
		pio->value=(double *)cMalloc(BlockSize*sizeof(double),9171);
		if(!pio->value)goto ErrorOut;
	}
	
	if(flags & B_XVEL){
    	if(pio->vectorx)cFree((char *)pio->vectorx);
		pio->vectorx=(double *)cMalloc(BlockSize*sizeof(double),9181);
		if(!pio->vectorx)goto ErrorOut;
	}
	
	if(flags & B_YVEL){
    	if(pio->vectory)cFree((char *)pio->vectory);
		pio->vectory=(double *)cMalloc(BlockSize*sizeof(double),9191);
		if(!pio->vectory)goto ErrorOut;
	}
	
	if(flags & B_ZVEL){
    	if(pio->vectorz)cFree((char *)pio->vectorz);
		pio->vectorz=(double *)cMalloc(BlockSize*sizeof(double),9192);
		if(!pio->vectorz)goto ErrorOut;
	}
	
	if(flags & B_GRADIENTS_2D){
    	if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=(double *)cMalloc(BlockSize*sizeof(double),9188);
		if(!pio->gradx)goto ErrorOut;
		
    	if(pio->grady)cFree((char *)pio->grady);
		pio->grady=(double *)cMalloc(BlockSize*sizeof(double),9187);
		if(!pio->grady)goto ErrorOut;
		
    	if(pio->lo)cFree((char *)pio->lo);
		pio->lo=(double *)cMalloc(BlockSize*2*sizeof(double),9186);
		if(!pio->lo)goto ErrorOut;
		
    	if(pio->hi)cFree((char *)pio->hi);
		pio->hi=(double *)cMalloc(BlockSize*2*sizeof(double),9111);
		if(!pio->hi)goto ErrorOut;
		
		for(n=0;n<4;++n){
		    if(pio->next[n])cFree((char *)pio->next[n]);
		    pio->next[n]=(double *)cMalloc(BlockSize*sizeof(double),9112);
		    if(!pio->next[n])goto ErrorOut;
		}
		
    	if(pio->value)cFree((char *)pio->value);
		pio->value=(double *)cMalloc(BlockSize*5*sizeof(double),9113);
		if(!pio->value)goto ErrorOut;

    	if(pio->level)cFree((char *)pio->level);
		pio->level=(double *)cMalloc(BlockSize*5*sizeof(double),9114);
		if(!pio->level)goto ErrorOut;
		
    	if(pio->gradl)cFree((char *)pio->gradl);
		pio->gradl=(double *)cMalloc(BlockSize*sizeof(double),9115);
		if(!pio->gradl)goto ErrorOut;
		
    	if(pio->gradh)cFree((char *)pio->gradh);
		pio->gradh=(double *)cMalloc(BlockSize*sizeof(double),9116);
		if(!pio->gradh)goto ErrorOut;
		
	}

	if(flags & B_GRADIENTS_3D){
    	if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=(double *)cMalloc(BlockSize*sizeof(double),9117);
		if(!pio->gradx)goto ErrorOut;
		
    	if(pio->grady)cFree((char *)pio->grady);
		pio->grady=(double *)cMalloc(BlockSize*sizeof(double),9118);
		if(!pio->grady)goto ErrorOut;
		
    	if(pio->gradz)cFree((char *)pio->gradz);
		pio->gradz=(double *)cMalloc(BlockSize*sizeof(double),9119);
		if(!pio->gradz)goto ErrorOut;
		
    	if(pio->lo)cFree((char *)pio->lo);
		pio->lo=(double *)cMalloc(BlockSize*2*sizeof(double),9120);
		if(!pio->lo)goto ErrorOut;
		
    	if(pio->hi)cFree((char *)pio->hi);
		pio->hi=(double *)cMalloc(BlockSize*2*sizeof(double),9121);
		if(!pio->hi)goto ErrorOut;
		
		for(n=0;n<6;++n){
		    if(pio->next[n])cFree((char *)pio->next[n]);
		    pio->next[n]=(double *)cMalloc(BlockSize*sizeof(double),9122);
		    if(!pio->next[n])goto ErrorOut;
		}
		
    	if(pio->value)cFree((char *)pio->value);
		pio->value=(double *)cMalloc(BlockSize*5*sizeof(double),9123);
		if(!pio->value)goto ErrorOut;

    	if(pio->level)cFree((char *)pio->level);
		pio->level=(double *)cMalloc(BlockSize*5*sizeof(double),9124);
		if(!pio->level)goto ErrorOut;
		
    	if(pio->gradl)cFree((char *)pio->gradl);
		pio->gradl=(double *)cMalloc(BlockSize*sizeof(double),9125);
		if(!pio->gradl)goto ErrorOut;
		
    	if(pio->gradh)cFree((char *)pio->gradh);
		pio->gradh=(double *)cMalloc(BlockSize*sizeof(double),9126);
		if(!pio->gradh)goto ErrorOut;
		
	}
	
	
	ret = 0;
ErrorOut:
	return ret;
}

int BlksEnd(struct FileInfo2 *Files,unsigned long flags)
{
	struct FilePIOInfo *pio;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	ret = 1;
	
	
	if(flags & B_X){
    	if(pio->xcenter)cFree((char *)pio->xcenter);
    	pio->xcenter=NULL;
    	
	}
	
	if(flags & B_Y){
    	if(pio->ycenter)cFree((char *)pio->ycenter);
    	pio->ycenter=NULL;
	
	}
	
	if(flags & B_Z){
    	if(pio->zcenter)cFree((char *)pio->zcenter);
    	pio->zcenter=NULL;
	
	}
	
	if(flags & B_DAUGHTER){
    	if(pio->daughter)cFree((char *)pio->daughter);
    	pio->daughter=NULL;
    	
	}
	
	if(flags & B_ACTIVE){
    	if(pio->active)cFree((char *)pio->active);
    	pio->active=NULL;
    	
	}
	
	if(flags & B_LEVEL){
    	if(pio->level)cFree((char *)pio->level);
   		pio->level=NULL;
   		
	}
	
	if(flags & B_VALUE){
    	if(pio->value)cFree((char *)pio->value);
    	pio->value=NULL;
	
	}
	
	if(flags & B_XVEL){
    	if(pio->vectorx)cFree((char *)pio->vectorx);
    	pio->vectorx=NULL;
    
	}
	
	if(flags & B_YVEL){
    	if(pio->vectory)cFree((char *)pio->vectory);
    	pio->vectory=NULL;
	}
	
	if(flags & B_ZVEL){
    	if(pio->vectorz)cFree((char *)pio->vectorz);
    	pio->vectorz=NULL;
	}
	
	if(flags & B_GRADIENTS_2D){
    	if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=NULL;
		
    	if(pio->grady)cFree((char *)pio->grady);
		pio->grady=NULL;
		
    	if(pio->lo)cFree((char *)pio->lo);
		pio->lo=NULL;
		
    	if(pio->hi)cFree((char *)pio->hi);
		pio->hi=NULL;
		
		for(n=0;n<4;++n){
		    if(pio->next[n])cFree((char *)pio->next[n]);
		    pio->next[n]=NULL;
		}
		
    	if(pio->value)cFree((char *)pio->value);
		pio->value=NULL;
		
    	if(pio->gradl)cFree((char *)pio->gradl);
		pio->gradl=NULL;
		
    	if(pio->gradh)cFree((char *)pio->gradh);
		pio->gradh=NULL;
	}
	
	if(flags & B_GRADIENTS_3D){
    	if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=NULL;
		
    	if(pio->grady)cFree((char *)pio->grady);
		pio->grady=NULL;
		
    	if(pio->gradz)cFree((char *)pio->gradz);
		pio->gradz=NULL;
		
    	if(pio->lo)cFree((char *)pio->lo);
		pio->lo=NULL;
		
    	if(pio->hi)cFree((char *)pio->hi);
		pio->hi=NULL;
		
		for(n=0;n<6;++n){
		    if(pio->next[n])cFree((char *)pio->next[n]);
		    pio->next[n]=NULL;
		}
		
    	if(pio->value)cFree((char *)pio->value);
		pio->value=NULL;
		
    	if(pio->gradl)cFree((char *)pio->gradl);
		pio->gradl=NULL;
		
    	if(pio->gradh)cFree((char *)pio->gradh);
		pio->gradh=NULL;
	}
		
	ret = 0;

	return ret;
}

int BlksRead(struct FileInfo2 *Files,unsigned long flags,long nb,long BlockSize,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	struct PIO_BLK *sage;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	sage=&pio->sageBlk;
	if(!sage)return 1;
	ret = 1;
	
	
	if(BlockSize > sage->xBlockSizeMax){
		ret=322;goto ErrorOut;
	}
	

	if(flags & B_LEVEL){
		if(SageGetFileItemBlk(Files,"cell_level",0,
	                  pio->level,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}


	if(flags & B_GRADIENTS_2D){
		char *name2d[]={"cell_index(1)","cell_index(2)","cell_index(3)","cell_index(4)"};
		for(n=0;n<4;++n){
			if(SageGetFileItemBlk(Files,name2d[n],0,
		                  pio->next[n],nb,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
			}
		}
	}
	
	if(flags & B_GRADIENTS_3D){
		char *name3d[]={"cell_index(1)","cell_index(2)","cell_index(3)","cell_index(4)","cell_index(5)","cell_index(6)"};
		for(n=0;n<6;++n){
			if(SageGetFileItemBlk(Files,name3d[n],0,
		                  pio->next[n],nb,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
			}
		}
	}



	if(flags & B_X){
		if(SageGetFileItemBlk(Files,"cell_center(1)",0,
	                  pio->xcenter,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_Y){
		if(SageGetFileItemBlk(Files,"cell_center(2)",0,
	                  pio->ycenter,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_Z){
		if(SageGetFileItemBlk(Files,"cell_center(3)",0,
	                  pio->zcenter,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_DAUGHTER){
		if(SageGetFileItemBlk(Files,"cell_daughter",0,
	                  pio->daughter,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_ACTIVE){
		if(SageGetFileItemBlk(Files,"cell_active",0,
	                  pio->active,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	
	if(flags & B_VALUE){
		if(SageGetFileItemBlk(Files,pio->pioName,pio->pioIndex,
	                  pio->value,nb,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_XVEL){
		if(SageGetFileItemBlk(Files,pio->vector.vectorxName,0,
		                  		pio->vectorx,nb,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_YVEL){
		if(SageGetFileItemBlk(Files,pio->vector.vectoryName,0,
		                  		pio->vectory,nb,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_ZVEL){
		if(SageGetFileItemBlk(Files,pio->vector.vectorzName,0,
		                  		pio->vectorz,nb,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
		}
	}
	
		
	ret = 0;
ErrorOut:
	return ret;
}

