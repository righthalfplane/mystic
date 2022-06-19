#include "Tools.h"
#include <string.h>

static int checkStack(pushpopPtr t);
static int checkDataStack(dataStackPtr t);

#define STACK_INCREMENT 50

int pushpopDepth(pushpopPtr pop)
{
	if(!pop)return 0L;
	return (int)pop->stackCount;
}

pushpopPtr pushpopStartR(long n)
{
	pushpopPtr l;
	int ret;
	
	if(n <= 0)return NULL;
	
	ret=1;
	
	l=(pushpopPtr)cMalloc(sizeof(struct pushpopStruct),1245);
	if(!l)goto ErrorOut;
	zerol((char *)l,sizeof(struct pushpopStruct));
	l->stackDataSize=n;
			
	ret=0;
ErrorOut:

    if(ret){
        pushpopEND(l);
        l=NULL;
    }
	
	return l;
	
}
int pushpopEND(pushpopPtr pop)
{
	if(!pop)return 1;
	
	if(pop->stackData){
	    cFree((char *)pop->stackData);
	    pop->stackData=NULL;
	}
	cFree((char *)pop);
	
	return 0;
}

int pushpopPushR(void *v,long n,pushpopPtr pop)
{
	if(!v || !pop)return 1;
	
	if((n <= 0) || (n != pop->stackDataSize))return 1;


	if(checkStack(pop)){
	    return 1;
	}
	
	memcpy(&pop->stackData[pop->stackCount*pop->stackDataSize],(char *)v,pop->stackDataSize);

	++(pop->stackCount);
		
	return 0;
}
int pushpopPeekR(void *v,long n,long nr,pushpopPtr pop)
{
	if(!v || !pop)return 1;
	
	if((n <= 0) || (n != pop->stackDataSize))return 1;

	if(pop->stackCount <= 0 || !pop->stackData)return 1;
	
	if(nr < 0 || nr >= pop->stackCount)return 1;
	
	memcpy(v,&pop->stackData[nr*pop->stackDataSize],pop->stackDataSize);
	
	return 0;
}
int pushpopPopR(void *v,long n,pushpopPtr pop)
{
	if(!v || !pop)return 1;
	
	if((n <= 0) || (n != pop->stackDataSize))return 1;

	if(pop->stackCount <= 0 || !pop->stackData)return 1;
	
	--(pop->stackCount);

	
	memcpy(v,&pop->stackData[pop->stackCount*pop->stackDataSize],pop->stackDataSize);

	
	return 0;
}
int pushpopPopAllR(void *v,long n,pushpopPtr pop)
{
	if(!v || !pop)return 1;
	
	if((n <= 0) || (n != pop->stackDataSize))return 1;

	if(pop->stackCount <= 0 || !pop->stackData)return 1;
	
	pop->stackCount=0;
	
	memcpy(v,&pop->stackData[pop->stackCount*pop->stackDataSize],pop->stackDataSize);

	
	return 0;
}
static int checkStack(pushpopPtr t)
{
    unsigned char *stackData;
    long stackMax;

	if(!t)return 1;

	if(t->stackCount+1 < t->stackMax)return 0;

	stackMax = t->stackMax+STACK_INCREMENT;

	stackData=NULL;
	if(t->stackData){
	    stackData=(unsigned char *)cRealloc((char *)t->stackData,stackMax*t->stackDataSize,8269);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)&stackData[t->stackMax*t->stackDataSize],STACK_INCREMENT*t->stackDataSize);
	}else{
	    stackData=(unsigned char *)cMalloc(stackMax*t->stackDataSize,8184);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)stackData,stackMax*t->stackDataSize);
	}

	t->stackMax=stackMax;
	t->stackData=stackData;
	return 0;
ErrorOut:
	return 1;
}

dataStackPtr dataStackStartR(long n)
{
	dataStackPtr l;
	int ret;
	
	if(n <= 0)return NULL;
	
	ret=1;
	
	l=(dataStackPtr)cMalloc(sizeof(struct dataStackStruct),1246);
	if(!l)goto ErrorOut;
	zerol((char *)l,sizeof(struct dataStackStruct));
	l->stackDataSize=n;
			
	ret=0;
ErrorOut:

    if(ret){
        dataStackEND(l);
        l=NULL;
    }
	
	return l;
	
}

int dataStackEND(dataStackPtr pop)
{
	if(!pop)return 1;
	
	if(pop->stackData){
	    cFree((char *)pop->stackData);
	    pop->stackData=NULL;
	}
	cFree((char *)pop);
	
	return 0;
}
void *dataStackNextR(dataStackPtr pop)
{
	if(!pop)return NULL;
	
	if(checkDataStack(pop)){
	    return NULL;
	}
	
	++(pop->stackCount);
		
	return (void *)&pop->stackData[pop->stackCount-1];
}

static int checkDataStack(dataStackPtr t)
{
    unsigned char *stackData;
    long stackMax;

	if(!t)return 1;

	if(t->stackCount+1 < t->stackMax)return 0;

	stackMax = t->stackMax+STACK_INCREMENT;

	stackData=NULL;
	if(t->stackData){
	    stackData=(unsigned char *)cRealloc((char *)t->stackData,stackMax*t->stackDataSize,8269);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)&stackData[t->stackMax*t->stackDataSize],STACK_INCREMENT*t->stackDataSize);
	}else{
	    stackData=(unsigned char *)cMalloc(stackMax*t->stackDataSize,8184);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)stackData,stackMax*t->stackDataSize);
	}

	t->stackMax=stackMax;
	t->stackData=stackData;
	return 0;
ErrorOut:
	return 1;
}



