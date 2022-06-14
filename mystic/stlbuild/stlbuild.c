#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*
cc -O3 -arch i386 -o stlbuild stlbuild.c -L/usr/local/hdf/lib -ldf -ljpeg -lz -Wall
cc -O3 -o stlbuild stlbuild.c -ldf -ljpeg -lz -Wall
*/


struct P{
    double x;
    double y;
    double z;
};


struct System{
	struct P x;
	struct P y;
	struct P z;
	struct P p;
	double Theta_x;
	double Theta_y;
	double Theta_z;
	double Scale_x;
	double Scale_y;
	double Scale_z;
};
 

struct Matrix{
	double x[4][4];
};


typedef double Flt ;
typedef Flt Vec[3] ;

#define VecSub(a,b,c)	 (c)[0]=(a)[0]-(b)[0];\
			 (c)[1]=(a)[1]-(b)[1];\
			 (c)[2]=(a)[2]-(b)[2]

#define VecCross(a,b,c)	 (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1];\
			 (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2];\
			 (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
			 
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])

#define VecLen(a)	(sqrt(VecDot(a,a)))

#define VecNegate(a)	(a)[0]=0-(a)[0];\
			(a)[1]=0-(a)[1];\
			(a)[2]=0-(a)[2];

Flt VecNormalize(Vec vec);
			 
			 

typedef struct Icon {
      long ixmax;
      long iymax;
      long izmax;
      float xmin,xmax;
      float ymin,ymax;
      float zmin,zmax;
      float *data;
      char filename[256];
} *IconPtr;
      


struct Commands{
    char *name;
    int (*sub)(IconPtr myIcon,char command[][16],double *value,int *nword);
};

int doSphere(IconPtr myIcon,char command[][16],double *value,int *nword);

int doStl(IconPtr myIcon,char command[][16],double *value,int *nword);



struct Commands clist[]=
    {
	{"cube",doStl},
	{"box",doStl},
	{"sphere",doStl},
	{"torus",doStl},
	{"cone",doStl},
	{"disk",doStl},
    {"translate",doStl},
    {"rotate",doStl},
    {"scale",doStl},
    {"xyplane",doStl},
    {"xzplane",doStl},
    {"yzplane",doStl},
    {"sinegauge",doStl},
    {"triangle",doStl},
	{"limits",doSphere},
	{"name",doSphere},
	{"sphere",doSphere},
	{"torus",doSphere},
	{"cone",doSphere},
	{"disk",doSphere},
	{"raw",doSphere},
	{"ellipse",doSphere},
	{"box",doSphere},
	{"stop",doSphere},
    };

int NextLine(FILE *input,char *line,int linelength);
int closeSDS(IconPtr myIcon,int flag);
int getCommand(char *line,char command[][16],double *value,int *nword);
int doRenderKeys(char *line,IconPtr myIcon);
int zerol(char *p,unsigned long n);
int cFree(char *buffer);
char *cMalloc(unsigned long length);

int DFSDputdata(char *path,int rank,int *size,float *data); 
int DFSDlastref(void);
int DFANputdesc(char *file,unsigned int type,unsigned int tag,char *name,long lenght);

int DFerror;

int xyplaneStl(double xl, int nx, double yl, int ny, double np, double z);
int xzplaneStl(double xl,int nx, double zl, int nz, double np, double y);
int yzplaneStl(double yl,int ny, double zl, int nz, double np, double xh);

int writeTriangle(Vec *points, double np);

struct P Translation={0.0,0.0,0.0};
struct P Rotation={0.0,0.0,0.0};
struct P Scale={1.0,1.0,1.0};

int DiskWrite(int nx,int ny,double rin,double rout);

void BuildTransformMatrix(struct Matrix *m,struct System *p1,struct System *p2);
struct P Sub(struct P *p1,struct P *p2);
double Dot(struct P *v1,struct P *v2);

struct P p(double x,double y,double z);

struct System Global={
							{1.0,0.0,0.0},
							{0.0,1.0,0.0},
							{0.0,0.0,1.0},
							{0.0,0.0,0.0},
							 0.0,0.0,0.0,
							 1.0,1.0,1.0
	                 };	
	                 
int rotate3d(struct System *Local);

int	writeQuad(int nx,int ny,double *xdatal,double *ydatal,double *zdatal);
int ConeWrite(int nx,int ny,double rtop,double rbottom,double length);
int TorusWrite(int nx,int ny,double rbig,double rsmall);

double Len(struct P *v1);
struct P CrossN(struct P *v1,struct P *v2);
int SphereWrite(int nx,int ny,double radius);

int sinegauge(void);

int BoxWrite(double height,double width,double debth);

int writeTriangleP(struct P *p1,struct P *p2,struct P *p3);

int main(int argc,char **argv)
{
	struct Icon myIcon;
	char line[4096];
	FILE *input;

	zerol((char *)&myIcon,sizeof(struct Icon));

	input=fopen(argv[1],"r");
	if(input == NULL){
	    fprintf(stderr,"Could Not Open %s To Read\n",argv[1]);
	    exit(1);
	}

	while(1){
	    if(NextLine(input,line,sizeof(line)))break;
	    if(doRenderKeys(line,&myIcon))break;
	}


	return 0;
}


int doStl(IconPtr myIcon,char command[][16],double *value,int *nword)
{
	
	if(!myIcon)return 1;

	if(!strcmp("xyplane",command[*nword])){
		double xl,yl,np,z;
		int nx,ny;
	    ++(*nword);
	    xl=value[*nword];
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    yl=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    np=value[*nword];
	    ++(*nword);
	    z=value[*nword];

	    printf("xyplane xl %f nx %d yl %f ny %d np %f z %f\n",xl,nx,yl,ny,np,z);
	    
	    xyplaneStl(xl, nx, yl, ny, np, z);

	} else if(!strcmp("xzplane",command[*nword])){
		double xl,zl,np,y;
		int nx,nz;
	    ++(*nword);
	    xl=value[*nword];
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    zl=value[*nword];
	    ++(*nword);
	    nz=value[*nword];
	    ++(*nword);
	    np=value[*nword];
	    ++(*nword);
	    y=value[*nword];

	    printf("xzplane xl %f nx %d zl %f nz %d np %f y %f\n",xl,nx,zl,nz,np,y);
	    
	    xzplaneStl(xl, nx, zl, nz, np, y);
	} else if(!strcmp("yzplane",command[*nword])){
		double yl,zl,np,x;
		int ny,nz;
	    ++(*nword);
	    yl=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    zl=value[*nword];
	    ++(*nword);
	    nz=value[*nword];
	    ++(*nword);
	    np=value[*nword];
	    ++(*nword);
	    x=value[*nword];

	    printf("yzplane yl %f yx %d zl %f nz %d np %f x %f\n",yl,ny,zl,nz,np,x);
	    
	    yzplaneStl(yl, ny, zl, nz, np, x);
	} else if(!strcmp("translate",command[*nword])){
	    ++(*nword);
	    Translation.x=value[*nword];
	    ++(*nword);
	    Translation.y=value[*nword];
	    ++(*nword);
	    Translation.z=value[*nword];
	    printf("translate x %g y %g z %g \n",Translation.x,Translation.y,Translation.z);
	} else if(!strcmp("rotate",command[*nword])){
	    ++(*nword);
	    Rotation.x=value[*nword];
	    ++(*nword);
	    Rotation.y=value[*nword];
	    ++(*nword);
	    Rotation.z=value[*nword];
	    printf("rotate x %g y %g z %g \n",Rotation.x,Rotation.y,Rotation.z);
	} else if(!strcmp("scale",command[*nword])){
	    ++(*nword);
	    Scale.x=value[*nword];
	    ++(*nword);
	    Scale.y=value[*nword];
	    ++(*nword);
	    Scale.z=value[*nword];
	    printf("scale x %g y %g z %g \n",Scale.x,Scale.y,Scale.z);
	} else if(!strcmp("sphere",command[*nword])){
		double radius;
		int nx,ny;
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    radius=value[*nword];
	    printf("sphere nx %d ny %d radius %g \n",nx,ny,radius);
	    SphereWrite(nx,ny,radius);
	} else if(!strcmp("disk",command[*nword])){
		double rin,rout;
		int nx,ny;
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    rin=value[*nword];
	    ++(*nword);
	    rout=value[*nword];
	    printf("disk nx %d ny %d rin %g rout %g\n",nx,ny,rin,rout);
	    DiskWrite(nx,ny,rin,rout);
	} else if(!strcmp("torus",command[*nword])){
		double rbig,rsmall;
		int nx,ny;
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    rbig=value[*nword];
	    ++(*nword);
	    rsmall=value[*nword];
	    printf("torus nx %d ny %d rbig %g rsmall %g \n",nx,ny,rbig,rsmall);
	    TorusWrite(nx,ny,rbig,rsmall);
	} else if(!strcmp("cone",command[*nword])){
		double rtop,rbottom,length;
		int nx,ny;
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    rtop=value[*nword];
	    ++(*nword);
	    rbottom=value[*nword];
	    ++(*nword);
	    length=value[*nword];
	    printf("cone nx %d ny %d rtop %g rbottom %g length %g\n",nx,ny,rtop,rbottom,length);
	    ConeWrite(nx,ny,rtop,rbottom,length);
	} else if(!strcmp("box",command[*nword])){
		double height,width,debth;
	    ++(*nword);
	    height=value[*nword];
	    ++(*nword);
	    width=value[*nword];
	    ++(*nword);
	    debth=value[*nword];
	    printf("box height %g width %g debth %g\n",height,width,debth);
	    BoxWrite(height,width,debth);
	} else if(!strcmp("cube",command[*nword])){
		double height,width,debth;
	    ++(*nword);
	    height=value[*nword];
	    ++(*nword);
	    width=value[*nword];
	    ++(*nword);
	    debth=value[*nword];
	    printf("cube dx %g dy %g dz %g\n",height,width,debth);
	    BoxWrite(width,height,debth);
	} else if(!strcmp("triangle",command[*nword])){
		struct P p1,p2,p3;
	    ++(*nword);
	    p1.x=value[*nword];
	    ++(*nword);
	    p1.y=value[*nword];
	    ++(*nword);
	    p1.z=value[*nword];

	    ++(*nword);
	    p2.x=value[*nword];
	    ++(*nword);
	    p2.y=value[*nword];
	    ++(*nword);
	    p2.z=value[*nword];

	    ++(*nword);
	    p3.x=value[*nword];
	    ++(*nword);
	    p3.y=value[*nword];
	    ++(*nword);
	    p3.z=value[*nword];

	    printf("triangle p1 %f %f %f p2 %f %f %f p3 %f %f %f\n",
	            p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z);
	    
	    
	    writeTriangleP(&p1,&p2,&p3);

	} else if(!strcmp("sinegauge",command[*nword])){
		sinegauge();
	}
	
	
	return 0;
}

int writeTriangleO(FILE *out,struct P *p1,struct P *p2,struct P *p3)
{
	struct P v1,v2,normal;
	if(!out || !p1 || !p2 || !p3)return 1;
	
	if(p1->x == p2->x && p1->y == p2->y && p1->z == p2->z)return 1;
	if(p1->x == p3->x && p1->y == p3->y && p1->z == p3->z)return 1;
	if(p2->x == p3->x && p2->y == p3->y && p2->z == p3->z)return 1;
	
	v1=Sub(p2,p1);
	v2=Sub(p3,p1);
	normal=CrossN(&v1,&v2);
	
  	fprintf(out,"  facet normal %g %g %g\n", normal.x, normal.y, normal.z);
    fprintf(out,"    outer loop\n");
    
	fprintf(out,"      vertex %g %g %g\n",p1->x,p1->y,p1->z);
	fprintf(out,"      vertex %g %g %g\n",p2->x,p2->y,p2->z);
	fprintf(out,"      vertex %g %g %g\n",p3->x,p3->y,p3->z);
   
    fprintf(out,"    endloop\n");
  	fprintf(out,"  endfacet\n");
	
	return 0;
}

int writeQuadElements(FILE *out,double *xe,double *ye,double *ze)
{
	struct P p1,p2,p3;
	int n1,n2,n3,n4;
	
	n1=0;
	n2=1;
	n3=2;
	n4=3;

	p1=p(xe[n1],ye[n1],ze[n1]);
	p2=p(xe[n2],ye[n2],ze[n2]);
	p3=p(xe[n3],ye[n3],ze[n3]);
	writeTriangleO(out,&p1,&p2,&p3);
	
	p1=p(xe[n3],ye[n3],ze[n3]);
	p2=p(xe[n4],ye[n4],ze[n4]);
	p3=p(xe[n1],ye[n1],ze[n1]);
	writeTriangleO(out,&p1,&p2,&p3);
			

	return 0;
}


int sinegauge()
{
	int i,zonesPerCycle,zones;
	double t,t1,pi,length,x,w,dx,a,v,vv,vv2,f,freq;
	double xe[4],ye[4],ze[4];
	char name[256];
	FILE *out;
	
	
	zonesPerCycle=40;
	
	a=0.1;
	pi=4.0*atan(1.0);
	length=200;

	for(f=0.5;f<=20;f += 0.5){
	   freq=f;
	   	w=2*pi*freq/(0.5*length);
	   	zones=zonesPerCycle*freq;
		dx=length/(zones);
	   sprintf(name,"Sine%d.stl",(int)(2*freq+100));
	   printf("File %s freq %g w %g zones %d dx %g\n",name,freq,w,zones,dx);
	   
	   out=fopen(name,"w");
	   if(out == NULL){
	       printf("Error Opening %s to write\n",name);
	       return 1;
	   }
	   
		fprintf(out,"solid o1\n");
		for(i=0;i<zones;++i){
		   x=i*dx;
		   t = -log(0.5+0.25*sin(x*w))/a;
		   t1= -log(0.5+0.25*sin((x+dx)*w))/a;
		   v=exp(-a*t);
		   vv=0.5+0.25*sin(x*w);
		   vv2=0.5+0.25*sin((x+dx)*w);
		   printf("x %g x*w %g (x+dx)*w %g t %g v %g vv %g vv2 %g\n",x,x*w,(x+dx)*w,t,v,vv,vv2);
		   xe[0]=x-0.5*length;
		   xe[1]=x+dx-0.5*length;
		   xe[2]=x+dx-0.5*length;
		   xe[3]=x-0.5*length;
		   ye[0]=-50;
		   ye[1]=-50;
		   ye[2]=50;
		   ye[3]=50;
		   ze[0]=ze[1]=ze[2]=ze[3]=0;
		   writeQuadElements(out,xe,ye,ze);
		   ze[0]=ze[3]=t;
		   ze[1]=ze[2]=t1;
		   writeQuadElements(out,xe,ye,ze);
		   ye[2]=-50;
		   ye[3]=-50;
		   ze[0]=t;
		   ze[1]=t1;
		   ze[2]=ze[3]=0;
		   writeQuadElements(out,xe,ye,ze);
		   ye[0]=50;
		   ye[1]=50;
		   ye[2]=50;
		   ye[3]=50;
		   ze[0]=t;
		   ze[1]=t1;
		   ze[2]=ze[3]=0;
		   writeQuadElements(out,xe,ye,ze);
		}
	   
		fprintf(out,"endsolid o1\n");
	   
	   if(out)fclose(out);
	   
	}

	return 0;
}
int sinegaugeOLD()
{
	int i,zonesPerCycle,zones;
	double t,t1,pi,length,x,w,dx,a,v,vv,f,freq;
	double xe[4],ye[4],ze[4];
	char name[256];
	FILE *out;
	
	
	zonesPerCycle=20;
	
	a=0.1;
	pi=4.0*atan(1.0);
	length=200;

	for(f=0.5;f<=20;f += 0.5){
	   freq=f;
	   	w=2*pi*freq/(0.5*length);
	   	zones=zonesPerCycle*freq;
		dx=length/(zones);
	   sprintf(name,"Sine%d.stl",(int)(2*freq+100));
	   printf("File %s freq %g w %g zones %d dx %g\n",name,freq,w,zones,dx);
	   
	   out=fopen(name,"w");
	   if(out == NULL){
	       printf("Error Opening %s to write\n",name);
	       return 1;
	   }
	   
		fprintf(out,"solid o1\n");
		for(i=0;i<zones;++i){
		   x=i*dx;
		   t=(-log((2+sin(x*w))/3.0))/a;
		   // t /= 20;
		   t1=(-log((2+sin((x+dx)*w))/3.0))/a;
		   // t1 /= 20;
		   v=exp(-a*t);
		   vv=(2+sin(x*w))/3.0;
		   printf("x %g x*w %g (x+dx)*w %g t %g v %g vv %g\n",x,x*w,(x+dx)*w,t,v,vv);
		   xe[0]=x-0.5*length;
		   xe[1]=x+dx-0.5*length;
		   xe[2]=x+dx-0.5*length;
		   xe[3]=x-0.5*length;
		   ye[0]=-50;
		   ye[1]=-50;
		   ye[2]=50;
		   ye[3]=50;
		   ze[0]=ze[1]=ze[2]=ze[3]=-10*fabs(log(0.75));
		   writeQuadElements(out,xe,ye,ze);
		   ze[0]=ze[3]=t;
		   ze[1]=ze[2]=t1;
		   writeQuadElements(out,xe,ye,ze);
		   ye[2]=-50;
		   ye[3]=-50;
		   ze[0]=t;
		   ze[1]=t1;
		   ze[2]=ze[3]=-10*fabs(log(0.75));
		   writeQuadElements(out,xe,ye,ze);
		   ye[0]=50;
		   ye[1]=50;
		   ye[2]=50;
		   ye[3]=50;
		   ze[0]=t;
		   ze[1]=t1;
		   ze[2]=ze[3]=-10*fabs(log(0.75));
		   writeQuadElements(out,xe,ye,ze);
		}
	   
		fprintf(out,"endsolid o1\n");
	   
	   if(out)fclose(out);
	   
	}

	return 0;
}
int SphereWrite(int nx,int ny,double radius)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz,pi,dumy,dumx;
	struct Matrix BlockToScreen;
	double *xdatal,*ydatal,*zdatal;
	struct System Local;	
	int i,j,k;
	                     
	Local=Global;
	Local.p=Translation;
	Local.Theta_x=Rotation.x;
	Local.Theta_y=Rotation.y;
	Local.Theta_z=Rotation.z;
	Local.Scale_x=Scale.x;
	Local.Scale_y=Scale.y;
	Local.Scale_z=Scale.z;
	
	rotate3d(&Local);
	
	BuildTransformMatrix(&BlockToScreen,&Local,&Global);

	v1x=BlockToScreen.x[0][0]*Local.Scale_x;
	v1y=BlockToScreen.x[0][1]*Local.Scale_y;
	v1z=BlockToScreen.x[0][2]*Local.Scale_z;
	
	v2x=BlockToScreen.x[1][0]*Local.Scale_x;
	v2y=BlockToScreen.x[1][1]*Local.Scale_y;
	v2z=BlockToScreen.x[1][2]*Local.Scale_z;
	
	v3x=BlockToScreen.x[2][0]*Local.Scale_x;
	v3y=BlockToScreen.x[2][1]*Local.Scale_y;
	v3z=BlockToScreen.x[2][2]*Local.Scale_z;
	
	xc=BlockToScreen.x[0][3];
	yc=BlockToScreen.x[1][3];
	zc=BlockToScreen.x[2][3];
		
	xdatal=ydatal=zdatal=NULL;
	
	xdatal=(double *)malloc(nx*ny*sizeof(double));
	ydatal=(double *)malloc(nx*ny*sizeof(double));
	zdatal=(double *)malloc(nx*ny*sizeof(double));
	
	if(!xdatal || !ydatal || !zdatal)goto ErrorOut;
	
	k=0;
	pi=4*atan(1.0);
	dumy=1.0/((double)ny-1);
	dumx=1.0/((double)nx-1);
	for(j=0;j<ny;++j){
		double sinf,cosf;
		sinf=sin(pi*(1.-j*dumy));
		cosf=cos(pi*(1.-j*dumy));
		for(i=0;i<nx;++i){
		   dx=radius*cos(2*pi*(1.-i*dumx))*sinf;
		   dy=radius*sin(2*pi*(1.-i*dumx))*sinf;
		   dz=radius*cosf;
		   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		   ++k;
		}
	}
		
	writeQuad(nx,ny,xdatal,ydatal,zdatal);
	
ErrorOut:
	if(xdatal)free(xdatal);
	if(ydatal)free(ydatal);
	if(zdatal)free(zdatal);
	return 0;
}

int TorusWrite(int nx,int ny,double rbig,double rsmall)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz,pi,dumy,dumx;
	struct Matrix BlockToScreen;
	double *xdatal,*ydatal,*zdatal;
	struct System Local;	
	int i,j,k;
	                     
	Local=Global;
	Local.p=Translation;
	Local.Theta_x=Rotation.x;
	Local.Theta_y=Rotation.y;
	Local.Theta_z=Rotation.z;
	Local.Scale_x=Scale.x;
	Local.Scale_y=Scale.y;
	Local.Scale_z=Scale.z;
	
	rotate3d(&Local);
	
	BuildTransformMatrix(&BlockToScreen,&Local,&Global);

	v1x=BlockToScreen.x[0][0]*Local.Scale_x;
	v1y=BlockToScreen.x[0][1]*Local.Scale_y;
	v1z=BlockToScreen.x[0][2]*Local.Scale_z;
	
	v2x=BlockToScreen.x[1][0]*Local.Scale_x;
	v2y=BlockToScreen.x[1][1]*Local.Scale_y;
	v2z=BlockToScreen.x[1][2]*Local.Scale_z;
	
	v3x=BlockToScreen.x[2][0]*Local.Scale_x;
	v3y=BlockToScreen.x[2][1]*Local.Scale_y;
	v3z=BlockToScreen.x[2][2]*Local.Scale_z;
	
	xc=BlockToScreen.x[0][3];
	yc=BlockToScreen.x[1][3];
	zc=BlockToScreen.x[2][3];
		
	xdatal=ydatal=zdatal=NULL;
	
	xdatal=(double *)malloc(nx*ny*sizeof(double));
	ydatal=(double *)malloc(nx*ny*sizeof(double));
	zdatal=(double *)malloc(nx*ny*sizeof(double));
	
	if(!xdatal || !ydatal || !zdatal)goto ErrorOut;
	
	k=0;
	pi=4*atan(1.0);
	dumy=1.0/((double)ny-1);
	dumx=1.0/((double)nx-1);
	for(j=0;j<ny;++j){
		double sinf,cosf;
		sinf=sin(2*pi*(1.-j*dumy)-pi);
		cosf=cos(2*pi*(1.-j*dumy)-pi);
		for(i=0;i<nx;++i){
		   dx=(rbig+rsmall*cosf)*cos(2*pi*(1.-i*dumx));
		   dy=(rbig+rsmall*cosf)*sin(2*pi*(1.-i*dumx));
		   dz=rsmall*sinf;
		   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		   ++k;
		}
	}
		
	writeQuad(nx,ny,xdatal,ydatal,zdatal);
	
ErrorOut:
	if(xdatal)free(xdatal);
	if(ydatal)free(ydatal);
	if(zdatal)free(zdatal);
	return 0;
}

int BoxWrite(double height,double width,double depth)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz;
	struct Matrix BlockToScreen;
	double *xdatal,*ydatal,*zdatal;
	struct System Local;	
	int j;
   	double x[24],y[24],z[24];
    double h2,d2,w2;
	                     
	Local=Global;
	Local.p=Translation;
	Local.Theta_x=Rotation.x;
	Local.Theta_y=Rotation.y;
	Local.Theta_z=Rotation.z;
	Local.Scale_x=Scale.x;
	Local.Scale_y=Scale.y;
	Local.Scale_z=Scale.z;
	
	rotate3d(&Local);
	
	BuildTransformMatrix(&BlockToScreen,&Local,&Global);

	v1x=BlockToScreen.x[0][0]*Local.Scale_x;
	v1y=BlockToScreen.x[0][1]*Local.Scale_y;
	v1z=BlockToScreen.x[0][2]*Local.Scale_z;
	
	v2x=BlockToScreen.x[1][0]*Local.Scale_x;
	v2y=BlockToScreen.x[1][1]*Local.Scale_y;
	v2z=BlockToScreen.x[1][2]*Local.Scale_z;
	
	v3x=BlockToScreen.x[2][0]*Local.Scale_x;
	v3y=BlockToScreen.x[2][1]*Local.Scale_y;
	v3z=BlockToScreen.x[2][2]*Local.Scale_z;
	
	xc=BlockToScreen.x[0][3];
	yc=BlockToScreen.x[1][3];
	zc=BlockToScreen.x[2][3];
	
	
	xdatal=ydatal=zdatal=NULL;
	
	xdatal=(double *)malloc(24*sizeof(double));
	ydatal=(double *)malloc(24*sizeof(double));
	zdatal=(double *)malloc(24*sizeof(double));
	
	if(!xdatal || !ydatal || !zdatal)goto ErrorOut;
	
		
	h2=0.5*height;
	d2=0.5*depth;
	w2=0.5*width;
	
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
	
	if(!xdatal || !ydatal || !zdatal)goto ErrorOut;
	
	for(j=0;j<24;++j){
	    dx=x[j];
	    dy=y[j];
	    dz=z[j];
	    xdatal[j]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
	    ydatal[j]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
	    zdatal[j]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
	}
		
	printf("solid o1\n");
	for(j=0;j<24;j+=4){
		struct P p1,p2,p3;
		p1=p(xdatal[j],ydatal[j],zdatal[j]);
		p2=p(xdatal[j+1],ydatal[j+1],zdatal[j+1]);
		p3=p(xdatal[j+2],ydatal[j+2],zdatal[j+2]);
		writeTriangleP(&p1,&p2,&p3);
		p1=p(xdatal[j+2],ydatal[j+2],zdatal[j+2]);
		p2=p(xdatal[j+3],ydatal[j+3],zdatal[j+3]);
		p3=p(xdatal[j],ydatal[j],zdatal[j]);
		writeTriangleP(&p1,&p2,&p3);
	}
	printf("endsolid o1\n");
	
ErrorOut:
	if(xdatal)free(xdatal);
	if(ydatal)free(ydatal);
	if(zdatal)free(zdatal);
	return 0;

}
int ConeWrite(int nx,int ny,double rtop,double rbottom,double length)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz,pi,dumy,dumx;
	struct Matrix BlockToScreen;
	double *xdatal,*ydatal,*zdatal;
	struct System Local;	
	int i,j,k;
	                     
	Local=Global;
	Local.p=Translation;
	Local.Theta_x=Rotation.x;
	Local.Theta_y=Rotation.y;
	Local.Theta_z=Rotation.z;
	Local.Scale_x=Scale.x;
	Local.Scale_y=Scale.y;
	Local.Scale_z=Scale.z;
	
	rotate3d(&Local);
	
	BuildTransformMatrix(&BlockToScreen,&Local,&Global);

	v1x=BlockToScreen.x[0][0]*Local.Scale_x;
	v1y=BlockToScreen.x[0][1]*Local.Scale_y;
	v1z=BlockToScreen.x[0][2]*Local.Scale_z;
	
	v2x=BlockToScreen.x[1][0]*Local.Scale_x;
	v2y=BlockToScreen.x[1][1]*Local.Scale_y;
	v2z=BlockToScreen.x[1][2]*Local.Scale_z;
	
	v3x=BlockToScreen.x[2][0]*Local.Scale_x;
	v3y=BlockToScreen.x[2][1]*Local.Scale_y;
	v3z=BlockToScreen.x[2][2]*Local.Scale_z;
	
	xc=BlockToScreen.x[0][3];
	yc=BlockToScreen.x[1][3];
	zc=BlockToScreen.x[2][3];
		
	xdatal=ydatal=zdatal=NULL;
	
	xdatal=(double *)malloc(nx*ny*sizeof(double));
	ydatal=(double *)malloc(nx*ny*sizeof(double));
	zdatal=(double *)malloc(nx*ny*sizeof(double));
	
	if(!xdatal || !ydatal || !zdatal)goto ErrorOut;
	
	k=0;
	pi=4*atan(1.0);
	dumy=1.0/((double)ny-1);
	dumx=1.0/((double)nx-1);
	for(j=0;j<ny;++j){
		for(i=0;i<nx;++i){
		   dx=(rbottom+j*dumy*(rtop-rbottom))*cos(2*pi*(1.-i*dumx));
		   dy=(rbottom+j*dumy*(rtop-rbottom))*sin(2*pi*(1.-i*dumx));
		   dz=j*dumy*length;
		   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		   ++k;
		}
	}
		
	writeQuad(nx,ny,xdatal,ydatal,zdatal);
	
ErrorOut:
	if(xdatal)free(xdatal);
	if(ydatal)free(ydatal);
	if(zdatal)free(zdatal);
	return 0;
}
int DiskWrite(int nx,int ny,double rin,double rout)
{
	double v1x,v1y,v1z,v2x,v2y,v2z,v3x,v3y,v3z;
	double xc,yc,zc,dx,dy,dz,pi,dumy,dumx;
	struct Matrix BlockToScreen;
	double *xdatal,*ydatal,*zdatal;
	struct System Local;	
	int i,j,k;
	                     
	Local=Global;
	Local.p=Translation;
	Local.Theta_x=Rotation.x;
	Local.Theta_y=Rotation.y;
	Local.Theta_z=Rotation.z;
	Local.Scale_x=Scale.x;
	Local.Scale_y=Scale.y;
	Local.Scale_z=Scale.z;
	
	rotate3d(&Local);
	
	BuildTransformMatrix(&BlockToScreen,&Local,&Global);

/*
	printf("p.x %g p.y %g p.z %g \n",Global.p.x,Global.p.y,Global.p.z);
	printf("x.x %g x.y %g x.z %g \n",Global.x.x,Global.x.y,Global.x.z);
	printf("y.x %g y.y %g y.z %g \n",Global.y.x,Global.y.y,Global.y.z);
	printf("z.x %g z.y %g z.z %g \n",Global.z.x,Global.z.y,Global.z.z);

	printf("p.x %g p.y %g p.z %g \n",Local.p.x,Local.p.y,Local.p.z);
	printf("x.x %g x.y %g x.z %g \n",Local.x.x,Local.x.y,Local.x.z);
	printf("y.x %g y.y %g y.z %g \n",Local.y.x,Local.y.y,Local.y.z);
	printf("z.x %g z.y %g z.z %g \n",Local.z.x,Local.z.y,Local.z.z);
*/
	v1x=BlockToScreen.x[0][0]*Local.Scale_x;
	v1y=BlockToScreen.x[0][1]*Local.Scale_y;
	v1z=BlockToScreen.x[0][2]*Local.Scale_z;
	
	v2x=BlockToScreen.x[1][0]*Local.Scale_x;
	v2y=BlockToScreen.x[1][1]*Local.Scale_y;
	v2z=BlockToScreen.x[1][2]*Local.Scale_z;
	
	v3x=BlockToScreen.x[2][0]*Local.Scale_x;
	v3y=BlockToScreen.x[2][1]*Local.Scale_y;
	v3z=BlockToScreen.x[2][2]*Local.Scale_z;
	
	xc=BlockToScreen.x[0][3];
	yc=BlockToScreen.x[1][3];
	zc=BlockToScreen.x[2][3];
	
/*
	printf("v1x %g v1y %g v1z %g \n",v1x,v1y,v1z);
	printf("v2x %g v2y %g v2z %g \n",v2x,v2y,v2z);
	printf("v3x %g v3y %g v3z %g \n",v3x,v3y,v3z);
	printf("xc %g yc %g zc %g \n",xc,yc,zc);
*/

	xdatal=ydatal=zdatal=NULL;
	
	xdatal=(double *)malloc(nx*ny*sizeof(double));
	ydatal=(double *)malloc(nx*ny*sizeof(double));
	zdatal=(double *)malloc(nx*ny*sizeof(double));
	
	if(!xdatal || !ydatal || !zdatal)goto ErrorOut;
	
	k=0;
	pi=4*atan(1.0);
	dumy=1.0/((double)ny-1);
	dumx=1.0/((double)nx-1);
	for(j=0;j<ny;++j){
		for(i=0;i<nx;++i){
		   dx=(rin+j*dumy*(rout-rin))*cos(2*pi*(1.-i*dumx));
		   dy=(rin+j*dumy*(rout-rin))*sin(2*pi*(1.-i*dumx));
		   dz=0.0;
		   xdatal[k]=((dx)*v1x+(dy)*v1y+(dz)*v1z+xc);
		   ydatal[k]=((dx)*v2x+(dy)*v2y+(dz)*v2z+yc);
		   zdatal[k]=((dx)*v3x+(dy)*v3y+(dz)*v3z+zc);
		   ++k;
		}
	}
		
	writeQuad(nx,ny,xdatal,ydatal,zdatal);
	
ErrorOut:
	if(xdatal)free(xdatal);
	if(ydatal)free(ydatal);
	if(zdatal)free(zdatal);
	return 0;
}

int yzplaneStl(double yl,int ny, double zl, int nz, double np, double xh)
{
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dy=yl/ny;
	double dz=zl/nz;
	double x1,y1,z1;
	Vec points[3];
	int i1,j1,n;

	
	xmin=1e30;
	xmax=-1e30;
	
	ymin=1e30;
	ymax=-1e30;
	
	zmin=1e30;
	zmax=-1e30;
	
	x1=xh;
	
	printf("solid o1\n");
	
	for(j1=0;j1<nz;++j1)
	{
		z1=0.5*dz+j1*dz;
		for(i1=0;i1<ny;++i1)
		{
			y1=0.5*dy+i1*dy;
			
			points[0][0]=x1;
			points[1][0]=x1;
			points[2][0]=x1;
			
			points[0][1]=y1-0.5*dy-0.5*yl;
			points[1][1]=y1+0.5*dy-0.5*yl;
			points[2][1]=y1-0.5*yl;
			
			points[0][2]=z1-0.5*dz-0.5*zl;
			points[1][2]=z1-0.5*dz-0.5*zl;
			points[2][2]=z1-0.5*zl;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			
			
			writeTriangle(points,np);
			
			points[0][0]=x1;
			points[1][0]=x1;
			points[2][0]=x1;
			
			points[0][1]=y1-0.5*dy-0.5*yl;
			points[1][1]=y1-0.5*yl;
			points[2][1]=y1-0.5*dy-0.5*yl;
			
			points[0][2]=z1-0.5*dz-0.5*zl;
			points[1][2]=z1-0.5*zl;
			points[2][2]=z1+0.5*dz-0.5*zl;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
			
			points[0][0]=x1;
			points[1][0]=x1;
			points[2][0]=x1;
			
			points[0][1]=y1+0.5*dy-0.5*yl;
			points[1][1]=y1-0.5*dy-0.5*yl;
			points[2][1]=y1-0.5*yl;
			
			points[0][2]=z1+0.5*dz-0.5*zl;
			points[1][2]=z1+0.5*dz-0.5*zl;
			points[2][2]=z1-0.5*zl;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
			
			points[0][0]=x1;
			points[1][0]=x1;
			points[2][0]=x1;
		
			points[0][1]=y1+0.5*dy-0.5*yl;
			points[1][1]=y1-0.5*yl;
			points[2][1]=y1+0.5*dy-0.5*yl;
			
			points[0][2]=z1+0.5*dz-0.5*zl;
			points[1][2]=z1-0.5*zl;
			points[2][2]=z1-0.5*dz-0.5*zl;
			
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
		}
	}
	
	printf("endsolid o1\n");
	
	printf("xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g \n",xmin,xmax,ymin,ymax,zmin,zmax);
	
	
	return 0;
}
int writeTriangleP(struct P *p1,struct P *p2,struct P *p3)
{
	struct P v1,v2,normal;
	if(!p1 || !p2 || !p3)return 1;
	
	if(p1->x == p2->x && p1->y == p2->y && p1->z == p2->z)return 1;
	if(p1->x == p3->x && p1->y == p3->y && p1->z == p3->z)return 1;
	if(p2->x == p3->x && p2->y == p3->y && p2->z == p3->z)return 1;
	
	v1=Sub(p2,p1);
	v2=Sub(p3,p1);
	normal=CrossN(&v1,&v2);
	
  	printf("  facet normal %g %g %g\n", normal.x, normal.y, normal.z);
    printf("    outer loop\n");
    
	printf("      vertex %g %g %g\n",p1->x,p1->y,p1->z);
	printf("      vertex %g %g %g\n",p2->x,p2->y,p2->z);
	printf("      vertex %g %g %g\n",p3->x,p3->y,p3->z);
   
    printf("    endloop\n");
  	printf("  endfacet\n");
	
	
	
	return 0;
}
int	writeQuad(int nx,int ny,double *xdatal,double *ydatal,double *zdatal)
{
	struct P p1,p2,p3;
	int n1,n2,n3,n4;
	int i,j;
	
	if(!xdatal || !ydatal || !zdatal)return 1;
	
	printf("solid o1\n");
	for(j=0;j<ny-1;++j){
		for(i=0;i<nx-1;++i){
			n1=i+j*nx;
			n2=i+1+j*nx;
			n3=i+1+(j+1)*nx;
			n4=i+(j+1)*nx;
			p1=p(xdatal[n1],ydatal[n1],zdatal[n1]);
			p2=p(xdatal[n2],ydatal[n2],zdatal[n2]);
			p3=p(xdatal[n3],ydatal[n3],zdatal[n3]);
			writeTriangleP(&p1,&p2,&p3);
			
			p1=p(xdatal[n3],ydatal[n3],zdatal[n3]);
			p2=p(xdatal[n4],ydatal[n4],zdatal[n4]);
			p3=p(xdatal[n1],ydatal[n1],zdatal[n1]);
			writeTriangleP(&p1,&p2,&p3);
			
		}
	}
	    
	printf("endsolid o1\n");
	
	return 0;
}

int xzplaneStl(double xl,int nx, double zl, int nz, double np, double yh)
{
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dx=xl/nx;
	double dz=zl/nz;
	double x1,y1,z1;
	Vec points[3];
	int i1,j1,n;

	
	xmin=1e30;
	xmax=-1e30;
	
	ymin=1e30;
	ymax=-1e30;
	
	zmin=1e30;
	zmax=-1e30;
	
	y1=yh;
	
	printf("solid o1\n");
	
	for(j1=0;j1<nz;++j1)
	{
		z1=0.5*dz+j1*dz;
		for(i1=0;i1<nx;++i1)
		{
			x1=0.5*dx+i1*dx;
			
			points[0][0]=x1-0.5*dx-0.5*xl;
			points[1][0]=x1+0.5*dx-0.5*xl;
			points[2][0]=x1-0.5*xl;
			
			points[0][1]=y1;
			points[1][1]=y1;
			points[2][1]=y1;
			
			points[0][2]=z1-0.5*dz-0.5*zl;
			points[1][2]=z1-0.5*dz-0.5*zl;
			points[2][2]=z1-0.5*zl;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			
			
			writeTriangle(points,np);
			
			points[0][0]=x1-0.5*dx-0.5*xl;
			points[1][0]=x1-0.5*xl;
			points[2][0]=x1-0.5*dx-0.5*xl;
			
			points[0][1]=y1;
			points[1][1]=y1;
			points[2][1]=y1;
			
			points[0][2]=z1-0.5*dz-0.5*zl;
			points[1][2]=z1-0.5*zl;
			points[2][2]=z1+0.5*dz-0.5*zl;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
			
			points[0][0]=x1+0.5*dx-0.5*xl;
			points[1][0]=x1-0.5*dx-0.5*xl;
			points[2][0]=x1-0.5*xl;
			
			points[0][1]=y1;
			points[1][1]=y1;
			points[2][1]=y1;
			
			points[0][2]=z1+0.5*dz-0.5*zl;
			points[1][2]=z1+0.5*dz-0.5*zl;
			points[2][2]=z1-0.5*zl;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
			
			points[0][0]=x1+0.5*dx-0.5*xl;
			points[1][0]=x1-0.5*xl;
			points[2][0]=x1+0.5*dx-0.5*xl;
			
			points[0][1]=y1;
			points[1][1]=y1;
			points[2][1]=y1;
		
			points[0][2]=z1+0.5*dz-0.5*zl;
			points[1][2]=z1-0.5*zl;
			points[2][2]=z1-0.5*dz-0.5*zl;
			
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
		}
	}
	
	printf("endsolid o1\n");
	
	printf("xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g \n",xmin,xmax,ymin,ymax,zmin,zmax);
	
	
	return 0;
}

int xyplaneStl(double xl,int nx, double yl, int ny, double np, double zh)
{
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dx=xl/nx;
	double dy=yl/ny;
	double x1,y1,z1;
	Vec points[3];
	int i1,j1,n;

	
	xmin=1e30;
	xmax=-1e30;
	
	ymin=1e30;
	ymax=-1e30;
	
	zmin=1e30;
	zmax=-1e30;
	
	z1=zh;
	
	printf("solid o1\n");
	
	for(j1=0;j1<ny;++j1)
	{
		y1=0.5*dy+j1*dy;
		for(i1=0;i1<nx;++i1)
		{
			x1=0.5*dx+i1*dx;
			
			points[0][0]=x1-0.5*dx-0.5*xl;
			points[1][0]=x1+0.5*dx-0.5*xl;
			points[2][0]=x1-0.5*xl;
			
			points[0][1]=y1-0.5*dy-0.5*yl;
			points[1][1]=y1-0.5*dy-0.5*yl;
			points[2][1]=y1-0.5*yl;
			
			points[0][2]=z1;
			points[1][2]=z1;
			points[2][2]=z1;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			
			
			writeTriangle(points,np);
			
			points[0][0]=x1-0.5*dx-0.5*xl;
			points[1][0]=x1-0.5*xl;
			points[2][0]=x1-0.5*dx-0.5*xl;
			
			points[0][1]=y1-0.5*dy-0.5*yl;
			points[1][1]=y1-0.5*yl;
			points[2][1]=y1+0.5*dy-0.5*yl;
			
			points[0][2]=z1;
			points[1][2]=z1;
			points[2][2]=z1;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
			
			points[0][0]=x1+0.5*dx-0.5*xl;
			points[1][0]=x1-0.5*dx-0.5*xl;
			points[2][0]=x1-0.5*xl;
			
			points[0][1]=y1+0.5*dy-0.5*yl;
			points[1][1]=y1+0.5*dy-0.5*yl;
			points[2][1]=y1-0.5*yl;
			
			points[0][2]=z1;
			points[1][2]=z1;
			points[2][2]=z1;
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
			
			points[0][0]=x1+0.5*dx-0.5*xl;
			points[1][0]=x1-0.5*xl;
			points[2][0]=x1+0.5*dx-0.5*xl;
			
			points[0][1]=y1+0.5*dy-0.5*yl;
			points[1][1]=y1-0.5*yl;
			points[2][1]=y1-0.5*dy-0.5*yl;
			
			points[0][2]=z1;
			points[1][2]=z1;
			points[2][2]=z1;
		
			
			for(n=0;n<3;++n){
			    if(points[n][0] > xmax)xmax=points[n][0];
			    if(points[n][1] > ymax)ymax=points[n][1];
			    if(points[n][2] > zmax)zmax=points[n][2];
			    if(points[n][0] < xmin)xmin=points[n][0];
			    if(points[n][1] < ymin)ymin=points[n][1];
			    if(points[n][2] < zmin)zmin=points[n][2];
			}
			writeTriangle(points,np);
		}
	}
	
	printf("endsolid o1\n");
	
	printf("xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g \n",xmin,xmax,ymin,ymax,zmin,zmax);
	
	
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

int writeTriangle(Vec *points, double np)
{
    Vec v1,v2,normal;
    double area;
    
	VecSub(points[1],points[0],v1);
	VecSub(points[2],points[0],v2);
	VecCross(v1,v2,normal);
	area=0.5*VecNormalize(normal);
	if(np < 0.0){
	   VecNegate(normal);
	}
  	printf("  facet normal %g %g %g\n", normal[0], normal[1], normal[2]);
    printf("    outer loop\n");
    
	if(np < 0.0){
    	printf("      vertex %g %g %g\n",points[0][0],points[0][1],points[0][2]);
    	printf("      vertex %g %g %g\n",points[2][0],points[2][1],points[2][2]);
    	printf("      vertex %g %g %g\n",points[1][0],points[1][1],points[1][2]);
	}else{
    	printf("      vertex %g %g %g\n",points[0][0],points[0][1],points[0][2]);
    	printf("      vertex %g %g %g\n",points[1][0],points[1][1],points[1][2]);
    	printf("      vertex %g %g %g\n",points[2][0],points[2][1],points[2][2]);
	}
   
    printf("    endloop\n");
  	printf("  endfacet\n");
	
	return 0;
}
int NextLine(FILE *input,char *line,int linelength)
{
	int nc;
	int c;

	if(!input || !line || linelength <= 0)return 1;

	--linelength;

	nc=0;
	while(1){
	    c=fgetc(input);
	    if(c == '\n' || c == '\r' || c == EOF){
	        break;
	    }
	    if(nc >= linelength)break;
	    line[nc++] = c;
	}

	line[nc]=0;

	if(c == EOF && nc == 0){
	    return 1;
	}

	return 0;
}
int doRenderKeys(char *line,IconPtr myIcon)
{
	char command[256][16];
	double value[256];
	int k,n,nword,nList;

	if(!line || !myIcon)return 1;

	nList=sizeof(clist)/sizeof(struct Commands);

	getCommand(line,command,value,&nword);
	if(!nword)return 0;
	for(k=0;k<nword;++k){
		for(n=0;n<nList;++n){
			if(!strcmp(clist[n].name,command[k])){
				(*(clist[n].sub))(myIcon,command,value,&k);
				break;
			}
		}
		if(n >= nList){
				 printf("Unknown Command : %s\n",command[k]);
				 break;
		}
	}
	return 0;
}
int doSphere(IconPtr myIcon,char command[][16],double *value,int *nword)
{
	long ixmax,iymax,izmax;
	float xmin,ymin,zmin;
	float dx,dy,dz;
	float x,y,z;
	float *data;
	long i,j,k;

	if(!myIcon)return 1;

	ixmax=myIcon->ixmax;
	iymax=myIcon->iymax;
	izmax=myIcon->izmax;

	xmin=myIcon->xmin;
	ymin=myIcon->ymin;
	zmin=myIcon->zmin;

	if(ixmax){
	    dx=(myIcon->xmax-xmin)/(float)(ixmax);
	}else{
	    dx=0;
	}

	if(iymax){
	    dy=(myIcon->ymax-ymin)/(float)(iymax);
	}else{
	    dy=0;
	}


	if(izmax){
	    dz=(myIcon->zmax-zmin)/(float)(izmax);
	}else{
	    dz=0;
	}

	data=myIcon->data;

	if(!strcmp("sphere",command[*nword])){
	    float xc,yc,zc,r,r2,density,radius2;
	    ++(*nword);
	    xc=value[*nword];
	    ++(*nword);
	    yc=value[*nword];
	    ++(*nword);
	    zc=value[*nword];
	    ++(*nword);
	    r=value[*nword];
	    r2=r*r;
	    ++(*nword);
	    density=value[*nword];

	    printf("Sphere xc %f yc %f zc %f r %f density %f\n",xc,yc,zc,r,density);

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                radius2=(x-xc)*(x-xc)+(y-yc)*(y-yc)+(z-zc)*(z-zc);
	                if(radius2 <= r2){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("box",command[*nword])){
	    float xc1,yc1,zc1,xc2,yc2,zc2,density;
	    ++(*nword);
	    xc1=value[*nword];
	    ++(*nword);
	    yc1=value[*nword];
	    ++(*nword);
	    zc1=value[*nword];
	    ++(*nword);
	    xc2=value[*nword];
	    ++(*nword);
	    yc2=value[*nword];
	    ++(*nword);
	    zc2=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    printf("box xc1 %f yc1 %f zc1 %f \n        xc2 %f yc2 %f zc2 %f\n            density %f\n",
	           xc1,yc1,zc1,xc2,yc2,zc2,density);

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                if(x >= xc1 && x <= xc2 && y >= yc1 && y <= yc2 && z >= zc1 && z <= zc2){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("ellipse",command[*nword])){
	    float xc1,yc1,zc1,xc2,yc2,zc2,r,density,ra,rb;
	    ++(*nword);
	    xc1=value[*nword];
	    ++(*nword);
	    yc1=value[*nword];
	    ++(*nword);
	    zc1=value[*nword];
	    ++(*nword);
	    xc2=value[*nword];
	    ++(*nword);
	    yc2=value[*nword];
	    ++(*nword);
	    zc2=value[*nword];
	    ++(*nword);
	    r=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    printf("ellipse xc1 %f yc1 %f zc1 %f \n        xc2 %f yc2 %f zc2 %f\n         r %f density %f\n",
	           xc1,yc1,zc1,xc2,yc2,zc2,r,density);

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                ra=sqrt((x-xc1)*(x-xc1)+(y-yc1)*(y-yc1)+(z-zc1)*(z-zc1));
	                rb=sqrt((x-xc2)*(x-xc2)+(y-yc2)*(y-yc2)+(z-zc2)*(z-zc2));
	                if((ra+rb) <= r){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("cone",command[*nword])){
	    float xc1,yc1,zc1,xc2,yc2,zc2,r1,r2,den,ra2;
	    float cx,cy,cz,nx,ny,nz,vx,vy,vz,dr,vr,dot;
	    float density,s;
	    ++(*nword);
	    xc1=value[*nword];
	    ++(*nword);
	    yc1=value[*nword];
	    ++(*nword);
	    zc1=value[*nword];
	    ++(*nword);
	    xc2=value[*nword];
	    ++(*nword);
	    yc2=value[*nword];
	    ++(*nword);
	    zc2=value[*nword];
	    ++(*nword);
	    r1=value[*nword];
	    ++(*nword);
	    r2=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    cx=xc2-xc1;
	    cy=yc2-yc1;
	    cz=zc2-zc1;
	    dr=r2-r1;

	    printf("cone    xc1 %f yc1 %f zc1 %f \n        xc2 %f yc2 %f zc2 %f\n         r1 %f r2 %f density %f\n",
	           xc1,yc1,zc1,xc2,yc2,zc2,r1,r2,density);

	    den = sqrt(cx*cx+cy*cy+cz*cz);

	    if(den <= 0){
	        fprintf(stderr,"Error cone end points must be different");
	        return 1;
	    }

	    nx=cx/den;
	    ny=cy/den;
	    nz=cz/den;

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
					dot=nx*(x-xc1)+ny*(y-yc1)+nz*(z-zc1);
					if(dot < 0)continue;
					s=dot/den;
					if(s > 1.0)continue;
					vx=s*cx+xc1;
					vy=s*cy+yc1;
					vz=s*cz+zc1;
					vr=r1+s*dr;
					ra2=(x-vx)*(x-vx)+(y-vy)*(y-vy)+(z-vz)*(z-vz);
	                if((ra2) <= (vr*vr)){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("torus",command[*nword])){
	    float xc,yc,zc,r1,r2,r22,den;
	    float ax,ay,az,nx,ny,nz,dotn,dotx;
	    float density;
	    ++(*nword);
	    xc=value[*nword];
	    ++(*nword);
	    yc=value[*nword];
	    ++(*nword);
	    zc=value[*nword];
	    ++(*nword);
	    nx=value[*nword];
	    ++(*nword);
	    ny=value[*nword];
	    ++(*nword);
	    nz=value[*nword];
	    ++(*nword);
	    r1=value[*nword];
	    ++(*nword);
	    r2=value[*nword];
	    ++(*nword);
	    density=value[*nword];

	    r22=r2*r2;


	    printf("torus   xc %f yc %f zc %f \n        nx %f ny %f nz %f\n         rmajor %f rminor %f density %f\n",
	           xc,yc,zc,nx,ny,nz,r1,r2,density);

	    den = sqrt(nx*nx+ny*ny+nz*nz);
	    if(den <= 0){
	        fprintf(stderr,"torus has zero normal\n");
	        return 1;
	    }

	    nx=nx/den;
	    ny=ny/den;
	    nz=nz/den;

	    for(k=0;k<izmax;++k){
	        z=zmin+((double)k+.5)*dz;
	        for(j=0;j<iymax;++j){
	            y=ymin+((double)j+.5)*dy;
	            for(i=0;i<ixmax;++i){
	                x=xmin+((double)i+.5)*dx;
	                ax=(x-xc);
	                ay=(y-yc);
	                az=(z-zc);

					dotn=nx*ax+ny*ay+nz*az;
		
					ax -= dotn*nx;
					ay -= dotn*ny;
					az -= dotn*nz;
		
					dotx=sqrt(ax*ax+ay*ay+az*az);
		
					dotx=r1-dotx;
	                if((dotn*dotn+dotx*dotx) <= r22){
	                    data[i+j*ixmax+k*ixmax*iymax]=density;
	                }
	            }
	        }
	    }
	}else if(!strcmp("disk",command[*nword])){
	    ;
	}else if(!strcmp("raw",command[*nword])){
	    unsigned char bout;
	    char *name;
	    FILE *out;
	    name="raw.raw";
	    out=fopen(name,"wb");
	    if(!out)
	    {
	    	printf("Error opening %s to write\n",name);
	    	return 0;
	    }
	    for(k=0;k<izmax;++k){
	        for(j=0;j<iymax;++j){
	            for(i=0;i<ixmax;++i){
	            	bout=data[i+j*ixmax+k*ixmax*iymax];
	            	fwrite(&bout,1,1,out);
	            }
	        }
	    }
	    printf("Write %s %ld %ld %ld\n",name,ixmax,iymax,izmax);
	}else if(!strcmp("limits",command[*nword])){
	    ++(*nword);
	    myIcon->xmin=value[*nword];
	    ++(*nword);
	    myIcon->xmax=value[*nword];
	    ++(*nword);
	    myIcon->ymin=value[*nword];
	    ++(*nword);
	    myIcon->ymax=value[*nword];
	    ++(*nword);
	    myIcon->zmin=value[*nword];
	    ++(*nword);
	    myIcon->zmax=value[*nword];
	    printf("limits xmin %f ymin %f zmin %f\n",
	            myIcon->xmin,myIcon->ymin,myIcon->zmin);
	    printf("       xmax %f ymax %f zmax %f\n",
	            myIcon->xmax,myIcon->ymax,myIcon->zmax);
	}else if(!strcmp("name",command[*nword])){
	    long length;
	    if(myIcon->data){
	        closeSDS(myIcon,0);
	    }
	    ++(*nword);
	    strncpy(myIcon->filename,command[*nword],256);
	    ++(*nword);
	    myIcon->ixmax=(long)value[*nword];
	    ++(*nword);
	    myIcon->iymax=(long)value[*nword];
	    ++(*nword);
	    myIcon->izmax=(long)value[*nword];

	    myIcon->xmin=0;
	    myIcon->ymin=0;
	    myIcon->zmin=0;

	    myIcon->xmax=(float)myIcon->ixmax;
	    myIcon->ymax=(float)myIcon->iymax;
	    myIcon->zmax=(float)myIcon->izmax;

	    printf("filename %s ixmax %ld iymax %ld izmax %ld\n",
	            myIcon->filename,myIcon->ixmax,myIcon->iymax,myIcon->izmax);
	    length=myIcon->ixmax*myIcon->iymax*myIcon->izmax;
	    myIcon->data=(float *)cMalloc(length*sizeof(float));
	    if(!myIcon->data){
	        fprintf(stderr,"file %s length %ld Out of Memory\n",
	                myIcon->filename,length*sizeof(float));
	        return 1;
	    }
	    zerol((char *)myIcon->data,length*sizeof(float));
	}else if(!strcmp("stop",command[*nword])){
	    if(myIcon->data)closeSDS(myIcon,1);
	    exit(0);
	}
	return 0;
}
int closeSDS(IconPtr myIcon,int flag)
{
	extern int DFerror;
	char outbuff[2048];
	int lastref;
	int rank,size[3];
	long length;
	int ret;

	rank=3;
	size[0]=(int)myIcon->ixmax;
	size[1]=(int)myIcon->iymax;
	size[2]=(int)myIcon->izmax;
	length=size[0]*size[1]*size[2];
	if(length <= 0 || !myIcon->data){
            if(flag)
              fprintf(stderr,"DFSDputdata error File: %s length %ld data %lx\n",
                     myIcon->filename,length,(unsigned long)myIcon->data);
	    return 1;
	}

	if((ret=DFSDputdata(myIcon->filename,rank,size,myIcon->data))){
            fprintf(stderr,"DFSDputdata error File: %s ret %ld\n",myIcon->filename,(long)ret);
            return 1;
	}

	sprintf(outbuff,"xmin %g xmax %g ymin %g ymax %g zmin %g zmax %g%c",
	       myIcon->xmin,myIcon->xmax,myIcon->ymin,myIcon->ymax,myIcon->zmin,myIcon->zmax,0);

	lastref=DFSDlastref();

	if(lastref < 0){
	    fprintf(stderr,"DFSDlastref %s lastref %d error %d",
		               myIcon->filename,lastref,DFerror);
	    return 1;
	}

	if(DFANputdesc(myIcon->filename,306,lastref,outbuff,(int)strlen(outbuff)) == -1){
		   fprintf(stderr,"DFANputdesc %s lastref %d error %d",
		               myIcon->filename,lastref,DFerror);
			return 1;
	}

	cFree((char *)myIcon->data);
	myIcon->data=NULL;
	return 0;
}
int getCommand(char *line,char command[][16],double *value,int *nword)
{
	static char number[]={
			'0','1','2','3','4','5','6','7','8','9',
			'+','-','.'};
	char buff[256];
	int c,n,nw,iret,k,inum;

	for(n=0;n<256;++n){
	    value[n]=0;
	    strcpy(command[n],"");
	}

	nw=0;
do{
	n=0;
	while((c = *line++) != 0 && c != ' ' && c != '\n' && c != '\r'
				     && c != EOF  && n < 15 ){
	    buff[n++]=c;
	}
	if(c == EOF){
	    strcpy(command[0],"stop");
	    *nword = 1;
	    return 1;	    
	}else if(c == ' '){
	    iret = 0;
	}else{
	    iret = 1;
	}

	buff[n]=0;

	if(!strcmp(buff,".") || !strcmp(buff,"..")){
	    strcpy(command[nw],buff);
	    continue;
	}

	inum = 0;
	for(k=0;k<sizeof(number);++k){
	    if(*buff == number[k]){
	        inum = 1;
	        break;
	    }
	}

	if(inum){
	     value[nw] = atof(buff);
	}else{
	    strcpy(command[nw],buff);
	}

    }while((++nw < 256) &&  !iret);

    *nword = nw;

    return 0;
}
int zerol(char *p,unsigned long n)
{
	if(!p)return 1;
	while(n-- > 0)*p++ = 0;
	return 0;
}
char *cMalloc(unsigned long length)
{
	char *buffer;

	buffer=malloc(length);

	return buffer;
}
int cFree(char *buffer)
{
    if(!buffer)return 1;
    free(buffer);
    return 0;
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
int rotate3d(struct System *Local)
{
	static struct P v1={1.,0.,0.};
	static struct P v2={0.,1.,0.};
	static struct P v3={0.,0.,1.};
	double xx,xy,xz,yx,yy,yz,zx,zy,zz;
	double c1,c2,c3,s1,s2,s3;
	double tx,ty,tz,dtor;
	double x,y,z;
	double xo[3],yo[3],zo[3],xn[3],yn[3],zn[3];
	double rx,ry,rz;
	long n;

	rx=Local->Theta_x;
	ry=Local->Theta_y;
	rz=Local->Theta_z;

	xo[0]=v1.x;
	yo[0]=v1.y;
	zo[0]=v1.z;

	xo[1]=v2.x;
	yo[1]=v2.y;
	zo[1]=v2.z;

	xo[2]=v3.x;
	yo[2]=v3.y;
	zo[2]=v3.z;

	dtor=atan(1.0)/45.;

	tx=dtor*rx;
	ty=dtor*ry;
	tz=dtor*rz;

	c1=cos(tz);
	s1=sin(tz);
	c2=cos(ty);
	s2=sin(ty);
	c3=cos(tx);
	s3=sin(tx);

	xx=c1*c2;
	xy=s3*c1*s2-s1*c3;
	xz=s1*s3+c1*s2*c3;

	yx=s1*c2;
	yy=c1*c3+s1*s2*s3;
	yz = -s3*c1+s1*s2*c3;

	zx = -s2;
	zy = s3*c2;
	zz = c2*c3;

	for(n=0;n<3;++n){
	    x=xo[n];
	    y=yo[n];
	    z=zo[n];
	    xn[n] = xx*x+xy*y+xz*z;
	    yn[n] = yx*x+yy*y+yz*z;
	    zn[n] = zx*x+zy*y+zz*z;
	}

	Local->x=p(xn[0],yn[0],zn[0]);
	Local->y=p(xn[1],yn[1],zn[1]);
	Local->z=p(xn[2],yn[2],zn[2]);
	return 0;

}
double Dot(struct P *v1,struct P *v2)
{
    double Length;

    Length=(v1->x*v2->x)+(v1->y*v2->y)+(v1->z*v2->z);

    return  Length;
}
struct P Sub(struct P *p1,struct P *p2)
{
    struct P psub;

    psub.x=p1->x-p2->x;
    psub.y=p1->y-p2->y;
    psub.z=p1->z-p2->z;
    return psub;
 
}
struct P p(double x,double y,double z)
{
	struct P ret;
	
	ret.x=x;
	ret.y=y;
	ret.z=z;
	return ret;
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
double Len(struct P *v1)
{
    double Lenr;

    Lenr=pow((v1->x)*(v1->x)+
            (v1->y)*(v1->y)+
            (v1->z)*(v1->z),.5);

    return  Lenr;
}

