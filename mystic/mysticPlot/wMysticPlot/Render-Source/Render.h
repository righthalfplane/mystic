#ifndef __RENDER__
#define __RENDER__

#include "uFiles.h"
#include "TracerDef.h"
#include "mThread.h"

extern IconPtr  doNewRenderWindow(IconPtr myIcon);

struct rStackData{
    struct P Target;
    struct P Eye;
    struct Sphere lamp;
    double Degrees;
    int WalkMode;
};


struct renderInfo{
    CKeyFrame CKeyFrames;
    struct rStackData stack;
    pushpopPtr popStack;
    struct Scene *scene;
    IconPtr myIcon;
    long CurrentFrame;
    long ImageCount;
    unsigned char *buffer;
    long xsize;
    long ysize;
    int Tool;
    uPoint p1;
    uPoint p2;
    double theta1,theta2;
    uRect windowRect;
    int defaultsSet;
    int mat;

    long milliSecondsPerFrame;
    unsigned long TimeToFlip;
    
    double UpdatePauseTime;
    
    int AnimationOn;
    int  LoopBackAndForth;
    int LoopFlag;
      
	struct FileInfo2 Files;
    
    int WalkMotion;
    int WalkMotionContinuous;
    
    int WalkCursorOn;
    int WalkCursorX;
    int WalkCursorY;
    int WalkCursorStep;
    
    long WalkObjectCount;
    CFlightPathPtr WalkObject;

    
    /*   save junk */
    int saveWhere;
    FILE *saveFILE;
    char *saveDirectory;
    char *saveName;
    char *saveFileName;
    struct FileInfo2 *saveFiles;
    /*  save junk end */
};

typedef struct renderInfo *renderPtr;


#define ControlModeRotateLocal		0
#define ControlModeDrag				1
#define ControlModeZoom				2
#define ControlModeSetTarget		3
#define ControlModeRotateGlobal		4
#define ControlModeDragGlobal		5
#define ControlModeSpinGlobal		6
#define ControlModeSelect			7
#define ControlModeFly				8


int CenterScreen(struct Scene *scene,int flag);

int ListReadWriteObjectList(struct Scene *scene,CObjectList *d,FILE8 *inOut,int flag);

int RenderAddPaint(CSds2dExtPtr s,struct FileList *Files);
 
CObjectPtr uPio2dExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CPio2dExtPtr CPio2dExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);
CObjectPtr uPio3dExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CPio3dExtPtr CPio3dExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);
CPio2dRotPtr CPio2dRotCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);
CObjectPtr uPio2dRotWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CPio3dLevelPtr CPio3dLevelCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);
CObjectPtr uPio3dLevelWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CPio2dTracerPtr CPio2dTracerCreate(struct Scene *scene,struct FileInfo2 *Files,long numdim);
CObjectPtr uPio2dTracerWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CObjectPtr uPio2dRayWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
int rotate3d(struct System *Local);

int RenderFlightAnimation(IconPtr myIcon);

int RenderSceneSetTime(struct Scene *scene);

#endif
