#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uLib.h"
#include "uFiles.h"
#include "paletteDraw.h"
#include "Linedata.h"
#include "Message.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Tools.h"
#include "fileSend.h"
struct MBHead {		/* MacBinary Header */
	char zero1;
	unsigned char nlen;
	unsigned char name[63];
	unsigned char type[4];
	unsigned char creator[4];
	char flags;
	char zero2;
	char location[6];
	char dprotected;
	char zero3;
	unsigned char dflen[4];
	unsigned char rflen[4];
	unsigned char cdate[4];
	unsigned char mdate[4];
};


int uMAB(struct uNetWork *uNet,IconPtr myIcon,char *path);

int SendMacBinaryFile(char *namei,char *pathnamei,netPtr s);

int cleanName(char *buff);

int NetNameSize(struct NetWork *Net,char *buff,long *nn);

int putString(unsigned char *s,long n,FILE *in);

int getString(unsigned char *s,long n,FILE *in);

int SendMacBinaryFile(char *namei,char *pathnamei,netPtr s)
{
	unsigned char c,*bp,*bh;
	unsigned long n,k,dLength,rLength;
	long length,sdLength,srLength,size,Total;
	struct MBHead Head;
	FILE *in;
	int ret;
	long nt;
	
	if(!namei || !pathnamei || !s)return 1;
	
	ret = 1;
	in = NULL;
	
	
	if(!(in=fopen(pathnamei,"rb"))){
	    sprintf(WarningBuff,"Could Not Open (%s) To Read\n",pathnamei);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	if(fseek(in,0L,2)){
	    sprintf(WarningBuff,"(%s) Error Looking For End Of File\n",pathnamei);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	if((length=ftell(in)) == -1L){
	    sprintf(WarningBuff,"(%s) Error Reading For End Of File Location\n",pathnamei);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	if(fseek(in,0L,0)){
	    sprintf(WarningBuff,"(%s) Error Rewinding File\n",pathnamei);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}


	if(length < 128){
	    sprintf(WarningBuff,"(%s) Error File length (%ld) Not a MacBinary File\n",pathnamei,length);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if(SetBuffers(IO_BUFFER+512L))goto ErrorOut;


	if(getString(xg.buffin,128,in)){
	    sprintf(WarningBuff,"(%s) Error Reading File length (%d)\n",pathnamei,128);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	bp=xg.buffin;
	bh=(unsigned char *)&Head;
	for(n=0;n<128;++n){
	    c = *bp++;
	    if(n < sizeof(struct MBHead)){
	        *bh++ = c;
	    }
	}

	k = Head.nlen;
	
	dLength = (((unsigned long)Head.dflen[0]) << 24) + (((unsigned long)Head.dflen[1]) << 16) +
	          (((unsigned long)Head.dflen[2]) << 8 ) + (((unsigned long)Head.dflen[3])      );

	rLength = (((unsigned long)Head.rflen[0]) << 24) + (((unsigned long)Head.rflen[1]) << 16) +
	          (((unsigned long)Head.rflen[2]) << 8 ) + (((unsigned long)Head.rflen[3])      );

	sdLength=(long)dLength;
	srLength=(long)rLength;

	if(sdLength < 0 || srLength < 0){
	    sprintf(WarningBuff,"(%s) Error in MacBinary File Data (%ld) Resource (%ld)\n",pathnamei,sdLength,srLength);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	
	Total = 128;

	n=(dLength+127)/128;

	Total += n*128;

	n=(rLength+127)/128;

	Total += n*128;

	if(Total > length || Total < 0){
	    sprintf(WarningBuff,"(%s) Error File length (%ld) is short by (%ld) bytes\n",pathnamei,length,Total-length);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
 	size=(long)(1+Head.nlen+dLength+rLength+16+8);
	
    if(SendLab(s->Net,"MAB2",size))goto ErrorOut;
    
 	if(SendByte(s->Net,Head.nlen))goto ErrorOut;
	if(Head.nlen > 0){
	    if(Head.nlen > 63){
	    	sprintf(WarningBuff,"(%s) Error File Name length (%ld) bytes\n",pathnamei,(long)Head.nlen);
	    	WarningBatch(WarningBuff);
	    }
	    if(SendData(s->Net,(char *)Head.name,(long)Head.nlen))goto ErrorOut;
	}
   
   if(SendData(s->Net,(char *)Head.creator,4L))goto ErrorOut;
	
   if(SendData(s->Net,(char *)Head.type,4L))goto ErrorOut;
	
   if(SendData(s->Net,(char *)Head.cdate,4L))goto ErrorOut;
	
   if(SendData(s->Net,(char *)Head.mdate,4L))goto ErrorOut;
   
   if(SendLong(s->Net,dLength))goto ErrorOut;
   
   if(SendLong(s->Net,rLength))goto ErrorOut;

	if(dLength){
		n=(dLength+127)/128;
		k=n*128-dLength;
		while(dLength > 0){
		    nt=(long)dLength;
		    if(nt > IO_BUFFER)nt=IO_BUFFER;

		    if(getString(xg.buffin,nt,in))goto ErrorOut;
		    
		    if(SendData(s->Net,(char *)xg.buffin,nt))goto ErrorOut;
		    dLength -= nt;
		}
		
		if(k > 0){
		    if(getString(xg.buffin,(long)k,in))goto ErrorOut;
		}
	}

	if(rLength){
		while(rLength > 0){
		    nt=(long)rLength;
		    if(nt > IO_BUFFER)nt=IO_BUFFER;

		    if(getString(xg.buffin,nt,in))goto ErrorOut;
		    
		    if(SendData(s->Net,(char *)xg.buffin,nt))goto ErrorOut;
		    rLength -= nt;
		}		
	}
	
	ret = 0;
ErrorOut:
    if(in)fclose(in);
	return ret;
}

int uMAB(struct uNetWork *uNet,IconPtr myIcon,char *path)
{
	struct MBHead HEAD;
	long Data,Resource,DataN,ResourceN;
	char buff[256],*bp,*bh;
	FILE *out;
	long k,n;
	int ret;
	
	
	if(!uNet || !path || !myIcon)return 1;
	
	ret=1;
	out=NULL;
		
	zerol((char *)&HEAD,sizeof(struct MBHead ));

	if(NetNameSize(uNet->Net,(char *)HEAD.name,&n))return 1;
	

	HEAD.nlen=(unsigned char)n;

	for(n=0;n<HEAD.nlen;++n)buff[n]=HEAD.name[n];

	buff[n]=0;

	cleanName(buff);
	
	if(!(out=fopen(buff,"wb"))){
	    sprintf(WarningBuff,"Could Not Open (%s) To Write\n",buff);
	    WarningBatch(WarningBuff);
	    return 1;
	}

	if(NetGet(uNet->Net,HEAD.creator,4L))goto ErrorOut;
	mstrncpy(buff,(char *)HEAD.creator,4);
	buff[4]=0;


	if(NetGet(uNet->Net,HEAD.type,4L))goto ErrorOut;
	mstrncpy(buff,(char *)HEAD.type,4);
	buff[4]=0;

	if(NetGet(uNet->Net,HEAD.cdate,4L))goto ErrorOut;
	mstrncpy(buff,(char *)HEAD.cdate,4);
	buff[4]=0;

	if(NetGet(uNet->Net,HEAD.mdate,4L))goto ErrorOut;
	mstrncpy(buff,(char *)HEAD.mdate,4);
	buff[4]=0;
	
	if(NetLong(uNet->Net,&Data))goto ErrorOut;


	HEAD.dflen[0]= (unsigned char)((Data >> 24) & 0xff);
	HEAD.dflen[1]= (unsigned char)((Data >> 16) & 0xff);
	HEAD.dflen[2]= (unsigned char)((Data >> 8) & 0xff);
	HEAD.dflen[3]= (unsigned char)(Data & 0xff);

	if(NetLong(uNet->Net,&Resource))goto ErrorOut;


	HEAD.rflen[0]= (unsigned char)((Resource >> 24) & 0xff);
	HEAD.rflen[1]= (unsigned char)((Resource >> 16) & 0xff);
	HEAD.rflen[2]= (unsigned char)((Resource >> 8) & 0xff);
	HEAD.rflen[3]= (unsigned char)(Resource & 0xff);

	
	bp=buff;
	bh=(char *)&HEAD;
	for(k=0;k<128;++k){
	   if(k < sizeof(HEAD)){
	       *bp++ = *bh++;
	   }else{
	       *bp++ = 0;
	   }
	}

	
	if(putString((unsigned char *)buff,128L,out))goto ErrorOut;
	
	
	if(SetBuffers(IO_BUFFER+512L))goto ErrorOut;

	k=Data;
	DataN=Data;
	while(Data > 0){
	    k=Data;
	    if(k > IO_BUFFER)k = IO_BUFFER;
	    if(NetGet(uNet->Net,xg.buffin,k))goto ErrorOut;
	    Data -= k;
	    if(out){
			if(putString(xg.buffin,k,out))goto ErrorOut;
	    }
	}

	n = (DataN+127)/128;

	k=128*n;

	n=k-DataN;

	while(n-- > 0)putc('\0',out);

	k=Resource;
	ResourceN=Resource;
	while(Resource > 0){
	    k=Resource;
	    if(k > IO_BUFFER)k = IO_BUFFER;
	    if(NetGet(uNet->Net,xg.buffin,k))goto ErrorOut;
	    Resource -= k;
	    if(out){
			if(putString(xg.buffin,k,out))goto ErrorOut;
	    }
	}

	n = (ResourceN+127)/128;

	k=128*n;

	n=k-ResourceN;

	while(n-- > 0)putc('\0',out);
	
	ret=0;
	
ErrorOut:	
	if(out)fclose(out);
	return ret;
}
int NetNameSize(struct NetWork *Net,char *name,long *nn)
{
	char namel[2048];
	int n;
	
	if(!Net || !name || !nn)return 1;
	
	if(NetByte(Net,&n))return 1;
	
	if(n > 0){
	    if(n > 62){
	        if(n > 2048){
	    		return 1;
	        }
		    if(NetGet(Net,(unsigned char *)namel,(long)n)){
		        return 1;
		    }
		    for(n=0;n<62;++n){
		        name[n]=namel[n];
		    }
	    	sprintf(WarningBuff,"(%s) Error File Name Shortened From (%ld) bytes\n",name,(long)n);
	    	WarningBatch(WarningBuff);
		    *nn=62;
	    }else{
		    if(NetGet(Net,(unsigned char *)name,(long)n)){
		        return 1;
		    }
	    }
	}else{
	    n=0;
	}	
	name[n]=0;
	*nn=n;
	return 0;
}
