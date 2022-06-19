#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Linedata.h"
#include "uDialog.h"
#include "SaveAllImages.h"
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

struct DrawInfo{
    struct FilePIOInfo *pio;
	struct areadata *ai;
    double rx,ry,rz,sx,sy,sz,tx,ty,tz;
	double p1x,p1y,p1z,v1x,v1y,v1z,v2x,v2y,v2z,d,od;
	double v3x,v3y,v3z;
	double l1,l2;
	double drHalf,dsHalf;
	double dr,ds;
	long nx,ny;
	double *sout;
};

struct dRange{
      double xmin,xmax,ymin,ymax,dxlim,dylim;
};

static int SliceFloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int FrameDataToRaster(struct SetFrameData  *sd,unsigned char *out);

static int doSDS3DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int doSDS3DGetDataSlice(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSDS3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int setFloat(double *d,long length);

int IsOption(void);

int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n);
int sds2DOut(char *path,char *name,long xsize,long ysize,double *data,long n,int flag,struct LIMITS *limits);
int sds3DOut(char *path,char *name,long xsize,long ysize,long zsize,double *data,long n,int flag,struct LIMITS *limits);
int hdfOut(char *filepath,char *name,char *palette,long xsize,long ysize,char *data,long n,int flag,struct LIMITS *limits);

int  isOut(double *x1,double *y1,double *x2,double *y2,struct dRange *r);

static int doSDS3D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch);

int doSDS3DStart(struct FileInfo2 *Files);

static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag);

static int pioGetValue(struct FileInfo2 *Files,double x,double y,double z,double *v);

static int GetSDS3DData(struct FileInfo2 *Files,long CurrentFrame);

static int doAuto(struct FileInfo2 *Files);

static int PlotSDS3DData(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int DrawCellColor(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r);

static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy);
static int IntLoc(struct FilePIOInfo *pio,double *x,double *y,int ix,int iy);
static int ConOne(double *x,double *y,double *p,struct FilePIOInfo *pio,struct screenData *ss);
static int sds3dSetLimits(struct FileInfo2 *Files,long nf);

static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame);
static int setSds3dScales(struct FilePIOInfo *pio);

static int sds3dGradients(struct FileInfo2 *Files,long CurrentFrame,int flag);

static int sds3dDraw(struct FileInfo2 *Files,struct screenData *ss);

static int sds3dSetLimitsSDS3D(struct FileInfo2 *Files,long nf);

static double *buildSDS3D(struct FileInfo2 *Files);

static int doSDS3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

static int doSDS3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai);
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li);
static int doSDS3DGetLineData(struct FileInfo2 *Files,struct linedata *li);

static int doSDS3DDoDialog(struct FileInfo2 *Files,int *type);

static int doSDS3DPutData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

int doVolumeSelection(struct FileInfo2 *Files);

int doSDS3DBatch(BatchPtr Batch,char *name)
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

    if(Files->fileClass != SDS3D_FILES){
        fclass=FilesClassName(Files);
        if(fclass){
            sprintf(WarningBuff,"doSDS3DBatch Found (%s) instead of SDS3D_FILES\n",fclass);
        }else{
            sprintf(WarningBuff,"doSDS3DBatch Can Only Process SDS3D_FILES\n");
        }
        WarningBatch(WarningBuff);
        goto ErrorOut;
    }
	
    if(doSDS3DStart(Files))goto ErrorOut;
	           
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
		for(cp.n=0;cp.n<cp.nword;++cp.n){
	       if(doSDS3D(Files,&cp,Batch))goto OK;
		}

	}
OK:	
    ret=0;
	
 ErrorOut:
 	getCommand(NULL,&cp);
    return ret;
}
static int doSDS3D(struct FileInfo2 *Files,CommandPtr cp,BatchPtr Batch)
{
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
    struct FilePIOInfo *pio;
    double xmin,xmax,ymin,ymax;
    long CurrentFrame;
	char *command;
 	double value;
 	int rangeNew;
    int flag;
 	
	if(!cp || !Batch || !Files)return 1;
	pio = &Files->pioData;
	
	
	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	rangeNew = FALSE;
		
	xmin=pio->range.xminData;
	ymin=pio->range.yminData;
	xmax=pio->range.xmaxData;
	ymax=pio->range.ymaxData;
	
	if(!mstrcmp("plane_type",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    if(!mstrcmp("x",command)){
			pio->plane.Plane_type=X_PLANE;
			SetRange(Files,pio->plane.Plane_ymin,pio->plane.Plane_ymax,pio->plane.Plane_zmin,pio->plane.Plane_zmax,USE_XY);
	    }else if(!mstrcmp("y",command)){
			pio->plane.Plane_type=Y_PLANE;
			SetRange(Files,pio->plane.Plane_xmin,pio->plane.Plane_xmax,pio->plane.Plane_zmin,pio->plane.Plane_zmax,USE_XY);
	    }else{
			pio->plane.Plane_type=Z_PLANE;
			SetRange(Files,pio->plane.Plane_xmin,pio->plane.Plane_xmax,pio->plane.Plane_ymin,pio->plane.Plane_ymax,USE_XY);
	    }
	}else if(!mstrcmp("plane_xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xmin=value;
	}else if(!mstrcmp("plane_ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_ymin=value;
	}else if(!mstrcmp("plane_zmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zmin=value;
	}else if(!mstrcmp("plane_xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xmax=value;
	}else if(!mstrcmp("plane_ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_ymax=value;
	}else if(!mstrcmp("plane_zmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zmax=value;
	}else if(!mstrcmp("plane_xcount",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xcount=(long)value;
	}else if(!mstrcmp("plane_ycount",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_ycount=(long)value;
	}else if(!mstrcmp("plane_zcount",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zcount=(long)value;
	}else if(!mstrcmp("plane_xloc",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_xloc=value;
	}else if(!mstrcmp("plane_yloc",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_yloc=value;
	}else if(!mstrcmp("plane_zloc",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->plane.Plane_zloc=value;
	}else if(!mstrcmp("sagedata",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(pio->pioName,command,255);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pioIndex=(int)value;
	}else if(!mstrcmp("xsize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixmax=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ysize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iymax=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("palette",command)){	
		 ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		getPaletteByName(command,pio->pd.palette);
	}else if(!mstrcmp("ixmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixminData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ixmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ixmaxData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("iymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iyminData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("iymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.iymaxData=(long)value;
		rangeNew = TRUE;
	}else if(!mstrcmp("xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.xminData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.xmaxData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.yminData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.ymaxData=value;
		rangeNew = TRUE;
	}else if(!mstrcmp("drawzones",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pa.DrawZones=(int)value;
	}else if(!mstrcmp("areafill",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.AreaFill=(int)value;
	}else if(!mstrcmp("MaintainAspectRatio",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->range.MaintainAspectRatio=(int)value;
	}else if(!mstrcmp("useGradients",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pa.flagGradients=(int)value;	    
	}else if(!mstrcmp("pos",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    CurrentFrame=(long)value;
	    if(GetSDS3DData(Files,CurrentFrame))goto ErrorOut;
	}else if(!mstrcmp("datatype",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(pio->pioName,command,255);
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pioIndex=(int)value;
	    pio->pd.AreaFill=1;
	}else if(!mstrcmp("filelist",command)){
	    if(FileManagerBatch(Batch,cp))goto ErrorOut;	
	}else if(!mstrcmp("datascale",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.sType=(int)value;
	    if(pio->pd.sType < 0 || pio->pd.sType > 2)pio->pd.sType=0;
	}else if(!mstrcmp("dmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.sPmin=value;
	}else if(!mstrcmp("dmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    pio->pd.sPmax=value;
	}else if(!mstrcmp("auto",command)){
	    if(doAuto(Files))goto ErrorOut;
	}else if(!mstrcmp("end",command)){
	    return 1;
	}
	
	if(rangeNew){
		flag=0;
		if(xmin != pio->range.xminData)flag  = 1;
		if(xmax != pio->range.xmaxData)flag += 2;
		if(ymin != pio->range.yminData)flag += 4;
		if(ymax != pio->range.ymaxData)flag += 8;		
		SetRange(Files,pio->range.xminData,pio->range.xmaxData,pio->range.yminData,pio->range.ymaxData,flagout[flag]);
	}
	
	return 0;
ErrorOut:
	return 1;
}
int doSDS3DStart(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;
    
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
	
	
	pio->CurrentFrameGradients = -1;
	
	
	pio->pd.sType=0;
	pio->pd.sPmin=0;
	pio->pd.sPmax=1;	
	pio->pd.LabeledPalette=FALSE;	
	pio->pd.paletteFont=2;
	pio->pa.DrawZones=0;
	pio->pd.AreaFill=1;
	

	pio->pd.UsePlotScales=1;
	pio->pd.UsePlotTime=1;
	pio->pd.UsePlotRange=1;
	
	pio->plane.Plane_type=Z_PLANE;
	pio->plane.Plane_xstep=0;
	pio->plane.Plane_xcount=50;
	pio->plane.Plane_ystep=0;
	pio->plane.Plane_ycount=50;
	pio->plane.Plane_zstep=0;
	pio->plane.Plane_zcount=50;
	pio->plane.Plane_gloc=0;
	pio->plane.Plane_gcount=50;
	
	
	Files->pGetData2=doSDS3DGetData;
	
	Files->pGetData3=doSDS3DGetDataSlice;
	
	Files->pGetAreaData=doSDS3DGetAreaData;

	Files->pGetLineData=doSDS3DGetLineData;
	
	Files->pDoDialog=doSDS3DDoDialog;
	
	Files->pInformation=doSDS3DInformation;
	
	Files->pPutData2=doSDS3DPutData;

	Files->pGetCellData=doSDS3DGetCellData;
	
	limits=NULL;
	FileGetLimits(&limits,0L,Files);
	if(limits && limits->iGot){
		Files->xminr=pio->plane.Plane_xloc=pio->plane.Plane_xmin=limits->xmin;
		Files->xmaxr=pio->plane.Plane_xmax=limits->xmax;
		Files->yminr=pio->plane.Plane_yloc=pio->plane.Plane_ymin=limits->ymin;
		Files->ymaxr=pio->plane.Plane_ymax=limits->ymax;
		Files->zminr=pio->plane.Plane_zloc=pio->plane.Plane_zmin=limits->zmin;
		Files->zmaxr=pio->plane.Plane_zmax=limits->zmax;
		pio->plane.Plane_gloc=pio->plane.Plane_gmin=limits->xmin;
		pio->plane.Plane_gmax=limits->xmax;
	}else{	
		Files->xminr=pio->plane.Plane_xloc=pio->plane.Plane_xmin=0;
		Files->xmaxr=pio->plane.Plane_xmax=(double)Files->xmax;
		Files->yminr=pio->plane.Plane_yloc=pio->plane.Plane_ymin=0;
		Files->ymaxr=pio->plane.Plane_ymax=(double)Files->ymax;
		Files->zminr=pio->plane.Plane_zloc=pio->plane.Plane_zmin=0;
		Files->zmaxr=pio->plane.Plane_zmax=(double)Files->zmax;
		pio->plane.Plane_gloc=pio->plane.Plane_gmin=(double)Files->xmin;
		pio->plane.Plane_gmax=(double)Files->xmax;
	}

		    
	SetRange(Files,pio->plane.Plane_xmin,pio->plane.Plane_xmax,pio->plane.Plane_ymin,pio->plane.Plane_ymax,USE_XY);
	
	getPaletteByName("ps",pio->pd.palette);
		
	return 0;
}
static int doSDS3DPutData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	int ret;

	if(!Files || !sd)return 1;
	pio=&Files->pioData;
	
	ret=1;

	if(sd->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"doSDS3DPutData - Only Puts FRAME_DATA_FLOAT");
	    Warning(WarningBuff);
	    goto ErrorOut;
	}

	ret=FileAppendSDS3D(NULL,(int)sd->xsize,(int)sd->ysize,(int)sd->zsize,(double *)sd->data,CurrentFrame,&Files);
	if(ret)goto ErrorOut;
	pio->GetDataFlag = 1;
	ret=0;
ErrorOut:
	return ret;
}
static int doSDS3DInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	
	if(!Files || !sd)return 1;
	
	pio=&Files->pioData;
	
	sd->rangeData=pio->rangeData;
	sd->range=pio->range;
	sd->pd=pio->pd;
	sd->pa=pio->pa;
	sd->plane=pio->plane;
	sd->vector=pio->vector;

	if(pio->pioName[0]){
	    mstrncpy(sd->pioName,pio->pioName,255);
	    sd->pioIndex=(int)pio->pioIndex;
	}else{
	   sd->pioName[0]=0;
	   pio->pioIndex=0;
	}

	
	return 0;
}
static int doPointGetListData(struct FileInfo2 *Files,struct linedata *li)
{
    struct FilePIOInfo *pio;
    double value;
	long n;

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
		
	if(li->type != LINEDATA_LIST_DATA)return 1;
	
	if(!li->x || !li->y || !li->z || !li->v || (li->PointLineCount < 1))return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	pio->pioIndex=(int)li->pioIndex;

	if(GetSDS3DData(Files,li->CurrentFrame))goto OutOfHere;

    setSds3dScales(pio);
	
	for(n=0;n<li->PointLineCount;++n){
	    		    
	    if(!pioGetValue(Files,li->x[n],li->y[n],li->z[n],&value)){
	        li->v[n]=0;
	    }else{
	        li->v[n]=value;
	    }
	}

	
	return 0;
OutOfHere:

	return 1;
}
static int doIntegrateLine(struct FileInfo2 *Files,struct linedata *li)
{
	struct FilePIOInfo *pio;
	struct SetFrameData  *ss;
	struct PIO *sage;
	double *xD,*yD,s,s1,s2;
	double x1,y1,z1,x2,y2,z2,x,y,z,xo,yo,zo,dx,dy,dz,integral,length;
	long nd,np,ne;
	double rxmin,rxmax,rymin,rymax,amax;
	double xmin,xmax,zmin,ymin,ymax,zmax;
	double *values;
	int flagGradients;
	double rx,ry,rz;
	int ibreak;
	double odxset,odyset,odzset;
	double dx2,dy2,dz2;
	
	if(!li)return 1;
	if(!Files)return 1;
	
	pio=&Files->pioData;
	ss = &Files->SetData;
	values=(double *)ss->data;
	if(!values)return 1;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA || ! li->integrate)return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;

	x1=li->x1;
	x2=li->x2;
	rx=x2-x1;
	y1=li->y1;
	y2=li->y2;
	ry=y2-y1;
	z1=li->z1;
	z2=li->z2;
	rz=z2-z1;

	flagGradients=pio->pa.flagGradients;

	if(GetSDS3DData(Files,li->CurrentFrame))goto OutOfHere;
	
	pio->pa.flagGradients = FALSE;

    setSds3dScales(pio);
	
	sage=pio->sage;
	
	np=10000;
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(np),81880))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(np),81883))){
	    WarningBatch("doIntegrateLine Out of List Memory\n");
	    goto OutOfHere;
	}	
	
	x=x1;
	y=y1;
	z=z1;
	
	length=0;
	integral=0;

	dx=0;
	dy=0;
	dz=0;
	
	s=0;
	
	nd=0;
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;	
	
	odxset=(double)(ss->xsize)/(pio->xmax-pio->xmin);
	dx2=1.0/odxset;

	odyset=(double)(ss->ysize)/(pio->ymax-pio->ymin);
	dy2=1.0/odyset;

	odzset=(double)(ss->zsize)/(pio->zmax-pio->zmin);
	dz2=1.0/odzset;

	while(1){
	    double value,xc,yc,zc,dl,yy,xx,zz;
	    long i,j,k;
	    
	    xx=x+dx;
	    if(xx < pio->xmin || xx > pio->xmax)break;
	    
	    yy=y+dy;
	    if(yy < pio->ymin || yy > pio->ymax)break;
	    
	    zz=z+dz;
	    if(zz < pio->zmin || zz > pio->zmax)break;
	    
		i=(long)((xx-pio->xmin)*odxset);	
		if(i < 0){
		    i=0;
		}else if(i >= ss->xsize){
		    i=ss->xsize-1;
		}
	    
		j=(long)((yy-pio->ymin)*odyset);
		if(j < 0){
		    j=0;
		}else if(j >= ss->ysize){
		    j=ss->ysize-1;
		}
		
		
		k=(long)((zz-pio->zmin)*odzset);
		if(k < 0){
		    k=0;
		}else if(k >= ss->zsize){
		    k=ss->zsize-1;
		}
		
		
	    ne=i+j*ss->xsize+k*ss->xsize*ss->zsize;
	    if(ne < 0 || ne >= ss->xsize*ss->ysize*ss->zsize){
			break;
	    }
		
		value=values[ne];
        xc=((double)i+0.5)*dx2+pio->xmin;
	    xmin=xc-dx2*1.0000001;
	    xmax=xc+dx2*1.0000001;
        yc=((double)j+0.5)*dy2+pio->ymin;
	    ymin=yc-dy2*1.0000001;
	    ymax=yc+dy2*1.0000001;
        zc=((double)k+0.5)*dz2+pio->zmin;
	    zmin=zc-dz2*1.0000001;
	    zmax=zc+dz2*1.0000001;
 	    
	  	  	        
	  	s=-1;
	    if(rx){
	        s1=((xc-dx2)-x1)/rx;
	        if(s1 > s){
		        yy=y1+s1*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            zz=z1+s1*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }	        
	        }
	        s2=((xc+dx2)-x1)/rx;
	        if(s2 > s){	        
	        	yy=y1+s2*ry;
		        if(yy < ymin || yy > ymax){
		            ;
		        }else{
		            zz=z1+s2*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
	        }
	    }
	        
	    if(ry){
	        s1=((yc-dy2)-y1)/ry;
            if(s1 > s){        
            	xx=x1+s1*rx;	        
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            zz=z1+s1*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }
	        }
	        s2=((yc+dy2)-y1)/ry;
            if(s2 > s){
                xx=x1+s2*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            zz=z1+s2*rz;
			        if(zz < zmin || zz > zmax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
            }
	    }
	    
	    if(rz){
	        s1=((zc-dz2)-z1)/rz;
            if(s1 > s){        
            	xx=x1+s1*rx;	        
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            yy=y1+s1*ry;
			        if(yy < ymin || yy > ymax){
			            ;
			        }else{
			            s=s1;
			        }	        
		        }
	        }
	        s2=((zc+dz2)-z1)/rz;
            if(s2 > s){
                xx=x1+s2*rx;	                
		        if(xx < xmin || xx > xmax){
		            ;
		        }else{
		            yy=y1+s2*ry;
			        if(yy < ymin || yy > ymax){
			            ;
			        }else{
			            s=s2;
			        }	        
		        }	        
            }
	    }
	    
	    if(s < 0){
	        sprintf(WarningBuff,"doIntegrateLine : Error Cell %ld No length\n",ne);
    		WarningBatch(WarningBuff);
    		goto OutOfHere;
	    }
	    
		if(s >= 1.0){
		   s=1.0;
		   ibreak=TRUE;
		}else{
		   ibreak=FALSE;
		}
		
		if(nd+4 > np){
		    double *xdn,*ydn;
		    		    	    
		   	xdn=(double *)cRealloc((char *)xD,2*np*sizeof(double),3761);
		   	ydn=(double *)cRealloc((char *)yD,2*np*sizeof(double),3762);
		   	if(!xdn || !ydn || (nd > 1000000)){
	        	sprintf(WarningBuff,"doIntegrateLine : Error Too many Points %ld\n",nd);
    			WarningBatch(WarningBuff);
    			if(xdn){
    			    cFree((char *)xdn);
    				xD=NULL;
    			}
    			if(ydn){
    				cFree((char *)ydn);
    				yD=NULL;
    			}
    			goto OutOfHere;
    		}
    		xD=xdn;
    		yD=ydn;
    		np=2*np;
		}

	    xD[nd]=length;
	    yD[nd++]=value;

	    if(length < rxmin)rxmin=length;
	    if(length > rxmax)rxmax=length;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;

	    xo=x1+s*rx;
	    yo=y1+s*ry;	
	    zo=z1+s*rz;	
		
		dl=sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y)+(zo-z)*(zo-z));
		
		integral += value*dl;
		
		length += dl;
    
	    xD[nd]=length;
	    yD[nd++]=value;
	    
	    if(ibreak)break;
	    
	    x=xo;
	    y=yo;
	    z=zo;
	    dx=1e-6*rx;
	    dy=1e-6*ry;
	    dz=1e-6*rz;
	}
	
	
	
	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	li->integral=integral;

	pio->pa.flagGradients=flagGradients;
	
	if(length > 0){
	    double ll;
	    ll=sqrt(rx*rx+ry*ry+rz*rz);
		if(fabs(length-ll)/length > 1e-5){
		     sprintf(WarningBuff,"doIntegrateLine Line Length %g Not Equal To Integral Length %g\n",ll,length);
		     WarningBatch(WarningBuff);
		}
	}
	
	
	
	return 0;
OutOfHere:
	pio->pa.flagGradients=flagGradients;
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}
static int doSDS3DGetLineData(struct FileInfo2 *Files,struct linedata *li)
{
	int ix1,iy1,ix2,iy2;
	double x1,y1,z1,x2,y2,z2,dl,ds,dx,dy,dz,dp;
	long ns,n,nd;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	long CurrentFrame;
    struct FilePIOInfo *pio;
	
	if(!li)return 1;
	if(li->type == LINEDATA_POINT_DATA){
	    return doPointGetData(Files,li);
	}else if(li->type == LINEDATA_LIST_DATA){
	     return doPointGetListData(Files,li);
	}
	if(!Files)return 1;
	pio=&Files->pioData;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_LINE_DATA)return 1;
	
	if(li->integrate)return doIntegrateLine(Files,li);
	

	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;
	
	pio->pd.AreaFill=TRUE;

	x1=li->x1;
	y1=li->y1;
	z1=li->z1;
	x2=li->x2;
	y2=li->y2;
	z2=li->z2;
	
		
	if(RealLoc(pio,&x1,&y1,&ix1,&iy1))return 1;
	if(RealLoc(pio,&x2,&y2,&ix2,&iy2))return 1;
	
	dl=sqrt((ix2-ix1)*(ix2-ix1)+(iy2-iy1)*(iy2-iy1));

	if(dl <= 0.0)dl=2.0;

	ns=(long)(2*dl+2);

	if(x1 == x2){
	    dl=abs(iy1-iy2);
	    ns=(long)(dl+1);
	}else if(y1 == y2){
	    dl=abs(ix1-ix2);
	    ns=(long)(dl+1);
	}

	if(ns < 2)ns=2;
	
	if((li->PointLineCount > 2) && (li->PointLineCount < 20000))ns=li->PointLineCount;

	ds=1.0/((double)(ns-1));


	CurrentFrame=li->CurrentFrame;
		
	if(GetSDS3DData(Files,CurrentFrame))goto OutOfHere;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

    setSds3dScales(pio);
	
	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81873))){
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    goto OutOfHere;
	}


	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	nd=0;
	for(n=0;n<ns;++n){
	    double value,x,y,z;

	    x=x1+(double)n*ds*(x2-x1);
	    y=y1+(double)n*ds*(y2-y1);
	    z=z1+(double)n*ds*(z2-z1);

	    if(!pioGetValue(Files,x,y,z,&value))continue;

	    dx=x-x1;
	    dy=y-y1;
	    dz=z-z1;
	    dp=sqrt(dx*dx+dy*dy+dz*dz);

	    xD[nd]=dp;
	    yD[nd]=value;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;
	    nd++;
	}
	
	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	return 0;
OutOfHere:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}
static int doPointGetData(struct FileInfo2 *Files,struct linedata *li)
{
	long ns,n,nd,nf,NumberOfRings,NumberOfPoints,nr,nt1,nt2;
	double valuet;
	double r,theta1,theta2,dr,dtheta;
	double dp;
	double *xD,*yD;
	double rxmin,rxmax,rymin,rymax,amax;
	double x,y,z;
	double xs,ys,zs;
	long nFirst,nLast;
	long CurrentFrame;
	double tFirst,tLast;
    struct FilePIOInfo *pio;
    double rMax,thetaMax1,thetaMax2;

	
	if(!Files)return 1;
	pio=&Files->pioData;
	if(!li)return 1;
	
	xD=NULL;
	yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;
	
	if(li->type != LINEDATA_POINT_DATA)return 1;
	
	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	pio->pioIndex=(int)li->pioIndex;
	pio->pd.AreaFill=TRUE;

	nFirst=li->FirstFrame;
	nLast=li->LastFrame;
	if(nFirst >= nLast){
	    if(Files){
	    	nFirst=0;
	    	nLast=Files->ImageCount-1;
	    }else{
	    	goto OutOfHere;
	    }
	}
	
	if(nLast >= Files->ImageCount){
		nLast=Files->ImageCount-1;
		if(nLast < 0)nLast=0;
	}
	
	if(nFirst >= nLast)nFirst=0;

	ns=nLast-nFirst+1;

	if(ns < 2)goto OutOfHere;

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81872))){
	    goto OutOfHere;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81882))){
	    goto OutOfHere;
	}

	tFirst=li->FirstTime;
	tLast=li->LastTime;
	if(tFirst >= tLast){
	    if(Files){
	    	tFirst=0;
	    	tLast=(double)(Files->ImageCount-1);
	    }else{
	    	goto OutOfHere;
	    }
	}


	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}

	x=li->x1;
	y=li->y1;
	z=li->z1;
	
	amax=1e60;
	rxmin=amax;
	rxmax=-amax;
	rymin=amax;
	rymax=-amax;

	
	if(li->SampleSteps < 1)li->SampleSteps=1;
	
	NumberOfRings=li->SampleSteps;
	
	dr=li->SampleRadius/(double)NumberOfRings;
	
	NumberOfPoints=li->SampleSteps;
	
	dtheta=8*atan(1.0)/((double)NumberOfPoints);

	xs=x;
	ys=y;
	zs=z;
	nd=0;
	rMax=0;
	thetaMax1=0;
	thetaMax2=0;
	
	
	nd=0;
	for(n=nFirst;n<=nLast;++n){
	    double value;
	    CurrentFrame=n;
	    if(GetSDS3DData(Files,CurrentFrame))goto OutOfHere;
	    
    	setSds3dScales(pio);
		 
    	x=xs;
    	y=ys;
    	z=zs;
    	nf=0;
    	r=dr;
    	valuet=0;
    	for(nr=0;nr<NumberOfRings;++nr){
    	    theta1=0;
    	    for(nt1=0;nt1<NumberOfPoints;++nt1){
    	    	theta2=0;
    	    	for(nt2=0;nt2<NumberOfPoints;++nt2){
	    			if(!pioGetValue(Files,x,y,z,&value))continue;
			        
			    	valuet += value;
			    	++nf;
			    	
			    	x=xs+r*cos(theta1)*sin(theta2);
			    	y=ys+r*sin(theta1)*sin(theta2);
			    	z=zs+r*cos(theta1);
			    	/*
			    	sprintf(WarningBuff,"x %g y %g z %g r %g theta1 %g theta2 %g",x,y,z,r,theta1*180/3.1415925,theta2*180/3.1415925);
			    	WarningBatchFast(WarningBuff);
			    	*/
			    	theta2 += 0.5*dtheta;
			    	if(theta2 > thetaMax2)thetaMax2=theta2;
			    	
	    	    }
			    if(theta1 > thetaMax1)thetaMax1=theta1;
			    theta1 += dtheta;
	    	}
			if(r > rMax)rMax=r;
	    	r += dr;
	    }
	    
	    if(nf <= 0)continue;
	    
	    value=valuet/(double)nf;
		        
	    dp=tFirst+(tLast-tFirst)*((double)(n-nFirst))/(double)(nLast-nFirst);
	    if(n >= 0 && n < Files->ImageCount){
	       dp=Files->limits.time;
	    }
	    xD[nd]=dp;
	    yD[nd]=value;
	    if(dp < rxmin)rxmin=dp;
	    if(dp > rxmax)rxmax=dp;
	    if(value < rymin)rymin=value;
	    if(value > rymax)rymax=value;
	    nd++;
	}

	if(!nd)goto OutOfHere;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
	li->rymin=rymin;
	li->rymax=rymax;
	
	return 0;
OutOfHere:
    if(xD)cFree((char *)xD);
    if(yD)cFree((char *)yD);
	return 1;
}
static int pioGetValue(struct FileInfo2 *Files,double x,double y,double z,double *v)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	double xc,yc,zc;
	double *value;
	long i,j,k;
	long loc;
	long ne;

	double xmin,ymin,zmin;
	double xmax,ymax,zmax;
	double odxset,odyset,odzset;
	long ixmax,ixymax;

	if(!Files || !v)return 0;
	pio=&Files->pioData;
	s = &Files->SetData;
	value=(double *)s->data;
	if(!value)return 1;
		
	
	xmin=pio->xmin;
	if(x < xmin)return 0;
	
	xmax=pio->xmax;
	if(x > xmax)return 0;
	
	ymin=pio->ymin;
	if(y < ymin)return 0;
	
	ymax=pio->ymax;
	if(y > ymax)return 0;
	
	zmin=pio->zmin;
	if(z < zmin)return 0;
	
	zmax=pio->zmax;
	if(z > zmax)return 0;
	
	
	odxset=(double)(s->xsize)/(xmax-xmin);
	
	i=(long)((x-xmin)*odxset);	
	if(i < 0){
	    i=0;
	}else if(i >= s->xsize){
	    i=s->xsize-1;
	}
	
	odyset=(double)(s->ysize)/(ymax-ymin);
	
	j=(long)((y-ymin)*odyset);
	if(j < 0){
	    j=0;
	}else if(j >= s->ysize){
	    j=s->ysize-1;
	}
	
	odzset=(double)(s->zsize)/(zmax-zmin);
	
	k=(long)((z-zmin)*odzset);
	if(k < 0){
	    k=0;
	}else if(k >= s->zsize){
	    k=s->zsize-1;
	}

	ixmax=s->xsize;
	ixymax=ixmax*s->ysize;
	
    loc=i+j*ixmax+k*ixymax;
    if(loc < 0 || loc >= ixymax*s->zsize){
		return 0;
    }
    
    ne=loc;
        
	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
    	xc=((double)i+0.5)/odxset+xmin;
    	yc=((double)j+0.5)/odyset+ymin;
    	zc=((double)k+0.5)/odzset+zmin;
		*v=value[ne]+pio->gradx[ne]*(x-xc)+
					 pio->grady[ne]*(y-yc)+
					 pio->gradz[ne]*(z-zc);
	}else{
		*v=value[ne];
	}
    
	return 1;
}
static int getPlotDataGeneral(struct DrawInfo *info,struct FileInfo2 *Files)
{
	struct SetFrameData  *ss;
	struct FilePIOInfo *pio;
	double value;
	long i,j,k;
	double *data;
	
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double didx,djdy,dkdz;
	double dxdi,dydj,dzdk;

	double xc,yc,zc;
	long n;
	double *sout;
	double dr,ds;
	double x,y,z,r,s;
	long nx,ny;
	long ii,jj;
	
	        
	if(!info)return 1;
	pio=info->pio;
	if(!pio)return 1;
	sout=info->sout;
	if(!sout)return 1;
	
	ss = &Files->SetData;
	data=(double *)ss->data;
	if(!data)return 1;
			
	pio->plane.Plane_type = G_PLANE;
	
	setSds3dScales(pio);
		
	nx=info->nx;
	ny=info->ny;
	
	dr=info->dr;
	ds=info->ds;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady){
		pio->doGradients = TRUE;
	}
	
	xmin=pio->xmin;
	xmax=pio->xmax;
	ymin=pio->ymin;
	ymax=pio->ymax;
	zmin=pio->zmin;
	zmax=pio->zmax;
	

	didx=((double)(ss->xsize))/(xmax-xmin);
	djdy=((double)(ss->ysize))/(ymax-ymin);
	dkdz=((double)(ss->zsize))/(zmax-zmin);
	
    dxdi=1.0/didx;
    dydj=1.0/djdy;
    dzdk=1.0/dkdz;
        
	for(jj=0;jj<info->ny;++jj){
	    double v;
	    s=((double)jj+0.5)*ds;
	    for(ii=0;ii<info->nx;++ii){
	        r=((double)ii+0.5)*dr;
	        x=r*info->v1x+s*info->v2x+info->p1x;
	        y=r*info->v1y+s*info->v2y+info->p1y;
	        z=r*info->v1z+s*info->v2z+info->p1z;
	        
	        i=(long)((x-xmin)*didx);
	        if(i < 0){
	            continue;
	        }else if(i >= ss->xsize){
	            continue;
	        }
	        
	        j=(long)((y-ymin)*djdy);
	        if(j < 0){
	            continue;
	        }else if(j >= ss->ysize){
	            continue;
	        }
	        
	        k=(long)((z-zmin)*dkdz);
	        if(k < 0){
	            continue;
	        }else if(k >= ss->zsize){
	            continue;
	        }
	        
	        xc=((double)i+0.5)*dxdi+xmin;
	        yc=((double)j+0.5)*dydj+ymin;
	        zc=((double)k+0.5)*dzdk+zmin;
	        
	        n=i+j*ss->xsize+k*ss->xsize*ss->ysize;
	        
			value=data[n];
	        	        
	    	if(pio->doGradients){
				v=value+(x-xc)*pio->gradx[n]+
				  		(y-yc)*pio->grady[n]+
				  		(z-zc)*pio->gradz[n];
			}else{
				v=value;
			}
			sout[ii+(ny-1-jj)*nx]=v;
	    }
	}			
		    		
	return 0;
}
static int doSDS3DGetGeneratData(struct areadata *ai,struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct DrawInfo info;
	double *data;
		
	
	if(!ai || !Files)return 1;
	pio=&Files->pioData;
		
	data=(double *)cMalloc(ai->xsize*ai->ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,ai->xsize*ai->ysize*sizeof(double));
	
	ai->data=data;

	
	zerol((char *)&info,sizeof(struct DrawInfo));

	info.pio=pio;
	info.ai=ai;
	
	info.sout=ai->data;
		
	info.nx=ai->xsize;
	info.ny=ai->ysize;
	
	info.dr = 1.0/(double)ai->xsize;
	info.drHalf = 0.5*info.dr;
	info.ds = 1.0/(double)ai->ysize;
	info.dsHalf = 0.5*info.ds;
	
	info.p1x=ai->x1;
	info.p1y=ai->y1;
	info.p1z=ai->z1;
	
	info.v1x=ai->x2-ai->x1;
	info.v1y=ai->y2-ai->y1;
	info.v1z=ai->z2-ai->z1;
	
	info.l1=sqrt(info.v1x*info.v1x+info.v1y*info.v1y+info.v1z*info.v1z);
	
	info.v2x=ai->x3-ai->x1;
	info.v2y=ai->y3-ai->y1;
	info.v2z=ai->z3-ai->z1;
	
	info.l2=sqrt(info.v2x*info.v2x+info.v2y*info.v2y+info.v2z*info.v2z);
	
	info.v3x =  (info.v1y*info.v2z-info.v2y*info.v1z);
	info.v3y = -(info.v1x*info.v2z-info.v2x*info.v1z);
	info.v3z =  (info.v1x*info.v2y-info.v2x*info.v1y);
	
	info.d=info.v1x*(info.v2y*info.v3z-info.v2z*info.v3y)-
	       info.v2x*(info.v1y*info.v3z-info.v1z*info.v3y)+
	       info.v3x*(info.v1y*info.v2z-info.v1z*info.v2y);
	
	if(info.l1 <= 0 || info.l2 <= 0 || info.d == 0.0){
	    sprintf(WarningBuff,"doSDS3DGetGeneratData - Error l1 %g l2 %g d %g\n",info.l1,info.l2,info.d);
	    WarningBatch(WarningBuff);
	    return 1;
	}
	


	info.od = 1.0/info.d;
	
	info.rx =  (info.v2y*info.v3z-info.v2z*info.v3y)*info.od;
	info.ry =  (info.v3x*info.v2z-info.v2x*info.v3z)*info.od;
	info.rz =  (info.v3y*info.v2x-info.v3x*info.v2y)*info.od;
	
	info.sx =  (info.v1z*info.v3y-info.v1y*info.v3z)*info.od;
	info.sy =  (info.v3z*info.v1x-info.v3x*info.v1z)*info.od;
	info.sz =  (info.v3x*info.v1y-info.v1x*info.v3y)*info.od;
	
	info.tx =  (info.v1y*info.v2z-info.v1z*info.v2y)*info.od;
	info.ty =  (info.v2x*info.v1z-info.v1x*info.v2z)*info.od;
	info.tz =  (info.v1x*info.v2y-info.v2x*info.v1y)*info.od;
	

	return getPlotDataGeneral(&info,Files);
}
static int doSDS3DGetAreaData(struct FileInfo2 *Files,struct areadata *ai)
{
    struct FilePIOInfo *pio;
	long CurrentFrame;
	long xsize,ysize;
	double *data;

	if(!ai)return 1;
	if(!Files)return 1;
	pio=&Files->pioData;

	if(ai->type != GENERAL_PLANE_DATA && ai->type != AREADATA_AREA_DATA){
	    sprintf(WarningBuff,"doSDS3DGetAreaData - Only Returns AREADATA_AREA_DATA or GENERAL_PLANE_DATA\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	CurrentFrame=ai->CurrentFrame;
	
	if(ai->pioName[0]){
	    mstrncpy(pio->pioName,ai->pioName,255);
	    pio->pioIndex=(int)ai->pioIndex;
	}

	pio->pd.AreaFill=TRUE;
	
	if(GetSDS3DData(Files,CurrentFrame))return 1;
		
	ai->pioTime=Files->pioTime;
	
	if(ai->type == GENERAL_PLANE_DATA){
	    return doSDS3DGetGeneratData(ai,Files);
	}
	
	if(ai->zsize <= 1){

		pio->plane.Plane_type=Z_PLANE;
		xsize=ai->xsize;
		ysize=ai->ysize;
		
		pio->range.xminData=ai->xminArea;
		pio->range.xmaxData=ai->xmaxArea;
		pio->range.yminData=ai->yminArea;
		pio->range.ymaxData=ai->ymaxArea;
		
		pio->plane.Plane_zloc=ai->zminArea;		
	
	}else if(ai->xsize <= 1){
	
		pio->plane.Plane_type=X_PLANE;
		
		ysize=ai->zsize;
		xsize=ai->ysize;
		
		pio->range.xminData=ai->yminArea;
		pio->range.xmaxData=ai->ymaxArea;
		pio->range.yminData=ai->zminArea;
		pio->range.ymaxData=ai->zmaxArea;
				
		pio->plane.Plane_xloc=ai->xminArea;
		
	}else if(ai->ysize <= 1){
	
		pio->plane.Plane_type=Y_PLANE;
		xsize=ai->xsize;
		ysize=ai->zsize;
		
		pio->range.xminData=ai->xminArea;
		pio->range.xmaxData=ai->xmaxArea;
		pio->range.yminData=ai->zminArea;
		pio->range.ymaxData=ai->zmaxArea;
				
		
		pio->plane.Plane_yloc=ai->yminArea;
		
	}else{
		pio->plane.Plane_xmin=ai->xminArea;
		pio->plane.Plane_xmax=ai->xmaxArea;

		pio->plane.Plane_ymin=ai->yminArea;
		pio->plane.Plane_ymax=ai->ymaxArea;

		pio->plane.Plane_zmin=ai->zminArea;
		pio->plane.Plane_zmax=ai->zmaxArea;

	    pio->plane.Plane_xcount=ai->xsize;
		
	    pio->plane.Plane_ycount=ai->ysize;
	    
	    pio->plane.Plane_zcount=ai->zsize;
	    
	    ai->data=buildSDS3D(Files);
	    
	    if(!ai->data){
	        WarningBatch("doSDS3DGetAreaData Error in buildSDS3D\n");
	        return 1;
	    }else{
	        return 0;
	    }
	    
	    
	}
	
		
	pio->range.ixmax=xsize;
	pio->range.iymax=ysize;
	
	pio->range.ixminData=0;
	pio->range.iyminData=0;
	
	pio->range.ixmaxData=xsize;
	pio->range.iymaxData=ysize;
		
	
	data=(double *)cMalloc(xsize*ysize*sizeof(double),72635);
	if(!data)return 1;
	zerol((char *)data,xsize*ysize*sizeof(double));
	
	ai->data=data;
	 	
    if(getPlotData(Files,data,CurrentFrame)){
        return 1;
    }	    
		    
	return 0;
}
static int doSDS3DGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	long length,n;
	double *data,*dp;

	if(!Files || !sd)return 1;
	pio=&Files->pioData;
	s = &Files->SetData;
	
	if(!pio->GetDataFlag)return doSDS3DGetDataSlice(Files,CurrentFrame,sd);

	if(sd->type != FRAME_DATA_FLOAT){
	    sprintf(WarningBuff,"doSDS3DGetData - Only Returns FRAME_DATA_FLOAT\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}
	
	if(GetSDS3DData(Files,CurrentFrame)){
	    sd->xsize=Files->xSize;
	    sd->ysize=Files->ySize;
	    sd->zsize=Files->zSize;
	    length=sd->xsize*sd->ysize*sd->zsize;
	    sd->length=length;
	    sd->data=cMalloc(length*sizeof(double),9206);
	    return 0;
	}		
	
	data=(double *)s->data;
	if(!data){
	    sprintf(WarningBuff,"doSDS3DGetData - Error2 - GetSDS3DData\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}	
	

    sd->xsize=s->xsize;
    sd->ysize=s->ysize;
    sd->zsize=s->zsize;
    length=sd->xsize*sd->ysize*sd->zsize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9206);
    dp=sd->data;
    if(!dp){
    	sprintf(WarningBuff,"doSDS3DGetData - Out of Memory\n");
    	WarningBatch(WarningBuff);
    	return 1;
    }
	
	for(n=0;n<length;++n)dp[n]=data[n];
	
	
	return 0;
}
static int doSDS3DGetCellData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct SetFrameData *s;
	double *fdata,*data;
	char name[256];
	long length;
    int ret;
	long n;

	if(!Files)return 1;
	if(!sd)return 1;
	s = &Files->SetData;

	ret=1;
	

	if(sd->type != LINEDATA_CELL_DATA){
	    sprintf(WarningBuff,"doSDS3DGetCellData - Only Returns LINEDATA_CELL_DATA\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	name[0]=0;
	mstrncpy(name,sd->pioName,255);
	if(name[0] == 0){
		mstrncpy(name,"density",255);
	}


	if(GetSDS3DData(Files,CurrentFrame))goto ErrorOut;
	
	length=s->xsize*s->ysize*s->zsize;		
    sd->xsize=length;
    sd->ysize=1;
    sd->zsize=1;
    sd->length=length;
    data=(double *)s->data;
    sd->data=(double *)cMalloc(length*sizeof(double),8261);
    fdata=(double *)sd->data;
    sd->pioTime=s->pioTime;
    if(!data || !fdata){
    	sprintf(WarningBuff,"doSDS3DGetCellData - Out of Memory\n");
    	WarningBatch(WarningBuff);
		goto ErrorOut;
    }

	for(n=0;n<length;++n)fdata[n]=data[n];
    
    ret=0;
    
ErrorOut:
	
	return ret;

}
static int SliceFloatAndRaster(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    double *sout;
    int ret;

	if(!Files || !sd)return 1;
	pio=&Files->pioData;
	limits=&Files->limits;
	
	ret = 1;
	
	if(sd->type != FRAME_DATA_FLOAT_RASTER){
	    sprintf(WarningBuff,"SliceFloatAndRaster - Only Returns FRAME_DATA_FLOAT_RASTER\n");
	    WarningBatch(WarningBuff);
	    return 1;
	}

	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;
		pio->plane  = sd->plane;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
	}
	
	pio->pd.AreaFill=TRUE;
	
	if(GetSDS3DData(Files,CurrentFrame))return 1;
        		
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9206);
    if(!sd->data){
    	sprintf(WarningBuff,"SliceFloatAndRaster - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }

	sout=(double *)sd->data;
    
	setFloat(sout,length);

    sd->data2=cMalloc(length*sizeof(double),9204);
    if(!sd->data2){
    	sprintf(WarningBuff,"SliceFloatAndRaster - Out of Memory\n");
    	WarningBatch(WarningBuff);
		    goto ErrorOut;
    }
    
    zerol((char *)sd->data2,length*sizeof(double));
    
    if(getPlotData(Files,sout,CurrentFrame)){
		goto ErrorOut;
    }
    
	sd->pd=pio->pd;
	
    sd->limits = *limits;
    
	FrameDataToRaster(sd,sd->data2);
    
	ret = 0;
	
ErrorOut:
    
	return ret;

}
static int doSDS3DGetDataSlice(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct FilePIOInfo *pio;
	struct LIMITS *limits;
	long length;
    double *sout;

	if(!Files || !sd)return 1;
	pio=&Files->pioData;
	limits=&Files->limits;
	
	
	
 	if(sd->type == FRAME_DATA_FLOAT_RASTER){
 	    return SliceFloatAndRaster(Files,CurrentFrame,sd);
 	}


	if(sd->type != FRAME_DATA_FLOAT){
	    if(sd->type == FRAME_DATA_RGB){
	           ;
	    }else if(sd->type == FRAME_DATA_RASTER){
	        return 1;
	    }else{
		    sprintf(WarningBuff,"doSDS3DGetDataSlice - Only Returns FRAME_DATA_FLOAT\n");
		    WarningBatch(WarningBuff);
		    return 1;
	    }
	}

	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}

	if(sd->typePlotRange == FRAME_RANGE_LIMITS){
		pio->range  = sd->range;
		pio->plane  = sd->plane;		
		pio->vector = sd->vector;		
		pio->pd     = sd->pd;		
	}
	
	pio->pd.AreaFill=TRUE;
	
	if(GetSDS3DData(Files,CurrentFrame))return 1;
        		
    sd->xsize=pio->range.ixmax;
    sd->ysize=pio->range.iymax;
    sd->zsize=1;
    length=sd->xsize*sd->ysize;
    sd->length=length;
    sd->data=cMalloc(length*sizeof(double),9206);
    if(!sd->data){
    	sprintf(WarningBuff,"doSDS3DGetDataSlice - Out of Memory\n");
    	WarningBatch(WarningBuff);
    	return 1;
    }

	sout=(double *)sd->data;
    
	setFloat(sout,length);

    
    if(getPlotData(Files,sout,CurrentFrame)){
        return 1;
    }
    
	sd->pd=pio->pd;
	
    if(sd->type == FRAME_DATA_RGB){
		sd->pd.dmin=pio->pd.dmin;
		sd->pd.dmax=pio->pd.dmax;
		FrameDataToRaster(sd,(unsigned char *)sout);
		convertTO24(sd,&pio->pd);
    }
    
    sd->limits = *limits;
    
	return 0;

}
static int doAuto(struct FileInfo2 *Files)
{
	int flag;
	long n;
	int ret;
	
	if(!Files)return 1;
	
	ret=1;
	
	flag=0;
	for(n=0;n<Files->ImageCount;++n){	    
	    if(GetSDS3DData(Files,n))goto ErrorOut;
	    if(n >= (Files->ImageCount-1))flag=1;
	    if(PlotSDS3DData(Files,n,flag))goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
	return ret;
}
static int PlotSDS3DData(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;
	struct screenData ss;
	unsigned char *buff;
	    
	char filepath[2048];
	char name[256],*namef;
	long length;
	double *data;
	int ret;
	
	if(!Files || (CurrentFrame < 0))return 1;
	pio = &Files->pioData;
	
	ret=1;
	data=NULL;
	
	length=(long)(pio->range.ixmax*pio->range.iymax*sizeof(double));
	
	buff=(unsigned char *)cMalloc(length,8765);
	if(!buff)goto ErrorOut;
	zerol((char *)buff,length);
	
		
	namef=FilesImageName(Files,CurrentFrame);
	if(namef){
		mstrncpy((char *)name,namef,255);	
	}else{
		sprintf(name,"image.%04ld%c",CurrentFrame,0);
	}
		    
	filepath[0]=0;
	mstrncpy(filepath,Files->outputDirectory,2048);
	strncatToPath(filepath,Files->outputName,2048);

	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
		pio->doGradients = TRUE;
	}
	
	if(Files->outputType == 0 || Files->outputType == HDF_FILES){ 
		zerol((char *)&ss,sizeof(struct screenData));
	    ss.viewRect.x=0;
	    ss.viewRect.y=0;
	    ss.viewRect.xsize=(int)(pio->range.ixmax);
	    ss.viewRect.ysize=(int)(pio->range.iymax);
	    ss.buffer=(unsigned char *)buff;
	    ss.ScreenBits=8;
	    ss.palette=&pio->pd.palette[0];
		ss.ZoneLevel=pio->pa.ZoneLevel;
		if(pio->pa.DrawZones < 2)ss.ZoneLevel=99999;
	    
		sds3dDraw(Files,&ss);
		
		sds3dSetLimits(Files,CurrentFrame);
				
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    	    
		if(hdfOut(filepath,name,(char *)pio->pd.palette,pio->range.ixmax,pio->range.iymax,(char *)buff,CurrentFrame,flag,limits))goto ErrorOut;

	}else if(Files->outputType == SDS2D_FILES){
        double *sout;
						
		sout=(double *)buff;

		getPlotData(Files,sout,CurrentFrame);
	    		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	        
	    if(sds2DOut(filepath,name,pio->range.ixmax,pio->range.iymax,sout,CurrentFrame,flag,limits))goto ErrorOut;
	    
	}else if(Files->outputType == SDS3D_FILES){
	    	    
	    data=buildSDS3D(Files);
	    if(!data)goto ErrorOut;
	    			        
		sds3dSetLimitsSDS3D(Files,CurrentFrame);
		
	    limits = &Files->limits;
	    if(!limits->iGot)limits=NULL;
	    
	    if(sds3DOut(filepath,name,pio->plane.Plane_xcount,pio->plane.Plane_ycount,pio->plane.Plane_zcount,data,CurrentFrame,flag,limits))goto ErrorOut;
	   	    
	}
	ret = 0;
ErrorOut:
	if(buff)cFree((char *)buff);
	if(data)cFree((char *)data);
	return ret;
}
static int setSds3dScales(struct FilePIOInfo *pio)
{
	if(!pio)return 1;

	if(pio->plane.Plane_type == X_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->ymin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->zmin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->ymax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->zmax;
	}else if(pio->plane.Plane_type == Y_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->zmin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->zmax;
	}else if(pio->plane.Plane_type == Z_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->ymin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->ymax;
	}else if(pio->plane.Plane_type == G_PLANE){
	    if(!pio->range.xminSet)pio->range.xminData=pio->xmin;
	    if(!pio->range.yminSet)pio->range.yminData=pio->ymin;
	    if(!pio->range.zminSet)pio->range.zminData=pio->zmin;
	    if(!pio->range.xmaxSet)pio->range.xmaxData=pio->xmax;
	    if(!pio->range.ymaxSet)pio->range.ymaxData=pio->ymax;
	    if(!pio->range.zmaxSet)pio->range.zmaxData=pio->zmax;
	}

    pio->range.xScale=((double)(pio->range.ixmaxData-pio->range.ixminData))/(pio->range.xmaxData-pio->range.xminData);
    pio->range.yScale=((double)(pio->range.iymaxData-pio->range.iyminData))/(pio->range.ymaxData-pio->range.yminData);

	return 0;
}
static int sds3dDraw(struct FileInfo2 *Files,struct screenData *ss)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
   	static double  small2=1e-10;
	struct dRange r;
	double *data;
	long length;
	long n;

	if(!Files || !ss)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(double *)s->data;
	if(!data)return 1;
	
	length=s->xsize*s->ysize*s->zsize;

	setSds3dScales(pio);

	r.xmin=pio->range.xminData;
	r.ymin=pio->range.yminData;
	r.xmax=pio->range.xmaxData;
	r.ymax=pio->range.ymaxData;

	r.dxlim=small2*(r.xmax-r.xmin);
	if(r.dxlim <= 0.0)r.dxlim=small2;
	r.dylim=small2*(r.ymax-r.ymin);
	if(r.dylim <= 0.0)r.dylim=small2;


	{
		double pmin;
		double pmax;

		pio->pd.dmin=0;
		pio->pd.dmax=255;

		pmin=pio->pd.sPmin;
		pmax=pio->pd.sPmax;

		if(pio->pd.sType == 0){
		    double dmin,dmax,d;
		    dmin =  data[0];
		    dmax = dmin;
		    
			for(n=0;n<length;++n){
			    d=data[n];
			    if(d < dmin){
			        dmin=d;
			    }else if(d > dmax){
			        dmax=d;
			    }
			}

		    if(dmax <= dmin){
		        dmax=dmin+1.;
		        --dmin;
		    }

		    pio->pd.dmin=dmin;
		    pio->pd.dmax=dmax;
		    pio->a=(254.0-2.0)/(dmax-dmin);
		    pio->b=2.0-pio->a*dmin;
		}else if(pio->pd.sType == 1){
		    pio->a=(254.0-2.0)/(log10(pmax)-log10(pmin));
		    pio->b=2.0-pio->a*log10(pmin);
		}else if(pio->pd.sType == 2){
		    pio->a=(254.0-2.0)/(pmax-pmin);
		    pio->b=2.0-pio->a*pmin;
		}

	}
	
	
	DrawCellColor(Files,ss,&r);

	return 0;
}
static int DrawCellColor(struct FileInfo2 *Files,struct screenData *ss,struct dRange *r)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	double x[3],y[3],c[3],value;
	double x1,y1,x2,y2,x3,y3,x4,y4;
	double xc,yc,zc,zs;
	long i,j,k;
	double dx,dy,dz;
	double *data;
	long n;
	int in;
	    
	if(!Files || !ss || !r)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(double *)s->data;
	if(!data)return 1;
	
	
    dx=(pio->xmax-pio->xmin)/(double)s->xsize;
    dy=(pio->ymax-pio->ymin)/(double)s->ysize;
    dz=(pio->zmax-pio->zmin)/(double)s->zsize;
		    
    if(pio->plane.Plane_type == X_PLANE){
		if(pio->plane.Plane_xloc < pio->xmin || pio->plane.Plane_xloc > pio->xmax)return 1;
		i=(long)((pio->plane.Plane_xloc-pio->xmin)/dx);
		if(i < 0){
		   i = 0;
		}else if(i >= s->xsize){
		   i= s->xsize-1;
		}
		zs=pio->plane.Plane_xloc;
		zc=pio->xmin+((double)i+0.5)*dx;
		for(k=0;k<s->zsize;++k){
		    for(j=0;j<s->ysize;++j){
		    			    
			    x1=pio->ymin+((double)j)*dy;
			    x2=x1+dy;
			    x3=x2;
			    x4=x1;
			    y1=pio->zmin+((double)k)*dz;
			    y2=y1;
			    y3=y1+dz;
			    y4=y3;

				in=0;
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			    	in=1;
			    }

			    if(!isOut(&x2,&y2,&x3,&y3,r)){
			    	in=1;
			    }

			    if(!isOut(&x3,&y3,&x4,&y4,r)){
			    	in=1;
			    }

			    if(!isOut(&x4,&y4,&x1,&y1,r)){
			    	in=1;
			    }
			    if(!in)continue;
		    
			    x1=pio->ymin+((double)i)*dy;
			    xc=x1+0.5*dy;
			    x2=x1+dy;
			    x3=x2;
			    x4=x1;
			    y1=pio->zmin+((double)k)*dz;
			    yc=y1+0.5*dz;
			    y2=y1;
			    y3=y1+dz;
			    y4=y3;
		    
				x[0]=x1;
				x[1]=x2;
				x[2]=x4;
				y[0]=y1;
				y[1]=y2;
				y[2]=y4;
				
				n=i+j*s->xsize+k*s->xsize*s->ysize;
								
				value=data[n];
				
				if(pio->doGradients){	
				    c[0]=value+(x1-xc)*pio->grady[n]+(y1-yc)*pio->gradz[n]+(zs-zc)*pio->gradx[n];
				    c[1]=value+(x2-xc)*pio->grady[n]+(y2-yc)*pio->gradz[n]+(zs-zc)*pio->gradx[n];
				    c[2]=value+(x4-xc)*pio->grady[n]+(y4-yc)*pio->gradz[n]+(zs-zc)*pio->gradx[n];
				}else{
				    c[0]=value;
				    c[1]=value;
				    c[2]=value;
			    }
			    
				ConOne(x,y,c,pio,ss);
				
				if(pio->doGradients){	
				    c[0]=value+(x3-xc)*pio->grady[n]+(y3-yc)*pio->gradz[n]+(zs-zc)*pio->gradx[n];
				    c[1]=value+(x4-xc)*pio->grady[n]+(y4-yc)*pio->gradz[n]+(zs-zc)*pio->gradx[n];
				    c[2]=value+(x2-xc)*pio->grady[n]+(y2-yc)*pio->gradz[n]+(zs-zc)*pio->gradx[n];
				}
				
				x[0]=x3;
				x[1]=x4;
				x[2]=x2;
				y[0]=y3;
				y[1]=y4;
				y[2]=y2;
				ConOne(x,y,c,pio,ss);		    
		    }
		}
    }else if(pio->plane.Plane_type == Y_PLANE){
		if(pio->plane.Plane_yloc < pio->ymin || pio->plane.Plane_yloc > pio->ymax)return 1;
		j=(long)((pio->plane.Plane_yloc-pio->ymin)/dy);
		if(j < 0){
		   j = 0;
		}else if(j >= s->ysize){
		   j= s->ysize-1;
		}
		zs=pio->plane.Plane_yloc;
		zc=pio->ymin+((double)j+0.5)*dy;
		for(k=0;k<s->zsize;++k){
		    for(i=0;i<s->xsize;++i){
		    			    
			    x1=pio->xmin+((double)i)*dx;
			    xc=x1+0.5*dx;
			    x2=x1+dx;
			    x3=x2;
			    x4=x1;
			    y1=pio->zmin+((double)k)*dz;
			    yc=y1+0.5*dz;
			    y2=y1;
			    y3=y1+dz;
			    y4=y3;

				in=0;
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			    	in=1;
			    }

			    if(!isOut(&x2,&y2,&x3,&y3,r)){
			    	in=1;
			    }

			    if(!isOut(&x3,&y3,&x4,&y4,r)){
			    	in=1;
			    }

			    if(!isOut(&x4,&y4,&x1,&y1,r)){
			    	in=1;
			    }
			    if(!in)continue;
		    
			    x1=pio->xmin+((double)i)*dx;
			    xc=x1+0.5*dx;
			    x2=x1+dx;
			    x3=x2;
			    x4=x1;
			    y1=pio->zmin+((double)k)*dz;
			    yc=y1+0.5*dz;
			    y2=y1;
			    y3=y1+dz;
			    y4=y3;
		    
				x[0]=x1;
				x[1]=x2;
				x[2]=x4;
				y[0]=y1;
				y[1]=y2;
				y[2]=y4;
				
				n=i+j*s->xsize+k*s->xsize*s->ysize;
				value=data[n];
				
				if(pio->doGradients){	
					c[0]=value+(x1-xc)*pio->gradx[n]+(y1-yc)*pio->gradz[n]+(zs-zc)*pio->grady[n];
		    		c[1]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->gradz[n]+(zs-zc)*pio->grady[n];
		    		c[2]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->gradz[n]+(zs-zc)*pio->grady[n];
				}else{
				    c[0]=value;
				    c[1]=value;
				    c[2]=value;
			    }
			    			    
				ConOne(x,y,c,pio,ss);
				
				if(pio->doGradients){	
					c[0]=value+(x3-xc)*pio->gradx[n]+(y3-yc)*pio->gradz[n]+(zs-zc)*pio->grady[n];
		    		c[1]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->gradz[n]+(zs-zc)*pio->grady[n];
		    		c[2]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->gradz[n]+(zs-zc)*pio->grady[n];
		    	}
				x[0]=x3;
				x[1]=x4;
				x[2]=x2;
				y[0]=y3;
				y[1]=y4;
				y[2]=y2;
				ConOne(x,y,c,pio,ss);		    
		    }
		}
    }else if(pio->plane.Plane_type == Z_PLANE){
		if(pio->plane.Plane_zloc < pio->zmin || pio->plane.Plane_zloc > pio->zmax)return 1;
		k=(long)((pio->plane.Plane_zloc-pio->zmin)/dx);
		if(k < 0){
		   k = 0;
		}else if(k >= s->zsize){
		   k= s->zsize-1;
		}
		zs=pio->plane.Plane_zloc;
		zc=pio->zmin+((double)k+0.5)*dz;
		for(j=0;j<s->ysize;++j){
		    for(i=0;i<s->xsize;++i){
		    			    
			    x1=pio->xmin+((double)i)*dx;
			    xc=x1+0.5*dx;
			    x2=x1+dx;
			    x3=x2;
			    x4=x1;
			    y1=pio->ymin+((double)j)*dy;
			    yc=y1+0.5*dy;
			    y2=y1;
			    y3=y1+dy;
			    y4=y3;

				in=0;
			    if(!isOut(&x1,&y1,&x2,&y2,r)){
			    	in=1;
			    }

			    if(!isOut(&x2,&y2,&x3,&y3,r)){
			    	in=1;
			    }

			    if(!isOut(&x3,&y3,&x4,&y4,r)){
			    	in=1;
			    }

			    if(!isOut(&x4,&y4,&x1,&y1,r)){
			    	in=1;
			    }
			    if(!in)continue;
		    
			    x1=pio->xmin+((double)i)*dx;
			    x2=x1+dx;
			    x3=x2;
			    x4=x1;
			    y1=pio->ymin+((double)j)*dy;
			    y2=y1;
			    y3=y1+dy;
			    y4=y3;
		    
				x[0]=x1;
				x[1]=x2;
				x[2]=x4;
				y[0]=y1;
				y[1]=y2;
				y[2]=y4;
				
				n=i+j*s->xsize+k*s->xsize*s->ysize;
				value=data[n];
				
				
				if(pio->doGradients){	
				    c[0]=value+(x1-xc)*pio->gradx[n]+(y1-yc)*pio->grady[n]+(zs-zc)*pio->gradz[n];
				    c[1]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->grady[n]+(zs-zc)*pio->gradz[n];
				    c[2]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->grady[n]+(zs-zc)*pio->gradz[n];
				}else{
				    c[0]=value;
				    c[1]=value;
				    c[2]=value;
			    }
							    
				ConOne(x,y,c,pio,ss);
				
				
				
				if(pio->doGradients){	
				    c[0]=value+(x3-xc)*pio->gradx[n]+(y3-yc)*pio->grady[n]+(zs-zc)*pio->gradz[n];
				    c[1]=value+(x4-xc)*pio->gradx[n]+(y4-yc)*pio->grady[n]+(zs-zc)*pio->gradz[n];
				    c[2]=value+(x2-xc)*pio->gradx[n]+(y2-yc)*pio->grady[n]+(zs-zc)*pio->gradz[n];
				}
								
				x[0]=x3;
				x[1]=x4;
				x[2]=x2;
				y[0]=y3;
				y[1]=y4;
				y[2]=y2;
				ConOne(x,y,c,pio,ss);		    
		    }
		}

	}

    
	
	return 0;
}
static int ConOne(double *x,double *y,double *p,struct FilePIOInfo *pio,struct screenData *ss)
{
	int ix,iy,ixmin,ixmax,iymin,iymax,large=32000,n,itIn;
	double td,te,tf,tg,th,ti,d,od,r,s,pp,xx,yy;
	double xstart,xend,ystart,yend,dx,dy,tj,tk,tl;
	unsigned int ic;
	double pmin,pmax;
	static double small2 = -1.0e-8;
	static double  onep = 1.000001;
	double a,b;
	int sType;

	if(!x || !y || !p || !pio || !ss)return 1;
	
	
	sType=pio->pd.sType;
	a=pio->a;
	b=pio->b;
	pmin=pio->pd.sPmin;
	pmax=pio->pd.sPmax;

	if(!(d=(x[1]-x[0])*(y[2]-y[0])-(y[1]-y[0])*(x[2]-x[0]))){
	    return 1;
	} 
	    
	    
	od = 1./d;
	
	ix=iy=0;
	xstart=xend=ystart=yend=0;
	iymin=ixmin=  large;
	iymax=ixmax= -large;
	for(n=0;n<3;++n){
	    RealLoc(pio,&x[n],&y[n],&ix,&iy);
	    if(ixmin > ix)ixmin = ix;
	    if(ixmax < ix)ixmax = ix;
	    if(iymin > iy)iymin = iy;
	    if(iymax < iy)iymax = iy;
	}

	td=od*(y[2]-y[0]);
	te=od*(-(x[2]-x[0]));
	tf= -td*x[0]-te*y[0];
	
	tg=od*(-(y[1]-y[0]));
	th=od*(x[1]-x[0]);
	ti= -tg*x[0]-th*y[0];
	
	tj=p[0];
	tk=p[1]-p[0];
	tl=p[2]-p[0];

	if(ixmin < pio->range.ixminData)ixmin=(int)(pio->range.ixminData);
	if(ixmax > pio->range.ixmaxData)ixmax=(int)(pio->range.ixmaxData);
	if(iymin < pio->range.iyminData)iymin=(int)(pio->range.iyminData);
	if(iymax > pio->range.iymaxData)iymax=(int)(pio->range.iymaxData);
	
	
	IntLoc(pio,&xstart,&ystart,ixmin,iymin);
	IntLoc(pio,&xend,&yend,ixmax,iymax);
	if((n=ixmax-ixmin)){
	    dx = (xend - xstart)/n;
	} else {
	    dx=0.;
	}
	if((n=iymax-iymin)){
	    dy = (yend - ystart)/n;
	} else {
	    dy=0.;
	}
	yy=ystart;
	for(iy=iymin;iy<=iymax;++iy,yy += dy){
	    xx=xstart;
	    itIn=FALSE;
	    for(ix=ixmin;ix<=ixmax;++ix,xx += dx){ 
	        if(((r=td*xx+te*yy+tf) >= small2) && ((s=tg*xx+th*yy+ti) >= small2) &&
	           ((r+s) <= onep)){
			    pp=tj+r*tk+s*tl;
			    if(sType == 1){
			        if(pp < pmin)pp=pmin;
			        if(pp > pmax)pp=pmax;
			        pp=log10(pp);
			    }else if(sType == 2){
			        if(pp < pmin)pp=pmin;
			        if(pp > pmax)pp=pmax;
			    }
			    ic=(unsigned char)(a*pp+b);
			    kpoint(ix,iy,ic,ss);
		        itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	}
	return 0;
	
}
static int sds3dSetLimits(struct FileInfo2 *Files,long nf)
{
    struct FilePIOInfo *pio;
	double xmin,xmax,ymin,ymax,xdelta,ydelta;
	long xzones,yzones;
	struct LIMITS *limits;


	if(!Files)return 1;
	pio = &Files->pioData;

	nf=nf;

	limits=&Files->limits;
	limits->iGot = FALSE;
	   
	xdelta=0.0;
	ydelta=0.0;
	xzones=pio->range.ixmaxData-pio->range.ixminData;
	if(xzones > 0)xdelta=(pio->range.xmaxData-pio->range.xminData)/(double)xzones;
	yzones=pio->range.iymaxData-pio->range.iyminData;
	if(yzones > 0)ydelta=(pio->range.ymaxData-pio->range.yminData)/(double)yzones;

	if(!xdelta || !ydelta)return 1;

	limits->pioIndex=Files->pioIndex;

	if(limits->pioName)cFree((char *)limits->pioName);
	limits->pioName=(unsigned char *)strsave(Files->pioName,5281);
	if(!limits->pioName)return 1;

	limits->iGot = TRUE;

	xmin=pio->range.xminData-xdelta*(double)(pio->range.ixminData);
	xmax=pio->range.xmaxData+xdelta*(double)(pio->range.ixmax-pio->range.ixmaxData);

	ymin=pio->range.yminData-ydelta*(double)(pio->range.iyminData);
	ymax=pio->range.ymaxData+ydelta*(double)(pio->range.iymax-pio->range.iymaxData);


	limits->xmin=xmin;
	limits->ymin=ymin;
	limits->xmax=xmax;
	limits->ymax=ymax;
	limits->zmin=0;
	limits->zmax=0;


	limits->vmin=pio->pd.dmin;

	limits->vmax=pio->pd.dmax;

	limits->dolog = (pio->pd.sType == 1);
	
	FilePutCommand(limits);

	return 0;
}
static int sds3dSetLimitsSDS3D(struct FileInfo2 *Files,long nf)
{
    struct FilePIOInfo *pio;
	struct LIMITS *limits;

	if(!Files)return 1;
	pio = &Files->pioData;

	nf=nf;
	
	limits=&Files->limits;
	limits->iGot = FALSE;
	
	limits->pioIndex=Files->pioIndex;
	if(limits->pioName)cFree((char *)limits->pioName);
	limits->pioName=(unsigned char *)strsave(Files->pioName,5281);
	if(!limits->pioName)return 1;

	limits->iGot = TRUE;

	limits->xmin=pio->plane.Plane_xmin;
	limits->ymin=pio->plane.Plane_ymin;
	limits->zmin=pio->plane.Plane_zmin;
	limits->xmax=pio->plane.Plane_xmax;
	limits->ymax=pio->plane.Plane_ymax;
	limits->zmax=pio->plane.Plane_zmax;
	limits->vmin=pio->plane.Plane_dmin;
	limits->vmax=pio->plane.Plane_dmax;
	
	limits->dolog = (pio->pd.sType == 1);
		
	FilePutCommand(limits);

	return 0;
}
static int getPlotData(struct FileInfo2 *Files,double *sout,long CurrentFrame)
{
	struct SetFrameData  *s;
	struct FilePIOInfo *pio;
	double value;
	long i,j,k;
	double dx,dy,dz;
	double dxhalf,dyhalf,dzhalf;
	double *data;
	
	double xmin,xmax,ymin,ymax,dmin,dmax;
	double didx,djdy;
	double dxdi,dydj;
	long ixmin,ixmax,iymin,iymax;

	double ymn,ymx,xmn,xmx;
	double xs,ys,zs,vout;
	double xc,yc,zc;
	long ii,jj,ihi,ilo,jhi,jlo;
	long n;
	        
	if(!Files)return 1;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(double *)s->data;
	if(!data)return 1;
	if(!sout)return 1;
	
	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
		pio->doGradients = TRUE;
	}
	
	
	setSds3dScales(pio);
	
	xmin=pio->range.xminData;
	xmax=pio->range.xmaxData;
	ymin=pio->range.yminData;
	ymax=pio->range.ymaxData;
	ixmin=pio->range.ixminData;
	ixmax=pio->range.ixmaxData;
	iymin=pio->range.iyminData;
	iymax=pio->range.iymaxData;

	didx=((double)(ixmax-ixmin))/(xmax-xmin);
	dxdi=1.0/didx;
	djdy=((double)(iymax-iymin))/(ymax-ymin);
	dydj=1.0/djdy;


	dmin  =1e60;
	dmax= -1e60;
	
    dx=(pio->xmax-pio->xmin)/(double)s->xsize;
    dxhalf=0.5*dx;
    dy=(pio->ymax-pio->ymin)/(double)s->ysize;
    dyhalf=0.5*dy;
    dz=(pio->zmax-pio->zmin)/(double)s->zsize;
    dzhalf=0.5*dz;
		    
    if(pio->plane.Plane_type == X_PLANE){
		if(pio->plane.Plane_xloc < pio->xmin || pio->plane.Plane_xloc > pio->xmax)return 1;
		i=(long)((pio->plane.Plane_xloc-pio->xmin)/dx);
		if(i < 0){
		   i = 0;
		}else if(i >= s->xsize){
		   i= s->xsize-1;
		}
		zs=pio->plane.Plane_xloc;
		zc=pio->xmin+((double)i+0.5)*dx;
		
		
		yc=pio->zmin-dzhalf;			
		for(k=0;k<s->zsize;++k){
		
			yc += dz;			
			ymn=max(ymin,yc-dzhalf);
			ymx=min(ymax,yc+dzhalf);
			if(ymx < ymn)continue;
			
			jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
			jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
			
			
		    xc=pio->ymin-dyhalf;
		    for(j=0;j<s->ysize;++j){
		    		    
		    	xc += dy;
				xmn=max(xmin,xc-dyhalf);
				xmx=min(xmax,xc+dyhalf);
				if(xmx < xmn)continue;				
				ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
				ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);
			    
			    n=i+j*s->xsize+k*s->xsize*s->ysize;
			    
				value=data[n];
				
				for(jj=jlo;jj<=jhi;++jj){
				    ys=ymin+(double)((double)(jj-iymin)+.5)*dydj;
				    if(ys < ymn || ys > ymx)continue;
				    for(ii=ilo;ii<=ihi;++ii){
				        xs=xmin+(double)((double)(ii-ixmin)+.5)*dxdi;
				        if(xs < xmn || xs > xmx)continue;
				        if(pio->doGradients){
							vout=value+(xs-xc)*pio->grady[n]+
							  		   (ys-yc)*pio->gradz[n]+ 
							  	       (zs-zc)*pio->gradx[n];
				        }else{
							vout=value;
				        }			        				        
						sout[ii+(pio->range.iymax-1-jj)*pio->range.ixmax]=vout;
				        if(dmin > vout)dmin=vout;
				        if(dmax < vout)dmax=vout;
				    }
				}				
		    
		    }
		}
    }else if(pio->plane.Plane_type == Y_PLANE){
		if(pio->plane.Plane_yloc < pio->ymin || pio->plane.Plane_yloc > pio->ymax)return 1;
		j=(long)((pio->plane.Plane_yloc-pio->ymin)/dy);
		if(j < 0){
		   j = 0;
		}else if(j >= s->ysize){
		   j= s->ysize-1;
		}
		zs=pio->plane.Plane_yloc;
		zc=pio->ymin+((double)j+0.5)*dy;
		
		yc=pio->zmin-dzhalf;
		for(k=0;k<s->zsize;++k){
		
		    yc += dz;
			ymn=max(ymin,yc-dzhalf);
			ymx=min(ymax,yc+dzhalf);
			if(ymx < ymn)continue;
		    		
			jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
			jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
			
			xc=pio->xmin-dxhalf;
		    for(i=0;i<s->xsize;++i){
		    			    
			    xc += dx;
				xmn=max(xmin,xc-dxhalf);
				xmx=min(xmax,xc+dxhalf);
				if(xmx < xmn)continue;			    
			    			    
				ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
				ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);
				
			    n=i+j*s->xsize+k*s->xsize*s->ysize;
				value=data[n];
								
				
				for(jj=jlo;jj<=jhi;++jj){
				    ys=ymin+(double)((double)(jj-iymin)+.5)*dydj;
				    if(ys < ymn || ys > ymx)continue;
				    for(ii=ilo;ii<=ihi;++ii){
				        xs=xmin+(double)((double)(ii-ixmin)+.5)*dxdi;
				        if(xs < xmn || xs > xmx)continue;
				        if(pio->doGradients){
							vout=value+(xs-xc)*pio->gradx[n]+
							  		   (ys-yc)*pio->gradz[n]+ 
							  	       (zs-zc)*pio->grady[n];
				        }else{
							vout=value;
				        }			        				        
						sout[ii+(pio->range.iymax-1-jj)*pio->range.ixmax]=vout;
				        if(dmin > vout)dmin=vout;
				        if(dmax < vout)dmax=vout;
				    }
				}				

		    }
		}
    }else if(pio->plane.Plane_type == Z_PLANE){
		if(pio->plane.Plane_zloc < pio->zmin || pio->plane.Plane_zloc > pio->zmax)return 1;
		k=(long)((pio->plane.Plane_zloc-pio->zmin)/dz);
		if(k < 0){
		   k = 0;
		}else if(k >= s->zsize){
		   k= s->zsize-1;
		}
		zs=pio->plane.Plane_zloc;
		zc=pio->zmin+((double)k+0.5)*dz;
		
		yc=pio->ymin-dyhalf;
		for(j=0;j<s->ysize;++j){
		    yc += dy;
			ymn=max(ymin,yc-dyhalf);
			ymx=min(ymax,yc+dyhalf);
			if(ymx < ymn)continue;
			jlo=max(iymin,  (long)((ymn-ymin)*djdy)+iymin);
			jhi=min(iymax-1,(long)((ymx-ymin)*djdy)+iymin);
			
			xc=pio->xmin-dxhalf;			
		    for(i=0;i<s->xsize;++i){
		    	xc += dx;
				xmn=max(xmin,xc-dxhalf);
				xmx=min(xmax,xc+dxhalf);
				if(xmx < xmn)continue;
				
				ilo=max(ixmin,(long)((xmn-xmin)*didx)+ixmin);
				ihi=min(ixmax-1,(long)((xmx-xmin)*didx)+ixmin);
			    
			    n=i+j*s->xsize+k*s->xsize*s->ysize;
				value=data[n];
								
				for(jj=jlo;jj<=jhi;++jj){
				    ys=ymin+(double)((double)(jj-iymin)+.5)*dydj;
				    if(ys < ymn || ys > ymx)continue;
				    for(ii=ilo;ii<=ihi;++ii){
				        xs=xmin+(double)((double)(ii-ixmin)+.5)*dxdi;
				        if(xs < xmn || xs > xmx)continue;
				        if(pio->doGradients){
							vout=value+(xs-xc)*pio->gradx[n]+
							  		   (ys-yc)*pio->grady[n]+ 
							  	       (zs-zc)*pio->gradz[n];
				        }else{
							vout=value;
				        }			        				        
						sout[ii+(pio->range.iymax-1-jj)*pio->range.ixmax]=vout;
				        if(dmin > vout)dmin=vout;
				        if(dmax < vout)dmax=vout;
				    }
				}				
		    }
		}

	}
    pio->pd.dmin=dmin;
    pio->pd.dmax=dmax;

	sds3dSetLimits(Files,CurrentFrame);
	    	
	return 0;
}
static int sds3dGradients(struct FileInfo2 *Files,long CurrentFrame,int flag)
{
    struct FilePIOInfo *pio;
	long Length;
	long n;
	struct SetFrameData  *s;
	double *value;
	double dx,dy,dz,gradl,gradh;
	double odx,ody,odz;
	long xsize,ysize,zsize,xysize;
	long i,j,k;
	
	if(flag < 0)return 0;
	if(!Files)return 1;
	pio = &Files->pioData;
	if(!pio)return 1;
	if(Files->fileClass != SDS3D_FILES)return 1;
	s = &Files->SetData;
	value=(double *)s->data;
	if(!value)return 1;
	
	
	xsize=s->xsize;
	ysize=s->ysize;
	zsize=s->zsize;
	xysize=xsize*ysize;
	Length=xsize*ysize*zsize;
	
	if(pio->gradx && pio->grady && pio->gradz &&
	   (CurrentFrame  == pio->CurrentFrameGradients)){
	    return 0;
	}
	
	pio->CurrentFrameGradients = -1;
	

	if(pio->gradx)cFree((char *)pio->gradx);
	pio->gradx=NULL;

	if(pio->grady)cFree((char *)pio->grady);
	pio->grady=NULL;

	if(pio->gradz)cFree((char *)pio->gradz);
	pio->gradz=NULL;

	pio->gradx=(double *)cMalloc(sizeof(double)*Length,19727);
	if(!pio->gradx)goto ErrorOut;
	zerol((char *)pio->gradx,sizeof(double)*Length);

	pio->grady=(double *)cMalloc(sizeof(double)*Length,19728);
	if(!pio->grady)goto ErrorOut;
	zerol((char *)pio->grady,sizeof(double)*Length);

	pio->gradz=(double *)cMalloc(sizeof(double)*Length,19729);
	if(!pio->gradz)goto ErrorOut;
	zerol((char *)pio->gradz,sizeof(double)*Length);
	
    dx=(pio->xmax-pio->xmin)/(double)s->xsize;
    odx=1.0/dx;
    dy=(pio->ymax-pio->ymin)/(double)s->ysize;
    ody=1.0/dy;
    dz=(pio->zmax-pio->zmin)/(double)s->zsize;
    odz=1.0/dz;
    
	for(k=0;k<zsize;++k){
	    for(j=0;j<ysize;++j){
	        for(i=0;i<xsize;++i){
	            n=i+j*xsize+k*xysize;	            
	            if(i != 0 && i != (xsize-1)){
		            gradl=(value[n]-value[n-1])*odx;
		            gradh=(value[n+1]-value[n])*odx;
					if((gradh*gradl >= 0.0)){
						if(fabs(gradh) < fabs(gradl)){
							pio->gradx[n] = gradh;
						}else{
							pio->gradx[n] = gradl;
						}
					}
				}
	            if(j != 0 && j != (ysize-1)){
		            gradl=(value[n]-value[n-xsize])*ody;
		            gradh=(value[n+xsize]-value[n])*ody;
					if((gradh*gradl >= 0.0)){
						if(fabs(gradh) < fabs(gradl)){
							pio->grady[n] = gradh;
						}else{
							pio->grady[n] = gradl;
						}
					}
				}
	            if(k != 0 && k != (zsize-1)){
		            gradl=(value[n]-value[n-xysize])*odz;
		            gradh=(value[n+xysize]-value[n])*odz;
					if((gradh*gradl >= 0.0)){
						if(fabs(gradh) < fabs(gradl)){
							pio->gradz[n] = gradh;
						}else{
							pio->gradz[n] = gradl;
						}
					}
				}
	        }
	    }
	}
	
	pio->CurrentFrameGradients = CurrentFrame;
	
	return 0;
ErrorOut:
	sprintf(WarningBuff,"sds3dGradients Error\n");
	WarningBatch(WarningBuff);
	return 1;
}
static int GetSDS3DData(struct FileInfo2 *Files,long CurrentFrame)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	struct LIMITS *limits;
	
	if(!Files)return 1;
	pio = &Files->pioData;
	
	if(Files->fileClass != SDS3D_FILES)return 1;
	
	if(FilesSetFrame(CurrentFrame,Files))return 1;
	
	s = &Files->SetData;
	
	limits=NULL;
	FileGetLimits(&limits,CurrentFrame,Files);
	if(limits && limits->iGot){
		Files->xminr=pio->xmin=limits->xmin;
		Files->xmaxr=pio->xmax=limits->xmax;

		Files->yminr=pio->ymin=limits->ymin;
		Files->ymaxr=pio->ymax=limits->ymax;

		Files->zminr=pio->zmin=limits->zmin;
		Files->zmaxr=pio->zmax=limits->zmax;
		
		Files->limits.time=limits->time;
		
		if(pio->pd.UsePlotScales){
			if(limits->dolog){
			   pio->pd.sType = 1;
			}else{
			   pio->pd.sType = 2;
			}

			if(limits->vmin == limits->vmax){
			   pio->pd.sType = 0;
			}

			pio->pd.sPmin=limits->vmin;
			pio->pd.sPmax=limits->vmax;
		}
		
	}else{	
		Files->xminr=pio->xmin=0;
		Files->xmaxr=pio->xmax=(double)s->xsize;

		Files->yminr=pio->ymin=0;
		Files->ymaxr=pio->ymax=(double)s->ysize;

		Files->zminr=pio->zmin=0;
		Files->zmaxr=pio->zmax=(double)s->zsize;
		
		if(pio->pd.UsePlotScales){			
			pio->pd.sType = 0;
		}
		
	}

	if(pio->pa.flagGradients)sds3dGradients(Files,CurrentFrame,pio->pa.flagGradients-1);
	
	
	pio->rangeData.xmin=Files->xminr;
	pio->rangeData.ymin=Files->yminr;
	pio->rangeData.zmin=Files->zminr;
	pio->rangeData.xmax=Files->xmaxr;
	pio->rangeData.ymax=Files->ymaxr;
	pio->rangeData.zmax=Files->zmaxr;
	pio->rangeData.xsize=s->xsize;
	pio->rangeData.ysize=s->ysize;
	pio->rangeData.zsize=s->zsize;
	

		
	return 0;
}
static int SetRange(struct FileInfo2 *Files,double xmin,double xmax,double ymin,double ymax,int flag)
{
    struct FilePIOInfo *pio;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	



	if(pio->range.MaintainAspectRatio){
	    double dxo,dyo,dxi,dyi;

	    dxi=(double)(pio->range.ixmaxData-pio->range.ixminData);
	    dyi=(double)(pio->range.iymaxData-pio->range.iyminData);
	    dxo=(xmax-xmin)/dxi;
	    dyo=(ymax-ymin)/dyi;	    
	   
	    if(flag == USE_XY){
		    if(dxo > pio->range.AspectRatio*dyo){  /* use x - range */
		        ymin = ymax - dyi*dxo/pio->range.AspectRatio;
		    }else{			     /* use y - range */
				xmax = xmin + dxi*dyo*pio->range.AspectRatio;
		    }
	    }else if(flag == USE_X){
		        ymax  = ymin + dyi*dxo/pio->range.AspectRatio;
	    }else if(flag == USE_Y){
				xmax =  xmin + dxi*dyo*pio->range.AspectRatio;
	    }
	}
	
	pio->range.xmaxData=xmax;
	pio->range.ymaxData=ymax;
	pio->range.xminData=xmin;
	pio->range.yminData=ymin;

	pio->range.xminSet=1;
	pio->range.xmaxSet=1;
	pio->range.yminSet=1;
	pio->range.ymaxSet=1;

	return 0;
}
static int IntLoc(struct FilePIOInfo *pio,double *x,double *y,int ix,int iy)
{

	if(!pio || !x || !y)return 1;
	if((pio->range.xScale == 0) || (pio->range.yScale == 0))return 1;

	*x=((double)(ix-pio->range.ixminData))/pio->range.xScale+pio->range.xminData;
	*y=((double)(iy-pio->range.iyminData))/pio->range.yScale+pio->range.yminData;

	return 0;
}
static int RealLoc(struct FilePIOInfo *pio,double *x,double *y,int *ix,int *iy)
{

	if(!pio || !x || !y || !ix || !iy)return 1;

	*ix=(int)((*x-pio->range.xminData)*pio->range.xScale+(double)pio->range.ixminData+.5);
	*iy=(int)((*y-pio->range.yminData)*pio->range.yScale+(double)pio->range.iyminData+.5);

	return 0;
}

static double *buildSDS3D(struct FileInfo2 *Files)
{
    struct FilePIOInfo *pio;
	struct SetFrameData  *s;
	double *data;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double didx,djdy,dkdz,dmin,dmax;
	double dxdi,dydj,dzdk;
    double xc,yc,zc;
	double dx,dy,dz;
	double dxhalf,dyhalf,dzhalf;
	long ixmax,iymax,izmax;
	long xsize,ysize,zsize,xysize;
	double *sout;
	long n,length,nn;
	long ixiymax;
	long error;
	long i,j,k;
	int ret;
 	
	if(!Files)return NULL;
	pio = &Files->pioData;
	s = &Files->SetData;
	data=(double *)s->data;
	if(!data)return NULL;
	
	ret=1;
	sout=NULL;
		
	pio->doGradients = FALSE;
	
	if(pio->pa.flagGradients && pio->gradx && pio->grady && pio->gradz){
	    pio->doGradients = TRUE;
	}
	
	xmin=pio->plane.Plane_xmin;
	xmax=pio->plane.Plane_xmax;

	ymin=pio->plane.Plane_ymin;
	ymax=pio->plane.Plane_ymax;

	zmin=pio->plane.Plane_zmin;
	zmax=pio->plane.Plane_zmax;

    ixmax=pio->plane.Plane_xcount;
	
    iymax=pio->plane.Plane_ycount;
    
    izmax=pio->plane.Plane_zcount;
    
    ixiymax=ixmax*iymax;
	
	length=ixmax*iymax*izmax;
	sout=(double *)cMalloc(length*sizeof(double),9658);
	if(!sout)goto OutOfHere;

	for(n=0;n<length;++n)sout[n] = 0.0;
	
	
    didx=((double)(ixmax))/(xmax-xmin);
    dxdi=1.0/didx;
    djdy=((double)(iymax))/(ymax-ymin);
    dydj=1.0/djdy;
    dkdz=((double)(izmax))/(zmax-zmin);
    dzdk=1.0/dkdz;
    
    xsize=s->xsize;
    ysize=s->ysize;
    zsize=s->zsize;
    xysize=xsize*ysize;
    
    dx=(pio->xmax-pio->xmin)/(double)xsize;
    dxhalf=0.5*dx;
    dy=(pio->ymax-pio->ymin)/(double)ysize;
    dyhalf=0.5*dy;
    dz=(pio->zmax-pio->zmin)/(double)zsize;
    dzhalf=0.5*dz;
   
	
	dmin =  1e60;
	dmax = -1e60;
    error=0;    
    
    
	zc=pio->zmin-dzhalf;     
	for(k=0;k<zsize;++k){
        double ymn,ymx,xmn,xmx,zmn,zmx;
        double ovalue,value,xs,ys,zs;
        long ii,jj,kk,ihi,ilo,jhi,jlo,khi,klo;
        zc += dz;
		zmn=max(zmin,zc-dzhalf);
		zmx=min(zmax,zc+dzhalf);
		if(zmx < zmn)continue;
		klo=max(0,(long)((zmn-zmin)*dkdz));
		khi=min(izmax,(long)((zmx-zmin)*dkdz));
		
		yc=pio->ymin-dyhalf;
	    for(j=0;j<ysize;++j){
		    yc += dy;
			ymn=max(ymin,yc-dyhalf);
			ymx=min(ymax,yc+dyhalf);
			if(ymx < ymn)continue;		    
			jlo=max(0,(long)((ymn-ymin)*djdy));
			jhi=min(iymax,(long)((ymx-ymin)*djdy));
			
		    xc=pio->xmin-dxhalf;
	        for(i=0;i<xsize;++i){
		        xc += dx;
				xmn=max(xmin,xc-dxhalf);
				xmx=min(xmax,xc+dxhalf);
				if(xmx < xmn)continue;
				
				ilo=max(0,(long)((xmn-xmin)*didx));
				ihi=min(ixmax,(long)((xmx-xmin)*didx));
				
	            n=i+j*xsize+k*xysize;	            
				value=data[n];
								
			    for(kk=klo;kk<=khi;++kk){
					zs=zmin+((double)kk+0.5)*dzdk;
					if(zs < zmn || zs > zmx)continue;
					for(jj=jlo;jj<=jhi;++jj){
					    ys=ymin+((double)jj+0.5)*dydj;
					    if(ys < ymn || ys > ymx)continue;
					    for(ii=ilo;ii<=ihi;++ii){
					        xs=xmin+((double)ii+0.5)*dxdi;
					        if(xs < xmn || xs > xmx)continue;
							nn=ii+jj*ixmax+kk*ixiymax;
							if(nn < 0 || nn >= length){
							   if(++error < 10){
							       ;
							   }
							}else{
						        if(pio->doGradients){
						            ovalue=value+(xs-xc)*pio->gradx[n]+
						                         (ys-yc)*pio->grady[n]+
						                         (zs-zc)*pio->gradz[n];
						        }else{
						            ovalue=value;
						        }
						        sout[nn]=ovalue;
						        if(ovalue < dmin)dmin=ovalue;
						        if(ovalue > dmax)dmax=ovalue;
						    }
						}
					}
			    }
			            
	        }
	    }
	}
    
    	
	pio->plane.Plane_dmin=dmin;
	pio->plane.Plane_dmax=dmax;
	
	ret=0;
OutOfHere:
    if(ret){
        if(sout)cFree((char *)sout);
        sout=NULL;
    }
	
	return sout;
}

static int doWrite2Dplanes(struct FileInfo2 *Files);
static int doSDS3D2(struct FileInfo2 *Files);
static int doPlane1Action(struct uDialogStruct *uList);
static int doDataPlane(struct FileInfo2 *Files);

static int doDataPlane(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {340,200,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {430,200,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"Save As SDS3D", {150,200,120,20},uDialogButton,uDialogNone},
       /*  3 */      {"xmin-data", {220,20,90,20},uDialogText,uDialogGetDouble},
       /*  4 */      {"ymin-data", {220,50,90,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"zmin-data", {220,80,90,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"gmin-data", {220,110,90,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"xmax-data", {370,20,90,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"ymax-data", {370,50,90,20},uDialogText,uDialogGetDouble},
       /*  9 */      {"zmax-data", {370,80,90,20},uDialogText,uDialogGetDouble},
       /* 10 */      {"gmax-data", {370,110,90,20},uDialogText,uDialogGetDouble},
       /* 11 */      {"xloc-data", {510,20,90,20},uDialogText,uDialogGetDouble},
       /* 12 */      {"yloc-data", {510,50,90,20},uDialogText,uDialogGetDouble},
       /* 13 */      {"zloc-data", {510,80,90,20},uDialogText,uDialogGetDouble},
       /* 14 */      {"gloc-data", {510,110,90,20},uDialogText,uDialogGetDouble},
       /* 15 */      {"xmin", {170,20,35,20},uDialogString,uDialogNone},
       /* 16 */      {"ymin", {170,50,35,20},uDialogString,uDialogNone},
       /* 17 */      {"zmin", {170,80,35,20},uDialogString,uDialogNone},
       /* 18 */      {"gmin", {170,110,35,20},uDialogString,uDialogNone},
       /* 19 */      {"xmax", {320,20,39,20},uDialogString,uDialogNone},
       /* 20 */      {"ymax", {320,50,39,20},uDialogString,uDialogNone},
       /* 21 */      {"zmax", {320,80,39,20},uDialogString,uDialogNone},
       /* 22 */      {"gmax", {320,110,39,20},uDialogString,uDialogNone},
       /* 23 */      {"xloc", {470,20,30,20},uDialogString,uDialogNone},
       /* 24 */      {"yloc", {470,50,30,20},uDialogString,uDialogNone},
       /* 25 */      {"zloc", {470,80,30,20},uDialogString,uDialogNone},
       /* 26 */      {"gloc", {470,110,30,20},uDialogString,uDialogNone},
       /* 27 */      {"x-slices", {15,20,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 28 */      {"y-slices", {15,50,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 29 */      {"z-slices", {15,80,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 30 */      {"g-slices", {15,110,75,20},uDialogButtonRadio,uDialogGetLong},
       /* 31 */      {"xmin-data", {100,20,50,20},uDialogText,uDialogGetLong},
       /* 32 */      {"ymin-data", {100,50,50,20},uDialogText,uDialogGetLong},
       /* 33 */      {"zmin-data", {100,80,50,20},uDialogText,uDialogGetLong},
       /* 34 */      {"gmin-data", {100,110,50,20},uDialogText,uDialogGetLong},
       /* 35 */      {"Save As SDS2D", {20,200,120,20},uDialogButton,uDialogNone},
       /* 36 */      {"Use Gradients", {20,140,120,20},uDialogCheck,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {160,60,620,240};
 
 	uDialogList *uList;
	uDialogReturn *uR;
	struct FilePIOInfo *pio;
    int Plane_type;
    int reti;
    int ret;
    
	if(!Files)return 1;
	pio = &Files->pioData;
	
	Plane_type=pio->plane.Plane_type;
	
	ret = 1;
		
	uR=NULL;
	
	uList=uDialogOpen("Planes",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,27L,
		          uDialogSetLogical,(pio->plane.Plane_type == X_PLANE),
	              
	              uDialogSetItem,28L,
	              uDialogSetLogical,(pio->plane.Plane_type == Y_PLANE),
	              
	              uDialogSetItem,29L,
	              uDialogSetLogical,(pio->plane.Plane_type == Z_PLANE),
	              
	              uDialogSetItem,30L,
	              uDialogSetLogical,(pio->plane.Plane_type == G_PLANE),
	              
	              
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,pio->plane.Plane_xmin,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,pio->plane.Plane_ymin,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,pio->plane.Plane_zmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,pio->plane.Plane_gmin,
	              
	              	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,pio->plane.Plane_xmax,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,pio->plane.Plane_ymax,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,pio->plane.Plane_zmax,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,pio->plane.Plane_gmax,
	              
	              
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,pio->plane.Plane_xloc,
	              
	              uDialogSetItem,12L,
	              uDialogSetDouble,pio->plane.Plane_yloc,
	              
	              uDialogSetItem,13L,
	              uDialogSetDouble,pio->plane.Plane_zloc,
	              
	              uDialogSetItem,14L,
	              uDialogSetDouble,pio->plane.Plane_gloc,
	              
	              	              
	              uDialogSetItem,31L,
	              uDialogSetDouble,(double)pio->plane.Plane_xcount,
	              
	              uDialogSetItem,32L,
	              uDialogSetDouble,(double)pio->plane.Plane_ycount,
	              
	              uDialogSetItem,33L,
	              uDialogSetDouble,(double)pio->plane.Plane_zcount,
	              
	              uDialogSetItem,34L,
	              uDialogSetDouble,(double)pio->plane.Plane_gcount,
	              
	              uDialogSetItem,36L,
	              uDialogSetLogical,(pio->pa.flagGradients == 1),
	              	              
	              uDialogSetMyIcon,Files,	      
	              uDialogSetAction,doPlane1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;

/*
Start:
*/
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
        
	ret=0;
	
	if(Plane_type != pio->plane.Plane_type){
		if(pio->plane.Plane_type == X_PLANE){
			SetRange(Files,pio->plane.Plane_ymin,pio->plane.Plane_ymax,pio->plane.Plane_zmin,pio->plane.Plane_zmax,USE_XY);
	    }else if(pio->plane.Plane_type == Y_PLANE){
			SetRange(Files,pio->plane.Plane_xmin,pio->plane.Plane_xmax,pio->plane.Plane_zmin,pio->plane.Plane_zmax,USE_XY);
	    }else if(pio->plane.Plane_type == Z_PLANE){
			SetRange(Files,pio->plane.Plane_xmin,pio->plane.Plane_xmax,pio->plane.Plane_ymin,pio->plane.Plane_ymax,USE_XY);
	    }

	}
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doPlane1Action(struct uDialogStruct *uList)
{
	struct FilePIOInfo *pio;
 	struct FileInfo2 *Files;
	uDialogReturn *uR;
 	int Type;
 	int ret;
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	pio = &Files->pioData;
	
	ret = 1;
	uR=NULL;
	
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 0 || uList->actionItem == 2 || uList->actionItem == 35){
	    if(!Files)goto OutOfHere;	
	    
		uR=uDialogGetReturn(uList);

    	if(!uR)goto OutOfHere;
    	
	    if(uR[31].lreturn <= 0){
	        Warning("x-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[32].lreturn <= 0){
	        Warning("y-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[33].lreturn <= 0){
	        Warning("z-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[34].lreturn <= 0){
	        Warning("g-slices Steps Must Be Greater Than Zero");
	        goto OutOfHere;
	    }

	    if(uR[3].dreturn >= uR[7].dreturn){
	        Warning("xmin Must Be Less Than xmax");
	        goto OutOfHere;
	    }

	    if(uR[4].dreturn >= uR[8].dreturn){
	        Warning("ymin Must Be Less Than ymax");
	        goto OutOfHere;
	    }

	    if(uR[5].dreturn >= uR[9].dreturn){
	        Warning("zmin Must Be Less Than zmax");
	        goto OutOfHere;
	    }

	    if(uR[6].dreturn >= uR[10].dreturn){
	        Warning("gmin Must Be Less Than gmax");
	        goto OutOfHere;
	    }
	    if(uR[27].lreturn){
	       Type=0;
	    }else if(uR[28].lreturn){
	       Type=1;
	    }else if(uR[29].lreturn){
	       Type=2;
	    }else if(uR[30].lreturn){
	       Type=3;
	    }else{
	       Type=0;
	    }

	    if(Type == 3){
	        Warning("g-slices Not Ready");
	        goto OutOfHere;
	    }
	    
		pio->plane.Plane_xmin=uR[3].dreturn;
		pio->plane.Plane_ymin=uR[4].dreturn;
		pio->plane.Plane_zmin=uR[5].dreturn;
		pio->plane.Plane_gmin=uR[6].dreturn;
	              
		pio->plane.Plane_xmax=uR[7].dreturn;
		pio->plane.Plane_ymax=uR[8].dreturn;
		pio->plane.Plane_zmax=uR[9].dreturn;
		pio->plane.Plane_gmax=uR[10].dreturn;
	              
		pio->plane.Plane_xloc=uR[11].dreturn;
		pio->plane.Plane_yloc=uR[12].dreturn;
		pio->plane.Plane_zloc=uR[13].dreturn;
		pio->plane.Plane_gloc=uR[14].dreturn;
	              
	    pio->plane.Plane_xcount=uR[31].lreturn;
	    pio->plane.Plane_ycount=uR[32].lreturn;
	    pio->plane.Plane_zcount=uR[33].lreturn;
	    pio->plane.Plane_gcount=uR[34].lreturn;
	              	              	              
	    pio->pa.flagGradients=(int)uR[36].lreturn;
	    
	    if(Type != pio->plane.Plane_type){
	        pio->plane.Plane_type = Type;
	        pio->range.xminSet=0;
	        pio->range.xmaxSet=0;
	        pio->range.yminSet=0;
	        pio->range.ymaxSet=0;
	    }
    
		pio->doGradients = FALSE;
		if(pio->pa.flagGradients && pio->gradx && pio->grady &&  pio->gradz){
			pio->doGradients = TRUE;
		}    

	    if(uList->actionItem == 2){
	        doSDS3D2(Files);
	    }else if(uList->actionItem == 35){
	        doWrite2Dplanes(Files);
	    }  
    	
	}
	ret = 0;
OutOfHere:	
    if(uR)uDialogFreeReturn(uR);
	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,-uARROW_CURSOR);
	return ret;
}
static int doSDS3D2(struct FileInfo2 *Files)
{
	static  unsigned char file[256]={'I','m','a','g','e','.','s','3','d',0};
	char *namef,name[1024];
	char buff[256];
	struct ImageSaveList *Images;
	char limits[1024];
	static struct FileListInfo info;
	IconPtr StatusWindow;
	double *sout;
	struct FilePIOInfo *pio;
 	long n,ni,nf;
	int ret;
	
	if(!Files)return 1;
	pio=&Files->pioData;
	
	nf=0;
	ret=1;
	sout=NULL;
	
	StatusWindow=NULL;
	
	Images=SaveAllImages("Save Selected SDS3D Images",Files);
	if(!Images)return 1;
	if(GetTheSaveFileName((char *)file,"Save SDS3D File",256,&info))goto OutOfHere;
	    
	    
	StatusWindow=uStartStatusWindow(NULL,"SDS3D");
	    
	for(n=0;n<Images->count;++n){
		int rank,size[3];
			
	    ni=Images->imageList[n];
	    if((namef=FilesImageName(Images->Files,ni))){
			mstrncpy((char *)name,namef,1024);
	    }else{
			sprintf(name,"%s%c",file,0);
	    }
	    
	    sprintf(buff,"%s%s","     Image: ",name);
	    uWriteStatusWindow(StatusWindow,buff);
	    
	    
		if(GetSDS3DData(Files,ni))goto OutOfHere;
	    
	    rank=3;
		size[0]=(int)pio->plane.Plane_zcount;
		size[1]=(int)pio->plane.Plane_ycount;
		size[2]=(int)pio->plane.Plane_xcount;
		    					    
		sout=buildSDS3D(Files);
		if(!sout)goto OutOfHere;
		
	        
    	FilePutCommandBuff(limits,
	        pio->plane.Plane_xmin,pio->plane.Plane_xmax,
	        pio->plane.Plane_ymin,pio->plane.Plane_ymax,
	        pio->plane.Plane_zmin,pio->plane.Plane_zmax,
	        pio->plane.Plane_dmin,pio->plane.Plane_dmax,
	        0,
	        (int)Files->limits.pioIndex,
	        (unsigned char *)Files->limits.pioName,
	        Files->limits.time);
	        
	        	    
		if(sdsOut((char *)file,name,limits,rank,size,sout,nf++))goto OutOfHere;
		
		if(sout)cFree((char *)sout);
		sout=NULL;

	    if(uCheckStatusWindow(StatusWindow,"Stop Writing SDS3D ?"))goto OutOfHere;
	}
	SetFileType((char *)file,"SDS3");
	ret=0;
OutOfHere:

	uStopStatusWindow(StatusWindow);
	
	if(Images)ImageFreeList(Images);
	
    if(sout)cFree((char *)sout);
	return ret;	 
	
}
static int doWrite2Dplanes(struct FileInfo2 *Files)
{
	static  unsigned char file[256]={'I','m','a','g','e','.','s','2','d',0};
	static struct FileListInfo info;
	struct SetFrameData sd;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dmin,dmax;
	double *sout,*sout2;
	long n,length,nn;
	struct FilePIOInfo *pio;
	int ret;
    char name[256],limits[1024];
	int rank,size[2];
	
	if(!Files)return 1;
	pio=&Files->pioData;
	
	ret=1;
	sout=NULL;
	sout2=NULL;
	if(GetTheSaveFileName((char *)file,"SDS2D Name",256,&info))goto OutOfHere;
	
	sd.xsize = pio->plane.Plane_xcount;
	sd.ysize = pio->plane.Plane_ycount;
	sd.zsize = pio->plane.Plane_zcount;

	xmin=pio->plane.Plane_xmin;
	xmax=pio->plane.Plane_xmax;

	ymin=pio->plane.Plane_ymin;
	ymax=pio->plane.Plane_ymax;

	zmin=pio->plane.Plane_zmin;
	zmax=pio->plane.Plane_zmax;
	
	
	sout=buildSDS3D(Files);
	if(!sout)goto OutOfHere;
	
	if(pio->plane.Plane_type == X_PLANE){
	     double dx;
	     
	     dx=(xmax-xmin)/(double)sd.xsize;
	
		length=sd.ysize*sd.zsize;
		sout2=(double *)cMalloc(length*sizeof(double),9659);
		if(!sout2)goto OutOfHere;

	
	    for(n=0;n<sd.xsize;++n){
			long i,j;
	    
	    	sprintf(name,"x-plane(%9.2e)",((double)n+0.5)*dx+xmin);
	    	
	    	dmin =  1e60;
			dmax = -1e60;
			
			
			for(j=0;j<sd.zsize;++j){
			    for(i=0;i<sd.ysize;++i){
				    double value;
				    value=sout[n+j*sd.xsize*sd.ysize+i*sd.xsize];
				    sout2[i+(sd.zsize-1-j)*sd.ysize]=value;
				    if(value < dmin)dmin=value;
				    if(value > dmax)dmax=value;
			    }
			}
								        
	    FilePutCommandBuff(limits,
			        ymin,ymax,
			        zmin,zmax,
			        xmin,xmax,
			        dmin,dmax,
			        0,
			        (int)Files->limits.pioIndex,
			        (unsigned char *)Files->limits.pioName,
			        Files->limits.time);
		      		        		        		      		       		        
	    	rank=2;
			size[0]=(int)sd.zsize;
			size[1]=(int)sd.ysize;
			
			if(sdsOut((char *)file,name,limits,rank,size,sout2,n))goto OutOfHere;
			
			
	    }
	    SetFileType((char *)file,"SDS2");
	}else if(pio->plane.Plane_type == Y_PLANE){
	     double dy;
	     
	     dy=(ymax-ymin)/(double)sd.ysize;
	
		length=sd.xsize*sd.zsize;
		sout2=(double *)cMalloc(length*sizeof(double),9660);
		if(!sout2)goto OutOfHere;

	
	    for(n=0;n<sd.ysize;++n){
			long i,j;
	    
	    	sprintf(name,"y-plane(%9.2e)",((double)n+0.5)*dy+ymin);
	    	
	    	dmin =  1e60;
			dmax = -1e60;
			
			
			for(j=0;j<sd.zsize;++j){
			    for(i=0;i<sd.xsize;++i){
				    double value;
				    value=sout[i+j*sd.xsize*sd.ysize+n*sd.xsize];
				    sout2[i+(sd.zsize-1-j)*sd.xsize]=value;
				    if(value < dmin)dmin=value;
				    if(value > dmax)dmax=value;
			    }
			}
						
		        
	    	FilePutCommandBuff(limits,
		        	xmin,xmax,
		        	zmin,zmax,
		        	ymin,ymax,
		        	dmin,dmax,
			        0,
			        (int)Files->limits.pioIndex,
			        (unsigned char *)Files->limits.pioName,
			        Files->limits.time);
		        		        
	    	rank=2;
			size[0]=(int)sd.zsize;
			size[1]=(int)sd.xsize;
			
			if(sdsOut((char *)file,name,limits,rank,size,sout2,n))goto OutOfHere;
			
	    }
	    SetFileType((char *)file,"SDS2");
	}else if(pio->plane.Plane_type == Z_PLANE){
	
	     double dz;
	     
	     dz=(zmax-zmin)/(double)sd.zsize;
	
		length=sd.xsize*sd.ysize;
		sout2=(double *)cMalloc(length*sizeof(double),9661);
		if(!sout2)goto OutOfHere;

	
	    for(n=0;n<sd.zsize;++n){
			long i,j;
	    
	    	sprintf(name,"z-plane(%9.2e)",((double)n+0.5)*dz+zmin);
	    	
	    	dmin =  1e60;
			dmax = -1e60;
			
			nn=0;
			
			for(j=0;j<sd.ysize;++j){
			    for(i=0;i<sd.xsize;++i){
				    double value;
				    value=sout[n*sd.xsize*sd.ysize+nn++];
				    sout2[i+(sd.ysize-1-j)*sd.xsize]=value;
				    if(value < dmin)dmin=value;
				    if(value > dmax)dmax=value;
			    }
			}
						
	    	FilePutCommandBuff(limits,
		        	xmin,xmax,
		        	ymin,ymax,
		        	zmin,zmax,
		        	dmin,dmax,
			        0,
			        (int)Files->limits.pioIndex,
			        (unsigned char *)Files->limits.pioName,
			        Files->limits.time);
		 		        
	    	rank=2;
			size[0]=(int)sd.ysize;
			size[1]=(int)sd.xsize;
			
			if(sdsOut((char *)file,name,limits,rank,size,sout2,n))goto OutOfHere;
			
	    }
	    SetFileType((char *)file,"SDS2");
	}else{
	    goto OutOfHere;
	}
	
	ret=0;
OutOfHere:
    if(sout)cFree((char *)sout);
    if(sout2)cFree((char *)sout2);
	return ret;	 
}

static int doScale1Action(struct uDialogStruct *uList);
int doDataScaleSDS3D(struct FileInfo2 *Files);

int doDataScaleSDS3D(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {220,160,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {310,160,70,20},uDialogButton,uDialogNone},
       
       /*  2 */      {"Auto Scale - Linear",   {20,20,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  3 */      {"Manual Scale - Log",    {20,50,160,20},uDialogButtonRadio,uDialogGetLong},
       /*  4 */      {"Manual Scale - Linear", {20,75,160,20},uDialogButtonRadio,uDialogGetLong},
       
       /*  5 */      {"",{275,50,105,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"",{275,75,105,20},uDialogText,uDialogGetDouble},
       
       /*  7 */      {"Range Min", {190,50,80,20},uDialogString,uDialogNone},
       /*  8 */      {"Range Max", {190,75,80,20},uDialogString,uDialogNone},
      
       /*  9 */      {"Labeled Palette", {20,105,120,20},uDialogCheck,uDialogGetLong},
       /* 10 */      {"Show Cells", {170,105,120,20},uDialogCheck,uDialogGetLong},
       /* 11 */      {"Area Fill",{170,125,120,20},uDialogCheck,uDialogGetLong},
      
       /* 12 */      {"Show Range", {275,20,100,20},uDialogButton,uDialogNone},
       /* 13 */      {"Use Gradients",{20,125,120,20},uDialogCheck,uDialogGetLong},
       
       /* 14 */      {"Use Plot Range",  {20,145,120,20},uDialogCheck,uDialogGetLong},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,200};
 	
 	
 	uDialogList *uList;
	uDialogReturn *uR;
    
	int sType;
	struct FilePIOInfo *pio;
    int reti;
    int ret;
    
	if(!Files)return 1;
	pio=&Files->pioData;
		
	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Range",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	reti=uDialogSet(uList,2L,
		          uDialogSetLogical,(pio->pd.sType == 0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(pio->pd.sType == 1),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(pio->pd.sType == 2),
	              	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,pio->pd.sPmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,pio->pd.sPmax,

	              uDialogSetItem,9L,
	              uDialogSetLogical,(pio->pd.LabeledPalette == 1),
	              
	              uDialogSetItem,10L,
	              uDialogSetLogical,(pio->pa.DrawZones  == 1),
	              
	              uDialogSetItem,11L,
	              uDialogSetLogical,(pio->pd.AreaFill  == 1),
	              
	              uDialogSetItem,13L,
	              uDialogSetLogical,(pio->pa.flagGradients  == 1),
	              
		          uDialogSetItem,14L,
	              uDialogSetLogical,(pio->pd.UsePlotScales  == 1),
              	              
	              uDialogSetMyIcon,Files,	              
	              uDialogSetAction,doScale1Action,	              
	              
	              uDialogSetDone,uDialogSetDone
	    );
	if(reti)goto ErrorOut;


Start:

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto ErrorOut;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto ErrorOut;
	    
	if(uR[2].lreturn){
	    sType=0;
	}else if(uR[3].lreturn){
	    sType=1;
	    if(uR[5].dreturn <= 0){
	        Warning("Pmin Must Be Greater Than Zero For Log Plots");
	        goto Start;
	    }
	}else{
	    sType=2;
	}
	if(uR[6].dreturn <= uR[5].dreturn){
	    Warning("Max Must Be Greater Than Min");
	    goto Start;
	}
    pio->pd.sType=sType;
	pio->pd.sPmin=  uR[5].dreturn;
	pio->pd.sPmax=  uR[6].dreturn;
	pio->pd.LabeledPalette=(int)uR[9].lreturn;
	pio->pa.DrawZones=(int)uR[10].lreturn;
	pio->pd.AreaFill= (int)uR[11].lreturn;    
	pio->pa.flagGradients= (int)uR[13].lreturn;
	pio->pd.UsePlotScales= (int)uR[14].lreturn;

	pio->doGradients = FALSE;
	if(pio->pa.flagGradients && pio->gradx && pio->grady &&  pio->gradz){
		pio->doGradients = TRUE;
	}
	
    
	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);

	return ret;
}
static int doScale1Action(struct uDialogStruct *uList)
{
	struct SetFrameData  sd;
	struct FileInfo2 *Files;
 	double data,dmin,dmax;
	long nf;
	int ret;
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	
	
	uSetCursor(NULL,-uWATCH_CURSOR);
	
    sd.data=NULL;  
	if(uList->actionItem == 12){
	    if(!Files)goto OutOfHere;
	
	    dmin =  1e60;
	    dmax = -1e60;
	    
		for(nf=0;nf<Files->ImageCount;++nf){
	    
		
		    zerol((char *)&sd,sizeof(struct SetFrameData));
				
		    sd.type=FRAME_DATA_FLOAT;
		    
		    if(doSDS3DGetDataSlice(Files,nf,&sd)){
				goto OutOfHere;
		    }
	    		    
		    data=sd.limits.vmin;
		
	    	if(data > dmax)dmax=data;
	    	if(data < dmin)dmin=data;

		    data=sd.limits.vmax;
		
	    	if(data > dmax)dmax=data;
	    	if(data < dmin)dmin=data;

		    if(sd.data)cFree((char *)sd.data); 
		    sd.data=NULL;  
		    
			if(Stop("Stop Data Ranging ?"))break;
	   }
	   
		ret=uDialogSet(uList,5L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,dmax,
	              	              	              	            	              
	              uDialogSetItem,14L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,2L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,3L,
	              uDialogSetLogical,(0),
	              
	              uDialogSetItem,4L,
	              uDialogSetLogical,(1),
	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
	
	}
OutOfHere:	
	if(uList)uDialogUpdate(uList);
	if(sd.data)cFree((char *)sd.data); 
	uSetCursor(NULL,-uARROW_CURSOR);
	return 0;
}
static int uTestRange(uDialogReturn *uR);
int doRange2Action(struct uDialogStruct *uList);
static int SendRange(struct FileInfo2 *Files,uDialogReturn *uR);

static int doDataRange(struct FileInfo2 *Files);

static int doDataRange(struct FileInfo2 *Files)
{
/*
	static int lflag[]={
			    DialogInt,DialogInt,
			    DialogFloat,DialogFloat,DialogFloat,DialogFloat,
			    DialogInt,DialogInt,DialogInt,DialogInt,
			    DialogCheck,DialogCheck,DialogUser
			    };
*/			    
	static int flagout[16]={USE_XY,USE_X,USE_X,USE_X,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY,
	                        USE_Y,USE_XY,USE_XY,USE_XY};
	                        
	static uDialogGroup Items[] =
	
        {
       /*  0 */      {"Ok", {250,270,70,20},uDialogButton,uDialogNone},
       /*  1 */      {"Cancel", {340,270,70,20},uDialogButton,uDialogNone},
       /*  2 */      {"ixmaxScr", {80,29,85,20},uDialogText,uDialogGetLong},
       /*  3 */      {"iymaxSc", {280,29,85,20},uDialogText,uDialogGetLong},
       /*  4 */      {"xmin data", {280,78,110,20},uDialogText,uDialogGetDouble},
       /*  5 */      {"ymindata", {280,103,110,20},uDialogText,uDialogGetDouble},
       /*  6 */      {"xmaxdata", {280,128,110,20},uDialogText,uDialogGetDouble},
       /*  7 */      {"ymaxdata", {280,153,110,20},uDialogText,uDialogGetDouble},
       /*  8 */      {"ixmin data", {80,78,85,20},uDialogText,uDialogGetLong},
       /*  9 */      {"iymindata", {80,103,85,20},uDialogText,uDialogGetLong},
       /* 10 */      {"ixmaxdata", {80,128,85,20},uDialogText,uDialogGetLong},
       /* 11 */      {"iymaxdata", {80,153,85,20},uDialogText,uDialogGetLong},
       /* 12 */      {"Maintain Aspect Ratio", {20,190,177,22},uDialogCheck,uDialogGetLong},
       /* 13 */      {"Receive Domains", {220,190,170,20},uDialogCheck,uDialogGetLong},
       /* 14 */      {"Send Domains", {220,220,110,20},uDialogButton,uDialogNone},
       /* 15 */      {"xmin", {220,78,50,20},uDialogString,uDialogNone},
       /* 16 */      {"ymin", {220,103,50,20},uDialogString,uDialogNone},
       /* 17 */      {"xmax", {220,128,50,20},uDialogString,uDialogNone},
       /* 18 */      {"ymax", {220,153,50,20},uDialogString,uDialogNone},
       /* 19 */      {"ixmin", {20,78,50,20},uDialogString,uDialogNone},
       /* 20 */      {"iymin", {20,104,50,20},uDialogString,uDialogNone},
       /* 21 */      {"ixmax", {20,132,50,20},uDialogString,uDialogNone},
       /* 22 */      {"iymax", {20,153,50,20},uDialogString,uDialogNone},
       /* 23 */      {"xsize", {20,29,50,20},uDialogString,uDialogNone},
       /* 24 */      {"ysize", {220,29,50,20},uDialogString,uDialogNone},
       /* 25 */      {"Image Domain Size", {183,5,130,20},uDialogString,uDialogNone},
       /* 26 */      {"Pixel Domain", {80,55,100,20},uDialogString,uDialogNone},
       /* 27 */      {"Plot Domain", {280,56,100,20},uDialogString,uDialogNone},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
	static uRect Rect1= {100,50,430,310};

    struct FilePIOInfo *pio;
 	uDialogList *uList;
 	uDialogReturn *uR;
    int ret;
    int flag;
	
	if(!Files)return 1;
	pio=&Files->pioData;

	ret=1;
	
	uR=NULL;
	
	uList=uDialogOpen("Domain",&Rect1,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	

	if(uDialogSet(uList,2L,
	
	              uDialogSetDouble,(double)pio->range.ixmax,
	              
	              uDialogSetItem,3L,
	              uDialogSetDouble,(double)pio->range.iymax,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)pio->range.xminData,
	              
	              uDialogSetItem,5L,
	              uDialogSetDouble,(double)pio->range.yminData,
	              
	              uDialogSetItem,6L,
	              uDialogSetDouble,(double)pio->range.xmaxData,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,(double)pio->range.ymaxData,
	              
	              uDialogSetItem,8L,
	              uDialogSetDouble,(double)pio->range.ixminData,
	              
	              uDialogSetItem,9L,
	              uDialogSetDouble,(double)pio->range.iyminData,
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)pio->range.ixmaxData,
	              
	              uDialogSetItem,11L,
	              uDialogSetDouble,(double)pio->range.iymaxData,
	              
	              uDialogSetItem,12L,
	              uDialogSetLogical,pio->range.MaintainAspectRatio,
	              
	              uDialogSetItem,13L,
	              uDialogSetLogical,pio->range.receiveRange,
	              
	              
	              uDialogSetAction,doRange2Action,
	              
	              uDialogSetMyIcon,Files,
	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
    
Start:	

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
    
	if(uDialog(uList))goto ErrorOut;
	
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;
	
	if(uTestRange(uR))goto Start;
	
	pio->range.ixmax=(int)uR[2].lreturn;
	pio->range.iymax=(int)uR[3].lreturn;
	
/*
	if(ScreenSetSize(nw->ixmax,nw->iymax,&myIcon->Screen))goto ErrorOut;
*/
		
	flag=0;
	if(uR[4].dreturn != pio->range.xminData)flag  = 1;
	if(uR[6].dreturn != pio->range.xmaxData)flag += 2;
	if(uR[5].dreturn != pio->range.yminData)flag += 4;
	if(uR[7].dreturn != pio->range.ymaxData)flag += 8;
		
		
	pio->range.xminData=uR[4].dreturn;
	pio->range.yminData=uR[5].dreturn;
	pio->range.xmaxData=uR[6].dreturn;
	pio->range.ymaxData=uR[7].dreturn;
	
	pio->range.ixminData=uR[8].lreturn;
	pio->range.iyminData=uR[9].lreturn;
	pio->range.ixmaxData=uR[10].lreturn;
	pio->range.iymaxData=uR[11].lreturn;

	pio->range.MaintainAspectRatio=(int)uR[12].lreturn;
	pio->range.receiveRange=(int)uR[13].lreturn;
	
    SetRange(Files,pio->range.xminData,pio->range.xmaxData,pio->range.yminData,pio->range.ymaxData,flagout[flag]);

	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	return ret;
}
int doRange2Action(struct uDialogStruct *uList)
{
 	uDialogReturn *uR;
	
	if(!uList)return 1;
	
    uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem ==14){
	
		uR=uDialogGetReturn(uList);
		
	    if(!uR)goto OutOfHere;

 	   SendRange((struct FileInfo2 *)uList->myIconItem,uR);	
	}
OutOfHere:
    if(uR)uDialogFreeReturn(uR);
	if(uList)uDialogUpdate(uList);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return 0;
}
static int SendRange(struct FileInfo2 *Files,uDialogReturn *uR)
{
	struct Message1000 m;

	if(!Files)return 1;

    if(uTestRange(uR))return 1;
		
	m.MaintainAspectRatio=(int)uR[12].lreturn;
	m.ixmax=uR[2].lreturn;
	m.iymax=uR[3].lreturn;
	m.xminData=uR[4].dreturn;
	m.xmaxData=uR[6].dreturn;
	m.yminData=uR[5].dreturn;
	m.ymaxData=uR[7].dreturn;
	m.ixminData=uR[8].lreturn;
	m.ixmaxData=uR[10].lreturn;
	m.iyminData=uR[9].lreturn;
	m.iymaxData=uR[11].lreturn;
	
	SendMessageToAll(NULL,0,MessageType_LIMITS,(void *)&m);
	return 0;
}
static int uTestRange(uDialogReturn *uR)
{
	if(!uR)return 1;
	
	if(uR[2].lreturn < 1){
	    Warning("xsize Must Be Greater Than 1");
	    goto Error;
	}

	if(uR[3].lreturn < 1){
	    Warning("ysize Must Be Greater Than 1");
	    goto Error;
	}


	if(uR[4].dreturn >= uR[6].dreturn){
	    Warning("xmax Must Be Greater Than xmin");
	    goto Error;
	}
	
	if(uR[8].lreturn >= uR[10].lreturn){
	    Warning("ixmax Must Be Greater Than ixmin");
	    goto Error;
	}

	if(uR[5].dreturn >= uR[7].dreturn){
	    Warning("ymax Must Be Greater Than ymin");
	    goto Error;
	}
	
	if(uR[9].lreturn >= uR[11].lreturn){
	    Warning("iymax Must Be Greater Than iymin");
	    goto Error;
	}
	if(uR[10].lreturn > uR[2].lreturn){
	    Warning("xsize Must Be Greater Than or Equal To ixmax");
	    goto Error;
	}

	if(uR[11].lreturn > uR[3].lreturn){
	    Warning("ysize Must Be Greater Than or Equal To iymax");
	    goto Error;
	}

	if(uR[8].lreturn < 0){
	    Warning("ixmin Must Be Greater Than or Equal To 0");
	    goto Error;
	}

	if(uR[9].lreturn < 0){
	    Warning("iymin Must Be Greater Than or Equal To 0");
	    goto Error;
	}
	return 0;
Error:
	return 1;
}

static int doScale2Move(struct uDialogStruct *uList);
static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Action(struct uDialogStruct *uList);
static int doScale2SetButtons(struct uDialogStruct *uList);
int doDataSelectionSDS3D(struct FileInfo2 *Files);

int doDataSelectionSDS3D(struct FileInfo2 *Files)
{
	static uDialogGroup Items[] =
	
        {
      /*  0 */      {"Ok", {10,50,65,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {80,50,65,20},uDialogButton,uDialogNone},
      /*  2 */      {"File List", {20,40,65,65},uDialogScrollList,uDialogNone},
      /*  3 */      {"Range", {315,29,85,20},uDialogButton,uDialogNone},
      /*  4 */      {"dmin", {5,5,65,20},uDialogString,uDialogNone},
      /*  5 */      {"dmax", {180,5,65,20},uDialogString,uDialogNone},
      /*  6 */      {"", {50,5,110,15},uDialogText,uDialogGetDouble},
      /*  7 */      {"", {240,5,110,15},uDialogText,uDialogGetDouble},
        };
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {20,40,400,300};
 	
 	uRect *rectOpen;
 	
 	uDialogList *uList;
    int ret;
    
	if(!Files)return 1;
		
	ret=1;
	
	if(uPref.dataSelectRect.xsize > 100 && uPref.dataSelectRect.ysize > 100){
		rectOpen=&uPref.dataSelectRect;
	}else{
		rectOpen=&Rect1;
	}

	uList=uDialogOpen("Data Selection",rectOpen,ItemCount);
	
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
	
	if(uDialogSet(uList,0L,
	              uDialogSetMyIcon,Files,
	              uDialogSetMove,doScale2Move,
	              uDialogSetStart,doScale2Start,
	              uDialogSetAction,doScale2Action,
	              uDialogSetButtons,doScale2SetButtons,
	              
	              uDialogSetItem,6L,
	              uDialogSetActive,0,
	              
	              uDialogSetItem,7L,
	              uDialogSetActive,0,
	              
	              uDialogSetDone,uDialogSetDone
	))goto ErrorOut;

	if(uDialog(uList))goto ErrorOut;
	
	if(uList && uList->myIcon){
        uRect portRect;
		uGetPortBoundsGlobal(uList->myIcon,&portRect);
		uPref.dataSelectRect=portRect;
	}

	ret=0;
	
ErrorOut:
  
    if(uList)uDialogClose(uList);

	return ret;
}

static int doScale2SetButtons(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	controlPtr Ok;
	controlPtr Range;
	int ret;
	
	
	if(!uList)return 1;
		
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              
              uDialogSetItem,0L,
              uDialogGetControl,&Ok,
              
              uDialogSetItem,3L,
              uDialogGetControl,&Range,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	if(ScrollListIsSelectAny(list2)){
	    uHiliteControl(Ok,0);
	    uHiliteControl(Range,0);
	}else{
	    uHiliteControl(Ok,255);
	    uHiliteControl(Range,255);
	}

	
	return 0;
}

static int doScale2Action(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	struct FileInfo2 *Files;
	struct SetFrameData  *s;
 	double dmin,dmax;
 	double *data;
	long count,nlist,length,n;
	int ret;
	
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	s = &Files->SetData;
	
	uSetCursor(NULL,uWATCH_CURSOR);
	if(uList->actionItem == 3){
	
	
		ret=uDialogSet(uList,2L,
	              uDialogGetScrollList,&list2,
	              	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
	    
		if(ret)return 1;
			
	    dmin =  1e60;
	    dmax = -1e60;

	    count=ScrollListCount(list2);

	    for(nlist=0;nlist<count;++nlist){
	    
		   if(!ScrollListIsSelected(nlist,list2))continue;
		   
		   	if(FilesSetFrame(nlist,Files))goto OutOfHere;
		   	
		   	data=(double *)s->data;
	
			length=s->xsize*s->ysize*s->zsize;
			
			for(n=0;n<length;++n){
	    		if(data[n] > dmax)dmax=data[n];
	    		if(data[n] < dmin)dmin=data[n];
			}
			
			if(Stop("Stop Data Ranging ?"))break;
		
	   }
	   
		ret=uDialogSet(uList,6L,
	              uDialogSetDouble,dmin,
	              
	              uDialogSetItem,7L,
	              uDialogSetDouble,dmax,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    );
		if(ret)goto OutOfHere;
	 
	}
OutOfHere:
	if(uList)uDialogUpdate(uList);
	uSetCursor(NULL,uARROW_CURSOR);
	return 0;
}
static int doScale2Start(struct uDialogStruct *uList)
{
	struct ScrollList *list2;
	struct FileInfo2 *Files;
	int ret;
	int n;
	
	
	if(!uList)return 1;
	Files=(struct FileInfo2 *)uList->myIconItem;
	if(!Files)return 1;
	
	if(!IsOption()){
		uMoveWindow(uList->myIcon,uList->Rect.x,uList->Rect.y);
		uSizeWindowGlobal(uList->myIcon,uList->Rect.xsize,uList->Rect.ysize);
	}
	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&list2,
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	for(n=0;n<Files->ImageCount;++n){
	
 		char *names[1];
		int type=3;

		 names[0]= FilesImageName(Files,n);
		ScrollListInsert(names,&type,n,1L,list2);
	}
	ScrollListSetTop(0,list2);

	return 0;
}
static int doScale2Move(struct uDialogStruct *uList)
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
              uDialogSetLocation,(r.xsize)/2-60,r.ysize-25,
              	              	              
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
	return ret;
}

static int doSDS3DDoDialog(struct FileInfo2 *Files,int *type)
{
	if(!Files || !type)return 1;
	
	if(*type == MessageType_DO_PLANE_DIALOG){
		return doDataPlane(Files);	
	}else if(*type == MessageType_DO_SCALES_DIALOG){
		return doDataScaleSDS3D(Files);	
	}else if(*type == MessageType_DO_RANGE_DIALOG){
		return doDataRange(Files);	
	}else if(*type == MessageType_DO_SELECTION_DIALOG){
		return doDataSelectionSDS3D(Files);	
	}else if(*type == MessageType_DO_AREA_DIALOG){
		return doVolumeSelection(Files);	
	}
	return 0;
}
