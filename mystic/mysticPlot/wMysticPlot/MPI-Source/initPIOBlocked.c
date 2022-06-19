/*
 *  initPIOBlocked.c
 *  mShow
 *
 *  Created by Dale Ranta on 7/29/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */
#define EXTERN22 extern

#include "initPIOBlocked.h"

#include "File8.h"

static int SageGetDataBlk(struct PIO_BLK *sage,char *dataName,int index,double *data,long start,long dataLength,int *dataType);

static int SageGetItemBlk(struct PIO_BLK *sage,struct FilePIOInfo *pio,char *dataName,long plotItem,double *data,long nb, long dataLength,int *dataType);

int SageBlkGetData(struct PIO_BLK *sage,char *dataName,int index,double **data,long *dataLength,int *dataType);

int SageBlkGetItem(struct PIO_BLK *sage,char *dataName,long plotItem,double **data,long *dataLength,int *dataType);

int initPIO2Blk(struct FileInfo2 *Files,int numdim);

static int CheckPIOBlkSpace(struct FileInfo2 *Files);

static int FilesSetFramePIO(long CurrentFrame,struct FileInfo2 *Files);

int FilesOpenBLK3D(struct FileInfo2 *files);

int FilesOpenPIO3D(struct FileInfo2 *files);
int FilesOpenPIO2D(struct FileInfo2 *files);

int CheckNames(struct FileInfo2 *Files);

static int SageTranslateDoubles(struct PIO_BLK *sage,double *d,long length);


int IsPIOBlk(char *namef,int *rank)
{
		struct PIO_BLK *sage;
        char name[256];
        FILE *in;
        int n;

        if(!namef || !rank)return 0;

        in=fopen(namef,"r");
        if(in == NULL){
           sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",namef);
           WarningBatch(WarningBuff);
           return 0;
        }
        for(n=0;n<8;++n){

            name[n]=fgetc(in);
        }
        name[8]=0;

        fclose(in);

        if(strcmp(name,"SHOW0000")){
            return 0;
        }

		sage=sageHeaderBLK(namef);
	    if(!(sage)){
	         return 0;
	    }

		*rank=(int)sage->numdim;

		sageCloseBlk(sage,1);

        return 1;
}

int sageCloseBlk(struct PIO_BLK *sage,int flag)
{

	if(!sage)return 1;

	if(sage->in)fclose8(sage->in);
	sage->in=NULL;
	
	if(sage->block.nBlockSize){
	    if(sage->block.BlockSize)cFree((char *)sage->block.nBlockSize);
	    sage->block.nBlockSize=NULL;
	}
	
	
	if(sage->block.xBlockSize){
	    if(sage->block.xBlockSize)cFree((char *)sage->block.xBlockSize);
	    sage->block.xBlockSize=NULL;
	}
	
	
	if(sage->block.xBlockAdress){
	    if(sage->block.xBlockAdress)cFree((char *)sage->block.xBlockAdress);
	    sage->block.xBlockAdress=NULL;
	}
	
	
	if(sage->names){
	    if(sage->names)cFree((char *)sage->names);
	    sage->names=NULL;
	}
	
	
	if(sage && flag){
	    cFree((char *)sage);
        sage=NULL;
    }

	return 0;
}
struct PIO_BLK *sageHeaderBLK(char *path)
{
	unsigned char items[1024];
	unsigned char header[9];
	struct HeaderBlock *block;
	struct PIO_BLK *sage;
	long xBlockSizeMax;
	double two;
	FILE8 *in;
	int ret;
	int k;
	
	if(!path)return (struct PIO_BLK *)NULL;
	
	ret=1;
	
	in=NULL;
	
	sage=(struct PIO_BLK *)cMalloc(sizeof(struct PIO_BLK),2999);
	
	if(!sage)goto ErrorOut;
	
	zerol((char *)sage,sizeof(*sage));
	
	if(!(in=fopen8(path,"rb"))){
		 goto ErrorOut;
	}
	

	if(getString8(header,8L,in))goto ErrorOut;
	
	header[8]=0;
	
	if(strcmp((char *)header,"SHOW0000")){
		goto ErrorOut;
	}

	if(getDouble8(&two,in))goto ErrorOut;

	if(two != 2.0){
	    in->convertRead = !in->convertRead;
	}
	
	sage->SageFlip=in->convertRead;
	
	block=&sage->block;
	
	zerol((char *)block,sizeof(*block));
	
	if(getDoubleArray8((double *)block,100L,in))goto ErrorOut;
	
	sage->names=(struct ItemNames *)cMalloc((unsigned long)(block->itemsNum*sizeof(struct ItemNames)),94823);
	if(!sage->names)goto ErrorOut;

	for(k=0;k<(int)block->itemsNum;++k){
	    if(getString8(items,(long)block->itemsSize,in))goto ErrorOut;
		strncpy(sage->names[k].name,(char *)items,sizeof(sage->names[k].name));
		/*
		sprintf(WarningBuff,"Read \"%s\" From \"%s\"\n",sage->names[k].name,path);
		WarningBatch(WarningBuff);
		*/
		
	}

	block->nBlockSize=(double *)cMalloc((unsigned long)(block->nBlocks*sizeof(double)),9482);
	block->xBlockSize=(double *)cMalloc((unsigned long)(block->nBlocks*sizeof(double)),9482);
	block->xBlockAdress=(double *)cMalloc((unsigned long)(block->nBlocks*sizeof(double)),9582);
	if(!block->xBlockSize || !block->xBlockAdress || !block->nBlockSize)goto ErrorOut;

	if(getDoubleArray8(block->nBlockSize,(long)(block->nBlocks),in))goto ErrorOut;
	
	if(getDoubleArray8(block->xBlockSize,(long)(block->nBlocks),in))goto ErrorOut;
	
	if(getDoubleArray8(block->xBlockAdress,(long)(block->nBlocks),in))goto ErrorOut;

	xBlockSizeMax=0;
	for(k=0;k<(int)block->nBlocks;++k){
		if(block->xBlockSize[k] > xBlockSizeMax)xBlockSizeMax=(long)(block->xBlockSize[k]);
		/*
		sprintf(WarningBuff,"%d nBlockSize %.0f xBlockSize %.0f xBlockAdress %.0f \n",
		        k,block->nBlockSize[k],block->xBlockSize[k],block->xBlockAdress[k]);
		WarningBatch(WarningBuff);
		*/
	}

	sage->numdim=(long)(sage->block.numdim);
	sage->xzero=sage->block.xzero;
	sage->ixmax=(long)(sage->block.ixmax);
	sage->dxset=sage->block.dxset;
	sage->yzero=sage->block.yzero;
	sage->iymax=(long)(sage->block.iymax);
	sage->dyset=sage->block.dyset;
	sage->numdim=(long)(sage->block.numdim);
	sage->zzero=sage->block.zzero;
	sage->dzset=sage->block.dzset;
	sage->izmax=(long)(sage->block.izmax);
	sage->xBlockSizeMax=xBlockSizeMax;
	sage->time=sage->block.time;
	
	ret=0;
	
ErrorOut:

	if(in)fclose8(in);
	
	if(ret){
		sageCloseBlk(sage,1);
		sage=NULL;
	}

	return sage;
}
int initPIOBlk(struct FileInfo2 *Files)
{
	struct PIO_BLK *sage;
    char name[2048];
	int numdim;
	

	if(!Files)return 1;
	if(!Files->FilesNames || !Files->FilesNames[0])return 1;

	mstrncpy(name,Files->directory,2048);
	strncatToPath(name,Files->FilesNames[0],2048);
	    
	sage=sageHeaderBLK(name);
    if(!(sage)){
         sprintf(WarningBuff,"initPIO Could Not Open Sage File \"%s\" To Read\n",Files->FilesNames[0]);
         WarningBatch(WarningBuff);
         return 1;
    }

	numdim=(int)sage->numdim;

	sageCloseBlk(sage,1);

	if(numdim == 1 || numdim == 2 || numdim == 3){
	    return initPIO2Blk(Files,numdim);
	}else{
	     sprintf(WarningBuff,"Sage File \"%s\" Has Unsupported numdim %d\n",
	                         Files->FilesNames[0],numdim);
	     WarningBatch(WarningBuff);
             return 1;
	}
}

int initPIO2Blk(struct FileInfo2 *Files,int numdim)
{
	double rmin[3],rmax[3];
	double rminp[3],rmaxp[3];
    long n,ImageCount,xBlockSizeMax;
	struct PIO_BLK *sage;
	int ret,ndim;
    char name[2048];
    int fileType;

	if(!Files)return 1;

	ret=1;
	
	if((numdim < 1 || numdim > 3)){
	   sprintf(WarningBuff,"initPIO2Blk Bad numdim %d\n",numdim);
	   WarningBatch(WarningBuff);
	   return 1;
	}

	rmin[0]=rmin[1]=rmin[2]=     1e60;
	rmax[0]=rmax[1]=rmax[2]=    -1e60;

	ImageCount=0;

	xBlockSizeMax=0;

	for(n=0;n<Files->FileCount;++n){

	    sage = NULL;

	    rminp[0]=rminp[1]=rminp[2]=     1e60;
	    rmaxp[0]=rmaxp[1]=rmaxp[2]=    -1e60;

		mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->FilesNames[n],2048);
	
	    sage=sageHeaderBLK(name);
        if(!sage){
	         sprintf(WarningBuff,"initPIO2Blk Could Not Open Sage File \"%s\" To Read\n",name);
	         WarningBatch(WarningBuff);
             break;
        }
	    if(sage->numdim != numdim){
			sprintf(WarningBuff,"Sage File \"%s\" Has UnMatched sage->numdim %ld numdim %d\n",
		                         Files->FilesNames[n],sage->numdim,numdim);
			WarningBatch(WarningBuff);
			sageCloseBlk(sage,1);
			sage=NULL;
			continue;
	    }

		if(CheckPIOBlkSpace(Files))goto ErrorOut;

		for(ndim=0;ndim<numdim;++ndim){
		    if(ndim == 0){
		        rminp[ndim]=sage->xzero;
		        rmaxp[ndim]=sage->xzero+sage->dxset*(double)sage->ixmax;
		        if(numdim == 1){
		            rmin[1]=0;
		            rmax[1]=sage->dxset;
		        }
		    }else if(ndim == 1){
		        rminp[ndim]=sage->yzero;
		        rmaxp[ndim]=sage->yzero+sage->dyset*(double)sage->iymax;
		    }else if(ndim == 2){
		        rminp[ndim]=sage->zzero;
		        rmaxp[ndim]=sage->zzero+sage->dzset*(double)sage->izmax;
		    }
		}

		for(ndim=0;ndim<numdim;++ndim){
            if(rminp[ndim] < rmin[ndim])rmin[ndim]=rminp[ndim];
            if(rmaxp[ndim] > rmax[ndim])rmax[ndim]=rmaxp[ndim];
		}


		sage->xmin=rminp[0];
		sage->ymin=rminp[1];
		sage->zmin=rminp[2];

		sage->xmax=rmaxp[0];
		sage->ymax=rmaxp[1];
		sage->zmax=rmaxp[2];

		Files->PIOBlkList[ImageCount]= *sage;

		Files->ImageNames[ImageCount]=strsave(Files->FilesNames[n],8812);
		
		Files->ImageFiles[ImageCount]=n;

		Files->ImageCount = ++ImageCount;


	    if(sage->xBlockSizeMax > xBlockSizeMax)xBlockSizeMax=sage->xBlockSizeMax;
		
		cFree((char *)sage);
		
	}
	
	for(n=0;n<Files->FileCount;++n){
		sage=&Files->PIOBlkList[n];
		sage->xBlockSizeMax=xBlockSizeMax;
	}
	
	
	Files->xminr=rmin[0];
	Files->yminr=rmin[1];
	Files->zminr=rmin[2];
	Files->xmaxr=rmax[0];
	Files->ymaxr=rmax[1];
	Files->zmaxr=rmax[2];
	Files->CurrentFrame = -1;
	
	fileType=Files->fileType;
	if(numdim == 1){
	/*
	    Files->fileClass=PIO1D_BLK_FILES;
	    Files->fileType=SAGE1D_BLOCKED;
	    Files->pdefault=FilesOpenPIO2D;
	    Files->pSetFrame=FilesSetFramePIO;
	    doSage1DStart(Files);
	*/
	}else if(numdim == 2){
	
	    Files->fileClass=PIO2D_BLK_FILES;
	    Files->fileType=SAGE2D_BLOCKED;
	    Files->pdefault=FilesOpenPIO2D;
	    Files->pSetFrame=FilesSetFramePIO;
	    doSage2DStartBlk(Files);
	
	}else if(numdim == 3){

	    Files->fileClass=PIO3D_BLK_FILES;
	    Files->fileType=SAGE3D_BLOCKED;
	    Files->pdefault=FilesOpenPIO3D;
	    Files->pSetFrame=FilesSetFramePIO;
	    doSage3DBlkStart(Files);
	}else{
	   sprintf(WarningBuff,"initPIO2Blk Bad numdim %d\n",numdim);
	   WarningBatch(WarningBuff);
	   return 1;
	}

		
	return 0;
ErrorOut:
	
	sprintf(WarningBuff,"initPIO2Blk Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	
	if(Files->ImageCount > 0)return 0;
	freeFileList(Files);
	return 1;
}
static int FilesSetFramePIO(long CurrentFrame,struct FileInfo2 *Files)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;

	if(!Files)return 1;
	
	pio=&Files->pioData;
	s = &Files->SetData;
	s->SetFrame= -1;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	s->name=Files->ImageNames[CurrentFrame];

	s->data= Files->dataSave;
	s->SetFrame= CurrentFrame;
	s->type= Files->fileClass;
	if(Files->PIOBlkList){
	    s->pioTime=Files->PIOBlkList[CurrentFrame].time;
	    Files->pioTime=s->pioTime;
	    s->numcell=Files->PIOBlkList[CurrentFrame].numcell;
	    pio->numcell=Files->PIOBlkList[CurrentFrame].numcell;
	    pio->sageBlk=Files->PIOBlkList[CurrentFrame];
	}else{
	    s->pioTime=0;
	    s->numcell=0;
	    pio->numcell=0;
	    pio->sage=NULL;
	}
	return 0;
}

static int CheckPIOBlkSpace(struct FileInfo2 *Files)
{
	struct PIO_BLK *PIOBlkList;
	long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	PIOBlkList=NULL;
	if(Files->PIOBlkList){
	    PIOBlkList=(struct PIO_BLK *)cRealloc((char *)Files->PIOBlkList,ImageMax*sizeof(struct PIO_BLK),8761);
	    if(!PIOBlkList){
	        sprintf(WarningBuff,"CheckPIOBlkSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&PIOBlkList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct PIO_BLK));
	}else{
	    PIOBlkList=(struct PIO_BLK *)cMalloc(ImageMax*sizeof(struct PIO_BLK),8452);
	    if(!PIOBlkList){
	        sprintf(WarningBuff,"CheckPIOBlkSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)PIOBlkList,ImageMax*sizeof(struct PIO_BLK));
	}

	Files->ImageMax=ImageMax;
	Files->PIOBlkList=PIOBlkList;

	return 0;
ErrorOut:
	if(PIOBlkList){
	    Files->PIOBlkList=PIOBlkList;
	}
	return 1;
}
int filePIOBlkList(struct FileInfo2 *Files,long CurrrentFrame,struct ScrollList *List,int flag)
{

	struct HeaderBlock *block;
    struct PIO_BLK *sage;
    char *names[20];
    int k,n;
	    
	if(!Files || !List)return 1;
	if(!Files->PIOBlkList)return 1;

	if(CurrrentFrame < 0 || CurrrentFrame >= Files->ImageCount)CurrrentFrame = 0;

	sage = &Files->PIOBlkList[CurrrentFrame];
	if(!sage)return 1;
	
	block=&sage->block;
	
	k=0;
	names[k++]="cell_index(1)";
	names[k++]="cell_index(2)";
	names[k++]="cell_level";
	names[k++]="cell_center(1)";
	
	if(sage->numdim >= 2){
		names[k++]="cell_index(3)";
		names[k++]="cell_index(4)";
		names[k++]="cell_center(2)";
	}
	
	if(sage->numdim >= 3){
		names[k++]="cell_index(5)";
		names[k++]="cell_index(6)";
		names[k++]="cell_center(3)";
	}
	
	names[k++]="cell_daughter";
	names[k++]="cell_active";
		
	ScrollListInsertOneType(names,3L,-1L,k,List);
	
	
	k=0;
	for(n=0;n<(int)block->itemsNum;++n){
	    if(k >= 20){
	        ScrollListInsertOneType(names,3L,-1L,k,List);
	        k=0;
	    }
	    names[k++]=sage->names[n].name;
	}
	if(k > 0)ScrollListInsertOneType(names,3L,-1L,k,List);
	
	ScrollListSort(LIST_SORT_ASCII,List);
		
	return 0;
}

int SageBlkGetFileItem(struct FileInfo2 *Files,char *dataName,long plotItem,
					   double **data,long *dataLength,long CurrentFrame)
{
	struct SetFrameData *s;
	struct PIO_BLK *sage;
	int dataType;
	int ret;
	
	if(!Files || !dataName || !data || !dataLength)return 1;
	if(!Files->PIOBlkList)return 1;
	s = &Files->SetData;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIOBlkList[CurrentFrame];
	
	if(!sage)return 1;
	
	s->pioTime=sage->time;

	if(SetFileToRead8(CurrentFrame,Files))return 1;
	
	sage->in=Files->in8;
	
	ret=SageBlkGetItem(sage,dataName,plotItem,data,dataLength,&dataType);
	
	sage->in=NULL;
	
	return ret;
}
int SageBlkGetItem(struct PIO_BLK *sage,char *dataName,long plotItem,double **data,long *dataLength,int *dataType)
{
	
	if(!sage || !dataName)return 1;

	return SageBlkGetData(sage,dataName,(int)plotItem,data,dataLength,dataType);

}
int SageItemNumber(struct PIO_BLK *sage,char *dataName,long plotItem)
{
	char *known1[]={
						"cell_level",
						"cell_index(1)",
						"cell_index(2)",
						"cell_center(1)",
						"cell_daughter",
						"cell_active",
				  };
				  
	char *known2[]={
						"cell_level",
						"cell_index(1)",
						"cell_index(2)",
						"cell_index(3)",
						"cell_index(4)",
						"cell_center(1)",
						"cell_center(2)",
						"cell_daughter",
						"cell_active",
				  };
				  
	char *known3[]={
						"cell_level",
						"cell_index(1)",
						"cell_index(2)",
						"cell_index(3)",
						"cell_index(4)",
						"cell_index(5)",
						"cell_index(6)",
						"cell_center(1)",
						"cell_center(2)",
						"cell_center(3)",
						"cell_daughter",
						"cell_active",
				  };
				  
	struct HeaderBlock *block;
	int ifound=0;
	int offset;
	int ret;
	int n;

	if(!sage || !dataName)return -1;
	
	block=&sage->block;

	ret=-1;

	offset=0;
	ifound = -1;
	if(sage->numdim == 1){
		offset=sizeof(known1)/sizeof(char *);
		return ret;
	}else if(sage->numdim == 2){
		offset=sizeof(known2)/sizeof(char *);
		for(n=0;n<offset;++n){
			if(!strcmp(known2[n],dataName)){
				ifound=n;
				break;
			}
		}
	}else if(sage->numdim == 3){
		offset=sizeof(known3)/sizeof(char *);
		for(n=0;n<offset;++n){
			if(!strcmp(known3[n],dataName)){
				ifound=n;
				break;
			}
		}
	}else{
		return ret;
	}
	
	if(ifound >= 0){
		offset=0;
		goto GotIt;
	}
	
	for(n=0;n<(int)block->itemsNum;++n){
		if(!strcmp(sage->names[n].name,dataName)){
			ifound=n;
			break;
		}
	}
		
GotIt:
	if(ifound < 0){
		return ret;
	}
	
	ret = offset+ifound;

	return ret;
}

int SageBlkGetData(struct PIO_BLK *sage,char *dataName,int index,double **data,long *dataLength,int *dataType)
{
	struct HeaderBlock *block;
	long length;
	int itemNumber;
	int ret;
	int n;

	if(!sage || !dataName || !data || !dataLength || !dataType)return 1;
	if(!sage->in)return 1;	
	block=&sage->block;

	ret=2;
	
	itemNumber=SageItemNumber(sage,dataName,index);

	if(itemNumber < 0){
		return 1;
	}
	
	length=0;
	for(n=0;n<(int)block->nBlocks;++n){
		length += (long)(block->nBlockSize[n]);
	}
	
	*data=(double *)cMalloc(length*sizeof(double),1018);
	if(!*data){ret=4;goto ErrorOut;};
	
	*dataLength=length;
	*dataType=SAGE_DOUBLE;
	
	length=0;
	for(n=0;n<(int)block->nBlocks;++n){
		fseek8(sage->in,(INT8_64)(block->xBlockAdress[n]+sizeof(double)*
			   (7+(itemNumber)*block->xBlockSize[n])),0);
		if(fget8(sage->in,(char *)(*data+length),(INT8_64)(sizeof(double)*block->nBlockSize[n]))){ret=5;goto ErrorOut;};
		length += (long)(block->nBlockSize[n]);
	}

	SageTranslateDoubles(sage,(double *)*data,*dataLength);
	
	ret = 0;

ErrorOut:
	return ret;
}

static int SageTranslateDoubles(struct PIO_BLK *sage,double *d,long length)
{
	unsigned char c,*cp;
	long np;


	if(!d || !sage)return 1;

	if(!sage->SageFlip)return 0;

	if(length <= 0)return 1;

	length=length*8;

        cp=(unsigned char *)d;
        for(np=0;np<length;np += 8){
            c=cp[np];
            cp[np]=cp[np+7];
            cp[np+7]=c;
            c=cp[np+1];
            cp[np+1]=cp[np+6];
            cp[np+6]=c;
            c=cp[np+2];
            cp[np+2]=cp[np+5];
            cp[np+5]=c;
            c=cp[np+3];
            cp[np+3]=cp[np+4];
            cp[np+4]=c;
        }
	return 0;
}

int SageGetFileItemBlk(struct FileInfo2 *Files,char *dataName,long plotItem,
					   double *data,long nb,long dataLength,long CurrentFrame)
{
	struct FilePIOInfo *pio;
	struct SetFrameData *s;
	struct PIO_BLK *sage;
	int dataType;
	int ret;
	
	if(!Files || !dataName || !data || !dataLength)return 1;
	if(!Files->PIOBlkList)return 1;
	s = &Files->SetData;
	pio=&Files->pioData;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIOBlkList[CurrentFrame];
	
	if(!sage)return 1;
	
	s->pioTime=sage->time;

	if(SetFileToRead8(CurrentFrame,Files))return 1;
	
	sage->in=Files->in8;
	
	ret=SageGetItemBlk(sage,pio,dataName,plotItem,data,nb,dataLength,&dataType);
	
	sage->in=NULL;
	
	return ret;
}

static int SageGetItemBlk(struct PIO_BLK *sage,struct FilePIOInfo *pio,char *dataName,long plotItem,double *data,long nb, long dataLength,int *dataType)
{
	
	if(!sage || !dataName)return 1;


	return SageGetDataBlk(sage,dataName,(int)plotItem,data,nb,dataLength,dataType);

}
static int SageGetDataBlk(struct PIO_BLK *sage, char *dataName, int index, double *data, long nb, long dataLength, int *dataType)
{
				  
	struct HeaderBlock *block;
	int itemNumber;
	long length;
	int ret;

	if(!sage || !dataName || !data || !dataType)return 1;
	if(!sage->in)return 1;	
	block=&sage->block;

	ret=2;

	itemNumber=SageItemNumber(sage,dataName,index);

	if(itemNumber < 0){
		return 1;
	}
	
	*dataType=SAGE_DOUBLE;
	
	fseek8(sage->in,(INT8_64)(block->xBlockAdress[nb]+sizeof(double)*
			        (7+(itemNumber)*block->xBlockSize[nb])),0);
	
	if(fget8(sage->in,(char *)data,(INT8_64)(sizeof(double)*block->xBlockSize[nb]))){ret=5;goto ErrorOut;};
	length=(long)(block->xBlockSize[nb]);

	SageTranslateDoubles(sage,(double *)data,length);
	
	ret = 0;

ErrorOut:
	return ret;
}

