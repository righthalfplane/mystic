#define EXTERN22 extern
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Sagevol.h"
#include "Universal.h"

int DFerrno(void);


static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp);

int sds3dRange(FilePtr Files);

int Sleep2(int ticks);

static int doSageVolumeInformation(struct FileInfo2 *Files,struct SetFrameData  *sd);

static int doSageVolumeGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd);

     
static int doSageVolumeLineData(struct FileInfo2 *Files,struct linedata *li);
              
int trueOut(char *filepath,char *name,long xsize,long ysize,unsigned char *image,long n,int flag);

static int doSageVolumeDoDialog(struct FileInfo2 *Files,int *type);
                   
int doSageVolStart(struct FileInfo2 *Files);
int doVolumeSelection(struct FileInfo2 *Files);

int setDefaults(FilePtr Files);
int writeTRUEthread(vIconPtr myIcon);
int FileTrueWrite(char *Header,unsigned char *buffer,
                  long xsize,long ysize,long UnCompressed,
	          long Compressed,unsigned int Esc1,
	          unsigned int Esc2,FILE *out);
	          
int doDataSelection(struct FileInfo2 *Files);

int doDataSelectionSDS3D(struct FileInfo2 *Files);
	
char *DefaultPathString(void);
          
int doRange(vIconPtr myIcon);

int sdsOut(char *path,char *name,char *limits,int rank,int *size,double *data,long n);

int sagePrintFull(vIconPtr myIcon,int flag);

#define SUM(n)   (((n) & 1)  ? (((n)+2)*((n)/2)+1):(((n)+1)*((n)/2)))

typedef struct OrbitStruct{
    long orbits;
    long targetloops;
    long in;
    long around;
    long out;
    long stepframes;
    double targetradius;
    double targetdegrees;
    struct P offset;
} * OrbitPtr;

typedef struct MoveStruct{
    long Steps;
    long stepframes;
    double targetradius;
    double findDegrees;
    struct P FindPoint;
} * MovePtr;

static int doOrbit(FilePtr Files,OrbitPtr Orbitin);

int sageGradHighX(mThread *Threads);
int sageGradLowX(mThread *Threads);
int sageGradLowY(mThread *Threads);
int sageGradHighY(mThread *Threads);
int sageGradLowZ(mThread *Threads);
int sageGradHighZ(mThread *Threads);

static int CommandGet(struct FileInfo2 *Files,CommandPtr Commands);

int getSDSLimits(char *name,long *imax,long *jmax,long *kmax,double **data);

static int doOrbitSetUp(FilePtr Files,CommandPtr cp,BatchPtr Batch);
static int doPrint(FilePtr Files,CommandPtr cp,BatchPtr Batch);
static int doSphere(FilePtr Files,CommandPtr cp,BatchPtr Batch);
int doSage(FilePtr Files,CommandPtr cp,BatchPtr Batch);
static int doMoveSetUp(FilePtr Files,CommandPtr cp,BatchPtr Batch);

static int flip2(unsigned char *b,int xsize,int ysize);

static int doMove(FilePtr Files,MovePtr Orbitin);

static struct Commands clist[]=
    {
		{"print",doPrint},
		{"sagemove",doMoveSetUp},
		{"sageorbit",doOrbitSetUp},
		{"dump",doSphere},
		{"use",doSphere},
		{"sagesweep",doSage},
		{"sagesweepdown",doSage},
		{"sagerotate",doSage},
		{"sagerange",doSage},
		{"sagesample",doSage},
		{"savesds3d",doSage},
		{"sagetrace",doSage},
		{"sagedata",doSage},
		{"datatype",doSage},
		{"pos",doSage},
		{"sagefile",doSage},
	    {"opacity",doSphere},
		{"range",doSphere},
		{"threads",doSphere},
		{"trace",doSphere},
		{"palette",doSphere},
		{"debug",doSphere},
		{"AspectRatio",doSphere},
		{"Height",doSphere},
		{"Degrees",doSphere},
		{"xResolution",doSphere},
		{"yResolution",doSphere},
		{"xsize",doSphere},
		{"ysize",doSphere},
		{"BackGround",doSphere},
		{"Eye",doSphere},
		{"Lamp",doSphere},
		{"Target",doSphere},
		{"Up",doSphere},
		{"opensds3d",doSphere},
		{"xmin",doSphere},
		{"ymin",doSphere},
		{"zmin",doSphere},
		{"xmax",doSphere},
		{"ymax",doSphere},
		{"zmax",doSphere},
		{"dmin",doSphere},
		{"dmax",doSphere},
		{"logscale",doSphere},
		{"limits",doSphere},
		{"reflect",doSphere},
		{"stop",doSphere},
		{"end",doSphere},
		{"savename",doSphere},
		{"filelist",doSphere},
		{"sequence",doSphere},
	
    };


static int doRenderKeys(FilePtr Files,CommandPtr cp,BatchPtr Batch);

static int doClose(FilePtr Files);

int doSageVolBatch(BatchPtr Batch,char *name)
{
	struct CommandInfo cp;
	FilePtr Files;
	char line[4096];
	volatile int isWriting;
	int ret;
	
    if(!Batch || !name)return 1;
    
    Files=FileManagerGet(name);
    
    if(!Files)return 1;
    
	zerol((char *)&Files->vmyIcon,sizeof(struct vIcon));

	zerol((char *)&cp,sizeof(struct CommandInfo));
	
	isWriting=FALSE;
	
	Files->vmyIcon.isWriting=&isWriting;
	
	Files->vmyIcon.ImageCount=Files->ImageCount;
	Files->vmyIcon.pio.CurrentFrame = -1;

	setDefaults(Files);
		
    ret=1;
	while(1){
	    if(BatchNextLine(Batch,line,sizeof(line)))break;
		if(getCommand(line,&cp))break;
	    if(doRenderKeys(Files,&cp,Batch))break;
	}
    
    ret=0;
    
    while(isWriting){
        WarningBatch("Wait doSageVolBatch isWriting\n");    
    	Sleep2(60);
    }
	
     doClose(Files);
 	 getCommand(NULL,&cp);
     return ret;
}
static int doRenderKeys(FilePtr Files,CommandPtr cp,BatchPtr Batch)
{
	char *command;
	int n,nList;

	if(!cp || !Files || !Batch)return 1;

	nList=sizeof(clist)/sizeof(struct Commands);

	for(cp->n=0;cp->n<cp->nword;++cp->n){
		command=stringCommand(cp);
		if(!command)return 1;
		for(n=0;n<nList;++n){
			if(!mstrcmp(clist[n].name,command)){
				if((*(clist[n].sub))(Files,cp,Batch))return 1;
				break;
			}
		}
		if(n >= nList){
			 sprintf(WarningBuff,"Unknown Command : %s\n",command);
			 WarningBatch(WarningBuff);
			 break;
		}
	}
	return 0;
}

static int CommandGet(struct FileInfo2 *Files,CommandPtr cp)
{
	char *command;
	int ret;

	if(!Files || !cp)return 1;
	ret = 1;
	
	for(cp->n=0;cp->n<cp->nword;++cp->n){
		command=stringCommand(cp);
		if(!command)break;
		if(CommandGetProcess(Files,cp))break;
	}
	
	
	ret = 0;
	return ret;
}
static int CommandGetProcess(struct FileInfo2 *Files,CommandPtr cp)
{
	struct CommandInfo cpl;
	/* char line[4096]; */
	struct vScene *scene;
 	vIconPtr myIcon;
	char *command;
 	double value;
	SagePtr pio;
	long count,n;
	char *name;
	int index;
	int ret;

	if(!Files || !cp)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	pio=&myIcon->pio;
	ret = 1;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));

	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	if(!mstrcmp("trace",command)){
	    doTrace(Files);
	}else if(!mstrcmp("palette",command)){	
		 ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(scene->palname,command,255);
	}else if(!mstrcmp("print",command)){	mstrncpy(scene->palname,"ps",255);

		 ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		if(!mstrcmp("eye",command)){
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->PrintEye=(int)value;
		}else if(!mstrcmp("time",command)){
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->PrintTime=(int)value;
		}else if(!mstrcmp("spheres",command)){
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->PrintSpheres=(int)value;
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->sphereSize=value;
		}
	}else if(!mstrcmp("sequence",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->trueCount=(long)value;
        sprintf(WarningBuff,"sequence %ld \n",myIcon->trueCount);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("opacity",command)){
	    long n;
	    
	    sprintf(WarningBuff,"opacity\n");
		if(scene->debug > 1)WarningBatch(WarningBuff);
		
		++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    count=(long)value;
	    if(count <= 0 || count >= 128){
	        sprintf(WarningBuff,"opacity count %ld exceeds 128 or is zero\n",count);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    
	    for(n=0;n<count;++n){
			++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->tvalue[n]=value;
			++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->topacity[n]=value;
	        sprintf(WarningBuff,"n %ld level %g opacity %g\n",n,scene->tvalue[n],scene->topacity[n]);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	    }
	    scene->tcount=count;
	}else if(!mstrcmp("dump",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    count=(long)value;
	    if(count <= 0 || count >= 126){
	        sprintf(WarningBuff,"dump count %ld exceeds 126 or is zero\n",count);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    sprintf(WarningBuff,"dump count %ld\n",count);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	    for(n=0;n<count;++n){
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        sageDump(myIcon,(long)value);
	    }
	}else if(!mstrcmp("range",command)){
	    doRange(myIcon);
	}else if(!mstrcmp("logscale",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->logscale=(int)value;
	    sprintf(WarningBuff,"logscale  %d \n",myIcon->logscale);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("dmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->dmin=value;
	    myIcon->dminSet=1;
	    sprintf(WarningBuff,"dmin  %g \n",myIcon->dmin);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("dmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->dmax=value;
	    myIcon->dmaxSet=1;
	    sprintf(WarningBuff,"dmax  %g \n",myIcon->dmax);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmin=value;
	}else if(!mstrcmp("xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmax=value;
	}else if(!mstrcmp("ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymin=value;
	}else if(!mstrcmp("ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymax=value;
	}else if(!mstrcmp("zmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmin=value;
	}else if(!mstrcmp("zmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmax=value;
	}else if(!mstrcmp("Degrees",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Degrees=value;
	    sprintf(WarningBuff,"Degrees  %f \n",scene->Degrees);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("threads",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->threads=(int)value;
	    sprintf(WarningBuff,"threads  %d\n",myIcon->threads);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("xResolution",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->xResolution=(long)value;
	    sprintf(WarningBuff,"xResolution  %ld\n",scene->xResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("xsize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->xResolution=(long)value;
	    sprintf(WarningBuff,"xsize  %ld\n",scene->xResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("yResolution",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->yResolution=(long)value;
	    sprintf(WarningBuff,"yResolution  %ld\n",scene->yResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("ysize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->yResolution=(long)value;
	    sprintf(WarningBuff,"ysize  %ld\n",scene->yResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("AspectRatio",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->AspectRatio=value;
	    sprintf(WarningBuff,"AspectRatio  %f\n",scene->AspectRatio);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Height",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Height=value;
	    sprintf(WarningBuff,"Height  %g\n",scene->Height);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("debug",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->debug=(int)value;
	    sprintf(WarningBuff,"debug  %d\n",scene->debug);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("use",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    if(!mstrcmp(command,"voxels")){
	        myIcon->TraceType=0;
	    }else if(!mstrcmp(command,"rays")){
	        myIcon->TraceType=1;
	    }else if(!mstrcmp(command,"samples")){
	        myIcon->TraceType=2;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->xSamples=(long)value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->ySamples=(long)value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->zSamples=(long)value;
	        sprintf(WarningBuff,"use  %s xSamples %ld ySamples %ld zSamples %ld\n",command,
	               myIcon->xSamples,myIcon->ySamples,myIcon->zSamples);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        return 0;
	    }else if(!mstrcmp(command,"sds3d")){
	        myIcon->TraceType=3;
	    }else if(!mstrcmp(command,"depth")){
	        myIcon->TraceType=4;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->CellSize=value;
	        if(myIcon->CellSize <= 0.0)myIcon->CellSize=1.0;
	        sprintf(WarningBuff,"depth  CellSize %g\n",myIcon->CellSize);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        return 0;
	    }else if(!mstrcmp(command,"group")){
	        myIcon->TraceType=5;
	        ++(cp->n);
			command=stringCommand(cp);
			if(!command)goto ErrorOut;
	        if(scene->groupname)cFree((char *)scene->groupname);
	        scene->groupname=strsave(command,9982);
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupTheta1=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupTheta2=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupThetan=(long)value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupPhi1=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupPhi2=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupPhin=(long)value;
	        sprintf(WarningBuff,"group  groupname %s\n",scene->groupname);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        sprintf(WarningBuff,"       groupTheta1 %g groupTheta2 %g groupThetan %ld\n",scene->groupTheta1,scene->groupTheta2,scene->groupThetan);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        sprintf(WarningBuff,"       groupPhi1   %g groupPhi2   %g groupPhin   %ld\n",scene->groupPhi1,scene->groupPhi2,scene->groupPhin);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        scene->xResolution=scene->groupThetan;
	        sprintf(WarningBuff,"xResolution  %ld\n",scene->xResolution);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        scene->yResolution=scene->groupPhin;
	        sprintf(WarningBuff,"yResolution  %ld\n",scene->yResolution);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        return 0;
	    }
	    sprintf(WarningBuff,"use  %s\n",command);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("BackGround",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->BackGround.red=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->BackGround.green=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->BackGround.blue=value;
	    sprintf(WarningBuff,"BackGround red %f green %f blue %f\n",
	    		scene->BackGround.red,scene->BackGround.green,scene->BackGround.blue);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Eye",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp)){
			command=stringCommand(cp);
			if(!command)goto ErrorOut;
			if(!mstrcmp("left",command)){
			   	scene->EyeView=Scene_EyeLeft;
			    sprintf(WarningBuff,"Eyes - Left View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("right",command)){
			   scene->EyeView=Scene_EyeRight;
			    sprintf(WarningBuff,"Eyes - Right View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("normal",command)){
			   scene->EyeView=Scene_Eye;
			    sprintf(WarningBuff,"Eyes - Normal View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("anaglyph",command)){
			   scene->EyeView=Scene_Anaglyph;
			    sprintf(WarningBuff,"Eyes - Anaglyph View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("seperation",command)){
			    ++(cp->n);
			    if(doubleCommand(&value,cp))goto ErrorOut;
	    		scene->EyeSeparation=value;
	    		sprintf(WarningBuff,"Eye - Separation %g\n",scene->EyeSeparation);
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}    	    
	    }else{
		    scene->Eye.x=value;
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->Eye.y=value;
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->Eye.z=value;
		    sprintf(WarningBuff,"Eye x %g y %g z %g\n",scene->Eye.x,scene->Eye.y,scene->Eye.z);
			if(scene->debug > 1)WarningBatch(WarningBuff);
		}
	}else if(!mstrcmp("Lamp",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Lamp.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Lamp.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Lamp.z=value;
	    sprintf(WarningBuff,"Lamp x %g y %g z %g\n",scene->Lamp.x,scene->Lamp.y,scene->Lamp.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Target",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Target.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Target.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Target.z=value;
	    sprintf(WarningBuff,"Target x %g y %g z %g\n",scene->Target.x,scene->Target.y,scene->Target.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Up",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Up.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Up.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Up.z=value;
	    sprintf(WarningBuff,"Up x %g y %g z %g\n",scene->Up.x,scene->Up.y,scene->Up.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("limits",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmin=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmax=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymin=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymax=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmin=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmax=value;
	    sprintf(WarningBuff,"limits xmin %g ymin %g zmin %g\n",
	            myIcon->xmin,myIcon->ymin,myIcon->zmin);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"       xmax %g ymax %g zmax %g\n",
	            myIcon->xmax,myIcon->ymax,myIcon->zmax);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("reflect",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->xReflect=(int)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->yReflect=(int)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->zReflect=(int)value;
	    sprintf(WarningBuff,"reflect xReflect %d yReflect %d zReflect %d\n",
	            scene->xReflect,scene->yReflect,scene->zReflect);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("opensds3d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(myIcon->filename,command,256);
	    openSDS(myIcon);
	}else if(!mstrcmp("datatype",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    name=command;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    index=(int)value;	    
	    mstrncpy(pio->pioName,name,255);	
		pio->pioIndex=index;
	    sprintf(WarningBuff,"datatype pioName %s pioIndex %d\n",
	            pio->pioName,pio->pioIndex);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("savename",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(scene->savename,command,256);
	    /*
	}else if(!mstrcmp("filelist",command)){
	    if(FileManagerBatch(Batch,cp))goto ErrorOut;	
	    */
	}else if(!mstrcmp("stop",command)){
	    closeSDS(myIcon);
	    goto ErrorOut;
	}else if(!mstrcmp("end",command)){
	    closeSDS(myIcon);
	    goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
 	getCommand(NULL,&cpl);
	return ret;
}
int doSageVolStart(struct FileInfo2 *Files)
{
	
	if(!Files)return 1;

	setDefaults(Files);
		
	Files->pGetAreaData=NULL;
	
	Files->pGetLineData=NULL;

	Files->pDoDialog=doSageVolumeDoDialog;
	
	Files->pInformation=doSageVolumeInformation;
	
	Files->pGetCellData=NULL;

	Files->pGetLineData=doSageVolumeLineData;
	
	Files->pCommandSend=CommandGet;
		
	Files->pGetData2=doSageVolumeGetData;
	
	
	return 0;
}
static int doSageVolumeLineData(struct FileInfo2 *Files,struct linedata *li)
{
	struct vScene *scene;
	long CurrentFrame;
	double *data,step,scale;
	long length;
	vIconPtr myIcon;
	double *xD,*yD;
	double dmin,dmax;
	double min;
	long ns,n,nd,nn;
	SagePtr pio;
	
	if(!li)return 1;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	scene=&myIcon->scene;
	
    xD=NULL;
    yD=NULL;
	
	li->dCount=0;
   	li->xData=NULL;
	li->yData=NULL;

	if(li->type != LINEDATA_HISTOGRAM_DATA)return 1;
	

	if(li->pioName[0]){
	    mstrncpy(pio->pioName,li->pioName,255);
	}else{
	    mstrncpy(pio->pioName,"density",255);
	}
	
	pio->pioIndex=(int)li->pioIndex;
	
	CurrentFrame=li->CurrentFrame;
	

	ns=50;
	
	if((li->PointLineCount > 2) && (li->PointLineCount < 20000))ns=li->PointLineCount;

	if(!(xD=(double *)cMalloc((long)sizeof(double)*(ns),81874))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}
	if(!(yD=(double *)cMalloc((long)sizeof(double)*(ns),81883))){
	    WarningBatch("doSage2DGetLineData Out of List Memory\n");
	    goto ErrorOut;
	}


	
	if(Files->fileClass == SDS3D_FILES){
        if(openSDS3D(Files,CurrentFrame))goto ErrorOut;
        
        length=myIcon->ixmax*myIcon->iymax*myIcon->izmax;
        data=myIcon->data;
		if(!data)goto ErrorOut;
		dmin=data[0];
		dmax=data[0];
		for(n=1;n<length;++n){
		    if(data[n] > dmax){
		       dmax=data[n];
		    }else if(data[n] < dmin){
		       dmin=data[n];
		    }
		}
		
		if(dmin >=  dmax){
		    dmin=dmax-0.1*dmax-1.0;
		}
		step=(dmax-dmin)/(double)ns;
		scale=1.0/step;
		
		min=dmin;

		for(n=0;n<ns;++n){		
		    xD[n]=min;
		    yD[n]=0;
			min += step;
		}
		
		for(nn=0;nn<length;++nn){
		    if(data[nn] <= dmin){
		       n=0;
		    }else if(data[nn] >= dmax){
		       n=ns-1;
		    }else{
		       n=(int)(scale*(data[nn]-dmin));
		    }
		    
		    ++yD[n];
		}						
		
		nd=ns;
		
	}else{
        if(openPIO(Files,CurrentFrame))goto ErrorOut;
        if(!pio->daughter || !pio->value)goto ErrorOut;
        
		dmax=scene->vmax;
		dmin=scene->vmin;
        
        
		if(dmin >=  dmax){
		    dmin=dmax-0.1*dmax-1.0;
		}
		step=(dmax-dmin)/(double)ns;
		scale=1.0/step;
		
		min=dmin;

		for(n=0;n<ns;++n){		
		    xD[n]=min;
		    yD[n]=0;
			min += step;
		}

		for(nn=0;nn<pio->numcell;++nn){
	    	if(pio->daughter[nn])continue;
		    if(pio->value[nn] <= dmin){
		       n=0;
		    }else if(pio->value[nn]  >= dmax){
		       n=ns-1;
		    }else{
		       n=(int)(scale*(pio->value[nn]-dmin));
		    }
		    
		    ++yD[n];
		}						
		
		nd=ns;
                
	}
	
	if(!nd)goto ErrorOut;

	li->dCount=nd;
   	li->xData=xD;
	li->yData=yD;
/*
	li->rymin=rymin;
	li->rymax=rymax;
*/	
	return 0;
ErrorOut:
	if(xD)cFree((char *)xD);
	if(yD)cFree((char *)yD);
	return 1;
}
static int doSageVolumeGetData(struct FileInfo2 *Files,long CurrentFrame,struct SetFrameData  *sd)
{
	struct vScene *scene;
	unsigned char *bline;
	vIconPtr myIcon;
    double *cdata;
	long length,n;
	double stime;
	double etime;
	SagePtr pio;
    int ret;


	if(!Files)return 1;	
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	scene=&myIcon->scene;

	stime=rtime();

	ret=1;
	
	if(sd->type != FRAME_DATA_FLOAT && sd->type != FRAME_DATA_RASTER){
	    sprintf(WarningBuff,"doSageVolumeGetData - Only Returns FRAME_DATA_FLOAT && FRAME_DATA_RASTER\n");
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	if(sd->pioName[0]){
	    mstrncpy(pio->pioName,sd->pioName,255);
	    pio->pioIndex=sd->pioIndex;
	}else if(!pio->pioName[0]){
	    mstrncpy(pio->pioName,"density",255);
	    pio->pioIndex=0;
	}
	
	
	myIcon->CurrentFrame=CurrentFrame;

	
	if(sd->type == FRAME_DATA_FLOAT){
		if(Files->fileClass == SDS3D_FILES){
	        if(openSDS3D(Files,CurrentFrame))goto ErrorOut;
			doRange(myIcon);
		}else{
	        if(openPIO(Files,CurrentFrame))goto ErrorOut;
		}

        sd->limits.vmax=scene->vmax;
        sd->limits.vmin=scene->vmin;
        
        return 0;
	
	}else{
	
		if(Files->fileClass == SDS3D_FILES){
	        if(openSDS3D(Files,CurrentFrame))goto ErrorOut;
			doRange(myIcon);
		}	
		
		myIcon->scene.noWrite=TRUE;

		sageTrace(Files,0);
		
		bline=(unsigned char *)scene->cdata;
		cdata=scene->cdata;
		length=scene->xResolution*scene->yResolution*3;
		for(n=0;n<length;++n){
		    bline[n]=(unsigned char)(cdata[n]*255);
		}

		flip2(bline,(int)scene->xResolution,(int)scene->yResolution);
		        			
	    sd->xsize=scene->xResolution;
	    sd->ysize=scene->yResolution;
	    sd->zsize=1;
	    length=sd->xsize*sd->ysize;
	    sd->length=length;
	    sd->data=scene->cdata;
        sd->limits.vmax=scene->vmax;
        sd->limits.vmin=scene->vmin;
    
    }
        
    ret=0;
    
ErrorOut:

	etime=rtime()-stime;

	sprintf(WarningBuff,"Time To Do Volumetric Ray Tracing %.2f Seconds\n",etime);
	if(scene->debug > 0)WarningBatch(WarningBuff);
	
	return ret;

}
static int doSageVolumeInformation(struct FileInfo2 *Files,struct SetFrameData  *sd)
{
    struct PlotRangeData *range;
	struct FilePIOInfo *pio;  /* only Here */
	SagePtr pio2;
	vIconPtr myIcon;
	
	if(!Files)return 1;
	if(!sd)return 1;
	pio=&Files->pioData;
	range=&sd->range;
	myIcon=&Files->vmyIcon;
	pio2=&myIcon->pio;
	
	sd->range=pio->range;
	sd->pd=pio->pd;
	sd->plane=pio->plane;
	sd->vector=pio->vector;

	if(!Files->pioName[0]){
	    mstrncpy(Files->pioName,"density",255);
	    Files->pioIndex=0;
	}
	
    mstrncpy(sd->pioName,Files->pioName,255);
    sd->pioIndex=(int)Files->pioIndex;
    mstrncpy(pio->pioName,Files->pioName,255);
    pio->pioIndex=(int)Files->pioIndex;
    mstrncpy(pio2->pioName,Files->pioName,255);
    pio2->pioIndex=(int)Files->pioIndex;
    
    sd->CurrentFrame=Files->ImageCount;    
	    
	if(Files->fileClass == PIO3D_FILES){
		sageRange(Files);
	}else if(Files->fileClass == SDS3D_FILES){
		if(sds3dRange(Files))goto ErrorOut;
	}
	
	range->xminData=myIcon->xmin;
	range->xmaxData=myIcon->xmax;
	range->yminData=myIcon->ymin;
	range->ymaxData=myIcon->ymax;
	range->zminData=myIcon->zmin;
	range->zmaxData=myIcon->zmax;
		
ErrorOut:
	return 0;
}
int sds3dRange(FilePtr Files)
{
	vIconPtr myIcon;
	struct vScene *scene;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dmin,dmax;
	double dx,dy,dz;
	double d,size;
	double *data;
	long count;
	long n;
	int ret;
	double *mat_opacity;
	double mat_scale;
	int nomat;
	long samples;
	int i,j,k;

	ret=1;

	if(!Files)goto ErrorOut;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;

	ret=2;

	if(openSDS3D(Files,myIcon->CurrentFrame))return 1;
	
	dmin = myIcon->dmin;
	dmax = myIcon->dmax;
	data = myIcon->data;

	if(!data)goto ErrorOut;
	
	if(dmin >= dmax){
	    dmin=     1e60;
		dmax=    -1e60;
		for(k=0;k<myIcon->ixmax*myIcon->iymax*myIcon->izmax;++k){
		    size=data[k];
			if(size < dmin)dmin=size;
			if(size > dmax)dmax=size;
		}
		myIcon->dmin = dmin;
		myIcon->dmax = dmax;
	}

	if(TraceSetUp(myIcon))goto ErrorOut;

	ret=3;

	ret=4;

    samples=scene->samples;

	mat_opacity=scene->mat_opacity;
	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	xmin=ymin=zmin=     1e60;
	xmax=ymax=zmax=    -1e60;

	dx=(myIcon->xmax-myIcon->xmin)/(double)myIcon->ixmax;
	dy=(myIcon->ymax-myIcon->ymin)/(double)myIcon->iymax;
	dz=(myIcon->zmax-myIcon->zmin)/(double)myIcon->izmax;

	n=0;
	count=0;
	for(k=0;k<myIcon->izmax;++k){
		for(j=0;j<myIcon->iymax;++j){
			for(i=0;i<myIcon->ixmax;++i){
		    	d=data[n];
		    	++n;
			    if(d < dmin || d > dmax)continue;

			    nomat=(int)((d-dmin)*mat_scale);
			    if(nomat < 0){
			        nomat=0;
			    }else if(nomat >= samples){
			       nomat = (int)(samples-1);
			    }


			    if(mat_opacity[nomat] <= 0.0){
			        continue;
			    }

			    size=myIcon->xmin+dx*(double)i;
			    if(size < xmin)xmin=size;
			    if(size > xmax)xmax=size;
			    size=myIcon->xmin+dx*(double)(i+1);
			    if(size < xmin)xmin=size;
			    if(size > xmax)xmax=size;

			    size=myIcon->ymin+dy*(double)j;
			    if(size < ymin)ymin=size;
			    if(size > ymax)ymax=size;
			    size=myIcon->ymin+dy*(double)(j+1);
			    if(size < ymin)ymin=size;
			    if(size > ymax)ymax=size;


			    size=myIcon->zmin+dz*(double)k;
			    if(size < zmin)zmin=size;
			    if(size > zmax)zmax=size;
			    size=myIcon->zmin+dz*(double)(k+1);
			    if(size < zmin)zmin=size;
			    if(size > zmax)zmax=size;
			    ++count;
			}
		}
	}

	if(scene->debug > 1){
		sprintf(WarningBuff,"Sage Range Total Cells %ld Visible %ld\n",n,count);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"xmin %g xmax %g\n",xmin,xmax);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"ymin %g ymax %g\n",ymin,ymax);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"zmin %g zmax %g\n",zmin,zmax);
		WarningBatch(WarningBuff);
	}

	myIcon->xmin=xmin;
	myIcon->xmax=xmax;
	myIcon->ymin=ymin;
	myIcon->ymax=ymax;
	myIcon->zmin=zmin;
	myIcon->zmax=zmax;

	ret=0;

ErrorOut:
	if(ret){
	    sprintf(WarningBuff,"sageRange Error ret %d\n",ret);
		WarningBatch(WarningBuff);
	}
	return ret;
}
int doDataScaleSDS3D(struct FileInfo2 *Files);
int doDataScale(struct FileInfo2 *Files);
static int doSageVolumeDoDialog(struct FileInfo2 *Files,int *type)
{
	if(!Files || !type)return 1;
	
	if(*type == MessageType_DO_SELECTION_DIALOG){
	    if(Files->fileClass == PIO3D_FILES){
		    return doDataSelection(Files);
		}else if(Files->fileClass == SDS3D_FILES){
		    return doDataSelectionSDS3D(Files);
		}	
	}else if(*type == MessageType_DO_SCALES_DIALOG){
	    if(Files->fileClass == PIO3D_FILES){
		    return doDataScale(Files);
		}else if(Files->fileClass == SDS3D_FILES){
		    return doDataScaleSDS3D(Files);
		}	
	}else if(*type == MessageType_DO_AREA_DIALOG){
		return doVolumeSelection(Files);	
	}
	return 0;
}
int setDefaults(FilePtr Files)
{
	struct vScene *scene;
	vIconPtr myIcon;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;

	scene=&myIcon->scene;

	scene->BackGround=  c(1.0,1.0,1.0);
	scene->Eye=         p(2000.0,2000.0,2000.0);
	scene->Lamp=        p(2000.0,2000.0,3000.0);
	scene->Target=      p(0.0,0.0,0.0);
	scene->Up=          p(0.0,0.0,1.0);
	scene->Degrees=        20.0;
	scene->xResolution=     400;
	scene->yResolution=     400;
	scene->AspectRatio=     1.0;
	scene->Height=          1.0;
	scene->debug=             0;
	scene->samples=      262144;
	scene->PrintTime=         1;
	mstrncpy(scene->savename,"test",255);

	scene->EyeSeparation=0;
	
	scene->tcount=2;
	scene->tvalue[0]=1;
	scene->tvalue[1]=10;
	
	scene->topacity[0]=1;
	scene->topacity[1]=1;
	
    if(Files->fileClass == PIO3D_FILES){
	    myIcon->TraceType=4;
	}else if(Files->fileClass == SDS3D_FILES){
	    myIcon->TraceType=3;
	}	
		
	mstrncpy(scene->palname,"ps",255);
	
	return 0;

}
int writeTRUE(FilePtr Files)
{
    vIconPtr myIcon2;
    vIconPtr myIcon;
	long length;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	
	if(myIcon->scene.noWrite)return 0;

	if(!myIcon->isWriting)return 1;

	myIcon2=(vIconPtr)cMalloc(sizeof(struct vIcon),72373);
	if(!myIcon2)return 1;

	*myIcon2 = *myIcon;
	length=myIcon->scene.xResolution*myIcon->scene.yResolution*3;
	myIcon2->FileInfo=Files;

	myIcon->scene.cdata=(double *)cMalloc(length*sizeof(double),52361);
	if(!myIcon->scene.cdata)return 1;

	myIcon->trueCount++;
	
	if(*myIcon->isWriting){
	    myIcon2->isWriting=NULL;
	    writeTRUEthread(myIcon2);
	}else{	
	    *myIcon->isWriting = TRUE;
		if(launchThread((void *)myIcon2,(int (*)(void *data))(writeTRUEthread))){
		    sprintf(WarningBuff,"writeTRUE launchThread Failed\n");
			WarningBatch(WarningBuff);
		}	
	}
	return 0;
}
int writeTRUEthread(vIconPtr myIcon)
{
	
	FilePtr Files;
	struct vScene *scene;
	char path[2048];
	char name[256],*name1;
	unsigned char *bline;
	double *cdata;
	long length;
	double end;
	long n;

	if(!myIcon)return 1;
	Files=myIcon->FileInfo;
	if(!Files)return 1;
	

	scene=&myIcon->scene;
	bline=(unsigned char *)scene->cdata;
	cdata=scene->cdata;
	length=scene->xResolution*scene->yResolution*3;
	for(n=0;n<length;++n){
	    bline[n]=(unsigned char)(cdata[n]*255);
	}

	flip2(bline,(int)scene->xResolution,(int)scene->yResolution);
	
	path[0]=0;
	if(Files->outputDirectory[0]){
		sprintf(name,"%s.%04ld.tru%c",Files->outputName,myIcon->trueCount,0);
		mstrncpy(path,Files->outputDirectory,2048);
		strncatToPath(path,name,2048);
	}else if(Files->outputName[0]){
		sprintf(path,"%s.%04ld.tru%c",Files->outputName,myIcon->trueCount,0);
	}else{
		sprintf(path,"%s.%04ld.tru%c",scene->savename,myIcon->trueCount,0);
	}

	name1=FilesImageName(Files,myIcon->CurrentFrame);

	if(name1){
		sprintf(name,"%s.%04ld",name1,myIcon->trueCount);
	}
	
	trueOut(path,name,scene->xResolution,scene->yResolution,bline,0L,1);
	
	end=rtime();

	if(scene->PrintTime){
        sprintf(WarningBuff,"%s(%ldx%ld) -  %.2f Seconds\n",path,scene->xResolution,scene->yResolution,end-myIcon->scene.start);
		WarningBatch(WarningBuff);
    }

	if(cdata)cFree((char *)cdata);
	
	if(myIcon){
		if(myIcon->isWriting){
		    *myIcon->isWriting=FALSE;
		}
	    cFree((char *)myIcon);
	}
	
	

	return 0;
}
int FileTrueWrite(char *Header,unsigned char *buffer,
                  long xsize,long ysize,long UnCompressed,
	          long Compressed,unsigned int Esc1,
	          unsigned int Esc2,FILE *out)
{
	int ret;

	ret = 1;

    if(putString((unsigned char *)Header,4L,out))goto ErrorOut;

    if(putInt((int)xsize,out))goto ErrorOut;
    if(putInt((int)ysize,out))goto ErrorOut;

    if(putInt((int)Esc1,out))goto ErrorOut;
    if(putInt((int)Esc2,out))goto ErrorOut;

    if(putLong(UnCompressed,out))goto ErrorOut;
    if(putLong(Compressed,out))goto ErrorOut;

    if(putString((unsigned char *)buffer,Compressed,out))goto ErrorOut;

	ret = 0;
ErrorOut:
	return ret;
}
int saveSDS3D(FilePtr Files,char *filename)
{
	vIconPtr myIcon;
	char limits[2048];
	char path[2048];
	char name[256];
	int rank,size[3];
	double dmin,dmax;
	long length,n;
	int ret;
	
	
	if(!Files || !filename)return 1;
	myIcon=&Files->vmyIcon;

	rank=3;
	size[0]=(int)myIcon->izmax;
	size[1]=(int)myIcon->iymax;
	size[2]=(int)myIcon->ixmax;
	length=size[0]*size[1]*size[2];
		
	path[0]=0;
	if(Files->outputDirectory[0]){
		sprintf(name,"%s.%04ld.s3d%c",filename,myIcon->sds3dCount++,0);
		mstrncpy(path,Files->outputDirectory,2048);
		strncatToPath(path,name,2048);
	}else{
		sprintf(path,"%s.%04ld.s3d%c",filename,myIcon->sds3dCount++,0);
	}
	
	if(length <= 0 || !myIcon->data){
		sprintf(WarningBuff,"saveSDS3D error File: %s length %ld data %lx\n",
		     path,length,(unsigned long)myIcon->data);
		WarningBatch(WarningBuff);
	    return 1;
	}
	
	dmin =  myIcon->data[0];
	dmax =  myIcon->data[0];
				
	for(n=0;n<length;++n){
	    double value;
	    value=myIcon->data[n];
	    if(value < dmin){
	        dmin=value;
	    }else if(value > dmax){
	        dmax=value;
	    }
	}
	
	
	sprintf(limits,"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g vmin %g vmax %g %c",
	       myIcon->xmin,myIcon->xmax,myIcon->ymin,myIcon->ymax,
	       myIcon->zmin,myIcon->zmax,dmin,dmax,0);
	
	ret=sdsOut(path,filename,limits,rank,size,(double *)myIcon->data,0L);
	
	SetFileType(path,"SDS3");

	return ret;
}
int doRange(vIconPtr myIcon)
{
	struct vScene *scene;
	double dmin,dmax;
	double *data,v;
	long length,n;

	if(!myIcon)return 1;
	data=myIcon->data;
	if(!data)return 0;
	scene=&myIcon->scene;

	length=myIcon->ixmax*myIcon->iymax*myIcon->izmax;
	
	dmin = *data;
	dmax = *data;

	for(n=0;n<length;++n){
	    v = *data++;
	    if(v < dmin){
	        dmin = v;
	    }else if(v > dmax){
	        dmax = v;
	    }
	}

	if(scene->debug > 1){
		sprintf(WarningBuff,"dmin %g dmax %g\n",dmin,dmax);
		WarningBatch(WarningBuff);
	}

	if(myIcon->dmin >= myIcon->dmax){
	    myIcon->dminSet=0;
	    myIcon->dmaxSet=0;
	}

	scene->vmin=dmin;
	scene->vmax=dmax;

	if(!myIcon->dminSet)myIcon->dmin=dmin;
	if(!myIcon->dmaxSet)myIcon->dmax=dmax;


	return 0;
}
int sageRange(FilePtr Files)
{
	struct PIO *sage;
	vIconPtr myIcon;
	SagePtr pio;
	struct vScene *scene;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dmin,dmax;
	double d,size;
	tFlt *data;
	long count;
	long n;
	int ret;
	int k;
	double *mat_opacity;
	double mat_scale;
	int nomat;
	long samples;

	ret=1;

	if(!Files)goto ErrorOut;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	pio=&myIcon->pio;

	ret=2;

	if(openPIO(Files,myIcon->CurrentFrame))return 1;

	sage=pio->sage;
	if(!sage)goto ErrorOut;

	if(TraceSetUp(myIcon))goto ErrorOut;

	ret=3;

	dmin = myIcon->dmin;
	dmax = myIcon->dmax;
	data = pio->value;

	if(!data)goto ErrorOut;

	ret=4;

    samples=scene->samples;

	mat_opacity=scene->mat_opacity;
	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	xmin=ymin=zmin=     1e60;
	xmax=ymax=zmax=    -1e60;

	count=0;
	for(n=0;n<pio->numcell;++n){
		double mopacity;
	    if(pio->daughter[n])continue;
	    d=data[n];
	    if(d < dmin || d > dmax)continue;

		if(scene){
			mopacity=interpolate(d,scene->tcount,scene->tvalue,scene->topacity);
		}else{
			nomat=(int)((d-dmin)*mat_scale);
			if(nomat < 0){
				nomat=0;
			}else if(nomat >= samples){
				nomat = (int)(samples-1);
			}
			mopacity=mat_opacity[nomat];

		}
		
	    if(mopacity <= 0.0){
	        continue;
	    }

	    k=(int)(pio->level[n]);
	    size=pio->xcenter[n]+pio->dx2[k];
	    if(size < xmin)xmin=size;
	    if(size > xmax)xmax=size;
	    size=pio->xcenter[n]-pio->dx2[k];
	    if(size < xmin)xmin=size;
	    if(size > xmax)xmax=size;

	    size=pio->ycenter[n]+pio->dy2[k];
	    if(size < ymin)ymin=size;
	    if(size > ymax)ymax=size;
	    size=pio->ycenter[n]-pio->dy2[k];
	    if(size < ymin)ymin=size;
	    if(size > ymax)ymax=size;


	    size=pio->zcenter[n]+pio->dz2[k];
	    if(size < zmin)zmin=size;
	    if(size > zmax)zmax=size;
	    size=pio->zcenter[n]-pio->dz2[k];
	    if(size < zmin)zmin=size;
	    if(size > zmax)zmax=size;
	    ++count;

	}

	if(scene->debug > 1){
		sprintf(WarningBuff,"Sage Range Total Cells %ld Visible %ld\n",pio->numcell,count);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"xmin %g xmax %g\n",xmin,xmax);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"ymin %g ymax %g\n",ymin,ymax);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"zmin %g zmax %g\n",zmin,zmax);
		WarningBatch(WarningBuff);
	}

	myIcon->xmin=xmin;
	myIcon->xmax=xmax;
	myIcon->ymin=ymin;
	myIcon->ymax=ymax;
	myIcon->zmin=zmin;
	myIcon->zmax=zmax;

	ret=0;

ErrorOut:
	if(ret){
	    sprintf(WarningBuff,"sageRange Error ret %d\n",ret);
		WarningBatch(WarningBuff);
	}
	return ret;
}
int sageSDS3D(vIconPtr myIcon)
{
	struct vScene *scene;
	int ret;


	if(!myIcon)return 1;
	scene=&myIcon->scene;

	ret=1;

	if(scene->debug > 1){
		sprintf(WarningBuff,"sageSDS3D ixmax %ld iymax %ld izmax %ld\n",myIcon->ixmax,myIcon->iymax,myIcon->izmax);
		WarningBatch(WarningBuff);
	}

	if(TraceSetUp(myIcon))goto ErrorOut;
	
	if(scene->debug > 1){
		sprintf(WarningBuff,"sageSDS3D ixmax %ld iymax %ld izmax %ld\n",myIcon->ixmax,myIcon->iymax,myIcon->izmax);
		WarningBatch(WarningBuff);
	}


	ret=0;

ErrorOut:
	return ret;
}
int sageSample(vIconPtr myIcon)
{
	struct vScene *scene;
	struct PIO *sage;
	SagePtr pio;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double d,size;
	tFlt *data;
	double dmin,dmax;
	long count;
	long n;
	int ret;
	int kk;
	long length;
	long ixmax;
	long iymax;
	long izmax;
	double *sdata;
	long iyxmax;
	double didx;
	double djdy;
	double dkdz;
	double dxdi;
	double dydj;
	double dzdk;
	double dv;
	long ne;
	long nn;


	if(!myIcon)return 1;
	pio=&myIcon->pio;
	scene=&myIcon->scene;
	sage=pio->sage;
	if(!sage)return 1;

	sdata=NULL;

	ret=1;

	ixmax=myIcon->ixmax;
	iymax=myIcon->iymax;
	izmax=myIcon->izmax;

	sprintf(WarningBuff,"sageSample ixmax %ld iymax %ld izmax %ld\n",ixmax,iymax,izmax);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	length=ixmax*iymax*izmax;
	sdata=(double *)cMalloc(length*sizeof(double),99875);
	if(!sdata)goto ErrorOut;
	zerol((char *)sdata,length*sizeof(double));

	if(TraceSetUp(myIcon))goto ErrorOut;


	data = pio->value;

	xmin=myIcon->xmin;
	xmax=myIcon->xmax;
	ymin=myIcon->ymin;
	ymax=myIcon->ymax;
	zmin=myIcon->zmin;
	zmax=myIcon->zmax;

	iyxmax=ixmax*iymax;
	didx=(double)(ixmax)/(xmax-xmin);
	djdy=(double)(iymax)/(ymax-ymin);
	dkdz=(double)(izmax)/(zmax-zmin);

	dxdi=1.0/didx;
	dydj=1.0/djdy;
	dzdk=1.0/dkdz;

	dmin =  1e33;
	dmax = -1e33;
	ne=0;
	count=0;
	for(n=0;n<pio->numcell;++n){
	    double xs,ys,zs,xe,ye,ze;
	    int x1,y1,z1,x2,y2,z2;
	    int i,j,k;

	    if(pio->daughter[n])continue;

	    d=data[n];

	    kk=(int)(pio->level[n]);

	    size=pio->xcenter[n];
	    dv=pio->dx2[kk];
	    xs=size-1.00000001*dv;
	    xe=size+1.00000001*dv;
	    if(xe < xmin || xs > xmax)continue;


	    size=pio->ycenter[n];
	    dv=pio->dy2[kk];
	    ys=size-1.00000001*dv;
	    ye=size+1.00000001*dv;
	    if(ye < ymin || ys > ymax)continue;


	    size=pio->zcenter[n];
	    dv=pio->dz2[kk];
	    zs=size-1.00000001*dv;
	    ze=size+1.00000001*dv;
	    if(ze < zmin || zs > zmax)continue;

	    x1=(int)((xs-xmin)*didx);
	    if(x1 < 0)x1 = 0;
	    x2=(int)((xe-xmin)*didx+1.0);
	    if(x2 >= ixmax)x2=(int)ixmax - 1;

	    y1=(int)((ys-ymin)*djdy);
	    if(y1 < 0)y1 = 0;
	    y2=(int)((ye-ymin)*djdy+1.0);
	    if(y2 >= iymax)y2=(int)iymax - 1;


	    z1=(int)((zs-zmin)*dkdz);
	    if(z1 < 0)z1 = 0;
	    z2=(int)((ze-zmin)*dkdz+1.0);
	    if(z2 >= izmax)z2=(int)izmax - 1;

	    for(k=z1;k<z2+1;++k){
	        double x,y,z;
	        z=zmin+((double)k+.5)*dzdk;
	        if(z < zs || z > ze)continue;
	        for(j=y1;j<y2+1;++j){
	            y=ymin+((double)j+.5)*dydj;
	            if(y < ys || y > ye)continue;
	            for(i=x1;i<x2+1;++i){
	                nn=i+j*ixmax+k*iyxmax;

	                x=xmin+((double)i+.5)*dxdi;
	                if(x < xs || x > xe)continue;

					sdata[nn] =   d;
					if(d < dmin)dmin = d;
					if(d > dmax)dmax = d;
					++ne;
	            }
	        }
	    }

	    ++count;
	}

	if(scene->xReflect){
	    ne *= 2;
	}

	if(scene->yReflect){
	    ne *= 2;
	}

	if(scene->zReflect){
	    ne *= 2;
	}

	if(scene->debug > 1){
		sprintf(WarningBuff,"sageSample ixmax %ld iymax %ld izmax %ld\n",myIcon->ixmax,myIcon->iymax,myIcon->izmax);
		WarningBatch(WarningBuff);

		sprintf(WarningBuff,"Total %ld Sampled %ld Generated %ld dmin %g dmax %g\n",pio->numcell,count,ne,dmin,dmax);
		WarningBatch(WarningBuff);
	}

	if(myIcon->data)cFree((char *)myIcon->data);
	myIcon->data=sdata;
	sdata = NULL;

	ret=0;

ErrorOut:
	if(sdata)cFree((char *)sdata);
	return ret;
}
int sagePrintFull(vIconPtr myIcon,int flag)
{
	struct PIO *sage;
	long *location;
	char *hasData;
	SagePtr pio;
	long n,baseWith;
	int i,j,jj,k;

	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	location=pio->location;
	if(!location)return 1;

	hasData=pio->hasData;
	    
	baseWith=0;
	if(flag){
	    if(!hasData)return 1;
	}

	sprintf(WarningBuff,"\n       SAGE BASE GRID FULL\n\n");;
	WarningBatch(WarningBuff);
	for(k=0;k<sage->izmax;++k){
	    sprintf(WarningBuff,"k = %d\n\n",k);
		WarningBatch(WarningBuff);
	    for(jj=0;jj<sage->iymax;++jj){
	        j=(int)(sage->iymax-1-jj);
			sprintf(WarningBuff,"       ");
			WarningBatch(WarningBuff);
	        for(i=0;i<sage->ixmax;++i){
	            n=i+j*sage->ixmax+k*sage->ixmax*sage->iymax;
			    n=location[n];
			    if(flag){
			        if(n < 0 || n >= pio->numcell){
			            sprintf(WarningBuff,"E");
						WarningBatch(WarningBuff);
			        }else if(hasData && hasData[n]){
			            ++baseWith;
			            sprintf(WarningBuff,"X");
						WarningBatch(WarningBuff);
			        }else{
			            sprintf(WarningBuff,"0");
						WarningBatch(WarningBuff);
			        }
			    }else{
			        sprintf(WarningBuff,"%ld ",n);
					WarningBatch(WarningBuff);
			    }
	        }
			sprintf(WarningBuff,"\n");
			WarningBatch(WarningBuff);
	    }
	    sprintf(WarningBuff,"\n");;
		WarningBatch(WarningBuff);
	}

        if(flag){
            sprintf(WarningBuff,"Base Cells With Data : %ld\n\n",baseWith);
			WarningBatch(WarningBuff);
        }


	return 0;
}
int sageLocation(vIconPtr myIcon)
{
	struct vScene *scene;
	struct PIO *sage;
	long *location;
	SagePtr pio;
	long count;
	long n;
	long loc,length;
	long ixmax,ixymax,ierror;
	long i,j,k;
	int kk;

	double xmin,ymin,zmin;
	double dxset,dyset,dzset;
	double odxset,odyset,odzset;
	double xc,yc,zc;

	if(!myIcon)return 1;
	scene=&myIcon->scene;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;

	sprintf(WarningBuff,"ixmax %ld iymax %ld izmax %ld\n",sage->ixmax,sage->iymax,sage->izmax);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	length=sage->ixmax*sage->iymax*sage->izmax;

	if(pio->location)cFree((char *)pio->location);
	location=(long *)cMalloc(sizeof(long)*length,19723);
	pio->location=location;
	if(!location)return 1;

	for(n=0;n<length;++n)location[n] = -1;

	ixmax=sage->ixmax;
	ixymax=ixmax*sage->iymax;


	xmin=pio->xmin;
	ymin=pio->ymin;
	zmin=pio->zmin;

	dxset=sage->dxset;
	odxset=1.0/dxset;
	dyset=sage->dyset;
	odyset=1.0/dyset;
	dzset=sage->dzset;
	odzset=1.0/dzset;

	ierror=0;
	count=0;
	for(n=0;n<pio->numcell;++n){
	    kk=(int)(pio->level[n]);
	    if(kk != 1)continue;
	    ++count;
	    xc=pio->xcenter[n];
	    i=(long)((xc-xmin)*odxset);

	    yc=pio->ycenter[n];
	    j=(long)((yc-ymin)*odyset);

	    zc=pio->zcenter[n];
	    k=(long)((zc-zmin)*odzset);

	    loc=i+j*ixmax+k*ixymax;
	    if(loc < 0 || loc >= length){
	          if(++ierror < 10){
	             sprintf(WarningBuff,"Cell Location Error loc %ld i %ld j %ld k %ld\n",loc,i,j,k);
			 	 if(scene->debug > 1)WarningBatch(WarningBuff);
	          }
	          continue;
	    }
	    location[loc]=n;
	}

	for(n=0;n<length;++n){
	    if((location[n] < 0) ||  (location[n] >= pio->numcell))++ierror;
	}

	if(ierror || (count != length)){
	    sprintf(WarningBuff,"Location Errors ierror %ld count %ld\n",ierror,count);
		if(scene->debug > 1)WarningBatch(WarningBuff);
		/* accept errors and contiune */
	}
	return 0;
}
int sageSweep(FilePtr Files,long steps)
{
	struct vScene *scene;
	vIconPtr myIcon;
	double start,end;
	double min,max,dx;
	long n,k;


	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	if(steps <= 0)return 1;

	start=rtime();


	if(sageSetFrame(Files,myIcon->CurrentFrame))goto ErrorOut;


	dx=(double)scene->samples/(double)steps;
	min=0;
    for(k=0;k<steps;++k){
	    max=min+dx;
	    if(TraceSetUp(myIcon))goto ErrorOut;
	    for(n=0;n<scene->samples;++n){
	        if((double)n < min || (double)n > max)scene->mat_opacity[n]=0;
	    }
	    min += dx;

	    if(sageDoTrace(Files))goto ErrorOut;
	}

    if(TraceSetUp(myIcon))goto ErrorOut;

	min=0;
    for(k=0;k<steps;++k){
        double d1,d2;
        long k1,k2;

        k1=(long)min;
        if(k1 < 0){
            k1 = 0;
        }else if(k1 >= scene->samples){
            k1 = scene->samples-1;
        }
    	d1=myIcon->dmin+(double)k1*(myIcon->dmax-myIcon->dmin)/(double)(scene->samples-1);

    	max=min+dx;
    	k2=(long)max;
        if(k2 < 0){
            k2 = 0;
        }else if(k2 >= scene->samples){
            k2 = scene->samples-1;
        }
    	d2=myIcon->dmin+(double)k2*(myIcon->dmax-myIcon->dmin)/(double)(scene->samples-1);

   	 	sprintf(WarningBuff,"k %ld min %g max %g\n",k,d1,d2);
		WarningBatch(WarningBuff);

    	min += dx;
	}


	end=rtime();
	sprintf(WarningBuff,"%.2f Seconds For %ld Values\n",end-start,steps);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	return 0;
ErrorOut:
	return 1;
}
int sageSweepDown(FilePtr Files,long steps)
{
	struct vScene *scene;
	vIconPtr myIcon;
	double start,end;
	double min,max,dx;
	long n,k;


	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	if(steps <= 0)return 1;

	start=rtime();


	if(sageSetFrame(Files,myIcon->CurrentFrame))goto ErrorOut;

	dx=(double)scene->samples/(double)steps;
	max=(double)scene->samples;
    for(k=0;k<steps;++k){
	    if(TraceSetUp(myIcon))goto ErrorOut;
	    for(n=0;n<scene->samples;++n){
	        if((double)n > max)scene->mat_opacity[n]=0;
	    }
	    max -= dx;
	    if(sageDoTrace(Files))goto ErrorOut;
	}

    if(TraceSetUp(myIcon))goto ErrorOut;

	min=0;
	max=(double)scene->samples;
    for(k=0;k<steps;++k){
        double d1,d2;
        long k1,k2;

        k1=(long)min;
        if(k1 < 0){
            k1 = 0;
        }else if(k1 >= scene->samples){
            k1 = scene->samples-1;
        }
        d1=myIcon->dmin+(double)k1*(myIcon->dmax-myIcon->dmin)/(double)(scene->samples-1);

        k2=(long)max;
        if(k2 < 0){
            k2 = 0;
        }else if(k2 >= scene->samples){
            k2 = scene->samples-1;
        }
        d2=myIcon->dmin+(double)k2*(myIcon->dmax-myIcon->dmin)/(double)(scene->samples-1);

        sprintf(WarningBuff,"k %ld min %g max %g\n",k,d1,d2);
	    if(scene->debug > 1)WarningBatch(WarningBuff);

        max -= dx;
	}


	end=rtime();
	sprintf(WarningBuff,"%.2f Seconds For %ld Values\n",end-start,steps);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	return 0;
ErrorOut:
	return 1;
}
static int doOrbit(FilePtr Files,OrbitPtr Orbitin)
{
	struct vScene *scene;
	vIconPtr myIcon;
	struct P EyeSave,LampSave;
	struct P EyeToTarget,EyeToTargetN,TargetLoop,V1,V2;
	struct OrbitStruct Orbit;
	double aEllispe,bEllispe,hEllispe;
	double dsin,dsout,epsin,epsout;
	double start,end;
	double DegreesSave;
	double r1,r2,a1,a2;
	double angle;
	double pi;
	long n,o,l;
	long sequence;
	int ret;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	if(!Orbitin)return 1;
	Orbit = *Orbitin;
	
	ret = 1;

	EyeSave=scene->Eye;

	LampSave=scene->Lamp;

	DegreesSave=scene->Degrees;
	
	start=rtime();

	if(sageSetFrame(Files,myIcon->CurrentFrame))goto ErrorOut;


	if(Orbit.targetradius <= 0.){
	    struct P diff;
	    diff=Sub(&scene->Target,&scene->Eye);
	    Orbit.targetradius=.3*Len(&diff);
	}

	if(Orbit.targetdegrees <= 0.){
	   Orbit.targetdegrees=scene->Degrees*3.0;
	   if(Orbit.targetdegrees > 170.)Orbit.targetdegrees=170.;
	}

	pi=4.0*atan(1.0);

	TargetLoop=Add(&Orbit.offset,&scene->Target);

	EyeToTarget=Sub(&TargetLoop,&scene->Eye);
	EyeToTargetN=Norm(&EyeToTarget);

	V1=CrossN(&EyeToTargetN,&scene->Up);

	V2=CrossN(&scene->Up,&V1);
	
	if(scene->debug > 1){
	    sprintf(WarningBuff,"Orbit V1 %f %f %f\n",V1.x,V1.y,V1.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"Orbit V2 %f %f %f\n",V2.x,V2.y,V2.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"Orbit Up %f %f %f\n",scene->Up.x,scene->Up.y,scene->Up.z);
		WarningBatch(WarningBuff);
	}
	

	bEllispe=Orbit.targetradius;

	aEllispe=Dot(&V2,&EyeToTarget);

	hEllispe=Dot(&scene->Up,&EyeToTarget);


	{
	    struct P Range;
	    Range=Sub(&scene->Target,&scene->Eye);
	    r1=Len(&Range);  
	    a1=r1*tan(.5*DegreesSave*pi/180.);
	    Range=Mult(&V1,Orbit.targetradius);
	    Range=Add(&Range,&TargetLoop);
	    Range=Sub(&scene->Target,&Range);
	    r2=Len(&Range);
	    a2=r2*tan(.5*Orbit.targetdegrees*pi/180.);
	}



	{
		double length;

		if(Orbit.around > 0 && Orbit.in > 3){
		    dsin=(Orbit.targetradius*pi/((double)Orbit.around))/(r1-r2);
		    length=dsin*(double)Orbit.in;
		    if(length > 1.0){
			dsin=1.0/((double)Orbit.in);
		        epsin=0.;
		    }else{
		        epsin=(1.0 - (double)length)/(double)SUM(Orbit.in-1);
		    }
		}else{
		    dsin=0.0;
		    if(Orbit.in > 0)dsin=1.0/((double)Orbit.in);
		    epsin=0.;
		}
	}
	{
		double length;

		if(Orbit.around > 0 && Orbit.out > 3){
		    dsout=(Orbit.targetradius*pi/((double)Orbit.around))/(r1-r2);
		    length=dsout*(double)Orbit.out;
		    if(length > 1.0){
			dsout=1.0/((double)Orbit.out);
			epsout=0.;
		    }else {
		        epsout=(1.0 - (double)length)/(double)SUM(Orbit.out-1);
		    }
		}else{
		    dsout=0.0;
		    if(Orbit.out > 0)dsout=1.0/((double)Orbit.out);
		    epsout=0.;
		}
	}

	if(Orbit.orbits < 0)Orbit.orbits = 1;

	sequence=0;

	for(o=0;o<Orbit.orbits;++o){
	    struct P Move,Move2;
	    double dm,s,rp,sp;

	    dm=0.0;
	    if(Orbit.in)dm = 1.0/((double)(Orbit.in));
	    for(n=0;n<Orbit.in;++n){
	        s=((double)(Orbit.in-n)*dsin);
	        if(epsin > 0.0){
	            if((Orbit.in-n-1) >= 1)s += epsin*(double)SUM((Orbit.in-n-1));
	        }
	        s = 1.0 - s;
	 
	 
			Move=EyeSave;
			Move2=Mult(&scene->Up,s*hEllispe);
			Move=Add(&Move,&Move2);


			Move2=Mult(&V2,aEllispe+aEllispe*cos((180.-s*90.)*pi/180.));
			Move=Add(&Move,&Move2);

			Move2=Mult(&V1,bEllispe*sin((180.-s*90.)*pi/180.));
			Move=Add(&Move,&Move2);

			scene->Eye=Move;
			scene->Lamp=scene->Eye;

			rp=(1.-s)*r1+s*r2;

			Move=Sub(&scene->Eye,&scene->Target);
			rp=Len(&Move);

			s=(rp-r1)/(r2-r1);

			sp=(1.-s)*a1+s*a2;
			scene->Degrees=2.0*atan2(sp,rp)*180./pi;

			if(scene->PrintSpheres){
			    sprintf(WarningBuff,"sphere %g  %g  %g %g\n",scene->Eye.x,scene->Eye.y,scene->Eye.z,scene->sphereSize);
				WarningBatch(WarningBuff);
			}

            if(TraceSetUp(myIcon))goto ErrorOut;
			if(Orbit.stepframes){
				if(sageSetFrame(Files,sequence))goto ErrorOut;
			}
	        if(sageDoTrace(Files))goto ErrorOut;
	        ++sequence;	    
	    }
	    
	    scene->Degrees=Orbit.targetdegrees;
	    dm=0.0;
	    if(Orbit.around > 1)dm = 1.0/((double)(Orbit.around));
	    angle = -dm;
	    for(l=0;l<Orbit.targetloops;++l){
			for(n=0;n<Orbit.around;++n){
				angle += dm;
				Move= Mult(&V1,Orbit.targetradius*cos(angle*pi));
				Move2=Mult(&V2,Orbit.targetradius*sin(angle*pi));
				Move=Add(&Move,&Move2);
				scene->Eye=Add(&TargetLoop,&Move);
				scene->Lamp=scene->Eye;
				if(scene->PrintSpheres){
					sprintf(WarningBuff,"sphere %g  %g  %g %g\n",scene->Eye.x,scene->Eye.y,scene->Eye.z,scene->sphereSize);
					WarningBatch(WarningBuff);
				}

				if(TraceSetUp(myIcon))goto ErrorOut;
				if(Orbit.stepframes){
					if(sageSetFrame(Files,sequence))goto ErrorOut;
				}
				if(sageDoTrace(Files))goto ErrorOut;
				++sequence;	  
	        }
	    }
	    dm=0.0;
	    if(Orbit.out)dm = 1.0/((double)(Orbit.out));
	    for(n=0;n<Orbit.out;++n){
			s=(double)n*dsout;
			if(epsout > 0.0){
				if((n-1) >= 1)s += epsout*(double)SUM((n-1));
			}
			s = 1.0 - s;


			Move=EyeSave;
			Move2=Mult(&scene->Up,s*hEllispe);
			Move=Add(&Move,&Move2);

			Move2=Mult(&V2,aEllispe+aEllispe*cos((180.-s*90.)*pi/180.));
			Move=Add(&Move,&Move2);

			Move2=Mult(&V1,-bEllispe*sin((180.-s*90.)*pi/180.));
			Move=Add(&Move,&Move2);

			scene->Eye=Move;
			scene->Lamp=scene->Eye;


			rp=(1.-s)*r1+s*r2;

			Move=Sub(&scene->Eye,&scene->Target);
			rp=Len(&Move);

			s=(rp-r1)/(r2-r1);

			sp=(1.-s)*a1+s*a2;
			scene->Degrees=2.0*atan2(sp,rp)*180./pi;


			if(scene->PrintSpheres){
				sprintf(WarningBuff,"sphere %g  %g  %g %g\n",scene->Eye.x,scene->Eye.y,scene->Eye.z,scene->sphereSize);
				WarningBatch(WarningBuff);
			}

			if(TraceSetUp(myIcon))goto ErrorOut;
			if(Orbit.stepframes){
				if(sageSetFrame(Files,sequence))goto ErrorOut;
			}
			if(sageDoTrace(Files))goto ErrorOut;
			++sequence;	    
	    }
	}

	if(scene->PrintSpheres){
	    sprintf(WarningBuff,"sphere %g  %g  %g %g\n",scene->Target.x,scene->Target.y,scene->Target.z,2.0*scene->sphereSize);
		WarningBatch(WarningBuff);
	}

	ret=0;

	if(scene->debug > 1){
	    end=rtime();
	    sprintf(WarningBuff,"%.2f Seconds For %ld Steps\n",end-start,sequence);
	    WarningBatch(WarningBuff);
	}

ErrorOut:


	scene->Degrees=DegreesSave;
	scene->Eye=EyeSave;
	scene->Lamp=LampSave;
	return ret;
}
int sageRotate(FilePtr Files,long tiltsteps,long rotatesteps)
{
	struct vScene *scene;
	vIconPtr myIcon;
	double DegreesToRadians;
	struct P EyeSave,originSave;
	double vstep,vinc,vert,vertDegree;
	double hstep,hinc,hort,hortDegree;
	struct P EyeToLamp,EyeToTarget,Up,Target;
	struct P x,y,z;
	struct P xEye,yEye,zEye;
	double radius,rOut;
	struct P *lamp;
	double start,end;


	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;

	start=rtime();

	if(sageSetFrame(Files,myIcon->CurrentFrame))goto ErrorOut;

  	DegreesToRadians=atan(1.0)/45.0;
	EyeSave=scene->Eye;
	
	lamp = &scene->Lamp;

	originSave = *lamp;
        
	vstep = (int)tiltsteps;
	vinc= 180./(vstep-1.0);
	hstep= (int)rotatesteps;
	hinc=360./hstep;
	radius=sqrt((scene->Eye.x-scene->Target.x)*(scene->Eye.x-scene->Target.x)+
                    (scene->Eye.y-scene->Target.y)*(scene->Eye.y-scene->Target.y)+
                    (scene->Eye.z-scene->Target.z)*(scene->Eye.z-scene->Target.z));

	EyeToLamp.x=lamp->x-scene->Eye.x;
	EyeToLamp.y=lamp->y-scene->Eye.y;
	EyeToLamp.z=lamp->z-scene->Eye.z;
    
	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);
	
	Up=Norm(&scene->Up);
	
	z=Up;
	x=CrossN(&EyeToTarget,&Up);
	y=CrossN(&z,&x);
	
	Target=scene->Target;
	

        for(vert=90.0;vert>= -90.0; vert -= vinc){
            vertDegree = vert;
            if(vertDegree == 90){
                vertDegree -= .0001;
            }else if(vertDegree == -90){
                vertDegree += .0001;
            }
            rOut=radius*cos(vertDegree*DegreesToRadians);
            zEye=Mult(&z,radius*sin(vertDegree*DegreesToRadians));
            for(hort=0;hort<hstep;++hort){
                hortDegree=hort*hinc;

        	xEye=Mult(&x,rOut*cos(hortDegree*DegreesToRadians));
        	yEye=Mult(&y,rOut*sin(hortDegree*DegreesToRadians));
            

                scene->Eye=p(xEye.x+yEye.x+zEye.x+Target.x,
            			 xEye.y+yEye.y+zEye.y+Target.y,
            			 xEye.z+yEye.z+zEye.z+Target.z);
            
                lamp->x=scene->Eye.x+EyeToLamp.x;
                lamp->y=scene->Eye.y+EyeToLamp.y;
                lamp->z=scene->Eye.z+EyeToLamp.z;

		*lamp = scene->Eye;
		
	        if(TraceSetUp(myIcon))goto ErrorOut;
	        if(sageDoTrace(Files))goto ErrorOut;

             /* sprintf(WarningBuff,"Sage Debug Stop\n"); 
	        break;*/

             }
            /* sprintf(WarningBuff,"Sage Debug Stop\n");
             break; */
	}


	scene->Eye=EyeSave;
	*lamp = originSave;

	end=rtime();
	sprintf(WarningBuff,"%.2f Seconds For %ld Angles\n",end-start,tiltsteps*rotatesteps);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	return 0;
ErrorOut:
	return 1;
}
int sageGetFull(FilePtr Files)
{
	vIconPtr myIcon;
	struct PIO *sage;
	struct vScene *scene;
	long samples;
	SagePtr pio;
	long count;
	long n;
	long length;
	double *mat_opacity;
	double mopacity;
	double mat_scale;

	double dmin,dmax;
	double start,end;
	char *hasData;
	double *mother;
	tFlt *data;
	double d;
	long nm;
	long nomat;
	long dataLength;
	int ret;
	double xmin,xmax;
	double ymin,ymax;
	double zmin,zmax;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	scene=&myIcon->scene;
	samples=scene->samples;
	if(!samples)return 1;
	mat_opacity=scene->mat_opacity;
	if(!mat_opacity)return 1;

	start=rtime();

	ret=1;
	
	mother=NULL;

	length=pio->numcell;
	if(pio->hasData)cFree((char *)pio->hasData);
	hasData=(char *)cMalloc(sizeof(char)*length,19733);
	pio->hasData=hasData;
	if(!hasData)return 1;
	zerol((char *)hasData,sizeof(char)*length);

	if(SageGetFileItem(Files,"cell_mother",0,
	                  &mother,&dataLength,pio->CurrentFrame)){
		ret=200;goto ErrorOut;
	}

	dmin = myIcon->dmin;
	dmax = myIcon->dmax;
	data = pio->value;

	xmin=myIcon->xmin;
	xmax=myIcon->xmax;
	ymin=myIcon->ymin;
	ymax=myIcon->ymax;
	zmin=myIcon->zmin;
	zmax=myIcon->zmax;


	mat_scale=(((double)samples-1.0)/(dmax-dmin));

	count=0;
	for(n=0;n<pio->numcell;++n){
	    if(pio->daughter[n])continue;
	    d=data[n];

	    if(d < dmin || d > dmax)continue;
		
		if(scene){
			mopacity=interpolate(d,scene->tcount,scene->tvalue,scene->topacity);
		}else{

			nomat=(long)((d-dmin)*mat_scale);
			if(nomat < 0){
                nomat=0;
			}else if(nomat >= samples){
               nomat = samples-1;
			}

			mopacity=mat_opacity[nomat];
		}
		
	    if(mopacity <= 0.0)continue;

	    if(pio->xcenter[n] < xmin || pio->xcenter[n] > xmax)continue; 
	    if(pio->ycenter[n] < ymin || pio->ycenter[n] > ymax)continue; 
	    if(pio->zcenter[n] < zmin || pio->zcenter[n] > zmax)continue; 


	    ++count;
	    hasData[n]=1;
	    nm=n;
	    while(1){
	         nm=(long)mother[nm]-1;
	         if(nm < 0)break;
	         if(!hasData[nm]){
	             ++count;
	             hasData[nm]=1;
	         }
	    }
	}

	end=rtime();
	
	if(scene->debug > 1){
		sprintf(WarningBuff,"limits xmin %g ymin %g zmin %g\n",
		        myIcon->xmin,myIcon->ymin,myIcon->zmin);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"       xmax %g ymax %g zmax %g\n",
		        myIcon->xmax,myIcon->ymax,myIcon->zmax);
		WarningBatch(WarningBuff);

		sprintf(WarningBuff,"dmin %g dmax %g\n",
		        dmin,dmax);
		WarningBatch(WarningBuff);
		
		sprintf(WarningBuff,"samples %ld mat_scale %g mat_opacity[0] %g mat_opacity[samples-1] %g \n",
		        (long)samples,mat_scale,mat_opacity[0],mat_opacity[samples-1]);
		WarningBatch(WarningBuff);
		
		sprintf(WarningBuff,"sageGetFull %.2f Seconds Total Cells %ld Visible %ld\n",end-start,pio->numcell,count);
		WarningBatch(WarningBuff);

	}

	if(mother)cFree((char *)mother);

	ret=0;

ErrorOut:
	return ret;
}
int sageGradLowX(mThread *Threads)
{
	struct PIO *sage;
	SagePtr pio;
	vIconPtr myIcon;
	double *next;
	float *gradl,*gradh;
	long ncellStart,ncellEnd;
	long n,n2;
	int k,k2;
	int ierror;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	next=pio->gradData.next;
	if(!next)return 1;
	gradl=pio->gradData.gradl;
	if(!gradl)return 1;
	gradh=pio->gradData.gradh;
	if(!gradh)return 1;


	ncellStart=Threads->smin;
	ncellEnd=Threads->smax;


	ierror=0;

	for(n=ncellStart;n<ncellEnd;++n){        /* lo_x */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradl[n] = (float)((pio->value[n]-pio->value[n2])/(pio->dx2[k]+pio->dx2[k2]));
	    }else{
	         /* should be zero for gradient , but for normal this is good */
	        gradl[n] = (float)((pio->value[n])/(2.0*pio->dx2[k]));
	    }
	}

    myIcon->ierror += ierror;
	Threads->done=TRUE;

	return ierror;
}
int sageGradHighX(mThread *Threads)
{
	struct PIO *sage;
	SagePtr pio;
	vIconPtr myIcon;

	double *next;
	float *gradl,*gradh;
	long ncellStart,ncellEnd;
	long n,n2;
	int k,k2;
	int ierror;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	next=pio->gradData.next;
	if(!next)return 1;
	gradl=pio->gradData.gradl;
	if(!gradl)return 1;
	gradh=pio->gradData.gradh;
	if(!gradh)return 1;


	ncellStart=Threads->smin;
	ncellEnd=Threads->smax;

	ierror=0;

	for(n=ncellStart;n<ncellEnd;++n){        /* hi_x */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradh[n] = (float)((pio->value[n2]-pio->value[n])/(pio->dx2[k]+pio->dx2[k2]));
	    }else{
	         /* should be zero for gradient , but for normal this is good */
	       	gradh[n] = (float)((-pio->value[n])/(2.0*pio->dx2[k]));
	    }

	}

	for(n=ncellStart;n<ncellEnd;++n){
		if(!pio->daughter[n]){
			if(fabs(gradh[n]) > fabs(gradl[n])){
				pio->gradx[n] = gradh[n];
			}else{
				pio->gradx[n] = gradl[n];
			}
		}else{
			pio->gradx[n] = 0;
		}
	}

    myIcon->ierror += ierror;
	Threads->done=TRUE;

	return ierror;
}
int sageGradLowY(mThread *Threads)
{
	struct PIO *sage;
	SagePtr pio;
	vIconPtr myIcon;

	double *next;
	float *gradl,*gradh;
	long ncellStart,ncellEnd;
	long n,n2;
	int k,k2;
	int ierror;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	next=pio->gradData.next;
	if(!next)return 1;
	gradl=pio->gradData.gradl;
	if(!gradl)return 1;
	gradh=pio->gradData.gradh;
	if(!gradh)return 1;


	ncellStart=Threads->smin;
	ncellEnd=Threads->smax;

	ierror=0;

	for(n=ncellStart;n<ncellEnd;++n){        /* lo_y */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradl[n] = (float)((pio->value[n]-pio->value[n2])/(pio->dy2[k]+pio->dy2[k2]));
	    }else{
	         /* should be zero for gradient , but for normal this is good */
	        gradl[n] = (float)((pio->value[n])/(2.0*pio->dy2[k]));
	    }
	}

    myIcon->ierror += ierror;
	Threads->done=TRUE;

	return ierror;
}
int sageGradHighY(mThread *Threads)
{
	struct PIO *sage;
	SagePtr pio;
	vIconPtr myIcon;

	double *next;
	float *gradl,*gradh;
	long ncellStart,ncellEnd;
	long n,n2;
	int k,k2;
	int ierror;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	next=pio->gradData.next;
	if(!next)return 1;
	gradl=pio->gradData.gradl;
	if(!gradl)return 1;
	gradh=pio->gradData.gradh;
	if(!gradh)return 1;


	ncellStart=Threads->smin;
	ncellEnd=Threads->smax;

	ierror=0;

	for(n=ncellStart;n<ncellEnd;++n){        /* hi_y */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradh[n] = (float)((pio->value[n2]-pio->value[n])/(pio->dy2[k]+pio->dy2[k2]));
	    }else{
	         /* should be zero for gradient , but for normal this is good */
	        gradh[n] = (float)((-pio->value[n])/(2.0*pio->dy2[k]));
	    }
	}


	for(n=ncellStart;n<ncellEnd;++n){
		if(!pio->daughter[n]){
			if(fabs(gradh[n]) > fabs(gradl[n])){
				pio->grady[n] = gradh[n];
			}else{
				pio->grady[n] = gradl[n];
			}
		}else{
			pio->grady[n] = 0;
		}
	}

    myIcon->ierror += ierror;
	Threads->done=TRUE;

	return ierror;
}
int sageGradLowZ(mThread *Threads)
{
	struct PIO *sage;
	SagePtr pio;
	vIconPtr myIcon;

	double *next;
	float *gradl,*gradh;
	long ncellStart,ncellEnd;
	long n,n2;
	int k,k2;
	int ierror;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	next=pio->gradData.next;
	if(!next)return 1;
	gradl=pio->gradData.gradl;
	if(!gradl)return 1;
	gradh=pio->gradData.gradh;
	if(!gradh)return 1;


	ncellStart=Threads->smin;
	ncellEnd=Threads->smax;

	ierror=0;

	for(n=ncellStart;n<ncellEnd;++n){        /* lo_z */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradl[n] = (float)((pio->value[n]-pio->value[n2])/(pio->dz2[k]+pio->dz2[k2]));
	    }else{
	         /* should be zero for gradient , but for normal this is good */
	        gradl[n] = (float)((pio->value[n])/(2.0*pio->dz2[k]));
	    }
	}

    myIcon->ierror += ierror;
	Threads->done=TRUE;

	return ierror;
}
int sageGradHighZ(mThread *Threads)
{
	struct PIO *sage;
	SagePtr pio;
	vIconPtr myIcon;

	double *next;
	float *gradl,*gradh;
	long ncellStart,ncellEnd;
	long n,n2;
	int k,k2;
	int ierror;


	if(!Threads)return 1;
	myIcon=(vIconPtr)Threads->data;
	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;
	next=pio->gradData.next;
	if(!next)return 1;
	gradl=pio->gradData.gradl;
	if(!gradl)return 1;
	gradh=pio->gradData.gradh;
	if(!gradh)return 1;


	ncellStart=Threads->smin;
	ncellEnd=Threads->smax;

	ierror=0;

	for(n=ncellStart;n<ncellEnd;++n){        /* hi_z */
	    if(pio->daughter[n])continue;
	    k=(int)(pio->level[n]);
	    n2=(long)(next[n]-1);
	    if(n2 != n){
	        k2=(int)(pio->level[n2]);
	        gradh[n] = (float)((pio->value[n2]-pio->value[n])/(pio->dz2[k]+pio->dz2[k2]));
	    }else{
	         /* should be zero for gradient , but for normal this is good */
	        gradh[n] = (float)((-pio->value[n])/(2.0*pio->dz2[k]));
	    }
	}

	for(n=ncellStart;n<ncellEnd;++n){
		if(!pio->daughter[n]){
			if(fabs(gradh[n]) > fabs(gradl[n])){
				pio->gradz[n] = gradh[n];
			}else{
				pio->gradz[n] = gradl[n];
			}
		}else{
			pio->gradz[n] = 0;
		}
	}

    myIcon->ierror += ierror;
	Threads->done=TRUE;

	return ierror;
}
int sageGradients(FilePtr Files)
{
	struct vScene *scene;
	vIconPtr myIcon;
	struct PIO *sage;
	SagePtr pio;
	double start,end;
	double *next;
	float *gradl,*gradh;
	long dataLength;
	int n;
	int ret;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;

	if(pio->HasGradients && pio->gradx && pio->grady && pio->gradz)return 0;

	ret = 1;
	
	start=rtime();

	gradh=NULL;
	gradl=NULL;

	pio->HasGradients = FALSE;

	if(pio->gradx)cFree((char *)pio->gradx);
	pio->gradx=NULL;

	if(pio->grady)cFree((char *)pio->grady);
	pio->grady=NULL;

	if(pio->gradz)cFree((char *)pio->gradz);
	pio->gradz=NULL;

	pio->gradx=(float *)cMalloc(sizeof(float)*pio->numcell,19727);
	if(!pio->gradx)goto ErrorOut;

	pio->grady=(float *)cMalloc(sizeof(float)*pio->numcell,19728);
	if(!pio->grady)goto ErrorOut;

	pio->gradz=(float *)cMalloc(sizeof(float)*pio->numcell,19729);
	if(!pio->gradz)goto ErrorOut;

	gradl=(float *)cMalloc(sizeof(float)*pio->numcell,19725);
	if(!gradl)goto ErrorOut;

	gradh=(float *)cMalloc(sizeof(float)*pio->numcell,19726);
	if(!gradh)goto ErrorOut;

    myIcon->ierror = 0;

	pio->gradData.gradh=gradh;
	pio->gradData.gradl=gradl;

	for(n=0;n<6;++n){
	   static int (*routines[])(mThread *Threads)=
	   					 {
	   					    sageGradLowX,sageGradHighX,
					        sageGradLowY,sageGradHighY,
					        sageGradLowZ,sageGradHighZ,
					      };

	      if(SageGetFileItem(Files,"cell_index",n+1,
	                  &next,&dataLength,pio->CurrentFrame)){
		  		ret=20+n;goto ErrorOut;
	      }

	      pio->gradData.next=next;

	      runThreads(myIcon->threads,myIcon,pio->numcell,routines[n]);

	      if(next)cFree((char *)next);

	}


	end=rtime();

	sprintf(WarningBuff,"Time To Calculate Sage Gradients %.2f Seconds ierror %ld\n",end-start,myIcon->ierror);
	if(scene->debug > 0)WarningBatch(WarningBuff);

	pio->HasGradients = TRUE;

	ret = 0;
ErrorOut:
	if(ret){
	    sprintf(WarningBuff,"sageGradients Error ret %d\n",ret);
		WarningBatch(WarningBuff);
	}
	if(gradl)cFree((char *)gradl);
	if(gradh)cFree((char *)gradh);
	return ret;
}
int sageDump(vIconPtr myIcon,long ncell)
{
	struct PIO *sage;
	SagePtr pio;

	if(!myIcon)return 1;
	pio=&myIcon->pio;
	sage=pio->sage;
	if(!sage)return 1;

	if(!pio->xcenter || !pio->ycenter || !pio->zcenter)return 1;

	if(ncell < 0 || ncell >= pio->numcell)return 1;

	if(pio->value){
	    sprintf(WarningBuff,"ne %ld %g %g %g %g\n",ncell,pio->xcenter[ncell],pio->ycenter[ncell],
	                                        pio->zcenter[ncell],pio->value[ncell]);
		WarningBatch(WarningBuff);
	}else{
	    sprintf(WarningBuff,"ne %ld %g %g %g\n",ncell,pio->xcenter[ncell],pio->ycenter[ncell],
	                                        pio->zcenter[ncell]);
		WarningBatch(WarningBuff);
	}

	return 0;
}
int openSDS(vIconPtr myIcon)
{
	struct vScene *scene;
	char nameout[4096];
	int lastref;

	if(!myIcon)return 1;
	scene=&myIcon->scene;

	if(myIcon->data){
	    cFree((char *)myIcon->data);
	    myIcon->data=NULL;
	}

	if(getSDSLimits(myIcon->filename,&myIcon->ixmax,
	       &myIcon->iymax,&myIcon->izmax,&myIcon->data))return 1;

	sprintf(WarningBuff,"filename %s ixmax %ld iymax %ld izmax %ld\n",
	        myIcon->filename,myIcon->ixmax,myIcon->iymax,myIcon->izmax);

	if(scene->debug > 1)WarningBatch(WarningBuff);
	myIcon->xmin=0;
	myIcon->ymin=0;
	myIcon->zmin=0;
	myIcon->xmax=(double)myIcon->ixmax;
	myIcon->ymax=(double)myIcon->iymax;
	myIcon->zmax=(double)myIcon->izmax;


	lastref=DFSDlastref();
	if(lastref != -1){
	   zerol((char *)nameout,4096);
	   if(DFANgetdesc(myIcon->filename,306,lastref,nameout,4096) != -1){
		     /* doRenderKeys(nameout,myIcon); */
	   }

	}

	sprintf(WarningBuff,"limits xmin %g ymin %g zmin %g\n",
	            myIcon->xmin,myIcon->ymin,myIcon->zmin);
	if(scene->debug > 1)WarningBatch(WarningBuff);
	sprintf(WarningBuff,"       xmax %g ymax %g zmax %g\n",
	            myIcon->xmax,myIcon->ymax,myIcon->zmax);
	if(scene->debug > 1)WarningBatch(WarningBuff);




	return 0;
}
int getSDSLimits(char *name,long *imax,long *jmax,long *kmax,double **data)
{
	int rank,maxrank;
	unsigned long length;
	double *buffer;
	int sizes[5];

	if(!name || !imax || !jmax || !kmax || !data)return 1;

	*data=NULL;

	maxrank=5;

	DFSDrestart();

	if(DFSDgetdims(name,&rank,sizes,maxrank)){
	    sprintf(WarningBuff,"Could not Read 3D Limits %s\n",name);
		WarningBatch(WarningBuff);
	    return 1;
	}

	if(rank == 3){
        *imax=sizes[2];
        *jmax=sizes[1];
        *kmax=sizes[0];
	    length=sizes[0]*sizes[1]*sizes[2]*sizeof(double);
	    buffer=(double *)cMalloc(length,8486);
	    if(buffer == NULL){
			sprintf(WarningBuff,"Memory Requested %ld failed\n",length);
			WarningBatch(WarningBuff);
			return 1;
	    }
	    if(DFSDgetdata(name,rank,sizes,(char *)buffer)){
	        sprintf(WarningBuff,"DFSDgetdata Error (%d) on file %s\n", DFerrno(),name);
			WarningBatch(WarningBuff);
	        if(buffer)cFree((char *)buffer);
	        return 1;
	    }
	    *data = buffer;
	    return 0;
	}
	return 1;
}
int closeSDS(vIconPtr myIcon)
{
	if(!myIcon)return 1;

	if(myIcon->data)cFree((char *)myIcon->data);
	myIcon->data=NULL;

	return 0;
}
int SetScreen3(struct vScene *scene)
{
	struct P EyeToTarget,V1,V2,V3,pScreen,V4;
	
	EyeToTarget=Sub(&scene->Target,&scene->Eye);
	EyeToTarget=Norm(&EyeToTarget);
	
	    /* Local x Direction */
	V1=CrossN(&EyeToTarget,&scene->Up);
	
	    /* Local y Direction */
	V2=CrossN(&V1,&EyeToTarget);
	
	    /* Local z Direction */
	V3=Mult(&EyeToTarget,-1.0);
	
	pScreen=Mult(&EyeToTarget,scene->Height);
	
	   /* Point At Center of Screen */
	pScreen=Add(&pScreen,&scene->Eye);
	if(scene->debug > 1){
	    sprintf(WarningBuff,"V1 %f %f %f\n",V1.x,V1.y,V1.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"V2 %f %f %f\n",V2.x,V2.y,V2.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"V3 %f %f %f\n",V3.x,V3.y,V3.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"pScreen %g %g %g\n",pScreen.x,pScreen.y,pScreen.z);
		WarningBatch(WarningBuff);
	}
   	scene->V1=V1;
   	scene->V2=V2;
   	scene->V3=V3;
   	scene->pScreen=pScreen;
   	
   	
   	V4=Mult(&V1,0.5*scene->EyeSeparation);
   	scene->EyeRight=Add(&V4,&scene->Eye);
   	
   	V4=Mult(&V1,-0.5*scene->EyeSeparation);
   	scene->EyeLeft=Add(&V4,&scene->Eye);
	if(scene->debug > 1){
	    sprintf(WarningBuff,"EyeRight %g %g %g\n",scene->EyeRight.x,scene->EyeRight.y,scene->EyeRight.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"EyeLeft %g %g %g\n",scene->EyeLeft.x,scene->EyeLeft.y,scene->EyeLeft.z);
		WarningBatch(WarningBuff);
	}   	
   	
	return 0;
}
static int flip2(unsigned char *b,int xsize,int ysize)
{
	register unsigned char *p1,*p2,p;
	int x,y,xinc;

	if(!b)return 1;

	xinc=xsize*3;

	for(y=0;y<ysize/2;++y){
		p1=b+y*xinc;
		p2=b+(ysize-1-y)*xinc;
		for(x=0;x<xinc;++x){
			p = *p1;
			*p1++ = *p2;
			*p2++ = p;
		}
	}
	return 0;
}
int sageData(FilePtr Files,long CurrentFrame,char *name,int index)
{
	struct vScene *scene;
	double start,end;
	vIconPtr myIcon;
	SagePtr pio;
	int ret;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	scene=&myIcon->scene;

	start=rtime();
	
	mstrncpy(pio->pioName,name,255);	
	pio->pioIndex=index;
	
	myIcon->dataname[0]=0;

	sprintf(WarningBuff,"sageData name %s index %d \n",name,index);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	sprintf(myIcon->dataname,"%s%d%c",name,index,0);
	
	ret=openPIO(Files,CurrentFrame);
	if(ret)goto ErrorOut;

	end=rtime();

	sprintf(WarningBuff,"dmin %g dmax %g %.2f Seconds\n",myIcon->dmin,myIcon->dmax,end-start);
	if(scene->debug > 1)WarningBatch(WarningBuff);

	return 0;
ErrorOut:
	sprintf(WarningBuff,"sageData Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	return 1;
}
int openSDS3D(FilePtr Files,long CurrentFrame)
{
	struct vScene *scene;
	struct CommandInfo cpl,*cp;
	struct SetFrameData  *sd;
	struct LIMITS *limits;
	unsigned long length;
	vIconPtr myIcon;
	double value;
	char *command;
	int ret;
	
	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	
	ret = 1;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));
	
	if(myIcon->data){
	    cFree((char *)myIcon->data);
	    myIcon->data=NULL;
	}
	
	if(FilesSetFrame(CurrentFrame,Files)){
	    sprintf(WarningBuff,"FilesSetFrame failed in openSDS3D\n");
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	sd = &Files->SetData;

	myIcon->ixmax=sd->xsize;
	myIcon->iymax=sd->ysize;
	myIcon->izmax=sd->zsize;
	
	length=myIcon->ixmax*myIcon->iymax*myIcon->izmax*sizeof(double);
	
	myIcon->data=(double *)cMalloc(length,8487);
	
	if(!myIcon->data)goto ErrorOut;
	
    memcpy(myIcon->data,sd->data,length);
	
	myIcon->xmin=0;
	myIcon->ymin=0;
	myIcon->zmin=0;
	myIcon->xmax=(double)myIcon->ixmax;
	myIcon->ymax=(double)myIcon->iymax;
	myIcon->zmax=(double)myIcon->izmax;
	
	sprintf(WarningBuff,"filename %s ixmax %ld iymax %ld izmax %ld\n",
	        myIcon->filename,myIcon->ixmax,myIcon->iymax,myIcon->izmax);

	if(scene->debug > 1)WarningBatch(WarningBuff);

	limits=NULL;

    if(!(FileGetLimits(&limits,CurrentFrame,Files))){	   
		if(limits && limits->command){
			if(!getCommand((char *)limits->command,&cpl)){
				cp=&cpl;
				for(cpl.n=0;cpl.n<cpl.nword;++cpl.n){
					command=stringCommand(cp);
					if(!command)continue;					
					if(!mstrcmp("xmin",command)){
					    ++(cp->n);
					    if(doubleCommand(&value,cp))goto ErrorOut;
					    myIcon->xmin=value;
					}else if(!mstrcmp("xmax",command)){
					    ++(cp->n);
					    if(doubleCommand(&value,cp))goto ErrorOut;
					    myIcon->xmax=value;
					}else if(!mstrcmp("ymin",command)){
					    ++(cp->n);
					    if(doubleCommand(&value,cp))goto ErrorOut;
					    myIcon->ymin=value;
					}else if(!mstrcmp("ymax",command)){
					    ++(cp->n);
					    if(doubleCommand(&value,cp))goto ErrorOut;
					    myIcon->ymax=value;
					}else if(!mstrcmp("zmin",command)){
					    ++(cp->n);
					    if(doubleCommand(&value,cp))goto ErrorOut;
					    myIcon->zmin=value;
					}else if(!mstrcmp("zmax",command)){
					    ++(cp->n);
					    if(doubleCommand(&value,cp))goto ErrorOut;
					    myIcon->zmax=value;
					}					
				}			
			}
		}
    }
    
    if(scene->debug > 1){
		sprintf(WarningBuff,"limits xmin %g ymin %g zmin %g\n",
	            myIcon->xmin,myIcon->ymin,myIcon->zmin);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"       xmax %g ymax %g zmax %g\n",
	            myIcon->xmax,myIcon->ymax,myIcon->zmax);
		WarningBatch(WarningBuff);  
	}      
	
	ret = 0;
ErrorOut:
 	getCommand(NULL,&cpl);	
	return ret;
}
int openPIO(FilePtr Files,long CurrentFrame)
{
	struct vScene *scene;
	vIconPtr myIcon;
	struct PIO *sage;
	SagePtr pio;
	long n;
	double size,lmin,lmax;
	double start,end;
	double dmin,dmax;
	double *level,*data;
	long dataLength;
	int k,ret;


	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	scene=&myIcon->scene;
	
	ret = 1;



	start=rtime();

	if(CurrentFrame >= Files->ImageCount)CurrentFrame=Files->ImageCount-1;
	if(CurrentFrame < 0)CurrentFrame=0;
	
	sage = &Files->PIO2DList[CurrentFrame];

	pio->sage=sage;

	if(scene->debug > 1){
		sprintf(WarningBuff,"openPIO numcell %ld nummat %ld numdim %ld mixed_cells %d strength %d\n",
	       sage->numcell,sage->nummat,sage->numdim,sage->mixed_cell,sage->strength);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"openPIO dxset %ld dyset %ld dzset %ld\n",(long)sage->dxset,(long)sage->dyset,(long)sage->dzset);
		WarningBatch(WarningBuff);
		sprintf(WarningBuff,"pioName \"%s\" pioIndex %ld time %g\n",pio->pioName,(long)pio->pioIndex,sage->time);
		WarningBatch(WarningBuff);
	}

	pio->numcell=sage->numcell;
	pio->nummat=sage->nummat;
	pio->numdim=sage->numdim;

	pio->dxset=sage->dxset;
	pio->dyset=sage->dyset;
	pio->dzset=sage->dzset;
	
	
	if((pio->CurrentFrame == CurrentFrame) && pio->xcenter && pio->ycenter && pio->zcenter &&
	      pio->level && pio->daughter){
	      
	    if(pio->value){

			if(!mstrcmp(pio->pioName,pio->pioNameOld) && (pio->pioIndex == pio->pioIndexOld)){
			    goto ExitOk;
			}

	        cFree((char *)pio->value);
	        pio->value=NULL;
	    }
	      
		pio->HasGradients = FALSE;
		
		
		pio->value=(tFlt *)cMalloc(sage->numcell*sizeof(tFlt),7383);
		if(!pio->value)goto ErrorOut;
		if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
		                  &data,&dataLength,pio->CurrentFrame)){
			ret=21;goto ErrorOut;
		}
		if((dataLength != pio->numcell))goto ErrorOut;
		for(n=0;n<pio->numcell;++n){
		    pio->value[n]=(tFlt)data[n];
		}
		if(data)cFree((char *)data);
	      
	    
		dmin = 1e33;
		dmax = -1e33;

		for(n=0;n<pio->numcell;++n){
		    if(pio->daughter[n])continue;
		    size=pio->value[n];
		    if(size < dmin)dmin=size;
		    if(size > dmax)dmax=size;
		}
		
		scene->vmax=dmax;
		scene->vmin=dmin;
		
		if(myIcon->dmin >= myIcon->dmax){
		   myIcon->dminSet=0;
		   myIcon->dmaxSet=0;
		}

		if(!myIcon->dminSet)myIcon->dmin=dmin;
		if(!myIcon->dmaxSet)myIcon->dmax=dmax;

        mstrncpy(pio->pioNameOld,pio->pioName,255);
        pio->pioIndexOld=pio->pioIndex;
   	 	mstrncpy(Files->pioName,pio->pioName,255);
   		Files->pioIndex=pio->pioIndex;
   		
	    return 0;
	}
	
	pio->HasGradients = FALSE;
		
	pio->CurrentFrame=CurrentFrame;

	if(pio->xcenter)cFree((char *)pio->xcenter);
	pio->xcenter=NULL;
	if(pio->ycenter)cFree((char *)pio->ycenter);
	pio->ycenter=NULL;
	if(pio->zcenter)cFree((char *)pio->zcenter);
	pio->zcenter=NULL;
	if(pio->level)cFree((char *)pio->level);
	pio->level=NULL;
	if(pio->daughter)cFree((char *)pio->daughter);
	pio->daughter=NULL;
	if(pio->value)cFree((char *)pio->value);
	pio->value=NULL;
	if(pio->mean)cFree((char *)pio->mean);
	pio->mean=NULL;
		
	
		pio->xcenter=(tFlt *)cMalloc(pio->numcell*sizeof(tFlt),7383);
		if(!pio->xcenter)goto ErrorOut;
		if(SageGetFileItem(Files,"cell_center",1,
		                  &data,&dataLength,pio->CurrentFrame)){
			ret=22;goto ErrorOut;
		}
		if((dataLength != pio->numcell))goto ErrorOut;
		for(n=0;n<pio->numcell;++n){
		    pio->xcenter[n]=(tFlt)data[n];
		}
		if(data)cFree((char *)data);
		
	
		pio->ycenter=(tFlt *)cMalloc(pio->numcell*sizeof(tFlt),7383);
		if(!pio->ycenter)goto ErrorOut;
		if(SageGetFileItem(Files,"cell_center",2,
		                  &data,&dataLength,pio->CurrentFrame)){
			ret=22;goto ErrorOut;
		}
		if((dataLength != pio->numcell))goto ErrorOut;
		for(n=0;n<pio->numcell;++n){
		    pio->ycenter[n]=(tFlt)data[n];
		}
		if(data)cFree((char *)data);
	
	if(sage->numdim > 2){	
			pio->zcenter=(tFlt *)cMalloc(pio->numcell*sizeof(tFlt),7383);
			if(!pio->zcenter)goto ErrorOut;
			if(SageGetFileItem(Files,"cell_center",3,
			                  &data,&dataLength,pio->CurrentFrame)){
				ret=22;goto ErrorOut;
			}
			if((dataLength != pio->numcell))goto ErrorOut;
			for(n=0;n<pio->numcell;++n){
			    pio->zcenter[n]=(tFlt)data[n];
			}
			if(data)cFree((char *)data);
	}

	pio->level=(unsigned char *)cMalloc(pio->numcell,7383);

	if(!pio->level)goto ErrorOut;

	level=NULL;
	if(SageGetFileItem(Files,"cell_level",0,
	                  &level,&dataLength,pio->CurrentFrame)){
		ret=25;goto ErrorOut;
	}

	if((dataLength != pio->numcell))goto ErrorOut;


	lmin=     1e60;
	lmax=    -1e60;
	
	for(n=0;n<pio->numcell;++n){
	    size=level[n];
	    if(size < lmin)lmin=size;
	    if(size > lmax)lmax=size;
	    pio->level[n]=(unsigned char)size;
	}
	
	if(lmin < 1){
	    long count;
	    
	    count=0;
		for(n=0;n<pio->numcell;++n){
		    if(level[n] < 1){
		        level[n]=lmax;
		        count++;
		    }
		}
	    sprintf(WarningBuff,"Error: %ld level zero cells set to level %.0f\n",count,lmax);
	    if(scene->debug > 1)WarningBatch(WarningBuff);
	    lmin=1;
	}	
	
	
	if(lmin < 1 || lmax >= 256){
	    sprintf(WarningBuff,"Error level min %f lenvel max %f\n",lmin,lmax);
		WarningBatch(WarningBuff);
	    ret=27;
	    goto ErrorOut;
	}

	pio->lmax=lmax;

	for(k=0;k<100;++k){
	    pio->dx2[k]=.0;
	    pio->dy2[k]=.0;
	    pio->dz2[k]=.0;
	}

	pio->dx2[0]=pio->dxset;
	pio->dy2[0]=pio->dyset;
	pio->dz2[0]=pio->dzset;

	for(k=1;k<=(int)lmax;++k){
	    pio->dx2[k]=.5*pio->dxset/pow(2,(double)(k-1));
	    pio->dy2[k]=.5*pio->dyset/pow(2,(double)(k-1));
	    pio->dz2[k]=.5*pio->dzset/pow(2,(double)(k-1));
	}


	if(level)cFree((char *)level);
	level=NULL;


	if(SageGetFileItem(Files,"cell_daughter",0,
	                  &pio->daughter,&dataLength,pio->CurrentFrame)){
		ret=26;goto ErrorOut;
	}

	if((dataLength != pio->numcell))goto ErrorOut;

	pio->value=(tFlt *)cMalloc(sage->numcell*sizeof(tFlt),7383);
	if(!pio->value)goto ErrorOut;
	if(SageGetFileItem(Files,pio->pioName,pio->pioIndex,
					  &data,&dataLength,pio->CurrentFrame)){
		ret=21;goto ErrorOut;
	}
	if((dataLength != pio->numcell))goto ErrorOut;
	for(n=0;n<pio->numcell;++n){
		pio->value[n]=(tFlt)data[n];
	}
	if(data)cFree((char *)data);
	
	if(myIcon->TraceType == 6){
		if(SageGetFileItem(Files,"lgt(-1)",0,
						   &pio->mean,&dataLength,pio->CurrentFrame)){
			ret=76;goto ErrorOut;
		}
		
	}
	
	
	

ExitOk:
	dmin = 1e33;
	dmax = -1e33;

	for(n=0;n<pio->numcell;++n){
	    if(pio->daughter[n])continue;
	    size=pio->value[n];
	    if(size < dmin)dmin=size;
	    if(size > dmax)dmax=size;
	}

	scene->vmax=dmax;
	scene->vmin=dmin;
      
    mstrncpy(pio->pioNameOld,pio->pioName,255);
    pio->pioIndexOld=pio->pioIndex;
 	mstrncpy(Files->pioName,pio->pioName,255);
	Files->pioIndex=pio->pioIndex;

	if(myIcon->dmin >= myIcon->dmax){
	   myIcon->dminSet=0;
	   myIcon->dmaxSet=0;
	}

	if(!myIcon->dminSet)myIcon->dmin=dmin;
	if(!myIcon->dmaxSet)myIcon->dmax=dmax;

	pio->xmin=sage->xmin;
	pio->xmax=sage->xmax;

	pio->ymin=sage->ymin;
	pio->ymax=sage->ymax;

	pio->zmin=sage->zmin;
	pio->zmax=sage->zmax;

	sprintf(WarningBuff,"xmin %g xmax %g\n",pio->xmin,pio->xmax);
	if(scene->debug > 1)WarningBatch(WarningBuff);
	sprintf(WarningBuff,"ymin %g ymax %g\n",pio->ymin,pio->ymax);
	if(scene->debug > 1)WarningBatch(WarningBuff);
	
	
	if(myIcon->xmin >= myIcon->xmax){
		myIcon->xmin=pio->xmin;
		myIcon->xmax=pio->xmax;
	}
	
	if(myIcon->ymin >= myIcon->ymax){
		myIcon->ymin=pio->ymin;
		myIcon->ymax=pio->ymax;
	}
		
	if(myIcon->zmin >= myIcon->zmax){
		myIcon->zmin=pio->zmin;
		myIcon->zmax=pio->zmax;
	}
		
	k=(int)pio->lmax;
	if(sage->numdim > 2){
	    sprintf(WarningBuff,"zmin %g zmax %g\n",pio->zmin,pio->zmax);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else{
	    pio->dz2[k]=0;
	}
	
	if(myIcon->CellSize <= 0){
	    myIcon->CellSize=pio->dx2[k]+pio->dy2[k];
	}

	if(scene->debug > 1){
		sprintf(WarningBuff,"lmax %d dx2 %g dy2 %g dz2 %g\n",k,pio->dx2[k],pio->dy2[k],pio->dz2[k]);
		WarningBatch(WarningBuff);
		end=rtime();
		sprintf(WarningBuff,"OpenPIO: %.2f Seconds\n",end-start);
		WarningBatch(WarningBuff);
	}
	return 0;
ErrorOut:
	sprintf(WarningBuff,"openPIO Error ret %d\n",ret);
	WarningBatch(WarningBuff);
	return 1;
}
static int doClose(FilePtr Files)
{
	vIconPtr myIcon;
	struct vScene *scene;
	SagePtr pio;
	
	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	scene=&myIcon->scene;
	
	if(pio->xcenter)cFree((char *)pio->xcenter);
	pio->xcenter=NULL;
	if(pio->ycenter)cFree((char *)pio->ycenter);
	pio->ycenter=NULL;
	if(pio->zcenter)cFree((char *)pio->zcenter);
	pio->zcenter=NULL;
	if(pio->level)cFree((char *)pio->level);
	pio->level=NULL;
	if(pio->daughter)cFree((char *)pio->daughter);
	pio->daughter=NULL;
	if(pio->value)cFree((char *)pio->value);
	pio->value=NULL;
	if(pio->mean)cFree((char *)pio->mean);
	pio->mean=NULL;
	
	
	if(scene->cdata){
	    cFree((char *)scene->cdata);
	    scene->cdata=NULL;
	}

	if(scene->opacity){
	    cFree((char *)scene->opacity);
	    scene->opacity=NULL;
	}
	
	if(scene->opacityToFireball){
	    cFree((char *)scene->opacityToFireball);
	    scene->opacityToFireball=NULL;
	}
	
	if(scene->mat_opacity){
	    cFree((char *)scene->mat_opacity);
	    scene->mat_opacity=NULL;
	}

	if(pio->gradx)cFree((char *)pio->gradx);
	pio->gradx=NULL;

	if(pio->grady)cFree((char *)pio->grady);
	pio->grady=NULL;

	if(pio->gradz)cFree((char *)pio->gradz);
	pio->gradz=NULL;
	
	if(pio->location)cFree((char *)pio->location);
	pio->location=NULL;

	if(pio->hasData)cFree((char *)pio->hasData);
	pio->hasData=NULL;
	
	return 0;
}
int doSphere(FilePtr Files,CommandPtr cp,BatchPtr Batch)
{
	vIconPtr myIcon;
	struct vScene *scene;
	struct CommandInfo cpl;
	char line[4096];
	char *command;
 	double value;
	int ret;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	ret = 1;
	
	zerol((char *)&cpl,sizeof(struct CommandInfo));

	command=stringCommand(cp);
	if(!command)goto ErrorOut;
	
	if(!mstrcmp("trace",command)){
	    doTrace(Files);
	}else if(!mstrcmp("palette",command)){	
		 ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
		mstrncpy(scene->palname,command,255);
	    sprintf(WarningBuff,"palette %s\n",scene->palname);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("sequence",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->trueCount=(long)value;
        sprintf(WarningBuff,"sequence %ld \n",myIcon->trueCount);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("opacity",command)){
	    long n;
	    
	    sprintf(WarningBuff,"opacity\n");
		if(scene->debug > 1)WarningBatch(WarningBuff);
		n=0;
	    while(1){
	    	if(BatchNextLine(Batch,line,sizeof(line)))goto ErrorOut;
			if(getCommand(line,&cpl))goto ErrorOut;
			command=stringCommand(&cpl);
			if(command && !mstrcmp("end",command))break;
		    if(n < 125){
		        cpl.n=0;
	    	    if(doubleCommand(&value,&cpl))goto ErrorOut;
		        scene->tvalue[n]=value;
		        ++(cpl.n);
	    	    if(doubleCommand(&value,&cpl))goto ErrorOut;
		        scene->topacity[n]=value;
		        
		        sprintf(WarningBuff,"n %ld density %g opacity %g\n",n,scene->tvalue[n],scene->topacity[n]);
				if(scene->debug > 1)WarningBatch(WarningBuff);
		        ++n;
		    }
	    }
	    scene->tcount=n;
	}else if(!mstrcmp("dump",command)){
	    long count,n;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    count=(long)value;
	    if(count <= 0 || count >= 126){
	        sprintf(WarningBuff,"dump count %ld exceeds 126 or is zero\n",count);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    sprintf(WarningBuff,"dump count %ld\n",count);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	    for(n=0;n<count;++n){
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        sageDump(myIcon,(long)value);
	    }
	}else if(!mstrcmp("range",command)){
	    doRange(myIcon);
	}else if(!mstrcmp("logscale",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->logscale=(int)value;
	    sprintf(WarningBuff,"logscale  %d \n",myIcon->logscale);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("dmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->dmin=value;
	    myIcon->dminSet=1;
	    sprintf(WarningBuff,"dmin  %g \n",myIcon->dmin);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("dmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->dmax=value;
	    myIcon->dmaxSet=1;
	    sprintf(WarningBuff,"dmax  %g \n",myIcon->dmax);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("xmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmin=value;
	}else if(!mstrcmp("xmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmax=value;
	}else if(!mstrcmp("ymin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymin=value;
	}else if(!mstrcmp("ymax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymax=value;
	}else if(!mstrcmp("zmin",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmin=value;
	}else if(!mstrcmp("zmax",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmax=value;
	}else if(!mstrcmp("Degrees",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Degrees=value;
	    sprintf(WarningBuff,"Degrees  %f \n",scene->Degrees);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("threads",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->threads=(int)value;
	    sprintf(WarningBuff,"threads  %d\n",myIcon->threads);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("xResolution",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->xResolution=(long)value;
	    sprintf(WarningBuff,"xResolution  %ld\n",scene->xResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("xsize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->xResolution=(long)value;
	    sprintf(WarningBuff,"xsize  %ld\n",scene->xResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("yResolution",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->yResolution=(long)value;
	    sprintf(WarningBuff,"yResolution  %ld\n",scene->yResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("ysize",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->yResolution=(long)value;
	    sprintf(WarningBuff,"ysize  %ld\n",scene->yResolution);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("AspectRatio",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->AspectRatio=value;
	    sprintf(WarningBuff,"AspectRatio  %f\n",scene->AspectRatio);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Height",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Height=value;
	    sprintf(WarningBuff,"Height  %g\n",scene->Height);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("debug",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->debug=(int)value;
	    sprintf(WarningBuff,"debug  %d\n",scene->debug);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("use",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    if(!mstrcmp(command,"voxels")){
	        myIcon->TraceType=0;
	    }else if(!mstrcmp(command,"rays")){
	        myIcon->TraceType=1;
	    }else if(!mstrcmp(command,"samples")){
	        myIcon->TraceType=2;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->xSamples=(long)value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->ySamples=(long)value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->zSamples=(long)value;
	        sprintf(WarningBuff,"use  %s xSamples %ld ySamples %ld zSamples %ld\n",command,
	               myIcon->xSamples,myIcon->ySamples,myIcon->zSamples);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        return 0;
	    }else if(!mstrcmp(command,"sds3d")){
	        myIcon->TraceType=3;
	    }else if(!mstrcmp(command,"depth")){
	        myIcon->TraceType=4;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        myIcon->CellSize=value;
	        if(myIcon->CellSize <= 0.0)myIcon->CellSize=1.0;
	        sprintf(WarningBuff,"depth  CellSize %g\n",myIcon->CellSize);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        return 0;
	    }else if(!mstrcmp(command,"group")){
	        myIcon->TraceType=5;
	        ++(cp->n);
			command=stringCommand(cp);
			if(!command)goto ErrorOut;
	        if(scene->groupname)cFree((char *)scene->groupname);
	        scene->groupname=strsave(command,9982);
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupTheta1=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupTheta2=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupThetan=(long)value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupPhi1=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupPhi2=value;
	        ++(cp->n);
	        if(doubleCommand(&value,cp))goto ErrorOut;
	        scene->groupPhin=(long)value;
	        sprintf(WarningBuff,"group  groupname %s\n",scene->groupname);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        sprintf(WarningBuff,"       groupTheta1 %g groupTheta2 %g groupThetan %ld\n",scene->groupTheta1,scene->groupTheta2,scene->groupThetan);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        sprintf(WarningBuff,"       groupPhi1   %g groupPhi2   %g groupPhin   %ld\n",scene->groupPhi1,scene->groupPhi2,scene->groupPhin);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        scene->xResolution=scene->groupThetan;
	        sprintf(WarningBuff,"xResolution  %ld\n",scene->xResolution);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        scene->yResolution=scene->groupPhin;
	        sprintf(WarningBuff,"yResolution  %ld\n",scene->yResolution);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        return 0;
		}else if(!mstrcmp(command,"bhangmeter")){
			myIcon->TraceType=6;
			++(cp->n);
			if(doubleCommand(&value,cp))goto ErrorOut;
			myIcon->CellSize=value;
			if(myIcon->CellSize <= 0.0)myIcon->CellSize=1.0;
			sprintf(WarningBuff,"bhangmeter  CellSize %g\n",myIcon->CellSize);
			if(scene->debug > 1)WarningBatch(WarningBuff);
			return 0;
		}
	    sprintf(WarningBuff,"use  %s\n",command);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("BackGround",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->BackGround.red=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->BackGround.green=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->BackGround.blue=value;
	    sprintf(WarningBuff,"BackGround red %f green %f blue %f\n",
	    		scene->BackGround.red,scene->BackGround.green,scene->BackGround.blue);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Eye",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp)){
			command=stringCommand(cp);
			if(!command)goto ErrorOut;
			if(!mstrcmp("left",command)){
			   	scene->EyeView=Scene_EyeLeft;
			    sprintf(WarningBuff,"Eyes - Left View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("right",command)){
			   scene->EyeView=Scene_EyeRight;
			    sprintf(WarningBuff,"Eyes - Right View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("normal",command)){
			   scene->EyeView=Scene_Eye;
			    sprintf(WarningBuff,"Eyes - Normal View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("anaglyph",command)){
			   scene->EyeView=Scene_Anaglyph;
			    sprintf(WarningBuff,"Eyes - Anaglyph View\n");
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}else if(!mstrcmp("seperation",command)){
			    ++(cp->n);
			    if(doubleCommand(&value,cp))goto ErrorOut;
	    		scene->EyeSeparation=value;
	    		sprintf(WarningBuff,"Eye - Separation %g\n",scene->EyeSeparation);
				if(scene->debug > 1)WarningBatch(WarningBuff);
			}    	    
	    }else{
		    scene->Eye.x=value;
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->Eye.y=value;
		    ++(cp->n);
		    if(doubleCommand(&value,cp))goto ErrorOut;
		    scene->Eye.z=value;
		    sprintf(WarningBuff,"Eye x %g y %g z %g\n",scene->Eye.x,scene->Eye.y,scene->Eye.z);
			if(scene->debug > 1)WarningBatch(WarningBuff);
		}
	}else if(!mstrcmp("Lamp",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Lamp.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Lamp.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Lamp.z=value;
	    sprintf(WarningBuff,"Lamp x %g y %g z %g\n",scene->Lamp.x,scene->Lamp.y,scene->Lamp.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Target",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Target.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Target.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Target.z=value;
	    sprintf(WarningBuff,"Target x %g y %g z %g\n",scene->Target.x,scene->Target.y,scene->Target.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("Up",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Up.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Up.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->Up.z=value;
	    sprintf(WarningBuff,"Up x %g y %g z %g\n",scene->Up.x,scene->Up.y,scene->Up.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("limits",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmin=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->xmax=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymin=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ymax=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmin=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->zmax=value;
	    sprintf(WarningBuff,"limits xmin %g ymin %g zmin %g\n",
	            myIcon->xmin,myIcon->ymin,myIcon->zmin);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"       xmax %g ymax %g zmax %g\n",
	            myIcon->xmax,myIcon->ymax,myIcon->zmax);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("reflect",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->xReflect=(int)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->yReflect=(int)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->zReflect=(int)value;
	    sprintf(WarningBuff,"reflect xReflect %d yReflect %d zReflect %d\n",
	            scene->xReflect,scene->yReflect,scene->zReflect);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("opensds3d",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(myIcon->filename,command,256);
	    openSDS(myIcon);
	}else if(!mstrcmp("savename",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    mstrncpy(scene->savename,command,256);
	}else if(!mstrcmp("filelist",command)){
	    if(FileManagerBatch(Batch,cp))goto ErrorOut;	
	}else if(!mstrcmp("stop",command)){
	    closeSDS(myIcon);
	    goto ErrorOut;
	}else if(!mstrcmp("end",command)){
	    closeSDS(myIcon);
	    goto ErrorOut;
	}
	
	ret = 0;
ErrorOut:
 	getCommand(NULL,&cpl);
	return ret;
}

static int doPrint(FilePtr Files,CommandPtr cp,BatchPtr Batch)
{
	struct vScene *scene;
	vIconPtr myIcon;
	char *command;
 	double value;
	int ret;

	if(!Files)return 1;
	if(!cp)return 1;
	if(!Batch)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	
	ret=1;
	

	 ++(cp->n);
	command=stringCommand(cp);
	if(!command)goto ErrorOut;

	if(!mstrcmp("eye",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->PrintEye=(int)value;
	}else if(!mstrcmp("time",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->PrintTime=(int)value;
	}else if(!mstrcmp("spheres",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->PrintSpheres=(int)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    scene->sphereSize=value;
	}
	ret = 0;
ErrorOut:
	return ret;
}

static int doMoveSetUp(FilePtr Files,CommandPtr cp,BatchPtr Batch)
{
	vIconPtr myIcon;
	struct vScene *scene;
	static struct MoveStruct Move;
	char *command;
 	double value;
	int ret;

	if(!Files)return 1;
	if(!cp)return 1;
	if(!Batch)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	
	ret=1;
	
	++(cp->n);
	command=stringCommand(cp);
	if(!command)goto ErrorOut;

	if(!mstrcmp("Steps",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Move.Steps=(long)value;
	    sprintf(WarningBuff,"orbits %ld \n",Move.Steps);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("stepframes",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Move.stepframes=(long)value;
	    sprintf(WarningBuff,"stepframes %ld \n",Move.stepframes);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("finddegrees",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Move.findDegrees=value;
	    sprintf(WarningBuff,"targetdegrees %g\n",Move.findDegrees);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("findpoint",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Move.FindPoint.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Move.FindPoint.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Move.FindPoint.z=value;
	    sprintf(WarningBuff,"FindPoint.x %g FindPoint.y %g FindPoint.z %g\n",Move.FindPoint.x,Move.FindPoint.y,Move.FindPoint.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("go",command)){
	    doMove(Files,&Move);
	}
	ret=0;
ErrorOut:
	return ret;
}

static int doOrbitSetUp(FilePtr Files,CommandPtr cp,BatchPtr Batch)
{
	vIconPtr myIcon;
	struct vScene *scene;
	static struct OrbitStruct Orbit=
	   { 1,1,20,10,20,0,
	     0.,0.,
	     {0.,0.,0.}
	   };
	char *command;
 	double value;
	int ret;

	if(!Files)return 1;
	if(!cp)return 1;
	if(!Batch)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	
	ret=1;
	
	++(cp->n);
	command=stringCommand(cp);
	if(!command)goto ErrorOut;

	if(!mstrcmp("orbits",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.orbits=(long)value;
	    sprintf(WarningBuff,"orbits %ld \n",Orbit.orbits);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("targetloops",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.targetloops=(long)value;
	    sprintf(WarningBuff,"targetloops %ld \n",Orbit.targetloops);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("in",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.in=(long)value;
	    sprintf(WarningBuff,"in %ld \n",Orbit.in);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("around",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.around=(long)value;
	    sprintf(WarningBuff,"around %ld \n",Orbit.around);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("stepframes",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.stepframes=(long)value;
	    sprintf(WarningBuff,"stepframes %ld \n",Orbit.stepframes);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("out",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.out=(long)value;
	    sprintf(WarningBuff,"out %ld \n",Orbit.out);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("targetradius",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.targetradius=value;
	    sprintf(WarningBuff,"targetradius %g\n",Orbit.targetradius);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("targetdegrees",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.targetdegrees=value;
	    sprintf(WarningBuff,"targetdegrees %g\n",Orbit.targetdegrees);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("offset",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.offset.x=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.offset.y=value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    Orbit.offset.z=value;
	    sprintf(WarningBuff,"offset.x %g offset.y %g offset.z %g\n",Orbit.offset.x,Orbit.offset.y,Orbit.offset.z);
		if(scene->debug > 1)WarningBatch(WarningBuff);
	}else if(!mstrcmp("go",command)){
	    doOrbit(Files,&Orbit);
	}
	ret=0;
ErrorOut:
	return ret;
}
int doSage(FilePtr Files,CommandPtr cp,BatchPtr Batch)
{
	long tiltsteps,rotatesteps;
	long CurrentFrame;
	char *command;
 	double value;
 	vIconPtr myIcon;
	SagePtr pio;
	char *name;
	int index;
	int ret;

	if(!Files || !cp || !Batch)return 1;
	myIcon=&Files->vmyIcon;
	pio=&myIcon->pio;
	
	ret=1;

	command=stringCommand(cp);
	if(!command)goto ErrorOut;

	if(!mstrcmp("sagetrace",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(command && !mstrcmp("all",command)){
	        sageTrace(Files,1);
	    }else{
	        sageTrace(Files,0);
	    }
	}else if(!mstrcmp("zonesperblock",command)){
	    ++(cp->n);
	     if(doubleCommand(&value,cp))goto ErrorOut;
		uPref.BlockSizeGlobal=(long)value;
		if(uPref.BlockSizeGlobal < 0)uPref.BlockSizeGlobal=0;
	}else if(!mstrcmp("range",command)){
	    doRange(myIcon);
	}else if(!mstrcmp("sagedata",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    CurrentFrame=(long)value;
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    name=command;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    index=(int)value;
	    sageData(Files,CurrentFrame,name,index);
	    doRange(myIcon);
	}else if(!mstrcmp("datatype",command)){
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)goto ErrorOut;
	    name=command;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    index=(int)value;	    
	    mstrncpy(pio->pioName,name,255);	
		pio->pioIndex=index;
	}else if(!mstrcmp("pos",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->CurrentFrame=(long)value;	    
	}else if(!mstrcmp("sagefile",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    if(openPIO(Files,(long)value))return 1;
	}else if(!mstrcmp("sagesample",command)){
	    if(openPIO(Files,myIcon->CurrentFrame))goto ErrorOut;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->ixmax=(long)value;
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->iymax=(long)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    myIcon->izmax=(long)value;
	    if(sageSample(myIcon))return 1;
	    doTrace(Files);
	}else if(!mstrcmp("savesds3d",command)){
	    ++(cp->n);
	    saveSDS3D(Files,(char *)command);
	}else if(!mstrcmp("sagerange",command)){
	    sageRange(Files);
	}else if(!mstrcmp("sagerotate",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    tiltsteps=(long)value;
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    rotatesteps=(long)value;
	    sageRotate(Files,tiltsteps,rotatesteps);
	}else if(!mstrcmp("sagesweep",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    tiltsteps=(long)value;
	    sageSweep(Files,tiltsteps);
	}else if(!mstrcmp("sagesweepdown",command)){
	    ++(cp->n);
	    if(doubleCommand(&value,cp))goto ErrorOut;
	    tiltsteps=(long)value;
	    sageSweepDown(Files,tiltsteps);
	}
	
	ret=0;
ErrorOut:
	return ret;
}
static int doMove(FilePtr Files,MovePtr Movein)
{
	struct vScene *scene;
	vIconPtr myIcon;
	struct P EyeSave,LampSave,TargetSave;
	struct P EyeToTarget;
	struct MoveStruct Moves;
	double start,end;
	double DegreesSave;
	double r1,r2,a1,a2;
	double pi;
	long o;
	long sequence;
	int ret;
	static long orbits=10;
	static double targetdegrees=0;
	struct P EyeToFind,TargetToFind;

	if(!Files)return 1;
	myIcon=&Files->vmyIcon;
	scene=&myIcon->scene;
	if(!Movein)return 1;
	
	Moves = *Movein;
	
	ret = 1;
	
	sequence=0;

	EyeSave=scene->Eye;

	LampSave=scene->Lamp;

	DegreesSave=scene->Degrees;

	TargetSave=scene->Target;
	
	start=rtime();

	if(sageSetFrame(Files,myIcon->CurrentFrame))goto ErrorOut;

	pi=4.0*atan(1.0);
	
	scene->FindPoint=Moves.FindPoint;
		
	targetdegrees=Moves.findDegrees;
	
	EyeToTarget=Sub(&scene->Target,&scene->Eye);

	EyeToFind=Sub(&scene->FindPoint,&scene->Eye);

	TargetToFind=Sub(&scene->FindPoint,&scene->Target);


	{
	    r1=Len(&EyeToTarget);  
	    a1=r1*tan(.5*DegreesSave*pi/180.);
	    r2=Len(&EyeToFind);
	    a2=r2*tan(.5*targetdegrees*pi/180.);
	}
	
	sequence=0;

	orbits=Moves.Steps;

	if(orbits < 0)orbits = 1;
	
	for(o=0;o<orbits;++o){
	    struct P Move;
	    double dm,s,rp,sp;

	    dm=0.0;
	    if(orbits > 1)dm = 1.0/((double)(orbits-1));

	    s=((double)(o)*dm);
	    Move=Mult(&TargetToFind,s);
	    scene->Target=Add(&TargetSave,&Move);
	    rp=(1.-s)*r1+s*r2;
	    sp=(1.-s)*a1+s*a2;
	    scene->Degrees=2.0*atan2(sp,rp)*180./pi;
	    
        if(TraceSetUp(myIcon))goto ErrorOut;
        
		if(Moves.stepframes){
			if(sageSetFrame(Files,sequence))goto ErrorOut;
		}
        if(sageDoTrace(Files))goto ErrorOut;
        ++sequence;	    
	        
	}
	


	ret=0;

	if(scene->debug > 1){
	    end=rtime();
	    sprintf(WarningBuff,"%.2f Seconds For %ld Steps\n",end-start,sequence);
	    WarningBatch(WarningBuff);
	}

ErrorOut:


	scene->Degrees=DegreesSave;
	scene->Eye=EyeSave;
	scene->Lamp=LampSave;
	scene->Target=TargetSave;
	return ret;
}

