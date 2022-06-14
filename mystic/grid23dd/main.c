#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct P{
    double x;
    double y;
    double z;
};

static int vec3();

 
struct P Sub(),Add(),Norm(),CrossN(),Cross(),Mult();
double Dot(),Distance(),Len();

int clearEx();

int Usage();

int setExCalls();

int doFile(char *Name,int argc,char *argv[]);

int main(int argc,char *argv[])
{
	int k;
	
#ifdef THINK_C
#include <console.h>
 	argc = ccommand (&argv);
       MacStart();
	
#endif
	
	if(argc < 2)Usage();

	setExCalls();
	
	for(k=1;k<argc;++k){
	    if(!strcmp(argv[k],"-p")){
	        k += 1;
		/* maxprims=atol(argv[k]);
	        printf("maxprims %ld\n",maxprims); */
	    }else{
	        if(doFile(argv[k],argc-k,&argv[k]))exit(1);
	    }
	}
	clearEx();
	return 0;
}
int Usage()
{
	printf("Usage : SaicTracer [-w] [-p maxprim] Test.t\n");
	exit(1);
	return 0;
}
int zerol(s,n)
char *s;
long n;
{
	while(n-- > 0)*s++ = 0;
	return 0;
}
 
struct P Add(p1,p2)
struct P *p1,*p2;
{
        struct P padd;
 
        padd.x=p1->x+p2->x;
        padd.y=p1->y+p2->y;
        padd.z=p1->z+p2->z;
        return padd;
 
}
struct P Mult(p1,Factor)
struct P *p1;
double Factor;
{
        struct P p;
 
        p.x=p1->x*Factor;
        p.y=p1->y*Factor;
        p.z=p1->z*Factor;
        return p;
}
struct P Norm(p1)
struct P *p1;
{
        struct P p;
        double sum;
 
        sum=1./pow(p1->x*p1->x+p1->y*p1->y+p1->z*p1->z,.5);
        p.x=p1->x*sum;
        p.y=p1->y*sum;
        p.z=p1->z*sum;
        return p;
}
double Dot(v1,v2)
struct P *v1,*v2;
{
        double Length;
 
        Length=(v1->x*v2->x)+(v1->y*v2->y)+(v1->z*v2->z);
 
        return  Length;
}
double Distance(v1,v2)
struct P *v1,*v2;
{
        double Length;
 
        Length=pow((v1->x-v2->x)*(v1->x-v2->x)+
               (v1->y-v2->y)*(v1->y-v2->y)+
               (v1->z-v2->z)*(v1->z-v2->z),.5);
 
        return  Length;
}
double Len(v1)
struct P *v1;
{
        double Lenr;
 
        Lenr=pow((v1->x)*(v1->x)+
                (v1->y)*(v1->y)+
                (v1->z)*(v1->z),.5);
 
        return  Lenr;
}
struct P p(x,y,z)
double x,y,z;
{
	struct P ret;
	
	ret.x=x;
	ret.y=y;
	ret.z=z;
	return ret;
}
int getnorm(p1,p2,p3,n)
struct P *p1,*p2,*p3,*n;
{
	struct P v1,v2;
	double x,y,z,Length;
	
	v1.x=p2->x-p1->x;
	v1.y=p2->y-p1->y;
	v1.z=p2->z-p1->z;
	v2.x=p3->x-p1->x;
	v2.y=p3->y-p1->y;
	v2.z=p3->z-p1->z;
        x=v1.y*v2.z-v2.y*v1.z;
        y=-(v1.x*v2.z-v2.x*v1.z);
        z=v1.x*v2.y-v2.x*v1.y;
        Length=sqrt(x*x+y*y+z*z);
        if(Length <= 0.){
            n->x=0;
            n->y=0;
            n->z=1;
            return 1;
        }
        n->x=x/Length;
        n->y=y/Length;
        n->z=z/Length;
        return 0;
}
