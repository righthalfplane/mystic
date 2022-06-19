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

int doSage3DGetStreamLineFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{	
	return  doSage3DGetStreamLineBlk(Files,CurrentFrame,sd);
}

double *doSage3DGetSDS3DFnt(struct FileInfo2 *Files,long CurrentFrame)
{
	return doSage3DGetSDS3DBlk(Files,CurrentFrame);
}

int doSage3DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li)
{
	return doSage3DGetLineDataBlk(Files,li);
}

int doSage2DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li)
{
	return doSage2DGetLineDataBlks(Files,li);
}

int doSage3DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai)
{
	return doSage3DGetAreaDataBlk(Files,ai);
}

int doSage2DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai)
{
	return doSage2DGetAreaDataBlk(Files,ai);
}

int doSage3DGetDataFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	return doSage3DGetDataBlk(Files,CurrentFrame,sd);
}
int doSage2DGetDataFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	return doSage2DGetDataBlks(Files,CurrentFrame,sd);
}
int SageGetFileItemFnt(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame)
{
	return SageBlkGetFileItem(Files,dataName,plotItem,
					                  data,dataLength,CurrentFrame);
}
