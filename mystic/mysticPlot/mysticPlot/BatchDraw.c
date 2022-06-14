#define EXTERN22 extern
#include "Xappl.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Message.h"
#include "uGraph.h"


#define AnyType			0
#define GridPlotType	1
#define LineDataType	2

#define DISPLAY_PIXELS   0
#define DISPLAY_INCHES   1


typedef struct rPointStruct3{
	double x,y,z;
} rPoint3;

typedef struct rRectStruct{
    double x,y;
    double xsize,ysize;
} rRect;

extern struct BatchDrawStruct *d;

struct DObject {
/* Start of Object variables */
    char name[64];
	rRect box;
	int type;
	long size;
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct BatchDrawStruct *d);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);
/* End of Object variables */
};

typedef struct DObject *DObjPtr;


struct CLineInformation{
/* Start of Object variables */
    char name[64];
	rRect box;
	int type;
	long size;
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct BatchDrawStruct *d);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);
/* End of Object variables */
    char fileName[64];
    char pioName[64];
    long pioIndex;
    rPoint3 finish;
    rPoint3 start;
    long count;
    double *xData;
    double *yData;
    long dCount;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

struct CGridPlotB{
/* Start of Object variables */
    char name[64];
	rRect box;
	int type;
	long size;
	void (*Kill)(struct DObject *);
	int (*Draw)(struct DObject *,struct BatchDrawStruct *d);
	int (*GetFrameCount)(struct DObject *,long *FrameCount);
	int (*SetFrame)(struct DObject *,long FrameCount);
/* End of Object variables */
    struct CLineInformation **Lines;
    long LineCount;
    long LineMax;
    
    double xMinimum;
    double yMinimum;
    double xMaximum;
    double yMaximum;
    
    double xMajorIn,xMajorOut;
    double yMajorIn,yMajorOut;
    double rMajorIn,rMajorOut;
    double tMajorIn,tMajorOut;
    double xMinorIn,xMinorOut;
    double yMinorIn,yMinorOut;
    double rMinorIn,rMinorOut;
    double tMinorIn,tMinorOut;
    int lAxis;			     /* Left Axis Flag		*/
    int bAxis;			     /* Bottom Axis Flag	*/
    int rAxis;			     /* Right Axis Flag		*/
    int tAxis;			     /* Top Axis Flag		*/
    int lAxisNumber;		     /* Left Axis Flag		*/
    int bAxisNumber;		     /* Bottom Axis Flag	*/
    int rAxisNumber;		     /* Right Axis Flag		*/
    int tAxisNumber;		     /* Top Axis Flag		*/
    int xGrid;			     /* X Axis Cross Hash	*/
    int yGrid;			     /* Y Axis Cross Hash	*/
    int xLogScale;		     /* X Axis Log Flag		*/
    int yLogScale;		     /* Y Axis Log Flag		*/
    int xAutoScale;		     /* X Axis AutoScale Flag	*/
    int yAutoScale;		     /* Y Axis AutoScale Flag	*/
    double xViewMin,xViewMax;        /* Window X Limits		*/
    double yViewMin,yViewMax;        /* Window Y Limits		*/
    double xViewMinLog,yViewMinLog;  /* Log10 of Limits		*/
    double xScale,yScale;            /* Window Scale Factors	*/
    double xScaleLog,yScaleLog;      /* Window Log of Scales	*/
    double xOff,yOff;                /* Window Off Sets		*/
    double xMajorStep,xMinorStep;    /* X Grid Step Sizes	*/
    double yMajorStep,yMinorStep;    /* Y Grid Step Sizes	*/
    double Small;
    
    
};


struct DOList2{
	DObjPtr *oOld;
	long oMaximun;
	DObjPtr *oCurrent;
	long oCount;
};

typedef struct DOList2 *DOListPtr;

typedef struct BatchDrawStruct{
    struct Color BackGround;
    char outputDirectory[1024];
    char outputName[256];
    uPattern *Patterns;
    struct screenData s;
    struct DOList2 DList;
	int displayType;
	unsigned char *data;
	long xsize;
	long ysize;
	long dpi;
	int raster;
	long length;
	long CurrentFrame;
	char palette[256*3];
	char palname[256];
	unsigned char red,green,blue;
	int nc;
} *BatchDrawPtr;

static int	rMoveTo(double x,double y,BatchDrawPtr d);
static int	rLineTo(double x,double y,BatchDrawPtr d);


int doubleNative(double *value,CommandPtr cp,BatchDrawPtr d);

static DObjPtr ObjectCreate(char *name,int type,rRect *box,long size);
static void ObjectKill(DObjPtr Object);
static int ObjectFramCount(struct DObject *Object,long *FrameCount);
static int ObjectSetFrame(struct DObject *Object,long FrameCount);
static int ObjectDraw(struct DObject *Object,struct BatchDrawStruct *d);


static DObjPtr GridPlotCreate(char *name,rRect *box);
static void GridPlotKill(DObjPtr Object);
static int GridPlotAdd2(DOListPtr l,char *name,char *name1);
static int GridPlotDraw(struct DObject *Object,struct BatchDrawStruct *d);
static int GridPlotRange(struct CGridPlotB *Plot);
static int GridPlotScale(struct CGridPlotB *Plot);
static double GridPlotpow10(int n);
static void GridPlotNeat(double *xmnc,double *xmxc,double *Large,double *Small);
static int GridPlotPlotGrid(struct CGridPlotB *b,BatchDrawPtr d);
void GridPlotxGridDraw(struct CGridPlotB *b,BatchDrawPtr d,double y,double xMajorIn,double xMajorOut,double xMinorIn,double xMinorOut,double mult,rRect *r);
void GridPlotyGridDraw(struct CGridPlotB *b,BatchDrawPtr d,double x,double yMajorIn,double yMajorOut,double yMinorIn,double yMinorOut,double mult,rRect *r);
static int GridPlotLineGrid(struct CGridPlotB *b,BatchDrawPtr d);
static int CPlotrPoint(struct CGridPlotB *Plot,double *x,double *y,double *ix,double *iy);


static int DOListADD(DOListPtr l,DObjPtr Object);
static DObjPtr DOListFindName(DOListPtr l,char *name,int type);
static void DOListDelete(DOListPtr l);


static DObjPtr LineDataCreate(char *name,char *file,char *pioName,long pioIndex,rPoint3 *start,rPoint3 *finish,long count);
static void LineDataKill(struct DObject *Object);


int ResizeArray(void *Array,long Length,long Count,long *Maximun,int tag);

int doDraw(BatchDrawPtr d,CommandPtr cp,BatchPtr Batch);

int DrawStart(BatchDrawPtr d);

int DrawStop(BatchDrawPtr d);

int doDrawAuto(BatchDrawPtr d);

int doDrawDraw(BatchDrawPtr d,long CurrentFrame,int flag);

long DrawFrameCount(BatchDrawPtr d);

static int SetDrawCanvas(BatchDrawPtr d);

int trueOut(char *filepath,char *name,long xsize,long ysize,unsigned char *image,long n,int flag);

int drawErase(BatchDrawPtr d);

int MakeNewDirectory(char *name);

int  doBatchDraw(BatchPtr Batch)
{
	struct BatchDrawStruct d;
	struct CommandInfo cp;
	char line[4096];
	int ret;
	
	if(!Batch)return 1;	
	
	ret=1;
	
	zerol((char *)&cp,sizeof(struct CommandInfo));
	
	zerol((char *)&d,sizeof(struct BatchDrawStruct));
		
	if(DrawStart(&d))goto ErrorOut;
	
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doDraw(&d,&cp,Batch))goto OK;
		}

	}
OK:	
	ret=0;
ErrorOut:
 	getCommand(NULL,&cp);
	return ret;
}
static int SetDrawCanvas(BatchDrawPtr d)
{
	long length;
	
	if(!d)return 1;
	
	if(d->raster == RASTER_8){
	    length=d->xsize*d->ysize;
	}else{
	    length=d->xsize*d->ysize*3;
	}
	
	if((length <= d->length) && d->data)return 0;
	
	d->length=0;
	
	if(d->data)cFree((char *)d->data);
	d->data=(unsigned char *)cMalloc(length,8264);
	
	if(!d->data)return 1;
	
	d->length=length;
	
	zerol((char *)d->data,d->length);


	d->s.ScreenBits=d->raster;
	d->s.viewRect.xsize=(int)d->xsize;
	d->s.viewRect.ysize=(int)d->ysize;
	d->s.buffer=d->data;
	d->s.palette=(unsigned char *)d->palette;
	
	return 0;
}


int doubleNative(double *value,CommandPtr cp,BatchDrawPtr d)
{
	if(!value || !cp || !d)return 1;
	
	if(doubleCommand(value,cp))return 1;
	
	if(d->displayType == DISPLAY_PIXELS){
		*value /= (double)d->dpi;
	}
	
	return 0;
}

int doDraw(BatchDrawPtr d,CommandPtr cp,BatchPtr Batch)
{
	struct DObject *Object;
	struct CommandInfo cpl;
	char *command,name[64],type[64];
	char name1[64],name2[64];
	rPoint3 start,finish;
	rRect box;
 	double value;
 	double radius;
 	long count;
 	int ret;
    char pioName[64];
    long pioIndex;
 	
	if(!d || !cp || !Batch)return 1;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));	
		
	ret = 1;
	
		
	command=stringCommand(cp);
	if(!command){
		sprintf(WarningBuff,"doDraw Error expecting command at word %ld\n",(long)cp->n);
		WarningBatch(WarningBuff);			
	    goto ErrorOut;
	}
	if(!mstrcmp("dpi",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    d->dpi=(int)value;
	    if(d->dpi < 1)d->dpi = 72;
		SetDrawCanvas(d);
	}else if(!mstrcmp("use",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		if(!mstrcmp("pixels",command)){
		    d->displayType=DISPLAY_PIXELS;
		}else if(!mstrcmp("inches",command)){
		    d->displayType=DISPLAY_INCHES;
		}else{
			sprintf(WarningBuff,"use command unknown type: '%s'\n",command);
			WarningBatch(WarningBuff);		
			goto ErrorOut;
		}
	}else if(!mstrcmp("canvas",command)){
	    ++(cp->n);
	    if(doubleNative(&value,cp,d))goto ErrorOut;
	    d->xsize=(long)(value*(double)d->dpi+0.5);
	    ++(cp->n);
	    if(doubleNative(&value,cp,d))goto ErrorOut;
	    d->ysize=(long)(value*(double)d->dpi+0.5);
		SetDrawCanvas(d);
	}else if(!mstrcmp("auto",command)){
	    doDrawAuto(d);
	}else if(!mstrcmp("raster",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    if((int)value == 8){
	    	d->raster=RASTER_8;
	    }else{
	    	d->raster=RASTER_24;
	    }
		SetDrawCanvas(d);
	}else if(!mstrcmp("save",command)){
	    Object=NULL;
	    
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
        if(goCD(command)){
            if(MakeNewDirectory(command)){
                  sprintf(WarningBuff,"Directory %s Could Not Be Found Or Created\n",command);
                  WarningBatch(WarningBuff);
                  goto ErrorOut;
            }
        }
		mstrncpy(d->outputDirectory,command,sizeof(d->outputDirectory));
		
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(d->outputName,command,sizeof(d->outputName));
	}else if(!mstrcmp("add",command)){
	    Object=NULL;
	    
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(name,command,64);
		
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(name1,command,64);
		
		
		if(GridPlotAdd2(&d->DList,name,name1))goto ErrorOut;
		
	}else if(!mstrcmp("lineplotset",command)){
	    Object=NULL;
	    
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(name,command,64);
		
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(name2,command,64);
		
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(pioName,command,64);

	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pioIndex=(long)value;
		
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    start.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    start.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    start.z=value;
	    
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    finish.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    finish.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    finish.z=value;
	    
	    
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    count=(long)value;
	    
        Object=LineDataCreate(name,name2,pioName,pioIndex,&start,&finish,count);
        if(!Object)goto ErrorOut;

	    if(DOListADD(&d->DList,Object))goto ErrorOut; 
	    
	    	    
	}else if(!mstrcmp("create",command)){
	    Object=NULL;
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(name,command,64);
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(type,command,64);
	    ++(cp->n);
	    if(doubleNative(&value,cp,d))goto ErrorOut;
	    box.x=value;
	    ++(cp->n);
	    if(doubleNative(&value,cp,d))goto ErrorOut;
	    box.y=value;
	    ++(cp->n);
	    if(doubleNative(&value,cp,d))goto ErrorOut;
	    box.xsize=value;
	    ++(cp->n);
	    if(doubleNative(&value,cp,d))goto ErrorOut;
	    box.ysize=value;
	    
	    if(!mstrcmp(type,"roundrect")){	    
		    ++(cp->n);
		    if(doubleNative(&value,cp,d))goto ErrorOut;
		    radius=value;	    	    
	    }else if(!mstrcmp(type,"colorbar")){
		    ++(cp->n);
			command=stringCommand(cp);
			if(!command)goto ErrorOut;
			if(!mstrcmp(command,"top")){
			}else if(!mstrcmp(command,"bottom")){
			}else if(!mstrcmp(command,"left")){
			}else if(!mstrcmp(command,"right")){
			}else{
				sprintf(WarningBuff,"doDraw colorbar direction '%s'\n",command);
				WarningBatch(WarningBuff);		
				goto ErrorOut;
			}
	    }else if(!mstrcmp(type,"text")){
		    ++(cp->n);
			command=stringCommand(cp);
			if(!command)goto ErrorOut;
	    }else if(!mstrcmp(type,"axis")){
	        Object=GridPlotCreate(name,&box);
	        if(!Object)goto ErrorOut;
	    }
	    
	    if(Object){
	       if(DOListADD(&d->DList,Object))goto ErrorOut; 
	    }
		
		//radius=radius;
		
	}else if(!mstrcmp("end",command)){
		ret = 2;
		goto ErrorOut;
	}else{
		sprintf(WarningBuff,"doDraw Bad command '%s'\n",command);
		WarningBatch(WarningBuff);		
		goto ErrorOut;
	}

	ret = 0;
ErrorOut:
 	getCommand(NULL,&cpl);
 	if(ret){
 	    DrawStop(d);
 	    if(ret != 2){
 			sprintf(WarningBuff,"doDraw Error On Following Line\n");
			WarningBatch(WarningBuff);		
			if(cp->line){
			   WarningBatch("'");
			   WarningBatch(cp->line);
			   WarningBatch("'\n");
			}
	    }
 	}
	return ret;
}
int drawErase(BatchDrawPtr d)
{
	long n;
	
	if(!d)return 1;
	if(!d->data)return 1;
	
	if(d->raster == RASTER_24){
	    for(n=0;n<d->xsize*d->ysize;++n){
	       d->data[3*n]=d->red;
	       d->data[3*n+1]=d->green;
	       d->data[3*n+2]=d->blue;
	    }
	}else{
		zerol((char *)d->data,d->length);
	}
	
	return 0;
}
int doDrawDraw(BatchDrawPtr d,long CurrentFrame,int flag)
{
	char filepath[2048];
	char name[256];
	DOListPtr l;
	int ret;
	long n;
	
	if(!d)return 1;
	if(!d->data)return 1;
	l=&d->DList;
	
	ret=1;
	
	drawErase(d);

	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->SetFrame){
            if((*Current->SetFrame)(Current,CurrentFrame))goto ErrorOut;
        }
	}
	
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->Draw){
            if((*Current->Draw)(Current,d))goto ErrorOut;
        }
	}
	
	sprintf(name,"image.%04ld%c",CurrentFrame,0);

	filepath[0]=0;
	mstrncpy(filepath,d->outputDirectory,2048);
	strncatToPath(filepath,d->outputName,2048);
	
	if(d->raster == RASTER_8){
	    ;
	}else if (d->raster == RASTER_24){
		trueOut(filepath,name,d->xsize,d->ysize,d->data,CurrentFrame,flag);
	}else{
	    goto ErrorOut;
	}	
	
	ret = 0;
	
ErrorOut:

	return ret;
}
long DrawFrameCount(BatchDrawPtr d)
{
	long frameCount;
	long n;
	DOListPtr l;
	
	if(!d)return -1;
	l=&d->DList;
	
	frameCount = -1;
	
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
        if(Current->GetFrameCount){
            if((*Current->GetFrameCount)(Current,&frameCount))goto ErrorOut;
        }
	}
ErrorOut:	
	return frameCount;
}
int doDrawAuto(BatchDrawPtr d)
{
	int flag;
	int ret;
	long frameCount;
	long n;
	
	if(!d)return 1;
	ret=1;
	
	frameCount=DrawFrameCount(d);	
	if(frameCount <= 0)goto ErrorOut;
	
	
	flag=0;
	for(n=0;n<frameCount;++n){	    
	    if(n >= (frameCount-1))flag=1;
	    if(doDrawDraw(d,n,flag))goto ErrorOut;
	}
	
	ret=0;
	
ErrorOut:

	return ret;
}
int DrawStart(BatchDrawPtr d)
{
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
	
	
	if(!d)return 1;
	
	zerol((char *)d,sizeof(*d));
	
	d->Patterns=Patterns;
	d->xsize=720;
	d->ysize=480;
	d->dpi=72;
	d->raster=RASTER_24;

	d->nc=255;
	
	mstrncpy(d->palname,"ps",3);
	
	getPaletteByName(d->palname,(unsigned char *)d->palette);
	
	d->red=255;
	d->green=255;
	d->blue=255;
	
	SetDrawCanvas(d);
	
	return 0;
}

static DObjPtr DOListFindName(DOListPtr l,char *name,int type)
{
	DObjPtr Found;
	long n;
    
	if(!l || !name)return NULL;
	
	Found=NULL;
		
	for(n=0;n<l->oCount;++n){
		DObjPtr Current;
	    if((Current = l->oCurrent[n]) == NULL)continue;
	    if(!strcmp(Current->name,name)){
	        Found=Current;
	        break;
	    }
	}
	
	if(type == AnyType)return Found;	
	
	if(Found && (Found->type == type)){
	    return Found;
	}
	
	return NULL;
}
int DrawStop(BatchDrawPtr d)
{
	if(!d)return 1;
	
 	DOListDelete(&d->DList);
 	
	if(d->data)cFree((char *)d->data);
	d->data=NULL;
	
	return 0;	
}

static void DOListDelete(DOListPtr l)
{
	DObjPtr Current;
	long n;
	
	if(!l)return;
	
	if(l->oCurrent){
	    for(n=0;n<l->oCount;++n){
	        if((Current = l->oCurrent[n]) == NULL)continue;
	        if(Current->Kill){
	            (*Current->Kill)(Current);
	        }
	    }
	    cFree((char *)l->oCurrent);
	    l->oCurrent=NULL;
	}
	if(l->oOld){
	    for(n=0;n<l->oCount;++n){
	        if((Current = l->oOld[n]) == NULL)continue;
	        if(Current->Kill){
	            (*Current->Kill)(Current);
	        }
	    }
	    cFree((char *)l->oOld);
	    l->oOld=NULL;
	}
	
	zerol((char *)l,sizeof(*l));
	
}

static int DOListADD(DOListPtr l,struct DObject *Object)
{
	long oMaximun;
	int ret;
	
	if(!l || !Object)return 1;
	
	ret = 1;
	if(DOListFindName(l,Object->name,AnyType)){
		sprintf(WarningBuff,"DOListADD create name  '%s' already used\n",Object->name);
		WarningBatch(WarningBuff);		
		goto ErrorOut;
	}

	oMaximun=l->oMaximun;
	if(ResizeArray(&l->oCurrent,sizeof(DObjPtr),l->oCount,&l->oMaximun,2002))goto ErrorOut;
	if(ResizeArray(&l->oOld,sizeof(DObjPtr),l->oCount,&oMaximun,2003))goto ErrorOut;
	l->oCurrent[l->oCount]=Object;
	l->oOld[l->oCount]=NULL;
	++l->oCount;
		
	ret = 0;
ErrorOut:
	return ret;
}


static struct DObject *ObjectCreate(char *name,int type,rRect *box,long size)
{
	struct DObject *Object;
	
	if(!name || !box )return (struct DObject *)NULL;
	
	Object=(struct DObject *)cMalloc(size,8213);
	if(!Object)return (struct DObject *)NULL;
	zerol((char *)Object,size);
	
	Object->box  = *box;
	Object->type = type;	
	Object->size = size;	

	Object->Kill=ObjectKill;

	Object->Draw=ObjectDraw;

	Object->GetFrameCount=ObjectFramCount;
	
	Object->SetFrame=ObjectSetFrame;
	
	mstrncpy(Object->name,name,64);
	
	return (struct DObject *)Object;
}

static void ObjectKill(struct DObject *Object)
{
	if(!Object)return;
	
	cFree((char *)Object);
}

static int ObjectDraw(struct DObject *Object,BatchDrawPtr d)
{
	if(!Object || !d)return 1;
	
	return 0;
	
}
static int ObjectSetFrame(struct DObject *Object,long FrameCount)
{
	if(!Object)return 1;
	
	//FrameCount=FrameCount;
	
	return 0;
	
}
static int ObjectFramCount(struct DObject *Object,long *FrameCount)
{
	if(!Object || !FrameCount)return 1;
	
	if(*FrameCount <= 0)*FrameCount = 1;
	
	return 0;
	
}
static int LineDataSetFrame(struct DObject *Object,long CurrentFrame)
{
	struct CLineInformation *Line;
	double xmin,xmax,ymin,ymax;
	struct linedata li;
	long n;
	int ret;
	
	if(!Object)return 1;
	Line=(struct CLineInformation *)Object;
	
	ret=1;
	
   if(Line->fileName[0]){
    	zerol((char *)&li,sizeof(li));
        mstrncpy(li.sourceName,Line->fileName,64);
        mstrncpy(li.pioName,Line->pioName,64);
        li.pioIndex=Line->pioIndex;
		li.x1=Line->start.x;
		li.y1=Line->start.y;
		li.z1=Line->start.z;
		li.x2=Line->finish.x;
		li.y2=Line->finish.y;
		li.z2=Line->finish.z;
		li.PointLineCount=(int)Line->count;
		li.type=LINEDATA_LINE_DATA;
		li.CurrentFrame=CurrentFrame;
		if(SendMessageByName(Line->fileName,MessageType_GET_LINE_DATA,&li)){
		    sprintf(WarningBuff,"source '%s' Name '%s' Index %ld Not Found x1 %g y1 %g z1 %g\n",
		           li.sourceName,li.pioName,li.pioIndex,li.x1,li.y1,li.z1);
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
		}
		
		
		if(Line->xData)cFree((char *)Line->xData);
		Line->xData=li.xData;
		
		if(Line->yData)cFree((char *)Line->yData);
		Line->yData=li.yData;
		
		Line->dCount=li.dCount;

		if(!li.xData || !Line->yData || (li.dCount <= 0))goto ErrorOut;
		xmin=li.xData[0];
		xmax=li.xData[0];
		
		ymin=li.yData[0];
		ymax=li.yData[0];
		
		for(n=1;n<li.dCount;++n){
		    if(li.xData[n] < xmin){
		        xmin=li.xData[n];
		    }else if(li.xData[n] > xmax){
		        xmax=li.xData[n];
		    }
		    
		    if(li.yData[n] < ymin){
		        ymin=li.yData[n];
		    }else if(li.yData[n] > ymax){
		        ymax=li.yData[n];
		    }
		}
		
		Line->xmin=xmin;
		Line->xmax=xmax;
		Line->ymin=ymin;
		Line->ymax=ymax;
		
	}
	ret=0;
ErrorOut:	
	return ret;
	
}
static int LineDataFrameCount(struct DObject *Object,long *FrameCount)
{
	struct CLineInformation *Line;
	struct Message1000 mo;
	
	if(!Object || !FrameCount)return 1;
	Line=(struct CLineInformation *)Object;
	
    if(Line->fileName[0]){
		mo.FrameCount=0;
		SendMessageByName(Line->fileName,MessageType_GET_FRAME_COUNT,&mo);
	    if(mo.FrameCount > *FrameCount)*FrameCount=mo.FrameCount;
    }
	
	return 0;
	
}
static DObjPtr LineDataCreate(char *name,char *file,char *pioName,long pioIndex,rPoint3 *start,rPoint3 *finish,long count)
{
	struct CLineInformation *Line;
	DObjPtr Object;
	rRect box;
	
	
	if(!name || !start || !finish || !file || !pioName)return NULL;
	
	Object=NULL;
	
	if(!FindWindowByName(file)){
	    if(!FileManagerGet(file)){
			sprintf(WarningBuff,"LineDataCreate data source '%s' not found\n",file);
			WarningBatch(WarningBuff);		
			goto ErrorOut;
	    }
	}

	
	zerol((char *)&box,sizeof(box));	
	
    Object=ObjectCreate(name,LineDataType,&box,sizeof(struct CLineInformation));
    if(!Object)return NULL;
    
    Line=(struct CLineInformation *)Object;
    Line->start=  *start;
    Line->finish= *finish;
    Line->count=  count;
    Line->Kill=LineDataKill;
    Line->GetFrameCount=LineDataFrameCount;
    Line->SetFrame=LineDataSetFrame;    
	mstrncpy(Line->fileName,file,64);	
	mstrncpy(Line->pioName,pioName,64);	
	Line->pioIndex=pioIndex;
        
ErrorOut:	
	return Object;
}
static void LineDataKill(struct DObject *Object)
{
	struct CLineInformation *Line;
	
	if(!Object)return;
	
    Line=(struct CLineInformation *)Object;
		
	if(Line->xData)cFree((char *)Line->xData);
	Line->xData=NULL;

	if(Line->yData)cFree((char *)Line->yData);
	Line->yData=NULL;

	Line->dCount=0;
		
	ObjectKill(Object);
	
}
static void GridPlotNeat(double *xmnc,double *xmxc,double *Large,double *Small)
{

        double xmn = *xmnc,xmx = *xmxc;
	    static double small2=1.e-6,dnum[4]={1.,2.,5.,10.};
        double delx,temp,t,fac,td,tmn,znd,zndt;
        int i,j,jj,itm;
    
	delx=xmx-xmn;
	temp=delx/6.;
	t=0.;
	if(temp != 0.)t=log10(fabs(temp)+small2*fabs(temp));
	i=(int)t;
	if(t < 0.)i=i-1;
	fac=GridPlotpow10(i);
	temp=temp/fac;
	for(j=0;j<4;++j){
	    jj=j;
	    if(dnum[j] >= temp) break;
    }
	td=dnum[jj]*fac;
	tmn=xmn/td;
	if(fabs(tmn) > 3e10){
blastOut:
	    *xmxc = *xmnc+1.0;
	    *xmnc = *xmnc-1.0;
	    temp=(*xmxc - *xmnc)/4.;
	    *Large = temp;
	    *Small = temp*.5;
	    return;
	}
	if(tmn < 0.)tmn=tmn-small2*tmn;
	itm=(int)tmn;
	if(tmn < 0.)itm=itm-1;
	tmn=itm;
	tmn=tmn*td;
	znd=2.;
l50:	znd=znd+1.;
	if(znd > 8.){
	    goto blastOut;
	}
	zndt=znd*td+tmn;
	if(zndt >= xmx) goto l60;
	goto l50;
l60:
	*xmnc=tmn;
	*xmxc=zndt;
	delx=xmx-xmn;
	*Large=td;
	if(jj == 1){
	    *Small = td/4;
	}else{
	    *Small = td/5;
	}
}
static double GridPlotpow10(int n)
{
	double a;
 
	a=1.;
 
	if(n < 0){
	   n = -n;
	   while(n--)a/=10.;
	} else {
	    while(n--)a*=10.;
	}
	return a;
}
static int GridPlotScale(struct CGridPlotB *b)
{
	double xmax,xmin,ymax,ymin;
	double xLarge,xSmall,yLarge,ySmall;
	rRect r;
	
	
	if(!b)return 1;
	if(b->xAutoScale || (b->xViewMin >= b->xViewMax)){
	    xmin=b->xMinimum;
	    xmax=b->xMaximum;
	    if(xmax <= xmin){
			xmin=xmax-1.0-0.001*fabs(xmax);
			xmax=xmax+1.0+0.001*fabs(xmax);
	    }
	    if(!b->xLogScale){
			GridPlotNeat(&xmin,&xmax,&xLarge,&xSmall);
			b->xMajorStep=xLarge;
			b->xMinorStep=xSmall;
	    }
	    b->xViewMin=xmin;
	    b->xViewMax=xmax;
	}

	if(b->yAutoScale || (b->yViewMin >= b->yViewMax)){
	    ymin=b->yMinimum;
	    ymax=b->yMaximum;
	    if(ymax <= ymin){
			ymin=ymax-1.0-0.001*fabs(ymax);
			ymax=ymax+1.0+0.001*fabs(ymax);
	    }
	    if(!b->yLogScale){
			GridPlotNeat(&ymin,&ymax,&yLarge,&ySmall);
			b->yMajorStep=yLarge;
			b->yMinorStep=ySmall;
	    }
	    b->yViewMin=ymin;
	    b->yViewMax=ymax;
	}
	r= b->box;
	
	b->xOff=r.x;
	b->yOff=r.y;

	b->xScale=(r.xsize)/(b->xViewMax-b->xViewMin);
	b->yScale=(r.ysize)/(b->yViewMax-b->yViewMin);
	if(b->xLogScale){
	    b->xViewMinLog=log10(b->xViewMin);
	    b->xScaleLog=(r.xsize)/(log10(b->xViewMax)-b->xViewMinLog);
	}
	if(b->yLogScale){
	    b->yViewMinLog=log10(b->yViewMin);
	    b->yScaleLog=(r.ysize)/(log10(b->yViewMax)-b->yViewMinLog);
	}
	
	
	return 0;
}
static int GridPlotRange(struct CGridPlotB *Plot)
{
	struct CLineInformation *Line;
	double xmin,xmax,ymin,ymax,amax;
	long n;
	
	if(!Plot)return 1;
	
	amax=1e60;
	xmin=amax;
	xmax=-amax;
	ymin=amax;
	ymax=-amax;
	if(Plot->Lines && (Plot->LineCount > 0)){	
		for(n=0;n<Plot->LineCount;++n){
		    Line=Plot->Lines[n];
		    if(Line->xmin < xmin)xmin=Line->xmin;
		    if(Line->ymin < ymin)ymin=Line->ymin;
		    if(Line->xmax > xmax)xmax=Line->xmax;
		    if(Line->ymax > ymax)ymax=Line->ymax;
		}
	}
	
	Plot->xMinimum=xmin;
	Plot->xMaximum=xmax;
	Plot->yMinimum=ymin;
	Plot->yMaximum=ymax;
	
	return 0;
}
static int GridPlotLineGrid(struct CGridPlotB *b,BatchDrawPtr d)
{
	struct CLineInformation *Line;
	int itout;
	double ix,iy,ixl,iyl;
	long n,k;
	
	if(!b || !d)return 1;
	
	
	if(b->Lines && (b->LineCount > 0)){	
		for(n=0;n<b->LineCount;++n){
		    Line=b->Lines[n];
		    
		    if(!Line->xData || !Line->yData )continue;
		    
		    itout=TRUE;
		    ixl=0;
		    iyl=0;
		    for(k=0;k<Line->dCount;++k){
				if(CPlotrPoint(b,Line->xData+k,Line->yData+k,&ix,&iy)){
				    if(!itout)rLineTo(ix,iy,d);
		    	    itout=TRUE;
					ixl=ix;
					iyl=iy;
			    	continue;
				}
				if(itout){
		    	    itout=FALSE;
		    	    if(k > 0){
			    	    rMoveTo(ixl,iyl,d);
		    	    }else{
			    	    rMoveTo(ix,iy,d);
			    	}
				}
				rLineTo(ix,iy,d);
		    }
		    
		    
		    
		}
	}
	
	return 0;
	
}
static int GridPlotDraw(struct DObject *Object,BatchDrawPtr d)
{
	struct CGridPlotB *Plot;
	
	if(!Object || !d)return 1;

	Plot=(struct CGridPlotB *)Object;
	
	if(GridPlotRange(Plot))return 1;
	
	if(GridPlotScale(Plot))return 1;
	
	if(GridPlotPlotGrid(Plot,d))return 1;

	if(GridPlotLineGrid(Plot,d))return 1;
	
	return 0;
	
}
static int CPlotrPoint(struct CGridPlotB *b,double *x,double *y,double *ix,double *iy)
{

	
	if(!b)return 0;
	if(!ix || !iy)return 0;
	if( !x ||  !y)return 0;
	
	if(b->xLogScale){
	    if(*x < b->xViewMin*(1-b->Small)){
	        *ix = (log10(b->xViewMin)-b->xViewMinLog)*b->xScaleLog+b->xOff;
	        return 1;
	    }else{
	        *ix = (log10(*x)-b->xViewMinLog)*b->xScaleLog+b->xOff;
	    }
	}else{
	    *ix = (*x-b->xViewMin)*b->xScale+b->xOff;
	}
	if(b->yLogScale){
	    if(*y < b->yViewMin*(1-b->Small)){
	        *iy = (log10(b->yViewMin)-b->yViewMinLog)*b->yScaleLog+b->yOff;
	        return 1;
	    }else{
	        *iy = (log10(*y)-b->yViewMinLog)*b->yScaleLog+b->yOff;
	    }
	}else{
	    *iy = (*y-b->yViewMin)*b->yScale+b->yOff;
	}
	return 0;
}
static int	rMoveTo(double x,double y,BatchDrawPtr d)
{
	int ix,iy;
	
	if(!d)return 1;
	
	ix=(int)(x*(double)d->dpi + 0.5);
	iy=(int)(y*(double)d->dpi + 0.5);
	
	uMoveU(ix,iy,&d->s);
			
	return 0;
}
static int	rLineTo(double x,double y,BatchDrawPtr d)
{
	int ix,iy;
	
	if(!d)return 1;
	
	ix=(int)(x*(double)d->dpi + 0.5);
	iy=(int)(y*(double)d->dpi + 0.5);
	
	uLineU(ix,iy,d->nc,&d->s);
	
	return 0;
}
static int GridPlotPlotGrid(struct CGridPlotB *b,BatchDrawPtr d)
{
	double xsize2,ysize2;
	double right,bottom;
	double mult;
/*	IconPtr myIcon; */
	rRect r;
	
	if(!b || !d)return 1;
	
	r=b->box;
	
/*
 	xsize2=b->vAttributes.xLineWidth/2;
 	ysize2=b->vAttributes.yLineWidth/2;
*/
	xsize2=0;
	ysize2=0;
	
	mult=1;
	
	right=r.x+r.xsize;
	bottom=r.y+r.ysize;
 	 	
/* 	
	SetAttributes(myIcon,&b->vAttributes,lPattern | lForeColor | lBackColor | lWidth);
*/
	if(b->lAxis){
	    GridPlotxGridDraw(b,d,bottom,b->xMajorIn,b->xMajorOut,b->xMinorIn,b->xMinorOut,mult,&r);
	    rMoveTo(r.x-xsize2,  bottom-ysize2,d);
	    rLineTo(right-xsize2,bottom-ysize2,d);
	}
	if(b->rAxis){
	    GridPlotxGridDraw(b,d,r.y,b->tMajorIn,b->tMajorOut,b->tMinorIn,b->tMinorOut,-mult,&r);
	    rMoveTo(r.x-xsize2,  r.y-ysize2,d);
	    rLineTo(right-xsize2,r.y-ysize2,d);
	}
	if(b->bAxis){
	    GridPlotyGridDraw(b,d,r.x,b->yMajorIn,b->yMajorOut,b->yMinorIn,b->yMinorOut,mult,&r);
	    rMoveTo(r.x-xsize2,r.y-ysize2,d);
	    rLineTo(r.x-xsize2,bottom-ysize2,d);
	}
	if(b->tAxis){
	    GridPlotyGridDraw(b,d,right,b->rMajorIn,b->rMajorOut,b->rMinorIn,b->rMinorOut,-mult,&r);
	    rMoveTo(right-xsize2,r.y-ysize2,d);
	    rLineTo(right-xsize2,bottom-ysize2,d);
	}
	/* SetPenNormal(myIcon); */
	return  0;
}
void GridPlotyGridDraw(struct CGridPlotB *b,BatchDrawPtr d,double x,double yMajorIn,double yMajorOut,double yMinorIn,double yMinorOut,double mult,rRect *r)
{
	double MajorIn,MajorOut,MinorIn,MinorOut;
	double xx,y,yy,End,mEnd,Start,Exp,Step,Expm,ySmall;
	int iStart,iEnd,n;
	double xsize2,ysize2;
	double ix,iy;
	
	if(!b || !r)return;
	
	/*
 	xsize2=b->vAttributes.xLineWidth/2;
 	ysize2=b->vAttributes.yLineWidth/2;
	*/

 	xsize2=0;
 	ysize2=0;
	
	MajorIn=x+yMajorIn*mult;
	MajorOut=x-yMajorOut*mult;
	MinorIn=x+yMinorIn*mult;
	MinorOut=x-yMinorOut*mult;
	End=b->yViewMax*(1+b->Small);
	mEnd=b->yMajorStep*(1-b->Small);
	if(b->yLogScale){
	    End=log10(b->yViewMax)*(1+b->Small);
	    iEnd=(int)End;
	    iStart=(int)(b->yViewMinLog-.9999);
	    Start=iStart;
	    xx=(b->xViewMax+b->xViewMin)/2.;
	    if((iEnd-iStart) <= 9){
	        Step=1;
	    }else{
	        Step=4;
	    }
	    for(y=Start;y <= End;y += Step){
	        Exp=pow(10.,y);
	        if(!CPlotrPoint(b,&xx,&Exp,&ix,&iy)){
	        /*
	            if(b->gridHash && (y > Start) && (y < End)){
	            	PatTo(r->left,iy-ysize2,r->right,iy-ysize2,2);
	            }
	        */
	            rMoveTo(MajorIn-xsize2,iy-ysize2,d);
	            rLineTo(MajorOut-xsize2,iy-ysize2,d);
	        }
	        if(Step == 1){
	            Expm=Exp+Exp;
	            for(n=0;(n<8) && (log10(Expm) < End);++n,Expm += Exp){
					if(!CPlotrPoint(b,&xx,&Expm,&ix,&iy)){
					    rMoveTo(MinorIn-xsize2,iy-ysize2,d);
					    rLineTo(MinorOut-xsize2,iy-ysize2,d);
					}
		        }
	        }
	    }
	}else{
	    ySmall=1.0e-5*(b->yViewMax-b->yViewMin);
	    End=b->yViewMax+ySmall;
	    mEnd=b->yMajorStep-ySmall;
	   	Start=b->yViewMin;
	    for(y=Start;y <= End;y += b->yMajorStep){
	            iy=(y-b->yViewMin)*b->yScale+b->yOff;
	            /*
	            if(b->gridHash && (y > Start) && (y < End)){
	            	PatTo(r->left,iy-ysize2,r->right,iy-ysize2,2);
	            }
	            */
	            rMoveTo(MajorIn-xsize2,iy-ysize2,d);
	            rLineTo(MajorOut-xsize2,iy-ysize2,d);
	        for(yy=y+b->yMinorStep;
	            yy < y+mEnd && yy < End;yy += b->yMinorStep){
	                iy=(yy-b->yViewMin)*b->yScale+b->yOff;
	                rMoveTo(MinorIn-xsize2,iy-ysize2,d);
	                rLineTo(MinorOut-xsize2,iy-ysize2,d);
	        }
	    }
	}
}
void GridPlotxGridDraw(struct CGridPlotB *b,BatchDrawPtr d,double y,double xMajorIn,double xMajorOut,double xMinorIn,double xMinorOut,double mult,rRect *r)
{
	double MajorIn,MajorOut,MinorIn,MinorOut;
	double x,xx,End,mEnd,Start,Exp,yy,Step,Expm,xSmall;
	int iStart,iEnd,n;
	double xsize2,ysize2;
	double ix,iy;
	
	if(!b || !r)return;
/*
 	xsize2=b->vAttributes.xLineWidth/2;
 	ysize2=b->vAttributes.yLineWidth/2;
*/

 	xsize2=0;
 	ysize2=0;
	
	MajorIn=y-xMajorIn*mult;
	MajorOut=y+xMajorOut*mult;
	MinorIn=y-xMinorIn*mult;
	MinorOut=y+xMinorOut*mult;
	End=b->xViewMax*(1+b->Small);
	mEnd=b->xMajorStep*(1-b->Small);
	if(b->xLogScale){
	    End=log10(b->xViewMax)*(1+b->Small);
	    iEnd=(int)End;
	    iStart=(int)(b->xViewMinLog-.9999);
	    Start=iStart;
	    yy=(b->yViewMax+b->yViewMin)/2.;
	    if((iEnd-iStart) <= 9){
	        Step=1;
	    }else{
	        Step=4;
	    }
	    for(x=Start;x <= End;x += Step){
	        Exp=pow(10.,x);
	        if(!CPlotrPoint(b,&Exp,&yy,&ix,&iy)){
	        /*
	            if(b->gridHash && (x > Start) && (x < End)){
	            	PatTo(ix-xsize2,r->top,ix-xsize2,r->bottom,2);
	            }
	        */
	            rMoveTo(ix-xsize2,MajorIn-ysize2,d);
	            rLineTo(ix-xsize2,MajorOut-ysize2,d);
	        }
	        if(Step == 1){
	            Expm=Exp+Exp;
	            for(n=0;(n<8) && (log10(Expm) < End);++n,Expm += Exp){
					if(!CPlotrPoint(b,&Expm,&yy,&ix,&iy)){
					    rMoveTo(ix-xsize2,MinorIn-ysize2,d);
					    rLineTo(ix-xsize2,MinorOut-ysize2,d);
					}
		    	}
	        }
	    }
	}else{
	    xSmall=1.0e-5*(b->xViewMax-b->xViewMin);
	    End=b->xViewMax+xSmall;
	    mEnd=b->xMajorStep-xSmall;
	    Start=b->xViewMin;
	    for(x=Start;x <= End;x += b->xMajorStep){
	            ix=(x-b->xViewMin)*b->xScale+b->xOff;
	            /*
	            if(b->gridHash && (x > Start) && (x < End)){
	            	PatTo(ix-xsize2,r->top,ix-xsize2,r->bottom,2);
	            }
	            */
	            rMoveTo(ix-xsize2,MajorIn-ysize2,d);
	            rLineTo(ix-xsize2,MajorOut-ysize2,d);
	        for(xx=x+b->xMinorStep;
	            xx < x+mEnd && xx < End;xx += b->xMinorStep){
	                ix=(xx-b->xViewMin)*b->xScale+b->xOff;
	                rMoveTo(ix-xsize2,MinorIn-ysize2,d);
	                rLineTo(ix-xsize2,MinorOut-ysize2,d);
	        }
  	    }
	}
}
static struct DObject *GridPlotCreate(char *name,rRect *box)
{
	struct CGridPlotB *b;
	struct DObject *Object;
	
    Object=ObjectCreate(name,GridPlotType,box,sizeof(struct CGridPlotB));
    if(!Object)return (struct DObject *)NULL;
    
    Object->Kill=GridPlotKill;
    
    Object->Draw=GridPlotDraw;
    
    b=(struct CGridPlotB *)Object;
	        
	b->xMinimum=0;
	b->yMinimum=0;
	b->xMaximum=1;
	b->yMaximum=1;

	b->rMajorOut=b->tMajorOut=b->yMajorOut=b->xMajorOut=.1;
	b->rMinorOut=b->tMinorOut=b->yMinorOut=b->xMinorOut=.05;
	b->yAutoScale=b->xAutoScale=TRUE;
	b->rAxis=b->tAxis=TRUE;
	b->lAxis=b->bAxis=TRUE;
	b->rAxisNumber=b->tAxisNumber=TRUE;
	b->lAxisNumber=b->bAxisNumber=TRUE;
	b->xLogScale=b->yLogScale=FALSE;		     
	b->xMajorIn=b->yMajorIn=b->rMajorIn=b->tMajorIn=0;
	b->xMinorIn=b->yMinorIn=b->rMinorIn=b->tMinorIn=0;
	
	return (struct DObject *)Object;
}
static int GridPlotAdd2(DOListPtr l,char *plot,char *line)
{
	struct CLineInformation *Line;
	struct DObject *Object;
	struct CGridPlotB *Plot;
	int ret;
	
	if(!l || !plot || !line)return 1;
	
	ret=1;
	
	Object=DOListFindName(l,plot,GridPlotType);
	if(!Object){
		sprintf(WarningBuff,"GridPlotAdd2 plot '%s' not found\n",plot);
		WarningBatch(WarningBuff);		
		goto ErrorOut;
	}
	Plot=(struct CGridPlotB *)Object;
	
	Object=DOListFindName(l,line,LineDataType);
	if(!Object){
		sprintf(WarningBuff,"GridPlotAdd2 line infomation '%s' not found\n",plot);
		WarningBatch(WarningBuff);		
		goto ErrorOut;
	}
	Line=(struct CLineInformation *)Object;
	
	
	
	if(ResizeArray(&Plot->Lines,sizeof(struct CLineInformation *),Plot->LineCount,&Plot->LineMax,2004))goto ErrorOut;
	Plot->Lines[Plot->LineCount++]=Line;
	
	ret=0;
ErrorOut:	
	return ret;
}
static void GridPlotKill(struct DObject *Object)
{
	struct CGridPlotB *Plot;
	
	if(!Object)return;
	
	Plot=(struct CGridPlotB *)Object;
	if(Plot->Lines)cFree((char *)Plot->Lines);
	Plot->Lines=NULL;
	
	zerol((char *)Object,sizeof(struct CGridPlotB));
	
	ObjectKill(Object);
	
	
}
