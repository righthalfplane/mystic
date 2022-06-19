#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "StatusWindow.h"
#include "fileSend.h"
#include "Paux.h"
#include "Net.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <setjmp.h>

#include "jpeglib.h"

#include "uDialog.h"


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


int doNetWork;
	
int NetReadU(struct NetWork *Net,unsigned char *buff,int *count);

int writeLab(struct NetWork *net,char *Type,long size);

int writeLong(struct NetWork *net,long i);

int writeInt(struct NetWork *net,int i);

int writeByte(struct NetWork *net,int i);

int writeString(struct NetWork *net,char *buffout,long HdfSize);

struct NetWork *NetStart(IconPtr myIcon);

int Synchronize(IconPtr myIconin,long CurrentFrame);
int doFiddleColors(IconPtr myIcon);


int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);

int uDrawPalette(struct paletteDraw *pd,IconPtr myIcon);

int NetStringToAddr(char *out5,long *ipNum);


int uSendPassword(struct NetWork *Net,char *SendPassword);

int isDirectory(char *name);

char *strpathname(char *path,char *name);

int strncatToPath(char *pathname,char *name,int length);


int GetJPGImage(char *name,long *xsizer,long *ysizer,unsigned char **image,int flag);
int doJPG(char *name,unsigned char *image,int ixmax,int iymax);

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

int putLong(long n,FILE *out);
int putInt(long n,FILE *out);


int SetFileTypeOwner(char *newpath,char *BinaryOwner,char *BinaryType)
{
	if(!newpath || !BinaryOwner || !BinaryType)return 1;
	return 0;
}
int NetGet(struct NetWork *Net,unsigned char *buff,INT8_64 count)
{
	
	if(Net->netErrorCheck){
	    /* return NetReadCheck(Net,(unsigned char *)buff,count); */
	}else{
	    return NetRead(Net,(unsigned char *)buff,(long)count);
	}
	
	return 1;
}
int NetLongr(struct NetWork *Net,long *xsize)
{
	unsigned char buff[4];
	
	if(NetGet(Net,buff,4L))return 1;
	*xsize = (long)( buff[3] + ((( unsigned long)buff[2]) << 8) + 
	         (((unsigned long)buff[1]) << 16) + (((unsigned long)buff[0]) << 24) );
	return 0;
}
int NetIntr(struct NetWork *Net,int *xsize)
{
	unsigned char buff[2];
	
	if(NetGet(Net,buff,2L))return 1;
	*xsize = buff[1] + (((unsigned)buff[0]) << 8);
	return 0;
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
int NetClose(struct NetWork *Net)
{
	if(!Net)return 1;

/*
	if(Net->ConnectedFlag == TRUE){
	    if(Net->serverSocket > 0){
			shutdown(Net->serverSocket,2);
			close(Net->serverSocket);    
			Net->serverSocket=0;   
		}      
	    Net->ConnectedFlag = FALSE;
	}
*/
	
    if(Net->toServerSocket > 0){
		shutdown(Net->toServerSocket,2);
		closesocket(Net->toServerSocket);    
		Net->toServerSocket=0;   
	}      
	
	return 0;
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

int NetListenCheck(struct NetWork *Net)
{
	
	if(!Net)return 1;
	if(!Net->win)return 1;

	
	if(Net->win->iaccept){
		Net->win->iaccept=0;
		Net->toServerSocket=Net->win->rd;
		return 0;
	}

	return 1;
	
}
int NetListenStart(struct NetWork *Net,int *localPort)
{
	struct sockaddr_in addr_server;
	struct Icon *win;
	char buff[256];
	int iRc;
	
	if(!Net || !localPort)return 1;
	win=Net->win;
	if(!win)return 1;
	
	NetListenStop(Net);

        win->sd = socket(PF_INET, SOCK_STREAM, 0);
        if(win->sd == INVALID_SOCKET) {
			Warning("doListen INVALID_SOCKET");
			goto ErrorOut;
        }
        
        addr_server.sin_family=PF_INET;
        addr_server.sin_port=htons((unsigned short)*localPort);       
        addr_server.sin_addr.s_addr=htonl(INADDR_ANY);
        
        iRc=bind(win->sd,(struct sockaddr far *) &addr_server,sizeof(addr_server));
        if (iRc== SOCKET_ERROR) {
			Warning("doListen bind SOCKET_ERROR");
			goto ErrorOut;
        } 

        iRc=listen(win->sd,SOMAXCONN);
        if (iRc== SOCKET_ERROR) {
			Warning("doListen listen SOCKET_ERROR");
 			goto ErrorOut;
        } 
	
 		win->iread = 0;
 		win->iwrite = 0;
		win->iconnect=0;
		win->iaccept=0;

        iRc = WSAAsyncSelect(win->sd, win->myWindow, WM_WSAASYNC, 
                             FD_ACCEPT|FD_READ|FD_CLOSE);
        if (iRc == SOCKET_ERROR)  {
			Warning("doListen WSAAsyncSelect SOCKET_ERROR");
			goto ErrorOut;
        }

		sprintf(buff,"Waiting For Connection (port:%d)",localPort);
		WriteStatus(win,buff);


ErrorOut:

	return 0;
}

int NetListenStop(struct NetWork *Net)
{
	if(!Net)return 1;
	if(!Net->win)return 1;
	
	if(Net->win->sd > 0){
	    shutdown(Net->win->sd,2);
	    closesocket(Net->win->sd);
	}
	Net->win->sd=0;
	
/*
	if(Net->serverSocket > 0){
	    shutdown(Net->serverSocket,2);
	    close(Net->serverSocket);
	}
	Net->serverSocket=0;
*/
	return 0;
}



int SendLab(struct NetWork *Net,char *header,long size)
{
	if(!Net && !header)return 1;
	return writeLab(Net,header,size);
}
int SendLong(struct NetWork *Net,unsigned long n)
{
	if(!Net)return 1;
	return writeLong(Net,(long)n);
}
int SendInt(struct NetWork *Net,int n)
{
	if(!Net)return 1;
	return writeInt(Net,n);
}
int SendByte(struct NetWork *Net,unsigned int n)
{
	if(!Net)return 1;
	return writeByte(Net,n);
}
int SendData(struct NetWork *Net,char *message,long n)
{
	if(!Net && !message)return 1;
	return writeString(Net,message,n);
}

long NetOpenWrite(struct NetWork *Net, char *remoteHost,int remotePort)
{
 	static struct sockaddr_in server_addr;
	IconPtr win;

	DWORD dwIP;
    LPHOSTENT lphp; 
	int iRc; 
	int buf_size;

	int ret;

	if(!Net || !remoteHost)return -1L;
	win=Net->win;
	if(!win)return -1L;

	buf_size=32768;

	Net->toServerSocket = -1;
	Net->hGetHostInfo = NULL;

    Net->toServerSocket = socket(AF_INET,SOCK_STREAM,0);
    if(Net->toServerSocket == INVALID_SOCKET){
		Warning("Error INVALID_SOCKET");
		goto ErrorOut;
    }        

	    ret=setsockopt( Net->toServerSocket, SOL_SOCKET, SO_SNDBUF, 
                  (char *)&buf_size, sizeof(int) );    
		if(ret < 0){
			WarningBatch("setsockopt SO_SNDBUF failed\n");
		}


    iRc = WSAAsyncSelect(Net->toServerSocket,win->myWindow,WM_WSAASYNC,
                             FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE);
    if(iRc == SOCKET_ERROR){
 		Warning("Error WSAAsyncSelect");
		goto ErrorOut;
    }

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons((short unsigned)remotePort);


            dwIP = inet_addr(remoteHost);
            if (dwIP != INADDR_NONE) {
                // have IP address 
                server_addr.sin_addr.s_addr = dwIP;
                
                iRc = connect(Net->toServerSocket, (struct sockaddr far *) &server_addr,
                                                     sizeof(server_addr));
                if (iRc == SOCKET_ERROR) {
                    if ( (iRc =WSAGetLastError()) != WSAEWOULDBLOCK) {
 						Warning("Error connect");
						goto ErrorOut;
                    }
                }            
			}else{
				Net->hGetHostInfo = LocalAlloc(LMEM_MOVEABLE,MAXGETHOSTSTRUCT);
				if (Net->hGetHostInfo == NULL) {
						Warning("Error doSend out of memory");
						goto ErrorOut;
				}

				Net->pGetHostInfo = LocalLock(Net->hGetHostInfo); 


				Net->hGetHostID = WSAAsyncGetHostByName(win->myWindow,WM_WSAGETHOST,
								 remoteHost,Net->pGetHostInfo,MAXGETHOSTSTRUCT);
				if (Net->hGetHostID == 0) {
					Warning("Error WSAAsyncGetHostByName");
					goto ErrorOut;
				}  
/*
				WriteStatusWindow(win,"Waiting For GetHostBy Name");
*/

				win->gotname=0;
				while(!CheckStatusWindow(win) && !win->gotname);
				if(win->gotname == -1 || win->gotname == 0)goto ErrorOut;
			
				lphp = (struct hostent *) Net->pGetHostInfo;
				server_addr.sin_addr.s_addr = 
									((struct in_addr *)(lphp->h_addr))->s_addr;
       
        
				iRc = connect(Net->toServerSocket, (struct sockaddr  *) &server_addr,
									   sizeof(server_addr));
				if (iRc == SOCKET_ERROR) {
					if ( (iRc =WSAGetLastError()) != WSAEWOULDBLOCK) {
						Warning("doSend connect failed");
						goto ErrorOut;
					}
				} 
             

			}



 		win->iread = 0;
 		win->iwrite = 0;
		win->iconnect=0;
		/*
		WriteStatusWindow(win,"Waiting To Connect");
		*/
		while(!CheckStatusWindow(win) && !win->iconnect);

		if(!win->iconnect)goto ErrorOut;



		return 0;


ErrorOut:
	return -1L;
}


char *strpathname(char *path,char *name)
{
	char *newpath;
	long size;
	
	if(!path || !name)return NULL;
	/*
	sprintf(WarningBuff,"strpathname %s pathname %s\n",path,name);
    WarningBatch(WarningBuff);
*/

	if(strrchr(name,'\\')){
	    sprintf(WarningBuff,"strpathname Bad pathname %s\n",name);
	    WarningBatch(WarningBuff);
	    return NULL;
	}




	size=(long)(strlen(path)+strlen(name)+16);
	newpath=cMalloc(size,7362);
	if(!newpath)return NULL;
	
	mstrncpy(newpath,path,size);
    strncatToPath(newpath,name,size);
	
	return newpath;
}
int isDirectory(char *name)
{
	WIN32_FIND_DATA file;
	HANDLE filelist;

	filelist=FindFirstFile(name,&file);
	if(filelist == INVALID_HANDLE_VALUE){
	   return 0;
	}
	
	if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)return 1;

	FindClose(filelist);


	return 0;
}
int uSendPassword(struct NetWork *Net,char *SendPassword)
{
	if(!Net || !SendPassword)return 1;
	
	return doSendPassword(Net,SendPassword);
	
}

int NetStringToAddr(char *out5,long *ipNum)
{
	if(!out5 || !ipNum)return 1;
	return 0;
}
struct NetWork *NetStart(IconPtr myIcon)
{
	char nameHost[256];
	struct hostent *host;
	struct sockaddr_in destAddr; 
	struct NetWork *Net;

	Net=(struct NetWork *)cMalloc(sizeof(struct NetWork),8176);
	if(Net == NULL){
	    WarningBatch("NetStart Memory");
	    goto ErrorOut;
	}
	zerol((char *)Net,sizeof(*Net));

	Net->myIcon=myIcon;
	Net->win=myIcon;

	nameHost[0]=0;

	gethostname(nameHost,256);
	
	host=gethostbyname(nameHost);
	
	memcpy(&(destAddr.sin_addr.s_addr), host->h_addr, host->h_length); 
		
	Net->ipNum=ntohl(destAddr.sin_addr.s_addr);

	return Net;
ErrorOut:
	if(Net)cFree((char *)Net);
	return NULL;

}
/*
int NetRead(struct NetWork *Net,unsigned char *buff,long count)
{
	int end;
		
	while(count > 0){
	   end=count;
	   if(NetReadU(Net,buff,&end)){
	       return 1;
	   }
	   buff += end;
	   count -= end;	   	    
	}
	return 0;
}
*/
int NetReadU(struct NetWork *Net,unsigned char *buff,int *count)
{
	/* int n; */
	
	if(!Net || !buff || !count)return 1;
	
/*	
	if(!CheckSocket(Net->clientSocket,&n)){
	   *count=0;
	   return 0;
	}
	
	if(n <= 0)return 0;
	
	n = *count;
	
    n=read(Net->clientSocket,buff,n);
    if(n > 0){
        *count = n;
    }else if(n == 0){
        *count = 0;
    }else{
        sprintf(WarningBuff,"NetReadU Socket Read Error\n");
        WarningBatch(WarningBuff);
        return 1;
    }
	
*/	
	return 0;
}

