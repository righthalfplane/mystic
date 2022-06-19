#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"
#include "uDialog.h"

#ifdef max
#undef max
#endif

struct drawInfo{
    struct Scene *scene;
	double *xdata;
	double *ydata;
	double *zdata;
	double *vdata;	
    float *nvdata;
	long *ndata;
    long CellCount;
    long nel;
    
    
    long *matdata;
	float *normal;
	long normmax;
	long *normlist;
	long normlistmax;
	struct tState *tS;
};


static Vec	Slab[] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0},
	{0.0, 1.0, 1.0}
} ;

extern int NullPrint(void) ;
extern int NullIntersect(void) ;
extern int NullNormal(void) ;


static ObjectProcs NullProcs = {
	NullPrint,
	NullIntersect,
	NullNormal
} ;

int writeTriangleP(FILE *out,struct P *p1,struct P *p2,struct P *p3);

int iMakeTriNC(struct Scene *scene,int npoints,Surface *s, Vec *points,Vec *normals);

int pclose(FILE *file);

int cMakeTriNCV(struct Scene *scene,int npoints,Vec *points,Vec *normals,float *color);

static int CleanTracing(void);

static int doAddShadowP(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                float *nvdata,long nel,long CellCount,struct Scene *scene);

static int doAddShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene);

static int doAddShadowVRML(long *ndata,long *matdata,float *normal,long normmax,
 		     long *normlist,long normlistmax,
                     double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS);
int doNoShadowVRMLBESTT(mThread *Threads);

int putLong(long n,FILE *out);
int putInt(long n,FILE *out);

Flt VecNormalize(Vec vec);
long doStripes(void *v1,void *v2,void *surf_color);
long doBumps(void *v1,void *v2,void *v3);

int ListObjectMessage(CObjectListPtr o,struct ObjectMessage *Message);

int ListGetMost(CObjectList *d,long *MostNodes,long *MostNormals,long *MostCells);

int uDrawPalette(struct paletteDraw *pd,IconPtr myIcon);
int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);

int ListObjectDraw(CObjectListPtr ObjectList,long *CellCount,int flag);

int doImageWrite(renderPtr openData,struct Scene *scene,float *vbuff);
int saveRenderImage(renderPtr openData,unsigned char *vbuff,long xResolution,long yResolution);
int oDumpit(renderPtr openData);

static int doFastPlot(renderPtr	openData);
static int doImage(struct Scene *scene);
void ScaleMatrix2(struct Matrix *m,struct System *p);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
int SetShadow(struct Scene *scene);
int SetScreen(struct Scene *scene);
int SetMaterialStuff(struct Scene *scene);
int StartBumps(struct Bump *b,long time);
int StartImages(struct Image *i,long time);
int StartStripes(struct Stripes *s,long time);
int colorit(long count,double *level,Flt value,int *ic);
int getString(char *s,long n,FILE *in);
struct P Norm(struct P *p1);
struct P CrossN(struct P *v1,struct P *v2);
struct P Sub(struct P *p1,struct P *p2);
struct P Add(struct P *p1,struct P *p2);
struct P Mult(struct P *p1,double Factor);
double Len(struct P *v1);
double Dot(struct P *v1,struct P *v2);

int doNoShadowP(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                float *nvdata,long nel,long CellCount,struct Scene *scene);
int doNoShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene);
int doNoShadowG(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,double *gdata,
		long nel,long CellCount,struct Scene *scene);
int doNoShadow4G(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,double *gdata,
		long nel,long CellCount,struct Scene *scene);
int doNoShadow4(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
				long nel,long CellCount,struct Scene *scene);
int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,
                double *vdata,long nel,long CellCount,struct Scene *scene);
int doNoShadowLines4(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene,int DrawZonesColor);	

int doNoShadow(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
			   long nel,long CellCount,struct Scene *scene);

int doNoShadowVRML(long *ndata,long *matdata,float *normal,long normmax,
 		     long *normlist,long normlistmax,
                     double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS);
                     
int doNoShadowVRMLBEST(long *ndata,long *matdata,float *normal,long normmax,
 		     long *normlist,long normlistmax,
                     double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS);
                     
int doNoShadow4PBEST(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene);
                 
int doNoShadowPBEST(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene);
                 
int Transf1(struct Trans1 *f);
int TransBlockSystem(struct Block *block,long count,
					 struct P *Oa,struct P *Xa,struct P *Ya,struct P *Za,
					 struct P *On,struct P *Xn,struct P *Yn,struct P *Zn);
int TransBlock(struct Block *block,long count,
			   struct P *Oa,struct P *Xa,struct P *Ya,struct P *Za,
			   struct P *On,struct P *Xn,struct P *Yn,struct P *Zn);
int TransLocalSystem(struct System *block,long count,
			   struct P *Oa,struct P *Xa,struct P *Ya,struct P *Za,
			   struct P *On,struct P *Xn,struct P *Yn,struct P *Zn);

int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);

int deScaleComp(struct Scene *scene,unsigned char *bline,unsigned char *bout,long xsize,long ysize);

int WriteComp(renderPtr openData,struct Scene *scene,float *vbuff);

static int RenderScene(renderPtr openData);

int doVrmlTexture(ColorVEC surf_color,double s1x,double s2x,long ns,long ip,struct tState *tS);

int RenderTrace(renderPtr openData);

static int RenderFast(renderPtr openData);

static struct renderData rdl;

extern struct renderData *rd;

int SetClipPlane(struct Scene *scene);

static long CountTri;
static long CountQuad;
static long CountPoly;
static long CountOther;
static long CountRejected;

static int typeOut=1;

int iMakeTriNCV(struct Scene *scene,int npoints,Surface *s, Vec *points,Vec *normals,struct tState *tS);
int iMakePoly(struct Scene *scene,Surface *s,int npoints, double * points,struct tState *tS);
int getColor(float *color,int mat,int *m,struct tState *tS);

int iMakePoly(struct Scene *scene,Surface *s,int npoints, double * points,struct tState *tS)
{
	struct vrmlMaterial *vrmlMat;
	char buffer[10000];
	int n,mat;
	double color[3];
	int MaterialType;
	
	if(!points || !tS || !scene)return 1;
	
	s=s;
	
	n=3*npoints;

	mat=(int)points[n];	

	MaterialType=tS->MaterialType;
	vrmlMat=tS->m;
	if(MaterialType == VRML_BY_FACE){
	
	    if(vrmlMat && (mat < vrmlMat->max)){
	        color[0]=vrmlMat->diffusedata[mat*3];
	        color[1]=vrmlMat->diffusedata[mat*3+1];
	        color[2]=vrmlMat->diffusedata[mat*3+2];
	    }else{
	        color[0]=.8f;
	        color[1]=.8f;
	        color[2]=.8f;
	    }
	
	
	}else{
		color[0]=.8f;
	    color[1]=.8f;
	    color[2]=.8f;
	}
	
	if(npoints == 3){
	    CountTri++;
	}else if (npoints == 4){
	    CountQuad++;
	}else if (npoints > 4){
		if(scene->pipe){
	        fprintf(scene->pipe,"1 %d  ",npoints);
	        fprintf(scene->pipe,"%g %g %g ",color[0],color[1],color[2]);
	        for(n=0;n<npoints;++n){
	        	fprintf(scene->pipe,"%g %g %g ",points[3*n],points[3*n+1],points[3*n+2]);
	        }
		}else{
		    if(typeOut == 0){
		        sprintf(buffer,"1 %d ",npoints);
		        sprintf(WarningBuff,"%g %g %g ",color[0],color[1],color[2]);
		        mstrncat(buffer,WarningBuff,10000);
		        for(n=0;n<npoints;++n){
		        	sprintf(WarningBuff,"%g %g %g %c ",points[3*n],points[3*n+1],points[3*n+2],0);
		        	mstrncat(buffer,WarningBuff,10000);
		        }
	       		/* WarningBatchFast(buffer); */
	        }else if(typeOut == 2){
				;
	        }else{
		        sprintf(WarningBuff,"f %g %g %g 1 0 20. 0 1",color[0],color[1],color[2]);
	       		WarningBatchFast(WarningBuff);
		        sprintf(buffer,"p %d ",npoints);
	       		WarningBatchFast(buffer);
		        for(n=0;n<npoints;++n){
	       			WarningBatchFast(WarningBuff);
		        }
	        }
        }
	     CountPoly++;
	}else{
	     CountOther++;
	}
	
	return 0;
}
int getColor(float *color,int mat,int *m,struct tState *tS)
{
	struct vrmlMaterial *vrmlMat;
	int MaterialType;
	int flag;
	
	if(!color || !tS)return 0;
	
	MaterialType=tS->MaterialType;
	vrmlMat=tS->m;
	
	if(MaterialType == VRML_BY_FACE){
	    if(vrmlMat && (mat < vrmlMat->max)){
	        color[0]=(float)vrmlMat->diffusedata[mat*3];
	        color[1]=(float)vrmlMat->diffusedata[mat*3+1];
	        color[2]=(float)vrmlMat->diffusedata[mat*3+2];
	        color[3]=(float)vrmlMat->diffusedata[mat*3];
	        color[4]=(float)vrmlMat->diffusedata[mat*3+1];
	        color[5]=(float)vrmlMat->diffusedata[mat*3+2];
	        color[6]=(float)vrmlMat->diffusedata[mat*3];
	        color[7]=(float)vrmlMat->diffusedata[mat*3+1];
	        color[8]=(float)vrmlMat->diffusedata[mat*3+2];
	    }else{
	        color[0]=.8f;
	        color[1]=.8f;
	        color[2]=.8f;
	        color[3]=.8f;
	        color[4]=.8f;
	        color[5]=.8f;
	        color[6]=.8f;
	        color[7]=.8f;
	        color[8]=.8f;
	    }
	}else if(MaterialType == VRML_BY_VERTEX){
	    double *d;


	    flag=vrmlMat->flagsdata[m[0]];

	    if(flag & M_DIFFUSE){

	        d=vrmlMat->diffusedata;
	        color[0]=(float)d[m[0]*3];
	        color[1]=(float)d[m[0]*3+1];
	        color[2]=(float)d[m[0]*3+2];
	        color[3]=(float)d[m[0]*3];
	        color[4]=(float)d[m[0]*3+1];
	        color[5]=(float)d[m[0]*3+2];
	        color[6]=(float)d[m[0]*3];
	        color[7]=(float)d[m[0]*3+1];
	        color[8]=(float)d[m[0]*3+2];
	    }else if(flag & M_SPECULAR){
	        d=vrmlMat->speculardata;

	        color[0]=(float)d[m[0]*3];
	        color[1]=(float)d[m[0]*3+1];
	        color[2]=(float)d[m[0]*3+2];
	        color[3]=(float)d[m[0]*3];
	        color[4]=(float)d[m[0]*3+1];
	        color[5]=(float)d[m[0]*3+2];
	        color[6]=(float)d[m[0]*3];
	        color[7]=(float)d[m[0]*3+1];
	        color[8]=(float)d[m[0]*3+2];
	    }

	}
	
	
	
	
	
	return 1;
}
static int CheckTrianglePoints(Vec *points1)
{
	Vec points[3];
	Vec v1,v2,v3;
	double len;
	
	if(!points1)return 1;
	
	
	sprintf(WarningBuff,"%g %g %g %g %g %g %g %g %g",
		points1[0][0],points1[0][1],points1[0][2],

		points1[1][0],points1[1][1],points1[1][2],

		points1[2][0],points1[2][1],points1[2][2]

		);

	sscanf(WarningBuff,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
		&points[0][0],&points[0][1],&points[0][2],

		&points[1][0],&points[1][1],&points[1][2],

		&points[2][0],&points[2][1],&points[2][2]

		);
		
	
	if(points[0][0] == points[1][0] && points[0][1] == points[1][1] && points[0][2] == points[1][2]){
		++CountRejected;
	    return 1;
	}
	
	if(points[1][0] == points[2][0] && points[1][1] == points[2][1] && points[1][2] == points[2][2]){
		++CountRejected;
	    return 1;
	}

	if(points[0][0] == points[2][0] && points[0][1] == points[2][1] && points[0][2] == points[2][2]){
		++CountRejected;
	    return 1;
	}
	
	
	VecSub(points[0],points[1],v1);
	VecSub(points[0],points[2],v2);
	VecCross(v1,v2,v3);
	len=VecLen(v3);
	
	if(len <= 0){
		++CountRejected;
	    return 1;
	}
	return 0;
}
int writeTriangleP(FILE *out,struct P *p1,struct P *p2,struct P *p3)
{
	struct P v1,v2,normal;
	
	if(!p1 || !p2 || !p3)return 1;
	
	if(p1->x == p2->x && p1->y == p2->y && p1->z == p2->z)return 1;
	if(p1->x == p3->x && p1->y == p3->y && p1->z == p3->z)return 1;
	if(p2->x == p3->x && p2->y == p3->y && p2->z == p3->z)return 1;
	
	v1=Sub(p2,p1);
	v2=Sub(p3,p1);
	normal=CrossN(&v1,&v2);
	
	if(out){
		fprintf(out,"  facet normal %g %g %g\n", normal.x, normal.y, normal.z);
		fprintf(out,"    outer loop\n");
		
		fprintf(out,"      vertex %g %g %g\n",p1->x,p1->y,p1->z);
		fprintf(out,"      vertex %g %g %g\n",p2->x,p2->y,p2->z);
		fprintf(out,"      vertex %g %g %g\n",p3->x,p3->y,p3->z);
		
		fprintf(out,"    endloop\n");
		fprintf(out,"  endfacet\n");
	}else{
	
		sprintf(WarningBuff,"  facet normal %g %g %g", normal.x, normal.y, normal.z);
		WarningBatchFast(WarningBuff);
		
		sprintf(WarningBuff,"    outer loop");
		WarningBatchFast(WarningBuff);
		
		sprintf(WarningBuff,"      vertex %g %g %g",p1->x,p1->y,p1->z);
		WarningBatchFast(WarningBuff);
		
		sprintf(WarningBuff,"      vertex %g %g %g",p2->x,p2->y,p2->z);
		WarningBatchFast(WarningBuff);
		
		sprintf(WarningBuff,"      vertex %g %g %g",p3->x,p3->y,p3->z);
		WarningBatchFast(WarningBuff);
		
	   
		sprintf(WarningBuff,"    endloop");
		WarningBatchFast(WarningBuff);
		
		sprintf(WarningBuff,"  endfacet");
		WarningBatchFast(WarningBuff);
	}

	return 0;
}

int cMakeTriNCV(struct Scene *scene,int npoints,Vec *points,Vec *normals,float *color)
{
	if(!points || !normals || !color || !scene)return 1;
	
	if(npoints == 3){
	    CountTri++;	    
		if(scene->pipe){
	        fprintf(scene->pipe,"1 %d %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g\n",npoints,
	           points[0][0],points[0][1],points[0][2],
	           points[1][0],points[1][1],points[1][2],
	           points[2][0],points[2][1],points[2][2],
	           
	           normals[0][0],normals[0][1],normals[0][2],
	           normals[1][0],normals[1][1],normals[1][2],
	           normals[2][0],normals[2][1],normals[2][2],
	           
	           color[0],color[1],color[2],
	           color[3],color[4],color[5],
	           color[6],color[7],color[8]
	           );
		}else{
		    if(typeOut == 0){
		        sprintf(WarningBuff,"1 %d %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g",npoints,
		           points[0][0],points[0][1],points[0][2],
		           points[1][0],points[1][1],points[1][2],
		           points[2][0],points[2][1],points[2][2],
		           
		           normals[0][0],normals[0][1],normals[0][2],
		           normals[1][0],normals[1][1],normals[1][2],
		           normals[2][0],normals[2][1],normals[2][2],
		           
		           color[0],color[1],color[2],
		           color[3],color[4],color[5],
		           color[6],color[7],color[8]
		           );
		        WarningBatchFast(WarningBuff);
	        }else if(typeOut == 2){
				writeTriangleP(NULL,(struct P *)points[0],(struct P *)points[1],(struct P *)points[2]);
	        }else{
	            if(CheckTrianglePoints(points))return 1;
	            
		        sprintf(WarningBuff,"f %g %g %g 1 0 20. 0 1",0.333*(color[0]+color[3]+color[6]),
		                                                     0.333*(color[1]+color[4]+color[7]),
		                                                     0.333*(color[2]+color[5]+color[8]));
	       		WarningBatchFast(WarningBuff);

	       		WarningBatchFast("p 3 ");
		        sprintf(WarningBuff,"%g %g %g",
		           points[0][0],points[0][1],points[0][2]
		           
		           );
		        WarningBatchFast(WarningBuff);
		        sprintf(WarningBuff,"%g %g %g ",
		           points[1][0],points[1][1],points[1][2]
		           
		           );
		        WarningBatchFast(WarningBuff);
		        sprintf(WarningBuff,"%g %g %g",
		           points[2][0],points[2][1],points[2][2]
		           
		           );
		        WarningBatchFast(WarningBuff);
	        }
        }
	}else if (npoints == 4){
	    CountQuad++;
		if(scene->pipe){
	        fprintf(scene->pipe,"1 %d %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g\n",npoints,
	           points[0][0],points[0][1],points[0][2],
	           points[1][0],points[1][1],points[1][2],
	           points[2][0],points[2][1],points[2][2],
	           points[3][0],points[3][1],points[3][2],
	           
	           normals[0][0],normals[0][1],normals[0][2],
	           normals[1][0],normals[1][1],normals[1][2],
	           normals[2][0],normals[2][1],normals[2][2],
	           normals[3][0],normals[3][1],normals[3][2],
	           
	           color[0],color[1],color[2],
	           color[3],color[4],color[5],
	           color[6],color[7],color[8],
	           color[9],color[10],color[11]
	           );
		}else{
		    if(typeOut == 0){
		        sprintf(WarningBuff,"1 %d %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g\n",npoints,
		           points[0][0],points[0][1],points[0][2],
		           points[1][0],points[1][1],points[1][2],
		           points[2][0],points[2][1],points[2][2],
		           points[3][0],points[3][1],points[3][2],
		           
		           normals[0][0],normals[0][1],normals[0][2],
		           normals[1][0],normals[1][1],normals[1][2],
		           normals[2][0],normals[2][1],normals[2][2],
		           normals[3][0],normals[3][1],normals[3][2],
		           
		           color[0],color[1],color[2],
		           color[3],color[4],color[5],
		           color[6],color[7],color[8],
		           color[9],color[10],color[11]
		           );
		        WarningBatchFast(WarningBuff);
	        }else if(typeOut == 2){
				writeTriangleP(NULL,(struct P *)points[0],(struct P *)points[1],(struct P *)points[2]);
				writeTriangleP(NULL,(struct P *)points[2],(struct P *)points[3],(struct P *)points[0]);
	        }else{
	        
	            if(CheckTrianglePoints(points))return 1;
	            
		        sprintf(WarningBuff,"f %g %g %g 1 0 20. 0 1",0.25*(color[0]+color[3]+color[6]+color[9]),
		                                                     0.25*(color[1]+color[4]+color[7]+color[10]),
		                                                     0.25*(color[2]+color[5]+color[8]+color[11]));
	       		WarningBatchFast(WarningBuff);
	       		
	       		
	       		if(points[2][0] == points[3][0] && points[2][1] == points[3][1] && points[2][2] == points[3][2]  ){
	       		
	    			++CountTri;
	   			 	--CountQuad;
	       		    
		       		WarningBatchFast("p 3 ");
			        sprintf(WarningBuff,"%g %g %g",
			           points[0][0],points[0][1],points[0][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[1][0],points[1][1],points[1][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[2][0],points[2][1],points[2][2]
			           
			           );
	       		}else{
	       		
		       		WarningBatchFast("p 4 ");
			        sprintf(WarningBuff,"%g %g %g",
			           points[0][0],points[0][1],points[0][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[1][0],points[1][1],points[1][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[2][0],points[2][1],points[2][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[3][0],points[3][1],points[3][2]
			           
			           );
		       }
		           
		        WarningBatchFast(WarningBuff);
	        }
        }
	}else if (npoints > 4){
	     CountPoly++;
	}else{
	     CountOther++;
	}
	return 0;
	
}


int iMakeTriNCV(struct Scene *scene,int npoints,Surface *s, Vec *points,Vec *normals,struct tState *tS)
{
	float color[9];
	int mat,m[3];
	
	if(!points || !normals || !tS)return 1;
	
	s=s;
	
	if(npoints == 3){
	    
	    mat=(int)points[3][0];
		m[0]=(int)points[4][0];
		m[1]=(int)points[4][1];
		m[2]=(int)points[4][2];
	    getColor(color,mat,m,tS);
	    
	    cMakeTriNCV(scene,npoints,points,normals,color);
	    
	}else if (npoints == 4){
	    CountQuad++;
	}else if (npoints > 4){
	     CountPoly++;
	}else{
	     CountOther++;
	}
	return 0;
}

int iMakeTriNC(struct Scene *scene,int npoints,Surface *s, Vec *points,Vec *normals)
{
	float color[12];
	if(!points || !normals || !scene)return 1;
	
	s=s;
	
	if(npoints == 3){
	    CountTri++;
		color[0]=color[3]=color[6]=(float)s->diffuse.red;
		color[1]=color[4]=color[7]=(float)s->diffuse.green;
		color[2]=color[5]=color[8]=(float)s->diffuse.blue;
		
		if(scene->pipe){
	        fprintf(scene->pipe,"1 %d %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g\n",npoints,
	           points[0][0],points[0][1],points[0][2],
	           points[1][0],points[1][1],points[1][2],
	           points[2][0],points[2][1],points[2][2],
	           
	           normals[0][0],normals[0][1],normals[0][2],
	           normals[1][0],normals[1][1],normals[1][2],
	           normals[2][0],normals[2][1],normals[2][2],
	           
	           color[0],color[1],color[2],
	           color[3],color[4],color[5],
	           color[6],color[7],color[8]
	           );
	    }else {
		    if(typeOut == 0){
		        sprintf(WarningBuff,"1 %d %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g   %g %g %g  %g %g %g  %g %g %g",npoints,
		           points[0][0],points[0][1],points[0][2],
		           points[1][0],points[1][1],points[1][2],
		           points[2][0],points[2][1],points[2][2],
		           
		           normals[0][0],normals[0][1],normals[0][2],
		           normals[1][0],normals[1][1],normals[1][2],
		           normals[2][0],normals[2][1],normals[2][2],
		           
		           color[0],color[1],color[2],
		           color[3],color[4],color[5],
		           color[6],color[7],color[8]
		           );
		        WarningBatchFast(WarningBuff);
	        }else if(typeOut == 2){
				writeTriangleP(NULL,(struct P *)points[0],(struct P *)points[1],(struct P *)points[2]);
	        }else{
	            if(CheckTrianglePoints(points))return 1;
	            
		        sprintf(WarningBuff,"f %g %g %g 1 0 20. 0 1",0.333*(color[0]+color[3]+color[6]),
		                                                     0.333*(color[1]+color[4]+color[7]),
		                                                     0.333*(color[2]+color[5]+color[8]));
	       		WarningBatchFast(WarningBuff);
	       		
	       		WarningBatchFast("p 3 ");
		        sprintf(WarningBuff,"%g %g %g",
		           points[0][0],points[0][1],points[0][2]

		           );
		        WarningBatchFast(WarningBuff);
		        sprintf(WarningBuff,"%g %g %g",
		           points[1][0],points[1][1],points[1][2]

		           );
		        WarningBatchFast(WarningBuff);
		        sprintf(WarningBuff,"%g %g %g",
		           points[2][0],points[2][1],points[2][2]

		           );
		        WarningBatchFast(WarningBuff);
		        
		        
	        }
        }
	}else if (npoints == 4){
	    CountQuad++;
		color[0]=color[3]=color[6]=color[9]=(float)s->diffuse.red;
		color[1]=color[4]=color[7]=color[10]=(float)s->diffuse.green;
		color[2]=color[5]=color[8]=color[11]=(float)s->diffuse.blue;
		if(scene->pipe){
	        fprintf(scene->pipe,"1 %d %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g\n",npoints,
	           points[0][0],points[0][1],points[0][2],
	           points[1][0],points[1][1],points[1][2],
	           points[2][0],points[2][1],points[2][2],
	           points[3][0],points[3][1],points[3][2],
	           
	           normals[0][0],normals[0][1],normals[0][2],
	           normals[1][0],normals[1][1],normals[1][2],
	           normals[2][0],normals[2][1],normals[2][2],
	           normals[3][0],normals[3][1],normals[3][2],
	           
	           color[0],color[1],color[2],
	           color[3],color[4],color[5],
	           color[6],color[7],color[8],
	           color[9],color[10],color[11]
	           );
		}else{
		    if(typeOut == 0){
		        sprintf(WarningBuff,"1 %d %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g  %g %g %g\n",npoints,
		           points[0][0],points[0][1],points[0][2],
		           points[1][0],points[1][1],points[1][2],
		           points[2][0],points[2][1],points[2][2],
		           points[3][0],points[3][1],points[3][2],
		           
		           normals[0][0],normals[0][1],normals[0][2],
		           normals[1][0],normals[1][1],normals[1][2],
		           normals[2][0],normals[2][1],normals[2][2],
		           normals[3][0],normals[3][1],normals[3][2],
		           
		           color[0],color[1],color[2],
		           color[3],color[4],color[5],
		           color[6],color[7],color[8],
		           color[9],color[10],color[11]
		           );
		        WarningBatchFast(WarningBuff);
	        }else if(typeOut == 2){
				writeTriangleP(NULL,(struct P *)points[0],(struct P *)points[1],(struct P *)points[2]);
	        }else{
	        
	            if(CheckTrianglePoints(points))return 1;
	            
		        sprintf(WarningBuff,"f %g %g %g 1 0 20. 0 1",0.25*(color[0]+color[3]+color[6]+color[9]),
		                                                     0.25*(color[1]+color[4]+color[7]+color[10]),
		                                                     0.25*(color[2]+color[5]+color[8]+color[11]));
	       		WarningBatchFast(WarningBuff);
	       		
	       		
	       		if(points[2][0] == points[3][0] && points[2][1] == points[3][1] && points[2][2] == points[3][2]  ){
	       		
	    			++CountTri;
	   			 	--CountQuad;
	       		    
		       		WarningBatchFast("p 3 ");
			        sprintf(WarningBuff,"%g %g %g",
			           points[0][0],points[0][1],points[0][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[1][0],points[1][1],points[1][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[2][0],points[2][1],points[2][2]
			           
			           );
	       		}else{
	       		
		       		WarningBatchFast("p 4 ");
			        sprintf(WarningBuff,"%g %g %g",
			           points[0][0],points[0][1],points[0][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[1][0],points[1][1],points[1][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[2][0],points[2][1],points[2][2]
			           
			           );
			        WarningBatchFast(WarningBuff);
			        sprintf(WarningBuff,"%g %g %g",
			           points[3][0],points[3][1],points[3][2]
			           
			           );
		       }
		           
		        WarningBatchFast(WarningBuff);
	        }
        }
	}else if (npoints > 4){
	     CountPoly++;
	}else{
	     CountOther++;
	}
	return 0;
}

FILE *getPIPE();

int DoTracingExport(IconPtr myIcon);

int DoTracingExport(IconPtr myIcon)
{
	static uDialogGroup Items[] =

        {
      /*  0 */      {"Ok", {10,180,80,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {115,180,80,20},uDialogButton,uDialogNone},
      /*  2 */      {"Export To OpenGL", {15,25,180,20},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"Write To BatchPrint", {15,55,180,20},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"clockwise polygons", {15,85,180,20},uDialogCheck,uDialogGetLong},
      /*  5 */      {"write geom", {15,115,180,20},uDialogButtonRadio,uDialogGetLong},
      /*  6 */      {"write nff", {15,135,180,20},uDialogButtonRadio,uDialogGetLong},
      /*  7 */      {"write stl", {15,155,180,20},uDialogButtonRadio,uDialogGetLong},
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,220,280};
 	uDialogList *uList;
	uDialogReturn *uR;
	static int extrude=0;
	static int clockwise=0;
	int ret;

	int Transparency;
	int SmoothNormals;
	struct Scene *scene;
	struct P Target;
	double Height;
	struct P Up;
	struct P Eye;
	renderPtr r;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;	
	scene=r->scene;
	if(!scene)return 1;	
	
	uR=NULL;
	
	uList=uDialogOpen("Export",&Rect1,ItemCount);
	
	if(!uList)goto OutOfHere;
	
	if(uDialogSetList(uList,Items,ItemCount))goto OutOfHere;
		
	ret=uDialogSet(uList,0L,
	              
                uDialogSetItem,2L,
                uDialogSetLogical,(extrude == 0),
				
                uDialogSetItem,3L,
                uDialogSetLogical,(extrude == 1),
	              
                uDialogSetItem,4L,
                uDialogSetLogical,(clockwise == 1),
	              
                uDialogSetItem,5L,
				uDialogSetRadioGroup,2,
                uDialogSetLogical,(typeOut == 0),
	              
                uDialogSetItem,6L,
				uDialogSetRadioGroup,2,
                uDialogSetLogical,(typeOut == 1),
	              
                uDialogSetItem,7L,
				uDialogSetRadioGroup,2,
                uDialogSetLogical,(typeOut == 2),
	              
				uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto OutOfHere;
Start:
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto OutOfHere;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto OutOfHere;
    
    if(uR[3].lreturn){
        extrude=1;
    }else{
        extrude=0;
    }
    
    clockwise=(int)uR[4].lreturn;

    typeOut=0;
    if(uR[6].lreturn)typeOut=1;
    if(uR[7].lreturn)typeOut=2;
    
	if(extrude == 0){
		scene->pipe=getPIPE();	
		if(!scene->pipe){
		    Warning("Could Not Launch mShowOpenGL");
		    extrude=1;
		    goto Start;
		}
	}

    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;
	
	
	CountTri=0;
	CountQuad=0;
	CountPoly=0;
	CountOther=0;
	CountRejected=0;
	
	SmoothNormals=scene->SmoothNormals;
	scene->SmoothNormals=3;
	Transparency=(int)scene->Transparency;
	Target=scene->Target;
	Height=scene->Height;
	Eye=scene->Eye;
	Up=scene->Up;
	
	
	scene->Target=p(0.0,0.0,-10.0);
	scene->Eye=p(0.0,0.0,1.0);
	scene->Height=1.0;
	scene->Up=p(0.0,1.0,0.0);
	
	
	if(scene->pipe){
	    fprintf(scene->pipe,"2 %d\n",clockwise);
	}else{
	    if(typeOut == 0){
	    	sprintf(WarningBuff,"2 %d",clockwise);
			WarningBatchFast(WarningBuff);  
		}else if(typeOut == 2){
			WarningBatchFast("solid o1");  
		}
	}    
	
	
	CleanTracing();

	RenderScene(r);


	CleanTracing();
	
	if(scene->pipe){
	    fprintf(scene->pipe,"99\n");
	    pclose(scene->pipe);
	}else{
	    if(typeOut == 0){
			WarningBatchFast("99");
			WarningBatchAtEnd(WarningBuff);  
		}else if(typeOut == 2){
			WarningBatchFast("endsolid o1");
			WarningBatchAtEnd(WarningBuff);  
		}
	}    

	scene->SmoothNormals=SmoothNormals;
	scene->Transparency=Transparency;
	scene->Target=Target;
	scene->Height=Height;
	scene->Eye=Eye;
	scene->Up=Up;
	
	sprintf(WarningBuff,"CountTri %ld CountQuad %ld CountPoly %ld CountOther %ld CountRejected %ld\n",CountTri,CountQuad,CountPoly,CountOther,CountRejected);
	WarningBatch(WarningBuff);
	
	if(!scene->pipe){
	    if(typeOut == 1){
			WarningBatch("b 1 1 1\n");
			WarningBatch("v\n");
			sprintf(WarningBuff,"from %g %g %g \n",scene->Eye.x,scene->Eye.y,scene->Eye.z);
			WarningBatch(WarningBuff);
			sprintf(WarningBuff,"at %g %g %g \n",scene->Target.x,scene->Target.y,scene->Target.z);
			WarningBatch(WarningBuff);
			sprintf(WarningBuff,"up %g %g %g \n",scene->Up.x,scene->Up.y,scene->Up.z);
			WarningBatch(WarningBuff);
			sprintf(WarningBuff,"angle %g\n",scene->Degrees);
			WarningBatch(WarningBuff);
			sprintf(WarningBuff,"hither 0.001\n");
			WarningBatch(WarningBuff);
			sprintf(WarningBuff,"resolution 720 480 \n");
			WarningBatch(WarningBuff);
			sprintf(WarningBuff,"l %g %g %g \n",scene->Eye.x,scene->Eye.y,scene->Eye.z);
			WarningBatch(WarningBuff);
		}  
	}    
	
	
	
	
	
OutOfHere:

    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	return 0;
}

static int RenderScene(renderPtr openData)
{
	long xResolution,yResolution;
	struct Scene *scene;
	struct P Eye;
	struct P EyeRight;
	double stime;
	double etime;
	int iWrite;
	int ret;
	

	if(!openData)return 1;
	
	scene=openData->scene;

	if(!scene)return 1;
		
	
	stime=rtime();

	ret=1;
	
	iWrite=TRUE;
		

	SetMaterialStuff(scene);
	SetScreen(scene);
	SetShadow(scene);
		
	Eye=scene->Eye;
	scene->EyeOffset=0;
	EyeRight=scene->EyeRight;
	xResolution=scene->xResolution;
	yResolution=scene->yResolution;
	
	ret=RenderFast(openData);
	
	ret=0;

	etime=rtime()-stime;

	sprintf(WarningBuff,"Time To Export Data %.2f Seconds\n",etime);
	if(scene->debug > 0)WarningBatch(WarningBuff);
	
	scene->Eye=Eye;
	scene->xResolution=xResolution;
	scene->yResolution=yResolution;
				
	return ret;
}

static int RenderFast(renderPtr openData)
{
	struct Scene *scene;

	
	if(!openData)return 1;
	
	scene=openData->scene;

	if(!scene)return 1;
		
	rd = &rdl;
	
	zerol((char *)&rdl,sizeof(rdl));
	
	rdl.Slab=Slab;
	rdl.rayeps=1e-6;
	rdl.maxlevel=5;
	rdl.Progname="Ray Trace";
	rdl.NullProcs=NullProcs;
	
	rd->popStack=pushpopStart(&rd->States);
	if(!rd->popStack)goto ErrorOut;
	
	
	SetClipPlane(scene);

	scene->g.Ambient= .15;
	scene->doNoShadow=doNoShadow;
	scene->doNoShadowG=doNoShadowG;
	scene->doNoShadowP=doNoShadowP;	
	scene->doNoShadow4=doNoShadow4;
	scene->doNoShadow4G=doNoShadow4G;
	scene->doNoShadow4P=doNoShadow4P;
	scene->doNoShadowVRML=doNoShadowVRML;
	if(scene->SmoothNormals == 3){
		scene->doNoShadowP=doAddShadowP;
		scene->doNoShadow4P=doAddShadow4P;
		scene->doNoShadowVRML=doAddShadowVRML;
	}
			
	doFastPlot(openData);


ErrorOut:
		
	return 0;

}


static int CleanTracing(void)
{

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
	
		
	return 0;
}

static int doFastPlot(renderPtr openData)
{
	long length,xResolution,yResolution;
	Vec		BackgroundColor;
	struct Scene *scene;
/*
	struct sWorld World;
*/

	scene=openData->scene;
	
	rd->o=scene->EyeOffset;
	rd->h=scene->Height;

	MakeVector(0.,0.,scene->Height,rd->Eyel);

	xResolution=scene->xResolution;
	yResolution=scene->yResolution;

	
	length=xResolution*yResolution;
			
	ColorToVec(scene->BackGround,BackgroundColor);
	
	doImage(scene);

	return 0;
}
static int doImage(struct Scene *scene)
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
			sprintf(WarningBuff,"doImage gdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}else if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if((nvdata=(float *)cMalloc(3*scene->MostNodes*sizeof(float),8638)) == NULL){
			sprintf(WarningBuff,"doImage nvdata Out of memory Requested (%ld)\n",3*scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rdata=(float *)cMalloc(scene->MostNodes*sizeof(float),8639)) == NULL){
			sprintf(WarningBuff,"doImage rdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((sdata=(float *)cMalloc(scene->MostNodes*sizeof(float),8640)) == NULL){
			sprintf(WarningBuff,"doImage sdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}

	if(scene->MostCells){
	    if((cdata=(long *)cMalloc(scene->MostCells*sizeof(long),8641)) == NULL){
			sprintf(WarningBuff,"doImage cdata Out of memory Requested (%ld)\n",scene->MostCells*sizeof(long));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}

	length=scene->xResolution*scene->yResolution;
	if(Transparency){	
	    if((rd->zbuff2=(double *)cMalloc(length*sizeof(double),8642)) == NULL){
			sprintf(WarningBuff,"doImage zbuff2 Out of memory Requested (%ld)\n",length*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rd->vbuff2=(float *)cMalloc(3*length*sizeof(float),8643)) == NULL){
			sprintf(WarningBuff,"doImage vbuff2 Out of memory Requested (%ld)\n",3*length*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rd->cellbuff2=(long *)cMalloc(length*sizeof(long),8644)) == NULL){
			sprintf(WarningBuff,"doImage cellbuff2 Out of memory Requested (%ld)\n",length*sizeof(long));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    if((rd->opacity=(float *)cMalloc(length*sizeof(float),8645)) == NULL){
			sprintf(WarningBuff,"doImage opacity Out of memory Requested (%ld)\n",length*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}
	if((xdatal=(double *)cMalloc(scene->MostNodes*sizeof(double),8646)) == NULL){
	    sprintf(WarningBuff,"doImage  xdatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if((ydatal=(double *)cMalloc(scene->MostNodes*sizeof(double),8647)) == NULL){
	    sprintf(WarningBuff,"doImage ydatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if((zdatal=(double *)cMalloc(scene->MostNodes*sizeof(double),8648)) == NULL){
	    sprintf(WarningBuff,"doImage zdatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	if((vdata=(double *)cMalloc(scene->MostNodes*sizeof(double),8649)) == NULL){
	    sprintf(WarningBuff,"doImage zdatal Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	
	Global=scene->Global;
	Screen.p=scene->g.pScreen;
	Screen.x=scene->g.V1;
	Screen.y=scene->g.V2;
	Screen.z=scene->g.V3;
	
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
	
	return 0;
}
static int doAddShadowP(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	float color[12];
	float *rdata;
	float *sdata;
	Surface	*s;
	/* Object *o; */
	long k1,k2,k3;
	long nn,nt;
	int ret;
	
	
	rdata=scene->rdata;
	sdata=scene->sdata;
	
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
			
		if((rdata && sdata) && s->flag & M_PATTERN){
				ColorVEC	surf_color;
			    Isect hit;

			    hit.f1=rdata[k1];
			    hit.f2=sdata[k1];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    			    
			    color[0]=(float)surf_color[0];
			    color[1]=(float)surf_color[1];
			    color[2]=(float)surf_color[2];
			    
			    hit.f1=rdata[k2];
			    hit.f2=sdata[k2];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    
			    color[3]=(float)surf_color[0];
			    color[4]=(float)surf_color[1];
			    color[5]=(float)surf_color[2];
			    
			    
			    hit.f1=rdata[k3];
			    hit.f2=sdata[k3];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    
			    color[6]=(float)surf_color[0];
			    color[7]=(float)surf_color[1];
			    color[8]=(float)surf_color[2];
			    
			    cMakeTriNCV(scene,3,points,normals,color);
			    
		}else if(vdata && (s->flag & M_COLORLEVEL)){
		    int ic;
        	
		    colorit(s->ColorCount,s->ColorLevel,
		    vdata[k1],&ic);
		    
		    color[0]=(float)s->ColorValue[ic].red;
		    color[1]=(float)s->ColorValue[ic].green;
		    color[2]=(float)s->ColorValue[ic].blue;
        	
		    colorit(s->ColorCount,s->ColorLevel,
		    vdata[k2],&ic);
        	
		    color[3]=(float)s->ColorValue[ic].red;
		    color[4]=(float)s->ColorValue[ic].green;
		    color[5]=(float)s->ColorValue[ic].blue;
        	
		    colorit(s->ColorCount,s->ColorLevel,
		    vdata[k3],&ic);

		    color[6]=(float)s->ColorValue[ic].red;
		    color[7]=(float)s->ColorValue[ic].green;
		    color[8]=(float)s->ColorValue[ic].blue;
        	
		    cMakeTriNCV(scene,3,points,normals,color);
        	
        }else{
			iMakeTriNC(scene,3,s,points,normals);
        }
	}

	
	ret=0;

	if(ret){
	    WarningBatch("doAddShadowP Out of Memory\n");
	}
	return ret;
}
static int doAddShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene)
{
  	Vec points[12],normals[12];
	float color[12];
	Surface	*s;
	/* Object *o; */
	long n1,n2,n3,n4;
	float *rdata;
	float *sdata;
	long nn,nt;
	int ret;
		
	rdata=scene->rdata;
	sdata=scene->sdata;
	
	if(!ndata || !xdata || !ydata || !zdata || !scene || !nvdata)return 1;
	
	CellCount=CellCount;
	
	ret=1;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0)continue;
/*	    
	    if(CellCount+nn != 45)continue;
*/	    
		    s=(Surface *)&scene->material[ndata[nt]];
			
	        points[0][0]=xdata[n1];
	        points[0][1]=ydata[n1];
	        points[0][2]=zdata[n1];

	        points[1][0]=xdata[n2];
	        points[1][1]=ydata[n2];
	        points[1][2]=zdata[n2];

	        points[2][0]=xdata[n3];
	        points[2][1]=ydata[n3];
	        points[2][2]=zdata[n3];
	        
	        points[3][0]=xdata[n4];
	        points[3][1]=ydata[n4];
	        points[3][2]=zdata[n4];
	        
	        
	        
			normals[0][0]=nvdata[n1*3];
			normals[0][1]=nvdata[n1*3+1];
			normals[0][2]=nvdata[n1*3+2];

			normals[1][0]=nvdata[n2*3];
			normals[1][1]=nvdata[n2*3+1];
			normals[1][2]=nvdata[n2*3+2];

			normals[2][0]=nvdata[n3*3];
			normals[2][1]=nvdata[n3*3+1];
			normals[2][2]=nvdata[n3*3+2];

			normals[4][0]=nvdata[n4*3];
			normals[4][1]=nvdata[n4*3+1];
			normals[4][2]=nvdata[n4*3+2];

	        
			
			if((rdata && sdata) && s->flag & M_PATTERN){
				ColorVEC	surf_color;
			    Isect hit;

			    hit.f1=rdata[n1];
			    hit.f2=sdata[n1];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    
			    color[0]=(float)surf_color[0];
			    color[1]=(float)surf_color[1];
			    color[2]=(float)surf_color[2];
			    
			    hit.f1=rdata[n2];
			    hit.f2=sdata[n2];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    
			    color[3]=(float)surf_color[0];
			    color[4]=(float)surf_color[1];
			    color[5]=(float)surf_color[2];
			    			    
			    hit.f1=rdata[n3];
			    hit.f2=sdata[n3];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    
			    color[6]=(float)surf_color[0];
			    color[7]=(float)surf_color[1];
			    color[8]=(float)surf_color[2];
			    			    
			    
			    hit.f1=rdata[n4];
			    hit.f2=sdata[n4];

			    (*s->PatternRoutine)(s,&hit,surf_color);
			    
			    color[9]=(float)surf_color[0];
			    color[10]=(float)surf_color[1];
			    color[11]=(float)surf_color[2];
			    			    
			    
			    cMakeTriNCV(scene,4,(Vec *)points,(Vec *)normals,color);
			    
			}else if(vdata && (s->flag & M_COLORLEVEL)){
			    int ic;
	        	
			    colorit(s->ColorCount,s->ColorLevel,
			    vdata[n1],&ic);
			    
			    color[0]=(float)s->ColorValue[ic].red;
			    color[1]=(float)s->ColorValue[ic].green;
			    color[2]=(float)s->ColorValue[ic].blue;
        	
			    colorit(s->ColorCount,s->ColorLevel,
			    vdata[n2],&ic);
			    
			    color[3]=(float)s->ColorValue[ic].red;
			    color[4]=(float)s->ColorValue[ic].green;
			    color[5]=(float)s->ColorValue[ic].blue;
        	
	        	
			    colorit(s->ColorCount,s->ColorLevel,
			    vdata[n3],&ic);
			    
			    color[6]=(float)s->ColorValue[ic].red;
			    color[7]=(float)s->ColorValue[ic].green;
			    color[8]=(float)s->ColorValue[ic].blue;
        	
			    colorit(s->ColorCount,s->ColorLevel,
			    vdata[n4],&ic);
			    
			    color[9]=(float)s->ColorValue[ic].red;
			    color[10]=(float)s->ColorValue[ic].green;
			    color[11]=(float)s->ColorValue[ic].blue;
        	
			    
			    cMakeTriNCV(scene,4,points,normals,color);
	        	
	        }else{
				iMakeTriNC(scene,4,s,points,normals);
	        }
	
	}

	
	ret=0;

	if(ret){
	    WarningBatch("doAddShadow4P Out of Memory\n");
	}
	return ret;
}
static int doAddShadowVRML(long *ndata,long *matdata,float *normal,long normmax,
		   long *normlist,long normlistmax,
           double *xdata,double *ydata,double *zdata,double *vdata,
           float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS)
{
	Vec points[4000*3],normals[4000*3];
	double ppoints[4000*3];
	Surface	*s;
	/* Object *o; */
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
        
   		iMakeTriNCV(scene,3,s,(Vec *)points,(Vec *)normals,rd->States);
    
	 }
	continue;
doPolyHere: /* nc */

	    {
	       long n,np;
		
			for(n=0;n<nc;++n){
			    np=ndata[ns+n];		    
		        ppoints[3*n]=xdata[np];
		        ppoints[3*n+1]=ydata[np];
		        ppoints[3*n+2]=zdata[np];
			}
			
			n=3*nc;

	        ppoints[n]=(double)matdata[ncell];
	        ppoints[n+1]=(double)ns;
	        ppoints[n+2]=(double)ip;
	        
	        if(tS->MaterialType == VRML_BY_VERTEX){
	        	ppoints[n+3]=(double)matdata[ns];
	        	ppoints[n+4]=(double)matdata[ns+1];
	        	ppoints[n+5]=(double)matdata[ns+2];
	        }

	   		iMakePoly(scene,s,(int)nc,ppoints,rd->States);

				
	    }    

    
	}

	return 0;
}
