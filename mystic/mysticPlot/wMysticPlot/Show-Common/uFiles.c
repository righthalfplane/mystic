#define EXTERN22 extern
#include <ctype.h>
#include "Xappl.h"
#include "uFiles.h"
#include "CExpress.h"
#include "Message.h"
#include "paletteDraw.h"
#include "TracerDef.h"
#include "HdfFiles.h"
#include "FileManager.h"
#include "Universal.h"
#include "initPIOBlocked.h"

int isDirectory(char *path);

static int CheckSTLSpace(struct FileInfo2 *Files);
static int initSTL(struct FileInfo2 *FilesL);

int AddjustType(unsigned char *d,long length,int floatType);
int FilePutLimitsDD(unsigned char *command,long length,int floatType,long CurrentFrame,struct FileInfo2 *Files);

struct FileInfo2 *GetFileInfoStruct(int tag);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int FilesOpenVRML(struct FileInfo2 *Files);
int DFR8restart(void);
int FilesSaveHDF3(struct FileInfo2 *Files);
int OpenViewLib2(struct FileInfo2 *Files);
int FilesOpenEdit(struct FileInfo2 *Files);
int doSDS3DStart(struct FileInfo2 *Files);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int UnCompress(unsigned int Esc1,unsigned int Esc2,long UnCompressed,long Compressed,unsigned char *fData,unsigned char *oData);
int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);
int HdfPack(unsigned char *,unsigned char *,long,long *);

int freeStreamLine3d(struct PlotStreamData *streamline3d);

int freeStreamLine2d(struct PlotStreamData *streamline2d);

int FilesInfoRGB(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files);

int DFPgetpal(char *name,char *pal);

int FilesOpenHDF(struct FileInfo2 *Files);
int FilesSaveTRUE(char *Name,long CurrentFrame,struct FileInfo2 *Files);
int FilesMergedTRUE(char *filename,char *Name,
                    long CurrentFrame,struct FileInfo2 *Files);
int FilesInfoTRUE(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files);
int FilesOpenSDS3D(struct FileInfo2 *files);

int TrueMergedDone(FILE *out);

int initVRML(struct FileInfo2 *FilesL);
int CheckVRMLSpace(struct FileInfo2 *Files);

struct FileInfo2 *FilesTRUE(void);
int FilesSaveTRUEToMemory(char *name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,struct FileInfo2 *Files);
			  
int FileTrueWrite(char *Header,unsigned char *buffer,
                  long xsize,long ysize,long UnCompressed,
	          long Compressed,unsigned int Esc1,
	          unsigned int Esc2,FILE *out);

int TrueWriteMerge(char *Name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,FILE *out);
			  
int initSDS3D(struct FileInfo2 *Files);
int CheckSDS3DSpace(struct FileInfo2 *Files);
int CheckShowSpace(struct FileInfo2 *Files);
int initSHOW(struct FileInfo2 *FilesL);

int FilesGetShow(struct Block *b,long CurrentFrame,struct FileInfo2 *Files);
int getFloatList(FILE *in,double *FloatData,unsigned long count);
int getLongList(FILE *in,long *LongData,unsigned long count);
int showLoad(struct SHOW *s,long CurrentFrame,struct FileInfo2 *Files);

double *getFloat3D(long CurrentFrame,struct FileInfo2 *Files);
int FilesGetSDS3Ddata(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);
int FilesSetFrameSDS3D(long CurrentFrame,struct FileInfo2 *Files);

int sageClose(struct PIO *sage,int flag);

int FilesSetType(struct FileList *Files);

int FilesOpenTek(struct FileInfo2 *files);

long FileLength(FILE *in);

int initTEK(struct FileInfo2 *Files);

int initVIEW(struct FileInfo2 *Files);

int initPICT(struct FileInfo2 *Files);

int initPIO(struct FileInfo2 *Files); 

int initPIOBlk(struct FileInfo2 *Files); 

int initQUICKTIME(struct FileInfo2 *Files);

int CheckQuickTimeSpace(struct FileInfo2 *Files);

int CheckPictSpace(struct FileInfo2 *Files);

int FilesSetFramePICT(long CurrentFrame,struct FileInfo2 *Files);


int FilesSetFrameQUICKTIME(long CurrentFrame,struct FileInfo2 *Files);

int CheckTekSpace(struct FileInfo2 *Files);

int CheckNames(struct FileInfo2 *Files);

int FilesGetTEK(char **ImageName,long *teklength,char **buffer,
                long CurrentFrame,struct FileInfo2 *Files);

int initTRUE(struct FileInfo2 *Files);

int CheckTrueSpace(struct FileInfo2 *Files);

int FilesSetFrameTRUE(long n,struct FileInfo2 *Files);


int initRGB(struct FileInfo2 *Files);

int CheckRGBSpace(struct FileInfo2 *Files);

int FilesSetFrameRGB(long n,struct FileInfo2 *Files);

int GetRGBImage(char *name,long *xsizei,long *ysizei,unsigned char **image);

static int doexpand(unsigned char *iptr,unsigned char *optr);

static int doput(unsigned char *i,unsigned char *o,long n);


int initSDS2D(struct FileInfo2 *Files);

int CheckSDS2DSpace(struct FileInfo2 *Files);

int FilesSetFrameSDS2D(long CurrentFrame,struct FileInfo2 *Files);

int getFloat(unsigned char *buffer,unsigned char *bs,
             long offset,long length,int floatType, FILE *in);
             
int FileSDS2DInfo(char **ImageName,long *xsize,long *ysize,
	          long *Length,long CurrentFrame,
	          struct FileInfo2 *Files);

int FilesGetSDS2Ddata(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);
                 
double *getFloatp(long CurrentFrame,struct FileInfo2 *Files);
                 

static unsigned char NCSAtag[]={'N'-'@','C'-'@','S'-'@','A'-'@'};

#ifdef uint16
#undef uint16
#endif

#ifdef int32
#undef int32
#endif

#define uint16 unsigned int
#define int32 long


typedef struct DFdd2 {	
    uint16 tag;		
    uint16 ref;		
    int32 offset;	
    int32 length;
    uint16 labeltag;		
    uint16 labelref;	
    char *labelname;
} DFdd2;


int getDD(DFdd2 **dd,long *count,FILE *in);

#define DFTAG_DIL    ((uint16)104) /*  */
#define DFTAG_DIA    ((uint16)105) /*  */
#define DFTAG_SDG    ((uint16)700) /* Scientific Data Group*/
#define DFTAG_SDD    ((uint16)701) /* Scientific Dim Rec*/
#define DFTAG_SD     ((uint16)702) /* Scientific Data*/
#define DFTAG_SDS    ((uint16)703) /* Scientific Data Scales*/


static unsigned char NCSA2[]={'N'-'@','C'-'@','S'-'@','A'-'@'};

int initHDF(struct FileInfo2 *Files);

int CheckHdfSpace(struct FileInfo2 *Files);

int FilesSetFrameHDF(long itIs,struct FileInfo2 *Files);

int FileHdfInfo(long *xsize,long *ysize,long *length,
                long CurrentFrame,struct FileInfo2 *Files);
                
int FilesGetHDF(char **ImageName,char *buffer,long length,
                long CurrentFrame,struct FileInfo2 *Files);
                

static long FileTagCount;

static char *DRt1="DRt1";

static char *DRt2="DRt2";

int getString(unsigned char *s,long n,FILE *in);

int getInt(int *i,FILE *in);

int getLong(long *n,FILE *in);

int freeDD(DFdd2 *dd,long dcount);


int DFR8getdims(char *name ,int *ixmax,int *iymax,int *hasPalette);
int DFSDgetdims(char *name,int *rank,int *sizes,int maxrank);  
int DFANputlabel(char *file,unsigned short int type,unsigned short int tag,char *name);
int DFSDadddata(char *path,int rank,int *size,double *data);
int DFSDlastref(void);
int DFSDrestart(void); 
int DFSDclear(void); 
int DFerrno(void);
int DFPrestart(void);

int IsRGB(char *name);
int IsViewLib(char *name);
int IsTRUE(char *name,int *type);
int IsHdf(char *name);
int IsPIOBlk(char *namef,int *rank);
int IsSDS(char *name,int *rank);
int IsPIO(char *name,int *rank);
int IsMOOV(char *name);
int IsTEXT(char *name);
int IsQUICKTIME(char *path);
int IsCONF(char *path);



int IsBMP(char *name);
int initBMP(struct FileInfo2 *Files);
int GetBMPImage(char *name,long *xsizer,long *ysizer,unsigned char **image);
int CheckBMPSpace(struct FileInfo2 *Files);
int flip(unsigned char *b,int xsize,int ysize);
int getLongLow(unsigned long *n,FILE *out);
int getIntLow(unsigned int *i,FILE *in);
int FilesSetFrameBMP(long CurrentFrame,struct FileInfo2 *Files);
int FilesInfoBMP(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files);


int Stuff3(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,
          unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);



#define COLSIZE		256
#define EXTRAFUDGE	128
#define	GIFGAMMA	(1.5)
#define	IMAX		((1<<16)-1)

struct GIFdataStruct{
	int global;				/* Is there a global color map? */
	int globalbits;				/* Number of bits of global colors */
	unsigned char globalmap[COLSIZE][3];	/* RGB values for global color map */
	char bgcolor;				/* background color */
	unsigned char *raster;                  /* Decoded image data */
	unsigned int left, top;
	unsigned int width, height;
	unsigned short red[COLSIZE];
	unsigned short green[COLSIZE];
	unsigned short blue[COLSIZE];
	char *filename, *imagename;
	short gamtab[256];
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
	long HdfSize;
};


int IsGIF(char *files);
int initGIF(struct FileInfo2 *Files);
int CheckGIFSpace(struct FileInfo2 *Files);
int FilesSetFrameGIF(long CurrentFrame,struct FileInfo2 *Files);
int FilesInfoGIF(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files);
int CheckGIFSpace(struct FileInfo2 *Files);
static int checksignature(FILE *in,struct GIFdataStruct *g);
static int readscreen(FILE *in,struct GIFdataStruct *g);
static int readgifimage(FILE *in,struct GIFdataStruct *g,char *filename);
static int rasterize(int interleaved, register unsigned char *raster,struct GIFdataStruct *g);
static int readextension(FILE *in);
static int initcolors(unsigned char colormap[COLSIZE][3], int ncolors,struct GIFdataStruct *g);
static int process(register int code,unsigned char **fill,struct GIFdataStruct *g);
static int readraster(unsigned width,unsigned  height,struct GIFdataStruct *g);

int IsJPG(char *files);
int initJPG(struct FileInfo2 *Files);
int CheckJPGSpace(struct FileInfo2 *Files);
int FilesSetFrameJPG(long CurrentFrame,struct FileInfo2 *Files);
int FilesInfoJPG(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files);
int GetJPGImage(char *name,long *xsizer,long *ysizer,unsigned char **image,int flag);


int FixPathName(char *path);

int FileOpenRemove(struct FileInfo2 *Files);

char *FilesImageName(struct FileInfo2 *Files,long n);

int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);
                 
int FilesGetHDFdata(double **datai,long *lengthi,
                 long CurrentFrame,struct FileInfo2 *Files);
                
int IsTEK(char *name);

int IsVRML(char *name);

int IsSHOW(char *namei);

int IsPICT(char *path);

int IsSTL(char *name);

struct FileInfo2 *FilesSDS3D(void);

int FilesSaveSDS2D(struct FileInfo2 *Files);

int FilesSaveHDF2(struct FileInfo2 *Files);

int FilesOpenDefault(struct FileInfo2 *Files);

int FilesMergeHDF(struct FileInfo2 *FilesOld,struct FileInfo2 *FilesNew);
int FilesMergeSDS2D(struct FileInfo2 *Files,struct FileInfo2 *FilesNew);

int FilesMergeTRUE(struct FileInfo2 *FilesOld,struct FileInfo2 *FilesNew);

int FilesMergePICT(struct FileInfo2 *Files,struct FileInfo2 *FilesNew);

struct FileInfo2 *FilesReOpen(struct FileInfo2 *File);

int flipSDS3DY(double *buffer,long xsize,long ysize,long zsize);

static int flip3(double *b,int xsize,int ysize);

static int FilesReflectX(struct FileInfo2 *Files);

int strains3Dget(struct Block *b);
int strains2Dget(struct Block *b);

static int FilesReflectX(struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	unsigned char *bp,*b;
	double *dp,*d;
	int i,j;
	long length;

	if(!Files)return 1;
	s = &Files->SetData;
	if(s->type != TRUE_FILES && s->type != HDF_FILES && 
	   s->type != SDS2D_FILES)return 0;
	if(!s->data)return 1;
	
	length=2*s->xsize*s->ysize;
	
	if(!Files->reflectSave || (Files->reflectLength < (long)(length*sizeof(double)))){
	    if(Files->reflectSave)cFree((char *)Files->reflectSave);
	    Files->reflectSave=(unsigned char *)cMalloc(length*sizeof(double),8631);
	    if(!Files->reflectSave)return 1;
	    Files->reflectLength=length*sizeof(double);	    
	}

   bp=Files->reflectSave;
   b=(unsigned char *)s->data;
	if(s->type == TRUE_FILES){
	   for(j=0;j<s->ysize;++j){
	      for(i=0;i<s->xsize;++i){
	          bp[3*(i+s->xsize)  +j*6*s->xsize]=b[i*3  +j*3*s->xsize];
	          bp[3*(i+s->xsize)+1+j*6*s->xsize]=b[i*3+1+j*3*s->xsize];
	          bp[3*(i+s->xsize)+2+j*6*s->xsize]=b[i*3+2+j*3*s->xsize];
	          bp[3*i  +j*6*s->xsize]=b[(s->xsize-i-1)*3  +j*3*s->xsize];
	          bp[3*i+1+j*6*s->xsize]=b[(s->xsize-i-1)*3+1+j*3*s->xsize];
	          bp[3*i+2+j*6*s->xsize]=b[(s->xsize-i-1)*3+2+j*3*s->xsize];
	      }
	   }
	   
	}else if(s->type == HDF_FILES){
	   for(j=0;j<s->ysize;++j){
	      for(i=0;i<s->xsize;++i){
	          bp[(i+s->xsize)  +j*2*s->xsize]=b[i  +j*s->xsize];
	          bp[i  +j*2*s->xsize]=b[(s->xsize-i-1)  +j*s->xsize];
	      }
	   }
	}else if(s->type == SDS2D_FILES){
   		dp=(double *)Files->reflectSave;
   		d=(double *)s->data;
	   for(j=0;j<s->ysize;++j){
	      for(i=0;i<s->xsize;++i){
	          dp[(i+s->xsize)  +j*2*s->xsize]=d[i  +j*s->xsize];
	          dp[i  +j*2*s->xsize]=d[(s->xsize-i-1)  +j*s->xsize];
	      }
	   }
	}

	s->xsize *= 2;
	s->data=Files->reflectSave;
	
	return 0;
}

int FilesSetParameters(struct FileInfo2 *Files,struct FilesSetList *list,int Item)
{
    struct FilePIOInfo *pio;
	
	if(!Files || !list)return 1;
	pio = &Files->pioData;
	
	if((Item & FILES_SET_REFLECT_X) && (Files->fileClass != PIO1D_FILES)){
	    if(list->Reflect_x){
	      	if(Files->Reflect_x)return 0;	      	
			pio->raster.xsize=2*Files->xmax;
			pio->raster.box.xsize=pio->raster.xsize;
	      	if(pio->range.xminData >= 0.0)pio->range.xminData=-Files->xmaxr;
	      	Files->Reflect_x=TRUE;
	      	Files->xminr = -Files->xmaxr;
	    }else{
	      	if(!Files->Reflect_x)return 0;
			pio->raster.xsize=Files->xmax;
			pio->raster.box.xsize=pio->raster.xsize;
	      	pio->range.xminData=0;
	      	Files->Reflect_x=FALSE;
	      	Files->xminr = 0;
	    }
	    pio->pa.xReflect=Files->Reflect_x;
	}
	return 0;
}
int printFileInformation(struct FileInfo2 *Files)
{
	struct LIMITS *limits;
	char buff[256];
	long CurrentFrame;
	long n,nf;
	
	if(!Files)return 1;
	
	sprintf(WarningBuff,"  Files Directory Path '%s'\n%c",Files->directory,0);
	WarningBatch(WarningBuff);
	
	
	limits=NULL;
	FileGetLimits(&limits,Files->CurrentFrame,Files);
	if(!limits || !limits->iGot)limits=NULL;
	
	if(limits){
	    FilePutCommandBuff(buff,
	        limits->xmin,limits->xmax,
	        limits->ymin,limits->ymax,
	        limits->zmin,limits->zmax,
	        limits->vmin,limits->vmax,
	        limits->dolog,
	        (int)limits->pioIndex,
	        limits->pioName,
	        limits->time);
	}
	
	WarningBuff[0]=0;
	
	
	CurrentFrame=Files->CurrentFrame;
	if(CurrentFrame < 0)CurrentFrame=0;
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	   
	if(Files->fileClass == PIO1D_FILES){
	 	sprintf(WarningBuff,"    Range xmin %g xmax %g\n%c",Files->xminr,Files->xmaxr,0);
		if(limits){
			WarningBatch(WarningBuff);
	        mstrncpy(WarningBuff,"    ",256);
	        mstrncat(WarningBuff,(char *)buff,256);
	        mstrncat(WarningBuff,"\n",256);
		}	
	}else if(Files->fileClass == PIO2D_FILES){
	 	sprintf(WarningBuff,"    Range xmin %g xmax %g ymin %g ymax %g \n%c",Files->xminr,Files->xmaxr,
	 	        Files->yminr,Files->ymaxr,0);
		if(limits){
			WarningBatch(WarningBuff);
	        mstrncpy(WarningBuff,"    ",256);
	        mstrncat(WarningBuff,(char *)buff,256);
	        mstrncat(WarningBuff,"\n",256);
		}	
	}else if(Files->fileClass == PIO3D_FILES){
	 	sprintf(WarningBuff,"    Range xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g\n%c",Files->xminr,Files->xmaxr,
	 	        Files->yminr,Files->ymaxr,Files->zminr,Files->zmaxr,0);
		if(limits){
			WarningBatch(WarningBuff);
	        mstrncpy(WarningBuff,"    ",256);
	        mstrncat(WarningBuff,(char *)buff,256);
	        mstrncat(WarningBuff,"\n",256);
		}	
	}else if(Files->fileClass == HDF_FILES || Files->fileClass == SDS2D_FILES){
	
	
	    if(Files->fileClass == HDF_FILES && Files->HDFList){
			sprintf(WarningBuff,"    CurrentFrame %ld Limits ixmax %ld iymax %ld xsize %d ysize %d\n%c",
	 	        CurrentFrame,Files->xmax,Files->ymax,Files->HDFList[CurrentFrame].xsize,Files->HDFList[CurrentFrame].ysize,0);
		}
		
	    if(Files->fileClass == SDS2D_FILES && Files->SDS2DList){
			sprintf(WarningBuff,"    CurrentFrame %ld Limits ixmax %ld iymax %ld xsize %d ysize %d\n%c",
	 	        CurrentFrame,Files->xmax,Files->ymax,Files->SDS2DList[CurrentFrame].xsize,Files->SDS2DList[CurrentFrame].ysize,0);
		}
		
		if(limits){
			WarningBatch(WarningBuff);
	        mstrncpy(WarningBuff,"    ",256);
	        mstrncat(WarningBuff,(char *)buff,256);
	        mstrncat(WarningBuff,"\n",256);
		}	
	}else if(Files->fileClass == SDS3D_FILES){
	 	sprintf(WarningBuff,"    CurrentFrame %ld Limits ixmax %ld iymax %ld izmax %ld\n%c",
	 	        Files->CurrentFrame,Files->xmax,Files->ymax,Files->zmax,0);
		if(limits){
			WarningBatch(WarningBuff);
	        mstrncpy(WarningBuff,"    ",256);
	        mstrncat(WarningBuff,(char *)buff,256);
	        mstrncat(WarningBuff,"\n",256);
		}
	}else if(Files->zmax > 0){
	 	sprintf(WarningBuff,"    Limits ixmax %ld iymax %ld izmax %ld\n%c",Files->xmax,Files->ymax,Files->zmax,0);
	}else if(Files->ymax > 0){
	 	sprintf(WarningBuff,"    Limits ixmax %ld iymax %ld\n%c",Files->xmax,Files->ymax,0);
	}
	
	WarningBatch(WarningBuff);
	
	sprintf(WarningBuff,"    Class '%s' Number of Images %ld Number of Files %ld\n%c",FilesClassTag(Files),Files->ImageCount,Files->FileCount,0);
	WarningBatch(WarningBuff);
	
	
	if(Files->FilesNames){
		nf=0;
		sprintf(WarningBuff,"      %c",0);
		WarningBatch(WarningBuff);
		for(n=0;n<Files->FileCount;++n){
			if(Files->FilesNames[n]){
				sprintf(WarningBuff,"%s  %c",Files->FilesNames[n],0);
				WarningBatch(WarningBuff);
			}
			if(++nf > 4){
				sprintf(WarningBuff,"\n      %c",0);
				WarningBatch(WarningBuff);
				nf=0;
			}
		}
	}
	sprintf(WarningBuff,"\n\n%c",0);
	WarningBatch(WarningBuff);
	return 0;
}

struct FileInfo2 *FilesReOpen(struct FileInfo2 *FilesIn)
{
    struct FileInfo2 *Files;
	struct FileList File;
	long n,nn;
	
	if(!FilesIn)return NULL;
	
    zerol((char *)&File,sizeof(File));
    File.donotfree=TRUE;
    File.directory=strsave(FilesIn->directory,3986);
    if(!File.directory)return NULL;
    File.name=(char **)cMalloc(sizeof(char **)*FilesIn->FileCount,3983);
    if(!File.name){
        if(File.directory)cFree((char *)File.directory);
        return NULL;;
    }
    nn=0;
	for(n=0;n<FilesIn->FileCount;++n){	    
    	File.name[n]=strsave(FilesIn->FilesNames[n],3986);
    	if(!File.name[n])break;	  
    	++nn;  
	}
    File.count=nn;
		    
    File.type=FilesIn->fileType;	    

	Files=FilesOpenList(&File);
		
	return 	Files;
	
}

int FilesAppendFiles(struct FileInfo2 *FilesOld,struct FileInfo2 *FilesNew)
{
	if(!FilesOld || !FilesNew)return 1;
	if(FilesOld->fileClass != FilesNew->fileClass)return 1;
	
	if(FilesOld->fileClass == HDF_FILES){
	    return FilesMergeHDF(FilesOld,FilesNew);
	}else if(FilesOld->fileClass == TRUE_FILES){
	    return  FilesMergeTRUE(FilesOld,FilesNew);
	}else if(FilesOld->fileClass == SDS2D_FILES){
	    return  FilesMergeSDS2D(FilesOld,FilesNew);
	}else if(FilesOld->fileClass == SDS3D_FILES){
/*      return  FilesMergeSDS3D(FilesOld,FilesNew); */
	}else if(FilesOld->fileClass == PICT_FILES){
	    return  FilesMergePICT(FilesOld,FilesNew);
	}
	
	
	
	return 1;
}
int FilesMergeTRUE(FilePtr Files,FilePtr FilesNew)
{
	long ImageCount;
	long xmax,ymax,xsize,ysize;
	int ret;
	long n;
	
	if(!Files || !FilesNew)return 1;
	if(Files->fileClass != TRUE_FILES)return 1;
	if(FilesNew->fileClass != TRUE_FILES)return 1;
	
	ret=1;
	ImageCount=Files->ImageCount;
	for(n=0;n<FilesNew->ImageCount;++n){
		if(CheckHdfSpace(Files))goto ErrorOut;
		Files->TRUEList[ImageCount]=FilesNew->TRUEList[n];
		
		FilesNew->TRUEList[n].buffer=NULL;
				
		Files->ImageNames[ImageCount]=FilesNew->ImageNames[n];
		FilesNew->ImageNames[n]=NULL;
		Files->ImageFiles[ImageCount]=0;
		xsize=Files->TRUEList[ImageCount].xsize;
		ysize=Files->TRUEList[ImageCount].ysize;
		
		Files->ImageCount = ++ImageCount;	
		
		xmax=Files->xmax;
		ymax=Files->ymax;
		if(xsize < Files->xmin)Files->xmin=xsize;
		if(xsize > Files->xmax)Files->xmax=xsize;
		if(ysize < Files->ymin)Files->ymin=ysize;
		if(ysize > Files->ymax)Files->ymax=ysize;
		if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
		    if(Files->dataSave)cFree((char *)Files->dataSave);
		    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
		    Files->dataSave=(unsigned char *)cMalloc(Files->length,8112);
		    if(Files->dataSave == NULL){
		        goto ErrorOut;
		    }
		    Files->CurrentFrame = -1;
		}		
		
	}
		
	if(Files->ImageCount == 1)doSDS2DStart(Files);	
	ret=0;
ErrorOut:

	if(FilesNew)freeFileList(FilesNew);
	
	return ret;	
}

int FilesMergeHDF(struct FileInfo2 *Files,struct FileInfo2 *FilesNew)
{
	long ImageCount;
	long xmax,ymax,xsize,ysize;
	int ret;
	long n;
	
	if(!Files || !FilesNew)return 1;
	if(Files->fileClass != HDF_FILES)return 1;
	if(FilesNew->fileClass != HDF_FILES)return 1;
	
	ret=1;
	ImageCount=Files->ImageCount;
	for(n=0;n<FilesNew->ImageCount;++n){
		if(CheckHdfSpace(Files))goto ErrorOut;
		Files->HDFList[ImageCount]=FilesNew->HDFList[n];
		
		FilesNew->HDFList[n].buffer=NULL;
		FilesNew->HDFList[n].limits.command=NULL;
		FilesNew->HDFList[n].limits.pioName=NULL;
				
		Files->ImageNames[ImageCount]=FilesNew->ImageNames[n];
		FilesNew->ImageNames[n]=NULL;
		Files->ImageFiles[ImageCount]=0;
		xsize=Files->HDFList[ImageCount].xsize;
		ysize=Files->HDFList[ImageCount].ysize;
		
		Files->ImageCount = ++ImageCount;	
		
		xmax=Files->xmax;
		ymax=Files->ymax;
		if(xsize < Files->xmin)Files->xmin=xsize;
		if(xsize > Files->xmax)Files->xmax=xsize;
		if(ysize < Files->ymin)Files->ymin=ysize;
		if(ysize > Files->ymax)Files->ymax=ysize;
		if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
		    if(Files->dataSave)cFree((char *)Files->dataSave);
		    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
		    Files->dataSave=(unsigned char *)cMalloc(Files->length,8112);
		    if(Files->dataSave == NULL){
		        goto ErrorOut;
		    }
		    Files->CurrentFrame = -1;
		}		
		
	}
		
	if(Files->ImageCount == 1)doSDS2DStart(Files);	
	ret=0;
ErrorOut:

	if(FilesNew)freeFileList(FilesNew);
	
	return ret;	
}
int FilesMergeSDS2D(struct FileInfo2 *Files,struct FileInfo2 *FilesNew)
{
	long ImageCount;
	long xmax,ymax,xsize,ysize;
	int ret;
	long n;
	
	if(!Files || !FilesNew)return 1;
	if(Files->fileClass != SDS2D_FILES)return 1;
	if(FilesNew->fileClass != SDS2D_FILES)return 1;
	
	ret=1;
	ImageCount=Files->ImageCount;
	for(n=0;n<FilesNew->ImageCount;++n){
		if(CheckHdfSpace(Files))goto ErrorOut;
		Files->SDS2DList[ImageCount]=FilesNew->SDS2DList[n];
		
		FilesNew->SDS2DList[n].buffer=NULL;
		FilesNew->SDS2DList[n].limits.command=NULL;
		FilesNew->SDS2DList[n].limits.pioName=NULL;
				
		Files->ImageNames[ImageCount]=FilesNew->ImageNames[n];
		FilesNew->ImageNames[n]=NULL;
		Files->ImageFiles[ImageCount]=0;
		xsize=Files->SDS2DList[ImageCount].xsize;
		ysize=Files->SDS2DList[ImageCount].ysize;
		
		Files->ImageCount = ++ImageCount;	
		
		xmax=Files->xmax;
		ymax=Files->ymax;
		if(xsize < Files->xmin)Files->xmin=xsize;
		if(xsize > Files->xmax)Files->xmax=xsize;
		if(ysize < Files->ymin)Files->ymin=ysize;
		if(ysize > Files->ymax)Files->ymax=ysize;
		if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
		    if(Files->dataSave)cFree((char *)Files->dataSave);
		    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
		    Files->dataSave=(unsigned char *)cMalloc(Files->length,8112);
		    if(Files->dataSave == NULL){
		        goto ErrorOut;
		    }
		    Files->CurrentFrame = -1;
		}		
		
	}
		
	if(Files->ImageCount == 1)doSDS2DStart(Files);	
	ret=0;
ErrorOut:

	if(FilesNew)freeFileList(FilesNew);
	
	return ret;	
}

struct FileInfo2 *FilesOpenList(struct FileList *FilesIn)
{
	struct FileInfo2 *Files;
	char name[2048];
	int rank,type;
	
	if(!FilesIn || !FilesIn->count || !FilesIn->directory || !FilesIn->name)return NULL;
	
	if(FilesSetType(FilesIn)){
		FileListFree(FilesIn);
		return NULL;
	}
	
	mstrncpy(name,FilesIn->directory,2048);
	strncatToPath(name,FilesIn->name[0],2048);
	
	Files=initFILES(FilesIn);
	if(!Files)return NULL;
	
	if(Files->fileType == TEK_DISK || Files->fileType == TEK_MEMORY){
TEKI:
		if(initTEK(Files))goto ErrorOut;
	}else if(Files->fileType == SAGEEOS_DISK || Files->fileType == SAGE_DUMP || Files->fileType == SAGE1D_DISK ||
	         Files->fileType == SAGE2D_DISK || Files->fileType == SAGE3D_DISK){
		if(!IsPIO(name,&rank)){
			goto Try3;
		}
PIO:
		if(initPIO(Files))goto ErrorOut;
		
	}else if(Files->fileType == SAGE1D_BLOCKED ||Files->fileType == SAGE2D_BLOCKED || Files->fileType == SAGE3D_BLOCKED){
		if(!IsPIOBlk(name,&rank)){
			goto Try3;
		}
		
PIOBLOCKED:
	if(initPIOBlk(Files))goto ErrorOut;
	
	}else if(Files->fileType == TRUE_DISK || Files->fileType == TRUE_MEMORY){
		if(!IsTRUE(name,&type)){
			goto Try3;
		}
TRUE2:
		if(initTRUE(Files))goto ErrorOut;		
	}else if(Files->fileType == RGB_DISK || Files->fileType == RGB_MEMORY){
		if(!IsRGB(name)){
			goto Try3;
		}
RGB:
		if(initRGB(Files))goto ErrorOut;		
	}else if(Files->fileType == SDS2D_DISK || Files->fileType == SDS2D_MEMORY || Files->fileType == SDS3D_DISK){
		if(!IsSDS(name,&rank)){
			goto Try3;
		}
SDS2D:
		if(rank == 2){
			if(initSDS2D(Files))goto ErrorOut;		
		}else if(rank == 3){
			if(initSDS3D(Files))goto ErrorOut;
		}			
	}else if(Files->fileType == HDF_DISK || Files->fileType == HDF_MEMORY){
		if(!IsHdf(name)){
			goto Try3;
		}
HDF:
		if(initHDF(Files))goto ErrorOut;	
	}else if(Files->fileType == BMP_DISK || Files->fileType == BMP_MEMORY){
		if(!IsBMP(name)){
			goto Try3;
		}
BMP:
		if(initBMP(Files))goto ErrorOut;
	}else if(Files->fileType == JPG_DISK || Files->fileType == JPG_MEMORY){
		if(!IsJPG(name)){
			goto Try3;
		}
JPG:
		if(initJPG(Files))goto ErrorOut;
	}else if(Files->fileType == GIF_DISK || Files->fileType == GIF_MEMORY){
		if(!IsGIF(name)){
			goto Try3;
		}
GIF:
		if(initGIF(Files))goto ErrorOut;
#ifdef Macintosh	
	}else if(Files->fileType == VIEWLIB_MEMORY){
		if(!IsViewLib(name)){
			goto Try3;
		}
		if(initVIEW(Files))goto ErrorOut;
	}else if(Files->fileType == PICT_DISK || Files->fileType == PICT_MEMORY){
		if(initPICT(Files))goto ErrorOut;	
	}else if(Files->fileType == QUICKTIME_DISK || Files->fileType == QUICKTIME_MEMORY){
		if(initQUICKTIME(Files))goto ErrorOut;	
#endif			
	}else if(Files->fileType == VRML_MEMORY || Files->fileType == VRML_DISK){
		if(!IsVRML(name)){
			goto Try3;
		}
VRML:
		if(initVRML(Files))goto ErrorOut;
	}else if(Files->fileType == SHOW_MEMORY || Files->fileType == SHOW_DISK){
		if(!IsSHOW(name)){
			goto Try3;
		}
SHOW:
		if(initSHOW(Files))goto ErrorOut;
	}else if(Files->fileType == STL_DISK){
		if(initSTL(Files))goto ErrorOut;
	}else if(Files->fileType == TEXT_MEMORY){
		Files->fileClass=TEXT_MEMORY;
		Files->pdefault=FilesOpenEdit;
	}else if(Files->fileType == CONF_DISK){
		Files->pdefault=FilesOpenUniversal;
	}else{
		goto Try3;
	}
	
	
	return Files;
Try3:
	if(IsTRUE(name,&type)){
		Files->fileType = TRUE_MEMORY;
		goto TRUE2;
	}
	if(IsRGB(name)){
		Files->fileType = RGB_MEMORY;
		goto RGB;
	}
	if(IsSDS(name,&rank)){
		Files->fileType = SDS2D_MEMORY;
		goto SDS2D;
	}
	if(IsHdf(name)){
		Files->fileType = HDF_MEMORY;
		goto HDF;
	}
	if(IsBMP(name)){
		Files->fileType = BMP_MEMORY;
		goto BMP;
	}
	if(IsJPG(name)){
		Files->fileType = JPG_MEMORY;
		goto JPG;
	}
	if(IsGIF(name)){
		Files->fileType = GIF_MEMORY;
		goto GIF;
	}
	
	if(IsPIO(name,&rank)){
		goto PIO;
	}
	
	if(IsPIOBlk(name,&rank)){
		goto PIOBLOCKED;
	}
	
	
	
	
	if(IsVRML(name)){
		goto VRML;
	}
	
	if(IsSHOW(name)){
		goto SHOW;
	}
	
	if(IsTEK(name)){
		Files->fileType = TEK_MEMORY;
		goto TEKI;
	}
	
	sprintf(WarningBuff,"FilesOpenList Type For File %s Unknown\n",name);
	WarningBatch(WarningBuff);
	
	if(Files)freeFileList(Files);
ErrorOut:
	return (struct FileInfo2 *)NULL;
}
int FilesSetType(struct FileList *Files)
{
	char filepath[2048];
	int type,rank;
	FILE *in;
	
	if(!Files)return 1;
			
    filepath[0]=0;
	mstrncpy(filepath,Files->directory,2048);
	strncatToPath(filepath,Files->name[0],2048);
	
	if(isDirectory(filepath))return 0;
	    		
	in=fopen(filepath,"r");
	if(in == NULL){
	    sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",filepath);
	    WarningBatch(WarningBuff);
		return 1;	
	}
	
	fclose(in);
	
	if(IsTRUE(filepath,&type)){
	    if(Files->type != TRUE_DISK && Files->type != TRUE_MEMORY){
			Files->type = TRUE_DISK;
	    }
	    return 0;
	}
	    
    if(IsPIO(filepath,&rank)){
        if(Files->type == SAGE_DUMP)return 0;
        if(rank == 1){
	        Files->type = SAGE1D_DISK;
        }else if(rank == 2){
	        Files->type = SAGE2D_DISK;
        }else if(rank == 3){
	        Files->type = SAGE3D_DISK;
        }else if(rank == -1){
	        Files->type = SAGEEOS_DISK;
	    }
	    return 0;
    }
        
    if(IsPIOBlk(filepath,&rank)){
        if(rank == 1){
	        Files->type = SAGE1D_BLOCKED;
        }else if(rank == 2){
	        Files->type = SAGE2D_BLOCKED;
        }else if(rank == 3){
	        Files->type = SAGE3D_BLOCKED;
        }
	    return 0;
    }
        

	if(IsSDS(filepath,&rank)){
		if(rank == 2){
			if(Files->type != SDS2D_DISK && Files->type != SDS2D_MEMORY){
				Files->type = SDS2D_DISK;
			}
		}else if(rank == 3){
			Files->type = SDS3D_DISK;
		}
		return 0;
	}

	if(IsRGB(filepath)){
	    if(Files->type != RGB_DISK && Files->type != RGB_MEMORY){
			Files->type = RGB_DISK;
	    }
	    return 0;
	}

	if(IsHdf(filepath)){
	    if(Files->type != HDF_DISK && Files->type != HDF_MEMORY){
			Files->type = HDF_DISK;
	    }
	    return 0;
	}
	
	if(IsJPG(filepath)){
	    if(Files->type != JPG_DISK && Files->type != JPG_MEMORY){
			Files->type = JPG_DISK;
	    }
	    return 0;
	}
	
	if(IsVRML(filepath)){
	    if(Files->type != VRML_MEMORY && Files->type != VRML_DISK){
			Files->type = VRML_DISK;
	    }
	    return 0;
	}
	
	
	if(IsViewLib(filepath)){
	    if(Files->type != VIEWLIB_MEMORY){
	       Files->type = VIEWLIB_MEMORY;
	    }
	    return 0;
	}
	
	
	if(IsSHOW(filepath)){
	    if(Files->type != SHOW_MEMORY && Files->type != SHOW_DISK){
			Files->type = SHOW_DISK;
	    }
	    return 0;
	}
	

	if(IsGIF(filepath)){
	    if(Files->type != GIF_DISK && Files->type != GIF_MEMORY){
			Files->type = GIF_DISK;
	    }
	    return 0;
	}
	
	if(IsBMP(filepath)){
	    if(Files->type != BMP_DISK && Files->type != BMP_MEMORY){
			Files->type = BMP_DISK;
	    }
	    return 0;
	}

	if(IsSTL(filepath)){
	    Files->type = STL_DISK;
	    return 0;
	}
	
	if(IsTEXT(filepath)){
		Files->type = TEXT_MEMORY;
	    return 0;
	}

	if(IsTEK(filepath)){
	    if(Files->type != TEK_DISK && Files->type != TEK_MEMORY){
			Files->type = TEK_DISK;
	    }
	    return 0;
	}
	
	if(IsQUICKTIME(filepath)){
	    if(Files->type != QUICKTIME_DISK && Files->type != QUICKTIME_MEMORY){
			Files->type = QUICKTIME_DISK;
	    }
	    return 0;
	}
	
	if(IsPICT(filepath)){
	    if(Files->type != PICT_DISK && Files->type != PICT_MEMORY){
			Files->type = PICT_DISK;
	    }
	    return 0;
	}

	if(IsCONF(filepath)){
		Files->type = CONF_DISK;
	    return 0;
	}
	
	return 0;
}


struct FileInfo2 *GetFileInfoStruct(int tag)
{
	struct FileInfo2 *Files;


	Files=(struct FileInfo2 *)cMalloc(sizeof(struct FileInfo2),tag);
	if(Files){
		zerol((char *)Files,sizeof(struct FileInfo2));
		
	    Files->xmin = 20000;
	    Files->ymin = 20000;
	    Files->zmin = 20000;
	    Files->xmax = -20000;
	    Files->ymax = -20000;
	    Files->zmax = -20000;

		Files->xminr=  1e60;
		Files->yminr=  1e60;
		Files->zminr=  1e60;
		Files->xmaxr= -1e60;
		Files->ymaxr= -1e60;
		Files->zmaxr= -1e60;
		mstrncpy(Files->paletteName,"ps",3);
		getPaletteByName(Files->paletteName,(unsigned char *)Files->palette);
		
	}
	
	FileOpenSave(Files);

	return Files;
}


struct FileInfo2 *initFILES(struct FileList *FilesIn)
{
        struct FileInfo2 *Files;
        long nf,count;
        char **FilesNames;

		if(!FilesIn)return (struct FileInfo2 *)NULL;
		

		Files=GetFileInfoStruct(8143);
		if(!Files){
	    	WarningBatch("initFILES out of Memory\n");
	    	goto ErrorOut;
		}

		count=FilesIn->count;

		FilesNames=(char **)cMalloc(count*sizeof(char *),8144);
		if(!FilesNames){
	    	sprintf(WarningBuff,"initFILES Error saving FilesNames List count %ld\n",count);
	    	WarningBatch(WarningBuff);
	    	goto ErrorOut;
		}
		zerol((char *)FilesNames,count*sizeof(char *));

		Files->FilesNames=FilesNames;

		count=0;
		for(nf=0;nf<FilesIn->count;++nf){
	    	FilesNames[count]=strsave(FilesIn->name[nf],8241);
	    	if(!FilesNames[count]){
	        	sprintf(WarningBuff,"initFILES Error saving File Name %s\n",
	               	FilesIn->name[nf]);
				WarningBatch(WarningBuff);
	        	goto ErrorOut;
	    	}
	    	Files->FileCount = ++count;
		}

		Files->FileTagCount=FileTagCount++;
		
		Files->fileType=(int)FilesIn->type;

		mstrncpy(Files->directory,FilesIn->directory,1024);

		Files->CurrentFrame = -1;
		
		Files->ImageIncrement=FilesIn->ImageIncrement;
		
		Files->loadToMemory=FilesIn->loadToMemory;
				
		FileListFree(FilesIn);
		
		return Files;

ErrorOut:
		freeFileList(Files);
		FileListFree(FilesIn);
		return (struct FileInfo2 *)NULL;
}
int FilesOpenDefault(struct FileInfo2 *Files)
{
	if(!Files)return 1;

	if(Files->pdefault){
	    return (*Files->pdefault)(Files);
	}

	if(Files->FilesNames && Files->FilesNames[0]){
	    sprintf(WarningBuff,"FilesOpenDefault (%s)- No default Open Routine\n",Files->FilesNames[0]);
	    WarningBatch(WarningBuff);
	}else{
	    WarningBatch("FilesOpenDefault - No default Open Routine\n");
	}
	return 1;
}
int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files)
{
	if(!Files || !data || !length)return 1;

	if(Files->pGetData){
	    return (*Files->pGetData)(data,length,CurrentFrame,Files);
	}

	WarningBatch("FilesGetData - No default GetData Routine\n");
	return 1;
}
char *FilesClassTag(struct FileInfo2 *Files)
{
	static char *names[]={
	  						HDF_FILES_TAG,TRUE_FILES_TAG,SDS2D_FILES_TAG,
	  						SDS3D_FILES_TAG,TEK_FILES_TAG,RGB_FILES_TAG,
	  						PICT_FILES_TAG,VIEW_FILES_TAG,BMP_FILES_TAG,
	  						GIF_FILES_TAG,JPG_FILES_TAG,QUICKTIME_FILES_TAG,
	  						PIO_FILES_TAG,PIO1D_FILES_TAG,PIO2D_FILES_TAG,
	  						PIO3D_FILES_TAG,VRML_FILES_TAG,SHOW_FILES_TAG,
	  						PIOEOS_FILES_TAG,STL_FILES_TAG,	
							PIO_BLK_FILES_TAG,PIO1D_BLK_FILES_TAG,PIO2D_BLK_FILES_TAG,
							PIO3D_BLK_FILES_TAG,
						 };
	static int types[]={
							HDF_FILES,TRUE_FILES,SDS2D_FILES,
							SDS3D_FILES,TEK_FILES,RGB_FILES,
							PICT_FILES,VIEW_FILES,BMP_FILES,
							GIF_FILES,JPG_FILES,QUICKTIME_FILES,
							PIO_FILES,PIO1D_FILES,PIO2D_FILES,
							PIO3D_FILES,VRML_FILES,SHOW_FILES,
							PIOEOS_FILES,STL_FILES,PIO_BLK_FILES,
							PIO1D_BLK_FILES,PIO2D_BLK_FILES,
							PIO3D_BLK_FILES,							
						};
						
	int n;
	
	if(!Files)return NULL;
	
	for(n=0;n<sizeof(types)/sizeof(int);++n){
	    if(Files->fileClass == types[n]){
	        return names[n];
	    }
	}
	
	return NULL;
}
char *FilesTypeName(struct FileList *Files)
{
	static char *names[]={
				HDF_DISK_NAME,
				HDF_MEMORY_NAME,
				PICT_DISK_NAME,
				PICT_MEMORY_NAME,
				TRUE_DISK_NAME,
				TRUE_MEMORY_NAME,
				TEK_DISK_NAME,
				TEK_MEMORY_NAME,
				RGB_DISK_NAME,
				RGB_MEMORY_NAME,
				SDS2D_DISK_NAME,
				SDS2D_MEMORY_NAME,
				VIEWLIB_MEMORY_NAME,
				BMP_DISK_NAME,
				BMP_MEMORY_NAME,
				GIF_DISK_NAME,
				GIF_MEMORY_NAME,
				JPG_DISK_NAME,
				JPG_MEMORY_NAME,
				QUICKTIME_DISK_NAME,
				QUICKTIME_MEMORY_NAME,
				TEXT_MEMOR_NAME,
				
				SAGE1D_DISK_NAME,
				SAGE2D_DISK_NAME,
				SAGE3D_DISK_NAME,
				VRML_MEMORY_NAME,
				VRML_DISK_NAME,
				SDS3D_MEMORY_NAME,
				SDS3D_DISK_NAME,
				SHOW_MEMORY_NAME,
				SHOW_DISK_NAME,
				SAGEEOS_DISK_NAME,
				STL_DISK_NAME,
				CONF_DISK_NAME,
				
						 };
	static int types[]={
				HDF_DISK,
				HDF_MEMORY,
				PICT_DISK,
				PICT_MEMORY,
				TRUE_DISK,
				TRUE_MEMORY,
				TEK_DISK,
				TEK_MEMORY,
				RGB_DISK,
				RGB_MEMORY,
				SDS2D_DISK,
				SDS2D_MEMORY,
				VIEWLIB_MEMORY,
				BMP_DISK,
				BMP_MEMORY,
				GIF_DISK,
				GIF_MEMORY,
				JPG_DISK,
				JPG_MEMORY,
				QUICKTIME_DISK,
				QUICKTIME_MEMORY,
				TEXT_MEMORY,
				
				SAGE1D_DISK,
				SAGE2D_DISK,
				SAGE3D_DISK,
				VRML_MEMORY,
				VRML_DISK,
				SDS3D_MEMORY,
				SDS3D_DISK,
				SHOW_MEMORY,
				SHOW_DISK,
				SAGEEOS_DISK,
				STL_DISK,
				CONF_DISK,
						};
						
	int n;
	
	if(!Files)return "Unknown Type";
	
	for(n=0;n<sizeof(types)/sizeof(int);++n){
	    if(Files->type == types[n]){
	        return names[n];
	    }
	}
	
	return "Unknown Type";
}
char *FilesClassName(struct FileInfo2 *Files)
{
	static char *names[]={
	  						HDF_FILES_NAME,TRUE_FILES_NAME,SDS2D_FILES_NAME,
	  						SDS3D_FILES_NAME,TEK_FILES_NAME,RGB_FILES_NAME,
	  						PICT_FILES_NAME,VIEW_FILES_NAME,BMP_FILES_NAME,
	  						GIF_FILES_NAME,JPG_FILES_NAME,QUICKTIME_FILES_NAME,
	  						PIO_FILES_NAME,PIO1D_FILES_NAME,PIO2D_FILES_NAME,
	  						PIO3D_FILES_NAME,VRML_FILES_NAME,SHOW_FILES_NAME,
	  						PIOEOS_FILES_NAME,	
						 };
	static int types[]={
							HDF_FILES,TRUE_FILES,SDS2D_FILES,
							SDS3D_FILES,TEK_FILES,RGB_FILES,
							PICT_FILES,VIEW_FILES,BMP_FILES,
							GIF_FILES,JPG_FILES,QUICKTIME_FILES,
							PIO_FILES,PIO1D_FILES,PIO2D_FILES,
							PIO3D_FILES,VRML_FILES,SHOW_FILES,	
							PIOEOS_FILES,						
						};
						
	int n;
	
	if(!Files)return NULL;
	
	for(n=0;n<sizeof(types)/sizeof(int);++n){
	    if(Files->fileClass == types[n]){
	        return names[n];
	    }
	}
	
	return NULL;
}

char *FilesImageName(struct FileInfo2 *Files,long n)
{
	static char buff[256];
	
	if(!Files){
		sprintf(buff,"FilePointerNULL.%04ld%c",n,0);
		return buff;
	}
	
	if(n >= 0 && n < Files->ImageCount){
		if(Files->ImageNames && Files->ImageNames[n])return Files->ImageNames[n];
	}
	
		
	if(n >= 0 && n < Files->FileCount){
	    if(Files->FilesNames && Files->FilesNames[n])return Files->FilesNames[n];
	}
		
	sprintf(buff,"ImageNoName.%04ld%c",n,0);
	return buff;
	
}
int FilesSetFrame(long CurrentFrame,struct FileInfo2 *Files)
{
	int ret,inputType;
	
	if(!Files)return 1;
	if(Files->pSetFrame){
	    inputType=Files->inputType;
	    if(Files->Reflect_x)Files->inputType=TRUE_EXPANDED;
	    ret=(*Files->pSetFrame)(CurrentFrame,Files);
	    Files->inputType=inputType;
	    if(ret)return ret;
	    if(Files->Reflect_x)ret=FilesReflectX(Files);	   
	    return ret;
	}
	WarningBatch("FilesSetFrame - No Routine Found\n");
	return  1;
}
int freeFileList(struct FileInfo2 *Files)
{
	long n;


	if(!Files)return 1;

	FileOpenRemove(Files);

	if(Files->SaveDirectory){
	    cFree((char *)Files->SaveDirectory);
	    Files->SaveDirectory=NULL;
	}
	
	if(Files->reflectSave){
		cFree((char *)Files->reflectSave);
		Files->reflectSave=NULL;
	}

	if(Files->FilesNames){
	    for(n=0;n<Files->FileCount;++n){
	        if(Files->FilesNames[n]){
	            cFree((char *)Files->FilesNames[n]);
	            Files->FilesNames[n]=NULL;
	        }
	    }
	    cFree((char *)Files->FilesNames);
	    Files->FilesNames=NULL;
	}

	if(Files->ImageNames){
	    for(n=0;n<Files->ImageCount;++n){
	        if(Files->ImageNames[n]){
	            cFree((char *)Files->ImageNames[n]);
	            Files->ImageNames[n]=NULL;
	        }
	    }
	    cFree((char *)Files->ImageNames);
	    Files->ImageNames=NULL;
	}

	if(Files->ImageFiles){
	    cFree((char *)Files->ImageFiles);
	    Files->ImageFiles=NULL;
	}
	
	if(Files->limits.pioName)cFree((char *)Files->limits.pioName);
	Files->limits.pioName=NULL;

	if(Files->limits.command)cFree((char *)Files->limits.command);
	Files->limits.command=NULL;

	if(Files->fileClass == HDF_FILES){
	    if(Files->HDFList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->HDFList[n].buffer){
		        	cFree((char *)Files->HDFList[n].buffer);
		        	Files->HDFList[n].buffer=NULL;
	            }
	            if(Files->HDFList[n].limits.command){
		        	cFree((char *)Files->HDFList[n].limits.command);
		        	Files->HDFList[n].limits.command=NULL;
	            }
	            if(Files->HDFList[n].limits.pioName){
		        	cFree((char *)Files->HDFList[n].limits.pioName);
		        	Files->HDFList[n].limits.pioName=NULL;
	            }
	        }
	        cFree((char *)Files->HDFList);
	        Files->HDFList=NULL;
	    }
	}else if(Files->fileClass == TEK_FILES){
	    if(Files->TEKList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->TEKList[n].buffer){
		        	cFree((char *)Files->TEKList[n].buffer);
		        	Files->TEKList[n].buffer=NULL;
	            }
	        }
	        cFree((char *)Files->TEKList);
	        Files->TEKList=NULL;
	    }
	}else if(Files->fileClass == VRML_FILES || Files->fileClass == STL_FILES){
	    if(Files->VRMLList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->VRMLList[n].buffer){
		        	cFree((char *)Files->VRMLList[n].buffer);
	            }
	        }
	        cFree((char *)Files->VRMLList);
	        Files->VRMLList=NULL;
	    }
	}else if(Files->fileClass == TRUE_FILES){
	    if(Files->TRUEList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->TRUEList[n].buffer){
		        	cFree((char *)Files->TRUEList[n].buffer);
		        	Files->TRUEList[n].buffer=NULL;
	            }
	        }
	        cFree((char *)Files->TRUEList);
	        Files->TRUEList=NULL;
	    }
	}else if(Files->fileClass == BMP_FILES){
	    if(Files->BMPList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->BMPList[n].buffer){
		        cFree((char *)Files->BMPList[n].buffer);
	            }
	        }
	        cFree((char *)Files->BMPList);
	        Files->BMPList=NULL;
	    }
	}else if(Files->fileClass == JPG_FILES){
	    if(Files->JPGList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->JPGList[n].buffer){
		        cFree((char *)Files->JPGList[n].buffer);
	            }
	        }
	        cFree((char *)Files->JPGList);
	        Files->JPGList=NULL;
	    }
	}else if(Files->fileClass == GIF_FILES){
	    if(Files->GIFList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->GIFList[n].buffer){
		        cFree((char *)Files->GIFList[n].buffer);
	            }
	        }
	        cFree((char *)Files->GIFList);
	        Files->GIFList=NULL;
	    }
	}else if(Files->fileClass == RGB_FILES){
	    if(Files->RGBList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->RGBList[n].buffer){
		        	cFree((char *)Files->RGBList[n].buffer);
		        	Files->RGBList[n].buffer=NULL;
	            }
	        }
	        cFree((char *)Files->RGBList);
	        Files->RGBList=NULL;
	    }
	}else if(Files->fileClass == SDS2D_FILES){
	    if(Files->SDS2DList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->SDS2DList[n].buffer){
		        	cFree((char *)Files->SDS2DList[n].buffer);
		        	Files->SDS2DList[n].buffer=NULL;
	            }
	            if(Files->SDS2DList[n].limits.command){
		        	cFree((char *)Files->SDS2DList[n].limits.command);
		        	Files->SDS2DList[n].limits.command=NULL;
	            }
	            if(Files->SDS2DList[n].limits.pioName){
		        	cFree((char *)Files->SDS2DList[n].limits.pioName);
		        	Files->SDS2DList[n].limits.pioName=NULL;
	            }
	        }
	        cFree((char *)Files->SDS2DList);
	        Files->SDS2DList=NULL;
	    }
	}else if(Files->fileClass == SDS3D_FILES){
	    if(Files->SDS3DList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->SDS3DList[n].buffer){
		        	cFree((char *)Files->SDS3DList[n].buffer);
		        	Files->SDS3DList[n].buffer=NULL;
	            }
	            if(Files->SDS3DList[n].limits.command){
		        	cFree((char *)Files->SDS3DList[n].limits.command);
		        	Files->SDS3DList[n].limits.command=NULL;
	            }
	            if(Files->SDS3DList[n].limits.pioName){
		        	cFree((char *)Files->SDS3DList[n].limits.pioName);
		        	Files->SDS3DList[n].limits.pioName=NULL;
	            }
	        }
	        cFree((char *)Files->SDS3DList);
	        Files->SDS3DList=NULL;
	    }
#ifdef Macintosh
	}else if(Files->fileClass == PICT_FILES){
	    if(Files->PICTList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->PICTList[n].buffer){
		        	cFree((char *)Files->PICTList[n].buffer);
		        	Files->PICTList[n].buffer=NULL;
	            }
	        }
	        cFree((char *)Files->PICTList);
	        Files->PICTList=NULL;
	    }
	}else if(Files->fileClass == QUICKTIME_FILES){
	    PicHandle moviePict;
	    if(Files->QUICKTIMEList){
	        if(Files->QUICKTIMEList[0].movie){
	    	    DisposeMovie( Files->QUICKTIMEList[0].movie );
	    	    Files->QUICKTIMEList[0].movie = NULL;
		    }
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->QUICKTIMEList[n].buffer){
	                moviePict=(PicHandle)Files->QUICKTIMEList[n].buffer;
	                KillPicture(moviePict);
		        	Files->QUICKTIMEList[n].buffer=NULL;
	            }
	        }
	        cFree((char *)Files->QUICKTIMEList);
	        Files->QUICKTIMEList=NULL;
	    }
#endif
	}else if(Files->fileClass == SHOW_FILES){
	    if(Files->SHOWList){
	        for(n=0;n<Files->ImageCount;++n){
		    	struct SHOW *s;
	            s = &Files->SHOWList[n];
			    if(s->x)cFree((char *)s->x);
			    if(s->y)cFree((char *)s->y);
			    if(s->z)cFree((char *)s->z);
			    if(s->v)cFree((char *)s->v);

			    if(s->nx)cFree((char *)s->nx);
			    if(s->ny)cFree((char *)s->ny);
			    if(s->nz)cFree((char *)s->nz);

			    if(s->dx)cFree((char *)s->dx);
			    if(s->dy)cFree((char *)s->dy);
			    if(s->dz)cFree((char *)s->dz);

			    if(s->xvel)cFree((char *)s->xvel);
			    if(s->yvel)cFree((char *)s->yvel);
			    if(s->zvel)cFree((char *)s->zvel);

			    if(s->r)cFree((char *)s->r);
			    if(s->s)cFree((char *)s->s);
			    if(s->stress)cFree((char *)s->stress);

			    if(s->elements)cFree((char *)s->elements);
	        }
	        cFree((char *)Files->SHOWList);
	        Files->SHOWList=NULL;
	    }
	}else if(Files->fileClass == PIOEOS_FILES || Files->fileClass == PIO2D_FILES ||
	         Files->fileClass == PIO1D_FILES  ||  Files->fileClass == PIO3D_FILES){
	    if(Files->PIO2DList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->PIO2DList[n].data){
	               sageClose(&Files->PIO2DList[n],0);
	            }
	        }
	        cFree((char *)Files->PIO2DList);
	        Files->PIO2DList=NULL;
	    }
	}else if(Files->fileClass == PIO1D_BLK_FILES ||
	         Files->fileClass == PIO2D_BLK_FILES  ||  Files->fileClass == PIO3D_BLK_FILES){
	    if(Files->PIOBlkList){
	        for(n=0;n<Files->ImageCount;++n){
	            if(Files->PIOBlkList){
	               sageCloseBlk(&Files->PIOBlkList[n],0);
	            }
	        }
	        cFree((char *)Files->PIOBlkList);
	        Files->PIOBlkList=NULL;
	    }
	}
	
	
	{
    	struct FilePIOInfo *pio;
		pio = &Files->pioData;
	    if(pio){
	    
			if(pio->xcenter)cFree((char *)pio->xcenter);
			pio->xcenter=NULL;
			if(pio->ycenter)cFree((char *)pio->ycenter);
			pio->ycenter=NULL;
			if(pio->zcenter)cFree((char *)pio->zcenter);
			pio->zcenter=NULL;
			if(pio->level)cFree((char *)pio->level);
			pio->level=NULL;
			if(pio->daughter)cFree((char *)pio->daughter);
			pio->daughter=NULL;
			if(pio->value)cFree((char *)pio->value);
			pio->value=NULL;
			if(pio->gradx)cFree((char *)pio->gradx);
			pio->gradx=NULL;
			if(pio->grady)cFree((char *)pio->grady);
			pio->grady=NULL;
			if(pio->gradz)cFree((char *)pio->gradz);
			pio->gradz=NULL;
			if(pio->location)cFree((char *)pio->location);
			pio->location=NULL;
			if(pio->vectorx)cFree((char *)pio->vectorx);
			pio->vectorx=NULL;	    
			if(pio->vectory)cFree((char *)pio->vectory);
			pio->vectory=NULL;	    
			if(pio->vectorz)cFree((char *)pio->vectorz);
			pio->vectorz=NULL;
			if(pio->active)cFree((char *)pio->active);
			pio->active=NULL;
			if(pio->LaserPlotList)cFree((char *)pio->LaserPlotList);
			pio->LaserPlotList=NULL;
			if(pio->TracerPlotList)cFree((char *)pio->TracerPlotList);
			pio->TracerPlotList=NULL;
			if(pio->bt1)cFree((char *)pio->bt1);
			pio->bt1=NULL;
			if(pio->bt2)cFree((char *)pio->bt2);
			pio->bt2=NULL;
			if(pio->bt3)cFree((char *)pio->bt3);
			pio->bt3=NULL;
			if(pio->bt4)cFree((char *)pio->bt4);
			pio->bt4=NULL;
	    	    
	    }
	}
	
	{
 		struct vScene *scene2;
   		SagePtr pio2;
		pio2 = &Files->vmyIcon.pio;
	    if(pio2){
	    
			if(pio2->xcenter)cFree((char *)pio2->xcenter);
			pio2->xcenter=NULL;
			if(pio2->ycenter)cFree((char *)pio2->ycenter);
			pio2->ycenter=NULL;
			if(pio2->zcenter)cFree((char *)pio2->zcenter);
			pio2->zcenter=NULL;
			if(pio2->level)cFree((char *)pio2->level);
			pio2->level=NULL;
			if(pio2->daughter)cFree((char *)pio2->daughter);
			pio2->daughter=NULL;
			if(pio2->value)cFree((char *)pio2->value);
			pio2->value=NULL;
			if(pio2->gradx)cFree((char *)pio2->gradx);
			pio2->gradx=NULL;
			if(pio2->grady)cFree((char *)pio2->grady);
			pio2->grady=NULL;
			if(pio2->gradz)cFree((char *)pio2->gradz);
			pio2->gradz=NULL;
			if(pio2->location)cFree((char *)pio2->location);
			pio2->location=NULL;
			if(pio2->hasData)cFree((char *)pio2->hasData);
			pio2->hasData=NULL;
	    	    
	    }
		scene2=&Files->vmyIcon.scene;
		if(scene2){
	/*
			if(scene2->cdata){
			    cFree((char *)scene2->cdata);
			    scene2->cdata=NULL;
			}
	*/

			if(scene2->opacity){
			    cFree((char *)scene2->opacity);
			    scene2->opacity=NULL;
			}

			if(scene2->mat_opacity){
			    cFree((char *)scene2->mat_opacity);
			    scene2->mat_opacity=NULL;
			}
		}
	}
	
	if(Files->in){
	    fclose(Files->in);
	    Files->in=NULL;
	}
	
	if(Files->in8){
	    fclose8(Files->in8);
	    Files->in8=NULL;
	}
	
	
	if(Files->dataSave){
	    cFree((char *)Files->dataSave);
	    Files->dataSave=NULL;
	}
	
	freeStreamLine3d(&Files->pioData.streamline3d);
	
	freeStreamLine2d(&Files->pioData.streamline2d);
	
	cFree((char *)Files);

	return 0;

}
int AddjustType(unsigned char *d,long length,int floatType)
{
	if(!d)return 1;
	
	//length=length;
	//floatType=floatType;
#ifdef PC
/*   do PC */
    {
        unsigned char c,*cp;
	    long np;
	    cp=(unsigned char *)d;
	    if(floatType == DFNT_FLOAT64){
	        for(np=0;np<length;np += 8){
			    c=cp[np];
			    cp[np]=cp[np+7];
			    cp[np+7]=c;
			    c=cp[np+1];
			    cp[np+1]=cp[np+6];
			    cp[np+6]=c;
			    c=cp[np+2];
			    cp[np+2]=cp[np+5];
			    cp[np+5]=c;
			    c=cp[np+3];
			    cp[np+3]=cp[np+4];
			    cp[np+4]=c;
	        }
	    }else{
	        for(np=0;np<length;np += 4){
			    c=cp[np];
			    cp[np]=cp[np+3];
			    cp[np+3]=c;
			    c=cp[np+1];
			    cp[np+1]=cp[np+2];
			    cp[np+2]=c;
	        }
	    }
    }
#endif
	return 0;
}
int FilePutLimitsDD(unsigned char *d,long length,int floatType,long CurrentFrame,struct FileInfo2 *Files)
{
	double xs[10000],ys[10000],zs[10000];
	double *dp;
	float *fp;
	long xsize,ysize,zsize;
	long lengthx;
	int k;
	
	if(!d || !Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != SDS3D_FILES){
		WarningBatch("FilePutLimitsDD Only does SDS3D files\n");
		return 1;
	}
	if(!Files->SDS3DList)return 1;
	if(d[0] != 1 || d[1] != 1 || d[2] != 1){
		WarningBatch("FilePutLimitsDD Limits Error\n");
		return 1;
	}
	
	length -= 3;
	
	xsize=Files->SDS3DList[CurrentFrame].xsize;
	ysize=Files->SDS3DList[CurrentFrame].ysize;
	zsize=Files->SDS3DList[CurrentFrame].zsize;
	
	if(floatType == DFNT_FLOAT64){
	   lengthx=(xsize+ysize+zsize)*8;
	}else{
	   lengthx=(xsize+ysize+zsize)*4;
	}
	
	if(length != lengthx){
		sprintf(WarningBuff,"FilePutLimitsDD Error length %ld lengthx %ld\n",length,lengthx);
		WarningBatch(WarningBuff);
		return 1;
	}
	
	for(k=0;k<10000;++k){
	   xs[k]=0;
	   ys[k]=0;
	   zs[k]=0;
	}
	
	d += 3;
	
	AddjustType(d,length,floatType);
	
	if(floatType == DFNT_FLOAT64){
	   dp=(double *)(d);
	   for(k=0;k<zsize;++k){
	      zs[k]=*dp++;
	   }
	   for(k=0;k<ysize;++k){
	      ys[k]=*dp++;
	   }
	   for(k=0;k<xsize;++k){
	      xs[k]=*dp++;
	   }
	}else{
	   fp=(float *)(d);
	   for(k=0;k<zsize;++k){
	      zs[k]=*fp++;
	   }
	   for(k=0;k<ysize;++k){
	      ys[k]=*fp++;
	   }
	   for(k=0;k<xsize;++k){
	      xs[k]=*fp++;
	   }
	}
	
	
	return 0;
}

int FilePutLimitsString(char *command,long CurrentFrame,struct FileInfo2 *Files)
{
    struct CExpress e;
    char buff[512];
    struct LIMITS *lim;
    struct HDF *h;
    struct SDS2D *s2;
    struct SDS3D *s3;
    double sign;
        
	if(!command || !Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	
	lim = NULL;
	if(Files->fileClass == HDF_FILES){
	    if(!Files->HDFList)return 1;
	    h = &Files->HDFList[CurrentFrame];
	    if(!h)return 1;
	    lim = &h->limits;
	}else if(Files->fileClass == SDS2D_FILES){
	    if(!Files->SDS2DList)return 1;
	    s2 = &Files->SDS2DList[CurrentFrame];
	    if(!s2)return 1;
	    lim = &s2->limits;
	}else if(Files->fileClass == SDS3D_FILES){
	    if(!Files->SDS3DList)return 1;
	    s3 = &Files->SDS3DList[CurrentFrame];
	    if(!s3)return 1;
	    lim = &s3->limits;
	}
	
	
	if(!lim)return 1;
	
	if(lim->command)cFree((char *)lim->command);
	lim->command=(unsigned char*)command;	

	zerol((char *)&e,sizeof(struct CExpress));
	e.dat=(unsigned char *)command;
	while(!CExpressNext(&e)){
		 if(e.tok == T_EOF)break;
	     mstrncpy(buff,e.ch,512);
 	     if(CExpressNext(&e))break;
		 if(e.tok == T_MINUS){
 	         if(CExpressNext(&e))break;
		     sign = -1.0;
		 }else{
		     sign = 1.0;
		 }
		if(!strcmp(buff,"dolog")){
		    lim->dolog=atoi(e.ch);
		}else if(!strcmp(buff,"xmin")){
		    lim->xmin=(sign*atof(e.ch));
		    if(lim->xmin < Files->xminr)Files->xminr=lim->xmin;
		    lim->iGotData   |= 1;
		    Files->iGotData |= 1;
		}else if(!strcmp(buff,"xmax")){
		    lim->xmax=(sign*atof(e.ch));
		    if(lim->xmax > Files->xmaxr)Files->xmaxr=lim->xmax;
		    lim->iGotData   |= 2;
		    Files->iGotData |= 2;
		}else if(!strcmp(buff,"ymin")){
		    lim->ymin=(sign*atof(e.ch));
		    if(lim->ymin < Files->yminr)Files->yminr=lim->ymin;
		    lim->iGotData   |= 4;
		    Files->iGotData |= 4;
		}else if(!strcmp(buff,"ymax")){
		    lim->ymax=(sign*atof(e.ch));
		    if(lim->ymax > Files->ymaxr)Files->ymaxr=lim->ymax;
		    lim->iGotData   |= 8;
		    Files->iGotData |= 8;
		}else if(!strcmp(buff,"zmin")){
		    lim->zmin=(sign*atof(e.ch));
		    if(lim->zmin < Files->zminr)Files->zminr=lim->zmin;
		    lim->iGotData   |= 16;
		    Files->iGotData |= 16;
		}else if(!strcmp(buff,"zmax")){
		    lim->zmax=(sign*atof(e.ch));
		    if(lim->zmax > Files->zmaxr)Files->zmaxr=lim->zmax;
		    lim->iGotData   |= 32;
		    Files->iGotData |= 32;
		}else if(!strcmp(buff,"vmin")){
		    lim->vmin=(sign*atof(e.ch));
		    if(lim->vmin < Files->vminr)Files->vminr=lim->vmin;
		    lim->iGotData   |= 64;
		    Files->iGotData |= 64;
		}else if(!strcmp(buff,"vmax")){
		    lim->vmax=(sign*atof(e.ch));
		    if(lim->vmax > Files->vmaxr)Files->vmaxr=lim->vmax;
		    lim->iGotData   |= 128;
		    Files->iGotData |= 128;
		}else if(!strcmp(buff,"pioIndex")){
		    lim->pioIndex=(long)(sign*atof(e.ch));
		}else if(!strcmp(buff,"time")){
		    lim->time=sign*atof(e.ch);
		    lim->iGotTime=TRUE;
		}else if(!strcmp(buff,"pioName")){
		    if(lim->pioName)cFree((char *)lim->pioName);
		    lim->pioName=(unsigned char *)strsave(e.ch,99771);
		    if(lim->pioName){
		        int k;
		        
		        k=strlen((char *)lim->pioName);
		        while(--k > 0){
		            if(lim->pioName[k] == ' '){
		                lim->pioName[k]=0;
		            }else{
		                break;
		            }
		        }
		    }
		}else{
		    sprintf(WarningBuff,"FilePutLimitsString %s Unknown Command\n",buff);
		    WarningBatch(WarningBuff);
		}
	}

	return 0;
}
int FileGetLimits(struct LIMITS **limits,long CurrentFrame,struct FileInfo2 *Files)
{
	struct LIMITS *limitl;
	
	if(!limits)return 1;
	
	*limits=NULL;
	
	if(!Files)return 1;
	
	Files->pioIndex=0;
	Files->pioName[0]=0;
	Files->pioTime=0;
	
	if(Files->ImageCount < 1)return 1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	if((Files->fileClass == HDF_FILES) && Files->HDFList[CurrentFrame].limits.command){
	   *limits=&Files->HDFList[CurrentFrame].limits;
	}else if((Files->fileClass == SDS2D_FILES) && Files->SDS2DList[CurrentFrame].limits.command){
	    *limits=&Files->SDS2DList[CurrentFrame].limits;
	}else if((Files->fileClass == SDS3D_FILES) && Files->SDS3DList[CurrentFrame].limits.command){
	    *limits=&Files->SDS3DList[CurrentFrame].limits;
	}else if((Files->fileClass == PIO1D_FILES) && Files->limits.iGot){
	    *limits=&Files->limits;
	}else if((Files->fileClass == PIO2D_FILES) && Files->limits.iGot){
	    *limits=&Files->limits;
	}else if((Files->fileClass == PIO3D_FILES) && Files->limits.iGot){
	    *limits=&Files->limits;
	}else if((Files->fileClass == PIO1D_BLK_FILES) && Files->limits.iGot){
	    *limits=&Files->limits;
	}else if((Files->fileClass == PIO2D_BLK_FILES) && Files->limits.iGot){
	    *limits=&Files->limits;
	}else if((Files->fileClass == PIO3D_BLK_FILES) && Files->limits.iGot){
	    *limits=&Files->limits;
	}
	
	if(!(*limits))return 1;
			
	limitl = *limits;
	
	limitl->ImageCount=Files->ImageCount;

	if(Files->Reflect_x){
	   if(!limitl->Reflect_x){
	       limitl->xmin = -limitl->xmax;
	       limitl->Reflect_x=TRUE;
	   }
	}else{
	   if(limitl->Reflect_x){
	       limitl->xmin = 0;
	       limitl->Reflect_x=FALSE;
	   }
	}
	
	if(limitl->xmin >= limitl->xmax){
	   limitl->xmax=limitl->xmin+fabs(limitl->xmin)+1.0;
	}
		
	if(limitl->ymin >= limitl->ymax){
	   limitl->ymax=limitl->ymin+fabs(limitl->ymin)+1.0;
	}
		
	FilePutCommand(limitl);
		
	Files->pioIndex=limitl->pioIndex;
    if(limitl->pioName){
        mstrncpy(Files->pioName,(char *)limitl->pioName,255);
    }
	Files->pioTime=limitl->time;
	
	return 0;
}              
int FilePutCommand(struct LIMITS *limits)
{
    char buff[2048];
    
 	if(!limits)return 1;


    FilePutCommandBuff(buff,
        limits->xmin,limits->xmax,
        limits->ymin,limits->ymax,
        limits->zmin,limits->zmax,
        limits->vmin,limits->vmax,
        limits->dolog,
        (int)limits->pioIndex,
        limits->pioName,
        limits->time);

	if(limits->command)cFree((char *)limits->command);
	limits->command=(unsigned char *)strsave(buff,5281);
	if(limits->command){
	    limits->iGot=TRUE;
	}else{
	    limits->iGot=FALSE;
	}
 	 	
 	return 0;
}
int FilePutCommandBuff(char *buff,double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,
                       double vmin,double vmax,int dolog,int pioIndex,unsigned char *pioName,double time)
{
	if(!buff)return 1;
	
	if(time != 0){		
    	sprintf(buff,
        	"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g vmin %g vmax %g dolog %d pioIndex %d pioName \"%s\" time %g %c",
        	xmin,xmax,
        	ymin,ymax,
        	zmin,zmax,
        	vmin,vmax,
       	 	dolog,
        	pioIndex,
        	(pioName && pioName[0]) ? pioName : (unsigned char *)"v",
        	time,
        	0);        
   }else{
    	sprintf(buff,
        	"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g vmin %g vmax %g dolog %d pioIndex %d pioName \"%s\" %c",
        	xmin,xmax,
        	ymin,ymax,
        	zmin,zmax,
        	vmin,vmax,
       	 	dolog,
        	pioIndex,
        	(pioName && pioName[0]) ? pioName : (unsigned char *)"v",
        	0);        
   }
        
	return 0;
}
int FilePutLimits(struct LIMITS *limits,long CurrentFrame,struct FileInfo2 *Files)
{
    struct LIMITS *lim;
    struct HDF *h;
    struct SDS2D *s2;
    struct SDS3D *s3;
        
	if(!limits || !Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	
	lim = NULL;
	if(Files->fileClass == HDF_FILES){
	    if(!Files->HDFList)return 1;
	    h = &Files->HDFList[CurrentFrame];
	    if(!h)return 1;
	    lim = &h->limits;
	}else if(Files->fileClass == SDS2D_FILES){
	    if(!Files->SDS2DList)return 1;
	    s2 = &Files->SDS2DList[CurrentFrame];
	    if(!s2)return 1;
	    lim = &s2->limits;
	}else if(Files->fileClass == SDS3D_FILES){
	    if(!Files->SDS3DList)return 1;
	    s3 = &Files->SDS3DList[CurrentFrame];
	    if(!s3)return 1;
	    lim = &s3->limits;
	}
	
	
	if(!lim)return 1;
	
	if(lim->command)cFree((char *)lim->command);
	if(lim->pioName)cFree((char *)lim->pioName);
	
	*lim = *limits;
	
	FilePutCommand(lim);
		
	return 0;
}
int FixPathName(char *path)
{
    char path2[2048],*fp;
    int n;
	
    if(!path)return 1;
    
    if(path != (char *)0x01212)return 1;
        
    fp=strchr((char *)path,':');
    if(fp){
        fp += 1;
    }else{
        fp=path;
    }
    
    n=0;
    path2[n++] = '/';
    while(*fp && (n < 2047) ){
        if(*fp == ':'){
            path2[n++] = '/';
        }else{
            path2[n++] = *fp;
        }
        fp++;
    }
    
     path2[n]=0;

     mstrncpy(path,path2,2048);
	     
    return 0;
}
int IsFile(char *name)
{
	FILE *in;

	if(!name)return 0;

	in=fopen(name,"rb");
	if(in == NULL){
	   return 0;
	}

	if(in)fclose(in);

	return 1;
}


int IsSDS(char *name,int *rank)
{
	int maxrank;
	int sizes[5];

	if(!name)return 0;

	maxrank=5;

	DFSDrestart();

	if(DFSDgetdims(name,rank,sizes,maxrank)){
	    return 0;
	}
	if(*rank == 3){
	    return 1;
	}else if(*rank == 2){
	    return 1;
	}
/*
	sprintf((WarningBuff,"SDS %s has rank %ld - Not Processed\n",files->name[0],(long)*rank);
	WarningBatch(WarningBuff);
*/
	return 0;
	
}
int IsTRUE(char *namef,int *type)
{
	char name[256];
	FILE *in;
	int n;

	if(!namef || !type)return 0;

	in=fopen(namef,"r");
	if(in == NULL){
/*
	   sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",files->name[0]);
	   WarningBatch(WarningBuff);
*/
	   return 0;
	}
	for(n=0;n<4;++n){
	    name[n]=fgetc(in);
	}
	name[4]=0;

	fclose(in);

	if(!strcmp(name,"DRt1")){
	   *type = 1;
	   return 1;
	}else if(!strcmp(name,"DRt2")){
	   *type = 2;
	   return 1;
	}
	return 0;
}
int IsMOOV(char *namef)
{
	char name[256];
	FILE *in;
	int n;

	if(!namef)return 0;

	in=fopen(namef,"r");
	if(in == NULL){
	   return 0;
	}
	for(n=0;n<8;++n){
	    name[n]=fgetc(in);
	}
	name[8]=0;

	fclose(in);

	if(!strcmp(&name[4],"moov")){
	   return 1;
	}
	return 0;
}

int IsRGB(char *namef)
{
	unsigned char *bp,name[24];
	FILE *in;
/* image header start */
    int	imagic;
    int itype;
    int idim;
    int xsize;
    int ysize;
    int izsize;
/* image header end */
	int n;

	if(!namef)return 0;

	in=fopen(namef,"r");
	if(in == NULL){
/*
	   sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",files->name[0]);
	   WarningBatch(WarningBuff);
*/
	   return 0;
	}
	for(n=0;n<12;++n){
	    name[n]=fgetc(in);
	}
	name[12]=0;

	fclose(in);

	bp=(unsigned char *)name;

	imagic = (((unsigned short)bp[0]) << 8) + bp[1];
	itype =  (((unsigned short)bp[2]) << 8) + bp[3];
	idim =   (((unsigned short)bp[4]) << 8) + bp[5];
	xsize = (((unsigned short)bp[6]) << 8) + bp[7];
	ysize = (((unsigned short)bp[8]) << 8) + bp[9];
	izsize = (((unsigned short)bp[10]) << 8) + bp[11];
    
	if(imagic != 0732){
	    return 0;
	}
	if(itype != 257 && itype != 1){
/*
	    sprintf(WarningBuff,"(%s) sgi file bad type is (%d)\n",name,itype);
	    WarningBatch(WarningBuff);
*/
	    return 0;
	}
	if(idim != 3){
/*
	    sprintf(WarningBuff,"(%s) sgi file bad idim is (%d)\n",name,idim);
            WarningBatch(WarningBuff);
 */
	    return 0;
	}
	if(izsize != 3){
/*
	    sprintf(WarningBuff,"(%s) sgi file bad izsize is (%d)\n",name,izsize);
            WarningBatch(WarningBuff);
*/
	    if(izsize != 1 || itype != 257)return 0;
	}
	if((xsize <= 0) || (ysize <= 0)){
/*
	    sprintf(WarningBuff,"(%s) sgi file bad xsize is (%d) ysize id (%d)\n",name,xsize,ysize);
            WarningBatch(WarningBuff);
*/
	    return 0;
	}
	return 1;
}
int IsBMP(char *fname)
{
	char name[3];
	FILE *in;
	int n;

	if(!fname)return 0;

	in=fopen(fname,"r");
	if(in == NULL){
	   sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",fname);
	   WarningBatch(WarningBuff);
	   return 0;
	}
	for(n=0;n<2;++n){
	    name[n]=fgetc(in);
	}
	name[2]=0;

	fclose(in);

	if(!strcmp("BM",name))return 1;

	return 0;
}
int IsGIF(char *fname)
{
	char name[24];
	FILE *in;
	int n;

	if(!fname)return 0;

	in=fopen(fname,"r");
	if(in == NULL){
	   sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",fname);
	   WarningBatch(WarningBuff);
	   return 0;
	}
	for(n=0;n<6;++n){
	    name[n]=fgetc(in);
	}
	name[6]=0;

	fclose(in);

	if(!strcmp("GIF87a",name))return 1;
	if(!strcmp("GIF89a",name))return 1;

	return 0;
}
int IsHdf(char *files)
{
 	int  ixmax,iymax;
	int hasPalette;

	if(!files)return 0;
	DFR8restart();

	if(DFR8getdims(files,&ixmax,&iymax,&hasPalette)){
            return 0;
	}
	 return 1;
}
int IsSHOW(char *namei)
{
	char buff[256],name[256];
	FILE *in;
	int n;

	if(!namei)return 0;

	in=fopen(namei,"r");
	if(in == NULL){
	   sprintf(buff,"FILE %s Could Not Be Opened to Read\n",namei);
	   WarningBatch(buff);
	   return 0;
	}
	for(n=0;n<4;++n){
	    name[n]=fgetc(in);
	}
	name[4]=0;

	fclose(in);

	if(!strcmp(name,"SHOW")){
	   return 1;
	}
	return 0;
}
int IsCONF(char *namei)
{
	char buff[256],name[256];
	FILE *in;
	int n;

	if(!namei)return 0;

	in=fopen(namei,"r");
	if(in == NULL){
	   sprintf(buff,"FILE %s Could Not Be Opened to Read\n",namei);
	   WarningBatch(buff);
	   return 0;
	}
	for(n=0;n<8;++n){
	    name[n]=fgetc(in);
	}
	name[8]=0;

	fclose(in);

	if(!strcmp(name,"DATAUNIV")){
	   return 1;
	}
	return 0;
}
int IsSTL(char *name)
{
	char buff[256];
	static char *stlhead="solid";
	FILE *in;
	int n,c,ret;

	if(!name)return 0;

	ret=0;

	in=fopen(name,"r");
	if(in == NULL){
	   sprintf(buff,"FILE %s Could Not Be Opened to Read\n",name);
	   WarningBatch(buff);
	   return 0;
	}
	for(n=0;n<5;++n){
	    c=fgetc(in);
	    if(c == EOF)goto Error;
	    buff[n]=c;
	}
	buff[5]=0;
	if(!strcmp(buff,stlhead)){
	    ret = 1;
	}
Error:
	fclose(in);
	return ret;
}
int IsVRML(char *name)
{
	char buff[256];
	static char *vrmlhead="#VRML V1.0";
	FILE *in;
	int n,c,ret;

	if(!name)return 0;

	ret=0;

	in=fopen(name,"r");
	if(in == NULL){
	   sprintf(buff,"FILE %s Could Not Be Opened to Read\n",name);
	   WarningBatch(buff);
	   return 0;
	}
	for(n=0;n<10;++n){
	    c=fgetc(in);
	    if(c == EOF)goto Error;
	    buff[n]=c;
	}
	buff[10]=0;
	if(!strcmp(buff,vrmlhead)){
	    ret = 1;
	}
Error:
	fclose(in);
	return ret;
}
int IsTEK(char *name)
{
	char buff[256];
	FILE *in;
	int n,c,ret;

	if(!name)return 0;

	ret=0;

	in=fopen(name,"r");
	if(in == NULL){
	   sprintf(buff,"FILE %s Could Not Be Opened to Read\n",name);
	   WarningBatch(buff);
	   return 0;
	}
	ret = 1;
	for(n=0;n<512;++n){
	    c=fgetc(in);
	    if(c == EOF)break;
	    if(isprint(c))continue;
	    if(c == 0x1b || c == 0x1d || c == 0x1f || c == 0x0c ||
	       c >= 32)continue;
		    ret = 0;
	        break;
	}
	fclose(in);
	return ret;
}
int IsTEXT(char *name)
{
	char buff[256];
	FILE *in;
	int n,c,ret;

	if(!name)return 0;

	ret=0;

	in=fopen(name,"r");
	if(in == NULL){
	   sprintf(buff,"FILE %s Could Not Be Opened to Read\n",name);
	   WarningBatch(buff);
	   return 0;
	}
	ret = 1;
	for(n=0;n<512;++n){
	    c=fgetc(in);
	    if(c == EOF)break;
	    if(isprint(c))continue;
	    if(c == '\t' || c == '\r' || c == '\n' ||
	       c >= 32)continue;
		    ret = 0;
	        break;
	}
	fclose(in);
	return ret;
}
int IsJPG(char *fname)
{
	char name[17];
	FILE *in;
	int n,c,ret;

	ret=0;

	if(!fname)return 0;

	in=fopen(fname,"rb");
	if(in == NULL){
	   sprintf(WarningBuff,"FILE %s Could Not Be Opened to Read\n",fname);
	   WarningBatch(WarningBuff);
	   return 0;
	}
	for(n=0;n<16;++n){
		c=fgetc(in);
		if(c == EOF)goto ErrorOut;
	    name[n]=c;
	}

	name[10]=0;


	if(!strcmp("JFIF",&name[6]))ret=1;

	if(!strcmp("Exif",&name[6]))ret=1;

ErrorOut:


	if(in)fclose(in);
	return ret;
}

static int initSTL(struct FileInfo2 *FilesL)
{
    long n,k,count,length,ImageCount,lengthM;
	static char *vrmlhead="solid";
    char **FilesNames;
    char buff[256];
	char filepath[2048];
	FILE *in;
	int itis;

	if(!FilesL)return 1;

	in=NULL;

	FilesL->fileClass=STL_FILES;
	FilesL->pdefault=FilesOpenVRML;

	FilesNames=FilesL->FilesNames;

	FilesL->loadToMemory=0;
	
	FilesL->fileType=STL_DISK;
	    
	count=0;
	ImageCount=0;
	lengthM=0;
	for(n=0;n<FilesL->FileCount;++n){
	    unsigned char *buffer;
	    int ok;


	    ok=FALSE;
	    buffer=NULL;

	    mstrncpy(filepath,FilesL->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);

	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }

	    itis=FALSE;
	    for(k=0;k<5;++k){
	        int c;
	        if((c=getc(in)) == EOF)break;
	        buff[k]=c;
	    }
	    buff[5]=0;
	    if(!strcmp(buff,vrmlhead)){
	        itis = TRUE;
	    }
	    if(!itis){
	        sprintf(WarningBuff,"File %s Skipped Not A STL File \n",
	                FilesNames[n]);
	       	WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    if(fseek(in,0L,2)){
	        sprintf(WarningBuff,"(%s) Error Looking For End Of File\n",FilesNames[n]);
	       	WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    if((length=ftell(in)) == -1L){
	        sprintf(WarningBuff,"(%s) Error Reading For End Of File Location\n",FilesNames[n]);
	       	WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    if(length > lengthM)lengthM=length;

	    if(CheckSTLSpace(FilesL))goto Loop;

		buffer=NULL;
		if(FilesL->loadToMemory){
		    if(fseek(in,0L,0)){
		        sprintf(WarningBuff,"(%s) Error Rewinding File\n",FilesNames[n]);
	       		WarningBatch(WarningBuff);
		        goto Loop;
		    }
		    buffer=(unsigned char *)cMalloc(length,8418);
		    if(!buffer)goto Loop;
		    if(getString((unsigned char *)buffer,length,in))goto Loop;
		}

		FilesL->VRMLList[ImageCount].buffer=buffer;
		FilesL->VRMLList[ImageCount].length=length;
		FilesL->ImageFiles[ImageCount]=n;
		buffer=NULL;
		FilesL->ImageCount = ++ImageCount;

	    ok=1;
Loop:

	    if(in)fclose(in); 
	    in=NULL;

	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
	        FilesL->FilesNames[count++]=FilesNames[n];
	    }else{
	        sprintf(WarningBuff,"Skipping file %s\n",FilesNames[n]);
	        WarningBatch(WarningBuff);
	        cFree((char *)FilesNames[n]);
	    }
	}
	FilesL->FileCount=count;
	FilesL->FileOpen = -1;

	FilesL->length=lengthM;

	if(count <= 0)goto ErrorOut;

	return 0;
ErrorOut:
	if(in)fclose(in);
	if(FilesL->ImageCount > 0)return 0;
	freeFileList(FilesL);
	return 1;
}
static int CheckSTLSpace(struct FileInfo2 *Files)
{
	struct VRML *VRMLList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	VRMLList=NULL;
	if(Files->VRMLList){
	    VRMLList=(struct VRML *)cRealloc((char *)Files->VRMLList,ImageMax*sizeof(struct VRML),8752);
	    if(!VRMLList){
	        WarningBatch("CheckVRMLSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&VRMLList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct VRML));
	}else{
	    VRMLList=(struct VRML *)cMalloc(ImageMax*sizeof(struct VRML),8419);
	    if(!VRMLList){
	        WarningBatch("CheckVRMLSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)VRMLList,ImageMax*sizeof(struct VRML));
	}

	Files->ImageMax=ImageMax;
	Files->VRMLList=VRMLList;

	return 0;
ErrorOut:
	if(VRMLList){
	    Files->VRMLList=VRMLList;
	}
	return 1;
}

int initVRML(struct FileInfo2 *FilesL)
{
    long n,k,count,length,ImageCount,lengthM;
	static char *vrmlhead="#VRML V1.0";
    char **FilesNames;
    char buff[256];
	char filepath[2048];
	FILE *in;
	int itis;

	if(!FilesL)return 1;

	in=NULL;

	FilesL->fileClass=VRML_FILES;
	FilesL->pdefault=FilesOpenVRML;
	/*
	FilesL->pdefault=FilesOpenTek;
	FilesL->pSaveOne=FilesSaveTRUE;
	FilesL->pSaveOneMerged=FilesMergedTRUE;
	*/

	FilesNames=FilesL->FilesNames;

	if(FilesL->loadToMemory){
	    FilesL->fileType=VRML_MEMORY;
	}else{
	    FilesL->fileType=VRML_DISK;
	}

	count=0;
	ImageCount=0;
	lengthM=0;
	for(n=0;n<FilesL->FileCount;++n){
	    unsigned char *buffer;
	    int ok;


	    ok=FALSE;
	    buffer=NULL;

	    mstrncpy(filepath,FilesL->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);

	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }

	    itis=FALSE;
	    for(k=0;k<10;++k){
	        int c;
	        if((c=getc(in)) == EOF)break;
	        buff[k]=c;
	    }
	    buff[10]=0;

	    if(!strcmp(buff,vrmlhead)){
	        itis = TRUE;
	    }
	    if(!itis){
	        sprintf(WarningBuff,"File %s Skipped Not A VRML File \n",
	                FilesNames[n]);
	       	WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    if(fseek(in,0L,2)){
	        sprintf(WarningBuff,"(%s) Error Looking For End Of File\n",FilesNames[n]);
	       	WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    if((length=ftell(in)) == -1L){
	        sprintf(WarningBuff,"(%s) Error Reading For End Of File Location\n",FilesNames[n]);
	       	WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    if(length > lengthM)lengthM=length;

	    if(CheckVRMLSpace(FilesL))goto Loop;

		buffer=NULL;
		if(FilesL->loadToMemory){
		    if(fseek(in,0L,0)){
		        sprintf(WarningBuff,"(%s) Error Rewinding File\n",FilesNames[n]);
	       		WarningBatch(WarningBuff);
		        goto Loop;
		    }
		    buffer=(unsigned char *)cMalloc(length,8418);
		    if(!buffer)goto Loop;
		    if(getString((unsigned char *)buffer,length,in))goto Loop;
		}

		FilesL->VRMLList[ImageCount].buffer=buffer;
		FilesL->VRMLList[ImageCount].length=length;
		FilesL->ImageFiles[ImageCount]=n;
		buffer=NULL;
		FilesL->ImageCount = ++ImageCount;

	    ok=1;
Loop:

	    if(in)fclose(in); 
	    in=NULL;

	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
	        FilesL->FilesNames[count++]=FilesNames[n];
	    }else{
	        sprintf(WarningBuff,"Skipping file %s\n",FilesNames[n]);
	        WarningBatch(WarningBuff);
	        cFree((char *)FilesNames[n]);
	    }
	}
	FilesL->FileCount=count;
	FilesL->FileOpen = -1;

	FilesL->length=lengthM;

	if(count <= 0)goto ErrorOut;

	return 0;
ErrorOut:
	if(in)fclose(in);
	if(FilesL->ImageCount > 0)return 0;
	freeFileList(FilesL);
	return 1;
}
int CheckVRMLSpace(struct FileInfo2 *Files)
{
	struct VRML *VRMLList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	VRMLList=NULL;
	if(Files->VRMLList){
	    VRMLList=(struct VRML *)cRealloc((char *)Files->VRMLList,ImageMax*sizeof(struct VRML),8752);
	    if(!VRMLList){
	        WarningBatch("CheckVRMLSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&VRMLList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct VRML));
	}else{
	    VRMLList=(struct VRML *)cMalloc(ImageMax*sizeof(struct VRML),8420);
	    if(!VRMLList){
	        WarningBatch("CheckVRMLSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)VRMLList,ImageMax*sizeof(struct VRML));
	}

	Files->ImageMax=ImageMax;
	Files->VRMLList=VRMLList;

	return 0;
ErrorOut:
	if(VRMLList){
	    Files->VRMLList=VRMLList;
	}
	return 1;
}
int initHDF(struct FileInfo2 *Files)
{
    long n,k,count,ImageCount,ImageFirst,dcount;
    char buff[256],path[2048];
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
	FILE *in;
    DFdd2 *dd;
    int HasPal;
	long imageskip;
	long ImageIncrement;


	if(!Files)return 1;

	dd=NULL;
	in=NULL;
	Files->fileClass=HDF_FILES;
	FilesNames=Files->FilesNames;
	Files->pSetFrame=FilesSetFrameHDF;
	Files->pGetData=FilesGetHDFdata;
	Files->pdefault=FilesOpenHDF;

	DFPrestart();
	DFPgetpal(FilesNames[0],(char *)Files->palette);
	
	if(Files->loadToMemory){
	    Files->fileType=HDF_MEMORY;
	}else{
	    Files->fileType=HDF_DISK;
	}


	xmin=ymin=200000;
	xmax=ymax=0;
	count=0;
	ImageCount=0;
	HasPal=FALSE;
	imageskip=0;
	ImageIncrement=Files->ImageIncrement;
	if(ImageIncrement <= 0)ImageIncrement = 1;
	dcount=0;
	dd=NULL;
	for(n=0;n<Files->FileCount;++n){
	    unsigned char *buffer;
	    long offset,length;
	    int xsize,ysize;
	    int ok;


		dd=NULL;
	    ok=FALSE;
		xsize=0;
		ysize=0;
		dcount=0;
	    buffer=NULL;
	    length = -1;
	    offset = -1;
	    ImageFirst=ImageCount;
		
		mstrncpy(path,Files->directory,2048);
		strncatToPath(path,FilesNames[n],2048);
		FixPathName(path);
	    in=fopen(path,"rb");
	    if(in == NULL){
	       	sprintf(WarningBuff,"Could Not Open %s To Read\n",path);
	       	WarningBatch(WarningBuff);
	       	goto Loop;
	    }


	    if(getString((unsigned char *)buff,4L,in))goto Loop;

	    for(k=0;k<4;++k)if(buff[k] != NCSAtag[k]){
	       	sprintf(WarningBuff,"%s is not a HDF file\n",FilesNames[n]);
	       	WarningBatch(WarningBuff);
	       	goto Loop;
	    }
	    
	    if(getDD(&dd,&dcount,in))goto ErrorOut;


	    for(k=0;k<dcount;++k){
	        char *name,*command;
	               
	            switch(dd[k].tag){
	            case DFTAG_CI8:
	                offset=dd[k].offset;
					length=dd[k].length;
	                break;
	            case DFTAG_ID8: /* Check Dimensions */
	             
		        	fseek(in,dd[k].offset,0); 

	                if(getInt(&xsize,in))goto ErrorOut;
	                if(getInt(&ysize,in))goto ErrorOut;
	                
	                break;
	            case DFTAG_NT:
	                break;
	        	case DFTAG_IP8:
		            if(!HasPal && dd[k].length == 768){
		            	if(fseek(in,dd[k].offset,SEEK_SET)){
		       				sprintf(WarningBuff,"%s Error fseek %ld\n",FilesNames[n],dd[k].offset);
		       				WarningBatch(WarningBuff);
		       				goto ErrorOut;
		            	}
			            if(getString((unsigned char *)Files->palette,dd[k].length,in))goto ErrorOut;
		            }
	            	HasPal=TRUE;
	            	break;
	            case DFTAG_MT:
	                break;
	            case DFTAG_RIG:
	    		if(++imageskip < ImageIncrement)continue;
	    		imageskip=0;
		        if(CheckHdfSpace(Files))goto ErrorOut;
					{
					    long kref,cref,kk;
					    name = NULL;
					    command=NULL;
					    kref = -1;
					    cref = -1;
					    for(kk=0;kk<dcount;++kk){
							if(dd[kk].tag == DFTAG_DIL){
							    if(dd[kk].labelref == dd[k].ref)kref = kk;
							}
							if(dd[kk].tag == DFTAG_DIA){
							    if(dd[kk].labelref == dd[k].ref)cref = kk;
							}
					    }
					    if(kref >= 0){
					        if(dd[kref].labelname){
					            name=dd[kref].labelname;
						    	dd[kref].labelname=NULL;
							}
					    }
					    if(cref >= 0){
					        if(dd[cref].labelname){
					            command=dd[cref].labelname;
						    	dd[cref].labelname=NULL;
							}
					    }
					}
					if(length == -1 || offset == -1){
					    sprintf(WarningBuff,"Error in File length %ld offset %ld\n",
					            (long)length,(long)offset);
			       		WarningBatch(WarningBuff);
					    goto ErrorOut;
					}
					buffer=NULL;
					if(Files->loadToMemory){
					    buffer=(unsigned char *)cMalloc(length,8107);
					    if(!buffer)goto ErrorOut;
					    fseek(in,offset,0); 
					    if(getString((unsigned char *)buffer,length,in))goto ErrorOut;
					}
					Files->HDFList[ImageCount].Offset=offset;
					Files->HDFList[ImageCount].buffer=buffer;
					Files->HDFList[ImageCount].HdfSize=length;
					Files->HDFList[ImageCount].length=xsize*ysize;
					Files->HDFList[ImageCount].xsize=xsize;
					Files->HDFList[ImageCount].ysize=ysize;
					Files->ImageFiles[ImageCount]=n;
					if(name == NULL){
					    char buff[256];
					    if(ImageFirst == ImageCount){
					        sprintf(buff,"%s%c",FilesNames[n],0);
					    }else{
					        sprintf(buff,"%s.%ld%c",FilesNames[n],ImageCount,0);
					    }
					    Files->ImageNames[ImageCount]=strsave(buff,8229);
					    if(!Files->ImageNames[ImageCount]){
			       			WarningBatch("initHDF Out Of Memory\n");
					    	goto ErrorOut;
					    }
					}else{
					    Files->ImageNames[ImageCount]=name;
					}     
				     
					buffer=NULL;
					Files->ImageCount = ++ImageCount;
					
					if(command){
					    FilePutLimitsString(command,ImageCount-1,Files);
					}
					
					if(xsize > xmax)xmax=xsize;
					if(xsize < xmin)xmin=xsize;
					if(ysize > ymax)ymax=ysize;
					if(ysize < ymin)ymin=ysize;
					
	                break;

	           }
	            
	    }
	  
	    ok=1;
Loop:

	    if(in)fclose(in); 
	    in=NULL;

	    if(dd){
	        freeDD(dd,dcount);
	    	dd=NULL;
	    }
	    
	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
		    if(count < n){
		        if(FilesNames[count])cFree((char *)FilesNames[count]);
	    		Files->FilesNames[count++]=FilesNames[n];
	    		FilesNames[n]=NULL;
		    }else{
	    		Files->FilesNames[count++]=FilesNames[n];
		    }
	    }else{
	       sprintf(WarningBuff,"Skipping file %s\n",FilesNames[n]);
	       WarningBatch(WarningBuff);
	       cFree((char *)FilesNames[n]);
	       FilesNames[n]=NULL;
	    }
	}
	Files->FileCount=count;
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	if(Files->xminr > Files->xmaxr){
		Files->xminr=0;
		Files->yminr=0;
		Files->xmaxr=(double)xmax;
		Files->ymaxr=(double)ymax;
	}
	
	
	Files->length=(long)(ymax*xmax*sizeof(double));
	Files->FileOpen = -1;
	Files->dataSave=(unsigned char *)cMalloc(Files->length,8108);
	if(Files->dataSave == NULL){
	    goto ErrorOut;
	}
	Files->CurrentFrame = -1;
	if(count <= 0)goto ErrorOut;
	if(ImageCount <= 0){
	    sprintf(WarningBuff,"No Raster Images Found\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
    doSDS2DStart(Files);
    
	return 0;
ErrorOut:
	if(dd){
		freeDD(dd,dcount);
	}
	if(in)fclose(in);
	freeFileList(Files);
	return 1;
}
int FilesSetFrameHDF(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;

	if(!Files)return 1;
	s = &Files->SetData;
	s->SetFrame= -1;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	if(FileHdfInfo(&s->xsize,&s->ysize,&s->length,CurrentFrame,Files))return 1;

	s->length=s->xsize*s->ysize;

	if(FilesGetHDF(&s->name,(char *)Files->dataSave,s->length,
                       CurrentFrame,Files))return 1;

	s->SetFrame= CurrentFrame;
	s->type= HDF_FILES;
	s->data= Files->dataSave;
	return 0;
}
int FileHdfInfo(long *xsize,long *ysize,long *length,
                long CurrentFrame,struct FileInfo2 *Files)
{
	if(!xsize || !ysize || !length || !Files || !Files->HDFList )return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != HDF_FILES)return 1;
	*xsize=Files->HDFList[CurrentFrame].xsize;
	*ysize=Files->HDFList[CurrentFrame].ysize;
	*length=Files->HDFList[CurrentFrame].HdfSize;
	return 0;
}
int FilesGetHDF(char **ImageName,char *buffer,long length,
                long CurrentFrame,struct FileInfo2 *Files)
{
	long xsize,ysize,xylength;

	if(!ImageName || !buffer || !Files || !Files->HDFList)return 1;

	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != HDF_FILES)return 1;

	xsize=Files->HDFList[CurrentFrame].xsize;
	ysize=Files->HDFList[CurrentFrame].ysize;
	xylength=xsize*ysize;

	if(xylength > length)return 1;

/*
	if(Files->CurrentFrame == CurrentFrame){
	    uMemcpy(buffer,Files->dataSave,xylength);
	}else 
*/
	
	if(Files->loadToMemory){
	    UnPack((unsigned char *)buffer,(unsigned char *)Files->HDFList[CurrentFrame].buffer,
	           Files->HDFList[CurrentFrame].length);
	    Files->CurrentFrame = CurrentFrame;
	}else{
	   if(SetFileToRead(CurrentFrame,Files))return 1;
	   fseek(Files->in,Files->HDFList[CurrentFrame].Offset,0); 
	   if(SetBuffers(Files->HDFList[CurrentFrame].HdfSize)){
	       return 1;  
	   }
	   if(getString((unsigned char *)xg.buffin,Files->HDFList[CurrentFrame].HdfSize,Files->in))return 1;
	   UnPack((unsigned char *)buffer,(unsigned char *)xg.buffin,
	                  Files->HDFList[CurrentFrame].length);
	    Files->CurrentFrame = CurrentFrame;
	}

	*ImageName=FilesImageName(Files,CurrentFrame);

	return 0;
	
}
int FilesGetHDFdata(double **datai,long *lengthi,
                 long CurrentFrame,struct FileInfo2 *Files)
{
	double *data,r;
	char *name;
	unsigned char *bb;
	/*
	struct paletteDraw pd;
	*/
	int blackZero,white255;
	long nn,length;
	int n;

	if(!datai || !lengthi || !Files)return 1;

	if(FilesGetHDF(&name,(char *)Files->dataSave,Files->length,
                      CurrentFrame,Files))return 1;

	Files->CurrentFrame = -1;

	length=Files->HDFList[CurrentFrame].length;
	*lengthi=length;
	data=(double *)Files->dataSave;
	*datai=data;
	
/*
	pd=Files->d.pd;
	blackZero=pd.blackZero;
	white255=pd.white255;
*/
	blackZero=0;
	white255=0;

	bb=(unsigned char *)Files->dataSave;
	for(nn=length-1;nn>=0;--nn){
	    n=bb[nn];
	    if(blackZero && (n == 255)){
	        n = 0;
      	}else if(white255 && (n == 0)){
	        n = 255;
      	}
      	r=(double)n;
/*
	    if(pd.sType == 0){
	        r=pd.sReal1+
	          (n-pd.sPixel1)*
	              (pd.sReal2-pd.sReal1)/
	                  (pd.sPixel2-pd.sPixel1);
	    }else if(n <= 1){
	        r=pd.sPmin;
	    }else if(n >= 254){
	        r=pd.sPmax;
	    }else if(pd.sType == 1){	    
	        r=pd.sPmin*pow(pd.sPmax/pd.sPmin,
	                                      (double)(n-2)/252.);
	    }else{
	        r=pd.sPmin+(pd.sPmax-pd.sPmin)*(n-2)/252.;
	    }
*/
	    data[nn]=r;
	}
	return 0;
}

int CheckHdfSpace(struct FileInfo2 *Files)
{
	struct HDF *HDFList;
    long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	HDFList=NULL;
	if(Files->HDFList){
	    HDFList=(struct HDF *)cRealloc((char *)Files->HDFList,ImageMax*sizeof(struct HDF),8259);
	    if(!HDFList){
	        WarningBatch("initHDF out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&HDFList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct HDF));
	}else{
	    HDFList=(struct HDF *)cMalloc(ImageMax*sizeof(struct HDF),8109);
	    if(!HDFList){
	        WarningBatch("initHDF out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)HDFList,ImageMax*sizeof(struct HDF));
	}

	Files->ImageMax=ImageMax;
	Files->HDFList=HDFList;

	return 0;
ErrorOut:
	if(HDFList){
	    Files->HDFList=HDFList;
	}
	return 1;
}
int FileAppendHdf(char *name,int xsize,int ysize,unsigned char *image,
                  long Length,struct FileInfo2 **iFiles)
{
	struct FileInfo2 *Files;
	unsigned char *buffer;
	long ImageCount,xmax,ymax;
	int ret;


	if(!name || !iFiles)return 1;

	ret=1;
	
	Files= *iFiles;
	
	if(Files){
	    if(Files->fileClass != HDF_FILES){
	       freeFileList(Files);
	       *iFiles=Files=NULL;
	    }
	}
	
	if(!Files){
		Files=GetFileInfoStruct(8110);
	    if(!Files){
	        WarningBatch("FileAppendHdf out of Memory\n");
	        goto ErrorOut;
	    }
	    
	    Files->loadToMemory=1;
	    Files->fileClass = HDF_FILES;
	    Files->fileType=HDF_MEMORY;
	    Files->CurrentFrame = -1;
	    	    
	    Files->pdefault=FilesOpenHDF;
	    Files->pGetData=FilesGetHDFdata;
	    Files->pSetFrame=FilesSetFrameHDF;	    
	}
	if(CheckHdfSpace(Files))goto ErrorOut;
	buffer=(unsigned char *)cMalloc(Length,8111);
	if(!buffer){
		WarningBatch("FileAppendHdf out of Memory\n");
	    goto ErrorOut;
	}
	uMemcpy(buffer,image,Length);
	ImageCount=Files->ImageCount;
	Files->HDFList[ImageCount].buffer=buffer;
	Files->HDFList[ImageCount].HdfSize=Length;
	Files->HDFList[ImageCount].length=xsize*ysize;
	Files->HDFList[ImageCount].xsize=xsize;
	Files->HDFList[ImageCount].ysize=ysize;
	Files->ImageNames[ImageCount]=strsave(name,8230);
	if(Files->ImageNames[ImageCount] == NULL){
		WarningBatch("FileAppendHdf out of Memory\n");
	    goto ErrorOut;
	}
	Files->ImageFiles[ImageCount]=0;
	Files->ImageCount = ++ImageCount;

	xmax=Files->xmax;
	ymax=Files->ymax;
	if(xsize < Files->xmin)Files->xmin=xsize;
	if(xsize > Files->xmax)Files->xmax=xsize;
	if(ysize < Files->ymin)Files->ymin=ysize;
	if(ysize > Files->ymax)Files->ymax=ysize;
	if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
	    if(Files->dataSave)cFree((char *)Files->dataSave);
	    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
	    Files->dataSave=(unsigned char *)cMalloc(Files->length,8112);
	    if(Files->dataSave == NULL){
	        WarningBatch("FileAppendHdf out of Memory\n");
	        goto ErrorOut;
	    }
	    Files->CurrentFrame = -1;
	}
	
	if(Files->ImageCount == 1)doSDS2DStart(Files);
	*iFiles=Files;
	ret = 0;
ErrorOut:
	return ret;
}
int flipSDS3DY(double *d,long xsize,long ysize,long zsize)
{
	long k;
	
	if(!d)return 1;
	if(!uPref.flipSDS2dY)return 0;
	
	for(k=0;k<zsize;++k){
	    flip3(d,(int)xsize,(int)ysize);
	    d += xsize*ysize;
	}
	
	
	return 0;
}

static int flip3(double *b,int xsize,int ysize)
{
	register double *p1,*p2,p;
	int x,y;

	if(!b)return 1;

	for(y=0;y<ysize/2;++y){
		p1=b+y*xsize;
		p2=b+(ysize-1-y)*xsize;
		for(x=0;x<xsize;++x){
			p = *p1;
			*p1++ = *p2;
			*p2++ = p;
		}
	}
	return 0;
}

int initSDS3D(struct FileInfo2 *Files)
{
	long xmin,xmax,ymin,ymax,zmin,zmax;
    char **FilesNames;
    char buff[256];
    long n,ImageCount,sdsCount,ImageFirst;
	char filepath[2048];
	int floatType;
	FILE *in;
	DFdd2 *dd;
	long count;

	if(!Files)return 1;
	
	dd=NULL;
	count=0;
	in=NULL;

	Files->fileClass=SDS3D_FILES;
	Files->pdefault=FilesOpenSDS3D;
	Files->pGetData=FilesGetSDS3Ddata;
	Files->pSetFrame=FilesSetFrameSDS3D;
	
	if(Files->loadToMemory){
	    Files->fileType=SDS3D_MEMORY;
	}else{
	    Files->fileType=SDS3D_DISK;
	}


	FilesNames=Files->FilesNames;

	xmin=ymin=zmin = 100000;
	xmax=ymax=zmax =     -1;

	ImageCount=0;
	sdsCount=0;
	floatType = -1;

	for(n=0;n<Files->FileCount;++n){
	    long k,idim,nscale;
	    int ok;
	    long xsize,ysize,zsize;


		idim = -1;
		nscale=-1;
		xsize=ysize=zsize=0;
		
	    dd=NULL;
	    ok=FALSE;
	    
	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    
	    
	    in=fopen(filepath,"rb");
	    if(in == NULL){
			sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
			WarningBatch(WarningBuff);
			goto Loop;
	    }

	    if(getString((unsigned char *)buff,4L,in))goto Loop;

	    for(k=0;k<4;++k)if(buff[k] != NCSA2[k])goto Loop;

	    if(getDD(&dd,&count,in))goto ErrorOut;

	    ImageFirst=ImageCount;

	    for(k=0;k<count;++k){

	        if(dd[k].tag == DFTAG_NT){
	            char nt[4];
		    	fseek(in,dd[k].offset,0L); 
	            if(getString((unsigned char *)nt,4L,in))continue;
	            if(floatType == -1){
	                floatType=nt[1];
	            }else{
	                if(floatType != nt[1]){
	                    sprintf(WarningBuff,"Error file %s floatType missmatch %d != %d\n",
	                    	FilesNames[n],floatType,nt[1]);
						WarningBatch(WarningBuff);
	                }
	            }
	            continue;
	        }else if(dd[k].tag != DFTAG_SDD)continue;
		    	fseek(in,dd[k].offset,0L); 
	            {
	                int rank,kk;
	                long dim;

	                if(getInt(&rank,in))goto ErrorOut;
	                if(rank != 3){
	                    sprintf(WarningBuff,"Skipping SDS with rank %d\n",rank);
						WarningBatch(WarningBuff);
	                    continue;
	                }
	                for(kk=0;kk<rank;++kk){
					    if(getLong(&dim,in))goto ErrorOut;
					    if(kk == 0){
							zsize=dim;
					    }else if(kk == 1){
							ysize=dim;
					    }else if(kk == 2){
							xsize=dim;
					    }
	                }
	            }
	           
	        idim = k;
	        break;
	    }
	    if(idim == -1){
	        sprintf(WarningBuff,"Error No Limits for SDS %s\n",FilesNames[n]);
			WarningBatch(WarningBuff);
		    goto Loop;
	    }
	    for(k=0;k<count;++k){
	        unsigned char *buffer;
	        char *name,*command;
	        long offset,length,idata,kk;

	        if(dd[k].tag == DFTAG_SDD){
		    	fseek(in,dd[k].offset,0L); 
	            {
	                int rank;
	                long dim;
	                if(getInt(&rank,in))goto ErrorOut;
	                if(rank != 3){
	                    sprintf(WarningBuff,"Skipping SDS with rank %d\n",rank);
						WarningBatch(WarningBuff);
	                    continue;
	                }
	                for(kk=0;kk<rank;++kk){
					    if(getLong(&dim,in))goto ErrorOut;
					    if(kk == 0){
							zsize=dim;
					    }else if(kk == 1){
							ysize=dim;
					    }else if(kk == 2){
							xsize=dim;
					    }
	                }
	            }	            
	            continue;	            
	        }else if(dd[k].tag == DFTAG_SDS){
				nscale=k;
				if(nscale >= 0 && ImageCount > 0){
			    	FilePutLimitsDD((unsigned char *)dd[nscale].labelname,dd[nscale].length,floatType,ImageCount-1,Files);
				}
				continue;
	        }else if(dd[k].tag != DFTAG_SD)continue;
			if(CheckSDS3DSpace(Files))goto ErrorOut;
			idata=k;
			{
			    long kref,cref;
			    name = NULL;
			    command = NULL;
			    kref = -1;
			    cref = -1;
			    for(kk=0;kk<count;++kk){
					if(dd[kk].tag == DFTAG_DIL){
					    if(dd[kk].labelref == dd[k].ref)kref = kk;
					}
					if(dd[kk].tag == DFTAG_DIA){
					    if(dd[kk].labelref == dd[k].ref)cref = kk;
					}
				}
				if(kref >= 0){
				    if(dd[kref].labelname){
				        name=dd[kref].labelname;
					    dd[kref].labelname=NULL;
					}
			    }
				if(cref >= 0){
				    if(dd[cref].labelname){
				        command=dd[cref].labelname;
					    dd[cref].labelname=NULL;
					}
			    }
			}
			offset=dd[idata].offset;
			length=dd[idata].length;
			buffer=NULL;

			if(Files->loadToMemory){
			    long lengthc;
			    
			    if(floatType == DFNT_FLOAT64){
			    	lengthc=(long)(length);
			    }else{
				    lengthc=(long)(2*length);
		    	}

			    buffer=(unsigned char *)cMalloc(lengthc,8431);
			    if(!buffer)goto ErrorOut;

			    if(getFloat(buffer,(unsigned char *)xg.buffin,offset,length,floatType,in))goto Loop;
			    flipSDS3DY((double *)buffer,xsize,ysize,zsize);

			}

			Files->SDS3DList[ImageCount].Offset=offset;
			Files->SDS3DList[ImageCount].buffer=buffer;
			Files->SDS3DList[ImageCount].HdfSize=length;

			Files->SDS3DList[ImageCount].length=xsize*ysize*zsize;
			Files->SDS3DList[ImageCount].xsize=(int)xsize;
			Files->SDS3DList[ImageCount].ysize=(int)ysize;
			Files->SDS3DList[ImageCount].zsize=(int)zsize;
			Files->ImageFiles[ImageCount]=n;
			if(name == NULL){
			    char buff[256];

			    if(ImageFirst == ImageCount){
			        sprintf(buff,"%s%c",FilesNames[n],0);
			    }else{
			        sprintf(buff,"%s.%ld%c",FilesNames[n],ImageCount,0);
			    }
			    Files->ImageNames[ImageCount]=strsave(buff,8812);
			    if(!Files->ImageNames[ImageCount]){
	       			WarningBatch("initSDS3D Out Of Memory\n");
			    	goto ErrorOut;
			    }
			}else{
			    Files->ImageNames[ImageCount]=name;
			}
			buffer=NULL;
			Files->ImageCount = ++ImageCount;
			
			if(command){
			    FilePutLimitsString(command,ImageCount-1,Files);
			}
	        sdsCount++;

	       if(xsize < xmin)xmin=xsize;
	       if(ysize < ymin)ymin=ysize;
	       if(zsize < zmin)zmin=zsize;
	       if(xsize > xmax)xmax=xsize;
	       if(ysize > ymax)ymax=ysize;
	       if(zsize > zmax)zmax=zsize;
	    }
	    ok=TRUE;
Loop:
	    if(in)fclose(in); 
	    in=NULL;
	    if(dd){
	        freeDD(dd,count);
	    	dd=NULL;
	    }
		count=0;
	    if(!ok)continue;
	}
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->zmin=zmin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	Files->zmax=zmax;
	if(Files->xminr > Files->xmaxr){
		Files->xminr=0;
		Files->yminr=0;
		Files->zminr=0;
		Files->xmaxr=(double)xmax;
		Files->ymaxr=(double)ymax;
		Files->zmaxr=(double)zmax;
	}
	Files->CurrentFrame = -1;
	Files->floatType = floatType;
	if(!Files->loadToMemory){
	    unsigned long length;

	    length=xmax*ymax*zmax*sizeof(double);
	    Files->dataSave=(unsigned char *)cMalloc(length,8432);
	    if(Files->dataSave == NULL){
	        goto ErrorOut;
	    }
	}
	
	doSDS3DStart(Files);

	return 0;
ErrorOut:
	sprintf(WarningBuff,"Error in initSDS3D\n");
	WarningBatch(WarningBuff);
	if(dd){
		freeDD(dd,count);
		dd=NULL;
	}
	if(in)fclose(in);
	if(Files->ImageCount > 0)return 0;
	freeFileList(Files);
	return 1;
}
int CheckSDS3DSpace(struct FileInfo2 *Files)
{
	struct SDS3D *SDS3DList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	SDS3DList=NULL;
	if(Files->SDS3DList){
	    SDS3DList=(struct SDS3D *)cRealloc((char *)Files->SDS3DList,ImageMax*sizeof(struct SDS3D),8761);
	    if(!SDS3DList){
	        sprintf(WarningBuff,"CheckSDS3DSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&SDS3DList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct SDS3D));
	}else{
	    SDS3DList=(struct SDS3D *)cMalloc(ImageMax*sizeof(struct SDS3D),8452);
	    if(!SDS3DList){
	        sprintf(WarningBuff,"CheckSDS3DSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)SDS3DList,ImageMax*sizeof(struct SDS3D));
	}

	Files->ImageMax=ImageMax;
	Files->SDS3DList=SDS3DList;

	return 0;
ErrorOut:
	if(SDS3DList){
	    Files->SDS3DList=SDS3DList;
	}
	return 1;
}
int FilesSetFrameSDS3D(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	double *data;

	if(!Files)return 1;
	s = &Files->SetData;
	s->SetFrame= -1;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	if(FilesGetSDS3Ddata(&data,&s->length,CurrentFrame,Files))return 1;

	s->data=(void *)data;
	s->SetFrame= CurrentFrame;
	s->type= SDS3D_FILES;
	return 0;
}
int FilesGetSDS3Ddata(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;

	if(!data || !length || !Files)return 1;


	if((*data=getFloat3D(CurrentFrame,Files)) == NULL)return 1;

	s = &Files->SetData;
	s->xsize=Files->SDS3DList[CurrentFrame].xsize;
	s->ysize=Files->SDS3DList[CurrentFrame].ysize;
	s->zsize=Files->SDS3DList[CurrentFrame].zsize;
	s->limits=Files->SDS3DList[CurrentFrame].limits;
	s->name = FilesImageName(Files,CurrentFrame);
	*length=s->xsize*s->ysize*s->zsize;

	return 0;
}
double *getFloat3D(long CurrentFrame,struct FileInfo2 *Files)
{
	double *data;
	long length;
	long offset;

	if(!Files)return (double *)NULL;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return (double *)NULL;
	if(Files->fileClass != SDS3D_FILES)return (double *)NULL;

	if(Files->fileType == SDS3D_DISK){

	    if(Files->CurrentFrame != CurrentFrame){

	        if(SetFileToRead(CurrentFrame,Files))return (double *)NULL;

	        offset=Files->SDS3DList[CurrentFrame].Offset;	

	        length=Files->SDS3DList[CurrentFrame].HdfSize;	


	        if(getFloat(Files->dataSave,(unsigned char *)xg.buffin,offset,
	                   length,Files->floatType,Files->in))return (double *)NULL;
	        flipSDS3DY((double *)Files->dataSave,Files->SDS3DList[CurrentFrame].xsize,
	                   Files->SDS3DList[CurrentFrame].ysize,Files->SDS3DList[CurrentFrame].zsize);


			Files->CurrentFrame=CurrentFrame;
	    }
	    data=(double *)Files->dataSave;
	}else{
	    data=(double *)Files->SDS3DList[CurrentFrame].buffer;		
		Files->CurrentFrame=CurrentFrame;
	}
	return data;
}
int initSDS2D(struct FileInfo2 *Files)
{
    long n,ImageCount,sdsCount,ImageFirst;
    long xmin,xmax,ymin,ymax;
    char buff[256];
    char **FilesNames;
	int floatType;
	FILE *in;
	DFdd2 *dd;
	long imageskip;
	long ImageIncrement;
    long count,k;
	char filepath[2048];

	if(!Files)return 1;

	in=NULL;
	dd=NULL;

	Files->fileClass=SDS2D_FILES;

	Files->pdefault=FilesOpenHDF;

	Files->pGetData=FilesGetSDS2Ddata;

	Files->pSetFrame=FilesSetFrameSDS2D;
	
	if(Files->loadToMemory){
	    Files->fileType=SDS2D_MEMORY;
	}else{
	    Files->fileType=SDS2D_DISK;
	}
		
	FilesNames=Files->FilesNames;

	ImageCount=0;
	sdsCount=0;
	xmin=ymin=200000;
	xmax=ymax=0;
	floatType = -1;
	
	imageskip=0;
	ImageIncrement=Files->ImageIncrement;
	if(ImageIncrement <= 0)ImageIncrement = 1;
	
	dd=NULL;
	count=0;
	for(n=0;n<Files->FileCount;++n){
	    long idim;
	    int ok;
	    long xsize,ysize;


	    dd=NULL;
		count=0;
	    ok=FALSE;
	    
	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    
	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }

	    if(getString((unsigned char *)buff,4L,in))goto Loop;

	    for(k=0;k<4;++k)if(buff[k] != NCSAtag[k]){
	       sprintf(WarningBuff,"%s is Not a SDS2D File\n",FilesNames[n]);
	       WarningBatch(WarningBuff);
	    	goto ErrorOut;
		}
	    if(getDD(&dd,&count,in))goto ErrorOut;

	    xsize=0;
	    ysize=0;
	    ImageFirst=ImageCount;
	    idim = -1;
	    for(k=0;k<count;++k){

	        if(dd[k].tag == DFTAG_NT){
	            char nt[4];
		    	fseek(in,dd[k].offset,0L); 
	            if(getString((unsigned char *)nt,4L,in))continue;
	            if(floatType == -1){
	                floatType=nt[1];
	            }else{
	                if(floatType != nt[1]){
	                    sprintf(WarningBuff,"Error file %s floatType missmatch %d != %d\n",
	                    					FilesNames[n],floatType,nt[1]);
	       				WarningBatch(WarningBuff);
	                }
	            }
	            continue;
	        }else if(dd[k].tag != DFTAG_SDD)continue;
		    	fseek(in,dd[k].offset,0L); 
	            {
	                int rank,kk;
	                long dim;

	                if(getInt(&rank,in))goto ErrorOut;
	                if(rank != 2){
	                    sprintf(WarningBuff,"Skipping SDS with rank %d\n",rank);
	       				WarningBatch(WarningBuff);
	                    continue;
	                }
	                for(kk=0;kk<rank;++kk){
					    if(getLong(&dim,in))goto ErrorOut;
					    if(kk == 0){
							ysize=dim;
					    }else if(kk == 1){
							xsize=dim;
					    }
	                }
	            }
	           
	        idim = k;
	        break;
	    }
	    if(idim == -1){
	        sprintf(WarningBuff,"Error No Limits for SDS %s\n",FilesNames[n]);
	       	WarningBatch(WarningBuff);
			goto ErrorOut;
	    }
	    for(k=0;k<count;++k){
	        unsigned char *buffer;
	        char *name,*command;
	        long offset,length,idata,kk;

	        if(dd[k].tag == DFTAG_SDD){
		    fseek(in,dd[k].offset,0L); 
	            {
	                int rank;
	                long dim;
	                if(getInt(&rank,in))goto ErrorOut;
	                if(rank != 2){
	                    sprintf(WarningBuff,"Skipping SDS with rank %d\n",rank);
	       				WarningBatch(WarningBuff);
	                    continue;
	                }
	                for(kk=0;kk<rank;++kk){
						if(getLong(&dim,in))goto ErrorOut;
						if(kk == 0){
							ysize=dim;
						}else if(kk == 1){
							xsize=dim;
						}
	                }
	            }	            
	            continue;	            
	        }else if(dd[k].tag != DFTAG_SD)continue;
	        
	    if(++imageskip < ImageIncrement)continue;
	    imageskip=0;
	    
		if(CheckSDS2DSpace(Files))goto ErrorOut;
		idata=k;
		
	    for(kk=k;kk<count;++kk){
			int kkk;
	        if(dd[kk].tag == DFTAG_SDD){
				fseek(in,dd[kk].offset,0L); 
	            {
	                int rank;
	                long dim;
	                if(getInt(&rank,in))goto ErrorOut;
	                if(rank != 2){
	                    sprintf(WarningBuff,"Skipping SDS with rank %d\n",rank);
	       				WarningBatch(WarningBuff);
	                    continue;
	                }
	                for(kkk=0;kkk<rank;++kkk){
						if(getLong(&dim,in))goto ErrorOut;
						if(kkk == 0){
							ysize=dim;
						}else if(kkk == 1){
							xsize=dim;
						}
	                }
					if(dd[kk].ref == dd[k].ref)
					{
					    dd[kk].ref=dd[kk].ref;
						break;
					}
	            }	            
			}
		}
		
		{
		    long kref,cref;
		    name = NULL;
		    command = NULL;
		    kref = -1;
		    cref = -1;
		    for(kk=0;kk<count;++kk){
				if(dd[kk].tag == DFTAG_DIL){
				    if(dd[kk].labelref == dd[k].ref)kref = kk;
				}
				if(dd[kk].tag == DFTAG_DIA){
				    if(dd[kk].labelref == dd[k].ref)cref = kk;
				}
			}
			if(kref >= 0){
			    if(dd[kref].labelname){
			        name=dd[kref].labelname;
				    dd[kref].labelname=NULL;
				}
		    }
			if(cref >= 0){
			    if(dd[cref].labelname){
			        command=dd[cref].labelname;
				    dd[cref].labelname=NULL;
				}
		    }
		}

		offset=dd[idata].offset;
		length=dd[idata].length;
		buffer=NULL;

		if(Files->loadToMemory){
		    long lengthc;

		    if(floatType == DFNT_FLOAT64){
		    	lengthc=(long)(length);
		    }else{
			    lengthc=(long)(2*length);
	    	}

		    buffer=(unsigned char *)cMalloc(lengthc,8116);
		    if(!buffer)goto ErrorOut;

		    if(getFloat(buffer,(unsigned char *)xg.buffin,offset,length,floatType,in))goto Loop;

		}

		Files->SDS2DList[ImageCount].Offset=offset;
		Files->SDS2DList[ImageCount].buffer=buffer;
		Files->SDS2DList[ImageCount].HdfSize=length;

		Files->SDS2DList[ImageCount].length=xsize*ysize;
		Files->SDS2DList[ImageCount].xsize=(int)xsize;
		Files->SDS2DList[ImageCount].ysize=(int)ysize;
		Files->ImageFiles[ImageCount]=n;
		if(name == NULL){
		    char buff[256];

		    if(ImageFirst == ImageCount){
		        sprintf(buff,"%s%c",FilesNames[n],0);
		    }else{
		        sprintf(buff,"%s.%ld%c",FilesNames[n],ImageCount,0);
		    }
		    Files->ImageNames[ImageCount]=strsave(buff,8233);
		    if(!Files->ImageNames[ImageCount]){
       			WarningBatch("initSDS2D Out Of Memory\n");
		    	goto ErrorOut;
		    }
		}else{
		    Files->ImageNames[ImageCount]=name;
		}
		buffer=NULL;
		Files->ImageCount = ++ImageCount;
		if(command){
		    FilePutLimitsString(command,ImageCount-1,Files);
		}
	    sdsCount++;
		if(xsize > xmax)xmax=xsize;
		if(ysize > ymax)ymax=ysize;
		if(xsize < xmin)xmin=xsize;
		if(ysize < ymin)ymin=ysize;
	        
	    }
	    ok=TRUE;
Loop:
	    if(in)fclose(in); 
	    in=NULL;
	    if(dd){
	        freeDD(dd,count);
	        dd=NULL;
	    }
	    if(!ok)continue;
	}
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	if(Files->xminr > Files->xmaxr){
		Files->xminr=0;
		Files->yminr=0;
		Files->xmaxr=(double)xmax;
		Files->ymaxr=(double)ymax;
	}
	Files->CurrentFrame = -1;
	Files->floatType = floatType;
	if(!Files->loadToMemory){
	    unsigned long length;

	    length=xmax*ymax*sizeof(double);
	    Files->dataSave=(unsigned char *)cMalloc(length,8117);
	    if(Files->dataSave == NULL){
	        goto ErrorOut;
	    }
	}
	
	doSDS2DStart(Files);

	return 0;
ErrorOut:
	sprintf(WarningBuff,"Error in initSDS2D\n");
	WarningBatch(WarningBuff);
	if(dd){
	    freeDD(dd,count);
	    dd=NULL;
	}
	if(in)fclose(in);
	if(Files->ImageCount > 0)return 0;
	freeFileList(Files);
	return 1;
}

struct FileInfo2 *FilesSDS2D(void)
{
	struct FileInfo2 *Files;
	
	Files=GetFileInfoStruct(8113);
    if(!Files){
        WarningBatch("FilesSDS2D out of Memory\n");
        return NULL;
    }	    
    
    Files->fileClass = SDS2D_FILES;
    
    Files->loadToMemory=1;
    Files->fileType=SDS2D_MEMORY;
    
    Files->CurrentFrame = -1;
	Files->pSetFrame=FilesSetFrameSDS2D;
	Files->pGetData=FilesGetSDS2Ddata;
    Files->pdefault=FilesOpenHDF;
    
	mstrncpy(Files->paletteName,"ps",3);
	getPaletteByName(Files->paletteName,(unsigned char *)Files->palette);
	
    doSDS2DStart(Files);
    
    return Files;

}
struct FileInfo2 *FilesSDS3D(void)
{
	struct FileInfo2 *Files;
	
	Files=GetFileInfoStruct(8113);
    if(!Files){
        WarningBatch("FilesSDS3D out of Memory\n");
        return NULL;
    }	    
        
    Files->loadToMemory=1;
    Files->fileType=SDS3D_MEMORY;
    
    Files->CurrentFrame = -1;
	Files->fileClass=SDS3D_FILES;
	Files->pdefault=FilesOpenSDS3D;
	Files->pGetData=FilesGetSDS3Ddata;
	Files->pSetFrame=FilesSetFrameSDS3D;
    
	mstrncpy(Files->paletteName,"ps",3);
	getPaletteByName(Files->paletteName,(unsigned char *)Files->palette);
	
    doSDS3DStart(Files);
    
    return Files;

}
int FileAppendSDS2D(char *name,int xsize,int ysize,
                  double *image,long CurrentFrame,struct FileInfo2 **iFiles)
{
	struct FileInfo2 *Files;
	unsigned char *buffer;
	long ImageCount,xmax,ymax,Length;
	int finc,ret;



	if(!iFiles)return 1;

	ret = 1;

	Length=(long)(xsize*ysize*sizeof(double));
	
	Files= *iFiles;
	if(Files){
	    if(Files->fileClass != SDS2D_FILES){
	       freeFileList(Files);
	       *iFiles=Files=NULL;
	    }
	}	
	
	if(!Files){
	    Files=FilesSDS2D();
	    if(!Files){
	     	WarningBatch("FileAppendSDS2D - FilesSDS2D Failed\n");
	        goto ErrorOut;
	    }
	}
	
    if(!Files->loadToMemory){
	     WarningBatch("FileAppendSDS2D Can Append Only to Memory Resident Files\n");
	     goto ErrorOut;
    }
   	
	if(CheckSDS2DSpace(Files))return 1;
	ImageCount=Files->ImageCount;
	if(CurrentFrame > ImageCount){
	   CurrentFrame=ImageCount;
	}
	if(CurrentFrame < ImageCount){
        ImageCount=CurrentFrame;
        if(Files->SDS2DList[ImageCount].buffer)cFree((char *)Files->SDS2DList[ImageCount].buffer);
        Files->SDS2DList[ImageCount].buffer=NULL;
        if(Files->ImageNames[ImageCount])cFree((char *)Files->ImageNames[ImageCount]);
        Files->ImageNames[ImageCount]=NULL;
	    finc=0;
	}else {
	    finc=1;
	}
	if(Files->loadToMemory){
	    buffer=(unsigned char *)cMalloc(Length,8114);
	    if(!buffer){
	       WarningBatch("FileAppendSDS2D out of Memory\n");
	       goto ErrorOut;
	    }
	    uMemcpy(buffer,image,Length);
	}else{
	    buffer=NULL;
	}
	Files->SDS2DList[ImageCount].buffer=buffer;
	Files->SDS2DList[ImageCount].HdfSize=Length;
	Files->SDS2DList[ImageCount].length=xsize*ysize;
	Files->SDS2DList[ImageCount].xsize=xsize;
	Files->SDS2DList[ImageCount].ysize=ysize;
	if(name && name[0]){
	    Files->ImageNames[ImageCount]=strsave(name,8231);
	}else{
		char iname[256];
		sprintf(iname,"sds2d.%04ld%c",ImageCount,0);
	    Files->ImageNames[ImageCount]=strsave(iname,8232);
	}
	if(Files->ImageNames[ImageCount] == NULL){
	    WarningBatch("FileAppendSDS2D out of Memory\n");
	    goto ErrorOut;
	}
	Files->ImageFiles[ImageCount]=0;

	if(finc)++Files->ImageCount;

	xmax=Files->xmax;
	ymax=Files->ymax;
	if(xsize < Files->xmin)Files->xmin=xsize;
	if(xsize > Files->xmax)Files->xmax=xsize;
	if(ysize < Files->ymin)Files->ymin=ysize;
	if(ysize > Files->ymax)Files->ymax=ysize;
	if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
	    if(Files->dataSave)cFree((char *)Files->dataSave);
	    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
	    Files->dataSave=(unsigned char *)cMalloc(Files->length,8115);
	    if(Files->dataSave == NULL){
	    	WarningBatch("FileAppendSDS2D out of Memory\n");
	       goto ErrorOut;
	    }
	    Files->CurrentFrame = -1;
	}
	
	if(Files->ImageCount == 1)doSDS2DStart(Files);
	
	*iFiles=Files;
	ret = 0;
ErrorOut:
	return ret;
}
int FileAppendSDS3D(char *name,int xsize,int ysize,int zsize,
                  double *image,long CurrentFrame,struct FileInfo2 **iFiles)
{
	struct FileInfo2 *Files;
	unsigned char *buffer;
	long ImageCount,xmax,ymax,zmax,Length;
	int finc,ret;

	if(!iFiles)return 1;
	
	ret=1;
	
	Length=(long)(xsize*ysize*zsize*sizeof(double));
	
	Files= *iFiles;
	if(Files){
	    if(Files->fileClass != SDS3D_FILES){
	       freeFileList(Files);
	       *iFiles=Files=NULL;
	    }
	}	
	
	if(!Files){
	    Files=FilesSDS3D();
	    if(!Files){
	   		WarningBatch("FileAppendSDS3D - FilesSDS3D Failed\n");
	   		goto ErrorOut;
	    }
	}
	
    if(!Files->loadToMemory){
	   WarningBatch("FileAppendSDS3D Can Append Only to Memory Resident Files\n");
	   goto ErrorOut;
    }
   	
	if(CheckSDS3DSpace(Files))return 1;
	ImageCount=Files->ImageCount;
	if(CurrentFrame > ImageCount){
	   CurrentFrame=ImageCount;
	}
	if(CurrentFrame < ImageCount){
        ImageCount=CurrentFrame;
        if(Files->SDS3DList[ImageCount].buffer)cFree((char *)Files->SDS3DList[ImageCount].buffer);
        Files->SDS3DList[ImageCount].buffer=NULL;
        if(Files->ImageNames[ImageCount])cFree((char *)Files->ImageNames[ImageCount]);
        Files->ImageNames[ImageCount]=NULL;
	    finc=0;
	}else {
	    finc=1;
	}
	if(Files->loadToMemory){
	    buffer=(unsigned char *)cMalloc(Length,8114);
	    if(!buffer){
	   		WarningBatch("FileAppendSDS3D out of Memory\n");
	   		goto ErrorOut;
	    }
	    uMemcpy(buffer,image,Length);
	}else{
	    buffer=NULL;
	}
	Files->SDS3DList[ImageCount].buffer=buffer;
	Files->SDS3DList[ImageCount].HdfSize=Length;
	Files->SDS3DList[ImageCount].length=xsize*ysize*zsize;
	Files->SDS3DList[ImageCount].xsize=xsize;
	Files->SDS3DList[ImageCount].ysize=ysize;
	Files->SDS3DList[ImageCount].zsize=zsize;
	if(name){
	    Files->ImageNames[ImageCount]=strsave(name,8231);
	}else{
		char iname[256];
		sprintf(iname,"sds3d.%04ld%c",ImageCount,0);
	    Files->ImageNames[ImageCount]=strsave(iname,8232);
	}
	if(Files->ImageNames[ImageCount] == NULL){
	   WarningBatch("FileAppendSDS3D out of Memory\n");
	   goto ErrorOut;
	}
	Files->ImageFiles[ImageCount]=0;

	if(finc)++Files->ImageCount;

	xmax=Files->xmax;
	ymax=Files->ymax;
	zmax=Files->zmax;
	if(xsize < Files->xmin)Files->xmin=xsize;
	if(xsize > Files->xmax)Files->xmax=xsize;
	if(ysize < Files->ymin)Files->ymin=ysize;
	if(ysize > Files->ymax)Files->ymax=ysize;
	if(zsize < Files->zmin)Files->zmin=zsize;
	if(zsize > Files->zmax)Files->zmax=zsize;
	if(!Files->dataSave || (xsize > xmax) || (ysize > ymax) || (zsize > zmax)){
	    if(Files->dataSave)cFree((char *)Files->dataSave);
	    Files->length=(long)(Files->zmax*Files->ymax*Files->xmax*sizeof(double));
	    Files->dataSave=(unsigned char *)cMalloc(Files->length,8115);
	    if(Files->dataSave == NULL){
	   		WarningBatch("FileAppendSDS3D out of Memory\n");
	   		goto ErrorOut;
	    }
	    Files->CurrentFrame = -1;
	}
	
	if(Files->ImageCount == 1)doSDS3DStart(Files);
	
	*iFiles=Files;
	
	ret=0;
	
ErrorOut:
	return ret;
}
int FilesSaveFiles(struct FileInfo2 *Files)
{	
	if(!Files)return 1;
	
	if(Files->fileClass == SDS2D_FILES){	
		if(Files->outputType == SDS2D_FILES){
		    return FilesSaveSDS2D(Files);
		}else if(Files->outputType == HDF_FILES){
		    return FilesSaveHDF2(Files);
		}
	}else if(Files->fileClass == SDS3D_FILES){
		if(Files->outputType == SDS3D_FILES){
		    return FilesSaveHDF3(Files);
		}
	}
	return 1;	
}
int FilesSaveSDS2D(struct FileInfo2 *Files)
{	
	struct SetFrameData *s;
	char name[256],*namef;
	struct LIMITS *limits;
	char filepath[2048];
	int flag;
	long n;
	
	if(!Files)return 1;
	s = &Files->SetData;
	
	flag=0;
	filepath[0]=0;
	mstrncpy(filepath,Files->outputDirectory,2048);
	strncatToPath(filepath,Files->outputName,2048);
	for(n=0;n<Files->ImageCount;++n){
		if(FilesSetFrameSDS2D(n,Files))return 1;

	    if(n >= (Files->ImageCount-1))flag=1;
	    
		namef=FilesImageName(Files,n);
		if(namef){
			mstrncpy((char *)name,namef,255);	
		}else{
			sprintf(name,"image.%04ld%c",n,0);
		}
				
		limits=NULL;
		FileGetLimits(&limits,n,Files);    

	    if(sds2DOut(filepath,name,s->xsize,s->ysize,(double *)s->data,n,flag,limits))return 1;

	}
    
	return 0;
}
int FilesSaveHDF3(struct FileInfo2 *Files)
{	
	struct SetFrameData *s;
	char name[256],*namef;
	struct LIMITS *limits;
	char filepath[2048];
	int flag,flagOne;
	long n,nn;
	
	if(!Files)return 1;
	s = &Files->SetData;
	
	
	
	flagOne=Files->outputName[0];
	
	if(flagOne){  /* write merged file */
		flag=0;
		filepath[0]=0;
		mstrncpy(filepath,Files->outputDirectory,2048);
		strncatToPath(filepath,Files->outputName,2048);
	}else{
		flag=1;
	}
	
	for(n=0;n<Files->ImageCount;++n){
		if(FilesSetFrameSDS3D(n,Files))return 1;

	    if(n >= (Files->ImageCount-1))flag=1;
	    
		namef=FilesImageName(Files,n);
		if(namef){
			mstrncpy((char *)name,namef,255);	
		}else{
			sprintf(name,"image.%04ld%c",n,0);
		}
				
		limits=NULL;
		FileGetLimits(&limits,n,Files);
		
		nn=n;
		if(!flagOne){
		    nn=0;
			filepath[0]=0;
			mstrncpy(filepath,Files->outputDirectory,2048);
			strncatToPath(filepath,namef,2048);
		} 
		
	    if(sds3DOut(filepath,name,Files->xSize,Files->ySize,Files->zSize,(double *)s->data,nn,flag,limits))return 1;
	}
	
    FilesSetFrameSDS3D(0,Files);
    
	return 0;
}
int FilesSaveHDF2(struct FileInfo2 *Files)
{	
	struct SetFrameData *s;
	char name[256],*namef;
	struct LIMITS *limits;
	char filepath[2048];
	struct paletteDraw pd;
	int flag;
	long n;
	
	if(!Files)return 1;
	s = &Files->SetData;
	
	zerol((char *)&pd,sizeof(pd));
	
	pd.sType=2;
	
	flag=0;
	filepath[0]=0;
	mstrncpy(filepath,Files->outputDirectory,2048);
	strncatToPath(filepath,Files->outputName,2048);
	for(n=0;n<Files->ImageCount;++n){
		if(FilesSetFrameSDS2D(n,Files))return 1;

	    if(n >= (Files->ImageCount-1))flag=1;
	    
		namef=FilesImageName(Files,n);
		if(namef){
			mstrncpy((char *)name,namef,255);	
		}else{
			sprintf(name,"image.%04ld%c",n,0);
		}
				
		limits=NULL;
		FileGetLimits(&limits,n,Files);    
		
		pd.sPmin=limits->vmin;
		pd.sPmax=limits->vmax;
		
		FloatToImage((double *)s->data,s->xsize*s->ysize,&pd,NULL);

	    if(hdfOut(filepath,name,(char *)Files->palette,s->xsize,s->ysize,(char *)s->data,n,flag,limits))return 1;
	}
	
    FilesSetFrameSDS2D(0,Files);
    
	return 0;
}

int FilesSetFrameSDS2D(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	double *data;

	if(!Files)return 1;
	s = &Files->SetData;
	s->SetFrame= -1;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;

	if(FileSDS2DInfo(&s->name,&s->xsize,&s->ysize,&s->length,CurrentFrame,Files))return 1;
	if(FilesGetSDS2Ddata(&data,&s->length,CurrentFrame,Files))return 1;
	s->data=(void *)data;
	s->SetFrame= CurrentFrame;
	s->type= SDS2D_FILES;
	return 0;
}


int FilesGetSDS2Dvalue(double *value,int ix,int iy,long CurrentFrame,
						struct FileInfo2 *Files)
{

	double *data;
	int xsize;
	int ysize;
	long length;
	long n;


	if(!value)return 1;
	if(!Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != SDS2D_FILES)return 1;
	if(Files->fileType == SDS2D_DISK){
	   if(Files->CurrentFrame != CurrentFrame){
	       sprintf(WarningBuff,"FilesGetSDS2Dvalue data-frame error\n");
	       WarningBatch(WarningBuff);
	       return 1;
	   }
	   data=(double *)Files->dataSave;		
	}else{
	   data=(double *)Files->SDS2DList[CurrentFrame].buffer;		
	}
	length=Files->SDS2DList[CurrentFrame].length;
	xsize=Files->SDS2DList[CurrentFrame].xsize;
	ysize=Files->SDS2DList[CurrentFrame].ysize;
	
	if(ix >= xsize)ix = xsize-1;
	if(iy >= ysize)iy = ysize-1;
	
	if(ix < 0)ix=0;
	if(iy < 0)iy=0;
	
	
	n=ix+xsize*iy;
	if(n < 0 || n >= length){
	   WarningBatch("FilesGetSDS2Dvalue n out of Range\n");
	   return 1;
	}
	
	*value=data[n];
	
	return 0;
}
int FilesGetSDS2DTextvalue(char *buff,int ix,int iy,long CurrentFrame,
						struct FileInfo2 *Files)
{

	double *data;
	int xsize;
	int ysize;
	long length;
	long n;


	if(!buff)return 1;
	sprintf(buff,"No SDS2D value");
	if(!Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != SDS2D_FILES)return 1;
	if(Files->fileType == SDS2D_DISK){
	   if(Files->CurrentFrame != CurrentFrame){
	       sprintf(WarningBuff,"FilesGetSDS2DTextvalue data-frame error\n");
	       WarningBatch(WarningBuff);
	       return 1;
	   }
	   data=(double *)Files->dataSave;		
	}else{
	    data=(double *)Files->SDS2DList[CurrentFrame].buffer;		
	}
	length=Files->SDS2DList[CurrentFrame].length;
	xsize=Files->SDS2DList[CurrentFrame].xsize;
	ysize=Files->SDS2DList[CurrentFrame].ysize;
	if(ix < 0 || ix >= xsize || iy < 0 || iy >= ysize){
		sprintf(buff,"out of Range");
		return 1;
	}
	
	n=ix+xsize*iy;
	if(n < 0 || n >= length){
	   sprintf(buff,"n Out Of Range");
	   return 0;
	}
	sprintf(buff,"%g%c",data[n],0);

	return 0;
}
int FileSDS2DInfo(char **ImageName,long *xsize,long *ysize,
	          long *Length,long CurrentFrame,
	          struct FileInfo2 *Files)
{
	if(!ImageName || !xsize || !ysize || !Length || !Files)return 1;
	if(Files->fileClass != SDS2D_FILES)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	*xsize=Files->SDS2DList[CurrentFrame].xsize;
	*ysize=Files->SDS2DList[CurrentFrame].ysize;
	*Length=Files->SDS2DList[CurrentFrame].length;
	*ImageName= FilesImageName(Files,CurrentFrame);
	return 0;	
}
int FilesGetSDS2Ddata(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files)
{
	if(!data || !length || !Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != SDS2D_FILES)return 1;

	if((*data=getFloatp(CurrentFrame,Files)) == NULL)return 1;
	*length=Files->SDS2DList[CurrentFrame].length;
	return 0;
}
double *getFloatp(long CurrentFrame,struct FileInfo2 *Files)
{
	double *data;
	long lengthc;
	long length;
	long offset;

	if(!Files)return (double *)NULL;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return (double *)NULL;
	if(Files->fileClass != SDS2D_FILES)return (double *)NULL;

	if(Files->fileType == SDS2D_DISK){

	    if(Files->CurrentFrame != CurrentFrame){

	        if(SetFileToRead(CurrentFrame,Files))return (double *)NULL;

	        offset=Files->SDS2DList[CurrentFrame].Offset;	

	        length=Files->SDS2DList[CurrentFrame].HdfSize;	

	        lengthc=(long)(length*sizeof(double)/4);

	        if(SetBuffers(lengthc))return (double *)NULL;

	        if(getFloat(Files->dataSave,(unsigned char *)xg.buffin,offset,
	                   length,Files->floatType,Files->in))return (double *)NULL;

			Files->CurrentFrame=CurrentFrame;
	    }
	    data=(double *)Files->dataSave;
	}else{
	    data=(double *)Files->SDS2DList[CurrentFrame].buffer;		
		Files->CurrentFrame=CurrentFrame;
	}
	return data;
}
int SetFileToRead(long CurrentFrame,struct FileInfo2 *Files)
{
	if(!Files || !Files->ImageFiles)return 1;
	
    if(Files->FileOpen >= 0){ 
       if(Files->ImageFiles[CurrentFrame] != Files->FileOpen){
           if(Files->in)fclose(Files->in);
	   		Files->in=NULL;
		}
    }
    if(!Files->in){
        char name[2048];


        Files->FileOpen=Files->ImageFiles[CurrentFrame];
        
		if(Files->FileOpen < 0 || Files->FileOpen >= Files->FileCount){
		    sprintf(WarningBuff,"Error FileOpen %ld out of Range FileCount %ld\n",
		            (long)Files->FileOpen,(long)Files->FileCount);
			WarningBatch(WarningBuff);
		    return 1;
		}
		
        if(Files->directory[0]){
            mstrncpy(name,Files->directory,2048);
        }else{
            name[0]=0;
        }
        
        if(Files->FilesNames && Files->FilesNames[Files->FileOpen])
            strncatToPath(name,Files->FilesNames[Files->FileOpen],2048);
        
        FixPathName(name);
        Files->in = fopen(name,"rb");
        if(Files->in == NULL){
	    	sprintf(WarningBuff,"Could Not Open %s To Read\n",name);
			WarningBatch(WarningBuff);
            return 1;
        }
       
    }
	return 0;
}
int SetFileToRead8(long CurrentFrame,struct FileInfo2 *Files)
{
	if(!Files || !Files->ImageFiles)return 1;
	
    if(Files->FileOpen >= 0){ 
		if(Files->ImageFiles[CurrentFrame] != Files->FileOpen){
		       if(Files->in8)fclose8(Files->in8);
		   		Files->in8=NULL;
		}
    }
    if(!Files->in8){
        char name[2048];


    	Files->FileOpen=Files->ImageFiles[CurrentFrame];
         
		if(Files->FileOpen < 0 || Files->FileOpen >= Files->FileCount){
		    sprintf(WarningBuff,"Error FileOpen %ld out of Range FileCount %ld\n",
		            (long)Files->FileOpen,(long)Files->FileCount);
			WarningBatch(WarningBuff);
		    return 1;
		}
        if(Files->directory[0]){
            mstrncpy(name,Files->directory,2048);
        }else{
            name[0]=0;
        }
        
        if(Files->FilesNames && Files->FilesNames[Files->FileOpen])
            strncatToPath(name,Files->FilesNames[Files->FileOpen],2048);
        FixPathName(name);
        Files->in8 = fopen8(name,"rb");
        if(Files->in8 == NULL){
	    	sprintf(WarningBuff,"Could Not Open %s To Read\n",name);
			WarningBatch(WarningBuff);
            return 1;
        }
       
    }
	return 0;
}

int CheckSDS2DSpace(struct FileInfo2 *Files)
{
	struct SDS2D *SDS2DList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	SDS2DList=NULL;
	if(Files->SDS2DList){
	    SDS2DList=(struct SDS2D *)cRealloc((char *)Files->SDS2DList,ImageMax*sizeof(struct SDS2D),8260);
	    if(!SDS2DList){
	        sprintf(WarningBuff,"CheckSDS2DSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&SDS2DList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct SDS2D));
	}else{
	    SDS2DList=(struct SDS2D *)cMalloc(ImageMax*sizeof(struct SDS2D),8118);
	    if(!SDS2DList){
	        sprintf(WarningBuff,"CheckSDS2DSpace out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)SDS2DList,ImageMax*sizeof(struct SDS2D));
	}

	Files->ImageMax=ImageMax;
	Files->SDS2DList=SDS2DList;

	return 0;
ErrorOut:
	if(SDS2DList){
	    Files->SDS2DList=SDS2DList;
	}
	return 1;
}
int getFloat(unsigned char *buffer,unsigned char *bs,
             long offset,long length,int floatType, FILE *in)
{

	if(!in)return 1;
	
	fseek(in,offset,0); 

#ifdef CRAY
/*   do cray */
	{
	    if(getString((char *)bs,length,in)){
	        return 1;
	    }
	}
        {
            int num1=1,num0=0,type,iee,nf;

	    if(floatType == DFNT_FLOAT64){
	        type=8;
                nf=length/8;
	    }else{
	        type=2;
                nf=length/4;
            }
            iee = IEG2CRAY(&type, &nf, bs, &num0, buffer, &num1);
            if(iee != 0){
                sprintf(WarningBuff,"ERROR IEG2CRAY CONVERTING DATA iee %d\n",iee);
				WarningBatch(WarningBuff);
                return 1;
            }
        }
#else
	//bs=bs;
	if(getString((unsigned char *)buffer,length,in)){
	    return 1;
	}
#ifdef PC
/*   do PC */
        {
            unsigned char c,*cp;
	    long np;
	    cp=(unsigned char *)buffer;
	    if(floatType == DFNT_FLOAT64){
	        for(np=0;np<length;np += 8){
		    c=cp[np];
		    cp[np]=cp[np+7];
		    cp[np+7]=c;
		    c=cp[np+1];
		    cp[np+1]=cp[np+6];
		    cp[np+6]=c;
		    c=cp[np+2];
		    cp[np+2]=cp[np+5];
		    cp[np+5]=c;
		    c=cp[np+3];
		    cp[np+3]=cp[np+4];
		    cp[np+4]=c;
	        }
	    }else{
	        for(np=0;np<length;np += 4){
		    c=cp[np];
		    cp[np]=cp[np+3];
		    cp[np+3]=c;
		    c=cp[np+1];
		    cp[np+1]=cp[np+2];
		    cp[np+2]=c;
	        }
	    }
        }
#endif
	if(floatType != DFNT_FLOAT64){
	    double *dp;
	    float *fp;
	    long n,nf;

	    nf=length/4;
	    dp=(double *)buffer;
	    fp=(float *)buffer;
	    for(n=nf-1;n>=0;--n){
	        dp[n]=fp[n];
	    }
	}
#endif
	return 0;
}
#define DD_INC 2000
int freeDD(DFdd2 *dd,long dcount)
{
	long k;
	
	if(!dd)return 1;
	for(k=0;k<dcount;++k){
        if(dd[k].labelname){
            cFree((char *)dd[k].labelname);
	    	dd[k].labelname=NULL;
		}
	}
    cFree((char *)dd);
	return 0;
}
int getDD(DFdd2 **ddi,long *count,FILE *in)
{
	DFdd2 *dd;
	long maxCount,Count;
	int k;

	if(!ddi || !count || !in)return 1;

	dd=NULL;
	Count=0;
	maxCount=0;
	*count = 0;
	while(1){
	    int ndd;
	    long next;

	    if(getInt(&ndd,in))break;
	    if(getLong(&next,in))break;
	    for(k=0;k<ndd;++k){
	        if(Count+1 >= maxCount){
	            if(dd){
			        DFdd2 *save;
					save=(DFdd2 *)cRealloc((char *)dd,(maxCount+DD_INC)*sizeof(struct DFdd2),7261);
					if(!save){
					    WarningBatch("getDD out of Memory\n");
					    cFree((char *)dd);
					    return 1;
					}
					dd=save;
					zerol((char *)&dd[maxCount],DD_INC*sizeof(struct DFdd2));
					maxCount += DD_INC;
	            }else{
	               	dd=(DFdd2 *)cMalloc(DD_INC*sizeof(struct DFdd2),8119);
	               	if(dd == NULL){
			    		WarningBatch("getDD out of Memory\n");
	                   	return 1;
	               	}
					zerol((char *)dd,DD_INC*sizeof(struct DFdd2));
		       		maxCount=DD_INC;
	            }
	        }
	        if(getInt((int *)&dd[Count].tag,in))goto OutOfHere;
	        if(dd[Count].tag == DFTAG_NULL)continue;
	        if(getInt((int *)&dd[Count].ref,in))goto OutOfHere;
	        if(getLong(&dd[Count].offset,in))goto OutOfHere;
	        if(getLong(&dd[Count].length,in))goto OutOfHere;
	        ++Count;
	    }
	    if(next == 0L)break;
	    fseek(in,next,0L); 
	}
OutOfHere:
	if(Count == 0){
	    if(dd)cFree((char *)dd);
	    return 1;
	}

	if(dd == NULL)return 1;

	{
	    unsigned char name[262];
	    long n,kref;
	    for(n=0;n<Count;++n){
	        if(dd[n].tag == DFTAG_SDS || dd[n].tag == DFTAG_SDG){
	             dd[n].labelname=(char *)cMalloc(dd[n].length,8712);
			     if(dd[n].labelname == NULL)break;
			    fseek(in,dd[n].offset,0L); 
			    if(getString((unsigned char *)dd[n].labelname,dd[n].length,in))break;	        
	        }else if(dd[n].tag == DFTAG_DIL || dd[n].tag == DFTAG_DIA){
			    if(dd[n].length <= 0 || dd[n].length > 260)continue;
			    fseek(in,dd[n].offset,0L); 
			    if(getString((unsigned char *)name,dd[n].length,in))break;
			    dd[n].labeltag=name[0]*256+name[1];
			    dd[n].labelref=name[2]*256+name[3];
			    name[dd[n].length]=0;
			    kref=4;
			    while((name[kref-4]=name[kref]) != 0){
			       ++kref;
			    }
			    dd[n].labelname=strsave((char *)name,8234);
			    if(dd[n].labelname == NULL)break;
			    k=strlen(dd[n].labelname);
			    while(--k > 0){
			        if(dd[n].labelname[k] == ' '){
			            dd[n].labelname[k]=0;
			        }else{
			            break;
			        }
			    }
	        }
	    }
	}
	*ddi=dd;
	*count=Count;
	return 0;
}
int initRGB(struct FileInfo2 *Files)
{
    unsigned char *buffer;
    long n,count,ImageCount;
    char buff[256];
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
    unsigned char *bp;
    long iskip;
	FILE *in;
	char filepath[2048];
/* image header start */
    int	imagic;
    int itype;
    int idim;
    int xsize;
    int ysize;
    int izsize;
/* image header end */

	if(!Files)return 1;

	in=NULL;
	Files->fileClass=RGB_FILES;
	Files->pSetFrame=FilesSetFrameRGB;
	Files->pdefault=FilesOpenHDF;
	Files->pGetInfo=FilesInfoRGB;
/*
	Files->pdefault=FilesOpenRGB;
	Files->pSaveOne=FilesSaveRGB;
	Files->pSaveOneMerged=FilesMergedRGB;
*/
	FilesNames=Files->FilesNames;

	
	if(Files->loadToMemory){
	    Files->fileType=RGB_MEMORY;
	}else{
	    Files->fileType=RGB_DISK;
	}
			
	count=0;
	ImageCount=0;
	xmax=ymax=0;
	xmin=ymin=200000;
	iskip=0;
	
	    buffer=NULL;
	    
	for(n=0;n<Files->FileCount;++n){
	    long length;
	    int ok;


	    ok=FALSE;
	    buffer=NULL;

	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    	    
	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }
	    
	    length=FileLength(in);
	    if(length == -1L){
	        sprintf(WarningBuff,"(%s) Error Looking For File Length\n",FilesNames[n]);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    
	    if(Files->loadToMemory){
		    buffer=(unsigned char *)cMalloc(length,8121);
		    if(!buffer)goto ErrorOut;
		    if(getString(buffer,length,in))goto ErrorOut;
	    	bp=(unsigned char *)buffer;
	    }else{
	    	if(getString((unsigned char *)buff,20L,in))goto ErrorOut;
	    	bp=(unsigned char *)buff;
	    } 
	    
		imagic = (((unsigned short)bp[0]) << 8) + bp[1];
		itype =  (((unsigned short)bp[2]) << 8) + bp[3];
		idim =   (((unsigned short)bp[4]) << 8) + bp[5];
		xsize = (((unsigned short)bp[6]) << 8) + bp[7];
		ysize = (((unsigned short)bp[8]) << 8) + bp[9];
		izsize = (((unsigned short)bp[10]) << 8) + bp[11];
	    
		if(imagic != 0732){
		    sprintf(WarningBuff,"(%s) is not a sgi file tag is (%d)\n",FilesNames[n],imagic);
	        WarningBatch(WarningBuff);
		    goto ErrorOut;
		}
		if(itype != 257 && itype != 1){
		    sprintf(WarningBuff,"(%s) sgi file bad type is (%d)\n",FilesNames[n],itype);
	        WarningBatch(WarningBuff);
		    goto Loop;
		}
		if(idim != 3){
		    sprintf(WarningBuff,"(%s) sgi file bad idim is (%d)\n",FilesNames[n],idim);
	        WarningBatch(WarningBuff);
		    goto Loop;
		}
		if(izsize != 3){
	    	if(izsize != 1 || itype != 257){
		    	sprintf(WarningBuff,"(%s) sgi file bad izsize is (%d)\n",FilesNames[n],izsize);
	        	WarningBatch(WarningBuff);
		    	goto Loop;
		    }
		}
	    

		if(CheckRGBSpace(Files))goto ErrorOut;

		Files->RGBList[ImageCount].buffer=buffer;
		Files->RGBList[ImageCount].xsize=xsize;
		Files->RGBList[ImageCount].ysize=ysize;
		Files->RGBList[ImageCount].length=length;
		Files->RGBList[ImageCount].itype=itype;
		Files->RGBList[ImageCount].izsize=izsize;
		
		buffer=NULL;
		Files->ImageCount = ++ImageCount;
		
		if(xsize > xmax)xmax=xsize;
		if(ysize > ymax)ymax=ysize;
		if(xsize < xmin)xmin=xsize;
		if(ysize < ymin)ymin=ysize;
		ok=TRUE;
Loop:
	    if(in)fclose(in); 
	    in=NULL;

	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
    	    if(count < n){
    	        if(FilesNames[count])cFree((char *)FilesNames[count]);
        		Files->FilesNames[count++]=FilesNames[n];
        		FilesNames[n]=NULL;
    	    }else{
        		Files->FilesNames[count++]=FilesNames[n];
    	    }
	    }else{
	       ++iskip;
	       cFree((char *)FilesNames[n]);
	       FilesNames[n]=NULL;
	    }
	}
	Files->FileCount=count;
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	Files->length=ymax*xmax;
	Files->FileOpen = -1;

	if(count <= 0)goto ErrorOut;

	{
	    unsigned long length;

	    length=3*xmax*ymax*sizeof(char);
	    Files->dataSave=(unsigned char *)cMalloc(length,8122);
	    if(Files->dataSave == NULL){
	        WarningBatch("initRGB Out Of Memory\n");
	        goto ErrorOut;
	    }
	}

	if(iskip){
	    sprintf(WarningBuff,"%ld Files Skipped\n",iskip);
	    WarningBatch(WarningBuff);
	}
	do24BitStart(Files);
	return 0;
ErrorOut:
	if(buffer)cFree((char *)buffer);
	buffer=NULL;
	if(in)fclose(in);
	freeFileList(Files);
	return 1;
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
int FilesInfoRGB(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files)
{
	if(!ImageName || !xsize || !ysize || !Length || !Files)return 1;

	if(Files->fileClass != RGB_FILES)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	*xsize=Files->RGBList[CurrentFrame].xsize;
	*ysize=Files->RGBList[CurrentFrame].ysize;
	*Length=Files->RGBList[CurrentFrame].length;
	*ImageName=FilesImageName(Files,CurrentFrame);
	return 0;
}
int FilesSetFrameRGB(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	unsigned char *bp,*buffd,*dataSave;
	unsigned long length;
	int itype,xsize,ysize,izsize;
	int k,j;

	if(!Files)return 1;
	if(Files->fileClass != RGB_FILES)return 1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	s = &Files->SetData;
	s->SetFrame= -1;
	
	s->name=FilesImageName(Files,CurrentFrame);
	s->xsize=xsize=Files->RGBList[CurrentFrame].xsize;
	s->ysize=ysize=Files->RGBList[CurrentFrame].ysize;
	length=Files->RGBList[CurrentFrame].length;
	s->length=(long)length;
	itype=Files->RGBList[CurrentFrame].itype;
	izsize=Files->RGBList[CurrentFrame].izsize;
	
	if(Files->loadToMemory){
		bp=(unsigned char *)Files->RGBList[CurrentFrame].buffer;
	}else{
        char name[1024];

        if(Files->directory[0]){
            mstrncpy(name,Files->directory,1024);
        }else{
            name[0]=0;
        }
        if(Files->FilesNames && Files->FilesNames[CurrentFrame])
                 strncatToPath(name,Files->FilesNames[CurrentFrame],1024);
                 
        FixPathName(name);
        Files->in = fopen(name,"rb");
        if(Files->in == NULL){
	        sprintf(WarningBuff,"Could Not Open %s To Read\n",name);
            WarningBatch(WarningBuff);
            return 1;
        }
	       
	   
	   if(SetBuffers(Files->length*3)){
	       return 1;  
	   }
	   
	   if(getString((unsigned char *)xg.buffout,(long)length,Files->in))return 1;
	   
	   bp=(unsigned char *)xg.buffout;
	   
	   fclose(Files->in);
	   
	   Files->in=NULL;
	} 

	buffd=bp;
	dataSave=Files->dataSave;
	if(itype == 257){
	    for(k=0;k<ysize;++k){
	        for(j=0;j<3;++j){
	            unsigned long r1,c1,c2,c3,c4;
	            unsigned char *r,*rm;

	    		if(izsize == 1){
	            	r=buffd+512+k*4;
	    		}else{
	            	r=buffd+512+k*4+j*ysize*4;
	            }

	            c1=r[0];
	            c2=r[1];
	            c3=r[2];
	            c4=r[3];
                r1 = ((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);
                rm=bp+r1;
                if((long)(rm-bp) >= (long)length){
	        		sprintf(WarningBuff,"File %s Error %lx %lx k %d j %d\n",s->name,(long)(rm-bp),length,k,j);
            		WarningBatch(WarningBuff);
                    return 1;
                }
	            doexpand(rm,(unsigned char *)dataSave+(ysize-k-1)*xsize*3+j);
	       }
   
	    }
	}else if(itype == 1){
	    for(k=0;k<ysize;++k){
	        for(j=0;j<3;++j){
	            unsigned long r1;

                r1 = 512+k*xsize+j*xsize*ysize;
	            doput(bp+r1,(unsigned char *)dataSave+(ysize-k-1)*xsize*3+j,xsize);
	       }
	    }
	}

	s->SetFrame= CurrentFrame;
	s->type = TRUE_FILES;
	s->data = Files->dataSave;
	return 0;
}
int GetRGBImage(char *name,long *xsizei,long *ysizei,unsigned char **image)
{
	unsigned char *Image,*ImageBuff,*bp,*buffd;
	long length;
	FILE *in;
	int ret;
/* image header start */
    int	imagic;
    int itype;
    int idim;
    int xsize;
    int ysize;
    int izsize;
/* image header end */
	int k,j;
	
	in=NULL;
	Image=NULL;
	ImageBuff=NULL;
	ret = 1;
	if(!(in=fopen(name,"rb"))){
		sprintf(WarningBuff,"Error Opening (%s)\n",name);
		WarningBatch(WarningBuff);
		goto EndIt;
	}
	
    length=FileLength(in);
    if(length == -1L){
        sprintf(WarningBuff,"(%s) Error Looking For File Length\n",name);
        WarningBatch(WarningBuff);
        goto EndIt;
    }
    
    if(!(ImageBuff=(unsigned char *)cMalloc(length,8123))){
            sprintf(WarningBuff,"Out Of Memory Request (%ld)\n",length);
            WarningBatch(WarningBuff);
            goto EndIt;
	}
	if(getString(ImageBuff,length,in)){
        sprintf(WarningBuff,"(%s) Error Reading\n",name);
        WarningBatch(WarningBuff);
        goto EndIt;
	}

	bp=(unsigned char *)ImageBuff;

	imagic = (((unsigned short)bp[0]) << 8) + bp[1];
	itype =  (((unsigned short)bp[2]) << 8) + bp[3];
	idim =   (((unsigned short)bp[4]) << 8) + bp[5];
	xsize = (((unsigned short)bp[6]) << 8) + bp[7];
	ysize = (((unsigned short)bp[8]) << 8) + bp[9];
	izsize = (((unsigned short)bp[10]) << 8) + bp[11];
    
	if(imagic != 0732){
	    sprintf(WarningBuff,"(%s) is not a sgi file tag is (%d)\n",name,imagic);
        WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(itype != 257 && itype != 1){
	    sprintf(WarningBuff,"(%s) sgi file bad type is (%d)\n",name,itype);
        WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(idim != 3){
	    sprintf(WarningBuff,"(%s) sgi file bad idim is (%d)\n",name,idim);
        WarningBatch(WarningBuff);
	    goto EndIt;
	}
	if(izsize != 3){
	    if(izsize != 1 || itype != 257){
	    	sprintf(WarningBuff,"(%s) sgi file bad izsize is (%d)\n",name,izsize);
        	WarningBatch(WarningBuff);
	    	goto EndIt;
	    }
	}
    
	length=xsize*ysize*3;
    
	if(!(Image=(unsigned char *)cMalloc(length,8124))){
            sprintf(WarningBuff,"Out Of Memory Request (%ld)\n",length);
            WarningBatch(WarningBuff);
            goto EndIt;
	}

	buffd=bp;
	if(itype == 257){
	    for(k=0;k<ysize;++k){
	        for(j=0;j<3;++j){
	            unsigned long r1,c1,c2,c3,c4;
	            unsigned char *r,*rm;

	    		if(izsize == 1){
	            	r=buffd+512+k*4;
	    		}else{
	            	r=buffd+512+k*4+j*ysize*4;
	            }

	            c1=r[0];
	            c2=r[1];
	            c3=r[2];
	            c4=r[3];
                r1 = ((c1 << 24) | (c2 << 16) | (c3 << 8) | c4);
                rm=bp+r1;
                if(rm-bp >= length){
	        		sprintf(WarningBuff,"File %s Error %lx %lx k %d j %d\n",name,(long)(rm-bp),length,k,j);
            		WarningBatch(WarningBuff);
                    goto EndIt;
                }
	            doexpand(rm,(unsigned char *)Image+(ysize-k-1)*xsize*3+j);
	       }
   
	    }
	}else if(itype == 1){
	    for(k=0;k<ysize;++k){
	        for(j=0;j<3;++j){
	            unsigned long r1;

                r1 = 512+k*xsize+j*xsize*ysize;
	            doput(bp+r1,(unsigned char *)Image+(ysize-k-1)*xsize*3+j,xsize);
	       }
	    }
	}

	*xsizei=xsize;
	*ysizei=ysize;
	*image=Image;
	
	ret = 0;
EndIt:
	if(in)fclose(in);
	if(ImageBuff)cFree((char *)ImageBuff);
	ImageBuff=NULL;
	if(ret){
		if(Image)cFree((char *)Image);
		Image=NULL;
	}
	return ret;
}
static int doput(unsigned char *i,unsigned char *o,long n)
{
	long k;

	if(!i || !o)return 1;


	for(k=0;k<n;++k){
	    *o = *i++;
	    o += 3;
	}
	return 0;
}
static int doexpand(unsigned char *iptr,unsigned char *optr)
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

int CheckRGBSpace(struct FileInfo2 *Files)
{
	struct RGBI *RGBList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	RGBList=NULL;
	if(Files->RGBList){
	    RGBList=(struct RGBI *)cRealloc((char *)Files->RGBList,ImageMax*sizeof(struct RGBI),8262);
	    if(!RGBList){
	        WarningBatch("initRGB out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&RGBList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct RGBI));
	}else{
	    RGBList=(struct RGBI *)cMalloc(ImageMax*sizeof(struct RGBI),8125);
	    if(!RGBList){
	        WarningBatch("initRGB out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)RGBList,ImageMax*sizeof(struct RGBI));
	}


	Files->ImageMax=ImageMax;
	Files->RGBList=RGBList;

	return 0;
ErrorOut:
	if(RGBList){
	    Files->RGBList=RGBList;
	}
	return 1;
}

int TrueWriteMerge(char *Name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,FILE *out)
{
        unsigned int mask;
        unsigned int Esc1,Esc2;
        long UnCompressed,Compressed;

        mask=255;
        UnCompressed=xsize*ysize*3;

        if(Stuff(&Esc1,&Esc2,bline,bout,UnCompressed,&Compressed,mask)){
            return 1;
        }
	if(FileTrueWrite(DRt2,bout,xsize,ysize,UnCompressed,
	                 Compressed,Esc1,Esc2,out)) return 1;

        if(putString((unsigned char *)Name,256L,out)) return 1;
        return 0;
}
int TrueMergedDone(FILE *out)
{
	if(!out)return 1;

	putString((unsigned char *)"ENDF",4L,out);
	fclose(out);
	return 0;
}
struct FileInfo2 *FilesTRUE(void)
{
        struct FileInfo2 *Files;

	Files=GetFileInfoStruct(8420);
	if(!Files){
	    sprintf(WarningBuff,"FilesTRUE out of Memory\n");
		WarningBatch(WarningBuff);
	    return (struct FileInfo2 *)NULL;
	}

	Files->fileClass=TRUE_FILES;
	Files->pSetFrame=FilesSetFrameTRUE;

	Files->pdefault=FilesOpenHDF;
	Files->pSaveOne=FilesSaveTRUE;
	Files->pSaveOneMerged=FilesMergedTRUE;
	Files->pGetInfo=FilesInfoTRUE;


	Files->loadToMemory=TRUE;
	Files->fileType=TRUE_MEMORY;
	
	return Files;
}
int FilesSaveTRUEToMemory(char *name,unsigned char *bline,
			  unsigned char *bout,long xsize,
			  long ysize,struct FileInfo2 *Files)
{
	unsigned char *buffer;
    unsigned int mask;
    unsigned int Esc1,Esc2;
    long lin,lout;
	long ImageCount;
	long xmax,ymax;
	int ret;

	if(CheckTrueSpace(Files))return 1;
	
	ret=1;

    mask=255;
    lin=xsize*ysize*3;

    if(Stuff(&Esc1,&Esc2,bline,bout,lin,&lout,mask)){
        return 1;
    }
	
	buffer=(unsigned char *)cMalloc(lout,8426);
	if(!buffer){
		WarningBatch("FilesSaveTRUEToMemory Out Of Memory\n");
	    goto ErrorOut;
	}
	memcpy(buffer,bout,lout);  

	ImageCount=Files->ImageCount;

	Files->TRUEList[ImageCount].Offset=0;
	Files->TRUEList[ImageCount].buffer=buffer;
	Files->TRUEList[ImageCount].Compressed=lout;
	Files->TRUEList[ImageCount].UnCompressed=lin;
	Files->TRUEList[ImageCount].xsize=(int)xsize;
	Files->TRUEList[ImageCount].ysize=(int)ysize;
	Files->TRUEList[ImageCount].Esc1=Esc1;
	Files->TRUEList[ImageCount].Esc2=Esc2;
	Files->ImageFiles[ImageCount]=0;
	Files->ImageNames[ImageCount]=strsave(name,8810);
    if(!Files->ImageNames[ImageCount]){
		WarningBatch("FilesSaveTRUEToMemory Out Of Memory\n");
	    goto ErrorOut;
    }
	Files->ImageCount = ++ImageCount;

	xmax=Files->xmax;
	ymax=Files->ymax;
	if(xsize < Files->xmin)Files->xmin=xsize;
	if(xsize > Files->xmax)Files->xmax=xsize;
	if(ysize < Files->ymin)Files->ymin=ysize;
	if(ysize > Files->ymax)Files->ymax=ysize;
	if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
	    if(Files->dataSave)cFree((char *)Files->dataSave);
	    Files->length=(long)(Files->ymax*Files->xmax*sizeof(char)*3);
	    Files->dataSave=(unsigned char *)cMalloc(Files->length,8427);
	    if(Files->dataSave == NULL){
			WarningBatch("FilesSaveTRUEToMemory Out Of Memory\n");
	    	goto ErrorOut;
	    }
	    Files->CurrentFrame = -1;
	}
	
	if(ImageCount == 1)do24BitStart(Files);
	
	ret = 0;
ErrorOut:

    return ret;
}

int initTRUE(struct FileInfo2 *Files)
{
    long n,k,count,ImageCount,ImageFirst;
    char buff[256];
	char filepath[2048];
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
    long iskip;
	FILE *in;
	long imageskip;
	long ImageIncrement;
	unsigned char *buffer;

	if(!Files)return 1;

	in=NULL;
	buffer=NULL;
	Files->fileClass=TRUE_FILES;
	Files->pdefault=FilesOpenHDF;
	Files->pSaveOne=FilesSaveTRUE;
	Files->pSaveOneMerged=FilesMergedTRUE;
	Files->pSetFrame=FilesSetFrameTRUE;
	Files->pGetInfo=FilesInfoTRUE;
	FilesNames=Files->FilesNames;

	if(Files->loadToMemory){
	    Files->fileType=TRUE_MEMORY;
	}else{
	    Files->fileType=TRUE_DISK;
	}
	
	xmin=ymin=200000;
	xmax=ymax=0;
	count=0;
	ImageCount=0;
	iskip=0;
	imageskip=0;
    ImageIncrement=Files->ImageIncrement;
    if(ImageIncrement <= 0)ImageIncrement=1;
	for(n=0;n<Files->FileCount;++n){
	    int ok,type;


	    ok=FALSE;
	    buffer=NULL;
	    ImageFirst=ImageCount;

	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    	    
	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }

	    if(getString((unsigned char *)buff,4L,in))goto Loop;

	    buff[4]=0;

	    if(!strcmp(buff,DRt1)){
	        type=TRUE_DISK;
	    }else if(!strcmp(buff,DRt2)){
	        type=TRUE_DISK_MERGED;
	    }else{
	        sprintf(WarningBuff,"File %s is Not A TrueColor File (%s)(%s)(%s)\n",
	                FilesNames[n],buff,DRt1,DRt2);
	       WarningBatch(WarningBuff);
	        goto Loop;
	    }

	    while(1){
	        char name[256];
	        int xsize,ysize;
	        unsigned int Esc1,Esc2;
	        long lin,lout;
	        long offset;

			if(CheckTrueSpace(Files))goto ErrorOut;

			if(getInt(&xsize,in))goto ErrorOut;
			if(getInt(&ysize,in))goto ErrorOut;

			if(getInt((int *)&Esc1,in))goto ErrorOut;
			if(getInt((int *)&Esc2,in))goto Loop;

			if(getLong(&lin,in))goto ErrorOut;
			if(getLong(&lout,in))goto ErrorOut;

			offset=ftell(in);

			if(++imageskip < ImageIncrement){
	        	if(type == TRUE_DISK_MERGED){
			        fseek(in,offset+lout,0); 
			    	if(getString((unsigned char *)name,256L,in))goto ErrorOut;		
				    goto GetMerge;
	        	}
				goto Loop;
			}
			imageskip=0;
			
			buffer=NULL;
			if(Files->loadToMemory){
			    buffer=(unsigned char *)cMalloc(lout,8126);
			    if(!buffer)goto ErrorOut;
			    if(getString((unsigned char *)buffer,lout,in))goto ErrorOut;
			}else{
			    fseek(in,offset+lout,0); 
			}
			Files->TRUEList[ImageCount].Offset=offset;
			Files->TRUEList[ImageCount].buffer=buffer;
			Files->TRUEList[ImageCount].Compressed=lout;
			Files->TRUEList[ImageCount].UnCompressed=lin;
			Files->TRUEList[ImageCount].xsize=xsize;
			Files->TRUEList[ImageCount].ysize=ysize;
			Files->TRUEList[ImageCount].Esc1=Esc1;
			Files->TRUEList[ImageCount].Esc2=Esc2;
			Files->ImageFiles[ImageCount]=n;
			buffer=NULL;
			Files->ImageCount = ++ImageCount;
			if(xsize > xmax)xmax=xsize;
			if(ysize > ymax)ymax=ysize;
			if(xsize < xmin)xmin=xsize;
			if(ysize < ymin)ymin=ysize;

	        if(type == TRUE_DISK_MERGED){
			    if(getString((unsigned char *)name,256L,in))goto ErrorOut;		
			    Files->ImageNames[ImageCount-1]=strsave(name,8235);
			    if(Files->ImageNames[ImageCount-1] == NULL)goto ErrorOut;
GetMerge:
			    if(getString((unsigned char *)buff,4L,in))goto ErrorOut;
			    buff[4]=0;
			    if(!strcmp(buff,(char *)DRt2))continue;
	        }
			break;
	        

	    }
	    ok=1;
Loop:
	    for(k=ImageFirst;k<ImageCount;++k){
	        char name[256];

	        if(!Files->ImageNames[k]){
	            if(ImageCount-ImageFirst > 1){
	                sprintf(name,"%s.%ld%c",FilesNames[n],k,0);
	            }else{
	                sprintf(name,"%s%c",FilesNames[n],0);
	            }
		    	Files->ImageNames[k]=strsave(name,8236);
			    if(Files->ImageNames[k] == NULL)goto ErrorOut;
	        }
	        ok=1;
	    }

	    if(in)fclose(in); 
	    in=NULL;

	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
    	    if(count < n){
    	        if(FilesNames[count])cFree((char *)FilesNames[count]);
        		Files->FilesNames[count++]=FilesNames[n];
        		FilesNames[n]=NULL;
    	    }else{
        		Files->FilesNames[count++]=FilesNames[n];
    	    }
	    }else{
	    	++iskip;
	        cFree((char *)FilesNames[n]);
	        FilesNames[n]=NULL;
	    }
	}
	Files->FileCount=count;
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	Files->length=ymax*xmax;
	Files->FileOpen = -1;

	if(count <= 0)goto ErrorOut;

	{
	    unsigned long length;

	    length=3*xmax*ymax*sizeof(char);
	    Files->dataSave=(unsigned char *)cMalloc(length,8127);
	    if(Files->dataSave == NULL){
	        goto ErrorOut;
	    }
	}

	if(iskip){
	    sprintf(WarningBuff,"%ld Images Skipped\n",iskip);
	    WarningBatch(WarningBuff);
	}

	do24BitStart(Files);
	
	return 0;
ErrorOut:
	if(buffer)cFree((char *)buffer);
	if(in)fclose(in);
	freeFileList(Files);
	WarningBatch("initTRUE Errored Trying To Load Files\n");
	return 1;
}
int CheckTrueSpace(struct FileInfo2 *Files)
{
	struct TRUEI *TRUEList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	TRUEList=NULL;
	if(Files->TRUEList){
	    TRUEList=(struct TRUEI *)cRealloc((char *)Files->TRUEList,ImageMax*sizeof(struct TRUEI),8264);
	    if(!TRUEList){
	        WarningBatch("initTRUE out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&TRUEList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct TRUEI));
	}else{
	    TRUEList=(struct TRUEI *)cMalloc(ImageMax*sizeof(struct TRUEI),8128);
	    if(!TRUEList){
	        WarningBatch("initTRUE out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)TRUEList,ImageMax*sizeof(struct TRUEI));
	}


	Files->ImageMax=ImageMax;
	Files->TRUEList=TRUEList;

	return 0;
ErrorOut:
	if(TRUEList){
	    Files->TRUEList=TRUEList;
	}
	return 1;
}
int FilesSetFrameTRUE(long n,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	long Compressed;
	unsigned int Esc1,Esc2;

	if(!Files)return 1;
	s = &Files->SetData;
	s->SetFrame= -1;
	
	if(n >= Files->ImageCount)n=Files->ImageCount-1;
	if(n < 0)n=0;

	if(FileTrueInfo(&s->name,&s->xsize,&s->ysize,&s->length,
	                &Compressed,&Esc1,&Esc2,n,Files))return 1;

	if(FilesGetTRUE(&Files->dataSave,s->length,n,
	               TRUE_EXPANDED,Files))return 1;

	s->SetFrame= n;
	s->type = TRUE_FILES;
	s->data = Files->dataSave;
	return 0;
}
int FilesSaveTRUE(char *Name,long CurrentFrame,struct FileInfo2 *Files)
{
	char path[4096];
	long UnCompressed,Compressed;
	unsigned int Esc1,Esc2;
	long xsize,ysize;
	char *ImageName;
	unsigned char *buffer;
	long length;
	int ret;

	if(FileTrueInfo(&ImageName,&xsize,&ysize,&UnCompressed,
	                &Compressed,&Esc1,&Esc2,CurrentFrame,Files))return 1;

	length=0;
	if(FilesGetTRUE(&buffer,length,CurrentFrame,
	               TRUE_COMRESSED_POINTER,Files))return 1;

	mstrncpy(path,Files->SaveDirectory,4096);
	strncatToPath(path,Name,4096);
	Files->out=fopen(path,"wb");
	if(Files->out == NULL){
	   sprintf(WarningBuff,"FilesSaveTRUE Could Not Open %s to write\n",path);
	    WarningBatch(WarningBuff);
	   return 1;
	}
	ret=1;
	if(FileTrueWrite(DRt1,buffer,xsize,ysize,UnCompressed,
	                 Compressed,Esc1,Esc2,Files->out))goto ErrorOut;
	ret=0;
ErrorOut:
	if(Files->out){
	    fclose(Files->out);
	    Files->out=NULL;
	}
	return ret;
}

int FilesMergedTRUE(char *filename,char *Name,
                    long CurrentFrame,struct FileInfo2 *Files)
{
	char path[4096];
	long UnCompressed,Compressed;
	unsigned int Esc1,Esc2;
	long xsize,ysize;
	char *ImageName;
	unsigned char *buffer;
	long length;
	int ret;

	if(FileTrueInfo(&ImageName,&xsize,&ysize,&UnCompressed,
	                &Compressed,&Esc1,&Esc2,CurrentFrame,Files))return 1;

	length=0;
	if(FilesGetTRUE(&buffer,length,CurrentFrame,
	               TRUE_COMRESSED_POINTER,Files))return 1;

	mstrncpy(path,Files->SaveDirectory,4096);
	strncatToPath(path,filename,4096);
	if(!Files->out){
	    Files->out=fopen(path,"wb");
	    if(Files->out == NULL){
	       	sprintf(WarningBuff,"FilesMergedTRUE Could Not Open %s to write\n",path);
	    	WarningBatch(WarningBuff);
	      	return 1;
	    }
	}
	ret=1;
	if(FileTrueWrite(DRt2,buffer,xsize,ysize,UnCompressed,
	                 Compressed,Esc1,Esc2,Files->out))goto ErrorOut;

        if(putString((unsigned char *)Name,256L,Files->out))goto ErrorOut;

	ret=0;
ErrorOut:
	return ret;
}
int FilesInfoTRUE(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files)
{
	unsigned int Esc;
	long Compressed;

	return FileTrueInfo(ImageName,xsize,ysize,
	                    Length,&Compressed,&Esc,&Esc,
	                    CurrentFrame,Files);
}
int FileTrueInfo(char **ImageName,long *xsize,long *ysize,
	long *UnCompressed,long *Compressed,
	unsigned int *Esc1, unsigned int *Esc2,
	long CurrentFrame,struct FileInfo2 *Files)
{
	if(!ImageName || !xsize || !ysize || !UnCompressed || !Files ||
	   !Compressed || !Esc1 || !Esc2)return 1;
	if(Files->fileClass != TRUE_FILES)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;

	*xsize=Files->TRUEList[CurrentFrame].xsize;
	*ysize=Files->TRUEList[CurrentFrame].ysize;
	*Compressed=Files->TRUEList[CurrentFrame].Compressed;
	*UnCompressed=Files->TRUEList[CurrentFrame].UnCompressed;
	*Esc1=Files->TRUEList[CurrentFrame].Esc1;
	*Esc2=Files->TRUEList[CurrentFrame].Esc2;
	*ImageName=FilesImageName(Files,CurrentFrame);
	return 0;
}

int FilesGetTRUE(unsigned char **buffer,long length,
                long CurrentFrame,int How,struct FileInfo2 *Files)
{
	long xsize,ysize,xylength;
	unsigned char *bp;
	struct TRUEI *TList;
	int n;

	if(!buffer || !Files)return 1;

	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != TRUE_FILES || !Files->TRUEList)return 1;
	TList = &Files->TRUEList[CurrentFrame];
	if(!TList)return 1;

	xsize=TList->xsize;
	ysize=TList->ysize;

	xylength=xsize*ysize*3;

	if(Files->loadToMemory){
	    bp=TList->buffer;
	}else{
	   if(SetFileToRead(CurrentFrame,Files))return 1;
	   fseek(Files->in,TList->Offset,0); 
	   if(SetBuffers(TList->Compressed)){
	       return 1;  
	   }
	   if(getString((unsigned char *)xg.buffout,TList->Compressed,Files->in))return 1;
	   bp=(unsigned char *)xg.buffout;
	}

	if(How == TRUE_EXPANDED){

	   if(xylength > length)return 1;

	    return UnCompress(TList->Esc1,TList->Esc2,
	                      TList->UnCompressed,TList->Compressed,
	                      bp,*buffer);
	}else if(How == TRUE_COMRESSED){
	    unsigned char *bo;

	   if(xylength > length)return 1;

	    bo = *buffer;
	    for(n=0;n<TList->Compressed;++n){
	        bo[n]=bp[n];
	    }
	} else if(TRUE_COMRESSED_POINTER){
	    *buffer=bp;
	}
	return 0;
}
int initTEK(struct FileInfo2 *Files)
{
        long n,k,count,length,ImageCount,lengthM;
        char **FilesNames;
		char filepath[2048];
		FILE *in;
		int itis;

		if(!Files)return 1;

		in=NULL;

		Files->fileClass=TEK_FILES;
		Files->pdefault=FilesOpenTek;
	/*
	Files->pSaveOne=FilesSaveTRUE;
	Files->pSaveOneMerged=FilesMergedTRUE;
	*/

		FilesNames=Files->FilesNames;
		
		if(Files->loadToMemory){
		    Files->fileType=TEK_MEMORY;
		}else{
		    Files->fileType=TEK_DISK;
		}
			
		count=0;
		ImageCount=0;
		lengthM=0;
		for(n=0;n<Files->FileCount;++n){
	    	unsigned char *buffer;
	    	unsigned char buff[256];
	    	long len;
	    	int ok;


	    	ok=FALSE;
	    	buffer=NULL;
	    	
	   		mstrncpy(filepath,Files->directory,2048);
	    	strncatToPath(filepath,FilesNames[n],2048);

		    in=fopen(filepath,"rb");
		    if(in == NULL){
		       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
		       WarningBatch(WarningBuff);
		       goto Loop;
		    }
		    
		    if(fseek(in,0L,2)){
		        sprintf(WarningBuff,"(%s) Error Looking For End Of File\n",filepath);
		        WarningBatch(WarningBuff);
		        goto Loop;
		    }

		    if((length=ftell(in)) == -1L){
		        sprintf(WarningBuff,"(%s) Error Reading For End Of File Location\n",filepath);
		        WarningBatch(WarningBuff);
		        goto Loop;
		    }
	    	
	    	if(fseek(in,0L,0)){
	        	sprintf(WarningBuff,"(%s) Error Rewinding File\n",filepath);
       			WarningBatch(WarningBuff);
	        	goto ErrorOut;
	    	}
	    	
	    	len=256;
	     	if(length < len)len=length;
	     	
	     	if(getString((unsigned char *)buff,len,in))goto ErrorOut;

	     	
	    	itis=FALSE;
	    	for(k=0;k<len;++k){
	        	int c;
	        	c=buff[k];
	        	if(c == 27 || c == 29){
		    	   itis=TRUE;
		    	   break;
	        	}
	    	}
	    	if(!itis){
	        	sprintf(WarningBuff,"File %s Skipped Not A TEK File\n",filepath);
	        	WarningBatch(WarningBuff);
	        	goto ErrorOut;
	    	}


	    	if(length > lengthM)lengthM=length;

	    	if(CheckTekSpace(Files))goto ErrorOut;
	    	
			buffer=NULL;
			if(Files->loadToMemory){
		    	if(fseek(in,0L,0)){
		        	sprintf(WarningBuff,"(%s) Error Rewinding File\n",filepath);
	       			WarningBatch(WarningBuff);
		        	goto ErrorOut;
		    	}
		    	buffer=(unsigned char *)cMalloc(length,8135);
		    	if(!buffer)goto ErrorOut;
		    	if(getString(buffer,length,in))goto ErrorOut;
			}

			Files->TEKList[ImageCount].buffer=buffer;
			Files->TEKList[ImageCount].length=length;
			Files->ImageNames[ImageCount]=strsave(FilesNames[n],8239);
		    if(!Files->ImageNames[ImageCount]){
       			WarningBatch("initTEK Out Of Memory\n");
		    	goto ErrorOut;
		    }
			Files->ImageFiles[ImageCount]=n;
			buffer=NULL;
			Files->ImageCount = ++ImageCount;
	    	ok=1;
Loop:
		    if(in)fclose(in); 
		    in=NULL;

	    	if(buffer)cFree((char *)buffer);
	    	buffer=NULL;

	    	if(ok){
	    	    if(count < n){
	    	        if(FilesNames[count])cFree((char *)FilesNames[count]);
	        		Files->FilesNames[count++]=FilesNames[n];
	        		FilesNames[n]=NULL;
	    	    }else{
	        		Files->FilesNames[count++]=FilesNames[n];
	    	    }
	    	}else{
	        	sprintf(WarningBuff,"Skipping file %s\n",filepath);
	       		WarningBatch(WarningBuff);
	        	cFree((char *)FilesNames[n]);
	        	FilesNames[n]=NULL;
	    	}
		}
		Files->FileCount=count;
		Files->FileOpen = -1;

		Files->length=lengthM;

		if(count <= 0)goto ErrorOut;
		
		if(!Files->loadToMemory){
			Files->dataSave=(unsigned char *)cMalloc(lengthM,8136);
			if(!Files->dataSave)goto ErrorOut;
		}

		return 0;
ErrorOut:

		if(in)fclose(in);
		freeFileList(Files);		
		return 1;
}
int CheckTekSpace(struct FileInfo2 *Files)
{
		struct TEKI *TEKList;
        long ImageMax;
	
		if(!Files)return 1;

		if(Files->ImageCount+1 < Files->ImageMax)return 0;

		if(CheckNames(Files))return 1;

		ImageMax = Files->ImageMax+IMAGE_INCREMENT;

		TEKList=NULL;
		if(Files->TEKList){
	    	TEKList=(struct TEKI *)cRealloc((char *)Files->TEKList,ImageMax*sizeof(struct TEKI),8265);
	    	if(!TEKList){
	        	WarningBatch("CheckTekSpace out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)&TEKList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct TEKI));
		}else{
	    	TEKList=(struct TEKI *)cMalloc(ImageMax*sizeof(struct TEKI),8138);
	    	if(!TEKList){
	        	WarningBatch("CheckTekSpace out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)TEKList,ImageMax*sizeof(struct TEKI));
		}

		Files->ImageMax=ImageMax;
		Files->TEKList=TEKList;

		return 0;
ErrorOut:
		if(TEKList){
	    	Files->TEKList=TEKList;
		}
		return 1;
}
struct FileInfo2 *FilesTEK(void)
{
        struct FileInfo2 *Files;

	Files=GetFileInfoStruct(8142);
	if(!Files){
	    WarningBatch("FilesTEK out of Memory\n");
	    return (struct FileInfo2 *)NULL;
	}

	Files->fileClass=TEK_FILES;

	Files->fileType = TEK_MEMORY;
	
	Files->loadToMemory=1;
	
	Files->length=0;
	
	Files->FileTagCount=FileTagCount++;

	return Files;
}
int FilesSaveTEKToMemory(char *name,unsigned char *buffer,long length,struct FileInfo2 *Files)
{
		long count;

		if(CheckTekSpace(Files))return 1;

		count=Files->ImageCount;
	
		Files->TEKList[count].buffer=buffer;
		Files->TEKList[count].length=length;
		Files->ImageFiles[count]=count;
		Files->ImageNames[count]=strsave(name,8240);
		if(!Files->ImageNames[count]){
		    WarningBatch("FilesSaveTEKToMemory Out Of Memory\n");
		    return 1;
		}
	    Files->FileCount = ++count;
		Files->ImageCount = count;
        return 0;
}
int FilesGetTEK(char **ImageName,long *teklength,char **buffer,
                long CurrentFrame,struct FileInfo2 *Files)
{
	int ret;
	if(!ImageName || !buffer || !Files)return 1;

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	if(Files->fileClass != TEK_FILES)return 1;

	*teklength=Files->TEKList[CurrentFrame].length;

	if(Files->loadToMemory){
		*buffer=(char *)Files->TEKList[CurrentFrame].buffer;
	}else{
	    if(!Files->in){
	        char name[1024];

			if(CurrentFrame < 0 || CurrentFrame >= Files->FileCount){
			    sprintf((char *)WarningBuff,"Error CurrentFrame %ld out of Range FileCount %ld\n",
			            CurrentFrame,Files->FileCount);
	        	WarningBatch((char *)WarningBuff);
			    return 1;
			}
			
	        if(Files->directory[0]){
	            mstrncpy(name,Files->directory,1024);
	            
        		if(Files->FilesNames && Files->FilesNames[CurrentFrame])
	        	    strncatToPath(name,Files->FilesNames[CurrentFrame],1024);
	        }else{
        		if(Files->FilesNames && Files->FilesNames[CurrentFrame])
	        	    mstrncpy(name,Files->FilesNames[CurrentFrame],1024);
	        }			
			
	        Files->in = fopen(name,"rb");
	        if(Files->in == NULL){
		    	sprintf((char *)WarningBuff,"Could Not Open %s To Read\n",name);
	        	WarningBatch((char *)WarningBuff);
	            return 1;
	        }
	       
	    }
	   ret=getString((unsigned char *)Files->dataSave,Files->TEKList[CurrentFrame].length,Files->in);
	   fclose(Files->in);
	   Files->in=NULL;
	   if(ret)return 1;
	   *buffer=(char *)Files->dataSave;

	}

	*ImageName=FilesImageName(Files,CurrentFrame);
	
	return 0;
	
}
int initBMP(struct FileInfo2 *Files)
{
	char filepath[2048];
	unsigned char *image;
	unsigned char *buffer;
    long n,count,ImageCount;
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
    long iskip;
	long xsize;
	long ysize;
    long length,lout;
    unsigned int Esc1,Esc2;

	if(!Files)return 1;

	Files->fileClass=BMP_FILES;
	Files->pSetFrame=FilesSetFrameBMP;
	
	Files->pdefault=FilesOpenHDF;
	Files->pGetInfo=FilesInfoBMP;
	
	FilesNames=Files->FilesNames;
	
	if(Files->loadToMemory){
	    Files->fileType=BMP_MEMORY;
	}else{
	    Files->fileType=BMP_DISK;
	}	
	
	count=0;
	ImageCount=0;
	xmax=ymax=0;
	xmin=ymin=200000;
	iskip=0;
	
	    buffer=NULL;
	    image=NULL;
	for(n=0;n<Files->FileCount;++n){
	    int ok;


	    ok=FALSE;

		Esc1=0;
		Esc2=0;
		lout=0;
	    image=NULL;
	    buffer=NULL;
	    
	    
	    
	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);

	    if(GetBMPImage(filepath,&xsize,&ysize,&image)){
	       sprintf(WarningBuff,"Could Not Load BMP %s\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }
	    
	    length=xsize*ysize*3;
	    
	    if(Files->loadToMemory){

	        if(SetBuffers(length))goto ErrorOut;

		    if(Stuff3(&Esc1,&Esc2,image,(unsigned char *)xg.buffin,length,&lout,255)){
			    goto ErrorOut;
		    }

	
		    buffer=(unsigned char *)cMalloc(lout,8443);
		    if(!buffer)goto ErrorOut;

		    uMemcpy(buffer,xg.buffin,lout);  

	    }

	    if(image)cFree((char *)image);
	    
		if(CheckBMPSpace(Files))goto ErrorOut;

		Files->BMPList[ImageCount].Esc1=Esc1;
		Files->BMPList[ImageCount].Esc2=Esc2;
		Files->BMPList[ImageCount].Compressed=lout;
		Files->BMPList[ImageCount].UnCompressed=length;
		Files->BMPList[ImageCount].buffer=buffer;
		Files->BMPList[ImageCount].xsize=(int)xsize;
		Files->BMPList[ImageCount].ysize=(int)ysize;
		Files->BMPList[ImageCount].length=length;
		
		buffer=NULL;
		Files->ImageCount = ++ImageCount;
		
		if(xsize > xmax)xmax=xsize;
		if(ysize > ymax)ymax=ysize;
		if(xsize < xmin)xmin=xsize;
		if(ysize < ymin)ymin=ysize;
		ok=TRUE;
Loop:
	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
	        if(count < n){
	            if(FilesNames[count])cFree((char *)FilesNames[count]);
				Files->FilesNames[count++]=FilesNames[n];
				FilesNames[n]=NULL;
	        }else{
				Files->FilesNames[count++]=FilesNames[n];
	        }
	    }else{
	       ++iskip;
	       cFree((char *)FilesNames[n]);
	       FilesNames[n]=NULL;
	    }
	}
	Files->FileCount=count;
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	Files->length=ymax*xmax;
	Files->FileOpen = -1;

	if(count <= 0)goto ErrorOut;

	{

	    length=(long)(3*xmax*ymax*sizeof(char));
	    Files->dataSave=(unsigned char *)cMalloc(length,8444);
	    if(Files->dataSave == NULL){
	        WarningBatch("initBMP Out Of Memory\n");
	        goto ErrorOut;
	    }
	}

	if(iskip){
	    sprintf(WarningBuff,"%ld Files Skipped\n",iskip);
	    WarningBatch(WarningBuff);
	}

	do24BitStart(Files);
	
	return 0;
ErrorOut:
	if(buffer)cFree((char *)buffer);
	buffer=NULL;
	freeFileList(Files);
	return 1;
}
int FilesInfoBMP(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files)
{
	if(!ImageName || !xsize || !ysize || !Length || !Files)return 1;

	if(Files->fileClass != BMP_FILES)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	*xsize=Files->BMPList[CurrentFrame].xsize;
	*ysize=Files->BMPList[CurrentFrame].ysize;
	*Length=Files->BMPList[CurrentFrame].length;
	*ImageName=FilesImageName(Files,CurrentFrame);
	return 0;
}
int FilesSetFrameBMP(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	unsigned char *bp,*image,*dataSave;
	long xsize,ysize,length;

	if(!Files)return 1;
	if(Files->fileClass != BMP_FILES)return 1;
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	s = &Files->SetData;
	s->SetFrame= -1;
	
	s->name=FilesImageName(Files,CurrentFrame);
	s->xsize=xsize=Files->BMPList[CurrentFrame].xsize;
	s->ysize=ysize=Files->BMPList[CurrentFrame].ysize;
	s->length=length=Files->BMPList[CurrentFrame].length;
	
	dataSave=Files->dataSave;

	if(Files->loadToMemory){
	    bp=(unsigned char *)Files->BMPList[CurrentFrame].buffer;
	    if(UnCompress(Files->BMPList[CurrentFrame].Esc1,
	                      Files->BMPList[CurrentFrame].Esc2,
	                      Files->BMPList[CurrentFrame].UnCompressed,
	                      Files->BMPList[CurrentFrame].Compressed,
	                      bp,dataSave))return 1;
	}else{
            char name[1024];

            if(Files->directory[0]){
                mstrncpy(name,Files->directory,1024);
            }else{
                name[0]=0;
            }
        if(Files->FilesNames && Files->FilesNames[CurrentFrame])
               strncatToPath(name,Files->FilesNames[CurrentFrame],1024);
                              
        FixPathName(name);              
	    if(GetBMPImage(name,&xsize,&ysize,&image)){
	       return 1;
	    }
	    length=3*xsize*ysize;
	    uMemcpy(dataSave,image,length);  
	    if(image)cFree((char *)image);
	    s->xsize=xsize;
	    s->ysize=ysize;
	} 


	s->SetFrame= CurrentFrame;
	s->type = TRUE_FILES;
	s->data = Files->dataSave;
	return 0;
}
int CheckBMPSpace(struct FileInfo2 *Files)
{
	struct BMPI *BMPList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	BMPList=NULL;
	if(Files->BMPList){
	    BMPList=(struct BMPI *)cRealloc((char *)Files->BMPList,ImageMax*sizeof(struct BMPI),8268);
	    if(!BMPList){
	        WarningBatch("CheckBMPSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&BMPList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct BMPI));
	}else{
	    BMPList=(struct BMPI *)cMalloc(ImageMax*sizeof(struct BMPI),8445);
	    if(!BMPList){
	        WarningBatch("CheckBMPSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)BMPList,ImageMax*sizeof(struct BMPI));
	}


	Files->ImageMax=ImageMax;
	Files->BMPList=BMPList;

	return 0;
ErrorOut:
	if(BMPList){
	    Files->BMPList=BMPList;
	}
	return 1;
}
struct paldata{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char d;
};
int GetBMPImage(char *name,long *xsizer,long *ysizer,unsigned char **image)
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
	    sprintf(WarningBuff,"GetBMPImage Could Not Open File %s To Read\n",name);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	ret=1;

	FileSize=FileLength(in);
	if(FileSize <= 0)goto ErrorOut;

	/* File Header */
	
	if(getString((unsigned char *)BM,2L,in))goto ErrorOut;

	BM[2]=0;

	if(strcmp(BM,"BM")){
	    sprintf(WarningBuff,"%s is not a BMP file\n",name);
	    WarningBatch(WarningBuff);
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
	    sprintf(WarningBuff,"%s Compression Type %ld Not Converted\n",name,biCompression);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}


	nx=(int)((biWidth*biBitCount+7)/8);
	nx=4*((nx-1)/4+1);
	if(biBitCount <= 8){
	    int nc;
	    
	    nc=0;
	    if(biClrUsed == 0){
	        if(biBitCount == 1){
	            nc=2;
	        }else if(biBitCount == 4){
	            nc=16;
	        }else if(biBitCount == 8){
	            nc=256;
	        }
	    }else{
	        nc=(int)biClrUsed;
	    }
	    for(n=0;n<256;++n){
	        palin[n].r=255;
	        palin[n].g=255;
	        palin[n].b=255;
	        palin[n].d=255;
	    }
	    for(n=0;n<nc;++n){
	        unsigned char c[4];
	        if(getString(c,4L,in))goto ErrorOut;
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
	    if((long)biSizeImage > (long)FileSize)biSizeImage = FileSize;

	     if(getString((unsigned char *)xg.buffout,(long)biSizeImage,in))goto ErrorOut;
	     zerol((char *)xg.buffin,length);
	     i=j=0;
	     po = (unsigned char *)xg.buffin;
	     pi = (unsigned char *)xg.buffout;
	     for(kk=0;kk<(long)biSizeImage;){
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
	     if(getString((unsigned char *)xg.buffin,rlen,in))goto ErrorOut;
	}
	*image=(unsigned char *)cMalloc(length,8270);
	if(!(*image)){
	    sprintf(WarningBuff,"%s Out Of Memory\n",name);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	po = *image;
	pi = (unsigned char *)xg.buffin;
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
	    sprintf(WarningBuff,"%s Error Reading BMP file\n",name);
	    WarningBatch(WarningBuff);
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
int getIntLow(unsigned int *i,FILE *in)
{
      unsigned char c[2];
      unsigned int c0,c1;

        if(!i || !in)return 1;
        if(getString(c,2L,in))return 1;
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

        if(getString(c,4L,out))return 1;

        c0=c[0];
        c1=c[1];
        c2=c[2];
        c3=c[3];

	*n = c0 + (c1 << 8) + (c2 << 16) + (c3 << 24);
	return 0;
}
int Stuff3(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask)
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
            WarningBatch("Stuff Packing Error\n");
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
            while(c1 == dataIn[k+kk] && 
                  c2 == dataIn[k+kk+1] && 
                  c3 == dataIn[k+kk+2]){
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
int initGIF(struct FileInfo2 *Files)
{
	struct GIFdataStruct g;
	unsigned char *buffer;
	char filepath[2048];
	char name[256];
    long n,count,ImageCount;
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
    long xsize,ysize;
    long iskip;
	long iloop;
	FILE *in;
	int ch,k;

	if(!Files)return 1;

	in=NULL;
	Files->fileClass=GIF_FILES;
	Files->pSetFrame=FilesSetFrameGIF;

	Files->pdefault=FilesOpenHDF;
	Files->pGetInfo=FilesInfoGIF;

	FilesNames=Files->FilesNames;

	Files->loadToMemory = 1;
	
	count=0;
	ImageCount=0;
	xmax=ymax=0;
	xmin=ymin=200000;
	iskip=0;
	
	buffer=NULL;
	    
	for(n=0;n<Files->FileCount;++n){
	    long length;
	    int ok;


	    ok=FALSE;
	    buffer=NULL;

	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
	       WarningBatch(WarningBuff);
	       goto Loop;
	    }
	    
	    length=FileLength(in);
	    if(length == -1L){
	        sprintf(WarningBuff,"(%s) Error Looking For File Length\n",FilesNames[n]);
	        WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    
	    zerol((char *)&g,sizeof(g));

	    g.in=in;
	    g.name=FilesNames[n];

	    if(checksignature(in,&g))goto Loop;
	    if(readscreen(in,&g))goto ErrorOut;

	    iloop=0;
            while ((ch = getc(in)) != ';' && ch != EOF) {
                switch (ch) {
                    case '\0':  break; 
                    case ',':   
		  			if(CheckGIFSpace(Files))goto ErrorOut;
					if (readgifimage(in,&g,g.name))goto Loop;


					buffer=(unsigned char *)cMalloc(g.HdfSize,8446);
					if(!buffer)goto ErrorOut;
					uMemcpy(buffer,g.raster,g.HdfSize);
					xsize=(long)g.width;
					ysize=(long)g.height;
					length=xsize*ysize;
					Files->GIFList[ImageCount].buffer=buffer;
					Files->GIFList[ImageCount].xsize=(int)xsize;
					Files->GIFList[ImageCount].ysize=(int)ysize;
					Files->GIFList[ImageCount].length=length;
					Files->GIFList[ImageCount].HdfSize=g.HdfSize;
					if(iloop++ == 0){
				            Files->ImageNames[ImageCount]=strsave(FilesNames[n],8817);
				    }else{
			                sprintf(name,"%s.%ld%c",FilesNames[n],iloop-1,0);
				            Files->ImageNames[ImageCount]=strsave(name,8818);
				    }
				    if(Files->ImageNames[ImageCount] == NULL)goto ErrorOut;

					for(k=0;k<256;++k){
					   Files->GIFList[ImageCount].pal[3*k]=(unsigned char)g.red[k];
					   Files->GIFList[ImageCount].pal[3*k+1]=(unsigned char)g.green[k];
					   Files->GIFList[ImageCount].pal[3*k+2]=(unsigned char)g.blue[k];
					}

					buffer=NULL;
					Files->ImageCount = ++ImageCount;
				
					if(xsize > xmax)xmax=xsize;
					if(ysize > ymax)ymax=ysize;
					if(xsize < xmin)xmin=xsize;
					if(ysize < ymin)ymin=ysize;
                        break;
                    case '!':   readextension(in);
                        break;
                    default:    
                        sprintf(WarningBuff, "(%s) unknown GIF block type (%c) %d\n",g.name,ch,ch);
                        WarningBatch(WarningBuff);
                        goto Loop;
                }
            }


		ok=TRUE;
Loop:
	    if(in)fclose(in); 
	    in=NULL;

	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
    	    if(count < n){
    	        if(FilesNames[count])cFree((char *)FilesNames[count]);
        		Files->FilesNames[count++]=FilesNames[n];
        		FilesNames[n]=NULL;
    	    }else{
        		Files->FilesNames[count++]=FilesNames[n];
    	    }
	    }else{
	       ++iskip;
	       cFree((char *)FilesNames[n]);
	       FilesNames[n]=NULL;
	    }
	}
	Files->FileCount=count;
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	Files->length=ymax*xmax;
	Files->FileOpen = -1;

	if(count <= 0)goto ErrorOut;

	{
	    unsigned long length;

	    length=3*xmax*ymax*sizeof(char);
	    Files->dataSave=(unsigned char *)cMalloc(length,8447);
	    if(Files->dataSave == NULL){
	        WarningBatch("initGIF Out Of Memory\n");
	        goto ErrorOut;
	    }
	}

	if(iskip){
	    sprintf(WarningBuff,"%ld Files Skipped\n",iskip);
	    WarningBatch(WarningBuff);
	}

	do24BitStart(Files);
	
	return 0;
ErrorOut:
	if(buffer)cFree((char *)buffer);
	buffer=NULL;
	if(in)fclose(in);
	freeFileList(Files);
	return 1;
}
int FilesSetFrameGIF(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	unsigned char *bp,*dataSave,*pal;
	long length,n;
	int c;

	if(!Files)return 1;
	if(Files->fileClass != GIF_FILES)return 1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	s = &Files->SetData;
	s->SetFrame= -1;
	
	s->name=FilesImageName(Files,CurrentFrame);
	s->xsize=Files->GIFList[CurrentFrame].xsize;
	s->ysize=Files->GIFList[CurrentFrame].ysize;
	s->length=length=Files->GIFList[CurrentFrame].length;
	pal=Files->GIFList[CurrentFrame].pal;
	bp=(unsigned char *)Files->GIFList[CurrentFrame].buffer;
	dataSave=Files->dataSave;
	if(length <= 0)return 1;
	UnPack((unsigned char *)dataSave,(unsigned char *)bp,length);
	for(n=length-1;n>=0;--n){
	    c=dataSave[n];
	    dataSave[3*n]=pal[3*c];
	    dataSave[3*n+1]=pal[3*c+1];
	    dataSave[3*n+2]=pal[3*c+2];
	}

	s->SetFrame= CurrentFrame;
	s->type = TRUE_FILES;
	s->data = Files->dataSave;
	return 0;
}
int FilesInfoGIF(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files)
{
	if(!ImageName || !xsize || !ysize || !Length || !Files)return 1;

	if(Files->fileClass != GIF_FILES)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	*xsize=Files->GIFList[CurrentFrame].xsize;
	*ysize=Files->GIFList[CurrentFrame].ysize;
	*Length=Files->GIFList[CurrentFrame].length;
	*ImageName=FilesImageName(Files,CurrentFrame);
	return 0;
}
static int readextension(FILE *in)
{
    int count;
    char buf[255];

    (void) getc(in);
    while ((count = getc(in)) != 0){
        fread(buf, 1, count, in);
    }
    return 0;
}
static int rasterize(int interleaved, register unsigned char *raster,struct GIFdataStruct *gl)
{
    register int row,col;
    register unsigned char *rr;
    unsigned char *newras;
    long length;
    int c;


	length=(long)(gl->width*gl->height);

	newras = (unsigned char*)xg.buffin;

    if (interleaved) {
       {		
            for (row = 0; row < (int)gl->height; row += 8) {	
                rr = newras + row*gl->width;	
                for(col=0;col<(int)gl->width;++col){
                    c=raster[col];
                    rr[col]=c;
                }
                raster += gl->width;	
            }
       }								
       {		
            for (row = 4; row < (int)gl->height; row += 8) {	
                rr = newras + row*gl->width;	
                for(col=0;col<(int)gl->width;++col){
                    c=raster[col];
                    rr[col]=c;
                }
                raster += gl->width;	
            }
       }								

       {		
            for (row = 2; row < (int)gl->height; row += 4) {	
                rr = newras + row*gl->width;	
                for(col=0;col<(int)gl->width;++col){
                    c=raster[col];
                    rr[col]=c;
                }
                raster += gl->width;	
            }
       }								

       {		
            for (row = 1; row < (int)gl->height; row += 2) {	
                rr = newras + row*gl->width;	
                for(col=0;col<(int)gl->width;++col){
                    c=raster[col];
                    rr[col]=c;
                }
                raster += gl->width;	
            }
       }								

    }else{
        for (row = 0; row < (int)gl->height; row += 1) {
            rr = newras + row*gl->width;	
            for(col=0;col<(int)gl->width;++col){
                c=raster[col];
                rr[col]=c;
            }
            raster += gl->width;
        }


    }

    if(HdfPack((unsigned char *)xg.buffin,(unsigned char *)xg.buffout,length,&gl->HdfSize))return 1;

/*

	{
	    char *Type,pal[256*3];
	    long size,ni;

            for(ni=0;ni<256;++ni){
 
                pal[3*ni]=g->red[ni];
                pal[3*ni+1]=g->green[ni];
                pal[3*ni+2]=g->blue[ni];
            }

	    Type="PAL8";

	    size=1+namesize(g->name)+256*3;

	    if(writeLab(g->toServerSocket,Type,size))return 1;

	    if(writeName(g->toServerSocket,g->name))return 1;
	
	    if(writeString(g->toServerSocket,pal,(long)256*3))return 1;

	}

	if(HdfPack((unsigned char *)buffin,(unsigned char *)buffout,length,&HdfSize))return 1;

	if(HdfSend(g->toServerSocket,g->name,(char *)buffout,(long)g->width,(long)g->height,HdfSize))return 1;
*/
    return 0;
} 
static int readgifimage(FILE *in,struct GIFdataStruct *gl,char *filename)
{
    unsigned char buf[9];
    int local, interleaved;
    unsigned char localmap[256][3];
    long length;
    int localbits;

    if (fread(buf, 1, 9, in) == 0) {
        sprintf(WarningBuff, "Error Reading %s GIF file\n",filename);
        WarningBatch(WarningBuff);
        return 1;
    }
    gl->left = buf[0] + (buf[1] << 8);
    gl->top = buf[2] + (buf[3] << 8);
    gl->width = buf[4] + (buf[5] << 8);
    gl->height = buf[6] + (buf[7] << 8);
    local = buf[8] & 0x80;
    interleaved = buf[8] & 0x40;

    if (local == 0 && gl->global == 0) {
        sprintf(WarningBuff, "(%s) no colormap present for image\n",filename);
        WarningBatch(WarningBuff);
        return 1;
    }

    length=(long)(gl->width*gl->height);
    if(SetBuffers(length+EXTRAFUDGE)){
        sprintf(WarningBuff, "(%s) not enough memory for image\n",filename);
        WarningBatch(WarningBuff);
       return 1;
    }

    gl->raster = (unsigned char*)xg.buffout;

    if (readraster(gl->width, gl->height,gl))
        return 1;
    if (local) {
        localbits = (buf[8] & 0x7) + 1;

        sprintf(WarningBuff, "   local colors: %d\n", 1<<localbits);
        WarningBatch(WarningBuff);

        fread(localmap, 3, 1<<localbits, in);
        initcolors(localmap, 1<<localbits,gl);
    } else if (gl->global) {
        initcolors(gl->globalmap, 1<<gl->globalbits,gl);
    }
    rasterize(interleaved, gl->raster,gl);

    return 0;
}
static int readraster(unsigned width,unsigned  height,struct GIFdataStruct *g)
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
		        sprintf(WarningBuff, "found eoi code\n");
		        WarningBatch(WarningBuff);
		#endif
				    goto exitloop;               /* because some GIF files*/
				}                                /* aren't standard */
				if (process(code, &fill,g)) {
				    goto exitloop;
				}
		    }
		}
		if (fill >= g->raster + width*height) {
		    sprintf(WarningBuff, "(%s) raster over flow error\n",g->name);
	        WarningBatch(WarningBuff);
		    goto exitloop;
		}
    }
exitloop:
    if (fill != g->raster + width*height)  {
	    sprintf(WarningBuff, "(%s) wrong size: %ld bytes \n",g->name,
						      (long) (fill-g->raster));
        WarningBatch(WarningBuff);
	    sprintf(WarningBuff, "instead of %ld bytes\n",
						      (long) width*height);
        WarningBatch(WarningBuff);
	    return 0; /* can still draw a picture ... */
    }
    return 0;
}
static int process(register int code,unsigned char **fill,struct GIFdataStruct *g)
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
	    sprintf(WarningBuff, "code % d to large for %d\n", code, g->avail);
        WarningBatch(WarningBuff);
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
static int initcolors(unsigned char colormap[COLSIZE][3], int ncolors,struct GIFdataStruct *g)
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
static int readscreen(FILE *in,struct GIFdataStruct *g)
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
static int checksignature(FILE *in,struct GIFdataStruct *g)
{
    char buf[6];

    fread(buf,1,6,in);
    if (strncmp(buf,"GIF",3)) {
        sprintf(WarningBuff, "file (%s) is not a GIF file\n",g->name);
        WarningBatch(WarningBuff);
        return 1;
    }
    if (!strncmp(&buf[3],"87a",3)) {
       g->gif89 = 0;
    }else if (!strncmp(&buf[3],"89a",3)) {
       g->gif89 = 1;
    }else{
        sprintf(WarningBuff, "unknown GIF version number (%s)\n",&buf[3]);
        WarningBatch(WarningBuff);
        return 1;
    }
    return 0;
}
int CheckGIFSpace(struct FileInfo2 *Files)
{
	struct GIF *GIFList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	GIFList=NULL;
	if(Files->GIFList){
	    GIFList=(struct GIF *)cRealloc((char *)Files->GIFList,ImageMax*sizeof(struct GIF),8759);
	    if(!GIFList){
	        WarningBatch("CheckGIFSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&GIFList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct GIF));
	}else{
	    GIFList=(struct GIF *)cMalloc(ImageMax*sizeof(struct GIF),8448);
	    if(!GIFList){
	        WarningBatch("CheckGIFSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)GIFList,ImageMax*sizeof(struct GIF));
	}


	Files->ImageMax=ImageMax;
	Files->GIFList=GIFList;

	return 0;
ErrorOut:
	if(GIFList){
	    Files->GIFList=GIFList;
	}
	return 1;
}

int initJPG(struct FileInfo2 *Files)
{
	char filepath[2048];
	unsigned char *image;
	unsigned char *buffer;
    long n,count,ImageCount;
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
    long iskip;
	long xsize;
	long ysize;

	if(!Files)return 1;

	Files->fileClass=JPG_FILES;
	Files->pSetFrame=FilesSetFrameJPG;
	Files->pdefault=FilesOpenHDF;
	Files->pGetInfo=FilesInfoJPG;
	FilesNames=Files->FilesNames;
	
	
	if(Files->loadToMemory){
	    Files->fileType=JPG_MEMORY;
	}else{
	    Files->fileType=JPG_DISK;
	}	
		
	count=0;
	ImageCount=0;
	xmax=ymax=0;
	xmin=ymin=200000;
	iskip=0;
	
	    buffer=NULL;
	    image=NULL;
	for(n=0;n<Files->FileCount;++n){
	    unsigned int Esc1,Esc2;
	    long length,lout;
	    int mask;
	    int ok;


	    lout=0;
	    Esc1=0;
	    Esc2=0;
	    ok=FALSE;
	    image=NULL;
	    buffer=NULL;


	    mstrncpy(filepath,Files->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    if(GetJPGImage(filepath,&xsize,&ysize,&image,Files->loadToMemory)){
	       sprintf(WarningBuff,"Could Not Load JPG %s",filepath);
	       goto ErrorOut;
	    }
	    
	    length=xsize*ysize*3;
	    
	    if(Files->loadToMemory){

			mask=255;

			buffer=(unsigned char *)cMalloc(length,9036);
			if(!buffer)goto ErrorOut;
			
			
			if(Stuff3(&Esc1,&Esc2,image,(unsigned char *)buffer,length,&lout,mask)){
				goto ErrorOut;
			}
	
			buffer=(unsigned char *)cRealloc((char *)buffer,lout,9036);
			if(!buffer)goto ErrorOut;

	    }

	    if(image)cFree((char *)image);
	    image=NULL;
	    
		if(CheckJPGSpace(Files))goto ErrorOut;

		Files->JPGList[ImageCount].Esc1=Esc1;
		Files->JPGList[ImageCount].Esc2=Esc2;
		Files->JPGList[ImageCount].Compressed=lout;
		Files->JPGList[ImageCount].UnCompressed=length;
		Files->JPGList[ImageCount].buffer=buffer;
		Files->JPGList[ImageCount].xsize=(int)xsize;
		Files->JPGList[ImageCount].ysize=(int)ysize;
		Files->JPGList[ImageCount].length=length;
		
		buffer=NULL;
		Files->ImageCount = ++ImageCount;
		
		if(xsize > xmax)xmax=xsize;
		if(ysize > ymax)ymax=ysize;
		if(xsize < xmin)xmin=xsize;
		if(ysize < ymin)ymin=ysize;
		ok=TRUE;

	    if(buffer)cFree((char *)buffer);
	    buffer=NULL;

	    if(ok){
    	        if(count < n){
    	            if(FilesNames[count])cFree((char *)FilesNames[count]);
        		    Files->FilesNames[count++]=FilesNames[n];
        		    FilesNames[n]=NULL;
    	        }else{
        		    Files->FilesNames[count++]=FilesNames[n];
    	        }
	    }else{
	       ++iskip;
	       cFree((char *)FilesNames[n]);
	       FilesNames[n]=NULL;
	    }
	}
	Files->FileCount=count;
	Files->xmin=xmin;
	Files->ymin=ymin;
	Files->xmax=xmax;
	Files->ymax=ymax;
	Files->length=ymax*xmax;
	Files->FileOpen = -1;

	if(count <= 0)goto ErrorOut;

	{
	    unsigned long lengths;

	    lengths=3*xmax*ymax*sizeof(char);
	    Files->dataSave=(unsigned char *)cMalloc(lengths,9037);
	    if(Files->dataSave == NULL){
	        WarningBatch("initJPG Out Of Memory\n");
	        goto ErrorOut;
	    }
	}

	if(iskip){
	    sprintf(WarningBuff,"%ld Files Skipped\n",iskip);
	    WarningBatch(WarningBuff);
	}
	
	do24BitStart(Files);

	return 0;
ErrorOut:
	WarningBatch("initJPG Failed\n");
	if(buffer)cFree((char *)buffer);
	buffer=NULL;
	freeFileList(Files);
	return 1;
}
int FilesInfoJPG(char **ImageName,long *xsize,long *ysize,long *Length,
              long CurrentFrame,struct FileInfo2 *Files)
{
	if(!ImageName || !xsize || !ysize || !Length || !Files)return 1;

	if(Files->fileClass != JPG_FILES)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	*xsize=Files->JPGList[CurrentFrame].xsize;
	*ysize=Files->JPGList[CurrentFrame].ysize;
	*Length=Files->JPGList[CurrentFrame].length;
	*ImageName=FilesImageName(Files,CurrentFrame);
	return 0;
}
int CheckJPGSpace(struct FileInfo2 *Files)
{
	struct JPGI *JPGList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	JPGList=NULL;
	if(Files->JPGList){
	    JPGList=(struct JPGI *)cRealloc((char *)Files->JPGList,ImageMax*sizeof(struct JPGI),9309);
	    if(!JPGList){
	        WarningBatch("CheckJPGSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&JPGList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct JPGI));
	}else{
	    JPGList=(struct JPGI *)cMalloc(ImageMax*sizeof(struct JPGI),9038);
	    if(!JPGList){
	        WarningBatch("CheckJPGSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)JPGList,ImageMax*sizeof(struct JPGI));
	}


	Files->ImageMax=ImageMax;
	Files->JPGList=JPGList;

	return 0;
ErrorOut:
	if(JPGList){
	    Files->JPGList=JPGList;
	}
	return 1;
}
int FilesSetFrameJPG(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	unsigned char *bp,*dataSave;
	long xsize,ysize;

	if(!Files)return 1;
	if(Files->fileClass != JPG_FILES)return 1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	s = &Files->SetData;
	s->SetFrame= -1;
	
	s->name=FilesImageName(Files,CurrentFrame);
	s->xsize=xsize=Files->JPGList[CurrentFrame].xsize;
	s->ysize=ysize=Files->JPGList[CurrentFrame].ysize;
	s->length=Files->JPGList[CurrentFrame].length;
	
	dataSave=Files->dataSave;

	if(Files->loadToMemory){
	    bp=(unsigned char *)Files->JPGList[CurrentFrame].buffer;
	    if(UnCompress(Files->JPGList[CurrentFrame].Esc1,
	                      Files->JPGList[CurrentFrame].Esc2,
	                      Files->JPGList[CurrentFrame].UnCompressed,
	                      Files->JPGList[CurrentFrame].Compressed,
	                      bp,dataSave))return 1;
	}else{
            char name[1024];

        if(Files->directory[0]){
            mstrncpy(name,Files->directory,1024);
        }else{
            name[0]=0;
        }
        if(Files->FilesNames && Files->FilesNames[CurrentFrame])
                      strncatToPath(name,Files->FilesNames[CurrentFrame],1024);
        FixPathName(name);              
	    if(GetJPGImage(name,&xsize,&ysize,&dataSave,1)){
	       return 1;
	    }
	    s->xsize=xsize;
	    s->ysize=ysize;
	} 


	s->SetFrame= CurrentFrame;
	s->type = TRUE_FILES;
	s->data = Files->dataSave;
	return 0;
}
int initSHOW(struct FileInfo2 *FilesL)
{
    long n,k,count,ImageCount,ImageFirst;
    char buff[256];
    char **FilesNames;
	FILE *in;
	char filepath[2048];

	if(!FilesL)return 1;

	in=NULL;
	FilesL->fileClass=SHOW_FILES;
	FilesL->pdefault=FilesOpenVRML;
	/*
	FilesL->pdefault=FilesOpenTRUE;
	FilesL->pSaveOne=FilesSaveTRUE;
	FilesL->pSaveOneMerged=FilesMergedTRUE;
	FilesL->pSetFrame=FilesSetFrameTRUE;
	*/
	FilesNames=FilesL->FilesNames;

	if(FilesL->loadToMemory){
	    FilesL->fileType=SHOW_MEMORY;
	}else{
	    FilesL->fileType=SHOW_DISK;
	}

	count=0;
	ImageCount=0;
	for(n=0;n<FilesL->FileCount;++n){
	    int ok;

	    ok=FALSE;
	    ImageFirst=ImageCount;

	    mstrncpy(filepath,FilesL->directory,2048);
	    strncatToPath(filepath,FilesNames[n],2048);
	    in=fopen(filepath,"rb");
	    if(in == NULL){
	       	sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
			WarningBatch(WarningBuff);
	       	goto Loop;
	    }

	    if(getString((unsigned char *)buff,4L,in))goto Loop;

	    buff[4]=0;

	    if(!strcmp(buff,"SHOW")){
	        ;
	    }else{
	        sprintf(WarningBuff,"File %s Skipped Not A Show File (%s)\n",
	                FilesNames[n],buff);
			WarningBatch(WarningBuff);
	        goto Loop;
	    }


	    while(1){
		struct SHOW *s;
	        long offset;

	        if(getString((unsigned char *)buff,4L,in))goto Loop;

	        buff[4]=0;

	        if(!strcmp(buff,"BLK1")){
	            ;
	        }else if(!strcmp(buff,"END1")){
	            continue;
	        }else if(!strcmp(buff,"DONE")){
	            break;
	        }else{
	            sprintf(WarningBuff,"File %s Skipped Not A BLK1 record (%s)\n",
	                FilesNames[n],buff);
				WarningBatch(WarningBuff);
	            goto Loop;
	        }

		if(CheckShowSpace(FilesL))goto Loop;

		s = &FilesL->SHOWList[ImageCount];

		if(getLongList(in,&s->nodeCount,1L))goto Loop;
		if(getLongList(in,&s->valueCount,1L))goto Loop;
		if(getLongList(in,&s->normalCount,1L))goto Loop;
		if(getLongList(in,&s->dxyzCount,1L))goto Loop;
		if(getLongList(in,&s->rsCount,1L))goto Loop;
		if(getLongList(in,&s->elementCount,1L))goto Loop;
		if(getLongList(in,&s->nodesMax,1L))goto Loop;
		if(getLongList(in,&s->localSystem,1L))goto Loop;
		if(getLongList(in,&s->stressCount,1L))goto Loop;
		if(getLongList(in,&s->stressMax,1L))goto Loop;
		if(getFloatList(in,&s->time,1L))goto Loop;
		if(getLongList(in,&s->velCount,1L))goto Loop;

		offset=ftell(in);

		s->offset=offset;

		if(FilesL->loadToMemory){

			if(s->nodeCount){
				s->x=(double *)cMalloc(s->nodeCount*sizeof(double),8454);
				s->y=(double *)cMalloc(s->nodeCount*sizeof(double),8455);
				s->z=(double *)cMalloc(s->nodeCount*sizeof(double),8456);
				if(!s->x || !s->y || !s->z)goto Loop;
		        if(getFloatList(in,s->x,s->nodeCount))goto Loop;
		        if(getFloatList(in,s->y,s->nodeCount))goto Loop;
		        if(getFloatList(in,s->z,s->nodeCount))goto Loop;
		/*
		        for(int k=0;k<s->nodeCount;++k){
		        	printf("%d %f %f %f\n",k,s->x[k],s->y[k],s->z[k]);
		        }
		*/
			}

			if(s->valueCount){
				s->v=(double *)cMalloc(s->valueCount*sizeof(double),8457);
				if(!s->v )goto Loop;
			    if(getFloatList(in,s->v,s->valueCount))goto Loop;
			}

			if(s->normalCount){
				s->nx=(double *)cMalloc(s->normalCount*sizeof(double),8458);
				s->ny=(double *)cMalloc(s->normalCount*sizeof(double),8459);
				s->nz=(double *)cMalloc(s->normalCount*sizeof(double),8460);
				if(!s->nx || !s->ny || !s->nz)goto Loop;
		        if(getFloatList(in,s->nx,s->normalCount))goto Loop;
		        if(getFloatList(in,s->ny,s->normalCount))goto Loop;
		        if(getFloatList(in,s->nz,s->normalCount))goto Loop;
			}

			if(s->dxyzCount){
				s->dx=(double *)cMalloc(s->dxyzCount*sizeof(double),8461);
				s->dy=(double *)cMalloc(s->dxyzCount*sizeof(double),8462);
				s->dz=(double *)cMalloc(s->dxyzCount*sizeof(double),8463);
				if(!s->dx || !s->dy || !s->dz)goto Loop;
			    if(getFloatList(in,s->dx,s->dxyzCount))goto Loop;
			    if(getFloatList(in,s->dy,s->dxyzCount))goto Loop;
			    if(getFloatList(in,s->dz,s->dxyzCount))goto Loop;
			}
			if(s->velCount){
				s->xvel=(double *)cMalloc(s->velCount*sizeof(double),8461);
				s->yvel=(double *)cMalloc(s->velCount*sizeof(double),8462);
				s->zvel=(double *)cMalloc(s->velCount*sizeof(double),8463);
				if(!s->xvel || !s->yvel || !s->zvel)goto Loop;
			    if(getFloatList(in,s->xvel,s->velCount))goto Loop;
			    if(getFloatList(in,s->yvel,s->velCount))goto Loop;
			    if(getFloatList(in,s->zvel,s->velCount))goto Loop;
			}

			if(s->rsCount){
				s->r=(double *)cMalloc(s->rsCount*sizeof(double),8464);
				s->s=(double *)cMalloc(s->rsCount*sizeof(double),8465);
				if(!s->r || !s->s)goto Loop;
			    if(getFloatList(in,s->r,s->rsCount))goto Loop;
			    if(getFloatList(in,s->s,s->rsCount))goto Loop;
			}

			if(s->elementCount){
				s->elements=(long *)cMalloc(s->elementCount*(s->nodesMax+1)*sizeof(long),8466);
				if(!s->elements)goto Loop;
			    if(getLongList(in,s->elements,s->elementCount*(s->nodesMax+1)))goto Loop;
			/*
			    for(int k=0;k<s->elementCount;++k){
			         printf("%ld %ld %ld %ld %ld\n",s->elements[k*5],s->elements[k*5+1],
			         s->elements[k*5+2],s->elements[k*5+3],s->elements[k*5+4]);
			    }
			*/
			}
			if(s->stressCount){
			//	printf("s->stressCount %ld s->stressMax %ld\n",s->stressCount,s->stressMax);
				s->stress=(double *)cMalloc(s->stressCount*s->stressMax*sizeof(double),8467);
				if(!s->stress)goto Loop;
			    if(getFloatList(in,s->stress,s->stressCount*s->stressMax))goto Loop;
			}
		}else{
		    long lout;
		    lout=0;
		    if(s->nodeCount){
		        lout += 3*s->nodeCount*sizeof(double);
		    }

		    if(s->valueCount){
		        lout += s->valueCount*sizeof(double);
		    }

		    if(s->normalCount){
		        lout += 3*s->normalCount*sizeof(double);
		    }

		    if(s->dxyzCount){
		        lout += 3*s->dxyzCount*sizeof(double);
		    }
		    
			if(s->velCount){
		        lout += 3*s->velCount*sizeof(double);
			}


		    if(s->rsCount){
		        lout += 2*s->rsCount*sizeof(double);
		    }

		    if(s->elementCount){
		        lout += s->elementCount*(s->nodesMax+1)*sizeof(long);
		    }
		    if(s->stressCount){
		        lout += s->stressCount*s->stressMax*sizeof(double);
		    }
		    fseek(in,offset+lout,0); 
		}
		FilesL->ImageFiles[ImageCount]=n;

		FilesL->ImageCount = ++ImageCount;

	    }
	    ok=1;
Loop:
	    for(k=ImageFirst;k<ImageCount;++k){
	        char name[256];

	        if(!FilesL->ImageNames[k]){
	            if(ImageCount-ImageFirst > 1){
	                sprintf(name,"%s.%ld%c",FilesNames[n],k,0);
	            }else{
	                sprintf(name,"%s%c",FilesNames[n],0);
	            }
		    FilesL->ImageNames[k]=strsave(name,8820);
	        }
	        ok=1;
	    }

	    if(in)fclose(in); 
	    in=NULL;

	    if(ok){
	        FilesL->FilesNames[count++]=FilesNames[n];
	    }else{
	        sprintf(WarningBuff,"Skipping file %s\n",FilesNames[n]);
			WarningBatch(WarningBuff);
	        cFree((char *)FilesNames[n]);
	    }
	}

	FilesL->FileCount=count;

	FilesL->FileOpen = -1;

	if(count <= 0)goto ErrorOut;

	return 0;
ErrorOut:
	if(in)fclose(in);
	if(FilesL->ImageCount > 0)return 0;
	freeFileList(FilesL);
	return 1;
}
int CheckShowSpace(struct FileInfo2 *Files)
{
	struct SHOW *SHOWList;
        long ImageMax;
	
	if(!Files)return 1;

	if(Files->ImageCount+1 < Files->ImageMax)return 0;

	if(CheckNames(Files))return 1;

	ImageMax = Files->ImageMax+IMAGE_INCREMENT;

	SHOWList=NULL;
	if(Files->SHOWList){
	    SHOWList=(struct SHOW *)cRealloc((char *)Files->SHOWList,ImageMax*sizeof(struct SHOW),8763);
	    if(!SHOWList){
	        WarningBatch("CheckShowSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)&SHOWList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct SHOW));
	}else{
	    SHOWList=(struct SHOW *)cMalloc(ImageMax*sizeof(struct SHOW),8468);
	    if(!SHOWList){
	        WarningBatch("CheckShowSpace out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)SHOWList,ImageMax*sizeof(struct SHOW));
	}


	Files->ImageMax=ImageMax;
	Files->SHOWList=SHOWList;

	return 0;
ErrorOut:
	if(SHOWList){
	    Files->SHOWList=SHOWList;
	}
	return 1;
}
double stressvalue(double *stress,long int stressmax,long int nstress)
{
	double value;
	//int nn;
	
	//printf("%g %ld %ld ",stress[0],stressmax,nstress);
	
	if(nstress < 0)nstress=0;
	
	value=0.0;
	if(nstress == 100){
	    if(stressmax == 6){
	       value += (stress[0]-stress[1])*(stress[0]-stress[1]);
	       value += (stress[1]-stress[2])*(stress[1]-stress[2]);
	       value += (stress[2]-stress[0])*(stress[2]-stress[0]);
	       value *= 0.25;
	       value += 3*(stress[3]*stress[3]);
	       value += 3*(stress[4]*stress[4]);
	       value += 3*(stress[5]*stress[5]);
	       if(value > 0.0)value=sqrt(value);
	       //printf(" 6 value %g\n",value);
	    }else{
	       value += (stress[0])*(stress[0]);
	       value += (stress[1])*(stress[1]);
	       value += (stress[0])*(stress[1]);
	       value += (stress[2]*stress[2]);
	       value *= 3.0;
	       if(value > 0.0)value=sqrt(value);
	       //printf(" <6 value %g\n",value);
	    }
	}else if(nstress == 101){
	    if(stressmax == 6){
	    }else{
		    value = 0.5*(stress[0])-(stress[1]);
		    value = value*value+stress[2]*stress[2];
  	        if(value > 0.0)value=sqrt(value);
       }
	}else{
	  //nstress=nstress;
	  if(nstress >= stressmax)nstress=0;
	  value=stress[nstress];
	  //printf(" value %g\n",value);
	}
	
	return value;
}
int FilesGetShow(struct Block *b,long CurrentFrame,struct FileInfo2 *Files)
{
	struct SHOW *s;
	long ElementCount;
	long NodeCount;
	long n;

	if(!b || !Files)return 1;

	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != SHOW_FILES)return 1;

	s = &Files->SHOWList[CurrentFrame];
	if(!s)return 1;
	if(Files->fileType == SHOW_DISK){
	    if(SetFileToRead(CurrentFrame,Files))return 1;
	    fseek(Files->in,s->offset,0); 
	}

	NodeCount=b->NodeCount;
	
	ElementCount=b->ElementCount;

	if(s->nodeCount){
		if(!b->xdata || !b->ydata || !b->zdata || 
		  (b->NodeCount < s->nodeCount)){
		    if(b->xdata)cFree((char *)b->xdata);
		    if(b->ydata)cFree((char *)b->ydata);
		    if(b->zdata)cFree((char *)b->zdata);
			b->xdata=(double *)cMalloc(s->nodeCount*sizeof(double),8469);
			b->ydata=(double *)cMalloc(s->nodeCount*sizeof(double),8470);
			b->zdata=(double *)cMalloc(s->nodeCount*sizeof(double),8471);
			if(!b->xdata || !b->ydata || !b->zdata)return 1;
		}
		if(Files->loadToMemory){
		    for(n=0;n<s->nodeCount;++n){
		        b->xdata[n]=s->x[n];
		        b->ydata[n]=s->y[n];
		        b->zdata[n]=s->z[n];
		    }
		}else{
		    if(getFloatList(Files->in,b->xdata,s->nodeCount))return 1;
		    if(getFloatList(Files->in,b->ydata,s->nodeCount))return 1;
		    if(getFloatList(Files->in,b->zdata,s->nodeCount))return 1;
		}
		b->NodeCount=s->nodeCount;
	}

	if(s->valueCount){
	    if(b->NodeCount != s->valueCount)return 1;
		if(!b->vdata || (NodeCount < s->valueCount)){
		    if(b->vdata)cFree((char *)b->vdata);
			b->vdata=(double *)cMalloc(b->NodeCount*sizeof(double),8472);
			if(!b->vdata)return 1;
		}
		if(Files->loadToMemory){
		    for(n=0;n<b->NodeCount;++n){
		        b->vdata[n]=s->v[n];
		    }
		}else{
		    if(getFloatList(Files->in,b->vdata,b->NodeCount))return 1;
		}
	}

	if(s->normalCount){
	    if(b->NodeCount != s->normalCount)return 1;
		if(!b->nx || !b->ny || !b->nz || (NodeCount < s->normalCount)){
		    if(b->nx)cFree((char *)b->nx);
		    if(b->ny)cFree((char *)b->ny);
		    if(b->nz)cFree((char *)b->nz);
			b->nx=(double *)cMalloc(s->nodeCount*sizeof(double),8473);
			b->ny=(double *)cMalloc(s->nodeCount*sizeof(double),8474);
			b->nz=(double *)cMalloc(s->nodeCount*sizeof(double),8475);
			if(!b->nx || !b->ny || !b->nz)return 1;
		}
		if(Files->loadToMemory){
		    for(n=0;n<b->NodeCount;++n){
		        b->nx[n]=s->x[n];
		        b->ny[n]=s->y[n];
		        b->nz[n]=s->z[n];
		    }
		}else{
		    if(getFloatList(Files->in,b->nx,b->NodeCount))return 1;
		    if(getFloatList(Files->in,b->ny,b->NodeCount))return 1;
		    if(getFloatList(Files->in,b->nz,b->NodeCount))return 1;
		}
	}else{
		if(b->nx)cFree((char *)b->nx);
		b->nx=NULL;
		
		if(b->ny)cFree((char *)b->ny);
		b->ny=NULL;
		
		if(b->nz)cFree((char *)b->nz);
		b->nz=NULL;
	}

	if(s->dxyzCount){
	    b->dxyzCount=0;
	    if(b->NodeCount != s->dxyzCount)return 1;
		if(!b->dxdata || !b->dydata || !b->dzdata || (NodeCount < s->dxyzCount)){
		    if(b->dxdata)cFree((char *)b->dxdata);
		    if(b->dydata)cFree((char *)b->dydata);
		    if(b->dzdata)cFree((char *)b->dzdata);
			b->dxdata=(double *)cMalloc(s->nodeCount*sizeof(double),8476);
			b->dydata=(double *)cMalloc(s->nodeCount*sizeof(double),8477);
			b->dzdata=(double *)cMalloc(s->nodeCount*sizeof(double),8478);
			if(!b->dxdata || !b->dydata || !b->dzdata)return 1;
		}
		if(Files->loadToMemory){
		    for(n=0;n<b->NodeCount;++n){
		        b->dxdata[n]=s->dx[n];
		        b->dydata[n]=s->dy[n];
		        b->dzdata[n]=s->dz[n];
		    }
		}else{
		    if(getFloatList(Files->in,b->dxdata,b->NodeCount))return 1;
		    if(getFloatList(Files->in,b->dydata,b->NodeCount))return 1;
		    if(getFloatList(Files->in,b->dzdata,b->NodeCount))return 1;
		}
	    b->dxyzCount=s->dxyzCount;
	}
	
	if(s->velCount){
		if(Files->loadToMemory){
		    
		}else{
			if(s->xvel)cFree((char *)s->xvel);
			if(s->yvel)cFree((char *)s->yvel);
			if(s->zvel)cFree((char *)s->xvel);
			s->xvel=(double *)cMalloc(s->velCount*sizeof(double),8461);
			s->yvel=(double *)cMalloc(s->velCount*sizeof(double),8462);
			s->zvel=(double *)cMalloc(s->velCount*sizeof(double),8463);
			if(s->xvel && s->yvel && s->zvel){
				if(getFloatList(Files->in,s->xvel,s->velCount))return 1;
				if(getFloatList(Files->in,s->yvel,s->velCount))return 1;
				if(getFloatList(Files->in,s->zvel,s->velCount))return 1;
		    }
		}
	}

	if(s->rsCount){
	    if(b->NodeCount != s->rsCount)return 1;
		if(!b->rdata || !b->sdata || (NodeCount < s->rsCount)){
		    if(b->rdata)cFree((char *)b->rdata);
		    if(b->sdata)cFree((char *)b->sdata);
			b->rdata=(float *)cMalloc(s->nodeCount*sizeof(float),8479);
			b->sdata=(float *)cMalloc(s->nodeCount*sizeof(float),8480);
			if(!b->rdata || !b->sdata)return 1;
		}
		if(Files->loadToMemory){
		    for(n=0;n<b->NodeCount;++n){
		        b->rdata[n]=(float)s->r[n];
		        b->sdata[n]=(float)s->s[n];
		    }
		}else{
		   /*
		    if(getFloatList(Files->in,b->rdata,b->NodeCount))return 1;
		    if(getFloatList(Files->in,b->sdata,b->NodeCount))return 1;
		   */
		   WarningBatch("getFloatList Error On RS-Data\n");
		}
	}

	if(s->elementCount){
		if(!b->Elements || 
		  (b->ElementCount*b->NodesMax < s->elementCount*s->nodesMax)){
		    if(b->Elements)cFree((char *)b->Elements);
			b->Elements=(long *)cMalloc(s->elementCount*(s->nodesMax+1)*sizeof(long),8481);
			if(!b->Elements)return 1;
		}
		if(Files->loadToMemory){
		    for(n=0;n<s->elementCount*(s->nodesMax+1);++n){
		        b->Elements[n]=s->elements[n];
		    }
		}else{
		    if(getLongList(Files->in,b->Elements,s->elementCount*(s->nodesMax+1)))return 1;
		}
		b->ElementCount=s->elementCount;
		b->NodesMax=s->nodesMax;
		if(s->stressCount <= 0 && b->dxyzCount > 0){
		     if(s->nodesMax == 8){
		      	strains3Dget(b);
		     }else if(s->nodesMax == 4){
		      	strains2Dget(b);
		     }
		}
	}



	if(s->stressCount){
	    int nstress;
	   // printf("s->stressCount %ld b->ElementCount %ld s->stressMax %ld\n",s->stressCount,b->ElementCount,s->stressMax);
	    if(b->ElementCount != s->stressCount)return 1;
		if(!b->stress || (ElementCount < s->stressCount)){
		    b->stress=(double *)cMalloc(s->stressCount*s->stressMax*sizeof(double),8482);
		    if(!b->stress)return 1;
		}
		b->StressCount=s->stressCount;
		nstress=b->nstress;
		if(Files->loadToMemory){
		    for(n=0;n<b->StressCount;++n){
		        //printf("%ld %f\n",n,s->stress[n*s->stressMax]);
		        b->stress[n]=stressvalue(&s->stress[n*s->stressMax],s->stressMax,nstress);
		        //b->stress[n]=s->stress[n*s->stressMax+nstress];
		    }
		}else{
		    if(getFloatList(Files->in,b->stress,b->StressCount*s->stressMax))return 1;
		    for(n=0;n<b->StressCount;++n){
		        //printf("%ld %f\n",n,b->stress[n*s->stressMax]);
		        b->stress[n]=stressvalue(&b->stress[n*s->stressMax],s->stressMax,nstress);
		        //b->stress[n]=s->stress[n*s->stressMax+nstress];
		    }
		}
	}

	return 0;
}

int deriq (int nel,int ityp2d,double *xx,double *b,double *det,double r,double s,double *x1bar);
int funct2 (double r,double s,double *h,double *p,double *xj,double *det,double *xx,int nel);
int funct(int nel,double r,double s,double t,double *h,double *p,double *xj,double *det,double *xx);
int deriq3(int nel,double *xx,double *b,double *det,double r,double s,double t);

int strains2Dget(struct Block *bb)
{
	double strain[9];
	double b[3*8],xxx[3*8],edis[3*8];
	int NodesMaxp;
	
	if(!bb)return 1;
	
	NodesMaxp=bb->NodesMax+1;
	
	if(!bb->stress){
		bb->stress=(double *)cMalloc(bb->ElementCount*sizeof(double),8489);
		if(!bb->stress)return 1;
	}
	
	for(int n=0;n<bb->ElementCount;++n){
		double x1bar;
		double e1=0;                                                                        
		double e2=0;                                                                        
		double det;  

        //fprintf(stderr,"n %d ",n+1);
		for(int j=0;j<4;++j){
		  xxx[2*j]=    bb->xdata[bb->Elements[NodesMaxp*n+j+1]];
		  xxx[2*j+1]=  bb->ydata[bb->Elements[NodesMaxp*n+j+1]];
		  edis[2*j]=  bb->dxdata[bb->Elements[NodesMaxp*n+j+1]];
		  edis[2*j+1]=bb->dydata[bb->Elements[NodesMaxp*n+j+1]];
          //fprintf(stderr,"%ld ",bb->Elements[NodesMaxp*n+j+1]+1);
		}

																
		deriq (n,2,xxx,b,&det,e1,e2,&x1bar);                              

		for(int j=0;j<5;++j){
		  strain[j]=0.0;
		}
		
#define b(i,j) b[i-1+(j-1)*2]

	  for(int j=2;j<12;j += 2){                                                     
		  int i=j - 1;                                                                   
		  strain[0]=strain[0] + b(1,i)*edis[i-1];                                         
		  strain[1]=strain[1] + b(2,j)*edis[j-1];                                          
		  strain[2]=strain[2] + b(3,i)*edis[i-1];                                          
		  strain[3]=strain[3] + b(3,j)*edis[j-1];
      }  
                                             
    //  if (ist.eq.3) go to 160  
                                                     
	  for(int i=2;i<12;i += 2){                                                     
            strain[4]=strain[4] + b(4,i)*edis[i-1];
       }                                          
	   bb->stress[n]=strain[bb->nstress];
	}
	
#undef b

	return 0;
}

                            
int deriq (int nel,int ityp2d,double *xx,double *b,double *det,double r,double s,double *x1bar)                            
{      
	double h[8],p[2*8],xj[2*2],xji[2*2];
	
      funct2 (r,s,h,p,xj,det,xx,nel);                                  

      double dum = 1.0/(*det);
                                                                   
#define xji(i,j) (xji[i-1+(j-1)*2])
#define xj(i,j) (xj[i-1+(j-1)*2])
                                                                  
      xji(1,1) = xj(2,2)* dum;                                                   
      xji(1,2) =-xj(1,2)* dum;                                                   
      xji(2,1) =-xj(2,1)* dum;                                                   
      xji(2,2) = xj(1,1)* dum;   
      
#define b(i,j) b[i-1+(j-1)*2]
#define p(i,j) p[i-1+(j-1)*2]
                                                      
	for(int k=1;k<=4;++k){                                                            
		  int k2=k*2;                                                                    
		  b(1,k2-1) = 0.;                                                            
		  b(1,k2  ) = 0.;                                                            
		  b(2,k2-1) = 0.;                                                            
		  b(2,k2  ) = 0.;                                                            
		  for(int i=1;i<=2;++i){                                                             
			 b(1,k2-1) = b(1,k2-1) + xji(1,i) * p(i,k);                                 
			 b(2,k2  ) = b(2,k2  ) + xji(2,i) * p(i,k);
		  }                                 
		  b(3,k2  ) = b(1,k2-1);                                                     
		  b(3,k2-1) = b(2,k2  );  
	}                                                   


      if (ityp2d > 0) return 1;                                                  
 
#define xx(i,j) xx[i-1+(j-1)*2]
 
      *x1bar = 0.0;                                                              
 	 for(int k=1;k<=4;++k){                                                                                                                         
         *x1bar = *x1bar + h[k-1]* xx(1,k);
     }                                             

      if(*x1bar < 0.00000001){                                         
		  int nd=2*4;                                                                  
		  for(int k=1;k<=nd;++k){                                                             
			  b(4,k)=b(1,k);      
		  }                                                       
		  return 0;
      }                                                                   

      dum = 1.0/(*x1bar);                                                           
      for(int k=1;k<=4;++k){                                                            
		  int k2=k*2;                                                                    
		  b(4,k2  ) = 0.;                                                            
		  b(4,k2-1) = h[k-1] * dum; 
      }    
      
#undef xx                                            
#undef b                                            
#undef p                                            
#undef xji                                            
#undef xj                                            
    return 0;                                         
}
int funct2 (double r,double s,double *h,double *p,double *xj,double *det,double *xx,int nel) 
{                                                                                          
      double rp = 1.0 + r;                                                              
      double sp = 1.0 + s;                                                              
      double rm = 1.0 - r;                                                              
      double sm = 1.0 - s;                                                              
      h[0] = 0.25* rp* sp;                                                      
      h[1] = 0.25* rm* sp;                                                       
      h[2] = 0.25* rm* sm;                                                       
      h[3] = 0.25* rp* sm;                                                      
      p[0  ]=0.25*s;                                                            
      p[0+1*2]=-p[0  ];                                                            
      p[0+2*2]=-0.25*sm;                                                          
      p[0+3*2]=-p[0+2*2];                                                           
      p[1  ]=0.25*rp;                                                            
      p[1+1*2]=0.25*rm;                                                           
      p[1+2*2]=-p[1+1*2];                                                            
      p[1+3*2]=-p[1  ];                                                           
                                                                               
      for(int i=0;i<2;++i){ 
		  for(int j=0;j<2;++j){                                                                
			  double dum = 0.0;
			  for(int k=0;k<4;++k){                                                                                                                         
					   dum = dum + p[i+k*2]* xx[j+k*2]; 
			  }                                             
			  xj[i+j*2] = dum;  
		  }     
      }                                                      
      *det = xj[0]* xj[3] - xj[2]* xj[1];                                 
      double dum=(*det);                                                                   
      if(dum < 0.00000001){  
          fprintf(stderr,"nel %d det %g\n",nel,dum);                                                                                                  
      }
	return 0;                                                          
}



int strains3Dget(struct Block *bb)
{
	double strain[9];
	double b[3*8],xxx[3*8],edis[3*8],xxx1[3*8],edis1[3*8];
	int NodesMaxp;
	
	if(!bb)return 1;
	
	NodesMaxp=bb->NodesMax+1;
	
	if(!bb->stress){
		bb->stress=(double *)cMalloc(bb->ElementCount*sizeof(double),8489);
		if(!bb->stress)return 1;
	}
	
	for(int n=0;n<bb->ElementCount;++n){
		double e1=0;                                                                        
		double e2=0;                                                                        
		double e3=0;
		double det;  

        //fprintf(stderr,"n %d ",n+1);
		for(int j=0;j<8;++j){
		  xxx1[3*j]=    bb->xdata[bb->Elements[NodesMaxp*n+j+1]];
		  xxx1[3*j+1]=  bb->ydata[bb->Elements[NodesMaxp*n+j+1]];
		  xxx1[3*j+2]=  bb->zdata[bb->Elements[NodesMaxp*n+j+1]];
		  edis1[3*j]=  bb->dxdata[bb->Elements[NodesMaxp*n+j+1]];
		  edis1[3*j+1]=bb->dydata[bb->Elements[NodesMaxp*n+j+1]];
		  edis1[3*j+2]=bb->dzdata[bb->Elements[NodesMaxp*n+j+1]];
          //fprintf(stderr,"%ld ",bb->Elements[NodesMaxp*n+j+1]+1);
		}
		
		for(int j=0;j<12;++j){
		  xxx[j]=xxx1[12+j];
		  xxx[j+12]=xxx1[j];
		  edis[j]=edis1[12+j];
		  edis[j+12]=edis1[j];
		}
        //fprintf(stderr,"\n");

																
		deriq3 (n,xxx,b,&det,e1,e2,e3);                              

		for(int j=0;j<9;++j){
		  strain[j]=0.0;
		}

		for(int j=2;j<24;j += 3){                                                     
		  int i=j-1;                                                                     
		  int k=j-2;                                                                     
		  strain[0]=strain[0]+b[k]*edis[k];                                              
		  strain[1]=strain[1]+b[i]*edis[i];                                              
		  strain[2]=strain[2]+b[j]*edis[j];                                              
		  strain[3]=strain[3]+b[i]*edis[k];                                              
		  strain[4]=strain[4]+b[j]*edis[k];                                              
		  strain[5]=strain[5]+b[k]*edis[i];                                              
		  strain[6]=strain[6]+b[j]*edis[i];                                              
		  strain[7]=strain[7]+b[k]*edis[j];                                              
		  strain[8]=strain[8]+b[i]*edis[j];
		}                                              

		bb->stress[n]=strain[bb->nstress];
	}
	
	return 0;
}
int deriq3(int nel,double *xx,double *b,double *det,double r,double s,double t)
{                              
                                                
      double h[21],p[3*21],xj[3*3],xji[3*3];                                
                                                                            
      funct(nel,r,s,t,h,p,xj,det,xx);                                     

      double dum=1.0/(*det);                                                               
      xji[0  ]=dum*(   xj[1+1*3]*xj[2+2*3] - xj[1+2*3]*  xj[2+1*3]);                         
      xji[1  ]=dum*(  -xj[1  ]*  xj[2+2*3] + xj[1+2*3]*  xj[2  ]);                         
      xji[2  ]=dum*(   xj[1  ]*  xj[2+1*3] - xj[1+1*3]*xj[2  ]);                         
      xji[0+1*3]=dum*(-xj[0+1*3]*xj[2+2*3] + xj[0+2*3]*  xj[2+1*3]);                         
      xji[1+1*3]=dum*( xj[0  ]*  xj[2+2*3] - xj[0+2*3]*  xj[2  ]);                         
      xji[2+1*3]=dum*(-xj[0  ]*  xj[2+1*3] + xj[0+1*3]*xj[2  ]);                         
      xji[0+2*3]=dum*( xj[0+1*3]*xj[1+2*3] - xj[0+2*3]*  xj[1+1*3]);                         
      xji[1+2*3]=dum*(-xj[0  ]*  xj[1+2*3] + xj[0+2*3]*  xj[1  ]);                         
      xji[2+2*3]=dum*( xj[0  ]*  xj[1+1*3] - xj[0+1*3]*xj[1  ]);                         

      for(int k=0;k<8;++k){                                                           
          int k2=k*3;
          for(int i=0;i<3;++i){                                                                                                                       
             b[k2-i]=0.0;
          }                                                             
          for(int i=0;i<3;++i){                                                                                                                       
      		b[k2-2]=b[k2-2]   + xji[0+3*i]*p[i+3*k];                                         
      		b[k2-1]=b[k2-1]   + xji[1+3*i]*p[i+3*k];                                         
   			b[k2] = b[k2]     + xji[2+3*i]*p[i+3*k];
          }
      }                                             
   
  
      return 0;
  
}                                                                                                                                                
int funct(int nel,double r,double s,double t,double *h,double *p,double *xj,double *det,double *xx)
{                                                                            
      double rp=1.0 + r;                                                                
      double sp=1.0 + s;                                                                
      double tp=1.0 + t;                                                                
      double rm=1.0 - r;                                                               
      double sm=1.0 - s;                                                              
      double tm=1.0 - t;                                                             
                                                                
      h[0]=0.125*rp*sp*tp;                                                       
      h[1]=0.125*rm*sp*tp;                                                       
      h[2]=0.125*rm*sm*tp;                                                       
      h[3]=0.125*rp*sm*tp;                                                       
      h[4]=0.125*rp*sp*tm;                                                       
      h[5]=0.125*rm*sp*tm;                                                       
      h[6]=0.125*rm*sm*tm;                                                       
      h[7]=0.125*rp*sm*tm;                                                       
                                                                            
                                                                               
      p[0]= 0.125*sp*tp;                                                       
      p[0+1*3]=-p[0];                                                            
      p[0+2*3]=-0.125*sm*tp;                                                       
      p[0+3*3]=-p[0+2*3];                                                            
      p[0+4*3]= 0.125*sp*tm;                                                       
      p[0+5*3]=-p[0+4*3];                                                            
      p[0+6*3]=-0.125*sm*tm;                                                       
      p[0+7*3]=-p[0+6*3];                                                            
                                                                               
      p[1]= 0.125*rp*tp;                                                       
      p[1+1*3]= 0.125*rm*tp;                                                       
      p[1+2*3]=-p[1+1*3];                                                            
      p[1+3*3]=-p[1];                                                            
      p[1+4*3]= 0.125*rp*tm;                                                       
      p[1+5*3]= 0.125*rm*tm;                                                       
      p[1+6*3]=-p[1+5*3];                                                            
      p[1+7*3]=-p[1+4*3];                                                            
                                                                               
      p[2]= 0.125*rp*sp;                                                       
      p[2+1*3]= 0.125*rm*sp;                                                       
      p[2+2*3]= 0.125*rm*sm;                                                       
      p[2+3*3]= 0.125*rp*sm;                                                       
      p[2+4*3]=-p[2];                                                            
      p[2+5*3]=-p[2+1*3];                                                            
      p[2+6*3]=-p[2+2*3];                                                            
      p[2+7*3]=-p[2+3*3];  
   /*   
      fprintf(stderr,"nel %d\n",nel);
      for(int n=0;n<8;++n){
         fprintf(stderr,"%g %g %g\n",xx[3*n],xx[3*n+1],xx[3*n+2]);
      }                                                          
    */                                                                      
      for(int i=0;i<3;++i){  
      	for(int j=0;j<3;++j){                                                         												   
			  double dum=0.0;
			  for(int k=0;k<8;++k){                                                                  
				  dum=dum + p[i+3*k]*xx[j+3*k];
			  }                                                  
			  xj[i+3*j]=dum;  
		  }
      }                                                             
      *det =     xj[0]*xj[1+1*3]*xj[2+2*3]                                            
           + xj[0+1*3]*xj[1+2*3]*xj[2]                                             
           + xj[0+2*3]*xj[1]*xj[2+1*3]                                             
           - xj[0+2*3]*xj[1+1*3]*xj[2]                                             
           - xj[0+1*3]*xj[1]*xj[2+2*3]                                            
           - xj[0]*xj[1+2*3]*xj[2+1*3];
                                                      
      double dum = *det;                                                                   
      if (dum < 1.0e-08){                                           
        fprintf(stderr,"nel %d det = %g\n",nel,dum);
         return 1;                                                            
      }                                                               
      return 0;                                                                   
}
int showLoad(struct SHOW *s,long CurrentFrame,struct FileInfo2 *Files)
{
	if(!s || !Files)return 1;
	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != SHOW_FILES)return 1;

	return 0;
}
int getLongList(FILE *in,long *LongData,unsigned long count)
{
	unsigned long n;

	if(!in || !LongData)return 1;

	for(n=0;n<count;++n){
	   if(getLong(&LongData[n],in))return 1;
	}

	return 0;
}
int getFloatList(FILE *in,double *FloatData,unsigned long count)
{
	unsigned long length;

	if(!in || !FloatData)return 1;

	length=count*sizeof(double);

	if(fread(FloatData,1,length,in) != length)return 1;
#ifdef PC
	{
		unsigned char c,*cp;
		unsigned long np;

	        for(np=0;np<count;np++){
	            cp=(unsigned char *)&FloatData[np];
			    c=cp[0];
			    cp[0]=cp[7];
			    cp[7]=c;
			    c=cp[1];
			    cp[1]=cp[6];
			    cp[6]=c;
			    c=cp[2];
			    cp[2]=cp[5];
			    cp[5]=c;
			    c=cp[3];
			    cp[3]=cp[4];
			    cp[4]=c;
	        }
	}
#endif

	return 0;
}
int CheckNames(struct FileInfo2 *Files)
{
		char **ImageNames;
		long *ImageFiles;
        long ImageMax;

		if(!Files)return 1;

		if(Files->ImageCount+1 < Files->ImageMax)return 0;

		ImageMax = Files->ImageMax+IMAGE_INCREMENT;

		ImageNames=NULL;
		ImageFiles=NULL;

		if(Files->ImageNames){
	    	ImageNames=(char **)cRealloc((char *)Files->ImageNames,ImageMax*sizeof(char *),8266);
	    	if(!ImageNames){
	        	WarningBatch("CheckNames out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)&ImageNames[Files->ImageMax],IMAGE_INCREMENT*sizeof(char *));
		}else{
	    	ImageNames=(char **)cMalloc(ImageMax*sizeof(char *),8139);
	    	if(!ImageNames){
	        	WarningBatch("CheckNames out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)ImageNames,ImageMax*sizeof(char *));
		}

		if(Files->ImageFiles){
	    	ImageFiles=(long *)cRealloc((char *)Files->ImageFiles,ImageMax*sizeof(long),8267);
	    	if(!ImageFiles){
	        	WarningBatch("initHDF out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)&ImageFiles[Files->ImageMax],IMAGE_INCREMENT*sizeof(long));
		}else{
	    	ImageFiles=(long *)cMalloc(ImageMax*sizeof(long),8141);
	    	if(!ImageFiles){
	        	WarningBatch("initHDF out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)ImageFiles,ImageMax*sizeof(long));
		
		}
		Files->ImageNames=ImageNames;
		Files->ImageFiles=ImageFiles;

		return 0;
ErrorOut:
		if(ImageNames){
	    	Files->ImageNames=ImageNames;
		}
		if(ImageFiles){
	    	Files->ImageFiles=ImageFiles;
		}
		return 1;
}
int IsViewLib(char *namef)
{
	char name[256];
	FILE *in;
	int n;

	if(!namef)return 0;

	in=fopen(namef,"r");
	if(in == NULL){
	   return 0;
	}
	for(n=0;n<4;++n){
	    name[n]=fgetc(in);
	}
	name[4]=0;

	fclose(in);

	if(!strcmp(name,"VHD1")){
	   return 1;
	}

	return 0;
}
int UnPack(register unsigned char *image,register unsigned char *buff,register long Length)
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
int HdfPack(unsigned char *image,unsigned char *buff,long ImageSize,long *HdfSize)
{
	unsigned char *nn;
	long n,kk,jj,nr;
	int max,nw,nc;
	unsigned int c;

	if(!image || !buff || !HdfSize)return 1;
		
	n=0;
	nc=0;
	nn=buff++;
	max=0x7f;
	--max;
	for(kk=0;kk<ImageSize;++kk){
	    c = image[kk];
	    nr=0;
	    for(jj=kk+1;jj<ImageSize;++jj){
	        if(c != image[jj])break;
	        if(++nr >= max)break;
	    }
	    if(nr > 2){
	        if(nc > 0){
	            *nn=nc;
	    	    nn=buff++;
	            n += (nc+1);
	            nc = 0;
	        }
	        if(nr > max){
	            nw=max;
	        }else{
	            nw=(int)nr;
	        }
	        kk += (nw-1);
	        *nn = nw | 0x80;
	        *buff++ = c;
	    	nn=buff++;
	        n += 2;
	    }else{
	        *buff++ = c;
	        if(++nc >= max){
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
	return 0;
}

int FileOpenSave(struct FileInfo2 *Files)
{
	if(!Files)return 1;
	
	if(xg.FileCount < 1000){
	    xg.FilesList[xg.FileCount++]=Files;
	}
	return 0;
}
int FileOpenIsOpen(struct FileInfo2 *File)
{
	long n;
	
	if(!File)return 0;
	
	for(n=0;n<xg.FileCount;++n){
	    if(xg.FilesList[n] == File){
	        return 1;
	    }
	}
	return 0;
}
int FileOpenRemove(struct FileInfo2 *Files)
{
	struct FileInfo2 *File;
	long n,nn;
	
	if(!Files)return 1;
	
	nn=0;
	for(n=0;n<xg.FileCount;++n){
	    File=xg.FilesList[n];
	    if(File == Files){
	        continue;
	    }
	    xg.FilesList[nn++]=File;
	}
	
	if(xg.FileCount == nn){
	    sprintf(WarningBuff,"FilesOpenRemoveNotOwned Could not remove %lx\n",(unsigned long)Files);
	    WarningBatch(WarningBuff);
	}
	
	xg.FileCount=nn;
	return 0;
}
int FilesOpenRemoveNotOwned()
{
	struct FileInfo2 *FilesDelete[1000];
	struct FileInfo2 **Files;
	struct Message1000 m;
	char *data;
	long n,nn;
	long Length;
	
	
	m.FilesList=xg.FilesList;
	m.FileOwner=xg.FileOwner;
	m.FileCount=xg.FileCount;
	for(n=0;n<xg.FileCount;++n){
	    xg.FileOwner[n]=0;
	}
	
	SendMessageToAll(NULL,1,MessageType_GET_FILE_OWNER,(void *)&m);
	
	data=NULL;
	Length=uGetScrap("FLST",&data);
	if(Length > 0){
	    Length /= sizeof(struct FileInfo2 *);
		Files = (struct FileInfo2 **)data;
		for(n=0;n<xg.FileCount;++n){
		    for(nn=0;nn<Length;++nn){
		        if(xg.FilesList[n] == Files[nn]){
		            xg.FileOwner[n] = TRUE;
		        }
		    }
		}
	}
	
	if(data)cFree((char *)data);	
	
	nn=0;
	for(n=0;n<xg.FileCount;++n){
	    if((xg.FileOwner[n] == 0) && (xg.FilesList[n]->doNotAutoRemove == 0)){
	       FilesDelete[nn++]=xg.FilesList[n];
	    }
	}
	
	
	if(nn > 0){
		for(n=0;n<nn;++n){
		    freeFileList(FilesDelete[n]);
		}
	}
	
	FileManagerNotOwned();
	
	return 0;
}

#ifdef Macintosh
                    
int initVIEW(struct FileInfo2 *Files)
{
    long n;
    char **FilesNames;
	FILE *in;
	char filepath[2048];

		if(!Files)return 1;

		in=NULL;

		Files->fileClass=VIEW_FILES;
		
		Files->pdefault=OpenViewLib2;
		
	/*
		Files->pdefault=FilesOpenTek;
	Files->pSaveOne=FilesSaveTRUE;
	Files->pSaveOneMerged=FilesMergedTRUE;
	*/

		FilesNames=Files->FilesNames;
				
		Files->loadToMemory=1;
		    
		for(n=0;n<Files->FileCount;++n){
	    	unsigned char *buffer;
	    	unsigned char buff[256];
	    	int ok;


	    	ok=FALSE;
	    	buffer=NULL;
	    	
	    	mstrncpy(filepath,Files->directory,2048);
	    	strncatToPath(filepath,FilesNames[n],2048);

		    in=fopen(filepath,"rb");
		    if(in == NULL){
		       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
		       WarningBatch(WarningBuff);
		       continue;
		    }
		    
	     	
	     	if(getString((unsigned char *)buff,4L,in))goto ErrorOut;
	     	if(buff[0] != 'V' || buff[1] != 'H' || buff[2] != 'D' || buff[3] != '1'){
	            sprintf(WarningBuff,"File %s is Not A ViewLib File (%s)\n",
	                FilesNames[n],buff);
	            WarningBatch(WarningBuff);
	            goto ErrorOut;
	     	}
		    if(in)fclose(in);
			in=NULL;
	     	
		}

		return 0;
ErrorOut:

		if(in)fclose(in);
		freeFileList(Files);		
		return 1;
}

int FileAppendPICT(char *name,Handle PICThand,struct FileInfo2 **iFiles);
int PictToTrue(struct FileInfo2 *Files,struct SetFrameData *s);

int initQUICKTIME(struct FileInfo2 *Files)
{

#define forwardNormalSpeed 0x00010000		/* normal speed (1) in fixed */

        long n,count,length,ImageCount,lengthM;
        unsigned char pname[2048];
        char imageName[256];
		unsigned char *fp;
		FSSpec theFSSpec;
		short refNum = -1, resID = 0;
		Movie movie = 0;
        char **FilesNames;
        long xmin,xmax,ymin,ymax;
	    int xsize,ysize;
 		short vRefNum;
 		long dirIDToSearch;
		TimeValue inTime;
		OSType trackTypes[2] = {'vide', 'text'};
		PicHandle moviePict;
		Rect oldRect;
		OSErr ret;

		if(!Files)return 1;

        if(goCD(Files->directory)){
            sprintf(WarningBuff,"initQUICKTIME Directory %s Could Not Be Found\n",Files->directory);
            WarningBatch(WarningBuff);
        	return 1;
        }

	    ret=HGetVol(NULL,&vRefNum,&dirIDToSearch);
		if(ret){
		    sprintf(WarningBuff,"initQUICKTIME HGetVol error (%ld)\n",(long)ret);
		    WarningBatch(WarningBuff);
		    return 1;
		}
	

		Files->fileClass=QUICKTIME_FILES;
		
		Files->pSetFrame=FilesSetFrameQUICKTIME;

		Files->pdefault=FilesOpenHDF;
		
		FilesNames=Files->FilesNames;
				
		if(Files->loadToMemory){
		    Files->fileType=QUICKTIME_MEMORY;
		}else{
		    Files->fileType=QUICKTIME_DISK;
		}	
				
		count=0;
		ImageCount=0;
		lengthM=0;
		xmin=ymin=200000;
		xmax=ymax=0;
		for(n=0;n<Files->FileCount;++n){
	    	unsigned char *buffer;
	    	int ok;


	    	ok=FALSE;
	    	buffer=NULL;
	    	

		    CopyCStringToPascal((char *)FilesNames[n],pname);

			ret=FSMakeFSSpec(vRefNum,dirIDToSearch,pname,&theFSSpec);
			if(ret){
			    sprintf(WarningBuff,"FSmakeFSSpec Error %ld File %s\n",(long)ret,FilesNames[n]);
			    WarningBatch(WarningBuff);
			    goto ErrorOut;
			}


			ret = OpenMovieFile( &theFSSpec, &refNum, fsRdPerm );
			if(ret){
			    sprintf(WarningBuff,"OpenMovieFile error (%ld)\n",(long)ret);
			    WarningBatch(WarningBuff);
			    goto Loop;
			}
			ret = NewMovieFromFile( &movie, refNum, &resID, NULL, newMovieActive, NULL );
			if(ret){
			    sprintf(WarningBuff,"NewMovieFromFile error (%ld)\n",(long)ret);
			    WarningBatch(WarningBuff);
			    goto ErrorOut;
			}
			ret = CloseMovieFile( refNum );
			if(ret){
			    sprintf(WarningBuff,"CloseMovieFile error (%ld)\n",(long)ret);
			    WarningBatch(WarningBuff);
			    goto ErrorOut;
			}


			inTime=0;
			
			GetMovieNextInterestingTime(movie,nextTimeMediaSample+nextTimeEdgeOK,2, trackTypes,
				                              inTime, forwardNormalSpeed, &inTime, nil);

			GetMovieBox( movie, &oldRect );

	    	
	    	while(1){
	    	
		        
	    		if(CheckQuickTimeSpace(Files))goto ErrorOut;
	    		
	    		xsize=oldRect.right-oldRect.left;
	    		ysize=oldRect.bottom -oldRect.top;
				length=4*xsize*ysize;
	    		if(xsize < 0 || ysize < 0){
	        	    sprintf(WarningBuff,"(%s) Bad Rect xsize %d ysize %d\n",FilesNames[n],xsize,ysize);
       			    WarningBatch(WarningBuff);
	        	    goto ErrorOut;
	    		}
		    	
				if(Files->loadToMemory){
		            moviePict = GetMoviePict(movie,inTime);
				    length=GetHandleSize((Handle)moviePict);
					Files->QUICKTIMEList[ImageCount].buffer=(unsigned char *)moviePict;
				}else{
					Files->QUICKTIMEList[ImageCount].buffer=NULL;
				}

	    		if(length > lengthM)lengthM=length;
	    		
				Files->QUICKTIMEList[ImageCount].length=length;
				
				fp=(unsigned char *)strrchr((char *)FilesNames[n],'.');
				if(fp){
				    *fp = 0;
				    sprintf(imageName,"%s.%04ld%c",FilesNames[n],ImageCount,0);
				    *fp = '.';
				}else{
				    sprintf(imageName,"%s.%04ld%c",FilesNames[n],ImageCount,0);
				}
				Files->ImageNames[ImageCount]=strsave(imageName,8237);
			    if(!Files->ImageNames[ImageCount]){
	       			WarningBatch("initQUICKTIME Out Of Memory\n");
			    	goto ErrorOut;
			    }
				Files->ImageFiles[ImageCount]=n;
				Files->QUICKTIMEList[ImageCount].xsize=xsize;
				Files->QUICKTIMEList[ImageCount].ysize=ysize;
				Files->QUICKTIMEList[ImageCount].inTime=inTime;
				Files->QUICKTIMEList[ImageCount].movie=movie;
				Files->ImageCount = ++ImageCount;
				
				if(xsize > xmax)xmax=xsize;
				if(ysize > ymax)ymax=ysize;
				if(xsize < xmin)xmin=xsize;
				if(ysize < ymin)ymin=ysize;
				
		 		 GetMovieNextInterestingTime(movie,nextTimeMediaSample,2, trackTypes,
		                              inTime, forwardNormalSpeed, &inTime, nil);
				  if ( inTime == -1 ) {	
				      break;
				  }
				
	    	}
				
				
		    	ok=1;
	Loop:


		    	if(ok){
		    	    if(count < n){
		    	        if(FilesNames[count])cFree((char *)FilesNames[count]);
		        		Files->FilesNames[count++]=FilesNames[n];
		        		FilesNames[n]=NULL;
		    	    }else{
		        		Files->FilesNames[count++]=FilesNames[n];
		    	    }
		    	}else{
		        	sprintf(WarningBuff,"Skipping file %s\n",FilesNames[n]);
		       		WarningBatch(WarningBuff);
		        	cFree((char *)FilesNames[n]);
		        	FilesNames[n]=NULL;
		    	}
	    	
	    	
	    	
		}
		Files->FileCount=count;
		Files->FileOpen = -1;
		
		Files->xmin=xmin;
		Files->ymin=ymin;
		Files->xmax=xmax;
		Files->ymax=ymax;

		Files->length=lengthM;

		if(count <= 0)goto ErrorOut;
		
		Files->dataSave=(unsigned char *)cMalloc(lengthM,8130);
		if(!Files->dataSave)goto ErrorOut;
		
		do24BitStart(Files);

		return 0;
ErrorOut:

		freeFileList(Files);		
		return 1;
}

int CheckQuickTimeSpace(struct FileInfo2 *Files)
{
		struct QUICKTIMEI *QUICKTIMEList;
        long ImageMax;
	
		if(!Files)return 1;

		if(Files->ImageCount+1 < Files->ImageMax)return 0;

		if(CheckNames(Files))return 1;

		ImageMax = Files->ImageMax+IMAGE_INCREMENT;

		QUICKTIMEList=NULL;
		if(Files->QUICKTIMEList){
	    	QUICKTIMEList=(struct QUICKTIMEI *)cRealloc((char *)Files->QUICKTIMEList,ImageMax*sizeof(struct QUICKTIMEI),8264);
	    	if(!QUICKTIMEList){
	        	WarningBatch("CheckQuickTimeSpace out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)&QUICKTIMEList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct QUICKTIMEI));
		}else{
	    	QUICKTIMEList=(struct QUICKTIMEI *)cMalloc(ImageMax*sizeof(struct QUICKTIMEI),8131);
	    	if(!QUICKTIMEList){
	        	WarningBatch("CheckQuickTimeSpace out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)QUICKTIMEList,ImageMax*sizeof(struct QUICKTIMEI));
		}

		Files->ImageMax=ImageMax;
		Files->QUICKTIMEList=QUICKTIMEList;


		return 0;
ErrorOut:
		if(QUICKTIMEList){
	    	Files->QUICKTIMEList=QUICKTIMEList;
		}
		return 1;
}

int FilesSetFrameQUICKTIME(long CurrentFrame,struct FileInfo2 *Files)
{
	PicHandle moviePict;
	struct SetFrameData *s;
	unsigned char *bp;
	unsigned long length,Length;
	int xsize,ysize;

	if(!Files)return 1;
	if(Files->fileClass != QUICKTIME_FILES)return 1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	s = &Files->SetData;
	s->SetFrame= -1;
	
	s->name=FilesImageName(Files,CurrentFrame);
	s->xsize=xsize=Files->QUICKTIMEList[CurrentFrame].xsize;
	s->ysize=ysize=Files->QUICKTIMEList[CurrentFrame].ysize;
	s->length=length=Files->QUICKTIMEList[CurrentFrame].length;
	
	if(Files->loadToMemory){
		moviePict=(PicHandle)Files->QUICKTIMEList[CurrentFrame].buffer;
	    Length=GetHandleSize((Handle)moviePict);
		HLock((Handle)moviePict);
		
		uMemcpy((char *)Files->dataSave,(char *)*moviePict,Length);

		HUnlock((Handle)moviePict);
	}else{
	   
	    moviePict = GetMoviePict(Files->QUICKTIMEList[CurrentFrame].movie,Files->QUICKTIMEList[CurrentFrame].inTime);
	    if(!moviePict){
	         WarningBatch("FilesSetFrameQUICKTIME : GetMoviePict Returned NULL\n");
			 s->SetFrame= CurrentFrame;
			 s->type = PICT_FILES;
			 s->data = NULL;
	         return 1;
	    }
	    Length=GetHandleSize((Handle)moviePict);
		HLock((Handle)moviePict);
		
		uMemcpy((char *)Files->dataSave,(char *)*moviePict,Length);

		HUnlock((Handle)moviePict);
		KillPicture(moviePict);
	} 

	bp=(unsigned char *)Files->dataSave;
	s->SetFrame= CurrentFrame;
	s->type = PICT_FILES;
	s->data = bp;
	
	if(Files->inputType == TRUE_EXPANDED){
	    PictToTrue(Files,s);
	}
	
	return 0;
}
int initPICT(struct FileInfo2 *Files)
{
	unsigned char *buffer;
    long n,count,length,ImageCount,lengthM;
    char **FilesNames;
    long xmin,xmax,ymin,ymax;
    int xsize,ysize;
	FILE *in;
	char filepath[2048];

		if(!Files)return 1;

		in=NULL;

		buffer=NULL;
						
		Files->fileClass=PICT_FILES;
		
		Files->pSetFrame=FilesSetFramePICT;

		Files->pdefault=FilesOpenHDF;

		FilesNames=Files->FilesNames;
		
		if(Files->loadToMemory){
		    Files->fileType=PICT_MEMORY;
		}else{
		    Files->fileType=PICT_DISK;
		}	
				
		count=0;
		ImageCount=0;
		lengthM=0;
		xmin=ymin=200000;
		xmax=ymax=0;
		for(n=0;n<Files->FileCount;++n){
	    	int ok;


	    	ok=FALSE;
	    	buffer=NULL;
	    	
	    	mstrncpy(filepath,Files->directory,2048);
	    	strncatToPath(filepath,FilesNames[n],2048);

		    in=fopen(filepath,"rb");
		    if(in == NULL){
		       sprintf(WarningBuff,"Could Not Open %s To Read\n",filepath);
		       WarningBatch(WarningBuff);
		       goto Loop;
		    }
		    
		    if(fseek(in,0L,2)){
		        sprintf(WarningBuff,"(%s) Error Looking For End Of File\n",FilesNames[n]);
		        WarningBatch(WarningBuff);
		        goto Loop;
		    }

		    if((length=ftell(in)) == -1L){
		        sprintf(WarningBuff,"(%s) Error Reading For End Of File Location\n",FilesNames[n]);
		        WarningBatch(WarningBuff);
		        goto Loop;
		    }
		    
		    length -= 512;
		    
		     if(length <= 0){
			  	sprintf(WarningBuff,"File : %s is Not a PICT\n",FilesNames[n]);
		        WarningBatch(WarningBuff);
		         goto ErrorOut;
		     }
		    

	    	if(length > lengthM)lengthM=length;

	    	if(CheckPictSpace(Files))goto ErrorOut;
	    	
	    	if(fseek(in,512L,0)){
	        	sprintf(WarningBuff,"(%s) Error Rewinding File\n",FilesNames[n]);
       			WarningBatch(WarningBuff);
	        	goto ErrorOut;
	    	}
	    	buffer=(unsigned char *)cMalloc(length,8129);
	    	if(!buffer)goto ErrorOut;
	    	if(getString(buffer,length,in))goto ErrorOut;
	    	
			{
				Rect oldRect;
			
	    		/* oldRect=(*((Rect *)(buffer+2))); */
	    		
	    		oldRect.top=   256*buffer[2]  + buffer[3];
	    		oldRect.left=  256*buffer[4]  + buffer[5];
	    		oldRect.bottom=256*buffer[6]  + buffer[7];
	    		oldRect.right= 256*buffer[8]  + buffer[9];
	    		xsize=oldRect.right-oldRect.left;
	    		ysize=oldRect.bottom -oldRect.top;
	    		if(xsize <= 0 || ysize <= 0){
	        	    sprintf(WarningBuff,"(%s) Bad Rect xsize %d ysize %d\n",FilesNames[n],xsize,ysize);
       			    WarningBatch(WarningBuff);
	        	    goto ErrorOut;
	    		}
			}
	    	
	    	
			if(Files->loadToMemory){
				Files->PICTList[ImageCount].buffer=buffer;
			}else{
				Files->PICTList[ImageCount].buffer=NULL;
				if(buffer)cFree((char *)buffer);
			}
			buffer=NULL;

			Files->PICTList[ImageCount].length=length;
			Files->ImageNames[ImageCount]=strsave(FilesNames[n],8237);
		    if(!Files->ImageNames[ImageCount]){
       			WarningBatch("initPICT Out Of Memory\n");
		    	goto ErrorOut;
		    }
			Files->ImageFiles[ImageCount]=n;
			Files->PICTList[ImageCount].xsize=xsize;
			Files->PICTList[ImageCount].ysize=ysize;
			Files->ImageCount = ++ImageCount;
			
			if(xsize > xmax)xmax=xsize;
			if(ysize > ymax)ymax=ysize;
			if(xsize < xmin)xmin=xsize;
			if(ysize < ymin)ymin=ysize;
			
			
			
	    	ok=1;
Loop:
		    if(in)fclose(in); 
		    in=NULL;

	    	if(buffer)cFree((char *)buffer);
	    	buffer=NULL;

	    	if(ok){
	    	    if(count < n){
	    	        if(FilesNames[count])cFree((char *)FilesNames[count]);
	        		Files->FilesNames[count++]=FilesNames[n];
	        		FilesNames[n]=NULL;
	    	    }else{
	        		Files->FilesNames[count++]=FilesNames[n];
	    	    }
	    	}else{
	        	sprintf(WarningBuff,"Skipping file %s\n",FilesNames[n]);
	       		WarningBatch(WarningBuff);
	        	cFree((char *)FilesNames[n]);
	        	FilesNames[n]=NULL;
	    	}
		}
		Files->FileCount=count;
		Files->FileOpen = -1;
		
		Files->xmin=xmin;
		Files->ymin=ymin;
		Files->xmax=xmax;
		Files->ymax=ymax;

		Files->length=lengthM;

		if(count <= 0)goto ErrorOut;
		
		if(!Files->loadToMemory){
			Files->dataSave=(unsigned char *)cMalloc(lengthM,8130);
			if(!Files->dataSave)goto ErrorOut;
		}

		do24BitStart(Files);

		return 0;
ErrorOut:
	    if(buffer)cFree((char *)buffer);
		if(in)fclose(in);
		freeFileList(Files);		
		return 1;
}

int CheckPictSpace(struct FileInfo2 *Files)
{
		struct PICTI *PICTList;
        long ImageMax;
	
		if(!Files)return 1;

		if(Files->ImageCount+1 < Files->ImageMax)return 0;

		if(CheckNames(Files))return 1;

		ImageMax = Files->ImageMax+IMAGE_INCREMENT;

		PICTList=NULL;
		if(Files->PICTList){
	    	PICTList=(struct PICTI *)cRealloc((char *)Files->PICTList,ImageMax*sizeof(struct PICTI),8264);
	    	if(!PICTList){
	        	WarningBatch("CheckPictSpace out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)&PICTList[Files->ImageMax],IMAGE_INCREMENT*sizeof(struct PICTI));
		}else{
	    	PICTList=(struct PICTI *)cMalloc(ImageMax*sizeof(struct PICTI),8131);
	    	if(!PICTList){
	        	WarningBatch("CheckPictSpace out of Memory\n");
	        	goto ErrorOut;
	    	}
	    	zerol((char *)PICTList,ImageMax*sizeof(struct PICTI));
		}

		Files->ImageMax=ImageMax;
		Files->PICTList=PICTList;

		return 0;
ErrorOut:
		if(PICTList){
	    	Files->PICTList=PICTList;
		}
		return 1;
}


int FilesSetFramePICT(long CurrentFrame,struct FileInfo2 *Files)
{
	struct SetFrameData *s;
	unsigned char *bp;
	unsigned long length;
	int xsize,ysize;

	if(!Files)return 1;
	if(Files->fileClass != PICT_FILES)return 1;
	
	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	s = &Files->SetData;
	s->SetFrame= -1;
	
	s->name=FilesImageName(Files,CurrentFrame);
	s->xsize=xsize=Files->PICTList[CurrentFrame].xsize;
	s->ysize=ysize=Files->PICTList[CurrentFrame].ysize;
	s->length=length=Files->PICTList[CurrentFrame].length;
	
	if(Files->loadToMemory){
		bp=(unsigned char *)Files->PICTList[CurrentFrame].buffer;
	}else{
        char name[1024];

        if(Files->directory[0]){
            mstrncpy(name,Files->directory,1024);
        }else{
            name[0]=0;
        }
        if(Files->FilesNames && Files->FilesNames[CurrentFrame])
                  strncatToPath(name,Files->FilesNames[CurrentFrame],1024);
        FixPathName(name);
        Files->in = fopen(name,"rb");
        if(Files->in == NULL){
	        sprintf(WarningBuff,"Could Not Open %s To Read\n",name);
            WarningBatch(WarningBuff);
            return 1;
        }
        
    	if(fseek(Files->in,512L,0)){
        	sprintf(WarningBuff,"(%s) Error Rewinding File\n",name);
   			WarningBatch(WarningBuff);
            return 1;
    	}
	       
	   	   
	   if(getString((unsigned char *)Files->dataSave,length,Files->in))return 1;
	   
	   if(Files->in)fclose(Files->in);
	   Files->in=NULL;
	   
	   bp=(unsigned char *)Files->dataSave;
	   
	} 

	s->SetFrame= CurrentFrame;
	s->type = PICT_FILES;
	s->data = bp;
	if(Files->inputType == TRUE_EXPANDED){
	    PictToTrue(Files,s);
	}
	return 0;
}
int FilesMergePICT(struct FileInfo2 *Files,struct FileInfo2 *FilesNew)
{
	long ImageCount;
	long xmax,ymax,xsize,ysize;
	int ret;
	long n;
	
	if(!Files || !FilesNew)return 1;
	if(Files->fileClass != PICT_FILES)return 1;
	if(FilesNew->fileClass != PICT_FILES)return 1;
	
	ret=1;
	ImageCount=Files->ImageCount;
	for(n=0;n<FilesNew->ImageCount;++n){
		if(CheckPictSpace(Files))goto ErrorOut;
		Files->PICTList[ImageCount]=FilesNew->PICTList[n];
		
		FilesNew->PICTList[n].buffer=NULL;
				
		Files->ImageNames[ImageCount]=FilesNew->ImageNames[n];
		FilesNew->ImageNames[n]=NULL;
		Files->ImageFiles[ImageCount]=0;
		xsize=Files->PICTList[ImageCount].xsize;
		ysize=Files->PICTList[ImageCount].ysize;
		
		Files->ImageCount = ++ImageCount;	
		
		xmax=Files->xmax;
		ymax=Files->ymax;
		if(xsize < Files->xmin)Files->xmin=xsize;
		if(xsize > Files->xmax)Files->xmax=xsize;
		if(ysize < Files->ymin)Files->ymin=ysize;
		if(ysize > Files->ymax)Files->ymax=ysize;
		if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
		    if(Files->dataSave)cFree((char *)Files->dataSave);
		    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
		    Files->dataSave=(unsigned char *)cMalloc(Files->length,8112);
		    if(Files->dataSave == NULL){
		        goto ErrorOut;
		    }
		    Files->CurrentFrame = -1;
		}		
		
	}
		
	if(Files->ImageCount >= 1)do24BitStart(Files);	
	ret=0;
ErrorOut:

	if(FilesNew)freeFileList(FilesNew);
	
	return ret;	
}

int FileAppendPICT(char *name,Handle PICThand,struct FileInfo2 **iFiles)
{
	struct FileInfo2 *Files;
	unsigned char *buffer;
	long ImageCount,xmax,ymax;
	long xsize,ysize;
	long Length;
	int ret;


	if(!name || !iFiles)return 1;
	
	ret=1;

	Files= *iFiles;
	
	if(Files){
	    if(Files->fileClass != PICT_FILES){
	       freeFileList(Files);
	       *iFiles=Files=NULL;
	    }
	}
	
	if(!Files){
		Files=GetFileInfoStruct(8132);
	    if(!Files){
	        WarningBatch("FileAppendPICT out of Memory\n");
	        goto ErrorOut;
	    }
	    	    
	    Files->loadToMemory=1;
	    Files->fileClass = PICT_FILES;
	    Files->fileType=PICT_MEMORY;
	    Files->CurrentFrame = -1;
		Files->pSetFrame=FilesSetFramePICT;
	}
	if(CheckPictSpace(Files))goto ErrorOut;
	Length=GetHandleSize(PICThand);
	buffer=(unsigned char *)cMalloc(Length,8133);
	if(!buffer){
	    WarningBatch("FileAppendPICT out of Memory\n");
	    goto ErrorOut;
	}
	HLock((Handle)PICThand);
	uMemcpy(buffer,*PICThand,Length);
	HUnlock((Handle)PICThand);
	DisposeHandle(PICThand);

	{
			Rect oldRect;
			
    		oldRect.top=   256*buffer[2]  + buffer[3];
    		oldRect.left=  256*buffer[4]  + buffer[5];
    		oldRect.bottom=256*buffer[6]  + buffer[7];
    		oldRect.right= 256*buffer[8]  + buffer[9];
	    		
	    	xsize=oldRect.right-oldRect.left;
	    	ysize=oldRect.bottom -oldRect.top;
	}

	
	ImageCount=Files->ImageCount;
	Files->PICTList[ImageCount].buffer=buffer;
	Files->PICTList[ImageCount].length=Length;
	Files->PICTList[ImageCount].xsize=xsize;
	Files->PICTList[ImageCount].ysize=ysize;
	Files->ImageNames[ImageCount]=strsave(name,8238);
	if(Files->ImageNames[ImageCount] == NULL){
	    WarningBatch("FileAppendPICT out of Memory\n");
	    goto ErrorOut;
	}
	Files->ImageFiles[ImageCount]=0;
	Files->ImageCount = ++ImageCount;

	xmax=Files->xmax;
	ymax=Files->ymax;
	if(xsize < Files->xmin)Files->xmin=xsize;
	if(xsize > Files->xmax)Files->xmax=xsize;
	if(ysize < Files->ymin)Files->ymin=ysize;
	if(ysize > Files->ymax)Files->ymax=ysize;
	if(!Files->dataSave || (xsize > xmax) || (ysize > ymax)){
	    if(Files->dataSave)cFree((char *)Files->dataSave);
	    Files->length=(long)(Files->ymax*Files->xmax*sizeof(double));
	    Files->dataSave=(unsigned char *)cMalloc(Files->length,8134);
	    if(Files->dataSave == NULL){
	    	WarningBatch("FileAppendPICT out of Memory\n");
	        goto ErrorOut;
	    }
	    Files->CurrentFrame = -1;
	}
	
	if(Files->ImageCount == 1)do24BitStart(Files);

	*iFiles=Files;
	
	ret=0;
	
ErrorOut:
	return ret;
}
int IsQUICKTIME(char *path)
{
	unsigned char name[255];
	FInfo info2;   
	FSSpec file;
	OSErr theErr;

	if(!path)return 0;

	if(IsMOOV(path)){
	    return 1;
	}
	mstrncpy ((char *)name, path,255);
	CopyCStringToPascal((char *)name,name);	
	theErr=FSMakeFSSpec(0,0L,name, &file);
	if(theErr){
		return 0;
	}

	theErr = FSpGetFInfo(&file, &info2 );              
	if(theErr){
		return 0;
	}
	if(info2.fdType == MovieFileType){
		return 1;
	}

	return 0;
}
int IsPICT(char *path)
{
    Boolean isDirectory;
	unsigned char name[255];
	GraphicsImportComponent importer = 0;
	FInfo info2;   
	FSSpec file;
	OSErr theErr;
    FSRef ref;

	if(!path)return 0;

	if(IsMOOV(path)){
	    return 1;
	}
	
	mstrncpy ((char *)name, path,255);
	CopyCStringToPascal((char *)name,name);	
	theErr=FSMakeFSSpec(0,0L,name, &file);
	if(theErr){
		theErr = FSPathMakeRef ((UInt8 *) path, &ref, &isDirectory);
	    if (theErr == noErr) {
	        theErr = FSGetCatalogInfo (&ref, kFSCatInfoNone, NULL, NULL, &file, NULL);
		}
	}
	
	theErr = FSpGetFInfo(&file, &info2 );              
	if(theErr){
		return 0;
	}
	
	if(info2.fdType == kQTFileTypePicture){
		return 1;
	}
			
	theErr = GetGraphicsImporterForFileWithFlags(&file,&importer,0);
	if(theErr == noErr){
	    ImageDescriptionHandle desc;
	    CodecType cType;
	    desc=NULL;
		theErr = GraphicsImportGetImageDescription(importer,&desc);
	    CloseComponent(importer);
	    if(desc && *desc){
	        cType=(*desc)->cType;
	    	DisposeHandle( (Handle)desc );
	    	if(cType == kQuickDrawCodecType)return 1;
	    }
	}
	
	return 0;
}


#else

int IsQUICKTIME(char *path)
{
	if(!path)return 0;
	return 0;
}

int IsPICT(char *path)
{
	if(!path)return 0;
	return 0;
}

int initPICT(struct FileInfo2 *Files)
{
	if(!Files)return 1;
	return 1;
}
int FilesMergePICT(struct FileInfo2 *Files,struct FileInfo2 *FilesNew)
{
	if(!Files || !FilesNew)return 1;
	return 1;
}

int OpenViewLib(struct FileList *Files);

int OpenViewLib(struct FileList *Files)
{

	if(!Files)return 1;
	
	
	return 0;
}

#endif
