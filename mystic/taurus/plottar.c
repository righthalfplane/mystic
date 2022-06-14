#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "showlib.h"


#ifdef CRAY
#define bgnplt2 BGNPLT2
#define endplt2 ENDPLT2
#define node3dcf NODE3DC
#define node3ddf NODE3DD
#define node3dvf NODE3DV
#define node3def NODE3DE
#define etime	ETIME
#define lshift	LSHIFT
#define rshift	RSHIFT
#define INC	1
#else
#define bgnplt2 bgnplt2_
#define endplt2 endplt2_
#define node3dcf node3dc_
#define node3ddf node3dd_
#define node3dvf node3dv_
#define node3def node3de_
#define etime	etime_
#define lshift	lshift_
#define rshift	rshift_
#define INC	1
#endif
static showPtr show;

#define EMAX	1000000

static long *elements;
long elementCount;

long lshift(long *ib,long *i)
{
	return (*ib << *i);
}
long rshift(long *ib,long *i)
{
	return (*ib >> *i);

}
/*
float etime(float *array)
{
	long start;
	
	time(&start);
	array[0]=(float)start;
	array[1]=0;
	return array[0];
}
*/
int bgnplt2(char *name)
{
	char name2[9];
	int k;

	if(!name)return 1;

	for(k=0;k<8;++k){
	    name2[k]=name[k];
	    if(name2[k] == ' ')name2[k]=0;
	}
	name2[8]=0;

	elements=(long *)malloc(EMAX*sizeof(long));
	if(!elements){
	   printf("bgnplt2 No Element Memory\n");
	   return 1;
	}
	show=showStart(name,"w");
	if(!show)return 1;

	elementCount=0;
	
	return 0;
}
int endplt2()
{

	if(!show)return 1;


	if(elementCount){
	     showElements(show,elementCount/5,4,elements);
	}

	showEnd(show);
	showDone(show);
	show=NULL;


	free(elements);

	elements=NULL;
	elementCount=0;

 	return 0;
}
int node3dcf(char *title,float *x,long *nxx,long *type)
{
	double *fout;
	long length,n,n3,nx;
	int ret;

	if(!show)return 1;

	if(!title || !x || !nxx || !type)return 1;

	nx = *nxx;
	n3=3*(nx);
	length = (long)(n3*sizeof(double));
	if(!(fout=(double *)malloc(length))){
	    printf("node3dc Out of Memory Request (%ld)\n",length);
	    return 1;
	}
	for(n=0;n < nx;++n){
	    fout[n]=x[3*n];
	    fout[n+nx]=x[3*n+1];
	    fout[n+2*nx]=x[3*n+2];
	}

	ret=showNodes(show,nx,fout,fout+nx,fout+2*nx);


	if(fout)free(fout);

	return ret;
}
int node3ddf(char *title,float *x,long *nxx,long *type)
{
	double *fout;
	long length,n,n3,nx;
	int ret;

	if(!show)return 1;

	if(!title || !x || !nxx || !type)return 1;

	nx = *nxx;
	n3=3*(nx);
	length = (long)(n3*sizeof(double));
	if(!(fout=(double *)malloc(length))){
	    printf("node3dc Out of Memory Request (%ld)\n",length);
	    return 1;
	}
	for(n=0;n < nx;++n){
	    fout[n]=x[3*n];
	    fout[n+nx]=x[3*n+1];
	    fout[n+2*nx]=x[3*n+2];
	}

	ret=showDisplacements(show,nx,fout,fout+nx,fout+2*nx);


	if(fout)free(fout);

	return ret;
}
int node3dvf(char *title,float *x,long *nxx,long *type)
{
	double *fout;
	long length,n,nx;
	int ret;

	if(!show)return 1;

	if(!title || !x || !nxx || !type)return 1;

	nx = *nxx;

	length = (long)(nx*sizeof(double));
	if(!(fout=(double *)malloc(length))){
	    printf("node3dvf Out of Memory Request (%ld)\n",length);
	    return 1;
	}
	
	for(n=0;n < nx;++n){
	    fout[n]=x[n];
	}
	
	ret=showValues(show,nx,fout);

	if(fout)free(fout);

	return ret;
}

int node3def(char *title,int *ip,int *ncount,int *imat,int *itype)
{
	long n;

	if(!title || !ip || !ncount || !imat || !itype)return 1;

	if((elementCount+5) >= EMAX-1)return 0;


	for(n=0;n < *ncount;++n){
	    if(ip[0] < 0 || ip[1*INC] < 0 || ip[2*INC] < 0 || ip[3*INC] < 0 ){
			ip += 4*INC;
			continue;
	    }
	    elements[elementCount++]= *imat;
        elements[elementCount++]= (*ip)-1;
	    ip += INC;
        elements[elementCount++]= (*ip)-1;
	    ip += INC;
        elements[elementCount++]= (*ip)-1;
 	    ip += INC;
        elements[elementCount++]= (*ip)-1;
	    ip += INC;
	}

	return 0;
}
