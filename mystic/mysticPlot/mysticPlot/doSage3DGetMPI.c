/*
 *  doSage3DGet.c
 *  mShow
 *
 *  Created by Dale Ranta on 8/19/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */
 
#define EXTERN22 extern

#include "doSage3DGet.h"

int doSage2DGetLineDataBlks(struct FileInfo2 *Files,struct linedata *li);
int doSage2DGetAreaDataBlk(struct FileInfo2 *Files,struct areadata *ai);

#ifdef MPI_ON

#include <mpi.h>

int startMPI(int argc, char *argv[]);
int doCommands(int command);
int stopMPI(void);


static int SageTranslateDoubles(struct PIO_BLK *sage,double *d,long length);

int SageGetFileItemBlk2(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame);

int doSage3DGetLineDataBlk2(struct FileInfo2 *Files,struct linedata *li);

int doSage2DGetLineDataBlk2(struct FileInfo2 *Files,struct linedata *li);

int doSage3DGetAreaDataBlk2(struct FileInfo2 *Files,struct areadata *ai);

int doSage2DGetAreaDataBlk2(struct FileInfo2 *Files,struct areadata *ai);

int SendToAll(void *data,int length);

int SageItemNumber(struct PIO_BLK *sage,char *dataName,long plotItem);

int Sage3DGetDataBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int Sage2DGetDataBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int GetCurrentFilePath(struct FileInfo2 *Files,long CurrentFrame,char *filePath,long size);

int LoadAll(struct PIO_BLK *sage,char *filePath);

int LoadItem(struct PIO_BLK *sage, char *dataName, long plotItem, double **data, long *dataLength);

int LoadItemBlocks(struct PIO_BLK *sage, char *filePath, long plotItem, double **data, long *dataLength);

double *doSage3DGetSDS3DBlk2(struct FileInfo2 *Files,long CurrentFrame);

int doSage3DGetStreamLineBlk2(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int doSage2DGetLineDataFnt(struct FileInfo2 *Filesi,struct linedata *lii);


static struct FileInfo2 *Files;
static char *dataName;
static long plotItem;
static double **data;
static long *dataLength;
static long CurrentFrame;
static struct SetFrameData  *sd;
static struct linedata *li;

static struct areadata *ai;

static struct FileInfo2 FilesSave;
static struct SetFrameData  sdSave;
static struct linedata liSave;

static struct areadata aiSave;

static void displayOff(void);

static void displayOn(void);

static double *dReturn;

int doCommands(int command)
{
	
	int iret=0;
	
	MPI_Barrier(MPI_COMM_WORLD);
/*
	if(xg.rank == xg.root){
	   fprintf(stderr,"doCommands In rank %d\n",xg.rank );
	}
*/
	SendToAll(&command, sizeof(command));
	
	displayOff();
		
	switch(command)
	{
		case NoCommand:
			break;
		case QuitProgram:
			stopMPI();
			exit(0);
			break;
		case GetStreamDataCommand:
			if(xg.rank == xg.root){
				FilesSave=*Files;
				sdSave=*sd;
			}
						
			SendToAll(&FilesSave,sizeof(FilesSave));
			SendToAll(&sdSave,sizeof(sdSave));
			SendToAll(&CurrentFrame,sizeof(CurrentFrame));
			
			if(xg.rank != xg.root){
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				pio->streamline3d.xPlane.x=NULL;
				pio->streamline3d.xPlane.y=NULL;
				pio->streamline3d.xPlane.z=NULL;
				pio->streamline3d.xPlane.v=NULL;
				
				pio->streamline3d.yPlane.x=NULL;
				pio->streamline3d.yPlane.y=NULL;
				pio->streamline3d.yPlane.z=NULL;
				pio->streamline3d.yPlane.v=NULL;
				
				pio->streamline3d.zPlane.x=NULL;
				pio->streamline3d.zPlane.y=NULL;
				pio->streamline3d.zPlane.z=NULL;
				pio->streamline3d.zPlane.v=NULL;
				
				pio->sageBlk.names=NULL;
				pio->sageBlk.block.nBlockSize=NULL;
				pio->sageBlk.block.xBlockSize=NULL;
				pio->sageBlk.block.xBlockAdress=NULL;
				
				
				if(sdSave.streamline3d.xyz.count > 0){
					if(sdSave.streamline3d.xyz.x)sdSave.streamline3d.xyz.x=cMalloc(sdSave.streamline3d.xyz.count*sizeof(double),9872);
					if(sdSave.streamline3d.xyz.y)sdSave.streamline3d.xyz.y=cMalloc(sdSave.streamline3d.xyz.count*sizeof(double),9872);
					if(sdSave.streamline3d.xyz.z)sdSave.streamline3d.xyz.z=cMalloc(sdSave.streamline3d.xyz.count*sizeof(double),9872);
					if(sdSave.streamline3d.xyz.v)sdSave.streamline3d.xyz.v=cMalloc(sdSave.streamline3d.xyz.count*sizeof(double),9872);
				}
			}
			
			if(sdSave.streamline3d.xyz.count > 0){
				if(sdSave.streamline3d.xyz.x)SendToAll(sdSave.streamline3d.xyz.x,sdSave.streamline3d.xyz.count*sizeof(double));
				if(sdSave.streamline3d.xyz.y)SendToAll(sdSave.streamline3d.xyz.y,sdSave.streamline3d.xyz.count*sizeof(double));
				if(sdSave.streamline3d.xyz.z)SendToAll(sdSave.streamline3d.xyz.z,sdSave.streamline3d.xyz.count*sizeof(double));
				if(sdSave.streamline3d.xyz.v)SendToAll(sdSave.streamline3d.xyz.v,sdSave.streamline3d.xyz.count*sizeof(double));
			}
		
			
			iret=doSage3DGetStreamLineBlk2(&FilesSave,CurrentFrame,&sdSave);
		
			if(xg.rank == xg.root){
				*Files=FilesSave;
				*sd=sdSave;
			}else{
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				if(pio->streamline3d.xPlane.x)cFree((char *)pio->streamline3d.xPlane.x);
				if(pio->streamline3d.xPlane.y)cFree((char *)pio->streamline3d.xPlane.y);
				
				if(pio->streamline3d.yPlane.x)cFree((char *)pio->streamline3d.yPlane.x);
				if(pio->streamline3d.yPlane.y)cFree((char *)pio->streamline3d.yPlane.y);
				
				if(pio->streamline3d.zPlane.x)cFree((char *)pio->streamline3d.zPlane.x);
				if(pio->streamline3d.zPlane.y)cFree((char *)pio->streamline3d.zPlane.y);
			
				if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
				if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
				if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
				if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
			
				if(sdSave.streamline3d.xyz.count > 0){
					if(sdSave.streamline3d.xyz.x)cFree((char *)sdSave.streamline3d.xyz.x);
					if(sdSave.streamline3d.xyz.y)cFree((char *)sdSave.streamline3d.xyz.y);
					if(sdSave.streamline3d.xyz.z)cFree((char *)sdSave.streamline3d.xyz.z);
					if(sdSave.streamline3d.xyz.v)cFree((char *)sdSave.streamline3d.xyz.v);
				}
			}
			break;
		case GetSDS3DDataCommand:
		    dReturn=NULL;
			if(xg.rank == xg.root){
				FilesSave=*Files;
			}
			
			SendToAll(&FilesSave,sizeof(FilesSave));
			SendToAll(&CurrentFrame,sizeof(CurrentFrame));
			
			
			if(xg.rank != xg.root){
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				
				pio->sageBlk.names=NULL;
				pio->sageBlk.block.nBlockSize=NULL;
				pio->sageBlk.block.xBlockSize=NULL;
				pio->sageBlk.block.xBlockAdress=NULL;
			}
			
			dReturn=doSage3DGetSDS3DBlk2(&FilesSave,CurrentFrame);
			
			iret=0;
			if(dReturn == NULL)iret=1;

			if(xg.rank == xg.root){
				*Files=FilesSave;
			}else{
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
				pio->sageBlk.names=NULL;
				
				if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
				pio->sageBlk.block.nBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
				pio->sageBlk.block.xBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
				pio->sageBlk.block.xBlockAdress=NULL;
				
				
				if(dReturn)cFree((char *)dReturn);
			}
			
			break;
		case FileItemCommand:
			iret=SageGetFileItemBlk2(Files,dataName,plotItem,data,dataLength,CurrentFrame);
			break ;
			
			case GetAreaDataCommand:
			if(xg.rank == xg.root){
				FilesSave=*Files;
				aiSave=*ai;
			}
			
			SendToAll(&FilesSave,sizeof(FilesSave));
			SendToAll(&aiSave,sizeof(aiSave));
			
			if(xg.rank != xg.root){
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				
				pio->sageBlk.names=NULL;
				pio->sageBlk.block.nBlockSize=NULL;
				pio->sageBlk.block.xBlockSize=NULL;
				pio->sageBlk.block.xBlockAdress=NULL;
			}
			
			iret= doSage3DGetAreaDataBlk2(&FilesSave,&aiSave);
			
			if(xg.rank == xg.root){
				*Files=FilesSave;
				*ai=aiSave;
				
			}else{
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
				pio->sageBlk.names=NULL;
				
				if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
				pio->sageBlk.block.nBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
				pio->sageBlk.block.xBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
				pio->sageBlk.block.xBlockAdress=NULL;
			}
			break;
			
			
			
			case GetAreaDataCommand2D:
			if(xg.rank == xg.root){
				FilesSave=*Files;
				aiSave=*ai;
			}
			
			SendToAll(&FilesSave,sizeof(FilesSave));
			SendToAll(&aiSave,sizeof(aiSave));
			
			if(xg.rank != xg.root){
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				
				pio->sageBlk.names=NULL;
				pio->sageBlk.block.nBlockSize=NULL;
				pio->sageBlk.block.xBlockSize=NULL;
				pio->sageBlk.block.xBlockAdress=NULL;
			}
			
			iret= doSage2DGetAreaDataBlk2(&FilesSave,&aiSave);
			
			if(xg.rank == xg.root){
				*Files=FilesSave;
				*ai=aiSave;
				
			}else{
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
				pio->sageBlk.names=NULL;
				
				if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
				pio->sageBlk.block.nBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
				pio->sageBlk.block.xBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
				pio->sageBlk.block.xBlockAdress=NULL;
			}
			break;
			
		case GetLineDataCommand:
			if(xg.rank == xg.root){
				FilesSave=*Files;
				liSave=*li;
			}
						
			SendToAll(&FilesSave,sizeof(FilesSave));
			SendToAll(&liSave,sizeof(liSave));
			
			if(xg.rank != xg.root){
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				pio->streamline3d.xPlane.x=NULL;
				pio->streamline3d.xPlane.y=NULL;
				pio->streamline3d.xPlane.z=NULL;
				pio->streamline3d.xPlane.v=NULL;
				
				pio->streamline3d.yPlane.x=NULL;
				pio->streamline3d.yPlane.y=NULL;
				pio->streamline3d.yPlane.z=NULL;
				pio->streamline3d.yPlane.v=NULL;
				
				pio->streamline3d.zPlane.x=NULL;
				pio->streamline3d.zPlane.y=NULL;
				pio->streamline3d.zPlane.z=NULL;
				pio->streamline3d.zPlane.v=NULL;
				
				pio->sageBlk.names=NULL;
				pio->sageBlk.block.nBlockSize=NULL;
				pio->sageBlk.block.xBlockSize=NULL;
				pio->sageBlk.block.xBlockAdress=NULL;
				
				if(liSave.PointLineCount > 0){
					if(liSave.x)liSave.x=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
					if(liSave.y)liSave.y=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
					if(liSave.z)liSave.z=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
					if(liSave.v)liSave.v=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
				}
			}
			
			if(liSave.PointLineCount > 0){
				if(liSave.x)SendToAll(liSave.x,liSave.PointLineCount*sizeof(double));
				if(liSave.y)SendToAll(liSave.y,liSave.PointLineCount*sizeof(double));
				if(liSave.z)SendToAll(liSave.z,liSave.PointLineCount*sizeof(double));
				if(liSave.v)SendToAll(liSave.v,liSave.PointLineCount*sizeof(double));
			}
			/*
	   fprintf(stderr,"1 doCommands rank %ld %p %p %p %p\n",(long)xg.rank,liSave.x,liSave.y,liSave.z,liSave.v);
		*/
			iret=doSage3DGetLineDataBlk2(&FilesSave,&liSave);
			/*
	   fprintf(stderr,"2 doCommands rank %ld %p %p %p %p\n",(long)xg.rank,liSave.x,liSave.y,liSave.z,liSave.v);
	   */
			if(xg.rank == xg.root){
				*Files=FilesSave;
				*li=liSave;
				
			}else{
				struct FilePIOInfo *pio;
								
				pio = &FilesSave.pioData;
				if(pio->streamline3d.xPlane.x)cFree((char *)pio->streamline3d.xPlane.x);
				pio->streamline3d.xPlane.x=NULL;
				
				if(pio->streamline3d.xPlane.y)cFree((char *)pio->streamline3d.xPlane.y);
				pio->streamline3d.xPlane.y=NULL;
				
				if(pio->streamline3d.yPlane.x)cFree((char *)pio->streamline3d.yPlane.x);
				pio->streamline3d.yPlane.x=NULL;
				
				if(pio->streamline3d.yPlane.y)cFree((char *)pio->streamline3d.yPlane.y);
				pio->streamline3d.yPlane.y=NULL;
				
				if(pio->streamline3d.zPlane.x)cFree((char *)pio->streamline3d.zPlane.x);
				pio->streamline3d.zPlane.x=NULL;
				
				if(pio->streamline3d.zPlane.y)cFree((char *)pio->streamline3d.zPlane.y);
				pio->streamline3d.zPlane.y=NULL;
			
				if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
				pio->sageBlk.names=NULL;
				
				if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
				pio->sageBlk.block.nBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
				pio->sageBlk.block.xBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
				pio->sageBlk.block.xBlockAdress=NULL;
			
				if(liSave.PointLineCount > 0){
					if(liSave.x)cFree((char *)liSave.x);
					liSave.x=NULL;
					
					if(liSave.y)cFree((char *)liSave.y);
					liSave.y=NULL;
					
					if(liSave.z)cFree((char *)liSave.z);
					liSave.z=NULL;
					
					if(liSave.v)cFree((char *)liSave.v);
					liSave.v=NULL;
				}
			}
			/*
	   fprintf(stderr,"3 doCommands rank %ld %p %p %p %p\n",(long)xg.rank,liSave.x,liSave.y,liSave.z,liSave.v);
	   fprintf(stderr,"4 doCommands rank %ld %p %p\n",(long)xg.rank,liSave.xData,liSave.yData);
	   */
			break;
			
			
			
			
			
			
			
			
			
			
			
			
			
		case GetLineDataCommand2D:
			if(xg.rank == xg.root){
				FilesSave=*Files;
				liSave=*li;
			}
						
			SendToAll(&FilesSave,sizeof(FilesSave));
			SendToAll(&liSave,sizeof(liSave));
			
			if(xg.rank != xg.root){
				struct FilePIOInfo *pio;
				pio = &FilesSave.pioData;
				pio->streamline3d.xPlane.x=NULL;
				pio->streamline3d.xPlane.y=NULL;
				pio->streamline3d.xPlane.z=NULL;
				pio->streamline3d.xPlane.v=NULL;
				
				pio->streamline3d.yPlane.x=NULL;
				pio->streamline3d.yPlane.y=NULL;
				pio->streamline3d.yPlane.z=NULL;
				pio->streamline3d.yPlane.v=NULL;
				
				pio->streamline3d.zPlane.x=NULL;
				pio->streamline3d.zPlane.y=NULL;
				pio->streamline3d.zPlane.z=NULL;
				pio->streamline3d.zPlane.v=NULL;
				
				pio->sageBlk.names=NULL;
				pio->sageBlk.block.nBlockSize=NULL;
				pio->sageBlk.block.xBlockSize=NULL;
				pio->sageBlk.block.xBlockAdress=NULL;
				
				if(liSave.PointLineCount > 0){
					if(liSave.x)liSave.x=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
					if(liSave.y)liSave.y=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
					if(liSave.z)liSave.z=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
					if(liSave.v)liSave.v=(double *)cMalloc(liSave.PointLineCount*sizeof(double),8889);
				}
			}
			
			if(liSave.PointLineCount > 0){
				if(liSave.x)SendToAll(liSave.x,liSave.PointLineCount*sizeof(double));
				if(liSave.y)SendToAll(liSave.y,liSave.PointLineCount*sizeof(double));
				if(liSave.z)SendToAll(liSave.z,liSave.PointLineCount*sizeof(double));
				if(liSave.v)SendToAll(liSave.v,liSave.PointLineCount*sizeof(double));
			}
			/*
	   fprintf(stderr,"1 doCommands rank %ld %p %p %p %p\n",(long)xg.rank,liSave.x,liSave.y,liSave.z,liSave.v);
		*/
			iret=doSage2DGetLineDataBlk2(&FilesSave,&liSave);
			/*
	   fprintf(stderr,"2 doCommands rank %ld %p %p %p %p\n",(long)xg.rank,liSave.x,liSave.y,liSave.z,liSave.v);
	   */
			if(xg.rank == xg.root){
				*Files=FilesSave;
				*li=liSave;
				
			}else{
				struct FilePIOInfo *pio;
								
				pio = &FilesSave.pioData;
				if(pio->streamline3d.xPlane.x)cFree((char *)pio->streamline3d.xPlane.x);
				pio->streamline3d.xPlane.x=NULL;
				
				if(pio->streamline3d.xPlane.y)cFree((char *)pio->streamline3d.xPlane.y);
				pio->streamline3d.xPlane.y=NULL;
				
				if(pio->streamline3d.yPlane.x)cFree((char *)pio->streamline3d.yPlane.x);
				pio->streamline3d.yPlane.x=NULL;
				
				if(pio->streamline3d.yPlane.y)cFree((char *)pio->streamline3d.yPlane.y);
				pio->streamline3d.yPlane.y=NULL;
				
				if(pio->streamline3d.zPlane.x)cFree((char *)pio->streamline3d.zPlane.x);
				pio->streamline3d.zPlane.x=NULL;
				
				if(pio->streamline3d.zPlane.y)cFree((char *)pio->streamline3d.zPlane.y);
				pio->streamline3d.zPlane.y=NULL;
			
				if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
				pio->sageBlk.names=NULL;
				
				if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
				pio->sageBlk.block.nBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
				pio->sageBlk.block.xBlockSize=NULL;
				
				if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
				pio->sageBlk.block.xBlockAdress=NULL;
			
				if(liSave.PointLineCount > 0){
					if(liSave.x)cFree((char *)liSave.x);
					liSave.x=NULL;
					
					if(liSave.y)cFree((char *)liSave.y);
					liSave.y=NULL;
					
					if(liSave.z)cFree((char *)liSave.z);
					liSave.z=NULL;
					
					if(liSave.v)cFree((char *)liSave.v);
					liSave.v=NULL;
				}
			}
			/*
	   fprintf(stderr,"3 doCommands rank %ld %p %p %p %p\n",(long)xg.rank,liSave.x,liSave.y,liSave.z,liSave.v);
	   fprintf(stderr,"4 doCommands rank %ld %p %p\n",(long)xg.rank,liSave.xData,liSave.yData);
	   */
			break;
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		
		case GetDataCommand:
		
	   /*fprintf(stderr,"1 doCommands rank %ld %p %p\n",(long)xg.rank,Files,sd);*/
	   
		if(xg.rank == xg.root){
			FilesSave=*Files;
			sdSave=*sd;
		}
		
	   /* fprintf(stderr,"2 doCommands rank %ld %p %p\n",(long)xg.rank,Files,sd); */
	   		
		SendToAll(&FilesSave,sizeof(FilesSave));
		SendToAll(&sdSave,sizeof(sdSave));
		SendToAll(&CurrentFrame,sizeof(CurrentFrame));
		
		if(xg.rank != xg.root){
			struct FilePIOInfo *pio;
			pio = &FilesSave.pioData;
			pio->streamline3d.xPlane.x=NULL;
			pio->streamline3d.xPlane.y=NULL;
			pio->streamline3d.xPlane.z=NULL;
			pio->streamline3d.xPlane.v=NULL;
			
			pio->streamline3d.yPlane.x=NULL;
			pio->streamline3d.yPlane.y=NULL;
			pio->streamline3d.yPlane.z=NULL;
			pio->streamline3d.yPlane.v=NULL;
			
			pio->streamline3d.zPlane.x=NULL;
			pio->streamline3d.zPlane.y=NULL;
			pio->streamline3d.zPlane.z=NULL;
			pio->streamline3d.zPlane.v=NULL;
			
			pio->sageBlk.names=NULL;
			pio->sageBlk.block.nBlockSize=NULL;
			pio->sageBlk.block.xBlockSize=NULL;
			pio->sageBlk.block.xBlockAdress=NULL;
		
		}
			
		iret=Sage3DGetDataBlk2(&FilesSave,CurrentFrame,&sdSave);
		
		if(xg.rank == xg.root){
			*Files=FilesSave;
			*sd=sdSave;
		}else{
			struct FilePIOInfo *pio;
			pio = &FilesSave.pioData;
			if(pio->streamline3d.xPlane.x)cFree((char *)pio->streamline3d.xPlane.x);
			if(pio->streamline3d.xPlane.y)cFree((char *)pio->streamline3d.xPlane.y);
			
			if(pio->streamline3d.yPlane.x)cFree((char *)pio->streamline3d.yPlane.x);
			if(pio->streamline3d.yPlane.y)cFree((char *)pio->streamline3d.yPlane.y);
			
			if(pio->streamline3d.zPlane.x)cFree((char *)pio->streamline3d.zPlane.x);
			if(pio->streamline3d.zPlane.y)cFree((char *)pio->streamline3d.zPlane.y);
		
			if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
			if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
			if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
			if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
		
		}
		
		break ;
		
		
		
		case GetDataCommand2D:
		
	   /*fprintf(stderr,"1 doCommands rank %ld %p %p\n",(long)xg.rank,Files,sd);*/
	   
		if(xg.rank == xg.root){
			FilesSave=*Files;
			sdSave=*sd;
		}
		
	   /* fprintf(stderr,"2 doCommands rank %ld %p %p\n",(long)xg.rank,Files,sd); */
	   		
		SendToAll(&FilesSave,sizeof(FilesSave));
		SendToAll(&sdSave,sizeof(sdSave));
		SendToAll(&CurrentFrame,sizeof(CurrentFrame));
		
		if(xg.rank != xg.root){
			struct FilePIOInfo *pio;
			pio = &FilesSave.pioData;
			
			pio->streamline2d.zPlane.x=NULL;
			pio->streamline2d.zPlane.y=NULL;
			pio->streamline2d.zPlane.z=NULL;
			pio->streamline2d.zPlane.v=NULL;
			
			pio->sageBlk.names=NULL;
			pio->sageBlk.block.nBlockSize=NULL;
			pio->sageBlk.block.xBlockSize=NULL;
			pio->sageBlk.block.xBlockAdress=NULL;
		
		}
					
		iret=Sage2DGetDataBlk2(&FilesSave,CurrentFrame,&sdSave);
		
		if(xg.rank == xg.root){
			*Files=FilesSave;
			*sd=sdSave;
		}else{
			struct FilePIOInfo *pio;
			pio = &FilesSave.pioData;
			
			if(pio->streamline2d.zPlane.x)cFree((char *)pio->streamline3d.zPlane.x);
			if(pio->streamline2d.zPlane.y)cFree((char *)pio->streamline3d.zPlane.y);
		
			if(pio->sageBlk.names)cFree((char *)pio->sageBlk.names);
			if(pio->sageBlk.block.nBlockSize)cFree((char *)pio->sageBlk.block.nBlockSize);
			if(pio->sageBlk.block.xBlockSize)cFree((char *)pio->sageBlk.block.xBlockSize);
			if(pio->sageBlk.block.xBlockAdress)cFree((char *)pio->sageBlk.block.xBlockAdress);
		
		}
		
		break ;
		
		
		
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
/*
	if(xg.rank == xg.root){
	   fprintf(stderr,"doCommands Out \n");
	}
*/
	displayOn();
	return iret;
}
int doSage3DGetStreamLineFnt(struct FileInfo2 *Filesi,long CurrentFramei,struct SetFrameData  *sdi)
{	

/*
	return  doSage3DGetStreamLineBlk(Files,CurrentFrame,sd);
*/	
	Files=Filesi;
	CurrentFrame=CurrentFramei;
	sd=sdi;
	return doCommands(GetStreamDataCommand);
	
	
	
}

double *doSage3DGetSDS3DFnt(struct FileInfo2 *Filesi,long CurrentFramei)
{

	Files=Filesi;
	CurrentFramei=CurrentFramei;
	doCommands(GetSDS3DDataCommand);
	return dReturn;
}

int doSage3DGetAreaDataFnt(struct FileInfo2 *Filesi,struct areadata *aii)
{
	Files=Filesi;
	ai=aii;
	return doCommands(GetAreaDataCommand);
}
int doSage2DGetAreaDataFnt(struct FileInfo2 *Filesi,struct areadata *aii)
{
	Files=Filesi;
	ai=aii;
	return doCommands(GetAreaDataCommand2D);
}

static Display *mydisplay;

static void displayOff()
{
	mydisplay=xg.mydisplay;
	xg.mydisplay=NULL;
	
}
static void displayOn()
{
	xg.mydisplay=mydisplay;
	mydisplay=NULL;
}

int doSage3DGetLineDataFnt(struct FileInfo2 *Filesi,struct linedata *lii)
{
	Files=Filesi;
	li=lii;
	return doCommands(GetLineDataCommand);
}

int doSage2DGetLineDataFnt(struct FileInfo2 *Filesi,struct linedata *lii)
{
	Files=Filesi;
	li=lii;
	return doCommands(GetLineDataCommand2D);
}


int doSage3DGetDataFnt(struct FileInfo2 *Filesi,long CurrentFramei,struct SetFrameData  *sdi)
{
	Files=Filesi;
	CurrentFrame=CurrentFramei;
	sd=sdi;
	return doCommands(GetDataCommand);

}

int doSage2DGetDataFnt(struct FileInfo2 *Filesi,long CurrentFramei,struct SetFrameData  *sdi)
{
	Files=Filesi;
	CurrentFrame=CurrentFramei;
	sd=sdi;
	return doCommands(GetDataCommand2D);
}

int SageGetFileItemFnt(struct FileInfo2 *Filesi,char *dataNamei,long plotItemi,
                       double **datai,long *dataLengthi,long CurrentFramei)
{
	Files=Filesi;
	dataName=dataNamei;
	plotItem=plotItemi;
	data=datai;
	dataLength=dataLengthi;
	CurrentFrame=CurrentFramei;
									  
	return doCommands(FileItemCommand);
}

int SageGetFileItemBlk2(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame)

{
		static struct PIO_BLK sage;
		char filePath[2048];
		long item=0;
		int iret=0;
		
		if(xg.rank == xg.root){
			*dataLength=0;
			*data=NULL;
			if(GetCurrentFilePath(Files,CurrentFrame,filePath,sizeof(filePath)))iret=1;
			if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
			if(CurrentFrame < 0)CurrentFrame=0;
			sage = Files->PIOBlkList[CurrentFrame];
			item=SageItemNumber(&sage,dataName,plotItem);
			if(item < 0)iret=1;
		}
		
		SendToAll(&iret, sizeof(iret));
	
		if(iret == 1){
			printf("SageGetFileItemBlk2 Error - rank %ld\n",(long)xg.rank);
			return 1;
		}
		
		SendToAll(&item, sizeof(item));
		
		SendToAll(&sage, sizeof(sage));
					
		SendToAll(&filePath, sizeof(filePath));
				
		LoadItemBlocks(&sage, filePath, item, data, dataLength);
		
		iret=0;
		
		return iret;
}


int LoadItemBlocks(struct PIO_BLK *sage, char *filePath, long item, double **data, long *dataLength)
{
	MPI_File myfile;
	MPI_Status status;
	MPI_Offset offset;
	unsigned char header[9];
	double two;
	double twoLocal=2.0;
	struct HeaderBlock *block;
	long length;
	double *recvbuf,*save;
	double *databuf=NULL;
	int *intbuff=NULL;
	int *counts=NULL;
	int *displacements=NULL;
	int ret=1;
	int n,k;
			
	block=&sage->block;
	
	if(xg.rank != xg.root){
		sage->names=cMalloc(block->itemsNum*sizeof(struct ItemNames),94823);
		block->nBlockSize=(double *)cMalloc(block->nBlocks*sizeof(double),9482);
		block->xBlockSize=(double *)cMalloc(block->nBlocks*sizeof(double),9482);
		block->xBlockAdress=(double *)cMalloc(block->nBlocks*sizeof(double),9582);
		if(!block->xBlockSize || !block->xBlockAdress || !block->nBlockSize || !sage->names)goto ErrorOut;
	}
	
	databuf=(double *)cMalloc(sage->xBlockSizeMax*sizeof(double),9482);
	if(!databuf)goto ErrorOut;
	
	intbuff=(int *)cMalloc(block->nBlocks*sizeof(double),9482);
	if(!intbuff)goto ErrorOut;
	
	SendToAll(sage->names, block->itemsNum*sizeof(struct ItemNames));
	SendToAll(block->nBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockAdress, block->nBlocks*sizeof(double));
	
	length=0;
	for(n=0;n<block->nBlocks;++n){
		intbuff[n]= length;
		length += block->nBlockSize[n];
	}

	recvbuf=NULL;
	save=NULL;
	if(xg.rank == xg.root){
		*data=(double *)cMalloc(length*sizeof(double),9482);
		recvbuf=*data;
		if(!recvbuf)goto ErrorOut;
		zerol((char *)recvbuf,length*sizeof(double));
		save=recvbuf;
		*dataLength=length;
		
		counts=(int *)cMalloc(xg.size*sizeof(int),9482);
		displacements=(int *)cMalloc(xg.size*sizeof(int),9482);
		if(!counts || !displacements)goto ErrorOut;
		
		/* memcpy(*data,block->xBlockBuffer+item*(long)(block->xBlockSize[xg.rank]),(long)(block->nBlockSize[xg.rank])*sizeof(double)); */
		/* fprintf(stderr,"%ld\n",(long)(block->nBlockSize[xg.rank])); */
		
	}

	MPI_File_open(MPI_COMM_WORLD,filePath,MPI_MODE_RDONLY,MPI_INFO_NULL,&myfile);
	
	for(n=0;n<block->nBlocks;n += xg.size){
		for(k=0;k<xg.size;++k){
		    int nb=(n + k) % (int)(block->nBlocks);
			if(xg.rank == xg.root){
				counts[k]=(int)(block->nBlockSize[nb])*sizeof(double);
				displacements[k]=intbuff[nb]*sizeof(double);
				if(n+k >= block->nBlocks){
					counts[k]=0;
				}
				/* fprintf(stderr,"n %d n+k %d k %d counts %d displacements %d\n",n,n+k,k,counts[k],displacements[k]); */
			}
		}
		for(k=0;k<xg.size;++k){
		    int nb=(n + k) % (int)(block->nBlocks);
			int np=(n + k) % xg.size;
			
			if(np == xg.rank){
			
			    int count;
			
				offset=block->xBlockAdress[nb];
				
				MPI_File_seek(myfile, offset, MPI_SEEK_SET);
				
				zerol((char *)&status,sizeof(status));
				
				MPI_File_read(myfile, header, 8L, MPI_CHARACTER, &status);
				
				header[8]=0;
				
				MPI_File_seek(myfile, offset + 8, MPI_SEEK_SET);
				
				zerol((char *)&status,sizeof(status));
				
				MPI_File_read(myfile, &two, sizeof(double), MPI_CHARACTER, &status);
				
				sage->SageFlip= !(two == twoLocal);
/*
				fprintf(stderr,"SageFlip %d nb %d np %d intbuff %d length %ld recvbuf %p %p\n",sage->SageFlip,nb,np,
				       intbuff[nb]+(int)block->nBlockSize[nb],length,recvbuf+intbuff[nb]+(int)block->nBlockSize[nb],
					   (char *)(recvbuf+intbuff[nb])+(int)block->nBlockSize[nb]*sizeof(double));
*/
				MPI_File_seek(myfile, offset + sizeof(double)*(7+block->xBlockSize[nb]*item) , MPI_SEEK_SET);
				
				MPI_File_read(myfile,databuf, block->nBlockSize[nb]*sizeof(double), MPI_CHARACTER, &status);
				
				SageTranslateDoubles(sage,databuf,block->nBlockSize[nb]);
				
				count=(int)(block->nBlockSize[nb])*sizeof(double);
				if(n+k >= block->nBlocks){
					count=0;
				}
				
				MPI_Gatherv(databuf,count , MPI_CHARACTER,
						   recvbuf, counts, displacements, MPI_CHARACTER,
						   xg.root, MPI_COMM_WORLD);
				
			}
		}
	}
	
	MPI_File_close(&myfile);
	
	ret=0;
ErrorOut:
	if(databuf)cFree((char *)databuf);
	if(intbuff)cFree((char *)intbuff);
	if(counts)cFree((char *)counts);
	if(displacements)cFree((char *)displacements);
	return ret;
}
int SageGetFileItemBlk2LOADALL(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame)

{
		static long LoadedFrame = -1;
		static struct PIO_BLK sage;
		struct HeaderBlock *block;
		char filePath[2048];
		int iret=0;
		
		if(xg.rank == xg.root){
			*dataLength=0;
			*data=NULL;
		}
		
		SendToAll(&CurrentFrame, sizeof(CurrentFrame));
		
		if(CurrentFrame != LoadedFrame){
			if(sage.block.xBlockBuffer)cFree((char *)sage.block.xBlockBuffer);
			sage.block.xBlockBuffer=NULL;
			
			if(xg.rank == xg.root){
				if(GetCurrentFilePath(Files,CurrentFrame,filePath,sizeof(filePath)))iret=1;
				
				if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
				if(CurrentFrame < 0)CurrentFrame=0;
				LoadedFrame=CurrentFrame;
				sage = Files->PIOBlkList[CurrentFrame];
			}else{
				if(sage.names)cFree((char *)sage.names);
				if(sage.block.nBlockSize)cFree((char *)sage.block.nBlockSize);
				if(sage.block.xBlockSize)cFree((char *)sage.block.xBlockSize);
				if(sage.block.xBlockAdress)cFree((char *)sage.block.xBlockAdress);
				
				zerol((char *)&sage,sizeof(sage));
			}
			
			SendToAll(&iret, sizeof(iret));
		
			if(iret == 1){
				printf("SageGetFileItemBlk2LOADALL Error - rank %ld\n",(long)xg.rank);
				return 1;
			}
			
			iret=1;
			
			SendToAll(&LoadedFrame, sizeof(LoadedFrame));
			
			SendToAll(&sage, sizeof(sage));
						
			SendToAll(&filePath, sizeof(filePath));
			
			block=&sage.block;
			
			if(block->nBlocks == xg.size){
				LoadAll(&sage,filePath);
			}
		}
		
		iret=1;
		
		block=&sage.block;
				
		if(block->nBlocks == xg.size){
			LoadItem(&sage,dataName,plotItem,data,dataLength);
		}
		iret=0;
		return iret;
}
int LoadItem(struct PIO_BLK *sage, char *dataName, long plotItem, double **data, long *dataLength)
{
	struct HeaderBlock *block;
	long length,location;
	double *recvbuf,*save;
	double *sendbuf;
	int item=0;
	int ret=1;
	int n;
	
	if(xg.rank == xg.root){
		item=SageItemNumber(sage,dataName,plotItem);
		if(item < 0){
			goto ErrorOut;
		}
	}
	
	SendToAll(&item, sizeof(item));
		
	block=&sage->block;
	
	length=0;
	location=0;
	for(n=0;n<block->nBlocks;++n){
	    if(n == xg.rank)location=length;
		length += block->nBlockSize[n];
	}

	recvbuf=NULL;
	save=NULL;
	if(xg.rank == xg.root){
		*data=(double *)cMalloc(length*sizeof(double),9482);
		recvbuf=*data;
		if(!recvbuf)goto ErrorOut;
		zerol((char *)recvbuf,length*sizeof(double));
		save=recvbuf;
		*dataLength=length;
		memcpy(*data,block->xBlockBuffer+item*(long)(block->xBlockSize[xg.rank]),(long)(block->nBlockSize[xg.rank])*sizeof(double));
		/* fprintf(stderr,"%ld\n",(long)(block->nBlockSize[xg.rank])); */
		
	}
	
	recvbuf += location;
	sendbuf = block->xBlockBuffer+item*(long)(block->xBlockSize[xg.rank]);
	
/*
	fprintf(stderr,"%ld %p %p %p %ld %ld %ld\n",(long)xg.rank,recvbuf,sendbuf,block->xBlockBuffer,
	       (long)(recvbuf-save),(long)(sendbuf-block->xBlockBuffer),(long)(block->xBlockSize[xg.rank]));
*/

	MPI_Gather(sendbuf, (int)(block->nBlockSize[xg.rank])*sizeof(double), MPI_CHARACTER,
	           recvbuf, (int)(block->nBlockSize[xg.rank])*sizeof(double), MPI_CHARACTER,
               xg.root, MPI_COMM_WORLD);

	ret=0;
ErrorOut:
	return ret;
	
}
int LoadAll(struct PIO_BLK *sage,char *filePath)
{
	struct HeaderBlock *block;
	MPI_File myfile;
	MPI_Status status;
	MPI_Offset offset;
	unsigned char header[9];
	double two;
	double twoLocal=2.0;
	long length;
	int iret=1;
	
	if(!sage || !filePath)goto ErrorOut;
	
	block=&sage->block;
	
	if(xg.rank != xg.root){
		sage->names=cMalloc(block->itemsNum*sizeof(struct ItemNames),94823);
		block->nBlockSize=(double *)cMalloc(block->nBlocks*sizeof(double),9482);
		block->xBlockSize=(double *)cMalloc(block->nBlocks*sizeof(double),9482);
		block->xBlockAdress=(double *)cMalloc(block->nBlocks*sizeof(double),9582);
		if(!block->xBlockSize || !block->xBlockAdress || !block->nBlockSize || !sage->names)goto ErrorOut;
	}
	
	SendToAll(sage->names, block->itemsNum*sizeof(struct ItemNames));
	SendToAll(block->nBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockSize, block->nBlocks*sizeof(double));
	SendToAll(block->xBlockAdress, block->nBlocks*sizeof(double));

	MPI_File_open(MPI_COMM_WORLD,filePath,MPI_MODE_RDONLY,MPI_INFO_NULL,&myfile);
	
	offset=block->xBlockAdress[xg.rank];
	
	MPI_File_seek(myfile, offset, MPI_SEEK_SET);
	
	zerol((char *)&status,sizeof(status));
	
	MPI_File_read(myfile, header, 8L, MPI_CHARACTER, &status);
	
	header[8]=0;
	
	MPI_File_seek(myfile, offset + 8, MPI_SEEK_SET);
	
	zerol((char *)&status,sizeof(status));
	
	MPI_File_read(myfile, &two, sizeof(double), MPI_CHARACTER, &status);
	
	sage->SageFlip= !(two == twoLocal);
		
	length=block->xBlockSize[xg.rank]*(block->itemsNum+12);
	block->xBlockBuffer=(double *)cMalloc(length*sizeof(double),9582);
	if(!block->xBlockBuffer)goto ErrorOut;

	MPI_File_seek(myfile, offset + sizeof(double)*7 , MPI_SEEK_SET);
	
	MPI_File_read(myfile,block->xBlockBuffer, length*sizeof(double), MPI_CHARACTER, &status);
	
	SageTranslateDoubles(sage,block->xBlockBuffer,length);
	
	MPI_File_close(&myfile);
		
	iret=0;
ErrorOut:
    if(iret){
	    fprintf(stderr,"LoadAll Error - file %s rank %ld\n",filePath,(long)xg.rank);
	}
	return iret;
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

int startMPI(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &xg.size);
  MPI_Comm_rank(MPI_COMM_WORLD, &xg.rank);
  xg.root=0;
 /* printf("size %ld rank %ld\n",(long)xg.size,(long)xg.rank); */
  return 0;
}
int stopMPI()
{
    MPI_Finalize();
  /*  printf("stopMPI size %ld rank %ld\n",(long)xg.size,(long)xg.rank); */
	return 0;
}
int SendToAll(void *data,int length)
{
	int ret;
	/*
	fprintf(stderr,"MPI_Bcast Rank %d data %p length %d\n",xg.rank,data,length);
	*/
	ret=MPI_Bcast(data, length, MPI_CHARACTER, xg.root, MPI_COMM_WORLD);
	
	if(ret != MPI_SUCCESS){
		fprintf(stderr,"MPI_Bcast rank %d Error %d data %p length %d\n",xg.size,ret,data,length);
	}
	
	return ret;

}
int SendToOthers(void *data,int length,int source)
{
	return MPI_Bcast(data, length, MPI_CHARACTER, source, MPI_COMM_WORLD);

}
int clearData(void **data);

int clearData(void **data)
{
	void *dat;
	
	if(xg.rank == xg.root)return 0;
	
	if(!data)return 1;
	
	dat=*data;
	if(dat){
	    cFree(dat);
	}
	*data=NULL;
	return 0;
	
}

#else
int clearData(void **data)
{
	void *dat;
	if(!data)return 1;
	dat=*data;
	return 0;
	
}
int SendToAll(void *data,int length)
{
	if(!data)return 1;
	return 1;
}
int SendToOthers(void *data,int length,int source)
{
	if(!data)return 1;
	return 1;
}
int SageGetFileItemFnt(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame)
{
	return SageBlkGetFileItem(Files,dataName,plotItem,
					                  data,dataLength,CurrentFrame);
}
int doSage3DGetDataFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	return doSage3DGetDataBlk(Files,CurrentFrame,sd);
}
int doSage3DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li)
{
	return doSage3DGetLineDataBlk(Files,li);
}
int doSage3DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai)
{
	return doSage3DGetAreaDataBlk(Files,ai);
}
double *doSage3DGetSDS3DFnt(struct FileInfo2 *Files,long CurrentFrame)
{
	return doSage3DGetSDS3DBlk(Files,CurrentFrame);
}
int doSage3DGetStreamLineFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{	
	return  doSage3DGetStreamLineBlk(Files,CurrentFrame,sd);
}


int doSage2DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li)
{
	return doSage2DGetLineDataBlks(Files,li);
}

int doSage2DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai)
{
	return doSage2DGetAreaDataBlk(Files,ai);
}

int doSage2DGetDataFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	return doSage2DGetDataBlks(Files,CurrentFrame,sd);
}

#endif


int GetCurrentFilePath(struct FileInfo2 *Files,long CurrentFrame,char *filePath,long size)
{
	if(!Files || !filePath || size < 1)return 1;
	
	if(CurrentFrame < 0 || CurrentFrame >= Files->FileCount)CurrentFrame=0;
	
	filePath[0]=0;
	
	if(Files->directory){
		mstrncpy(filePath,Files->directory,size);
	}
	
	if(Files->FilesNames && Files->FilesNames[CurrentFrame]){
		strncatToPath(filePath,Files->FilesNames[CurrentFrame],size);
	}

	return 0;
}

