#define EXTERN22 extern
#include <math.h>
#include "Xappl.h"
#include "TracerDef.h"
#include "Vrml.h"
#include "uFiles.h"
#include "Render.h"
#include "Defs.h"
#include "uDialog.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Object3d.h"
#include "Universal.h"

static int CStlLoad(CStlPtr s,long itIs);
static CObjectPtr CStlDuplicate(CObject *b);
static int CStlkill(CObject *o);
int CStlMessage(CObject *b,struct ObjectMessage *Message);
static int FilesGetStl(struct Block *b,long CurrentFrame,struct FileInfo2 *Files);

int doPloti(struct Block *b,struct LIMITS *limits,double t,double *data,long imax,long jmax,long kmax);

int doNoShadow4G(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,double *gdata,
                 long nel,long CellCount,struct Scene *scene);
int doNoShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,double *gdata,
                 long nel,long CellCount,struct Scene *scene);
int doNoShadow4(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene);
		
int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene);	

int CPio2dRayDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);

void ScaleMatrix2(struct Matrix *m,struct System *p);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
int Transform(struct P *p,struct Matrix *m);

int CPio2dRayLoad(CPio2dRayPtr s,long itIs);

int doDataDump(IconPtr myIcon,struct FileInfo2 *Files);

void CObjectCreate(struct Scene *scene,CObject *b);
int ListAddObjectList(CObjectList *d,CObject *Object);
int ObjectListAddBlock(struct Scene *scene,long k,long mat);

int CObjectkill(CObject *o);
int blockFix(struct Block *b,long mat,long xsize,long ysize,double xoff,double yoff);
int blockSize(struct Block *b,long mat,long nodes,long cells);
int blockCells(struct Block *b,long mat,long ns,long *nee,long xsize,long ysize);

int MergeBlock(struct Scene *scene,struct Block *block);
int blockFree(struct Block *block);
int SetNormalsBlock(struct Block *block);

int LineCountours(double *x,double *y,double *value,long *material,
                 double *levels,long nx,long ny,long nl,struct cdatal *d);

int CPio2dRayDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);

int SetBlockRS(struct Block *block);

int elementrange(struct Block *block);

int CPio2dExtkill(CObject *o);

int CPio2dRotkill(CObject *o);

CObjectPtr CPio2dRotDuplicate(CObject *b);

int CPio2dRotLoad(CPio2dRotPtr s,long itIs);

int CPio2dRotMessage(CObject *b,struct ObjectMessage *Message);

CObjectPtr CPio2dExtDuplicate(CObject *b);

int CPio2dExtLoad(CPio2dExtPtr s,long itIs);

int CPio2dExtMessage(CObject *b,struct ObjectMessage *Message);



CPio2dRayPtr CPio2dRayCreate(struct Scene *scene,struct FileInfo2 *Files);

int CPio2dRaykill(CObject *o);

int CPio2dRayMessage(CObject *b,struct ObjectMessage *Message);

CObjectPtr CPio2dRayDuplicate(CObject *b);

int CObjectPio2dRayCount(CObject *o,long *nodes,long *cells);

long CObjectPio2dRaySum(CObject *o);


int CObjectPio2dRayMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
		      
		      
		      
CObjectPtr CPio3dLevelDuplicate(CObject *b);
int CPio3dLevelkill(CObject *o);
int CPio3dLevelLoad(CPio3dLevelPtr s,long itIs);
int CPio3dLevelMessage(CObject *b,struct ObjectMessage *Message);

int CPio3dExtkill(CObject *o);
CObjectPtr CPio3dExtDuplicate(CObject *b);
int CPio3dExtLoad(CPio3dExtPtr s,long itIs);
int CPio3dExtMessage(CObject *b,struct ObjectMessage *Message);
static int CPio3dLevelMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
		      
int CPio2dTracerkill(CObject *o);
CObjectPtr CPio2dTracerDuplicate(CObject *b);
int CPio2dTracerMessage(CObject *b,struct ObjectMessage *Message);
int CPio2dTracerLoad(CPio2dTracerPtr s,long itIs);
int CPio2dTracerDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
int CPio2dTracerDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
int CObjectPio2dTracerCount(CObject *o,long *nodes,long *cells);
int CObjectPio2dTracerMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
		      
long CObjectPio2dTracerSum(CObject *o);

long CObjectPio2dTracerSum(CObject *o)
{
	CPio2dTracerPtr s=(CPio2dTracerPtr)o;

	double *x;
	long itIs;
	long length,sum;
	long n;
	
	if(!s)return 0L;

	itIs=s->ItWas;
	if(itIs < 0)itIs=0;

	sum=0;

	x=NULL;
	if(SageGetFileItem(s->Files,"tracer_data",0,&x,&length,itIs)){
	    goto doContinue;
	}
	
	for(n=0;n<s->LevelCount;++n){		
	    sum += length;				
	}
	
doContinue:		
	if(x)cFree((char *)x);
	
	return sum;
	
}


int CObjectPio2dTracerMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CPio2dTracerPtr s=(CPio2dTracerPtr)o;
	double radiusStepFactor,rstep,r;
	long NodeCount;
	long CellCount;
	long sum,nsphere;

	if(!s)return 1;
	
	
	r=s->radius;
    if(r <= 0)r=0.2*s->LengthGreatest;
    radiusStepFactor=s->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    nsphere=(s->LevelCount*2)+(long)(2+s->LengthTotal/rstep);
	
	sum=CObjectPio2dTracerSum(o);

	if(s->lineType){
		NodeCount=s->nx*s->ny*(nsphere);
		CellCount=(s->nx-1)*(s->ny-1)*(nsphere)*5;
	}else{
		NodeCount=2*sum;
		CellCount=3*sum;
	}
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(s->scene->SmoothNormals)*MostNormals=1;
	return 0;
}

int CObjectPio2dTracerCount(CObject *o,long *nodes,long *cells)
{
	CPio2dTracerPtr s=(CPio2dTracerPtr)o;
	double radiusStepFactor,rstep,r;
	long sum,nsphere;

	if(!s)return 1;

	r=s->radius;
    if(r <= 0)r=0.2*s->LengthGreatest;
    radiusStepFactor=s->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    nsphere=(s->LevelCount*2)+(long)(2+s->LengthTotal/rstep);


	sum=CObjectPio2dTracerSum(o);
	
	if(s->lineType){
		*nodes += s->nx*s->ny*(sum+nsphere);
		*cells += (s->nx-1)*(s->ny-1)*(sum+nsphere);
	}else{
		*nodes += 2*sum;
		*cells += sum;
	}
	return 0;
}
CObjectPtr uPio2dTracerWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio2dTracerStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"REAL",4018,universal_TypeLong,(void *)&b.numdim},
		
		{"REAL",4019,universal_TypeLong,(void *)&b.nx},
		{"REAL",4020,universal_TypeLong,(void *)&b.ny},
		
		{"DOUB",4021,universal_TypeDouble,(void *)&b.radius},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.radiusStepFactor},	

		{"REAL",4023,universal_TypeLong,(void *)&b.itemToPlot},

		{"REAL",4024,universal_TypeLong,(void *)&b.lineType},
		
		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio2dTracerPtr bp=(CPio2dTracerPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio2dTracerCreate(l->scene,b.Files,b.numdim);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto ErrorOut;
		    }
		}else{
			goto ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CPio2dTracerPtr CPio2dTracerCreate(struct Scene *scene,struct FileInfo2 *Files,long numdim)
{
	CPio2dTracerPtr s;
	struct Block b;
	char buff[256];
	static int Count;

	if(!scene)return (CPio2dTracerPtr)NULL;

	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CPio2dTracerPtr)NULL;

	s=(CPio2dTracerPtr)cMalloc(sizeof(CPio2dTracer),8566);
	if(!s)return (CPio2dTracerPtr)NULL;
	zerol((char *)s,sizeof(CPio2dTracer));

	sprintf(buff,"PioTracer_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_TRACER_PIO2D;
	s->duplicateObject=CPio2dTracerDuplicate;
	s->countObject=CObjectPio2dTracerCount;
	s->pGetMost=CObjectPio2dTracerMost;
	s->drawObject=CPio2dTracerDrawLines;	
	s->message=CPio2dTracerMessage;		
	s->killObject=CPio2dTracerkill;
	s->ReadWrite=uPio2dTracerWrite;	
	
	s->Files=Files;
	s->vscale=1.0;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	
	s->nx=20;
	s->ny=20;
	
	s->numdim=numdim;
	
	s->radiusStepFactor=0.75;

	s->itemToPlot=2;
		
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CPio2dTracerPtr)NULL;
}
int CPio2dTracerDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio2dTracerPtr c=(CPio2dTracerPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	double vmin,vmax;
	long *ndata,nel;
	long k,ks,kk;
	int i;
	long n,itIs;
	double *x,*y,*z,*v,*e;
	double *namep;
	char *names;
	long length,length1;
	struct FileInfo2 *Files;
	struct FilePIOInfo *pio;
	
	if(!c)return 0;
	scene=c->scene;
	if(!scene)return 0;
	Files=c->Files;
	if(!Files)return 0;

	x=NULL;
	y=NULL;
	z=NULL;
	v=NULL;
	e=NULL;
	names=NULL;
	
	pio = &Files->pioData;
	
	if(c->itemToPlot < 0)c->itemToPlot=0;
	
	if(c->itemToPlot >= pio->tracer_words_per_point){
		sprintf(WarningBuff,"Tracer Item %ld Not Found\n",c->itemToPlot);
		WarningBatch(WarningBuff);
	   	c->itemToPlot=pio->tracer_words_per_point-1;
		sprintf(WarningBuff,"Tracer Item Defaults To %ld\n",c->itemToPlot);
		WarningBatch(WarningBuff);
    }


	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	
	if(flag == 1){
	    if(scene->material[c->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[c->Material].opacity == 1.0)return 0;
	}
    
    c->itemName[0]=0;
    
	if(c->itemToPlot == 0){
	    mstrncpy(c->itemName,"time",sizeof(c->itemName));
	}else if(c->itemToPlot == 1){
	    mstrncpy(c->itemName,"x",sizeof(c->itemName));
	}else if(c->itemToPlot == 2 && pio->numdim > 1){
	    mstrncpy(c->itemName,"y",sizeof(c->itemName));
	}else if(c->itemToPlot == 3 && pio->numdim > 2){
	    mstrncpy(c->itemName,"z",sizeof(c->itemName));
	}else{
		if(SageGetFileItem(c->Files,"tracer_type",0,&namep,&length,itIs))goto ErrorOut;
		names=(char *)namep;
		n=c->itemToPlot-pio->numdim-1;
		if(n < 0)n=0;
		n *= 3;
		if(n + 3 < length*8){
    		c->itemName[0]=names[n];
    		c->itemName[1]=names[n+1];
    		c->itemName[2]=names[n+2];
    		c->itemName[3]=0;
		}
		if(names)cFree((char *)names);
		names=NULL;
	}
			
	if(c->lineType)return CPio2dTracerDraw(o,WorldToScreen,CellCount,flag);	


	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	ndata=scene->cdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
		
	if(SageGetFileItem(c->Files,"tracer_data",0,&v,&length,itIs))goto ErrorOut;
	if(length < 2)goto ErrorOut;

	x=(double *)cMalloc(length*sizeof(double),8266);
	if(!x)goto ErrorOut;
	
	y=(double *)cMalloc(length*sizeof(double),8267);
	if(!y)goto ErrorOut;
		
	z=(double *)cMalloc(length*sizeof(double),8268);
	if(!z)goto ErrorOut;
		
	e=(double *)cMalloc(length*sizeof(double),8269);
	if(!e)goto ErrorOut;
	
	
	vmin =  1e60;
	vmax = -1e60;
	
    k=0;
    ks=0;
	nel=0;
	for(n=0;n<c->LevelCount;++n){
	    int nv,nl;
	
		nl=(int)c->Levels[n];
		
		if(nl >= pio->tracer_num_pnts){
		     sprintf(WarningBuff,"Tracer Point %d Item %ld Not Found\n",nl,c->itemToPlot);
		     WarningBatch(WarningBuff);
		     continue;
		}
				
	    nv  = nl*pio->tracer_words_per_point;
	    	    
	    length1=0;
	    for(kk=0;kk<length;kk+=pio->tracer_words_per_record){
	    	e[length1]=v[c->itemToPlot+nv+kk];
	    	x[length1]=v[1+nv+kk];
	    	if(c->numdim > 1){
	    		y[length1]=v[2+nv+kk];
	    		if(c->numdim > 2){
	    			z[length1]=v[3+nv+kk];
	    		}else{
	    			z[length1]=0;
	    		}
	    	}else{
	    		y[length1]=0;
	    		z[length1]=0;
	    	}
	    	length1++;
	    }
	
	
		if(length1 < 2)continue;
				
		for(kk=0;kk<length1-1;++kk){
		    double xl,s;
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;

			s=0;

			for(i=0;i<2;++i){ 
			   double vv;
			   dx=x[kk]+s*(x[kk+1]-x[kk])+c->origin.x;
			   dy=y[kk]+s*(y[kk+1]-y[kk])+c->origin.y;
			   dz=z[kk]+s*(z[kk+1]-z[kk])+c->origin.z;
			   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
			   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
			   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
			   vv=e[kk];
			   vdata[k]=vv;
			   if(vv < vmin)vmin=vv;
			   if(vv > vmax)vmax=vv;
			   ++k;
			   s=1.0;
			}
			    
		    ndata[nel++]=c->Material;
		    ndata[nel++]=ks;
		    ndata[nel++]=ks+1;
			    
		    ks=k;
			    			    
		}
	}
	    	    
	nel /= 3;
	    
	checkPaletteLevels(scene,c->Material,vmin,vmax);

	doNoShadowLines(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);

	*CellCount += nel;
	
    c->LastCell = *CellCount;
    
ErrorOut:

	if(x)cFree((char *)x);
	if(y)cFree((char *)y);
	if(z)cFree((char *)z);
	if(v)cFree((char *)v);
	if(e)cFree((char *)e);
	if(names)cFree((char *)names);
		
	return 0;
}
int CPio2dTracerDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio2dTracerPtr c=(CPio2dTracerPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k,ks,kk;
	Vec Lampl,Eyel;
	int i,j;
	long n,itIs;
	double *x,*y,*z,*e,*v;
	long length1,nsphere,length;
	double r,rstep,sum,radiusStepFactor;
	struct FileInfo2 *Files;
	struct FilePIOInfo *pio;

	if(!c)return 0;	
	scene=c->scene;
	if(!scene)return 0;
	Files=c->Files;
	if(!Files)return 0;	
	pio = &Files->pioData;

	if(flag == 1){
	    if(scene->material[c->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[c->Material].opacity == 1.0)return 0;
	}

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	gdata=scene->gdata;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
    r=c->radius;
    if(r <= 0)r=0.2*c->LengthGreatest;
    radiusStepFactor=c->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);


	e=NULL;
	x=NULL;
	y=NULL;
	z=NULL;
	v=NULL;

	if(SageGetFileItem(Files,"tracer_data",0,&v,&length,itIs))goto ErrorOut;
	if(length < 2)goto ErrorOut;

	x=(double *)cMalloc(length*sizeof(double),8261);
	if(!x)goto ErrorOut;
	
	y=(double *)cMalloc(length*sizeof(double),8262);
	if(!y)goto ErrorOut;
		
	z=(double *)cMalloc(length*sizeof(double),8262);
	if(!z)goto ErrorOut;
		
	e=(double *)cMalloc(length*sizeof(double),8262);
	if(!e)goto ErrorOut;
    
        
    k=0;
    ks=0;
	nel=0;
	nsphere=0;
	for(n=0;n<c->LevelCount;++n){
	    int nv,nl;
	
		nl=(int)c->Levels[n];
		
		if(nl >= pio->tracer_num_pnts){
		     sprintf(WarningBuff,"Tracer Point %d Item %ld Not Found\n",nl,c->itemToPlot);
		     WarningBatch(WarningBuff);
		     continue;
		}
				
	    nv  = nl*pio->tracer_words_per_point;
	    	    
	    length1=0;
	    for(kk=0;kk<length;kk+=pio->tracer_words_per_record){
	    	e[length1]=v[c->itemToPlot+nv+kk];
	    	x[length1]=v[1+nv+kk];
	    	if(c->numdim > 1){
	    		y[length1]=v[2+nv+kk];
	    		if(c->numdim > 2){
	    			z[length1]=v[3+nv+kk];
	    		}else{
	    			z[length1]=0;
	    		}
	    	}else{
	    		y[length1]=0;
	    		z[length1]=0;
	    	}
	    	length1++;
	    }
	
	
	
		if(length1 < 2)continue;
		
		
		sum=0;		    
		for(kk=0;kk<length1-1;++kk){
		    double xl,s,delta;
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;
		    
		    if(sum+xl < rstep){
		        sum += 	xl;	
		        continue;
		   	} 
		   			   			   	
		   	delta=rstep-sum;
		   	
		    do {
		    
	           if((k+c->ny*c->nx) > scene->MostNodes){
	               sprintf(WarningBuff,"CPio2dTracerDraw Skipping Memory Used %ld Avaiable %ld\n",
	                       k,scene->MostNodes);
	               WarningBatch(WarningBuff);
	               sprintf(WarningBuff,"delta %g rstep %g xl %g\n",delta,rstep,xl);
	               WarningBatch(WarningBuff);
	               goto OutOfHere;
	           }
	           
		        ++nsphere;
		        		    		   	   
		   	    s=delta/xl;
		   	    
		   	    
			    for(j=0;j<c->ny;++j){
			        double sinf,cosf,vv;
			        sinf=sin(pi*(1.-j*dumy));
			        cosf=cos(pi*(1.-j*dumy));
			        for(i=0;i<c->nx;++i){
			           dx=x[kk]+s*(x[kk+1]-x[kk])+c->origin.x+r*cos(2*pi*(1.-i*dumx))*sinf;
			           dy=y[kk]+s*(y[kk+1]-y[kk])+c->origin.y+r*sin(2*pi*(1.-i*dumx))*sinf;
			           dz=z[kk]+s*(z[kk+1]-z[kk])+c->origin.z+r*cosf;
			           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
			           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
			           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
					   vv=e[kk];
					   vdata[k]=vv;
			           ++k;
			        }
			    }			    
			    
			    for(j=0;j<c->ny-1;++j){
			        for(i=0;i<c->nx-1;++i){
					    ndata[nel++]=c->Material;
					    ndata[nel++]=i+j*c->nx+ks;
					    ndata[nel++]=i+1+j*c->nx+ks;
					    ndata[nel++]=i+1+(j+1)*c->nx+ks;
					    ndata[nel++]=i+(j+1)*c->nx+ks;
			        }
			    }
			    
			    ks=k;			    			    
		   		delta += rstep;
			    
		    }while(delta < xl);
		    
		    sum=(1-s)*xl;
		}
	}
	
OutOfHere:
	    	    
	nel /= 5;
	    

	if(gdata){
	    k=0;
		for(n=0;n<nsphere;++n){
		    for(j=0;j<c->ny;++j){
		        double sinf,cosf;
		        sinf=sin(pi*(1.-j*dumy));
		        cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){
			        Vec P,N,D,L;
			        double len;
			        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
			        
				MakeVector(xdatal[k],ydatal[k],zdatal[k],P);
					
				MakeVector((dx)*v1x+(dy)*v1y+(dz)*v1z,(dx)*v2x+(dy)*v2y+(dz)*v2z,
					           (dx)*v3x+(dy)*v3y+(dz)*v3z,N);
					
				/* Vector Eye To Point */
					
		    		VecSub(P,Eyel,D);
		    		
				/* Check Direction To Eye Point */

				if((VecDot(D,N)) > 0.0){
				     VecNegate(N);
				}


				/*  Vector from Point to Lamp */

				VecSub(Lampl,P,L);
					
				/* Check Direction of Lamp */


				 if((VecDot(L,N)) > 0.0){
				     if((len=VecLen(N)) > 0.0){
				         N[0] /= len;
				         N[1] /= len;
				         N[2] /= len;
				     }
				     if((len=VecLen(L)) > 0.0){
				         L[0] /= len;
				         L[1] /= len;
				         L[2] /= len;
				     }
				     gdata[k]=VecDot(N,L);
				 }else{
				     gdata[k]=0.;
				 }
				 ++k;
			    }
		    }
		}
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	}else if(nvdata){
	    k=0;
		for(n=0;n<nsphere;++n){
	        for(j=0;j<c->ny;++j){
	            double sinf,cosf;
	            sinf=sin(pi*(1.-j*dumy));
	            cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){		        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
					 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
					 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
					 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			    }
	    	}
	    }
	    if(rdata && sdata){
		    dx=0;
		    if(c->nx > 1)dx=1.0/((double)(c->nx-1));

		    dy=0;
		    if(c->ny > 1)dy=1.0/((double)(c->ny-1));

    	    k=0;
			for(n=0;n<nsphere;++n){
	            for(j=0;j<c->ny;++j){
			        for(i=0;i<c->nx;++i){
					    rdata[k]=(float)((i)*dx);
					    sdata[k++]=(float)((j)*dy);
			        }
		    	}
	    	}
	    }
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}
	*CellCount += nel;
    c->LastCell = *CellCount;
ErrorOut:
	if(x)cFree((char *)x);
	x=NULL;		
	if(y)cFree((char *)y);
	y=NULL;		
	if(z)cFree((char *)z);
	z=NULL;		
	if(e)cFree((char *)e);
	e=NULL;		
	if(v)cFree((char *)v);
	v=NULL;		
	return 0;
}
int CPio2dTracerLoad(CPio2dTracerPtr s,long itIs)
{
	struct FileInfo2 *Files;
	struct FilePIOInfo *pio;
	struct Scene *scene;
	struct PIO *sage;
	long n;
	double *x,*y,*z,*v,*e,len,sum,ll;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double vmin,vmax;
	long length,length1;
	int k;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	Files=s->Files;
	if(!Files)return 1;
	pio = &Files->pioData;
	
	if((s->LevelCount <= 0) || !s->Levels)return 0;
	if(s->ItWas == itIs)return 1;

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    return 0;
	}

	if(s->ItWas == itIs)return 1;
	
	if(itIs < 0)itIs=0;
	if(itIs >= Files->ImageCount)itIs=0;

	x=NULL;
	y=NULL;
	z=NULL;
	v=NULL;
	e=NULL;
	
	xmin=1e60;
	ymin=1e60;
	zmin=1e60;
	vmin=1e60;
	xmax=-1e60;
	ymax=-1e60;
	zmax=-1e60;
	vmax=-1e60;
	
		
	if(SageGetFileItem(Files,"tracer_data",0,&v,&length,itIs))goto ErrorOut;
	if(length < 2)goto ErrorOut;
	
	sage = &s->Files->PIO2DList[itIs];
	
	pio->tracer_num_pnts=sage->tracer_num_pnts;
	pio->tracer_num_vars=sage->tracer_num_vars;
	pio->tracer_words_per_record=sage->tracer_words_per_record;
	pio->tracer_words_per_point=sage->tracer_words_per_point;
	pio->numdim=sage->numdim;

	
	x=(double *)cMalloc(length*sizeof(double),9261);
	if(!x)goto ErrorOut;
	
	y=(double *)cMalloc(length*sizeof(double),9262);
	if(!y)goto ErrorOut;
			
	z=(double *)cMalloc(length*sizeof(double),9262);
	if(!z)goto ErrorOut;
			
	e=(double *)cMalloc(length*sizeof(double),9262);
	if(!e)goto ErrorOut;
			
	sum = 0;
	len = 0;
	for(n=0;n<s->LevelCount;++n){
	    int nv,nl;
	
		nl=(int)s->Levels[n];
		
		if(nl >= pio->tracer_num_pnts){
		     continue;
		}
				
	    nv  = nl*pio->tracer_words_per_point;
	    	    
	    length1=0;
	    for(k=0;k<length;k+=pio->tracer_words_per_record){
	    	double vv;
	    	vv=v[s->itemToPlot+nv+k];
	    	e[length1]=vv;
			x[length1  ]=v[1+nv+k];
			if(s->numdim > 1){
				y[length1]=v[2+nv+k];
				if(s->numdim > 2){
					z[length1]=v[3+nv+k];
				}else{
					z[length1]=0;
				}
			}else{
				y[length1]=0;
				z[length1]=0;
			}
			if(x[length1] < xmin)xmin=x[length1];
			if(x[length1] > xmax)xmax=x[length1];
			if(y[length1] < ymin)ymin=y[length1];
			if(y[length1] > ymax)ymax=y[length1];
			if(z[length1] < zmin)zmin=z[length1];
			if(z[length1] > zmax)zmax=z[length1];
			if(vv < vmin)vmin=vv;
			if(vv > vmax)vmax=vv;
			length1++;
	    }
		if(length1 < 2)continue;

		for(k=1;k<length1;++k){
		    ll=sqrt((x[k-1]-x[k])*(x[k-1]-x[k])+(y[k-1]-y[k])*(y[k-1]-y[k])+
		            (z[k-1]-z[k])*(z[k-1]-z[k]));
		    sum += ll;
		    if((ll > len))len = ll;
		}	

	}
	
	checkPaletteLevels(scene,s->Material,vmin,vmax);	
	
	s->LengthTotal=sum;
	s->LengthGreatest=len;
	
	
	s->xminData=xmin;
	s->yminData=ymin;
	s->zminData=zmin;
	s->xmaxData=xmax;
	s->ymaxData=ymax;
	s->ymaxData=ymax;
	
	s->setData=TRUE;

	s->ItWas = itIs;
	
	
ErrorOut:
	if(x)cFree((char *)x);
	x=NULL;		
	if(y)cFree((char *)y);
	y=NULL;		
	if(z)cFree((char *)z);
	z=NULL;		
	if(v)cFree((char *)v);
	v=NULL;		
	if(e)cFree((char *)e);
	e=NULL;		

	return 0;
}
int CPio2dTracerMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio2dTracerPtr s=(CPio2dTracerPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio2dTracerLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Tracer File\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}else if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    double xdata[8],ydata[8],zdata[8];
		struct Matrix GroupToWorld;
		struct Matrix GroupToScreen;
	    struct P p;
	    long n;
	    
	    if(!s->setData)return 0;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);
	    
	    xdata[0]=s->xminData;
	    xdata[1]=s->xmaxData;
	    xdata[2]=s->xmaxData;
	    xdata[3]=s->xminData;
	    xdata[4]=s->xminData;
	    xdata[5]=s->xmaxData;
	    xdata[6]=s->xmaxData;
	    xdata[7]=s->xminData;

	    ydata[0]=s->yminData;
	    ydata[1]=s->yminData;
	    ydata[2]=s->ymaxData;
	    ydata[3]=s->ymaxData;
	    ydata[4]=s->yminData;
	    ydata[5]=s->yminData;
	    ydata[6]=s->ymaxData;
	    ydata[7]=s->ymaxData;

	    zdata[0]=s->zminData;
	    zdata[1]=s->zminData;
	    zdata[2]=s->zminData;
	    zdata[3]=s->zminData;
	    zdata[4]=s->zmaxData;
	    zdata[5]=s->zmaxData;
	    zdata[6]=s->zmaxData;
	    zdata[7]=s->zmaxData;


	    for(n=0;n<8;++n){
	        p.x=xdata[n];
	        p.y=ydata[n];
	        p.z=zdata[n];
	        Transform(&p,&GroupToScreen);
	        if(p.x > Message->xmax)Message->xmax=p.x;
	        if(p.y > Message->ymax)Message->ymax=p.y;
	        if(p.z > Message->zmax)Message->zmax=p.z;
	        if(p.x < Message->xmin)Message->xmin=p.x;
	        if(p.y < Message->ymin)Message->ymin=p.y;
	        if(p.z < Message->zmin)Message->zmin=p.z;
	    }
	}
	return 0;
}
CObjectPtr CPio2dTracerDuplicate(CObject *b)
{
	CPio2dTracerPtr s=(CPio2dTracerPtr)b;
	CPio2dTracerPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio2dTracerPtr)cMalloc(sizeof(CPio2dTracer),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}

	return (CObjectPtr)sdsNew;
}
int CPio2dTracerkill(CObject *o)
{
	CPio2dTracerPtr s=(CPio2dTracerPtr)o;
	if(!s)return 1;
	
	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}
	cFree((char *)s);
	return 0;
}
CObjectPtr uPio3dLevelWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio3dLevelStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"REAL",5002,universal_TypeDoubleLongPtr,(void *)&b.blockNumbers,0L},				
		{"DOUB",5003,universal_TypeLong,(void *)&b.blockCount},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		

		{"REAL",4020,universal_TypeInt,(void *)&b.pa.DrawZones},
		{"REAL",4021,universal_TypeInt,(void *)&b.pa.ZoneLevel},
		{"REAL",4022,universal_TypeInt,(void *)&b.pa.DrawZonesColor},
		{"REAL",4023,universal_TypeInt,(void *)&b.pa.flagGradients},
		

		{"DOUB",5021,universal_TypeDouble,(void *)&b.sd.plane.Plane_xmin},	
		{"DOUB",5022,universal_TypeDouble,(void *)&b.sd.plane.Plane_ymin},	
		{"DOUB",5023,universal_TypeDouble,(void *)&b.sd.plane.Plane_zmin},	
		
		{"DOUB",5024,universal_TypeDouble,(void *)&b.sd.plane.Plane_xmax},	
		{"DOUB",5025,universal_TypeDouble,(void *)&b.sd.plane.Plane_ymax},	
		{"DOUB",5026,universal_TypeDouble,(void *)&b.sd.plane.Plane_zmax},	
		

		{"DOUB",5033,universal_TypeLong,(void *)&b.sd.plane.Plane_xcount},	
		{"DOUB",5034,universal_TypeLong,(void *)&b.sd.plane.Plane_ycount},	
		{"DOUB",5035,universal_TypeLong,(void *)&b.sd.plane.Plane_zcount},			

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},
		
		{"REAL",4028,universal_TypeInt,(void *)&b.pioIndex},	
		{"STRI",4029,universal_TypeString,(void *)&b.pioName[0],sizeof(b.pioName)},
		
		
		{"REAL",4030,universal_TypeInt,(void *)&b.pioIndexPaint},	
		{"STRI",4031,universal_TypeString,(void *)&b.pioNamePaint[0],sizeof(b.pioNamePaint)},
		
		
		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio3dLevelPtr bp=(CPio3dLevelPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    Local[2].space=0;
		zerol((char *)&b,sizeof(b));
		b.pa.ZoneLevel=1;
		b.BlockNumber = -1;
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio3dLevelCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto ErrorOut;
		    }
		}else{
			goto ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		bp->blockNumbers=b.blockNumbers;
		b.blockNumbers=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
	    Local[2].space=b.blockCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
static int CPio3dLevelMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CPio3dLevelPtr s=(CPio3dLevelPtr)o;
	long NodeCount,CellCount;
	struct Scene *scene;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 0;
	if(s->Material < 0 || s->Material >= scene->MaterialUsed)return 1;

	NodeCount=0;
	CellCount=0;
		
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(s->scene->SmoothNormals)*MostNormals=1;
	return 0;
}
CPio3dLevelPtr CPio3dLevelCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	struct Block b;
	CPio3dLevelPtr s;
	char buff[256];
	static int Count;
	int type;

	if(!scene || !Files)return (CPio3dLevelPtr)NULL;


	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CPio3dLevelPtr)NULL;

	s=(CPio3dLevelPtr)cMalloc(sizeof(CPio3dLevel),8569);
	if(!s)return (CPio3dLevelPtr)NULL;
	zerol((char *)s,sizeof(CPio3dLevel));

	sprintf(buff,"Pio3dLevel_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
        	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
	
	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_LEVEL_PIO3D;
	s->Files=Files;
		
	s->duplicateObject=CPio3dLevelDuplicate;
	s->message=CPio3dLevelMessage;
	s->killObject=CPio3dLevelkill;
	s->message=CPio3dLevelMessage;
	s->ReadWrite=uPio3dLevelWrite;
	s->pGetMost=CPio3dLevelMost;
	
	
	
	s->BlockNumber = -1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CPio3dLevelPtr)NULL; 
}
int CPio3dLevelMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio3dLevelPtr s=(CPio3dLevelPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio3dLevelLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Sage3d Level Surface Files\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}
	return 0;
}
static int CPio3dGridDraw(void *info,double *x,double *y,double *z,double *v,long ncell)
{
	CPio3dLevelPtr s=(CPio3dLevelPtr)info;
	struct Scene *scene;
	struct Block *bp;
	struct Block b;
	long *Elements;
	double *vdata,vmin,vmax;
	long n;
	
	if(!s || !x || !y || !z || !v || (ncell <= 0))return 1;
	scene=s->scene;
	if(!scene || !scene->block)return 1;
	
	if(s->BlockNumber < 0){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return 1;	
		s->BlockNumber=scene->BlockCount-1;
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return 1;
	}
	
	bp = &(scene->block[s->BlockNumber]);

	blockFree(bp);

	vdata=(double *)cMalloc(ncell*4*sizeof(double),7876);
	Elements=(long *)cMalloc(ncell*(4+1)*sizeof(double),7876);
	if(!vdata || !Elements){
	   if(vdata)cFree((char *)vdata);
	   if(Elements)cFree((char *)Elements);
	   return 1;
	}
	
    bp->xdata=x;
    bp->ydata=y;
    bp->zdata=z;
    bp->vdata=vdata;
    bp->Elements=Elements;
    bp->NodeCount = ncell*4;
    bp->ElementCount = ncell;
	bp->NodesMax=4;
	bp->BlockType=1;
	
	vmin =  1e60;
	vmax = -1e60;
	for(n=0;n<ncell;++n){
		double vv;
	    Elements[n*5]=s->Material;
	    Elements[n*5+1]=4*n;
	    Elements[n*5+2]=4*n+1;
	    Elements[n*5+3]=4*n+2;
	    Elements[n*5+4]=4*n+3;
	    vv=v[n];
	    vdata[4*n]=vv;
	    vdata[4*n+1]=vv;
	    vdata[4*n+2]=vv;
	    vdata[4*n+3]=vv;
		if(vv < vmin)vmin=vv;
		if(vv > vmax)vmax=vv;
	}

	elementrange(bp);

	checkPaletteLevels(scene,s->Material,vmin,vmax);
	
	bp->pa=s->pa;
	
	cFree((char *)v);
	return 0;
}
static int CPio3dGridLoad(CPio3dLevelPtr s,long itIs)
{
	struct Scene *scene;
	struct areadata ai;
	struct Material *Mat;
	struct Block *bp;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if(s->BlockNumber >= 0){
		bp = &(scene->block[s->BlockNumber]);
		blockFree(bp);
	}
	
	if(!s->pa.DrawZones)return 1;
	
	Mat=NULL;
	if(SetMaterialProperty(scene,&Mat,s->Material))return 1;
	if(!Mat)return 1;

	zerol((char *)&ai,sizeof(struct areadata));
		
	ai.uDraw=CPio3dGridDraw;
	ai.info=s;
		
    if(s->usePaint){
		ai.pioIndex=s->pioIndexPaint;	
	    if(s->pioNamePaint[0]){
	        mstrncpy(ai.pioName,s->pioNamePaint,64);
	    }else{
	        mstrncpy(ai.pioName,"density",64);
	    }
	}else{	
    	mstrncpy(ai.pioName,s->pioName,64);
		ai.pioIndex=s->pioIndex;	
	}	
		
	ai.pd=Mat->pd;
	ai.pa=s->pa;
	
	ai.xminArea=s->sd.plane.Plane_xmin;
	ai.yminArea=s->sd.plane.Plane_ymin;
	ai.zminArea=s->sd.plane.Plane_zmin;
	
	ai.xmaxArea=s->sd.plane.Plane_xmax;
	ai.ymaxArea=s->sd.plane.Plane_ymax;
	ai.zmaxArea=s->sd.plane.Plane_zmax;
	
	ai.xsize=s->sd.plane.Plane_xcount;
	ai.ysize=s->sd.plane.Plane_ycount;
	ai.zsize=s->sd.plane.Plane_zcount;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_GRID_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CPio3dLevelLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

ErrorOut:	
	return 0;
}
int CPio3dLevelLoad(CPio3dLevelPtr s,long itIs)
{
	struct Scene *scene;
	struct areadata ai;
	struct LIMITS limits;
	double vmin,vmax;
	double *data;
	long n,k;
	int iret;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if(s->ItWas == itIs){
	    iret=1;
		for(n=0;n<s->blockCount;++n){
		    if(s->blockNumbers){
				if(scene->block[s->blockNumbers[n]].ItWas != itIs){
	    			iret=0;
	    			break;
				}
		    }
		}
	    if(iret)return 1;
	    s->ItWas = -1;
	}

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    return 0;
	}

	if(s->ItWas == itIs)return 1;
		
	CPio3dGridLoad(s,itIs);

	if((s->LevelCount <= 0) && !s->Levels)return 0;
	
	data=NULL;

	for(n=0;n<s->blockCount;++n){
	    if(s->blockNumbers){
			blockFree(&scene->block[s->blockNumbers[n]]);
	    }
	}

	if(s->LevelCount > s->blockCount){
	   long *blockNumbers;
	   blockNumbers=(long *)cMalloc(s->LevelCount*sizeof(long),8573);
	   if(!blockNumbers){
	       	sprintf(WarningBuff,"CPio3dLevelLoad out of Memory\n");
			WarningBatch(WarningBuff);
	       	return 1;
	   }
	   zerol((char *)blockNumbers,s->LevelCount*sizeof(long));

	   for(n=0;n<s->blockCount;++n){
	       if(s->blockNumbers)blockNumbers[n]=s->blockNumbers[n];
	   }

	   if(s->blockNumbers)cFree((char *)s->blockNumbers);
	   s->blockNumbers=blockNumbers;

		for(n=s->blockCount;n<s->LevelCount;++n){
			struct Block bb;
			zerol((char *)&bb,sizeof(struct Block));
			if(MergeBlock(scene,&bb))return 1;
			s->blockNumbers[n]=scene->BlockCount-1;
			if(ObjectListAddBlock(scene,s->blockNumbers[n],s->Material))return 1;
		}
		s->blockCount=s->LevelCount;
	}
	

	zerol((char *)&ai,sizeof(struct areadata));
		
	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
	
	ai.xminArea=s->sd.plane.Plane_xmin;
	ai.yminArea=s->sd.plane.Plane_ymin;
	ai.zminArea=s->sd.plane.Plane_zmin;
	
	ai.xmaxArea=s->sd.plane.Plane_xmax;
	ai.ymaxArea=s->sd.plane.Plane_ymax;
	ai.zmaxArea=s->sd.plane.Plane_zmax;
	
	ai.xsize=s->sd.plane.Plane_xcount;
	ai.ysize=s->sd.plane.Plane_ycount;
	ai.zsize=s->sd.plane.Plane_zcount;
	ai.pa=s->pa;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CPio3dLevelLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	
	data=ai.data;
	
	zerol((char *)&limits,sizeof(limits));
	
	limits.iGotData=63;
	
	limits.xmin=ai.xminArea;
	limits.ymin=ai.yminArea;
	limits.zmin=ai.zminArea;
	
	limits.xmax=ai.xmaxArea;
	limits.ymax=ai.ymaxArea;
	limits.zmax=ai.zmaxArea;
		
	vmin =  1e60;
	vmax = -1e60;
	for(n=0;n<s->LevelCount;++n){
	    struct Block *bp;
	    bp = &(scene->block[s->blockNumbers[n]]);
	    if(doPloti(bp,&limits,s->Levels[n],(double *)ai.data,ai.xsize,
	               ai.ysize,ai.zsize)){
	        break;
	    }	  
	    
	    bp->ItWas = itIs;
	    
	    if(bp->Elements){
	        for(k=0;k<bp->ElementCount;++k){
	           bp->Elements[k]=s->Material;
	           k += bp->NodesMax;
	        } 
	    }

	    if(scene->SmoothNormals){
	        SetNormalsBlock(bp);
	    }
	    if(s->usePaint){
		    struct linedata li;
			zerol((char *)&li,sizeof(struct linedata));
		    li.pioIndex=s->pioIndexPaint;
		    if(s->pioNamePaint[0]){
		        mstrncpy(li.pioName,s->pioNamePaint,64);
		    }else{
		        mstrncpy(li.pioName,"density",64);
		    }
			li.x=bp->xdata;
			li.y=bp->ydata;
			li.z=bp->zdata;
			li.v=bp->vdata;
			li.PointLineCount=(int)bp->NodeCount;
			li.type=LINEDATA_LIST_DATA;
			li.CurrentFrame=itIs;

			if(SendMessageByName(s->BatchName,MessageType_GET_LINE_DATA,&li)){
				sprintf(WarningBuff,"CPio3dLevelLoad -  pioName (%s) Error Getting Paint Data\n",li.pioName);
				WarningBatch(WarningBuff);
				continue;
			}	  

	    }
		
	    elementrange(bp);
	    
	    if(bp->vmin < vmin)vmin=bp->vmin;
	    if(bp->vmax > vmax)vmax=bp->vmax;
	}

	checkPaletteLevels(scene,s->Material,vmin,vmax);

	s->ItWas = itIs;
	
ErrorOut:

	if(data)cFree((char *)data);

	return 0;
}
int CPio3dLevelkill(CObject *o)
{
	CPio3dLevelPtr s=(CPio3dLevelPtr)o;
	if(!s)return 1;

	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->blockNumbers)cFree((char *)s->blockNumbers);
	s->blockNumbers=NULL;
	s->blockCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}

	cFree((char *)s);
	return 0;
}
CObjectPtr CPio3dLevelDuplicate(CObject *b)
{
	CPio3dLevelPtr s=(CPio3dLevelPtr)b;
	CPio3dLevelPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio3dLevelPtr)cMalloc(sizeof(CPio3dLevel),8567);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;
	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}
	
	if(s->blockNumbers && s->blockCount){
	   sdsNew->blockNumbers=(long *)cMalloc(sizeof(long)*s->blockCount,8572);
	   if(!sdsNew->blockNumbers){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->blockCount;++n){
	       sdsNew->blockNumbers[n]=s->blockNumbers[n];
	   }
	}else{
	    sdsNew->blockNumbers=NULL;
	}
	
	return (CObjectPtr)sdsNew;
}

long CObjectPio2dRaySum(CObject *o)
{
	CPio2dRayPtr s=(CPio2dRayPtr)o;

	char name[256];
	double *x;
	long itIs;
	long length,sum;
	long n;
	
	if(!s)return 0L;

	itIs=s->ItWas;
	if(itIs < 0)itIs=0;
	
	sum=0;
	for(n=0;n<s->LevelCount;++n){
		sprintf(name,"laser_plot_x(%ld)%c",(long)s->Levels[n],0);
		
		x=NULL;
		if(SageGetFileItem(s->Files,name,0,&x,&length,itIs)){
		    goto doContinue;
		}
	    sum += length;
				
doContinue:		
		if(x)cFree((char *)x);
	}
	return sum;
	
}


int CObjectPio2dRayMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CPio2dRayPtr s=(CPio2dRayPtr)o;
	double radiusStepFactor,rstep,r;
	long NodeCount;
	long CellCount;
	long sum,nsphere;

	if(!s)return 1;
	
	
	r=s->radius;
    if(r <= 0)r=0.2*s->LengthGreatest;
    radiusStepFactor=s->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    nsphere=(s->LevelCount*2)+(long)(2+s->LengthTotal/rstep);
	
	sum=CObjectPio2dRaySum(o);

	if(s->lineType){
		NodeCount=s->nx*s->ny*(nsphere);
		CellCount=(s->nx-1)*(s->ny-1)*(nsphere)*5;
	}else{
		NodeCount=2*sum;
		CellCount=3*sum;
	}
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(s->scene->SmoothNormals)*MostNormals=1;
	return 0;
}

int CObjectPio2dRayCount(CObject *o,long *nodes,long *cells)
{
	CPio2dRayPtr s=(CPio2dRayPtr)o;
	double radiusStepFactor,rstep,r;
	long sum,nsphere;

	if(!s)return 1;

	r=s->radius;
    if(r <= 0)r=0.2*s->LengthGreatest;
    radiusStepFactor=s->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    nsphere=(s->LevelCount*2)+(long)(2+s->LengthTotal/rstep);


	sum=CObjectPio2dRaySum(o);
	
	if(s->lineType){
		*nodes += s->nx*s->ny*(sum+nsphere);
		*cells += (s->nx-1)*(s->ny-1)*(sum+nsphere);
	}else{
		*nodes += 2*sum;
		*cells += sum;
	}
	return 0;
}
CObjectPtr uPio2dRayWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio2dRayStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"REAL",4018,universal_TypeLong,(void *)&b.numdim},
		
		{"REAL",4019,universal_TypeLong,(void *)&b.nx},
		{"REAL",4020,universal_TypeLong,(void *)&b.ny},
		
		{"DOUB",4021,universal_TypeDouble,(void *)&b.radius},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.radiusStepFactor},	

		{"REAL",4023,universal_TypeLong,(void *)&b.itemToPlot},

		{"REAL",4024,universal_TypeLong,(void *)&b.lineType},
		
		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio2dRayPtr bp=(CPio2dRayPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio2dRayCreate(l->scene,b.Files);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto ErrorOut;
		    }
		}else{
			goto ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CPio2dRayPtr CPio2dRayCreate(struct Scene *scene,struct FileInfo2 *Files)
{
	CPio2dRayPtr s;
	struct Block b;
	char buff[256];
	static int Count;

	if(!scene)return (CPio2dRayPtr)NULL;

	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CPio2dRayPtr)NULL;

	s=(CPio2dRayPtr)cMalloc(sizeof(CPio2dRay),8566);
	if(!s)return (CPio2dRayPtr)NULL;
	zerol((char *)s,sizeof(CPio2dRay));

	sprintf(buff,"Pio2dRay_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_RAY_PIO2D;
	s->duplicateObject=CPio2dRayDuplicate;
	s->countObject=CObjectPio2dRayCount;
	s->pGetMost=CObjectPio2dRayMost;
	s->drawObject=CPio2dRayDrawLines;	
	s->message=CPio2dRayMessage;		
	s->killObject=CPio2dRaykill;
	s->ReadWrite=uPio2dRayWrite;			
	
	s->Files=Files;
	s->vscale=1.0;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	
	s->nx=20;
	s->ny=20;
	
	s->radiusStepFactor=0.75;
	
		
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CPio2dRayPtr)NULL;
}
int CPio2dRayDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio2dRayPtr c=(CPio2dRayPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double vmin,vmax;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	long *ndata,nel;
	long k,ks,kk;
	char name[256];
	int i;
	long n,itIs;
	double *x,*y,*z,*e;
	long length1,length2;
	
	if(c->lineType)return CPio2dRayDraw(o,WorldToScreen,CellCount,flag);

	scene=c->scene;

	if(flag == 1){
	    if(scene->material[c->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[c->Material].opacity == 1.0)return 0;
	}


	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	ndata=scene->cdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
	vmin =  1e60;
	vmax = -1e60;
        
    k=0;
    ks=0;
	nel=0;
	for(n=0;n<c->LevelCount;++n){
		sprintf(name,"laser_plot_e(%ld)%c",(long)c->Levels[n],0);
		e=NULL;
		if(SageGetFileItem(c->Files,name,0,&e,&length1,itIs)){
		
		    sprintf(WarningBuff,"CPio2dRayLoad Error Reading %s Frame %ld\n",name,itIs);
		    WarningBatch(WarningBuff);
		
		    goto doContinue;
		}
		sprintf(name,"laser_plot_x(%ld)%c",(long)c->Levels[n],0);
		x=NULL;
		if(SageGetFileItem(c->Files,name,0,&x,&length1,itIs)){
		    goto doContinue;
		}
				
		sprintf(name,"laser_plot_y(%ld)%c",(long)c->Levels[n],0);
		y=NULL;
		if(SageGetFileItem(c->Files,name,0,&y,&length2,itIs)){
		    goto doContinue;
		}
						
		sprintf(name,"laser_plot_z(%ld)%c",(long)c->Levels[n],0);
		z=NULL;
		if(SageGetFileItem(c->Files,name,0,&z,&length2,itIs)){
		    goto doContinue;
		}
		if((length1 != length2) || (length1 < 2))goto doContinue;
		
		
		for(kk=0;kk<length1-1;++kk){
		    double xl,s;
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;

			s=0;

			for(i=0;i<2;++i){
			   double vv;
			   dx=x[kk]+s*(x[kk+1]-x[kk])+c->origin.x;
			   dy=y[kk]+s*(y[kk+1]-y[kk])+c->origin.y;
			   dz=z[kk]+s*(z[kk+1]-z[kk])+c->origin.z;
			   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
			   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
			   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
			   vv=e[kk];
			   vdata[k]=vv;
			   if(vv < vmin)vmin=vv;
			   if(vv > vmax)vmax=vv;
			   ++k;
			   s=1.0;
			}
			    
		    ndata[nel++]=c->Material;
		    ndata[nel++]=ks;
		    ndata[nel++]=ks+1;
			    
		    ks=k;
			    			    
		}
doContinue:		
		if(x)cFree((char *)x);
		x=NULL;		
		if(y)cFree((char *)y);
		y=NULL;		
		if(z)cFree((char *)z);
		z=NULL;		
		if(e)cFree((char *)e);
		e=NULL;		
	}
	    	    
	nel /= 3;
	    
	checkPaletteLevels(scene,c->Material,vmin,vmax);

	doNoShadowLines(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);

	*CellCount += nel;
    c->LastCell = *CellCount;

	return 0;
}
int CPio2dRayDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio2dRayPtr c=(CPio2dRayPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double vmin,vmax;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k,ks,kk;
	Vec Lampl,Eyel;
	char name[256];
	int i,j;
	long n,itIs;
	double *x,*y,*z,*e;
	long length1,length2,nsphere;
	double r,rstep,sum,radiusStepFactor;

	scene=c->scene;

	if(flag == 1){
	    if(scene->material[c->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[c->Material].opacity == 1.0)return 0;
	}

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	gdata=scene->gdata;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
    r=c->radius;
    if(r <= 0)r=0.2*c->LengthGreatest;
    radiusStepFactor=c->radiusStepFactor;
    if(radiusStepFactor <= 0)radiusStepFactor=1.0;
    rstep=2*r*radiusStepFactor;
    
    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);
    	
	vmin =  1e60;
	vmax = -1e60;
    k=0;
    ks=0;
	nel=0;
	nsphere=0;
	for(n=0;n<c->LevelCount;++n){
		sprintf(name,"laser_plot_e(%ld)%c",(long)c->Levels[n],0);
		e=NULL;
		if(SageGetFileItem(c->Files,name,0,&e,&length1,itIs)){
		
		    sprintf(WarningBuff,"CPio2dRayLoad Error Reading %s Frame %ld\n",name,itIs);
		    WarningBatch(WarningBuff);
		
		    goto doContinue;
		}
		sprintf(name,"laser_plot_x(%ld)%c",(long)c->Levels[n],0);
		x=NULL;
		if(SageGetFileItem(c->Files,name,0,&x,&length1,itIs)){
		    goto doContinue;
		}
				
		sprintf(name,"laser_plot_y(%ld)%c",(long)c->Levels[n],0);
		y=NULL;
		if(SageGetFileItem(c->Files,name,0,&y,&length2,itIs)){
		    goto doContinue;
		}
						
		sprintf(name,"laser_plot_z(%ld)%c",(long)c->Levels[n],0);
		z=NULL;
		if(SageGetFileItem(c->Files,name,0,&z,&length2,itIs)){
		    goto doContinue;
		}
		if((length1 != length2) || (length1 < 2))goto doContinue;
		
		
		sum=0;		    
		for(kk=0;kk<length1-1;++kk){
		    double xl,s,delta;
		    
	        xl=sqrt((x[kk+1]-x[kk])*(x[kk+1]-x[kk])+(y[kk+1]-y[kk])*(y[kk+1]-y[kk])+(z[kk+1]-z[kk])*(z[kk+1]-z[kk]));
	        if(xl <= 0)continue;
		    
		    if(sum+xl < rstep){
		        sum += 	xl;	
		        continue;
		   	} 
		   			   			   	
		   	delta=rstep-sum;
		   	
		    do {
	           if((k+c->ny*c->nx) > scene->MostNodes){
	               sprintf(WarningBuff,"CPio2dRayDraw Skipping Memory Used %ld Avaiable %ld\n",
	                       k,scene->MostNodes);
	               WarningBatch(WarningBuff);
	               sprintf(WarningBuff,"delta %g rstep %g xl %g\n",delta,rstep,xl);
	               WarningBatch(WarningBuff);
	               kk=length1;
	               goto doContinue;
	           }
		        ++nsphere;
		        		    		   	   
		   	    s=delta/xl;
		   	    
			    for(j=0;j<c->ny;++j){
			        double sinf,cosf,vv;
			        sinf=sin(pi*(1.-j*dumy));
			        cosf=cos(pi*(1.-j*dumy));
			        for(i=0;i<c->nx;++i){
			           dx=x[kk]+s*(x[kk+1]-x[kk])+c->origin.x+r*cos(2*pi*(1.-i*dumx))*sinf;
			           dy=y[kk]+s*(y[kk+1]-y[kk])+c->origin.y+r*sin(2*pi*(1.-i*dumx))*sinf;
			           dz=z[kk]+s*(z[kk+1]-z[kk])+c->origin.z+r*cosf;
			           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
			           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
			           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
					   vv=e[kk];
					   vdata[k]=vv;
					   if(vv < vmin)vmin=vv;
					   if(vv > vmax)vmax=vv;
			           ++k;
			        }
			    }			    
			    
			    for(j=0;j<c->ny-1;++j){
			        for(i=0;i<c->nx-1;++i){
					    ndata[nel++]=c->Material;
					    ndata[nel++]=i+j*c->nx+ks;
					    ndata[nel++]=i+1+j*c->nx+ks;
					    ndata[nel++]=i+1+(j+1)*c->nx+ks;
					    ndata[nel++]=i+(j+1)*c->nx+ks;
			        }
			    }
			    
			    ks=k;			    			    
		   		delta += rstep;
			    
		    }while(delta < xl);
		    
		    sum=(1-s)*xl;
		}
doContinue:		
		if(x)cFree((char *)x);
		x=NULL;		
		if(y)cFree((char *)y);
		y=NULL;		
		if(z)cFree((char *)z);
		z=NULL;		
		if(e)cFree((char *)e);
		e=NULL;		
	}
	    	    
	nel /= 5;
	    
	checkPaletteLevels(scene,c->Material,vmin,vmax);
	
	if(gdata){
	    k=0;
		for(n=0;n<nsphere;++n){
		    for(j=0;j<c->ny;++j){
		        double sinf,cosf;
		        sinf=sin(pi*(1.-j*dumy));
		        cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){
			        Vec P,N,D,L;
			        double len;
			        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
			        
				MakeVector(xdatal[k],ydatal[k],zdatal[k],P);
					
				MakeVector((dx)*v1x+(dy)*v1y+(dz)*v1z,(dx)*v2x+(dy)*v2y+(dz)*v2z,
					           (dx)*v3x+(dy)*v3y+(dz)*v3z,N);
					
				/* Vector Eye To Point */
					
		    		VecSub(P,Eyel,D);
		    		
				/* Check Direction To Eye Point */

				if((VecDot(D,N)) > 0.0){
				     VecNegate(N);
				}


				/*  Vector from Point to Lamp */

				VecSub(Lampl,P,L);
					
				/* Check Direction of Lamp */


				 if((VecDot(L,N)) > 0.0){
				     if((len=VecLen(N)) > 0.0){
				         N[0] /= len;
				         N[1] /= len;
				         N[2] /= len;
				     }
				     if((len=VecLen(L)) > 0.0){
				         L[0] /= len;
				         L[1] /= len;
				         L[2] /= len;
				     }
				     gdata[k]=VecDot(N,L);
				 }else{
				     gdata[k]=0.;
				 }
				 ++k;
			    }
		    }
		}
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	}else if(nvdata){
	    k=0;
		for(n=0;n<nsphere;++n){
	        for(j=0;j<c->ny;++j){
	            double sinf,cosf;
	            sinf=sin(pi*(1.-j*dumy));
	            cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){		        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
					 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
					 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
					 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			    }
	    	}
	    }
	    if(rdata && sdata){
		    dx=0;
		    if(c->nx > 1)dx=1.0/((double)(c->nx-1));

		    dy=0;
		    if(c->ny > 1)dy=1.0/((double)(c->ny-1));

    	    k=0;
			for(n=0;n<nsphere;++n){
	            for(j=0;j<c->ny;++j){
			        for(i=0;i<c->nx;++i){
					    rdata[k]=(float)((i)*dx);
					    sdata[k++]=(float)((j)*dy);
			        }
		    	}
	    	}
	    }
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}
	*CellCount += nel;
    c->LastCell = *CellCount;

	return 0;
}
int CPio2dRayLoad(CPio2dRayPtr s,long itIs)
{
	char name[256];
	struct Scene *scene;
	long n;
	double *e,*x,*y,*z,len,sum,ll;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double vmin,vmax;
	long length,length1;
	int k;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if((s->LevelCount <= 0) || !s->Levels)return 0;
	if(s->ItWas == itIs)return 1;

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    return 0;
	}
	
	if(s->ItWas == itIs)return 1;
	
	xmin=1e60;
	ymin=1e60;
	zmin=1e60;
	vmin=1e60;
	xmax=-1e60;
	ymax=-1e60;
	zmax=-1e60;
	vmax=-1e60;
	sum = 0;
	len = 0;
	for(n=0;n<s->LevelCount;++n){
		e=NULL;
		x=NULL;
		y=NULL;
		z=NULL;
		sprintf(name,"laser_plot_e(%ld)%c",(long)s->Levels[n],0);
		if(SageGetFileItem(s->Files,name,0,&e,&length,itIs)){
		
		    sprintf(WarningBuff,"CPio2dRayLoad Error Reading %s Frame %ld\n",name,itIs);
		    WarningBatch(WarningBuff);
		
		    goto doContinue;
		}
		sprintf(name,"laser_plot_x(%ld)%c",(long)s->Levels[n],0);
		if(SageGetFileItem(s->Files,name,0,&x,&length,itIs)){		
		    sprintf(WarningBuff,"CPio2dRayLoad Error Reading %s Frame %ld\n",name,itIs);
		    WarningBatch(WarningBuff);		
		    goto doContinue;
		}
				
		sprintf(name,"laser_plot_y(%ld)%c",(long)s->Levels[n],0);
		if(SageGetFileItem(s->Files,name,0,&y,&length1,itIs)){
		    goto doContinue;
		}
						
		sprintf(name,"laser_plot_z(%ld)%c",(long)s->Levels[n],0);
		if(SageGetFileItem(s->Files,name,0,&z,&length1,itIs)){
		    goto doContinue;
		}
		if(length != length1)goto doContinue;

		for(k=1;k<length;++k){
		    ll=sqrt((x[k-1]-x[k])*(x[k-1]-x[k])+(y[k-1]-y[k])*(y[k-1]-y[k])+(z[k-1]-z[k])*(z[k-1]-z[k]));
		    sum += ll;
		    if((ll > len))len = ll;
			if(x[k] < xmin)xmin=x[k];
			if(x[k] > xmax)xmax=x[k];
			if(y[k] < ymin)ymin=y[k];
			if(e[k] < vmin)vmin=e[k];
			if(y[k] > ymax)ymax=y[k];
			if(z[k] < zmin)zmin=z[k];
			if(z[k] > zmax)zmax=z[k];
			if(e[k] > vmax)vmax=e[k];
		}	

doContinue:		
		if(e)cFree((char *)e);
		e=NULL;		
		if(x)cFree((char *)x);
		x=NULL;		
		if(y)cFree((char *)y);
		y=NULL;		
		if(z)cFree((char *)z);
		z=NULL;		
	}

	checkPaletteLevels(scene,s->Material,vmin,vmax);	
	
	s->LengthTotal=sum;
	s->LengthGreatest=len;

	s->xminData=xmin;
	s->yminData=ymin;
	s->zminData=zmin;
	s->xmaxData=xmax;
	s->ymaxData=ymax;
	s->ymaxData=ymax;
	s->setData=TRUE;
	
	s->ItWas = itIs;
	return 0;
}
int CPio2dRayMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio2dRayPtr s=(CPio2dRayPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio2dRayLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Sage2d Ray Files\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}else if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    double xdata[8],ydata[8],zdata[8];
		struct Matrix GroupToWorld;
		struct Matrix GroupToScreen;
	    struct P p;
	    long n;
	    
	    if(!s->setData)return 0;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);
	    
	    xdata[0]=s->xminData;
	    xdata[1]=s->xmaxData;
	    xdata[2]=s->xmaxData;
	    xdata[3]=s->xminData;
	    xdata[4]=s->xminData;
	    xdata[5]=s->xmaxData;
	    xdata[6]=s->xmaxData;
	    xdata[7]=s->xminData;

	    ydata[0]=s->yminData;
	    ydata[1]=s->yminData;
	    ydata[2]=s->ymaxData;
	    ydata[3]=s->ymaxData;
	    ydata[4]=s->yminData;
	    ydata[5]=s->yminData;
	    ydata[6]=s->ymaxData;
	    ydata[7]=s->ymaxData;

	    zdata[0]=s->zminData;
	    zdata[1]=s->zminData;
	    zdata[2]=s->zminData;
	    zdata[3]=s->zminData;
	    zdata[4]=s->zmaxData;
	    zdata[5]=s->zmaxData;
	    zdata[6]=s->zmaxData;
	    zdata[7]=s->zmaxData;


	    for(n=0;n<8;++n){
	        p.x=xdata[n];
	        p.y=ydata[n];
	        p.z=zdata[n];
	        Transform(&p,&GroupToScreen);
	        if(p.x > Message->xmax)Message->xmax=p.x;
	        if(p.y > Message->ymax)Message->ymax=p.y;
	        if(p.z > Message->zmax)Message->zmax=p.z;
	        if(p.x < Message->xmin)Message->xmin=p.x;
	        if(p.y < Message->ymin)Message->ymin=p.y;
	        if(p.z < Message->zmin)Message->zmin=p.z;
	    }
	}
	return 0;
}
CObjectPtr CPio2dRayDuplicate(CObject *b)
{
	CPio2dRayPtr s=(CPio2dRayPtr)b;
	CPio2dRayPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio2dRayPtr)cMalloc(sizeof(CPio2dRay),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}

	return (CObjectPtr)sdsNew;
}
int CPio2dRaykill(CObject *o)
{
	CPio2dRayPtr s=(CPio2dRayPtr)o;
	if(!s)return 1;
	
	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}
	cFree((char *)s);
	return 0;
}

CObjectPtr uPio2dExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio2dExtStruct b;
	
	static struct universalTableListStruct Local[] = {

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.CutMin},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.CutMax},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.vscale},	

		{"DOUB",4021,universal_TypeDouble,(void *)&b.sd.range.xminData},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.sd.range.yminData},	

		{"DOUB",4023,universal_TypeDouble,(void *)&b.sd.range.xmaxData},	
		{"DOUB",4024,universal_TypeDouble,(void *)&b.sd.range.ymaxData},	

		{"DOUB",4025,universal_TypeLong,(void *)&b.sd.range.ixmax},	
		{"DOUB",4026,universal_TypeLong,(void *)&b.sd.range.iymax},	

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},
		
		{"REAL",4028,universal_TypeInt,(void *)&b.pioIndex},	
		{"STRI",4029,universal_TypeString,(void *)&b.pioName[0],sizeof(b.pioName)},
				
		{"REAL",4030,universal_TypeInt,(void *)&b.pioIndexPaint},	
		{"STRI",4031,universal_TypeString,(void *)&b.pioNamePaint[0],sizeof(b.pioNamePaint)},
				
		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio2dExtPtr bp=(CPio2dExtPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio2dExtCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CPio2dExtLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CPio2dExtPtr CPio2dExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	CPio2dExtPtr s;
	struct Block b;
	char buff[256];
	static int Count;
	int type;

	if(!scene)return (CPio2dExtPtr)NULL;
	
	if(flag){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CPio2dExtPtr)NULL;
	}

	s=(CPio2dExtPtr)cMalloc(sizeof(CPio2dExt),8566);
	if(!s)return (CPio2dExtPtr)NULL;
	zerol((char *)s,sizeof(CPio2dExt));

	if(Files->fileClass == PIO1D_FILES){
		sprintf(buff,"Pio1dExt_%03d",Count++);	
	}else{
		sprintf(buff,"Pio2dExt_%03d",Count++);	
	}
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;
    
    if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
	
	CObjectCreate(scene,(CObjectPtr)s);

	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);

	s->type = G_EXTRUDE_PIO2D;
	s->duplicateObject=CPio2dExtDuplicate;
	
	s->message=CPio2dExtMessage;
	
	s->ReadWrite=uPio2dExtWrite;
	
	s->killObject=CPio2dExtkill;
	s->Files=Files;
	s->vscale=0.0;
	s->CutMin=0.0;
	s->CutMax=0.0;
	
	
	s->BlockNumber=scene->BlockCount-1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CPio2dExtPtr)NULL;
		CPio2dExtLoad(s,0L);
	}
	return s;
ErrorOut:
	return (CPio2dExtPtr)NULL;
}
int CPio2dExtMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio2dExtPtr s=(CPio2dExtPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio2dExtLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Sage2d Extrude Files\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}
	return 0;
}
int CPio2dExtLoad(CPio2dExtPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct Block *b;
	double *data;
	double *dat;
	long ne;
	long i,j;
	long xdiff,ydiff;
	double vscale,xoff,yoff;
	double dx,dy;
	struct areadata ai;
	int ret;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;


	if(s->ItWas == itIs)return 0;

	b = &(scene->block[s->BlockNumber]);


	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}

	if(s->ItWas == itIs)return 0;
	
	ret=1;
	
	data=NULL;

	zerol((char *)&ai,sizeof(struct areadata));
	
	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
	
	ai.xminArea=s->sd.range.xminData;
	ai.yminArea=s->sd.range.yminData;
	ai.zminArea=0;
	ai.xmaxArea=s->sd.range.xmaxData;
	ai.ymaxArea=s->sd.range.ymaxData;
	ai.zmaxArea=0;
	ai.xsize=s->sd.range.ixmax;
	ai.ysize=s->sd.range.iymax;;
	ai.zsize=0;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CPio2dExtLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	if(!data)goto ErrorOut;
	
	s->ai=ai;
	
	dx=(ai.xmaxArea-ai.xminArea)/(double)xsize;
	dy=(ai.ymaxArea-ai.yminArea)/(double)ysize;


	xdiff=xsize;
	ydiff=ysize;

	xoff=0.;
	yoff=0.;
	
	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;
	
	
	if(s->usePaint){
		ai.pioIndex=s->pioIndexPaint;
	    if(s->pioNamePaint[0]){
	        mstrncpy(ai.pioName,s->pioNamePaint,64);
	    }else{
	        mstrncpy(ai.pioName,"density",64);
	    }
	    ai.data=NULL;
		if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
			sprintf(WarningBuff,"CPio2dExtLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
		if(!ai.data)goto ErrorOut;
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    for(i=0;i<xsize;++i){
				ne=i+(j)*xsize;
				b->vdata[ne]=ai.data[i+(ysize-1-j)*xsize];
				b->zdata[ne]=vscale*dat[i+(ysize-1-j)*xsize];
				b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
				b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
		    }
		}
		
		if(ai.data)cFree((char *)ai.data);
	}else{
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    for(i=0;i<xsize;++i){
				ne=i+(j)*xsize;
				b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
				b->zdata[ne]=(vscale*b->vdata[ne]);
				b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
				b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
		    }
		}

	}
	
	if(s->CutMin < s->CutMax){
	    long ElementCount;
	    long *Elements;
	    long nn,kp;
	    
	    Elements=b->Elements;
	    if(!Elements)goto ErrorOut;
	    ElementCount=b->ElementCount;
	    
	    ne=0;
	    nn=0;
		dat=(double *)data;
		for(ne=0;ne<ElementCount;++ne){
		    kp=Elements[ne*5+1];
		    j=kp/xsize;
		    i=kp-j*xsize;
			if(dat[i+(ysize-1-j)*xsize] >= s->CutMin &&
			   dat[i+(ysize-1-j)*xsize] <= s->CutMax){
			   Elements[nn++]=Elements[ne*5];
			   Elements[nn++]=Elements[ne*5+1];
			   Elements[nn++]=Elements[ne*5+2];
			   Elements[nn++]=Elements[ne*5+3];
			   Elements[nn++]=Elements[ne*5+4];
			}
		}
		b->ElementCount=nn/5;
	}

	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if(SetBlockRS(b))goto ErrorOut;
	    if(b->rdata && b->sdata){
			double dx,dy;
			long k;
			dx=0;
			if(xdiff > 1)dx=1.0/((double)(xdiff-1));

			dy=0;
			if(ydiff > 1)dy=1.0/((double)(ydiff-1));

			k=0;
			for(j=0;j<ydiff;++j){
			    for(i=0;i<xdiff;++i){
				    b->rdata[k]=(float)((i)*dx);
				    b->sdata[k++]=(float)((j)*dy);
				}
			}
	    }
	}

	elementrange(b);

	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);
	
	s->ItWas = itIs;
	
	ret=0;
	
ErrorOut:

	if(data)cFree((char *)data);

	return ret;
}
CObjectPtr CPio2dExtDuplicate(CObject *b)
{
	CPio2dExtPtr s=(CPio2dExtPtr)b;
	CPio2dExtPtr sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio2dExtPtr)cMalloc(sizeof(CPio2dExt),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}

int CPio2dExtkill(CObject *o)
{
	CPio2dExtPtr s=(CPio2dExtPtr)o;
	if(!s)return 1;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}
	cFree((char *)s);
	return 0;
}
CObjectPtr uPio2dRotWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio2dRotStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"DOUB",4016,universal_TypeLong,(void *)&b.Axis},	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.Angle},	
		{"DOUB",4019,universal_TypeLong,(void *)&b.PassSmooth},	
		{"DOUB",4020,universal_TypeLong,(void *)&b.Angles},	


		{"DOUB",4021,universal_TypeDouble,(void *)&b.sd.range.xminData},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.sd.range.yminData},	

		{"DOUB",4023,universal_TypeDouble,(void *)&b.sd.range.xmaxData},	
		{"DOUB",4024,universal_TypeDouble,(void *)&b.sd.range.ymaxData},	

		{"DOUB",4025,universal_TypeLong,(void *)&b.sd.range.ixmax},	
		{"DOUB",4026,universal_TypeLong,(void *)&b.sd.range.iymax},	

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},
		
		{"REAL",4028,universal_TypeInt,(void *)&b.pioIndex},	
		{"STRI",4029,universal_TypeString,(void *)&b.pioName[0],sizeof(b.pioName)},
		
		
		{"REAL",4030,universal_TypeInt,(void *)&b.pioIndexPaint},	
		{"STRI",4031,universal_TypeString,(void *)&b.pioNamePaint[0],sizeof(b.pioNamePaint)},		
		

		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files2},	

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio2dRotPtr bp=(CPio2dRotPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio2dRotCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CPio2dRotLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CPio2dRotPtr CPio2dRotCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	struct Block b;
	CPio2dRotPtr s;
	char buff[256];
	static int Count;
	int type;

	if(!scene || !Files)return (CPio2dRotPtr)NULL;

	if(flag){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CPio2dRotPtr)NULL;
	}

	s=(CPio2dRotPtr)cMalloc(sizeof(CPio2dRot),8569);
	if(!s)return (CPio2dRotPtr)NULL;
	zerol((char *)s,sizeof(CPio2dRot));

	sprintf(buff,"Pio2dRot_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);	

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_ROTATE_PIO2D;
	s->Files=Files;
		
	s->duplicateObject=CPio2dRotDuplicate;

	s->message=CPio2dRotMessage;
	s->ReadWrite=uPio2dRotWrite;

	s->killObject=CPio2dRotkill;
	s->BlockNumber=scene->BlockCount-1;
	s->vscale=1.0;
	s->Angles=20;
	s->Axis=0;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->xmax = (int)s->sd.range.ixmax;
	s->ymax = (int)s->sd.range.iymax;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->Angle=180;
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CPio2dRotPtr)NULL;
	}
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CPio2dRotPtr)NULL; 
}
int CPio2dRotMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio2dRotPtr s=(CPio2dRotPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio2dRotLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Sage2d Rotate Files\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}
	return 0;
}
int CPio2dRotLoad(CPio2dRotPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct cdatal d;
	struct Block *b;
	double *data;
	double *dat;
	long ne,n,nn,ns;
	long i,j,nodes,cells;
	long xdiff,ydiff;
	double xoff,yoff;
	struct areadata ai;
	double dx,dy;
	long nee,ne1,ne2,n1,n2;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if((s->LevelCount <= 0) || !s->Levels)return 0;
	if(s->ItWas == itIs)return 1;

	b = &(scene->block[s->BlockNumber]);

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}

	if(s->ItWas == itIs)return 1;
	
	data=NULL;

	d.cList=NULL;

	zerol((char *)&ai,sizeof(struct areadata));

	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
		
	ai.xminArea=s->sd.range.xminData;
	ai.yminArea=s->sd.range.yminData;
	ai.zminArea=0;
	ai.xmaxArea=s->sd.range.xmaxData;
	ai.ymaxArea=s->sd.range.ymaxData;
	ai.zmaxArea=0;
	ai.xsize=s->sd.range.ixmax;
	ai.ysize=s->sd.range.iymax;
	ai.zsize=0;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CPio2dRotLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	s->ai=ai;

	dx=(ai.xmaxArea-ai.xminArea)/(double)xsize;
	dy=(ai.ymaxArea-ai.yminArea)/(double)ysize;

	xdiff=xsize;
	ydiff=ysize;
	xoff=0.;
	yoff=0.;

	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;

	dat=(double *)data;
	for(j=0;j<ysize;++j){
	    for(i=0;i<xsize;++i){
			ne=i+(j)*xsize;
			b->Elements[ne]=s->Material;
			b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
			b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
			b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
	    }
	}

	zerol((char *)&d,sizeof(struct cdatal));

	LineCountours(b->xdata,b->ydata,b->vdata,b->Elements,
	             s->Levels,xdiff,ydiff,s->LevelCount,&d);

	nodes=0;
	cells=0;
	for(n=0;n<d.count;++n){
	    nodes += d.cList[n].nndata;
	    cells += d.cList[n].nndata-1;
	}


	if(blockSize(b,s->Material,nodes*s->Angles,cells*(s->Angles-1)))return 1;

	ne=0;
	nn=0;
	for(n=0;n<d.count;++n){
	    double *xx,*yy,*vv;
	    double r,sine,cosine,afac,v;

	    afac=(8.0/360.)*atan(1.0)*s->Angle/((double)(s->Angles)-1.0);

	    nodes = d.cList[n].nndata;
	    cells = d.cList[n].nndata-1;
	    xx=d.cList[n].xdata;
	    yy=d.cList[n].ydata;
	    vv=d.cList[n].vdata;
	    if(!xx || !yy || !vv)continue;
	    v=s->Levels[d.cList[n].nlevel];
	    ns=nn;
	    
	    if(s->usePaint){
		    struct linedata li;
			zerol((char *)&li,sizeof(struct linedata));
		    li.pioIndex=s->pioIndexPaint;
		    if(s->pioNamePaint[0]){
		        mstrncpy(li.pioName,s->pioNamePaint,64);
		    }else{
		        mstrncpy(li.pioName,"density",64);
		    }
			li.x=xx;
			li.y=yy;
			li.z=NULL;
			li.v=vv;
			li.PointLineCount=(int)nodes;
			li.type=LINEDATA_LIST_DATA;
			li.CurrentFrame=itIs;
			if(SendMessageByName(s->BatchName,MessageType_GET_LINE_DATA,&li)){
				sprintf(WarningBuff,"CPio2dRotLoad -  pioName (%s) Error Getting Paint Data\n",li.pioName);
				WarningBatch(WarningBuff);
				continue;
			}	    	    
	    }else{
	    	for(i=0;i<nodes;++i){
	    	    vv[i]=v;
	    	}
	    }
	    
	    for(j=0;j<s->PassSmooth;++j){
	        for(i=2;i<nodes;++i){
	            yy[i-1]=(.5*yy[i-1]+.25*yy[i-2]+.25*yy[i]);
	            xx[i-1]=(.5*xx[i-1]+.25*xx[i-2]+.25*xx[i]);
	        }
	    }

	    if(s->Axis == 0){
	        for(j=0;j<s->Angles;++j){
	            sine=sin((double)j*afac);
		    	cosine=cos((double)j*afac);
	            for(i=0;i<nodes;++i){
	                r=sqrt(yy[i]*yy[i]);
					b->xdata[nn]=xx[i];
					b->ydata[nn]=(r*cosine);
					b->zdata[nn]=(r*sine);
					b->vdata[nn]=vv[i];
					++nn;
	            }
	        }
	    }else {
	        for(j=0;j<s->Angles;++j){
	            sine=sin((double)j*afac);
		    	cosine=cos((double)j*afac);
	            for(i=0;i<nodes;++i){
	                r=sqrt(xx[i]*xx[i]);
					b->xdata[nn]=(r*cosine);
					b->ydata[nn]=yy[i];
					b->zdata[nn]=(r*sine);
					b->vdata[nn]=vv[i];
					++nn;
	            }
	        }
	    }
	    
		if(blockCells(b,s->Material,ns,&ne,nodes,s->Angles))goto ErrorOut;
		if(s->Angle == 360){
		    
		    nee=ne/5;
		    
		    ne2=nee-(nodes-1);
		    ne1=nee-(nodes-1)*(s->Angles-1);
		    
            for(i=0;i<nodes-1;++i){
                n1=5*(ne1+i);
                n2=5*(ne2+i);
                b->Elements[n1+1]=b->Elements[n2+4];
                b->Elements[n1+2]=b->Elements[n2+3];
            }
		    
		    
		}	    
	}
	
	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    double dx,dy;
	    if(SetBlockRS(b))goto ErrorOut;
	    nn=0;
	    for(n=0;n<d.count;++n){
 
	        nodes = d.cList[n].nndata;
  	        ns=nn;

	        dx=0;
	        if(nodes > 1)dx=1.0/((double)(nodes-1));

	        dy=0;
	        if(s->Angles > 1)dy=1.0/((double)(s->Angles-1));
 	        for(j=0;j<s->Angles;++j){
 	            for(i=0;i<nodes;++i){
					b->sdata[nn]=(float)((i)*dx);
					b->rdata[nn]=(float)((j)*dy);
					++nn;
	            }
	        }
	    }
	}


	elementrange(b);

	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);

	s->ItWas = itIs;
	
ErrorOut:

	if(data)cFree((char *)data);

	if(d.cList){
	    for(n=0;n<d.count;++n){
	        if(d.cList[n].xdata){
	            cFree((char *)d.cList[n].xdata);
	            d.cList[n].xdata=NULL;
	        }
	        if(d.cList[n].ydata){
	            cFree((char *)d.cList[n].ydata);
	            d.cList[n].ydata=NULL;
	        }
	        if(d.cList[n].vdata){
	            cFree((char *)d.cList[n].vdata);
	            d.cList[n].vdata=NULL;
	        }
	    }
	    cFree((char *)d.cList);
	    d.cList=NULL;
	}
	
	return 0;
}
int CPio2dRotkill(CObject *o)
{
	CPio2dRotPtr s=(CPio2dRotPtr)o;
	if(!s)return 1;

	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}

	cFree((char *)s);
	return 0;
}
CObjectPtr CPio2dRotDuplicate(CObject *b)
{
	CPio2dRotPtr s=(CPio2dRotPtr)b;
	CPio2dRotPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio2dRotPtr)cMalloc(sizeof(CPio2dRot),8567);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;
	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}

	return (CObjectPtr)sdsNew;
}
CObjectPtr uPio3dExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CPio3dExtStruct b;
	
	static struct universalTableListStruct Local[] = {

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.CutMin},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.CutMax},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.vscale},	


		{"DOUB",5021,universal_TypeDouble,(void *)&b.sd.plane.Plane_gx1},	
		{"DOUB",5022,universal_TypeDouble,(void *)&b.sd.plane.Plane_gy1},	
		{"DOUB",5023,universal_TypeDouble,(void *)&b.sd.plane.Plane_gz1},	
		
		{"DOUB",5024,universal_TypeDouble,(void *)&b.sd.plane.Plane_gx2},	
		{"DOUB",5025,universal_TypeDouble,(void *)&b.sd.plane.Plane_gy2},	
		{"DOUB",5026,universal_TypeDouble,(void *)&b.sd.plane.Plane_gz2},	
		
		{"DOUB",5027,universal_TypeDouble,(void *)&b.sd.plane.Plane_gx3},	
		{"DOUB",5028,universal_TypeDouble,(void *)&b.sd.plane.Plane_gy3},	
		{"DOUB",5029,universal_TypeDouble,(void *)&b.sd.plane.Plane_gz3},	
		
		{"DOUB",5030,universal_TypeDouble,(void *)&b.sd.plane.Plane_gmin},	
		{"DOUB",5031,universal_TypeDouble,(void *)&b.sd.plane.Plane_gmax},	
		{"DOUB",5032,universal_TypeDouble,(void *)&b.sd.plane.Plane_gloc},	


		{"DOUB",5033,universal_TypeLong,(void *)&b.sd.plane.Plane_xcount},	
		{"DOUB",5034,universal_TypeLong,(void *)&b.sd.plane.Plane_ycount},	



		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},
		
		{"REAL",4028,universal_TypeInt,(void *)&b.pioIndex},	
		{"STRI",4029,universal_TypeString,(void *)&b.pioName[0],sizeof(b.pioName)},
		
		
		{"REAL",4030,universal_TypeInt,(void *)&b.pioIndexPaint},	
		{"STRI",4031,universal_TypeString,(void *)&b.pioNamePaint[0],sizeof(b.pioNamePaint)},
		
		

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CPio3dExtPtr bp=(CPio3dExtPtr)o;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CPio3dExtCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CPio3dExtLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CPio3dExtPtr CPio3dExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	CPio3dExtPtr s;
	struct Block b;
	char buff[256];
	static int Count;
	int type,isOpen;

	if(!scene)return (CPio3dExtPtr)NULL;

	isOpen=FALSE;
	
	if(flag){	
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CPio3dExtPtr)NULL;	
	}

	s=(CPio3dExtPtr)cMalloc(sizeof(CPio3dExt),8566);
	if(!s)return (CPio3dExtPtr)NULL;
	zerol((char *)s,sizeof(CPio3dExt));

	sprintf(buff,"Pio3dExt_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	isOpen=TRUE;
	if(flag){
	    type = MessageType_DO_GENERAL_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
	
	CObjectCreate(scene,(CObjectPtr)s);

	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);

	s->type = G_EXTRUDE_PIO3D;
	
	s->duplicateObject=CPio3dExtDuplicate;	
	s->message=CPio3dExtMessage;	
	s->ReadWrite=uPio3dExtWrite;	
	s->killObject=CPio3dExtkill;
	
	s->Files=Files;
	s->vscale=0.0;
	s->CutMin=0.0;
	s->CutMax=0.0;
	
	
	s->BlockNumber=scene->BlockCount-1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CPio3dExtPtr)NULL;
		CPio3dExtLoad(s,0L);
	}
	
	return s;
ErrorOut:
	if(s){
	   if(isOpen){
	   		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
		}
		cFree((char *)s);
	}
	return (CPio3dExtPtr)NULL;
}
int CPio3dExtMessage(CObject *b,struct ObjectMessage *Message)
{
	CPio3dExtPtr s=(CPio3dExtPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CPio3dExtLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Sage3d Extrude Files\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}
	return 0;
}
int CPio3dExtLoad(CPio3dExtPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct Block *b;
	double *data;
	double *dat;
	long ne;
	long i,j;
	long xdiff,ydiff;
	double vscale,xoff,yoff;
	double dx,dy,dz;
	double v1x,v1y,v1z;
	double v2x,v2y,v2z;
	double v3x,v3y,v3z;
	struct areadata ai;
	int ret;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;

	if(s->ItWas == itIs)return 0;

	b = &(scene->block[s->BlockNumber]);

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}

	if(s->ItWas == itIs)return 0;
	
	ret=1;
	
	data=NULL;


	zerol((char *)&ai,sizeof(struct areadata));
		
	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
	
	ai.x1=s->sd.plane.Plane_gx1;
	ai.y1=s->sd.plane.Plane_gy1;
	ai.z1=s->sd.plane.Plane_gz1;
	
	ai.x2=s->sd.plane.Plane_gx2;
	ai.y2=s->sd.plane.Plane_gy2;
	ai.z2=s->sd.plane.Plane_gz2;
	
	ai.x3=s->sd.plane.Plane_gx3;
	ai.y3=s->sd.plane.Plane_gy3;
	ai.z3=s->sd.plane.Plane_gz3;
	
	ai.gmin=s->sd.plane.Plane_gmin;
	ai.gmax=s->sd.plane.Plane_gmax;
	ai.gloc=s->sd.plane.Plane_gloc;
	
	ai.xsize=s->sd.plane.Plane_xcount;
	ai.ysize=s->sd.plane.Plane_ycount;
	ai.zsize=1;
	
	ai.CurrentFrame=itIs;
	ai.type = GENERAL_OFFSET_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CPio3dExtLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	
	data=ai.data;

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	if(!data)goto ErrorOut;
	
	s->ai=ai;
	
	v1x=ai.x2-ai.x1;
	v1y=ai.y2-ai.y1;
	v1z=ai.z2-ai.z1;
	
	dx=sqrt(v1x*v1x+v1y*v1y+v1z*v1z);

	if(dx){
	   v1x /= dx;
	   v1y /= dx;
	   v1z /= dx;
	}

	dx /= (double)xsize;
	
	v2x=ai.x3-ai.x1;
	v2y=ai.y3-ai.y1;
	v2z=ai.z3-ai.z1;
	
	dy=sqrt(v2x*v2x+v2y*v2y+v2z*v2z);

	if(dy){
	   v2x /= dy;
	   v2y /= dy;
	   v2z /= dy;
	}

	dy /= (double)ysize;

	v3x =  (v1y*v2z-v2y*v1z);
	v3y = -(v1x*v2z-v2x*v1z);
	v3z =  (v1x*v2y-v2x*v1y);
	
	dz=sqrt(v3x*v3x+v3y*v3y+v3z*v3z);
	
	if(dz){
	   v3x /= dz;
	   v3y /= dz;
	   v3z /= dz;
	}

	xdiff=xsize;
	ydiff=ysize;

	xoff=0.;
	yoff=0.;
	
	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;
	
	
	if(s->usePaint){
		ai.pioIndex=s->pioIndexPaint;
	    if(s->pioNamePaint[0]){
	        mstrncpy(ai.pioName,s->pioNamePaint,64);
	    }else{
	        mstrncpy(ai.pioName,"density",64);
	    }
	    ai.data=NULL;
		if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
			sprintf(WarningBuff,"CPio3dExtLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
		if(!ai.data)goto ErrorOut;
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    double dyl,dxl,dzl;
		    dyl=((double)j+0.5)*dy;
		    for(i=0;i<xsize;++i){
		        dxl=((double)i+0.5)*dx;
		        dzl=vscale*dat[i+(ysize-1-j)*xsize];
				ne=i+(j)*xsize;
				b->vdata[ne]=ai.data[i+(ysize-1-j)*xsize];
				b->xdata[ne]=ai.x1+dxl*v1x+dyl*v2x+dzl*v3x;
				b->ydata[ne]=ai.y1+dxl*v1y+dyl*v2y+dzl*v3y;
				b->zdata[ne]=ai.z1+dxl*v1z+dyl*v2z+dzl*v3z;
		    }
		}
		
		if(ai.data)cFree((char *)ai.data);
	}else{
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    double dyl,dxl,dzl;
		    dyl=((double)j+0.5)*dy;
		    for(i=0;i<xsize;++i){
		        dxl=((double)i+0.5)*dx;
		        dzl=vscale*dat[i+(ysize-1-j)*xsize];
				ne=i+(j)*xsize;
				b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
				b->xdata[ne]=ai.x1+dxl*v1x+dyl*v2x+dzl*v3x;
				b->ydata[ne]=ai.y1+dxl*v1y+dyl*v2y+dzl*v3y;
				b->zdata[ne]=ai.z1+dxl*v1z+dyl*v2z+dzl*v3z;
		    }
		}

	}
	
	if(s->CutMin < s->CutMax){
	    long ElementCount;
	    long *Elements;
	    long nn,kp;
	    
	    Elements=b->Elements;
	    if(!Elements)goto ErrorOut;
	    ElementCount=b->ElementCount;
	    
	    ne=0;
	    nn=0;
		dat=(double *)data;
		for(ne=0;ne<ElementCount;++ne){
		    kp=Elements[ne*5+1];
		    j=kp/xsize;
		    i=kp-j*xsize;
			if(dat[i+(ysize-1-j)*xsize] >= s->CutMin &&
			   dat[i+(ysize-1-j)*xsize] <= s->CutMax){
			   Elements[nn++]=Elements[ne*5];
			   Elements[nn++]=Elements[ne*5+1];
			   Elements[nn++]=Elements[ne*5+2];
			   Elements[nn++]=Elements[ne*5+3];
			   Elements[nn++]=Elements[ne*5+4];
			}
		}
		b->ElementCount=nn/5;
	}

	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if(SetBlockRS(b))goto ErrorOut;
	    if(b->rdata && b->sdata){
			double dx,dy;
			long k;
			dx=0;
			if(xdiff > 1)dx=1.0/((double)(xdiff-1));

			dy=0;
			if(ydiff > 1)dy=1.0/((double)(ydiff-1));

			k=0;
			for(j=0;j<ydiff;++j){
			    for(i=0;i<xdiff;++i){
				    b->rdata[k]=(float)((i)*dx);
				    b->sdata[k++]=(float)((j)*dy);
				}
			}
	    }
	}

	elementrange(b);
	
	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);

	s->ItWas = itIs;
	
	ret=0;
	
ErrorOut:

	if(data)cFree((char *)data);

	return ret;
}
CObjectPtr CPio3dExtDuplicate(CObject *b)
{
	CPio3dExtPtr s=(CPio3dExtPtr)b;
	CPio3dExtPtr sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CPio3dExtPtr)cMalloc(sizeof(CPio3dExt),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}

int CPio3dExtkill(CObject *o)
{
	CPio3dExtPtr s=(CPio3dExtPtr)o;
	if(!s)return 1;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
	}
	cFree((char *)s);
	return 0;
}



int CStlMessage(CObject *b,struct ObjectMessage *Message)
{
	CStlPtr s=(CStlPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CStlLoad(s,Message->time);
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  SHOW File\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		}
	}
	return 0;
}
CStlPtr CStlCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	CStlPtr s;
	struct Block b;

	s=NULL;
	if(!scene || !Files)return (CStlPtr)NULL;

	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CStlPtr)NULL;

	s=(CStlPtr)cMalloc(sizeof(CStl),8597);
	if(!s)return (CStlPtr)NULL;
	zerol((char *)s,sizeof(CStl));

	CObjectCreate(scene,(CObjectPtr)s);


	s->type = G_STL;
	s->Files=Files;
	s->vscale=1.0;

	s->duplicateObject=CStlDuplicate;
	s->killObject=CStlkill;
	s->message=CStlMessage;
	s->ReadWrite=CStlWrite;	
	s->BlockNumber=scene->BlockCount-1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->Material=getNextMaterial(scene);;
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CStlPtr)NULL;
		CStlLoad(s,0L);
	}
	return s;
}
CObjectPtr CStlWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CStlStruct b;
	
	static struct universalTableListStruct Local[] = {

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8201,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CStlPtr bp=(CStlPtr)o;
	char head[5];
	long length;
	long tag;
	void *op;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CStlCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		CStlLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	op=&b;
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}

static int CStlkill(CObject *o)
{
	CStlPtr s=(CStlPtr)o;
	if(!s)return 1;

	if(s->scene && s->scene->Closing){
	    freeFileList(s->Files);
	}
	cFree((char *)s);
	return 0;
}
static CObjectPtr CStlDuplicate(CObject *b)
{
	CStlPtr s=(CStlPtr)b;
	CStlPtr sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CStlPtr)cMalloc(sizeof(CStl),8598);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}
static int CStlLoad(CStlPtr s,long itIs)
{
	struct Material *Mat;
	struct Scene *scene;
	struct Block *b;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;


	if(s->ItWas == itIs)return 0;

	b = &(scene->block[s->BlockNumber]);

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}
		
	if(s->ItWas == itIs)return 0;
	
	 blockFree(b);

	if(FilesGetStl(b,itIs,s->Files))return 1;
	
	if(!SetMaterialProperty(scene,&Mat,s->Material)){
		Mat->ColorCount=0;
	}

	elementrange(b);

	s->ItWas = itIs;

	return 0;
}
static int FilesGetStl(struct Block *b,long CurrentFrame,struct FileInfo2 *Files)
{
	double *xdata,*ydata,*zdata;
	char buff[256];
	double xx,yy,zz;
    struct VRML *s;
	long ElementCount;
	long *Elements;
	long NodeCount;
	long nelem4;
	long nn;
	long k;
	int nc;
	int n;
	int c;
	int ret;

	if(!b || !Files)return 1;

	if(CurrentFrame < 0 || CurrentFrame >= Files->ImageCount)return 1;
	if(Files->fileClass != STL_FILES)return 1;

	ret=1;

	s = &Files->VRMLList[CurrentFrame];
	if(!s)return 1;
	if(Files->fileType == STL_DISK){
	    if(SetFileToRead(CurrentFrame,Files))return 1;
	    fseek(Files->in,0L,0); 
	}

	b->NodesMax=3;

	NodeCount=b->NodeCount;
	
	ElementCount=b->ElementCount;
		
	nn=0;
	nc=0;
	nelem4=0;

	while(fgets(buff,256,Files->in)){
		for(n=0;n<256;++n){
		    c=buff[n];
		    if(c == 'v' && buff[n+1] == 'e' && buff[n+2] == 'r'){
		        sscanf(&buff[n]+6,"%lf %lf %lf",&xx,&yy,&zz);
		        if(nn+1 >= NodeCount){
		            NodeCount += 2*NodeCount+10000;
					xdata=(double *)cMalloc(NodeCount*sizeof(double),8469);
					ydata=(double *)cMalloc(NodeCount*sizeof(double),8470);
					zdata=(double *)cMalloc(NodeCount*sizeof(double),8471);
					if(!xdata || !ydata || !zdata)goto ErrorOut;
					
					if(b->xdata && b->ydata && b->zdata){
						for(k=0;k<nn;++k){
						   xdata[k]=b->xdata[k];
						   ydata[k]=b->ydata[k];
						   zdata[k]=b->zdata[k];
						}
					}
					
					if(b->xdata)cFree((char *)b->xdata);
					if(b->ydata)cFree((char *)b->ydata);
					if(b->zdata)cFree((char *)b->zdata);
					b->xdata=xdata;
					b->ydata=ydata;
					b->zdata=zdata;
		        
		        }
	            b->xdata[nn]=xx;
	            b->ydata[nn]=yy;
	            b->zdata[nn++]=zz;		
		        if(++nc == 3){
		            nc = 0;
		            if(nelem4+5 >= ElementCount*(b->NodesMax+1)){
		                ElementCount += 2*ElementCount+10000;
		            
						Elements=(long *)cMalloc(ElementCount*(b->NodesMax+1)*sizeof(long),8481);
						if(!Elements)goto ErrorOut;
		            
		            	if(b->Elements){
							for(k=0;k<nelem4;++k){
							   Elements[k]=b->Elements[k];
							}
						}
						if(b->Elements)cFree((char *)b->Elements);
						b->Elements=Elements;
		            }
	                b->Elements[nelem4++] = 1;
	                b->Elements[nelem4++] = nn-3;
	                b->Elements[nelem4++] = nn-2;
	                b->Elements[nelem4++] = nn-1;		            
		        }   
		        break;
		    }else if(c == 's'  && buff[n+1] == 'o' && buff[n+2] == 'l'){
		       /* ojbect++; */
		        break;
		    }else if(n == 2 && c == 'f' && buff[n+1] == 'a' && buff[n+2] == 'c'){
		       /* sscanf(&buff[n]+12,"%lf %lf %lf",&nxx,&nyy,&nzz); */
		        break;
		    }
		}
	}

	ret=0;
ErrorOut:

	if(!ret){
		b->NodeCount=nn;
	
		b->ElementCount=nelem4/4;
	}

	return 0;
}



#ifdef Junsad9389
int CPio2dRayDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CPio2dRayPtr c=(CPio2dRayPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double vux,vuy,vuz;
	double vxx[2000],vxy[2000],vxz[2000],vyx[2000],vyy[2000],vyz[2000];
	int straight[2000];
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k,ks,kk;
    double rtop;
    double rbottom;
    double length;
	Vec Lampl,Eyel;
	char name[256];
	int i,j;
	long n,itIs,nn,nl;
	double *x,*y,*z,*e;
	long length1,length2;
	double vax,vay,vaz;

	scene=c->scene;

	if(flag == 1){
	    if((scene->material[c->Material].opacity != 1.0))return 0;
	}else if(flag == 2){
	    if((scene->material[c->Material].opacity == 1.0))return 0;
	}


	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	
	vux=scene->Up.x;
	vuy=scene->Up.y;
	vuz=scene->Up.z;

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	gdata=scene->gdata;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
    rbottom=0.2*c->rbottom;
    rtop=rbottom;
    
    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);
    
    k=0;
    ks=0;
	nel=0;
	nl = -1;
	for(n=0;n<c->LevelCount;++n){
		sprintf(name,"laser_plot_e(%ld)%c",(long)c->Levels[n],0);
		e=NULL;
		if(SageGetFileItem(c->Files,name,0,(void **)&e,&length1,itIs)){
		
		    sprintf(WarningBuff,"CPio2dRayLoad Error Reading %s Frame %ld\n",name,itIs);
		    WarningBatch(WarningBuff);
		
		    goto doContinue;
		}
		sprintf(name,"laser_plot_x(%ld)%c",(long)c->Levels[n],0);
		x=NULL;
		if(SageGetFileItem(c->Files,name,0,(void **)&x,&length1,itIs)){
		    goto doContinue;
		}
				
		sprintf(name,"laser_plot_y(%ld)%c",(long)c->Levels[n],0);
		y=NULL;
		if(SageGetFileItem(c->Files,name,0,(void **)&y,&length2,itIs)){
		    goto doContinue;
		}
						
		sprintf(name,"laser_plot_z(%ld)%c",(long)c->Levels[n],0);
		z=NULL;
		if(SageGetFileItem(c->Files,name,0,(void **)&z,&length2,itIs)){
		    goto doContinue;
		}
		if((length1 != length2) || (length1 < 2))goto doContinue;
		
		if(length1 >= 2000)length1=2000-1;
		
		for(kk=1;kk<length1-1;++kk){
		    double vbx,vby,vbz;
		    double nvax,nvay,nvaz,nvbx,nvby,nvbz;
		    double lengtha,lengthb,len;
		    
		    vax=x[kk]-x[kk-1];
		    vay=y[kk]-y[kk-1];
		    vaz=z[kk]-z[kk-1];
		    lengtha=sqrt(vax*vax+vay*vay+vaz*vaz);
		    if(lengtha){
		    	nvax=vax/lengtha;
		    	nvay=vay/lengtha;
		    	nvaz=vaz/lengtha;		    
			}else{
			   continue;
			}
			
		    vbx=x[kk+1]-x[kk];
		    vby=y[kk+1]-y[kk];
		    vbz=z[kk+1]-z[kk];
		    lengthb=sqrt(vbx*vbx+vby*vby+vbz*vbz);
		    if(lengthb){
		    	nvbx=vbx/lengthb;
		    	nvby=vby/lengthb;
		    	nvbz=vbz/lengthb;		    
			}else{
			   continue;
			}
					    
			vyx[kk]=nvay*nvbz-nvby*nvaz;
			vyy[kk]=-(nvax*nvbz-nvbx*nvaz);
			vyz[kk]=nvax*nvby-nvbx*nvay;
		    length=sqrt(vyx[kk]*vyx[kk]+vyy[kk]*vyy[kk]+vyz[kk]*vyz[kk]);
		    
		    
		    if(length < 1e-10){
		    	straight[kk]=TRUE;
		    }else{
		        straight[kk]=FALSE;
		    }
		    
		    
		    
			vxx[kk]=0.5*(nvbx-nvax);
			vxy[kk]=0.5*(nvby-nvay);
			vxz[kk]=0.5*(nvbz-nvaz);
		    len=sqrt(vxx[kk]*vxx[kk]+vxy[kk]*vxy[kk]+vxz[kk]*vxz[kk]);
		    
		    sprintf(WarningBuff,"%ld %g %g",(long)kk,length,len);
		    WarningBatchFast(WarningBuff);
		    
	        if(len){
				vyx[kk] /= length;
				vyy[kk] /= length;
				vyz[kk] /= length;
				vxx[kk] /= len;
				vxy[kk] /= len;
				vxz[kk] /= len;
	        }else{
	           /* straight line */	           
		        if(fabs(vax) > fabs(vay)){
		            if(fabs(vax) > fabs(vaz)){
		                /* x largest */
						vyx[kk]=0;
						vyy[kk]=0;
						vyz[kk]=1;
						
						vxx[kk]=0;
						vxy[kk]=1;
						vxz[kk]=0;	           
		            }else{
		                /* z largest */
						vyx[kk]=0;
						vyy[kk]=1;
						vyz[kk]=0;
						
						vxx[kk]=1;
						vxy[kk]=0;
						vxz[kk]=0;	           
		            }
		        }else if(fabs(vay) > fabs(vaz)){
		               /* y largest */
						vyx[kk]=0;
						vyy[kk]=0;
						vyz[kk]=1;
						
						vxx[kk]=1;
						vxy[kk]=0;
						vxz[kk]=0;	           
		        }else{
		               /* z largest */
						vyx[kk]=0;
						vyy[kk]=1;
						vyz[kk]=0;
						
						vxx[kk]=1;
						vxy[kk]=0;
						vxz[kk]=0;	           
		        }
	        }
	        
		}
		
		{
		    long kk1,kk2;
		    
		    kk1=0;
		    for(kk=1;kk<length1;++kk){
		        if(!straight[kk]){
		            for(kk2=kk1;kk2<kk;++kk2){
						vyx[kk2]=vyx[kk];
						vyy[kk2]=vyy[kk];
						vyz[kk2]=vyz[kk];
						
						vxx[kk2]=vxx[kk];
						vxy[kk2]=vxy[kk];
						vxz[kk2]=vxz[kk];	
						
		            }
					kk1=kk+1;           		            
		        }
		    }
		    
		    kk1=length1-1;
		    for(kk=length1-1;kk>=0;--kk){
		        if(!straight[kk]){
		            for(kk2=kk1;kk2>kk;--kk2){
						vyx[kk2]=vyx[kk];
						vyy[kk2]=vyy[kk];
						vyz[kk2]=vyz[kk];
						
						vxx[kk2]=vxx[kk];
						vxy[kk2]=vxy[kk];
						vxz[kk2]=vxz[kk];	
						
		            }
					kk1=kk-1;           		            
		        }
		    }


		}
		
		vyx[length1-1]=vyx[length1-2];
		vyy[length1-1]=vyy[length1-2];
		vyz[length1-1]=vyz[length1-2];
		
		vxx[length1-1]=vxx[length1-2];
		vxy[length1-1]=vxy[length1-2];
		vxz[length1-1]=vxz[length1-2];	           
		
		for(kk=0;kk<length1;++kk){
		    vax=x[kk]-x[kk-1];
		    vay=y[kk]-y[kk-1];
		    vaz=z[kk]-z[kk-1];
		    length=sqrt(vax*vax+vay*vay+vaz*vaz);
		    if(kk > 0 && length){
		        double d1,d2;
		        vax /= length;
		        vay /= length;
		        vaz /= length;
		        d1=vax*vxx[kk]+vay*vxy[kk]+vaz*vxz[kk];
		        d2=vax*vyx[kk]+vay*vyy[kk]+vaz*vyz[kk];
		        sprintf(WarningBuff,"(%ld %g %g)   %g %g %g   %g %g %g",(long)kk,d1,d2,vxx[kk],vxy[kk],vxz[kk],vyx[kk],vyy[kk],vyz[kk]);
		    }else{
		        sprintf(WarningBuff,"%ld %g %g %g %g   %g %g %g ",(long)kk,vxx[kk],vxy[kk],vxz[kk],vyx[kk],vyy[kk],vyz[kk]);
		    }
		    WarningBatchFast(WarningBuff);
		}
		WarningBatchAtEnd(WarningBuff);        
		
				    
		for(kk=1;kk<length1-1;++kk){
		    
		    vax=x[kk]-x[kk-1];
		    vay=y[kk]-y[kk-1];
		    vaz=z[kk]-z[kk-1];
		    
		    for(j=0;j<c->ny;++j){
		        for(i=0;i<c->nx;++i){
		           dx=x[kk]+j*dumy*vax+c->origin.x+rtop*vxx[kk]*cos(2*pi*(1.-i*dumx))+rtop*vyx[kk]*sin(2*pi*(1.-i*dumx));
		           dy=y[kk]+j*dumy*vay+c->origin.y+rtop*vxy[kk]*cos(2*pi*(1.-i*dumx))+rtop*vyy[kk]*sin(2*pi*(1.-i*dumx));
		           dz=z[kk]+j*dumy*vaz+c->origin.z+rtop*vxz[kk]*cos(2*pi*(1.-i*dumx))+rtop*vyz[kk]*sin(2*pi*(1.-i*dumx));
		           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		           vdata[k]=e[kk];
		           ++k;
		        }
		    }
		    for(j=0;j<c->ny-1;++j){
		        for(i=0;i<c->nx-1;++i){
				    ndata[nel++]=c->Material;
				    ndata[nel++]=i+j*c->nx+ks;
				    ndata[nel++]=i+1+j*c->nx+ks;
				    ndata[nel++]=i+1+(j+1)*c->nx+ks;
				    ndata[nel++]=i+(j+1)*c->nx+ks;
		        }
		    }
		    
		    if(ks > 0 && (nl == n)){
	    	   nn=ks;
			   for(i=0;i<c->nx-1;++i){
			       dx=0.5*(xdatal[nn+i]+xdatal[nn+i-c->nx]);
		           xdatal[nn+i]=dx;
		           xdatal[nn+i-c->nx]=dx;

			       dy=0.5*(ydatal[nn+i]+ydatal[nn+i-c->nx]);
		           ydatal[nn+i]=dy;
		           ydatal[nn+i-c->nx]=dy;

			       dz=0.5*(zdatal[nn+i]+zdatal[nn+i-c->nx]);
		           zdatal[nn+i]=dz;
		           zdatal[nn+i-c->nx]=dz;
	           }
		    }
		    ks=k;
		    nl=n;
		}
doContinue:		
		if(x)cFree((char *)x);
		x=NULL;		
		if(y)cFree((char *)y);
		y=NULL;		
		if(z)cFree((char *)z);
		z=NULL;		
		if(e)cFree((char *)e);
		e=NULL;		
	}
	    	    
	    nel /= 5;
	    

	    if(0 && gdata){
	        double den,th;

			VecCopy(scene->Eyel,Eyel);
			VecCopy(scene->Lampl,Lampl);

	    	den=rtop-rbottom;
	    	if(den == 0.0){
	    	    dz=0.0;
	    	}else{
	    	    th=atan2(1.0,length/den);
	    	    dz=sin(th);
	    	}	               
	    	k=0;
	        for(j=0;j<c->ny;++j){
			    for(i=0;i<c->nx;++i){
			        Vec P,N,D,L;
			        double len;
			        
	               dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(1.-i*dumx));
	               dy=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(1.-i*dumx));
				        
					MakeVector(xdatal[k],ydatal[k],zdatal[k],P);
						
					MakeVector((dx)*v1x+(dy)*v1y+(dz)*v1z,(dx)*v2x+(dy)*v2y+(dz)*v2z,
						           (dx)*v3x+(dy)*v3y+(dz)*v3z,N);
						
					/* Vector Eye To Point */
						
			    		VecSub(P,Eyel,D);
			    		
					/* Check Direction To Eye Point */

					if((VecDot(D,N)) > 0.0){
					     VecNegate(N);
					}


					/*  Vector from Point to Lamp */

					VecSub(Lampl,P,L);
						
					/* Check Direction of Lamp */


					 if((VecDot(L,N)) > 0.0){
					     if((len=VecLen(N)) > 0.0){
					         N[0] /= len;
					         N[1] /= len;
					         N[2] /= len;
					     }
					     if((len=VecLen(L)) > 0.0){
					         L[0] /= len;
					         L[1] /= len;
					         L[2] /= len;
					     }
					     gdata[k]=VecDot(N,L);
					 }else{
					     gdata[k]=0.;
					 }
					 ++k;
			    }
	    	}
			(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	    }else if(0 && nvdata){
	        double den,th;

			VecCopy(scene->Eyel,Eyel);
			VecCopy(scene->Lampl,Lampl);

	    	den=rtop-rbottom;
	    	if(den == 0.0){
	    	    dz=0.0;
	    	}else{
	    	    th=atan2(1.0,length/den);
	    	    dz=sin(th);
	    	}	               
	    	k=0;
	        for(j=0;j<c->ny;++j){
			    for(i=0;i<c->nx;++i){
					dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(1.-i*dumx));
					dy=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(1.-i*dumx));

					nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
					nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
					nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			    }
	    	}
	    	if(rdata && sdata){
			    dx=0;
			    if(c->nx > 1)dx=1.0/((double)(c->nx-1));
			    dy=0;
			    if(c->ny > 1)dy=1.0/((double)(c->ny-1));
	    	    k=0;
	            for(j=0;j<c->ny;++j){
			        for(i=0;i<c->nx;++i){
					    rdata[k]=(float)(i*dx);
					    sdata[k++]=(float)(j*dy);
			        }
	    	    }
	        }
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}

	*CellCount += nel;
    c->FirstCell = *CellCount;

	return 0;
}
#endif
