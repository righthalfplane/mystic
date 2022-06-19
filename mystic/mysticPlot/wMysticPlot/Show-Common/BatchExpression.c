#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "CExpress.h"
#include "Message.h"

typedef struct BatchExpressStruct{
	unsigned char *data;
	long CurrentFrame;
	long count;
	CExpressPtr e;
} *BatchExpressPtr;


int  ExpressGetInput(BatchExpressPtr exp,BatchPtr Batch);

int doExpress(BatchExpressPtr exp,CommandPtr cp,BatchPtr Batch);

int  doBatchExpression(BatchPtr Batch)
{
	struct BatchExpressStruct exp;
	struct CommandInfo cp;
	char line[4096];
	int ret;
	
	if(!Batch)return 1;
	
	ret=1;
	
	zerol((char *)&cp,sizeof(struct CommandInfo));
	
	zerol((char *)&exp,sizeof(struct BatchExpressStruct));
	
	if(ExpressGetInput(&exp,Batch))goto ErrorOut;
	
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doExpress(&exp,&cp,Batch))goto OK;
		}

	}
OK:	
	ret=0;
ErrorOut:
    if(exp.data)cFree((char *)exp.data);
    if(exp.e)CExpressClose(exp.e);
 	getCommand(NULL,&cp);
	return ret;
}
int doExpress(BatchExpressPtr exp,CommandPtr cp,BatchPtr Batch)
{
	struct Message1000 mo;
	char *command;
 	double value;
 	long first,last;
 	long n;
 	int reti;
 	int ret;
 	
	if(!cp || !Batch || !exp)return 1;
	
	ret=1;
	
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	if(!mstrcmp("evaluate",command)){
	    exp->e->CurrentFrame=exp->CurrentFrame;
	    exp->e->LastFrame=exp->CurrentFrame;
	    reti=CExpressEvaluate(exp->e,exp->data);
	    if(reti)goto ErrorOut;
	}else if(!mstrcmp("pos",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
		exp->CurrentFrame=(long)value;
	}else if(!mstrcmp("step",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(command){
    		mo.FrameCount=0;
    		if(SendMessageByName(command,MessageType_GET_FRAME_COUNT,&mo)){
		        goto ErrorOut;
		    }
		    first=0;
		    last=mo.FrameCount;
		    if(last <= 0)goto ErrorOut;
		    --last;
		}else{
		    if(doubleCommand(&value,cp))goto ErrorOut;
			first=(long)value;
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
			last=(long)value;
		}
		for(n=first;n<=last;++n){
		    exp->CurrentFrame=n;
			exp->e->CurrentFrame=exp->CurrentFrame;
			exp->e->LastFrame=last;
		    reti=CExpressEvaluate(exp->e,exp->data);
		    if(reti)goto ErrorOut;	
		}
	
	}else if(!mstrcmp("end",command)){
	    goto ErrorOut;
	}	
	ret = 0;
ErrorOut:
	return ret;
}
int  ExpressGetInput(BatchExpressPtr exp,BatchPtr Batch)
{
	long n,nmax,pcount;
	char line[4096],*cp;	
	int c,co,ret;
	
	if(!exp || !Batch)return 1;
	
	ret=1;
	
	n=0;
	nmax=0;
	pcount=0;
	
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))goto ErrorOut;
	    cp=line;
	    while((c = *cp++) != 0){
	        co=c;
	    	if(c == '{')goto FoundIt;	         
	    }	    
	}

FoundIt:
		
	pcount = 1;
		
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))goto ErrorOut;
	    cp=line;
		while((c = *cp++) != 0){
		    if((n+1) >= nmax){
		        if(exp->data){
		            exp->data=(unsigned char *)cRealloc((char *)exp->data,nmax+1000,7624);
		        }else{
		            exp->data=(unsigned char *)cMalloc(nmax+1000,7625);
		        }
		        if(!exp->data)goto ErrorOut;
		        if(n == 0){
		           exp->data[n++]=co;
		        }
		        nmax += 1000;
		    }
			if(c == '\r')c='\n';
		    exp->data[n++]=c;
		    if(c == '}'){
		        if(--pcount <= 0)goto Done;
		    }else if(c == '{'){
		        ++pcount;
		    }
		}
	}
	
Done:	

	exp->data[n]=0;
	
	exp->count=n;
	
	exp->e=CExpressStart(CExpressBatch);
	
	if(!exp->e)goto ErrorOut;

	ret=0;
	
ErrorOut:	

	return ret;
}
