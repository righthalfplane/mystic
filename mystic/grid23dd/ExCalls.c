#include <stdio.h>
#include "Simple.h"

#ifdef CRAY
#define vel VEL
#define post POST
#define grid GRID
#define cbeam CBEAM
#define bwrite BWRITE
#define cbrick CBRICK
#define b8write B8WRITE
#define s4write S4WRITE
#define cpress CPRESS
#define press4  PRESS4
#define cshell  CSHELL
#else
#define vel vel_
#define post post_
#define grid grid_
#define cbeam cbeam_
#define bwrite bwrite_
#define cbrick cbrick_
#define cshell cshell_
#define b8write b8write_
#define s4write s4write_
#define cpress cpress_
#define press4  press4_
#endif

#define SHELLTHICKNESS	0
#define VELOCITY	1
#define GRIDGEN		2
#define T_CBEAM		3
#define T_CBRICK	4
#define T_CPRESS	5
#define T_CSHELL	6

static char *table[]={"shellThickness","velocity","grid","cbeam",
		      "cbrick","cpress","cshell"};

#define  TABLESIZE (sizeof(table)/sizeof(char *))

static long  tableAddress[TABLESIZE];

static double  *cx,*cy,*cz;
static long *cibc,*cns,*cnee;

int setCalls(char *name);
int setEx(char *name,int (*routine)(),int count);

int adjust(int n);

extern int post();
extern int s4write();
extern int bwrite();
extern int post();
extern int press4();
extern int b8write();

int pushl(long code);

int doRun(long codein);

int fmain()
{
	struct c_list *findCalls();
	struct c_list *calls;
	double x,y,z,t;
	int k;
	long n;
	
	
	for(k=0;k<TABLESIZE;++k){
	    if((calls=findCalls(table[k]))){
	        tableAddress[k]=(long)(calls->c_long);
	    }else{
	        tableAddress[k]=0;
	    }
	}
	post();
	return 0;
}
int cshell(x,y,z,ibc,ns,nee)
double *x,*y,*z;
long *ibc,*ns,*nee;
{
	long Address;
	
	if((Address=tableAddress[T_CSHELL])){
	    cx=x;
	    cy=y;
	    cz=z;
	    cibc=ibc;
	    cns=ns;
	    cnee=nee;
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(1*sizeof(long)));
	}
	return 0;
}
int dshell()
{
	long *nfam,*imat,*i1,*j1,*k1,*i2,*j2,*k2,*i3,*j3,*k3,*i4,*j4,*k4;
	extern unsigned char *stack; /*  Run Subroutine stack Location     	*/
	
	nfam=(long *)(stack+sizeof(long));
	imat=nfam+1;
	i1=imat+1;
	j1=i1+1;
	k1=j1+1;
	i2=k1+1;
	j2=i2+1;
	k2=j2+1;
	i3=k2+1;
	j3=i3+1;
	k3=j3+1;
	i4=k3+1;
	j4=i4+1;
	k4=j4+1;
	s4write(cx,cy,cz,cibc,cns,cnee,nfam,imat,
	        i1,j1,k1,i2,j2,k2,i3,j3,k3,i4,j4,k4);
	return 0;
}
int cbeam(x,y,z,ibc,ns,nee)
double *x,*y,*z;
long *ibc,*ns,*nee;
{
	long Address;
	
	if((Address=tableAddress[T_CBEAM])){
	    cx=x;
	    cy=y;
	    cz=z;
	    cibc=ibc;
	    cns=ns;
	    cnee=nee;
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(1*sizeof(long)));
	}
	return 0;
}
int dbeam()
{
	long *nfam,*imat,*i1,*j1,*k1,*i2,*j2,*k2,*i3,*j3,*k3;
	extern unsigned char *stack; /*  Run Subroutine stack Location     	*/
	
	nfam=(long *)(stack+sizeof(long));
	imat=nfam+1;
	i1=imat+1;
	j1=i1+1;
	k1=j1+1;
	i2=k1+1;
	j2=i2+1;
	k2=j2+1;
	i3=k2+1;
	j3=i3+1;
	k3=j3+1;
	bwrite(cx,cy,cz,cibc,cns,cnee,nfam,imat,i1,j1,k1,i2,j2,k2,i3,j3,k3);
	return 0;
}
int cbrick(x,y,z,ibc,ns,nee)
double *x,*y,*z;
long *ibc,*ns,*nee;
{
	long Address;
	
	if((Address=tableAddress[T_CBRICK])){
	    cx=x;
	    cy=y;
	    cz=z;
	    cibc=ibc;
	    cns=ns;
	    cnee=nee;
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(1*sizeof(long)));
	}
	return 0;
}
int dbrick()
{
	long *nfam,*imat,*i1,*j1,*k1,*i2,*j2,*k2,*i3,*j3,*k3,*i4,*j4,*k4;
	long *i5,*j5,*k5,*i6,*j6,*k6,*i7,*j7,*k7,*i8,*j8,*k8;
	extern unsigned char *stack; /*  Run Subroutine stack Location     	*/
	
	nfam=(long *)(stack+sizeof(long));
	imat=nfam+1;
	i1=imat+1;
	j1=i1+1;
	k1=j1+1;
	i2=k1+1;
	j2=i2+1;
	k2=j2+1;
	i3=k2+1;
	j3=i3+1;
	k3=j3+1;
	i4=k3+1;
	j4=i4+1;
	k4=j4+1;
	i5=k4+1;
	j5=i5+1;
	k5=j5+1;
	i6=k5+1;
	j6=i6+1;
	k6=j6+1;
	i7=k6+1;
	j7=i7+1;
	k7=j7+1;
	i8=k7+1;
	j8=i8+1;
	k8=j8+1;
	b8write(cx,cy,cz,cibc,cns,cnee,nfam,imat,
	        i1,j1,k1,i2,j2,k2,i3,j3,k3,i4,j4,k4,
	        i5,j5,k5,i6,j6,k6,i7,j7,k7,i8,j8,k8);
	return 0;
}
int cpress(x,y,z,ibc,ns,nee)
double *x,*y,*z;
long *ibc,*ns,*nee;
{
	long Address;
	
	if((Address=tableAddress[T_CPRESS])){
	    cx=x;
	    cy=y;
	    cz=z;
	    cibc=ibc;
	    cns=ns;
	    cnee=nee;
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(1*sizeof(long)));
	}
	return 0;
}
int dpress()
{
	long *nfam,*imat,*i1,*j1,*k1,*i2,*j2,*k2,*i3,*j3,*k3,*i4,*j4,*k4;
	long *p;
	extern unsigned char *stack; /*  Run Subroutine stack Location     	*/
	
	nfam=(long *)(stack+sizeof(long));
	imat=nfam+1;
	p=imat+1;
	i1=p+1;
	j1=i1+1;
	k1=j1+1;
	i2=k1+1;
	j2=i2+1;
	k2=j2+1;
	i3=k2+1;
	j3=i3+1;
	k3=j3+1;
	i4=k3+1;
	j4=i4+1;
	k4=j4+1;
	press4(cx,cy,cz,cibc,cns,cnee,nfam,imat,*p,
	        i1,j1,k1,i2,j2,k2,i3,j3,k3,i4,j4,k4);
	return 0;
}
int grid(x,y,z,n)
double *x,*y,*z;
long *n;
{
	long Address;
	
	if((Address=tableAddress[GRIDGEN])){
	    pushl((long)n);  /*   n   */
	    pushl((long)z);  /*   z   */
	    pushl((long)y);  /*   y   */  
	    pushl((long)x);  /*   x   */
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(5*sizeof(long)));
	}
	return 0;
}
int vel(x,y,z,n)
double *x,*y,*z;
long *n;
{
	long Address;
	
	if((Address=tableAddress[VELOCITY])){
	    pushl((long)n);  /*   n   */
	    pushl((long)z);  /*   z   */
	    pushl((long)y);  /*   y   */  
	    pushl((long)x);  /*   x   */
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(5*sizeof(long)));
	}
	return 0;
}
int shellThickness(n,x,y,z,t)
long *n;
double *x,*y,*z,*t;
{
	long Address;
	
	if((Address=tableAddress[SHELLTHICKNESS])){
	    pushl((long)t);  /*   t   */
	    pushl((long)z);  /*   z   */
	    pushl((long)y);  /*   y   */  
	    pushl((long)x);  /*   x   */
	    pushl((long)n);  /*   n   */
	    pushl((long)0xFFFFFFFFL);  /*   n   */
	    doRun(Address);
	    adjust((int)(6*sizeof(long)));
	}
	return 0;
}
int setExCalls()
{
	int fmain();
	int k;
	
	setEx("fmain",fmain,1);	
	for(k=0;k<TABLESIZE;++k){
	    setCalls(table[k]);
	}
	return 0;
}	
