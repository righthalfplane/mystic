#define EXTERN22 extern
#include "Xappl.h"
#include "TracerDef.h"
#include "Render.h"
#include "Vrml.h"

#include "Defs.h"

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

static int doNoShadow4PBESTT(mThread *Threads);
static int doNoShadowPBESTT(mThread *Threads);
int doNoShadowVRMLBESTT(mThread *Threads);

int RenderKdTrace(renderPtr openData);

int putLong(long n,FILE *out);
int putInt(long n,FILE *out);

Flt VecNormalize(Vec vec);
long doStripes(void *v1,void *v2,void *surf_color);
long doImages(void *v1,void *v2,void *v3);
long doBumps(void *v1,void *v2,void *v3);

int ListObjectMessage(CObjectListPtr o,struct ObjectMessage *Message);

int ListGetMost(CObjectList *d,long *MostNodes,long *MostNormals,long *MostCells);

int uDrawPalette(struct paletteDraw *pd,IconPtr myIcon);
int uGetPaletteSize(long *xsizep,long *ysizep,struct paletteDraw *pd,IconPtr myIcon);

int ListObjectDraw(CObjectListPtr ObjectList,long *CellCount,int flag);

int doImageWrite(renderPtr openData,struct Scene *scene,float *vbuff);
int saveRenderImage(renderPtr openData,unsigned char *vbuff,long xResolution,long yResolution);
int oDumpit(renderPtr openData);

int doFastPlot(renderPtr	openData);
int doImage(struct Scene *scene);
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

static int vec3(struct P *x,struct P *y,struct P *z);

int Stuff(unsigned int *Esc1,unsigned int *Esc2,unsigned char *dataIn,unsigned char *dataOut,long Lin,long *Lout,unsigned int mask);

int deScaleComp(struct Scene *scene,unsigned char *bline,unsigned char *bout,long xsize,long ysize);

int WriteComp(renderPtr openData,struct Scene *scene,float *vbuff);

int RenderScene(renderPtr openData);

int doVrmlTexture(ColorVEC surf_color,double s1x,double s2x,long ns,long ip,struct tState *tS);

int RenderTrace(renderPtr openData);

int KdTreeTrace(renderPtr openData);

static int RenderFast(renderPtr openData);

int doAnaglyph(long xsize,long ysize,float *left,float *right);

static struct renderData rdl;

struct renderData *rd;

int SetClipPlane(struct Scene *scene);

static int doAntiAlias(int ixmax,int iymax,float *data);

static int doBuffers(struct Scene *scene,long *CellBuff,double *zbuff);
/*
static int doPlotData(int ixmax,int iymax,double *data,int flag);
*/
static long ipow2(int n);

struct pathData{
    long xResolution;
    long yResolution;
    float *data;
};

static struct pathData pd;
static pushpopPtr popStack;

static int doTemporal(struct Scene *scene,struct pathData *pd,float *vbuff);


static int doTemporal(struct Scene *scene,struct pathData *pd,float *vbuff)
{
	long nn,n,length,kk;
	float dnom;
	
	if(!scene || !pd || !vbuff)return 1;
	
	nn=pushpopDepth(popStack);
	if(nn < 1)return 1;
    for(n=0;n<nn;++n){
		pushpopPeek(pd,n,popStack);
		if((pd->xResolution != scene->xResolution)  || 
		   (pd->yResolution != scene->yResolution)  ||
		   !pd->data)return 1;
		   
	}
	
	length=pd->xResolution*scene->yResolution*3;
	
	for(n=0;n<length;++n)vbuff[n]=0;
	
	dnom=1.0f/(float)nn;
    for(n=0;n<nn;++n){
		pushpopPeek(pd,n,popStack);
		if(pd->data){
			for(kk=0;kk<length;++kk)vbuff[kk] += dnom*pd->data[kk];
			cFree((char *)pd->data);
			pd->data=NULL;
		}
		   
	}


	return 0;
}

int RenderScene(renderPtr openData)
{
	long xResolution,yResolution;
	struct Scene *scene;
	float *vbuff;
	struct P Eye;
	struct P EyeRight;
	long *CellBuff;
	double *zbuff;
	double stime;
	double etime;
	long length;
	int iWrite;
	int ret;
	int n,np;
	

	if(!openData)return 1;
	
	scene=openData->scene;

	if(!scene)return 1;
	
/*	
	sprintf(WarningBuff,"RenderScene Time %ld\n",(long)scene->time);
	WarningBatch(WarningBuff);
*/	
	if(!scene->AutoCenterScreenDone){
	    CenterScreen(scene,1);
	}
	
	
	stime=rtime();

	ret=1;
	
	iWrite=TRUE;
	
	zbuff=NULL;
	CellBuff=NULL;
	

	SetMaterialStuff(scene);
	SetScreen(scene);
	SetShadow(scene);
		
	Eye=scene->Eye;
	scene->EyeOffset=0;
	EyeRight=scene->EyeRight;
	xResolution=scene->xResolution;
	yResolution=scene->yResolution;

	np=1;
	vbuff=NULL;
	
	if(scene->EyeView == Scene_Anaglyph){
	    np=2;
		scene->Eye=scene->EyeLeft;
		scene->EyeOffset= -0.5*scene->EyeSeparation;
	}else if(scene->EyeView == Scene_EyeLeft){
		scene->Eye=scene->EyeLeft;
		scene->EyeOffset= -0.5*scene->EyeSeparation;
	}else if(scene->EyeView == Scene_EyeRight){
		scene->Eye=scene->EyeRight;
		scene->EyeOffset=0.5*scene->EyeSeparation;
	}
	
	if(scene->AntiAlias > 0){
	   scene->xResolution *= ipow2(scene->AntiAlias);
	   scene->yResolution *= ipow2(scene->AntiAlias);
	   CellBuff=scene->CellBuff;
	   zbuff=scene->zbuff;
	   
		length=scene->xResolution*scene->yResolution;
	   
	   	if(!(scene->CellBuff=(long  *)cMalloc(length*sizeof(long),8391))){
		    sprintf(WarningBuff,"RenderScene  CellBuff Out of memory Requested : (%ld)\n",length*sizeof(long));
			WarningBatch(WarningBuff);
		    goto ErrorOut;
		}
		
		if((scene->zbuff=(double *)cMalloc(length*sizeof(double),8392)) == NULL){
		    sprintf(WarningBuff,"RenderScene  zbuff Out of memory Requested : (%ld)\n",length*sizeof(double));
			WarningBatch(WarningBuff);
		    goto ErrorOut;
		}

	}

	
	for(n=0;n<np;++n){

		if(scene->RayTracing == 1){
		   /* ret=RenderKdTrace(openData); */
		   ret=RenderTrace(openData);
		}else if(scene->RayTracing == 2){
		   ret=KdTreeTrace(openData);
		}else{
		    ret=RenderFast(openData);
		}
		
		if(ret)goto ErrorOut;
		
		scene->Eye=EyeRight;
		scene->EyeOffset=0.5*scene->EyeSeparation;
		
		if(!vbuff)vbuff=rd->vbuff;
	}
	
	if(scene->EyeView == Scene_Anaglyph){
	    doAnaglyph(scene->xResolution,scene->yResolution,vbuff,rd->vbuff);
	    if(vbuff)cFree((char *)vbuff);
	}
	
	/* Temporal save Images until count complete */
	
	if(scene->MotionBlurGather > 0){
	    if(!popStack){
			popStack=pushpopStart(&pd);
			if(!popStack)goto ErrorOut;		
	    }
	    pd.xResolution=scene->xResolution;
	    pd.yResolution=scene->yResolution;
	    pd.data=(float *)cMalloc(pd.xResolution*pd.yResolution*sizeof(float)*3,7204);
	    if(!pd.data)goto ErrorOut;
	    memcpy(pd.data,rd->vbuff,pd.xResolution*pd.yResolution*sizeof(float)*3);
		if(pushpopPush(&pd,popStack))goto ErrorOut;
		if(scene->MotionBlurGather++ >= scene->MotionBlur){
		    if(doTemporal(scene,&pd,rd->vbuff))goto ErrorOut;
			pushpopEND(popStack);
			iWrite=TRUE;
			popStack=NULL;
			scene->MotionBlurGather=1;
		}else{
			iWrite=FALSE;
		}
	}

	if(scene->AntiAlias > 0){
	    for(n=0;n<scene->AntiAlias;++n){
	        if(iWrite)doAntiAlias(scene->xResolution,scene->yResolution,rd->vbuff);
	   		scene->xResolution /= 2;
	   		scene->yResolution /= 2;
	   		scene->odx /= 2;
	   		scene->ody /= 2;
	    }
	    
	    if(iWrite)doBuffers(scene,CellBuff,zbuff);
    	cFree((char *)scene->CellBuff);
    	scene->CellBuff=CellBuff;
    	cFree((char *)scene->zbuff);
    	scene->zbuff=zbuff;
	    
	}
		
	if(iWrite){

		doImageWrite(openData,scene,rd->vbuff);
	
		oDumpit(openData);
	}
	
	ret=0;

	etime=rtime()-stime;

	sprintf(WarningBuff,"Time To Render Image %.2f Seconds\n",etime);
	if(scene->debug > 0)WarningBatch(WarningBuff);
	
ErrorOut:

	scene->Eye=Eye;
	scene->xResolution=xResolution;
	scene->yResolution=yResolution;
	
	if(rd->vbuff)cFree((char *)rd->vbuff);
	
	rd->vbuff=NULL;
	
	if(scene->MotionBlurGather < 1 && popStack){	
	   long nn;
		nn=pushpopDepth(popStack);
	    for(n=0;n<nn;++n){
			pushpopPeek(&pd,n,popStack);
			if(pd.data){
				cFree((char *)pd.data);
				pd.data=NULL;
			}
		}	
		pushpopEND(popStack);
		popStack=NULL;
	}
		
	return ret;
}
/*
static int doPlotData(int ixmax,int iymax,double *data,int flag)
{
	IconPtr  NewNetWindow(IconPtr myIcon);
	IconPtr p;
	double *zbuff;
	long *Cells;
	long length,n;
	
	if(!data)return 1;
	
	zbuff=NULL;
	if(flag){
	    length=ixmax*iymax;
		if((zbuff=(double *)cMalloc(length*sizeof(double),8392)) == NULL){
		    sprintf(WarningBuff,"RenderScene  zbuff Out of memory Requested : (%ld)\n",length*sizeof(double));
			WarningBatch(WarningBuff);
		    goto ErrorOut;
		}
		Cells=(long *)data;
		for(n=0;n<length;++n)zbuff[n]=(double)Cells[n];
		data=zbuff;
		
		
	}
	p=NewNetWindow(NULL);
	if(p){
		    struct SetFrameData  sd;
			zerol((char *)&sd,sizeof(struct SetFrameData));
		    sd.type=FRAME_DATA_FLOAT;
	        sd.xsize=ixmax;
	        sd.ysize=iymax;
	        sd.zsize=1;
	        sd.data=data;
	        	        	        	        
	    	sd.CurrentFrame=0;	    			    	
			if(SendMessageByIcon(p,MessageType_SET_REGION_DATA,&sd)){	        	        
				    sprintf(WarningBuff,"CExpressDoEqual - Window  Error Puting Data\n");
				    WarningBatch(WarningBuff);
		    }
	}
ErrorOut:
    if(zbuff)cFree((char *)zbuff);
	return 0;
}
*/
static int doBuffers(struct Scene *scene,long *CellBuff,double *zbuff)
{
	long ix,iy;
	long nant;
	
	
	if(!scene || !CellBuff || !zbuff || !scene->CellBuff || !scene->zbuff)return 1;
	
	nant=scene->AntiAlias*2;
	

	for(iy=0;iy<scene->yResolution;++iy){
	    for(ix=0;ix<scene->xResolution;++ix){
	        CellBuff[ix+iy*scene->xResolution]=scene->CellBuff[ix*nant+iy*nant*nant*scene->xResolution];
	        zbuff[ix+iy*scene->xResolution]=scene->zbuff[ix*nant+iy*nant*nant*scene->xResolution];
	    }
	}
	
	
	return 0;
}
static long ipow2(int n)
{
    long ret;
    
    ret=1;
    
	if(n <= 0)return ret;
	
	while(n--)ret *= 2;
	
	return ret;
}
static int doAntiAlias(int ixmax,int iymax,float *data)
{
	long n1,n2,n3,n4,n5,n6,n7,n8,n9;
	float *bline,*bc,*bp,d1,d2,d3;
	long LineSize,length,n;
	int ix,iy;
	
	if(!data)return 1;

	LineSize=ixmax*3;
	
	length=LineSize*iymax;
	
    if(SetBuffers(length*sizeof(float))){
        WarningBatch("doAntiAlias Out of Memory\n");
        return 1;
    }
    
	bline=(float *)xg.buffin;
	
	for(n=0;n<length;++n)bline[n]=data[n];
	
	bp=bline;
	bc=data;

	ixmax=ixmax/2;
	iymax=iymax/2;
	for(iy=0;iy<iymax;++iy){
	    for(ix=0;ix<ixmax;++ix){
	        n1=2L*ix*3+2L*iy*LineSize;
		if(ix == 0){
		    if(iy == 0){
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        d1= 0.1111f*(4*bp[n1]+2*(bp[n2]+bp[n4])+bp[n3]);
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        d2= 0.1111f*(4*bp[n1]+2*(bp[n2]+bp[n4])+bp[n3]);
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        d3= 0.1111f*(4*bp[n1]+2*(bp[n2]+bp[n4])+bp[n3]);
		    }else{
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-LineSize;
		        n6=n5+3;
		        d1= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5])+(bp[n3]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-LineSize;
		        n6=n5+3;
		        d2= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5])+(bp[n3]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-LineSize;
		        n6=n5+3;
		        d3= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5])+(bp[n3]+bp[n6]));
		    }
		}else{
		    if(iy == 0){
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        d1= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n3]+bp[n5])+(bp[n4]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        d2= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n3]+bp[n5])+(bp[n4]+bp[n6]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        d3= 0.08333f*(4*bp[n1]+2*(bp[n2]+bp[n3]+bp[n5])+(bp[n4]+bp[n6]));
		    }else{
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        n7=n5-LineSize;
		        n8=n1-LineSize;
		        n9=n4-LineSize;
		        d1= 0.0625f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5]+bp[n8])+(bp[n3]+bp[n6]+bp[n7]+bp[n9]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        n7=n5-LineSize;
		        n8=n1-LineSize;
		        n9=n4-LineSize;
		        d2= 0.0625f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5]+bp[n8])+(bp[n3]+bp[n6]+bp[n7]+bp[n9]));
		        n1=n1+1;
		        n2=n1+LineSize;
		        n3=n2+3;
		        n4=n1+3;
		        n5=n1-3;
		        n6=n2-3;
		        n7=n5-LineSize;
		        n8=n1-LineSize;
		        n9=n4-LineSize;
		        d3= 0.0625f*(4*bp[n1]+2*(bp[n2]+bp[n4]+bp[n5]+bp[n8])+(bp[n3]+bp[n6]+bp[n7]+bp[n9]));
		    }
		}

	        *bc++ = d1;
	        *bc++ = d2;
	        *bc++ = d3;
	    }
	}
	
	return 0;
}
int doAnaglyph(long xsize,long ysize,float *left,float *right)
{
	long length,n;
	
	if(!left || !right)return 1;
	
    length=xsize*ysize;
    
    for(n=0;n<length;++n){
        right[3*n]=left[3*n];
        right[3*n+1]=right[3*n+1];
        right[3*n+2]=right[3*n+2];      
    }
	
	return 0;
}

static int RenderFast(renderPtr openData)
{
	struct Scene *scene;

	
	if(!openData)return 1;
	
	scene=openData->scene;

	if(!scene)return 1;
		
	rd = &rdl;
	
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
		scene->doNoShadowP=doNoShadowPBEST;
		scene->doNoShadow4P=doNoShadow4PBEST;
	    scene->doNoShadowVRML=doNoShadowVRMLBEST;
	}
			
	doFastPlot(openData);
		
	return 0;

}
int doFastPlot(renderPtr openData)
{
	long length,xResolution,yResolution,n,n3;
	Vec		BackgroundColor;
	struct Scene *scene;
	char buff[256];
/*
	struct sWorld World;
*/

	scene=openData->scene;
	
	rd->shadowdata=NULL;
	rd->zbuff=NULL;
	rd->vbuff=NULL;	
	
	rd->o=scene->EyeOffset;
	rd->h=scene->Height;

	MakeVector(0.,0.,scene->Height,rd->Eyel);

	xResolution=scene->xResolution;
	yResolution=scene->yResolution;

	
	length=xResolution*yResolution;

	rd->zbuff=scene->zbuff;
		
	for(n=0;n<length;++n){
	    rd->zbuff[n] = (-1.0e30);
	    scene->CellBuff[n] = -1;
	}
	
	if((rd->vbuff=(float *)cMalloc(3*length*sizeof(float),8636)) == NULL){
	    sprintf(buff,"doFastPlot  zbuff Out of memory Requested (%ld)",length*sizeof(double));
	    Warning(buff);
	    return 1;
	}

	ColorToVec(scene->BackGround,BackgroundColor);
	
	n3=0;
	for(n=0;n<length;++n){
	    rd->vbuff[n3++]=(float)BackgroundColor[0];
	    rd->vbuff[n3++]=(float)BackgroundColor[1];
	    rd->vbuff[n3++]=(float)BackgroundColor[2];
	}

	doImage(scene);

	if(rd->shadowdata)cFree((char *)rd->shadowdata);

	return 0;
}
int RenderSceneSetTime(struct Scene *scene)
{
    struct ObjectMessage Message;
	CObjectListPtr ObjectList;
	
	if(!scene)return 1;
	ObjectList=scene->ObjectList;


    Message.MessageType=OBJECT_MESSAGE_SET_TIME;
    Message.time=scene->time;
    ListObjectMessage(ObjectList,&Message);

	return 0;
}
int doImage(struct Scene *scene)
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
	long length,n,overwrite;
	long *CellBuff;
	long long MemoryUsed;
	
	CellBuff=scene->CellBuff;

	Transparency=(int)scene->Transparency;
	ObjectList=scene->ObjectList;

	scene->MostCells=0;
	scene->MostNodes=0;
	scene->MostNormals=0;

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

	MemoryUsed=0;
	
	if(scene->SmoothNormals == 1){
		MemoryUsed += scene->MostNodes*sizeof(double);
	    if((gdata=(double *)cMalloc(scene->MostNodes*sizeof(double),8637)) == NULL){
			sprintf(WarningBuff,"doImage gdata Out of memory Requested (%ld)\n",scene->MostNodes*sizeof(double));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}else if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
		MemoryUsed += 5*scene->MostNodes*sizeof(float);
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
		MemoryUsed += scene->MostCells*sizeof(long);
	    if((cdata=(long *)cMalloc(scene->MostCells*sizeof(long),8641)) == NULL){
			sprintf(WarningBuff,"doImage cdata Out of memory Requested (%ld)\n",scene->MostCells*sizeof(long));
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}

	length=scene->xResolution*scene->yResolution;
	if(Transparency){	
		MemoryUsed += length*sizeof(double)+5*length*sizeof(float);
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

	MemoryUsed += 4*scene->MostNodes*sizeof(double);
	
	if(scene->debug > 1){
	    sprintf(WarningBuff,"MostNodes %ld MostNormals %ld MostCells %ld MemoryUsed %lld\n",
	            scene->MostNodes,scene->MostNormals,scene->MostCells,MemoryUsed);
		WarningBatch(WarningBuff);
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

	if(scene->debug > 3){
	    sprintf(WarningBuff,"raster xmin %f xmax %f delx %f\n",rd->xmin,rd->xmax,delx);
		WarningBatch(WarningBuff);
	}
	if(scene->debug > 3){
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

	if(Transparency){
	    int ntry;

	    CellCount=0;
	    ListObjectDraw(ObjectList,&CellCount,1);

	    rd->doTransparency=TRUE;
	    ntry=0;
        do{

		    for(n=0;n<length;++n){
		        rd->zbuff2[n]=rd->h;
		        rd->cellbuff2[n] = -1;
		    }

		    CellCount=0;
		    ListObjectDraw(ObjectList,&CellCount,2);

		    overwrite=0;
		    for(n=0;n<length;++n){
		        if(rd->cellbuff2[n] != -1){
		            ++overwrite;
		            rd->zbuff[n]=rd->zbuff2[n];
				    rd->vbuff[3*n]=  (1.0f-rd->opacity[n])*rd->vbuff[3*n]  +rd->opacity[n]*rd->vbuff2[3*n];
				    rd->vbuff[3*n+1]=(1.0f-rd->opacity[n])*rd->vbuff[3*n+1]+rd->opacity[n]*rd->vbuff2[3*n+1];
				    rd->vbuff[3*n+2]=(1.0f-rd->opacity[n])*rd->vbuff[3*n+2]+rd->opacity[n]*rd->vbuff2[3*n+2];
				    CellBuff[n]=rd->cellbuff2[n];
		        }
		    }

		 }while (++ntry < scene->TransparencyPasses && overwrite > 0);

		 if(overwrite > 0){
			sprintf(WarningBuff,"Transparency incomplete after %d passes for %ld pixels out of %ld pixels\n",ntry,overwrite,length);
			WarningBatch(WarningBuff);
		 }
	}else{
	    CellCount=0;
	    ListObjectDraw(ObjectList,&CellCount,0);
	}
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

Flt VecNormalize(Vec vec)
{
        Flt len ;
        len = (Flt) VecLen(vec);
        if(len > 0.0){
            vec[0]/=len ;
            vec[1]/=len ;
            vec[2]/=len ;
        }
        return(len) ;
}
int doNoShadowLines4(long *ndata,double *xdata,double *ydata,double *zdata,
                double *vdata,long nel,long CellCount,struct Scene *scene,int DrawZonesColor)
{
	double s1x,d1x;
	double *bs;
	double x,y;
	long ixx;
	double p1x,p1y,p1z,p2x,p2y,p2z,ztran1,ztran2;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	int dx,dy,dxy;
	double z,*b,intensity;
	float *v,*vs;
	long nn,count,over;
	long n1,n2;
	long xsize3;
	int rastorY,ic;
	int ir;
	
	double cvalue,value1,dvalue1;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
    long *CellBuff;
	
	if(!ndata || !xdata || !ydata || !zdata || !vdata || !scene)return 1;
		
	CellBuff=scene->CellBuff;
	
	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}



	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){	    
		    nt=5*nn;
		for(ir=0;ir<4;++ir){
			if(ir == 0){
			  n1=ndata[nt+1];
			  n2=ndata[nt+2];
			}else if(ir == 1){
			  n1=ndata[nt+2];
			  n2=ndata[nt+3];
			}else if(ir == 2){
			  n1=ndata[nt+3];
			  n2=ndata[nt+4];
			}else{
			  n1=ndata[nt+4];
			  n2=ndata[nt+1];
			}
		    p1z=zdata[n1];
	        if(p1z >= rd->h)continue;  

		    p2z=zdata[n2];
	        if(p2z >= rd->h)continue;  

		    ztran1 = rd->h/(rd->h-p1z);
		    p1x=(rd->o+ztran1*(xdata[n1]-rd->o)-rd->xmin)*rd->odx;
		    
		    ztran2 = rd->h/(rd->h-p2z);
		    p2x=(rd->o+ztran2*(xdata[n2]-rd->o)-rd->xmin)*rd->odx;

		    ix = (int)p1x;
		    ixmax = ix;
		    ixmin = ix;
		    ix = (int)p2x;
		    if(ix > ixmax){
		        ixmax = ix;
		    }else if(ix < ixmin){
		        ixmin = ix;
		    }

		    if((ixmax < 0) || (ixmin > xsizen)){
		        ++CellSkip;
		        continue;
		    }

		    p1y=(ztran1*ydata[n1]-rd->ymin)*rd->ody;
		    p2y=(ztran2*ydata[n2]-rd->ymin)*rd->ody;

		    iy = (int)p1y;
		    iymax = iy;
		    iymin = iy;
		    	    
		    
		    iy = (int)p2y;
		    if(iy > iymax){
		        iymax = iy;
		    }else if(iy < iymin){
		        iymin = iy;
		    }
		    

		    if((iymax < 0) || (iymin > ysizen)){
		        ++CellSkip;
		        continue;
		    }

		    dx=ixmax-ixmin;
		    dy=iymax-iymin;
	/*
		    if(dx == 0 && dy == 0){
		        ++CellSkip;
		        continue;
		    }
	*/	    
		    dxy=(dx+dy+2)*2;

			d1x=1.0/(dxy-1.);
			

			material=ndata[nt];
			surf = &scene->material[material];

		    if(vdata){
				value1=vdata[n1];
		        dvalue1=vdata[n2]-value1;
	        }else{
				value1=0;
		        dvalue1=0;
		    }
		    

		    if(ixmin < 0)ixmin=0;
		    if(iymin < 0)iymin=0;
		    if(ixmax > xsizen)ixmax=xsizen;
		    if(iymax > ysizen)iymax=ysizen;
		    
			value=1.0;

	    if(rd->doTransparency){
		  bs=(double *)rd->zbuff2;
		  vs=(float *)rd->vbuff2;
		    b=bs;
		    v=vs;
		    itIn=FALSE;
			for(ixx=0;ixx<=dxy;++ixx){ 
			    s1x=(double)ixx*d1x;
			    x=p1x+s1x*(p2x-p1x);
			    y=p1y+s1x*(p2y-p1y);
				ix=(int)(x+0.5);
				iy=(int)(y+0.5);
				if((ix >= 0 && ix < xsizen) && (iy >= 0 && iy < ysizen)){
				    z=p1z+s1x*(p2z-p1z);
					iy=(rastorY-iy-1);
					if(z < b[ix+iy*xsize]){
					    if(z <= rd->zbuff[ix+iy*xsize])continue;
						if(DrawZonesColor && surf->flag & M_COLORLEVEL){
						    cvalue=value1+s1x*dvalue1;
						    colorit(surf->ColorCount,surf->ColorLevel,
						    cvalue,&ic);
						    ColorToVec(surf->ColorValue[ic],surf_color);
						}else{
							/* ColorToVec(surf->diffuse,surf_color); */
							surf_color[0]=0;
							surf_color[1]=0;
							surf_color[2]=0;
						}
					    v[3*ix+iy*xsize3]=(float)(value*surf_color[0]);
					    v[3*ix+1+iy*xsize3]=(float)(value*surf_color[1]);
					    v[3*ix+2+iy*xsize3]=(float)(value*surf_color[2]);
						rd->cellbuff2[ix+iy*xsize]=CellCount+nn;
						rd->opacity[ix+iy*xsize]=(float)surf->opacity;
					    b[ix+iy*xsize]=z;
					}else{
					    ++over;
					}
					++count;
					itIn=TRUE;
				}else {
				    if(itIn)break;
				}
			}
		  
	    }else{
		  bs=(double *)rd->zbuff;
		  vs=(float *)rd->vbuff;
		    b=bs;
		    v=vs;
		    itIn=FALSE;
			for(ixx=0;ixx<=dxy;++ixx){ 
			    s1x=(double)ixx*d1x;
			    x=p1x+s1x*(p2x-p1x);
			    y=p1y+s1x*(p2y-p1y);
				ix=(int)(x+0.5);
				iy=(int)(y+0.5);
				if((ix >= 0 && ix < xsizen) && (iy >= 0 && iy < ysizen)){
				    z=p1z+s1x*(p2z-p1z);
					iy=(rastorY-iy-1);
					if(z > b[ix+iy*xsize]){
						if(DrawZonesColor && surf->flag & M_COLORLEVEL){
						    cvalue=value1+s1x*dvalue1;
						    colorit(surf->ColorCount,surf->ColorLevel,
						    cvalue,&ic);
						    ColorToVec(surf->ColorValue[ic],surf_color);
						}else{
							/* ColorToVec(surf->diffuse,surf_color); */
							surf_color[0]=0;
							surf_color[1]=0;
							surf_color[2]=0;
						}
					    v[3*ix+iy*xsize3]=(float)(value*surf_color[0]);
					    v[3*ix+1+iy*xsize3]=(float)(value*surf_color[1]);
					    v[3*ix+2+iy*xsize3]=(float)(value*surf_color[2]);
						CellBuff[ix+iy*xsize]=CellCount+nn;
					    b[ix+iy*xsize]=z;
					}else{
					    ++over;
					}
					++count;
					itIn=TRUE;
				}else {
				    if(itIn)break;
				}
			}
		  
		}
	  }
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	return 0;

}

int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,
                double *vdata,long nel,long CellCount,struct Scene *scene)
{
	double s1x,d1x;
	double *bs;
	double x,y;
	long ixx;
	double p1x,p1y,p1z,p2x,p2y,p2z,ztran1,ztran2;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	int dx,dy,dxy;
	double z,*b,intensity;
	float *v,*vs;
	long nn,count,over;
	long n1,n2;
	long xsize3;
	int rastorY,ic;
	
	double cvalue,value1,dvalue1;
	struct Material *surf;
	static struct Material surfl;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
    long *CellBuff;
	
	if(!ndata || !xdata || !ydata || !zdata || !scene || !rd->zbuff)return 1;
		
	CellBuff=scene->CellBuff;
	
	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}

	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){	    
	    nt=3*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;

	    p1z=zdata[n1];
        if(p1z >= rd->h)continue;  

	    p2z=zdata[n2];
        if(p2z >= rd->h)continue;  

	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[n1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[n2]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[n1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[n2]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    

	    if((iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    dx=ixmax-ixmin;
	    dy=iymax-iymin;
/*
	    if(dx == 0 && dy == 0){
	        ++CellSkip;
	        continue;
	    }
*/	    
	    dxy=(dx+dy+2)*2;

		d1x=1.0/(dxy-1.);
		
	    material=ndata[nt];
	    surf = &scene->material[material];

	    if(vdata){
			value1=vdata[n1];
	        dvalue1=vdata[n2]-value1;
        }else{
			value1=0;
	        dvalue1=0;
	        surf=&surfl;
	    }
	    

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;
	    
		value=1.0;

    if(rd->doTransparency){
		  bs=(double *)rd->zbuff2;
		  vs=(float *)rd->vbuff2;
		    b=bs;
		    v=vs;
		    itIn=FALSE;
			for(ixx=0;ixx<=dxy;++ixx){ 
			    s1x=(double)ixx*d1x;
			    x=p1x+s1x*(p2x-p1x);
			    y=p1y+s1x*(p2y-p1y);
				ix=(int)(x+0.5);
				iy=(int)(y+0.5);
				if((ix >= 0 && ix < xsizen) && (iy >= 0 && iy < ysizen)){
				    z=p1z+s1x*(p2z-p1z);
					iy=(rastorY-iy-1);
					if(z < b[ix+iy*xsize]){
					    if(z <= rd->zbuff[ix+iy*xsize])continue;
						if(vdata && surf->flag & M_COLORLEVEL){
						    cvalue=value1+s1x*dvalue1;
						    colorit(surf->ColorCount,surf->ColorLevel,
						    cvalue,&ic);
						    ColorToVec(surf->ColorValue[ic],surf_color);
						}else{
							ColorToVec(surf->diffuse,surf_color);
						}
					    v[3*ix+iy*xsize3]=(float)(value*surf_color[0]);
					    v[3*ix+1+iy*xsize3]=(float)(value*surf_color[1]);
					    v[3*ix+2+iy*xsize3]=(float)(value*surf_color[2]);
						rd->cellbuff2[ix+iy*xsize]=CellCount+nn;
						rd->opacity[ix+iy*xsize]=(float)surf->opacity;
					    b[ix+iy*xsize]=z;
					}else{
					    ++over;
					}
					++count;
					itIn=TRUE;
				}else {
				    if(itIn)break;
				}
			}
	  
    }else{
	  bs=(double *)rd->zbuff;
	  vs=(float *)rd->vbuff;
	    b=bs;
	    v=vs;
	    itIn=FALSE;
		for(ixx=0;ixx<=dxy;++ixx){ 
		    s1x=(double)ixx*d1x;
		    x=p1x+s1x*(p2x-p1x);
		    y=p1y+s1x*(p2y-p1y);
			ix=(int)(x+0.5);
			iy=(int)(y+0.5);
			if((ix >= 0 && ix < xsizen) && (iy >= 0 && iy < ysizen)){
			    z=p1z+s1x*(p2z-p1z);
				iy=(rastorY-iy-1);
				if(z > b[ix+iy*xsize]){
					if(vdata && surf->flag & M_COLORLEVEL){
					    cvalue=value1+s1x*dvalue1;
					    colorit(surf->ColorCount,surf->ColorLevel,
					    cvalue,&ic);
					    ColorToVec(surf->ColorValue[ic],surf_color);
					}else{
						ColorToVec(surf->diffuse,surf_color);
					}
				    v[3*ix+iy*xsize3]=(float)(value*surf_color[0]);
				    v[3*ix+1+iy*xsize3]=(float)(value*surf_color[1]);
				    v[3*ix+2+iy*xsize3]=(float)(value*surf_color[2]);
					CellBuff[ix+iy*xsize]=CellCount+nn;
				    b[ix+iy*xsize]=z;
				}else{
				    ++over;
				}
				++count;
				itIn=TRUE;
			}else {
			    if(itIn)break;
			}
		}
	  
}
	 }
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	return 0;

}
int doVrmlTexture(ColorVEC surf_color,double s1x,double s2x,long ns,long ip,struct tState *tS)
{
	struct vrmlTextureCoordinate2 *t2;
	struct vrmlIndexedFaceSet *sx;
	struct vrmlTexture2 *tx;
	long nx,ny;
	long n1,n2,n3;
	unsigned long color;
	double s,t;

	if(!tS || !tS->s || !tS->t2 || !tS->t)return 1;
	sx=tS->s;
	t2=tS->t2;
	tx=tS->t;
	if(ip == 0){
	    n1=sx->textdata[ns];
	    n2=sx->textdata[ns+1];
	    n3=sx->textdata[ns+2];
	}else{
	    n1=sx->textdata[ns+2];
	    n2=sx->textdata[ns+3];
	    n3=sx->textdata[ns];
	}
	s=t2->pointdata[n1*2]+s1x*(t2->pointdata[n2*2]-t2->pointdata[n1*2])+
	                      s2x*(t2->pointdata[n3*2]-t2->pointdata[n1*2]);
	s=fmod(s,1.0); 
	if(s < 0.0)s += 1.0;
	nx=(long)(s*((double)tx->xsize-1));
	t=t2->pointdata[n1*2+1]+s1x*(t2->pointdata[n2*2+1]-t2->pointdata[n1*2+1])+
			       s2x*(t2->pointdata[n3*2+1]-t2->pointdata[n1*2+1]);
	t=fmod(t,1.0); 
	if(t < 0.0)t += 1.0;
	ny=(long)(t*((double)tx->ysize-1));
	color=tx->image[nx+ny*tx->xsize];
	surf_color[0]= ((double)((color >> 16) & 255))/256.f;
	surf_color[1]=((double)((color >> 8) & 255))/256.f;
	surf_color[2]=((double)(color & 255))/256.f;
	return 0;
}
int doNoShadow(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
               long nel,long CellCount,struct Scene *scene)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value,len;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over;
	Vec P,N,D,PV1,PV2,L;
	long xsize3,CellSkip;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt;
	double Ambient;
	long *CellBuff,b2;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;
	
	CellBuff=scene->CellBuff;
	
	Ambient=scene->g.Ambient;
	

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}
	Rastor8=(int)scene->Rastor8;
	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){
	    nt=4*nn;
	    if((k1=ndata[nt+1]) < 0)continue;
	    p1z=zdata[k1];
        if(p1z >= rd->h)continue;  

	    if((k2=ndata[nt+2]) < 0 )continue;
	    p2z=zdata[k2];
        if(p2z >=  rd->h)continue;  

	    if((k3=ndata[nt+3]) < 0)continue;
	    p3z=zdata[k3];
        if(p3z >=  rd->h)continue;  

	    ztran1 =  rd->h/( rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 =  rd->h/( rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    ztran3 =  rd->h/( rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

	    material=ndata[nt];
	    surf = &scene->material[material];

	    if(vdata){
			value1=vdata[k1];
	        dvalue1=vdata[k2]-value1;
	        dvalue2=vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	/* Centroid */

	    MakeVector(	(xdata[k1]+xdata[k2]+xdata[k3])/3.0,
	               	(ydata[k1]+ydata[k2]+ydata[k3])/3.0,
		       		(zdata[k1]+zdata[k2]+zdata[k3])/3.0,P);

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	/*  Normal Vector */

	    MakeVector(xdata[k2]-xdata[k1],ydata[k2]-ydata[k1],
	               zdata[k2]-zdata[k1],PV1);

	    MakeVector(xdata[k3]-xdata[k1],ydata[k3]-ydata[k1],
	               zdata[k3]-zdata[k1],PV2);

	    VecCross(PV1,PV2,N);
	    
	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

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
	         value=VecDot(N,L);
	     }else{
	         value=0.;
	     }

	    value  *= intensity;
	    if(value < Ambient)value=Ambient;
	    
	    if(ixmin < 0)ixmin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymin < 0)iymin=0;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;
    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			if(vdata && surf->flag & M_COLORLEVEL){
			    
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);			    
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth2;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			*v=(float)(value*surf_color[0]);
	        *(v+1)=(float)(value*surf_color[1]);
	        *(v+2)=(float)(value*surf_color[2]);
        	rd->cellbuff2[b2]=CellCount+nn;
        	rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) > *b){
		    
			if(vdata && surf->flag & M_COLORLEVEL){
			    
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);			    
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			*v=(float)(value*surf_color[0]);
	        *(v+1)=(float)(value*surf_color[1]);
	        *(v+2)=(float)(value*surf_color[2]);
        	CellBuff[b-rd->zbuff]=CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"TotalCells %ld CellSkip %ld\n",nel,CellSkip);
		WarningBatch(WarningBuff);
	}
	return 0;
}
int doNoShadow4(long *ndata,double *xdata,double *ydata,double *zdata,
                double *vdata,long nel,long CellCount,struct Scene *scene)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value,len;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over,ipass,ip;
	long n1,n2,n3,n4;
	Vec P,N,D,PV1,PV2,L;
	long xsize3;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
	double Ambient;
    long *CellBuff,b2;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;
	
	CellBuff=scene->CellBuff;

	
	Ambient=scene->g.Ambient;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}

	Rastor8=(int)scene->Rastor8;

	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
	  for(ip=0;ip<ipass;++ip){
	    if(ip == 0){
	        if(ipass == 1){
	            k1=n1;
	            k2=n2;
	            k3=n3;
	        }else{
	            k1=n1;
	            k2=n2;
	            k3=n4;
	        }
	    }else{
	        k1=n3;
	        k2=n4;
	        k3=n2;
	    }

	    p1z=zdata[k1];
        if(p1z >= rd->h)continue;  

	    p2z=zdata[k2];
        if(p2z >= rd->h)continue;  

	    p3z=zdata[k3];
        if(p3z >= rd->h)continue;  

	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    ztran3 = rd->h/(rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

	    material=ndata[nt];
	    surf = &scene->material[material];

	    if(vdata){
			value1=vdata[k1];
	        dvalue1=vdata[k2]-value1;
	        dvalue2=vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	/* Centroid */

	    MakeVector(	(xdata[k1]+xdata[k2]+xdata[k3])/3.0,
	               	(ydata[k1]+ydata[k2]+ydata[k3])/3.0,
		       		(zdata[k1]+zdata[k2]+zdata[k3])/3.0,P);

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	/*  Normal Vector */

	    MakeVector(xdata[k2]-xdata[k1],ydata[k2]-ydata[k1],
	               zdata[k2]-zdata[k1],PV1);

	    MakeVector(xdata[k3]-xdata[k1],ydata[k3]-ydata[k1],
	               zdata[k3]-zdata[k1],PV2);

	    VecCross(PV1,PV2,N);
	    
	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

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
	         value=VecDot(N,L);
	     }else{
	         value=0.;
	     }

	    value  *= intensity;
	    if(value < Ambient)value=Ambient;

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;
    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			if(vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
			    if(Rastor8){
					*v=(float)surf->diffuse.red;
					goto SetDepth2;
			    }else{
					ColorToVec(surf->diffuse,surf_color);
			    }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    if(ip == 0){
			        hit.f1=s1x;
			        hit.f2=s2x;
			    }else{
			        hit.f1=1.0f-s1x;
			        hit.f2=1.0f-s2x;
			    }
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}
		        *v=(float)(value*surf_color[0]);
		        *(v+1)=(float)(value*surf_color[1]);
		        *(v+2)=(float)(value*surf_color[2]);
	        	rd->cellbuff2[b2]=CellCount+nn;
	        	rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) > *b){
			if(vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
			    if(Rastor8){
					*v=(float)surf->diffuse.red;
					goto SetDepth;
			    }else{
					ColorToVec(surf->diffuse,surf_color);
			    }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    if(ip == 0){
			        hit.f1=s1x;
			        hit.f2=s2x;
			    }else{
			        hit.f1=1.0f-s1x;
			        hit.f2=1.0f-s2x;
			    }
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}
		        *v=(float)(value*surf_color[0]);
		        *(v+1)=(float)(value*surf_color[1]);
		        *(v+2)=(float)(value*surf_color[2]);
	        	CellBuff[b-rd->zbuff]=CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
}
	 }
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	return 0;

}
int doNoShadowG(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                double *gdata,long nel,long CellCount,struct Scene *scene)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over;
	long xsize3,CellSkip;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt;
	double Ambient;
	double g1,dg1,dg2;
	long *CellBuff,b2;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;
	
	CellBuff=scene->CellBuff;

	
	Ambient=scene->g.Ambient;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}
	Rastor8=(int)scene->Rastor8;
	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){
	    nt=4*nn;
	    if((k1=ndata[nt+1]) < 0)continue;
	    p1z=zdata[k1];
	    if(p1z >= rd->h)continue;  

	    if((k2=ndata[nt+2]) < 0 )continue;
	    p2z=zdata[k2];
        if(p2z >= rd->h)continue;  

	    if((k3=ndata[nt+3]) < 0)continue;
	    p3z=zdata[k3];
        if(p3z >= rd->h)continue;  

	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    ztran3 = rd->h/(rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

	    material=ndata[nt];
	    surf = &scene->material[material];

	    if(vdata){
			value1=vdata[k1];
	        dvalue1=vdata[k2]-value1;
	        dvalue2=vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	    g1=gdata[k1];
	    dg1=gdata[k2]-g1;
	    dg2=gdata[k3]-g1;
    
	    if(ixmin < 0)ixmin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymin < 0)iymin=0;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;
    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			if(vdata && surf->flag & M_COLORLEVEL){			    
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);			    
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth2;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
    		value=g1+s1x*dg1+s2x*dg2;

    		value  *= intensity;
    		if(value < Ambient)value=Ambient;

			*v=(float)(value*surf_color[0]);
	        *(v+1)=(float)(value*surf_color[1]);
	        *(v+2)=(float)(value*surf_color[2]);
	        rd->cellbuff2[b2]=CellCount+nn;
	        rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) > *b){
		    
			if(vdata && surf->flag & M_COLORLEVEL){			    
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);			    
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
    		value=g1+s1x*dg1+s2x*dg2;

    		value  *= intensity;
    		if(value < Ambient)value=Ambient;

			*v=(float)(value*surf_color[0]);
	        *(v+1)=(float)(value*surf_color[1]);
	        *(v+2)=(float)(value*surf_color[2]);
	        CellBuff[b-rd->zbuff]=CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"TotalCells %ld CellSkip %ld\n",nel,CellSkip);
		WarningBatch(WarningBuff);
	}

	return 0;
}
int doNoShadow4G(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 double *gdata,long nel,long CellCount,struct Scene *scene)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over,ipass,ip;
	long n1,n2,n3,n4;
	long xsize3;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
	double Ambient;
	double g1,dg1,dg2;
    long *CellBuff,b2;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;
	
	CellBuff=scene->CellBuff;

	
	Ambient=scene->g.Ambient;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}

	Rastor8=(int)scene->Rastor8;

	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
	  for(ip=0;ip<ipass;++ip){
	    if(ip == 0){
	        if(ipass == 1){
	            k1=n1;
	            k2=n2;
	            k3=n3;
	        }else{
	            k1=n1;
	            k2=n2;
	            k3=n4;
	        }
	    }else{
	        k1=n3;
	        k2=n4;
	        k3=n2;
	    }
	    p1z=zdata[k1];
        if(p1z >= rd->h)continue;  

	    p2z=zdata[k2];
        if(p2z >= rd->h)continue;  

	    p3z=zdata[k3];
        if(p3z >= rd->h)continue;  

	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    ztran3 = rd->h/(rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

	    material=ndata[nt];
	    surf = &scene->material[material];

	    if(vdata){
			value1=vdata[k1];
	        dvalue1=vdata[k2]-value1;
	        dvalue2=vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }
	    
	    g1=gdata[k1];
        dg1=gdata[k2]-g1;
        dg2=gdata[k3]-g1;

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			if(vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth2;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    if(ip == 0){
			        hit.f1=s1x;
			        hit.f2=s2x;
			    }else{
			        hit.f1=1.0f-s1x;
			        hit.f2=1.0f-s2x;
			    }
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}
    		value=g1+s1x*dg1+s2x*dg2;

    		value  *= intensity;
    		if(value < Ambient)value=Ambient;
	        *v=(float)(value*surf_color[0]);
	        *(v+1)=(float)(value*surf_color[1]);
	        *(v+2)=(float)(value*surf_color[2]);
	        rd->cellbuff2[b2]=CellCount+nn;
	        rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) > *b){
			if(vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    if(ip == 0){
			        hit.f1=s1x;
			        hit.f2=s2x;
			    }else{
			        hit.f1=1.0f-s1x;
			        hit.f2=1.0f-s2x;
			    }
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}
    		value=g1+s1x*dg1+s2x*dg2;

    		value  *= intensity;
    		if(value < Ambient)value=Ambient;
	        *v=(float)(value*surf_color[0]);
	        *(v+1)=(float)(value*surf_color[1]);
	        *(v+2)=(float)(value*surf_color[2]);
	        CellBuff[b-rd->zbuff]=CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	 }
	    
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	return 0;
}
int doNoShadowP(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                float *nvdata,long nel,long CellCount,struct Scene *scene)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over;
	long xsize3,CellSkip;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt;
	long *CellBuff,b2;
	Vec N,P,L,D;
	double nx1,ndx1,ndx2,ny1,ndy1,ndy2,nz1,ndz1,ndz2;
	double x1,dx1,dx2,y1,dy1,dy2,z1,dz1,dz2;
	double r1,dr1,dr2,s1,ds1,ds2;
	double len;
	double spec,colr,colg,colb;
	float *rdata;
	float *sdata;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;

	rdata=scene->rdata;
	sdata=scene->sdata;

	CellBuff=scene->CellBuff;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}
	Rastor8=(int)scene->Rastor8;
	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){
/*	
	    if(nn == 544 || nn == 1416 || nn == 1388 || nn == 1528 || nn == 1389 ||
	    nn == 1418 || nn == 1417 || nn ==1386 || nn == 1387 ||
	    nn == 543 || nn == 1425 || nn ==1440)continue;
*/
	    nt=4*nn;
	    if((k1=ndata[nt+1]) < 0)continue;
	    p1z=zdata[k1];
	    if(p1z >= rd->h)continue;  

	    if((k2=ndata[nt+2]) < 0 )continue;
	    p2z=zdata[k2];
        if(p2z >= rd->h)continue;  

	    if((k3=ndata[nt+3]) < 0)continue;
	    p3z=zdata[k3];
        if(p3z >= rd->h)continue;  

	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    ztran3 = rd->h/(rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

	    material=ndata[nt];
	    surf = &scene->material[material];

	    if(vdata){
			value1=vdata[k1];
	        dvalue1=vdata[k2]-value1;
	        dvalue2=vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	    if(rdata){
			r1=rdata[k1];
	        dr1=rdata[k2]-r1;
	        dr2=rdata[k3]-r1;
        }else{
			r1=0;
	        dr1=0;
			dr2=0;
	    }
	    if(sdata){
			s1=sdata[k1];
	        ds1=sdata[k2]-s1;
	        ds2=sdata[k3]-s1;
        }else{
			s1=0;
	        ds1=0;
			ds2=0;
	    }

	    nx1=nvdata[k1*3];
	    ndx1=nvdata[k2*3]-nx1;
	    ndx2=nvdata[k3*3]-nx1;

	    ny1=nvdata[k1*3+1];
	    ndy1=nvdata[k2*3+1]-ny1;
	    ndy2=nvdata[k3*3+1]-ny1;

 	    nz1=nvdata[k1*3+2];
	    ndz1=nvdata[k2*3+2]-nz1;
	    ndz2=nvdata[k3*3+2]-nz1;

 	    x1=xdata[k1];
	    dx1=xdata[k2]-x1;
	    dx2=xdata[k3]-x1;

	    y1=ydata[k1];
	    dy1=ydata[k2]-y1;
	    dy2=ydata[k3]-y1;

 	    z1=zdata[k1];
	    dz1=zdata[k2]-z1;
	    dz2=zdata[k3]-z1;
   
  
	    if(ixmin < 0)ixmin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymin < 0)iymin=0;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;
    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
				   nz1+s1x*ndz1+s2x*ndz2,N);
			if(vdata && surf->flag & M_COLORLEVEL){			    
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);			    
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
			    if(Rastor8){
		              *v=(float)surf->diffuse.red;
		              goto SetDepth2;
			    }else{
		              ColorToVec(surf->diffuse,surf_color);
			    }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			     hit.f1=r1+s1x*dr1+s2x*dr2;
			     hit.f2=s1+s1x*ds1+s2x*ds2;

			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong)) ;
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
			*v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=CellCount+nn;
	        rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    
	    
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) > *b){
			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
				   nz1+s1x*ndz1+s2x*ndz2,N);
		    
			if(vdata && surf->flag & M_COLORLEVEL){			    
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,cvalue,&ic);			    
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}

			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+s1x*dr1+s2x*dr2;
			    hit.f2=s1+s1x*ds1+s2x*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}
/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }

    		value  *= intensity;
			*v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"TotalCells %ld CellSkip %ld\n",nel,CellSkip);
		WarningBatch(WarningBuff);
	}

	return 0;
}
int doNoShadow4P(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over,ipass,ip;
	long n1,n2,n3,n4;
	long xsize3;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
    long *CellBuff,b2;
	Vec N,P,L,D;
	double nx1,ndx1,ndx2,ny1,ndy1,ndy2,nz1,ndz1,ndz2;
	double x1,dx1,dx2,y1,dy1,dy2,z1,dz1,dz2;
	double r1,dr1,dr2,s1,ds1,ds2;
	double len;
	double spec,colr,colg,colb;
	float *rdata;
	float *sdata;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;

	rdata=scene->rdata;
	sdata=scene->sdata;

	CellBuff=scene->CellBuff;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}


	Rastor8=(int)scene->Rastor8;

	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	for(nn=0;nn<nel;++nn){	    
	    nt=5*nn;
	    if((n1=ndata[nt+1]) < 0)continue;
	    if((n2=ndata[nt+2]) < 0 )continue;
	    if((n3=ndata[nt+3]) < 0)continue;
	    if((n4=ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
	  for(ip=0;ip<ipass;++ip){
	    if(ip == 0){
	        if(ipass == 1){
	            k1=n1;
	            k2=n2;
	            k3=n3;
	        }else{
	            k1=n1;
	            k2=n2;
	            k3=n4;
	        }
	    }else{
	        k1=n3;
	        k2=n4;
	        k3=n2;
	    }
	    
	    p1z=zdata[k1];
        if(p1z >= rd->h)continue;  

	    p2z=zdata[k2];
        if(p2z >= rd->h)continue;  

	    p3z=zdata[k3];
        if(p3z >= rd->h)continue;  
        
	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;
	    
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    ztran3 = rd->h/(rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;

	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

	    material=ndata[nt];
	    surf = &scene->material[material];
	    if(vdata){
			value1=vdata[k1];
	        dvalue1=vdata[k2]-value1;
	        dvalue2=vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	    if(rdata){
			r1=rdata[k1];
	        dr1=rdata[k2]-r1;
	        dr2=rdata[k3]-r1;
        }else{
			r1=0;
	        dr1=0;
			dr2=0;
	    }
	    if(sdata){
			s1=sdata[k1];
	        ds1=sdata[k2]-s1;
	        ds2=sdata[k3]-s1;
        }else{
			s1=0;
	        ds1=0;
			ds2=0;
	    }
	    
	    nx1=nvdata[k1*3];
	    ndx1=nvdata[k2*3]-nx1;
	    ndx2=nvdata[k3*3]-nx1;

	    ny1=nvdata[k1*3+1];
	    ndy1=nvdata[k2*3+1]-ny1;
	    ndy2=nvdata[k3*3+1]-ny1;

 	    nz1=nvdata[k1*3+2];
	    ndz1=nvdata[k2*3+2]-nz1;
	    ndz2=nvdata[k3*3+2]-nz1;

 	    x1=xdata[k1];
	    dx1=xdata[k2]-x1;
	    dx2=xdata[k3]-x1;

	    y1=ydata[k1];
	    dy1=ydata[k2]-y1;
	    dy2=ydata[k3]-y1;

 	    z1=zdata[k1];
	    dz1=zdata[k2]-z1;
	    dz2=zdata[k3]-z1;

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
				   nz1+s1x*ndz1+s2x*ndz2,N);
				   
			if(vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth2;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+s1x*dr1+s2x*dr2;
			    hit.f2=s1+s1x*ds1+s2x*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0f;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=CellCount+nn;
	        rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
	           
		    if((z = tj+s1x*tk+s2x*tl) > *b){
			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
				   nz1+s1x*ndz1+s2x*ndz2,N);
				   
			if(vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+s1x*dvalue1+s2x*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+s1x*dr1+s2x*dr2;
			    hit.f2=s1+s1x*ds1+s2x*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	 }
	    
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	return 0;
}
int doNoShadowVRML(long *ndata,long *matdata,float *normal,long normmax,
		 long *normlist,long normlistmax,
                 double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od,tj,tk,tl;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over,ipass,ip;
	long n1,n2,n3,n4;
	long xsize3;
	int rastorY;
	
	struct Material surf;
	ColorVEC	surf_color,specular,ambient;
	long CellSkip;
        long *CellBuff,b2;
	Vec N,P,L,D;
	double nx1,ndx1,ndx2,ny1,ndy1,ndy2,nz1,ndz1,ndz2;
	double x1,dx1,dx2,y1,dy1,dy2,z1,dz1,dz2;
	double len;
	double spec,colr,colg,colb;
	long ns,ne,nc,ncell,mat;
	int MaterialType;
	struct vrmlMaterial *vrmlMat;
	long SkipPoly;
	int warn;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;

	warn=0;

	vdata=vdata;

	normmax=normmax;

	ambient[0]=ambient[1]=ambient[2]=0;
	specular[0]=specular[1]=specular[2]=0;

	CellBuff=scene->CellBuff;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}

	if(!tS)return 1;
	

	zerol((char *)&surf,sizeof(struct Material));
	surf.opacity=tS->opacity;

	MaterialType=tS->MaterialType;
	vrmlMat=tS->m;
	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	ncell=0;
	SkipPoly=0;
	for(nn=0;nn<nel;++nn,++ncell){	    
	    ns=nn;
	    ne=nn;
	    while(ndata[ne] >= 0 && ne < nel)++ne;
	    nn=ne;
	    nc=ne-ns;
	    if(nc < 3){
	        SkipPoly++;
	        continue;
	    }else if( nc > 4){
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
	  /*
	    double xx1,xx2,xx3,yy1,yy2,yy3;
	  */
	    if(ip == 0){
	        k1=n1;
	        k2=n2;
	        k3=n3;
	    }else{
	        k1=n3;
	        k2=n4;
	        k3=n1;
	    }

	    p1z=zdata[k1];
        if(p1z >= rd->h)continue;
	    ztran1 = rd->h/(rd->h-p1z);
	    p1x=(rd->o+ztran1*(xdata[k1]-rd->o)-rd->xmin)*rd->odx;

	    p2z=zdata[k2];
        if(p2z >= rd->h)continue;
	    ztran2 = rd->h/(rd->h-p2z);
	    p2x=(rd->o+ztran2*(xdata[k2]-rd->o)-rd->xmin)*rd->odx;

	    p3z=zdata[k3];
        if(p3z >= rd->h)continue;
	    ztran3 = rd->h/(rd->h-p3z);
	    p3x=(rd->o+ztran3*(xdata[k3]-rd->o)-rd->xmin)*rd->odx;
/*
		xx1=xdata[k1];
		xx2=xdata[k2];
		xx3=xdata[k3];
		
		yy1=ydata[k1];
		yy2=ydata[k2];
		yy3=ydata[k3];
 */
	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }

	    p1y=(ztran1*ydata[k1]-rd->ymin)*rd->ody;
	    p2y=(ztran2*ydata[k2]-rd->ymin)*rd->ody;
	    p3y=(ztran3*ydata[k3]-rd->ymin)*rd->ody;

	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	        ++CellSkip;
	        continue;
	    }

	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

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
	        nx1=normal[kn1*3];
	        ndx1=normal[kn2*3]-nx1;
	        ndx2=normal[kn3*3]-nx1;

	        ny1=normal[kn1*3+1];
	        ndy1=normal[kn2*3+1]-ny1;
	        ndy2=normal[kn3*3+1]-ny1;

 	        nz1=normal[kn1*3+2];
	        ndz1=normal[kn2*3+2]-nz1;
	        ndz2=normal[kn3*3+2]-nz1;
	    }else{

	        nx1=nvdata[k1*3];
	        ndx1=nvdata[k2*3]-nx1;
	        ndx2=nvdata[k3*3]-nx1;

	        ny1=nvdata[k1*3+1];
	        ndy1=nvdata[k2*3+1]-ny1;
	        ndy2=nvdata[k3*3+1]-ny1;

 	        nz1=nvdata[k1*3+2];
	        ndz1=nvdata[k2*3+2]-nz1;
	        ndz2=nvdata[k3*3+2]-nz1;
	    }

 	    x1=xdata[k1];
	    dx1=xdata[k2]-x1;
	    dx2=xdata[k3]-x1;

	    y1=ydata[k1];
	    dy1=ydata[k2]-y1;
	    dy2=ydata[k3]-y1;

 	    z1=zdata[k1];
	    dz1=zdata[k2]-z1;
	    dz2=zdata[k3]-z1;

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
		    
			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
				   nz1+s1x*ndz1+s2x*ndz2,N);
				   
		        
			if(MaterialType == VRML_BY_FACE){
			    mat=matdata[ncell];
			    if(vrmlMat){
			        if(mat >= vrmlMat->max)mat=0;
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;
			    if(ip == 0){
			        m[0]=matdata[ns];
			        m[1]=matdata[ns+1];
			        m[2]=matdata[ns+2];
			    }else{
			        m[0]=matdata[ns+2];
			        m[1]=matdata[ns+3];
			        m[2]=matdata[ns];
			    }

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                            s2x*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                            s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                            s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                            s2x*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                            s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                            s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                            s2x*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                            s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                            s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
			   if(warn++ < 2){
			        sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
			    }
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}

			if(tS->flagTexture)doVrmlTexture(surf_color,s1x,s2x,ns,ip,tS);

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong) ;
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);
	        }


    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=CellCount+nn;
	        rd->opacity[b2]=(float)(surf.opacity);
		    *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
/*    
	    	s1x=((ixx-p1x)*v2y-(iyy-p1y)*v2x)*od;
	    	s2x=(v1x*(iyy-p1y)-v1y*(ixx-p1x))*od;
*/    
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
/*	           
			    {
					double a,b,c,d,e,f,den,xx,yy;

					xx=ixx/rd->odx+rd->xmin;
					yy=iyy/rd->ody+rd->ymin;
					a=(xx2-xx1)+xx*(p2z-p1z)/rd->h;
					b=(xx3-xx1)+xx*(p3z-p1z)/rd->h;
					d=(yy2-yy1)+yy*(p2z-p1z)/rd->h;
					e=(yy3-yy1)+yy*(p3z-p1z)/rd->h;
					if(!(den=a*e-d*b))continue;
					c= -xx1+xx*(rd->h-p1z)/rd->h;
					f= -yy1+yy*(rd->h-p1z)/rd->h;
					s1x=(c*e-f*b)/den;
					s2x=(a*f-d*c)/den;

		    }
*/	           
	           
	           
		    if((z = tj+s1x*tk+s2x*tl) > *b){
		    
			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
				   nz1+s1x*ndz1+s2x*ndz2,N);
				   
			if(MaterialType == VRML_BY_FACE){
			    mat=matdata[ncell];
			    if(vrmlMat){
			        if(mat >= vrmlMat->max)mat=0;
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;
			    if(ip == 0){
			        m[0]=matdata[ns];
			        m[1]=matdata[ns+1];
			        m[2]=matdata[ns+2];
			    }else{
			        m[0]=matdata[ns+2];
			        m[1]=matdata[ns+3];
			        m[2]=matdata[ns];
			    }

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                                s2x*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                  s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                  s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                              s2x*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                             s2x*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                               s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                               s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
				if(warn++ < 3){
				 	sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
				}
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}
			if(tS->flagTexture)doVrmlTexture(surf_color,s1x,s2x,ns,ip,tS);

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong);			
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);		        
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){	 
	        
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);	
	        
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=CellCount+nn;
		    *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	 }
	continue;
doPolyHere: /* nc */

	    n1=ndata[ns];
	    n2=ndata[ns+1];
	    n3=ndata[ns+2];
	    {
	       Vec v1,v2;
	       double z;
	       long n,np;

		MakeVector(xdata[n2]-xdata[n1],ydata[n2]-ydata[n1],
			   zdata[n2]-zdata[n1],v1);
		MakeVector(xdata[n3]-xdata[n1],ydata[n3]-ydata[n1],
			   zdata[n3]-zdata[n1],v2);
		VecCross(v1,v2,N);
		VecNormalize(N);

		if(fabs(N[2]) < 1.e-4)continue;
	    
		ixmax = -100000;
		ixmin = 100000;
		iymax = -100000;
		iymin = 100000;
		
		 p1x=p2x=p3x=0;
		 p1y=p2y=p3y=0;
		 p1z=p2z=p3z=0;
		 k1=k2=k3=0;
		

		for(n=0;n<nc;++n){
		    double px,py;

		    np=ndata[ns+n];
		    if((z=zdata[np]) >= rd->h)goto EndOut;
		    ztran1 = rd->h/(rd->h-z);

		    px=(rd->o+ztran1*(xdata[np]-rd->o)-rd->xmin)*rd->odx;
		    ix = (int)px;
		    if(ix > ixmax)ixmax = ix;
		        
		    if(ix < ixmin)ixmin = ix;

		    py=(ztran1*ydata[np]-rd->ymin)*rd->ody;	    	    	    
		    iy = (int)py;
		    if(iy > iymax)iymax = iy;
		        
		    if(iy < iymin)iymin = iy;
		        
		    if(n == 0){
		       p1x=px;
		       p1y=py;
		       p1z=z;
		       k1=np;
		    }else if(n == 1){
		       p2x=px;
		       p2y=py;
		       p2z=z;
		       k2=np;
		    }else if(n == 2){
		       p3x=px;
		       p3y=py;
		       p3z=z;
		       k3=np;
		    }

		}
	        if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	            ++CellSkip;
	            continue;
	        }
	        if((iymax == iymin) || (iymax < 0) || (iymin > ysizen)){
	            ++CellSkip;
	            continue;
	        }
	    }
	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;

 	    x1=xdata[k1];
	    dx1=xdata[k2]-x1;
	    dx2=xdata[k3]-x1;

	    y1=ydata[k1];
	    dy1=ydata[k2]-y1;
	    dy2=ydata[k3]-y1;

 	    z1=zdata[k1];
	    dz1=zdata[k2]-z1;
	    dz2=zdata[k3]-z1;

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

	    tj=p1z;
	    tk=p2z-p1z;
	    tl=p3z-p1z;

	    if((tS->NormalType == VRML_BY_VERTEX) && (ns < normlistmax)){
	        long kn1,kn2,kn3;

			kn1=normlist[ns];
			kn2=normlist[ns+1];
			kn3=normlist[ns+2];

	        nx1=normal[kn1*3];
	        ndx1=normal[kn2*3]-nx1;
	        ndx2=normal[kn3*3]-nx1;

	        ny1=normal[kn1*3+1];
	        ndy1=normal[kn2*3+1]-ny1;
	        ndy2=normal[kn3*3+1]-ny1;

 	        nz1=normal[kn1*3+2];
	        ndz1=normal[kn2*3+2]-nz1;
	        ndz2=normal[kn3*3+2]-nz1;
	    }else{
	        nx1=nvdata[k1*3];
	        ndx1=nvdata[k2*3]-nx1;
	        ndx2=nvdata[k3*3]-nx1;

	        ny1=nvdata[k1*3+1];
	        ndy1=nvdata[k2*3+1]-ny1;
	        ndy2=nvdata[k3*3+1]-ny1;

 	        nz1=nvdata[k1*3+2];
	        ndz1=nvdata[k2*3+2]-nz1;
	        ndz2=nvdata[k3*3+2]-nz1;

	    }
	    

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        {
		    if((z = tj+s1x*tk+s2x*tl) < *b){
		        {   
		            double x,y;
		            long np0,np1,npp0,npp1;
		            int l;
		            long np;

			    x=(double)(p1x+s1x*(p2x-p1x)+s2x*(p3x-p1x));	
			    y=(double)(p1y+s1x*(p2y-p1y)+s2x*(p3y-p1y));
			    np1=ns;
			    np0=ns+nc-1;
			    np=nc;

			    l = 0 ;	
			    while (np--) {
			        npp0=ndata[np0];
			        npp1=ndata[np1];
					if(((int)(ydata[npp0] > y) ^ (int)(ydata[npp1] > y))){
					    if(xdata[npp0] > x && xdata[npp1] > x){
					    	++l;
					    }else if (xdata[npp0] > x || xdata[npp1] > x){
				            l += (int)(ydata[npp1] > ydata[npp0]) ^
				                 (int)((ydata[npp0] - y)*(xdata[npp1] - x) >
				                       (xdata[npp0] - x)*(ydata[npp1] - y));
					    }
					}
					np0  = np1;
					++np1;
			    }
			    if ((l % 2) == 0)continue;
		        }
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
			if(MaterialType == VRML_BY_FACE){
			    mat=matdata[ncell];
			    if(vrmlMat && (mat < vrmlMat->max)){
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;


			    m[0]=matdata[ns];
			    m[1]=matdata[ns+1];
			    m[2]=matdata[ns+2];

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                                s2x*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                  s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                  s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                              s2x*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                             s2x*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                               s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                               s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
			   if(warn++ < 2){
			        sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
			   }
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}

			MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
				   z1+s1x*dz1+s2x*dz2,P);

		    MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
			       nz1+s1x*ndz1+s2x*ndz2,N);
/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong) ;
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);
	        }


    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=CellCount+nn;
	        rd->opacity[b2]=(float)(surf.opacity);
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        {
		    if((z = tj+s1x*tk+s2x*tl) > *b){
		        {   
		            double x,y;
		            long np0,np1,npp0,npp1;
		            int l;
		            long np;

			    x=(double)(x1+s1x*dx1+s2x*dx2);	
			    y=(double)(y1+s1x*dy1+s2x*dy2);
			    np1=ns;
			    np0=ns+nc-1;

			    np=nc;

			    l = 0 ;	
			    while (np--) {
			        npp0=ndata[np0];
			        npp1=ndata[np1];
					if((int)((int)(ydata[npp0] > y) ^ (int)(ydata[npp1] > y))){
					    if(xdata[npp0] > x && xdata[npp1] > x){
				            ++l;
					    }else if (xdata[npp0] > x || xdata[npp1] > x){
				            l += (int)(ydata[npp1] > ydata[npp0]) ^
				                 (int)((ydata[npp0] - y)*(xdata[npp1] - x) >
				                       (xdata[npp0] - x)*(ydata[npp1] - y));
					    }
					}
					np0  = np1;
					++np1;
			    }
			    if ((l % 2) == 0)continue;
		        }
			if(MaterialType == VRML_BY_FACE){
			    mat=matdata[ncell];
			    if(vrmlMat && (mat < vrmlMat->max)){
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;


			    m[0]=matdata[ns];
			    m[1]=matdata[ns+1];
			    m[2]=matdata[ns+2];

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                                s2x*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                  s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                  s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                              s2x*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+s1x*(d[m[1]*3]-d[m[0]*3])+
			                             s2x*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+s1x*(d[m[1]*3+1]-d[m[0]*3+1])+
			                               s2x*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+s1x*(d[m[1]*3+2]-d[m[0]*3+2])+
			                               s2x*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
			    if(warn++ < 3){
			        sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
			    }
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}
		MakeVector(x1+s1x*dx1+s2x*dx2,y1+s1x*dy1+s2x*dy2,
			   z1+s1x*dz1+s2x*dz2,P);
		MakeVector(nx1+s1x*ndx1+s2x*ndx2,ny1+s1x*ndy1+s2x*ndy2,
			   nz1+s1x*ndz1+s2x*ndz2,N);

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong);			
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);		        
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){	 
	        
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);	
	        
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=CellCount+nn;
		    *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
	        }
	    }
	  }
    }
 
EndOut:
	     ;
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	if(SkipPoly && scene->debug > 2){
	    sprintf(WarningBuff,"Polygone skipped %ld\n",SkipPoly);
		WarningBatch(WarningBuff);
	}
	return 0;
}
int SetShadow(struct Scene *scene)
{
	struct P LampToTarget,V1,V2,V3;

	if(!scene)return 1;
		
	scene->g.Lamp=scene->lamp.origin;
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Lamp %f %f %f\n",scene->g.Lamp.x,scene->g.Lamp.y,scene->g.Lamp.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"Target %f %f %f\n",scene->Target.x,scene->Target.y,scene->Target.z);
		WarningBatch(WarningBuff);
	}
	LampToTarget=Sub(&scene->Target,&scene->g.Lamp);
	LampToTarget=Norm(&LampToTarget);
	
	    /* Local x Direction */
	V1=CrossN(&LampToTarget,&scene->Up);

	
	    /* Local y Direction */
	V2=CrossN(&V1,&LampToTarget);
	
	    /* Local z Direction */
	V3=Mult(&LampToTarget,-1.0);
	
   if(scene->debug > 2){
	    sprintf(WarningBuff,"LampToTarget %f %f %f\n",LampToTarget.x,LampToTarget.y,LampToTarget.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"S1 %f %f %f\n",V1.x,V1.y,V1.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"S2 %f %f %f\n",V2.x,V2.y,V2.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"S3 %f %f %f\n",V3.x,V3.y,V3.z);
		WarningBatch(WarningBuff);
	}
	scene->g.S1=V1;
	scene->g.S2=V2;
	scene->g.S3=V3;
	return 0;
}
int SetScreen(struct Scene *scene)
{
	struct P EyeToTarget,V1,V2,V3,V4,pScreen;
	
	if(!scene)return 1;
	
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
	if(scene->debug >2){
	    sprintf(WarningBuff,"V1 %f %f %f\n",V1.x,V1.y,V1.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"V2 %f %f %f\n",V2.x,V2.y,V2.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"V3 %f %f %f\n",V3.x,V3.y,V3.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"pScreen %f %f %f\n",pScreen.x,pScreen.y,pScreen.z);
		WarningBatch(WarningBuff);
	}
   	scene->g.V1=V1;
   	scene->g.V2=V2;
   	scene->g.V3=V3;
   	scene->g.pScreen=pScreen;
   	
   	V4=Mult(&V1,0.5*scene->EyeSeparation);
   	scene->EyeRight=Add(&V4,&scene->Eye);
   	
   	V4=Mult(&V1,-0.5*scene->EyeSeparation);
   	scene->EyeLeft=Add(&V4,&scene->Eye);
	if(scene->debug > 2){
	    sprintf(WarningBuff,"EyeRight %g %g %g\n",scene->EyeRight.x,scene->EyeRight.y,scene->EyeRight.z);
		WarningBatch(WarningBuff);
	    sprintf(WarningBuff,"EyeLeft %g %g %g\n",scene->EyeLeft.x,scene->EyeLeft.y,scene->EyeLeft.z);
		WarningBatch(WarningBuff);
	}   	 	
   	
	return 0;
}
int SetClipPlane(struct Scene *scene)
{
	struct P V1,V2,V3,pScreen;
	
	if(!scene)return 1;

   	V1=scene->g.V1;
   	V2=scene->g.V2;
   	V3=scene->g.V3;
   	pScreen=scene->g.pScreen;


	if(scene->ClipPlane == ClipPlane_Eye){
	     struct P ClipPlanePoint;
	     
	     ClipPlanePoint.x=scene->Eye.x-scene->ClipPlaneDistance*V3.x;
	     ClipPlanePoint.y=scene->Eye.y-scene->ClipPlaneDistance*V3.y;
	     ClipPlanePoint.z=scene->Eye.z-scene->ClipPlaneDistance*V3.z;

		MakeVector((ClipPlanePoint.x-pScreen.x)*V1.x+(ClipPlanePoint.y-pScreen.y)*V1.y+(ClipPlanePoint.z-pScreen.z)*V1.z,
			       (ClipPlanePoint.x-pScreen.x)*V2.x+(ClipPlanePoint.y-pScreen.y)*V2.y+(ClipPlanePoint.z-pScreen.z)*V2.z,
			       (ClipPlanePoint.x-pScreen.x)*V3.x+(ClipPlanePoint.y-pScreen.y)*V3.y+(ClipPlanePoint.z-pScreen.z)*V3.z,rd->ClipPlanePoint);
			       

	     rd->ClipPlaneNormal[0]=0.0;
	     rd->ClipPlaneNormal[1]=0.0;
	     rd->ClipPlaneNormal[2]=1.0;
	     
	}else{
			
		MakeVector((scene->ClipPlanePoint.x-pScreen.x)*V1.x+(scene->ClipPlanePoint.y-pScreen.y)*V1.y+(scene->ClipPlanePoint.z-pScreen.z)*V1.z,
			       (scene->ClipPlanePoint.x-pScreen.x)*V2.x+(scene->ClipPlanePoint.y-pScreen.y)*V2.y+(scene->ClipPlanePoint.z-pScreen.z)*V2.z,
			       (scene->ClipPlanePoint.x-pScreen.x)*V3.x+(scene->ClipPlanePoint.y-pScreen.y)*V3.y+(scene->ClipPlanePoint.z-pScreen.z)*V3.z,rd->ClipPlanePoint);

		
		MakeVector((scene->ClipPlaneNormal.x)*V1.x+(scene->ClipPlaneNormal.y)*V1.y+(scene->ClipPlaneNormal.z)*V1.z,
			       (scene->ClipPlaneNormal.x)*V2.x+(scene->ClipPlaneNormal.y)*V2.y+(scene->ClipPlaneNormal.z)*V2.z,
			       (scene->ClipPlaneNormal.x)*V3.x+(scene->ClipPlaneNormal.y)*V3.y+(scene->ClipPlaneNormal.z)*V3.z,rd->ClipPlaneNormal);
	}
	
	return 0;

}
int SetMaterialStuff(struct Scene *scene)
{
	struct Material *m;
	long k,flag;

	if(!scene)return 1;

	for(k=0;k<scene->MaterialCount;++k){
	    flag=0;
	    m = &scene->material[k];
	    if(m->diffuse.red || m->diffuse.blue || m->diffuse.green){
	        flag |= M_DIFFUSE;
	    }
	    if(m->ambient.red || m->ambient.blue || m->ambient.green){
	        flag |= M_AMBIENT;
	    }
	    if(m->mirror.red || m->mirror.blue || m->mirror.green){
     		if(!m->mirrorfactor)m->mirrorfactor=1;
	        flag |= M_MIRROR;
	    }
	    if(m->transmit.red || m->transmit.blue || m->transmit.green){
     		if(!m->transmitfactor)m->transmitfactor=1;
	        flag |= M_TRANSMIT;
	    }
	    if(m->index){
	        flag |= M_INDEX;
	    }
	    if(m->specular){
	        flag |= M_SPECULAR;
	    }
	    if(m->ColorCount){
	        flag |= M_COLORLEVEL;
	    }
	    
	    if(m->transparent.red || m->transparent.blue || m->transparent.green){
	        flag |= M_TRANSPARENT;
	    }
	    
	    if(m->PatternType==PATTERN_STRIPES){
	    	StartStripes((struct Stripes *)m->PatternData,scene->time);
	    	m->PatternRoutine=(long (*)(void *, void *, void *))doStripes;
	    	flag |= M_PATTERN;
	    }else if(m->PatternType==PATTERN_IMAGE){
	    	if(!(StartImages(&m->ImageData,scene->time))){
	    	    m->PatternRoutine=(long (*)(void *, void *, void *))doImages;
	    	    flag |= M_PATTERN;
	    	}
	    
	    }
	    
	    if(m->BumpType==BUMP_BUMPS){
	    	StartBumps((struct Bump *)m->BumpData,scene->time);
	    	m->BumpRoutine=(long (*)(void *, void *, void *))doBumps;
	    	flag |= M_BUMP;
	    }
	    
	    m->flag=flag;
	}
	return 0;
}
int StartBumps(struct Bump *b,long time)
{
	long k,nn,nx,ny,i,j,ii,jj;
	
	if(!b)return 1;
	
	time=time;
	nx=b->xCount;
	ny=b->yCount;
	nn=nx*ny;
	
	b->xRange[0]=0.;
	b->yRange[0]=0.;
	b->xRange[nx-1]=1.;
	b->yRange[ny-1]=1.;
	for(k=0;k<nn;++k){
	    b->bNormal[k]=Norm(&(b->bNormal[k]));
	}
	nn=0;
	for(j=0;j<ny;++j){
	    jj=j;
	    if(jj+1 >= ny && jj > 0)--jj;
	    for(i=0;i<nx;++i){
	        ii=i;
	        if(ii+1 >= nx && ii > 0)--ii;
	        b->cFlags[nn]=ii+nx*jj;
	        b->cFlags[nn+1]=ii+1+nx*jj;
	        b->cFlags[nn+2]=ii+1+nx*(jj+1);
	        b->cFlags[nn+3]=ii+nx*(jj+1);
	        nn += 4;
	    }
	}

	return 0;
}
int StartImages(struct Image *s,long time)
{	

	if(!s || !s->Files)return 1;


	if(s->ItWas == time)return 0;

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    time = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)time = time % s->tmax;
	}else if(s->cycleFlag == 2){
	    return 0;
	}

	if(FilesSetFrame(time,s->Files))return 1;

	return 0;
	
}
int StartStripes(struct Stripes *s,long time)
{
	long k,nn,nx,ny;

	if(!s)return 1;
	
	time=time;
	nx=s->xCount;
	ny=s->yCount;
	nn=nx*ny;
	
	s->xRange[0]=0.;
	s->yRange[0]=0.;
	s->xRange[nx-1]=1.;
	s->yRange[ny-1]=1.;
	for(k=0;k<nn;++k){
	    if(s->cColor[k].red || s->cColor[k].green || s->cColor[k].blue){
	        s->cFlags[k]=1;
	    }else{
	        s->cFlags[k]=0;
	    }
	}
	return 0;
}
long doBumps(void *v1,void *v2,void *v3)
{
	long n1,n2,n3,n4;
	int ic;
	Flt f1,f2,s1,s2,c1,c2,c3,c4;
 	Vec NL,NS;
	long nn;
	struct Bump *b;
	Isect * hit;
	Flt *N;

	b=(struct Bump *)v1;
	hit=(Isect *)v2;
	N=(Flt *)v3;

	f1=fmod(hit->f1*b->xRepeat,1.0);
	colorit(b->xCount,b->xRange,f1,&ic);
	n1=ic;
	if(n1+1 >= b->xCount){
	    s1=1;
	}else{
	    s1=(f1-b->xRange[n1])/(b->xRange[n1+1]-b->xRange[n1]);
	}
	
	f2=fmod(hit->f2*b->yRepeat,1.0);
	colorit(b->yCount,b->yRange,f2,&ic);
	n2=ic;
	if(n2+1 >= b->yCount){
	    s2=1;
	}else{
	    s2=(f2-b->yRange[n2])/(b->yRange[n2+1]-b->yRange[n2]);
	}
	
	nn = 4*(n1 + n2 * b->xCount);
	n1=b->cFlags[nn];
	n2=b->cFlags[nn+1];
	n3=b->cFlags[nn+2];
	n4=b->cFlags[nn+3];
	
	c1=(1.f-s2)*(1.f-s1);
	c2=(1.f-s2)*s1;
	c3=s1*s2;
	c4=s2*(1.f-s1);
	NL[0]=c1*b->bNormal[n1].x+c2*b->bNormal[n2].x+c3*b->bNormal[n3].x+c4*b->bNormal[n4].x;
	NL[1]=c1*b->bNormal[n1].y+c2*b->bNormal[n2].y+c3*b->bNormal[n3].y+c4*b->bNormal[n4].y;
	NL[2]=c1*b->bNormal[n1].z+c2*b->bNormal[n2].z+c3*b->bNormal[n3].z+c4*b->bNormal[n4].z;
	VecCopy(N,NS);
	N[0]=hit->v1[0]*NL[0]+hit->v2[0]*NL[1]+NS[0]*NL[2];
	N[1]=hit->v1[1]*NL[0]+hit->v2[1]*NL[1]+NS[1]*NL[2];
	N[2]=hit->v1[2]*NL[0]+hit->v2[2]*NL[1]+NS[2]*NL[2];
	/*
	VecNormalize(N);
	*/
	return 0;
}
long doImages(void *v1,void *v2,void *v3)
{
    struct FileInfo2 *Files;
	struct Material *m;
	long nn;
	unsigned char *Data;
	struct Image *i;
	Isect * hit;
	Flt *surf_color;
	double r,s;
    struct SetFrameData *sd;
    long x,y;


	m=(struct Material *)v1;
	i = &m->ImageData;
	hit=(Isect *)v2;
	surf_color=(Flt *)v3;

	Files=i->Files;
	if(!Files)return 0l;

	sd = &Files->SetData;
	if(sd->SetFrame < 0)return 0l;


	r=hit->f1;
	if(r < i->rmin || r > i->rmax)return 0;
	s=hit->f2;
	if(s < i->smin || s > i->smax)return 0;


	i->dr=i->rmax-i->rmin;
	i->ds=i->smax-i->smin;
	i->dx=(double)(i->xmax-i->xmin);
	i->dy=(double)(i->ymax-i->ymin);
	

	if(i->repeatr > 0){
	    r=fmod((double)(i->repeatr)*(r-i->rmin)/(i->dr),1.0);
	}else{
	    r=(r-i->rmin)/(i->dr);
	}


	if(i->repeats > 0){
	    s=fmod((double)(i->repeats)*(s-i->smin)/(i->ds),1.0);
	}else{
	    s=(s-i->smin)/(i->ds);
	}


	x=(long)((double)(i->xmin)+r*(i->dx));
	if(x >= sd->xsize)x=sd->xsize-1;
	y=(long)((double)(i->ymin)+s*(i->dy));
	if(y >= sd->ysize)y=sd->ysize-1;


	if(sd->type == TRUE_FILES ){
	    Data = (unsigned char *)Files->dataSave;
	    nn = 3*(x + (sd->ysize-1-y)*sd->xsize);
	    surf_color[0]=(Data[nn])/255.f;
	    surf_color[1]=(Data[nn+1])/255.f;
	    surf_color[2]=(Data[nn+2])/255.f;
	}else if(sd->type == HDF_FILES){
	    Data = (unsigned char *)Files->dataSave;
	    nn = (x + (sd->ysize-1-y)*sd->xsize);
	    surf_color[0]=(Files->palette[3*Data[nn]])/255.f;
	    surf_color[1]=(Files->palette[3*Data[nn]+1])/255.f;
	    surf_color[2]=(Files->palette[3*Data[nn]+2])/255.f;
	}else if(sd->type == SDS2D_FILES){
	    double *dat;
	    int ic;
	    if(m->flag & M_COLORLEVEL){
	        dat = (double *)sd->data;
	        nn = (x + (sd->ysize-1-y)*sd->xsize);
			colorit(m->ColorCount,m->ColorLevel,dat[nn],&ic);
			ColorToVec(m->ColorValue[ic],surf_color);
	    }
	}
		return 0;

}
long doStripes(void *v1,void *v2,void *surf_color)
{
	struct Material *m;
	struct Stripes *s;
	Isect *hit;

	int n1,n2;
	Flt f1,f2;
	long nn;

	m=(struct Material *)v1;
	s=(struct Stripes *)m->PatternData;
	hit=(Isect *)v2;

	f1=fmod(hit->f1*s->xRepeat,1.0);
	f2=fmod(hit->f2*s->yRepeat,1.0);
	colorit(s->xCount,s->xRange,f1,&n1);
	colorit(s->yCount,s->yRange,f2,&n2);
	nn = n1 + n2 * s->xCount;
	if(s->cFlags[nn]){
	    ColorToVec(s->cColor[nn],(Flt *)surf_color);
	}
	return 0;

}
int colorit(long count,double *level,Flt value,int *ic)
{
	long ib,it,ns;
	
	if(value <= *level){
	    *ic=0;
	    return 0;
	}
	it=count-1;
	if(value >= level[it]){
	    *ic=(int)it;
	    return 0;
	}
	ib=0;
	while(it > ib+1){
	    ns=(it+ib)/2;
	    if(value > level[ns]){
	        ib=ns;
	    }else{
	        it=ns;
	    }
	}
	*ic=(int)ib;
	return 0;
	
}
struct P Norm(struct P *p1)
{
    struct P p;
    double sum;

    sum=1./pow(p1->x*p1->x+p1->y*p1->y+p1->z*p1->z,.5);
    p.x=(p1->x*sum);
    p.y=(p1->y*sum);
    p.z=(p1->z*sum);
    return p;
}
struct P CrossN(struct P *v1,struct P *v2)
{
    static struct P Zero;
    struct P Vcross;
    double Length;

    Vcross.x =   v1->y*v2->z-v2->y*v1->z;
    Vcross.y = -(v1->x*v2->z-v2->x*v1->z);
    Vcross.z =   v1->x*v2->y-v2->x*v1->y;
    Length=Len(&Vcross);
    if(Length <= 0.)return Zero;
    Vcross.x/=Length;
    Vcross.y/=Length;
    Vcross.z/=Length;

    return  Vcross;
}
struct P Sub(struct P *p1,struct P *p2)
{
    struct P psub;

    psub.x=p1->x-p2->x;
    psub.y=p1->y-p2->y;
    psub.z=p1->z-p2->z;
    return psub;
 
}
struct P Add(struct P *p1,struct P *p2)
{
    struct P padd;

    padd.x=p1->x+p2->x;
    padd.y=p1->y+p2->y;
    padd.z=p1->z+p2->z;
    return padd;
 
}
struct P Mult(struct P *p1,double Factor)
{
    struct P p;

    p.x=(p1->x*Factor);
    p.y=(p1->y*Factor);
    p.z=(p1->z*Factor);
    return p;
}
double Len(struct P *v1)
{
    double Lenr;

    Lenr=pow((v1->x)*(v1->x)+
            (v1->y)*(v1->y)+
            (v1->z)*(v1->z),.5);

    return  Lenr;
}
double Dot(struct P *v1,struct P *v2)
{
    double Length;

    Length=(v1->x*v2->x)+(v1->y*v2->y)+(v1->z*v2->z);

    return  Length;
}
int Transf1(struct Trans1 *f)
{
	struct P Oa,Xa,Ya,Za,On,Xn,Yn,Zn;
	long p,pmax,g,gmax,range;
	struct P *d,*dn,pl;
	double a,b,c;
	int Type,normals;
	char *pp,*pn,*ps;
	struct Sphere *Spherep;
	struct Poly *Polyp;
	struct Cone *Conep;
	struct Quad *Quadp;
	struct QuadN *Quadnp;
	struct QuadNC *Quadncp;
	struct Triangle *Trianglep;
	struct TriangleN *Trianglenp;
	struct TriNC *TriNCp;
	struct Ring *Ringp;
	
	Oa=f->Oa;
	Xa=f->Xa;
	Ya=f->Ya;
	Za=f->Za;
	vec3(&Xa,&Ya,&Za);
	On=f->On;
	Xn=f->Xn;
	Yn=f->Yn;
	Zn=f->Zn;
	vec3(&Xn,&Yn,&Zn);
	
	normals=FALSE;
	ps=f->P;
	pp=NULL;
	pn=NULL;
	pmax=0;
	switch(Type=(int)f->Type){
	case T_SPHERE:
	     range=sizeof(struct Sphere);
	     pmax=1;
	     Spherep=(struct Sphere *)ps;
	     pp=(char *)(&Spherep->origin);
	     break;
	case T_POLY:
	     range=sizeof(struct Poly);
	     Polyp=(struct Poly *)ps;
	     pp=(char *)(&Polyp->points);
	     break;
	case T_CONE:
	     range=sizeof(struct Cone);
	     pmax=2;
	     Conep=(struct Cone *)ps;
	     pp=(char *)(&Conep->basepoint);
	     break;
	case T_QUAD:
	     range=sizeof(struct Quad);
	     pmax=4;
	     Quadp=(struct Quad *)ps;
	     pp=(char *)(&Quadp->p1);
	     break;
	case T_QUADN:
	    range=sizeof(struct QuadN);
	    pmax=4;
	    normals=TRUE;
	    Quadnp=(struct QuadN *)ps;
	    pp=(char *)(&Quadnp->p1);
	    pn=(char *)(&Quadnp->n1);
	    break;
	case T_QUADNC:
	    range=sizeof(struct QuadNC);
	    pmax=4;
	    normals=TRUE;
	    Quadncp=(struct QuadNC *)ps;
	    pp=(char *)(&Quadncp->p1);
	    pn=(char *)(&Quadncp->n1);
	    break;
	case T_TRI:
	    range=sizeof(struct Triangle);
	    pmax=3;
	    Trianglep=(struct Triangle *)ps;
	    pp=(char *)(&Trianglep->p1);
	    break;
	case T_TRIN:
	    range=sizeof(struct TriangleN);
	    pmax=3;
	    normals=TRUE;
	    Trianglenp=(struct TriangleN *)ps;
	    pp=(char *)(&Trianglenp->p1);
	    pn=(char *)(&Trianglenp->n1);
	    break;
	case T_TRINC:
	    range=sizeof(struct TriNC);
	    pmax=3;
	    normals=TRUE;
	    TriNCp=(struct TriNC *)ps;
	    pp=(char *)(&TriNCp->p1);
	    pn=(char *)(&TriNCp->n1);
	    break;
	case T_RING:
	    range=sizeof(struct Ring);
	    pmax=1;
	    normals=TRUE;
	    Ringp=(struct Ring *)ps;
	    pp=(char *)(&Ringp->origin);
	    pn=(char *)(&Ringp->normal);
	    break;
	case T_BLOCK:
	    return TransBlock((struct Block *)ps,f->Groups,&Oa,&Xa,&Ya,&Za,&On,&Xn,&Yn,&Zn);
	case T_BLOCKSYSTEM:
	    return TransBlockSystem((struct Block *)ps,f->Groups,&Oa,&Xa,&Ya,&Za,&On,&Xn,&Yn,&Zn);
	case T_LOCALSYSTEM:
	    return TransLocalSystem((struct System *)ps,f->Groups,&Oa,&Xa,&Ya,&Za,&On,&Xn,&Yn,&Zn);
	default:
	    sprintf(WarningBuff,"Unknown Transformation Type (%d)\n",Type);
		WarningBatch(WarningBuff);
	    return 1;
	}
	gmax=f->Groups;
	for(g=0;g<gmax;++g){
	    if(Type != T_POLY){
		d=(struct P *)pp;
		dn=(struct P *)pn;
	    }else{
		d=(*(struct P **)pp);
		dn=(struct P *)pn;
		pmax=(*(long *)(pp+sizeof(long)));
	    }
	    for(p=0;p<pmax;++p){
		pl=Sub(d,&Oa);
		a=Dot(&pl,&Xa);
		b=Dot(&pl,&Ya);
		c=Dot(&pl,&Za);
		pl.x=On.x+a*Xn.x+b*Yn.x+c*Zn.x;
		pl.y=On.y+a*Xn.y+b*Yn.y+c*Zn.y;
		pl.z=On.z+a*Xn.z+b*Yn.z+c*Zn.z;
		*d++ = pl;
		if(normals){
		    pl = *dn;
	            a=Dot(&pl,&Xa);
	            b=Dot(&pl,&Ya);
	            c=Dot(&pl,&Za);
	            pl.x=a*Xn.x+b*Yn.x+c*Zn.x;
	            pl.y=a*Xn.y+b*Yn.y+c*Zn.y;
	            pl.z=a*Xn.z+b*Yn.z+c*Zn.z;
	            *dn++ = pl;
	             
		}
	    }
	    pp += range;
	    pn += range;
	}
	return 0;
}
int TransBlockSystem(struct Block *block,long count,
					 struct P *Oa,struct P *Xa,struct P *Ya,struct P *Za,
					 struct P *On,struct P *Xn,struct P *Yn,struct P *Zn)
{
	struct Block *bl;
	struct P d,pl;
	double a,b,c;
	long k;

	for(k=0;k<count;++k){
	    bl=&block[k];
	    d=bl->Local.p;
	    pl=Sub(&d,Oa);
	    a=Dot(&pl,Xa);
	    b=Dot(&pl,Ya);
	    c=Dot(&pl,Za);
	    pl.x=On->x+a*Xn->x+b*Yn->x+c*Zn->x;
	    pl.y=On->y+a*Xn->y+b*Yn->y+c*Zn->y;
	    pl.z=On->z+a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->Local.p=pl;
	    
	    pl=bl->Local.x;
            a=Dot(&pl,Xa);
            b=Dot(&pl,Ya);
            c=Dot(&pl,Za);
            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->Local.x=pl;
		
		
	    pl=bl->Local.y;
            a=Dot(&pl,Xa);
            b=Dot(&pl,Ya);
            c=Dot(&pl,Za);
            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->Local.y=pl;
		
		
	    pl=bl->Local.z;
            a=Dot(&pl,Xa);
            b=Dot(&pl,Ya);
            c=Dot(&pl,Za);
            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->Local.z=pl;
		
	}	
	return 0;
}
int TransLocalSystem(struct System *block,long count,
					 struct P *Oa,struct P *Xa,struct P *Ya,struct P *Za,
					 struct P *On,struct P *Xn,struct P *Yn,struct P *Zn)
{
	struct System *bl;
	struct P d,pl;
	double a,b,c;
	long k;

	for(k=0;k<count;++k){
	    bl=&block[k];
	    d=bl->p;
	    pl=Sub(&d,Oa);
	    a=Dot(&pl,Xa);
	    b=Dot(&pl,Ya);
	    c=Dot(&pl,Za);
	    pl.x=On->x+a*Xn->x+b*Yn->x+c*Zn->x;
	    pl.y=On->y+a*Xn->y+b*Yn->y+c*Zn->y;
	    pl.z=On->z+a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->p=pl;
	    
	    pl=bl->x;
            a=Dot(&pl,Xa);
            b=Dot(&pl,Ya);
            c=Dot(&pl,Za);
            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->x=pl;
		
		
	    pl=bl->y;
            a=Dot(&pl,Xa);
            b=Dot(&pl,Ya);
            c=Dot(&pl,Za);
            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->y=pl;
		
		
	    pl=bl->z;
            a=Dot(&pl,Xa);
            b=Dot(&pl,Ya);
            c=Dot(&pl,Za);
            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
	    bl->z=pl;
		
	}	
	return 0;
}
int TransBlock(struct Block *block,long count,
struct P *Oa,struct P *Xa,struct P *Ya,struct P *Za,
struct P *On,struct P *Xn,struct P *Yn,struct P *Zn)
{
    double *dxdata,*dydata,*dzdata;
    double *xdata,*ydata,*zdata;
    double *oldx,*oldy,*oldz;
    double *nx,*ny,*nz;
	struct Block *bl;
	struct P d,pl;
	double a,b,c;
	long k,n,NodeCount;

	for(k=0;k<count;++k){
	    bl=&block[k];
        dxdata=bl->dxdata;
        dydata=bl->dydata;
        dzdata=bl->dzdata;
        xdata=bl->xdata;
        ydata=bl->ydata;
        zdata=bl->zdata;
        oldx=bl->oldx;
        oldy=bl->oldy;
        oldz=bl->oldz;
        nx=bl->nx;
        ny=bl->ny;
        nz=bl->nz;
	    NodeCount=bl->NodeCount;
	    if(xdata && ydata && zdata){
	    	for(n=0;n<NodeCount;++n){
	            d.x=xdata[n];
	            d.y=ydata[n];
	            d.z=zdata[n];
			    pl=Sub(&d,Oa);
			    a=Dot(&pl,Xa);
			    b=Dot(&pl,Ya);
			    c=Dot(&pl,Za);
			    pl.x=On->x+a*Xn->x+b*Yn->x+c*Zn->x;
			    pl.y=On->y+a*Xn->y+b*Yn->y+c*Zn->y;
			    pl.z=On->z+a*Xn->z+b*Yn->z+c*Zn->z;
			    xdata[n]=pl.x;
			    ydata[n]=pl.y;
			    zdata[n]=pl.z;
			}
		}
		if(dxdata && dydata && dzdata){
	    	for(n=0;n<NodeCount;++n){
			    pl.x=dxdata[n];
			    pl.y=dydata[n];
			    pl.z=dzdata[n];
	            a=Dot(&pl,Xa);
	            b=Dot(&pl,Ya);
	            c=Dot(&pl,Za);
	            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
	            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
	            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
			    dxdata[n]=pl.x;
			    dydata[n]=pl.y;
			    dzdata[n]=pl.z;
			}
		             
		}
		if(oldx && oldy && oldz){
	    	for(n=0;n<NodeCount;++n){
	            d.x=oldx[n];
	            d.y=oldy[n];
	            d.z=oldz[n];
			    pl=Sub(&d,Oa);
			    a=Dot(&pl,Xa);
			    b=Dot(&pl,Ya);
			    c=Dot(&pl,Za);
			    pl.x=On->x+a*Xn->x+b*Yn->x+c*Zn->x;
			    pl.y=On->y+a*Xn->y+b*Yn->y+c*Zn->y;
			    pl.z=On->z+a*Xn->z+b*Yn->z+c*Zn->z;
			    oldx[n]=pl.x;
			    oldy[n]=pl.y;
			    oldz[n]=pl.z;
			}
		}
		if(nx && ny && nz){
	    	for(n=0;n<NodeCount;++n){
			    pl.x=nx[n];
			    pl.y=ny[n];
			    pl.z=nz[n];
	            a=Dot(&pl,Xa);
	            b=Dot(&pl,Ya);
	            c=Dot(&pl,Za);
	            pl.x=a*Xn->x+b*Yn->x+c*Zn->x;
	            pl.y=a*Xn->y+b*Yn->y+c*Zn->y;
	            pl.z=a*Xn->z+b*Yn->z+c*Zn->z;
			    nx[n]=pl.x;
			    ny[n]=pl.y;
			    nz[n]=pl.z;
			}
	    }
	}	
	return 0;
}
static int vec3(struct P *x,struct P *y,struct P *z)
{
	double lenx,leny,lenz;
	static int error;

	if(!x || !y || !z)return 1;
	
	lenx = Dot(x,x);
	leny = Dot(y,y);
	lenz = Dot(z,z);
	
	if(lenx && leny){
 	    *x=Norm(x);
 	    *y=Norm(y);
 	    *z=CrossN(x,y);
 	    *y=CrossN(z,x);
	}else if(lenx && lenz){
 	    *x=Norm(x);
 	    *z=Norm(z);
 	    *y=CrossN(z,x);
 	    *x=CrossN(y,z);
	}else if(leny && lenz){
 	    *y=Norm(y);
 	    *z=Norm(z);
 	    *x=CrossN(y,z);
 	    *z=CrossN(x,y);
	}else{
	    if(error){
	        x->x=1;
	        y->y=1;
	        z->z=1;
			/*
	        Warning("Transformation Coordinate System Error Default System Used");
			*/
		}
	    error=TRUE;
	    return 1;
	}
	return 0;
}
int doImageWrite(renderPtr openData,struct Scene *scene,float *vbuff)
{
	if(!scene)return 1;
	
    if(scene->NetAddress){
         /* return SendNetv(scene,vbuff); */
    }else{
         return WriteComp(openData,scene,vbuff);
    }
    return 0;
}
int WriteComp(renderPtr openData,struct Scene *scene,float *vbuff)
{
    unsigned char *bline,*bout,*bp;
    long ixmax,iymax;
    long length,n;
    register int c1,c2,c3,max;
	struct paletteDraw *pd;
	long xsizep,ysizep,ix,iy;
	double scale;
	
	if(!scene)return 1;

        bline=NULL;
        bout=NULL;
/*
        if(scene->Rastor8){
            WriteR8(scene,vbuff);
            return;
        }
*/
	pd=&(scene->pd);
    ixmax=scene->xResolution;
    iymax=scene->yResolution;
	uGetPaletteSize(&xsizep,&ysizep,pd,openData->myIcon);
	ixmax += xsizep;
	if(ysizep > iymax)iymax=ysizep;
    length=3*ixmax*iymax;
    if(SetBuffers(length))return 1;
	bline=(unsigned char *)xg.buffin;
	bout=(unsigned char *)xg.buffout;

	if(pd->LabeledPalette){
	    long xsize,ysize;
            xsize=scene->xResolution;
            ysize=scene->yResolution;
            for(iy=0;iy<iymax;++iy){
                for(ix=0;ix<ixmax;++ix){
                    n=ix*3+iy*ixmax*3;
                    if(ix < xsize && iy < ysize){
                        c1 = max = (int)(255.0 * (*vbuff++));
                        c2 = (int)(255.0 * (*vbuff++));
                        c3 = (int)(255.0 * (*vbuff++));
                        if(c2 > max)max = c2;
                        if(c3 > max)max = c3;
                        if(max > 255){
                            scale=255./(double)max;
                            bline[n]=   (unsigned char)(c1*scale);
                            bline[1+n]= (unsigned char)(c2*scale);
                            bline[2+n]= (unsigned char)(c3*scale);
                       }else{
                            bline[n]=   (unsigned char)c1;
                            bline[1+n]= (unsigned char)c2;
                            bline[2+n]= (unsigned char)c3;
                        }
                    }else{
                        bline[n]= 255;
                        bline[1+n]= 255;
                        bline[2+n]= 255;
                    }
                }
            }
	    pd->buffer=bline;
	    pd->xsize=ixmax;
	    pd->ysize=iymax;
	    pd->left=ixmax-xsizep;
	    pd->top=iymax/2-128-10;
	    pd->ScreenBits=RASTER_24;
	    uDrawPalette(pd,openData->myIcon);
	}else{
	    bp=bline;;
            for(iy=0;iy<iymax;++iy){
                for(ix=0;ix<ixmax;++ix){
                    c1 = max = (int)(255.0 * (*vbuff++));
                    c2 = (int)(255.0 * (*vbuff++));
                    c3 = (int)(255.0 * (*vbuff++));
                    if(c2 > max)max = c2;
                    if(c3 > max)max = c3;
                    if(max > 255){
                        scale=255./(double)max;
                        *bp++ = (unsigned char)(c1*scale);
                        *bp++ = (unsigned char)(c2*scale);
                        *bp++ = (unsigned char)(c3*scale);
                   }else{
                        *bp++ = (unsigned char)c1;
                        *bp++ = (unsigned char)c2;
                        *bp++ = (unsigned char)c3;
                    }
                }
            }
	}

	saveRenderImage(openData,bline,ixmax,iymax);

    if(scene->FileOut)deScaleComp(scene,bline,bout,ixmax,iymax);
    if(scene->Dump)
        (*scene->Dump)(scene,bline,bout,ixmax,iymax,scene->DumpData);
    return 0;
}
int deScaleComp(struct Scene *scene,unsigned char *bline,unsigned char *bout,long xsize,long ysize)
{
        unsigned int Esc1,Esc2;
        long lin,lout;
        unsigned int mask;
        char buff[256];
        FILE *nout;
        int ret;
        
        if(!scene || !bline || !bout)return 1;

        mask=255;
        lin=xsize*ysize*3;

        if(Stuff(&Esc1,&Esc2,bline,bout,lin,&lout,mask)){
            return 1;
        }

        if(!(nout=fopen(scene->FileOut,"wb"))){
            sprintf(buff,"Error Opening %s To Write\n",scene->FileOut);
            WarningBatch(buff);
            return 1;
        }
	ret = 1;

        if(putString((unsigned char *)"DRt1",4L,nout))goto ErrorOut;

        if(putInt((int)xsize,nout))goto ErrorOut;
        if(putInt((int)ysize,nout))goto ErrorOut;

        if(putInt((int)Esc1,nout))goto ErrorOut;
        if(putInt((int)Esc2,nout))goto ErrorOut;

        if(putLong(lin,nout))goto ErrorOut;
        if(putLong(lout,nout))goto ErrorOut;

        if(putString((unsigned char *)bout,lout,nout))goto ErrorOut;

 	ret = 0;
 ErrorOut:
       if(ret){
           WarningBatch("Error Writing TrueColor File\n");
       }
       if(nout)fclose(nout);
       return ret;
}
void ScaleMatrix2(struct Matrix *m,struct System *p)
{
	if(!m || !p)return;
	
	m->x[0][0]=m->x[0][0]*p->Scale_x;
	m->x[1][0]=m->x[1][0]*p->Scale_x;
	m->x[2][0]=m->x[2][0]*p->Scale_x;
	m->x[0][1]=m->x[0][1]*p->Scale_y;
	m->x[1][1]=m->x[1][1]*p->Scale_y;
	m->x[2][1]=m->x[2][1]*p->Scale_y;
	m->x[0][2]=m->x[0][2]*p->Scale_z;
	m->x[1][2]=m->x[1][2]*p->Scale_z;
	m->x[2][2]=m->x[2][2]*p->Scale_z;
}
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1)
{
	double sum;
	int i,j,m;

	if(!t || !t2 || !t1)return;
	
	for(j=0;j<4;++j){
		for(i=0;i<4;++i){
		    sum = 0;
		    for(m=0;m<4;++m){
		    	sum += t2->x[i][m]*t1->x[m][j];
		    }
		    t->x[i][j]=sum;
		}
	}
}
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2)
{
    /*   Matrix to transform a point from system p1 to system p2  */

	struct P p12;
	
	if(!m || !p1 || !p2)return;
	
	m->x[0][0]=Dot(&p2->x,&p1->x);
	m->x[1][0]=Dot(&p2->y,&p1->x);
	m->x[2][0]=Dot(&p2->z,&p1->x);
	m->x[3][0]=0;
	m->x[0][1]=Dot(&p2->x,&p1->y);
	m->x[1][1]=Dot(&p2->y,&p1->y);
	m->x[2][1]=Dot(&p2->z,&p1->y);
	m->x[3][1]=0;
	m->x[0][2]=Dot(&p2->x,&p1->z);
	m->x[1][2]=Dot(&p2->y,&p1->z);
	m->x[2][2]=Dot(&p2->z,&p1->z);
	m->x[3][2]=0;
	p12=Sub(&p1->p,&p2->p);
	m->x[0][3]=Dot(&p2->x,&p12);
	m->x[1][3]=Dot(&p2->y,&p12);
	m->x[2][3]=Dot(&p2->z,&p12);
	m->x[3][3]=1;
}

int doNoShadowVRMLBEST(long *ndata,long *matdata,float *normal,long normmax,
		 long *normlist,long normlistmax,
                 double *xdata,double *ydata,double *zdata,double *vdata,
                 float *nvdata,long nel,long CellCount,struct Scene *scene,struct tState *tS)
{
		struct drawInfo di;
		
		zerol((char *)&di,sizeof(di));
		
		di.ndata=ndata;
		di.xdata=xdata;
		di.ydata=ydata;
		di.zdata=zdata;
		di.nvdata=nvdata;
		di.vdata=vdata;
		di.nel=nel;
		di.CellCount=CellCount;
		di.scene=scene;
		
		di.matdata=matdata;
		di.normal=normal;
		di.normmax=normmax;
		di.normlist=normlist;
		di.normlistmax=normlistmax;
		di.tS=tS;
		

		runThreads(scene->threads,&di,scene->yResolution,doNoShadowVRMLBESTT);
		
		return 0;
}               

int doNoShadowVRMLBESTT(mThread *Threads)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over,ipass,ip;
	long n1,n2,n3,n4;
	long xsize3;
	int rastorY;
	
	struct Material surf;
	ColorVEC	surf_color,specular,ambient;
	long CellSkip;
        long *CellBuff,b2;
	Vec N,P,L,D;
	double nx1,ndx1,ndx2,ny1,ndy1,ndy2,nz1,ndz1,ndz2;
	double x1,dx1,dx2,y1,dy1,dy2,z1,dz1,dz2;
	double len;
	double spec,colr,colg,colb;
	long ns,ne,nc,ncell,mat;
	int MaterialType;
	struct vrmlMaterial *vrmlMat;
	long SkipPoly;
	int warn;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;
	int ip2,ipass2;
	double xplane,yplane;
	double h99;
	Vec v1,v2;
                                  
	struct Scene *scene;
	struct drawInfo *di;
	
	if(!Threads)goto ErrorOut;
	di=(struct drawInfo *)Threads->data;
	if(!di)goto ErrorOut;
	scene=di->scene;
	if(!scene)goto ErrorOut;

	warn=0;

	ambient[0]=ambient[1]=ambient[2]=0;
	specular[0]=specular[1]=specular[2]=0;
	
	CellBuff=scene->CellBuff;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}

	if(!di->tS)goto ErrorOut;
	

	zerol((char *)&surf,sizeof(struct Material));
	surf.opacity=di->tS->opacity;

	MaterialType=di->tS->MaterialType;
	vrmlMat=di->tS->m;
	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	ncell=0;
	SkipPoly=0;
	h99=0.99*rd->h;
	for(nn=0;nn<di->nel;++nn,++ncell){	    
		double sxx1,sxx2,sxx3,syy1,syy2,syy3,sp1z,sp2z,sp3z;
		double xx1,xx2,xx3,yy1,yy2,yy3;
		double nx,ny,nz,den,cn,dd,odd;
		double zmin,zmax;
	    ns=nn;
	    ne=nn;
	    while(di->ndata[ne] >= 0 && ne < di->nel)++ne;
	    nn=ne;
	    nc=ne-ns;
	    if(nc < 3){
	        SkipPoly++;
	        continue;
	    }else if( nc > 4){
	        goto doPolyHere;
	    }
	    n1=di->ndata[ns];
	    n2=di->ndata[ns+1];
	    n3=di->ndata[ns+2];
	    if(nc == 3){
	        ipass=1;
	        n4=0;
	    }else{
	        ipass=2;
	        n4=di->ndata[ns+3];
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

	    p1z=di->zdata[k1];
	    p2z=di->zdata[k2];
	    p3z=di->zdata[k3];
	    
	    
	    
		{
		    double ss;
			sxx1=xx1=di->xdata[k1];
			sxx2=xx2=di->xdata[k2];
			sxx3=xx3=di->xdata[k3];
			syy1=yy1=di->ydata[k1];
			syy2=yy2=di->ydata[k2];
			syy3=yy3=di->ydata[k3];
			sp1z=p1z;
			sp2z=p2z;
			sp3z=p3z;
        	ipass2=1;
	        if(p1z >= h99){
	            if(p2z >= h99){
	                if(p3z >= h99)continue; /* All */
	                /* p1 & p2  */
	            	ss=(h99-p3z)/(p1z-p3z);
	            	xx1=xx3+ss*(xx1-xx3);
	            	yy1=yy3+ss*(yy1-yy3);
	            	p1z=h99;
	            	
	            	ss=(h99-p3z)/(p2z-p3z);
	            	xx2=xx3+ss*(xx2-xx3);
	            	yy2=yy3+ss*(yy2-yy3);
	            	p2z=h99;
	            }else if(p3z >= h99){
	                /* p1 & p3  */
	            	ss=(h99-p2z)/(p1z-p2z);
	            	xx1=xx2+ss*(xx1-xx2);
	            	yy1=yy2+ss*(yy1-yy2);
	            	p1z=h99;
	            	
	            	ss=(h99-p2z)/(p3z-p2z);
	            	xx3=xx2+ss*(xx3-xx2);
	            	yy3=yy2+ss*(yy3-yy2);
	            	p3z=h99;
	            }else{
	                /* p1 should split  */
	            	ss=(h99-p2z)/(p1z-p2z);
	            	xx1=xx2+ss*(xx1-xx2);
	            	yy1=yy2+ss*(yy1-yy2);
	            	p1z=h99;

	            	ss=(h99-sp3z)/(sp1z-sp3z);
	            	sxx3=sxx3+ss*(sxx1-sxx3);
	            	syy3=syy3+ss*(syy1-syy3);
	            	sp3z=h99;
	            	
		            sxx1=xx1;
	            	syy1=yy1;
	            	sp1z=h99;
	            		            	
	            	sxx2=xx3;
	            	syy2=yy3;
	            	sp2z=p3z;
	            		            	
            		            	
	                ipass2=2;
	            	
	            }
	        }else if(p2z >= h99){
	             if(p3z >= h99){
	                 /* p2 & p3  */
	            	ss=(h99-p1z)/(p2z-p1z);
	            	xx2=xx1+ss*(xx2-xx1);
	            	yy2=yy1+ss*(yy2-yy1);
	            	p2z=h99;

	            	ss=(h99-p1z)/(p3z-p1z);
	            	xx3=xx1+ss*(xx3-xx1);
	            	yy3=yy1+ss*(yy3-yy1);
	            	p3z=h99;
	             }else{
	                 /* p2 should split  */
	            	ss=(h99-p1z)/(p2z-p1z);
	            	xx2=xx1+ss*(xx2-xx1);
	            	yy2=yy1+ss*(yy2-yy1);
	            	p2z=h99;
	            	
		            ss=(h99-sp3z)/(sp2z-sp3z);
	            	sxx2=sxx3+ss*(sxx2-sxx3);
	            	syy2=syy3+ss*(syy2-syy3);
	            	sp2z=h99;
	            	
	            	sxx1=xx2;
	            	syy1=yy2;
	            	sp1z=h99;
	            	
	                ipass2=2;
	             }
	        }else if(p3z >= h99){
	            /* p3 should split */
	            ss=(h99-p1z)/(p3z-p1z);
	            xx3=xx1+ss*(xx3-xx1);
	            yy3=yy1+ss*(yy3-yy1);
	            p3z=h99;
	            
		        ss=(h99-sp2z)/(sp3z-sp2z);
	            sxx3=sxx2+ss*(sxx3-sxx2);
	            syy3=syy2+ss*(syy3-syy2);
	            sp3z=h99;
	            
	            sxx1=xx3;
	            syy1=yy3;
	            sp1z=h99;
	            
	            ipass2=2;
	        }
	        
	        
		}
		
        for(ip2=0;ip2<ipass2;++ip2){
        
        if(ip2 == 1){
			xx1=sxx1;
			xx2=sxx2;
			xx3=sxx3;
			yy1=syy1;
			yy2=syy2;
			yy3=syy3;
			p1z=sp1z;
			p2z=sp2z;
			p3z=sp3z;
        }

  	    ztran1 = rd->h/(rd->h-p1z);
	    ztran2 = rd->h/(rd->h-p2z);
	    ztran3 = rd->h/(rd->h-p3z);
	    p1y=(ztran1*yy1-rd->ymin)*rd->ody;
	    p2y=(ztran2*yy2-rd->ymin)*rd->ody;
	    p3y=(ztran3*yy3-rd->ymin)*rd->ody;
	    
	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }

	    if((iymax == iymin) || (iymax < Threads->smin) || (iymin >= Threads->smax)){
	        ++CellSkip;
	        continue;
	    }
	    
	    
	    if(iymin < Threads->smin)iymin=Threads->smin;
	    if(iymax >= Threads->smax)iymax=Threads->smax-1;
	    	    	    	    
	    p1x=(rd->o+ztran1*(xx1-rd->o)-rd->xmin)*rd->odx;	    
	    p2x=(rd->o+ztran2*(xx2-rd->o)-rd->xmin)*rd->odx;
	    p3x=(rd->o+ztran3*(xx3-rd->o)-rd->xmin)*rd->odx;
	    
	 	    
	    zmin=zmax=p1z;
	    if(p1z > p2z){
	        zmin=p2z;
	    }else{
	        zmax=p2z;
	    }
	    
	    if(p3z > zmax)zmax=p3z;
	    
	    if(p3z < zmin)zmin=p3z;
		
	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }


	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;
	    
 	    x1=di->xdata[k1];
	    dx1=di->xdata[k2]-x1;
	    dx2=di->xdata[k3]-x1;

	    y1=di->ydata[k1];
	    dy1=di->ydata[k2]-y1;
	    dy2=di->ydata[k3]-y1;

 	    z1=di->zdata[k1];
	    dz1=di->zdata[k2]-z1;
	    dz2=di->zdata[k3]-z1;
	    
	    
		if(!(dd=(dx1)*(dy2)-(dy1)*(dx2))){
			continue;
		}

		odd=1.0/dd;
	    
	    nx=dy1*dz2-dy2*dz1;
	    ny=dx2*dz1-dx1*dz2;
	    nz=dx1*dy2-dx2*dy1;
	    den=sqrt(nx*nx+ny*ny+nz*nz);
	    if(den == 0)continue;	    
	    
	    nx /= den;
	    ny /= den;
	    nz /= den;
	    
	    cn=-(-nx*x1-ny*y1+(rd->h-z1)*nz);
	    
	    if((di->tS->NormalType == VRML_BY_VERTEX) && (ns < di->normlistmax)){
	        long kn1,kn2,kn3;
			if(ip == 0){
			    kn1=di->normlist[ns];
			    kn2=di->normlist[ns+1];
			    kn3=di->normlist[ns+2];
			}else{
			    kn1=di->normlist[ns+2];
			    kn2=di->normlist[ns+3];
			    kn3=di->normlist[ns];
			}
	        nx1=di->normal[kn1*3];
	        ndx1=di->normal[kn2*3]-nx1;
	        ndx2=di->normal[kn3*3]-nx1;

	        ny1=di->normal[kn1*3+1];
	        ndy1=di->normal[kn2*3+1]-ny1;
	        ndy2=di->normal[kn3*3+1]-ny1;

 	        nz1=di->normal[kn1*3+2];
	        ndz1=di->normal[kn2*3+2]-nz1;
	        ndz2=di->normal[kn3*3+2]-nz1;
	    }else{

	        nx1=di->nvdata[k1*3];
	        ndx1=di->nvdata[k2*3]-nx1;
	        ndx2=di->nvdata[k3*3]-nx1;

	        ny1=di->nvdata[k1*3+1];
	        ndy1=di->nvdata[k2*3+1]-ny1;
	        ndy2=di->nvdata[k3*3+1]-ny1;

 	        nz1=di->nvdata[k1*3+2];
	        ndz1=di->nvdata[k2*3+2]-nz1;
	        ndz2=di->nvdata[k3*3+2]-nz1;
/*	        
	        nx1=nx;
	        ndx1=0;
	        ndx2=0;

	        ny1=ny;
	        ndy1=0;
	        ndy2=0;

 	        nz1=nz;
	        ndz1=0;
	        ndz2=0;
*/	        
	        
	    }

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
		double den,xx,yy;
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
		
		yy=iyy/rd->ody+rd->ymin;

	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        double r,s,aa;
	        
	        if(zmin >= *b)continue;
	        
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
           
			xx=ixx/rd->odx+rd->xmin;
	        den=xx*nx+yy*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
	        	        	           		       
		    if((z=-aa*rd->h+rd->h) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
	    	r=((xx*aa-x1)*dy2-(yy*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy*aa-y1)-dy1*(xx*aa-x1))*odd;
	   		
	   		if(r < 0 || s < 0 || (r+s) > 1 )continue;
	   		
		    
			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
				   
		        
			if(MaterialType == VRML_BY_FACE){
			    mat=di->matdata[ncell];
			    if(vrmlMat){
			        if(mat >= vrmlMat->max)mat=0;
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;
			    if(ip == 0){
			        m[0]=di->matdata[ns];
			        m[1]=di->matdata[ns+1];
			        m[2]=di->matdata[ns+2];
			    }else{
			        m[0]=di->matdata[ns+2];
			        m[1]=di->matdata[ns+3];
			        m[2]=di->matdata[ns];
			    }

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                            s*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                            s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                            s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                            s*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                            s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                            s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                            s*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                            s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                            s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
			   if(warn++ < 2){
			        sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
			    }
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}

			if(di->tS->flagTexture)doVrmlTexture(surf_color,r,s,ns,ip,di->tS);

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong) ;
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);
	        }


    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=di->CellCount+nn;
	        rd->opacity[b2]=(float)(surf.opacity);
		    *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
		double den,xx,yy;
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    
		yy=iyy/rd->ody+rd->ymin;
		
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        double r,s,aa;
	        
	        if(zmax <= *b)continue;
	        
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){           

			xx=ixx/rd->odx+rd->xmin;

	        den=xx*nx+yy*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
	        	        	           
		    if((z=-aa*rd->h+rd->h) > *b){

	    	r=((xx*aa-x1)*dy2-(yy*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy*aa-y1)-dy1*(xx*aa-x1))*odd;

	   		if(r < 0 || s < 0 || (r+s) > 1 )continue;
	   		
	   		xplane=xx*aa;
	   		yplane=yy*aa;
		    
			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
				   
			if(MaterialType == VRML_BY_FACE){
			    mat=di->matdata[ncell];
			    if(vrmlMat){
			        if(mat >= vrmlMat->max)mat=0;
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;
			    if(ip == 0){
			        m[0]=di->matdata[ns];
			        m[1]=di->matdata[ns+1];
			        m[2]=di->matdata[ns+2];
			    }else{
			        m[0]=di->matdata[ns+2];
			        m[1]=di->matdata[ns+3];
			        m[2]=di->matdata[ns];
			    }

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                                s*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                  s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                  s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                              s*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                             s*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                               s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                               s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
				if(warn++ < 3){
				 	sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
				}
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}
			if(di->tS->flagTexture)doVrmlTexture(surf_color,r,s,ns,ip,di->tS);

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong);			
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);		        
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){	 
	        
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);	
	        
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=di->CellCount+nn;
		    *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	      }
	    }
	  }
    }
	 }
	continue;
doPolyHere: /* nc */

	    n1=di->ndata[ns];
	    n2=di->ndata[ns+1];
	    n3=di->ndata[ns+2];
	    {
	       double z;
	       long n,np;

		MakeVector(di->xdata[n2]-di->xdata[n1],di->ydata[n2]-di->ydata[n1],
			   di->zdata[n2]-di->zdata[n1],v1);
		MakeVector(di->xdata[n3]-di->xdata[n1],di->ydata[n3]-di->ydata[n1],
			   di->zdata[n3]-di->zdata[n1],v2);
		VecCross(v1,v2,N);
		VecNormalize(N);
		VecCross(N,v1,v2);
		VecNormalize(v1);
		VecNormalize(v2);
/*
		if(fabs(N[2]) < 1.e-4)continue;
*/	    
		ixmax = -100000;
		ixmin = 100000;
		iymax = -100000;
		iymin = 100000;
		
		 zmax = -1e60;
		 zmin =  1e60;
		
		 p1x=p2x=p3x=0;
		 p1y=p2y=p3y=0;
		 p1z=p2z=p3z=0;
		 k1=k2=k3=0;
		

		for(n=0;n<nc;++n){
		    double px,py;

		    np=di->ndata[ns+n];
		    z=di->zdata[np];
		    
		    if(z > zmax)zmax=z;
		    if(z < zmin)zmin=z;
		    
		    if(z >= h99){
		        z=h99;
		    }
		    
		    
		    ztran1 = rd->h/(rd->h-z);

		    px=(rd->o+ztran1*(di->xdata[np]-rd->o)-rd->xmin)*rd->odx;
		    ix = (int)px;
		    if(ix > ixmax)ixmax = ix;		        
		    if(ix < ixmin)ixmin = ix;		        

		    py=(ztran1*di->ydata[np]-rd->ymin)*rd->ody;	    	    	    
		    iy = (int)py;
		    if(iy > iymax)iymax = iy;		        
			if(iy < iymin)iymin = iy;
		    
		    if(n == 0){
		       k1=np;
		    }else if(n == 1){
		       k2=np;
		    }else if(n == 2){
		       k3=np;
		    }

		}
		
		if(zmin > h99){
		   zmin = h99;
		   continue;
		}
		
	        if((iymax == iymin) ||  (iymax < Threads->smin) || (iymin >= Threads->smax)){
	            ++CellSkip;
	            continue;
	        }
	        
	        if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	            ++CellSkip;
	            continue;
	        }
	    }

	    if(iymin < Threads->smin)iymin=Threads->smin;
	    if(iymax >= Threads->smax)iymax=Threads->smax-1;
	    	    	    

 	    x1=di->xdata[k1];
	    dx1=di->xdata[k2]-x1;
	    dx2=di->xdata[k3]-x1;

	    y1=di->ydata[k1];
	    dy1=di->ydata[k2]-y1;
	    dy2=di->ydata[k3]-y1;

 	    z1=di->zdata[k1];
	    dz1=di->zdata[k2]-z1;
	    dz2=di->zdata[k3]-z1;

	    
		if(!(dd=(dx1)*(dy2)-(dy1)*(dx2))){
	        ++CellSkip;
			continue;
		}

		odd=1.0/dd;
	    
	    nx=dy1*dz2-dy2*dz1;
	    ny=dx2*dz1-dx1*dz2;
	    nz=dx1*dy2-dx2*dy1;
	    den=sqrt(nx*nx+ny*ny+nz*nz);
	    if(den == 0){
	        ++CellSkip;
	        continue;	 
	    }   
	    
	    nx /= den;
	    ny /= den;
	    nz /= den;
	    
	    cn=-(-nx*x1-ny*y1+(rd->h-z1)*nz);
	

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    if((di->tS->NormalType == VRML_BY_VERTEX) && (ns < di->normlistmax)){
	        long kn1,kn2,kn3;

			kn1=di->normlist[ns];
			kn2=di->normlist[ns+1];
			kn3=di->normlist[ns+2];

	        nx1=di->normal[kn1*3];
	        ndx1=di->normal[kn2*3]-nx1;
	        ndx2=di->normal[kn3*3]-nx1;

	        ny1=di->normal[kn1*3+1];
	        ndy1=di->normal[kn2*3+1]-ny1;
	        ndy2=di->normal[kn3*3+1]-ny1;

 	        nz1=di->normal[kn1*3+2];
	        ndz1=di->normal[kn2*3+2]-nz1;
	        ndz2=di->normal[kn3*3+2]-nz1;
	    }else{
	        nx1=di->nvdata[k1*3];
	        ndx1=di->nvdata[k2*3]-nx1;
	        ndx2=di->nvdata[k3*3]-nx1;

	        ny1=di->nvdata[k1*3+1];
	        ndy1=di->nvdata[k2*3+1]-ny1;
	        ndy2=di->nvdata[k3*3+1]-ny1;

 	        nz1=di->nvdata[k1*3+2];
	        ndz1=di->nvdata[k2*3+2]-nz1;
	        ndz2=di->nvdata[k3*3+2]-nz1;

	    }
	    

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,bs -= xsize,vs -= xsize3){
		double den,xx,yy,aa,r,s;
	    b=bs;
	    v=vs;
		yy=iyy/rd->ody+rd->ymin;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,++b,v += 3){ 
	        {
			xx=ixx/rd->odx+rd->xmin;
	        den=xx*nx+yy*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
		    if((z=-aa*rd->h+rd->h) < *b){
		        {   
		            double x,y,xx5,yy5;
		            long np0,np1;
		            int l;
		            long np;
			    
			    xx5=xx*aa;	
			    yy5=yy*aa;
			    x=(xx5-x1)*v1[0]+(yy5-y1)*v1[1]+(z-z1)*v1[2];
			    y=(xx5-x1)*v2[0]+(yy5-y1)*v2[1]+(z-z1)*v2[2];
			    np1=ns;
			    np0=ns+nc-1;

			    np=nc;

			    l = 0 ;	
			    while (np--) {
			        double xxx0,yyy0;
			        double xxx1,yyy1;
			    	xxx0=(di->xdata[di->ndata[np0]]-x1)*v1[0]+(di->ydata[di->ndata[np0]]-y1)*v1[1]+
			    	   (di->zdata[di->ndata[np0]]-z1)*v1[2];
			    	xxx1=(di->xdata[di->ndata[np1]]-x1)*v1[0]+(di->ydata[di->ndata[np1]]-y1)*v1[1]+
			    	   (di->zdata[di->ndata[np1]]-z1)*v1[2];
			    	yyy0=(di->xdata[di->ndata[np0]]-x1)*v2[0]+(di->ydata[di->ndata[np0]]-y1)*v2[1]+
			    	   (di->zdata[di->ndata[np0]]-z1)*v2[2];
			    	yyy1=(di->xdata[di->ndata[np1]]-x1)*v2[0]+(di->ydata[di->ndata[np1]]-y1)*v2[1]+
			    	   (di->zdata[di->ndata[np1]]-z1)*v2[2];
					if((int)((int)(yyy0 > y) ^ (int)(yyy1 > y))){
					    if(xxx0 > x && xxx1 > x){
				            ++l;
					    }else if (xxx0 > x || xxx1 > x){
				            l += (int)(yyy1 > yyy0) ^
				                 (int)((yyy0 - y)*(xxx1 - x) >
				                       (xxx0 - x)*(yyy1 - y));
					    }
					}
					np0  = np1;
					++np1;
			    }
			    if ((l % 2) == 0)continue;
		        }
	    		r=((xx*aa-x1)*dy2-(yy*aa-y1)*dx2)*odd;
	   			s=(dx1*(yy*aa-y1)-dy1*(xx*aa-x1))*odd;
/*
	    		r=0.5;
	   			s=0.5;
*/   		
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
		        
		        
		   		xplane=xx*aa;
		   		yplane=yy*aa;
			    
				MakeVector(xplane,yplane,z,P);
				
				if(scene->ClipPlane){
		    		VecSub(P,rd->ClipPlanePoint,D);
				     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
				         continue;
				     }
				}
/*		        

			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
*/

		    MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
			       nz1+r*ndz1+s*ndz2,N);
		        
		        
			if(MaterialType == VRML_BY_FACE){
			    mat=di->matdata[ncell];
			    if(vrmlMat){
			        if(mat >= vrmlMat->max)mat=0;
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;


			    m[0]=di->matdata[ns];
			    m[1]=di->matdata[ns+1];
			    m[2]=di->matdata[ns+2];

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                                s*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                  s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                  s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                              s*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                             s*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                               s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                               s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
			   if(warn++ < 2){
			        sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
			   }
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}
/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong) ;
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);
	        }


    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=di->CellCount+nn;
	        rd->opacity[b2]=(float)(surf.opacity);
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,bs -= xsize,vs -= xsize3){
		double den,xx,yy,aa,r,s;
	    b=bs;
	    v=vs;
		yy=iyy/rd->ody+rd->ymin;
	    for(ixx=ixmin;ixx<=ixmax;++ixx,++b,v += 3){ 
	        {
	       	if(zmax <= *b)continue;

			xx=ixx/rd->odx+rd->xmin;
	        den=xx*nx+yy*ny-rd->h*nz;
	        
	        if(den == 0)continue;
	        
	        aa=cn/den;
	      
	      	z=-aa*rd->h+rd->h;
	      
		    if(z > h99){
		       	continue;
		    }

		    if(z > *b){
		        {   
		            double x,y,xx5,yy5;
		            long np0,np1;
		            int l;
		            long np;
			    
			    xx5=xx*aa;	
			    yy5=yy*aa;
			    x=(xx5-x1)*v1[0]+(yy5-y1)*v1[1]+(z-z1)*v1[2];
			    y=(xx5-x1)*v2[0]+(yy5-y1)*v2[1]+(z-z1)*v2[2];
			    np1=ns;
			    np0=ns+nc-1;

			    np=nc;

			    l = 0 ;	
			    while (np--) {
			        double xxx0,yyy0;
			        double xxx1,yyy1;
			    	xxx0=(di->xdata[di->ndata[np0]]-x1)*v1[0]+(di->ydata[di->ndata[np0]]-y1)*v1[1]+
			    	   (di->zdata[di->ndata[np0]]-z1)*v1[2];
			    	xxx1=(di->xdata[di->ndata[np1]]-x1)*v1[0]+(di->ydata[di->ndata[np1]]-y1)*v1[1]+
			    	   (di->zdata[di->ndata[np1]]-z1)*v1[2];
			    	yyy0=(di->xdata[di->ndata[np0]]-x1)*v2[0]+(di->ydata[di->ndata[np0]]-y1)*v2[1]+
			    	   (di->zdata[di->ndata[np0]]-z1)*v2[2];
			    	yyy1=(di->xdata[di->ndata[np1]]-x1)*v2[0]+(di->ydata[di->ndata[np1]]-y1)*v2[1]+
			    	   (di->zdata[di->ndata[np1]]-z1)*v2[2];
					if((int)((int)(yyy0 > y) ^ (int)(yyy1 > y))){
					    if(xxx0 > x && xxx1 > x){
				            ++l;
					    }else if (xxx0 > x || xxx1 > x){
				            l += (int)(yyy1 > yyy0) ^
				                 (int)((yyy0 - y)*(xxx1 - x) >
				                       (xxx0 - x)*(yyy1 - y));
					    }
					}
					np0  = np1;
					++np1;
			    }
			    if ((l % 2) == 0)continue;
		        }
		        
		        
	    	r=((xx*aa-x1)*dy2-(yy*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy*aa-y1)-dy1*(xx*aa-x1))*odd;
/*		        
	    	r=0.5;
	   		s=0.5;
*/
		        
	   		xplane=xx*aa;
	   		yplane=yy*aa;
		    
			MakeVector(xplane,yplane,z,P);
			
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
/*		        
			MakeVector(ny,ny,nz,N);

			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
*/
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
			   
			   
			if(MaterialType == VRML_BY_FACE){
			    mat=di->matdata[ncell];
			    if(vrmlMat){
			        if(mat >= vrmlMat->max)mat=0;
			        surf_color[0]=vrmlMat->diffusedata[mat*3];
			        surf_color[1]=vrmlMat->diffusedata[mat*3+1];
			        surf_color[2]=vrmlMat->diffusedata[mat*3+2];

					surf.phong= 128*vrmlMat->shinedata[mat];

					specular[0]=vrmlMat->speculardata[mat*3];
					specular[1]=vrmlMat->speculardata[mat*3+1];
					specular[2]=vrmlMat->speculardata[mat*3+2];

					ambient[0]=vrmlMat->ambientdata[mat*3];
					ambient[1]=vrmlMat->ambientdata[mat*3+1];
					ambient[2]=vrmlMat->ambientdata[mat*3+2];

			        surf.flag=vrmlMat->flagsdata[mat];
			    }else{
			        surf_color[0]=.8f;
			        surf_color[1]=.8f;
			        surf_color[2]=.8f;
					ambient[0]=.2f;
					ambient[1]=.2f;
					ambient[2]=.2f;
			        surf.flag = M_AMBIENT;
			    }
			}else if(MaterialType == VRML_BY_VERTEX){
			    long m[3];
			    double *d;


			    m[0]=di->matdata[ns];
			    m[1]=di->matdata[ns+1];
			    m[2]=di->matdata[ns+2];

			    surf.flag=vrmlMat->flagsdata[m[0]];
			    surf.phong= 128*vrmlMat->shinedata[m[0]];

			    if(surf.flag & M_DIFFUSE){

			        d=vrmlMat->diffusedata;
			        surf_color[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                                s*(d[m[2]*3]-d[m[0]*3]);
			        surf_color[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                  s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        surf_color[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                  s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }else{
			        surf_color[0]=0;
			        surf_color[1]=0;
			        surf_color[2]=0;
			    }

			    if(surf.flag & M_SPECULAR){
			        d=vrmlMat->speculardata;

			        specular[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                              s*(d[m[2]*3]-d[m[0]*3]);
			        specular[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                                s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        specular[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                                s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }

			    if(surf.flag & M_AMBIENT){
			        d=vrmlMat->ambientdata;
			        ambient[0]=d[m[0]*3]+r*(d[m[1]*3]-d[m[0]*3])+
			                             s*(d[m[2]*3]-d[m[0]*3]);
			        ambient[1]=d[m[0]*3+1]+r*(d[m[1]*3+1]-d[m[0]*3+1])+
			                               s*(d[m[2]*3+1]-d[m[0]*3+1]);
			        ambient[2]=d[m[0]*3+2]+r*(d[m[1]*3+2]-d[m[0]*3+2])+
			                               s*(d[m[2]*3+2]-d[m[0]*3+2]);
			    }
			}else{
			    if(warn++ < 3){
			        sprintf(WarningBuff,"MaterialType != VRML_BY_FACE\n");
					WarningBatch(WarningBuff);
			    }
			    surf.flag=0;
			    surf_color[0]=0;
			    surf_color[1]=0;
			    surf_color[2]=0;
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf.flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)pow(-costheta2,surf.phong);			
			        colr=(spec*specular[0]);
			        colg=(spec*specular[1]);
			        colb=(spec*specular[2]);		        
			    }
	         }
	     }
/* end of insert */

	        if(surf.flag & M_AMBIENT){	 
	        
	           colr += (double)(surf_color[0]*ambient[0]);
	           colg += (double)(surf_color[1]*ambient[1]);
	           colb += (double)(surf_color[2]*ambient[2]);	
	        
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=di->CellCount+nn;
		    *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
	        }
	    }
	  }
    }
 
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
	if(SkipPoly && scene->debug > 2){
	    sprintf(WarningBuff,"Polygone skipped %ld\n",SkipPoly);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	Threads->done=TRUE;    
	return 0;
}
int doNoShadow4PBEST(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene)
{
		struct drawInfo di;
		
		zerol((char *)&di,sizeof(di));
		
		di.ndata=ndata;
		di.xdata=xdata;
		di.ydata=ydata;
		di.zdata=zdata;
		di.nvdata=nvdata;
		di.vdata=vdata;
		di.nel=nel;
		di.CellCount=CellCount;
		di.scene=scene;

		runThreads(scene->threads,&di,scene->yResolution,doNoShadow4PBESTT);
		
		return 0;
}               
static int doNoShadow4PBESTT(mThread *Threads)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over,ipass,ip;
	long n1,n2,n3,n4;
	long xsize3;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
    long *CellBuff,b2;
	Vec N,P,L,D;
	double nx1,ndx1,ndx2,ny1,ndy1,ndy2,nz1,ndz1,ndz2;
	double x1,dx1,dx2,y1,dy1,dy2,z1,dz1,dz2;
	double r1,dr1,dr2,s1,ds1,ds2;
	double len;
	double spec,colr,colg,colb;
	float *rdata;
	float *sdata;
	double small2 = (double)-1.0e-5;
	double  onep = 1.00001f;
	int ip2,ipass2;
	double h99;
	struct Scene *scene;
	struct drawInfo *di;
	
	if(!Threads)goto ErrorOut;
	di=(struct drawInfo *)Threads->data;
	if(!di)goto ErrorOut;
	scene=di->scene;
	if(!scene)goto ErrorOut;
	
	
	rdata=scene->rdata;
	sdata=scene->sdata;

	CellBuff=scene->CellBuff;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}


    Rastor8=(int)scene->Rastor8;

	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	h99=0.99*rd->h;
	for(nn=0;nn<di->nel;++nn){	    
		double sxx1,sxx2,sxx3,syy1,syy2,syy3,sp1z,sp2z,sp3z;
		double xx1,xx2,xx3,yy1,yy2,yy3;
		double nx,ny,nz,den,cn,dd,odd;
		double zmin,zmax;
	    nt=5*nn;
	    if((n1=di->ndata[nt+1]) < 0)continue;
	    if((n2=di->ndata[nt+2]) < 0 )continue;
	    if((n3=di->ndata[nt+3]) < 0)continue;
	    if((n4=di->ndata[nt+4]) < 0){
	        ipass=1;
	    }else{
	        ipass=2;
	    }
	  for(ip=0;ip<ipass;++ip){
	    if(ip == 0){
	        if(ipass == 1){
	            k1=n1;
	            k2=n2;
	            k3=n3;
	        }else{
	            k1=n1;
	            k2=n2;
	            k3=n4;
	        }
	    }else{
	        k1=n3;
	        k2=n4;
	        k3=n2;
	    }
	    
	    p1z=di->zdata[k1];
	    p2z=di->zdata[k2];
	    p3z=di->zdata[k3];
/*	    
	    if(CellCount+nn != 45)continue;
*/	    	    
		{
		    double ss;
			sxx1=xx1=di->xdata[k1];
			sxx2=xx2=di->xdata[k2];
			sxx3=xx3=di->xdata[k3];
			syy1=yy1=di->ydata[k1];
			syy2=yy2=di->ydata[k2];
			syy3=yy3=di->ydata[k3];
			sp1z=p1z;
			sp2z=p2z;
			sp3z=p3z;
        	ipass2=1;
	        if(p1z >= h99){
	            if(p2z >= h99){
	                if(p3z >= h99)continue; /* All */
	                /* p1 & p2 */
	            	ss=(h99-p3z)/(p1z-p3z);
	            	xx1=xx3+ss*(xx1-xx3);
	            	yy1=yy3+ss*(yy1-yy3);
	            	p1z=h99;
	            	
	            	ss=(h99-p3z)/(p2z-p3z);
	            	xx2=xx3+ss*(xx2-xx3);
	            	yy2=yy3+ss*(yy2-yy3);
	            	p2z=h99;
	            	
	            }else if(p3z >= h99){
	                /* p1 & p3 */
	            	ss=(h99-p2z)/(p1z-p2z);
	            	xx1=xx2+ss*(xx1-xx2);
	            	yy1=yy2+ss*(yy1-yy2);
	            	p1z=h99;
	            	
	            	ss=(h99-p2z)/(p3z-p2z);
	            	xx3=xx2+ss*(xx3-xx2);
	            	yy3=yy2+ss*(yy3-yy2);
	            	p3z=h99;
	            }else{
	                /* p1 should split */
	            	ss=(h99-p2z)/(p1z-p2z);
	            	xx1=xx2+ss*(xx1-xx2);
	            	yy1=yy2+ss*(yy1-yy2);
	            	p1z=h99;

	            	ss=(h99-sp3z)/(sp1z-sp3z);
	            	sxx3=sxx3+ss*(sxx1-sxx3);
	            	syy3=syy3+ss*(syy1-syy3);
	            	sp3z=h99;
	            	
		            sxx1=xx1;
	            	syy1=yy1;
	            	sp1z=h99;
	            		            	
	            	sxx2=xx3;
	            	syy2=yy3;
	            	sp2z=p3z;
	            		            	
            		            	
	                ipass2=2;
	            	
	            }
	        }else if(p2z >= h99){
	             if(p3z >= h99){
	                 /* p2 & p3 */
	            	ss=(h99-p1z)/(p2z-p1z);
	            	xx2=xx1+ss*(xx2-xx1);
	            	yy2=yy1+ss*(yy2-yy1);
	            	p2z=h99;

	            	ss=(h99-p1z)/(p3z-p1z);
	            	xx3=xx1+ss*(xx3-xx1);
	            	yy3=yy1+ss*(yy3-yy1);
	            	p3z=h99;
	             }else{
	                 /* p2 should split */
	            	ss=(h99-p1z)/(p2z-p1z);
	            	xx2=xx1+ss*(xx2-xx1);
	            	yy2=yy1+ss*(yy2-yy1);
	            	p2z=h99;
	            	
		            ss=(h99-sp3z)/(sp2z-sp3z);
	            	sxx2=sxx3+ss*(sxx2-sxx3);
	            	syy2=syy3+ss*(syy2-syy3);
	            	sp2z=h99;
	            	
	            	sxx1=xx2;
	            	syy1=yy2;
	            	sp1z=p2z;
	            	
	                ipass2=2;
	             }
	        }else if(p3z >= h99){
	            /* p3 should split */
	            ss=(h99-p1z)/(p3z-p1z);
	            xx3=xx1+ss*(xx3-xx1);
	            yy3=yy1+ss*(yy3-yy1);
	            p3z=h99;
	            
		        ss=(h99-sp2z)/(sp3z-sp2z);
	            sxx3=sxx2+ss*(sxx3-sxx2);
	            syy3=syy2+ss*(syy3-syy2);
	            sp3z=h99;
	            
	            sxx1=xx3;
	            syy1=yy3;
	            sp1z=h99;
	            
	            ipass2=2;
	        }
	        
		}

		
        for(ip2=0;ip2<ipass2;++ip2){
        
        if(ip2 == 1){
			xx1=sxx1;
			xx2=sxx2;
			xx3=sxx3;
			yy1=syy1;
			yy2=syy2;
			yy3=syy3;
			p1z=sp1z;
			p2z=sp2z;
			p3z=sp3z;
        }

  	    ztran1 = rd->h/(rd->h-p1z);
	    ztran2 = rd->h/(rd->h-p2z);
	    ztran3 = rd->h/(rd->h-p3z);
	    p1y=(ztran1*yy1-rd->ymin)*rd->ody;
	    p2y=(ztran2*yy2-rd->ymin)*rd->ody;
	    p3y=(ztran3*yy3-rd->ymin)*rd->ody;
	    
	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
		
	    if((iymax == iymin) || (iymax < Threads->smin) || (iymin >= Threads->smax)){
	        ++CellSkip;
	        continue;
	    }

	    if(iymin < Threads->smin)iymin=Threads->smin;
	    if(iymax >= Threads->smax)iymax=Threads->smax-1;
	    	    	    
	    p1x=(rd->o+ztran1*(xx1-rd->o)-rd->xmin)*rd->odx;	    
	    p2x=(rd->o+ztran2*(xx2-rd->o)-rd->xmin)*rd->odx;
	    p3x=(rd->o+ztran3*(xx3-rd->o)-rd->xmin)*rd->odx;
		
	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }
		
	    zmin=zmax=p1z;
	    if(p1z > p2z){
	        zmin=p2z;
	    }else{
	        zmax=p2z;
	    }
	    
	    if(p3z > zmax)zmax=p3z;
	    
	    if(p3z < zmin)zmin=p3z;
	    
	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;
	    
	    material=di->ndata[nt];
	    surf = &scene->material[material];
	    if(di->vdata){
			value1=di->vdata[k1];
	        dvalue1=di->vdata[k2]-value1;
	        dvalue2=di->vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	    if(rdata){
			r1=rdata[k1];
	        dr1=rdata[k2]-r1;
	        dr2=rdata[k3]-r1;
        }else{
			r1=0;
	        dr1=0;
			dr2=0;
	    }
	    if(sdata){
			s1=sdata[k1];
	        ds1=sdata[k2]-s1;
	        ds2=sdata[k3]-s1;
        }else{
			s1=0;
	        ds1=0;
			ds2=0;
	    }
	    

        nx1=di->nvdata[k1*3];
        ndx1=di->nvdata[k2*3]-nx1;
        ndx2=di->nvdata[k3*3]-nx1;

        ny1=di->nvdata[k1*3+1];
        ndy1=di->nvdata[k2*3+1]-ny1;
        ndy2=di->nvdata[k3*3+1]-ny1;

        nz1=di->nvdata[k1*3+2];
        ndz1=di->nvdata[k2*3+2]-nz1;
        ndz2=di->nvdata[k3*3+2]-nz1;

 	    x1=di->xdata[k1];
	    dx1=di->xdata[k2]-x1;
	    dx2=di->xdata[k3]-x1;

	    y1=di->ydata[k1];
	    dy1=di->ydata[k2]-y1;
	    dy2=di->ydata[k3]-y1;

 	    z1=di->zdata[k1];
	    dz1=di->zdata[k2]-z1;
	    dz2=di->zdata[k3]-z1;
	    
	    
		if(!(dd=(dx1)*(dy2)-(dy1)*(dx2))){
			continue;
		}

		odd=1.0/dd;
	    
	    nx=dy1*dz2-dy2*dz1;
	    ny=dx2*dz1-dx1*dz2;
	    nz=dx1*dy2-dx2*dy1;
	    den=sqrt(nx*nx+ny*ny+nz*nz);
	    if(den == 0)continue;
	    
	    nx /= den;
	    ny /= den;
	    nz /= den;
	    
	    cn=-(-nx*x1-ny*y1+(rd->h-z1)*nz);	    

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
		double den,xx,yy;
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
		
		yy=iyy/rd->ody+rd->ymin;

	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        double r,s,aa;

	        if(zmin >= *b)continue;
	        
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
           
			xx=ixx/rd->odx+rd->xmin;
	        den=xx*nx+yy*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
	        	        	           		       
		    if((z=-aa*rd->h+rd->h) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
		        
	    	r=((xx*aa-x1)*dy2-(yy*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy*aa-y1)-dy1*(xx*aa-x1))*odd;
	   		
	   		if(r < 0 || s < 0 || (r+s) > 1 )continue;
	   		
			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
				   
			if(di->vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+r*dvalue1+s*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth2;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth2;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+r*dr1+s*dr2;
			    hit.f2=s1+r*ds1+s*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0f;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=di->CellCount+nn;
	        rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
		double den,xx,yy2=0.0;
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    
		yy2=iyy/rd->ody+rd->ymin;
		
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        double r,s,aa;
        
	        if(zmax <= *b)continue;
	        
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
           

			xx=ixx/rd->odx+rd->xmin;

	        den=xx*nx+yy2*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
	        	        	           
		    if((z=-aa*rd->h+rd->h) > *b){

	    	r=((xx*aa-x1)*dy2-(yy2*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy2*aa-y1)-dy1*(xx*aa-x1))*odd;
	   		
	   		if(r < 0 || s < 0 || (r+s) > 1 )continue;
	    
			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
				   
 			if(di->vdata && (surf->flag & M_COLORLEVEL)){
			    cvalue=value1+r*dvalue1+s*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+r*dr1+s*dr2;
			    hit.f2=s1+r*ds1+s*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=di->CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	 }
	 }
	    
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	Threads->done=TRUE;    
	return 0;
}
int doNoShadowPBEST(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
                     float *nvdata,long nel,long CellCount,struct Scene *scene)
{
		struct drawInfo di;
		
		zerol((char *)&di,sizeof(di));
		
		di.ndata=ndata;
		di.xdata=xdata;
		di.ydata=ydata;
		di.zdata=zdata;
		di.nvdata=nvdata;
		di.vdata=vdata;
		di.nel=nel;
		di.CellCount=CellCount;
		di.scene=scene;

		runThreads(scene->threads,&di,scene->yResolution,doNoShadowPBESTT);
		
		return 0;
}               
                 
static int doNoShadowPBESTT(mThread *Threads)
{
	double v1x,v1y,v2x,v2y,s1y,s2y,s1x,s2x,d1x,d1y,d2x,d2y;
	double *bs;
	long ixx,iyy;
	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,ztran1,ztran2,ztran3;
	double d,od;
	double value;
	int ixmax,ixmin,iymax,iymin,ix,iy,itIn,xsize,ysize,xsizen,ysizen;
	double z,*b,intensity;
	float *v,*vs;
	long nn,k1,k2,k3,count,over;
	long n1,n2,n3;
	long xsize3;
	int rastorY,ic,Rastor8;
	
	double cvalue,value1,dvalue1,dvalue2;
	struct Material *surf;
	ColorVEC	surf_color;
	long material,nt,CellSkip;
    long *CellBuff,b2;
	Vec N,P,L,D;
	double nx1,ndx1,ndx2,ny1,ndy1,ndy2,nz1,ndz1,ndz2;
	double x1,dx1,dx2,y1,dy1,dy2,z1,dz1,dz2;
	double r1,dr1,dr2,s1,ds1,ds2;
	double len;
	double spec,colr,colg,colb;
	float *rdata;
	float *sdata;
	static double small2 = (double)-1.0e-5;
	static double  onep = 1.00001f;
	int ip2,ipass2;
	double h99;
	struct Scene *scene;
	struct drawInfo *di;
	
	if(!Threads)goto ErrorOut;
	di=(struct drawInfo *)Threads->data;
	if(!di)goto ErrorOut;
	scene=di->scene;
	if(!scene)goto ErrorOut;

	rdata=scene->rdata;
	sdata=scene->sdata;

	CellBuff=scene->CellBuff;

	if(scene->material){
	    intensity=(scene->material[scene->lamp.material].intensity);
	    if(intensity <= 0.0)intensity=1.0;
	}else{
	    intensity=1.0;
	}


	Rastor8=(int)scene->Rastor8;

	xsize=(int)scene->xResolution;
	xsizen=xsize-1;
	ysize=(int)scene->yResolution;
	ysizen=ysize-1;
	xsize3=3*xsize;
	rastorY=(int)scene->yResolution-1;
	over=0;
	count=0;
	CellSkip=0;
	h99=0.99*rd->h;
	for(nn=0;nn<di->nel;++nn){	    
		double sxx1,sxx2,sxx3,syy1,syy2,syy3,sp1z,sp2z,sp3z;
		double xx1,xx2,xx3,yy1,yy2,yy3;
		double nx,ny,nz,den,cn,dd,odd;
		double zmin,zmax;
	    nt=4*nn;
	    if((n1=di->ndata[nt+1]) < 0)continue;
	    if((n2=di->ndata[nt+2]) < 0 )continue;
	    if((n3=di->ndata[nt+3]) < 0)continue;
	  {
        k1=n1;
        k2=n2;
        k3=n3;
	            
	    p1z=di->zdata[k1];
	    p2z=di->zdata[k2];
	    p3z=di->zdata[k3];

		{
		    double ss;
			sxx1=xx1=di->xdata[k1];
			sxx2=xx2=di->xdata[k2];
			sxx3=xx3=di->xdata[k3];
			syy1=yy1=di->ydata[k1];
			syy2=yy2=di->ydata[k2];
			syy3=yy3=di->ydata[k3];
			sp1z=p1z;
			sp2z=p2z;
			sp3z=p3z;
        	ipass2=1;
	        if(p1z >= h99){
	            if(p2z >= h99){
	                if(p3z >= h99)continue; /* All */
	                /* p1 & p2 */
	            	ss=(h99-p3z)/(p1z-p3z);
	            	xx1=xx3+ss*(xx1-xx3);
	            	yy1=yy3+ss*(yy1-yy3);
	            	p1z=h99;
	            	
	            	ss=(h99-p3z)/(p2z-p3z);
	            	xx2=xx3+ss*(xx2-xx3);
	            	yy2=yy3+ss*(yy2-yy3);
	            	p2z=h99;
	            	
	            }else if(p3z >= h99){
	                /* p1 & p3 */
	            	ss=(h99-p2z)/(p1z-p2z);
	            	xx1=xx2+ss*(xx1-xx2);
	            	yy1=yy2+ss*(yy1-yy2);
	            	p1z=h99;
	            	
	            	ss=(h99-p2z)/(p3z-p2z);
	            	xx3=xx2+ss*(xx3-xx2);
	            	yy3=yy2+ss*(yy3-yy2);
	            	p3z=h99;
	            }else{
	                /* p1 should split */
	            	ss=(h99-p2z)/(p1z-p2z);
	            	xx1=xx2+ss*(xx1-xx2);
	            	yy1=yy2+ss*(yy1-yy2);
	            	p1z=h99;

	            	ss=(h99-sp3z)/(sp1z-sp3z);
	            	sxx3=sxx3+ss*(sxx1-sxx3);
	            	syy3=syy3+ss*(syy1-syy3);
	            	sp3z=h99;
	            	
		            sxx1=xx1;
	            	syy1=yy1;
	            	sp1z=h99;
	            		            	
	            	sxx2=xx3;
	            	syy2=yy3;
	            	sp2z=p3z;
	            		            	
            		            	
	                ipass2=2;
	            	
	            }
	        }else if(p2z >= h99){
	             if(p3z >= h99){
	                 /* p2 & p3 */
	            	ss=(h99-p1z)/(p2z-p1z);
	            	xx2=xx1+ss*(xx2-xx1);
	            	yy2=yy1+ss*(yy2-yy1);
	            	p2z=h99;

	            	ss=(h99-p1z)/(p3z-p1z);
	            	xx3=xx1+ss*(xx3-xx1);
	            	yy3=yy1+ss*(yy3-yy1);
	            	p3z=h99;
	             }else{
	                 /* p2 should split */
	            	ss=(h99-p1z)/(p2z-p1z);
	            	xx2=xx1+ss*(xx2-xx1);
	            	yy2=yy1+ss*(yy2-yy1);
	            	p2z=h99;
	            	
		            ss=(h99-sp3z)/(sp2z-sp3z);
	            	sxx2=sxx3+ss*(sxx2-sxx3);
	            	syy2=syy3+ss*(syy2-syy3);
	            	sp2z=h99;
	            	
	            	sxx1=xx2;
	            	syy1=yy2;
	            	sp1z=p2z;
	            	
	                ipass2=2;
	             }
	        }else if(p3z >= h99){
	            /* p3 should split  */
	            ss=(h99-p1z)/(p3z-p1z);
	            xx3=xx1+ss*(xx3-xx1);
	            yy3=yy1+ss*(yy3-yy1);
	            p3z=h99;
	            
		        ss=(h99-sp2z)/(sp3z-sp2z);
	            sxx3=sxx2+ss*(sxx3-sxx2);
	            syy3=syy2+ss*(syy3-syy2);
	            sp3z=h99;
	            
	            sxx1=xx3;
	            syy1=yy3;
	            sp1z=h99;
	            
	            ipass2=2;
	        }
	        
		}

		
        for(ip2=0;ip2<ipass2;++ip2){
        
        if(ip2 == 1){
			xx1=sxx1;
			xx2=sxx2;
			xx3=sxx3;
			yy1=syy1;
			yy2=syy2;
			yy3=syy3;
			p1z=sp1z;
			p2z=sp2z;
			p3z=sp3z;
        }

  	    ztran1 = rd->h/(rd->h-p1z);
	    ztran2 = rd->h/(rd->h-p2z);
	    ztran3 = rd->h/(rd->h-p3z);
	    p1y=(ztran1*yy1-rd->ymin)*rd->ody;
	    p2y=(ztran2*yy2-rd->ymin)*rd->ody;
	    p3y=(ztran3*yy3-rd->ymin)*rd->ody;
	    
	    iy = (int)p1y;
	    iymax = iy;
	    iymin = iy;
	    	    
	    
	    iy = (int)p2y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    
	    iy = (int)p3y;
	    if(iy > iymax){
	        iymax = iy;
	    }else if(iy < iymin){
	        iymin = iy;
	    }
	    
	    if((iymax == iymin) || (iymax < Threads->smin) || (iymin >= Threads->smax)){
	        ++CellSkip;
	        continue;
	    }
	    
	    if(iymin < Threads->smin)iymin=Threads->smin;
	    if(iymax >= Threads->smax)iymax=Threads->smax-1;
	    	    
	    p1x=(rd->o+ztran1*(xx1-rd->o)-rd->xmin)*rd->odx;	    
	    p2x=(rd->o+ztran2*(xx2-rd->o)-rd->xmin)*rd->odx;
	    p3x=(rd->o+ztran3*(xx3-rd->o)-rd->xmin)*rd->odx;	    
	    
	    
	    zmin=zmax=p1z;
	    if(p1z > p2z){
	        zmin=p2z;
	    }else{
	        zmax=p2z;
	    }
	    
	    if(p3z > zmax)zmax=p3z;
	    
	    if(p3z < zmin)zmin=p3z;
		
	    ix = (int)p1x;
	    ixmax = ix;
	    ixmin = ix;
	    ix = (int)p2x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }
	    ix = (int)p3x;
	    if(ix > ixmax){
	        ixmax = ix;
	    }else if(ix < ixmin){
	        ixmin = ix;
	    }

	    if((ixmax == ixmin) || (ixmax < 0) || (ixmin > xsizen)){
	        ++CellSkip;
	        continue;
	    }



	    v1x=p2x-p1x;
	    v1y=p2y-p1y;
	    v2x=p3x-p1x;
	    v2y=p3y-p1y;

	    if(!(d=(v1x)*(v2y)-(v1y)*(v2x))){
	        continue;
	    }
	    od=1.f/d;
	    
	    material=di->ndata[nt];
	    surf = &scene->material[material];
	    if(di->vdata){
			value1=di->vdata[k1];
	        dvalue1=di->vdata[k2]-value1;
	        dvalue2=di->vdata[k3]-value1;
        }else{
			value1=0;
	        dvalue1=0;
			dvalue2=0;
	    }

	    if(rdata){
			r1=rdata[k1];
	        dr1=rdata[k2]-r1;
	        dr2=rdata[k3]-r1;
        }else{
			r1=0;
	        dr1=0;
			dr2=0;
	    }
	    if(sdata){
			s1=sdata[k1];
	        ds1=sdata[k2]-s1;
	        ds2=sdata[k3]-s1;
        }else{
			s1=0;
	        ds1=0;
			ds2=0;
	    }
	    

        nx1=di->nvdata[k1*3];
        ndx1=di->nvdata[k2*3]-nx1;
        ndx2=di->nvdata[k3*3]-nx1;

        ny1=di->nvdata[k1*3+1];
        ndy1=di->nvdata[k2*3+1]-ny1;
        ndy2=di->nvdata[k3*3+1]-ny1;

        nz1=di->nvdata[k1*3+2];
        ndz1=di->nvdata[k2*3+2]-nz1;
        ndz2=di->nvdata[k3*3+2]-nz1;

 	    x1=di->xdata[k1];
	    dx1=di->xdata[k2]-x1;
	    dx2=di->xdata[k3]-x1;

	    y1=di->ydata[k1];
	    dy1=di->ydata[k2]-y1;
	    dy2=di->ydata[k3]-y1;

 	    z1=di->zdata[k1];
	    dz1=di->zdata[k2]-z1;
	    dz2=di->zdata[k3]-z1;
	    
	    
		if(!(dd=(dx1)*(dy2)-(dy1)*(dx2))){
			continue;
		}

		odd=1.0/dd;
	    
	    nx=dy1*dz2-dy2*dz1;
	    ny=dx2*dz1-dx1*dz2;
	    nz=dx1*dy2-dx2*dy1;
	    den=sqrt(nx*nx+ny*ny+nz*nz);
	    if(den == 0)continue;
	    
	    nx /= den;
	    ny /= den;
	    nz /= den;
	    
	    cn=-(-nx*x1-ny*y1+(rd->h-z1)*nz);	    

	    if(ixmin < 0)ixmin=0;
	    if(iymin < 0)iymin=0;
	    if(ixmax > xsizen)ixmax=xsizen;
	    if(iymax > ysizen)iymax=ysizen;

	    s1y=((ixmin-p1x)*v2y-(iymin-p1y)*v2x)*od;
	    s2y=(v1x*(iymin-p1y)-v1y*(ixmin-p1x))*od;
	    d1x=  v2y*od;
	    d1y= -v2x*od;
	    d2x= -v1y*od;
	    d2y=  v1x*od;

    if(rd->doTransparency){
	  bs=(double *)rd->zbuff2+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff2+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
		double den,xx,yy;
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
		
		yy=iyy/rd->ody+rd->ymin;

	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        double r,s,aa;

	        if(zmin >= *b)continue;
	        
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
           
			xx=ixx/rd->odx+rd->xmin;
	        den=xx*nx+yy*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
	        	        	           		       
		    if((z=-aa*rd->h+rd->h) < *b){
		        b2=b-rd->zbuff2;
		        if(z <= rd->zbuff[b2])continue;
		        
	    	r=((xx*aa-x1)*dy2-(yy*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy*aa-y1)-dy1*(xx*aa-x1))*odd;
	   		
	   		if(r < 0 || s < 0 || (r+s) > 1 )continue;
	   		
			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
				   
			if(di->vdata && surf->flag & M_COLORLEVEL){
			    cvalue=value1+r*dvalue1+s*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth2;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth2;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+r*dr1+s*dr2;
			    hit.f2=s1+r*ds1+s*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.;
	     colr=colg=colb=0.0f;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        rd->cellbuff2[b2]=di->CellCount+nn;
	        rd->opacity[b2]=(float)surf->opacity;
SetDepth2:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }else{
	  bs=(double *)rd->zbuff+(rastorY-iymin)*xsize+(ixmin);
	  vs=(float *)rd->vbuff+(rastorY-iymin)*xsize3+(ixmin*3);
	  for(iyy=iymin;iyy<=iymax;++iyy,s1y += d1y,s2y += d2y,bs -= xsize,vs -= xsize3){
		double den,xx,yy1=0.0;
		
	    b=bs;
	    v=vs;
	    s1x=s1y;
	    s2x=s2y;
	    itIn=FALSE;
	    
		yy1=iyy/rd->ody+rd->ymin;
		
	    for(ixx=ixmin;ixx<=ixmax;++ixx,s1x += d1x,s2x += d2x,++b,v += 3){ 
	        double r,s,aa;
        
	        if(zmax <= *b)continue;
	        
	        if((s1x >= small2) && (s2x >= small2) &&
	           ((s1x+s2x) <= onep)){
           

			xx=ixx/rd->odx+rd->xmin;

	        den=xx*nx+yy1*ny-rd->h*nz;
	        if(den == 0)continue;
	        
	        aa=cn/den;
	        	        	           
		    if((z=-aa*rd->h+rd->h) > *b){

	    	r=((xx*aa-x1)*dy2-(yy1*aa-y1)*dx2)*odd;
	   		s=(dx1*(yy1*aa-y1)-dy1*(xx*aa-x1))*odd;
	   		
	   		if(r < 0 || s < 0 || (r+s) > 1 )continue;
	    
			MakeVector(x1+r*dx1+s*dx2,y1+r*dy1+s*dy2,
				   z1+r*dz1+s*dz2,P);
			if(scene->ClipPlane){
	    		VecSub(P,rd->ClipPlanePoint,D);
			     if((VecDot(D,rd->ClipPlaneNormal)) > 0.0){
			         continue;
			     }
			}
			
			MakeVector(nx1+r*ndx1+s*ndx2,ny1+r*ndy1+s*ndy2,
				   nz1+r*ndz1+s*ndz2,N);
				   
 			if(di->vdata && (surf->flag & M_COLORLEVEL)){
			    cvalue=value1+r*dvalue1+s*dvalue2;
			    colorit(surf->ColorCount,surf->ColorLevel,
			    cvalue,&ic);
			    if(Rastor8){
					*v=(float)ic;
					goto SetDepth;
			    }else{
			        ColorToVec(surf->ColorValue[ic],surf_color);
			    }
			}else{
                if(Rastor8){
                    *v=(float)surf->diffuse.red;
                    goto SetDepth;
                }else{
                    ColorToVec(surf->diffuse,surf_color);
                }
			}
			if(surf->flag & M_PATTERN){
			    Isect hit;

			    hit.f1=r1+r*dr1+s*dr2;
			    hit.f2=s1+r*ds1+s*ds2;
			    (*surf->PatternRoutine)(surf,&hit,surf_color);
			}

/*     insert */

	/* Vector Eye To Point */

	    VecSub(P,rd->Eyel,D);

	   
	/* Check Direction To Eye Point */

	     if((VecDot(D,N)) > 0.0){
	         VecNegate(N);
	     }

	/*  Vector from Point to Lamp */

	    VecSub(rd->Lampl,P,L);

	/* Check Direction of Lamp */


	     value = 0.0;
	     colr=colg=colb=0.0;
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
	         value=VecDot(N,L);
	         if(surf->flag & M_SPECULAR){
	            double costheta2;
			    Vec H;
			    VecComb((-1.),L,(2.*value),N,H);
			    if((len=VecLen(H)) > 0.0){
					H[0] /= len;
					H[1] /= len;
					H[2] /= len;
			    }
			    if((len=VecLen(D)) > 0.0){
					D[0] /= len;
					D[1] /= len;
					D[2] /= len;
			    }
			    costheta2=VecDot(H,D);
			    if(costheta2 < 0.0){
					spec = (double)(surf->specular*pow(-costheta2,surf->phong) );
			        colr=colg=colb=spec;
			    }
	         }
	     }
/* end of insert */

	        if(surf->flag & M_AMBIENT){
	           colr += (double)(surf_color[0]*surf->ambient.red);
	           colg += (double)(surf_color[1]*surf->ambient.green);
	           colb += (double)(surf_color[2]*surf->ambient.blue);
	        }
    		value  *= intensity;
	        *v=(float)(value*surf_color[0]+colr);
	        *(v+1)=(float)(value*surf_color[1]+colg);
	        *(v+2)=(float)(value*surf_color[2]+colb);
	        CellBuff[b-rd->zbuff]=di->CellCount+nn;
SetDepth:
		        *b=z;
		    }else{
		        ++over;
		    }
		    ++count;
		    itIn=TRUE;
	        }else {
	            if(itIn)break;
	        }
	    }
	  }
    }
	 }
	 }
	    
	}
	if(scene->debug > 2){
	    sprintf(WarningBuff,"Total Image Points found %ld Overlap %ld\n",count,over);
		WarningBatch(WarningBuff);
	}
ErrorOut:
	Threads->done=TRUE;    
	return 0;
}
