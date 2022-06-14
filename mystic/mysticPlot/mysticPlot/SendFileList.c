#define EXTERN22 extern
#include "Xappl.h"
#include "fileSend.h"
#include "uDialog.h"
#include "Scanfile.h"
#include "uFiles.h"

int DFR8restart(void);
int DFR8getdims(char *name ,int *ixmax,int *iymax,int *hasPalette);
int DFR8getimage(char *name,char *image,int ixmax,int iymax,char *pal);
int DFR8lastref(void);
int DFANgetlabel(char *file,unsigned int type,unsigned int tag,char *name,int length);
int DFANgetdesc(char *file,unsigned int type,unsigned int tag,char *name,int length);

int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize);

int SendMacBinaryFile(char *name,char *pathname,netPtr s);

int getString(unsigned char *s,long n,FILE *in);

char *strpathname(char *path,char *name);

int isDirectory(char *path);

int uSendPassword(struct NetWork *Net,char *SendPassword);

int SendFileName(char *name,char *directory,netPtr s);

static int SendFile(char *name,char *directory,netPtr s);


long FileLength(FILE *out);

int SendFileList(netPtr s)
{
   	static char *mess="ERRC\0\0\0\0";
 	double start,total;
	long n,BytesWrite;
	char buff[256];
	long retn;
	int ret;
	
	if(!s || !s->Files || !s->Net)return 1;
	
	start=rtime();
	
	s->Net->win=s->myIcon;
	
	ret=1;
	
	if((retn=NetOpenWrite(s->Net,s->SendTarget,s->SendPort)) != 0){
	    sprintf(WarningBuff,"NetOpenWrite error %ld",(long)retn);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
    if(s->netErrorCheck){
        s->Net->netErrorCheck=1;
   	    if(NetWrite(s->Net,(unsigned char *)mess,8L))goto ErrorOut;
    }else{
        s->Net->netErrorCheck=0;
    }

	if(s->SendPassword[0]){
	    if(uSendPassword(s->Net,s->SendPassword))goto ErrorOut;
	}

	start=rtime();
	s->Net->BytesWrite=0;

	WriteStatus(s->Net->win,"Starting to Send");
	
	for(n=0;n<s->Files->count;++n){
	    if(SendFileName(s->Files->name[n],s->Files->directory,s))break;
	}	
		
	SendLab(s->Net,"ENDT",0L);

	ret=NetDump(s->Net);
	
ErrorOut:
	NetClose(s->Net);

	total=rtime()-start;
	if(total <= 0.0)total=1.0;
	BytesWrite=s->Net->BytesWrite;
	sprintf(buff,"%.2f Seconds To Send %ld Bytes (%.0f Bytes/s)",
			 total,BytesWrite,((double)BytesWrite/total));
	WriteStatus(s->Net->win,buff);
	
	return ret;
}

int SendFileName(char *name,char *directory,netPtr s)
{
	struct scanList *filelist;
	char *pathname;
	int ret;
	int n;
	
	if(!name || !directory || !s)return 1;
	
	ret=1;
	
	filelist=NULL;
	
	pathname=strpathname(directory,name);
	if(!pathname)goto ErrorOut;
	
	if(isDirectory(pathname)){
	    filelist=scan_dir(pathname,".*");
	    if(filelist == NULL){
	        sprintf(WarningBuff,"SendFileName: scan_dir Error\n");
	        WarningBatch(WarningBuff);
			goto ErrorOut;
	    }
		sprintf(WarningBuff,"Sending Directory %s",name);
		WriteStatus(s->Net->win,WarningBuff);
	    if(uSendDirectoryName(s->Net,name))goto ErrorOut;		    
	    for(n=0;n<filelist->countDirs;++n){
	        if(SendFileName(filelist->namesDirs[n],pathname,s))goto ErrorOut;
	    }
	    for(n=0;n<filelist->countFiles;++n){
	        if(SendFileName(filelist->namesFiles[n],pathname,s))goto ErrorOut;
	    }
	    if(SendLab(s->Net,"CATE",0L))goto ErrorOut;
	}else{
	      if(SendFile(name,directory,s))goto ErrorOut;
	}
    	
	ret=0;
ErrorOut:
    if(pathname)cFree((char *)pathname);
	if(filelist)free_dir(filelist);
	return ret;
}
static int SendTextFile(char *name,char *pathname,netPtr s)
{
	unsigned long size;
	char buff[256];
	long length;
	FILE *in;
	int ret;
	long nt,k;
	int n;
	
	if(!name || !pathname || !s)return 1;
	
	in=NULL;
	
	ret=1;

	if(SetBuffers(IO_BUFFER))goto ErrorOut;
	
	n=(int)strlen(name);
	
    in=fopen(pathname,"rb");
    if(!in){
    	sprintf(WarningBuff,"SendTextFile Error Opening (%s) To Read\n",pathname);
    	WarningBatch(WarningBuff);
    	goto ErrorOut;
    }
	length=FileLength(in);
	if(length < 0){
	    goto ErrorOut;
	}
	sprintf(buff,"Sending %s length %ld",name,length);
	WriteStatus(s->Net->win,buff);
    size=1+n+4+length;
    
    if(SendLab(s->Net,"TEXT",(long)size))goto ErrorOut;
	if(SendByte(s->Net,n))goto ErrorOut;
	if(n > 0){
	    if(SendData(s->Net,name,(long)n))goto ErrorOut;
	}
	if(SendLong(s->Net,length))goto ErrorOut;
	while(length > 0){
	    nt=length;
	    if(nt > IO_BUFFER)nt=IO_BUFFER;

	    if(getString(xg.buffin,nt,in))goto ErrorOut;
        for(k=0;k<nt;++k){
            if(xg.buffin[k] == '\r')xg.buffin[k]='\n';
        }	        
	    if(SendData(s->Net,(char *)xg.buffin,nt))goto ErrorOut;
	    length -= nt;
	}
	ret=0;
ErrorOut:	
	if(in)fclose(in);
	return ret;
}
static int HdfSend(netPtr s,char *name,char *buffout,long ixmax,long iymax,long HdfSize)
{
	long size;
	int ret;
	int n;

	if(!s || !name || !buffout)return 1;

	ret=1;
	
	n=(int)strlen(name);

	size=1+n+2*2+4+HdfSize;

	if(SendLab(s->Net,"H8CM",size))goto ErrorOut;
	
	if(SendByte(s->Net,n))goto ErrorOut;
	if(n > 0){
	    if(SendData(s->Net,name,(long)n))goto ErrorOut;
	}

	if(SendInt(s->Net,(int)ixmax))goto ErrorOut;
	
	if(SendInt(s->Net,(int)iymax))goto ErrorOut;

	if(SendLong(s->Net,HdfSize))goto ErrorOut;

	if(SendData(s->Net,buffout,HdfSize))goto ErrorOut;
	
	ret=0;
ErrorOut:
	return ret;

}
static int SendRasterFile(char *name,char *pathname,netPtr s)
{
	unsigned long size;
	char pal[768],nameout[256];
	long length,HdfSize;
	int ret;
 	int  ixmax,iymax;
	int hasPalette;
	int lastref;
	int Merge;
	
	if(!name || !pathname || !s)return 1;
		
	ret=1;

	if(SetBuffers(IO_BUFFER))goto ErrorOut;
	
	Merge=0;
	DFR8restart();
Start:
	if(DFR8getdims(name,&ixmax,&iymax,&hasPalette)){
		if(Merge == 0){
		   sprintf(WarningBuff,"SendRasterFile: DFR8getdims Error Getting Dimensions (%s)\n",pathname);
		   WarningBatch(WarningBuff);
		   return 1;
		}else{
		   return 0;
		}
	}

	length=((long)ixmax)*((long)iymax);

	if(SetBuffers(length))return 1;

	if(DFR8getimage(name,(char *)xg.buffin,ixmax,iymax,pal)){
		sprintf(WarningBuff,"SendRasterFile: DFR8getimage Error Getting Image (%s)\n",pathname);
		WarningBatch(WarningBuff);
		return 1;
	}

	lastref=DFR8lastref();
	if(lastref != -1 && 
	   DFANgetlabel(name,306,lastref,nameout,256) != -1){
	    ++Merge;
	}else{
	    mstrncpy(nameout,name,256);
	    Merge=0;
	}

	if(HdfPack((unsigned char *)xg.buffin,(unsigned char *)xg.buffout,length,&HdfSize))goto ErrorOut;

	if(HdfSend(s,nameout,(char *)xg.buffout,ixmax,iymax,HdfSize))goto ErrorOut;

	if(lastref != -1){
	   zerol((char *)nameout,256L);
	   if(DFANgetdesc(name,306,lastref,nameout,256) != -1){
	        int nn;	        
	        nn=(int)strlen(nameout);
			size=1+nn;
			if(SendLab(s->Net,"LIMI",(long)size))goto ErrorOut;
			if(SendByte(s->Net,nn))goto ErrorOut;
			if(nn > 0){
			    if(SendData(s->Net,nameout,(long)nn))goto ErrorOut;
			}
	   }
	}

	if(Merge){
	    goto Start;
	}
	
	ret=0;
ErrorOut:	
	return ret;
}
static int SendBinaryFile(char *name,char *pathname,netPtr s)
{
	unsigned long size;
	char buff[256];
	long length;
	FILE *in;
	int ret;
	long nt;
	int n;
	
	if(!name || !pathname || !s)return 1;
	
	in=NULL;
	
	ret=1;

	if(SetBuffers(IO_BUFFER))goto ErrorOut;
	
	n=(int)strlen(name);
	
    in=fopen(pathname,"rb");
    if(!in){
    	sprintf(WarningBuff,"SendBinaryFile Error Opening (%s) To Read\n",pathname);
    	WarningBatch(WarningBuff);
    	goto ErrorOut;
    }
	length=FileLength(in);
	if(length < 0){
	    goto ErrorOut;
	}
	sprintf(buff,"Sending %s length %ld",name,length);
	WriteStatus(s->Net->win,buff);
    size=1+n+4+length;
    
    if(SendLab(s->Net,"BINA",(long)size))goto ErrorOut;
	if(SendByte(s->Net,n))goto ErrorOut;
	if(n > 0){
	    if(SendData(s->Net,name,(long)n))goto ErrorOut;
	}
	if(SendLong(s->Net,length))goto ErrorOut;
	while(length > 0){
	    nt=length;
	    if(nt > IO_BUFFER)nt=IO_BUFFER;

	    if(getString(xg.buffin,nt,in))goto ErrorOut;
	    
	    if(SendData(s->Net,(char *)xg.buffin,nt))goto ErrorOut;
	    length -= nt;
	}
	ret=0;
ErrorOut:	
	if(in)fclose(in);
	return ret;
}
static int SendFile(char *name,char *directory,netPtr s)
{
	char *pathname;
	int ret;
	
	if(!name || !directory || !s)return 1;
	
	ret = 1;
	pathname=NULL;
	
	pathname=strpathname(directory,name);
	if(!pathname)goto ErrorOut;
	
	if(s->SendMode == TextFile){
	    if(SendTextFile(name,pathname,s))goto ErrorOut;		
	} else if(s->SendMode == BinaryFile){
	    if(SendBinaryFile(name,pathname,s))goto ErrorOut;		
	} else if(s->SendMode == RasterFile){
	    if(SendRasterFile(name,pathname,s))goto ErrorOut;		
	} else if(s->SendMode == MacBinaryFile){
	    if(SendMacBinaryFile(name,pathname,s))goto ErrorOut;		
	}

	ret=0;
ErrorOut:
    if(pathname)cFree((char *)pathname);
	return ret;
}

