#ifndef __NET__
#define __NET__

#define BUFFER_SIZE 32768

struct NetWork{
	unsigned char bufferSB[BUFFER_SIZE+128];
	unsigned char *bufferS;
	unsigned char bufferRB[BUFFER_SIZE+128];
	unsigned char *bufferR;
	char ReceiveName[256];
	unsigned long ipNum;
	char ipName[256];
	long bufferCount;
	long netsize;
	long netsizeMax;
	long netcount;
	long netblock;
	short int netErrorCheck;
	struct Icon *myIcon;
	int serverSocket;
	int clientSocket;
	int toServerSocket;
	int ConnectedFlag;
	unsigned short Port;
	struct Icon *win;
	long BytesWrite;
	int iRead64;

};
#define IO_BUFFER	128000

#define noErr 0


extern struct NetWork *NetStart(struct Icon *myIcon);
extern int NetStop(struct NetWork *data);
extern int NetListenStop(struct NetWork *data);
extern int NetListenCheck(struct NetWork *data);
extern int NetListenStart(struct NetWork *data,int *localPort);
extern int netGetType(struct NetWork *data,char *buff,long *size);
extern int NetByte(struct NetWork *Net,int *xsize);
extern int NetName(struct NetWork *Net,char *name);
extern int NetCommand(struct NetWork *Net,char *command,INT8_64 *size);
extern int NetLong(struct NetWork *Net,long *xsize);
extern int NetLongLong(struct NetWork *Net,INT8_64 *xsize);
extern int NetGet(struct NetWork *Net,unsigned char *buff,long count);


extern int Flush(struct NetWork *Net,INT8_64 size);
extern int NetIntr(struct NetWork *Net,int *xsize);
extern int NetLongr(struct NetWork *Net,long *xsize);
extern int doConnection(struct Icon *myIcon,struct NetWork *Net,long dirIDToSearch,char *commandi,long size);
extern int NetTag(struct NetWork *Net);

extern int NetInt(struct NetWork *Net,int *xsize);
extern int NetClose (struct NetWork *data);
extern int UnCompress(unsigned int Esc1,unsigned int Esc2,long UnCompressed,long Compressed,unsigned char *fData,unsigned char *oData);
extern int SetFileTypeOwner(char *newpath,char *BinaryOwner,char *BinaryType);
extern int NetReadCheck(struct NetWork *Net,unsigned char *buff,long nbyte);
extern int NetRead(struct NetWork *Net,unsigned char *buff,long count);
extern int NetWrite(struct NetWork *data,unsigned char *buffer,long length);

int CheckSocket(int serverSocket,int *count);

int createService(unsigned short *Port);

int SendByte(struct NetWork *data,unsigned int n);
int SendData(struct NetWork *data,char *message,long n);
int SendLab(struct NetWork *data,char *header,long n);
int SendLong(struct NetWork *data,unsigned long n);
int SendInt(struct NetWork *Net,int nl);
int NetDump(struct NetWork *data);
long NetOpenWrite(struct NetWork *data, char *remoteHost,int remotePort);

#endif
