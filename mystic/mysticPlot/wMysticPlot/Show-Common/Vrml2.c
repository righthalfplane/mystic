#define EXTERN22 extern
#include <math.h>
#include "Xappl.h"
#include "TracerDef.h"
#include "Vrml.h"
#include "uFiles.h"
#include "Render.h"
#include "Defs.h"
#include "uDialog.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Object3d.h"
#include "Universal.h"

int Transform(struct P *p,struct Matrix *m);

void ScaleMatrix2(struct Matrix *m,struct System *p);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
void CObjectCreate(struct Scene *scene,CObject *b);
int ListAddObjectList(CObjectList *d,CObject *Object);
int CObjectkill(CObject *o);


static CVrmlListPtr CVrmlListCreate(struct Scene *scene,struct FileInfo2 *Files);

static CObjectPtr CVrmlListDuplicate(CObject *b);

static int CVrmlListKill(CObject *o);

static int CVrmlListMessage(CObject *b,struct ObjectMessage *Message);

static int CVrmlListLoad(CVrmlListPtr s,long itIs);

struct vrmlList *GetVRML(char *FilesNames,char *directory);

static CVrmlPtr2 CVrmlCreate2(struct Scene *scene,long vrmlNumber);

static CObjectPtr CVrmlDuplicate(CObject *b);

static int CVrmlDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);


static int CVrmlDrawObjects(struct vrmlObjects *o,struct tState *tS);



static int CVrmlCount(CObject *o,long *nodes,long *cells);

static int CVrmlMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
		      
		 
static int MergeVrmlList(struct Scene *scene,struct vrmlList *list);

static int drawTranslationTransform(struct vrmlTranslation *mt,struct tState *tS);
static int drawMatrixTransform(struct vrmlMatrixTransform *mt,struct tState *tS);
static int drawScaleTransform(struct vrmlScale *mt,struct tState *tS);
static int drawRotationTransform(struct vrmlRotation *mt,struct tState *tS);
static int drawTransFormTransform(struct vrmlTransForm *mt,struct tState *tS);

static int ObjectListAddVrml(struct Scene *scene,long k);
static int CVrmlRangeObjects(struct vrmlObjects *o,struct tState *tS,struct ObjectMessage *Message);
static int CVrmlMessage(CObject *b,struct ObjectMessage *Message);
static int drawIndexedFaceSet(struct vrmlIndexedFaceSet *s,struct tState *tS);
static int drawSphere(struct vrmlSphere *s,struct tState *tS);
static int drawDisk(struct vrmlDisk *d,struct tState *tS);
static int drawCylinder(struct vrmlCylinder *c,struct tState *tS);
static int drawCube(struct vrmlCube *d,struct tState *tS);
static int drawCone(struct vrmlCone *c,struct tState *tS);

CObjectPtr uVrmlListWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);

static int MergeVrmlList(struct Scene *scene,struct vrmlList *list)
{
         struct vrmlList *vrml;
         unsigned long length;
	
	if(scene->VrmlCount){
	    length=(scene->VrmlCount+1)*sizeof(struct vrmlList);
	    if(!(vrml=(struct vrmlList *)cRealloc((char *)scene->vrml,length,8788))){
	        sprintf(WarningBuff,"MergeVrmlList Out of memory Requested (%ld)\n",length);
		 	WarningBatch(WarningBuff);
	        return 1;
	    }
	    vrml[scene->VrmlCount++] = *list;
	    scene->vrml = vrml;
	}else{
	    if(!(vrml=(struct vrmlList *)cMalloc((long)sizeof(struct vrmlList),8574))){
	        sprintf(WarningBuff,"MergeVrmlList Out of memory Requested (%ld)\n",(long)sizeof(struct vrmlList));
			WarningBatch(WarningBuff);
	        return 1;
	    }
	    scene->VrmlCount=1;
	    scene->vrml=vrml;
	    *vrml = *list;
	}
	return 0;
}
CObjectPtr uVrmlListWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CVrmlListStruct b;
	
	static struct universalTableListStruct Local[] = {

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

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CVrmlListPtr bp=(CVrmlListPtr)o;
	char head[5];
	long length;
	void *op;
	long tag;
	
	
	if(!inOut || !l)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CVrmlListCreate(l->scene,b.Files);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		CVrmlListLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	op=&b;
	return op;
ErrorOut:
	return NULL;
}
static CVrmlListPtr CVrmlListCreate(struct Scene *scene,struct FileInfo2 *Files)
{
	struct vrmlList list;
	CVrmlListPtr s;

	if(!scene)return (CVrmlListPtr)NULL;

	zerol((char *)&list,sizeof(struct vrmlList));
	if(MergeVrmlList(scene,&list))return (CVrmlListPtr)NULL;

	s=(CVrmlListPtr)cMalloc(sizeof(CVrmlList),8575);
	if(!s)return (CVrmlListPtr)NULL;
	zerol((char *)s,sizeof(CVrmlList));

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_VRML_LIST2;
	s->Files=Files;
	s->duplicateObject=CVrmlListDuplicate;
	s->killObject=CVrmlListKill;
	s->message=CVrmlListMessage;
	s->ReadWrite=uVrmlListWrite;	
	s->vscale=1.0;
	s->Angles=20;
	s->Axis=0;
	s->ImageCount=Files->FileCount;
	s->tmax=Files->FileCount;
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	s->Material=1;
	s->Angle=180;

	s->VrmlNumber=scene->VrmlCount-1;

	s->vrmlNumbers=(long *)cMalloc(sizeof(long),8576);
	if(!s->vrmlNumbers){
	    sprintf(WarningBuff,"CVrmlListCreate out of Memory\n");
		WarningBatch(WarningBuff);
	    cFree((char *)s);
	    return (CVrmlListPtr)NULL;
	}

	s->vrmlCount=1;
	s->vrmlNumbers[0]=s->VrmlNumber;

	if(ObjectListAddVrml(scene,s->VrmlNumber))return (CVrmlListPtr)NULL;

	return s;
}
static int CVrmlListMessage(CObject *b,struct ObjectMessage *Message)
{
	CVrmlListPtr s=(CVrmlListPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CVrmlListLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  VRML File\n%c",0);
	    	WarningBatch(WarningBuff);
		    if(s->Files && (s->ItWas < s->Files->FileCount) && s->Files->FilesNames && s->Files->FilesNames[s->ItWas]){
	    		sprintf(WarningBuff,"  VRML Image Currectly Selected '%s'\n%c",s->Files->FilesNames[s->ItWas],0);
		    }else{
	    		sprintf(WarningBuff,"  VRML Image Currectly Selected %ld\n%c",s->ItWas,0);
	    	}
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    break;
		}
	    
	}
	return 0;
}
static int CVrmlListLoad(CVrmlListPtr s,long itIs)
{
	struct vrmlList *vrml;
	struct Scene *scene;
	long n;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if(s->ItWas == itIs)return 1;


	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    for(n=0;n<s->vrmlCount;++n){
	       if(s->vrmlNumbers){
		    freeVrmlList(&scene->vrml[s->vrmlNumbers[n]]);
		    zerol((char *)&scene->vrml[s->vrmlNumbers[n]],sizeof(struct vrmlList));
	       }
	    }
	    return 0;
	}

	if(itIs < 0 || itIs >= s->Files->FileCount){
	    itIs=0;
	}

	if(s->ItWas == itIs)return 1;

	for(n=0;n<s->vrmlCount;++n){
	   if(s->vrmlNumbers){
		freeVrmlList(&scene->vrml[s->vrmlNumbers[n]]);
		zerol((char *)&scene->vrml[s->vrmlNumbers[n]],sizeof(struct vrmlList));
	   }
	}


	vrml=GetVRML(s->Files->FilesNames[itIs],s->Files->directory);
	if(vrml == NULL){
	    sprintf(WarningBuff,"GetVRML failed in CVrmlListLoad\n");
		WarningBatch(WarningBuff);
	    return 1;
	}

	scene->vrml[s->vrmlNumbers[0]] = *vrml;

	cFree((char *)vrml);

	s->ItWas = itIs;
	return 0;
}

static int CVrmlListKill(CObject *o)
{
	CVrmlListPtr s=(CVrmlListPtr)o;

	if(!s)return 1;

	if(s->vrmlNumbers)cFree((char *)s->vrmlNumbers);
	s->vrmlNumbers=NULL;
	s->vrmlCount=0;

	if(s->scene && s->scene->Closing){
	    freeFileList(s->Files);
	}

	cFree((char *)s);
	return 0;
}
static CObjectPtr CVrmlListDuplicate(CObject *b)
{
	CVrmlListPtr s=(CVrmlListPtr)b;
	CVrmlListPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CVrmlListPtr)cMalloc(sizeof(CVrmlList),8577);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	if(s->vrmlNumbers && s->vrmlCount){
	   sdsNew->vrmlNumbers=(long *)cMalloc(sizeof(long)*s->vrmlCount,8578);
	   if(!sdsNew->vrmlNumbers){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->vrmlCount;++n){
	       sdsNew->vrmlNumbers[n]=s->vrmlNumbers[n];
	   }
	}else{
	    sdsNew->vrmlNumbers=NULL;
	}

	return (CObjectPtr)sdsNew;
}

static int ObjectListAddVrml(struct Scene *scene,long k)
{
	CVrmlPtr2 vrml;

	if(!scene)return 1;

	vrml=CVrmlCreate2(scene,k);
	if(!vrml)return 1;

	if(ListAddObjectList(scene->ObjectList,(CObjectPtr)vrml)){
	    cFree((char *)vrml);
	    return 1;
	}

	return 0;
}
static int CVrmlRangeObjects(struct vrmlObjects *o,struct tState *tS,struct ObjectMessage *Message)
{
	struct vrmlSeparator *s;
	struct tState tSsave;
	struct vrmlUse *u;
	struct P p;
	static double x[8]={-1.,1.,1.,-1., -1.,1.,1.,-1.};
	static double y[8]={-1.,-1.,1.,1., -1.,-1.,1.,1.};
	static double z[8]={-1.,-1.,-1.,-1.,1.,1.,1.,1.};
	double xm,ym,zm;
	long n;

	if(!o)return 1;
	if(0 <= o->type && o->type < sizeof(vrmlTypeNames)/sizeof(char *)){
	}else{
	    sprintf(WarningBuff,"CVrmlRangeObjects type %ld Out of Range\n",o->type);
		WarningBatch(WarningBuff);
	    return 1;
	}

	switch(o->type)
	{

	   case T_VRML_Transform:
		return drawTransFormTransform((struct vrmlTransForm *)o,tS);
	   case T_VRML_Rotation:
		return drawRotationTransform((struct vrmlRotation *)o,tS);
	   case T_VRML_Scale:
		return drawScaleTransform((struct vrmlScale *)o,tS);
	   case T_VRML_Translation:
		return drawTranslationTransform((struct vrmlTranslation *)o,tS);
	   case T_VRML_MatrixTransform:
		return drawMatrixTransform((struct vrmlMatrixTransform *)o,tS);
	   case T_VRML_Cone:
	   {
	        struct vrmlCone *c=(struct vrmlCone *)o;

			xm=zm=c->bottomRadius;
			ym=5.*c->height;
	   }
DoRange:
        for(n=0;n<8;++n){
            p.x=xm*x[n];
            p.y=ym*y[n];
            p.z=zm*z[n];
            Transform(&p,&tS->GroupToScreen);
            if(p.x > Message->xmax)Message->xmax=p.x;
            if(p.y > Message->ymax)Message->ymax=p.y;
            if(p.z > Message->zmax)Message->zmax=p.z;
            if(p.x < Message->xmin)Message->xmin=p.x;
            if(p.y < Message->ymin)Message->ymin=p.y;
            if(p.z < Message->zmin)Message->zmin=p.z;
        }

		return 0;
	   case T_VRML_Cube:
	   {
	        struct vrmlCube *c=(struct vrmlCube *)o;

			ym=0.5*c->height;
			zm=0.5*c->depth;
			xm=0.5*c->width;
			goto DoRange;
	   }
	   case T_VRML_Cylinder:
	   {
	        struct vrmlCylinder *c=(struct vrmlCylinder *)o;
			xm=c->radius;
			ym=0.5*c->height;
			zm=c->radius;
			goto DoRange;
	   }
	   case T_VRML_Sphere:
	   {
	        struct vrmlSphere *s=(struct vrmlSphere *)o;
			xm=s->radius;
			ym=s->radius;
			zm=s->radius;
			goto DoRange;
	   }
	   case T_VRML_IndexedFaceSet:
	   {
	        struct vrmlCoordinate3 *s=(struct vrmlCoordinate3 *)tS->p;

			if(!s->pointdata)return 0;

	        for(n=0;n<s->points;n += 3){
	            p.x=s->pointdata[n];
	            p.y=s->pointdata[n+1];
	            p.z=s->pointdata[n+2];
	            Transform(&p,&tS->GroupToScreen);
	            if(p.x > Message->xmax)Message->xmax=p.x;
	            if(p.y > Message->ymax)Message->ymax=p.y;
	            if(p.z > Message->zmax)Message->zmax=p.z;
	            if(p.x < Message->xmin)Message->xmin=p.x;
	            if(p.y < Message->ymin)Message->ymin=p.y;
	            if(p.z < Message->zmin)Message->zmin=p.z;
	        }
	   }
		return 0;
	   case T_VRML_Texture2:
		tS->t=(struct vrmlTexture2 *)o;
	   break;
	   case T_VRML_Texture2Transform:
		tS->t2t=(struct vrmlTexture2Transform *)o;
	   break;
	   case T_VRML_TextureCoordinate2:
		tS->t2=(struct vrmlTextureCoordinate2 *)o;
	   break;
	   case T_VRML_NormalBinding:
		tS->nb=(struct vrmlNormalBinding *)o;
	   break;
	   case T_VRML_Normal:
		tS->n=(struct vrmlNormal *)o;
	   break;
	   case T_VRML_Coordinate3:
		tS->p=(struct vrmlCoordinate3 *)o;
	   break;
	   case T_VRML_Material:
		tS->m=(struct vrmlMaterial *)o;
	   break;
	   case T_VRML_MaterialBinding:
		tS->mb=(struct vrmlMaterialBinding *)o;
	   break;
	   case T_VRML_Separator:
	   case T_VRML_TransformSeparator:
	   case T_VRML_Group:
	   case T_VRML_Switch:
	    s=(struct vrmlSeparator *)o;
	    if(o->type != T_VRML_Group)tSsave = *tS;
	    for(n=0;n<s->Count;++n){
	        if(s->o && s->o[n]){
	            if(CVrmlRangeObjects(s->o[n],tS,Message)){
					*tS = tSsave;
	                return 1;
	            }
	        }
	    }
	    if(o->type != T_VRML_Group)*tS = tSsave;
	    break;
	   case T_VRML_Use:
	    u=(struct vrmlUse *)o;
	    if(u->o){
	        return CVrmlRangeObjects(u->o,tS,Message);
	    }
	    break;
	   default:
		;
	}

	return 0;
}
static int CVrmlMessage(CObject *b,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct vrmlList *vrml;
	struct Scene *scene;
	struct tState tS;

	CVrmlPtr2 s=(CVrmlPtr2)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 

	    scene=s->scene;
	    if(!scene)return 0;

	    if(scene->SmoothNormals != 2 && scene->SmoothNormals != 3){
	       return 0;
	    }

 
	    vrml = &scene->vrml[s->VrmlNumber];

	    zerol((char *)&tS,sizeof(struct tState));

	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&tS.GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&tS.GroupToScreen,&s->Local);

 	    tS.scene=scene;
 
	    CVrmlRangeObjects(vrml->vrml,&tS,Message);
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
		}
	}
	return 0;
}
static CVrmlPtr2 CVrmlCreate2(struct Scene *scene,long vrmlNumber)
{
	CVrmlPtr2 s;

	s=(CVrmlPtr2)cMalloc(sizeof(CVrml2),8540);
	if(!s)return (CVrmlPtr2)NULL;
	zerol((char *)s,sizeof(CVrml2));

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_VRML2;
	s->duplicateObject=CVrmlDuplicate;
	s->killObject=CObjectkill;
	s->drawObject=CVrmlDraw;
	s->countObject=CVrmlCount;
	s->pGetMost=CVrmlMost;
	
	s->message=CVrmlMessage;

	s->VrmlNumber=vrmlNumber;

	s->Material=3;

	return s;
}
static int CVrmlDraw(CObject *oo,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CVrmlPtr2 s=(CVrmlPtr2)oo;
	struct Matrix GroupToWorld;
	struct vrmlList *vrml;
	struct vrmlObjects *o;
	struct Scene *scene;
	struct tState tS;

	if(!s || !WorldToScreen || !CellCount)return 1;

	scene=s->scene;
	if(!scene)return 0;

	if(flag == 1){
	    if(scene->material[s->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[s->Material].opacity == 1.0)return 0;
	}


	if(scene->SmoothNormals != 2 && scene->SmoothNormals != 3){
	   	sprintf(WarningBuff,"CVrmlDraw Skipping VRML Object - Turn On Phong Shading\n");
		WarningBatch(WarningBuff);
	  	return 0;
	}

	vrml = &scene->vrml[s->VrmlNumber];

	o=vrml->vrml;

	zerol((char *)&tS,sizeof(struct tState));

	tS.opacity=scene->material[s->Material].opacity;
	
	s->FirstCell = *CellCount;

	BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	MultMatrix(&tS.GroupToScreen,WorldToScreen,&GroupToWorld);
	ScaleMatrix2(&tS.GroupToScreen,&s->Local);

	tS.scene=scene;
	tS.CellCount=CellCount;
	tS.flag=flag;

	CVrmlDrawObjects(o,&tS);
	
	s->LastCell = *CellCount;

	return 0;
}
static int drawIndexedFaceSet(struct vrmlIndexedFaceSet *s,struct tState *tS)
{
	struct vrmlCoordinate3 *p;
	struct Scene *scene;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	float *rdata;
	double *xdatal,*ydatal,*zdatal;
	float *nvdata;
	long *ndata,nel;
	int j;
	long *CellCount;
	long np;
	int parts,ntype;
	long *matdata;
	float *normal;
	long normmax;
	long *normlist;
	long normlistmax;

	if(!s || !tS)return 1;
	matdata=NULL;
	normal=NULL;
	normmax=0;
	scene=tS->scene;
	CellCount=tS->CellCount;


	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;

	rdata=scene->rdata;
	nvdata=scene->nvdata;
	ndata=scene->cdata;
	s->FirstCell = *CellCount;

	p=tS->p;
	if(!p){
	   	sprintf(WarningBuff,"drawIndexedFaceSet NULL Node Pointer\n");
		WarningBatch(WarningBuff);
	   	return 1;
	}
    v1x=tS->GroupToScreen.x[0][0];
    v1y=tS->GroupToScreen.x[0][1];
    v1z=tS->GroupToScreen.x[0][2];
    v2x=tS->GroupToScreen.x[1][0];
    v2y=tS->GroupToScreen.x[1][1];
    v2z=tS->GroupToScreen.x[1][2];
    v3x=tS->GroupToScreen.x[2][0];
    v3y=tS->GroupToScreen.x[2][1];
    v3z=tS->GroupToScreen.x[2][2];
    xc=tS->GroupToScreen.x[0][3];
    yc=tS->GroupToScreen.x[1][3];
    zc=tS->GroupToScreen.x[2][3];

    np=0;
    for(j=0;j<p->points;j += 3){
           dx=p->pointdata[j];
           dy=p->pointdata[j+1];
           dz=p->pointdata[j+2];
           xdatal[np]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
           ydatal[np]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
           zdatal[np]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
           ++np;
    }

    nel=0;
    for(j=0;j<s->coord;j++){
        if(s->coorddata[j] == -1)++nel;
    }

    parts=0;
    if(tS->mb){
		if(tS->mb->value == PER_PART || tS->mb->value == PER_FACE){
		    parts=1;
		}else if(tS->mb->value == PER_FACE_INDEXED || tS->mb->value == PER_PART_INDEXED){
		    parts=2;
		}else if(tS->mb->value == PER_VERTEX_INDEXED){
		    parts=3;
		}
    }

    if(parts == 2){
        if(s->matdata){
           matdata=s->matdata;
        }else{
            if(tS->scene->debug > 1){
            	sprintf(WarningBuff,"drawIndexedFaceSet Material Index List Missing\n");
				WarningBatch(WarningBuff);
			}
            parts=0;
        }
    }else if(parts == 3){
        if(s->matdata){
           matdata=s->matdata;
        }else{
            if(tS->m && (s->coord <= tS->m->diffuse)){
           		matdata=s->coorddata;
            }else{
                if(tS->scene->debug > 1){
            		sprintf(WarningBuff,"drawIndexedFaceSet Material Index List Missing\n");
					WarningBatch(WarningBuff);
				}
            	parts=0;
            }
        }
    }

    if(parts != 2 && parts != 3){
       matdata=ndata;
        for(j=0;j<nel;j++){
            if(parts == 0){
                matdata[j]=0;
            }else{
                matdata[j]=j;
            }
        }
    }

    ntype=0;
    if(tS->nb){
		if(tS->nb->value == PER_PART || tS->nb->value == PER_FACE){
		    ntype=1;
		}else if(tS->nb->value == PER_FACE_INDEXED || tS->nb->value == PER_PART_INDEXED){
		    ntype=2;
		}else if(tS->nb->value == PER_VERTEX){
		    ntype=3;
		}else if(tS->nb->value == PER_VERTEX_INDEXED){
		    ntype=0;
		}
    }

	ndata=s->coorddata;

	nel = s->coord;

	vdata=zdatal;

    if(nvdata){
        for(j=0;j<np;++j){
            rdata[j]=0;
	    	nvdata[j*3]=0;
	    	nvdata[j*3+1]=0;
            nvdata[j*3+2]=0;
        }

        for(j=0;j<nel;++j){
	    	struct P v1,v2,norm;
            long n1,n2,n3,n4;
	    	long ns,k;
	    
    	    n1=ndata[j];
    	    if(n1 < 0)continue;
            n2=ndata[++j];
    	    if(n2 < 0)continue;
            n3=ndata[++j];
    	    if(n3 < 0)continue;
	    	n4=ndata[++j];
	    	if(n4 != -1){
		        ns=j;
		        while(++j < nel && ndata[j] != -1){
		            ;
		        }
	    	}else{
	        	ns = -1;
	    	}
	    	
	    	if(n1 >= np || n2 >= np || n3 >= np){
	    	    continue;
	    	}

            v1.x=xdatal[n2]-xdatal[n1];
            v1.y=ydatal[n2]-ydatal[n1];
            v1.z=zdatal[n2]-zdatal[n1];
            v2.x=xdatal[n3]-xdatal[n1];
            v2.y=ydatal[n3]-ydatal[n1];
            v2.z=zdatal[n3]-zdatal[n1];
            norm=CrossN(&v1,&v2);
			
            nvdata[n1*3] += (float)norm.x;
            nvdata[n1*3+1] += (float)norm.y;
            nvdata[n1*3+2] += (float)norm.z;
            ++rdata[n1];
            nvdata[n2*3] += (float)norm.x;
            nvdata[n2*3+1] += (float)norm.y;
            nvdata[n2*3+2] += (float)norm.z;
            ++rdata[n2];
            nvdata[n3*3] += (float)norm.x;
            nvdata[n3*3+1] += (float)norm.y;
            nvdata[n3*3+2] += (float)norm.z;
            ++rdata[n3];
            if(ns >= 0){
                for(k=ns;k<j;++k){
                    n4=ndata[k];
		            nvdata[n4*3]   += (float)norm.x;
		            nvdata[n4*3+1] += (float)norm.y;
		            nvdata[n4*3+2] += (float)norm.z;
		            ++rdata[n4];
	        	}
    		}
    	}

        for(j=0;j<np;++j){
             if(rdata[j] > 0){
                 nvdata[j*3] /= (float)rdata[j];
                 nvdata[j*3+1] /= (float)rdata[j];
                 nvdata[j*3+2] /= (float)rdata[j];
             }
        }

		normlist=NULL;
		normlistmax=0;
		if(ntype == 0){
		    if(tS->n){
				normal=nvdata+3*np;
				normmax=(tS->n->normal)/3;
				for(j=0;j<normmax;++j){
				   dx=tS->n->normaldata[j*3];
		           dy=tS->n->normaldata[j*3+1];
		           dz=tS->n->normaldata[j*3+2];
		           normal[j*3]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
		           normal[j*3+1]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
		           normal[j*3+2]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
				}
		    }
		
		    if(s->normdata){
		        normlist=s->normdata;
		        normlistmax=s->norm;
		    }
		}
		{
		    int flagTexture;

			flagTexture=0;
			if(tS->t){
			    if(tS->t->image){
			        if(tS->t->xsize > 0 && tS->t->ysize > 0 && tS->t->componets == 3){
			            ++flagTexture;
			        }else{
			   			if(scene->debug){
				            sprintf(WarningBuff,"Bad Texture xsize %ld ysize %ld componets %ld\n",tS->t->xsize,
				              tS->t->ysize,tS->t->componets);
							WarningBatch(WarningBuff);
						}
			        }
			    }
			}
			if(tS->t2){
			   ++flagTexture;
			}

			if(s->textdata){
			   if(s->text >= s->coord){
			       ++flagTexture;
			   }else{
			   		if(scene->debug){
			       		sprintf(WarningBuff,"Error textureCoordIndex count %ld cell count %ld\n",s->text,s->coord);
						WarningBatch(WarningBuff);
					}
			   }
			}
			if(flagTexture == 3){
			    tS->flagTexture=TRUE;
			}else{
			    tS->flagTexture=FALSE;
			}
		}
		if(parts == 3){
		    tS->MaterialType=VRML_BY_VERTEX;
		}else{
		    tS->MaterialType=VRML_BY_FACE;
		}
		tS->NormalType=VRML_BY_VERTEX;
		tS->s=s;
		(*scene->doNoShadowVRML)(ndata,matdata,normal,normmax,normlist,normlistmax,
		                         xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene,tS);
		tS->flagTexture=FALSE;
		tS->s=NULL;
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}


	*CellCount += nel;


	return 0;
}
static int drawSphere(struct vrmlSphere *s,struct tState *tS)
{
	struct Scene *scene;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	float *nvdata;
	float *rdata,*sdata;
	double r,pi,dumx,dumy;
	long *ndata,nel;
	long k;
	int i,j;
	long *CellCount;
	long *matdata;

	if(!s || !tS)return 1;

	scene=tS->scene;
	CellCount=tS->CellCount;
	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	nvdata=scene->nvdata;
	ndata=scene->cdata;
	rdata=scene->rdata;
	sdata=scene->sdata;
	s->FirstCell = *CellCount;
	s->nx=20;
	s->ny=20;
    v1x=tS->GroupToScreen.x[0][0];
    v1y=tS->GroupToScreen.x[0][1];
    v1z=tS->GroupToScreen.x[0][2];
    v2x=tS->GroupToScreen.x[1][0];
    v2y=tS->GroupToScreen.x[1][1];
    v2z=tS->GroupToScreen.x[1][2];
    v3x=tS->GroupToScreen.x[2][0];
    v3y=tS->GroupToScreen.x[2][1];
    v3z=tS->GroupToScreen.x[2][2];
    xc=tS->GroupToScreen.x[0][3];
    yc=tS->GroupToScreen.x[1][3];
    zc=tS->GroupToScreen.x[2][3];
    k=0;
    r=s->radius;
    pi=4*atan(1.0);
    dumy=1.0/((double)s->ny-1);
    dumx=1.0/((double)s->nx-1);

    for(j=0;j<s->ny;++j){
        double sinf,cosf;
        sinf=sin(pi*(1.-j*dumy));
        cosf=cos(pi*(1.-j*dumy));
        for(i=0;i<s->nx;++i){
           dx=r*cos(2*pi*(1.-i*dumx))*sinf;
           dy=r*sin(2*pi*(1.-i*dumx))*sinf;
           dz=r*cosf;
           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
           ++k;
        }
    }
    nel=0;
    for(j=0;j<s->ny-1;++j){
        for(i=0;i<s->nx-1;++i){
		    ndata[nel++]=i+j*s->nx;
		    ndata[nel++]=i+1+j*s->nx;
		    ndata[nel++]=i+1+(j+1)*s->nx;
		    ndata[nel++]=i+(j+1)*s->nx;
		    ndata[nel++] = -1;
        }
    }
    matdata = ndata+nel;

    for(i=0;i<nel/5;++i)matdata[i]=0;

    vdata=zdatal;
    if(nvdata){
    	k=0;
        for(j=0;j<s->ny;++j){
            double sinf,cosf;
            sinf=sin(pi*(1.-j*dumy));
            cosf=cos(pi*(1.-j*dumy));
		    for(i=0;i<s->nx;++i){		        
		        dx=r*cos(2*pi*(1.-i*dumx))*sinf;
		        dy=r*sin(2*pi*(1.-i*dumx))*sinf;
		        dz=r*cosf;
				 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
				 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
				 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
		    }
    	}
    	if(rdata && sdata){
		    dx=0;
		    if(s->nx > 1)dx=1.0/((double)(s->nx-1));

		    dy=0;
		    if(s->ny > 1)dy=1.0/((double)(s->ny-1));
    	    k=0;
            for(j=0;j<s->ny;++j){
		        for(i=0;i<s->nx;++i){
				    rdata[k]=(float)((i)*dx);
				    sdata[k++]=(float)((j)*dy);
		        }
    	    }
    	}
		tS->MaterialType=VRML_BY_FACE;
		(*scene->doNoShadowVRML)(ndata,matdata,NULL,0L,NULL,0L,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene,tS);
    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
    }


	*CellCount += nel;
	

	return 0;
}
static int drawDisk(struct vrmlDisk *d,struct tState *tS)
{
	struct Scene *scene;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k;
    double rout;

	int i,j;
	long *CellCount;
	long *matdata;

	if(!d || !tS)return 1;
	scene=tS->scene;
	CellCount=tS->CellCount;
	d->FirstCell = *CellCount;


	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    v1x=tS->GroupToScreen.x[0][0];
    v1y=tS->GroupToScreen.x[0][1];
    v1z=tS->GroupToScreen.x[0][2];
    v2x=tS->GroupToScreen.x[1][0];
    v2y=tS->GroupToScreen.x[1][1];
    v2z=tS->GroupToScreen.x[1][2];
    v3x=tS->GroupToScreen.x[2][0];
    v3y=tS->GroupToScreen.x[2][1];
    v3z=tS->GroupToScreen.x[2][2];
    xc=tS->GroupToScreen.x[0][3];
    yc=tS->GroupToScreen.x[1][3];
    zc=tS->GroupToScreen.x[2][3];

    k=0;
    rout=d->radius;
    pi=4*atan(1.0);
    dumy=1.0/((double)d->ny-1);
    dumx=1.0/((double)d->nx-1);

    if(d->type == T_VRML_TOP){
        dy = 0.5*d->height;
    }else{
        dy = -0.5*d->height;
    }

    for(j=0;j<d->ny;++j){
        for(i=0;i<d->nx;++i){
           dx=(j*dumy*rout)*cos(2*pi*(0.5+i*dumx));
           dz=(j*dumy*rout)*sin(2*pi*(0.5+i*dumx));
           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
           ++k;
        }
    }

    nel=0;
    for(j=0;j<d->ny-1;++j){
        for(i=0;i<d->nx-1;++i){
		    ndata[nel++]=i+j*d->nx;
		    ndata[nel++]=i+1+j*d->nx;
		    ndata[nel++]=i+1+(j+1)*d->nx;
		    ndata[nel++]=i+(j+1)*d->nx;
		    ndata[nel++] = -1;
        }
    }

    matdata=ndata+nel;

    vdata=zdatal;
    if(nvdata){
    	dz=0.0;
        dx=0.0;
        dy=1.0;
    	k=0;
        for(j=0;j<d->ny;++j){
		    for(i=0;i<d->nx;++i){		        	        
				nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
				nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
				nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
		    }
    	}
    	if(rdata && sdata){
	    dx=0;
	    if(d->nx > 1)dx=1.0/((double)(d->nx-1));

	    dy=0;
	    if(d->ny > 1)dy=1.0/((double)(d->ny-1));

    	    k=0;
            for(j=0;j<d->ny;++j){
		        for(i=0;i<d->nx;++i){
				    rdata[k]=(float)(i*dx);
				    sdata[k++]=(float)(j*dy);
		        }
    	    }
    	}
        for(j=0;j<nel/5;++j){
	    	matdata[j]=tS->Material;
    	}
		tS->MaterialType=VRML_BY_FACE;
		(*scene->doNoShadowVRML)(ndata,matdata,NULL,0L,NULL,0L,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene,tS);
    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
    }

	*CellCount += nel;

	return 0;

}
static int drawCylinder(struct vrmlCylinder *c,struct tState *tS)
{
	struct Scene *scene;
	struct vrmlDisk d;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k;
    double rtop;
    double length,l2;
	int i,j;
	long *CellCount;
	long *matdata;

	if(!c || !tS)return 1;
	scene=tS->scene;
	CellCount=tS->CellCount;
	c->FirstCell = *CellCount;
	c->nx=40;
	c->ny=10;

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;
    v1x=tS->GroupToScreen.x[0][0];
    v1y=tS->GroupToScreen.x[0][1];
    v1z=tS->GroupToScreen.x[0][2];
    v2x=tS->GroupToScreen.x[1][0];
    v2y=tS->GroupToScreen.x[1][1];
    v2z=tS->GroupToScreen.x[1][2];
    v3x=tS->GroupToScreen.x[2][0];
    v3y=tS->GroupToScreen.x[2][1];
    v3z=tS->GroupToScreen.x[2][2];
    xc=tS->GroupToScreen.x[0][3];
    yc=tS->GroupToScreen.x[1][3];
    zc=tS->GroupToScreen.x[2][3];

    rtop=c->radius;
    length=c->height;
    l2=.5*length;
    zerol((char *)&d,sizeof(struct vrmlDisk));
    d.radius=rtop;
    d.height=c->height;
    d.Material=c->Material;
    d.nx=c->nx;
    d.ny=c->ny;

    if(c->parts == T_VRML_ALL || c->parts == T_VRML_TOP ){
		d.type=T_VRML_TOP;
		if(tS->mb && (tS->mb->value == PER_PART || tS->mb->value == PER_PART_INDEXED)){
		    tS->Material=1;
		}else{
		    tS->Material=0;
		}
		drawDisk(&d,tS);
    }
    if(c->parts == T_VRML_ALL || c->parts == T_VRML_BOTTOM ){
		d.type=T_VRML_BOTTOM;
		if(tS->mb && (tS->mb->value == PER_PART || tS->mb->value == PER_PART_INDEXED)){
		    tS->Material=2;
		}else{
		    tS->Material=0;
		}
		drawDisk(&d,tS);
    }

    if(c->parts != T_VRML_ALL && c->parts != T_VRML_SIDES)return 0;

    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);
    k=0;
    for(j=0;j<c->ny;++j){
        for(i=0;i<c->nx;++i){
           dx=rtop*cos(2*pi*(0.5+i*dumx));
           dz=rtop*sin(2*pi*(0.5+i*dumx));
           dy = -l2+j*dumy*(length);
           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
           ++k;
        }
    }
    nel=0;
    for(j=0;j<c->ny-1;++j){
        for(i=0;i<c->nx-1;++i){
		    ndata[nel++]=i+j*c->nx;
		    ndata[nel++]=i+1+j*c->nx;
		    ndata[nel++]=i+1+(j+1)*c->nx;
		    ndata[nel++]=i+(j+1)*c->nx;
		    ndata[nel++] = -1;
        }
    }

    matdata=ndata+nel;

    vdata=zdatal;

   if(nvdata){

    	dy=0.0;
    	    
    	k=0;
        for(j=0;j<c->ny;++j){
		    for(i=0;i<c->nx;++i){
                dx=rtop*cos(2*pi*(0.5+i*dumx));
                dz=rtop*sin(2*pi*(0.5+i*dumx));
			        
				nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
				nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
				nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
		    }
    	}
    	if(rdata && sdata){
		    dx=0;
		    if(c->nx > 1)dx=1.0/((double)(c->nx-1));

		    dy=0;
		    if(c->ny > 1)dy=1.0/((double)(c->ny-1));

    	    k=0;
            for(j=0;j<c->ny;++j){
		        for(i=0;i<c->nx;++i){
				    rdata[k]=(float)(i*dx);
				    sdata[k++]=(float)(j*dy);
		        }
    	    }
    	}
        for(j=0;j<nel/5;++j){
	    	matdata[j]=0;
    	}
		tS->MaterialType=VRML_BY_FACE;
		(*scene->doNoShadowVRML)(ndata,matdata,NULL,0L,NULL,0L,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene,tS);
    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
    }

	*CellCount += nel;

	return 0;
}
static int drawCube(struct vrmlCube *d,struct tState *tS)
{
	struct Scene *scene;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	float *nvdata;
	float *rdata,*sdata;
	long *ndata,nel;
	long k;
    double x[24],y[24],z[24];
    double h2,d2,w2;

	int i,j;
	long *CellCount;
	int parts;
	long *matdata;

	if(!d || !tS)return 1;
	scene=tS->scene;
	CellCount=tS->CellCount;
	d->FirstCell = *CellCount;


	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

    v1x=tS->GroupToScreen.x[0][0];
    v1y=tS->GroupToScreen.x[0][1];
    v1z=tS->GroupToScreen.x[0][2];
    v2x=tS->GroupToScreen.x[1][0];
    v2y=tS->GroupToScreen.x[1][1];
    v2z=tS->GroupToScreen.x[1][2];
    v3x=tS->GroupToScreen.x[2][0];
    v3y=tS->GroupToScreen.x[2][1];
    v3z=tS->GroupToScreen.x[2][2];
    xc=tS->GroupToScreen.x[0][3];
    yc=tS->GroupToScreen.x[1][3];
    zc=tS->GroupToScreen.x[2][3];

	h2=0.5*d->height;
	d2=0.5*d->depth;
	w2=0.5*d->width;
	/* front */
	x[0]= -w2;
	y[0]= -h2;
	z[0]= d2;

	x[1]= w2;
	y[1]= -h2;
	z[1]= d2;

	x[2]= w2;
	y[2]= h2;
	z[2]= d2;


	x[3]= -w2;
	y[3]= h2;
	z[3]= d2;

	/* back */

	x[4]= w2;
	y[4]= -h2;
	z[4]= -d2;

	x[5]= -w2;
	y[5]= -h2;
	z[5]= -d2;

	x[6]= -w2;
	y[6]= h2;
	z[6]= -d2;


	x[7]= w2;
	y[7]= h2;
	z[7]= -d2;

	/* left */

	x[8]= -w2;
	y[8]= -h2;
	z[8]= d2;

	x[9]= -w2;
	y[9]=  h2;
	z[9]=  d2;

	x[10]= -w2;
	y[10]= h2;
	z[10]= -d2;


	x[11]= -w2;
	y[11]= -h2;
	z[11]= -d2;


	/* right */

	x[12]= w2;
	y[12]= -h2;
	z[12]= d2;

	x[13]= w2;
	y[13]= -h2;
	z[13]= -d2;

	x[14]= w2;
	y[14]= h2;
	z[14]= -d2;


	x[15]= w2;
	y[15]= h2;
	z[15]= d2;

	/* top */

	x[16]= -w2;
	y[16]= h2;
	z[16]= d2;

	x[17]= w2;
	y[17]= h2;
	z[17]= d2;

	x[18]= w2;
	y[18]= h2;
	z[18]= -d2;


	x[19]= -w2;
	y[19]= h2;
	z[19]= -d2;


	/* bottom */

	x[20]= w2;
	y[20]= -h2;
	z[20]= d2;

	x[21]= -w2;
	y[21]= -h2;
	z[21]= d2;

	x[22]= -w2;
	y[22]= -h2;
	z[22]= -d2;


	x[23]= w2;
	y[23]= -h2;
	z[23]= -d2;

	for(j=0;j<24;++j){
	    dx=x[j];
	    dy=y[j];
	    dz=z[j];
	    xdatal[j]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	    ydatal[j]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	    zdatal[j]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
	}
	nel=0;
	for(j=0;j<6;++j){
	    ndata[nel++]=j*4;
	    ndata[nel++]=1+j*4;
	    ndata[nel++]=2+j*4;
	    ndata[nel++]=3+j*4;
	    ndata[nel++] = -1;
	}

	matdata=ndata+nel;

	if(tS->mb && (tS->mb->value == PER_PART || tS->mb->value == PER_PART_INDEXED ||
		 tS->mb->value == PER_FACE || tS->mb->value == PER_FACE_INDEXED)){
		parts=TRUE;
	}else{
		parts=FALSE;
	}
	vdata=zdatal;
	if(nvdata){
		k=0;
	    for(j=0;j<6;++j){
	        if(j == 0){
				dz=1.0;
				dx=0.0;
				dy=0.0;
	        }else if(j == 1){
				dz= -1.0;
				dx=0.0;
				dy=0.0;
	        }else if(j == 2){
				dz= 0.0;
				dx= -1.0;
				dy=0.0;
	        }else if(j == 3){
				dz= 0.0;
				dx= 1.0;
				dy=0.0;
	        }else if(j == 4){
				dz= 0.0;
				dx= 0.0;
				dy=1.0;
	        }else{
				dz= 0.0;
				dx= 0.0;
				dy= -1.0;
	        }
		    for(i=0;i<4;++i){	
		        double xx,yy,zz,r;
				xx=(dx)*v1x+(dy)*v1y+(dz)*v1z;
				yy=(dx)*v2x+(dy)*v2y+(dz)*v2z;
				zz=(dx)*v3x+(dy)*v3y+(dz)*v3z;
				r=sqrt(xx*xx+yy*yy+zz*zz);
				nvdata[k++]=(float)(xx/r);
				nvdata[k++]=(float)(yy/r);
				nvdata[k++]=(float)(zz/r);
		    }
		}
		if(rdata && sdata){
		    k=0;
	        for(j=0;j<6;++j){
		        for(i=0;i<4;++i){
				    rdata[k]=(float)(i*dx);
				    sdata[k++]=(float)(j*dy);
		        }
		    }
		}
	    for(j=0;j<6;++j){
	        if(parts){
	            matdata[j]=j;
	        }else{
	            matdata[j]=0;
	        }
		}
		tS->MaterialType=VRML_BY_FACE;
		(*scene->doNoShadowVRML)(ndata,matdata,NULL,0L,NULL,0L,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene,tS);
		*CellCount += nel;
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
		*CellCount += nel;
	}
	return 0;
}
static int drawCone(struct vrmlCone *c,struct tState *tS)
{
	struct Scene *scene;
	struct vrmlDisk d;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	float *nvdata;
	float *rdata,*sdata;
	double pi,dumx,dumy;
	long *ndata,nel;
	long k;
    double rtop;
    double rbottom;
    double length,l2;
	int i,j;
	long *CellCount;
	long *matdata;

	if(!c || !tS)return 1;
	scene=tS->scene;
	CellCount=tS->CellCount;
	c->FirstCell = *CellCount;
	c->nx=20;
	c->ny=20;

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	ndata=scene->cdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;


    v1x=tS->GroupToScreen.x[0][0];
    v1y=tS->GroupToScreen.x[0][1];
    v1z=tS->GroupToScreen.x[0][2];
    v2x=tS->GroupToScreen.x[1][0];
    v2y=tS->GroupToScreen.x[1][1];
    v2z=tS->GroupToScreen.x[1][2];
    v3x=tS->GroupToScreen.x[2][0];
    v3y=tS->GroupToScreen.x[2][1];
    v3z=tS->GroupToScreen.x[2][2];
    xc=tS->GroupToScreen.x[0][3];
    yc=tS->GroupToScreen.x[1][3];
    zc=tS->GroupToScreen.x[2][3];

    rtop=0.0;
    rbottom=c->bottomRadius;
    length=c->height;
    l2=.5*length;
    zerol((char *)&d,sizeof(struct vrmlDisk));
    d.radius=rbottom;
    d.height=c->height;
    d.Material=c->Material;
    d.nx=c->nx;
    d.ny=c->ny;

    if(c->parts == T_VRML_ALL || c->parts == T_VRML_BOTTOM ){
		d.type=T_VRML_BOTTOM;
		if(tS->mb && (tS->mb->value == PER_PART || tS->mb->value == PER_PART_INDEXED)){
		    tS->Material=1;
		}else{
		    tS->Material=0;
		}
		drawDisk(&d,tS);
    }

    if(c->parts != T_VRML_ALL && c->parts != T_VRML_SIDES)return 0;

    pi=4*atan(1.0);
    dumy=1.0/((double)c->ny-1);
    dumx=1.0/((double)c->nx-1);
    k=0;
    for(j=0;j<c->ny;++j){
        for(i=0;i<c->nx;++i){
           dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(0.25+i*dumx));
           dz=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(0.25+i*dumx));
           dy = -l2+j*dumy*(length);
           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
           ++k;
        }
    }
    nel=0;
    for(j=0;j<c->ny-1;++j){
        for(i=0;i<c->nx-1;++i){
		    ndata[nel++]=i+j*c->nx;
		    ndata[nel++]=i+1+j*c->nx;
		    ndata[nel++]=i+1+(j+1)*c->nx;
		    ndata[nel++]=i+(j+1)*c->nx;
		    ndata[nel++] = -1;
        }
    }

	matdata=ndata+nel;

	vdata=zdatal;

	if(nvdata){
	    double den,th;

		den=rtop-rbottom;
		if(den == 0.0){
		    dy=0.0;
		}else{
		    th=atan2(1.0,length/den);
		    dy=sin(th);
		}	               
		k=0;
	    for(j=0;j<c->ny;++j){
			for(i=0;i<c->nx;++i){
				dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(0.25+i*dumx));
				dz=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(0.25+i*dumx));

				nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
				nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
				nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
			}
		}
		if(rdata && sdata){
			dx=0;
			if(c->nx > 1)dx=1.0/((double)(c->nx-1));

			dy=0;
			if(c->ny > 1)dy=1.0/((double)(c->ny-1));

			k=0;
			for(j=0;j<c->ny;++j){
				for(i=0;i<c->nx;++i){
					rdata[k]=(float)(i*dx);
					sdata[k++]=(float)(j*dy);
				}
			}
		}
	    for(j=0;j<nel/5;++j){
	        matdata[j]=0;
		}

		tS->MaterialType=VRML_BY_FACE;
		(*scene->doNoShadowVRML)(ndata,matdata,NULL,0L,NULL,0L,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene,tS);
		*CellCount += nel;
	}else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	}

	*CellCount += nel;

	return 0;
}
static int drawMatrixTransform(struct vrmlMatrixTransform *mt,struct tState *tS)
{
	struct Matrix m;
	struct Matrix r;

	if(!mt || !tS)return 1;


	m.x[0][0]=mt->matrix[0];
	m.x[1][0]=mt->matrix[1];
	m.x[2][0]=mt->matrix[2];
	m.x[3][0]=mt->matrix[3];
	m.x[0][1]=mt->matrix[4];
	m.x[1][1]=mt->matrix[5];
	m.x[2][1]=mt->matrix[6];
	m.x[3][1]=mt->matrix[7];
	m.x[0][2]=mt->matrix[8];
	m.x[1][2]=mt->matrix[9];
	m.x[2][2]=mt->matrix[10];
	m.x[3][2]=mt->matrix[11];
	m.x[0][3]=mt->matrix[12];
	m.x[1][3]=mt->matrix[13];
	m.x[2][3]=mt->matrix[14];
	m.x[3][3]=mt->matrix[15];

	MultMatrix(&r,&tS->GroupToScreen,&m);

	tS->GroupToScreen=r;
	return 0;
}
static int drawTranslationTransform(struct vrmlTranslation *mt,struct tState *tS)
{
	struct Matrix m;
	struct Matrix r;

	if(!mt || !tS)return 1;

	m.x[0][0]=1.0;
	m.x[1][0]=0.0;
	m.x[2][0]=0.0;
	m.x[3][0]=0.0;
	m.x[0][1]=0.0;
	m.x[1][1]=1.0;
	m.x[2][1]=0.0;
	m.x[3][1]=0.0;
	m.x[0][2]=0.0;
	m.x[1][2]=0.0;
	m.x[2][2]=1.0;
	m.x[3][2]=0.0;
	m.x[0][3]=mt->offset[0];
	m.x[1][3]=mt->offset[1];
	m.x[2][3]=mt->offset[2];
	m.x[3][3]=1.0;

	MultMatrix(&r,&tS->GroupToScreen,&m);

	tS->GroupToScreen=r;
	return 0;
}
int drawScaleTransform(struct vrmlScale *mt,struct tState *tS)
{
/*
	struct Matrix m;
	struct Matrix r;
*/
	struct System p;

	if(!mt || !tS)return 1;


	p.Scale_x=mt->scale[0];
	p.Scale_y=mt->scale[1];
	p.Scale_z=mt->scale[2];

	ScaleMatrix2(&(tS->GroupToScreen),&p);
	/*
	m.x[0][0]=mt->scale[0];
	m.x[1][0]=0.0;
	m.x[2][0]=0.0;
	m.x[3][0]=0.0;
	m.x[0][1]=0.0;
	m.x[1][1]=mt->scale[1];
	m.x[2][1]=0.0;
	m.x[3][1]=0.0;
	m.x[0][2]=0.0;
	m.x[1][2]=0.0;
	m.x[2][2]=mt->scale[2];
	m.x[3][2]=0.0;
	m.x[0][3]=0.0;
	m.x[1][3]=0.0;
	m.x[2][3]=0.0;
	m.x[3][3]=1.0;

	MultMatrix(&r,&tS->GroupToScreen,&m);

	tS->GroupToScreen=r;
	*/
	return 0;
}
static int drawRotationTransform(struct vrmlRotation *mt,struct tState *tS)
{
	struct System p1;
	struct System p2;
	struct Matrix BlockToWorld,r,LocalToGlobal,GlobalToLocal;
	struct P zl,xl,yl,r1,r2,r3,t;
	static struct P x={1.0,0.0,0.0};
	static struct P y={0.0,1.0,0.0};
	static struct P z={0.0,0.0,1.0};
	static struct P p={0.0,0.0,0.0};
	double len1,len2,len3,angle,rl,theta;

	if(!mt || !tS)return 1;
	zl.x=mt->axis[0];
	zl.y=mt->axis[1];
	zl.z=mt->axis[2];
	if((zl.x*zl.x +zl.y*zl.y+zl.z*zl.z) <= 0.0){
	    zl.z=1.0;
	}
	zl=Norm(&zl);
	r1=CrossN(&x,&zl);
	r2=CrossN(&y,&zl);
	r3=CrossN(&z,&zl);
	len1=r1.x*r1.x+r1.y*r1.y+r1.z*r1.z;
	len2=r2.x*r2.x+r2.y*r2.y+r2.z*r2.z;
	len3=r3.x*r3.x+r3.y*r3.y+r3.z*r2.z;
	if(len1 > len2){
	    if(len1 > len3){
	       xl=r1;
	    }else{
	       xl=r3;
	    }
	}else if(len2 > len3){
	       xl=r2;
	}else{
	       xl=r3;
	}

	yl=CrossN(&zl,&xl);

	p1.x=xl;
	p1.y=yl;
	p1.z=zl;
	p1.p=p;

	angle=mt->angle;

	BuildTransformMatrix(&LocalToGlobal,&p1,&tS->scene->Global); 
	BuildTransformMatrix(&GlobalToLocal,&tS->scene->Global,&p1); 

	t=x;
	Transform(&t,&GlobalToLocal);
	rl=sqrt(t.x*t.x+t.y*t.y);
	if(rl > 0){
	    theta=atan2(t.y,t.x)+angle;
	    t.x=rl*cos(theta);
	    t.y=rl*sin(theta);
	}
	Transform(&t,&LocalToGlobal);
	p2.x=t;

	t=y;
	Transform(&t,&GlobalToLocal);
	rl=sqrt(t.x*t.x+t.y*t.y);
	if(rl > 0){
	    theta=atan2(t.y,t.x)+angle;
	    t.x=rl*cos(theta);
	    t.y=rl*sin(theta);
	}
	Transform(&t,&LocalToGlobal);
	p2.y=t;

	t=z;
	Transform(&t,&GlobalToLocal);
	rl=sqrt(t.x*t.x+t.y*t.y);
	if(rl > 0){
	    theta=atan2(t.y,t.x)+angle;
	    t.x=rl*cos(theta);
	    t.y=rl*sin(theta);
	}
	Transform(&t,&LocalToGlobal);
	p2.z=t;

	p2.p=p;


	BuildTransformMatrix(&BlockToWorld,&p2,&tS->scene->Global); 
	MultMatrix(&r,&tS->GroupToScreen,&BlockToWorld);
	tS->GroupToScreen=r;

	return 0;
}
static int drawTransFormTransform(struct vrmlTransForm *mt,struct tState *tS)
{
	struct vrmlTranslation t;
	struct vrmlRotation r;
	struct vrmlScale s;

	if(!mt || !tS)return 1;
	if(mt->translation[0] != 0 || mt->translation[1] != 0 || mt->translation[2] != 0){
	    zerol((char *)&t,sizeof(struct vrmlTranslation));
	    t.offset[0]=mt->translation[0];
	    t.offset[1]=mt->translation[1];
	    t.offset[2]=mt->translation[2];
	    drawTranslationTransform(&t,tS);
	}
	if(mt->center[0] != 0 || mt->center[1] != 0 || mt->center[2] != 0){
	    zerol((char *)&t,sizeof(struct vrmlTranslation));
	    t.offset[0]=mt->center[0];
	    t.offset[1]=mt->center[1];
	    t.offset[2]=mt->center[2];
	    drawTranslationTransform(&t,tS);
	}

	if(mt->rotation[3]){
	    zerol((char *)&r,sizeof(struct vrmlRotation));
	    r.axis[0]=mt->rotation[0];
	    r.axis[1]=mt->rotation[1];
	    r.axis[2]=mt->rotation[2];
	    r.angle=mt->rotation[3];
	    drawRotationTransform(&r,tS);
	}

	if(mt->scaleOrientation[3]){
	    zerol((char *)&r,sizeof(struct vrmlRotation));
	    r.axis[0]=mt->scaleOrientation[0];
	    r.axis[1]=mt->scaleOrientation[1];
	    r.axis[2]=mt->scaleOrientation[2];
	    r.angle=mt->scaleOrientation[3];
	    drawRotationTransform(&r,tS);
	}

	if(mt->scaleFactor[0] != 1 || mt->scaleFactor[1] != 1 || mt->scaleFactor[2] != 1){
	    zerol((char *)&s,sizeof(struct vrmlScale));
	    s.scale[0]= (double)mt->scaleFactor[0];
	    s.scale[1]= (double)mt->scaleFactor[1];
	    s.scale[2]= (double)mt->scaleFactor[2];
	    drawScaleTransform(&s,tS);
	}

	if(mt->scaleOrientation[3]){
	    zerol((char *)&r,sizeof(struct vrmlRotation));
	    r.axis[0]=   (double)mt->scaleOrientation[0];
	    r.axis[1]=   (double)mt->scaleOrientation[1];
	    r.axis[2]=   (double)mt->scaleOrientation[2];
	    r.angle= (double)-mt->scaleOrientation[3];
	    drawRotationTransform(&r,tS);
	}

	if(mt->center[0] != 0 || mt->center[1] != 0 || mt->center[2] != 0){
	    zerol((char *)&t,sizeof(struct vrmlTranslation));
	    t.offset[0]= (double)-mt->center[0];
	    t.offset[1]= (double)-mt->center[1];
	    t.offset[2]= (double)-mt->center[2];
	    drawTranslationTransform(&t,tS);
	}

	return 0;
}
static int CVrmlDrawObjects(struct vrmlObjects *o,struct tState *tS)
{
	struct vrmlSeparator *s;
	struct tState tSsave;
	struct Matrix GroupToScreen;
	
	static struct Matrix zero={
								{
								{0.,0.,0.,0.},
								{0.,0.,0.,0.},
								{0.,0.,0.,0.},
								{0.,0.,0.,0.},
								}
							   };
	
	struct vrmlUse *u;
	long n;

	if(!o)return 1;
	
	GroupToScreen=zero;
	
	if(0 <= o->type && o->type < sizeof(vrmlTypeNames)/sizeof(char *)){
	    ;
	}else{
	    sprintf(WarningBuff,"CVrmlDrawObjects type %ld Out of Range\n",o->type);
		WarningBatch(WarningBuff);
	    return 1;
	}

	switch(o->type)
	{

	   case T_VRML_Transform:
			return drawTransFormTransform((struct vrmlTransForm *)o,tS);
	   case T_VRML_Rotation:
			return drawRotationTransform((struct vrmlRotation *)o,tS);
	   case T_VRML_Scale:
			return drawScaleTransform((struct vrmlScale *)o,tS);
	   case T_VRML_Translation:
			return drawTranslationTransform((struct vrmlTranslation *)o,tS);
	   case T_VRML_MatrixTransform:
			return drawMatrixTransform((struct vrmlMatrixTransform *)o,tS);
	   case T_VRML_Cone:
			return drawCone((struct vrmlCone *)o,tS);
	   case T_VRML_Cube:
			return drawCube((struct vrmlCube *)o,tS);
	   case T_VRML_Cylinder:
			return drawCylinder((struct vrmlCylinder *)o,tS);
	   case T_VRML_Sphere:
			return drawSphere((struct vrmlSphere *)o,tS);
	   case T_VRML_IndexedFaceSet:
			return drawIndexedFaceSet((struct vrmlIndexedFaceSet *)o,tS);
	   case T_VRML_Texture2:
			tS->t=(struct vrmlTexture2 *)o;
	   break;
	   case T_VRML_Texture2Transform:
			tS->t2t=(struct vrmlTexture2Transform *)o;
	   break;
	   case T_VRML_TextureCoordinate2:
			tS->t2=(struct vrmlTextureCoordinate2 *)o;
	   break;
	   case T_VRML_NormalBinding:
			tS->nb=(struct vrmlNormalBinding *)o;
	   break;
	   case T_VRML_Normal:
			tS->n=(struct vrmlNormal *)o;
	   break;
	   case T_VRML_Coordinate3:
			tS->p=(struct vrmlCoordinate3 *)o;
	   break;
	   case T_VRML_Material:
			tS->m=(struct vrmlMaterial *)o;
	   break;
	   case T_VRML_MaterialBinding:
			tS->mb=(struct vrmlMaterialBinding *)o;
	   break;
	   case T_VRML_Separator:
	   case T_VRML_TransformSeparator:
	   case T_VRML_Group:
	   case T_VRML_Switch:
	    s=(struct vrmlSeparator *)o;

	    if(o->type == T_VRML_Group){
	        ;
	    }else if(o->type == T_VRML_TransformSeparator){
	        GroupToScreen = tS->GroupToScreen;
	    }else{
	        tSsave = *tS;
	    }

	    for(n=0;n<s->Count;++n){
	        if(s->o && s->o[n]){
	            if(CVrmlDrawObjects(s->o[n],tS)){
					*tS = tSsave;
	                return 1;
	            }
	        }
	    }
	    if(o->type == T_VRML_Group){
	        ;
	    }else if(o->type == T_VRML_TransformSeparator){
	        tS->GroupToScreen=GroupToScreen;
	    }else{
	        *tS = tSsave;
	    }
	    break;
	   case T_VRML_Use:
	    u=(struct vrmlUse *)o;
	    if(u->o){
	        return CVrmlDrawObjects(u->o,tS);
	    }
	    break;
	   case T_VRML_IndexedLineSet:
	    break;
	   default:
		sprintf(WarningBuff,"default type %ld \n",(long)(o->type));
		WarningBatch(WarningBuff);
	}

	return 0;
}
static int CVrmlMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CVrmlPtr2 t=(CVrmlPtr2)o;
	struct vrmlList *vrml;
	long NodeCount;
	long CellCount;

	vrml = &t->scene->vrml[t->VrmlNumber];

	NodeCount=vrml->MostNodes;
	CellCount=vrml->MostCells*5;
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	*MostNormals=1;
	return 0;
}
static int CVrmlCount(CObject *o,long *nodes,long *cells)
{
	//CVrmlPtr2 t=(CVrmlPtr2)o;

	if(!o || !nodes || !cells)return 1;
	//t=t;
	*nodes += 4;
	*cells += 1;
	return 0;
}
static CObjectPtr CVrmlDuplicate(CObject *b)
{
	CVrmlPtr2 s=(CVrmlPtr2)b;
	CVrmlPtr2 sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CVrmlPtr2)cMalloc(sizeof(CVrml2),8541);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}

