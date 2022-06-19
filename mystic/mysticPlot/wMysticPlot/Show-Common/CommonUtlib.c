#define EXTERN22 extern
#include "Xappl.h"
#include "uFiles.h"
#include "uDialog.h"
#include "FileManager.h"
#include "Message.h"
#include "Universal.h"
#include "uGraph.h"
#include "uNetWork.h"

long CountRelativePath(struct FileList *Files);
char *ConvertPath(char *namein,int flag);

IconPtr NewPIO3DWindow(IconPtr myIcon);
IconPtr NewPIO2DWindow(IconPtr myIcon);
IconPtr NewNetWindow(IconPtr myIcon);
IconPtr NewFileSendWindow(IconPtr myIcon);
IconPtr NewTekWindow(IconPtr myIcon);

IconPtr NewCEdit(IconPtr myIcon,struct FileList *files);
IconPtr NewRenderWindow(IconPtr myIcon);
IconPtr NewTableWindow(IconPtr myIcon);
IconPtr macUtilities(IconPtr myIcon);
IconPtr NewDrawWindow(IconPtr myIcon);

static int doScale2Action(struct uDialogStruct *uList);
static int doScale1Action(struct uDialogStruct *uList);

IconPtr  NewVolumetricWindow(IconPtr myIcon);
int BatchFile(void);
int doAbout(IconPtr myIcon);
int doQuit(void);
 
int doHistogram(struct linedata *li);

int doBhangMeter(struct linedata *li);

int doFiddleColors(IconPtr myIcon);

long CountFilesInList(struct FileList *Files);

unsigned char *saveblock(unsigned char *buffin,long length,int tag);
int CreateNewDirectory(void);
int MakeNewDirectory(char *name);


#define infoList_Length  2000
static struct FileListInfo *infoList[infoList_Length];
static int infoListCount;

static int doScale2Start(struct uDialogStruct *uList);
int uRotateRect(uRect *Rect,unsigned char **image,double angle);

int uInsetRect(uRect *r,int x,int y);
static int xBuff,yBuff;   

int OpenViewLib(struct FileList *Files);

IconPtr FindTypeWindow(int Type);

int SynchronizePalettes(IconPtr myIcon,unsigned char *red,unsigned char *green,unsigned char *blue);

int sear(double cmax,double cmin,double *zcon,int *ncon,int nmax);

int SetWindowToFileName(IconPtr myIcon,struct FileInfo2 *Files);

int doEraseDown(IconPtr myIcon);

int FileListFree(struct FileList *Files);

struct FileList *uOpenFileDialogLocal(struct Icon *myIcon,int *TypeList);

int doLineRange(struct linedata *li);
static int doPointRange(struct linedata *li);

int WriteRectToRect(unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize);
                    

int FontToScreen(uRect *cRect,unsigned char *image,int nc,struct screenData *s);
int FontToRect(char *out,uRect *cRect,unsigned char **image,IconPtr myIcon);

int doTestSpeed(IconPtr myIcon);

int doMouseSweepDown(IconPtr myIcon);

int doMouseDown(IconPtr myIcon);

int doCursorDisplay(IconPtr myIcon);

extern int crctab[];

IconPtr NewNewWindow(IconPtr myIcon);

static int uNewWindowAction(struct uDialogStruct *uList);

static int uNewWindowClose(IconPtr myIcon);


int FindFilesInList(struct FileList *Files);


char DefaultPaletteName[256]={'p','s',0};

int uOffsetRect(uRect *LineRect,int x,int y);

static int doScale5Start(struct uDialogStruct *uList);
static int doScale5Action(struct uDialogStruct *uList);

static int doScale4Move(struct uDialogStruct *uList);
static int doScale4Start(struct uDialogStruct *uList);
static int doScale4Action(struct uDialogStruct *uList);
static int doScale4SetButtons(struct uDialogStruct *uList);
int doCursorItems(char *BatchName,struct CursorStruct *CursorList,IconPtr myIcon);
int uWriteCursorItems(IconPtr myIcon,long nn,uRect *dataRect2);
int doGetCursorData(IconPtr myIcon,char *BatchName,struct SetFrameData  *sd);

struct passItems{
	struct CursorStruct *CursorList;
	char *BatchName;
};

int doCursorFree(struct CursorStruct *Cursor);


int IsOption(void);

int	uGetCursorItemsString(char *buffout,long sizebuff,IconPtr myIcon,long nn);

int writeCursorString(uRect *dataRect,char *buffout,IconPtr myIcon);

int uCStringWidth(char *name,IconPtr myIcon);

#define MaxIncrement 200

int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag);

int doFourier(double *x,double *y,long nodes,int Order,int PolynomialPrint);
int doSmooth(double *x,double *y,long n,int Order,int PolynomialPrint);
int doPolynomial(double *x,double *y,long n,int Order,int PolynomialPrint);
int solve(double *a,double *f,double *x,int n,int iflag);
int fitit(double *x,double *y,long nodes,int Order,double *a,double *b,double w);

struct buffStuff{
   char *buffer;
   long length;
   long lengthMax;
};


int doLineToBuff(char *buff,struct buffStuff *buffer,int flag);




int WriteRect8ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize,
                       unsigned char *palette);
                       
int WriteRect24ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize);

int kpointPattern(int x,int y,struct screenData *s);

static uPattern  Patterns[] = {
	{{0xff ,0xff, 0xff, 0xff, 0xff ,0xff, 0xff, 0xff}},				
	{{0xdd ,0xff, 0x77, 0xff, 0xdd ,0xff, 0x77, 0xff}},				
	{{0xdd ,0x77, 0xdd, 0x77, 0xdd ,0x77, 0xdd, 0x77}},				
	{{0xaa ,0x55, 0xaa, 0x55, 0xaa ,0x55, 0xaa, 0x55}},				
	{{0x55 ,0xff, 0x55, 0xff, 0x55 ,0xff, 0x55, 0xff}},				
	{{0xaa ,0xaa, 0xaa, 0xaa, 0xaa ,0xaa, 0xaa, 0xaa}},				   
	{{0xee ,0xdd, 0xbb, 0x77, 0xee ,0xdd, 0xbb, 0x77}},				
	{{0x88 ,0x88, 0x88, 0x88, 0x88 ,0x88, 0x88, 0x88}},				   
	{{0xb1 ,0x30, 0x03, 0x1b, 0xd8 ,0xc0, 0x0c, 0x8d}},				   
	{{0x80 ,0x10, 0x02, 0x20, 0x01 ,0x08, 0x40, 0x04}},				
	{{0xff ,0x88, 0x88, 0x88, 0xff ,0x88, 0x88, 0x88}},				
	{{0xff ,0x80, 0x80, 0x80, 0xff ,0x08, 0x08, 0x08}},				
	{{0x80 ,0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00}},				   
	{{0x80 ,0x40, 0x20, 0x00, 0x02 ,0x04, 0x08, 0x00}},				 
	{{0x82 ,0x44, 0x39, 0x44, 0x82 ,0x01, 0x01, 0x01}},				   
	{{0xf8 ,0x74, 0x22, 0x47, 0x8f ,0x17, 0x22, 0x71}},				
	{{0x55 ,0xa0, 0x40, 0x40, 0x55 ,0x0a, 0x04, 0x04}},				
	{{0x20 ,0x50, 0x88, 0x88, 0x88 ,0x88, 0x05, 0x02}},				
	{{0xbf ,0x00, 0xbf, 0xbf, 0xb0 ,0xb0, 0xb0, 0xb0}},				
	{{0x00 ,0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00}},				
	{{0x80 ,0x00, 0x08, 0x00, 0x80 ,0x00, 0x08, 0x00}},				   
	{{0x88 ,0x00, 0x22, 0x00, 0x88 ,0x00, 0x22, 0x00}},				
	{{0x88 ,0x22, 0x88, 0x22, 0x88 ,0x22, 0x88, 0x22}},				
	{{0xaa ,0x00, 0xaa, 0x00, 0xaa ,0x00, 0xaa, 0x00}},				
	{{0xff ,0x00, 0xff, 0x00, 0xff ,0x00, 0xff, 0x00}},				   
	{{0x11 ,0x22, 0x44, 0x88, 0x11 ,0x22, 0x44, 0x88}},				
	{{0xff ,0x00, 0x00, 0x00, 0xff ,0x00, 0x00, 0x00}},				   
	{{0x01 ,0x02, 0x04, 0x08, 0x10 ,0x20, 0x40, 0x80}},				
	{{0xaa ,0x00, 0x80, 0x00, 0x88 ,0x00, 0x80, 0x00}},				
	{{0xff ,0x80, 0x80, 0x80, 0x80 ,0x80, 0x80, 0x80}},				   
	{{0x08 ,0x1c, 0x22, 0xc1, 0x80 ,0x01, 0x02, 0x04}},				   
	{{0x88 ,0x14, 0x22, 0x41, 0x88 ,0x00, 0xaa, 0x00}},				
	{{0x40 ,0xa0, 0x00, 0x00, 0x04 ,0x0a, 0x00, 0x00}},				
	{{0x03 ,0x84, 0x48, 0x30, 0x0c ,0x02, 0x01, 0x01}},				
	{{0x80 ,0x80, 0x41, 0x3e, 0x08 ,0x08, 0x14, 0xe3}},				   
	{{0x10 ,0x20, 0x54, 0xaa, 0xff ,0x02, 0x04, 0x08}},				
	{{0x77 ,0x89, 0x8f, 0x8f, 0x77 ,0x98, 0xf8, 0xf8}},				
	{{0x00 ,0x08, 0x14, 0x2a, 0x55 ,0x2a, 0x14, 0x08}},				
};

extern IconPtr TableWindow2(int XCount,int YCount,int XLength,int YLength);

int doPaletteToTable(IconPtr myIcon);

int checkStreamData(struct dataStruct *plane);

#define POINT_INCREMENT 10

int freeStreamLine3d(struct PlotStreamData *streamline3d);

int freeStreamLine2d(struct PlotStreamData *streamline2d);

int freeBhangMeter2d(struct BhangMeterData *streamline2d);

int checkBhangMeterData(struct dataStruct *plane);

struct dataStructHold{
    long count;
    long countMax;
    char **message;
};

static struct dataStructHold BatchHold;

static int checkBatchHold(struct dataStructHold *plane);

int makeCommand(char *command, char *file, char *Application, int length);

int makeCommand(char *command, char *file, char *Application, int length)
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

int WarningBatchHoldDump(void)
{
	struct dataStructHold *b;
	long n;

	b=&BatchHold;
	
	if(!b->message || (b->count <= 0))return 1;
	
	
	for(n=0;n<b->count;++n){
		if(b->message[n]){
		    WarningBatch(b->message[n]);
		    cFree((char *)b->message[n]);
		    b->message[n]=NULL;
		}
	}
	
	cFree((char *)b->message);
	
	b->message=NULL;
	b->countMax=0;
	b->count=0;
		
	return 0;
	
}
int WarningBatchHold(char *buff)
{
	struct dataStructHold *b;
	
	if(!buff)return 1;
	
	b=&BatchHold;
	
	if(checkBatchHold(b))return 1;
	
	b->message[b->count]=strsave(buff,1974);
	
	if(!b->message[b->count])return 1;
	
	++b->count;
	
	return 0;
}
static int checkBatchHold(struct dataStructHold *plane)
{
    long countMax;
	char **message;
	if(!plane)return 1;
	
	if(plane->count+1 < plane->countMax)return 0;
	
	countMax=plane->countMax+POINT_INCREMENT;
	
	message=NULL;
	
	if(plane->message){
	    message=(char **)cRealloc((char *)plane->message,countMax*sizeof(char **),7761);
	    if(!message){
	        goto ErrorOut;
	    }
	    zerol((char *)&message[plane->countMax],POINT_INCREMENT*sizeof(char **));
	}else{
	    message=(char **)cMalloc(countMax*sizeof(char **),7452);
	    if(!message){
	        goto ErrorOut;
	    }
	    zerol((char *)message,countMax*sizeof(char **));
	}
	
	plane->countMax=countMax;
	plane->message=message;
	
	return 0;
ErrorOut:
    if(message)cFree((char *)message);
	return 1;
	
}	

int freeStreamLine3d(struct PlotStreamData *streamline3d)
{
	if(!streamline3d)return 1;
	
	streamline3d->xPlane.count=0;
	streamline3d->xPlane.countMax=0;

	streamline3d->yPlane.count=0;
	streamline3d->yPlane.countMax=0;

	streamline3d->zPlane.count=0;
	streamline3d->zPlane.countMax=0;

	if(streamline3d->xPlane.x)cFree((char *)streamline3d->xPlane.x);
	streamline3d->xPlane.x=NULL;
	
	if(streamline3d->xPlane.y)cFree((char *)streamline3d->xPlane.y);
	streamline3d->xPlane.y=NULL;
	
	
	if(streamline3d->yPlane.x)cFree((char *)streamline3d->yPlane.x);
	streamline3d->yPlane.x=NULL;
	
	if(streamline3d->yPlane.y)cFree((char *)streamline3d->yPlane.y);
	streamline3d->yPlane.y=NULL;
	
	
	if(streamline3d->zPlane.x)cFree((char *)streamline3d->zPlane.x);
	streamline3d->zPlane.x=NULL;
	
	if(streamline3d->zPlane.y)cFree((char *)streamline3d->zPlane.y);
	streamline3d->zPlane.y=NULL;
	
	
	return 0;
}
int freeStreamLine2d(struct PlotStreamData *streamline2d)
{
	if(!streamline2d)return 1;
	
	streamline2d->zPlane.count=0;
	streamline2d->zPlane.countMax=0;
	
	
	if(streamline2d->zPlane.x)cFree((char *)streamline2d->zPlane.x);
	streamline2d->zPlane.x=NULL;
	
	if(streamline2d->zPlane.y)cFree((char *)streamline2d->zPlane.y);
	streamline2d->zPlane.y=NULL;
	
	
	return 0;
}

int freeBhangMeter2d(struct BhangMeterData *bhangmeter2d)
{
	if(!bhangmeter2d)return 1;
	
	bhangmeter2d->xyz.count=0;
	bhangmeter2d->xyz.countMax=0;
	
	
	if(bhangmeter2d->xyz.x)cFree((char *)bhangmeter2d->xyz.x);
	bhangmeter2d->xyz.x=NULL;
	
	if(bhangmeter2d->xyz.y)cFree((char *)bhangmeter2d->xyz.y);
	bhangmeter2d->xyz.y=NULL;
	
	if(bhangmeter2d->xyz.z)cFree((char *)bhangmeter2d->xyz.z);
	bhangmeter2d->xyz.z=NULL;
	
	if(bhangmeter2d->xyz.v)cFree((char *)bhangmeter2d->xyz.v);
	bhangmeter2d->xyz.v=NULL;
	
	
	return 0;
}


int checkBhangMeterData(struct dataStruct *plane)
{
    long countMax;
	double *x;
	double *y;
	double *z;
	double *v;
	if(!plane)return 1;
	
	if(plane->count+1 < plane->countMax && plane->x && plane->y)return 0;
	
	countMax=plane->countMax+POINT_INCREMENT;
	
	x=NULL;
	y=NULL;
	z=NULL;
	v=NULL;
	
	if(plane->x){
	    x=(double *)cRealloc((char *)plane->x,countMax*sizeof(double),7761);
	    if(!x){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&x[plane->countMax],POINT_INCREMENT*sizeof(double));
	}else{
	    x=(double *)cMalloc(countMax*sizeof(double),7452);
	    if(!x){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)x,countMax*sizeof(double));
	}
	
	
	if(plane->y){
	    y=(double *)cRealloc((char *)plane->y,countMax*sizeof(double),9761);
	    if(!y){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&y[plane->countMax],POINT_INCREMENT*sizeof(double));
	}else{
	    y=(double *)cMalloc(countMax*sizeof(double),9452);
	    if(!y){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)y,countMax*sizeof(double));
	}
	
	if(plane->z){
	    z=(double *)cRealloc((char *)plane->z,countMax*sizeof(double),9761);
	    if(!z){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&z[plane->countMax],POINT_INCREMENT*sizeof(double));
	}else{
	    z=(double *)cMalloc(countMax*sizeof(double),9452);
	    if(!z){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)z,countMax*sizeof(double));
	}
	
	if(plane->v){
	    v=(double *)cRealloc((char *)plane->v,countMax*sizeof(double),9761);
	    if(!v){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&v[plane->countMax],POINT_INCREMENT*sizeof(double));
	}else{
	    v=(double *)cMalloc(countMax*sizeof(double),9452);
	    if(!v){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)v,countMax*sizeof(double));
	}
	
	plane->countMax=countMax;
	plane->x=x;
	plane->y=y;
	plane->z=z;
	plane->v=v;
	
	return 0;
ErrorOut:
    if(x)cFree((char *)x);
    if(y)cFree((char *)y);
    if(z)cFree((char *)z);
    if(v)cFree((char *)v);
	return 1;
	
}	


int checkStreamData(struct dataStruct *plane)
{
    long countMax;
	double *x;
	double *y;
	if(!plane)return 1;
	
	if(plane->count+1 < plane->countMax && plane->x && plane->y)return 0;
	
	countMax=plane->countMax+POINT_INCREMENT;
	
	x=NULL;
	y=NULL;
	
	if(plane->x){
	    x=(double *)cRealloc((char *)plane->x,countMax*sizeof(double),7761);
	    if(!x){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&x[plane->countMax],POINT_INCREMENT*sizeof(double));
	}else{
	    x=(double *)cMalloc(countMax*sizeof(double),7452);
	    if(!x){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)x,countMax*sizeof(double));
	}
	
	
	if(plane->y){
	    y=(double *)cRealloc((char *)plane->y,countMax*sizeof(double),9761);
	    if(!y){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&y[plane->countMax],POINT_INCREMENT*sizeof(double));
	}else{
	    y=(double *)cMalloc(countMax*sizeof(double),9452);
	    if(!y){
	        sprintf(WarningBuff,"checkStreamData out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)y,countMax*sizeof(double));
	}
	
	plane->countMax=countMax;
	plane->x=x;
	plane->y=y;
	
	return 0;
ErrorOut:
    if(x)cFree((char *)x);
    if(y)cFree((char *)y);
	return 1;
	
}	
int putENDOFLINE(FILE *out)
{
	unsigned char cr[2];
	int ncout;
	
	if(!out)return 1;

	ncout=1;
	if(uPref.TextOutputFormat == WindowsTextType){
		cr[0]=0x0D;
		cr[1]=0x0A;
		ncout=2;
	}else if(uPref.TextOutputFormat == UnixTextType){
		cr[0]=0x0A;
		cr[1]=0;
	}else{
		cr[0]=0x0D;
		cr[1]=0;
	}

	return putString(cr,ncout,out);
	
}

int copyInfo(struct FileListInfo *in,struct FileListInfo *out)
{
	if(!in || !out)return 1;
	
	if(out->directory)cFree((char *)out->directory);
	out->directory=strsave(in->directory,8806);
	if(!out)return 1;

	return 0;
}

int setInfo(char *in,struct FileListInfo *out)
{
	if(!in || !out)return 1;
	
	if(out->directory)cFree((char *)out->directory);
	out->directory=strsave(in,8806);
	if(!out)return 1;

	return 0;
}

int doPaletteToTable(IconPtr myIcon)
{
	struct SetFrameData  sd;
	double *d;
	IconPtr p;
	int k;
	
	if(!myIcon)return 1;

	zerol((char *)&sd,sizeof(struct SetFrameData));
	
	p=TableWindow2((int)3,(int)256,85,16);
	if(p == NULL){
		sprintf(WarningBuff,"doPaletteToTable - Error Opening Palette Table\n");
		WarningBatch(WarningBuff);
		goto ErrorOut;    	 
	}
	
	SetWindowName(p,myIcon->DefaultPaletteName);	
	
    sd.type=FRAME_DATA_FLOAT;

    sd.xsize=3;
    sd.ysize=256;
    sd.zsize=0;
    sd.data=cMalloc(sd.xsize*sd.ysize*sizeof(double),9991);
    if(!sd.data)goto ErrorOut;

	d=(double *)sd.data;
	
	for(k=0;k<256;++k){
    	d[k*3]=myIcon->red[k];
     	d[k*3+1]=myIcon->green[k];
     	d[k*3+2]=myIcon->blue[k];
	}
    	        	        	        
	sd.CurrentFrame=0;	    			    	
	if(SendMessageByIcon(p,MessageType_SET_REGION_DATA,&sd)){	        	        
		sprintf(WarningBuff,"doPaletteToTable - Error Puting Palette Data\n");
		WarningBatch(WarningBuff);
		goto ErrorOut;
    }
	
ErrorOut:
    if(sd.data)cFree((char *)sd.data);
	return 0;
}
int uPointerGlobal(uPoint *uwhere,IconPtr myIcon)
{	
	uRect r;

	if(!uwhere || !myIcon)return 1;
	
	uPointer(uwhere,myIcon);
	uGetPortBoundsGlobal(myIcon,&r);
	uwhere->x += r.x;
	uwhere->y += r.y;
	
	return 0;
}

uPattern *PatternPattern(int n)
{
	
	if(n < 0 || n > sizeof(Patterns)/sizeof(uPattern)){
	    n=1;
	}
	return &Patterns[n];
}                       
int PatternCount(void)
{
	return sizeof(Patterns)/sizeof(uPattern);
}            
int uImageRectPattern(iRect *ir,uPattern *pat,IconPtr myIcon)
{
	int i,j,k,n,m;
	unsigned char *out,p;
	int ForeColorIndex;
	
	if(!ir || !pat)return 1;
	
    out=(unsigned char *)ir->data;
    
    ForeColorIndex=myIcon->ForeColorIndex;
    
    n=0;
    m=0;
    for(j=0;j<ir->box.ysize;++j){
        p=pat->pat[m];
        k=0;
        for(i=0;i<ir->box.xsize;++i){
           if((p << k) & 128){
               out[n++]=ForeColorIndex;
           }else{
               n++;
           }
           if(++k >= 8)k=0;
        }
        if(++m >= 8)m=0;
    }
	
	return 0;
}
                      
int uImageRectMerge(iRect *ir,iRect *r)
{
	int i,j,ii,jj;
	int ic;
	
	if(!ir || !r)return 1;
	if(ir->type == ScreenDataType8 && r->type == ScreenDataType8){
	
	for(j=0;j<r->box.ysize;++j){
		jj=j+r->box.y;
		if(jj < 0 || jj >= ir->box.ysize)continue;
		for(i=0;i<r->box.xsize;++i){
			ii=i+r->box.x;
			if(ii < 0 || ii >= ir->box.xsize)continue;
			ic=r->data[i+j*r->box.xsize];
			if(ic == 0)continue;
		    ir->data[ii+jj*ir->box.xsize]=ic;		       
		}
	}
	
	}else{
	    WarningBatch("uImageRectMerge type != ScreenDataType8\n");
	    return 1;
	}
	return 0;
}                      

int uImageRectStart(iRect *ir,uRect *r)
{
	long lenght;
	
	if(!ir || !r)return 1;
		
	lenght=3*r->xsize*r->ysize;
	if(ir->data)cFree((char *)ir->data);
	ir->data=(unsigned char *)cMalloc(lenght,8263);
	if(!ir->data)return 1;
	
	memset((char *)ir->data, 0,lenght);

	ir->box = *r;
	
	return 0;
}                       
int uImageRectEnd(iRect *ir)
{
	if(!ir)return 1;
	
	if(ir->data)cFree((char *)ir->data);
	ir->data=NULL;
	return 0;
}
int uFillRectPattern(uRect *r,uPattern *pat,IconPtr myIcon)
{
 	int ForeColorIndex;
 	int BackColorIndex;
	unsigned char *out;
	unsigned char p;
	long length;
	int i,j,n,m,k;
	iRect ro;
	
	
	if(!r || !pat || !myIcon)return 1;
	
	ForeColorIndex=myIcon->ForeColorIndex;
	BackColorIndex=myIcon->BackColorIndex;
	
	length=r->xsize*r->ysize;

    if(SetBuffers(length*3+512L))return 1;
    
    out=(unsigned char *)xg.buffin;
    
    n=0;
    m=0;
    for(j=0;j<r->ysize;++j){
        p=pat->pat[m];
        k=0;
        for(i=0;i<r->xsize;++i){
           if((p << k) & 128){
               out[n++]=ForeColorIndex;
           }else{
               out[n++]=BackColorIndex;
           }
           if(++k >= 8)k=0;
        }
        if(++m >= 8)m=0;
    }
    
    ro.type=ScreenDataType8;
    ro.box= *r;
    ro.data=out;
    
    uDrawImageRect(&ro,myIcon);
    
	return 0;
}

int WriteRect24ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize)
{
	double xScale,yScale;
	register unsigned char *outp,*inp;
	register int ic;
	long oxsize3;
	long xsize3;
	int i,j,jj,ii;
	int jd,id;
	int is,js;
	int ie,je;
	
	
	if(!in || !out || !SRect || !DRect )return 1;
	
	oxsize3=3*oxsize;
	xsize3=3*xsize;
	
	if((SRect->xsize == DRect->xsize) && (SRect->ysize == DRect->ysize)){
	    js=SRect->y;
	    if(js < 0)js = 0;
	    
	    je=SRect->y+SRect->ysize;
	    if(je >= ysize)je=(int)ysize;
	    
	    is=SRect->x;
	    if(is < 0)is = 0;
	    
	    ie=SRect->x+SRect->xsize;
	    if(ie >= xsize)ie=(int)xsize;
	    	    
	    jj=DRect->y;
	    if(jj < 0){
	        js -= jj;
	        jj=0;
	    }
	    
	    if(jj+je-js > oysize){
	        je=(int)(js+oysize-jj);
	    }


	    ii=DRect->x;
	    if(ii < 0){
	        is -= ii;
	        ii=0;
	    }

	    if(ii+ie-is > oxsize){
	        ie=(int)(is+oxsize-ii);
	    }
	    
	    ie -= is;
	    
	    if(ie < 0)ie = 0;
	    
	    for(j=js;j<je;++j){
	    	outp=out+3*ii+jj++ *oxsize3;
	    	inp=in+is*3+j*xsize3;	    	
	    	ic=ie+1;
	    	while(--ic){	    	
	    		*outp++ = *inp++;
	    		*outp++ = *inp++;
	    		*outp++ = *inp++;
	    	}
	    }
	}else{
	    xScale=(double)SRect->xsize/(double)DRect->xsize;
	    yScale=(double)SRect->ysize/(double)DRect->ysize;
	    if(xScale > 1.0 && yScale > 1.0){
	    	xScale=(double)DRect->xsize/(double)SRect->xsize;
	    	yScale=(double)DRect->ysize/(double)SRect->ysize;
		    for(j=0;j<SRect->ysize;++j){
		        js=j+SRect->y;
		    	if(js < 0 || js >= ysize)continue;	 
		    	jd=DRect->y+(int)(j*yScale);
		    	if(jd < 0 || jd >= oysize)continue;
		    	for(i=0;i<SRect->xsize;++i){
		        	is=i+SRect->x;
		    		if(is < 0 || is >= xsize)continue;	 
		    		id=DRect->x+(int)(i*xScale);
		    		if(id < 0 || id >= oxsize)continue;
		    		/*
		    		if(in[is*3+js*xsize3] == 255 && in[is*3+1+js*xsize3] == 255 &&
		    		   in[is*3+2+js*xsize3] == 255){
		    		       continue;
		    		}
		    		*/
		    		out[id*3+jd*oxsize3]=in[is*3+js*xsize3];
		    		out[id*3+1+jd*oxsize3]=in[is*3+1+js*xsize3];
		    		out[id*3+2+jd*oxsize3]=in[is*3+2+js*xsize3];
		    		
		    	}
		    }
	    }else{
		    for(j=0;j<DRect->ysize;++j){
		        jd=j+DRect->y;
		    	if(jd < 0 || jd >= oysize)continue;
		    	js=SRect->y+(int)(j*yScale);
		    	if(js < 0 || js >= ysize)continue;	 
		    	for(i=0;i<DRect->xsize;++i){
		        	id=i+DRect->x;
		    		if(id < 0 || id >= oxsize)continue;
		    		is=SRect->x+(int)(i*xScale);
		    		if(is < 0 || is >= xsize)continue;	 
		    		out[id*3+jd*oxsize3]=in[is*3+js*xsize3];
		    		out[id*3+1+jd*oxsize3]=in[is*3+1+js*xsize3];
		    		out[id*3+2+jd*oxsize3]=in[is*3+2+js*xsize3];
		    	}
		    }
	    }
	}
	
	return 0;
}
int WriteRect8ToRect24(unsigned char *in, uRect *SRect,long xsize, long ysize,
                       unsigned char *out,uRect *DRect,long oxsize,long oysize,
                       unsigned char *pal)
{
	double xScale,yScale;
	register unsigned char *outp,*inp;
	register int ic;
	long oxsize3;
	int i,j,jj,ii;
	int jd,id;
	int is,js;
	int ie,je;
	
	
	if(!in || !out || !SRect || !DRect || !pal)return 1;
	
	oxsize3=3*oxsize;
	
	if((SRect->xsize == DRect->xsize) && (SRect->ysize == DRect->ysize)){
	    js=SRect->y;
	    if(js < 0)js = 0;
	    
	    je=SRect->y+SRect->ysize;
	    if(je >= ysize)je=(int)ysize;
	    
	    is=SRect->x;
	    if(is < 0)is = 0;
	    
	    ie=SRect->x+SRect->xsize;
	    if(ie >= xsize)ie=(int)xsize;
	    	    
	    jj=DRect->y;
	    if(jj < 0){
	        js -= jj;
	        jj=0;
	    }
	    
	    if(jj+je-js > oysize){
	        je=(int)(js+oysize-jj);
	    }


	    ii=DRect->x;
	    if(ii < 0){
	        is -= ii;
	        ii=0;
	    }

	    if(ii+ie-is > oxsize){
	        ie=(int)(is+oxsize-ii);
	    }
	    
	    ie -= is;
	    
	    if(ie < 0)ie = 0;
	    
	    for(j=js;j<je;++j){
	    	outp=out+3*ii+jj++ *oxsize3;
	    	inp=in+is+j*xsize;	    	
	    	ic=ie+1;
	    	while(--ic){	    	
	    		*outp++ = pal[3*(*inp)];
	    		*outp++ = pal[3*(*inp)+1];
	    		*outp++ = pal[3*(*inp++)+2];
	    	}
	    }
	}else{
	    xScale=(double)SRect->xsize/(double)DRect->xsize;
	    yScale=(double)SRect->ysize/(double)DRect->ysize;
	    if(xScale > 1.0 && yScale > 1.0){
	    	xScale=(double)DRect->xsize/(double)SRect->xsize;
	    	yScale=(double)DRect->ysize/(double)SRect->ysize;
		    for(j=0;j<SRect->ysize;++j){
		        js=j+SRect->y;
		    	if(js < 0 || js >= ysize)continue;	 
		    	jd=DRect->y+(int)(j*yScale);
		    	if(jd < 0 || jd >= oysize)continue;
		    	for(i=0;i<SRect->xsize;++i){
		        	is=i+SRect->x;
		    		if(is < 0 || is >= xsize)continue;	 
		    		id=DRect->x+(int)(i*xScale);
		    		if(id < 0 || id >= oxsize)continue;
			    	out[id*3+jd*oxsize3]   = pal[3*in[is+js*xsize]];
			    	out[id*3+1+jd*oxsize3] = pal[3*in[is+js*xsize]+1];
			    	out[id*3+2+jd*oxsize3] = pal[3*in[is+js*xsize]+2];
		    	}
		    }
	    }else{		    
		    for(j=0;j<DRect->ysize;++j){
		        jd=j+DRect->y;
		    	if(jd < 0 || jd >= oysize)continue;
		    	js=SRect->y+(int)(j*yScale);
		    	if(js < 0 || js >= ysize)continue;	 
		    	for(i=0;i<DRect->xsize;++i){
		        	id=i+DRect->x;
		    		if(id < 0 || id >= oxsize)continue;
		    		is=SRect->x+(int)(i*xScale);
		    		if(is < 0 || is >= xsize)continue;	 
		    		out[id*3+jd*oxsize3]   = pal[3*in[is+js*xsize]];
		    		out[id*3+1+jd*oxsize3] = pal[3*in[is+js*xsize]+1];
		    		out[id*3+2+jd*oxsize3] = pal[3*in[is+js*xsize]+2];
		    	}
		    }
	    }
	    
	}
	
	return 0;
}
int WriteRectToRect(unsigned char *in, uRect *SRect,long xsize, long ysize,
                    unsigned char *out,uRect *DRect,long oxsize,long oysize)
{
	double xScale,yScale;
	register unsigned char *outp,*inp;
	register int ic;
	int i,j,jj,ii;
	int jd,id;
	int is,js;
	int ie,je;
	
	
	if(!in || !out || !SRect || !DRect )return 1;
	
	if((SRect->xsize == DRect->xsize) && (SRect->ysize == DRect->ysize)){
	    js=SRect->y;
	    if(js < 0)js = 0;
	    
	    je=SRect->y+SRect->ysize;
	    if(je >= ysize)je=(int)ysize;
	    
	    is=SRect->x;
	    if(is < 0)is = 0;
	    
	    ie=SRect->x+SRect->xsize;
	    if(ie >= xsize)ie=(int)xsize;
	    	    
	    jj=DRect->y;
	    if(jj < 0){
	        js -= jj;
	        jj=0;
	    }
	    
	    if(je-js > oysize){
	        je=(int)(js+oysize);
	    }


	    ii=DRect->x;
	    if(ii < 0){
	        is -= ii;
	        ii=0;
	    }

	    if(ie-is > oxsize){
	        ie=(int)(is+oxsize);
	    }
	    
	    ie -= is;
	    
	    if(ie < 0)ie = 0;
	    
	    for(j=js;j<je;++j){
	    	outp=out+jj++ *oxsize;
	    	inp=in+is+j*xsize;	    	
	    	ic=ie+1;
	    	while(--ic){	    	
	    		*outp++ = *inp++;
	    	}
	    }
	}else{
	    xScale=(double)SRect->xsize/(double)DRect->xsize;
	    yScale=(double)SRect->ysize/(double)DRect->ysize;
	    for(j=0;j<DRect->ysize;++j){
	        jd=j+DRect->y;
	    	if(jd < 0 || jd >= oysize)continue;
	    	js=SRect->y+(int)(j*yScale);
	    	if(js < 0 || js >= ysize)continue;	 
	    	for(i=0;i<DRect->xsize;++i){
	        	id=i+DRect->x;
	    		if(id < 0 || id >= oxsize)continue;
	    		is=SRect->x+(int)(i*xScale);
	    		if(is < 0 || is >= xsize)continue;	 
	    		out[id+jd*oxsize]=in[is+js*xsize];
	    	}
	    }
	}
	
	return 0;
}
int uSetForeByIndex(int icolor,IconPtr myIcon)
{
	if(!myIcon)return 1;
	if(icolor < 0 || icolor > 255)return 1;
	
	myIcon->ForeColorIndex=icolor;
	uSetForeColor(myIcon->red[icolor],myIcon->green[icolor],myIcon->blue[icolor],myIcon);
	return 0;
}
int uSetBackByIndex(int icolor,IconPtr myIcon)
{
	if(!myIcon)return 1;
	if(icolor < 0 || icolor > 255)return 1;
	myIcon->BackColorIndex=icolor;
	uSetBackColor(myIcon->red[icolor],myIcon->green[icolor],myIcon->blue[icolor],myIcon);
	return 0;
}
int doLineToBuff(char *buff,struct buffStuff *b,int flag)
{
	long n,k,nadd;
	char *buffer;
	
	if(!buff || !b)return 1;
	
	n=(long)strlen(buff);
	if(n < 0)n = 0;
	if((n + 2 + b->length) >= b->lengthMax){
	    nadd=n+20000;
	    buffer=(char *)cMalloc(nadd+b->lengthMax,8250);
	    if(!buffer)return 1;
	    if(b->buffer){
	        for(k=0;k<b->length;++k){
	            buffer[k]=b->buffer[k];
	        } 
	        cFree((char *)b->buffer);
	        b->buffer=NULL;
	    }
	    b->buffer=buffer;
	    b->lengthMax += nadd;
	}
	
	for(k=0;k < n;++k){
	    b->buffer[b->length+k]=buff[k];
	}
	
	
	if(flag){
		if(uPref.TextOutputFormat == WindowsTextType){
			b->buffer[b->length+n] = '\r';
			++n;
			b->buffer[b->length+n] = '\n';
		}else if(uPref.TextOutputFormat == UnixTextType){
			b->buffer[b->length+n] = '\n';
		}else{
			b->buffer[b->length+n] = '\r';
		}
	}else{
		b->buffer[b->length+n] = '\r';
	}
	
	b->length += (n+1);
	
	b->buffer[b->length] = 0;
	
	return 0;
}

int doSmooth(double *x,double *y,long nodes,int PassSmooth,int PolynomialPrint)
{
	long i,j;
	
	if(!x || !y || (nodes <= 0) || (PassSmooth < 1))return 1;

	//PolynomialPrint=PolynomialPrint;
		
    for(j=0;j<PassSmooth;++j){
        for(i=2;i<nodes;++i){
            y[i-1]=(.5*y[i-1]+.25*y[i-2]+.25*y[i]);
            x[i-1]=(.5*x[i-1]+.25*x[i-2]+.25*x[i]);
        }
    }
	
	return 0;
	
}

int fitit(double *x,double *y,long nodes,int Order,double *a,double *b,double ww)
{
	double *k,*f,*xx,*w;
	int size;
	int ret;
	int n,m;
	long l;
	
	if(!x || !y || !a || !b || (nodes < 1) || (Order < 1) || !ww)return 1;
	
	ret=1;
	
	size=2*Order+1;
	
	k=NULL;
	f=NULL;
	xx=NULL;
	w=NULL;
	
	k=(double *)cMalloc(sizeof(double)*size*size,9921);
	if(!k)goto ErrorOut; 
	
	f=(double *)cMalloc(sizeof(double)*size,9923);
	if(!f)goto ErrorOut; 
	
	xx=(double *)cMalloc(sizeof(double)*size,9924);
	if(!xx)goto ErrorOut; 
	
	w=(double *)cMalloc(sizeof(double)*size,9925);
	if(!w)goto ErrorOut; 
	
	for(n=0;n<Order;++n)w[n]=(n+1)*ww;
	
	for(m=0;m<Order;++m){
	    for(n=0;n<Order;++n){
			k[m*size+n] = 0.;
			k[m*size+n+Order] = 0.;
			k[(m+Order)*size+n] = 0.;
			k[(m+Order)*size+n+Order] = 0.;
			for(l=0;l<nodes;++l){
			    k[m*size+n] += sin(w[m]*x[l])*sin(w[n]*x[l]);
			    k[m*size+n+Order] += sin(w[m]*x[l])*cos(w[n]*x[l]);
			    k[(m+Order)*size+n] += cos(w[m]*x[l])*sin(w[n]*x[l]);
			    k[(m+Order)*size+n+Order] += cos(w[m]*x[l])*cos(w[n]*x[l]);
			}
	    }
	}
	k[size*size-1] = (double)nodes;
	
	for(m=0;m<Order;++m){
	    f[m] = 0;
	    f[m+Order] = 0;
	    f[size-1] = 0;
	    k[m*size+size-1] = 0.;
	    k[(m+Order)*size+size-1] = 0.;
	    k[(size-1)*size+m+Order] = 0.;
	    k[(size-1)*size+m] = 0.;
        for(l=0;l<nodes;++l){
			f[m] += sin(w[m]*x[l])*y[l];
			f[m+Order] += cos(w[m]*x[l])*y[l];
			f[size-1] += y[l];
			k[m*size+size-1] += sin(w[m]*x[l]);
			k[(m+Order)*size+size-1] += cos(w[m]*x[l]);
			k[(size-1)*size+m+Order] += cos(w[m]*x[l]);
			k[(size-1)*size+m] += sin(w[m]*x[l]);
	    }
	}
		
	solve(k,f,xx,size,0);

    a[0]=0;
    b[0]=xx[size-1];

	for(m=0;m<Order;++m){
        a[m+1]=xx[m];
        b[m+1]=xx[m+Order];
    }

		
	ret=0;
ErrorOut:

	if(k)cFree((char *)k);
	if(f)cFree((char *)f);
	if(xx)cFree((char *)xx);
	if(w)cFree((char *)w);
	return ret;
}

int doFourier(double *x,double *y,long nodes,int Order,int PolynomialPrint)
{
	double *a,*b,dt,w,pi;
	double t,yy;
	int Csize;
	int ret;
	long n;
	int k;
		
	if(!x || !y || (nodes <= 0) || (Order < 1))return 1;
	
	ret = 1;
	
	
	a=NULL;
	b=NULL;
	Csize=Order+1;
	
	a=(double *)cMalloc(sizeof(double)*Csize,7628);
	if(!a)goto ErrorOut;
	zerol((char *)a,sizeof(sizeof(double)*Csize));
		
	b=(double *)cMalloc(sizeof(double)*Csize,7629);
	if(!b)goto ErrorOut;
	zerol((char *)b,sizeof(sizeof(double)*Csize));
	
	dt=x[nodes-1]-x[0];
	if(dt <= 0.0)goto ErrorOut;
	
	pi=4.0*atan(1.0);
	
	w=2*pi/dt;
	
	if(fitit(x,y,nodes,Order,a,b,w))goto ErrorOut;
	
    for(n=0;n<nodes;++n){
        t=x[n]-x[0];
        yy=b[0];
        for(k=1;k<Csize;++k){
           yy += a[k]*sin(k*w*t)+b[k]*cos(k*w*t);
        }
        y[n]=yy;
    }
        
    if(PolynomialPrint){
        sprintf(WarningBuff,"b[0] %g w %g",b[0],w);
        WarningBatchFast(WarningBuff);
        for(k=1;k<Csize;++k){
        	sprintf(WarningBuff,"a[%d] %g b[%d] %g k*w %g",k,a[k],k,b[k],k*w);
        	WarningBatchFast(WarningBuff);
        }        	
		WarningBatchAtEnd(WarningBuff);        
    }
	
		
	ret = 0;
ErrorOut:
	if(a)cFree((char *)a);
	if(b)cFree((char *)b);
	return ret;
	
}


struct fitcom{
	double xfmn;
	double xfmx;
	double yfmn;
	double yfmx;
	double *a;
	double *x;
	double *f;
    int npoly;
    int npf;
};

int sort(double *a,double *f,double *x,double *y,int n,int num,struct fitcom *fc);
int solve(double *a,double *f,double *x,int n,int iflag);
int sfxy(double xx,double *yyi,struct fitcom *f);

int sf(struct fitcom *f);

int doPolynomial(double *x,double *y,long n,int Order,int PolynomialPrint)
{
	struct fitcom fc;
	int ret;
	long i;
	
	if(!x || !y || (n <= 0) || (Order < 1))return 1;

	ret = 1;
	
	zerol((char *)&fc,sizeof(struct fitcom));

	fc.npoly=Order;
	fc.a=(double *)cMalloc(sizeof(double)*fc.npoly*fc.npoly,6253);
	if(!fc.a)goto ErrorOut;
	fc.x=(double *)cMalloc(sizeof(double)*fc.npoly,6254);
	if(!fc.x)goto ErrorOut;
	fc.f=(double *)cMalloc(sizeof(double)*fc.npoly,6255);
	if(!fc.f)goto ErrorOut;

	sort(fc.a,fc.f,x,y,(int)n,fc.npoly,&fc);
	solve(fc.a,fc.f,fc.x,fc.npoly,0);
	
	
	if(PolynomialPrint){
		sprintf(WarningBuff,"npoly %d",fc.npoly);
		WarningBatchFast(WarningBuff);
		for(i=0;i<fc.npoly;++i){
			sprintf(WarningBuff,"n %ld %g ",i,fc.x[i]);
			WarningBatchFast(WarningBuff);
		}	
		WarningBatchAtEnd("");
	}
	
	for(i=0;i<n;++i){
		sfxy(x[i],&y[i],&fc);
    }
	
	ret = 0;
ErrorOut:	

	if(fc.a)cFree((char *)fc.a);
	if(fc.x)cFree((char *)fc.x);
	if(fc.f)cFree((char *)fc.f);
	
	return ret;
}
int sort(double *a,double *f,double *x,double *y,int n,int num,struct fitcom *fc)
{
	double xp1,xp2,ft,xx,yy,tt;
	double dx,dy,odx,ody;
	double abig=1.e30;
	int i,j,k;
	
	if(!a || !f || !x || !y || !fc)return 1;
	
	fc->xfmn=abig;
	fc->xfmx=-abig;		
	fc->yfmn=abig;
	fc->yfmx=-abig;
	for(i=0;i<n;++i){
	  if(x[i] < fc->xfmn)fc->xfmn=x[i];
	  if(y[i] < fc->yfmn)fc->yfmn=y[i];
	  if(x[i] > fc->xfmx)fc->xfmx=x[i];
	  if(y[i] > fc->yfmx)fc->yfmx=y[i];
	}			
	dx=fc->xfmx-fc->xfmn;
	dy=fc->yfmx-fc->yfmn;
	if(dx <= 0.0 || dy <= 0.0){
		return 1;
	}
	odx=1./dx;
	ody=1./dy;	
	for(i=0;i<num;++i){
      f[i]=0.;
      for(j=0;j<num;++j){
      	a[i+j*num]=0.0;
      }
   }
   
   for(i=0;i<num;++i){
      xp1=1.0;
      ft=0.0;
      for(k=0;k<n;++k){
         xx=(x[k]-fc->xfmn)*odx;
         yy=(y[k]-fc->yfmn)*ody;
         if (i != 0){
             xp1=pow(xx,(double)i);
         }
         ft=ft+yy*xp1;
      }
      xp1=1.0;
      f[i]=ft;
	  for(j=0;j<num;++j){
		  tt=0.0;
		  xp2=1.0;
		  for(k=0;k<n;++k){
			  xx=(x[k]-fc->xfmn)*odx;
			  if (i != 0)xp1=pow(xx,(double)i);
			  if (j != 0)xp2=pow(xx,(double)j);
			  tt=tt+xp1*xp2;
		  }
		  a[i+j*num]=tt;
   	  }
   }
   return 0;
}   
int solve(double *a,double *f,double *x,int n,int iflag)
{
	int k,i,j,kn,kp,ik,ij;
	double den;
	
      kn=n-1;
      if (iflag > 0)goto L45;
/*
c
c       triangularize matrix
c
*/
	for(k=1;k<=kn;++k){
		kp=k+1;
		for(i=k;i<=n;++i){
		  if (a[i-1+(k-1)*n] == 0.0)continue;
		  den=1.0/a[i-1+(k-1)*n];
		  if(i < k+1){
			for(j=kp;j<=n;++j){
		  		a[i-1+(j-1)*n]=a[i-1+(j-1)*n]*den;
		  	}
		  }else{
			for(j=kp;j<=n;++j){
		  		a[i-1+(j-1)*n]=a[i-1+(j-1)*n]*den;
		  		a[i-1+(j-1)*n]=a[k-1+(j-1)*n]-a[i-1+(j-1)*n];
	   		}
	   	  }
   		}
 	}
   
   
/*
c
c       reduce load vector
c
*/

L45:

	for(k=1;k<=kn;++k){
      	kp=k+1;
		for(i=k;i<=n;++i){
		  if (a[i-1+(k-1)*n] == 0.0)continue;
		  den=1.0/a[i-1+(k-1)*n];
      	  f[i-1]=f[i-1]*den;
      	  if(i < k+1)continue;
      	  f[i-1]=f[k-1]-f[i-1];
      	}
    }
/*
c
c       begin back substitution
c
*/

	x[n-1]=f[n-1]/a[n-1+(n-1)*n];
	for(k=1;k<=kn;++k){
		ik=n-k;
		ij=ik+1;
		x[ik-1]=f[ik-1];
		for(j=ij;j<=n;++j){
			x[ik-1]=x[ik-1]-a[ik-1+(j-1)*n]*x[j-1];
		}
	}
	return 0;
      
}
int sfxy(double xx,double *yyi,struct fitcom *f)
{
	double dx,dy,odx;
	double xxx,yy,xt;
	int i;
	
	if(!yyi || !f)return 1;
	
	dx=f->xfmx-f->xfmn;
	dy=f->yfmx-f->yfmn;
	if(dx <= 0.0 || dy <= 0.0)return 1;
	
	odx=1./dx;
	xxx=(xx-f->xfmn)*odx;
    xt=xxx;
	yy=f->x[0];
	if(f->npoly <= 1){
	    *yyi = yy;
	    return 0;
	}
	
	for(i=1;i<f->npoly;++i){
	  yy=yy+xt*f->x[i];
	  xt=xt*xxx;
    }
        
    *yyi=yy=f->yfmn+yy*dy;
        	
	return 0;
}

int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag)
{
	char ***array;
	char **array1;
	char **array2;
	long Max;
	long n;
	
	if(!Array || !Maximun || (Length <= 0))return 1;
	
	Max = *Maximun;
	
	if(Count+1 <= Max)return 0;
	
	array=(char ***)Array;
	
	if(Count > 0){
		array1 = *array;
		if(!array1)return 1;
		array2=(char **)cMalloc((MaxIncrement+Max)*Length,tag);
		for(n=0;n<Max;++n){		    
            memcpy(&array2[n],&array1[n],Length);
		}
	    zerol((char *)&array2[Max],MaxIncrement*Length);
	    cFree((char *)array1);
	    *array=array2;
		*Maximun=MaxIncrement+Max;
	}else{
	    *array=(char **)cMalloc(MaxIncrement*Length,tag);
	    if(!*array)return 1;
	    zerol((char *)*array,MaxIncrement*Length);
	    *Maximun=MaxIncrement;
	}
	
	
	return 0;
}
int writeCursorString(uRect *dataRect2,char *buffout,IconPtr myIcon)
{
	char buff[512],*bp,*bs;
	uRect dataRect;
	int linecount;
	long length;
	uRect r;
	
	if(!dataRect2 || !buffout || !myIcon)return 1;
	
	uGetPortBounds(myIcon,&r);
	
    uSetTextFace(myIcon,0);
    uSetTextFont(myIcon,DefaultFont);
    uSetFontSize(myIcon,DefaultFontSize);
		
	dataRect = *dataRect2;
	
	dataRect.ysize=myIcon->CursorSpace;
	uEraseRect(&dataRect,myIcon);
	dataRect = *dataRect2;

	buff[0]=0;
	length=0;
	bs=bp=buffout;
	linecount=0;
	while(*bp){
	    if(*bp == '#'){
	       *bp=0;
	       if((length+uCStringWidth(bs,myIcon)) >= (r.xsize-ScrollOffSet)){
				++linecount;
				if(length > 0){
					length=0;
					*bp = '#';
					uWriteBox(&dataRect,buff,myIcon);
			        dataRect.y += DataSpace;
					buff[0]=0;
					continue;
				}else{
					length=0;
					uWriteBox(&dataRect,bs,myIcon);
			        dataRect.y += DataSpace;
					buff[0]=0;
				}
	       }else{
				length += (uCStringWidth(bs,myIcon)+uCStringWidth(" ",myIcon)); 
				mstrncat(buff,bs,sizeof(buff));         
				mstrncat(buff," ",sizeof(buff));         
	       }
	       bs=bp+1;
	    }
	    ++bp;
	}
	
	if(length > 0){
		uWriteBox(&dataRect,buff,myIcon);
	}
	
	return 0;
}
int	uGetCursorItemsString(char *buffout,long sizebuff,IconPtr myIcon,long nn)
{
	char name[256];
	long nd;
	int n;
	
	if(!buffout || !myIcon || (myIcon->Cursor.count <= 0) || !myIcon->Cursor.Items)return 1;
			
	for(n=0;n<myIcon->Cursor.count;++n){
	    if(myIcon->Cursor.Items[n].data && nn >= 0){
	        mstrncat(buffout,myIcon->Cursor.Items[n].pioName,sizebuff);
	        mstrncat(buffout,"=",sizebuff);	    
	        zerol((char *)name,sizeof(name));    
		    if(myIcon->Cursor.Items[n].valueFlag == 0){
				nd=(long)myIcon->Cursor.Items[n].data[nn];
		        sprintf(name,"%12ld",nd);
		    }else{
		    	sprintf(name,myIcon->Cursor.Items[n].format,myIcon->Cursor.Items[n].data[nn]);	
		    }	        
	        mstrncat(buffout,name,sizebuff);
	    	mstrncat(buffout,"#",sizebuff);
		}    
	}
	
	return 0;
}

int doCursorFree(struct CursorStruct *Cursor)
{
	int n;
	
	if(!Cursor)return 1;
	
	for(n=0;n<Cursor->count;++n){
		if(Cursor->Items){
		   if(Cursor->Items[n].data)cFree((char *)Cursor->Items[n].data);
		   Cursor->Items[n].data=NULL;
		}
	}
	if(Cursor->Items)cFree((char *)Cursor->Items);
	Cursor->Items=NULL;
	Cursor->count=0;		
			
	return 0;
}


int doGetCursorData(IconPtr myIcon,char *BatchName,struct SetFrameData  *sd)
{
	int n;
	
	if(!myIcon || !BatchName || !sd || !myIcon->Cursor.Items)return 1;
	
	for(n=0;n<myIcon->Cursor.count;++n){
	    if(myIcon->Cursor.Items[n].data)cFree((char *)myIcon->Cursor.Items[n].data);
	    myIcon->Cursor.Items[n].data=NULL;
		mstrncpy(sd->pioName,myIcon->Cursor.Items[n].pioName,64);
		sd->pioIndex=myIcon->Cursor.Items[n].pioIndex;			    
		
		if(SendMessageByName(BatchName,MessageType_GET_REGION_DATA,sd)){		    
		    continue;
		}
	    myIcon->Cursor.Items[n].data=(double *)sd->data;
	    sd->data=NULL;	
	}
	return 0;
}
int uWriteCursorItems(IconPtr myIcon,long nn,uRect *dataRect2)
{
	uRect dataRect;
	char buff[256],name[256];
	long nd;
	int n;
	int k;
	
	if(!dataRect2 || !myIcon || (myIcon->Cursor.count <= 0) || !myIcon->Cursor.Items)return 1;
	
	dataRect = *dataRect2;
		
	k=0;
	buff[0]=0;
	for(n=0;n<myIcon->Cursor.count;++n){
	    if(myIcon->Cursor.Items[n].data && nn >= 0){
	        mstrncat(buff,myIcon->Cursor.Items[n].pioName,255);
	        mstrncat(buff,"=",255);	        
		    if(myIcon->Cursor.Items[n].valueFlag == 0){
				nd=(long)myIcon->Cursor.Items[n].data[nn];
		        sprintf(name,"%12ld",nd);
		    }else{
		    	sprintf(name,myIcon->Cursor.Items[n].format,myIcon->Cursor.Items[n].data[nn]);	
		    }	        
	        mstrncat(buff,name,255);
	        mstrncat(buff," ",255);
		}    
	    if(k++ >= 2){
	        dataRect.y += DataSpace;
			uWriteBox(&dataRect,buff,myIcon);
			buff[0]=0;
			k=0;
	    }
	}
	
	if(k > 0){
        dataRect.y += DataSpace;
		uWriteBox(&dataRect,buff,myIcon);
	}
	return 0;
}
int doCursorItems(char *BatchName,struct CursorStruct *CursorList,IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {10,50,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {80,50,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,40,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"Add", {315,29,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"Delete", {10,50,65,20},uDialogButton,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,300};
 	
 	struct CursorDisplayItems *Item;
 	
	struct ScrollList *list2;

 	uDialogList *uList;
 	struct passItems p;
 	uRect *rectOpen;
 	char buff[256];
 	long count,n,k;
    int ret;
    
	if(!BatchName || !CursorList)return 1;
	
	Item=NULL;
	
	count=0;
	
	p.BatchName=BatchName;

	p.CursorList=CursorList;

	if(uPref.cursorSelectRect.xsize > 100 && uPref.cursorSelectRect.ysize > 100){
		rectOpen=&uPref.cursorSelectRect;
	}else{
		rectOpen=&Rect1;
	}
	
	ret=1;
	
	uList=uDialogOpen("Cursor Items",rectOpen,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetMyIcon,&p,
	              uDialogSetMove,doScale4Move,
	              uDialogSetStart,doScale4Start,
	              uDialogSetAction,doScale4Action,
	              uDialogSetButtons,doScale4SetButtons,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)goto ErrorOut;
	
	
	for(n=0;n<CursorList->count;++n){
		if(!CursorList->Items)continue;
		if(CursorList->Items[n].data)cFree((char *)CursorList->Items[n].data);
		CursorList->Items[n].data=NULL;
	}
	
	count=ScrollListCount(list2);
	if(count > 0){
		Item=(struct CursorDisplayItems *)cMalloc(count*sizeof(struct CursorDisplayItems),8253);
		if(!Item)goto ErrorOut;
		zerol((char *)Item,count*sizeof(struct CursorDisplayItems));
		for(n=0;n<count;++n){
			if(ScrollListGet(buff,n,list2))goto ErrorOut;
			mstrncpy(Item[n].pioName,buff,64);
			mstrncpy(Item[n].format,"%10.3e",8);
			Item[n].valueFlag=1;
		}
		
		for(k=0;k<CursorList->count;++k){
		    if(!CursorList->Items)continue;
			for(n=0;n<count;++n){
				if(!mstrcmp(Item[n].pioName,CursorList->Items[n].pioName)){
					mstrncpy(Item[n].format,CursorList->Items[n].format,8);
					Item[n].valueFlag=CursorList->Items[n].valueFlag;
					break;
				}
			}
		}
		if(CursorList->Items)cFree((char *)CursorList->Items);
		CursorList->Items=Item;
		Item=NULL;
		CursorList->count=(int)count;
	}else{
		if(CursorList->Items)cFree((char *)CursorList->Items);
		CursorList->Items=NULL;
		CursorList->count=0;
	}
	
	
	
	if(uList && uList->myIcon){
        uRect portRect;
		uGetPortBoundsGlobal(uList->myIcon,&portRect);
		uPref.cursorSelectRect=portRect;
	}
	
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    
    if(Item)cFree((char *)Item);
    
    if(!ret){
    	doCursorDisplay(myIcon);
    }

	return ret;
}


static int doScale4SetButtons(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	controlPtr Ok;
	controlPtr Delete;
	int ret;
	
	
	if(!uList)return 1;
		
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,0L,
              uDialogGetControl,&Ok,
              
              uDialogSetItem,4L,
              uDialogGetControl,&Delete,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	if(ScrollListIsSelectAny(list2)){
	    uHiliteControl(Delete,0);
	}else{
	    uHiliteControl(Delete,255);
	}

	
	return 0;
}

static int doScale4Action(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
    struct SetFrameData sd;    
	struct passItems *t;
	int ret;
	int type;
	
	if(!uList)return 1;
	t=(struct passItems *)uList->myIconItem;
	if(!t)return 1;
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	uSetCursor(NULL,uWATCH_CURSOR);
	if(uList->actionItem == 4){
		ScrollListDeleteSelected(list2);
	}else if(uList->actionItem == 3){
		
  		    type = MessageType_DO_SELECTION_DIALOG;
			if(SendMessageByName(t->BatchName,MessageType_DO_DIALOG,&type)){
			    goto OutOfHere;
			}
			
			if(SendMessageByName(t->BatchName,MessageType_GET_INFORMATION,&sd)){
				goto OutOfHere;
			}
			
			if(sd.pioName[0]){
			
	
				char *names[1];
				type=3;

				 names[0]= &sd.pioName[0];
				 
				ScrollListInsert(names,&type,-1L,1L,list2);

				ScrollListSetTop(0,list2);
			
			
			}						
	}
OutOfHere:
	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,uARROW_CURSOR);
	return 0;
}
static int doScale4Start(struct uDialogStruct *uList)
{
	struct passItems *p;
	struct ScrollList *list2;
	int ret;
	int n;
	
	
	if(!uList)return 1;
	p=(struct passItems *)uList->myIconItem;
	if(!p)return 1;
	
	if(!IsOption()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;

	
	
	for(n=0;n<p->CursorList->count;++n){	
 		char *names[1];
		int type=3;
		
		if(p->CursorList->Items){
		    if(!mstrcmp("none",p->CursorList->Items[n].pioName))continue;
			names[0]= p->CursorList->Items[n].pioName;		 
			ScrollListInsert(names,&type,n,1L,list2);
		}
	}
	ScrollListSetTop(0,list2);

	return 0;
}
static int doScale4Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	uRect rList1;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;
	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,10,r.ysize-25,
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize-85,r.ysize-25,
              	              	              
              uDialogSetItem,3L,
              uDialogSetLocation,r.xsize-185,r.ysize-25,
              	              	              
              uDialogSetItem,4L,
              uDialogSetLocation,10+100,r.ysize-25,
              	              	              
              uDialogSetItem,2L,
              uDialogGetRect,&rList1,
              uDialogGetScrollList,&list2,
              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)goto OutOfHere;
	
	
	
	
	rList1.x = 15;
	rList1.y = 25;
	rList1.xsize = (r.xsize-rList1.x);
	rList1.ysize = r.ysize-35-rList1.y;
	
	ScrollListFitRect(&rList1,list2);
	

	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList1,
              
              uDialogSetDone,uDialogSetDone
    );
    
OutOfHere:

	return 0;
}
int uOffsetRect(uRect *LineRect,int x,int y)
{
	if(!LineRect)return 1;
	LineRect->x += x;
	LineRect->y += y;
	return 0;
}
int SetDefaultPaletteName(IconPtr myIcon,char *name1)
{

	if(!name1)return 1;
	mstrncpy(DefaultPaletteName,name1,255);
	if(!myIcon)return 0;
	mstrncpy(myIcon->DefaultPaletteName,name1,255);
	return 0;
}
int GetDefaultPaletteName(IconPtr myIcon,char *name1)
{
	if(!name1)return 1;
	
	if(!myIcon || !myIcon->DefaultPaletteName[0]){
	    if(!DefaultPaletteName[0])return 1;
	    mstrncpy(name1,DefaultPaletteName,255);
	    return 0;
	}
	mstrncpy(name1,myIcon->DefaultPaletteName,255);
	return 0;
}
int GetPaletteByName(char *name,unsigned char *r,unsigned char *g,unsigned char *b)
{
	unsigned char *pal;
	char *name2;
	int n,k;
	
	if(!r || !g || !b)return 1;
	
    n=0;
    while(!getUniversalPaletteItem(n++,&name2,&pal)){
        if(!name || !mstrcmp(name,name2)){
        	for(k=0;k<256;++k){
        	    r[k]=pal[3*k];
        	    g[k]=pal[3*k+1];
        	    b[k]=pal[3*k+2];
        	}
        	return 0;
        }
	}
	
	return 1;
}
long CountFilesInList(struct FileList *Files)
{
	char name[2048];
	long n,nn;
	
	if(!Files)return 0;

	nn=0;
	for(n=0;n<Files->count;++n){
	    if(!Files->name[n])continue;
		mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->name[n],2048);
	    if(IsFile(name)){
	       nn++;
	    }
	}
	
	return nn;

}
long CountRelativePath(struct FileList *Files)
{
	char name[2048],*fp,*fp2;
	char *saveDirectory;
	long nn,nc,n;
	
	if(!Files || !uPref.readWritePath || !uPref.relativePath)return 0L;
	
	saveDirectory=Files->directory;
	if(!saveDirectory)return 0L;
	
	nc=0;
	for(n=0;n<2048;++n){
	    if(uPref.relativePath[n] == 0){
	        break;
	    }else if(uPref.relativePath[n] == BS){
	        ++nc;
	    }
	}
		
	mstrncpy(name,uPref.readWritePath,2048);
	fp=strrchr((char *)name,FILE_NAME_SEPERATOR);
    if(fp){
        while(fp > name && nc > 0){
            --fp;
            if(*fp == FILE_NAME_SEPERATOR)--nc;
        }
    }else{
        fp=name+strlen(name);
        *fp = FILE_NAME_SEPERATOR; 
    }
    
    ++fp;
    
	fp2=strchr((char *)uPref.relativePath,FS);
	if(fp2){
	    ++fp2;
		for(n=0;n<2048;++n,++fp2){
		    if(*fp2 == 0){
		    	*fp++ = 0;
		    	break;
		    }else if(*fp2 == FS){
		    	*fp++ = FILE_NAME_SEPERATOR;
		    }else{
		    	*fp++ = *fp2;
		    }
		}
	}else{
		*fp=0;
	}
    
    
    Files->directory=strsave(name,8886);
	
	nn=CountFilesInList(Files);
	
	if(nn > 0){
	   cFree((char *)saveDirectory);
    	sprintf(WarningBuff,"Relative File Directory Used - %s\n",name);
    	WarningBatch(WarningBuff);
	}else{
		if(Files->directory)cFree((char *)Files->directory);
		Files->directory=saveDirectory;
	}
	
	return nn;
}
int FindFilesInList(struct FileList *Files)
{
	char name[2048],*fp;
	char *saveDirectory;
	long n,nn;
	int try1;
	
	if(!Files || !Files->name)return 0;
	
	try1=1;
	
	saveDirectory=strsave(Files->directory,8888);

StartAgain:

	nn=CountFilesInList(Files);
	
	
	if(nn <= 0 && try1){
	   nn=CountRelativePath(Files);
	}
	
	if(nn <= 0){
   		char *pathname;
		if(try1){
			try1=0;
			Files->directory=ConvertPath(Files->directory,1);
			if(Files->directory)goto StartAgain;
		}
		if(saveDirectory){
		    if(Files->directory)cFree((char *)Files->directory);
			Files->directory=saveDirectory;
			saveDirectory=NULL;
		}
		mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->name[0],2048);
		fp=strrchr((char *)name,FILE_NAME_SEPERATOR);
	    if(fp){
	        fp += 1;
	    }else{
	        fp="";
	    }
		sprintf(WarningBuff,"Directory \'%s\' No Files (%s) Found To Load\n",Files->directory,fp);
		WarningBatch(WarningBuff);
		pathname=FileGetPath("Set New Load Directory Here",NULL);
		if(!pathname){
		    return 0;
		}		
		if(Files->directory)cFree(Files->directory);
		Files->directory=pathname;
		goto StartAgain;
	}

	if(saveDirectory)cFree((char *)saveDirectory);
		
	nn=0;
	for(n=0;n<Files->count;++n){
	    if(!Files->name[n])continue;
		mstrncpy(name,Files->directory,2048);
		strncatToPath(name,Files->name[n],2048);
	    if(IsFile(name)){
	       Files->name[nn++]=Files->name[n];
	    }else{
	       cFree((char *)Files->name[n]);
	       Files->name[n]=NULL;
	    }
	}
	
	Files->count=nn;
	
	return 1;
}

struct uNewWindowStruct{
    uRect windowRect;
    int AutoClose;
};

typedef struct uNewWindowStruct *uNewWindowPtr;


IconPtr NewNewWindow(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Auto Close", 		 {10, 10,140,30},uDialogCheck,uDialogGetLong},
      /*  1 */      {"About", 	     	 {10, 50,140,30},uDialogButton,uDialogGetLong},
      /*  2 */      {"Open", 	     	 {10, 90,140,30},uDialogButton,uDialogGetLong},
      /*  3 */      {"NetWindow", 	     {10,130,140,30},uDialogButton,uDialogGetLong},
      /*  4 */      {"SendWindow",       {10,170,140,30},uDialogButton,uDialogGetLong},
      /*  5 */      {"ExpressionWindow", {10,210,140,30},uDialogButton,uDialogGetLong},
      /*  6 */      {"RenderWindow",     {10,250,140,30},uDialogButton,uDialogGetLong},
      /*  7 */      {"TableWindow",      {10,290,140,30},uDialogButton,uDialogGetLong},
      /*  8 */      {"UtilitiesWindow",  {10,330,140,30},uDialogButton,uDialogGetLong},
      /*  9 */      {"Batch",     		 {10,450,140,30},uDialogButton,uDialogGetLong},
      /*  10 */     {"Preferences",      {10,490,140,30},uDialogButton,uDialogGetLong},
      /*  11 */     {"DrawWindow",       {10,410,140,30},uDialogButton,uDialogGetLong},
      /*  12 */     {"VolumetricWindow", {10,370,140,30},uDialogButton,uDialogNone},      
      /*  13 */     {"Close",     		 {10,530,140,30},uDialogButton,uDialogNone},      
      /*  14 */     {"Quit",     		 {10,570,140,30},uDialogButton,uDialogNone},      
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,160,620};
 	
 	extern char *ProgramName;
 	
 	IconPtr myIconRet;

	uNewWindowPtr n;
 	
 	uDialogList *uList;
    int retr;
    int ret;
    
    myIconRet=FindTypeWindow(NewWindowType);
    if(myIconRet){
    	if(myIconRet->pClose){
    	    (*myIconRet->pClose)(myIconRet);
    	}
    }
	
	ret=1;
	
	uList=NULL;
	
	myIconRet=NULL;
	
	if((n=(uNewWindowPtr)cMalloc(sizeof(struct uNewWindowStruct),8187)) == NULL){
		WarningBatch("NewNewWindow Out of Memory\n");
		goto ErrorOut;
	}
	zerol((char *)n,sizeof(struct uNewWindowStruct));
	
	myIcon=(IconPtr)n;
	
	uList=uDialogOpen(ProgramName,&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	retr=uDialogSet(uList,2L,	
	              
	          uDialogSetItem,11L,
	          uDialogSetHidden,(!isInstalled(Installed_DrawProgram)), 
	                    
	          uDialogSetStart,doScale2Start,
	              
              uDialogSetAction,uNewWindowAction,	 

	          uDialogSetItemClose,uNewWindowClose,
                           
	          uDialogSetModeLess,TRUE,	   
	          
	          uDialogSetMyIcon,myIcon,
	              
	          uDialogWindowType,NewWindowType,
	          
              uDialogSetDone,uDialogSetDone
	);
	if(retr)goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
    
    myIconRet=uList->myIcon;
	    	
	ret=0;
	
ErrorOut:

	if(ret){  
    	if(uList)uDialogClose(uList);
    }

	if(myIcon)myIconRet->isOpen=TRUE;
			
	return myIconRet;
}
static int uNewWindowClose(IconPtr myIcon)
{
	uNewWindowPtr n;
		
	if(myIcon){
	    n=(uNewWindowPtr)myIcon;
   	    if(n){
		    cFree((char *)n);
	    }
	       	    
	}
	return 0;

}
static int doScale2Start(struct uDialogStruct *uList)
{

	if(!uList)return 1;
		
	if(!IsShift()){
		doUniversalNewFile(uList->myIcon,universal_GetDefaults,NULL);		
	}
	
	return 0;
}
int doUniversalNewFile(IconPtr myIcon,int flag,FILE8 *inOut)
{

	static struct uNewWindowStruct net;
	static struct Icon mm;

	static struct universalTableListStruct Local2[] = {
		{"STRI",7202,universal_TypeStringPtr,(void *)&mm.infoSave.directory},
		{"STRI",7203,universal_TypeStringPtr,(void *)&mm.infoLoad.directory},
	};
	
	static struct universalTableStruct Global2= {
		"NewFile parameters",universal_WindowNewFile,
		(void *)&mm,sizeof(mm),
		NULL,0L,							
		Local2,sizeof(Local2)/sizeof(struct universalTableListStruct),							
	};
	
	static struct universalTableListStruct Local[] = {
		{"RECT",8001,universal_TypeRect,(void *)&net.windowRect},
		{"REAL",8002,universal_TypeInt,(void *)&net.AutoClose},									
		{"TABL",8200,universal_TypeTable,(void *)&Global2},	
		
	};
	
	static struct universalTableStruct Global= {
		"NewFile parameters",universal_WindowNewFile,
		(void *)&net,sizeof(net),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};
	
	struct uDialogStruct *uList;
	uNewWindowPtr n;
	int retr;
	int ret;
	
	n=NULL;
	uList=NULL;
	if(myIcon && (myIcon->WindowType == NewWindowType)){
		uList=uDialogGetList(myIcon);
		if(uList){
			n=(uNewWindowPtr)uList->myIconItem;
		}
	}
	
	Global.tableDynamic=n;
	Global.tableDynamicSpace=sizeof(struct uNewWindowStruct);
	
	
	Global2.tableDynamic=myIcon;
	Global2.tableDynamicSpace=sizeof(struct Icon);
		
	ret=1;
	if(flag == universal_SetDefaults){
	    if(n && myIcon){
			uGetPortBoundsGlobal(myIcon,&n->windowRect);
			if(moveUniversalTable(&Global,universal_MoveToStatic))goto ErrorOut;	
		}else{
			if(mm.infoSave.directory)cFree((char *)mm.infoSave.directory);
			mm.infoSave.directory=NULL;
			if(mm.infoLoad.directory)cFree((char *)mm.infoLoad.directory);
			mm.infoLoad.directory=NULL;
			uSetRect(&net.windowRect,40,45,160,620);
			net.AutoClose=FALSE;
		}
	}else if(flag == universal_SaveData){		
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_ReadData){		
		if(getUniversalTable(&Global,inOut))goto ErrorOut;				
	}else if(flag == universal_GetDefaults){		
	    if(n && myIcon){
			if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;				
			uMoveWindow(myIcon,n->windowRect.x,n->windowRect.y);
			uSizeWindowGlobal(myIcon,n->windowRect.xsize,n->windowRect.ysize);			
			
			if(myIcon->pMoveControls){
			    (myIcon->pMoveControls)(myIcon);
			}
			
			retr=uDialogSet(uList,0L,	
			              
				uDialogSetItem,0L,
				uDialogSetLogical,n->AutoClose,
				   
				uDialogSetDone,uDialogSetDone
			);
			if(retr)goto ErrorOut;
			
	    }
	}else if(flag == universal_Stop){	
		if(freeUniversalTable(&Global))goto ErrorOut;				
	}else{
		sprintf(WarningBuff,"doUniversalFileSend unknown flag %d\n",flag );
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}

static int uNewWindowAction(struct uDialogStruct *uList)
{
	uNewWindowPtr net;
	int setClose;
	int ret;
	
	if(!uList)return 1;
	net=(uNewWindowPtr)uList->myIconItem;
	if(!net)return 1;
	
	ret = 1;
	
	if(net->AutoClose){
	   setClose=TRUE;
	}else{
	   setClose=FALSE;
	}
		
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
    	switch(uList->actionItem){
    	case 0:
    		net->AutoClose = !net->AutoClose;
			if(!net->AutoClose)setClose=FALSE;
    		break;
    	case 1:
    		doAbout(uList->myIcon);
    		break;
    	case 2:
    		uOpenFileDialogLocal(uList->myIcon,NULL);
    		break;
    	case 3:
	    	NewNetWindow(uList->myIcon);
    	break;
    	case 4:
	    	NewFileSendWindow(uList->myIcon);
    	break;
    	case 5:
	    	NewCEdit(uList->myIcon,NULL);
    	break;
    	case 6:
	    	NewRenderWindow(uList->myIcon);
    	break;
    	case 7:
	    	NewTableWindow(uList->myIcon);
    	break;
    	case 8:
	    	macUtilities(uList->myIcon);
    	break;
    	case 9:
	        BatchFile();
    	break;
    	case 10:	     
			saveUniversal(uList->myIcon);
   		break;
    	case 11:	     
			NewDrawWindow(uList->myIcon);
    		break;
    	case 12:	     
			NewVolumetricWindow(uList->myIcon);
    		break;
    	case 13:
    	default:
			setClose=TRUE;
    		break;
     	case 14:
     		doQuit();
     		return 1;
   	}
	
	ret = 0;
	

	if(setClose){
		uDialogSet(uList,0L,
	          uDialogSetExitOk,
	          uDialogSetDone,uDialogSetDone
		);	
	}

	if(uList){
	    uDialogUpdate(uList);
	    uSetCursor(uList->myIcon,-uARROW_CURSOR);
	}
	return ret;
}

int doTestSpeed(IconPtr myIcon)
{
	extern INT8_64 FileSeeks8,FileReads8,FileWrite8,FileReadBytes8;
    extern INT8_64 TotalUsedMaximun;
	double start,end;
	long count;
	double den;
    FilePtr Files;
	
	if(!myIcon  || !myIcon->uDrawAnimate)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;
	count=0;
		
	FileReads8=0;
	FileSeeks8=0;
	FileWrite8=0;
	FileReadBytes8=0;
    TotalUsedMaximun=0;
    
	start=rtime();
	
	for(count=0;count<Files->ImageCount;++count){
	    if(Stop("Stop Time Test ?"))break;
	    if(myIcon->uDrawAnimate){
	        (*myIcon->uDrawAnimate)(myIcon,count);
	    }
	    if(myIcon->Synchronize)Synchronize(myIcon,count);
	}
	
	end=rtime();
	den=end-start;
	if(den <= 0)den=1;
	sprintf(WarningBuff,"Seconds %.2f Frames %ld (Frames/sec) %.2f",
	        den,count,((double)count)/(den));
	Warning(WarningBuff);
	
	sprintf(WarningBuff,"Seconds %.2f Frames %ld (Frames/sec) %.2f Memory %lld Reads8 %lld Seeks8 %lld Bytes8 %lld\n",
	        den,count,((double)count)/(den),TotalUsedMaximun,FileReads8,FileSeeks8,FileReadBytes8);
	WarningBatch(WarningBuff);
/*	
	InvalRectMyWindow(myIcon);
*/
	return 0;
}
int doMouseDown(IconPtr myIcon)
{
    FilePtr Files;
	uPoint ptStart,pt;
	long CurrentFrame;
	int dx,dxold;
	
	
	if(!myIcon  || !myIcon->uDrawAnimate)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;

	
	dxold=0;
	uPointer(&ptStart,myIcon);
	while(uStillDown(myIcon)){
		uPointer(&pt,myIcon);
	    dx=pt.x-ptStart.x;	
	    if(dx == dxold)continue;  
	    dxold = dx;  
	    CurrentFrame = Files->ImageCount/2 + dx;
	    if(CurrentFrame >= Files->ImageCount){
	        CurrentFrame=Files->ImageCount-1;
	    }else if(CurrentFrame < 0){
	       CurrentFrame=0;
	    }
	    
	    if(myIcon->uDrawAnimate){
	        (*myIcon->uDrawAnimate)(myIcon,CurrentFrame);
	    }
	    if(myIcon->Synchronize)Synchronize(myIcon,CurrentFrame);
		FlushCurrentPort();
	}
	
	InvalRectMyWindow(myIcon);
	
	return 0;
}
int doMouseSweepDown(IconPtr myIcon)
{
    FilePtr Files;
	uPoint current,old;
	long CurrentFrame,Show,CurrentFrameOld;
	int xsize;
	double place;
	uRect Box;

	if(!myIcon  || !myIcon->uDrawAnimate)return 1;
	Files=myIcon->Files;
	if(!Files)return 1;

	CurrentFrame    =  0;
	
	CurrentFrameOld = -1;

	uPointer(&old,myIcon);

	uGetPortBounds(myIcon,&Box);

	xsize=Box.xsize-ScrollOffSet;

	while(uStillDown(myIcon)){
		uPointer(&current,myIcon);
	    Show = CurrentFrame;
	    if(current.x < 0){
		    Show=0;
	    }else if(current.x > xsize){
		    Show=Files->ImageCount-1;
	    }else{
		    place=(double)(current.x)/(double)(xsize);
		    Show=(int)((double)(Files->ImageCount-1)*(place));
	    }

	    CurrentFrame=Show;
	    if(CurrentFrame >= Files->ImageCount){
	        CurrentFrame=Files->ImageCount-1;
	    }else if(CurrentFrame < 0){
	        CurrentFrame=0;
	    }
	    
	    if(CurrentFrame == CurrentFrameOld)continue;
	    	    	    
	    if(myIcon->uDrawAnimate){
	        (*myIcon->uDrawAnimate)(myIcon,CurrentFrame);
	    }
	    if(myIcon->Synchronize)Synchronize(myIcon,CurrentFrame);
	    
	    CurrentFrameOld=CurrentFrame;
	    
		FlushCurrentPort();
	}
	
	InvalRectMyWindow(myIcon);

	return 0;
}
int doFiddleColors(IconPtr myIcon)
{
	uPoint pt;
	short int xdim,ydim;
	short int xpos,ypos;
	double aa,bb;
	double unit_Theta;
	double up,down;
	register short int prej,i,j,k;
	short int	NUMCOLOR = 256;
	unsigned char red[256],green[256],blue[256];
	unsigned char reds[256],greens[256],blues[256];
	unsigned char *r,*g,*b;

	
	if(!myIcon)return 1;
		
	r=myIcon->red;
	g=myIcon->green;
	b=myIcon->blue;
	
	memmove(red,r,256L);
	memmove(green,g,256L);
	memmove(blue,b,256L);
	
	memmove(reds,r,256L);
	memmove(greens,g,256L);
	memmove(blues,b,256L);
	
	xdim=myIcon->uviewRect.xsize;
	ydim=myIcon->uviewRect.ysize;


	unit_Theta = (double) 3.14159 / (double) (ydim*2);
	

	while(uStillDown(myIcon)){
		uPointer(&pt,myIcon);
	    xpos=pt.x;
	    ypos=pt.y;
	    if(xpos < 0){
	        xpos=0;
	    }else if(xpos > xdim){
	        xpos=xdim;
	    }
	    if(ypos < 0){
	        ypos=0;
	    }else if(ypos > ydim){
	        ypos=ydim;
	    }

	    aa = (double) tan((double) (unit_Theta * (double) (ydim - ypos)));

	    up = 1 + (short) (127.0 * aa);
	    down = 256 - (short) (128.0 * aa);
	    if (up > down)
		bb = (((double)xpos / (double)xdim) * (double)(up - down))
			 + (double) down;
	    else
		bb = (((double)xpos / (double)xdim) * (double)(down - up))
			 + (double) up;
	    prej = 1;
	    
	    j=0;

	    for ( i = 1 ; i <= NUMCOLOR - 2 ; i++) {
	        j = (short) (aa * (double) (i - (NUMCOLOR/2)) + bb);
	        if ((j > 0) && (j <= NUMCOLOR-2)) {
		    for ( k = prej; k <= j - 1 ; k++){
		        if(k >= 0 && k < 256){
	        	    red[k]=reds[i];
	        	    green[k]=greens[i];
	        	    blue[k]=blues[i];
	        	}else{
	    		   Bell(5020);
	        	   goto OutOfHere;
	        	}
		    }
		    if(j >= 0 && j < 256){
	                red[j]=reds[i];
	                green[j]=greens[i];
	                blue[j]=blues[i];
	            }else{
	    		Bell(5021);
	        	goto OutOfHere;
	            }
	        }else if (j >= NUMCOLOR-1) {
		    for ( k = prej; k <= NUMCOLOR-2; k++){
		        if(k >= 0 && k < 256){
	        	    red[k]=reds[i];
	        	    green[k]=greens[i];
	        	    blue[k]=blues[i];
	        	}else{
	    		    Bell(5022);
	        	    goto OutOfHere;
	        	}
		    }
		    break;
		} else
			continue;

	        prej = j + 1;
 	    }
	    if(j <= 0)j = 1;
	    for(;j < NUMCOLOR;j++){
	        if(j >=0 && j < 256){
	            red[j]=reds[254];
	            green[j]=greens[254];
	            blue[j]=blues[254];
	        }else{
	    		Bell(5023);
	            goto OutOfHere;
	        }
	    
	    }

		memmove(r,red,256L);
		memmove(g,green,256L);
		memmove(b,blue,256L);
	    
	    if(myIcon->pUpdateWrite){
	        (*myIcon->pUpdateWrite)(myIcon);
	    }
	   
	}
OutOfHere:
	InvalRectMyWindow(myIcon);
	return 0;
}
int doEraseDown(IconPtr myIcon)
{
	uPoint start,current;
	int xold,dx,isDrawn;
	uRect oldR,currentR,eraseRect;

	if(!myIcon)return 1;
	xold=0;

	eraseRect.y=myIcon->eraseRect.y;
	eraseRect.x=myIcon->eraseRect.x;
	eraseRect.ysize=myIcon->eraseRect.ysize;
	eraseRect.xsize=myIcon->eraseRect.xsize;
	eraseRect.y -= 1;
	eraseRect.x += 1;
	eraseRect.xsize -= 1;
	eraseRect.ysize += 1;
	oldR=eraseRect;
	
	currentR.xsize=100;

	uPointer(&start,myIcon);

	isDrawn=FALSE;
	uSetPenXOR(TRUE,myIcon);
	uFrameRect(&oldR,myIcon);
	while(uStillDown(myIcon)){
		    uPointer(&current,myIcon);
		    dx=current.x-start.x;
		    if(dx == xold)continue;
		    xold=dx;
		    uFrameRect(&oldR,myIcon);
		    isDrawn=TRUE;
		    currentR=eraseRect;
		    currentR.xsize += dx;
		    if(current.y < currentR.y)currentR.y=current.y;
		    oldR=currentR;
		    uFrameRect(&oldR,myIcon);
	}
	
	uFrameRect(&oldR,myIcon);
	uSetPenXOR(FALSE,myIcon);
	if(isDrawn){
	    myIcon->eraseRectSize=currentR.xsize;
	    if(myIcon->pMoveControls)(*myIcon->pMoveControls)(myIcon);
	}
	return 0;
}
int doCursorDisplay(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {166,430,80,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {263,430,80,20},uDialogButton,uDialogNone},
      /*  2 */      {"Integer", {11,95,84,15},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"Real", {11,115,118,20},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"0", {11,62,80,20},uDialogText,uDialogNone},
      /*  5 */      {"0", {138,62,80,20},uDialogText,uDialogNone},
      /*  6 */      {"0", {259,62,80,20},uDialogText,uDialogNone},
      /*  7 */      {"Value", {11,12,75,16},uDialogString,uDialogNone},
      /*  8 */      {"Format", {11,37,75,16},uDialogString,uDialogNone},
      /*  9 */      {"Coordinates", {138,12,89,17},uDialogString,uDialogNone},
      /* 10 */      {"Format", {138,37,75,16},uDialogString,uDialogNone},
      /* 11 */      {"Time", {260,13,89,17},uDialogString,uDialogNone},
      /* 12 */      {"Format", {259,37,75,16},uDialogString,uDialogNone},
      /* 13 */      {"File List", {10,150,340,200},uDialogScrollList,uDialogNone},
      /* 14 */      {"Cursor Items",{138,130,89,20},uDialogString,uDialogNone},
      /* 15 */      {"Format", {11,355,75,20},uDialogString,uDialogNone},
      /* 16 */      {"Integer", {160,355,84,15},uDialogButtonRadio,uDialogGetLong},
      /* 17 */      {"Real", {160,375,118,20},uDialogButtonRadio,uDialogGetLong},
      /* 18 */      {"%11.3e", {11,380,80,20},uDialogText,uDialogNone},
      /* 19 */      {"last", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,361,490};
 	
 	uDialogList *uList;
	uDialogReturn *uR;
    int reti;
    int ret;
	
	
	if(!myIcon)return 1;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Cursor Display",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	reti=uDialogSet(uList,2L,
	              	              
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(int)(myIcon->Cursor.valueFlag == 0),
	              
	              uDialogSetItem,3L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(int)(myIcon->Cursor.valueFlag == 1),
	              
	              uDialogSetItem,4L,
		          uDialogSetText,myIcon->Cursor.valueformat,
              
	              uDialogSetItem,5L,
		          uDialogSetText,myIcon->Cursor.coordinateFormat,
              
	              uDialogSetItem,6L,
		          uDialogSetText,myIcon->Cursor.timeFormat,
              
	              uDialogSetItem,16L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(int)(0),
	              
	              uDialogSetItem,17L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(int)(1),

	              uDialogSetItem,19L,
	              uDialogSetFloatAux,(double)-1,
	              
	              uDialogSetMyIcon,myIcon,	              
	              uDialogSetStart,doScale5Start,
	              uDialogSetAction,doScale5Action,
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;
	
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
    
    if(uR[2].lreturn){
       myIcon->Cursor.valueFlag = 0;
    }else{
       myIcon->Cursor.valueFlag = 1;
    }
    
 	myIcon->Cursor.valueformat[0]=0;
	mstrncpy(myIcon->Cursor.valueformat,uR[4].sreturn,255);
  
 	myIcon->Cursor.coordinateFormat[0]=0;
	mstrncpy(myIcon->Cursor.coordinateFormat,uR[5].sreturn,255);
  
 	myIcon->Cursor.timeFormat[0]=0;
	mstrncpy(myIcon->Cursor.timeFormat,uR[6].sreturn,255);
  
		

	ret = 0;
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doScale5Action(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	uDialogReturn *uR;
	IconPtr myIcon;
	int ret;
	long n;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	
	ret=uDialogSet(uList,13L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	
	
	uSetCursor(NULL,uWATCH_CURSOR);
	if(uList->actionItem == 0 || uList->actionItem == 13){
	
		if(!myIcon->Cursor.Items)goto ErrorOut;
		
	    if((uR[19].dreturn >= 0) && (uR[19].dreturn < myIcon->Cursor.count)){
	        n=(long)uR[19].dreturn;
	        myIcon->Cursor.Items[n].valueFlag=(int)uR[17].lreturn;
	        mstrncpy(myIcon->Cursor.Items[n].format,uR[18].sreturn,8);
	    }
	    
	    if(uList->actionItem == 0)goto ErrorOut;
	    
		n=ScrollListFirstSelect(list2);
	    if((n < 0) || (n >= myIcon->Cursor.count))goto ErrorOut; 
	    
		ret=uDialogSet(uList,19L,
		
	              uDialogSetItem,16L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(int)(myIcon->Cursor.Items[n].valueFlag == 0),
	              
	              uDialogSetItem,17L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(int)(myIcon->Cursor.Items[n].valueFlag == 1),
		
		          uDialogSetItem,18L,
		          uDialogSetText,myIcon->Cursor.Items[n].format,
	
		
	              uDialogSetItem,19L,
	              uDialogSetFloatAux,(double)n,
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto ErrorOut;
	}
ErrorOut:
	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,uARROW_CURSOR);
    if(uR)uDialogFreeReturn(uR);
	return 0;
}
static int doScale5Start(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	IconPtr myIcon;
	int ret;
	int n;
	
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	
	if(!IsOption()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	ret=uDialogSet(uList,13L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;

	
	
	for(n=0;n<myIcon->Cursor.count;++n){	
 		char *names[1];
		int type=3;
		
		if(myIcon->Cursor.Items){
		    if(!mstrcmp("none",myIcon->Cursor.Items[n].pioName))continue;
			names[0]= myIcon->Cursor.Items[n].pioName;		 
			ScrollListInsert(names,&type,n,1L,list2);
		}
	}
	ScrollListSetTop(0,list2);

	return 0;
}
int uLineU(int x,int y,int nc,struct screenData *s)
{
	int ysize;
	
	if(!s)return 1;
	
	ysize=s->viewRect.ysize;	
	
	return LineB(x,ysize-1-y,nc,s);
}
int uMoveU(int x,int y,struct screenData *s)
{
	int ysize;
	
	if(!s)return 1;
	
	ysize=s->viewRect.ysize;
		
	return MoveB(x,ysize-1-y);
}
int uDrawStringU(char *out,int nc,int fn,struct screenData *s)
{
	unsigned char *image;
	uRect cRect;
	int x,y;
	int ret;
	
	if(!s)return 1;
	
	ret=1;
	
	//fn=fn;
	
	x=s->xLineWidth;
	y=s->yLineWidth;
	s->xLineWidth=0;
	s->yLineWidth=0;
	
	if(FontToRect(out,&cRect,&image,s->myIcon))goto ErrorOut;
	
	if(FontToScreen(&cRect,image,nc,s))goto ErrorOut;
		
	ret=0;
ErrorOut:
	s->xLineWidth=x;
	s->yLineWidth=y;
	return ret;
}
int uRotateRect(uRect *Rect,unsigned char **image,double fontAngle)
{
	unsigned char *dataPtr2;
	unsigned char *dataPtr;
	int xsize,ysize;
	int x, y;
	int i,j;
	long length;
	
	if(!Rect || !image)return 1;
	dataPtr = *image;
	if(!dataPtr)return 1;
	if(fontAngle == 0)return 0;
	
	if(FindB(&x,&y))return 1;
		
	xsize=Rect->xsize;
	ysize=Rect->ysize;
	
	length=xsize*ysize;
	if((dataPtr2=(unsigned char *)cMalloc(length,8103)) == NULL){
	    WarningBatch("uRotateRect Out of Memory\n");
	    goto ErrorOut;
	}
	
	zerol((char *)dataPtr2,length);
	
	if(fontAngle == 270){
	    for(i=0;i<xsize;++i){
		  for(j=0;j<ysize;++j){
		    dataPtr2[j+(xsize-1-i)*ysize]=dataPtr[i+j*xsize];
		  }
	    }
	    Rect->xsize=ysize;
	    Rect->ysize=xsize;
	    MoveB(x,y-xsize);
	} else if(fontAngle == 180){
	    for(j=0;j<ysize;++j){
		  for(i=0;i<xsize;++i){
		    dataPtr2[i+j*xsize]=dataPtr[xsize-1-i+(ysize-1-j)*xsize];
		  }
	    }
	    MoveB(x,y-ysize);
	} else if(fontAngle == -180){
	    for(j=0;j<ysize;++j){
		  for(i=0;i<xsize;++i){
		    dataPtr2[i+j*xsize]=dataPtr[i+(ysize-1-j)*xsize];
		  }
	    }
	} else if(fontAngle == 90){
	    for(i=0;i<xsize;++i){
		  for(j=0;j<ysize;++j){
		    dataPtr2[ysize-1-j+i*ysize]=dataPtr[i+j*xsize];
		  }
	    }
	    Rect->xsize=ysize;
	    Rect->ysize=xsize;
	} else {
	    for(i=0;i<xsize;++i){
		  for(j=0;j<ysize;++j){
		    dataPtr2[i+j*xsize]=dataPtr[i+j*xsize];
		  }
	    }
	}
	cFree((char *)dataPtr);
	
	*image=dataPtr2;
	
	return 0;
ErrorOut:
	return 1;
}
int uDrawStringAngleU(char *out,double angle,int nc,int fn,struct screenData *s)
{
	unsigned char *image;
	uRect cRect;
	int x,y;
	int ret;
	
	if(!s)return 1;
	
	ret=1;
	
	//fn=fn;
	
	x=s->xLineWidth;
	y=s->yLineWidth;
	s->xLineWidth=0;
	s->yLineWidth=0;
	
	if(FontToRect(out,&cRect,&image,s->myIcon))goto ErrorOut;
	
	if(uRotateRect(&cRect,&image,angle))goto ErrorOut;
	
	if(FontToScreen(&cRect,image,nc,s))goto ErrorOut;
		
	ret=0;
ErrorOut:
	s->xLineWidth=x;
	s->yLineWidth=y;
	return ret;
}
int FindB(int *x,int *y)
{
	if(!x || !y)return 1;
	*x=xBuff;
	*y=yBuff;
	return 0;
}
int LineC(int ix0,int iy0,int ix1,int iy1,int nc,struct screenData *s)
{
	int idx,irl,idy,iup,idxy;
	int ix,iy,ixs,iys,nxy;
	
	if(!s)return 1;
	
	if(ix1 < ix0){
		idx = ix0-ix1;
		irl = -1;
	}else{
		idx = ix1-ix0;
		irl = 1;
	}
	
	if(iy1 < iy0){
		idy = iy0-iy1;
		iup = -1;
	}else{
		idy = iy1-iy0;
		iup = 1;
	}
	idxy=idx+idy;
	if(idxy <= 0){
		kpoint(ix0,iy0,nc,s);
		goto Out;
	}
	ix=ix0;
	iy=iy0;
	ixs=0;
	iys=0;
	for(nxy=1;nxy<=idxy;++nxy){
		kpoint(ix,iy,nc,s);
		ixs=ixs+idx;
		if(ixs >= idxy){
			ixs=ixs-idxy;
			ix=ix+irl;
		}
		iys=iys+idy;
		if(iys < idxy)continue;
		iys=iys-idxy;
		iy=iy+iup;
	}
Out:
	kpoint(ix1,iy1,nc,s);
	
	return 0;
}
int MoveB(int x,int y)
{   
        xBuff=x;
        yBuff=y;
        return 0;
}   
int LineB(int x,int y,int nc,struct screenData *s)
{
        int ix0,iy0,ix1,iy1;
        int idx,irl,idy,iup,idxy;
        int ix,iy,ixs,iys,nxy;

        if(!s)return 1;
        ix0=xBuff;
        iy0=yBuff;
        ix1=x;
        iy1=y;

        if(ix1 < ix0){
            idx = ix0-ix1;
            irl = -1;
        }else{
            idx = ix1-ix0;
            irl = 1;
        }

        if(iy1 < iy0){
            idy = iy0-iy1;
            iup = -1;
        }else{
            idy = iy1-iy0;
            iup = 1;
        }
        idxy=idx+idy;
        if(idxy <= 0){
            kpoint(ix0,iy0,nc,s);
            goto Out;
        }
        ix=ix0;
        iy=iy0;
        ixs=0;
        iys=0;
        for(nxy=1;nxy<=idxy;++nxy){
            kpoint(ix,iy,nc,s);
            ixs=ixs+idx;
            if(ixs >= idxy){
                ixs=ixs-idxy;
                ix=ix+irl;
            }
            iys=iys+idy;
            if(iys < idxy)continue;
            iys=iys-idxy;
            iy=iy+iup;
        }
Out:
        kpoint(ix1,iy1,nc,s);
        xBuff=x;
        yBuff=y;
        return 0;
}
int kpoint(int x,int y,int nc,struct screenData *s)
{   
	long n,yr;

	if(!s || !s->buffer)return 1;

	if(s->xLineWidth > 1 || s->xLineWidth > 1){
	
		return kpointPattern(x,y,s);

    }else{
        if(x < 0 || x >= s->viewRect.xsize)return 0;
        if(y < 0 || y >= s->viewRect.ysize)return 0;
        yr=s->viewRect.ysize-1-y;
        if(s->ScreenBits == RASTER_8){
            s->buffer[x+yr*s->viewRect.xsize]=nc;
        }else if(s->ScreenBits == RASTER_24){
            unsigned char *pal;
            pal=s->palette;
            n=x*3+yr*s->viewRect.xsize*3;
            s->buffer[n]=pal[nc*3];
            s->buffer[1+n]=pal[nc*3+1];
            s->buffer[2+n]=pal[nc*3+2];
        }
    }
    return 0;
}
int kpointPattern(int x,int y,struct screenData *s)
{   
	static uPattern Pat={{0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44}};
	uPattern *pat;
	unsigned char p;
	int n,yr;
	int xn,yn;
	int i,j;
	int xs,ys;
	int nc;

	if(!s || !s->buffer)return 1;

		pat=PatternPattern(s->LinePattern);
		if(!pat){
		   pat=&Pat;
		}
        
    	xs=x;
    	ys=y;
        xn=s->xLineWidth;
        if(xn <= 0)xn=1;
        yn=s->yLineWidth;
        if(yn <= 0)yn=1;
        for(j=0;j<yn;++j){
            y=ys+j;
    		p=pat->pat[y % 8];
            for(i=0;i<xn;++i){
            	x=xs+i;
            	if((p << (x % 8)) & 128){
            	   nc=s->nLineFore;
            	}else{
            	   nc=s->nLineBack;
            	}
		        if(x < 0 || x >= s->viewRect.xsize)continue;
		        if(y < 0 || y >= s->viewRect.ysize)continue;
		        yr=s->viewRect.ysize-1-y;
		        if(s->ScreenBits == RASTER_8){
		            s->buffer[x+yr*s->viewRect.xsize]=nc;
		        }else if(s->ScreenBits == RASTER_24){
		            unsigned char *pal;
		            pal=s->palette;
		            n=x*3+yr*s->viewRect.xsize*3;
		            s->buffer[n]=pal[nc*3];
		            s->buffer[1+n]=pal[nc*3+1];
		            s->buffer[2+n]=pal[nc*3+2];
		        }	            
            	            	            
            }
        }
    return 0;
}
int uPaintRoundRectU(uRect *r,int ovalWidth,int ovalHeight,struct screenData *s)
{
	double dtheta,theta;
	double xc,yc,a,b;
	double pi;
	long steps;
	int n,k,m;
	int yr,yy,xx;
	int LinePattern;
	int nLineFore;
	int nLineBack;
	int xLineWidth;
	int yLineWidth;
	
	if(!r || !s  || !s->buffer)return 1;
	
	LinePattern=s->LinePattern;

	nLineFore=s->nLineFore;

	nLineBack=s->nLineBack;

	xLineWidth=s->xLineWidth;
	
	yLineWidth=s->yLineWidth;


	s->LinePattern=s->SolidPattern;

	s->nLineFore=s->nSolidFore;

	s->nLineBack=s->nSolidBack;

	s->xLineWidth=1;

	s->yLineWidth=1;


	steps=2*(r->ysize+r->xsize)+4;
		
	pi=4.0*atan(1.0);
	
	dtheta=0.5*pi/(double)(steps-1);
	
	if(ovalWidth > r->xsize)ovalWidth=r->xsize;
	
	if(ovalHeight > r->ysize)ovalHeight=r->ysize;
		
	a=(double)ovalWidth*0.5;
	b=(double)ovalHeight*0.5;
	for(k=0;k<4;++k){
	    if(k == 0){
			xc=(double)r->x+r->xsize-a;
			yc=(double)r->y+r->ysize-b;
			theta = -dtheta;
	    }else if(k == 1){
			xc=(double)r->x+a;
			yc=(double)r->y+r->ysize-b;
			theta = 0.5*pi-dtheta;
	    }else if(k == 2){
			xc=(double)r->x+a;
			yc=(double)r->y+b;
			theta = pi-dtheta;
	    }else{
			xc=(double)r->x+r->xsize-a;
			yc=(double)r->y+b;
			theta = 1.5*pi-dtheta;
	    }
	    
		for(n=0;n<steps;++n){
			theta += dtheta;
			xx=(int)(xc+a*cos(theta)+0.5);
			yy=(int)(yc+b*sin(theta)+0.5);
		    yr=s->viewRect.ysize-1-yy;
			kpointPattern(xx,yr,s);
		    if(k == 0){
		       for(m=(int)(r->x+r->xsize-a);m<xx;++m){
					kpointPattern(m,yr,s);
		       }
		    }else if(k == 1){
		       for(m=xx;m<(int)(r->x+a);++m){
					kpointPattern(m,yr,s);
		       }
		    }else if(k == 2){
		       for(m=xx;m<(int)(r->x+a);++m){
					kpointPattern(m,yr,s);
		       }
		    }else{
		       for(m=(int)(r->x+r->xsize-a);m<xx;++m){
					kpointPattern(m,yr,s);
		       }
		    }
		}
		
		


	}
	
	for(n=(int)(r->x+a);n<(int)(r->x+r->xsize-a);++n){
		for(yy=r->y;yy<r->y+r->ysize+1;++yy){
		    yr=s->viewRect.ysize-1-yy;
			kpointPattern(n,yr,s);
		}
	}
	
	for(yy=(int)(r->y+a);yy<(int)(r->y+r->ysize-a);++yy){
	    yr=s->viewRect.ysize-1-yy;
	    
		for(xx=(int)(r->x);xx<(int)(r->x+a);++xx){	    
			kpointPattern(xx,yr,s);
		}
		for(xx=(int)(r->x+r->xsize-a);xx<(int)(r->x+r->xsize);++xx){	    
			kpointPattern(xx,yr,s);
		}
	}


	s->LinePattern=LinePattern;

	s->nLineFore=nLineFore;

	s->nLineBack=nLineBack;

	s->xLineWidth=xLineWidth;

	s->yLineWidth=yLineWidth;

	return 0;
}
int uFrameRoundRectU(uRect *rr,int ovalWidth,int ovalHeight,struct screenData *s)
{
	double dtheta,theta;
	double x,y,xc,yc,a,b;
	uRect *r,rl;
	double pi;
	long steps;
	int n,k;
	int yr;
	
	if(!rr || !s  || !s->buffer)return 1;
	
	rl = *rr;
	
	r=&rl;
	
	r->x -= (int)(0.5*(s->xLineWidth));
	r->y += (int)(0.5*(s->yLineWidth));

	steps=2*(r->ysize+r->xsize)+4;
	
	pi=4.0*atan(1.0);
	
	dtheta=0.5*pi/(double)(steps-1);
	
	if(ovalWidth > r->xsize)ovalWidth=r->xsize;
	
	if(ovalHeight > r->ysize)ovalHeight=r->ysize;
	
	
	a=(double)ovalWidth*0.5;
	b=(double)ovalHeight*0.5;
	for(k=0;k<4;++k){
	    if(k == 0){
			xc=(double)r->x+r->xsize-a;
			yc=(double)r->y+r->ysize-b;
			theta = -dtheta;
	    }else if(k == 1){
			xc=(double)r->x+a;
			yc=(double)r->y+r->ysize-b;
			theta = 0.5*pi-dtheta;
	    }else if(k == 2){
			xc=(double)r->x+a;
			yc=(double)r->y+b;
			theta = pi-dtheta;
	    }else{
			xc=(double)r->x+r->xsize-a;
			yc=(double)r->y+b;
			theta = 1.5*pi-dtheta;
	    }
		for(n=0;n<steps;++n){
			theta += dtheta;
			x=xc+a*cos(theta)+0.5;
			y=yc+b*sin(theta)+0.5;
		    yr=s->viewRect.ysize-1-(int)(y);
			kpoint((int) x,yr,s->nLineFore,s);
		}
	}

	for(n=(int)(r->x+a);n<(int)(r->x+r->xsize-a);++n){
	    yr=s->viewRect.ysize-1-(int)(r->y);
		kpoint(n,yr,s->nLineFore,s);
	    yr=s->viewRect.ysize-1-(int)(r->y+r->ysize);
		kpoint(n,yr,s->nLineFore,s);
	}
	for(n=(int)(r->y+b);n<(int)(r->y+r->ysize-b);++n){
	    yr=s->viewRect.ysize-1-n;
		kpoint((int)r->x,yr,s->nLineFore,s);
	    yr=s->viewRect.ysize-1-n;
		kpoint((int)(r->x+r->xsize),yr,s->nLineFore,s);
	}
	
	return 0;
}
int uFrameOvalU(uRect *rr,struct screenData *s)
{
	double dtheta,theta;
	double x,y,xc,yc,a,b;
	uRect *r,rl;
	long steps;
	long n;
	int yr;


	if(!rr || !s  || !s->buffer)return 1;

	rl = *rr;
	
	r=&rl;
	
	r->x -= (int)(0.5*(s->xLineWidth));
	r->y += (int)(0.5*(s->yLineWidth));
	
	steps=3*(r->ysize+r->xsize)+4;
	
	dtheta=8.0*atan(1.0)/(double)(steps-1);
	theta = -dtheta;
	
	a=(double)r->xsize*0.5;
	xc=(double)r->x+a;
	b=(double)r->ysize*0.5;
	yc=(double)r->y+b;
	for(n=0;n<steps;++n){
		theta += dtheta;
		x=xc+a*cos(theta)+0.5;
		y=yc+b*sin(theta)+0.5;
	    yr=s->viewRect.ysize-1-(int)y;
		kpoint((int) x,yr,s->nLineFore,s);
	}
		
	return 0;
}
int uPaintOvalU(uRect *r,struct screenData *s)
{
	static uPattern Pat={{0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44}};
	unsigned char p;
	uPattern *pat;
	double dtheta,theta;
	double x,y,xc,yc,a,b;
	double pi;
	long steps;
	long n,nn;
	int yr1,yr2,ys,xr,yr;
	int k,nc;


	if(!r || !s  || !s->buffer)return 1;

	pat=PatternPattern(s->SolidPattern);
	if(!pat)pat=&Pat;
	
	steps=2*(r->ysize+r->xsize)+4;
	
	pi=4.0*atan(1.0);
	
	dtheta=pi/(double)(steps-1);
	theta = -dtheta;
	
	a=(double)r->xsize*0.5;
	xc=(double)r->x+a;
	b=(double)r->ysize*0.5;
	yc=(double)r->y+b;
	for(n=0;n<steps;++n){
		theta += dtheta;
		x=xc+a*cos(-theta)+0.5;
		y=yc+b*sin(-theta)+0.5;
	    yr1=(int)y;
		x=xc+a*cos(theta)+0.5;
		y=yc+b*sin(theta)+0.5;
	    yr2=(int)y;
	    if(yr1 > yr2){
	       ys=yr1;
	       yr1=yr2;
	       yr2=ys;
	    }
	    xr=(int)x;
	    for(k=yr1;k<=yr2;++k){
			p=pat->pat[k % 8];	    
	    	if((p << (xr % 8)) & 128){
	    	   nc=s->nSolidFore;
	    	}else{
	    	   nc=s->nSolidBack;
	    	}
	        if(xr < 0 || xr >= s->viewRect.xsize)continue;
	        if(k < 0 || k >= s->viewRect.ysize)continue;
	        yr=k;
	        if(s->ScreenBits == RASTER_8){
	            s->buffer[xr+yr*s->viewRect.xsize]=nc;
	        }else if(s->ScreenBits == RASTER_24){
	            unsigned char *pal;
	            pal=s->palette;
	            nn=xr*3+yr*s->viewRect.xsize*3;
	            s->buffer[nn]=pal[nc*3];
	            s->buffer[1+nn]=pal[nc*3+1];
	            s->buffer[2+nn]=pal[nc*3+2];
	        }	            
	    	            	            						
	    }
	}
		
	return 0;
}
int uPaintRectU(uRect *r,struct screenData *s)
{
	static uPattern Pat={{0x11,0x44,0x11,0x44,0x11,0x44,0x11,0x44}};
	unsigned char p;
	uPattern *pat;
	long n,yr;
	int i,j;
	int xs,ys;
	int x,y;
	int nc;


	if(!r || !s  || !s->buffer)return 1;
		
	pat=PatternPattern(s->SolidPattern);
	if(!pat)pat=&Pat;

	xs=r->x;
	ys=r->y;
	for(j=0;j<(int)r->ysize;++j){
	    y=ys+j;
		p=pat->pat[y % 8];
	    for(i=0;i<(int)r->xsize;++i){
	    	x=xs+i;
	    	if((p << (x % 8)) & 128){
	    	   nc=s->nSolidFore;
	    	}else{
	    	   nc=s->nSolidBack;
	    	}
	        if(x < 0 || x >= s->viewRect.xsize)continue;
	        if(y < 0 || y >= s->viewRect.ysize)continue;
	        yr=s->viewRect.ysize-1-y;
	        yr=y;
	        if(s->ScreenBits == RASTER_8){
	            s->buffer[x+yr*s->viewRect.xsize]=nc;
	        }else if(s->ScreenBits == RASTER_24){
	            unsigned char *pal;
	            pal=s->palette;
	            n=x*3+yr*s->viewRect.xsize*3;
	            s->buffer[n]=pal[nc*3];
	            s->buffer[1+n]=pal[nc*3+1];
	            s->buffer[2+n]=pal[nc*3+2];
	        }	            
	    	            	            
	    }
	}
		
	return 0;
}

int SetOldColorPalette(IconPtr myIcon)
{
	int k;
	
	if(!myIcon)return 1;
	for(k=0;k<256;++k){
    	myIcon->redOld[k]=myIcon->red[k];
     	myIcon->greenOld[k]=myIcon->green[k];
     	myIcon->blueOld[k]=myIcon->blue[k];
	}
	return 0;
}

int GetOldColorPalette(IconPtr myIcon)
{
	int k;
	
	if(!myIcon)return 1;
	for(k=0;k<256;++k){
    	myIcon->red[k]=myIcon->redOld[k];
     	myIcon->green[k]=myIcon->greenOld[k];
     	myIcon->blue[k]=myIcon->blueOld[k];
	}
	return 0;
}

static int doPointRange(struct linedata *li)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {260,270,70,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {340,270,70,20},uDialogButton,uDialogNone},
      /*  2 */      {"x data", {25,120,80,16},uDialogText,uDialogGetDouble},
      /*  3 */      {"y data", {122,120,80,16},uDialogText,uDialogGetDouble},
      /*  4 */      {"Pioname data", {125,60,170,20},uDialogText,uDialogNone},
      /*  5 */      {"z data", {25,170,80,16},uDialogText,uDialogGetDouble},
      /*  6 */      {"pioIndex data", {306,60,65,20},uDialogText,uDialogGetLong},
      /*  7 */      {"Frame 1 data", {25,223,80,16},uDialogText,uDialogGetLong},
      /*  8 */      {"Frame last data", {122,223,80,16},uDialogText,uDialogGetLong},
      /*  9 */      {"Time First data", {215,223,80,16},uDialogText,uDialogGetDouble},
      /* 10 */      {"Time Last Data", {319,223,80,16},uDialogText,uDialogGetDouble},
      /* 11 */      {"Frame data", {122,170,80,16},uDialogText,uDialogGetLong},
      /* 12 */      {"First Time", {215,200,83,16},uDialogString,uDialogNone},
      /* 13 */      {"Last Time", {319,200,76,16},uDialogString,uDialogNone},
      /* 14 */      {"x", {25,100,82,16},uDialogString,uDialogNone},
      /* 15 */      {"y", {122,100,75,16},uDialogString,uDialogNone},
      /* 16 */      {"z", {25,150,79,16},uDialogString,uDialogNone},
      /* 17 */      {"First Frame", {25,200,86,16},uDialogString,uDialogNone},
      /* 18 */      {"Last Frame", {122,200,80,16},uDialogString,uDialogNone},
      /* 19 */      {"Frames", {122,150,59,16},uDialogString,uDialogNone},
      /* 20 */      {"Name", {125,40,75,16},uDialogString,uDialogNone},
      /* 21 */      {"Index", {306,40,75,16},uDialogString,uDialogNone},      
      /* 22 */      {"Source", {25,0,100,16},uDialogString,uDialogNone},
      /* 23 */      {"sourceName data", {25,20,370,17},uDialogText,uDialogNone},
      /* 24 */      {"Raw Data",  {5+210,245-135,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 25 */      {"Polynomial",{5+210,265-135,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 26 */      {"Smooth",  {5+210,285-135,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 27 */      {"Fourier",  {5+210,305-135,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 28 */      {"3", {108+210,305-135,75,18},uDialogText,uDialogGetLong},
      /* 29 */      {"Order", {108+210,285-135,95,18},uDialogString,uDialogNone},
      /* 30 */      {"Print", {108+210,245-135,95,18},uDialogCheck,uDialogGetLong},
      /* 31 */      {"Distance", {11,60,100,20},uDialogButtonRadio,uDialogGetLong},
      /* 32 */      {"Cross Plots", {11,80,100,20},uDialogButtonRadio,uDialogGetLong},
      /* 33 */      {"pres", {125,80,170,20},uDialogText,uDialogNone},
      /* 34 */      {"0", {306,80,65,20},uDialogText,uDialogGetLong},
      
      /* 35 */      {"Radius", {25,250,100,16},uDialogString,uDialogNone},
      /* 36 */      {"Steps", {122,250,100,16},uDialogString,uDialogNone},
      /* 37 */      {"Radius data", {25,270,80,16},uDialogText,uDialogGetDouble},
      /* 38 */      {"Count data", {122,270,80,16},uDialogText,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {37,41,465,310};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
								
	if(!li)return 1;


	uGetPort(&save);
	
	if(li->type != LINEDATA_POINT_DATA)return 1;

	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Point Plot Data Setup",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)li->x1,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)li->y1,
	              
	              uDialogSetItem,4L,
	              uDialogSetText,li->pioName,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)li->z1,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)li->pioIndex,

	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)li->FirstFrame,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)li->LastFrame,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)li->FirstTime,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)li->LastTime,
	              
	              uDialogSetItem,11L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)li->FrameCount,
	              
	              	              
	              uDialogSetItem,23L,
	              uDialogSetText,li->sourceName,
	              
	              uDialogSetItem,24L,
	              uDialogSetLogical,(li->Polynomial == 0),
	              
	              uDialogSetItem,25L,
	              uDialogSetLogical,(li->Polynomial == 1),
	              
	              uDialogSetItem,26L,
	              uDialogSetLogical,(li->Polynomial == 2),
	              
	              uDialogSetItem,27L,
	              uDialogSetLogical,(li->Polynomial == 3),
	              
	              uDialogSetItem,28L,
	              uDialogSetDouble,(double)li->PolynomialOrder,
	              
	              uDialogSetItem,30L,
	              uDialogSetLogical,li->PolynomialPrint,
	              
	              
	              uDialogSetItem,31L,
	          	  uDialogSetRadioGroup,3,
	              uDialogSetLogical,(li->LineSourceMode == LINEDATA_DISTANCE_MODE),
	              
	              uDialogSetItem,32L,
	          	  uDialogSetRadioGroup,3,
	              uDialogSetLogical,(li->LineSourceMode == LINEDATA_CROSSPLOT_MODE),              	              
	              
	              
	              uDialogSetItem,33L,
	              uDialogSetHidden,(li->LineSourceMode == LINEDATA_DISTANCE_MODE),
	              uDialogSetText,li->pioName2,
	              
	              uDialogSetItem,34L,
	              uDialogSetHidden,(li->LineSourceMode == LINEDATA_DISTANCE_MODE),
	              uDialogSetDouble,(double)li->pioIndex2,	              
              	              
	              uDialogSetItem,37L,
	              uDialogSetDouble,(double)li->SampleRadius,
	              
	              uDialogSetItem,38L,
	              uDialogSetDouble,(double)li->SampleSteps,
	              
	  	          uDialogSetAction,doScale1Action,            
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	if(uR[7].lreturn < 0){
	    Warning("First Frame Must Be Greater Than Or Equal To Zero");
 	    goto Start;
	}
	if((uR[7].lreturn >= uR[8].lreturn)){
	    Warning("First Frame Must Be Less Than Last Frame");
 	    goto Start;
	}

	if((uR[7].lreturn >= uR[11].lreturn)){
	    Warning("Last Frame Must Be Less Than Total Number Of Frame");
 	    goto Start;
	}

	if((uR[9].dreturn >= uR[10].dreturn)){
	    Warning("First Time Must Be Less Than Last Time");
 	    goto Start;
	}
	
	if((uR[25].lreturn > 0 || uR[26].lreturn > 0 || uR[27].lreturn > 0) && (uR[28].lreturn < 1)){
	    Warning("Order Must Be Greater Than One");
 	    goto Start;
	}



	li->x1=uR[2].dreturn;
	li->y1=uR[3].dreturn;
	mstrncpy(li->pioName,uR[4].sreturn,255);
	li->z1=uR[5].dreturn;
	li->pioIndex=uR[6].lreturn;
	li->FirstFrame=uR[7].lreturn;
	li->LastFrame=uR[8].lreturn;
	li->FirstTime=uR[9].dreturn;
	li->LastTime=uR[10].dreturn;
	li->SampleRadius=uR[37].dreturn;
	if(li->SampleRadius < 0)li->SampleRadius=0;
	li->SampleSteps=uR[38].lreturn;
	if(li->SampleSteps < 1)li->SampleSteps=1;
	mstrncpy(li->sourceName,uR[23].sreturn,255);
	
	li->Polynomial=0;
	if(uR[24].lreturn)li->Polynomial=0;
	if(uR[25].lreturn)li->Polynomial=1;
	if(uR[26].lreturn)li->Polynomial=2;	
	if(uR[27].lreturn)li->Polynomial=3;	
	li->PolynomialOrder=(int)uR[28].lreturn;
	li->PolynomialPrint=(int)uR[30].lreturn;

	li->LineSourceMode=LINEDATA_CROSSPLOT_MODE;
	if(uR[31].lreturn > 0)li->LineSourceMode=LINEDATA_DISTANCE_MODE;
	
	mstrncpy(li->pioName2,uR[33].sreturn,255);
	li->pioIndex2=uR[34].lreturn;		
	

	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
}

static int doScale1Action(struct uDialogStruct *uList)
{
	uDialogReturn *uR;
	int LineSourceMode;
	int actionItem;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto OutOfHere;
	
	
	LineSourceMode=(int)uR[31].dreturn;
		
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 31){
		LineSourceMode=LINEDATA_DISTANCE_MODE;	
	}else if(actionItem == 32){
		LineSourceMode=LINEDATA_CROSSPLOT_MODE;	
	}

	if(uDialogSet(uList,2L,

          uDialogSetItem,33L,
          uDialogSetHidden,(LineSourceMode == LINEDATA_DISTANCE_MODE),
          
          uDialogSetItem,34L,
          uDialogSetHidden,(LineSourceMode == LINEDATA_DISTANCE_MODE),
        
          uDialogSetDone,uDialogSetDone
	))goto OutOfHere;
	
	

OutOfHere:
	if(uList){
		uDialogUpdate(uList);
		uSetCursor(uList->myIcon,-uARROW_CURSOR);
	}
    if(uR)uDialogFreeReturn(uR);
	return 0;
}


int doBhangMeter(struct linedata *li)
{
	static uDialogGroup Items[] =
	
	{
		/* 00 */      {"Ok", {239,170,65,20},uDialogButton,uDialogNone},
		/* 01 */      {"Cancel", {319,170,65,20},uDialogButton,uDialogNone},
		
		/* 02 */      {"0", {77,15,110,20},uDialogText,uDialogGetDouble},
		/* 03 */      {"0", {197,15,110,20},uDialogText,uDialogGetDouble},
		/* 04 */      {"0", {317,15,110,20},uDialogText,uDialogGetDouble},
		/* 05 */      {"location", {5,15,65,20},uDialogString,uDialogNone},
		
		/* 06 */      {"0", {77,45,110,20},uDialogText,uDialogGetDouble},
		/* 07 */      {"0", {197,45,110,20},uDialogText,uDialogGetDouble},
		/* 08 */      {"0", {317,45,110,20},uDialogText,uDialogGetDouble},
		/* 09 */      {"normal", {5,45,65,20},uDialogString,uDialogNone},
		/* 10 */      {"multigroup", {100,75,110,20},uDialogCheck,uDialogGetLong},
	
		/* 11 */      {"0", {77,135,110,20},uDialogText,uDialogGetDouble},
		/* 12 */      {"height", {5,135,65,20},uDialogString,uDialogNone},
		
		/* 13 */      {"0", {77,105,110,20},uDialogText,uDialogGetDouble},
		/* 14 */      {"radius", {5,105,65,20},uDialogString,uDialogNone},
		
		/* 15 */      {"0", {317,105,110,20},uDialogText,uDialogGetLong},
		/* 16 */      {"steps", {250,105,55,20},uDialogString,uDialogNone},
	};
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {37,41,460,205};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
	
	if(!li)return 1;
	
	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("BhangMeter Dialog",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
				  
	              uDialogSetItem,2L,
	              uDialogSetDouble,li->x2,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)li->y2,
	              
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,li->z2,
	              
				  
				  
	              uDialogSetItem,6L,
	              uDialogSetDouble,li->nx,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)li->ny,
	              
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,li->nz,
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(int)li->multigroup,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,li->bhangMeterHeight,
				  
	              uDialogSetItem,13L,
	              uDialogSetDouble,li->bhangMeterRadius,
				  
	              uDialogSetItem,15L,
	              uDialogSetDouble,(double)li->bhangMeterSteps,
				  
	              uDialogSetDone,uDialogSetDone
				  ))goto ErrorOut;
    
	
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);
	
	if(!uR)goto ErrorOut;
	
	li->x2=uR[2].dreturn;
	li->y2=uR[3].dreturn;
	li->z2=uR[4].dreturn;
		
	li->nx=uR[6].dreturn;
	li->ny=uR[7].dreturn;
	li->nz=uR[8].dreturn;
	li->multigroup=(int)uR[10].lreturn;
	li->bhangMeterHeight=uR[11].dreturn;
	li->bhangMeterRadius=uR[13].dreturn;
	li->bhangMeterSteps=(int)uR[15].lreturn;
		
	ret = 0;
	
ErrorOut:
	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
}

int doHistogram(struct linedata *li)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {239,98,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {319,98,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"pioName data", {125,60,168,20},uDialogText,uDialogNone},
      /*  3 */      {"PioIndex data", {306,60,65,20},uDialogText,uDialogGetLong},
      /*  4 */      {"sourceName data", {11,20,370,18},uDialogText,uDialogNone},
      
      
      /*  5 */      {"Name", {125,40,75,18},uDialogString,uDialogNone},
      /*  6 */      {"Index", {306,40,75,18},uDialogString,uDialogNone},
      /*  7 */      {"Source", {11,0,100,18},uDialogString,uDialogNone},

      /*  8 */      {"3", {11,105,75,18},uDialogText,uDialogGetLong},
      /*  9 */      {"bins", {11,85,95,18},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {37,41,460,145};
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
								
	if(!li)return 1;

	uGetPort(&save);

	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Histogram",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	
	if(uDialogSet(uList,2L,
		              	              
	              uDialogSetItem,2L,
	              uDialogSetText,li->pioName,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)li->pioIndex,
	              
	              
	              uDialogSetItem,4L,
	              uDialogSetText,li->sourceName,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)li->PointLineCount,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	if(uR[8].lreturn <= 0){
	    Warning("Need At Least One Bin");
	    goto Start;
	}

	mstrncpy(li->pioName,uR[2].sreturn,255);
	li->pioIndex=uR[3].lreturn;	
	mstrncpy(li->sourceName,uR[4].sreturn,255);
	li->PointLineCount=(int)uR[8].lreturn;	
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
}


int doLineRange(struct linedata *li)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {239,278,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {319,278,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"x1 data", {10,150,105,18},uDialogText,uDialogGetDouble},
      /*  3 */      {"y1 data", {120,150,105,18},uDialogText,uDialogGetDouble},
      /*  4 */      {"x2 data", {230,150,105,18},uDialogText,uDialogGetDouble},
      /*  5 */      {"y2 data", {340,150,105,18},uDialogText,uDialogGetDouble},
      /*  6 */      {"pioName data", {125,60,168,20},uDialogText,uDialogNone},
      /*  7 */      {"PioIndex data", {306,60,65,20},uDialogText,uDialogGetLong},
      /*  8 */      {"z1 data", {10,196,105,18},uDialogText,uDialogGetDouble},
      /*  9 */      {"z2 data", {120,196,105,18},uDialogText,uDialogGetDouble},
      /* 10 */      {"n data", {230,196,105,18},uDialogText,uDialogGetLong},
      /* 11 */      {"Frame data", {305,245,75,18},uDialogText,uDialogGetLong},
      /* 12 */      {"Frames data", {206,245,75,18},uDialogText,uDialogGetLong},
      /* 13 */      {"z1", {10,174,75,18},uDialogString,uDialogNone},
      /* 14 */      {"z2", {120,174,75,18},uDialogString,uDialogNone},
      /* 15 */      {"n", {230,174,75,18},uDialogString,uDialogNone},
      /* 16 */      {"x1", {10,125,75,18},uDialogString,uDialogNone},
      /* 17 */      {"y1", {120,125,75,18},uDialogString,uDialogNone},
      /* 18 */      {"x2", {230,125,75,18},uDialogString,uDialogNone},
      /* 19 */      {"y2", {340,125,75,18},uDialogString,uDialogNone},
      /* 20 */      {"Frame", {305,220,75,18},uDialogString,uDialogNone},
      /* 21 */      {"Frames", {206,220,75,18},uDialogString,uDialogNone},
      /* 22 */      {"Name", {125,40,75,18},uDialogString,uDialogNone},
      /* 23 */      {"Index", {306,40,75,18},uDialogString,uDialogNone},
      /* 24 */      {"Source", {11,0,100,18},uDialogString,uDialogNone},
      /* 25 */      {"sourceName data", {11,20,370,18},uDialogText,uDialogNone},
      /* 26 */      {"Raw Data",  {5,225,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 27 */      {"Polynomial",{5,245,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 28 */      {"Smooth",  {5,265,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 29 */      {"Fourier",  {5,285,100,18},uDialogButtonRadio,uDialogGetLong},
      /* 30 */      {"3", {108,285,75,18},uDialogText,uDialogGetLong},
      /* 31 */      {"Order", {108,265,95,18},uDialogString,uDialogNone},
      /* 32 */      {"Print", {108,225,95,18},uDialogCheck,uDialogGetLong},
      /* 33 */      {"radius", {230,125,75,18},uDialogString,uDialogNone},
      /* 34 */      {"angle", {340,125,75,18},uDialogString,uDialogNone},
      
      /* 35 */      {"x-y",{11,107,60,18},uDialogButtonRadio,uDialogGetLong},
      /* 36 */      {"r-theta",{70,107,100,18},uDialogButtonRadio,uDialogGetLong},

      /* 37 */      {"Direction", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
      
      /* 38 */      {"pres", {125,80,168,20},uDialogText,uDialogNone},
      /* 39 */      {"0", {306,80,65,20},uDialogText,uDialogGetLong},
     
      /* 40 */      {"Distance", {11,60,100,20},uDialogButtonRadio,uDialogGetLong},
      /* 41 */      {"Cross Plots", {11,80,100,20},uDialogButtonRadio,uDialogGetLong},

      /* 42 */      {"Integrate", {108,245,95,18},uDialogCheck,uDialogGetLong},

     
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {37,41,460,325};
	uPort save;
	
	double x1,y1,x2,y2,r,t,pi;
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
								
	if(!li)return 1;

	uGetPort(&save);

	if(li->type == LINEDATA_POINT_DATA){
	    return doPointRange(li);
	}else if(li->type == LINEDATA_HISTOGRAM_DATA){
	    return doHistogram(li);
	}else if(li->type == LINEDATA_BHANGMETER_DATA){
	    return doBhangMeter(li);
	}

	ret = 1;
	
	uR=NULL;
	
	uList=uDialogOpen("Line Plot Data Setup",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	x1=li->x1;
	x2=li->x2;
	y1=li->y1;
	y2=li->y2;

	pi=4.0*atan(1.0);
	
	if(li->LineSelectionMode == LINEDATA_SELECTION_MODE_RT){
		r=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		t=atan2((y2-y1),(x2-x1))*180.0/pi;
		x2=r;
		y2=t;
	}
	
	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)x1,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)y1,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)x2,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)y2,
	              	              
	              uDialogSetItem,6L,
	              uDialogSetText,li->pioName,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)li->pioIndex,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)li->z1,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)li->z2,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)li->PointLineCount,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)li->CurrentFrame,
	              
	              uDialogSetItem,12L,
	              uDialogSetActive,0,
	              uDialogSetDouble,(double)li->FrameCount,
	              
	              uDialogSetItem,25L,
	              uDialogSetText,li->sourceName,
	              
	              uDialogSetItem,26L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(li->Polynomial == 0),
	              
	              uDialogSetItem,27L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(li->Polynomial == 1),
	              
	              uDialogSetItem,28L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(li->Polynomial == 2),
	              
	              uDialogSetItem,29L,
	          	  uDialogSetRadioGroup,1,
	              uDialogSetLogical,(li->Polynomial == 3),
	              
	              uDialogSetItem,30L,
	              uDialogSetDouble,(double)li->PolynomialOrder,
	              
	              uDialogSetItem,32L,
	              uDialogSetLogical,li->PolynomialPrint,

	              uDialogSetItem,18L,
	              uDialogSetHidden,(li->LineSelectionMode != LINEDATA_SELECTION_MODE_XY),
	              
	              uDialogSetItem,19L,
	              uDialogSetHidden,(li->LineSelectionMode != LINEDATA_SELECTION_MODE_XY),
	              
	              
	              uDialogSetItem,33L,
	              uDialogSetHidden,(li->LineSelectionMode != LINEDATA_SELECTION_MODE_RT),
	              
	              uDialogSetItem,34L,
	              uDialogSetHidden,(li->LineSelectionMode != LINEDATA_SELECTION_MODE_RT),
	              	              
	              uDialogSetItem,35L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(li->LineSelectionMode == LINEDATA_SELECTION_MODE_XY),
	              
	              uDialogSetItem,36L,
	          	  uDialogSetRadioGroup,2,
	              uDialogSetLogical,(li->LineSelectionMode == LINEDATA_SELECTION_MODE_RT),
	              	              
	              uDialogSetItem,37L,
		          uDialogSetFloatAux,(double)li->LineSelectionMode,
              	              
	              uDialogSetItem,38L,
	              uDialogSetHidden,(li->LineSourceMode == LINEDATA_DISTANCE_MODE),
	              uDialogSetText,li->pioName2,
	              
	              uDialogSetItem,39L,
	              uDialogSetHidden,(li->LineSourceMode == LINEDATA_DISTANCE_MODE),
	              uDialogSetDouble,(double)li->pioIndex2,	              
              	              
	              uDialogSetItem,40L,
	          	  uDialogSetRadioGroup,3,
	              uDialogSetLogical,(li->LineSourceMode == LINEDATA_DISTANCE_MODE),
	              
	              uDialogSetItem,41L,
	          	  uDialogSetRadioGroup,3,
	              uDialogSetLogical,(li->LineSourceMode == LINEDATA_CROSSPLOT_MODE),              	              

	              uDialogSetItem,42L,
	              uDialogSetLogical,li->integrate,

              	              
	  	          uDialogSetAction,doScale2Action,            
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;


	if((uR[2].dreturn == uR[4].dreturn) && (uR[3].dreturn == uR[5].dreturn) &&
	   (uR[8].dreturn == uR[9].dreturn)){
	    Warning("End Points Must Be Different");
 	    goto Start;
	}


	if((uR[11].lreturn < 0) || (uR[11].lreturn >= li->FrameCount)){
	    sprintf(WarningBuff,"Frame %ld Out of Range (FrameCountfiles %ld)",(long)uR[11].lreturn,
	                    (long)(li->FrameCount-1));
	    Warning(WarningBuff);
 	    goto Start;
	}
	
	
	if((uR[27].lreturn > 0 || uR[28].lreturn > 0 || uR[29].lreturn > 0) && (uR[30].lreturn < 1)){
	    Warning("Order Must Be Greater Than One");
 	    goto Start;
	}
	
	x1=uR[2].dreturn;
	y1=uR[3].dreturn;
	r=x2=uR[4].dreturn;
	t=y2=uR[5].dreturn;
	mstrncpy(li->pioName,uR[6].sreturn,255);
	li->pioIndex=uR[7].lreturn;	
	li->z1=uR[8].dreturn;
	li->z2=uR[9].dreturn;
	li->PointLineCount=(int)uR[10].lreturn;
	li->CurrentFrame=uR[11].lreturn;
	mstrncpy(li->sourceName,uR[25].sreturn,255);
	li->Polynomial=0;
	if(uR[26].lreturn)li->Polynomial=0;
	if(uR[27].lreturn)li->Polynomial=1;
	if(uR[28].lreturn)li->Polynomial=2;	
	if(uR[29].lreturn)li->Polynomial=3;	
	li->PolynomialOrder=(int)uR[30].lreturn;
	li->PolynomialPrint=(int)uR[32].lreturn;
	
	li->LineSelectionMode=(int)uR[37].dreturn;
	
	
	li->LineSourceMode=LINEDATA_CROSSPLOT_MODE;
	if(uR[40].lreturn > 0)li->LineSourceMode=LINEDATA_DISTANCE_MODE;
	
	mstrncpy(li->pioName2,uR[38].sreturn,255);
	li->pioIndex2=uR[39].lreturn;		
	
	li->integrate=(int)uR[42].lreturn;
	if(li->integrate)li->LineSourceMode=LINEDATA_DISTANCE_MODE;
	
	
	if(li->LineSelectionMode == LINEDATA_SELECTION_MODE_RT){
	    x2=x1+r*cos(t*pi/180.);
	    y2=y1+r*sin(t*pi/180.);		    
	}
	
	li->x1=x1;
	li->y1=y1;
	li->x2=x2;
	li->y2=y2;
	
	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	return ret;
}
static int doScale2Action(struct uDialogStruct *uList)
{
	uDialogReturn *uR;
	int LineSelectionMode,LineSourceMode;
	int actionItem;
	double x1,y1,x2,y2,r,t,pi;
	
	if(!uList)return 1;
	
	actionItem=(int)uList->actionItem;
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto OutOfHere;
	
	x1=uR[2].dreturn;
	y1=uR[3].dreturn;
	r=x2=uR[4].dreturn;
	t=y2=uR[5].dreturn;

	LineSelectionMode=(int)uR[37].dreturn;
	
	LineSourceMode=(int)uR[40].dreturn;
		
	pi=4.0*atan(1.0);
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	if(actionItem == 35){
		if(LineSelectionMode == LINEDATA_SELECTION_MODE_RT){
		    x2=x1+r*cos(t*pi/180.);
		    y2=y1+r*sin(t*pi/180.);		    
		}
	    LineSelectionMode=LINEDATA_SELECTION_MODE_XY;
	
		if(uDialogSet(uList,2L,

              uDialogSetItem,4L,
              uDialogSetDouble,(double)x2,
              
              uDialogSetItem,5L,
              uDialogSetDouble,(double)y2,
	              	                          
              uDialogSetDone,uDialogSetDone
    	))goto OutOfHere;
	
	
	}else if(actionItem == 36){
		if(LineSelectionMode == LINEDATA_SELECTION_MODE_XY){
		    r=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		    t=atan2((y2-y1),(x2-x1))*180.0/pi;
		}
	    LineSelectionMode=LINEDATA_SELECTION_MODE_RT;
	
		if(uDialogSet(uList,2L,
		
              uDialogSetItem,4L,
              uDialogSetDouble,(double)r,
              
              uDialogSetItem,5L,
              uDialogSetDouble,(double)t,
              
              uDialogSetDone,uDialogSetDone
    	))goto OutOfHere;
    
	}else if(actionItem == 40){
		LineSourceMode=LINEDATA_DISTANCE_MODE;	
	}else if(actionItem == 41){
		LineSourceMode=LINEDATA_CROSSPLOT_MODE;	
	}

	if(uDialogSet(uList,2L,


          uDialogSetItem,18L,
          uDialogSetHidden,(LineSelectionMode != LINEDATA_SELECTION_MODE_XY),
          
          uDialogSetItem,19L,
          uDialogSetHidden,(LineSelectionMode != LINEDATA_SELECTION_MODE_XY),
          
          
          uDialogSetItem,33L,
          uDialogSetHidden,(LineSelectionMode != LINEDATA_SELECTION_MODE_RT),
          
          uDialogSetItem,34L,
          uDialogSetHidden,(LineSelectionMode != LINEDATA_SELECTION_MODE_RT),
          	              
          uDialogSetItem,35L,
          uDialogSetLogical,(LineSelectionMode == LINEDATA_SELECTION_MODE_XY),
          
          uDialogSetItem,36L,
          uDialogSetLogical,(LineSelectionMode == LINEDATA_SELECTION_MODE_RT),
          	              
          uDialogSetItem,37L,
          uDialogSetFloatAux,(double)LineSelectionMode,
          
          uDialogSetItem,38L,
          uDialogSetHidden,(LineSourceMode == LINEDATA_DISTANCE_MODE),
          
          uDialogSetItem,39L,
          uDialogSetHidden,(LineSourceMode == LINEDATA_DISTANCE_MODE),
          
          uDialogSetDone,uDialogSetDone
	))goto OutOfHere;
	
	

OutOfHere:
	if(uList){
		uDialogUpdate(uList);
		uSetCursor(uList->myIcon,-uARROW_CURSOR);
	}
    if(uR)uDialogFreeReturn(uR);
	return 0;
}

int FileListFree(struct FileList *Files)
{
	long n;
	
	if(!Files)return 1;
	if(Files->name){
		for(n=0;n<Files->count;++n){
			if(Files->name[n])cFree((char *)Files->name[n]);
			Files->name[n]=NULL;
	    }
	    cFree((char *)Files->name);
	    Files->name=NULL;
	}
	
	if(Files->directory){
	    cFree((char *)Files->directory);
	    Files->directory=NULL;
	}
	if(!Files->donotfree)cFree((char *)Files);
	return 0;
}

int drawLineOnScreen(IconPtr myIcon,uPoint *p1,uPoint *p2)
{
	uPoint po;
	int start;
	
	if(!myIcon || !p1 || !p2 )return 1;
		
	uPointer(p1,myIcon);
	start=TRUE;
	po = *p1;
	while(uStillDown(myIcon)){
		uPointer(p2,myIcon);
		uSetPenXOR(TRUE,myIcon);
	    if(po.x != p2->x || po.y != p2->y){
	        if(!start){
		        uMoveTo(p1->x,p1->y);
		        uLineTo(po.x,po.y,myIcon);
	        } 
		    po = *p2;
		    uMoveTo(p1->x,p1->y);
		    uLineTo(po.x,po.y,myIcon);
		    
			uSetPenXOR(FALSE,myIcon);
			if(myIcon->uWriteCursor){
				(*myIcon->uWriteCursor)(po,myIcon);
			}
		    
		    start=FALSE;
	    }
	}
	if(!start){
		uSetPenXOR(TRUE,myIcon);
	    uMoveTo(p1->x,p1->y);
	    uLineTo(po.x,po.y,myIcon);
	}

	uSetPenXOR(FALSE,myIcon);
	
	if(start)return 1;

	return 0;
}
int drawBoxOnScreen(IconPtr myIcon,uRect *box,int (*uDraw)(struct Icon *,struct SyncDrawInfo *DSync))
{
	uPoint start,current,last;
	int isDrawn;
	uRect oldR;
	struct SyncDrawInfo DSync;
	
	if(!myIcon || !box)return 1;
	
	uSetPenXOR(TRUE,myIcon);
	
	uPointer(&start,myIcon);
	DSync.start=start;
	last=start;
	oldR.y=0;
	oldR.x=0;
	oldR.xsize=0;
	oldR.ysize=0;
	isDrawn=FALSE;
	while(uStillDown(myIcon)){
		    uPointer(&current,myIcon);
		    if((current.x == last.x) && (current.y == last.y))continue;
		    if(isDrawn){
			    uFrameRect(&oldR,myIcon);
		    }
		    isDrawn=TRUE;
		    last=current;
		    if(start.x < last.x){
		        oldR.x=start.x;
		        oldR.xsize=last.x-start.x;
		    }else{
		        oldR.x=last.x;
		        oldR.xsize=start.x-last.x;
		    }
		    if(start.y < last.y){
		        oldR.y=start.y;
		        oldR.ysize=last.y-start.y;
		    }else{
		        oldR.y=last.y;
		        oldR.ysize=start.y-last.y;
		    }
		    uFrameRect(&oldR,myIcon);
		    if(uDraw){
		         DSync.current=current;
		         DSync.DRect.x=oldR.x;
		         DSync.DRect.y=oldR.y;
		         DSync.DRect.xsize=oldR.xsize;
		         DSync.DRect.ysize=oldR.ysize;
		        (*uDraw)(myIcon,&DSync);
		    }
	}

	if(isDrawn){
		uFrameRect(&oldR,myIcon);
	}
	
	box->x=oldR.x;
	box->y=oldR.y;
	
	box->xsize=oldR.xsize;
	box->ysize=oldR.ysize;
	
	uSetPenXOR(FALSE,myIcon);
	
	return 0;
}
struct FileList *uOpenFileDialog(struct Icon *myIcon,int *TypeList)
{
    static struct FileListInfo infoLoad;
	struct FileList *Filesl;
    struct FileInfo2 *Files;
	int n;
	
	if(uPref.LocalOpenDialog && isInstalled(Installed_LocalOpenDialog))return uOpenFileDialogLocal(myIcon,TypeList);

	if(!myIcon){
		Filesl=FileDialog(NULL,"File Selection Dialog",FileDialog_GetFiles,&infoLoad);
	}else{		
		Filesl=FileDialog(NULL,"File Selection Dialog",FileDialog_GetFiles,&myIcon->infoLoad);
	}		
	if(!Filesl)return NULL;

	if(FilesSetType(Filesl)){
		FileListFree(Filesl);
	    return NULL;
	}
	
	if(TypeList){
	    n=0;
	    while(TypeList[n]){
	        if((TypeList[n] == OPEN_ALL_FILE_TYPES) || (Filesl->type == TypeList[n++])){
	            return Filesl;
	        }
	    }
	}
	
	
	if(Filesl->type == VIEWLIB_MEMORY){
	    OpenViewLib(Filesl);
	}else{
		Files=FilesOpenList(Filesl);
	    if(Files){
	        if(FilesOpenDefault(Files)){
	            return NULL;
	        }
	    }
    }
	    
	return NULL;
}

int uActivateMenuItem(struct Icon *myIcon,int menu,int item,int value)
{
	if(value){
	    return uEnableMenuItem(myIcon,menu,item);
	}
	
	return uDisableMenuItem(myIcon,menu,item);

}

int SetWindowToFileName(IconPtr myIcon,struct FileInfo2 *Files)
{
	static int Count;
	char buff[256];
	char *tag;
	
	if(!myIcon || !Files)return 1;
	
    if(Files->FilesNames && Files->FilesNames[0]){
        tag=FilesClassTag(Files);
        if(tag){
        	sprintf(buff,"%s[%s.%03d]%c",Files->FilesNames[0],tag,Count++,0);
        }else{
        	sprintf(buff,"%s[%03d]%c",Files->FilesNames[0],Count++,0);
        }
		SetWindowName(myIcon,buff);	
    }
	    
	return 0;
}
int sear(double cmax,double cmin,double *zcon,int *ncon,int nmax)
{
      static double d[3]={2.0,5.0,10.0};
      double delz,fnx,dz,zcmin,z;
      int k,kk;
      long nn,n;
      
      if(cmax < cmin)cmax=cmin;
      delz=cmax-cmin;
      if(delz < 0.0 || nmax == 0){
          *ncon=0;
          return 1;
      }
      if(nmax < 0){
          *ncon = -nmax;
          return 0;
      }
      fnx=nmax+.01;
      dz=delz/fnx;
      n=(long)log10(dz);
      if(dz < .9999)n=n-1;
      dz=dz/pow(10.0,(double)n);
      for(k=0;k<3;++k){
         if(dz <= d[k])break;
      }
      dz=d[k]*pow(10.0,(double)n);
      
      if(fabs(cmin/dz) > 2e9){
          zcon[0]=cmin;
          zcon[1]=cmax;
          *ncon=2;
          return 1;
      }
      nn=(long)(cmin/dz);
      if(cmin < 0.0)nn=nn-1;
      zcmin=((double)nn*dz);
      if(zcmin < cmin)zcmin=zcmin-dz;
      zcon[0]=z=zcmin;
      k=0;
      do {
		k=k+1;
		z += dz;
        zcon[k]=z;
      }while(z <= (cmax+1.e-6*dz));
      *ncon=k;
      for(kk=0;kk<k;++kk){
          if(fabs(zcon[kk]) < 1.e-6*dz){
              zcon[kk]=0.;
          } else {
              if(zcon[kk] >= 0.0){
              	zcon[kk]=zcon[kk]+1.e-8*dz;
              }else{
              	zcon[kk]=zcon[kk]-1.e-8*dz;
              }
          }
      }
      return 0;
}
int SynchronizePalettes(IconPtr myIcon,unsigned char *red,unsigned char *green,unsigned char *blue)
{
	struct Message1000 m;
	IconPtr p;

	if(!myIcon)return 1;
	
	m.red=red;
	m.green=green;
	m.blue=blue;
	
	p=IconRoot;
	
	while(p){
	    SetPortMyWindow(p);
		SendMessageToAll(myIcon,0,MessageType_SET_PALETTE,(void *)&m);
	    p=p->IconNext;
	}
	
	SetPortMyWindow(myIcon);
	
	return 0;
}

IconPtr FindWindowByName(char *name)
{
	IconPtr p;
	char file[256];
	
	if(!name)return NULL;
	
	p=IconRoot;
	while(p){
		GetWindowName(p,file,0);
		if(!strcmp(file,name)){
	    	break;
		}
	    p=p->IconNext;
	}
	
	return p;
	
}

int SendMessageToAll(IconPtr myIcon,int SendToSelf,
                long MessageType,void *message)
{       
        IconPtr p;
 
        p=IconRoot;

        while(p){
            if(p->pMessage){
                if((p != myIcon) || SendToSelf)(*p->pMessage)(p,MessageType,message); 
            }
            p=p->IconNext;
        }
        return 0;
}
int SendMessageByName(char *name,long MessageType,void *message)
{
	IconPtr myIcon;
    FilePtr Files; 
	
	if(!name)return 1;
	
	myIcon=FindWindowByName(name);
	if(myIcon){
	    return SendMessageByIcon(myIcon,MessageType,message);
	}	
	Files=FileManagerGet(name);
	if(Files){
	    return FileManagerSendMessage(Files,MessageType,message);
	}
		
	return 1;
}
IconPtr FindWindowInList(IconPtr w)
{
	IconPtr p;
	
	p=IconRoot;
	while(p){
	    if(p == w)return p;
	    p=p->IconNext;
	}
	
	return NULL;
}
IconPtr FindTypeWindow(int Type)
{
	IconPtr p;
	
	p=IconRoot;
	
	while(p){		/*	Search for Window	*/
	    if(p->WindowType == Type)return p;
	    p=p->IconNext;
	}
	return NULL;
}
int SendMessageByIcon(IconPtr myIcon,long MessageType,void *message)
{       
    IconPtr p;

	if(!myIcon)return 1;
	
    p=IconRoot;

    while(p){
        if(p->pMessage){
            if(p == myIcon){
                return (*p->pMessage)(p,MessageType,message); 
            }
        }
        p=p->IconNext;
    }
    return 0;
}
int Synchronize(IconPtr myIcon,long CurrentFrame)
{
	struct Message1000 m;
	IconPtr p;
	uPort save;

	if(!myIcon)return 1;

	m.CurrentFrame=CurrentFrame;
	
	uGetPort(&save);
	p=IconRoot;
	
	while(p){
		if(p->Synchronize && (p != myIcon)){
			SetPortMyWindow(p);
			SendMessageByIcon(p,MessageType_SET_FRAME_NUMBER,(void *)&m);
	    }
		p=p->IconNext;
	}
	uSetPort(&save);
	return 0;
}

int CreateNewDirectory(void)
{
	static char out1[256]={'u','n','t','i','t','l','e','d',' ',
			      'f','o','l','d','e','r'};
			      
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Cancel", 			{239,120,80,30},uDialogButton,uDialogNone},
      /*  1 */      {"Ok", 				{135,120,80,30},uDialogButton,uDialogNone},
      /*  2 */      {"", 				{26,58,252,25},uDialogText,uDialogString},
      /*  3 */      {"Folder Name :", 	{80,16,127,20},uDialogString,uDialogNone},
        };
	
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {117,45,340,170};
 	
 	uDialogList *uList;
 	
	uDialogReturn *uR;

    int ret;
    
    ret = 1;
 	
	uR=NULL;
	
	uList=uDialogOpen("New Folder",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
    
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetItem,2L,
	              uDialogSetText,out1,
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;
    
	if(uDialog(uList))goto ErrorOut;    
	
	uR=uDialogGetReturn(uList);

    if(!uR){
	    Warning("Error Getting Search String Specified");
	    goto ErrorOut;
    }
    
	out1[0]=0;
	mstrncpy(out1,uR[2].sreturn,255);
	    
    if(MakeNewDirectory(out1))goto ErrorOut;
	
	ret=0;
 ErrorOut:
 
  	if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
     
 	return ret;
}

int WriteStatus(IconPtr myIcon,char *buff)
{

	if(!myIcon && buff)return 1;

	if(myIcon->pdoWriteStatus){

		(*myIcon->pdoWriteStatus)(myIcon,buff);
	}
	return 0;
}
int SetWindowPath(IconPtr myIconIn,IconPtr myIcon)
{
	if(!myIconIn || !myIcon)return 1;
	
	FileDialogRemoveInfo(&myIcon->infoLoad);
	
	myIcon->infoLoad=myIconIn->infoLoad;
	
	if(myIcon->infoLoad.directory)myIcon->infoLoad.directory=strsave(myIcon->infoLoad.directory,98876);
	
	if(myIcon->infoLoad.name)myIcon->infoLoad.name=strsave(myIcon->infoLoad.name,98877);
	
	FileDialogRecordInfo(&myIcon->infoLoad);
	
	return 0;
}

int FileDialogRecordInfo(struct FileListInfo *info)
{
	int n;
	
	if(!info)return 1;
	
	for(n=0;n<infoListCount;++n){
	    if(infoList[n] == info)return 0;
	}
	if(infoListCount < infoList_Length)infoList[infoListCount++]=info;	
	return 0;
}

int FileDialogRemoveInfo(struct FileListInfo *info)
{
	int n,nl;
	
	if(!info)return 1;
	if(info->directory)cFree((char *)info->directory);
	info->directory=NULL;
	if(info->name)cFree((char *)info->name);
	info->name=NULL;
	
	nl=0;
	for(n=0;n<infoListCount;++n){
	    if(infoList[n] == info)continue;
	    infoList[nl++] = infoList[n];
	}
	
	infoListCount=nl;
	
	return 0;
}
int FileDialogRemoveInfoAll()
{
	struct FileListInfo *info;
	int n;
	
	
	for(n=0;n<infoListCount;++n){
	    info=infoList[n];
	    if(info){
			if(info->directory)cFree((char *)info->directory);
			info->directory=NULL;
			if(info->name)cFree((char *)info->name);
			info->name=NULL;
	    }
	}
	
	infoListCount=0;
	
	return 0;
}


int nWriteBox(IconPtr myIcon,char *buff)
{
	uRect cBox;
	
	if(!myIcon)return 1;
	cBox=myIcon->eraseRect;
	uWriteBox(&cBox,buff,myIcon);
	cBox.x=cBox.x+cBox.xsize-4;
	cBox.xsize=4;
	cBox.ysize=4;
	uPaintBox(&cBox,myIcon);
	return 0;
}
static int doRange2Action(struct uDialogStruct *uList);

static int doRange2Action(struct uDialogStruct *uList)
{
	
	if(!uList)return 1;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 12){
	
		uDialogSet(uList,0L,
	              uDialogSetExitOk,
	              
	              uDialogSetItem,13L,
	              uDialogSetFloatAux,(double)0.0,
	              
	              uDialogSetDone,uDialogSetDone
	    );
 	
	}

	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}

int doWindowInformation(IconPtr myIcon)
{
	static uDialogGroup Items[] =
	
        {
      
      /*  0 */      {"Ok", {75,175,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {170,175,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"Rect", {14,27,295,31},uDialogText,uDialogNone},
      /*  3 */      {"0", {160,67,80,20},uDialogText,uDialogGetLong},
      /*  4 */      {"10", {160,92,80,20},uDialogText,uDialogGetLong},
      /*  5 */      {"1", {160,117,80,20},uDialogText,uDialogGetLong},
      /*  6 */      {"1", {160,142,80,20},uDialogText,uDialogGetLong},
      /*  7 */      {"y         :", {70,92,65,20},uDialogString,uDialogNone},
      /*  8 */      {"Window Name", {95,3,95,20},uDialogString,uDialogNone},
      /*  9 */      {"xsize     :", {70,117,65,20},uDialogString,uDialogNone},
      /* 10 */      {"ysize     :", {70,142,65,20},uDialogString,uDialogNone},
      /* 11 */      {"x         :", {70,67,65,20},uDialogString,uDialogNone},
      /* 12 */      {"File Information", {75,200,150,20},uDialogButton,uDialogNone},
      /* 13 */      {"last", {69,70,169,19},uDialogFloatAux,uDialogFloatAux},
	  /* 14 */      {"File List", {75,225,150,20},uDialogButton,uDialogNone},
      
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,325,260};
	char WindowName[256];
	uPort save;
	
 	uDialogList *uList;
 	uDialogReturn *uR;
 	int FileInformation;
    int ret;
    
	uRect r;

	if(!myIcon)return 1;

	uGetPortBoundsGlobal(myIcon,&r);

	GetWindowName(myIcon,WindowName,0);

	uGetPort(&save);
	
	ret = 1;
	
	uR=NULL;

	FileInformation=FALSE;
	
	uList=uDialogOpen("Window Information",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
		
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,2L,
	              uDialogSetText,WindowName,
		              
	              uDialogSetItem,3L,
		          uDialogSetDouble,(double)r.x,
  	              
	              uDialogSetItem,4L,
            	  uDialogSetDouble,(double)r.y,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)r.xsize,
	              	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)r.ysize,
	              
	              uDialogSetItem,13L,
	              uDialogSetFloatAux,(double)-1,
	              
	              uDialogSetAction,doRange2Action,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	if(uR[13].dreturn == 0.0){
	    FileInformation=TRUE;
	}
	
	if(!strcmp(uR[2].sreturn,"none")){
		myIcon->WindowNameSet=FALSE;
	}else if(strcmp(uR[2].sreturn,WindowName)){
		myIcon->WindowNameSet=FALSE;
		SetWindowName(myIcon,uR[2].sreturn);
		myIcon->WindowNameSet=TRUE;
	}
	
	SetWindowName(myIcon,uR[2].sreturn);

	uMoveWindow(myIcon,(int)uR[3].lreturn,(int)uR[4].lreturn);

	uSizeWindowGlobal(myIcon,(int)uR[5].lreturn,(int)uR[6].lreturn);

	ret = 0;
	
ErrorOut:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	uSetPort(&save);
	
	if(FileInformation){
		struct Message1000 m;
		
		if(uPref.path){
			WarningBatch("Preference File Path\n    ");
			WarningBatch(uPref.path);
			WarningBatch("\n");
		}else{
			WarningBatch("Preference NULL\n");
		}

		if(uPref.startConfiguration){
			WarningBatch("Startup Configuration File Path\n    ");
			WarningBatch(uPref.startConfiguration);
			WarningBatch("\n");
		}
		
		SendMessageToAll(NULL,0,MessageType_PRINT_FILE_INFORMATION,(void *)&m);
	}
	
	return ret;
}
int mstrcmp(char *out,char *in)
{
    char out2[256],in2[256];
    int c,n;
    
	if(!out || !in)return 0;
	
	mstrncpy(out2,out,255);	
	for(n=0;n<255;++n){
	    c=out2[n];
	    if(!c)break;
	    out2[n]=toupper(c);
	}
	
	mstrncpy(in2,in,255);
	for(n=0;n<255;++n){
	    c=in2[n];
	    if(!c)break;
	    in2[n]=toupper(c);
	}
	
	return strcmp(out2,in2);
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
unsigned char *saveblock(unsigned char *buffin,long length,int tab)
{
	unsigned char *data;
	
	if(!buffin)return NULL;
	
	data=(unsigned char *)cMalloc(length,tab);
	if(data == NULL){
	    WarningBatch("saveblock out of Memory\n");
	    return NULL;
	}
	uMemcpy((char *)data,(char *)buffin,length);
	return data;
}
char *strsave(char *s,int tag)
{
	long length;
	char *p;
	
	if(!s)return NULL;
	
	length=(long)strlen(s)+1;
	
	if((p=(char *)cMalloc(length+1,tag)) != NULL)
	mstrncpy(p,s,length);
	return(p);
}
int SetBuffers(long Length)
{
	
	if(Length <= xg.buffsize && xg.buffin && xg.buffout)return 0;

	xg.buffsize=0;

	if(xg.buffin)cFree((char *)xg.buffin);
	xg.buffin=NULL;

	if(xg.buffout)cFree((char *)xg.buffout);
	xg.buffout=NULL;

    if((xg.buffin=(unsigned char *)cMalloc(Length*sizeof(char),8206)) == NULL){
        sprintf(WarningBuff,"SetBuffers Out of Memory (%ld)\n",Length*sizeof(char));
    	WarningBatch(WarningBuff);
        return 1;
    }
    if((xg.buffout=(unsigned char *)cMalloc(Length*sizeof(char),8207)) == NULL){
        sprintf(WarningBuff,"SetBuffers Out of Memory (%ld)\n",Length*sizeof(char));
    	WarningBatch(WarningBuff);
    	if(xg.buffin)cFree((char *)xg.buffin);
    	xg.buffin=NULL;
        return 1;
    }
	xg.buffsize=Length;
    return 0;
}
int uMemcpy(void *out1,void *in1,long length)
{
	unsigned char *out,*in;
	
	if(!out1 || !in1 || (length <= 0))return 1;
	
	out=(unsigned char *)out1;
	in=(unsigned char *)in1;
	
	while(length--)*out++ = *in++;
	
	
	return 0;
	
}
int uSetRect(uRect *r,int x,int y,int xsize,int ysize)
{
	if(!r)return 1;
	r->x=x;
	r->y=y;
	r->xsize=xsize;
	r->ysize=ysize;
	return 0;
}
int uPtInRect(uPoint p,uRect *r)
{
	if(!r)return 0;
	
	if((p.x < r->x) || (p.y < r->y))return 0;
	if((p.x > r->x+r->xsize) || (p.y > r->y+r->ysize))return 0;
	
	return 1;
}
int uInsetRect(uRect *r,int x,int y)
{
	if(!r)return 1;
	r->x += x;
	r->y += y;
	r->xsize -= x;
	r->ysize -= y;
	return 0;
}
int getDoubleArray8(double *d,long n,FILE8 *in)
{
	long k;
	
	if(!d || !in)return 1;
	
	if(in->convertRead){
		for(k=0;k<n;++k){
			if(getDouble8(&d[k],in))return 1;
		}
	}else{
        return getString8((unsigned char *)d,n*sizeof(double),in);	
	}
	return 0;
}

int getString8(unsigned char *s,long n,FILE8 *in)
{
	unsigned int crcval,c;
	
	if(!s || !in || (n < 0))return 1;
	
	if(fget8(in,(char *)s,(INT8_64)n))return 1;
	
	if(in->saveCRC){
		crcval=in->crcval;
		while(n-- > 0){
		    c=*s++;
		    
		    crcval = ((((crcval>>8)&0x00ff) ^ crctab[(crcval^(c))&0x00ff]) & 0xFFFF);
		}
		in->crcval=crcval;
	}
	
	return 0;
}
int putString8(unsigned char *s,long n,FILE8 *nout)
{
	unsigned short int crcval,c;
	unsigned char *ss;
	long nn;
	
	if(!s || !nout || (n < 0))return 1;
	
	if(nout->saveCRC){
		ss=s;
		nn=n;
		crcval=nout->crcval;
		while(nn-- > 0){
		    c=*ss++;
		   
		    crcval = ((((crcval>>8)&0x00ff) ^ crctab[(crcval^(c))&0x00ff]) & 0xFFFF);
		}
		nout->crcval=crcval;
	}
		
	return fput8(nout,(char *)s,(INT8_64)n);
}
int putLong8(long n,FILE8 *nout)
{
    unsigned char c[4];
    
	if(!nout)return 1;
	
    c[3] = (unsigned char)(n & 255);
    c[2] = (unsigned char)((n >> 8) & 255);
    c[1] = (unsigned char)((n >> 16) & 255);
    c[0] = (unsigned char)((n >> 24) & 255);
    
    return putString8((unsigned char *)c,4L,nout);	
}
int getLong8(long *n,FILE8 *in)
{
    unsigned char c[4];
    
	if(!n || !in)return 1;
	
	if(getString8(c,4L,in))return 1;

    *n = (long)((c[0] << 24) | (c[1] << 16) | (c[2] << 8) | c[3]);
	
	return 0;	
}
int putFloat8(float d,FILE8 *nout)
{
    
	if(!nout)return 1;
	
	if(nout->convertWrite){
   	 	unsigned char c,*cp;
   	 	int np;
   	 	
   	 	np=0;
		cp=(unsigned char *)&d;
	    c=cp[np];
	    cp[np]=cp[np+3];
	    cp[np+3]=c;
	    c=cp[np+1];
	    cp[np+1]=cp[np+2];
	    cp[np+2]=c;
	}	

    return putString8((unsigned char *)&d,4L,nout);	
}
int getFloat8(float *d,FILE8 *in)
{
    
	if(!d || !in)return 1;
	
	if(getString8((unsigned char *)d,4L,in))return 1;

	if(in->convertRead){
   	 	unsigned char c,*cp;
   	 	int np;
   	 	
   	 	np=0;
		cp=(unsigned char *)d;
	    c=cp[np];
	    cp[np]=cp[np+3];
	    cp[np+3]=c;
	    c=cp[np+1];
	    cp[np+1]=cp[np+2];
	    cp[np+2]=c;
	}	

	return 0;	
}
int putDouble8(double d,FILE8 *nout)
{
    
	if(!nout)return 1;
	

	if(nout->convertWrite){
   	 	unsigned char c,*cp;
   	 	int np;
   	 	
   	 	np=0;
		cp=(unsigned char *)&d;
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

    return putString8((unsigned char *)&d,8L,nout);	
}
int getDouble8(double *d,FILE8 *in)
{
    
	if(!d || !in)return 1;
	
	if(getString8((unsigned char *)d,8L,in))return 1;

	if(in->convertRead){
   	 	unsigned char c,*cp;
   	 	int np;
   	 	
   	 	np=0;
		cp=(unsigned char *)d;
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

	return 0;	
}
int startCRC8(FILE8 *in)
{
	if(!in)return 1;
	in->saveCRC = TRUE;
	in->crcval=0;
	return 0;
}
int stopCRC8(unsigned int *crcval,FILE8 *in)
{
	if(!in || !crcval)return 1;
	
	in->saveCRC = FALSE;
	*crcval=in->crcval;
	return 0;
}
INT8_64 FileLength8(FILE8 *in)
{
	INT8_64 length;

	if(!in)goto ErrorOut;

    if(fseek8(in,0L,2)){
        goto ErrorOut;
    }

    if((length=ftell8(in)) == -1L){
        goto ErrorOut;
    }
    
	if(fseek8(in,0L,0)){
	    goto ErrorOut;
	}
	return length;
ErrorOut:
	return (INT8_64)(-1);
}
int rewind8(FILE8 *in)
{

	if(!in)return 1;
	
	rewind(in->file);

	return 0;
}

