#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "showlib.h"
// cc -c showlib.c
// c++ -o readstl readstl05.cpp showlib.o -Wall

struct P{
    double x;
    double y;
    double z;
};

double Len(struct P *v1);
struct P p(double x,double y,double z);
struct P CrossN(struct P *v1,struct P *v2,double *len);
struct P Sub(struct P *p1,struct P *p2);
struct P Norm(struct P *p1);
struct P Add(struct P *p1,struct P *p2);
struct P Mult(struct P *p1,double Factor);

int readstl(),mass1(),march();

double *bxdata,*bydata,*bzdata;
long *bElements;
double *x0,*xv0,*x1;
double *yy0,*yyv0,*yy1;
double *z0,*zv0,*z1;
double *mass;

long ElementCount;
long NodeCount;

double xmin,xmax,ymin,ymax,zmin,zmax;

showPtr s;

int main(int arg,char *argv[])
{
	s=showStart((char *)"data.shw",(char *)"w");
	if(!s)return 1;
	readstl();
	printf("NodeCount %ld ElementCount %ld\n ",NodeCount,ElementCount);
	mass1();
	showEnd(s);
	march();
}
int march()
{
	struct P ps,pe,pv,hole,pl; 
	double v,dx,dt;
	double t,fm,g,massh;
	long nstep=200;
	char name[256];
	
	g=6.67e-11;
	v=1.16e6;
	dx=(137886+319012)/nstep;
	dt=dx/v;
	
	massh=5.94e26;

	pe=p(-319012,33997.3,-146197.0);		            
	ps=p(137886,33997.3,-146197.0);	
	pv=Sub(&pe,&ps);
	pv=Norm(&pv);	
	
	for(int k=0;k<nstep;++k){
	    pl=Mult(&pv,k*dx);
	    hole=Add(&ps,&pl);
		t=dt*k;
		s->time=t;
		printf("%g %g %g %g %g %g\n",x0[0],yy0[0],z0[0],xv0[0],yyv0[0],zv0[0]);
		for(int n=0;n<NodeCount;++n){
		   struct P pn,rv;
		   double r;
		   pn=p(x0[n],yy0[n],z0[n]);
		   rv=Sub(&hole,&pn);
		   r=Len(&rv);
		   rv=Norm(&rv);
		   fm=0;
		   if(r){
		   	   fm=g*massh/(r*r);
		   }
		   x1[n]=x0[n]+xv0[n]*dt+0.5*fm*dt*dt*rv.x;
		   yy1[n]=yy0[n]+yyv0[n]*dt+0.5*fm*dt*dt*rv.y;
		   z1[n]=z0[n]+zv0[n]*dt+0.5*fm*dt*dt*rv.z;
		   xv0[n]=(x1[n]-x0[n])/dt;
		   yyv0[n]=(yy1[n]-yy0[n])/dt;
		   zv0[n]=(z1[n]-z0[n])/dt;
		   x0[n]=x1[n];
		   yy0[n]=yy1[n];
		   z0[n]=z1[n];
		}
		sprintf(name,"data%04d.shw",k);
		s=showStart(name,(char *)"w");
		if(!s)return 1;
		showNodes(s,NodeCount,x1,yy1,z1);
		showElements(s,ElementCount,3L,bElements);
		showEnd(s);
		showDone(s);
	}            

	return 0;
}
int mass1()
{
	long n1,n2,n3;
	struct P p1,p2,p3; 
	
	double masst=0;
	
	mass=(double *)malloc(NodeCount*sizeof(double));
	if(!mass){
		return 1;
	}
	x0=(double *)malloc(NodeCount*sizeof(double));
	if(!x0){
		return 1;
	}
	yy0=(double *)malloc(NodeCount*sizeof(double));
	if(!yy0){
		return 1;
	}
	z0=(double *)malloc(NodeCount*sizeof(double));
	if(!z0){
		return 1;
	}
	x1=(double *)malloc(NodeCount*sizeof(double));
	if(!x1){
		return 1;
	}
	yy1=(double *)malloc(NodeCount*sizeof(double));
	if(!yy1){
		return 1;
	}
	z1=(double *)malloc(NodeCount*sizeof(double));
	if(!z1){
		return 1;
	}
	xv0=(double *)malloc(NodeCount*sizeof(double));
	if(!xv0){
		return 1;
	}
	
	yyv0=(double *)malloc(NodeCount*sizeof(double));
	if(!yyv0){
		return 1;
	}
	
	zv0=(double *)malloc(NodeCount*sizeof(double));
	if(!zv0){
		return 1;
	}
	
	for(int k=0;k<NodeCount;++k){
		mass[k]=0.0;
		x0[k]=bxdata[k];
		yy0[k]=bydata[k];
		z0[k]=bzdata[k];
		xv0[k]=0;
		yyv0[k]=0;
		zv0[k]=0;
		x1[k]=0;
		yy1[k]=0;
		z1[k]=0;
	}

	showNodes(s,NodeCount,bxdata,bydata,bzdata);
	showElements(s,ElementCount,3L,bElements);
	
	xmin=1e33;
	xmax= -1e33;
	ymin=1e33;
	ymax= -1e33;
	zmin=1e33;
	zmax= -1e33;
	
	for(int k=0;k<ElementCount;++k){
	    double x,y,z;
		double len;
	    n1=bElements[k*4+1];
	    n2=bElements[k*4+2];
	    n3=bElements[k*4+3];
	    x=bxdata[n1];
	    if(x < xmin)xmin=x;
	    if(x > xmax)xmax=x;
	    y=bydata[n1];
	    if(y < ymin)ymin=y;
	    if(y > ymax)ymax=y;
	    z=bzdata[n1];
	    if(z < zmin)zmin=z;
	    if(z > zmax)zmax=z;
	    p1=p(bxdata[n1],bydata[n1],bzdata[n1]);		            
	    p2=p(bxdata[n2],bydata[n2],bzdata[n2]);		            
	    p3=p(bxdata[n3],bydata[n3],bzdata[n3]);		
	    CrossN(&p1,&p2,&len);  
	    if(len == 0)continue;
	    mass[n1]+=0.5*len;  
	    mass[n2]+=0.5*len;  
	    mass[n3]+=0.5*len;  
	    masst+=0.5*len; 
	    CrossN(&p1,&p3,&len);  
	    if(len == 0)continue;
	    mass[n1]+=0.5*len;  
	    mass[n2]+=0.5*len;  
	    mass[n3]+=0.5*len;  
	    masst+=0.5*len; 
	}
	
	printf("mass total %g %g %g %g %g %g %g\n",masst,xmin,xmax,ymin,ymax,zmin,zmax);

	return 0;
}
int readstl()
{
	double *xdata,*ydata,*zdata;
	char buff[256];
	double xx,yy,zz;
	long *Elements;
	long nelem4;
	long nn;
	long k;
	int nc;
	int n;
	int c;
	int ret;
	
	FILE *in;
	
	char *name=(char *)"/Users/dir/images/manhattan.stl";
	
	in=fopen(name,"rb");
	if(!in){
	   printf("Could not open %s to Read\n",name);
	   return 1;
	}
	
	bxdata=NULL;
	bydata=NULL;
	bzdata=NULL;
    bElements=NULL;
	ret=1;

	NodeCount=0;
	
	ElementCount=0;
	
	int NodesMax=3;
		
	nn=0;
	nc=0;
	nelem4=0;

	while(fgets(buff,256,in)){
		for(n=0;n<256;++n){
		    c=buff[n];
		    if(c == 'v' && buff[n+1] == 'e' && buff[n+2] == 'r'){
		        sscanf(&buff[n]+6,"%lf %lf %lf",&xx,&yy,&zz);
		        if(nn+1 >= NodeCount){
		            NodeCount += 2*NodeCount+10000;
					xdata=(double *)malloc(NodeCount*sizeof(double));
					ydata=(double *)malloc(NodeCount*sizeof(double));
					zdata=(double *)malloc(NodeCount*sizeof(double));
					if(!xdata || !ydata || !zdata)goto ErrorOut;
					
					if(bxdata && bydata && zdata){
						for(k=0;k<nn;++k){
						   xdata[k]=bxdata[k];
						   ydata[k]=bydata[k];
						   zdata[k]=bzdata[k];
						}
					}
					
					if(bxdata)free((char *)bxdata);
					if(bydata)free((char *)bydata);
					if(bzdata)free((char *)bzdata);
					bxdata=xdata;
					bydata=ydata;
					bzdata=zdata;
		        
		        }
	            bxdata[nn]=xx;
	            bydata[nn]=yy;
	            bzdata[nn++]=zz;		
		        if(++nc == 3){
		            nc = 0;
		            if(nelem4+5 >= ElementCount*(NodesMax+1)){
		                ElementCount += 2*ElementCount+10000;
		            
						Elements=(long *)malloc(ElementCount*(NodesMax+1)*sizeof(long));
						if(!Elements)goto ErrorOut;
		            
		            	if(bElements){
							for(k=0;k<nelem4;++k){
							   Elements[k]=bElements[k];
							}
						}
						if(bElements)free((char *)bElements);
						bElements=Elements;
		            }
	                bElements[nelem4++] = 1;
	                bElements[nelem4++] = nn-3;
	                bElements[nelem4++] = nn-2;
	                bElements[nelem4++] = nn-1;		            
		        }   
		        break;
		    }else if(c == 's'  && buff[n+1] == 'o' && buff[n+2] == 'l'){
		       /* ojbect++; */
		        break;
		    }else if(n == 2 && c == 'f' && buff[n+1] == 'a' && buff[n+2] == 'c'){
		       /* sscanf(&buff[n]+12,"%lf %lf %lf",&nxx,&nyy,&nzz); */
		        break;
		    }
		}
	}

	ret=0;
ErrorOut:

	if(!ret){
		NodeCount=nn;
	
		ElementCount=nelem4/4;
	}

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
struct P CrossN(struct P *v1,struct P *v2,double *len)
{
    static struct P Zero;
    struct P Vcross;
    double Length;
    
    *len=0;

    Vcross.x =   v1->y*v2->z-v2->y*v1->z;
    Vcross.y = -(v1->x*v2->z-v2->x*v1->z);
    Vcross.z =   v1->x*v2->y-v2->x*v1->y;
    Length=Len(&Vcross);
    if(Length <= 0.)return Zero;
    Vcross.x/=Length;
    Vcross.y/=Length;
    Vcross.z/=Length;
    
    *len=Length;

    return  Vcross;
}
struct P p(double x,double y,double z)
{
	struct P ret;
	
	ret.x=x;
	ret.y=y;
	ret.z=z;
	
	return ret;
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

