/*
 *  doSage3DGet.h
 *  mShow
 *
 *  Created by Dale Ranta on 8/19/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */
#ifndef __doSage3DGet_h__

#define __doSage3DGet_h__

#include "Xappl.h"
#include "uFiles.h"
#include "initPIOBlocked.h"

int doSage3DGetStreamLineFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

double *doSage3DGetSDS3DFnt(struct FileInfo2 *Files,long CurrentFrame);

int doSage3DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li);

int doSage2DGetLineDataFnt(struct FileInfo2 *Files,struct linedata *li);

int doSage2DGetLineDataBlks(struct FileInfo2 *Files,struct linedata *li);

int doSage3DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai);

int doSage2DGetAreaDataFnt(struct FileInfo2 *Files,struct areadata *ai);

int doSage2DGetAreaDataBlk(struct FileInfo2 *Files,struct areadata *ai);

int doSage3DGetDataFnt(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int doSage2DGetDataBlks(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int SageGetFileItemFnt(struct FileInfo2 *Files,char *dataName,long plotItem,
                double **data,long *dataLength,long CurrentFrame);


#endif
