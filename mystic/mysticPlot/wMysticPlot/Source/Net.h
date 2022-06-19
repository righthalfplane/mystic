#ifndef __NET__
#define __NET__
struct NetWork{
	unsigned char *netbuff;
	unsigned char Directory[2048];
	int toServerSocket;
	int netErrorCheck;
	long netsize;
	long netsizeMax;
	long netcount;
	long netblock;
	long WriteBytes;
	long ReadBytes;
	int Debug;
	struct Icon *win;
	char *passname;
	int passwd;
	struct Icon *rastorAppend;
	struct Icon *tekAppend;
	struct Icon *trueAppend;
	int saveRastorToFile;
	int saveTekToFile;
	int saveTrueToFile;
	unsigned char pal[3*256];
	unsigned char palname[256];
	long BytesWrite;
	unsigned long ipNum;
	struct Icon *myIcon;
	HANDLE hGetHostInfo; 
    char *pGetHostInfo;
    HANDLE hGetHostID;
	int iRead64;
};

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



int doSendPassword(struct NetWork *net,char *SendPassword);
int doSendFile(struct NetWork *net,char *name,char *Type);
int doEnd(struct NetWork *net);
int NetStart2(struct NetWork *net,int netCheck);
struct NetWork *NetStart(struct Icon *myIcon);
int NetStop(struct NetWork *Net);
int NetStartListen(struct NetWork *net,int netCheck);
int doSendFileText(struct NetWork *net,char *name,char *Type);
int doSendTrue(struct NetWork *net,char *name);
int doSendRastor(struct NetWork *net,char *name);
int doARGC(struct NetWork *net,int argc);
int doSendMAB2(struct NetWork *net,char *name,char *Type);

#define WM_WSAGETHOST WM_USER + 1
#define WM_WSAASYNC WM_USER + 2    

#define HDF_TYPE			1
#define TEXT_TYPE			2
#define BINARY_TYPE			3
#define PICT_TYPE			4
#define TRUE_TYPE			5
#define MAC_BINARY_TYPE		6
#define PTHSP_TYPE			7
#define PTPSP_TYPE			8
#define P5_TYPE				9
#define TEXT_BINARY_TYPE	10
#define LEVEL_TYPE			11
#define TEK_TYPE			12


#define IO_BUFFER		128000

#define BUFFER_SIZE		32768

extern long NetOpenWrite(struct NetWork *data, char *remoteHost,int remotePort);
extern int NetWrite(struct NetWork *data,unsigned char *buffer,long length);
extern int WriteStatus(struct Icon *myIcon,char *buff);
extern int SendLab(struct NetWork *data,char *header,long n);
extern int NetDump(struct NetWork *data);
extern int NetClose (struct NetWork *data);

extern int SendByte(struct NetWork *data,unsigned int n);
extern int SendData(struct NetWork *data,char *message,long n);
extern int SendLab(struct NetWork *data,char *header,long n);
extern int SendLong(struct NetWork *data,unsigned long n);
extern int SendInt(struct NetWork *Net,int nl);
extern int NetListenStop(struct NetWork *data);
extern int NetListenStart(struct NetWork *data,int *localPort);
extern int NetListenCheck(struct NetWork *data);
extern int netGetType(struct NetWork *data,char *buff,long *size);
extern int NetCommand(struct NetWork *Net,char *command,INT8_64 *size);
extern int NetName(struct NetWork *Net,char *name);
extern int NetClose (struct NetWork *data);
extern int Flush(struct NetWork *Net,INT8_64 size);
extern int NetLongLong(struct NetWork *Net,INT8_64 *xsize);
extern int NetLong(struct NetWork *Net,long *xsize);
extern int NetTag(struct NetWork *Net);
extern int NetIntr(struct NetWork *Net,int *xsize);
extern int NetLongr(struct NetWork *Net,long *xsize);
extern int NetGet(struct NetWork *Net,unsigned char *buff,INT8_64 count);
extern int UnCompress(unsigned int Esc1,unsigned int Esc2,long UnCompressed,long Compressed,unsigned char *fData,unsigned char *oData);
extern int NetInt(struct NetWork *Net,int *xsize);
extern int NetByte(struct NetWork *Net,int *xsize);
extern int SetFileTypeOwner(char *newpath,char *BinaryOwner,char *BinaryType);



#endif