#define EXTERN22 extern

#include "Blocks.h"

int BlocksStart(struct FileInfo2 *Files,unsigned long flags)
{
	struct FilePIOInfo *pio;
	long BlockSize;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	ret = 1;
	
	

	BlockSize=pio->BlockSize;

	pio->vectorLength=BlockSize;
	
	if(flags & B_X){
    	if(pio->xcenter)cFree((char *)pio->xcenter);
		pio->xcenter=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->xcenter)goto ErrorOut;
	}
	
	if(flags & B_Y){
    	if(pio->ycenter)cFree((char *)pio->ycenter);
		pio->ycenter=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->ycenter)goto ErrorOut;
	}
	
	if(flags & B_Z){
    	if(pio->zcenter)cFree((char *)pio->zcenter);
		pio->zcenter=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->zcenter)goto ErrorOut;
	}
	
	if(flags & B_DAUGHTER){
    	if(pio->daughter)cFree((char *)pio->daughter);
		pio->daughter=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->daughter)goto ErrorOut;
	}
	
	if(flags & B_ACTIVE){
    	if(pio->active)cFree((char *)pio->active);
		pio->active=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->active)goto ErrorOut;
	}
	
	if(flags & B_LEVEL){
    	if(pio->level)cFree((char *)pio->level);
		pio->level=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->level)goto ErrorOut;
	}
	
	if(flags & B_VALUE){
    	if(pio->value)cFree((char *)pio->value);
		pio->value=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->value)goto ErrorOut;
	}
	
	if(flags & B_XVEL){
    	if(pio->vectorx)cFree((char *)pio->vectorx);
		pio->vectorx=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->vectorx)goto ErrorOut;
	}
	
	if(flags & B_YVEL){
    	if(pio->vectory)cFree((char *)pio->vectory);
		pio->vectory=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->vectory)goto ErrorOut;
	}
	
	if(flags & B_ZVEL){
    	if(pio->vectorz)cFree((char *)pio->vectorz);
		pio->vectorz=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->vectorz)goto ErrorOut;
	}
	
	if(flags & B_GRADIENTS_2D){
    	if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradx)goto ErrorOut;
		
    	if(pio->grady)cFree((char *)pio->grady);
		pio->grady=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->grady)goto ErrorOut;
		
    	if(pio->lo)cFree((char *)pio->lo);
		pio->lo=(double *)cMalloc(BlockSize*2*sizeof(double),9182);
		if(!pio->lo)goto ErrorOut;
		
    	if(pio->hi)cFree((char *)pio->hi);
		pio->hi=(double *)cMalloc(BlockSize*2*sizeof(double),9182);
		if(!pio->hi)goto ErrorOut;
		
		for(n=0;n<4;++n){
		    if(pio->next[n])cFree((char *)pio->next[n]);
		    pio->next[n]=(double *)cMalloc(BlockSize*sizeof(double),9182);
		    if(!pio->next[n])goto ErrorOut;
		}
		
    	if(pio->value)cFree((char *)pio->value);
		pio->value=(double *)cMalloc(BlockSize*5*sizeof(double),9182);
		if(!pio->value)goto ErrorOut;

    	if(pio->level)cFree((char *)pio->level);
		pio->level=(double *)cMalloc(BlockSize*5*sizeof(double),9182);
		if(!pio->level)goto ErrorOut;
		
    	if(pio->gradl)cFree((char *)pio->gradl);
		pio->gradl=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradl)goto ErrorOut;
		
    	if(pio->gradh)cFree((char *)pio->gradh);
		pio->gradh=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradh)goto ErrorOut;
		
	}

	if(flags & B_GRADIENTS_3D){
    	if(pio->gradx)cFree((char *)pio->gradx);
		pio->gradx=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradx)goto ErrorOut;
		
    	if(pio->grady)cFree((char *)pio->grady);
		pio->grady=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->grady)goto ErrorOut;
		
    	if(pio->gradz)cFree((char *)pio->gradz);
		pio->gradz=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradz)goto ErrorOut;
		
    	if(pio->lo)cFree((char *)pio->lo);
		pio->lo=(double *)cMalloc(BlockSize*2*sizeof(double),9182);
		if(!pio->lo)goto ErrorOut;
		
    	if(pio->hi)cFree((char *)pio->hi);
		pio->hi=(double *)cMalloc(BlockSize*2*sizeof(double),9182);
		if(!pio->hi)goto ErrorOut;
		
		for(n=0;n<6;++n){
		    if(pio->next[n])cFree((char *)pio->next[n]);
		    pio->next[n]=(double *)cMalloc(BlockSize*sizeof(double),9182);
		    if(!pio->next[n])goto ErrorOut;
		}
		
    	if(pio->value)cFree((char *)pio->value);
		pio->value=(double *)cMalloc(BlockSize*5*sizeof(double),9182);
		if(!pio->value)goto ErrorOut;

    	if(pio->level)cFree((char *)pio->level);
		pio->level=(double *)cMalloc(BlockSize*5*sizeof(double),9182);
		if(!pio->level)goto ErrorOut;
		
    	if(pio->gradl)cFree((char *)pio->gradl);
		pio->gradl=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradl)goto ErrorOut;
		
    	if(pio->gradh)cFree((char *)pio->gradh);
		pio->gradh=(double *)cMalloc(BlockSize*sizeof(double),9182);
		if(!pio->gradh)goto ErrorOut;
		
	}


	if(flags & B_Z){
		;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}

int BlocksEnd(struct FileInfo2 *Files,unsigned long flags)
{
	struct FilePIOInfo *pio;
	long BlockSize;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	ret = 1;
	

	BlockSize=pio->BlockSize;
	
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
	
	if(flags & B_Z){
		;
	}
	
	ret = 0;

	return ret;
}

int BlocksRead(struct FileInfo2 *Files,unsigned long flags,long start,long BlockSize,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	int ret;
	int n;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	ret = 1;
	

	if(flags & B_X){
		if(SageGetFileItemBlock(Files,"cell_center",1,
	                  pio->xcenter,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_Y){
		if(SageGetFileItemBlock(Files,"cell_center",2,
	                  pio->ycenter,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_Z){
		if(SageGetFileItemBlock(Files,"cell_center",3,
	                  pio->zcenter,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_DAUGHTER){
		if(SageGetFileItemBlock(Files,"cell_daughter",0,
	                  pio->daughter,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_ACTIVE){
		if(SageGetFileItemBlock(Files,"cell_active",0,
	                  pio->active,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_LEVEL){
		if(SageGetFileItemBlock(Files,"cell_level",0,
	                  pio->level,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_VALUE){
		if(SageGetFileItemBlock(Files,pio->pioName,pio->pioIndex,
	                  pio->value,start,BlockSize,CurrentFrame)){
			ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_XVEL){
		if(SageGetFileItemBlock(Files,pio->vector.vectorxName,0,
		                  		pio->vectorx,start,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_YVEL){
		if(SageGetFileItemBlock(Files,pio->vector.vectoryName,0,
		                  		pio->vectory,start,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_ZVEL){
		if(SageGetFileItemBlock(Files,pio->vector.vectorzName,0,
		                  		pio->vectorz,start,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
		}
	}
	
	if(flags & B_GRADIENTS_2D){
		for(n=0;n<4;++n){
			if(SageGetFileItemBlock(Files,"cell_index",n+1,
		                  pio->next[n],start,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
			}
		}
	}
	
	if(flags & B_GRADIENTS_3D){
		for(n=0;n<6;++n){
			if(SageGetFileItemBlock(Files,"cell_index",n+1,
		                  pio->next[n],start,BlockSize,CurrentFrame)){
				ret=2;goto ErrorOut;
			}
		}
	}
	
	if(flags & B_Z){
		;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}
