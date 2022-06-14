#include <stdio.h>
#define EXTERN22 extern
#include "Xappl.h"
#include "mThread.h"
#include "TracerDef.h"
static int TraceIt(mThread *Threads);
double calc(double sum);
int main (int argc, char * argv [])
{
    struct Scene scene;
    
    scene.xResolution=1000;

    printf("hello\n");
    runThreads(10,&scene,1000,TraceIt);
    printf("good bye\n");
}

static int TraceIt(mThread *Threads)
{
    struct Scene *scene;
    int xResolution;
    double sum;

    if(!Threads)goto ErrorOut;
    scene=(struct Scene *)Threads->data;
    if(!scene)goto ErrorOut;
    
    xResolution=(int)scene->xResolution;
    sum=0;
    for(int j=Threads->smin;j<Threads->smax;++j){
        for(int i=0;i<xResolution;++i){
            sum += calc(sum);
        }
    }
    
    printf("TraceIt smin %ld smax %ld sum %g\n",Threads->smin,Threads->smax,sum);
    
ErrorOut:
    
    Threads->done=TRUE;
    
    
    return 0;
}
double calc(double sum)
{
    sum = sum + 1;
    for(int n=0;n<1000000000;n++){
        sum = sqrt(sum)+atan(sum)+sin(sum);
        sum = sum/(2*fabs(sum));
    }
    return sum;
}


