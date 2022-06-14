#include <stdio.h>

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

struct SHOW{
    FILE *out;
    long nodeCount;
    long valueCount;
    long normalCount;
    long dxyzCount;
    long rsCount;
    long localSystem;
    long elementCount;
    long nodesMax;
    long stressCount;
    long stressMax;
    long pc;
    long *elements;
    long velCount;
    double *x;
    double *y;
    double *z;
    double *v;
    double *nx;
    double *ny;
    double *nz;
    double *dx;
    double *dy;
    double *dz;
    double *r;
    double *s;
    double *stress;
    double time;
    double *xvel;
    double *yvel;
    double *zvel;
};
typedef struct SHOW *showPtr;

showPtr showStart(char *name,char *mode);
int showEnd(showPtr s);
int showDone(showPtr s);
int showRS(showPtr s,long nodeCount,double *rr,double *ss);
int showDisplacements(showPtr s,long nodeCount,double *dx,double *dy,double *dz);
int showVelocities(showPtr s,long nodeCount,double *xvel,double *yvel,double *zvel);
int showValues(showPtr s,long nodeCount,double *v);
int showNormals(showPtr s,long nodeCount,double *nx,double *ny,double *nz);
int showNodes(showPtr s,long nodeCount,double *x,double *y,double *z);
int showElements(showPtr s,long elementCount,long nodesMax,long *elements);
int showStress(showPtr s,long nodeCount,double *stress);
int showStressFloat(showPtr s,long nodeCount,float *stress,long ibq);
int showTime(showPtr s,double time);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

