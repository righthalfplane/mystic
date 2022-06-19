#define EXTERN22 extern
#include "Xappl.h"

#include "StatusWindow.h"

#include "Paux.h"
#include "Net.h"
#include <string.h>
#include <stdio.h>

#include "df.h"

int namesize(char *name);
int writeLab(struct NetWork *net,char *Type,long size);
int sendclean(char *folder);
int writeName(struct NetWork *net,char *name);
int writeInt(struct NetWork *net,int i);
int writeLong(struct NetWork *net,long i);
int writeString(struct NetWork *net,char *buffout,long HdfSize);
int writeByte(struct NetWork *net,int i);
int writeFileBlocks(struct NetWork *net,char *name,FILE *in,long length);

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


int PaletteOne;

#define FILE_BLOCKS		32000

int netWriteCheck(struct NetWork *net,char *buffout,long HdfSize);
int netFlush(struct NetWork *net);
int netCheck(struct NetWork *net,char *m,long *block);
int NetDump(struct NetWork *net);
unsigned int Crc(unsigned char *s,long int n);
int netPutInt(unsigned char *c,int i);
int netPutLong(unsigned char *c,long i);
int netGetLong(unsigned char *c,long *n);
int netSendBlock(char *m,long block,struct NetWork *net);
int CheckSocket(struct NetWork *net,int *count);
int NetRead(struct NetWork *net,char *buff,long n);

int getInt(int *n,FILE *in);
int getLong(long *n,FILE *in);
int getString(char *data,long length,FILE *out);
int writeIntC(struct NetWork *net,int i);
int writeLongC(struct NetWork *net,long i);

int doSendPal(struct NetWork *net,char *name);
int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize);
int HdfSend(struct NetWork *net,char *name,char *buffout,long ixmax,long iymax,long HdfSize);


#define TRUE_MEMORY		4
#define TRUE_DISK		5
#define TRUE_DISK_MERGED	6

int WriteStatus(IconPtr myIcon,char *buff);

int doSendMAB2(struct NetWork *net,char *name,char *Type)
{
	unsigned char *buffin,c,*bp,*bh;
	unsigned long n,k,size,dLength,rLength;
	long length,Total,sdLength,srLength;
	struct MBHead Head;
	char buff[256];
	/* struct stat buf; */
	FILE *in;

/*
	if(SendDirectory){
	    if(!stat(name,&buf) && (buf.st_mode & S_IFDIR)){
	       doSendDir(name,Type);
	       return 0;
	    }	   
	}
*/
	if(!(in=fopen(name,"rb"))){
	    sprintf(buff,"Could Not Open (%s) To Read",name);
		WriteStatus(net->win,buff);
	    return 1;
	}

	if(fseek(in,0L,2)){
	    sprintf(buff,"(%s) Error Looking For End Of File",name);
		WriteStatus(net->win,buff);
	    fclose(in);
	    return 1;
	}

	if((length=ftell(in)) == -1L){
	    sprintf(buff,"(%s) Error Reading For End Of File Location",name);
		WriteStatus(net->win,buff);
	    fclose(in);
	    return 1;
	}

	if(fseek(in,0L,0)){
	    sprintf(buff,"(%s) Error Rewinding File",name);
		WriteStatus(net->win,buff);
	    fclose(in);
	    return 1;
	}

	if(!(buffin=(unsigned char *)cMalloc(length,9113))){
	    sprintf(buff,"(%s) Error Allocating Memory  Requested (%ld)",name,length);
		WriteStatus(net->win,buff);
	    fclose(in);
	    return 1;
	}

	if(fread(buffin,1,length,in) != (unsigned)length){
	    sprintf(buff,"(%s) Error Reading File length (%ld)",name,length);
		WriteStatus(net->win,buff);
	    fclose(in);
	    if(buffin)free(buffin);
	    return 1;
	}

	if(in)fclose(in);

	if(length < 128){
	    sprintf(buff,"(%s) Error File length (%ld) Not a MacBinary File",name,length);
		WriteStatus(net->win,buff);
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
	    sprintf(buff,"(%s) Error in MacBinary File Data (%ld) Resource (%ld)",name,sdLength,srLength);
		WriteStatus(net->win,buff);
	    if(buffin)free(buffin);
	    return 1;
	}

	Total = 128;

	n=(dLength+127)/128;

	Total += n*128;

	n=(rLength+127)/128;

	Total += n*128;

	if(Total > length || Total < 0){
	    sprintf(buff,"(%s) Error File length (%ld) is short by (%ld) bytes",name,length,Total-length);
		WriteStatus(net->win,buff);
	    if(buffin)free(buffin);
	    return 1;

	}



 	size=1+Head.nlen+dLength+rLength+16+8;

	if(writeLab(net,Type,(long)size))return 1;

	if(writeByte(net,Head.nlen))return 1;

	if(writeString(net,(char *)Head.name,(long)Head.nlen))return 1;
	
	if(writeString(net,(char *)Head.creator,4L))return 1;

	if(writeString(net,(char *)Head.type,4L))return 1;

	if(writeString(net,(char *)Head.cdate,4L))return 1;

	if(writeString(net,(char *)Head.mdate,4L))return 1;

	if(writeLong(net,(long)dLength))return 1;

	if(writeLong(net,(long)rLength))return 1;

	if(dLength > 0){
	    if(writeString(net,(char *)bp,(long)dLength))return 1;
	}

	n=(dLength+127)/128;
	k=n*128;

	bp += k;

	if(rLength > 0){
	    if(writeString(net,(char *)bp,(long)rLength))return 1;
	}

	if(buffin)cFree(buffin);


	sprintf(buff,"%s %ld",name,length);
	WriteStatus(net->win,buff);

	return 0;
}

int doARGC(struct NetWork *net,int argc)
{
	char *endt;
	long size;

	endt="ARGC";

	size=2;
	if(writeLab(net,endt,size))return 1;
	if(writeInt(net,argc))return 1;
	return 0;
}

int doSendRastor(struct NetWork *net,char *name)
{
	int ixmax,iymax;
	long length,HdfSize;
	int hasPalette;
	char pal[256*3];
	char buff[256],nameout[256];
	extern int DFerror;
	int lastref;
	int Merge;

	if(PaletteOne){
	    doSendPal(net,name);
	    PaletteOne=FALSE;
	}

	Merge=0;
	DFR8restart();
Start:
	if(DFR8getdims(name,&ixmax,&iymax,&hasPalette)){
	    if(Merge == 0){
               sprintf(buff,"Error (%d)  DFR8getdims",DFerror);
               Warning(buff);
               return 1;
            }else{
               return 0;
            }
	}

	length=((long)ixmax)*((long)iymax);

	if(SetBuffers(length))return 1;

	if(DFR8getimage(name,(char *)xg.buffin,ixmax,iymax,pal)){
            sprintf(buff,"Error (%d)  DFR8getdims",DFerror);
            Warning(buff);
            return 1;
	}

	lastref=DFR8lastref();
	if(lastref != -1 && 
	   DFANgetlabel(name,306,lastref,nameout,256) != -1){
	    ++Merge;
	}else{
	    strcpy(nameout,name);
	    Merge=0;
	}

	if(HdfPack((unsigned char *)xg.buffin,(unsigned char *)xg.buffout,length,&HdfSize))return 1;

	if(HdfSend(net,nameout,(char *)xg.buffout,ixmax,iymax,HdfSize))return 1;

	if(Merge){
	    goto Start;
	}
	return 0;
}
int HdfSend(struct NetWork *net,char *name,char *buffout,long ixmax,long iymax,long HdfSize)
{
	char buff[256];
	char *header;
	long length,size;
	int xmax,ymax;

	length=ixmax*iymax;
	sprintf(buff,"%s length %ld %ld",name,length,HdfSize);
	WriteStatus(net->win,buff);

	header="H8CM";

	size=1+namesize(name)+2*2+4+HdfSize;

	if(writeLab(net,header,size))return 1;

	if(writeName(net,name))return 1;

	xmax=(int)ixmax;
	if(writeInt(net,xmax))return 1;

	ymax=(int)iymax;
	if(writeInt(net,ymax))return 1;

	if(writeLong(net,HdfSize))return 1;

	if(writeString(net,buffout,HdfSize))return 1;
	return 0;

}
int doSendPal(struct NetWork *net,char *name)
{
	char pal[256*3],buff[256];
	extern int DFerror;
	char *Type;
	long size;

	DFPrestart();
	if(DFPgetpal(name,pal)){
        sprintf(buff,"Error (%d)  DFPgetpal",DFerror);
		WriteStatus(net->win,buff);
        return 1;
	}

	Type="PAL8";

	size=1+namesize(name)+256*3;

	if(writeLab(net,Type,size))return 1;

	if(writeName(net,name))return 1;
	
	if(writeString(net,pal,(long)256*3))return 1;
	return 0;
}
int doSendTrue(struct NetWork *net,char *name)
{
	static char *DRt1="DRt1";
	static char *DRt2="DRt2";
	FILE *in;
	char mess[256],nameout[256],buff[256];
	long length,size;
	int type;
	int ret;
	int xsize,ysize;
	unsigned int Esc1,Esc2;
	long lin,lout;

	ret=1;

	in=NULL;

	if(!(in=fopen(name,"rb"))){
	    sprintf(buff,"Could Not Open (%s) To Read",name);
		WriteStatus(net->win,buff);
	    goto ErroOut;
	}

	    if(getString((char *)mess,4L,in))goto ErroOut;

	    mess[4]=0;

	    if(!strcmp(mess,DRt1)){
	        type=TRUE_DISK;
	    }else if(!strcmp(mess,DRt2)){
	        type=TRUE_DISK_MERGED;
	    }else{
	        sprintf(buff,"File %s Skipped Not A TrueColor File (%s)(%s)(%s)",
	                name,mess,DRt1,DRt2);
			WriteStatus(net->win,buff);
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
		    sprintf(buff,"(%s) Error Allocating Memory  Requested (%ld)",name,lout);
			WriteStatus(net->win,buff);
		    goto ErroOut;
		}

		if(getString((char *)xg.buffin,lout,in))goto ErroOut;

	        if(type == TRUE_DISK_MERGED){
		    if(getString((char *)nameout,256L,in))goto ErroOut;			
	        }else{
	            strcpy(nameout,name);
	        }

		length=lout+4+2*2+2*4;

		size=1+namesize(nameout)+length;

		sprintf(buff,"%s length %ld %ld",nameout,lin,lout);
		WriteStatus(net->win,buff);

		if(writeLab(net,"TRUE",size))goto ErroOut;

		if(writeName(net,nameout))goto ErroOut;
	
		if(writeLong(net,length))goto ErroOut;

		if(writeString(net,DRt1,4L))goto ErroOut;

		if(writeIntC(net,xsize))goto ErroOut;
		if(writeIntC(net,ysize))goto ErroOut;

		if(writeIntC(net,(int )Esc1))goto ErroOut;
		if(writeIntC(net,(int )Esc2))goto ErroOut;

		if(writeLongC(net,lin))goto ErroOut;
		if(writeLongC(net,lout))goto ErroOut;

		if(writeString(net,(char *)xg.buffin,(long)lout))goto ErroOut;

	    if(type == TRUE_DISK_MERGED){
		if(getString((char *)mess,4L,in))goto ErroOut;
		mess[4]=0;
		if(!strcmp(mess,(char *)DRt2))continue;
	    }
	    break;
	}
	ret=0;
ErroOut:
	if(in)fclose(in);

	return ret;
}
int writeLongC(struct NetWork *net,long i)
{
      unsigned char c[4];
 
        c[3] = (unsigned char)(i & 255);
        c[2] = (unsigned char)((i >> 8) & 255);
        c[1] = (unsigned char)((i >> 16) & 255);
        c[0] = (unsigned char)((i >> 24) & 255);

	if(writeString(net,(char *)c,4L))return 1;

	return 0;
}
int writeIntC(struct NetWork *net,int i)
{
      unsigned char c[2];
 
        c[1] = i & 255;
        c[0] = (i >> 8) & 255;

	if(writeString(net,(char *)c,2L))return 1;

	return 0;
}
int doSendFileText(struct NetWork *net,char *name,char *Type)
{
	char buff[256];
	FILE *in;
	long length,lengtht,n,k,size,ks;
/*
	struct stat buf;
*/
	int ret;

	ret=1;
	in=NULL;
/*
	if(SendDirectory){
	    if(!stat(name,&buf) && (buf.st_mode & S_IFDIR)){
	       doSendDir(name,Type);
	       return 0;
	    }	   
	}
*/
	if(!(in=fopen(name,"rb"))){
	    sprintf(buff,"Could Not Open (%s) To Read",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	if(fseek(in,0L,2)){
	    sprintf(buff,"(%s) Error Looking For End Of File",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	if((length=ftell(in)) == -1L){
	    sprintf(buff,"(%s) Error Reading End Of File Location",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	if(fseek(in,0L,0)){
	    sprintf(buff,"(%s) Error Rewinding File",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	lengtht=0;
	for(n=0;n<length;++n){
		if(fgetc(in) == '\r')continue;
		++lengtht;
	}

	if(fseek(in,0L,0)){
	    sprintf(buff,"(%s) Error Rewinding File",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}


	size=1+namesize(name)+lengtht+4;

	if(writeLab(net,Type,size))goto ErrorOut;

	if(writeName(net,name))goto ErrorOut;
	
	if(writeLong(net,lengtht))goto ErrorOut;

	if(SetBuffers(FILE_BLOCKS))goto ErrorOut;

	while(length > 0){
		unsigned char *p;
	    n=length;
	    if(n > FILE_BLOCKS)n=FILE_BLOCKS;
	    if(fread(xg.buffin,1,n,in) != (unsigned)n){
	        sprintf(buff,"(%s) Error Reading File length (%ld)",name,n);
			WriteStatus(net->win,buff);
	        return 1;
	    }
		ks=0;
		p=xg.buffin;
		for(k=0;k<n;++k){
			if(p[k] != '\r'){
				p[ks++]=p[k];
			}
		}
		
	    if(writeString(net,xg.buffin,(long)ks))return 1;
	    length -= n;
	}


	ret = 0;

ErrorOut:

	if(in)fclose(in);

	return ret;
}

int NetStart2(struct NetWork *net,int netCheck)
{
	char buff[256];

	net->netsize=32768;
	net->netsizeMax=32768;
	net->netcount=0;
	net->netblock=0;
	net->ReadBytes=0;
	net->WriteBytes=0;
	net->netErrorCheck=netCheck;

	if(!net->netbuff){
		net->netbuff=(unsigned char *)cMalloc(net->netsize+128,9114);
		if(!net->netbuff){
			sprintf(buff,"Error Could Not Allocate %ld Bytes For Network Buffer",net->netsize);
			Warning(buff);
			return 1;
		}
	}

	if(net->netErrorCheck){
	   static char *mess="ERRC\0\0\0\0";
	   if(NetWrite(net,(unsigned char *)mess,8L))return 1;
	}



	return 0;

}
int NetRead(struct NetWork *net,char *buff,long n)
{
	
	IconPtr win;
	long k;
	int isleep,ierror;

	if(!net || !buff)return 1;

	win=net->win;

	isleep=0;

	net->ReadBytes += n;

	k=n;
	while(k > 0){
	    n=k;
	    n=recv(net->toServerSocket,buff,n,0);
	    if(n > 0){
	        k -= n;
	        buff += n;
		isleep=0;
	    }else if(n == 0){
	        if(++isleep > 20){
	            Warning("netRead Time Out Error");
	            return 1;
	        }else{
	            Sleep2(100L);
	        }
	    }else{
			ierror=WSAGetLastError();
			if((ierror == WSAEWOULDBLOCK) && win){
				if(CheckStatusWindow(win)){
					Warning("break in netRead");
					return 1;
				}
				Sleep2(1);
				continue;
			}else{
				WarningBatch("netRead Error Reading Socket\n");
			}
			return 1;        
	    }
	}
	return 0;
}
int netSendBlock(char *m,long block,struct NetWork *net)
{
	unsigned char message[14];

	mstrncpy((char *)message,"nt01",14);
	mstrncat((char *)message,m,14);
	netPutLong(&message[8],block);

	if(NetWrite(net,(unsigned char *)message,12L))return 1;

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
int NetDump(struct NetWork *net)
{
	unsigned int crcval;
	char message[5],buff[256];
	long block;
	int cerror,berror;
/*
	static long err,save;
*/
	if(!net->netbuff)return 1;
	if(net->netcount <= 0)return 0;


	crcval=Crc(&net->netbuff[18],net->netcount);
	mstrncpy((char *)net->netbuff,"nt01data",18);
	netPutLong(&net->netbuff[8],net->netblock);
	netPutLong(&net->netbuff[12],net->netcount);
	netPutInt(&net->netbuff[16],crcval);

/*
	++err;

	if(!(err % 10)){
	    save=netbuff[9];
	    netbuff[9]=77;
	}
*/
	berror=0;
Start:
	if(NetWrite(net,(unsigned char *)net->netbuff,18+net->netcount))return 1;
/*
	if(!(err % 10) )netbuff[9]=save;
*/

	cerror=0;
Error1:


	if(netCheck(net,message,&block)){
	    if(cerror < 3){
		netFlush(net);
		netSendBlock("stat",net->netblock,net);
	        sprintf(buff,"NetDump cerror (%ld)",(long)++cerror);
			WriteStatus(net->win,buff);
	        goto Error1;
	    }else{
	        sprintf(buff,"NetDump Giving up on cerror");
			WriteStatus(net->win,buff);
	        return 1;
	    }
	}

	if(berror++ > 3){
	    sprintf(buff,"NetDump Giving up on berror");
		WriteStatus(net->win,buff);
	    return 1;
	}

	if(net->netblock != block){
	    sprintf(buff,"NetDump Block Miss Match netblock %ld Block %ld",net->netblock,block);
		WriteStatus(net->win,buff);
	    goto Start;
	}

	if(!strcmp(message,"ok  ")){
	   ;
	}else if(!strcmp(message,"bad ")){
	    sprintf(buff,"NetDump block Error Re-Send Block %ld",net->netblock);
		WriteStatus(net->win,buff);
	    goto Start;
	}else{
	    netFlush(net);
	    netSendBlock("stat",net->netblock,net);
	    sprintf(buff,"NetDump read unknown Message %s netblock %ld",message,net->netblock);
		WriteStatus(net->win,buff);
	    goto Error1;
	}
	net->netcount=0;
	++net->netblock;
	return 0;
}
int netCheck(struct NetWork *net,char *m,long *block)
{
	unsigned char d[12];

	if(!m || !block)return 1;

	if(NetRead(net,(char *)d,12L))return 1;

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
int netFlush(struct NetWork *net)
{
	int count,k;
	char buff[256];

Start:
	if(CheckSocket(net,&count)){
	    if(count <= 0)return 0;
	    sprintf(buff,"netFlush skipping %d bytes",count);
		WriteStatus(net->win,buff);
	    do{
		k=count;
		if(k > net->netsizeMax)k = (int)net->netsizeMax;
		if(NetRead(net,(char *)net->netbuff,k))return 1;
		count -= k;
	    }while(count > 0);
	    Sleep2(33);
	    goto Start;
	}
	return 0;
}
int CheckSocket(struct NetWork *net,int *count)
{
  static struct timeval   timeout = {0L, 0L};
  fd_set filedes;
  
  FD_ZERO(&filedes);
  FD_SET(net->toServerSocket, &filedes);
  
  if (select(32, &filedes, (fd_set *)NULL, (fd_set *)NULL, &timeout))  {
    ioctlsocket(net->toServerSocket, FIONREAD, count);
    return TRUE;
  }
		*count = 0;
    return FALSE;
}
int doSendPassword(struct NetWork *net,char *SendPassword)
{
	char *header;
	long size;

	header="PASS";

	size=1+namesize(SendPassword);

	if(writeLab(net,header,size))return 1;

	if(writeName(net,SendPassword))return 1;
	return 0;
}

int netWriteCheck(struct NetWork *net,char *buffout,long HdfSize)
{
	long n;

	if(!buffout || !net->netbuff)return 1;


	for(n=0;n<HdfSize;++n){
Start:
	    if(net->netcount < net->netsize){
	        net->netbuff[18+net->netcount++]=buffout[n];
	    }else{
	        if(NetDump(net))return 1;
	        goto Start;
	    }
	}
	return 0;
}
int doSendFile(struct NetWork *net,char *name,char *Type)
{
	char buff[256];
	FILE *in;
	long length,size;
/*
	struct stat buf;
*/
	int ret;

	ret=1;
	in=NULL;
/*
	if(SendDirectory){
	    if(!stat(name,&buf) && (buf.st_mode & S_IFDIR)){
	       doSendDir(name,Type);
	       return 0;
	    }	   
	}
*/
	if(!(in=fopen(name,"rb"))){
	    sprintf(buff,"Could Not Open (%s) To Read",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	if(fseek(in,0L,2)){
	    sprintf(buff,"(%s) Error Looking For End Of File",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	if((length=ftell(in)) == -1L){
	    sprintf(buff,"(%s) Error Reading End Of File Location",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}

	if(fseek(in,0L,0)){
	    sprintf(buff,"(%s) Error Rewinding File",name);
		WriteStatus(net->win,buff);
	    goto ErrorOut;
	}


	size=1+namesize(name)+length+4;

	if(writeLab(net,Type,size))goto ErrorOut;

	if(writeName(net,name))goto ErrorOut;
	
	if(writeLong(net,length))goto ErrorOut;


	if(writeFileBlocks(net,name,in,(long)length))goto ErrorOut;

	sprintf(buff,"%s %ld",name,length);
	WriteStatus(net->win,buff);

	ret = 0;

ErrorOut:

	if(in)fclose(in);

	return ret;
}
int writeFileBlocks(struct NetWork *net,char *name,FILE *in,long length)
{
	unsigned long n;
	char buff[256];

	if(!in || !name)return 1;

	if(SetBuffers(FILE_BLOCKS))return 1;

	while(length > 0){
	    n=length;
	    if(n > FILE_BLOCKS)n=FILE_BLOCKS;
	    if(fread(xg.buffin,1,n,in) != n){
	        sprintf(buff,"(%s) Error Reading File length (%ld)",name,n);
			WriteStatus(net->win,buff);
	        return 1;
	    }
	    if(writeString(net,xg.buffin,(long)n))return 1;
	    length -= n;
	}

	return 0;
}
int doEnd(struct NetWork *net)
{
	char *endt;
	int ret;

	if(!net)return 1;

	ret = 0;

	endt="ENDT";

	if(writeLab(net,endt,0L))ret = 1;
	NetDump(net);
	if(net->netbuff){
		cFree((char *)net->netbuff);
		net->netbuff=NULL;
	}
	return ret;
}
int NetWrite(struct NetWork *net,unsigned char *buffout,long HdfSize)
{
		IconPtr win;
		char buff[256];

		int ierror;
        long n;

		if(!net || !buffout)return 1;

		win=net->win;

        net->WriteBytes += HdfSize;
        net->BytesWrite += HdfSize;
        while(HdfSize > 0){
            n=HdfSize;
            if(n > 32000)n=32000;
            n=send(net->toServerSocket,buffout,n,0);
            if(n < 0){
				ierror=WSAGetLastError();
				if((ierror == WSAEWOULDBLOCK) && win){
					if(CheckStatusWindow(win)){
						Warning("break in netWrite");
						return 1;
					}
					Sleep2(1);
					continue;
				}
                sprintf(buff,"netWrite Write Socket Error %d",ierror);
				Warning(buff);
                return 1;
            }
            buffout += n;
            HdfSize -= n;
        }
	return 0;
}
int writeByte(struct NetWork *net,int i)
{
      unsigned char c[2];
 
        c[0] = i & 255;

	if(writeString(net,(char *)c,1L))return 1;

	return 0;
}
int writeString(struct NetWork *net,char *buffout,long HdfSize)
{
	if(net->netErrorCheck){
	    return netWriteCheck(net,buffout,HdfSize);
	}
	return NetWrite(net,buffout,HdfSize);
}
int writeLong(struct NetWork *net,long i)
{
      unsigned char c[4];
 
        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);
        c[2] = (unsigned char)((i >> 16) & 255);
        c[3] = (unsigned char)((i >> 24) & 255);

	if(writeString(net,(char *)c,4L))return 1;

	return 0;
}
int writeInt(struct NetWork *net,int i)
{
      unsigned char c[2];
 
        c[0] = (unsigned char)(i & 255);
        c[1] = (unsigned char)((i >> 8) & 255);

	if(writeString(net,(char *)c,2L))return 1;

	return 0;
}
int writeName(struct NetWork *net,char *name)
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

	if(writeByte(net,n))return 1;

	if(writeString(net,buff,(long)n))return 1;

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
int writeLab(struct NetWork *net,char *Type,long size)
{
	if(writeString(net,Type,4L))return 1;
	if(writeLong(net,size))return 1;
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
