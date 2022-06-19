#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "SaveAllImages.h"
#include "Linedata.h"
#include "Message.h"
#include "Universal.h"
#include "uGraph.h"

#define USE_XY			0
#define USE_X			1
#define USE_Y 			2

#define X_PLANE	0
#define Y_PLANE	1
#define Z_PLANE	2
#define G_PLANE	3

#define max(x1,x2)    (((x1) > (x2)) ? (x1) : (x2))
#define min(x1,x2)    (((x1) < (x2)) ? (x1) : (x2))


int IsOption(void);

int FilesGetData(double **data,long *length,
                 long CurrentFrame,struct FileInfo2 *Files);

static int GetRange3(struct uDialogStruct *uList);

int doVectorSelection(struct FileInfo2 *Files);

struct DrawInfo3{
    struct FilePIOInfo *pio;
	struct areadata *ai;
    double rx,ry,rz,sx,sy,sz,tx,ty,tz;
	double p1x,p1y,p1z,v1x,v1y,v1z,v2x,v2y,v2z,d,od;
	double v3x,v3y,v3z;
	double l1,l2,l3;
	double drHalf,dsHalf;
	double dr,ds;
	long nx,ny;
	double *sout;
};

struct dRange{
      double xmin,xmax,ymin,ymax,dxlim,dylim;
};

static int doFindMaximum(struct FileInfo2 *Files);

int FloatToColor(double *d,struct paletteDraw *pd,unsigned char *bp);

int doStreamlineSelection(struct PlotStreamlineData *streamline);

static int doSage3DTracerPlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int pioDrawTracers(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int doSage3DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files);

static int doSage3DGetGeneratDataOffSet(struct areadata *ai,struct FileInfo2 *Files);

static int doRange4Action(struct uDialogStruct *uList);

static int pioDrawVoid(struct FileInfo2 *Files,unsigned char *sout);

int doVolumeSelection(struct FileInfo2 *Files);

static int doGeneralSelection(struct FileInfo2 *Files);

static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li);

int setFloat(double *d,long length);

static int doSage3DPlotLineData(struct FileInfo2 *Files,struct Message1000 *m);

static int doSage3DSavePlotLineData(struct FileInfo2 *Files,int *list,int nc);

static int pioDrawLasers(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int RangeCell(long n,struct FilePIOInfo *pio,double *dmin,double *dmax);
static int RangeCellVelocity(long n,struct FilePIOInfo *pio,double *vmax);

static int doSage3DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

	struct DrawCellStuff{
		struct screenData *ss;
		struct dRange *r;
	    double vmaxLen;
	    double vmax;
	    double turn;
	    double arrow;
	    double vmax2;
	    double dx;
	    double dy;
	};

static int DrawCellVelocity(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc);
static int DrawCellVelocity2(long n,struct FilePIOInfo *pio,struct DrawCellStuff *dc);

static int doSage3DDoDialog(struct FileInfo2 *Files,int *type);
static int getPlotImage(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame);

int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

static int pioDrawVectors(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int sageLocation(struct FileInfo2 *Files);

static int doAuto(struct FileInfo2 *Files);

static int GetPIOData(struct FileInfo2 *Files,long CurrentFrame);

static int pioSetLimitsSDS3D(struct FileInfo2 *Files,long nf);

static int doSage3DStart(struct FileInfo2 *Files);

static int pioGetValue(struct FilePIOInfo *pio,double x,double y,double z,double *v);

static double *buildSDS3D(struct FileInfo2 *Files);

static int doSage3D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

static int SageGetGradients(struct FileInfo2 *Files,int flag);

static int PlotPIOData(struct FileInfo2 *Files,long CurrentFrame,int flag);
static int pioSetLimits(struct FileInfo2 *Files,long nf);

static int doSage3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);

static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);
static int doSage3DGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int DrawCellLines(long n,struct FilePIOInfo *pio,struct screenData *ss,struct dRange *r);
static int pioDraw(struct FileInfo2 *Files,unsigned char *buff,long CurrentFrame,struct screenData *ss);
static int setPioScales(struct FilePIOInfo *pio);
static int doSage3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);
static int doSage3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);
static int doRange3Action(struct uDialogStruct *uList);

int doSageRewrite(BatchPtr Batch,char *name)
{
	struct FileInfo2 *Files;
	struct CommandInfo cp;
	char line[4096];
	char *fclass;
	int ret;
	
    if(!Batch || !name)return 1;
    Files=FileManagerGet(name);
    if(!Files)return 1;
    
    ret=1;
    
	zerol((char *)&cp,sizeof(struct CommandInfo));

    if(Files->fileClass != PIO3D_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"doSage3DBatch Found (%s) instead of PIO3D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSage3DBatch Can Only Process PIO3D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
	
    if(doSage3DStart(Files))goto ErrorOut;
	           
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSage3D(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}
static int doSage3D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
/*
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
*/
    struct FilePIOInfo *pio;
	char *command;
 	
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
	
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
		
	if(!mstrcmp("plane_type",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    if(!mstrcmp("x",command)){
	    }
	}else if(!mstrcmp("end",command)){
	    return 1;
	}
	
	return 0;
ErrorOut:
	return 1;
}
static int doSage3DStart(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	
	pio->range.ixmax=532;
	pio->range.iymax=432;
	
	pio->range.ixminData=20;
	pio->range.ixmaxData=522;
	pio->range.iyminData=20;
	pio->range.iymaxData=422;

	pio->range.AspectRatio=1.0;
	pio->range.MaintainAspectRatio=1;
	pio->range.receiveRange=1;
	
	
	pio->CurrentFrameGradients =  -1;
	
	pio->CurrentFrameVectors = -1;
	pio->vector.DrawVectors=0;
	pio->vector.arrowLength=0.1;
	pio->vector.arrowAngle=145.0;
	pio->vector.arrowHeads=1;
	pio->vector.vspace= -21;
	pio->vector.vmax=0;
	pio->vector.vfac=0.2;
	
	
	pio->pd.sType=0;
	pio->pd.sPmin=0;
	pio->pd.sPmax=1;	
	pio->pd.LabeledPalette=FALSE;	
	pio->pd.paletteFont=2;
	pio->pa.DrawZones=0;
	pio->pa.ZoneLevel=1;
	pio->pd.AreaFill=1;
	
	pio->pd.red=1.0;
	pio->pd.green=1.0;
	pio->pd.blue=1.0;
	

	pio->plane.Plane_type=Z_PLANE;
	pio->plane.Plane_xstep=0;
	pio->plane.Plane_xcount=50;
	pio->plane.Plane_ystep=0;
	pio->plane.Plane_ycount=50;
	pio->plane.Plane_zstep=0;
	pio->plane.Plane_zcount=50;
	pio->plane.Plane_gloc=0;
	pio->plane.Plane_gcount=50;
	
	pio->plane.Plane_xloc=pio->plane.Plane_xmin=Files->xminr;
	pio->plane.Plane_xmax=Files->xmaxr;
	pio->plane.Plane_yloc=pio->plane.Plane_ymin=Files->yminr;
	pio->plane.Plane_ymax=Files->ymaxr;
	pio->plane.Plane_zloc=pio->plane.Plane_zmin=Files->zminr;
	pio->plane.Plane_zmax=Files->zmaxr;
	
	pio->plane.Plane_gx1=Files->xminr;
	pio->plane.Plane_gy1=Files->yminr;
	pio->plane.Plane_gz1=Files->zminr;
	
	pio->plane.Plane_gx2=Files->xmaxr;
	pio->plane.Plane_gy2=Files->yminr;
	pio->plane.Plane_gz2=Files->zminr;
	
	pio->plane.Plane_gx3=Files->xminr;
	pio->plane.Plane_gy3=Files->ymaxr;
	pio->plane.Plane_gz3=Files->zminr;
	
	pio->plane.Plane_gmin=Files->zminr;
	pio->plane.Plane_gmax=Files->zmaxr;
	pio->plane.Plane_gloc=0;
	
	
    mstrncpy(pio->pioName,"density",255);
    pio->pioIndex=0;

    mstrncpy(pio->vector.vectorxName,"xdt",32);
    mstrncpy(pio->vector.vectoryName,"ydt",32);
    mstrncpy(pio->vector.vectorzName,"zdt",32);
		    	
	getPaletteByName("ps",pio->pd.palette);
		
	return 0;
}

