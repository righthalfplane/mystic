#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/param.h>
# include <sys/signal.h> 
#include <dirent.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
/* #include <malloc.h> */
#include <sys/ioctl.h>

#include <zlib.h>

#include "ulibTypes.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif
     char *getwd(char *pathname);  
     int socket(int domain,int  type,int  protocol);
     int shutdown(int s,int  how);
/*
     int listen(int s,int backlog);
     int ioctl(int d, int request, ...);
     long atol(char *str);
     int select (int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
     int connect(int s, const struct sockaddr *name, int namelen);
     int chdir(char *pathname);
     int accept(int s,const struct sockaddr *addr,int *addrlen);
     int bind(int s, const struct sockaddr *name,int namelen);
     int getsockname(int s, struct sockaddr *name, int *namelen);
*/

#ifndef FD_SET
#include <sys/select.h>
#else
        int select (int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
#ifdef sgi
	int gettimeofday(struct timeval *tp,...);
#else
#endif
#endif

#ifndef FIONREAD
#include <sys/filio.h>
#endif


#ifdef __cplusplus
}
#endif


int errorOut(char *msg);
int createService(unsigned short *Port);
int dump(char *bp,long count);
int readString(int clientSocket,char *buff,long n);
int getByte(int clientSocket,int *n);
int getname(int clientSocket,char *buff);
int getnamesize(int clientSocket,char *buff,long *nn);
static int getLong(int clientSocket,long *n);
static int zerol(char *p,long n);
int doMACB(int clientSocket);
int doTEXT(int clientSocket);
int readCommand(int clientSocket,char *buff,long *size);
int flush(int clientSocket,long size);
int writeByte(int toServerSocket,int i);
int writeInt(int toServerSocket,int i);
int writeLong(int toServerSocket,long i);
int writeString(int toServerSocket,char *buffout,long HdfSize);
int namesize(char *name);
int writeName(int toServerSocket,char *name);
int writeLab(int toServerSocket,char *Type,long size);
int doSendFile(int clientSocket,char *name,char *Type);
int doFILB(int clientSocket);
int compar(struct dirent **a,struct dirent **b);
int dodir(int clientSocket,char *nb);
int doDIRR(int clientSocket);
int docd(char *nb,int flag);
int domkdir(char *nb);
int cleanName(char *buff);
int doCATN(int clientSocket);
int doCATE(int clientSocket);
int doMAB2(int clientSocket);

int ListenSocket(int clientSocket);

int Usage(void);
int copyl(char *p1,char *p2,long n);


struct MBHead {		/* MacBinary Header */
	char zero1;
	unsigned char nlen;
	unsigned char name[63];
	unsigned char type[4];
	unsigned char creator[4];
	char flags;
	char zero2;
	char location[6];
	char protected2;
	char zero3;
	unsigned char dflen[4];
	unsigned char rflen[4];
	unsigned char cdate[4];
	unsigned char mdate[4];
};


#ifndef SUN
#define d_namlen d_reclen
#endif
#define IN_BUFF_SIZE 163840
char buffin[IN_BUFF_SIZE];
long Bytes;
FILE *FileOut;


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

unsigned char *netbuff;
long netsize=32768;
long netsizeMax=32768;
long netcount=0;
long netblock=0;
int netErrorCheck;

int netFill(int clientSocket);
int netRead(int clientSocket,char *buff,long nbyte);
int netCheck(int toServerSocket,char *m,long *block);
int netGetInt(unsigned char *c,unsigned int *n);
int netGetLong(unsigned char *c,long *n);
unsigned int Crc(unsigned char *s,long int n);
int netGetBlock(long *blength,int toServerSocket);
int netSendBlock(char *m,long block,int clientSocket);
int netPutLong(unsigned char *c,long i);
int netFlush(int clientSocket);
int Sleep(int ms);
int netGetType(int clientSocket,char *buff,long *size);
int netReadCheck(int clientSocket,char *buff,long nbyte);

int Debug;

char *passname;
int passwd;

int replaceSpaces;

int getPortAndName(char *in,unsigned int *hostAddr,unsigned short *iPort);

int CheckSocket(int serverSocket,int *count,int ms);

static int putString(char *data,long length,FILE *out);

struct scanFiles{
   char *name;
   int type;
};

struct scanDir{
   struct scanFiles *Files;
   char *path;
   int count;
};


struct scanDir *scandirectory(char *dir);

int freedirectory(struct scanDir *scan);

extern int intcmp(const void *, const void *);

char *strsave(char *);

struct sendData{
    char *buff;
    long buffSize;
    long count;
    long Bytes;
    long BytesLast;
    int socket;
};

int relayIt(int relay,int argc,char *argv[]);

int relaySocket(int clientSocket,int toServerSocket,int buffsize);

int connectToServer(char *serverName,unsigned short *Port);

int GetTime(long *Seconds,long *milliseconds);

double rtime(void);

int netWrite(struct sendData *toServer,char *buffout,long HdfSize);

int netReadAny(int clientSocket,char *buff,long *n);


#define WAIT_TO_READ_COMMAND      1000
#define WAIT_TO_PROCESS_HEADER    1001
#define WAIT_TO_PROCESS_DATA      1002

#define UN_PACK_HEADER            1003
#define UN_PAK0_HEADER            1004
#define UN_PAK1_HEADER            1005
#define UN_PAK2_HEADER            1006
#define UN_PAK4_HEADER            1007
#define UN_PAKZ_HEADER            1008

struct global{
    unsigned char *buffin;
    unsigned char *buffout;
    long buffsize;
}xg;



int doAutoCompression(unsigned char *buff,long nbytes,struct sendData *toServer);

int doPAK0(unsigned char *buff,long nbytes,struct sendData *toServer);

int doPAK1Test(unsigned char *buff,long nbytes,struct sendData *toServer,unsigned long *nl);

int doPAK2Test(unsigned char *buff,long nbytes,struct sendData *toServer,unsigned long *nl);

int doPAK4Test(unsigned char *buff,long nbytes,struct sendData *toServer,unsigned long *nl);

int writeBuffString4(unsigned char *c,char *string);

int writeBuffLong(unsigned char *c,unsigned long i);

int writeBuffInt(unsigned char *c,unsigned int i);

int writeBuffByte(unsigned char *c,unsigned int i);

int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize);

int doUnPack(int clientSocket,struct sendData *toServer,
             int *readModeBytes,int flag,unsigned char *data);

int UnPack(register unsigned char *image,register unsigned char *buff,
           register long Length);

int SetBuffers(long Length);

int relayDebug,relayFastLine;

int relayEnd(int clientSocket,int toServerSocket,int buffsize);

int doUnPack4(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data);

int doUnPack2(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data);

int doUnPack1(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data);

int doUnPack0(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data);

int doUnPackz(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data);

int doPAKZ(unsigned char *buff,long nbytes,struct sendData *toServer);

int relayUsage(void);

char WarningBuff[256];

int WarningBatch(char *message);

int Warning(char *message);

static char *OpenWithApplication="/Applications/mShow.app";

int mstrncat(char *out,char *in,long n);

int mstrncpy(char *out,char *in,long n);


struct universalPalettes{
    char *name;
    unsigned char *pal;
};

struct universalListOfPalettes{
	int palCount;
	int palMax;
	struct universalPalettes *pals;
};

struct CursorDisplayItems{
    char pioName[64];
    char format[8];
    int pioIndex;
    int valueFlag;
	double *data;
};

struct CursorStruct{
	struct CursorDisplayItems *Items;
    char coordinateFormat[8];
    char valueformat[8];
    char timeFormat[8];
    int valueFlag;
	int count;
};

struct universalPreferencesStruct{
	struct universalListOfPalettes palList;
	uRect SaveFileRect;
	uRect LoadFileRect;
	uRect SaveImageRect;
	uRect dataSelectRect;
	uRect newDialogRect;
	uRect dumpSelectRect;
	uRect cursorSelectRect;
	int SaveSDS64Bits;
	int LocalOpenDialog;
	int TextOutputFormat;
	int UseStartConfiguration;
	char *startConfiguration;
	int flipSDS2dY;
	int VoiceRecognition;
	long BlockSizeGlobal;
	char *OpenWithApplication;
/* scratch variables */	
	struct CursorStruct Cursor;
	struct FileList *Files;
	struct FileList *Files2;
	int config;
	char *path;
	char *readWritePath;
	char *relativePath;
};
struct universalPreferencesStruct uPref;


int doPAL8(int clientSocket);

static char pal[256*3];

int getPalette(int n,char *name,char *pal);

static int getInt(int clientSocket,int *n);

int doH8CM(int clientSocket);

int doLIMI(int clientSocket);

void *cMalloc(unsigned long r,int tag);

int cFree(char *p);

int main(int argc,char *argv[])
{
	struct sockaddr_in clientSocketAddr;
	unsigned char buff[1024];
	unsigned int hostAddr;
	unsigned int auxAddr[10];
	int auxCount;
	int relay;
	unsigned int inAddr;
	unsigned long start,total,end,key;
	time_t ship;
	int count;
	int status;
	pid_t flag;
	unsigned short Port;
	int serverSocket;
	int clientSocket;
	unsigned int addrLen;
	int ntry;
	int pid;
	int n;
	
	
	getPalette(12,NULL,pal);

	fprintf(stderr,"************************************************\n");
	fprintf(stderr,"**  hdfget 17.0a0 - COPYRIGHT SAIC 1993-2012. **\n");
	fprintf(stderr,"************************************************\n");

	passname=getenv("HDFGET_PASSWORD");
	passwd=1;


	Debug = 0;
	FileOut=stdout;
	flag = -1;
	Port=0;

	auxCount=0;

	relay=0;

	ntry=0;

	for(n=1;n<argc;++n){
	    if(!strcmp(argv[n],"-debug")){
		   Debug = 1;
	    }else if(!strcmp(argv[n],"-a")){
		    if(auxCount < 10)getPortAndName(argv[++n],&auxAddr[auxCount++],&Port);
	    }else if(!strcmp(argv[n],"-spaces")){
	        replaceSpaces = 1;
	    }else if(!strcmp(argv[n],"-relay")){
	        relay=1;
	    }else if(!strcmp(argv[n],"-relayStart")){
	        relay=2;
	    }else if(!strcmp(argv[n],"-relayEnd")){
	        relay=3;
	    }
	}

	if(relay > 0){
	    relayIt(relay,argc,argv);
	    exit(1);
	}

	if(argc < 2){
	   Usage();
	}


	if(getPortAndName(argv[1],&hostAddr,&Port))exit(1);

	/* printf("hostAddr %lx Port %d\n",(long)hostAddr,Port); */

	if(passwd){
	    if(passname && (*passname)){
			fprintf(stderr,"\n   Warning key is appended to password\n\n");
	    }
	}

	serverSocket=createService(&Port);
    if(serverSocket == -1){
          printf("Create Port %d Failed\n",Port);
	      exit(1);
	}

	addrLen=sizeof(clientSocketAddr);
	start=time(&ship);
	end=start+28800;
	key = start+Port+(crctab[start & 0xff] << 16)+crctab[start & 0xff];
	printf("Listen on Port %d tag %lx\n",Port,key);
	fflush(stdout);
	while(1){
	    long size;
	    int ret;

	    ret=CheckSocket(serverSocket,&count,30000);

            total=time(&ship);

            if(total > end){
                printf("hdfget Auto Logout Out After %ld Seconds\n ",total-start);
  	        	shutdown(serverSocket,2);
	        	close(serverSocket);
                break;
            }

	    if(!ret){
			continue;
        }
            

	    clientSocket=accept(serverSocket,(struct  sockaddr  *)&clientSocketAddr,
	                        &addrLen);

	    inAddr=clientSocketAddr.sin_addr.s_addr;
 	    inAddr=htonl(inAddr);
	    if(inAddr != hostAddr){
	        
	        for(n=0;n<auxCount;++n)if(inAddr == auxAddr[n])goto Okgo;
	        
	        printf("Dropping Unauthorized Connection from %d.%d.%d.%d\n",
			(unsigned int)((inAddr >> 24) & 0xff),
			(unsigned int)((inAddr >> 16) & 0xff),
			(unsigned int)((inAddr >> 8) & 0xff),
			(unsigned int)((inAddr ) & 0xff));
ErrorOut:
	        shutdown(clientSocket,2);
	        close(clientSocket);
	        if(++ntry > 4){
				fprintf(stderr,"Program Exit ntry (%d)\n",ntry);	
				exit(1);        
	        }
	        continue;
	       
	    }
Okgo:

		if(netGetType(clientSocket,(char *)buff,&size))return 1;
		if(!strcmp((char *)buff,"ERRC")){
			netErrorCheck=1;
			printf("Error Checking On\n");
			fflush(stdout);
		    if(netGetType(clientSocket,(char *)buff,&size))return 1;
		}else{
			netErrorCheck=0;
			printf("Error Checking Off\n");
			fflush(stdout);
		}
		if(!strcmp((char *)buff,"PASS")){
			char password[256];
			char pass[256];
			unsigned char nnpp;
			
			if(netRead(clientSocket,(char *)&nnpp,1L))goto ErrorOut;
			if(netRead(clientSocket,pass,(long)nnpp))goto ErrorOut;
			pass[nnpp]=0;
			if(passname){
				strcpy(password,passname);
			}else{
				password[0]=0;
			}
			sprintf((char *)buff,"%lx%c",key,0);
			strcat(password,(char *)buff);
			if(strcmp(pass,password)){
				fprintf(stderr,"Password rejected \n");
				goto ErrorOut;
			}
		}else{
			fprintf(stderr,"Password Not found %d %d %d %d size %ld \n ",buff[0],buff[1],buff[2],buff[3],size);
			goto ErrorOut;
		}


	    pid=fork();
	    if(pid == 0){
	        close(serverSocket);
			netbuff=(unsigned char *)malloc(netsize+128);
			if(!netbuff){
				fprintf(stderr,"Error Could Not Allocate %ld Bytes For Network Buffer\n",netsize);
				shutdown(clientSocket,2);
				close(clientSocket);
				exit(1);
			}
			netsize=32768;
			netcount=netsize;
			netblock=0;
	        ListenSocket(clientSocket);
	        shutdown(clientSocket,2);
	        close(clientSocket);
	        exit(0);
	    }
	    close(clientSocket);
	    waitpid(flag,&status,WNOHANG);
	}
	return 0;
}
int CheckSocket(int serverSocket,int *count,int ms)
{
       struct timeval   timeout;
       fd_set filedes;

        FD_ZERO(&filedes);
        FD_SET(serverSocket, &filedes);

	timeout.tv_sec = ms/1000;
	timeout.tv_usec = (ms%1000)*1000;
	
        if (select(32, &filedes, (fd_set *)NULL, (fd_set *)NULL, &timeout))  {
                ioctl(serverSocket, FIONREAD, count);
                return TRUE;
        }
	*count = 0;
        return FALSE;
}
int getPortAndName(char *in,unsigned int *hostAddr,unsigned short *Port)
{
	struct sockaddr_in serverSocketAddr;
	struct hostent *serverHostEnt;
	char out[256];
	unsigned int oneNeg;
	char *np;

	if(!in || !Port)return 1;

	zerol((char *)&serverSocketAddr,sizeof(serverSocketAddr));

	strcpy(out,in);
	if((np=strrchr(out,':'))){
	    *np=0;
	    np += 1;
	    *Port=(unsigned short)atol(np);
	}

	oneNeg = -1L;

	*hostAddr=(unsigned int)inet_addr(out);
	if((long)(*hostAddr) != (long)oneNeg){
 	    *hostAddr=htonl(*hostAddr);
	    printf("Found Address %lx hostAddr %x oneNeg %x diff %x\n",(long)(*hostAddr),*hostAddr,oneNeg,*hostAddr-oneNeg);
	}else{
	    serverHostEnt=gethostbyname(out);
	    if(serverHostEnt == NULL){
	        printf("Could Not Find Host (%s)\n",out);
	        return 1;
	    }
	    copyl((char *)serverHostEnt->h_addr,(char *)&serverSocketAddr.sin_addr,serverHostEnt->h_length);
	    *hostAddr=serverSocketAddr.sin_addr.s_addr;
 	    *hostAddr=htonl(*hostAddr);
	    printf("Found Address %lx\n",(long)*hostAddr);
	}

	return 0;
}
int copyl(char *p1,char *p2,long n)
{
	if(!p1 || !p2)return 1;

	while(n-- > 0)*p2++ = *p1++;

	return 0;
}
int Usage(void)
{
	fprintf(stderr,"Usage : hdfget IP:Port [options]\n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"          examples                              \n");
	fprintf(stderr,"hdfget gamma.saic.com:3800                      \n");
	fprintf(stderr,"hdfget 198.151.90.60:3800 -debug                \n");
	fprintf(stderr,"hdfget 198.151.90.60:3800 -spaces               \n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"         [options]                              \n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"     -debug    (turn on debug print)            \n");
	fprintf(stderr,"     -spaces   (remove spaces from names)       \n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"     (hdfget will take data from the IP address indicted)           \n");
	fprintf(stderr,"      (only if the password (passname + tag) is correct)            \n");
	fprintf(stderr,"                                                                    \n");
	fprintf(stderr,"     (hdfget creates a new tag everytime that it is started and)  \n");
	fprintf(stderr,"     (logs itself off after 8 hours. The optional passname is  )  \n");
	fprintf(stderr,"     (read from the environment variable  \"HDFGET_PASSWORD\". )  \n");
	fprintf(stderr,"                                                                     \n");
	exit(1);
	return 0;
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
int netGetType(int clientSocket,char *buff,long *size)
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
int netFill(int clientSocket)
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
	if(CheckSocket(clientSocket,&count,33)){
	    if(count <= 0)return 0;
	    fprintf(stderr,"netFlush skipping %d bytes\n",count);
	    do{
		k=count;
		if(k > netsizeMax)k = (int)netsizeMax;
		if(netRead(clientSocket,(char *)netbuff,k))return 1;
		count -= k;
	    }while(count > 0);
	    goto Start;
	}
	return 0;
}
int Sleep(int ms)
{
    static struct timeval delay;

#define SELECT_MASK fd_set

    delay.tv_sec = ms/1000;
    delay.tv_usec = (ms%1000)*1000;
    (void) select(0, (SELECT_MASK *) 0, (SELECT_MASK *) 0,
            (SELECT_MASK *) 0, &delay);
	return 0;
}
int netCheck(int toServerSocket,char *m,long *block)
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
int netSendBlock(char *m,long block,int clientSocket)
{
	unsigned char message[14];

	strcpy((char *)message,"nt01");
	strcat((char *)message,m);
	netPutLong(&message[8],block);
	if(writeString(clientSocket,(char *)message,12L))return 1;
	return 0;
}
int netGetBlock(long *blength,int toServerSocket)
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
	        if(netbuff)free(netbuff);
	        netbuff=(unsigned char *)malloc(*blength);
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
int netPutLong(unsigned char *c,long i)
{ 
	if(!c)return 1;

        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);

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
int netRead(int clientSocket,char *buff,long n)
{
	long k;
	int isleep;

	if(!buff)return 1;

	Bytes += n;

	isleep=0;

	k=n;
	while(k > 0){
	    n=k;
	    n=read(clientSocket,buff,n);
	    if(n > 0){
	        k -= n;
	        buff += n;
		isleep=0;
	    }else if(n == 0){
	        if(++isleep > 20){
	            fprintf(stderr,"netRead Time Out Error\n");
	            return 1;
	        }else{
	            sleep(1L);
	        }
	    }else{
	        fprintf(stderr,"netRead Error Reading Socket\n");
		return 1;        
	    }
	}
	return 0;
}
int ListenSocket(int clientSocket)
{
	unsigned long start,total;
	time_t ship;
	char buff[256];
	long size;

	fprintf(stderr,"******************************************************\n");
	fprintf(stderr,"**  hdfget 17.0a0 - COPYRIGHT SAIC 1993-2012. Start **\n");
	fprintf(stderr,"******************************************************\n");

	start=time(&ship);
	Bytes=0;


	while(1){
	    if(readCommand(clientSocket,buff,&size))return 1;

	    if(!strcmp(buff,"ENDT")){
	        break;
	    }else if(!strcmp(buff,"LIMI")){
	        if(Debug){
				printf("doLIMI\n");
				fflush(stdout);
		    }
	        doLIMI(clientSocket);
	    }else if(!strcmp(buff,"PAL8")){
	        if(Debug){
				printf("doPAL8\n");
				fflush(stdout);
		    }
	        doPAL8(clientSocket);
	    }else if(!strcmp(buff,"H8CM")){
	        if(Debug){
				printf("doH8CM\n");
				fflush(stdout);
		    }
	        doH8CM(clientSocket);
	    }else if(!strcmp(buff,"TEXT")){
	        if(Debug){
				printf("doTEXT\n");
				fflush(stdout);
		    }
	        doTEXT(clientSocket);
	    }else if(!strcmp(buff,"BINA")){
	        if(Debug){
				printf("doBINA\n");
				fflush(stdout);
		    }
	        doTEXT(clientSocket);
	    }else if(!strcmp(buff,"MAB2")){
	        if(Debug){
				printf("doMAB2\n");
				fflush(stdout);
			}
	        doMAB2(clientSocket);
	    }else if(!strcmp(buff,"CATN")){
	        if(Debug){
				printf("doCATN\n");
				fflush(stdout);
			}
	        doCATN(clientSocket);
	    }else if(!strcmp(buff,"CATE")){
	        if(Debug){
				printf("doCATE\n");
				fflush(stdout);
		    }
	        doCATE(clientSocket);
	    }else if(!strcmp(buff,"FILB")){
	        if(Debug){
				printf("doFILB\n");
				fflush(stdout);
			}
	        doFILB(clientSocket);
	    }else if(!strcmp(buff,"DIRR")){
	        if(Debug){
				printf("doDIRR\n");
				fflush(stdout);
			}
			
	        if(doDIRR(clientSocket))continue;

	        if(Debug){
				printf("doDIRR Done\n");
				fflush(stdout);
			}
	    }else{
	        printf("Unknown Command (%s) %d %d %d %d Droping Connection\n",
	                buff,buff[0],buff[1],buff[2],buff[3]);
	        return 1;
	    }
	}

    total=time(&ship)-start;
	if(!total)total=1;
    printf("%ld Seconds To Receive %ld Bytes (%ld Bytes/s)\n",
                 total,Bytes,(Bytes/total));
	fprintf(stderr,"******************************************************\n");
	fprintf(stderr,"**  hdfget 17.0a0 - COPYRIGHT SAIC 1993-2012. Done  **\n");
	fprintf(stderr,"******************************************************\n");

	return 0;
}
int doMAB2(int clientSocket)
{
	struct MBHead HEAD;
	long Data,Resource,DataN,ResourceN;
	char buff[IN_BUFF_SIZE],*bp,*bh,name[256];
	FILE *out;
	long k,n;
	int ret;

	ret=1;
	out=NULL;

	zerol((char *)&HEAD,sizeof(struct MBHead ));

	if(getnamesize(clientSocket,(char *)HEAD.name,&n))return 1;

	printf("doMAB2 name (%s) \n",HEAD.name);

	if(Debug){
	    fflush(stdout);
	}

	HEAD.nlen=(unsigned char)n;

	for(n=0;n<HEAD.nlen;++n)buff[n]=HEAD.name[n];

	buff[n]=0;

	cleanName(buff);
	strcpy(name,buff);
	if(!(out=fopen(name,"wb"))){
	    printf("Could Not Open (%s) To Write\n",name);
	    fflush(stdout);
	}

	if(readString(clientSocket,(char *)HEAD.creator,4L))goto Error;
	strncpy(buff,(char *)HEAD.creator,4);
	buff[4]=0;

	if(Debug){
	    printf("doMAB2 Creator (%s) \n",buff);
	    fflush(stdout);
	}

	if(readString(clientSocket,(char *)HEAD.type,4L))goto Error;
	strncpy(buff,(char *)HEAD.type,4);
	buff[4]=0;

	if(Debug){
	    printf("doMAB2 Type  (%s) \n",buff);
	    fflush(stdout);
	}

	if(readString(clientSocket,(char *)HEAD.cdate,4L))goto Error;
	strncpy(buff,(char *)HEAD.cdate,4);
	buff[4]=0;

	if(Debug){
	    printf("doMAB2 cdate  (%lx) \n",*((unsigned long *)buff));
	    fflush(stdout);
	}

	if(readString(clientSocket,(char *)HEAD.mdate,4L))goto Error;
	strncpy(buff,(char *)HEAD.mdate,4);
	buff[4]=0;

	if(Debug){
	    printf("doMAB2 mdate  (%lx) \n",*((unsigned long *)buff));
	    fflush(stdout);
	}

	if(getLong(clientSocket,&Data))goto Error;

	if(Debug){
	    printf("doMAB2 Data  (%ld) \n",Data);
	    fflush(stdout);
	}

	HEAD.dflen[0]= (unsigned char)((Data >> 24) & 0xff);
	HEAD.dflen[1]= (unsigned char)((Data >> 16) & 0xff);
	HEAD.dflen[2]= (unsigned char)((Data >> 8) & 0xff);
	HEAD.dflen[3]= (unsigned char)(Data & 0xff);

	if(getLong(clientSocket,&Resource))goto Error;

	if(Debug){
	    printf("doMAB2 Resource (%ld)\n",Resource);
	    fflush(stdout);
	}

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

	

	if(putString(buff,128,out))goto Error;
	

	k=Data;
	DataN=Data;
	while(Data > 0){
	    k=Data;
	    if(k > IN_BUFF_SIZE)k = IN_BUFF_SIZE;
	    if(readString(clientSocket,buff,k))goto Error;
	    Data -= k;
	    if(out){
		if(putString(buff,k,out))goto Error;
	    }
	}

	n = (DataN+127)/128;

	k=128*n;

	n=k-DataN;

	while(n-- > 0)putc('\0',out);

	if(Debug){
	    printf("doMAB2 data Read\n");
	    fflush(stdout);
	}

	k=Resource;
	ResourceN=Resource;
	while(Resource > 0){
	    k=Resource;
	    if(k > IN_BUFF_SIZE)k = IN_BUFF_SIZE;
	    if(readString(clientSocket,buff,k))goto Error;
	    Resource -= k;
	    if(out){
		if(putString(buff,k,out))goto Error;
	    }
	}

	n = (ResourceN+127)/128;

	k=128*n;

	n=k-ResourceN;

	while(n-- > 0)putc('\0',out);

	if(Debug){
	    printf("doMAB2 Resource Read\n");
	}
	ret=0;
Error:
	fflush(stdout);
	if(out)fclose(out);
	if(ret)fprintf(stderr,"Error Trying To Write %s\n",name);
	return ret;	
}
int doCATE(int clientSocket)
{
	clientSocket=clientSocket;
	return docd("..",0);
}
int doCATN(int clientSocket)
{
	char buff[IN_BUFF_SIZE];

	if(getname(clientSocket,buff))return 1;
	cleanName(buff);
	if(docd(buff,1)){
	    if(domkdir(buff))return 1;
	    return docd(buff,0);
	}
	return 0;
}
int cleanName(char *buff)
{
	char *bp;
	int c;

	if(!buff)return 1;

	bp=buff;
	while((c=*bp) != 0){
	    if(c == '/'){
	        c = ':';
	    }else if(c <= ' ' || c > '~'){
	        if(c == ' '){
	            if(replaceSpaces)c = '_';	        
	        }else{
	            c = '_';
	        }
	    }
	    *bp = c;
	    ++bp;
	}
	return 0;
}
int domkdir(char *nb)
{
        int ret;

        ret = mkdir(nb,0755);

	return ret;
}
int docd(char *nb,int flag)
{
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
	            sprintf(out,"%s not a directory\n",dirname);
	            fprintf(FileOut,"%s",out);
		    fflush(FileOut);
	            return 1;		   
	        }
	    }
	}
	if(chdir(dirname)){
	     if(flag)return 1;
	     sprintf(out,"Could Not Change to Directory %s\n",dirname);
	     fprintf(FileOut,"%s",out);
	     fflush(FileOut);
	     return 1;		   
	}
	return 0;
}
int doDIRR(int clientSocket)
{
	char buff[IN_BUFF_SIZE];

	if(getname(clientSocket,buff))return 1;
	if(dodir(clientSocket,buff))return 1;
	return 0;
}
int dodir(int clientSocket,char *nb)
{
	struct scanDir *scan;
	int k,count,length;
	char *dirname,out[256],name[256];
	struct stat buf;

	if(!nb){
	    dirname=".";
	}else{
	    dirname=nb;
	    if((nb=strrchr(dirname,' '))){
	        *nb=0;
	    }
	    if(stat(dirname,&buf)){
	        errorOut("dodir Not a File\n");
	        return 0;		   
	    }else{
	        if(!(buf.st_mode & S_IFDIR)){
	            sprintf(out,"%s not a directory\n",dirname);
	            errorOut(out);
	            return 0;		   
	        }
	    }
	}

	scan=scandirectory(".");
	if(scan == NULL){
	    errorOut("scandirectory error\n");
	    return 1;
	}

	count=scan->count;


	length=4;
	for(k=0;k<count;++k){
	    strcpy(name,scan->Files[k].name);
	    length += 1+namesize(name);
	}

	writeLab(clientSocket,"DIRL",length);

	writeLong(clientSocket,count);
	
	for(k=0;k<count;++k){
	    strcpy(name,scan->Files[k].name);
	    writeName(clientSocket,name);
	}

	freedirectory(scan);

	return 0;
}
int compar(struct dirent **a,struct dirent **b)
{
	return strcmp((*a)->d_name,(*b)->d_name);
}
int doFILB(int clientSocket)
{
	char buff[IN_BUFF_SIZE];

	if(getname(clientSocket,buff))return 1;
	if(doSendFile(clientSocket,buff,"BINA"))return 1;
	return 0;
}
int doSendFile(int clientSocket,char *name,char *Type)
{
	FILE *in;
	long length,size;
	char *buffin;
	int ret;

	ret=0;
	length=0;
	in=NULL;
	buffin=NULL;
	if(!(in=fopen(name,"rb"))){
	    printf("Could Not Open (%s) To Read\n",name);
	    fflush(stdout);
	    ret=1;
	    goto Write;
	}

	if(fseek(in,0L,2)){
	    printf("(%s) Error Looking For End Of File\n",name);
	    fflush(stdout);
	    ret=1;
	    goto Write;
	}

	if((length=ftell(in)) == -1L){
	    printf("(%s) Error Reading For End Of File Location\n",name);
	    fflush(stdout);
	    ret=1;
	    goto Write;
	}

	if(fseek(in,0L,0)){
	    printf("(%s) Error Rewinding File\n",name);
	    fflush(stdout);
	    ret=1;
	    goto Write;
	}

	if(!(buffin=(char *)malloc(length))){
	    printf("(%s) Error Allocating Memory  Requested (%ld)\n",name,length);
	    ret=1;
	    length=0;
	    goto Write;
	}

	if(fread(buffin,1,length,in) != length){
	    printf("(%s) Error Reading File length (%ld)\n",name,length);
	    ret=1;
	    goto Write;
	}
Write:
	size=1+namesize(name)+length;

	writeLab(clientSocket,Type,size);


	writeName(clientSocket,name);
	
	writeLong(clientSocket,length);

	if(length)writeString(clientSocket,buffin,(long)length);

	if(buffin)free(buffin);

	if(in)fclose(in);

	printf("%s %ld\n",name,length);

	return ret;
}
int writeLab(int toServerSocket,char *Type,long size)
{
	writeString(toServerSocket,Type,4L);
	writeLong(toServerSocket,size);
	return 0;
}
int writeName(int toServerSocket,char *name)
{
	char *np;
	int n;

	if(!name)return 1;

	if(!(np=strrchr(name,'/'))){
	    np=name;
	}else{
	    np += 1;
	}

	n=namesize(name);

	writeByte(toServerSocket,n);

	writeString(toServerSocket,np,(long)n);
	return 0;
}
int namesize(char *name)
{
	char *np;
	int n;

	if(!(np=strrchr(name,'/'))){
	    np=name;
	}else{
	    np += 1;
	}

	n=(int)strlen(np);

	return n;
}
int writeString(int toServerSocket,char *buffout,long HdfSize)
{
	long n;

	Bytes += HdfSize;
	while(HdfSize > 0){
	    n=HdfSize;
	    n=write(toServerSocket,buffout,n);
	    if(n < 0){
	        printf("Write error (%ld)\n",(long)errno);
	        return 1;
	    }
	    buffout += n;
	    HdfSize -= n;
	}
	return 0;
}
int writeLong(int toServerSocket,long i)
{
      unsigned char c[4];
 
        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);
        return writeString(toServerSocket,(char *)c,4L);
}
int writeInt(int toServerSocket,int i)
{
      unsigned char c[2];
 
        c[0] = i & 255;
        c[1] = (i >> 8) & 255;
        return writeString(toServerSocket,(char *)c,2L);
}
int writeByte(int toServerSocket,int i)
{
      unsigned char c[2];
 
        c[0] = i & 255;
        return writeString(toServerSocket,(char *)c,1L);
}
int flush(int clientSocket,long size)
{
	long n;

	while(size > 0){
	    n=size;
	    if(n > IN_BUFF_SIZE)n=IN_BUFF_SIZE;
	    if(readString(clientSocket,buffin,n))return 1;
	    size -= n;
	}
	return 0;
}
int readCommand(int clientSocket,char *buff,long *size)
{
	long n;

	n=4;
	if(readString(clientSocket,buff,n))return 1;
	buff[n]=0;
	if(getLong(clientSocket,size))return 1;
	return 0;
	
}



int doH8CM(int clientSocket)
{
	int xsize,ysize;
	long Data;
	char name[256];
	char *buff;
	int ret;

	ret=1;
	buff=NULL;

	if(getname(clientSocket,name))return 1;
	
	if(getInt(clientSocket,&xsize))goto Error;
	if(getInt(clientSocket,&ysize))goto Error;
	if(getLong(clientSocket,&Data))goto Error;

	printf("name %s xsize %d ysize %d length %ld\n",name,xsize,ysize,Data);
	fflush(stdout);
	
	buff=(char *)cMalloc(Data,1889);
	if(!buff)goto Error;

     if(readString(clientSocket,buff,Data))goto Error;
     
     printf("doH8CM read Ok\n");
	fflush(stdout);
	    
	ret = 0;
Error:
	if(buff)cFree((char *)buff);
	return ret;
}

int doLIMI(int clientSocket)
{
	char buff[IN_BUFF_SIZE];

	if(getname(clientSocket,buff))return 1;
	printf("LIMI %s\n",buff);
	fflush(stdout);
	
	return 0;
}
int doPAL8(int clientSocket)
{
	char buff[IN_BUFF_SIZE];

	if(getname(clientSocket,buff))return 1;
	printf("doPAL8 name %s\n",buff);
	if(readString(clientSocket,pal,256*3))return 1;
	printf("doPAL8 read Palette Ok\n");
	fflush(stdout);
	
	return 0;
}
int doTEXT(int clientSocket)
{
	long Data;
	char buff[IN_BUFF_SIZE],name[256];
	FILE *out;
	long k;
	int ret;

	ret=1;

	if(getname(clientSocket,buff))return 1;
	cleanName(buff);
	strcpy(name,buff);
	if(!(out=fopen(name,"wb"))){
	    printf("Could Not Open (%s) To Write\n",name);
	    fflush(stdout);
	}

	if(getLong(clientSocket,&Data))goto Error;

	printf("name %s length %ld\n",buff,Data);
	fflush(stdout);

	k=Data;
	while(Data > 0){
	    k=Data;
	    if(k > IN_BUFF_SIZE)k = IN_BUFF_SIZE;
	    if(readString(clientSocket,buff,k))goto Error;
	    Data -= k;
	    if(out){
			if(putString(buff,k,out))goto Error;
	    }
	}
	ret = 0;
Error:
	if(out)fclose(out);
	if(ret)fprintf(stderr,"Error Trying To Write %s\n",name);
	return ret;
}
static int zerol(char *p,long n)
{
	if(!p)return 1;
	while(n-- > 0)*p++ = 0;
	return 0;
}
static int getLong(int clientSocket,long *n)
{
	unsigned char c[4];

	if(readString(clientSocket,(char *)c,4L))return 1;

	*n = c[0] + ((long)c[1] << 8) 
	          + ((long)c[2] << 16) + ((long)c[3] << 24);

	return 0;
}
static int getInt(int clientSocket,int *n)
{
	unsigned char c[2];

	if(readString(clientSocket,(char *)c,2L))return 1;

	*n = c[0] + ((long)c[1] << 8);

	return 0;
}
int getnamesize(int clientSocket,char *buff,long *nn)
{
	int n;

	if(getByte(clientSocket,&n))return 1;
	if(readString(clientSocket,buff,(long)n)){
	    printf("getname return error\n");
	    fflush(stdout);
	    return 1;
	}
	buff[n]=0;
	*nn=n;
	return 0;
}
int getname(int clientSocket,char *buff)
{
	int n;

	if(getByte(clientSocket,&n))return 1;
	if(readString(clientSocket,buff,(long)n)){
	    printf("getname return error\n");
	    fflush(stdout);
	    return 1;
	}
	buff[n]=0;
	return 0;
}
int getByte(int clientSocket,int *n)
{
	unsigned char buff[1];

	if(readString(clientSocket,(char *)buff,1L))return 1;

	*n=buff[0];

	return 0;
}
int dump(char *bp,long count)
{
	unsigned char *p;
	long k;

	if(!bp)return 1;

	p=(unsigned char *)bp;

	for(k=0;k<count;k += 32){
	    fprintf(stdout,
	    "%02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x\n",
	    p[k],p[k+1],p[k+2],p[k+3],p[k+4],p[k+5],p[k+6],p[k+7],
	    p[k+8],p[k+9],p[k+10],p[k+11],p[k+12],p[k+13],p[k+14],p[k+15]);
	    fprintf(stdout,
	    "%02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x\n",
	    p[k+16],p[k+17],p[k+18],p[k+19],p[k+20],p[k+21],p[k+22],p[k+23],
	    p[k+24],p[k+25],p[k+26],p[k+27],p[k+28],p[k+29],p[k+30],p[k+31]);
	}
	fflush(stdout);
	return 0;
}
int createService(unsigned short *Port)
{
	struct sockaddr_in serverSocketAddr;
	int serverSocket;
	struct sockaddr_in name;
	unsigned int namelen;

	zerol((char *)&serverSocketAddr,sizeof(serverSocketAddr));
	serverSocketAddr.sin_port=htons((unsigned short)0);
	serverSocketAddr.sin_port=htons(*Port);
	serverSocketAddr.sin_family=AF_INET;
	serverSocketAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverSocket=socket(AF_INET,SOCK_STREAM,0);
	bind(serverSocket,(struct  sockaddr  *)&serverSocketAddr,sizeof(serverSocketAddr));
	listen(serverSocket,5);
	namelen=sizeof(struct sockaddr_in);
	getsockname(serverSocket,(struct  sockaddr  *)&name,&namelen);
/*
	p=(unsigned char *)&name;
	p[namelen]=0;
	printf("namelen %d\n",namelen);
	for(n=0;n<namelen;++n)printf("%d ",p[n]);
	printf("\n");
	printf("Port %d\n",ntohs(name.sin_port));
*/
	*Port=ntohs(name.sin_port);
	return serverSocket;
	
}
int errorOut(char *msg)
{
	if(!msg)return 1;
	printf("%s\n",msg);
	return 0;
}
static int putString(char *data,long length,FILE *out)
{
	if(!data || !out)return 1;
	if(fwrite(data,1,length,out) != length){
	    return 1;
	}
	return 0;
}
int freedirectory(struct scanDir *scan)
{
	int n;

	if(!scan)return 1;
	if(scan->Files){
	    for(n=0;n<scan->count;++n){
	        if(scan->Files[n].name)free(scan->Files[n].name);
	    }
	    free((char *)scan->Files);
	}

	free((char *)scan);
	return 0;
}
struct scanDir *scandirectory(char *dir)
{
	register struct dirent *dirent;
	struct scanDir		*scan;
	register DIR		*dirp;
	char			*name;
	int			count;
	int			n,nfile;
        char			working[2048];
        int			ret;

	ret=1;

	dirp = NULL;

	if(!getcwd(working,2048)){
		fprintf(stderr,"scandirectory  Working directory error %s\n",working);
		return (struct scanDir *)NULL;
	}

	if((scan=(struct scanDir *)malloc(sizeof(struct scanDir))) == NULL){
		fprintf(stderr,"scandirectory  Out of Memroy\n");
		return (struct scanDir *)NULL;
	}

	zerol((char *)scan,sizeof(struct scanDir));

	nfile=0;
	scan->Files = NULL;

	scan->path = strsave(working);

	if(scan->path == NULL)goto ErrorOut;

	dirp = opendir(dir);

	if (dirp == NULL)goto ErrorOut;

	count = 0;

	while ((dirent = readdir(dirp)) != NULL) {
	    ++count;
	}

        rewinddir(dirp);

        if(((scan->Files)=(struct scanFiles*)malloc(sizeof(struct scanFiles)*count)) == NULL){
	   fprintf(stderr,"scanDir out of memory\n");
            goto ErrorOut;
        }

	zerol((char *)scan->Files,(long)(sizeof(struct scanFiles)*count));

        n=0;
	while ((dirent = readdir(dirp)) != NULL) {
	     struct stat buf;

	     if(!stat(dirent->d_name,&buf) && (buf.st_mode & S_IFDIR)){
	        if(!strcmp(dirent->d_name,".") || !strcmp(dirent->d_name,"..")){
	            --count;
	            continue;
	        }
		name=strsave(dirent->d_name);
		if(name == NULL){
	             fprintf(stderr,"scandir out of memory\n");
	             goto ErrorOut;
		}
		scan->Files[nfile].name=name;
		scan->Files[nfile++].type=1;
	     }else{	   
		name=strsave(dirent->d_name);
		if(name == NULL){
	             fprintf(stderr,"scandir out of memory\n");
	             goto ErrorOut;
		}
		scan->Files[nfile].name=name;
		scan->Files[nfile++].type=0;
	     }
	     ++n;
	}
	if(count != n){
	   fprintf(stderr,"scandir error %d %d\n",count,n);
	   goto ErrorOut;
	}
	scan->count=nfile;

	qsort((char *)scan->Files, scan->count, sizeof(struct scanFiles), intcmp);

	ret=0;
ErrorOut:
	if(dirp)closedir(dirp);
	if(ret == 0)return (scan);
	if(scan){
	   scan->count=nfile;
	   freedirectory(scan);
	}
	return (struct scanDir *)NULL;
}

int intcmp(const void *xx, const void *yy)
{
	struct scanFiles *x;
	struct scanFiles *y;

	x=(struct scanFiles *)xx;
	y=(struct scanFiles *)yy;
	return strcmp(x->name,y->name);
}
char *strsave(char *s)
{
	char *r;
	unsigned long n;

	if(!s)return (char *)NULL;
	n=strlen(s)+1;
	r=(char *)malloc(n);
	if(r == NULL)return (char *)NULL;
	strcpy(r,s);
	return r;
}
int relayUsage(void)
{
	fprintf(stderr,"Usage : hdfget IP1:Port1 IP2:Port2 -mode [options]      \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"          example : relay x-windows                     \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"     (on cpcsto)                                        \n");
	fprintf(stderr,"xhost cpcsto                                            \n");
	fprintf(stderr,"hdfget oasis:3800 cpcsto:6000 -relayEnd                 \n");
	fprintf(stderr,"    (on oasis)                                          \n");
	fprintf(stderr,"setenv DISPLAY oasis:200                                \n");
	fprintf(stderr,"hdfget oasis:6200 cpcsto:3800 -relayStart  | xshow      \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"         [mode]                                         \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"   -relay      (relay Port1 to Port2)                   \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"   -relayStart (relay Port1 to Port2 with compression)  \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"   -relayEnd   (relay Port1 to Port2 with decompression)\n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"         [options]                                      \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"   -relayDebug    (turn on relay debug print)           \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"   -relayFastLine (use fast compression)                \n");
	fprintf(stderr,"                                                        \n");
	fprintf(stderr,"   -relaySlowLine (use slower but better compression)   \n");
	fprintf(stderr,"                                                        \n");
	exit(1);
	return 0;
}
int relayIt(int relay,int argc,char *argv[])
{
	struct sockaddr_in clientSocketAddr;
	unsigned int hostAddr;
	unsigned int inAddr;
	unsigned short Port;
	unsigned short Port2;
	int toServerSocket;
	int serverSocket;
	int clientSocket;
	int buffsize;
	unsigned int addrLen;
	int count;
	int ret;
	int n;


	if(!argv)return 1;

	if(argc < 3){
	    relayUsage();
	    return 1;
	}

	buffsize=90000;
	relayDebug = 0;
	relayFastLine = 0;
	for(n=3;n<argc;++n){
	    if(!strcmp(argv[n],"-relayDebug")){
		relayDebug = 1;
	    }else if(!strcmp(argv[n],"-buffer")){
	        buffsize=atoi(argv[++n]);
	    }else if(!strcmp(argv[n],"-relayFastLine")){
	        relayFastLine=1;
	    }else if(!strcmp(argv[n],"-relaySlowLine")){
	        relayFastLine=0;
	    }
	}

	if(getPortAndName(argv[1],&hostAddr,&Port)){
	      return 1;
	}

	serverSocket=createService(&Port);
        if(serverSocket == -1){
              printf("Create Port %d Failed\n",Port);
	      return 1;
	}
/*
	start=time(&ship);
	end=start+28800;
	key = start+Port+(crctab[start & 0xff] << 16)+crctab[start & 0xff];
	fprintf(stderr,"Listen on Port %d tag %lx\n",Port,key);
*/

	fprintf(stderr,"Listen on Port %d \n",Port);

	    ret=CheckSocket(serverSocket,&count,60000);
	    if(!ret){
                fprintf(stderr,"relay - Time Out !\n");
  	        shutdown(serverSocket,2);
	        close(serverSocket);
                goto OutOfHere;
	    }

	    addrLen=sizeof(clientSocketAddr);

	    clientSocket=accept(serverSocket,(struct  sockaddr  *)&clientSocketAddr,
	                        &addrLen);

	    inAddr=clientSocketAddr.sin_addr.s_addr;
 	    inAddr=htonl(inAddr);
	    if(inAddr != hostAddr){
	        fprintf(stderr,"Dropping Unauthorized Connection from %d.%d.%d.%d\n",
			(unsigned int)((inAddr >> 24) & 0xff),
			(unsigned int)((inAddr >> 16) & 0xff),
			(unsigned int)((inAddr >> 8) & 0xff),
			(unsigned int)((inAddr ) & 0xff));
	        shutdown(clientSocket,2);
	        close(clientSocket);
	        goto OutOfHere;
	    }

	        close(serverSocket);

		netbuff=(unsigned char *)malloc(netsize+128);
		if(!netbuff){
		    fprintf(stderr,"Error Could Not Allocate %ld Bytes For Network Buffer\n",netsize);
		    shutdown(clientSocket,2);
	            close(clientSocket);
	            goto OutOfHere;
		}

		netsize=32768;
		netcount=netsize;
		netblock=0;

		toServerSocket=connectToServer(argv[2],&Port2);
		if(toServerSocket == -1){
		    fprintf(stderr,"Connection failed to %s:%d\n",argv[2],Port2);
	            goto OutOfHere;
		}

	{
		static char *names[]={" ","off","on","relay","relayStart","relayEnd"};
		char *lineSpeed,*relayType,*debugType;

		debugType=names[1];
		if(relayDebug)debugType=names[2];
		lineSpeed=names[1];
		if(relayFastLine)lineSpeed=names[2];
		relayType=names[0];
		if(relay >= 1 && relay <=3)relayType=names[relay+2];
		fprintf(stderr,"Relay mode:%s relayFastLine:%s relayDebug:%s\n",relayType,lineSpeed,debugType);

	}


	if(relay == 1){
	    relaySocket(clientSocket,toServerSocket,buffsize);
	}else if(relay == 2){
	    relayEnd(toServerSocket,clientSocket,buffsize);
	}else if(relay == 3){
	    relayEnd(clientSocket,toServerSocket,buffsize);
	}else{
	    fprintf(stderr,"relayIt found unknown type %d\n",relay);
	    return 1;
	}

	shutdown(clientSocket,2);
	close(clientSocket);

	shutdown(toServerSocket,2);
	close(toServerSocket);

	return 0;
OutOfHere:
	return 1;
}
int relayEnd(int clientSocket,int toServerSocket,int buffsize)
{
	unsigned long start,total;
	time_t ship;
	char Command[1025];
	unsigned char *buff;
	unsigned char *buffRead;
	int readMode,readModeType,buffReadStart,readModeBytes;
	long nbytes;
	struct sendData toClient,toServer;
	long size;
	double tstart,tend;
	int count;

	fprintf(stderr,"****************************************************\n");
	fprintf(stderr,"**  relayEnd 16.9 - COPYRIGHT SAIC 2007.    Start **\n");
	fprintf(stderr,"****************************************************\n");


	buff=NULL;
	buffRead=NULL;
	toClient.buff=NULL;
	toServer.buff=NULL;

	start=time(&ship);

	buff=(unsigned char *)malloc(buffsize+128);
	if(!buff){
	    fprintf(stderr,"relayEnd Out of Memory %d\n",buffsize);
	    goto ErrorOut;
	}

	buffRead=(unsigned char *)malloc(buffsize+128);
	if(!buffRead){
	    fprintf(stderr,"relayEnd Out of Memory %d\n",buffsize);
	    goto ErrorOut;
	}


	toClient.buffSize=1000000;
	toClient.buff=(char *)malloc(toClient.buffSize);
	if(!toClient.buff){
	    fprintf(stderr,"relayEnd Out of Memory %ld",toClient.buffSize);
	    goto ErrorOut;
	}
	toClient.socket=clientSocket;
	toClient.count=0;
	toClient.Bytes=0;
	toClient.BytesLast=0;

	toServer.buffSize=1000000;
	toServer.buff=(char *)malloc(toServer.buffSize);
	if(!toServer.buff){
	    fprintf(stderr,"relayEnd Out of Memory %ld",toServer.buffSize);
	    goto ErrorOut;
	}
	toServer.socket=toServerSocket;
	toServer.count=0;
	toServer.Bytes=0;
	toServer.BytesLast=0;

	if(SetBuffers(buffsize+256))goto ErrorOut;

	start=time(&ship);
	Bytes=0;
	tstart=rtime();
	readMode=WAIT_TO_READ_COMMAND;
	readModeBytes = -1;
	readModeType  = -1;
	buffReadStart = -1;
	while(1){
	    count=0;
	    CheckSocket(toServerSocket,&count,2);
	    if(count > 0 ){
	        
		nbytes=buffsize;

		if(netReadAny(toServerSocket,(char *)&buff[32],&nbytes))goto ErrorOut;

	        if(nbytes > 0){

		    	if(SetBuffers(6*nbytes+100000))goto ErrorOut;

		    	if(doAutoCompression(buff,nbytes,&toClient))goto ErrorOut;

		    	tstart=rtime();
	        }
	    }

	    if(netWrite(&toClient,(char *)xg.buffout,0L)){
	        fprintf(stderr,"netWrite Buffer Dump Error\n");
	        goto ErrorOut;
	    }


	    tend=rtime();
	    if(tend > (tstart+3600.)){
	        fprintf(stderr,"Time Out No Activity for %.1f seconds readMode %d\n",3600.,readMode);
			goto ErrorOut;
	    }


	    count=0;
	    CheckSocket(clientSocket,&count,2);
	    if(count <= 0)continue;
	    tstart=rtime();
	    if(readMode == WAIT_TO_READ_COMMAND){
	        if(count < 8)continue;
	        if(readCommand(clientSocket,Command,&size))break;
	        if(!strcmp(Command,"ENDT")){
	            break;
	        }else if(!strcmp(Command,"PACK")){
		    readModeBytes=8;
		    readModeType=UN_PACK_HEADER;
	        }else if(!strcmp(Command,"PAK0")){
		    readModeBytes=8;
		    readModeType=UN_PAK0_HEADER;
	        }else if(!strcmp(Command,"PAK1")){
		    readModeBytes=8;
		    readModeType=UN_PAK1_HEADER;
	        }else if(!strcmp(Command,"PAK2")){
		    readModeBytes=20;
		    readModeType=UN_PAK2_HEADER;
	        }else if(!strcmp(Command,"PAK4")){
		    readModeBytes=28;
		    readModeType=UN_PAK4_HEADER;
	        }else if(!strcmp(Command,"PAKZ")){
		    readModeBytes=8;
		    readModeType=UN_PAKZ_HEADER;
	        }else{
	            fprintf(stderr,"Stop - Unknown Command %d %d %d %d '%s'\n",
	                          Command[0],Command[1],Command[2],Command[3],Command);
	            break;
	        }
		readMode=WAIT_TO_PROCESS_HEADER;
		count -= 8;
		if(count >= readModeBytes)goto GetHeader;
	    }else if(readMode == WAIT_TO_PROCESS_HEADER){
GetHeader:
	        if(count < readModeBytes)continue;
	        count -= readModeBytes;
	        if(readModeType == UN_PACK_HEADER){
	            doUnPack(clientSocket,&toServer,&readModeBytes,0,buffRead);
	        }else if(readModeType == UN_PAK0_HEADER){
	            doUnPack0(clientSocket,&toServer,&readModeBytes,0,buffRead);
	        }else if(readModeType == UN_PAK1_HEADER){
	            doUnPack1(clientSocket,&toServer,&readModeBytes,0,buffRead);
	        }else if(readModeType == UN_PAK2_HEADER){
	            doUnPack2(clientSocket,&toServer,&readModeBytes,0,buffRead);
	        }else if(readModeType == UN_PAK4_HEADER){
	            doUnPack4(clientSocket,&toServer,&readModeBytes,0,buffRead);
	        }else if(readModeType == UN_PAKZ_HEADER){
	            doUnPackz(clientSocket,&toServer,&readModeBytes,0,buffRead);
	        }else{
	            fprintf(stderr,"Stop - Unknown Header to Process %d\n",readModeType);
	            break;
	        }
			readMode=WAIT_TO_PROCESS_DATA;
			buffReadStart=0;
			if(count > 0)goto GetData;
	    }else if(readMode == WAIT_TO_PROCESS_DATA){
GetData:
	        nbytes=readModeBytes;
	        if(buffReadStart+readModeBytes > buffsize){
	            fprintf(stderr,"Stop - buffsize %d readModeBytes %d\n",buffsize,readModeBytes);
	            goto ErrorOut;
	        }
	        if(netReadAny(clientSocket,(char *)&buffRead[buffReadStart],&nbytes))goto ErrorOut;
	        if(nbytes <= 0)continue;
			buffReadStart += nbytes;
			readModeBytes -= nbytes;
	        if(readModeBytes > 0)continue;
	        if(readModeType == UN_PACK_HEADER){
	            doUnPack(clientSocket,&toServer,&readModeBytes,1,buffRead);
	        }else if(readModeType == UN_PAK0_HEADER){
	            doUnPack0(clientSocket,&toServer,&readModeBytes,1,buffRead);
	        }else if(readModeType == UN_PAK1_HEADER){
	            doUnPack1(clientSocket,&toServer,&readModeBytes,1,buffRead);
	        }else if(readModeType == UN_PAK2_HEADER){
	            doUnPack2(clientSocket,&toServer,&readModeBytes,1,buffRead);
	        }else if(readModeType == UN_PAK4_HEADER){
	            doUnPack4(clientSocket,&toServer,&readModeBytes,1,buffRead);
	        }else if(readModeType == UN_PAKZ_HEADER){
	            doUnPackz(clientSocket,&toServer,&readModeBytes,1,buffRead);
	        }else{
	            fprintf(stderr,"Stop - Unknown Data to Process %d\n",readModeType);
	            break;
	        }
			readMode=WAIT_TO_READ_COMMAND;
	    }
	}
ErrorOut:
        total=time(&ship)-start;
		if(!total)total=1;
        printf("%ld Seconds To Receive %ld Bytes (%ld Bytes/s)\n",
                 total,Bytes,(Bytes/total));
	fprintf(stderr,"****************************************************\n");
	fprintf(stderr,"**  relayEnd 16.9 - COPYRIGHT SAIC 2007.     Done **\n");
	fprintf(stderr,"****************************************************\n");

	if(buff)free((char *)buff);
	if(buffRead)free((char *)buffRead);
	if(toClient.buff)free((char *)toClient.buff);
	if(toServer.buff)free((char *)toServer.buff);

	return 0;
}

int doUnPack0(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data)
{
	static long Compressed,UnCompressed;

	if(!toServer)return 1;


	if(flag == 0){
	    if(getLong(clientSocket,&Compressed))return 1;
	    if(getLong(clientSocket,&UnCompressed))return 1;
	    if(SetBuffers(UnCompressed+100000))return 1;
	    *readModeBytes= (int)Compressed;
	}else{
	    if(netWrite(toServer,(char *)data,UnCompressed))return 1;

	    if(relayDebug){
	        fprintf(stderr,"doUnPack0 in %ld out %ld total %ld\n",
	                        UnCompressed,Compressed,toServer->Bytes);
	    }
	    
	}
	return 0;
}
int doUnPack1(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data)

{
	static long Compressed,UnCompressed;

	if(!toServer)return 1;


	if(flag == 0){
	    if(getLong(clientSocket,&Compressed))return 1;
	    if(getLong(clientSocket,&UnCompressed))return 1;
	    if(SetBuffers(UnCompressed+100000))return 1;
	    *readModeBytes=(int)Compressed;

	}else{
	    if(UnPack(xg.buffout,data,UnCompressed))return 1;
	    if(netWrite(toServer,(char *)xg.buffout,UnCompressed))return 1;

	    if(relayDebug){
	        fprintf(stderr,"doUnPack1 in %ld out %ld total %ld\n",
	                        Compressed,UnCompressed,toServer->Bytes);
	    }
	}
	return 0;
}
int doUnPack2(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data)
{
	static long n1,n2,n;
	static long Length,Word2,Words;
	static unsigned char pad[4];

	if(!toServer)return 1;

	if(flag == 0){
	    if(getLong(clientSocket,&Length))return 1;
	    if(getLong(clientSocket,&Word2))return 1;
	    if(getLong(clientSocket,&n1))return 1;
	    if(getLong(clientSocket,&n2))return 1;
	    if(readString(clientSocket,(char *)pad,4L))return 1;
	    if(SetBuffers(Length+Word2+100000))return 1;
	    *readModeBytes=(int)Length;
	}else{

	    Words=Word2/2;

	    if(UnPack(&xg.buffin[Length],data,Words))return 1;


	    for(n=0;n<Words;++n){
	        xg.buffout[2*n]=xg.buffin[Length+n];
	    }
 
	    if(UnPack(&xg.buffin[Length],&data[n1],Words))return 1;

	    for(n=0;n<Words;++n){
	        xg.buffout[2*n+1]=xg.buffin[Length+n];
	    }
 
	    n=pad[0];
	    if(n > 0){
	        xg.buffout[Word2+0]=pad[1];
	        Word2 += n;
	    }

	    if(netWrite(toServer,(char *)xg.buffout,Word2))return 1;

	    if(relayDebug){
	        fprintf(stderr,"doUnPack2 in %ld out %ld total %ld\n",
	                        Length,Word2,toServer->Bytes);
	    }
	}
	return 0;
}
int doUnPack4(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data)
{
	static long n1,n2,n3,n4,n;
	static long Length,Word4,Words;
	static unsigned char pad[4];

	if(!toServer)return 1;

	if(flag == 0){
	    if(getLong(clientSocket,&Length))return 1;
	    if(getLong(clientSocket,&Word4))return 1;
	    if(getLong(clientSocket,&n1))return 1;
	    if(getLong(clientSocket,&n2))return 1;
	    if(getLong(clientSocket,&n3))return 1;
	    if(getLong(clientSocket,&n4))return 1;
	    if(readString(clientSocket,(char *)pad,4L))return 1;
	    if(SetBuffers(Length+Word4+100000))return 1;
	    *readModeBytes=(int)Length;
	}else{

	    Words=Word4/4;

	    if(UnPack(&xg.buffin[Length],data,Words))return 1;


	    for(n=0;n<Words;++n){
	        xg.buffout[4*n]=xg.buffin[Length+n];
	    }
 
	    if(UnPack(&xg.buffin[Length],&data[n1],Words))return 1;

	    for(n=0;n<Words;++n){
	        xg.buffout[4*n+1]=xg.buffin[Length+n];
	    }
 
	    if(UnPack(&xg.buffin[Length],&data[n1+n2],Words))return 1;

	    for(n=0;n<Words;++n){
	        xg.buffout[4*n+2]=xg.buffin[Length+n];
	    }
 
	    if(UnPack(&xg.buffin[Length],&data[n1+n2+n3],Words))return 1;

	    for(n=0;n<Words;++n){
	        xg.buffout[4*n+3]=xg.buffin[Length+n];
	    }

	    n=pad[0];
	    if(n > 0){
	        xg.buffout[Word4+0]=pad[1];
	        if(n > 1)xg.buffout[Word4+1]=pad[2];
	        if(n > 2)xg.buffout[Word4+2]=pad[3];
	        Word4 += n;
	    }

 
	    if(netWrite(toServer,(char *)xg.buffout,Word4))return 1;

	    if(relayDebug){
	        fprintf(stderr,"doUnPack4 in %ld out %ld total %ld\n",
	                        Length,Word4,toServer->Bytes);
	   }

	}
	return 0;
}
int doUnPackz(int clientSocket,struct sendData *toServer,
              int *readModeBytes,int flag,unsigned char *data)

{
	static unsigned long Compressed,UnCompressed;
	int err;

	if(!toServer)return 1;


	if(flag == 0){
	    if(getLong(clientSocket,(long *)&Compressed))return 1;
	    if(getLong(clientSocket,(long *)&UnCompressed))return 1;
	    if(SetBuffers((long)(UnCompressed+100000)))return 1;
	    *readModeBytes=(int)Compressed;
	}else{

	    err = uncompress(xg.buffout, &UnCompressed, data,Compressed);
	    if(err > 0){
	        fprintf(stderr,"doUnPackz err %d\n",err);
	        return 1;
	    }

	    if(netWrite(toServer,(char *)xg.buffout,(long)UnCompressed))return 1;

	    if(relayDebug){
	        fprintf(stderr,"doUnPackz in %ld out %ld total %ld\n",
	                        Compressed,UnCompressed,toServer->Bytes);
	    }
	}
	return 0;
}
int doAutoCompression(unsigned char *buff,long nbytes,struct sendData *toServer)
{
	unsigned long pak0,pak1,pak2,pak4/* ,pakz1 */;

	if(!buff || !toServer)goto ErrorOut;

	if(!relayFastLine){
	    return doPAKZ(buff,nbytes,toServer);
	}

	pak0=4*4+nbytes;

	doPAK1Test(buff,nbytes,NULL,&pak1);

	doPAK2Test(buff,nbytes,NULL,&pak2);

	doPAK4Test(buff,nbytes,NULL,&pak4);

	if(relayDebug){
	    fprintf(stderr,"pak0 %ld pak1 %ld pak2 %ld pak4 %ld\n",
	            pak0,pak1,pak2,pak4/* ,pakz1 */);
	}

	if(pak0 <= pak1){
	    if(pak2 <= pak4){
	        if(pak0 <= pak2){
	            doPAK0(buff,nbytes,toServer);
	        }else{
		    if(netWrite(toServer,(char *)&xg.buffout[2*nbytes],(long)pak2))goto ErrorOut;
	        }
	    }else{
	        if(pak0 <= pak4){
	            doPAK0(buff,nbytes,toServer);
	        }else{
		    if(netWrite(toServer,(char *)&xg.buffout[4*nbytes],(long)pak4))goto ErrorOut;
	        }
	    }
	}else{
	    if(pak2 <= pak4){
	        if(pak1 <= pak2){
		    if(netWrite(toServer,(char *)&xg.buffout[0],(long)pak1))goto ErrorOut;
	        }else{
		    if(netWrite(toServer,(char *)&xg.buffout[2*nbytes],(long)pak2))goto ErrorOut;
	        }
	    }else{
	        if(pak1 <= pak4){
		    if(netWrite(toServer,(char *)&xg.buffout[0],(long)pak1))goto ErrorOut;
	        }else{
		    if(netWrite(toServer,(char *)&xg.buffout[4*nbytes],(long)pak4))goto ErrorOut;
	        }
	    }
	}

	return 0;
ErrorOut:
	return 1;
}
int doPAKZ(unsigned char *buff,long nbytes,struct sendData *toServer)
{
	unsigned long HdfSize;
	int err;

	if(!buff)goto ErrorOut;

	HdfSize=nbytes*2;

	err = compress(&xg.buffout[16], &HdfSize, (const Bytef*)&buff[32], nbytes);
	if(err > 0){
	    fprintf(stderr,"doPAKZ Err %d\n",err);
	    goto ErrorOut;
	}
	    
	writeBuffString4(&xg.buffout[0],"PAKZ");
	writeBuffLong(&xg.buffout[4],HdfSize+8L);
	writeBuffLong(&xg.buffout[8],HdfSize);
	writeBuffLong(&xg.buffout[12],nbytes);

	if(netWrite(toServer,(char *)xg.buffout,(long)(HdfSize+16)))goto ErrorOut;

	if(relayDebug)fprintf(stderr,"doPAKz in %ld out %ld\n",nbytes,HdfSize);

	return 0;
ErrorOut:
	return 1;
}
int doPAK0(unsigned char *buff,long nbytes,struct sendData *toServer)
{
	if(!buff || !toServer)goto ErrorOut;

	    
	writeBuffString4(&buff[16],"PAK0");
	writeBuffLong(&buff[16+4],nbytes+8L);
	writeBuffLong(&buff[16+8],nbytes);
	writeBuffLong(&buff[16+12],nbytes);

	if(netWrite(toServer,(char *)&buff[16],nbytes+16))goto ErrorOut;

	return 0;
ErrorOut:
	return 1;
}
int doPAK1Test(unsigned char *buff,long nbytes,struct sendData *toServer,unsigned long *nl)
{
	long HdfSize;

	if(!buff)goto ErrorOut;

	if(HdfPack((unsigned char *)&buff[32],&xg.buffout[16],nbytes,&HdfSize))goto ErrorOut;

	if(toServer)if(HdfSize >= nbytes)return doPAK0(buff,nbytes,toServer);
	    
	writeBuffString4(&xg.buffout[0],"PAK1");
	writeBuffLong(&xg.buffout[4],HdfSize+8L);
	writeBuffLong(&xg.buffout[8],HdfSize);
	writeBuffLong(&xg.buffout[12],nbytes);

	*nl=4*4+HdfSize;

	if(toServer)if(netWrite(toServer,(char *)xg.buffout,HdfSize+16))goto ErrorOut;

	if(toServer && relayDebug)fprintf(stderr,"doPAK1 in %ld out %ld\n",nbytes,HdfSize);

	return 0;
ErrorOut:
	return 1;
}
int doPAK2Test(unsigned char *buff,long nbytes,struct sendData *toServer,unsigned long *nl)
{
	long Block2,Word2,Word1;
	long n1,n2,n,nn;
	unsigned char pad[4];
	long Length;

	if(!buff || (nbytes <= 0))goto ErrorOut;

	Block2=nbytes/2;
	Word2=Block2*2;
	Word1=nbytes-Word2;

	    nn=0;
	    for(n=0;n<Word2; n += 2){
	        xg.buffin[nn++]=buff[n+32];
	    }
            if(HdfPack(xg.buffin,&xg.buffout[28+2*nbytes],nn,&n1))goto ErrorOut;
	    nn=0;
	    for(n=0;n<Word2; n += 2){
	        xg.buffin[nn++]=buff[n+33];
	    }
            if(HdfPack(xg.buffin,&xg.buffout[n1+28+2*nbytes],nn,&n2))goto ErrorOut;

	    Length=n1+n2;

	    writeBuffString4(&xg.buffout[0+2*nbytes],"PAK2");
	    writeBuffLong(&xg.buffout[4+2*nbytes],Length+7*4L);
	    writeBuffLong(&xg.buffout[8+2*nbytes],Length);
	    writeBuffLong(&xg.buffout[12+2*nbytes],Word2);
	    writeBuffLong(&xg.buffout[16+2*nbytes],n1);
	    writeBuffLong(&xg.buffout[20+2*nbytes],n2);

		pad[0]=0;
		pad[1]=0;
		pad[2]=0;
		pad[3]=0;
		
	    pad[0]=(unsigned char)Word1;
	    if(Word1 > 0)pad[1]=buff[Word2+32];

	    writeBuffString4(&xg.buffout[24+2*nbytes],(char *)pad);

	    if(toServer)if(netWrite(toServer,(char *)&xg.buffout[2*nbytes],Length+7*4))goto ErrorOut;

	    *nl=7*4+Length;

            if(toServer && relayDebug)fprintf(stderr,"doPAK2 in %ld out %ld\n",Word2,Length);
		

	return 0;
ErrorOut:
	return 1;
}

int doPAK4Test(unsigned char *buff,long nbytes,struct sendData *toServer,unsigned long *nl)
{
	long Block4,Word4,Word1;
	long n1,n2,n3,n4,n,nn;
	unsigned char pad[4];
	long Length;

	if(!buff || (nbytes <= 0))goto ErrorOut;

	Block4=nbytes/4;
	Word4=Block4*4;
	Word1=nbytes-Word4;

	    nn=0;
	    for(n=0;n<Word4; n += 4){
	        xg.buffin[nn++]=buff[n+32];
	    }
            if(HdfPack(xg.buffin,&xg.buffout[36+4*nbytes],nn,&n1))goto ErrorOut;
	    nn=0;
	    for(n=0;n<Word4; n += 4){
	        xg.buffin[nn++]=buff[n+33];
	    }
            if(HdfPack(xg.buffin,&xg.buffout[n1+36+4*nbytes],nn,&n2))goto ErrorOut;
	    nn=0;
	    for(n=0;n<Word4; n += 4){
	        xg.buffin[nn++]=buff[n+34];
	    }
            if(HdfPack(xg.buffin,&xg.buffout[n1+n2+36+4*nbytes],nn,&n3))goto ErrorOut;
	    nn=0;
	    for(n=0;n<Word4; n += 4){
	        xg.buffin[nn++]=buff[n+35];
	    }
            if(HdfPack(xg.buffin,&xg.buffout[n1+n2+n3+36+4*nbytes],nn,&n4))goto ErrorOut;

	    Length=n1+n2+n3+n4;

	    writeBuffString4(&xg.buffout[0+4*nbytes],"PAK4");
	    writeBuffLong(&xg.buffout[4+4*nbytes],Length+7*4L);
	    writeBuffLong(&xg.buffout[8+4*nbytes],Length);
	    writeBuffLong(&xg.buffout[12+4*nbytes],Word4);
	    writeBuffLong(&xg.buffout[16+4*nbytes],n1);
	    writeBuffLong(&xg.buffout[20+4*nbytes],n2);
	    writeBuffLong(&xg.buffout[24+4*nbytes],n3);
	    writeBuffLong(&xg.buffout[28+4*nbytes],n4);

		pad[0]=0;
		pad[1]=0;
		pad[2]=0;
		pad[3]=0;
		
	    pad[0]=(unsigned char)Word1;
	    if(Word1 > 0)pad[1]=buff[Word4+32];
	    if(Word1 > 1)pad[2]=buff[Word4+33];
	    if(Word1 > 2)pad[3]=buff[Word4+34];

	    writeBuffString4(&xg.buffout[32+4*nbytes],(char *)pad);

	    if(toServer)if(netWrite(toServer,(char *)&xg.buffout[4*nbytes],Length+9*4))goto ErrorOut;

	    *nl=9*4+Length;

            if(toServer && relayDebug)fprintf(stderr,"doPAK4 in %ld out %ld\n",Word4,Length);
		

	return 0;
ErrorOut:
	return 1;
}
int writeBuffByte(unsigned char *c,unsigned int i)
{
	if(!c)return 1;
        c[0] = (unsigned char)(i & 255);
        return 0;
}
int writeBuffInt(unsigned char *c,unsigned int i)
{
 
	if(!c)return 1;
        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        return 0;
}
int writeBuffLong(unsigned char *c,unsigned long i)
{
 
	if(!c)return 1;
        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);
        return 0;
}
int writeBuffString4(unsigned char *c,char *string)
{
 
	if(!c || !string)return 1;
        c[0] = string[0];
        c[1] = string[1];
        c[2] = string[2];
        c[3] = string[3];
        return 0;
}
int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize)
{
	unsigned char *nn,*pi,*End,*pend;
	long n,Last;
	unsigned int c;
	int max,nr,nc;

	if(!image || !buff || !HdfSize)return 1;
	
	Last=ImageSize-1;
	if(image[Last] == image[Last-1]){
	    image[Last] += 1;
	    pend = &image[Last];
	}else{
	    pend = NULL;
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

	if(pend){
	    if(n){*(buff-1) -= 1;}
	    *pend  -= 1;
	}
	return 0;
}
int doUnPack(int clientSocket,struct sendData *toServer,
             int *readModeBytes,int flag,unsigned char *data)
{
	static long Compressed,UnCompressed;

	if(!toServer || !readModeBytes)return 1;

	if(flag == 0){
	    if(getLong(clientSocket,&Compressed))return 1;
	    if(getLong(clientSocket,&UnCompressed))return 1;
	    if(SetBuffers(UnCompressed+100000))return 1;
	    *readModeBytes=(int)Compressed;
	}else{
	    if(UnPack(xg.buffout,data,UnCompressed))return 1;

	    if(netWrite(toServer,(char *)xg.buffout,UnCompressed))return 1;

	    if(relayDebug){
	        fprintf(stderr,"doUnPack in %ld out %ld total %ld\n",
	                             Compressed,UnCompressed,toServer->Bytes);
	    }
	}
 
	return 0;
}
int UnPack(register unsigned char *image,register unsigned char *buff,
           register long Length)
{
	register unsigned int c;
	register long n;
	register int nr,i;

	if(!image || !buff)return 1;

	n=0;
	do{
	    c = *buff++;
	    nr=c & 0x7f;
	    if((n + nr) > Length){
		nr = (int)(Length - n);
		if(nr < 0)nr = 0;
	    }
	    if(c & 0x80){
	        c = *buff++;
	        i = -1;
	        while(++i < nr)*image++ = c;	       
	    }else{
	        i = -1;
	        while(++i < nr)*image++ = *buff++;
	    }

	}while((n += nr) < Length);

	return 0;
}
int relaySocket(int clientSocket,int toServerSocket,int buffsize)
{
	unsigned long start,total;
	time_t ship;
	char *buff;
	long nbytes;
	struct sendData toClient,toServer;
	double tstart,tend;

	fprintf(stderr,"****************************************************\n");
	fprintf(stderr,"**  relay 16.9 - COPYRIGHT SAIC 2007.       Start **\n");
	fprintf(stderr,"****************************************************\n");

	buff=NULL;

	toClient.buff=NULL;

	toServer.buff=NULL;

	buff=(char *)malloc(buffsize+128);
	if(!buff){
	    fprintf(stderr,"RelaySocket Out of Memory %d",buffsize+128);
	    return 1;
	}

	toClient.buffSize=1000000;
	toClient.buff=(char *)malloc(toClient.buffSize);
	if(!toClient.buff){
	    fprintf(stderr,"RelaySocket Out of Memory %ld",toClient.buffSize);
	    return 1;
	}
	toClient.socket=clientSocket;
	toClient.count=0;
	toClient.Bytes=0;
	toClient.BytesLast=0;

	toServer.buffSize=1000000;
	toServer.buff=(char *)malloc(toServer.buffSize);
	if(!toServer.buff){
	    fprintf(stderr,"RelaySocket Out of Memory %ld",toServer.buffSize);
	    return 1;
	}
	toServer.socket=toServerSocket;
	toServer.count=0;
	toServer.Bytes=0;
	toServer.BytesLast=0;


	start=time(&ship);
	Bytes=0;


	tstart=rtime();

	while(1){
	    nbytes=buffsize;
	    if(netReadAny(clientSocket,buff,&nbytes))goto ErrorOut;
	    if(nbytes > 0){

	        if(relayDebug){
	            fprintf(stderr,"clientSocket nbytes %ld toServer.Bytes %ld\n",
	                        nbytes,toServer.Bytes);
	        }
	    
		if(netWrite(&toServer,buff,nbytes))goto ErrorOut;

		tstart=rtime();

	    }
	    nbytes=buffsize;
	    if(netReadAny(toServerSocket,buff,&nbytes))goto ErrorOut;
	    if(nbytes > 0){
	        
		if(relayDebug){
	             fprintf(stderr,"toServerSocket nbytes %ld toClient.Bytes %ld\n",
	                        nbytes,toClient.Bytes);
		}

		if(netWrite(&toClient,buff,nbytes))goto ErrorOut;

		tstart=rtime();

	    }
	    tend=rtime();
	    if(tend > (tstart+3600.)){
	        fprintf(stderr,"Time Out No Activity for %.1f seconds\n",3600.);
		goto ErrorOut;
	    }
	}
ErrorOut:
        total=time(&ship)-start;
	if(!total)total=1;
        fprintf(stderr,"%ld Seconds To Receive %ld Bytes (%ld Bytes/s)\n",
                 total,Bytes,(Bytes/total));
	fprintf(stderr,"****************************************************\n");
	fprintf(stderr,"**  relay 16.9 - COPYRIGHT SAIC 2007.       Done  **\n");
	fprintf(stderr,"****************************************************\n");

	if(buff)free((char *)buff);

	if(toClient.buff)free((char *)toClient.buff);

	if(toServer.buff)free((char *)toServer.buff);

	return 0;
}
int connectToServer(char *serverName,unsigned short *Port)
{
	struct sockaddr_in serverSocketAddr;
	struct hostent *serverHostEnt;
	int toServerSocket,ret;
	unsigned int hostAddr;
	unsigned int oneNeg;
	short result,Try;
	char *np;
	int buf_size;

	/* oneNeg=0xffffffff; */
	oneNeg = -1L;

	buf_size=32768;

	result= -1;

	zerol((char *)&serverSocketAddr,sizeof(serverSocketAddr));

	if(!(np=strrchr(serverName,':'))){
	    printf("Bad Address (%s)",serverName);
	    return result;
	}else{
	    *np=0;
	    np += 1;
	    *Port=(unsigned short)atol(np);
	}
	hostAddr=(unsigned int)inet_addr(serverName);
	if((long)hostAddr != (long)oneNeg){
	    serverSocketAddr.sin_addr.s_addr=hostAddr;
	    printf("Found Address %lx hostAddr %x oneNeg %x diff %x\n",(long)hostAddr,hostAddr,oneNeg,hostAddr-oneNeg);
	}else{
	    serverHostEnt=gethostbyname(serverName);
	    if(serverHostEnt == NULL){
	        printf("Could Not Find Host (%s)\n",serverName);
	        return result;
	    }
	    copyl((char *)serverHostEnt->h_addr,(char *)&serverSocketAddr.sin_addr,serverHostEnt->h_length);
	}
	serverSocketAddr.sin_family=AF_INET;
	serverSocketAddr.sin_port=htons(*Port);
	Try=0;
	while(Try++ < 10){
	    if((toServerSocket=socket(AF_INET,SOCK_STREAM,0)) < 0){
                printf("socket Error  (%ld)\n",(long)errno);
	        return toServerSocket;
	    }

	    ret=setsockopt( toServerSocket, SOL_SOCKET, SO_SNDBUF, 
                  (char *)&buf_size, sizeof(int) );    
            if(ret < 0)printf("setsockopt SO_SNDBUF failed\n");
   
	    ret=connect(toServerSocket,(struct sockaddr *)&serverSocketAddr,sizeof(serverSocketAddr));
	    if(ret == -1){
                if (errno == ECONNREFUSED)  {
                    printf("Connection Refused  Try(%d)\n",Try);
                    close(toServerSocket);
                    sleep(4);
                    continue;
                }else{
                    printf("Connection Error  (%ld)\n",(long)errno);
                    return ret;
                }
	    }
	    return toServerSocket;
	}
	
       return ret;
}
int netReadAny(int clientSocket,char *buff,long *nin)
{	
	static unsigned long timep;
	time_t ship;
	long n;
	int count;

	if(!buff || !nin)return 1;

	n= *nin;

	*nin = 0;

	count=0;
	CheckSocket(clientSocket,&count,2);
	if(count <= 0)return 0;

	n=read(clientSocket,buff,n);
	if(n >= 0){
	    Bytes += n;
	    *nin = n;
	}else{
	    fprintf(stderr,"netReadAny Error Reading Socket\n");
	    return 1;        
	}

	if(time(&ship) > timep){
	   /* fprintf(stderr,"clientSocket %d n %ld *nin %ld\n",clientSocket,n, *nin); */
	   timep = time(&ship) +2;
	}
	return 0;
}
int netWrite(struct sendData *toServer,char *buffout,long HdfSize)
{
        long n,k;

        if(!toServer || !buffout)return 1;

        if(toServer->count > 0){
            n=toServer->count;
            if(n > 120000)n=120000;
            n=write(toServer->socket,toServer->buff,n);
            if(n < 0){
                fprintf(stderr,"Write Socket Error (%ld)\n",(long)errno);
                return 1;
            }
/*
           fprintf(stderr,"Socket %d bytes written %ld bytes left %ld\n",toServer->socket,n,toServer->Bytes-n);
*/       
            if(n > 0){
		toServer->count -= n;
		toServer->Bytes += n;

		for(k=0;k<toServer->count;++k){
		    toServer->buff[k]=toServer->buff[k+n];
		}
            }
        }

        if(HdfSize > 0){
            if(toServer->count > 0){
		if((toServer->count + HdfSize) > toServer->buffSize){
                    fprintf(stderr,"netWrite buffer over flow(%ld)\n",(long)(toServer->count + HdfSize));
		    return 1;
		}
		for(k=toServer->count;k<toServer->count+HdfSize;++k){
		    toServer->buff[k]=buffout[k];
		}
		toServer->count += HdfSize;
            }else{
                n=HdfSize;
                if(n > 120000)n=120000;
                n=write(toServer->socket,buffout,n);
                if(n < 0){
                    fprintf(stderr,"Write Socket Error (%ld)\n",(long)errno);
                    return 1;
                }
		toServer->Bytes += n;
                HdfSize -= n;
 /*
                fprintf(stderr,"Socket %d bytes written %ld bytes left %ld\n",toServer->socket,n,HdfSize);
 */
                if(HdfSize > 0){
                    toServer->count=HdfSize;
 		    for(k=0;k<toServer->count;++k){
		        toServer->buff[k]=buffout[k];
		    }
               }
            }
        }
	return 0;
}
double rtime(void)
{
        long milliseconds;
        long Seconds;
        double ret;


        GetTime(&Seconds,&milliseconds);

        ret=(double)Seconds+(double)milliseconds/1000.;

        return ret;

}
int GetTime(long *Seconds,long *milliseconds)
{
        struct timeval curTime;

        gettimeofday(&curTime,(struct timezone *) NULL);
        *Seconds=curTime.tv_sec;
        *milliseconds=curTime.tv_usec/1000;
        return 0;
}
int SetBuffers(long Length)
{
	if(Length < xg.buffsize)return 0;

	xg.buffsize=Length;

	if(xg.buffin)free((char *)xg.buffin);

	if(xg.buffout)free((char *)xg.buffout);

        if((xg.buffin=(unsigned char *)malloc(xg.buffsize*sizeof(char))) == NULL){
            fprintf(stderr,"SetBuffers Out of Memory %ld\n",xg.buffsize*sizeof(char));
            return 1;
        }
        if((xg.buffout=(unsigned char *)malloc(xg.buffsize*sizeof(char))) == NULL){
            fprintf(stderr,"SetBuffers Out of Memory %ld\n",xg.buffsize*sizeof(char));
            return 1;
        }
        return 0;
}

int WarningBatch(char *message)
{
	fprintf(stderr,"%s",message);
	return 0;
}
int Warning(char *message)
{
	fprintf(stderr,"%s",message);
	return 0;
}
#ifdef MAC_RESOURCES

#include <sys/attr.h>

static int makeCommand(char *command, char *file, char *Application, int length);

struct fileinfobuf {
   u_int32_t info_length;
   union {
     u_int32_t padding[8];
     struct {
       u_int32_t type;
       u_int32_t creator;
       u_int16_t fdFlags;
       u_int16_t location;
       u_int32_t padding[4];
     } info;
   } data;
};

int SetFileType(char *path,char *type)
{
   	struct attrlist alist;
  	struct fileinfobuf finfo;
	u_int32_t creatorName;
	u_int32_t typeName;
	char *crea="NTL1";
	int ret;
   	int err;
	int n;
	
	if(!path || !type)return 1;
	
	ret=1;
	
	alist.bitmapcount = 5;
	alist.reserved = 0;
	alist.commonattr = ATTR_CMN_FNDRINFO;
	alist.volattr = 0;
	alist.dirattr = 0;
	alist.fileattr = 0;
	alist.forkattr = 0;

	err = getattrlist(path, &alist, &finfo, sizeof(finfo),
	            FSOPT_NOFOLLOW);

	if(err) {
		sprintf(WarningBuff, "SetFileType Can't get file information for %s\n", path);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
		
	typeName=0;
	creatorName=0;
	for(n=0;n<4;++n){
#ifdef PC
		typeName    |= ((unsigned long)(type[n] & 255) << 8*(n));
		creatorName |= ((unsigned long)(crea[n] & 255) << 8*(n));
#else
		typeName    |= ((unsigned long)(type[n] & 255) << 8*(3-n));
		creatorName |= ((unsigned long)(crea[n] & 255) << 8*(3-n));
#endif
	}

	finfo.data.info.type=typeName;
	finfo.data.info.creator=creatorName;


	err = setattrlist(path, &alist, &finfo.data, sizeof(finfo.data), 1);
	if(err) {
		sprintf(WarningBuff, "SetFileType Error while setting file information for %s\n",path);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	
	{
		char command[4096];
		if(OpenWithApplication){
	        makeCommand(command, path, OpenWithApplication, sizeof(command));
			ret=system(command);
		}
	}
	
	ret=0;
ErrorOut:
	
	return ret;
}
static int makeCommand(char *command, char *file, char *Application, int length)
{
	if(!command || !file || !Application)return 1;
	
	zerol((char *)command,length);
	
	mstrncat(command,"/usr/bin/osascript -e 'tell application \"System Events\"' -e '  set default application of file \"",length);
	
	mstrncat(command,file,length);
	
	mstrncat(command,"\" to \"",length);
	
	mstrncat(command,Application,length);
	
	mstrncat(command,"\" ' -e 'end tell'",length);
	
	return 0;
}

#else
int SetFileType(char *path,char *type)
{
	int ret;
	
	
	if(!path || !type)return 1;
	
	
	
	ret=0;
	
	return ret;
}
#endif

int mstrncat(char *out,char *in,long n)
{
	long len;
	
	if(!out || !in || (n <= 0))return 1;
		
	len=(long)strlen(out);
	
	n -= len;
	
	if(n <= 0)return 1;
	
	out += len;
	
	while(n-- > 0){
	    if(*in == 0){
	       *out = 0;
	       break;
	    }else{
	       *out++ = *in++;
	    }
	}
	
	return 0;
}
int mstrncpy(char *out,char *in,long n)
{
	if(!out || !in || (n <= 0))return 1;
		
	while(n-- > 0){
	    if(*in == 0){
	       *out = 0;
	       break;
	    }else{
	       *out++ = *in++;
	    }
	}
	
	return 0;
}
