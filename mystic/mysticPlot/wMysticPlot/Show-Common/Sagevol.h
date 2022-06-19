#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "uFiles.h"
#include "TracerDef.h"
#include "mThread.h"
#include "Universal.h"




#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#ifdef __cplusplus
extern "C" {
#endif
    int DFPrestart(void);
    int DFPgetpal(char *name,char *pal);
    int DFSDrestart(void);
    int DFSDgetdims(char *name,int *rank,int *sizes,int maxrank);
    int DFSDgetdata(char *name,int rank,int *sizes,char *buffer);
    int DFSDlastref(void);
    int DFANputdesc(char *file,unsigned int type,unsigned int tag,char *name,long lenght);
    int DFANgetdesc(char *file,unsigned int type,unsigned int tag,char *name,int length);
#ifdef __cplusplus
}
#endif



#define sgn(x)  ((x < 0) ?  -1 : 1)


      
struct rayData{
	struct vScene *scene;
    char *hasData;
	float *gradx;
	float *grady;
	float *gradz;
	double *daughter;
	double *opacityToFireball;
	double *mean;
	long no;
	long nc;
	double *red;
	double *green;
	double *blue;
	double dmin;
	double dmax;
	double *cdata;
	double *opacity;
	double *mat_opacity;
	double mat_scale;
	long samples;
	struct P Lamp;
	struct P Eye;
	struct P r; 
	tFlt *xcenter;
	tFlt *ycenter;
	tFlt *zcenter;
	tFlt *data;
    unsigned char *level;
	double *dx2;
	double *dy2;
	double *dz2;
	long nemax;
    double CellSizeOver;
    unsigned char xReflect;
    unsigned char yReflect;
    unsigned char zReflect;
    double *r1;
    double *r2;
    double *v;
    long CellCount;
    long CellCountMax;
	long cellDepthError;
	int RayDirection;
	int logscale;
	double a;
	double b;
	int debug;
};

      
      


struct Commands{
    char *name;
    int (*sub)(FilePtr Files,CommandPtr cp,BatchPtr Batch);
};



struct cell{
    double s;
    long ne;
    long nee;
    unsigned char xReflect;
    unsigned char yReflect;
    unsigned char zReflect;
};

#define PAL_RAINBOW					0
#define PAL_SOME_GRAYS				1
#define PAL_BCGYRM_REDUCED			2
#define PAL_BCGYRM					3
#define PAL_NTSC_REDUCED_GREEN_PAL	4
#define PAL_NTSCPAL					5
#define PAL_PS_PAL					6

#define X_PLANE						1
#define Y_PLANE						2
#define Z_PLANE						0

#define Scene_Eye       0
#define Scene_EyeLeft   1
#define Scene_EyeRight  2
#define Scene_Anaglyph  3

double Dot(struct P *v1,struct P *v2);
double Len(struct P *v1);
struct P Mult(struct P *p1,double Factor);
struct P Add(struct P *p1,struct P *p2);
struct P Sub(struct P *p1,struct P *p2);
struct P CrossN(struct P *v1,struct P *v2);
struct P Norm(struct P *p1);
struct P p(double x,double y,double z);
struct Color c(double red,double green,double blue);

int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,
         long Lin,long *Lout,unsigned int mask);

int putInt(long n,FILE *out);
int putLong(long n,FILE *in);

double rtime(void);

int flip(unsigned char *b,int xsize,int ysize);

int sageData(FilePtr Files,long CurrentFrame,char *name,int index);
int sageTrace(FilePtr Files,int flag);
int sageGradients(FilePtr Files);
int sageDoTrace(FilePtr Files);
int sageGetFull(FilePtr Files);
int sageSample(vIconPtr myIcon);
int sageSDS3D(vIconPtr myIcon);
int saveSDS3D(FilePtr Files,char *filename);
int sageRange(FilePtr Files);
int sageLocation(vIconPtr myIcon);
int intcmp1(const void *xx,const  void *yy);
int intcmp2(const void *xx,const  void *yy);
int sageRotate(FilePtr Files,long tiltsteps,long rotatesteps);
int sageSweep(FilePtr Files,long steps);
int sageSweepDown(FilePtr Files,long steps);
int SageCellHitDaughter(long ne,struct rayData *rayin,char *daughter);
int SageProcessRay(long ne,struct rayData *rayin);
int SageCellHit(long ne,struct rayData *rayin);
int SageTraceCellHit(long ne,struct rayData *rayin);
int sageDump(vIconPtr myIcon,long cell);
int SetScreen3(struct vScene *scene);
int openPIO(FilePtr Files,long CurrentFrame);
int doTrace(FilePtr Files);
int TraceSetUp(vIconPtr myIcon);
int TraceFindLines(vIconPtr myIcon);

int closeSDS(vIconPtr myIcon);
int openSDS(vIconPtr myIcon);

int writeTRUE(FilePtr Files);

int sageSetFrame(FilePtr Files,long CurrentFrame);

double interpolate(double d,long count,double *x,double *y);

int openSDS3D(FilePtr Files,long CurrentFrame);

int TraceLines(mThread *Threads);
int SageTraceLines(mThread *Threads);
int SageTraceLines1(mThread *Threads);
int SageTraceLines0(mThread *Threads);
int SageTraceLines2(mThread *Threads);
int SageTraceBangMeter(mThread *Threads);


