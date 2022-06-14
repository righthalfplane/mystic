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
int writeLab(int toServerSocket,char *Type,long size);
int writeString(int toServerSocket,char *buffout,long HdfSize);
int writeByte(int toServerSocket,int i);
int writeLong(int toServerSocket,long i);
int writeInt(int toServerSocket,int i);

int NetDump2(int toServerSocket);
int connectToServer(char *serverName,unsigned short *Port);

int doSendPassword(int toServerSocket,char *SendPassword);

int uSendPassword(struct NetWork *Net,char *SendPassword);

int uSendPassword(struct NetWork *Net,char *SendPassword)
{
	if(!Net || !SendPassword)return 1;
	
	return doSendPassword(Net->toServerSocket,SendPassword);
}



long NetOpenWrite(struct NetWork *Net, char *remoteHost,int remotePort)
{
	char name[256];
	int toServerSocket;
	unsigned short Port;
	
	if(!Net || !remoteHost)return 1;
	//remotePort=remotePort;
	
	Port=remotePort;
	sprintf(name,"%s:%d\n",remoteHost,Port);	
	toServerSocket=connectToServer(name,&Port);
    if(toServerSocket == -1){
        sprintf(WarningBuff,"connect failed %s:%d",remoteHost,Port);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	if(Port != remotePort){
        sprintf(WarningBuff,"connect Port changed to %d",Port);
	    WarningBatch(WarningBuff);
	}
	
	Net->toServerSocket=toServerSocket;
		
	return 0;
}

int NetDump(struct NetWork *Net)
{
	if(!Net)return 1;
	return NetDump2(Net->toServerSocket);
}

int SendLong(struct NetWork *Net,unsigned long n)
{
	if(!Net)return 1;
	return writeLong(Net->toServerSocket,(long)n);
}
int SendInt(struct NetWork *Net,int n)
{
	if(!Net)return 1;
	return writeInt(Net->toServerSocket,n);
}
int SendByte(struct NetWork *Net,unsigned int n)
{
	if(!Net)return 1;
	return writeByte(Net->toServerSocket,n);
}
int SendData(struct NetWork *Net,char *message,long n)
{
	if(!Net && !message)return 1;
	return writeString(Net->toServerSocket,message,n);
}
int SendLab(struct NetWork *Net,char *header,long size)
{
	if(!Net && !header)return 1;
	return writeLab(Net->toServerSocket,header,size);
}
int NetName(struct NetWork *Net,char *name)
{
	int n;
	
	if(NetByte(Net,&n))return 1;
	
	if(n > 0){
	    if(NetGet(Net,(unsigned char *)name,(long)n)){
	        return 1;
	    }
	}else{
	    n=0;
	}	
	if(n > 31)n=31;
	name[n]=0;
	return 0;
}
int NetGet(struct NetWork *Net,unsigned char *buff,long count)
{
	
	if(Net->netErrorCheck){
	    return NetReadCheck(Net,(unsigned char *)buff,count);
	}else{
	    return NetRead(Net,(unsigned char *)buff,count);
	}
}
int NetTag(struct NetWork *Net)
{
	int n1,n2,n3,n4;
	
	if(NetByte(Net,&n1))return 1;
	if(NetByte(Net,&n2))return 1;
	if(NetByte(Net,&n3))return 1;
	if(NetByte(Net,&n4))return 1;

	if(n1 != 'D')return 1;
	if(n2 != 'R')return 1;
	if(n3 != 't')return 1;
	if(n4 != '1')return 1;
	return 0;
}
int NetIntr(struct NetWork *Net,int *xsize)
{
	unsigned char buff[2];
	
	if(NetGet(Net,buff,2L))return 1;
	*xsize = buff[1] + (((unsigned)buff[0]) << 8);
	return 0;
}
int NetLongr(struct NetWork *Net,long *xsize)
{
	unsigned char buff[4];
	
	if(NetGet(Net,buff,4L))return 1;
	*xsize = (long)( buff[3] + ((( unsigned long)buff[2]) << 8) + 
	         (((unsigned long)buff[1]) << 16) + (((unsigned long)buff[0]) << 24) );
	return 0;
}
