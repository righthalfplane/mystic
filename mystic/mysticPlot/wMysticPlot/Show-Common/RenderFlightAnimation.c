#define EXTERN22 extern
#include "Xappl.h"
#include "Render.h"
#include "uFiles.h"
#include "uDialog.h"
#include "Message.h"
#include "NewEdit.h"
#include "FileManager.h"
#include "Universal.h"
#include "Defs.h"

CObjectPtr uFlightPathWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);

int SetScreen(struct Scene *scene);

static int CFlightPathkill(CObject *o);
static CObjectPtr CFlightPathDuplicate(CObject *b);
static int CFlightPathMessage(CObject *b,struct ObjectMessage *Message);
static int CFlightPathDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
static int CFlightPathDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
static int CObjectFlightPathCount(CObject *o,long *nodes,long *cells);
static int CObjectFlightPathMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
		      
CFlightPathPtr CFlightPathCreate(struct Scene *scene,struct P *flightPath,long flightCount,long SplineSteps);



int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene);	
void CObjectCreate(struct Scene *scene,CObject *b);
int MergeBlock(struct Scene *scene,struct Block *block);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
int Transform(struct P *p,struct Matrix *m);
void ScaleMatrix2(struct Matrix *m,struct System *p);


CSpherePtr CSphereCreate(struct Scene *scene,int mat,int nx,int ny,
                         double radius,struct P *origin);
int ListAddObjectList(CObjectList *d,CObject *Object);

static int doScale2Start(struct uDialogStruct *uList);
static int doScale2Move(struct uDialogStruct *uList);
static int doFlight(char *editWindowName,long startStep,IconPtr myIcon);
int doSetImageInfo(char *Name,char *Directory,int flag);
int Dump(struct Scene *scene,unsigned char *bline,
           unsigned char *bout,long xsize,long ysize,void *DumpData);
int TrueMergedDone(FILE *out);
int RenderScene(renderPtr openData);

static int getNumber(struct CExpress *e,double *x);

struct pathData{
    long Step;
     struct P Eye;
     struct P Target;
     double Degrees;
};

static struct pathData pd;
static pushpopPtr popStack;
static int doSpheres;



int bspline(int n, int t, struct P *control, struct P *output, int num_output);
void compute_point(int *u, int n, int t, double v, struct P *control,
		   struct P *output);
void compute_intervals(int *u, int n, int t);
double blend2(int k, int t, int *u, double v);


static int testSplines(int SlineSteps,int PassSmooth,int CyclicSmooth);

static int doView1Action(struct uDialogStruct *uList);


static int doSpheres2(IconPtr myIcon,long SplineSteps,int PassSmooth,int CyclicSmooth);

static int doSpheres2(IconPtr myIcon,long SplineSteps,int PassSmooth,int CyclicSmooth)
{
    struct P *flightPath;
	struct Scene *scene;
	renderPtr r;
    CFlightPathPtr s;
	long nn,n;
			
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	if(!doSpheres)return 1;
	
	flightPath=NULL;
 	nn=pushpopDepth(popStack);
 	if(nn <= 0)goto ErrorOut;
	flightPath=(struct P *)cMalloc(sizeof(struct P)*nn,4629);
	if(!flightPath)goto ErrorOut;
    for(n=0;n<nn;++n){
		pushpopPeek(&pd,n,popStack);
		
		flightPath[n].x=pd.Eye.x;
		flightPath[n].y=pd.Eye.y;
		flightPath[n].z=pd.Eye.z;
    }
	s=CFlightPathCreate(scene,flightPath,nn,SplineSteps);
	if(!s)goto ErrorOut;
    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)s)){
        (*s->killObject)((CObjectPtr)s);
        goto ErrorOut;
    }    	
	s->PassSmooth=PassSmooth;
	s->CyclicSmooth=CyclicSmooth;
    if(r)RenderScene(r);	
    
    flightPath=NULL;
ErrorOut:
	if(flightPath)cFree((char *)flightPath);
	return 0;
}

static int doView1Action(struct uDialogStruct *uList)
{
	IconPtr myIcon;	
	struct Scene *scene;
	renderPtr r;
	uDialogReturn *uR;
	int reti;
	int ret;
	
	if(!uList)return 1;
	myIcon=uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	ret = 1;
	uR=NULL;
	uSetCursor(uList->myIcon,-uWATCH_CURSOR);
	
	if(uList->actionItem == 8){
	
        doSpheres=TRUE;

		reti=uDialogSet(uList,0L,
          uDialogSetExitOk,
          uDialogSetDone,uDialogSetDone
		);

		if(reti)goto ErrorOut;	       
	}
	ret = 0;
ErrorOut:	
    if(uR)uDialogFreeReturn(uR);
	uSetCursor(uList->myIcon,-uARROW_CURSOR);
	return ret;
}

static int testSplines(int SplineSteps,int PassSmooth,int CyclicSmooth)
{
	struct P *control;
	struct P *output;
	int nn,n,k;

	control=NULL;
	output=NULL;
	
    nn=pushpopDepth(popStack);
    if(nn < 3)return 0;
    
    if(SplineSteps <= 0)SplineSteps=2*nn;
    
	control=(struct P *)cMalloc(sizeof(struct P)*nn,7263);
	output=(struct P *)cMalloc(sizeof(struct P)*SplineSteps,7264);
	if(!control || !output)goto ErrorOut;
    
    for(n=0;n<nn;++n){
		pushpopPeek(&pd,n,popStack);
    	control[n].x=pd.Eye.x;
    	control[n].y=pd.Eye.y;
    	control[n].z=pd.Eye.z;
    }
    
    bspline(nn-1,4,control,output,SplineSteps);
    
	for(k=0;k<PassSmooth;++k){
        for(n=2;n<SplineSteps;++n){
            output[n-1].x=(.5*output[n-1].x+.25*output[n-2].x+.25*output[n].x);
            output[n-1].y=(.5*output[n-1].y+.25*output[n-2].y+.25*output[n].y);
            output[n-1].z=(.5*output[n-1].z+.25*output[n-2].z+.25*output[n].z);
        }
        if(CyclicSmooth){
        
            output[SplineSteps-1].x=(.5*output[SplineSteps-1].x+.25*output[SplineSteps-2].x+.25*output[0].x);
            output[SplineSteps-1].y=(.5*output[SplineSteps-1].y+.25*output[SplineSteps-2].y+.25*output[0].y);
            output[SplineSteps-1].z=(.5*output[SplineSteps-1].z+.25*output[SplineSteps-2].z+.25*output[0].z);
            output[0].x=(.5*output[0].x+.25*output[SplineSteps-1].x+.25*output[1].x);
            output[0].y=(.5*output[0].y+.25*output[SplineSteps-1].y+.25*output[1].y);
            output[0].z=(.5*output[0].z+.25*output[SplineSteps-1].z+.25*output[1].z);
        }
    }
    
    
    if(pushpopPopAll(&pd,popStack))goto ErrorOut;

    for(n=1;n<SplineSteps;++n){
    	pd.Eye.x=output[n-1].x;
    	pd.Eye.y=output[n-1].y;
    	pd.Eye.z=output[n-1].z;
    	pd.Target.x=output[n].x;
    	pd.Target.y=output[n].y;
    	pd.Target.z=output[n].z;
        if(pushpopPush(&pd,popStack))goto ErrorOut;
    }
    
ErrorOut:
    
    if(control)cFree((char *)control);
    if(output)cFree((char *)output);
	return 0;
}

static int doFlight(char *editWindowName,long startStep,IconPtr myIcon)
{
	static char saveFileName[256]={'f','l','i','g','h','t','.','t','r','u',};
	char fileName[256],Directory[512],FileOut[1280];
	IconPtr StatusWindow;
 	struct Scene *scene;
	struct pathData pdo;
 	unsigned char *dat;
    struct CExpress e;
 	IconPtr pWindow;
    char buff[512];
    int SplineSteps;
	int CyclicSmooth;
	int PassSmooth;
	int saveWhere;
 	double start,end;
 	double pi;
	renderPtr r;
	double s,ds;
	double x;
	long k,kk;
	int nn,n;
	int ret;
	
	if(!editWindowName || !myIcon || !editWindowName[0])return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	ret=1;
	
	dat=NULL;
	
	StatusWindow=NULL;
	
	
	popStack=pushpopStart(&pd);
	if(!popStack)goto ErrorOut;		

	start=rtime();
	
	pWindow=FindWindowByName(editWindowName);
	
	if(!pWindow){
	    sprintf(WarningBuff,"Could Not Find Path Window %s\n",editWindowName);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	dat=EditGetData(pWindow);
	if(!dat){
	    sprintf(WarningBuff,"Found No Path Data In Window %s\n",editWindowName);
	    WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	
	
	nn=0;
	pd.Step=0;
	pd.Eye=scene->Eye;
	pd.Target=scene->Target;
	pd.Degrees=scene->Degrees;
	
	PassSmooth=0;
	SplineSteps=0;
	CyclicSmooth=0;
	
	zerol((char *)&e,sizeof(struct CExpress));
	e.dat=(unsigned char *)dat;
	while(!CExpressNext(&e)){
		 if(e.tok == T_EOF){
		      break;	
		 }	 
		 mstrncpy(buff,e.ch,512);
		 if(!mstrcmp(buff,"Step")){
		    if(nn > 0){
		 	    if(pushpopPush(&pd,popStack))goto ErrorOut;
		 	}
		    if(getNumber(&e,&x))continue;
			pd.Step=(long)x;
		    ++nn;
		 }else if(!mstrcmp(buff,"Degrees")){
		    if(getNumber(&e,&pd.Degrees))continue;
		 }else if(!mstrcmp(buff,"Target")){
		    if(getNumber(&e,&pd.Target.x))continue;
		    if(getNumber(&e,&pd.Target.y))continue;
		    if(getNumber(&e,&pd.Target.z))continue;
		 }else if(!mstrcmp(buff,"Eye")){
		    if(getNumber(&e,&pd.Eye.x))continue;
		    if(getNumber(&e,&pd.Eye.y))continue;
		    if(getNumber(&e,&pd.Eye.z))continue;
		 }else if(!mstrcmp(buff,"SplineSteps")){
		    if(getNumber(&e,&x))continue;
			SplineSteps=(long)x;
		 }else if(!mstrcmp(buff,"PassSmooth")){
		    if(getNumber(&e,&x))continue;
			PassSmooth=(long)x;
		 }else if(!mstrcmp(buff,"CyclicSmooth")){
		    if(getNumber(&e,&x))continue;
			CyclicSmooth=(long)x;
		 }else if(!mstrcmp(buff,"tilt")){
		    if(getNumber(&e,&x))continue;
			scene->tilt=x;
		 }else{
		    sprintf(WarningBuff,"Unknown Command = %s",buff);
	        WarningBatchFast(WarningBuff);
	     }

	}	
	if(pushpopPush(&pd,popStack))goto ErrorOut;	
	
	if(doSpheres){
		doSpheres2(myIcon,SplineSteps,PassSmooth,CyclicSmooth);
		goto ErrorOut; 
	}
	
	testSplines(SplineSteps,PassSmooth,CyclicSmooth);
	
	if(scene->sequenceSave > 0){	
	    saveWhere=doSetImageInfo(saveFileName,Directory,1);
	    if(saveWhere == 1)goto ErrorOut;
	}else{
	    saveWhere = 1;
	}


	StatusWindow=uStartStatusWindow(myIcon,"Saving Time Animation");
	if(StatusWindow == NULL){
	    sprintf(WarningBuff,"StatusWindow NULL\n");
		WarningBatch(WarningBuff);
	}

	if(saveWhere == 3){
	    scene->FileOut=FileOut;
	}else if(saveWhere == 2 || saveWhere == 4){
	    scene->Dump=Dump;
	    scene->DumpData=(void *)myIcon;
	    r->saveWhere=saveWhere;
	    r->saveDirectory=Directory;
	    r->saveName=fileName;
	    r->saveFileName=saveFileName;
	}
	
	if(scene->MotionBlur > 1)scene->MotionBlurGather=1;
	
	pi=4.0*atan(1.0);
	
	kk=0;
	pdo=pd;
    nn=pushpopDepth(popStack);
    for(n=0;n<nn;++n){
		pushpopPeek(&pd,n,popStack);
		if(n == 0)pdo=pd;
	   
	   if(pd.Step > 0){
	      ds=1.0/(double)(pd.Step+1);
	   }else{
	      ds=1.0;
	   }
	   s=0;
	   for(k=0;k<=pd.Step;++k){
	        s += ds;
			scene->Eye.x=pdo.Eye.x+s*(pd.Eye.x-pdo.Eye.x);
			scene->Eye.y=pdo.Eye.y+s*(pd.Eye.y-pdo.Eye.y);
			scene->Eye.z=pdo.Eye.z+s*(pd.Eye.z-pdo.Eye.z);
	 		scene->lamp.origin=scene->Eye;
			scene->Target.x=pdo.Target.x+s*(pd.Target.x-pdo.Target.x);
			scene->Target.y=pdo.Target.y+s*(pd.Target.y-pdo.Target.y);
			scene->Target.z=pdo.Target.z+s*(pd.Target.z-pdo.Target.z);
	 		if(scene->tilt != 0){
	 		    struct P e;
	 		    double rr,t,st,ct;
	 		    t=scene->tilt*pi/180.;
	 		    st=sin(t);
	 		    ct=cos(t);
	 		    SetScreen(scene);
				e=Sub(&scene->Target,&scene->Eye);
	 		    rr=sqrt(e.x*e.x+e.y*e.y+e.z*e.z);
				scene->Target.x=scene->Eye.x+rr*(st*scene->g.V2.x-ct*scene->g.V3.x);
				scene->Target.y=scene->Eye.y+rr*(st*scene->g.V2.y-ct*scene->g.V3.y);
				scene->Target.z=scene->Eye.z+rr*(st*scene->g.V2.z-ct*scene->g.V3.z);
	 		    
			}
	        scene->Degrees=pdo.Degrees+s*(pd.Degrees-pdo.Degrees);

/*		
		sprintf(WarningBuff,"Step %04ld Eye %g %g %g Target %g %g %g Degrees %g",
	             kk,scene->Eye.x,scene->Eye.y,scene->Eye.z,
	             scene->Target.x,scene->Target.y,scene->Target.z,
	             scene->Degrees);
	    WarningBatchFast(WarningBuff);
	   
*/		   
			if(kk++ < startStep)continue;
			scene->time=kk-1;
	        sprintf(fileName,"%s.%04ld.tru",scene->sequenceName,scene->sequence);
	        sprintf(buff,"Creating: %s",fileName);
	        mstrncpy(FileOut,Directory,1280);
	        strncatToPath(FileOut,fileName,1280);
	        uWriteStatusWindow(StatusWindow,buff);
	        if(uCheckStatusWindow(StatusWindow,"Stop Aninamtion"))goto ErrorOut;
	        RenderScene(r);
	        ++scene->sequence;
        }
        
       
        pdo=pd;
    }
	end=rtime();
	sprintf(WarningBuff,"Flight Animation %.2f Seconds To Render %ld Frames",end-start,kk);
	WarningBatchFast(WarningBuff);	
	WarningBatchAtEnd(WarningBuff);        
	
	ret =0;	
ErrorOut:

	pushpopEND(popStack);

	scene->MotionBlurGather=0;
	
	if(dat)cFree((char *)dat);

	uStopStatusWindow(StatusWindow);
	if(r->saveWhere == 2){
	    if(r->saveFiles){
	       FilesOpenDefault(r->saveFiles);
	    }
	    r->saveFiles=NULL;
	} else if(r->saveWhere == 4){
	    if(r->saveFILE){
	       TrueMergedDone(r->saveFILE);
	       r->saveFILE=NULL;
	    }
	}
	scene->FileOut=NULL;
	scene->Dump=NULL;
	scene->DumpData=NULL;
	
	return ret;
}
static int getNumber(struct CExpress *e,double *x)
{
    double sign;
    
	if(!e || !x)return 1;
	
     if(CExpressNext(e))return 1;
	 if(e->tok == T_MINUS){
         if(CExpressNext(e))return 1;
	     sign = -1.0;
	 }else{
	     sign = 1.0;
	 }
	
	*x=sign*atof(e->ch);
	
	return 0;
}
int RenderFlightAnimation(IconPtr myIcon)
{

	static uDialogGroup Items[] =

        {
      /* 00 */      {"Ok", {230,270,70,20},uDialogButton,uDialogNone},
      /* 01 */      {"Cancel", {320,270,80,20},uDialogButton,uDialogNone},
      /* 02 */      {"File List",{20,40,400,200},uDialogScrollList,uDialogNone},
      /* 03 */      {"0", {397,8,80,20},uDialogText,uDialogNone},
      /* 04 */      {"0", {397,33,80,20},uDialogText,uDialogGetLong},
      /* 05 */      {"Save Images", {329,66,125,20},uDialogCheck,uDialogGetLong},
      /* 06 */      {"root name", {317,9,77,20},uDialogString,uDialogNone},
      /* 07 */      {"sequence", {317,34,65,20},uDialogString,uDialogNone},
      /* 08 */      {"Add Path", {320,270,80,20},uDialogButton,uDialogNone},
      /* 09 */      {"start", {317,9,77,20},uDialogString,uDialogNone},
      /* 10 */      {"0", {397,9,80,20},uDialogText,uDialogGetLong},
                 
        };

    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
 	static uRect  Rect1   = {100,50,420,310};
 	char editWindowName[256];
 	static long startStep=0;

 	uDialogList *uList;
	uDialogReturn *uR;

 	struct Scene *scene;
	renderPtr r;
    int reti;
    int ret;
	
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;

	reti=1;
	
	uR=NULL;
	
	uList=NULL;
	
	doSpheres=FALSE;

	editWindowName[0]=0;
	
	uList=uDialogOpen("Render Flight Animation",&Rect1,ItemCount);
	if(!uList)goto ErrorOut;
	
	if(uDialogSetList(uList,Items,ItemCount))goto ErrorOut;
		
	ret=uDialogSet(uList,0L,	              
	              uDialogSetMyIcon,myIcon,	 
	              
	              uDialogSetItem,3L,
	              uDialogSetText,scene->sequenceName,
	              
	              uDialogSetItem,4L,
	              uDialogSetDouble,(double)scene->sequence,
	              
	              uDialogSetItem,5L,
	              uDialogSetLogical,(scene->sequenceSave == 1),
	              
	              uDialogSetItem,10L,
	              uDialogSetDouble,(double)startStep,
	              	              
		          uDialogSetStart,doScale2Start,
 	              uDialogSetMove,doScale2Move,
	              uDialogSetAction,doView1Action,	              
 
	              uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto ErrorOut;
	
	if(uDialog(uList))goto ErrorOut;
	
	uR=uDialogGetReturn(uList);

	if(!uR)goto ErrorOut;

	mstrncpy(scene->sequenceName,uR[3].sreturn,255);
	
	scene->sequence=uR[4].lreturn;
	
	scene->sequenceSave=uR[5].lreturn;
	
	startStep=uR[10].lreturn;
	
	
	{
		struct ScrollList *List2;
		long count,n;
		
		if(uDialogSet(uList,0L,
		              
	              uDialogSetItem,2L,
	             uDialogGetScrollList,&List2,
	              	              	              
	              uDialogSetDone,uDialogSetDone
	    ))goto ErrorOut;
	    
		count=ScrollListCount(List2);
		if(count){
		   n=ScrollListFirstSelect(List2);
		   if(count == 1)n=0;
		   if(n >= 0){
		       ScrollListGet(editWindowName,n,List2);
		   }else{
		       WarningBatch("No Window Was Selected For Animation Path\n");
		       goto ErrorOut;
		   }
		}		    	    
	}

	reti=0;
ErrorOut:	
	
    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
	
	if(reti== 0){
		return doFlight(editWindowName,startStep,myIcon);
	}
	
	return reti;	
	
}
int doScale2Move(struct uDialogStruct *uList)
{

	struct ScrollList *list2;
	uRect rList2;
	int ret;
	
	uRect r;
	
	
	if(!uList)return 1;
	
	
	uGetPortBounds(uList->myIcon,&r);
	
	ret=uDialogSet(uList,0L,
              uDialogSetLocation,10,r.ysize-25,
              
              uDialogSetItem,1L,
              uDialogSetLocation,r.xsize-85,r.ysize-25,
              	              	              
              uDialogSetItem,8L,
              uDialogSetLocation,r.xsize-85,r.ysize-55,
              	              	              
              uDialogSetItem,2L,
              uDialogGetRect,&rList2,
              uDialogGetScrollList,&list2,
              
               uDialogSetItem,3L,
               uDialogSetLocation,100,r.ysize-100,
             	              	              
               uDialogSetItem,4L,
               uDialogSetLocation,100,r.ysize-75,
             	              	              
               uDialogSetItem,5L,
               uDialogSetLocation,10,r.ysize-50,
             	              	              
               uDialogSetItem,6L,
               uDialogSetLocation,10,r.ysize-100,
             	              	              
               uDialogSetItem,7L,
               uDialogSetLocation,10,r.ysize-75,
             	              	              
               uDialogSetItem,9L,
               uDialogSetLocation,200,r.ysize-100,
             	              	              
               uDialogSetItem,10L,
               uDialogSetLocation,250,r.ysize-100,
             	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	if(ret)return 1;
	
	
			
	rList2.x = 15;
	rList2.y = 25;
	rList2.xsize = (r.xsize-20);
	rList2.ysize = r.ysize-125;
	
	ScrollListFitRect(&rList2,list2);
	
	
	ret=uDialogSet(uList,0L,
	
              uDialogSetItem,2L,
              uDialogSetRect,&rList2,
              	              	              
              uDialogSetDone,uDialogSetDone
    );
	
	return 0;
}

int doScale2Start(struct uDialogStruct *uList)
{
	struct ScrollList *List2;
	IconPtr myIcon;
	renderPtr r;
	IconPtr p;
	int ret;
	int n;
	
	
	if(!uList)return 1;
	myIcon=(IconPtr)uList->myIconItem;
	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;


	
	ret=uDialogSet(uList,2L,
              uDialogGetScrollList,&List2,
                            
              uDialogSetDone,uDialogSetDone
    );
    
	if(ret)return 1;
	
	n=0;
	p=IconRoot;
	while(p){
	
 		char sourceName[256];
 		char *names[1];
		int type=2;
		
		if(p->WindowType == ExpressionType){
			GetWindowName(p,sourceName,0);
			if(mstrcmp(sourceName,"BatchPrint")){
				names[0]= sourceName;
				ScrollListInsert(names,&type,n++,1L,List2);
			}
		}
	    p=p->IconNext;
	}
	ScrollListSetTop(0,List2);
	
	if(uList->myIcon)InvalRectMyWindow(uList->myIcon);
	
	return 0;
}
int bspline(int n, int tp, struct P *control, struct P *output, int num_output)
{
    double ds,s,t;
    double /*dc, */cs,dcl;
	int nn,ns;
	tp=tp;
	
	if(!control || !output || (num_output < 2) || (n < 2))return 1;
	
	output[0]=control[0];
	
	output[num_output-1]=control[n];
	
	ds=1.0/(double)(num_output-1);
	/*
	dc=1.0/(double)(n);
	*/
	s=ds;
	
	for(nn=1;nn<num_output-1;++nn,s += ds){
	    cs=s*(n);
	    ns=(int)cs;
	    dcl=cs-(double)ns;
	    if(ns <= 0){
	       t=dcl-1.0;
	       output[nn].x=0.5*t*(t-1.0)*control[0].x-(t+1.0)*(t-1.0)*control[1].x+0.5*(t)*(t+1.0)*control[2].x;
	       output[nn].y=0.5*t*(t-1.0)*control[0].y-(t+1.0)*(t-1.0)*control[1].y+0.5*(t)*(t+1.0)*control[2].y;
	       output[nn].z=0.5*t*(t-1.0)*control[0].z-(t+1.0)*(t-1.0)*control[1].z+0.5*(t)*(t+1.0)*control[2].z;
	    }else if(ns >= n-1){
	       t=dcl+1.0;
	       output[nn].x=0.5*(t-1.0)*(t-2.0)*control[n-2].x-(t)*(t-2.0)*control[n-1].x+0.5*(t)*(t-1.0)*control[n].x;
	       output[nn].y=0.5*(t-1.0)*(t-2.0)*control[n-2].y-(t)*(t-2.0)*control[n-1].y+0.5*(t)*(t-1.0)*control[n].y;
	       output[nn].z=0.5*(t-1.0)*(t-2.0)*control[n-2].z-(t)*(t-2.0)*control[n-1].z+0.5*(t)*(t-1.0)*control[n].z;
	    }else{
	       t=dcl;
	       output[nn].x=control[ns].x+0.5*(t)*(control[ns+1].x-control[ns-1].x)+
	            0.5*(t*t)*(-control[ns+2].x+4.0*control[ns+1].x-5.0*control[ns].x+
	                       2.0*control[ns-1].x)+
	            0.5*(t*t*t)*(control[ns+2].x-3.0*control[ns+1].x+3.0*control[ns].x-
	                         control[ns-1].x);
	       output[nn].y=control[ns].y+0.5*(t)*(control[ns+1].y-control[ns-1].y)+
	            0.5*(t*t)*(-control[ns+2].y+4.0*control[ns+1].y-5.0*control[ns].y+
	                       2.0*control[ns-1].y)+
	            0.5*(t*t*t)*(control[ns+2].y-3.0*control[ns+1].y+3.0*control[ns].y-
	                         control[ns-1].y);
	       output[nn].z=control[ns].z+0.5*(t)*(control[ns+1].z-control[ns-1].z)+
	            0.5*(t*t)*(-control[ns+2].z+4.0*control[ns+1].z-5.0*control[ns].z+
	                       2.0*control[ns-1].z)+
	            0.5*(t*t*t)*(control[ns+2].z-3.0*control[ns+1].z+3.0*control[ns].z-
	                         control[ns-1].z);
	    }	   
	     
	}
	
	
	return 0;
}

static int CObjectFlightPathMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CFlightPathPtr s=(CFlightPathPtr)o;
	long NodeCount;
	long CellCount;

	if(!s)return 1;
	
	NodeCount=s->nx*s->ny*(s->flightCount)+2*s->flightCount+3;
	CellCount=(s->nx-1)*(s->ny-1)*(s->flightCount)*5+3*s->flightCount+5;
		
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(s->scene->SmoothNormals)*MostNormals=1;
	return 0;
}

static int CObjectFlightPathCount(CObject *o,long *nodes,long *cells)
{
	CFlightPathPtr s=(CFlightPathPtr)o;

	if(!s)return 1;
	
	*nodes += s->nx*s->ny*(s->flightCount)+2*s->flightCount+3;
	*cells += (s->nx-1)*(s->ny-1)*(s->flightCount)+3*s->flightCount+5;
		
	return 0;
}
CObjectPtr uFlightPathWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CFlightPathStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",4000,universal_TypeDoubleArrayPtr,(void *)&b.flightPath,0L},				

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		
		{"REAL",4019,universal_TypeLong,(void *)&b.nx},
		{"REAL",4020,universal_TypeLong,(void *)&b.ny},
		
		{"DOUB",4021,universal_TypeDouble,(void *)&b.radius},	

		{"REAL",4030,universal_TypeLong,(void *)&b.flightCount},
		{"REAL",4031,universal_TypeLong,(void *)&b.SplineSteps},
		{"REAL",4032,universal_TypeLong,(void *)&b.PassSmooth},
		{"REAL",4033,universal_TypeLong,(void *)&b.CyclicSmooth},
		{"REAL",4034,universal_TypeLong,(void *)&b.Hide},
					
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CFlightPathPtr bp=(CFlightPathPtr)o;
	struct Material *Mat;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=CFlightPathCreate(l->scene,0,0,0);
		if(!bp)goto ErrorOut;
		if(bp->flightPath)cFree((char *)bp->flightPath);
		bp->flightPath=b.flightPath;
		b.flightPath=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		Mat=NULL;
		if(!SetMaterialProperty(l->scene,&Mat,bp->Material)){
		    if(Mat){
		        Mat->ColorCount=0;
		    }
		}
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.flightCount*3;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}	
		
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CFlightPathPtr CFlightPathCreate(struct Scene *scene,struct P *flightPath,long flightCount,long SplineSteps)
{
	struct Material *m;
	CFlightPathPtr s;
	struct Block b;

	if(!scene)return (CFlightPathPtr)NULL;

	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CFlightPathPtr)NULL;

	s=(CFlightPathPtr)cMalloc(sizeof(CFlightPath),8566);
	if(!s)goto ErrorOut;
	zerol((char *)s,sizeof(CFlightPath));


	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_FLIGHT_PATH;
	s->duplicateObject=CFlightPathDuplicate;
	s->countObject=CObjectFlightPathCount;
	s->pGetMost=CObjectFlightPathMost;
	s->drawObject=CFlightPathDrawLines;	
	s->message=CFlightPathMessage;		
	s->killObject=CFlightPathkill;
	s->ReadWrite=uFlightPathWrite;	
	
	s->flightPath=flightPath;
	s->flightCount=flightCount;
	s->SplineSteps=SplineSteps;
	s->vscale=1.0;
	s->ImageCount=1;
	s->tmax=1;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	m = &scene->material[s->Material];
	if(m){
		m->ColorCount=0;	
	}
	
	s->nx=20;
	s->ny=20;
		
	s->radiusStepFactor=0.75;
	s->radius=20;

	s->itemToPlot=2;
		
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CFlightPathPtr)NULL;
}
static int CFlightPathDrawLines(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CFlightPathPtr c=(CFlightPathPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	struct P *control;
	struct P *output;
	long *ndata,nel;
	long k,ks;
	long n,itIs;
	long SplineSteps;
	
	if(!c)return 0;
	scene=c->scene;
	if(!scene)return 0;

	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	
	if(flag == 1){
	    if((scene->material[c->Material].opacity != 1.0))return 0;
	}else if(flag == 2){
	    if((scene->material[c->Material].opacity == 1.0))return 0;
	}
    
	if(c->Hide)return 0;
	
	control=NULL;
	output=NULL;	

	CFlightPathDraw(o,WorldToScreen,CellCount,flag);

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	ndata=scene->cdata;

   /* c->FirstCell = *CellCount; */

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    	
	SplineSteps=c->SplineSteps;	    	
    if(SplineSteps <= 0)SplineSteps=2*c->flightCount;
	    	    	
	control=(struct P *)cMalloc(sizeof(struct P)*c->flightCount,7263);
	output=(struct P *)cMalloc(sizeof(struct P)*SplineSteps,7264);
	if(!control || !output)goto ErrorOut;
    
    for(n=0;n<c->flightCount;++n){
    	control[n].x=c->flightPath[n].x;
    	control[n].y=c->flightPath[n].y;
    	control[n].z=c->flightPath[n].z;
    }
    
    bspline(c->flightCount-1,4,control,output,SplineSteps);
	
	for(k=0;k<c->PassSmooth;++k){
        for(n=2;n<SplineSteps;++n){
            output[n-1].x=(.5*output[n-1].x+.25*output[n-2].x+.25*output[n].x);
            output[n-1].y=(.5*output[n-1].y+.25*output[n-2].y+.25*output[n].y);
            output[n-1].z=(.5*output[n-1].z+.25*output[n-2].z+.25*output[n].z);
        }
        if(c->CyclicSmooth){
        
            output[SplineSteps-1].x=(.5*output[SplineSteps-1].x+.25*output[SplineSteps-2].x+.25*output[0].x);
            output[SplineSteps-1].y=(.5*output[SplineSteps-1].y+.25*output[SplineSteps-2].y+.25*output[0].y);
            output[SplineSteps-1].z=(.5*output[SplineSteps-1].z+.25*output[SplineSteps-2].z+.25*output[0].z);
            output[0].x=(.5*output[0].x+.25*output[SplineSteps-1].x+.25*output[1].x);
            output[0].y=(.5*output[0].y+.25*output[SplineSteps-1].y+.25*output[1].y);
            output[0].z=(.5*output[0].z+.25*output[SplineSteps-1].z+.25*output[1].z);
        }
    }
    	    	
		
    k=0;
    ks=0;
	nel=0;
	for(n=0;n<SplineSteps;++n){
		dx=output[n].x+c->origin.x;
		dy=output[n].y+c->origin.y;
		dz=output[n].z+c->origin.z;
		xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		++k;

		if(n >= SplineSteps-1)continue;
		ndata[nel++]=c->Material;
		ndata[nel++]=ks;
		ndata[nel++]=ks+1;

		ks=k;
			    			    
	}
	    	    
	nel /= 3;
	    
	vdata=NULL;
	
	doNoShadowLines(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);

	*CellCount += nel;
	
    c->LastCell = *CellCount;
    
ErrorOut:

	if(control)cFree((char *)control);
	if(output)cFree((char *)output);

	return 0;
}
static int CFlightPathDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CFlightPathPtr c=(CFlightPathPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	struct Material *m;
	long *ndata,nel;
	long k,ks;
	Vec Lampl,Eyel;
	int i,j;
	long n,itIs;
	long nsphere;
	double r;

	if(!c)return 0;	
	scene=c->scene;
	if(!scene)return 0;
	
	m = &scene->material[c->Material];
	if(!m)return 0;

	if(flag == 1){
	    if((m->opacity != 1.0))return 0;
	}else if(flag == 2){
	    if((m->opacity == 1.0))return 0;
	}

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

	itIs=c->ItWas;
	if(itIs < 0)itIs=0;
	
	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	gdata=scene->gdata;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    c->FirstCell = *CellCount;

    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&c->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*c->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*c->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*c->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*c->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*c->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*c->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*c->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*c->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*c->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
	    	    
    r=c->radius;
    if(r <= 0)r=50;
    
    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);

        
    k=0;
    ks=0;
	nel=0;
	nsphere=0;
	for(n=0;n<c->flightCount;++n){

		if((k+c->ny*c->nx) > scene->MostNodes){
		   sprintf(WarningBuff,"CFlightPathDraw Skipping Memory Used %ld Avaiable %ld\n",
		           k,scene->MostNodes);
		   WarningBatch(WarningBuff);
		   goto OutOfHere;
		}

		++nsphere;
				    		   	   		   	    
		for(j=0;j<c->ny;++j){
		    double sinf,cosf;
		    sinf=sin(pi*(1.-j*dumy));
		    cosf=cos(pi*(1.-j*dumy));
		    for(i=0;i<c->nx;++i){
		       dx=c->flightPath[n].x+c->origin.x+r*cos(2*pi*(1.-i*dumx))*sinf;
		       dy=c->flightPath[n].y+c->origin.y+r*sin(2*pi*(1.-i*dumx))*sinf;
		       dz=c->flightPath[n].z+c->origin.z+r*cosf;
		       xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		       ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		       zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
			   vdata[k]=0;
		       ++k;
		    }
		}			    

		for(j=0;j<c->ny-1;++j){
		    for(i=0;i<c->nx-1;++i){
			    ndata[nel++]=c->Material;
			    ndata[nel++]=i+j*c->nx+ks;
			    ndata[nel++]=i+1+j*c->nx+ks;
			    ndata[nel++]=i+1+(j+1)*c->nx+ks;
			    ndata[nel++]=i+(j+1)*c->nx+ks;
		    }
		}

		ks=k;			    			    

	}
	
OutOfHere:
	    	    
	nel /= 5;
	    
	vdata=NULL;

	if(gdata){
	    k=0;
		for(n=0;n<nsphere;++n){
		    for(j=0;j<c->ny;++j){
		        double sinf,cosf;
		        sinf=sin(pi*(1.-j*dumy));
		        cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){
			        Vec P,N,D,L;
			        double len;
			        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
			        
				MakeVector(xdatal[k],ydatal[k],zdatal[k],P);
					
				MakeVector((dx)*v1x+(dy)*v1y+(dz)*v1z,(dx)*v2x+(dy)*v2y+(dz)*v2z,
					           (dx)*v3x+(dy)*v3y+(dz)*v3z,N);
					
				/* Vector Eye To Point */
					
		    		VecSub(P,Eyel,D);
		    		
				/* Check Direction To Eye Point */

				if((VecDot(D,N)) > 0.0){
				     VecNegate(N);
				}


				/*  Vector from Point to Lamp */

				VecSub(Lampl,P,L);
					
				/* Check Direction of Lamp */


				 if((VecDot(L,N)) > 0.0){
				     if((len=VecLen(N)) > 0.0){
				         N[0] /= len;
				         N[1] /= len;
				         N[2] /= len;
				     }
				     if((len=VecLen(L)) > 0.0){
				         L[0] /= len;
				         L[1] /= len;
				         L[2] /= len;
				     }
				     gdata[k]=VecDot(N,L);
				 }else{
				     gdata[k]=0.;
				 }
				 ++k;
			    }
		    }
		}
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	}else if(nvdata){
	    k=0;
		for(n=0;n<nsphere;++n){
	        for(j=0;j<c->ny;++j){
	            double sinf,cosf;
	            sinf=sin(pi*(1.-j*dumy));
	            cosf=cos(pi*(1.-j*dumy));
			    for(i=0;i<c->nx;++i){		        
			        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
			        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
			        dz=r*cosf;
					 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
					 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
					 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			    }
	    	}
	    }
	    if(rdata && sdata){
		    dx=0;
		    if(c->nx > 1)dx=1.0/((double)(c->nx-1));

		    dy=0;
		    if(c->ny > 1)dy=1.0/((double)(c->ny-1));

    	    k=0;
			for(n=0;n<nsphere;++n){
	            for(j=0;j<c->ny;++j){
			        for(i=0;i<c->nx;++i){
					    rdata[k]=(float)((i)*dx);
					    sdata[k++]=(float)((j)*dy);
			        }
		    	}
	    	}
	    }
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}
	*CellCount += nel;
    c->LastCell = *CellCount;

	return 0;
}
static int CFlightPathMessage(CObject *b,struct ObjectMessage *Message)
{
	CFlightPathPtr s=(CFlightPathPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	;
	}else if(Message->MessageType == OBJECT_MESSAGE_MESSAGE1000_TYPE){
		struct Message1000 *m;
		int MessageType;
		int n;
		
	    m=Message->m;
	    if(!m)return 0;
	    MessageType=(int)Message->MessageType1000;	    
	    switch(MessageType){
	    case MessageType_GET_FILE_LIST:
	    	{
			    int isFound;
			    
				if(!s->Files)return 0;
				
			    isFound=FALSE;
				for(n=0;n<m->FileCount;++n){
				    if(s->Files == m->FilesList[n]){isFound=TRUE;break;}
				}
				if(isFound == FALSE && (m->FileCount < m->FileCountMax-1)){
				    m->FilesList[m->FileCount++]=s->Files;
				}		
			}	
		    break;
	    case MessageType_GET_FILE_OWNER:
			for(n=0;n<m->FileCount;++n){
			    if(s->Files == m->FilesList[n])m->FileOwner[n] = TRUE;
			}
		    break;
	    case MessageType_GET_FRAME_COUNT:
		    if(s->ImageCount > m->FrameCount)m->FrameCount=s->ImageCount;
		    break;
		case MessageType_PRINT_FILE_INFORMATION:
	    	sprintf(WarningBuff,"  Tracer File\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	}else if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    double xdata[8],ydata[8],zdata[8];
		struct Matrix GroupToWorld;
		struct Matrix GroupToScreen;
	    struct P p;
	    long n;
	    
	    if(!s->setData)return 0;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);
	    
	    xdata[0]=s->xminData;
	    xdata[1]=s->xmaxData;
	    xdata[2]=s->xmaxData;
	    xdata[3]=s->xminData;
	    xdata[4]=s->xminData;
	    xdata[5]=s->xmaxData;
	    xdata[6]=s->xmaxData;
	    xdata[7]=s->xminData;

	    ydata[0]=s->yminData;
	    ydata[1]=s->yminData;
	    ydata[2]=s->ymaxData;
	    ydata[3]=s->ymaxData;
	    ydata[4]=s->yminData;
	    ydata[5]=s->yminData;
	    ydata[6]=s->ymaxData;
	    ydata[7]=s->ymaxData;

	    zdata[0]=s->zminData;
	    zdata[1]=s->zminData;
	    zdata[2]=s->zminData;
	    zdata[3]=s->zminData;
	    zdata[4]=s->zmaxData;
	    zdata[5]=s->zmaxData;
	    zdata[6]=s->zmaxData;
	    zdata[7]=s->zmaxData;


	    for(n=0;n<8;++n){
	        p.x=xdata[n];
	        p.y=ydata[n];
	        p.z=zdata[n];
	        Transform(&p,&GroupToScreen);
	        if(p.x > Message->xmax)Message->xmax=p.x;
	        if(p.y > Message->ymax)Message->ymax=p.y;
	        if(p.z > Message->zmax)Message->zmax=p.z;
	        if(p.x < Message->xmin)Message->xmin=p.x;
	        if(p.y < Message->ymin)Message->ymin=p.y;
	        if(p.z < Message->zmin)Message->zmin=p.z;
	    }
	}
	return 0;
}
static CObjectPtr CFlightPathDuplicate(CObject *b)
{
	CFlightPathPtr s=(CFlightPathPtr)b;
	CFlightPathPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CFlightPathPtr)cMalloc(sizeof(CFlightPath),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	if(s->flightPath && (s->flightCount > 0)){
	   sdsNew->flightPath=(struct P *)cMalloc(sizeof(struct P)*s->flightCount,8568);
	   if(!sdsNew->flightPath){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->flightCount;++n){
	       sdsNew->flightPath[n]=s->flightPath[n];
	   }
	}else{
	    sdsNew->flightPath=NULL;
	}

	return (CObjectPtr)sdsNew;
}
static int CFlightPathkill(CObject *o)
{
	CFlightPathPtr s=(CFlightPathPtr)o;
	if(!s)return 1;
	
	if(s->flightPath)cFree((char *)s->flightPath);
	s->flightPath=NULL;
	s->flightCount=0;

	cFree((char *)s);
	return 0;
}


