/*
 *  initPIOBlocked.h
 *  mShow
 *
 *  Created by Dale Ranta on 7/29/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */
#ifndef __INITPIOBLOCKED__
#define __INITPIOBLOCKED__

#include "Xappl.h"
#include "uFiles.h"

int IsPIOBlk(char *namef,int *rank);

int initPIOBlk(struct FileInfo2 *Files);

struct PIO_BLK *sageHeaderBLK(char *name);

int sageCloseBlk(struct PIO_BLK *sage,int flag);

int filePIOBlkList(struct FileInfo2 *Files,long CurrrentFrame,struct ScrollList *List,int flag);

int SageBlkGetFileItem(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame);
				
int SageGetFileItemBlk(struct FileInfo2 *Files,char *dataName,long plotItem,
                         double *data,long nb,long dataLength,long CurrentFrame);
						 
int doSage3DBlkStart(struct FileInfo2 *Files);

int doSage2DStartBlk(struct FileInfo2 *Files);

int doSage3DGetStreamLineBlk(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

double *doSage3DGetSDS3DBlk(struct FileInfo2 *Files,long CurrentFrame);

int doSage3DGetLineDataBlk(struct FileInfo2 *Files,struct linedata *li);

int doSage3DGetAreaDataBlk(struct FileInfo2 *Files,struct areadata *ai);

int doSage3DGetDataBlk(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int SageItemNumber(struct PIO_BLK *sage,char *dataName,long plotItem);

int doSage2DBatchBlk(BatchPtr Batch,char *name);

int doSage2DGetDataBlks(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData *sd);

#endif
