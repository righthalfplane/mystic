#define EXTERN22 extern
#include "Xappl.h"
#include <ctype.h>
#include <unistd.h>
#include "uFiles.h"
#include "GWorld.h"
#include "SaveAllImages.h"
#include "uLib.h"

int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);
int putInt(long n,FILE *out);
int putLong(long n,FILE *in);

int SaveTrueToFile(char *name,long xsize,long ysize,unsigned char *image,
                   int BitsCompression,FILE *out);
                   
int trueOut(char *filepath,char *name,long xsize,long ysize,unsigned char *image,long n,int flag);

int trueOut(char *filepath,char *name,long xsize,long ysize,unsigned char *image,long n,int flag)
{
	static FILE *out;
	int ret;
	
	if(!filepath)return 1;
	
	ret = 1;
	if(n == 0){
    	unlink((char *)filepath);
		if(!(out=fopen(filepath,"wb"))){
			sprintf(WarningBuff,"trueOut Error Opening File (%s) To Write\n",filepath);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
	}
	
	if(!out)goto ErrorOut;
	
	if(SaveTrueToFile(name,xsize,ysize,image,8,out))goto ErrorOut;
    if(putString((unsigned char *)name,256L,out))goto ErrorOut;	    
	
	ret = 0;
		
ErrorOut:

	
	if(ret){
	   out=NULL;
	}else{	
		if(flag && out){
			putString((unsigned char *)"ENDF",4L,out);
			fclose(out);
			if(SetFileType(filepath,"TRUE"))goto ErrorOut;
		}	
	}
		
	
	return ret;
	
}
int uOpenGWorld(struct Icon *myIcon,struct sWorld *World,uRect *Box,int depth)
{
	if(!myIcon || !World || !Box)return 1;
	World->depth=depth;
	World->myIcon=myIcon;
	
	return 0;
}
void StartGWorld(struct sWorld *World)
{
	struct ScreenInfo *s;
	if(!World)return;
	myIcon=World->myIcon;
	if(!myIcon)return;
	s = &myIcon->Screen;
	s->buff=NULL;
	s->data=NULL;
	return;
}
void StopGWorld(struct sWorld *World)
{
	//World=World;
}
void CloseGWorld(struct sWorld *World)
{
	//World=World;
}
int SaveTrueToFile(char *name,long xsize,long ysize,unsigned char *image,
                   int BitsCompression,FILE *out)
{
	static unsigned int lmask[]={0,128,192,224,240,248,252,254,255};
	unsigned char *ImageBuff;
	long length;
	int ret;
    unsigned int Esc1,Esc2;
    long lout;
    int bits,isopen;
    unsigned int mask;
	
	ret=1;
	ImageBuff=NULL;
	
	if(out != NULL){
	    isopen = TRUE;
	}else{
	    isopen = FALSE;
		out=NULL;
		
		if(!(out=fopen(name,"wb"))){
			sprintf(WarningBuff,"Error Opening File (%s) To Write",name);
			Warning(WarningBuff);
			return 1;
		}
	}
	
	
	length=xsize*ysize*3;
     if(!(ImageBuff=(unsigned char *)cMalloc(length,8209))){
            sprintf(WarningBuff,"SaveTrueToFile Out Of Memory Request (%ld)",length);
            Warning(WarningBuff);
            goto EndIt;
	}
	
    bits=BitsCompression;
    if(bits <= 0 || bits > 8)bits=8;
    mask=lmask[bits];
    if(Stuff(&Esc1,&Esc2,image,ImageBuff,length,&lout,mask)){
        Warning("Stuff Error");
		goto EndIt;
    }
        
	if(isopen){
        if(putString((unsigned char *)"DRt2",4L,out))goto EndIt;
	}else{
        if(putString((unsigned char *)"DRt1",4L,out))goto EndIt;
	}
	
        if(putInt((int)xsize,out))goto EndIt;
        if(putInt((int)ysize,out))goto EndIt;

        if(putInt((int)Esc1,out))goto EndIt;
        if(putInt((int)Esc2,out))goto EndIt;

        if(putLong(length,out))goto EndIt;
        if(putLong(lout,out))goto EndIt;

        if(putString(ImageBuff,lout,out))goto EndIt;

	ret=0;
EndIt:	
	if(ret){
		sprintf(WarningBuff,"SaveTrueToFile Error Writing File %s",name);
	    Warning(WarningBuff);
    }
	if(!isopen && out)fclose(out);
	if(ImageBuff)cFree((char *)ImageBuff);
	return ret;
}
int WriteHexFromGWorld(char *name,struct sWorld *World,FILE *out)
{
	unsigned char *image,*data;
	long xsize;
	long ysize;
	long x,y;
	int n,ret;
	int iclose;
	
	ret=1;
	if(GetTrueFromGWorld(&xsize,&ysize,&image,World)){
		return 1;
	}
	
	iclose=FALSE;
	
	if(!out){
	    out=fopen(name,"w");
	    if(!out)goto Error;
		iclose=TRUE;
	}
	
	data=image;
	
	fprintf(out,"%s xsize %ld ysize %ld\n",name,xsize,ysize);
	
	fprintf(out,"\n{\n");
	for(y=0;y<ysize;++y){
	    for(x=0;x<xsize;++x){
	        n = *data++;
	        fprintf(out,"0x%02x",n);
	            fprintf(out,",");
	        n = *data++;
	        fprintf(out,"0x%02x",n);
	            fprintf(out,",");
	        n = *data++;
	        fprintf(out,"0x%02x",n);
	            fprintf(out,",  ");
	    }
	    fprintf(out,"\n");
	}	
	fprintf(out,"\n}\n");
	
	
	
	
	ret=0;
Error:
	if(image)cFree((char *)image);
	
	if(iclose){
	    if(out)fclose(out);
	}
	return ret;
}
int SaveTrueFromGWorld(char *name,struct sWorld *World,FILE *out)
{
	unsigned char *image;
	long xsize;
	long ysize;
	int ret;
	
	ret=1;
	
	if(!name || !World)return 1;
	
	if(GetTrueFromGWorld(&xsize,&ysize,&image,World))return 1;
	
	if(SaveTrueToFile(name,xsize,ysize,image,8,out))goto Error;
	ret=0;
Error:
	if(image)cFree((char *)image);
	return ret;
	
}
int GetTrueFromGWorld(long *xsize,long *ysize,unsigned char **image,struct sWorld *World)
{
	struct Icon *myIcon;
	struct ScreenInfo *s;
	unsigned char *dout;
	long i,j,xsize3;
	
	if(!xsize  || !ysize  || !image  || !World)return 1;
	myIcon=World->myIcon;
	if(!myIcon)return 1;
	s = &myIcon->Screen;
	if(!s->buff || !s->data)return 1;
	*xsize=s->xsize;
	*ysize=s->ysize;
	xsize3=s->xsize*3;
	
	dout=(unsigned char *)cMalloc(xsize3*s->ysize,1999);
	if(!dout)return 1;
	
	if(s->DataType == ScreenDataType8){
		for(j=0;j<s->ysize;++j){
		    for(i=0;i<s->xsize;++i){
		       unsigned char n;
		       
		       n=s->buff[i+j*s->xsize];
		       dout[3*i+j*xsize3]=myIcon->pal[n*3];
		       dout[3*i+1+j*xsize3]=myIcon->pal[n*3+1];
		       dout[3*i+2+j*xsize3]=myIcon->pal[n*3+2];
		    }
		}
	}else if(s->DataType == ScreenDataType24){
		for(j=0;j<s->ysize;++j){
		    for(i=0;i<s->xsize;++i){
		       
		       dout[3*i+j*xsize3]=s->buff[3*i+j*3*s->xsize];
		       dout[3*i+1+j*xsize3]=s->buff[3*i+1+j*3*s->xsize];
		       dout[3*i+2+j*xsize3]=s->buff[3*i+2+j*3*s->xsize];
		    }
		}
	}
	
	*image=dout;
	
	return 0;
}
int GetHdfFromGWorld(long *xsize,long *ysize,unsigned char **image,struct sWorld *World)
{
	struct Icon *myIcon;
	struct ScreenInfo *s;
	unsigned char *dout;
	long i,j;
	
	if(!xsize  || !ysize  || !image  || !World)return 1;
	myIcon=World->myIcon;
	if(!myIcon)return 1;
	s = &myIcon->Screen;
	if(!s->buff || !s->data)return 1;
	*xsize=s->xsize;
	*ysize=s->ysize;
	
	dout=(unsigned char *)cMalloc(s->xsize*s->ysize,1999);
	if(!dout)return 1;
	
	if(s->DataType == ScreenDataType8){
		for(j=0;j<s->ysize;++j){
		    for(i=0;i<s->xsize;++i){
		       dout[i+j*s->xsize]=s->buff[i+j*s->xsize];
		    }
		}
	}
	
	*image=dout;
	
	return 0;
}
