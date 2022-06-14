#define EXTERN22 extern
#include "Xappl.h"
#include <ctype.h>
#include "uFiles.h"
#include "GWorld.h"
#include "SaveAllImages.h"
#include "uLib.h"

#ifdef Windows
#include <vfw.h>
#include <objbase.h>
int doBmp(char *name,unsigned char *image,int ixmax,int iymax);
int SetToAviFileExt(char *path);
int flip2(unsigned char *b,int xsize,int ysize);
#endif


#ifdef Macintosh
extern int StartQuickTime(IconPtr myIcon,int flag,PicHandle thePict,Rect *theRect,char *MovieName);
int EndDrawToQuickTime(void);
int StartDrawToQuickTime(void);
int EndQuickTime(void);
int SetQuickTimeOptions(IconPtr myIconi,int flag,PicHandle thePict,Rect *theRect);
#endif


int savescreen(char *name,int x1,int x2,int y1,int y2,unsigned int parray[]);

static int doWriteBMP(IconPtr myIcon,struct ImageSaveList *Images,int flag);
static int doWriteAVI(IconPtr myIcon,struct ImageSaveList *Images,int flag);

int getPICTData(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame);

int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);

int Start24to8(void);
int Add24to8(unsigned char *pp,long xsize,long ysize);
int Build24to8(unsigned char *pal);
int Crush24to8(unsigned char *pp,long xsize,long ysize);
int treeClean(void);


int doJPG(char *filename,unsigned char *image,int xsize,int ysize);

static int WriteHdfHex(char *name,unsigned char *data,unsigned char *pal,int xsize,int ysize);


int putString(unsigned char *s,long n,FILE *in);



extern int goCD(char *name);


static int doWritePicts(IconPtr myIcon,struct ImageSaveList *Images,int flag);

int doWriteTEK(IconPtr myIcon,struct ImageSaveList *Images,int flag);

static int doWriteHdf(IconPtr myIcon,struct ImageSaveList *Images,int flag);

static int doWriteTrue(IconPtr myIcon,struct ImageSaveList *Images,int flag);

static int doWriteJPG(IconPtr myIcon,struct ImageSaveList *Images,int flag);

static int doWriteQuickTime(IconPtr myIcon,struct ImageSaveList *Images,int flag);

static int doWrite256(IconPtr myIcon,struct ImageSaveList *Images,int type);

static int doWriteSDS2D(IconPtr myIcon,struct ImageSaveList *Images,int flag);

static int doWriteRGB(IconPtr myIcon,struct ImageSaveList *Images,int flag);


int doSaveImages(IconPtr myIcon,struct ImageSaveList *Images)
{

	if(!Images || !(Images->Files))return 1;
	
	switch(Images->type)
	{
		case IMAGE_PICT:
			return doWritePicts(myIcon,Images,0);
		case IMAGE_PICT32:
			return doWritePicts(myIcon,Images,1);
		case IMAGE_HDF_TO_256:
			return doWrite256(myIcon,Images,0);
		case IMAGE_HDF_TO_256_MERGE:
			return doWrite256(myIcon,Images,1);
		case IMAGE_HDF:
			return doWriteHdf(myIcon,Images,0);
		case IMAGE_HDF_MERGE:
			return doWriteHdf(myIcon,Images,1);
		case IMAGE_TRUE:
			return doWriteTrue(myIcon,Images,0);
		case IMAGE_TRUE_MERGE:
			return doWriteTrue(myIcon,Images,1);
		case IMAGE_QUICKTIME8:
			return doWriteQuickTime(myIcon,Images,0);
		case IMAGE_QUICKTIME32:
			return doWriteQuickTime(myIcon,Images,1);
		case IMAGE_SDS2D:
			return doWriteSDS2D(myIcon,Images,0);
		case IMAGE_SDS2D_MERGE:
			return doWriteSDS2D(myIcon,Images,1);
		case IMAGE_JPG:
			return doWriteJPG(myIcon,Images,0);
		case IMAGE_BMP:
			return doWriteBMP(myIcon,Images,0);
		case IMAGE_AVI:
			return doWriteAVI(myIcon,Images,0);
		case IMAGE_TEK:
			return doWriteTEK(myIcon,Images,0);
		case IMAGE_RGB:
			return doWriteRGB(myIcon,Images,0);
		default:
			sprintf(WarningBuff,"doSaveImages unknown type %ld",(long)(Images->type));
			Warning(WarningBuff);
		return 1;
	}
	

}
static int doWriteSDS2D(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct SetFrameData  sd;
	char *output;
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,seq,nameflag;
	char *namef,*path;
	long ni;
	int ret,ipath;
	IconPtr StatusWindow;
	struct LIMITS *limits;
	uRect rSave;

	if(!myIcon)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	ret=1;
	
	ipath=0;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;
	
			
	uGetClipRect(&rSave,myIcon);
  

	StatusWindow=uStartStatusWindow(myIcon,"SDS2D");

	zerol((char *)&sd,sizeof(struct SetFrameData));
	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    
	    uWriteStatusWindow(StatusWindow,buff);
	    
	    
	    zerol((char *)&sd,sizeof(struct SetFrameData));
			
	    sd.type=FRAME_DATA_FLOAT;
	    
	    if((*myIcon->pGetData)(myIcon,ni,&sd)){
			goto OutOfHere;
	    }
	    
		    				
	    mstrncpy(filepath,Images->directory,2048);
	    	    
	    limits = &myIcon->limits;
	    if(!limits->iGot)limits=NULL;
	    
	    if(flag){
	         strncatToPath(filepath,path,2048);	         
	        if(sds2DOut(filepath,name,sd.xsize,sd.ysize,(double *)sd.data,n,(int)(n >= (Images->count-1)),limits))goto OutOfHere;	        
			ipath=1;
	    }else{
	        strncatToPath(filepath,name,2048);	        
	        if(sds2DOut(filepath,NULL,sd.xsize,sd.ysize,(double *)sd.data,0L,1,limits))goto OutOfHere;
	    }

		if(sd.data)cFree((char *)sd.data); 
		sd.data=NULL;  

	    if(uCheckStatusWindow(StatusWindow,"Stop Writing SDS2D ?"))goto OutOfHere;
	}
	ret=0;
OutOfHere:
	if(ipath){
	 	SetFileType(filepath,"SDS2");
	}

	uStopStatusWindow(StatusWindow);
	
	if(sd.data)cFree((char *)sd.data); 
	sd.data=NULL;  
		
	uSetClipRect(&rSave,myIcon);

	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return ret;
}
static int doWriteTrue(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	char *output;
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,seq,nameflag;
	char *namef,*path;
	IconPtr StatusWindow;
	FILE *out;
	long ni;
	int ret;
	int retd,isflag;
	uRect rSave;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	
	isflag=IsShift();
			
	ret=1;
	
	out=NULL;
	
	StatusWindow=NULL;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;
				
	uGetClipRect(&rSave,myIcon);
  
 	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth32)){
			goto OutOfHere;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"WriteTrue");
	
	if(flag){	
	    mstrncpy(filepath,Images->directory,2048);
	    strncatToPath(filepath,path,2048);
		if(!(out=fopen(filepath,"wb"))){
			sprintf(WarningBuff,"doSaveTrue Error Opening (%s)",filepath);
			Warning(WarningBuff);
			goto OutOfHere;
		}
 	}
 	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    

	    StartGWorld(&World);
	    
	    retd=pDrawGo(myIcon,ni,Images->DRect);	    
        StopGWorld(&World);
        if(retd)goto OutOfHere;
				
	    mstrncpy(filepath,Images->directory,2048);
	    if(isflag){
	    	 strncatToPath(filepath,name,2048);
	         WriteHexFromGWorld((char *)filepath,&World,out);
	    }else if(flag){
	    	strncatToPath(filepath,path,2048);
	    	if(SaveTrueFromGWorld(filepath,&World,out))goto OutOfHere;	    
        	if(putString((unsigned char *)name,256L,out))goto OutOfHere;	    
	    }else{
	    	strncatToPath(filepath,name,2048);
	    	if(SaveTrueFromGWorld(filepath,&World,NULL))goto OutOfHere;
	        if(SetFileType(filepath,"TRUE"))goto OutOfHere;
	    }
	    
	    if(uCheckStatusWindow(StatusWindow,"Stop Writing True ?"))goto OutOfHere;
		
	}
	ret=0;
OutOfHere:
	uStopStatusWindow(StatusWindow);
	CloseGWorld(&World);
	if(flag && out){	
		putString((unsigned char *)"ENDF",4L,out);
		fclose(out);
	    SetFileType(path,"TRUE");
	}
	uSetClipRect(&rSave,myIcon);
	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return ret;
}
static int doWriteHdf(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	unsigned char r[256],g[256],b[256],pal[768];
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,k,seq,nameflag;
	char *namef,*path;
	long ni;
	int ret,isflag,retd;
	IconPtr StatusWindow;
	struct LIMITS *limits;
	unsigned char *dataPtr;
	long xsize,ysize;
	uRect rSave;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	ret=1;
	
	dataPtr=NULL;
		
	StatusWindow=NULL;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
						
	path=Images->savename;
	
	isflag=IsShift();
			
	GetPaletteColors(myIcon,r,g,b);
	
	k=0;
	for(n=0;n<256;++n){
	    pal[k++]=r[n];
	    pal[k++]=g[n];
	    pal[k++]=b[n];
	}
	
	uGetClipRect(&rSave,myIcon);
  
 	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth8)){
			goto OutOfHere;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"WriteHdf");
			
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(Images->prefix){
			sprintf(name,"%s.%04d%c",Images->prefix,seq+n,0);
	    }else if(path){
			mstrncpy(name,path,255);
	    }
	    
	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    	
	    StartGWorld(&World);
	    	
	    retd=pDrawGo(myIcon,ni,Images->DRect);	    
        StopGWorld(&World);
        if(retd)goto OutOfHere;
        
	    mstrncpy(filepath,Images->directory,2048);
	    
	    limits = &myIcon->limits;
	    if(!limits->iGot)limits=NULL;
		
		dataPtr=NULL;
        if(GetHdfFromGWorld(&xsize,&ysize,(unsigned char **)&dataPtr,&World))goto OutOfHere;
        	    
	    if(isflag){
	    	 strncatToPath(filepath,path,2048);
	         WriteHdfHex((char *)filepath,(unsigned char *)dataPtr,(unsigned char *)pal,(int)xsize,(int)ysize);
	    } else if(flag){
	    	strncatToPath(filepath,path,2048);	    	
	    	if(hdfOut(filepath,name,(char *)pal,xsize,ysize,(char *)dataPtr,n,(int)(n >= Images->count-1),limits))goto OutOfHere;
		
	    }else{
	    	strncatToPath(filepath,name,2048);
	    		    	
	    	if(hdfOut(filepath,name,(char *)pal,xsize,ysize,(char *)dataPtr,0,1,limits))goto OutOfHere;	    	
	    }
	
	    if(dataPtr)cFree((char *)dataPtr);
	    dataPtr=NULL;

	    if(uCheckStatusWindow(StatusWindow,"Stop Writing Hdf ?"))goto OutOfHere;
	}
	ret=0;
OutOfHere:

	uStopStatusWindow(StatusWindow);
	uSetClipRect(&rSave,myIcon);

	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);
	
	CloseGWorld(&World);

	if(dataPtr)cFree((char *)dataPtr);

	return ret;
}
static int doWrite256(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	char *output;
	unsigned char pal[768];
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,seq,nameflag;
	char *namef,*path;
	long ni;
	int ret;
	long xsize,ysize;
	IconPtr StatusWindow;
    unsigned char *image;
    int retd;
	uRect rSave;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	image=NULL;
	
	StatusWindow=NULL;
	
	ret=1;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;

	uGetClipRect(&rSave,myIcon);
  
	if(Start24to8())return 1;

	StatusWindow=uStartStatusWindow(myIcon,"Save256");
	
 	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth32)){
			goto OutOfHere;
	}	
	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }
	    
	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    
	    uWriteStatusWindow(StatusWindow,buff);
	    
	    StartGWorld(&World);
	    retd=pDrawGo(myIcon,ni,Images->DRect);
        StopGWorld(&World);
        if(retd)goto OutOfHere;
		    
		image=NULL;
		if(GetTrueFromGWorld(&xsize,&ysize,&image,&World))goto OutOfHere;
		    
		Add24to8(image,xsize,ysize);
		   	
		if(image)cFree((char *)image);

	}	
	
	Build24to8(pal);
	
		
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    
	    uWriteStatusWindow(StatusWindow,buff);
	    
	    StartGWorld(&World);
	    retd=pDrawGo(myIcon,ni,Images->DRect);
        StopGWorld(&World);
        if(retd)goto OutOfHere;
					
		image=NULL;
		
		if(GetTrueFromGWorld(&xsize,&ysize,&image,&World))goto OutOfHere;
		    
		if(Crush24to8(image,xsize,ysize))goto OutOfHere;
		 				
	    mstrncpy(filepath,Images->directory,2048);
	    
	    if(flag){
	    	strncatToPath(filepath,path,2048);
	    	if(hdfOut(filepath,name,(char *)pal,xsize,ysize,(char *)image,n,(int)(n >= Images->count-1),NULL))goto OutOfHere;
	    }else{
	    	strncatToPath(filepath,name,2048);
	    	if(hdfOut(filepath,name,(char *)pal,xsize,ysize,(char *)image,0,1,NULL))goto OutOfHere;
	    }
	
		if(image)cFree((char *)image);
		image=NULL;		
			    

	    if(uCheckStatusWindow(StatusWindow,"Stop Writing Hdf ?"))goto OutOfHere;
	}
	ret=0;
OutOfHere:
	if(image)cFree((char *)image);
	
	CloseGWorld(&World);

	treeClean();
	
	uStopStatusWindow(StatusWindow);
	
	uSetClipRect(&rSave,myIcon);

	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return ret;
}
static int WriteHdfHex(char *name,unsigned char *data,unsigned char *pal,int xsize,int ysize)
{
	FILE *out;
	int n,x,y;
	
	if(!name || !data || !pal)return 1;
	
	out=fopen(name,"w");
	if(!out)return 1;
	
	fprintf(out,"xsize %d ysize %d\n",xsize,ysize);
	
	fprintf(out,"\n{\n");
	for(y=0;y<ysize;++y){
	    for(x=0;x<xsize;++x){
	        n = *data++;
	        fprintf(out,"0x%02x",n);
	            fprintf(out,",");
	    }
	    fprintf(out,"\n");
	}	
	fprintf(out,"\n}\n");
	
	if(out)fclose(out);
	return 0;
	/*
	FILE *out;
	int n,x,y;
	
	if(!name || !data || !pal)return 1;
	
	out=fopen(name,"w");
	if(!out)return 1;
	
	for(y=0;y<ysize;++y){
	    for(x=0;x<xsize;++x){
	        n = *data++;
	        fprintf(out,"%02x",n);
	        if(x & 1){
	            fprintf(out," ");
	        }
	    }
	    fprintf(out,"\n");
	}	
	
	if(out)fclose(out);
	return 0;
	*/
}

static int doWriteJPG(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	char *output;
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,seq,nameflag;
	char *namef,*path;
	IconPtr StatusWindow;
	long ni;
	int ret;
	int retd;
	unsigned char *image;
	long xsize;
	long ysize;
	uRect rSave;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	//flag=flag;
	
	ret=1;
		
	StatusWindow=NULL;
	
	image=NULL;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;
	

	uGetClipRect(&rSave,myIcon);
   	
 	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth32)){
			goto OutOfHere;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"WriteJPG");
	
 	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }
	    
	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    

	    StartGWorld(&World);
	    retd=pDrawGo(myIcon,ni,Images->DRect);	    
        StopGWorld(&World);
        if(retd)goto OutOfHere;
				
	    mstrncpy(filepath,Images->directory,2048);
	    strncatToPath(filepath,name,2048);
	    	    
	    image=NULL;
	    
		if(GetTrueFromGWorld(&xsize,&ysize,&image,&World)){
			goto OutOfHere;;
		}
		
		if(doJPG(filepath,image,(int)xsize,(int)ysize))goto OutOfHere;
		
		if(image)cFree((char *)image);
		
		image=NULL;

	    	
	    if(SetFileType(filepath,"JPEG"))goto OutOfHere;
	    
	    if(uCheckStatusWindow(StatusWindow,"Stop Writing JPG ?"))goto OutOfHere;
		
	}
	ret=0;
OutOfHere:
	uStopStatusWindow(StatusWindow);
	CloseGWorld(&World);
	
	uSetClipRect(&rSave,myIcon);
	
	if(image)cFree((char *)image);

	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	return ret;
}
static int doWriteRGB(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	char *output;
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,seq,nameflag;
	char *namef,*path;
	IconPtr StatusWindow;
	long ni;
	int ret;
	int retd;
	unsigned char *image;
	unsigned int *parray;
	long xsize,xsizea;
	long ysize,ysizea;
	uRect rSave;
	int i,j;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	//flag=flag;
	
	ret=1;
		
	StatusWindow=NULL;
	
	image=NULL;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;
	

	uGetClipRect(&rSave,myIcon);
	
	parray=NULL;

   	xsizea=Images->DRect->xsize;
   	ysizea=Images->DRect->ysize;
   	
 	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth32)){
			goto OutOfHere;
	}

	parray=(unsigned int *)cMalloc(xsizea*ysizea*sizeof(unsigned int),9868);
	if(parray == NULL){
		goto OutOfHere;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"WriteRGB");
	
 	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s%c",path,0);
	    }
	    
	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    

	    StartGWorld(&World);
	    retd=pDrawGo(myIcon,ni,Images->DRect);	    
        StopGWorld(&World);
        if(retd)goto OutOfHere;
				
	    mstrncpy(filepath,Images->directory,2048);
	    strncatToPath(filepath,name,2048);
	    	    
	    image=NULL;
	    
		if(GetTrueFromGWorld(&xsize,&ysize,&image,&World)){
			goto OutOfHere;
		}
		
		if(xsizea != xsize || ysizea != ysize){
		    sprintf(WarningBuff,"doWriteRGB Buffer Size Error xsizea %ld xsize %ld ysizea %ld ysize %ld\n",xsizea,xsize,ysizea,ysize);
		    goto OutOfHere;
		}
		
		for(j=0;j<ysize;++j){
			for(i=0;i<xsize;++i){
			    unsigned char *bc = &image[3*i+j*3*xsize];
			    parray[i+j*xsize]=((unsigned int)bc[0] << 24) | ((unsigned int)bc[1] << 16) | ((unsigned int)bc[2] << 8);
			}
		}
		
		if(savescreen(filepath,(int)0,(int)(xsize-1),(int)0,(int)(ysize-1),parray))goto OutOfHere;
		
		if(image)cFree((char *)image);
		
		image=NULL;

	    	
	    if(SetFileType(filepath,"RGBF"))goto OutOfHere;
	    
	    if(uCheckStatusWindow(StatusWindow,"Stop Writing RGB ?"))goto OutOfHere;
		
	}
	ret=0;
OutOfHere:
	uStopStatusWindow(StatusWindow);
	CloseGWorld(&World);
	
	uSetClipRect(&rSave,myIcon);
	
	if(image)cFree((char *)image);

	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);

	if(parray)cFree((char *)parray);
	
	return ret;
}
#ifdef Macintosh
static int doWriteQuickTime(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	int seq,n,nameflag;
	long ni;
	PicHandle PICThand;
	Rect Box;
	int retd;
	int ret;
	int quickTimeOpen;
	uRect rSave;
	
	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
  	
	if(goCD(Images->directory))return 1;
	
  	ret=1;  	
  	PICThand=NULL;
  	quickTimeOpen=FALSE;
		
	nameflag=Images->UseImageNames;	
	seq=(int)Images->SequenceNumber;
			
	Box = RectConvert(Images->DRect);
	uGetClipRect(&rSave,myIcon);
 	ClipRect(&Box);
	
	PICThand=OpenPicture(&Box);
	ForeColor((long)blackColor);
	BackColor((long)whiteColor);
	ni=Images->imageList[0];
	    
	retd=pDrawGo(myIcon,ni,Images->DRect);	    
		
	ClosePicture();
	
	if(retd)goto OutOfHere;
	
	if(StartQuickTime(myIcon,flag,PICThand,&Box,Images->savename)){
		Warning("doSaveQuickTime StartQuickTime Failed");
		goto OutOfHere;
	}
	
  	quickTimeOpen=TRUE;
	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];	    
	    StartDrawToQuickTime();
	    if(pDrawGo(myIcon,ni,Images->DRect))goto OutOfHere;
	    if(EndDrawToQuickTime())goto OutOfHere;
 	    if(Stop("Stop QuickTime ?"))break;
	    
	}

  	ret=0;
	
OutOfHere:

    if(quickTimeOpen)EndQuickTime();
    
	if(PICThand){
	    KillPicture(PICThand);
	}
	uSetClipRect(&rSave,myIcon);
	EraseRectMyWindow(myIcon);
	InvalRectMyWindow(myIcon);

	return ret;
}
int doSaveQuickTimeOptions(IconPtr myIcon,struct ImageSaveList *Images)
{
	PicHandle PICThand;
	long ni;
	Rect Box;
	int flag;
	int retd;
	int ret;
	
	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
  	
  	
  	ret=1;
  	
  	PICThand=NULL;
  	
	Box = RectConvert(Images->DRect);
	GetClip(myIcon->saveRegion);
 	ClipRect(&Box);
	
	PICThand=OpenPicture(&Box);
	ForeColor((long)blackColor);
	BackColor((long)whiteColor);
	ni=Images->imageList[0];
	    
	retd=pDrawGo(myIcon,ni,Images->DRect);	    
		
	ClosePicture();
	
	if(retd)goto OutOfHere;

	flag = (Images->type == IMAGE_QUICKTIME32);
	
	if(SetQuickTimeOptions(myIcon,flag,PICThand,&Box))goto OutOfHere;
  	
  	ret=0;
  	
OutOfHere:
	if(PICThand){
	    KillPicture(PICThand);
	}  	
  	
  	return ret;
}
static int doWritePicts(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	char name[256],*namef,*output,*path;
	int seq,n,nameflag;
	long ni;
	PicHandle PICThand;
	Rect Box;
	struct sWorld World;
	int retd;
	uRect rSave;
	char filepath[2048];
	
	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
		

	PICThand=NULL;
	    
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
	
	path=Images->savename;

	uGetClipRect(&rSave,myIcon);

	Box= RectConvert(Images->DRect);
	
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			sprintf(name,"%s",path);
	    }

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
		uGetClipRect(&rSave,myIcon);
 	    ClipRect(&Box);
 	    
	    if(flag == 1){
			if(OpenGWorld(&World,&Box,GWorldDepth32))break;
		    StartGWorld(&World);
	        retd=pDrawGo(myIcon,ni,Images->DRect);
	        StopGWorld(&World);
		    PICThand=MakePictFromGWorld(&World,&Box);
			CloseGWorld(&World);
	        if(retd)goto OutOfHere;
		}else{
		    PICThand=OpenPicture(&Box);
		    ForeColor((long)blackColor);
		    BackColor((long)whiteColor);
		    
		    PicComment(picLParen,0,NULL);
	        retd=pDrawGo(myIcon,ni,Images->DRect);
		    PicComment(picRParen,0,NULL);
		    ClosePicture();
	        if(retd)goto OutOfHere;

		}
 	    
 	    	
		uSetClipRect(&rSave,myIcon);
		SetGWorld((CGrafPtr)NULL,myIcon->GDevWindow);
		SetPortMyWindow(myIcon);
		    
	    
	    mstrncpy(filepath,Images->directory,2048);
	    strncatToPath(filepath,name,2048);
	    
	    
	    if(SavePict(PICThand,filepath))goto OutOfHere;
	
	    if(PICThand)KillPicture(PICThand);
	    PICThand=NULL;
	    
	    if(SetFileType(filepath,"PICT"))goto OutOfHere;
	    
 	    if(Stop("Stop Writing Pict ?"))break;
	}
OutOfHere:
	uSetClipRect(&rSave,myIcon);
	if(PICThand)KillPicture(PICThand);
	InvalRectMyWindow(myIcon);

	return 0;
}

int PictToTrue(struct FileInfo2 *Files,struct SetFrameData *s);

int PictToTrue(struct FileInfo2 *Files,struct SetFrameData *s)
{
	struct sWorld World;
	unsigned char *image;
	long xsizei,ysizei,length,n;
	Rect Box;
	
	if(!Files || !s)return 1;

	Box.top=0;
	Box.left=0;
	Box.right=s->xsize;
	Box.bottom=s->ysize;
	
	if(OpenGWorld(&World,&Box,GWorldDepth32))return 1;
    StartGWorld(&World);
	DrawPicture((PicHandle)&s->data,&Box);
	StopGWorld(&World);

    image=NULL;
    
	if(GetTrueFromGWorld(&xsizei,&ysizei,&image,&World)){
		goto ErrorOut;
	}
	
	if(!image)goto ErrorOut; 
	
	length=3*xsizei*ysizei;
	if((length > Files->length) || !Files->dataSave){
		Files->length=0;
		if(Files->dataSave)cFree((char *)Files->dataSave);
		Files->dataSave=(unsigned char *)cMalloc(length,8131);
		if(!Files->dataSave)goto ErrorOut;
		Files->length=length;
	}
	
	for(n=0;n<length;++n)Files->dataSave[n]=image[n];
	
	s->type = TRUE_FILES;
	s->data = Files->dataSave;
	
ErrorOut:

	if(image)cFree((char *)image);
	CloseGWorld(&World);	
	
	return 0;
}
int getPICTData(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame)
{
	struct sWorld World;
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	unsigned char *data;
	unsigned char *image;
	long i,j;
	long x,y,xsize,ysize,xinc,iystep;
	long xsizei,ysizei;
	double xscale,yscale,oxscale,oyscale;
	int onetoOne;
	register unsigned char *bp,*dp;
	Rect Box;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(unsigned char *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	
	//CurrentFrame=CurrentFrame;
	
	onetoOne=TRUE;
	if((pio->raster.xScale != 1.0) || (pio->raster.yScale != 1.0))onetoOne=FALSE;	
	
	
	Box.top=0;
	Box.left=0;
	Box.right=s->xsize;
	Box.bottom=s->ysize;
	
	if(OpenGWorld(&World,&Box,GWorldDepth32))return 1;
    StartGWorld(&World);
	DrawPicture((PicHandle)&s->data,&Box);
	StopGWorld(&World);
	
    image=NULL;
    
	if(GetTrueFromGWorld(&xsizei,&ysizei,&image,&World)){
		goto OutOfHere;
	}
	
	data=(unsigned char *)image;
	if(!data)goto OutOfHere;
			
	xscale=pio->raster.xScale*xsizei;
	oxscale=1.0/pio->raster.xScale;
	yscale=pio->raster.yScale*ysizei;
	oyscale=1.0/pio->raster.yScale;
	
	x=pio->raster.box.x;
	xsize=pio->raster.box.xsize;
	xinc=xsize*3;
	if((x+xsize) > xscale){
	    xsize=(long)xscale-x;
	}
	
	y=pio->raster.box.y;
	ysize=pio->raster.box.ysize;
	if((y+ysize) > yscale){
	    ysize=(long)yscale-y;
	}
	
			    
	if(onetoOne){		
		for(j=0;j<ysize;++j){
		    bp=sout+j*xinc;
		    dp=data+x*3+xsizei*3*(y+j);
		    for(i=0;i<xsize;++i){
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
		    }
		}	
	}else{
	    for(j=0;j<ysize;++j){
	        iystep=xsizei*3*((long)((double)(y+j)*oyscale));
	        bp=sout+j*xinc;
	        for(i=0;i<xsize;++i){
	            dp=data+(3*((long)((double)(x+i)*oxscale))+iystep);
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
		    	*bp++ = *dp++;
	        }
	    }
	}
	
OutOfHere:

	if(image)cFree((char *)image);
	CloseGWorld(&World);	
	return 0;
}
#else
static int doWriteQuickTime(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	if(!myIcon || !Images)return 1;
	//flag=flag;
	Warning("doWriteQuickTime Not Active");
	return 1;
}
static int doWritePicts(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	if(!myIcon || !Images)return 1;
	//flag=flag;
	Warning("doWritePicts Not Active");
	return 1;
}
int getPICTData(struct FileInfo2 *Files,unsigned char *sout,long CurrentFrame)
{
	if(!Files || !sout)return 1;
	Warning("getPICTData Not Active");
	//CurrentFrame=CurrentFrame;
	return 1;
}
#endif
#ifdef Windows
int SetToAviFileExt(char *path) 
{
	int ic;

	ic = strlen(path)-1;
	while (ic>=0) {
		if (path[ic]=='.') {
			goto ADDEXT;
		}
		ic--;
	}
	ic = strlen(path);
ADDEXT:
	path[ic]   = '.';
	path[ic+1] = 'a';
	path[ic+2] = 'v';
	path[ic+3] = 'i';
	path[ic+4] = '\0';
	return 1;
}

static int doWriteAVI(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	char *output;
	unsigned char r[256],g[256],b[256],pal[768];
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,k,seq,nameflag;
	char *namef,*path;
	long ni;
	int ret,retd;
	IconPtr StatusWindow;
	long xsize,ysize;
	uRect rSave;
	WORD wVer;
	HRESULT hr;
	PAVIFILE pfile;
	AVISTREAMINFO strhdr;
	BITMAPINFOHEADER *ib;
	PAVISTREAM ps,psCompressed;
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS * aopts[1] = {&opts};
	long inc;
	struct ScreenInfo *s;
	double realSecondsPerFrame;

	realSecondsPerFrame = 1.0;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;

	wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a){
		MessageBox(NULL, "Wrong Version of VideoForWindowsVersion","Error", MB_OK|MB_ICONSTOP);
		return 1;
	}
		
	ret = 1;

	s=&myIcon->Screen;
	if (!s) return 1;
	
	ib=&s->ibitmap.bmiHeader;
	ib->biHeight=abs(ib->biHeight);
	
	xsize=ib->biWidth;
	ysize=ib->biHeight;

	StatusWindow=NULL;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
	
	output=Images->prefix;
	
	path=Images->savename;

	SetToAviFileExt(path);
		
	GetPaletteColors(myIcon,r,g,b);
	
	k=0;
	for(n=0;n<256;++n){
		pal[k++]=r[n];
		pal[k++]=g[n];
		pal[k++]=b[n];
	}
	
	uGetClipRect(&rSave,myIcon);
		
	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth32)){
		goto OutOfHere;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"WriteAVI");
	
	
	AVIFileInit();
	
	ps = NULL;
	pfile = NULL;
	psCompressed = NULL;
	
	
	hr = AVIFileOpen(&pfile,path,OF_WRITE | OF_CREATE,NULL);
	if (hr==AVIERR_BADFORMAT){
		goto OutOfHere;
	} else if (hr==AVIERR_MEMORY){
		goto OutOfHere;
	} else if (hr==AVIERR_FILEREAD){
		goto OutOfHere;
	} else if (hr==AVIERR_FILEOPEN){
		goto OutOfHere;
	} else if (hr==REGDB_E_CLASSNOTREG){
		goto OutOfHere;
	} else if (hr){
		goto OutOfHere;
	}

	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType                = streamtypeVIDEO;
	strhdr.fccHandler             = 0;
	strhdr.dwScale                = 1;
	strhdr.dwRate                 = (unsigned long)floor(myIcon->framesPerSecond);		
	strhdr.dwSuggestedBufferSize  = ib->biSizeImage;
	SetRect(&strhdr.rcFrame, 0, 0,		
		(int) ib->biWidth,
		(int) abs(ib->biHeight));
	
	hr = AVIFileCreateStream(pfile,&ps,&strhdr);
	if (hr)goto OutOfHere;
	
	memset(&opts, 0, sizeof(opts));
	
	if (!AVISaveOptions(myIcon->myWindow,
		ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE,
		1,
		&ps,
		(LPAVICOMPRESSOPTIONS *)&aopts))
		goto OutOfHere;
	
	hr = AVIMakeCompressedStream(&psCompressed, ps, &opts, NULL);
	if (hr != AVIERR_OK)goto OutOfHere;
	
	hr = AVIStreamSetFormat(psCompressed,0,ib,ib->biSize);
	if (hr)goto OutOfHere;
	
	inc = (long)realSecondsPerFrame; /* Real time, not 1/framerate */
	if(inc < 1)inc=1;
	
	for(n=0;n<Images->count;++n){
		ni=Images->imageList[n];
		if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
		}else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
		}else if(path){
			mstrncpy(name,path,255);
		}

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
		
		sprintf(buff,"%s%s","     Image: ",name);
		uWriteStatusWindow(StatusWindow,buff);
		
		StartGWorld(&World);
		
		retd=pDrawGo(myIcon,ni,Images->DRect);	    
        StopGWorld(&World);
        if(retd)goto OutOfHere;
        
		mstrncpy(filepath,Images->directory,2048);
						
		if(flag){
			strncatToPath(filepath,path,2048);	    	
		    flip2(s->buffer,(int)xsize,(int) ysize);
			hr = AVIStreamWrite(psCompressed,
				n * inc,			
				1,				
				s->buffer,
				ib->biSizeImage,
				AVIIF_KEYFRAME,	
				NULL,
				NULL);
			if (hr != AVIERR_OK)
				goto OutOfHere;
			
		}else{
			strncatToPath(filepath,name,2048);
	    		    	
		    flip2(s->buffer,(int)xsize,(int) ysize);
			hr = AVIStreamWrite(psCompressed,
				n * inc,			
				1,				
				s->buffer,
				ib->biSizeImage,
				AVIIF_KEYFRAME,	
				NULL,
				NULL);
			if (hr != AVIERR_OK)
				goto OutOfHere;
		}
		
		
		if(uCheckStatusWindow(StatusWindow,"Stop Writing AVI ?"))goto OutOfHere;
	}
	ret=0;
OutOfHere:
	
	ib->biHeight= -abs(ib->biHeight);
	
	if (ps)
		AVIStreamRelease(ps);
	
	if (psCompressed)
		AVIStreamRelease(psCompressed);
	
	if (pfile)
		AVIFileRelease(pfile);
	
	AVIFileExit();
	
	uStopStatusWindow(StatusWindow);
	uSetClipRect(&rSave,myIcon);
	
	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);
	
	CloseGWorld(&World);
		
	return ret;
}
static int doWriteBMP(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	struct sWorld World;
	char *output;
	unsigned char r[256],g[256],b[256],pal[768];
	char filepath[2048];
	char name[256];
	char buff[256];
	int n,k,seq,nameflag;
	char *namef,*path;
	long ni;
	int ret,isflag,retd;
	IconPtr StatusWindow;
	struct LIMITS *limits;
	unsigned char *dataPtr;
	long xsize,ysize;
	uRect rSave;

	if(!myIcon || !Images || !myIcon->uDraw)return 1;
  	if(!Images->imageList || !Images->count)return 1;
	
	ret=1;
	
	
	dataPtr=NULL;
		
	StatusWindow=NULL;
	
	nameflag=Images->UseImageNames;
	
	seq=(int)Images->SequenceNumber;
		
	output=Images->prefix;
		
	path=Images->savename;
	
	isflag=IsShift();
			
	GetPaletteColors(myIcon,r,g,b);
	
	k=0;
	for(n=0;n<256;++n){
	    pal[k++]=r[n];
	    pal[k++]=g[n];
	    pal[k++]=b[n];
	}
	
	uGetClipRect(&rSave,myIcon);
  
 	if(uOpenGWorld(myIcon,&World,Images->DRect,GWorldDepth8)){
			goto OutOfHere;
	}
	
	StatusWindow=uStartStatusWindow(myIcon,"WriteBMP");
		
	for(n=0;n<Images->count;++n){
	    ni=Images->imageList[n];
	    if(nameflag && (namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,255);
	    }else if(output){
			sprintf(name,"%s.%04d%c",output,seq+n,0);
	    }else if(path){
			mstrncpy(name,path,255);
	    }

	    if(Images->suffix && Images->suffix[0]){
			mstrncat((char *)name,".",255);
			mstrncat((char *)name,Images->suffix,255);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    	
	    StartGWorld(&World);
	    	
	    retd=pDrawGo(myIcon,ni,Images->DRect);	    
        StopGWorld(&World);
        if(retd)goto OutOfHere;
        
	    mstrncpy(filepath,Images->directory,2048);
	    
	    limits = &myIcon->limits;
	    if(!limits->iGot)limits=NULL;
		
		dataPtr=NULL;
        if(GetTrueFromGWorld(&xsize,&ysize,(unsigned char **)&dataPtr,&World))goto OutOfHere;
        	    
	    if(isflag){
	    	 strncatToPath(filepath,path,2048);
	         WriteHdfHex((char *)filepath,(unsigned char *)dataPtr,(unsigned char *)pal,(int)xsize,(int)ysize);
	    } else if(flag){
	    	strncatToPath(filepath,path,2048);	    	
	    	if(doBmp(filepath,dataPtr,xsize,ysize))goto OutOfHere;
		
	    }else{
	    	strncatToPath(filepath,name,2048);
	    		    	
	    	if(doBmp(filepath,dataPtr,xsize,ysize))goto OutOfHere;	    	
	    }
	
	    if(dataPtr)cFree((char *)dataPtr);
	    dataPtr=NULL;

	    if(uCheckStatusWindow(StatusWindow,"Stop Writing BMP ?"))goto OutOfHere;
	}
	ret=0;
OutOfHere:

	uStopStatusWindow(StatusWindow);
	uSetClipRect(&rSave,myIcon);

	myIcon->FileSave=TRUE;
	InvalRectMyWindow(myIcon);
	
	CloseGWorld(&World);

	if(dataPtr)cFree((char *)dataPtr);

	return ret;
}
int flip2(unsigned char *b,int xsize,int ysize)
{
	register unsigned char *p1,*p2,p;
	int x,y,xinc;

	if(!b)return 1;

	xinc=xsize*3;

	for(y=0;y<ysize/2;++y){
		p1=b+y*xinc;
		p2=b+(ysize-1-y)*xinc;
		for(x=0;x<xinc;++x){
			p = *p1;
			*p1++ = *p2;
			*p2++ = p;
		}
	}
	return 0;
}
#else
static int doWriteBMP(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	if(!myIcon || !Images)return 1;
	//flag=flag;
	WarningBatch("doWriteBMP Not Active\n");
	return 1;
	
}
static int doWriteAVI(IconPtr myIcon,struct ImageSaveList *Images,int flag)
{
	if(!myIcon || !Images)return 1;
	//flag=flag;
	WarningBatch("doWriteAVI Not Active\n");
	return 1;
}
#endif


