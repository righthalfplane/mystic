#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "StatusWindow.h"

#include "Paux.h"
#include "Net.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int appendTek(IconPtr myIcon,char *name,long Length,char *data);

int appendRastor(IconPtr myIcon,char *name,int ixmax,int iymax,long Length,char *pal);

IconPtr NewRastorType(long count);

int UnPack(register unsigned char *image,register unsigned char *buff,register long Length);
IconPtr openTrue(struct FileInfo2 *Files);

#define IN_BUFF_SIZE 32000
int netGetBlock(long *blength,struct NetWork *net);
int netGetInt(unsigned char *c,unsigned int *n);
extern int crctab[];
unsigned int Crc(unsigned char *s,long int n);
int netSendBlock(char *m,long block,struct NetWork *net);
int netFlush(struct NetWork *net);
int netCheck(struct NetWork *net,char *m,long *block);
int netReadCheck(struct NetWork *net,char *buff,long nbyte);
int netFill(struct NetWork *net);
int readString(struct NetWork *net,char *buff,long nbyte);
int getnamesize(struct NetWork *net,char *buff,long *nn);
int NetRead(struct NetWork *net,char *buff,long n);
int netGetType(struct NetWork *net,char *buff,long *size);
int readCommand(struct NetWork *net,char *buff,long *size);
int getname(struct NetWork *net,char *buff);
int getNetLong(struct NetWork *net,long *n);
int getByte(struct NetWork *net,int *n);
int netGetLong(unsigned char *c,long *n);

int cleanName(char *buff);
int docd(struct NetWork *net,char *nb,int flag);
int domkdir(char *nb);

int WriteStatus(IconPtr myIcon,char *buff);

static int getLongSocket(struct NetWork *net,long *n);
static int getIntSocket(struct NetWork *net,int *n);
static int getLongSocketC(struct NetWork *net,long *n);
static int getIntSocketC(struct NetWork *net,int *n);


IconPtr NewTekType(struct FileInfo2 *files);



int NetStartListen(struct NetWork *net,int netCheck)
{
	char buff[256];

	net->netsizeMax=32768;

	if(!net->netbuff){
		net->netbuff=(unsigned char *)cMalloc(net->netsizeMax+128,9115);
		if(!net->netbuff){
			sprintf(buff,"Error Could Not Allocate %ld Bytes For Network Buffer",net->netsize);
			Warning(buff);
			return 1;
		}
	}
	net->netsize=net->netsizeMax;
	net->netcount=net->netsizeMax;
	net->netblock=0;
	net->WriteBytes=0;
	net->ReadBytes=0;
	net->netErrorCheck=netCheck;

	if(net->passname && net->passname[0]){
		net->passwd=1;
	}else{
		net->passwd=0;
	}


	return 0;

}

int NetStop(struct NetWork *net)
{
	if(!net)return 1;

	if(net->netbuff){
		cFree((char *)net->netbuff);
		net->netbuff=NULL;
	}

	cFree((char *)net);
	return 0;
}
int domkdir(char *nb)
{
    int ret;

    ret = mkdir(nb);

	return ret;
}
int docd(struct NetWork *net,char *nb,int flag)
{
	char *dirname,out[256];

	if(!nb || !(*nb)){
	    dirname=".";
	}else{
	    dirname=nb;
	}

	if(chdir(dirname)){
		if(flag)return 1;
		sprintf(out,"Could Not Change to Directory %s",dirname);
		WriteStatus(net->win,out);
		return 1;		   
	}
	return 0;
}
int cleanName(char *buff)
{
	char *bp;
	int c;

	if(!buff)return 1;

	bp=buff;
	while(*bp){
		c = *bp;
	    if(c == '/' || c == '<' || c == '>' ||
		   c == ':' || c == '"' || c == '?' ||
		   c == '|' || c == '\\' || c < 32 || c > 125)c = ' ';
		*bp++ = c;
	}
	return 0;
}
int netGetType(struct NetWork *net,char *buff,long *size)
{
	unsigned char mess[8];

	if(NetRead(net,(char *)mess,8L))return 1;
	buff[0]=mess[0];
	buff[1]=mess[1];
	buff[2]=mess[2];
	buff[3]=mess[3];
	buff[4]=0;
	netGetLong(&mess[4],size);
	return 0;
}
int readCommand(struct NetWork *net,char *buff,long *size)
{
	long n;

	n=4;
	if(readString(net,buff,n))return 1;
	buff[n]=0;
	if(getNetLong(net,size))return 1;
	return 0;
	
}
int getname(struct NetWork *net,char *buff)
{
	int n;

	if(getByte(net,&n))return 1;
	if(readString(net,buff,(long)n)){
		WriteStatus(net->win,"getname error readString");
	    return 1;
	}
	buff[n]=0;
	return 0;
}
int getNetLong(struct NetWork *net,long *n)
{
	unsigned char c[4];

	if(readString(net,(char *)c,4L))return 1;

	*n = c[0] + ((long)c[1] << 8) 
	          + ((long)c[2] << 16) + ((long)c[3] << 24);

	return 0;
}
int getnamesize(struct NetWork *net,char *buff,long *nn)
{
	int n;

	if(getByte(net,&n))return 1;
	if(readString(net,buff,(long)n)){
		WriteStatus(net->win,"getnamesize error readString");
	    return 1;
	}
	buff[n]=0;
	*nn=n;
	return 0;
}

int getByte(struct NetWork *net,int *n)
{
	unsigned char buff[1];

	if(readString(net,(char *)buff,1L))return 1;

	*n=buff[0];

	return 0;
}
int readString(struct NetWork *net,char *buff,long nbyte)
{

	if(net->netErrorCheck){
	    return netReadCheck(net,buff,nbyte);
	}
	return NetRead(net,buff,nbyte);	
}
int netReadCheck(struct NetWork *net,char *buff,long nbyte)
{
	long n;

	if(!net || !net->netbuff)return 1;

	for(n=0;n<nbyte;++n){
Start:
	    if(net->netcount < net->netsize){
	        *buff++ = net->netbuff[net->netcount++];
	    }else{
	        if(netFill(net))return 1;
			if(!net->netbuff)return 1;
			goto Start;
	    }
	}
	return 0;
}
int netFill(struct NetWork *net)
{
	char buff[256];
	char message[18];
	long block,blength;
	int berror,ret;

	berror=0;
Start:
	ret=netCheck(net,message,&block);
	if(berror++ > 10){
	    sprintf(buff,"netFill Giving Up Too Many Errors\n");
		WriteStatus(net->win,buff);
	    return 1;
	}

	if(ret){
	    netFlush(net);
	    netSendBlock("bad ",net->netblock,net);
	    sprintf(buff,"netFill block header Error\n");
		WriteStatus(net->win,buff);
	    goto Start;
	}


	if(!strcmp(message,"data")){
	    if(netGetBlock(&blength,net)){
	        netFlush(net);
	        netSendBlock("bad ",block,net);
	        goto Start;
	    }
	    if(block != net->netblock){
	        netFlush(net);
	        if(block < net->netblock){
	            netSendBlock("ok  ",block,net);
	        }else{
	            netSendBlock("bad ",net->netblock,net);
	        }
	        sprintf(buff,"netFill Block Order Error - block %ld netblock %ld\n",block,net->netblock);
			WriteStatus(net->win,buff);
	        goto Start;
	    }
	    netSendBlock("ok  ",block,net);

	}else if(!strcmp(message,"stat")){
	    sprintf(buff,"stat block %ld netblock %ld\n",block,net->netblock);
		WriteStatus(net->win,buff);
	    if(block == net->netblock){
	        netFlush(net);
	        netSendBlock("bad ",block,net);
	        goto Start;
	    }else if(block == net->netblock-1){
	        netFlush(net);
	        netSendBlock("ok  ",block,net);
	        goto Start;
	    }else{
	        netFlush(net);
	        netSendBlock("bad ",block,net);
	        sprintf(buff,"netFill Block Order Error - block %ld netblock %ld\n",block,net->netblock);
			WriteStatus(net->win,buff);
	        goto Start;
	    }
	}else{
	    netFlush(net);
	    netSendBlock("bad ",block,net);
	    sprintf(buff,"netFill read unknown Message %s block %ld berror %ld\n",message,block,(long)berror);
		WriteStatus(net->win,buff);
	    goto Start;
	}
	net->netcount=0;
	++net->netblock;
	return 0;
}
int netGetBlock(long *blength,struct NetWork *net)
{
	char buff[256];
	unsigned int crcval;
	unsigned int check;
	unsigned char d[6];

	if(!blength)return 1;

	if(NetRead(net,(char *)d,6L))return 1;

	netGetLong(d,blength);

	netGetInt(&d[4],&check);

	if(*blength > net->netsizeMax){
	    if(*blength < 262144){
	        if(net->netbuff)free(net->netbuff);
	        net->netbuff=(unsigned char *)malloc(*blength);
	        if(!net->netbuff){
	            sprintf(buff,"netGetBlock Out Of Memory - Requested %ld",*blength);
				WriteStatus(net->win,buff);
	            return 1;
	        }
		net->netsizeMax = *blength;
	    }else{
	        sprintf(buff,"netGetBlock Error length %ld netsizeMax %ld",*blength,net->netsizeMax);
			WriteStatus(net->win,buff);
	        return 1;
	    }
	}

	net->netsize = *blength;

	if(NetRead(net,(char *)net->netbuff,net->netsize))return 1;

	crcval=Crc(net->netbuff,net->netsize);
	if(crcval != check){
	    sprintf(buff,"Check Sum Error Length %ld crcval %x check %x",net->netsize,crcval,check);
		WriteStatus(net->win,buff);
	    return 1;
	}

	return 0;
}
int netGetInt(unsigned char *c,unsigned int *n)
{ 
	unsigned long c1,c2;

	if(!c || !n)return 1;

	c1=c[0];
	c2=c[1];

        *n = (unsigned int)((c2 << 8) | c1);

	return 0;
}
static int getIntSocket(struct NetWork *net,int *n)
{
	unsigned char c[2];

	if(readString(net,(char *)c,2))return 1;

	*n = c[0] + ((unsigned)c[1] << 8);

	return 0;
}
static int getIntSocketC(struct NetWork *net,int *n)
{
	unsigned char c[2];

	if(readString(net,(char *)c,2))return 1;

	*n = c[1] + ((unsigned)c[0] << 8);

	return 0;
}
static int getLongSocket(struct NetWork *net,long *n)
{
	unsigned char c[4];

	if(readString(net,(char *)c,4))return 1;

	*n = c[0] + ((long)c[1] << 8) 
	          + ((long)c[2] << 16) + ((long)c[3] << 24);

	return 0;
}
static int getLongSocketC(struct NetWork *net,long *n)
{
	unsigned char c[4];

	if(readString(net,(char *)c,4))return 1;

	*n = c[3] + ((long)c[2] << 8) 
	          + ((long)c[1] << 16) + ((long)c[0] << 24);

	return 0;
}
