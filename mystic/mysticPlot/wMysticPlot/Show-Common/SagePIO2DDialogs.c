/*
 *  SagePIO2DDialogs.c
 *  mShow
 *
 *  Created by Dale Ranta on 6/7/11.
 *  Copyright 2011 SAIC. All rights reserved.
 *
 */

#define EXTERN22 extern

#include "SagePIO2D.h"

static int doBhangMeter2(struct BhangMeterData *bhangmeter2d);

static int dobhangStart(struct uDialogStruct *uList);

static int dobhangAction(struct uDialogStruct *uList);

static int SetLightData(struct uDialogStruct *uList,long n);

static int SaveLightData(struct uDialogStruct *uList,long n);

static int ClickLightList0(struct uDialogStruct *uList);

static int dobhangAddMeter(struct BhangMeterData *b);

static int gitIs;

int doBhangMeter3(struct BhangMeterData *bhangmeter2d);

int doBhangMeter3(struct BhangMeterData *bhangmeter2d)
{
	if(!bhangmeter2d)return 1;
	
	return doBhangMeter2(bhangmeter2d);
}
static int doBhangMeter2(struct BhangMeterData *bhangmeter2d)
{
	static uDialogGroup Items[] =
	
	{
		/* 00 */      {"materials",{20,20,110,135},uDialogScrollList,uDialogNone},
		
		/* 01 */      {"0", {217,65,110,20},uDialogText,uDialogGetDouble},
		/* 02 */      {"0", {337,65,110,20},uDialogText,uDialogGetDouble},
		/* 03 */      {"0", {457,65,110,20},uDialogText,uDialogGetDouble},
		
		/* 04 */      {"0", {217,35,110,20},uDialogText,uDialogGetDouble},
		/* 05 */      {"0", {337,35,110,20},uDialogText,uDialogGetLong},
		/* 06 */      {"0", {457,35,110,20},uDialogText,uDialogGetDouble},
		
		/* 07 */      {"Show Lines", {217,100,110,20},uDialogCheck,uDialogGetLong},
		
		
		/* 08 */      {"Ok", {15,232,80,20},uDialogButton,uDialogNone},
		/* 09 */      {"Cancel", {105,232,80,20},uDialogButton,uDialogNone},
		/* 10 */      {"Render", {195,232,80,20},uDialogButton,uDialogNone},
		/* 11 */      {"New", {285,232,80,20},uDialogButton,uDialogNone},
		/* 12 */      {"Delete", {375,232,80,20},uDialogButton,uDialogNone},
		
		/* 13 */      {"location", {135,65,65,20},uDialogString,uDialogNone},
		
		/* 14 */      {"Name", {217,10,65,20},uDialogString,uDialogNone},
		/* 15 */      {"Index", {337,10,75,20},uDialogString,uDialogNone},
		/* 16 */      {"Value", {457,10,75,20},uDialogString,uDialogNone},
		/* 17 */      {"Calculate", {217,130,110,20},uDialogCheck,uDialogGetLong},
		
		
	};
	
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,593,270};
	
	
	struct BhangMeterData bSave;
 	uDialogList *uList;
	uDialogReturn *uR;
	uPort save;
    int reti;
    int ret;
    int n;
	
	if(!bhangmeter2d)return 1;	
	
	uGetPort(&save);
	
	bSave = *bhangmeter2d;
	
	
	if(bhangmeter2d->xyz.count){
		bSave.xyz.x=cMalloc(bhangmeter2d->xyz.count*sizeof(double),8677);
		if(!bSave.xyz.x)return 1;
		bSave.xyz.y=cMalloc(bhangmeter2d->xyz.count*sizeof(double),8677);
		if(!bSave.xyz.y){
			cFree((char *)bSave.xyz.x);
			return 1;
		}
		bSave.xyz.z=cMalloc(bhangmeter2d->xyz.count*sizeof(double),8677);
		if(!bSave.xyz.z){
			cFree((char *)bSave.xyz.x);
			cFree((char *)bSave.xyz.y);
			return 1;
		}
		bSave.xyz.v=cMalloc(bhangmeter2d->xyz.count*sizeof(double),8677);
		if(!bSave.xyz.v){
			cFree((char *)bSave.xyz.x);
			cFree((char *)bSave.xyz.y);
			cFree((char *)bSave.xyz.z);
			return 1;
		}
		for(n=0;n<bhangmeter2d->xyz.count;++n)
		{
			bSave.xyz.x[n]=bhangmeter2d->xyz.x[n];
			bSave.xyz.y[n]=bhangmeter2d->xyz.y[n];
			bSave.xyz.z[n]=bhangmeter2d->xyz.z[n];
			bSave.xyz.v[n]=bhangmeter2d->xyz.v[n];
		}
	}
	else 
	{
		dobhangAddMeter(bhangmeter2d);
	}
	
	
	gitIs = -1;

	
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Bhangmeter Dialog",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	reti=uDialogSet(uList,0L,
					
					uDialogSetMyIcon,bhangmeter2d,	              
					uDialogSetAction,dobhangAction,	              
					uDialogSetStart,dobhangStart,
					
					uDialogSetItem,10L,
					uDialogSetHidden,(int)(1),
								
					uDialogSetDone,uDialogSetDone
					);
	if(reti)goto ErrorOut;
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
	
	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);
	
    if(!uR)goto ErrorOut;
    
	
	ret=0;
	
	
ErrorOut:
	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(ret){    
		if(bhangmeter2d->xyz.x)cFree((char *)bhangmeter2d->xyz.x);
		if(bhangmeter2d->xyz.y)cFree((char *)bhangmeter2d->xyz.y);
		if(bhangmeter2d->xyz.z)cFree((char *)bhangmeter2d->xyz.z);
		if(bhangmeter2d->xyz.v)cFree((char *)bhangmeter2d->xyz.v);
		*bhangmeter2d= bSave;
		
    }else{
		if(bSave.xyz.count){
			if(bSave.xyz.x)cFree((char *)bSave.xyz.x);
			if(bSave.xyz.y)cFree((char *)bSave.xyz.y);
			if(bSave.xyz.z)cFree((char *)bSave.xyz.z);
			if(bSave.xyz.v)cFree((char *)bSave.xyz.v);
		}
    }
	
	uSetPort(&save);
	
	return ret;
}
static int dobhangAction(struct uDialogStruct *uList)
{
	struct BhangMeterData *b;
	uDialogReturn *uR;
	long itis;
	int n,nn;
	int ret;
	
	if(!uList)return 1;
	b=(struct BhangMeterData *)uList->myIconItem;
	if(!b)return 1;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(uList->actionItem == 0){
		ClickLightList0(uList);
	}else if(uList->actionItem == 8){
		SaveLightData(uList,gitIs);
	}else if(uList->actionItem == 10){
		SaveLightData(uList,gitIs);
	}else if(uList->actionItem == 11){
		SaveLightData(uList,gitIs);
		dobhangAddMeter(b);
		dobhangStart(uList);
		SetLightData(uList,gitIs);
	}else if(uList->actionItem == 12){
	    itis=gitIs;
		SaveLightData(uList,gitIs);
		gitIs=0;
		if(gitIs == 0 && b->xyz.count == 1){
			Warning("Must Have At Least One Meter");
			goto ErrorOut;
		}
		nn=0;
		for(n=0;n<b->xyz.count;++n){
			if(itis == n)continue;
			b->xyz.x[nn]=b->xyz.x[n];
			b->xyz.y[nn]=b->xyz.y[n];
			b->xyz.z[nn]=b->xyz.z[n];
			b->xyz.v[nn++]=b->xyz.v[n];
		}
		b->xyz.count=nn;
		dobhangStart(uList);
		SetLightData(uList,0);
	}
	
	ret = 0;
	
ErrorOut:	
	if(uList)uDialogUpdate(uList);
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}
static int dobhangAddMeter(struct BhangMeterData *b)
{

	double *x,*y,*z,*v;
	int n;
	
	if(!b)return 1;
	
	x=cMalloc((b->xyz.count+1)*sizeof(double),8677);
	if(!x)return 1;
	
	y=cMalloc((b->xyz.count+1)*sizeof(double),8677);
	if(!y){
		cFree((char *)x);
		return 1;
	}
	
	z=cMalloc((b->xyz.count+1)*sizeof(double),8677);
	if(!z){
		cFree((char *)x);
		cFree((char *)y);
		return 1;
	}
	
	v=cMalloc((b->xyz.count+1)*sizeof(double),8677);
	if(!v){
		cFree((char *)x);
		cFree((char *)y);
		cFree((char *)z);
		return 1;
	}
	
	for(n=0;n<b->xyz.count;++n)
	{
		x[n]=b->xyz.x[n];
		y[n]=b->xyz.y[n];
		z[n]=b->xyz.z[n];
		v[n]=b->xyz.v[n];
	}
	
	x[b->xyz.count]=0;
	y[b->xyz.count]=0;
	z[b->xyz.count]=0;
	v[b->xyz.count]=0;
	
	if(b->xyz.x)cFree((char *)b->xyz.x);
	if(b->xyz.y)cFree((char *)b->xyz.y);
	if(b->xyz.z)cFree((char *)b->xyz.z);
	if(b->xyz.v)cFree((char *)b->xyz.v);
	b->xyz.x=x;
	b->xyz.y=y;
	b->xyz.z=z;
	b->xyz.v=v;
	gitIs=b->xyz.count;
	b->xyz.count += 1;
	
	return 0;
}
static int dobhangStart(struct uDialogStruct *uList)
{
	struct ScrollList *scroll;
	struct BhangMeterData *b;
	int ret;
	long count;
	long n;
	
	if(!uList)return 1;
	b=(struct BhangMeterData *)uList->myIconItem;
	if(!b)return 1;
	
	ret=uDialogSet(uList,0L,
				   uDialogGetScrollList,&scroll,
				   uDialogSetDone,uDialogSetDone
				   );
    
	if(ret)return 1;
	
	count=ScrollListCount(scroll);
	if(count){
		ScrollListDelete(0L,count,scroll);	
	}	
	
	for(n=0;n<b->xyz.count;++n){
	    char buff[256];
		char *data;
		char *names[]={"Meter",};
		data = &buff[0];
	    sprintf(buff,"%s<%ld>",names[0],n);
		if(ScrollListInsertOneType(&data,3,n,
								   1L,scroll))return 1;
	}
	ScrollListTop(0L,scroll);
	
	SetLightData(uList,0L);
	
	gitIs = 0;
	
	return 0;
}
static int ClickLightList0(struct uDialogStruct *uList)
{
	struct ScrollList *List0;
	struct BhangMeterData *b;
    int ret;
    long n;
	
	if(!uList)return 1;
	b=(struct BhangMeterData *)uList->myIconItem;
	if(!b)return 1;
	
	
	ret=uDialogSet(uList,0L,
				   uDialogGetScrollList,&List0,
				   
				   uDialogSetDone,uDialogSetDone
				   );		
	if(ret)goto ErrorOut;
	
	n=ScrollListFirstSelect(List0);
	if(n >= 0){
	    if((gitIs) >= 0 && (gitIs != n)){
			SaveLightData(uList,gitIs);
	    }
		gitIs=n;
	    SetLightData(uList,gitIs);		
	}
	
ErrorOut:
	
	if(uList){
		uDialogUpdate(uList);
		if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	}
	return 0;
}

static int SaveLightData(struct uDialogStruct *uList,long n)
{
	struct BhangMeterData *b;
	uDialogReturn *uR;
    int ret;
    
	if(!uList)return 1;
	b=(struct BhangMeterData *)uList->myIconItem;
	if(!b)return 1;
		
	ret = 1;
	uR=NULL;
	
    uR=uDialogGetReturn(uList);
    if(!uR)goto ErrorOut;
	
	if(n >= 0 && n < b->xyz.count)
	{
		b->xyz.x[n]=uR[1].dreturn;
		b->xyz.y[n]=uR[2].dreturn;
		b->xyz.z[n]=uR[3].dreturn;
		b->xyz.v[n]=uR[7].lreturn;
	}
	
	strncpy(b->pioName,uR[4].sreturn,sizeof(b->pioName));
	
	b->pioIndex=uR[5].lreturn;
    
	b->targetValue=uR[6].dreturn;
	
	b->bhangOn=uR[17].lreturn;
		
 	ret = 0;           
ErrorOut:	
	
    if(uR)uDialogFreeReturn(uR);
	
	return ret;
}
static int SetLightData(struct uDialogStruct *uList,long n)
{
	struct ScrollList *List0;
	struct BhangMeterData *b;
    int reti;
    int ret;
    
	if(!uList)return 1;
	b=(struct BhangMeterData *)uList->myIconItem;
	if(!b)return 1;
	
	
	if((n < 0))return 1;
	
	reti=uDialogSet(uList,0L,
					uDialogGetScrollList,&List0,
					
					uDialogSetDone,uDialogSetDone
					);		
	if(reti) return 1;
	
	ret = 1;
	
	if(n < b->xyz.count)
	{
	    reti=uDialogSet(uList,0L,
					
					uDialogSetItem,1L,
					uDialogSetDouble,(double)b->xyz.x[n],
					
					uDialogSetItem,2L,
					uDialogSetDouble,(double)b->xyz.y[n],
					
					uDialogSetItem,3L,
					uDialogSetDouble,(double)b->xyz.z[n],
					
					uDialogSetItem,7L,
					uDialogSetLogical,(b->xyz.v[n] > 0),
				
					uDialogSetDone,uDialogSetDone
					);		
	    if(reti) goto ErrorOut;
	}
	
	reti=uDialogSet(uList,0L,
							
							
							uDialogSetItem,4L,
							uDialogSetText,b->pioName,
							
							uDialogSetItem,5L,
							uDialogSetLong,b->pioIndex,
							
							uDialogSetItem,6L,
							uDialogSetDouble,(double)b->targetValue,
							
							uDialogSetItem,17L,
							uDialogSetLogical,(b->bhangOn > 0),
					
							uDialogSetDone,uDialogSetDone
							);		
	if(reti) goto ErrorOut;
	
	ScrollListDeSelect(List0);
	ScrollListSelect(n,List0);
	
	ret = 0;
ErrorOut:
	return ret;
}

