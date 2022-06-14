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


static struct ContourGlobal{
	char *Used;
	double *x1,*y1,*x2,*y2;
	int treesize;
	struct tree *pfind;
	struct tree *xtree,*ytree;
	int inc;
	double tDiff;
} cg;

extern char *strpathname(char *path,char *name);

int CObjectSaveLamp(IconPtr myIcon,int number,Light *lamp);

int CObjectReadFileList(IconPtr myIcon);

int CObjectReadFile(char *command,IconPtr myIcon);

int doNoShadowLines(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene);	

int dupBlock(struct Block *block,struct Block *blockout);

int elementsplitF(struct Block *b,float **vi,long ncount);

CObjectPtr CPio3dStreamLinesCreate(struct Scene *scene,struct FileInfo2 *files,char *streamName);

static CObjectPtr uShowWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);

static int CSds3dkill(CObject *o);

CObjectPtr CHdfRotDuplicate(CObject *b);

CObjectListPtr duplicateObjectList(CObjectListPtr o);

int adjustPalette(struct Scene *scene,int mat);

int doNoShadowLines4(long *ndata,double *xdata,double *ydata,double *zdata,double *vdata,
		long nel,long CellCount,struct Scene *scene,int DrawZonesColor);	

int blockFree(struct Block *block);

char *StreamLine3D(struct Scene *scene);

int doPloti(struct Block *b,struct LIMITS *limits,double t,double *data,long imax,long jmax,long kmax);

int PrintSystem(struct System *s,char *name);

int SetNormalsBlock(struct Block *block);

int SetBlockRS(struct Block *block);

int elementrange(struct Block *block);

int blockCells(struct Block *b,long mat,long ns,long *nee,long xsize,long ysize);

int findAngles(struct System *Local);
int rotate(struct System *Local);

int blockFix(struct Block *b,long mat,long xsize,long ysize,double xoff,double yoff);

int MergeBlock(struct Scene *scene,struct Block *block);

void ScaleMatrix2(struct Matrix *m,struct System *p);
void MultMatrix(struct Matrix *t,struct Matrix *t2,struct Matrix *t1);
void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);


void CObjectCreate(struct Scene *scene,CObject *b);
int ListAddObjectList(CObjectList *d,CObject *Object);
int ObjectListAddBlock(struct Scene *scene,long k,long mat);

int CObjectkill(CObject *o);

int ObjectListAddFiles(struct FileInfo2 *files,struct Scene *scene);

int PrintMatrix(struct Matrix *m,char *name);

CBlockPtr CBlockCreate(struct Scene *scene,long BlockNumber,long Material,
			struct P *origin);

CSpherePtr CSphereCreate(struct Scene *scene,int mat,int nx,int ny,
                         double radius,struct P *origin);

CTorusPtr CTorusCreate(struct Scene *scene,int mat,int nx,int ny,
                         double rbig,double rsmall,struct P *origin);

CDiskPtr CDiskCreate(struct Scene *scene,int mat,int nx,int ny,
                         double rin,double rout,struct P *origin);

CConePtr CConeCreate(struct Scene *scene,int mat,int nx,int ny,
                         double rtop,double rbottom,double length,
                         struct P *origin);

CGroupPtr CGroupCreate(struct Scene *scene,long ObjectCount);

CWorldPtr CWorldCreate(struct Scene *scene);

CSds2dRotPtr CSds2dRotCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);

int CHdfRotkill(CObject *o);

CSds2dExtPtr CSds2dExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);

int CSds2dExtkill(CObject *o);

int CSds2dExtLoad(CSds2dExtPtr s,long Image);

CHdfRotPtr CHdfRotCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);

CHdfExtPtr CHdfExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);

int CHdfExtkill(CObject *o);

int CHdfExtLoad(CHdfExtPtr s,long itIs);

CSds3dPtr CSds3dCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);

CPio2dRotPtr CPio2dRayCreate(struct Scene *scene,struct FileInfo2 *Files);

CShowPtr CShowCreate(struct Scene *scene,struct FileInfo2 *Files,int flag);

int CShowLoad(CShowPtr s,long itIs);

CObjectPtr CShowDuplicate(CObject *b);

int CShowkill(CObject *o);

int FilesGetShow(struct Block *b,long CurrentFrame,struct FileInfo2 *Files);




int CheckDraw(struct cdatal *d);

int LineCountours(double *x,double *y,double *value,long *material,
                 double *levels,long nx,long ny,long nl,struct cdatal *d);

int Transform(struct P *p,struct Matrix *m);

int elementsplit8(struct Block *b);
int elementaverage(struct Block *b);
int elementconstant(struct Block *b);
int elementsplit(struct Block *b,double **vi,long ncount);
int elementint8(struct Block *b);
int gethrst(double *h,double *hr,double *hs,double *ht,double r,double s,double t);
int elementsvol(double *xd,double *yd,double *zd,double *volo);

int freeObjectList(CObjectListPtr o,int flag);
int Draw(double *xx,double *yy,long nn,long nlevel,struct cdatal *d);
struct tree *tree(struct tree *p,double  *x,long nseg);

struct tree *findtree(struct tree *p,double *x);
int Search(double *xx,double *yy,long *nn);
struct tree *freeTree(struct tree *p);
int CHdfExtMessage(CObject *b,struct ObjectMessage *Message);
CObjectPtr CHdfExtDuplicate(CObject *b);
int blockSize(struct Block *b,long mat,long nodes,long cells);
int CHdfRotLoad(CHdfRotPtr s,long itIs);
int CHdfRotMessage(CObject *b,struct ObjectMessage *Message);
CObjectPtr ChdfRotDuplicate(CObject *b);
int CSds2dExtMessage(CObject *b,struct ObjectMessage *Message);
CObjectPtr CSds2dExtDuplicate(CObject *b);
int CSds2dRotLoad(CSds2dRotPtr s,long itIs);

int CSds2dRotMessage(CObject *b,struct ObjectMessage *Message);
CObjectPtr CSds2dRotDuplicate(CObject *b);
int CSds2dRotkill(CObject *o);
CObjectPtr CSds3dDuplicate(CObject *b);
int CSds3dRotkill(CObject *o);
int CSds3dLoad(CSds3dPtr s,long itIs);
int CSds3dMessage(CObject *b,struct ObjectMessage *Message);
int doRenderKeys(char *line,IconPtr myIcon);
CObjectPtr CWorldDuplicate(CObject *b);
int checkWorld(struct Scene *scene,CObjectListPtr o,int flag);
int CConeDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);


int CObjectConeMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
CObjectPtr CConeDuplicate(CObject *b);
int CObjectConeCount(CObject *o,long *nodes,long *cells);
int CConeMessage(CObject *b,struct ObjectMessage *Message);
int CObjectdrawDisk(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
int CObjectDiskMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
CObjectPtr CDiskDuplicate(CObject *b);
int CObjectDiskCount(CObject *o,long *nodes,long *cells);
int CDiskMessage(CObject *b,struct ObjectMessage *Message);
int CTorusDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
int CTorusMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
CObjectPtr CTorusDuplicate(CObject *b);
int CTorusCount(CObject *o,long *nodes,long *cells);
int CTorusMessage(CObject *b,struct ObjectMessage *Message);


int CSphereDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
int CSphereMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells);
CObjectPtr CSphereDuplicate(CObject *b);
int CSphereCount(CObject *o,long *nodes,long *cells);
int CSphereMessage(CObject *b,struct ObjectMessage *Message);
int ListObjectKill(CObjectList *d);
int ListObjectMessage(CObjectListPtr o,struct ObjectMessage *Message);
int ListObjectDraw(CObjectListPtr o,long *CellCount,int flag);
int ListCountObjects(CObjectList *d,long *nodes,long *cells);
int ListGetMost(CObjectList *d,long *MostNodes,long *MostNormals,long *MostCells);
int CObjectGetMost(CObject *o,long *MostNodes,long *MostNormals,long *MostCells);
int CObjectCount(CObject *o,long *nodes,long *cells);


int CObjectdrawObject(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
static CObjectPtr CObjectDuplicate(CObject *b);
int CObjectMessage(CObject *b,struct ObjectMessage *Message);
int CBlockMost(CObject *o,long *MostNodes,long *MostNormals,long *MostCells);
int CBlockCount(CObject *o,long *nodes,long *cells);
int doBlockValues(struct Block *block,double *vdata,struct Image *ImageData);
int CBlockDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
CObjectPtr CBlockDuplicate(CObject *bb);
int CBlockMessage(CObject *b,struct ObjectMessage *Message);
int CObjectGroupMost(CObject *o,long *MostNodes,long *MostNormals,long *MostCells);
int CObjectGroupMessage(CObject *o,struct ObjectMessage *Message);
int CObjectGroupCount(CObject *o,long *nodes,long *cells);
int CObjectdrawGroup(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag);
static CObjectPtr CGroupDuplicate(CObject *bb);
int CShowMessage(CObject *b,struct ObjectMessage *Message);

int doSphere(IconPtr myIcon,CommandPtr cp);

CObjectPtr ListGetObjectPtr(int type,CObjectList *l);

CObjectPtr uVrmlListWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uSphereWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uDiskWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uConeWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uTorusWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uHdfExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uSds2dExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CObjectPtr uSds3dWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uHdfRotWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uSds2dRotWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uBlockWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uGroupWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
static CObjectPtr uWorldWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CObjectPtr uFlightPathWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);
CObjectPtr uPio3dStreamWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l);

static int writeReadNextMaterial(struct Scene *scene,long n,FILE8 *inOut,int flag);
static int writeReadNextLight(struct Scene *scene,long n,FILE8 *inOut,int flag);

int ListReadWriteObjectList(struct Scene *scene,CObjectList *l,FILE8 *inOut,int flag)
{
	static struct CObjectListStruct9{	
		float *itemList;
		long BlockCount;
    	long MaterialCount;
    	long nLights;
	}ll;

	static struct universalTableListStruct Local[] = {
		{"REAL",3319,universal_TypeRealArrayPtr,(void *)&ll.itemList,0L},				
		{"REAL",3320,universal_TypeLong,(void *)&ll.BlockCount},						
		{"REAL",3321,universal_TypeLong,(void *)&ll.MaterialCount},						
		{"REAL",3322,universal_TypeLong,(void *)&ll.nLights},						
	};
	
	static struct universalTableStruct Global = {
		"Render parameters",3200,
		(void *)&ll,sizeof(ll),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};
	
	
	char head[5];
	long length;
	long tag;
	int n,nn,ret;
	
	if(!scene || !l || !inOut)return 1;


	ret=1;	
	
	if(flag == 1){
		if(l->oCount > 0){
		    if(ll.itemList)cFree((char *)ll.itemList);
		    ll.itemList=(float *)cMalloc(l->oCount*sizeof(float),8327);
		    if(!ll.itemList)return 1;
		    
		    nn=0;
		    for(n=0;n<l->oCount;++n){
		        if(!l->oCurrent[n] || !l->oCurrent[n]->ReadWrite)continue;
		    	ll.itemList[nn++]=(float)l->oCurrent[n]->type;
		    }
	    }else{
		    nn=0;
	    }
	    ll.BlockCount=scene->BlockCount;
	    ll.MaterialCount=scene->MaterialUsed;
	    ll.nLights=scene->nLights;
	    Local[0].space=nn;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	    if(nn <= 0)return 0;
	    for(n=0;n<l->oCount;++n){
	        if(!l->oCurrent[n] || !l->oCurrent[n]->ReadWrite)continue;
	        if((*l->oCurrent[n]->ReadWrite)(l->oCurrent[n],inOut,l) == NULL)goto ErrorOut;
	    }
	    if(ll.itemList)cFree((char *)ll.itemList);
	    ll.itemList=NULL;
	    if(ll.MaterialCount > 0){
	    	for(n=0;n<ll.MaterialCount;++n){
	    		writeReadNextMaterial(scene,n,inOut,1);
	    	}
	    }
	    if(ll.nLights > 0){
	    	for(n=0;n<ll.nLights;++n){
	    		writeReadNextLight(scene,n,inOut,1);
	    	}
	    }
	}else{
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    zerol((char *)&ll,sizeof(ll));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
	    if(ll.BlockCount > scene->BlockCount){
			struct Block b;
			for(n=scene->BlockCount;n<ll.BlockCount;++n){
				zerol((char *)&b,sizeof(struct Block));
				MergeBlock(scene,&b);
			}	
	    }
		nn=(int)Local[0].space;
		if(nn <= 0 || !ll.itemList)goto ErrorOut;
	    for(n=0;n<nn;++n){
			CObjectPtr Return,Object=ListGetObjectPtr((int)ll.itemList[n],l);
			if(!Object || !Object->ReadWrite)continue;
	        if((Return = (*Object->ReadWrite)(NULL,inOut,l)) == NULL)continue;
		    if(ListAddObjectList(l,Return)){
		        (*Return->killObject)(Return);
		        goto ErrorOut;
		    }
	    }
	    if(ll.itemList)cFree((char *)ll.itemList);
	    ll.itemList=NULL;
	    
	    if(ll.MaterialCount > scene->MaterialUsed){
	        nn=ll.MaterialCount-scene->MaterialUsed;
	    	for(n=0;n<nn;++n){
	    		getNextMaterial(scene);
	    	}
	    }
	    	    
	    if(ll.MaterialCount > 0){
	    	for(n=0;n<ll.MaterialCount;++n){
	    		writeReadNextMaterial(scene,n,inOut,0);	    		
				uMemcpy(scene->pd.palette,scene->material[n].pd.palette,768);	    		
	        	adjustPalette(scene,n);
	    	}
	    }
	    if(ll.nLights > 0){
	        if((ll.nLights > scene->nLights) || !scene->Lights){
	            if(scene->Lights)cFree((char *)scene->Lights);
	            scene->nLights=0;
	            scene->Lights=(struct t_light *)cMalloc(ll.nLights*sizeof(struct t_light),8607);
	            if(!scene->Lights)goto ErrorOut;
	            scene->nLights=ll.nLights;
	        }
	    	for(n=0;n<ll.nLights;++n){
	    		writeReadNextLight(scene,n,inOut,0);
	    	}
	    }
	}
	ret=0;
ErrorOut:
	return ret;
}


static int writeReadNextLight(struct Scene *scene,long n,FILE8 *inOut,int flag)
{
	static struct t_light Lights;
	
	static struct universalTableListStruct Local[] = {
		{"DOUB",3320,universal_TypeDouble,(void *)&Lights.light_pos[0]},						
		{"DOUB",3321,universal_TypeDouble,(void *)&Lights.light_pos[1]},						
		{"DOUB",3322,universal_TypeDouble,(void *)&Lights.light_pos[2]},	
							
		{"DOUB",3323,universal_TypeDouble,(void *)&Lights.light_brightness},						
		{"DOUB",3324,universal_TypeDouble,(void *)&Lights.light_bright},						
		{"DOUB",3325,universal_TypeDouble,(void *)&Lights.red},						
		{"DOUB",3326,universal_TypeDouble,(void *)&Lights.green},						
		{"DOUB",3327,universal_TypeDouble,(void *)&Lights.blue},		
		{"REAL",3328,universal_TypeLong,(void *)&Lights.type},		
		
		
	};
	
	static struct universalTableStruct Global = {
		"Render parameters",3200,
		(void *)&Lights,sizeof(Lights),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};
	struct t_light *bp;
	char head[5];
	long length;
	long tag;
	int ret;
	
	if(!scene || !inOut)return 1;
	if(n < 0 || n >= scene->nLights)return 1;
	
	ret=1;	
	
	if(flag == 1){
	    Lights = scene->Lights[n];
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	}else{
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    zerol((char *)&Lights,sizeof(Lights));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp = &scene->Lights[n];
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}
static int writeReadNextMaterial(struct Scene *scene,long n,FILE8 *inOut,int flag)
{
	static struct Material mat;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",3320,universal_TypeInt,(void *)&mat.pd.sType},						
		{"DOUB",3321,universal_TypeDouble,(void *)&mat.pd.sPmin},						
		{"DOUB",3322,universal_TypeDouble,(void *)&mat.pd.sPmax},						
		{"STRI",3323,universal_TypeBytes,(void *)&mat.pd.palette[0],sizeof(mat.pd.palette)},

		{"DOUB",3324,universal_TypeDouble,(void *)&mat.diffuse.red},						
		{"DOUB",3325,universal_TypeDouble,(void *)&mat.diffuse.green},						
		{"DOUB",3326,universal_TypeDouble,(void *)&mat.diffuse.blue},						

		{"DOUB",3327,universal_TypeDouble,(void *)&mat.ambient.red},						
		{"DOUB",3328,universal_TypeDouble,(void *)&mat.ambient.green},						
		{"DOUB",3329,universal_TypeDouble,(void *)&mat.ambient.blue},						

		{"DOUB",3330,universal_TypeDouble,(void *)&mat.opacity},						
		{"DOUB",3331,universal_TypeDouble,(void *)&mat.specular},						
		{"DOUB",3332,universal_TypeDouble,(void *)&mat.phong},	
		
		
			
		{"FILE",3340,universal_TypeFilesPtr,(void *)&mat.ImageData.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"Render parameters",3200,
		(void *)&mat,sizeof(mat),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};
	struct Material *bp;
	char head[5];
	long length;
	long tag;
	int ret;
	
	if(!scene || !inOut)return 1;
	if(n < 0 || n >= scene->MaterialCount)return 1;
	
	ret=1;	
	
	if(flag == 1){
	    mat = scene->material[n];
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;		
	}else{
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    zerol((char *)&mat,sizeof(mat));
	    getPaletteByName("ps",(unsigned char *)&mat.pd.palette);
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp = &scene->material[n];
		if(uPref.Files){
			bp->ImageData.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(bp->ImageData.Files){
			    bp->PatternType = PATTERN_IMAGE;
			    bp->ImageData.tmax=bp->ImageData.Files->ImageCount;				    
			    bp->ImageData.xmax=bp->ImageData.Files->xmax;
			    bp->ImageData.ymax=bp->ImageData.Files->ymax;
		    }else{
		        goto  ErrorOut;
		    }
		}
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
	}
	ret=0;
ErrorOut:
	return ret;
}

CObjectPtr ListGetObjectPtr(int type,CObjectList *l)
{
	static struct CWorldStruct World;
	static struct CGroupStruct Group;
	static struct CBlockStruct Block;
	static struct CSphereStruct Sphere;
	static struct CTorusStruct Torus;	
	static struct CDiskStruct Disk;
	static struct CConeStruct Cone;
	static struct CSds2dExtStruct Sds2dExt;
	static struct CSds2dRotStruct Sds2dRot;
	static struct CHdfExtStruct HdfExt;
	static struct CHdfRotStruct HdfRot;
	static struct CSds3dStruct Sds3d;
	static struct CVrmlListStruct VrmlList;
	static struct CVrmlStruct2 Vrml2;
	static struct CShowStruct Show;
	static struct CPio2dExtStruct Pio2dExt;
	static struct CPio2dRotStruct Pio2dRot;
	static struct CPio3dLevelStruct Pio3dLevel;
	static struct CPio3dExtStruct Pio3dExt;
	static struct CPio2dRayStruct Pio2dRay;
	static struct CPio2dTracerStruct Pio2dTracer;
	static struct CFlightPathStruct flightPath;
	static struct CStlStruct Stl;
	static struct CPio3dStreamLinesStruct StreamLines;
	static struct CVrmlStruct Vrml;
	void *o;

		//l=l;
	
		
	    switch(type)
	    {
	    	        	
	        	
	        case G_SPHERE:
				zerol((char *)&Sphere,sizeof(struct CSphereStruct));
				Sphere.ReadWrite=uSphereWrite;
				o=&Sphere;
	        	return (CObjectPtr)o;
	        	
	        case G_CONE:
				zerol((char *)&Cone,sizeof(struct CConeStruct));
				Cone.ReadWrite=uConeWrite;				
				o=&Cone;
	        	return (CObjectPtr)o;
	        	
	        case G_DISK:
				zerol((char *)&Disk,sizeof(struct CDiskStruct));
				Disk.ReadWrite=uDiskWrite;				
				o=&Disk;
	        	return (CObjectPtr)o;
	        	
	        case G_TORUS:
				zerol((char *)&Torus,sizeof(struct CTorusStruct));
				Torus.ReadWrite=uTorusWrite;				
				o=&Torus;
	        	return (CObjectPtr)o;
	        	
	        case G_BLOCK:
				zerol((char *)&Block,sizeof(struct CBlockStruct));
				Block.ReadWrite=uBlockWrite;				
				o=&Block;
	        	return (CObjectPtr)o;
	        	
	        case G_GROUP:
				zerol((char *)&Group,sizeof(struct CGroupStruct));
				Group.ReadWrite=uGroupWrite;				
				o=&Group;
	        	return (CObjectPtr)o;
	        	
	        case G_WORLD:
				zerol((char *)&World,sizeof(struct CWorldStruct));
				World.ReadWrite=uWorldWrite;				
				o=&World;
	        	return (CObjectPtr)o;
				
	        case G_EXTRUDE_SDS2D:
				zerol((char *)&Sds2dExt,sizeof(struct CSds2dExtStruct));
				Sds2dExt.ReadWrite=uSds2dExtWrite;
				o=&Sds2dExt;
	        	return (CObjectPtr)o;
	        	           
	        case G_ROTATE_SDS2D:
				zerol((char *)&Sds2dRot,sizeof(struct CSds2dRotStruct));
				Sds2dRot.ReadWrite=uSds2dRotWrite;				
				o=&Sds2dRot;
	        	return (CObjectPtr)o;
	        	          
	        case G_EXTRUDE_HDF:
				zerol((char *)&HdfExt,sizeof(struct CHdfExtStruct));
				HdfExt.ReadWrite=uHdfExtWrite;				
				o=&HdfExt;
	        	return (CObjectPtr)o;
	        	           
	        case G_ROTATE_HDF:
				zerol((char *)&HdfRot,sizeof(struct CHdfRotStruct));
				HdfRot.ReadWrite=uHdfRotWrite;								
				o=&HdfRot;
	        	return (CObjectPtr)o;
	        	      	
	        case G_LEVEL_SDS3D:
				zerol((char *)&Sds3d,sizeof(struct CSds3dStruct));
				Sds3d.ReadWrite=uSds3dWrite;								
				o=&Sds3d;
	        	return (CObjectPtr)o;
	        	
	        case G_VRML_LIST2:
				zerol((char *)&VrmlList,sizeof(struct CVrmlListStruct));
				VrmlList.ReadWrite=uVrmlListWrite;								
				o=&VrmlList;
	        	return (CObjectPtr)o;
	        	
	        case G_VRML2:
				zerol((char *)&Vrml2,sizeof(struct CVrmlStruct2));
				o=&Vrml2;
	        	return (CObjectPtr)o;
	        	
	        case G_VRML:
				zerol((char *)&Vrml,sizeof(struct CVrmlStruct));
				Vrml.ReadWrite=uVrmlWrite;								
				o=&Vrml;
	        	return (CObjectPtr)o;
	        	
	        case G_SHOW:
				zerol((char *)&Show,sizeof(struct CShowStruct));
				Show.ReadWrite=uShowWrite;
				o=&Show;
	        	return (CObjectPtr)o;
	        	
	        case G_EXTRUDE_PIO2D:
				zerol((char *)&Pio2dExt,sizeof(struct CPio2dExtStruct));
				Pio2dExt.ReadWrite=uPio2dExtWrite;				
				o=&Pio2dExt;
	        	return (CObjectPtr)o;
	        	
	        case G_ROTATE_PIO2D:
				zerol((char *)&Pio2dRot,sizeof(struct CPio2dRotStruct));
				Pio2dRot.ReadWrite=uPio2dRotWrite;								
				o=&Pio2dRot;
	        	return (CObjectPtr)o;
	        	
	        case G_RAY_PIO2D:
				zerol((char *)&Pio2dRay,sizeof(struct CPio2dRayStruct));
				Pio2dRay.ReadWrite=uPio2dRayWrite;								
				o=&Pio2dRay;
	        	return (CObjectPtr)o;
	        	
	        case G_LEVEL_PIO3D:
				zerol((char *)&Pio3dLevel,sizeof(struct CPio3dLevelStruct));
				Pio3dLevel.ReadWrite=uPio3dLevelWrite;												
				o=&Pio3dLevel;
	        	return (CObjectPtr)o;
	        	
	        case G_EXTRUDE_PIO3D:
				zerol((char *)&Pio3dExt,sizeof(struct CPio3dExtStruct));
				Pio3dExt.ReadWrite=uPio3dExtWrite;								
				o=&Pio3dExt;
	        	return (CObjectPtr)o;
	        	
	        case G_TRACER_PIO2D:
				zerol((char *)&Pio2dTracer,sizeof(struct CPio2dTracerStruct));
				Pio2dTracer.ReadWrite=uPio2dTracerWrite;								
				o=&Pio2dTracer;
	        	return (CObjectPtr)o;
	        	
	        case G_FLIGHT_PATH:
				zerol((char *)&flightPath,sizeof(struct CFlightPathStruct));
				flightPath.ReadWrite=uFlightPathWrite;
				o=&flightPath;
	        	return (CObjectPtr)o;
	        	
	        case G_STL:
				zerol((char *)&Stl,sizeof(struct CStlStruct));
				Stl.ReadWrite=CStlWrite;
				o=&Stl;
	        	return (CObjectPtr)o;
	        	
	        case G_STREAMLINE_PIO3D:
				zerol((char *)&StreamLines,sizeof(struct CPio3dStreamLinesStruct));
				StreamLines.ReadWrite=uPio3dStreamWrite;
				o=&StreamLines;
	        	return (CObjectPtr)o;
	        	
	        default:
	            sprintf(WarningBuff,"DOGetObjectPtr : Unknown Type (%ld)\n",(long)type);
	            WarningBatch(WarningBuff);
	            break;
	    }
	    return (CObjectPtr)NULL;
}

int ListObjectKill(CObjectList *d)
{
	CObjectPtr Current;
	long n;

	if(!d)return 1;
	for(n=0;n<d->oCount;++n){
	    if(d->oCurrent && ((Current=d->oCurrent[n]) != NULL)){
	        (*Current->killObject)(Current);
		d->oCurrent[n]=NULL;
	    }
	/*   Closing causes problems - not needed now
	    if(d->oOld && ((Current=d->oOld[n]) != NULL)){
	        (*Current->killObject)(Current);
		d->oOld[n]=NULL;
	    }
	*/
	}
	if(d->oOld)cFree((char *)d->oOld);
	d->oOld=NULL;
	if(d->oCurrent)cFree((char *)d->oCurrent);
	d->oCurrent=NULL;
	d->oCount=0;
	d->oMaximun=0;
	cFree((char *)d);
	return 0;
}
int ListObjectMessage(CObjectListPtr o,struct ObjectMessage *Message)
{
	struct System Global;
	struct System World;
	struct Matrix GlobalToScreen;
	struct Matrix WorldToGlobal;
	struct Matrix WorldToScreen;
	struct Scene *scene;
	CObjectPtr Current;
	long n;

	if(!o || !Message)return 1;

	Message->xmax = -1e60;
	Message->ymax = -1e60;
	Message->zmax = -1e60;
	Message->vmax = -1e60;
	
	Message->xmin =  1e60;
	Message->ymin =  1e60;
	Message->zmin =  1e60;
	Message->vmin =  1e60;
	
	Message->SurfaceCount =  0;
	
	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){
		startPaletteLevels(o->scene);
	}
	
	for(n=0;n<o->oCount;++n){
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    if(!Current->message)continue;
	    if(Message->MessageType == OBJECT_MESSAGE_FORCE_LOAD){
	        Current->ItWas= -1;	
	    }else if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){	
			scene=o->scene;
			World=scene->World;
			Global=scene->Global;
			GlobalToScreen=scene->GlobalToScreen;
			BuildTransformMatrix(&WorldToGlobal,&World,&Global);
			Message->WorldToScreen = &WorldToGlobal;
	        (*Current->message)(Current,Message);
	  
	    }else if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA_SCREEN){	
			scene=o->scene;
			World=scene->World;
			Global=scene->Global;
			GlobalToScreen=scene->GlobalToScreen;
			BuildTransformMatrix(&WorldToGlobal,&World,&Global);
			MultMatrix(&WorldToScreen,&GlobalToScreen,&WorldToGlobal);
			ScaleMatrix2(&WorldToScreen,&World);
			Message->WorldToScreen = &WorldToScreen;
			Message->MessageType=OBJECT_MESSAGE_RANGE_DATA;
	        (*Current->message)(Current,Message);
			Message->MessageType=OBJECT_MESSAGE_RANGE_DATA_SCREEN;
	    }else{
	        (*Current->message)(Current,Message);
	    }
	}
	
	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){
		setPaletteLevels(o->scene);
	}
	
	return 0;
}

int ListObjectDraw(CObjectListPtr o,long *CellCount,int flag)
{
	CObjectPtr Current;
	struct System Global;
	struct System World;
	struct Matrix GlobalToScreen;
	struct Matrix WorldToGlobal;
	struct Matrix WorldToScreen;
	struct Scene *scene;
	long n;

	if(!o || !CellCount)return 1;

	scene=o->scene;
	World=scene->World;
	Global=scene->Global;
	GlobalToScreen=scene->GlobalToScreen;


	BuildTransformMatrix(&WorldToGlobal,&World,&Global);
	MultMatrix(&WorldToScreen,&GlobalToScreen,&WorldToGlobal);
	ScaleMatrix2(&WorldToScreen,&World);

	
	for(n=0;n<o->oCount;++n){
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    (*Current->drawObject)(Current,&WorldToScreen,CellCount,flag);
	}
	return 0;
}
int ListCountObjects(CObjectList *d,long *nodes,long *cells)
{
	CObjectPtr Current;
	long n;

	if(!d || !nodes || !cells)return 1;
	*nodes = 0;
	*cells = 0;
	for(n=0;n<d->oCount;++n){
	    if((Current=d->oCurrent[n]) == NULL)continue;
	    (*Current->countObject)(Current,nodes,cells);
	}
	return 0;

}
int ListGetMost(CObjectList *d,long *MostNodes,long *MostNormals,long *MostCells)
{
	CObjectPtr Current;
	long n;

	if(!d || !MostNodes || !MostNormals)return 1;
	*MostNodes = 0;
	*MostNormals = 0;
	for(n=0;n<d->oCount;++n){
	    if((Current=d->oCurrent[n]) == NULL)continue;
	    (*Current->pGetMost)(Current,MostNodes,MostNormals,MostCells);
	}
	return 0;
}
int ListAddObjectList(CObjectList *d,CObject *Object)
{
	long Max,n;
	
	if(d->oCount < d->oMaximun-1){
Putin:
	    d->oCurrent[d->oCount++]=Object;
	}else{
		CObjectPtr *oOld,*oCurrent;

		Max= 2*d->oMaximun;
		if(Max <= 0)Max = 100;

		oOld =(CObjectPtr *)cMalloc(sizeof(CObjectPtr)*Max,8593);		
		if(oOld == NULL){
		    sprintf(WarningBuff,"Could Not Allocate Old Data List\n");
			WarningBatch(WarningBuff);
		    return 1;
		}

		oCurrent =(CObjectPtr *)cMalloc(sizeof(CObjectPtr)*Max,8594);		
		if(oCurrent == NULL){
		    if(oOld)cFree((char *)oOld);
		    sprintf(WarningBuff,"Could Not Allocate oCurrent Data List\n");
			WarningBatch(WarningBuff);
		    return 1;
		}

		for(n=0;n<Max;++n){
		    oOld[n]=NULL;
		    oCurrent[n]=NULL;
		}

		for(n=0;n<d->oCount;++n){
		    oOld[n]=d->oOld[n];
		    oCurrent[n]=d->oCurrent[n];
		}

		if(d->oOld)cFree((char *)d->oOld);
		if(d->oCurrent)cFree((char *)d->oCurrent);

		d->oOld=oOld;
		d->oCurrent=oCurrent;
		d->oMaximun=Max;
		goto Putin;
	}
	return 0;
}
CObjectPtr uSds3dWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CSds3dStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"REAL",5002,universal_TypeDoubleLongPtr,(void *)&b.blockNumbers,0L},				
		{"DOUB",5003,universal_TypeLong,(void *)&b.blockCount},	

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

		{"DOUB",5021,universal_TypeDouble,(void *)&b.sd.plane.Plane_xmin},	
		{"DOUB",5022,universal_TypeDouble,(void *)&b.sd.plane.Plane_ymin},	
		{"DOUB",5023,universal_TypeDouble,(void *)&b.sd.plane.Plane_zmin},	
		
		{"DOUB",5024,universal_TypeDouble,(void *)&b.sd.plane.Plane_xmax},	
		{"DOUB",5025,universal_TypeDouble,(void *)&b.sd.plane.Plane_ymax},	
		{"DOUB",5026,universal_TypeDouble,(void *)&b.sd.plane.Plane_zmax},	
		

		{"DOUB",5033,universal_TypeLong,(void *)&b.sd.plane.Plane_xcount},	
		{"DOUB",5034,universal_TypeLong,(void *)&b.sd.plane.Plane_ycount},	
		{"DOUB",5035,universal_TypeLong,(void *)&b.sd.plane.Plane_zcount},	


		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},
		
		
		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files2},	
		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CSds3dPtr bp=(CSds3dPtr)o;
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
	    Local[2].space=0;
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CSds3dCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    	if(uPref.Files2){
		    	     RenderAddPaint((CSds2dExtPtr)bp,uPref.Files2);
		    	     uPref.Files2=NULL;
		    	}
		    }else{
		        goto ErrorOut;
		    }
		}else{
			goto ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		bp->blockNumbers=b.blockNumbers;
		b.blockNumbers=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
	    Local[2].space=b.blockCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CSds3dPtr CSds3dCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	struct Block b;
	CSds3dPtr s;
	char buff[256];
	static int Count;
	int type;

	if(!scene || !Files)return (CSds3dPtr)NULL;


	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CSds3dPtr)NULL;

	s=(CSds3dPtr)cMalloc(sizeof(CSds3d),8569);
	if(!s)return (CSds3dPtr)NULL;
	zerol((char *)s,sizeof(CSds3d));

	sprintf(buff,"Sds3dLevel_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
        	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
	
	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_LEVEL_SDS3D;
	s->Files=Files;
	
	
	
	s->duplicateObject=CSds3dDuplicate;
	s->message=CSds3dMessage;
	s->killObject=CSds3dkill;	
	s->ReadWrite=uSds3dWrite;
	
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;	
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	
	
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CSds3dPtr)NULL; 
}
int CSds3dMessage(CObject *b,struct ObjectMessage *Message)
{
	CSds3dPtr s=(CSds3dPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CSds3dLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  Sds3D Level Surface\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    if(s->Files2){
	    		sprintf(WarningBuff,"  Sds3D Level Surface Paint\n%c",0);
	    		WarningBatch(WarningBuff);
		    	printFileInformation(s->Files2);
		    }
		    break;
		}
	    
	}
	return 0;
}
int CSds3dLoad(CSds3dPtr s,long itIs)
{
	struct Scene *scene;
	struct areadata ai;
	struct LIMITS limits;
	double vmin,vmax;
	double *data;
	long n,k;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;

	if((s->LevelCount <= 0) || !s->Levels)return 0;
	if(s->ItWas == itIs)return 1;

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    return 0;
	}

	if(s->ItWas == itIs)return 1;

	data=NULL;

	for(n=0;n<s->blockCount;++n){
	    if(s->blockNumbers){
			blockFree(&scene->block[s->blockNumbers[n]]);
	    }
	}

	if(s->LevelCount > s->blockCount){
	   long *blockNumbers;
	   blockNumbers=(long *)cMalloc(s->LevelCount*sizeof(long),8573);
	   if(!blockNumbers){
	       	sprintf(WarningBuff,"CSds3dLoad out of Memory\n");
			WarningBatch(WarningBuff);
	       	return 1;
	   }
	   zerol((char *)blockNumbers,s->LevelCount*sizeof(long));

	   for(n=0;n<s->blockCount;++n){
	       if(s->blockNumbers)blockNumbers[n]=s->blockNumbers[n];
	   }

	   if(s->blockNumbers)cFree((char *)s->blockNumbers);
	   s->blockNumbers=blockNumbers;

		for(n=s->blockCount;n<s->LevelCount;++n){
			struct Block bb;
			zerol((char *)&bb,sizeof(struct Block));
			if(MergeBlock(scene,&bb))return 1;
			s->blockNumbers[n]=scene->BlockCount-1;
			if(ObjectListAddBlock(scene,s->blockNumbers[n],s->Material))return 1;
		}
		s->blockCount=s->LevelCount;
	}
	

	zerol((char *)&ai,sizeof(struct areadata));
		
	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
	
	ai.xminArea=s->sd.plane.Plane_xmin;
	ai.yminArea=s->sd.plane.Plane_ymin;
	ai.zminArea=s->sd.plane.Plane_zmin;
	
	ai.xmaxArea=s->sd.plane.Plane_xmax;
	ai.ymaxArea=s->sd.plane.Plane_ymax;
	ai.zmaxArea=s->sd.plane.Plane_zmax;
	
	ai.xsize=s->sd.plane.Plane_xcount;
	ai.ysize=s->sd.plane.Plane_ycount;
	ai.zsize=s->sd.plane.Plane_zcount;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CSds3dLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}
	
	data=ai.data;
	
	zerol((char *)&limits,sizeof(limits));
	
	limits.iGotData=63;
	
	limits.xmin=ai.xminArea;
	limits.ymin=ai.yminArea;
	limits.zmin=ai.zminArea;
	
	limits.xmax=ai.xmaxArea;
	limits.ymax=ai.ymaxArea;
	limits.zmax=ai.zmaxArea;
		
	vmin =  1e60;
	vmax = -1e60;
	for(n=0;n<s->LevelCount;++n){
	    struct Block *bp;
	    bp = &(scene->block[s->blockNumbers[n]]);
	    if(doPloti(bp,&limits,s->Levels[n],(double *)ai.data,ai.xsize,
	               ai.ysize,ai.zsize)){
	        break;
	    }	  
	    
	    if(bp->Elements){
	        for(k=0;k<bp->ElementCount;++k){
	           bp->Elements[k]=s->Material;
	           k += bp->NodesMax;
	        } 
	    }

	    if(scene->SmoothNormals){
	        SetNormalsBlock(bp);
	    }
	    if(s->usePaint && s->PaintName[0]){
		    struct linedata li;
			zerol((char *)&li,sizeof(struct linedata));
		    li.pioIndex=s->pioIndexPaint;
		    if(s->pioNamePaint[0]){
		        mstrncpy(li.pioName,s->pioNamePaint,64);
		    }else{
		        mstrncpy(li.pioName,"density",64);
		    }
			li.x=bp->xdata;
			li.y=bp->ydata;
			li.z=bp->zdata;
			li.v=bp->vdata;
			li.PointLineCount=(int)bp->NodeCount;
			li.type=LINEDATA_LIST_DATA;
			li.CurrentFrame=itIs;
			if(SendMessageByName(s->PaintName,MessageType_GET_LINE_DATA,&li)){
				sprintf(WarningBuff,"CSds3dLoad -  pioName (%s) Error Getting Paint Data\n",li.pioName);
				WarningBatch(WarningBuff);
				continue;
			}	    	    
	    }
	    elementrange(bp);
	    if(bp->vmin < vmin)vmin=bp->vmin;
	    if(bp->vmax > vmax)vmax=bp->vmax;
	}
	
	checkPaletteLevels(scene,s->Material,vmin,vmax);

	s->ItWas = itIs;
	
ErrorOut:

	if(data)cFree((char *)data);

	return 0;
}
static int CSds3dkill(CObject *o)
{
	CSds3dPtr s=(CSds3dPtr)o;
	if(!s)return 1;

	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->blockNumbers)cFree((char *)s->blockNumbers);
	s->blockNumbers=NULL;
	s->blockCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
		if(s->PaintName[0])FileManagerCloseName(s->PaintName);
	}

	cFree((char *)s);
	return 0;
}
CObjectPtr CSds3dDuplicate(CObject *b)
{
	CSds3dPtr s=(CSds3dPtr)b;
	CSds3dPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CSds3dPtr)cMalloc(sizeof(CSds3d),8567);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;
	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}
	
	if(s->blockNumbers && s->blockCount){
	   sdsNew->blockNumbers=(long *)cMalloc(sizeof(long)*s->blockCount,8572);
	   if(!sdsNew->blockNumbers){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->blockCount;++n){
	       sdsNew->blockNumbers[n]=s->blockNumbers[n];
	   }
	}else{
	    sdsNew->blockNumbers=NULL;
	}
	
	return (CObjectPtr)sdsNew;
}


static CObjectPtr uHdfRotWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CHdfRotStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"DOUB",4016,universal_TypeLong,(void *)&b.Axis},	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.Angle},	
		{"DOUB",4019,universal_TypeLong,(void *)&b.PassSmooth},	
		{"DOUB",4020,universal_TypeLong,(void *)&b.Angles},	


		{"DOUB",4021,universal_TypeDouble,(void *)&b.sd.range.xminData},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.sd.range.yminData},	

		{"DOUB",4023,universal_TypeDouble,(void *)&b.sd.range.xmaxData},	
		{"DOUB",4024,universal_TypeDouble,(void *)&b.sd.range.ymaxData},	

		{"DOUB",4025,universal_TypeLong,(void *)&b.sd.range.ixmax},	
		{"DOUB",4026,universal_TypeLong,(void *)&b.sd.range.iymax},	

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},

		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files2},	

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CHdfRotPtr bp=(CHdfRotPtr)o;
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
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CHdfRotCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    	if(uPref.Files2){
		    	     RenderAddPaint((CSds2dExtPtr)bp,uPref.Files2);
		    	     uPref.Files2=NULL;
		    	}
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CHdfRotLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}

CHdfRotPtr CHdfRotCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	struct Block b;
	CHdfRotPtr s;
	char buff[256];
	static int Count;
	int type;

	if(!scene || !Files)return (CHdfRotPtr)NULL;


	if(flag){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CHdfRotPtr)NULL;
	}
	
	s=(CHdfRotPtr)cMalloc(sizeof(CHdfRot),8569);
	if(!s)return (CHdfRotPtr)NULL;
	zerol((char *)s,sizeof(CHdfRot));

	sprintf(buff,"HdfRot_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);	

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_ROTATE_HDF;
	s->Files=Files;
		
	s->duplicateObject=CHdfRotDuplicate;
	s->message=CHdfRotMessage;
	s->killObject=CHdfRotkill;
	s->ReadWrite=uHdfRotWrite;
		
	s->BlockNumber=scene->BlockCount-1;
	s->vscale=1.0;
	s->Angles=20;
	s->Axis=0;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->xmax = (int)s->sd.range.ixmax;
	s->ymax = (int)s->sd.range.iymax;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->Angle=180;
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CHdfRotPtr)NULL;
	}
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CHdfRotPtr)NULL; 
}
int CHdfRotMessage(CObject *b,struct ObjectMessage *Message)
{
	CHdfRotPtr s=(CHdfRotPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CHdfRotLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  Hdf Raster Rotate\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    if(s->Files2){
	    		sprintf(WarningBuff,"  Hdf Raster Rotate Paint\n%c",0);
	    		WarningBatch(WarningBuff);
		    	printFileInformation(s->Files2);
		    }
		    break;
		}
	    
	}
	return 0;
}
int CHdfRotLoad(CHdfRotPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct cdatal d;
	struct Block *b;
	double *data;
	double *dat;
	long ne,n,nn,ns;
	long i,j,nodes,cells;
	long xdiff,ydiff;
	double xoff,yoff;
	struct areadata ai;
	double dx,dy;
	long nee,ne1,ne2,n1,n2;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if((s->LevelCount <= 0) || !s->Levels)return 0;
	if(s->ItWas == itIs)return 1;

	b = &(scene->block[s->BlockNumber]);

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}
	
	if(s->ItWas == itIs)return 1;
	
	data=NULL;

	d.cList=NULL;

	zerol((char *)&ai,sizeof(struct areadata));

	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
		
	ai.xminArea=s->sd.range.xminData;
	ai.yminArea=s->sd.range.yminData;
	ai.zminArea=0;
	ai.xmaxArea=s->sd.range.xmaxData;
	ai.ymaxArea=s->sd.range.ymaxData;
	ai.zmaxArea=0;
	ai.xsize=s->sd.range.ixmax;
	ai.ysize=s->sd.range.iymax;;
	ai.zsize=0;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CHdfRotLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	s->ai=ai;

	dx=(ai.xmaxArea-ai.xminArea)/(double)xsize;
	dy=(ai.ymaxArea-ai.yminArea)/(double)ysize;

	xdiff=xsize;
	ydiff=ysize;
	xoff=0.;
	yoff=0.;

	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;

	dat=(double *)data;
	for(j=0;j<ysize;++j){
	    for(i=0;i<xsize;++i){
			ne=i+(j)*xsize;
			b->Elements[ne]=s->Material;
			b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
			b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
			b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
	    }
	}

	zerol((char *)&d,sizeof(struct cdatal));

	LineCountours(b->xdata,b->ydata,b->vdata,b->Elements,
	             s->Levels,xdiff,ydiff,s->LevelCount,&d);

	nodes=0;
	cells=0;
	for(n=0;n<d.count;++n){
	    nodes += d.cList[n].nndata;
	    cells += d.cList[n].nndata-1;
	}


	if(blockSize(b,s->Material,nodes*s->Angles,cells*(s->Angles-1)))return 1;

	ne=0;
	nn=0;
	for(n=0;n<d.count;++n){
	    double *xx,*yy,*vv;
	    double r,sine,cosine,afac,v;

	    afac=(8.0/360.)*atan(1.0)*s->Angle/((double)(s->Angles)-1.0);

	    nodes = d.cList[n].nndata;
	    cells = d.cList[n].nndata-1;
	    xx=d.cList[n].xdata;
	    yy=d.cList[n].ydata;
	    vv=d.cList[n].vdata;
	    if(!xx || !yy || !vv)continue;
	    v=s->Levels[d.cList[n].nlevel];
	    ns=nn;
	    
	    if(s->usePaint && s->PaintName[0]){
		    struct linedata li;
			zerol((char *)&li,sizeof(struct linedata));
		    li.pioIndex=s->pioIndexPaint;
		    if(s->pioNamePaint[0]){
		        mstrncpy(li.pioName,s->pioNamePaint,64);
		    }else{
		        mstrncpy(li.pioName,"density",64);
		    }
			li.x=xx;
			li.y=yy;
			li.z=NULL;
			li.v=vv;
			li.PointLineCount=(int)nodes;
			li.type=LINEDATA_LIST_DATA;
			li.CurrentFrame=itIs;
			if(SendMessageByName(s->PaintName,MessageType_GET_LINE_DATA,&li)){
				sprintf(WarningBuff,"CHdfRotLoad -  pioName (%s) Error Getting Paint Data\n",li.pioName);
				WarningBatch(WarningBuff);
				continue;
			}	    	    
	    }else{
	    	for(i=0;i<nodes;++i){
	    	    vv[i]=v;
	    	}
	    }
	    
	    for(j=0;j<s->PassSmooth;++j){
	        for(i=2;i<nodes;++i){
	            yy[i-1]=(.5*yy[i-1]+.25*yy[i-2]+.25*yy[i]);
	            xx[i-1]=(.5*xx[i-1]+.25*xx[i-2]+.25*xx[i]);
	        }
	    }

	    if(s->iWrite){
	        sprintf(WarningBuff,"nodes %ld",nodes);
	        WarningBatchFast(WarningBuff);
	        for(i=0;i<nodes;++i){
	        	sprintf(WarningBuff,"%g%c%g",xx[i],9,yy[i]);
	        	WarningBatchFast(WarningBuff);
	        }
	    }

	    if(s->Axis == 0){
	        for(j=0;j<s->Angles;++j){
	            sine=sin((double)j*afac);
		    	cosine=cos((double)j*afac);
	            for(i=0;i<nodes;++i){
	                r=sqrt(yy[i]*yy[i]);
					b->xdata[nn]=xx[i];
					b->ydata[nn]=(r*cosine);
					b->zdata[nn]=(r*sine);
					b->vdata[nn]=vv[i];
					++nn;
	            }
	        }
	    }else {
	        for(j=0;j<s->Angles;++j){
	            sine=sin((double)j*afac);
		    	cosine=cos((double)j*afac);
	            for(i=0;i<nodes;++i){
	                r=sqrt(xx[i]*xx[i]);
					b->xdata[nn]=(r*cosine);
					b->ydata[nn]=yy[i];
					b->zdata[nn]=(r*sine);
					b->vdata[nn]=vv[i];
					++nn;
	            }
	        }
	    }
	    
		if(blockCells(b,s->Material,ns,&ne,nodes,s->Angles))goto ErrorOut;
		if(s->Angle == 360){
		    
		    nee=ne/5;
		    
		    ne2=nee-(nodes-1);
		    ne1=nee-(nodes-1)*(s->Angles-1);
		    
            for(i=0;i<nodes-1;++i){
                n1=5*(ne1+i);
                n2=5*(ne2+i);
                b->Elements[n1+1]=b->Elements[n2+4];
                b->Elements[n1+2]=b->Elements[n2+3];
            }
		    
		    
		}	    
	}
	
	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    double dx,dy;
	    if(SetBlockRS(b))goto ErrorOut;
	    nn=0;
	    for(n=0;n<d.count;++n){
 
	        nodes = d.cList[n].nndata;
  	        ns=nn;

	        dx=0;
	        if(nodes > 1)dx=1.0/((double)(nodes-1));

	        dy=0;
	        if(s->Angles > 1)dy=1.0/((double)(s->Angles-1));
 	        for(j=0;j<s->Angles;++j){
 	            for(i=0;i<nodes;++i){
					b->sdata[nn]=(float)((i)*dx);
					b->rdata[nn]=(float)((j)*dy);
					++nn;
	            }
	        }
	    }
	}


	elementrange(b);

	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);

	s->ItWas = itIs;
	
ErrorOut:

	if(data)cFree((char *)data);

	if(d.cList){
	    for(n=0;n<d.count;++n){
	        if(d.cList[n].xdata){
	            cFree((char *)d.cList[n].xdata);
	            d.cList[n].xdata=NULL;
	        }
	        if(d.cList[n].ydata){
	            cFree((char *)d.cList[n].ydata);
	            d.cList[n].ydata=NULL;
	        }
	        if(d.cList[n].vdata){
	            cFree((char *)d.cList[n].vdata);
	            d.cList[n].vdata=NULL;
	        }
	    }
	    cFree((char *)d.cList);
	    d.cList=NULL;
	}
	
    if(s->iWrite){
		WarningBatchAtEnd(WarningBuff);        
    }

	return 0;
}
int CHdfRotkill(CObject *o)
{
	CHdfRotPtr s=(CHdfRotPtr)o;
	if(!s)return 1;

	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
		if(s->PaintName[0])FileManagerCloseName(s->PaintName);
	}

	cFree((char *)s);
	return 0;
}
CObjectPtr CHdfRotDuplicate(CObject *b)
{
	CHdfRotPtr s=(CHdfRotPtr)b;
	CHdfRotPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CHdfRotPtr)cMalloc(sizeof(CHdfRot),8567);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;
	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}

	return (CObjectPtr)sdsNew;
}

static CObjectPtr uHdfExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CHdfExtStruct b;
	
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
	
		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
		
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.CutMin},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.CutMax},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.vscale},	


		{"DOUB",4021,universal_TypeDouble,(void *)&b.sd.range.xminData},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.sd.range.yminData},	

		{"DOUB",4023,universal_TypeDouble,(void *)&b.sd.range.xmaxData},	
		{"DOUB",4024,universal_TypeDouble,(void *)&b.sd.range.ymaxData},	

		{"DOUB",4025,universal_TypeLong,(void *)&b.sd.range.ixmax},	
		{"DOUB",4026,universal_TypeLong,(void *)&b.sd.range.iymax},	

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},

		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files2},	

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CHdfExtPtr bp=(CHdfExtPtr)o;
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
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CHdfExtCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    	if(uPref.Files2){
		    	     RenderAddPaint((CSds2dExtPtr)bp,uPref.Files2);
		    	     uPref.Files2=NULL;
		    	}
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
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CHdfExtLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}

CHdfExtPtr CHdfExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	CHdfExtPtr s;
	struct Block b;
	char buff[256];
	static int Count;
	int type;

	if(!scene)return (CHdfExtPtr)NULL;

    if(flag){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CHdfExtPtr)NULL;
	}	

	s=(CHdfExtPtr)cMalloc(sizeof(CHdfExt),8566);
	if(!s)return (CHdfExtPtr)NULL;
	zerol((char *)s,sizeof(CHdfExt));

	sprintf(buff,"CHdfExt_%03d",Count++);	
		
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;
    
    if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
	
	CObjectCreate(scene,(CObjectPtr)s);

	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);

	s->type = G_EXTRUDE_HDF;
		
	s->duplicateObject=CHdfExtDuplicate;	
	s->message=CHdfExtMessage;	
	s->killObject=CHdfExtkill;
	s->ReadWrite=uHdfExtWrite;				
	
	s->Files=Files;
	s->vscale=0.0;
	s->CutMin=0.0;
	s->CutMax=0.0;
	
	
	s->BlockNumber=scene->BlockCount-1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	
    if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CHdfExtPtr)NULL;
		CHdfExtLoad(s,0L);
	}
	
	return s;
ErrorOut:
	return (CHdfExtPtr)NULL;
}
int CHdfExtMessage(CObject *b,struct ObjectMessage *Message)
{
	CHdfExtPtr s=(CHdfExtPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CHdfExtLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  Hdf Raster Extrude\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    if(s->Files2){
	    		sprintf(WarningBuff,"  Hdf Raster Extrude Paint\n%c",0);
	    		WarningBatch(WarningBuff);
		    	printFileInformation(s->Files2);
		    }
		    break;
		}
	    
	}
	return 0;
}
int CHdfExtLoad(CHdfExtPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct Block *b;
	double *data;
	double *dat;
	long ne;
	long i,j;
	long xdiff,ydiff;
	double vscale,xoff,yoff;
	double dx,dy;
	struct areadata ai;
	int ret;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;

	if(s->ItWas == itIs)return 0;

	b = &(scene->block[s->BlockNumber]);

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}
	
	if(s->ItWas == itIs)return 0;
	
	ret=1;
	
	data=NULL;

	zerol((char *)&ai,sizeof(struct areadata));
	
	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
	
	ai.xminArea=s->sd.range.xminData;
	ai.yminArea=s->sd.range.yminData;
	ai.zminArea=0;
	ai.xmaxArea=s->sd.range.xmaxData;
	ai.ymaxArea=s->sd.range.ymaxData;
	ai.zmaxArea=0;
	ai.xsize=s->sd.range.ixmax;
	ai.ysize=s->sd.range.iymax;
	ai.zsize=0;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CHdfExtLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	if(!data)goto ErrorOut;
	
	s->ai=ai;
	
	dx=(ai.xmaxArea-ai.xminArea)/(double)xsize;
	dy=(ai.ymaxArea-ai.yminArea)/(double)ysize;

	xdiff=xsize;
	ydiff=ysize;

	xoff=0.;
	yoff=0.;
	
	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;
	
	if(s->usePaint && s->PaintName[0]){
		ai.pioIndex=s->pioIndexPaint;
	    if(s->pioNamePaint[0]){
	        mstrncpy(ai.pioName,s->pioNamePaint,64);
	    }else{
	        mstrncpy(ai.pioName,"density",64);
	    }
	    ai.data=NULL;
		if(SendMessageByName(s->PaintName,MessageType_GET_AREA_DATA,&ai)){
			sprintf(WarningBuff,"CHdfExtLoad - file (%s)  Error Getting Area Data\n",s->PaintName);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
		if(!ai.data)goto ErrorOut;
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    for(i=0;i<xsize;++i){
				ne=i+(j)*xsize;
				b->vdata[ne]=ai.data[i+(ysize-1-j)*xsize];
				b->zdata[ne]=vscale*dat[i+(ysize-1-j)*xsize];
				b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
				b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
		    }
		}
		
		if(ai.data)cFree((char *)ai.data);
	}else{
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    for(i=0;i<xsize;++i){
				ne=i+(j)*xsize;
				b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
				b->zdata[ne]=(vscale*b->vdata[ne]);
				b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
				b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
		    }
		}

	}
	
	if(s->CutMin < s->CutMax){
	    long ElementCount;
	    long *Elements;
	    long nn,kp;
	    
	    Elements=b->Elements;
	    if(!Elements)goto ErrorOut;
	    ElementCount=b->ElementCount;
	    
	    ne=0;
	    nn=0;
		dat=(double *)data;
		for(ne=0;ne<ElementCount;++ne){
		    kp=Elements[ne*5+1];
		    j=kp/xsize;
		    i=kp-j*xsize;
			if(dat[i+(ysize-1-j)*xsize] >= s->CutMin &&
			   dat[i+(ysize-1-j)*xsize] <= s->CutMax){
			   Elements[nn++]=Elements[ne*5];
			   Elements[nn++]=Elements[ne*5+1];
			   Elements[nn++]=Elements[ne*5+2];
			   Elements[nn++]=Elements[ne*5+3];
			   Elements[nn++]=Elements[ne*5+4];
			}
		}
		b->ElementCount=nn/5;
	}

	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if(SetBlockRS(b))goto ErrorOut;
	    if(b->rdata && b->sdata){
			double dx,dy;
			long k;
			dx=0;
			if(xdiff > 1)dx=1.0/((double)(xdiff-1));

			dy=0;
			if(ydiff > 1)dy=1.0/((double)(ydiff-1));

			k=0;
			for(j=0;j<ydiff;++j){
			    for(i=0;i<xdiff;++i){
				    b->rdata[k]=(float)((i)*dx);
				    b->sdata[k++]=(float)((j)*dy);
				}
			}
	    }
	}

	elementrange(b);
	
	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);

	s->ItWas = itIs;
	
	ret=0;
	
ErrorOut:

	if(data)cFree((char *)data);

	return ret;
}
CObjectPtr CHdfExtDuplicate(CObject *b)
{
	CHdfExtPtr s=(CHdfExtPtr)b;
	CHdfExtPtr sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CHdfExtPtr)cMalloc(sizeof(CHdfExt),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}

int CHdfExtkill(CObject *o)
{
	CHdfExtPtr s=(CHdfExtPtr)o;
	if(!s)return 1;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
		if(s->PaintName[0])FileManagerCloseName(s->PaintName);
	}
	cFree((char *)s);
	return 0;
}

static CObjectPtr uSds2dRotWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CSds2dRotStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",5000,universal_TypeDoubleArrayPtr,(void *)&b.Levels,0L},				
		{"DOUB",5001,universal_TypeLong,(void *)&b.LevelCount},	

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	
		
		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"DOUB",4016,universal_TypeLong,(void *)&b.Axis},	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.Angle},	
		{"DOUB",4019,universal_TypeLong,(void *)&b.PassSmooth},	
		{"DOUB",4020,universal_TypeLong,(void *)&b.Angles},	


		{"DOUB",4021,universal_TypeDouble,(void *)&b.sd.range.xminData},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.sd.range.yminData},	

		{"DOUB",4023,universal_TypeDouble,(void *)&b.sd.range.xmaxData},	
		{"DOUB",4024,universal_TypeDouble,(void *)&b.sd.range.ymaxData},	

		{"DOUB",4025,universal_TypeLong,(void *)&b.sd.range.ixmax},	
		{"DOUB",4026,universal_TypeLong,(void *)&b.sd.range.iymax},	

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},

		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files2},	

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CSds2dRotPtr bp=(CSds2dRotPtr)o;
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
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CSds2dRotCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    	if(uPref.Files2){
		    	     RenderAddPaint((CSds2dExtPtr)bp,uPref.Files2);
		    	     uPref.Files2=NULL;
		    	}
		    }else{
		        goto  ErrorOut;
		    }
		}else{
			goto  ErrorOut;
		}	
		if(!bp)goto ErrorOut;
		bp->Levels=b.Levels;
		b.Levels=NULL;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CSds2dRotLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
	    Local[0].space=b.LevelCount;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}

CSds2dRotPtr CSds2dRotCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	struct Block b;
	CSds2dRotPtr s;
	char buff[256];
	static int Count;
	int type;

	if(!scene || !Files)return (CSds2dRotPtr)NULL;

	if(flag){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CSds2dRotPtr)NULL;
	}

	s=(CSds2dRotPtr)cMalloc(sizeof(CSds2dRot),8569);
	if(!s)return (CSds2dRotPtr)NULL;
	zerol((char *)s,sizeof(CSds2dRot));

	sprintf(buff,"Sds2dRot_%03d",Count++);	
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;

	if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);	

	CObjectCreate(scene,(CObjectPtr)s);

	s->type = G_ROTATE_SDS2D;
	s->Files=Files;
		
	s->duplicateObject=CSds2dRotDuplicate;
	s->message=CSds2dRotMessage;
	s->killObject=CSds2dRotkill;
	s->ReadWrite=uSds2dRotWrite;
	
	
	s->BlockNumber=scene->BlockCount-1;
	s->vscale=1.0;
	s->Angles=20;
	s->Axis=0;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->xmax = (int)s->sd.range.ixmax;
	s->ymax = (int)s->sd.range.iymax;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->Angle=180;
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CSds2dRotPtr)NULL;
	}
	return s;
ErrorOut:
	if(s)cFree((char *)s);
	return (CSds2dRotPtr)NULL; 
}
int CSds2dRotMessage(CObject *b,struct ObjectMessage *Message)
{
	CSds2dRotPtr s=(CSds2dRotPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CSds2dRotLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  Sds2D Rotate\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    if(s->Files2){
	    		sprintf(WarningBuff,"  Sds2D Rotate Paint\n%c",0);
	    		WarningBatch(WarningBuff);
		    	printFileInformation(s->Files2);
		    }
		    break;
		}
	    
	}
	return 0;
}
int CSds2dRotLoad(CSds2dRotPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct cdatal d;
	struct Block *b;
	double *data;
	double *dat;
	long ne,n,nn,ns;
	long i,j,nodes,cells;
	long xdiff,ydiff;
	double xoff,yoff;
	struct areadata ai;
	double dx,dy;
	long nee,ne1,ne2,n1,n2;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;
	
	if((s->LevelCount <= 0) || !s->Levels)return 0;
	if(s->ItWas == itIs)return 1;


	b = &(scene->block[s->BlockNumber]);

	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}
	
	if(s->ItWas == itIs)return 1;
	
	data=NULL;

	d.cList=NULL;

	zerol((char *)&ai,sizeof(struct areadata));

	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
		
	ai.xminArea=s->sd.range.xminData;
	ai.yminArea=s->sd.range.yminData;
	ai.zminArea=0;
	ai.xmaxArea=s->sd.range.xmaxData;
	ai.ymaxArea=s->sd.range.ymaxData;
	ai.zmaxArea=0;
	ai.xsize=s->sd.range.ixmax;
	ai.ysize=s->sd.range.iymax;;
	ai.zsize=0;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CSds2dRotLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	s->ai=ai;

	dx=(ai.xmaxArea-ai.xminArea)/(double)xsize;
	dy=(ai.ymaxArea-ai.yminArea)/(double)ysize;

	xdiff=xsize;
	ydiff=ysize;
	xoff=0.;
	yoff=0.;

	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;

	dat=(double *)data;
	for(j=0;j<ysize;++j){
	    for(i=0;i<xsize;++i){
			ne=i+(j)*xsize;
			b->Elements[ne]=s->Material;
			b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
			b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
			b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
	    }
	}

	zerol((char *)&d,sizeof(struct cdatal));

	LineCountours(b->xdata,b->ydata,b->vdata,b->Elements,
	             s->Levels,xdiff,ydiff,s->LevelCount,&d);

	nodes=0;
	cells=0;
	for(n=0;n<d.count;++n){
	    nodes += d.cList[n].nndata;
	    cells += d.cList[n].nndata-1;
	}


	if(blockSize(b,s->Material,nodes*s->Angles,cells*(s->Angles-1)))return 1;

	ne=0;
	nn=0;
	for(n=0;n<d.count;++n){
	    double *xx,*yy,*vv;
	    double r,sine,cosine,afac,v;

	    afac=(8.0/360.)*atan(1.0)*s->Angle/((double)(s->Angles)-1.0);

	    nodes = d.cList[n].nndata;
	    cells = d.cList[n].nndata-1;
	    xx=d.cList[n].xdata;
	    yy=d.cList[n].ydata;
	    vv=d.cList[n].vdata;
	    if(!xx || !yy || !vv)continue;
	    v=s->Levels[d.cList[n].nlevel];
	    ns=nn;
	    
	    if(s->usePaint && s->PaintName[0]){
		    struct linedata li;
			zerol((char *)&li,sizeof(struct linedata));
		    li.pioIndex=s->pioIndexPaint;
		    if(s->pioNamePaint[0]){
		        mstrncpy(li.pioName,s->pioNamePaint,64);
		    }else{
		        mstrncpy(li.pioName,"density",64);
		    }
			li.x=xx;
			li.y=yy;
			li.z=NULL;
			li.v=vv;
			li.PointLineCount=(int)nodes;
			li.type=LINEDATA_LIST_DATA;
			li.CurrentFrame=itIs;
			if(SendMessageByName(s->PaintName,MessageType_GET_LINE_DATA,&li)){
				sprintf(WarningBuff,"CSds2dRotLoad -  pioName (%s) Error Getting Paint Data\n",li.pioName);
				WarningBatch(WarningBuff);
				continue;
			}	    	    
	    }else{
	    	for(i=0;i<nodes;++i){
	    	    vv[i]=v;
	    	}
	    }
	    
	    for(j=0;j<s->PassSmooth;++j){
	        for(i=2;i<nodes;++i){
	            yy[i-1]=(.5*yy[i-1]+.25*yy[i-2]+.25*yy[i]);
	            xx[i-1]=(.5*xx[i-1]+.25*xx[i-2]+.25*xx[i]);
	        }
	    }

	    if(s->Axis == 0){
	        for(j=0;j<s->Angles;++j){
	            sine=sin((double)j*afac);
		    	cosine=cos((double)j*afac);
	            for(i=0;i<nodes;++i){
	                r=sqrt(yy[i]*yy[i]);
					b->xdata[nn]=xx[i];
					b->ydata[nn]=(r*cosine);
					b->zdata[nn]=(r*sine);
					b->vdata[nn]=vv[i];
					++nn;
	            }
	        }
	    }else {
	        for(j=0;j<s->Angles;++j){
	            sine=sin((double)j*afac);
		    	cosine=cos((double)j*afac);
	            for(i=0;i<nodes;++i){
	                r=sqrt(xx[i]*xx[i]);
					b->xdata[nn]=(r*cosine);
					b->ydata[nn]=yy[i];
					b->zdata[nn]=(r*sine);
					b->vdata[nn]=vv[i];
					++nn;
	            }
	        }
	    }
	    
		if(blockCells(b,s->Material,ns,&ne,nodes,s->Angles))goto ErrorOut;
		if(s->Angle == 360){
		    
		    nee=ne/5;
		    
		    ne2=nee-(nodes-1);
		    ne1=nee-(nodes-1)*(s->Angles-1);
		    
            for(i=0;i<nodes-1;++i){
                n1=5*(ne1+i);
                n2=5*(ne2+i);
                b->Elements[n1+1]=b->Elements[n2+4];
                b->Elements[n1+2]=b->Elements[n2+3];
            }
		    
		    
		}	    
	}
	
	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    double dx,dy;
	    if(SetBlockRS(b))goto ErrorOut;
	    nn=0;
	    for(n=0;n<d.count;++n){
 
	        nodes = d.cList[n].nndata;
  	        ns=nn;

	        dx=0;
	        if(nodes > 1)dx=1.0/((double)(nodes-1));

	        dy=0;
	        if(s->Angles > 1)dy=1.0/((double)(s->Angles-1));
 	        for(j=0;j<s->Angles;++j){
 	            for(i=0;i<nodes;++i){
					b->sdata[nn]=(float)((i)*dx);
					b->rdata[nn]=(float)((j)*dy);
					++nn;
	            }
	        }
	    }
	}


	elementrange(b);
	
	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);	

	s->ItWas = itIs;
	
ErrorOut:

	if(data)cFree((char *)data);

	if(d.cList){
	    for(n=0;n<d.count;++n){
	        if(d.cList[n].xdata){
	            cFree((char *)d.cList[n].xdata);
	            d.cList[n].xdata=NULL;
	        }
	        if(d.cList[n].ydata){
	            cFree((char *)d.cList[n].ydata);
	            d.cList[n].ydata=NULL;
	        }
	        if(d.cList[n].vdata){
	            cFree((char *)d.cList[n].vdata);
	            d.cList[n].vdata=NULL;
	        }
	    }
	    cFree((char *)d.cList);
	    d.cList=NULL;
	}
	
	return 0;
}
int CSds2dRotkill(CObject *o)
{
	CSds2dRotPtr s=(CSds2dRotPtr)o;
	if(!s)return 1;

	if(s->Levels)cFree((char *)s->Levels);
	s->Levels=NULL;
	s->LevelCount=0;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
		if(s->PaintName[0])FileManagerCloseName(s->PaintName);
	}

	cFree((char *)s);
	return 0;
}
CObjectPtr CSds2dRotDuplicate(CObject *b)
{
	CSds2dRotPtr s=(CSds2dRotPtr)b;
	CSds2dRotPtr sdsNew;
	long n;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CSds2dRotPtr)cMalloc(sizeof(CSds2dRot),8567);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;
	if(s->Levels && s->LevelCount){
	   sdsNew->Levels=(double *)cMalloc(sizeof(double)*s->LevelCount,8568);
	   if(!sdsNew->Levels){
	       cFree((char *)sdsNew);
	       return (CObjectPtr)NULL;
	   }
	   for(n=0;n<s->LevelCount;++n){
	       sdsNew->Levels[n]=s->Levels[n];
	   }
	}else{
	    sdsNew->Levels=NULL;
	}

	return (CObjectPtr)sdsNew;
}

static CObjectPtr uSds2dExtWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CSds2dExtStruct b;
	
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

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.CutMin},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.CutMax},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.vscale},	


		{"DOUB",4021,universal_TypeDouble,(void *)&b.sd.range.xminData},	
		{"DOUB",4022,universal_TypeDouble,(void *)&b.sd.range.yminData},	

		{"DOUB",4023,universal_TypeDouble,(void *)&b.sd.range.xmaxData},	
		{"DOUB",4024,universal_TypeDouble,(void *)&b.sd.range.ymaxData},	

		{"DOUB",4025,universal_TypeLong,(void *)&b.sd.range.ixmax},	
		{"DOUB",4026,universal_TypeLong,(void *)&b.sd.range.iymax},	

		{"REAL",4027,universal_TypeInt,(void *)&b.usePaint},

		{"FILE",4033,universal_TypeFilesPtr,(void *)&b.Files2},	

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CSds2dExtPtr bp=(CSds2dExtPtr)o;
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
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		if(uPref.Files){
			b.Files=FilesOpenList(uPref.Files);
		    uPref.Files=NULL;
			if(b.Files){
				bp=CSds2dExtCreate(l->scene,b.Files,0);
				if(!bp)goto  ErrorOut;
				bp->Files=b.Files;
		        b.Files=NULL;
		    	if(uPref.Files2){
		    	     RenderAddPaint((CSds2dExtPtr)bp,uPref.Files2);
		    	     uPref.Files2=NULL;
		    	}
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
		if(bp->Files){
    		struct FilePIOInfo *pio;
			pio=&bp->Files->pioData;
			pio->plane=bp->sd.plane;
		}
		CSds2dExtLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	return (CObjectPtr)&b;
ErrorOut:
	return NULL;
}
CSds2dExtPtr CSds2dExtCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	CSds2dExtPtr s;
	struct Block b;
	char buff[256];
	static int Count;
	int type;

	if(!scene)return (CSds2dExtPtr)NULL;
	
	if(flag){
		zerol((char *)&b,sizeof(struct Block));
		if(MergeBlock(scene,&b))return (CSds2dExtPtr)NULL;	
	}

	s=(CSds2dExtPtr)cMalloc(sizeof(CSds2dExt),8566);
	if(!s)return (CSds2dExtPtr)NULL;
	zerol((char *)s,sizeof(CSds2dExt));

	sprintf(buff,"CSds2dExt_%03d",Count++);	
		
    mstrncat(buff,".b",255);
    mstrncpy(s->BatchName,buff,255);
    if(FileManagerAddToList(Files,s->BatchName))goto ErrorOut;
    
    if(flag){
	    type = MessageType_DO_AREA_DIALOG;
		if(SendMessageByName(s->BatchName,MessageType_DO_DIALOG,&type)){
		    goto ErrorOut;
		}
	}
	if(SendMessageByName(s->BatchName,MessageType_GET_INFORMATION,&s->sd)){
		goto ErrorOut;
	}
	
	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);
	
	CObjectCreate(scene,(CObjectPtr)s);

	s->pioIndex=s->sd.pioIndex;
    mstrncpy(s->pioName,s->sd.pioName,64);

	s->type = G_EXTRUDE_SDS2D;
	s->duplicateObject=CSds2dExtDuplicate;
	
	s->message=CSds2dExtMessage;
	
	s->killObject=CSds2dExtkill;
	
	s->ReadWrite=uSds2dExtWrite;
			
	s->Files=Files;
	s->vscale=0.0;
	s->CutMin=0.0;
	s->CutMax=0.0;
		
	s->BlockNumber=scene->BlockCount-1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->pa.ZoneLevel=1;
	s->pa.DrawZonesColor=1;
	s->Material=getNextMaterial(scene);
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CSds2dExtPtr)NULL;
	    CSds2dExtLoad(s,0L);
	}
	
	return s;
ErrorOut:
	return (CSds2dExtPtr)NULL;
}
int CSds2dExtMessage(CObject *b,struct ObjectMessage *Message)
{
	CSds2dExtPtr s=(CSds2dExtPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
	     	CSds2dExtLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  Sds2D Extrude\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		    if(s->Files2){
	    		sprintf(WarningBuff,"  Sds2D Extrude Paint\n%c",0);
	    		WarningBatch(WarningBuff);
		    	printFileInformation(s->Files2);
		    }
		    break;
		}
	    
	}
	return 0;
}
int CSds2dExtLoad(CSds2dExtPtr s,long itIs)
{
	long xsize,ysize;
	struct Scene *scene;
	struct Block *b;
	double *data;
	double *dat;
	long ne;
	long i,j;
	long xdiff,ydiff;
	double vscale,xoff,yoff;
	double dx,dy;
	struct areadata ai;
	int ret;
	
	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;


	if(s->ItWas == itIs)return 0;

	b = &(scene->block[s->BlockNumber]);


	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}
	
	if(s->ItWas == itIs)return 0;
	
	ret=1;
	
	data=NULL;

	zerol((char *)&ai,sizeof(struct areadata));
	
	ai.pioIndex=s->pioIndex;
    mstrncpy(ai.pioName,s->pioName,64);
	
	ai.xminArea=s->sd.range.xminData;
	ai.yminArea=s->sd.range.yminData;
	ai.zminArea=0;
	ai.xmaxArea=s->sd.range.xmaxData;
	ai.ymaxArea=s->sd.range.ymaxData;
	ai.zmaxArea=0;
	ai.xsize=s->sd.range.ixmax;
	ai.ysize=s->sd.range.iymax;;
	ai.zsize=0;
	ai.CurrentFrame=itIs;
	ai.type = AREADATA_AREA_DATA;
	if(SendMessageByName(s->BatchName,MessageType_GET_AREA_DATA,&ai)){
		sprintf(WarningBuff,"CSds2dExtLoad - file (%s)  Error Getting Area Data\n",s->BatchName);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	s->xsize=xsize=ai.xsize;
	s->ysize=ysize=ai.ysize;
	s->length=ai.xsize*ai.ysize;
	
	data=ai.data;
	
	if(!data)goto ErrorOut;
	
	s->ai=ai;
	
	dx=(ai.xmaxArea-ai.xminArea)/(double)xsize;
	dy=(ai.ymaxArea-ai.yminArea)/(double)ysize;


	xdiff=xsize;
	ydiff=ysize;

	xoff=0.;
	yoff=0.;
	
	if(blockFix(b,s->Material,xdiff,ydiff,xoff,yoff))goto ErrorOut;
	
	
	if(s->usePaint && s->PaintName[0]){
		ai.pioIndex=s->pioIndexPaint;
	    if(s->pioNamePaint[0]){
	        mstrncpy(ai.pioName,s->pioNamePaint,64);
	    }else{
	        mstrncpy(ai.pioName,"density",64);
	    }
	    ai.data=NULL;
		if(SendMessageByName(s->PaintName,MessageType_GET_AREA_DATA,&ai)){
			sprintf(WarningBuff,"CSds2dExtLoad - file (%s)  Error Getting Area Data\n",s->PaintName);
			WarningBatch(WarningBuff);
			goto ErrorOut;
		}
		if(!ai.data)goto ErrorOut;
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    for(i=0;i<xsize;++i){
				ne=i+(j)*xsize;
				b->vdata[ne]=ai.data[i+(ysize-1-j)*xsize];
				b->zdata[ne]=vscale*dat[i+(ysize-1-j)*xsize];
				b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
				b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
		    }
		}
		
		if(ai.data)cFree((char *)ai.data);
	}else{
		vscale=s->vscale;
		dat=(double *)data;
		for(j=0;j<ysize;++j){
		    for(i=0;i<xsize;++i){
				ne=i+(j)*xsize;
				b->vdata[ne]=dat[i+(ysize-1-j)*xsize];
				b->zdata[ne]=(vscale*b->vdata[ne]);
				b->xdata[ne]=(double)i*dx+ai.xminArea+0.5*dx;
				b->ydata[ne]=(double)j*dy+ai.yminArea+0.5*dy;
		    }
		}

	}
	
	if(s->CutMin < s->CutMax){
	    long ElementCount;
	    long *Elements;
	    long nn,kp;
	    
	    Elements=b->Elements;
	    if(!Elements)goto ErrorOut;
	    ElementCount=b->ElementCount;
	    
	    ne=0;
	    nn=0;
		dat=(double *)data;
		for(ne=0;ne<ElementCount;++ne){
		    kp=Elements[ne*5+1];
		    j=kp/xsize;
		    i=kp-j*xsize;
			if(dat[i+(ysize-1-j)*xsize] >= s->CutMin &&
			   dat[i+(ysize-1-j)*xsize] <= s->CutMax){
			   Elements[nn++]=Elements[ne*5];
			   Elements[nn++]=Elements[ne*5+1];
			   Elements[nn++]=Elements[ne*5+2];
			   Elements[nn++]=Elements[ne*5+3];
			   Elements[nn++]=Elements[ne*5+4];
			}
		}
		b->ElementCount=nn/5;
	}

	if(scene->SmoothNormals){
	    SetNormalsBlock(b);
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if(SetBlockRS(b))goto ErrorOut;
	    if(b->rdata && b->sdata){
			double dx,dy;
			long k;
			dx=0;
			if(xdiff > 1)dx=1.0/((double)(xdiff-1));

			dy=0;
			if(ydiff > 1)dy=1.0/((double)(ydiff-1));

			k=0;
			for(j=0;j<ydiff;++j){
			    for(i=0;i<xdiff;++i){
				    b->rdata[k]=(float)((i)*dx);
				    b->sdata[k++]=(float)((j)*dy);
				}
			}
	    }
	}

	elementrange(b);

	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);
	
	s->ItWas = itIs;
	
	ret=0;
	
ErrorOut:

	if(data)cFree((char *)data);

	return ret;
}
CObjectPtr CSds2dExtDuplicate(CObject *b)
{
	CSds2dExtPtr s=(CSds2dExtPtr)b;
	CSds2dExtPtr sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CSds2dExtPtr)cMalloc(sizeof(CSds2dExt),8565);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}

int CSds2dExtkill(CObject *o)
{
	CSds2dExtPtr s=(CSds2dExtPtr)o;
	if(!s)return 1;

	if(s->scene && s->scene->Closing){
		if(s->BatchName[0])FileManagerCloseName(s->BatchName);
		if(s->PaintName[0])FileManagerCloseName(s->PaintName);
	}
	cFree((char *)s);
	return 0;
}
int doRenderKeys(char *line,IconPtr myIcon)
{
	struct CommandInfo cp;
	int ret;

	if(!line || !myIcon)return 1;

	ret=1;

	zerol((char *)&cp,sizeof(struct CommandInfo));

	if(getCommand(line,&cp))goto ErrorOut;
	
	for(cp.n=0;cp.n<cp.nword;++cp.n){
	    if(doSphere(myIcon,&cp))goto ErrorOut;
	}
	
	
	ret=0;
	
ErrorOut:	

	getCommand(NULL,&cp);
	
	return ret;
}
int doSphere(IconPtr myIcon,CommandPtr cp)
{
	static struct P origin;
	struct P origin2;
	struct Scene *scene;
	double radius;
	double value;
	char *command;
	renderPtr r;
	int nx,ny,mat;
	int k;

	if(!myIcon)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	if(!cp)return 1;
	
	origin2.x=0;
	origin2.y=0;
	origin2.z=0;
	
	scene=r->scene;
	
	command=stringCommand(cp);
	if(!command)return 1;

	if(!strcmp("sphere",command)){
	    CSpherePtr s;
	    radius=10;
	    nx=20;
	    ny=10;	  
	    mat=2;
	    k=0;
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        mat=(int)value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        origin2.x=value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        origin2.y=value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        origin2.z=value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        radius=value;
	    	++k;
	    }
	    if(k == 5){
	        sprintf(WarningBuff,"Sphere %d %g %g %g %g\n",mat,origin2.x,origin2.y,origin2.z,radius);
			if(scene->debug > 1)WarningBatch(WarningBuff);
	        s=CSphereCreate(scene,mat,nx,ny,radius,&origin2);
	    }else{
	        s=CSphereCreate(scene,mat,nx,ny,radius,&origin);
	    }
	    if(!s)return 1;
	    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)s)){
	        (*s->killObject)((CObjectPtr)s);
	        return 1;
	    }
	}else if(!strcmp("torus",command)){
	    CTorusPtr t;
	    double rbig;
	    double rsmall;

	    rbig=50;
	    rsmall=10;
	    nx=20;
	    ny=20;	  
	    mat=3;
	    t=CTorusCreate(scene,mat,nx,ny,rbig,rsmall,&origin);
	    if(!t)return 1;
	    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)t)){
	        (*t->killObject)((CObjectPtr)t);
	        return 1;
	    }
	}else if(!strcmp("disk",command)){
	    CDiskPtr d;
	    double rin;
	    double rout;

	    rout=50;
	    rin=10;
	    nx=20;
	    ny=20;	  
	    mat=4;
	    d=CDiskCreate(scene,mat,nx,ny,rin,rout,&origin);
	    if(!d)return 1;
	    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)d)){
	        (*d->killObject)((CObjectPtr)d);
	        return 1;
	    }
	}else if(!strcmp("cone",command)){
	    CConePtr c;
	    double rtop;
	    double rbottom;
	    double length;

	    rbottom=50;
	    rtop=10;
	    length=50;
	    nx=20;
	    ny=20;	  
	    mat=5;
	    c=CConeCreate(scene,mat,nx,ny,rtop,rbottom,length,&origin);
	    if(!c)return 1;
	    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)c)){
	        (*c->killObject)((CObjectPtr)c);
	        return 1;
	    }
	}else if(!strcmp("read",command)){
	    
	    ++(cp->n);
		command=stringCommand(cp);
		if(!command)return 1;
	    
	    sprintf(WarningBuff,"read %s\n",command);
		WarningBatch(WarningBuff);
		
		CObjectReadFile(command,myIcon);

    }else if(!strcmp("lamp",command)){
		Light lamp;
		int number;
		
		zerol((char *)&lamp,sizeof(lamp));
		
	    k=0;
		number=0;
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        number=(int)value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.light_bright=value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.type=(long)value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.light_pos[0]=value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.light_pos[1]=value;
	    	++k;
		}
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.light_pos[2]=value;
	    	++k;
	    }
		
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.red=value;
	    	++k;
	    }
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.green=value;
	    	++k;
		}
	    ++(cp->n);
	    if(!doubleCommand(&value,cp)){
	        lamp.blue=value;
	    	++k;
	    }
		
		CObjectSaveLamp(myIcon,number,&lamp);
	}
	
	return 0;
}
int CObjectSaveLamp(IconPtr myIcon,int number,Light *lamp)
{
	struct Scene *scene;
	struct t_light *Lights;
	renderPtr r;
	int ret=1;
	int n;
	
	if(!myIcon || !lamp)return 1;
	r=(renderPtr)myIcon->DWindow;
	if(!r)return 1;
	scene=r->scene;
	if(!scene)return 1;
	
	if(number < scene->nLights)
	{
		scene->Lights[number] = *lamp;
		ret = 0;
		goto ErrorOut;
	}
	
	Lights=(struct t_light *)cMalloc((number+1)*sizeof(struct t_light),8408);
	if(!Lights)goto ErrorOut;
	zerol((char *)Lights,(number+1)*sizeof(struct t_light));
	for(n=0;n<scene->nLights;++n){
		if(scene->Lights)Lights[n]=scene->Lights[n];
	}
	
	Lights[number]= *lamp;
	if(scene->Lights)cFree((char *)scene->Lights);
	scene->Lights=Lights;
	scene->nLights += 1;
	ret=0;
ErrorOut:
	if(ret == 0)
	{
		if(scene->Lights){
		    scene->lamp.origin.x=scene->Lights[0].light_pos[0];
		    scene->lamp.origin.y=scene->Lights[0].light_pos[1];
		    scene->lamp.origin.z=scene->Lights[0].light_pos[2];
			scene->Lights[0].type=0;
		}
		
	}
	return ret;
}
int CObjectReadFileList(IconPtr myIcon)
{
	struct FileList *Files;
	int n;
	
	if(!myIcon)return 1;		
	
	Files=FileDialog(NULL,"Object Files To Open",FileDialog_GetFiles,&myIcon->infoLoad);
	if(!Files)return 1;	
	
	for(n=0;n<Files->count;++n)
	{
		char *pathname=strpathname(Files->directory,Files->name[0]);
		if(!pathname)continue;	
		CObjectReadFile(pathname,myIcon);
	}
	FileListFree(Files);
	return 0;
}
int CObjectReadFile(char *command,IconPtr myIcon)
{
	char buff[256];
	FILE *in;
	int n;
	
	in=fopen(command,"r");
	if(!in){
		sprintf(WarningBuff,"Could Not Open %s to Read\n",command);
		WarningBatch(WarningBuff);
		return 1;
	}
	while(fgets(buff,256,in)){
		for(n=0;n<256;++n){
			if(buff[n] == '\n'){
				buff[n] = 0;
			}else if(buff[n] == '\r'){
				buff[n] = 0;
			}
		}
		doRenderKeys(buff,myIcon);
	}
	fclose(in);
	return 0;
}
CObjectListPtr duplicateObjectList(CObjectListPtr o)
{
	CObjectListPtr c;
	long n;

	if(!o)return (CObjectListPtr)NULL;
	c=(CObjectListPtr)cMalloc(sizeof(CObjectList),8542);
	if(!c)return c;
	zerol((char *)c,sizeof(CObjectList));
	for(n=0;n<o->oCount;++n){
	    CObjectPtr Current;
	    if((Current=o->oCurrent[n]) == NULL)continue;
	    Current=(*Current->duplicateObject)(Current);
	    if(ListAddObjectList(c,Current)){
	        if(Current)(*Current->killObject)(Current);
	        goto ErrorOut;
	    }
	}
	c->KeyFrameTime=o->KeyFrameTime;
	mstrncpy(c->KeyFrameTitle,o->KeyFrameTitle,255);
	c->scene=o->scene;
	c->sceneKeyFrame=o->sceneKeyFrame;
	return c;
ErrorOut:
	sprintf(WarningBuff,"duplicateObjectList failed\n");
	WarningBatch(WarningBuff);
	if(c){
	    freeObjectList(c,1);
	}
	return (CObjectListPtr)NULL;
}
int Draw(double *xx,double *yy,long nn,long nlevel,struct cdatal *d)
{
	double *xdata,*ydata,*vdata;
	long count,n;

	if(CheckDraw(d))return 1;

	count = d->count;
	xdata=(double *)cMalloc(nn*sizeof(double),8543);
	ydata=(double *)cMalloc(nn*sizeof(double),8544);
	vdata=(double *)cMalloc(nn*sizeof(double),8544);
	if(!xdata || !ydata || !vdata)goto ErrorOut;

	for(n=0;n<nn;++n){
	    xdata[n]=xx[n];
	    ydata[n]=yy[n];
	    vdata[n]=0;
	}

	d->cList[count].nndata=nn;
	d->cList[count].nlevel=nlevel;
	d->cList[count].xdata=xdata;
	d->cList[count].ydata=ydata;
	d->cList[count].vdata=vdata;
	++d->count;
	return 0;
ErrorOut:
	return 1;
}
int CheckDraw(struct cdatal *d)
{
        struct cdata *cList;
	long cmax;

	if(!d)return 1;

	if(d->count+1 < d->cmax)return 0;


	cmax = d->cmax+CONTOUR_INCREMENT;

	cList=NULL;
	if(d->cList){
	    cList=(struct cdata *)cRealloc((char *)d->cList,cmax*sizeof(struct cdata),8786);
	    if(!cList){
	        sprintf(WarningBuff,"CheckDraw out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)&cList[d->cmax],CONTOUR_INCREMENT*sizeof(struct cdata));
	}else{
	    cList=(struct cdata *)cMalloc(cmax*sizeof(struct cdata),8545);
	    if(!cList){
	        sprintf(WarningBuff,"CheckDraw out of Memory\n");
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    zerol((char *)cList,cmax*sizeof(struct cdata));
	}

	d->cList=cList;
	d->cmax=cmax;

	return 0;
ErrorOut:
	if(cList){
	    d->cList=cList;
	}
	return 1;
}
struct tree *tree(struct tree *p,double  *x,long nseg)
{
	double cond;

	if(!p){
	    p=(struct tree *)cMalloc((long)cg.treesize,8546);
	    if(!p){
	        sprintf(WarningBuff,"tree out of Memory cMalloc 0\n");
			WarningBatch(WarningBuff);
			return NULL;
	    }
	    zerol((char *)p,(long)cg.treesize);
	    p->value=*x;
	    p->left=NULL;
	    p->right=NULL;
	    p->Count=1;
	    p->MaxCount=5;
	    p->List=(long  *)cMalloc((long)(p->MaxCount)*sizeof(long),8547);
        if(!p->List){
	        sprintf(WarningBuff,"tree out of Memory cMalloc 1\n");
			WarningBatch(WarningBuff);
			return NULL;
        }
	    *(p->List)=nseg;
	    cg.pfind=p;
	} else if(fabs(cond=(*x-p->value)) < cg.tDiff){
	    if((p->Count + 1) > p->MaxCount){
	        p->MaxCount *= 2;
	        p->List=(long *)cRealloc((char *)p->List,(long)(p->MaxCount)*sizeof(long),8787);
	        if(!p->List){
		        sprintf(WarningBuff,"tree out of Memory cRealloc 2\n");
				WarningBatch(WarningBuff);
				return NULL;
	        }
	    }
	    *(p->List+(p->Count)++)=nseg;
	    cg.pfind=p;
	}
	else if (cond<0)
	    p->left=tree(p->left,x,nseg);
	else
	    p->right=tree(p->right,x,nseg);
	return(p);
}

struct tree *findtree(struct tree *p,double *x)
{
	double cond;

	if(!p){
	    cg.pfind=NULL;
	} else if(fabs(cond=(*x-p->value)) < cg.tDiff){
	    cg.pfind=p;
	} else if (cond<0){
	    p->left=findtree(p->left,x);
	} else{
	    p->right=findtree(p->right,x);
	}

	return(p);

}
int Search(double *xx,double *yy,long *nn)
{
	struct tree *xtree,*ytree;
	int x,y;
	long n,nx,ny;
	
	n = *nn;
	findtree(cg.xtree,xx+n);
	xtree=cg.pfind;
	findtree(cg.ytree,yy+n);
	ytree=cg.pfind;
	for(x=0;x<xtree->Count;++x){
	    nx=xtree->List[x];
	    if(cg.Used[nx])continue;
	    for(y=0;y<ytree->Count;++y){
	        ny=ytree->List[y];
	        if(cg.Used[ny])continue;
	        if(nx == ny)goto found;
	    }
	}
	return 0;
found:
	cg.Used[nx]=TRUE;
	if(fabs(xx[n]-cg.x1[nx]) < cg.tDiff &&
	   fabs(yy[n]-cg.y1[nx]) < cg.tDiff ){
	    xx[n+cg.inc]=cg.x2[nx];
	    yy[n+cg.inc]=cg.y2[nx];
	}else {
	    xx[n+cg.inc]=cg.x1[nx];
	    yy[n+cg.inc]=cg.y1[nx];
	}

	*nn=n+cg.inc;
/*	Search(xx,yy,nn); */
	return 1;
}
struct tree *freeTree(struct tree *p)
{

	if(p){
	    freeTree(p->left);
	    freeTree(p->right);
	    if(p->List)cFree((char *)p->List);
	    cFree((char *)p);
	}
	return (struct tree *)NULL;
}

int LineCountours(double *x,double *y,double *v,long *material,
                 double *levels,long nx,long ny,long nl,struct cdatal *d)
{
	double *fp[7];
	long *lp[3];
	long *np1,*np2,*np3;
	long n1,n2,n3;
	double s1,s2,s3;
	double *xx,*yy;
	long xl,yl,kmax,nseg,ask2;
	long Count,Offset,nxp,n,k,nn;
	double level,xmin,xmax,ymin,ymax,dum,dum2;
	
	
	cg.treesize=sizeof(struct tree);

	kmax=nx*ny*2;

	for(n=0;n<3;++n)lp[n]=NULL;
	for(n=0;n<7;++n)fp[n]=NULL;

	for(n=0;n<7;++n){
	    if(!(fp[n]=(double *)cMalloc(sizeof(double)*kmax,8548))){
	        sprintf(WarningBuff,"LineCountours  Out of Memory (%ld) nx %ld ny %ld\n",
	                sizeof(double)*16*kmax,nx,ny);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}
	
	for(n=0;n<3;++n){
	    if(!(lp[n]=(long *)cMalloc(sizeof(long)*kmax,8549))){
	        sprintf(WarningBuff,"LineCountours  Out of Memory (%ld) nx %ld ny %ld\n",
	                sizeof(long)*kmax,nx,ny);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}
	
	ask2=(long)sizeof(char)*kmax;

	np1=lp[0];
	np2=lp[1];
	np3=lp[2];
	xx=fp[0];
	yy=fp[1];
	cg.x1=fp[2];
	cg.x2=fp[3];
	cg.y1=fp[4];
	cg.y2=fp[5];
	cg.Used=(char *)fp[6];

	zerol(cg.Used,ask2);
	
	
/*		Gather into Triangles		*/
	
	Count=0;
	nxp=nx+1;
	Offset=nx;
	xmin = 1.0e30;
	xmax = -1.e030;
	ymin=1.e030;
	ymax = -1.0e30;
	for(yl=1;yl<ny;++yl){
	    for(xl=1;xl<nx;++xl){
		if(!material[Offset+xl])continue;

		np1[Count]=(xl+(Offset-nxp));
		dum=*(xl+x+(Offset-nxp));
		if(dum < xmin)xmin = dum;
		if(dum > xmax)xmax = dum;
		
		dum=*(xl+y+(Offset-nxp));
		if(dum < ymin)ymin = dum;
		if(dum > ymax)ymax = dum;
		
	
		np2[Count]=(xl+(Offset-nx));
		dum=*(xl+x+(Offset-nx));
		if(dum < xmin)xmin = dum;
		if(dum > xmax)xmax = dum;
		
		dum=*(xl+y+(Offset-nx));
		if(dum < ymin)ymin = dum;
		if(dum > ymax)ymax = dum;
		
		
		np3[Count]=(xl+(Offset));
		dum=*(xl+x+(Offset));
		if(dum < xmin)xmin = dum;
		if(dum > xmax)xmax = dum;
		
		dum=*(xl+y+(Offset));
		if(dum < ymin)ymin = dum;
		if(dum > ymax)ymax = dum;
		
		++Count;
		
		np1[Count]=(xl+(Offset-nxp));
		dum=*(xl+x+(Offset-nxp));
		if(dum < xmin)xmin = dum;
		if(dum > xmax)xmax = dum;
		
		dum=*(xl+y+(Offset-nxp));
		if(dum < ymin)ymin = dum;
		if(dum > ymax)ymax = dum;
		
		
		np2[Count]=(xl+(Offset));
		dum=*(xl+x+(Offset));
		if(dum < xmin)xmin = dum;
		if(dum > xmax)xmax = dum;
		
		dum=*(xl+y+(Offset));
		if(dum < ymin)ymin = dum;
		if(dum > ymax)ymax = dum;
		
		
		np3[Count]=(xl+(Offset-1));
		dum=*(xl+x+(Offset-1));
		if(dum < xmin)xmin = dum;
		if(dum > xmax)xmax = dum;
		
		dum=*(xl+y+(Offset-1));
		if(dum < ymin)ymin = dum;
		if(dum > ymax)ymax = dum;
		
		++Count;
	    }
	    Offset += nx;
	}
	
	cg.tDiff=(xmax-xmin);
	if((ymax-ymin) < cg.tDiff)cg.tDiff=(ymax-ymin);
	cg.tDiff *= 1e-6;
	
	for(n=0;n<nl;++n){
	    level=(1.0001379)*levels[n];
	    zerol(cg.Used,ask2);
	    
/*			Find All Intersections		*/
/*			Gather all Line Segments	*/

	   nseg=0;
	   cg.xtree=NULL;
	   cg.ytree=NULL;
	   for(k=0;k<Count;++k){
	        n1=np1[k];
	        n2=np2[k];
	        n3=np3[k];
	        dum=v[n2]-v[n1];
		dum2=v[n3]-v[n2];
		if(dum && (s1=(level-v[n1])/dum) > 0.  && s1 < 1.){
		        if(dum2){
		            s2=(level-v[n2])/dum2;
			    if(s2 > 0. && s2 < 1.){
		                cg.x1[nseg]=(x[n1]+s1*(x[n2]-x[n1]));
		                cg.y1[nseg]=(y[n1]+s1*(y[n2]-y[n1]));
		                cg.x2[nseg]=(x[n2]+s2*(x[n3]-x[n2]));
		                cg.y2[nseg]=(y[n2]+s2*(y[n3]-y[n2]));
		                cg.xtree=tree(cg.xtree,&cg.x1[nseg],nseg);
		                cg.xtree=tree(cg.xtree,&cg.x2[nseg],nseg);
		                cg.ytree=tree(cg.ytree,&cg.y1[nseg],nseg);
		                cg.ytree=tree(cg.ytree,&cg.y2[nseg],nseg);
		                ++nseg;
		            }
		        }
			dum=v[n3]-v[n1];
		        if(dum){
			    s3=(level-v[n1])/dum;
			    if(s3 > 0. && s3 < 1.){
		                cg.x1[nseg]=(x[n1]+s1*(x[n2]-x[n1]));
		                cg.y1[nseg]=(y[n1]+s1*(y[n2]-y[n1]));
		                cg.x2[nseg]=(x[n1]+s3*(x[n3]-x[n1]));
		                cg.y2[nseg]=(y[n1]+s3*(y[n3]-y[n1]));
		                cg.xtree=tree(cg.xtree,&cg.x1[nseg],nseg);
		                cg.xtree=tree(cg.xtree,&cg.x2[nseg],nseg);
		                cg.ytree=tree(cg.ytree,&cg.y1[nseg],nseg);
		                cg.ytree=tree(cg.ytree,&cg.y2[nseg],nseg);
		                ++nseg;
		            }
		        }
		}else if(dum2){
		    s2=(level-v[n2])/dum2;
		    if(s2 > 0. && s2 < 1.){
			dum=v[n3]-v[n1];
		        if(dum){
			    s3=(level-v[n1])/dum;
			    if(s3 > 0. && s3 < 1.){
		                cg.x1[nseg]=(x[n2]+s2*(x[n3]-x[n2]));
		                cg.y1[nseg]=(y[n2]+s2*(y[n3]-y[n2]));
		                cg.x2[nseg]=(x[n1]+s3*(x[n3]-x[n1]));
		                cg.y2[nseg]=(y[n1]+s3*(y[n3]-y[n1]));
		                cg.xtree=tree(cg.xtree,&cg.x1[nseg],nseg);
		                cg.xtree=tree(cg.xtree,&cg.x2[nseg],nseg);
		                cg.ytree=tree(cg.ytree,&cg.y1[nseg],nseg);
		                cg.ytree=tree(cg.ytree,&cg.y2[nseg],nseg);
		                ++nseg;
		            }
		        }
		    }
		}
	   }
	   if(nseg == 0)continue;
	
/*			Trace Each Line		*/
	    nn=0;
	    for(k=0;k<nseg;++k){
	        long limit;
	        
	        if(!cg.Used[k]){

	            cg.Used[k]=TRUE;
	            cg.inc=1;
	            nn=kmax/2;
	            xx[nn]=cg.x2[k];
	            yy[nn]=cg.y2[k];
	            nn=nn+1;
	            xx[nn]=cg.x1[k];
	            yy[nn]=cg.y1[k];
	            while(1)
	            {
	                if(!Search(xx,yy,&nn))break;
	            }
	            limit=nn+1;
	            nn=kmax/2;
	            cg.inc= -1;
	            xx[nn]=cg.x2[k];
	            yy[nn]=cg.y2[k];
	            while(1)
	            {
	                if(!Search(xx,yy,&nn))break;
	            }
	            
	            Draw(&xx[nn],&yy[nn],limit-nn,n,d);

	        }
	    }

	    if(cg.xtree)freeTree(cg.xtree);
	    if(cg.ytree)freeTree(cg.ytree);
		
	}
ErrorOut:
	for(n=0;n<3;++n)if(lp[n])cFree((char *)lp[n]);
	for(n=0;n<7;++n)if(fp[n])cFree((char *)fp[n]);
	return 0;
}
int freeObjectList(CObjectListPtr o,int flag)
{
	long n;

	if(!o)return 1;

	if(o->oCurrent){
	    for(n=0;n<o->oCount;++n){
	        CObjectPtr Current;
	        if((Current=o->oCurrent[n]) == NULL)continue;
	        (*Current->killObject)(Current);
	    }
	    cFree((char *)o->oCurrent);
	    o->oCurrent=NULL;
	}
	if(o->oOld)cFree((char *)o->oOld);
	o->oOld=NULL;
	o->oCount=0;
	o->oMaximun=0;
	if(flag)cFree((char *)o);
	return 0;
}
int ObjectListAddFiles(struct FileInfo2 *files,struct Scene *scene)
{
	static uDialogGroup Items[] =

        {
      /*  0 */      {"Ok", {10,210,80,20},uDialogButton,uDialogNone},
      /*  1 */      {"Cancel", {115,210,80,20},uDialogButton,uDialogNone},
      /*  2 */      {"rotate", {15,25,105,20},uDialogButtonRadio,uDialogGetLong},
      /*  3 */      {"extrude", {15,55,105,20},uDialogButtonRadio,uDialogGetLong},
      /*  4 */      {"rays",{15,85,105,20},uDialogButtonRadio,uDialogGetLong},
      /*  5 */      {"level surfaces",{15,115,105,20},uDialogButtonRadio,uDialogGetLong},
      /*  6 */      {"tracers",{15,145,105,20},uDialogButtonRadio,uDialogGetLong},      
      /*  7 */      {"streamlines",{15,175,105,20},uDialogButtonRadio,uDialogGetLong}, 
        };
        
    long ItemCount = sizeof(Items)/sizeof(uDialogGroup);
    
 	static uRect  Rect1   = {100,50,220,250};
 	uDialogList *uList;
	uDialogReturn *uR;
 	
	static int extrude=1;
	char *streamName;
	CObjectPtr o;
	int reti;
	int ret;
	int iobject;

	if(!files || !scene)return 1;

	reti=1;
	
	uR=NULL;
	
	uList=NULL;
	
	iobject=0;

	if(files->fileClass == SDS2D_FILES){
	    ;
	}else if(files->fileClass == HDF_FILES){
	    ;
	}else if(files->fileClass == PIO1D_FILES){
	    ;
	}else if(files->fileClass == PIO2D_FILES){
	    ;
	}else if(files->fileClass == PIO3D_FILES){
	    ;
	}else if(files->fileClass == PIO1D_BLK_FILES){
	    ;
	}else if(files->fileClass == PIO2D_BLK_FILES){
	    ;
	}else if(files->fileClass == PIO3D_BLK_FILES){
	    ;
	}else if(files->fileClass == SDS3D_FILES){
		iobject=1;
	    goto Add;
	}else if(files->fileClass == VRML_FILES){
		iobject=1;
	    goto Add;
	}else if(files->fileClass == SHOW_FILES){
		iobject=1;
	    goto Add;
	}else if(files->fileClass == STL_FILES){
		iobject=1;
	    goto Add;
	}else{
	    return 1;
	}
	
	
	uList=uDialogOpen("Objects",&Rect1,ItemCount);
	
	if(!uList)goto OutOfHere;
	
	if(uDialogSetList(uList,Items,ItemCount))goto OutOfHere;
		
	ret=uDialogSet(uList,0L,
	              
                uDialogSetItem,2L,
                uDialogSetLogical,(extrude == 0),
				
                uDialogSetItem,3L,
                uDialogSetLogical,(extrude == 1),
	              
                uDialogSetItem,4L,
                uDialogSetLogical,(extrude == 2),
	              
                uDialogSetItem,5L,
                uDialogSetLogical,(extrude == 3),
	              
                uDialogSetItem,6L,
                uDialogSetLogical,(extrude == 4),
	              
                uDialogSetItem,7L,
                uDialogSetLogical,(extrude == 5),
	            uDialogSetActive,(files->fileClass == SDS3D_FILES),
	              
                uDialogSetDone,uDialogSetDone
	    );
	if(ret)goto OutOfHere;

    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	if(uDialog(uList))goto OutOfHere;
    
	uR=uDialogGetReturn(uList);

    if(!uR)goto OutOfHere;
    
    if(uR[3].lreturn){
        extrude=1;
    }else if(uR[4].lreturn){
        extrude=2;
    }else if(uR[5].lreturn){
        extrude=3;
    }else if(uR[6].lreturn){
        extrude=4;
    }else if(uR[7].lreturn){
        extrude=5;
    }else{
        extrude=0;
    }

    if(uList)uDialogClose(uList);
    uList=NULL;
    if(uR)uDialogFreeReturn(uR);
    uR=NULL;

	
Add:
	o=NULL;
	if(files->fileClass == SDS2D_FILES){
	    if(extrude == 0){
	        o=(CObjectPtr)CSds2dRotCreate(scene,files,1);
	    }else if(extrude == 1){
	        o=(CObjectPtr)CSds2dExtCreate(scene,files,1);
	    }
	}else if(files->fileClass == HDF_FILES){
	    if(extrude == 0){
	        o=(CObjectPtr)CHdfRotCreate(scene,files,1);
	    }else if(extrude == 1){
	        o=(CObjectPtr)CHdfExtCreate(scene,files,1);
	    }
	}else if(files->fileClass == PIO1D_FILES || files->fileClass == PIO2D_FILES ||
	         files->fileClass == PIO2D_BLK_FILES){
	    if(extrude == 0){
	        o=(CObjectPtr)CPio2dRotCreate(scene,files,1);
	    }else if(extrude == 1){
	        o=(CObjectPtr)CPio2dExtCreate(scene,files,1);
	    }else if(extrude == 2){
	        o=(CObjectPtr)CPio2dRayCreate(scene,files);
	    }else if(extrude == 4){
	        o=(CObjectPtr)CPio2dTracerCreate(scene,files,2);
	    }
	}else if(files->fileClass == PIO3D_FILES || files->fileClass == PIO3D_BLK_FILES){
	    if(extrude == 0){
	        /* o=(CObjectPtr)CPio2dRotCreate(scene,files); */
	        goto OutOfHere;
	    }else if(extrude == 1){
	       o=(CObjectPtr)CPio3dExtCreate(scene,files,1);
	    }else if(extrude == 2){
	        o=(CObjectPtr)CPio2dRayCreate(scene,files);
	    }else if(extrude == 3){
	        o=(CObjectPtr)CPio3dLevelCreate(scene,files,1);
	    }else if(extrude == 4){
	        o=(CObjectPtr)CPio2dTracerCreate(scene,files,3);
	    }else if(extrude == 5){
	         streamName=StreamLine3D(scene);
	         if(!streamName)goto OutOfHere;
	         o=(CObjectPtr)CPio3dStreamLinesCreate(scene,files,streamName);
	    }
	}else if(files->fileClass == SDS3D_FILES){
	    o=(CObjectPtr)CSds3dCreate(scene,files,1);
	}else if(files->fileClass == VRML_FILES){
	    o=(CObjectPtr)CVrmlCreate(scene,files);
	}else if(files->fileClass == SHOW_FILES){
	    o=(CObjectPtr)CShowCreate(scene,files,1);
	}else if(files->fileClass == STL_FILES){
	    o=(CObjectPtr)CStlCreate(scene,files,1);
	}

	if(!o)goto OutOfHere;
	if(ListAddObjectList(scene->ObjectList,o)){
	    (*o->killObject)(o);
	    goto OutOfHere;;
	}

	reti=0;
OutOfHere:

    if(uList)uDialogClose(uList);
    if(uR)uDialogFreeReturn(uR);
    
    if(!reti && iobject){
    	return 7;
    }

    return reti;
}
int blockCells(struct Block *b,long mat,long ns,long *nee,long xsize,long ysize)
{
	long *Elements,ne;
	long i,j,n;
	long NeeMax,Nnmax;
	
	if(!b || !nee)return 1;
	ne = *nee;
	Elements=b->Elements;
	NeeMax=ne+(ysize-1)*(xsize-1)*5;
	if((NeeMax) > b->ElementCount*5){
	    sprintf(WarningBuff,"blockCells Error cell pointer %ld exceeds max %ld\n",
	            NeeMax,b->ElementCount*5);
		WarningBatch(WarningBuff);
	    return 1;
	}
	Nnmax=ns+xsize-2+1+(ysize-2+1)*xsize;
	if((Nnmax) > b->NodeCount){
	    sprintf(WarningBuff,"blockCells Error node %ld exceeds max %ld\n",
	            Nnmax,b->NodeCount);
		WarningBatch(WarningBuff);
	    return 1;
	}
	n=ne;
	for(j=0;j<ysize-1;++j){
	    for(i=0;i<xsize-1;++i){
			Elements[n++]=mat;
			Elements[n++]=ns+i+j*xsize;
			Elements[n++]=ns+i+1+j*xsize;
			Elements[n++]=ns+i+1+(j+1)*xsize;
			Elements[n++]=ns+i+(j+1)*xsize;
	    }
	}
	*nee = n;
	return 0;
}

CObjectPtr CWorldDuplicate(CObject *b)
{
	CWorldPtr w=(CWorldPtr)b;
	CWorldPtr worldNew;

	if(!w)return (CObjectPtr)NULL;

	worldNew=(CWorldPtr)cMalloc(sizeof(CWorld),8581);
	if(!worldNew)return (CObjectPtr)NULL;

	*worldNew = *w;

	return (CObjectPtr)worldNew;
}
static CObjectPtr uWorldWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CWorldStruct b;
	
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
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CWorldPtr bp=(CWorldPtr)o;
	void *op;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l || !l->scene)return NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		
		zerol((char *)&b,sizeof(b));
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		
		{
		    CObjectPtr Current;
		    int n;
		    for(n=0;n<l->oCount;++n){
				if((Current=l->oCurrent[n]) == NULL)continue;
				if(Current->type == G_WORLD){
				    op=Current;
				    bp=(CWorldPtr)op;
				    break;
				}
		    }
		
		}
		
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		l->scene->World=bp->Local;
		rotate(&l->scene->World);
		return (CObjectPtr)NULL;			
		
	}else{
	    b = *bp;
		b.Local=l->scene->World;
		findAngles(&b.Local);

		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	op=&b;
	
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
CWorldPtr CWorldCreate(struct Scene *scene)
{
	CWorldPtr w;

	if(!scene)return (CWorldPtr)NULL;
	
	w=(CWorldPtr)cMalloc(sizeof(CWorld),8582);
	if(!w)return (CWorldPtr)NULL;
	zerol((char *)w,sizeof(CWorld));

	CObjectCreate(scene,(CObjectPtr)w);

	w->type = G_WORLD;
	w->Local=scene->World;
	findAngles(&w->Local);
	w->duplicateObject=CWorldDuplicate;
	w->ReadWrite=uWorldWrite;
	return w;
}
int checkWorld(struct Scene *scene,CObjectListPtr o,int flag)
{
	long n;

	if(!scene || !o)return 1;
	
	if(flag == 0){
	    CObjectPtr Current;
	    CWorldPtr w;
	    for(n=0;n<o->oCount;++n){
			if((Current=o->oCurrent[n]) == NULL)continue;
			if(Current->type == G_WORLD){
			    Current->Local=scene->World;
			    findAngles(&Current->Local);
			    return 0;
			}
	    }
	    w=CWorldCreate(scene);
	    if(!w)return 1;
	    if(ListAddObjectList(scene->ObjectList,(CObjectPtr)w)){
	        (*w->killObject)((CObjectPtr)w);
	        return 1;
	    }

	}else if(flag == 1){
	    for(n=0;n<o->oCount;++n){
	        CObjectPtr Current;
			if((Current=o->oCurrent[n]) == NULL)continue;
			if(Current->type == G_WORLD){
			    scene->World=Current->Local;
			    rotate(&scene->World);
			    return 0;
			}
	    }
	}
	return 0;
}
int CConeDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CConePtr c=(CConePtr)o;
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
	long *ndata,nel;
	long k;
        double rtop;
        double rbottom;
        double length;
	Vec Lampl,Eyel;

	int i,j;

	scene=c->scene;

	if(flag == 1){
	    if(scene->material[c->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[c->Material].opacity == 1.0)return 0;
	}

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
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
	    rtop=c->rtop;
	    rbottom=c->rbottom;
	    length=c->length;
	    pi=4*atan(1.0);
	    dumy=1.0/((double)c->ny-1);
	    dumx=1.0/((double)c->nx-1);
	    k=0;
	    for(j=0;j<c->ny;++j){
	        for(i=0;i<c->nx;++i){
	           dx=c->origin.x+(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(1.-i*dumx));
	           dy=c->origin.y+(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(1.-i*dumx));
	           dz=c->origin.z+j*dumy*length;
	           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
	           ++k;
	        }
	    }
	    nel=0;
	    for(j=0;j<c->ny-1;++j){
	        for(i=0;i<c->nx-1;++i){
		    ndata[nel++]=c->Material;
		    ndata[nel++]=i+j*c->nx;
		    ndata[nel++]=i+1+j*c->nx;
		    ndata[nel++]=i+1+(j+1)*c->nx;
		    ndata[nel++]=i+(j+1)*c->nx;
	        }
	    }
	    nel /= 5;

	    vdata=zdatal;
	    vdata=NULL;

	    if(gdata){
	        double den,th;

		VecCopy(scene->Eyel,Eyel);
		VecCopy(scene->Lampl,Lampl);

	    	den=rtop-rbottom;
	    	if(den == 0.0){
	    	    dz=0.0;
	    	}else{
	    	    th=atan2(1.0,length/den);
	    	    dz=sin(th);
	    	}	               
	    	k=0;
	        for(j=0;j<c->ny;++j){
		    for(i=0;i<c->nx;++i){
		        Vec P,N,D,L;
		        double len;
		        

	               dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(1.-i*dumx));
	               dy=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(1.-i*dumx));
		        
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
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	    }else if(nvdata){
	        double den,th;

		VecCopy(scene->Eyel,Eyel);
		VecCopy(scene->Lampl,Lampl);

	    	den=rtop-rbottom;
	    	if(den == 0.0){
	    	    dz=0.0;
	    	}else{
	    	    th=atan2(1.0,length/den);
	    	    dz=sin(th);
	    	}	               
	    	k=0;
	        for(j=0;j<c->ny;++j){
		    for(i=0;i<c->nx;++i){
		        

	               dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(1.-i*dumx));
	               dy=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(1.-i*dumx));
		        
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
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	    }

	*CellCount += nel;
	c->LastCell = *CellCount;

	return 0;
}
int CObjectConeMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CConePtr c=(CConePtr)o;
	long NodeCount;
	long CellCount;

	NodeCount=c->nx*c->ny;
	CellCount=(c->nx-1)*(c->ny-1)*5;
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(c->scene->SmoothNormals)*MostNormals=1;
	return 0;
}
CObjectPtr CConeDuplicate(CObject *b)
{
	CConePtr c=(CConePtr)b;
	CConePtr coneNew;

	if(!c)return (CObjectPtr)NULL;

	coneNew=(CConePtr)cMalloc(sizeof(CCone),8583);
	if(!coneNew)return (CObjectPtr)NULL;

	*coneNew = *c;
	return (CObjectPtr)coneNew;
}
int CObjectConeCount(CObject *o,long *nodes,long *cells)
{
	CConePtr c=(CConePtr)o;

	*nodes += c->nx*c->ny;
	*cells  += (c->nx-1)*(c->ny-1);
	return 0;
}
int CConeMessage(CObject *b,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	CConePtr s=(CConePtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    struct P p;
	    double r;
	    static double x[8]={-1.,1.,1.,-1., -1.,1.,1.,-1.};
	    static double y[8]={-1.,-1.,1.,1., -1.,-1.,1.,1.};
	    static double z[8]={0.,0.,0.,0.,1.,1.,1.,1.};
	    int n;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);

	    r=s->rtop;
	    if(s->rbottom > r)r=s->rbottom;
	    for(n=0;n<8;++n){
	        p.x=s->origin.x+r*x[n];
	        p.y=s->origin.y+r*y[n];
	        p.z=s->origin.z+s->length*z[n];
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
static CObjectPtr uConeWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CConeStruct b;
	
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
	
		{"REAL",4015,universal_TypeLong,(void *)&b.nx},
		{"REAL",4016,universal_TypeLong,(void *)&b.ny},
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.origin.x},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.origin.y},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.origin.z},	
		{"DOUB",4030,universal_TypeDouble,(void *)&b.rtop},	
		{"DOUB",4031,universal_TypeDouble,(void *)&b.rbottom},	
		{"DOUB",4032,universal_TypeDouble,(void *)&b.length},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CConePtr bp=(CConePtr)o;
	void *op;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=CConeCreate(l->scene,(int)b.Material,(int)b.ny,(int)b.ny,
                         b.rtop,b.rbottom,b.length,&b.origin);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		rotate3d(&(bp->Local));	
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	op=&b;
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
CConePtr CConeCreate(struct Scene *scene,int mat,int nx,int ny,
                         double rtop,double rbottom,double length,
                         struct P *origin)
{
	CConePtr cone;

	if(!scene)return (CConePtr)NULL;
	cone=(CConePtr)cMalloc(sizeof(CCone),8584);
	if(!cone)return (CConePtr)NULL;
	zerol((char *)cone,sizeof(CCone));

	CObjectCreate(scene,(CObjectPtr)cone);
	cone->Material=mat;
	cone->nx=nx;
	cone->ny=ny;
	cone->rtop=rtop;
	cone->rbottom=rbottom;
	cone->length=length;
	cone->origin= *origin;
	cone->pGetMost=CObjectConeMost;
	cone->drawObject=CConeDraw;
	cone->duplicateObject=CConeDuplicate;
	cone->countObject=CObjectConeCount;
	cone->message=CConeMessage;
	cone->ReadWrite=uConeWrite;				
	cone->type=G_CONE;
	return cone;
}
int CObjectdrawDisk(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CDiskPtr d=(CDiskPtr)o;
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
	long *ndata,nel;
	long MostNormals,k;
        double rin;
        double rout;

	/*
	Vec Lampl,Eyel;
	*/
	int i,j;


	scene=d->scene;
	if(flag == 1){
	    if(scene->material[d->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[d->Material].opacity == 1.0)return 0;
	}

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	gdata=scene->gdata;
	nvdata=scene->nvdata;
	ndata=scene->cdata;
	rdata=scene->rdata;
	sdata=scene->sdata;
/*
	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);
*/
	    d->FirstCell = *CellCount;

	    MostNormals=0;
	    /* Must Rotate local system to un-rotated local */
	    BuildTransformMatrix(&BlockToWorld,&d->Local,&scene->Global);
	    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

	    v1x=BlockToScreen.x[0][0]*d->Local.Scale_x;
	    v1y=BlockToScreen.x[0][1]*d->Local.Scale_y;
	    v1z=BlockToScreen.x[0][2]*d->Local.Scale_z;
	    v2x=BlockToScreen.x[1][0]*d->Local.Scale_x;
	    v2y=BlockToScreen.x[1][1]*d->Local.Scale_y;
	    v2z=BlockToScreen.x[1][2]*d->Local.Scale_z;
	    v3x=BlockToScreen.x[2][0]*d->Local.Scale_x;
	    v3y=BlockToScreen.x[2][1]*d->Local.Scale_y;
	    v3z=BlockToScreen.x[2][2]*d->Local.Scale_z;
	    xc=BlockToScreen.x[0][3];
	    yc=BlockToScreen.x[1][3];
	    zc=BlockToScreen.x[2][3];
	    k=0;
	    rin=d->rin;
	    rout=d->rout;
	    pi=4*atan(1.0);
	    dumy=1.0/((double)d->ny-1);
	    dumx=1.0/((double)d->nx-1);
	    for(j=0;j<d->ny;++j){
	        for(i=0;i<d->nx;++i){
	           dx=d->origin.x+(rin+j*dumy*(rout-rin))*cos(2*pi*(1.-i*dumx));
	           dy=d->origin.y+(rin+j*dumy*(rout-rin))*sin(2*pi*(1.-i*dumx));
	           dz=d->origin.z;
	           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
	           ++k;
	        }
	    }
	    nel=0;
	    for(j=0;j<d->ny-1;++j){
	        for(i=0;i<d->nx-1;++i){
		    ndata[nel++]=d->Material;
		    ndata[nel++]=i+j*d->nx;
		    ndata[nel++]=i+1+j*d->nx;
		    ndata[nel++]=i+1+(j+1)*d->nx;
		    ndata[nel++]=i+(j+1)*d->nx;
	        }
	    }
	    nel /= 5;

	    vdata=zdatal;
	    if(nvdata){
	    	dz=1.0;
	        dx=0.0;
	        dy=0.0;
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
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	    }else if(MostNormals){
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	    }

	*CellCount += nel;

	d->LastCell = *CellCount;

	return 0;
}
int CObjectDiskMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CDiskPtr d=(CDiskPtr)o;
	long NodeCount;
	long CellCount;

	NodeCount=d->nx*d->ny;
	CellCount=(d->nx-1)*(d->ny-1)*5;
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	// MostNormals = MostNormals;
	return 0;
}
CObjectPtr CDiskDuplicate(CObject *b)
{
	CDiskPtr d=(CDiskPtr)b;
	CDiskPtr diskNew;

	if(!d)return (CObjectPtr)NULL;

	diskNew=(CDiskPtr)cMalloc(sizeof(CDisk),8585);
	if(!diskNew)return (CObjectPtr)NULL;

	*diskNew = *d;
	return (CObjectPtr)diskNew;
}
int CObjectDiskCount(CObject *o,long *nodes,long *cells)
{
	CDiskPtr d=(CDiskPtr)o;

	*nodes += d->nx*d->ny;
	*cells += (d->nx-1)*(d->ny-1);
	return 0;
}
static CObjectPtr uDiskWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CDiskStruct b;
	
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
	
		{"REAL",4015,universal_TypeLong,(void *)&b.nx},
		{"REAL",4016,universal_TypeLong,(void *)&b.ny},
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.origin.x},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.origin.y},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.origin.z},	
		{"DOUB",4030,universal_TypeDouble,(void *)&b.rin},	
		{"DOUB",4031,universal_TypeDouble,(void *)&b.rout},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CDiskPtr bp=(CDiskPtr)o;
	void *op;
	char head[5];
	long length;
	long tag;
	
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=CDiskCreate(l->scene,(int)b.Material,(int)b.ny,(int)b.ny,
                         b.rin,b.rout,&b.origin);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		rotate3d(&(bp->Local));	
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	op=&b;
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
int CDiskMessage(CObject *b,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	CDiskPtr s=(CDiskPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    struct P p;
	    double r;
	    static double x[4]={-1.,1.,1.,-1.};
	    static double y[4]={-1.,-1.,1.,1.};
	    int n;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);

	    r=s->rout;
	    for(n=0;n<4;++n){
	        p.x=s->origin.x+r*x[n];
	        p.y=s->origin.y+r*y[n];
	        p.z=s->origin.z;
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
CDiskPtr CDiskCreate(struct Scene *scene,int mat,int nx,int ny,
                         double rin,double rout,struct P *origin)
{
	CDiskPtr disk;

	if(!scene)return (CDiskPtr)NULL;
	disk=(CDiskPtr)cMalloc(sizeof(CDisk),8586);
	if(!disk)return (CDiskPtr)NULL;
	zerol((char *)disk,sizeof(CDisk));

	CObjectCreate(scene,(CObjectPtr)disk);
	disk->Material=mat;
	disk->nx=nx;
	disk->ny=ny;
	disk->rin=rin;
	disk->rout=rout;
	disk->origin= *origin;
	disk->pGetMost=CObjectDiskMost;
	disk->drawObject=CObjectdrawDisk;
	disk->duplicateObject=CDiskDuplicate;
	disk->countObject=CObjectDiskCount;
	disk->message=CDiskMessage;
	disk->ReadWrite=uDiskWrite;
	disk->type=G_DISK;	
	return disk;
}
int CTorusDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CTorusPtr t=(CTorusPtr)o;
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
	long *ndata,nel;
	long k;
        double rbig;
        double rsmall;

	Vec Lampl,Eyel;

	int i,j;

	scene=t->scene;
	if(flag == 1){
	    if(scene->material[t->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[t->Material].opacity == 1.0)return 0;
	}

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	gdata=scene->gdata;
	nvdata=scene->nvdata;
	ndata=scene->cdata;
	rdata=scene->rdata;
	sdata=scene->sdata;
	    t->FirstCell = *CellCount;

	    /* Must Rotate local system to un-rotated local */

	    BuildTransformMatrix(&BlockToWorld,&t->Local,&scene->Global);
	    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

	    v1x=BlockToScreen.x[0][0]*t->Local.Scale_x;
	    v1y=BlockToScreen.x[0][1]*t->Local.Scale_y;
	    v1z=BlockToScreen.x[0][2]*t->Local.Scale_z;
	    v2x=BlockToScreen.x[1][0]*t->Local.Scale_x;
	    v2y=BlockToScreen.x[1][1]*t->Local.Scale_y;
	    v2z=BlockToScreen.x[1][2]*t->Local.Scale_z;
	    v3x=BlockToScreen.x[2][0]*t->Local.Scale_x;
	    v3y=BlockToScreen.x[2][1]*t->Local.Scale_y;
	    v3z=BlockToScreen.x[2][2]*t->Local.Scale_z;
	    xc=BlockToScreen.x[0][3];
	    yc=BlockToScreen.x[1][3];
	    zc=BlockToScreen.x[2][3];
	    k=0;
	    rbig=t->rbig;
	    rsmall=t->rsmall;
	    pi=4*atan(1.0);
	    dumy=1.0/((double)t->ny-1);
	    dumx=1.0/((double)t->nx-1);
	    for(j=0;j<t->ny;++j){
	        double sinf,cosf;
	        sinf=sin(2*pi*(1.-j*dumy)-pi);
	        cosf=cos(2*pi*(1.-j*dumy)-pi);
	        for(i=0;i<t->nx;++i){
	           dx=t->origin.x+(rbig+rsmall*cosf)*cos(2*pi*(1.-i*dumx));
	           dy=t->origin.y+(rbig+rsmall*cosf)*sin(2*pi*(1.-i*dumx));
	           dz=t->origin.z+rsmall*sinf;
	           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
	           ++k;
	        }
	    }
	    nel=0;
	    for(j=0;j<t->ny-1;++j){
	        for(i=0;i<t->nx-1;++i){
		    ndata[nel++]=t->Material;
		    ndata[nel++]=i+j*t->nx;
		    ndata[nel++]=i+1+j*t->nx;
		    ndata[nel++]=i+1+(j+1)*t->nx;
		    ndata[nel++]=i+(j+1)*t->nx;
	        }
	    }
	    nel /= 5;

	    vdata=zdatal;
	    if(gdata){
		VecCopy(scene->Eyel,Eyel);
		VecCopy(scene->Lampl,Lampl);
	    	k=0;
	        for(j=0;j<t->ny;++j){
	            double sinf,cosf;
	            sinf=sin(2*pi*(1.-j*dumy)-pi);
	            cosf=cos(2*pi*(1.-j*dumy)-pi);
		    for(i=0;i<t->nx;++i){
		        Vec P,N,D,L;
		        double len;
		        dx=rsmall*cosf*cos(2*pi*(1.-i*dumx));
		        dy=rsmall*cosf*sin(2*pi*(1.-i*dumx));
		        dz=rsmall*sinf;
		        
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
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	    }else if(nvdata){
	    	k=0;
	        for(j=0;j<t->ny;++j){
	            double sinf,cosf;
	            sinf=sin(2*pi*(1.-j*dumy)-pi);
	            cosf=cos(2*pi*(1.-j*dumy)-pi);
		    for(i=0;i<t->nx;++i){
		        dx=rsmall*cosf*cos(2*pi*(1.-i*dumx));
		        dy=rsmall*cosf*sin(2*pi*(1.-i*dumx));
		        dz=rsmall*sinf;		        
			 nvdata[k++]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
			 nvdata[k++]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
			 nvdata[k++]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
		    }
	    	}
	    	if(rdata && sdata){
		    dx=0;
		    if(t->nx > 1)dx=1.0/((double)(t->nx-1));

		    dy=0;
		    if(t->ny > 1)dy=1.0/((double)(t->ny-1));

	    	    k=0;
	            for(j=0;j<t->ny;++j){
		        for(i=0;i<t->nx;++i){
			    rdata[k]=(float)(i*dx);
			    sdata[k++]=(float)(j*dy);
		        }
	    	    }
	    	}
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	    }

	*CellCount += nel;
	t->LastCell = *CellCount;

	return 0;
}
int CTorusMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CTorusPtr t=(CTorusPtr)o;
	long NodeCount;
	long CellCount;

	NodeCount=t->nx*t->ny;
	CellCount=(t->nx-1)*(t->ny-1)*5;
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(t->scene->SmoothNormals)*MostNormals=1;
	return 0;
}
CObjectPtr CTorusDuplicate(CObject *b)
{
	CTorusPtr t=(CTorusPtr)b;
	CTorusPtr torusNew;

	if(!t)return (CObjectPtr)NULL;

	torusNew=(CTorusPtr)cMalloc(sizeof(CTorus),8587);
	if(!torusNew)return (CObjectPtr)NULL;

	*torusNew = *t;
	return (CObjectPtr)torusNew;
}
int CTorusCount(CObject *o,long *nodes,long *cells)
{
	CTorusPtr t=(CTorusPtr)o;

	*nodes += t->nx*t->ny;
	*cells += (t->nx-1)*(t->ny-1);
	return 0;
}
int CTorusMessage(CObject *b,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	CTorusPtr s=(CTorusPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    struct P p;
	    double r;
	    static double x[8]={-1.,1.,1.,-1., -1.,1.,1.,-1.};
	    static double y[8]={-1.,-1.,1.,1., -1.,-1.,1.,1.};
	    static double z[8]={-1.,-1.,-1.,-1.,1.,1.,1.,1.};
	    int n;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);

	    r=s->rbig+s->rsmall;
	    for(n=0;n<8;++n){
	        p.x=s->origin.x+r*x[n];
	        p.y=s->origin.y+r*y[n];
	        p.z=s->origin.z+r*z[n];
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
static CObjectPtr uTorusWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CTorusStruct b;
	
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
	
		{"REAL",4015,universal_TypeLong,(void *)&b.nx},
		{"REAL",4016,universal_TypeLong,(void *)&b.ny},
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.origin.x},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.origin.y},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.origin.z},	
		{"DOUB",4030,universal_TypeDouble,(void *)&b.rbig},	
		{"DOUB",4031,universal_TypeDouble,(void *)&b.rsmall},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CTorusPtr bp=(CTorusPtr)o;
	char head[5];
	void *op;
	long length;
	long tag;
	
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=CTorusCreate(l->scene,(int)b.Material,(int)b.ny,(int)b.ny,
                         b.rbig,b.rsmall,&b.origin);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		rotate3d(&(bp->Local));	
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	op=&b;
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
CTorusPtr CTorusCreate(struct Scene *scene,int mat,int nx,int ny,
                         double rbig,double rsmall,struct P *origin)
{
	CTorusPtr torus;

	if(!scene)return (CTorusPtr)NULL;
	torus=(CTorusPtr)cMalloc(sizeof(CTorus),8588);
	if(!torus)return (CTorusPtr)NULL;
	zerol((char *)torus,sizeof(CTorus));

	CObjectCreate(scene,(CObjectPtr)torus);
	torus->Material=mat;
	torus->nx=nx;
	torus->ny=ny;
	torus->rbig=rbig;
	torus->rsmall=rsmall;
	torus->origin= *origin;
	torus->pGetMost=CTorusMost;
	torus->drawObject=CTorusDraw;
	torus->duplicateObject=CTorusDuplicate;
	torus->countObject=CTorusCount;
	torus->message=CTorusMessage;
	torus->ReadWrite=uTorusWrite;				
	torus->type=G_TORUS;
	return torus;
}
int CSphereDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CSpherePtr s=(CSpherePtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	float *rdata,*sdata;
	double r,pi,dumx,dumy;
	long *ndata,nel;
	long k;
	Vec Lampl,Eyel;
	int i,j;
	scene=s->scene;
	if(flag == 1){
	    if(scene->material[s->Material].opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(scene->material[s->Material].opacity == 1.0)return 0;
	}

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	gdata=scene->gdata;
	nvdata=scene->nvdata;
	ndata=scene->cdata;
	rdata=scene->rdata;
	sdata=scene->sdata;

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

	    s->FirstCell = *CellCount;

	    /* Must Rotate local system to un-rotated local */
	    BuildTransformMatrix(&BlockToWorld,&s->Local,&scene->Global);
	    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);
	    v1x=BlockToScreen.x[0][0]*s->Local.Scale_x;
	    v1y=BlockToScreen.x[0][1]*s->Local.Scale_y;
	    v1z=BlockToScreen.x[0][2]*s->Local.Scale_z;
	    v2x=BlockToScreen.x[1][0]*s->Local.Scale_x;
	    v2y=BlockToScreen.x[1][1]*s->Local.Scale_y;
	    v2z=BlockToScreen.x[1][2]*s->Local.Scale_z;
	    v3x=BlockToScreen.x[2][0]*s->Local.Scale_x;
	    v3y=BlockToScreen.x[2][1]*s->Local.Scale_y;
	    v3z=BlockToScreen.x[2][2]*s->Local.Scale_z;
	    xc=BlockToScreen.x[0][3];
	    yc=BlockToScreen.x[1][3];
	    zc=BlockToScreen.x[2][3];
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
	           dx=s->origin.x+r*cos(2*pi*(1.-i*dumx))*sinf;
	           dy=s->origin.y+r*sin(2*pi*(1.-i*dumx))*sinf;
	           dz=s->origin.z+r*cosf;
	           xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	           ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	           zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
	           ++k;
	        }
	    }

	    nel=0;
	    for(j=0;j<s->ny-1;++j){
	        for(i=0;i<s->nx-1;++i){
		    ndata[nel++]=s->Material;
		    ndata[nel++]=i+j*s->nx;
		    ndata[nel++]=i+1+j*s->nx;
		    ndata[nel++]=i+1+(j+1)*s->nx;
		    ndata[nel++]=i+(j+1)*s->nx;
	        }
	    }
	    nel /= 5;
	    vdata=zdatal;
	    vdata=NULL;

	    if(gdata){
	    	k=0;
	        for(j=0;j<s->ny;++j){
	            double sinf,cosf;
	            sinf=sin(pi*(1.-j*dumy));
	            cosf=cos(pi*(1.-j*dumy));
		    for(i=0;i<s->nx;++i){
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
		(*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
	    }else if(nvdata){
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
		(*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
	    }else{
		(*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
	    }


	*CellCount += nel;
	s->LastCell = *CellCount;

	return 0;
}
int CSphereMost(CObject *o,long *MostNodes,long *MostNormals,
		      long *MostCells)
{
	CSpherePtr s=(CSpherePtr)o;
	long NodeCount;
	long CellCount;

	NodeCount=s->nx*s->ny;
	CellCount=(s->nx-1)*(s->ny-1)*5;
	if(NodeCount > *MostNodes)*MostNodes = NodeCount;
	if(CellCount > *MostCells)*MostCells = CellCount;
	if(s->scene->SmoothNormals)*MostNormals=1;
	return 0;
}
CObjectPtr CSphereDuplicate(CObject *b)
{
	CSpherePtr s=(CSpherePtr)b;
	CSpherePtr sphereNew;

	if(!s)return (CObjectPtr)NULL;

	sphereNew=(CSpherePtr)cMalloc(sizeof(CSphere),8589);
	if(!sphereNew)return (CObjectPtr)NULL;

	*sphereNew = *s;
	return (CObjectPtr)sphereNew;
}
int CSphereCount(CObject *o,long *nodes,long *cells)
{
	CSpherePtr s=(CSpherePtr)o;

	*nodes += s->nx*s->ny;
	*cells += (s->nx-1)*(s->ny-1);
	return 0;
}
int CSphereMessage(CObject *b,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	CSpherePtr s=(CSpherePtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    struct P p;
	    double r;
	    static double x[8]={-1.,1.,1.,-1., -1.,1.,1.,-1.};
	    static double y[8]={-1.,-1.,1.,1., -1.,-1.,1.,1.};
	    static double z[8]={-1.,-1.,-1.,-1.,1.,1.,1.,1.};
	    int n;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);

	    r=s->radius;
	    for(n=0;n<8;++n){
	        p.x=s->origin.x+r*x[n];
	        p.y=s->origin.y+r*y[n];
	        p.z=s->origin.z+r*z[n];
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
static CObjectPtr uSphereWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CSphereStruct b;
	
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
	
		{"REAL",4015,universal_TypeLong,(void *)&b.nx},
		{"REAL",4016,universal_TypeLong,(void *)&b.ny},
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},
		{"DOUB",4018,universal_TypeDouble,(void *)&b.origin.x},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.origin.y},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.origin.z},	
		{"DOUB",4030,universal_TypeDouble,(void *)&b.radius},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CSpherePtr bp=(CSpherePtr)o;
	char head[5];
	void *op;
	long length;
	long tag;
	
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=CSphereCreate(l->scene,(int)b.Material,(int)b.ny,(int)b.ny,
                         b.radius,&b.origin);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(struct CSphereStruct);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;	
		rotate3d(&(bp->Local));	
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	op=&b;
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
CSpherePtr CSphereCreate(struct Scene *scene,int mat,int nx,int ny,
                         double radius,struct P *origin)
{
	CSpherePtr sphere;

	if(!scene)return (CSpherePtr)NULL;
	sphere=(CSpherePtr)cMalloc(sizeof(CSphere),8590);
	if(!sphere)return (CSpherePtr)NULL;
	zerol((char *)sphere,sizeof(CSphere));

	CObjectCreate(scene,(CObjectPtr)sphere);
	sphere->Material=mat;
	sphere->nx=nx;
	sphere->ny=ny;
	sphere->radius=radius;
	sphere->origin= *origin;
	sphere->pGetMost=CSphereMost;
	sphere->drawObject=CSphereDraw;
	sphere->duplicateObject=CSphereDuplicate;
	sphere->countObject=CSphereCount;
	sphere->message=CSphereMessage;
	sphere->type=G_SPHERE;
	sphere->ReadWrite=uSphereWrite;
	return sphere;
}
int CObjectGetMost(CObject *o,long *MostNodes,long *MostNormals,long *MostCells)
{
	//o=o;
	//MostNodes=MostNodes;
	//MostNormals=MostNormals;
	//MostCells=MostCells;
	return 0;
}
int CObjectCount(CObject *o,long *nodes,long *cells)
{
	//o=o;
	//nodes=nodes;
	//cells=cells;
	return 0;
}
int CObjectkill(CObject *o)
{
	if(!o)return 1;
	cFree((char *)o);
	return 0;
}
int CObjectdrawObject(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	//o=o;
	//WorldToScreen=WorldToScreen;
	//CellCount=CellCount;
	//flag=flag;
	return 0;
}
static CObjectPtr CObjectDuplicate(CObject *b)
{
	//b=b;
	return (CObjectPtr)NULL;
}
int CObjectMessage(CObject *b,struct ObjectMessage *Message)
{
	//b=b;
	//Message=Message;
	return 0;
}
void CObjectCreate(struct Scene *scene,CObject *b)
{
	if(!scene || !b)return;
	b->scene=scene;
 	b->Local=scene->Global;
 	b->Local.Scale_x=1.0;
 	b->Local.Scale_y=1.0;
 	b->Local.Scale_z=1.0;
	b->pGetMost=CObjectGetMost;
	b->drawObject=CObjectdrawObject;
	b->killObject=CObjectkill;
	b->duplicateObject=CObjectDuplicate;
	b->countObject=CObjectCount;
	b->message=CObjectMessage;
	b->type=0;
	b->cycleFlag=1;
	b->ItWas = -1;
	return;
}
int CBlockMost(CObject *o,long *MostNodes,long *MostNormals,long *MostCells)
{
	struct Scene *scene;
	CBlockPtr b=(CBlockPtr)o;
	struct Block *block;

	scene=b->scene;
	if(b->BlockNumber < 0  || b->BlockNumber >= scene->BlockCount)return 1;
	block= &scene->block[b->BlockNumber];
	if(block->NodeCount > *MostNodes)*MostNodes = block->NodeCount;
	if(block->nx && block->nx && block->nz)*MostNormals = 1;
	//MostCells=MostCells;
	return 0;
}
int CBlockCount(CObject *o,long *nodes,long *cells)
{
	struct Scene *scene;
	CBlockPtr b=(CBlockPtr)o;
	struct Block *block;

	scene=b->scene;
	if(b->BlockNumber < 0  || b->BlockNumber >= scene->BlockCount)return 1;
	block= &scene->block[b->BlockNumber];
	*nodes += block->NodeCount;
	*cells += block->ElementCount;
	return 0;
}
int doBlockValues(struct Block *block,double *vdata,struct Image *ImageData)
{
	struct FileInfo2 *Files;
	struct SetFrameData *s;
	double xfac,yfac,zfac,dx,dy,dz;
	struct LIMITS *limits;
	double *v;
	long i,j,k,xsize,xysize;
	long n;

    double xmin,xmax,x;
    double ymin,ymax,y;
    double zmin,zmax,z;
    double didx,djdy,dkdz;
	double dxdi,dydj,dzdk;
  
	if(!block || !vdata || !ImageData)return 1;
	Files=ImageData->Files;
	if(!Files || (Files->fileClass != SDS3D_FILES))return 1;
	s = &Files->SetData;
	limits=&s->limits;
	
	if(limits && ((limits->iGotData & 63) == 63)){
	
		xmin=limits->xmin;
		xmax=limits->xmax;		
		didx=(double)(s->xsize)/(xmax-xmin);
		dxdi = 1.0/didx;
		
		ymin=limits->ymin;
		ymax=limits->ymax;
		djdy=(double)(s->ysize)/(ymax-ymin);
		dydj = 1.0/djdy;
		
		zmin=limits->zmin;
		zmax=limits->zmax;
		dkdz=(double)(s->zsize)/(zmax-zmin);
		dzdk = 1.0/dkdz;
		
		

		xsize=s->xsize;
		xysize=xsize*s->ysize;
		v=(double *)Files->dataSave;
		if(!v)return 1;
		for(n=0;n<block->NodeCount;++n){
		    
		    x=block->xdata[n];
			if(x < xmin || x > xmax)continue;
			y=block->ydata[n];
			if(y < ymin || y > ymax)continue;
			z=block->zdata[n];
			if(z < zmin || z > zmax)continue;
		    
		    i=(long)((x-xmin)*didx-0.5);
		    if(i < 0)i=0;
		    if(i >= s->xsize-1)i = s->xsize-2;
		    dx=didx*(block->xdata[n]-(((double)i+.5)*dxdi+xmin));
		    
		    j=(long)((y-ymin)*djdy-0.5);
		    if(j < 0)j=0;
		    if(j >= s->ysize-1)j = s->ysize-2;
		    dy=djdy*(block->ydata[n]-(((double)j+.5)*dydj+ymin));
		    
		    k=(long)((z-zmin)*dkdz-0.5);
		    if(k < 0)k=0;
		    if(k >= s->zsize-1)k = s->zsize-2;
		    dz=dkdz*(block->zdata[n]-(((double)k+.5)*dzdk+zmin));
		    		   	    
		    vdata[n]=((1.-dx)*(1.-dy)*(1.-dz)*v[i+   j*xsize+    k*xysize]+
			              dx*(1.-dy)*(1.-dz)*v[i+1+ j*xsize+    k*xysize]+
			                   dx*dy*(1.-dz)*v[i+1+(j+1)*xsize+ k*xysize]+
			              (1.-dx)*dy*(1.-dz)*v[i+  (j+1)*xsize+ k*xysize]+
			              (1.-dx)*(1.-dy)*dz*v[i+   j*xsize+   (k+1)*xysize]+
			                   dx*(1.-dy)*dz*v[i+1+ j*xsize+   (k+1)*xysize]+
			                        dx*dy*dz*v[i+1+(j+1)*xsize+(k+1)*xysize]+
			                   (1.-dx)*dy*dz*v[i+  (j+1)*xsize+(k+1)*xysize]);
			     
		}
	}else{

		xfac=.5*((double)(s->xsize-1));
		yfac=.5*((double)(s->ysize-1));
		zfac=.5*((double)(s->zsize-1));
		xsize=s->xsize;
		xysize=xsize*s->ysize;
		v=(double *)Files->dataSave;
		if(!v)return 1;
		for(n=0;n<block->NodeCount;++n){
		    
		    
		    
		    i=(long)(block->xdata[n]+xfac);
		    if(i < 0)i=0;
		    if(i >= s->xsize-1)i = s->xsize-2;
		    dx=block->xdata[n]-((double)i-xfac);
		    
		    j=(long)(block->ydata[n]+yfac);
		    if(j < 0)j=0;
		    if(j >= s->ysize-1)j = s->ysize-2;
		    dy=block->ydata[n]-((double)j-yfac);
		    
		    k=(long)(block->zdata[n]+zfac);
		    if(k < 0)k=0;
		    if(k >= s->zsize-1)k = s->zsize-2;
		    dz=block->zdata[n]-((double)k-zfac);
		    
	/*	    
		    x=dx;
		    if(x < xmin)xmin = x;
		    if(x > xmax)xmax = x;
		    
		    y=dy;
		    if(y < ymin)ymin = y;
		    if(y > ymax)ymax = y;
		    
		    z=dz;
		    if(z < zmin)zmin = z;
		    if(z > zmax)zmax = z;
		    
	*/	    
		   	    
		    vdata[n]=((1.-dx)*(1.-dy)*(1.-dz)*v[i+   j*xsize+    k*xysize]+
			              dx*(1.-dy)*(1.-dz)*v[i+1+ j*xsize+    k*xysize]+
			                   dx*dy*(1.-dz)*v[i+1+(j+1)*xsize+ k*xysize]+
			              (1.-dx)*dy*(1.-dz)*v[i+  (j+1)*xsize+ k*xysize]+
			              (1.-dx)*(1.-dy)*dz*v[i+   j*xsize+   (k+1)*xysize]+
			                   dx*(1.-dy)*dz*v[i+1+ j*xsize+   (k+1)*xysize]+
			                        dx*dy*dz*v[i+1+(j+1)*xsize+(k+1)*xysize]+
			                   (1.-dx)*dy*dz*v[i+  (j+1)*xsize+(k+1)*xysize]);
			     
		}
	/*	
		sprintf(WarningBuff," xmin %f xmax %f \n ymin %f ymax %f \n zmin %f zmax %f \n",xmin,xmax,ymin,ymax,zmin,zmax);
		WarningBatch(WarningBuff);
	*/				
	}
	return 0;
}

static int BlockDrawLines(CObject *o,struct Matrix *WorldToScreen,long *CellCount)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	CBlockPtr b=(CBlockPtr)o;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	struct Scene *scene;
	struct Block *block;
	double *vdata;
	double *xdatal,*ydatal,*zdatal;
	long n;
	
	if(!b)return 1;
	scene=b->scene;
	if(!scene)return 1;

	block = &(scene->block[b->BlockNumber]);

    block->FirstCell = *CellCount;
    b->FirstCell = *CellCount;
	
	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	vdata=scene->vdata;
	
    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&b->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*b->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*b->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*b->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*b->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*b->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*b->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*b->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*b->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*b->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
    
    for(n=0;n<block->NodeCount;++n){
		double vv;
	   dx=block->xdata[n]+b->origin.x;
	   dy=block->ydata[n]+b->origin.y;
	   dz=block->zdata[n]+b->origin.z;
	   xdatal[n]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	   ydatal[n]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	   zdatal[n]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		vv=block->vdata[n];
		vdata[n]=vv;
    }
    
	doNoShadowLines4(block->Elements,xdatal,ydatal,zdatal,vdata,block->ElementCount,*CellCount,scene,block->pa.DrawZonesColor);

	*CellCount += block->ElementCount;
	b->LastCell = *CellCount;
	
	return 0;
}
int CBlockDraw(CObject *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CBlockPtr b=(CBlockPtr)o;
	struct Scene *scene;
	struct Matrix BlockToWorld;
	struct Matrix BlockToScreen;
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	double *xdata,*ydata,*zdata,*vdata;
	double *xdatal,*ydatal,*zdatal,*gdata;
	float *nvdata;
	struct Material *material;
	float *rdata,*sdata;
	long *ndata,nodes,nel;
	struct Block *block;
	long k,step,mat;
	Vec Lampl,Eyel;


	scene=b->scene;
	mat=b->Material;
	if(mat < 0)mat = -mat;
	material = &scene->material[mat];
	if(flag == 1){
	    if(material->opacity != 1.0)return 0;
	}else if(flag == 2){
	    if(material->opacity == 1.0)return 0;
	}

	block = &(scene->block[b->BlockNumber]);
	
	if(block->BlockType == 1){ 
	    return BlockDrawLines(o,WorldToScreen,CellCount);
	}

	nodes=block->NodeCount;
	nel=block->ElementCount;

	if(!nodes || !nel)return 0;

	if(scene->SmoothNormals){
	    if(!block->nx || !block->ny || !block->nz){
	        SetNormalsBlock(block);
	    }
	}

	if(scene->SmoothNormals == 2 || scene->SmoothNormals == 3){
	    if(SetBlockRS(block))return 1;
	}

	xdatal=scene->xdatal;
	ydatal=scene->ydatal;
	zdatal=scene->zdatal;
	gdata=scene->gdata;
	nvdata=scene->nvdata;
	rdata=scene->rdata;
	sdata=scene->sdata;
	vdata=scene->vdata;

	VecCopy(scene->Eyel,Eyel);
	VecCopy(scene->Lampl,Lampl);

    block->FirstCell = *CellCount;
    b->FirstCell = *CellCount;

    xdata=block->xdata;
    ydata=block->ydata;
    zdata=block->zdata;
    ndata=block->Elements;


    /* Must Rotate local system to un-rotated local */
    BuildTransformMatrix(&BlockToWorld,&b->Local,&scene->Global);
    MultMatrix(&BlockToScreen,WorldToScreen,&BlockToWorld);

    v1x=BlockToScreen.x[0][0]*b->Local.Scale_x;
    v1y=BlockToScreen.x[0][1]*b->Local.Scale_y;
    v1z=BlockToScreen.x[0][2]*b->Local.Scale_z;
    v2x=BlockToScreen.x[1][0]*b->Local.Scale_x;
    v2y=BlockToScreen.x[1][1]*b->Local.Scale_y;
    v2z=BlockToScreen.x[1][2]*b->Local.Scale_z;
    v3x=BlockToScreen.x[2][0]*b->Local.Scale_x;
    v3y=BlockToScreen.x[2][1]*b->Local.Scale_y;
    v3z=BlockToScreen.x[2][2]*b->Local.Scale_z;
    xc=BlockToScreen.x[0][3];
    yc=BlockToScreen.x[1][3];
    zc=BlockToScreen.x[2][3];
    for(k=0;k<nodes;++k){
        dx=xdata[k];
        dy=ydata[k];
        dz=zdata[k];
        xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
        ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
        zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
    }

    step=block->NodesMax+1;

    if(b->Material >= 0){
        for(k=0;k<nel;++k){
            ndata[k*step]=b->Material;
        }
    }

    if(material->PatternType == PATTERN_IMAGE){
		doBlockValues(block,vdata,&material->ImageData);
    }else{
		vdata=block->vdata;
    }

	if(scene->SmoothNormals && block->nx && block->ny && block->nz){
		double *nx,*ny,*nz;
		
		nx=block->nx;
		ny=block->ny;
		nz=block->nz;
		if(gdata){
		    for(k=0;k<nodes;++k){
		        Vec P,N,D,L;
		        double len;
		        
		        dx=nx[k];
		        dy=ny[k];
		        dz=nz[k];
		        
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
		    }
		    if(block->NodesMax == 3){
		         (*scene->doNoShadowG)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
		    }else if(block->NodesMax == 4){
		         (*scene->doNoShadow4G)(ndata,xdatal,ydatal,zdatal,vdata,gdata,nel,*CellCount,scene);
		    }		    
		}else if(nvdata){
		    for(k=0;k<nodes;++k){		        
		        dx=nx[k];
		        dy=ny[k];
		        dz=nz[k];
				nvdata[k*3]=(float)((dx)*v1x+(dy)*v1y+(dz)*v1z);
				nvdata[k*3+1]=(float)((dx)*v2x+(dy)*v2y+(dz)*v2z);
				nvdata[k*3+2]=(float)((dx)*v3x+(dy)*v3y+(dz)*v3z);
		    }
		    if(rdata && sdata && block->rdata && block->sdata){
		        for(k=0;k<nodes;++k){		        
	 			    rdata[k]=block->rdata[k];
				    sdata[k]=block->sdata[k];
		        }
		    }
		    if(block->NodesMax == 3){
		         (*scene->doNoShadowP)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
		    }else if(block->NodesMax == 4){
		         (*scene->doNoShadow4P)(ndata,xdatal,ydatal,zdatal,vdata,nvdata,nel,*CellCount,scene);
		    }
		}
	}else{
		if(block->NodesMax == 3){
		    (*scene->doNoShadow)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
		}else if(block->NodesMax == 4){
		    (*scene->doNoShadow4)(ndata,xdatal,ydatal,zdatal,vdata,nel,*CellCount,scene);
		}
	}

	*CellCount += nel;
	b->LastCell = *CellCount;
	
	
	if(b->ShowNormals){
	    long NodesMax,ne,n,n1,n2,n3,n4,nn,np,nel,*nee;
		struct P v1,v2,norm;
		double len;
	
	     if((nee=(long *)cMalloc(sizeof(long)*block->ElementCount*3,8611)) == NULL){
		     goto OutOfHere;
		 }

	    
	    np=0;
	    
	    nel=0;
	    
		NodesMax=block->NodesMax;
    
		for(ne=0;ne<block->ElementCount;++ne){
			nn=(NodesMax+1)*ne;
			n1=block->Elements[nn+1];
			n2=block->Elements[nn+2];
			n3=block->Elements[nn+3];

			v1.x=block->xdata[n2]-block->xdata[n1];
			v1.y=block->ydata[n2]-block->ydata[n1];
			v1.z=block->zdata[n2]-block->zdata[n1];
			v2.x=block->xdata[n3]-block->xdata[n1];
			v2.y=block->ydata[n3]-block->ydata[n1];
			v2.z=block->zdata[n3]-block->zdata[n1];
			
		    norm.x =   v1.y*v2.z-v2.y*v1.z;
		    norm.y = -(v1.x*v2.z-v2.x*v1.z);
		    norm.z =   v1.x*v2.y-v2.x*v1.y;
		    
		    len=sqrt(norm.x*norm.x+norm.y*norm.y+norm.z*norm.z);
		    if(!len)continue;
		    
		    /* len=sqrt(len)/800; */
			
		    len=sqrt(len);
		    
		    norm.x /=  len;
		    norm.y /=  len;
		    norm.z /=  len;
		    		    

		    nee[nel++]=b->Material;
		    nee[nel++]=np;
		    nee[nel++]=np+1;
		    
		    if(NodesMax == 4){
				n4=block->Elements[nn+4];
		    	xdatal[np]=0.25*(block->xdata[n1]+block->xdata[n2]+
		    	           block->xdata[n3]+block->xdata[n4]);
		    	ydatal[np]=0.25*(block->ydata[n1]+block->ydata[n2]+
		    	           block->ydata[n3]+block->ydata[n4]);
		    	zdatal[np]=0.25*(block->zdata[n1]+block->zdata[n2]+
		    	           block->zdata[n3]+block->zdata[n4]);
		    }else {
		    	xdatal[np]=0.333333*(block->xdata[n1]+block->xdata[n2]+block->xdata[n3]);
		    	ydatal[np]=0.333333*(block->ydata[n1]+block->ydata[n2]+block->ydata[n3]);
		    	zdatal[np]=0.333333*(block->zdata[n1]+block->zdata[n2]+block->zdata[n3]);
		    }
		    ++np;
	    	xdatal[np]=xdatal[np-1]+norm.x;
	    	ydatal[np]=ydatal[np-1]+norm.y;
	    	zdatal[np]=zdatal[np-1]+norm.z;
		    ++np;
		    
		    if((np >= nodes-4) || (ne >= block->ElementCount-1)){
		    
				for(n=0;n<np;++n){
				   dx=xdatal[n]+b->origin.x;
				   dy=ydatal[n]+b->origin.y;
				   dz=zdatal[n]+b->origin.z;
				   xdatal[n]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
				   ydatal[n]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
				   zdatal[n]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
			    }

				nel /= 3;
			    
				doNoShadowLines(nee,xdatal,ydatal,zdatal,NULL,nel,*CellCount,scene);

				*CellCount += nel;
				
		    	np=0;
		    	nel=0;
		    }
		  
		}	    
		
	
    	b->LastCell = *CellCount;
    
	    
		if(nee)cFree((char *)nee);
	}
OutOfHere:
	return 0;
}
CObjectPtr CBlockDuplicate(CObject *bb)
{
	CBlockPtr b=(CBlockPtr)bb;
	CBlockPtr blockNew;

	if(!b)return (CObjectPtr)NULL;

	blockNew=(CBlockPtr)cMalloc(sizeof(CBlock),8591);
	if(!blockNew)return (CObjectPtr)NULL;

	*blockNew = *b;
	return (CObjectPtr)blockNew;
}
int CBlockMessage(CObject *b,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	CBlockPtr s=(CBlockPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    struct P p;
	    double *xdata,*ydata,*zdata;
	    struct Block *block;
	    long n,nn,k;
	    
	    BuildTransformMatrix(&GroupToWorld,&s->Local,&s->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&s->Local);

	    block = &(s->scene->block[s->BlockNumber]);
	    xdata=block->xdata;
	    ydata=block->ydata;
	    zdata=block->zdata;

	    if(!xdata || !ydata || !zdata)return 0;
		
		Message->SurfaceCount += block->ElementCount;
	    
	    nn=block->NodesMax+1;
	    
		for(n=0;n<block->ElementCount;++n){
		   for(k=0;k<block->NodesMax;++k){
		   		nn=block->Elements[n*(block->NodesMax+1)+1+k];
		   		if(nn < 0 || nn >= block->NodeCount)continue;
		        p.x=xdata[nn];
		        p.y=ydata[nn];
		        p.z=zdata[nn];
		        Transform(&p,&GroupToScreen);
		        if(p.x > Message->xmax)Message->xmax=p.x;
		        if(p.y > Message->ymax)Message->ymax=p.y;
		        if(p.z > Message->zmax)Message->zmax=p.z;
		        if(p.x < Message->xmin)Message->xmin=p.x;
		        if(p.y < Message->ymin)Message->ymin=p.y;
		        if(p.z < Message->zmin)Message->zmin=p.z;
		   }
		}
	
	} else if(Message->MessageType == OBJECT_MESSAGE_RANGE_LIGHTS){
	    double *xdata,*ydata,*zdata,t;
		struct Block *block;
		long n,nn,k,l;
		Vec P,L;
		
		block = &(s->scene->block[s->BlockNumber]);
		xdata=block->xdata;
		ydata=block->ydata;
		zdata=block->zdata;
		
		if(!xdata || !ydata || !zdata || !rd  || !rd->Lights || !rd->nLights)return 0;
		
		nn=block->NodesMax+1;
		for(n=0;n<block->ElementCount;++n){
			for(k=0;k<block->NodesMax;++k){
		   		nn=block->Elements[n*(block->NodesMax+1)+1+k];
		   		if(nn < 0 || nn >= block->NodeCount)continue;
		        P[0]=xdata[nn];
		        P[1]=ydata[nn];
		        P[2]=zdata[nn];
				for (l = 1; l < rd->nLights; l++) {
					VecSub(rd->Lights[l].light_pos,P, L);
					t = VecNormalize(L);
					if(t)t=1.0/(t*t);
					if(t > Message->vmax)Message->vmax=t;
					if(t < Message->vmin)Message->vmin=t;
				}
						
			}
		}			
		
	}
	return 0;
}
static CObjectPtr uBlockWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CBlockStruct b;
	
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
	
		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
		
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},

		{"DOUB",4018,universal_TypeDouble,(void *)&b.origin.x},	
		{"DOUB",4019,universal_TypeDouble,(void *)&b.origin.y},	
		{"DOUB",4020,universal_TypeDouble,(void *)&b.origin.z},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CBlockPtr bp=(CBlockPtr)o;
	char head[5];
	long length;
	void *op;
	long tag;
	
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
		if(getUniversalTable(&Global,inOut))goto ErrorOut;	
		bp=CBlockCreate(l->scene,b.BlockNumber,b.Material,&b.origin);
		if(!bp)goto  ErrorOut;
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	op=&b;
	
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}

CBlockPtr CBlockCreate(struct Scene *scene,long BlockNumber,long Material,
			struct P *origin)
{
	CBlockPtr block;

	if(!scene)return (CBlockPtr)NULL;
	block=(CBlockPtr)cMalloc(sizeof(CBlock),8592);
	if(!block)return (CBlockPtr)NULL;
	zerol((char *)block,sizeof(CBlock));

	CObjectCreate(scene,(CObjectPtr)block);
	block->BlockNumber=BlockNumber;
	block->Material=Material;
	block->origin= *origin;
	block->pGetMost=CBlockMost;
	block->drawObject=CBlockDraw;
	block->duplicateObject=CBlockDuplicate;
	block->countObject=CBlockCount;
	block->message=CBlockMessage;
	block->type=G_BLOCK;
	block->ReadWrite=uBlockWrite;
	return block;
}
int blockSize(struct Block *b,long mat,long nodes,long cells)
{
	double *xdata,*ydata,*zdata,*vdata;
	long *Elements;
	long j,n,length;
	
	if(!b)return 1;
	Elements=NULL;
	xdata=NULL;
	ydata=NULL;
	zdata=NULL;
	vdata=NULL;
	if(b->NodeCount != nodes || b->ElementCount != cells){
	    if(b->Elements)cFree((char *)b->Elements);
	    b->Elements=NULL;
	    if(b->xdata)cFree((char *)b->xdata);
	    b->xdata=NULL;
	    if(b->ydata)cFree((char *)b->ydata);
	    b->ydata=NULL;
	    if(b->zdata)cFree((char *)b->zdata);
	    b->zdata=NULL;
	    if(b->vdata)cFree((char *)b->vdata);
	    b->vdata=NULL;

	    length=(long)(nodes*sizeof(double));
	    if(!(xdata=(double *)cMalloc(length,8552))){
		    goto ErrorOut;
	    }
	    if(!(ydata=(double *)cMalloc(length,8553))){
		    goto ErrorOut;
	    }
	     if(!(zdata=(double *)cMalloc(length,8554))){
		    goto ErrorOut;
	    }
	    if(!(vdata=(double *)cMalloc(length,8555))){
		    goto ErrorOut;
	    }
	    length=(long)(cells*sizeof(long)*5);
	    if(!(Elements=(long *)cMalloc(length,8556))){
		    goto ErrorOut;
	    }
	    b->Elements=Elements;
	    b->xdata=xdata;
	    b->ydata=ydata;
	    b->zdata=zdata;
	    b->vdata=vdata;
	    b->NodeCount = nodes;


	    n=0;
	    for(j=0;j<cells;++j){
			Elements[n++]=mat;
			Elements[n++]=0;
			Elements[n++]=0;
			Elements[n++]=0;
			Elements[n++]=0;
	    }

	    b->ElementCount=cells;
	    b->NodesMax=4;
	}
	return 0;
ErrorOut:
	sprintf(WarningBuff,"blockSize Out of Memory Requested %ld\n",length);
	WarningBatch(WarningBuff);
	if(Elements)cFree((char *)Elements);
	if(xdata)cFree((char *)xdata);
	if(ydata)cFree((char *)ydata);
	if(zdata)cFree((char *)zdata);
	if(vdata)cFree((char *)vdata);
	return 1;
}
int blockFree(struct Block *block)
{
	if(!block)return 1;

	block->NodeCount = 0;
	block->ElementCount=0;
	block->NodesMax=0;

	if(block->xdata){
	    cFree((char *)block->xdata);
	    block->xdata=NULL;
	}
	if(block->ydata){
	    cFree((char *)block->ydata);
	    block->ydata=NULL;
	}
	if(block->zdata){
	    cFree((char *)block->zdata);
	    block->zdata=NULL;
	}

	if(block->dxdata){
	    cFree((char *)block->dxdata);
	    block->dxdata=NULL;
	}
	if(block->dydata){
	    cFree((char *)block->dydata);
	    block->dydata=NULL;
	}
	if(block->dzdata){
	    cFree((char *)block->dzdata);
	    block->dzdata=NULL;
	}

	if(block->oldx){
	    cFree((char *)block->oldx);
	    block->oldx=NULL;
	}
	if(block->oldy){
	    cFree((char *)block->oldy);
	    block->oldy=NULL;
	}
	if(block->oldz){
	    cFree((char *)block->oldz);
	    block->oldz=NULL;
	}
	if(block->vdata){
	    cFree((char *)block->vdata);
	    block->vdata=NULL;
	}
	if(block->Elements){
	    cFree((char *)block->Elements);
	    block->Elements=NULL;
	}
	if(block->nx){
	    cFree((char *)block->nx);
	    block->nx=NULL;
	}
	if(block->ny){
	    cFree((char *)block->ny);
	    block->ny=NULL;
	}
	if(block->nz){
	    cFree((char *)block->nz);
	    block->nz=NULL;
	}
	
	if(block->rdata){
		cFree((char *)block->rdata);
		block->rdata = NULL;
	}
	if(block->sdata){
	   cFree((char *)block->sdata);
	   block->sdata = NULL;
	}
	if(block->stress){
	   cFree((char *)block->stress);
	   block->stress = NULL;
	}
		    
	return 0;
}
int blockFix(struct Block *b,long mat,long xsize,long ysize,double xoff,double yoff)
{
	double *xdata,*ydata,*zdata,*vdata;
	long *Elements,ne;
	long i,j,n,length;
	
	if(!b)return 1;
	if(b->NodeCount != xsize*ysize){
	    Elements=NULL;
	    xdata=NULL;
	    ydata=NULL;
	    zdata=NULL;
	    vdata=NULL;
	    if(b->Elements)cFree((char *)b->Elements);
	    b->Elements=NULL;
	    if(b->xdata)cFree((char *)b->xdata);
	    b->xdata=NULL;
	    if(b->ydata)cFree((char *)b->ydata);
	    b->ydata=NULL;
	    if(b->zdata)cFree((char *)b->zdata);
	    b->zdata=NULL;
	    if(b->vdata)cFree((char *)b->vdata);
	    b->vdata=NULL;

	    length=(long)(xsize*ysize*sizeof(double));
	    if(!(xdata=(double *)cMalloc(length,8557))){
		    goto ErrorOut;
	    }
	    if(!(ydata=(double *)cMalloc(length,8558))){
		    goto ErrorOut;
	    }
	     if(!(zdata=(double *)cMalloc(length,8559))){
		    goto ErrorOut;
	    }
	    if(!(vdata=(double *)cMalloc(length,8560))){
		    goto ErrorOut;
	    }
	    length=(long)(xsize*ysize*sizeof(long)*5);
	    if(!(Elements=(long *)cMalloc(length,8561))){
		    goto ErrorOut;
	    }
	    b->Elements=Elements;
	    b->xdata=xdata;
	    b->ydata=ydata;
	    b->zdata=zdata;
	    b->vdata=vdata;
	    b->NodeCount = xsize*ysize;
	    b->NodesMax=4;
	}
	    Elements=b->Elements;
	    xdata=b->xdata;
	    ydata=b->ydata;

	    for(j=0;j<ysize;++j){
	        for(i=0;i<xsize;++i){
	            ne=i+(ysize-1-j)*xsize;
      		    xdata[ne]=((double)i-xoff);
      		    ydata[ne]=((double)(ysize-1-j)-yoff);
	        }
	    }

	    n=0;
	    for(j=0;j<ysize-1;++j){
	        for(i=0;i<xsize-1;++i){
			    Elements[n++]=mat;
			    Elements[n++]=i+j*xsize;
			    Elements[n++]=i+1+j*xsize;
			    Elements[n++]=i+1+(j+1)*xsize;
			    Elements[n++]=i+(j+1)*xsize;
	        }
	    }

	    b->ElementCount=n/5;

	return 0;
ErrorOut:
	sprintf(WarningBuff,"BlockFix Out of Memory Requested %ld\n",length);
	WarningBatch(WarningBuff);
	if(Elements)cFree((char *)Elements);
	if(xdata)cFree((char *)xdata);
	if(ydata)cFree((char *)ydata);
	if(zdata)cFree((char *)zdata);
	if(vdata)cFree((char *)vdata);
	return 1;
}
int ObjectListAddBlock(struct Scene *scene,long k,long mat)
{
    static struct P origin;
	CBlockPtr block;

	if(!scene)return 1;

	block=CBlockCreate(scene,k,mat,&origin);
	if(!block)return 1;

	if(ListAddObjectList(scene->ObjectList,(CObjectPtr)block)){
	    cFree((char *)block);
	    return 1;
	}
	return 0;
}
int CObjectGroupMost(CObject *o,long *MostNodes,long *MostNormals,long *MostCells)
{
	CGroupPtr g=(CGroupPtr)o;
	long n;

	for(n=0;n<g->ObjectCount;++n){
	    CObjectPtr Current;
	    if((Current=g->Objects[n]) == NULL)continue;
	    (*Current->pGetMost)(Current,MostNodes,MostNormals,MostCells);
	}
	return 0;
}
int CObjectGroupMessage(CObject *o,struct ObjectMessage *Message)
{
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	struct Matrix *save;
	CGroupPtr g=(CGroupPtr)o;
	long n;

	save=NULL;
	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    BuildTransformMatrix(&GroupToWorld,&g->Local,&g->scene->Global);
	    MultMatrix(&GroupToScreen,Message->WorldToScreen,&GroupToWorld);
	    ScaleMatrix2(&GroupToScreen,&g->Local);
	    save=Message->WorldToScreen;
	    Message->WorldToScreen=&GroupToScreen;
	}

	for(n=0;n<g->ObjectCount;++n){
	    CObjectPtr Current;
	    if((Current=g->Objects[n]) == NULL)continue;
	    (*Current->message)(Current,Message);
	}

	if(Message->MessageType == OBJECT_MESSAGE_RANGE_DATA){ 
	    Message->WorldToScreen=save;
	}
	return 0;
}

int CObjectGroupCount(CObject *o,long *nodes,long *cells)
{
	CGroupPtr g=(CGroupPtr)o;
	long n;

	for(n=0;n<g->ObjectCount;++n){
	    CObjectPtr Current;
	    if((Current=g->Objects[n]) == NULL)continue;
	    (*Current->countObject)(Current,nodes,cells);
	}
	return 0;
}
int CObjectdrawGroup(struct CObjectStruct *o,struct Matrix *WorldToScreen,
		      long *CellCount,int flag)
{
	CGroupPtr g=(CGroupPtr)o;
	struct Matrix GroupToWorld;
	struct Matrix GroupToScreen;
	long n;

	BuildTransformMatrix(&GroupToWorld,&g->Local,&g->scene->Global);
	MultMatrix(&GroupToScreen,WorldToScreen,&GroupToWorld);
	ScaleMatrix2(&GroupToScreen,&g->Local);

	g->FirstCell = *CellCount;

	for(n=0;n<g->ObjectCount;++n){
	    CObjectPtr Current;
	    if((Current=g->Objects[n]) == NULL)continue;
	    (*Current->drawObject)(Current,&GroupToScreen,CellCount,flag);
	}
	g->LastCell = *CellCount;
	return 0;
}
static CObjectPtr CGroupDuplicate(CObject *bb)
{
	CGroupPtr g=(CGroupPtr)bb;
	CGroupPtr groupNew;
        CObjectPtr *Objects;
        long n;

	if(!g)return (CObjectPtr)NULL;

	groupNew=CGroupCreate(g->scene,g->ObjectCount);
	if(!groupNew)return (CObjectPtr)NULL;
	Objects=groupNew->Objects;
	*groupNew = *g;
	groupNew->Objects=Objects;
	for(n=0;n<g->ObjectCount;++n){
	    CObjectPtr Current;
	    if((Current=g->Objects[n]) == NULL)continue;
	    Objects[n]=(*Current->duplicateObject)(Current);
	}
	return (CObjectPtr)groupNew;
}
static CObjectPtr uGroupWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{


	static struct CGroupStruct b;
	
	static struct universalTableListStruct Local[] = {
		{"REAL",4319,universal_TypeDoubleLongPtr,(void *)&b.itemList,0L},				

		{"DOUB",4001,universal_TypeDouble,(void *)&b.Local.p.x},	
		{"DOUB",4002,universal_TypeDouble,(void *)&b.Local.p.y},	
		{"DOUB",4003,universal_TypeDouble,(void *)&b.Local.p.z},	
	
		{"DOUB",4004,universal_TypeDouble,(void *)&b.Local.Theta_x},	
		{"DOUB",4005,universal_TypeDouble,(void *)&b.Local.Theta_y},	
		{"DOUB",4006,universal_TypeDouble,(void *)&b.Local.Theta_z},	
	
		{"DOUB",4007,universal_TypeDouble,(void *)&b.Local.Scale_x},	
		{"DOUB",4008,universal_TypeDouble,(void *)&b.Local.Scale_y},	
		{"DOUB",4009,universal_TypeDouble,(void *)&b.Local.Scale_z},	
	
	};
	
	static struct universalTableStruct Global = {
		"uDraw parameters",8200,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CGroupPtr bp=(CGroupPtr)o;
	char head[5];
	long length;
	long tag;
	void *op;
	int n,nn;
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
	if(!bp){
		/* read and create */
		if(getUniversalHeader(head,&tag,&length,inOut))goto ErrorOut;
		if(mstrcmp(head,"WIND")){
		    goto ErrorOut;
		}
	    Local[0].space=0;
	    b.itemList=NULL;
		if(getUniversalTable(&Global,inOut))goto ErrorOut;		
		nn=(int)Local[0].space;
		if(nn <= 0 || !b.itemList)goto ErrorOut;
		bp=CGroupCreate(l->scene,nn);
		if(!bp)goto  ErrorOut;
		bp->itemList=b.itemList;
	    for(n=0;n<nn;++n){
			CObjectPtr Return,Object=ListGetObjectPtr((int)bp->itemList[n],l);
			if(!Object || !Object->ReadWrite)continue;
	        if((Return = (*Object->ReadWrite)(NULL,inOut,l)) == NULL)goto ErrorOut;
	        bp->Objects[n]=Return;
	    }		
	    if(bp->itemList)cFree((char *)bp->itemList);
	    bp->itemList=NULL;
	       
		Global.tableDynamic=bp;
		Global.tableDynamicSpace=sizeof(*bp);
		if(moveUniversalTable(&Global,universal_MoveToDynamic))goto ErrorOut;
		bp->ItWas= -1;	
		rotate3d(&(bp->Local));	
	    
	    op=bp;
		return (CObjectPtr)op;			
		
	}else{
		if(bp->ObjectCount <= 0)return NULL;
	    if(bp->itemList)cFree((char *)bp->itemList);
	    bp->itemList=(long *)cMalloc(bp->ObjectCount*sizeof(long),8327);
	    if(!bp->itemList)return NULL;
	    
	    nn=0;
	    for(n=0;n<bp->ObjectCount;++n){
	        if(!bp->Objects[n] || !bp->Objects[n]->ReadWrite)continue;
	    	bp->itemList[nn++]=bp->Objects[n]->type;
	    }
	    if(nn <= 0)return NULL;
	    b = *bp;
	    Local[0].space=nn;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut))goto ErrorOut;				
	    for(n=0;n<bp->ObjectCount;++n){
	        if(!bp->Objects[n] || !bp->Objects[n]->ReadWrite)continue;
	        if((*bp->Objects[n]->ReadWrite)(bp->Objects[n],inOut,l) == NULL)goto ErrorOut;
	    }
	    if(bp->itemList)cFree((char *)bp->itemList);
	    bp->itemList=NULL;
	}
	
	op=&b;
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
static int CGroupkill(CObject *o)
{
	CGroupPtr group=(CGroupPtr)o;
	long n;
		
	if(!group)return 1;
	if(group->Objects){
	    for(n=0;n<group->ObjectCount;++n){
	        if(group->Objects[n] && group->Objects[n]->killObject){
	        	(*group->Objects[n]->killObject)(group->Objects[n]);
	        }
	    }
	    cFree((char *)group->Objects);
	    group->Objects=NULL;
	}
	cFree((char *)group);
	
	return 0;
}
CGroupPtr CGroupCreate(struct Scene *scene,long ObjectCount)
{
	CGroupPtr group;
    CObjectPtr *Objects;

	if(!scene || !ObjectCount)return (CGroupPtr)NULL;
	
	
	group=(CGroupPtr)cMalloc(sizeof(CGroup),8595);
	if(!group)return (CGroupPtr)NULL;
	zerol((char *)group,sizeof(CGroup));

	Objects=(CObjectPtr *)cMalloc(ObjectCount*sizeof(CObjectPtr),8596);

	if(!Objects){
	    cFree((char *)group);
	    return (CGroupPtr)NULL;
	}

	zerol((char *)Objects,ObjectCount*sizeof(CObjectPtr));

	CObjectCreate(scene,(CObjectPtr)group);
	group->ObjectCount=ObjectCount;
	group->Objects=Objects;

	group->pGetMost=CObjectGroupMost;
	group->drawObject=CObjectdrawGroup;
	group->duplicateObject=CGroupDuplicate;
	group->countObject=CObjectGroupCount;
	group->message=CObjectGroupMessage;
	group->ReadWrite=uGroupWrite;
	group->type=G_GROUP;
	group->killObject=CGroupkill;
	return group;
}
int CShowMessage(CObject *b,struct ObjectMessage *Message)
{
	CShowPtr s=(CShowPtr)b;

	if(!s || !Message)return 1;

	if(Message->MessageType == OBJECT_MESSAGE_SET_TIME){ 
		CShowLoad(s,Message->time);
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
	    	sprintf(WarningBuff,"  SHOW File\n%c",0);
	    	WarningBatch(WarningBuff);
		    printFileInformation(s->Files);
		}
	}
	return 0;
}
CShowPtr CShowCreate(struct Scene *scene,struct FileInfo2 *Files,int flag)
{
	CShowPtr s;
	struct Block b;

	s=NULL;
	if(!scene || !Files)return (CShowPtr)NULL;

	zerol((char *)&b,sizeof(struct Block));
	if(MergeBlock(scene,&b))return (CShowPtr)NULL;

	s=(CShowPtr)cMalloc(sizeof(CShow),8597);
	if(!s)return (CShowPtr)NULL;
	zerol((char *)s,sizeof(CShow));

	CObjectCreate(scene,(CObjectPtr)s);


	s->type = G_SHOW;
	s->Files=Files;
	s->vscale=1.0;

	s->duplicateObject=CShowDuplicate;
	s->killObject=CShowkill;
	s->message=CShowMessage;
	s->ReadWrite=uShowWrite;	
	s->BlockNumber=scene->BlockCount-1;
	s->ImageCount=Files->ImageCount;
	s->tmax=Files->ImageCount;
	s->Material=getNextMaterial(scene);
	s->xmax = (int)Files->xmax;
	s->ymax = (int)Files->ymax;
	if(flag){
		if(ObjectListAddBlock(scene,s->BlockNumber,s->Material))return (CShowPtr)NULL;
		CShowLoad(s,0L);
	}
	
	s->ItWas = -1;
	
	return s;
}
static CObjectPtr uShowWrite(struct CObjectStruct *o,FILE8 *inOut,CObjectListPtr l)
{
	static struct CShowStruct b;
	
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

		{"REAL",4010,universal_TypeLong,(void *)&b.BlockNumber},
	
		{"REAL",4017,universal_TypeLong,(void *)&b.Material},

		{"DOUB",4018,universal_TypeDouble,(void *)&b.vscale},	

		{"FILE",4034,universal_TypeFilesPtr,(void *)&b.Files},	
			
	};
	
	static struct universalTableStruct Global = {
		"uRender parameters",8201,
		(void *)&b,sizeof(b),
		NULL,0L,							
		Local,sizeof(Local)/sizeof(struct universalTableListStruct),							
	};

	CShowPtr bp=(CShowPtr)o;
	char head[5];
	long length;
	void *op;
	long tag;
	
	
	if(!inOut || !l)return (CObjectPtr)NULL;
	
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
				bp=CShowCreate(l->scene,b.Files,0);
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
		CShowLoad(bp,0L);
		return (CObjectPtr)bp;			
		
	}else{
	    b = *bp;
		if(putUniversalTable((int)Global.tableTag,&Global,inOut)){
		    goto ErrorOut;
		}			
	}
	
	op=&b;
	
	return (CObjectPtr)op;
ErrorOut:
	return (CObjectPtr)NULL;
}
int CShowkill(CObject *o)
{
	CShowPtr s=(CShowPtr)o;
	if(!s)return 1;

	if(s->scene && s->scene->Closing){
	    freeFileList(s->Files);
	}
	cFree((char *)s);
	return 0;
}
CObjectPtr CShowDuplicate(CObject *b)
{
	CShowPtr s=(CShowPtr)b;
	CShowPtr sdsNew;

	if(!s)return (CObjectPtr)NULL;

	sdsNew=(CShowPtr)cMalloc(sizeof(CShow),8598);
	if(!sdsNew)return (CObjectPtr)NULL;

	*sdsNew = *s;

	return (CObjectPtr)sdsNew;
}
int CShowLoad(CShowPtr s,long itIs)
{
	struct Material *Mat;
	struct Scene *scene;
	struct Block *b;
	double vscale;
	long n;

	if(!s)return 1;
	scene=s->scene;
	if(!scene)return 1;


	if(s->ItWas == itIs)return 0;

	b = &(scene->block[s->BlockNumber]);


	if(s->thold >= s->tmax)s->thold=s->tmax-1;
	if(s->thold < 0)s->thold=0;

	if(s->cycleFlag == 0){
	    itIs = s->thold;
	}else if(s->cycleFlag == 1){
	    if(s->tmax > 0)itIs = itIs % s->tmax;
	}else if(s->cycleFlag == 2){
	    blockFree(b);
	    return 0;
	}

	if(s->ItWas == itIs)return 0;

	b->nstress=s->nstress;
	if(FilesGetShow(b,itIs,s->Files))return 1;
	
	Mat=NULL;
	if(!SetMaterialProperty(scene,&Mat,s->Material)){
	    ;
	}

	/* elementint8(b); */ /* Cell Fail Calculations */

	if(s->vscale && b->NodeCount && b->dxyzCount){
	    vscale=s->vscale;
	    for(n=0;n<b->NodeCount;++n){
			b->xdata[n]=(b->xdata[n]+vscale*b->dxdata[n]);
			b->ydata[n]=(b->ydata[n]+vscale*b->dydata[n]);
			b->zdata[n]=(b->zdata[n]+vscale*b->dzdata[n]);
	    }
	}

	elementsplit8(b);            /* Breaks 8 Node into 4 node */

	if(s->averageStress){
	    elementaverage(b);      /* Averages Stress if given at cell centers */
	}else{
	    elementconstant(b);     /* Constant Stress if given at cell centers */
	}
	
	if(!b->vdata && Mat){
		Mat->ColorCount=0;
	}
	

	elementrange(b);

	checkPaletteLevels(scene,s->Material,b->vmin,b->vmax);

	s->ItWas = itIs;

	return 0;
}
int elementsvol(double *xd,double *yd,double *zd,double *volo)
{
	static double  xg[]= {-.5773502691896,.5773502691896};
	static double  wgt[]= {1.,   1.};
	int ir,is,it,i;
	double r,s,t,vol,det;
	double xr,xs,xt,yr,ys,yt,zr,zs,zt;
	double h[8],hr[8],hs[8],ht[8];

	if(!xd || !yd || !zd || !volo)return 1;

	vol=0;
	for(ir=0;ir<2;++ir){
        r=xg[ir];
	    for(is=0;is<2;++is){
            s=xg[is];
	        for(it=0;it<2;++it){
                t=xg[it];
			    gethrst(h,hr,hs,ht,r,s,t);
			    xr=0;
			    xs=0;
			    xt=0;
			    yr=0;
			    ys=0;
			    yt=0;
			    zr=0;
			    zs=0;
			    zt=0;
			    for(i=0;i<8;++i){
					xr=xr+hr[i]*xd[i];
					yr=yr+hr[i]*yd[i];
					zr=zr+hr[i]*zd[i];
					xs=xs+hs[i]*xd[i];
					ys=ys+hs[i]*yd[i];
					zs=zs+hs[i]*zd[i];
					xt=xt+ht[i]*xd[i];
					yt=yt+ht[i]*yd[i];
					zt=zt+ht[i]*zd[i];
			    }
			    det=xr*(ys*zt-zs*yt)-xs*(yr*zt-zr*yt)
			        +xt*(yr*zs-zr*ys);
			    if(det <= 0.0)return 1;
			    vol += wgt[ir]*wgt[is]*wgt[it]*det;
			}
	    }
	}
	*volo=vol;
	return 0;
}
int gethrst(double *h,double *hr,double *hs,double *ht,double r,double s,double t)
{
	double rr,rb,ss,sb,tt,tb;

	if(!h || !hr || !hs || !ht)return 1;

	h  -= 1;
	hr -= 1;
	hs -= 1;
	ht -= 1;

	rr=(1.-r);
	rb=(1.+r);
	ss=(1.-s);
	sb=(1.+s);
	tt=(1.-t);
	tb=(1.+t);
	h[1]=.125*rr*ss*tt;
	h[2]=.125*rb*ss*tt;
	h[3]=.125*rb*sb*tt;
	h[4]=.125*rr*sb*tt;
	h[5]=.125*rr*ss*tb;
	h[6]=.125*rb*ss*tb;
	h[7]=.125*rb*sb*tb;
	h[8]=.125*rr*sb*tb;
	hr[1] = -.125*ss*tt;
	hs[1] = -.125*rr*tt;
	ht[1] = -.125*rr*ss;
	hr[2] =  .125*ss*tt;
	hs[2] = -.125*rb*tt;
	ht[2] = -.125*rb*ss;
	hr[3] =  .125*sb*tt;
	hs[3] =  .125*rb*tt;
	ht[3] = -.125*rb*sb;
	hr[4] = -.125*sb*tt;
	hs[4] =  .125*rr*tt;
	ht[4] = -.125*rr*sb;
	hr[5] = -.125*ss*tb;
	hs[5] = -.125*rr*tb;
	ht[5] =  .125*rr*ss;
	hr[6] =  .125*ss*tb;
	hs[6] = -.125*rb*tb;
	ht[6] =  .125*rb*ss;
	hr[7] =  .125*sb*tb;
	hs[7] =  .125*rb*tb;
	ht[7] =  .125*rb*sb;
	hr[8] = -.125*sb*tb;
	hs[8] =  .125*rr*tb;
	ht[8] =  .125*rr*sb;
	return 0;
}

int elementconstant(struct Block *b)
{
	long NodeCount;
	long n,nn,ncount;

	if(!b)return 1;
	if(!b->stress)return 0;
	if(b->NodesMax != 4)return 1;
	if(!b->Elements)return 1;

	NodeCount=b->NodeCount;

	ncount=0;
	for(n=0;n<b->ElementCount;++n){
	   nn=b->Elements[n*5+1];
	   if(nn >= 0 && nn < NodeCount){
	       ++ncount;
	   }
	   nn=b->Elements[n*5+2];
	   if(nn >= 0 && nn < NodeCount){
	       ++ncount;
	   }
	   nn=b->Elements[n*5+3];
	   if(nn >= 0 && nn < NodeCount){
	       ++ncount;
	   }
	   nn=b->Elements[n*5+4];
	   if(nn >= 0 && nn < NodeCount){
	       ++ncount;
	   }
	}

	if(!ncount)return 0;

	if(!b->vdata){
	    b->vdata=(double *)cMalloc((NodeCount)*sizeof(double),8599);
	    if(!b->vdata)goto ErrorOut;
	    zerol((char *)b->vdata,(NodeCount)*sizeof(double));
	}

	if(elementsplit(b,&b->xdata,ncount))goto ErrorOut;
	if(elementsplit(b,&b->ydata,ncount))goto ErrorOut;
	if(elementsplit(b,&b->zdata,ncount))goto ErrorOut;
	if(elementsplit(b,&b->vdata,ncount))goto ErrorOut;
	if(elementsplit(b,&b->nx,ncount))goto ErrorOut;
	if(elementsplit(b,&b->ny,ncount))goto ErrorOut;
	if(elementsplit(b,&b->nz,ncount))goto ErrorOut;
	if(elementsplit(b,&b->dxdata,ncount))goto ErrorOut;
	if(elementsplit(b,&b->dydata,ncount))goto ErrorOut;
	if(elementsplit(b,&b->dzdata,ncount))goto ErrorOut;
	if(elementsplitF(b,&b->rdata,ncount))goto ErrorOut;
	if(elementsplitF(b,&b->sdata,ncount))goto ErrorOut;

	ncount=0;
	for(n=0;n<b->ElementCount;++n){
	   nn=b->Elements[n*5+1];
	   if(nn >= 0 && nn < NodeCount){
	       b->vdata[NodeCount+ncount]=b->stress[n];
	       b->Elements[n*5+1]=NodeCount+ncount++;
	   }
	   nn=b->Elements[n*5+2];
	   if(nn >= 0 && nn < NodeCount){
	       b->vdata[NodeCount+ncount]=b->stress[n];
	       b->Elements[n*5+2]=NodeCount+ncount++;
	   }
	   nn=b->Elements[n*5+3];
	   if(nn >= 0 && nn < NodeCount){
	       b->vdata[NodeCount+ncount]=b->stress[n];
	       b->Elements[n*5+3]=NodeCount+ncount++;
	   }
	   nn=b->Elements[n*5+4];
	   if(nn >= 0 && nn < NodeCount){
	       b->vdata[NodeCount+ncount]=b->stress[n];
	       b->Elements[n*5+4]=NodeCount+ncount++;
	   }
	}

	b->NodeCount += ncount;

	return 0;
ErrorOut:
	return 1;
}
int elementsplit(struct Block *b,double **vi,long ncount)
{
	long NodeCount;
	long n,nn;
	double *v,*vr;

	if(!b || !vi)return 1;
	if(b->NodesMax != 4)return 1;
	if(!b->Elements)return 1;

	v = *vi;
	if(!v)return 0;

	NodeCount=b->NodeCount;

	vr=(double *)cMalloc((ncount+NodeCount)*sizeof(double),8600);
	if(!vr)return 1;


	ncount=0;
	for(n=0;n<b->ElementCount;++n){
	   nn=b->Elements[n*5+1];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	   nn=b->Elements[n*5+2];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	   nn=b->Elements[n*5+3];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	   nn=b->Elements[n*5+4];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	}

	if(v)cFree((char *)v);

	*vi=vr;

	return 0;
}
int elementsplitF(struct Block *b,float **vi,long ncount)
{
	long NodeCount;
	long n,nn;
	float *v,*vr;

	if(!b || !vi)return 1;
	if(b->NodesMax != 4)return 1;
	if(!b->Elements)return 1;

	v = *vi;
	if(!v)return 0;

	NodeCount=b->NodeCount;

	vr=(float *)cMalloc((ncount+NodeCount)*sizeof(float),8600);
	if(!vr)return 1;


	ncount=0;
	for(n=0;n<b->ElementCount;++n){
	   nn=b->Elements[n*5+1];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	   nn=b->Elements[n*5+2];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	   nn=b->Elements[n*5+3];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	   nn=b->Elements[n*5+4];
	   if(nn >= 0 && nn < NodeCount){
	       vr[NodeCount+ncount++]=v[nn];
	   }
	}

	if(v)cFree((char *)v);

	*vi=vr;

	return 0;
}
int elementaverage(struct Block *b)
{
	long n,nn;
	long *ncount;
	double *stress;

	if(!b)return 1;
	if(!b->stress)return 0;
	if(b->NodesMax != 4)return 1;

 	if((ncount=(long *)cMalloc(sizeof(long)*b->NodeCount,8601))  == NULL){
	    sprintf(WarningBuff,"elementaverage out of memory\n");
		WarningBatch(WarningBuff);
	    return 1;
 	}

 	if((stress=(double *)cMalloc(sizeof(double)*b->NodeCount,8602))  == NULL){
	    sprintf(WarningBuff,"elementaverage out of memory\n");
		WarningBatch(WarningBuff);
	    return 1;
 	}

	for(n=0;n<b->NodeCount;++n){
	    stress[n]=0;
	    ncount[n]=0;
	}

	for(n=0;n<b->ElementCount;++n){
	   nn=b->Elements[n*5+1];
	   if(nn >= 0 && nn < b->NodeCount){
	       stress[nn] += b->stress[n];
	       ++ncount[nn];
	   }
	   nn=b->Elements[n*5+2];
	   if(nn >= 0 && nn < b->NodeCount){
	       stress[nn] += b->stress[n];
	       ++ncount[nn];
	   }
	   nn=b->Elements[n*5+3];
	   if(nn >= 0 && nn < b->NodeCount){
	       stress[nn] += b->stress[n];
	       ++ncount[nn];
	   }
	   nn=b->Elements[n*5+4];
	   if(nn >= 0 && nn < b->NodeCount){
	       stress[nn] += b->stress[n];
	       ++ncount[nn];
	   }
	}
	for(n=0;n<b->NodeCount;++n){
	    if(ncount[n] > 0){
	        stress[n] /= (double)ncount[n];
	    }
	}

	if(b->vdata)cFree((char *)b->vdata);
	b->vdata=stress;

	if(ncount)cFree((char *)ncount);
	return 0;
}
int elementsplit8(struct Block *b)
{
	long n,n8,n4,nel,*Elements,ne;
	double *stress;

	if(!b)return 1;

	if(b->NodesMax != 8)return 0;

	if(!b->Elements)return 1;

	n8=0;
	n4=0;
	for(n=0;n<b->ElementCount;++n){
	   if(b->Elements[n*9+8] >= 0)++n8;
	   if((b->Elements[n*9+4] >= 0) && (b->Elements[n*9+5] < 0))++n4;
	}


	nel=6*n8+n4;

	if(nel == 0)return 0;

	Elements=(long *)cMalloc(nel*5*sizeof(long),8603);
	if(!Elements){
	    sprintf(WarningBuff,"elementsplit8 Out Of Memory Requested (%ld)\n",nel*5*sizeof(long));
		WarningBatch(WarningBuff);
	    return 1;
	}

	stress=NULL;
	if(b->stress){
	    stress=(double *)cMalloc(nel*5*sizeof(double),8604);
	    if(!stress){
	        sprintf(WarningBuff,"elementsplit8 Out Of Memory Requested (%ld)\n",nel*5*sizeof(double));
			WarningBatch(WarningBuff);
	        if(Elements)cFree((char *)Elements);
	        return 1;
	    }
	}

	ne=0;
	for(n=0;n<b->ElementCount;++n){
	   if(b->Elements[n*9+8] >= 0){
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+4];
	       Elements[5*ne+2]=b->Elements[n*9+3];
	       Elements[5*ne+3]=b->Elements[n*9+2];
	       Elements[5*ne+4]=b->Elements[n*9+1];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+5];
	       Elements[5*ne+2]=b->Elements[n*9+6];
	       Elements[5*ne+3]=b->Elements[n*9+7];
	       Elements[5*ne+4]=b->Elements[n*9+8];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+1];
	       Elements[5*ne+2]=b->Elements[n*9+2];
	       Elements[5*ne+3]=b->Elements[n*9+6];
	       Elements[5*ne+4]=b->Elements[n*9+5];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+2];
	       Elements[5*ne+2]=b->Elements[n*9+3];
	       Elements[5*ne+3]=b->Elements[n*9+7];
	       Elements[5*ne+4]=b->Elements[n*9+6];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+3];
	       Elements[5*ne+2]=b->Elements[n*9+4];
	       Elements[5*ne+3]=b->Elements[n*9+8];
	       Elements[5*ne+4]=b->Elements[n*9+7];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+4];
	       Elements[5*ne+2]=b->Elements[n*9+1];
	       Elements[5*ne+3]=b->Elements[n*9+5];
	       Elements[5*ne+4]=b->Elements[n*9+8];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	   } else if((b->Elements[n*9+4] >= 0) && (b->Elements[n*9+5] < 0)){
	       Elements[5*ne]=b->Elements[n*9];
	       Elements[5*ne+1]=b->Elements[n*9+1];
	       Elements[5*ne+2]=b->Elements[n*9+2];
	       Elements[5*ne+3]=b->Elements[n*9+3];
	       Elements[5*ne+4]=b->Elements[n*9+4];
	       if(stress)stress[ne]=b->stress[n];
	       ++ne;
	   }
	}

	if(b->Elements)cFree((char *)b->Elements);
	b->Elements=Elements;
	if(b->stress)cFree((char *)b->stress);
	b->stress=stress;
	b->ElementCount=ne;
	b->StressCount=ne;
	b->NodesMax=4;
	return 0;
}
/*
int elementint8(struct Block *b)
{
	double volmat[200],volfail[200],vol;
	double x[8],y[8],z[8];
	double vmin,vmax;
	long n,nn;
	int k,imat;
	double *stress;

	if(!b)return 1;

	if(b->NodesMax != 8)return 0;

	if(!b->Elements)return 1;

	stress=b->stress;

	for(n=0;n<200;++n){
	    volmat[n]=0;
	    volfail[n]=0;
	}

	vmin = 1e60;
	vmax = -1e60;
	for(n=0;n<b->ElementCount;++n){
	   if(b->Elements[n*9+8] >= 0){
	      imat=(int)(b->Elements[n*9]);
	      if(imat < 0 || imat >= 200)continue;
	       for(k=0;k<8;++k){
	           nn=b->Elements[n*9+1+k];
	           x[k]=b->xdata[nn];
	           y[k]=b->ydata[nn];
	           z[k]=b->zdata[nn];
	       }
		if(elementsvol(x,y,z,&vol))continue;
		volmat[imat] += vol;
		if(stress){
		    if(stress[n] > .1)volfail[imat] += vol;
		    if(stress[n] > vmax)vmax = stress[n];
		    if(stress[n] < vmin)vmin = stress[n];
		}
	   }
	}
	for(n=0;n<200;++n){
	    if(volmat[n] > 0){
	        sprintf(WarningBuff,"mat %ld volume %f fail %f percent %.2f vmin %f vmax %f\n",
	                n,volmat[n],volfail[n],100.0*(volfail[n]/volmat[n]),vmin,vmax);
			WarningBatch(WarningBuff);
	    }
	}
	return 0;
}
*/
int MergeBlock(struct Scene *scene,struct Block *block)
{
         struct Block *b;
         char buff[256];
         unsigned long length;
	
	if(scene->BlockCount){
	    length=(scene->BlockCount+1)*sizeof(struct Block);
	    if(!(b=(struct Block *)cRealloc((char *)scene->block,length,8775))){
	        sprintf(buff,"MergeBlock Out of memory Requested (%ld)\n",length);
			WarningBatch(buff);
	        return 1;
	    }
	    b[scene->BlockCount++] = *block;
	    scene->block = b;
	}else{
	    if(!(b=(struct Block *)cMalloc((long)sizeof(struct Block),8498))){
	        sprintf(buff,"MergeBlock Out of memory Requested (%ld)\n",(long)sizeof(struct Block));
			WarningBatch(buff);
	        return 1;
	    }
	    scene->BlockCount=1;
	    scene->block=b;
	    *b = *block;
	}
	return 0;
}
int dupBlock(struct Block *block,struct Block *blockout)
{
        long NodeCount,ElementCount,NodesMax,*Elements,*Elementso;
        unsigned long length;
        double *xdata,*ydata,*zdata,*vdata;
        double *dxdata,*dydata,*dzdata;
        double *xdatao,*ydatao,*zdatao,*vdatao;
        double *dxdatao,*dydatao,*dzdatao;

        zerol((char *)blockout,sizeof(struct Block));
        blockout->NodeCount=NodeCount=block->NodeCount;
        blockout->ElementCount=ElementCount=block->ElementCount;
        blockout->NodesMax=NodesMax=block->NodesMax;
        Elements=block->Elements;
        xdata=block->xdata;
        ydata=block->ydata;
        zdata=block->zdata;
        dxdata=block->dxdata;
        dydata=block->dydata;
        dzdata=block->dzdata;
        vdata=block->vdata;
        Elementso=NULL;
        xdatao=NULL;
        ydatao=NULL;
        zdatao=NULL;
        dxdatao=NULL;
        dydatao=NULL;
        dzdatao=NULL;
        vdatao=NULL;
        if(Elements){
           length=(ElementCount*(NodesMax+1))*sizeof(long);
           if(!(Elementso=(long *)cMalloc(length,8499))){
                sprintf(WarningBuff,"dupBlock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(Elementso,Elements,length);
        }

        if(xdata){
           length=(NodeCount)*sizeof(double);
           if(!(xdatao=(double *)cMalloc(length,8500))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(xdatao,xdata,length);
        }

        if(ydata){
           length=(NodeCount)*sizeof(double);
           if(!(ydatao=(double *)cMalloc(length,8501))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(ydatao,ydata,length);
        }

        if(zdata){
           length=(NodeCount)*sizeof(double);
           if(!(zdatao=(double *)cMalloc(length,8502))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(zdatao,zdata,length);
        }

        if(dxdata){
           length=(NodeCount)*sizeof(double);
           if(!(dxdatao=(double *)cMalloc(length,8503))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(dxdatao,dxdata,length);
        }

        if(dydata){
           length=(NodeCount)*sizeof(double);
           if(!(dydatao=(double *)cMalloc(length,8504))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(dydatao,dydata,length);
        }

        if(dzdata){
           length=(NodeCount)*sizeof(double);
           if(!(dzdatao=(double *)cMalloc(length,8505))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(dzdatao,dzdata,length);
        }

        if(vdata){
           length=(NodeCount)*sizeof(double);
           if(!(vdatao=(double *)cMalloc(length,8506))){
                sprintf(WarningBuff,"doubleblock out of Memory Requested (%ld)\n",length);
				WarningBatch(WarningBuff);
                return 1;
            }
            memcpy(vdatao,vdata,length);
        }

        blockout->Elements=Elementso;
        blockout->xdata=xdatao;
        blockout->ydata=ydatao;
        blockout->zdata=zdatao;
        blockout->dxdata=dxdatao;
        blockout->dydata=dydatao;
        blockout->dzdata=dzdatao;
        blockout->vdata=vdatao;
        return 0;
}
int elementrange(struct Block *block)
{
	double xmax,xmin,ymax,ymin,zmax,zmin,vmax,vmin,v;
	double *xdata,*ydata,*zdata,*vdata;
	long n,NodesMax,ElementCount,*Elements,nq,m,nn,NodeCount;
	long nerror;

	xdata=block->xdata;
	ydata=block->ydata;
	zdata=block->zdata;
	vdata=block->vdata;
	Elements=block->Elements;
	NodeCount=block->NodeCount;
	if(!xdata || !ydata || !zdata || !Elements)return 1;
	NodesMax=block->NodesMax;
	ElementCount=block->ElementCount;
	xmin =  1e60;
	xmax = -1e60;	
	ymin =  1e60;
	ymax = -1e60;	
	zmin =  1e60;
	zmax = -1e60;	
	vmin =  1e60;
	vmax = -1e60;	
	nerror=0;
	for(n=0;n<ElementCount;++n){
	    nq=(NodesMax+1)*n;
	    for(m=0;m<NodesMax;++m){
	        if((nn=Elements[nq+1+m]) >= 0){
	            if(nn < 0 || nn >= NodeCount){
	                ++nerror;
	                continue;
	            }
			    if(xdata){
					v=xdata[nn];
					if(v > xmax)xmax=v;
					if(v < xmin)xmin=v;
			    }
			    if(ydata){
					v=ydata[nn];
					if(v > ymax)ymax=v;
					if(v < ymin)ymin=v;
			    }
			    if(zdata){
					v=zdata[nn];
					if(v > zmax)zmax=v;
					if(v < zmin)zmin=v;
			    }
			    if(vdata){
					v=vdata[nn];
					if(v > vmax)vmax=v;
					if(v < vmin)vmin=v;
			    }
	        }
	    }

	}
	if(nerror > 0){  
	     sprintf(WarningBuff,"Block Node Errors %ld\n",nerror);
	     WarningBatch(WarningBuff);
	}
	block->xmin=xmin;
	block->ymin=ymin;
	block->zmin=zmin;
	block->vmin=vmin;
	block->xmax=xmax;
	block->ymax=ymax;
	block->zmax=zmax;
	block->vmax=vmax;
		return 0;

}
int PrintSystem(struct System *s,char *name)
{

	if(!s || !name)return 1;
	sprintf(WarningBuff,"%s\n",name);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"x %f %f %f\n",s->x.x,s->x.y,s->x.z);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"y %f %f %f\n",s->y.x,s->y.y,s->y.z);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"z %f %f %f\n",s->z.x,s->z.y,s->z.z);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"p %f %f %f\n",s->p.x,s->p.y,s->p.z);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"sx %f\n",s->Scale_x);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"sy %f\n",s->Scale_y);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"sz %f\n",s->Scale_z);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"tx %f\n",s->Scale_x);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"ty %f\n",s->Scale_y);
	WarningBatch(WarningBuff);
	sprintf(WarningBuff,"tz %f\n",s->Scale_z);
	WarningBatch(WarningBuff);
	return 0;
}
int PrintMatrix(struct Matrix *m,char *name)
{
	int i,j;

	if(!m || !name)return 1;
	sprintf(WarningBuff,"%s\n",name);
	WarningBatch(WarningBuff);

	for(j=0;j<4;++j){
	    for(i=0;i<4;++i){
	        sprintf(WarningBuff,"%f ",m->x[i][j]);
			WarningBatch(WarningBuff);
	    }
	    sprintf(WarningBuff,"\n");
		WarningBatch(WarningBuff);
	}

	return 0;
}

