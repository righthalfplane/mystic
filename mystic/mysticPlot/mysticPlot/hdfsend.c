#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <dirent.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/param.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <unistd.h>
/* #include <malloc.h> */

int DFerror;

struct scanFiles{
   char *name;
   int type;
};

struct scanDir{
   struct scanFiles *Files;
   char *path;
   int count;
};

char WarningBuff[256];

struct scanDir *scandirectory(char *dir);

int freedirectory(struct scanDir *scan);

extern int intcmp(const void *, const void *);

char *strsave(char *);

int checkOrder(unsigned char *cp,long length);

#define TRUE_MEMORY			4
#define TRUE_DISK			5
#define TRUE_DISK_MERGED	6

#define FILE_BLOCKS		200000

#ifndef MAXPATHLEN
#define MAXPATHLEN PATHSIZE
#endif

typedef long long INT8_64;

INT8_64 FileLength8(FILE *in);

INT8_64 Max32=2147483647LL;

int isend64;

int writeLab(int toServerSocket,char *Type,INT8_64 size);

int writeLongLong(int toServerSocket,INT8_64 i);

int Bit64Start(int toServerSocket);

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
	char protected1;
	char zero3;
	unsigned char dflen[4];
	unsigned char rflen[4];
	unsigned char cdate[4];
	unsigned char mdate[4];
};

#define HDF_TYPE			1
#define TEXT_TYPE			2
#define BINARY_TYPE			3
#define PICT_TYPE			4
#define TRUE_TYPE			5
#define MAC_BINARY_TYPE		6
#define P5_TYPE				9
#define TEK_TYPE	       		12
#define SGI_TYPE	       		14
#define GIF_TYPE	       		15
#define BMP_TYPE	       		16
#define FP32_TYPE          		17
#define JPG_TYPE          		18
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

FILE *nout,*in;
 
char fin[256],fp[256],*fe;

char *buffin,*buffout;
long buffsize;

unsigned short Port;
int toServerSocket;

INT8_64 Bytes;

int PaletteOne;
int SendDirectory;
FILE *FileOut;

char *passname;
int passwd;

int limits;

#ifdef __cplusplus
extern "C" {
#endif

    int DFSDrestart(void);
    int DFSDgetdims(char *name,int *rank,int *sizes,int maxrank);
    int DFSDgetdata(char *name,int rank,int *sizes,char *buffer);
    int DFR8getdims(char *name ,int *ixmax,int *iymax,int *hasPalet);
    int DFR8getimage(char *name,char *image,int ixmax,int iymax,char *pal);
    int DFR8restart(void);
    int DFPgetpal(char *name,char *pal);  
    int DFPrestart(void);  
    int DFR8lastref(void);
    int DFSDlastref(void);
    int DFANgetlabel(char *file,unsigned int type,unsigned int tag,char *name,int length);
    int DFANgetdesc(char *file,unsigned int type,unsigned int tag,char *name,int length);

     int socket(int domain,int  type,int  protocol);
     int shutdown(int s,int  how);
/*
     int ioctl(int d, int request, ...);
     int listen(int s,int backlog);
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

int GetJPGImage(char *name,long *xsizer,long *ysizer,unsigned char **image,int flag);
int FP32Send(int toServerSocket,char *name,char *buffout,int rank,int *dimsizes,long datasize);
int doSendRastor(int toServerSocket,char *name);
int doSendTrue(int toServerSocket,char *name);
int getString(char *s,long n,FILE *in); 
int getInt(int *n,FILE *in);
int getLong(long *n,FILE *in);
int Warning(char *message);

int connectToServer(char *serverName,unsigned short *Port);
int zerol(char *p,unsigned long n);
int copyl(char *p1,char *p2,long n);
int SetBuffers(long Length);
int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize);
int writeByte(int toServerSocket,int i);
int writeIntC(int toServerSocket,int i);
int writeInt(int toServerSocket,int i);
int writeLongC(int toServerSocket,long i);
int writeLong(int toServerSocket,long i);
int writeString(int toServerSocket,char *buffout,long HdfSize);
int HdfSend(int toServerSocket,char *name,char *buffout,long ixmax,long iymax,long HdfSize);
int doSendHdf(char *name);
int doSendFP32(int toServerSocket,char *name);
int doEnd(void);
int namesize(char *name);
int writeName(int toServerSocket,char *name);
int doSendPal(char *name);
int doSendFile(char *name,char *Type);
int compar(struct dirent **a,struct dirent **b);
int docd(char *nb,int flag);
int doSendDir(char *namedir,char *Type);
int doSendMAB2(char *name,char *Type);
int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);
int TrueSend(int toServerSocket,char *name,char *buffin,long ixmax,long iymax);
int PalSend(int toServerSocket,char *name,char *buffout);
int doSendP5(int toServerSocket,char *name);
int SendLevel(char *name,long ixmax,long iymax,float *properties,char *vbuff,char *zbuff);
void Usage(void);

int writeFileBlocks(int toServerSocket,char *name,FILE *in,INT8_64 length);
int doSendPassword(int toServerSocket,char *SendPassword);
int sendclean(char *folder);
int doexpand(unsigned char *iptr,unsigned char *optr);

#define COLSIZE 256
#define EXTRAFUDGE	128

#define	GIFGAMMA	(1.5)
#define	IMAX		((1<<16)-1)



struct GIFdataStruct{
	int global;				/* Is there a global color map? */
	int globalbits;				/* Number of bits of global colors */
	unsigned char globalmap[COLSIZE][3];	/* RGB values for global color map */
	char bgcolor;				/* background color */
	unsigned char *raster;                  /* Decoded image data */
	unsigned int width, height;
	unsigned short red[COLSIZE];
	unsigned short green[COLSIZE];
	unsigned short blue[COLSIZE];
	char *filename, *imagename;
/*
	short gamtab[256];
*/
	FILE *in;

	unsigned char *stackp;
	unsigned int prefix[4096];
	unsigned char suffix[4096];
	unsigned char stack[4096];
	int datasize,codesize,codemask;     /* Decoder working variables */
	int clear,eoi;                      /* Special code values */
	int avail, oldcode;
	int toServerSocket;
	char *name;
	int aspect;
	int gif89;
};


int doSendSGI(int toServerSocket,char *name);
int doSendGIF(int toServerSocket,char *name);
int doSendBMP(int toServerSocket,char *name);
int doSendJPG(int toServerSocket,char *name);
int LoadBmp(char *name,unsigned char **image,long *xsizer,long *ysizer);
long FileLength(FILE *in);
int getLongLow(unsigned long *n,FILE *out);
int getIntLow(unsigned int *i,FILE *in);
int flip(unsigned char *b,int xsize,int ysize);

int checksignature(FILE *in,struct GIFdataStruct *g);
int readscreen(FILE *in,struct GIFdataStruct *g);
int readextension(FILE *in);
int readgifimage(FILE *in,struct GIFdataStruct *g,char *filename);
int initcolors(unsigned char colormap[COLSIZE][3], int ncolors,struct GIFdataStruct *g);
int readraster(unsigned width,unsigned  height,struct GIFdataStruct *g);
int process(register int code,unsigned char **fill,struct GIFdataStruct *g);
int rasterize(int interleaved, register unsigned char *raster,struct GIFdataStruct *g);


int doput(unsigned char *i,unsigned char *o,long n);

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
long netsize=200000;
long netsizeMax=32768;
long netcount=0;
long netblock=0;
int netErrorCheck;


unsigned int Crc(unsigned char *s,long int n);
int NetDump(int toServerSocket);
int netWrite(int toServerSocket,char *buffout,long HdfSize);
int netPutLong(unsigned char *c,long i);
int netPutInt(unsigned char *c,int i);
int netCheck(int toServerSocket,char *message,long *block);
int netGetLong(unsigned char *c,long *n);
int netRead(int clientSocket,char *buff,long n);
int netSendBlock(char *m,long block,int clientSocket);
int CheckSocket(int serverSocket,int *count);
int Sleep(int ms);
int netFlush(int clientSocket);
int netWriteCheck(int toServerSocket,char *buffout,long HdfSize);

double rtime(void);

int main(int argc,char *argv[])
{
 
	double start,total;
	
        int n,k,type;

	fprintf(stderr,"******************************************************\n");
	fprintf(stderr,"**  hdfsend 17.0a0 - COPYRIGHT SAIC 1993-2021.      **\n");
	fprintf(stderr,"******************************************************\n");



	passname=getenv("HDFSEND_PASSWORD");
	passwd=0;

	netbuff=(unsigned char *)malloc(netsize+128);
	if(!netbuff){
	    fprintf(stderr,"Error Could Not Allocate %ld Bytes For Network Buffer\n",netsize);
	    exit(1);
	}

	FileOut=stdout;

	if(argc < 3){
	    Usage();
	}


	n=2;

	isend64=FALSE;
	limits=TRUE;
	PaletteOne=FALSE;
	SendDirectory=FALSE;
	for(k=n;k<argc;++k){
	    if(!strcmp(argv[k],"-eck")){
	         netErrorCheck=TRUE;
	    }else if(!strcmp(argv[k],"-block")){
			netsize=atoi(argv[++k]);
			if(netsize < 0 || netsize > 1000000)netsize=1000000;
			fprintf(stderr,"New Block Size = %ld\n",netsize);
	    }else if(!strcmp(argv[k],"-limits")){
	         limits=TRUE;
	    }else if(!strcmp(argv[k],"-pw")){
	         passwd=TRUE;
	    }else if(!strcmp(argv[k],"-p1")){
	         PaletteOne=TRUE;
	    }else if(!strcmp(argv[k],"-R")){
			SendDirectory=TRUE;
	    }
	}

	if(passwd){
	    if(!passname || !(*passname)){
			fprintf(stderr,"\n%s%s%s%s\n",
		       "  Warning password is NULL\n",
	               "        in csh use \n",
	               "  setenv HDFSEND_PASSWORD yourhdfpassword\n",
	               "         to set it\n");
			exit(1);
	    }
	}

	Port=3800;

	/* toServerSocket=connectToServer("198.151.8.14",Port); */

	toServerSocket=connectToServer(argv[1],&Port);
    if(toServerSocket == -1){
        printf("Connection failed to %s:%d\n",argv[1],Port);
	  	exit(1);
	}

	SetBuffers(640L*480L);

	Bytes=0;

	type=HDF_TYPE;
	
	start=rtime();

	if(netErrorCheck){
	   static char *mess="ERRC\0\0\0\0";
	   if(netWrite(toServerSocket,(char *)mess,8L))exit(1);
	}

	if(passwd){
	    if(passname && *passname){
	        if(doSendPassword(toServerSocket,passname))goto OutOfHere;
	    }else{
			fprintf(stderr,"\n%s%s%s%s\n",
		       	   "  Warning password is NULL\n",
	               "        in csh use \n",
	               "  setenv HDFSEND_PASSWORD yourhdfpassword\n",
	               "         to set it\n");
			goto OutOfHere;
	    }
	}

    while(n<argc){
	    if(!strcmp(argv[n],"-p")){
			++n;
	        doSendPal(argv[n++]);
	    }else if(!strcmp(argv[n],"-64")){
	        Bit64Start(toServerSocket);
			++n;
	    }else if(!strcmp(argv[n],"-eck")){
			++n;
	    }else if(!strcmp(argv[n],"-block")){
			n += 2;
	    }else if(!strcmp(argv[n],"-pw")){
			++n;
	    }else if(!strcmp(argv[n],"-limits")){
			++n;
	    }else if(!strcmp(argv[n],"-p1")){
	         PaletteOne=TRUE;
			++n;
	    }else if(!strcmp(argv[n],"-R")){
			++n;
	    }else if(!strcmp(argv[n],"-h")){
			type=HDF_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-pict")){
			type=PICT_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-true")){
			type=TRUE_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-sgi")){
			type=SGI_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-gif")){
			type=GIF_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-bmp")){
			type=BMP_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-p5")){
			type=P5_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-t")){
			type=TEXT_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-b")){
			type=BINARY_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-tek")){
			type=TEK_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-macb")){
			type=MAC_BINARY_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-fp32")){
			type=FP32_TYPE;
			++n;
	    }else if(!strcmp(argv[n],"-jpg")){
			type=JPG_TYPE;
			++n;
	    }else{
	        switch(type){
	            case HDF_TYPE:
					doSendRastor(toServerSocket,argv[n++]);
				break;
	            case TEXT_TYPE:
					doSendFile(argv[n++],"TEXT");
				break;
	            case BINARY_TYPE:
					doSendFile(argv[n++],"BINA");
				break;
	            case TEK_TYPE:
					doSendFile(argv[n++],"TEKB");
				break;
	            case MAC_BINARY_TYPE:
					doSendMAB2(argv[n++],"MAB2");
				break;
	            case PICT_TYPE:
					doSendFile(argv[n++],"PICT");
				break;
	            case TRUE_TYPE:
					doSendTrue(toServerSocket,argv[n++]);
				break;
	            case SGI_TYPE:
					doSendSGI(toServerSocket,argv[n++]);
				break;
	            case GIF_TYPE:
					doSendGIF(toServerSocket,argv[n++]);
				break;
	            case BMP_TYPE:
					doSendBMP(toServerSocket,argv[n++]);
				break;
	            case P5_TYPE:
					doSendP5(toServerSocket,argv[n++]);
				break;
	            case FP32_TYPE:
					doSendFP32(toServerSocket,argv[n++]);
				break;
	            case JPG_TYPE:
					doSendJPG(toServerSocket,argv[n++]);
				break;
	        }
	    } 
    }
OutOfHere:
    total=rtime()-start;
	if(!total)total=1.0;
        printf("%.2f Seconds To Send %lld Bytes (%.0f Bytes/s)\n",
                 total,Bytes,(double)(Bytes/total));

	if(toServerSocket >= 0){
	    doEnd();
	    shutdown(toServerSocket,2);
	    close(toServerSocket);
	}

	fprintf(stderr,"******************************************************\n");
	fprintf(stderr,"**  hdfsend 17.0a0 - COPYRIGHT SAIC 1993-2021. Done **\n");
	fprintf(stderr,"******************************************************\n");

	return 0;
}
void Usage(void)
{
	fprintf(stderr,"Usage : hdfsend IP:Port [options] [f1 f2 ... fn]\n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"          examples                              \n");
	fprintf(stderr,"hdfsend lanl.saic.com:3800 -p1 *.hdf            \n");
	fprintf(stderr,"hdfsend 198.151.8.99:3800 -eck -R -b -pw  E*    \n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"         [options]                              \n");
	fprintf(stderr,"         -b   (send binary files)               \n");
	fprintf(stderr,"         -t   (send text files)\n");
	fprintf(stderr,"       -tek   (send tektronix files)\n");
	fprintf(stderr,"         -h   (send Hdf Raster files (default file type))\n");
	fprintf(stderr,"      -macb   (send MacBinary files)\n");
	fprintf(stderr,"      -true   (send TrueColor files)\n");
	fprintf(stderr,"       -sgi   (send SGI RGB image files)\n");
	fprintf(stderr,"       -bmp   (send BMP image files)\n");
	fprintf(stderr,"       -gif   (send GIF 87a and 89a image files)\n");
	fprintf(stderr,"       -jpg   (send JPG image files)\n");
	fprintf(stderr,"      -fp32   (send 32-bit floating point SDS files)\n");
	fprintf(stderr,"     -p pal   (send the Hdf Rastor palette from the file called pal)\n");
	fprintf(stderr,"        -p1   (send the palette with the Hdf Raster file sent)\n");
	fprintf(stderr,"         -R   (Recursively send all files and directories (-macb or -b))\n");
	fprintf(stderr,"        -pw   (send password as indicated by the \"HDFSEND_PASSWORD\" variable)\n");
	fprintf(stderr,"       -eck   (use error checking)\n");
	fprintf(stderr,"    -limits   (send raster limits)\n");
	fprintf(stderr,"                                                \n");
	fprintf(stderr,"         [f1 f2 ... fn] - file names\n");
	exit(1);
}
int doSendJPG(int toServerSocket,char *name)
{
	unsigned char *image;
	long xsize;
	long ysize;
	long length;
	int ret;

	ret=1;

	image=NULL;

	if(GetJPGImage(name,&xsize,&ysize,&image,1))return 1;

	length=xsize*ysize*3;

	if(SetBuffers(length))goto ErroOut;

	if(TrueSend(toServerSocket,name,(char *)image,xsize,ysize))goto ErroOut;

	ret=0;
ErroOut:
	if(image)free(image);
	return ret;
}
int WarningBatch(char *WarningBuff)
{
	fprintf(stderr,"%s\n",WarningBuff);
	return 0;
}

int doSendBMP(int toServerSocket,char *name)
{
	unsigned char *image;
	long xsize;
	long ysize;
	long length;
	int ret;


	ret=1;

	image=NULL;

	if(LoadBmp(name,&image,&xsize,&ysize))return 1;

	length=xsize*ysize*3;

	if(SetBuffers(length))goto ErroOut;

	if(TrueSend(toServerSocket,name,(char *)image,xsize,ysize))goto ErroOut;

	ret=0;
ErroOut:
	if(image)free(image);
	return ret;
}
struct paldata{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char d;
};
int LoadBmp(char *name,unsigned char **image,long *xsizer,long *ysizer)
{
	unsigned long biSize,biCompression,biSizeImage,biClrUsed,biImportant;
	unsigned long ubiWidth,ubiHeight,uxPixelPerMeter,uyPixelPerMeter;
	long biWidth,biHeight,xPixelPerMeter,yPixelPerMeter,lscale;
	unsigned int biPlanes,biBitCount;
	unsigned long bfSize,bfOffset;
	unsigned int bfRes1,bfRes2;
	long xsize,ysize,length,rlen,kk;
	long FileSize;
	FILE *in;
	char BM[3];
	struct paldata palin[256];
	unsigned char *pi,*po;
	int nx,nb;
	int n,i,j;
	int ret;

	if(!name || !image || !xsizer || !ysizer)return 1;

	if(!(in=fopen(name,"rb"))){
	    fprintf(stderr,"LoadBmp Could Not Open File %s To Read\n",name);
	    return 1;
	}
	ret=1;

	FileSize=FileLength(in);
	if(FileSize <= 0)goto ErrorOut;

	/* File Header */
	
	if(getString(BM,2L,in))goto ErrorOut;

	BM[2]=0;

	if(strcmp(BM,"BM")){
	    fprintf(stderr,"%s is not a BMP file\n",name);
	    goto ErrorOut;
	}


	if(getLongLow(&bfSize,in))goto ErrorOut;

	if(getIntLow(&bfRes1,in))goto ErrorOut;

	if(getIntLow(&bfRes2,in))goto ErrorOut;

	if(getLongLow(&bfOffset,in))goto ErrorOut;

	/* BitMap Header */


	if(getLongLow(&biSize,in))goto ErrorOut;

	if(getLongLow(&ubiWidth,in))goto ErrorOut;

	if(getLongLow(&ubiHeight,in))goto ErrorOut;

	if(getIntLow(&biPlanes,in))goto ErrorOut;

	if(getIntLow(&biBitCount,in))goto ErrorOut;

	if(getLongLow(&biCompression,in))goto ErrorOut;

	if(getLongLow(&biSizeImage,in))goto ErrorOut;

	if(getLongLow(&uxPixelPerMeter,in))goto ErrorOut;

	if(getLongLow(&uyPixelPerMeter,in))goto ErrorOut;

	if(getLongLow(&biClrUsed,in))goto ErrorOut;

	if(getLongLow(&biImportant,in))goto ErrorOut;

	biWidth = (long)ubiWidth;
	biHeight = (long)ubiHeight;
	xPixelPerMeter = (long)uxPixelPerMeter;
	yPixelPerMeter = (long)uyPixelPerMeter;
	if(sizeof(long) > 4){
	    lscale=(long)(0xffffffff)+1;
	    if(ubiWidth > 0x7fffffff)biWidth=biWidth-lscale;
	    if(ubiHeight > 0x7fffffff)biHeight=biHeight-lscale;
	    if(uxPixelPerMeter > 0x7fffffff)xPixelPerMeter=xPixelPerMeter-lscale;
	    if(uyPixelPerMeter > 0x7fffffff)yPixelPerMeter=yPixelPerMeter-lscale;
	}

	if((biCompression != 0) && (biCompression != 1)){
	    fprintf(stderr,"%s Compression Type %ld Not Converted\n",name,biCompression);
	    goto ErrorOut;
	}

/*
	fprintf(stderr,"%s %ld %ld (%ld) Size %ld Bit %ld bSize %ld biClrUsed %ld\n",
	               name,biWidth,biHeight,biWidth*biHeight,biSizeImage,biBitCount,bfSize,biClrUsed);
*/

	nx=(int)((biWidth*biBitCount+7)/8);
	nx=4*((nx-1)/4+1);
	if(biBitCount <= 8){
	    int nc;
	    if(biClrUsed == 0){
	        if(biBitCount == 1){
	            nc=2;
	        }else if(biBitCount == 4){
	            nc=16;
	        }else if(biBitCount == 8){
	            nc=256;
	        }else{
	    		fprintf(stderr,"%s biBitCount %ld Error\n",name,(long)biBitCount);
	    		goto ErrorOut;
	        }
/*
	        fprintf(stderr,"t %ld - %ld - %ld - 54= %ld\n",
	            bfSize,nc*4,nx*biHeight,
	            bfSize-nc*4-nx*biHeight-54);
*/
	    }else{
	        nc=(int)biClrUsed;
/*
	        fprintf(stderr,"f %ld - %ld - %ld - 54= %ld\n",
	            bfSize,biClrUsed*4,nx*biHeight,
	            bfSize-biClrUsed*4-nx*biHeight-54);
*/
	    }
	    for(n=0;n<256;++n){
	        palin[n].r=255;
	        palin[n].g=255;
	        palin[n].b=255;
	        palin[n].d=255;
	    }
/*
	    fprintf(stderr,"%s palette %d \n",name,nc);
*/
	    for(n=0;n<nc;++n){
	        unsigned char c[4];
	        if(getString((char *)c,4L,in))goto ErrorOut;
	        palin[n].r=c[0];
	        palin[n].g=c[1];
	        palin[n].b=c[2];
	        palin[n].d=c[3];
	    }

	}

	xsize=biWidth;
	ysize=biHeight;
	length=xsize*ysize*3;
	rlen=nx*biHeight;
	if(rlen > length)length=rlen;
	if(SetBuffers(length+256))goto ErrorOut;
	if(biCompression == 1){
	    long where;
	    int en,cn;

	    where=ftell(in);
	    FileSize -= where;
	    if(biSizeImage <= 0)biSizeImage = FileSize;
	    if(biSizeImage > FileSize)biSizeImage = FileSize;

	     if(getString(buffout,(long)biSizeImage,in))goto ErrorOut;
	     zerol((char *)buffin,length);
	     i=j=0;
	     po = (unsigned char *)buffin;
	     pi = (unsigned char *)buffout;
	     for(kk=0;kk<biSizeImage;){
	        en=pi[kk++];
	        if(en == 0){
		    en=pi[kk++];
	            if(en == 0){
	               i=0;
	               ++j;
	            }else if(en == 1){
	                break;
	            }else if(en == 2){
	               i += pi[kk++];
	               j += pi[kk++];
	            }else{
	                for(n=0;n<en;++n){
	                    po[i+j*nx]=pi[kk++];
	                    if(++i >= nx){
	                        i=0;
	                        if(++j >= ysize){
	                            j=0;
	                        }
	                    }
	                }
	                if(kk & 1)++kk;
		    }
		    if(i >= nx){
	                i=0;
	                if(++j >= ysize){
	                    j=0;
	                }
		    }
	        }else{
	            cn=pi[kk++];
	            for(n=0;n<en;++n){
	                po[i+j*nx]=cn;
	                if(++i >= nx){
	                    i=0;
	                    if(++j >= ysize){
	                        j=0;
	                    }
	                }
	            }
	        }
	     }
	}else {
	     if(getString(buffin,rlen,in))goto ErrorOut;
	}
	*image=(unsigned char *)malloc(length);
	if(!(*image)){
	    fprintf(stderr,"%s Out Of Memory\n",name);
	    goto ErrorOut;
	}
	po = *image;
	pi = (unsigned char *)buffin;
	if(biBitCount == 1){
	    for(j=0;j<ysize;++j){
	        for(i=0;i<xsize;++i){
	            nb=i-8*(i/8);
	            n=(((*(pi+i/8+nx*j)) >> (7 - nb)) & 1);
	            *po++ = palin[n].r;
	            *po++ = palin[n].g;
	            *po++ = palin[n].b;
	        }
	    }
	}else if(biBitCount == 4){
	    for(j=0;j<ysize;++j){
	        for(i=0;i<xsize;++i){
	            nb=i-2*(i/2);
	            n=(((*(pi+i/2+nx*j)) >> (4 - 4*nb)) & 15);
	            *po++ = palin[n].r;
	            *po++ = palin[n].g;
	            *po++ = palin[n].b;
	        }
	    }
	}else if(biBitCount == 8){
	    for(j=0;j<ysize;++j){
	        for(i=0;i<xsize;++i){
	            n = *(pi+i+nx*j);
	            *po++ = palin[n].r;
	            *po++ = palin[n].g;
	            *po++ = palin[n].b;
	        }
	    }
	}else if(biBitCount == 24){
	    for(j=0;j<ysize;++j){
	        for(i=0;i<xsize;++i){
	            *po++ = *(pi+3*i+nx*j);
	            *po++ = *((pi+3*i+nx*j)+1);
	            *po++ = *((pi+3*i+nx*j)+2);
	        }
	    }
	}
	
	*xsizer = xsize;
	*ysizer = ysize;
	flip(*image,(int)xsize,(int)ysize);
	    
	ret=0;
ErrorOut:
	if(ret){
	    fprintf(stderr,"%s Error Reading BMP file\n",name);
	}
	if(in)fclose(in);
	return ret;
}
int flip(unsigned char *b,int xsize,int ysize)
{
	register unsigned char *p1,*p2,c1,c2,c3;
	int x,y,xinc;

	if(!b)return 1;

	xinc=xsize*3;

	for(y=0;y<ysize/2;++y){
		p1=b+y*xinc;
		p2=b+(ysize-1-y)*xinc;
		for(x=0;x<xsize;++x){
			c1 = *p1;
			c2 = *(p1+1);
			c3 = *(p1+2);
			*p1++ = *(p2+2);
			*p1++ = *(p2+1);
			*p1++ = *p2;
			*p2++ = c3;
			*p2++ = c2;
			*p2++ = c1;
		}
	}

	if(ysize & 1){
	    y=ysize/2;
	    p1=b+y*xinc;
	    for(x=0;x<xsize;++x){
		c1 = *p1;
		c2 = *(p1+1);
		c3 = *(p1+2);
		*p1++ = c3;
		*p1++ = c2;
		*p1++ = c1;
	    }

	}
	return 0;
}
long FileLength(FILE *in)
{
	long length;
	
	if(!in)goto ErrorOut;
	
	if(fseek(in,0L,2)){
	    goto ErrorOut;
	}

	if((length=ftell(in)) == -1L){
	    goto ErrorOut;
	}
	    
	if(fseek(in,0L,0)){
	    goto ErrorOut;
	}
	return length;
ErrorOut:
	return -1L;
}
int getIntLow(unsigned int *i,FILE *in)
{
      unsigned char c[2];
      unsigned int c0,c1;

        if(!i || !in)return 1;
        if(getString((char *)c,2L,in))return 1;
	c0=c[0];
	c1=c[1];
        *i = c0 + (c1 << 8);
        return 0;
}
int getLongLow(unsigned long *n,FILE *out)
{
      unsigned char c[4];
      unsigned long c0,c1,c2,c3;

        if(!n || !out)return 1;

        if(getString((char *)c,4L,out))return 1;

        c0=c[0];
        c1=c[1];
        c2=c[2];
        c3=c[3];

	*n = c0 + (c1 << 8) + (c2 << 16) + (c3 << 24);
	return 0;
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
int netWriteCheck(int toServerSocket,char *buffout,long HdfSize)
{
	long n;

	if(!buffout || !netbuff)return 1;

	for(n=0;n<HdfSize;++n){
Start:
	    if(netcount < netsize){
	        netbuff[18+netcount++]=buffout[n];
	    }else{
	        if(NetDump(toServerSocket))return 1;
	        goto Start;
	    }
	}
	return 0;
}
int netWrite(int toServerSocket,char *buffout,long HdfSize)
{
	long n;
	
	Bytes += HdfSize;
	while(HdfSize > 0){
		n=HdfSize;
		if(n > 120000)n=120000;
		n=write(toServerSocket,buffout,n);
		if(n < 0){
			fprintf(stderr,"Write Socket Error (%ld)\n",(long)errno);
			return 1;
		}
		buffout += n;
		HdfSize -= n;
	}
        
	return 0;
}
int NetDump(int toServerSocket)
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
	strcpy((char *)netbuff,"nt01data");
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
	        fprintf(stderr,"NetDump cerror (%ld)\n",(long)++cerror);
	        goto Error1;
	    }else{
	        fprintf(stderr,"NetDump Giving up on cerror\n");
	        return 1;
	    }
	}

	if(berror++ > 3){
	    fprintf(stderr,"NetDump Giving up on berror\n");
	    return 1;
	}

	if(netblock != block){
	    fprintf(stderr,"NetDump Block Miss Match netblock %ld Block %ld\n",netblock,block);
	    goto Start;
	}

	if(!strcmp(message,"ok  ")){
	   ;
	}else if(!strcmp(message,"bad ")){
	    fprintf(stderr,"NetDump block Error Re-Send Block %ld\n",netblock);
	    goto Start;
	}else{
	    netFlush(toServerSocket);
	    netSendBlock("stat",netblock,toServerSocket);
	    fprintf(stderr,"NetDump read unknown Message %s netblock %ld\n",message,netblock);
	    goto Error1;
	}
	netcount=0;
	++netblock;
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
	    Sleep(33);
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
int CheckSocket(int serverSocket,int *count)
{
       static struct timeval   timeout = {0L, 0L};
       fd_set filedes;

        FD_ZERO(&filedes);
        FD_SET(serverSocket, &filedes);

        if (select(32, &filedes, (fd_set *)NULL, (fd_set *)NULL, &timeout))  {
                ioctl(serverSocket, FIONREAD, count);
                return TRUE;
        }
	*count = 0;
        return FALSE;
}
int netSendBlock(char *m,long block,int clientSocket)
{
	unsigned char message[14];

	strcpy((char *)message,"nt01");
	strcat((char *)message,m);
	netPutLong(&message[8],block);

	if(netWrite(clientSocket,(char *)message,12L))return 1;

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
int Warning(char *message)
{
	if(!message)return 1;
	fprintf(stderr,"%s\n",message);
	return 0;
}
int doSendRastor(int toServerSocket,char *name)
{
	int ixmax,iymax;
	long length,HdfSize;
	int hasPalette;
	char pal[256*3];
	char buff[256],nameout[256];
	extern int DFerror;
	int lastref;
	
	if(PaletteOne){
		doSendPal(name);
		PaletteOne=FALSE;
	}
	
	DFR8restart();
	
	while(DFR8getdims(name,&ixmax,&iymax,&hasPalette) != -1) {
		
		length=((long)ixmax)*((long)iymax);
		
		if(SetBuffers(length))return 1;
		
		if(DFR8getimage(name,(char *)buffin,ixmax,iymax,pal)){
			sprintf(buff,"Error (%d)  DFR8getimage",DFerror);
			Warning(buff);
			return 1;
		}
		
		strcpy(nameout,name);
		
		lastref=DFR8lastref();
		if(lastref != -1) {
			DFANgetlabel(name,306,lastref,nameout,256);			
		}
		
		if(HdfPack((unsigned char *)buffin,(unsigned char *)buffout,length,&HdfSize))return 1;
		
		if(HdfSend(toServerSocket,nameout,(char *)buffout,ixmax,iymax,HdfSize))return 1;
		
		if(lastref != -1){
			zerol((char *)nameout,256L);
			if(DFANgetdesc(name,306,lastref,nameout,256) != -1){
				int size;				
				size=1+namesize(nameout);
				if(writeLab(toServerSocket,"LIMI",size))return 1;
				if(writeName(toServerSocket,nameout))return 1;
			}
		}

	}
	return 0;
}
int doSendFP32(int toServerSocket,char *name)
{
	int ixmax,iymax,izmax;
	long size;
	char buff[256],nameout[256];
	extern int DFerror;
	int lastref,rank,dimsizes[3],maxrank=3;
	
	DFSDrestart();
	
	while(DFSDgetdims(name,&rank,dimsizes,maxrank) != -1) {
		
		ixmax = dimsizes[0];
		size = ixmax;
		if (rank>1) {
			iymax = dimsizes[1];
			size *= iymax;
		}
		if (rank>2) {
			izmax = dimsizes[2];
			size *= izmax;
		}
		size *= sizeof(float);
		
		if(SetBuffers(size))return 1;
		
		if(DFSDgetdata(name,rank,dimsizes,buffin)==-1){
			sprintf(buff,"Error (%d)  DFSDgetdata",DFerror);
			Warning(buff);
			return 1;
		}
		
		strcpy(nameout,name);
				
		lastref=DFSDlastref();
		if(lastref != -1) {
			DFANgetlabel(name,700,lastref,nameout,256);			
		}
				
		if(checkOrder((unsigned char *)buffin,size))return 1;
		
		if(FP32Send(toServerSocket,nameout,(char *)buffin,
			rank,dimsizes,size))return 1;
			
		if(lastref != -1){
			zerol((char *)nameout,256L);
			if(DFANgetdesc(name,700,lastref,nameout,256) != -1){
				int size;				
				size=1+namesize(nameout);
				if(writeLab(toServerSocket,"LIMI",size))return 1;
				if(writeName(toServerSocket,nameout))return 1;
			}
		}		
		
	}
	
	return 0;
}
int checkOrder(unsigned char *cp,long length)
{
	if(!cp || length <= 0)return 1;
#ifdef PC
	{
		unsigned char c;
	    long np;
	    for(np=0;np<length;np += 4){
		    c=cp[np];
		    cp[np]=cp[np+3];
		    cp[np+3]=c;
		    c=cp[np+1];
		    cp[np+1]=cp[np+2];
		    cp[np+2]=c;
	    }
	}
#endif
	return 0;
}


int outstr(char *name)
{
    int n;

    if(!name)return 1;

    n=0;
    while(*name){
        ++n;
        putc(*name,stdout);
        if(n > 75 && *name == ' '){
            putc('\n',stdout);
            n=0;
        }
        ++name;
    }

    putc('\n',stdout);
    return 0;
}

int doSendGIF(int toServerSocket,char *name)
{
	struct GIFdataStruct g;
	int ch;
	FILE *in;
	int ret;

	ret=1;
	in=NULL;
	if(!(in=fopen(name,"rb"))){
	    fprintf(stderr,"Could Not Open (%s) To Read\n",name);
	    goto ErroOut;
	}

	zerol((char *)&g,sizeof(g));

	g.in=in;
	g.toServerSocket=toServerSocket;
	g.name=name;

	if(checksignature(in,&g))goto ErroOut;

        readscreen(in,&g);
        while ((ch = getc(in)) != ';' && ch != EOF) {
            switch (ch) {
                case '\0':  break; 
                case ',':   if (readgifimage(in,&g,name))goto ErroOut;
                        break;
                case '!':   readextension(in);
                        break;
                default:    fprintf(stderr, "unknown GIF block type\n");
                        goto ErroOut;
            }
        }


	ret=0;
ErroOut:
	if(in)fclose(in);
	return ret;
}
int readgifimage(FILE *in,struct GIFdataStruct *g,char *filename)
{
    unsigned char buf[9];
    int local, interleaved;
    unsigned char localmap[256][3];
    int localbits;

    if (fread(buf, 1, 9, in) == 0) {
        fprintf(stderr, "Error Reading %s GIF file\n",filename);
        return 1;
    }
    g->width = buf[4] + (buf[5] << 8);
    g->height = buf[6] + (buf[7] << 8);
    local = buf[8] & 0x80;
    interleaved = buf[8] & 0x40;

    if (local == 0 && g->global == 0) {
        fprintf(stderr, "no colormap present for image\n");
        return 1;
    }
    if ((g->raster = (unsigned char*) malloc(g->width*g->height+EXTRAFUDGE)) == NULL) {
        fprintf(stderr, "not enough memory for image\n");
        return 1;
    }

    if (readraster(g->width, g->height,g))
        return 1;
    if (local) {
        localbits = (buf[8] & 0x7) + 1;

        fprintf(stderr, "   local colors: %d\n", 1<<localbits);

        fread(localmap, 3, 1<<localbits, in);
        initcolors(localmap, 1<<localbits,g);
    } else if (g->global) {
        initcolors(g->globalmap, 1<<g->globalbits,g);
    }
    rasterize(interleaved, g->raster,g);
    free(g->raster);
    return 0;
}

int rasterize(int interleaved, register unsigned char *raster,struct GIFdataStruct *g)
{
    register int row,col;
    register unsigned char *rr;
    unsigned char *newras;
    long length,HdfSize;
    int c;


	length=(long)(g->width*g->height);

	if(SetBuffers(length+EXTRAFUDGE))return 1;
	newras = (unsigned char*)buffin;

    if (interleaved) {
       {		
            for (row = 0; row < g->height; row += 8) {	
                rr = newras + row*g->width;	
                for(col=0;col<g->width;++col){
                    c=raster[col];
                    if(c == 0){
                       c=255;
                    }else if(c == 255){
                       c=0;
                    }
                    rr[col]=c;
                }
                raster += g->width;	
            }
       }								
       {		
            for (row = 4; row < g->height; row += 8) {	
                rr = newras + row*g->width;	
                for(col=0;col<g->width;++col){
                    c=raster[col];
                    if(c == 0){
                       c=255;
                    }else if(c == 255){
                       c=0;
                    }
                    rr[col]=c;
                }
                raster += g->width;	
            }
       }								

       {		
            for (row = 2; row < g->height; row += 4) {	
                rr = newras + row*g->width;	
                for(col=0;col<g->width;++col){
                    c=raster[col];
                    if(c == 0){
                       c=255;
                    }else if(c == 255){
                       c=0;
                    }
                    rr[col]=c;
                }
                raster += g->width;	
            }
       }								

       {		
            for (row = 1; row < g->height; row += 2) {	
                rr = newras + row*g->width;	
                for(col=0;col<g->width;++col){
                    c=raster[col];
                    if(c == 0){
                       c=255;
                    }else if(c == 255){
                       c=0;
                    }
                    rr[col]=c;
                }
                raster += g->width;	
            }
       }								

    }else{
        for (row = 0; row < g->height; row += 1) {
            rr = newras + row*g->width;	
            for(col=0;col<g->width;++col){
                c=raster[col];
                if(c == 0){
                   c=255;
                }else if(c == 255){
                   c=0;
                }
                rr[col]=c;
            }
            raster += g->width;
        }


    }


	{
	    char pal[256*3];
	    long ni;

            for(ni=0;ni<256;++ni){
 
                pal[3*ni]=g->red[ni];
                pal[3*ni+1]=g->green[ni];
                pal[3*ni+2]=g->blue[ni];
            }
                        
            if(PalSend(g->toServerSocket,g->name,pal))return 1;
	}


	if(HdfPack((unsigned char *)buffin,(unsigned char *)buffout,length,&HdfSize))return 1;

	if(HdfSend(g->toServerSocket,g->name,(char *)buffout,(long)g->width,(long)g->height,HdfSize))return 1;

    return 0;
} 
int readraster(unsigned width,unsigned  height,struct GIFdataStruct *g)
{
    unsigned char *fill = g->raster;
    unsigned char buf[255];
    register int bits=0;
    register unsigned datum=0;
    register unsigned char *ch;
    register int count, code;

    g->datasize = getc(g->in);
    g->clear = 1 << g->datasize;
    g->eoi = g->clear + 1;
    g->avail = g->clear + 2;
    g->oldcode = -1;
    g->codesize = g->datasize + 1;
    g->codemask = (1 << g->codesize) - 1;
    for (code = 0; code < g->clear; code++) {
	g->prefix[code] = 0;
	g->suffix[code] = code;
    }
    g->stackp = g->stack;
    for (count = getc(g->in); count > 0; count = getc(g->in)) {
	fread(buf,1,count,g->in);
	for (ch=buf; count-- > 0; ch++) {
	    datum += *ch << bits;
	    bits += 8;
	    while (bits >= g->codesize) {
		code = datum & g->codemask;
		datum >>= g->codesize;
		bits -= g->codesize;
		if (code == g->eoi) {               /* This kludge put in */
#ifdef DEBUG
    fprintf(stderr, "found eoi code\n");
#endif
		    goto exitloop;               /* because some GIF files*/
		}                                /* aren't standard */
		if (process(code, &fill,g)) {
		    goto exitloop;
		}
	    }
	}
	if (fill >= g->raster + width*height) {
	    fprintf(stderr, "raster full before eoi code\n");
	    goto exitloop;
	}
    }
exitloop:
    if (fill != g->raster + width*height)  {
	fprintf(stderr, "warning: wrong rastersize: %ld bytes\n",
						      (long) (fill-g->raster));
	fprintf(stderr, "         instead of %ld bytes\n",
						      (long) width*height);
	return 0; /* can still draw a picture ... */
    }
    return 0;
}
int process(register int code,unsigned char **fill,struct GIFdataStruct *g)
{
    int incode;
    static unsigned char firstchar;

    if (code == g->clear) {
	g->codesize = g->datasize + 1;
	g->codemask = (1 << g->codesize) - 1;
	g->avail = g->clear + 2;
	g->oldcode = -1;
	return 0;
    }

    if (g->oldcode == -1) {
	*(*fill)++ = g->suffix[code];
	firstchar = g->oldcode = code;
	return 0;
    }
    if (code > g->avail) {
	fprintf(stderr, "code % d to large for %d\n", code, g->avail);
	return 1; 
    }

    incode = code;
    if (code == g->avail) {      /* the first code is always < avail */
	*g->stackp++ = firstchar;
	code = g->oldcode;
    }
    while (code > g->clear) {
	*g->stackp++ = g->suffix[code];
	code = g->prefix[code];
    }

    *g->stackp++ = firstchar = g->suffix[code];
    g->prefix[g->avail] = g->oldcode;
    g->suffix[g->avail] = firstchar;
    g->avail++;

    if (((g->avail & g->codemask) == 0) && (g->avail < 4096)) {
	g->codesize++;
	g->codemask += g->avail;
    }
    g->oldcode = incode;
    do {
	*(*fill)++ = *--g->stackp;
    } while (g->stackp > g->stack);
    return 0;
}
int initcolors(unsigned char colormap[COLSIZE][3], int ncolors,struct GIFdataStruct *g)
{
    register int i;

    for (i = 0; i < ncolors; i++) {
    /*
        g->red[i]   = g->gamtab[colormap[i][0]];
        g->green[i] = g->gamtab[colormap[i][1]];
        g->blue[i]  = g->gamtab[colormap[i][2]];
    */
        g->red[i]   = colormap[i][0];
        g->green[i] = colormap[i][1];
        g->blue[i]  = colormap[i][2];
    }
    return 0;
}
int readextension(FILE *in)
{
    int count;
    char buf[255];

    (void) getc(in);
    while ((count = getc(in)))
        fread(buf, 1, count, in);
    return 0;
}
int readscreen(FILE *in,struct GIFdataStruct *g)
{
    unsigned char buf[7];

    fread(buf,1,7,in);
    g->global = buf[4] & 0x80;
    if (g->global) {
        g->globalbits = (buf[4] & 0x07) + 1;
        fread(g->globalmap,3,1<<g->globalbits,in);
    }
    g->bgcolor = buf[5];
    g->aspect =  buf[6];

    return 0;
}

int checksignature(FILE *in,struct GIFdataStruct *g)
{
    char buf[6];

    fread(buf,1,6,in);
    if (strncmp(buf,"GIF",3)) {
        fprintf(stderr, "file is not a GIF file\n");
        return 1;
    }
    if (!strncmp(&buf[3],"87a",3)) {
       g->gif89 = 0;
    }else if (!strncmp(&buf[3],"89a",3)) {
       g->gif89 = 1;
    }else{
        fprintf(stderr, "unknown GIF version number\n");
        return 1;
    }
    return 0;
}

int doSendSGI(int toServerSocket,char *name)
{
	FILE *in;
	unsigned char *buffd,*bp;
	long length;
	int ret;
	int k,j;
/* image header start */
    int	imagic;
    int itype;
    int idim;
    int ixsize;
    int iysize;
    int izsize;
/* image header end */

	ret=1;

	in=NULL;
	buffd=NULL;

	if(!(in=fopen(name,"rb"))){
	    fprintf(stderr,"Could Not Open (%s) To Read\n",name);
	    goto ErroOut;
	}
	if(fseek(in,0L,2)){
	    fprintf(stderr,"(%s) Error Looking For End Of File\n",name);
	    goto ErroOut;
	}

	if((length=ftell(in)) == -1L){
	    fprintf(stderr,"(%s) Error Reading For End Of File Location\n",name);
	    goto ErroOut;
	}

	if(fseek(in,0L,0)){
	    fprintf(stderr,"(%s) Error Rewinding File\n",name);
	    goto ErroOut;
	}

	if(!(buffd=(unsigned char *)malloc(length))){
	    fprintf(stderr,"(%s) Error Allocating Memory  Requested (%ld)\n",name,length);
	    goto ErroOut;
	}

	if(fread(buffd,1,length,in) != length){
	    fprintf(stderr,"(%s) Error Reading File length (%ld)\n",name,length);
	    goto ErroOut;
	}

	bp=buffd;
	imagic = (((unsigned short)bp[0]) << 8) + bp[1];
	itype =  (((unsigned short)bp[2]) << 8) + bp[3];
	idim =   (((unsigned short)bp[4]) << 8) + bp[5];
	ixsize = (((unsigned short)bp[6]) << 8) + bp[7];
	iysize = (((unsigned short)bp[8]) << 8) + bp[9];
	izsize = (((unsigned short)bp[10]) << 8) + bp[11];
/*
	printf("length %ld %d %d %d %d %d %d\n",length,imagic,itype,idim,ixsize,iysize,izsize);
*/
	if(imagic != 0732){
	    fprintf(stderr,"(%s) is not a sgi file tag is (%d)\n",name,imagic);
	    goto ErroOut;
	}
	if(itype != 257 && itype != 1){
	    fprintf(stderr,"(%s) sgi file bad type is (%d)\n",name,itype);
	    goto ErroOut;
	}
	if(idim != 3){
	    fprintf(stderr,"(%s) sgi file bad idim is (%d)\n",name,idim);
	    goto ErroOut;
	}
	if(izsize != 3){
	    fprintf(stderr,"(%s) sgi file bad idim is (%d)\n",name,izsize);
	    goto ErroOut;
	}

	length=ixsize*iysize*3;

	if(SetBuffers(length))goto ErroOut;

	if(itype == 257){
	    for(k=0;k<iysize;++k){
	        for(j=0;j<3;++j){
	            unsigned long r1,c1,c2,c3,c4;
	            unsigned char *r;

	            r=buffd+512+k*4+j*iysize*4;;

	            c1=r[0];
	            c2=r[1];
	            c3=r[2];
	            c4=r[3];
                    r1 = ((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);
	           doexpand(bp+r1,(unsigned char *)buffin+(iysize-k-1)*ixsize*3+j);
	       }
   
	    }
	}else if(itype == 1){
	    for(k=0;k<iysize;++k){
	        for(j=0;j<3;++j){
	            unsigned long r1;

                    r1 = 512+k*ixsize+j*ixsize*iysize;
	            doput(bp+r1,(unsigned char *)buffin+(iysize-k-1)*ixsize*3+j,ixsize);
	       }
	    }
	}
	if(TrueSend(toServerSocket,name,buffin,ixsize,iysize))goto ErroOut;

	ret=0;
ErroOut:
	if(in)fclose(in);
	if(buffd)free(buffd);

	return ret;
}
int doput(unsigned char *i,unsigned char *o,long n)
{
	long k;

	if(!i || !o)return 1;


	for(k=0;k<n;++k){
	    *o = *i++;
	    o += 3;
	}
	return 0;
}
int doexpand(unsigned char *iptr,unsigned char *optr)
{
	unsigned char pixel;
	unsigned short count;

	while(1) {				
	    pixel = *iptr++;			
	    if ( !(count = (pixel & 0x7f)) ){	
		    return 0;			
	    }
	    if(pixel & 0x80) {			
		    while(count--){		
		        *optr = *iptr++;	
		        optr += 3;
		    }
	    } else {				
	       pixel = *iptr++;			
		   while(count--){		
		        *optr = pixel;	
		        optr += 3;
		    }
	    }					
	}
}

int doSendTrue(int toServerSocket,char *name)
{
	static char *DRt1="DRt1";
	static char *DRt2="DRt2";
	FILE *in;
	char buff[256],nameout[256];
	long length,size;
	int type;
	int ret;
	int xsize,ysize;
	unsigned int Esc1,Esc2;
	long lin,lout;

	ret=1;

	in=NULL;

	if(!(in=fopen(name,"rb"))){
	    fprintf(stderr,"Could Not Open (%s) To Read\n",name);
	    goto ErroOut;
	}

	if(getString((char *)buff,4L,in))goto ErroOut;

	buff[4]=0;

	if(!strcmp(buff,DRt1)){
		type=TRUE_DISK;
	}else if(!strcmp(buff,DRt2)){
		type=TRUE_DISK_MERGED;
	}else{
		fprintf(stderr,"File %s Skipped Not A TrueColor File (%s)(%s)(%s) \n",
				name,buff,DRt1,DRt2);
		goto ErroOut;
	}

	while(1){
		if(getInt(&xsize,in))goto ErroOut;
		if(getInt(&ysize,in))goto ErroOut;

		if(getInt((int *)&Esc1,in))goto ErroOut;
		if(getInt((int *)&Esc2,in))goto ErroOut;

		if(getLong(&lin,in))goto ErroOut;
		if(getLong(&lout,in))goto ErroOut;

		if(SetBuffers((long)lout)){
		    fprintf(stderr,"(%s) Error Allocating Memory  Requested (%ld)\n",name,lout);
		    goto ErroOut;
		}

		if(getString((char *)buffin,lout,in))goto ErroOut;

		if(type == TRUE_DISK_MERGED){
			if(getString((char *)nameout,256L,in))goto ErroOut;			
		}else{
			strcpy(nameout,name);
		}

		length=lout+4+2*2+2*4;

		size=1+namesize(nameout)+length;

		printf("%s length %ld %ld\n",nameout,lin,lout);

		if(writeLab(toServerSocket,"TRUE",size))goto ErroOut;

		if(writeName(toServerSocket,nameout))goto ErroOut;
	
		if(writeLong(toServerSocket,length))goto ErroOut;

		if(writeString(toServerSocket,DRt1,4L))goto ErroOut;

		if(writeIntC(toServerSocket,xsize))goto ErroOut;
		if(writeIntC(toServerSocket,ysize))goto ErroOut;

		if(writeIntC(toServerSocket,(int )Esc1))goto ErroOut;
		if(writeIntC(toServerSocket,(int )Esc2))goto ErroOut;

		if(writeLongC(toServerSocket,lin))goto ErroOut;
		if(writeLongC(toServerSocket,lout))goto ErroOut;

		if(writeString(toServerSocket,(char *)buffin,(long)lout))goto ErroOut;

	    if(type == TRUE_DISK_MERGED){
			if(getString((char *)buff,4L,in))goto ErroOut;
			buff[4]=0;
			if(!strcmp(buff,(char *)DRt2))continue;
	    }
	    break;
	}
	ret=0;
ErroOut:
	if(in)fclose(in);

	return ret;
}
int doSendP5(int toServerSocket,char *name)
{
	FILE *in;
	long ixmax,iymax,length,range,HdfSize;
	char pal[256*3];
	int c,ret,n,ptype,k;

	ret=1;

	if(!(in=fopen(name,"rb"))){
	    printf("Could Not Open (%s) To Read\n",name);
	    return 1;
	}
	c=getc(in);
	if(c != 'P')goto error;
	c=getc(in);

	ptype=3;
	if(c == '6'){
	    ptype=6;
	}else if(c == '5' ){
	    ptype=5;
	}else if(c != '3' )goto error;
	    c=getc(in);
	while((c=getc(in)) == '#'){
	    fscanf(in,"%s",pal);
	    n=0;
	    while((c = getc(in)) != '\n' && c != EOF && n < 256)pal[n++]=c;
	     pal[n++]='\0';
	    fprintf(stderr,"%s \n",pal);
	}

	ungetc(c,in);
	

	fscanf(in,"%ld %ld %ld",&ixmax,&iymax,&range);

	printf("ixmax %ld iymax %ld range %ld\n",ixmax,iymax,range);

	c=getc(in);
	if(c != '\n'){
	    ungetc(c,in);
	}

	if(ptype == 6 || ptype == 3){
	    length=3*ixmax*iymax;
	}else{
	    length=ixmax*iymax;
	}

	if(SetBuffers(length))return 1;

	if(ptype == 3){
	    unsigned int ir,ig,ib;
	    unsigned char *bp;
	    for(k=0;k<iymax;++k){
			bp=(unsigned char *)buffin+k*ixmax*3;
	        for(n=0;n<ixmax;++n){
				fscanf(in,"%d %d %d",&ir,&ig,&ib);
				*bp++ =ir;
				*bp++ =ig;
				*bp++ =ib;
	        }
	    }
	}else{
	    if(fread(buffin,1,length,in) != length){
	        printf("(%s) Error Reading File length (%ld)\n",name,length);
	        goto error;
	    }
	}

	if(in)fclose(in);
	in = NULL;

	if(ptype == 6 || ptype == 3){
	   return TrueSend(toServerSocket,name,buffin,ixmax,iymax);
	}

	{
	    long ni;

            for(ni=0;ni<256;++ni){
                unsigned int r,g,b;
 
                r=(unsigned int)ni;
                g=(unsigned int)ni;
                b=(unsigned int)ni;
                pal[3*ni]=r;
                pal[3*ni+1]=g;
                pal[3*ni+2]=b;
            }
            
            
            if(PalSend(toServerSocket,name,pal))return 1;

	    	PaletteOne=FALSE;
	}

	HdfSize=0;
	HdfPack((unsigned char *)buffin,(unsigned char *)buffout,length,&HdfSize);

	HdfSend(toServerSocket,name,buffout,ixmax,iymax,HdfSize);

	ret=0;
error:
	if(in)fclose(in);
	return ret;
}
int TrueSend(int toServerSocket,char *name,char *buffin,long ixmax,long iymax)
{
	unsigned int Esc1,Esc2,mask;
	long length,lout,lin,size;
	char *Type="TRUE";

	lin=3*(long)ixmax*iymax;

	mask=255;

	if(Stuff(&Esc1,&Esc2,(unsigned char *)buffin,(unsigned char *)buffout,lin,&lout,mask)){
	    printf("Stuff Error\n");
	    return 1;
	}

	printf("%s uncompressed %ld Compressed %ld\n",name,lin,lout);

	length=4+4*2+2*4+lout;

	size=1+namesize(name)+length;

	if(writeLab(toServerSocket,Type,size))return 1;


	if(writeName(toServerSocket,name))return 1;
	
	if(writeLong(toServerSocket,length))return 1;

	if(writeString(toServerSocket,"DRt1",(long)4L))return 1;

	if(writeIntC(toServerSocket,(int)ixmax))return 1;
	if(writeIntC(toServerSocket,(int)iymax))return 1;

	if(writeIntC(toServerSocket,Esc1))return 1;
	if(writeIntC(toServerSocket,Esc2))return 1;

	if(writeLongC(toServerSocket,lin))return 1;
	if(writeLongC(toServerSocket,lout))return 1;

	if(writeString(toServerSocket,buffout,(long)lout))return 1;

	return 0;
}
int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask)
{
	unsigned char entry,c1,c2,c3,*dataStart;
	unsigned int E1,E2;
	long nuse[256],n1,n2,k,kk,kkn;
	int k1,k2,n;
	
	for(n=0;n<256;++n){
	   nuse[n]=0;
	}
	
	for(k=0;k<Lin;++k){
	    dataIn[k] &= mask;
	    entry=dataIn[k];
	    ++nuse[entry];
	}
	
	n1=300000000;
	k1 = -1;
	n2=300000000;
	k2 = -1;
	for(n=0;n<256;++n){
	   k=nuse[n];
	   if(k < n1){
	       n1=k;
	       k1=n;
	   }else if(k < n2){
	       n2=k;
	       k2=n;
	   }
	   
	}
	if(k1 < 0 || k2 < 0){
	    printf("Packing Error");
	    return 1;
	}
	E1=k1;
	E2=k2;
	dataStart=dataOut;
	for(k=0;k<Lin;k += 3){
            c1=dataIn[k];
            c2=dataIn[k+1];
            c3=dataIn[k+2];
            kk=3;
            while(c1 == dataIn[k+kk] && c2 == dataIn[k+kk+1] && c3 == dataIn[k+kk+2]){
                if((kk+k) >= Lin || kk > 180000){
                    break;
                }
                kk += 3;
            }
	    	kkn=kk/3;
            if(kkn >= 2 || c1 == E1 || c1 == E2){
                 if(kkn < 255){
                    *dataOut++ = E1;
                    *dataOut++ = (unsigned char)(kkn);
                }else{
                    *dataOut++ = E2;
                    *dataOut++ = (unsigned char)(kkn & 255);
                    *dataOut++ = (unsigned char)((kkn >> 8) & 255);
                }
                k  += 3*(kkn-1);
            }
            *dataOut++ = c1;
            *dataOut++ = c2;
            *dataOut++ = c3;
	}	
	*Esc1 = E1;
	*Esc2 = E2;
	*Lout = dataOut-dataStart;
	return 0;
}
int doSendMAB2(char *name,char *Type)
{
	unsigned char *buffin,c,*bp,*bh;
	unsigned long n,k,size,dLength,rLength;
	long length,Total,sdLength,srLength;
	struct MBHead Head;
	struct stat buf;
	FILE *in;
	
	if(SendDirectory){
	    if(!stat(name,&buf) && (buf.st_mode & S_IFDIR)){
	       if(!lstat(name,&buf) && (buf.st_mode & S_IFLNK))return 0;
	       doSendDir(name,Type);
	       return 0;
	    }
	}
	if(!(in=fopen(name,"rb"))){
	    printf("Could Not Open (%s) To Read\n",name);
	    return 1;
	}

	if(fseek(in,0L,2)){
	    printf("(%s) Error Looking For End Of File\n",name);
	    fclose(in);
	    return 1;
	}

	if((length=ftell(in)) == -1L){
	    printf("(%s) Error Reading For End Of File Location\n",name);
	    fclose(in);
	    return 1;
	}

	if(fseek(in,0L,0)){
	    printf("(%s) Error Rewinding File\n",name);
	    fclose(in);
	    return 1;
	}

	if(!(buffin=(unsigned char *)malloc(length))){
	    printf("(%s) Error Allocating Memory  Requested (%ld)\n",name,length);
	    fclose(in);
	    return 1;
	}

	if(fread(buffin,1,length,in) != length){
	    printf("(%s) Error Reading File length (%ld)\n",name,length);
	    fclose(in);
	    if(buffin)free(buffin);
	    return 1;
	}

	if(in)fclose(in);

	if(length < 128){
	    printf("(%s) Error File length (%ld) Not a MacBinary File\n",name,length);
	    if(buffin)free(buffin);
	    return 1;
	}

	bp=buffin;
	bh=(unsigned char *)&Head;
	for(n=0;n<128;++n){
	    c = *bp++;
	    if(n < sizeof(struct MBHead)){
	        *bh++ = c;
	    }
	}


	dLength = (((unsigned long)Head.dflen[0]) << 24) + (((unsigned long)Head.dflen[1]) << 16) +
	          (((unsigned long)Head.dflen[2]) << 8 ) + (((unsigned long)Head.dflen[3])      );

	rLength = (((unsigned long)Head.rflen[0]) << 24) + (((unsigned long)Head.rflen[1]) << 16) +
	          (((unsigned long)Head.rflen[2]) << 8 ) + (((unsigned long)Head.rflen[3])      );

	sdLength=(long)dLength;
	srLength=(long)rLength;

	if(sdLength < 0 || srLength < 0){
	    printf("(%s) Error in MacBinary File Data (%ld) Resource (%ld)\n",name,sdLength,srLength);
	    if(buffin)free(buffin);
	    return 1;
	}

	Total = 128;

	n=(dLength+127)/128;

	Total += n*128;

	n=(rLength+127)/128;

	Total += n*128;

	if(Total > length || Total < 0){
	    printf("(%s) Error File length (%ld) is short by (%ld) bytes\n",name,length,Total-length);
	    printf("(%s) MacBinary File Data (%ld) Resource (%ld) Total (%ld) length (%ld)\n",
	           name,sdLength,srLength,Total,length);
	    if(buffin)free(buffin);
	    return 1;

	}



 	size=1+Head.nlen+dLength+rLength+16+8;

	if(writeLab(toServerSocket,Type,(long)size))return 1;

	if(writeByte(toServerSocket,Head.nlen))return 1;

	if(writeString(toServerSocket,(char *)Head.name,(long)Head.nlen))return 1;
	
	if(writeString(toServerSocket,(char *)Head.creator,4L))return 1;

	if(writeString(toServerSocket,(char *)Head.type,4L))return 1;

	if(writeString(toServerSocket,(char *)Head.cdate,4L))return 1;

	if(writeString(toServerSocket,(char *)Head.mdate,4L))return 1;

	if(writeLong(toServerSocket,(long)dLength))return 1;

	if(writeLong(toServerSocket,(long)rLength))return 1;

	if(dLength > 0){
	    if(writeString(toServerSocket,(char *)bp,(long)dLength))return 1;
	}

	n=(dLength+127)/128;
	k=n*128;

	bp += k;

	if(rLength > 0){
	    if(writeString(toServerSocket,(char *)bp,(long)rLength))return 1;
	}

	if(buffin)free(buffin);


	printf("%s %ld\n",name,length);

	return 0;
}
int writeFileBlocks(int toServerSocket,char *name,FILE *in,INT8_64 length)
{
	INT8_64 n64;
	int n;
	
	if(!in || !name)return 1;

	if(SetBuffers(FILE_BLOCKS))return 1;

	while(length > 0){
	    n64=length;
	    if(n64 > FILE_BLOCKS)n64=FILE_BLOCKS;
	    n=(int)n64;
	    if(fread(buffin,1,n,in) != n){
	        printf("(%s) Error Reading File length (%d)\n",name,n);
	        return 1;
	    }
	    if(writeString(toServerSocket,buffin,(long)n))return 1;
	    length -= n;
	}

	return 0;
}
int doSendDir(char *namedir,char *Type)
{
	struct scanDir *scan;
	char name[256];
	char working[MAXPATHLEN];
	int k,count,size;

	if(!getcwd(working,MAXPATHLEN)){
	    printf("Working directory error %s\n",working);
	    return 1;
	}

	if(docd(namedir,0))return 1;

	size=1+namesize(namedir);

	if(writeLab(toServerSocket,"CATN",size))return 1;
	if(writeName(toServerSocket,namedir))return 1;

	scan=scandirectory(".");
	if(scan == NULL){
	    printf("scandirectory error\n");
	    return 1;
	}

	count=scan->count;

	printf("doSendDir %s count %d\n",namedir,count);
	fflush(stdout);

	for(k=0;k<count;++k){
	    strcpy(name,scan->Files[k].name);
	    if(!strcmp(name,".") || !strcmp(name,".."))continue;
	    	    
	    if(!strcmp(Type,"MAB2")){
			doSendMAB2(name,Type);
	    }else if(!strcmp(Type,"TEXT")){
			doSendFile(name,Type);
	    }else if(!strcmp(Type,"BINA")){
			doSendFile(name,Type);
	    }

	}

	freedirectory(scan);

	size=0;

	if(writeLab(toServerSocket,"CATE",size))return 1;

	return docd(working,0);
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
int compar(struct dirent **a,struct dirent **b)
{
	return strcmp((*a)->d_name,(*b)->d_name);
}
int doSendFile(char *name,char *Type)
{
	FILE *in;
	long length;
	INT8_64 length8,size;
	struct stat buf;
	int ret;

	ret=1;
	in=NULL;

	if(SendDirectory){
	    if(!stat(name,&buf) && (buf.st_mode & S_IFDIR)){
	       if(!lstat(name,&buf) && (buf.st_mode & S_IFLNK))return 0;
	       doSendDir(name,Type);
	       return 0;
	    }
	}

	if(!(in=fopen(name,"rb"))){
	    printf("Could Not Open (%s) To Read\n",name);
	    goto ErrorOut;
	}
	
	length8=FileLength8(in);
	
	if(length8 < 0)goto ErrorOut;
	
	if(length8 > Max32){
		if(Bit64Start(toServerSocket))goto ErrorOut;
	}
	
	length=(long)length8;

	size=1+namesize(name)+length8+4;

	if(writeLab(toServerSocket,Type,size))goto ErrorOut;

	if(writeName(toServerSocket,name))goto ErrorOut;
	
	if(isend64){
		if(writeLongLong(toServerSocket,length8))goto ErrorOut;
	}else{	
		if(writeLong(toServerSocket,length))goto ErrorOut;
	}

	if(writeFileBlocks(toServerSocket,name,in,length8))goto ErrorOut;

	printf("%s %lld\n",name,length8);

	ret = 0;

ErrorOut:

	if(in)fclose(in);

	return ret;
}
int Bit64Start(int toServerSocket)
{
	char *bit64="BT64";
	int ret;
	
	if(isend64)return 0;
	
	ret=writeLab(toServerSocket,bit64,0L);
		
	isend64=TRUE;
			
	return ret;
}
int doSendPal(char *name)
{
	char pal[256*3];
	extern int DFerror;

	DFPrestart();
	if(DFPgetpal(name,pal)){
		printf("Error (%d)  DFPgetpal\n",DFerror);
		return 1;
	}

	if(PalSend(toServerSocket,name,pal))return 1;

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

	strcpy(buff,np);

	
	n=namesize(buff);

	sendclean(buff);

	if(writeByte(toServerSocket,n))return 1;

	if(writeString(toServerSocket,buff,(long)n))return 1;

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
int doEnd(void)
{
	char *endt;

	endt="ENDT";

	if(writeLab(toServerSocket,endt,0L))return 1;
	NetDump(toServerSocket);
	return 0;
}
int writeLab(int toServerSocket,char *Type,INT8_64 size)
{
	if(isend64){
	    if(writeString(toServerSocket,Type,4L))return 1;
	    if(writeLongLong(toServerSocket,size))return 1;
	}else{
	    if(writeString(toServerSocket,Type,4L))return 1;
	    if(writeLong(toServerSocket,(long)size))return 1;
	}
	return 0;
}
int doSendHdf(char *name)
{
	long length,HdfSize;
	int ixmax,iymax;
	int hasPalette;
	char pal[256*3];
	extern int DFerror;

	DFR8restart();

	if(DFR8getdims(name,&ixmax,&iymax,&hasPalette)){
            printf("Error (%d)  DFR8getdims\n",DFerror);
            return 1;
	}

	length=ixmax*iymax;

	if(SetBuffers(length))return 1;

	if(DFR8getimage(name,buffin,ixmax,iymax,pal)){
            printf("Error (%d)  DFR8getdims\n",DFerror);
            return 1;
	}

	if(PaletteOne){
	    
	    PalSend(toServerSocket,name,pal);

	    PaletteOne=FALSE;
	}

	HdfSize=0;
	HdfPack((unsigned char *)buffin,(unsigned char *)buffout,length,&HdfSize);

	HdfSend(toServerSocket,name,buffout,ixmax,iymax,HdfSize);

	return 0;
}
int PalSend(int toServerSocket,char *name,char *pal)
{
	char *Type;
	long size;

	if(!name || !pal)return 1;
	
	Type="PAL8";

	size=1+namesize(name)+256*3;

	if(writeLab(toServerSocket,Type,size))return 1;

	if(writeName(toServerSocket,name))return 1;

	if(writeString(toServerSocket,pal,(long)256*3))return 1;
	
	
	return 0;

}
int HdfSend(int toServerSocket,char *name,char *buffout,long ixmax,long iymax,long HdfSize)
{
	char *header;
	long length,size;
	int xmax,ymax;

	length=ixmax*iymax;
	printf("%s length %ld %ld\n",name,length,HdfSize);

	header="H8CM";

	size=1+namesize(name)+2*2+4+HdfSize;

	if(writeLab(toServerSocket,header,size))return 1;

	if(writeName(toServerSocket,name))return 1;

	xmax=(int)ixmax;
	if(writeInt(toServerSocket,xmax))return 1;

	ymax=(int)iymax;
	if(writeInt(toServerSocket,ymax))return 1;

	if(writeLong(toServerSocket,HdfSize))return 1;

	if(writeString(toServerSocket,buffout,HdfSize))return 1;
	return 0;

}
int FP32Send(int toServerSocket,char *name,char *buffout,int rank,int *dimsizes,long datasize)
{
	char *header;
	long size,length;
	int xmax,ymax,zmax;

	header="FP32";

    length = dimsizes[0];
    if (rank>1) length *= dimsizes[1];
    if (rank>2) length *= dimsizes[2];
    	
	printf("%s length %ld %ld\n",name,length,datasize);

	size=1+namesize(name)+4*4+datasize;

	if(writeLab(toServerSocket,header,size))return 1;

	if(writeName(toServerSocket,name))return 1;

	if(writeInt(toServerSocket,rank))return 1;

	xmax=(int)dimsizes[0];
	if(writeInt(toServerSocket,xmax))return 1;

	ymax=(int)dimsizes[1];
	if(writeInt(toServerSocket,ymax))return 1;

	zmax=(int)dimsizes[2];
	if(writeInt(toServerSocket,zmax))return 1;

	if(writeLong(toServerSocket,datasize))return 1;

	if(writeString(toServerSocket,buffout,datasize))return 1;
	return 0;

}
int writeString(int toServerSocket,char *buffout,long HdfSize)
{
	if(netErrorCheck){
	    return netWriteCheck(toServerSocket,buffout,HdfSize);
	}
	return netWrite(toServerSocket,buffout,HdfSize);
}
int writeLongLong(int toServerSocket,INT8_64 i)
{
	unsigned char c[8];
	
	c[0] = (unsigned char)(i & 255);
	c[1] = (unsigned char)((i >> 8) & 255);
	c[2] = (unsigned char)((i >> 16) & 255);
	c[3] = (unsigned char)((i >> 24) & 255);

	c[4] = (unsigned char)((i >> 32) & 255);
	c[5] = (unsigned char)((i >> 40) & 255);
	c[6] = (unsigned char)((i >> 48) & 255);
	c[7] = (unsigned char)((i >> 56) & 255);
	
	if(writeString(toServerSocket,(char *)c,8L))return 1;

	return 0;
}
int writeLong(int toServerSocket,long i)
{
	unsigned char c[4];
	
	c[0] = (unsigned char)(i & 255);
	c[1] = (unsigned char)((i >> 8) & 255);
	c[2] = (unsigned char)((i >> 16) & 255);
	c[3] = (unsigned char)((i >> 24) & 255);

	if(writeString(toServerSocket,(char *)c,4L))return 1;

	return 0;
}
int writeInt(int toServerSocket,int i)
{
    unsigned char c[2];
 
	c[0] = (unsigned char)(i & 255);
	c[1] = (unsigned char)((i >> 8) & 255);

	if(writeString(toServerSocket,(char *)c,2L))return 1;

	return 0;
}
int writeLongC(int toServerSocket,long i)
{
    unsigned char c[4];
 
	c[3] = (unsigned char)(i & 255);
	c[2] = (unsigned char)((i >> 8) & 255);
	c[1] = (unsigned char)((i >> 16) & 255);
	c[0] = (unsigned char)((i >> 24) & 255);

	if(writeString(toServerSocket,(char *)c,4L))return 1;

	return 0;
}
int writeIntC(int toServerSocket,int i)
{
    unsigned char c[2];
 
    c[1] = i & 255;
    c[0] = (i >> 8) & 255;

	if(writeString(toServerSocket,(char *)c,2L))return 1;

	return 0;
}
int writeByte(int toServerSocket,int i)
{
    unsigned char c[2];
 
    c[0] = i & 255;

	if(writeString(toServerSocket,(char *)c,1L))return 1;

	return 0;
}
int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize)
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
int SetBuffers(long Length)
{
	if(Length < buffsize)return 0;

	buffsize=Length;

	if(buffin)free((char *)buffin);

	if(buffout)free((char *)buffout);

        if((buffin=(char *)malloc(buffsize*sizeof(char))) == NULL){
            printf("SetBuffers Out of Memory %ld\n",buffsize*sizeof(char));
            return 1;
        }
        if((buffout=(char *)malloc(buffsize*sizeof(char))) == NULL){
            printf("SetBuffers Out of Memory %ld\n",buffsize*sizeof(char));
            return 1;
        }
        return 0;
}
int zerol(char *p,unsigned long n)
{
	if(!p || !n)return 1;

	while(n-- > 0)*p++ = 0;

	return 0;
}
int copyl(char *p1,char *p2,long n)
{
	if(!p1 || !p2)return 1;

	while(n-- > 0)*p2++ = *p1++;

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

	buf_size=(int)(netsize+30);

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
int getString(char *data,long length,FILE *out)
{
	if(!data || !out)return 1;
	if(fread(data,1,length,out) != length){
	    return 1;
	}
	return 0;
}
int getLong(long *n,FILE *in)
{
	unsigned long c1,c2,c3,c4;
	unsigned char c[4];

	if(getString((char *)c,4L,in))return 1;

	c1=c[0];
	c2=c[1];
	c3=c[2];
	c4=c[3];
    *n = (long)((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);

    return 0;
 
}
int getInt(int *n,FILE *in)
{
	unsigned long c1,c2;
	unsigned char c[2];

	if(getString((char *)c,2L,in))return 1;

	c1=c[0];
	c2=c[1];
	*n = (int)((c1 << 8) | c2);

    return 0;
}
int netRead(int clientSocket,char *buff,long n)
{
	long k;
	int isleep;

	if(!buff)return 1;

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

	zerol((char *)scan->Files,sizeof(struct scanFiles)*count);

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
INT8_64 FileLength8(FILE *in)
{
	INT8_64 length;

	if(!in)goto ErrorOut;

    if(fseeko(in,0LL,2)){
		goto ErrorOut;
    }

    if((length=ftello(in)) == -1L){
		goto ErrorOut;
    }
   
	if(fseeko(in,0LL,0)){
		goto ErrorOut;
	}
	return length;
ErrorOut:
	return (INT8_64)(-1);
}
int GetTime(long *Seconds,long *milliseconds)
{
        struct timeval curTime;

        gettimeofday(&curTime,(struct timezone *) NULL);
        *Seconds=curTime.tv_sec;
        *milliseconds=curTime.tv_usec/1000;
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
