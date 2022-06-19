#define EXTERN22 extern
#include <stdio.h>
#include <setjmp.h>
#include "Xappl.h"
#include "TracerDef.h"

#ifndef bool
#define bool int
#endif

#define True  1
#define False 0
#define VERTEX0	1
#define VERTEX1	2
#define VERTEX2	4
#define VERTEX3	8
#define VERTEX4	16
#define VERTEX5	32
#define VERTEX6	64
#define VERTEX7	128

#define Edge0	1
#define Edge1	2
#define Edge2	4
#define Edge3	8
#define Edge4	16
#define Edge5	32
#define Edge6	64
#define Edge7	128
#define Edge8	256
#define Edge9	512
#define Edge10	1024
#define Edge11	2048



#define FlagAll		(Edge5 | Edge6 | Edge11)
#define FlagNoX 	(FlagAll | Edge7 | Edge10)
#define FlagNoY 	(FlagAll | Edge4 | Edge9)
#define FlagNoZ 	(FlagAll | Edge1 | Edge2)
#define FlagNoXY 	(FlagAll | Edge7 | Edge10 | Edge4 | Edge9 | Edge8)
#define FlagNoXZ 	(FlagAll | Edge7 | Edge10 | Edge1 | Edge2 | Edge3)
#define FlagNoYZ 	(FlagAll | Edge4 | Edge9 | Edge1 | Edge2 | Edge0)
#define FlagNoXYZ 	(FlagAll | Edge0 | Edge1 | Edge2 | Edge3 | Edge4 | \
				Edge7 | Edge8 | Edge9 | Edge10)
 typedef unsigned char u_char2;
 
 typedef struct float3_st{
     double x;
     double y;
     double z;
  }float3_ft;

 typedef struct int3_st{
     long x;
     long y;
     long z;
  }int3_st;
  
 int3_st *slice[2];
 
static double *xpos;
static double *ypos;
static double *zpos;
static double  *data;

typedef struct table
{
    int		num_tri;
    int		edge_list[12];
} table;

table lookup[128] = { 
{0,{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*   0 */}},
{1, {3, 0, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*   1 */}},
{1,{ 1, 9, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*   2 */}},
{2,{ 3, 1, 8, 1, 9, 8, -1, -1, -1, -1, -1, -1,   		/*   3 */}},
{1, {2, 11, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*   4 */}},
{2,{ 3, 0, 8, 2, 11, 1, -1, -1, -1, -1, -1, -1,   	/*   5 */}},
{2,{ 2, 11, 0, 11, 9, 0, -1, -1, -1, -1, -1, -1,   	/*   6 */}},
{3,{ 3, 2, 11, 8, 3, 11, 9, 8, 11, -1, -1, -1,   		/*   7 */}},

{1,{ 10, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*   8 */}},
{2,{ 10, 2, 0, 10, 0, 8, -1, -1, -1, -1, -1, -1,   	/*   9 */}},
{2,{ 0, 1, 9, 10, 2, 3, -1, -1, -1, -1, -1, -1,   	/*  10 */}},
{3,{ 2, 1, 9, 2, 9, 10, 9, 8, 10, -1, -1, -1,   		/*  11 */}},
{2,{ 10, 11, 3, 3, 11, 1, -1, -1, -1, -1, -1, -1,   	/*  12 */}},
{3,{ 0, 8, 1, 8, 11, 1, 8, 10, 11, -1, -1, -1,   		/*  13 */}},
{3,{ 0, 3, 10, 9, 0, 10, 11, 9, 10, -1, -1, -1,   	/*  14 */}},
{2,{ 10, 11, 8, 11, 9, 8, -1, -1, -1, -1, -1, -1,   	/*  15 */}},

{1, {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*  16 */}},
{2,{ 7, 3, 4, 3, 0, 4, -1, -1, -1, -1, -1, -1,   		/*  17 */}},
{2, {8, 4, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1,   		/*  18 */}},
{3, {4, 7, 9, 7, 1, 9, 7, 3, 1, -1, -1, -1,   		/*  19 */}},
{2,{ 7, 8, 4, 2, 11, 1, -1, -1, -1, -1, -1, -1,   	/*  20 */}},
{3, {7, 3, 4, 3, 0, 4, 2, 11, 1, -1, -1, -1,   		/*  21 */}},
{3, {2, 11, 0, 11, 9, 0, 4, 7, 8, -1, -1, -1,   		/*  22 */}},
{4,{ 4, 3, 7, 4, 11, 3, 9, 11, 4, 11, 2, 3,   		/*  23 */}},

{2, {7, 8, 4, 10, 2, 3, -1, -1, -1, -1, -1, -1,   	/*  24 */}},
{3, {2, 7, 10, 2, 4, 7, 0, 4, 2, -1, -1, -1,   		/*  25 */}},
{3, {7, 8, 4, 0, 1, 9, 3, 10, 2, -1, -1, -1,   		/*  26 */}},
{4, {4, 7, 9, 7, 10, 9, 10, 1, 9, 10, 2, 1,   		/*  27 */}},
{3,{10, 11, 3, 3, 11, 1, 8, 4, 7, -1, -1, -1,   		/*  28 */}},
{4, {10, 4, 7, 10, 1, 4, 1, 0, 4, 11, 1, 10,   		/*  29 */}},
{4, {0, 3, 10, 9, 0, 10, 11, 9, 10, 4, 7, 8,   		/*  30 */}},
{3, {4, 7, 9, 9, 7, 10, 9, 10, 11, -1, -1, -1,   		/*  31 */}},

{1, {5, 4, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*  32 */}},
{2,{ 5, 4, 9, 3, 0, 8, -1, -1, -1, -1, -1, -1,   		/*  33 */}},
{2,{ 1, 5, 0, 5, 4, 0, -1, -1, -1, -1, -1, -1,   		/*  34 */}},
{3, {8, 3, 4, 5, 4, 3, 1, 5, 3, -1, -1, -1,   		/*  35 */}},
{2, {9, 5, 4, 1, 2, 11, -1, -1, -1, -1, -1, -1,   	/*  36 */}},
{3, {3, 0, 8, 2, 11, 1, 5, 4, 9, -1, -1, -1,   		/*  37 */}},
{3, {4, 11, 5, 2, 11, 4, 0, 2, 4, -1, -1, -1,   		/*  38 */}},
{4, {4, 8, 3, 2, 4, 3, 5, 4, 2, 11, 5, 2,   		/*  39 */}},

{2, {9, 5, 4, 10, 2, 3, -1, -1, -1, -1, -1, -1,   	/*  40 */}},
{3, {10, 2, 0, 10, 0, 8, 5, 4, 9, -1, -1, -1,   		/*  41 */}},
{3, {1, 5, 0, 5, 4, 0, 3, 10, 2, -1, -1, -1,   		/*  42 */}},
{4, {2, 8, 10, 2, 5, 8, 5, 2, 1, 8, 5, 4,   		/*  43 */}},
{3, {10, 11, 3, 3, 11, 1, 5, 4, 9, -1, -1, -1,   		/*  44 */}},
{4, {5, 4, 9, 8, 1, 0, 11, 1, 8, 10, 11, 8,   		/*  45 */}},
{4, {4, 3, 11, 5, 4, 11, 3, 10, 11, 3, 4, 0,   		/*  46 */}},
{3, {11, 5, 4, 11, 4, 8, 11, 8, 10, -1, -1, -1,   	/*  47 */}},

{2, {5, 7, 9, 7, 8, 9, -1, -1, -1, -1, -1, -1,   		/*  48 */}},
{3, {0, 9, 5, 5, 3, 0, 7, 3, 5, -1, -1, -1,   		/*  49 */}},
{3, {8, 0, 1, 7, 8, 1, 5, 7, 1, -1, -1, -1,   		/*  50 */}},
{2, {7, 3, 1, 1, 5, 7, -1, -1, -1, -1, -1, -1,   		/*  51 */}},
{3, {1, 2, 11, 8, 9, 7, 9, 5, 7, -1, -1, -1,   		/*  52 */}},
{4, {2, 11, 1, 0, 9, 5, 3, 0, 5, 7, 3, 5,   		/*  53 */}},
{4, {8, 0, 2, 5, 8, 2, 5, 2, 11, 8, 5, 7,   		/*  54 */}},
{3, {3, 2, 11, 3, 11, 5, 3, 5, 7, -1, -1, -1,   		/*  55 */}},

{3, {10, 2, 3, 5, 7, 9, 7, 8, 9, -1, -1, -1,   		/*  56 */}},
{4, {9, 5, 7, 2, 9, 7, 0, 9, 2, 7, 10, 2,   		/*  57 */}},
{4, {10, 2, 3, 8, 0, 1, 7, 8, 1, 1, 5, 7,   		/*  58 */}},
{3, {10, 2, 7, 7, 2, 1, 7, 1, 5, -1, -1, -1,   		/*  59 */}},
{4, {11, 1, 10, 1, 3, 10, 9, 5, 7, 9, 7, 8,   		/*  60 */}},
{3, {1, 0, 9, 10, 5, 7, 10, 11, 5, -1, -1, -1,   		/*  61 */}},
{3, {8, 0, 3, 5, 10, 11, 5, 7, 10, -1, -1, -1,   		/*  62 */}},
{2, {5, 10, 11, 5, 7, 10, -1, -1, -1, -1, -1, -1,   	/*  63 */}},

{1, {5, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/*  64 */}},
{2, {5, 11, 6, 0, 8, 3, -1, -1, -1, -1, -1, -1,   	/*  65 */}},
{2, {5, 11, 6, 1, 9, 0, -1, -1, -1, -1, -1, -1,   	/*  66 */}},
{3, {5, 11, 6, 3, 1, 8, 1, 9, 8, -1, -1, -1,   		/*  67 */}},
{2, {2, 6, 1, 6, 5, 1, -1, -1, -1, -1, -1, -1,   		/*  68 */}},
{3, {3, 0, 8, 2, 6, 1, 6, 5, 1, -1, -1, -1,   		/*  69 */}},
{3, {0, 5, 9, 6, 5, 0, 2, 6, 0, -1, -1, -1,   		/*  70 */}},
{4, {3, 2, 6, 6, 9, 3, 3, 9, 8, 6, 5, 9,   		/*  71 */}},

{2, {2, 3, 10, 5, 11, 6, -1, -1, -1, -1, -1, -1,   	/*  72 */}},
{3, {6, 5, 11, 8, 10, 0, 10, 2, 0, -1, -1, -1,   		/*  73 */}},
{3, {2, 3, 10, 5, 11, 6, 0, 1, 9, -1, -1, -1,   		/*  74 */}},
{4, {6, 5, 11, 2, 1, 9, 10, 2, 9, 8, 10, 9,   		/*  75 */}},
{3, {10, 6, 5, 3, 10, 5, 1, 3, 5, -1, -1, -1,   		/*  76 */}},
{4, {6, 5, 1, 8, 6, 1, 10, 6, 8, 8, 1, 0,   		/*  77 */}},
{4, {3, 10, 0, 10, 6, 0, 6, 9, 0, 6, 5, 9,   		/*  78 */}},
{3, {10, 6, 5, 10, 5, 9, 10, 9, 8, -1, -1, -1,   		/*  79 */}},

{2, {5, 11, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1,   	/*  80 */}},
{3, {5, 11, 6, 7, 3, 4, 3, 0, 4, -1, -1, -1,   		/*  81 */}},
{3, {6, 5, 11, 8, 4, 7, 9, 0, 1, -1, -1, -1,   		/*  82 */}},
{4, {11, 6, 5, 9, 4, 7, 7, 1, 9, 3, 1, 7,   		/*  83 */}},
{3, {7, 8, 4, 2, 6, 1, 1, 6, 5, -1, -1, -1,   		/*  84 */}},
{4, {1, 2, 5, 2, 6, 5, 3, 0, 7, 0, 4, 7,   		/*  85 */}},
{4, {0, 5, 9, 6, 5, 0, 2, 6, 0, 7, 8, 4,   		/*  86 */}},
{3, {5, 9, 4, 2, 7, 3, 6, 7, 2, -1, -1, -1,   		/*  87 */}},

{3, {4, 7, 8, 11, 6, 5, 10, 2, 3, -1, -1, -1,   		/*  88 */}},
{4, {5, 11, 6, 2, 7, 10, 2, 4, 7, 0, 4, 2,   		/*  89 */}},
{4, {2, 3, 10, 5, 11, 6, 1, 9, 0, 8, 4, 7,   		/*  90 */}},
{3, {4, 5, 9, 1, 11, 2, 7, 10, 6, -1, -1, -1,   		/*  91 */}},
{4, {8, 4, 7, 1, 3, 5, 10, 5, 3, 5, 10, 6,   		/*  92 */}},
{3, {7, 10, 6, 0, 5, 1, 0, 4, 5, -1, -1, -1,   		/*  93 */}},
{3, {3, 8, 0, 5, 9, 4, 6, 7, 10, -1, -1, -1,   		/*  94 */}},
{2, {4, 5, 9, 7, 10, 6, -1, -1, -1, -1, -1, -1,   	/*  95 */}},

{2, {9, 11, 4, 11, 6, 4, -1, -1, -1, -1, -1, -1,   	/*  96 */}},
{3, {9, 11, 4, 11, 6, 4, 8, 3, 0, -1, -1, -1,   		/*  97 */}},
{3, {6, 1, 11, 0, 1, 6, 4, 0, 6, -1, -1, -1,   		/*  98 */}},
{4, {8, 3, 4, 3, 11, 4, 11, 6, 4, 11, 1, 3,   		/*  99 */}},
{3, {2, 9, 1, 4, 9, 2, 6, 4, 2, -1, -1, -1,   		/* 100 */}},
{4, {8, 3, 0, 9, 1, 2, 4, 9, 2, 6, 4, 2,   		/* 101 */}},
{2, {2, 6, 4, 4, 0, 2, -1, -1, -1, -1, -1, -1,   		/* 102 */}},
{3, {4, 8, 3, 4, 3, 2, 4, 2, 6, -1, -1, -1,   		/* 103 */}},

{3, {2, 3, 10, 9, 11, 4, 11, 6, 4, -1, -1, -1,   		/* 104 */}},
{4, {9, 11, 4, 11, 6, 4, 10, 2, 0, 10, 0, 8,   		/* 105 */}},
{4, {3, 10, 2, 1, 11, 6, 0, 1, 6, 4, 0, 6,   		/* 106 */}},
{3, {11, 1, 2, 4, 6, 8, 6, 10, 8, -1, -1, -1,   		/* 107 */}},
{4, {4, 9, 6, 9, 3, 6, 9, 1, 3, 3, 10, 6,   		/* 108 */}},
{3, {0, 1, 9, 10, 8, 6, 8, 4, 6, -1, -1, -1,   		/* 109 */}},
{3, {0, 3, 10, 0, 10, 6, 0, 6, 4, -1, -1, -1,   		/* 110 */}},
{2, {8, 6, 4, 8, 10, 6, -1, -1, -1, -1, -1, -1,   	/* 111 */}},

{3, {6, 7, 8, 11, 6, 8, 9, 11, 8, -1, -1, -1,   		/* 112 */}},
{4, {7, 3, 0, 11, 7, 0, 9, 11, 0, 6, 7, 11,   		/* 113 */}},
{4, {11, 0, 1, 6, 0, 11, 8, 0, 6, 7, 8, 6,   		/* 114 */}},
{3, {1, 11, 6, 1, 6, 7, 3, 1, 7, -1, -1, -1,   		/* 115 */}},
{4, {7, 8, 9, 9, 2, 7, 7, 2, 6, 2, 9, 1,   		/* 116 */}},
{3, {0, 9, 1, 7, 2, 6, 3, 2, 7, -1, -1, -1,   		/* 117 */}},
{3, {6, 7, 8, 6, 8, 0, 6, 0, 2, -1, -1, -1,   		/* 118 */}},
{2, {7, 2, 6, 3, 2, 7, -1, -1, -1, -1, -1, -1,   		/* 119 */}},

{4, {3, 10, 2, 8, 9, 11, 11, 6, 8, 6, 7, 8,   		/* 120 */}},
{3, {10, 6, 7, 0, 11, 2, 0, 9, 11, -1, -1, -1,   		/* 121 */}},
{3, {10, 6, 7, 8, 0, 3, 1, 11, 2, -1, -1, -1,   		/* 122 */}},
{2, {10, 6, 7, 1, 11, 2, -1, -1, -1, -1, -1, -1,   	/* 123 */}},
{3, {10, 6, 7, 8, 1, 3, 8, 9, 1, -1, -1, -1,   		/* 124 */}},
{2, {10, 6, 7, 1, 0, 9, -1, -1, -1, -1, -1, -1,   	/* 125 */}},
{2, {0, 3, 8, 10, 6, 7, -1, -1, -1, -1, -1, -1,   	/* 126 */}},
{1, {10, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,   	/* 127 */}},
    };
static double	xend, yend, zend;
static long npoint,ntriangle;
static  int	nlayer,jinc,kinc;

struct Nodes{
    double x1,y1,z1;
    double x2,y2,z2;
    double x3,y3,z3;
};
static double *xdata,*ydata,*zdata;
static long *elements;
static long MaxNodes,MaxNel;

int debug;

static jmp_buf env77;

int Ploti(struct Block *b,double *datai,double threshold,int imax,int jmax,int kmax,
double *xposi,double *yposi,double *zposi);
int first_slice(double	threshold);
int first_row(double	threshold, int		z);
long get_edge(double threshold,int flag,int z,int y,int x, int this_edge);
unsigned char get_index(double threshold,int y,int x,int *rev);
int polygon_wt(struct Block *b,double threshold);
int read_slice(double *datai,int nlayer,int imax,int jmax);
long add_point(float3_ft *p);
long add_triangle(long v1,long  v2,long  v3);

int doPloti(struct Block *b,struct LIMITS *limits,double t,double *data,long imax,long jmax,long kmax);

int range3D(struct LIMITS *limits,long imax,long jmax,long kmax,double **xposi,double **yposi,double **zposi);

int doPloti(struct Block *b,struct LIMITS *limits,double t,double *data,long imax,long jmax,long kmax)
{
 	double *xposi,*yposi,*zposi;
 	int ret;
 
	ret=1;
 	xposi=NULL;
 	yposi=NULL;
 	zposi=NULL;


	if(range3D(limits,imax,jmax,kmax,&xposi,&yposi,&zposi))goto End;

	ret=Ploti(b,data,t,(int)imax,(int)jmax,(int)kmax,xposi,yposi,zposi);
End:

	if(xposi)cFree((char *)xposi);
	if(yposi)cFree((char *)yposi);
	if(zposi)cFree((char *)zposi);

	return ret;
}
int range3D(struct LIMITS *limits,long imax,long jmax,long kmax,double **xposi,double **yposi,double **zposi)
{
 	double *xpos,*ypos,*zpos,den;
 	char buff[256];
 	long length,k;
 	int ret;
 
	ret=1;

 	xpos=NULL;
 	ypos=NULL;
 	zpos=NULL;

	length=imax;
	if((xpos=(double *)cMalloc(length*sizeof(double),8495)) == NULL){
	    sprintf(buff,"Out of memory Requested (%ld)\n",length*sizeof(double));
		WarningBatch(buff);
	    goto End;
	}
	
	if(limits && ((limits->iGotData & 63) == 63)){
		for(k=0;k<imax;++k){
		    xpos[k]=limits->xmin+((double)k+0.5)*(limits->xmax-limits->xmin)/(double)(imax);
		}
	}else{
		den=.5*((double)(imax-1));
		for(k=0;k<imax;++k)xpos[k]=((double)k)-den;
	}
	
	length=jmax;
	if((ypos=(double *)cMalloc(length*sizeof(double),8496)) == NULL){
	    sprintf(buff,"Out of memory Requested (%ld)\n",length*sizeof(double));
		WarningBatch(buff);
	    goto End;
	}

	if(limits &&  ((limits->iGotData & 63) == 63)){
		for(k=0;k<jmax;++k){
		    ypos[k]=limits->ymin+((double)k+0.5)*(limits->ymax-limits->ymin)/(double)(jmax);
		}
	}else{
		den=.5*((double)(jmax-1));
		for(k=0;k<jmax;++k)ypos[k]=((double)k)-den;
	}
	

	length=kmax;
	if((zpos=(double *)cMalloc(length*sizeof(double),8497)) == NULL){
	    sprintf(buff,"Out of memory Requested (%ld)\n",length*sizeof(double));
		WarningBatch(buff);
	    goto End;
	}

	if(limits &&  ((limits->iGotData & 63) == 63)){
		for(k=0;k<kmax;++k){
		    zpos[k]=limits->zmin+((double)k+0.5)*(limits->zmax-limits->zmin)/(double)(kmax);
		}
	}else{
		den=.5*((double)(kmax-1));
		for(k=0;k<kmax;++k)zpos[k]=((double)k)-den;
	}
		
	
	ret=0;
End:
	if(ret){
	    if(xpos)cFree((char *)xpos);
	    if(ypos)cFree((char *)ypos);
	    if(zpos)cFree((char *)zpos);
	    xpos=NULL;
	    ypos=NULL;
	    zpos=NULL;
	}

	*xposi = xpos;
	*yposi = ypos;
	*zposi = zpos;

	return ret;

}
int Ploti(struct Block *block,double *datai,double threshold,int imax,int jmax,int kmax,
          double *xposi,double *yposi,double *zposi)
{
    u_char2	index;
    char buff[256];
    int		i, j, k, l, e1, e2, e3;
    long	v1, v2, v3;
    int		flag, rev,ret;
    long length,sides;
    


    ret=1;
    slice[0]=NULL;
    slice[1]=NULL;
    xdata=NULL;
    ydata=NULL;
    zdata=NULL;
    elements=NULL;
    npoint=0;
    ntriangle=0;
    sides=1000+2L*(imax*(long)jmax)+2L*(imax*(long)kmax)+2L*(jmax*(long)kmax);
    MaxNel=sides;
    MaxNodes=sides;

    if(!(xdata=(double *)cMalloc(MaxNodes*sizeof(double),8533))){
		sprintf(buff,"Ploti Out of memory Requested (%ld)\n",MaxNodes*sizeof(double));
		WarningBatch(buff);
		goto End;
    }

    if(!(ydata=(double *)cMalloc(MaxNodes*sizeof(double),8534))){
		sprintf(buff,"Ploti Out of memory Requested (%ld)\n",MaxNodes*sizeof(double));
		WarningBatch(buff);
		goto End;
    }

    if(!(zdata=(double *)cMalloc(MaxNodes*sizeof(double),8535))){
		sprintf(buff,"Ploti Out of memory Requested (%ld)\n",MaxNodes*sizeof(double));
		WarningBatch(buff);
		goto End;
    }

    if(!(elements=(long *)cMalloc(4*MaxNel*sizeof(long),8536))){
		sprintf(buff,"Ploti Out of memory Requested (%ld)\n",4*MaxNel*sizeof(long));
		WarningBatch(buff);
		goto End;
    }

    nlayer=0;
    jinc=imax;
    kinc=imax*jmax;
    length=kinc;
    
    if(!(slice[0]=(int3_st *)cMalloc(length*sizeof(int3_st),8537))){
		sprintf(buff,"Ploti Out of memory Requested (%ld)\n",length*sizeof(int3_st));
		WarningBatch(buff);
		goto End;
    }

    if(!(slice[1]=(int3_st *)cMalloc(length*sizeof(int3_st),8538))){
		sprintf(buff,"Ploti Out of memory Requested (%ld)\n",length*sizeof(int3_st));
		WarningBatch(buff);
		goto End;
    }

    data=datai;
    xend = (double)(imax - 1);
    yend = (double)(jmax - 1);
    zend = (double)(kmax - 1);

	xpos = xposi;
	ypos = yposi;
	zpos = zposi;
    if(setjmp(env77)){
	goto End;
    }
	
    read_slice(datai,nlayer,imax,jmax);

    first_slice(threshold);

    read_slice(datai,++nlayer,imax,jmax);

    for (i=1; i<zend; i++)
    {						/* over z */
	first_row(threshold,i);

	for (j=1; j<yend; j++)
	{					/* over y */
	    index = get_index(threshold, j, 0, &rev);

	    flag = FlagNoX;

	    for (l=0; l<lookup[index].num_tri; l++)
	    {
		e1 = lookup[index].edge_list[l*3];
		e2 = lookup[index].edge_list[l*3+1];
		e3 = lookup[index].edge_list[l*3+2];

		v1 = get_edge(threshold, flag, i, j, 0, e1);
		v2 = get_edge(threshold, flag, i, j, 0, e2);
		v3 = get_edge(threshold, flag, i, j, 0, e3);

		flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

		if (rev)
		{
		    add_triangle(v3, v2, v1);
		}
		else
		{
		    add_triangle(v1, v2, v3);
		}
	    }

	    for (k=1; k<xend; k++)
	    {					/* over x */
		index = get_index(threshold, j, k, &rev);

		flag = FlagAll;

		for (l=0; l<lookup[index].num_tri; l++)
		{
		    e1 = lookup[index].edge_list[l*3];
		    e2 = lookup[index].edge_list[l*3+1];
		    e3 = lookup[index].edge_list[l*3+2];

		    v1 = get_edge(threshold, flag, i, j, k, e1);
		    v2 = get_edge(threshold, flag, i, j, k, e2);
		    v3 = get_edge(threshold, flag, i, j, k, e3);

		    flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

		    if (rev)
		    {
			add_triangle(v3, v2, v1);
		    }
		    else
		    {
			add_triangle(v1, v2, v3);
		    }
		}
	    }
	}
	if (i < zend)
	{
	    read_slice(datai,++nlayer,imax,jmax);
	}
    }
    if(slice[0])cFree((char *)slice[0]);
    slice[0]=NULL;
    if(slice[1])cFree((char *)slice[1]);
    slice[1]=NULL;
    ret=polygon_wt(block,threshold);
End:
    if(slice[0])cFree((char *)slice[0]);
    if(slice[1])cFree((char *)slice[1]);
    if(xdata)cFree((char *)xdata);
    if(ydata)cFree((char *)ydata);
    if(zdata)cFree((char *)zdata);
    if(elements)cFree((char *)elements);
    return ret;
}
int first_slice(double	threshold)
{
    u_char2	 index;
    int		flag, rev, i, j, l;
    int		e1, e2, e3;
    long	v1, v2, v3;

    index = get_index(threshold, 0, 0, &rev);

    flag = FlagNoXYZ;

    for (l=0; l<lookup[index].num_tri; l++)
    {
	e1 = lookup[index].edge_list[l*3];
	e2 = lookup[index].edge_list[l*3+1];
	e3 = lookup[index].edge_list[l*3+2];

	v1 = get_edge(threshold, flag, 0, 0, 0, e1);
	v2 = get_edge(threshold, flag, 0, 0, 0, e2);
	v3 = get_edge(threshold, flag, 0, 0, 0, e3);

	flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

	if (rev)
	{
	    add_triangle(v3, v2, v1);
	}
	else
	{
	    add_triangle(v1, v2, v3);
	}
    }

    for (i=1; i<xend; i++)
    {
	index = get_index(threshold, 0, i, &rev);

	flag = FlagNoYZ;

	for (l=0; l<lookup[index].num_tri; l++)
	{
	    e1 = lookup[index].edge_list[l*3];
	    e2 = lookup[index].edge_list[l*3+1];
	    e3 = lookup[index].edge_list[l*3+2];

	    v1 = get_edge(threshold, flag, 0, 0, i, e1);
	    v2 = get_edge(threshold, flag, 0, 0, i, e2);
	    v3 = get_edge(threshold, flag, 0, 0, i, e3);

	    flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

	    if (rev)
	    {
		add_triangle(v3, v2, v1);
	    }
	    else
	    {
		add_triangle(v1, v2, v3);
	    }
	}
    }

    for (i=1; i<yend; i++)
    {
	index = get_index(threshold, i, 0, &rev);

	flag = FlagNoXZ;

	for (l=0; l<lookup[index].num_tri; l++)
	{
	    e1 = lookup[index].edge_list[l*3];
	    e2 = lookup[index].edge_list[l*3+1];
	    e3 = lookup[index].edge_list[l*3+2];

	    v1 = get_edge(threshold, flag, 0, i, 0, e1);
	    v2 = get_edge(threshold, flag, 0, i, 0, e2);
	    v3 = get_edge(threshold, flag, 0, i, 0, e3);

	    flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

	    if (rev)
	    {
		add_triangle(v3, v2, v1);
	    }
	    else
	    {
		add_triangle(v1, v2, v3);
	    }
	}

	for (j=1; j<xend; j++)
	{
	    index = get_index(threshold, i, j, &rev);

	    flag = FlagNoZ;

	    for (l=0; l<lookup[index].num_tri; l++)
	    {
		e1 = lookup[index].edge_list[l*3];
		e2 = lookup[index].edge_list[l*3+1];
		e3 = lookup[index].edge_list[l*3+2];

		v1 = get_edge(threshold, flag, 0, i, j, e1);
		v2 = get_edge(threshold, flag, 0, i, j, e2);
		v3 = get_edge(threshold, flag, 0, i, j, e3);

		flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

		if (rev)
		{
		    add_triangle(v3, v2, v1);
		}
		else
		{
		    add_triangle(v1, v2, v3);
		}
	    }
	}
    }
    return 0;
}

int first_row(double	threshold, int		z)
{
    u_char2  index;
    int		flag, rev, i, l;
    int		e1, e2, e3;
    long	v1, v2, v3;

    index = get_index(threshold, 0, 0, &rev);

    flag = FlagNoXY;

    for (l=0; l<lookup[index].num_tri; l++)
    {
	e1 = lookup[index].edge_list[l*3];
	e2 = lookup[index].edge_list[l*3+1];
	e3 = lookup[index].edge_list[l*3+2];

	v1 = get_edge(threshold, flag, z, 0, 0, e1);
	v2 = get_edge(threshold, flag, z, 0, 0, e2);
	v3 = get_edge(threshold, flag, z, 0, 0, e3);

	flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

	if (rev)
	{
	    add_triangle(v3, v2, v1);
	}
	else
	{
	    add_triangle(v1, v2, v3);
	}
    }

    for (i=0; i<xend; i++)
    {
	index = get_index(threshold, 0, i, &rev);

	flag = FlagNoY;

	for (l=0; l<lookup[index].num_tri; l++)
	{
	    e1 = lookup[index].edge_list[l*3];
	    e2 = lookup[index].edge_list[l*3+1];
	    e3 = lookup[index].edge_list[l*3+2];

	    v1 = get_edge(threshold, flag, z, 0, i, e1);
	    v2 = get_edge(threshold, flag, z, 0, i, e2);
	    v3 = get_edge(threshold, flag, z, 0, i, e3);

	    flag &= (~((1<<e1) | (1<<e2) | (1<<e3)));

	    if (rev)
	    {
			add_triangle(v3, v2, v1);
	    }
	    else
	    {
			add_triangle(v1, v2, v3);
	    }
	}
    }
    return 0;
}
long get_edge(double threshold,int flag,int z,int y,int x, int this_edge)
{
    double	d1, d2, t;
    float3_ft	p;
    long root,yjincx;

    root=nlayer*kinc+y*jinc+x;
    yjincx=y*jinc+x;
    switch(this_edge)
    {
	case 0:
	    if (flag & Edge0)
	    {
		d1 = data[root];
		d2 = data[root+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = (xpos[x] + t * (xpos[x+1] - xpos[x]));
		p.y = ypos[y];
		p.z = zpos[z];

		slice[0][yjincx].x = add_point(&p);

	    }
	    return(slice[0][yjincx].x);
	case 1:
	    if (flag & Edge1)
	    {
		d1 = data[root+1];		
		d2 = data[root+jinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x+1];
		p.y = (ypos[y] + t * (ypos[y+1] - ypos[y]));
		p.z = zpos[z];

		slice[0][y*jinc+x+1].y = add_point(&p);

	    }
	    return(slice[0][yjincx+1].y);
	case 2:
	    if (flag & Edge2)
	    {
		d1 = data[root+jinc];
		d2 = data[root+jinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = (xpos[x] + t * (xpos[x+1] - xpos[x]));
		p.y = ypos[y+1];
		p.z = zpos[z];

		slice[0][yjincx+jinc].x = add_point(&p);

	    }
	    return(slice[0][yjincx+jinc].x);
	case 3:
	    if (flag & Edge3)
	    {
		d1 = data[root];
		d2 = data[root+jinc];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x];
		p.y = (ypos[y] + t * (ypos[y+1] - ypos[y]));
		p.z = zpos[z];

		slice[0][yjincx].y = add_point(&p);

	    }
	    return(slice[0][yjincx].y);
	case 4:
	    if (flag & Edge4)
	    {
	    
		d1 = data[root+kinc];
		d2 = data[root+kinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = (xpos[x] + t * (xpos[x+1] - xpos[x]));
		p.y = ypos[y];
		p.z = zpos[z+1];

		slice[1][yjincx].x = add_point(&p);

	    }
	    return(slice[1][yjincx].x);
	case 5:
	    if (flag & Edge5)
	    {
		d1 = data[root+kinc+1];
		d2 = data[root+kinc+jinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x+1];
		p.y = (ypos[y] + t * (ypos[y+1] - ypos[y]));
		p.z = zpos[z+1];

		slice[1][yjincx+1].y = add_point(&p);

	    }
	    return(slice[1][yjincx+1].y);

	case 6:
	    if (flag & Edge6)
	    {
		d1 = data[root+kinc+jinc];
		d2 = data[root+kinc+jinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = (xpos[x] + t * (xpos[x+1] - xpos[x]));
		p.y = ypos[y+1];
		p.z = zpos[z+1];

		slice[1][yjincx+jinc].x = add_point(&p);

	    }
	    return(slice[1][yjincx+jinc].x);
	case 7:
	    if (flag & Edge7)
	    {
		d1 = data[root+kinc];
		d2 = data[root+kinc+jinc];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x];
		p.y = (ypos[y] + t * (ypos[y+1] - ypos[y]));
		p.z = zpos[z+1];

		slice[1][yjincx].y = add_point(&p);

	    }
	    return(slice[1][yjincx].y);
	case 8:
	    if (flag & Edge8)
	    {
		d1 = data[root];
		d2 = data[root+kinc];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x];
		p.y = ypos[y];
		p.z = (zpos[z] + t * (zpos[z+1] - zpos[z]));

		slice[0][yjincx].z = add_point(&p);
	    }
	    return(slice[0][yjincx].z);
	case 9:
	    if (flag & Edge9)
	    {
		d1 = data[root+1];
		d2 = data[root+kinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x+1];
		p.y = ypos[y];
		p.z = (zpos[z] + t * (zpos[z+1] - zpos[z]));

		slice[0][yjincx+1].z = add_point(&p);

	    }
	    return(slice[0][yjincx+1].z);
	case 10:
	    if (flag & Edge10)
	    {
		d1 = data[root+jinc];
		d2 = data[root+kinc+jinc];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x];
		p.y = ypos[y+1];
		p.z = (zpos[z] + t * (zpos[z+1] - zpos[z]));

		slice[0][yjincx+jinc].z = add_point(&p);

	    }
	    return(slice[0][yjincx+jinc].z);
	case 11:
	    if (flag & Edge11)
	    {
		d1 = data[root+jinc+1];
		d2 = data[root+kinc+jinc+1];
		t = (threshold - d1) / (d2 - d1);

		p.x = xpos[x+1];
		p.y = ypos[y+1];
		p.z = (zpos[z] + t * (zpos[z+1] - zpos[z]));

		slice[0][yjincx+jinc+1].z = add_point(&p);

	    }
	    return(slice[0][yjincx+jinc+1].z);
    }
    return 0;
}
unsigned char get_index(double threshold,int y,int x,int *rev)
{
    unsigned char       index = 0;
    long root;

    root=nlayer*kinc+y*jinc+x;
    if (data[root] > threshold)
    {
        index = index | VERTEX0;
    }
    if (data[root+1] > threshold)
    {
        index = index | VERTEX1;
    }
    if (data[root+jinc+1] > threshold)
    {
        index = index | VERTEX2;
    }
    if (data[root+jinc] > threshold)
    {
        index = index | VERTEX3;
    }
    if (data[root+kinc] > threshold)
    {
        index = index | VERTEX4;
    }
    if (data[root+kinc+1] > threshold)
    {
        index = index | VERTEX5;
    }
    if (data[root+kinc+jinc+1] > threshold)
    {
        index = index | VERTEX6;
    }
    if (data[root+kinc+jinc] > threshold)
    {
        index = index | VERTEX7;
    }
    
    if (index > 127)
    {			
        index = ~index;
	*rev = False;
    }
    else
    {
	*rev = True;
    }
    return(index);
}
int polygon_wt(struct Block *block,double threshold)
{
	char buff[256];
	double *vdata,*fsave;
	long *isave;
	int ret,n;
	

	ret=1;
	vdata=NULL;

	zerol((char *)block,(long)sizeof(struct Block));

	if(debug){
	    sprintf(buff,"npoint %ld ntriangle %ld\n",npoint,ntriangle);
	    WarningBatch(buff);
	}
	
	MaxNodes = npoint;
	MaxNel   = ntriangle;

	if(MaxNodes == 0 || MaxNel == 0){
	    ret=0;
	    goto End;
	}

		fsave=xdata;
        if(!(xdata=(double *)cRealloc((char *)xdata,MaxNodes*sizeof(double),8778))){
            if(fsave)cFree((char *)fsave);
            sprintf(buff,"polygon_wt xdata Out of Memory %ld\n",MaxNodes*sizeof(double));
	        WarningBatch(buff);
            goto End;
        }
		fsave=ydata;
        if(!(ydata=(double *)cRealloc((char *)ydata,MaxNodes*sizeof(double),8779))){
            if(fsave)cFree((char *)fsave);
            sprintf(buff,"polygon_wt ydata Out of Memory %ld\n",MaxNodes*sizeof(double));
	        WarningBatch(buff);
            goto End;
        }
		fsave=zdata;
        if(!(zdata=(double *)cRealloc((char *)zdata,MaxNodes*sizeof(double),8780))){
            if(fsave)cFree((char *)fsave);
            sprintf(buff,"polygon_wt zdata Out of Memory %ld\n",MaxNodes*sizeof(double));
	        WarningBatch(buff);
            goto End;
        }
		isave=elements;
        if(!(elements=(long *)cRealloc((char *)elements,4*MaxNel*sizeof(long),8781))){
            if(isave)cFree((char *)isave);
            sprintf(buff,"polygon_wt elements Out of Memory %ld\n",4*MaxNel*sizeof(long));
	        WarningBatch(buff);
            goto End;
        }

        if(!(vdata=(double *)cMalloc(MaxNodes*sizeof(double),8539))){
            sprintf(buff,"polygon_wt vdata Out of Memory %ld\n",MaxNodes*sizeof(double));
	        WarningBatch(buff);
            goto End;
        }


        block->xdata=xdata;
        block->ydata=ydata;
        block->zdata=zdata;
        block->vdata=vdata;
        block->NodeCount=npoint;

		for(n=0;n<npoint;++n){
		    vdata[n]=threshold;
		}


        xdata=NULL;
        ydata=NULL;
        zdata=NULL;
        vdata=NULL;

        block->ElementCount=ntriangle;
        block->Elements=elements;
        block->NodesMax=3;

        elements=NULL;
	ret=0;
End:
        if(ret){
            if(vdata)cFree((char *)vdata);
            vdata=NULL;
        }else{
            return 0;
	}
        return ret;
}   
int read_slice(double *datai,int nlayer,int imax,int jmax)
{
 	int3_st *save;

	//datai=datai;
	//nlayer=nlayer;
	//imax=imax;
	//jmax=jmax;
	save=slice[0];
	slice[0]=slice[1];
	slice[1]=save;
	return 0;
}
long add_point(float3_ft *p)
{
	char buff[256];
	double *save;
	
    if(npoint >= MaxNodes){
        MaxNodes *= 2;
        save=xdata;
        if(!(xdata=(double *)cRealloc((char *)xdata,MaxNodes*sizeof(double),8782))){
            cFree((char *)save);
            sprintf(buff,"add_point xdata Out of Memory %ld\n",MaxNodes*sizeof(double));
            WarningBatch(buff);
            longjmp(env77,1);
        }
        save=ydata;
        if(!(ydata=(double *)cRealloc((char *)ydata,MaxNodes*sizeof(double),8783))){
            cFree((char *)save);
            sprintf(buff,"add_point ydata Out of Memory %ld\n",MaxNodes*sizeof(double));
            WarningBatch(buff);
            longjmp(env77,1);
        }
        save=zdata;
        if(!(zdata=(double *)cRealloc((char *)zdata,MaxNodes*sizeof(double),8784))){
            cFree((char *)save);
            sprintf(buff,"add_point zdata Out of Memory %ld\n",MaxNodes*sizeof(double));
            WarningBatch(buff);
            longjmp(env77,1);
        }

    }
    xdata[npoint]=p->x;
    ydata[npoint]=p->y;
    zdata[npoint]=p->z;
    return npoint++;
}
long add_triangle(long v1,long  v2,long  v3)
{
	char buff[256];
	long *save;
	long n;

	if(ntriangle >= MaxNel){
	    MaxNel *= 2;
	    	save=elements;
            if(!(elements=(long *)cRealloc((char *)elements,4*MaxNel*sizeof(long),8785))){
               	cFree((char *)save);
                sprintf(buff,"add_triangle Out of Memory Triangles %ld Points %ld Memory %ld\n",
                		ntriangle,npoint,4*MaxNel*sizeof(long));
            	WarningBatch(buff);
            	longjmp(env77,1);
            }
	}
	n=4*ntriangle;
	elements[n]=1;
	elements[n+1]=v1;
	elements[n+2]=v2;
	elements[n+3]=v3;
        ++ntriangle;
    return 0L;
}

