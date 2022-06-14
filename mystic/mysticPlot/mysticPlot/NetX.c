#define EXTERN22 extern
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "Xappl.h"
#include "uLib.h"
#include "uFiles.h"
#include "paletteDraw.h"
#include "Linedata.h"
#include "Message.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Tools.h"
#include "Raster2D.h"


static int crctab[] =				/* CRC lookup table */
{	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

int NetListenAccept(struct NetWork *Net);

int NetReadU(struct NetWork *Net,unsigned char *buff,int *count);

int netGetBlock(long *blength,struct NetWork *Net);
int netFill(struct NetWork *Net);
int NetReadCheck(struct NetWork *Net,unsigned char *buff,long nbyte);
int NetWriteCheck(struct NetWork *Net);

int netPutLong(unsigned char *c,long i);
int netGetInt(unsigned char *c,unsigned int *n);
int netGetLong(unsigned char *c,long *n);
unsigned int Crc(unsigned char *s,long int n);
int netCheck(struct NetWork *Net,char *m,long *block);
int netSendBlock(char *m,long block,struct NetWork *Net);
int netPutInt(unsigned char *c,int i);
int netFlush(struct NetWork *Net);


struct NetWork *NetStart(IconPtr myIcon)
{
	struct NetWork *Net;
	char nameHost[256];
	struct hostent *host;
	struct sockaddr_in destAddr; 

	Net=(struct NetWork *)cMalloc(sizeof(struct NetWork),8176);
	if(Net == NULL){
	    WarningBatch("NetStart Memory");
	    goto ErrorOut;
	}
	zerol((char *)Net,sizeof(*Net));

	Net->myIcon=myIcon;

	nameHost[0]=0;

	gethostname(nameHost,256);
	
	host=gethostbyname(nameHost);
	
	if(host == NULL){
		fprintf(stderr,"nameHost '%s' host == NULL\n",nameHost);
		Net->ipNum=0;
		return Net;
		
	}
	
	memcpy(&(destAddr.sin_addr.s_addr), host->h_addr, host->h_length); 
		
	Net->ipNum=ntohl(destAddr.sin_addr.s_addr);

	return Net;
ErrorOut:
	if(Net)cFree((char *)Net);
	return NULL;

}

int NetWrite(struct NetWork *Net,unsigned char *buffout,long length)
{
	char buff[256];
	long n;
	
	if(!Net || !buffout)return 1;
	
	Net->BytesWrite += length;
	
	while(length > 0){
	    n=length;
	    if(n > 120000)n=120000;
	    n=write(Net->clientSocket,buffout,n);
	    if(n < 0){
	        sprintf(WarningBuff,"Write Socket Error (%ld)\n",(long)errno);
	        WarningBatch(WarningBuff);
	        return 1;
	    }
	    buffout += n;
	    length -= n;
	    /* if(uCheckStatusWindow(StatusWindow,"Stop netWrite ?"))return 1; */
	    if(length > 0){
	        sprintf(buff,"Bytes Left %ld in file",length);
	        /* if(uWriteStatusWindow(StatusWindow,buff))return 1; */
	    }
	}
	return 0;
}


int NetRead(struct NetWork *Net,unsigned char *buff,long count)
{
	int end;
		
	while(count > 0){
	   end=(int)count;
	   if(NetReadU(Net,buff,&end)){
	       return 1;
	   }
	   buff += end;
	   count -= end;	   	    
	}
	return 0;
}
int NetReadU(struct NetWork *Net,unsigned char *buff,int *count)
{
	int n;
	
	if(!Net || !buff || !count)return 1;
	
	
	if(!CheckSocket(Net->clientSocket,&n)){
	   *count=0;
	   return 0;
	}
	
	if(n <= 0)return 0;
	
	n = *count;
	
    n=(int)read(Net->clientSocket,buff,n);
    if(n > 0){
        *count = n;
    }else if(n == 0){
        *count = 0;
    }else{
        sprintf(WarningBuff,"NetReadU Socket Read Error\n");
        WarningBatch(WarningBuff);
        return 1;
    }
	
	
	return 0;
}

int NetClose(struct NetWork *Net)
{
	if(!Net)return 1;
	
	if(Net->ConnectedFlag == TRUE){
	    if(Net->serverSocket > 0){
			shutdown(Net->serverSocket,2);
			close(Net->serverSocket);    
			Net->serverSocket=0;   
		}      
	    Net->ConnectedFlag = FALSE;
	}
	
    if(Net->toServerSocket > 0){
		shutdown(Net->toServerSocket,2);
		close(Net->toServerSocket);    
		Net->toServerSocket=0;   
	}      
	
	return 0;
}
int NetListenCheck(struct NetWork *Net)
{
	int count;
	
	if(!Net)return 1;
	
	if(CheckSocket(Net->serverSocket,&count)){
	     return NetListenAccept(Net);
	}
	
	return 1;
	
}
int NetListenAccept(struct NetWork *Net)
{
	struct sockaddr_in clientSocketAddr;
	unsigned long hostAddr;
	unsigned int addrLen;
	
	if(!Net)return 1;	
	if(!Net->serverSocket)return 1;
	
	addrLen=sizeof(clientSocketAddr);
	Net->clientSocket=accept(Net->serverSocket,
	                        (struct  sockaddr  *)&clientSocketAddr,
	                        &addrLen);
	if(Net->clientSocket < 0)return 1;
	hostAddr=clientSocketAddr.sin_addr.s_addr;
	hostAddr=htonl(hostAddr);
	sprintf(Net->ReceiveName,"%d.%d.%d.%d",
			(unsigned int)((hostAddr >> 24) & 0xff),
			(unsigned int)((hostAddr >> 16) & 0xff),
			(unsigned int)((hostAddr >> 8) & 0xff),
			(unsigned int)((hostAddr ) & 0xff));
	Net->ConnectedFlag=TRUE;
	return 0;
}

int NetListenStart(struct NetWork *Net,int *localPort)
{
	unsigned short Port;
	

	
	if(!Net || !localPort)return 1;
	
	NetListenStop(Net);
	
	sleep(1);
		
	Port = *localPort;
	
	Net->serverSocket=createService(&Port);

    if(Net->serverSocket == -1){
	     sprintf(WarningBuff,"Create Port %d Failed\n",Port);
	     WarningBatch(WarningBuff);
	     return 1;
	}

	if(Port != *localPort){
	     sprintf(WarningBuff,"ListenPort changed to Port %d\n",Port);
	     WarningBatch(WarningBuff);
	}
	Net->Port=Port;
	
	*localPort=Port;
		
	return 0;
}
int NetListenStop(struct NetWork *Net)
{
	if(!Net)return 1;
	
	if(Net->clientSocket > 0){
	    shutdown(Net->clientSocket,2);
	    close(Net->clientSocket);
	}
	Net->clientSocket=0;
	
	if(Net->serverSocket > 0){
	    shutdown(Net->serverSocket,2);
	    close(Net->serverSocket);
	}
	Net->serverSocket=0;
	return 0;
}
int NetStop(struct NetWork *Net)
{
	if(!Net)return 1;
	
	NetClose(Net);
		
	if(Net)cFree((char *)Net);
	
	return 0;
}

int NetStringToAddr(char *out5,unsigned long *ipNum)
{
	if(!out5 || !ipNum)return 1;
	return 0;
}

int netSendBlock(char *m,long block,struct NetWork *Net)
{
	unsigned char message[14];

	mstrncpy((char *)message,"nt01",14);
	mstrncat((char *)message,m,14);
	netPutLong(&message[8],block);
	if(NetWrite(Net,message,12L))return 1;
	return 0;
}
int netCheck(struct NetWork *Net,char *m,long *block)
{
	unsigned char d[12];

	if(!m || !block)return 1;

	if(NetRead(Net,d,12L))return 1;

	if((d[0] == 'n') && (d[1] == 't') && (d[2] == '0') && (d[3] == '1')){
	    m[0]=d[4];
	    m[1]=d[5];
	    m[2]=d[6];
	    m[3]=d[7];
	    m[4]=0;
	    netGetLong(&d[8],block);
	}else{
	    return 1;
	}
	return 0;
}
unsigned int Crc(unsigned char *s,long int n)
{
	unsigned int crcval,c;
	
	crcval=0;
	while(n-- > 0){
	    c=*s++;
	    crcval = (((crcval>>8)&0x00ff) ^ crctab[(crcval^(c))&0x00ff]);
	}
	
	return crcval;
}
int netGetLong(unsigned char *c,long *n)
{ 
	unsigned long c1,c2,c3,c4;

	if(!c || !n)return 1;

	c4=c[0];
	c3=c[1];
	c2=c[2];
	c1=c[3];

        *n = (long)((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);

	return 0;
}

int netGetInt(unsigned char *c,unsigned int *n)
{ 
	unsigned long c1,c2;

	if(!c || !n)return 1;

	c2=c[0];
	c1=c[1];

        *n = (unsigned int)((c1 << 8) | c2);

	return 0;
}
int netPutLong(unsigned char *c,long i)
{ 
	if(!c)return 1;

        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);

	return 0;
}
int netPutInt(unsigned char *c,int i)
{
	if(!c)return 1;
 
        c[0] = i & 255;
        c[1] = (i >> 8) & 255;

	return 0;
}
int NetWriteCheck(struct NetWork *Net)
{
	    unsigned int crcval;
	    unsigned char *netbuff;
	    long netblock;
	    long netcount;
		char message[5];
		long block;
		int cerror,berror;
	    int  ret;
	    
	    ret=0;
	    if(!Net)return -1;
	    if(Net->bufferCount <= 0)return ret;
	    netbuff = &Net->bufferSB[0];
	    netcount = Net->bufferCount;
	    netblock = Net->netblock;
	    
	    crcval=Crc(&netbuff[18],netcount);
	    mstrncpy((char *)netbuff,"nt01data",9);
	    netPutLong(&netbuff[8],netblock);
	    netPutLong(&netbuff[12],netcount);
	    netPutInt(&netbuff[16],crcval);
	    
		berror=0;
Start:
		if(NetWrite(Net,netbuff,18+netcount))return -1;
		cerror=0;
Error1:
		if(netCheck(Net,message,&block)){
	    	if(cerror < 3){
		    	netFlush(Net);
		    	netSendBlock("stat",netblock,Net);
		    	++cerror;
	        	goto Error1;
	    	}else{
	       	 return 1;
	    	}
		}	    
		if(berror++ > 3){
	    	return 1;
		}

		if(netblock != block){
	    	goto Start;
		}
		if(!strcmp(message,"ok  ")){
	   		;
		}else if(!strcmp(message,"bad ")){
	    	goto Start;
		}else{
	    	netFlush(Net);
	    	netSendBlock("stat",netblock,Net);
	    	goto Error1;
		}
	    Net->bufferCount = 0;
	    Net->netblock = ++netblock;
		return ret;
}
int NetReadCheck(struct NetWork *Net,unsigned char *buff,long nbyte)
{
	long n;

	if(!Net || !buff)return 1;

	for(n=0;n<nbyte;++n){
Start:
	    if(Net->netcount < Net->netsize){
	        *buff++ = Net->bufferR[Net->netcount++];
	    }else{
	        if(netFill(Net))return 1;
		    goto Start;
	    }
	}
	return 0;
}
int netFill(struct NetWork *Net)
{
	char message[18];
	long block,blength;
	int berror,ret;
	long netblock;

	berror=0;
	block=0;
	netblock=Net->netblock;	
Start:
	ret=netCheck(Net,message,&block);
	if(berror++ > 10){
	    return 1;
	}

	if(ret){
	    netFlush(Net);
	    netSendBlock("bad ",netblock,Net);
	    goto Start;
	}


	if(!strcmp(message,"data")){
	    if(netGetBlock(&blength,Net)){
	        netFlush(Net);
	        netSendBlock("bad ",block,Net);
	        goto Start;
	    }
	    if(block != netblock){
	        netFlush(Net);
	        if(block < netblock){
	            netSendBlock("ok  ",block,Net);
	        }else{
	            netSendBlock("bad ",netblock,Net);
	        }
	        goto Start;
	    }
	    netSendBlock("ok  ",block,Net);

	}else if(!strcmp(message,"stat")){
	    if(block == netblock){
	        netFlush(Net);
	        netSendBlock("bad ",block,Net);
	        goto Start;
	    }else if(block == netblock-1){
	        netFlush(Net);
	        netSendBlock("ok  ",block,Net);
	        goto Start;
	    }else{
	        netFlush(Net);
	        netSendBlock("bad ",block,Net);
	        goto Start;
	    }
	}else{
	    netFlush(Net);
	    netSendBlock("bad ",block,Net);
	    goto Start;
	}
	Net->netcount=0;	
	++Net->netblock;
	return 0;
}
int netGetBlock(long *blength,struct NetWork *Net)
{
	unsigned int crcval;
	unsigned int check;
	unsigned char d[6];

	if(!blength)return 1;

	if(NetRead(Net,d,6L))return 1;

	netGetLong(d,blength);

	netGetInt(&d[4],&check);

	if(*blength > Net->netsizeMax){
	    WarningBatch("netGetBlock Block Too large");
	    sprintf(WarningBuff,"netGetBlock Block Too large %ld %ld",(long)*blength,(long)Net->netsizeMax);
	    WarningBatch(WarningBuff);
	    return 1;
	}

	Net->netsize = *blength;

	if(NetRead(Net,Net->bufferR,Net->netsize))return 1;

	crcval=Crc(Net->bufferR,Net->netsize);
	if(crcval != check){
	    sprintf(WarningBuff,"Check Sum Error Length %ld crcval %x check %x",Net->netsize,crcval,check);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	return 0;
}
int netGetType(struct NetWork *data,char *buff,long *size)
{
	unsigned char mess[8];

	if(NetRead(data,mess,8L))return 1;
	buff[0]=mess[0];
	buff[1]=mess[1];
	buff[2]=mess[2];
	buff[3]=mess[3];
	buff[4]=0;
	netGetLong(&mess[4],size);
	return 0;
}

