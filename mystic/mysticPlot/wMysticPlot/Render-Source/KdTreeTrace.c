#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Render.h"
#include "Vrml.h"
#include "Defs.h"

static int SaveObject2(Object *o,int flag);

int ListObjectMessage(CObjectListPtr o,struct ObjectMessage *Message);

int ListGetMost(CObjectList *d,long *MostNodes,long *MostNormals,long *MostCells);

int ListObjectDraw(CObjectListPtr ObjectList,long *CellCount,int flag);

int doImageWrite(renderPtr openData,struct Scene *scene,float *vbuff);
int oDumpit(renderPtr openData);

int doImage(struct Scene *scene);
void ScaleMatrix2(struct Matrix *m,struct System *p);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
int SetShadow(struct Scene *scene);
int SetScreen(struct Scene *scene);
int SetMaterialStuff(struct Scene *scene);

struct P Norm(struct P *p1);
struct P CrossN(struct P *v1,struct P *v2);
struct P Sub(struct P *p1,struct P *p2);
struct P Add(struct P *p1,struct P *p2);
struct P Mult(struct P *p1,double Factor);
double Len(struct P *v1);
double Dot(struct P *v1,struct P *v2);

int getPaletteReal(struct Color *ct,int n);

static int doAddShadow4(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
				        long nel,long CellCount,struct Scene *scene);

static int doAddShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene);

static int doAddShadow4G(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,double *gdata,
		long nel,long CellCount,struct Scene *scene);

static int doAddShadow(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
			   long nel,long CellCount,struct Scene *scene);

static int doAddShadowG(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,double *gdata,
		long nel,long CellCount,struct Scene *scene);
		
static int doAddShadowP(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                float *nvdata,long nel,long CellCount,struct Scene *scene);
int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,
                double *vdata,long nel,long CellCount,struct Scene *scene);
static int doAddShadowVRML(long *ndata,long *matdata,float *normal,long normmax,
 		     long *normlist,long normlistmax,
                     double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS);
int doVrmlTexture(ColorVEC surf_color,double s1x,double s2x,long ns,long ip,struct tState *tS);

int KdTreeTrace(renderPtr openData);


static struct renderData rdl;

static int doImageSlow(struct Scene *scene);

static int doSlowPlot(renderPtr openData);

static int doEndObjects(struct Scene *scene);

struct Raydata{
	void    *traverseStack;
	long bestObject;
	Isect nhits ;
	Ray *ray;
	double min_dist;
};

static Vec	Slab[] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0},
	{0.0, 1.0, 1.0}
} ;

static int NullPrint(void) ;
static int NullIntersect(void) ;
static int NullNormal(void) ;


static ObjectProcs NullProcs = {
	NullPrint,
	NullIntersect,
	NullNormal
} ;
int CleanTracing2(struct Scene *scene);
static int Trace(int level, Flt weight, Ray *ray, Color color);
static int TraceIt(mThread *Threads);
static int AllocatedTracing(void);
static int startLights(struct Scene *scene);
static int SetClipPlane(struct Scene *scene);

static int BuildKdTree(struct Scene *scene);

int potHitSeekIntersection(int numberOfObjects, long *objectNums, double *retStopDistance, void *useData );

struct Scene *sceneLocal;

static void myExtentFunc( long objNum, AABB *retBox );
static int  myExtentsInBox( long objNum, AABB *aabb, AABB *retBox);

static int Shadow(Ray *ray, Flt tmax);

static int Intersect(Ray * ray, Isect * hit);

int potHitSeekIntersection(int numberOfObjects, long *objectNums, double *retStopDistance, void *useData);

struct Material KdTreeMaterial;

int potHitSeekIntersection(int numberOfObjects, long *objectNums, double *retStopDistance, void *useData) 
{
	struct Scene *scene=sceneLocal;
	Object *cobj,**Prims;
	long n,objNum,nPrims;
	static int error;
	Isect nhit ;
	int ret;
	Vec P,D;
	
	struct Raydata *r;
	
	r=(struct Raydata *)useData;
	
	if(!r)return FALSE;
	
	Prims=rd->Prims;
	if(!Prims)return FALSE;
	
	nPrims=rd->nPrims;
	
	ret=FALSE;
	for(n=0;n<numberOfObjects;++n){
	    objNum=objectNums[n];
		if(objNum < 0 || objNum >= nPrims){
			if(error++ < 100)WarningBatch("potHitSeekIntersection Out of Range\n");
			continue;
		}
		cobj=Prims[objNum];
		if(!cobj){
			if(error++ < 100)WarningBatch("potHitSeekIntersection NULL\n");
			continue;
		}
		
		if ((cobj -> o_procs -> intersect) (cobj, r->ray, &nhit)) {
			if (nhit.isect_t < r->min_dist) {
				RayPoint(r->ray, nhit.isect_t, P);
				if(scene->ClipPlane){
		    		VecSub(P,rd->ClipPlanePoint,D);
				     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
				         continue;
				     }
				}
				r->min_dist = nhit.isect_t ;
				r->bestObject = objNum;
				r->nhits=nhit;
				ret=TRUE;
				*retStopDistance = VecLen(P);
			}
		}
	
	}
	
	return ret;
}

static int SetClipPlane(struct Scene *scene)
{
	struct P ClipPlanePoint;
	struct P EyeToTarget;
	
	if(!scene)return 1;

	if(scene->ClipPlane == ClipPlane_Eye){
	
	  	EyeToTarget=Sub(&scene->Target,&scene->Eye);
		EyeToTarget=Norm(&EyeToTarget);
	     
	    ClipPlanePoint.x=scene->Eye.x+scene->ClipPlaneDistance*EyeToTarget.x;
	    ClipPlanePoint.y=scene->Eye.y+scene->ClipPlaneDistance*EyeToTarget.y;
	    ClipPlanePoint.z=scene->Eye.z+scene->ClipPlaneDistance*EyeToTarget.y;

		MakeVector(ClipPlanePoint.x,ClipPlanePoint.y,ClipPlanePoint.z,rd->ClipPlanePoint);	
		
		if(scene->ClipPlaneAngle){
	 		double t,st,ct;
	 		t=scene->ClipPlaneAngle*4.0*atan(1.0)/180.;
	 		st=sin(t);
	 		ct=cos(t);
		
	    	rd->ClipPlaneNormal[0]= -(st*scene->g.V2.x-ct*scene->g.V3.x);
	    	rd->ClipPlaneNormal[1]= -(st*scene->g.V2.y-ct*scene->g.V3.y);
	    	rd->ClipPlaneNormal[2]= -(st*scene->g.V2.z-ct*scene->g.V3.z);
		}else{       
	    	rd->ClipPlaneNormal[0]= -EyeToTarget.x;
	    	rd->ClipPlaneNormal[1]= -EyeToTarget.y;
	    	rd->ClipPlaneNormal[2]= -EyeToTarget.z;
	    }
	     
	}else{		
		MakeVector(scene->ClipPlanePoint.x,scene->ClipPlanePoint.y,scene->ClipPlanePoint.z,rd->ClipPlanePoint);
		MakeVector(scene->ClipPlaneNormal.x,scene->ClipPlaneNormal.y,scene->ClipPlaneNormal.z,rd->ClipPlaneNormal);
	}
	
	return 0;

}

static void myExtentFunc(long objNum, AABB *retBox)
{
	struct Scene *scene=sceneLocal;
	
	if(!scene)return;
	
	if(objNum < 0 || objNum >= rd->nPrims)return;
	
	if(rd->Prims[objNum]){
	   Object *o;
	    o=rd->Prims[objNum];
	    if(o){
			retBox->BoxMin.x=o->o_dmin[0];
			retBox->BoxMax.x=o->o_dmax[0];

			retBox->BoxMin.y=o->o_dmin[1];
			retBox->BoxMax.y=o->o_dmax[1];

			retBox->BoxMin.z=o->o_dmin[2];
			retBox->BoxMax.z=o->o_dmax[2];
		}
	}
	
	return;
}
static int myExtentsInBox(long objNum, AABB *aabb, AABB *retBox)
{
	VectorR3 an;
	VectorR3 ax;

	VectorR3 bn;
	VectorR3 bx;

	myExtentFunc(objNum,retBox);

	an = retBox->BoxMin;
	ax = retBox->BoxMax;

	bn = aabb->BoxMin;
	bx = aabb->BoxMax;
	
	if(bn.x > an.x)an.x=bn.x;
	if(bn.y > an.y)an.y=bn.y;
	if(bn.z > an.z)an.z=bn.z;

	if(bx.x < ax.x)ax.x=bx.x;
	if(bx.y < ax.y)ax.y=bx.y;
	if(bx.z < ax.z)ax.z=bx.z;


	return !(      ax.x < an.x 
				|| ax.y < an.y 
				|| ax.z < an.z );

}

static int BuildKdTree(struct Scene *scene)
{
	if(!scene)return 1;
	
	sceneLocal=scene;
	
	KdTreeInit(&scene->ObjectKdTree);
	KdTreeSetObjectCost (&scene->ObjectKdTree, 8.0);
	KdTreeBuildTree(&scene->ObjectKdTree, rd->nPrims, myExtentFunc, myExtentsInBox);
	
	return 0;
}

int KdTreeTrace(renderPtr openData)
{
	struct Scene *scene;
	long FixRayObjects,length;

	if(!openData)return 1;
	
	scene=openData->scene;

	if(!scene || !scene->RayTracing)return 1;
	
	rd = &rdl;

	sceneLocal=scene;

	FixRayObjects=scene->FixRayObjects;
		
	if(AllocatedTracing())FixRayObjects=0;
	
	if(!FixRayObjects){
		CleanTracing2(scene);
	
		zerol((char *)&rdl,sizeof(rdl));
		
		rdl.Slab=Slab;
		rdl.rayeps=1e-6;
		rdl.maxlevel=5;
		rdl.Progname="kdTrace";
		rdl.NullProcs=NullProcs;
		rdl.Shadow=Shadow;
		
		rd->popStack=pushpopStart(&rd->States);
		if(!rd->popStack)goto ErrorOut;
		
	}
	
	SetClipPlane(scene);	
			
	if(startLights(scene))goto ErrorOut;

	length=3*scene->xResolution*scene->yResolution;
	if((rd->vbuff=(float *)cMalloc(length*sizeof(float),8636)) == NULL){
	    sprintf(WarningBuff,"  vbuff Out of memory Requested (%ld)\n",length*sizeof(float));
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
		
	scene->g.Ambient= .15;
	
	scene->doNoShadow=doAddShadow;
	scene->doNoShadowG=doAddShadowG;
	scene->doNoShadowP=doAddShadowP;
	scene->doNoShadow4=doAddShadow4;
	scene->doNoShadow4P=doAddShadow4P;
	scene->doNoShadow4G=doAddShadow4G;
	scene->doNoShadowVRML=doAddShadowVRML;
		
	
	if(!FixRayObjects){
	    if(doSlowPlot(openData))goto ErrorOut;
	}

	runThreads(scene->threads,scene,scene->yResolution,TraceIt);
		
ErrorOut:


	if(!scene->FixRayObjects)CleanTracing2(scene);
		
	return 0;
}


static int doSlowPlot(renderPtr openData)
{
	long length,n;
	struct Scene *scene;

	scene=openData->scene;
	
	rd->zbuff=NULL;

	MakeVector(0.,0.,scene->Height,rd->Eyel);
	
	length=scene->xResolution*scene->yResolution;

	rd->zbuff=scene->zbuff;
		
	for(n=0;n<length;++n){
	    rd->zbuff[n] = (-1.0e30);
	    scene->CellBuff[n] = -1;
	}
	
	ColorToVec(scene->BackGround,rd->BackgroundColor);

	return doImageSlow(scene);

}

static int doImageSlow(struct Scene *scene)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata;
	float *sdata;
	double *vdata;
	long *cdata;
	double delx,dely;
	struct P Lamp;
	struct System Global;
	struct System Screen;
	struct Matrix GlobalToScreen;
	struct Matrix ScreenToGlobal;
	CObjectListPtr ObjectList;
	long CellCount;
	int Transparency;
	long length;
	int ret;
	
	ret=1;
	
	Transparency=(int)scene->Transparency;
	ObjectList=scene->ObjectList;

	scene->MostNodes=0;
	scene->MostNormals=0;
	scene->MostCells=0;

	rd->zbuff2=NULL;
	rd->vbuff2=NULL;
	rd->cellbuff2=NULL;
	rd->opacity=NULL;

	xdatal=NULL;
	ydatal=NULL;
	zdatal=NULL;
	gdata=NULL;
	cdata=NULL;
	nvdata=NULL;
	rdata=NULL;
	sdata=NULL;
	vdata=NULL;

	RenderSceneSetTime(scene);

	ListGetMost(ObjectList,&scene->MostNodes,&scene->MostNormals,&scene->MostCells);

	if(!scene->SmoothNormals)scene->MostNormals=0;

	if(scene->MostNodes == 0){
	    return 1;
	}

	if(scene->SmoothNormals == 1){
	    if((gdata=(double *)cMalloc(scene->MostNodes*sizeof(double),8637)) == NULL){
			sprintf(WarningBuff,"doImageSlow gdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}else if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if((nvdata=(float *)cMalloc(3*scene->MostNodes*sizeof(float),8638)) == NULL){
			sprintf(WarningBuff,"doImageSlow nvdata Out of memory Requested (%ld)\n",3*scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rdata=(float *)cMalloc(scene->MostNodes*sizeof(float),8639)) == NULL){
			sprintf(WarningBuff,"doImageSlow rdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((sdata=(float *)cMalloc(scene->MostNodes*sizeof(float),8640)) == NULL){
			sprintf(WarningBuff,"doImageSlow sdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}

	if(scene->MostCells){
	    if((cdata=(long *)cMalloc(scene->MostCells*sizeof(long),8641)) == NULL){
			sprintf(WarningBuff,"doImageSlow cdata Out of memory Requested (%ld)\n",scene->MostCells*sizeof(long));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}

	length=scene->xResolution*scene->yResolution;
	if(Transparency){	
	    if((rd->zbuff2=(double *)cMalloc(length*sizeof(double),8642)) == NULL){
			sprintf(WarningBuff,"doImageSlow zbuff2 Out of memory Requested (%ld)\n",length*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rd->vbuff2=(float *)cMalloc(3*length*sizeof(float),8643)) == NULL){
			sprintf(WarningBuff,"doImageSlow vbuff2 Out of memory Requested (%ld)\n",3*length*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rd->cellbuff2=(long *)cMalloc(length*sizeof(long),8644)) == NULL){
			sprintf(WarningBuff,"doImageSlow cellbuff2 Out of memory Requested (%ld)\n",length*sizeof(long));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rd->opacity=(float *)cMalloc(length*sizeof(float),8645)) == NULL){
			sprintf(WarningBuff,"doImageSlow opacity Out of memory Requested (%ld)\n",length*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}
	if((xdatal=(double *)cMalloc(scene->MostNodes*sizeof(double),8646)) == NULL){
	    sprintf(WarningBuff,"doImageSlow  xdatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if((ydatal=(double *)cMalloc(scene->MostNodes*sizeof(double),8647)) == NULL){
	    sprintf(WarningBuff,"doImageSlow ydatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if((zdatal=(double *)cMalloc(scene->MostNodes*sizeof(double),8648)) == NULL){
	    sprintf(WarningBuff,"doImageSlow zdatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if((vdata=(double *)cMalloc(scene->MostNodes*sizeof(double),8649)) == NULL){
	    sprintf(WarningBuff,"doImageSlow zdatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	
	Global=scene->Global;
	Screen.p=scene->g.pScreen;
	Screen.x=scene->g.V1;
	Screen.y=scene->g.V2;
	Screen.z=scene->g.V3;
	Screen=Global;
	
	BuildTransformMatrix(&GlobalToScreen,&Global,&Screen);
	BuildTransformMatrix(&ScreenToGlobal,&Screen,&Global);
	
	scene->GlobalToScreen=GlobalToScreen;
	scene->ScreenToGlobal=ScreenToGlobal;
	
	/* transform to local coordinate system */
	
	Lamp=scene->g.Lamp;
		
	xc=scene->g.pScreen.x;
	yc=scene->g.pScreen.y;
	zc=scene->g.pScreen.z;
	
	v1x=scene->g.V1.x;
	v1y=scene->g.V1.y;
	v1z=scene->g.V1.z;
	
	v2x=scene->g.V2.x;
	v2y=scene->g.V2.y;
	v2z=scene->g.V2.z;
	
	v3x=scene->g.V3.x;
	v3y=scene->g.V3.y;
	v3z=scene->g.V3.z;
	
	MakeVector((Lamp.x-xc)*v1x+(Lamp.y-yc)*v1y+(Lamp.z-zc)*v1z,
		   (Lamp.x-xc)*v2x+(Lamp.y-yc)*v2y+(Lamp.z-zc)*v2z,
		   (Lamp.x-xc)*v3x+(Lamp.y-yc)*v3y+(Lamp.z-zc)*v3z,rd->Lampl);

	
	if(scene->AutoFullScreen){
        struct ObjectMessage Message;
        double ztran;
	    double dx,dy,dd;
	   
        Message.MessageType=OBJECT_MESSAGE_RANGE_DATA_SCREEN;
        ListObjectMessage(ObjectList,&Message);
	   
	    ztran = 1.0f/(1-Message.zmax);
	    rd->xmin=Message.xmin*ztran;
	    rd->xmax=Message.xmax*ztran;
	    rd->ymin=Message.ymin*ztran;
	    rd->ymax=Message.ymax*ztran;

		dx=rd->xmax-rd->xmin;
		dy=rd->ymax-rd->ymin;
		if(dx > dy){
		    dd=dx-dy;
		    rd->ymin -= .5f*dd;
		    rd->ymax += .5f*dd;
		}else{
		    dd=dy-dx;
		    rd->xmin -= .5f*dd;
		    rd->xmax += .5f*dd;
		}
		
	}else{
        rd->ymax = (double)(scene->Height*tan((scene->Degrees*4.0*atan(1.0))/360.));
	    rd->ymin = -rd->ymax;

        rd->xmax = (double)(rd->ymax*scene->AspectRatio*(double)(scene->xResolution-1)/(double)(scene->yResolution-1));
	    rd->xmin = - rd->xmax;
	}


	delx=rd->xmax-rd->xmin;
	dely=rd->ymax-rd->ymin;

	if(scene->debug > 2){
	    sprintf(WarningBuff,"raster xmin %f xmax %f delx %f\n",rd->xmin,rd->xmax,delx);
		WarningBatch(WarningBuff);
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"raster ymin %f ymax %f dely %f\n",rd->ymin,rd->ymax,dely);
		WarningBatch(WarningBuff);
	}

	rd->odx=((double)(scene->xResolution-1)/delx);	
	rd->ody=((double)(scene->yResolution-1)/dely);	
	
	scene->delx=delx;
	scene->dely=dely;
	
	scene->odx=rd->odx;
	scene->ody=rd->ody;
	
	scene->xmin=rd->xmin;
	scene->ymin=rd->ymin;
	
	scene->xdatal=xdatal;
	scene->ydatal=ydatal;
	scene->zdatal=zdatal;
	scene->gdata=gdata;
	scene->cdata=cdata;
	scene->nvdata=nvdata;
	scene->rdata=rdata;
	scene->sdata=sdata;
	scene->vdata=vdata;

	VecCopy(rd->Lampl,scene->Lampl);
	VecCopy(rd->Eyel,scene->Eyel);

	rd->doTransparency=FALSE;

	CellCount=0;
	ListObjectDraw(ObjectList,&CellCount,0);

	if(doEndObjects(scene))goto ErrorOut;
	
	if(InitSlabs())goto ErrorOut;
	BuildKdTree(scene);

	ret=0;
		
ErrorOut:
	if(nvdata)cFree((char *)nvdata);
	if(gdata)cFree((char *)gdata);
	if(xdatal)cFree((char *)xdatal);
	if(ydatal)cFree((char *)ydatal);
	if(zdatal)cFree((char *)zdatal);
	if(cdata)cFree((char *)cdata);
	if(rd->zbuff2)cFree((char *)rd->zbuff2);
	if(rd->vbuff2)cFree((char *)rd->vbuff2);
	if(rd->cellbuff2)cFree((char *)rd->cellbuff2);
	if(rd->opacity)cFree((char *)rd->opacity);
	if(rdata)cFree((char *)rdata);
	if(sdata)cFree((char *)sdata);
	if(vdata)cFree((char *)vdata);
	
	nvdata=NULL;
	gdata=NULL;
	xdatal=NULL;
	ydatal=NULL;
	zdatal=NULL;
	cdata=NULL;
	rd->zbuff2=NULL;
	rd->vbuff2=NULL;
	rd->cellbuff2=NULL;
	rd->opacity=NULL;
	rdata=NULL;
	sdata=NULL;
	nvdata=NULL;
	vdata=NULL;
	
	return ret;
}
static int AllocatedTracing(void)
{
	rd = &rdl;
	
	if(!rd->popStack)return 1;
	
	if(!rd->Prims)return 1;
	
	if(rd->nPrims <= 0)return 1;
	
	return 0;
}
static int startLights(struct Scene *scene)
{
	int k,ret;
	
	ret=1;
	
	if(!scene)goto ErrorOut;

	rd = &rdl;
		
	rd->Lights=scene->Lights;
		
	rd->nLights=scene->nLights;

	if(scene->Lights){
	    scene->Lights[0].light_pos[0]=scene->lamp.origin.x;
	    scene->Lights[0].light_pos[1]=scene->lamp.origin.y;
	    scene->Lights[0].light_pos[2]=scene->lamp.origin.z;
	}
	
	{
		struct ObjectMessage Message;
		double delta;
		int nSeeLamps;
		
		Message.MessageType=OBJECT_MESSAGE_RANGE_LIGHTS;
		ListObjectMessage(scene->ObjectList,&Message);
	
		nSeeLamps=0;
		for (k = 0 ; k < rd->nLights ; k++) {
			if(rd->Lights[k].type == 0)
			{
				++nSeeLamps;
			}
		}
		
		if(nSeeLamps <= 0)
		{
			nSeeLamps=1;
		}
		
		for (k = 0 ; k < rd->nLights ; k++) {
	
			rd->Lights[k].light_brightness =  
								rd->Lights[k].light_bright*sqrt((Flt)nSeeLamps) /
								((Flt)nSeeLamps);
			rd->Lights[k].vmin=Message.vmin;
			rd->Lights[k].vmax=Message.vmax;
			rd->Lights[k].vdiff=Message.vmax-Message.vmin;
		}
	
		KdTreeMaterial.ColorCount=192;
		delta=(log10(Message.vmax)-log10(Message.vmin))/192;
		for(k=0;k<192;++k)
		{
			KdTreeMaterial.ColorLevel[k]=log10(Message.vmin)+k*delta;
		}
		
		getPaletteReal(&KdTreeMaterial.ColorValue[0],12);
		
		for(k=0;k<192;++k)
		{			
			KdTreeMaterial.ColorValue[k].red=KdTreeMaterial.ColorValue[k+63].red;
			KdTreeMaterial.ColorValue[k].green=KdTreeMaterial.ColorValue[k+63].green;
			KdTreeMaterial.ColorValue[k].blue=KdTreeMaterial.ColorValue[k+63].blue;
		}
		
		
    }
	
	ret=0;
ErrorOut:
	return ret;
}
int CleanTracing2(struct Scene *scene)
{
	int n;

	rd = &rdl;
	

	if(rd->popStack){
		while(1){
		    rd->States=NULL;
	 		if(pushpopPop(&rd->States,rd->popStack))break;
	 		if(rd->States)cFree((char *)rd->States);

		}
		pushpopEND(rd->popStack);
		
		rd->popStack=NULL;
	}
	
	if(rd->Prims){
	    for(n=0;n<rd->nPrims;++n){	
		    if(rd->Prims[n]){
		        if(rd->Prims[n]->o_data)cFree((char *)rd->Prims[n]->o_data);
		    	cFree((char *)rd->Prims[n]);
		    }
		}
		cFree((char *)rd->Prims);
	}
	
	rd->Prims=NULL;
		
	rd->Lights=NULL;
	
	if(scene){
		KdTreeDestroy(&scene->ObjectKdTree);
		scene->ObjectKdTree.TreeBuilt=FALSE;
	}
		
	return 0;
}
static int doEndObjects(struct Scene *scene)
{
	int ret;
	
	if(!scene)return 1;
	
	ret=1;
	
	/* if(SaveObject2((Object *)1,-1))goto ErrorOut; */
	
	ret=0;
/*
ErrorOut:
*/
	if(ret){
	    WarningBatch("doEndObjects Out of Memory\n");
	}
	return ret;
}

static int TraceIt(mThread *Threads)
{
	double yRange,yRange2,dy,dx,pi,xStart,yStart,zStart,xRange;
	struct stackKD tStack;
	struct Scene *scene;
	Color color,csum;
	int xResolution;
	struct P p,Eye;
	int JitterCount;
	int i,j,jn;
	float *dp;
	Ray ray ;
	
	if(!Threads)goto ErrorOut;
	scene=(struct Scene *)Threads->data;
	if(!scene)goto ErrorOut;
	
	zerol((char *)&ray,sizeof(ray));
	
	tStack.pop=dataStackStart(&tStack.td);
	if(!tStack.pop)goto ErrorOut;		
	
	ray.traverseStack=(void *)&tStack;
	
	xResolution=(int)scene->xResolution;
	
	pi=4.0*atan(1.0);
	
	yRange=scene->Height*tan((scene->Degrees*pi)/360.);
	yRange2=2.0*yRange;
	dy=(yRange2)/((double)scene->yResolution-1);
	dx=dy*scene->AspectRatio;
	
	xRange=scene->AspectRatio*((double)scene->xResolution-1)/((double)scene->yResolution-1);
	
	xStart=scene->g.pScreen.x-yRange*(scene->g.V1.x*xRange+scene->g.V2.x);
	yStart=scene->g.pScreen.y-yRange*(scene->g.V1.y*xRange+scene->g.V2.y);
	zStart=scene->g.pScreen.z-yRange*(scene->g.V1.z*xRange+scene->g.V2.z);
		
	ray.P[0]=scene->Eye.x;
	ray.P[1]=scene->Eye.y;
	ray.P[2]=scene->Eye.z;
	
	Eye.x=scene->Eye.x;
	Eye.y=scene->Eye.y;
	Eye.z=scene->Eye.z;
    
    
    if(scene->Jitter){
		if(scene->JitterCount <= 0)scene->JitterCount=1;        
   	 	JitterCount=(int)scene->JitterCount;
    }else{
   	 	JitterCount=1;
    }
    
	dp=(float *)rd->vbuff;
	dp=dp+Threads->smin*xResolution*3;
	for(j=Threads->smin;j<Threads->smax;++j){
	    for(i=0;i<xResolution;++i){
			double idx1,jdy1;

			double dx1,dy1;
				
			MakeVector(0.0, 0.0, 0.0, csum);
						
			for(jn=0;jn<JitterCount;++jn){
				if(scene->Jitter){
					dx1=(((double)rand()/(double)RAND_MAX)-0.5);
					dy1=(((double)rand()/(double)RAND_MAX)-0.5);

				   idx1=(i+dx1)*dx;
				   jdy1=(j+dy1)*dy;
				}else{

				   idx1=(i)*dx;
				   jdy1=(j)*dy;
				}
		    
		        p.x=xStart+idx1*scene->g.V1.x+(yRange2-jdy1)*scene->g.V2.x;
		        p.y=yStart+idx1*scene->g.V1.y+(yRange2-jdy1)*scene->g.V2.y;
		        p.z=zStart+idx1*scene->g.V1.z+(yRange2-jdy1)*scene->g.V2.z;
		    		        	        
		        ray.D[0]=p.x-Eye.x;
		        ray.D[1]=p.y-Eye.y;
		        ray.D[2]=p.z-Eye.z;
		        
				VecNormalize(ray.D);	
		    	Trace(0, 1.0, &ray, color);		    	
		    	VecAdd(color,csum,csum);	    	
	    	}
	
			*dp++ = (float)(csum[0]/(double)JitterCount);
			*dp++ = (float)(csum[1]/(double)JitterCount);
			*dp++ = (float)(csum[2]/(double)JitterCount);
	    }
	}
		
	sprintf(WarningBuff,"TraceIt smin %ld smax %ld\n",Threads->smin,Threads->smax);
	if(scene->debug > 1)WarningBatch(WarningBuff);
	
ErrorOut:
	
	Threads->done=TRUE;

	dataStackEND(tStack.pop);

	return 0;
}
static int Trace(int level, Flt weight, Ray *ray, Color color)
{
	Object *prim ;
	Vec P, N ;
	Isect hit ;

	if (level >= rd->maxlevel || weight <= 0.0) {
		color[0] = color[1] = color[2] = 0 ;
		return 0;
	}

	if (Intersect(ray, &hit)) {
		prim = hit.isect_prim ;
		RayPoint(ray, hit.isect_t, P);
		(*prim -> o_procs -> normal) (prim, &hit, P, N);
		if ((VecDot(ray->D, N)) >= 0.0) {
			VecNegate(N);
		}
		if(prim -> o_procs -> shader){
		       (*prim -> o_procs -> shader)(prim,level, weight, P, N, ray -> D, &hit, color, ray);
		}else{
		   Shade(ray,level, weight, P, N, ray -> D, &hit, color);
		}
	} else {
		VecCopy(rd->BackgroundColor, color) ;
	}
	return 0;
}
static int Intersect(Ray * ray, Isect * hit)
{
	struct Raydata r;
	VectorR3 direction;
	VectorR3 pos;
	
	if(!ray || !hit)return 0;

	pos.x=ray->P[0];
	pos.y=ray->P[1];
	pos.z=ray->P[2];
	
	direction.x=ray->D[0];
	direction.y=ray->D[1];
	direction.z=ray->D[2];
	
	r.ray = ray;
	
	r.traverseStack = ray->traverseStack;
	
	r.min_dist=1e60;
	
	r.bestObject = -1;
	
	KdTreeTraverse(&sceneLocal->ObjectKdTree ,&pos, &direction, potHitSeekIntersection, (void *)&r,0.0, FALSE);
	
	if(r.bestObject >= 0){
		*hit=r.nhits;
		return 1;
	}
	
	return 0;
}
static int Shadow(Ray *ray, Flt tmax)
{
	Isect hit ;

	if (Intersect(ray, &hit) && hit.isect_t < tmax - rd->rayeps) {
		return(0);
	} else {
		return(1);
	}
}
#define NPRIMS 200000
static int SaveObject2(Object *o,int flag)
{
	if(!rd || !o)return 1;
	
	if((rd->nPrims + 1 > rd->maxprims) || !rd->Prims || (flag == -1)){
	    if(flag == 1){
	        sprintf(WarningBuff,"SaveObject2 Error %ld \n",rd->maxprims);
	        WarningBatch(WarningBuff);
	        return 1;
	    }
	    
	    if(rd->Prims){
		   rd->maxprims = 2*rd->maxprims+NPRIMS;
		   rd->Prims=(Object **)cRealloc((char *)rd->Prims,rd->maxprims*sizeof(Object *),8824);
		   if(!rd->Prims)return 1;
		}else{
		   rd->Prims=(Object **)cMalloc(NPRIMS*sizeof(Object *),8824);
		   if(!rd->Prims)return 1;
		   rd->maxprims=NPRIMS;
		}
		
		if(flag == -1)return 0;
	}
	
	rd->Prims[rd->nPrims++]=o;
	
	return 0;
}
static int doAddShadow4G(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 double *gdata,long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	Flt values[3];
	Surface	*s;
	Object *o;
	struct P v1,v2,norm;
	long n1,n2,n3,n4;
	long k1,k2,k3;
	long nn,nt;
	int ipass,ip;
	int ret;
	
	
	if(!ndata || !xdata || !ydata || !zdata || !scene || !gdata)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
		for(ip=0;ip<ipass;++ip){
			if(ip == 0){
			    if(ipass == 1){
			        k1=n1;
			        k2=n2;
			        k3=n3;
			    }else{
			        k1=n1;
			        k2=n2;
			        k3=n4;
			    }
			}else{
			    k1=n3;
			    k2=n4;
			    k3=n2;
			}
		    s=(Surface *)&scene->material[ndata[nt]];
			
	        points[0][0]=xdata[k1];
	        points[0][1]=ydata[k1];
	        points[0][2]=zdata[k1];

	        points[1][0]=xdata[k2];
	        points[1][1]=ydata[k2];
	        points[1][2]=zdata[k2];

	        points[2][0]=xdata[k3];
	        points[2][1]=ydata[k3];
	        points[2][2]=zdata[k3];
		
			
			v1.x=points[3]-points[0];
			v1.y=points[4]-points[1];
			v1.z=points[5]-points[2];
			v2.x=points[6]-points[0];
			v2.y=points[7]-points[1];
			v2.z=points[8]-points[2];
			
			norm=CrossN(&v1,&v2);
			normals[0][0]=norm.x;
			normals[0][1]=norm.y;
			normals[0][2]=norm.z;

			normals[1][0]=norm.x;
			normals[1][1]=norm.y;
			normals[1][2]=norm.z;

			normals[2][0]=norm.x;
			normals[2][1]=norm.y;
			normals[2][2]=norm.z;
			
			if(vdata){
	        	values[0]=vdata[k1];
	        	values[1]=vdata[k2];
	        	values[2]=vdata[k3];
				o = MakeTriNC(3,s,points,normals,values);
	        }else{
				o = MakeTri(3,s,points);
	        }
	        
			if(!o)goto ErrorOut;
						
			if(SaveObject2(o,0))goto ErrorOut;

		}
	
	}

	
	ret=0;
ErrorOut:
	if(ret){
	    WarningBatch("doAddShadow4G Out of Memory\n");
	}
	return ret;
}
static int doAddShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	Flt values[3];
	Surface	*s;
	Object *o;
	long n1,n2,n3,n4;
	long k1,k2,k3;
	long nn,nt;
	int ipass,ip;
	int ret;
	
	
	if(!ndata || !xdata || !ydata || !zdata || !scene || !nvdata)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
/*	    
	    if(CellCount+nn != 45)continue;
*/	    
		for(ip=0;ip<ipass;++ip){
			if(ip == 0){
			    if(ipass == 1){
			        k1=n1;
			        k2=n2;
			        k3=n3;
			    }else{
			        k1=n1;
			        k2=n2;
			        k3=n4;
			    }
			}else{
			    k1=n3;
			    k2=n4;
			    k3=n2;
			}
		    s=(Surface *)&scene->material[ndata[nt]];
			
	        points[0][0]=xdata[k1];
	        points[0][1]=ydata[k1];
	        points[0][2]=zdata[k1];

	        points[1][0]=xdata[k2];
	        points[1][1]=ydata[k2];
	        points[1][2]=zdata[k2];

	        points[2][0]=xdata[k3];
	        points[2][1]=ydata[k3];
	        points[2][2]=zdata[k3];
			
			normals[0][0]=nvdata[k1*3];
			normals[0][1]=nvdata[k1*3+1];
			normals[0][2]=nvdata[k1*3+2];

			normals[1][0]=nvdata[k2*3];
			normals[1][1]=nvdata[k2*3+1];
			normals[1][2]=nvdata[k2*3+2];

			normals[2][0]=nvdata[k3*3];
			normals[2][1]=nvdata[k3*3+1];
			normals[2][2]=nvdata[k3*3+2];
			
			
			if(vdata){
	        	values[0]=vdata[k1];
	        	values[1]=vdata[k2];
	        	values[2]=vdata[k3];
				o = MakeTriNC(3,s,points,normals,values);
	        }else{
				o = MakeTriN(3,s,points,normals);
	        }
			if(!o)goto ErrorOut;
			
			if(SaveObject2(o,0))goto ErrorOut;

		}
	
	}

	
	ret=0;
ErrorOut:
	if(ret){
	    WarningBatch("doAddShadow4P Out of Memory\n");
	}
	return ret;
}
static int doAddShadow4(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
				long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	Flt values[3];
	Surface	*s;
	Object *o;
	struct P v1,v2,norm;
	long n1,n2,n3,n4;
	long k1,k2,k3;
	long nn,nt;
	int ipass,ip;
	int ret;
	
	
	if(!ndata || !xdata || !ydata || !zdata || !scene)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
		for(ip=0;ip<ipass;++ip){
			if(ip == 0){
			    if(ipass == 1){
			        k1=n1;
			        k2=n2;
			        k3=n3;
			    }else{
			        k1=n1;
			        k2=n2;
			        k3=n4;
			    }
			}else{
			    k1=n3;
			    k2=n4;
			    k3=n2;
			}
		    s=(Surface *)&scene->material[ndata[nt]];
			
	        points[0][0]=xdata[k1];
	        points[0][1]=ydata[k1];
	        points[0][2]=zdata[k1];

	        points[1][0]=xdata[k2];
	        points[1][1]=ydata[k2];
	        points[1][2]=zdata[k2];

	        points[2][0]=xdata[k3];
	        points[2][1]=ydata[k3];
	        points[2][2]=zdata[k3];
						
			v1.x=points[3]-points[0];
			v1.y=points[4]-points[1];
			v1.z=points[5]-points[2];
			v2.x=points[6]-points[0];
			v2.y=points[7]-points[1];
			v2.z=points[8]-points[2];
			
			norm=CrossN(&v1,&v2);
			normals[0][0]=norm.x;
			normals[0][1]=norm.y;
			normals[0][2]=norm.z;

			normals[1][0]=norm.x;
			normals[1][1]=norm.y;
			normals[1][2]=norm.z;

			normals[2][0]=norm.x;
			normals[2][1]=norm.y;
			normals[2][2]=norm.z;
			
			
			if(vdata){
	        	values[0]=vdata[k1];
	        	values[1]=vdata[k2];
	        	values[2]=vdata[k3];
				o = MakeTriNC(3,s,points,normals,values);
	        }else{
				o = MakeTri(3,s,points);
	        }
			if(!o)goto ErrorOut;
			
			if(SaveObject2(o,0))goto ErrorOut;

		}
	
	}

	
	ret=0;
ErrorOut:
	if(ret){
	    WarningBatch("doAddShadow4 Out of Memory\n");
	}
	return ret;
}
static int doAddShadow(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
				long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	Flt values[3];
	Surface	*s;
	Object *o;
	struct P v1,v2,norm;
	long k1,k2,k3;
	long nn,nt;
	int ret;
	
	
	if(!ndata || !xdata || !ydata || !zdata || !scene)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=4*nn;
	    if((k1=ndata[nt+1]) < 0)continue;
	    if((k2=ndata[nt+2]) < 0 )continue;
	    if((k3=ndata[nt+3]) < 0)continue;
	    s=(Surface *)&scene->material[ndata[nt]];
		
        points[0][0]=xdata[k1];
        points[0][1]=ydata[k1];
        points[0][2]=zdata[k1];

        points[1][0]=xdata[k2];
        points[1][1]=ydata[k2];
        points[1][2]=zdata[k2];

        points[2][0]=xdata[k3];
        points[2][1]=ydata[k3];
        points[2][2]=zdata[k3];
		
		
		v1.x=points[3]-points[0];
		v1.y=points[4]-points[1];
		v1.z=points[5]-points[2];
		v2.x=points[6]-points[0];
		v2.y=points[7]-points[1];
		v2.z=points[8]-points[2];
		
		norm=CrossN(&v1,&v2);
		normals[0][0]=norm.x;
		normals[0][1]=norm.y;
		normals[0][2]=norm.z;

		normals[1][0]=norm.x;
		normals[1][1]=norm.y;
		normals[1][2]=norm.z;

		normals[2][0]=norm.x;
		normals[2][1]=norm.y;
		normals[2][2]=norm.z;
		
		if(vdata){
        	values[0]=vdata[k1];
        	values[1]=vdata[k2];
        	values[2]=vdata[k3];
			o = MakeTriNC(3,s,points,normals,values);
        }else{
			o = MakeTri(3,s,points);
        }
		if(!o)goto ErrorOut;
		
		if(SaveObject2(o,0))goto ErrorOut;	
	}

	
	ret=0;
ErrorOut:
	if(ret){
	    WarningBatch("doAddShadow Out of Memory\n");
	}
	return ret;
}
static int doAddShadowG(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                double *gdata,long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	Flt values[3];
	Surface	*s;
	Object *o;
	struct P v1,v2,norm;
	long k1,k2,k3;
	long nn,nt;
	int ret;
	
	
	if(!ndata || !xdata || !ydata || !zdata || !scene || !gdata)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=4*nn;
	    if((k1=ndata[nt+1]) < 0)continue;
	    if((k2=ndata[nt+2]) < 0 )continue;
	    if((k3=ndata[nt+3]) < 0)continue;
	    s=(Surface *)&scene->material[ndata[nt]];
		
        points[0][0]=xdata[k1];
        points[0][1]=ydata[k1];
        points[0][2]=zdata[k1];

        points[1][0]=xdata[k2];
        points[1][1]=ydata[k2];
        points[1][2]=zdata[k2];

        points[2][0]=xdata[k3];
        points[2][1]=ydata[k3];
        points[2][2]=zdata[k3];
		
		
		v1.x=points[3]-points[0];
		v1.y=points[4]-points[1];
		v1.z=points[5]-points[2];
		v2.x=points[6]-points[0];
		v2.y=points[7]-points[1];
		v2.z=points[8]-points[2];
		
		norm=CrossN(&v1,&v2);
		normals[0][0]=norm.x;
		normals[0][1]=norm.y;
		normals[0][2]=norm.z;

		normals[1][0]=norm.x;
		normals[1][1]=norm.y;
		normals[1][2]=norm.z;

		normals[2][0]=norm.x;
		normals[2][1]=norm.y;
		normals[2][2]=norm.z;
		
		if(vdata){
        	values[0]=vdata[k1];
        	values[1]=vdata[k2];
        	values[2]=vdata[k3];
			o = MakeTriNC(3,s,points,normals,values);
        }else{
			o = MakeTri(3,s,points);
        }
		if(!o)goto ErrorOut;
		
		if(SaveObject2(o,0))goto ErrorOut;
	
	}

	
	ret=0;
ErrorOut:
	if(ret){
	    WarningBatch("doAddShadowG Out of Memory\n");
	}
	return ret;
}
static int doAddShadowP(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	Flt values[3];
	Surface	*s;
	Object *o;
	long k1,k2,k3;
	long nn,nt;
	int ret;
	
	
	if(!ndata || !xdata || !ydata || !zdata || !scene || !nvdata)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=4*nn;
	    if((k1=ndata[nt+1]) < 0)continue;
	    if((k2=ndata[nt+2]) < 0 )continue;
	    if((k3=ndata[nt+3]) < 0)continue;
	    s=(Surface *)&scene->material[ndata[nt]];
		
        points[0][0]=xdata[k1];
        points[0][1]=ydata[k1];
        points[0][2]=zdata[k1];

        points[1][0]=xdata[k2];
        points[1][1]=ydata[k2];
        points[1][2]=zdata[k2];

        points[2][0]=xdata[k3];
        points[2][1]=ydata[k3];
        points[2][2]=zdata[k3];
		
		normals[0][0]=nvdata[k1*3];
		normals[0][1]=nvdata[k1*3+1];
		normals[0][2]=nvdata[k1*3+2];

		normals[1][0]=nvdata[k2*3];
		normals[1][1]=nvdata[k2*3+1];
		normals[1][2]=nvdata[k2*3+2];

		normals[2][0]=nvdata[k3*3];
		normals[2][1]=nvdata[k3*3+1];
		normals[2][2]=nvdata[k3*3+2];
		
		if(vdata){
        	values[0]=vdata[k1];
        	values[1]=vdata[k2];
        	values[2]=vdata[k3];
			o = MakeTriNC(3,s,points,normals,values);
        }else{
			o = MakeTriN(3,s,points,normals);
        }
		if(!o)goto ErrorOut;
		
		if(SaveObject2(o,0))goto ErrorOut;
	
	}

	
	ret=0;
ErrorOut:
	if(ret){
	    WarningBatch("doAddShadowP Out of Memory\n");
	}
	return ret;
}
static int doAddShadowVRML(long *ndata,long *matdata,float *normal,long normmax,
		   long *normlist,long normlistmax,
           double *xdata,double *ydata,double *zdata,double *vdata,
           float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS)
{
	Vec points[4000*3],normals[4000*3];
	Surface	*s;
	Object *o;
	double intensity;
	long nn,k1,k2,k3,ipass,ip;
	long n1,n2,n3,n4;
	
	long ns,ne,nc,ncell;
	long SkipPoly;
	
	
	s=NULL;
	
	CellCount=CellCount;

	ncell=0;
	
	SkipPoly=0;
	
	vdata=vdata;

	normmax=normmax;


	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}

	if(!tS)return 1;
	rd->States=(struct tState *)cMalloc(sizeof(struct tState),8754);
	if(!rd->States)return 1;
	
	*rd->States= *tS;
	
	if(pushpopPush(&rd->States,rd->popStack))return 1;

	
	for(nn=0;nn<nel;++nn,++ncell){	    
	    ip=0;
	    ns=nn;
	    ne=nn;
	    while(ndata[ne] >= 0 && ne < nel)++ne;
	    nn=ne;
	    nc=ne-ns;
	    if(nc < 3){
	        SkipPoly++;
	        continue;
	    }else if( nc > 4){
	    	if(nc >= 3998)nc = 3998;
	        goto doPolyHere;
	    }
	    n1=ndata[ns];
	    n2=ndata[ns+1];
	    n3=ndata[ns+2];
	    if(nc == 3){
	        ipass=1;
	        n4=0;
	    }else{
	        ipass=2;
	        n4=ndata[ns+3];
	    }
	  for(ip=0;ip<ipass;++ip){
	    if(ip == 0){
	        k1=n1;
	        k2=n2;
	        k3=n3;
	    }else{
	        k1=n3;
	        k2=n4;
	        k3=n1;
	    }
	    
	    if(k1 == k2 || k1 == k3 || k2 == k3)continue;

	    if((tS->NormalType == VRML_BY_VERTEX) && (ns < normlistmax)){
	        long kn1,kn2,kn3;
			if(ip == 0){
			    kn1=normlist[ns];
			    kn2=normlist[ns+1];
			    kn3=normlist[ns+2];
			}else{
			    kn1=normlist[ns+2];
			    kn2=normlist[ns+3];
			    kn3=normlist[ns];
			}
	        
			normals[0][0]=normal[kn1*3];
			normals[0][1]=normal[kn1*3+1];
			normals[0][2]=normal[kn1*3+2];

			normals[1][0]=normal[kn2*3];
			normals[1][1]=normal[kn2*3+1];
			normals[1][2]=normal[kn2*3+2];

			normals[2][0]=normal[kn3*3];
			normals[2][1]=normal[kn3*3+1];
			normals[2][2]=normal[kn3*3+2];
	    }else{


			normals[0][0]=nvdata[k1*3];
			normals[0][1]=nvdata[k1*3+1];
			normals[0][2]=nvdata[k1*3+2];

			normals[1][0]=nvdata[k2*3];
			normals[1][1]=nvdata[k2*3+1];
			normals[1][2]=nvdata[k2*3+2];

			normals[2][0]=nvdata[k3*3];
			normals[2][1]=nvdata[k3*3+1];
			normals[2][2]=nvdata[k3*3+2];


	    }

        points[0][0]=xdata[k1];
        points[0][1]=ydata[k1];
        points[0][2]=zdata[k1];

        points[1][0]=xdata[k2];
        points[1][1]=ydata[k2];
        points[1][2]=zdata[k2];

        points[2][0]=xdata[k3];
        points[2][1]=ydata[k3];
        points[2][2]=zdata[k3];

        points[3][0]=(double)matdata[ncell];
        points[3][1]=(double)ns;
        points[3][2]=(double)ip;
        
        if(tS->MaterialType == VRML_BY_VERTEX){
		    if(ip == 0){
		        points[4][0]=(double)matdata[ns];
		        points[4][1]=(double)matdata[ns+1];
		        points[4][2]=(double)matdata[ns+2];
		    }else{
		        points[4][0]=(double)matdata[ns+2];
		        points[4][1]=(double)matdata[ns+3];
		        points[4][2]=(double)matdata[ns];
		    }
        }
        
   		o = MakeTriNCV(3,s,points,normals,rd->States);
		if(!o)goto ErrorOut;
		
		if(SaveObject2(o,0))goto ErrorOut;
     

    
	 }
	continue;
doPolyHere: /* nc */

	    {
	       long n,np;
		
			for(n=0;n<nc;++n){
			    np=ndata[ns+n];		    
		        points[n][0]=xdata[np];
		        points[n][1]=ydata[np];
		        points[n][2]=zdata[np];
			}
			
			n=3*nc;

	        points[n][0]=(double)matdata[ncell];
	        points[n][1]=(double)ns;
	        points[n][2]=(double)ip;
	        ++n;
	        if(tS->MaterialType == VRML_BY_VERTEX){
	        	points[n][0]=(double)matdata[ns];
	        	points[n][1]=(double)matdata[ns+1];
	        	points[n][2]=(double)matdata[ns+2];
	        }

	   		o = MakePoly(s,(int)nc,points,rd->States);
			if(!o)goto ErrorOut;
			
			if(SaveObject2(o,0))goto ErrorOut;

				
	    }    

    
	}
ErrorOut:
	return 0;
}
static int NullPrint()
{
	fprintf(stderr, "%s: called (* print)(...), dying...\n", rd->Progname) ;
	abort() ;
	return 0;
}

static int NullIntersect()
{
	fprintf(stderr, "%s: called (* intersect)(...), dying...\n", rd->Progname) ;
	abort() ;
	return 0;
}

static int NullNormal()
{
	fprintf(stderr, "%s: called (* normal)(...), dying...\n", rd->Progname) ;
	abort() ;
	return 0;
}
