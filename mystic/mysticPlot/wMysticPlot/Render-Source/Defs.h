/*
 * D E F S . H
 *
 */
 
#include "TracerDef.h"

#define M_AMBIENT		1
#define M_DIFFUSE		2
#define M_MIRROR		4
#define M_TRANSMIT		8
#define M_INDEX			16
#define M_SPECULAR		32
#define M_COLORLEVEL	64
#define M_PATTERN		128
#define M_BUMP			256
#define M_TRANSPARENT	512
#define M_EMISSIVE	1024

 
#define		NSLABS		(3)
#define		BUNCHINGFACTOR	(10)
#define		PQSIZE		(10000)
 
#define Point Point2
 
#ifndef HUGE
#define HUGE 1e60
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif
 
#ifndef FLTDEF
#define FLTDEF
typedef double Flt ;
typedef Flt Vec[3] ;
#endif
typedef Vec Color7 ;
typedef Vec Point ;
typedef Vec ColorVEC ;
 
#define MakeVector(x, y, z, v)		(v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a)	(a)[0]=0-(a)[0];\
			(a)[1]=0-(a)[1];\
			(a)[2]=0-(a)[2];
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a)	(sqrt(VecDot(a,a)))
#define VecCopy(a,b)	 (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c)	 (c)[0]=(a)[0]+(b)[0];\
			 (c)[1]=(a)[1]+(b)[1];\
			 (c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c)	 (c)[0]=(a)[0]-(b)[0];\
			 (c)[1]=(a)[1]-(b)[1];\
			 (c)[2]=(a)[2]-(b)[2]
#define VecComb(A,a,B,b,c)	(c)[0]=(A)*(a)[0]+(B)*(b)[0];\
				(c)[1]=(A)*(a)[1]+(B)*(b)[1];\
			 	(c)[2]=(A)*(a)[2]+(B)*(b)[2]
#define VecAddS(A,a,b,c)	 (c)[0]=(A)*(a)[0]+(b)[0];\
				 (c)[1]=(A)*(a)[1]+(b)[1];\
				 (c)[2]=(A)*(a)[2]+(b)[2]
#define VecCross(a,b,c)	 (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1];\
			 (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2];\
			 (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
 
#define ColorToVec(a,b)	 (b)[0]=(a.red);(b)[1]=(a.green);(b)[2]=(a.blue);

#define PToVec(a,b)	 (b)[0]=(a.x);(b)[1]=(a.y);(b)[2]=(a.z);


 
/* #define max(a,b) 	((a)>(b)?(a):(b)) */
/* #define min(a,b) 	((a)<(b)?(a):(b)) */
 
/*----------------------------------------------------------------------*/
 
typedef struct Material Surface ;
 
typedef struct t_light {
	Vec	light_pos ;
	Flt	light_brightness ;
	Flt	light_bright;
	Flt	red ;
	Flt	green ;
	Flt	blue ;
	Flt vmin;
	Flt vmax;
	Flt vdiff;
	long type;

} Light ;
 
 
typedef struct t_object {
	unsigned short 	o_type ;
	Flt	o_dmin[NSLABS] ;
	Flt	o_dmax[NSLABS] ;
	struct t_objectprocs {
		int 	(*print) () ;
		int 	(*intersect) () ;
		int 	(*normal) () ;
		int 	(*shader) () ;
	} * o_procs ;
	struct Material 	* o_surf ;
	void	* o_data ;
} Object ;
 
typedef struct t_compositedata {
	unsigned short 	c_size ;
	Object *	c_object[BUNCHINGFACTOR] ;
} CompositeData ;
 
typedef struct t_objectprocs ObjectProcs ;
 
typedef struct t_isect {
	Flt 		isect_t;
	Flt 		isect_value;
	int 		isect_enter;
	Object 		* isect_prim;
	Surface 	* isect_surf;
	Flt 		f1;
	Flt 		f2;
	Vec		v1;
	Vec		v2;
} Isect ;
  
#define degtorad(x)	(((Flt)(x))*PI/180.0)

typedef struct t_qelem {
	Flt	q_key ;
	Object	* q_obj ;
} Qelem ;


typedef struct Ray {
	Point P ;
	Point D ;
	int 	Qsize ;
	Qelem	*Q;
	void  *traverseStack;
} Ray ;
 
#define RayPoint(ray,t,point)	VecAddS(t,(ray)->D,(ray)->P,point)

  
struct renderData{
	double *shadowdata,*zbuff;
	double *zbuff2;
	float *opacity,*vbuff,*vbuff2;
	long *cellbuff2;
	Vec Lampl,Eyel;

	Vec ClipPlanePoint;
	Vec ClipPlaneNormal;

	int doTransparency;

	double xmin,xmax,ymin,ymax,odx,ody;
	

    pushpopPtr popStack;
	struct tState *States;

	Object **Prims;
	long maxprims;
	int	nPrims;
	
	Vec	*Slab;	
	Flt	rayeps;
	
	Light *Lights;
	int	nLights;

	char *Progname;
	Object *Root;

	ObjectProcs NullProcs;	
/*	
	long nRays;	
	long nShadows;
	long nReflected;
	long nRefracted;
	long nChecked;
	long nEnqueued;
	long totalQueueResets;
	int	totalQueues;
	long maxQueueSize;
*/	
	
	int	maxlevel;
	Vec BackgroundColor;
	double h;      /* eye height */
	double o;      /* 1/2 eye separation */
	
	int (*Shadow)(Ray *ray, Flt tmax);

	
};

extern struct renderData *rd;

Flt VecNormalize(Vec vec);

Object *MakeTriN(int npoints,Surface *s, Vec *points,Vec *normals);

Object *MakeTriN2(int npoints,Surface *s, Vec *points,Vec *normals,Vec *col);

Object *MakeTriNC(int npoints,Surface *s, Vec *points,Vec *normals,Flt *values);

Object *MakeTri(int npoints,Surface *s, Vec * points);

Object *MakeTriNCV(int npoints,Surface *s, Vec *points,Vec *normals,struct tState *tS);

Object *MakeSphere(Surface *s,Vec pos,Flt radius);

Object *MakePoly(Surface *s,int npoints, Vec * points,struct tState *tS);

int Shade(Ray *ray,int level, Flt weight, Vec P,Vec N,Vec I, Isect *hit,Color7 col);

Object *MakeTriC1N1(int npoints,Surface *s, Vec *points,Vec *normals,Vec *col);

int SaveObject(Object *o,int flag);

int AllocatePQueue(Ray *ray);

int DallocatePQueue(Ray *ray);

int PriorityQueueInsert(Ray *ray,Flt key, Object * obj);

int PriorityQueueDelete(Ray *ray,Flt * key, Object ** obj);

int PriorityQueueNull(Ray *ray);

int PriorityQueueEmpty(Ray *ray);

int BuildBoundingSlabs(void);

int InitSlabs(void);
