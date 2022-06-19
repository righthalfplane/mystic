#define EXTERN22 extern
#include <math.h>
#include <ctype.h>
#include "Xappl.h"
#include "TracerDef.h"
#include "Defs.h"
#include "Vrml.h"

#ifdef max
#undef max
#endif


int deScaleComp(struct Scene *scene,unsigned char *bline,unsigned char *bout,long xsize,long ysize);

char *vrmlTypeNames[25]={
		       "T_VRML_EAT",
		       "T_VRML_Separator",
		       "T_VRML_TransformSeparator",
		       "T_VRML_Group",
		       "T_VRML_Rotation",
		       "T_VRML_Translation",
		       "T_VRML_Scale",
		       "T_VRML_MaterialBinding",
		       "T_VRML_Material",
		       "T_VRML_MatrixTransform",
		       "T_VRML_Coordinate3",
		       "T_VRML_IndexedFaceSet",
		       "T_VRML_Normal",
		       "T_VRML_NormalBinding",
		       "T_VRML_Use",
		       "T_VRML_Transform",
		       "T_VRML_Sphere",
		       "T_VRML_Cylinder",
		       "T_VRML_Cube",
		       "T_VRML_Cone",
		       "T_VRML_Switch",
		       "T_VRML_IndexedLineSet",
		       "T_VRML_TextureCoordinate2",
		       "T_VRML_Texture2Transform",
		       "T_VRML_Texture2",
		       };


#define T_DEF		1000
#define T_STRING	1001
#define T_NAME		1002
#define T_NUMBER	1003

#define POINTS_INCREMENT	3000


#define BUFF_SIZE	2048

struct Parser{
    char buff[BUFF_SIZE];
    char *FileName;
    FILE *in;
    int c;
    struct vrmlNamelist *list;
    long nameCount;
    long nameMax;
    long MostNodes;
    long MostNormals;
    long MostCells;
};

struct allocate{
	long *textdata;
	long text;
	long textmax;
};

struct fallocate{
	double *pointdata;
	long points;
	long pointsmax;
};

int getLine(char *Line,int length,FILE *in);

int next(struct Parser *p);

int white(struct Parser *p);

int vrmlNumber(int c,struct Parser *p);

int vrmlName(int c,struct Parser *p);

int ishex(int a);

int EatIt(struct Parser *p,int flag);

struct vrmlObjects *getObjects(int c,char *name,struct Parser *p);

struct vrmlObjects *Separator(struct Parser *p,int type);

struct vrmlObjects *Rotation(struct Parser *p);

struct vrmlObjects *Translation(struct Parser *p);

struct vrmlObjects *Scale(struct Parser *p);

struct vrmlObjects *MaterialBinding(struct Parser *p);

struct vrmlObjects *Material(struct Parser *p);

struct vrmlObjects *MatrixTransform(struct Parser *p);

struct vrmlObjects *Coordinate3(struct Parser *p);

struct vrmlObjects *IndexedFaceSet(struct Parser *p);

struct vrmlObjects *Normal(struct Parser *p);

struct vrmlObjects *NormalBinding(struct Parser *p);

struct vrmlObjects *Use(struct Parser *p);

static struct vrmlObjects *Transform(struct Parser *p);

static struct vrmlObjects *Sphere(struct Parser *p);

static struct vrmlObjects *Cylinder(struct Parser *p);

static struct vrmlObjects *Cube(struct Parser *p);

static struct vrmlObjects *Cone(struct Parser *p);

static struct vrmlObjects *IndexedLineSet(struct Parser *p);

struct vrmlObjects *TextureCoordinate2(struct Parser *p);

static struct vrmlObjects *Texture2Transform(struct Parser *p);

static struct vrmlObjects *Texture2(struct Parser *p);

static int kSeparator(struct vrmlObjects *vrml);

static int kObject(struct vrmlObjects *vrml);

static int kMaterial(struct vrmlObjects *vrml);

static int kCoordinate3(struct vrmlObjects *vrml);

int check2(struct checker *r,long length);

int checkfix(struct checker *r,long length);

int putObjectName(char *name,struct vrmlObjects *vrml,struct Parser *p);

int freeVrmlObjectList(struct vrmlObjects *vrml);

struct vrmlList *GetVRML(char *FilesNames,char *directory)
{
	struct vrmlList *list;
	struct vrmlObjects *vrml;
	static char *vrmlhead="#VRML V1.0";
	char buff[256],Line[256],name[256];
	struct Parser p;
	FILE *in;
	long n;
	int c;

	in=NULL;
	vrml=NULL;
	list=NULL;

	if(!FilesNames || !directory)return list;

	zerol((char *)&p,sizeof(struct Parser));

	*buff=0;
	if(directory){
	    mstrncpy(buff,directory,255);
	}
	
	strncatToPath(buff,FilesNames,255);

	in=fopen(buff,"rb");
	if(in == NULL){
	   	sprintf(WarningBuff,"VRMLdata Could Not Open %s To Read\n",buff);
		WarningBatch(WarningBuff);
	   	goto ErrorOut;
	}

	p.in=in;
	p.FileName=FilesNames;

	if(getLine(Line,256,in))goto ErrorOut;
	Line[10]=0;
	if(strcmp(Line,vrmlhead)){
	   	sprintf(WarningBuff,"VRMLdata %s is not a VRML 1.0 File\n",buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	p.MostNodes=0;
	p.MostNormals=0;
	p.MostCells=0;

	list=(struct vrmlList *)cMalloc(sizeof(struct vrmlList),8661);
	if(!list)goto ErrorOut;
	zerol((char *)list,sizeof(struct vrmlList));

	c=next(&p);
	vrml=getObjects(c,name,&p);
	if(!vrml)goto ErrorOut;


	list->vrml=vrml;
	vrml=NULL;

	list->list=p.list;
	p.list=NULL;

	list->nameCount=p.nameCount;
	list->nameMax=p.nameMax;
	list->MostNodes=p.MostNodes;
	list->MostNormals=p.MostNormals;
	list->MostCells=p.MostCells;

/*
	sprintf(WarningBuff,"GetVRML Ok %s MostNodes %ld MostNormals %ld MostCells %ld\n",
	       FilesNames,list->MostNodes,list->MostNormals,list->MostCells);
	WarningBatch(WarningBuff);
*/


ErrorOut:
	if(in)fclose(in);

	if(p.list){
	    for(n=0;n<p.nameCount;++n){
	       if(p.list[n].name){
	           cFree((char *)p.list[n].name);
	       }
	    }
	    cFree((char *)p.list);
	}
	return list;
}
struct vrmlList *duplicateVRML(struct vrmlList *listin)
{
	struct vrmlList *list;
	long n;

	list=NULL;
	if(!listin)return list;


	list=(struct vrmlList *)cMalloc(sizeof(struct vrmlList),8662);
	if(!list)goto ErrorOut;
	zerol((char *)list,sizeof(struct vrmlList));

	list->nameCount=listin->nameCount;
	list->nameMax=listin->nameCount;
	list->MostNodes=listin->MostNodes;
	list->MostNormals=listin->MostNormals;
	list->MostCells=listin->MostCells;

	if(list->nameCount > 0){
	    list->list=(struct vrmlNamelist *)cMalloc(list->nameCount*sizeof(struct vrmlNamelist),8663);
	    if(!list->list)goto ErrorOut;
	    zerol((char *)list->list,list->nameCount*sizeof(struct vrmlNamelist));
	    for(n=0;n<list->nameCount;++n){
	        list->list[n].objectNumber=listin->list[n].objectNumber;
	    }
	}

	if(listin->vrml){
	    if(listin->vrml->duplicateObject){
	        list->vrml=(*(listin->vrml->duplicateObject))(listin->vrml);
	        if(!list->vrml)goto ErrorOut;
	    }
	}
	return list;
ErrorOut:
	sprintf(WarningBuff,"Error in duplicateVRML\n");
	WarningBatch(WarningBuff);
	freeVrmlList(list);
	list=NULL;
	return list;
}
int freeVrmlList(struct vrmlList *list)
{
	long n;

	if(!list)return 1;
	if(list->list){
	    for(n=0;n<list->nameCount;++n){
	       if(list->list[n].name){
	           cFree((char *)list->list[n].name);
	       }
	    }
	    cFree((char *)list->list);
	}
	freeVrmlObjectList(list->vrml);
	return 0;
}
struct vrmlObjects *getObjects(int c,char *name,struct Parser *p)
{
	struct vrmlObjects *vrml;
	static struct vrmlObjects vrmlEat;
	char command[256];

	vrml=NULL;
	if(!name || !p || !p->in)return vrml;

	name[0]='\0';

	if(c == EOF)return vrml;

	zerol((char *)&vrmlEat,sizeof(struct vrmlObjects));
	vrmlEat.type = T_VRML_EAT;


	    if(c == '}'){
	        return vrml;
	    }
	    if(c != T_NAME){
	        sprintf(WarningBuff,"Parse Error %d %c No NODE name\n",c,c);
			WarningBatch(WarningBuff);
	        return vrml;
	    }
	    if(!strcmp(p->buff,"DEF")){
	       c=next(p);
	       if(c != T_NAME && c != T_STRING){
	           	sprintf(WarningBuff,"Parse Error %d %c looking for DEF name\n",c,c);
				WarningBatch(WarningBuff);
	           	return vrml;
	        }
	        mstrncpy(name,p->buff,255);
	        c=next(p);
	        if(c != T_NAME){
	            sprintf(WarningBuff,"Parse Error %d %c looking for DEF NODE\n",c,c);
				WarningBatch(WarningBuff);
	            return vrml;
	         }
	    } else if(!strcmp(p->buff,"USE")){
	        c=next(p);
	        if(c != T_NAME){
	            sprintf(WarningBuff,"Parse Error %d %c looking for USE NODENANE\n",c,c);
				WarningBatch(WarningBuff);
	            return vrml;
	         }
	         return Use(p);
	    }
	    mstrncpy(command,p->buff,255);
	    c=next(p);
	    if(c != '{'){
	        sprintf(WarningBuff,"Error Parse looking for '{' after command %s\n",command);
			WarningBatch(WarningBuff);
	        return vrml;
	    }
	    if(!strcmp(command,"Separator")){
			return Separator(p,T_VRML_Separator);
	    }else if(!strcmp(command,"TransformSeparator")){
			return Separator(p,T_VRML_TransformSeparator);
	    }else if(!strcmp(command,"Group")){
			return Separator(p,T_VRML_Group);
	    }else if(!strcmp(command,"Switch")){
			return Separator(p,T_VRML_Switch);
	    }else if(!strcmp(command,"Rotation")){
	        return Rotation(p);
	    }else if(!strcmp(command,"Scale")){
	        return Scale(p);
	    }else if(!strcmp(command,"Translation")){
	        return Translation(p);
	    }else if(!strcmp(command,"Coordinate3")){
	        return Coordinate3(p);
	    }else if(!strcmp(command,"IndexedFaceSet")){
	        return IndexedFaceSet(p);
	    }else if(!strcmp(command,"MaterialBinding")){
	        return MaterialBinding(p);
	    }else if(!strcmp(command,"NormalBinding")){
	        return NormalBinding(p);
	    }else if(!strcmp(command,"MatrixTransform")){
	        return MatrixTransform(p);
	    }else if(!strcmp(command,"Material")){
	        return Material(p);
	    }else if(!strcmp(command,"Normal")){
	        return Normal(p);
	    }else if(!strcmp(command,"Transform")){
	        return Transform(p);
  	    }else if(!strcmp(command,"Sphere")){
	        return Sphere(p);
  	    }else if(!strcmp(command,"Cylinder")){
	        return Cylinder(p);
  	    }else if(!strcmp(command,"Cube")){
	        return Cube(p);
  	    }else if(!strcmp(command,"Cone")){
	        return Cone(p);
	    }else if(!strcmp(command,"IndexedLineSet")){
	        return IndexedLineSet(p);
	    }else if(!strcmp(command,"TextureCoordinate2")){
	        return TextureCoordinate2(p);
 	    }else if(!strcmp(command,"Texture2Transform")){
	        return Texture2Transform(p);
 	    }else if(!strcmp(command,"Texture2")){
	        return Texture2(p);
	    }else if(!strcmp(command,"WWWAnchor")){
			return Separator(p,T_VRML_Separator);
  /*   Kill unneeded Commands  */
	    }else if(!strcmp(command,"ShapeHints")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"SpotLight")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"OrthographicCamera")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"PerspectiveCamera")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"DirectionalLight")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"Info")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"PointLight")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }else if(!strcmp(command,"Label")){
	        if(EatIt(p,0))return vrml;
			return &vrmlEat;
	    }
	    if(EatIt(p,1))return vrml;
	    return &vrmlEat;
}
static int kTexture2(struct vrmlObjects *vrml)
{
	struct vrmlTexture2 *r;

	if(!vrml)return 1;
	r=(struct vrmlTexture2 *)vrml;

	if(r->filename)cFree((char *)r->filename);
	r->filename=NULL;

	if(r->image)cFree((char *)r->image);
	r->image=NULL;

	cFree((char *)r);
	return 0;
}
static struct vrmlObjects *dTexture2(struct vrmlObjects *vrml)
{
	struct vrmlTexture2 *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlTexture2 *)vrml;

	vrml=NULL;

	r1=(struct vrmlTexture2 *)cMalloc(sizeof(struct vrmlTexture2),8664);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlTexture2));

	*r1 = *r2;

	r1->filename=NULL;
	r1->image=NULL;

	if(r2->filename){
	    r1->filename=strsave(r2->filename,8829);
	    if(!r1->filename)goto ErrorOut;
	}
	
	if(r2->image && r2->xsize && r2->ysize){
	    r1->image=(long *)cMalloc(r2->xsize*r2->ysize*sizeof(long),8665);
	    if(!r1->image)goto ErrorOut;
	    for(n=0;n<r2->xsize*r2->ysize;++n){
	       r1->image[n]=r2->image[n];
	    }
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dTexture2 Error\n");
	WarningBatch(WarningBuff);
	kTexture2((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
/*
static int writeTexture(struct vrmlTexture2 *r)
{
	unsigned char *image,*buff;
	struct Scene scene;
	char name[256];
	long length,n;
	static long count;

	if(!r)return 1;
	if(!r->image)return 0;
	image=NULL;
	buff=NULL;
	length=(long)r->xsize*r->ysize;
	buff=(unsigned char *)cMalloc(length*3,8666);
	image=(unsigned char *)cMalloc(length*3,8667);
	if(!buff || !image)goto Error;
	sprintf(name,"texture%04ld.tru",count++);
	scene.FileOut=name;
	for(n=0;n<length;++n){
	   image[3*n]=(unsigned char)(r->image[n] >> 16);
	   image[3*n+1]=(unsigned char)(r->image[n] >> 8);
	   image[3*n+2]=(unsigned char)(r->image[n]);
	}
	if(deScaleComp(&scene,image,buff,r->xsize,r->ysize)){
	    goto Error;
	}
Error:
	if(buff)cFree((char *)buff);
	if(image)cFree((char *)image);
	return 0;
}
*/
static struct vrmlObjects *Texture2(struct Parser *p)
{
	struct vrmlTexture2 *r;
	struct vrmlObjects *vrml;
	char command[256];
	long lvalue[3],count;
	int n,pass;
	int c;


	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlTexture2 *)cMalloc(sizeof(struct vrmlTexture2),8668);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlTexture2));

	r->type=T_VRML_Texture2;
	r->killObject=kTexture2;
	r->duplicateObject=dTexture2;


	r->filename = NULL;
	r->image = NULL;

	r->xsize = 0;
	r->ysize = 0;
	r->componets = 0;
	r->wrapS = T_VRML_REPEAT;
	r->wrapT = T_VRML_REPEAT;

	pass=0;

	while(1){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '}'){
	        if(pass == 0){
	            return (struct vrmlObjects *)r;
	        }else{
	            break;
	        }
	    }
	    mstrncpy(command,p->buff,255);
	    pass=1;
	    if(!strcmp(command,"filename")){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c != T_STRING){
	           	sprintf(WarningBuff,"filename: Texture2 Expecting String got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
		    r->filename=strsave(p->buff,8830);
	    }else if(!strcmp(command,"model")){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NAME){
	           	sprintf(WarningBuff,"model: Texture2 Expecting name got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	    }else if(!strcmp(command,"wrapS")){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NAME){
	           	sprintf(WarningBuff,"wrapS: Texture2 Expecting name got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        if(!strcmp(p->buff,"CLAMP")){
		    r->wrapS = T_VRML_CLAMP;
	        }else{
		    r->wrapS = T_VRML_REPEAT;
	        }
	    }else if(!strcmp(command,"blendColor")){
	        c=next(p);
	        c=next(p);
	        c=next(p);	    
	    }else if(!strcmp(command,"wrapT")){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NAME){
	           	sprintf(WarningBuff,"wrapT: Texture2 Expecting name got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        if(!strcmp(p->buff,"CLAMP")){
		    r->wrapT = T_VRML_CLAMP;
	        }else{
		    r->wrapT = T_VRML_REPEAT;
	        }
	    }else if(!strcmp(command,"image")){
	        for(n=0;n<3;++n){
	            c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c != T_NUMBER){
	               	sprintf(WarningBuff,"image: Texture2 Expecting NUMBER got %d %c\n",c,c);
					WarningBatch(WarningBuff);
	               	goto ErrorOut;
	            }
		    sscanf(p->buff,"%ld",&lvalue[n]);
	        }
			r->xsize = lvalue[0];
			r->ysize = lvalue[1];
			r->componets = lvalue[2];
			count=r->xsize*r->ysize;
			r->image=(long *)cMalloc(count*sizeof(long),8669);
			if(!r->image){
		            sprintf(WarningBuff,"Texture2 out of memory Requested %ld\n",count*sizeof(long));
					WarningBatch(WarningBuff);
			}
	        for(n=0;n<count;++n){
	            c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c != T_NUMBER){
	               	sprintf(WarningBuff,"Error Texture2 Expecting NUMBER got %d %c\n",c,c);
					WarningBatch(WarningBuff);
	               	goto ErrorOut;
	            }
		    	sscanf(p->buff,"%lx",&r->image[n]);
	        }
	    }else{
	        sprintf(WarningBuff,"Texture2 UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	}
/*
	writeTexture(r);
*/
	return (struct vrmlObjects *)r;
ErrorOut:
	kTexture2((struct vrmlObjects *)r);
	return vrml;
}
static struct vrmlObjects *dTexture2Transform(struct vrmlObjects *vrml)
{
	struct vrmlTexture2Transform *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlTexture2Transform *)vrml;

	s1=(struct vrmlTexture2Transform *)cMalloc(sizeof(struct vrmlTexture2Transform),8670);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dTexture2Transform\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *Texture2Transform(struct Parser *p)
{
	struct vrmlTexture2Transform *r;
	struct vrmlObjects *vrml;
	double *a[4];
	static int inc[4]={2,1,2,2};
	double value;
	int n,type,pass;
	int c;


	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlTexture2Transform *)cMalloc(sizeof(struct vrmlTexture2Transform),8671);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlTexture2Transform));

	r->type=T_VRML_Texture2Transform;
	r->killObject=kObject;
	r->duplicateObject=dTexture2Transform;

	a[0]= (double *)&r->translation;
	a[1]= (double *)&r->rotation;
	a[2]= (double *)&r->scaleFactor;
	a[3]= (double *)&r->center;


	r->translation[0] = 0;
	r->translation[1] = 0;

	r->rotation[0] = 0;

	r->scaleFactor[0] = 1;
	r->scaleFactor[1] = 1;

	r->center[0] = 0;
	r->center[1] = 0;

	pass=0;

	while(1){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '}'){
	        if(pass == 0){
	            return (struct vrmlObjects *)r;
	        }else{
	            break;
	        }
	    }
	    pass=1;
	    if(!strcmp(p->buff,"translation")){
	       type=0;
	    }else if(!strcmp(p->buff,"rotation")){
	       type=1;
	    }else if(!strcmp(p->buff,"scaleFactor")){
	       type=2;
	    }else if(!strcmp(p->buff,"center")){
	       type=3;
	    }else{
	        sprintf(WarningBuff,"Texture2Transform UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    for(n=0;n<inc[type];++n){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Texture2Transform Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        sscanf(p->buff,"%lf",&value);
		a[type][n]=value;
	    }
	}
	return (struct vrmlObjects *)r;
ErrorOut:
	kObject((struct vrmlObjects *)r);
	return vrml;
}

static int kTextureCoordinate2(struct vrmlObjects *vrml)
{
	struct vrmlTextureCoordinate2 *r;

	if(!vrml)return 1;
	r=(struct vrmlTextureCoordinate2 *)vrml;

	if(r->pointdata)cFree((char *)r->pointdata);
	r->pointdata=NULL;

	cFree((char *)r);
	return 0;
}
static struct vrmlObjects *dTextureCoordinate2(struct vrmlObjects *vrml)
{
	struct vrmlTextureCoordinate2 *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlTextureCoordinate2 *)vrml;

	vrml=NULL;

	r1=(struct vrmlTextureCoordinate2 *)cMalloc(sizeof(struct vrmlTextureCoordinate2),8672);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlTextureCoordinate2));

	*r1 = *r2;

	r1->pointdata=NULL;

	r1->points=0;

	if(r2->pointdata && r2->points){
	    r1->pointdata=(double *)cMalloc(r2->points*sizeof(double),8673);
	    if(!r1->pointdata)goto ErrorOut;
	    for(n=0;n<r2->points;++n){
	        r1->pointdata[n]=r2->pointdata[n];
	    }
	    r1->points=r2->points;
	    r1->pointsmax=r1->points;
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dTextureCoordinate2 Error\n");
	WarningBatch(WarningBuff);
	kTextureCoordinate2((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *TextureCoordinate2(struct Parser *p)
{
	struct vrmlTextureCoordinate2 *r;
	struct vrmlObjects *vrml;
	struct checker *a;
	double value;
	void *op;
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlTextureCoordinate2 *)cMalloc(sizeof(struct vrmlTextureCoordinate2),8674);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlTextureCoordinate2));

	r->type=T_VRML_TextureCoordinate2;
	r->killObject=kTextureCoordinate2;
	r->duplicateObject=dTextureCoordinate2;

	op=&r->pointdata;
	a=op;

	if(check2(a,sizeof(double)))goto ErrorOut;

	r->pointdata[0] = 0.0;
	r->pointdata[1] = 0.0;

	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c == '}'){
	    r->points = 2;
	    goto Ok;
	}
	if(strcmp(p->buff,"point")){
	    sprintf(WarningBuff,"TextureCoordinate2 UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c == '['){
	    while(1){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c == ']')break;
	        if(c == ',')continue;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error TextureCoordinate2 Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
		if(check2(a,sizeof(double)))goto ErrorOut;
	        sscanf(p->buff,"%lf",&value);
		r->pointdata[r->points++]=value;
	    }
	}else{
	    for(n=0;n<2;++n){
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error TextureCoordinate2 Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
		if(check2(a,sizeof(double)))goto ErrorOut;
	        sscanf(p->buff,"%lf",&value);
		r->pointdata[r->points++]=value;
	        c=next(p);
	    }
	    goto Next;
	}
	c=next(p);
Next:
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error TextureCoordinate2 Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
Ok:
	if(checkfix(a,sizeof(double)))goto ErrorOut;
	return (struct vrmlObjects *)r;
ErrorOut:
	kTextureCoordinate2((struct vrmlObjects *)r);
	return vrml;
}

static int kIndexedLineSet(struct vrmlObjects *vrml)
{
	struct vrmlIndexedLineSet *r;

	if(!vrml)return 1;
	r=(struct vrmlIndexedLineSet *)vrml;

	if(r->coorddata)cFree((char *)r->coorddata);
	r->coorddata=NULL;

	if(r->matdata)cFree((char *)r->matdata);
	r->matdata=NULL;

	if(r->normdata)cFree((char *)r->normdata);
	r->normdata=NULL;

	if(r->textdata)cFree((char *)r->textdata);
	r->textdata=NULL;

	cFree((char *)r);
	return 0;
}

static struct vrmlObjects *dIndexedLineSet(struct vrmlObjects *vrml)
{
	struct vrmlIndexedLineSet *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlIndexedLineSet *)vrml;

	vrml=NULL;

	r1=(struct vrmlIndexedLineSet *)cMalloc(sizeof(struct vrmlIndexedLineSet),8675);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlIndexedLineSet));

	*r1 = *r2;

	r1->coorddata=NULL;
	r1->matdata=NULL;
	r1->normdata=NULL;
	r1->textdata=NULL;

	r1->coord=0;
	r1->mat=0;
	r1->norm=0;
	r1->text=0;

	if(r2->coorddata && r2->coord){
	    r1->coorddata=(long *)cMalloc(r2->coord*sizeof(long),8676);
	    if(!r1->coorddata)goto ErrorOut;
	    for(n=0;n<r2->coord;++n){
	        r1->coorddata[n]=r2->coorddata[n];
	    }
	    r1->coord=r2->coord;
	    r1->coordmax=r1->coord;
	}

	if(r2->matdata && r2->mat){
	    r1->matdata=(long *)cMalloc(r2->mat*sizeof(long),8677);
	    if(!r1->matdata)goto ErrorOut;
	    for(n=0;n<r2->mat;++n){
	        r1->matdata[n]=r2->matdata[n];
	    }
	    r1->mat=r2->mat;
	    r1->matmax=r1->mat;
	}

	if(r2->normdata && r2->norm){
	    r1->normdata=(long *)cMalloc(r2->norm*sizeof(long),8678);
	    if(!r1->normdata)goto ErrorOut;
	    for(n=0;n<r2->norm;++n){
	        r1->normdata[n]=r2->normdata[n];
	    }
	    r1->norm=r2->norm;
	    r1->normmax=r1->norm;
	}

	if(r2->textdata && r2->text){
	    r1->textdata=(long *)cMalloc(r2->text*sizeof(long),8679);
	    if(!r1->textdata)goto ErrorOut;
	    for(n=0;n<r2->text;++n){
	        r1->textdata[n]=r2->textdata[n];
	    }
	    r1->text=r2->text;
	    r1->textmax=r1->text;
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dIndexedLineSet Error\n");
	WarningBatch(WarningBuff);
	kIndexedLineSet((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *IndexedLineSet(struct Parser *p)
{
	struct vrmlIndexedLineSet *r;
	struct vrmlObjects *vrml;
	struct allocate *a[4];
	char *bp;
	long lvalue;
	int n,type,pass;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlIndexedLineSet *)cMalloc(sizeof(struct vrmlIndexedLineSet),8680);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlIndexedLineSet));

	r->type=T_VRML_IndexedLineSet;
	r->killObject=kIndexedLineSet;
	r->duplicateObject=dIndexedLineSet;

	a[0]=(struct allocate *)&r->coorddata;
	a[1]=(struct allocate *)&r->matdata;
	a[2]=(struct allocate *)&r->normdata;
	a[3]=(struct allocate *)&r->textdata;

	for(n=0;n<4;++n){
	    if(check2((struct checker *)a[n],sizeof(long)))goto ErrorOut;
	}

	r->coorddata[0] = 0;
	r->matdata[0] =  -1;
	r->normdata[0] = -1;
	r->textdata[0] = -1;
	pass=0;

	while(1){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '}'){
	        if(pass == 0){
	            ++r->coord;
	            goto Ok;
	        }else{
	            break;
	        }
	    }
	    pass=1;
	    if(!strcmp(p->buff,"coordIndex")){
	       type=0;
	    }else if(!strcmp(p->buff,"materialIndex")){
	       type=1;
	    }else if(!strcmp(p->buff,"normalIndex")){
	       type=2;
	    }else if(!strcmp(p->buff,"textureCoordIndex")){
	       type=3;
	    }else{
	        sprintf(WarningBuff,"IndexedLineSet UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '['){
	        while(1){
	            c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c == ']')break;
	            if(c == ',')continue;
	            if(c != T_NUMBER){
	               	sprintf(WarningBuff,"Error IndexedLineSet Expecting Number got %d %c\n",c,c);
					WarningBatch(WarningBuff);
	               	goto ErrorOut;
	            }
		    if(check2((struct checker *)a[type],sizeof(long)))goto ErrorOut;
		    bp=p->buff;
		    if(*bp == '-' || *bp == '+'){
		        if(bp[1] == '0' && (bp[2] == 'X' || bp[2] == 'x' )){
	                    sscanf(bp,"%lx",&lvalue);
		        }else{
	                    sscanf(bp,"%ld",&lvalue);
		        }
		    }else if(*bp == '0' && (bp[1] == 'X' || bp[1] == 'x' )){
	                sscanf(bp,"%lx",&lvalue);
		    }else{
	                sscanf(bp,"%ld",&lvalue);
	            }
		    a[type]->textdata[a[type]->text++]=lvalue;
	        }
	    }else{
	        for(n=0;n<1;++n){
	            if(n != 0)c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c != T_NUMBER){
	               	sprintf(WarningBuff,"Error IndexedLineSet Expecting Number got %d %c\n",c,c);
					WarningBatch(WarningBuff);
	               	goto ErrorOut;
	            }
		    if(check2((struct checker *)a[type],sizeof(long)))goto ErrorOut;
		    bp=p->buff;
		    if(*bp == '-' || *bp == '+'){
		        if(bp[1] == '0' && (bp[2] == 'X' || bp[2] == 'x' )){
	                    sscanf(bp,"%lx",&lvalue);
		        }else{
	                    sscanf(bp,"%ld",&lvalue);
		        }
		    }else if(*bp == '0' && (bp[1] == 'X' || bp[1] == 'x' )){
	                sscanf(bp,"%lx",&lvalue);
		    }else{
	                sscanf(bp,"%ld",&lvalue);
	            }
		    a[type]->textdata[a[type]->text++]=lvalue;	            
	        }
	    }
	}
Ok:
	for(n=0;n<4;++n){
	    if(checkfix((struct checker *)a[n],sizeof(long)))goto ErrorOut;
	}
	return (struct vrmlObjects *)r;
ErrorOut:
	kIndexedLineSet((struct vrmlObjects *)r);
	return vrml;
}

static struct vrmlObjects *dCone(struct vrmlObjects *vrml)
{
	struct vrmlCone *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlCone *)vrml;

	s1=(struct vrmlCone *)cMalloc(sizeof(struct vrmlCone),8681);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dCone\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *Cone(struct Parser *p)
{
	struct vrmlCone *m;
	struct vrmlObjects *vrml;
	double value;
	int c,type;

	vrml=NULL;
	if(!p)return vrml;

	m=(struct vrmlCone *)cMalloc(sizeof(struct vrmlCone),8682);
	if(!m)return vrml;
	zerol((char *)m,sizeof(struct vrmlCone));

	m->type=T_VRML_Cone;
	m->killObject=kObject;
	m->duplicateObject=dCone;

	m->bottomRadius = 1.0;
	m->height = 2.0;
	m->parts = T_VRML_ALL;

	while(1){
	    c=next(p);
	    if(c == '}'){
	        break;
	    }
	    if(!strcmp(p->buff,"bottomRadius")){
	        type = 0;
	    }else if(!strcmp(p->buff,"height")){
	        type = 1;
	    }else if(!strcmp(p->buff,"parts")){
	        type = 2;
	    }else {
	        sprintf(WarningBuff,"Cone UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(type == 0){
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Cone Expecting number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        sscanf(p->buff,"%lf",&value);
	        m->bottomRadius = value;
	    }else if(type == 1){
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Cone Expecting number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        sscanf(p->buff,"%lf",&value);
	        m->height = value;
	    }else{
	        if(c != T_NAME){
	           	sprintf(WarningBuff,"Error Cone Expecting name got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        c=T_VRML_ALL;

	        if(!strcmp(p->buff,"BOTTOM")){
	           c=T_VRML_BOTTOM;
	        }else if(!strcmp(p->buff,"SIDES")){
	           c=T_VRML_SIDES;
	        }else if(!strcmp(p->buff,"ALL")){
	        	c=T_VRML_ALL;
	        }
	        m->parts = c;
	    }
	}
	if(p->MostNodes < 2*400)p->MostNodes=2*400;
	if(p->MostCells < 2*400)p->MostCells=2*400;
	if(p->MostNormals < 2*400)p->MostNormals=2*400;
	return (struct vrmlObjects *)m;
ErrorOut:
	kObject((struct vrmlObjects *)m);
	return vrml;
}
static struct vrmlObjects *dCube(struct vrmlObjects *vrml)
{
	struct vrmlCube *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlCube *)vrml;

	s1=(struct vrmlCube *)cMalloc(sizeof(struct vrmlCube),8683);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dCube\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *Cube(struct Parser *p)
{
	struct vrmlCube *m;
	struct vrmlObjects *vrml;
	double value;
	double *fp;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	m=(struct vrmlCube *)cMalloc(sizeof(struct vrmlCube),8684);
	if(!m)return vrml;
	zerol((char *)m,sizeof(struct vrmlCube));

	m->type=T_VRML_Cube;
	m->killObject=kObject;
	m->duplicateObject=dCube;

	m->width = 2.0;
	m->height = 2.0;
	m->depth = 2.0;

	while(1){
	    c=next(p);
	    if(c == '}'){
	        break;
	    }
	    if(!strcmp(p->buff,"width")){
			fp = &m->width;
	    }else if(!strcmp(p->buff,"height")){
			fp = &m->height;
	    }else if(!strcmp(p->buff,"depth")){
			fp = &m->depth;
	    }else {
	        sprintf(WarningBuff,"Cube UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    c=next(p);
	    if(c == EOF)goto ErrorOut;

	    if(c != T_NUMBER){
	       	sprintf(WarningBuff,"Error Cube Expecting number got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	    }
	    sscanf(p->buff,"%lf",&value);
	    *fp = value;
	}
	if(p->MostNodes < 24*2)p->MostNodes=24*2;
	if(p->MostCells < 6*2)p->MostCells=6*2;
	if(p->MostNormals < 2*24)p->MostNormals=2*24;
	return (struct vrmlObjects *)m;
ErrorOut:
	kObject((struct vrmlObjects *)m);
	return vrml;
}

static struct vrmlObjects *dCylinder(struct vrmlObjects *vrml)
{
	struct vrmlCylinder *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlCylinder *)vrml;

	s1=(struct vrmlCylinder *)cMalloc(sizeof(struct vrmlCylinder),8685);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dCylinder\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *Cylinder(struct Parser *p)
{
	struct vrmlCylinder *m;
	struct vrmlObjects *vrml;
	double value;
	int c,type;

	vrml=NULL;
	if(!p)return vrml;

	m=(struct vrmlCylinder *)cMalloc(sizeof(struct vrmlCylinder),8686);
	if(!m)return vrml;
	zerol((char *)m,sizeof(struct vrmlCylinder));

	m->type=T_VRML_Cylinder;
	m->killObject=kObject;
	m->duplicateObject=dCylinder;

	m->radius = 1.0;
	m->height = 2.0;
	m->parts = T_VRML_ALL;

	while(1){
	    c=next(p);
	    if(c == '}'){
	        break;
	    }
	    if(!strcmp(p->buff,"radius")){
	        type = 0;
	    }else if(!strcmp(p->buff,"height")){
	        type = 1;
	    }else if(!strcmp(p->buff,"parts")){
	        type = 2;
	    }else {
	        sprintf(WarningBuff,"Cylinder UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(type == 0){
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Cylinder Expecting number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        sscanf(p->buff,"%lf",&value);
	        m->radius = value;
	    }else if(type == 1){
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Cylinder Expecting number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        sscanf(p->buff,"%lf",&value);
	        m->height = value;
	    }else{
	        if(c != T_NAME){
	           	sprintf(WarningBuff,"Error Cylinder Expecting name got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        
	        c=T_VRML_ALL;
	           
	        if(!strcmp(p->buff,"TOP")){
	           c=T_VRML_TOP;
	        }else if(!strcmp(p->buff,"BOTTOM")){
	           c=T_VRML_BOTTOM;
	        }else if(!strcmp(p->buff,"SIDES")){
	           c=T_VRML_SIDES;
	        }else if(!strcmp(p->buff,"ALL")){
	           c=T_VRML_ALL;
	        }
	        m->parts = c;
	    }
	}
	if(p->MostNodes < 2*400)p->MostNodes=2*400;
	if(p->MostCells < 2*400)p->MostCells=2*400;
	if(p->MostNormals < 2*400)p->MostNormals=2*400;
	return (struct vrmlObjects *)m;
ErrorOut:
	kObject((struct vrmlObjects *)m);
	return vrml;
}

static struct vrmlObjects *dSphere(struct vrmlObjects *vrml)
{
	struct vrmlSphere *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlSphere *)vrml;

	s1=(struct vrmlSphere *)cMalloc(sizeof(struct vrmlSphere),8687);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dSphere\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *Sphere(struct Parser *p)
{
	struct vrmlSphere *m;
	struct vrmlObjects *vrml;
	double value;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	m=(struct vrmlSphere *)cMalloc(sizeof(struct vrmlSphere),8688);
	if(!m)return vrml;
	zerol((char *)m,sizeof(struct vrmlSphere));

	m->type=T_VRML_Sphere;
	m->killObject=kObject;
	m->duplicateObject=dSphere;

	c=next(p);
	m->radius = 1.0;
	if(c == '}'){
	    return (struct vrmlObjects *)m;
	}
	if(strcmp(p->buff,"radius")){
	    sprintf(WarningBuff,"Sphere UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != T_NUMBER){
	       	sprintf(WarningBuff,"Error Sphere Expecting number got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	}
	sscanf(p->buff,"%lf",&value);
	m->radius = value;
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error Sphere Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	if(p->MostNodes < 400*2)p->MostNodes=400*2;
	if(p->MostCells < 400*2)p->MostCells=400*2;
	if(p->MostNormals < 400*2)p->MostNormals=400*2;
	return (struct vrmlObjects *)m;
ErrorOut:
	kObject((struct vrmlObjects *)m);
	return vrml;
}
static struct vrmlObjects *dTransform(struct vrmlObjects *vrml)
{
	struct vrmlTransForm *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlTransForm *)vrml;

	s1=(struct vrmlTransForm *)cMalloc(sizeof(struct vrmlTransForm),8689);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dTransform\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
static struct vrmlObjects *Transform(struct Parser *p)
{
	struct vrmlTransForm *r;
	struct vrmlObjects *vrml;
	double *a[5];
	static int inc[5]={3,4,3,4,3};
	double value;
	int n,type,pass;
	int c;


	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlTransForm *)cMalloc(sizeof(struct vrmlTransForm),8690);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlTransForm));

	r->type=T_VRML_Transform;
	r->killObject=kObject;
	r->duplicateObject=dTransform;

	a[0]= (double *)&r->translation;
	a[1]= (double *)&r->rotation;
	a[2]= (double *)&r->scaleFactor;
	a[3]= (double *)&r->scaleOrientation;
	a[4]= (double *)&r->center;


	r->translation[0] = 0;
	r->translation[1] = 0;
	r->translation[2] = 0;

	r->rotation[0] = 0;
	r->rotation[1] = 0;
	r->rotation[2] = 1;
	r->rotation[3] = 0;

	r->scaleFactor[0] = 1;
	r->scaleFactor[1] = 1;
	r->scaleFactor[2] = 1;

	r->scaleOrientation[0] = 0;
	r->scaleOrientation[1] = 0;
	r->scaleOrientation[2] = 1;
	r->scaleOrientation[3] = 0;

	r->center[0] = 0;
	r->center[1] = 0;
	r->center[2] = 0;

	pass=0;

	while(1){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '}'){
	        if(pass == 0){
	            return (struct vrmlObjects *)r;
	        }else{
	            break;
	        }
	    }
	    pass=1;
	    if(!strcmp(p->buff,"translation")){
	       type=0;
	    }else if(!strcmp(p->buff,"rotation")){
	       type=1;
	    }else if(!strcmp(p->buff,"scaleFactor")){
	       type=2;
	    }else if(!strcmp(p->buff,"scaleOrientation")){
	       type=3;
	    }else if(!strcmp(p->buff,"center")){
	       type=4;
	    }else{
	        sprintf(WarningBuff,"Transform UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    for(n=0;n<inc[type];++n){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Transform Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
	        sscanf(p->buff,"%lf",&value);
		a[type][n]=value;
	    }
	}
	return (struct vrmlObjects *)r;
ErrorOut:
	kObject((struct vrmlObjects *)r);
	return vrml;
}
static struct vrmlObjects *dUse(struct vrmlObjects *vrml)
{
	struct vrmlUse *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlUse *)vrml;

	s1=(struct vrmlUse *)cMalloc(sizeof(struct vrmlUse),8691);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dUse\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Use(struct Parser *p)
{
	struct vrmlUse *r;
	struct vrmlObjects *vrml;
	int found;
	long n;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlUse *)cMalloc(sizeof(struct vrmlUse),8692);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlUse));

	r->type=T_VRML_Use;
	r->killObject=kObject;
	r->duplicateObject=dUse;

	found=FALSE;
	for(n=0;n<p->nameCount;++n){
	   if(!strcmp(p->list[n].name,p->buff)){
	       found=TRUE;
	       r->o=p->list[n].o;
	       break;
	   }
	}
	if(!found){
	    sprintf(WarningBuff,"USE name %s Not Found\n",p->buff);
		WarningBatch(WarningBuff);
	    r->objectNumber=0;
	}else{
	    r->objectNumber=n+1;
	}
	return (struct vrmlObjects *)r;
}
static struct vrmlObjects *dNormalBinding(struct vrmlObjects *vrml)
{
	struct vrmlNormalBinding *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlNormalBinding *)vrml;

	s1=(struct vrmlNormalBinding *)cMalloc(sizeof(struct vrmlNormalBinding),8693);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dNormalBinding\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *NormalBinding(struct Parser *p)
{
	struct vrmlNormalBinding *r;
	struct vrmlObjects *vrml;
	long value;
	int c;


	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlNormalBinding *)cMalloc(sizeof(struct vrmlNormalBinding),8694);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlNormalBinding));

	r->type=T_VRML_NormalBinding;
	r->killObject=kObject;
	r->duplicateObject=dNormalBinding;

	c=next(p);
	r->value = OVERALL;
	if(c == '}'){
	    return (struct vrmlObjects *)r;
	}
	if(strcmp(p->buff,"value")){
	    sprintf(WarningBuff,"NormalBinding UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != T_NAME){
	       	sprintf(WarningBuff,"Error NormalBinding Expecting name got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	}
	if(!strcmp(p->buff,"DEFAULT")){
	    value=DEFAULT;
	}else if(!strcmp(p->buff,"OVERALL")){
	    value=OVERALL;
	}else if(!strcmp(p->buff,"PER_PART")){
	    value=PER_PART;
	}else if(!strcmp(p->buff,"PER_PART_INDEXED")){
	    value=PER_PART_INDEXED;
	}else if(!strcmp(p->buff,"PER_FACE")){
	    value=PER_FACE;
	}else if(!strcmp(p->buff,"PER_FACE_INDEXED")){
	    value=PER_FACE_INDEXED;
	}else if(!strcmp(p->buff,"PER_VERTEX")){
	    value=PER_VERTEX;
	}else if(!strcmp(p->buff,"PER_VERTEX_INDEXED")){
	    value=PER_VERTEX_INDEXED;
	}else{
	    sprintf(WarningBuff,"Error MaterialBinding Bad value %s\n",p->buff);
		WarningBatch(WarningBuff);
	   	goto ErrorOut;
	}
	r->value = value;
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error NormalBinding Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	return (struct vrmlObjects *)r;
ErrorOut:
	kObject((struct vrmlObjects *)r);
	return vrml;
}
static int kNormal(struct vrmlObjects *vrml)
{
	struct vrmlNormal *r;

	if(!vrml)return 1;
	r=(struct vrmlNormal *)vrml;

	if(r->normaldata)cFree((char *)r->normaldata);
	r->normaldata=NULL;

	cFree((char *)r);
	return 0;
}
static struct vrmlObjects *dNormal(struct vrmlObjects *vrml)
{
	struct vrmlNormal *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlNormal *)vrml;

	vrml=NULL;

	r1=(struct vrmlNormal *)cMalloc(sizeof(struct vrmlNormal),8695);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlNormal));

	*r1 = *r2;

	r1->normaldata=NULL;

	r1->normal=0;

	if(r2->normaldata && r2->normal){
	    r1->normaldata=(double *)cMalloc(r2->normal*sizeof(double),8696);
	    if(!r1->normaldata)goto ErrorOut;
	    for(n=0;n<r2->normal;++n){
	        r1->normaldata[n]=r2->normaldata[n];
	    }
	    r1->normal=r2->normal;
	    r1->normalmax=r1->normal;
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dNormal Error\n");
	WarningBatch(WarningBuff);
	kNormal((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Normal(struct Parser *p)
{
	struct vrmlNormal *r;
	struct vrmlObjects *vrml;
	struct fallocate *a;
	double value;
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlNormal *)cMalloc(sizeof(struct vrmlNormal),8697);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlNormal));

	r->type=T_VRML_Normal;
	r->killObject=kNormal;
	r->duplicateObject=dNormal;

	a=(struct fallocate *)&r->normaldata;

	if(check2((struct checker *)a,sizeof(double)))goto ErrorOut;

	r->normaldata[0] = 0.0;
	r->normaldata[1] = 0.0;
	r->normaldata[2] = 1.0;

	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c == '}'){
	    r->normal += 3;
	    goto Ok;
	}
	if(strcmp(p->buff,"vector")){
	    sprintf(WarningBuff,"Normal UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c == '['){
	    while(1){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c == ']')break;
	        if(c == ',')continue;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Normal Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
		if(check2((struct checker *)a,sizeof(double)))goto ErrorOut;
	        sscanf(p->buff,"%lf",&value);
			r->normaldata[r->normal++]=value;
	    }
	}else{
	    for(n=0;n<3;++n){
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Normal Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
		if(check2((struct checker *)a,sizeof(double)))goto ErrorOut;
	        sscanf(p->buff,"%lf",&value);
			r->normaldata[r->normal++]=value;
	        c=next(p);
	    }
	    goto Next;
	}
	c=next(p);
Next:
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error Normal Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
Ok:
	if(checkfix((struct checker *)a,sizeof(double)))goto ErrorOut;
	if(r->normal/3 > p->MostNormals)p->MostNormals = r->normal/3;
	if(2*r->normal/3 > p->MostNodes)p->MostNodes = 2*r->normal/3;
	return (struct vrmlObjects *)r;
ErrorOut:
	kNormal((struct vrmlObjects *)r);
	return vrml;
}
static int kIndexedFaceSet(struct vrmlObjects *vrml)
{
	struct vrmlIndexedFaceSet *r;

	if(!vrml)return 1;
	r=(struct vrmlIndexedFaceSet *)vrml;

	if(r->coorddata)cFree((char *)r->coorddata);
	r->coorddata=NULL;

	if(r->matdata)cFree((char *)r->matdata);
	r->matdata=NULL;

	if(r->normdata)cFree((char *)r->normdata);
	r->normdata=NULL;

	if(r->textdata)cFree((char *)r->textdata);
	r->textdata=NULL;

	cFree((char *)r);
	return 0;
}

static struct vrmlObjects *dIndexedFaceSet(struct vrmlObjects *vrml)
{
	struct vrmlIndexedFaceSet *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlIndexedFaceSet *)vrml;

	vrml=NULL;

	r1=(struct vrmlIndexedFaceSet *)cMalloc(sizeof(struct vrmlIndexedFaceSet),8698);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlIndexedFaceSet));

	*r1 = *r2;

	r1->coorddata=NULL;
	r1->matdata=NULL;
	r1->normdata=NULL;
	r1->textdata=NULL;

	r1->coord=0;
	r1->mat=0;
	r1->norm=0;
	r1->text=0;

	if(r2->coorddata && r2->coord){
	    r1->coorddata=(long *)cMalloc(r2->coord*sizeof(long),8699);
	    if(!r1->coorddata)goto ErrorOut;
	    for(n=0;n<r2->coord;++n){
	        r1->coorddata[n]=r2->coorddata[n];
	    }
	    r1->coord=r2->coord;
	    r1->coordmax=r1->coord;
	}

	if(r2->matdata && r2->mat){
	    r1->matdata=(long *)cMalloc(r2->mat*sizeof(long),8700);
	    if(!r1->matdata)goto ErrorOut;
	    for(n=0;n<r2->mat;++n){
	        r1->matdata[n]=r2->matdata[n];
	    }
	    r1->mat=r2->mat;
	    r1->matmax=r1->mat;
	}

	if(r2->normdata && r2->norm){
	    r1->normdata=(long *)cMalloc(r2->norm*sizeof(long),8701);
	    if(!r1->normdata)goto ErrorOut;
	    for(n=0;n<r2->norm;++n){
	        r1->normdata[n]=r2->normdata[n];
	    }
	    r1->norm=r2->norm;
	    r1->normmax=r1->norm;
	}

	if(r2->textdata && r2->text){
	    r1->textdata=(long *)cMalloc(r2->text*sizeof(long),8702);
	    if(!r1->textdata)goto ErrorOut;
	    for(n=0;n<r2->text;++n){
	        r1->textdata[n]=r2->textdata[n];
	    }
	    r1->text=r2->text;
	    r1->textmax=r1->text;
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dIndexedFaceSet Error\n");
	WarningBatch(WarningBuff);
	kIndexedFaceSet((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *IndexedFaceSet(struct Parser *p)
{
	struct vrmlIndexedFaceSet *r;
	struct vrmlObjects *vrml;
	struct allocate *a[4];
	char *bp;
	long lvalue;
	int n,type,pass;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlIndexedFaceSet *)cMalloc(sizeof(struct vrmlIndexedFaceSet),8703);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlIndexedFaceSet));

	r->type=T_VRML_IndexedFaceSet;
	r->killObject=kIndexedFaceSet;
	r->duplicateObject=dIndexedFaceSet;



	a[0]=(struct allocate *)&r->coorddata;
	a[1]=(struct allocate *)&r->matdata;
	a[2]=(struct allocate *)&r->normdata;
	a[3]=(struct allocate *)&r->textdata;

	for(n=0;n<4;++n){
	    if(check2((struct checker *)a[n],sizeof(long)))goto ErrorOut;
	}

	r->coorddata[0] = 0;
	r->matdata[0] =  -1;
	r->normdata[0] = -1;
	r->textdata[0] = -1;
	pass=0;

	while(1){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '}'){
	        if(pass == 0){
	            ++r->coord;
	            goto Ok;
	        }else{
	            break;
	        }
	    }
	    pass=1;
	    if(!strcmp(p->buff,"coordIndex")){
	       type=0;
	    }else if(!strcmp(p->buff,"materialIndex")){
	       type=1;
	    }else if(!strcmp(p->buff,"normalIndex")){
	       type=2;
	    }else if(!strcmp(p->buff,"textureCoordIndex")){
	       type=3;
	    }else{
	        sprintf(WarningBuff,"IndexedFaceSet UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '['){
	        while(1){
	            c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c == ']')break;
	            if(c == ',')continue;
	            if(c != T_NUMBER){
	               	sprintf(WarningBuff,"Error IndexedFaceSet Expecting Number got %d %c\n",c,c);
					WarningBatch(WarningBuff);
	               	goto ErrorOut;
	            }
			    if(check2((struct checker *)a[type],sizeof(long)))goto ErrorOut;
			    bp=p->buff;
			    if(*bp == '-' || *bp == '+'){
			        if(bp[1] == '0' && (bp[2] == 'X' || bp[2] == 'x' )){
		                    sscanf(bp,"%lx",&lvalue);
			        }else{
		                    sscanf(bp,"%ld",&lvalue);
			        }
			    }else if(*bp == '0' && (bp[1] == 'X' || bp[1] == 'x' )){
		                sscanf(bp,"%lx",&lvalue);
			    }else{
		                sscanf(bp,"%ld",&lvalue);
		        }
			    a[type]->textdata[a[type]->text++]=lvalue;
	        }
	    }else{
	        for(n=0;n<1;++n){
	            if(n != 0)c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c != T_NUMBER){
	               	sprintf(WarningBuff,"Error IndexedFaceSet Expecting Number got %d %c\n",c,c);
					WarningBatch(WarningBuff);
	               	goto ErrorOut;
	            }
			    if(check2((struct checker *)a[type],sizeof(long)))goto ErrorOut;
			    bp=p->buff;
			    if(*bp == '-' || *bp == '+'){
			        if(bp[1] == '0' && (bp[2] == 'X' || bp[2] == 'x' )){
		                    sscanf(bp,"%lx",&lvalue);
			        }else{
		                    sscanf(bp,"%ld",&lvalue);
			        }
			    }else if(*bp == '0' && (bp[1] == 'X' || bp[1] == 'x' )){
		                sscanf(bp,"%lx",&lvalue);
			    }else{
		                sscanf(bp,"%ld",&lvalue);
		        }
			    a[type]->textdata[a[type]->text++]=lvalue;	            
	        }
	    }
	}
Ok:
	for(n=0;n<4;++n){
	    if(checkfix((struct checker *)a[n],sizeof(long)))goto ErrorOut;
	}

	if(p->MostNodes < 2*(r->coord))p->MostNodes=2*(r->coord);
	if(p->MostCells < 2*(r->coord))p->MostCells=2*(r->coord);
	if(p->MostNormals < 2*(r->coord))p->MostNormals=2*(r->coord);

	return (struct vrmlObjects *)r;
ErrorOut:
	kIndexedFaceSet((struct vrmlObjects *)r);
	return vrml;
}
static int kCoordinate3(struct vrmlObjects *vrml)
{
	struct vrmlCoordinate3 *r;

	if(!vrml)return 1;
	r=(struct vrmlCoordinate3 *)vrml;

	if(r->pointdata)cFree((char *)r->pointdata);
	r->pointdata=NULL;

	cFree((char *)r);
	return 0;
}
static struct vrmlObjects *dCoordinate3(struct vrmlObjects *vrml)
{
	struct vrmlCoordinate3 *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlCoordinate3 *)vrml;

	vrml=NULL;

	r1=(struct vrmlCoordinate3 *)cMalloc(sizeof(struct vrmlCoordinate3),8704);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlCoordinate3));

	*r1 = *r2;

	r1->pointdata=NULL;

	r1->points=0;

	if(r2->pointdata && r2->points){
	    r1->pointdata=(double *)cMalloc(r2->points*sizeof(double),8705);
	    if(!r1->pointdata)goto ErrorOut;
	    for(n=0;n<r2->points;++n){
	        r1->pointdata[n]=r2->pointdata[n];
	    }
	    r1->points=r2->points;
	    r1->pointsmax=r1->points;
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dCoordinate3 Error\n");
	WarningBatch(WarningBuff);
	kCoordinate3((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Coordinate3(struct Parser *p)
{
	struct vrmlCoordinate3 *r;
	struct vrmlObjects *vrml;
	struct checker *a;
	double value;
	void *op;
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlCoordinate3 *)cMalloc(sizeof(struct vrmlCoordinate3),8706);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlCoordinate3));

	r->type=T_VRML_Coordinate3;
	r->killObject=kCoordinate3;
	r->duplicateObject=dCoordinate3;


	op=&r->pointdata;
	a=op;

	if(check2(a,sizeof(double)))goto ErrorOut;

	r->pointdata[0] = 0.0;
	r->pointdata[1] = 0.0;
	r->pointdata[2] = 0.0;

	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c == '}'){
	    r->points = 3;
	    goto Ok;
	}
	if(strcmp(p->buff,"point")){
	    sprintf(WarningBuff,"Coordinate3 UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c == '['){
	    while(1){
	        c=next(p);
	        if(c == EOF)goto ErrorOut;
	        if(c == ']')break;
	        if(c == ',')continue;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Coordinate3 Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
			if(check2(a,sizeof(double)))goto ErrorOut;
		        sscanf(p->buff,"%lf",&value);
			r->pointdata[r->points++]=value;
	    }
	}else{
	    for(n=0;n<3;++n){
	        if(c == EOF)goto ErrorOut;
	        if(c != T_NUMBER){
	           	sprintf(WarningBuff,"Error Coordinate3 Expecting Number got %d %c\n",c,c);
				WarningBatch(WarningBuff);
	           	goto ErrorOut;
	        }
			if(check2(a,sizeof(double)))goto ErrorOut;
		        sscanf(p->buff,"%lf",&value);
			r->pointdata[r->points++]=value;
	        c=next(p);
	    }
	    goto Next;
	}
	c=next(p);
Next:
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error Coordinate3 Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
Ok:
	if(checkfix(a,sizeof(double)))goto ErrorOut;
	{
	    long np;

	    np=2*(r->points/3);
	    if(np > p->MostNodes)p->MostNodes=np;

	}
	return (struct vrmlObjects *)r;
ErrorOut:
	kCoordinate3((struct vrmlObjects *)r);
	return vrml;
}

static struct vrmlObjects *dMatrixTransform(struct vrmlObjects *vrml)
{
	struct vrmlMatrixTransform *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlMatrixTransform *)vrml;

	s1=(struct vrmlMatrixTransform *)cMalloc(sizeof(struct vrmlMatrixTransform),8708);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dMatrixTransform\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *MatrixTransform(struct Parser *p)
{
	struct vrmlMatrixTransform *r;
	struct vrmlObjects *vrml;
	double value;
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlMatrixTransform *)cMalloc(sizeof(struct vrmlMatrixTransform),8709);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlMatrixTransform));

	r->type=T_VRML_MatrixTransform;
	r->killObject=kObject;
	r->duplicateObject=dMatrixTransform;

	c=next(p);
	r->matrix[0] = 1.0;
	r->matrix[1] = 0.0;
	r->matrix[2] = 0.0;
	r->matrix[3] = 0.0;

	r->matrix[4] = 0.0;
	r->matrix[5] = 1.0;
	r->matrix[6] = 0.0;
	r->matrix[7] = 0.0;

	r->matrix[8] = 0.0;
	r->matrix[9] = 0.0;
	r->matrix[10] = 1.0;
	r->matrix[11] = 0.0;

	r->matrix[12] = 0.0;
	r->matrix[13] = 0.0;
	r->matrix[14] = 0.0;
	r->matrix[15] = 1.0;

	if(c == '}'){
	    return (struct vrmlObjects *)r;
	}

	if(strcmp(p->buff,"matrix")){
	    sprintf(WarningBuff,"MatrixTransform UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}

	for(n=0;n<16;++n){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;;
	    if(c != T_NUMBER){
	       	sprintf(WarningBuff,"Error MatrixTransform Expecting Number got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	    }
	    sscanf(p->buff,"%lf",&value);
	    r->matrix[n]=value;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error MatrixTransform Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	return (struct vrmlObjects *)r;
ErrorOut:
	kObject((struct vrmlObjects *)r);
	return vrml;
}

static int kMaterial(struct vrmlObjects *vrml)
{
	struct vrmlMaterial *r;

	if(!vrml)return 1;
	r=(struct vrmlMaterial *)vrml;

	if(r->ambientdata)cFree((char *)r->ambientdata);
	r->ambientdata=NULL;

	if(r->diffusedata)cFree((char *)r->diffusedata);
	r->diffusedata=NULL;

	if(r->speculardata)cFree((char *)r->speculardata);
	r->speculardata=NULL;

	if(r->emissivedata)cFree((char *)r->emissivedata);
	r->emissivedata=NULL;

	if(r->shinedata)cFree((char *)r->shinedata);
	r->shinedata=NULL;


	if(r->transdata)cFree((char *)r->transdata);
	r->transdata=NULL;

	if(r->flagsdata)cFree((char *)r->flagsdata);
	r->flagsdata=NULL;

	cFree((char *)r);
	return 0;
}
static struct vrmlObjects *dMaterial(struct vrmlObjects *vrml)
{
	struct vrmlMaterial *r1,*r2;
	long n;

	if(!vrml)return vrml;

	r2=(struct vrmlMaterial *)vrml;

	vrml=NULL;

	r1=(struct vrmlMaterial *)cMalloc(sizeof(struct vrmlMaterial),8710);
	if(!r1)return vrml;
	zerol((char *)r1,sizeof(struct vrmlMaterial));

	*r1 = *r2;

	r1->ambientdata=NULL;
	r1->diffusedata=NULL;
	r1->speculardata=NULL;
	r1->emissivedata=NULL;
	r1->shinedata=NULL;
	r1->transdata=NULL;
	r1->flagsdata=NULL;

	r1->ambient=0;
	r1->diffuse=0;
	r1->specular=0;
	r1->emissive=0;
	r1->shine=0;
	r1->flags=0;
	r1->trans=0;

	if(r2->ambientdata && r2->ambient){
	    r1->ambientdata=(double *)cMalloc(r2->ambient*sizeof(double),8711);
	    if(!r1->ambientdata)goto ErrorOut;
	    for(n=0;n<r2->ambient;++n){
	        r1->ambientdata[n]=r2->ambientdata[n];
	    }
	    r1->ambient=r2->ambient;
	    r1->ambientmax=r1->ambient;
	}
	
	if(r2->diffusedata && r2->diffuse){
	    r1->diffusedata=(double *)cMalloc(r2->diffuse*sizeof(double),8712);
	    if(!r1->diffusedata)goto ErrorOut;
	    for(n=0;n<r2->diffuse;++n){
	        r1->diffusedata[n]=r2->diffusedata[n];
	    }
	    r1->diffuse=r2->diffuse;
	    r1->diffusemax=r1->diffuse;
	}
	
	if(r2->speculardata && r2->specular){
	    r1->speculardata=(double *)cMalloc(r2->specular*sizeof(double),8713);
	    if(!r1->speculardata)goto ErrorOut;
	    for(n=0;n<r2->specular;++n){
	        r1->speculardata[n]=r2->speculardata[n];
	    }
	    r1->specular=r2->specular;
	    r1->specularmax=r1->specular;
	}
	
	if(r2->emissivedata && r2->emissive){
	    r1->emissivedata=(double *)cMalloc(r2->emissive*sizeof(double),8714);
	    if(!r1->emissivedata)goto ErrorOut;
	    for(n=0;n<r2->emissive;++n){
	        r1->emissivedata[n]=r2->emissivedata[n];
	    }
	    r1->emissive=r2->emissive;
	    r1->emissivemax=r1->emissive;
	}
	
	if(r2->shinedata && r2->shine){
	    r1->shinedata=(double *)cMalloc(r2->shine*sizeof(double),8715);
	    if(!r1->shinedata)goto ErrorOut;
	    for(n=0;n<r2->shine;++n){
	        r1->shinedata[n]=r2->shinedata[n];
	    }
	    r1->shine=r2->shine;
	    r1->shinemax=r1->shine;
	}
	
	if(r2->transdata && r2->trans){
	    r1->transdata=(double *)cMalloc(r2->trans*sizeof(double),8716);
	    if(!r1->transdata)goto ErrorOut;
	    for(n=0;n<r2->trans;++n){
	        r1->transdata[n]=r2->transdata[n];
	    }
	    r1->trans=r2->trans;
	    r1->transmax=r1->trans;
	}

	if(r2->flagsdata && r2->flags){
	    r1->flagsdata=(long *)cMalloc(r2->flags*sizeof(long),8717);
	    if(!r1->flagsdata)goto ErrorOut;
	    for(n=0;n<r2->flags;++n){
	        r1->flagsdata[n]=r2->flagsdata[n];
	    }
	    r1->flags=r2->flags;
	    r1->flagsmax=r1->flags;
	}
	return (struct vrmlObjects *)r1;
ErrorOut:
	sprintf(WarningBuff,"dMaterial Error\n");
	WarningBatch(WarningBuff);
	kMaterial((struct vrmlObjects *)r1);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Material(struct Parser *p)
{
	struct vrmlMaterial *r;
	struct vrmlObjects *vrml;
	struct fallocate *a[7];
	static int inc[6]={3,3,3,3,1,1};
	double value;
	int type,pass;
	long n,max;
	void *op;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	r=(struct vrmlMaterial *)cMalloc(sizeof(struct vrmlMaterial),8718);
	if(!r)return vrml;
	zerol((char *)r,sizeof(struct vrmlMaterial));

	r->type=T_VRML_Material;
	r->killObject=kMaterial;
	r->duplicateObject=dMaterial;

	op=&r->ambientdata;
	a[0]=op;
	op=&r->diffusedata;
	a[1]=op;
	op=&r->speculardata;
	a[2]=op;
	op=&r->emissivedata;
	a[3]=op;
	op=&r->shinedata;
	a[4]=op;
	op=&r->transdata;
	a[5]=op;
	op=&r->flagsdata;
	a[6]=op;

	for(n=0;n<6;++n){
	    if(check2((struct checker *)a[n],sizeof(double)))goto ErrorOut;
	}

	r->ambientdata[0] = .2f;
	r->ambientdata[1] = .2f;
	r->ambientdata[2] = .2f;
	r->diffusedata[0] = .8f;
	r->diffusedata[1] = .8f;
	r->diffusedata[2] = .8f;
	r->shinedata[0] = .2f;
	pass=0;

	while(1){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '}'){
	        if(pass == 0){
	            r->ambient = 3;
	            r->diffuse = 3;
	            r->shine = 1;
	            goto Ok;
	        }else{
	            break;
	        }
	    }
	    pass=1;
	    if(!strcmp(p->buff,"ambientColor")){
	       type=0;
	    }else if(!strcmp(p->buff,"diffuseColor")){
	       type=1;
	    }else if(!strcmp(p->buff,"specularColor")){
	       type=2;
	    }else if(!strcmp(p->buff,"emissiveColor")){
	       type=3;
	    }else if(!strcmp(p->buff,"shininess")){
	       type=4;
	    }else if(!strcmp(p->buff,"transparency")){
	       type=5;
	    }else{
	        sprintf(WarningBuff,"Material UnExpected Command %s\n",p->buff);
			WarningBatch(WarningBuff);
	        goto ErrorOut;
	    }
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c == '['){
	        while(1){
	            c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c == ']')break;
	            if(c == ',')continue;
	            if(c != T_NUMBER){
	               break;
	            }
			    if(check2((struct checker *)a[type],sizeof(double)))goto ErrorOut;
		            sscanf(p->buff,"%lf",&value);
			    a[type]->pointdata[a[type]->points++]=value;
	        }
	    }else{
	        for(n=0;n<inc[type];++n){
	            if(n != 0)c=next(p);
	            if(c == EOF)goto ErrorOut;
	            if(c != T_NUMBER){
	               break;
	            }
			    if(check2((struct checker *)a[type],sizeof(double)))goto ErrorOut;
		            sscanf(p->buff,"%lf",&value);
			    a[type]->pointdata[a[type]->points++]=value;
	        }
	    }
	}
Ok:
	max=0;
	if(r->emissive/3 > max)max=r->emissive/3;
	if(r->diffuse/3 > max)max=r->diffuse/3;
	if(r->ambient/3 > max)max=r->ambient/3;
	if(r->specular/3 > max)max=r->specular/3;
	if(r->shine > max)max=r->shine;
	if(r->trans > max)max=r->trans;

	r->flags=0;
	for(n=0;n<max;++n){
	    op=&r->flagsdata;
	    if(check2(op,sizeof(long)))goto ErrorOut;
	    r->flagsdata[n]=0;
	    ++r->flags;

	    if(n >= r->diffuse/3){
	        op=&r->diffusedata;
	        if(check2(op,sizeof(double)))goto ErrorOut;
	        if(n < r->emissive/3){
		    r->diffusedata[3*n] = r->emissivedata[3*n];
		    r->diffusedata[3*n+1] = r->emissivedata[3*n+1];
		    r->diffusedata[3*n+2] = r->emissivedata[3*n+2];
	            r->flagsdata[n] |= M_DIFFUSE;
	        }
			r->diffuse  = 3*(n+1);
	    }else{
	        r->flagsdata[n] |= M_DIFFUSE;
	    }

	    if(n >= r->emissive/3){
	        op=&r->emissivedata;
	        if(check2(op,sizeof(double)))goto ErrorOut;
			r->emissive  = 3*(n+1);
	    }else{
	        r->flagsdata[n] |= M_EMISSIVE;
	    }

	    if(n >= r->ambient/3){
	        op=&r->ambientdata;
	        if(check2(op,sizeof(double)))goto ErrorOut;
			r->ambient  = 3*(n+1);
	    }else{
	        r->flagsdata[n] |= M_AMBIENT;
	    }

	    if(n >= r->specular/3){
	        op=&r->speculardata;
	        if(check2(op,sizeof(double)))goto ErrorOut;
			r->specular = 3*(n+1);
	    }else{
	        r->flagsdata[n] |= M_SPECULAR;
	    }

	    if(n >= r->trans){
	        op=&r->transdata;
	        if(check2(op,sizeof(double)))goto ErrorOut;
			r->trans = n+1;
	    }else{
	        r->flagsdata[n] |= M_TRANSPARENT;
	    }

	    if(n >= r->shine){
	        op=&r->shinedata;
	        if(check2(op,sizeof(double)))goto ErrorOut;
			r->shinedata[n]=.2f;
			r->shine = n+1;
	    }

	}
	for(n=0;n<6;++n){
	    if(checkfix((struct checker *)a[n],sizeof(double)))goto ErrorOut;
	}
	op=&r->flagsdata;
	if(checkfix(op,sizeof(long)))goto ErrorOut;

	r->max=max;
	return (struct vrmlObjects *)r;
ErrorOut:
	kMaterial((struct vrmlObjects *)r);
	return vrml;
}
static struct vrmlObjects *dMaterialBinding(struct vrmlObjects *vrml)
{
	struct vrmlMaterialBinding *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlMaterialBinding *)vrml;

	s1=(struct vrmlMaterialBinding *)cMalloc(sizeof(struct vrmlMaterialBinding),8719);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dMaterialBinding\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *MaterialBinding(struct Parser *p)
{
	struct vrmlMaterialBinding *m;
	struct vrmlObjects *vrml;
	long value;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	m=(struct vrmlMaterialBinding *)cMalloc(sizeof(struct vrmlMaterialBinding),8720);
	if(!m)return vrml;
	zerol((char *)m,sizeof(struct vrmlMaterialBinding));

	m->type=T_VRML_MaterialBinding;
	m->killObject=kObject;
	m->duplicateObject=dMaterialBinding;

	c=next(p);
	m->value = OVERALL;
	if(c == '}'){
	    return (struct vrmlObjects *)m;
	}
	if(strcmp(p->buff,"value")){
	    sprintf(WarningBuff,"MaterialBinding UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != T_NAME){
	       	sprintf(WarningBuff,"Error MaterialBinding Expecting name got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	}
	if(!strcmp(p->buff,"DEFAULT")){
	    value=DEFAULT;
	}else if(!strcmp(p->buff,"OVERALL")){
	    value=OVERALL;
	}else if(!strcmp(p->buff,"PER_PART")){
	    value=PER_PART;
	}else if(!strcmp(p->buff,"PER_PART_INDEXED")){
	    value=PER_PART_INDEXED;
	}else if(!strcmp(p->buff,"PER_FACE")){
	    value=PER_FACE;
	}else if(!strcmp(p->buff,"PER_FACE_INDEXED")){
	    value=PER_FACE_INDEXED;
	}else if(!strcmp(p->buff,"PER_VERTEX")){
	    value=PER_VERTEX;
	}else if(!strcmp(p->buff,"PER_VERTEX_INDEXED")){
	    value=PER_VERTEX_INDEXED;
	}else{
	    sprintf(WarningBuff,"Error MaterialBinding Bad value %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	m->value = value;
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error MaterialBinding Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	return (struct vrmlObjects *)m;
ErrorOut:
	kObject((struct vrmlObjects *)m);
	return vrml;
}
static struct vrmlObjects *dScale(struct vrmlObjects *vrml)
{
	struct vrmlScale *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlScale *)vrml;

	s1=(struct vrmlScale *)cMalloc(sizeof(struct vrmlScale),8721);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dScale\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Scale(struct Parser *p)
{
	struct vrmlScale *s;
	struct vrmlObjects *vrml;
	double value[3];
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	s=(struct vrmlScale *)cMalloc(sizeof(struct vrmlScale),8722);
	if(!s)return vrml;
	zerol((char *)s,sizeof(struct vrmlScale));

	s->type=T_VRML_Scale;
	s->killObject=kObject;
	s->duplicateObject=dScale;

	c=next(p);
	s->scale[0] = 1.0;
	s->scale[1] = 1.0;
	s->scale[2] = 1.0;
	if(c == '}'){
	    return (struct vrmlObjects *)s;
	}
	if(strcmp(p->buff,"scaleFactor")){
	    sprintf(WarningBuff,"Scale UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	for(n=0;n<3;++n){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c != T_NUMBER){
	       	sprintf(WarningBuff,"Error Scale Expecting Number got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	    }
	    sscanf(p->buff,"%lf",&value[n]);
	}
	s->scale[0] = value[0];
	s->scale[1] = value[1];
	s->scale[2] = value[2];
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error Scale Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	return (struct vrmlObjects *)s;
ErrorOut:
	kObject((struct vrmlObjects *)s);
	return vrml;
}
static struct vrmlObjects *dTranslation(struct vrmlObjects *vrml)
{
	struct vrmlTranslation *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlTranslation *)vrml;

	s1=(struct vrmlTranslation *)cMalloc(sizeof(struct vrmlTranslation),8723);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dTranslation\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Translation(struct Parser *p)
{
	struct vrmlTranslation *t;
	struct vrmlObjects *vrml;
	double value[3];
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	t=(struct vrmlTranslation *)cMalloc(sizeof(struct vrmlTranslation),8724);
	if(!t)return vrml;
	zerol((char *)t,sizeof(struct vrmlTranslation));

	t->type=T_VRML_Translation;
	t->killObject=kObject;
	t->duplicateObject=dTranslation;

	c=next(p);
	t->offset[0] = 0.0;
	t->offset[1] = 0.0;
	t->offset[2] = 0.0;
	if(c == '}'){
	    return (struct vrmlObjects *)t;
	}
	if(strcmp(p->buff,"translation")){
	    sprintf(WarningBuff,"Translation UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	for(n=0;n<3;++n){
	    c=next(p);
	    if(c == EOF)goto ErrorOut;
	    if(c != T_NUMBER){
	       	sprintf(WarningBuff,"Error Translation Expecting Number got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	    }
	    sscanf(p->buff,"%lf",&value[n]);
	}
	t->offset[0] = value[0];
	t->offset[1] = value[1];
	t->offset[2] = value[2];
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error Translation Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	return (struct vrmlObjects *)t;
ErrorOut:
	kObject((struct vrmlObjects *)t);
	return vrml;
}
static struct vrmlObjects *dRotation(struct vrmlObjects *vrml)
{
	struct vrmlRotation *s1,*s2;

	if(!vrml)return vrml;

	s2=(struct vrmlRotation *)vrml;

	s1=(struct vrmlRotation *)cMalloc(sizeof(struct vrmlRotation),8725);
	if(!s1)goto ErrorOut;

	*s1= *s2;
	return (struct vrmlObjects *)s1;
ErrorOut:
	sprintf(WarningBuff,"Error dRotation\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Rotation(struct Parser *p)
{
	struct vrmlRotation *rot;
	struct vrmlObjects *vrml;
	double value[4];
	int n;
	int c;

	vrml=NULL;
	if(!p)return vrml;

	rot=(struct vrmlRotation *)cMalloc(sizeof(struct vrmlRotation),8726);
	if(!rot)return vrml;
	zerol((char *)rot,sizeof(struct vrmlRotation));

	rot->type=T_VRML_Rotation;
	rot->killObject=kObject;
	rot->duplicateObject=dRotation;

	c=next(p);
	rot->axis[0] = 0.0;
	rot->axis[1] = 0.0;
	rot->axis[2] = 1.0;
	rot->angle = 0.0;
	if(c == '}'){
	    return (struct vrmlObjects *)rot;
	}
	if(strcmp(p->buff,"rotation")){
	    sprintf(WarningBuff,"Rotation UnExpected Command %s\n",p->buff);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	for(n=0;n<4;++n){
	    c=next(p);
	    if(c == EOF)return vrml;
	    if(c != T_NUMBER){
	       	sprintf(WarningBuff,"Error Rotation Expecting Number got %d %c\n",c,c);
			WarningBatch(WarningBuff);
	       	goto ErrorOut;
	    }
	    sscanf(p->buff,"%lf",&value[n]);
	}
	rot->axis[0] = value[0];
	rot->axis[1] = value[1];
	rot->axis[2] = value[2];
	rot->angle = value[3];
	c=next(p);
	if(c == EOF)goto ErrorOut;
	if(c != '}'){
	    sprintf(WarningBuff,"Error Rotation Expecting } got %d %c\n",c,c);
		WarningBatch(WarningBuff);
	    goto ErrorOut;
	}
	return (struct vrmlObjects *)rot;
ErrorOut:
	kObject((struct vrmlObjects *)rot);
	return vrml;
}
static int kSeparator(struct vrmlObjects *vrml)
{
	struct vrmlSeparator *s;
	long n;

	if(!vrml)return 1;

	s=(struct vrmlSeparator *)vrml;

	if(s->o){
	    for(n=0;n<s->Count;++n){
	       vrml=s->o[n];
	       if(vrml && vrml->killObject){
	           (*vrml->killObject)(vrml);
	       }
	    }
	    cFree((char *)s->o);
	}
	cFree((char *)s);
	return 0;
}
static struct vrmlObjects *dSeparator(struct vrmlObjects *vrml)
{
	struct vrmlSeparator *s1,*s2;
	struct checker *ck;
	void *op;
	long n;

	if(!vrml)return vrml;

	s2=(struct vrmlSeparator *)vrml;

	s1=(struct vrmlSeparator *)cMalloc(sizeof(struct vrmlSeparator),8727);
	if(!s1)goto ErrorOut;

	*s1= *s2;

	s1->o=NULL;
	s1->Count=0;
	s1->CountMax=0;

	op=&s1->o;
	ck = op;

	if(s2->o){
	    for(n=0;n<s2->Count;++n){
	       vrml=s2->o[n];
			if(vrml && vrml->duplicateObject){
		    	if(check2(ck,sizeof(struct vrmlObjects *)))goto ErrorOut;
	            vrml=(*vrml->duplicateObject)(vrml);
	            if(!vrml)goto ErrorOut;
		    	s1->o[s1->Count]=vrml;
		    	++s1->Count;
	        }
	    }
	}
	if(checkfix(ck,sizeof(struct vrmlObjects *)))goto ErrorOut;
	if(s1->Count != s2->Count){
        sprintf(WarningBuff,"dSeparator Error s1->Count %ld s2->Count %ld\n",s1->Count,s2->Count);
		WarningBatch(WarningBuff);
    }
	return (struct vrmlObjects *)s1;
ErrorOut:
    sprintf(WarningBuff,"Error dSeparator\n");
	WarningBatch(WarningBuff);
	vrml=NULL;
	return vrml;
}
struct vrmlObjects *Separator(struct Parser *p,int type)
{
	struct vrmlObjects *vrml;
	struct checker *ck;
	struct vrmlSeparator list,*lr;
	long whichChild;
	char name[256];
	void *op;
	int c;

	vrml=NULL;

	if(!p || !p->in)return vrml;

	zerol((char *)&list,sizeof(struct vrmlSeparator));

	whichChild = -1;

	list.type=type;

	op=&list.o;
	ck = op;

	while(1){
	    c=next(p);
	    if(c == '}')break;

	    if(c == T_NAME){
	        if(!strcmp(p->buff,"whichChild")){
	            c=next(p);
	            if(c == T_NUMBER){
	               sscanf(p->buff,"%ld",&whichChild);
	            }else{
	               	sprintf(WarningBuff,"whichChild Error Continue\n");
					WarningBatch(WarningBuff);
	            }
	            continue;
	        }else if(!strcmp(p->buff,"renderCulling")){
	            c=next(p);
	            continue;
	        }else if(!strcmp(p->buff,"renderCaching")){
	            c=next(p);
	            continue;
	        }else if(!strcmp(p->buff,"map")){
	            c=next(p);
	            continue;
	        }else if(!strcmp(p->buff,"name")){
	            c=next(p);
	            continue;
	        }else if(!strcmp(p->buff,"description")){
	            c=next(p);
	            continue;
	        }
	    }
	

	    name[0]=0;
	    vrml=getObjects(c,name,p);
	    if(!vrml)goto ErrorOut;
	    if(vrml->type == T_VRML_EAT){
	        continue;
	    }
	    if(name[0]){
	        if(putObjectName(name,vrml,p))goto ErrorOut;
	    }
	    if(check2(ck,sizeof(struct vrmlObjects *)))goto ErrorOut;
	    list.o[list.Count++]=vrml;
	}
	if(checkfix(ck,sizeof(struct vrmlObjects *)))goto ErrorOut;
	lr=(struct vrmlSeparator *)cMalloc(sizeof(struct vrmlSeparator),8728);
	if(!lr)goto ErrorOut;
	*lr=list;
	lr->whichChild=whichChild;
	lr->killObject=kSeparator;
	lr->duplicateObject=dSeparator;
	return (struct vrmlObjects *)lr;
ErrorOut:
	sprintf(WarningBuff,"Error in Separator\n");
	WarningBatch(WarningBuff);
	op=&list;
	freeVrmlObjectList(op);
	vrml=NULL;
	return vrml;
}
int freeVrmlObjectList(struct vrmlObjects *vrml)
{
	if(!vrml)return 1;
	if(vrml->killObject){
	    (*vrml->killObject)(vrml);
	}else{
	    sprintf(WarningBuff,"freeVrmlObjectList vrml->killObject NULL type %ld\n",vrml->type);
		WarningBatch(WarningBuff);
	}
	return 0;
}
static int kObject(struct vrmlObjects *vrml)
{

	if(!vrml)return 1;
	cFree((char *)vrml);
	return 0;
}
int putObjectName(char *name,struct vrmlObjects *vrml,struct Parser *p)
{
	struct checker *ck;
	void *op;
	char *nm;

	if(!name || !vrml || !p)return 1;

	op=&p->list;
	ck = op;

	if(check2(ck,sizeof(struct vrmlNamelist)))return 1;

	nm=strsave(name,8831);

	if(!nm)return 1;

	p->list[p->nameCount].name=nm;
	p->list[p->nameCount].objectNumber=p->nameCount+1;
	p->list[p->nameCount++].o=vrml;

	return 0;
}
int checkfix(struct checker *r,long length)
{
	char *data;

	if(!r)return 1;

	if(r->count <= 0){
	    if(r->data)cFree((char *)r->data);
	    r->data=NULL;
	    r->count=0;
	    r->countmax=0;
	    return 0;
	}

	data=NULL;
	if(r->data){
	    data=(char *)cRealloc((char *)r->data,r->count*length,8791);
	    if(!data){
	        WarningBatch("checkfix cRealloc out of Memory\n");
	        goto ErrorOut;
	    }
	    r->data=data;
	    r->countmax=r->count;
	}else{
	    WarningBatch("checkfix found no data\n");
	    goto ErrorOut;
	}
	return 0;
ErrorOut:
	return 1;
}
int check2(struct checker *r,long length)
{
	char *data;
	long countmax;

	if(!r)return 1;

	if(r->count+3 < r->countmax)return 0;

	countmax = r->countmax+POINTS_INCREMENT;

	data=NULL;
	if(r->data){
	    data=(char *)cRealloc((char *)r->data,countmax*length,8792);
	    if(!data){
	        WarningBatch("check out of Memory\n");
	        goto ErrorOut;
	    }
	    zerol((char *)(data+r->countmax*length),POINTS_INCREMENT*length);
	}else{
	    data=(char *)cMalloc(countmax*length,8728);
	    if(!data){
	        WarningBatch("check out of Memory");
	        goto ErrorOut;
	    }
	    zerol((char *)data,countmax*length);
	}

	r->countmax=countmax;
	r->data=data;

	return 0;
ErrorOut:
	if(data){
	    r->data=data;
	}
	return 1;
}
int EatIt(struct Parser *p,int flag)
{
	int count;
	int c;

	if(!p)return 1;

	if(flag){
	    sprintf(WarningBuff,"Eat %s\n",p->buff);
		if(0)WarningBatch(WarningBuff);
	}
	count = 1;
	while((c=next(p)) != EOF){
	    if(c == '{'){
	       ++count;
	    }else if(c == '}'){
	       if(--count == 0){
	           return 0;
	       }
	    }
	}
	if(c == EOF){
	    sprintf(WarningBuff,"EatIt Hit EOF\n");
		WarningBatch(WarningBuff);
	    return 1;
	}
	return 0;
}
int next(struct Parser *p)
{
	int n;
	int c;

	if(!p || !p->in)return EOF;
Start:
	if(white(p))return EOF;
	c=p->c;
	switch(c){
	    case '#':
	    while((c=fgetc(p->in)) != EOF && c != '\n' && c != '\r'){
	        ;
	    }
	    if(c == EOF)return EOF;
	    goto Start;
	    case '"':
	    n=0;
	    while((c=fgetc(p->in)) != EOF){
	        if(c == '\\'){
	           c=fgetc(p->in);
	           if(c == EOF)return EOF;
	        }else if(c == '"'){
	           break;
	        }
			if(n < BUFF_SIZE){
			   p->buff[n++]=c;
			}
	    }
	    p->buff[n++] = '\0';
	    return T_STRING;
	    case '{':
	    case '}':
	    case '[':
	    case ']':
	    case ',':
	    return c;
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	    case '8':
	    case '9':
	    case '-':
	    case '+':
	    case '.':
	    return vrmlNumber(c,p);
	}
	return vrmlName(c,p);
}
int vrmlNumber(int c,struct Parser *p)
{
	int n;
	if(!p || !p->in)return EOF;
	n=0;
	p->buff[n++]=c;
	if(c == '+' || c == '-'){
	    c=fgetc(p->in);
	    if(c == EOF)return EOF;
	    p->buff[n++]=c;
	}
	if(c == '0'){
	    c=fgetc(p->in);
	    if(c == EOF)return EOF;
	    p->buff[n++]=c;
	    if(c == 'x' || c == 'X'){
		while((c=fgetc(p->in)) != EOF){
		    if(!ishex(c))break;
		    p->buff[n++]=c;
		}
	        if(c == EOF)return EOF;
		ungetc(c,p->in);
		goto Out;
	    }if(c == 'e' || c == 'E' || c == '.'){
	        goto lfloat;
	    }if(!isdigit(c)){
	        --n;
		ungetc(c,p->in);
		goto Out;
	    }
	}else if(c == '.'){
lfloat:
		while((c=fgetc(p->in)) != EOF){
		    if(!isdigit(c))break;
		    p->buff[n++]=c;
		}
	        if(c == EOF)return EOF;
	        if(c != 'e' && c != 'E'){
		    ungetc(c,p->in);
		    goto Out;
	        }
		p->buff[n++]=c;
	        c=fgetc(p->in);
	        if(c == EOF)return EOF;
	        p->buff[n++]=c;
	        if(!isdigit(c) && c != '+' && c != '-'){
			    p->buff[n++]=c;
			    sprintf(WarningBuff,"vrmlNumber Bad Float %s\n",p->buff);
				WarningBatch(WarningBuff);
			    return EOF;
	        }
		while((c=fgetc(p->in)) != EOF){
		    if(!isdigit(c))break;
		    p->buff[n++]=c;
		}
	        if(c == EOF)return EOF;
		ungetc(c,p->in);
		goto Out;
	}
	while((c=fgetc(p->in)) != EOF){
	    if(!isdigit(c))break;
	    p->buff[n++]=c;
	}
	if(c == EOF)return EOF;
	if(c == '.'){
	    p->buff[n++]=c;
	    goto lfloat;
	}
	ungetc(c,p->in);
	if(c == 'e' || c == 'E')goto lfloat;
Out:
	if(c == EOF)return EOF;
	p->buff[n++]='\0';
	return T_NUMBER;
}
int ishex(int a)
{
	return(	(a >= '0' && a <= '9') ||
		(a >= 'a' && a <= 'f') ||
		(a >= 'A' && a <= 'F'));
}

int vrmlName(int c,struct Parser *p)
{
	int n;

	if(!p || !p->in)return EOF;

	n=0;
	p->buff[n++]=c;
	while((c=fgetc(p->in)) != EOF){
	   if(c >=0 && c <= 0x20){
			ungetc(c,p->in);
			goto Out;
	   }
	   switch(c){
		case 0x22:
		case 0x23:
		case 0x27:
		case 0x2c:
		case 0x2e:
		case 0x5b:
		case 0x5c:
		case 0x5d:
		case 0x7b:
		case 0x7d:
		case 0x7f:
		ungetc(c,p->in);
		goto Out;
	       default:
			if(n < BUFF_SIZE){
			   p->buff[n++]=c;
			}
	    }
	}
Out:
	if(c == EOF)return EOF;
	p->buff[n++]='\0';
	return T_NAME;
}
int white(struct Parser *p)
{
	int c;

	if(!p || !p->in)return 1;
	while((c=fgetc(p->in)) != EOF){
	    if(c == ' ' || c == '\t' || c == '\r' || c == '\n' ||
	       c == '\f')continue;
	    break;
	}
	if(c == EOF)return 1;
	p->c=c;
	return 0;
}
int getLine(char *Line,int length,FILE *in)
{
	int c;

	if(!Line || !in || (length <= 1))return 1;

	while(length-- > 1){
	   c=getc(in);
	   if(c == EOF){
	       return 1;
	   }else if(c == '\r'){
	       continue;
	   }else if(c == '\n'){
	       break;
	   }
	   *Line++ = c;
	}

	*Line++ = '\0';

	return 0;
}
