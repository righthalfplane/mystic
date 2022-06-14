#define EXTERN22 extern
#include "Xappl.h"
#include "Control.h"
#include "fileDialog.h"
#include "xText.h"
#include "xStrings.h"
#include "Scanfile.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <netdb.h>
# include <signal.h> 
#include <string.h>
#include <arpa/inet.h>

#include "uFiles.h"

#include "Tools.h"


#define d_namlen d_reclen  

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

int writeLongT(int toServerSocket,long i);
int writeIntT(int toServerSocket,int i);
int getString(unsigned char *s,long n,FILE *in); 
int getInt(int *n,FILE *in);
int getLong(long *n,FILE *in);

int cleanName(char *buff);
int docd(char *nb,int flag);
int NotMyWindow(IconPtr myIcon,XEvent *event);

#ifndef SUN
#define d_namlen d_reclen
#endif

#include <string.h>

#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif
     int DFR8setpalette(char *pal);
     int DFR8restart(void);
     int DFR8getdims(char *name ,int *ixmax,int *iymax,int *hasPalette);
     int DFR8getimage(char *name,char *image,int ixmax,int iymax,char *pal);
	int DFR8lastref(void);
	int DFANgetlabel(char *file,unsigned int type,unsigned int tag,char *name,int length);
	int DFANputlabel(char *file,unsigned int type,unsigned int tag,char *name);
	int DFPrestart(void);
	int DFPgetpal(char *name,char *pal);
#ifdef __cplusplus
}
#endif


#define STRING_COUNT	6
#define TEXT_COUNT	6

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

struct tdata{
    controlPtr Control1;
    controlPtr Control2;
    controlPtr Control3;
    controlPtr RastorToMemory;
    controlPtr RastorToDisk;
    controlPtr AppendToMemory;
    controlPtr netErrorCheck;
    int Ok;
    int argc;
    char **argv;
    char *message;
    IconPtr myIconin;
    struct textData *Text[TEXT_COUNT];
    struct stringData *Strings[STRING_COUNT];
    uRect stringRect[STRING_COUNT];
    int textIn;
};


int copyl(char *p1,char *p2,long n);

int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize);
int GetTime(long *Seconds,long *milliseconds); 
int doSendPassword(int toServerSocket,char *SendPassword);
int domkdir(char *nb);

int createService(unsigned short *Port);
int CheckSocket(int serverSocket,int *count);
int readString(int clientSocket,char *buff,long n);
int readCommand(int clientSocket,char *buff,int *size);
int ListenFlush(int clientSocket,int size);
int getByte(int clientSocket,int *n);
int getname(int clientSocket,char *buff);
static int getLongSocket(int clientSocket,long *n);

IconPtr uStartStatusWindow(IconPtr myIcon,char *header);

int uWriteStatusWindow(IconPtr StatusWindow,char *buff);
int uCheckStatusWindow(IconPtr StatusWindow,char *Message);


int uStopStatusWindow(IconPtr StatusWindow);


int UnPack(register unsigned char *image,register unsigned char *buff,register long Length);
int connectToServer(char *serverName,unsigned short *Port);
int namesize(char *name);
int writeLab(int toServerSocket,char *Type,long size);
int writeName(int toServerSocket,char *name);
int writeInt(int toServerSocket,int i);
int writeLong(int toServerSocket,long i);
int writeString(int toServerSocket,char *buffout,long HdfSize);
int writeByte(int toServerSocket,int i);

char *strsave(char *,int tag);


static long Bytes;
static long ReadBytes;

int compar(struct dirent **a,struct dirent **b);

static IconPtr StatusWindow;

int crctab[] =				/* CRC lookup table */
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

static unsigned char *netbuff;
static long netsize=32768;
static long netsizeMax=32768;
static long netcount=0;
static long netblock=0;
static int netErrorCheck;

static int netFill(int clientSocket);
int netRead(int clientSocket,char *buff,long nbyte);
static int netCheck(int toServerSocket,char *m,long *block);
static int netGetInt(unsigned char *c,unsigned int *n);
static int netGetLong(unsigned char *c,long *n);
static unsigned int Crc(unsigned char *s,long int n);
static int netGetBlock(long *blength,int toServerSocket);
static int netSendBlock(char *m,long block,int clientSocket);
static int netPutLong(unsigned char *c,long i);
int netFlush(int clientSocket);
int CheckSocket(int serverSocket,int *count);
int netGetType2(int clientSocket,char *buff,long *size);
int netReadCheck(int clientSocket,char *buff,long nbyte);
int netWrite(int toServerSocket,char *buffout,long HdfSize);
int netWriteCheck(int toServerSocket,char *buffout,long HdfSize);
int NetDump2(int toServerSocket);
static int netPutInt(unsigned char *c,int i);




int sendclean(char *folder);
int ListenCheck(IconPtr myIcon);
int ListenAccept(IconPtr myIcon);

int netWriteCheck(int toServerSocket,char *buffout,long HdfSize)
{
	long n;

	if(!buffout || !netbuff)return 1;

	for(n=0;n<HdfSize;++n){
Start:
	    if(netcount < netsize){
	        netbuff[18+netcount++]=buffout[n];
	    }else{
	        if(NetDump2(toServerSocket))return 1;
	        goto Start;
	    }
	}
	return 0;
}
int NetDump2(int toServerSocket)
{
	unsigned int crcval;
	char message[5];
	long block;
	int cerror,berror;
/*
	static long err,save;
*/
	if(!netbuff)return 1;
	if(netcount <= 0)return 0;


	crcval=Crc(&netbuff[18],netcount);
	mstrncpy((char *)netbuff,"nt01data",9);
	netPutLong(&netbuff[8],netblock);
	netPutLong(&netbuff[12],netcount);
	netPutInt(&netbuff[16],crcval);

/*
	++err;

	if(!(err % 10)){
	    save=netbuff[9];
	    netbuff[9]=77;
	}
*/
	berror=0;
Start:
	if(netWrite(toServerSocket,(char *)netbuff,18+netcount))return 1;
/*
	if(!(err % 10) )netbuff[9]=save;
*/

	cerror=0;
Error1:


	if(netCheck(toServerSocket,message,&block)){
	    if(cerror < 3){
		netFlush(toServerSocket);
		netSendBlock("stat",netblock,toServerSocket);
	        fprintf(stderr,"NetDump2 cerror (%ld)\n",(long)++cerror);
	        goto Error1;
	    }else{
	        fprintf(stderr,"NetDump2 Giving up on cerror\n");
	        return 1;
	    }
	}

	if(berror++ > 3){
	    fprintf(stderr,"NetDump2 Giving up on berror\n");
	    return 1;
	}

	if(netblock != block){
	    fprintf(stderr,"NetDump2 Block Miss Match netblock %ld Block %ld\n",netblock,block);
	    goto Start;
	}

	if(!strcmp(message,"ok  ")){
	   ;
	}else if(!strcmp(message,"bad ")){
	    fprintf(stderr,"NetDump2 block Error Re-Send Block %ld\n",netblock);
	    goto Start;
	}else{
	    netFlush(toServerSocket);
	    netSendBlock("stat",netblock,toServerSocket);
	    fprintf(stderr,"NetDump2 read unknown Message %s netblock %ld\n",message,netblock);
	    goto Error1;
	}
	netcount=0;
	++netblock;
	return 0;
}
int writeString(int toServerSocket,char *buffout,long HdfSize)
{
	if(netErrorCheck){
	    return netWriteCheck(toServerSocket,buffout,HdfSize);
	}
	return netWrite(toServerSocket,buffout,HdfSize);
}
int netWrite(int toServerSocket,char *buffout,long HdfSize)
{
	char buff[256];
	long n;

	Bytes += HdfSize;
	while(HdfSize > 0){
	    n=HdfSize;
	    if(n > 120000)n=120000;
	    n=write(toServerSocket,buffout,n);
	    if(n < 0){
	        sprintf(WarningBuff,"Write Socket Error (%ld)\n",(long)errno);
	        WarningBatch(WarningBuff);
	        return 1;
	    }
	    buffout += n;
	    HdfSize -= n;
	    if(uCheckStatusWindow(StatusWindow,"Stop netWrite ?"))return 1;
	    if(HdfSize > 0){
	        sprintf(buff,"Bytes Left %ld in file",HdfSize);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop netWrite"))return 1;

	    }
	}
	return 0;
}
int sendclean(char *folder)
{
	if(!folder)return 1;

	while(*folder){
	    if(*folder == ':')*folder ='/';
	    ++folder;
	}
	return 0;
}
int compar(struct dirent **a,struct dirent **b)
{
	return strcmp((*a)->d_name,(*b)->d_name);
}

int doSendPassword(int toServerSocket,char *SendPassword)
{
	char *header;
	long size;

	header="PASS";

	size=1+namesize(SendPassword);

	if(writeLab(toServerSocket,header,size))return 1;

	if(writeName(toServerSocket,SendPassword))return 1;
	return 0;
}
int writeLab(int toServerSocket,char *Type,long size)
{
	if(writeString(toServerSocket,Type,4L))return 1;
	if(writeLong(toServerSocket,size))return 1;
	return 0;
}
int writeByte(int toServerSocket,int i)
{
      unsigned char c[2];
	extern int errno;

        c[0] = i & 255;
	if(writeString(toServerSocket,(char *)c,1))return 1;
	return 0;
}
int writeInt(int toServerSocket,int i)
{
	unsigned char c[2];
 
        c[0] = i & 255;
        c[1] = (i >> 8) & 255;
	if(writeString(toServerSocket,(char *)c,2))return 1;
	return 0;
}
int writeIntT(int toServerSocket,int i)
{
	unsigned char c[2];
 
        c[1] = i & 255;
        c[0] = (i >> 8) & 255;
	if(writeString(toServerSocket,(char *)c,2))return 1;
	return 0;
}
int writeLong(int toServerSocket,long i)
{
      unsigned char c[4];

        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);
	if(writeString(toServerSocket,(char *)c,4))return 1;
	return 0;
}
int writeLongT(int toServerSocket,long i)
{
      unsigned char c[4];

        c[3] = (unsigned char)(i & 255);
        c[2] = (unsigned char)((i >> 8) & 255);
        c[1] = (unsigned char)((i >> 16) & 255);
        c[0] = (unsigned char)((i >> 24) & 255);
	if(writeString(toServerSocket,(char *)c,4))return 1;
	return 0;
}
int writeName(int toServerSocket,char *name)
{
	char buff[256];
	char *np;
	int n;

	if(!(np=strrchr(name,'/'))){
	    np=name;
	}else{
	    np += 1;
	}

	mstrncpy(buff,np,255);

	
	n=namesize(buff);

	sendclean(buff);

	if(writeByte(toServerSocket,n))return 1;

	if(writeString(toServerSocket,buff,(long)n))return 1;
	return 0;
}
int namesize(char *name)
{
	char *np;
	unsigned long n;

	if(!(np=strrchr(name,'/'))){
	    np=name;
	}else{
	    np += 1;
	}

	n=strlen(np);

	return (int)n;
}
int connectToServer(char *serverName,unsigned short *Port)
{
	struct sockaddr_in serverSocketAddr;
	struct hostent *serverHostEnt;
	int toServerSocket,ret;
	unsigned long hostAddr;
	short result,Try;
	char *np;
	int buf_size;

	buf_size=32768;

	result= -1;

	zerol((char *)&serverSocketAddr,sizeof(serverSocketAddr));

	if(!(np=strrchr(serverName,':'))){
	    printf("Bad Address (%s)\n",serverName);
	    return result;
	}else{
	    *np=0;
	    np += 1;
	    *Port=(unsigned short)atol(np);
	}
	hostAddr=inet_addr(serverName);
	if((long)hostAddr != -1L){
	    serverSocketAddr.sin_addr.s_addr=(unsigned int)hostAddr;
	}else{
	    serverHostEnt=gethostbyname(serverName);
	    if(serverHostEnt == NULL){
	        fprintf(stderr,"Could Not Find Host (%s)\n",serverName);
	        return result;
	    }
	    copyl(serverHostEnt->h_addr,(char *)&serverSocketAddr.sin_addr,serverHostEnt->h_length);
	}
	serverSocketAddr.sin_family=AF_INET;
	serverSocketAddr.sin_port=htons(*Port);
	Try=0;
	while(Try++ < 10){
	    if((toServerSocket=socket(AF_INET,SOCK_STREAM,0)) < 0){
                fprintf(stderr,"socket Error  (%ld)\n",(long)errno);
	        return toServerSocket;
	    }

	ret=setsockopt( toServerSocket, SOL_SOCKET, SO_SNDBUF, 
                  (char *)&buf_size, sizeof(int) );    
        if(ret < 0)fprintf(stderr,"setsockopt SO_SNDBUF failed\n");

/*
         ret=setsockopt(toServerSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&d, sizeof d);
        if(ret < 0)printf("setsockopt TCP_NODELAY failed\n");
*/
	    ret=connect(toServerSocket,(struct sockaddr *)&serverSocketAddr,sizeof(serverSocketAddr));
	    if(ret == -1){
                if (errno == ECONNREFUSED)  {
                    fprintf(stderr,"Connection Refused  Try(%d)\n",Try);
                    close(toServerSocket);
                    sleep(2);
                    continue;
                }else{
                    fprintf(stderr,"Connection Error  (%ld)\n",(long)errno);
                    return ret;
                }
	    }
	    return toServerSocket;
	}
	
       return ret;
}
int createService(unsigned short *Port)
{
	struct sockaddr_in serverSocketAddr;
	int serverSocket;
	struct sockaddr_in name;
	unsigned int namelen;
/*
	int buf_size;
	int ret;

	buf_size=32768;
*/
	zerol((char *)&serverSocketAddr,sizeof(serverSocketAddr));
	serverSocketAddr.sin_port=htons((unsigned short)0);
	serverSocketAddr.sin_port=htons(*Port);
	serverSocketAddr.sin_family=AF_INET;
	serverSocketAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverSocket=socket(AF_INET,SOCK_STREAM,0);
/*
	ret=setsockopt( serverSocket, SOL_SOCKET, SO_RCVBUF, 
                  (char *)&buf_size, sizeof(int) );    
        if(ret < 0)printf("setsockopt failed\n");
 */
	
	bind(serverSocket,(struct  sockaddr  *)&serverSocketAddr,sizeof(serverSocketAddr));
	listen(serverSocket,5);
	namelen=sizeof(struct sockaddr_in);
	getsockname(serverSocket,(struct  sockaddr  *)&name,&namelen);
		
	*Port=ntohs(name.sin_port);
	return serverSocket;
	
}
int readString(int clientSocket,char *buff,long nbyte)
{

	if(netErrorCheck){
	    return netReadCheck(clientSocket,buff,nbyte);
	}
	return netRead(clientSocket,buff,nbyte);
}
int netReadCheck(int clientSocket,char *buff,long nbyte)
{
	long n;

	if(!netbuff)return 1;

	for(n=0;n<nbyte;++n){
Start:
	    if(netcount < netsize){
	        *buff++ = netbuff[netcount++];
	    }else{
	        if(netFill(clientSocket))return 1;
			if(!netbuff)return 1;
			goto Start;
	    }
	}
	return 0;
}
int netGetType2(int clientSocket,char *buff,long *size)
{
	unsigned char mess[8];

	if(netRead(clientSocket,(char *)mess,8L))return 1;
	buff[0]=mess[0];
	buff[1]=mess[1];
	buff[2]=mess[2];
	buff[3]=mess[3];
	buff[4]=0;
	netGetLong(&mess[4],size);
	return 0;
}
static int netFill(int clientSocket)
{
	char message[18];
	long block,blength;
	int berror,ret;

	block=0;
	berror=0;
Start:
	ret=netCheck(clientSocket,message,&block);
	if(berror++ > 10){
	    fprintf(stderr,"netFill Giving Up Too Many Errors\n");
	    return 1;
	}

	if(ret){
	    netFlush(clientSocket);
	    netSendBlock("bad ",netblock,clientSocket);
	    fprintf(stderr,"netFill block header Error\n");
	    goto Start;
	}


	if(!strcmp(message,"data")){
	    if(netGetBlock(&blength,clientSocket)){
	        netFlush(clientSocket);
	        netSendBlock("bad ",block,clientSocket);
	        goto Start;
	    }
	    if(block != netblock){
	        netFlush(clientSocket);
	        if(block < netblock){
	            netSendBlock("ok  ",block,clientSocket);
	        }else{
	            netSendBlock("bad ",netblock,clientSocket);
	        }
	        fprintf(stderr,"netFill Block Order Error - block %ld netblock %ld\n",block,netblock);
	        goto Start;
	    }
/*
	    {
	       static int err;
	       if(!(++err % 10)){
	           netSendBlock("ok12",block,clientSocket);
	       }else{
	           netSendBlock("ok  ",block,clientSocket);
	       }
	    }
*/	    
	    netSendBlock("ok  ",block,clientSocket);

	}else if(!strcmp(message,"stat")){
	    fprintf(stderr,"stat block %ld netblock %ld\n",block,netblock);
	    if(block == netblock){
	        netFlush(clientSocket);
	        netSendBlock("bad ",block,clientSocket);
	        goto Start;
	    }else if(block == netblock-1){
	        netFlush(clientSocket);
	        netSendBlock("ok  ",block,clientSocket);
	        goto Start;
	    }else{
	        netFlush(clientSocket);
	        netSendBlock("bad ",block,clientSocket);
	        fprintf(stderr,"netFill Block Order Error - block %ld netblock %ld\n",block,netblock);
	        goto Start;
	    }
	}else{
	    netFlush(clientSocket);
	    netSendBlock("bad ",block,clientSocket);
	    fprintf(stderr,"netFill read unknown Message %s block %ld berror %ld\n",message,block,(long)berror);
	    goto Start;
	}
	netcount=0;
	++netblock;
	return 0;
}
int netFlush(int clientSocket)
{
	int count,k;

Start:
	if(CheckSocket(clientSocket,&count)){
	    if(count <= 0)return 0;
	    fprintf(stderr,"netFlush skipping %d bytes\n",count);
	    do{
			k=count;
			if(k > netsizeMax)k = (int)netsizeMax;
			if(netRead(clientSocket,(char *)netbuff,k))return 1;
			count -= k;
	    }while(count > 0);
	    Sleep2(33);
	    goto Start;
	}
	return 0;
}
static int netCheck(int toServerSocket,char *m,long *block)
{
	unsigned char d[12];

	if(!m || !block)return 1;

	if(netRead(toServerSocket,(char *)d,12L))return 1;

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
static int netSendBlock(char *m,long block,int clientSocket)
{
	unsigned char message[14];

	mstrncpy((char *)message,"nt01",14);
	mstrncat((char *)message,m,14);
	netPutLong(&message[8],block);
	if(netWrite(clientSocket,(char *)message,12L))return 1;
	return 0;
}
static int netGetBlock(long *blength,int toServerSocket)
{
	unsigned int crcval;
	unsigned int check;
	unsigned char d[6];

	if(!blength)return 1;

	if(netRead(toServerSocket,(char *)d,6L))return 1;

	netGetLong(d,blength);

	netGetInt(&d[4],&check);

	if(*blength > netsizeMax){
	    if(*blength < 262144){
	        if(netbuff)cFree((char *)netbuff);
	        netbuff=(unsigned char *)cMalloc(*blength,8413);
	        if(!netbuff){
	            fprintf(stderr,"netGetBlock Out Of Memory - Requested %ld\n",*blength);
	            return 1;
	        }
		netsizeMax = *blength;
	    }else{
	        fprintf(stderr,"netGetBlock Error length %ld netsizeMax %ld\n",*blength,netsizeMax);
	        return 1;
	    }
	}

	netsize = *blength;

	if(netRead(toServerSocket,(char *)netbuff,netsize))return 1;

	crcval=Crc(netbuff,netsize);
	if(crcval != check){
	    fprintf(stderr,"Check Sum Error Length %ld crcval %x check %x\n",netsize,crcval,check);
	    return 1;
	}

	return 0;
}
static unsigned int Crc(unsigned char *s,long int n)
{
	unsigned int crcval,c;
	
	crcval=0;
	while(n-- > 0){
	    c=*s++;
	    crcval = (((crcval>>8)&0x00ff) ^ crctab[(crcval^(c))&0x00ff]);
	}
	
	return crcval;
}
static int netPutLong(unsigned char *c,long i)
{ 
	if(!c)return 1;

        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);

	return 0;
}
static int netPutInt(unsigned char *c,int i)
{
	if(!c)return 1;
 
        c[0] = i & 255;
        c[1] = (i >> 8) & 255;

	return 0;
}
static int netGetInt(unsigned char *c,unsigned int *n)
{ 
	unsigned long c1,c2;

	if(!c || !n)return 1;

	c2=c[0];
	c1=c[1];

        *n = (unsigned int)((c1 << 8) | c2);

	return 0;
}
static int netGetLong(unsigned char *c,long *n)
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
int netRead(int clientSocket,char *buff,long n)
{
	long k,count;
	char mes[256];

	count=k=n;
	while(k > 0){
	    n=k;
	    n=read(clientSocket,buff,n);
	    if(n > 0){
	        k -= n;
	        buff += n;
	    }else if(n == 0){
	         XEvent event;
		char text[10];
		KeySym mykey;
		int i;
	        if(XCheckMaskEvent(xg.mydisplay,ButtonPressMask | KeyPressMask,&event)){
		    i = XLookupString (&event.xkey,text, 10, &mykey, 0);
		    if (i == 1 && text[0] == 'q'){
		        return 1;
		    }else{
			sprintf(mes,
			"Hit 'q' to quit trying to read %ld bytes",k);
		        Warning(mes);
		    }
	        }else{
	            Sleep2(50);
	        }
	    }else{
	        fprintf(stderr,"Error Reading readString\n");
	        return 1;
	    }
	}
	ReadBytes += count;
	return 0;
}
int docd(char *nb,int flag)
{
	int compar();
	char *dirname,out[256];
	struct stat buf;


	if(!nb || !(*nb)){
	    dirname=".";
	}else{
	    dirname=nb;
	    if(stat(dirname,&buf)){
	        ;
	    }else{
	        if(!(buf.st_mode & S_IFDIR)){
	            if(flag)return 1;
	            sprintf(out,"%s not a directory",dirname);
	            Warning(out);
	            return 1;		   
	        }
	    }
	}
	if(chdir(dirname)){
	     if(flag)return 1;
	     sprintf(out,"docd Could Not Change to Directory %s",dirname);
	     Warning(out);
	     return 1;		   
	}
	return 0;
}
int cleanName(char *buff)
{
	char *bp;

	if(!buff)return 1;
	bp=buff;
	while(*bp){
	    if(*bp == '/')*bp = ':';
	    ++bp;
	}
	return 0;
}
int getname(int clientSocket,char *buff)
{
	int n;

	if(getByte(clientSocket,&n))return 1;
	if(readString(clientSocket,buff,(long)n)){
	    fprintf(stderr,"getname return error\n");
	    return 1;
	}
	buff[n]=0;
	return 0;
}
int getByte(int clientSocket,int *n)
{
	unsigned char buff[1];

	if(readString(clientSocket,(char *)buff,1))return 1;

	*n=buff[0];

	return 0;
}
static int getLongSocket(int clientSocket,long *n)
{
	unsigned char c[4];

	if(readString(clientSocket,(char *)c,4))return 1;

	*n = c[0] + ((long)c[1] << 8) 
	          + ((long)c[2] << 16) + ((long)c[3] << 24);

	return 0;
}
int ListenFlush(int clientSocket,int size)
{
	long n;

	while(size > 0){
	    n=size;
	    if(n > xg.buffsize)n=xg.buffsize;
	    if(readString(clientSocket,(char *)xg.buffin,n))return 1;
	    size -= n;
	}
	return 0;
}
int readCommand(int clientSocket,char *buff,int *size)
{
	int n;
	long Length;

	n=4;
	if(readString(clientSocket,buff,n))return 1;
	buff[n]=0;
	if(getLongSocket(clientSocket,&Length))return 1;
	*size=(int)Length;
	return 0;
	
}
int copyl(char *p1,char *p2,long n)
{
	if(!p1 || !p2)return 1;

	while(n-- > 0)*p2++ = *p1++;

	return 0;
}
/*
int HdfPackOLD(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize)
{
	unsigned char *nn,*pi,*End;
	long n,Last;
	unsigned int c;
	int max,nr,nc,flag;

	if(!image || !buff || !HdfSize)return 1;
	
	Last=ImageSize-1;
	if(image[Last] == image[Last-1]){
	    image[Last] += 1;
	    flag=TRUE;
	}else{
	    flag=FALSE;
	}
	
	n=0;
	nc=0;
	nn=buff++;
	max=0x7f;
	End = image + ImageSize;
	while(image < End){
	    c = *image;
	    pi = ++image;
	    nr=1;
	    while(nr < max){
	        if(c != *pi++)break;
	        ++nr;
	    }
	    if(nr > 2){
	        if(nc > 0){
	            *nn=nc;
	    	    nn=buff++;
	            n += (nc+1);
	            nc = 0;
	        }
	        image += (nr-1);
	        *nn = nr | 0x80;
	        *buff++ = c;
	    	nn=buff++;
	        n += 2;
	    }else{
	        *buff++ = c;
	        if(++nc >= 0x7f){
	            *nn=nc;
	    	    nn=buff++;
	            n += (nc+1);
	            nc = 0;
	        }
	    }

	}
	if(nc > 0){
	    *nn=nc;
	    n += (nc+1);
	}
	*HdfSize=n;
	if(flag && n){
	    *(buff-1) -= 1;
	}
	return 0;
}
*/
