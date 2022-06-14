#ifndef TRACERDEF

#define TRACERDEF

#include "paletteDraw.h"
#include "Message.h"
#include "Linedata.h"
#include "File8.h"
#include "KdTree.h"

#ifndef FLTDEF
#define FLTDEF
typedef double Flt ;
typedef Flt Vec[3] ;
#endif

#define PATTERN_STRIPES		1
#define PATTERN_IMAGE 		2
#define BUMP_BUMPS			3
#define VOLUME_VAULES		4


#define B_REFLECT_X		(1)
#define B_REFLECT_Y		(2)
#define B_REFLECT_Z		(4)
#define B_SPLIT_2		(8)
#define B_SPLIT_4		(16)

#define B_ROTATE_X		(32)
#define B_ROTATE_Y		(64)
#define B_ROTATE_Z		(128)

#define B_2D_VALUE		(0)
#define B_3D_VALUE		(1)
#define B_2D_COORDINATES	(2)
#define B_3D_COORDINATES	(3)


#define		T_COMPOSITE		(0)
#define		T_SPHERE		(1)
#define		T_POLY			(2)
#define		T_CONE			(3)
#define		T_QUAD			(4)
#define		T_QUADN			(5)
#define		T_QUADNC		(6)
#define		T_TRI			(7)
#define		T_TRIN			(8)
#define		T_TRINC			(9)
#define		T_RING			(10)
#define		T_BLOCK			(11)
#define		T_BLOCKSYSTEM		(12)
#define		T_LOCALSYSTEM		(14)
#define		T_VRML				(15)
#define		T_TRIC1N1			(16)

struct P{
    double x;
    double y;
    double z;
};


struct System{
	struct P x;
	struct P y;
	struct P z;
	struct P p;
	double Theta_x;
	double Theta_y;
	double Theta_z;
	double Scale_x;
	double Scale_y;
	double Scale_z;
};
 

struct Matrix{
	double x[4][4];
};
 

struct Block{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    double vmin;
    double vmax;
    long *Elements;
    double *xdata;
    double *ydata;
    double *zdata;
    float *rdata;
    float *sdata;
    double *stress;
    double *nx;
    double *ny;
    double *nz;
    double *dxdata;
    double *dydata;
    double *dzdata;
    double *oldx;
    double *oldy;
    double *oldz;
    double *vdata;
    long NodeCount;
    long ElementCount;
    long StressCount;
    long dxyzCount;
    long NodesMax;
    long BlockType;
    char **Labels;
    long LabelCount;
    long FirstCell;
	struct PlotAttributes pa;
    struct System Local;
    long RsCount;
    long ItWas;
    long nstress;
};


struct normtree{
    long i;
    struct P norm;
    struct normtree *left;
    struct normtree *right;
};

struct normtransform{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double odx;
    double ody;
    struct normtree *root,*pfind;
};


struct Trans1{
    struct P Oa;
    struct P Xa;
    struct P Ya;
    struct P Za;
    struct P On;
    struct P Xn;
    struct P Yn;
    struct P Zn;
    char *P;
    long Groups;
    long Type;
};

 
struct Color{
    double red;
    double green;
    double blue;
};

struct Ring{
    struct P origin;
    struct P normal;
    double inner;
    double outer;
    long material;
};

 
struct Bump{
    double xRepeat;
    double yRepeat;
    long xCount;
    long yCount;
    struct P *bNormal;
    double *xRange;
    double *yRange;
    long *cFlags;
};

struct Image{
    struct FileInfo2 *Files;
    int type;
    char *Data;
    char *Name;
    long xSize;
    long ySize;
    double xRepeat;
    double yRepeat;
    long repeatr;
    long repeats;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    long ItWas;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    double rmin;
    double rmax;
    double smin;
    double smax;
    double dr;
    double ds;
    double dx;
    double dy;
    double vmin;
    double vmax;
};


struct Stripes{
    struct FileInfo2 *Files;
    int type;
    double xRepeat;
    double yRepeat;
    long xCount;
    long yCount;
    struct Color *cColor;
    double *xRange;
    double *yRange;
    long *cFlags;
};

struct Material{
    struct Color transparent;
    struct Color ambient;
    struct Color diffuse;
    struct Color mirror;
    struct Color transmit;
	struct PlotAttributes pa;
	struct paletteDraw pd;
    double opacity;
    double mirrorfactor;
    double transmitfactor;
    double index;
    double phong;
    double specular;
    double intensity;    
    struct Color ColorValue[256];
    double ColorLevel[256];    
    long ColorCount;
    long flag; 
    struct Image ImageData;
    long PatternType;
    char *PatternData;
    long (*PatternRoutine)(void *s,void * hit,void *surf_color); 
    long BumpType;
    char *BumpData;
    long (*BumpRoutine)(void *s,void * hit,void *surf_color); 
};
 
struct Quad{
    struct P p1;
    struct P p2;
    struct P p3;
    struct P p4;
    long material;
};
 
struct QuadN{
    struct P p1;
    struct P p2;
    struct P p3;
    struct P p4;
    struct P n1;
    struct P n2;
    struct P n3;
    struct P n4;
    long material;
};
 
struct QuadNC{
    struct P p1;
    struct P p2;
    struct P p3;
    struct P p4;
    struct P n1;
    struct P n2;
    struct P n3;
    struct P n4;
    double v1;
    double v2;
    double v3;
    double v4;
    long material;
};
 
struct Triangle{
    struct P p1;
    struct P p2;
    struct P p3;
    long material;
};
 
struct TriangleN{
    struct P p1;
    struct P p2;
    struct P p3;
    struct P n1;
    struct P n2;
    struct P n3;
    long material;
};
 
struct TriNC{
    struct P p1;
    struct P p2;
    struct P p3;
    struct P n1;
    struct P n2;
    struct P n3;
    double v1;
    double v2;
    double v3;
    long material;
};
 
struct Sphere{
    struct P origin;
    double radius;
    long material;
};

struct Poly{
    struct P *points;
    long npoints;
    long material;
    long aline;
};
 
 
struct Cone{
    struct P basepoint;
    struct P apexpoint;
    double baseradius;
    double apexradius;
    long material;
};

struct tState{
	struct vrmlMaterialBinding *mb;
	struct vrmlMaterial *m;
	struct vrmlCoordinate3 *p;
	struct vrmlNormal *n;
	struct vrmlNormalBinding *nb;
	struct vrmlTextureCoordinate2 *t2;
	struct vrmlTexture2Transform *t2t;
	struct vrmlTexture2 *t;
	struct Matrix GroupToScreen;
	struct Scene *scene;
	long Material;
	long *CellCount;
	int flag;
	int MaterialType;
	int NormalType;
	struct vrmlIndexedFaceSet *s;
	int flagTexture;
	double opacity;
};


#define G_SPHERE		1
#define G_CONE			2
#define G_DISK			3
#define G_TORUS			4
#define G_BLOCK			5
#define G_GROUP			6
#define G_WORLD			7
#define G_EXTRUDE_SDS2D		8
#define G_ROTATE_SDS2D		9
#define G_EXTRUDE_HDF		10
#define G_ROTATE_HDF		11
#define G_LEVEL_SDS3D		12
#define G_VRML_LIST2		13
#define G_VRML2			    14
#define G_SHOW				15
#define G_EXTRUDE_PIO2D		16
#define G_ROTATE_PIO2D		17
#define G_RAY_PIO2D			18
#define G_LEVEL_PIO3D	    19
#define G_EXTRUDE_PIO3D		20
#define G_TRACER_PIO2D		21
#define G_FLIGHT_PATH		22
#define G_STL				23
#define G_STREAMLINE_PIO3D	24
#define G_VRML				25

struct Scene{
    struct KdTree ObjectKdTree;
    struct CObjectListStruct *ObjectList;
    long PolyCount;
    struct Poly *poly;
    struct Material *material;
    long MaterialCount;
    long MaterialUsed;
    struct Ring *ring;
    long RingCount;
    struct Cone *cone;
    long ConeCount;
    struct Quad *quad;
    long QuadCount;
    struct QuadNC *quadnc;
    long QuadNCCount;
    struct QuadN *quadn;
    long QuadNCount;
    struct TriNC *trinc;
    long TriNCCount;
    struct TriangleN *trianglen;
    long TriangleNCount;
    struct Triangle *triangle;
    long TriangleCount;
    struct Sphere *sphere;
    long SphereCount;
    struct Sphere lamp;
    struct Block *block;
    long BlockCount;
    struct vrmlList *vrml;
    long VrmlCount;
    struct P EyeLeft;
    struct P EyeRight;
    struct P Eye;
    struct P Target;
    struct P Up;
    struct P FindPoint;
    long xResolution;
    long yResolution;
    long EyeView;
    struct Color BackGround;
    double Degrees;
    double AspectRatio;
    double Height;
    double EyeSeparation;
    double EyeOffset;
    long MaxLevel;
    long MacRays;
    long BitsCompression;
    char *FileOut;
    void *DumpData;
    int (*Dump)(struct Scene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData);
    long debug;
    long AutoFullScreen;
    long AutoCenterScreen;
    long AutoCenterScreenDone;
    char *NetAddress;
    char sequenceName[256];
    long sequence;
    long sequenceSave;
    long ShadowFlag;
    long Rastor8;
    long SmoothNormals;
    struct DisplayInfo{
		double Ambient;
		struct P V1;
		struct P V2;
		struct P V3;
		struct P pScreen;
		struct P S1,S2,S3;
		struct P Lamp;
    }g;
	struct System Global;
	struct Matrix GlobalToScreen;
	struct Matrix ScreenToGlobal;
	struct System World;
	double xmin;
	double ymin;
	double odx;
	double ody;
	double delx;
	double dely;
    long *CellBuff;
	double *zbuff;
	double *xdatal;
	double *ydatal;
	double *zdatal;
	double *gdata;
	float *nvdata;
	float *rdata;
	float *sdata;
	double *vdata;
	long *cdata;
	long MostNodes;
	long MostNormals;
	long MostCells;
	Vec Lampl,Eyel;
	double CenterCz;
	struct PlotAttributes pa;
	struct paletteDraw pd;
	long Transparency;
	long TransparencyPasses;
    long time;
    long itIs;
	long Closing;
	double xmove;
	double ymove;
	double zmove;
    struct Sphere lamp0;
    long RayTracing;
    long Jitter;
    long JitterCount;
    long FixRayObjects;

	struct CObjectStruct *CurrectObject;
    
    struct t_light *Lights;
    int	nLights;

 	int (*doNoShadowG)(long *ndata,double *xdata,double *ydata,
 				double *zdata,double *vdata,double *gdata,
                long nel,long CellCount,struct Scene *scene);
	int (*doNoShadowP)(long *ndata,double *xdata,double *ydata,double *zdata,
                double *vdata,float *nvdata,
                long nel,long CellCount,struct Scene *scene);
	int (*doNoShadow4G)(long *ndata,double *xdata,double *ydata,double *zdata,
 				 double *vdata,double *gdata,
                 long nel,long CellCount,struct Scene *scene);
	int (*doNoShadow4P)(long *ndata,double *xdata,double *ydata,double *zdata,
				 double *vdata,float *nvdata,
                 long nel,long CellCount,struct Scene *scene);
	int (*doNoShadow4)(long *ndata,double *xdata,double *ydata,double *zdata,
				double *vdata,
		        long nel,long CellCount,struct Scene *scene);
	int (*doNoShadow)(long *ndata,double *xdata,double *ydata,double *zdata,
			   double *vdata,
	           long nel,long CellCount,struct Scene *scene);

	int (*doNoShadowVRML)(long *ndata,long *matdata,float *normal,long normmax,
		            long *normlist,long normlistmax,
                    double *xdata,double *ydata,double *zdata,double *vdata,
                    float *nvdata,long nel,long CellCount,struct Scene *scene,
                    struct tState *tS);
   
    double WalkStepSize;
    double WalkScale;
    double WalkAngle;
    int WalkMode;
    
    struct P ClipPlanePoint;
    struct P ClipPlaneNormal;
    double ClipPlaneDistance;
    double ClipPlaneAngle;
    long ClipPlane;
    
    long AntiAlias;
    long MotionBlur;
    long MotionBlurGather;
    
    double tilt;
    long threads;
    FILE *pipe;
};
struct CObjectListStruct{
	struct CObjectStruct **oOld;
	long oMaximun;
	struct CObjectStruct **oCurrent;
	struct Scene *scene;
	long oCount;
	long oSave;
	long KeyFrameTime;
	char KeyFrameTitle[256];
	struct Scene sceneKeyFrame;
	int Selected;
};

typedef struct CObjectListStruct CObjectList;
typedef struct CObjectListStruct *CObjectListPtr;

struct CKeyFrameStruct{
	CObjectListPtr *oCurrent;
	long oMaximun;
	long oCount;
    long KeyFrame;
};

typedef struct CKeyFrameStruct CKeyFrame;
typedef struct CKeyFrameStruct *CKeyFramePtr;

#define OBJECT_MESSAGE_SET_TIME				1
#define OBJECT_MESSAGE_FORCE_LOAD			2
#define OBJECT_MESSAGE_RANGE_DATA			3
#define OBJECT_MESSAGE_RANGE_DATA_SCREEN	4
#define OBJECT_MESSAGE_MESSAGE1000_TYPE		5
#define OBJECT_MESSAGE_RANGE_LIGHTS			6

struct ObjectMessage{
    int MessageType;
    long time;
    struct Matrix *WorldToScreen;
	struct Message1000 *m;
	long MessageType1000;
	long long SurfaceCount;
    double xmin;
    double ymin;
    double zmin;
    double xmax;
    double ymax;
    double zmax;
	double vmin;
	double vmax;
};

struct CObjectStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
};

typedef struct CObjectStruct CObject;
typedef struct CObjectStruct *CObjectPtr;

struct CWorldStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
};

typedef struct CWorldStruct CWorld;
typedef struct CWorldStruct *CWorldPtr;


struct CGroupStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    long *itemList;
    CObjectPtr *Objects;
    long ObjectCount;
};

typedef struct CGroupStruct CGroup;
typedef struct CGroupStruct *CGroupPtr;

struct CBlockStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct P origin;
    long BlockNumber;
};

typedef struct CBlockStruct CBlock;
typedef struct CBlockStruct *CBlockPtr;

struct CSphereStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct P origin;
    long nx;
    long ny;
    double radius;
};

typedef struct CSphereStruct CSphere;
typedef struct CSphereStruct *CSpherePtr;

struct CTorusStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct P origin;
    long nx;
    long ny;
    double rbig;
    double rsmall;
};

typedef struct CTorusStruct CTorus;
typedef struct CTorusStruct *CTorusPtr;


struct CDiskStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct P origin;
    long nx;
    long ny;
    double rin;
    double rout;
};

typedef struct CDiskStruct CDisk;
typedef struct CDiskStruct *CDiskPtr;


struct CConeStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct P origin;
    long nx;
    long ny;
    double rtop;
    double rbottom;
    double length;
};

typedef struct CConeStruct CCone;
typedef struct CConeStruct *CConePtr;

struct CSds2dExtStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
};

typedef struct CSds2dExtStruct CSds2dExt;
typedef struct CSds2dExtStruct *CSds2dExtPtr;

struct CSds2dRotStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
};

typedef struct CSds2dRotStruct CSds2dRot;
typedef struct CSds2dRotStruct *CSds2dRotPtr;


struct CHdfExtStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
};

typedef struct CHdfExtStruct CHdfExt;
typedef struct CHdfExtStruct *CHdfExtPtr;

struct CHdfRotStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
};

typedef struct CHdfRotStruct CHdfRot;
typedef struct CHdfRotStruct *CHdfRotPtr;

struct CSds3dStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
    long *blockNumbers;
    long blockCount;
};

typedef struct CSds3dStruct CSds3d;
typedef struct CSds3dStruct *CSds3dPtr;

struct CVrmlListStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    struct vrmlList *vrml;
    long CurrentFrame;
    long ImageCount;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    long VrmlNumber;
    long *vrmlNumbers;
    long vrmlCount;
};

typedef struct CVrmlListStruct CVrmlList;
typedef struct CVrmlListStruct *CVrmlListPtr;

struct CVrmlStruct2{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    struct vrmlList *vrml;
    long CurrentFrame;
    long ImageCount;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    long VrmlNumber;
};

typedef struct CVrmlStruct2 CVrml2;
typedef struct CVrmlStruct2 *CVrmlPtr2;



struct CShowStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    long *blockNumbers;
    long blockCount;
    int averageStress;
    int nstress;
};

typedef struct CShowStruct CShow;
typedef struct CShowStruct *CShowPtr;

struct CPio2dExtStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
};

typedef struct CPio2dExtStruct CPio2dExt;
typedef struct CPio2dExtStruct *CPio2dExtPtr;

struct CPio2dRotStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
};

typedef struct CPio2dRotStruct CPio2dRot;
typedef struct CPio2dRotStruct *CPio2dRotPtr;

struct CPio3dLevelStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
    long *blockNumbers;
    long blockCount;
};

typedef struct CPio3dLevelStruct CPio3dLevel;
typedef struct CPio3dLevelStruct *CPio3dLevelPtr;

struct CPio3dExtStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
};

typedef struct CPio3dExtStruct CPio3dExt;
typedef struct CPio3dExtStruct *CPio3dExtPtr;



struct CPio2dRayStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    struct P origin;
    long nx;
    long ny;
    long lineType;
    double radius;
    double LengthTotal;
    double LengthGreatest;
    double radiusStepFactor;
    long numdim;
    double xminData;
    double yminData;
    double zminData;
    double xmaxData;
    double ymaxData;
    double zmaxData;
    long setData;
    long itemToPlot;
    char itemName[256];
};

typedef struct CPio2dRayStruct CPio2dRay;
typedef struct CPio2dRayStruct *CPio2dRayPtr;

struct CPio2dTracerStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    struct P origin;
    long nx;
    long ny;
    long lineType;
    double radius;
    double LengthTotal;
    double LengthGreatest;
    double radiusStepFactor;
    long numdim;
    double xminData;
    double yminData;
    double zminData;
    double xmaxData;
    double ymaxData;
    double zmaxData;
    long setData;
    long itemToPlot;
    char itemName[256];
};

typedef struct CPio2dTracerStruct CPio2dTracer;
typedef struct CPio2dTracerStruct *CPio2dTracerPtr;


struct CFlightPathStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    struct P *flightPath;
    long flightCount;
    long SplineSteps;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    struct P origin;
    long nx;
    long ny;
    long lineType;
    double radius;
    double LengthTotal;
    double LengthGreatest;
    double radiusStepFactor;
    long numdim;
    double xminData;
    double yminData;
    double zminData;
    double xmaxData;
    double ymaxData;
    double zmaxData;
    long setData;
    long itemToPlot;
    char itemName[256];
    long Hide;
    long CyclicSmooth;
};

typedef struct CFlightPathStruct CFlightPath;
typedef struct CFlightPathStruct *CFlightPathPtr;

CFlightPathPtr CFlightPathCreate(struct Scene *scene,struct P *flightPath,long flightCount,long SplineSteps);


struct CStlStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    long *blockNumbers;
    long blockCount;
    int averageStress;
};

typedef struct CStlStruct CStl;
typedef struct CStlStruct *CStlPtr;



struct CPio3dStreamLinesStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    long CurrentFrame;
    long ImageCount;
    long BlockNumber;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    int noShift;
    char BatchName[256];
    struct SetFrameData sd;    
	struct areadata ai;
    char pioName[64];
    int pioIndex;
    char pioNamePaint[64];
    int pioIndexPaint;
    int usePaint;
    double CutMin;
    double CutMax;
    char PaintName[256];
    long *blockNumbers;
    long blockCount;
    struct dataStruct *xyzReturned;
    int streamsReturned;
    struct dataStruct xyz;
    struct P origin;
    int setData;
    double xminData;
    double yminData;
    double zminData;
    double xmaxData;
    double ymaxData;
    double zmaxData;
    long nx;
    long ny;
    long lineType;
    double radius;
    double LengthTotal;
    double LengthGreatest;
    double radiusStepFactor;
    long streamGradients;
    long streamSteps;
    long streamPasses;

};

typedef struct CPio3dStreamLinesStruct CPio3dStreamLines;
typedef struct CPio3dStreamLinesStruct *CPio3dStreamLinesPtr;

struct CVrmlStruct{
/*   start Object Data */
    struct CObjectStruct *((*ReadWrite)(struct CObjectStruct *o,FILE8 *inOut,CObjectList *l));
    int (*killObject)(struct CObjectStruct *o);
    int (*drawObject)(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
    int (*pGetMost)(struct CObjectStruct *o,long *MostNodes,long *MostNormals,long *MostCells);
    struct CObjectStruct *(*duplicateObject)(struct CObjectStruct *o);
    int (*countObject)(struct CObjectStruct *o,long *nodes,long *cells);
    int (*message)(struct CObjectStruct *o,struct ObjectMessage *Message);
	struct PlotAttributes pa;
    struct Scene *scene;
    struct System Local;
    long Material;
    long FirstCell;
    long LastCell;
    long tzero;
    long tmax;
    long thold;
    long repeat;
    long cycleFlag;
    int Selected;
    int type;
    long ItWas;
    long iWrite;
    long ShowNormals;
/*   End Object Data */
    struct FileInfo2 *Files;
    struct FileInfo2 *Files2;
    struct vrmlList *vrml;
    long CurrentFrame;
    long ImageCount;
    double vscale;
    long xsize;
    long ysize;
    long length;
    double LastEntry;
    double *Levels;
    long LevelCount;
    double dmin;
    double dmax;
    long Angles;
    double Angle;
    long PassSmooth;
    long StepLevels;
    long Axis;
    long xmin;
    long xmax;
    long ymin;
    long ymax;
    int blackZero;
    int white255;
    long VrmlNumber;
};

typedef struct CVrmlStruct CVrml;
typedef struct CVrmlStruct *CVrmlPtr;

CVrmlPtr CVrmlCreate(struct Scene *scene,struct FileInfo2 *Files);
CObjectPtr uVrmlWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);



CStlPtr CStlCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);
CObjectPtr CStlWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);

#define Scene_Eye       0
#define Scene_EyeLeft   1
#define Scene_EyeRight  2
#define Scene_Anaglyph  3

#define Walk_Move				0
#define Walk_Rotate				1
#define Walk_Move_Object		2

#define ClipPlane_Off       0
#define ClipPlane_Fixed   	1
#define ClipPlane_Eye  		2


extern double Dot(struct P *v1,struct P *v2);
extern struct Color c(double red,double green,double blue);
extern double Len(struct P *v1);
extern struct P Mult(struct P *p1,double Factor);
extern struct P Add(struct P *p1,struct P *p2);
extern struct P Sub(struct P *p1,struct P *p2);
extern struct P CrossN(struct P *v1,struct P *v2);
extern struct P Norm(struct P *p1);
extern struct P p(double x,double y,double z);
extern int Transf1(struct Trans1 *f);
long getNextMaterial(struct Scene *scene);
int adjustPalette2(struct Scene *scene,int mat);
int SetMaterialProperty(struct Scene *scene,struct Material **Mat,int mat);
int checkPaletteLevels(struct Scene *scene,int mat,double vmin,double vmax);
int startPaletteLevels(struct Scene *scene);
int setPaletteLevels(struct Scene *scene);
#endif


